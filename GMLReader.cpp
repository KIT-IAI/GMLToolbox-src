#include "StdAfx.h"

#include <vector>
#include <map>
#include <string>
#include <set>

#include "Convert.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "Geometrie.h"
#include "GmlSchema.h"
#include "Dictionary.h"
#include "GmlNamespaces.h"
#include "TimeX.h"
#include "UnitOfMeasure.h"
#include "TimeSeries.h"
#include "Appearance.h"
#include "Checking.h"
#include "constant.h"
#include "Logfile.h"
#include "GMLReader.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Text;
using namespace System::Collections;

///////////////////////////////////////////////////////////////////////////////
//  GMLReader                                                                //
//  Einlesen ein oder mehrerer GML-Dateien                                   //
///////////////////////////////////////////////////////////////////////////////
GMLReader::GMLReader ( Features * pPlanObjekteP, bool readCodeListDictionaries )
{
  pFeatures                = pPlanObjekteP;
  pGmlSchema               = pFeatures->getGmlSchema();
  pReadErrors              = NULL;
  m_readCodeListDictionary = readCodeListDictionaries;

  pAppearances      = new Appearances( pFeatures );
  pGmlNamespacesAct = NULL;

  delimiter = gcnew cli::array<wchar_t> {_T(' '), _T('\n'), _T('\t'), _T('\r'), _T('\f') };

	nErr		     = 0;
	featureAnz	 = 0;
	planTyp      = KEIN_XPLAN;

	swRead		= false;
	noRead		= false;

  generateFeatureDateienMap = true;

  gmlIdTag	            = "gml:id";
  INSPIRECodelistSuffix = ".de.xml";
}

