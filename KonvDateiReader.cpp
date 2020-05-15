#include "StdAfx.h"

#include <set>

#include "XPlanGML.h"
#include "Convert.h"
#include "ShpFileNamenKonv.h"
#include "GmlSchema.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "XPlanGMLVersionsDialog.h"
#include "TimeX.h"
#include "UnitOfMeasure.h"
#include "Logfile.h"

#include "KonvDateiReader.h"

///////////////////////////////////////////////////////////////////////////////
//  KonvDateiReader																													 //
//  Einlesen einer XML-Datei mit Konvertierungsregeln Shapefile -> XPlanGML  //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor und Destruktor										                      		 //
///////////////////////////////////////////////////////////////////////////////
KonvDateiReader::KonvDateiReader ( XPlanGML * pXPlanGML )
{
	m_pXPlanGML = pXPlanGML;
}

KonvDateiReader::~KonvDateiReader ()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Konvertierungsdatei                               					 //
///////////////////////////////////////////////////////////////////////////////
ShapeFileNamenKonvertierung * KonvDateiReader::read( std::string fileName )
{
	String ^ STR;
	String ^ NAME;
	String ^ WERT;
	string   stdString;
	string   name;
	string   wert;
  string   subAttributName;
  string   subAttributTyp;
  string   referenziertesElement;
	int	     iWert;
	double   dWert;
	bool     bWert;
  GML_SCHEMA_TYPE  xplanGmlTyp;
  bool             success;

  GmlSchema      * pGmlObjekte;
  Feature      * pPlanObjekt;
  GmlKlasse       * pGmlPlanKlasse = NULL;
  GmlAttribut     * pGmlAttribut;
  ATTRIBUT_TYP      attributTyp;

	System::String ^ fName = gcnew System::String( fileName.c_str() );
	validReader      = gcnew XmlTextReader(fName);

	int nElements	= 0;

	m_pKonv		   = new ShapeFileNamenKonvertierung ( m_pXPlanGML );
	m_pPlanAttribute = m_pXPlanGML->createPlanAttribute();

	while( validReader->Read() )
	{
    pActElement = validReader->LocalName;

		if ( validReader->NodeType == XmlNodeType::Element )
		{
			nElements++;
			if ( (nElements == 1) && (pActElement->CompareTo("ShpTransform") != 0))
			{
				MessageBox::Show( "Ungültige Konvertierungs-Datei");
				break;
			}

			if (pActElement->CompareTo("ShpTransform") == 0 )
			{
        STR = validReader->GetAttribute ( "gmlTyp" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          
          pGmlObjekte = new GmlSchema (  m_pXPlanGML->getFeatures() );
          success = pGmlObjekte->readGmlSchemaFromString( stdString, NULL );
          if ( success )
            m_pXPlanGML->getFeatures()->setGmlSchema( pGmlObjekte );
          else
          {
            delete pGmlObjekte;
            pGmlObjekte = NULL;
          }
        }
        else
        {
          xplanGmlTyp     =  XPLANGML_5_2;
          XPlanGMLVersionsDialog ^ pVersionsDialog = gcnew XPlanGMLVersionsDialog( xplanGmlTyp );
          pVersionsDialog->ShowDialog();

          pGmlObjekte = new GmlSchema (  m_pXPlanGML->getFeatures() );
          success = pGmlObjekte->readGmlSchema( pVersionsDialog->gmlTyp, NULL );
          if ( success )
            m_pXPlanGML->getFeatures()->setGmlSchema( pGmlObjekte );
          else
          {
            delete pGmlObjekte;
            pGmlObjekte = NULL;
          }
        }

				STR = validReader->GetAttribute ( "planTyp" );
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
          m_pXPlanGML->setPlanTyp ( XPlanGML::planStringToTyp( stdString ) );
          pGmlPlanKlasse = m_pXPlanGML->getGmlPlanKlasse();
				}


				STR = validReader->GetAttribute ( "planObjekteAusShapeFileErzeugen" );
				if ( STR != nullptr )
				{
					if ( STR->CompareTo("true") == 0 )
						m_pKonv->setPlaeneErzeugen ( true );
					else
						m_pKonv->setPlaeneErzeugen ( false );
				}

				STR = validReader->GetAttribute ( "srsName" );
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					m_pXPlanGML->getFeatures()->setSrsName ( stdString );
				}
			}
			else
			if (pActElement->CompareTo("StringAttributPlan") == 0 )
			{
				NAME = validReader->GetAttribute ( "name" );
				QuConvert::systemStr2stdStr( name, NAME );
        name = addDefaultNamespaceKuerzel ( name );

				WERT = validReader->GetAttribute ( "wert" );
				QuConvert::systemStr2stdStr( wert, WERT );

				m_pPlanAttribute->addStringAttribut ( name, wert );
			}
			else
			if (pActElement->CompareTo("DateAttributPlan") == 0 )
			{
				NAME = validReader->GetAttribute ( "name" );
				QuConvert::systemStr2stdStr( name, NAME );
        name = addDefaultNamespaceKuerzel ( name );

				WERT = validReader->GetAttribute ( "wert" );
				QuConvert::systemStr2stdStr( wert, WERT );

				m_pPlanAttribute->addDateAttribut ( name, wert );
			}
			else if (pActElement->CompareTo("IntegerAttributPlan") == 0 )
			{
				NAME = validReader->GetAttribute ( "name" );
				QuConvert::systemStr2stdStr( name, NAME );
        name = addDefaultNamespaceKuerzel ( name );

				WERT = validReader->GetAttribute ( "wert" );
				iWert = XmlConvert::ToInt32 ( WERT );

				m_pPlanAttribute->addIntegerAttribut ( name, iWert );
			}
			else if (pActElement->CompareTo("DoubleAttributPlan") == 0 )
			{
				NAME = validReader->GetAttribute ( "name" );
				QuConvert::systemStr2stdStr( name, NAME );
        name = addDefaultNamespaceKuerzel ( name );

				WERT = validReader->GetAttribute ( "wert" );
				dWert = XmlConvert::ToDouble ( WERT );

				m_pPlanAttribute->addDoubleAttribut ( name, dWert );
			}
			else if (pActElement->CompareTo("BoolAttributPlan") == 0 )
			{
				NAME = validReader->GetAttribute ( "name" );
				QuConvert::systemStr2stdStr( name, NAME );
        name = addDefaultNamespaceKuerzel ( name );

				WERT = validReader->GetAttribute ( "wert" );
				bWert = XmlConvert::ToBoolean ( WERT );

				m_pPlanAttribute->setBoolAttribut ( name, bWert );
			}
      else if (pActElement->CompareTo("ComplexAttributPlan") == 0 )
      {
				NAME = validReader->GetAttribute ( "name" );
				QuConvert::systemStr2stdStr( name, NAME );
        name = addDefaultNamespaceKuerzel ( name );

        pGmlAttribut = pGmlPlanKlasse->getGmlAttribut ( name );
        if ( pGmlAttribut !=NULL )
        {
          referenziertesElement = pGmlAttribut->getReferenziertesElement();
          pPlanObjekt = new Feature ( ANY_OBJEKT, "", m_pXPlanGML->getFeatures(), referenziertesElement );
        }
      }
      else if (pActElement->CompareTo("ComplexSubAttribut") == 0 )
      {
        NAME = validReader->GetAttribute ( "subAttributName" );
        if ( NAME != nullptr )
          QuConvert::systemStr2stdStr ( subAttributName, NAME );
        else
          subAttributName = "";
        subAttributName = addDefaultNamespaceKuerzel ( subAttributName );

        STR = validReader->GetAttribute ( "subAttributTyp" );
				if ( STR->CompareTo ( "DOUBLE") == 0 )
        {
          attributTyp = _DOUBLE;
          if ( referenziertesElement == "xplan:XP_GenerAttribut" && subAttributName == "xplan:wert" )
            pPlanObjekt->setKlassenName ( "xplan:XP_DoubleAttribut" );
        }
				else
				if ( STR->CompareTo ("INT") == 0 )
        {
          attributTyp = _INTEGER;
          if ( referenziertesElement == "xplan:XP_GenerAttribut" && subAttributName == "xplan:wert" )
            pPlanObjekt->setKlassenName ( "xplan:XP_IntegerAttribut" );
        }
				else
				if ( STR->CompareTo ("CHARACTER_STRING") == 0 )
        {
          attributTyp = _STRING;
          if ( referenziertesElement == "xplan:XP_GenerAttribut" && subAttributName == "xplan:wert" )
            pPlanObjekt->setKlassenName ( "xplan:XP_StringAttribut" );
        }
				else
				if ( STR->CompareTo ("DATE") == 0 )
        {
          attributTyp = _DATE;
          if ( referenziertesElement == "xplan:XP_GenerAttribut" && subAttributName == "xplan:wert" )
            pPlanObjekt->setKlassenName ( "xplan:XP_DatumAttribut" );
        }
				else
				if ( STR->CompareTo ("BOOL") == 0 )  attributTyp = _BOOLEAN;
				else
				if ( STR->CompareTo ("ENUMERATION") == 0 ) attributTyp = _ENUMERATION;
				else
				if ( STR->CompareTo ("EXTERNAL_CODE_LIST") == 0 ) attributTyp = _EXTERNAL_CODE_LIST;
				else
				if ( STR->CompareTo ("URL") == 0 )
        {
          attributTyp = _ANY_URI;
          if ( referenziertesElement == "xplan:XP_GenerAttribut" && subAttributName == "xplan:wert" )
            pPlanObjekt->setKlassenName ( "xplan:XP_URLAttribut" );
        }

        WERT = validReader->GetAttribute ( "subAttributWert" );
        switch ( attributTyp )
        {
        case _STRING:
        case _ENUMERATION:
        case _EXTERNAL_CODE_LIST:
          QuConvert::systemStr2stdStr ( wert, WERT );
          pPlanObjekt->setStringAttribut ( subAttributName, wert );
          break;

        case _INTEGER:
          iWert = XmlConvert::ToInt32 ( WERT );
          pPlanObjekt->setIntegerAttribut ( subAttributName, iWert );
          break;

        case _DOUBLE:
          dWert = XmlConvert::ToDouble ( WERT );
          pPlanObjekt->setDoubleAttribut ( subAttributName, dWert );
          break;

        case _BOOLEAN:
          bWert = XmlConvert::ToBoolean ( WERT );
          pPlanObjekt->setBoolAttribut ( subAttributName, bWert );
          break;

        case _DATE:
          {
            QuConvert::systemStr2stdStr ( wert, WERT );
            TimePosition * pTime = new TimePosition;
            pTime->fromXMLString( wert );
            DateAttr * pDateAttr = new DateAttr;
            pDateAttr->value = pTime;
            pPlanObjekt->setDateAttribut ( subAttributName, pDateAttr );
            break;
          }

        case _ANY_URI:
          QuConvert::systemStr2stdStr ( wert, WERT );
          pPlanObjekt->setURLAttribut ( subAttributName, wert );
          break;
        }
      }
			else
      if (pActElement->CompareTo("TextAbschnitt") == 0 )
			{
				readTextAbschnitt();
			}
			else if (pActElement->CompareTo("Bereich") == 0 )
			{
				readBereich();
			}
			else if (pActElement->CompareTo("ClassTransform") == 0 )
			{
				readClassTransform();
			}
			else if (pActElement->CompareTo("EnumerationTransform") == 0 )
			{
				readEnumerationTransform();
			}
		}
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      if ( pActElement->CompareTo("ComplexAttributPlan") == 0 )
      {
        m_pPlanAttribute->addKomplexAttribut ( name, pPlanObjekt );
        delete pPlanObjekt;
      }
    }
	}

	m_pXPlanGML->getLogfile()->writeLogFile ( "\n\n" );

	validReader->Close();

	return m_pKonv;
} 
///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines Text-Abschnittes				                    							 //
///////////////////////////////////////////////////////////////////////////////
void KonvDateiReader::readTextAbschnitt()
{
	String    ^ STR;
	string      schluessel;
	string      wert = "";
	string      wertURL = "";
	string      planTyp;

  STR = validReader->GetAttribute( "schluessel");
	QuConvert::systemStr2stdStr( schluessel, STR );

  STR = validReader->GetAttribute( "wert");
	if ( STR != nullptr ) QuConvert::systemStr2stdStr( wert, STR );

  STR = validReader->GetAttribute( "wertURL");
	if ( STR != nullptr ) QuConvert::systemStr2stdStr( wertURL, STR );

	STR = validReader->GetAttribute( "planTyp");
	if ( STR != nullptr ) QuConvert::systemStr2stdStr( planTyp, STR );

	m_pPlanAttribute->addTextAbschnitt ( schluessel, wert, wertURL, planTyp );
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Bereich		                            												 //
///////////////////////////////////////////////////////////////////////////////
void KonvDateiReader::readBereich()
{
	String^ STR;
	int      nummer             = 0;
	string   name               = "";
	string   bedeutung          = "";
	string   bedeutungEnum      = "";
	string   geometrieDatei     = "";
	int      massstab           = -1;

	RasterplanBasis     * pRasterplanBasis;
	RasterplanAenderung * pRasterplanAenderung;

	Bereich * pBereich = new Bereich ();

  STR = validReader->GetAttribute( "nummer");
	if ( STR != nullptr ) pBereich->nummer = XmlConvert::ToInt32 ( STR );

	STR = validReader->GetAttribute( "name");
	if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->name , STR );

	STR = validReader->GetAttribute( "bedeutung");
	if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->bedeutung , STR );

	STR = validReader->GetAttribute( "bedeutungEnum");
	if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->bedeutungEnum , STR );

	STR = validReader->GetAttribute( "geometrieDatei");
	if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->geometrieDatei , STR );

	STR = validReader->GetAttribute( "versionBauNVO");
	if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->versionBauNVO , STR );

  STR = validReader->GetAttribute( "versionBauNVOText");
  if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->versionBauNVOText , STR );


  STR = validReader->GetAttribute( "versionBauGBDatum");
  if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->versionBauGBDatum , STR );

  STR = validReader->GetAttribute( "versionBauGBText");
  if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->versionBauGBText , STR );

  STR = validReader->GetAttribute( "versionBROGDatum");
  if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->versionBROGDatum , STR );

  STR = validReader->GetAttribute( "versionBROGText");
  if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->versionBROGText , STR );

  STR = validReader->GetAttribute( "versionLPLGDatum");
  if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->versionLPLGDatum , STR );

  STR = validReader->GetAttribute( "versionLPLGText");
  if ( STR != nullptr ) QuConvert::systemStr2stdStr ( pBereich->versionLPLGText , STR );

	STR = validReader->GetAttribute( "massstab");
	if ( STR != nullptr ) pBereich->massstab = XmlConvert::ToInt32  ( STR );

	if ( !validReader->IsEmptyElement )
	{
		while( validReader->Read() )
		{
			pActElement = validReader->LocalName;

			if ( validReader->NodeType == XmlNodeType::Element )
			{

				if (pActElement->CompareTo("rasterplanBasis") == 0 )
				{
					pRasterplanBasis = readRasterplanBasis ();
					if ( pRasterplanBasis != NULL )
						pBereich->setRasterplanBasis ( pRasterplanBasis );
				}
				else
				if (pActElement->CompareTo("rasterplanAenderung") == 0 )
				{
					pRasterplanAenderung = readRasterplanAenderung ();
					if ( pRasterplanAenderung != NULL )
						pBereich->addRasterplanAenderung ( pRasterplanAenderung );
				}
			}

			if ( validReader->NodeType == XmlNodeType::EndElement )
			{
				if ( pActElement->CompareTo ( "Bereich" ) == 0 )
					break;
			}
		}
	}

	m_pPlanAttribute->addBereich ( pBereich );
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen RasterplanBasis                                                 //
///////////////////////////////////////////////////////////////////////////////
RasterplanBasis * KonvDateiReader::readRasterplanBasis ()
{
	RasterplanBasis * pRasterplanBasis = new RasterplanBasis;
  ExterneReferenz * pExterneReferenz;

	while( validReader->Read() )
	{
		pActElement = validReader->LocalName;

		if ( validReader->NodeType == XmlNodeType::Element )
		{
      if ( pActElement->CompareTo( "externeReferenz" ) == 0 )
      {
        pExterneReferenz = readExterneReferenz();
        if ( pExterneReferenz != NULL )
            pRasterplanBasis->addExterneReferenz( pExterneReferenz );
      }
		}

		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			if (pActElement->CompareTo("rasterplanBasis") == 0 )
				break;
		}
	}

	return pRasterplanBasis;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen RasterplanAenderung                                             //
