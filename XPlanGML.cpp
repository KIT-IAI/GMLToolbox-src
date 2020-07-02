#include "stdafx.h"

#include <vector>
#include <map>
#include <string>
#include <set>

#include "Convert.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "Geometrie.h"
#include "GmlSchema.h"
#include "UnitOfMeasure.h"
#include "includes.h"
#include "constant.h"
#include "Logfile.h"
#include "ShpFileNamenKonv.h"
#include "ShpFileObjekte.h"
#include "shpfilereader.h"
#include "SvgVisualisierung.h"
#include "KonvDateiReader.h"
#include "TimeX.h"
#include "Migration.h"
#include "GMLReader.h"
#include "GmlWriter.h"
#include "KonversionINSPIRE_PLU.h"
#include "ObjectDataWriter.h"
#include "XPlanGML.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  class XPlanGML                                                           //
//  Verarbeitung von 2D-GML Daten (XPlanGML, INSPIRE-PLU, ALKIS              //
///////////////////////////////////////////////////////////////////////////////
XPlanGML::XPlanGML ( Features * pFeatures, UOMList * pUOMList, Logfile * pLogfile )
{
  m_pFeatures = pFeatures;
  m_pUOMs     = pUOMList;
  m_pLogfile  = pLogfile;

   m_anzObjRead    = 0;
   m_anzObjNotRead = 0;

  m_pShapeFileNamenKonvertierung = NULL;
  m_pShapeFileObjekte            = NULL;
  m_pBLeitPlan	   = NULL;
  m_pPlanAttribute = NULL;

   m_readBereichGeometrie   = false;
   m_bereichNummer          = 0;
   m_plaeneAusShapeErzeugen = false;

   m_planGMLId		= "BPlan_0";
   m_planTyp = KEIN_XPLAN;
}