GMLReader::~GMLReader()
{
  delete pAppearances;
  if ( pGmlNamespacesAct != NULL ) delete pGmlNamespacesAct;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer GML-Datei ohne Modifikation des Modells                   //
///////////////////////////////////////////////////////////////////////////////
int GMLReader::readWithoutModification( std::string fileName, Checking * pChecking )
{
  string                      std_string, gmlId;
  string                      gmlKlassenName;
  string                      srsName;
  int                         srsDimension;
  double                      sw[3], no[3];
  bool			                  success;
  bool                        first = true;
  Feature                   * pFeature;
  String                    ^ ATTRIBUT_NAME;
  String                    ^ ATTRIBUT_WERT;
  string                      attributName;
  string                      attributWert;
  ReadError                 * pReadError;
  int                         iFeature = 0;
  int                         maxFeature = 10000000;
  GML_SCHEMA_TYPE             schemaType = GML_SCHEMA_UNKNOWN;

  if ( pChecking != NULL )
  {
    pReadErrors = pChecking->getErrorReport();
    pReadErrors->resetReadErrors();
  }

  String ^ FILE_NAME = QuConvert::ToString( fileName );
  pFolderName = Path::GetDirectoryName( FILE_NAME );

  if ( generateFeatureDateienMap )
    featureDateiName = FILE_NAME;
  else
    featureDateiName = "";
 
    if ( pGmlSchema != NULL )
    {
      pGmlNamespacesStandard = pGmlSchema->getGmlNamespacesStandard();
      pSchemaTypeNamesStandard = pGmlSchema->getSchemaTypeNames();
    }

    if ( pGmlNamespacesAct == NULL )
        pGmlNamespacesAct = new GmlNamespaces();

    System::String ^ fName = gcnew System::String( fileName.c_str() );
    validReader      = gcnew XmlTextReader( fName );

    nErr		= 0;

    while( validReader->Read() )
    {
      if ( validReader->NodeType == XmlNodeType::Element )
      {

        pActElement = validReader->Name;
        pActElementLoc = validReader->LocalName;

        if ( first == true || schemaType == GML_SCHEMA_UNKNOWN )
        {
          while ( validReader->MoveToNextAttribute() )
          {
            String ^XMLNS = "xmlns";
            String ^KUERZEL;
            string  kuerzel;

            ATTRIBUT_NAME = validReader->Name;
            ATTRIBUT_WERT = validReader->Value;
            QuConvert::systemStr2stdStr ( attributWert, ATTRIBUT_WERT );

            if ( ATTRIBUT_NAME->Contains ( XMLNS ) )
            {
              if ( ATTRIBUT_NAME->Length < 6 )
                pGmlNamespacesAct->setDefaultNamespace ( attributWert );
              else
              {
                KUERZEL = ATTRIBUT_NAME->Substring ( 6 );
                QuConvert::systemStr2stdStr ( kuerzel, KUERZEL );
                pGmlNamespacesAct->addNamespace ( kuerzel, attributWert );
              }
            } 
            else
              if ( ATTRIBUT_NAME->CompareTo( "gml:id") == 0 )
              {
                pFeatures->setGmlId( attributWert );
              }
          }

          schemaType = pGmlNamespacesAct->getSchemaType();
          if ( schemaType != GML_SCHEMA_UNKNOWN )         
          {
            if ( pFeatures->getGmlTyp() != schemaType )
            {
              if ( pFeatures->getDazuladen() && pGmlSchema != NULL )
              {
                MessageBox::Show("Die hinzugeladene Datei hat ein anderes GML-Schema als die schon gelesenen Dateien");
                return 0;
              }

              pGmlSchema = new GmlSchema ( pFeatures );
              success = pGmlSchema->readGmlSchema( schemaType, pGmlNamespacesAct );
              if (!success )
              {
                delete pGmlSchema;
                MessageBox::Show( "Fehler bein Einlesen des GML-Schemas" );
                validReader->Close();
              return 0;
              }
              else
                pFeatures->setGmlSchema( pGmlSchema );
            }
            else            
              pGmlSchema = pFeatures->getGmlSchema();
             

            pGmlNamespacesStandard = pGmlSchema->getGmlNamespacesStandard();
            pSchemaTypeNamesStandard = pGmlSchema->getSchemaTypeNames();
          }

          first = false;
        }

        if ( pActElementLoc->CompareTo("Envelope") == 0  )
        {
          success = readEnvelope(  sw, no, srsName, srsDimension );
          if ( success )
          {
            if ( srsDimension > 0 )
            {
              pFeatures->setSrsDimension ( srsDimension );
              if ( srsName != "" )
                pFeatures->setSrsName( srsName );
            }
            else
            {
              if ( srsName != "" )
                pFeatures->setSrsNameAndDimension( srsName );
            }
          }
        }
        else
        if ( pActElementLoc->CompareTo("featureMember") == 0    ||
             pActElementLoc->CompareTo("featureMembers") == 0    ||
             pActElementLoc->CompareTo("member") == 0           ||
             pActElementLoc->CompareTo("cityObjectMember") == 0 ||
             pActElementLoc->CompareTo("siteMember") == 0 ||
             pActElementLoc->CompareTo("Insert") == 0)
            {
              if ( validReader->IsEmptyElement == false )
              {
                while ( validReader->MoveToNextAttribute() )
                {                        
                  ATTRIBUT_NAME = validReader->Name;
                  ATTRIBUT_WERT = validReader->Value;
                  if ( ATTRIBUT_NAME->CompareTo( "xlink:href") == 0 && pReadErrors != NULL )
                  {
                    QuConvert::systemStr2stdStr( attributWert, ATTRIBUT_WERT );
                    std_string = "Verbotenes xlink " + attributWert + " im cityObjectMember: ";    

                    pReadError = new ReadError ( Error::ERROR, std_string, validReader->LineNumber );
                    pReadErrors->addReadError ( pReadError );
                  }
                }

                srsDimension = pFeatures->getSrsDimension();
                pFeature = readFeatureMember( pActElement );
                if ( pFeature == NULL )
                  break;
                
                iFeature++;
                if ( iFeature >= maxFeature )
                  break;

              }
        }
        else    
        if ( pActElementLoc->CompareTo(_T("appearanceMember")) == 0 )
        {
          if ( validReader->IsEmptyElement == false )
            readAppearance ( pActElementLoc );
        }
      }
    }


    validReader->Close();   

    return featureAnz;
}

///////////////////////////////////////////////////////////////////////////////
//  Überarbeitung des Gesamtmodells nach Einlesen von ein oder mehreren      //
//  Dateien                                                                  //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::modifyModel()
{
	size_t               i, j, anz, anzEbenen;
  vector<BLeitPlan*>   vBPlaene;
  vector<PlanBereich*> vBPlanEbenen;
  BLeitPlan          * pBPlan;
  PlanBereich        * pBPlanEbene;

  pAppearances->resolveReferences();

	pFeatures->resolveOrientableCurveGeometry();

  anz = pFeatures->getPlaene ( vBPlaene );
  for ( i = 0; i < anz; i++ )
  {
    vBPlanEbenen.clear();
    pBPlan = vBPlaene[i];
    pBPlan->createTextMaps();
    anzEbenen = pBPlan->getPlanBereiche ( vBPlanEbenen );
    for ( j = 0; j < anzEbenen; j++ )
    {
      pBPlanEbene = vBPlanEbenen[j];
      pBPlanEbene->resolveObjektIds( pBPlan );
    }
  }

  if ( pFeatures->isALKIS() )
    pFeatures->modifyAlkisModel();

  if ( pFeatures->getGmlTyp() == BOREHOLEML_3_0 )
    pFeatures->createBMLVisualizationGeometry(); 

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer GML-Datei mit Modifikation                                //
///////////////////////////////////////////////////////////////////////////////
int GMLReader::read( std::string fileName, Checking * pChecking )
{
  int anz = readWithoutModification( fileName, pChecking );
  modifyModel();

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert den Reader vor Einlesen einer neuen Datei                  //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::reset()
{
  nErr = 0;
  featureAnz = 0;;
  swRead = false;
  noRead = false;
  planTyp      = KEIN_XPLAN;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest die Bounding Box                                                   //
///////////////////////////////////////////////////////////////////////////////
bool GMLReader::readEnvelope( double * sw, double * no, string &srsName, int &srsDimension )
{
  int                   i, dim;
  bool                  swRead, noRead;
  System::String^       STR;
  cli::array<String^> ^ split;
  std::string          stdString;

  swRead       = false;
  noRead       = false;
  srsName      = "";
  srsDimension = -1;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = XmlConvert::ToInt32( STR );
  
	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo ( "pos") == 0 )
			{
				STR = validReader->ReadString();
				if (STR->Length != 0)
				{
					STR = STR->Trim();
					split = STR->Split(delimiter, StringSplitOptions::RemoveEmptyEntries );
          dim = split->Length;
          if ( dim > 3 ) dim = 3;

					for ( i = 0; i <dim; i++)
					{
						if ( !swRead ) sw[i] = XmlConvert::ToDouble(split[i] );
						else		       no[i] = XmlConvert::ToDouble(split[i] );
					}
					if ( !swRead ) swRead = true;
					else
					if ( !noRead ) noRead = true;
				}
			}
			else
			if ( pActElement->CompareTo ( "lowerCorner") == 0 )
			{
				STR = validReader->ReadString();
				if (STR->Length != 0)
				{
					STR = STR->Trim();
					split = STR->Split(delimiter, StringSplitOptions::RemoveEmptyEntries );
          dim = split->Length;
          if ( dim > 3 ) dim = 3;

					for ( i = 0; i < dim; i++)
					{
						sw[i] = XmlConvert::ToDouble(split[i] );
					}
					swRead = true;
				}
			}
			else
			if ( pActElement->CompareTo ( "upperCorner") == 0 )
			{
				STR = validReader->ReadString();
				if (STR->Length != 0)
				{
					STR = STR->Trim();
					split = STR->Split(delimiter, StringSplitOptions::RemoveEmptyEntries );
          dim = split->Length;
          if ( dim > 3 ) dim = 3;

					for ( i = 0; i < dim; i++)
					{
						no[i] = XmlConvert::ToDouble(split[i] );
					}
					noRead = true;
				}
			}
		}
		else if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if (pActElement->CompareTo("Envelope") == 0 ) break;
		}
	}

	if ( swRead && noRead )
	{
    srsDimension = dim;
    if ( dim < 3 )
    {
      sw[2] = 0.0;
      no[2] = 0.0;
    }
		return true;
	}
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest ein featureMember												                        	 //
///////////////////////////////////////////////////////////////////////////////
Feature * GMLReader::readFeatureMember(  String ^ featureProp )
{
  string            gmlKlassenName;
  string            featureMemberName;
	string            stdString;
  string            gmlId = "";
  GmlElement      * pGmlElement;
  bool              success;
	Feature         * pFeature = NULL;

	while( validReader->Read() )
	{
    pActElement    = validReader->Name;
    pActElementLoc = validReader->LocalName;

    if ( validReader->NodeType == XmlNodeType::Element )
		{
      if ( pGmlSchema == NULL )
      {
        while ( validReader->MoveToNextAttribute() )
        {
          String ^ATTRIBUT_NAME;
          String ^ATTRIBUT_WERT;
          String ^XMLNS = "xmlns";
          String ^KUERZEL;
          string  attributName;
          string  attributWert;
          string  kuerzel;

          ATTRIBUT_NAME = validReader->Name;
          ATTRIBUT_WERT = validReader->Value;
          QuConvert::systemStr2stdStr ( attributWert, ATTRIBUT_WERT );

          if ( ATTRIBUT_NAME->Contains ( XMLNS ) )
          {
            if ( ATTRIBUT_NAME->Length < 6 )
              pGmlNamespacesAct->setDefaultNamespace ( attributWert );
            else
            {
              KUERZEL = ATTRIBUT_NAME->Substring ( 6 );
              QuConvert::systemStr2stdStr ( kuerzel, KUERZEL );
              pGmlNamespacesAct->addNamespace ( kuerzel, attributWert );
            }
          }
          else
          if ( ATTRIBUT_NAME->CompareTo( "gml:id") == 0 )
          {
              gmlId = attributWert;
          }                    
        }

        GML_SCHEMA_TYPE schemaType = pGmlNamespacesAct->getSchemaType();
        if ( schemaType == GML_SCHEMA_UNKNOWN )
        {
          schemaType = GML_SCHEMA_DEFAULT;
          pGmlSchema = new GmlSchema ( pFeatures );
          success = pGmlSchema->readGmlSchema( schemaType, pGmlNamespacesAct );
          if (!success )
          {
            delete pGmlSchema;
            return 0;
          }
          pFeatures->setGmlSchema( pGmlSchema );

          pGmlNamespacesStandard = pGmlSchema->getGmlNamespacesStandard();
          pSchemaTypeNamesStandard = pGmlSchema->getSchemaTypeNames();
        }

        if ( pFeatures->getGmlTyp() != schemaType )
        {
          pGmlSchema =  new GmlSchema ( pFeatures );
          success = pGmlSchema->readGmlSchema( schemaType, pGmlNamespacesAct );
          if ( !success )
          {
            delete pGmlSchema;
            pGmlSchema = NULL;
            MessageBox::Show( "Fehler bein Einlesen des GML-Schemas" );
            validReader->Close();
            return NULL;
          }
          else
          {
            pFeatures->setGmlSchema( pGmlSchema );
          }
        }
        else
          pGmlSchema = pFeatures->getGmlSchema();

        pGmlNamespacesStandard = pGmlSchema->getGmlNamespacesStandard();
        pSchemaTypeNamesStandard = pGmlSchema->getSchemaTypeNames();
      }

      QuConvert::systemStr2stdStr( featureMemberName, featureProp );
      featureMemberName = addDefaultNamespaceKuerzel ( featureMemberName );
      featureMemberName = createGmlNameStandard ( featureMemberName );


      QuConvert::systemStr2stdStr ( gmlKlassenName, pActElement );
      gmlKlassenName = addDefaultNamespaceKuerzel ( gmlKlassenName );
      gmlKlassenName = createGmlNameStandard ( gmlKlassenName );
      pGmlElement =pGmlSchema->getGmlElement( gmlKlassenName );
      if ( pGmlElement != NULL )
      {
        pFeature = readFeature ( pActElement, NULL, featureMemberName );
        if ( gmlId != "" )
          pFeature->setGmlId( gmlId );		
      }
      else
        pFeature = NULL;
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			if ( pActElement->CompareTo( featureProp ) == 0 ) break;
		}
	}
  return pFeature;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest ein Feature 												                          		 //
///////////////////////////////////////////////////////////////////////////////
Feature * GMLReader::readFeature( System::String ^ pObjektNameP, GmlKlasse * pGmlReferenceType, string featureMemberName )
{
  string            dateiName;
	String			    ^ STR;
  String			    ^ STR_LOC;
	String			    ^ END_TAG;
  String          ^ HREF;
  String          ^ TITLE;
  String          ^ END_TAG_LOC;
  String          ^ GENERIC_ATTRIBUTE_VALUE;
	Feature         * pFeature;
	Feature         * pFeatureRelated;
	FeatureRelation * pRelation;
  Feature         * pGenericAttribute;
	string            stdString;
  string            errorMessage;
  string            typName;
	string            gmlId;
  string            parentId;
  string            genericAttributName;
  string            genericAttributValue;
  string            gmlAttributNameAct;
  string            gmlKlassenNameAct;
  string            gmlAttributNameOrg;
	string			      classifierName;
  string			      classifierNameLoc;
	string			      classifierValue;
  string            externalCodeListPath;
  string            externalCodeListName;
  string            gmlAttributTypName;
  string            href;
  string            uom;
  string            nilReason;
  GmlKlasse       * pGmlReferenceTypeLoc;
	OBJEKT_TYP        objektTyp;
  OBJEKT_TYP        objektTypLoc;
	double			      h;
	int               ih;
  unsigned int      k;
	bool			        bh;
  bool              emptyElement;
  bool              nil;
	ATTRIBUT_TYP      gmlAttributTyp;
  GmlKlasse       * pGmlKlasse;
  GmlElement      * pGmlElement;
  vector<string>    classifierNamen;
  vector<string>    classifierValues;

  GmlDictionaryEntry  * pDictionaryEntry;
  GmlDictionaryReader ^ pDictionaryReader;
  StringAttr          * pSimpleContent;
  ReadError           * pReadError;
  UOM                 * pUOM;
  AbstractTimeSeries * pTimeSeries;

  if ( m_readCodeListDictionary && pGmlSchema->externalCodeListDictionariesExist() )
    pDictionaryReader = gcnew GmlDictionaryReader ( pFeatures->getGmlTyp() );
  else
    pDictionaryReader = nullptr;

  featureAnz++;

	STR = validReader->GetAttribute ( gmlIdTag );
	if ( STR == nullptr )
		gmlId = Constant::createGmlId();
	else
		QuConvert::systemStr2stdStr ( gmlId, STR );

	QuConvert::systemStr2stdStr ( gmlKlassenNameAct, pObjektNameP );
  gmlKlassenNameAct = addDefaultNamespaceKuerzel ( gmlKlassenNameAct );
  gmlKlassenNameAct = createGmlNameStandard ( gmlKlassenNameAct );

  pGmlElement = pGmlSchema->getGmlElement( gmlKlassenNameAct );
	if ( pGmlElement == NULL )
	{
    pGmlKlasse = pGmlReferenceType;
	}
  else
  {
    typName = pGmlElement->getGmlTypeName();
    pGmlKlasse = pGmlSchema->getGmlKlasse ( typName );
    if ( pGmlKlasse == NULL )
    {
      pGmlKlasse = new GmlKlasse ( typName );
      pGmlSchema->addGmlKlasse( pGmlKlasse );
    }
  }

  if ( pGmlKlasse == NULL )
  {
    stdString = "Ungültige Objektart: " + gmlKlassenNameAct;
    pFeatures->getLogfile()->writeLogFile ( stdString );
   
    if ( pReadErrors != NULL )
    {
      pReadError = new ReadError ( Error::ERROR, stdString, validReader->LineNumber );
      pReadErrors->addReadError ( pReadError );
    }

    skipElement( validReader->LocalName );
    return NULL;
  }

	objektTyp =  pGmlKlasse->getGmlObjektTyp();
	switch ( objektTyp )
	{
	case BLEIT_PLAN:
		pFeature = new BLeitPlan ( pFeatures, gmlId, gmlKlassenNameAct );
		break;

	case PLAN_BEREICH:
		pFeature = new PlanBereich ( pFeatures, gmlId, gmlKlassenNameAct );
		break;

  default:
		pFeature = new Feature ( objektTyp, gmlId, pFeatures, gmlKlassenNameAct, featureMemberName ); 
		break;
	}

	if ( validReader->IsEmptyElement )
  {
    if ( validReader->AttributeCount > 0 )
    {
      pSimpleContent = new StringAttr;

      while ( validReader->MoveToNextAttribute() )
      {
        STR = validReader->Name;
        if ( STR != nullptr ) QuConvert::systemStr2stdStr ( classifierName, STR );
        else			   classifierName = "";

        STR = validReader->Value;
        if ( STR != nullptr ) QuConvert::systemStr2stdStr ( classifierValue, STR );
        else			   classifierValue = "";

        if ( classifierName != "" && classifierValue != "" )
        {
          pSimpleContent->classifier[classifierName] = classifierValue;
        }
      }
      pFeature->setSimpleContent ( pSimpleContent );
    }

    QuConvert::systemStr2stdStr( dateiName, featureDateiName );
    pFeatures->addFeature ( pFeature, dateiName );
		return pFeature;
  }

  parentId = pFeature->getGmlId();

	while( validReader->Read() )
	{
    if ( validReader->NodeType == XmlNodeType::Text )
    {
      STR = validReader->Value;
      if ( STR->CompareTo( "" ) != 0 )
      {
        pSimpleContent = pFeature->getSimpleContent();
        if ( pSimpleContent == NULL )
        {
          pSimpleContent = new StringAttr;
          pFeature->setSimpleContent( pSimpleContent );
        }
        QuConvert::systemStr2stdStr ( pSimpleContent->value, STR );
      }
      
      QuConvert::systemStr2stdStr( dateiName, featureDateiName );
      pFeatures->addFeature ( pFeature, dateiName );
      return pFeature;
    }
    else
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

			QuConvert::systemStr2stdStr ( gmlAttributNameAct, pActElement );
      gmlAttributNameOrg = gmlAttributNameAct;
      gmlAttributNameAct = addDefaultNamespaceKuerzel ( gmlAttributNameAct );
      gmlAttributNameAct = createGmlNameStandard ( gmlAttributNameAct );

			GmlAttribut * pGmlAttribut = pGmlKlasse->getGmlAttribut ( gmlAttributNameAct );
			if ( pGmlAttribut != NULL )	gmlAttributTyp = pGmlAttribut->getGmlAttributTyp();
			else						            gmlAttributTyp = _SONST;

      classifierNamen.clear();
      classifierValues.clear();
      HREF  = nullptr;
      TITLE = nullptr;
      href  = "";
      pUOM = NULL;

      nil = false;
      nilReason = "";
      emptyElement = validReader->IsEmptyElement;
      while ( validReader->MoveToNextAttribute() )
      {
        STR = validReader->Name;
        STR_LOC = validReader->LocalName;
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( classifierName, STR );
          QuConvert::systemStr2stdStr ( classifierNameLoc, STR_LOC );
        }
        else
        {
          classifierName = "";
          classifierNameLoc = "";
        }

        STR = validReader->Value;
        if ( STR != nullptr ) QuConvert::systemStr2stdStr ( classifierValue, STR );
        else			   classifierValue = "";

        if ( classifierName != "" && classifierValue != "" )
        {
          if ( classifierName == "xlink:href" )
          {
            href =  classifierValue;
            HREF = QuConvert::ToString ( classifierValue );
          }
          else
          if ( classifierNameLoc == "uom" )
          {
            classifierNamen.push_back( classifierName );
            classifierValues.push_back( classifierValue );
            pUOM = pFeatures->getUOMList()->getUOMFromIdentifier( classifierValue );
          }
          else
          {
            classifierNamen.push_back( classifierName );
            classifierValues.push_back( classifierValue );

            if ( classifierNameLoc == "nil" && classifierValue == "true" )
              nil = true;

            if ( classifierNameLoc == "nilReason" )
              nilReason = classifierValue;
          }
        }
      }

			switch ( gmlAttributTyp )
			{
      case _ADDRESS:
        if ( emptyElement )
        {
          if ( HREF != nullptr && HREF->Length > 1 )
          {
            if ( HREF[0] == '#' )
              HREF = HREF->Substring ( 1 );
            QuConvert::systemStr2stdStr ( stdString, HREF );

            pRelation = new FeatureRelation;
            pRelation->rollenName = gmlAttributNameAct;
            pRelation->featureId = stdString;
            for ( k = 0; k < classifierNamen.size(); k++ )
            {
              pRelation->classifierName.push_back( classifierNamen[k] );
              pRelation->classifierValue.push_back( classifierValues[k] );
            }
            pFeature->addFeatureRelation ( pRelation );
          }
        }
        else
        {
          if ( HREF != nullptr && pReadErrors != NULL )
          {
            stdString = "Verbotenes xlink bei Referenz auf Adresse: " + href;

            pReadError = new ReadError ( Error::ERROR, stdString, validReader->LineNumber );
            pReadErrors->addReadError ( pReadError );
          }
          readAddress ( pFeature, pActElement );
        }
        break;

			case _GEOMETRIE:
        if ( emptyElement )
        {
          if ( HREF != nullptr && HREF->Length > 1 )
          {
            if ( HREF[0] == '#' )
              HREF = HREF->Substring ( 1 );
            QuConvert::systemStr2stdStr ( stdString, HREF );

            GeometryReference * pGeometryReference = new GeometryReference ( pFeatures );
            pGeometryReference->setRefGeoId ( stdString );
            pGeometryReference->setRefGeoType( pGmlAttribut->getGmlAttributTypName() );

            pFeature->addGeometryProperty( gmlAttributNameAct, pGeometryReference );
        
          }
          break;
        }
        else
        {
          if ( HREF != nullptr && pReadErrors != NULL )
          {
            stdString = "Verbotenes xlink bei Referenz auf Geometrie: " + href;

            pReadError = new ReadError ( Error::ERROR, stdString,  validReader->LineNumber );
            pReadErrors->addReadError ( pReadError );
          }

          readFeatureGeometrie ( gmlAttributNameAct, pFeature, gmlAttributNameOrg, emptyElement );
        }
			  break;

      case _DIRECT_POSITION:
        readDirectPosition ( gmlAttributNameAct, pFeature, gmlAttributNameOrg );
        break;

      case _GENER_STRING:
        if ( pGmlSchema->isCityGml() )
        {
          gmlId =  Constant::createGmlId();
          objektTypLoc = ANY_OBJEKT;
          pGenericAttribute = new Feature ( objektTypLoc, gmlId, pFeatures, pSchemaTypeNamesStandard->genericStringAttribute ); 
          if ( validReader->AttributeCount > 0 )
          {
            STR = validReader->GetAttribute ( 0 );
            QuConvert::systemStr2stdStr ( stdString, STR );
            pGenericAttribute->setStringAttribut ( pSchemaTypeNamesStandard->genericNameAttribute, stdString );
          }

          while( validReader->Read() )
          {
            pActElementLoc = validReader->LocalName;
            if ( validReader->NodeType == XmlNodeType::Element )
            {
              if ( pActElementLoc == "value" )
              {
                STR = validReader->ReadString ();
                if ( STR != nullptr )
                {
                  QuConvert::systemStr2stdStr ( stdString, STR );
                  pGenericAttribute->setStringAttribut (  pSchemaTypeNamesStandard->genericValueAttribute, stdString );
                }
              }

            }
            else
            if ( validReader->NodeType == XmlNodeType::EndElement )
            {
              if ( pActElementLoc == "stringAttribute"  )
                break;
            }
          }

          QuConvert::systemStr2stdStr( dateiName, featureDateiName );
          pFeatures->addFeature( pGenericAttribute, dateiName );

          pRelation = pRelation = new FeatureRelation;
          pRelation->rollenName = "_GenericStringAttribute";
          pRelation->featureId = gmlId;
          pRelation->relToFeature = false;

          pFeature->addFeatureRelation ( pRelation );

          pGenericAttribute->setParentId( pFeature->getGmlId() );
          pFeature->addChild( gmlId );
        }
        break;


      case _GENER_DATE:
        if ( pGmlSchema->isCityGml() )
        {
          gmlId =  Constant::createGmlId();
          objektTypLoc = ANY_OBJEKT;
          pGenericAttribute = new Feature ( objektTypLoc, gmlId, pFeatures, pSchemaTypeNamesStandard->genericDateAttribute ); 
          if ( validReader->AttributeCount > 0 )
          {
            STR = validReader->GetAttribute ( 0 );
            QuConvert::systemStr2stdStr ( stdString, STR );
            pGenericAttribute->setStringAttribut ( pSchemaTypeNamesStandard->genericNameAttribute, stdString );
          }

          while( validReader->Read() )
          {
            pActElementLoc = validReader->LocalName;
            if ( validReader->NodeType == XmlNodeType::Element )
            {
              if ( pActElementLoc == "value" )
              {
                STR = validReader->ReadString ();
                if ( STR != nullptr )
                {
                  QuConvert::systemStr2stdStr ( stdString, STR );
                  TimePosition * pTimePos = new TimePosition;
                  pTimePos->fromXMLString( stdString );
                  DateAttr * pDateAttr = new DateAttr;
                  pDateAttr->value = pTimePos;
                  pGenericAttribute->setDateAttribut (  pSchemaTypeNamesStandard->genericValueAttribute, pDateAttr );
                }
              }
            }
            else
            if ( validReader->NodeType == XmlNodeType::EndElement )
            {
              if ( pActElementLoc == "dateAttribute" )
                break;
            }
          }

          QuConvert::systemStr2stdStr( dateiName, featureDateiName );
          pFeatures->addFeature( pGenericAttribute, dateiName );

          pRelation = pRelation = new FeatureRelation;
          pRelation->rollenName = "_GenericDateAttribute";
          pRelation->featureId = gmlId;
          pRelation->relToFeature = false;

          pFeature->addFeatureRelation ( pRelation );

          pGenericAttribute->setParentId( pFeature->getGmlId() );
          pFeature->addChild( gmlId );
        }
        break;


      case _GENER_URL:
        if ( pGmlSchema->isCityGml() )
        {
          gmlId =  Constant::createGmlId();
          pGenericAttribute = new Feature ( ANY_OBJEKT, gmlId, pFeatures, pSchemaTypeNamesStandard->genericUrlAttribute ); 
          if ( validReader->AttributeCount > 0 )
          {
            STR = validReader->GetAttribute ( 0 );
            QuConvert::systemStr2stdStr ( stdString, STR );
            pGenericAttribute->setStringAttribut ( pSchemaTypeNamesStandard->genericNameAttribute, stdString );
          }

          while( validReader->Read() )
          {
            pActElementLoc = validReader->LocalName;
            if ( validReader->NodeType == XmlNodeType::Element )
            {
              if ( pActElementLoc == "value" )
              {
                STR = validReader->ReadString ();
                if ( STR != nullptr )
                {
                  QuConvert::systemStr2stdStr ( stdString, STR );
                  pGenericAttribute->setURLAttribut ( pSchemaTypeNamesStandard->genericValueAttribute, stdString );
                }
              }
            }
            else
            if ( validReader->NodeType == XmlNodeType::EndElement )
            {
              if ( pActElementLoc == "uriAttribute" )
                break;
            }
          }

          QuConvert::systemStr2stdStr( dateiName, featureDateiName );
          pFeatures->addFeature( pGenericAttribute, dateiName );

          pRelation = pRelation = new FeatureRelation;
          pRelation->rollenName = "_GenericURLAttribute";
          pRelation->featureId = gmlId;
          pRelation->relToFeature = false;

          pFeature->addFeatureRelation ( pRelation );

          pGenericAttribute->setParentId( pFeature->getGmlId() );
          pFeature->addChild( gmlId );
        }
        break;


      case _GENER_INTEGER:
        if ( pGmlSchema->isCityGml() )
        {
          gmlId =  Constant::createGmlId();
          pGenericAttribute = new Feature ( ANY_OBJEKT, gmlId, pFeatures, pSchemaTypeNamesStandard->genericIntAttribute );
          if ( validReader->AttributeCount > 0 )
          {
            STR = validReader->GetAttribute ( 0 );
            QuConvert::systemStr2stdStr ( stdString, STR );
            pGenericAttribute->setStringAttribut ( pSchemaTypeNamesStandard->genericNameAttribute, stdString );
          }

          while( validReader->Read() )
          {
            pActElementLoc = validReader->LocalName;
            if ( validReader->NodeType == XmlNodeType::Element )
            {
              if ( pActElementLoc == "value" )
              {
                STR = validReader->ReadString ();
                if ( STR != nullptr )
                {
                  ih = XmlConvert::ToInt32 ( STR );
                  pGenericAttribute->setIntegerAttribut (  pSchemaTypeNamesStandard->genericValueAttribute, ih );
                }
              }
            }
            else
              if ( validReader->NodeType == XmlNodeType::EndElement )
              {
                if ( pActElementLoc == "intAttribute" )
                  break;
              }
          }

          QuConvert::systemStr2stdStr( dateiName, featureDateiName );
          pFeatures->addFeature( pGenericAttribute, dateiName );

          pRelation = pRelation = new FeatureRelation;
          pRelation->rollenName = "_GenericIntegerAttribute";
          pRelation->featureId = gmlId;
          pRelation->relToFeature = false;

          pFeature->addFeatureRelation ( pRelation );

          pGenericAttribute->setParentId( pFeature->getGmlId() );
          pFeature->addChild( gmlId );
        }
        break;

      case _GENER_DOUBLE:
        if ( pGmlSchema->isCityGml() )
        {
          gmlId =  Constant::createGmlId();
          pGenericAttribute = new Feature ( ANY_OBJEKT, gmlId, pFeatures, pSchemaTypeNamesStandard->genericDoubleAttribute );
          if ( validReader->AttributeCount > 0 )
          {
            STR = validReader->GetAttribute ( 0 );
            QuConvert::systemStr2stdStr ( stdString, STR );
            pGenericAttribute->setStringAttribut ( pSchemaTypeNamesStandard->genericNameAttribute, stdString );
          }

          while( validReader->Read() )
          {
            pActElementLoc = validReader->LocalName;
            if ( validReader->NodeType == XmlNodeType::Element )
            {
              if ( pActElementLoc == "value" )
              {
                STR = validReader->ReadString ();
                if ( STR != nullptr )
                {
                  h = XmlConvert::ToDouble ( STR );
                  pGenericAttribute->setDoubleAttribut (  pSchemaTypeNamesStandard->genericValueAttribute, h );
                }
              }
            }
            else
            if ( validReader->NodeType == XmlNodeType::EndElement )
            {
              if ( pActElementLoc == "doubleAttribute" )
                break;
            }
          }


          QuConvert::systemStr2stdStr( dateiName, featureDateiName );
          pFeatures->addFeature( pGenericAttribute, dateiName );

          pRelation = pRelation = new FeatureRelation;
          pRelation->rollenName = "_GenericDoubleAttribute";
          pRelation->featureId = gmlId;
          pRelation->relToFeature = false;

          pFeature->addFeatureRelation ( pRelation );

          pGenericAttribute->setParentId( pFeature->getGmlId() );
          pFeature->addChild( gmlId );
        }
        break;


      case _GENER_MEASURE:
        classifierNamen.clear();
        classifierValues.clear();
        pUOM = NULL;

        if ( pGmlSchema->isCityGml() )
        {
          gmlId =  Constant::createGmlId();
          pGenericAttribute = new Feature ( ANY_OBJEKT, gmlId, pFeatures, pSchemaTypeNamesStandard->genericMeasureAttribute );
          if ( validReader->AttributeCount > 0 )
          {
            STR = validReader->GetAttribute ( 0 );
            QuConvert::systemStr2stdStr ( stdString, STR );
            pGenericAttribute->setStringAttribut ( pSchemaTypeNamesStandard->genericNameAttribute, stdString );
          }

          while( validReader->Read() )
          {
            pActElementLoc = validReader->LocalName;
            if ( validReader->NodeType == XmlNodeType::Element )
            {
              if ( pActElementLoc == "value" )
              {
                STR = validReader->GetAttribute ( "uom" );
                if ( STR != nullptr )
                {
                  QuConvert::systemStr2stdStr ( uom, STR );
                  classifierNamen.push_back( "uom" );
                  classifierValues.push_back( uom );
                  pUOM = pFeatures->getUOMList()->getUOMFromIdentifier( uom );
                }
                else
                  pUOM = NULL;

                STR = validReader->ReadString ();
                if ( STR != nullptr )
                {
                  h = XmlConvert::ToDouble ( STR );
                  pGenericAttribute->setDoubleAttribut (  pSchemaTypeNamesStandard->genericValueAttribute, h, 
                                    classifierNamen, classifierValues, pUOM  );
                }
              }
            }
            else
            if ( validReader->NodeType == XmlNodeType::EndElement )
            {
              if ( pActElementLoc == "measureAttribute" )
                break;
            }
          }

          QuConvert::systemStr2stdStr( dateiName, featureDateiName );
          pFeatures->addFeature( pGenericAttribute, dateiName );

          pRelation = pRelation = new FeatureRelation;
          pRelation->rollenName = "_GenericMeasureAttribute";
          pRelation->featureId = gmlId;
          pRelation->relToFeature = false;

          pFeature->addFeatureRelation ( pRelation );

          pGenericAttribute->setParentId( pFeature->getGmlId() );
          pFeature->addChild( gmlId );
        }
        break;

      case _GENER_LIST:
        classifierNamen.clear();
        classifierValues.clear();
        pUOM = NULL;

        if ( pGmlSchema->isCityGml() )
        {
          gmlId =  Constant::createGmlId();
          pGenericAttribute = new Feature ( ANY_OBJEKT, gmlId, pFeatures, pSchemaTypeNamesStandard->genericAttributeSet );
          if ( validReader->AttributeCount > 0 )
          {
            STR = validReader->GetAttribute ( 0 );
            QuConvert::systemStr2stdStr ( stdString, STR );
            pGenericAttribute->setStringAttribut ( pSchemaTypeNamesStandard->genericNameAttribute, stdString );
          }

          while( validReader->Read() )
          {
            pActElement = validReader->Name;
            pActElementLoc = validReader->LocalName;
            if ( validReader->NodeType == XmlNodeType::Element )
            {
              if ( validReader->AttributeCount > 0 )
              {
                STR = validReader->GetAttribute ( 0 );
                QuConvert::systemStr2stdStr ( genericAttributName, STR );
              }

              END_TAG_LOC = pActElement;
              QuConvert::systemStr2stdStr ( gmlAttributNameAct, pActElement );
              gmlAttributNameAct = addDefaultNamespaceKuerzel ( gmlAttributNameAct );
              gmlAttributNameAct = createGmlNameStandard ( gmlAttributNameAct );

              while( validReader->Read() )
              {
                pActElementLoc = validReader->LocalName;
                if ( validReader->NodeType == XmlNodeType::Element )
                {
                  if ( pActElementLoc == "value" )
                  {
                    STR = validReader->GetAttribute( "uom" );
                    if ( STR != nullptr )
                    {
                      QuConvert::systemStr2stdStr ( uom, STR );
                      classifierNamen.push_back( "uom" );
                      classifierValues.push_back( uom );
                      pUOM = pFeatures->getUOMList()->getUOMFromIdentifier( uom );
                    }
                    else
                      pUOM = NULL;

                    GENERIC_ATTRIBUTE_VALUE = validReader->ReadString ();
                    if ( GENERIC_ATTRIBUTE_VALUE != nullptr )
                      QuConvert::systemStr2stdStr ( genericAttributValue, GENERIC_ATTRIBUTE_VALUE );
                    else
                      genericAttributValue = "";
                  }
                }
                else
                  if ( validReader->NodeType == XmlNodeType::EndElement )
                  {
                    if ( pActElement == END_TAG_LOC  )
                      break;
                  }
              }

              if ( gmlAttributNameAct == pSchemaTypeNamesStandard->genericStringAttribute )
                pGenericAttribute->setStringAttribut ( genericAttributName, genericAttributValue );
              else
              if ( gmlAttributNameAct == pSchemaTypeNamesStandard->genericIntAttribute  )
                pGenericAttribute->setIntegerAttribut ( genericAttributName, XmlConvert::ToInt32 ( GENERIC_ATTRIBUTE_VALUE ) );
              else
              if ( gmlAttributNameAct == pSchemaTypeNamesStandard->genericDoubleAttribute  )
                pGenericAttribute->setDoubleAttribut ( genericAttributName, XmlConvert::ToDouble ( GENERIC_ATTRIBUTE_VALUE ), 
                        classifierNamen, classifierValues, pUOM );
              else
              if ( gmlAttributNameAct == pSchemaTypeNamesStandard->genericDateAttribute )
              {
                TimePosition * pTimePosition = new TimePosition;
                pTimePosition->fromXMLString( genericAttributValue );
                DateAttr * pTime = new DateAttr;
                pTime->value = pTimePosition;
                pGenericAttribute->setDateAttribut ( genericAttributName, pTime );
              }
              else
              if ( gmlAttributNameAct == pSchemaTypeNamesStandard->genericUrlAttribute )
                pGenericAttribute->setURLAttribut ( genericAttributName, genericAttributValue );
              else
              if ( gmlAttributNameAct == pSchemaTypeNamesStandard->genericMeasureAttribute )
              {
                classifierValues[0] = classifierValue;
                pGenericAttribute->setDoubleAttribut ( genericAttributName, XmlConvert::ToDouble ( GENERIC_ATTRIBUTE_VALUE ), pUOM );
              }
            }
            else
            {
              if ( validReader->NodeType == XmlNodeType::EndElement )
              {
                if ( pActElementLoc == "genericAttributeSet"  )
                  break;
              }
            }
          }

          QuConvert::systemStr2stdStr( dateiName, featureDateiName );
          pFeatures->addFeature( pGenericAttribute, dateiName );

          pRelation = pRelation = new FeatureRelation;
          pRelation->rollenName = "_GenericAttributeSet";
          pRelation->featureId = gmlId;
          pRelation->relToFeature = false;

          pFeature->addFeatureRelation ( pRelation );

          pGenericAttribute->setParentId( pFeature->getGmlId() );
          pFeature->addChild( gmlId );
        }
        break;


			case _POS_XY:
        if (  gmlAttributNameAct  == "xplan:symbolPosition"  )
          skipElement( pActElementLoc );
				break;

			case _DATA_TYPE:
        gmlAttributTypName = pGmlAttribut->getGmlAttributTypName();
        pGmlReferenceTypeLoc =pGmlSchema->getGmlKlasse ( gmlAttributTypName );

        if ( nil )
        {
          pRelation = new FeatureRelation;
          pRelation->rollenName   = gmlAttributNameAct;
          pRelation->nil          = true;
          pRelation->nilReason    = nilReason;
          pRelation->relToFeature = false;
          for ( k = 0; k < classifierNamen.size(); k++ )
          {
            pRelation->classifierName.push_back( classifierNamen[k] );
            pRelation->classifierValue.push_back( classifierValues[k] );
          }
          pFeature->addFeatureRelation ( pRelation );
          break;
        }

        if ( gmlAttributNameAct == "app:appearance" ||
             gmlAttributNameAct == "app:appearanceMember" )
        {
          if ( validReader->IsEmptyElement == false )
          {
            readAppearance ( validReader->LocalName );
            continue;
          }
        }

        END_TAG = pActElement;

        if ( gmlAttributTypName == "core:ExternalReferenceType"        ||
          gmlAttributTypName == "citygml:ExternalReferenceType"     ||
          gmlAttributTypName == "core:ExternalObjectReferenceType"  ||
          gmlAttributTypName == "citygml:ExternalObjectReferenceType" )
        {
          pFeatureRelated = readFeature( END_TAG, pGmlReferenceTypeLoc, "" );
          if ( pFeatureRelated != NULL )
          {
            pFeatureRelated->setParentId( pFeature->getGmlId() );
            pFeature->addChild ( pFeatureRelated->getGmlId() );

            pRelation = new FeatureRelation;
            pRelation->rollenName = gmlAttributNameAct;
            pRelation->featureId = pFeatureRelated->getGmlId();
            pRelation->relToFeature = false;
 
            pFeature->addFeatureRelation ( pRelation );
          }
          continue;
        }

        STR = validReader->Value;
   
        if ( HREF != nullptr && HREF->Length > 1 )
        {
          if ( HREF[0] == '#' )
            HREF = HREF->Substring ( 1 );
          QuConvert::systemStr2stdStr ( stdString, HREF );

          pRelation = new FeatureRelation;
          pRelation->rollenName = gmlAttributNameAct;
          pRelation->featureId = stdString;
          for ( k = 0; k < classifierNamen.size(); k++ )
          {
            pRelation->classifierName.push_back( classifierNamen[k] );
            pRelation->classifierValue.push_back( classifierValues[k] );
          }
          pFeature->addFeatureRelation ( pRelation );
        }
        else
        {
          if ( emptyElement ) 
            break;
          readRelatedFeatures ( pFeature, pGmlReferenceTypeLoc, gmlAttributNameAct, classifierNamen, classifierValues, pActElement );
        }

			  break;

			case _REFERENZ:
        gmlAttributTypName = pGmlAttribut->getGmlAttributTypName();
        pGmlReferenceTypeLoc =pGmlSchema->getGmlKlasse ( gmlAttributTypName );

        if ( nil )
        {
          pRelation = new FeatureRelation;
          pRelation->rollenName   = gmlAttributNameAct;
          pRelation->nil          = true;
          pRelation->nilReason    = nilReason;
          pRelation->relToFeature = false;
          for ( k = 0; k < classifierNamen.size(); k++ )
          {
            pRelation->classifierName.push_back( classifierNamen[k] );
            pRelation->classifierValue.push_back( classifierValues[k] );
          }
          pFeature->addFeatureRelation ( pRelation );
          break;
        }


        if ( HREF != nullptr && HREF->Length > 1 )
        {
				  if ( HREF[0] == '#' )
					  HREF = HREF->Substring ( 1 );
				  QuConvert::systemStr2stdStr ( stdString, HREF );

				  pRelation = new FeatureRelation;
				  pRelation->rollenName = gmlAttributNameAct;
				  pRelation->featureId = stdString;
          for ( k = 0; k < classifierNamen.size(); k++ )
          {
            pRelation->classifierName.push_back( classifierNamen[k] );
            pRelation->classifierValue.push_back( classifierValues[k] );
          }
				 pFeature->addFeatureRelation ( pRelation );
        }
        else
        {
          if ( emptyElement ) 
            break;
          readRelatedFeatures ( pFeature, pGmlReferenceTypeLoc, gmlAttributNameAct, classifierNamen, classifierValues, pActElement );
        }

				break;

			case _DOUBLE:
			case _ANGLE:
			case _LENGTH:
			case _AREA:
			case _VOLUME:
      case _MEASURE:
        if ( nil )
        {
          DoubleAttr * pDoubleAttr = new DoubleAttr;
          pDoubleAttr->isNil = true;
          pDoubleAttr->setNilReason( nilReason );
          pFeature->setDoubleAttribut ( gmlAttributNameAct, pDoubleAttr );
        }
        else
        {
				  STR = validReader->ReadString();
				  if ( STR->CompareTo( "" ) != 0  || classifierNamen.size() > 0 )
				  {
            if (  STR->CompareTo( "" ) != 0  )  h = XmlConvert::ToDouble ( STR );
            else                                h = 0;
					  pFeature->setDoubleAttribut ( gmlAttributNameAct, h, classifierNamen, classifierValues, pUOM );
				  }
        }

			  break;

			case _INTEGER:
        if ( nil )
        {
          IntAttr * pIntAttr = new IntAttr;
          pIntAttr->isNil = true;
          pIntAttr->setNilReason( nilReason );
          pFeature->setIntegerAttribut ( gmlAttributNameAct, pIntAttr );
        }
        else
        {
				  STR = validReader->ReadString();
          if ( ( STR != nullptr && STR != "" ) || classifierNamen.size() > 0 )
				  {
					   if ( STR != nullptr && STR != "" ) ih = XmlConvert::ToInt32 ( STR );
            else                               ih = 0;
					  pFeature->setIntegerAttribut ( gmlAttributNameAct, ih, classifierNamen, classifierValues );
				  }
        }
				break;

			case _BOOLEAN:
        if ( nil )
        {
          BoolAttr * pBoolAttr = new BoolAttr;
          pBoolAttr->isNil = true;
          pBoolAttr->setNilReason( nilReason );
          pFeature->setBoolAttribut ( gmlAttributNameAct, pBoolAttr );
        }
        else
        {
          STR = validReader->ReadString();
          if ( ( STR != nullptr && STR != "" ) || classifierNamen.size() > 0 )
          {
            if ( STR != nullptr && STR != "" ) bh = XmlConvert::ToBoolean ( STR );
            else                               bh = false;
            pFeature->setBoolAttribut ( gmlAttributNameAct, bh, classifierNamen, classifierValues  );
          }
        }
				break;			

      case _MEASURE_LISTE:
      case _QUANTITY_EXTENT:
        if ( nil )
        {
          DoubleAttr * pDoubleAttr = new DoubleAttr;
          pDoubleAttr->isNil = true;
          pDoubleAttr->setNilReason( nilReason );
          pFeature->setDoubleAttribut ( gmlAttributNameAct, pDoubleAttr );
        }
        else
        {
          cli::array<String^> ^ split;
          double           d;

          STR = validReader->ReadString();
          if ( STR != nullptr && STR != "" )
          {
            STR = STR->Trim();
            split = STR->Split(delimiter, StringSplitOptions::RemoveEmptyEntries );
            for ( int j = 0; j < split->Length; j++)
            {
              d = XmlConvert::ToDouble( split[j] );
              pFeature->setDoubleAttribut ( gmlAttributNameAct, d, classifierNamen, classifierValues, pUOM );
            }
          }
        }
        break;


      case _STRING:
        if ( nil )
        {
          StringAttr * pStringAttr = new StringAttr;
          pStringAttr->isNil = true;
          pStringAttr->setNilReason( nilReason );
          pFeature->setStringAttribut ( gmlAttributNameAct, pStringAttr );
        }
        else
        {
				  STR = validReader->ReadString();
          if ( STR != nullptr && STR != "" ) QuConvert::systemStr2stdStr ( stdString, STR );
          else                               stdString = "";
				  pFeature->setStringAttribut ( gmlAttributNameAct, stdString, classifierNamen, classifierValues );
        }
        break;

      case _EXTERNAL_CODE_LIST_GML3_3:
        if ( nil )
        {
          StringAttr * pStringAttr = new StringAttr;
          pStringAttr->isNil = true;
          pStringAttr->setNilReason( nilReason );
          pFeature->setStringAttribut ( gmlAttributNameAct, pStringAttr );
          break;
        }

        if ( HREF != nullptr && HREF->Length > 0 )
        {
          cli::array<String^> ^ splitString;
          string           code;

          splitString = HREF->Split( '#' );
          if ( splitString->Length < 1 ) break;

          QuConvert::systemStr2stdStr( code, HREF );
          pFeature->setStringAttribut ( gmlAttributNameAct, code, classifierNamen, classifierValues );

          String ^ CODE_URI = splitString[0];
          string externalCodeListPath = generateExternalCodelistPath ( CODE_URI );

         
          if ( m_readCodeListDictionary                     && 
              pGmlSchema->externalCodeListDictionariesExist()     &&
              pGmlSchema->getCodeListsDictionary()->existEntryId( externalCodeListPath ) == false &&
              ( pGmlSchema->isInspire()              || 
                pGmlSchema->getGmlTyp() == INSPIRE_LU_PLANNED )      )
          {

            pDictionaryEntry = pDictionaryReader->readINSPIRECodeList( externalCodeListPath );
            if ( pDictionaryEntry != NULL )
            {
              pDictionaryEntry->setName( externalCodeListPath );
              pGmlAttribut->setGmlEnumeration( externalCodeListPath );

              pGmlSchema->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );
            }
          }
          break;
        }

      case _EXTERNAL_CODE_LIST:
        if ( nil )
        {
          StringAttr * pStringAttr = new StringAttr;
          pStringAttr->isNil = true;
          pStringAttr->setNilReason( nilReason );
          pFeature->setStringAttribut ( gmlAttributNameAct, pStringAttr );
          break;
        }

        STR = validReader->ReadString();
        if ( STR != nullptr && STR != "" ) QuConvert::systemStr2stdStr ( stdString, STR );
        else                               stdString = "";
        pFeature->setStringAttribut ( gmlAttributNameAct, stdString, classifierNamen, classifierValues );

        externalCodeListName = pGmlAttribut->getGmlEnumeration();

        if ( m_readCodeListDictionary && pGmlSchema->externalCodeListDictionariesExist() )
        {
          externalCodeListPath = "";
          for ( k = 0; k < classifierNamen.size(); k++ )
          {
            if ( classifierNamen[k] == "codeSpace" )
              externalCodeListPath = classifierValues[k];
          }
          if ( externalCodeListPath == "" && externalCodeListName != ""  )
          {
            if ( pGmlSchema->getGmlTyp() == CITYGML_2_0 )
              externalCodeListPath = pGmlSchema->createCityGML20CodeListPath ( pGmlKlasse, pGmlAttribut );
            else
              externalCodeListPath = pGmlSchema->createExternalCodeListsPath ( externalCodeListName );
            pGmlAttribut->setGmlEnumeration( externalCodeListPath );
          }

          if ( externalCodeListPath != ""  )
          {
            if ( pGmlSchema->getCodeListsDictionary()->existEntryId( externalCodeListPath ) == false  )
            {
              if ( pGmlSchema->getGmlTyp() == BOREHOLEML_3_0 )
              {
                pDictionaryEntry = pDictionaryReader->readBMLCodeList( externalCodeListPath );
                if ( pDictionaryEntry != NULL )
                {
                  pDictionaryEntry->setName( externalCodeListPath );
                  pGmlAttribut->setGmlEnumeration( externalCodeListPath );

                  pGmlSchema->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );
                }
              }
              else
              if ( pGmlSchema->isInspire()               || 
                   pGmlSchema->getGmlTyp() == INSPIRE_LU_PLANNED )
              {
                pDictionaryEntry = pDictionaryReader->readINSPIRECodeList( externalCodeListPath );
                if ( pDictionaryEntry != NULL )
                {
                  pDictionaryEntry->setName( externalCodeListPath );
                  pGmlAttribut->setGmlEnumeration( externalCodeListPath );

                  pGmlSchema->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );
                }
              }
              else
              {
                bool cityGMLSpecial;

                if ( pGmlSchema->getGmlTyp() == CITYGML_0_4_0 ||  pGmlSchema->getGmlTyp() == CITYGML_1_0 )
                  cityGMLSpecial = true;
                else
                  cityGMLSpecial = false;
                pDictionaryEntry = pDictionaryReader->readGmlSingleDictionary( externalCodeListPath, cityGMLSpecial );
                if ( pDictionaryEntry !=  NULL )
                {
                  pDictionaryEntry->setName( externalCodeListPath );

                  pGmlSchema->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );
                }
              }                       
            }
          }
        }
				break;

      case _ENUMERATION:
        if ( nil )
        {
          StringAttr * pStringAttr = new StringAttr;
          pStringAttr->isNil = true;
          pStringAttr->setNilReason( nilReason );
          pFeature->setStringAttribut ( gmlAttributNameAct, pStringAttr );
        }
        else
        {
          STR = validReader->ReadString();
          if ( STR != nullptr && STR != "" ) QuConvert::systemStr2stdStr ( stdString, STR );
          else                               stdString = "";
          pFeature->setStringAttribut ( gmlAttributNameAct, stdString, classifierNamen, classifierValues );
        }
        break;

			case _ANY_URI:
        if ( nil )
        {
          StringAttr * pStringAttr = new StringAttr;
          pStringAttr->isNil = true;
          pStringAttr->setNilReason( nilReason );
          pFeature->setURLAttribut ( gmlAttributNameAct, pStringAttr );
        }
        else
        {
				  STR = validReader->ReadString();
          if ( STR->CompareTo( "" ) != 0 ) QuConvert::systemStr2stdStr ( stdString, STR );
          else                             stdString = "";
				  pFeature->setURLAttribut ( gmlAttributNameAct, stdString, classifierNamen, classifierValues );
        }
				break;

			case _DATE:
      case _DATE_TIME:
      case TIME_POSITION:
        if ( nil )
        {
          DateAttr * pDateAttr = new DateAttr;
          pDateAttr->isNil = true;
          pDateAttr->setNilReason( nilReason );
          pFeature->setDateAttribut ( gmlAttributNameAct, pDateAttr );
        }
        else
        {
				  STR = validReader->ReadString();
          if ( STR != nullptr && STR != "" ) QuConvert::systemStr2stdStr ( stdString, STR );
          else                               stdString = "";

          if ( stdString != "" )
          {
            TimePosition * pTimePosition = new TimePosition;
            pTimePosition->fromXMLString( stdString );

            for ( unsigned int i = 0; i < classifierNamen.size(); i++ )
            {
              if ( classifierNamen[i] == "indeterminatePosition" )
                pTimePosition->setQualifier( classifierValues[i] );
            }
            DateAttr * pDateAttr = new DateAttr;
            pDateAttr->value = pTimePosition;
            pFeature->setDateAttribut ( gmlAttributNameAct, pDateAttr );
          }
        }
				break;

      case TIME_PERIOD:
        readTimePeriod ( gmlAttributNameAct, pFeature, gmlAttributNameOrg );
        break;

      case ABSTRACT_TIME_SERIES:
        readTimeSeries( gmlAttributNameAct, pFeature, gmlAttributNameOrg );
        break;

      case REGULAR_TIME_SERIES:
        pTimeSeries = readRegularTimeSeries ( gmlAttributNameAct, gmlAttributNameOrg );
        if ( pTimeSeries != NULL )
          pFeature->addTimeSeries ( gmlAttributNameAct, pTimeSeries );
        break;

      case IRREGULAR_TIME_SERIES:
        pTimeSeries = readIrregularTimeSeries ( gmlAttributNameAct, gmlAttributNameOrg );
        if ( pTimeSeries != NULL )
          pFeature->addTimeSeries ( gmlAttributNameAct, pTimeSeries );
        break;

      case REGULAR_TIME_SERIES_FILE:
        pTimeSeries = readRegularTimeSeriesFile ( gmlAttributNameAct, gmlAttributNameOrg );
        if ( pTimeSeries != NULL )
          pFeature->addTimeSeries ( gmlAttributNameAct, pTimeSeries );
        break;

      case IRREGULAR_TIME_SERIES_FILE:
        pTimeSeries = readIrregularTimeSeriesFile ( gmlAttributNameAct, gmlAttributNameOrg );
        if ( pTimeSeries != NULL )
          pFeature->addTimeSeries ( gmlAttributNameAct, pTimeSeries );
        break;

			default:
        errorMessage = "Unbekannte Feature-Property: " + gmlAttributNameAct;
        pFeatures->getLogfile()->writeLogFile( errorMessage );
     
        if ( pReadErrors != NULL )
        {
          pReadError = new ReadError ( Error::ERROR, errorMessage, validReader->LineNumber );
          pReadErrors->addReadError ( pReadError );
        }

        if ( !emptyElement )
          skipElement( pActElementLoc );
				break;	
			}
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->Name;
			if (pActElement->CompareTo( pObjektNameP ) == 0 ) break;
		}
	}

  QuConvert::systemStr2stdStr( dateiName, featureDateiName );
  pFeatures->addFeature ( pFeature, dateiName );
	return pFeature;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest ein GML-Feature, das in ein anderes GML-Feature eingebettet ist    //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readRelatedFeatures ( Feature *  pParent, GmlKlasse * pGmlReferenceType, string gmlAttributName, 
                                      vector<string> classifierNamen, vector<string> classifierValues, String ^ END_TAG )
{
  string            dateiName;
  Feature         * pRelatedFeature;
  FeatureRelation * pRelation;
  unsigned int      k;

  while( validReader->Read() )
  {
    pActElement = validReader->Name;
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pRelatedFeature = readFeature ( pActElement, pGmlReferenceType, "" );
      if ( pRelatedFeature != NULL )
      {
        pRelatedFeature->setParentId( pParent->getGmlId() );
        pParent->addChild ( pRelatedFeature->getGmlId() );

        pRelation = new FeatureRelation;
        pRelation->rollenName = gmlAttributName;
        pRelation->featureId = pRelatedFeature->getGmlId();
        pRelation->relToFeature = false;
        for ( k = 0; k < classifierNamen.size(); k++ )
        {
          pRelation->classifierName.push_back( classifierNamen[k] );
          pRelation->classifierValue.push_back( classifierValues[k] );
        }
        pParent->addFeatureRelation ( pRelation );
      }
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      if ( pActElement->CompareTo( END_TAG ) == 0 ) break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Einlesen einer xAl Adresse                                               //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readAddress ( Feature * pParent, String ^  GML_TAG )
{
  String   ^ STR;
  string     stdString;
  string     dateiName;
  Feature  * pAddress;
  string     addressTag;
  string     gmlId;

  STR = validReader->GetAttribute ( gmlIdTag );
  if ( STR == nullptr )
    gmlId = Constant::createGmlId();
  else
    QuConvert::systemStr2stdStr ( gmlId, STR );

  if (pGmlSchema->getGmlTyp() == CITYGML_1_0 || 
      pGmlSchema->getGmlTyp() == CITYGML_2_0 || 
      pGmlSchema->getGmlTyp() == CITYGML_3_0  )
    addressTag = "core:xalAddress";

  if (pGmlSchema->getGmlTyp() == CITYGML_0_4_0 )
    addressTag = "citygml:xalAddress";

  pAddress = new Feature( ANY_OBJEKT, gmlId, pFeatures, addressTag );

  if ( validReader->IsEmptyElement ) return;

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement = validReader->LocalName;
      if ( pActElement->CompareTo ( "streetName" ) == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAddress->setStringAttribut ( "xAL:streetName", stdString );
        }
      }
      else
      if ( pActElement->CompareTo ( "city" ) == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAddress->setStringAttribut ( "xAL:city", stdString );
        }
      }
      else
      if ( pActElement->CompareTo ( "zipCode" ) == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAddress->setStringAttribut ( "xAL:zipCode", stdString );
        }
      }
      else
      if ( pActElement->CompareTo ( "houseNumber" ) == 0 )
      {
         STR = validReader->ReadString();
         if ( STR != nullptr )
         {
           QuConvert::systemStr2stdStr ( stdString, STR );
           pAddress->setStringAttribut ( "xAL:houseNumber", stdString );
         }
      }
      else
      if ( pActElement->CompareTo ( "CountryName" ) == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAddress->setStringAttribut ( "xAL:CountryName", stdString );
        }
      }
      else
      if ( pActElement->CompareTo ( "LocalityName" ) == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAddress->setStringAttribut ( "xAL:LocalityName", stdString );
        }
      }
      else
      if ( pActElement->CompareTo ( "PostBoxNumber" ) == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAddress->setStringAttribut ( "xAL:PostBoxNumber", stdString );
        }
      }
      else
      if ( pActElement->CompareTo ( "ThoroughfareNumber" ) == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAddress->setStringAttribut ( "xAL:ThoroughfareNumber", stdString );
        }
      }
      else
      if ( pActElement->CompareTo ( "ThoroughfareName" ) == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAddress->setStringAttribut ( "xAL:ThoroughfareName", stdString );
        }
      }
      else
      if ( pActElement->CompareTo ( "PostalCodeNumber" ) == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
           QuConvert::systemStr2stdStr ( stdString, STR );
           pAddress->setStringAttribut ( "xAL:PostalCodeNumber", stdString );
        }
        else
        if ( pActElement->CompareTo ( "core:multiPoint" ) == 0 )
        {
          pFeatures->getLogfile()->writeLogFile ( "!!! Multi-Punkt noch nicht unterstützt:" );
        }
      }
    }
    else
      if ( validReader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = validReader->Name;
        if (pActElement->CompareTo( GML_TAG ) == 0 ) break;
      }
  }
  FeatureRelation * pRel = new FeatureRelation;
  pRel->rollenName = "Address";
  pRel->featureId = pAddress->getGmlId();
  pRel->relToFeature = false;
  pParent->addFeatureRelation ( pRel );

  QuConvert::systemStr2stdStr( dateiName, featureDateiName );
  pFeatures->addFeature ( pAddress, dateiName );

  pParent->addChild( pAddress->getGmlId() );
  pAddress->setParentId( pParent->getGmlId() );
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Geometrie													                          	 //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readFeatureGeometrie ( string geometryPropertyName, Feature * pObjP, string gmlTagP, bool emptyElement )
{
  double              sw[3], no[3];
  string              stdString;
  string              srsName;
  int                 srsDimension;
	String            ^ GML_TAG;
	MultiCurve        * pMultiCurve;
	MultiPoint        * pMultiPoint;
  vector<ArcSegment*> vArcSegments;
  unsigned int        i;
  bool                success;

  srsName = pObjP->getSrsName();
  if ( srsName =="" )
    srsName = pFeatures->getSrsName();

  srsDimension = pObjP->getSrsDimension();
  if ( srsDimension < 0 )
    srsDimension = pFeatures->getSrsDimension();

	GML_TAG = QuConvert::ToString ( gmlTagP );

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

      if ( validReader->IsEmptyElement ) continue;

      if ( pActElement->CompareTo("Envelope") == 0 )
      {
        string srsNameLoc;
        int    srsDimensionLoc;

        success = readEnvelope(  sw, no, srsNameLoc, srsDimensionLoc );
        if ( success )
        {
          if ( srsDimensionLoc > 0 )
            pObjP->setSrsDimension( srsDimensionLoc );

          if ( srsNameLoc != "" )
             pObjP->setSrsName( srsNameLoc );
        }
      }
      else
			if ( pActElement->CompareTo("Polygon") == 0 )
			{
				PolygonGeo * pPoly = readPolygon ( pActElement, srsName, srsDimension );
				if ( pPoly != NULL )
					pObjP->addGeometryProperty ( geometryPropertyName, pPoly );
			}
			else
        if ( pActElement->CompareTo("Surface") == 0       ||
            pActElement->CompareTo("TriangulatedSurface") == 0 )
			{
				Surface * pSurface = readSurface ( pActElement, srsName, srsDimension );
				if ( pSurface != NULL )
					pObjP->addGeometryProperty ( geometryPropertyName, pSurface );
			}
			else
			if ( pActElement->CompareTo("MultiSurface") == 0 ||
           pActElement->CompareTo("CompositeSurface") == 0  )
			{
				MultiSurface * pMultiSurface = readMultiSurface ( pActElement, srsName, srsDimension );
        if ( pMultiSurface != NULL  )
				  pObjP->addGeometryProperty ( geometryPropertyName, pMultiSurface );
			}
			else
			if ( pActElement->CompareTo("Curve") == 0 )
			{
				Curve * pCurve = readCurve ( pActElement, srsName, srsDimension );
				if ( pCurve != NULL )
					pObjP->addGeometryProperty ( geometryPropertyName, pCurve );
			}
			else
			if ( pActElement->CompareTo("MultiCurve") == 0 )
			{
				pMultiCurve = readMultiCurve ( pActElement, srsName, srsDimension );
				if ( pMultiCurve != NULL )
					pObjP->addGeometryProperty ( geometryPropertyName, pMultiCurve );
			}
      else
      if ( pActElement->CompareTo("CompositeCurve") == 0 )
      {
        pMultiCurve = readMultiCurve ( pActElement, srsName, srsDimension );
        if ( pMultiCurve != NULL )
        {
          pMultiCurve->setIsCompositeCurve ( true );
          pObjP->addGeometryProperty ( geometryPropertyName, pMultiCurve );
        }
      }
			else
			if (  pActElement->CompareTo("LineString") == 0  )
			{
				LineSegment * pLineSegment = readLineSegment ( pActElement, srsName, srsDimension, LineSegment::_LINE_STRING );
				if ( pLineSegment != NULL )
					pObjP->addGeometryProperty ( geometryPropertyName, pLineSegment );
			}
			else
			if ( pActElement->CompareTo("Arc") == 0 )
			{
        vArcSegments.clear();
				readArcSegment ( pActElement, ArcSegment::_ARC, vArcSegments, srsName, srsDimension );
				if ( vArcSegments.size() > 0 )
				{
					pMultiCurve = new MultiCurve( srsDimension );
          for ( i = 0; i < vArcSegments.size(); i++ )
					  pMultiCurve->addCurve ( vArcSegments[i] );
          srsName = vArcSegments[0]->getSrsName();
          if ( srsName != "" ) pMultiCurve->setSrsName ( srsName );
					pObjP->addGeometryProperty ( geometryPropertyName, pMultiCurve );
				}
			}
      else
        if ( pActElement->CompareTo("ArcString") == 0 )
        {
          vArcSegments.clear();
          readArcSegment ( pActElement, ArcSegment::_ARC_STRING, vArcSegments, srsName, srsDimension );
          if ( vArcSegments.size() > 0 )
          {
            pMultiCurve = new MultiCurve( srsDimension );
            for ( i = 0; i < vArcSegments.size(); i++ )
              pMultiCurve->addCurve ( vArcSegments[i] );
            srsName = vArcSegments[0]->getSrsName();
            if ( srsName != "" ) pMultiCurve->setSrsName ( srsName );
            pObjP->addGeometryProperty ( geometryPropertyName, pMultiCurve );
          }
        }
			else
			if (  pActElement->CompareTo("Circle") == 0   )
			{
        vArcSegments.clear();
				readArcSegment ( pActElement, ArcSegment::_CIRCLE, vArcSegments, srsName, srsDimension );
				if (  vArcSegments.size() > 0  )
				{
					pMultiCurve = new MultiCurve( srsDimension );
					pMultiCurve->addCurve ( vArcSegments[0] );
          srsName = vArcSegments[0]->getSrsName();
          if ( srsName != "" ) pMultiCurve->setSrsName ( srsName );
					pObjP->addGeometryProperty ( geometryPropertyName, pMultiCurve );
				}
			}
			else
			if ( pActElement->CompareTo("Point") == 0 )
			{
				GeoPoint * pPoint =  readPoint ( pActElement, srsName, srsDimension );
				if ( pPoint != NULL )
				{
          if ( gmlTagP == "xplan:symbolPosition" )
						pObjP->addSymbolPosition ( pPoint );
					else
						pObjP->addGeometryProperty ( geometryPropertyName, pPoint );
				}
			}
			else
			if ( pActElement->CompareTo("MultiPoint") == 0 )
			{
				pMultiPoint = readMultiPoint ( pActElement, srsName, srsDimension );
				if ( pMultiPoint != NULL )
					pObjP->addGeometryProperty ( geometryPropertyName, pMultiPoint );
			}
			else
      if ( pActElement->CompareTo("MultiGeometry") == 0 )
      {
        MultiGeometry * pMultiGeometry = readMultiGeometry ( pActElement, srsName, srsDimension );
        if ( pMultiGeometry != NULL )
          pObjP->addGeometryProperty ( geometryPropertyName, pMultiGeometry );
      }
      else
      if ( pActElement->CompareTo("GeometricComplex") == 0 )
      {
        GeometricComplex * pGeometricComplex = readGeometricComplex ( pActElement, srsName, srsDimension );
        if ( pGeometricComplex != NULL )
          pObjP->addGeometryProperty ( geometryPropertyName, pGeometricComplex );
      }
      else
      if ( pActElement->CompareTo("Solid") == 0 )
      {
        Solid * pSolid = readSolid ( pActElement, srsName, srsDimension );
        if ( pSolid != NULL )
          pObjP->addGeometryProperty ( geometryPropertyName, pSolid );
      }
      else
      if ( pActElement->CompareTo("MultiSolid") == 0 )
      {
        MultiSolid * pMultiSolid = readMultiSolid ( pActElement, srsName, srsDimension );
        if ( pMultiSolid != NULL )
          pObjP->addGeometryProperty ( geometryPropertyName, pMultiSolid );
      }
      else
      if ( pActElement->CompareTo("CompositeSolid") == 0 )
      {
         MultiSolid * pMultiSolid = readMultiSolid ( pActElement, srsName, srsDimension );
         if ( pMultiSolid != NULL )
         {
           pMultiSolid->setIsCompositeSolid( true );
           pObjP->addGeometryProperty ( geometryPropertyName, pMultiSolid );
         }
      }
      else
      if ( pActElement->CompareTo( "OrientableSurface" ) == 0 ||
           pActElement->CompareTo( "TexturedSurface" ) == 0 )
      {
        OrientableSurface * pGeometrie = readOrientableSurface( pActElement, srsName, srsDimension );
        if ( pGeometrie != NULL  )
          pObjP->addGeometryProperty ( geometryPropertyName, pGeometrie );
      }
      else
      if ( pActElement->CompareTo("ImplicitGeometry") == 0 )
      {
        ImplicitGeometry * pGeometry = readImplicitGeometry ( pActElement, srsName, srsDimension );
        if ( pGeometry != NULL )
          pObjP->addGeometryProperty ( geometryPropertyName, pGeometry );
      }
      else
      {
				writeGmlError ( "Unbekannte GML-Geometrie", pActElement );
        if ( !validReader->IsEmptyElement )
          skipElement( pActElement );
      }
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->Name;
			if (pActElement->CompareTo( GML_TAG ) == 0 ) break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen gml:DirectPosition                                              //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readDirectPosition ( std::string geometryPropertyName, Feature * pObjP, std::string gmlTagP )
{
  String         ^ STR;
  string           stdString;
  int              i, srsDimension;
  double           h;
  vector<double>   vC;
  cli::array<String^> ^ split;
  DirectPosition * pDirectPosition;

  pDirectPosition = new DirectPosition;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pDirectPosition->setSrsName( stdString );
  }

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );
  else
    srsDimension = 0;

  STR = validReader->ReadString();
  if (STR->Length != 0)
  {
    STR = STR->Trim();
    split = STR->Split( delimiter, StringSplitOptions::RemoveEmptyEntries );
    for ( i = 0; i < split->Length; i++)
    {
      h = XmlConvert::ToDouble( split[i] );
      vC.push_back( h );
    }
  }
  pDirectPosition->set( vC );
  pObjP->addGeometryProperty ( geometryPropertyName, pDirectPosition );
}