///////////////////////////////////////////////////////////////////////////////
RasterplanAenderung * KonvDateiReader::readRasterplanAenderung ()
{

	while( validReader->Read() )
	{
		pActElement = validReader->LocalName;



		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			if (pActElement->CompareTo("rasterplanAenderung") == 0 )
				break;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//   Einlesen ExterneReferenz                                                //
///////////////////////////////////////////////////////////////////////////////
ExterneReferenz * KonvDateiReader::readExterneReferenz()
{
  ExterneReferenz * pExterneReferenz = new ExterneReferenz;
  String          ^ STR;

  STR = validReader->GetAttribute ( "relationName" );
  if ( STR != nullptr )  QuConvert::systemStr2stdStr( pExterneReferenz->relationName, STR );

  STR = validReader->GetAttribute ( "informationssystemURL" );
  if ( STR != nullptr )  QuConvert::systemStr2stdStr( pExterneReferenz->informationssystemURL, STR );

  STR = validReader->GetAttribute ( "referenzName" );
  if ( STR != nullptr )  QuConvert::systemStr2stdStr( pExterneReferenz->referenzName, STR );

  STR = validReader->GetAttribute ( "referenzURL" );
  if ( STR != nullptr )  QuConvert::systemStr2stdStr( pExterneReferenz->referenzURL, STR );

  STR = validReader->GetAttribute ( "referenzMimeType" );
  if ( STR != nullptr )  QuConvert::systemStr2stdStr( pExterneReferenz->referenzMimeType, STR );

  STR = validReader->GetAttribute ( "georefzURL" );
  if ( STR != nullptr )  QuConvert::systemStr2stdStr( pExterneReferenz->georefURL, STR );

  STR = validReader->GetAttribute ( "georefMimeType" );
  if ( STR != nullptr )  QuConvert::systemStr2stdStr( pExterneReferenz->georefMimeType, STR );

  STR = validReader->GetAttribute ( "beschreibung" );
  if ( STR != nullptr )  QuConvert::systemStr2stdStr( pExterneReferenz->beschreibung, STR );

  STR = validReader->GetAttribute ( "datum" );
  if ( STR != nullptr )  QuConvert::systemStr2stdStr( pExterneReferenz->datum, STR );

  return pExterneReferenz;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen ClassTransform					                        								 //
///////////////////////////////////////////////////////////////////////////////
void KonvDateiReader::readClassTransform()
{
	String ^ STR;
  String ^ NAME;
  String ^ WERT;

  string   wert;
  int	     iWert;
  double   dWert;
  bool     bWert;

	string	standardName;
	string	actualName;
	int     index                  = 0;
	string	enumerationId          = "";
	string  generAttributName      = "";
	string	defaultWert            = "";
	string  subAttributName        = "";
  string  subAttributNameLoc     = "";
  string  rechtlicheBedeutung    = "";
	int		  bereichNummer          = 0;
  
  GmlSchema   * pGmlObjekte = m_pXPlanGML->getFeatures()->getGmlSchema();
  GmlKlasse   * pGmlKlasse;
  GmlAttribut * pGmlAttribut;
  Feature     * pPlanObjekt;
  GmlElement  * pGmlElement;
  string        referenziertesElement;


	ATTRIBUT_TYP attributTyp;
  ATTRIBUT_TYP attributTypLoc;
	ATTRIBUT_TYP subAttributTyp;

	STR = validReader->GetAttribute( "standardName");
	QuConvert::systemStr2stdStr( standardName, STR );
  standardName = addDefaultNamespaceKuerzel( standardName );
  pGmlElement = pGmlObjekte->getGmlElement( standardName );
  if ( pGmlElement != NULL )
    pGmlKlasse = pGmlObjekte->getGmlKlasse( pGmlElement->getGmlTypeName() );
  else
    pGmlKlasse = NULL;

	STR = validReader->GetAttribute( "actualName");
	QuConvert::systemStr2stdStr( actualName, STR );

	STR = validReader->GetAttribute( "bereichNummer");
	bereichNummer = XmlConvert::ToInt32 ( STR );

	STR = validReader->GetAttribute( "rechtlicheBedeutung");
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( rechtlicheBedeutung, STR );
	
	KlassenName * pKlName = m_pKonv->addKlassenName (  actualName, standardName, bereichNummer, rechtlicheBedeutung );
	
	if ( validReader->IsEmptyElement ) return;

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo ( "AttributeTransform") == 0 )
			{
				STR = validReader->GetAttribute( "standardName");
				QuConvert::systemStr2stdStr( standardName, STR );
        standardName = addDefaultNamespaceKuerzel( standardName );

        if ( pGmlKlasse != NULL )
          pGmlAttribut = pGmlKlasse->getGmlAttribut( standardName );
        else
          pGmlAttribut = NULL;

				STR = validReader->GetAttribute( "actualName");
				if ( STR != nullptr )
					QuConvert::systemStr2stdStr( actualName, STR );
				else
					actualName = "";	

				STR = validReader->GetAttribute("index");
				if (STR != nullptr)
					index = XmlConvert::ToInt32(STR);
				else
					index = 0;

				STR = validReader->GetAttribute( "type");
				if ( STR->CompareTo ( "DOUBLE") == 0 ) attributTyp = _DOUBLE;
				else
				if ( STR->CompareTo ("INT") == 0 ) attributTyp = _INTEGER;
				else
				if ( STR->CompareTo ("CHARACTER_STRING") == 0 ) attributTyp = _STRING;
				else
				if ( STR->CompareTo ("DATE") == 0 ) attributTyp = _DATE;
				else
				if ( STR->CompareTo ("BOOL") == 0 ) attributTyp = _BOOLEAN;
				else
				if ( STR->CompareTo ("ENUMERATION") == 0 ) attributTyp = _ENUMERATION;
				else
				if ( STR->CompareTo ("EXTERNAL_CODE_LIST") == 0 ) attributTyp = _EXTERNAL_CODE_LIST;
				else
				if ( STR->CompareTo ("ANGLE") == 0 ) attributTyp = _ANGLE;
				else
				if ( STR->CompareTo ("LENGTH") == 0 ) attributTyp = _LENGTH;
				else
				if ( STR->CompareTo ("AREA") == 0 ) attributTyp = _AREA;
				else
				if ( STR->CompareTo ("VOLUME") == 0 ) attributTyp = _VOLUME;
				else
				if ( STR->CompareTo ("URL") == 0 ) attributTyp = _ANY_URI;
				else
				if ( STR->CompareTo ("REFERENZ") == 0 ) attributTyp = _REFERENZ;
				else
				if ( STR->CompareTo ("DATA_TYPE") == 0 )
          attributTyp = _DATA_TYPE;
				else
				if ( STR->CompareTo ("POS_X") == 0 ) attributTyp = _POS_X;
				else
				if ( STR->CompareTo ("POS_Y") == 0 ) attributTyp = _POS_Y;
				else
				if ( STR->CompareTo ("POS_XY") == 0 ) attributTyp = _POS_XY;

				STR = validReader->GetAttribute( "enumerationId");
				if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr( enumerationId, STR );
          enumerationId = addDefaultNamespaceKuerzel( enumerationId );
        }
				else enumerationId = "";

				STR = validReader->GetAttribute( "default");
				if ( STR != nullptr )	QuConvert::systemStr2stdStr( defaultWert, STR );
				else defaultWert = "";

				STR = validReader->GetAttribute( "generAttributName");
				if ( STR != nullptr )	QuConvert::systemStr2stdStr( generAttributName, STR );
				else generAttributName = "";

				STR = validReader->GetAttribute( "subAttributName");
				if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr( subAttributName, STR );
          subAttributName = addDefaultNamespaceKuerzel( subAttributName );
        }
				else subAttributName = "";

				subAttributTyp = _SONST;
				STR = validReader->GetAttribute( "subAttributTyp");
        if ( STR == nullptr ) subAttributTyp = _SONST;
        else
				if ( STR->CompareTo ( "DOUBLE") == 0 ) subAttributTyp = _DOUBLE;
				else
				if ( STR->CompareTo ("INT") == 0 ) subAttributTyp = _INTEGER;
				else
				if ( STR->CompareTo ("CHARACTER_STRING") == 0 ) subAttributTyp = _STRING;
				else
				if ( STR->CompareTo ("DATE") == 0 ) subAttributTyp = _DATE;
				else
				if ( STR->CompareTo ("BOOL") == 0 ) subAttributTyp = _BOOLEAN;
				else
				if ( STR->CompareTo ("ENUMERATION") == 0 ) subAttributTyp = _ENUMERATION;
				else
				if ( STR->CompareTo ("EXTERNAL_CODE_LIST") == 0 ) subAttributTyp = _EXTERNAL_CODE_LIST;
				else
				if ( STR->CompareTo ("ANGLE") == 0 ) subAttributTyp = _ANGLE;
				else
				if ( STR->CompareTo ("LENGTH") == 0 ) subAttributTyp = _LENGTH;
				else
				if ( STR->CompareTo ("AREA") == 0 ) subAttributTyp = _AREA;
				else
				if ( STR->CompareTo ("VOLUME") == 0 ) subAttributTyp = _VOLUME;
				else
				if ( STR->CompareTo ("URL") == 0 ) subAttributTyp = _ANY_URI;
				else
				if ( STR->CompareTo ("REFERENZ") == 0 ) subAttributTyp = _REFERENZ;
				else
				if ( STR->CompareTo ("DATA_TYPE") == 0 ) subAttributTyp = _DATA_TYPE;
				else
				if ( STR->CompareTo ("POS_X") == 0 ) subAttributTyp = _POS_X;
				else
				if ( STR->CompareTo ("POS_Y") == 0 ) subAttributTyp = _POS_Y;
				else
				if ( STR->CompareTo ("POS_XY") == 0 ) subAttributTyp = _POS_XY;
				else
				subAttributTyp = _SONST;	

        pPlanObjekt = NULL;
        if ( !validReader->IsEmptyElement )
        {
          if ( pGmlAttribut != NULL )
            referenziertesElement = pGmlAttribut->getReferenziertesElement();
          else
            referenziertesElement = "";

          while( validReader->Read() )
          {
            if ( validReader->NodeType == XmlNodeType::Element )
            {
              pActElement = validReader->LocalName;
              if ( pActElement->CompareTo ( "ComplexSubAttribut") == 0 )
              {
                if ( pPlanObjekt == NULL )
                {
                  string gmlId = Constant::createGmlId();
                  pPlanObjekt = new Feature ( ANY_OBJEKT, gmlId, m_pXPlanGML->getFeatures(), referenziertesElement );
                }

                NAME = validReader->GetAttribute ( "subAttributName" );
                if ( NAME != nullptr )
                {
                  QuConvert::systemStr2stdStr ( subAttributNameLoc, NAME );
                  subAttributNameLoc = addDefaultNamespaceKuerzel( subAttributNameLoc );
                }
                else
                  subAttributNameLoc = "";

                STR = validReader->GetAttribute ( "subAttributTyp" );
                if ( STR->CompareTo ( "DOUBLE") == 0 )
                {
                  attributTypLoc = _DOUBLE;
                  if ( referenziertesElement == "xplan:XP_GenerAttribut" && subAttributName == "xplan:wert" )
                    pPlanObjekt->setKlassenName ( "xplan:XP_DoubleAttribut" );
                }
                else
                if ( STR->CompareTo ("INT") == 0 )
                {
                  attributTypLoc = _INTEGER;
                  if ( referenziertesElement == "xplan:XP_GenerAttribut" && subAttributName == "xplan:wert" )
                    pPlanObjekt->setKlassenName ( "xplan:XP_IntegerAttribut" );
                }
                else
                if ( STR->CompareTo ("CHARACTER_STRING") == 0 )
                {
                  attributTypLoc = _STRING;
                  if ( referenziertesElement == "xplan:XP_GenerAttribut" && subAttributName == "xplan:wert" )
                    pPlanObjekt->setKlassenName ( "xplan:XP_StringAttribut" );
                }
                else
                if ( STR->CompareTo ("DATE") == 0 )
                {
                  attributTypLoc = _DATE;
                  if ( referenziertesElement == "xplan:XP_GenerAttribut" && subAttributName == "xplan:wert" )
                    pPlanObjekt->setKlassenName ( "xplan:XP_DatumAttribut" );
                }
                else
                if ( STR->CompareTo ("BOOL") == 0 )  attributTypLoc = _BOOLEAN;
                else
                if ( STR->CompareTo ("ENUMERATION") == 0 ) attributTypLoc = _ENUMERATION;
                else
                if ( STR->CompareTo ("EXTERNAL_CODE_LIST") == 0 ) attributTypLoc = _EXTERNAL_CODE_LIST;
                else
                if ( STR->CompareTo ("URL") == 0 )
                {
                  attributTypLoc = _ANY_URI;
                  if ( referenziertesElement == "xplan:XP_GenerAttribut" && subAttributName == "xplan:wert" )
                    pPlanObjekt->setKlassenName ( "xplan:XP_URLAttribut" );
                }
                if ( STR->CompareTo ( "LENGTH") == 0  ) attributTypLoc = _LENGTH;
                else
                if ( STR->CompareTo ( "AREA") == 0  ) attributTypLoc = _AREA;
                else
                if ( STR->CompareTo ( "VOLUME") == 0  ) attributTypLoc = _VOLUME;
                else
                if ( STR->CompareTo ( "ANGLE") == 0  ) attributTypLoc = _ANGLE;

                WERT = validReader->GetAttribute ( "subAttributWert" );
                switch ( attributTypLoc )
                {
                case _STRING:
                case _ENUMERATION:
                case _EXTERNAL_CODE_LIST:
                   QuConvert::systemStr2stdStr ( wert, WERT );
                   pPlanObjekt->setStringAttribut ( subAttributNameLoc, wert );
                   break;

                case _INTEGER:
                   iWert = XmlConvert::ToInt32 ( WERT );
                   pPlanObjekt->setIntegerAttribut ( subAttributNameLoc, iWert );
                   break;

                 case _DOUBLE:
                   dWert = XmlConvert::ToDouble ( WERT );
                   pPlanObjekt->setDoubleAttribut ( subAttributNameLoc, dWert );
                   break;

                 case _LENGTH:
                   dWert = XmlConvert::ToDouble ( WERT );
                   pPlanObjekt->setDoubleAttribut ( subAttributNameLoc, dWert, m_pXPlanGML->getUOMList()->getUomLength() );
                   break;

                 case _AREA:
                   dWert = XmlConvert::ToDouble ( WERT );
                   pPlanObjekt->setDoubleAttribut ( subAttributNameLoc, dWert,  m_pXPlanGML->getUOMList()->getUomArea() );
                   break;

                 case _VOLUME:
                   dWert = XmlConvert::ToDouble ( WERT );
                   pPlanObjekt->setDoubleAttribut ( subAttributNameLoc, dWert, m_pXPlanGML->getUOMList()->getUomVolume() );
                   break;

                 case _ANGLE:
                   dWert = XmlConvert::ToDouble ( WERT );
                   pPlanObjekt->setDoubleAttribut ( subAttributNameLoc, dWert, m_pXPlanGML->getUOMList()->getUomAngle() );
                   break;

                 case _BOOLEAN:
                   bWert = XmlConvert::ToBoolean ( WERT );
                   pPlanObjekt->setBoolAttribut ( subAttributNameLoc, bWert );
                   break;

                 case _DATE:
                   {
                    QuConvert::systemStr2stdStr ( wert, WERT );
                    TimePosition * pTime = new TimePosition;
                    pTime->fromXMLString( wert );
                    DateAttr * pDateAttr = new DateAttr;
                    pDateAttr->value = pTime;
                    pPlanObjekt->setDateAttribut ( subAttributNameLoc, pDateAttr );
                    break;
                   }

                 case _ANY_URI:
                   QuConvert::systemStr2stdStr ( wert, WERT );
                   pPlanObjekt->setURLAttribut ( subAttributNameLoc, wert );
                   break;
                 }
              }

            }
            else if ( validReader->NodeType == XmlNodeType::EndElement )
            {
              pActElement = validReader->LocalName;
              if (pActElement->CompareTo("AttributeTransform") == 0 ) break;
            }      
          
          }		
        }
				pKlName->addAttributName ( actualName, standardName, index, attributTyp,
										   generAttributName, 
										   enumerationId, defaultWert, subAttributName, subAttributTyp, pPlanObjekt );
			}
		}
		else if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if (pActElement->CompareTo("ClassTransform") == 0 ) break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen EnumerationTransform                                            //