XPlanGML::~XPlanGML()
{
  if ( m_pShapeFileNamenKonvertierung != NULL ) delete m_pShapeFileNamenKonvertierung;
  if (  m_pShapeFileObjekte != NULL )			      delete  m_pShapeFileObjekte;
  if (  m_pPlanAttribute != NULL )			        delete  m_pPlanAttribute;

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Typ des Plans													                         //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::setPlanTyp ( XPLAN_TYP planTypP )
{
   m_planTyp	= planTypP;

  switch (  m_planTyp )
  {
  case BPLAN:
     m_planKlassenName		   = "xplan:BP_Plan";
     m_planBereichKlassenName = "xplan:BP_Bereich";
    break;

  case FPLAN:
     m_planKlassenName        = "xplan:FP_Plan";
     m_planBereichKlassenName = "xplan:FP_Bereich";
    break;

  case RPLAN:
     m_planKlassenName        = "xplan:RP_Plan";
     m_planBereichKlassenName = "xplan:RP_Bereich";
    break;

  case LPLAN:
     m_planKlassenName        = "xplan:LP_Plan";
     m_planBereichKlassenName = "xplan:LP_Bereich";
    break;

  case SOPLAN:
     m_planKlassenName        = "xplan:SO_Plan";
     m_planBereichKlassenName = "xplan:SO_Bereich";
    break;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Transformation Plan-Typ (Enumeration) --> Plan-Typ (Text)							   //
///////////////////////////////////////////////////////////////////////////////
std::string XPlanGML::planTypToString (  XPLAN_TYP planTypP )
{
  switch ( planTypP )
  {
  case BPLAN:
    return "BPlan";

  case FPLAN:
    return "FPlan";

  case RPLAN:
    return "RPlan";

  case LPLAN:
    return "LPlan";

  case SOPLAN:
    return "SOPlan";
  }

  return "KeinXPlan";
}

///////////////////////////////////////////////////////////////////////////////
//  Transformation Plan-Typ (Text) --> Plan-Typ (Enumeration)							   //
///////////////////////////////////////////////////////////////////////////////
XPLAN_TYP XPlanGML::planStringToTyp ( std::string planTypP )
{
  if ( planTypP == "BPlan" ) return BPLAN;
  else
  if ( planTypP == "FPlan" ) return FPLAN;
  else
  if ( planTypP == "RPlan" ) return RPLAN;
  else
  if ( planTypP == "LPlan" ) return LPLAN;
  else
  if ( planTypP == "SOlan" ) return SOPLAN;
  else                       return KEIN_XPLAN;
}

///////////////////////////////////////////////////////////////////////////////
// Liefert die zu einen Plantyp gehörige XPlanGML-Klasse                     //
///////////////////////////////////////////////////////////////////////////////
GmlKlasse * XPlanGML::getGmlPlanKlasse ()
{
  switch (  m_planTyp )
  {
  case BPLAN:
    return m_pFeatures->getGmlSchema()->getGmlKlasse ( "xplan:BP_PlanType" );

  case FPLAN:
    return m_pFeatures->getGmlSchema()->getGmlKlasse ( "xplan:FP_PlanType" );

  case RPLAN:
    return m_pFeatures->getGmlSchema()->getGmlKlasse ( "xplan:RP_PlanType" );

  case LPLAN:
    return m_pFeatures->getGmlSchema()->getGmlKlasse ( "xplan:LP_PlanType" );

  case SOPLAN:
    return m_pFeatures->getGmlSchema()->getGmlKlasse ( "xplan:SO_PlanType" );
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine GML-ID für Features, die bei der Shapefile-Transformation //
//  erzeugt werden                                                           //
///////////////////////////////////////////////////////////////////////////////
string XPlanGML::createGmlIdFromShapefile ( string shapefileName, int FID )
{
  char   buffer[6];
  string id;
  string shapefileNamePlain;

  _itoa_s ( FID, buffer, 5, 10 );

  shapefileNamePlain = QuConvert::ToAscii( shapefileName );
  id = shapefileNamePlain + "-FID-" + string ( buffer );
  return id;
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt neue Objekt pPlanAttribute										                   //
///////////////////////////////////////////////////////////////////////////////
PlanAttribute * XPlanGML::createPlanAttribute()
{
  if (  m_pPlanAttribute != NULL ) delete  m_pPlanAttribute;
   m_pPlanAttribute = new PlanAttribute;

  return  m_pPlanAttribute;
}

///////////////////////////////////////////////////////////////////////////////
//  Verwaltung von Referenzen währen der Konversion							             //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::addPlanObjektKonversion ( std::string uuidP, Feature * pPlanObjekt )
{
  m_mPlanObjekteKonversion[uuidP] = pPlanObjekt;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein referiertes Feature auf Basis einer uuid										 //
///////////////////////////////////////////////////////////////////////////////
Feature * XPlanGML::getPlanObjektKonversion ( std::string uuidP )
{
  map<string,Feature*>::iterator iter;

  iter = m_mPlanObjekteKonversion.find ( uuidP );
  if ( iter != m_mPlanObjekteKonversion.end() )
    return iter->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht die Zuordnung uuid - Feature für referenzierte Features					 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::clearPlanObjekteKonversion()
{
  m_mPlanObjekteKonversion.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Auflösung externer Referenzen																				     //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::resolveReferencesKonversion()
{
  vector<Feature*>         planObjekte;
  string                   gmlId;
  vector<string>					 stringAttributWerte;
  string							     stringAttributWert;
  vector<FeatureRelation*> vObjRelationen;
  Feature                * pPlanObjekt;
  Feature						     * pTextlicheFestsetzung;
  Feature                 * pReferiertesPlanObjekt;
  FeatureRelation         * pRelation;
	size_t                   i, j, anz, objAnz;

  objAnz = m_pFeatures->getAllFeatures ( planObjekte );
  for ( j = 0; j < objAnz; j++ )
  {
    pPlanObjekt = planObjekte[j];

    if ( pPlanObjekt->getObjektTyp() == BLEIT_PLAN ||
      pPlanObjekt->getObjektTyp() == PLAN_BEREICH ) continue;

    if ( m_pFeatures->getGmlTyp() >= XPLANGML_3_0 )
    {
      anz = pPlanObjekt->getStringAttributWerte ( "xplan:textSchluessel", stringAttributWerte );
      if ( anz > 0 )
      {
        for ( i = 0; i < anz; i++ )
        {
          stringAttributWert = stringAttributWerte[i];
          pTextlicheFestsetzung = m_pBLeitPlan->getTextlicheFestsetzung ( stringAttributWert );
          if ( pTextlicheFestsetzung != NULL )
          {
            pRelation = new FeatureRelation ();
            pRelation->rollenName = "xplan:refTextInhalt";
            pRelation->featureId = pTextlicheFestsetzung->getGmlId();
            pPlanObjekt->addFeatureRelation ( pRelation );
          }
        }
        pPlanObjekt->deleteStringAttribut ( "xplan:textSchluessel" );
      }

      stringAttributWerte.clear();

      anz = pPlanObjekt->getStringAttributWerte ( "xplan:textSchluesselBegruendung", stringAttributWerte );
      if ( anz > 0 )
      {
        for ( i = 0; i < anz; i++ )
        {
          stringAttributWert = stringAttributWerte[i];
          pTextlicheFestsetzung = m_pBLeitPlan->getTextlicheBegruendung ( stringAttributWert );
          if ( pTextlicheFestsetzung != NULL )
          {
            pRelation = new FeatureRelation ();
            pRelation->rollenName = "xplan:refBegruendungInhalt";
            pRelation->featureId = pTextlicheFestsetzung->getGmlId();
            pPlanObjekt->addFeatureRelation ( pRelation );
          }
        }
        pPlanObjekt->deleteStringAttribut ( "xplan:textSchluesselBegruendung" );
      }
    }

    vObjRelationen.clear();
    anz = pPlanObjekt->getFeatureRelationen( vObjRelationen );
    for ( i = 0; i < anz; i++ )
    {
      pRelation = vObjRelationen[i];
      if ( pRelation->uuid != "" )
      {
        pReferiertesPlanObjekt = getPlanObjektKonversion ( pRelation->uuid );
        if ( pReferiertesPlanObjekt != NULL )
        {
          gmlId = pReferiertesPlanObjekt->getGmlId();
          pRelation->featureId = gmlId;

					if ( pRelation->rollenName == "xplan:dientZurDarstellungVon" )
					{
						FeatureRelation * pRelInvers = new FeatureRelation;
						pRelInvers->rollenName = "xplan:wirdDargestelltDurch";
						pRelInvers->featureId = pPlanObjekt->getGmlId();
						pReferiertesPlanObjekt->addFeatureRelation ( pRelInvers );
					}
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Übergibt einen Vektor zu konvertierender Shapefiles						           //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::setShapeFiles ( std::vector<std::string> shapeFilesP )
{
	size_t  i, index;
  string   pathName, fileName;

  m_vShapeFilePaths.clear();
  m_vShapeFileNames.clear();

  for ( i = 0; i < shapeFilesP.size(); i++ )
  {
    pathName = shapeFilesP[i];
    m_vShapeFilePaths.push_back ( pathName );

    index = pathName.find_last_of ( "\\" );
    fileName = pathName.substr ( index+1 );
    m_vShapeFileNames.push_back ( fileName );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine neue Konversion aus pShapeFileObjekte					           //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::updateConversion()
{
  if ( m_pShapeFileObjekte == NULL ) return false;

  ShapeFileNamenKonvertierung * pKonvNew = m_pShapeFileObjekte->generateConversion();
  if ( pKonvNew == NULL ) return false;

  if ( m_pShapeFileNamenKonvertierung != NULL ) delete m_pShapeFileNamenKonvertierung;
  m_pShapeFileNamenKonvertierung = pKonvNew;
  m_plaeneAusShapeErzeugen = m_pShapeFileNamenKonvertierung->getPlaeneErzeugen();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Spezifiziert eine neue Konversions-Datei								                 //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::setConversionFile ( std::string dateiP )
{
  if ( m_pShapeFileNamenKonvertierung != NULL && m_konvertierungsDatei != dateiP )
  {
    delete m_pShapeFileNamenKonvertierung;
    m_pShapeFileNamenKonvertierung = NULL;
  }

  m_pLogfile->writeLogFile( "" );
  m_pLogfile->writeLogFile( "Einlesen einer Konvertierungs-Datei" );

  KonvDateiReader ^ pReader = gcnew KonvDateiReader ( this );
  m_pShapeFileNamenKonvertierung = pReader->read ( dateiP );
  delete pReader;

  if ( m_pShapeFileNamenKonvertierung != NULL )
  {
    m_konvertierungsDatei = dateiP;
    m_plaeneAusShapeErzeugen = m_pShapeFileNamenKonvertierung->getPlaeneErzeugen();
    return true;
  }
  else return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Namen der Konversions-Datei									                 //
///////////////////////////////////////////////////////////////////////////////
std::string XPlanGML::getConversionFile ()
{
  return m_konvertierungsDatei;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert, ob eine Konvertierung existiert								                 //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::existConversion()
{
  if ( m_pShapeFileNamenKonvertierung != NULL ) return true;
  else										                      return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Speichert eine Konvertierung											                       //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::saveConversion()
{
  if ( m_konvertierungsDatei == "" ) return;

  KonvDateiWriter ^ pWriter = gcnew KonvDateiWriter ( this );
  pWriter->write ( m_konvertierungsDatei );

  delete pWriter;
}

///////////////////////////////////////////////////////////////////////////////
//  Speichert eine Konvertierung unter einem neuen Namen				          	 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::saveConversionAs ( std::string dateiP )
{
  KonvDateiWriter ^ pWriter = gcnew KonvDateiWriter ( this );
  pWriter->write ( dateiP );

  m_konvertierungsDatei = dateiP;
  delete pWriter;
}

///////////////////////////////////////////////////////////////////////////////
//  Alalysiert die spezifizierten Shapefiles und baut die Struktur			     //
//  ShapeFileObjekte auf													                           //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::analyzeShapeFiles( int &anzErrors )
{
  unsigned int i;
  string       shapeFilePath;
  bool         success;

	anzErrors = 0;

  if ( m_vShapeFilePaths.size() == 0 ) return false;
  m_shapeFileMode = ANALYSE;

  if ( m_pShapeFileObjekte != NULL ) delete m_pShapeFileObjekte;
  m_pShapeFileObjekte = new ShapeFileObjekte ( this );

  for ( i = 0; i < m_vShapeFilePaths.size(); i++ )
  {
    shapeFilePath = m_vShapeFilePaths[i];
    success = analyzeShapeFile ( shapeFilePath );
		if ( !success )
			anzErrors++;
  }

	if ( anzErrors > 0 )
		return false;
	else
		return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Analyse eines Shape-Files												                         //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::analyzeShapeFile ( string shapeFilePathP )
{
  m_vObjekteInShapeFile.clear();
  SHPFileReader* pShpFileReader = new SHPFileReader ( this, shapeFilePathP );
  bool success = pShpFileReader->read_from_files();
  delete pShpFileReader;

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertiert Shape-Files													                         //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::convertShapeFiles()
{
  string       logFileName, shapeFilePath, schemaVersion;
  string       shapeFileName, klassenName;
	size_t       i, index;
  bool         success;
  vector<PlanBereich*> bereiche;

  if ( m_vShapeFilePaths.size() == 0 ) return false;

  m_shapeFileMode = KONVERSION;

  clearPlanObjekteKonversion();
  m_pFeatures->setSrsDimension ( 2 );

  m_anzObjRead			= 0;
  m_anzObjNotRead		= 0;
  m_anzShapeFileRead	= 0;
  m_anzShapeFileNotRead = 0;

  m_pLogfile->writeLogFile ( "" );
  m_pLogfile->writeLogFile ( "Konversion von Shape-Dateien - Start" );

  if ( m_plaeneAusShapeErzeugen == false )
  {
    m_planGMLId =  Constant::createGmlId ();
    m_pBLeitPlan = new BLeitPlan ( m_pFeatures, m_planGMLId, m_planKlassenName );
    m_pFeatures->addFeature ( m_pBLeitPlan );
    setPlanAttribute();
  }

  for ( i = 0; i < m_vShapeFileNames.size(); i++ )
  {
    shapeFilePath = m_vShapeFilePaths[i];
    shapeFileName = m_vShapeFileNames[i];
    index = shapeFileName.find ( "." );
    if ( index > 0 ) shapeFileName = shapeFileName.substr ( 0, index );

    klassenName = m_pShapeFileNamenKonvertierung->getKlassenName ( shapeFileName );
    switch ( m_planTyp )
    {
    case BPLAN:
      if ( klassenName == "xplan:BP_Plan" )break;
      else						                     continue;

    case FPLAN:
      if ( klassenName == "xplan:FP_Plan" )break;
      else						                     continue;

    case LPLAN:
      if ( klassenName == "xplan:LP_Plan" )break;
      else						                     continue;

    case RPLAN:
      if ( klassenName == "xplan:RP_Plan" )break;
      else						                     continue;

    case SOPLAN:
      if ( klassenName == "xplan:SO_Plan" )break;
      else						                    continue;

    default:
      continue;
    }

    success = readShapeFile ( shapeFilePath );
    if ( success ) m_anzShapeFileRead++;
    else		       m_anzShapeFileNotRead++;
  }

  m_pFeatures->createPlanMap();


  for ( i = 0; i < m_vShapeFilePaths.size(); i++ )
  {
    shapeFilePath = m_vShapeFilePaths[i];
    shapeFileName = m_vShapeFileNames[i];
    index = shapeFileName.find ( "." );
    if ( index > 0 ) shapeFileName = shapeFileName.substr ( 0, index );

    klassenName = m_pShapeFileNamenKonvertierung->getKlassenName ( shapeFileName );

    switch ( m_planTyp )
    {
    case BPLAN:
      if ( klassenName == "xplan:BP_Plan" ) continue;
      else							                    break;

    case FPLAN:
      if ( klassenName == "xplan:FP_Plan" ) continue;
      else						                break;

    case LPLAN:
      if ( klassenName == "xplan:LP_Plan" ) continue;
      else						                break;

    case RPLAN:
      if ( klassenName == "xplan:RP_Plan" ) continue;
      else						                break;

    case SOPLAN:
      if ( klassenName == "xplan:SO_Plan" ) continue;
      else						                break;

    default:
      break;
    }

    success = readShapeFile ( shapeFilePath );
    if ( success ) m_anzShapeFileRead++;
    else		       m_anzShapeFileNotRead++;
  }

  resolveReferencesKonversion();

  if ( m_plaeneAusShapeErzeugen == false )
  {
    success = m_pBLeitPlan->getStringAttributWert ( "xplan:xPlanGMLVersion", schemaVersion );
    if ( !success )
    {
      schemaVersion = m_pFeatures->getGmlTypAsString();
      m_pBLeitPlan->setStringAttribut ( "xplan:xPlanGMLVersion", schemaVersion );
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Übernimmt die generellen Attribute des Plans aus der Konvertierungs-Datei//
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::setPlanAttribute ()
{
	size_t            anz, anzBereiche, i, j;
  bool              found;
  string            gmlId;
  string            stdString;
  string            name;
  string            wert;
  string			      rollenName;
  FeatureRelation * pRel;
  Feature         * pObj;
  Feature         * pKomplexAttribut;
  PlanBereich     * pPlanBereich;
  GmlSchema      * pGmlObjekte;

  vector<string>			       attributNamen;
  vector<string>			       stringAttributWerte;
  vector<int>				         intAttributWerte;
  vector<double>			       doubleAttributWerte;
  vector<bool>			         boolAttributWerte;
  vector<TextAbschnitt*>     textAbschnitte;
  vector<Bereich*>           bereiche;
  vector<Feature*>           komplexAttribute;
  vector<ExterneReferenz*>   externeReferenzen;

  if ( m_pPlanAttribute == NULL ) return false;

  pGmlObjekte = m_pFeatures->getGmlSchema();

  anz = m_pPlanAttribute->getIntegerAttributWerte ( attributNamen, intAttributWerte );
  for ( i = 0; i < anz; i++ )
    m_pBLeitPlan->setIntegerAttribut ( attributNamen[i], intAttributWerte[i] );

  attributNamen.clear();
  anz = m_pPlanAttribute->getStringAttributWerte ( attributNamen, stringAttributWerte );
  for ( i = 0; i < anz; i++ )
    m_pBLeitPlan->setStringAttribut ( attributNamen[i], stringAttributWerte[i] );

  attributNamen.clear();
  anz = m_pPlanAttribute->getDoubleAttributWerte ( attributNamen, doubleAttributWerte );
  for ( i = 0; i < anz; i++ )
    m_pBLeitPlan->setDoubleAttribut ( attributNamen[i], doubleAttributWerte[i] );

  attributNamen.clear();
  anz = m_pPlanAttribute->getBoolAttributWerte ( attributNamen, boolAttributWerte );
  for ( i = 0; i < anz; i++ )
    m_pBLeitPlan->setBoolAttribut ( attributNamen[i], boolAttributWerte[i] );

  attributNamen.clear();
  stringAttributWerte.clear();
  anz = m_pPlanAttribute->getDateAttributWerte ( attributNamen, stringAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    TimePosition * pDateValue = new TimePosition();
    pDateValue->fromXMLString( stringAttributWerte[i] );
    DateAttr * pDate = new DateAttr;
    pDate->value = pDateValue;
    m_pBLeitPlan->setDateAttribut ( attributNamen[i], pDate );
  }

  attributNamen.clear();
  stringAttributWerte.clear();
  anz = m_pPlanAttribute->getURLAttributWerte ( attributNamen, stringAttributWerte );
  for ( i = 0; i < anz; i++ )
    m_pBLeitPlan->setURLAttribut ( attributNamen[i], stringAttributWerte[i] );

  attributNamen.clear();
  anz = m_pPlanAttribute->getKomplexAttributWerte ( attributNamen, komplexAttribute );
  for ( i = 0; i < anz; i++ )
  {
    gmlId =  Constant::createGmlId();
    pKomplexAttribut = new Feature ( komplexAttribute[i], gmlId ); 
    m_pFeatures->addFeature ( pKomplexAttribut );
    pKomplexAttribut->setParentId ( m_pBLeitPlan->getGmlId() );
    m_pBLeitPlan->addChild( pKomplexAttribut->getGmlId() );

    pRel = new FeatureRelation;
    pRel->featureId = gmlId;
    if ( attributNamen[i] == "hatGenerStringAttribut"  || 
         attributNamen[i] == "hatGenerDoubleAttribut"  ||
         attributNamen[i] == "hatGenerIntegerAttribut" ||
         attributNamen[i] == "hatGenerDatumAttribut"   ||
         attributNamen[i] == "hatGenerURLAttribut"       )
      rollenName = "xplan:hatGenerAttribut";
    else
      rollenName = attributNamen[i];
    pRel->rollenName = rollenName;
    m_pBLeitPlan->addFeatureRelation ( pRel );
  }

  anz = m_pPlanAttribute->getTextAbschnitte ( textAbschnitte );
  for ( i = 0; i < anz; i++ )
  {
		string klassenName;
		string rechtscharakter;

    gmlId =  Constant::createGmlId();

		if ( textAbschnitte[i]->planTyp =="BPlan"	)
		{
			klassenName = "xplan:BP_TextAbschnitt";
			rechtscharakter = "1000";
		}
		else
		if ( textAbschnitte[i]->planTyp =="FPlan"	)
		{
			klassenName = "xplan:FP_TextAbschnitt";
			rechtscharakter = "1000";
		}
		else
		if ( textAbschnitte[i]->planTyp =="LPlan"	)
		{
			klassenName = "xplan:LP_TextAbschnitt";
			rechtscharakter = "1000";
		}
		else
		if ( textAbschnitte[i]->planTyp =="RPlan"	)
		{
			klassenName = "xplan:RP_TextAbschnitt";
			rechtscharakter = "1000";
		}
		else
		if ( textAbschnitte[i]->planTyp =="SOPlan"	)
		{
			klassenName = "xplan:SO_TextAbschnitt";
			rechtscharakter = "2000";
		}

    pObj = new Feature ( ANY_OBJEKT, gmlId, m_pFeatures, klassenName ); 
		pObj->setStringAttribut ( "xplan:rechtscharakter", rechtscharakter );


    stdString = textAbschnitte[i]->schluessel;
    pObj->setStringAttribut ( "xplan:schluessel", stdString );

    stdString = textAbschnitte[i]->text; 
    if ( stdString != "" ) pObj->setStringAttribut ( "xplan:text", stdString );

    stdString = textAbschnitte[i]->textURL; 
    if ( stdString != "" ) pObj->setStringAttribut ( "xplan:textURL", stdString );

    m_pFeatures->addFeature ( pObj );

    pRel = new FeatureRelation;
    pRel->featureId = gmlId;
    pRel->rollenName = "xplan:texte";
    m_pBLeitPlan->addFeatureRelation ( pRel );
  }
  m_pBLeitPlan->createTextMaps();

  m_readBereichGeometrie = true;

  anzBereiche = m_pPlanAttribute->getBereiche ( bereiche );
  for ( i = 0; i < anzBereiche; i++ )
  {
    gmlId =  Constant::createGmlId();

    Bereich * pBereich = bereiche[i];
    pPlanBereich = new PlanBereich ( m_pFeatures, gmlId, m_planBereichKlassenName );

    int bereichNummer = pBereich->nummer;
    pPlanBereich->setIntegerAttribut ( "xplan:nummer", bereichNummer );

    m_pFeatures->addFeature ( pPlanBereich );
    m_pBLeitPlan->addPlanBereich ( gmlId );

    stdString = pBereich->name;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:name", stdString );

    if ( m_pFeatures->getGmlTyp() == XPLANGML_2_0 )
      setBereichAttribute_V2 ( pPlanBereich, pBereich );
    else
    if ( m_pFeatures->getGmlTyp() == XPLANGML_3_0 )
      setBereichAttribute_V3 ( pPlanBereich, pBereich );
    else
    if ( m_pFeatures->getGmlTyp() == XPLANGML_4_0 || m_pFeatures->getGmlTyp() == XPLANGML_4_1  )
      setBereichAttribute_V4 ( pPlanBereich, pBereich );
    else
    if ( m_pFeatures->getGmlTyp() == XPLANGML_5_0 )
      setBereichAttribute_V5_0 ( pPlanBereich, pBereich );
    else
    if ( m_pFeatures->getGmlTyp() == XPLANGML_5_1 || m_pFeatures->getGmlTyp() == XPLANGML_5_2 || 
         m_pFeatures->getGmlTyp() == XPLANGML_5_3 )
      setBereichAttribute_V5_X ( pPlanBereich, pBereich );

    pRel = new FeatureRelation;
    pRel->rollenName    = "xplan:gehoertZuPlan";
    pRel->featureId = m_pBLeitPlan->getGmlId();
    pPlanBereich->addFeatureRelation ( pRel );

    if ( pBereich->geometrieDatei != "" )
    {
      string shapeFilePathBereich;
      string shapeFileName;
      string geometrieDatei;

      string klassenName = m_planBereichKlassenName;

      geometrieDatei = pBereich->geometrieDatei + ".shp";
      found = false;
      for ( j = 0; j < m_vShapeFileNames.size(); j++ )
      {
        shapeFileName = m_vShapeFileNames[j];
        if ( shapeFileName == geometrieDatei )
        {
          shapeFilePathBereich = m_vShapeFilePaths[j];
          m_pKlassenName = m_pShapeFileNamenKonvertierung->getKlassenNameObj( pBereich->geometrieDatei );
          m_klassenName = m_pKlassenName->getKlassenName();

          found = true;
          break;
        }
      }
      if ( found )
        readShapeFile ( shapeFilePathBereich );
    }
  }
  m_readBereichGeometrie = false;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Plan-Bereich Attribute nach der spez. Syntax von XPlanGML 2.0  //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::setBereichAttribute_V2 ( PlanBereich * pPlanBereich, Bereich * pBereich )
{
  string            stdString;
  string            gmlId;
  Feature         * pObj;
  FeatureRelation * pRel;

  stdString = pBereich->bedeutung;
  if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:bedeutung", stdString );

  RasterplanBasis * pRasterplanBasis = pBereich->getRasterplanBasis();
  if ( pRasterplanBasis != NULL )
  {
    gmlId =  Constant::createGmlId();
    pObj = new Feature ( RASTER_BASIS, gmlId, m_pFeatures, "xplan:XP_RasterplanBasis" );

    m_pFeatures->addFeature ( pObj );
    pPlanBereich->addObjekt ( pObj );
    pObj->setBereichId ( pPlanBereich->getGmlId() );

    pRel = new FeatureRelation;
    pRel->featureId = gmlId;
    pRel->rollenName = "xplan:rasterBasis";
    pPlanBereich->addFeatureRelation ( pRel );
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Plan-Bereich Attribute nach der spez. Syntax von XPlanGML 3.0  //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::setBereichAttribute_V3 ( PlanBereich * pPlanBereich, Bereich * pBereich )
{
  string                   stdString;
  string                   gmlId;
  Feature                * pObj;
  FeatureRelation        * pRel;
	size_t                   j, anz;
  ExterneReferenz        * pExterneReferenz;
  Feature                * pObjExterneReferenz;
  vector<ExterneReferenz*> externeReferenzen;

  stdString = pBereich->bedeutungEnum;
  if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:bedeutung", stdString );

  stdString = pBereich->bedeutung;
  if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:sonstigeBedeutung", stdString );

  if ( m_planTyp == BPLAN || m_planTyp == FPLAN )
  {
    stdString = pBereich->versionBauNVOText;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionBauNVO", stdString );
  }

  RasterplanBasis * pRasterplanBasis = pBereich->getRasterplanBasis();
  if ( pRasterplanBasis != NULL )
  {
    gmlId =  Constant::createGmlId();
    pObj = new Feature ( RASTER_BASIS, gmlId, m_pFeatures, "xplan:XP_RasterplanBasis" );

    m_pFeatures->addFeature ( pObj );
    pPlanBereich->addObjekt ( pObj );
    pObj->setBereichId ( pPlanBereich->getGmlId() );

    pRel = new FeatureRelation;
    pRel->featureId = gmlId;
    pRel->rollenName = "xplan:rasterBasis";
    pPlanBereich->addFeatureRelation ( pRel );

    anz = pRasterplanBasis->getExterneReferenzen( externeReferenzen );
    for ( j = 0; j < anz; j++ )
    {
      gmlId =  Constant::createGmlId();
      pExterneReferenz = externeReferenzen[j];

      if ( pExterneReferenz->relationName == "xplan:refScan" )
        pObjExterneReferenz = new Feature ( ANY_OBJEKT, gmlId, m_pFeatures, "xplan:XP_ExterneReferenzPlan" );
      else
        pObjExterneReferenz = new Feature ( ANY_OBJEKT, gmlId, m_pFeatures, "xplan:XP_ExterneReferenz" );

      if ( pExterneReferenz->informationssystemURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:informationssystemURL", pExterneReferenz->informationssystemURL );
      if ( pExterneReferenz->referenzName != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:referenzName", pExterneReferenz->referenzName );
      if ( pExterneReferenz->referenzURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:referenzURL", pExterneReferenz->referenzURL );
      if ( pExterneReferenz->referenzMimeType != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:referenzMimeType", pExterneReferenz->referenzMimeType );
      if ( pExterneReferenz->georefURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:georefURL", pExterneReferenz->georefURL );
      if ( pExterneReferenz->georefMimeType != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:georefMimeType", pExterneReferenz->georefMimeType );
      if ( pExterneReferenz->beschreibung != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:beschreibung", pExterneReferenz->beschreibung );

      m_pFeatures->addFeature ( pObjExterneReferenz );
      pRel = new FeatureRelation;
      pRel->featureId = gmlId;
      pRel->rollenName = pExterneReferenz->relationName;
      pObj->addFeatureRelation ( pRel );
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Plan-Bereich Attribute nach der spez. Syntax von               //
//  XPlanGML 4.0 und 4.1                                                     //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::setBereichAttribute_V4 ( PlanBereich * pPlanBereich, Bereich * pBereich )
{
  string                   stdString;
  string                   gmlId;
  Feature                * pObj;
  FeatureRelation        * pRel;
	size_t                   j, anz;
  ExterneReferenz        * pExterneReferenz;
  Feature                * pObjExterneReferenz;
  vector<ExterneReferenz*> externeReferenzen;

  stdString = pBereich->bedeutungEnum;
  if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:bedeutung", stdString );

  stdString = pBereich->bedeutung;
  if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:sonstigeBedeutung", stdString );

  if ( m_planTyp == BPLAN || m_planTyp == FPLAN )
  {
    stdString = pBereich->versionBauNVOText;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionBauNVOText", stdString );

    stdString = pBereich->versionBauGBText;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionBauGBText", stdString );

    stdString = pBereich->versionBauNVO;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionBauNVO", stdString );

    stdString = pBereich->versionBauGBDatum;
    if ( stdString != "" )
    {
      TimePosition * pTimeValue = new TimePosition;
      pTimeValue->fromXMLString( stdString );
      DateAttr * pTime = new DateAttr;
      pTime->value = pTimeValue;
      pPlanBereich->setDateAttribut ( "xplan:versionBauGB", pTime );
    }
  }
  else
  if ( m_planTyp == RPLAN )
  {
    stdString = pBereich->versionBROGText;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionBROGText", stdString );
    stdString = pBereich->versionBROGDatum;
    if ( stdString != "" )
    {
      TimePosition * pTimeValue = new TimePosition;
      pTimeValue->fromXMLString( stdString );
      DateAttr * pTime = new DateAttr;
      pTime->value = pTimeValue;
      pPlanBereich->setDateAttribut ( "xplan:versionBROG", pTime );
    }
    stdString = pBereich->versionLPLGText;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionLPLGText", stdString );
    stdString = pBereich->versionLPLGDatum;
    if ( stdString != "" )
    {
      TimePosition * pTimeValue = new TimePosition;
      pTimeValue->fromXMLString( stdString );
      DateAttr * pTime = new DateAttr;
      pTime->value = pTimeValue;
      pPlanBereich->setDateAttribut ( "xplan:versionLPLG", pTime );
    }
  }

  RasterplanBasis * pRasterplanBasis = pBereich->getRasterplanBasis();
  if ( pRasterplanBasis != NULL )
  {
    gmlId =  Constant::createGmlId();
    pObj = new Feature ( RASTER_BASIS, gmlId, m_pFeatures, "xplan:XP_RasterplanBasis" );

    m_pFeatures->addFeature ( pObj );
    pPlanBereich->addObjekt ( pObj );
    pObj->setBereichId ( pPlanBereich->getGmlId() );

    pRel = new FeatureRelation;
    pRel->featureId = gmlId;
    pRel->rollenName = "xplan:rasterBasis";
    pPlanBereich->addFeatureRelation ( pRel );

    anz = pRasterplanBasis->getExterneReferenzen( externeReferenzen );
    for ( j = 0; j < anz; j++ )
    {
      gmlId =  Constant::createGmlId();
      pExterneReferenz = externeReferenzen[j];

      pObjExterneReferenz = new Feature ( ANY_OBJEKT, gmlId, m_pFeatures, "xplan:XP_ExterneReferenz" );

      pObjExterneReferenz->setParentId( pObj->getGmlId() );
      pObj->addChild( pObjExterneReferenz->getGmlId() );

      if ( pExterneReferenz->relationName == "xplan:refScan" )
        pObjExterneReferenz->setStringAttribut ( "xplan:art", "PlanMitGeoreferenz" );
      else
        pObjExterneReferenz->setStringAttribut ( "xplan:art", "Dokument" );

      if ( pExterneReferenz->informationssystemURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:informationssystemURL", pExterneReferenz->informationssystemURL );
      if ( pExterneReferenz->referenzName != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:referenzName", pExterneReferenz->referenzName );
      if ( pExterneReferenz->referenzURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:referenzURL", pExterneReferenz->referenzURL );
      if ( pExterneReferenz->referenzMimeType != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:referenzMimeType", pExterneReferenz->referenzMimeType );
      if ( pExterneReferenz->georefURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:georefURL", pExterneReferenz->georefURL );
      if ( pExterneReferenz->georefMimeType != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:georefMimeType", pExterneReferenz->georefMimeType );
      if ( pExterneReferenz->beschreibung != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:beschreibung", pExterneReferenz->beschreibung );

      if ( m_pFeatures->getGmlTyp() == XPLANGML_4_1 )
      {
        if ( pExterneReferenz->datum != "" )
        {
          TimePosition * pTimeValue = new TimePosition;
          pTimeValue->fromXMLString(  pExterneReferenz->datum );
          DateAttr * pTime = new DateAttr;
          pTime->value = pTimeValue;
          pObjExterneReferenz->setDateAttribut ( "xplan:datum", pTime );

        }
      }

      m_pFeatures->addFeature ( pObjExterneReferenz );
      pRel = new FeatureRelation;
      pRel->featureId = gmlId;
      pRel->rollenName = pExterneReferenz->relationName;
      pObj->addFeatureRelation ( pRel );
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Plan-Bereich Attribute nach der spez. Syntax von               //
//  XPlanGML 5.0                                                             //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::setBereichAttribute_V5_0 ( PlanBereich * pPlanBereich, Bereich * pBereich )
{
  string                   stdString;
  string                   gmlId;
  Feature                * pObj;
  FeatureRelation        * pRel;
	size_t                   j, anz;
  ExterneReferenz        * pExterneReferenz;
  Feature                * pObjExterneReferenz;
  vector<ExterneReferenz*> externeReferenzen;

  stdString = pBereich->bedeutungEnum;
  if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:bedeutung", stdString );

  stdString = pBereich->bedeutung;
  if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:sonstigeBedeutung", stdString );

  if ( m_planTyp == BPLAN || m_planTyp == FPLAN )
  {
    stdString = pBereich->versionBauNVOText;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionBauNVOText", stdString );

    stdString = pBereich->versionBauGBText;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionBauGBText", stdString );

    stdString = pBereich->versionBauNVO;
    if ( stdString != "" ) 
		{
			TimePosition * pTimeValue = new TimePosition;
			pTimeValue->fromXMLString( stdString );
			DateAttr * pTime = new DateAttr;
			pTime->value = pTimeValue;
			pPlanBereich->setDateAttribut ( "xplan:versionBauNVODatum", pTime );
		}						

    stdString = pBereich->versionBauGBDatum;
    if ( stdString != "" )
    {
      TimePosition * pTimeValue = new TimePosition;
      pTimeValue->fromXMLString( stdString );
      DateAttr * pTime = new DateAttr;
      pTime->value = pTimeValue;
      pPlanBereich->setDateAttribut ( "xplan:versionBauGBDatum", pTime );
    }
  }
  else
  if ( m_planTyp == RPLAN )
  {
    stdString = pBereich->versionBROGText;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionBROGText", stdString );

    stdString = pBereich->versionBROGDatum;
    if ( stdString != "" )
    {
      TimePosition * pTimeValue = new TimePosition;
      pTimeValue->fromXMLString( stdString );
      DateAttr * pTime = new DateAttr;
      pTime->value = pTimeValue;
      pPlanBereich->setDateAttribut ( "xplan:versionBROG", pTime );
    }
    stdString = pBereich->versionLPLGText;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionLPLGText", stdString );
    stdString = pBereich->versionLPLGDatum;
    if ( stdString != "" )
    {
      TimePosition * pTimeValue = new TimePosition;
      pTimeValue->fromXMLString( stdString );
      DateAttr * pTime = new DateAttr;
      pTime->value = pTimeValue;
      pPlanBereich->setDateAttribut ( "xplan:versionLPLG", pTime );
    }
  }

  RasterplanBasis * pRasterplanBasis = pBereich->getRasterplanBasis();
  if ( pRasterplanBasis != NULL )
  {
    gmlId =  Constant::createGmlId();
    pObj = new Feature ( RASTER_BASIS, gmlId, m_pFeatures, "xplan:XP_Rasterdarstellung" );

    m_pFeatures->addFeature ( pObj );
    pPlanBereich->addObjekt ( pObj );
    pObj->setBereichId ( pPlanBereich->getGmlId() );

    pRel = new FeatureRelation;
    pRel->featureId = gmlId;
    pRel->rollenName = "xplan:rasterBasis";
    pPlanBereich->addFeatureRelation ( pRel );

    anz = pRasterplanBasis->getExterneReferenzen( externeReferenzen );
    for ( j = 0; j < anz; j++ )
    {
      gmlId =  Constant::createGmlId();
      pExterneReferenz = externeReferenzen[j];

      pObjExterneReferenz = new Feature ( ANY_OBJEKT, gmlId, m_pFeatures, "xplan:XP_ExterneReferenz" );
      if ( pExterneReferenz->relationName == "xplan:refScan" )
        pObjExterneReferenz->setStringAttribut ( "xplan:art", "PlanMitGeoreferenz" );
      else
        pObjExterneReferenz->setStringAttribut ( "xplan:art", "Dokument" );

      pObjExterneReferenz->setParentId( pObj->getGmlId() );
      pObj->addChild( pObjExterneReferenz->getGmlId() );

      if ( pExterneReferenz->informationssystemURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:informationssystemURL", pExterneReferenz->informationssystemURL );
      if ( pExterneReferenz->referenzName != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:referenzName", pExterneReferenz->referenzName );
      if ( pExterneReferenz->referenzURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:referenzURL", pExterneReferenz->referenzURL );
      if ( pExterneReferenz->referenzMimeType != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:referenzMimeType", pExterneReferenz->referenzMimeType );
      if ( pExterneReferenz->georefURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:georefURL", pExterneReferenz->georefURL );
      if ( pExterneReferenz->georefMimeType != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:georefMimeType", pExterneReferenz->georefMimeType );
      if ( pExterneReferenz->beschreibung != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:beschreibung", pExterneReferenz->beschreibung );
      if ( pExterneReferenz->datum != "" )
      {
        TimePosition * pTimeValue = new TimePosition;
        pTimeValue->fromXMLString(  pExterneReferenz->datum );
        DateAttr * pTime = new DateAttr;
        pTime->value = pTimeValue;
        pObjExterneReferenz->setDateAttribut ( "xplan:datum", pTime );

      }

      m_pFeatures->addFeature ( pObjExterneReferenz );
      pRel = new FeatureRelation;
      pRel->featureId = gmlId;
      pRel->rollenName = pExterneReferenz->relationName;
      pObj->addFeatureRelation ( pRel );
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Plan-Bereich Attribute nach der spez. Syntax von               //
//  XPlanGML 5.1 und 5.2                                                     //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::setBereichAttribute_V5_X ( PlanBereich * pPlanBereich, Bereich * pBereich )
{
  string                   stdString;
  string                   gmlId;
  vector<ExterneReferenz*> externeReferenzen;

  stdString = pBereich->bedeutungEnum;
  if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:bedeutung", stdString );

  stdString = pBereich->bedeutung;
  if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:sonstigeBedeutung", stdString );

  if ( m_planTyp == RPLAN )
  {
    stdString = pBereich->versionBROGText;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionBROGText", stdString );
    stdString = pBereich->versionBROGDatum;
    if ( stdString != "" )
    {
      TimePosition * pTimeValue = new TimePosition;
      pTimeValue->fromXMLString( stdString );
      DateAttr * pTime = new DateAttr;
      pTime->value = pTimeValue;
      pPlanBereich->setDateAttribut ( "xplan:versionBROG", pTime );
    }
    stdString = pBereich->versionLPLGText;
    if ( stdString != "" ) pPlanBereich->setStringAttribut ( "xplan:versionLPLGText", stdString );
    stdString = pBereich->versionLPLGDatum;
    if ( stdString != "" )
    {
      TimePosition * pTimeValue = new TimePosition;
      pTimeValue->fromXMLString( stdString );
      DateAttr * pTime = new DateAttr;
      pTime->value = pTimeValue;
      pPlanBereich->setDateAttribut ( "xplan:versionLPLG", pTime );
    }
  }


  RasterplanBasis * pRasterplanBasis = pBereich->getRasterplanBasis();
  if ( pRasterplanBasis != NULL )
  {
    size_t anz = pRasterplanBasis->getExterneReferenzen( externeReferenzen );
    for ( size_t j = 0; j < anz; j++ )
    {
      gmlId =  Constant::createGmlId();
      ExterneReferenz * pExterneReferenz = externeReferenzen[j];

      Feature * pObjExterneReferenz = new Feature ( ANY_OBJEKT, gmlId, m_pFeatures, "xplan:XP_ExterneReferenz" );
      if ( pExterneReferenz->relationName == "xplan:refScan" )
        pObjExterneReferenz->setStringAttribut ( "xplan:art", "PlanMitGeoreferenz" );
      else
        pObjExterneReferenz->setStringAttribut ( "xplan:art", "Dokument" );

      pObjExterneReferenz->setParentId( pPlanBereich->getGmlId() );
      pPlanBereich->addChild( pObjExterneReferenz->getGmlId() );

      if ( pExterneReferenz->informationssystemURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:informationssystemURL", pExterneReferenz->informationssystemURL );
      if ( pExterneReferenz->referenzName != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:referenzName", pExterneReferenz->referenzName );
      if ( pExterneReferenz->referenzURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:referenzURL", pExterneReferenz->referenzURL );
      if ( pExterneReferenz->referenzMimeType != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:referenzMimeType", pExterneReferenz->referenzMimeType );
      if ( pExterneReferenz->georefURL != "" )
        pObjExterneReferenz->setURLAttribut ( "xplan:georefURL", pExterneReferenz->georefURL );
      if ( pExterneReferenz->georefMimeType != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:georefMimeType", pExterneReferenz->georefMimeType );
      if ( pExterneReferenz->beschreibung != "" )
        pObjExterneReferenz->setStringAttribut ( "xplan:beschreibung", pExterneReferenz->beschreibung );
      if ( pExterneReferenz->datum != "" )
      {
        TimePosition * pTimeValue = new TimePosition;
        pTimeValue->fromXMLString( pExterneReferenz->datum );
        DateAttr * pTime = new DateAttr;
        pTime->value = pTimeValue;
        pObjExterneReferenz->setDateAttribut ( "xplan:datum", pTime );

      }

      m_pFeatures->addFeature ( pObjExterneReferenz );
      FeatureRelation * pRel = new FeatureRelation;
      pRel->featureId = gmlId;
      pRel->rollenName = pExterneReferenz->relationName;
      pPlanBereich->addFeatureRelation ( pRel );
    }
  }
  
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines Shape-Files												                       //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::readShapeFile ( std::string shapeFileP )
{
  m_pLogfile->writeLogFile( "" );
  m_pLogfile->writeLogFile( "Einlesen Shape-File" );
  m_pLogfile->writeLogFile( "shapeFileP" );

  m_vObjekteInShapeFile.clear();
  SHPFileReader* pShpFileReader = new SHPFileReader ( this, shapeFileP );
  bool success = pShpFileReader->read_from_files();

  delete pShpFileReader;

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Start des Einlesens eines neuen Shape-Files und Interpretation des		   //
//  Headers																	                                 //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::setShapeFileStructure
  ( SHPTypeEnum               shapeFileTypP, 
    std::string&              shapeFileNameP, 
    std::vector<std::string>& vStrAttrN,	// Vektor der String-Attribut Namen
    std::vector<std::string>& vIntAttrN,		// Vektor der Integer-Attribut Namen
    std::vector<std::string>& vDblAttrN,
    std::vector<std::string>& vDateAttrN )
{
  bool          success;
	size_t        i, j, anz;
  string        attributName;
  string        shapeAttributName;
  string        shapefileTypText;
  vector<AttributName*>  vAttrObjekte;

  ATTRIBUT_TYP   attributTyp;
  AttributName * pAttributObj;

  if ( m_shapeFileMode == ANALYSE )
  {
    m_vStringAttributNamen.clear();
    m_vIntegerAttributNamen.clear();
    m_vDoubleAttributNamen.clear();
    m_vDateAttributNamen.clear();

    ShapeFileAttribut * pShapeFileAttribut;
    m_pShapeFileObjektAkt = new ShapeFileObjekt ( shapeFileNameP );
    m_pShapeFileObjektAkt->setShapeGeometrieTyp ( shapeFileTypP );
    success = m_pShapeFileObjekte->addShapeFileObjekt ( m_pShapeFileObjektAkt );
    if ( !success ) return false;

    for ( i = 0; i < vStrAttrN.size(); i++ )
    {
      attributName = vStrAttrN[i];
      pShapeFileAttribut = new ShapeFileAttribut ( attributName );
      pShapeFileAttribut->setShapeAttributDatentyp ( _STRING );
      m_pShapeFileObjektAkt->addShapeFileAttribut ( pShapeFileAttribut );
      m_vStringAttributNamen.push_back ( attributName );
    }

    for ( i = 0; i < vIntAttrN.size(); i++ )
    {
      attributName = vIntAttrN[i];
      pShapeFileAttribut = new ShapeFileAttribut ( attributName );
      pShapeFileAttribut->setShapeAttributDatentyp ( _INTEGER );
      m_pShapeFileObjektAkt->addShapeFileAttribut ( pShapeFileAttribut );
      m_vIntegerAttributNamen.push_back ( attributName );
    }

    for ( i = 0; i < vDblAttrN.size(); i++ )
    {
      attributName = vDblAttrN[i];
      pShapeFileAttribut = new ShapeFileAttribut ( attributName );
      pShapeFileAttribut->setShapeAttributDatentyp ( _DOUBLE );
      m_pShapeFileObjektAkt->addShapeFileAttribut ( pShapeFileAttribut );
      m_vDoubleAttributNamen.push_back ( attributName );
    }

    for ( i = 0; i < vDateAttrN.size(); i++ )
    {
      attributName = vDateAttrN[i];
      pShapeFileAttribut = new ShapeFileAttribut ( attributName );
      pShapeFileAttribut->setShapeAttributDatentyp ( _DATE );
      m_pShapeFileObjektAkt->addShapeFileAttribut ( pShapeFileAttribut );
      m_vDateAttributNamen.push_back ( attributName );
    }

    return true;
  }

  m_vStringAttributObjekte.clear();
  m_vIntegerAttributObjekte.clear();
  m_vDoubleAttributObjekte.clear();
  m_vDateAttributObjekte.clear();

  m_vStringAttributAnz.clear();
  m_vIntegerAttributAnz.clear();
  m_vDoubleAttributAnz.clear();
  m_vDateAttributAnz.clear();

  m_fehlermeldung = "";

  if (shapeFileTypP == SHPT_NULL)
  {
    m_objektTyp = UNGUELTIG;
    shapefileTypText = "NULL";
  }
  else
  if (shapeFileTypP == SHPT_POINT)
  {
    m_objektTyp = XP_PUNKT_OBJEKT;
    shapefileTypText = "POINT";
  }
  else
  if (shapeFileTypP == SHPT_ARC)
  {
    m_objektTyp = XP_LINIEN_OBJEKT;
    shapefileTypText = "ARC";
  }
  else
  if (shapeFileTypP == SHPT_POLYGON)
  {
    m_objektTyp = XP_FLAECHEN_OBJEKT;
    shapefileTypText = "POLYGON";
  }
  else
  if (shapeFileTypP == SHPT_MULTIPOINT)
  {
    m_objektTyp = XP_PUNKT_OBJEKT;
    shapefileTypText = "MULTIPOINT";
  }
  else
  if (shapeFileTypP == SHPT_POINT_Z)
  {
    m_objektTyp = XP_PUNKT_OBJEKT;
    shapefileTypText = "POINT_Z";
  }
  else
  if (shapeFileTypP == SHPT_ARC_Z)
  {
    m_objektTyp = XP_LINIEN_OBJEKT;
    shapefileTypText = "ARC_Z";
  }
  else
  if (shapeFileTypP == SHPT_POLYGON_Z)
  {
    m_objektTyp = XP_FLAECHEN_OBJEKT;
    shapefileTypText = "POLYGON_Z";
  }
  else
  if (shapeFileTypP == SHPT_MULTIPOINT_Z)
  {
    m_objektTyp = XP_PUNKT_OBJEKT;
    shapefileTypText = "MULTIPOINT_Z";
  }
  else
  if (shapeFileTypP == SHPT_POINT_M)
  {
    m_objektTyp = XP_PUNKT_OBJEKT;
    shapefileTypText = "POINT_M";
  }
  else
  if (shapeFileTypP == SHPT_ARC_M)
  {
    m_objektTyp = XP_LINIEN_OBJEKT;
    shapefileTypText = "ARC_M";
  }
  else
  if (shapeFileTypP == SHPT_POLYGON_M)
  {
    m_objektTyp = XP_FLAECHEN_OBJEKT;
    shapefileTypText = "POLYGON_M";
  }
  else
  if (shapeFileTypP == SHPT_MULTIPOINT_M)
  {
    m_objektTyp = XP_PUNKT_OBJEKT;
    shapefileTypText = "MULTIPOINT_M";
  }
  else
  if (shapeFileTypP == SHPT_MULTIPATCH)
  {
    m_objektTyp = UNGUELTIG;
    shapefileTypText = "MULTIPATCH";
  }

  if ( m_objektTyp == UNGUELTIG )
  {
    m_fehlermeldung = "Ungültiger Shapefile-Typ: ";
    m_pLogfile->writeLogFile( m_fehlermeldung );
    m_pLogfile->writeLogFile( shapefileTypText );
    return false;
  }

  m_shapeFileName = shapeFileNameP;
  m_pShapeFileObjektAkt =  m_pShapeFileObjekte->getShapeFileObjekt ( m_shapeFileName );
  if ( m_readBereichGeometrie )
  {
    m_pShapeFileObjektAkt->setBereichNummer ( m_bereichNummer );
    return true;
  }

  m_klassenName = m_pShapeFileNamenKonvertierung->getKlassenName ( m_shapeFileName );
  m_pKlassenName = m_pShapeFileNamenKonvertierung->getKlassenNameObj ( m_shapeFileName );

  if ( m_klassenName == "Error" )
  {
    m_fehlermeldung = "Keine Zuordnung einer XPlanGML-Klasse zu einem Shapefile: ";
    m_pLogfile->writeLogFile( m_fehlermeldung );
    m_pLogfile->writeLogFile( m_shapeFileName );
    return false;
  }

  m_bereichNummer = m_pKlassenName->getBereichNummer();
  m_pShapeFileObjektAkt->setBereichNummer ( m_bereichNummer );


  OBJEKT_TYP typ = getObjektTyp ( m_klassenName );

  if ( typ == UNGUELTIG )
  {
    m_fehlermeldung = "Die zugeordnete Klasse gehört nicht zum XPlanGML-Schema: ";
    m_pLogfile->writeLogFile( m_fehlermeldung );
    m_pLogfile->writeLogFile( m_klassenName );
    return false;
  }

  if ( m_objektTyp != typ ) 
  {
    if ( m_objektTyp == XP_FLAECHEN_OBJEKT && ( typ == XP_FPO || typ == XP_GEOMETRIE_OBJEKT || typ == BLEIT_PLAN || typ == PLAN_BEREICH || typ == RASTER_BASIS || typ == RASTER_AENDERUNG ) )
      success = true;
    else
    if ( m_objektTyp == XP_LINIEN_OBJEKT && ( typ == XP_LPO || typ == XP_GEOMETRIE_OBJEKT ) ) 
      success =  true;
    else
    if ( m_objektTyp == XP_PUNKT_OBJEKT && ( typ == XP_PTO || typ == XP_PPO || typ == XP_GEOMETRIE_OBJEKT ) )
      success = true;
    else
      success = false;
  }
  else
    success = true;

  if ( success == false )
  {
    m_fehlermeldung = "Shapefile-Typ und Typ der XPlanGML Klasse sind inkonsistent";
    m_pLogfile->writeLogFile( m_fehlermeldung );
    m_pLogfile->writeLogFile( "" );
    return false;
  }

  for ( i = 0; i < vStrAttrN.size(); i++ )
  {
    vAttrObjekte.clear();
    shapeAttributName = vStrAttrN[i];

    anz = m_pShapeFileNamenKonvertierung->getAttributObjekte ( m_shapeFileName, shapeAttributName, vAttrObjekte );
    m_vStringAttributAnz.push_back ( (int)anz );
    if ( anz > 0 )
    {
      for ( j = 0; j < anz; j++ )
      {
        pAttributObj = vAttrObjekte[j];
        attributName = pAttributObj->nameStandard;
        attributTyp  = pAttributObj->typ;

        if ( attributTyp == _DOUBLE || attributTyp == _INTEGER || attributTyp == _SONST )
        {
          m_fehlermeldung = "Falscher Shapefile-Attribut Typ: ";
          m_pLogfile->writeLogFile( m_fehlermeldung );
          m_pLogfile->writeLogFile( shapeAttributName );
          m_pLogfile->writeLogFile( "" );
          pAttributObj = NULL;
        }
        m_vStringAttributObjekte.push_back ( pAttributObj );
      }
    }
    else
    {
      m_fehlermeldung = "Shapefile-Attribut wurde nicht zugeordnet: ";
      m_pLogfile->writeLogFile( m_fehlermeldung );
      m_pLogfile->writeLogFile( shapeAttributName );
      m_pLogfile->writeLogFile( "" );
    }
  }

  for ( i = 0; i < vIntAttrN.size(); i++ )
  {
    vAttrObjekte.clear();
    shapeAttributName = vIntAttrN[i];

    anz = m_pShapeFileNamenKonvertierung->getAttributObjekte ( m_shapeFileName, shapeAttributName, vAttrObjekte );
    m_vIntegerAttributAnz.push_back ( (int)anz );

    if ( anz > 0 )
    {
      for ( j = 0; j < anz; j++ )
      {
        pAttributObj = vAttrObjekte[j];
        attributName = pAttributObj->nameStandard;
        attributTyp  = pAttributObj->typ;

       if ( attributTyp != _INTEGER   &&
            attributTyp != _DOUBLE      &&
            attributTyp != _ANGLE       && 
            attributTyp != _LENGTH      && 
            attributTyp != _AREA        && 
            attributTyp != _VOLUME      &&
            attributTyp != _STRING      &&
            attributTyp != _ENUMERATION &&
            attributTyp != _EXTERNAL_CODE_LIST )
        {
          m_fehlermeldung = "Falscher Shapefile-Attribut Typ: ";
          m_pLogfile->writeLogFile( m_fehlermeldung );
          m_pLogfile->writeLogFile( shapeAttributName );
          m_pLogfile->writeLogFile( "" );
          pAttributObj = NULL;
        }
        m_vIntegerAttributObjekte.push_back ( pAttributObj );
      }
    }
    else	
    {
      m_fehlermeldung = "Shapefile-Attribut wurde nicht zugeordnet: ";
      m_pLogfile->writeLogFile( m_fehlermeldung );
      m_pLogfile->writeLogFile( shapeAttributName );
      m_pLogfile->writeLogFile( "" );
    }
  }

  for ( i = 0; i < vDblAttrN.size(); i++ )
  {
    vAttrObjekte.clear();
    shapeAttributName = vDblAttrN[i];

    anz = m_pShapeFileNamenKonvertierung->getAttributObjekte ( m_shapeFileName, shapeAttributName, vAttrObjekte );
    m_vDoubleAttributAnz.push_back ( (int)anz );

    if ( anz > 0 )
    {
      for ( j = 0; j < anz; j++ )
      {
        pAttributObj = vAttrObjekte[j];
        attributName = pAttributObj->nameStandard;
        attributTyp  = pAttributObj->typ;

        if ( attributTyp != _INTEGER      &&
             attributTyp != _DOUBLE       &&
             attributTyp != _ANGLE        && 
             attributTyp != _LENGTH       && 
             attributTyp != _AREA         && 
             attributTyp != _VOLUME       && 
             attributTyp != _POS_X        && 
             attributTyp != _POS_Y        && 
             attributTyp != _STRING       &&
             attributTyp != _REFERENZ     &&
             attributTyp != _DATA_TYPE )
         {
           m_fehlermeldung = "Falscher Shapefile-Attribut Typ: ";
           m_pLogfile->writeLogFile( m_fehlermeldung );
           m_pLogfile->writeLogFile( shapeAttributName );
           m_pLogfile->writeLogFile( "" );
           pAttributObj = NULL;
         }
         m_vDoubleAttributObjekte.push_back ( pAttributObj );
       }
     }
     else	
     {
       m_fehlermeldung = "Shapefile-Attribut wurde nicht zugeordnet: ";
       m_pLogfile->writeLogFile( m_fehlermeldung );
       m_pLogfile->writeLogFile( shapeAttributName );
       m_pLogfile->writeLogFile( "" );
     }
   }

   for ( i = 0; i < vDateAttrN.size(); i++ )
   {
     vAttrObjekte.clear();
     shapeAttributName = vDateAttrN[i];

     anz = m_pShapeFileNamenKonvertierung->getAttributObjekte ( m_shapeFileName, shapeAttributName, vAttrObjekte );
     m_vDateAttributAnz.push_back ( (int)anz );
     if ( anz > 0 )
     {
       for ( j = 0; j < anz; j++ )
       {
         pAttributObj = vAttrObjekte[j];
         attributName = pAttributObj->nameStandard;
         attributTyp  = pAttributObj->typ;

         if ( attributTyp != _DATE && pAttributObj->subAttributTyp != _DATE )
         {
           m_fehlermeldung = "Falscher Shapefile-Attribut Typ: ";
           m_pLogfile->writeLogFile( m_fehlermeldung );
           m_pLogfile->writeLogFile( shapeAttributName );
           m_pLogfile->writeLogFile( "" );
           pAttributObj = NULL;
         }
         m_vDateAttributObjekte.push_back ( pAttributObj );
       }
     }
     else
     {
       m_fehlermeldung = "Shapefile-Attribut wurde nicht zugeordnet: ";
       m_pLogfile->writeLogFile( m_fehlermeldung );
       m_pLogfile->writeLogFile( shapeAttributName );
       m_pLogfile->writeLogFile( "" );
     }
   }
   return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Punktobjekt zum BPlan hinzu								                  	 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::addShpPoint (
  SHPTypeEnum							         shapeFileTyp,  	// Typ des Geometrie-Objektes:	
  const std::string&					     shapeFileNameP,	// Layer-Name
  const std::vector<CoordPair>   & vCoords,	// Vektor der Punkt-Koordinaten
  const std::vector<std::string> & vStrAttrV,	// Vektor der String-Attribut Werte
  const std::vector<int>         & vIntAttrV,	// Vektor der Integer-Attribut Werte
  const std::vector<double>      & vDblAttrV, // Vektor der Double-Attribut Werte
  const std::vector<std::string> & vDateAttrV ) // Vektor der Date-Attribut Werte
{
  unsigned int       i, bereichNummer;
  bool		           success;
  string		         gmlId;
  string             bereichId;
  string			       planId;
  string             relationNameFachobjekt;
  string             relationNamePlanbereich;
  Feature          * pObj;
  PlanBereich      * pPlanBereich;
  BLeitPlan        * pPlan;
  vector<double>     vX;
  vector<double>     vY;
  OBJEKT_TYP         gmlObjektTyp;
  FeatureRelation  * pRelation;
  String           ^ STR;

  if ( m_shapeFileMode == ANALYSE )
  {
    string					attributName;
    string					attributWert;
    int             iAttributWert;
    ShapeFileAttribut	  * pShapeFileAttribut;
    ShapeFileAttributWert * pShapeFileAttributWert;

    for ( i = 0; i < m_vStringAttributNamen.size(); i++ )
    {
      attributName = m_vStringAttributNamen[i];
      attributWert = vStrAttrV[i];
      pShapeFileAttribut = m_pShapeFileObjektAkt->getShapeFileAttribut ( attributName );
      if ( pShapeFileAttribut != NULL && 
        !pShapeFileAttribut->existShapeFileAttributWert ( attributWert ) )
      {
        pShapeFileAttributWert = new ShapeFileAttributWert ( attributWert );
        pShapeFileAttribut->addShapeFileAttributWert ( pShapeFileAttributWert );
      }
    }

    for ( i = 0; i < m_vIntegerAttributNamen.size(); i++ )
    {
      attributName = m_vIntegerAttributNamen[i];
      iAttributWert = vIntAttrV[i];
      STR = Convert::ToString( vIntAttrV[i] );
      QuConvert::systemStr2stdStr( attributWert, STR );
      pShapeFileAttribut = m_pShapeFileObjektAkt->getShapeFileAttribut ( attributName );
      if ( pShapeFileAttribut != NULL && 
        !pShapeFileAttribut->existShapeFileAttributWert ( attributWert ) )
      {
        pShapeFileAttributWert = new ShapeFileAttributWert ( attributWert );
        pShapeFileAttribut->addShapeFileAttributWert ( pShapeFileAttributWert );
      }
    }

    return;
  }

  gmlObjektTyp            = getObjektTyp ( m_klassenName );
  relationNameFachobjekt  = getRelationsNameFachobjekt();
  relationNamePlanbereich = getRelationsNamePlanbereich();

  for ( i = 0; i < vCoords.size(); i++ )
  {
    vX.push_back ( vCoords[i].x );
    vY.push_back ( vCoords[i].y );
  }

  gmlId = createGmlIdFromShapefile( shapeFileNameP,  vIntAttrV[0] );

  pObj = new Feature ( gmlObjektTyp, gmlId, m_pFeatures, m_klassenName );
  pObj->addShapePoints2D ( "xplan:position", vX, vY, m_pFeatures->getSrsName() );

  setDefaultKomplexAttributWerte ( pObj, shapeFileNameP );

  success = setStringAttribute ( pObj, m_vStringAttributObjekte, m_vStringAttributAnz, vStrAttrV );
  setIntegerAttribute ( pObj, m_vIntegerAttributObjekte, m_vIntegerAttributAnz, vIntAttrV );
  setDoubleAttribute ( pObj, m_vDoubleAttributObjekte, m_vDoubleAttributAnz, vDblAttrV );
  setDateAttribute ( pObj, m_vDateAttributObjekte, m_vDateAttributAnz, vDateAttrV );

  setDefaultAttributWerte ( pObj, shapeFileNameP );

  if ( success )
  {
    m_pFeatures->addFeature ( pObj );

    if ( m_plaeneAusShapeErzeugen )
    {
      success = pObj->getStringAttributWert ( "xplan:__planId", planId );
      if ( success )
      {
        pPlan = m_pFeatures->getBLeitPlan ( planId );
        if ( pPlan != NULL )
        {
          pPlanBereich = pPlan->getPlanBereich ( 0 );
          bereichId    = pPlanBereich->getGmlId();

          pRelation = new FeatureRelation;
          pRelation->rollenName = relationNamePlanbereich;
          pRelation->featureId = bereichId;
          pObj->addFeatureRelation ( pRelation );
          pObj->setBereichId ( bereichId );
        }
        else success = false;
      }
      if ( !success ) return;
    }
    else
    {
      bereichNummer = m_pKlassenName->getBereichNummer();
      pPlanBereich = m_pBLeitPlan->getPlanBereich ( bereichNummer );
      bereichId    = pPlanBereich->getGmlId();

      pRelation = new FeatureRelation;
      pRelation->rollenName = relationNamePlanbereich;
      pRelation->featureId = bereichId;
      pObj->addFeatureRelation ( pRelation );
      pObj->setBereichId ( bereichId );
    }

    pRelation = new FeatureRelation;
    pRelation->featureId = gmlId;
    if (  gmlObjektTyp == XP_GEOMETRIE_OBJEKT || gmlObjektTyp == XP_PUNKT_OBJEKT )
      pRelation->rollenName = relationNameFachobjekt;
    else
      if (  gmlObjektTyp == XP_PPO  ||  gmlObjektTyp == XP_PTO  )
        pRelation->rollenName = "xplan:praesentationsobjekt";

    pPlanBereich->addFeatureRelation ( pRelation );
    pPlanBereich->addObjekt ( pObj );

    m_anzObjRead++;
  }
  else
  {
    m_pLogfile->writeLogFile( "Punktobjekt wurde nicht gelesen" );
    m_anzObjNotRead++;
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Linienobjekt zum BPlan hinzu								                  	 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::addShpLine ( 
  SHPTypeEnum		 					shapeFileTyp,		// Typ des Geometrie-Objektes:
  const std::string&		 			     shapeFileNameP,	// Layer-Name
  const std::vector<CoordPair>   & vCoords,	// Vektor der Punkt-Koordinaten
  const std::vector<int>         & vPartStarts,// Vektor der Anfangs-Indizes der Parts
  const std::vector<std::string> & vStrAttrV,	// Vektor der String-Attribut Werte
  const std::vector<int>         & vIntAttrV,	// Vektor der Integer-Attribut Werte
  const std::vector<double>      & vDblAttrV, // Vektor der Double-Attribut Werte
  const std::vector<std::string> & vDateAttrV ) // Vektor der Date-Attribut Werte
{
	size_t             i, anz;
  int                bereichNummer;
  errno_t            err;
  bool		           success;
  string		         gmlId;
  string             gmlIdNew;
  string             bereichId;
  string			       planId;
  string             relationNameFachobjekt;
  string             relationNamePlanbereich;
  PlanBereich      * pPlanBereich;
  BLeitPlan        * pPlan;
  vector<double>     vXCoords;
  vector<double>     vYCoords;
  vector<double>     vX;
  vector<double>     vY;
  Feature	         * pObj;
  Feature	         * pObjNew;
  OBJEKT_TYP         gmlObjektTyp;
  FeatureRelation  * pRelation;
  char               buffer[6];
  String           ^ STR;

  if ( m_shapeFileMode == ANALYSE )
  {
    string				        	attributName;
    string				        	attributWert;
    int                     iAttributWert;
    ShapeFileAttribut	    * pShapeFileAttribut;
    ShapeFileAttributWert * pShapeFileAttributWert;

    for ( i = 0; i < m_vStringAttributNamen.size(); i++ )
    {
      attributName = m_vStringAttributNamen[i];
      attributWert = vStrAttrV[i];
      pShapeFileAttribut = m_pShapeFileObjektAkt->getShapeFileAttribut ( attributName );
      if ( pShapeFileAttribut != NULL && 
        !pShapeFileAttribut->existShapeFileAttributWert ( attributWert ) )
      {
        pShapeFileAttributWert = new ShapeFileAttributWert ( attributWert );
        pShapeFileAttribut->addShapeFileAttributWert ( pShapeFileAttributWert );
      }
    }

    for ( i = 0; i < m_vIntegerAttributNamen.size(); i++ )
    {
      attributName = m_vIntegerAttributNamen[i];
      iAttributWert = vIntAttrV[i];
      STR = Convert::ToString( vIntAttrV[i] );
      QuConvert::systemStr2stdStr( attributWert, STR );
      pShapeFileAttribut = m_pShapeFileObjektAkt->getShapeFileAttribut ( attributName );
      if ( pShapeFileAttribut != NULL && 
        !pShapeFileAttribut->existShapeFileAttributWert ( attributWert )  )
      {
        pShapeFileAttributWert = new ShapeFileAttributWert ( attributWert );
        pShapeFileAttribut->addShapeFileAttributWert ( pShapeFileAttributWert );
      }
    }

    return;
  }

  gmlObjektTyp            = getObjektTyp ( m_klassenName );
  relationNameFachobjekt  = getRelationsNameFachobjekt();
  relationNamePlanbereich = getRelationsNamePlanbereich();

  gmlId = createGmlIdFromShapefile( shapeFileNameP, vIntAttrV[0] );

  pObj = new Feature ( gmlObjektTyp, gmlId, m_pFeatures, m_klassenName );
  for ( i = 0; i < vCoords.size(); i++ )
  {
    vXCoords.push_back ( vCoords[i].x );
    vYCoords.push_back ( vCoords[i].y );
  }

  pObj->addShapeLines2D ( "xplan:position", vXCoords, vYCoords, vPartStarts, m_pFeatures->getSrsName() );

  setDefaultKomplexAttributWerte ( pObj, shapeFileNameP );

  success = setStringAttribute ( pObj, m_vStringAttributObjekte, m_vStringAttributAnz, vStrAttrV );
  setIntegerAttribute ( pObj, m_vIntegerAttributObjekte, m_vIntegerAttributAnz, vIntAttrV );
  setDoubleAttribute ( pObj, m_vDoubleAttributObjekte, m_vDoubleAttributAnz, vDblAttrV );
  setDateAttribute ( pObj, m_vDateAttributObjekte, m_vDateAttributAnz, vDateAttrV );

  setDefaultAttributWerte ( pObj, shapeFileNameP );

  if ( success )
  {
    MultiCurve * pMultiCurve = (MultiCurve*)pObj->getGeometry( "xplan:position", 0 );
    anz = pMultiCurve->getCurveMemberAnz();

    for ( i = 0; i < anz; i++ )
    {
      if ( i > 0 && m_pFeatures->getGmlTyp() >= XPLANGML_3_0 )
        break;

      if ( i > 0 )
      {
        _Curve * pCurve = pMultiCurve->getCurve ( i );
        MultiCurve * pMultiCurveNew = new MultiCurve ( 2 );
        pMultiCurveNew->addCurve ( pCurve );

        err =  _itoa_s ( (int)i, buffer, 5, 10 );
        gmlIdNew = gmlId + "_" + string ( buffer );
        pObjNew = new Feature ( gmlObjektTyp, gmlIdNew, m_pFeatures, m_klassenName );
        pObjNew->setGmlId ( gmlIdNew );
        pObjNew->addGeometryProperty ( "xplan:position", pMultiCurveNew );
      }
      else
      {
        gmlIdNew = gmlId;
        pObjNew  = pObj;
      }

      m_pFeatures->addFeature ( pObjNew );

      if ( i == 0 )
      {
        if ( m_plaeneAusShapeErzeugen )
        {
          success = pObj->getStringAttributWert ( "xplan:__planId", planId );
          if ( success )
          {
            pPlan = m_pFeatures->getBLeitPlan ( planId );
            if ( pPlan != NULL )
            {
              pPlanBereich = pPlan->getPlanBereich ( 0 );
              bereichId    = pPlanBereich->getGmlId();

              if ( gmlObjektTyp != RASTER_BASIS &&
                gmlObjektTyp != RASTER_AENDERUNG )
              {
                pRelation = new FeatureRelation;
                pRelation->rollenName = relationNamePlanbereich;
                pRelation->featureId = bereichId;
                pObjNew->addFeatureRelation ( pRelation );
                pObjNew->setBereichId ( bereichId );
              }
            }
            else success = false;
          }
          if ( !success ) return;
        }
        else
        {
          bereichNummer = m_pKlassenName->getBereichNummer();
          pPlanBereich = m_pBLeitPlan->getPlanBereich ( bereichNummer );
          bereichId    = pPlanBereich->getGmlId();

          if ( gmlObjektTyp != RASTER_BASIS && gmlObjektTyp != RASTER_AENDERUNG )
          {
            pRelation = new FeatureRelation;
            pRelation->rollenName = relationNamePlanbereich;
            pRelation->featureId = bereichId;
            pObjNew->addFeatureRelation ( pRelation );
            pObjNew->setBereichId ( bereichId );
          }
        }
      }

      pRelation				 = new FeatureRelation;
      pRelation->featureId = gmlIdNew;
      if (  gmlObjektTyp == XP_GEOMETRIE_OBJEKT || gmlObjektTyp == XP_LINIEN_OBJEKT )
        pRelation->rollenName = relationNameFachobjekt;
      else
      if (  gmlObjektTyp == XP_LPO  )
        pRelation->rollenName = "xplan:praesentationsobjekt";
      else
      if (  gmlObjektTyp == RASTER_BASIS  )
        pRelation->rollenName = "xplan:rasterBasis";
      else
      if (  gmlObjektTyp == RASTER_AENDERUNG  )
        pRelation->rollenName = "xplan:rasterAenderung";

      pPlanBereich->addFeatureRelation ( pRelation ); 
      pPlanBereich->addObjekt ( pObjNew );

      m_vObjekteInShapeFile.push_back ( pObjNew );
      m_anzObjRead++;
    }

    if ( m_pFeatures->getGmlTyp() == XPLANGML_2_0 )
      pMultiCurve->removeMultiLine();
  }
  else
  {
    m_pLogfile->writeLogFile( "Linienobjekt wurde nicht gelesen" );
    m_anzObjNotRead++;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Flächenobjekt zum BPlan hinzu								                	 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::addShpPolygon (
  SHPTypeEnum	 						         shapeFileTyp,		// Typ des Geometrie-Objektes
  const std::string&		 			     shapeFileNameP,	// Layer-Name
  const std::vector<CoordPair>& 	 vCoords,	// Vektor der Punkt-Koordinaten
  const std::vector<int>         & vRingStarts,// Vektor der Anf.-Indizes der Ringe
  const std::vector<int>         & vRingTypes,	// Vektor der Ring-Typen
  const std::vector<std::string> & vStrAttrV,	// Vektor der String-Attribut Werte
  const std::vector<int>         & vIntAttrV,	// Vektor der Integer-Attribut Werte
  const std::vector<double>      & vDblAttrV,  // Vektor der Double-Attribut-Werte
  const std::vector<std::string> & vDateAttrV ) // Vektor der Date-Attribut Werte
  //  Bedeutung des Vektors vRingTypes:
  //  Jeder äußere Ring erhält eine eindeutige Kennung k > 0
  //  Jedes Loch im äußeren Ring mit Kennung k erhält die Kennung -k
{
  unsigned int	       i, bereichNummer;
  errno_t              err;
  bool	    	         success;
  string		           gmlId;
  string               gmlIdNew;
  string               bereichId;
  string               planId;
  string               relationNameFachobjekt;
  string               relationNamePlanbereich;
  BLeitPlan          * pPlan;
  PlanBereich        * pPlanBereich;
  vector<double>       vX;
  vector<double>       vY;
  Feature		         * pObjNew;
  Feature            * pObj;
  OBJEKT_TYP           gmlObjektTyp;
  FeatureRelation    * pRelation;
  char                 buffer[6];
  String             ^ STR;

  if ( m_shapeFileMode == ANALYSE )
  {
    string					attributName;
    string					attributWert;
    string					iAttributWert;
    ShapeFileAttribut	  * pShapeFileAttribut;
    ShapeFileAttributWert * pShapeFileAttributWert;

    for ( i = 0; i < m_vStringAttributNamen.size(); i++ )
    {
      attributName = m_vStringAttributNamen[i];
      attributWert = vStrAttrV[i];
      pShapeFileAttribut = m_pShapeFileObjektAkt->getShapeFileAttribut ( attributName );
      if ( pShapeFileAttribut != NULL && 
        !pShapeFileAttribut->existShapeFileAttributWert ( attributWert ) )
      {
        pShapeFileAttributWert = new ShapeFileAttributWert ( attributWert );
        pShapeFileAttribut->addShapeFileAttributWert ( pShapeFileAttributWert );
      }
    }

    for ( i = 0; i < m_vIntegerAttributNamen.size(); i++ )
    {
      attributName = m_vIntegerAttributNamen[i];
      iAttributWert = vIntAttrV[i];
      STR = Convert::ToString( vIntAttrV[i] );
      QuConvert::systemStr2stdStr( attributWert, STR );
      pShapeFileAttribut = m_pShapeFileObjektAkt->getShapeFileAttribut ( attributName );
      if ( pShapeFileAttribut != NULL && 
        !pShapeFileAttribut->existShapeFileAttributWert ( attributWert ) )
      {
        pShapeFileAttributWert = new ShapeFileAttributWert (attributWert );
        pShapeFileAttribut->addShapeFileAttributWert ( pShapeFileAttributWert );
      }
    }

    return;
  }

  gmlObjektTyp            = getObjektTyp ( m_klassenName );
  relationNameFachobjekt  = getRelationsNameFachobjekt();
  relationNamePlanbereich = getRelationsNamePlanbereich();

  for ( i = 0; i < vCoords.size(); i++ )
  {
    vX.push_back ( vCoords[i].x );
    vY.push_back ( vCoords[i].y );
  }

  if ( gmlObjektTyp == BLEIT_PLAN )
  {
    if ( m_plaeneAusShapeErzeugen )
    {
      gmlId =  Constant::createGmlId();
      pObj = new BLeitPlan ( m_pFeatures, gmlId, m_planKlassenName );
      m_pFeatures->addFeature ( pObj );
      pObj->setStringAttribut ( "xplan:xPlanGMLVersion", m_pFeatures->getGmlTypAsString() );

      gmlId =  Constant::createGmlId();
      pPlanBereich = new PlanBereich ( m_pFeatures, gmlId, m_planBereichKlassenName );
      pPlanBereich->setIntegerAttribut ( "xplan:nummer", 0 );
      m_pFeatures->addFeature ( pPlanBereich );
      ((BLeitPlan*)pObj)->addPlanBereich ( gmlId );

      pRelation = new FeatureRelation;
      pRelation->rollenName    = "xplan:gehoertZuPlan";
      pRelation->featureId = pObj->getGmlId();
      pPlanBereich->addFeatureRelation ( pRelation );
    }
    else
      pObj = m_pBLeitPlan;
  }
  else
  if ( gmlObjektTyp == PLAN_BEREICH )
  {
    if ( m_plaeneAusShapeErzeugen ) return;
    else
    {
      int bereichNummer;

      bereichNummer = m_pShapeFileObjektAkt->getBereichNummer();
      pObj = m_pBLeitPlan->getPlanBereich ( bereichNummer );
      if ( pObj == NULL ) return;
    }
  }
  else
  {
    gmlId = createGmlIdFromShapefile( shapeFileNameP, vIntAttrV[0] );
    pObj = new Feature ( gmlObjektTyp, gmlId, m_pFeatures, m_klassenName  );
  }

  if ( gmlObjektTyp == BLEIT_PLAN  )
    pObj->addShapePolygons2D ( "xplan:raeumlicherGeltungsbereich", vX, vY, vRingStarts, vRingTypes, m_pFeatures->getSrsName() );
  else
  if ( gmlObjektTyp == PLAN_BEREICH )
    pObj->addShapePolygons2D ( "xplan:geltungsbereich", vX, vY, vRingStarts, vRingTypes, m_pFeatures->getSrsName() );
  else
    pObj->addShapePolygons2D ( "xplan:position", vX, vY, vRingStarts, vRingTypes, m_pFeatures->getSrsName() );

  setDefaultKomplexAttributWerte ( pObj, shapeFileNameP );

  success = setStringAttribute ( pObj, m_vStringAttributObjekte, m_vStringAttributAnz, vStrAttrV );
  setIntegerAttribute ( pObj, m_vIntegerAttributObjekte, m_vIntegerAttributAnz, vIntAttrV );
  setDoubleAttribute ( pObj, m_vDoubleAttributObjekte, m_vDoubleAttributAnz, vDblAttrV );
  setDateAttribute ( pObj, m_vDateAttributObjekte, m_vDateAttributAnz, vDateAttrV );

  setDefaultAttributWerte ( pObj, shapeFileNameP );

  if ( success )
  {
    if ( gmlObjektTyp != BLEIT_PLAN && gmlObjektTyp != PLAN_BEREICH )
    {
      MultiSurface * pMultiSurface = (MultiSurface*)pObj->getGeometry( "xplan:position", 0 );

      for ( i = 0; i < pMultiSurface->getSurfaceMemberAnz(); i++ )
      {
        if ( i > 0 && m_pFeatures->getGmlTyp() >= XPLANGML_3_0 )
          break;

        if ( i > 0 )
        {
          _Surface * pPoly = pMultiSurface->getSurface ( i );
          MultiSurface * pMultiSurfaceNew = new MultiSurface( 2 );
          pMultiSurfaceNew->addSurface ( pPoly );

          err = _itoa_s ( i, buffer, 5, 10 );
          gmlIdNew = gmlId + "_" + string ( buffer  );
          pObjNew = new Feature ( gmlObjektTyp, gmlIdNew, m_pFeatures, m_klassenName );
          pObjNew->addGeometryProperty ( "xplan:position", pMultiSurfaceNew );
        }
        else
        {
          gmlIdNew = gmlId;
          pObjNew  = pObj;
        }

        m_pFeatures->addFeature ( pObjNew );

        if ( i == 0 )
        {
          if ( m_plaeneAusShapeErzeugen )
          {
            success = pObj->getStringAttributWert ( "xplan:__planId", planId );
            if ( success )
            {
              pPlan = m_pFeatures->getBLeitPlan ( planId );
              if ( pPlan != NULL )
              {
                pPlanBereich = pPlan->getPlanBereich ( 0 );
                bereichId    = pPlanBereich->getGmlId();

                if ( gmlObjektTyp != RASTER_BASIS && gmlObjektTyp != RASTER_AENDERUNG )
                {
                  pRelation = new FeatureRelation;
                  pRelation->rollenName = relationNamePlanbereich;
                  pRelation->featureId = bereichId;
                  pObjNew->addFeatureRelation ( pRelation );
                  pObjNew->setBereichId ( bereichId );
                }
              }
              else success = false;
            }
            if ( !success ) return;
          }
          else
          {
            bereichNummer = m_pKlassenName->getBereichNummer();
            pPlanBereich  = m_pBLeitPlan->getPlanBereich ( bereichNummer );
            bereichId     = pPlanBereich->getGmlId();

            if ( gmlObjektTyp != RASTER_BASIS && gmlObjektTyp != RASTER_AENDERUNG )
            {
              pRelation = new FeatureRelation;
              pRelation->rollenName = relationNamePlanbereich;
              pRelation->featureId = bereichId;
              pObjNew->addFeatureRelation ( pRelation );
              pObjNew->setBereichId ( bereichId );
            }
          }
        }

        pRelation				 = new FeatureRelation;
        pRelation->featureId = gmlIdNew;
        if (  gmlObjektTyp == XP_GEOMETRIE_OBJEKT || gmlObjektTyp == XP_FLAECHEN_OBJEKT )
          pRelation->rollenName = relationNameFachobjekt;
        else
        if (  gmlObjektTyp == XP_FPO  )
          pRelation->rollenName = "xplan:praesentationsobjekt";
        else
        if (  gmlObjektTyp == RASTER_BASIS  )
          pRelation->rollenName = "xplan:rasterBasis";
        else
        if (  gmlObjektTyp == RASTER_AENDERUNG  )
          pRelation->rollenName = "xplan:rasterAenderung";

        pPlanBereich->addFeatureRelation ( pRelation );
        pPlanBereich->addObjekt ( pObjNew );

        m_vObjekteInShapeFile.push_back ( pObjNew );
        m_anzObjRead++;
      }
      if ( m_pFeatures->getGmlTyp() == XPLANGML_2_0 )
        pMultiSurface->removeMultiPolygon();
    }
  }
  else
  {
    m_pLogfile->writeLogFile( "Flächenobjekt nicht gelesen" );
    m_anzObjNotRead++;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Namen der Relation eines Planbereichs auf ein Fachobjekt     //
///////////////////////////////////////////////////////////////////////////////
std::string XPlanGML::getRelationsNameFachobjekt ()
{
  GML_SCHEMA_TYPE schemaTyp = m_pFeatures->getGmlTyp();
  if ( m_pKlassenName == NULL ) return "";

  int rechtlicheBedeutung = m_pKlassenName->getRechtlicheBedeutung();

  if ( schemaTyp <= XPLANGML_3_0 ) return "xplan:fachobjekt";

  else
  if ( schemaTyp <= XPLANGML_4_1 )
  {
    if ( rechtlicheBedeutung == NACHR_UEBERNAHME || rechtlicheBedeutung == SONST_RECHTL_BEDEUTUNG )
      return "xplan:nachrichtlich";
    else
      switch ( m_planTyp )
      {
      case BPLAN:
          return "xplan:inhaltBPlan";

      case FPLAN:
          return "xplan:inhaltFPlan";

      case RPLAN:
          return "xplan:inhaltRPlan";

      case LPLAN:
          return "xplan:inhaltLPlan";

      case SOPLAN:
          return "xplan:inhaltSoPlan";

      default:
          return "";
      }
    }
    else
      return "xplan:planinhalt";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Namen der Relation eines Fachobjekt auf den zug Planbereich  //
///////////////////////////////////////////////////////////////////////////////
std::string XPlanGML::getRelationsNamePlanbereich()
{
  GML_SCHEMA_TYPE schemaTyp = m_pFeatures->getGmlTyp();
  if ( m_pKlassenName == NULL ) return "";

  int rechtlicheBedeutung = m_pKlassenName->getRechtlicheBedeutung();

  if ( schemaTyp <= XPLANGML_3_0 ) return "xplan:gehoertZuBereich";

  if ( schemaTyp <= XPLANGML_4_1 )
  {
    if ( rechtlicheBedeutung == NACHR_UEBERNAHME || rechtlicheBedeutung == SONST_RECHTL_BEDEUTUNG )
      return "xplan:gehoertNachrichtlichZuBereich";
    else
      switch ( m_planTyp )
    {
      case BPLAN:
        return "xplan:gehoertZuBP_Bereich";

      case FPLAN:
        return "xplan:gehoertZuFP_Bereich";

      case RPLAN:
        return "xplan:gehoertZuRP_Bereich";

      case LPLAN:
        return "xplan:gehoertZuLP_Bereich";

      case SOPLAN:
        return "xplan:gehoertZuSO_Bereich";

      default:
        return "";
    }
  }
  else
    return "xplan:gehoertZuBereich";
}

////////////////////////////////////////////////////////////////////////////////
//  Liefert den OBJEKT_TYP einer XPlanGML-Klasse auf Basis des Klassen-Namens //
////////////////////////////////////////////////////////////////////////////////
OBJEKT_TYP XPlanGML::getObjektTyp ( std::string objektNameP )
{
  string      typeName;
  GmlKlasse * pGmlKlasse;
  GmlSchema * pGmlObjekte = m_pFeatures->getGmlSchema();

  GmlElement * pGmlElement = pGmlObjekte->getGmlElement( objektNameP );
  if ( pGmlElement!= NULL )
    pGmlKlasse = pGmlObjekte->getGmlKlasse( pGmlElement->getGmlTypeName() );
  else
    pGmlKlasse = NULL;

  if ( pGmlKlasse == NULL )
    return UNGUELTIG;
  else
    return pGmlKlasse->getGmlObjektTyp();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Attribut-Typ eines XPlanGML-Attributs auf Basis von          //
//  Attribut- und Klassen-Namen                                              //
///////////////////////////////////////////////////////////////////////////////
ATTRIBUT_TYP XPlanGML::getAttributTyp ( string klassenNameP, string attributNameP, string &enumerationNameP )
{
  GmlSchema * pGmlObjekte = m_pFeatures->getGmlSchema();

  enumerationNameP = "";

  GmlKlasse * pGmlKlasse = pGmlObjekte->getGmlKlasseFromElementName ( klassenNameP );
  if ( pGmlKlasse == NULL )
    return _SONST;

  GmlAttribut * pGmlAttribut = pGmlKlasse->getGmlAttribut ( attributNameP );
  if ( pGmlAttribut == NULL )
    return _SONST;

  ATTRIBUT_TYP typ = pGmlAttribut->getGmlAttributTyp();
  if ( typ == _ENUMERATION || typ == _EXTERNAL_CODE_LIST )
    enumerationNameP = pGmlAttribut->getGmlEnumeration();

  return typ;
}

ATTRIBUT_TYP XPlanGML::getAttributTyp ( string klassenNameP, string attributNameP )
{
  string enumerationName;

  return getAttributTyp ( klassenNameP, attributNameP, enumerationName );
}


///////////////////////////////////////////////////////////////////////////////
//  Setzt die im Konvertierungs-File angegebenen Defaultwerte für alle       //
//  Attribute von pObjP, deren Wert noch nicht belegt ist					           //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::setDefaultAttributWerte ( Feature * pObjP, std::string shapeFileNameP )
{
	vector<AttributName*>    vAttribute;
  vector<FeatureRelation*> objRelationen;
	bool				             success;
	size_t  		             i, attributAnz, anz;
	string				           defaultWert, attributName, gmlId, rollenName;
  Feature                * pComplexDefaultWert;

	KlassenName * pKlName = m_pShapeFileNamenKonvertierung->getKlassenNameObj ( shapeFileNameP );
	if ( pKlName == NULL ) return;

	attributAnz = pKlName->getBPlanAttributObjekte ( vAttribute );
	for ( i = 0; i < attributAnz; i++ )
	{
		AttributName * pAttribut = vAttribute[i];
		defaultWert         = pAttribut->defaultWert;
    pComplexDefaultWert = pAttribut->defaultWertKomplexAttribut;
		if ( defaultWert != "" || pComplexDefaultWert != NULL )
		{
			attributName = pAttribut->nameStandard;
			switch ( pAttribut->typ )
			{
        case _DATA_TYPE:
        case _REFERENZ:
          break;

				case _DOUBLE:
				{
					anz = pObjP->getDoubleAttributWertAnz ( attributName );
					if ( anz == 0 )
					{
						double wert = atof ( defaultWert.c_str() );
						pObjP->setDoubleAttribut ( attributName, wert );
						break;
					}
				}

				case _ANGLE:
				{
					anz = pObjP->getDoubleAttributWertAnz ( attributName );
					if ( anz == 0 )
					{
						double wert = atof ( defaultWert.c_str() );
						pObjP->setDoubleAttribut ( attributName, wert,  m_pFeatures->getUOMList()->getUOMFromName( "DEGREE" ) );
						break;
					}
				}

        case _LENGTH:
          {
            anz = pObjP->getDoubleAttributWertAnz ( attributName );
            if ( anz == 0 )
            {
              double wert = atof ( defaultWert.c_str() );
              pObjP->setDoubleAttribut ( attributName, wert,  m_pFeatures->getUOMList()->getUomLength() );
              break;
            }
          }

        case _AREA:
          {
            anz = pObjP->getDoubleAttributWertAnz ( attributName );
            if ( anz == 0 )
            {
              double wert = atof ( defaultWert.c_str() );
              pObjP->setDoubleAttribut ( attributName, wert, m_pFeatures->getUOMList()->getUomArea() );
              break;
            }
          }

        case _VOLUME:
          {
            anz = pObjP->getDoubleAttributWertAnz ( attributName );
            if ( anz == 0 )
            {
              double wert = atof ( defaultWert.c_str() );
              pObjP->setDoubleAttribut ( attributName, wert, m_pFeatures->getUOMList()->getUomVolume() );
              break;
            }
          }

				case _INTEGER:
				{
					anz = pObjP->getIntegerAttributWertAnz ( attributName );
					if ( anz == 0 )
					{
						int wert = atoi ( defaultWert.c_str() );
						pObjP->setIntegerAttribut ( attributName, wert );
						break;
					}
				}

				case _BOOLEAN:
				{
					bool b;
					success = pObjP->getBoolAttributWert ( attributName, b );
					if ( !success )
					{
						if ( defaultWert == "true" ) b = true;
						else						 b = false;
						pObjP->setBoolAttribut ( attributName, b );
						break;
					}
				}

				case _STRING:
				{
					anz = pObjP->getStringAttributWertAnz ( attributName );
					if ( anz == 0 )
					{
						pObjP->setStringAttribut ( attributName, defaultWert );
						break;
					}
				}

				case _DATE:
				{
					anz = pObjP->getDateAttributWertAnz ( attributName );
					if ( anz == 0 )
					{
            TimePosition * pTimeValue = new TimePosition;
            pTimeValue->fromXMLString( defaultWert );
            DateAttr * pTime = new DateAttr;
            pTime->value = pTimeValue;
						pObjP->setDateAttribut ( attributName, pTime );
						break;
					}
				}

				case _ANY_URI:
				{
					anz = pObjP->getURLAttributWertAnz ( attributName );
					if ( anz == 0 )
					{
						pObjP->setURLAttribut ( attributName, defaultWert );
						break;
					}
				}
			}
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt Default-Werte für Komplex-Attribute																 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::setDefaultKomplexAttributWerte ( Feature * pObjP, std::string shapeFileNameP )
{
  vector<AttributName*>    vAttribute;
  vector<FeatureRelation*> objRelationen;
	size_t  		             i, attributAnz, anz;
  string				           defaultWert, attributName, gmlId, rollenName;
  Feature                * pComplexDefaultWert;
  Feature                * pPlanObjekt;
  FeatureRelation        * pRel;

  KlassenName * pKlName = m_pShapeFileNamenKonvertierung->getKlassenNameObj ( shapeFileNameP );
  if ( pKlName == NULL ) return;

  attributAnz = pKlName->getBPlanAttributObjekte ( vAttribute );
  for ( i = 0; i < attributAnz; i++ )
  {
    AttributName * pAttribut = vAttribute[i];
    defaultWert         = pAttribut->defaultWert;
    pComplexDefaultWert = pAttribut->defaultWertKomplexAttribut;
    if ( defaultWert != "" || pComplexDefaultWert != NULL )
    {
      attributName = pAttribut->nameStandard;
      switch ( pAttribut->typ )
      {
      case _DATA_TYPE:
      case _REFERENZ:
        if ( pComplexDefaultWert == NULL ) continue ;

        gmlId =  Constant::createGmlId();
        pPlanObjekt = new Feature ( pComplexDefaultWert, gmlId ); 
        m_pFeatures->addFeature ( pPlanObjekt );
        if ( pAttribut->typ == _DATA_TYPE )
        {
          pPlanObjekt->setParentId( pObjP->getGmlId() ); 
          pObjP->addChild ( pPlanObjekt->getGmlId() );
        }

        pRel = new FeatureRelation;
        pRel->featureId = gmlId;
        if ( attributName == "xplan:hatGenerStringAttribut"  || 
             attributName == "xplan:hatGenerDoubleAttribut"  ||
             attributName == "xplan:hatGenerIntegerAttribut" ||
             attributName == "xplan:hatGenerDatumAttribut"   ||
             attributName == "xplan:hatGenerURLAttribut"       )
          rollenName = "xplan:hatGenerAttribut";
        else
          rollenName = attributName;
        pRel->rollenName = rollenName;
        pObjP->addFeatureRelation ( pRel );
        break;

      default:
        {
          anz = pObjP->getStringAttributWertAnz ( attributName );
          if ( anz == 0 )
          {
            pObjP->setStringAttribut ( attributName, defaultWert );
            break;
          }
        }
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////
//  Transformiert die angegebene Datei von XPlanGML Version 3.0 nach 4.0 oder 4.1, //
//  von Version 4.0/4.1 nach 5.0, 5.1 oder 5.2, und von Version 5.0 nach           //
//  5.1 oder 5.2                                                                   //
/////////////////////////////////////////////////////////////////////////////////////
MigrationProtokoll * XPlanGML::migrationDatei (  string quellDatei, string zielOrdner, string protokollDatei, 
  bool ueberschreiben, string versionXPlanGMLQuelle,  string versionXPlanGMLZiel )
{
  int                        objektAnz;
  string                     dateiName;
  string                     extension;
  GmlSchema                * pGmlObjekteSave;
  MigrationDateiProtokoll  * pMigDateiProtokoll;
  MigrationProtokollWriter ^ pWriter;
  string                     codeSpaceCodeLists;
  string                     ausgabePath;
  GmlSchema                * pGmlObjekteNew;
  MigrationProtokoll       * pMigProtokoll;

  m_pFeatures->clear();
  GMLReader ^ pReader = gcnew GMLReader ( m_pFeatures, false );
  objektAnz = pReader->read ( quellDatei, NULL );
  if ( objektAnz == 0 ) return NULL;

  if ( versionXPlanGMLQuelle == "3.0" && m_pFeatures->getGmlTyp() != XPLANGML_3_0 )
  {
    MessageBox::Show ( "Es wurde keine XPlanung Version 3.0 Datei spezifiziert" );
    return NULL;
  }

  if ( versionXPlanGMLQuelle == "4.x" && m_pFeatures->getGmlTyp() != XPLANGML_4_0  && m_pFeatures->getGmlTyp() != XPLANGML_4_1 )
  {
    MessageBox::Show ( "Es wurde keine XPlanung Version 4 Datei spezifiziert" );
    return NULL;
  }

  if ( versionXPlanGMLQuelle == "5.0" && m_pFeatures->getGmlTyp() != XPLANGML_5_0  )
  {
    MessageBox::Show ( "Es wurde keine XPlanung Version 5.0 Datei spezifiziert" );
    return NULL;
  }

  pGmlObjekteNew = new GmlSchema ( m_pFeatures );
  pGmlObjekteSave = m_pFeatures->getGmlSchema();

  if ( versionXPlanGMLZiel == "4.0" )
  {
    pGmlObjekteNew->readGmlSchema ( XPLANGML_4_0, NULL );
    codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_4_0 );
  }
  else
  if ( versionXPlanGMLZiel == "4.1" )
  {
    pGmlObjekteNew->readGmlSchema ( XPLANGML_4_1, NULL );
    codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_4_1 );
  }
  else
  if ( versionXPlanGMLZiel == "5.0" )
  {
    pGmlObjekteNew->readGmlSchema ( XPLANGML_5_0, NULL );
    codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_5_0 );
  }
  else
  if ( versionXPlanGMLZiel == "5.1" )
  {
    pGmlObjekteNew->readGmlSchema ( XPLANGML_5_1, NULL );
    codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_5_1 );
  }
  else
  if ( versionXPlanGMLZiel == "5.2" )
  {
    pGmlObjekteNew->readGmlSchema ( XPLANGML_5_2, NULL );
    codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_5_2 );
  }
  else
  if ( versionXPlanGMLZiel == "5.2" )
  {
    pGmlObjekteNew->readGmlSchema ( XPLANGML_5_3, NULL );
    codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_5_3 );
  }

  pMigProtokoll      = new MigrationProtokoll;
  pMigDateiProtokoll = new MigrationDateiProtokoll;
  pMigDateiProtokoll->setDateiName( quellDatei );
  pMigDateiProtokoll->setAnzahlObjekte( objektAnz );

  if ( versionXPlanGMLZiel == "4.0" || versionXPlanGMLZiel == "4.1"  )
  {
    MigrationV4 * pMigration = new MigrationV4( m_pFeatures, m_pUOMs, pMigDateiProtokoll, codeSpaceCodeLists, versionXPlanGMLZiel );
    pMigration->transformation ();
    delete pMigration;
  }
  else
  if ( versionXPlanGMLZiel == "5.0" || versionXPlanGMLZiel == "5.1" || versionXPlanGMLZiel == "5.2" )
  {
    MigrationV5 * pMigration = new MigrationV5( m_pFeatures, m_pUOMs, pMigDateiProtokoll, versionXPlanGMLZiel );
    pMigration->transformation ();
    delete pMigration;
  }

	size_t indexName = quellDatei.rfind ( "\\" );
	size_t indexExtension = quellDatei.rfind ( "." );
  dateiName = quellDatei.substr ( indexName+1, indexExtension - indexName - 1 );
  extension = quellDatei.substr( indexExtension, quellDatei.length() );
  if ( !ueberschreiben )
  {
    if ( versionXPlanGMLZiel == "4.0" ||  versionXPlanGMLZiel == "4.1" )
      dateiName = dateiName + "_V4";
    else
    if ( versionXPlanGMLZiel == "5.0"  )
      dateiName = dateiName + "_V5";
    else
    if ( versionXPlanGMLZiel == "5.1" )
      dateiName = dateiName + "_V5_1";
    else
    if (versionXPlanGMLZiel == "5.2")
      dateiName = dateiName + "_V5_2";
  }
  ausgabePath = zielOrdner + "\\" + dateiName + extension;

  m_pFeatures->exchangeGmlSchema( pGmlObjekteNew );

  GMLWriter ^ pGMLWriter = gcnew GMLWriter ( m_pFeatures );
  objektAnz = pGMLWriter->write ( ausgabePath, 0 );

  pMigProtokoll->addDateiProtokoll ( pMigDateiProtokoll );
  m_pFeatures->exchangeGmlSchema( pGmlObjekteSave );

  if ( pMigProtokoll->getMigrationFehlerAnz() > 0 )
  {
    pWriter = gcnew MigrationProtokollWriter ( pMigProtokoll );
    pWriter->write ( protokollDatei, "XPlanung " + versionXPlanGMLQuelle, "XPlanung " + versionXPlanGMLZiel );
  }

  ausgabePath = "";
  m_pFeatures->clear();
  delete pGmlObjekteNew;

  return pMigProtokoll;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert alle Dateien im angegebenen Ordner von XPlanGML            //
//  Version 3.0 nach 4.0/4.1 bzw. von 40./4.1 nach 5.0, 5.1 oder 5.2         //
///////////////////////////////////////////////////////////////////////////////
MigrationProtokoll * XPlanGML::migrationOrdner (  string quellOrdner, string zielOrdner, string protokollDatei,
             bool ueberschreiben, string versionXPlanGMLQuelle, string versionXPlanGMLZiel )
{
  int                        objektAnz, objektAnzGes;
  string                     dateiName;
  string                     quellDatei;
  string                     extension;
  GmlSchema                * pGmlObjekteQuelle;
  GmlSchema                * pGmlObjekteZiel;
  MigrationDateiProtokoll  * pMigDateiProtokoll;
  MigrationProtokollWriter ^ pWriter;
  String                   ^ QUELL_ORDNER;
  String                   ^ EXTENSION;
  int                        index;
  string                     codeSpaceCodeLists;
  string                     ausgabePath;
  MigrationProtokoll       * pMigProtokoll;

  m_pFeatures->clear();

  /*
  pGmlObjekteQuelle = new GmlSchema( m_pFeatures );
  m_pFeatures->setGmlSchema( pGmlObjekteQuelle );

  if (versionXPlanGMLQuelle == "3.0")
  {
    pGmlObjekteQuelle->readGmlSchema(XPLANGML_3_0, NULL);
    codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder(XPLANGML_3_0);
  }
  else
  if (versionXPlanGMLQuelle == "4.0")
  {
    pGmlObjekteQuelle->readGmlSchema(XPLANGML_4_0, NULL);
    codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder(XPLANGML_4_0);
  }
  else
  if (versionXPlanGMLQuelle == "4.1")
  {
    pGmlObjekteQuelle->readGmlSchema(XPLANGML_4_1, NULL);
    codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder(XPLANGML_4_1);
  }
  */

  GMLReader ^ pGMLReader = gcnew GMLReader ( m_pFeatures, false );

  objektAnzGes = 0;

  QUELL_ORDNER = QuConvert::ToString ( quellOrdner );
  bool exist = Directory::Exists ( QUELL_ORDNER );
  if ( exist )
  {
    pGmlObjekteZiel = new GmlSchema ( m_pFeatures );
    pMigProtokoll = new MigrationProtokoll;

    if ( versionXPlanGMLZiel == "4.0" )
    {
      pGmlObjekteZiel->readGmlSchema ( XPLANGML_4_0, NULL );
      codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_4_0 );
    }
    else
    if ( versionXPlanGMLZiel == "4.1" )
    {
      pGmlObjekteZiel->readGmlSchema ( XPLANGML_4_1, NULL );
      codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_4_1 );
    }
    else
    if ( versionXPlanGMLZiel == "5.0" )
    {
      pGmlObjekteZiel->readGmlSchema ( XPLANGML_5_0, NULL );
      codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_5_0 );
    }
    else
    if ( versionXPlanGMLZiel == "5.1" )
    {
      pGmlObjekteZiel->readGmlSchema ( XPLANGML_5_1, NULL );
      codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_5_1 );
    }
    else
    if ( versionXPlanGMLZiel == "5.2" )
    {
      pGmlObjekteZiel->readGmlSchema ( XPLANGML_5_2, NULL );
      codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_5_2 );
    }
    else
    if ( versionXPlanGMLZiel == "5.3" )
    {
      pGmlObjekteZiel->readGmlSchema ( XPLANGML_5_3, NULL );
      codeSpaceCodeLists = m_pFeatures->getExternalCodeListsFolder( XPLANGML_5_3 );
    }

    IEnumerator ^ FILES_ENUM = Directory::GetFiles ( QUELL_ORDNER )->GetEnumerator();
    while ( FILES_ENUM->MoveNext() )
    {
      String ^ FILE_NAME = dynamic_cast<String^>(FILES_ENUM->Current);
      index  = FILE_NAME->LastIndexOf( "." );
      if ( index == FILE_NAME->Length ) continue;
      else
      {
        EXTENSION = FILE_NAME->Substring( index+1, FILE_NAME->Length - index - 1 );
        EXTENSION = EXTENSION->ToUpper();
        if ( EXTENSION->CompareTo( "XML") != 0 &&
             EXTENSION->CompareTo( "GML") != 0 ) continue;
      }
      QuConvert::systemStr2stdStr( quellDatei, FILE_NAME );
      objektAnz = pGMLReader->read ( quellDatei, NULL );
      if ( objektAnz == 0 ) continue;

      pMigDateiProtokoll = new MigrationDateiProtokoll;
      pMigDateiProtokoll->setDateiName( quellDatei );
      pMigDateiProtokoll->setAnzahlObjekte( objektAnz );

      if ( versionXPlanGMLQuelle == "3.0" && m_pFeatures->getGmlTyp() != XPLANGML_3_0 )
      {
        MigrationFehler * pMigrationsFehler = new MigrationFehler;
        pMigrationsFehler->fehlerKlassifikation = SCHWERER_FEHLER;
        pMigrationsFehler->fehlerBeschreibung = "Es wurde keine XPlanung Version 3.0 Datei spezifiziert";
        pMigDateiProtokoll->addMigrationFehler( pMigrationsFehler );
        continue;
      }

      if ( versionXPlanGMLQuelle == "4.x" &&  m_pFeatures->getGmlTyp() != XPLANGML_4_0  &&  m_pFeatures->getGmlTyp() != XPLANGML_4_1 )
      {
        MigrationFehler * pMigrationsFehler = new MigrationFehler;
        pMigrationsFehler->fehlerKlassifikation = SCHWERER_FEHLER;
        pMigrationsFehler->fehlerBeschreibung = "Es wurde keine XPlanung Version 4.0 oder 4.1 Datei spezifiziert";
        pMigDateiProtokoll->addMigrationFehler( pMigrationsFehler );
        continue;
      }

      if ( versionXPlanGMLZiel == "4.0"|| versionXPlanGMLZiel == "4.1" )
      {
        MigrationV4 * pMigration = new MigrationV4( m_pFeatures, m_pUOMs, pMigDateiProtokoll, codeSpaceCodeLists, versionXPlanGMLZiel );
        pMigration->transformation ();
        delete pMigration;
      }
      else
      if ( versionXPlanGMLZiel == "5.0" || versionXPlanGMLZiel == "5.1" || versionXPlanGMLZiel == "5.2")
      {
        MigrationV5 * pMigration = new MigrationV5( m_pFeatures, m_pUOMs, pMigDateiProtokoll, versionXPlanGMLZiel );
        pMigration->transformation ();
        delete pMigration;
      }

			size_t indexName = quellDatei.rfind ( "\\" );
			size_t indexExtension = quellDatei.rfind ( "." );
      dateiName = quellDatei.substr ( indexName+1, indexExtension - indexName - 1 );
      extension = quellDatei.substr( indexExtension, quellDatei.length() );
      if ( !ueberschreiben )
      {
        if ( versionXPlanGMLZiel == "4.0" || versionXPlanGMLZiel == "4.1" )
          dateiName = dateiName + "_V4";
        else
          if ( versionXPlanGMLZiel == "5.0" || versionXPlanGMLZiel == "5.1" || versionXPlanGMLZiel == "5.2" )
            dateiName = dateiName + "_V5";
      }
      ausgabePath = zielOrdner + "\\" + dateiName + extension;

      pGmlObjekteQuelle = m_pFeatures->exchangeGmlSchema(pGmlObjekteZiel);
      GMLWriter ^ pGMLWriter = gcnew GMLWriter(m_pFeatures);
      objektAnz = pGMLWriter->write( ausgabePath, 0 );
      delete pGMLWriter;

      pMigDateiProtokoll->setAnzahlObjekteTransformiert( objektAnz );
      pGmlObjekteZiel = m_pFeatures->exchangeGmlSchema ( pGmlObjekteQuelle );
      objektAnzGes = objektAnzGes + objektAnz;
      m_pFeatures->clear();

      pMigProtokoll->addDateiProtokoll ( pMigDateiProtokoll );
    }

    if ( pMigProtokoll->getMigrationFehlerAnz() > 0 )
    {
      pWriter = gcnew MigrationProtokollWriter ( pMigProtokoll );
      pWriter->write ( protokollDatei, "XPlanung " + versionXPlanGMLQuelle, "XPlanung " + versionXPlanGMLZiel );
    }

    ausgabePath = "";
    delete pGmlObjekteZiel;

    return pMigProtokoll;
  }
  else
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//  Transformation einer XPlanGML 4.0 oder 4.1 Datei nach INSPIRE PLU        //
///////////////////////////////////////////////////////////////////////////////
TransformationINSPIREProtokoll * XPlanGML::transformationINSPIRE_PLU( string quellDatei, string zielDatei, Features * pPlanObjekteINSPIRE, 
  TransformationINSPIRE_PLU_Parameter * pTransParam, int outputFormat, GML_SCHEMA_TYPE versionXPlanGML )
{
	size_t               objektAnz, planAnz, bereichAnz;
  BLeitPlan          * pPlan;
  //  PlanBereich        * pPlanBereich;
  string               dateiName;
  string               extension;
  vector<PlanBereich*> vPlanBereiche;
  GMLReader          ^ pGMLReader = gcnew GMLReader ( m_pFeatures, false );

  m_pFeatures->clear();
  objektAnz = pGMLReader->read ( quellDatei, false );
  if ( objektAnz == 0 ) return NULL;

  if ( versionXPlanGML == XPLANGML_4_1 &&  
    m_pFeatures->getGmlTyp() != XPLANGML_4_0 && 
    m_pFeatures->getGmlTyp() != XPLANGML_4_1 ) 
  {
    MessageBox::Show ( "Es wurde keine XPlanGML 4.x Datei spezifiziert" );
    m_pFeatures->clear();
    return NULL;
  }

  if ( versionXPlanGML == XPLANGML_5_0 && m_pFeatures->getGmlTyp() != XPLANGML_5_0 ) 
  {
    MessageBox::Show ( "Es wurde keine XPlanGML 5.0 Datei spezifiziert" );
    m_pFeatures->clear();
    return NULL;
  }

  planAnz = m_pFeatures->getPlanAnz();
  if ( planAnz == 0 ) return NULL;

  TransformationINSPIREProtokoll * pProtokoll = new TransformationINSPIREProtokoll ( versionXPlanGML );

  for ( int i = 0; i < planAnz; i++ )
  {
    pPlan = m_pFeatures->getBLeitPlan ( 0 );

    vPlanBereiche.clear();
    bereichAnz = pPlan->getPlanBereiche ( vPlanBereiche );

    if ( versionXPlanGML == XPLANGML_4_1 )
    {
      TransformationINSPIRE_PLU * pTransformation = new TransformationINSPIRE_PLU 
              ( m_pFeatures, pPlanObjekteINSPIRE, m_pUOMs, m_pTransformationINSPIRE_PLU_Parameter );
      pTransformation->transformationToPLU ( pPlan, vPlanBereiche, pProtokoll );
      delete pTransformation;
    }
    else
      if ( versionXPlanGML == XPLANGML_5_0 )
      {
        TransformationINSPIRE_PLU_V5 * pTransformation = new TransformationINSPIRE_PLU_V5 
                ( m_pFeatures, pPlanObjekteINSPIRE, m_pUOMs, m_pTransformationINSPIRE_PLU_Parameter );
        pTransformation->transformationToPLU ( pPlan, vPlanBereiche, pProtokoll );
        delete pTransformation;
      }
  }

  if ( pPlanObjekteINSPIRE->getFeatureAnz( Constant::getKlassenNameINSPIRE( SPATIAL_PLAN )) == 0 )
    return pProtokoll;

  GMLWriter ^ pWriter = gcnew GMLWriter ( pPlanObjekteINSPIRE );
  int anz = pWriter->write ( zielDatei, outputFormat );
  delete pWriter;

  return pProtokoll;
}


///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Migration einer V4 Datei nach INSPIRE PLU:             //
//      Einlesen des INSPIRE PLU Schemas                                     //
///////////////////////////////////////////////////////////////////////////////
TransformationINSPIRE_PLU_Parameter * XPlanGML::readINSPIRE_PLUTransformation( GML_SCHEMA_TYPE versionXPlanGML )
{
  string                stdString;
  string                schemaDateiFolderINSPIRE;
  string                transformationsDateiFolderINSPIRE;
  string                pfadName;
  String              ^ SCHEMA_FOLDER;
  cli::array<String^> ^ FILES;
  IEnumerator         ^ FILES_ENUM;
  unsigned int          anzRead = 0;
  bool                  success;
  string                folder_4_1 = "GmlSchemata\\INSPIRE_LU_PLANNED\\Transformation_4_1";
  string                folder_5_0 = "GmlSchemata\\INSPIRE_LU_PLANNED\\Transformation_5_0";

  m_pTransformationINSPIRE_PLU_Parameter = new TransformationINSPIRE_PLU_Parameter;

  XPlanGML_INSPIRE_Transformation_Reader ^ pReader = 
    gcnew XPlanGML_INSPIRE_Transformation_Reader ( m_pTransformationINSPIRE_PLU_Parameter );

  if ( versionXPlanGML == XPLANGML_4_1 )
    transformationsDateiFolderINSPIRE = Constant::getSystemFolder() + folder_4_1;
  else
    if ( versionXPlanGML == XPLANGML_5_0 )
      transformationsDateiFolderINSPIRE = Constant::getSystemFolder() + folder_5_0;

  SCHEMA_FOLDER = QuConvert::ToString ( transformationsDateiFolderINSPIRE );
  bool exist = Directory::Exists ( SCHEMA_FOLDER );
  if ( exist )
  {
    FILES = Directory::GetFiles ( SCHEMA_FOLDER );

    FILES_ENUM = FILES->GetEnumerator();
    while ( FILES_ENUM->MoveNext() )
    {
      String^ FILE_NAME = safe_cast<String^>(FILES_ENUM->Current);
      QuConvert::systemStr2stdStr ( pfadName, FILE_NAME );
      success = pReader->read ( pfadName );
      if ( success ) 
        anzRead++;
    }
  }

  if ( anzRead == 0 ) 
  {
    delete m_pTransformationINSPIRE_PLU_Parameter;
    m_pTransformationINSPIRE_PLU_Parameter = NULL;
    return NULL;
  }
  else
    return m_pTransformationINSPIRE_PLU_Parameter;
}


///////////////////////////////////////////////////////////////////////////////
//  Verarbeitung der Shapefile string-Attribute	bei der Konversion  			   //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML::setStringAttribute ( Feature * pFeature, vector<AttributName*> vAttributNamenP,
              vector<unsigned int> vAttributAnzP , vector<std::string> vAttributWerteP  )
{
  GmlKlasse              * pGmlKlasse;
  AttributName           * pAttributObj;
  Enumeration            * pEnumeration;
  string			             fehlermeldung;
  string				           attributWert;
  string				           attributWertNeu;
  string                   classifierName;
  string                   classifierValue;
  string			             enumerationId;
  string			             attributName;
  string		          	   gmlIdNew;
  string                   referenziertesElement;
  Feature		             * pObjNew;
	size_t                   i, j, anz, anzRel, anzges;
  vector<FeatureRelation*> objRelationen;
  StringAttr             * pStringAttr;
  BoolAttr               * pBoolAttr;
  string                   klassenName = pFeature->getKlassenName();

  pGmlKlasse = m_pFeatures->getGmlSchema()->getGmlKlasseFromElementName( klassenName );

  anzges = 0;
  for ( i = 0; i < vAttributAnzP.size(); i++ )
  {
    anz		     = vAttributAnzP[i];
    attributWert = vAttributWerteP[i];

    for ( j = 0; j < anz; j++ )
    {
      pAttributObj = vAttributNamenP[anzges];
      anzges++;
      if ( pAttributObj == NULL )
        continue;

      attributName = pAttributObj->nameStandard;
      if ( attributWert == "" )
      {
        string text;
        text = "Warnung: Attribut " + attributName + " mit leerem String wurde nicht übernommen";
        m_pLogfile->writeLogFile ( text );
        continue;
      }

      if ( pAttributObj->typ == _STRING )
      {
        if ( attributWert != "" )
        {
          pStringAttr = new StringAttr;
          pStringAttr->value = attributWert;

          pFeature->setStringAttribut( attributName, pStringAttr );

          if ( attributName == "xplan:uuid" )
            addPlanObjektKonversion ( attributWert, pFeature );
        }
      }
      else
      if ( pAttributObj->typ == _BOOLEAN )
      {
        pBoolAttr = new BoolAttr;
        pBoolAttr->value = m_pFeatures->interpretBooleanAttribute ( attributWert );
        pFeature->setBoolAttribut( attributName, pBoolAttr );
      }
      else 
      if ( pAttributObj->typ == _DATE )
      {
        DateAttr     * pDate = new DateAttr;
        TimePosition * pDateValue = new TimePosition ( attributWert );
        pDate->value = pDateValue;
        pFeature->setDateAttribut( attributName, pDate );
      }
      else
      if ( pAttributObj->typ == _ANY_URI )
      {
        pStringAttr = new StringAttr;
        pStringAttr->value = attributWert;
        pFeature->setURLAttribut( attributName, pStringAttr );
      }
      else
      if ( pAttributObj->typ == _REFERENZ || 
           pAttributObj->typ == _DATA_TYPE )
      {
        FeatureRelation * pRel;
        GmlAttribut     * pGmlAttribut = pGmlKlasse->getGmlAttribut( attributName );
        if ( pGmlAttribut != NULL )
          referenziertesElement = pGmlAttribut->getReferenziertesElement();
        else
          referenziertesElement = "";

        pObjNew = NULL;
        objRelationen.clear();

        if ( pAttributObj->subAttributName == "xplan:uuid" )
        {
          pRel = new FeatureRelation;
          pRel->rollenName = attributName;
          pRel->uuid       = attributWert;

          pFeature->addFeatureRelation ( pRel );        
        }
        else
        {
          anzRel = pFeature->getFeatureRelationen ( attributName,  objRelationen );
          if ( anzRel == 0 || pAttributObj->generAttributName != "" )
          {
            gmlIdNew  = Constant::createGmlId();
            pObjNew = new Feature ( ANY_OBJEKT, gmlIdNew, m_pFeatures, referenziertesElement );
            if ( pAttributObj->generAttributName != "" )
              pObjNew->setStringAttribut ( "xplan:name", pAttributObj->generAttributName );
            m_pFeatures->addFeature ( pObjNew );
            if ( pAttributObj->typ == _DATA_TYPE )
            {
              pObjNew->setParentId( pFeature->getGmlId() );
              pFeature->addChild( pObjNew->getGmlId() );
            }

            pRel = new FeatureRelation;
            pRel->featureId = pObjNew->getGmlId();
            if ( pAttributObj->generAttributName != "" )
              pRel->rollenName = "xplan:hatGenerAttribut";
            else
              pRel->rollenName = attributName;

            pFeature->addFeatureRelation ( pRel );
          }
          else
					if (pAttributObj->getIndex() < (int)anzRel )
          {

            pRel = objRelationen[pAttributObj->getIndex()];
            pObjNew = m_pFeatures->getFeature ( pRel->featureId );
          }
        }

        if ( pObjNew != NULL )
        {
          switch ( pAttributObj->subAttributTyp )
          {
          case _ENUMERATION:
          case _EXTERNAL_CODE_LIST:
            if ( attributWert == "" ) continue;

            enumerationId = pAttributObj->enumerationId;
            pEnumeration = m_pShapeFileNamenKonvertierung->getEnumeration ( enumerationId );
            if ( pEnumeration != NULL )
            {
              attributWertNeu = pEnumeration->getEnumerationItem ( attributWert );
              if ( attributWertNeu == "Error" || attributWertNeu == "" )
              {
                fehlermeldung = "Ungültiger Attribut-Wert ";
                fehlermeldung += attributWert;
                fehlermeldung += " in Attribut ";
                fehlermeldung += pAttributObj->nameAktuell;
                m_pLogfile->writeLogFile ( fehlermeldung );
                continue;
              }
            }
            else
            {
              fehlermeldung = "Ungültige EnumerationsId: ";
              fehlermeldung += enumerationId;
              fehlermeldung += " in Attribut ";
              fehlermeldung += pAttributObj->nameStandard;
              m_pLogfile->writeLogFile ( fehlermeldung );
              return false;
            }

						pObjNew->deleteStringAttribut(pAttributObj->subAttributName);
            if ( pAttributObj->subAttributTyp == _ENUMERATION || 
                 getFeatures()->getGmlTyp() <=  XPLANGML_3_0 || 
                 pEnumeration->getDictionary() == ""  )
              pObjNew->setStringAttribut ( pAttributObj->subAttributName, attributWertNeu );
            else
            {
              classifierName = "codeSpace";
              classifierValue = pEnumeration->getDictionary();
               pObjNew->setStringAttribut ( pAttributObj->subAttributName, attributWertNeu ,
                        classifierName, classifierValue);
            }
            break;

          case _STRING:
						pObjNew->deleteStringAttribut( pAttributObj->subAttributName );
						pObjNew->setStringAttribut ( pAttributObj->subAttributName, attributWert );
            break;

          case _ANY_URI:
						pObjNew->deleteURLAttribut(pAttributObj->subAttributName);
						pObjNew->setURLAttribut ( pAttributObj->subAttributName, attributWert );
            break;

          case _BOOLEAN:
            bool b = XmlConvert::ToBoolean ( QuConvert::ToString ( attributWert ) );
            pObjNew->setBoolAttribut ( pAttributObj->subAttributName, b );
            break;
          }
        }

      }
			else
			if ( pAttributObj->typ == _TEXTE)
			{
				string						textAbschnittName;
				string						rechtscharakter;
				string						gmlId ;
				Feature					* pTextAbschnitt;
				Feature					* pExterneReferenz;
				FeatureRelation * pRel;

				if ( pFeature->getKlassenName() == "xplan:BP_Plan" )
				{
					textAbschnittName = "xplan:BP_TextAbschnitt";
					rechtscharakter   = "1000";
				}
				else
			  if ( pFeature->getKlassenName() == "xplan:FP_Plan" )
			  {
				  textAbschnittName = "xplan:FP_TextAbschnitt";
				  rechtscharakter   = "1000";
		  	}
			  else
				  continue;

				pTextAbschnitt = pFeature->getRelatedFeature ( "xplan:texte" );
				if (pTextAbschnitt == NULL)
				{
					gmlId =  Constant::createGmlId();
					pTextAbschnitt = new Feature ( ANY_OBJEKT, gmlId, m_pFeatures, textAbschnittName );
					pTextAbschnitt->setStringAttribut ( "xplan:rechtscharakter", rechtscharakter );
					m_pFeatures->addFeature ( pTextAbschnitt );

					pRel = new FeatureRelation;
					pRel->rollenName = "xplan:texte";
					pRel->featureId = gmlId;
					pFeature->addFeatureRelation ( pRel );
				}

				gmlId =  Constant::createGmlId();
				pExterneReferenz = new Feature ( ANY_OBJEKT, gmlId, m_pFeatures, "xplan:XP_ExterneReferenz" );
				pExterneReferenz->setURLAttribut ( "xplan:referenzURL", attributWert );
				pExterneReferenz->setStringAttribut ( "xplan:referenzName", attributWert );
				m_pFeatures->addFeature ( pExterneReferenz );

				TimePosition * pTimePosition = new TimePosition;
				pTimePosition->fromXMLString( "9999-01-01" );
				DateAttr * pDateAttr = new DateAttr;
				pDateAttr->value = pTimePosition;
				pExterneReferenz->setDateAttribut ( "xplan:datum", pDateAttr );

				pRel = new FeatureRelation;
				pRel->rollenName = "xplan:refText";
				pRel->featureId = gmlId;
				pTextAbschnitt->addFeatureRelation ( pRel );

				pExterneReferenz->setParentId ( pTextAbschnitt->getGmlId() );
				pTextAbschnitt->addChild ( gmlId );			
			}
      else
			if (pAttributObj->typ == _RASTERDARSTELLUNG )
			{
				string						gmlId ;
				Feature					* pBereich;
				Feature					* pExterneReferenz;
				Feature         * pRasterdarstellung;
				FeatureRelation * pRel;

				if ( pFeature->getKlassenName() != "xplan:BP_Plan" && 
					   pFeature->getKlassenName() != "xplan:FP_Plan" )
					continue;

				pBereich = pFeature->getRelatedFeature ( "xplan:bereich" );
				if ( pBereich == NULL )
					continue;

				gmlId = Constant::createGmlId();
				pExterneReferenz = new Feature ( ANY_OBJEKT, gmlId, m_pFeatures, "xplan:XP_ExterneReferenz" );
				pExterneReferenz->setStringAttribut ( "xplan:art", "PlanMitGeoreferenz" );
				pExterneReferenz->setURLAttribut ( "xplan:referenzURL", attributWert );

				size_t index = attributWert.find ( "." );
				string georefURL = attributWert.substr ( 0, index ) + ".pwg";
				pExterneReferenz->setURLAttribut ( "xplan:georefURL", georefURL );
				pExterneReferenz->setStringAttribut ( "xplan:referenzName", attributWert.substr ( 0, index ) );

				TimePosition * pTimePosition = new TimePosition;
				pTimePosition->fromXMLString( "9999-01-01" );
				DateAttr * pDateAttr = new DateAttr;
				pDateAttr->value = pTimePosition;
				pExterneReferenz->setDateAttribut ( "xplan:datum", pDateAttr );
				
				m_pFeatures->addFeature ( pExterneReferenz );

				if (m_pFeatures->getGmlSchema()->getGmlTyp() <= XPLANGML_5_0)
				{
					gmlId = Constant::createGmlId();
					if (m_pFeatures->getGmlSchema()->getGmlTyp() < XPLANGML_5_0)
						pRasterdarstellung = new Feature ( ANY_OBJEKT, gmlId, m_pFeatures, "xplan:XP_RasterplanBasis" );
					else
						pRasterdarstellung = new Feature ( ANY_OBJEKT, gmlId, m_pFeatures, "xplan:XP_Rasterdarstellung" );
					m_pFeatures->addFeature ( pRasterdarstellung );

					pRel = new FeatureRelation;
					pRel->rollenName = "xplan:rasterBasis";
					pRel->featureId = gmlId;

					pBereich->addFeatureRelation ( pRel );
				}
				else
					pRasterdarstellung = pBereich;

				pRel = new FeatureRelation;
				pRel->rollenName = "xplan:refScan";
				pRel->featureId = pExterneReferenz->getGmlId();
				pRasterdarstellung->addFeatureRelation ( pRel );

				pExterneReferenz->setParentId (pRasterdarstellung->getGmlId() );
				pRasterdarstellung->addChild ( pExterneReferenz->getGmlId() );		 						 
			}
			else
      if ( pAttributObj->typ == _POS_XY )
      {
        double x = 0;
        double y = 0;
				size_t index;
        string rWert, hWert;

        index = attributWert.find ( "," );
        if ( index != string::npos )
        {
          rWert = attributWert.substr ( 0, index );
          x = atof ( rWert.c_str() );
          hWert = attributWert.substr ( index + 1 );
          y = atof ( hWert.c_str() );
          GeoPoint * pPoint = new GeoPoint ( x, y );
          pFeature->addSymbolPosition ( pPoint );
        }
      }
      else
      if ( pAttributObj->typ == _ENUMERATION ||
           pAttributObj->typ == _EXTERNAL_CODE_LIST )
      {
        if ( attributWert == "" ) continue;

        enumerationId = pAttributObj->enumerationId;
        pEnumeration = m_pShapeFileNamenKonvertierung->getEnumeration ( enumerationId );
        if ( pEnumeration != NULL )
        {
          attributWertNeu = pEnumeration->getEnumerationItem ( attributWert );
          if ( attributWertNeu == "Error" || attributWertNeu == "" )
          {
            fehlermeldung = "Ungültiger Attribut-Wert ";
            fehlermeldung += attributWert;
            fehlermeldung += " in Attribut ";
            fehlermeldung += pAttributObj->nameAktuell;
            m_pLogfile->writeLogFile ( fehlermeldung );
            continue;
          }
        }
        else
        {
          fehlermeldung = "Ungültige EnumerationsId: ";
          fehlermeldung += enumerationId;
          fehlermeldung += " in Attribut ";
          fehlermeldung += pAttributObj->nameStandard;
          m_pLogfile->writeLogFile ( fehlermeldung );
          return false;
        }

        if ( pAttributObj->typ == _EXTERNAL_CODE_LIST && 
             m_pFeatures->getGmlTyp() >  XPLANGML_3_0       &&  
             pEnumeration->getDictionary() != "" )
        {
          classifierName = "codeSpace";
          classifierValue = pEnumeration->getDictionary();
          pFeature->setStringAttribut ( attributName, attributWertNeu, classifierName, classifierValue );
        }
        else
          pFeature->setStringAttribut ( attributName, attributWertNeu );
      }
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Verarbeitung der Shapefile Integer-Attribute ei der Konversion					 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::setIntegerAttribute ( Feature * pFeature, vector<AttributName*> vAttributNamenP, 
              vector<unsigned int> vAttributAnzP, vector<int> vAttributWerteP )
{
  GmlKlasse    * pGmlKlasse;
  String       ^ STR;
  AttributName * pAttributObj;
  string		     attributName;
  Enumeration  * pEnumeration;
  string			   fehlermeldung;
  string         gmlIdNew;
  string         stringWert;
  string         referenziertesElement;
  Feature      * pObjNew;
  IntAttr      * pIntAttr;
  DoubleAttr   * pDoubleAttr;
  StringAttr   * pStringAttr;
  string	       attributWertNeu, attributWertStr;
  string	       enumerationId;
	size_t         i, j, anz, anzges;
  int            attributWert;
  vector<FeatureRelation*> objRelationen;
  string                   klassenName = pFeature->getKlassenName();

  pGmlKlasse = m_pFeatures->getGmlSchema()->getGmlKlasseFromElementName( klassenName );

  anzges = 0;
  for ( i = 0; i < vAttributAnzP.size(); i++ )
  {
    anz		     = vAttributAnzP[i];
    attributWert = vAttributWerteP[i];
    STR = Convert::ToString( attributWert );
    if ( STR != nullptr )
      QuConvert::systemStr2stdStr( attributWertStr, STR );

    for ( j = 0; j < vAttributAnzP[i]; j++ )
    {
      pAttributObj = vAttributNamenP[anzges];
      anzges++;
      if ( pAttributObj == NULL )
        continue;

      attributName = pAttributObj->nameStandard;
      if ( m_pFeatures->isXPlanGML() && attributWert == 0 && attributName != "xplan:ebene" )
      {
        string text;
        text = "Warnung: Attribut " + attributName + " mit Wert 0 wurde nicht übernommen";
        m_pLogfile->writeLogFile ( text );
        continue;
      }

      if ( pAttributObj->typ == _INTEGER )
      {
        pIntAttr = new IntAttr;
        pIntAttr->value = attributWert;
        pFeature->setIntegerAttribut( attributName, pIntAttr );
      }
      else
      if ( pAttributObj->typ == _STRING )
      {
        STR = XmlConvert::ToString ( attributWert );
        QuConvert::systemStr2stdStr ( stringWert, STR );
        pStringAttr = new StringAttr;
        pStringAttr->value = stringWert;
        pFeature->setStringAttribut( attributName, pStringAttr );
      }
      else
      if ( pAttributObj->typ == _ANGLE )
      {
        pDoubleAttr = new DoubleAttr;
        pDoubleAttr->value = attributWert;
        pDoubleAttr->uom =  m_pUOMs->getUomAngle();
        pFeature->setDoubleAttribut( attributName, pDoubleAttr );
      }
      else
      if ( pAttributObj->typ == _LENGTH )
      {
        pDoubleAttr = new DoubleAttr;
        pDoubleAttr->value = attributWert;
        pDoubleAttr->uom =  m_pUOMs->getUomLength();
        pFeature->setDoubleAttribut(attributName, pDoubleAttr );

      }
      else
      if ( pAttributObj->typ == _AREA )
      {
        pDoubleAttr = new DoubleAttr;
        pDoubleAttr->value = attributWert;
        pDoubleAttr->uom =  m_pUOMs->getUomArea();
        pFeature->setDoubleAttribut( attributName, pDoubleAttr );
      }
      else
      if ( pAttributObj->typ == _VOLUME )
      {
        pDoubleAttr = new DoubleAttr;
        pDoubleAttr->value = attributWert;
        pDoubleAttr->uom =  m_pUOMs->getUomVolume();
        pFeature->setDoubleAttribut( attributName, pDoubleAttr );
      }
      else
      if ( pAttributObj->typ == _REFERENZ || pAttributObj->typ == _DATA_TYPE )
      {
        FeatureRelation * pRel;
        GmlAttribut     * pGmlAttribut = pGmlKlasse->getGmlAttribut( attributName );
        if ( pGmlAttribut != NULL ) referenziertesElement = pGmlAttribut->getReferenziertesElement();
        else                        referenziertesElement = "";

        pObjNew = NULL;
        objRelationen.clear();

        anz = pFeature->getFeatureRelationen ( attributName,  objRelationen );
        if ( anz == 0 || pAttributObj->generAttributName != "" )
        {
          gmlIdNew  = Constant::createGmlId();
          pObjNew = new Feature ( ANY_OBJEKT, gmlIdNew, m_pFeatures, referenziertesElement );
          if ( pAttributObj->generAttributName != "" )
            pObjNew->setStringAttribut ( "xplan:name", pAttributObj->generAttributName );
          m_pFeatures->addFeature ( pObjNew );
          if ( pAttributObj->typ == _DATA_TYPE )
          {
            pObjNew->setParentId( pFeature->getGmlId() );
            pFeature->addChild( pObjNew->getGmlId() );
          }

          pRel = new FeatureRelation;
          pRel->featureId = pObjNew->getGmlId();
          if ( pAttributObj->generAttributName != "" )
            pRel->rollenName = "xplan:hatGenerAttribut";
          else
            pRel->rollenName = attributName;

          pFeature->addFeatureRelation ( pRel );        
        }
        else
			  if (pAttributObj->getIndex() < (int)anz )
				{
          pRel = objRelationen[pAttributObj->getIndex()];
          pObjNew = m_pFeatures->getFeature ( pRel->featureId );
        }

        if ( pObjNew != NULL )
        {
          switch ( pAttributObj->subAttributTyp )
          {
          case _INTEGER:
						pObjNew->deleteIntegerAttribut ( pAttributObj->subAttributName );
            pObjNew->setIntegerAttribut ( pAttributObj->subAttributName, attributWert );
            break;
          }
        }
      }
      else
      if ( pAttributObj->typ == _ENUMERATION ||
           pAttributObj->typ == _EXTERNAL_CODE_LIST )
      {
        if ( attributWertStr == "" ) continue;

        enumerationId = pAttributObj->enumerationId;
        pEnumeration = m_pShapeFileNamenKonvertierung->getEnumeration ( enumerationId );
        if ( pEnumeration != NULL )
        {
          attributWertNeu = pEnumeration->getEnumerationItem ( attributWertStr );
          if ( attributWertNeu == "Error" || attributWertNeu == "" )
          {
            fehlermeldung = "Ungültiger Attribut-Wert ";
            fehlermeldung += attributWert;
            fehlermeldung += " in Attribut ";
            fehlermeldung += pAttributObj->nameAktuell;
            m_pLogfile->writeLogFile ( fehlermeldung );
            continue;
          }

        }
        else
        {
          fehlermeldung = "Ungültige EnumerationsId: ";
          fehlermeldung += enumerationId;
          fehlermeldung += " in Attribut ";
          fehlermeldung += pAttributObj->nameStandard;
          m_pLogfile->writeLogFile ( fehlermeldung );
          continue;
        }
        pStringAttr = new StringAttr;
        pStringAttr->value = attributWertNeu;

        if ( pAttributObj->typ == _EXTERNAL_CODE_LIST && 
             getFeatures()->getGmlTyp() >  XPLANGML_3_0 &&  
             pEnumeration->getDictionary() != "" )
        {
          pFeature->setStringAttribut ( attributName, attributWertNeu, "codeSpace", pEnumeration->getDictionary() );
        }
        else
          pFeature->setStringAttribut ( attributName, attributWertNeu );
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Verarbeitung der Shapefile Double-Attribute bei der Konversion       		 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::setDoubleAttribute ( Feature * pFeature, vector<AttributName*> vAttributNamenP, 
              vector<unsigned int> vAttributAnzP, vector<double> vAttributWerteP )
{
  GmlKlasse              * pGmlKlasse;
  AttributName           * pAttributObj;
  string		               attributName;
  string                   gmlIdNew;
  string                   stringWert;
  string                   referenziertesElement;
  String                 ^ STR;
  Feature             * pObjNew;
  double		               attributWert;
	size_t      	           i, j, anz, anzges;
  DoubleAttr             * pDoubleAttr;
  StringAttr             * pStringAttr;
  vector<FeatureRelation*> objRelationen;
  string                   klassenName = pFeature->getKlassenName();

  pGmlKlasse = m_pFeatures->getGmlSchema()->getGmlKlasseFromElementName( klassenName );

  anzges = 0;
  for ( i = 0; i < vAttributAnzP.size(); i++ )
  {
    anz		     = vAttributAnzP[i];
    attributWert = vAttributWerteP[i];

    for ( j = 0; j < anz; j++ )
    {
      pAttributObj = vAttributNamenP[anzges];
      anzges++;
      if ( pAttributObj == NULL )
        continue;

      attributName = pAttributObj->nameStandard;

      if ( m_pFeatures->isXPlanGML()  && attributWert == 0.0  )
      {
        string text;
        text = "Warnung: Attribut " + attributName + " mit Wert 0 wurde nicht übernommen";
        m_pLogfile->writeLogFile ( text );
        continue;
      }

      if ( pAttributObj->typ == _DOUBLE  )
      {
        pDoubleAttr = new DoubleAttr;
        pDoubleAttr->value = attributWert;
        pDoubleAttr->uom = NULL;
        pFeature->setDoubleAttribut( attributName, pDoubleAttr );
      }
      else
      if ( pAttributObj->typ == _STRING )
      {
        STR = XmlConvert::ToString ( attributWert );
        QuConvert::systemStr2stdStr ( stringWert, STR );
        pStringAttr = new StringAttr;
        pStringAttr->value = stringWert;
        pFeature->setStringAttribut( attributName, pStringAttr );
      }
      else
      if ( pAttributObj->typ == _ANGLE )
      {
        pDoubleAttr = new DoubleAttr;
        pDoubleAttr->value = attributWert;
        pDoubleAttr->uom =  m_pUOMs->getUOMFromName( "DEGREE" );
        pFeature->setDoubleAttribut( attributName, pDoubleAttr );
      }
      else
      if ( pAttributObj->typ == _LENGTH )
      {
        pDoubleAttr = new DoubleAttr;
        pDoubleAttr->value = attributWert;
        pDoubleAttr->uom =  m_pUOMs->getUomLength();
        pFeature->setDoubleAttribut(  attributName, pDoubleAttr );
      }
      else
      if ( pAttributObj->typ == _AREA )
      {
        pDoubleAttr = new DoubleAttr;
        pDoubleAttr->value = attributWert;
        pDoubleAttr->uom =  m_pUOMs->getUomArea();
        pFeature->setDoubleAttribut( attributName, pDoubleAttr );
      }
      else
      if ( pAttributObj->typ == _VOLUME )
      {
        pDoubleAttr = new DoubleAttr;
        pDoubleAttr->value = attributWert;
        pDoubleAttr->uom =  m_pUOMs->getUomVolume();
        pFeature->setDoubleAttribut( attributName, pDoubleAttr );
      }      
      else
      if (  pAttributObj->typ == _REFERENZ || 
            pAttributObj->typ == _DATA_TYPE )
      {
        FeatureRelation * pRel;
        GmlAttribut     * pGmlAttribut = pGmlKlasse->getGmlAttribut( attributName );

        if ( pGmlAttribut != NULL )
          referenziertesElement = pGmlAttribut->getReferenziertesElement();
        else
          referenziertesElement = "";

        pObjNew = NULL;
        objRelationen.clear();

        anz = pFeature->getFeatureRelationen ( attributName,  objRelationen );
        if ( anz == 0 || pAttributObj->generAttributName != "" )
        {
          gmlIdNew  = Constant::createGmlId();
          pObjNew = new Feature ( ANY_OBJEKT, gmlIdNew, m_pFeatures, referenziertesElement );
          if ( pAttributObj->generAttributName != "" )
            pObjNew->setStringAttribut ( "xplan:name", pAttributObj->generAttributName );
          m_pFeatures->addFeature ( pObjNew );
          if ( pAttributObj->typ == _DATA_TYPE )
          {
            pObjNew->setParentId( pFeature->getGmlId() );
            pFeature->addChild( pObjNew->getGmlId() );
          }

          pRel = new FeatureRelation;
          pRel->featureId = pObjNew->getGmlId();
          if ( pAttributObj->generAttributName != "" )
            pRel->rollenName = "xplan:hatGenerAttribut";
          else
            pRel->rollenName = attributName;

          pFeature->addFeatureRelation ( pRel );        
        }
        else
				if (pAttributObj->getIndex() < (int)anz )
				{
          pRel = objRelationen[pAttributObj->getIndex()];
          pObjNew = m_pFeatures->getFeature ( pRel->featureId );
        }				

        if ( pObjNew != NULL )
        {
					pObjNew->deleteDoubleAttribut ( pAttributObj->subAttributName );
          switch ( pAttributObj->subAttributTyp )
          {
          case _DOUBLE:
            pObjNew->setDoubleAttribut ( pAttributObj->subAttributName, attributWert );
            break;

          case _LENGTH:
            pObjNew->setDoubleAttribut ( pAttributObj->subAttributName, attributWert, m_pUOMs->getUomLength() );
            break;

          case _AREA:
            pObjNew->setDoubleAttribut ( pAttributObj->subAttributName, attributWert, m_pUOMs->getUomArea() );
            break;

          case _VOLUME:
            pObjNew->setDoubleAttribut ( pAttributObj->subAttributName, attributWert,  m_pUOMs->getUomVolume() );
            break;

          case _ANGLE:
            pObjNew->setDoubleAttribut ( pAttributObj->subAttributName, attributWert,  m_pUOMs->getUomAngle() );
            break;
          }
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Verarbeitung der Shapefile Datum-Attribute bei der Konversion      			 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML::setDateAttribute ( Feature * pFeature, vector<AttributName*> vAttributNamenP, 
                    vector<unsigned int> vAttributAnzP, vector<string> vAttributWerteP )
{
  AttributName * pAttributObj;
  string		   attributName;
  string         gmlIdNew;
  string         stringWert;
  string		     attributWert;
  unsigned int	 i, j, anz, anzges;
  DateAttr     * pDate;
  TimePosition * pDateValue;
  vector<FeatureRelation*> objRelationen;
	Feature                * pObjNew;

  anzges = 0;
  for ( i = 0; i < vAttributAnzP.size(); i++ )
  {
    anz		     = vAttributAnzP[i];
    attributWert = vAttributWerteP[i];

    for ( j = 0; j < anz; j++ )
    {
      pAttributObj = vAttributNamenP[anzges];
      anzges++;
      if ( pAttributObj == NULL )
        continue;

      attributName = pAttributObj->nameStandard;

      if ( pAttributObj->typ == _DATE )
      {
        if ( attributWert.size() < 8 ) continue;
        string year  = attributWert.substr ( 0, 4 );
        string month = attributWert.substr ( 4, 2 );
        string day   = attributWert.substr ( 6, 2 );

        pDateValue = new TimePosition ( atoi ( year.c_str() ), atoi ( month.c_str() ), atoi ( day.c_str() ) );
        pDate      = new DateAttr;
        pDate->value = pDateValue;
        pFeature->setDateAttribut( attributName, pDate );
      }
      else
      if (  pAttributObj->typ == _REFERENZ ||  pAttributObj->typ == _DATA_TYPE )
      {
        if ( attributName == "xplan:hatGenerDatumAttribut" )
        {
          if ( attributWert.size() < 8 ) continue;

          gmlIdNew             = Constant::createGmlId();
          pObjNew = new Feature ( ANY_OBJEKT, gmlIdNew, m_pFeatures, "XP_DatumAttribut" );

          if ( attributWert.size() < 8 ) continue;
          string year  = attributWert.substr ( 0, 4 );
          string month = attributWert.substr ( 4, 2 );
          string day   = attributWert.substr ( 6, 2 );

          pDateValue = new TimePosition ( atoi ( year.c_str() ), atoi ( month.c_str() ), atoi ( day.c_str() ) );
          pDate = new DateAttr;
          pDate->value = pDateValue;

          pObjNew->setStringAttribut ( "xplan:name", pAttributObj->generAttributName );

          pObjNew->setDateAttribut ( "xplan:wert", pDate );
          m_pFeatures->addFeature ( pObjNew );
          if ( pAttributObj->typ == _DATA_TYPE )
          {
            pObjNew->setParentId( pFeature->getGmlId() );
            pFeature->addChild( pObjNew->getGmlId() );
          }

          FeatureRelation * pRel = new FeatureRelation;
          pRel->rollenName = "hatGenerAttribut";
          pRel->featureId = gmlIdNew;
          pFeature->addFeatureRelation ( pRel );		    
        }
				else
				{
					string                   klassenName = pFeature->getKlassenName();
					GmlKlasse * pGmlKlasse = m_pFeatures->getGmlSchema()->getGmlKlasseFromElementName( klassenName );
					FeatureRelation * pRel;
					string            referenziertesElement;

					GmlAttribut     * pGmlAttribut = pGmlKlasse->getGmlAttribut( attributName );
					if ( pGmlAttribut != NULL )
						referenziertesElement = pGmlAttribut->getReferenziertesElement();
					else
						referenziertesElement = "";

					pObjNew = NULL;
					objRelationen.clear();

					size_t anzRel = pFeature->getFeatureRelationen ( attributName,  objRelationen );
					if ( anzRel == 0 )
					{
						gmlIdNew  = Constant::createGmlId();
						pObjNew = new Feature ( ANY_OBJEKT, gmlIdNew, m_pFeatures, referenziertesElement );
						m_pFeatures->addFeature ( pObjNew );
						if ( pAttributObj->typ == _DATA_TYPE )
						{
							pObjNew->setParentId( pFeature->getGmlId() );
							pFeature->addChild( pObjNew->getGmlId() );
						}

						pRel = new FeatureRelation;
						pRel->featureId = pObjNew->getGmlId();
  					pRel->rollenName = attributName;

						pFeature->addFeatureRelation ( pRel );
					}
					else
					if (pAttributObj->getIndex() < anzRel )
				  {
							pRel = objRelationen[pAttributObj->getIndex()];
							pObjNew = m_pFeatures->getFeature ( pRel->featureId );
					}
				}
				if (pObjNew != NULL)
				{
					if ( attributWert.size() < 8 ) continue;
					string year  = attributWert.substr ( 0, 4 );
					string month = attributWert.substr ( 4, 2 );
					string day   = attributWert.substr ( 6, 2 );

					//string dateString = year + "-" + month + "-" + day;
					pObjNew->deleteDateAttribut(pAttributObj->subAttributName);

					pDateValue = new TimePosition ( atoi ( year.c_str() ), atoi ( month.c_str() ), atoi ( day.c_str() ) );
					pDate      = new DateAttr;
					pDate->value = pDateValue;
					pObjNew->setDateAttribut( pAttributObj->subAttributName, pDate );
				}
      }
    }
  }
}