///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines GML-Geometrieobjektes                                     //
///////////////////////////////////////////////////////////////////////////////
_Geometrie * GMLReader::readGeometrie ( System::String ^ GML_TAG, string srsName, int srsDimension )
{
  _Geometrie        * pGeometrie = NULL;
  vector<ArcSegment*> vArcSegments;

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement = validReader->LocalName;

      if ( pActElement->CompareTo("Envelope") == 0 )
      {
        skipElement ( pActElement );
      }
      else
      if ( pActElement->CompareTo("Polygon") == 0 || 
           pActElement->CompareTo("Triangle") == 0 )
      {
        pGeometrie = readPolygon ( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo("Surface") == 0  ||
           pActElement->CompareTo("TriangulatedSurface") == 0)
      {
        pGeometrie = readSurface ( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo("MultiSurface") == 0 ||
           pActElement->CompareTo("CompositeSurface") == 0 )
      {
        pGeometrie = readMultiSurface ( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo("Curve") == 0 )
      {
        pGeometrie = readCurve ( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo("MultiCurve") == 0 )
      {
        pGeometrie = readMultiCurve ( pActElement, srsName, srsDimension );
      }
      else
      if (  pActElement->CompareTo("LineString") == 0  )
      {
        pGeometrie = readLineSegment ( pActElement, srsName, srsDimension, LineSegment::_LINE_STRING );
      }
      else
      if ( pActElement->CompareTo("Point") == 0 )
      {
        GeoPoint * pPoint =  readPoint ( pActElement, srsName, srsDimension );
        return pPoint;
      }
      else
      if ( pActElement->CompareTo("MultiPoint") == 0 )
      {
        pGeometrie = readMultiPoint ( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo("EX_BoundingPolygon") == 0 )
      {
     
      }
      else
      if ( pActElement->CompareTo("Solid") == 0 )
      {
        pGeometrie = readSolid( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo("MultiSolid") == 0 )
      {
        pGeometrie = readMultiSolid( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo("MultiGeometry") == 0 )
      {
        pGeometrie = readMultiGeometry( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo("ImplicitGeometry") == 0 )
      {
        pGeometrie = readImplicitGeometry( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo( "OrientableSurface" ) == 0 ||
           pActElement->CompareTo( "TexturedSurface" ) == 0 )
      {
        pGeometrie = readOrientableSurface( pActElement, srsName, srsDimension );
      }
      else
      {
        writeGmlError ( "Unbekannte GML-Geometrie", pActElement );
        skipElement( pActElement );
      }
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = validReader->LocalName;
      if (pActElement->CompareTo( GML_TAG ) == 0 ) break;
    }
  }
  return pGeometrie;

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer MultiSurface											                      	 //
///////////////////////////////////////////////////////////////////////////////
MultiSurface *  GMLReader::readMultiSurface ( System::String ^ gmlTag, string srsName, int srsDimension )
{
	String		   ^ STR;
	string		     stdString;
	_Surface     * pSurface;
	MultiSurface * pMultiSurface = nullptr;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32( STR );

  if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

  if ( gmlTag->CompareTo("MultiSurface") == 0 )
	  pMultiSurface = new MultiSurface ( srsDimension );
  else
  if ( gmlTag->CompareTo("CompositeSurface") == 0 )
    pMultiSurface = new CompositeSurface ( srsDimension );
  else
  if ( gmlTag->CompareTo("Shell") == 0 )
    pMultiSurface = new Shell ( srsDimension );

  pMultiSurface->setSrsName ( srsName );


	STR = validReader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
  {
		QuConvert::systemStr2stdStr ( stdString, STR );
    pAppearances->addGeometry ( stdString, pMultiSurface );

  }
	else
		stdString = Constant::createGmlId();

	pMultiSurface->setGmlId ( stdString );

  pFeatures->addGeometry( pMultiSurface );

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo("surfaceMember") == 0   )
			{
        STR = validReader->GetAttribute ( "xlink:href" );
        if ( STR != nullptr )
        {
          if ( STR[0] == '#' ) STR = STR->Substring ( 1 );
          QuConvert::systemStr2stdStr ( stdString, STR );

          SurfaceReference * pSurfaceReference = new SurfaceReference ( pFeatures );
          pSurfaceReference->setRefGeoId ( stdString );
          pMultiSurface->addSurface ( pSurfaceReference );
        }
        else
				if ( !validReader->IsEmptyElement )
				{
					pSurface = readSurfaceMember ( pActElement, srsName, srsDimension );
					if ( pSurface != NULL )
          {
            pMultiSurface->addSurface ( pSurface );
            pMultiSurface->setDim( pSurface->getDim() );
          }
				}
			}
      else
      if ( pActElement->CompareTo("surfaceMembers") == 0   )
      {
        readSurfaceMembers ( pActElement, srsName, srsDimension, pMultiSurface );
      }

		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}

  return pMultiSurface;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines Members einer gml:MultiSurface							               //
///////////////////////////////////////////////////////////////////////////////
_Surface * GMLReader::readSurfaceMember ( System::String ^ gmlTag, string srsName, int srsDimension )
{
	_Surface * pSurface = NULL;

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo ("Polygon") == 0 ||
           pActElement->CompareTo ("Triangle") == 0 )
			{
				pSurface = readPolygon ( pActElement, srsName, srsDimension );
			}
			else
			if ( pActElement->CompareTo("Surface") == 0 ||
           pActElement->CompareTo("TriangulatedSurface") == 0 )
			{
				if ( !validReader->IsEmptyElement )
					pSurface = readSurface ( pActElement, srsName, srsDimension );
			}
      else
      if ( pActElement->CompareTo("MultiSurface") == 0  ||
           pActElement->CompareTo("CompositeSurface") == 0 ||
           pActElement->CompareTo("Shell") == 0 )
      {
        if ( !validReader->IsEmptyElement )
          pSurface = readMultiSurface ( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo( "OrientableSurface" ) == 0 ||
        pActElement->CompareTo( "TexturedSurface" ) == 0 )
      {
        pSurface = readOrientableSurface( pActElement, srsName, srsDimension );
      }
			else
			{
				writeGmlError (  "Unbekannte GML-Geometrie", pActElement );
			}

		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}
	return pSurface;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen von gml:SurfaceMembers                                          //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readSurfaceMembers ( System::String ^ gmlTag, string srsName, int srsDimension, MultiSurface * pMultiSurface )
{
  _Surface * pSurface = NULL;

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement = validReader->LocalName;
      pSurface = NULL;

      if ( pActElement->CompareTo ("Polygon") == 0 ||
        pActElement->CompareTo ("Triangle") == 0 )
      {
        pSurface = readPolygon ( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo("Surface") == 0 ||
        pActElement->CompareTo("TriangulatedSurface") == 0 )
      {
        if ( !validReader->IsEmptyElement )
          pSurface = readSurface ( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo("MultiSurface") == 0  ||
           pActElement->CompareTo("CompositeSurface") == 0 ||
           pActElement->CompareTo("Shell") == 0 )
      {
        if ( !validReader->IsEmptyElement )
          pSurface = readMultiSurface ( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo( "OrientableSurface" ) == 0 ||
             pActElement->CompareTo( "TexturedSurface" ) == 0 )
      {
        pSurface = readOrientableSurface( pActElement, srsName, srsDimension );
      }
      else
      {
        writeGmlError (  "Unbekannte GML-Geometrie", pActElement );
      }

      if ( pSurface != NULL )
        pMultiSurface->addSurface( pSurface );
    }
    else
      if ( validReader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = validReader->LocalName;
        if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
      }
  }
  
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines Members einer gml:MultiSolid	  						               //
///////////////////////////////////////////////////////////////////////////////
_Solid * GMLReader::readSolidMember ( System::String ^ gmlTag, string srsName, int srsDimension )
{
  _Solid * pSolid = NULL;

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement = validReader->LocalName;

      if ( pActElement->CompareTo ("Solid") == 0 )
      {
        pSolid = readSolid ( pActElement, srsName, srsDimension );
      }
      else
      if ( pActElement->CompareTo("CompositeSolid") == 0  )
      {
        if ( !validReader->IsEmptyElement )
          pSolid = readMultiSolid ( pActElement, srsName, srsDimension );
      }
      else
      {
        writeGmlError (  "Unbekannte GML-Geometrie", pActElement );
      }
    }
    else
      if ( validReader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = validReader->LocalName;
        if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
      }
  }
  return pSolid;
}


///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer gml:Surface											                      	 //
///////////////////////////////////////////////////////////////////////////////
Surface * GMLReader::readSurface (  System::String ^ gmlTag, string srsName, int srsDimension  )
{
	String  ^ STR;
	string    stdString;
	PolygonGeo * pPoly;
  Surface * pSurface = nullptr;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );

  if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

 if ( gmlTag->CompareTo("Surface") == 0 )
	  pSurface = new Surface ( srsDimension );
  else
  if ( gmlTag->CompareTo("TriangulatedSurface") == 0 )
    pSurface = new TriangulatedSurface ( srsDimension );

  pSurface->setSrsName( srsName );


	STR = validReader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
  {
		QuConvert::systemStr2stdStr ( stdString, STR );
    pAppearances->addGeometry ( stdString, pSurface );
  }
	else
		stdString = Constant::createGmlId();
	pSurface->setGmlId ( stdString );

  pFeatures->addGeometry( pSurface );

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

      if ( pActElement->CompareTo("patches") == 0 )
      {

      }
      else
      if ( pActElement->CompareTo("trianglePatches") == 0 )
      {

      }
      else
			if ( pActElement->CompareTo("PolygonPatch") == 0 )
			{
				pPoly = readPolygon ( pActElement, srsName, srsDimension );
				if ( pPoly != NULL )  pSurface->addPolygon ( pPoly );
			}
      else
      if ( pActElement->CompareTo("Triangle") == 0 )
      {
        pPoly = readPolygon ( pActElement, srsName, srsDimension  );
        if ( pPoly != NULL )  pSurface->addPolygon ( pPoly );
      }
			else
				writeGmlError ( "Unbekannte GML-Geometrie", pActElement );
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}

	if ( pSurface->getPolygonAnz() == 0 )
	{
		delete pSurface;
		return NULL;
	}
	else
  {
		return pSurface;
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines gml:Polygon oder gml:PolygonPatch					            	 //
///////////////////////////////////////////////////////////////////////////////
PolygonGeo * GMLReader::readPolygon ( System::String ^ gmlTag, string srsName, int srsDimension  )
{
	String  ^ STR;
	string    stdString;
	_Ring   * pRing;
  PolygonGeo * pPoly = nullptr;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );

  if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

  if ( gmlTag->CompareTo("Polygon") == 0 || 
       gmlTag->CompareTo("PolygonPatch") == 0  )
	  pPoly = new PolygonGeo ( srsDimension );
  else
  if ( gmlTag->CompareTo("Triangle") == 0 )
    pPoly = new Triangle ( srsDimension );

  pPoly->setSrsName( srsName );

	STR = validReader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
  {
		QuConvert::systemStr2stdStr ( stdString, STR );
    pAppearances->addGeometry ( stdString, pPoly );
  }
	else
		stdString = Constant::createGmlId();
	pPoly->setGmlId ( stdString );

  pFeatures->addGeometry( pPoly );

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo("exterior") == 0 )
			{
				pRing = readRing ( pActElement, srsName, srsDimension );
				if ( pRing != NULL )
        {
          pPoly->setAussenkontur ( pRing );
          pPoly->setDim ( pRing->getDim() );
        }
			}
			else
			if ( pActElement->CompareTo("interior") == 0 )
			{
				pRing = readRing ( pActElement, srsName, srsDimension );
				if ( pRing != NULL ) pPoly->addLoch ( pRing );
			}
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}
	return pPoly;

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines Ringes												                          	 //
///////////////////////////////////////////////////////////////////////////////
_Ring * GMLReader::readRing ( String ^  gmlTag, string srsName, int srsDimension )
{
	string        stdString;
  string        gmlId;
  String      ^ STR;
	_Ring       * pRing = NULL;
	LineSegment * pLineSegment;
	MultiCurve	* pMultiCurve;

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo("LinearRing") == 0  )
			{
				pRing = new LinearRing ( srsDimension );

        STR = validReader->GetAttribute ( _T("gml:id") );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr ( gmlId, STR );
        else
          gmlId = Constant::createGmlId();

        pAppearances->addGeometry ( gmlId, pRing );
        pRing->setGmlId ( gmlId );

				pLineSegment = readLineSegment ( pActElement, srsName, srsDimension, LineSegment::_LINEAR_RING );
				if ( pLineSegment != NULL )
        {
          ((LinearRing*)pRing)->setLineSegment ( pLineSegment );
          pRing->setDim ( pLineSegment->getDim() );
        }
			}
			else
			if ( pActElement->CompareTo("Ring") == 0 )
			{
				pRing = new Ring( srsDimension );

        STR = validReader->GetAttribute ( _T("gml:id") );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr ( gmlId, STR );
        else
          gmlId = Constant::createGmlId();

        pRing->setGmlId ( gmlId );
        pAppearances->addGeometry ( gmlId, pRing );

				pMultiCurve = readMultiCurve ( pActElement, srsName, srsDimension );
				if ( pMultiCurve != NULL )
        {
          ((Ring*)pRing)->setRingGeometrie ( pMultiCurve );
          pRing->setDim( pMultiCurve->getDim() );
        }
			}
			else
				writeGmlError ( "Unbekannte GML-Geometrie", pActElement );	
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}

	return pRing;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer MultiCurve											                        	 //
///////////////////////////////////////////////////////////////////////////////
MultiCurve * GMLReader::readMultiCurve (  String ^  gmlTag, string srsName, int srsDimension )
{
	String     ^ STR;
	string       stdString;
	_Curve     * pCurve = nullptr;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );

  if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

	MultiCurve * pMultiCurve = new MultiCurve ( srsDimension );
  pMultiCurve->setSrsName( srsName );

  STR = validReader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
  {
		QuConvert::systemStr2stdStr ( stdString, STR );
    pAppearances->addGeometry ( stdString, pMultiCurve );
  }
	else
		stdString = Constant::createGmlId();
  pMultiCurve->setGmlId ( stdString );
	

  pFeatures->addGeometry ( pMultiCurve );

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo("curveMember") == 0   )
			{
        STR = validReader->GetAttribute ( "xlink:href" );
        if ( STR != nullptr )
        {
          if ( STR[0] == '#' ) STR = STR->Substring ( 1 );
          QuConvert::systemStr2stdStr ( stdString, STR );

          CurveReference * pCurveReference = new CurveReference ( pFeatures );
          pCurveReference->setRefGeoId ( stdString );
          pMultiCurve->addCurve ( pCurveReference );
        }
        else

				pCurve = readCurveMember ( pActElement, srsName, srsDimension );
				if ( pCurve != NULL )
        {
					pMultiCurve->addCurve ( pCurve );
          pMultiCurve->setDim( pCurve->getDim() );
        }
			}
      else
      if ( pActElement->CompareTo("curveMembers") == 0   )
      {
        readCurveMembers ( pActElement, srsName, srsDimension, pMultiCurve );
      }


		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}

	if ( pMultiCurve->getCurveMemberAnz() == 0 )
	{
		delete pMultiCurve;
		return NULL;
	}
	else
		return pMultiCurve;
}


///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines Members einer MultiCurve								                	 //
///////////////////////////////////////////////////////////////////////////////
_Curve * GMLReader::readCurveMember ( String ^ gmlTag, string srsName, int srsDimension  )
{
	_Curve * pCurve = nullptr;

  if ( validReader->IsEmptyElement )
    return NULL;

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo("LineString") == 0   )
			{
				pCurve = readLineSegment ( pActElement, srsName, srsDimension, LineSegment::_LINE_STRING );
			}
			else
			if ( pActElement->CompareTo("Curve") == 0  )
			{
				pCurve = readCurve ( pActElement, srsName, srsDimension );
			}
			else
			if ( pActElement->CompareTo("OrientableCurve") == 0  )
			{
				pCurve = readOrientableCurve ( pActElement, srsName, srsDimension );
			}
			else
				writeGmlError ( "Unbekannte GML-Geometrie", pActElement );	
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}
	
	return pCurve;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen aller Members einer MultiCurve								                	 //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readCurveMembers ( String ^ gmlTag, string srsName, int srsDimension, MultiCurve * pMultiCurve  )
{
  _Curve * pCurve;

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement = validReader->LocalName;

      if ( pActElement->CompareTo("LineString") == 0   )
      {
        pCurve = readLineSegment ( pActElement, srsName, srsDimension, LineSegment::_LINE_STRING );
        if ( pCurve != NULL )
        {
          pMultiCurve->addCurve( pCurve );
          pMultiCurve->setDim( pCurve->getDim() );
        }
      }
      else
        if ( pActElement->CompareTo("Curve") == 0  )
        {
          pCurve = readCurve ( pActElement, srsName, srsDimension );
          if ( pCurve != NULL )
          {
            pMultiCurve->addCurve( pCurve );
            pMultiCurve->setDim( pCurve->getDim() );
          }
        }
        else
          writeGmlError ( "Unbekannte GML-Geometrie", pActElement );	
    }
    else
      if ( validReader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = validReader->LocalName;
        if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
      }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer gml:OrientableCurve    								                	 //
