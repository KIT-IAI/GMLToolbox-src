#include "stdafx.h"

#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <stdlib.h>

#include "Geometrie.h"
#include "Convert.h"
#include "Constant.h"
#include "GmlSchema.h"
#include "GmlReader.h"
#include "GmlWriter.h"
#include "Appearance.h"
#include "TimeX.h"
#include "TimeSeries.h"
#include "UnitOfMeasure.h"
#include "Logfile.h"
#include "SrsManagerRef.h"
#include "Feature.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  Features															                                	 //
//  Aggregation aller Features  							                               //
///////////////////////////////////////////////////////////////////////////////
Features::Features ( UOMList * pUOMs, SrsManager * pSrsManager, Logfile * pLogfile )
{
	m_pUOMs             = pUOMs;
  m_pSrsManager       = pSrsManager;
  m_pLogfile          = pLogfile;

	gmlId               = Constant::createGmlId();
  pGmlSchema          = NULL;

  srsDimension = -1;
  srsName      = "Local";

  shapeFileBooleanValueTrue.insert( "true" );
  shapeFileBooleanValueTrue.insert( "ja" );
  shapeFileBooleanValueTrue.insert( "yes" );
  shapeFileBooleanValueTrue.insert( "0" );
  shapeFileBooleanValueTrue.insert( "staedtebaulicher vertrag" );
  shapeFileBooleanValueTrue.insert ( "durchführungsvertrag" );

  boreholeRadius             = 0.5;
  boreholeCylinderComplexity = 24;
  boreholeShowIntervals      = true;
  dazuladen                  = false;
}

Features::~Features()
{
	map<string,Feature*>::iterator							iter;
	map<unsigned int,cAppearance*>::iterator    iterAppearance;
	map<unsigned int,AppearanceData*>::iterator iterAppearanceData;
	map<unsigned int,TexCoord*>::iterator       iterTexCoord;


	for ( iter = features.begin(); iter != features.end(); iter++ )
  {
    Feature * pFeature = iter->second;
		delete pFeature;
  }

	for ( iterAppearance = mAppearance.begin(); iterAppearance != mAppearance.end(); iterAppearance++ )
		delete iterAppearance->second;

	for ( iterAppearanceData = mAppearanceData.begin(); iterAppearanceData != mAppearanceData.end(); iterAppearanceData++ )
		delete iterAppearanceData->second;

	for ( iterTexCoord = mTexCoord.begin(); iterTexCoord != mTexCoord.end(); iterTexCoord++ )
	{
		TexCoord * pTexCoord = iterTexCoord->second;
		delete pTexCoord;
	}


  if ( pGmlSchema != NULL )     delete pGmlSchema;
}