///////////////////////////////////////////////////////////////////////////////
void KonvDateiReader::readEnumerationTransform()
{
	System::String^ STR;

	std::string	  standardName;
	std::string	  actualName;
	std::string	  enumerationId;
  std::string   dictionaryName;

	STR = validReader->GetAttribute( "enumerationId");
	QuConvert::systemStr2stdStr( enumerationId, STR );
  enumerationId = addDefaultNamespaceKuerzel( enumerationId );

  Enumeration * pEnumerattion = m_pKonv->addEnumeration ( enumerationId );

  STR = validReader->GetAttribute( "codeSpace");
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr( dictionaryName, STR );
    pEnumerattion->setDictionary( dictionaryName );
  }

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo ( "EnumerationItem" ) == 0 )
			{
				STR = validReader->GetAttribute( "standardName");
				QuConvert::systemStr2stdStr( standardName, STR );

				STR = validReader->GetAttribute( "actualName");
				QuConvert::systemStr2stdStr( actualName, STR );

				pEnumerattion->addEnumerationItem ( actualName, standardName );
			}
		}
		else if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if (pActElement->CompareTo("EnumerationTransform") == 0 ) break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt vor GML Klassen- bzw. Attributname das Default Namespace-Kürzel ein //
///////////////////////////////////////////////////////////////////////////////
string KonvDateiReader::addDefaultNamespaceKuerzel ( string gmlName )
{
	size_t index;
  string gmlNameAct;

  index = gmlName.find ( ":" );
  if ( index != string::npos )
    return gmlName;

  gmlNameAct = "xplan:" + gmlName;
  return gmlNameAct;
}