///////////////////////////////////////////////////////////////////////////////
OrientableCurve * GMLReader::readOrientableCurve ( System::String ^ gmlTag, std::string srsName, int srsDimension )
{
	bool        orientation = true;
	String    ^ STR;
	std::string stdString;

	STR = validReader->GetAttribute ( "srsName" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( srsName, STR );

	STR = validReader->GetAttribute ( "srsDimension" );
	if ( STR != nullptr )
		srsDimension = Convert::ToInt32 ( STR );

	if ( srsDimension < 0 && srsName != "" )
		srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

	OrientableCurve * pOrientableCurve = new OrientableCurve ( srsDimension );
	pOrientableCurve->setSrsName( srsName );

	STR = validReader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pAppearances->addGeometry ( stdString, pOrientableCurve );
	}
	else
		stdString = Constant::createGmlId();
	pOrientableCurve->setGmlId ( stdString );

	pFeatures->addGeometry ( pOrientableCurve );


	STR = validReader->GetAttribute( "orientation" );
	if ( STR->CompareTo ( "-" ) == 0 )
		pOrientableCurve->setOrientation ( false );
	else
		pOrientableCurve->setOrientation ( true );

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo("baseCurve" ) == 0   )
			{
				STR = validReader->GetAttribute ( "xlink:href" );
				if ( STR != nullptr )
				{
					if ( STR[0] == '#' ) STR = STR->Substring ( 1 );
					QuConvert::systemStr2stdStr ( stdString, STR );
					pOrientableCurve->setBaseCurveId ( stdString );
				}
				else
				if ( !validReader->IsEmptyElement )
				{
					_Curve * pBaseCurve = readCurveMember ( pActElement, srsName, srsDimension );
					if ( pBaseCurve != NULL ) 
						pOrientableCurve->setBaseCurve ( pBaseCurve );
				}
			}

		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo ( "OrientableCurve") == 0 )
				break;
		}
	}

	return pOrientableCurve;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer Curve												                          	 //