///////////////////////////////////////////////////////////////////////////////
//  Lösche den Inhalt bis auf das GML-Schema                                 //
///////////////////////////////////////////////////////////////////////////////
void Features::clear()
{
  map<string,Feature*>::iterator              iter;
//  map<string,AbstractTimeSeries*>::iterator   iterTimeSeries;
  map<unsigned int,cAppearance*>::iterator    iterAppearance;
  map<unsigned int,AppearanceData*>::iterator iterAppearanceData;
  map<unsigned int,TexCoord*>::iterator       iterTexCoord;

  for ( iter = features.begin(); iter != features.end(); iter++ )
  {
    try
    {
      string gmlId = iter->first;
      Feature * pFeature = iter->second;
      delete pFeature;
      features[gmlId] = NULL;
    }
    catch ( System::AccessViolationException ^  )
    {
    }
  }

  for ( iterAppearance = mAppearance.begin(); iterAppearance != mAppearance.end(); iterAppearance++ )
    delete iterAppearance->second;

  for ( iterAppearanceData = mAppearanceData.begin(); iterAppearanceData != mAppearanceData.end(); iterAppearanceData++ )
    delete iterAppearanceData->second;

  for ( iterTexCoord = mTexCoord.begin(); iterTexCoord != mTexCoord.end(); iterTexCoord++ )
	{
		TexCoord * pTexCoord = iterTexCoord->second;
    delete pTexCoord;
	}

  mGeometryMap.clear();
  features.clear();
  mAppearance.clear();
  mAppearanceData.clear();
  mTexCoord.clear();
  mGeoAppearanceRef.clear();

  m_vPlaene.clear();
  m_mPlanMap.clear();

  srsName      = "";
  gmlId        = "";
  srsDimension = -1;
  dazuladen    = false;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das Kordinaten-Referenzsystem und die GML Id zurück                //
///////////////////////////////////////////////////////////////////////////////
void Features::reset()
{
  srsName             = "";
  gmlId               = "";
  srsDimension        = -1;
  dazuladen           = true;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob ein String bereits als gml:id verwendet wird               //
///////////////////////////////////////////////////////////////////////////////
bool Features::existGmlId( std::string gmlIdP )
{
  if ( features.find( gmlIdP ) != features.end() )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
// Modifiziert einen übergebenen String, um eine noch nicht benutzte gml:id  //
// zu generieren                                                             //
///////////////////////////////////////////////////////////////////////////////
std::string Features::modifyGmlId ( std::string gmlId )
{
  string gmlIdNeu = gmlId;
  string str_i;
  int   i = 1;
  char buffer[5];

  while ( i < 9999 )
  {
    _itoa_s ( i, buffer, 5, 10);
    gmlIdNeu = gmlId + "_" + string ( buffer );
    if ( existGmlId ( gmlIdNeu ) == false )
      return gmlIdNeu;
    else
      i++;
  }

  return gmlId;
}

///////////////////////////////////////////////////////////////////////////////
//  Übernimmt das GML Applikationsschema                                     //
//  Ein evtl. schon zugewiesenes Schema wird gelöscht                        //
///////////////////////////////////////////////////////////////////////////////
void Features::setGmlSchema ( GmlSchema * pGmlSchemaP )
{
  if ( pGmlSchema != NULL ) delete pGmlSchema;
  pGmlSchema = pGmlSchemaP;
}

///////////////////////////////////////////////////////////////////////////////
//  Tauscht das GML Applikationsschema aus, ohne das vorhandene Schema zu    //
//  löschen                                                                  //
///////////////////////////////////////////////////////////////////////////////
GmlSchema * Features::exchangeGmlSchema ( GmlSchema * pGmlSchemaNew )
{
  GmlSchema * pGmlSchemaOld = pGmlSchema;
  pGmlSchema = pGmlSchemaNew;
  return pGmlSchemaOld;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die GML-Namespaces                                               //
///////////////////////////////////////////////////////////////////////////////
GmlNamespaces * Features::getGmlNamespaces ()
{
  if ( pGmlSchema == NULL ) return NULL;
  else return pGmlSchema->getGmlNamespaces();
}


///////////////////////////////////////////////////////////////////////////////
//  Setzt das Attribut srsName                                               //
///////////////////////////////////////////////////////////////////////////////
void Features::setSrsName ( std::string name )
{  
  srsName = name;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Attribute srsName und srsDimension                             //
///////////////////////////////////////////////////////////////////////////////
void Features::setSrsNameAndDimension ( std::string name )
{  
  srsName = name;  
  srsDimension = m_pSrsManager->getSrsDimension( name );
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die Dimension des Koordinaten-Referenzsystems                    //
///////////////////////////////////////////////////////////////////////////////
int Features::getSrsDimension ()
{  
  if ( srsDimension > 0 )
    return srsDimension;
  else
    return getSrsDimensionFromSchemaType();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die standardmäßige Dimension des Koordinaten-Referenzsystems     //
//  für ein bestimmtes Schema                                                //
///////////////////////////////////////////////////////////////////////////////
int Features::getSrsDimensionFromSchemaType ()
{
  if ( isXPlanGML() || isINSPIREPLU() || isALKIS() )
    return 2;

  if ( isCityGML() || isBoreholeML() )
    return 3;

  return -1;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob auf Ebene der FeatureCollection ein gültiges Koordinaten-  //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool Features::existReferenceSystem()
{
  if ( srsName == "" )
    return false;

  return m_pSrsManager->existCoordSystem( srsName );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den GML-Typ                                                      //
///////////////////////////////////////////////////////////////////////////////
GML_SCHEMA_TYPE Features::getGmlTyp()
{
  if ( pGmlSchema != NULL ) return pGmlSchema->getGmlTyp();
  else                      return GML_SCHEMA_UNKNOWN;
}

std::string Features::getGmlTypAsString()
{
  if ( pGmlSchema != NULL ) return pGmlSchema->getGmlTypAsString();
  else                     return "Unbekanntes Schema";
}

///////////////////////////////////////////////////////////////////////////////
// Liefert ob ein XPlanGML Modell geladen wurde                              //
///////////////////////////////////////////////////////////////////////////////
bool Features::isXPlanGML()
{
  if ( pGmlSchema == NULL ) return false;

  GML_SCHEMA_TYPE gmlTyp = pGmlSchema->getGmlTyp();

  if ( gmlTyp == XPLANGML_2_0 || 
       gmlTyp == XPLANGML_3_0 || 
       gmlTyp == XPLANGML_4_0 ||
       gmlTyp == XPLANGML_4_1 || 
       gmlTyp == XPLANGML_5_0 ||
       gmlTyp == XPLANGML_5_1 ||
       gmlTyp == XPLANGML_5_2 ||
       gmlTyp == XPLANGML_5_3 )
    return true;

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Information, ob ein INSPIRE_PLU Modell geladen wurde         //
///////////////////////////////////////////////////////////////////////////////
bool Features::isINSPIREPLU()
{
  if ( pGmlSchema == NULL ) return false;

  GML_SCHEMA_TYPE gmlTyp = pGmlSchema->getGmlTyp();

  if ( gmlTyp == INSPIRE_LU_PLANNED )
    return true;

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Information, ob ein INSPIRE_Building Modell geladen wurde    //
///////////////////////////////////////////////////////////////////////////////
bool Features::isINSPIRE()
{
  if ( pGmlSchema == NULL  ) return false;

  GML_SCHEMA_TYPE gmlTyp = pGmlSchema->getGmlTyp();

  if ( gmlTyp == INSPIRE )
    return true;

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Information, ob ein CityGML Modell geladen wurde             //
///////////////////////////////////////////////////////////////////////////////
bool Features::isCityGML()
{
  if ( pGmlSchema == NULL  ) return false;

  GML_SCHEMA_TYPE gmlTyp = pGmlSchema->getGmlTyp();

  if ( gmlTyp == CITYGML_0_4_0 || 
       gmlTyp == CITYGML_1_0   || 
       gmlTyp == CITYGML_2_0   ||
       gmlTyp == CITYGML_3_0 )
    return true;

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Information, ob ein ALKIS Modell geladen wurde               //
///////////////////////////////////////////////////////////////////////////////
bool Features::isALKIS()
{
  if ( pGmlSchema == NULL  ) return false;

  GML_SCHEMA_TYPE gmlTyp = pGmlSchema->getGmlTyp();

  if ( gmlTyp == NAS_5_1   ||
       gmlTyp == NAS_5_1_1 ||
       gmlTyp == NAS_6_0 )
    return true;

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Information, ob ein BoreholeML Modell geladen wurde          //
///////////////////////////////////////////////////////////////////////////////
bool Features::isBoreholeML()
{
  if ( pGmlSchema == NULL  ) return false;

  GML_SCHEMA_TYPE gmlTyp = pGmlSchema->getGmlTyp();

  if ( gmlTyp == BOREHOLEML_3_0 )
    return true;

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Feature hinzu					                          							 //
///////////////////////////////////////////////////////////////////////////////
void Features::addFeature ( Feature * pFeatureP, string dateiName )
{
	string id = pFeatureP->getGmlId();
  string idNeu;

  if ( existGmlId( id ) )
  {
    idNeu = modifyGmlId ( id );
    pFeatureP->setGmlId( idNeu );

    string errorMessage = "Doppelte gml:id im Datensatz: \"" + id + "\"\r\n";
    errorMessage = errorMessage + "Geändert in: \"" + idNeu + "\"";
    m_pLogfile->writeLogFile( errorMessage );
  }
  else
    idNeu = id;

	features[idNeu] = pFeatureP;
  if ( dateiName != "" )
    featureDateien[idNeu] = dateiName;

  if ( pFeatureP->getFeatures() == NULL )
    pFeatureP->setFeatures ( this );

	if ( pFeatureP->getObjektTyp() == BLEIT_PLAN )
		m_vPlaene.push_back ( (BLeitPlan*)pFeatureP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die XML-Datei, in dem ein Feature definiert ist                  //                            
///////////////////////////////////////////////////////////////////////////////
std::string Features::getFeatureDatei ( std::string gmlId )
{
  if ( featureDateien.find( gmlId ) != featureDateien.end() )
    return featureDateien[gmlId];
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt alle Features in die angegebene Datei                           //
///////////////////////////////////////////////////////////////////////////////
int Features::writeFeatures ( string ausgabePath )
{
  GMLWriter ^ pWriter = gcnew GMLWriter ( this );
  int         outputFormat;

  if ( pGmlSchema->isInspirePlu() )
    outputFormat = 1;
  else
    outputFormat = 0;

  int anz = pWriter->write ( ausgabePath, outputFormat );
  delete pWriter;

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest die angegebene Datei ein                                           //
///////////////////////////////////////////////////////////////////////////////
int Features::readFeatures ( std::string fileName, bool dazuladen, bool readCodeListDirectories, Checking * pChecking )
{
  int anz;

  if ( dazuladen )
    reset();
  else
    clear();  

  GMLReader ^ pReader = gcnew GMLReader ( this, readCodeListDirectories );
  anz = pReader->read ( fileName, pChecking );

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Geometrie zur map mGeometryMap hinzu                           //
///////////////////////////////////////////////////////////////////////////////
void Features::addGeometry ( _Geometrie * pGeometry )
{
  string gmlId;
  if ( pGeometry != NULL )
  {
    gmlId = pGeometry->getGmlId();
    mGeometryMap[gmlId] = pGeometry;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Geometrieobjekt aus der map ,GeometryMap                     //
///////////////////////////////////////////////////////////////////////////////
_Geometrie * Features::getGeometry ( std::string gmlId )
{
  map<string,_Geometrie*>::iterator iter;

  iter = mGeometryMap.find( gmlId );
  if ( iter != mGeometryMap.end() )
    return iter->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt ein Geometrieobjekt aus der map mGeometryMap                    //
///////////////////////////////////////////////////////////////////////////////
void Features::removeGeometry ( std::string gmlId )
{
  mGeometryMap.erase( gmlId );
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob alle Geometrie-Referenzen valide sind                      //
///////////////////////////////////////////////////////////////////////////////
void Features::checkGeometryReferences ( std::vector<Error*> &vErrorReport )
{
  map<string,Feature*>::iterator iterFeatures;
  vector<_Geometrie*>            vGeometrie;
  vector<string>                 vPropertyNames;
  Feature                      * pFeature;
  _Geometrie                   * pGeo;
  string                         propertyName;
	size_t                         i, anz;

  for ( iterFeatures = features.begin(); iterFeatures != features.end(); iterFeatures++  )
  {
    pFeature     = iterFeatures->second;

    vGeometrie.clear();
    vPropertyNames.clear();
    anz = pFeature->getGeometryProperties ( vPropertyNames, vGeometrie );
    for ( i = 0; i < anz; i++ )
    {
      pGeo         =  vGeometrie[i];
      propertyName = vPropertyNames[i];
      pGeo->checkReferences( pFeature, propertyName, vErrorReport );  
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Kopiert die referierte Curve-Geometrie für OrientableCurve Objekte       //
///////////////////////////////////////////////////////////////////////////////
void Features::resolveOrientableCurveGeometry()
{
	std::map<std::string,_Geometrie*>::iterator iter;

	for ( iter = mGeometryMap.begin(); iter != mGeometryMap.end(); ++iter )
	{
		_Geometrie * pGeo = iter->second;
		if ( pGeo->getGeometryType() == ORIENTABLE_CURVE )
		{
			OrientableCurve * pOrientableCurve = (OrientableCurve*)pGeo;
			if ( pOrientableCurve->getBaseCurve() == NULL ) 
			{
				string baseCurveID = pOrientableCurve->getBaseCurveId();
				_Geometrie * pRefGeo = getGeometry ( baseCurveID );
				if ( pRefGeo != NULL )
				{
					switch ( pRefGeo->getGeometryType() )
					{
					case CURVE:
					{
						Curve * pNewCurve = new Curve ( (Curve*)pRefGeo );
						pOrientableCurve->setBaseCurve ( pNewCurve );
						break;
					}

					case LINE_STRING:
					case LINE_STRING_SEGMENT:
					{
						LineSegment * pLineSegment = new LineSegment ( (LineSegment*)pRefGeo );
						pOrientableCurve->setBaseCurve ( pLineSegment );
						break;
					}

					case ARC_STRING:
					{
						ArcSegment * pArcSegment = new ArcSegment ( (ArcSegment*)pRefGeo );
						pOrientableCurve->setBaseCurve ( pArcSegment );
						break;
					}
					
					}
				
				}
			}
		}
	
	}
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Features, die im Strukturbaum an oberster Stelle stehen,    //
//  d.h. die kein anderes Feature als Parent haben                           //
//  Die MultiMap mChildren liefert diese Features mit dem Klassennamen als   //
//  Schlüssel.                                                               //
//  Alle in mChildren vorkommenden Klassen werden im Set sKlassenNamen       //
//  geliefert                                                                //
///////////////////////////////////////////////////////////////////////////////
void Features::getChildren ( multimap<string,Feature*> &mChildren, set<string> &sKlassenNamen )
{
  string    klassenName;
  Feature * pFeature;

  map<string,Feature*>::iterator iterFeatures;

  for ( iterFeatures = features.begin(); iterFeatures != features.end(); iterFeatures++ )
  {
    pFeature    = iterFeatures->second;

    if ( pFeature->getParentId() == "" )
    {
      klassenName = pFeature->getKlassenName();
      sKlassenNamen.insert( klassenName );
      mChildren.insert( multimap<string,Feature*>::value_type ( klassenName, pFeature ) );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle genutzte Klassen                                            //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getUsedClasses ( set<string> &sKlassenNamen )
{
  Feature * pFeature;

  map<string,Feature*>::iterator iter;

  for ( iter = features.begin(); iter != features.end(); iter++ )
  {
    pFeature = iter->second;
    sKlassenNamen.insert( pFeature->getKlassenName() );
  }
  return sKlassenNamen.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML: Liefert ein XPlanGML Plan-Objekt               						 //
///////////////////////////////////////////////////////////////////////////////
BLeitPlan * Features::getBLeitPlan ( unsigned int indexP )
{
	if ( indexP < 0 || indexP >= m_vPlaene.size() ) return NULL;
	else										                        return m_vPlaene[indexP];
}

BLeitPlan * Features::getBLeitPlan ( std::string planId )
{
	map<string,BLeitPlan*>::iterator pos;

	pos = m_mPlanMap.find ( planId );
	if ( pos != m_mPlanMap.end() ) return pos->second;
	else						               return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML: Liefert ein Objekt BPlanBereich über die GML Id 					 //
///////////////////////////////////////////////////////////////////////////////
PlanBereich * Features::getPlanBereich ( std::string bereichIdP )
{
	Feature * pObj = getFeature ( bereichIdP );
	if ( pObj != NULL && pObj->getObjektTyp() == PLAN_BEREICH )
		return (PlanBereich*)pObj;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Feature über die GML Id                       							 //
///////////////////////////////////////////////////////////////////////////////
Feature * Features::getFeature ( std::string objektIdP )
{
	map<string,Feature*>::iterator iter;

	iter = features.find ( objektIdP );
	if ( iter != features.end() ) return iter->second;
	else						      return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Features   							                        					 //
//  Bei XPlanGML-Modellen mit der Einschränkung, das sie zu einem bestimmten //
//  Plan und Plan-Bereich gehören                                            //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getFeatures ( vector<Feature*> &vFeatures,  BLeitPlan * pPlanAktuell, PlanBereich * pPlanBereichAktuell  )
{
  if ( isXPlanGML() )
  {   
    if ( pPlanAktuell == NULL && pPlanBereichAktuell == NULL ) return 0;

    if ( pPlanBereichAktuell != NULL )
        pPlanBereichAktuell->getBPlanObjekte ( vFeatures, true );
    vFeatures.push_back( pPlanAktuell );
  }  

	return vFeatures.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Features, ggf. sortiert nach den Klassen-Namen              //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getSortedFeatures ( vector<Feature*> &vFeatures, bool sort )
{
  map<string,Feature*>::iterator      iter;
  multimap<string,Feature*>           sortedFeatures;
  multimap<string,Feature*>::iterator itersf;
  vector<PlanBereich*>                vPlanBereiche;
  Feature                           * pFeature;
  string                              klassenName;

/*  if ( isXPlanGML() )
  {
    for ( i = 0; i < m_vPlaene.size(); i++ )
    {
      pPlan = m_vPlaene[i];
      vFeatures.push_back ( pPlan );
      vPlanBereiche.clear();
      bereichAnz =  pPlan->getPlanBereiche ( vPlanBereiche );
      for ( j = 0; j < bereichAnz; j++ )
      {
        pBereich =   vPlanBereiche[j];
        pBereich->getBPlanObjekte ( vFeatures, true );
      }
    }   
  }*/
//  else
//  {
    if ( sort )
    {
      for ( iter = features.begin(); iter != features.end(); iter++ )
      {
        pFeature = iter->second;
        klassenName = pFeature->getKlassenName();
        sortedFeatures.insert(  multimap<string,Feature*>::value_type ( klassenName, pFeature ) );
      }
      for ( itersf = sortedFeatures.begin(); itersf != sortedFeatures.end(); itersf++ )
        vFeatures.push_back( itersf->second );
    }
    else
      for ( iter = features.begin(); iter != features.end(); iter++ )
        vFeatures.push_back( iter->second );
 // }

  return vFeatures.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Features mit dem übergebenen Klassen-Namen                  //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getFeatures ( std::vector<Feature*> &vFeatures, std::string klassenName )
{
  map<string,Feature*>::iterator iter;

  for ( iter = features.begin (); iter != features.end(); iter++ )
  {
    Feature * pFeature = iter->second;
    if ( pFeature->getKlassenName() == klassenName )
      vFeatures.push_back( iter->second );
  }

  return vFeatures.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Features mit dem übergebenen Klassen-Namen        //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getFeatureAnz ( std::string klassenName )
{
	size_t anz = 0;
  map<string,Feature*>::iterator iter;

  for ( iter = features.begin (); iter != features.end(); iter++ )
  {
    Feature * pFeature = iter->second;
    if ( pFeature->getKlassenName() == klassenName )
      anz++;
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Gesamt-Anzahl der Features                                   //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getFeatureAnz ()
{
  return features.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Features                                                    //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getAllFeatures ( std::vector<Feature*> &vFeatures )
{
  map<string,Feature*>::iterator iter;

  for ( iter = features.begin (); iter != features.end(); iter++ )
  {
    Feature * pFeature = iter->second;
    vFeatures.push_back( iter->second );
  }
  return vFeatures.size();
}

size_t Features::getAllFeatures ( std::multimap<std::string,Feature*> &mFeatures )
 {
    map<string,Feature*>::iterator iter;

   for ( iter = features.begin(); iter != features.end(); iter++ )
   {
     Feature * pFeature = iter->second;
     string klassenName = pFeature->getKlassenName();
     mFeatures.insert(  multimap<string,Feature*>::value_type ( klassenName, pFeature ) );
   }
   return mFeatures.size();
 }


///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML: Liefert alle Objekte BLeitPlan                          	 //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getPlaene( std::vector<BLeitPlan*> &vplaene )
{
	unsigned int i;

	for ( i = 0; i < m_vPlaene.size(); i++ )
	{
		BLeitPlan * pBLeitPlan = m_vPlaene[i];
			vplaene.push_back ( pBLeitPlan );
	}

	return vplaene.size();
}

///////////////////////////////////////////////////////////////////////////////
//   Liefert die BoundingBox aller Features 	                        			 //
///////////////////////////////////////////////////////////////////////////////
bool Features::getBoundingBox ( double * sw, double * no, string &srsNameP, int &srsDimensionP )
{
	size_t                         i, anz;
	double                         swLoc[3], noLoc[3];
	bool                           success;
  _Geometrie                   * pGeometrie;
  Feature                      * pFeature;
  map<string,Feature*>::iterator iter;
  vector<Feature*>               vFeatures;
  string                         srsNameLoc;
  int                            srsDimensionLoc;

  srsNameP = srsName;

  sw[0] =  1.0E10;
  sw[1] =  1.0E10;
  sw[2] =  1.0E10;
  no[0] = -1.0E10;
  no[1] = -1.0E10;
  no[2] = -1.0E10;

  if ( isINSPIREPLU() )
  {
    anz = getFeatures ( vFeatures, Constant::getKlassenNameINSPIRE( SPATIAL_PLAN ) );
    for ( i = 0; i < anz; i ++ )
    {
      pFeature = vFeatures[i];
      pGeometrie = pFeature->getGeometry( "plu:extent", 0 );
      if ( pGeometrie != NULL )
      {
        success = pGeometrie->getBoundingBox( swLoc, noLoc );
        if ( success )
        {
          if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
          if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
          if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
          if ( noLoc[1] > no[1] ) no[1] = noLoc[1];

          if ( srsNameP == "" )
            srsNameP = pGeometrie->getSrsName();

          if ( srsDimensionP < 0 )
            srsDimensionP = pGeometrie->getDim();
        }
      }
    }
    sw[2] = 0.0;
    no[2] = 0.0;

    if ( srsDimension < 0 ) 
      srsDimension = 2;
    srsDimensionP = srsDimension;

    return true;
  }

  if ( isXPlanGML() )
  {
    if ( m_vPlaene.size() == 0 ) return false;

	  for ( i = 0; i < m_vPlaene.size(); i++ )
	  {
  		BLeitPlan * pPlan = m_vPlaene[i];
      pGeometrie = pPlan->getGeometry( "xplan:raeumlicherGeltungsbereich", 0 );
		  if ( pGeometrie != NULL )
		  {
			  success = pGeometrie->getBoundingBox ( swLoc, noLoc );
			  if ( success )
			  {
				  if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
				  if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
				  if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
				  if ( noLoc[1] > no[1] ) no[1] = noLoc[1];

          if ( srsNameP == "" )
            srsNameP = pGeometrie->getSrsName();

          if ( srsDimensionP < 0 )
            srsDimensionP = pGeometrie->getDim();
			  }
		  }
      else
      {
        vector<PlanBereich*> vPlanBereiche;
				size_t                bereichAnz, j;

        bereichAnz = pPlan->getPlanBereiche( vPlanBereiche );
        for ( j = 0; j < bereichAnz; j++ )
        {
          PlanBereich * pPlanBereich = vPlanBereiche[j];

          pGeometrie = pPlanBereich->getGeometry( "xplan:geltungsbereich", 0 );
          if ( pGeometrie != NULL )
          {
            success = pGeometrie->getBoundingBox ( swLoc, noLoc );
            if ( success )
            {
              if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
              if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
              if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
              if ( noLoc[1] > no[1] ) no[1] = noLoc[1];

              if ( srsNameP == "" )
                srsNameP = pGeometrie->getSrsName();

              if ( srsDimensionP < 0 )
                srsDimensionP = pGeometrie->getDim();
            }
          }
          else
            break;
        }
        if ( !success )
        {
          for ( iter = features.begin(); iter != features.end(); iter++ )
          {
            pFeature = iter->second;
            success =  pFeature->getBoundingBox ( swLoc, noLoc, srsNameLoc, srsDimensionLoc, false );
            if ( success )
            {
              if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
              if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
              if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
              if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
              if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
              if ( noLoc[2] > no[2] ) no[2] = noLoc[2];

              if ( srsNameP == "" )
                srsNameP = srsNameLoc;

              if ( srsDimensionP < 0 )
                srsDimensionP = srsDimensionLoc;
            }
          }
        }
      }
	  }

    sw[2] = 0.0;
    no[2] = 0.0;

    if ( srsDimension < 0 ) 
      srsDimension = 2;
    srsDimensionP = srsDimension;

	  return true;
  }

  if ( isCityGML() || isINSPIRE() || isBoreholeML() )
  {
    for ( iter = features.begin(); iter != features.end(); iter++ )
    {
      pFeature = iter->second;
      success =  pFeature->getBoundingBox ( swLoc, noLoc, srsNameLoc, srsDimensionLoc, false );
      if ( success )
      {
        if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
        if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
        if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
        if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
        if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
        if ( noLoc[2] > no[2] ) no[2] = noLoc[2];

        if ( srsNameP == "" )
          srsNameP = srsNameLoc;

        if ( srsDimensionP < 0 )
          srsDimensionP = srsDimensionLoc;
      }
    }

    if ( srsDimension < 0 ) 
      srsDimension = 3;
    srsDimensionP = srsDimension;
    return true;
  }

  if ( isALKIS() )
  {
    for ( iter = features.begin(); iter != features.end(); iter++ )
    {
      pFeature = iter->second;
      success =  pFeature->getBoundingBox ( swLoc, noLoc, srsNameLoc, srsDimensionLoc, false );
      if ( success )
      {
        if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
        if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
        if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
        if ( noLoc[1] > no[1] ) no[1] = noLoc[1];

        if ( srsNameP == "" )
          srsNameP = srsNameLoc;

        if ( srsDimensionP < 0 )
          srsDimensionP = srsDimensionLoc;
      }
    }

    if ( srsDimension < 0 ) 
      srsDimension = 2;
    srsDimensionP = srsDimension;

    return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt alle Features um einen offset                      					 //
///////////////////////////////////////////////////////////////////////////////
void Features::offset ( GeoPoint & point )
{
  map<string,Feature*>::iterator iter;
  set<_Geometrie*> geometries;

  for ( iter = features.begin(); iter != features.end(); iter++ )
  {
    Feature * pFeature = iter->second;
    geometries.clear();
    pFeature->offset( point, geometries );
  }  
}


///////////////////////////////////////////////////////////////////////////////
//  Löscht ein Feature								                            					 //
///////////////////////////////////////////////////////////////////////////////
bool Features::removeFeature ( std::string objId )
{
  Feature * pFeature;
  Feature * pParent;

	map<string,Feature*>::iterator pos;

	pos = features.find( objId );
	if ( pos != features.end() )
	{
		if ( pos->second != NULL )
    {
      pFeature = pos->second;
      pParent = pFeature->getParent();
      if ( pParent != NULL )
        pParent->removeChild( objId );
      delete pFeature;
    }
		features.erase ( pos );
		return true;
	}
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt alle gml_id Attribute neu		                    								 //
///////////////////////////////////////////////////////////////////////////////
void Features::updateGmlIds()
{
	map<string,Feature*>           mPlanObjekteSave;
	map<string,Feature*>::iterator iter;
	map<string,string>             mGmlIds;
  vector<string>                 vChildIds;
	vector<FeatureRelation*>       vObjRelationen;
	Feature                      * pPlanObjekt;
	FeatureRelation              * pRel;
	string							           gmlId, gmlIdOld;
  string                         parentId;
  string                         childId;
	size_t                         i, anz;

	mPlanObjekteSave = features;

	features.clear();

	for ( iter = mPlanObjekteSave.begin(); iter != mPlanObjekteSave.end(); iter++ )
	{
		pPlanObjekt = iter->second;
		gmlIdOld = pPlanObjekt->getGmlId();

		gmlId       = Constant::createGmlId();
		mGmlIds[gmlIdOld] = gmlId;
		pPlanObjekt->setGmlId ( gmlId );
		features[gmlId] = pPlanObjekt;
	}

	mPlanObjekteSave.clear();

	for ( iter = features.begin(); iter != features.end(); iter++ )
	{
		vObjRelationen.clear();

		pPlanObjekt = iter->second;

    parentId = pPlanObjekt->getParentId();
    if ( parentId != "" )
      pPlanObjekt->setParentId( mGmlIds[parentId] );

    anz = pPlanObjekt->getChildrenIds( vChildIds );
    pPlanObjekt->removeAllChildren();
    for ( i = 0; i < anz; i++ )
    {
      childId = mGmlIds[vChildIds[i]];
      pPlanObjekt->addChild( childId );
    }

		anz = pPlanObjekt->getFeatureRelationen ( vObjRelationen );
		for ( i = 0; i < anz; i++ )
		{
			pRel = vObjRelationen[i];
			gmlId = mGmlIds[pRel->featureId];
			pRel->featureId = gmlId;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML: Generiert eine Map aller Pläne. Schlüsselattribut ist      //
//  das Attribut __planId der Plan-Objekte									                 //
///////////////////////////////////////////////////////////////////////////////
bool Features::createPlanMap()
{
	unsigned int					 i;
	bool							 success;
	string							 planId;
	map<string,BLeitPlan*>::iterator pos;

	for ( i = 0; i < m_vPlaene.size(); i++ )
	{
		BLeitPlan * pPlan = m_vPlaene[i];
    success = pPlan->getStringAttributWert ( "xplan:__planId", planId );
		if ( !success ) return false;

		pos = m_mPlanMap.find ( planId );
		if ( pos == m_mPlanMap.end() ) m_mPlanMap[planId] = pPlan;
		else						return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur für Alkis-Modelle                                                    //
//  Macht spezifische Modifikationen:                                        //
//  - Entfernung der Objektkoordinaten aus AX_Flurstueck-Objekten            //
///////////////////////////////////////////////////////////////////////////////
void Features::modifyAlkisModel()
{
  Feature                      * pFeature;
  map<string,Feature*>::iterator iter;

  if ( !isALKIS() ) return;

  for ( iter = features.begin(); iter != features.end(); iter++ )
  {
    pFeature = iter->second;
    if ( pFeature->getKlassenName() == "adv:AX_Flurstueck" )
    {
      pFeature->deleteGeometryProperty( "adv:objektkoordinaten" );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Nur für BoreholeML                                                       //
//  Modifiziert die BoreholeML Objekte Interval und Component, so dass sie   //
//  explizite Geometrieinformationen hnben                                   //
///////////////////////////////////////////////////////////////////////////////
void Features::createBMLVisualizationGeometry()
{
  vector<Feature*> vBoreholes;
  vector<Feature*> vIntervalSeries;
  vector<Feature*> vLayers;
  vector<Feature*> vComponents;
	size_t           boreholeAnz, i, l, lEnd, pointAnz;
	int              j, k;
  _Curve         * pBoreholePath;
  GeoPoint       * pBoreholePosition = nullptr;
  Feature        * pBorehole;
  Feature        * pIntervalSeries;
  Feature        * pLayer;
  double           from;
  double           to;
  int              layerAkt;
  bool             successFrom, successTo;
  GeoPoint         pA (3), pE(3);
  GeoPoint         nLineA(3), nLineE(3);
  GeoPoint         nArea(3);
  GeoPoint         pLine(3);
  GeoPoint         pArea(3);
  GeoPoint         p(3);
  GeoPoint         s1(3), s2(3), s3(3), s4(3);
  PolygonGeo        * pPoly = nullptr;
  LinearRing     * pRing;
  Surface        * pSurface;
  vector<GeoPoint> vPointsA;
  vector<GeoPoint> vPointsE;
  vector<GeoPoint> vPolyPoints;
  vector<GeoPoint> vBoreholePoints;
  vector<GeoPoint> vBoreholePointsLayer;
  vector<int>      vLayerNum;
  string           srsName;

  getFeatures( vBoreholes, "bml:Borehole");
  getFeatures( vBoreholes, "bmlh:BoreholeHeader");
  boreholeAnz =  vBoreholes.size();

  for ( i = 0; i < boreholeAnz; i++ )
  {
    vPointsA.clear();
    vPointsE.clear();

    pBorehole = vBoreholes[i];
    if ( pBorehole->getKlassenName() == "bml:Borehole" )
      pBoreholePosition = dynamic_cast<GeoPoint*>(pBorehole->getGeometry( "bml:location", 0 ));
    else
      if ( pBorehole->getKlassenName() == "bmlh:BoreholeHeader" )
        pBoreholePosition = dynamic_cast<GeoPoint*>(pBorehole->getGeometry( "bmlh:location", 0 ));

    if ( pBoreholePosition != nullptr )
    {
      srsName = pBoreholePosition->getSrsName();
      nLineA.set ( 0.0, 0.0, 1.0 );
      _Geometrie::generateNPolygon( pBoreholePosition, boreholeRadius, nLineA, boreholeCylinderComplexity, vPointsA );
      pPoly = new PolygonGeo ( 3 );
      pPoly->setGmlId( Constant::createGmlId() );
      if ( srsName != "" )
        pPoly->setSrsName( srsName );
      addGeometry ( pPoly );
      pRing = new LinearRing ( 3 );
      pRing->addPoints( vPointsA );
      pPoly->setAussenkontur( pRing );

      pBorehole->addGeometryProperty( "bml:boreholeLocationGeometry", pPoly );
    }

    if ( boreholeShowIntervals )
    {
      pBoreholePath = dynamic_cast<_Curve*>(pBorehole->getGeometry( "bml:boreholePath", 0 ));
      if ( pBoreholePath == NULL )
        return;

      srsName = pBoreholePath->getSrsName();

      vIntervalSeries.clear();
      pBorehole->getRelatedFeatures( "bml:intervalSeries", vIntervalSeries );
      for ( j = 0; j < vIntervalSeries.size(); j++ )
      {
        pIntervalSeries = vIntervalSeries[j];
        vLayers.clear();
        vLayerNum.clear();
        vBoreholePoints.clear();

        pIntervalSeries->getRelatedFeatures( "bml:layer", vLayers );
        if ( vLayers.size() == 0 ) return;

        for ( k = 0; k < vLayers.size(); k++ )
        {
          vBoreholePointsLayer.clear();
          pLayer = vLayers[k];
          successFrom = pLayer->getDoubleAttributWert( "bml:from", from );
          successTo   = pLayer->getDoubleAttributWert ( "bml:to", to );
          if ( successFrom && successTo )
          {
            pointAnz = pBoreholePath->extractCurvePart( from, to, vBoreholePointsLayer );
            if ( k < vLayers.size() - 1 ) lEnd = pointAnz - 1;
            else                          lEnd = pointAnz;
            for ( l = 0; l < lEnd; l++ )
            {
              vBoreholePoints.push_back( vBoreholePointsLayer[l] );
              vLayerNum.push_back( k );
            }
          }
        }

        pA = vBoreholePoints[0];
        pE = vBoreholePoints[1];
        nLineA =  pE - pA;
        nLineA.normalize();
        layerAkt = vLayerNum[0];
        pSurface = new Surface (3);
        pSurface->setGmlId( Constant::createGmlId() );
        pSurface->setSrsName( srsName );
        addGeometry ( pSurface );

        _Geometrie::generateNPolygon( pA, boreholeRadius, nLineA, boreholeCylinderComplexity, vPointsA );

        pA = pE;

        for ( k = 1; k < vBoreholePoints.size(); k++ )
        {
          if ( k < vBoreholePoints.size() - 1 )
          {
            pE = vBoreholePoints[k+1];
            nLineE =  pE - pA;
            nLineE.normalize();

            nArea = nLineA + nLineE;
            nArea.normalize();
          }
          else
            nArea = nLineA;

          for ( l = 0; l < boreholeCylinderComplexity; l++ )
          {
            pLine = vPointsA[l];
            _Geometrie::lineAreaCut( pLine, nLineA, pA, nArea, p );
            vPointsE.push_back( p );
          }

          //  Zylindermantel Erzeugen
          for ( l = 0; l < boreholeCylinderComplexity; l++ )
          {
            s1 = vPointsA[l];
            s2 = vPointsE[l];
            if ( l < boreholeCylinderComplexity - 1 )
            {
              s4 = vPointsA[l+1];
              s3 = vPointsE[l+1];
            }
            else
            {
              s4 = vPointsA[0];
              s3 = vPointsE[0];
            }
            vPolyPoints.clear();
            pPoly = new PolygonGeo ( 3 );
            pPoly->setGmlId (  Constant::createGmlId() );
            addGeometry ( pPoly );
            pPoly->setSrsName( srsName );
            pSurface->addPolygon( pPoly );

            pRing = new LinearRing ( 3 );
            pPoly->setAussenkontur( pRing );

            vPolyPoints.push_back( s1 );
            vPolyPoints.push_back( s2 );
            vPolyPoints.push_back( s4 );
            vPolyPoints.push_back( s1 );
            pRing->addPoints( vPolyPoints );

            vPolyPoints.clear();
            pPoly = new PolygonGeo ( 3 );
            pPoly->setGmlId (  Constant::createGmlId() );
            addGeometry ( pPoly );
            pPoly->setSrsName( srsName );
            pSurface->addPolygon( pPoly );

            pRing = new LinearRing ( 3 );
            pPoly->setAussenkontur( pRing );

            vPolyPoints.push_back( s2 );
            vPolyPoints.push_back( s3 );
            vPolyPoints.push_back( s4 );
            vPolyPoints.push_back( s2 );
            pRing->addPoints( vPolyPoints );
          }

          pA     = pE;
          nLineA = nLineE;
          vPointsA.clear();
          for ( l = 0; l < boreholeCylinderComplexity; l++ )
            vPointsA.push_back(vPointsE[l]);
          vPointsE.clear();

          if ( vLayerNum[k] != layerAkt )
          {
            vLayers[layerAkt]->addGeometryProperty( "bml:layerGeometry", pSurface );
            layerAkt = vLayerNum[k];
            pSurface = new Surface ( 3 );
            pSurface->setGmlId(  Constant::createGmlId() );
            addGeometry ( pPoly );
          }
        }
        vLayers[layerAkt]->addGeometryProperty( "bml:layerGeometry", pSurface );
      }
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Fügt ein Appearance Objekt hinzu                                         //
///////////////////////////////////////////////////////////////////////////////
void Features::addAppearance ( cAppearance * pAppearance )
{
  unsigned int id = pAppearance->getAppearId();
  mAppearance[id] = pAppearance;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Fügt ein AppearanceData Objekt hinzu                                     //
///////////////////////////////////////////////////////////////////////////////
void Features::addAppearanceData ( AppearanceData * pAppearanceData )
{
  unsigned int id = pAppearanceData->getAppearDataId();
  mAppearanceData[id] = pAppearanceData;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Fügt ein TexCoord Objekt hinzu                                           //
///////////////////////////////////////////////////////////////////////////////
void Features::addTexCoord ( TexCoord * pTexCoord )
{
  unsigned int id = pTexCoord->getTexCoordId();
  mTexCoord[id] = pTexCoord;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Liefert ein TexCoord Objekt                                              //
///////////////////////////////////////////////////////////////////////////////
TexCoord * Features::getTexCoord ( unsigned int id )
{
  map<unsigned int,TexCoord*>::iterator pos;

  pos = mTexCoord.find ( id );
  if ( pos != mTexCoord.end() ) return pos->second;
  else                          return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Liefert ein Appearance Objekt                                              //
///////////////////////////////////////////////////////////////////////////////
cAppearance * Features::getAppearance ( unsigned int id )
{
  map<unsigned int,cAppearance*>::iterator iter;

  iter = mAppearance.find ( id );
  if ( iter != mAppearance.end() ) return iter->second;
  else                             return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Liefert die Namen aller Appearance Themen                                //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getAllThemes( set<string> &sThemes )
{
  map<unsigned int,cAppearance*>::iterator iter;
  string                                   theme;

  for ( iter = mAppearance.begin(); iter != mAppearance.end(); iter++ )
  {
    cAppearance * pApp = iter->second;
    theme              = pApp->getThema();
    sThemes.insert( theme );
  }

  return sThemes.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Liefert alle Appearance Objekte                                          //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getAllAppearances ( vector<cAppearance*> & appearances )
{
  map<unsigned int,cAppearance*>::iterator iter;

  for ( iter = mAppearance.begin(); iter != mAppearance.end(); iter++ )
    appearances.push_back ( iter->second );

  return appearances.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Liefert ein AppearanceData Objekt                                        //
///////////////////////////////////////////////////////////////////////////////
AppearanceData * Features::getAppearanceData ( unsigned int id )
{
  map<unsigned int,AppearanceData*>::iterator iter;

  iter = mAppearanceData.find ( id );
  if ( iter != mAppearanceData.end() ) return iter->second;
  else                                 return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Liefert alle AppearanceData Objekte                                      //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getAllAppearanceData ( std::vector<AppearanceData*> & appearData )
{
  map<unsigned int,AppearanceData*>::iterator iter;

  for ( iter = mAppearanceData.begin(); iter != mAppearanceData.end(); iter++ )
    appearData.push_back ( iter->second );

  return appearData.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Liefert ein GeoAppearanceRef Objekt hinzu                                //
///////////////////////////////////////////////////////////////////////////////
void Features::addGeoAppearanceMaterial ( string geomId, string thema, unsigned int appearDataId, bool overwrite )
{
  multimap<string,GeoAppearanceRef>::iterator iter;
  GeoAppearanceRef                            ref;
  AppearanceData                            * pAppearanceData;

  for ( iter = mGeoAppearanceRef.lower_bound( geomId );
    iter != mGeoAppearanceRef.upper_bound ( geomId ); iter++ )
  {
    ref = iter->second;
    pAppearanceData = getAppearanceData ( ref.m_appearData );
    if ( thema == ref.m_thema )
    {
      if ( overwrite )
      {
        mGeoAppearanceRef.erase( iter );
        break;
      }
      else
        return;
    }
  }

  ref.m_thema      = thema;
  ref.m_appearData = appearDataId;
  ref.m_texCoordId = 0;

  mGeoAppearanceRef.insert (std::multimap<string, GeoAppearanceRef>::value_type ( geomId, ref));
}

void Features::addGeoAppearanceTexture ( string geomId, string thema, unsigned int idAppear, unsigned int idCoords )
{
  GeoAppearanceRef ref;

  ref.m_thema      = thema;
  ref.m_appearData = idAppear;
  ref.m_texCoordId = idCoords;

  mGeoAppearanceRef.insert (std::multimap<string, GeoAppearanceRef>::value_type ( geomId, ref));
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Liefert die einem Geometrie-Objekt zugeordneten ApparanceData Objekte    //
//  und die zugehörigen Appearance-Themen                                    //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getGeoAppearance( string geomId, vector<string> &thema, vector<AppearanceData*> & appearData )
{
  multimap<string,GeoAppearanceRef>::iterator iter;
  GeoAppearanceRef                            ref;
  AppearanceData                            * pAppearanceData;

  for ( iter = mGeoAppearanceRef.lower_bound( geomId );
    iter != mGeoAppearanceRef.upper_bound ( geomId ); iter++ )
  {
    ref = iter->second;
    pAppearanceData = getAppearanceData ( ref.m_appearData );
    thema.push_back ( ref.m_thema );
    appearData.push_back ( pAppearanceData );
  }

  return thema.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Liefert die einem Geometrie-Objekt zugeordneten ApparanceData Objekte,   //
//  die eine Farbe (X3DMaterial) beinhalten, und die zugehörigen             //
//  Appearance-Themen                                                        //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getGeoAppearanceColor ( string geomId, vector<string> &thema, vector<AppearanceData*> & appearData )
{
  multimap<string,GeoAppearanceRef>::iterator iter;
  GeoAppearanceRef                            ref;
  AppearanceData                            * pAppearanceData;

  for ( iter = mGeoAppearanceRef.lower_bound( geomId );
    iter != mGeoAppearanceRef.upper_bound ( geomId ); iter++ )
  {
    ref = iter->second;
    pAppearanceData = getAppearanceData ( ref.m_appearData );
    if ( pAppearanceData->getAppearanceDataType () == AppearanceData::MATERIAL )
    {
      thema.push_back ( ref.m_thema );
      appearData.push_back ( pAppearanceData );
    }
  }

  return thema.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Liefert die einem Geometrie-Objekt zugeordneten ApparanceData Objekte,   //
//  die eine Textur (TexCoordPolyloop) beinhalten, und die zugehörigen       //
//  Appearance-Themen                                                        //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getGeoAppearanceTexture ( string geomId, vector<string> &thema, vector<AppearanceData*> & appearData, 
                                          vector<TexCoordPolyloop*> & texCoord )
{
  multimap<string,GeoAppearanceRef>::iterator iter;
  GeoAppearanceRef                            ref;
  AppearanceData                            * pAppearanceData;
  TexCoord                                  * pTexCoord;

  for ( iter = mGeoAppearanceRef.lower_bound( geomId );
    iter != mGeoAppearanceRef.upper_bound ( geomId ); iter++ )
  {
    ref = iter->second;
    pAppearanceData = getAppearanceData ( ref.m_appearData );
    if ( pAppearanceData->getAppearanceDataType () == AppearanceData::PARAMETRIZED_TEXTURE  ||
      pAppearanceData->getAppearanceDataType () == AppearanceData::GEOREFERENCED_TEXTURE ) 
    {
      if ( ref.m_texCoordId != 0 )
      {
        pTexCoord = getTexCoord (  ref.m_texCoordId );
        if ( pTexCoord && pTexCoord->getTextureCoordType() == TexCoord::TEXTURE_COORD_POLYLOOP )
        {
          thema.push_back ( ref.m_thema );
          appearData.push_back ( pAppearanceData );
          texCoord.push_back ( (TexCoordPolyloop*)pTexCoord );
        }
      }
    }
  }

  return thema.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML: Liefert alle vorhandenen LOD-Stufen                         //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getLODStufen ( vector<CITYGML_LOD> &vLodStufen )
{
  set<CITYGML_LOD>                  sLODStufen;
  set<CITYGML_LOD>::iterator        iterLODStufen;
  map<string,Feature*>::iterator    iterFeatures;
  map<string,_Geometrie*>::iterator iterGeometrie;
  vector<string>                    vPropertyNames;
  vector<_Geometrie*>               vGeometries;
  Feature                         * pFeature;
  string                            propertyName;
	size_t                            i, anz;

  if (! pGmlSchema->isCityGml() )
    return 0;

  for ( iterFeatures = features.begin(); iterFeatures != features.end(); iterFeatures++ )
  {
    pFeature = iterFeatures->second;

    vPropertyNames.clear();
    vGeometries.clear();

    anz = pFeature->getGeometryProperties( vPropertyNames, vGeometries );
    for ( i = 0; i < anz; i++ )
    {
      propertyName = vPropertyNames[i];

      if ( pGmlSchema->getGmlTyp() == CITYGML_0_4_0 || 
           pGmlSchema->getGmlTyp() == CITYGML_1_0 ||
           pGmlSchema->getGmlTyp() == CITYGML_2_0 || 
           pGmlSchema->getGmlTyp() == CITYGML_3_0 )
      {
        if ( propertyName.find( "lod0" ) != string::npos )
          sLODStufen.insert( LOD_0 );
        else
        if ( propertyName.find( "lod1" ) != string::npos )
          sLODStufen.insert( LOD_1 );
        else
        if ( propertyName.find( "lod2" ) != string::npos )
          sLODStufen.insert( LOD_2 );
        else
        if ( propertyName.find( "lod3" ) != string::npos )
          sLODStufen.insert( LOD_3 );
        else
        if ( propertyName.find( "lod4" ) != string::npos )
          sLODStufen.insert( LOD_4 );
      }      
    }
  }

  for ( iterLODStufen = sLODStufen.begin(); iterLODStufen != sLODStufen.end(); iterLODStufen++ )
    vLodStufen.push_back( *iterLODStufen );  
  
  return vLodStufen.size();
}

size_t Features::getLODStufenAsString ( vector<string> &vLodStufen )
{
  vector<CITYGML_LOD> vLodStufenEnum;
	size_t              i, anz;
  string              str;

  anz = getLODStufen( vLodStufenEnum );
  for ( i = 0; i < anz; i++ )
  {
    str = lodStufeToString ( vLodStufenEnum[i] );
    vLodStufen.push_back( str );
  }

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Transformiert die Angabe einer LOD-Stufe zwischen string-Repräsentation  //
//  und enum-Repräsentation                                                  //
///////////////////////////////////////////////////////////////////////////////
std::string Features::lodStufeToString ( CITYGML_LOD lodStufe )
{
  switch ( lodStufe )
  {
  case LOD_0:  return "LOD 0";
  case LOD_1:  return "LOD 1";
  case LOD_2:  return "LOD 2";
  case LOD_3:  return "LOD 3";
  case LOD_4:  return "LOD 4";
  case ALL:    return "Beliebig";
  }

  return "";
}

CITYGML_LOD Features::lodStufeToEnum ( std::string lodStufe )
{
  if ( lodStufe == "LOD 0" ) return LOD_0;
  else
  if ( lodStufe == "LOD 1" ) return LOD_1;
  else
  if ( lodStufe == "LOD 2" ) return LOD_2;
  else
  if ( lodStufe == "LOD 3" ) return LOD_3;
  else
  if ( lodStufe == "LOD 4" ) return LOD_4;
  else                       return ALL;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML: Bestmmu, ob ein Geometrie-Atztribut ein LoD-Angabe enthält, //
//  und liefert ggf. diese LoD-Stufe                                         //
///////////////////////////////////////////////////////////////////////////////
bool Features::isLodProperty ( std::string geometryProperty, CITYGML_LOD lodStufe )
{
  switch ( lodStufe )
  {
  case LOD_0:
    if ( geometryProperty.find( "lod0" ) != string::npos ) return true;
    else                                                   return false;

  case LOD_1:
    if ( geometryProperty.find( "lod1" ) != string::npos ) return true;
    else                                                   return false;
  case LOD_2:
    if ( geometryProperty.find( "lod2" ) != string::npos ) return true;
    else                                                   return false;
  case LOD_3:
    if ( geometryProperty.find( "lod3" ) != string::npos ) return true;
    else                                                   return false;
  case LOD_4:
    if ( geometryProperty.find( "lod4" ) != string::npos ) return true;
    else                                                   return false;
  case ALL:
    return true;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Liefert true, wenn es in einem CityGML-Modell Appearances gibt           //
///////////////////////////////////////////////////////////////////////////////
bool Features::hasAppearances ()
{
  if ( mAppearance.size() > 0 )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Generiert für CityGML-Modelle einen neuer RepesenmtationContext          //
///////////////////////////////////////////////////////////////////////////////
Feature * Features::createCityGMLRepresentationContext ( string contextIdentifier, string namespaceKuerzel, string contextName, double precision )
{
  if ( !isCityGML() )
    return NULL;

  string gmlId = Constant::createGmlId();
  Feature * pRepContext = new Feature ( ANY_OBJEKT, gmlId, this, namespaceKuerzel + ":" + "RepresentationContext", "gml:featureMember" );
  addFeature( pRepContext );

  pRepContext->setURLAttribut (  namespaceKuerzel + ":" + "contextIdentifier", contextIdentifier );
  if ( contextName != "" )
    pRepContext->setStringAttribut(  namespaceKuerzel + ":" + "contextName", contextName );

  if ( precision != 0.0 )
    pRepContext->setDoubleAttribut(  namespaceKuerzel + ":" + "precision", precision );

  return pRepContext;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert ein neues Feature mit dem übergebenen Klassen-Namen            //
//  das über die Relation featurePropertyName vom Feature pParentFeature     //
//  referiert wird                                                           //                           
///////////////////////////////////////////////////////////////////////////////
Feature * Features::createFeature ( std::string klassenName, Feature * pParentFeature, std::string featurePropertyName)
{
  string gmlId = Constant::createGmlId();
  return createFeature( klassenName, pParentFeature, featurePropertyName, gmlId );
}

Feature * Features::createFeature ( std::string klassenName, Feature * pParentFeature, 
                                    std::string featurePropertyName, std::string gmlId )
{
  Feature * pNewFeature;

  if ( pParentFeature != NULL )
    pNewFeature = new Feature ( ANY_OBJEKT, gmlId, this, klassenName, "" );
  else
    pNewFeature = new Feature ( ANY_OBJEKT, gmlId, this, klassenName, featurePropertyName );
  addFeature( pNewFeature );

  if ( pParentFeature != NULL )
  {
    pParentFeature->addChild( gmlId );
    pNewFeature->setParentId ( pParentFeature->getGmlId() );

    FeatureRelation * pRel = new FeatureRelation;
    pRel->rollenName = featurePropertyName;
    pRel->featureId  = gmlId;
    pParentFeature->addFeatureRelation( pRel );
  }

  return pNewFeature;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Ordner der Externen Codelisten für ein bestimmtes GML-         //
//  Applikationsschema                                                       //
///////////////////////////////////////////////////////////////////////////////
void Features::setExternalCodeListFolder ( GML_SCHEMA_TYPE gmlTyp, std::string folder )
{
  m_mExternalCodeListFolders[gmlTyp] = folder;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Ordner der Externen Codelisten für ein bestimmtes GML-       //
//  Applikationsschema                                                       //
///////////////////////////////////////////////////////////////////////////////
std::string Features::getExternalCodeListsFolder( GML_SCHEMA_TYPE gmlTyp )
{
  if ( m_mExternalCodeListFolders.find ( gmlTyp ) != m_mExternalCodeListFolders.end() )
    return m_mExternalCodeListFolders[gmlTyp];
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
// Liefert die GML-Applikationsschema Typen (als Texte) aller Einträge der   //
// map m_mExternalCodeListFolders                                            //
///////////////////////////////////////////////////////////////////////////////
size_t Features::getExternalCodeListGmlTypesAsString ( std::vector<std::string> &vStrings )
{
  map<GML_SCHEMA_TYPE,string>::iterator iter;
  GML_SCHEMA_TYPE typ;

  for ( iter = m_mExternalCodeListFolders.begin(); iter != m_mExternalCodeListFolders.end(); iter++ )
  {
    typ = iter->first;
    vStrings.push_back( Constant::getGMLSchemaTypeAsString( typ ) );
  }

  return vStrings.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertiert Shapefile Attributwerte in bool-Werte true/false             //
///////////////////////////////////////////////////////////////////////////////
bool Features::interpretBooleanAttribute ( std::string v )
{
  set<string>::iterator iter;

  String ^ STR = QuConvert::ToString( v );
  STR = STR->ToLower();
  QuConvert::systemStr2stdStr( v, STR );

  for ( iter = shapeFileBooleanValueTrue.begin(); iter != shapeFileBooleanValueTrue.end(); iter++ )
    if ( *iter ==  v )
      return true;

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//  FeatureList  													                                   //
//  Liste von Feature-Objekten eines bestimmten Typs                         //
///////////////////////////////////////////////////////////////////////////////
FeatureList::FeatureList()
{

}

FeatureList::FeatureList( FeatureList * pListOld )
{
  for ( unsigned int i = 0; i < pListOld->vFeatures.size(); i++ )
    vFeatures.push_back( new Feature ( pListOld->vFeatures[i], "") );
}


FeatureList::~FeatureList()
{
  for ( unsigned int i = 0; i < vFeatures.size(); i++ )
    delete vFeatures[i];

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen Listen-Eintrag                                             //
///////////////////////////////////////////////////////////////////////////////
Feature * FeatureList::get ( size_t index )
{
  if ( index < 0 || index >= vFeatures.size() ) return NULL;
  return vFeatures[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Listen-Einträge                                   //
///////////////////////////////////////////////////////////////////////////////
size_t FeatureList::count()
{
  return vFeatures.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Listen-Eintrag hinzu                                          //
///////////////////////////////////////////////////////////////////////////////
size_t FeatureList::add ( Feature * pPlanObjekt )
{
  vFeatures.push_back( pPlanObjekt );
  return vFeatures.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Listen-Eintrag als Kopie des übergebenen Features hinzu       //
///////////////////////////////////////////////////////////////////////////////
size_t FeatureList::addCopy ( Feature * pPlanObjekt )
{
  Feature * pObjNew = new Feature ( pPlanObjekt, pPlanObjekt->getGmlId() );
  vFeatures.push_back( pObjNew );
  return vFeatures.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt einen Listen-Eintrag                                            //
///////////////////////////////////////////////////////////////////////////////
size_t FeatureList::remove ( size_t index )
{
  if ( index >= 0 && index < vFeatures.size() )
  {
    delete vFeatures[index];
    for ( size_t i = index+1; i < vFeatures.size(); i++ )
      vFeatures[i-1] = vFeatures[i];
    vFeatures.pop_back();
  }
  return vFeatures.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Listen-Einträge                                            //
///////////////////////////////////////////////////////////////////////////////
void FeatureList::removeAll()
{
  for ( unsigned int i = 0; i < vFeatures.size(); i++ )
    delete vFeatures[i];

  vFeatures.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Feature													                              			     //
//  Raumbezogenes, semantisches GML-Objekt (Feature)                         //
///////////////////////////////////////////////////////////////////////////////
Feature::Feature ( OBJEKT_TYP objektTypP, string gmlIdP, Features * pPlanObjekteP,  string klassenNameP, string featureMemberNameP )
{
	pFeatures = pPlanObjekteP;
	gmlId		          = gmlIdP;
	klassenName       = klassenNameP;
  featureMemberName = featureMemberNameP;
	objektTyp         = objektTypP;
  srsName           = "";
  srsDimension      = -1;

  isSimple      = false;
  pSimpleContent = NULL;
}

Feature::Feature ( Feature * pObjOld, std::string gmlIdP )
{
	multimap<string,StringAttr*>::iterator              iterString;
	multimap<string,IntAttr*>::iterator                 iterInt;
	multimap<string,DoubleAttr*>::iterator              iterDouble;
	map<string,BoolAttr*>::iterator                     iterBool;
	multimap<string,FeatureRelation*>::iterator         iterRelationen;
  multimap<string,_Geometrie*>::iterator              iterGeometrie;
  multimap<std::string,DateAttr*>::iterator           iterDate;
  multimap<std::string,AbstractTimeSeries*>::iterator iterTimeSeries;
  map<std::string,std::string>::iterator              iterClassifier;

	string         attributName;
	StringAttr *   stringAttributWert;
	IntAttr      * integerAttributWert;
	DoubleAttr   * doubleAttributWert;
	StringAttr   * stringAttributWertCopy;
	IntAttr      * integerAttributWertCopy;
	DoubleAttr   * doubleAttributWertCopy;
  BoolAttr     * boolAttributWert;
  BoolAttr     * boolAttributWertCopy;
  DateAttr     * dateAttributWert;
  DateAttr     * dateAttributWertCopy;
	string         text;
	unsigned int   i;
	FeatureRelation  * pRel;

	gmlId			 = gmlIdP;

	pFeatures         = pObjOld->pFeatures;
	klassenName       = pObjOld->klassenName;
  featureMemberName = pObjOld->featureMemberName;
	objektTyp         = pObjOld->objektTyp;
	bereichId	        = pObjOld->bereichId;
  parentId          = pObjOld->parentId;
  srsName           = pObjOld->srsName;
  srsDimension      = pObjOld->srsDimension;
  isSimple          = pObjOld->isSimple;
  
  if ( pObjOld->pSimpleContent != NULL )
  {
    pSimpleContent  = new StringAttr ( pObjOld->pSimpleContent );
  }
  else
    pSimpleContent = NULL;

  for ( i = 0; i < pObjOld->vChildIds.size(); i++ )
  {
    vChildIds.push_back( pObjOld->vChildIds[i] );
  }

	for ( iterString = pObjOld->stringAttribute.begin(); 
		    iterString != pObjOld->stringAttribute.end(); iterString++ )
	{
		attributName	     = iterString->first;
		stringAttributWert = iterString->second;

		stringAttributWertCopy = new StringAttr ( stringAttributWert );
		stringAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributName, stringAttributWertCopy ) );
	}

	for ( iterInt = pObjOld->integerAttribute.begin(); 
		    iterInt != pObjOld->integerAttribute.end(); iterInt++ )
	{
		attributName	      = iterInt->first;
		integerAttributWert = iterInt->second;

		integerAttributWertCopy = new IntAttr ( integerAttributWert );
		integerAttribute.insert ( multimap<string,IntAttr*>::value_type ( attributName, integerAttributWertCopy ) );
	}

	for ( iterDouble = pObjOld->doubleAttribute.begin(); 
		    iterDouble != pObjOld->doubleAttribute.end(); iterDouble++ )
	{
		attributName	     = iterDouble->first;
		doubleAttributWert = iterDouble->second;

		doubleAttributWertCopy = new DoubleAttr ( doubleAttributWert );
		doubleAttribute.insert ( multimap<string,DoubleAttr*>::value_type ( attributName, doubleAttributWertCopy ) );
	}

	for ( iterDate = pObjOld->dateAttribute.begin(); iterDate != pObjOld->dateAttribute.end(); iterDate++ )
	{
		attributName	     = iterDate->first;
		dateAttributWert   = iterDate->second;

    dateAttributWertCopy = new DateAttr ( dateAttributWert );
		dateAttribute.insert ( multimap<string,DateAttr*>::value_type ( attributName, dateAttributWertCopy ) );
	}

 	for ( iterString = pObjOld->urlAttribute.begin(); 
		    iterString != pObjOld->urlAttribute.end(); iterString++ )
	{
		attributName	     = iterString->first;
		stringAttributWert = iterString->second;

		stringAttributWertCopy = new StringAttr ( stringAttributWert );
		urlAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributName, stringAttributWertCopy ) );
	}

	for ( iterBool = pObjOld->boolAttribute.begin(); 
		    iterBool != pObjOld->boolAttribute.end(); iterBool++ )
	{
		attributName	   = iterBool->first;
    boolAttributWert = iterBool->second;

		boolAttributWertCopy = new BoolAttr ( boolAttributWert );
		boolAttribute[attributName] = boolAttributWertCopy;
	}

	for ( iterRelationen = pObjOld->objRelationen.begin(); 
		  iterRelationen != pObjOld->objRelationen.end(); iterRelationen++ )
	{
		attributName = iterRelationen->first;
    pRel         = iterRelationen->second;

    FeatureRelation * pRelNew = new FeatureRelation ( pRel );
    
    objRelationen.insert ( multimap<string,FeatureRelation*>::value_type ( attributName, pRelNew ) );
	}

  for ( iterGeometrie = pObjOld->mGeometrie.begin(); iterGeometrie != pObjOld->mGeometrie.end(); iterGeometrie++ )
  {
    string geometryRelation = iterGeometrie->first;
    _Geometrie * pGeoCopy = _Geometrie::copy ( iterGeometrie->second );
    
    mGeometrie.insert ( multimap<string,_Geometrie*>::value_type ( geometryRelation, pGeoCopy ) );
  }

	for ( i = 0; i < pObjOld->symbolPositionen.size(); i++ )
	{
		GeoPoint * pPoint = pObjOld->symbolPositionen[i];
		GeoPoint * pPointNew = new GeoPoint ( pPoint );
		symbolPositionen.push_back ( pPointNew );
	}

  for ( iterTimeSeries = pObjOld->mTimeSeries.begin(); iterTimeSeries != pObjOld->mTimeSeries.end(); iterTimeSeries++ )
  {
    string property                  = iterTimeSeries->first;
    AbstractTimeSeries * pTimeSeries = iterTimeSeries->second;
    string gmlIdNew                  = Constant::createGmlId();

    switch ( pTimeSeries->getTimeSeriesType() )
    {
    case AbstractTimeSeries::Regular:
      {
        RegularTimeSeries * pTimeSeriesNew = new RegularTimeSeries ( (RegularTimeSeries*)pTimeSeries, gmlIdNew );
        mTimeSeries.insert ( multimap<string,AbstractTimeSeries*>::value_type ( property, pTimeSeriesNew ) );
        break;
      }

    case AbstractTimeSeries::Irregular:
      {
        IrregularTimeSeries * pTimeSeriesNew = new IrregularTimeSeries ( (IrregularTimeSeries*)pTimeSeries, gmlIdNew );
        mTimeSeries.insert ( multimap<string,AbstractTimeSeries*>::value_type ( property, pTimeSeriesNew ) );
        break;
      }

    case AbstractTimeSeries::RegularFile:
      {
        RegularTimeSeriesFile * pTimeSeriesNew = new RegularTimeSeriesFile ( (RegularTimeSeriesFile*)pTimeSeries, gmlIdNew );
        mTimeSeries.insert ( multimap<string,AbstractTimeSeries*>::value_type ( property, pTimeSeriesNew ) );
        break;
      }

    case AbstractTimeSeries::IrregularFile:
      {
        IrregularTimeSeriesFile * pTimeSeriesNew = new IrregularTimeSeriesFile ( (IrregularTimeSeriesFile*)pTimeSeries, gmlIdNew );
        mTimeSeries.insert ( multimap<string,AbstractTimeSeries*>::value_type ( property, pTimeSeriesNew ) );
        break;
      }
    }

  }
}

Feature::~Feature()
{
	unsigned int                                        i;
	multimap<string,StringAttr*>::iterator              iterString;
  multimap<string,DateAttr*>::iterator                iterDate;
	multimap<string,IntAttr*>::iterator                 iterInt;
	multimap<string,DoubleAttr*>::iterator              iterDouble;
  map<string,BoolAttr*>::iterator                     iterBool;
  multimap<string,_Geometrie*>::iterator              iterGeometrie;
	multimap<string,FeatureRelation*>::iterator         iter;
  multimap<std::string,AbstractTimeSeries*>::iterator iterTimeSeries;

  vector<FeatureRelation*> vRel;

  for ( i = 0; i < vChildIds.size(); i++ )
  {
    string childId = vChildIds[i];
    Feature * pChild = pFeatures->getFeature ( childId );
    if ( pChild != NULL )
        pChild->resetParent();
  }

  if ( pSimpleContent != NULL )
    delete pSimpleContent;

  if ( parentId != "" )
  {
    Feature * pParent = getParent();
    if ( pParent != NULL )
      pParent->removeChild( gmlId );
  }

	for ( iterString = stringAttribute.begin(); iterString != stringAttribute.end(); iterString++ )
		delete iterString->second;

	for ( iterDate = dateAttribute.begin(); iterDate != dateAttribute.end(); iterDate++ )
		delete iterDate->second;

	for ( iterString = urlAttribute.begin(); iterString != urlAttribute.end(); iterString++ )
		delete iterString->second;

	for ( iterInt = integerAttribute.begin(); iterInt != integerAttribute.end(); iterInt++ )
		delete iterInt->second;

	for ( iterDouble = doubleAttribute.begin(); iterDouble != doubleAttribute.end(); iterDouble++ )
		delete iterDouble->second;

  for ( iterBool = boolAttribute.begin(); iterBool != boolAttribute.end(); iterBool++ )
    delete iterBool->second;

	for ( i = 0; i < symbolPositionen.size(); i++ ) delete symbolPositionen[i];

	for ( iter = objRelationen.begin(); iter != objRelationen.end(); iter++ )
  {
		delete iter->second;
  }

  for ( iterGeometrie = mGeometrie.begin(); iterGeometrie != mGeometrie.end(); iterGeometrie++ )
    delete iterGeometrie->second;

  for ( iterTimeSeries = mTimeSeries.begin(); iterTimeSeries != mTimeSeries.end(); iterTimeSeries++ )
    delete iterTimeSeries->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das Attribut srsName                                               //
///////////////////////////////////////////////////////////////////////////////
void Feature::setSrsName ( string name )
{  
  srsName = name;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Attribute srsName und srsDimension                             //
///////////////////////////////////////////////////////////////////////////////
void Feature::setSrsNameAndDimension ( string name )
{  
  srsName = name;  
  srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den srsNamen                                                     //
///////////////////////////////////////////////////////////////////////////////
std::string Feature::getSrsName ()  
{  
  if ( srsName != "" )
    return srsName;  
  else
    return pFeatures->getSrsName();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die srsDimension                                                 //
///////////////////////////////////////////////////////////////////////////////
int Feature::getSrsDimension ()
{  
  if ( srsDimension > 0 )
    return srsDimension;
  else
    return pFeatures->getSrsDimension();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die parentId eines Features, und trägt das Feature als child des   //
//  Parent-Features ein                                                      //
///////////////////////////////////////////////////////////////////////////////
void Feature::setParentId ( std::string parentIdP )
{
  parentId = parentIdP;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die parentId eines Features zurück                                 //
///////////////////////////////////////////////////////////////////////////////
void Feature::resetParent ()
{
  parentId = "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das direkt übergeordnete Feature im Feature Baum                 //
///////////////////////////////////////////////////////////////////////////////
Feature * Feature::getParent()
{
  if ( parentId == "" ) 
    return NULL;
  else
    return pFeatures->getFeature ( parentId );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das oberste Feature im Feature-Baum                              //
///////////////////////////////////////////////////////////////////////////////
Feature * Feature::getRootFeature()
{
  Feature * pFeature = this;
  Feature * pParent;

  while ( pFeature != NULL )
  {
    pParent = pFeature->getParent();
    if ( pParent != NULL )
      pFeature = pParent;
    else
      break;
  }
  
  return pFeature;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle übergeordneten Features im Feature-Baum                     //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getParents ( std::vector<Feature*> &vParents )
{
  Feature * pFeature = this;
  Feature * pParent;

  while ( pFeature != NULL )
  {
    pParent = pFeature->getParent();
    if ( pParent != NULL )
    {
     vParents.push_back ( pParent );
     pFeature = pParent;
    }
    else
      break;
  }

  return vParents.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt die Id eines direkt untergeordneten Feature hinzu                   //
///////////////////////////////////////////////////////////////////////////////
void Feature::addChild ( std::string childId )
{
  vChildIds.push_back( childId );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die direkt untergeordneten Features des Features                 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getChildren ( multimap<string,Feature*> &mChildren, set<string> &sKlassenNamen )
{
  unsigned int i;
  string       klassenName;

  for ( i = 0; i < vChildIds.size(); i++ )
  {
    Feature * pFeature = pFeatures->getFeature( vChildIds[i] );
    if ( pFeature != NULL )
    {
      klassenName =  pFeature->getKlassenName();
      mChildren.insert ( multimap<string,Feature*>::value_type ( klassenName, pFeature ) );
      sKlassenNamen.insert( klassenName );
    }
  }

  return mChildren.size();
}

size_t Feature::getChildrenIds ( vector<string> &vChildrenIds )
{
  unsigned int i;

  for (i = 0; i < vChildIds.size(); i++ )
  {
    vChildrenIds.push_back( vChildIds[i] );
  }

  return vChildrenIds.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert rekursiv alle untergeordneten Features in Feature-Baum           //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getChildrenIdsRecursive ( std::set<std::string> &sChildrenIds )
{
  unsigned int i;
  for (i = 0; i < vChildIds.size(); i++ )
  {
    string gmlId = vChildIds[i];
    if ( sChildrenIds.count ( gmlId ) == 0 )
    {
      sChildrenIds.insert( gmlId );
      Feature * pFeature = pFeatures->getFeature( gmlId );
      if ( pFeature != NULL )
        pFeature->getChildrenIdsRecursive( sChildrenIds );
    }
  }

  return sChildrenIds.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt ein direkt untergeordnetes Feature                              //
///////////////////////////////////////////////////////////////////////////////
bool Feature::removeChild ( std::string childId )
{
  unsigned int i, index;

  index = -1;
  for ( i = 0; i < vChildIds.size(); i++ )
  {
    if ( vChildIds[i] == childId )
    {
      index = i;
      break;
    }
  }

  if ( index == -1 )
    return false;

  for ( i = index + 1; i < vChildIds.size(); i++ )
    vChildIds[i-1] = vChildIds[i];

  vChildIds.pop_back();
  return true; 
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle direkt untergeordneten Features                            //
///////////////////////////////////////////////////////////////////////////////
void Feature::removeAllChildren()
{
  vChildIds.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert true, wenn das Feature eine eigene Geometrie besitzt             //
///////////////////////////////////////////////////////////////////////////////
bool Feature::hasOwnGeometry()
{
  if ( mGeometrie.size() > 0 )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert true, wenn es in den direkt untergeordneten Features mindestens  //
//  eines mit Geometrie gibt                                                 //
///////////////////////////////////////////////////////////////////////////////
bool Feature::hasRelatedGeometry()
{
  unsigned int i;

  for (i = 0; i < vChildIds.size(); i++ )
  {
    Feature * pFeature = pFeatures->getFeature( vChildIds[i] );
    if ( pFeature != NULL && pFeature->hasOwnGeometry() )
      return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Übernimmt die Feature-Attribute von pFeature                             //
//  Nicht übernommen werden referierte Features, Geometrie-Attribute,        //
//  Symbo0l-Positionen und Zeitreihen                                        //
///////////////////////////////////////////////////////////////////////////////
void Feature::uebernehmeSachdatenAttribute ( Feature * pFeature )
{
  multimap<string,StringAttr*>::iterator   iterString;
  multimap<string,IntAttr*>::iterator      iterInt;
  multimap<string,DoubleAttr*>::iterator   iterDouble;
  multimap<string,DateAttr*>::iterator     iterDate;
  map<string,BoolAttr*>::iterator          iterBool;

  string       attributName;
  StringAttr * stringAttributWert;
  IntAttr    * integerAttributWert;
  DoubleAttr * doubleAttributWert;
  BoolAttr   * boolAttributWert;
  StringAttr * stringAttributWertCopy;
  IntAttr    * integerAttributWertCopy;
  DoubleAttr * doubleAttributWertCopy;
  BoolAttr   * boolAttributWertCopy;
  DateAttr   * pDateAttributWert;
  DateAttr  * pDateAttributWertCopy;
  string       text;

  for ( iterString = pFeature->stringAttribute.begin(); 
    iterString != pFeature->stringAttribute.end(); iterString++ )
  {
    attributName	   = iterString->first;
    stringAttributWert = iterString->second;

    stringAttributWertCopy = new StringAttr ( stringAttributWert );
    stringAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributName, stringAttributWertCopy ) );
  }

  for ( iterInt = pFeature->integerAttribute.begin(); 
    iterInt != pFeature->integerAttribute.end(); iterInt++ )
  {
    attributName	    = iterInt->first;
    integerAttributWert = iterInt->second;

    integerAttributWertCopy = new IntAttr ( integerAttributWert );
    integerAttribute.insert ( multimap<string,IntAttr*>::value_type ( attributName, integerAttributWertCopy ) );
  }

  for ( iterDouble = pFeature->doubleAttribute.begin(); 
    iterDouble != pFeature->doubleAttribute.end(); iterDouble++ )
  {
    attributName	   = iterDouble->first;
    doubleAttributWert = iterDouble->second;

    doubleAttributWertCopy        = new DoubleAttr ( doubleAttributWert );
    doubleAttribute.insert ( multimap<string,DoubleAttr*>::value_type ( attributName, doubleAttributWertCopy ) );
  }

  for ( iterDate = pFeature->dateAttribute.begin();  iterDate != pFeature->dateAttribute.end(); iterDate++ )
  {
    attributName	   = iterDate->first;
    pDateAttributWert = iterDate->second;

    pDateAttributWertCopy = new DateAttr ( pDateAttributWert );
    dateAttribute.insert ( multimap<string,DateAttr*>::value_type ( attributName, pDateAttributWertCopy ) );
  }

  for ( iterString = pFeature->urlAttribute.begin(); 
    iterString != pFeature->urlAttribute.end(); iterString++ )
  {
    attributName	   = iterString->first;
    stringAttributWert = iterString->second;

    stringAttributWertCopy = new StringAttr ( stringAttributWert );
    urlAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributName, stringAttributWertCopy ) );
  }

  for ( iterBool = pFeature->boolAttribute.begin(); 
    iterBool != pFeature->boolAttribute.end(); iterBool++ )
  {
    attributName	 = iterBool->first;
    boolAttributWert = iterBool->second;

    boolAttributWertCopy = new BoolAttr ( boolAttributWert );
    boolAttribute.insert ( map<string,BoolAttr*>::value_type ( attributName, boolAttributWertCopy ) );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Übernimmt bis auf Geometrie-Attribute und Zeitreihen alle Informationen  //
//  von pPlanObjekt                                                          //
///////////////////////////////////////////////////////////////////////////////
void Feature::uebernehmeAttribute ( Feature * pPlanObjekt )
{
  multimap<string,StringAttr*>::iterator   iterString;
  multimap<string,IntAttr*>::iterator      iterInt;
  multimap<string,DoubleAttr*>::iterator   iterDouble;
  map<string,BoolAttr*>::iterator          iterBool;
  multimap<string,DateAttr*>::iterator     iterDate;

  multimap<string,FeatureRelation*>::iterator iterRelationen;

  string       attributName;
  StringAttr * stringAttributWert;
  IntAttr    * integerAttributWert;
  DoubleAttr * doubleAttributWert;
  BoolAttr   * boolAttributWert;
  StringAttr * stringAttributWertCopy;
  IntAttr    * integerAttributWertCopy;
  DoubleAttr * doubleAttributWertCopy;
  BoolAttr   * boolAttributWertCopy;
  DateAttr   * pDateAttributWert;
  DateAttr   * pDateAttributWertCopy;
  string       text;
  unsigned int i;

  FeatureRelation * pRel;
  FeatureRelation * pRelNew;

  bereichId  = pPlanObjekt->bereichId;
  objektTyp  = pPlanObjekt->objektTyp;

  for ( iterString = pPlanObjekt->stringAttribute.begin(); 
        iterString != pPlanObjekt->stringAttribute.end(); iterString++ )
  {
    attributName	   = iterString->first;
    stringAttributWert = iterString->second;

    stringAttributWertCopy = new StringAttr ( stringAttributWert );
    stringAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributName, stringAttributWertCopy ) );
  }

  for ( iterInt = pPlanObjekt->integerAttribute.begin(); 
       iterInt != pPlanObjekt->integerAttribute.end(); iterInt++ )
  {
    attributName	    = iterInt->first;
    integerAttributWert = iterInt->second;

    integerAttributWertCopy = new IntAttr ( integerAttributWert );
    integerAttribute.insert ( multimap<string,IntAttr*>::value_type ( attributName, integerAttributWertCopy ) );
  }

  for ( iterDouble = pPlanObjekt->doubleAttribute.begin(); 
    iterDouble != pPlanObjekt->doubleAttribute.end(); iterDouble++ )
  {
    attributName	   = iterDouble->first;
    doubleAttributWert = iterDouble->second;

    doubleAttributWertCopy        = new DoubleAttr ( doubleAttributWert );
    doubleAttribute.insert ( multimap<string,DoubleAttr*>::value_type ( attributName, doubleAttributWertCopy ) );
  }

   for ( iterDate = pPlanObjekt->dateAttribute.begin();  iterDate != pPlanObjekt->dateAttribute.end(); iterDate++ )
  {
    attributName	   = iterDate->first;
    pDateAttributWert = iterDate->second;

    pDateAttributWertCopy = new DateAttr ( pDateAttributWert );
    dateAttribute.insert ( multimap<string,DateAttr*>::value_type ( attributName, pDateAttributWertCopy ) );
  }

  for ( iterString = pPlanObjekt->urlAttribute.begin(); 
       iterString != pPlanObjekt->urlAttribute.end(); iterString++ )
  {
    attributName	   = iterString->first;
    stringAttributWert = iterString->second;

    stringAttributWertCopy = new StringAttr ( stringAttributWert );
    urlAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributName, stringAttributWertCopy ) );
  }

  for ( iterBool = pPlanObjekt->boolAttribute.begin(); 
    iterBool != pPlanObjekt->boolAttribute.end(); iterBool++ )
  {
    attributName	 = iterBool->first;
    boolAttributWert = iterBool->second;

    boolAttributWertCopy = new BoolAttr ( boolAttributWert );
    boolAttribute.insert ( map<string,BoolAttr*>::value_type ( attributName, boolAttributWertCopy ) );
  }

  for ( iterRelationen = pPlanObjekt->objRelationen.begin(); 
    iterRelationen != pPlanObjekt->objRelationen.end(); iterRelationen++ )
  {
    attributName = iterRelationen->first;
    pRel         = iterRelationen->second;
    pRelNew      = new FeatureRelation ( pRel );

    objRelationen.insert ( multimap<string,FeatureRelation*>::value_type ( attributName, pRelNew ) );
  }

  for ( i = 0; i < pPlanObjekt->symbolPositionen.size(); i++ )
  {
    GeoPoint * pPoint = pPlanObjekt->symbolPositionen[i];
    GeoPoint * pPointNew = new GeoPoint ( pPoint );
    symbolPositionen.push_back ( pPointNew );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Geometrie-Typ des Features                                   //
///////////////////////////////////////////////////////////////////////////////
GEOMETRY_TYPE Feature::getGeometrieTyp( string geometryPropertyName )
{
  multimap<string,_Geometrie*>::iterator iter;
  _Geometrie                           * pGeo;

  if ( geometryPropertyName =="" )
  {
    switch (  mGeometrie.size() )
    {
    case 0:
      return NO_GEOMETRY;

    case 1:
      {
        iter = mGeometrie.begin();
        pGeo = iter->second;
        return pGeo->getGeometryType();
      }
    }
    return OTHER;
  }
  else
  {
     pGeo = getGeometry( geometryPropertyName, 0 );
     if ( pGeo == NULL )
       return NO_GEOMETRY;
     else
       return pGeo->getGeometryType();
  }
 
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den abstrakten (d.h. aggregierten) Geometrie-Typ des Features    //
///////////////////////////////////////////////////////////////////////////////
ABSTRACT_GEOMETRY_TYPE Feature::getAbstractGeometrieTyp( string geometryPropertyName )
{
  multimap<string,_Geometrie*>::iterator iter;
  _Geometrie                           * pGeo;
  GEOMETRY_TYPE                          geoType;

  if ( geometryPropertyName =="" )
  {
    switch (  mGeometrie.size() )
    {
    case 0:
      return ABSTRACT_NO_GEOMETRY;

    case 1:
      iter = mGeometrie.begin();
      pGeo = iter->second;

    default:
      return ABSTRACT_UNKNOWN;
   }
  }
  else
  {
    pGeo = getGeometry( geometryPropertyName, 0 );
    if ( pGeo == NULL )
      return ABSTRACT_NO_GEOMETRY;
  }

  geoType = pGeo->getGeometryType();

  switch ( geoType )
  {
  case GEO_POINT:
  case MULTI_POINT:
  case POINT_REFERENCE:
  case DIRECT_POSITION:
    return ABSTRACT_POINT;

  case SURFACE:
  case POLYGON:
  case MULTI_SURFACE:
  case COMPOSITE_SURFACE:
  case SHELL:
  case ORIENTABLE_SURFACE:
  case SURFACE_REFERENCE:
  case TRIANGULATED_SURFACE:
  case TRIANGLE:
    return ABSTRACT_SURFACE;

  case CURVE:
  case MULTI_CURVE:
  case LINE_STRING:
	case LINE_STRING_SEGMENT:
  case ARC:
  case CIRCLE:
  case CURVE_REFERENCE:
    return ABSTRACT_CURVE;

  case SOLID: 
  case MULTI_SOLID:
  case SOLID_REFERENCE:
  case COMPOSITE_SOLID:
    return ABSTRACT_SOLID;

  case LINEAR_RING:
  case RING:
    return ABSTRACT_RING;

  case IMPLICIT_GEOMETRY:
  case IMPLICIT_GEOMETRY_REFERENCE:
    return ABSTRACT_IMPLICIT_GEOMETRY;

  case MULTI_GEOMETRY:
  case ALLG_GEOMETRIE:
  case GEOMETRIC_COMPLEX:
  case GEOMETRY_REFERENCE:
    return ABSTRACT_MULTI_GEOMETRY;

  case NO_GEOMETRY:
    return ABSTRACT_NO_GEOMETRY;
  }   

  return ABSTRACT_UNKNOWN; 
}

///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML: Liefert die zugehörige BPlanEbene	                 				 //
///////////////////////////////////////////////////////////////////////////////
PlanBereich * Feature::getPlanBereich ()
{
	return pFeatures->getPlanBereich ( bereichId );
}

///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML: Setzt die BereichId in den referierten Datenobjekten       //
//  vom Typ  OBJEKT                                                          //
///////////////////////////////////////////////////////////////////////////////
void  Feature::propagateBereichId()
{
  multimap<string,FeatureRelation*>::iterator iterRelationen;
  FeatureRelation                           * pRel;
  Feature                                * pRefObj;

  for ( iterRelationen = objRelationen.begin(); iterRelationen != objRelationen.end(); iterRelationen++ )
  {
    pRel = iterRelationen->second;
    pRefObj = pFeatures->getFeature( pRel->featureId );
    if ( pRefObj != NULL && pRel->relToFeature == false )
      pRefObj->setBereichId( bereichId );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Statistik der von diesem Feature belegten Properties        //
///////////////////////////////////////////////////////////////////////////////
void Feature::getPropertyStatistics ( std::map<std::string,std::vector<int>> &mPropertyStatistics )
{
  multimap<string,StringAttr*>::iterator      iterStringAttribute;
  multimap<string,IntAttr*>::iterator         iterIntAttribute;
  multimap<string,DoubleAttr*>::iterator      iterDoubleAttribute;
  multimap<string,FeatureRelation*>::iterator iterFeatureRelationen;
  map<string,BoolAttr*>::iterator             iterBoolAttribute;
  multimap<string,DateAttr*>::iterator        iterDateAttribute;
  multimap<string,_Geometrie*>::iterator      iterGeometrie;
  string                                      propertyName;
  vector<int>                                 vAnz;
  set<string>                                 attributNamen;

  vAnz.push_back( 0 );
  vAnz.push_back( 0 );

  for ( iterStringAttribute =  stringAttribute.begin(); iterStringAttribute != stringAttribute.end(); iterStringAttribute++ )
  {
    propertyName = iterStringAttribute->first;
    if ( mPropertyStatistics.find( propertyName ) == mPropertyStatistics.end() )
    {
      vAnz[0] = 1;
      vAnz[1] = 1;
      mPropertyStatistics[propertyName] = vAnz;
      attributNamen.insert( propertyName );
    }
    else
    {
      mPropertyStatistics[propertyName][0] =  mPropertyStatistics[propertyName][0] + 1;
      if ( attributNamen.find( propertyName ) == attributNamen.end() )
      {
        mPropertyStatistics[propertyName][1] =  mPropertyStatistics[propertyName][1] + 1;
        attributNamen.insert ( propertyName );
      }
    }
  }

  attributNamen.clear();
  for ( iterDateAttribute =  dateAttribute.begin(); iterDateAttribute != dateAttribute.end(); iterDateAttribute++ )
  {
    propertyName = iterDateAttribute->first;
    if ( mPropertyStatistics.find( propertyName ) == mPropertyStatistics.end() )
    {
      vAnz[0] = 1;
      vAnz[1] = 1;
      mPropertyStatistics[propertyName] = vAnz;
      attributNamen.insert( propertyName );
    }
    else
    {
      mPropertyStatistics[propertyName][0] =  mPropertyStatistics[propertyName][0] + 1;
      if ( attributNamen.find( propertyName ) == attributNamen.end() )
      {
        mPropertyStatistics[propertyName][1] =  mPropertyStatistics[propertyName][1] + 1;
        attributNamen.insert( propertyName );
      }
    }
  }

  attributNamen.clear();
  for ( iterStringAttribute =  urlAttribute.begin(); iterStringAttribute != urlAttribute.end(); iterStringAttribute++ )
  {
    propertyName = iterStringAttribute->first;
    if ( mPropertyStatistics.find( propertyName ) == mPropertyStatistics.end() )
    {
      vAnz[0] = 1;
      vAnz[1] = 1;
      mPropertyStatistics[propertyName] = vAnz;
      attributNamen.insert( propertyName );
    }
    else
    {
      mPropertyStatistics[propertyName][0] =  mPropertyStatistics[propertyName][0] + 1;
      if ( attributNamen.find( propertyName ) == attributNamen.end() )
      {
        mPropertyStatistics[propertyName][1] =  mPropertyStatistics[propertyName][1] + 1;
        attributNamen.insert( propertyName );
      }
    }
  }

  attributNamen.clear();
  for ( iterIntAttribute =  integerAttribute.begin(); iterIntAttribute != integerAttribute.end(); iterIntAttribute++ )
  {
    propertyName = iterIntAttribute->first;
    if ( mPropertyStatistics.find( propertyName ) == mPropertyStatistics.end() )
    {
      vAnz[0] = 1;
      vAnz[1] = 1;
      mPropertyStatistics[propertyName] = vAnz;
      attributNamen.insert( propertyName );
    }
    else
    {
      mPropertyStatistics[propertyName][0] =  mPropertyStatistics[propertyName][0] + 1;
      if ( attributNamen.find( propertyName ) == attributNamen.end() )
      {
        mPropertyStatistics[propertyName][1] =  mPropertyStatistics[propertyName][1] + 1;
        attributNamen.insert( propertyName );
      }
    }
  }

  attributNamen.clear();
  for ( iterDoubleAttribute =  doubleAttribute.begin(); iterDoubleAttribute != doubleAttribute.end(); iterDoubleAttribute++ )
  {
    propertyName = iterDoubleAttribute->first;
    if ( mPropertyStatistics.find( propertyName ) == mPropertyStatistics.end() )
    {
      vAnz[0] = 1;
      vAnz[1] = 1;
      mPropertyStatistics[propertyName] = vAnz;
      attributNamen.insert( propertyName );
    }
    else
    {
      mPropertyStatistics[propertyName][0] =  mPropertyStatistics[propertyName][0] + 1;
      if ( attributNamen.find( propertyName ) == attributNamen.end() )
      {
        mPropertyStatistics[propertyName][1] =  mPropertyStatistics[propertyName][1] + 1;
        attributNamen.insert( propertyName );
      }
    }
  }

  attributNamen.clear();
  for ( iterBoolAttribute =  boolAttribute.begin(); iterBoolAttribute != boolAttribute.end(); iterBoolAttribute++ )
  {
    propertyName = iterBoolAttribute->first;
    if ( mPropertyStatistics.find( propertyName ) == mPropertyStatistics.end() )
    {
      vAnz[0] = 1;
      vAnz[1] = 1;
      mPropertyStatistics[propertyName] = vAnz;
      attributNamen.insert( propertyName );
    }
    else
    {
      mPropertyStatistics[propertyName][0] =  mPropertyStatistics[propertyName][0] + 1;
      if ( attributNamen.find( propertyName ) == attributNamen.end() )
      {
        mPropertyStatistics[propertyName][1] =  mPropertyStatistics[propertyName][1] + 1;
        attributNamen.insert( propertyName );
      }
    }
  }

  attributNamen.clear();
  for ( iterFeatureRelationen = objRelationen.begin(); iterFeatureRelationen != objRelationen.end(); iterFeatureRelationen++ )
  {
    propertyName = iterFeatureRelationen->first;
    if ( mPropertyStatistics.find( propertyName ) == mPropertyStatistics.end() )
    {
      vAnz[0] = 1;
      vAnz[1] = 1;
      mPropertyStatistics[propertyName] = vAnz;
      attributNamen.insert( propertyName );
    }
    else
    {
      mPropertyStatistics[propertyName][0] =  mPropertyStatistics[propertyName][0] + 1;
      if ( attributNamen.find( propertyName ) == attributNamen.end() )
      {
        mPropertyStatistics[propertyName][1] =  mPropertyStatistics[propertyName][1] + 1;
        attributNamen.insert( propertyName );
      }
    }
  }

  attributNamen.clear();
  for ( iterGeometrie = mGeometrie.begin(); iterGeometrie != mGeometrie.end(); iterGeometrie++ )
  {
    propertyName = iterGeometrie->first;
    if ( mPropertyStatistics.find( propertyName ) == mPropertyStatistics.end() )
    {
      vAnz[0] = 1;
      vAnz[1] = 1;
      mPropertyStatistics[propertyName] = vAnz;
      attributNamen.insert( propertyName );
    }
    else
    {
      mPropertyStatistics[propertyName][0] =  mPropertyStatistics[propertyName][0] + 1;
      if ( attributNamen.find( propertyName ) == attributNamen.end() )
      {
        mPropertyStatistics[propertyName][1] =  mPropertyStatistics[propertyName][1] + 1;
        attributNamen.insert( propertyName );
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein einzelnes Text-Attribut                       		        		 //
///////////////////////////////////////////////////////////////////////////////
void Feature::setStringAttribut ( string attributNameP, string attributWertP )
{
	StringAttr * pAttr = new StringAttr;
	pAttr->value = attributWertP;

	stringAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributNameP, pAttr ) );
}

void Feature::setStringAttribut ( std::string attributNameP, StringAttr * pStringAttr )
{
  stringAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributNameP, pStringAttr ) );
}


void Feature::setStringAttribut ( string attributNameP, string attributWertP, 
									                   string classifierNameP, string classifierValueP )
{
	StringAttr * pAttr = new StringAttr;
	pAttr->value           = attributWertP;
  pAttr->classifier[classifierNameP] = classifierValueP;

	stringAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributNameP, pAttr ) );
}

void Feature::setStringAttribut ( string attributNameP, string attributWertP, 
                                     vector<string> classifierNameP, vector<string> classifierValueP )
{
  unsigned int i;

  StringAttr * pAttr = new StringAttr;
  pAttr->value           = attributWertP;
  for ( i = 0; i < classifierNameP.size(); i++ )
  {
    pAttr->classifier[ classifierNameP[i] ] = classifierValueP[i];
  }

  stringAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributNameP, pAttr ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Tauscht einen bestimmten Attributwert eines Text-Attributs gegen einen	 //
//  neuen Attributwert sowie neue Classifier-Werte aus                       //
///////////////////////////////////////////////////////////////////////////////
void Feature::exchangeStringAttribut ( std::string attributNameP, std::string attributWertAltP, std::string attributWertNeuP,
                                      std::vector<std::string> classifierNameP, std::vector<std::string> classifierValueP )
{
  vector<StringAttr*> vStringAttributWerte;
  StringAttr        * pStringAttr;
	size_t              i, anz;
	size_t              j;
    
  anz = getStringAttributWerte( attributNameP, vStringAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    pStringAttr = vStringAttributWerte[i];
    if ( pStringAttr->value == attributWertAltP )
    {
      pStringAttr->value = attributWertNeuP;
      pStringAttr->classifier.clear();
      for ( j = 0; j < classifierNameP.size(); j++ )
      {
        pStringAttr->classifier[ classifierNameP[j] ] = classifierValueP[j];
      }
      return;
    }
  }

  setStringAttribut( attributNameP, attributWertNeuP, classifierNameP, classifierValueP );
}


///////////////////////////////////////////////////////////////////////////////
//  Tauscht einen bestimmten Attributwert eines Text-Attributs gegen einen	 //
//  neuen Attributwert aus. Evtl. vorhandene Classifier-Werte werden gelöscht//
///////////////////////////////////////////////////////////////////////////////
void Feature::exchangeStringAttribut ( std::string attributNameP, std::string attributWertAltP, std::string attributWertNeuP )
{
  vector<StringAttr*> vStringAttributWerte;
  StringAttr        * pStringAttr;
	size_t              i, anz;

  anz = getStringAttributWerte( attributNameP, vStringAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    pStringAttr = vStringAttributWerte[i];
    if ( pStringAttr->value == attributWertAltP )
    {
      pStringAttr->value = attributWertNeuP;
      pStringAttr->classifier.clear();
      return;
    }
  }

  setStringAttribut( attributNameP, attributWertNeuP );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein einzelnes Datum-Attribut                       		        		 //
///////////////////////////////////////////////////////////////////////////////
void Feature::setDateAttribut ( string attributNameP, DateAttr * pDateTime )
{
	dateAttribute.insert ( multimap<string,DateAttr*>::value_type ( attributNameP, pDateTime ) );
}


///////////////////////////////////////////////////////////////////////////////
//  Tauscht einen bestimmten Attributwert eines Datum-Attributs aus      		 //
///////////////////////////////////////////////////////////////////////////////
void Feature::exchangeDateAttribut ( string attributNameP, DateAttr * pDateTimeOld, DateAttr * pDateTimeNew  )
{
  vector<DateAttr*> vDateAttributWerte;
  DateAttr        * pTime;
	size_t            i, anz;

  if ( pDateTimeOld == NULL )
  {
    setDateAttribut( attributNameP, pDateTimeNew );
    return;
  }

  anz = getDateAttributWerte( attributNameP, vDateAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    pTime = vDateAttributWerte[i];
    if ( pTime == pDateTimeOld )
    {
      *pTime = *pDateTimeNew;   
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein Attribut vom Typ URL                                           //
///////////////////////////////////////////////////////////////////////////////
void Feature::setURLAttribut ( string attributNameP, string attributWertP )
{
	StringAttr * pAttr = new StringAttr;
	pAttr->value = attributWertP;

	urlAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributNameP, pAttr ) );
}

void Feature::setURLAttribut ( std::string attributNameP, StringAttr * pURLAttr )
{
  urlAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributNameP, pURLAttr ) );
}

void Feature::setURLAttribut ( string attributNameP, string attributWertP, 
                               string classifierNameP, string classifierValueP )
{
  StringAttr * pAttr = new StringAttr;
  pAttr->value = attributWertP;

  pAttr->classifier[classifierNameP] = classifierValueP;
  urlAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributNameP, pAttr ) );

}

void Feature::setURLAttribut ( string attributNameP, string attributWertP, 
                               vector<string> classifierNameP, vector<string> classifierValueP )
{
  unsigned int i;

  StringAttr * pAttr = new StringAttr;
  pAttr->value = attributWertP;

  for ( i = 0; i < classifierNameP.size(); i++ )
  {
    pAttr->classifier[classifierNameP[i]] = classifierValueP[i];
  }

  urlAttribute.insert ( multimap<string,StringAttr*>::value_type ( attributNameP, pAttr ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Tauscht einen bestimmten Attributwert eines URL-Attributs aus            //
///////////////////////////////////////////////////////////////////////////////
void Feature::exchangeURLAttribut ( string attributNameP, string attributWertAltP, string attributWertNeuP,
                                    vector<string> classifierNameP, vector<string> classifierValueP )
{
  vector<StringAttr*> vStringAttributWerte;
  StringAttr        * pStringAttr;
	size_t              i, anz;
	size_t              j;

  if ( attributWertAltP == "" )
  {
    setURLAttribut( attributNameP, attributWertNeuP, classifierNameP, classifierValueP );
    return;
  }

  anz = getURLAttributWerte( attributNameP, vStringAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    pStringAttr = vStringAttributWerte[i];
    if ( pStringAttr->value == attributWertAltP )
    {
      pStringAttr->value = attributWertNeuP;
      pStringAttr->classifier.clear();
      for ( j = 0; j < classifierNameP.size(); j++ )
      {
        pStringAttr->classifier[classifierNameP[j]] = classifierValueP[j];
      }
      return;
    }
  }

  setURLAttribut( attributNameP, attributWertNeuP, classifierNameP, classifierValueP );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein Attribut vom Typ Integer                                       //
///////////////////////////////////////////////////////////////////////////////
void Feature::setIntegerAttribut ( string attributNameP, int attributWertP )
{
	IntAttr * pAttr = new IntAttr;
	pAttr->value = attributWertP;

	integerAttribute.insert ( multimap<string,IntAttr*>::value_type ( attributNameP, pAttr ) );
}

void Feature::setIntegerAttribut ( std::string attributNameP, IntAttr * pIntAttr )
{
  integerAttribute.insert ( multimap<string,IntAttr*>::value_type ( attributNameP, pIntAttr ) );
}

void Feature::setIntegerAttribut ( string attributNameP, int attributWertP, 
                                      string classifierNameP, string classifierValueP )
{
  IntAttr * pAttr = new IntAttr;
  pAttr->value = attributWertP;
  pAttr->classifier[classifierNameP] = classifierValueP;

  integerAttribute.insert ( multimap<string,IntAttr*>::value_type ( attributNameP, pAttr ) );

}

void Feature::setIntegerAttribut ( string attributNameP, int attributWertP, 
                                      vector<string> classifierNameP, vector<string> classifierValueP )
{
  unsigned int i;

  IntAttr * pAttr = new IntAttr;
  pAttr->value = attributWertP;

  for ( i = 0; i < classifierNameP.size(); i++ )
  {
    pAttr->classifier[classifierNameP[i]] = classifierValueP[i];
  }

  integerAttribute.insert ( multimap<string,IntAttr*>::value_type ( attributNameP, pAttr ) );
}

///////////////////////////////////////////////////////////////////////////////
// Tauscht einen bestimmten Attributwert incl. der Classifier eines          //
// Integer-Attributs aus                                                     //
///////////////////////////////////////////////////////////////////////////////
void Feature::exchangeIntegerAttribut ( string attributNameP, string attributWertAltP, int attributWertNeuP,
                                        vector<string> classifierNameP, vector<string> classifierValueP )
{
  vector<IntAttr*> vIntAttributWerte;
  IntAttr        * pIntAttr;
	size_t           i, anz;
  int              attributWertAlt;
	size_t           j;

  if ( attributWertAltP == "" )
  {
    setIntegerAttribut( attributNameP, attributWertNeuP, classifierNameP, classifierValueP );
    return;
  }

  attributWertAlt = XmlConvert::ToInt32( QuConvert::ToString( attributWertAltP ) );

  anz = getIntegerAttributWerte( attributNameP, vIntAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    pIntAttr = vIntAttributWerte[i];
    if ( pIntAttr->value == attributWertAlt )
    {
      pIntAttr->value = attributWertNeuP;
      pIntAttr->classifier.clear();
      for ( j = 0; j < classifierNameP.size(); j++ )
      {
        pIntAttr->classifier[classifierNameP[j]] = classifierValueP[j];
      }
      return;
    }
  }

  setIntegerAttribut( attributNameP, attributWertNeuP, classifierNameP, classifierValueP );
}

///////////////////////////////////////////////////////////////////////////////
// Tauscht einen bestimmten Attributwert eines Integer-Attributs aus         //
// Die Classifier werden gelöscht                                            //
///////////////////////////////////////////////////////////////////////////////
void Feature::exchangeIntegerAttribut ( string attributNameP, int attributWertAltP, int attributWertNeuP )
{
  vector<IntAttr*> vIntAttributWerte;
  IntAttr        * pIntAttr;
	size_t           i, anz;

  anz = getIntegerAttributWerte( attributNameP, vIntAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    pIntAttr = vIntAttributWerte[i];
    if ( pIntAttr->value == attributWertAltP )
    {
      pIntAttr->value = attributWertNeuP;
      pIntAttr->classifier.clear();
      return;
    }
  }

  setIntegerAttribut( attributNameP, attributWertNeuP );
}

///////////////////////////////////////////////////////////////////////////////
// Setzt ein Attribut vom Typ Double                                         //
///////////////////////////////////////////////////////////////////////////////
void Feature::setDoubleAttribut ( string attributNameP, double attributWertP )
{
	DoubleAttr * pAttr = new DoubleAttr;
	pAttr->value = attributWertP;
  pAttr->uom = NULL;

	doubleAttribute.insert ( multimap<string,DoubleAttr*>::value_type ( attributNameP, pAttr ) );
}

void Feature::setDoubleAttribut ( string attributNameP, double attributWertP, string classifierNameP, string classifierValueP, UOM * pUOM )
{
	DoubleAttr * pAttr = new DoubleAttr;
	pAttr->value		   = attributWertP;
  if ( pUOM != NULL )
    pAttr->uom = pUOM;
  else
    pAttr->uom = NULL;

  if ( classifierNameP == "uom" && pUOM == NULL )
  {
    pUOM = pFeatures->getUOMList()->getUOMFromIdentifier( classifierValueP );
    pAttr->uom = pUOM;
  }
  else
  {
    pAttr->classifier[classifierNameP] = classifierValueP;
  }

	doubleAttribute.insert ( multimap<string,DoubleAttr*>::value_type ( attributNameP, pAttr ) );
}

void Feature::setDoubleAttribut ( string attributNameP, double attributWertP, UOM * pUOM )
{
  DoubleAttr * pAttr = new DoubleAttr;
  pAttr->value		   = attributWertP;
  pAttr->uom         = pUOM;

  doubleAttribute.insert ( multimap<string,DoubleAttr*>::value_type ( attributNameP, pAttr ) );
}

void Feature::setDoubleAttribut ( std::string attributNameP, DoubleAttr * pDoubleAttr )
{
  doubleAttribute.insert ( multimap<string,DoubleAttr*>::value_type ( attributNameP, pDoubleAttr ) );
}


void Feature::setDoubleAttribut ( string attributNameP, double attributWertP, 
                                     vector<string> classifierNameP, vector<string> classifierValueP, UOM * pUOM )
{
  unsigned int i;

  DoubleAttr * pAttr = new DoubleAttr;
  pAttr->value = attributWertP;
  if ( pUOM != NULL )
    pAttr->uom = pUOM;
  else
    pAttr->uom = NULL;


  for ( i = 0; i < classifierNameP.size(); i++ )
  {
    if ( classifierNameP[i] == "uom" && pUOM == NULL )
    {
      UOM * pUOM = pFeatures->getUOMList()->getUOMFromIdentifier( classifierValueP[i] );
      if ( pUOM != NULL )
        pAttr->uom = pUOM;
      else
      {
        pAttr->classifier[ classifierNameP[i]] = classifierValueP[i];
      }
    }
    else
    {
      pAttr->classifier[ classifierNameP[i]] = classifierValueP[i];
    }
  }

  doubleAttribute.insert ( multimap<string,DoubleAttr*>::value_type ( attributNameP, pAttr ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Tauscht einen bestimmten Attributwert eines Double-Attributs aus, wobei  //
//  Classifier und uom gesetzt werden                                        //
///////////////////////////////////////////////////////////////////////////////
void Feature::exchangeDoubleAttribut ( string attributNameP, string attributWertAltP, double attributWertNeuP,
                                       vector<string> classifierNameP, vector<string> classifierValueP )
{
  vector<DoubleAttr*> vDoubleAttributWerte;
  DoubleAttr        * pDoubleAttr;
	size_t              i, anz;
  double              attributWertAlt;
	size_t              j;

  if ( attributWertAltP == "" )
  {
    setDoubleAttribut( attributNameP, attributWertNeuP, classifierNameP, classifierValueP, NULL );
    return;
  }

  attributWertAlt = XmlConvert::ToDouble( QuConvert::ToString( attributWertAltP ) );

  anz = getDoubleAttributWerte( attributNameP, vDoubleAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    pDoubleAttr = vDoubleAttributWerte[i];
    if ( pDoubleAttr->value == attributWertAlt )
    {
      pDoubleAttr->value = attributWertNeuP;
      pDoubleAttr->classifier.clear();
      for ( j = 0; j < classifierNameP.size(); j++ )
      {
        if ( classifierNameP[j] == "uom" )
        {
          UOM * pUOM = pFeatures->getUOMList()->getUOMFromIdentifier( classifierValueP[j] );
          pDoubleAttr->uom = pUOM;
        }
        else
        {
          pDoubleAttr->classifier[classifierNameP[j]] = classifierValueP[j];
        }
      }
      return;
    }
  }

  setDoubleAttribut( attributNameP, attributWertNeuP, classifierNameP, classifierValueP, NULL );
}

///////////////////////////////////////////////////////////////////////////////
//  Tauscht einen bestimmten Attributwert eines Double-Attributs aus, wobei  //
//  Classifier gelöscht und eine uom gesetzt werden                          //
///////////////////////////////////////////////////////////////////////////////
void Feature::exchangeDoubleAttribut ( string attributNameP, double attributWertAltP, double attributWertNeuP, UOM * pUOM )
{
  vector<DoubleAttr*> vDoubleAttributWerte;
  DoubleAttr        * pDoubleAttr;
	size_t              i, anz;

  anz = getDoubleAttributWerte( attributNameP, vDoubleAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    pDoubleAttr = vDoubleAttributWerte[i];
    if ( pDoubleAttr->value == attributWertAltP )
    {
      pDoubleAttr->value = attributWertNeuP;
      pDoubleAttr->classifier.clear();
      pDoubleAttr->uom = pUOM;
     
      return;
    }
  }

  setDoubleAttribut( attributNameP, attributWertNeuP, pUOM );
}

///////////////////////////////////////////////////////////////////////////////
// Setzt ein Attribut vom Typ Boolean                                        //
///////////////////////////////////////////////////////////////////////////////
void Feature::setBoolAttribut ( std::string attributNameP, bool attributWertP )
{
  BoolAttr * pAttr = new BoolAttr;
  pAttr->value = attributWertP;

  boolAttribute.insert ( map<string,BoolAttr*>::value_type ( attributNameP, pAttr ) );
}

void Feature::setBoolAttribut ( std::string attributNameP, BoolAttr * pBoolAttr )
{
  boolAttribute.insert ( map<string,BoolAttr*>::value_type ( attributNameP, pBoolAttr ) );
}


void Feature::setBoolAttribut ( string attributNameP, bool attributWertP, 
                                   string classifierNameP, string classifierValueP )
{
  BoolAttr * pAttr = new BoolAttr;
  pAttr->value		   = attributWertP;

  pAttr->classifier[classifierNameP] = classifierValueP;

  boolAttribute.insert ( map<string,BoolAttr*>::value_type ( attributNameP, pAttr ) );
}

void Feature::setBoolAttribut ( string attributNameP, bool attributWertP, 
                                   vector<string> classifierNameP, vector<string> classifierValueP )
{
  unsigned int i;

  BoolAttr * pAttr = new BoolAttr;
  pAttr->value = attributWertP;

  for ( i = 0; i < classifierNameP.size(); i++ )
  {
    pAttr->classifier[classifierNameP[i]] = classifierValueP[i];
  }

  boolAttribute.insert ( map<string,BoolAttr*>::value_type ( attributNameP, pAttr ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Tauscht einen bestimmten Attributwert aus, wobei Name und Typ des        //
//  Attributs übergeben werden                                               //
//  Datum-Attribute werden nicht verarbeitet                                 //
///////////////////////////////////////////////////////////////////////////////
void Feature::exchangeAttributWert ( ATTRIBUT_TYP attributTyp, string attributName, string attributWertAlt,string attributWertNeu,
                               vector<string> classifierNameP, vector<string> classifierValueP )
{
  double                   h;
	size_t                   i, anz;
	int											 ih;
  bool                     b;
  vector<FeatureRelation*> vRelationen;
  Feature                * pGenericAttribute;
  string                   genericAttributeName;
  string                   genericAttributeValue;

  SchemaTypeNames * pSchemaTypeNames = pFeatures->getGmlSchema()->getSchemaTypeNames();

  switch ( attributTyp )
  {
  case _ANY_URI:
    exchangeURLAttribut ( attributName, attributWertAlt, attributWertNeu, classifierNameP, classifierValueP );
    break;

  case _ENUMERATION:
  case _EXTERNAL_CODE_LIST:
  case _EXTERNAL_CODE_LIST_GML3_3:
  case _STRING:
    exchangeStringAttribut ( attributName, attributWertAlt, attributWertNeu, classifierNameP, classifierValueP );
    break;

  case _DOUBLE:
  case _ANGLE:
  case _LENGTH:
  case _AREA:
  case _VOLUME:
    h = XmlConvert::ToDouble( QuConvert::ToString( attributWertNeu ) );
    exchangeDoubleAttribut ( attributName, attributWertAlt, h, classifierNameP, classifierValueP );
    break;

  case _INTEGER:
    ih = XmlConvert::ToInt32( QuConvert::ToString( attributWertNeu ) );
    exchangeIntegerAttribut ( attributName, attributWertAlt, ih, classifierNameP, classifierValueP );
    break;

  case _BOOLEAN:
    b = XmlConvert::ToBoolean( QuConvert::ToString( attributWertNeu ) );
    setBoolAttribut ( attributName, b, classifierNameP, classifierValueP );
    break;

  case _GENER_STRING:
    anz = getFeatureRelationen ( "_GenericStringAttribute", vRelationen );
    for ( i = 0; i < anz; i++ )
    {
      pGenericAttribute = pFeatures->getFeature ( vRelationen[i]->featureId );
      pGenericAttribute->getStringAttributWert ( pSchemaTypeNames->genericNameAttribute, genericAttributeName );
      if ( genericAttributeName == attributName )
      {
        pGenericAttribute->exchangeStringAttribut( pSchemaTypeNames->genericValueAttribute, attributWertAlt, attributWertNeu, classifierNameP, classifierValueP );
        break;
      }
    }
    break;  

  case _GENER_URL:
    anz = getFeatureRelationen ( "_GenericURLAttribute", vRelationen );
    for ( i = 0; i < anz; i++ )
    {
      pGenericAttribute = pFeatures->getFeature ( vRelationen[i]->featureId );
      pGenericAttribute->getStringAttributWert ( pSchemaTypeNames->genericNameAttribute, genericAttributeName );
      if ( genericAttributeName == attributName )
      {
        pGenericAttribute->exchangeURLAttribut( pSchemaTypeNames->genericValueAttribute, attributWertAlt, attributWertNeu, classifierNameP, classifierValueP );
        break;
      }
    }
    break;

  case _GENER_INTEGER:
    anz = getFeatureRelationen ( "_GenericIntegerAttribute", vRelationen );
    for ( i = 0; i < anz; i++ )
    {
      pGenericAttribute = pFeatures->getFeature ( vRelationen[i]->featureId );
      pGenericAttribute->getStringAttributWert ( pSchemaTypeNames->genericNameAttribute, genericAttributeName );
      if ( genericAttributeName == attributName )
      {
        ih = XmlConvert::ToInt32( QuConvert::ToString( attributWertNeu ) );
        pGenericAttribute->exchangeIntegerAttribut( pSchemaTypeNames->genericValueAttribute, attributWertAlt, ih, classifierNameP, classifierValueP );
        break;
      }
    }
    break;

  case _GENER_DOUBLE:
    anz = getFeatureRelationen ( "_GenericDoubleAttribute", vRelationen );
    for ( i = 0; i < anz; i++ )
    {
      pGenericAttribute = pFeatures->getFeature ( vRelationen[i]->featureId );
      pGenericAttribute->getStringAttributWert ( pSchemaTypeNames->genericNameAttribute, genericAttributeName );
      if ( genericAttributeName == attributName )
      {
        h = XmlConvert::ToDouble( QuConvert::ToString( attributWertNeu ) );
        pGenericAttribute->exchangeDoubleAttribut( pSchemaTypeNames->genericValueAttribute, attributWertAlt, h, classifierNameP, classifierValueP );
        break;
      }
    }
    break;

  case _GENER_MEASURE:
    anz = getFeatureRelationen ( "_GenericMeasureAttribute", vRelationen );
    for ( i = 0; i < anz; i++ )
    {
      pGenericAttribute = pFeatures->getFeature ( vRelationen[i]->featureId );
      pGenericAttribute->getStringAttributWert ( pSchemaTypeNames->genericNameAttribute, genericAttributeName );
      if ( genericAttributeName == attributName )
      {
        h = XmlConvert::ToDouble( QuConvert::ToString( attributWertNeu ) );
        pGenericAttribute->exchangeDoubleAttribut( pSchemaTypeNames->genericValueAttribute, attributWertAlt, h, classifierNameP, classifierValueP );
        break;
      }
    }
    break;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht ein Text-Attribut mit bestimmtem Namen und Wert      	      		 //
//  Wenn attributWert == "" ist, werden alle Text-Attribute mit Namen        //
//  attributName gelöscht                                                    //
///////////////////////////////////////////////////////////////////////////////
void Feature::deleteStringAttribut ( string attributName, string attributWert )
{
  multimap<string,StringAttr*>::iterator iter;
  StringAttr                           * pStringAttr;

  if ( attributWert == "" )
  {
    for ( iter = stringAttribute.lower_bound ( attributName ); iter != stringAttribute.upper_bound( attributName ); iter++ )
      delete iter->second;

	  stringAttribute.erase ( attributName );
  }
  else
  {
    for ( iter = stringAttribute.lower_bound ( attributName ); iter != stringAttribute.upper_bound( attributName ); iter++ )
    {
      pStringAttr = iter->second;
      if ( pStringAttr->value == attributWert )
      {
        stringAttribute.erase( iter );
        delete pStringAttr;
        return;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht ein Integer-Attribut mit bestimmtem Namen und Wert    	      		 //
//  Wenn attributWert == "" ist, werden alle Integer-Attribute mit Namen     //
//  attributName gelöscht                                                    //
///////////////////////////////////////////////////////////////////////////////
void Feature::deleteIntegerAttribut ( std::string attributName, string attributWertP )
{
  multimap<string,IntAttr*>::iterator iter;
  IntAttr                           * pIntAttr;
  int                                 attributWert;

  if ( attributWertP == "" )
  {
    for ( iter = integerAttribute.lower_bound ( attributName ); iter != integerAttribute.upper_bound( attributName ); iter++ )
      delete iter->second;

    integerAttribute.erase ( attributName );
  }
  else
  {
    attributWert = XmlConvert::ToInt32( QuConvert::ToString ( attributWertP ) );
    for ( iter = integerAttribute.lower_bound ( attributName ); iter != integerAttribute.upper_bound( attributName ); iter++ )
    {
      pIntAttr = iter->second;
      if ( pIntAttr->value == attributWert )
      {
        integerAttribute.erase( iter );
        delete pIntAttr;
        return;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht ein Boolean-Attribut mit bestimmtem Namen            	      		 //
///////////////////////////////////////////////////////////////////////////////
void Feature::deleteBoolAttribut ( std::string attributName )
{
  if ( boolAttribute.find( attributName ) != boolAttribute.end() )
  {
    delete boolAttribute[attributName];
	  boolAttribute.erase ( attributName );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht ein Double-Attribut mit bestimmtem Namen und Wert     	      		 //
//  Wenn attributWert == "" ist, werden alle Double-Attribute mit Namen      //
//  attributName gelöscht                                                    //
///////////////////////////////////////////////////////////////////////////////
void Feature::deleteDoubleAttribut ( std::string attributName, string attributWertP )
{
  multimap<string,DoubleAttr*>::iterator iter;
  DoubleAttr                           * pDoubleAttr;
  double                                 attributWert;

  if ( attributWertP == "" )
  {
    for ( iter = doubleAttribute.lower_bound ( attributName ); iter != doubleAttribute.upper_bound( attributName ); iter++ )
      delete iter->second;

    doubleAttribute.erase ( attributName );
  }
  else
  {
    attributWert = XmlConvert::ToDouble( QuConvert::ToString ( attributWertP ) );
    for ( iter = doubleAttribute.lower_bound ( attributName ); iter != doubleAttribute.upper_bound( attributName ); iter++ )
    {
      pDoubleAttr = iter->second;
      if ( pDoubleAttr->value == attributWert )
      {
        doubleAttribute.erase( iter );
        delete pDoubleAttr;
        return;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht ein Datum-Attribut mit bestimmtem Namen              	      		 //
///////////////////////////////////////////////////////////////////////////////
void Feature::deleteDateAttribut ( std::string attributName )
{  
  multimap<string,DateAttr*>::iterator iter;
  for ( iter = dateAttribute.lower_bound( attributName ); iter != dateAttribute.upper_bound( attributName ); iter++ )
  {
    DateAttr * pTime = iter->second;
    delete pTime;
  }
  dateAttribute.erase ( attributName ); 
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht ein URL-Attribut mit bestimmtem Namen und Wert      	      		  //
//  Wenn attributWert == "" ist, werden alle URL-Attribute mit Namen         //
//  attributName gelöscht                                                    //
///////////////////////////////////////////////////////////////////////////////
void Feature::deleteURLAttribut ( std::string attributName, string attributWert )
{
  multimap<string,StringAttr*>::iterator iter;
  StringAttr                           * pStringAttr;

  if ( attributWert == "" )
  {
    for ( iter = urlAttribute.lower_bound ( attributName ); iter != urlAttribute.upper_bound( attributName ); iter++ )
      delete iter->second;

    urlAttribute.erase ( attributName );
  }
  else
  {
    for ( iter = urlAttribute.lower_bound ( attributName ); iter != urlAttribute.upper_bound( attributName ); iter++ )
    {
      pStringAttr = iter->second;
      if ( pStringAttr->value == attributWert )
      {
        urlAttribute.erase( iter );
        delete pStringAttr;
        return;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht ein beliebiges Attribut mit bestimmtem Namen und Wert 	      		 //
//  Wenn attributWert == "" ist, werden alle Attribute des übergebenen Typs  //
//  mit Namen attributName gelöscht                                          //
///////////////////////////////////////////////////////////////////////////////
void Feature::deleteAttributWert ( ATTRIBUT_TYP attributTyp, string attributName, string attributWert )
{
  switch ( attributTyp )
  {
  case _ANY_URI:
    deleteURLAttribut ( attributName, attributWert );
    break;

  case _ENUMERATION:
  case _EXTERNAL_CODE_LIST:
  case _EXTERNAL_CODE_LIST_GML3_3:
  case _STRING:
    deleteStringAttribut ( attributName, attributWert );
    break;

  case _DATE:
    deleteDateAttribut ( attributName );
    break;

  case _DOUBLE:
  case _ANGLE:
  case _LENGTH:
  case _AREA:
  case _VOLUME:
    deleteDoubleAttribut ( attributName, attributWert );
    break;

  case _INTEGER:
    deleteIntegerAttribut ( attributName, attributWert );
    break;

  case _BOOLEAN:
    deleteBoolAttribut ( attributName );
    break;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten unter attributName referierten Text-Attribut Wert 		 //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getStringAttributWert ( string attributName, string & attributWert )
{
	StringAttr * pAttr;
	if ( stringAttribute.lower_bound ( attributName ) != stringAttribute.upper_bound ( attributName ) )
	{
		pAttr = stringAttribute.lower_bound ( attributName )->second;
	  attributWert = pAttr->value;
		return true;
	}
	else return false;
}

StringAttr * Feature::getStringAttributWert ( string attributName )
{
  StringAttr * pAttr;

  if ( stringAttribute.lower_bound ( attributName ) != stringAttribute.upper_bound ( attributName ) )
  {
    pAttr = stringAttribute.lower_bound ( attributName )->second;
    return pAttr;
  }
  else 
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle unter attributName referierte Text-Attribut Werte       		 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getStringAttributWerte ( string attributName, vector<StringAttr*> & vAttributWerteP )
{
	multimap<string,StringAttr*>::iterator iter;
	StringAttr						* pAttr;

	for ( iter = stringAttribute.lower_bound ( attributName );
		  iter !=  stringAttribute.upper_bound ( attributName );
		  iter ++ )
	{
		pAttr = iter->second;
    vAttributWerteP.push_back( pAttr );
	}

	return vAttributWerteP.size();
}

size_t Feature::getStringAttributWerte ( std::string attributName, std::vector<std::string> & vStringP )
{
	multimap<string,StringAttr*>::iterator iter;
	StringAttr						* pAttr;

	for ( iter = stringAttribute.lower_bound ( attributName );
		  iter !=  stringAttribute.upper_bound ( attributName );
		  iter ++ )
	{
		pAttr = iter->second;
		vStringP.push_back ( pAttr->value );
	}

	return vStringP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Text-Attribut Werte                                     		 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getStringAttributWerte ( vector<string> &vAttributNamen, vector<StringAttr*> & vAttributWerte )
{
  multimap<string,StringAttr*>::iterator iter;

  for ( iter = stringAttribute.begin(); iter !=  stringAttribute.end(); iter ++ )
  {
    vAttributNamen.push_back( iter->first );
    vAttributWerte.push_back ( iter->second );
  }

  return vAttributNamen.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Text-Attribut Werte                               //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getStringAttributWertAnz ( std::string attributName )
{
	return stringAttribute.count ( attributName);
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten unter attributName referierten Wert eines generischen //
//  Text-Attributs                                                           //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getGenericStringAttributWert ( std::string attributName, std::string & attributWert )
{
	size_t                   relAnz, i;
	bool			          		 success;
	vector<FeatureRelation*> relationen;
	FeatureRelation        * pRel;
	Feature                * pObj;
	string                   attributNameLoc;

  relAnz = getFeatureRelationen ( "xplan:hatGenerAttribut", relationen );
	for ( i = 0; i < relAnz; i++ )
	{
		pRel = relationen[i];
		pObj = pFeatures->getFeature ( pRel->featureId );
    if ( pObj->getKlassenName() == "xplan:XP_StringAttribut" )
		{
      success = pObj->getStringAttributWert ( "xplan:name", attributNameLoc );
			if ( success && attributName == attributNameLoc) 
        return pObj->getStringAttributWert ( "xplan:wert", attributWert );
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten unter attributName referierten Datum-Attribut Wert    //
///////////////////////////////////////////////////////////////////////////////
DateAttr * Feature::getDateAttributWert ( string attributName )
{
	DateAttr * pAttr;
	if ( dateAttribute.lower_bound ( attributName ) != dateAttribute.upper_bound ( attributName ) )
	{
		pAttr = dateAttribute.lower_bound ( attributName )->second;
		return pAttr;
	}
	else 
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle unter attributName referierte Datum-Attribut Werte      		 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getDateAttributWerte ( std::string attributName, std::vector<DateAttr*> & vDates )
{
	multimap<string,DateAttr*>::iterator iter;
	DateAttr          					  		 * pAttr;

	for ( iter = dateAttribute.lower_bound ( attributName );
		  iter !=  dateAttribute.upper_bound ( attributName );
		  iter ++ )
	{
		pAttr = iter->second;
		vDates.push_back ( pAttr );
	}

	return vDates.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Datum-Attribut Werte                                    		 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getDateAttributWerte ( vector<string> &vAttributNamen, vector<DateAttr*> & vAttributWerte )
{
  multimap<string,DateAttr*>::iterator iter;

  for ( iter = dateAttribute.begin(); iter !=  dateAttribute.end(); iter ++ )
  {
    vAttributNamen.push_back( iter->first );
    vAttributWerte.push_back ( iter->second );
  }

  return vAttributNamen.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Datum-Attribut Werte                          		 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getDateAttributWertAnz ( std::string attributName )
{
	return dateAttribute.count ( attributName);
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten unter attributName referierten Wert eines generischen //
//  Datum-Attributs                                                           //
///////////////////////////////////////////////////////////////////////////////
DateAttr * Feature::getGenericDateAttributWert ( std::string attributName )
{
	size_t                   relAnz, i;
	bool					           success;
	vector<FeatureRelation*> relationen;
	FeatureRelation        * pRel;
	Feature			           * pObj;
	string                   attributNameLoc;

  relAnz = getFeatureRelationen ( "xplan:hatGenerAttribut", relationen );
	for ( i = 0; i < relAnz; i++ )
	{
		pRel = relationen[i];
		pObj = pFeatures->getFeature ( pRel->featureId );
    if ( pObj->getKlassenName() == "xplan:XP_DatumAttribut" )
		{
      success = pObj->getStringAttributWert ( "xplan:name", attributNameLoc );
			if ( success && attributName == attributNameLoc) 
        return pObj->getDateAttributWert ( "xplan:wert" );
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten unter attributName referierten URL-Attribut Wert      //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getURLAttributWert ( string attributName, string & attributWert )
{
	StringAttr * pAttr;
	if ( urlAttribute.lower_bound ( attributName ) != urlAttribute.upper_bound ( attributName ) )
	{
		pAttr = urlAttribute.lower_bound ( attributName )->second;
		attributWert = pAttr->value;
		return true;
	}
	else return false;
}

StringAttr * Feature::getURLAttributWert ( string attributName )
{
  StringAttr * pAttr;

  if ( urlAttribute.lower_bound ( attributName ) != urlAttribute.upper_bound ( attributName ) )
  {
    pAttr = urlAttribute.lower_bound ( attributName )->second;
    return pAttr;
  }
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle unter attributName referierte URL-Attribut Werte            //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getURLAttributWerte ( std::string attributName, std::vector<std::string> & vStringP )
{
	multimap<string,StringAttr*>::iterator iter;
	StringAttr							  * pAttr;

	for ( iter = urlAttribute.lower_bound ( attributName );
		  iter !=  urlAttribute.upper_bound ( attributName );
		  iter ++ )
	{
		pAttr = iter->second;
		vStringP.push_back ( pAttr->value );
	}

	return vStringP.size();
}

size_t Feature::getURLAttributWerte ( string attributName, vector<StringAttr*> & vAttributWerteP )
{
  multimap<string,StringAttr*>::iterator iter;
  StringAttr						* pAttr;

  for ( iter = urlAttribute.lower_bound ( attributName );
    iter !=  urlAttribute.upper_bound ( attributName );
    iter ++ )
  {
    pAttr = iter->second;
    vAttributWerteP.push_back( pAttr );
  }

  return vAttributWerteP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle URL-Attribut Werte                                          //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getURLAttributWerte ( vector<string> &vAttributNamen, vector<StringAttr*> & vAttributWerte )
{
  multimap<string,StringAttr*>::iterator iter;

  for ( iter = urlAttribute.begin(); iter !=  urlAttribute.end(); iter ++ )
  {
    vAttributNamen.push_back( iter->first );
    vAttributWerte.push_back ( iter->second );
  }

  return vAttributNamen.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der URL-Attribute                                     //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getURLAttributWertAnz ( std::string attributName )
{
	return urlAttribute.count ( attributName);
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten unter attributName referierten Wert eines generischen //
//  URL-Attributs                                                            //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getGenericURLAttributWert ( std::string attributName, std::string & attributWert )
{
	size_t                   relAnz, i;
	bool				          	 success;
	vector<FeatureRelation*> relationen;
	FeatureRelation        * pRel;
	Feature			           * pObj;
	string                   attributNameLoc;

  relAnz = getFeatureRelationen ( "xplan:hatGenerAttribut", relationen );
	for ( i = 0; i < relAnz; i++ )
	{
		pRel = relationen[i];
		pObj = pFeatures->getFeature ( pRel->featureId );
    if ( pObj->getKlassenName() == "xplan:XP_URLAttribut" )
		{
      success = pObj->getStringAttributWert ( "xplan:name", attributNameLoc );
			if ( success && attributName == attributNameLoc) 
        return pObj->getURLAttributWert ( "xplan:wert", attributWert );
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die nicht belegten Attributwerte auf die Werte von pPlanObjekt     //
///////////////////////////////////////////////////////////////////////////////
void Feature::mergeAttribute ( Feature * pPlanObjekt )
{
	size_t                i, anz;
  string                attributName, attributWert;
  ATTRIBUT_TYP          attributTyp;
  vector<ATTRIBUT_TYP>  vAttributTypen;
  vector<std::string>   vAttributNamen; 
  vector<std::string>   vAttributWerte;
  vector<UOM*>          vUOMs;
  vector<DateAttr*>     vDateAttributWerte;

  anz = pPlanObjekt->getAttributWerte ( vAttributTypen, vAttributNamen, vAttributWerte, vUOMs );
  for ( i = 0; i < anz; i++ )
  {
    attributTyp = vAttributTypen[i];
    attributName = vAttributNamen[i];
    attributWert = vAttributWerte[i];

    switch ( attributTyp )
    {
    case _STRING:
    case _ENUMERATION:
    case _EXTERNAL_CODE_LIST:
    case  _EXTERNAL_CODE_LIST_GML3_3:
      if ( getStringAttributWertAnz ( attributName ) == 0 )
        setStringAttribut( attributName, attributWert);
      break;

    case _DOUBLE:
    case _ANGLE:
    case _LENGTH:
    case _VOLUME:
    case _MEASURE:

      if ( getDoubleAttributWertAnz ( attributName ) == 0 )
      {
        double wert = atof ( attributWert.c_str() );
        setDoubleAttribut( attributName, wert, vUOMs[i] );
      }
      break;

    case _INTEGER:
      if ( getIntegerAttributWertAnz ( attributName ) == 0 )
      {
        int wert = atoi ( attributWert.c_str() );
        setIntegerAttribut ( attributName, wert );
      }
      break;

    case _BOOLEAN:
      {
        bool b;
      
        if ( getBoolAttributWert( attributName, b ) == false )
        {
          if ( attributWert == "true" )
            setBoolAttribut ( attributName, true );
          else
            setBoolAttribut ( attributName, false );
        }
      }
      break;

    case _ANY_URI:
      if ( getURLAttributWertAnz ( attributName ) == 0 )
        setURLAttribut( attributName, attributWert);
      break;
    }
  }

  vAttributNamen.clear();
  anz = pPlanObjekt->getDateAttributWerte ( vAttributNamen, vDateAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    attributName = vAttributNamen[i];
    DateAttr * pTime = vDateAttributWerte[i];

    if ( getDateAttributWertAnz ( attributName ) == 0 )
      setDateAttribut( attributName, pTime);
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten Wert eines bestimmten Integer-Attributs  						 //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getIntegerAttributWert ( std::string attributName, int & attributWert )
{
	IntAttr * pAttr;
	if ( integerAttribute.lower_bound ( attributName ) != integerAttribute.upper_bound ( attributName ) )
	{
		pAttr = integerAttribute.lower_bound ( attributName )->second;
		attributWert = pAttr->value;
		return true;
	}
	else return false;
}

IntAttr * Feature::getIntegerAttributWert ( string attributName )
{
  IntAttr    * pAttr;

  if ( integerAttribute.lower_bound ( attributName ) != integerAttribute.upper_bound ( attributName ) )
  {
    pAttr = integerAttribute.lower_bound ( attributName )->second;
    return pAttr;
  }
  else 
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Werte eines bestimmten Integer-Attributs  						       //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getIntegerAttributWerte ( string attributName, vector<int> & vIntP )
{
	multimap<string,IntAttr*>::iterator iter;
	IntAttr							  * pAttr;

	for ( iter = integerAttribute.lower_bound ( attributName );
		  iter !=  integerAttribute.upper_bound ( attributName );
		  iter ++ )
	{
		pAttr = iter->second;
		vIntP.push_back ( pAttr->value );
	}

	return vIntP.size();
}

size_t Feature::getIntegerAttributWerte ( string attributName, vector<IntAttr*> & vAttributWerteP )
{
  multimap<string,IntAttr*>::iterator iter;
  IntAttr						                * pAttr;

  for ( iter = integerAttribute.lower_bound ( attributName );
    iter !=  integerAttribute.upper_bound ( attributName );
    iter ++ )
  {
    pAttr = iter->second;
    vAttributWerteP.push_back( pAttr );
  }

  return vAttributWerteP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Integer-Attribut Werte                   						       //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getIntegerAttributWerte ( vector<string> &vAttributNamen, vector<IntAttr*> & vAttributWerte )
{
  multimap<string,IntAttr*>::iterator iter;

  for ( iter = integerAttribute.begin(); iter !=  integerAttribute.end(); iter ++ )
  {
    vAttributNamen.push_back( iter->first );
    vAttributWerte.push_back ( iter->second );
  }

  return vAttributNamen.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Werte eines bestimmten Integer-Attributs		       //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getIntegerAttributWertAnz ( std::string attributName )
{
	return integerAttribute.count ( attributName);
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten unter attributName referierten Wert eines generischen //
//  Integer-Attributs                                                        //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getGenericIntegerAttributWert ( std::string attributName, int & attributWert )
{
	size_t                   relAnz, i;
	bool					           success;
	vector<FeatureRelation*> relationen;
	FeatureRelation        * pRel;
	Feature			           * pObj;
	string                   attributNameLoc;

  relAnz = getFeatureRelationen ( "xplan:hatGenerAttribut", relationen );
	for ( i = 0; i < relAnz; i++ )
	{
		pRel = relationen[i];
		pObj = pFeatures->getFeature ( pRel->featureId );
    if ( pObj->getKlassenName() == "xplan:XP_IntegerAttribut" )
		{
      success = pObj->getStringAttributWert ( "xplan:name", attributNameLoc );
			if ( success && attributName == attributNameLoc) 
        return pObj->getIntegerAttributWert ( "xplan:wert", attributWert );
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten Wert eines bestimmten Double-Attributs 							 //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getDoubleAttributWert ( std::string attributName, double & attributWert )
{
  DoubleAttr * pAttr;
  if ( doubleAttribute.lower_bound ( attributName ) != doubleAttribute.upper_bound ( attributName ) )
  {
    pAttr = doubleAttribute.lower_bound ( attributName )->second;
    attributWert = pAttr->value;
    return true;
  }
  else 
    return false;
}


DoubleAttr *  Feature::getDoubleAttributWert ( std::string attributName )
{
	DoubleAttr * pAttr;
	if ( doubleAttribute.lower_bound ( attributName ) != doubleAttribute.upper_bound ( attributName ) )
	{
		pAttr = doubleAttribute.lower_bound ( attributName )->second;
    return pAttr;
	}
	else 
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Werte eines bestimmten Double-Attributs 			      				 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getDoubleAttributWerte ( std::string attributName, vector<DoubleAttr*> & vAttributWerteP )
{
	multimap<string,DoubleAttr*>::iterator iter;
	DoubleAttr							             * pAttr;

	for ( iter = doubleAttribute.lower_bound ( attributName );
		  iter !=  doubleAttribute.upper_bound ( attributName );
		  iter ++ )
	{
		pAttr = iter->second;
    vAttributWerteP.push_back( pAttr );
	}

	return vAttributWerteP.size();
}

size_t Feature::getDoubleAttributWerte ( std::string attributName, vector<double> & vAttributWerteP )
{
  multimap<string,DoubleAttr*>::iterator iter;
  DoubleAttr							             * pAttr;

  for ( iter = doubleAttribute.lower_bound ( attributName );
    iter !=  doubleAttribute.upper_bound ( attributName );
    iter ++ )
  {
    pAttr = iter->second;
    vAttributWerteP.push_back( pAttr->value );
  }

  return vAttributWerteP.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Double-Attribut Werte                         							 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getDoubleAttributWerte ( vector<string> &vAttributNamen, vector<DoubleAttr*> & vAttributWerte )
{
  multimap<string,DoubleAttr*>::iterator iter;

  for ( iter = doubleAttribute.begin(); iter !=  doubleAttribute.end(); iter ++ )
  {
    vAttributNamen.push_back( iter->first );
    vAttributWerte.push_back ( iter->second );
  }

  return vAttributNamen.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Werte eines bestimmten Double-Attributs 			  	 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getDoubleAttributWertAnz ( std::string attributName )
{
	return doubleAttribute.count ( attributName);
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten unter attributName referierten Wert eines generischen //
// Double-Attributs                                                          //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getGenericDoubleAttributWert ( std::string attributName, double & attributWert, UOM *& pUOM )
{
	size_t                   relAnz, i;
	bool					           success;
	vector<FeatureRelation*> relationen;
	FeatureRelation        * pRel;
	Feature			           * pObj;
	string                   attributNameLoc;

  relAnz = getFeatureRelationen ( "xplan:hatGenerAttribut", relationen );
	for ( i = 0; i < relAnz; i++ )
	{
		pRel = relationen[i];
		pObj = pFeatures->getFeature ( pRel->featureId );
    if ( pObj->getKlassenName() == "xplan:XP_DoubleAttribut" )
		{
      success = pObj->getStringAttributWert ( "xplan:name", attributNameLoc );
			if ( success && attributName == attributNameLoc) 
      {
        DoubleAttr * pDoubleAttr = pObj->getDoubleAttributWert ( "xplan:wert" );
        if ( pDoubleAttr != NULL )
        {
          attributWert = pDoubleAttr->value;
          pUOM = pDoubleAttr->uom;
          return true;
        }
      }
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Wert eines bestimmten Boolean-Attributs          						 //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getBoolAttributWert ( std::string attributName, bool & attributWert )
{
	std::map<std::string,BoolAttr*>::iterator iter;

	iter = boolAttribute.find ( attributName );
	if ( iter != boolAttribute.end() )
	{
    attributWert = iter->second->value;
		return true;
	}
	else return false;
}

BoolAttr * Feature::getBoolAttributWert ( string attributName )
{
  std::map<std::string,BoolAttr*>::iterator iter;

  iter = boolAttribute.find ( attributName );
  if ( iter != boolAttribute.end() )
  {
    return iter->second;
  }
  else return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert Attribut-Wert für bel. Attribut-Typ. Der Wert wird in string	   //
//  konvertiert															                              	 //
//  Datum-Attribute werden nicht geliefert                                   //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getAttributWert ( ATTRIBUT_TYP attributTyp, std::string attributName, std::string & attributWert, UOM *& pUOM  )
{
	double   dAttributWert;
	int      iAttributWert;
	bool     bAttributWert;
	bool     success;
	System::String ^ STR;

	switch ( attributTyp )
	{
	case _ANY_URI:
		success = getURLAttributWert ( attributName, attributWert );
		return success;

	case _ENUMERATION:
	case _EXTERNAL_CODE_LIST:
  case _EXTERNAL_CODE_LIST_GML3_3:
	case _STRING:
		success = getStringAttributWert ( attributName, attributWert );
		return success;

	case _DOUBLE:
	case _ANGLE:
  case _LENGTH:
  case _AREA:
  case _VOLUME:
  case _MEASURE:
		success = getDoubleAttributWert ( attributName, dAttributWert );
		if ( success )
		{
			STR = System::Xml::XmlConvert::ToString ( dAttributWert );
			QuConvert::systemStr2stdStr ( attributWert, STR );
		}
		return success;

	case _INTEGER:
		success = getIntegerAttributWert ( attributName, iAttributWert );
		if ( success )
		{
			STR = System::Xml::XmlConvert::ToString ( iAttributWert );
			QuConvert::systemStr2stdStr ( attributWert, STR );
		}
		return success;

	case _BOOLEAN:
		success = getBoolAttributWert ( attributName, bAttributWert );
		if ( success )
		{
			STR = XmlConvert::ToString ( bAttributWert );
			QuConvert::systemStr2stdStr ( attributWert, STR );
		}
		return success;

	default:
		return false;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten Wert eines bestimmten Attributs im ersten referierten //
//  Feature. attributName ist ein zusammengesetzter String, der an erster    //
//  Stelle den Attribut-namen im referierten Objekt, und an zweiter Stelle   //
//  den Relations-Namen enthält, beides durch einen "." Character getrennt   //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getReferenzWert ( string referenziertesElement, string attributName, 
                                   ATTRIBUT_TYP & referenzAttributTyp, string & attributWert, UOM *& pUOM )
{
	size_t                    index, anz;
  string                    relationName, relationAttributName;
  vector<FeatureRelation*>  vObjRelationen;
  Feature                 * pPlanObjReferiert;
  GmlKlasse               * pGmlKlasse = pFeatures->getGmlSchema()->getGmlKlasseFromElementName( referenziertesElement );

  if ( pGmlKlasse == NULL ) return false;

  index = attributName.find( "." );
  if ( index == string::npos ) return false;

  relationName = attributName.substr ( 0, index );
  relationAttributName = attributName.substr ( index+1, attributName.length() );

  anz =  getFeatureRelationen( relationName, vObjRelationen );
  if ( anz == 0 ) return false;

  pPlanObjReferiert = pFeatures->getFeature( vObjRelationen[0]->featureId );
  if ( pPlanObjReferiert == NULL ) return false;
    
  GmlAttribut * pGmlAttribut = pGmlKlasse->getGmlAttribut( relationAttributName );
  if ( pGmlAttribut == NULL ) return false;

  referenzAttributTyp = pGmlAttribut->getGmlAttributTyp();

  return pPlanObjReferiert->getAttributWert( referenzAttributTyp, relationAttributName, attributWert, pUOM );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Attributwerte ohne Classifier                               //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getAttributWerte ( std::vector<ATTRIBUT_TYP> & vAttributTyp, 
                                   std::vector<std::string> & vAttributName, 
						                       std::vector<std::string> & vAttributWerte,
                                   std::vector<UOM*>        & vUOMs )
{
  double   h;
  int      ih;
  bool     b;
  String ^ STR;
  string   stdString;

  GmlKlasse   * pGmlKlasse = pFeatures->getGmlSchema()->getGmlKlasseFromElementName( klassenName );
  GmlAttribut * pGmlAttribut;

  multimap<string,StringAttr*>::iterator iterString;
  multimap<string,IntAttr*>::iterator    iterInteger;
  multimap<string,DoubleAttr*>::iterator iterDouble;
  multimap<string,DateAttr*>::iterator   iterDate;
  map<string,BoolAttr*>::iterator        iterBool;

  for ( iterString = stringAttribute.begin(); iterString != stringAttribute.end(); iterString++ )
  {
    stdString = iterString->first;
    vAttributName.push_back ( stdString );
    vAttributWerte.push_back ( iterString->second->value );
   
    pGmlAttribut = pGmlKlasse->getGmlAttribut( stdString );
    vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );
    vUOMs.push_back( NULL );
 }

  for ( iterInteger = integerAttribute.begin(); iterInteger != integerAttribute.end(); iterInteger++ )
  {
    vAttributName.push_back ( iterInteger->first );
    ih = iterInteger->second->value;
    STR = XmlConvert::ToString ( ih );
    QuConvert::systemStr2stdStr ( stdString, STR );
    vAttributWerte.push_back ( stdString );

    pGmlAttribut = pGmlKlasse->getGmlAttribut( iterInteger->first );
    vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );
    vUOMs.push_back( NULL );
  }

  for ( iterDouble = doubleAttribute.begin(); iterDouble != doubleAttribute.end(); iterDouble++ )
  {
    vAttributName.push_back ( iterDouble->first );
    h = iterDouble->second->value;
    STR = XmlConvert::ToString ( h );
    QuConvert::systemStr2stdStr ( stdString, STR );
    vAttributWerte.push_back ( stdString );
    vUOMs.push_back( iterDouble->second->uom );

    pGmlAttribut = pGmlKlasse->getGmlAttribut( iterDouble->first );
    vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );
  }

  for ( iterBool = boolAttribute.begin(); iterBool != boolAttribute.end(); iterBool++ )
  {
    vAttributName.push_back ( iterBool->first );
    b = iterBool->second->value;
    if ( b ) stdString = "true";
    else     stdString = "false";
    vAttributWerte.push_back ( stdString );

    pGmlAttribut = pGmlKlasse->getGmlAttribut( iterBool->first );
    vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );
    vUOMs.push_back( NULL );
  }


  for ( iterString = urlAttribute.begin(); iterString != urlAttribute.end(); iterString++ )
  {
    vAttributName.push_back ( iterString->first );
    vAttributWerte.push_back ( iterString->second->value );
    pGmlAttribut = pGmlKlasse->getGmlAttribut( iterString->first );
    vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );
    vUOMs.push_back( NULL );
  }

  for ( iterDate = dateAttribute.begin(); iterDate != dateAttribute.end(); iterDate++ )
  {
    vAttributName.push_back ( iterDate->first );
    vAttributWerte.push_back( iterDate->second->value->toString() );
    pGmlAttribut = pGmlKlasse->getGmlAttribut( iterDate->first );
    vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );
    vUOMs.push_back( NULL );
  }

  return vAttributName.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Attributwerte mit Classifiern                               //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getAttributWerte ( std::vector<ATTRIBUT_TYP> & vAttributTyp, 
                                  std::vector<std::string> & vAttributName, 
                                  std::vector<std::string> & vAttributWerte,
                                  std::vector<size_t>      & vClassifierAnz,
                                  std::vector<std::string> & vClassifierNamen,
                                  std::vector<std::string> & vClassifierValues,
                                  std::vector<UOM*>        & vUOMs  )
{
  double       h;
  int          ih;
  bool         b;
  String     ^ STR;
  string       stdString;
  DoubleAttr * pDoubleAttr;
  StringAttr * pStringAttr;
  IntAttr    * pIntAttr;
  BoolAttr   * pBoolAttr;

  GmlKlasse   * pGmlKlasse = pFeatures->getGmlSchema()->getGmlKlasseFromElementName( klassenName );
  GmlAttribut * pGmlAttribut;

  multimap<string,StringAttr*>::iterator iterString;
  multimap<string,IntAttr*>::iterator    iterInteger;
  multimap<string,DoubleAttr*>::iterator iterDouble;
  multimap<string,DateAttr*>::iterator   iterDate;
  map<string,BoolAttr*>::iterator        iterBool;
  map<string,string>::iterator           iterClassifier;

  if ( pGmlKlasse == NULL ) return 0;

  for ( iterString = stringAttribute.begin(); iterString != stringAttribute.end(); iterString++ )
  {
    stdString = iterString->first;
    pStringAttr = iterString->second;

    pGmlAttribut = pGmlKlasse->getGmlAttribut( stdString );
    if ( pGmlAttribut != NULL )
    {
      vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );  
      vAttributName.push_back ( stdString );
      vAttributWerte.push_back ( pStringAttr->value );
      vUOMs.push_back( NULL );
      for ( iterClassifier = pStringAttr->classifier.begin(); iterClassifier != pStringAttr->classifier.end(); iterClassifier++ )
      {
        vClassifierNamen.push_back( iterClassifier->first);
        vClassifierValues.push_back( iterClassifier->second );
      }
      vClassifierAnz.push_back( pStringAttr->classifier.size() );
    }

  }

  for ( iterInteger = integerAttribute.begin(); iterInteger != integerAttribute.end(); iterInteger++ )
  {
    pGmlAttribut = pGmlKlasse->getGmlAttribut( iterInteger->first );
    if ( pGmlAttribut != NULL )
    {
      vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );
      vAttributName.push_back ( iterInteger->first );
      pIntAttr = iterInteger->second;

      ih = pIntAttr->value;
      STR = XmlConvert::ToString ( ih );
      QuConvert::systemStr2stdStr ( stdString, STR );
      vAttributWerte.push_back ( stdString );
      vUOMs.push_back( NULL );
      for ( iterClassifier = pIntAttr->classifier.begin(); iterClassifier != pIntAttr->classifier.end(); iterClassifier++ )
      {
        vClassifierNamen.push_back( iterClassifier->first );
        vClassifierValues.push_back( iterClassifier->second );
      }
      vClassifierAnz.push_back( pIntAttr->classifier.size() );
    }
  }

  for ( iterDouble = doubleAttribute.begin(); iterDouble != doubleAttribute.end(); iterDouble++ )
  {
    pGmlAttribut = pGmlKlasse->getGmlAttribut( iterDouble->first );
    if ( pGmlAttribut != NULL )
    {
      vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );
      vAttributName.push_back ( iterDouble->first );
      pDoubleAttr = iterDouble->second;

      h = pDoubleAttr->value;
      STR = XmlConvert::ToString ( h );
      QuConvert::systemStr2stdStr ( stdString, STR );
      vAttributWerte.push_back ( stdString );
      vUOMs.push_back( pDoubleAttr->uom );
      for ( iterClassifier = pDoubleAttr->classifier.begin(); iterClassifier != pDoubleAttr->classifier.end(); iterClassifier++ )
      {
        vClassifierNamen.push_back( iterClassifier->first );
        vClassifierValues.push_back( iterClassifier->second );
      }
      vClassifierAnz.push_back( pDoubleAttr->classifier.size() );
    }
  }

  for ( iterBool = boolAttribute.begin(); iterBool != boolAttribute.end(); iterBool++ )
  {
    pGmlAttribut = pGmlKlasse->getGmlAttribut( iterBool->first );
    if ( pGmlAttribut != NULL )
    {
      vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );
      vAttributName.push_back ( iterBool->first );
      pBoolAttr = iterBool->second;
      b = pBoolAttr->value;
      if ( b ) stdString = "true";
      else     stdString = "false";
      vAttributWerte.push_back ( stdString );
      vUOMs.push_back( NULL );
      for ( iterClassifier = pBoolAttr->classifier.begin(); iterClassifier != pBoolAttr->classifier.end(); iterClassifier++ )
      {
        vClassifierNamen.push_back( iterClassifier->first );
        vClassifierValues.push_back( iterClassifier->second );
      }
      vClassifierAnz.push_back( pBoolAttr->classifier.size() );
    }
  }

  for ( iterString = urlAttribute.begin(); iterString != urlAttribute.end(); iterString++ )
  {
    pGmlAttribut = pGmlKlasse->getGmlAttribut( iterString->first );
    if ( pGmlAttribut != NULL )
    {
      vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );
      pStringAttr = iterString->second;

      vAttributName.push_back ( iterString->first );
      vAttributWerte.push_back ( pStringAttr->value );
      vUOMs.push_back( NULL );
      for ( iterClassifier = pStringAttr->classifier.begin(); iterClassifier != pStringAttr->classifier.end(); iterClassifier++ )
      {
        vClassifierNamen.push_back( iterClassifier->first );
        vClassifierValues.push_back( iterClassifier->second );
      }
      vClassifierAnz.push_back( pStringAttr->classifier.size() );
    }
  }

  for ( iterDate = dateAttribute.begin(); iterDate != dateAttribute.end(); iterDate++ )
  {
    pGmlAttribut = pGmlKlasse->getGmlAttribut( iterDate->first );
    if ( pGmlAttribut != NULL )
    {
      vAttributTyp.push_back ( pGmlAttribut->getGmlAttributTyp() );
      vAttributName.push_back ( iterDate->first );
      DateAttr * pDateAttr = iterDate->second;
      AbstractTime * pTime = pDateAttr->value;
      if ( pTime != NULL )
        vAttributWerte.push_back( pTime->toString() );
      else
        vAttributWerte.push_back( "nil" );

      vUOMs.push_back( NULL );
      for ( iterClassifier = pDateAttr->classifier.begin(); iterClassifier != pDateAttr->classifier.end(); iterClassifier++ )
      {
        vClassifierNamen.push_back( iterClassifier->first );
        vClassifierValues.push_back( iterClassifier->second );
      }
      vClassifierAnz.push_back( pDateAttr->classifier.size() );
    }
  }

  return vAttributName.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Werte eines bestimmten Attributs mit den Classifiern,       //
//  die den Namen codeSpace haben                                            //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getAttributWerte ( ATTRIBUT_TYP attributTyp, std::string attributName, std::vector<std::string> & vStringP, 
                std::vector<std::string> & vCodeSpaces, std::vector<UOM*> & vUOMs )
{
	vector<DoubleAttr*> dAttributWerte;
  vector<StringAttr*> sAttributWerte;
	vector<int>         iAttributWerte;
  vector<DateAttr*>   vDateAttrWerte;
	bool			          bAttributWert;
	string              attributWert;
	bool			          success;
	size_t              i, anz;
	System::String    ^ STR;

	switch ( attributTyp )
	{
	case _ANY_URI:
		anz = getURLAttributWerte ( attributName, vStringP );
    for ( i = 0; i < anz; i++ )
      vUOMs.push_back( NULL );
		return anz;

	case _ENUMERATION:
	case _EXTERNAL_CODE_LIST:
  case _EXTERNAL_CODE_LIST_GML3_3:
	case _STRING:

		anz = getStringAttributWerte ( attributName, sAttributWerte );
    for ( i = 0; i < anz; i++ )
    {
      vStringP.push_back(sAttributWerte[i]->value );
      if ( sAttributWerte[i]->classifier.find( "codeSpace" ) != sAttributWerte[i]->classifier.end() )
        vCodeSpaces.push_back( sAttributWerte[i]->classifier["codeSpace"] );
      else
        vCodeSpaces.push_back ( "" );
      vUOMs.push_back( NULL );
    }
		return anz;

	case _DOUBLE:
	case _ANGLE:
  case _LENGTH:
  case _AREA:
  case _VOLUME:
		anz = getDoubleAttributWerte ( attributName, dAttributWerte );
		for ( i = 0; i < anz; i++ )
		{
			STR = System::Xml::XmlConvert::ToString ( dAttributWerte[i]->value );
			QuConvert::systemStr2stdStr ( attributWert, STR );
			vStringP.push_back ( attributWert );
      vUOMs.push_back(  dAttributWerte[i]->uom );
		}
		return anz;

	case _INTEGER:
		anz = getIntegerAttributWerte ( attributName, iAttributWerte );
		for ( i = 0; i < anz; i++ )
		{
			STR = System::Xml::XmlConvert::ToString ( iAttributWerte[i] );
			QuConvert::systemStr2stdStr ( attributWert, STR );
			vStringP.push_back ( attributWert );
      vUOMs.push_back( NULL );
		}
		return anz;

	case _BOOLEAN:
		success = getBoolAttributWert ( attributName, bAttributWert );
		if ( success )
		{
			STR = XmlConvert::ToString ( bAttributWert );
			QuConvert::systemStr2stdStr ( attributWert, STR );
			vStringP.push_back ( attributWert );
      vUOMs.push_back( NULL );
	    return 1;
		}
		else return 0;

  case _DATE:
  case _DATE_TIME:
  case TIME_POSITION:
    anz = getDateAttributWerte ( attributName, vDateAttrWerte );
    for ( i = 0; i < anz; i++ )
    {
      vStringP.push_back (  vDateAttrWerte[i]->value->toString() );
      vUOMs.push_back( NULL );
    }
    return anz;



	default:
		return 0;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Werte eines bestimmten Attributs und Typs         //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getAttributWerteAnz ( ATTRIBUT_TYP attributTyp, std::string attributName )
{
	string           attributWert;
	bool			 success, b;

	switch ( attributTyp )
	{
	case _ANY_URI:
		return getURLAttributWertAnz ( attributName );

	case _ENUMERATION:
	case _EXTERNAL_CODE_LIST:
  case _EXTERNAL_CODE_LIST_GML3_3:
	case _STRING:
		return getStringAttributWertAnz ( attributName );

	case _DATE:
  case _DATE_TIME:
  case TIME_POSITION:
		return getDateAttributWertAnz ( attributName );

	case _DOUBLE:
	case _ANGLE:
		return getDoubleAttributWertAnz ( attributName );

	case _INTEGER:
		return getIntegerAttributWertAnz ( attributName );

	case _BOOLEAN:
		success = getBoolAttributWert ( attributName, b );
		if ( success )  return 1;
		else			return 0;

	default:
		return 0;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Werte aller Attribute mit Namen attributName      //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getAttributWerteAnz ( std::string attributName )
{
	size_t anz;
  bool b;

  anz =  getURLAttributWertAnz ( attributName ) + getStringAttributWertAnz ( attributName ) +  getDateAttributWertAnz ( attributName ) +
         getDoubleAttributWertAnz ( attributName ) + getIntegerAttributWertAnz ( attributName ) + getFeatureRelationenAnz( attributName );

  if ( getBoolAttributWert( attributName, b ) )
    anz += 1;

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Feature-Relation hinzu		                    								 //
///////////////////////////////////////////////////////////////////////////////
void Feature::addFeatureRelation ( FeatureRelation * pRelationP )
{
	string rollenName = pRelationP->rollenName;
	
	objRelationen.insert ( multimap<string,FeatureRelation*>::value_type ( rollenName, pRelationP ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt eine Relation auf ein Feature, ohne das Relations-Objekt    		 //
//  zu löschen                                                               //
///////////////////////////////////////////////////////////////////////////////
FeatureRelation *  Feature::removeFeatureRelation ( Feature * pObjP )
{
	string									    objId;
	FeatureRelation							  * pRel = nullptr;
	multimap<string,FeatureRelation*>::iterator iter;

	objId = pObjP->getGmlId();

	for ( iter = objRelationen.begin();
		  iter !=  objRelationen.end ();
		  iter ++ )
	{
		pRel = iter->second;
		if ( pRel->featureId == objId )
			break;
	}
	if ( iter != objRelationen.end () )
	{
		objRelationen.erase ( iter );
    return pRel;
	}
  else
    return NULL;
}

bool Feature::removeFeatureRelation (  FeatureRelation * pRelation )
{
  FeatureRelation							              * pRel;
  multimap<string,FeatureRelation*>::iterator iter;

  for ( iter = objRelationen.begin();
    iter !=  objRelationen.end ();
    iter ++ )
  {
    pRel = iter->second;
    if ( pRel == pRelation )
      break;
  }
  if ( iter != objRelationen.end () )
  {
    objRelationen.erase ( iter );
    return true;
  }
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt eine Relation auf ein Feature und löscht das Relations-Objekt   //
///////////////////////////////////////////////////////////////////////////////
void  Feature::deleteFeatureRelation ( Feature * pObjP )
{
  FeatureRelation * pRel = removeFeatureRelation( pObjP );

  if ( pRel != NULL ) delete pRel;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Feature-Relationen						                    					 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getFeatureRelationen ( vector<FeatureRelation*> & objRelationenP )
{
	multimap<string,FeatureRelation*>::iterator iter;

	for ( iter = objRelationen.begin(); iter != objRelationen.end(); iter++ )
	{
		FeatureRelation * pRel = iter->second;
		objRelationenP.push_back ( pRel );
	}

	return objRelationenP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Feature-Relationen zu einem bestimmten Relations-Namen 		 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getFeatureRelationen ( string rollenNameP, vector<FeatureRelation*> & objRelationenP )
{
	multimap<string,FeatureRelation*>::iterator iter;

	for ( iter = objRelationen.lower_bound ( rollenNameP );
		  iter !=  objRelationen.upper_bound ( rollenNameP );
		  iter ++ )
	{
		FeatureRelation * pRel = iter->second;
		objRelationenP.push_back ( pRel );
	}

	return objRelationenP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle über einen Relations-Namen referierte Features  						 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getRelatedFeatures ( string rollenNameP, vector<Feature*> & vFeatures )
{
  multimap<string,FeatureRelation*>::iterator iter;
  FeatureRelation                           * pRel;
  Feature                                   * pFeature;

  for ( iter = objRelationen.lower_bound ( rollenNameP );
    iter !=  objRelationen.upper_bound ( rollenNameP );
    iter ++ )
  {
    pRel = iter->second;
    pFeature = pFeatures->getFeature ( pRel->featureId );
    if ( pFeature != NULL )
      vFeatures.push_back ( pFeature );
  }

  return vFeatures.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das erste über einen Relations-Namen referierte Feature     		 //
///////////////////////////////////////////////////////////////////////////////
Feature * Feature::getRelatedFeature ( std::string rollenNameP )
{
  if ( objRelationen.lower_bound ( rollenNameP ) !=  objRelationen.upper_bound ( rollenNameP ))
  {
    FeatureRelation * pRel = objRelationen.lower_bound ( rollenNameP )->second;
    Feature * pFeature =  pFeatures->getFeature ( pRel->featureId );
    return pFeature;
  }
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Liefert das über eine FeatureRelation adressierte Feature                 //
///////////////////////////////////////////////////////////////////////////////
Feature * Feature::getRelatedFeature ( FeatureRelation * pRel )
{
  if ( pRel == NULL )
    return NULL;

  Feature * pFeature =  pFeatures->getFeature( pRel->featureId );
  return pFeature;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der über einen Relations-Namen referierten Features 	 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getFeatureRelationenAnz ( std::string rollenNameP )
{

	return objRelationen.count ( rollenNameP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Relations-Objekt für ein referiertes Feature           		   //
///////////////////////////////////////////////////////////////////////////////
FeatureRelation * Feature::getFeatureRelation ( Feature * pObjP )
{
  string									                    objId;
  FeatureRelation							              * pRel;
  multimap<string,FeatureRelation*>::iterator iter;

  objId = pObjP->getGmlId();

  for ( iter = objRelationen.begin();
    iter !=  objRelationen.end ();
    iter ++ )
  {
    pRel = iter->second;
    if ( pRel->featureId == objId )  return pRel;
  }
   
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt und löscht alle Feature-Relationen zu einem Relations-Namen 		 //
///////////////////////////////////////////////////////////////////////////////
void Feature::deleteFeatureRelationen ( std::string rollenNameP )
{
	multimap<string,FeatureRelation*>::iterator iter;

	for ( iter = objRelationen.lower_bound ( rollenNameP );
		  iter !=  objRelationen.upper_bound ( rollenNameP );
		  iter ++ )
	{
		FeatureRelation * pRel = iter->second;
		delete pRel;
	}

	objRelationen.erase ( rollenNameP );
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob das Feature ein Attribut gmlId != "" hat                   //
// Returnwert ist 1, wenn keine gmlId gesetzt ist, sonst 0							     //
///////////////////////////////////////////////////////////////////////////////
int Feature::checkFeatureIdsExist()
{
	if ( gmlId == "" ) return 1;
	else			   return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Koordinaten des Feature-Mittelpunktes, wobei alle referierten		 //
//  Geometrieobjekte berücksichtigt werden                                   //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getCenter2D ( double &xP, double &yP )
{
  double                                 xLoc, yLoc;
  bool                                   success;
  multimap<string,_Geometrie*>::iterator iter;

  if ( mGeometrie.size() == 0 )
    return false;
  xP = 0.0;
  yP = 0.0;

  for ( iter = mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
  {
    _Geometrie * pGeo = iter->second;
    success = pGeo->getCenter2D( xLoc, yLoc );
    if ( !success ) 
      return false;
    xP = xP + xLoc;
    yP = yP + yLoc;
  }

  xP = xP / mGeometrie.size();
  yP = yP / mGeometrie.size();
  return true;
}

bool Feature::getCenter3D ( double &xP, double &yP, double &zP )
{
  double                                 xLoc, yLoc, zLoc;
  bool                                   success;
  multimap<string,_Geometrie*>::iterator iter;

  if ( mGeometrie.size() == 0 )
    return false;
  xP = 0.0;
  yP = 0.0;
  zP = 0.0;

  for ( iter = mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
  {
    _Geometrie * pGeo = iter->second;
    success = pGeo->getCenter3D( xLoc, yLoc, zLoc );
    if ( !success )
      return false;
    xP = xP + xLoc;
    yP = yP + yLoc;
    zP = zP + zLoc;
  }

  xP = xP / mGeometrie.size();
  yP = yP / mGeometrie.size();
  zP = zP / mGeometrie.size();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die BoundingBox des Features, wobei alle referierte              //
//  Geometrieobjekte berücksichtigt werden                                   //
//  Wenn das Feature keine Geometrie besitzt, wird die gemeinsame BoundingBox//
//  der referierten Features geliefert                                       //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getBoundingBox ( double * sw, double * no, std::string &srsNameP, int &srsDimensionP, bool recursive )
{
  double    swLoc[3], noLoc[3];
  bool      success;
  bool      geoExist = false;

  multimap<string,_Geometrie*>::iterator      iter;
  multimap<string,FeatureRelation*>::iterator iterRel;

  srsNameP      = pFeatures->getSrsName();
  if ( srsNameP == "" )
    srsNameP = srsName;
  srsDimensionP = pFeatures->getSrsDimension();

  sw[0] =  1.0E20;
  sw[1] =  1.0E20;
  sw[2] =  1.0E20;
  no[0] = -1.0E20;
  no[1] = -1.0E20;
  no[2] = -1.0E20;

  if ( mGeometrie.size() > 0 )
  {
    for ( iter = mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
    {
      _Geometrie * pGeo = iter->second;
      if ( pGeo != NULL )
      {
        success = pGeo->getBoundingBox ( swLoc, noLoc );
        if ( !success ) 
          return false;
      }
      else
        return false;

      if ( srsNameP == "" )
        srsNameP = pGeo->getSrsName();

      if ( srsDimensionP < 0 )
        srsDimensionP = pGeo->getDim();

      if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
      if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
      if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
      if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
      if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
      if ( noLoc[2] > no[2] ) no[2] = noLoc[2];

      geoExist = true;
    }
  }
  else
	if ( recursive )
  {
    for ( iterRel = objRelationen.begin(); iterRel != objRelationen.end(); iterRel++ )
    {
      FeatureRelation * pRel = iterRel->second;
      Feature * pFeatureRel = pFeatures->getFeature( pRel->featureId );
      if ( pFeatureRel != NULL )
      {
        success = pFeatureRel->getBoundingBox( swLoc, noLoc, srsNameP, srsDimensionP, recursive );
        if ( success )
        {
          if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
          if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
          if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
          if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
          if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
          if ( noLoc[2] > no[2] ) no[2] = noLoc[2];

          geoExist = true;
        }
      }
    }  
  }
  return geoExist;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Symbol-Position hinzu		                      										 //
///////////////////////////////////////////////////////////////////////////////
void Feature::addSymbolPosition ( GeoPoint * pPointP )
{
	symbolPositionen.push_back ( pPointP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Symbol-Positionen			                    								 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getSymbolPositionen ( std::vector<GeoPoint*> &symbolPositionenP )
{
	unsigned int i;

	for ( i = 0; i < symbolPositionen.size(); i++ )
		symbolPositionenP.push_back ( symbolPositionen[i] );

	return symbolPositionen.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht alle Symbol-Positionen						                      					 //
///////////////////////////////////////////////////////////////////////////////
void Feature::deleteSymbolPositionen()
{
	unsigned int i;

	for ( i = 0; i < symbolPositionen.size(); i++ )
		delete symbolPositionen[i];

	symbolPositionen.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML: Liefert Attributwert flaechenschluss	           					 //
///////////////////////////////////////////////////////////////////////////////
bool Feature::getFlaechenschluss ()
{
	bool success, flaechenschluss;

	success = getBoolAttributWert ( "flaechenschluss", flaechenschluss );
	if ( success )
		return flaechenschluss;
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine GeometryProperty zur map mGeometrie hinzu. Falls der Property  //
//  bereits eine Geometrie zugewiesen wurde wird diese überschrieben         //
///////////////////////////////////////////////////////////////////////////////
void Feature::addGeometryProperty ( std::string propertyName, _Geometrie * pGeometrie )
{
  multimap<string,_Geometrie*>::iterator iter;

  mGeometrie.insert( multimap<string,_Geometrie*>::value_type ( propertyName, pGeometrie ) );
}
///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Liefert für einen gegebenen RepresnetationContext die zugehörige         //
//  Representation                                                           //
///////////////////////////////////////////////////////////////////////////////
Feature * Feature::getCityGMLRepresentation (  Feature * pRepContext, string namespaceKuerzel )
{
  vector<Feature*> vReps;
  vector<Feature*> vRepContext;

  string           rollenNameRepresentation = namespaceKuerzel + ":" + "representation";
  string           rollenNameContext        = namespaceKuerzel + ":" + "context";

	size_t anz = getRelatedFeatures( rollenNameRepresentation, vReps );
  if ( anz == 0 )
    return NULL;

  for ( size_t i = 0; i < anz; i++ )
  {
    Feature * pRep = vReps[i];
    vRepContext.clear();
		size_t anzContext = pRep->getRelatedFeatures( rollenNameContext, vRepContext );
    if ( anzContext == 0 ) continue;

    if ( vRepContext[0] == pRepContext )
      return pRep;
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Fügt für CityGML-Modelle eine neue Geometrie-Repräsentation hinzu        //
///////////////////////////////////////////////////////////////////////////////
void Feature::addCityGMLGeometryRepresentationWithContext ( _Geometrie *pGeo, std::string namespaceKuerzel,  
                string contextIdentifier, string contextName, double precision  )
{
  string    representationItemPropertyName;
  string    representationPropertyName;
  string    representationItemName;
  string    representationName;
  string    representationContextPropertyName;
  string    geometryPropertyName;
  Feature * pRep;
  Feature * pRepContext;
  Feature * pRepItem;

  if ( !pFeatures->isCityGML() )
    return;

  representationName         = namespaceKuerzel + ":" + "ShapeRepresentation";
  representationPropertyName = namespaceKuerzel + ":" + "representation";
  pRep = addNewChildFeature( representationName, representationPropertyName  );

  representationName         = namespaceKuerzel + ":" + "RepresentationContext";
  representationPropertyName = namespaceKuerzel + ":" + "context";
  pRepContext = pRep->addNewChildFeature( representationName, representationPropertyName  );

  pRepContext->setURLAttribut (  namespaceKuerzel + ":" + "contextIdentifier", contextIdentifier );
  if ( contextName != "" )
  {
    pRepContext->setStringAttribut(  namespaceKuerzel + ":" + "contextName", contextName );
    pRepContext->setStringAttribut( "gml:name", contextName );
  }

  if ( precision != 0.0 )
    pRepContext->setDoubleAttribut(  namespaceKuerzel + ":" + "precision", precision );

  representationItemName = getCityGMLRepresentationItemName ( pGeo );
  if ( representationItemName == "" )
    return;

  representationItemName            = namespaceKuerzel + ":" + representationItemName;
  representationItemPropertyName    =  namespaceKuerzel + ":" + "representationItem";
  geometryPropertyName              = namespaceKuerzel + ":" + "geometryProperty";

  pRepItem = pRep->addNewChildFeature( representationItemName, representationItemPropertyName );
  pRepItem->addGeometryProperty ( geometryPropertyName, pGeo );
}

void Feature::addCityGMLGeometryRepresentation ( Feature * pRepContext, _Geometrie *pGeo, string namespaceKuerzel )
{
  string    representationItemPropertyName;
  string    representationPropertyName;
  string    representationItemName;
  string    representationName;
  string    representationContextPropertyName;
  string    geometryPropertyName;
  Feature * pRep;
  Feature * pRepItem;

  if ( !pFeatures->isCityGML() ||  pRepContext == NULL )
    return;

  representationItemName = getCityGMLRepresentationItemName ( pGeo );
  if ( representationItemName == "" )
    return;

  representationItemName            = namespaceKuerzel + ":" + representationItemName;
  representationItemPropertyName    =  namespaceKuerzel + ":" + "representationItem";
  representationContextPropertyName =  namespaceKuerzel + ":" + "context";
  geometryPropertyName              = namespaceKuerzel + ":" + "geometryProperty";


  pRep = getCityGMLRepresentation( pRepContext, namespaceKuerzel );
  if ( pRep == NULL )
  {
    representationName         = namespaceKuerzel + ":" + "ShapeRepresentation";
    representationPropertyName = namespaceKuerzel + ":" + "representation";
    pRep = addNewChildFeature( representationName, representationPropertyName  );
    pRep->addRelatedFeature( pRepContext, representationContextPropertyName, true );
  }

  pRepItem = pRep->addNewChildFeature( representationItemName, representationItemPropertyName );
  pRepItem->addGeometryProperty ( geometryPropertyName, pGeo );
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Liefert den Representation-Item Namen für ein bestimmtes Geometrieobjekt //
///////////////////////////////////////////////////////////////////////////////
string Feature::getCityGMLRepresentationItemName ( _Geometrie * pGeo )
{
  string representationItemName = "";

  switch ( pGeo->getGeometryType() )
  {
  case GEOMETRY_REFERENCE:
    representationItemName = getCityGMLRepresentationItemName( ((GeometryReference*)pGeo)->getRefGeoObjekt() );
    break;

  case GEO_POINT:
  case DIRECT_POSITION:
    representationItemName = "PointRepresentation";
    break;

  case SURFACE_REFERENCE:
    representationItemName = getCityGMLRepresentationItemName( ((SurfaceReference*)pGeo)->getRefGeoObjekt() );
    break;

  case CURVE_REFERENCE:
    representationItemName = getCityGMLRepresentationItemName( ((CurveReference*)pGeo)->getRefGeoObjekt() );
    break;

  case SOLID_REFERENCE:
    representationItemName = getCityGMLRepresentationItemName( ((SolidReference*)pGeo)->getRefGeoObjekt() );
    break;

  case MULTI_GEOMETRY:
  case GEOMETRIC_COMPLEX:
  case ALLG_GEOMETRIE:
    representationItemName = "GeometryRepresentation";
    break;

  case MULTI_SURFACE:
  case COMPOSITE_SURFACE:
  case SHELL:
    representationItemName = "MultiSurfaceRepresentation";
    break;

  case MULTI_CURVE:
    representationItemName = "MultiCurveRepresentation";
    break;

  case MULTI_POINT:
    representationItemName = "MultiPointRepresentation";
    break;

  case ORIENTABLE_SURFACE:
  case SURFACE:
  case TRIANGULATED_SURFACE:
  case POLYGON:
  case TRIANGLE:
    representationItemName = "SurfaceRepresentation";
    break;

  case CURVE:
  case LINE_STRING:
	case LINE_STRING_SEGMENT:
  case ARC:
  case CIRCLE:
    representationItemName = "CurveRepresentation";
    break;

  case SOLID:
    representationItemName = "SolidRepresentation";
    break;

  case MULTI_SOLID:
    representationItemName = "MultiSolidRepresentation";
    break;

  case IMPLICIT_GEOMETRY:
  case IMPLICIT_GEOMETRY_REFERENCE:
    representationItemName = "ImplicitRepresentation";
    break;
  }

  return representationItemName;  
}

/////////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:
//  Entfernt eine CityGML Representation mit dem übergebenen Kontext-Identifier//
/////////////////////////////////////////////////////////////////////////////////
void Feature::deleteCityGMLGeometryRepresentation ( std::string repContextIdentifier )
{
  vector<Feature*> vRepresentations;
  Feature        * pRepContext = nullptr;
  Feature        * pShapeRepresentation = nullptr;
  Feature        * pRepresentationItem;
  string           contextIdentifier;
	size_t           i, anz;
  bool             success, found;


  anz = getRelatedFeatures( "rep:representation", vRepresentations );
  if ( anz == 0 ) return;

  found = false;
  for ( i = 0; i < anz; i++ )
  {
    pShapeRepresentation = vRepresentations[i];
    pRepContext          = pShapeRepresentation->getRelatedFeature( "rep:context" );
    if ( pRepContext != NULL )
    {
      success = pRepContext->getURLAttributWert( "rep:contextIdentifier", contextIdentifier );
      if ( success && contextIdentifier == repContextIdentifier )
      {
        found = true;
        break;
      }
    }
  }

  if ( found )
  {
    if ( pRepContext->getParent() == pShapeRepresentation )
      pFeatures->removeFeature ( pRepContext->getGmlId() );

    pRepresentationItem = pShapeRepresentation->getRelatedFeature( "rep:representationItem" );
    if ( pRepresentationItem != NULL )
      pFeatures->removeFeature( pRepresentationItem->getGmlId() );

    FeatureRelation * pRel = removeFeatureRelation( pShapeRepresentation );
    delete pRel;

    pFeatures->removeFeature( pShapeRepresentation->getGmlId() );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert ein neues untergeordnetes Feature und fügt es dem Feature      //
//  unter dem festgelegten propertyName hinzu                                //
///////////////////////////////////////////////////////////////////////////////
Feature * Feature::addNewChildFeature ( string className, string propertyName, OBJEKT_TYP featureType )
{
  string gmlIdChild     = Constant::createGmlId();
  Feature * pNewFeature = new Feature ( featureType, gmlIdChild, pFeatures, className );

  pFeatures->addFeature( pNewFeature );

  pNewFeature->setParentId( gmlId );
  addChild( gmlIdChild );

  FeatureRelation * pRel = new FeatureRelation;
  pRel->featureId = gmlIdChild;
  pRel->relToFeature = true;
  pRel->rollenName = propertyName;

  addFeatureRelation( pRel );
  return pNewFeature;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein bereits existierendes Feature über eine Relation hinzu          //
///////////////////////////////////////////////////////////////////////////////
void Feature::addRelatedFeature ( Feature * pChildFeature, std::string propertyName, bool relToFeature )
{

  addChild( pChildFeature->getGmlId() );
  
  FeatureRelation * pRel = new FeatureRelation;
  pRel->featureId    = pChildFeature->getGmlId();
  pRel->rollenName   = propertyName;
  pRel->relToFeature = relToFeature;

  addFeatureRelation( pRel );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die zu einer GeometryProperty gehörige Geometrie                 //
///////////////////////////////////////////////////////////////////////////////
_Geometrie * Feature::getGeometry ( std::string propertyName, size_t index )
{
  unsigned int i = 0;
  multimap<string,_Geometrie*>::iterator iter;

  if ( index < 0 || index >= mGeometrie.count( propertyName ) )
    return NULL;

  for ( iter = mGeometrie.lower_bound( propertyName ); iter != mGeometrie.upper_bound( propertyName ); iter++ )
  {
    if ( i == index )
      break;
    else
      i++;
  }

  return iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Liefert alle referierte Geometrie-Objekte einer best. lodStufe           //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getGeometry ( std::vector<_Geometrie*> &vGeometrie, CITYGML_LOD lodStufe )
{
  multimap<string,_Geometrie*>::iterator iter;
  string                                 geometryProperty;
  string                                 prefix = "";
  bool                                   success;
  int                                    lod;
  char                                   buffer[10];

  if ( klassenName == "relief:TINRelief"       ||
       klassenName == "relief:MassPointRelief" ||
       klassenName == "relief:RasterRelief"    ||
       klassenName == "relief:BreaklineRelief" )
  {
    success = getIntegerAttributWert( "relief:lod", lod );
    if ( success )
    {
      errno_t err = _itoa_s ( lod, buffer, 3, 10 ); 
      if ( err == 0 )
        prefix = "lod" + string ( buffer );
    }
  }

  for ( iter = mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
  {
    geometryProperty = prefix + iter->first;
    if ( Features::isLodProperty ( geometryProperty, lodStufe ) )
      vGeometrie.push_back( iter->second );
  }

  return vGeometrie.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der unter einer Property referierten Geometrien       //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getGeometryPropertyAnz ( string geometryPropertyNameP )
{
  return mGeometrie.count( geometryPropertyNameP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Geometry-Objekte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getGeometryProperties ( std::vector<std::string> &vPropertyNames, std::vector<_Geometrie*> &vGeometrie )
{
  multimap<string,_Geometrie*>::iterator iter;

  for ( iter = mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
  {
    vPropertyNames.push_back( iter->first );
    vGeometrie.push_back( iter->second );
  }

  return vGeometrie.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht die unter propertyName referierte Geometrie                       //
///////////////////////////////////////////////////////////////////////////////
void Feature::deleteGeometryProperty( std::string propertyName )
{
  multimap<string,_Geometrie*>::iterator iter;

  for ( iter = mGeometrie.lower_bound ( propertyName ); iter != mGeometrie.upper_bound ( propertyName ); iter++ )
  {
    _Geometrie * pGeo = iter->second;
    pFeatures->removeGeometry( pGeo->getGmlId() );
    delete pGeo;
  }

  mGeometrie.erase( propertyName );
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt das Feature um einen Offset                                   //
///////////////////////////////////////////////////////////////////////////////
void Feature::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  std::multimap<std::string,_Geometrie*>::iterator iter;

  for ( iter = mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
  {
    _Geometrie * pGeo = iter->second;
    pGeo->offset( point, sGeometries );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert Polygon-Geometrie aus Shapefiles                               //
///////////////////////////////////////////////////////////////////////////////
void Feature::addShapePolygons2D ( std::string propertyName, std::vector<double> xCoordP, std::vector<double> yCoordP, 
                                      std::vector<int> ringOffsetsP, std::vector<int> ringTypenP, std::string srsName )
{
   string gmlId;

  //  Feststellen der Anzahl der Aussenkonturen
  int polygonAnz = 0;
  for ( unsigned int i = 0; i < ringTypenP.size(); i++ )
  {
    int ringTyp = ringTypenP[i];
    if ( ringTyp > 0 ) polygonAnz++;
  }
  if ( polygonAnz == 1)
  {
    PolygonGeo * position = new PolygonGeo( 2 );

    gmlId = Constant::createGmlId();
    position->setGmlId( gmlId );
    pFeatures->addGeometry( position );

    if ( srsName != "" ) position->setSrsName ( srsName );
    position->addShapePolygons2D ( xCoordP, yCoordP, ringOffsetsP, ringTypenP, pFeatures ); 

    addGeometryProperty( propertyName, position );
  }
  else
  {
    MultiSurface * position = new MultiSurface( 2 );

    gmlId = Constant::createGmlId();
    position->setGmlId( gmlId );
    pFeatures->addGeometry( position );

    if ( srsName != "" ) position->setSrsName ( srsName );
    position->addShapePolygons2D ( xCoordP, yCoordP, ringOffsetsP, ringTypenP, pFeatures ); 

    addGeometryProperty( propertyName, position );
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Generiert Linien-Geometrie aus Shapefiles                                //
///////////////////////////////////////////////////////////////////////////////
void Feature::addShapeLines2D ( std::string propertyName, std::vector<double> xCoordP, std::vector<double> yCoordP, 
                                   std::vector<int> partOffsetsP, std::string srsName )
{
  vector<double>        vX;
  vector<double>        vY;
  string                gmlId;
  CurveSegment        * pCurveSegment;

  if ( partOffsetsP.size() == 1 )
  {
    gmlId = Constant::createGmlId();
    Curve * position = new Curve ( 2 );
    position->setGmlId( gmlId );
    position->setSrsName ( srsName );
    pFeatures->addGeometry( position );

    for ( unsigned i = 0; i < xCoordP.size(); i++ )
    {
      vX.push_back ( xCoordP[i] );
      vY.push_back ( yCoordP[i] );
    }

    gmlId = Constant::createGmlId();
    pCurveSegment = new LineSegment ( 2, LineSegment::_LINE_STRING );
    pCurveSegment->setGmlId( gmlId );
    pCurveSegment->setSrsName ( srsName );
    pCurveSegment->addPoints2D ( vX, vY );
   pFeatures->addGeometry( pCurveSegment );

    position->addCurveSegment( pCurveSegment );

    addGeometryProperty( propertyName, position );
    return;
  }

  MultiCurve * position = new MultiCurve( 2 );

  gmlId = Constant::createGmlId();
  position->setGmlId( gmlId );
  pFeatures->addGeometry( position );

  if ( srsName != "" ) position->setSrsName ( srsName );
  position->addShapeLines2D ( xCoordP, yCoordP, partOffsetsP,  pFeatures );


  addGeometryProperty( propertyName, position );
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert Punkt-Geometrie aus Shapefiles                                 //
///////////////////////////////////////////////////////////////////////////////
void Feature::addShapePoints2D ( std::string propertyName, std::vector<double> xCoordP, std::vector<double> yCoordP, 
                               std::string srsName )
{
  string gmlId = Constant::createGmlId();

  if ( xCoordP.size() > 1 )
  {
    MultiPoint * position = new MultiPoint( 2 );

    position->setGmlId( gmlId );
    pFeatures->addGeometry( position );

    if ( srsName != "" ) position->setSrsName ( srsName );
    position->addShapePoints2D ( xCoordP, yCoordP, pFeatures );

    addGeometryProperty( propertyName, position );
  }
  else
  {
    GeoPoint * position = new GeoPoint( 2 );

    position->setGmlId( gmlId );
    pFeatures->addGeometry( position );

    if ( srsName != "" ) position->setSrsName ( srsName );
    position->set ( xCoordP[0], yCoordP[0] );

    addGeometryProperty( propertyName, position );
  }
}
///////////////////////////////////////////////////////////////////////////////
//  Liefert die über gmlProperty referierte flächenhafte Geometrie des       //
//  Features                                                                 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getFlaechenGeometrie ( string gmlProperty, vector<PolygonGeo*> &vPolygone )
{
  multimap<string,_Geometrie*>::iterator iter;
  string                                 gmlPropertyLoc;
  _Geometrie                           * pGeo;

  for ( iter =  mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
  {
    gmlPropertyLoc = iter->first;

    if ( gmlProperty != "" && gmlProperty != gmlPropertyLoc )
      continue;

    pGeo = iter->second;
    
    switch ( pGeo->getGeometryType() )
    {
    case MULTI_SURFACE:
    case COMPOSITE_SURFACE:
    case SHELL:
      {
        MultiSurface * pMultiSurface = dynamic_cast<MultiSurface*>(pGeo);
        pMultiSurface->getPolygone( vPolygone );
        break;
       }

    case  SURFACE:
      {
        Surface * pSurface = dynamic_cast<Surface*>(pGeo);
        pSurface->getPolygone( vPolygone );
        break;
      }

    case POLYGON:
      vPolygone.push_back( dynamic_cast<PolygonGeo*>(pGeo) );
      break;
    }

  }

  return vPolygone.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die über gmlProperty referierte linienhafte Geometrie des        //
//  Features                                                                 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getLinienGeometrie ( string gmlProperty, vector<_Curve*> &vCurves )
{
  multimap<string,_Geometrie*>::iterator iter;
  string                                 gmlPropertyLoc;
  _Geometrie                           * pGeo;

  for ( iter =  mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
  {
    gmlPropertyLoc = iter->first;

    if ( gmlProperty != "" && gmlProperty != gmlPropertyLoc )
      continue;

    pGeo = iter->second;

    switch ( pGeo->getGeometryType() )
    {
    case MULTI_CURVE:
      {
        MultiCurve * pMultiCurve = dynamic_cast<MultiCurve*>(pGeo);
        pMultiCurve->getCurves( vCurves );
        break;
      }

    case CURVE:
      vCurves.push_back( dynamic_cast<Curve*>(pGeo) );
      break;

    case LINE_STRING:
		case LINE_STRING_SEGMENT:
      vCurves.push_back( dynamic_cast<LineSegment*>(pGeo) );
      break;

    case ARC:
    case CIRCLE:
      vCurves.push_back( dynamic_cast<ArcSegment*>(pGeo) );
      break;
    }
  }

  return vCurves.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die über gmlProperty referierte punktförmige Geometrie des       //
//  Features                                                                 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getPunktGeometrie ( string gmlProperty, vector<GeoPoint*> &vPoints )
{
  multimap<string,_Geometrie*>::iterator iter;
  string                                 gmlPropertyLoc;
  _Geometrie                           * pGeo;

  for ( iter =  mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
  {
    gmlPropertyLoc = iter->first;

    if ( gmlProperty != "" && gmlProperty != gmlPropertyLoc )
      continue;

    pGeo = iter->second;

    switch ( pGeo->getGeometryType() )
    {
    case MULTI_POINT:
      {
        MultiPoint * pMultiPoint = dynamic_cast<MultiPoint*>(pGeo);
        pMultiPoint->getPoints( vPoints );
        break;
      }

    case GEO_POINT:
      vPoints.push_back( dynamic_cast<GeoPoint*>(pGeo) );
      break;
    }
  }

  return vPoints.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die über gmlProperty referierte volumenförmige Geometrie des     //
//  Features                                                                 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getVolumenGeometrie ( string gmlProperty, vector<_Solid*> &vSolids )
{
  multimap<string,_Geometrie*>::iterator iter;
  string                                 gmlPropertyLoc;
  _Geometrie                           * pGeo;

  for ( iter =  mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
  {
    gmlPropertyLoc = iter->first;

    if ( gmlProperty != "" && gmlProperty != gmlPropertyLoc )
      continue;

    pGeo = iter->second;

    switch ( pGeo->getGeometryType() )
    {
    case MULTI_SOLID:
    case COMPOSITE_SOLID:
      {
        MultiSolid * pMultiSolid = dynamic_cast<MultiSolid*>(pGeo);
        pMultiSolid->getSolids( vSolids );
        break;
      }

    case SOLID:
      vSolids.push_back( dynamic_cast<Solid*>(pGeo) );
      break;
    }
  }
  return vSolids.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle über gmlProperty referierte Geometriobjekte                 //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getGeometrie ( std::string gmlProperty, std::vector<_Geometrie*> &vGeometries )
{
  multimap<string,_Geometrie*>::iterator iter;
  string                                 gmlPropertyLoc;
  _Geometrie                           * pGeo;

  for ( iter =  mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
  {
    gmlPropertyLoc = iter->first;

    if ( gmlProperty != "" && gmlProperty != gmlPropertyLoc )
      continue;

    pGeo = iter->second;
    vGeometries.push_back( pGeo );
  }
  return vGeometries.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das Attribut pSimpleContent                                        //
///////////////////////////////////////////////////////////////////////////////
void Feature::setSimpleContent ( StringAttr * pSimpleContentP )
{
  if ( pSimpleContent != NULL ) delete pSimpleContent;

  pSimpleContent = pSimpleContentP;
  isSimple = true;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert die Feature-Geometrie in ein neues Koordinatensystem       //
///////////////////////////////////////////////////////////////////////////////
bool Feature::coordTransformation ( std::string srsSystemNew )
{
  if ( mGeometrie.size() == 0 )
    return true;

  SrsManager * pSrsManager =  pFeatures->getSrsManager();

  string srsSystemOld = getSrsName();

  if ( !pSrsManager->existCoordSystem( srsSystemOld ) )
    return false;

  return  coordTransformation( srsSystemOld, srsSystemNew );
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert die Feature-Geometrie von einem Koordinatensystem          //
//  in ein anderen                                                           //
///////////////////////////////////////////////////////////////////////////////
bool Feature::coordTransformation ( std::string srsSystemOld, std::string srsSystemNew )
{
  multimap<std::string,_Geometrie*>::iterator iter;

  SrsManager * pSrsManager =  pFeatures->getSrsManager();
  if ( !pSrsManager->existCoordSystem( srsSystemOld ) || 
       !pSrsManager->existCoordSystem( srsSystemNew ))
    return false;

  SrsProjection ^ pSrsProjection = gcnew SrsProjection ( pSrsManager );

  for ( iter = mGeometrie.begin(); iter != mGeometrie.end(); iter++ )
  {
    _Geometrie * pGeometrie = iter->second;
    bool success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pGeometrie );
    if ( !success )
      return false;
    else
      pGeometrie->setSrsName( srsSystemNew );
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert Geometrieobjekte von einem Koordinatensystem in ein anderes//
///////////////////////////////////////////////////////////////////////////////
bool Feature::coordTransformation ( string srsSystemOld, string srsSystemNew, SrsProjection ^ pSrsProjection,  _Geometrie * pGeometrie )
{
  bool success;

  GEOMETRY_TYPE geoType = pGeometrie->getGeometryType();

  switch (geoType)
  {
  case GEO_POINT:
    { 
      GeoPoint * pGeoPoint = dynamic_cast<GeoPoint*>(pGeometrie);
      success = pSrsProjection->convertSrs2Srs( srsSystemOld, srsSystemNew, pGeoPoint );
      if ( success && pGeoPoint->getSrsName() != "" )
        pGeoPoint->setSrsName( srsSystemNew );
      break;
    }

  case MULTI_GEOMETRY:
    {
      MultiGeometry * pMultiGeometry = dynamic_cast<MultiGeometry*>(pGeometrie);

      for ( unsigned int i = 0; i < pMultiGeometry->getGeometryMemberAnz(); i++ )
      {
        _Geometrie * pMember = pMultiGeometry->getGeometry( i );
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pMember );
        if ( !success )
          return false;

        if ( pMember->getSrsName() != "" )
          pMember->setSrsName( srsSystemNew );
      }
      break;
    }

  case GEOMETRIC_COMPLEX:
    {
      GeometricComplex * pMultiGeometry = dynamic_cast<GeometricComplex*>(pGeometrie);

      for ( unsigned int i = 0; i < pMultiGeometry->getGeometryMemberAnz(); i++ )
      {
        _Geometrie * pMember = pMultiGeometry->getGeometry( i );
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pMember );
        if ( !success )
          return false;

        if ( pMember->getSrsName() != "" )
          pMember->setSrsName( srsSystemNew );
      }
      break;
    }

  case MULTI_SURFACE:
  case COMPOSITE_SURFACE:
  case SHELL:
    {
      MultiSurface * pMultiSurface = dynamic_cast<MultiSurface*>(pGeometrie);

      for ( unsigned int i = 0; i < pMultiSurface->getSurfaceMemberAnz(); i++ ) 
      {
        _Surface * pMember =  pMultiSurface->getSurface ( i );
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pMember );
        if ( !success )
          return false;

        if ( pMember->getSrsName() != "" )
          pMember->setSrsName( srsSystemNew );
      }
      break;
    } 

  case MULTI_CURVE:
    {
      MultiCurve * pMultiCurve = dynamic_cast<MultiCurve*>(pGeometrie);

      for ( unsigned int i = 0; i < pMultiCurve->getCurveMemberAnz(); i++ ) 
      {
        _Curve * pMember =  pMultiCurve->getCurve ( i );
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pMember );
        if ( !success )
          return false;

        if ( pMember->getSrsName() != "" )
          pMember->setSrsName( srsSystemNew );
      }
      break;
    }

  case MULTI_POINT:
    {
      MultiPoint * pMultiPoint = dynamic_cast<MultiPoint*>(pGeometrie);

      for ( int i = 0; i < pMultiPoint->getPointAnz(); i++ ) 
      {
       GeoPoint * pMember =  pMultiPoint->getPoint ( i );
       success = pSrsProjection->convertSrs2Srs( srsSystemOld, srsSystemNew, pMember );
        if ( !success )
          return false;

        if ( pMember->getSrsName() != "" )
          pMember->setSrsName( srsSystemNew );
      }
      break;
    }

  case ORIENTABLE_SURFACE:
    {
      OrientableSurface * pOrientableSurface = dynamic_cast<OrientableSurface*>(pGeometrie);
      _Surface * pMember = pOrientableSurface->getBaseSurface();
      success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pMember );
      if ( !success )
        return false;

      if ( pMember->getSrsName() != "" )
        pMember->setSrsName( srsSystemNew );

      break;
    }

  case SURFACE:
  case TRIANGULATED_SURFACE:
    {
      Surface * pSurface = dynamic_cast<Surface*>(pGeometrie);

      for ( int i = 0; i < pSurface->getPolygonAnz(); i++ ) 
      {
        PolygonGeo * pMember =  pSurface->getPolygon ( i );
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pMember );
        if ( !success )
          return false;

        if ( pMember->getSrsName() != "" )
          pMember->setSrsName( srsSystemNew );
      }
      break;
    } 

  case POLYGON:
  case  TRIANGLE:
    {
      PolygonGeo * pPolygon = dynamic_cast<PolygonGeo*>(pGeometrie);

      _Ring * pAussenkontur = pPolygon->getAussenkontur();
      if ( pAussenkontur != NULL  )
      {
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pAussenkontur );
        if ( !success )
          return false;

        if ( pAussenkontur->getSrsName() != "" )
          pAussenkontur->setSrsName( srsSystemNew );

      }

      for ( unsigned  int i = 0; i < pPolygon->getLochAnz(); i++ )
      {
        _Ring * pLoch = pPolygon->getLoch( i );
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pLoch );
        if ( !success )
          return false;

        if ( pLoch->getSrsName() != "" )
          pLoch->setSrsName( srsSystemNew );
      }
      break;
    }

  case RING:
    {
      Ring * pRing = dynamic_cast<Ring*>(pGeometrie);

      for ( unsigned int i = 0; i < pRing->getRingMemberAnz(); i++ )
      {
        _Curve * pMember = pRing->getRingMember( i );
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pMember );
        if ( !success )
          return false;

        if ( pMember->getSrsName() != "" )
          pMember->setSrsName( srsSystemNew );
      }
      break;
    }

  case LINEAR_RING:
    {
      LinearRing * pLinearRing =  dynamic_cast<LinearRing*>(pGeometrie);

      LineSegment * pLineSegment = pLinearRing->getCurve();
      if ( pLineSegment != NULL )
      {
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pLineSegment );
        if ( !success )
          return false;

        if ( pLineSegment->getSrsName() != "" )
          pLineSegment->setSrsName( srsSystemNew );
      }
      break;
    }

  case CURVE:
    {
      Curve * pCurve =  dynamic_cast<Curve*>(pGeometrie);

      for ( int i = 0; i < pCurve->getCurveSegmentAnz(); i++ )
      {
        CurveSegment * pMember = pCurve->getCurveSegment( i );
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pMember );
        if ( !success )
          return false;

        if ( pMember->getSrsName() != "" )
          pMember->setSrsName( srsSystemNew );
      }
      break;
    }

  case LINE_STRING:
  case CIRCLE:
  case LINE_STRING_SEGMENT:
    {
      vector<GeoPoint> vPoints;
      LineSegment *  pLineSegment = dynamic_cast<LineSegment*>(pGeometrie);

      pLineSegment->getPoints( vPoints );

      for ( unsigned int i = 0; i < vPoints.size(); i++ )
      {
        GeoPoint p =  vPoints[i];
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, &p );
        if ( !success )
          return false;
        else
          pLineSegment->setPoint( i,  p );
      }

      break;
    }

  case ARC:
  {
    vector<GeoPoint> vPoints;
    ArcSegment *  pArcSegment = dynamic_cast<ArcSegment*>(pGeometrie);

    pArcSegment->getPoints(vPoints);

    for (unsigned int i = 0; i < vPoints.size(); i++)
    {
      GeoPoint p = vPoints[i];
      success = coordTransformation(srsSystemOld, srsSystemNew, pSrsProjection, &p);
      if (!success)
        return false;
      else
        pArcSegment->setPoint(i, p);
    }

    break;
  }


  case SOLID:
    {
      Solid * pSolid =  dynamic_cast<Solid*>(pGeometrie);

      _Surface *  pAussenkontur = pSolid->getAussenkontur();
      if ( pAussenkontur != NULL )
      {
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pAussenkontur );
        if ( !success )
          return false;

        if ( pAussenkontur->getSrsName() != "" )
          pAussenkontur->setSrsName( srsSystemNew );
      }

      for ( int i = 0; i < pSolid->getInnenkonturAnz(); i++ )
      {
        _Surface * pInnenkontur = pSolid->getInnenkontur( i );
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pInnenkontur );
        if ( !success )
          return false;

        if ( pInnenkontur->getSrsName() != "" )
          pInnenkontur->setSrsName( srsSystemNew );
      }
      break;
    }

  case MULTI_SOLID:
    {
      MultiSolid * pMultiSolid = dynamic_cast<MultiSolid*>(pGeometrie);

      for ( unsigned int i = 0; i < pMultiSolid->getSolidMemberAnz(); i++ )
      {
        _Solid * pMember = pMultiSolid->getSolid( i );
        success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pMember );
        if ( !success )
          return false;

        if ( pMember->getSrsName() != "" )
          pMember->setSrsName( srsSystemNew );
      }
      break;
    }

  case  ALLG_GEOMETRIE:
    {
      Geometrie * pAllgGeometrie = dynamic_cast<Geometrie*>(pGeometrie);
      
      _Geometrie * pMember = pAllgGeometrie->getGeometrie();     
      success =  coordTransformation ( srsSystemOld, srsSystemNew, pSrsProjection, pMember );
      if ( !success )
        return false;

      if ( pMember->getSrsName() != "" )
        pMember->setSrsName( srsSystemNew );

      break;
    }

  case IMPLICIT_GEOMETRY:
    {
      ImplicitGeometry *  pImplicitGeometry = dynamic_cast<ImplicitGeometry*>(pGeometrie);

      GeoPoint *  pGeoPoint = pImplicitGeometry->getReferencePoint();
      if (  pGeoPoint != NULL )
      {
        success = pSrsProjection->convertSrs2Srs( srsSystemOld, srsSystemNew, pGeoPoint );
        if ( !success )
          return false;

        if ( pGeoPoint->getSrsName() != "" )
          pGeoPoint->setSrsName( srsSystemNew );
      }
      break;
    }
  }

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Fügt ein TimeSeries Objekt hinzu                                         //
///////////////////////////////////////////////////////////////////////////////
bool Feature::addTimeSeries ( std::string propertyName, AbstractTimeSeries * pTimeSeries )
{
  
  mTimeSeries.insert ( multimap<string,AbstractTimeSeries*>::value_type ( propertyName, pTimeSeries ) );
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das erste unter propertyName referierte TimeSeries Objekt        //
///////////////////////////////////////////////////////////////////////////////
AbstractTimeSeries * Feature::getTimeSeries ( std::string propertyName )
{
  if ( mTimeSeries.lower_bound ( propertyName ) != mTimeSeries.upper_bound ( propertyName ) )
    return mTimeSeries.lower_bound( propertyName )->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle unter propertyName referierte TimeSeries Objekte            //
///////////////////////////////////////////////////////////////////////////////
size_t Feature::getAllTimeSeries ( std::string propertyName, std::vector<AbstractTimeSeries*> &vTimeSeries )
{
  multimap<string,AbstractTimeSeries*>::iterator iter;

  for ( iter = mTimeSeries.lower_bound ( propertyName ); iter != mTimeSeries.upper_bound ( propertyName ); iter++ )
    vTimeSeries.push_back( iter->second );

  return vTimeSeries.size();
}


///////////////////////////////////////////////////////////////////////////////
//  FeatureRelation											                              			 //
//  Relation eines BPlan-Objektes mit einem anderen Feature	      		    	 //
///////////////////////////////////////////////////////////////////////////////
FeatureRelation::FeatureRelation()
{
	relToFeature = true;
  nil          = false;
}

FeatureRelation::FeatureRelation ( FeatureRelation * pRelOld )
{
  rollenName    = pRelOld->rollenName;
  featureId     = pRelOld->featureId;
  uuid          = pRelOld->uuid;
  relToFeature  = pRelOld->relToFeature;
  nil           = pRelOld->nil;
}

FeatureRelation::~FeatureRelation()
{

}

///////////////////////////////////////////////////////////////////////////////
//  BLeitPlan                                                                //
//  Nur XPlanGML: Klasse für einen XPlanGML-Plan                        		 //
///////////////////////////////////////////////////////////////////////////////
BLeitPlan::BLeitPlan ( Features * pPlanObjekteP, string gmlIdP, string bPlanKlassenNameP )
:Feature ( BLEIT_PLAN, gmlIdP, pPlanObjekteP, "" )
{
	klassenName = bPlanKlassenNameP;

	if ( bPlanKlassenNameP == "xplan:BP_Plan" ) planTyp = BPLAN;
	else
	if ( bPlanKlassenNameP == "xplan:FP_Plan" ) planTyp = FPLAN;
	else
	if ( bPlanKlassenNameP == "xplan:RP_Plan" ) planTyp = RPLAN;
	else
	if ( bPlanKlassenNameP == "xplan:LP_Plan" ) planTyp = LPLAN;
  else
  if ( bPlanKlassenNameP == "xplan:SO_Plan" ) planTyp = SOPLAN;
}

BLeitPlan::~BLeitPlan()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle BLeitPlan-Bereiche							                    				 //
///////////////////////////////////////////////////////////////////////////////
size_t BLeitPlan::getPlanBereiche ( std::vector<PlanBereich*> & vPlanBereiche )
{
	size_t					      	 i, anz;
	vector<FeatureRelation*> bereiche;
	string				           objId;

  anz = getFeatureRelationen ( "xplan:bereich", bereiche );
	for ( i = 0; i < anz; i++ )
	{
		FeatureRelation * pRel = bereiche[i];
		objId = pRel->featureId;
		Feature * pObj = pFeatures->getFeature ( objId );
		if ( pObj != NULL && pObj->getObjektTyp() == PLAN_BEREICH ) 
			vPlanBereiche.push_back ( (PlanBereich*)pObj );
	}
	return vPlanBereiche.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen bestimmten BLeitPlan-Bereich	           									 //
///////////////////////////////////////////////////////////////////////////////
PlanBereich * BLeitPlan::getPlanBereich ( int bereichNummerP )
{
	vector<PlanBereich*> vPlanBereiche;
	size_t               i, anz, nummer;

	anz = getPlanBereiche ( vPlanBereiche );
	for ( i = 0; i < anz; i++ )
	{
		PlanBereich * pPlanBereich = vPlanBereiche[i];
		nummer = pPlanBereich->getBereichNummer();
		if ( nummer == bereichNummerP ) return pPlanBereich;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Objekt BLeitPlan-Bereich hinzu	                        						 //
///////////////////////////////////////////////////////////////////////////////
void BLeitPlan::addPlanBereich ( std::string planBereichIdP )
{
	Feature* pObj;
	int         nummer;

	pObj = pFeatures->getFeature ( planBereichIdP );
	if ( pObj != NULL )
	{
		nummer = ((PlanBereich*)pObj)->getBereichNummer();
		bereiche[nummer] =((PlanBereich*)pObj);

		FeatureRelation * pRel = new FeatureRelation;
		pRel->rollenName    = "xplan:bereich";
		pRel->featureId = planBereichIdP;

		addFeatureRelation ( pRel );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Feature-Relation hinzu			                   								 //
///////////////////////////////////////////////////////////////////////////////
void BLeitPlan::addFeatureRelation ( FeatureRelation * pRelationP )
{
	string       objId;
	string       schluessel;
	string       text;
	bool         success;

	Feature::addFeatureRelation ( pRelationP );

	if ( pFeatures->getGmlSchema()->getGmlTyp() >=  XPLANGML_3_0 )
		return;

	if ( pRelationP->rollenName == "xplan:texte" )
	{
		Feature * pTextlicheFestsetzung;

		objId = pRelationP->featureId;
		pTextlicheFestsetzung = pFeatures->getFeature ( objId );
		if ( pTextlicheFestsetzung == NULL ) return;
		success = pTextlicheFestsetzung->getStringAttributWert ( "xplan:schluessel", schluessel );
		if ( !success ) return;
		
		textlicheFestsetzungen[schluessel] = pTextlicheFestsetzung;
	}

	if ( pRelationP->rollenName == "xplan:begruendungsTexte" )
	{
		Feature * pTextlicheBegruendung;

		objId = pRelationP->featureId;
		pTextlicheBegruendung = pFeatures->getFeature ( objId );
		if ( pTextlicheBegruendung == NULL ) return;
		success = pTextlicheBegruendung->getStringAttributWert ( "xplan:schluessel", schluessel );
		if ( !success ) return;
		
		textlicheBegruendungen[schluessel] = pTextlicheBegruendung;
	}	
}

///////////////////////////////////////////////////////////////////////////////
//  Baut die maps für Textliche Festsetzungen und Textliche Begründungen auf //
///////////////////////////////////////////////////////////////////////////////
void BLeitPlan::createTextMaps ()
{
	string									    rollenName;
	string										objektId;
	string										schluessel;
	bool										success;
	FeatureRelation							  * pRelation;
	Feature								  * pPlanObjekt;
	multimap<string,FeatureRelation*>::iterator iter;

	for ( iter = objRelationen.begin(); iter != objRelationen.end(); iter++ )
	{
		if ( iter->first == "xplan:texte" ||
			 iter->first == "xplan:begruendungsTexte" )
		{
			pRelation = iter->second;
			objektId =  pRelation->featureId;
			pPlanObjekt = pFeatures->getFeature ( objektId );
			if ( pPlanObjekt != NULL )
			{
				success = pPlanObjekt->getStringAttributWert ( "xplan:schluessel", schluessel );
				if ( success )
				{
					if ( iter->first == "xplan:texte" )
						textlicheFestsetzungen[schluessel] = pPlanObjekt;
					else
					if ( iter->first == "xplan:begruendungsTexte" )
						textlicheBegruendungen[schluessel] = pPlanObjekt;
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Text einer Textlichen Festsetzung		              					 //
///////////////////////////////////////////////////////////////////////////////
Feature * BLeitPlan::getTextlicheFestsetzung ( std::string schluessel )
{
	map<string,Feature*>::iterator iter;

	iter = textlicheFestsetzungen.find ( schluessel );
	if ( iter == textlicheFestsetzungen.end() ) return NULL;
	else return iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Text einer Textlichen Begruendung		              					 //
///////////////////////////////////////////////////////////////////////////////
Feature * BLeitPlan::getTextlicheBegruendung ( std::string schluessel )
{
	map<string,Feature*>::iterator iter;

	iter = textlicheBegruendungen.find ( schluessel );
	if ( iter == textlicheBegruendungen.end() ) return NULL;
	else return iter->second;
}


///////////////////////////////////////////////////////////////////////////////
//  Entfernt Objekt								                            							 //
///////////////////////////////////////////////////////////////////////////////
void BLeitPlan::removeObjekt ( Feature * pObjP )
{
	deleteFeatureRelation ( pObjP );
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Textliche Festsetzungen			                  						 //
///////////////////////////////////////////////////////////////////////////////
size_t BLeitPlan::getTextlicheFestsetzungen ( std::vector<Feature*> &objekteP )
{
	size_t      						 i, anz;
	vector<FeatureRelation*> vRelationen;

	anz = getFeatureRelationen ( "xplan:texte", vRelationen );
	for  ( i = 0; i < anz; i++ )
	{
		FeatureRelation * pRelation = vRelationen[i];
		Feature * pBPlanObj = pFeatures->getFeature ( pRelation->featureId );
		if ( pBPlanObj != NULL ) objekteP.push_back ( pBPlanObj );
	}

	return objekteP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Verfahrens-Merkmale					                   						 //
///////////////////////////////////////////////////////////////////////////////
size_t BLeitPlan::getVerfahrensMerkmale ( std::vector<Feature*> &objekteP )
{
	size_t      						 i, anz;
	vector<FeatureRelation*> vRelationen;

	anz = getFeatureRelationen ( "xplan:verfahrensMerkmale", vRelationen );
	for  ( i = 0; i < anz; i++ )
	{
		FeatureRelation * pRelation = vRelationen[i];
		Feature * pBPlanObj = pFeatures->getFeature ( pRelation->featureId );
		if ( pBPlanObj != NULL ) objekteP.push_back ( pBPlanObj );
	}

	return objekteP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den srsNamen							                          						 //
///////////////////////////////////////////////////////////////////////////////
string BLeitPlan::getSrsName ()
{
  MultiSurface * pGeltungsbereich = dynamic_cast<MultiSurface*>(getGeometry ( "xplan:raeumlicherGeltungsbereich", 0 ) );
	if (  pGeltungsbereich != NULL )
		return pGeltungsbereich->getSrsName();
	else
		return "";
}


///////////////////////////////////////////////////////////////////////////////
//  PlanBereich                                                              //
//  Nur XPlanGML: Klasse für eine Ebene eines XPlanGML-Plans         				 //
///////////////////////////////////////////////////////////////////////////////
PlanBereich::PlanBereich ( Features * pPlanObjekteP, string gmlIdP, string bPlanKlassenNameP  )
:Feature ( PLAN_BEREICH, gmlIdP, pPlanObjekteP, "" )
{
	klassenName = bPlanKlassenNameP;
  pBLeitPlan                 = NULL;
}

PlanBereich::~PlanBereich()
{
	map<string,KlassenObjekte*>::iterator iter;

	for ( iter = bPlanKlassen.begin(); iter != bPlanKlassen.end(); iter++ )
		delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Relation für den referierten BPlan			              				 //
///////////////////////////////////////////////////////////////////////////////
void  PlanBereich::setBPlan (  BLeitPlan * pBLeitPlanP  )
{
	 pBLeitPlan = pBLeitPlanP;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert den zugehörigen BPlan				                      							 //
///////////////////////////////////////////////////////////////////////////////
BLeitPlan * PlanBereich::getBPlan ()
{
	size_t      						 anz;
	vector<FeatureRelation*> vRelationen;

  if ( pBLeitPlan != NULL )
    return pBLeitPlan;

	anz = getFeatureRelationen ( "xplan:gehoertZuPlan", vRelationen );
	if ( anz > 0 )
	{
		FeatureRelation * pRel = vRelationen[0];
		Feature * pObj = pFeatures->getFeature ( pRel->featureId );
		return (BLeitPlan*)pObj;
	}
	else return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Nummer des Bereichs			                    								 //
///////////////////////////////////////////////////////////////////////////////
int PlanBereich::getBereichNummer()
{
	int  nummer = 0;
	bool success;

  success = getIntegerAttributWert( "xplan:nummer", nummer );
	if ( success ) return nummer;
	else		   return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle BPlan-Objekte					                      							 //
///////////////////////////////////////////////////////////////////////////////
size_t PlanBereich::getBPlanObjekte ( std::vector<Feature*> & vplanObjekte, bool allObjectsP )
{
	string					        id;
	size_t        	        i, j, anz;
	Feature				        * pPlanObjekt;
	KlassenObjekte        * pKlassenObjekte;
	vector<KlassenObjekte*> vBPlanKlassen;
	vector<Feature*>        vBPlanObjekte;

	map<string,KlassenObjekte*>::iterator iter;

	for ( iter = bPlanKlassen.begin(); iter != bPlanKlassen.end(); iter++ )
	{
		pKlassenObjekte = iter->second;
		if ( pKlassenObjekte->getShowKlasse() || allObjectsP )
			vBPlanKlassen.push_back ( pKlassenObjekte );
	}

	for ( i = 0; i < vBPlanKlassen.size(); i++ )
	{
		vBPlanObjekte.clear();

		pKlassenObjekte = vBPlanKlassen[i];
		anz = pKlassenObjekte->getObjekte ( vBPlanObjekte );
		for ( j = 0; j < anz; j++ )
		{
			pPlanObjekt = vBPlanObjekte[j];
			if ( pPlanObjekt != NULL )	vplanObjekte.push_back ( pPlanObjekt );
		}
	}

	return vplanObjekte.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle benutzten Klassen von Plan-Objekten		            				 //
///////////////////////////////////////////////////////////////////////////////
size_t PlanBereich::getKlassen ( std::vector<KlassenObjekte*> &vKlassenP )
{
	map<string,KlassenObjekte*>::iterator iter;

	for ( iter = bPlanKlassen.begin(); iter != bPlanKlassen.end(); iter++ )
	{
		KlassenObjekte * pKlasse = iter->second;
		vKlassenP.push_back ( pKlasse );
	}

	return vKlassenP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein einzelnes Objekt								                      			 //
///////////////////////////////////////////////////////////////////////////////
KlassenObjekte * PlanBereich::getKlasse ( string & klassenName )
{
	map<string,KlassenObjekte*>::iterator iter;

	iter = bPlanKlassen.find ( klassenName );
	if ( iter != bPlanKlassen.end() ) return iter->second;
	else							  return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Objekt hinzu						                          							 //
///////////////////////////////////////////////////////////////////////////////
void PlanBereich::addObjekt (  Feature * pObjP )
{
	string				  klassenName;
	KlassenObjekte * pKlassenObjekte;

	map<string,KlassenObjekte*>::iterator iter;

	klassenName = pObjP->getKlassenName();

	iter = bPlanKlassen.find ( klassenName );
	if ( iter != bPlanKlassen.end() )
		pKlassenObjekte = iter->second;
	else
	{
		pKlassenObjekte = new KlassenObjekte ( pObjP );
		bPlanKlassen[klassenName] = pKlassenObjekte;
	}

	pKlassenObjekte->addObjekt ( pObjP );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt ein Objekt 									                          				 //
///////////////////////////////////////////////////////////////////////////////
void PlanBereich::removeObjekt ( Feature * pObjP )
{
	string				  objId;
	string				  klassenName;
	KlassenObjekte * pKlassenObjekte;

	map<string,KlassenObjekte*>::iterator iter;

	klassenName = pObjP->getKlassenName();

	iter = bPlanKlassen.find ( klassenName );
	if ( iter != bPlanKlassen.end() )
	{
		pKlassenObjekte = iter->second;
		pKlassenObjekte->removeObjekt ( pObjP );
	}

	deleteFeatureRelation ( pObjP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding-Box							                        					 //
///////////////////////////////////////////////////////////////////////////////
bool PlanBereich::getBoundingBox ( double * sw, double * no, std::string &srsNameP, int &srsDimensionP )
{
  _Geometrie * pGeltungsbereich;

  pGeltungsbereich = getGeometry( "xplan:geltungsbereich", 0 );
	if ( pGeltungsbereich != NULL ) 
  {
    srsNameP = pGeltungsbereich->getSrsName();
    srsDimensionP = 2;
		return pGeltungsbereich->getBoundingBox ( sw, no );
  }
	else
	{
    if ( getBPlan() == NULL ) return false;

		pGeltungsbereich = getBPlan()->getGeometry( "xplan:raeumlicherGeltungsbereich", 0 );
		if ( pGeltungsbereich != NULL )
    {
      srsNameP = pGeltungsbereich->getSrsName();
      srsDimensionP = 2;
			return pGeltungsbereich->getBoundingBox ( sw, no );
    }
		else
			return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den srsNamen			                          										 //
///////////////////////////////////////////////////////////////////////////////
string PlanBereich::getSrsName ()
{
  _Geometrie * pGeltungsbereich;

  pGeltungsbereich = getGeometry( "xplan:geltungsbereich", 0 );

  if (  pGeltungsbereich != NULL )
		return pGeltungsbereich->getSrsName();
	else
	{
    BLeitPlan * pPlan = getBPlan();
    if ( pPlan == NULL ) return "";

		pGeltungsbereich = pPlan->getGeometry( "xplan:raeumlicherGeltungsbereich", 0 );
		if ( pGeltungsbereich != NULL )
			return pGeltungsbereich->getSrsName ();
		else
			return "";
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Sortiert alle Objekte, die nur über eine Id referiert werden, in die map //
//  bPlanKlassen ein							                            							 //
///////////////////////////////////////////////////////////////////////////////
void PlanBereich::resolveObjektIds( BLeitPlan * pPlan )
{
	size_t    i, anz;
	string    id;
	vector<FeatureRelation*> relationen;

  setBPlan( pPlan );

  if ( pFeatures->getGmlSchema()->getGmlTyp() <  XPLANGML_4_0 )
    anz = getFeatureRelationen ( "xplan:fachobjekt", relationen );
  else
  if ( pFeatures->getGmlSchema()->getGmlTyp() <  XPLANGML_5_0 )
  {
    getFeatureRelationen ( "xplan:nachrichtlich", relationen );
   
    switch ( pPlan->getPlanTyp() )
    { 
    case BPLAN:
      anz = getFeatureRelationen ( "xplan:inhaltBPlan", relationen );
      break;

    case FPLAN:
      anz = getFeatureRelationen ( "xplan:inhaltFPlan", relationen );
      break;

    case RPLAN:
      anz = getFeatureRelationen ( "xplan:inhaltRPlan", relationen );
      break;

    case LPLAN:
      anz = getFeatureRelationen ( "xplan:inhaltLPlan", relationen );
      break;

    case SOPLAN:
      anz = getFeatureRelationen ( "xplan:inhaltSOPlan", relationen );
      break;
    }
  }
  else
  {
    anz = getFeatureRelationen ( "xplan:planinhalt", relationen );
  }
  
  for ( i = 0; i < anz; i++ )
  {
    FeatureRelation * pRel = relationen[i];
    id = pRel->featureId;
    Feature * pObj = pFeatures->getFeature ( id );
    if ( pObj != NULL )
    {
      addObjekt ( pObj );
      pObj->setBereichId( gmlId );
      pObj->propagateBereichId();
    }
  }

	relationen.clear();
	anz = getFeatureRelationen ( "xplan:praesentationsobjekt", relationen );
	for ( i = 0; i < anz; i++ )
	{
		FeatureRelation * pRel = relationen[i];
		id = pRel->featureId;
		Feature * pObj = pFeatures->getFeature ( id );
		if ( pObj != NULL ) addObjekt ( pObj );
	}

	relationen.clear();
	anz = getFeatureRelationen ( "xplan:rasterBasis", relationen );
	for ( i = 0; i < anz; i++ )
	{
		FeatureRelation * pRel = relationen[i];
		id = pRel->featureId;
		Feature * pObj = pFeatures->getFeature ( id );
		if ( pObj != NULL )
		{
			pObj->setBereichId ( gmlId );
			addObjekt ( pObj );
		}
	}

	relationen.clear();
	anz = getFeatureRelationen ( "xplan:rasterAenderung", relationen );
	for ( i = 0; i < anz; i++ )
	{
		FeatureRelation * pRel = relationen[i];
		id = pRel->featureId;
		Feature * pObj = pFeatures->getFeature ( id );
		if ( pObj != NULL )
		{
			pObj->setBereichId ( gmlId );
			addObjekt ( pObj );
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
//  KlassenObjekte                                                           //
//  Nur XPlanGML: Zusammenfassung von Features einer Klasse             		 //
///////////////////////////////////////////////////////////////////////////////
KlassenObjekte::KlassenObjekte ( Feature * pBPlanObjektP )
{
	bool flaechenschluss = true;

	klassenName = pBPlanObjektP->getKlassenName();
	showKlasse  = true;
	objektTyp = pBPlanObjektP->getObjektTyp();

	switch ( pBPlanObjektP->getObjektTyp() )
	{
	case XP_PUNKT_OBJEKT:  priority = 30; break;
	case XP_LINIEN_OBJEKT: priority = 20; break;
	case XP_FLAECHEN_OBJEKT:
		if ( klassenName == "RaeumlicherGeltungsbereich" )
			priority = 5;
		else
		{
			pBPlanObjektP->getBoolAttributWert ( "xplan:flaechenschluss", flaechenschluss );
			if ( flaechenschluss )
				priority = 7;
			else
				priority = 10;
		}
		break;
	case XP_PPO:		priority = 35; break;
	case XP_LPO:		priority = 25; break;
	case XP_FPO:		priority = 10; break;
	case XP_PTO:		priority = 36; break;
	case XP_LTO:		priority = 26; break;
	default:		priority = 50; break;
	};
}

KlassenObjekte::~KlassenObjekte ()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Objektes					                        							 //
///////////////////////////////////////////////////////////////////////////////
void KlassenObjekte::addObjekt ( Feature * pObjP )
{
	objekte.push_back ( pObjP );
}

///////////////////////////////////////////////////////////////////////////////
//  Löschen eines Objektes	                        												 //
///////////////////////////////////////////////////////////////////////////////
void KlassenObjekte::removeObjekt ( Feature * pObjP )
{
	unsigned int i, j;

	for ( i = 0; i < objekte.size(); i++ )
	{
		if ( objekte[i] == pObjP )
		{
			for ( j = i + 1; j < objekte.size(); j++ ) objekte[j-1] = objekte[j];
			objekte.pop_back();
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Objekte			                          										 //
///////////////////////////////////////////////////////////////////////////////
size_t KlassenObjekte::getObjekte ( vector<Feature*> & objekteP )
{
	unsigned int i;

	for ( i = 0; i < objekte.size(); i++ ) objekteP.push_back ( objekte[i] );

	return objekte.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Objekte									                    		 //
///////////////////////////////////////////////////////////////////////////////
size_t KlassenObjekte::getObjektAnz()
{
	return objekte.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Attr                                                                     //
//  Basisklasse für die Attributwerte eines Features                         //
///////////////////////////////////////////////////////////////////////////////
Attr::Attr()
{
  isNil = false;
}

Attr::Attr ( Attr * pAttrOld )
{
  map<string,string>::iterator iter;

  isNil = pAttrOld->isNil;
  for ( iter = pAttrOld->classifier.begin(); iter != pAttrOld->classifier.end(); iter++ )
  {
    classifier[iter->first] = iter->second;
  }
}

Attr::~Attr()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Wert des Classifiers nilReason                               //
///////////////////////////////////////////////////////////////////////////////
std::string Attr::getNilReason()
{
  if ( classifier.find( "nilReason") != classifier.end() )
    return classifier["nilReason"];
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Wert des Classifiers nilReason                                 //
///////////////////////////////////////////////////////////////////////////////
void Attr::setNilReason( std::string nilReason )
{
  classifier["nilReason"] = nilReason;
}

///////////////////////////////////////////////////////////////////////////////
//  StringAttr                                                               //
//  Textlicher Attribut-Wert eines Features                                  //
///////////////////////////////////////////////////////////////////////////////
StringAttr::StringAttr(): Attr()
{
}

StringAttr::StringAttr ( StringAttr * pAttrOld ) : Attr ( pAttrOld )
{
  value = pAttrOld->value;
}


StringAttr::~StringAttr()
{
}

///////////////////////////////////////////////////////////////////////////////
//  DoubleAttr                                                               //
//  Double Attribut-Wert eines Features                                      //
///////////////////////////////////////////////////////////////////////////////
DoubleAttr::DoubleAttr(): Attr()
{
  value = 0.0;
  uom    = NULL;
}

DoubleAttr::DoubleAttr ( DoubleAttr * pAttrOld ): Attr ( pAttrOld )
{
  value = pAttrOld->value;
  uom   = pAttrOld->uom;
}

DoubleAttr::~DoubleAttr()
{

}

///////////////////////////////////////////////////////////////////////////////
//  IntAttr                                                                  //
//  Integer Attribut-Wert eines Features                                     //
///////////////////////////////////////////////////////////////////////////////
IntAttr::IntAttr(): Attr()
{
  value = 0;
}

IntAttr::IntAttr ( IntAttr * pAttrOld ): Attr ( pAttrOld )
{
  value = pAttrOld->value;
}

IntAttr::~IntAttr()
{

}

///////////////////////////////////////////////////////////////////////////////
//  BoolAttr                                                                 //
//  Boolean Attribut-Wert eines Features                                     //
///////////////////////////////////////////////////////////////////////////////
BoolAttr::BoolAttr(): Attr()
{
  value = true;
}

BoolAttr::BoolAttr ( BoolAttr * pAttrOld ): Attr ( pAttrOld )
{
  value = pAttrOld->value;
}

BoolAttr::~BoolAttr()
{

}

///////////////////////////////////////////////////////////////////////////////
//  DateAttr                                                                 //
//  Datum / Uhrzeit Attribut-Wert eines Features                             //
///////////////////////////////////////////////////////////////////////////////
DateAttr::DateAttr(): Attr()
{
  value = NULL;
}

DateAttr::DateAttr ( DateAttr * pAttrOld ): Attr ( pAttrOld )
{
  if ( pAttrOld->value != NULL )
    value = AbstractTime::copy( pAttrOld->value );
  else
    value = NULL;
}


DateAttr::~DateAttr()
{
  if ( value != NULL )
    delete value;
}