///////////////////////////////////////////////////////////////////////////////
//  __gc class KonvDateiWriter												                       //
//  Erzeugung einer XML-Repräsentation der Konvertierungsregeln							 //
///////////////////////////////////////////////////////////////////////////////
KonvDateiWriter::KonvDateiWriter(XPlanGML * pXPlanGML )
{
  m_pXPlanGML = pXPlanGML;
	m_pKonv			= pXPlanGML->getKonv();
	writer      = nullptr;
}

KonvDateiWriter::~KonvDateiWriter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben der Konvertierungs-Datei		                              		 //
///////////////////////////////////////////////////////////////////////////////
void KonvDateiWriter::write ( std::string fileNameP )
{
	size_t         i, j, anz, subAttributAnz;
	int            iWert;
	double		     dWert;
	bool		       bWert;
	string         stdString;
	string         srsName;
  GML_SCHEMA_TYPE        gmlTyp;

	String		     ^ fileName = QuConvert::ToString ( fileNameP );
	String         ^ STR;
	PlanAttribute  * pPlanAttribute;
  Feature        * pPlanObjekt;

	vector<string>			       attributNamen;
	vector<string>			       subAttributNamen;
	vector<string>			       stringAttributWerte;
	vector<int>				         intAttributWerte;
	vector<double>			       doubleAttributWerte;
	vector<bool>			         boolAttributWerte;
	vector<TextAbschnitt*>     textAbschnitte;
	vector<Bereich*>           bereiche;
	vector<KlassenName*>       klassenNamen;
	vector<Enumeration*>       enumerationen;
	vector<Feature*>           komplexAttribute;
  vector<ATTRIBUT_TYP>       attributTypen;
  vector<UOM*>               vUOMs;

	System::Text::UTF8Encoding ^  utf8	= gcnew UTF8Encoding();
	writer							              	= gcnew XmlTextWriter( fileName, utf8 );

	writer->Formatting = Formatting::Indented;
	writer->WriteStartDocument(true);
	writer->WriteComment("Generated by: KIT Shapefile-GML Converter");

  gmlTyp = m_pXPlanGML->getFeatures()->getGmlTyp();

  writer->WriteStartElement("ShpTransform");

	srsName = m_pXPlanGML->getFeatures()->getSrsName();
	if ( srsName != "" )
	{
		writer->WriteAttributeString( "srsName", QuConvert::ToString ( srsName ) );
	}

  XPLAN_TYP planTyp = m_pXPlanGML->getPlanTyp();
  writer->WriteAttributeString( "planTyp", QuConvert::ToString( XPlanGML::planTypToString( planTyp) ) );

  stdString = Constant::getGMLSchemaTypeAsString( gmlTyp );
  STR = QuConvert::ToString( stdString );
  writer->WriteAttributeString( "gmlTyp", STR );

 	writer->WriteAttributeString("xmlns:xsi",	"http://www.w3.org/2001/XMLSchema-instance");

	bWert = m_pKonv->getPlaeneErzeugen();
	if ( bWert == true )
		writer->WriteAttributeString( "planObjekteAusShapeFileErzeugen", "true" );

	pPlanAttribute = m_pXPlanGML->getPlanAttribute();

	if ( pPlanAttribute!= NULL )
	{
		anz = pPlanAttribute->getStringAttributWerte ( attributNamen, stringAttributWerte );
		for ( i = 0; i < anz; i++ )
		{
			writer->WriteStartElement ( "StringAttributPlan" );

			stdString = attributNamen[i];
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "name", STR );

			stdString = stringAttributWerte[i];
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "wert", STR );

			writer->WriteEndElement();
		}

		attributNamen.clear();
		anz = pPlanAttribute->getIntegerAttributWerte ( attributNamen, intAttributWerte );
		for ( i = 0; i < anz; i++ )
		{
			writer->WriteStartElement ( "IntegerAttributPlan" );

			stdString = attributNamen[i];
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "name", STR );

			iWert = intAttributWerte[i];
			STR = XmlConvert::ToString ( iWert );
			writer->WriteAttributeString ( "wert", STR );

			writer->WriteEndElement();
		}

		attributNamen.clear();
		anz = pPlanAttribute->getDoubleAttributWerte ( attributNamen, doubleAttributWerte );
		for ( i = 0; i < anz; i++ )
		{
			writer->WriteStartElement ( "DoubleAttributPlan" );

			stdString = attributNamen[i];
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "name", STR );

			dWert = doubleAttributWerte[i];
			STR = XmlConvert::ToString ( dWert );
			writer->WriteAttributeString ( "wert", STR );

			writer->WriteEndElement();
		}

		attributNamen.clear();
		anz = pPlanAttribute->getBoolAttributWerte ( attributNamen, boolAttributWerte );
		for ( i = 0; i < anz; i++ )
		{
			writer->WriteStartElement ( "BoolAttributPlan" );

			stdString = attributNamen[i];
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "name", STR );

			bWert = boolAttributWerte[i];
			STR = XmlConvert::ToString ( bWert );
			writer->WriteAttributeString ( "wert", STR );

			writer->WriteEndElement();
		}

		attributNamen.clear();
		stringAttributWerte.clear();
		anz = pPlanAttribute->getDateAttributWerte ( attributNamen, stringAttributWerte );
		for ( i = 0; i < anz; i++ )
		{
			writer->WriteStartElement ( "DateAttributPlan" );

			stdString = attributNamen[i];
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "name", STR );

			stdString = stringAttributWerte[i];
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "wert", STR );

			writer->WriteEndElement();
		}

		attributNamen.clear();
		stringAttributWerte.clear();
		anz = pPlanAttribute->getURLAttributWerte ( attributNamen, stringAttributWerte );
		for ( i = 0; i < anz; i++ )
		{
			writer->WriteStartElement ( "URLAttributPlan" );

			stdString = attributNamen[i];
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "name", STR );

			stdString = stringAttributWerte[i];
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "wert", STR );

			writer->WriteEndElement();
		}

 		attributNamen.clear();
		anz = pPlanAttribute->getKomplexAttributWerte ( attributNamen, komplexAttribute );
		for ( i = 0; i < anz; i++ )
		{
			writer->WriteStartElement ( "ComplexAttributPlan" );

			stdString = attributNamen[i];
      pPlanObjekt = komplexAttribute[i];
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "name", STR );

      subAttributNamen.clear();
  		stringAttributWerte.clear();
      attributTypen.clear();
      subAttributAnz = pPlanObjekt->getAttributWerte ( attributTypen, subAttributNamen, stringAttributWerte, vUOMs );

      for ( j = 0; j < subAttributAnz; j++ )
      {
        writer->WriteStartElement ( "ComplexSubAttribut" );
        writer->WriteAttributeString ( "subAttributName", QuConvert::ToString ( subAttributNamen[j] ) );
        writer->WriteAttributeString( "subAttributWert", QuConvert::ToString ( stringAttributWerte[j] ) );

        switch ( attributTypen[j] )
        {
        case _STRING:
        case _ENUMERATION:
        case _EXTERNAL_CODE_LIST:
          writer->WriteAttributeString ( "subAttributTyp", "CHARACTER_STRING" );
          break;

        case _INTEGER:
          writer->WriteAttributeString ( "subAttributTyp", "INT" );
          break;

        case _DOUBLE:
          writer->WriteAttributeString ( "subAttributTyp", "DOUBLE" );
          break;

        case _DATE:
          writer->WriteAttributeString ( "subAttributTyp", "DATE" );
          break;

        case _BOOLEAN:
          writer->WriteAttributeString ( "subAttributTyp", "BOOL" );
          break;

        case _ANY_URI:
          writer->WriteAttributeString ( "subAttributTyp", "URL" );
          break;
        }
        writer->WriteEndElement();
      }

      writer->WriteEndElement();
    }

		anz = pPlanAttribute->getTextAbschnitte ( textAbschnitte );
		for ( i = 0; i < anz; i++ )
		{
			writer->WriteStartElement ( "TextAbschnitt" );

			stdString = textAbschnitte[i]->schluessel;
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "schluessel", STR );

			stdString = textAbschnitte[i]->text;
			if ( stdString != "" )
			{
				STR = QuConvert::ToString ( stdString );
				writer->WriteAttributeString ( "wert", STR );
			}

			stdString = textAbschnitte[i]->textURL;
			if ( stdString != "" )
			{
				STR = QuConvert::ToString ( stdString );
				writer->WriteAttributeString ( "wertURL", STR );
			}

			stdString = textAbschnitte[i]->planTyp;
			if ( stdString != "" )
			{
				STR = QuConvert::ToString ( stdString );
				writer->WriteAttributeString ( "planTyp", STR );
			}

			writer->WriteEndElement();
		}

    writer->WriteStartElement ( "Bereiche" );

		anz = pPlanAttribute->getBereiche ( bereiche );
		for ( i = 0; i < anz; i++ )
		{
			Bereich * pBereich = bereiche[i];
	
			writer->WriteStartElement ( "Bereich" );

			STR = XmlConvert::ToString ( pBereich->nummer );
			writer->WriteAttributeString ( "nummer", STR );

			STR = QuConvert::ToString ( pBereich->name );
			if ( STR != "" ) writer->WriteAttributeString ( "name", STR );

			STR = QuConvert::ToString ( pBereich->bedeutung );
			if ( STR != "" ) writer->WriteAttributeString ( "bedeutung", STR );

			STR = QuConvert::ToString ( pBereich->geometrieDatei );
			if ( STR != "" ) writer->WriteAttributeString ( "geometrieDatei", STR );

			STR = QuConvert::ToString ( pBereich->versionBauNVO );
			if ( STR != "" ) writer->WriteAttributeString ( "versionBauNVO", STR );

      STR = QuConvert::ToString ( pBereich->versionBauNVOText );
      if ( STR != "" ) writer->WriteAttributeString ( "versionBauNVOText", STR );

      STR = QuConvert::ToString ( pBereich->versionBauGBDatum );
      if ( STR != "" ) writer->WriteAttributeString ( "versionBauGBDatum", STR );

      STR = QuConvert::ToString ( pBereich->versionBauGBText );
      if ( STR != "" ) writer->WriteAttributeString ( "versionBauGBText", STR );

      STR = QuConvert::ToString ( pBereich->versionBROGDatum );
      if ( STR != "" ) writer->WriteAttributeString ( "versionBROGDatum", STR );

      STR = QuConvert::ToString ( pBereich->versionBROGText );
      if ( STR != "" ) writer->WriteAttributeString ( "versionBROGText", STR );

      STR = QuConvert::ToString ( pBereich->versionLPLGDatum );
      if ( STR != "" ) writer->WriteAttributeString ( "versionLPLGDatum", STR );

      STR = QuConvert::ToString ( pBereich->versionLPLGText );
      if ( STR != "" ) writer->WriteAttributeString ( "versionLPLGText", STR );

			STR = QuConvert::ToString ( pBereich->bedeutungEnum );
			if ( STR != "" ) writer->WriteAttributeString ( "bedeutungEnum", STR );

			STR = QuConvert::ToString ( pBereich->massstab );
			if ( STR != "" ) writer->WriteAttributeString ( "massstab", STR );

			if ( pBereich->getRasterplanBasis() != NULL )
				writeRasterplanBasis ( pBereich->getRasterplanBasis() );

			writer->WriteEndElement();
		}
	}
	writer->WriteEndElement();


	anz = m_pKonv->getKlassenNamen ( klassenNamen );
	for ( i = 0; i < anz; i++ )
	{
		KlassenName * pKlassenName = klassenNamen[i];
		writeKlassenName ( pKlassenName );
	}

	anz = m_pKonv->getEnumerations ( enumerationen );
	for ( i = 0; i < anz; i++ )
	{
		Enumeration * pEnum = enumerationen[i];
		writeEnumeration ( pEnum );
	}

	writer->WriteEndElement();

	writer->Flush();
	writer->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben Klassen-Name			                      									 //