///////////////////////////////////////////////////////////////////////////////
Curve * GMLReader::readCurve ( String ^  gmlTag, string srsName, int srsDimension )
{
	String            ^ STR;
	string              stdString;
	CurveSegment      * pCurveSegment;
  vector<ArcSegment*> vArcSegments;
  unsigned int        i;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );

  if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

	Curve * pCurve = new Curve ( srsDimension );
  pCurve->setSrsName( srsName );

	STR = validReader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
  {
		QuConvert::systemStr2stdStr ( stdString, STR );
    pAppearances->addGeometry ( stdString, pCurve );
  }
	else
		stdString = Constant::createGmlId();
	pCurve->setGmlId ( stdString );

  pFeatures->addGeometry ( pCurve );

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo("LineStringSegment") == 0   )
			{
				pCurveSegment = readLineSegment ( pActElement, srsName, srsDimension, LineSegment::_LINE_STRING_SEGMENT );
				if ( pCurveSegment != NULL )
        {
					pCurve->addCurveSegment ( pCurveSegment );
          pCurve->setDim( pCurveSegment->getDim() );
        }
			}
			else
			if ( pActElement->CompareTo("Arc") == 0 )
			{
        vArcSegments.clear();
				readArcSegment ( pActElement,  ArcSegment::_ARC, vArcSegments, srsName, srsDimension );
				if ( vArcSegments.size() > 0 )
        {
					pCurve->addCurveSegment ( vArcSegments[0] );
          pCurve->setDim( vArcSegments[0]->getDim() );
        }

			}
      else
      if ( pActElement->CompareTo("ArcString") == 0 )
      {
        vArcSegments.clear();
        readArcSegment ( pActElement, ArcSegment::_ARC_STRING, vArcSegments, srsName, srsDimension );
        if (  vArcSegments.size() > 0 )
        {
          for ( i = 0; i < vArcSegments.size(); i++ )
          {
            pCurve->addCurveSegment ( vArcSegments[i] );
            pCurve->setDim( vArcSegments[i]->getDim() );
          }
        }
      }
			else
			if ( pActElement->CompareTo("Circle") == 0 )
			{
        vArcSegments.clear();
				readArcSegment ( pActElement, ArcSegment::_CIRCLE, vArcSegments, srsName, srsDimension );
				if ( vArcSegments.size() > 0  )
					pCurve->addCurveSegment ( vArcSegments[0] );
			}
      else
			if ( pActElement->CompareTo("segments") == 0 )
			{
			}
			else
				writeGmlError ( "Unbekannte GML-Geometrie", pActElement );	
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}

	if ( pCurve->getCurveSegmentAnz() == 0 )
	{
		delete pCurve;
		return NULL;
	}
	else
		return pCurve;

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines LineSegment									                      			 //
///////////////////////////////////////////////////////////////////////////////
LineSegment * GMLReader::readLineSegment (  String ^  gmlTag, string srsName, int srsDimension, LineSegment::LINE_SEGMENT_TYPE lineSegmentType )
{
	vector<double>   xCoords;
	vector<double>   yCoords;
  vector<double>   zCoords;
	double		       coord[3];
	int				       i, anz;
	System::String ^ STR;
	string			     stdString;
	string           interpolation;
	cli::array<String^> ^ split;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );

	STR = validReader->GetAttribute ( "interpolation" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( interpolation, STR );
	
	if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

  LineSegment * pLineSegment = new LineSegment ( srsDimension, lineSegmentType );
  pLineSegment->setSrsName( srsName );
	pLineSegment->setInterpolation ( interpolation );

  STR = validReader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
  {
		QuConvert::systemStr2stdStr ( stdString, STR );
	  pLineSegment->setGmlId ( stdString );
  }

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;
			
			if ( pActElement->CompareTo("pos") == 0 )
			{
        STR = validReader->GetAttribute ( "srsDimension" );
        if ( STR != nullptr )
        {
          srsDimension = Convert::ToUInt32( STR );
          pLineSegment->setDim( srsDimension );
        }

				STR = validReader->ReadString();
				if (STR->Length != 0)
				{
					coord[0] = 0.0;
					coord[1] = 0.0;
          coord[2] = 0.0;
					STR = STR->Trim();
					split = STR->Split(delimiter, StringSplitOptions::RemoveEmptyEntries );
					for ( i = 0; i < split->Length && i < srsDimension; i++)
						coord[i] = XmlConvert::ToDouble(split[i] );
					xCoords.push_back ( coord[0]);
					yCoords.push_back ( coord[1]);
          if ( srsDimension == 3 )
            zCoords.push_back ( coord[2]);
				}
			}

			if ( pActElement->CompareTo("posList") == 0 )
			{
         STR = validReader->GetAttribute ( "srsDimension" );
         if ( STR != nullptr )
         {
           srsDimension = Convert::ToUInt32( STR );
           pLineSegment->setDim( srsDimension );
         }

				STR = validReader->ReadString();
				if (STR->Length != 0)
				{
					coord[0] = 0.0;
					coord[1] = 0.0;
					STR = STR->Trim();
					split = STR->Split( delimiter, StringSplitOptions::RemoveEmptyEntries );

          anz = div ( split->Length, srsDimension ).quot;

          for ( i = 0; i < anz; i++ )
          {
            xCoords.push_back( XmlConvert::ToDouble( split[srsDimension*i] ));
            yCoords.push_back( XmlConvert::ToDouble( split[srsDimension*i+1] ));
            if ( srsDimension > 2 )
              zCoords.push_back( XmlConvert::ToDouble( split[srsDimension*i+2] ));
					}
				}
			}
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if (pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}

	if ( srsDimension == 2 )
    pLineSegment->addPoints2D ( xCoords, yCoords );
  else
    pLineSegment->addPoints3D ( xCoords, yCoords, zCoords );

	return pLineSegment;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines ArcSegment											                        	 //
///////////////////////////////////////////////////////////////////////////////
size_t GMLReader::readArcSegment ( System::String ^ gmlTag, ArcSegment::ARC_SEGMENT_TYPE arcSegmentType, vector<ArcSegment*> &vSegments, 
                    string srsName, int srsDimension )
{
	vector<double>   xCoords;
	vector<double>   yCoords;
  vector<double>   zCoords;
  vector<double>   xCoordsLoc;
  vector<double>   yCoordsLoc;
  vector<double>   zCoordsLoc;
	double		       coord[3];
  int              il;
	unsigned int	   i, anz;
  string           gmlId;
	System::String ^ STR;
	cli::array<String^>  ^split;
  ArcSegment     * pArcSegment;
  string           interpolation = "";
  int              numArc = 0;

  STR = validReader->GetAttribute ( "gml:id" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( gmlId, STR );

  STR = validReader->GetAttribute( "interpolation" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( interpolation, STR );

  STR = validReader->GetAttribute( "numArc" );
  if ( STR != nullptr )
    numArc = Convert::ToInt32 ( STR );


	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo("pos") == 0 )
			{
        STR = validReader->GetAttribute ( "srsDimension" );
        if ( STR != nullptr )
          srsDimension = Convert::ToUInt32( STR );
         
				STR = validReader->ReadString();
				if (STR->Length != 0)
				{
					coord[0] = 0.0;
					coord[1] = 0.0;
          coord[2] = 0.0;
					STR = STR->Trim();
					split = STR->Split( delimiter, StringSplitOptions::RemoveEmptyEntries );
					for ( il = 0; il < split->Length && il < srsDimension; il++)
						coord[il] = XmlConvert::ToDouble(split[il] );
					xCoords.push_back ( coord[0]);
					yCoords.push_back ( coord[1]);
          if ( srsDimension == 3 )
            zCoords.push_back ( coord[2]);
				}
      }

      if ( pActElement->CompareTo("posList") == 0 )
      {
        STR = validReader->GetAttribute ( "srsDimension" );
        if ( STR != nullptr )
          srsDimension = Convert::ToUInt32( STR );

        STR = validReader->ReadString();
        if (STR->Length != 0)
        {
          coord[0] = 0.0;
          coord[1] = 0.0;
          coord[2] = 0.0;
          STR = STR->Trim();
          split = STR->Split( delimiter, StringSplitOptions::RemoveEmptyEntries );

          anz = div ( split->Length, srsDimension ).quot;

          for ( i = 0; i < anz; i++ )
          {
            xCoords.push_back( XmlConvert::ToDouble( split[srsDimension*i] ));
            yCoords.push_back( XmlConvert::ToDouble( split[srsDimension*i+1] ));
            if ( srsDimension > 2 )
              zCoords.push_back( XmlConvert::ToDouble( split[srsDimension*i+2] ));
          }
        }
      }
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if (pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}

  pArcSegment = new ArcSegment( arcSegmentType );

  if ( gmlId != "" )
    pArcSegment->setGmlId( gmlId );
  if ( srsDimension == 2 )
    pArcSegment->addPoints2D ( xCoords, yCoords );
  else
    pArcSegment->addPoints3D ( xCoords, yCoords, zCoords );

  if ( interpolation != "" )
    pArcSegment->setInterpolation( interpolation );
  if ( numArc > 0 )
    pArcSegment->setNumArc( numArc );

  vSegments.push_back ( pArcSegment );
 
	return vSegments.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines MultiPoint											                        	 //
///////////////////////////////////////////////////////////////////////////////
MultiPoint * GMLReader::readMultiPoint ( String ^  gmlTag, string srsName, int srsDimension  )
{
	String   ^ STR;
	string     stdString;
	GeoPoint * pPoint;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );

  if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

  MultiPoint * pMultiPoint = new MultiPoint( srsDimension );
  pMultiPoint->setSrsName( srsName );
 
	STR = validReader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( stdString, STR );
	else
		stdString = Constant::createGmlId();
	pMultiPoint->setGmlId ( stdString );

  pFeatures->addGeometry ( pMultiPoint );

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo("pointMember") == 0   )
			{
				pPoint = readPoint ( pActElement, srsName, srsDimension );
				if ( pPoint != NULL ) pMultiPoint->addPoint ( pPoint );
			}
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}

	if ( pMultiPoint->getPointAnz() == 0 )
	{
		delete pMultiPoint;
		return NULL;
	}
	else
		return pMultiPoint;
}


///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines Point									                          				 //
///////////////////////////////////////////////////////////////////////////////
GeoPoint * GMLReader::readPoint ( String ^  gmlTag, string srsName, int srsDimension )
{
	double	         coord[3];
	int	             i;
	bool             pointRead = false;
	String         ^ STR;
	string           stdString;
	cli::array<String^> ^ split;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );

  if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

	GeoPoint * pPoint = new GeoPoint( srsDimension );
  pPoint->setSrsName( srsName );

 	STR = validReader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( stdString, STR );
	else
		stdString = Constant::createGmlId();
	pPoint->setGmlId ( stdString );

  pFeatures->addGeometry ( pPoint );

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo("pos") == 0 )
			{
				STR = validReader->ReadString();
				if (STR->Length != 0)
				{
					coord[0] = 0.0;
					coord[1] = 0.0;
          coord[2] = 0.0;
					STR = STR->Trim();
					split = STR->Split( delimiter, StringSplitOptions::RemoveEmptyEntries );
					for ( i = 0; i < split->Length && i < srsDimension; i++)
						coord[i] = XmlConvert::ToDouble(split[i] );
					pointRead = true;
				}
			}
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if (pActElement->CompareTo( gmlTag ) == 0 ) break;
		}
	}
	if ( pointRead )
	{
    if ( srsDimension == 2 )
		  pPoint->set ( coord[0], coord[1] );
    else
      pPoint->set ( coord[0], coord[1],coord[2] );
		return pPoint;
	}
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen von gml:Geometry                                                //
///////////////////////////////////////////////////////////////////////////////
MultiGeometry * GMLReader::readMultiGeometry ( System::String ^ gmlTag, string srsName, int srsDimension )
{
  String		    ^ STR;
  string		      stdString;
  _Geometrie    * pGeometrie;
  MultiGeometry * pMultiGeometry;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );

  if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );
  
  pMultiGeometry = new MultiGeometry ( srsDimension );
  pMultiGeometry->setSrsName( srsName );

  STR = validReader->GetAttribute ( "gml:id" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pAppearances->addGeometry ( stdString, pMultiGeometry );
  }
  else
    stdString = Constant::createGmlId();
  pMultiGeometry->setGmlId ( stdString );

  pFeatures->addGeometry ( pMultiGeometry );

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement = validReader->LocalName;

      if ( pActElement->CompareTo("geometryMember") == 0 )
      {
        STR = validReader->GetAttribute ( "xlink:href" );
        if ( STR != nullptr )
        {
          if ( STR[0] == '#' ) STR = STR->Substring ( 1 );
          QuConvert::systemStr2stdStr ( stdString, STR );

          GeometryReference * pGeometryReference = new GeometryReference ( pFeatures );
          pGeometryReference->setRefGeoId ( stdString );
          pMultiGeometry->addGeometry ( pGeometryReference );
        }
        else
        if ( !validReader->IsEmptyElement )
        {
          pGeometrie = readGeometrie ( pActElement, srsName, srsDimension );
          if ( pGeometrie != NULL ) pMultiGeometry->addGeometry ( pGeometrie );
        }
      }
    }
    else
      if ( validReader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = validReader->LocalName;

        if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
      }
  }

  return pMultiGeometry;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen von gml:GeometricComplex                                        //
///////////////////////////////////////////////////////////////////////////////
GeometricComplex * GMLReader::readGeometricComplex ( System::String ^ gmlTag, string srsName, int srsDimension )
{
  String		       ^ STR;
  string		         stdString;
  _Geometrie       * pGeometrie;
  GeometricComplex * pGeometricComplex;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );

  if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

  pGeometricComplex = new GeometricComplex ( srsDimension );
  pGeometricComplex->setSrsName( srsName );

  STR = validReader->GetAttribute ( "gml:id" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pAppearances->addGeometry ( stdString, pGeometricComplex );
  }
  else
    stdString = Constant::createGmlId();
  pGeometricComplex->setGmlId ( stdString );

  pFeatures->addGeometry ( pGeometricComplex );

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement = validReader->LocalName;

      if ( pActElement->CompareTo("element") == 0 )
      {
        STR = validReader->GetAttribute ( "xlink:href" );
        if ( STR != nullptr )
        {
          if ( STR[0] == '#' ) STR = STR->Substring ( 1 );
          QuConvert::systemStr2stdStr ( stdString, STR );

          GeometryReference * pGeometryReference = new GeometryReference ( pFeatures );
          pGeometryReference->setRefGeoId ( stdString );
          pGeometricComplex->addGeometry ( pGeometryReference );
        }
        else
        if ( !validReader->IsEmptyElement )
        {
          pGeometrie = readGeometrie ( pActElement, srsName, srsDimension );
          if ( pGeometrie != NULL ) pGeometricComplex->addGeometry ( pGeometrie );
        }
      }
    }
    else
      if ( validReader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = validReader->LocalName;

        if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
      }
  }

  return pGeometricComplex;
}


///////////////////////////////////////////////////////////////////////////////
//  Einlesen von gml:Solid                                                   //
///////////////////////////////////////////////////////////////////////////////
Solid * GMLReader::readSolid ( System::String ^ gmlTag, string srsName, int srsDimension )
{
  String		 ^ STR;
  string		   stdString;
  Solid      * pSolid;
  _Surface   * pSurfaceMember;

  pSolid = new Solid ();

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( srsName, STR );
    pSolid->setSrsName ( srsName );
  }
  srsDimension = 3;

  STR = validReader->GetAttribute ( "gml:id" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pAppearances->addGeometry ( stdString, pSolid );
  }
  else
    stdString = Constant::createGmlId();
  pSolid->setGmlId ( stdString );

  pFeatures->addGeometry ( pSolid );

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement = validReader->LocalName;

      if ( pActElement->CompareTo("exterior") == 0 )
      {
        pSurfaceMember = readSurfaceMember ( pActElement, srsName, srsDimension );
        if ( pSurfaceMember != NULL ) pSolid->setAussenkontur ( pSurfaceMember );
      }
      else
        if ( pActElement->CompareTo("interior") == 0 )
        {
          pSurfaceMember = readSurfaceMember ( pActElement, srsName, srsDimension );
          if ( pSurfaceMember != NULL ) pSolid->addInnenkontur ( pSurfaceMember );
        }
    }
    else
      if ( validReader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = validReader->LocalName;

        if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
      }
  }

  return pSolid;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen von gml:MultiSolid                                              //
///////////////////////////////////////////////////////////////////////////////
MultiSolid * GMLReader::readMultiSolid ( System::String ^ gmlTag, string srsName, int srsDimension )
{
  String		 ^ STR;
  string		   stdString;
  _Solid     * pSolid;
  MultiSolid * pMultiSolid;

  pMultiSolid = new MultiSolid ();

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( srsName, STR );
    pMultiSolid->setSrsName ( stdString );
  }

  if ( gmlTag->CompareTo("CompositeSolid") == 0 )
    pMultiSolid->setIsCompositeSolid( true );
  else
    pMultiSolid->setIsCompositeSolid( false );
  
  srsDimension = 3;

  STR = validReader->GetAttribute ( "gml:id" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pAppearances->addGeometry ( stdString, pMultiSolid );
  }
  else
    stdString = Constant::createGmlId();
  pMultiSolid->setGmlId ( stdString );

  pFeatures->addGeometry ( pMultiSolid );

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement = validReader->LocalName;

      if ( pActElement->CompareTo("solidMember") == 0   )
      {
        STR = validReader->GetAttribute ( "xlink:href" );
        if ( STR != nullptr )
        {
          if ( STR[0] == '#' ) STR = STR->Substring ( 1 );
          QuConvert::systemStr2stdStr ( stdString, STR );

          SolidReference * pSolidReference = new SolidReference ( pFeatures );
          pSolidReference->setRefGeoId ( stdString );
          pMultiSolid->addSolid ( pSolidReference );
        }
        else
        if ( !validReader->IsEmptyElement )
        {
          pSolid = readSolidMember ( pActElement, srsName, srsDimension );
          if ( pSolid != NULL ) pMultiSolid->addSolid ( pSolid );
        }
      }
    }
    else
      if ( validReader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = validReader->LocalName;

        if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
      }
  }

  return pMultiSolid;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Einlesen von core:ImplicitGeometry                                       //
///////////////////////////////////////////////////////////////////////////////
ImplicitGeometry * GMLReader::readImplicitGeometry( System::String ^ gmlTag, string srsName, int srsDimension )
{
  String           ^ STR;
  string             stdString;
  ImplicitGeometry * pImplicitGeometry;
  Matrix4          * pTransformationMatrix;
  _Geometrie       * pGeometry;
  GeoPoint         * pPoint;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );

  if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

  pImplicitGeometry = new ImplicitGeometry ( srsDimension );
  pImplicitGeometry->setSrsName ( srsName );

  STR = validReader->GetAttribute ( "gml:id" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( stdString, STR );
  else
    stdString = Constant::createGmlId();
  pImplicitGeometry->setGmlId ( stdString );

  pFeatures->addGeometry ( pImplicitGeometry );

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement = validReader->LocalName;
      if ( pActElement->CompareTo("mimeType") == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr( stdString, STR );
          pImplicitGeometry->setMimeType( stdString );
        }
      }
      else
      if ( pActElement->CompareTo("transformationMatrix") == 0 )
      {
        pTransformationMatrix = readMatrix4 ( pActElement );
        if ( pTransformationMatrix != NULL  )
          pImplicitGeometry->setTransformationMatrix( pTransformationMatrix );
      }
      else
      if ( pActElement->CompareTo("libraryObject") == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr( stdString, STR );
          pImplicitGeometry->setLibraryObject( stdString );
        }
      }
      else
      if ( pActElement->CompareTo("relativeGMLGeometry") == 0 )
      {
        STR = validReader->GetAttribute( "xlink:href" );
        if ( STR != nullptr )
        {
          if ( STR[0] == '#' ) STR = STR->Substring ( 1 );
          QuConvert::systemStr2stdStr( stdString, STR );
          GeometryReference * pGeometryReference = new GeometryReference( pFeatures );
          pGeometryReference->setRefGeoId ( stdString );
          pImplicitGeometry->setRelativeGMLGeometry( pGeometryReference );
        }
        else
        {
          pGeometry = readGeometrie( validReader->LocalName, srsName, srsDimension );
          if ( pGeometry != NULL )
            pImplicitGeometry->setRelativeGMLGeometry( pGeometry );
        }
      }
      else
      if ( pActElement->CompareTo("referencePoint") == 0 )
      {
        pPoint = readPoint( pActElement, srsName, srsDimension );
        if ( pPoint != NULL )
          pImplicitGeometry->setReferencePoint( pPoint );
      }
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = validReader->LocalName;

      if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
    }

  }

  return pImplicitGeometry;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Einlesen von core:transformationMatrix                                   //
///////////////////////////////////////////////////////////////////////////////
Matrix4 * GMLReader::readMatrix4 (  System::String ^ gmlTag )
{
  String         ^ STR;
  string           stdString;
  cli::array<String^> ^ split;
  double           matrixElement[12];
  int              i;
//  array<wchar_t> ^ delimiter = { ' ', '\n', '\t', '\r', '\f' };

  STR = validReader->ReadString();
  if (STR->Length == 0 ) return NULL;
  
  STR = STR->Trim();
  split = STR->Split( delimiter, StringSplitOptions::RemoveEmptyEntries );
  if ( split->Length < 12 ) return NULL;

  for ( i = 0; i < split->Length && i < 12; i++)
    matrixElement[i] = XmlConvert::ToDouble(split[i] );

    Matrix4 * pMatrix = new Matrix4( matrixElement[0], matrixElement[1], matrixElement[2], matrixElement[3], 
                                     matrixElement[4], matrixElement[5], matrixElement[6], matrixElement[7],
                                     matrixElement[8], matrixElement[9], matrixElement[10], matrixElement[11] );

    return pMatrix;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer OrientableSurface                                         //
///////////////////////////////////////////////////////////////////////////////
OrientableSurface * GMLReader::readOrientableSurface ( System::String ^ gmlTag, string srsName, int srsDimension )
{
  String		        ^ STR;
  string		          stdString;
  string              errorMessage;
  OrientableSurface * pOrientableSurface;
  _Surface          * pBaseSurface;

  STR = validReader->GetAttribute ( "srsName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( srsName, STR );

  STR = validReader->GetAttribute ( "srsDimension" );
  if ( STR != nullptr )
    srsDimension = Convert::ToInt32 ( STR );

  if ( srsDimension < 0 && srsName != "" )
    srsDimension = pFeatures->getSrsManager()->getSrsDimension( srsName );

  pOrientableSurface = new OrientableSurface ( srsDimension );
  pOrientableSurface->setSrsName ( srsName );

  STR = validReader->GetAttribute ( "gml:id" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr ( stdString, STR );
  else
    stdString = Constant::createGmlId();
  pOrientableSurface->setGmlId ( stdString );

  pFeatures->addGeometry ( pOrientableSurface );

  STR = validReader->GetAttribute( "orientation" );
  if ( STR == nullptr ) STR = "+";

  if ( STR->CompareTo("+") == 0 )
    pOrientableSurface->setOrientation( true );
  else
    pOrientableSurface->setOrientation( false );

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement = validReader->LocalName;

      if ( pActElement->CompareTo("baseSurface") == 0 )
      {
        STR = validReader->GetAttribute ( "xlink:href" );
        if ( STR != nullptr )
        {
          if ( STR[0] == '#' ) STR = STR->Substring ( 1 );
          QuConvert::systemStr2stdStr ( stdString, STR );

          SurfaceReference * pSurfaceReference = new SurfaceReference ( pFeatures );
          pSurfaceReference->setRefGeoId ( stdString );
          pOrientableSurface->setBaseSurface ( pSurfaceReference );
        }
        else
        if ( !validReader->IsEmptyElement )
        {
          pBaseSurface = readSurfaceMember ( pActElement, srsName, srsDimension );
          if ( pBaseSurface != NULL ) 
            pOrientableSurface->setBaseSurface ( pBaseSurface );
        }
      }
      else
      if ( pActElement->CompareTo("appearance") == 0 )
      {
        errorMessage = "Appearance einer TexturedSurface wird noch nicht unterstützt";
        pFeatures->getLogfile()->writeLogFile( errorMessage );

        if ( pReadErrors != NULL )
        {
          ReadError * pReadError = new ReadError ( Error::ERROR, errorMessage, validReader->LineNumber );
          pReadErrors->addReadError( pReadError );
        }

        skipElement( pActElement );
      }
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = validReader->LocalName;

      if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
    }
  }

  return pOrientableSurface;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Einlesen von app:Appearance                                              //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readAppearance ( String ^ gmlTag )
{
  String         ^ STR;
  string           stdString;
  cAppearance    * pAppearance= NULL;

	if ( !m_readAppearances )
	{
		skipElement ( gmlTag );
		return;
	}


  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {    
      pActElement = validReader->Name;
      pActElementLoc = validReader->LocalName;

      if ( pActElementLoc->CompareTo ( _T("Appearance") ) == 0 )
      {
        pAppearance = new cAppearance ();
        pFeatures->addAppearance ( pAppearance );

        STR = validReader->GetAttribute ( _T("gml:id") );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAppearance->setGmlId ( stdString );
        }
      }
      else
      if ( pActElementLoc->CompareTo ( _T("theme") ) == 0 )
      {
        STR = validReader->ReadString ();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAppearance->setThema ( stdString );
        }
      }
      else
      if ( pActElementLoc->CompareTo ( _T("surfaceDataMember") ) == 0 )
      {
        readSurfaceDataMember( pAppearance );
      }
    }

    else if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElementLoc = validReader->LocalName;
      if (pActElementLoc->CompareTo( gmlTag ) == 0 ) 
        break;
    }
  } 
}
///////////////////////////////////////////////////////////////////////////////
//  Einlesen von SurfaceData                                                 //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readSurfaceDataMember( cAppearance * pAppearance )
{
  String            ^ STR;
  string              stdString;
  AppearanceDataRef * pAppearanceDataRef;

  STR = validReader->GetAttribute ( _T("xlink:href") );
  if ( STR != nullptr )
  {
    if ( STR[0] == _T('#') )
      STR = STR->Substring ( 1 );
    QuConvert::systemStr2stdStr ( stdString, STR );

    pAppearanceDataRef = new AppearanceDataRef ( stdString );
    pAppearance->addAppearanceDataRef ( pAppearanceDataRef );

    pAppearances->addAppearanceRef ( pAppearance, stdString );
  }

  if ( validReader->IsEmptyElement ) return;

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {    
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      if ( pActElementLoc->CompareTo ( _T("X3DMaterial") ) == 0 )
      {
        AppearanceColor * pColor;

        pColor = new AppearanceColor();
        pFeatures->addAppearanceData ( pColor );
        pAppearances->addAppearDataAppearRef ( pColor->getAppearDataId(), pAppearance->getAppearId() );

        pAppearanceDataRef = new AppearanceDataRef ( pColor->getAppearDataId() );

        STR = validReader->GetAttribute ( _T("gml:id") );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pColor->setGmlId( stdString );
          pAppearances->addAppearanceData ( pColor );
        }
        readX3DMaterial ( pColor );
        pAppearance->addAppearanceDataRef ( pAppearanceDataRef );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("GeoreferencedTexture") ) == 0 )
      {
        AppearanceGeoreferencedTexture * pTexture;

        pTexture = new AppearanceGeoreferencedTexture();
        pFeatures->addAppearanceData ( pTexture );
        pAppearances->addAppearDataAppearRef ( pTexture->getAppearDataId(), pAppearance->getAppearId() );

        pAppearanceDataRef = new AppearanceDataRef ( pTexture->getAppearDataId() );

        STR = validReader->GetAttribute ( _T("gml:id") );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pTexture->setGmlId( stdString );
          pAppearances->addAppearanceData ( pTexture );
        }
        readGeoreferencedTexture ( pTexture );
        pAppearance->addAppearanceDataRef ( pAppearanceDataRef );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("ParameterizedTexture") ) == 0 )
      {
        AppearanceParametrizedTexture * pTexture;

        pTexture = new AppearanceParametrizedTexture();
        pFeatures->addAppearanceData ( pTexture );
        pAppearances->addAppearDataAppearRef ( pTexture->getAppearDataId(), pAppearance->getAppearId() );

        pAppearanceDataRef = new AppearanceDataRef ( pTexture->getAppearDataId() );

        STR = validReader->GetAttribute ( _T("gml:id") );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pTexture->setGmlId( stdString );
          pAppearances->addAppearanceData ( pTexture );
        }

        readParametrizedTexture ( pTexture );
        pAppearance->addAppearanceDataRef ( pAppearanceDataRef );
      }
    }
    else if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElementLoc = validReader->LocalName;
      if (pActElementLoc->CompareTo(_T("surfaceDataMember")) == 0 ) break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen von X3DMaterial                                                 //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readX3DMaterial ( AppearanceColor * pColor )
{
  String ^ STR;
  string   stdString;
  bool     b;
  int      i;
  double   h;
  cli::array<String^> ^ COLOR;
  double           color[3];

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      if ( pActElementLoc->CompareTo ( _T("isFront") ) == 0 )
      {
        STR = validReader->ReadString ();
        if ( STR != nullptr && STR->CompareTo(_T("")) != 0 )
        {
          b = XmlConvert::ToBoolean ( STR );
          pColor->setIsFront ( b );
        }
      }
      else
      if ( pActElementLoc->CompareTo ( _T("shineness") ) == 0 )
      {
        STR = validReader->ReadString ();
        if ( STR != nullptr && STR->CompareTo(_T("")) != 0  )
        {
          h = XmlConvert::ToDouble ( STR );
          pColor->setShininess ( h );
        }
      }
      else
      if ( pActElementLoc->CompareTo ( _T("transparency") ) == 0 )
      {
        STR = validReader->ReadString ();
        if ( STR != nullptr && STR->CompareTo(_T("")) != 0  )
        {
          h = XmlConvert::ToDouble ( STR );
          pColor->setTransparency ( h );
        }
      }
      else
      if ( pActElementLoc->CompareTo ( _T("ambientIntensity") ) == 0 )
      {
        STR = validReader->ReadString ();
        if ( STR != nullptr && STR->CompareTo(_T("")) != 0  )
        {
          h = XmlConvert::ToDouble ( STR );
          pColor->setAmbientIntensity ( h );
        }
      }
      else
      if ( pActElementLoc->CompareTo ( _T("specularColor") ) == 0 )
      {
        STR = validReader->ReadString ();
        if ( STR != nullptr && STR->CompareTo(_T("")) != 0  )
        {
          COLOR = STR->Split ( delimiter, StringSplitOptions::RemoveEmptyEntries );
          for ( i = 0; i < 3; i++ )
            color[i] = XmlConvert::ToDouble ( COLOR[i] );
          pColor->setSpecularColor ( color );
        }
      }
      else
      if ( pActElementLoc->CompareTo ( _T("diffuseColor") ) == 0 )
      {
        STR = validReader->ReadString ();
        if ( STR != nullptr && STR->CompareTo(_T("")) != 0  )
        {
          COLOR = STR->Split ( delimiter, StringSplitOptions::RemoveEmptyEntries );
          for ( i = 0; i < 3; i++ )
            color[i] = XmlConvert::ToDouble ( COLOR[i] );
          pColor->setDiffuseColor ( color );
        }
      }
      else
      if ( pActElementLoc->CompareTo ( _T("emissiveColor") ) == 0 )
      {
         STR = validReader->ReadString ();
         if ( STR != nullptr && STR->CompareTo(_T("")) != 0  )
         {
           COLOR = STR->Split ( delimiter, StringSplitOptions::RemoveEmptyEntries );
           for ( i = 0; i < 3; i++ )
             color[i] = XmlConvert::ToDouble ( COLOR[i] );
           pColor->setEmissiveColor ( color );
         }
      }
      else
      if ( pActElementLoc->CompareTo ( _T("isSmooth") ) == 0 )
      {
        STR = validReader->ReadString ();
        if ( STR != nullptr && STR->CompareTo(_T("")) != 0  )
        {
           b = XmlConvert::ToBoolean ( STR );
           pColor->setIsSmooth ( b );
        }
      }
      else
      if ( pActElementLoc->CompareTo ( _T("target") ) == 0 )
      {
         STR = validReader->ReadString ();
         if ( STR != nullptr && STR->CompareTo(_T("")) != 0  )
         {
           if ( STR[0] == _T('#') )
             STR = STR->Substring ( 1 );
           QuConvert::systemStr2stdStr ( stdString, STR );

           pAppearances->addAppearanceDataRef ( pColor, stdString );
           pColor->addTarget ( stdString );
         }
      }
    }
    else if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElementLoc = validReader->LocalName;
      if (pActElementLoc->CompareTo(_T("X3DMaterial")) == 0 ) break;
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen GeoreferencedTexture                                            //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readGeoreferencedTexture ( AppearanceGeoreferencedTexture * pTexture )
{
  String ^ STR;
  string   stdString;
  bool     b;
  int      i;
  cli::array<String^> ^ COLOR;
  double           color[4];

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {    
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      if ( pActElementLoc->CompareTo ( _T("isFront") ) == 0 )
      {
        STR = validReader->ReadString ();
        b = XmlConvert::ToBoolean ( STR );
        pTexture->setIsFront ( b );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("imageURI") ) == 0 )
      {
        STR = validReader->ReadString ();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pTexture->setImageURI ( stdString );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("mimeType") ) == 0 )
      {
        STR = validReader->ReadString ();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pTexture->setMimeType ( stdString );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("textureType") ) == 0 )
      {
        STR = validReader->ReadString ();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pTexture->setTextureType ( stdString );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("wrapMode") ) == 0 )
      {
        STR = validReader->ReadString ();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pTexture->setWrapMode ( stdString );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("borderColor") ) == 0 )
      {
        color[3] = 0.0;
        STR = validReader->ReadString ();
        STR = STR->Trim ( delimiter );
        COLOR = STR->Split ( delimiter, StringSplitOptions::RemoveEmptyEntries );
        for ( i = 0; i < COLOR->Length; i++ )
          color[i] = XmlConvert::ToDouble ( COLOR[i] );

        pTexture->setBorderColor ( color );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("preferWorldFile") ) == 0 )
      {
        STR = validReader->ReadString ();
        b = XmlConvert::ToBoolean ( STR );
        pTexture->setPreferWorldFile ( b );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("orientation") ) == 0 )
      {
        STR = validReader->ReadString ();
        COLOR = STR->Split ( delimiter, StringSplitOptions::RemoveEmptyEntries );
        for ( i = 0; i < COLOR->Length; i++ )
          color[i] = XmlConvert::ToDouble ( COLOR[i] );

        pTexture->setOrientation ( color );
      }
      else
      if (  pActElementLoc->CompareTo ( _T("referencePoint") ) == 0 )
      {
        GeoPoint * pVertex = readPoint ( _T("referencePoint"), _T(""), 2 );
        if ( pVertex != NULL )
          pTexture->setReferencePoint ( pVertex );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("target") ) == 0 )
      {
        STR = validReader->ReadString ();
        if ( STR != nullptr )
        {
          if ( STR[0] == _T('#') )
            STR = STR->Substring ( 1 );
          QuConvert::systemStr2stdStr ( stdString, STR );

          pAppearances->addAppearanceDataRef ( pTexture, stdString );
          pTexture->addTarget ( stdString );
        }
      }
    }
    else if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElementLoc = validReader->LocalName;
      if (pActElementLoc->CompareTo(_T("GeoreferencedTexture")) == 0 ) break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen ParametrizedTexture                                             //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readParametrizedTexture ( AppearanceParametrizedTexture * pTexture )
{
  String                 ^ STR;
  string                   stdString;
  string                   uri;
  string                   target;
  bool                     b;
  int                      i;
  cli::array<String^>         ^ COLOR;
  double                   color[4];
  TexCoordRef            * pTexCoordRef = nullptr;

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      if ( pActElementLoc->CompareTo ( _T("isFront") ) == 0 )
      {
        STR = validReader->ReadString ();
        b = XmlConvert::ToBoolean ( STR );
        pTexture->setIsFront ( b );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("imageURI") ) == 0 )
      {
        STR = validReader->ReadString ();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pTexture->setImageURI ( stdString );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("mimeType") ) == 0 )
      {
        STR = validReader->ReadString ();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pTexture->setMimeType ( stdString );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("textureType") ) == 0 )
      {
        STR = validReader->ReadString ();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pTexture->setTextureType ( stdString );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("wrapMode") ) == 0 )
      {
        STR = validReader->ReadString ();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pTexture->setWrapMode ( stdString );
      }
      else
      if ( pActElementLoc->CompareTo ( _T("borderColor") ) == 0 )
      {
        color[3] = 0.0;
        STR = validReader->ReadString ();
        COLOR = STR->Split ( delimiter, StringSplitOptions::RemoveEmptyEntries );
        for ( i = 0; i < COLOR->Length; i++ )
          color[i] = XmlConvert::ToDouble ( COLOR[i] );

        pTexture->setBorderColor ( color );
      }
      else
      if (  pActElementLoc->CompareTo ( _T("target") ) == 0 )
      {
        pTexCoordRef = new TexCoordRef;
        pTexture->addTexCoord ( pTexCoordRef );

        STR = validReader->GetAttribute ( _T("uri") );
        if ( STR[0] == _T('#') ) STR = STR->Substring ( 1 );
        QuConvert::systemStr2stdStr ( uri, STR );
        pTexCoordRef->m_targetGmlId = uri;

        STR = validReader->GetAttribute ( _T("xlink:href") );
        if ( STR != nullptr )
        {
          if ( STR[0] == _T('#') )
            STR = STR->Substring ( 1 );
          QuConvert::systemStr2stdStr ( stdString, STR );

          pTexCoordRef->m_texCoordGmlId = stdString;
          pAppearances->addAppearanceDataRef ( pTexture, uri, stdString );         
        } 
      }
      else
      if (  pActElementLoc->CompareTo ( _T("TexCoordList") ) == 0 )
      {
        TexCoordList * pTexCoordList = new TexCoordList ();
        pFeatures->addTexCoord ( pTexCoordList );
        pTexCoordRef->m_texCoordId = pTexCoordList->getTexCoordId();

        pAppearances->addAppearanceDataRef ( pTexture, uri, pTexCoordList->getTexCoordId() );
        readTexCoordList ( pTexture, pTexCoordList );
      }
      else
      if (  pActElementLoc->CompareTo ( _T("TexCoordGen") ) == 0 )
      {
         TexCoordGen * pTexCoordGen = new TexCoordGen ();
         pFeatures->addTexCoord ( pTexCoordGen );
         pTexCoordRef->m_texCoordId = pTexCoordGen->getTexCoordId();

         pAppearances->addAppearanceDataRef ( pTexture, uri, pTexCoordGen->getTexCoordId() );
         readTexCoordGen ( pTexture, pTexCoordGen );
       }
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElementLoc = validReader->LocalName;
      if ( pActElementLoc->CompareTo(_T("ParameterizedTexture")) == 0 ) break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen TexCoordList                                                    //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readTexCoordList( AppearanceParametrizedTexture * pTexture, TexCoordList * pTexCoordList )
{
  String           ^ STR;
  string             stdString;
  string             ring;
  int                i;
  cli::array<String^>   ^ TEXTURE_COORDINATES;
  DoubleList         textureCoordinates;

  STR = validReader->GetAttribute ( _T("gml:id") );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pTexCoordList->setGmlId( stdString );
    pAppearances->addTexCoord ( pTexCoordList );
  }

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {    
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      if (  pActElementLoc->CompareTo ( _T("textureCoordinates") ) == 0 )
      {
        textureCoordinates.clear();

        STR = validReader->GetAttribute ( _T("ring") );
        if ( STR[0] == _T('#') )
          STR = STR->Substring ( 1 );
        QuConvert::systemStr2stdStr ( ring, STR );

        STR = validReader->ReadString ();
        TEXTURE_COORDINATES = STR->Split ( delimiter, StringSplitOptions::RemoveEmptyEntries );
        for ( i = 0; i < TEXTURE_COORDINATES->Length; i++ )
          textureCoordinates.push_back ( XmlConvert::ToDouble ( TEXTURE_COORDINATES[i] ) );

        TexCoordPolyloop * pTexCoordPolyloop = new TexCoordPolyloop (textureCoordinates );
        pFeatures->addTexCoord ( pTexCoordPolyloop );
        pAppearances->addAppearanceDataRef ( pTexture, ring, pTexCoordPolyloop->getTexCoordId() );
        pTexCoordList->addTexCoordPolyloop ( ring, pTexCoordPolyloop );
      }
    }
    else
      if ( validReader->NodeType == XmlNodeType::EndElement )
      {
        pActElementLoc = validReader->LocalName;
        if ( pActElementLoc->CompareTo(_T("TexCoordList")) == 0 ) break;
      }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen TexCoordGen                                                     //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readTexCoordGen( AppearanceParametrizedTexture * pTexture, TexCoordGen * pTexCoordGen )
{
  String           ^ STR;
  string             stdString;
  cli::array<String^>   ^ WORLD_TO_TEXTURE;
  Matrix4          * pTransformation;

  STR = validReader->GetAttribute ( _T("gml:id") );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pTexCoordGen->setGmlId( stdString );
    pAppearances->addTexCoord ( pTexCoordGen );
  }

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {    
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      if (  pActElementLoc->CompareTo ( _T("worldToTexture") ) == 0 )
      {
        STR = validReader->ReadString ();
        WORLD_TO_TEXTURE = STR->Split ( delimiter, StringSplitOptions::RemoveEmptyEntries );
        pTransformation = new Matrix4 ( XmlConvert::ToDouble ( WORLD_TO_TEXTURE[0] ),
          XmlConvert::ToDouble ( WORLD_TO_TEXTURE[1] ),
          XmlConvert::ToDouble ( WORLD_TO_TEXTURE[2] ),
          XmlConvert::ToDouble ( WORLD_TO_TEXTURE[3] ),
          XmlConvert::ToDouble ( WORLD_TO_TEXTURE[4] ),
          XmlConvert::ToDouble ( WORLD_TO_TEXTURE[5] ),
          XmlConvert::ToDouble ( WORLD_TO_TEXTURE[6] ),
          XmlConvert::ToDouble ( WORLD_TO_TEXTURE[7] ),
          XmlConvert::ToDouble ( WORLD_TO_TEXTURE[8] ),
          XmlConvert::ToDouble ( WORLD_TO_TEXTURE[9] ),
          XmlConvert::ToDouble ( WORLD_TO_TEXTURE[10] ),
          XmlConvert::ToDouble ( WORLD_TO_TEXTURE[11] ));

        pTexCoordGen->setTransformation ( pTransformation );
      }
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElementLoc = validReader->LocalName;
      if ( pActElementLoc->CompareTo(_T("TexCoordGen")) == 0 ) break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen des aktuellen Kürzels des Default-Namespaces                  //
///////////////////////////////////////////////////////////////////////////////
string GMLReader::addDefaultNamespaceKuerzel ( string gmlName )
{
	size_t index;
  string stdString;
  string kuerzel;
  string gmlNameAct;
  string defaultNamespaceKuerzel;

  index = gmlName.find ( ":" );
  if ( index != string::npos )
    return gmlName;

  gmlNameAct = gmlName;

  stdString = pGmlNamespacesAct->getDefaultNamespace();
  if ( stdString != "" )
  {
    defaultNamespaceKuerzel = pGmlNamespacesAct->getKuerzel ( stdString );
    if ( defaultNamespaceKuerzel != "" ) 
      gmlNameAct = defaultNamespaceKuerzel + ":" + gmlName;
  }

  return gmlNameAct;
}

///////////////////////////////////////////////////////////////////////////////
//  Austausch des aktuellen Namespace-Kürzels gegen das Standard-Kürzel      //
///////////////////////////////////////////////////////////////////////////////
std::string GMLReader::createGmlNameStandard ( std::string gmlName )
{
	size_t index;
  string stdString;
  string kuerzel;
  string kuerzelAct;
  string gmlNameAct = "";
  string defaultNamespaceKuerzel;

  stdString = pGmlNamespacesAct->getDefaultNamespace();
  if ( stdString != "" )
    defaultNamespaceKuerzel = pGmlNamespacesStandard->getKuerzel ( stdString );
  else
    defaultNamespaceKuerzel = "";

  index = gmlName.find ( ":" );
  if ( index == string::npos )
  {
    if ( defaultNamespaceKuerzel != "" )
      gmlNameAct = defaultNamespaceKuerzel + ":" + gmlName;
    else
      gmlNameAct = gmlName;
  }
  else
  {
    kuerzelAct = gmlName.substr ( 0, index );
    stdString = pGmlNamespacesAct->getNamespace ( kuerzelAct );
    kuerzel = pGmlNamespacesStandard->getKuerzel ( stdString );

    if ( kuerzel != "" )
    {
      stdString = gmlName.substr ( index+1 );
      gmlNameAct = kuerzel + ":" + stdString;
    }
    else
      gmlNameAct = gmlName;
  }

  return gmlNameAct;
}


///////////////////////////////////////////////////////////////////////////////
//  Schreibt eine Fehlermeldung in den Logfile						              		 //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::writeGmlError ( string errorMessage, String ^  gmlTag )
{
	string errText;

	pFeatures->getLogfile()->writeLogFile ( errorMessage );
	if ( gmlTag != nullptr )
	{
		QuConvert::systemStr2stdStr ( errText, gmlTag );
		pFeatures->getLogfile()->writeLogFile ( errText );
    errorMessage = errorMessage + ": " + errText;
	}
	pFeatures->getLogfile()->writeLogFile ( "" );
}

///////////////////////////////////////////////////////////////////////////////
//  Überliest ein GML-Element										                        		 //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::skipElement( String ^  gmlTag )
{
  if ( validReader->IsEmptyElement ) return;

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = validReader->LocalName;
      if ( pActElement->CompareTo( gmlTag ) == 0 ) break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt den Default Namespace-Kürzel hinzu                                  //
///////////////////////////////////////////////////////////////////////////////
String ^ GMLReader::addDefaultNamespaceKuerzel ( String ^ gmlName )
{
  string stdString;

  QuConvert::systemStr2stdStr ( stdString, gmlName );
  stdString = addDefaultNamespaceKuerzel ( stdString );
  return QuConvert::ToString ( stdString );
}

///////////////////////////////////////////////////////////////////////////////
//  Liest gml:TimePeriod                                                     //
///////////////////////////////////////////////////////////////////////////////
TimePeriod * GMLReader::readTimePeriod ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg )
{
  TimePosition   timePosition;
  string         qualifier;

  if ( validReader->IsEmptyElement ) return NULL;

  TimePeriod * pTimePeriod = new TimePeriod;

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {    
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      if (  pActElementLoc->CompareTo( "beginPosition") == 0 )
      {
        timePosition = readTimePosition();
        pTimePeriod->setStartPosition( timePosition );
      }
      else
        if (  pActElementLoc->CompareTo( "endPosition") == 0 )
        {
          timePosition = readTimePosition();
          pTimePeriod->setEndPosition( timePosition );
        }
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {    
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      if ( pActElement->CompareTo( QuConvert::ToString( gmlAttributNameOrg ) ) )
      {
        break;
      }
    }
  }
  return pTimePeriod;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest gml:TimePeriod                                                     //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readTimePeriod ( std::string gmlAttributNameAct, Feature * pFeature, std::string gmlAttributNameOrg )
{
  TimePeriod * pTimePeriod = readTimePeriod ( gmlAttributNameAct, gmlAttributNameOrg );
  DateAttr * pDateAttr = new DateAttr;
  pDateAttr->value = pTimePeriod;
  pFeature->setDateAttribut( gmlAttributNameAct, pDateAttr );
}

///////////////////////////////////////////////////////////////////////////////
//  Liest gml:timePosition                                                   //
///////////////////////////////////////////////////////////////////////////////
TimePosition  GMLReader::readTimePosition()
{
  String      ^ STR;
  string        stdString;
  TimePosition  timePosition;
  string        qualifier;

  qualifier = "";
  while ( validReader->MoveToNextAttribute() )
  {
    STR = validReader->Name;
    if ( STR->CompareTo( "indeterminatePosition") == 0 )
    {
      STR = validReader->Value;
      QuConvert::systemStr2stdStr( qualifier, STR );
    }
  }

  STR = validReader->ReadString();
  if (STR != nullptr && STR != "") QuConvert::systemStr2stdStr ( stdString, STR );
  else                               stdString = "";

  if ( stdString != "" )
  {
    timePosition.fromXMLString ( stdString );
    timePosition.setQualifier( qualifier );
  }

  return timePosition;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest gml:TimeIntervalLength                                             //
///////////////////////////////////////////////////////////////////////////////
TimeIntervalLength * GMLReader::readTimeIntervalLength()
{
  String              ^ STR;
  string                stdString;
  string                unit;
  double                h;

  unit = "";
  while ( validReader->MoveToNextAttribute() )
  {
    STR = validReader->Name;
    if ( STR->CompareTo( "unit") == 0 )
    {
      STR = validReader->Value;
      QuConvert::systemStr2stdStr( unit, STR );
    }
  }

  STR = validReader->ReadString();
  if ( STR != nullptr && STR != "" )
    h = XmlConvert::ToDouble ( STR );
  else
    h = 0.0;

  TimeIntervalLength * pTimeIntervalLength = new TimeIntervalLength;
  pTimeIntervalLength->set( (float)h, unit );

  return pTimeIntervalLength;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur Energy ADE:                                                          //
//  Liest eine beliebige TimeSeries                                          //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readTimeSeries ( std::string gmlAttributNameAct, Feature * pFeature, std::string gmlAttributNameOrg )
{
  std::string           gmlAttributNameActLoc;
  std::string           gmlAttributNameOrgLoc;
  AbstractTimeSeries * pTimeSeries;

  if ( validReader->IsEmptyElement) return;

  while ( validReader->Read ( ))
  {
    if ( validReader->NodeType == XmlNodeType::Element)
    {
      String ^ pActElement = validReader->Name;
      String ^ pActElementLoc = validReader->LocalName;

      QuConvert::systemStr2stdStr ( gmlAttributNameActLoc, pActElement );
      gmlAttributNameOrgLoc = gmlAttributNameActLoc;
      gmlAttributNameActLoc = addDefaultNamespaceKuerzel ( gmlAttributNameActLoc );
      gmlAttributNameActLoc = createGmlNameStandard ( gmlAttributNameActLoc );

      if (gmlAttributNameActLoc ==  "energy:RegularTimeSeries" )
      {
        pTimeSeries = readRegularTimeSeries ( gmlAttributNameActLoc, gmlAttributNameOrgLoc );
        if (pTimeSeries != NULL)
          pFeature->addTimeSeries ( gmlAttributNameAct, pTimeSeries );
      }
      else
      if (gmlAttributNameActLoc ==  "energy:RegularTimeSeriesFile" )
      {
        pTimeSeries = readRegularTimeSeriesFile ( gmlAttributNameActLoc, gmlAttributNameOrgLoc );
        if (pTimeSeries != NULL)
          pFeature->addTimeSeries ( gmlAttributNameAct, pTimeSeries );
      }
      else
      if (gmlAttributNameActLoc ==  "energy:IrregularTimeSeries" )
      {
        pTimeSeries = readIrregularTimeSeries ( gmlAttributNameActLoc, gmlAttributNameOrgLoc );
        if (pTimeSeries != NULL)
          pFeature->addTimeSeries ( gmlAttributNameAct, pTimeSeries );
      }
      else
      if (gmlAttributNameActLoc ==  "energy:MonthlyTimeSeries" )
      {
        pTimeSeries = readMonthlyTimeSeries ( gmlAttributNameActLoc, gmlAttributNameOrgLoc );
        if (pTimeSeries != NULL)
          pFeature->addTimeSeries ( gmlAttributNameAct, pTimeSeries );
      }
      else
      if (gmlAttributNameActLoc ==  "energy:IrregularTimeSeriesFile" )
      {
        pTimeSeries = readIrregularTimeSeriesFile ( gmlAttributNameActLoc, gmlAttributNameOrgLoc );
        if (pTimeSeries != NULL)
          pFeature->addTimeSeries ( gmlAttributNameAct, pTimeSeries );
      }
    }
    else
      if ( validReader->NodeType == XmlNodeType::EndElement)
      {
        String ^ pActElement = validReader->Name;
        if (pActElement->CompareTo ( QuConvert::ToString ( gmlAttributNameOrg ) ) == 0)
          break;
      }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Nur Energy ADE:                                                          //
//  Liest energy:RegularTimeSeries                                           //
///////////////////////////////////////////////////////////////////////////////
AbstractTimeSeries * GMLReader::readRegularTimeSeries ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg )
{
  string gmlAttributNameActLoc;
  string gmlAttributNameOrgLoc;

  if ( validReader->IsEmptyElement ) return NULL;

  RegularTimeSeries * pRegularTimeSeries = new RegularTimeSeries ( Constant::createGmlId() );

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      QuConvert::systemStr2stdStr ( gmlAttributNameActLoc, pActElement );
      gmlAttributNameOrgLoc = gmlAttributNameActLoc;
      gmlAttributNameActLoc = addDefaultNamespaceKuerzel ( gmlAttributNameActLoc );
      gmlAttributNameActLoc = createGmlNameStandard ( gmlAttributNameActLoc );

      if (  pActElementLoc->CompareTo( "variableProperties" ) == 0 )
        readTimeSeriesVariableProperties ( gmlAttributNameActLoc, pRegularTimeSeries, gmlAttributNameOrgLoc );
      else
      if ( pActElementLoc->CompareTo( "temporalExtent" ) == 0 )
      {
        TimePeriod * pTimePeriod = readTimePeriod ( gmlAttributNameActLoc, gmlAttributNameOrgLoc );
        pRegularTimeSeries->setTemporalExtent( pTimePeriod );
      }
      else
      if ( pActElementLoc->CompareTo( "timeInterval" ) == 0 )
      {
        TimeIntervalLength * pTimeIntervalLength = readTimeIntervalLength();
        pRegularTimeSeries->setTimeIntervalLength( pTimeIntervalLength );
      }
      else
      if ( pActElementLoc->CompareTo( "values" ) == 0 )
        readTimeSeriesValues ( gmlAttributNameActLoc, pRegularTimeSeries, gmlAttributNameOrgLoc );
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement    = validReader->Name;
      if ( pActElement->CompareTo( QuConvert::ToString( gmlAttributNameOrg ) ) == 0 )
      {
        break;
      }
    }
  }
  return pRegularTimeSeries;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur Energy ADE:                                                          //
//  Liest energy:MonthlyTimeSeries                                           //
///////////////////////////////////////////////////////////////////////////////
AbstractTimeSeries * GMLReader::readMonthlyTimeSeries ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg )
{
  string   gmlAttributNameActLoc;
  string   gmlAttributNameOrgLoc;
  String ^ STR;
  string   str;
  cli::array<String^> ^ split;

  if ( validReader->IsEmptyElement ) return NULL;

  MonthlyTimeSeries * pMonthlyTimeSeries = new MonthlyTimeSeries ( Constant::createGmlId() );

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      QuConvert::systemStr2stdStr ( gmlAttributNameActLoc, pActElement );
      gmlAttributNameOrgLoc = gmlAttributNameActLoc;
      gmlAttributNameActLoc = addDefaultNamespaceKuerzel ( gmlAttributNameActLoc );
      gmlAttributNameActLoc = createGmlNameStandard ( gmlAttributNameActLoc );

      if (  pActElementLoc->CompareTo( "variableProperties" ) == 0 )
        readTimeSeriesVariableProperties ( gmlAttributNameActLoc, pMonthlyTimeSeries, gmlAttributNameOrgLoc );
      else
      if (  pActElementLoc->CompareTo( "uom" ) == 0 )
      {
        STR = validReader->GetAttribute ( "uom" );
        QuConvert::systemStr2stdStr( str, STR );
        UOM * pUOM = pFeatures->getUOMList()->getUOMFromIdentifier( str );
        pMonthlyTimeSeries->setUOM ( pUOM );
      }
      else
      if ( pActElementLoc->CompareTo( "year" ) == 0 )
      {
        STR = validReader->ReadString ();
        int value = XmlConvert::ToInt32( STR );
        pMonthlyTimeSeries->setYear ( value );
      }
      else
      if ( pActElementLoc->CompareTo( "values" ) == 0 )
      {
        vector<double> vValues;

        STR = validReader->ReadString();
        split = STR->Split(delimiter, StringSplitOptions::RemoveEmptyEntries );
        for (int i = 0; i < split->Length; i++)
          vValues.push_back(XmlConvert::ToDouble(split[i]));
        pMonthlyTimeSeries->setValues( vValues );
      }
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement    = validReader->Name;
      if ( pActElement->CompareTo( QuConvert::ToString( gmlAttributNameOrg ) ) == 0 )
      break;
    }
  }
  return pMonthlyTimeSeries;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur Energy ADE:                                                          //
//  Liest die Werte einer Zeitreihe                                          //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readTimeSeriesValues ( std::string gmlAttributNameActLoc, RegularTimeSeries * pRegularTimeSeries, std::string gmlAttributNameOrgLoc )
{
  String         ^ STR;
  string           stdString;
  string           uom;
  cli::array<String^> ^ split;
  vector<double>   values;

  uom = "";
  while ( validReader->MoveToNextAttribute() )
  {
    STR = validReader->Name;
    if ( STR->CompareTo( "uom" ) == 0 )
    {
      STR = validReader->Value;
      QuConvert::systemStr2stdStr( uom, STR );
      break;
    }
  }

  UOM * pUOM = pFeatures->getUOMList()->getUOMFromIdentifier( uom );
  pRegularTimeSeries->setUOM( pUOM );

  STR = validReader->ReadString();
  split = STR->Split(delimiter, StringSplitOptions::RemoveEmptyEntries );
  for (int i = 0; i < split->Length; i++)
    pRegularTimeSeries->addValue(XmlConvert::ToDouble(split[i]));
}

///////////////////////////////////////////////////////////////////////////////
//  Liest energy:IrrgularTimeSeries                                          //
///////////////////////////////////////////////////////////////////////////////
AbstractTimeSeries * GMLReader::readIrregularTimeSeries ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg )
{
  string       gmlAttributNameActLoc;
  string       gmlAttributNameOrgLoc;
  String     ^ STR;
  string       stdString;
  TimePosition timePosition;
  double       value;

  if ( validReader->IsEmptyElement ) return NULL;

  IrregularTimeSeries * pIrregularTimeSeries = new IrregularTimeSeries ( Constant::createGmlId() );

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      QuConvert::systemStr2stdStr ( gmlAttributNameActLoc, pActElement );
      gmlAttributNameOrgLoc = gmlAttributNameActLoc;
      gmlAttributNameActLoc = addDefaultNamespaceKuerzel ( gmlAttributNameActLoc );
      gmlAttributNameActLoc = createGmlNameStandard ( gmlAttributNameActLoc );

      if (  pActElementLoc->CompareTo( "variableProperties" ) == 0 )
        readTimeSeriesVariableProperties ( gmlAttributNameActLoc, pIrregularTimeSeries, gmlAttributNameOrgLoc );
      else
      if (  pActElementLoc->CompareTo( "uom" ) == 0 )
      {
        STR = validReader->GetAttribute ( "uom" );
        QuConvert::systemStr2stdStr( stdString, STR );
        UOM * pUOM = pFeatures->getUOMList()->getUOMFromIdentifier( stdString );
        pIrregularTimeSeries->setUOM ( pUOM );
      }
      else
      if (  pActElementLoc->CompareTo( "time" ) == 0 )
        timePosition = readTimePosition();
      else
      if (  pActElementLoc->CompareTo( "value" ) == 0 )
      {
        STR = validReader->ReadString ();
        value = XmlConvert::ToDouble( STR );
      }
    }
    else
    {
      if ( validReader->NodeType == XmlNodeType::EndElement )
      {
        pActElement    = validReader->Name;
        pActElementLoc = validReader->LocalName;

        if ( pActElementLoc->CompareTo( "contains" ) == 0)
          pIrregularTimeSeries->addData( timePosition, value );
        else
        if ( pActElement->CompareTo( QuConvert::ToString ( gmlAttributNameOrg ) ) == 0 )
        {
          break;
        }
      }
    }
  }
  return pIrregularTimeSeries;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest energy:RegularTimeSeriesFile                                       //