///////////////////////////////////////////////////////////////////////////////
void KonvDateiWriter::writeKlassenName( KlassenName * pKlassenNameP )
{
	String              ^ STR;
	string                stdString;
  vector<ATTRIBUT_TYP>  attributTypen;
  vector<string>        subAttributNamen;
  vector<string>        subAttributWerte;
  vector<AttributName*> attribute;
	size_t                i, j, anz, subAttributAnz;
  vector<UOM*>          vUOMs;

	writer->WriteStartElement ( "ClassTransform" );

	stdString = pKlassenNameP->getKlassenName();
	STR = QuConvert::ToString ( stdString );
	writer->WriteAttributeString ( "standardName", STR );

	stdString = pKlassenNameP->getShapeFileName();
	STR = QuConvert::ToString ( stdString );
	writer->WriteAttributeString ( "actualName", STR );

	i = pKlassenNameP->getBereichNummer();
	STR = XmlConvert::ToString ( i );
	writer->WriteAttributeString ( "bereichNummer", STR );

  int rechtlicheBedeutung = pKlassenNameP->getRechtlicheBedeutung();
  switch ( rechtlicheBedeutung )
  {
  case PLANINHALT:
  	writer->WriteAttributeString ( "rechtlicheBedeutung", "PLANINHALT" );
    break;

  case NACHR_UEBERNAHME:
  	writer->WriteAttributeString ( "rechtlicheBedeutung", "NACHR_UEBERNAHME" );
    break;
  }

	anz = pKlassenNameP->getBPlanAttributObjekte ( attribute );
	for ( i = 0; i < anz; i++ )
	{
		AttributName * pAttributName = attribute[i];

		writer->WriteStartElement ( "AttributeTransform" );

		STR = QuConvert::ToString ( pAttributName->nameStandard );
		writer->WriteAttributeString ( "standardName", STR );

		if ( pAttributName->nameAktuell != "" )
		{
			STR = QuConvert::ToString ( pAttributName->nameAktuell );
			writer->WriteAttributeString ( "actualName", STR );
		}

		STR = XmlConvert::ToString ( pAttributName->getIndex() );
		writer->WriteAttributeString ( "index", STR );

		switch ( pAttributName->typ )
		{
		case _ENUMERATION:
			writer->WriteAttributeString ( "type", "ENUMERATION" );
			STR = QuConvert::ToString ( pAttributName->enumerationId );
			writer->WriteAttributeString ( "enumerationId", STR );
			break;

		case _EXTERNAL_CODE_LIST:
			writer->WriteAttributeString ( "type", "EXTERNAL_CODE_LIST" );
			STR = QuConvert::ToString ( pAttributName->enumerationId );
			writer->WriteAttributeString ( "enumerationId", STR );
			break;

		case _STRING:
			writer->WriteAttributeString ( "type", "CHARACTER_STRING" );
			break;

		case _DOUBLE:
			writer->WriteAttributeString ( "type", "DOUBLE" );
			break;

		case _INTEGER:
			writer->WriteAttributeString ( "type", "INT" );
			break;

		case _BOOLEAN:
			writer->WriteAttributeString ( "type", "BOOL" );
			break;

		case _ANGLE:
			writer->WriteAttributeString ( "type", "ANGLE" );
			break;

		case _LENGTH:
			writer->WriteAttributeString ( "type", "LENGTH" );
			break;

		case _AREA:
			writer->WriteAttributeString ( "type", "AREA" );
			break;

		case _VOLUME:
			writer->WriteAttributeString ( "type", "VOLUME" );
			break;

		case _DATE:
			writer->WriteAttributeString ( "type", "DATE" );
			break;

		case _TEXTE:
			writer->WriteAttributeString ( "type", "TEXTE" );
			break;

		case _ANY_URI:
			writer->WriteAttributeString ( "type", "URL" );
			break;

		case _POS_X:
			writer->WriteAttributeString ( "type", "POS_X" );
			break;

		case _POS_Y:
			writer->WriteAttributeString ( "type", "POS_Y" );
			break;

		case _POS_XY:
			writer->WriteAttributeString ( "type", "POS_XY" );
			break;

		case _REFERENZ:
			writer->WriteAttributeString ( "type", "REFERENZ" );
			break;

		case _DATA_TYPE:
			writer->WriteAttributeString ( "type", "DATA_TYPE" );
			break;

		case _SONST:
			writer->WriteAttributeString ( "type", "CHARACTER_STRING" );
			break;
		}

		if ( pAttributName->defaultWert != "" )
		{
			STR = QuConvert::ToString ( pAttributName->defaultWert );
			writer->WriteAttributeString ( "default", STR );
		}

		if ( pAttributName->subAttributName != "" )
		{
			STR = QuConvert::ToString ( pAttributName->subAttributName );
			writer->WriteAttributeString ( "subAttributName", STR );
		}

		switch ( pAttributName->subAttributTyp )
		{
		case _ENUMERATION:
			writer->WriteAttributeString ( "subAttributTyp", "ENUMERATION" );
			STR = QuConvert::ToString ( pAttributName->enumerationId );
			writer->WriteAttributeString ( "enumerationId", STR );
			break;

		case _EXTERNAL_CODE_LIST:
			writer->WriteAttributeString ( "subAttributTyp", "EXTERNAL_CODE_LIST" );
			STR = QuConvert::ToString ( pAttributName->enumerationId );
			writer->WriteAttributeString ( "enumerationId", STR );
			break;

		case _STRING:
			writer->WriteAttributeString ( "subAttributTyp", "CHARACTER_STRING" );
			break;

		case _DOUBLE:
			writer->WriteAttributeString ( "subAttributTyp", "DOUBLE" );
			break;

		case _INTEGER:
			writer->WriteAttributeString ( "subAttributTyp", "INT" );
			break;

		case _BOOLEAN:
			writer->WriteAttributeString ( "subAttributTyp", "BOOL" );
			break;

		case _ANGLE:
			writer->WriteAttributeString ( "subAttributTyp", "ANGLE" );
			break;

		case _LENGTH:
			writer->WriteAttributeString ( "subAttributTyp", "LENGTH" );
			break;

		case _AREA:
			writer->WriteAttributeString ( "subAttributTyp", "AREA" );
			break;

		case _VOLUME:
			writer->WriteAttributeString ( "subAttributTyp", "VOLUME" );
			break;

		case _DATE:
			writer->WriteAttributeString ( "subAttributTyp", "DATE" );
			break;

		case _ANY_URI:
			writer->WriteAttributeString ( "subAttributTyp", "URL" );
			break;

		}

    if ( pAttributName->generAttributName != "" )
    {
      STR = QuConvert::ToString ( pAttributName->generAttributName );
      writer->WriteAttributeString ( "generAttributName", STR );
    }

    if ( pAttributName->defaultWertKomplexAttribut!= NULL )
    {

      attributTypen.clear();
      subAttributNamen.clear();
      subAttributWerte.clear();
      subAttributAnz = pAttributName->defaultWertKomplexAttribut->getAttributWerte ( attributTypen, subAttributNamen, subAttributWerte, vUOMs );

      for ( j = 0; j < subAttributAnz; j++ )
      {
        writer->WriteStartElement ( "ComplexSubAttribut" );
        writer->WriteAttributeString ( "subAttributName", QuConvert::ToString ( subAttributNamen[j] ) );
        writer->WriteAttributeString( "subAttributWert", QuConvert::ToString ( subAttributWerte[j] ) );

        switch ( attributTypen[j] )
        {
        case _STRING:
        case _ENUMERATION:
        case _EXTERNAL_CODE_LIST:
          writer->WriteAttributeString ( "subAttributTyp", "CHARACTER_STRING" );
          break;

        case _INTEGER:
          writer->WriteAttributeString ( "subAttributTyp", "INT" );
          break;

        case _DOUBLE:
          writer->WriteAttributeString ( "subAttributTyp", "DOUBLE" );
          break;

        case _DATE:
          writer->WriteAttributeString ( "subAttributTyp", "DATE" );
          break;

        case _BOOLEAN:
          writer->WriteAttributeString ( "subAttributTyp", "BOOL" );
          break;

        case _ANY_URI:
          writer->WriteAttributeString ( "subAttributTyp", "URL" );
          break;

        case _LENGTH:
          writer->WriteAttributeString ( "subAttributTyp", "LENGTH" );
          break;

        case _AREA:
          writer->WriteAttributeString ( "subAttributTyp", "AREA" );
          break;

        case _VOLUME:
          writer->WriteAttributeString ( "subAttributTyp", "VOLUME" );
          break;

        case _REFERENZ:
          writer->WriteAttributeString ( "subAttributTyp", "REFERENZ" );
          break;

        case _DATA_TYPE:
          writer->WriteAttributeString ( "subAttributTyp", "DATA_TYPE" );
          break;

        }
        writer->WriteEndElement();
      }
    }

		writer->WriteEndElement();
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben Enumeration							                        					 //
///////////////////////////////////////////////////////////////////////////////
void KonvDateiWriter::writeEnumeration( Enumeration * pEnumerationP )
{
	String ^ STR;
	string   stdString;
  string   standardName;
  string   aktuellName;
	size_t   i, anz;

	vector<string> itemNamenAktuell;
	vector<string> itemNamenStandard;

	writer->WriteStartElement ( "EnumerationTransform" );

	stdString = pEnumerationP->getEnumerationId();
	STR = QuConvert::ToString ( stdString );
	writer->WriteAttributeString ( "enumerationId", STR );

  stdString = pEnumerationP->getDictionary();
  if ( stdString != "" )
  {
    STR = QuConvert::ToString ( stdString );
    writer->WriteAttributeString ( "codeSpace" , STR );
  }

	anz = pEnumerationP->getEnumerationItems ( itemNamenAktuell, itemNamenStandard );
	for ( i = 0; i < anz; i++ )
	{
		standardName = itemNamenStandard[i];
    aktuellName = itemNamenAktuell[i];

    if ( standardName != "Error" )
    {
      writer->WriteStartElement ( "EnumerationItem" );

      STR = QuConvert::ToString ( standardName );
      writer->WriteAttributeString ( "standardName", STR );

      STR = QuConvert::ToString ( aktuellName );
      writer->WriteAttributeString ( "actualName", STR );

      writer->WriteEndElement();
    }
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Rausscheiben RasterplanBasis						                      					 //
///////////////////////////////////////////////////////////////////////////////
void KonvDateiWriter::writeRasterplanBasis( RasterplanBasis * pRasterplanBasis )
{
	string                   stdString;
	size_t                   i, anz;
	vector<ExterneReferenz*> vExterneReferenzen;
	ExterneReferenz        * pExterneReferenz;

	anz = pRasterplanBasis->getExterneReferenzen ( vExterneReferenzen );
	if ( anz == 0 ) return;

	writer->WriteStartElement ( "rasterplanBasis" );
	
	for ( i = 0; i < anz; i++ )
	{
		pExterneReferenz = vExterneReferenzen[i];
		writeExterneReferenz ( pExterneReferenz );
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben ExterneReferenz				                      							 //
///////////////////////////////////////////////////////////////////////////////
void KonvDateiWriter::writeExterneReferenz ( ExterneReferenz * pExterneReferenz )
{
	String^ STR;
	string   stdString;

	writer->WriteStartElement ( "externeReferenz" );

	stdString = pExterneReferenz->relationName;
	if ( stdString != "" )
	{
		STR = QuConvert::ToString ( stdString );
		writer->WriteAttributeString( "relationName", STR );
	}

	stdString = pExterneReferenz->informationssystemURL;
	if ( stdString != "" )
	{
		STR = QuConvert::ToString ( stdString );
		writer->WriteAttributeString( "informationssystemURL", STR );
	}

	stdString = pExterneReferenz->referenzName;
	if ( stdString != "" )
	{
		STR = QuConvert::ToString ( stdString );
		writer->WriteAttributeString( "referenzName", STR );
	}

	stdString = pExterneReferenz->referenzURL;
	if ( stdString != "" )
	{
		STR = QuConvert::ToString ( stdString );
		writer->WriteAttributeString( "referenzURL", STR );
	}

	stdString = pExterneReferenz->referenzMimeType;
	if ( stdString != "" )
	{
		STR = QuConvert::ToString ( stdString );
		writer->WriteAttributeString( "referenzMimeType", STR );
	}

	stdString = pExterneReferenz->beschreibung;
	if ( stdString != "" )
	{
		STR = QuConvert::ToString ( stdString );
		writer->WriteAttributeString( "beschreibung", STR );
	}

	stdString = pExterneReferenz->georefURL;
	if ( stdString != "" )
	{
		STR = QuConvert::ToString ( stdString );
		writer->WriteAttributeString( "georefURL", STR );
	}

	stdString = pExterneReferenz->georefMimeType;
	if ( stdString != "" )
	{
		STR = QuConvert::ToString ( stdString );
		writer->WriteAttributeString( "georefMimeType", STR );
	}

  stdString = pExterneReferenz->datum;
  if ( stdString != "" )
  {
    STR = QuConvert::ToString ( stdString );
    writer->WriteAttributeString( "datum", STR );
  }

	writer->WriteEndElement();
}