///////////////////////////////////////////////////////////////////////////////
AbstractTimeSeries * GMLReader::readRegularTimeSeriesFile ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg )
{
  string   gmlAttributNameActLoc;
  string   gmlAttributNameOrgLoc;
  String ^ STR;
  String ^ ERR_TEXT;
  string   stdString;

  if ( validReader->IsEmptyElement ) return NULL;

  RegularTimeSeriesFile * pRegularTimeSeriesFile = new RegularTimeSeriesFile ( Constant::createGmlId() );

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      QuConvert::systemStr2stdStr ( gmlAttributNameActLoc, pActElement );
      gmlAttributNameOrgLoc = gmlAttributNameActLoc;
      gmlAttributNameActLoc = addDefaultNamespaceKuerzel ( gmlAttributNameActLoc );
      gmlAttributNameActLoc = createGmlNameStandard ( gmlAttributNameActLoc );

      if (  pActElementLoc->CompareTo( "variableProperties" ) == 0 )
        readTimeSeriesVariableProperties ( gmlAttributNameActLoc, pRegularTimeSeriesFile, gmlAttributNameOrgLoc );
      else
      if ( pActElementLoc->CompareTo( "uom" ) == 0 )
      {
        STR = validReader->GetAttribute ( "uom" );
        QuConvert::systemStr2stdStr( stdString, STR );
        UOM * pUOM = pFeatures->getUOMList()->getUOMFromIdentifier( stdString );
        pRegularTimeSeriesFile->setUOM ( pUOM );
      }
      else
      if ( pActElementLoc->CompareTo( "file" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pRegularTimeSeriesFile->setFile( stdString );
      }
      else
      if ( pActElementLoc->CompareTo( "temporalExtent" ) == 0 )
      {
        TimePeriod * pTimePeriod = readTimePeriod ( gmlAttributNameActLoc, gmlAttributNameOrgLoc );
        pRegularTimeSeriesFile->setTemporalExtent( pTimePeriod );
      }
      else
      if ( pActElementLoc->CompareTo( "timeInterval" ) == 0 )
      {
        TimeIntervalLength * pTimeIntervalLength = readTimeIntervalLength();
        pRegularTimeSeriesFile->setTimeIntervalLength( pTimeIntervalLength );
      }
      else
      if ( pActElementLoc->CompareTo( "numberOfHeaderLines" ) == 0 )
      {
        STR = validReader->ReadString();
        int anz = XmlConvert::ToInt32( STR );
        pRegularTimeSeriesFile->setNumberOfHeaderLines( anz );
      }
      else
      if ( pActElementLoc->CompareTo( "fieldSeparator" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pRegularTimeSeriesFile->setFieldSeparator( stdString );
      }
      else
      if ( pActElementLoc->CompareTo( "recordSeparator" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pRegularTimeSeriesFile->setRecordSeparator( stdString );
      }
      else
      if ( pActElementLoc->CompareTo( "decimalSymbol" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pRegularTimeSeriesFile->setDecimalSymbol( stdString );
      }
      else
      if ( pActElementLoc->CompareTo( "valueColumnNumber" ) == 0 )
      {
        STR = validReader->ReadString();
        int anz = XmlConvert::ToInt32( STR );
        pRegularTimeSeriesFile->setValueColumnNumber( anz );
      }
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement    = validReader->Name;
      if (pActElement->CompareTo ( QuConvert::ToString ( gmlAttributNameOrg ) ) == 0)
      {
        break;
      }
    }
  }

  ERR_TEXT = readCSVFile( pRegularTimeSeriesFile, pFolderName );

  if ( ERR_TEXT != nullptr )
  {
    STR = String::Concat( "Fehler bei Einlesen der CSV-Datei ", QuConvert::ToString( pRegularTimeSeriesFile->getFile() ), ERR_TEXT );
    MessageBox::Show ( STR );
  }

  return pRegularTimeSeriesFile;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest einen CSV-File                                                     //
///////////////////////////////////////////////////////////////////////////////
String ^  GMLReader::readCSVFile(RegularTimeSeriesFile* pRegularTimeSeriesFile, String^ pFolderName)
{
  int              i;
  int              lineNumber;
  double           value;
  String         ^ STR;
  StreamReader   ^ pReader;
  String         ^ pCSVFileName;
  cli::array<String^> ^ splittedString;
  cli::array<String^> ^ delimiter = gcnew cli::array<String^>(1);

  delimiter[0] = QuConvert::ToString( pRegularTimeSeriesFile->getFieldSeparator() );
  lineNumber   = 0;

  pCSVFileName =  QuConvert::ToString( pRegularTimeSeriesFile->getFile() );
  if ( !Path::IsPathRooted ( pCSVFileName ) )
    pCSVFileName = Path::Combine( pFolderName, pCSVFileName );

  try
  {
    System::Text::ASCIIEncoding ^ asciiEncoding = gcnew System::Text::ASCIIEncoding() ;
    pReader = gcnew StreamReader ( pCSVFileName, asciiEncoding );
  }
  catch ( Exception ^ )
  {
    return "CSV-Datei existiert nicht";
  }

  for ( i = 0; i < pRegularTimeSeriesFile->getNumberOfHeaderLines(); i++ )
  {
    STR = pReader->ReadLine();
    lineNumber++;
  }

  STR = pReader->ReadLine();
  lineNumber++;
  while ( STR )
  {
    try
    {
      splittedString = STR->Split( delimiter, StringSplitOptions::None );
      if ( splittedString->Length >= pRegularTimeSeriesFile->getValueColumnNumber() ) 
      {
        String ^valueString = splittedString[pRegularTimeSeriesFile->getValueColumnNumber()];
        valueString = valueString->Trim();
        if ( valueString->CompareTo( QuConvert::ToString( pRegularTimeSeriesFile->getMissingValue() ) ) != 0 )
        {
          valueString = valueString->Replace( QuConvert::ToString( pRegularTimeSeriesFile->getDecimalSymbol() ), "," );
          value = Convert::ToDouble( valueString );
        }
        else
          value = Convert::ToDouble( QuConvert::ToString( pRegularTimeSeriesFile->getMissingValue()  ) );
        pRegularTimeSeriesFile->addValue( value );
      }
    }
    catch ( Exception ^ )
    {
      pReader->Close();
      STR = String::Concat( "Falsche Syntax der CSV-Datei in Zeile ", Convert::ToString( lineNumber ) );
      return STR;
    }
    STR = pReader->ReadLine();
    lineNumber++;
  }
  pReader->Close();
  return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest energy:IrregularTimeSeriesFile                                     //
///////////////////////////////////////////////////////////////////////////////
AbstractTimeSeries * GMLReader::readIrregularTimeSeriesFile ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg )
{
  string   gmlAttributNameActLoc;
  string   gmlAttributNameOrgLoc;
  String ^ STR;
  string   stdString;

  if ( validReader->IsEmptyElement ) return NULL;

  IrregularTimeSeriesFile * pIrregularTimeSeriesFile = new IrregularTimeSeriesFile ( Constant::createGmlId() );

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      QuConvert::systemStr2stdStr ( gmlAttributNameActLoc, pActElement );
      gmlAttributNameOrgLoc = gmlAttributNameActLoc;
      gmlAttributNameActLoc = addDefaultNamespaceKuerzel ( gmlAttributNameActLoc );
      gmlAttributNameActLoc = createGmlNameStandard ( gmlAttributNameActLoc );

      if (  pActElementLoc->CompareTo( "variableProperties" ) == 0 )
        readTimeSeriesVariableProperties ( gmlAttributNameActLoc, pIrregularTimeSeriesFile, gmlAttributNameOrgLoc );
      else
      if ( pActElementLoc->CompareTo( "uom" ) == 0 )
      {
        STR = validReader->GetAttribute ( "uom" );
        QuConvert::systemStr2stdStr( stdString, STR );
        UOM * pUOM = pFeatures->getUOMList()->getUOMFromIdentifier( stdString );
        pIrregularTimeSeriesFile->setUOM ( pUOM );
      }
      else
      if ( pActElementLoc->CompareTo( "file" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pIrregularTimeSeriesFile->setFile( stdString );
      }
      else
      if ( pActElementLoc->CompareTo( "numberOfHeaderLines" ) == 0 )
      {
        STR = validReader->ReadString();
        int anz = XmlConvert::ToInt32( STR );
        pIrregularTimeSeriesFile->setNumberOfHeaderLines( anz );
      }
      else
      if ( pActElementLoc->CompareTo( "fieldSeparator" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pIrregularTimeSeriesFile->setFieldSeparator( stdString );
      }
      else
      if ( pActElementLoc->CompareTo( "recordSeparator" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pIrregularTimeSeriesFile->setRecordSeparator( stdString );
      }
      else
      if ( pActElementLoc->CompareTo( "decimalSymbol" ) == 0 )
      {
        STR = validReader->ReadString();
        char c = XmlConvert::ToByte( STR );
        pIrregularTimeSeriesFile->setDecimalSymbol( c );
      }
      else
      if ( pActElementLoc->CompareTo( "timeColumnNumber" ) == 0 )
      {
        STR = validReader->ReadString();
        int anz = XmlConvert::ToInt32( STR );
        pIrregularTimeSeriesFile->setTimeColumnNumber( anz );
      }
      else
      if ( pActElementLoc->CompareTo( "valueColumnNumber" ) == 0 )
      {
        STR = validReader->ReadString();
        int anz = XmlConvert::ToInt32( STR );
        pIrregularTimeSeriesFile->setValueColumnNumber( anz );
      }
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement    = validReader->Name;
      if (pActElement->CompareTo( QuConvert::ToString( gmlAttributNameOrg ) ) == 0)
      {
        break;
      }
    }
  }

 return pIrregularTimeSeriesFile;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur Energy ADE:                                                          //
//  Liest TimeSeriesVariableProperties                                       //
///////////////////////////////////////////////////////////////////////////////
void GMLReader::readTimeSeriesVariableProperties (  std::string gmlAttributNameAct, AbstractTimeSeries * pTimeSeries, std::string gmlAttributNameOrg )
{
  string   gmlAttributNameActLoc;
  string   gmlAttributNameOrgLoc;
  String ^ STR;
  string   stdString;

  if ( validReader->IsEmptyElement ) return;

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElement    = validReader->Name;
      pActElementLoc = validReader->LocalName;

      QuConvert::systemStr2stdStr ( gmlAttributNameActLoc, pActElement );
      gmlAttributNameOrgLoc = gmlAttributNameActLoc;
      gmlAttributNameActLoc = addDefaultNamespaceKuerzel ( gmlAttributNameActLoc );
      gmlAttributNameActLoc = createGmlNameStandard ( gmlAttributNameActLoc );

      if ( pActElementLoc->CompareTo( "acquisitionMethod" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pTimeSeries->setAquisitionMethod( stdString );
      }
      else
      if ( pActElementLoc->CompareTo( "interpolationType" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pTimeSeries->setInterpolationTypeFromString( stdString );
      }
      else
      if ( pActElementLoc->CompareTo( "qualityDescription" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pTimeSeries->setQualityDescription( stdString );
      }
      else
      if ( pActElementLoc->CompareTo( "source" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pTimeSeries->setSource( stdString );
      }
      else
      if ( pActElementLoc->CompareTo( "thematicDescription" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pTimeSeries->setThematicDescription( stdString );
      }
			else
			if ( pActElementLoc->CompareTo( "property" ) == 0 )
			{
				STR = validReader->ReadString();
				QuConvert::systemStr2stdStr( stdString, STR );
				pTimeSeries->setProperty( stdString );
			}
			else
			if ( pActElementLoc->CompareTo( "propertyQualifier" ) == 0 )
			{
				STR = validReader->ReadString();
				QuConvert::systemStr2stdStr( stdString, STR );
				pTimeSeries->setPropertyQualifier( stdString );
			}
		}
    else
    {
      if ( validReader->NodeType == XmlNodeType::EndElement )
      {
        pActElement    = validReader->Name;
        if (pActElement->CompareTo( QuConvert::ToString( gmlAttributNameOrg ) ) == 0 )
        {
          break;
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Nur INSPIRE:                                                             //
//  Generiert den vollständigen Pfad einer INSPIRE Externen Codeliste        //
///////////////////////////////////////////////////////////////////////////////
std::string GMLReader::generateExternalCodelistPath ( String ^ CODE_URI )
{
  string           codelistPfad;
  cli::array<String^> ^ splitString;

  int index = CODE_URI->LastIndexOf( '/' );
  if ( index == -1 )
  {
    QuConvert::systemStr2stdStr( codelistPfad, CODE_URI );
    return codelistPfad;
  }
  String ^ CODELIST_URI = CODE_URI->Substring( 0, index );

  splitString = CODELIST_URI->Split ( '/' );
  String ^ CODELIST_NAME = splitString[splitString->Length-1];

  String ^ CODELIST_PFAD = String::Concat( CODELIST_URI, "/", CODELIST_NAME, INSPIRECodelistSuffix );
  QuConvert::systemStr2stdStr( codelistPfad, CODELIST_PFAD );

  return codelistPfad;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  class Appearances                                                        //
//  Aggregation von CityGML Appearances                                      //
///////////////////////////////////////////////////////////////////////////////
Appearances::Appearances(  Features * pFeaturesP )
{
  pFeatures        = pFeaturesP;
  pDefaultAppearance = NULL;
}

Appearances::~Appearances()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Objekt AppearanceData hinzu                                         //
///////////////////////////////////////////////////////////////////////////////
void Appearances::addAppearanceData ( AppearanceData * pAppearData )
{
  unsigned int id   = pAppearData->getAppearDataId();
  string       gmlId = pAppearData->getGmlId();

  m_appearDataIds[gmlId] = id;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Objekt TexCoord hinzu                                               //
///////////////////////////////////////////////////////////////////////////////
void Appearances::addTexCoord ( TexCoord * pTexCoord )
{
  unsigned int id = pTexCoord->getTexCoordId();
  string     gmlId = pTexCoord->getGmlId();

  m_texCoordIds[gmlId] = id;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Texturkoordinaten-Id                                        //
///////////////////////////////////////////////////////////////////////////////
unsigned int Appearances::getTexCoordId ( std::string gmlId )
{
  map<string ,unsigned int>::iterator iter;

  iter = m_texCoordIds.find ( gmlId );
  if ( iter != m_texCoordIds.end() ) return iter->second;
  else                               return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Fügt ein Appearance Objekt hinzu                                          //
///////////////////////////////////////////////////////////////////////////////
void Appearances::addAppearanceRef ( cAppearance * pAppearance, std::string gmlId )
{
  unsigned int id = pAppearance->getAppearId();
  m_appearRefs.insert ( multimap<unsigned int, std::string>::value_type( id, gmlId ) );
}

///////////////////////////////////////////////////////////////////////////////
// Liefert zu einer Appearance die Ids der zug. AppearanceData               //
///////////////////////////////////////////////////////////////////////////////
size_t  Appearances::getAppearanceRefs ( cAppearance * pAppearance, vector<unsigned int> & appearDataIds )
{
  multimap<unsigned int, std::string>::iterator iter;
  unsigned int id = pAppearance->getAppearId();
  string        gmlId;
  unsigned int idData;

  appearDataIds.clear();

  for ( iter  = m_appearRefs.lower_bound ( id );
        iter != m_appearRefs.upper_bound ( id ); iter++ )
  {
    gmlId = iter->second;
    idData = m_appearDataIds[gmlId];
    appearDataIds.push_back ( idData );
  }

  return appearDataIds.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt AppearanceData hinzu                                                //
///////////////////////////////////////////////////////////////////////////////
void Appearances::addAppearanceDataRef ( AppearanceData * pAppearanceData, std::string geoGmlId )
{
  AppearDataRef ref;
  unsigned int id = pAppearanceData->getAppearDataId();

  ref.m_geoGmlId      = geoGmlId;
  ref.m_texCoordId    = 0;
  ref.m_texCoordGmlId = _T("");

  m_appearDataRefs.insert ( multimap<unsigned int, AppearDataRef>::value_type( id, ref ) );
}

void Appearances::addAppearanceDataRef ( AppearanceData * pAppearanceData, std::string geoGmlId, std::string texCoordGmlId )
{
  AppearDataRef ref;
  unsigned int id = pAppearanceData->getAppearDataId();

  ref.m_geoGmlId      = geoGmlId;
  ref.m_texCoordId    = 0;
  ref.m_texCoordGmlId = texCoordGmlId;

  m_appearDataRefs.insert ( multimap<unsigned int, AppearDataRef>::value_type( id, ref ) );
}

void Appearances::addAppearanceDataRef ( AppearanceData * pAppearanceData, std::string geoGmlId, unsigned int texCoordId )
{
  AppearDataRef ref;
  unsigned int id = pAppearanceData->getAppearDataId();

  ref.m_geoGmlId      = geoGmlId;
  ref.m_texCoordId    = texCoordId;
  ref.m_texCoordGmlId = _T("");

  m_appearDataRefs.insert ( multimap<unsigned int, AppearDataRef>::value_type( id, ref ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die referierten AppearanceData                                   //
///////////////////////////////////////////////////////////////////////////////
size_t Appearances::getAppearanceDataRefs ( AppearanceData * pAppearanceData, 
                                         vector<AppearDataRef> & appearDataRefs )
{
  multimap<unsigned int, AppearDataRef>::iterator iter;
  unsigned int id = pAppearanceData->getAppearDataId();
  AppearDataRef ref;

  appearDataRefs.clear();

  for ( iter  = m_appearDataRefs.lower_bound ( id );
        iter != m_appearDataRefs.upper_bound ( id ); iter++ )
  {
    ref = iter->second;
    appearDataRefs.push_back ( ref );
  }

  return appearDataRefs.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Auflösung offener Referenzen                                             //
///////////////////////////////////////////////////////////////////////////////
void Appearances::resolveReferences()
{
	size_t                  i, j, k, l, appearanceAnz, appearDataAnz, appearDataRefAnz, appearAnz, faceAnz;
  vector<cAppearance*>    appearances;
  vector<AppearanceData*> appearData;
  vector<unsigned int>    appearDataIds;
  vector<AppearDataRef>   appearDataRefs;
  vector<unsigned int>    idAppears;
  vector<Feature*>        features;
  vector<_Geometrie*>     vRepresentations;
  unsigned int            idAppearData;
  cAppearance           * pAppearance = nullptr;
  AppearanceData        * pAppearData;
  AppearDataRef           ref;
  _Geometrie            * pGeometry;
  TexCoord              * pTexCoord;
  string                  thema;
  string                  gmlId;
  vector<PolygonGeo*>     vFaces;

  appearanceAnz = pFeatures->getAllAppearances ( appearances );
  for ( i = 0; i < appearanceAnz; i++ )
  {
    pAppearance = appearances[i];
    appearDataAnz = getAppearanceRefs ( pAppearance, appearDataIds );
    for ( j = 0; j < appearDataAnz; j++ )
    {
      idAppearData =  appearDataIds[j];
      addAppearDataAppearRef ( idAppearData, pAppearance->getAppearId() );
    }
  }

  appearDataAnz = pFeatures->getAllAppearanceData ( appearData );
  for ( i = 0; i < appearDataAnz; i++ )
  {
    pAppearData = appearData[i];
    appearDataRefAnz = getAppearanceDataRefs ( pAppearData, appearDataRefs );

    appearAnz = getAppearDataAppearRefs ( pAppearData->getAppearDataId(), idAppears );

    for ( j = 0; j < appearDataRefAnz; j++ )
    {
      ref = appearDataRefs[j];
      pGeometry = getGeometry ( ref.m_geoGmlId );
      if ( ref.m_texCoordId != 0 )
        pTexCoord = pFeatures->getTexCoord ( ref.m_texCoordId );
      else
      if ( ref.m_texCoordGmlId != _T("") )
      {
        ref.m_texCoordId = getTexCoordId ( ref.m_texCoordGmlId );
        pTexCoord = pFeatures->getTexCoord ( ref.m_texCoordId );
      }
      else
        pTexCoord = NULL;

      if ( pGeometry != NULL )
      {
        if ( pAppearData->getAppearanceDataType() == AppearanceData::GEOREFERENCED_TEXTURE )
        {
          vFaces.clear();
          faceAnz = getAllFaces ( pGeometry, vFaces );
          for ( k = 0; k < faceAnz; k++ )
          {
            PolygonGeo * pFace = vFaces[k];
            createTextureCoordinates ( pFace, (AppearanceGeoreferencedTexture*)pAppearData );
          }
        }
        else
        if ( pAppearData->getAppearanceDataType() == AppearanceData::MATERIAL )
        {
          if ( pGeometry->getGeometryType() == POLYGON )
          {
            PolygonGeo * pPolygon = dynamic_cast<PolygonGeo*>(pGeometry);
            pFeatures->addGeoAppearanceMaterial ( pPolygon->getAussenkontur()->getGmlId(), 
                                pAppearance->getThema(), pAppearData->getAppearDataId(), true );         
          }
          else
          if ( pGeometry->getGeometryType() == LINEAR_RING ||
               pGeometry->getGeometryType() == RING )
              pFeatures->addGeoAppearanceMaterial ( pGeometry->getGmlId(), pAppearance->getThema(), 
                                                  pAppearData->getAppearDataId(), true );         
          else
          {
            vFaces.clear();
            faceAnz = getAllFaces ( pGeometry, vFaces );
            pAppearance = pFeatures->getAppearance ( pAppearData->getAppearDataId() );

            for ( k = 0; k < faceAnz; k++ )
            {
              PolygonGeo * pFace = vFaces[k];
              for ( l = 0; l < appearAnz; l++ )
              {
                idAppearData = idAppears[l];
                pAppearance = pFeatures->getAppearance ( idAppearData );

                pFeatures->addGeoAppearanceMaterial ( pFace->getAussenkontur()->getGmlId(), pAppearance->getThema(), 
                                                      pAppearData->getAppearDataId(), false ); 
              }
            }
          }
        }
        else
        if ( pTexCoord != NULL                                                      &&
             pTexCoord->getTextureCoordType() == TexCoord::TEXTURE_COORD_GENERATION &&
             pAppearData->getAppearanceDataType() == AppearanceData::PARAMETRIZED_TEXTURE )
        {
          vFaces.clear();
          faceAnz = getAllFaces ( pGeometry, vFaces );
          for ( k = 0; k < faceAnz; k++ )
          {
            PolygonGeo * pFace = vFaces[k];
            createTextureCoordinates ( pFace, (AppearanceParametrizedTexture*)pAppearData, (TexCoordGen*)pTexCoord );
          }
        }

        for ( k = 0; k < appearAnz; k++ )
        {
          idAppearData = idAppears[k];
          pAppearance = pFeatures->getAppearance ( idAppearData );

          pFeatures->addGeoAppearanceTexture ( pGeometry->getGmlId(),
                                               pAppearance->getThema(),
                                               pAppearData->getAppearDataId(),
                                               ref.m_texCoordId );
        }
      }
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die Texturkoordinaten für ein bestimmtes Polygon               //
///////////////////////////////////////////////////////////////////////////////
void Appearances::createTextureCoordinates ( PolygonGeo * pFace, AppearanceGeoreferencedTexture * pAppearData )
{
  _Ring               * pPolyloop;
  vector<double>        vX;
  vector<double>        vY;
  double                x, y, tx, ty;
  double                orientation[4];
  GeoPoint            * refPoint;
  vector<double>        texCoordinates;
  vector<unsigned int> idAppears;
	size_t               i, k, appearAnz, anz;
  cAppearance         * pAppearance;
  unsigned int         idAppear;

  refPoint = pAppearData->getReferencePoint ();
  pAppearData->getOrientation ( orientation );

  pPolyloop = pFace->getAussenkontur();
  if ( pPolyloop == NULL ) return;

  anz = pPolyloop->getPoints2D ( vX, vY );
  for ( i = 0; i < anz; i++ )
  {
    x = vX[i] - refPoint->getX();
    y = vY[i] - refPoint->getY();
    tx = orientation[0]* x + orientation[2]*y;
    ty = orientation[1]*x + orientation[3]*y;
    texCoordinates.push_back ( tx );
    texCoordinates.push_back ( ty );
  }

  TexCoordPolyloop * pTexCoordPolyloop = new TexCoordPolyloop (texCoordinates );
  pFeatures->addTexCoord ( pTexCoordPolyloop );

  appearAnz = getAppearDataAppearRefs ( pAppearData->getAppearDataId(), idAppears );

  for ( k = 0; k < appearAnz; k++ )
  {
    idAppear = idAppears[k];
    pAppearance = pFeatures->getAppearance ( idAppear );

    pFeatures->addGeoAppearanceTexture ( pPolyloop->getGmlId(),
                                         pAppearance->getThema(),
                                         pAppearData->getAppearDataId(),
                                         pTexCoordPolyloop->getTexCoordId() );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die Texturkoordinaten für ein bestimmtes Polygon               //
///////////////////////////////////////////////////////////////////////////////
void Appearances::createTextureCoordinates ( PolygonGeo * pFace, AppearanceParametrizedTexture * pAppearData,
                                             TexCoordGen * pTexCoordGen )
{
  _Ring               * pPolyloop;
  vector<double>        vX;
  vector<double>        vY;
  vector<double>        vZ;
  Matrix4             * matrix;
  vector<double>        texCoordinates;
  vector<unsigned int>  idAppears;
	size_t                i, k, appearAnz, anz;
  cAppearance         * pAppearance;
  unsigned int          idAppear;
  GeoPoint               p;
  GeoPoint               tp;

  matrix = pTexCoordGen->getTransformation ();
  if ( matrix == NULL ) return;

  pPolyloop = pFace->getAussenkontur();
  if ( pPolyloop == NULL ) return;

  anz = pPolyloop->getPoints3D ( vX, vY, vZ );
  for ( i = 0; i < anz; i++ )
  {
    p = GeoPoint ( vX[i], vY[i], vZ[i] );
    tp = matrix->transform( p );

    texCoordinates.push_back ( tp.getX() );
    texCoordinates.push_back ( tp.getY() );
  }

  TexCoordPolyloop * pTexCoordPolyloop = new TexCoordPolyloop (texCoordinates );
  pFeatures->addTexCoord ( pTexCoordPolyloop );

  appearAnz = getAppearDataAppearRefs ( pAppearData->getAppearDataId(), idAppears );

  for ( k = 0; k < appearAnz; k++ )
  {
    idAppear = idAppears[k];
    pAppearance = pFeatures->getAppearance ( idAppear );

    pFeatures->addGeoAppearanceTexture ( pPolyloop->getGmlId(),
                                         pAppearance->getThema(),
                                         pAppearData->getAppearDataId(),
                                         pTexCoordPolyloop->getTexCoordId() );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Geometrieobjekt zur map hinzu                                   //
///////////////////////////////////////////////////////////////////////////////
void Appearances::addGeometry ( string gmlId, _Geometrie * pGeometry )
{
  m_geoMap[gmlId]           = pGeometry;
  m_inversGeoMap[pGeometry] = gmlId;
}

///////////////////////////////////////////////////////////////////////////////
// Liefert ein Geometrieobjekt aus der map                                   //
///////////////////////////////////////////////////////////////////////////////
_Geometrie * Appearances::getGeometry ( string gmlId )
{
  map<string,_Geometrie*>::iterator iter;

  iter = m_geoMap.find ( gmlId );
  if ( iter != m_geoMap.end() ) return iter->second;
  else                          return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Liefert die gml:id, unter der das Geometrieobjekt registriert ist         //
///////////////////////////////////////////////////////////////////////////////
std::string Appearances::getGmlId ( _Geometrie * pGeometry )
{
  map<_Geometrie*,string>::iterator iter;
  iter = m_inversGeoMap.find ( pGeometry );
  if ( iter != m_inversGeoMap.end() ) return iter->second;
  else                                return _T("");
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt ein Geometrieobjekt aus der map                                 //
///////////////////////////////////////////////////////////////////////////////
_Geometrie * Appearances::removeGeometry ( std::string gmlId )
{
  map<string,_Geometrie*>::iterator iter;
  _Geometrie                      * pGeometry;

  iter = m_geoMap.find ( gmlId );
  if ( iter != m_geoMap.end() )
  {
    pGeometry = iter->second;
    m_geoMap.erase ( iter );
    m_inversGeoMap.erase ( pGeometry );
  }
  else
    pGeometry = NULL;

  return pGeometry;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Referenzen zwischen AppearanceData und Appearance hinzu             //
///////////////////////////////////////////////////////////////////////////////
void Appearances::addAppearDataAppearRef ( unsigned int idAppearData, unsigned int idAppear)
{
  m_appearDataAppearRefs.insert ( multimap<unsigned int, unsigned int>::value_type( idAppearData, idAppear ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Referenzen zwischen AppearanceData und Appearance                //
///////////////////////////////////////////////////////////////////////////////
size_t Appearances::getAppearDataAppearRefs ( unsigned int idAppearData, vector<unsigned int> & idAppears )
{
  multimap<unsigned int, unsigned int>::iterator iter;

  idAppears.clear();
  for ( iter  = m_appearDataAppearRefs.lower_bound ( idAppearData );
        iter != m_appearDataAppearRefs.upper_bound ( idAppearData ); iter++  )
  {
    idAppears.push_back ( iter->second );
  }

  return idAppears.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine einfache Textur hinzu                                          //
///////////////////////////////////////////////////////////////////////////////
void Appearances::addSimpleTexture ( std::string textureMap, unsigned int id, bool orientation )
{
  if ( orientation )
    m_simpleTextureFrontMap[textureMap] = id;
  else
    m_simpleTextureBackMap[textureMap] = id;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine einfache Textur                                             //
///////////////////////////////////////////////////////////////////////////////
unsigned int Appearances::getSimpleTexture ( std::string textureMap,  bool orientation )
{
  map<string,unsigned int>::iterator iter;

  if ( orientation )
  {
    iter = m_simpleTextureFrontMap.find ( textureMap );
    if ( iter != m_simpleTextureFrontMap.end() ) return iter->second;
    else                                         return 0;
  }
  else
  {
    iter = m_simpleTextureBackMap.find ( textureMap );
    if ( iter != m_simpleTextureBackMap.end() ) return iter->second;
    else                                        return NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Faces eines GML-Geometrieobjektes                           //
///////////////////////////////////////////////////////////////////////////////
size_t Appearances::getAllFaces ( _Geometrie * pGeometry, vector<PolygonGeo*> &vFaces )
{
  vector<bool> vOrientations;

  if ( pGeometry == NULL)
      return 0;

  switch ( pGeometry->getGeometryType() )
  {
      case MULTI_SURFACE:
      case SHELL:
      case COMPOSITE_SURFACE:
      case SURFACE:
      case TRIANGULATED_SURFACE:
      {
        _Surface* pSurface = dynamic_cast<_Surface*> ( pGeometry );
        pSurface->getPolygone( vFaces );
        
        break;
      }

      case MULTI_SOLID:
      case COMPOSITE_SOLID:
      case SOLID:
      {
        _Solid* pSolid = dynamic_cast<_Solid*> ( pGeometry );
        pSolid->getAussenkonturPolygone( vFaces, vOrientations );
        
        break;
      }

      case POLYGON:
      {
        PolygonGeo * pFace = dynamic_cast<PolygonGeo*> ( pGeometry);

        if ( pFace->getAussenkontur() != NULL )
          vFaces.push_back ( pFace );

        break;
      }
  }

  return vFaces.size();
}


///////////////////////////////////////////////////////////////////////////////
//  class AppearDataRef                                                      //
///////////////////////////////////////////////////////////////////////////////
AppearDataRef::AppearDataRef()
{
  m_geoGmlId      = _T("");
  m_texCoordGmlId = _T("");
  m_texCoordId    = 0;
}

AppearDataRef::~ AppearDataRef()
{
}







