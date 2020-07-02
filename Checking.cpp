#include "StdAfx.h"

using namespace System;
using namespace System::IO;

#include <time.h>
#include <vector>
#include <map>
#include <string>
#include <set>

#include "Convert.h"
#include "constant.h"
#include "GmlSchema.h"
#include "Dictionary.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "GmlModelTest.h"
#include "UnitOfMeasure.h"
#include "Logfile.h"
#include "Checking.h"
#include "CheckingRef.h"

using namespace std;

class SemanticRules;
class ErrorReport;

///////////////////////////////////////////////////////////////////////////////
//  Checking                                                                 //
//  Überprüfung von Instanz-Dokumenten                                       //
///////////////////////////////////////////////////////////////////////////////
Checking::Checking ( Features * pFeatures, UOMList * pUOMList, SrsManager * pSrsManager, Logfile * pLogfile )
{
  m_pErrorReport     = new ErrorReport ( this );

  m_pCheckParams = new CheckingParams;
  m_pCheckParams->minimalAngleDistance = 1.0E-3;

  m_pFeatures        = pFeatures;
  m_pUOMList         = pUOMList;
  m_pSrsManager      = pSrsManager;
  m_pLogfile         = pLogfile;

  m_pRules           = NULL;
  m_pProfileRules    = NULL;
  m_pModelStatistics = NULL;

  checkSemanticRules         = true;
  checkCodeLists             = false;
  checkUnitsOfMeasurement    = false;
  checkReferenzen            = true;
  checkCoordReferenceSystems = true;

  checkXPlanGMLRules           = true;
  checkXPlanGMLBackwardPointer = true;
  checkXPlanGMLUOM             = true;

  fullStatistics               = true;

  geometryCheckLevel             = BASIC_CHECKS;

  checkFlaechenschlussGrob       = true;
  checkFlaechenschlussExakt      = false;
  checkVollstaendigeUeberdeckung = true;

  checkBoundarySurfaces  = true;
  checkOuterShell        = true;
}

Checking::~Checking()
{
  delete m_pCheckParams;

  if ( m_pRules != NULL )
    delete m_pRules;

  if ( m_pModelStatistics != NULL )
    delete m_pModelStatistics;

  delete m_pErrorReport;
}

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert eine neue Modell-Überprüfung                               //
///////////////////////////////////////////////////////////////////////////////
void Checking::initialize()
{
  if ( m_pRules != NULL ) delete m_pRules;
  m_pRules = new SemanticRules ( m_pFeatures, m_pCheckParams, m_pErrorReport );
  m_pRules->initialize();

  m_pErrorReport->resetGeometryErrors();
}

///////////////////////////////////////////////////////////////////////////////
//  Liest vordefinierte Konformitätsregeln ein                               //
///////////////////////////////////////////////////////////////////////////////
void Checking::readProfileRules ( string profileName )
{
  string profileRuleFolder;
  string fileName;

  if ( profileName == "" ) return;

  if ( m_pProfileRules != NULL )
    delete m_pProfileRules;

  m_pProfileRules = new SemanticRules ( m_pFeatures, m_pCheckParams, m_pErrorReport );

  profileRuleFolder = m_pFeatures->getGmlSchema()->getSchemaFolder() + "\\Profile\\" + profileName + "\\Rules\\";

  String ^ PROFILE_RULE_FOLDER = QuConvert::ToString ( profileRuleFolder );
  bool exist = Directory::Exists ( PROFILE_RULE_FOLDER );
  if ( exist )
  {
    ProfileReader ^ pProfileReader = gcnew ProfileReader ( m_pProfileRules );

    cli::array<String^> ^FILES = Directory::GetFiles ( PROFILE_RULE_FOLDER );

    IEnumerator ^ FILES_ENUM = FILES->GetEnumerator();
    while ( FILES_ENUM->MoveNext() )
    {
      String^ FILE_NAME = safe_cast<String^>(FILES_ENUM->Current);
      QuConvert::systemStr2stdStr ( fileName, FILE_NAME );
      pProfileReader->read ( fileName );
    }
  } 
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Validierungsfehler-Bereich des ErrorReports zurück             //
///////////////////////////////////////////////////////////////////////////////
void Checking::resetValidation()
{
  m_pErrorReport->resetValidationErrors();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Semantikfehler-Bereich des ErrorReports zurück                 //
///////////////////////////////////////////////////////////////////////////////
void Checking::resetSemanticErrors()
{
  m_pErrorReport->resetSemanticErrors();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Geometriefehler-Bereich des ErrorReports zurück                //
///////////////////////////////////////////////////////////////////////////////
void Checking::resetGeometryErrors()
{
  m_pErrorReport->resetGeometryErrors();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Einlesefehler-Bereich des ErrorReports zurück                  //
///////////////////////////////////////////////////////////////////////////////
void Checking::resetReadErrors()
{
  m_pErrorReport->resetReadErrors();
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die Modell-Statistik                                           //
///////////////////////////////////////////////////////////////////////////////
void Checking::GenerateModelStatistics( std::string outputFile )
{
  if ( m_pModelStatistics != NULL )
    delete m_pModelStatistics;
  m_pModelStatistics = new GmlModelStatistics;
  
  GmlModelStatisticsGeneration ^ pModelStatisticsGeneration = gcnew GmlModelStatisticsGeneration ( m_pModelStatistics );
  
  pModelStatisticsGeneration->setFullStatistics( fullStatistics );

  if ( outputFile != "" )
    pModelStatisticsGeneration->generateStatistics( m_pFeatures, outputFile );
  else
    pModelStatisticsGeneration->generateStatistics( m_pFeatures );
}

///////////////////////////////////////////////////////////////////////////////
//  Validiert ein GML-Dokument gegen das XML-Schema                          //
///////////////////////////////////////////////////////////////////////////////
void Checking::ValidateModelAgainstXMLSchema( std::string fileName, string profileName )
{
  //resetValidation();
  SchemaValidierung ^ pSchemaValidierung = gcnew SchemaValidierung (  m_pFeatures->getGmlSchema(),
          m_pFeatures->getGmlSchema()->getGmlNamespacesStandard(), m_pErrorReport, fileName );

  if ( profileName != "" )
    pSchemaValidierung->ValidateProfile ( profileName );
  else
    pSchemaValidierung->Validate();

}

///////////////////////////////////////////////////////////////////////////////
//  Validiert ein GML-Dokument gegen Schematron-Regeln                       //
///////////////////////////////////////////////////////////////////////////////
void Checking::ValidateModelAgainstSchematronRules( string fileName, string profileName )
{
  String ^ schematronTransformationPath;
  String ^ schematronErrorPath;
  String ^ inputPath = QuConvert::ToString ( fileName );


  if ( profileName != ""  )
  {
    schematronTransformationPath = QuConvert::ToString ( m_pFeatures->getGmlSchema()->getSchemaFolder() + "Profile\\" + profileName + "\\Schematron\\ConformanceRules.xsl" );
    schematronErrorPath          = QuConvert::ToString ( m_pFeatures->getGmlSchema()->getSchemaFolder() + "Profile\\" + profileName + "\\Schematron\\SchematronErrors.xml" );
  }
  else
  {
    schematronTransformationPath = QuConvert::ToString ( m_pFeatures->getGmlSchema()->getSchemaFolder() + "Schematron\\ConformanceRules.xsl" );
    schematronErrorPath          = QuConvert::ToString ( m_pFeatures->getGmlSchema()->getSchemaFolder() + "Schematron\\SchematronErrors.xml" );
  }


  try
  {
    Xsl::XslCompiledTransform ^ xslTransformer = gcnew Xsl::XslCompiledTransform;
    xslTransformer->Load( schematronTransformationPath );
    xslTransformer->Transform( inputPath, schematronErrorPath );
  }
  catch ( System::Exception ^ )
  {
    return;
  }
  analyzeSchematromErrors ( schematronErrorPath );


/*

  System::Xml::XmlTextReader  ^ pDocumentReader;
  System::Xml::XmlTextReader  ^ pSchematronTransformationReader;

  Saxon::Api::XsltExecutable  ^ pExecutable;
  Saxon::Api::XsltTransformer ^ pTransformer;
  Saxon::Api::Serializer      ^ pSerializer;
  Saxon::Api::XdmNode         ^ pNode;
  Saxon::Api::Processor       ^ pProcessor = gcnew Saxon::Api::Processor;
  Saxon::Api::XsltCompiler    ^ pXsltCompiler = pProcessor->NewXsltCompiler();
  Saxon::Api::DocumentBuilder ^ pDocumentBuilder = pProcessor->NewDocumentBuilder();

  String                      ^ schematronTransformationPath;
  String                      ^ schematronErrorPath;

  if ( profileName != ""  )
  {
    schematronTransformationPath = QuConvert::ToString ( m_pFeatures->getGmlSchema()->getSchemaFolder() + "Profile\\" + profileName + "\\Schematron\\ConformanceRules.xsl" );
    schematronErrorPath          = QuConvert::ToString ( m_pFeatures->getGmlSchema()->getSchemaFolder() + "Profile\\" + profileName + "\\Schematron\\SchematronErrors.xml" );
  }
  else
  {
    schematronTransformationPath = QuConvert::ToString ( m_pFeatures->getGmlSchema()->getSchemaFolder() + "Schematron\\ConformanceRules.xsl" );
    schematronErrorPath          = QuConvert::ToString ( m_pFeatures->getGmlSchema()->getSchemaFolder() + "Schematron\\SchematronErrors.xml" );
  }

  if ( !System::IO::File::Exists( schematronTransformationPath ) )
  {
    ValidationError * pError = new ValidationError ( Error::ERROR, "Schematron-Stylesheet nicht gefunden", -1 );
    m_pErrorReport->addValidationError( pError );
    return;   
  }

  File::Delete( schematronErrorPath );

  pSerializer = gcnew Saxon::Api::Serializer;
  pSerializer->SetOutputProperty( Saxon::Api::Serializer::METHOD, "xml" );
  pSerializer->SetOutputProperty( Saxon::Api::Serializer::ENCODING, "iso-8859-1" );

  pSchematronTransformationReader = gcnew XmlTextReader ( schematronTransformationPath );
  pExecutable = pXsltCompiler->Compile( pSchematronTransformationReader );
  pTransformer = pExecutable->Load();

  pDocumentReader = gcnew XmlTextReader ( QuConvert::ToString( fileName ) );
  pNode = pDocumentBuilder->Build( pDocumentReader );
  pTransformer->InitialContextNode = pNode;
  pSerializer->SetOutputFile( schematronErrorPath );
  pTransformer->Run( pSerializer );
  pDocumentReader->Close();

  analyzeSchematromErrors ( schematronErrorPath );
  */
}


///////////////////////////////////////////////////////////////////////////////
// Analysiert die während der Schematron-Transformation erzeugte Datei und   //
//  extrahiert die Konformitätsfehler                                        //
///////////////////////////////////////////////////////////////////////////////
void Checking::analyzeSchematromErrors ( String ^ schematronErrorPath )
{
  XmlTextReader   ^ validReader = gcnew XmlTextReader( schematronErrorPath );
  String          ^ pActElementLoc;
  ValidationError * pError;
  bool              errorActive;
  String          ^ STR;
  string            stdString;

  while( validReader->Read() )
  {
    if ( validReader->NodeType == XmlNodeType::Element )
    {
      pActElementLoc = validReader->LocalName;

      if ( pActElementLoc->CompareTo( "failed-assert" ) == 0 || 
           pActElementLoc->CompareTo( "successful-report" ) == 0 )
           errorActive = true;
      else
      if ( pActElementLoc->CompareTo( "text" ) == 0 && errorActive )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pError = new ValidationError ( Error::ERROR, stdString, -1 );
        m_pErrorReport->addValidationError( pError );
      }
    }
    else
    if ( validReader->NodeType == XmlNodeType::EndElement )
    {
      if ( pActElementLoc->CompareTo( "failed-assert" ) == 0 || 
           pActElementLoc->CompareTo( "successful-report" ) == 0 )
        errorActive = false;
    }
  }
  validReader->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die semantischen Regeln                                        //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckSemanticRules( std::string profileName )
{
  vector<Feature*> vFeatures;
	size_t           i, anz;

  if ( profileName != "" )
    readProfileRules( profileName );

 anz = m_pFeatures->getAllFeatures( vFeatures );
 for ( i = 0; i < anz; i++ )
  {
    Feature * pFeature = vFeatures[i];
    m_pRules->checkRules( pFeature );
  }

  if ( m_pProfileRules != NULL )
  {
    for ( i = 0; i < anz; i++ )
    {
      Feature * pFeature = vFeatures[i];
      m_pProfileRules->checkRules( pFeature );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Validiert ein Gültigkeit von Codelist-Attributen                         //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckCodeLists()
{
  vector<Feature*>     vFeatures;
  vector<GmlAttribut*> vGmlAttributeAll;
  vector<GmlAttribut*> vEnumAttribute;
  GmlSchema          * pSchema = m_pFeatures->getGmlSchema();
	size_t               i, j, anz, attributAnz;

  anz = m_pFeatures->getAllFeatures( vFeatures );
  for ( i = 0; i < anz; i++ )
  {
    Feature * pFeature = vFeatures[i];

    string klassenName = pFeature->getKlassenName();
    GmlKlasse *  pGmlKlasse  = pSchema->getGmlKlasseFromElementName ( klassenName );

    if ( pGmlKlasse == NULL ) continue;

    vGmlAttributeAll.clear();
    vEnumAttribute.clear();

    attributAnz = pGmlKlasse->getGmlAttribute ( vGmlAttributeAll );
    for ( j = 0; j < attributAnz; j++ )
    {
      GmlAttribut *  pGmlAttribut = vGmlAttributeAll[j];
      ATTRIBUT_TYP typ = pGmlAttribut->getGmlAttributTyp();
      if (  typ == _EXTERNAL_CODE_LIST )
        vEnumAttribute.push_back ( pGmlAttribut );
    }
    CheckEnumValues ( pFeature, vEnumAttribute );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft ob alle Attribute vom Typ  EXTERNAL_CODE_LIST                  //
//  Werte haben, die in den zug. Dictionaries enthalen sind			        		 //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckEnumValues ( Feature * pFeature, std::vector<GmlAttribut*> vEnumAttribute )
{
	unsigned int         i, j;
	string               attributName;
	vector<string>       vAttributWerte;
  vector<string>       vCodeSpaces;
	string               enumerationName;
	GmlAttribut        * pAttribut;
	GmlDictionaryEntry * pDictionaryEntry;
	SemanticError      * pError;
  vector<UOM*>         vUOMs;

	GmlDictionary * pCodeLists   = m_pFeatures->getGmlSchema()->getCodeListsDictionary();
	GmlSchema     * pGmlObjekte  = m_pFeatures->getGmlSchema();

	for ( i = 0; i < vEnumAttribute.size(); i++ )
	{
		pAttribut = vEnumAttribute[i];
		attributName = pAttribut->getGmlAttributName();
   
    vAttributWerte.clear();
    vCodeSpaces.clear();
		size_t anz = pFeature->getAttributWerte ( _EXTERNAL_CODE_LIST, attributName, vAttributWerte, vCodeSpaces, vUOMs );
    if ( anz == 0 )
      continue;

    for ( j = 0; j < vAttributWerte.size(); j++ )
    {
      if ( vCodeSpaces[j] != "" )
      {
        pDictionaryEntry = pCodeLists->getDictionaryEntry ( vCodeSpaces[j] );
        if ( pDictionaryEntry != NULL )
        {
          if ( pDictionaryEntry->existSchluesselNummer ( vAttributWerte[j] ) == false )
          {
            pError = new SemanticError ( pFeature, Error::ERROR,  "Ungültiger Wert eines Codelist-Attributs" );
            pError->setAttribute ( attributName, vAttributWerte[j] );
            pError->setEnumError ( vCodeSpaces[j], attributName, vAttributWerte[j] );
            m_pErrorReport->addSemanticError ( pError );
          }
        }
        else
        {
          pError = new SemanticError ( pFeature, Error::WARNING,  "Kein Zugriff auf Codeliste möglich" );
          pError->setEnumError ( vCodeSpaces[j], attributName, vAttributWerte[j] );
          m_pErrorReport->addSemanticError ( pError );
        }
      }
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Gültigkeit von UOM-Angaben                                 //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckUnitsOfMeasurement()
{
  vector<Feature*> vFeatures;
	size_t           i, anz;

  anz = m_pFeatures->getAllFeatures( vFeatures );
  for ( i = 0; i < anz; i++ )
  {
    Feature * pFeature = vFeatures[i];
    m_pRules->checkUnitsOfMeasurement( pFeature );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft alle Referenzen im Modell                                      //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckReferenzen()
{
  vector<Feature*> vFeatures;
	size_t           i, anz;

  anz = m_pFeatures->getAllFeatures( vFeatures );
  for ( i = 0; i < anz; i++ )
  {
    Feature * pFeature = vFeatures[i];
    CheckReferenzen ( pFeature );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Referenzen eines Features                                  //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckReferenzen ( Feature * pFeature )
{
  Features * pFeatures = pFeature->getFeatures();
  vector<FeatureRelation*> vRels;

  pFeature->getFeatureRelationen( vRels );
  for ( unsigned int i = 0; i < vRels.size(); i++ )
  {
    FeatureRelation * pRel = vRels[i];
    if ( pRel->nil )
      continue;

    Feature * pFeatureRel = pFeatures->getFeature( pRel->featureId );
    if ( pFeatureRel == NULL )
    {
      SemanticError * pError = new SemanticError ( pFeature, Error::ERROR, "Fehlerhafte Relation" );
      pError->setRelatedFeatureId( pRel->featureId );
      pError->setRelationName( pRel->rollenName );
      m_pErrorReport->addSemanticError( pError );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML                                                             //
//  Überprüft XPlanGML-spezifische Regeln                                    //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckXPlanGMLRules()
{
  if ( checkXPlanGMLBackwardPointer )
    CheckXPlanGMLBackwardPointer();

  if ( checkXPlanGMLUOM )
    CheckUnitsOfMeasurement();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML                                                             //
//  Überprüft die Existenz von Rückwärts-Pointern Planobjekt-Bereich         //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckXPlanGMLBackwardPointer()
{
  vector<BLeitPlan*>     vPlaene;
  vector<PlanBereich*>   vPlanBereiche;
  BLeitPlan            * pPlanLoc;
  PlanBereich          * pPlanBereich;
	size_t                 i, j, anzPlaene, anzBereiche;

  anzPlaene = m_pFeatures->getPlaene( vPlaene );

  for ( i = 0; i < anzPlaene; i++ )
  {
    pPlanLoc = vPlaene[i];
    vPlanBereiche.clear();
    anzBereiche = pPlanLoc->getPlanBereiche( vPlanBereiche );
    for ( j = 0; j < anzBereiche; j++ )
    {
      pPlanBereich = vPlanBereiche[j];
      CheckBereichRelation ( pPlanBereich, "xplan:nachrichtlich", "xplan:gehoertNachrichtlichZuBereich" );
      CheckBereichRelation ( pPlanBereich, "xplan:planinhalt", "xplan:gehoertZuBereich" );

      switch ( pPlanBereich->getBPlan()->getPlanTyp() )
      {
        case BPLAN:
          CheckBereichRelation ( pPlanBereich, "xplan:inhaltBPlan", "xplan:gehoertZuBP_Bereich" );
          break;

        case FPLAN:
          CheckBereichRelation ( pPlanBereich, "xplan:inhaltFPlan", "xplan:gehoertZuFP_Bereich" );
          break;

        case LPLAN:
          CheckBereichRelation ( pPlanBereich, "xplan:inhaltLPlan", "xplan:gehoertZuLP_Bereich" );
          break;

        case RPLAN:
          CheckBereichRelation ( pPlanBereich, "xplan:inhaltRPlan", "xplan:gehoertZuRP_Bereich" );
          break;

        case SOPLAN:
          CheckBereichRelation ( pPlanBereich, "xplan:inhaltSoPlan", "xplan:gehoertZuSO_Bereich" );
          break;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML                                                             //
//  Überprüft Relation Planobjekt-Bereich                                    //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckBereichRelation ( PlanBereich * pPlanBereich, std::string relationName, string inverseRelationName )
{
  vector<FeatureRelation*> vPlanObjektRelationen;
  vector<FeatureRelation*> vPlanObjektInverseRelationen;
  FeatureRelation         * pRel;
  FeatureRelation         * pRelInvers;
  Feature                 * pPlanObjekt;
  Feature                 * pPlanObjektInvers;
  SemanticError           * pError;
	size_t                    i, j, anz, anzInvers, anzGefunden;
  string                    errorString;

  anz = pPlanBereich->getFeatureRelationen( relationName, vPlanObjektRelationen );

  for ( i = 0; i < anz; i++ )
  {
    pRel = vPlanObjektRelationen[i];

    pPlanObjekt = m_pFeatures->getFeature( pRel->featureId );

    if ( pPlanObjekt == NULL )
    {
      pError = new SemanticError ( pPlanBereich, Error::ERROR, "Referiertes Planobjekt existiert nicht" );
      m_pErrorReport->addSemanticError ( pError );	
      continue;
    }

    vPlanObjektInverseRelationen.clear();
    anzInvers = pPlanObjekt->getFeatureRelationen ( inverseRelationName, vPlanObjektInverseRelationen );

    if ( anzInvers == 0 )
    {
      errorString = "Relation \"" + inverseRelationName + "\" ist nicht belegt";
      pError = new SemanticError ( pPlanObjekt, Error::WARNING, errorString );
      m_pErrorReport->addSemanticError ( pError );	
    }
    else
    {
      anzGefunden = 0;
      for ( j = 0; j < anzInvers; j++ )
      {
        pRelInvers = vPlanObjektInverseRelationen[j];
        pPlanObjektInvers = m_pFeatures->getFeature( pRelInvers->featureId );
        if ( pPlanObjektInvers->getGmlId() == pPlanBereich->getGmlId() ) anzGefunden++;
      }

      if ( anzGefunden == 0 )
      {
        errorString = "Relation \"" + inverseRelationName + "\" zeigt nicht auf den richtigen Plan-Bereich";
        pError = new SemanticError ( pPlanObjekt, Error::ERROR, errorString );
        m_pErrorReport->addSemanticError ( pError );	
      }
      else
        if ( anzGefunden > 1 )
        {
          errorString = "Relation \"" + inverseRelationName + "\" zeigt mehrfach auf denselben Plan-Bereich";
          pError = new SemanticError ( pPlanObjekt, Error::ERROR, errorString );
          m_pErrorReport->addSemanticError ( pError );	
        }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüfung, ob für alle Geometrien ein bekanntes Koordinaten-Referenz-  //
//  sysem spezifiziert ist                                                   //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckCoordReferenceSystems()
{
  vector<Feature*>    vFeatures;
  vector<_Geometrie*> vGeometrie;
  Feature           * pFeature = nullptr;
	size_t              i, j, anz;
  bool                errorFound = false;

  if ( m_pFeatures->existReferenceSystem() )
    return;

  anz = m_pFeatures->getAllFeatures( vFeatures );
  for ( i = 0; i < anz; i++ )
  {
    vGeometrie.clear();

    pFeature = vFeatures[i];
    pFeature->getGeometry( vGeometrie );

    for ( j = 0; j < vGeometrie.size(); j++ )
    {
      if ( vGeometrie[j]->existReferenceSystem( m_pSrsManager ) == false )
      {
        errorFound = true;
        break;
      }
    }
    if ( errorFound )
      break;
  }

  if ( errorFound )
  {
    SemanticError * pError;
    string          errorString;

    errorString = "Geometrie ohne gültiges Koordinaten-Referenzsystem";
    pError = new SemanticError ( pFeature, Error::WARNING, errorString );
    m_pErrorReport->addSemanticError ( pError );	
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Korrektheit von Flächengeometrie                           //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckGeometry( System::Windows::Forms::ProgressBar ^ testProgress )
{
  vector<Feature*>     vFeatures;
  vector<Feature*>     vFeaturesZuTesten;
  vector<_Geometrie*>  vGeometries;
  vector<string>       vGeometryPropertyNames;
  Feature		         * pFeature;
  _Geometrie         * pGeo;
	size_t               i, j, featureAnz, geoAnz, geoAnzGes;
  int                  progressBarMaximum = 0;
  int                  progressBarValue = 0;
  bool                 success;
  vector<Error*>       vErrors;
  string               propertyName;

  if (geometryCheckLevel == NO_GEOMETRY_CHECK )
    return;

  featureAnz = m_pFeatures->getAllFeatures( vFeatures );
  geoAnzGes  = 0;
  for ( i = 0; i < featureAnz; i++ )
  {
    pFeature = vFeatures[i];
    geoAnz = pFeature->getGeometryPropertyAnz();
    geoAnzGes = geoAnzGes + geoAnz;
    if ( geoAnz > 0 )   
      vFeaturesZuTesten.push_back( pFeature );
  }

  if ( testProgress != nullptr )
  {
    testProgress->Minimum = 0;
    testProgress->Maximum = (int)geoAnzGes;
    testProgress->Value   = 0;
  }

  for ( i = 0; i < vFeaturesZuTesten.size(); i++ )
  {
    pFeature = vFeaturesZuTesten[i];
    vGeometries.clear();
    vGeometryPropertyNames.clear();
    geoAnz = pFeature->getGeometryProperties( vGeometryPropertyNames, vGeometries );
    for ( j = 0; j < geoAnz; j++ )
    {
      pGeo         = vGeometries[j];
      propertyName = vGeometryPropertyNames[j];

      success = pGeo->checkReferences( pFeature, propertyName, vErrors );
      
      if ( !success )
        continue;

      if ( geometryCheckLevel == BASIC_CHECKS || geometryCheckLevel == ALL_CHECKS )
        pGeo->basicCheck( pFeature, propertyName, m_pCheckParams, vErrors );

      if ( geometryCheckLevel == ADVANCED_CHECKS || geometryCheckLevel == ALL_CHECKS )
        pGeo->advancedCheck( pFeature, propertyName, m_pCheckParams, vErrors, true );

      if ( geometryCheckLevel == TOPOLOGY_CHECKS || geometryCheckLevel == ALL_CHECKS )
      {
        if ( pGeo->getGeometryType() == SOLID )
        {
          Solid * pSolid = (Solid*)pGeo;
          pSolid->topologyCheck( pFeature, propertyName, m_pCheckParams, vErrors, true );
        }        
      }

      if ( testProgress != nullptr ) testProgress->Value++;     
    }
  }

  if ( geometryCheckLevel == SPECIFIC_CHECKS || geometryCheckLevel == ALL_CHECKS )
  {
    if ( m_pFeatures->getGmlSchema()->isCityGml() )
      CheckCityGMLGeometry ( testProgress );

    if ( m_pFeatures->getGmlSchema()->isXPlanGml() )
      checkXPLanGMLGeometry( testProgress );
  }

  if ( vErrors.size() > 0 )
    for ( i = 0; i < vErrors.size(); i++ )
      m_pErrorReport->addError( vErrors[i] );
}

///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML                                                             //
//  Überprüft spezielle Geometriebedingungen von XPlanGML:                   //
//  Für Flächenschlussobjekte:                                               //
//    - Überlappungsfreiheit der Member einer Multi-Surface                  //
//    - Überlappungsfreiheit der einzelnen Flächen                           //
//    - Berechnung der Summe der Einzelflächen und Vergleich mit der         //
//      Fläche des Geltungsbereichs                                          //
///////////////////////////////////////////////////////////////////////////////
void Checking::checkXPLanGMLGeometry( System::Windows::Forms::ProgressBar ^ testProgress )
{
	size_t               iPlan, planAnz, i, planBereichAnz, j, planObjektAnz, k;
  vector<BLeitPlan*>   vPlaene;
  vector<PlanBereich*> vPlanBereiche;
  vector<Feature*>     vPlanObjekte;
  vector<Feature*>     vFlaechenschlussObjekte;
  BLeitPlan          * pPlan;
  PlanBereich        * pPlanBereich;
  Feature            * pPlanObjekt;
  Feature            * pPlanObjektA;
  SemanticError      * pError;
  GeometryError      * pGeometryError;
  bool                 success, flaechenschluss, ebeneExist;
  _Surface           * pSurface;
  _Surface           * pSurfaceA;
  vector<_Geometrie*>  vGeo;
  int                  ebene;
  double               ueberlappungsFlaeche;
  vector<Error*>       vErrors;
  string               featureProperty;

  flaecheGeltungsbereich = 0.0;

  planAnz = m_pFeatures->getPlaene ( vPlaene );

  for ( iPlan = 0; iPlan < planAnz; iPlan++ )
  {
    pPlan           = vPlaene[iPlan];
    featureProperty = "xplan:raeumlicherGeltungsbereich";

    if ( checkVollstaendigeUeberdeckung )
    {
      vGeo.clear();
      pPlan->getGeometrie( featureProperty, vGeo );
      if ( vGeo.size() > 0 )
      {
        pSurface = dynamic_cast<_Surface*>( vGeo[0] );
        flaecheGeltungsbereich = pSurface->getFlaeche();
      }
      flaecheEinzelobjekte   = 0.0;
      flaecheUeberlappung    = 0.0;
    }

    vPlanBereiche.clear();
    planBereichAnz = pPlan->getPlanBereiche ( vPlanBereiche );
    for ( i = 0; i < planBereichAnz; i++ )
    {
      pPlanBereich = vPlanBereiche[i];
      vPlanObjekte.clear();
      vFlaechenschlussObjekte.clear();
      planObjektAnz = pPlanBereich->getBPlanObjekte ( vPlanObjekte, true );

      for ( j = 0; j < planObjektAnz; j++ )
      {
        pPlanObjekt = vPlanObjekte[j];
        if ( pPlanObjekt->getGeometrieTyp() == MULTI_SURFACE     ||
             pPlanObjekt->getGeometrieTyp() == SURFACE           ||
             pPlanObjekt->getGeometrieTyp() == COMPOSITE_SURFACE ||
             pPlanObjekt->getGeometrieTyp() == SHELL             ||
             pPlanObjekt->getGeometrieTyp() == POLYGON  )
        {         
          if ( pPlanObjekt->getObjektTyp() != BLEIT_PLAN && 
               pPlanObjekt->getObjektTyp() != PLAN_BEREICH && 
               pPlanObjekt->getObjektTyp() != RASTER_BASIS &&
               pPlanObjekt->getObjektTyp() != RASTER_AENDERUNG )
          {              
            success = pPlanObjekt->getBoolAttributWert ( "xplan:flaechenschluss", flaechenschluss );
            if ( !success )
            {
              pError = new SemanticError ( pPlanObjekt, Error::ERROR, "Flächenobjekt ohne Angabe des Attributs flaechenschluss" );
              m_pErrorReport->addSemanticError ( pError );	
            }
            else
            {
              ebeneExist = pPlanObjekt->getIntegerAttributWert ( "xplan:ebene", ebene );
              if ( ebeneExist && ebene != 0 ) continue;

              if ( (!ebeneExist || ebene == 0 ) &&   flaechenschluss == true )
                vFlaechenschlussObjekte.push_back ( pPlanObjekt );
            }
          }
        }
      }

      planObjektAnz = vFlaechenschlussObjekte.size();

      if ( testProgress != nullptr )
      {
        testProgress->Minimum = 0;
        testProgress->Maximum = int ( 0.5 * planObjektAnz * (planObjektAnz + 1) );
        testProgress->Value   = 0;
      }

      for ( j = 0; j < planObjektAnz; j++ )
      {
        pPlanObjekt     = vFlaechenschlussObjekte[j];
        featureProperty = "xplan:position";

        //  Geometrie rausholen und auf Selbstüberschneidung prüfen
        vGeo.clear();
        pPlanObjekt->getGeometrie( featureProperty, vGeo );
        if ( vGeo.size() == 0 )
          continue;
        
        pSurface = dynamic_cast<_Surface*>( vGeo[0] );

        if ( checkVollstaendigeUeberdeckung )
        {
          double flaeche =  pSurface->getFlaeche();
          flaecheEinzelobjekte = flaecheEinzelobjekte + flaeche;

          string stdString;
          String ^ STR = String::Concat ( Convert::ToString( j ),  ": ",  Convert::ToString( flaecheEinzelobjekte ), "; ", Convert::ToString( flaeche ),
                                            "; Id: ",  QuConvert::ToString ( pPlanObjekt->getGmlId() ) );
          QuConvert::systemStr2stdStr( stdString, STR );

          m_pLogfile->writeLogFile( stdString );
        }

        for ( k = j + 1; k < planObjektAnz; k++ )
        {
          pPlanObjektA = vFlaechenschlussObjekte[k];

          //  Geometrie rausholen
          vGeo.clear();
          pPlanObjektA->getGeometrie( featureProperty, vGeo );
          if ( vGeo.size() > 0 )
          {
            pSurfaceA = dynamic_cast<_Surface*>( vGeo[0] );
            if ( checkFlaechenschlussGrob  && !checkFlaechenschlussExakt )
            {
              success = pSurface->checkUeberlappung2D( pSurfaceA, m_pCheckParams->minimalPointDistance, m_pCheckParams->minimalAngleDistance );
              if ( !success )
              {
                pGeometryError = new GeometryError( pPlanObjekt, Error::ERROR, GeometryError::XPLANGML_FLAECHENSCHLUSS_OVERLAPP );
                pGeometryError->setPropertyName( featureProperty );
                pGeometryError->setRelatedPropertyName( featureProperty );
                pGeometryError->setFeatureGeometryId( pSurface->getGmlId() );
                pGeometryError->setRelatedFeature( pPlanObjektA );
                pGeometryError->setRelatedFeatureGeometryId( pSurfaceA->getGmlId() );
                m_pErrorReport->addGeometryError( pGeometryError );
              }
              if ( testProgress != nullptr ) testProgress->Value++;
            }
            else
            if ( checkFlaechenschlussExakt )
            {
              success = pSurface->checkUeberlappung2D( pSurfaceA, m_pCheckParams->minimalPointDistance, m_pCheckParams->minimalAngleDistance, ueberlappungsFlaeche );

              if ( testProgress != nullptr ) testProgress->Value++;

              if ( !success )
              {
                if ( ueberlappungsFlaeche > 0 )
                {
                  flaecheUeberlappung = flaecheUeberlappung + ueberlappungsFlaeche;

                  pGeometryError = new GeometryError( pPlanObjekt, Error::ERROR, GeometryError::XPLANGML_FLAECHENSCHLUSS_OVERLAPP );
                  pGeometryError->setPropertyName( featureProperty );
                  pGeometryError->setRelatedPropertyName( featureProperty );
                  pGeometryError->setFeatureGeometryId( pSurface->getGmlId() );
                  pGeometryError->setRelatedFeature( pPlanObjektA );
                  pGeometryError->setRelatedFeatureGeometryId( pSurfaceA->getGmlId() );
                  pGeometryError->setFlaechenangaben( pSurface->getFlaeche(), pSurfaceA->getFlaeche(), ueberlappungsFlaeche );
                  m_pErrorReport->addGeometryError( pGeometryError );
                }
              }
             }
           }
         }
      }
    }

    if ( checkVollstaendigeUeberdeckung )
    {
      double differenzflaeche = flaecheEinzelobjekte - flaecheUeberlappung - flaecheGeltungsbereich;
      if ( fabs ( differenzflaeche ) > flaecheGeltungsbereich*1.0E-6 )
      {
        pGeometryError = new GeometryError( NULL, Error::WARNING, GeometryError::SURFACE_TOTAL_COVERAGE_ERROR );
        pGeometryError->setPropertyName( featureProperty );
        if ( flaecheEinzelobjekte - flaecheUeberlappung < flaecheGeltungsbereich )
          pGeometryError->setErrorDescription( "Keine vollständige Überdeckung durch Flächenschlussobjekte" );
        else
          pGeometryError->setErrorDescription( "Gesamtfläche der Flächenschlussobjekte ist zu groß" );

        pGeometryError->setFlaechenangaben( flaecheEinzelobjekte - flaecheUeberlappung, flaecheGeltungsbereich, flaecheUeberlappung );
        m_pErrorReport->addGeometryError( pGeometryError );
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  Überprüft spezielle Geometriebedingungen von CityGML                     //
//  - Topologie-Überprüfung einzelner BoundarySurfaces                       //
//    - Keine Überlappung von MultiSurface Members                           //
//    - Überprüfung der Kanten-Topologie, Bestimmung Außenkontur             //
//  - Überprüfung des gesamten Verbundes von BoundearySutfaces               //
//    - Finden Überlappungen zwischen BoundarySurfaces statt                 //
//    - Ergibt die Vereinigung aller BoundardSurfaces einen Solid            //
//    - Gibt es mindestens eine RoofSurface, WallSurface und GroundSurface   //
//  - Überprüfung der BuildingParts eines Buildings                          //
//    - Überlappung oder Durchdringung von BoundarySurfaces                  //
//      unterschiedlicher Parts                                              //
//  - Überprüfung unterschiedlicher Buildings                                //
//     Überlappung oder Durchdringung von BoundarySurfaces                   //
///////////////////////////////////////////////////////////////////////////////
void Checking::CheckCityGMLGeometry(  System::Windows::Forms::ProgressBar ^ testProgress )
{
  if ( checkBoundarySurfaces )
    checkCityGMLBoundarySurfaces ( testProgress );

  if ( checkOuterShell )
    checkCityGMLOuterShell ( testProgress );
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  - Topologie-Überprüfung einzelner BoundarySurfaces                       //
//    - Keine Überlappung von MultiSurface Members                           //
//    - Überprüfung der Kanten-Topologie, Bestimmung Außenkontur             //
///////////////////////////////////////////////////////////////////////////////
void Checking::checkCityGMLBoundarySurfaces ( System::Windows::Forms::ProgressBar ^testProgress )
{
  vector<Feature*>       vFeatures;
  vector<Feature*>       vBoundarySurfaces;
  vector<Feature*>       vOpenings;
  vector<_Geometrie*>    vGeometries;
  vector<_Geometrie*>    vOpeningGeometries;
  vector<string>         vGeometryPropertyNames;
  vector<Error*>         vErrors;
  string                 propertyName;
  MultiSurface         * pTestSurface = new MultiSurface( 3 );
	size_t                 i, j, k, l, anz, boundarySurfaceAnz, geoAnz;

  m_pFeatures->getFeatures( vFeatures, "bldg:Building" );
  anz = m_pFeatures->getFeatures( vFeatures, "bldg:BuildingPart" );

  if ( testProgress != nullptr )
  {
    testProgress->Minimum = 0;
    testProgress->Maximum = (int)anz;
    testProgress->Value   = 0;
  }

  for ( i = 0; i < anz; i++ )
  {
    Feature * pFeature = vFeatures[i];
    vBoundarySurfaces.clear();

    boundarySurfaceAnz = pFeature->getRelatedFeatures( "bldg:boundedBy", vBoundarySurfaces );
    for ( j = 0; j < boundarySurfaceAnz; j++ )
    {
      Feature * pBoundarySurface = vBoundarySurfaces[j];
      vGeometries.clear();
      vGeometryPropertyNames.clear();

      geoAnz = pBoundarySurface->getGeometryProperties( vGeometryPropertyNames, vGeometries );
      for ( k = 0; k < geoAnz; k++ )
      {
        vErrors.clear();
        _Surface * pSurface = (_Surface*)(vGeometries[k]);
        pTestSurface->addSurface( pSurface );
     
        propertyName        = vGeometryPropertyNames[k];
        if ( propertyName == "bldg:lod3MultiSurface" || 
             propertyName == "bldg:lod4MultiSurface" )
        {
          vOpenings.clear();
          pBoundarySurface->getRelatedFeatures( "bldg:opening", vOpenings );
          for ( unsigned int o = 0; o < vOpenings.size(); ++o )
          {
            Feature * pOpening = vOpenings[o];
            vOpeningGeometries.clear();
            _Surface * pOpeningSurface = dynamic_cast<_Surface*>(pOpening->getGeometry( propertyName, 0 ));
            pTestSurface->addSurface( pOpeningSurface );
          }
        }

        pTestSurface->topologyCheck ( pBoundarySurface, propertyName, m_pCheckParams, vErrors, true );
        for ( l = 0; l < vErrors.size(); l++ )
          m_pErrorReport->addError( vErrors[l] );
        pTestSurface->clearSurfaces();
      }
    }     
    if ( testProgress != nullptr ) testProgress->Value++;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML                                                              //
//  - Überprüfung des gesamten Verbundes von BoundarySurfaces                //
//    - Finden Überlappungen zwischen BoundarySurfaces statt                 //
//    - Ergibt die Vereinigung aller BoundardSurfaces einen Solid            //
//    - Gibt es mindestens eine RoofSurface, WallSurface und GroundSurface   //
///////////////////////////////////////////////////////////////////////////////
void Checking::checkCityGMLOuterShell ( System::Windows::Forms::ProgressBar ^testProgress )
{
  vector<Feature*>   vFeatures;
  vector<Feature*>   vBoundarySurfaces;
  vector<Feature*>   vOpenings;
  vector<_Surface*>  vSurfaces;
  vector<string>     vProperties;
  vector<Error*>     vErrors;
	size_t             i, j, k, l, anz, anzOpenings, boundarySurfaceAnz;
  GeometryError    * pError;

  vProperties.push_back ( "bldg:lod2MultiSurface" );
  vProperties.push_back ( "bldg:lod3MultiSurface" );
  vProperties.push_back ( "bldg:lod4MultiSurface" );

  m_pFeatures->getFeatures( vFeatures, "bldg:Building" );
  anz = m_pFeatures->getFeatures( vFeatures, "bldg:BuildingPart" );

  if ( testProgress != nullptr )
  {
    testProgress->Minimum = 0;
    testProgress->Maximum = (int)anz;
    testProgress->Value   = 0;
  }

  for ( i = 0; i < anz; i++ )
  {
    Feature * pFeature = vFeatures[i];
    vBoundarySurfaces.clear();

    bool roofSurfaceExist   = false;
    bool wallSurfaceExist   = false;
    bool groundSurfaceExist = false;

    boundarySurfaceAnz = pFeature->getRelatedFeatures( "bldg:boundedBy", vBoundarySurfaces );
    if ( boundarySurfaceAnz > 0 )
    {
      for ( j = 0; j < vProperties.size(); j++ )
      {
        string geometryProperty = vProperties[j];
        vSurfaces.clear();

        for ( k = 0; k < boundarySurfaceAnz; k++ )
        {
          Feature * pBoundarySurface = vBoundarySurfaces[k];
          _Surface * pSurface = dynamic_cast<_Surface*>(pBoundarySurface->getGeometry( geometryProperty, 0 ));
          if ( pSurface != NULL )
          {
            vSurfaces.push_back( pSurface );
            if ( pBoundarySurface->getKlassenName() == "bldg:RoofSurface" )
              roofSurfaceExist = true;
            else
            if ( pBoundarySurface->getKlassenName() == "bldg:WallSurface" )
              wallSurfaceExist = true;
            else
            if ( pBoundarySurface->getKlassenName() == "bldg:GroundSurface" )
              groundSurfaceExist = true;
          }

          vOpenings.clear();
          anzOpenings = pBoundarySurface->getRelatedFeatures( "bldg:opening", vOpenings );
          for ( l = 0; l < anzOpenings; l++ )
          {
            Feature * pOpening = vOpenings[l];
            pSurface = dynamic_cast<_Surface*>(pOpening->getGeometry( geometryProperty, 0 ));
            if ( pSurface != NULL )
              vSurfaces.push_back( pSurface );
          }
        }

        if ( vSurfaces .size() > 0 )
        {
          if ( !roofSurfaceExist )
          {
            pError = new GeometryError( pFeature, Error::WARNING, GeometryError::CITYGML_MISSING_ROOF_SURFACE );
            pError->setPropertyName( geometryProperty );
            m_pErrorReport->addError ( pError );
          }
         
          if ( !wallSurfaceExist )
          {
            pError = new GeometryError( pFeature, Error::WARNING, GeometryError::CITYGML_MISSING_WALL_SURFACE );
            pError->setPropertyName( geometryProperty );
            m_pErrorReport->addError ( pError );
          }
          
          if ( !groundSurfaceExist )
          {
            pError = new GeometryError( pFeature, Error::WARNING, GeometryError::CITYGML_MISSING_GROUND_SURFACE );
            pError->setPropertyName( geometryProperty );
            m_pErrorReport->addError ( pError );
          }

          CompositeSurface * pCompositeSurface = new CompositeSurface( 3 );
          for ( k = 0; k < vSurfaces.size(); k++ )
          {
            _Surface * pSurfaceCopy = dynamic_cast<_Surface*>(_Geometrie::copy ( vSurfaces[k] ));
            if ( pSurfaceCopy != NULL )
              pCompositeSurface->addSurface( pSurfaceCopy );          
          }

          Solid * pSolid = new Solid;
          pSolid->setAussenkontur( pCompositeSurface );

          vErrors.clear();
          pSolid->topologyCheck( pFeature, geometryProperty, m_pCheckParams, vErrors, true );
          if ( vErrors.size() > 0 )
          {
            bool missingCounterpart = false;
            bool doubleEdge         = false;
            bool overlap            = false;

            MultiGeometry * pErrorGeometry = new MultiGeometry ( 3 );
            for ( k = 0; k < vErrors.size(); k++ )
            {

              pError = (GeometryError*)vErrors[k];
              switch ( pError->getGeometryErrorType() )
              {
              case GeometryError::SOLID_EDGE_CHECK_FAILED_MISSING_COUNTERPART:
                missingCounterpart = true;
                for ( l = 0; l < pError->getErrorGeometryAnz(); l++ )
                {
                  if ( pError->getErrorGeometry( l )->getGeometryType() == MULTI_CURVE )
                    pErrorGeometry->addGeometry( _Geometrie::copy (pError->getErrorGeometry( l ) ) );
                }
                break;

              case GeometryError::SOLID_EDGE_CHECK_FAILED_DOUBLE_EDGE:
              case GeometryError::SOLID_EDGE_CHECK_FAILED_MULTIPLE_EDGE:
                doubleEdge = true;
                for ( l = 0; l < pError->getErrorGeometryAnz(); l++ )
                {
                  if ( pError->getErrorGeometry( l )->getGeometryType() == MULTI_CURVE )
                    pErrorGeometry->addGeometry( _Geometrie::copy (pError->getErrorGeometry( l ) ) );
                }
                break;

              case GeometryError::MULTISURFACE_IRREGULAR_POINT_TOUCH:
              case GeometryError::MULTISURFACE_IRREGULAR_LINE_TOUCH_INTERSECT:
              case GeometryError::MULTISURFACE_IRREGULAR_SURFACE_OVERLAP:
                overlap = true;
                for ( l = 0; l < pError->getErrorGeometryAnz(); l++ )
                    pErrorGeometry->addGeometry( _Geometrie::copy (pError->getErrorGeometry( l ) ) );
                break;
              }
              delete vErrors[k];
            }

            pError = new GeometryError( pFeature, Error::WARNING, GeometryError::CITYGML_BOUNDARY_SURFACES_NO_SOLID );
            pError->setPropertyName( geometryProperty );
            pError->addErrorGeometry( pErrorGeometry, "" );

            string errorDescription = "";
            if ( missingCounterpart )
              errorDescription = "Offene Kanten; ";
            if ( doubleEdge )
              errorDescription = errorDescription + "Mehrfach durchlaufene Kanten; ";
            if ( overlap )
              errorDescription = errorDescription + "Fehlerhafte Überlappung/Durchdringung von BoundarySurfaces";
            pError->setErrorDescription( errorDescription );
            m_pErrorReport->addError ( pError );
          }
       }
     }
   }
   if ( testProgress != nullptr ) testProgress->Value++;
 }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Namen der vom Programm unterstützten Profile                 //
///////////////////////////////////////////////////////////////////////////////
int Checking::getProfiles ( std::vector<std::string> &vProfiles )
{
  cli::array<String^> ^ FILES;
  IEnumerator         ^ FILES_ENUM;
  string                stdString;
  String              ^ SCHEMA_FOLDER = QuConvert::ToString ( m_pFeatures->getGmlSchema()->getSchemaFolder() );
  String              ^ PROFILE_FOLDER = String::Concat( SCHEMA_FOLDER, "Profile\\" );

  if ( !Directory::Exists( PROFILE_FOLDER ) )
    return  0;

  FILES      = Directory::GetDirectories ( PROFILE_FOLDER );
  FILES_ENUM = FILES->GetEnumerator();

  while ( FILES_ENUM->MoveNext() )
  {
    String ^ FOLDER_NAME = safe_cast<String^>(FILES_ENUM->Current);
    String ^ FILE_NAME   = Path::GetFileName( FOLDER_NAME );
    QuConvert::systemStr2stdStr( stdString, FILE_NAME );
    vProfiles.push_back( stdString );
  }

  return FILES->Length;
}

///////////////////////////////////////////////////////////////////////////////
//  CheckingParams                                                           //
//  Parameter zur Konfigurierung der geometrischen Prüfung                   //
///////////////////////////////////////////////////////////////////////////////
CheckingParams::CheckingParams()
{
  minimalPointDistance    = MINIMAL_POINT_DISTANCE;
  minimalAngleDistance    = MINIMAL_ANGLE_DISTANCE;
  planarityLimit          = AREA_TOLERANCE_RELATIVE;
  eliminateDoublePoints   = false;
  checkReferencedGeometry = false;

  checkBoundarySurfaces = true;
  checkSolid            = false;
  checkUeberlappung     = false;
}

CheckingParams::~CheckingParams()
{

}

///////////////////////////////////////////////////////////////////////////////
//  SchemaValidierung                                                        //
//  Hilfsklasse zur Schema-Validierung                                       //
///////////////////////////////////////////////////////////////////////////////
SchemaValidierung::SchemaValidierung (GmlSchema * pSchemaP,  GmlNamespaces  * pNamespacesStandardP, ErrorReport * pErrorReportP, std::string dateiName )
{
  m_pErrorReport = pErrorReportP;
  m_pSchema      = pSchemaP;
  m_pNamespaces  = pNamespacesStandardP;
  m_pDateiName   = QuConvert::ToString( dateiName );

}

SchemaValidierung::~SchemaValidierung()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Validiert gegen Profil profileName                                       //
///////////////////////////////////////////////////////////////////////////////

void SchemaValidierung::ValidateProfile( std::string profileName )
{
  string profileFolderName = m_pSchema->getSchemaFolder() + "Profile\\" + profileName + "\\";
  string schemaDatei;
  string targetNamespace;

  XmlSchemaSet ^ schema = gcnew XmlSchemaSet; 
  XmlReader    ^ validatingReader;
   
  String ^ SCHEMA_FOLDER = QuConvert::ToString ( profileFolderName );
  bool exist = Directory::Exists ( SCHEMA_FOLDER );
  if ( exist )
  {
    cli::array<String^> ^ FILES = Directory::GetFiles ( SCHEMA_FOLDER );
    IEnumerator    ^ FILES_ENUM = FILES->GetEnumerator();

    while ( FILES_ENUM->MoveNext() )
    {
      String ^ SCHEMA_FILE_NAME = safe_cast<String^>(FILES_ENUM->Current);
      String ^ TARGET_NAMESPACE = getTargetNamespace ( SCHEMA_FILE_NAME );

      schema->Add (  TARGET_NAMESPACE, SCHEMA_FILE_NAME );
    }
  }

  XmlReaderSettings^ settings = gcnew XmlReaderSettings;
  settings->ValidationType = ValidationType::Schema;
  settings->Schemas = schema;
  settings->ValidationEventHandler += gcnew ValidationEventHandler( this, &SchemaValidierung::ValidationCallback );

  try
  {
    validatingReader =  XmlReader::Create( m_pDateiName, settings );
  }

  catch ( System::Exception ^ )
  {
    ValidationError * pValidationError = new ValidationError ( Error::ERROR, "Schema-Validierung ist nicht möglich, evtl. besteht keine Internet-Verbindung", -1 );
    m_pErrorReport->addValidationError ( pValidationError );
    return;
  }

  while ( validatingReader->Read() )
  {
  }

  //Close the reader.
  validatingReader->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Liest den Header einer Schema-Datei und extrahiert den Target-Namespace  //
///////////////////////////////////////////////////////////////////////////////
  String ^ SchemaValidierung::getTargetNamespace ( String ^ schemaDatei )
  {
    if ( !File::Exists ( schemaDatei ) )
      return "";

    XmlTextReader ^ reader = gcnew System::Xml::XmlTextReader( schemaDatei );

    while( reader->Read() )
    {
      if ( reader->NodeType == XmlNodeType::Element )
      {
        if ( reader->LocalName->CompareTo("schema") != 0 )
          return "";

        String ^ STR = reader->GetAttribute( "targetNamespace" );
        if ( STR != nullptr )
          return STR;
        else
          return "";
      }
    }
    return "";
  }

///////////////////////////////////////////////////////////////////////////////
//  Validiert gegen die Standard-Schemadateien des zug. GML-APplikations-    //
//  schemas                                                                  //
///////////////////////////////////////////////////////////////////////////////
void SchemaValidierung::Validate()
{
  XmlSchemaSet   ^ schema; 
  XmlReader      ^ validatingReader;
	size_t           i, anz;
  vector<string>   vNamespaces;
  vector<string>   vSchemaDateien;

  schema = gcnew XmlSchemaSet; 

  anz = m_pNamespaces->getSchemaDateien( vNamespaces, vSchemaDateien );
  for ( i = 0; i < anz; i++ )
  {
    schema->Add (  QuConvert::ToString ( vNamespaces[i] ), QuConvert::ToString ( vSchemaDateien[i] ) );
  }

  XmlReaderSettings^ settings = gcnew XmlReaderSettings;
  settings->ValidationType = ValidationType::Schema;
  settings->Schemas = schema;
  settings->ValidationEventHandler += gcnew ValidationEventHandler( this, &SchemaValidierung::ValidationCallback );

  try
  {
    validatingReader =  XmlReader::Create( m_pDateiName, settings );
  }
 
  catch ( System::Exception ^ )
  {
    ValidationError * pValidationError = new ValidationError ( Error::ERROR, "Schema-Validierung ist nicht möglich, evtl. besteht keine Internet-Verbindung", -1 );
    m_pErrorReport->addValidationError ( pValidationError );
    return;
  }

  while ( validatingReader->Read() )
  {
  }

  //Close the reader.
  validatingReader->Close();
}


///////////////////////////////////////////////////////////////////////////////
//  Callback für Schema-Validierung                                          //
///////////////////////////////////////////////////////////////////////////////
void SchemaValidierung::ValidationCallback(System::Object ^ sender, ValidationEventArgs ^ args )
{
   std::string errText;
   int         lineNumber;

   QuConvert::systemStr2stdStr( errText, args->Message );
   lineNumber = args->Exception->LineNumber;

   ValidationError * pValidationError = new ValidationError ( Error::ERROR,  errText, lineNumber );
   m_pErrorReport->addValidationError ( pValidationError );
}

///////////////////////////////////////////////////////////////////////////////
//  SemanticRules                                                            //
//  Sammlung aller Regeln, mit denen ein geladenes Modell überprüft          //
//  werden kann.												                              			 //
///////////////////////////////////////////////////////////////////////////////
SemanticRules::SemanticRules (  Features * pFeatures, CheckingParams * pCheckParams, ErrorReport * pErrorReportP )
{
	m_pFeatures    = pFeatures;
  m_pCheckParams = pCheckParams;
	m_pErrorReport = pErrorReportP;
}

SemanticRules::~SemanticRules()
{
	multimap<string,ProfileRule*>::iterator iter;

	for ( iter = profileRulesClasses.begin(); iter != profileRulesClasses.end(); iter++ )
		delete iter->second;

	for ( iter = profileRulesSuperclasses.begin(); iter != profileRulesSuperclasses.end(); iter++ )
		delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest die Profilregeln des GML-Applikationsschemas ein                   //
///////////////////////////////////////////////////////////////////////////////
void SemanticRules::initialize()
{
  GmlSchema           * pGmlObjekte = m_pFeatures->getGmlSchema();
  string                conformityPath;
  string                fileName;
  string                systemFolder = Constant::getSystemFolder();
  String              ^ CONFORMITY_PATH;
  IEnumerator         ^ FILES_ENUM;
  cli::array<String^> ^ FILES;
  ProfileReader       ^ pProfileReader;
  bool                  success;

  conformityPath = Constant::getConformityPath ( pGmlObjekte->getGmlTyp() ); 

  pProfileReader = gcnew ProfileReader ( this );

  CONFORMITY_PATH = QuConvert::ToString ( conformityPath );
  bool exist = Directory::Exists ( CONFORMITY_PATH );
  if ( exist )
  {
    FILES = Directory::GetFiles ( CONFORMITY_PATH );

    FILES_ENUM = FILES->GetEnumerator();
    while ( FILES_ENUM->MoveNext() )
    {
      String^ FILE_NAME = safe_cast<String^>(FILES_ENUM->Current);
      QuConvert::systemStr2stdStr ( fileName, FILE_NAME );
      success = pProfileReader->read ( fileName );
    }
  } 
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Regel zur Liste hinzu						                    					 //
///////////////////////////////////////////////////////////////////////////////
void SemanticRules::addProfileRule( ProfileRule * pRule )
{
	string gmlClassName = pRule->getGmlClassName();
	string gmlSuperclassTypeName = pRule->getGmlSuperclassTypeName();

	if ( gmlClassName == "" && gmlSuperclassTypeName == "" )
		return;
	if ( gmlSuperclassTypeName != "" )
		profileRulesSuperclasses.insert 
		( multimap<string,ProfileRule*>::value_type ( gmlSuperclassTypeName, pRule ) );
	else
		profileRulesClasses.insert ( multimap<string,ProfileRule*>::value_type ( gmlClassName, pRule ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Regeln, die eines best. Klasse zugeordnet sind           	 //
///////////////////////////////////////////////////////////////////////////////
size_t  SemanticRules::getRules ( std::string klassenName, std::vector<ProfileRule*> & vRules )
{
	multimap<string,ProfileRule*>::iterator iter;
	vector<string>						 	            parentTypeNames;
	string									                parentTypeName;
	ProfileRule						                * pRule;
	size_t                                  i, anz;

	for ( iter = profileRulesClasses.lower_bound ( klassenName );
		  iter !=  profileRulesClasses.upper_bound ( klassenName );
		  iter ++ )
	{
		pRule = iter->second;
		vRules.push_back ( pRule );
	}

	GmlKlasse * pGmlKlasse = m_pFeatures->getGmlSchema()->getGmlKlasseFromElementName ( klassenName );
	if ( pGmlKlasse != NULL )
	{
		anz = pGmlKlasse->getParentTypeNames( parentTypeNames );
		for ( i = 0; i < anz; i++ )
		{
			parentTypeName = parentTypeNames[i];
			for ( iter = profileRulesSuperclasses.lower_bound ( parentTypeName );
				  iter !=  profileRulesSuperclasses.upper_bound ( parentTypeName );
				iter ++ )
			{
				pRule = iter->second;
				vRules.push_back ( pRule );
			}
		}
	}	

	return vRules.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft alle Regeln, die für eine best. Feature relevant sind          //
///////////////////////////////////////////////////////////////////////////////
void SemanticRules::checkRules ( Feature * pFeature )
{
  vector<ProfileRule*> vProfileRules;
	size_t               iRule, ruleAnz;
  string               klassenName;

  klassenName = pFeature->getKlassenName();

  ruleAnz = getRules ( klassenName, vProfileRules );

  for ( iRule = 0; iRule < ruleAnz; iRule++ )
  {
    ProfileRule * pRule = vProfileRules[iRule];
    pRule->checkRule ( pFeature, this );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft Vorhandensein und Korrektheit von UOM-Angaben                  //
///////////////////////////////////////////////////////////////////////////////
void SemanticRules::checkUnitsOfMeasurement( Feature * pFeature )
{
  string               klassenName;
  GmlKlasse          * pGmlKlasse;
  GmlSchema          * pGmlSchema = m_pFeatures->getGmlSchema();
  vector<GmlAttribut*> vGmlAttributeAll;
	size_t               i, j, attributAnz, attributWertAnz;
  vector<DoubleAttr*>  vDoubleAttributWerte;
  SemanticError      * pError;
  map<string,string>::iterator iterClassifier;


  klassenName = pFeature->getKlassenName();
  pGmlKlasse  = pGmlSchema->getGmlKlasseFromElementName ( klassenName );
  if ( pGmlKlasse == NULL ) return;

  attributAnz = pGmlKlasse->getGmlAttribute ( vGmlAttributeAll );
  for ( i = 0; i < attributAnz; i++ )
  {
    GmlAttribut * pGmlAttribut = vGmlAttributeAll[i];
    ATTRIBUT_TYP typ = pGmlAttribut->getGmlAttributTyp();

    if ( typ == _LENGTH || typ == _AREA || typ == _VOLUME || typ == _ANGLE || typ == _MEASURE )
    {
      string attributName = pGmlAttribut->getGmlAttributName();
      vDoubleAttributWerte.clear();
      attributWertAnz = pFeature->getDoubleAttributWerte ( attributName, vDoubleAttributWerte  );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        string       uomValue;
        DoubleAttr * pDoubleAttr = vDoubleAttributWerte[j];

				if ( pDoubleAttr->uom != NULL )
					uomValue = pDoubleAttr->uom->getIdentifier();
				else
				{
					uomValue = "";
					for ( iterClassifier = pDoubleAttr->classifier.begin(); iterClassifier != pDoubleAttr->classifier.end(); iterClassifier++ )
					{
						if ( iterClassifier->first == "uom" )
						{
	            uomValue =  iterClassifier->second;
		        }
			    }
				}

        if ( uomValue == "" )
        {
          pError = new SemanticError ( pFeature, Error::ERROR, "Keine Unit of Measurement für einen Measure-Type spezifiziert" );
          pError->setAttribute( attributName, "" );
          m_pErrorReport->addSemanticError ( pError );
        }
        else
        { 
          switch ( typ )
          {
          case _LENGTH:
            if ( uomLength.find ( uomValue ) == uomLength.end() )
            {
              pError = new SemanticError ( pFeature, Error::ERROR, "Ungültige Unit of Measurement für einen Measure-Type spezifiziert" );
              pError->setAttribute( attributName, "" );
              m_pErrorReport->addSemanticError ( pError );
            }
            break;

          case _AREA:
            if ( uomArea.find ( uomValue ) == uomArea.end() )
            {
                pError = new SemanticError ( pFeature, Error::ERROR, "Ungültige Unit of Measurement für einen Measure-Type spezifiziert" );
                pError->setAttribute( attributName, "" );
                m_pErrorReport->addSemanticError ( pError );
            }
            break;

          case _VOLUME:
            if ( uomVolume.find ( uomValue ) == uomVolume.end() )
            {
              pError = new SemanticError ( pFeature, Error::ERROR, "Ungültige Unit of Measurement für einen Measure-Type spezifiziert" );
              pError->setAttribute( attributName, "" );
              m_pErrorReport->addSemanticError ( pError );
            }
            break;

          case _ANGLE:
            if (  uomAngle.find (uomValue ) == uomAngle.end() )
            {
              pError = new SemanticError ( pFeature, Error::ERROR, "Ungültige Unit of Measurement für einen Measure-Type spezifiziert" );
              pError->setAttribute( attributName, "" );
              m_pErrorReport->addSemanticError ( pError );
            }
            break;

          case _MEASURE:
            if (  uomAngle.find (uomValue ) == uomMeasure.end() )
            {
              pError = new SemanticError ( pFeature, Error::ERROR, "Ungültige Unit of Measurement für einen Measure-Type spezifiziert" );
              pError->setAttribute( attributName, "" );
              m_pErrorReport->addSemanticError ( pError );
            }
            break;

          }
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Fehler zur Fehlerliste hinzu				                					 //
///////////////////////////////////////////////////////////////////////////////
void SemanticRules::addError ( SemanticError * pError )
{
	m_pErrorReport->addSemanticError ( pError );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen srsNamen hinzu					                        							 //
///////////////////////////////////////////////////////////////////////////////
void SemanticRules::addSrsName( std::string srsName )
{
	srsNamen.insert ( srsName );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt uom-Kürzel für "Länge" hinzu                                        //
///////////////////////////////////////////////////////////////////////////////
void SemanticRules::addUomLength ( std::string name )
{
  uomLength.insert( name );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt uom-Kürzel für "Flächeninhalt" hinzu                                //
///////////////////////////////////////////////////////////////////////////////
void SemanticRules::addUomArea ( std::string name )
{
  uomArea.insert( name );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt uom-Kürzel für "Volumen" hinzu                                      //
///////////////////////////////////////////////////////////////////////////////
void SemanticRules::addUomVolume ( std::string name )
{
  uomVolume.insert( name );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt uom-Kürzel für "Winkel" hinzu                                       //
///////////////////////////////////////////////////////////////////////////////
void SemanticRules::addUomAngle ( std::string name )
{
  uomAngle.insert( name );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt uom-Kürzel für "Measure" hinzu                                      //
///////////////////////////////////////////////////////////////////////////////
void SemanticRules::addUomMeasure ( std::string name )
{
  uomMeasure.insert( name );
}


///////////////////////////////////////////////////////////////////////////////
//  ProfileRule                                                              //
//  Einzelne Konformitätsregel  	                                  				 //
///////////////////////////////////////////////////////////////////////////////
ProfileRule::ProfileRule()
{
	gmlClassName        = "";

	pIfFilter       = NULL;
	pRequiredFilter = NULL;
}

ProfileRule::~ProfileRule()
{
	if ( pIfFilter != NULL )       delete pIfFilter;
	if ( pRequiredFilter != NULL ) delete pRequiredFilter;
}

///////////////////////////////////////////////////////////////////////////////
//  Interpretiert den Textstring zur Bezeichnung der Kombination             //
///////////////////////////////////////////////////////////////////////////////
COMBINATION_TYPE ProfileRule::interpretCombination ( String ^ combination )
{
  COMBINATION_TYPE comb;

  if ( combination == nullptr )
    comb = AND;
  else
  {
    if ( combination->CompareTo( "AND" ) == 0 )
      comb = AND;
    else
    if ( combination->CompareTo( "OR" ) == 0 )
      comb = OR;
    else
    if ( combination->CompareTo( "XOR" ) == 0 )
      comb = XOR;
    else
    if ( combination->CompareTo( "XOR_OPT" ) == 0 )
      comb = XOR_OPT;
    else
      comb = AND;
  }

  return comb;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Attribut-Filters   				                        		 //
///////////////////////////////////////////////////////////////////////////////
void ProfileRule::addAttributeFilter ( AttributeFilter * pFilter,  FILTER_PART part, String ^ combination )
{
	if ( part == IF_FILTER )
	{
		if ( pIfFilter == NULL )
    {
      pIfFilter = new ProfileFilter ( this );
      pIfFilter->setCombinationType( interpretCombination ( combination) );
    }
		pIfFilter->addAttributeFilter ( pFilter );
	}
	else
	if ( part == REQUIRED_FILTER )
	{
		if ( pRequiredFilter == NULL )
    {
      pRequiredFilter = new ProfileFilter ( this );
      pRequiredFilter->setCombinationType( interpretCombination ( combination) );
    }

		pRequiredFilter->addAttributeFilter ( pFilter );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Attribut-Kardinalitäts-Filters                      		 //
///////////////////////////////////////////////////////////////////////////////
void ProfileRule::addAttributeCardinalityFilter ( AttributeCardinalityFilter * pFilter,  FILTER_PART part, String ^ combination )
{
  if ( part == IF_FILTER )
  {
    if ( pIfFilter == NULL )
    {
      pIfFilter = new ProfileFilter ( this );
      pIfFilter->setCombinationType( interpretCombination ( combination) );
    }
    pIfFilter->addAttributeCardinalityFilter ( pFilter );
  }
  else
  if ( part == REQUIRED_FILTER )
  {
    if ( pRequiredFilter == NULL )
    {
      pRequiredFilter = new ProfileFilter ( this );
      pRequiredFilter->setCombinationType( interpretCombination ( combination) );
    }

    pRequiredFilter->addAttributeCardinalityFilter ( pFilter );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Relations-Filters   				                        		 //
///////////////////////////////////////////////////////////////////////////////
void ProfileRule::addRelationFilter  ( RelationFilter * pFilter,  FILTER_PART part, String ^ combination )  
{
	if ( part == IF_FILTER )
	{
		if ( pIfFilter == NULL )
    {
      pIfFilter = new ProfileFilter ( this );
      pIfFilter->setCombinationType( interpretCombination ( combination) );
    }
		pIfFilter->addRelationFilter ( pFilter );
	}
	else
	if ( part == REQUIRED_FILTER )
	{
		if ( pRequiredFilter == NULL )
    {
      pRequiredFilter = new ProfileFilter ( this );
      pRequiredFilter->setCombinationType( interpretCombination ( combination) );
    }
		pRequiredFilter->addRelationFilter ( pFilter );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Gruppen-Filters   				                          		 //
///////////////////////////////////////////////////////////////////////////////
void ProfileRule::addGroupFilter  ( GroupFilter * pFilter,  FILTER_PART part, String ^ combination ) 
{
	if ( part == IF_FILTER )
	{
		if ( pIfFilter == NULL )
    {
      pIfFilter = new ProfileFilter ( this );
      pIfFilter->setCombinationType( interpretCombination ( combination) );
    }
    pIfFilter->addGroupFilter ( pFilter );
	}
	else
	if ( part == REQUIRED_FILTER )
	{
		if ( pRequiredFilter == NULL )
    {
      pRequiredFilter = new ProfileFilter ( this );
      pRequiredFilter->setCombinationType( interpretCombination ( combination) );
    }
		pRequiredFilter->addGroupFilter ( pFilter );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Geometrie-Filters   				                        		 //
///////////////////////////////////////////////////////////////////////////////
void ProfileRule::addGeometryFilter  ( GeometryFilter * pFilter,  FILTER_PART part, String ^ combination )
{
	if ( part == IF_FILTER )
	{
		if ( pIfFilter == NULL )
    {
      pIfFilter = new ProfileFilter ( this );
      pIfFilter->setCombinationType( interpretCombination ( combination) );
    }
		pIfFilter->addGeometryFilter ( pFilter );
	}
	else
	if ( part == REQUIRED_FILTER )
	{
		if ( pRequiredFilter == NULL )
    {
      pRequiredFilter = new ProfileFilter ( this );
      pRequiredFilter->setCombinationType( interpretCombination ( combination) );
    }
		pRequiredFilter->addGeometryFilter ( pFilter );
	}
}		

///////////////////////////////////////////////////////////////////////////////
//  Überprüft ein Objekt gegen die Regel									                   //
//  Wenn die im pIfFilter formulierten Bedingungen nicht erfüllt sind, wird  //
//  true als Returnwert geliefert. Wenn die Bedingung erfüllt ist, wird das  //
//  Ergebnis des pRequiredFilter geliefert									                 //
///////////////////////////////////////////////////////////////////////////////
bool ProfileRule::checkRule ( Feature * pObj, SemanticRules * pProfile )
{
	bool              weiter;

	if ( pRequiredFilter == NULL ) return false;

	if ( pIfFilter != NULL )
	{
		weiter = pIfFilter->checkRule ( pObj, pProfile, false );
		if ( weiter == false ) return true;
	}

	return pRequiredFilter->checkRule ( pObj, pProfile, true );
}

///////////////////////////////////////////////////////////////////////////////
//  ProfileFilter                                                            //
//  Kombination von Bedingungen an Attributwerte, Relationen und Geometrie-  //
//  Eigenschaften, die ein bestimmtes Objekt erfüllen muss                 	 //
///////////////////////////////////////////////////////////////////////////////
ProfileFilter::ProfileFilter( ProfileRule * pParentRuleP )
{
	pParentRule     = pParentRuleP;
  combinationType = AND;
}

ProfileFilter::~ProfileFilter()
{
	unsigned int i;

	for ( i = 0; i < attributeFilters.size(); i++ )
		delete attributeFilters[i];

  for ( i = 0; i < attributeCardinalityFilters.size(); i++ )
    delete attributeCardinalityFilters[i];

	for ( i = 0; i < relationFilters.size(); i++ )
		delete relationFilters[i];

  for ( i = 0; i < groupFilters.size(); i++ )
    delete groupFilters[i];

  for ( i = 0; i < geometryFilters.size(); i++ )
    delete geometryFilters[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Attribut-Filter hinzu		  	                        				 //
///////////////////////////////////////////////////////////////////////////////
void ProfileFilter::addAttributeFilter ( AttributeFilter * pFilter )
{
	attributeFilters.push_back ( pFilter );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Attribut-Kardinalitäts-Filter hinzu                   				 //
///////////////////////////////////////////////////////////////////////////////
void ProfileFilter::addAttributeCardinalityFilter ( AttributeCardinalityFilter * pFilter )
{
  attributeCardinalityFilters.push_back( pFilter );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Relationen-Filter hinzu                              				 //
///////////////////////////////////////////////////////////////////////////////
void ProfileFilter::addRelationFilter ( RelationFilter * pFilter )
{
	relationFilters.push_back ( pFilter );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Gruppen-Filter hinzu                                 				 //
///////////////////////////////////////////////////////////////////////////////
void ProfileFilter::addGroupFilter ( GroupFilter * pFilter )
{
	groupFilters.push_back ( pFilter );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Geometrie-Filter hinzu                   		            		 //
///////////////////////////////////////////////////////////////////////////////
void ProfileFilter::addGeometryFilter ( GeometryFilter * pFilter )
{
	geometryFilters.push_back ( pFilter );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Attribut-Filter      	                            				 //
///////////////////////////////////////////////////////////////////////////////
size_t ProfileFilter::getAttributeFilters ( std::vector<AttributeFilter*> & attributeFiltersP )
{
	size_t i;

	for ( i = 0; i < attributeFilters.size(); i++ )
		attributeFiltersP.push_back ( attributeFilters[i] );

	return attributeFiltersP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Attribut-Kardinalitäts-Filter                       				 //
///////////////////////////////////////////////////////////////////////////////
size_t ProfileFilter::getAttributeCardinalityFilters ( std::vector<AttributeCardinalityFilter*> & attributeCardinalityFiltersP )
{
  unsigned int i;

  for ( i = 0; i < attributeCardinalityFilters.size(); i++ )
    attributeCardinalityFiltersP.push_back ( attributeCardinalityFilters[i] );

  return attributeCardinalityFiltersP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Relationen-Filter      	                          				 //
///////////////////////////////////////////////////////////////////////////////
size_t ProfileFilter::getRelationFilters ( std::vector<RelationFilter*> & relationFiltersP )
{
	unsigned int i;

	for ( i = 0; i < relationFilters.size(); i++ )
		relationFiltersP.push_back ( relationFilters[i] );

	return relationFiltersP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Gruppen-Filter        	                            				 //
///////////////////////////////////////////////////////////////////////////////
size_t ProfileFilter::getGroupFilters ( std::vector<GroupFilter*> & groupFiltersP )
{
	unsigned int i;

	for ( i = 0; i < groupFilters.size(); i++ )
		groupFiltersP.push_back ( groupFilters[i] );

	return groupFiltersP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Geometrie-Filter      	                            				 //
///////////////////////////////////////////////////////////////////////////////
size_t ProfileFilter::getGeometryFilters ( std::vector<GeometryFilter*> & geometryFiltersP )
{
	unsigned int i;

	for ( i = 0; i < geometryFilters.size(); i++ )
		geometryFiltersP.push_back ( geometryFilters[i] );

	return geometryFiltersP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Wendet den Filter für ein bestimmtes Objekt an		            					 //
///////////////////////////////////////////////////////////////////////////////
bool ProfileFilter::checkRule ( Feature * pObj, SemanticRules * pProfile, bool generateError )
{
	unsigned int           i;
	AttributeFilter            * pAttributeFilter;
  AttributeCardinalityFilter * pAttributeCardinalityFilter;
	RelationFilter             * pRelationFilter;
	GroupFilter                * pGroupFilter;
	GeometryFilter             * pGeometryFilter;
  SemanticError              * pSemanticError;
  vector<SemanticError*>       vSemanticErrors;
  int                          anzSuccess = 0;
	size_t                       anzFilter = attributeFilters.size() + attributeCardinalityFilters.size() + relationFilters.size() +  
                                            groupFilters.size() + geometryFilters.size();

	for ( i = 0; i < attributeFilters.size(); i++ )
	{
		pAttributeFilter = attributeFilters[i];
    pSemanticError = pAttributeFilter->checkRule ( pObj, pProfile );
    if ( pSemanticError == NULL )
      anzSuccess++;
    else
    {
      if ( generateError )
      {
        if ( combinationType == AND )
          pProfile->addError ( pSemanticError );
        else
          vSemanticErrors.push_back( pSemanticError );
      }
      else
        delete pSemanticError;
    }
	}

  for ( i = 0; i < attributeCardinalityFilters.size(); i++ )
  {
    pAttributeCardinalityFilter = attributeCardinalityFilters[i];
    pSemanticError = pAttributeCardinalityFilter->checkRule ( pObj, pProfile );
    if ( pSemanticError == NULL )
      anzSuccess++;
    else
    {
      if ( generateError )
      {
        if ( combinationType == AND )
          pProfile->addError ( pSemanticError );
        else
          vSemanticErrors.push_back( pSemanticError );
      }
      else
        delete pSemanticError;
    }
  }

	for ( i = 0; i < relationFilters.size(); i++ )
	{
		pRelationFilter = relationFilters[i];
		pSemanticError = pRelationFilter->checkRule ( pObj, pProfile );
    if ( pSemanticError == NULL )
      anzSuccess++;
    else
    {
      if ( generateError )
      {
        if ( combinationType == AND )
          pProfile->addError ( pSemanticError );
        else
          vSemanticErrors.push_back( pSemanticError );
      }
      else
        delete pSemanticError;
    }
	}

	for ( i = 0; i < groupFilters.size(); i++ )
	{
		pGroupFilter = groupFilters[i];
		pSemanticError = pGroupFilter->checkRule ( pObj, pProfile );
    if ( pSemanticError == NULL )
      anzSuccess++;
    else
    {
      if ( generateError )
      {
        if ( combinationType == AND )
          pProfile->addError ( pSemanticError );
        else
          vSemanticErrors.push_back( pSemanticError );
      }
      else
        delete pSemanticError;
    }
	}

	for ( i = 0; i < geometryFilters.size(); i++ )
	{
		pGeometryFilter = geometryFilters[i];
		pSemanticError = pGeometryFilter->checkRule ( pObj, pProfile );
    if ( pSemanticError == NULL )
      anzSuccess++;
    else
    {
      if ( generateError )
      {
        if ( combinationType == AND )
          pProfile->addError ( pSemanticError );
        else
          vSemanticErrors.push_back( pSemanticError );
      }
      else
        delete pSemanticError;
    }
	}

  switch ( combinationType )
  {
  case AND:
    if ( anzSuccess == anzFilter ) return true;
    else                           return false;

  case OR:
    if ( anzSuccess > 0 )
    {
      for ( i = 0; i < vSemanticErrors.size(); i++ )
        delete vSemanticErrors[i];
      return true;
    }
    else
    {
      AggregatedSemanticError * pAggregatedError = new AggregatedSemanticError ( pObj, Error::ERROR, "Nachfolgende Bedingungen dürfen nicht alle verletzt sein" );
      for ( i = 0; i < vSemanticErrors.size(); i++ )
        pAggregatedError->addError( vSemanticErrors[i] );
      pAggregatedError->generateOverallError();
      pAggregatedError->setBrokenRule( pParentRule );
      pProfile->addError( pAggregatedError );
      return false;
    }
  }

	return false;
}

///////////////////////////////////////////////////////////////////////////////
//  AttributeFilter                                                          //
//  Bedingungen für den Wertebereich eines einzelnen Attributes	      			 //
///////////////////////////////////////////////////////////////////////////////
AttributeFilter::AttributeFilter(  ProfileRule * pParentRuleP )
{
	pParentRule = pParentRuleP;

	minOccurs = 1;
	maxOccurs = INT_MAX;

  location = SELF;
}

AttributeFilter::~ AttributeFilter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index gespeicherten textuellen Attributwert            //
///////////////////////////////////////////////////////////////////////////////
std::string AttributeFilter::getAttributeValue ( size_t index )
{
	if ( index < 0 || index >= attributeValues.size() )
		return "";
	else
		return attributeValues[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index gespeicherten Vergleichsoperator für textuelle   //
//  Attributwerte                                                            //
///////////////////////////////////////////////////////////////////////////////
int AttributeFilter::getAttributeValueOperator ( size_t index )
{
	if ( index < 0 || index >= attributeValueOperators.size() )
		return 0;
	else
		return attributeValueOperators[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der textuellen Attributwerte                          //
///////////////////////////////////////////////////////////////////////////////
size_t AttributeFilter::getAttributeValueAnz()
{
	return attributeValues.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index gespeicherten numerischen Attributwert           //
///////////////////////////////////////////////////////////////////////////////
double AttributeFilter::getNumericValue ( size_t index )
{
  if ( index < 0 || index >= numericValues.size() )
    return 0.0;
  else
    return numericValues[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index gespeicherten Vergleichsoperator für numerische  //
//  Attributwerte                                                            //
///////////////////////////////////////////////////////////////////////////////
int AttributeFilter::getNumericValueOperator ( size_t index )
{
  if ( index < 0 || index >= attributeValueOperators.size() )
    return 0;
  else
    return numericValueOperators[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der textuellen Attributwerte                          //
///////////////////////////////////////////////////////////////////////////////
size_t AttributeFilter::getNumericValueAnz()
{
  return numericValues.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Bezug des Attributes:                                          //
//     Self:        Die Klasse der Regel                                     //
//     PlanBereich: Der zug. Plan-Bereich                                    //
//     Plan;        Der zug. Plan                                            //
///////////////////////////////////////////////////////////////////////////////
void AttributeFilter::setLocation ( std::string loc )
{
  if ( loc == "Self" ) location = SELF;
  else
  if ( loc == "PlanBereich" ) location = PLAN_BEREICH;
  else
  if ( loc == "Plan" ) location = PLAN;
  else
  location = SELF;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Regel für ein bestimmtes Objekt							               //
///////////////////////////////////////////////////////////////////////////////
SemanticError * AttributeFilter::checkRule ( Feature * pObj, SemanticRules * pProfile )
{
	GmlSchema       * pGmlObjekte = pObj->getFeatures()->getGmlSchema();
	string            klassenName = pObj->getKlassenName();
  string            klassenNameRef;
	GmlKlasse       * pGmlKlasse = pGmlObjekte->getGmlKlasseFromElementName ( klassenName );
  PlanBereich     * pPlanBereich;
  BLeitPlan       * pPlan;
	SemanticError   * pError = NULL;
	vector<string>    attributWerteAct;
  vector<string>    vCodeSpaces;
	string		        attributWertAct;
  string            errorText;
	size_t            i, j, anz;
	bool              found;
  double            attributWertNumeric;
  Feature         * pRefObj = NULL; 
  vector<UOM*>      vUOMs;

	if ( pGmlKlasse == NULL )
	{    
		pError = new SemanticError ( pObj, Error::ERROR, "Unbekannte Klasse" );	
		return pError;
	}

  if ( location == PLAN_BEREICH )
  {
    klassenNameRef = klassenName;
    pRefObj        = pObj;

    pPlanBereich = pObj->getPlanBereich();
    klassenName  = pPlanBereich->getKlassenName();
    pGmlKlasse   = pGmlObjekte->getGmlKlasseFromElementName ( klassenName );
    pObj         = pPlanBereich;
  }
  else
  if ( location == PLAN )
  {
    klassenNameRef = klassenName;
    pRefObj        = pObj;

    pPlan = pObj->getPlanBereich()->getBPlan();
    klassenName = pPlan->getKlassenName();
    pGmlKlasse   = pGmlObjekte->getGmlKlasseFromElementName ( klassenName );
    pObj         = pPlan;
  }

	GmlAttribut * pGmlAttribut = pGmlKlasse->getGmlAttribut ( attributeName );
	if ( pGmlAttribut == NULL )
	{
    errorText = "Unbekanntes XPlanGML-Attribut: " + attributeName;
		pError = new SemanticError ( pObj, Error::ERROR, errorText );
    if ( pRefObj != NULL )
    {
      pError->setRelatedFeature( pRefObj );
    }
		return pError;
	}

	ATTRIBUT_TYP typ = pGmlAttribut->getGmlAttributTyp();
	anz = pObj->getAttributWerte ( typ, attributeName, attributWerteAct, vCodeSpaces, vUOMs );

	if ( anz < minOccurs )
	{
		if ( anz == 0 )
       errorText = "Dem Attribut " + attributeName + " ist kein Wert zugewiesen";
		else
      errorText = "Dem Attribut " + attributeName + " sind zu wenig Werte zugewiesen";
 		pError = new SemanticError ( pObj, Error::ERROR, errorText );
    if ( pRefObj != NULL )
    {
      pError->setRelatedFeatureId( pRefObj->getGmlId() );
    }
		pError->setBrokenRule ( pParentRule );
		pError->setAttributes ( pObj );
		return pError;
	}

	if ( anz > maxOccurs )
	{
		if ( maxOccurs == 0 )
      errorText = "Dem Attribut " + attributeName + " ist ein Wert zugewiesen";
		else
      errorText = "Dem Attribut " + attributeName + " sind zu viele Werte zugewiesen";
		pError = new SemanticError ( pObj, Error::ERROR, errorText );

    if ( pRefObj != NULL )
    {
      pError->setRelatedFeatureId( pRefObj->getGmlId() );
    }
		pError->setBrokenRule ( pParentRule );
		pError->setAttributes ( pObj );
		return pError;
	}

	if ( ( numericValues.size() == 0 && attributeValues.size() == 0  ) || anz == 0 ) return pError;

  if ( attributeValues.size() > 0 )
  {
	  for ( j = 0; j < anz; j++ )
	  {
		  attributWertAct = attributWerteAct[j];
		  found = false;
		  for ( i = 0; i < attributeValues.size(); i++ )
		  {
			  if ( attributeValueOperators[i] == EQ )
			  {
				  if ( attributeValues[i] == attributWertAct )
				  {
					  found = true;
					  break;
				  }
			  }
	
			  if ( attributeValueOperators[i] == NOT_EQ )
			  {
				  if ( attributeValues[i] != attributWertAct )
				  {
					  found = true;
					  break;
				  }
			  }

		  }
      if ( found == false ) break;
	  }
	
	  if ( found == false  )
	  {
      errorText = "Der Wert " + attributWertAct + " des Attributs " + attributeName + " ist nicht erlaubt";
		  pError = new SemanticError ( pObj, Error::ERROR, errorText );
      if ( pRefObj != NULL )
      {
        pError->setRelatedFeatureId( pRefObj->getGmlId() );
      }
		  pError->setBrokenRule ( pParentRule );
		  pError->setAttributes ( pObj );
	  }
  }
  else
  if ( numericValues.size() > 0 )
  {
    for ( j = 0; j < anz; j++ )
    {
      attributWertAct = attributWerteAct[j];
      attributWertNumeric = QuConvert::ToDouble( attributWertAct );
      found = false;
      for ( i = 0; i < attributeValues.size(); i++ )
      {
        found = checkNumericRestriction ( attributWertNumeric, numericValues[i],  numericValueOperators[i] );
        if ( found )
          break;
      }             
      if ( found == false ) break;
    }
    if ( found == false  )
    {
      errorText = "Der numerische Wert " + attributWertAct + " des Attributs " + attributeName + " ist außerhalb des gültigen Bereichs";
      pError = new SemanticError ( pObj, Error::ERROR, errorText );
      if ( pRefObj != NULL )
      {
        pError->setRelatedFeatureId( pRefObj->getGmlId() );
      }
      pError->setBrokenRule ( pParentRule );
      pError->setAttributes ( pObj );
    }
  }

	return pError;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Einhaltung einer numerischen Restriktion                   //
///////////////////////////////////////////////////////////////////////////////
bool AttributeFilter::checkNumericRestriction ( double attributWert, double restriction, ATTRIBUTE_VALUE_OPERATOR op )
{
  bool success = false;

  switch ( op )
  {
  case EQ:
    if (  attributWert == restriction )
      success = true;
    break;

  case NOT_EQ:
    if (  attributWert != restriction )
      success = true;
    break;

  case LT:
    if (  attributWert < restriction )
      success = true;
    break;

  case LE:
    if (  attributWert <= restriction )
      success = true;
    break;

  case GT:
    if (  attributWert > restriction )
      success = true;
    break;

  case GE:
    if (  attributWert >= restriction )
      success = true;
    break;
  }
  return success;
 
}

///////////////////////////////////////////////////////////////////////////////
//  AttributeCardinalityFilter                                               //
//  Bedingungen für die Häufigkeit, mit der zwei unterschiedliche Attribute  //
//  belegt sind                                                              //
///////////////////////////////////////////////////////////////////////////////
AttributeCardinalityFilter::AttributeCardinalityFilter(  ProfileRule * pParentRuleP )
{
  pParentRule = pParentRuleP;
  compare = equal;
}

AttributeCardinalityFilter::~ AttributeCardinalityFilter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Vergleichsoperator                                             //
///////////////////////////////////////////////////////////////////////////////
void AttributeCardinalityFilter::setCompareOperator ( std::string op )
{  
  if ( op == "less" ) compare = less;
  else
  if ( op == "lessEqual" ) compare = lessOrEqual;
  else
  if ( op == "equal" ) compare = equal;
  else
  if ( op == "greaterEqual" ) compare = greaterOrEqual;
  else
  if ( op == "greater" ) compare = greater;
}


///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Regel für ein bestimmtes Objekt							               //
///////////////////////////////////////////////////////////////////////////////
SemanticError * AttributeCardinalityFilter::checkRule ( Feature * pObj, SemanticRules * pProfile )
{
  GmlSchema     * pGmlObjekte = pObj->getFeatures()->getGmlSchema();
  string          klassenName = pObj->getKlassenName();
  GmlKlasse     * pGmlKlasse = pGmlObjekte->getGmlKlasseFromElementName ( klassenName );
  SemanticError * pError = NULL;
  string          errorText;
  GmlAttribut   * pGmlAttribut;
	size_t          anzA, anzB;
  bool            success;

  if ( pGmlKlasse == NULL )
  {    
    pError = new SemanticError ( pObj, Error::ERROR, "Unbekannte XPlanGML-Klasse" );	
    return pError;
  }

  pGmlAttribut = pGmlKlasse->getGmlAttribut ( attributNameA );
  if ( pGmlAttribut == NULL )
  {
    errorText = "Unbekanntes XPlanGML-Attribut: " + attributNameA;
    pError = new SemanticError ( pObj, Error::ERROR, errorText );
    return pError;
  }

  ATTRIBUT_TYP typ = pGmlAttribut->getGmlAttributTyp();
  anzA = pObj->getAttributWerteAnz ( typ, attributNameA );

  pGmlAttribut = pGmlKlasse->getGmlAttribut ( attributNameB );
  if ( pGmlAttribut == NULL )
  {
    errorText = "Unbekanntes XPlanGML-Attribut: " + attributNameB;
    pError = new SemanticError ( pObj, Error::ERROR, errorText );
    return pError;
  }

  typ = pGmlAttribut->getGmlAttributTyp();
  anzB = pObj->getAttributWerteAnz ( typ, attributNameB );

  switch ( compare )
  {      
  case less:
    if ( anzB < anzA ) success = true;
    else               success = false;
    break;

  case lessOrEqual:
    if ( anzB <= anzA ) success = true;
    else                success = false;
    break;

  case equal:
    if ( anzB == anzA ) success = true;
    else                success = false;
    break;

  case greaterOrEqual:
    if ( anzB >= anzA ) success = true;
    else                success = false;
    break;

  case greater:
    if ( anzB > anzA ) success = true;
    else               success = false;
    break;
  }

  if ( !success )
  {
    errorText = "Falsche Kardinalität der Attribute " + attributNameA + " und " + attributNameB;
    pError = new SemanticError ( pObj, Error::ERROR, errorText );
  }

  return pError;  
}

///////////////////////////////////////////////////////////////////////////////
//  GroupFilter                                                              //
//  Spezifikation von Gruppen von Attributen, Relationen und Geometrie-      //
//  Eigenschaften, die entweder alle belegt sein müssen (AND Verknüpfung),   //
//  von denen mindestens eines belegt sein muss (OR Verknüpfung), oder genau //
//  eines belegt sein muss (XOR Verknüpfung)                                 //
///////////////////////////////////////////////////////////////////////////////
GroupFilter::GroupFilter( ProfileRule * pParentRuleP )
{
	pParentRule = pParentRuleP;
	combination = XOR;
}

GroupFilter::~GroupFilter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Attributnamen hinzu                                           //
///////////////////////////////////////////////////////////////////////////////
void GroupFilter::addAttributeName ( std::string name )
{
	attributeNames.push_back ( name );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Attributnamen                                               //
///////////////////////////////////////////////////////////////////////////////
size_t GroupFilter::getAttributeNames ( std::vector<std::string> & vAttributeNames )
{
	for ( unsigned int i = 0; i < attributeNames.size(); i++ )
		vAttributeNames.push_back ( attributeNames[i] );

	return vAttributeNames.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Relationsnamen hinzu                                          //
///////////////////////////////////////////////////////////////////////////////
void GroupFilter::addRelationName ( std::string name )
{
	relationNames.push_back ( name );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Relationsnamen                                              //
///////////////////////////////////////////////////////////////////////////////
size_t GroupFilter::getRelationNames ( std::vector<std::string> & vRelationNames )
{
	for ( unsigned int i = 0; i < relationNames.size(); i++ )
		vRelationNames.push_back ( relationNames[i] );

	return vRelationNames.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Geometrieeigenschafts-Namen hinzu                             //
///////////////////////////////////////////////////////////////////////////////
void GroupFilter::addGeometryPropertyName ( std::string name )
{
	geometryPropertyNames.push_back ( name );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Geometrieeigenschafts-Namen                                 //
///////////////////////////////////////////////////////////////////////////////
size_t GroupFilter::getGeometryPropertyNames ( std::vector<std::string> & vGeometryPropertyNames )
{
	for ( unsigned int i = 0; i < geometryPropertyNames.size(); i++ )
		vGeometryPropertyNames.push_back ( geometryPropertyNames[i] );

	return vGeometryPropertyNames.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Regel für ein bestimmtes Objekt                            //
///////////////////////////////////////////////////////////////////////////////
SemanticError * GroupFilter::checkRule ( Feature * pObj, SemanticRules * pProfile )
{
	GmlSchema       * pGmlObjekte = pObj->getFeatures()->getGmlSchema();
	string            klassenName = pObj->getKlassenName();
	GmlKlasse       * pGmlKlasse = pGmlObjekte->getGmlKlasseFromElementName ( klassenName );
	SemanticError   * pError = NULL;
	string		        attributeName;
	string		        relationName;
  string            errorText;
	string			      geometryPropertyName;
	size_t      	    i, anz, ruleCount;

	ruleCount = 0;

	if ( pGmlKlasse == NULL )
	{
    errorText = "Die Klasse " + klassenName + " ist unbekannt";
		pError = new SemanticError ( pObj, Error::ERROR, errorText );
		
		return pError;
	}

	for ( i = 0; i < attributeNames.size(); i++ )
	{
		attributeName = attributeNames[i];
		GmlAttribut * pGmlAttribut = pGmlKlasse->getGmlAttribut ( attributeName );
		if ( pGmlAttribut == NULL )
		{
      errorText = "Unbekanntes Attribut: " + attributeName;
			pError = new SemanticError ( pObj, Error::ERROR, errorText );
			
			return pError;
		}

		ATTRIBUT_TYP typ = pGmlAttribut->getGmlAttributTyp();
		anz = pObj->getAttributWerteAnz ( typ, attributeName );
		if ( anz > 0 ) ruleCount++;
	}

	for ( i = 0; i < relationNames.size(); i++ )
	{
		relationName = relationNames[i];
		anz = pObj->getFeatureRelationenAnz ( relationName );
		if ( anz > 0 ) ruleCount++;
	}

	for ( i = 0; i < geometryPropertyNames.size(); i++ )
	{
		geometryPropertyName = geometryPropertyNames[i];
		anz = pObj->getGeometryPropertyAnz ( geometryPropertyName );
		if ( anz > 0 ) ruleCount++;
	}

	anz = attributeNames.size() + relationNames.size() + geometryPropertyNames.size();

	switch ( combination )
	{
	case AND:
		if ( anz == ruleCount ) return pError;
		else
		{
			pError = new SemanticError ( pObj, Error::ERROR, "Ein(e) Attribut / Relation / geometrische Eigenschaft der Gruppe ist nicht belegt" );
			pError->setBrokenRule ( pParentRule );
			pError->setAttributes ( pObj );
			
			return pError;
		}

	case OR:
		if ( ruleCount > 0 ) return pError;
		else
		{
			pError = new SemanticError( pObj, Error::ERROR, "Kein(e) Attribut / Relation / geometrische Eigenschaft der Gruppe ist belegt" );
			pError->setBrokenRule ( pParentRule );
			pError->setAttributes ( pObj );
			
			return pError;
		}

	case XOR:
		if ( ruleCount == 1 ) return pError;
		else
		{
  		pError = new SemanticError ( pObj, Error::ERROR, "Mehr als ein(e) oder kein(e) Attribut / Relation / geometrische Eigenschaft der Gruppe ist belegt" );
			pError->setBrokenRule ( pParentRule );
			pError->setAttributes ( pObj );
			
			return pError;
		}

  case XOR_OPT:
    if ( ruleCount <= 1 ) return pError;
    else
    {
      pError = new SemanticError ( pObj, Error::ERROR, "Mehr als ein(e) Attribut / Relation / geometrische Eigenschaft der Gruppe ist belegt" );
      pError->setBrokenRule ( pParentRule );
      pError->setAttributes ( pObj );
      
      return pError;
    }
	}

	return pError;
}

///////////////////////////////////////////////////////////////////////////////
//  GeometryFilter                                                           //
//  Bedingungen an referierte Geometrie							                  			 //
///////////////////////////////////////////////////////////////////////////////
GeometryFilter::GeometryFilter ( ProfileRule * pParentRuleP )
{
	pParentRule			    	= pParentRuleP;
	geometryRestriction		= NO_GEOMETRY_RESTRICTION;
  surfaceNormal         = INDIFFERENT;

  minOccurs = 1;
  maxOccurs = INT_MAX;

}

GeometryFilter::~ GeometryFilter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Restriktion des Geometrie-Typs                                 //
///////////////////////////////////////////////////////////////////////////////
void GeometryFilter::setGeometryRestriction ( std::string rest )
{
	if ( rest == "OnlyPoint" )     geometryRestriction = ONLY_POINT;
	else  
	if ( rest == "OnlyLine" )      geometryRestriction = ONLY_LINE;
	else
	if ( rest == "OnlyArea" )      geometryRestriction = ONLY_AREA;
	else
	if ( rest == "NoPoint" )       geometryRestriction = NO_POINT;
	else
	if ( rest == "NoLine" )        geometryRestriction = NO_LINE;
	else
	if ( rest == "NoArea" )        geometryRestriction = NO_AREA;
  else
  if ( rest == "NoMultiObject" ) geometryRestriction = NO_MULTI_AREA;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die vorgegebene Richtung der Flächen-Normale                       //
///////////////////////////////////////////////////////////////////////////////
void GeometryFilter::setSurfaceNormal( string ori )
{
  if ( ori == "Horizontal" )       surfaceNormal = HORIZONTAL;
  else
  if ( ori == "VerticalUpward" )   surfaceNormal = VERTICAL_UPWARD;
  else
  if ( ori == "Upward" )           surfaceNormal = UPWARD;
  else
  if ( ori == "Downward" )         surfaceNormal = DOWNWARD;
  else
  if ( ori == "VerticalDownward" ) surfaceNormal = VERTICAL_DOWNWARD;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Regel für ein bestimmtes Objekt                            //
///////////////////////////////////////////////////////////////////////////////
SemanticError * GeometryFilter::checkRule ( Feature * pObj, SemanticRules * pProfile )
{
	GmlSchema              * pGmlObjekte = pObj->getFeatures()->getGmlSchema();
	SemanticError          * pError = NULL;
	size_t                   i, propertyAnz;
  _Geometrie             * pGeometry;
  string                   aktuell, gefordert, errorString;
	ABSTRACT_GEOMETRY_TYPE   geometrieTyp = pObj->getAbstractGeometrieTyp( geometryPropertyName );
  SURFACE_NORMAL_TYPE      surfaceOrientationAct;

	switch (  geometrieTyp )
	{
	case ABSTRACT_SURFACE:
		if ( geometryRestriction == ONLY_POINT ||
			   geometryRestriction == ONLY_LINE  ||
			   geometryRestriction == NO_AREA )
		{
      aktuell = "Flächengeometrie";
      switch ( geometryRestriction )
      {
      case ONLY_POINT:
        gefordert = "Punktgeometrie";
        break;
      case ONLY_LINE:
        gefordert = "Liniengeometrie";
        break;
      case NO_AREA:
        gefordert = "Keine Flächengeometrie";
        break;
      }
      errorString = "Falsche Objekt-Geometrie " + aktuell + ". Gefordert ist " + gefordert;

			pError = new SemanticError ( pObj, Error::ERROR, errorString );
			pError->setBrokenRule ( pParentRule );
			pError->setAttributes ( pObj );
			
			return pError;
		}
		break;

		case ABSTRACT_CURVE:
		if ( geometryRestriction == ONLY_POINT ||
		  	 geometryRestriction == ONLY_AREA  ||
		  	 geometryRestriction == NO_LINE )
		{
      aktuell = "Liniengeometrie";
      switch ( geometryRestriction )
      {
      case ONLY_POINT:
        gefordert = "Punktgeometrie";
        break;
      case NO_LINE:
        gefordert = "Keine Liniengeometrie";
        break;
      case ONLY_AREA:
        gefordert = "Flächengeometrie";
        break;
      }
      errorString = "Falsche Objekt-Geometrie " + aktuell + ". Gefordert ist " + gefordert;

		pError = new SemanticError ( pObj, Error::ERROR, errorString );
		pError->setBrokenRule ( pParentRule );
		pError->setAttributes ( pObj );
			
	  return pError;
		}
		break;

		case ABSTRACT_POINT:
		if ( geometryRestriction == ONLY_LINE ||
			   geometryRestriction == ONLY_AREA  ||
			   geometryRestriction == NO_POINT )
		{
      aktuell = "Punktgeometrie";
      switch ( geometryRestriction )
      {
      case NO_POINT:
        gefordert = "Keine Punktgeometrie";
        break;
      case ONLY_LINE:
        gefordert = "Liniengeometrie";
        break;
      case ONLY_AREA:
        gefordert = "Flächengeometrie";
        break;
      }
      errorString = "Falsche Objekt-Geometrie " + aktuell + ". Gefordert ist " + gefordert;

  		pError = new SemanticError ( pObj, Error::ERROR, errorString );
			pError->setBrokenRule ( pParentRule );
			pError->setAttributes ( pObj );
			
			return pError;
		}
		break;
	}

  propertyAnz = pObj->getGeometryPropertyAnz( geometryPropertyName );
  if ( propertyAnz > maxOccurs )
  {
    if ( maxOccurs == 0 )
      errorString = "Dem Geometrieattribut " + geometryPropertyName + " ist ein Wert zugewiesen";
    else
      errorString = "Dem Geometrieattribut " + geometryPropertyName + " sind zuviele Werte zugewiesen";
    pError = new SemanticError ( pObj, Error::ERROR, errorString );    
    pError->setBrokenRule ( pParentRule );
    pError->setAttributes ( pObj );
    return pError;
  }
  else
  if ( propertyAnz < minOccurs )
  {
    if ( propertyAnz == 0 )
      errorString = "Dem Geometrieattribut " + geometryPropertyName + " ist kein Wert zugewiesen";
    else
      errorString = "Dem Geometrieattribut " + geometryPropertyName + " sind zu wenig Werte zugewiesen";
    pError = new SemanticError ( pObj, Error::ERROR, errorString );
    
    pError->setBrokenRule ( pParentRule );
    pError->setAttributes ( pObj );
    
    return pError;
  }

  if ( surfaceNormal != INDIFFERENT && 
    ( geometrieTyp == ABSTRACT_SURFACE || geometrieTyp == ABSTRACT_MULTI_GEOMETRY ) )
  {
    double angleTolerance = pProfile->getCheckingParams()->minimalAngleDistance;
    double areaTolerance  = pProfile->getCheckingParams()->planarityLimit;

    for ( i = 0; i < propertyAnz; i++ )
    {
      pGeometry = pObj->getGeometry( geometryPropertyName, i );
      _Surface * pSurface = dynamic_cast<_Surface*> (pGeometry );
      if ( pSurface != NULL )
      {
        surfaceOrientationAct = pSurface->getSurfaceNormal( angleTolerance , areaTolerance );
        if ( surfaceNormal == surfaceOrientationAct                                  || 
             ( surfaceNormal == UPWARD && surfaceOrientationAct == VERTICAL_UPWARD ) || 
             ( surfaceNormal == DOWNWARD && surfaceOrientationAct == VERTICAL_DOWNWARD ) )
             continue;

        errorString = "Falsche Flächenorientierung, gefordert ist " + _Surface::convertOrientationToString( surfaceNormal ) + 
          " Die aktuelle Orientierung ist " + _Surface::convertOrientationToString( surfaceOrientationAct );
        pError = new SemanticError ( pObj, Error::ERROR, errorString );

        pError->setBrokenRule ( pParentRule );
        pError->setAttributes ( pObj );

        return pError;
      }
    }

  }

	return pError;
}

///////////////////////////////////////////////////////////////////////////////
//  RelationFilter                                                           //
//  Bedingungen für den Wertebereich einer Relation		            					 //
///////////////////////////////////////////////////////////////////////////////
RelationFilter::RelationFilter( ProfileRule * pParentRuleP )
{
	pParentRule = pParentRuleP;

  relationName                        = "";
  inverseRelationName                 = "";
  attributNameInRelatedObject         = "";
  requiredAttributWertInRelatedObject = "";
  relationNameInRelatedObject         = "";
  geometryPropertyNameInRelatedObject = "";

	minOccurs = 0;
	maxOccurs = INT_MAX;
  
  minOccursProperty = 0;
  maxOccursProperty = INT_MAX;
}

RelationFilter::~ RelationFilter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index referierten Basisklassen-Typnamen                //      
///////////////////////////////////////////////////////////////////////////////
string RelationFilter::getGmlSuperTypeName ( unsigned int index )
{
  if ( index < 0 || index >= gmlSuperType.size() )   return "";
  else                                               return gmlSuperType[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index referierten Typnamen                             //      
///////////////////////////////////////////////////////////////////////////////
std::string RelationFilter::getGmlTypeName ( unsigned int index )
{
  if ( index < 0 || index >= gmlType.size() )
    return "";
  else
    return gmlType[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index referierten textuellen Attributwert im           //
//  verbundenen Objekt                                                       //      
///////////////////////////////////////////////////////////////////////////////
std::string RelationFilter::getAttributWertInRelatedObject( unsigned int index )
{
  if ( index < 0 || index >= attributWertInRelatedObject.size() )
    return "";
  else
    return attributWertInRelatedObject[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index referierten Vergleichsoperator für textuelle     //
//  Attributwert im verbundenen Objekt                                       //      
///////////////////////////////////////////////////////////////////////////////
int RelationFilter::getAttributWertInRelatedObjectOperator( unsigned int index )
{
  if ( index < 0 || index >= attributWertInRelatedObjectOperators.size() )
    return 0;
  else
    return attributWertInRelatedObjectOperators[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index referierten numerischen Attributwert im          //
//  verbundenen Objekt                                                       //      
///////////////////////////////////////////////////////////////////////////////
double RelationFilter::getNumericWertInRelatedObject( unsigned int index )
{
  if ( index < 0 || index >= numericWertInRelatedObject.size() )
    return 0;
  else
    return numericWertInRelatedObject[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index referierten Vergleichsoperator für numerische    //
//  Attributwert im verbundenen Objekt                                       //      
///////////////////////////////////////////////////////////////////////////////
int RelationFilter::getNumericWertInRelatedObjectOperator( unsigned int index )
{
  if ( index < 0 || index >= numericWertInRelatedObjectOperators.size() )
    return 0;
  else
    return numericWertInRelatedObjectOperators[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Regel für ein bestimmtes Objekt		              					 //
///////////////////////////////////////////////////////////////////////////////
SemanticError * RelationFilter::checkRule ( Feature * pObj, SemanticRules * pProfile )
{
	vector<FeatureRelation*> objRelationen;
  vector<FeatureRelation*> inverseObjRelationen;
  vector<Feature*>         relatedObjects;
  vector<string>           attributWerteAct;
  vector<string>           vCodeSpaces;
	FeatureRelation		     * pObjRel;
	Feature			           * pPlanObj;
	size_t      	           i, j, relAnz, inversRelAnz, anz;
	string					         objId;
	string					         klassenname;
	string					         typName;
  string                   typNameLoc;
  string                   errorText;
  string                   attributWertAct;
	SemanticError          * pError = NULL;
	GmlKlasse              * pGmlKlasse;
	bool                     found, errorExist = false;
  double                   numericWertAct;
  vector<UOM*>             vUOMs;

	relAnz = pObj->getFeatureRelationen ( relationName, objRelationen );

  if ( attributNameInRelatedObject == "" || requiredAttributWertInRelatedObject == "" )
  {
  	if ( relAnz < minOccurs ) 
	  {
	    if ( relAnz == 0 )
        errorText = "Die Relation " + relationName + " ist nicht benutzt";
		  else
        errorText = "Die Relation " + relationName + " wird nicht häufig genug benutzt";
		  pError = new SemanticError ( pObj, Error::ERROR,errorText );
		  pError->setRelationName ( relationName );
		  pError->setBrokenRule ( pParentRule );

		  return pError;
	  }

	  if ( relAnz > maxOccurs )
	  {
		  if ( maxOccurs == 0 )
        errorText = "Die Relation " + relationName + " wird benutzt";
	  	else
        errorText = "Die Relation " + relationName + " wird zu häufig benutzt";
		  pError = new SemanticError ( pObj, Error::ERROR, errorText );
		  pError->setRelationName ( relationName );
		  pError->setBrokenRule ( pParentRule );

      return pError;
	  }
  }


	GmlSchema * pGmlObjekte = pObj->getFeatures()->getGmlSchema();

	for ( i = 0; i < relAnz; i++ )
	{
		pObjRel = objRelationen[i];
		objId   = pObjRel->featureId;
		pPlanObj = pObj->getFeatures()->getFeature ( objId );
		if ( pPlanObj == NULL )
		{
      errorText = "Ungültiges xlink der Relation " + relationName + ": " +  objId;
			pError = new SemanticError ( pObj, Error::ERROR, errorText );
			pError->setRelationName ( relationName );
			pError->setRelatedFeatureId( objId );
			pError->setBrokenRule ( pParentRule );

      return pError;       		
		}
    relatedObjects.push_back( pPlanObj );

		klassenname = pPlanObj->getKlassenName ();
		pGmlKlasse  = pGmlObjekte->getGmlKlasseFromElementName ( klassenname );
		typName     = pGmlKlasse->getGmlKlassenName();

		if ( gmlType.size() != 0 )
    {
      found = false;
      for ( j = 0; j < gmlType.size(); j++ )
      {
        if ( typName == gmlType[j] )
        {
          found = true;
          break;
        }      
      }

      if ( !found )
		  {
        errorText = "Der Typ " + typName + " des über die Relation " + relationName + " referierten Objekts ist ungültig";
				pError = new SemanticError ( pObj, Error::ERROR, errorText );
				pError->setRelationName ( relationName );
				pError->setRelatedFeature ( pPlanObj );
				pError->setBrokenRule ( pParentRule );

        return pError;
			}
    }
    else
    if ( gmlSuperType.size() != 0 )
    {
      pGmlKlasse = pGmlObjekte->getGmlKlasseFromElementName ( klassenname );
      found = false;
      for ( j = 0; j < gmlSuperType.size(); j++ )
      {
        typNameLoc = gmlSuperType[j];
        if ( pGmlKlasse->getGmlKlassenName() == typNameLoc || pGmlKlasse->isParentTypeName( typNameLoc ) )
        {
          found = true;
          break;
        }

      }
      if ( !found )
      {
        errorText = "Der Typ " + typName + " des über die Relation " + relationName + " referierten Objekts ist ungültig";
        pError = new SemanticError ( pObj,Error::ERROR, errorText );
        pError->setRelationName ( relationName );
        pError->setRelatedFeature ( pPlanObj );
        pError->setBrokenRule ( pParentRule );
        return pError;
      }

    }

    if ( inverseRelationName != "" )
    {
      inverseObjRelationen.clear();
      inversRelAnz = pPlanObj->getFeatureRelationen( inverseRelationName, inverseObjRelationen );
      found = false;
      for ( j = 0; j < inversRelAnz; j++ )
      {
        pObjRel = inverseObjRelationen[j];
        objId = pObjRel->featureId;
        if ( objId == pObj->getGmlId() )
        {
          found = true;
          break;
        }
      }
      if ( !found )
      {
        if ( inversRelAnz == 0 )
          errorText = "Die zu " + relationName + " inverse Relation ist nicht belegt";
        else
          errorText = "Die zu " + relationName + " inverse Relation zeigt auf das falsche Objekt";
        pError = new SemanticError ( pObj, Error::ERROR, errorText );
        pError->setRelationName ( relationName );
        pError->setRelatedFeatureId ( objId );
        pError->setBrokenRule ( pParentRule );

        return pError;
      }

    }
	}  

  if ( attributNameInRelatedObject != "" && requiredAttributWertInRelatedObject != "" )
  {
    anz = 0;
    for ( i = 0; i < relatedObjects.size(); i++  )
    {
      Feature * pFeature = relatedObjects[i];

      klassenname  = pFeature->getKlassenName();
      pGmlKlasse   = pGmlObjekte->getGmlKlasseFromElementName ( klassenname );

      GmlAttribut * pGmlAttribut = pGmlKlasse->getGmlAttribut ( attributNameInRelatedObject );
      if ( pGmlAttribut != NULL )
      {
        attributWerteAct.clear();
        vCodeSpaces.clear();

        ATTRIBUT_TYP typ = pGmlAttribut->getGmlAttributTyp();
        pFeature->getAttributWerte ( typ, attributNameInRelatedObject, attributWerteAct, vCodeSpaces, vUOMs );
        for ( j = 0; j < attributWerteAct.size(); j++ )
          if ( attributWerteAct[j] == requiredAttributWertInRelatedObject )
            anz++;
      }
    }

    if ( anz < minOccurs )
    {
      if ( anz == 0 )
        errorText = "Die Attribut-Wert Kombination \"" + attributNameInRelatedObject + " == " + requiredAttributWertInRelatedObject + 
        "\" kommt in den über " + relationName +" referierten Objekten nicht vor";
      else
        errorText = "Die Attribut-Wert Kombination \"" + attributNameInRelatedObject + " == " + requiredAttributWertInRelatedObject + 
        "\" kommt in den über " + relationName +" referierten Objekten nicht häufig genug vor";
      pError = new SemanticError ( pObj, Error::ERROR, errorText );

      pError->setBrokenRule ( pParentRule );
      pError->setAttributes ( pObj );

      return pError;
    }

    if ( anz > maxOccurs )
    {
      if ( maxOccursProperty == 0 )
        errorText = "Die Attribut-Wert Kombination \"" + attributNameInRelatedObject + " == " + requiredAttributWertInRelatedObject + 
        "\" kommt in den über " + relationName +" referierten Objekten vor";
      else
        errorText = "Die Attribut-Wert Kombination \"" + attributNameInRelatedObject + " == " + requiredAttributWertInRelatedObject + 
        "\" kommt in den über " + relationName + " referierten Objekten nicht häufig genug vor";
      pError = new SemanticError ( pObj, Error::ERROR, errorText );

      pError->setBrokenRule ( pParentRule );
      pError->setAttributes ( pObj );

      return pError;
    }
  }

  if ( attributNameInRelatedObject != "" && relatedObjects.size() > 0 )
  {
    attributWerteAct.clear();
    vCodeSpaces.clear();

    for ( i = 0; i < relatedObjects.size(); i++  )
    {
      Feature * pFeature = relatedObjects[i];

      klassenname  = pFeature->getKlassenName();
      pGmlKlasse   = pGmlObjekte->getGmlKlasseFromElementName ( klassenname );

      GmlAttribut * pGmlAttribut = pGmlKlasse->getGmlAttribut ( attributNameInRelatedObject );
      if ( pGmlAttribut != NULL )
      {
        ATTRIBUT_TYP typ = pGmlAttribut->getGmlAttributTyp();
        pFeature->getAttributWerte ( typ, attributNameInRelatedObject, attributWerteAct, vCodeSpaces, vUOMs );        
      }
    }

    anz = attributWerteAct.size();

    if ( anz < minOccursProperty )
    {
      if ( anz == 0 )
        errorText = "Dem Attribut " + attributNameInRelatedObject + " im referierten Objekt ist kein Wert zugewiesen";
      else
        errorText = "Dem Attribut " + attributNameInRelatedObject + " im referierten Objekt sind zu wenig Werte zugewiesen";
      pError = new SemanticError ( pObj, Error::ERROR, errorText );
        
      pError->setAttributes ( pObj );
      
      return pError;
    }

    if ( anz > maxOccursProperty )
    {
      if ( maxOccursProperty == 0 )
        errorText = "Dem Attribut " + attributNameInRelatedObject + " im referierten Objekt ist ein Wert zugewiesen";
      else
        errorText = "Dem Attribut " + attributNameInRelatedObject + " im referierten Objekt sind zuviele Werte zugewiesen";
      pError = new SemanticError ( pObj, Error::ERROR, errorText );
        
      pError->setBrokenRule ( pParentRule );
      pError->setAttributes ( pObj );

     return pError;
    }

    if ( attributWertInRelatedObject.size() > 0 && anz > 0 )
    {
      for ( j = 0; j < anz; j++ )
      {
        attributWertAct = attributWerteAct[j];
        found = false;

        for ( i = 0; i < attributWertInRelatedObject.size(); i++ )
        {
          if ( attributWertInRelatedObjectOperators[i] == EQ )
          {
            if ( attributWertInRelatedObject[i] == attributWertAct )
            {
              found = true;
              break;
            }
          }

          if ( attributWertInRelatedObjectOperators[i] == NOT_EQ )
          {
            if ( attributWertInRelatedObject[i] != attributWertAct )
            {
              found = true;
              break;
            }
          }

          if ( found == true ) break;

        }
        if ( found == true )
          break;
      }

      if ( found == false )
      {
        errorText = "Der Attributwert " + attributWertAct + " in einem referierten Objekt ist nicht erlaubt";
        pError = new SemanticError ( pObj, Error::ERROR, errorText );

        pError->setBrokenRule ( pParentRule );
        pError->setAttributes ( pObj );
        return pError;
      }
    }   

    if ( numericWertInRelatedObject.size() > 0 && anz > 0 )
    {
      for ( j = 0; j < anz; j++ )
      {
        attributWertAct = attributWerteAct[j];
        numericWertAct  = QuConvert::ToDouble( attributWertAct );
        found = false;

        for ( i = 0; i < attributWertInRelatedObject.size(); i++ )
        {
          found = AttributeFilter::checkNumericRestriction( numericWertAct,numericWertInRelatedObject[i], numericWertInRelatedObjectOperators[i] );         
          if ( found == true ) 
            break;
        }
        if ( found == false )
          break;
      }

      if ( found == false )
      {
        errorText = "Der numerische Attributwert " + attributWertAct + " in einem referierten Objekt ist außerhalb des erlaubten Bereichs";
        pError = new SemanticError ( pObj, Error::ERROR, errorText );

        pError->setBrokenRule ( pParentRule );
        pError->setAttributes ( pObj );
        return pError;
      }
    }   
  }

  if ( relationNameInRelatedObject != "" && relatedObjects.size() > 0 )
  {
    objRelationen.clear();
    
    for ( i = 0; i < relatedObjects.size(); i++  )
    {
      Feature * pFeature = relatedObjects[i];
      pFeature->getFeatureRelationen ( relationNameInRelatedObject, objRelationen );
    }

    relAnz = objRelationen.size();
    if ( relAnz < minOccursProperty ) 
    {
      if ( relAnz == 0 )
        errorText = "Die Relation " + relationNameInRelatedObject + " wird in den referierten Objekten  nicht benutzt";
      else
        errorText = "Die Relation " + relationNameInRelatedObject + " wird in den referierten Objekten  nicht häufig genug benutzt";
      pError = new SemanticError ( pObj, Error::ERROR, errorText );
      pError->setRelationName ( relationNameInRelatedObject );
      pError->setBrokenRule ( pParentRule );

      return pError;
    }

    if ( relAnz > maxOccursProperty )
    {
      if ( maxOccursProperty == 0 )
        errorText = "Die Relation " + relationNameInRelatedObject + " wird in den referierten Objekten benutzt";
      else
        errorText = "Die Relation " + relationNameInRelatedObject + " wird in den referierten Objekten zu häufig benutzt";
      pError = new SemanticError ( pObj, Error::ERROR, errorText );
      pError->setRelationName ( relationName );
      pError->setBrokenRule ( pParentRule );
      
      return pError;
    }      
  }

  if ( geometryPropertyNameInRelatedObject != "" && relatedObjects.size() > 0 )
  {
    anz = 0; 
    for ( i = 0; i < relatedObjects.size(); i++  )
    {
      Feature * pFeature = relatedObjects[i];
      anz = anz +  pFeature->getGeometryPropertyAnz( geometryPropertyNameInRelatedObject );
    }

    if ( anz < minOccursProperty ) 
    {
      if ( anz == 0 )
        errorText = "Die Geometrieeigenschaft " + geometryPropertyNameInRelatedObject + "wird in den referierten Objekten nicht genutzt";
      else
        errorText = "Die Geometrieeigenschaft " + geometryPropertyNameInRelatedObject + "wird in den referierten Objekten nicht häufig genug genutzt";
      pError = new SemanticError ( pObj, Error::ERROR, errorText );
      pError->setRelationName ( geometryPropertyNameInRelatedObject );
      pError->setBrokenRule ( pParentRule );
      
      return pError;
    }

    if ( anz > maxOccursProperty )
    {
      if ( maxOccursProperty == 0 )
        errorText = "Die Geometrieeigenschaft " + geometryPropertyNameInRelatedObject + "wird in den referierten Objekten genutzt";
      else
        errorText = "Die Geometrieeigenschaft " + geometryPropertyNameInRelatedObject + "wird in den referierten Objekten zu häufig genutzt";
      pError = new SemanticError ( pObj, Error::ERROR, errorText );
      pError->setRelationName ( relationName );
      pError->setBrokenRule ( pParentRule );
      
      return pError;
    }      
  }

	return pError;
}

///////////////////////////////////////////////////////////////////////////////
//  AbstractReport                                                           //
//  Basisklasse für Fehlerreports			          		                				 //
///////////////////////////////////////////////////////////////////////////////
AbstractReport::AbstractReport( Checking * pCheckingP )
{
  m_pChecking = pCheckingP;

//  modelFileName = "";
  testDate      = "";
}

AbstractReport::~AbstractReport()
{

}

///////////////////////////////////////////////////////////////////////////////
//  ErrorReport                                                              //
//  Konformitätsfehler-Report                				                				 //
///////////////////////////////////////////////////////////////////////////////
ErrorReport::ErrorReport( Checking * pCheckingP ) : AbstractReport ( pCheckingP )
{
}

ErrorReport::~ErrorReport()
{
	unsigned int i;
  multimap<int,GeometryError*>::iterator iter;

	for ( i = 0; i < semanticErrors.size(); i++ )
		delete semanticErrors[i];

  for ( i = 0; i < validationErrors.size(); i++ )
    delete validationErrors[i];

  for ( i = 0; i < readErrors.size(); i++ )
    delete readErrors[i];

 for ( iter = geometryErrors.begin(); iter != geometryErrors.end(); iter++ )
    delete iter->second;

}

///////////////////////////////////////////////////////////////////////////////
//  Löscht die gespeicherten Objekte für Semantikfehler          						 //
///////////////////////////////////////////////////////////////////////////////
void ErrorReport::resetSemanticErrors()
{
  unsigned int i;

  for ( i = 0; i < semanticErrors.size(); i++ )
    delete semanticErrors[i];

  semanticErrors.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht die gespeicherten Objekte für Geometriefehler         						 //
///////////////////////////////////////////////////////////////////////////////
void ErrorReport::resetGeometryErrors()
{
  multimap<int,GeometryError*>::iterator iter;

  for ( iter = geometryErrors.begin(); iter != geometryErrors.end(); iter++ )
  {
    GeometryError * pError = iter->second;
    delete iter->second;
  }

  geometryErrors.clear();
  geometryErrorTypes.clear();
  parentFeatureTypes.clear();
  geometryErrorMap.clear();
  parentMap.clear();
  parentErrorMap.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht die gespeicherten Objekte für Einlesefehler           						 //
///////////////////////////////////////////////////////////////////////////////
void ErrorReport::resetReadErrors()
{
  unsigned int i;

  for ( i = 0; i < readErrors.size(); i++ )
    delete readErrors[i];

  readErrors.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht die gespeicherten Objekte für Validierungsfehler      						 //
///////////////////////////////////////////////////////////////////////////////
void ErrorReport::resetValidationErrors()
{
  unsigned int i;

  for ( i = 0; i < validationErrors.size(); i++ )
    delete validationErrors[i];

  validationErrors.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Semantik-Fehler hinzu  	                        						 //
///////////////////////////////////////////////////////////////////////////////
void ErrorReport::addSemanticError ( SemanticError * pError )
{  
	int errorNumber = (int)semanticErrors.size() + 1;
	pError->setErrorNumber ( errorNumber );
	semanticErrors.push_back ( pError ); 
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Geometrie-Fehler hinzu  	                        						 //
///////////////////////////////////////////////////////////////////////////////
void ErrorReport::addGeometryError ( GeometryError * pError )
{
  int errorNumber = (int)geometryErrors.size();
  pError->setErrorNumber ( errorNumber );
  geometryErrorMap[errorNumber] = pError;

  int errorType = pError->getGeometryErrorType();
  
  geometryErrorTypes.insert( errorType );
  geometryErrors.insert(  multimap<int,GeometryError*>::value_type ( errorType, pError ) );

  Feature * pFeature = pError->getFeature();
  if ( pFeature != NULL )
  {
    Feature * pRootFeature = pFeature->getRootFeature();
    if ( pRootFeature != NULL )
    {
      parentFeatureTypes.insert( pRootFeature->getKlassenName() );
      parentMap.insert( multimap<string,Feature*>::value_type ( pRootFeature->getKlassenName(), pRootFeature ) );
      parentErrorMap.insert( multimap<Feature*,GeometryError*>::value_type ( pRootFeature, pError ) );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Validierungsfehler hinzu  	                       						 //
///////////////////////////////////////////////////////////////////////////////
void ErrorReport::addValidationError (ValidationError * pError )
{  
  validationErrors.push_back ( pError );  
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Einlesefehler hinzu       	                       						 //
///////////////////////////////////////////////////////////////////////////////
void ErrorReport::addReadError ( ReadError * pError )
{
  readErrors.push_back ( pError );  
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Fehler hinzu                                                  //
///////////////////////////////////////////////////////////////////////////////
void ErrorReport::addError ( Error * pError )
{
  switch ( pError->getErrorType() )
  {
  case Error::VALIDATION_ERROR:
    validationErrors.push_back ( (ValidationError*)pError );  
    break;

  case Error::GEOMETRY_ERROR:
    {
      GeometryError * pGeometryError = (GeometryError*)pError;
      addGeometryError( pGeometryError );     

      break;
    }

  case Error::SEMANTIC_ERROR:
    {
      SemanticError * pSemanticError = (SemanticError*)pError;
      addSemanticError( pSemanticError ); 

      break;
    }

  case Error::READ_ERROR:
    readErrors.push_back ( (ReadError*)pError );  
    break;
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index gespeicherten Semantikfehler                		 //
///////////////////////////////////////////////////////////////////////////////
SemanticError * ErrorReport::getSemanticError ( size_t index )
{
	if ( index < 0 || index >= semanticErrors.size() ) 
		return NULL;
	else
		return semanticErrors[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Semantikfehler                                		 //
///////////////////////////////////////////////////////////////////////////////
size_t ErrorReport::getSemanticErrorAnz()
{
	return semanticErrors.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den set der Geometriefehler-Typen                           		 //
///////////////////////////////////////////////////////////////////////////////
std::set<int> & ErrorReport::getGeometryErrorTypes ()
{
  return geometryErrorTypes;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Geometriefehler eines bestimmten Typs                  		 //
///////////////////////////////////////////////////////////////////////////////
size_t ErrorReport::getGeometryErrors ( int errorType, std::vector<GeometryError*> &vGeometryErrors )
{
  multimap<int,GeometryError*>::iterator iter;

  for ( iter = geometryErrors.lower_bound( errorType ); iter != geometryErrors.upper_bound( errorType ); iter++ )
  {
    vGeometryErrors.push_back( iter->second );
  }
  return vGeometryErrors.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Geometriefehler mit einer bestimmten Nummer              		 //
///////////////////////////////////////////////////////////////////////////////
GeometryError * ErrorReport::getGeometryError ( int errorNumber )
{
  map<int,GeometryError*>::iterator iter;

  iter = geometryErrorMap.find( errorNumber );
  if ( iter != geometryErrorMap.end() )
    return iter->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Geometriefehler                                        		 //
///////////////////////////////////////////////////////////////////////////////
size_t ErrorReport::getGeometryErrors (std::vector<GeometryError*> &vGeometryErrors )
{
  multimap<int,GeometryError*>::iterator iter;

  for ( iter = geometryErrors.begin(); iter != geometryErrors.end() ; iter++ )
  {
    vGeometryErrors.push_back( iter->second );
  }
  return vGeometryErrors.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Geometriefehler eines bestimmten Typs         		 //
///////////////////////////////////////////////////////////////////////////////
size_t ErrorReport::getGeometryErrorAnz ( int errorType )
{
  return geometryErrors.count( errorType );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl aller Geometriefehler                            		 //
///////////////////////////////////////////////////////////////////////////////
size_t ErrorReport::getGeometryErrorAnz ()
{
  return geometryErrors.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert den set aller Namen von Basis-Features, in denen Geometriefehler //
//  aufgetreten sind                                                         //
///////////////////////////////////////////////////////////////////////////////
set<string> & ErrorReport::getGeometryErrorParentFeatureClasses ()
{
  return parentFeatureTypes;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert zum Namen einer Basis-Features den set aller Basis-Features, in  //
//  denen Geometriefehler aufgetreten sind                                   //
///////////////////////////////////////////////////////////////////////////////
size_t ErrorReport::getGeometryErrorParentFeatures ( std::string featureClassName, std::set<Feature*> & sFeatures )
{
  multimap<string,Feature*>::iterator iter;

  for ( iter = parentMap.lower_bound( featureClassName ); iter != parentMap.upper_bound( featureClassName ); iter++ )
  {
    Feature * pFeature = iter->second;
    sFeatures.insert( pFeature );
  }
  return sFeatures.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert zu einem Basis-Feature die zugehörigen Geometriefehler-Objekte	 //
///////////////////////////////////////////////////////////////////////////////
size_t ErrorReport::getGeometryErrors ( Feature * pParentFeature, std::vector<GeometryError*> &vGeometryErrors )
{
  multimap<Feature*,GeometryError*>::iterator iter;

  for ( iter = parentErrorMap.lower_bound( pParentFeature ); iter != parentErrorMap.upper_bound( pParentFeature ); iter++ )
  {
    GeometryError * pError = iter->second;
    vGeometryErrors.push_back( pError );
  }
  return vGeometryErrors.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index referierten Validierungsfehler                   //
///////////////////////////////////////////////////////////////////////////////
ValidationError * ErrorReport::getValidationError ( size_t index )
{
	if ( index < 0 || index >= validationErrors.size() )
		return NULL;
	else
		return validationErrors[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Validierungsfehler                                //
///////////////////////////////////////////////////////////////////////////////
size_t ErrorReport::getValidationErrorAnz()
{
	return validationErrors.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den unter index referierten Einlesefehler                        //
///////////////////////////////////////////////////////////////////////////////
ReadError * ErrorReport::getReadError ( size_t index )
{
  if ( index < 0 || index >= readErrors.size() )
    return NULL;
  else
    return readErrors[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Einlesefehler                                     //
///////////////////////////////////////////////////////////////////////////////
size_t ErrorReport::getReadErrorAnz()
{
  return readErrors.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Semantikfehler des Features mit Kennung objId               //
///////////////////////////////////////////////////////////////////////////////
size_t ErrorReport::getSemanticErrors ( string objId, vector<SemanticError*> & vSemanticErrors )
{
	size_t          i;
	SemanticError * pError;
  string          objIdLoc;
  Features      * pFeatures = m_pChecking->getFeatures();

	for ( i = 0; i < semanticErrors.size(); i++ )
	{
		pError = semanticErrors[i];
    Feature * pFeature = pError->getFeature();
    objIdLoc = pFeature->getGmlId();

		if ( objIdLoc == objId )
			vSemanticErrors.push_back ( pError );
	}

	return vSemanticErrors.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Error                                                                    //
//  Abstrakte Oberklasse für alle Fehler-Typen                               //
///////////////////////////////////////////////////////////////////////////////
Error::Error ( ERROR_CLASSIFICATION classification, std::string name )
{
  errorClassification = classification;
  errorName           = name;
}

Error::Error ( Error * pErrorOld )
{
  errorClassification = pErrorOld->errorClassification;
  errorName           = pErrorOld->errorName;
}

Error::~Error()
{

}
///////////////////////////////////////////////////////////////////////////////
//  Transformiert die Fehlerklassifikation: Enumeration --> Text             //
///////////////////////////////////////////////////////////////////////////////
std::string Error::getErrorClassificationAsString ( ERROR_CLASSIFICATION errorClassification )
{
  switch ( errorClassification )
  {
  case OK:
    return "Kein Fehler";

  case INFORMATION:
    return "Information";

  case WARNING:
    return "Warnung";

  case ERROR:
    return "Fehler";
  }
  
  return "";
}


///////////////////////////////////////////////////////////////////////////////
//  SemanticError                                                            //
//  Beschreibung eines einzelnen Semantik-Fehlers					              		 //
///////////////////////////////////////////////////////////////////////////////
SemanticError::SemanticError( Feature * pFeatureP, ERROR_CLASSIFICATION classification, string errorNameP ) : Error ( classification, errorNameP )
{
  pFeature         = pFeatureP;
  pRelatedFeature  = NULL;
  relatedFeatureId = "";
  errorDescription = "";

	pBrokenRule            = NULL;
	pAttributes            = NULL;
}

SemanticError::SemanticError ( SemanticError * pErrorOld )
 : Error ( pErrorOld )
{
  pFeature                = pErrorOld->pFeature;
  pRelatedFeature         = pErrorOld->pRelatedFeature;
  errorNumber             = pErrorOld->errorNumber;
  errorId                 = pErrorOld->errorId;
  errorDescription        = pErrorOld->errorDescription;
  pBrokenRule             = pErrorOld->pBrokenRule;

  if ( pErrorOld->pAttributes != NULL )
    pAttributes             = new NamesValuesList ( pErrorOld->pAttributes );
  else
    pAttributes           = NULL;

  relatedFeatureId         = pErrorOld->relatedFeatureId;
  relationName            = pErrorOld->relationName;
}


SemanticError::~ SemanticError()
{
	if ( pAttributes != NULL )             delete pAttributes;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das Objekt der verletzten Konformitätsregel                        //
///////////////////////////////////////////////////////////////////////////////
void SemanticError::setBrokenRule ( ProfileRule * pRule )
{  
  pBrokenRule = pRule;  
  errorId = pBrokenRule->getRuleId();
} 

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Id des zugehörigen Features                                  //
///////////////////////////////////////////////////////////////////////////////
std::string SemanticError::getFeatureId ()
{
  if ( pFeature != NULL )
    return pFeature->getGmlId();
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Geometrietyp des zugehörigen Features als Enumeration        //
///////////////////////////////////////////////////////////////////////////////
GEOMETRY_TYPE SemanticError::getFeatureGeometryType ()
{
  if ( pFeature == NULL )
    return NO_GEOMETRY;
  else
    return pFeature->getGeometrieTyp();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Geometrietyp des zugehörigen Features als Text               //
///////////////////////////////////////////////////////////////////////////////
std::string SemanticError::getFeatureGeometryTypeAsString ()
{

  GEOMETRY_TYPE geoType = getFeatureGeometryType();
  return _Geometrie::convertTypeToString ( geoType );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Klassennamen des zugehörigen Features                        //
///////////////////////////////////////////////////////////////////////////////
std::string SemanticError::getFeatureType ()
{
  if ( pFeature != NULL )
    return pFeature->getKlassenName();
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Id des verbundenen Features                                  //
///////////////////////////////////////////////////////////////////////////////
std::string SemanticError::getRelatedFeatureId ()
{
  if ( pRelatedFeature != NULL )
    return pRelatedFeature->getGmlId();
  else
    return relatedFeatureId;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Klassennamen des verbundenen Features                        //
///////////////////////////////////////////////////////////////////////////////
std::string SemanticError::getRelatedFeatureType ()
{
  if ( pRelatedFeature != NULL )
    return pRelatedFeature->getKlassenName();
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Vermerkt die für die Regel pBrokenRule relevanten Attribute		      		 //
///////////////////////////////////////////////////////////////////////////////
void SemanticError::setAttributes ( Feature * pObj )
{
	string               klassenName = pObj->getKlassenName();
	string               attributeName;
	string			         attributWert;
	string			         schluesselText;
	string               relationName;
	string			         enumerationName;
	string			         relationZielTyp; 
	string			         objId;
  GmlAttribut        * pGmlAttribut;
  AttributeFilter    * pAttributeFilter;
	size_t               i, j, anz, anzWerte;
  bool			           success;
  GmlDictionaryEntry * pDictionaryEntry;

  Features       * pFeatures   = pObj->getFeatures();
  GmlSchema      * pGmlObjekte = pFeatures->getGmlSchema();
	GmlKlasse      * pGmlKlasse  = pGmlObjekte->getGmlKlasseFromElementName( klassenName );
	GmlDictionary  * pCodeLists  = pGmlObjekte->getCodeListsDictionary();

	vector<AttributeFilter*> attributeFilters;
	vector<RelationFilter*>  relationFilters;
	vector<string>			     attributWerte;
  vector<string>           vCodeSpaces;
	vector<FeatureRelation*> objRelationen;
  vector<UOM*>             vUOMs;

	if ( pBrokenRule == NULL ) return;

  if ( pAttributes == NULL )
	  pAttributes = new NamesValuesList;

	if ( pBrokenRule->getIfFilter() != NULL )
	{
		anz = pBrokenRule->getIfFilter()->getAttributeFilters ( attributeFilters );
		for ( i = 0; i < anz; i++ )
		{
			pAttributeFilter = attributeFilters[i];
			attributeName = pAttributeFilter->getAttributeName();
			pGmlAttribut  = pGmlKlasse->getGmlAttribut ( attributeName );
			if ( pGmlAttribut != NULL )
			{
				attributWerte.clear();
        vCodeSpaces.clear();
        vUOMs.clear();

				anzWerte = pObj->getAttributWerte ( pGmlAttribut->getGmlAttributTyp(), attributeName, attributWerte, vCodeSpaces, vUOMs );
				for ( j = 0; j < anzWerte; j++ )
				{
					attributWert = attributWerte[j];
					if (  pGmlAttribut->getGmlAttributTyp()== _ENUMERATION || 
                pGmlAttribut->getGmlAttributTyp()== _EXTERNAL_CODE_LIST )
					{
						enumerationName = pGmlAttribut->getGmlEnumeration();
						pDictionaryEntry = pCodeLists->getDictionaryEntry ( enumerationName );
						if ( pDictionaryEntry != NULL )
						   success = pDictionaryEntry->getSchluesselText ( attributWert, schluesselText );
						else
							success = false;

						if ( success )
							attributWert = schluesselText + " (" + attributWert + ")";
					}
					
          pAttributes->addNameValue ( attributeName, attributWert );
				}
        if ( anzWerte == 0 )
          pAttributes->addNameValue( attributeName, "" );
			}
		}	
	}

	if ( pBrokenRule->getRequiredFilter() != NULL )
	{
		attributeFilters.clear();

		anz = pBrokenRule->getRequiredFilter()->getAttributeFilters ( attributeFilters );
		for ( i = 0; i < anz; i++ )
		{
			pAttributeFilter = attributeFilters[i];
			attributeName = pAttributeFilter->getAttributeName();
			pGmlAttribut  = pGmlKlasse->getGmlAttribut ( attributeName );
			if ( pGmlAttribut != NULL )
			{
				attributWerte.clear();
        vCodeSpaces.clear();
        vUOMs.clear();
				anzWerte = pObj->getAttributWerte ( pGmlAttribut->getGmlAttributTyp(), attributeName, attributWerte, vCodeSpaces, vUOMs );
				for ( j = 0; j < anzWerte; j++ )
				{
					attributWert = attributWerte[j];

					if (  pGmlAttribut->getGmlAttributTyp()== _ENUMERATION || 
                pGmlAttribut->getGmlAttributTyp()== _EXTERNAL_CODE_LIST  )
					{
						enumerationName = pGmlAttribut->getGmlEnumeration();
						pDictionaryEntry = pCodeLists->getDictionaryEntry ( enumerationName );
						if ( pDictionaryEntry != NULL )
						   success = pDictionaryEntry->getSchluesselText ( attributWert, schluesselText );
						else
							success = false;

						if ( success )
							attributWert = schluesselText + " (" + attributWert + ")";
					}

					pAttributes->addNameValue ( attributeName, attributWert );
				}
        if ( anzWerte == 0 )
          pAttributes->addNameValue( attributeName, "" );
			}
		}

		if ( pAttributes->getNamesValuesAnz() == 0 )
		{
			delete pAttributes;
			pAttributes = NULL;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein Paar Attribut-Name - Attribut-Wert                             //
///////////////////////////////////////////////////////////////////////////////
void SemanticError::setAttribute ( std::string attributName, std::string attributWert )
{
  if ( pAttributes == NULL )
    pAttributes = new NamesValuesList;

  pAttributes->addNameValue ( attributName, attributWert );
}


///////////////////////////////////////////////////////////////////////////////
//  Generiert die Fehlerbeschreibung für einen Enumerations-Fehler           //
///////////////////////////////////////////////////////////////////////////////
void SemanticError::setEnumError ( std::string codeListName, std::string attributeName, std::string attributeValue )
{
  errorDescription = "Codelist-Name:" + codeListName + "\r\n";
  errorDescription = errorDescription + "Attributname: " + attributeName   + "\r\n";
  errorDescription = errorDescription + "Attibutwert : " + attributeValue  + "\r\n";

}


///////////////////////////////////////////////////////////////////////////////
//  AggregatedSemanticError                                                  //
//  Beschreibung einer Aggregation von Semantikfehlern                       //
///////////////////////////////////////////////////////////////////////////////
AggregatedSemanticError::AggregatedSemanticError ( Feature * pObj,  ERROR_CLASSIFICATION classification, std::string errorNameP )
  : SemanticError ( pObj, classification, errorNameP )
{

}

AggregatedSemanticError::~AggregatedSemanticError()
{
  for ( unsigned int i = 0; i < vErrors.size(); i++ )
    delete vErrors[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert einen Text, der alle Fehler-Bezeichnungen aggregiert           //
///////////////////////////////////////////////////////////////////////////////
void AggregatedSemanticError::generateOverallError()
{
  if ( errorName != " ")
    errorName = errorName + "\r\n";

  for ( unsigned int i = 0; i < vErrors.size(); i++ )
    errorName = errorName + vErrors[i]->getErrorName() + "\r\n";
}

///////////////////////////////////////////////////////////////////////////////
//  GeometryError                                                            //
//  Beschreibung eines einzelnen Geometriefehlers                            //
///////////////////////////////////////////////////////////////////////////////
GeometryError::GeometryError( Feature * pObj,  ERROR_CLASSIFICATION classification, GEOMETRY_ERROR_TYPE errorTypeP )
 :Error ( classification, getGeometryErrorTypeAsString ( errorTypeP ) )
{
  errorType           = errorTypeP;
  pFeature            = pObj;
  pRelatedFeature     = NULL;
  errorDescription    = "";
  geometryId          = "";
  relatedGeometryId   = "";
  propertyName        = "";
  relatedPropertyName = "";

  flaecheObj          = 0.0;
  flaecheObjRelated   = 0.0;
  flaecheUeberlappung = 0.0;

  planaritaetsfehlerMaximal      = 0.0;
  planaritaetsfehlerDurchschnitt = 0.0;

  index1 = 0;
  index2 = 0;

  errorGeometryCreated = false;
}

GeometryError::GeometryError ( GeometryError * pErrorOld ): Error ( pErrorOld )
{
  errorNumber             = pErrorOld->errorNumber;
  errorDescription        = pErrorOld->errorDescription;
  pFeature                = pErrorOld->pFeature;
  pRelatedFeature         = pErrorOld->pRelatedFeature;
  propertyName            = pErrorOld->propertyName;
  relatedPropertyName     = pErrorOld->relatedPropertyName;

  geometryId              = pErrorOld->geometryId;
  relatedGeometryId       = pErrorOld->relatedGeometryId;

  flaecheObj          = pErrorOld->flaecheObj;
  flaecheObjRelated   = pErrorOld->flaecheObjRelated;
  flaecheUeberlappung = pErrorOld->flaecheUeberlappung;

  planaritaetsfehlerMaximal      = pErrorOld->planaritaetsfehlerMaximal;
  planaritaetsfehlerDurchschnitt = pErrorOld->planaritaetsfehlerDurchschnitt;

  errorGeometryCreated = pErrorOld->errorGeometryCreated;
}

GeometryError::~GeometryError()
{
  for ( unsigned int i = 0; i < vErrorGeometry.size(); i++  )
    delete vErrorGeometry[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Id des zugehörigen Features                                  //
///////////////////////////////////////////////////////////////////////////////
std::string GeometryError::getFeatureId ()
{
  if ( pFeature != NULL )
    return pFeature->getGmlId();
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Klassennamen des zugehörigen Features                        //
///////////////////////////////////////////////////////////////////////////////
std::string GeometryError::getFeatureType ()
{
  if ( pFeature != NULL )
    return pFeature->getKlassenName();
  else
    return "";
}

//////////////////////////////////////////////////////////////////////////////////
//  Liefert den Typ des fehlerhaften Geometrieobjektes (Enumeration)  im Feature//
//////////////////////////////////////////////////////////////////////////////////
GEOMETRY_TYPE GeometryError::getFeatureGeometryType ()
{
  if ( geometryId == "" || pFeature == NULL )
    return NO_GEOMETRY;

  _Geometrie * pGeo = pFeature->getFeatures()->getGeometry( geometryId );
  if ( pGeo != NULL )
    return pGeo->getGeometryType();
  else
    return NO_GEOMETRY;
}

//////////////////////////////////////////////////////////////////////////////
//  Liefert den Typ des fehlerhaften Geometrieobjektes (Text)  im Feature   //
//////////////////////////////////////////////////////////////////////////////
std::string GeometryError::getFeatureGeometryTypeAsString ()
{
  return _Geometrie::convertTypeToString( getFeatureGeometryType() );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Id des verbundenen Features                                  //
///////////////////////////////////////////////////////////////////////////////
std::string GeometryError::getRelatedFeatureId ()
{
  if ( pRelatedFeature != NULL )
    return pRelatedFeature->getGmlId();
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Klassennamen des verbundenen Features                        //
///////////////////////////////////////////////////////////////////////////////
std::string GeometryError::getRelatedFeatureType ()
{
  if ( pRelatedFeature != NULL )
    return pRelatedFeature->getKlassenName();
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Typ des fehlerhaften Geometrieobjektes (Enumeration)  im     // 
//  verbundenen Feature                                                      //
///////////////////////////////////////////////////////////////////////////////
GEOMETRY_TYPE GeometryError::getRelatedFeatureGeometryType()
{
  if ( relatedGeometryId == "" || pRelatedFeature == NULL )
    return NO_GEOMETRY;

  _Geometrie * pGeo = pRelatedFeature->getFeatures()->getGeometry( relatedGeometryId );
  if ( pGeo != NULL )
    return pGeo->getGeometryType();
  else
    return NO_GEOMETRY;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Typ des fehlerhaften Geometrieobjektes (Text)  im            // 
//  verbundenen Feature                                                      //
///////////////////////////////////////////////////////////////////////////////
std::string GeometryError::getRelatedFeatureGeometryTypeAsString()
{
  return _Geometrie::convertTypeToString( getFeatureGeometryType() );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt  die Angaben zur Überlappungsfläche von Flächenschlussobjekten     //
///////////////////////////////////////////////////////////////////////////////
void GeometryError::setFlaechenangaben ( double flaecheObjP,  double flaecheObjRelatedP, double flaecheUeberlappungP )
{
  flaecheObj          = flaecheObjP;
  flaecheObjRelated   = flaecheObjRelatedP;
  flaecheUeberlappung = flaecheUeberlappungP;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Angaben zur Überlappungsfläche von Flächenschlussobjekten    //
///////////////////////////////////////////////////////////////////////////////
void GeometryError::getFlaechenangaben ( double &flaecheObjP, double &flaecheObjRelatedP, 
                                           double &flaecheUeberlappungP )
{
  flaecheObjP          = flaecheObj;
  flaecheObjRelatedP   = flaecheObjRelated;
  flaecheUeberlappungP = flaecheUeberlappung;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt Angaben zu Planaritätsfehlern                                      //
///////////////////////////////////////////////////////////////////////////////
void GeometryError::setPlanaritaetsfehler ( double distMax, double distAvarage )
{
  planaritaetsfehlerMaximal      = distMax;
  planaritaetsfehlerDurchschnitt = distAvarage;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Angaben zu Planaritätsfehlern                                    //
///////////////////////////////////////////////////////////////////////////////
void GeometryError::getPlanaritaetsfehler ( double &distMax, double &distAvarage )
{
  distMax     = planaritaetsfehlerMaximal;
  distAvarage = planaritaetsfehlerDurchschnitt;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Fehlertyp in Textform                                        //
///////////////////////////////////////////////////////////////////////////////
std::string GeometryError::getGeometryErrorTypeAsString( int errorTypeP )
{
  switch ( errorTypeP )
  {
  case NO_GEOMETRY_SPECIFIED:
    return "Keine Geometrie vorhanden";

  case RING_NOT_ENOUGH_POINTS:
    return "Ein Ring hat weniger als 3 Stützpunkte";

    case RING_NOT_CLOSED:
      return "Erster und letzter Stützpunkt eines Ringes stimmen nicht überein";

    case RING_DOUBLE_POINTS:
      return "Ein Ring enthält doppelte Stützpunkte";

    case RING_SELF_OVERLAP:
      return "Selbstüberschneidung eines Polygon-Ringes";

    case POLYGON_VOID_ERROR:
      return "Fehlerhafte Innenkonturen eines Polygons";

    case POLYGON_NON_PLANAR:
      return "Nicht-planares Polygon";

    case POLYGON__VOID_ORIENTATION_ERROR:
      return "Innenkontur eines Polygons hat eine falsche Orientierung";

    case POLYGON_VOID_PLANARITY_ERROR:
      return "Innenkontur eines Polygons liegt nicht in der Ebene der Außenkontur";

    case SURFACE_OVERLAP_ERROR:
      return "Verbotene Überlappung von Einzel-Polygonen einer Multi-Fläche";

    case POLYGON_WRONG_ORIENTATION:
      return "Die Außenkontur eines ebenen Polygons hat eine falsche Orientierung";

    case SURFACE_TOTAL_COVERAGE_ERROR:
      return "Fehler bei der vollständigen Überdeckung des Geltungsbereiches durch  Flächenschlussobjekte";

    case RING_ZERO_AREA:
      return "Polygonfläche hat keinen Flächeninhalt";

    case SOLID_EULER_CHECK_FAILED:
      return "Ein Solid erfüllt die Euler-Bedingung nicht";

    case SOLID_EDGE_CHECK_FAILED_MISSING_COUNTERPART:
      return "Solid-Kanten werden nur einmal durchlaufen";

    case SOLID_EDGE_CHECK_FAILED_DOUBLE_EDGE:
      return "Solid-Kanten werden zweimal in derselben Richtung durchlaufen";

    case SOLID_EDGE_CHECK_FAILED_MULTIPLE_EDGE:
      return "Solid-Kanten werden mehrfach und unterschiedlich häufig hin und zurück durchlaufen";

    case SOLID_EDGE_CHECK_INFO_BACK_FORTH_EDGE:
      return "Solid-Kanten werden mehrfach hin und zurück durchlaufen";

    case MULTISURFACE_IRREGULAR_POINT_TOUCH:
      return "Flächenteile einer Multi-Surface berühren sich außerhalb von Stützpunkten";

    case MULTISURFACE_IRREGULAR_LINE_TOUCH_INTERSECT:
      return "Flächenteile einer Multi-Surface berühren oder durchdringen sich irregulär";

    case MULTISURFACE_IRREGULAR_SURFACE_OVERLAP:
      return "Flächenteile einer Multi-Surface überlappen sich irregulär";

    case CITYGML_BOUNDARY_SURFACES_NO_SOLID:
      return "Die CityGML Boundary Surfaces bilden keinen Solid";

    case CITYGML_MISSING_ROOF_SURFACE:
      return "CityGML Modell ohne RoofSurface";

    case CITYGML_MISSING_WALL_SURFACE:
      return "CityGML Modell ohne WallSurface";

    case CITYGML_MISSING_GROUND_SURFACE:
      return "CityGML Modell ohne GroundSurface";
    case XPLANGML_FLAECHENSCHLUSS_OVERLAPP:
      return "Überlappende Flächenschlussobjekte";
  }

  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Fehlertyp in Textform                                        //
///////////////////////////////////////////////////////////////////////////////
std::string GeometryError::getGeometryErrorTypeAsString()
{
  return  getGeometryErrorTypeAsString( errorType );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Indices für die Anzeige einer Selbst-Überlappung eines Ringes  //
///////////////////////////////////////////////////////////////////////////////
void GeometryError::setSelfOverlapIndeces ( size_t  index1P, size_t  index2P )
{
  index1 = index1P;
  index2 = index2P;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Geometrieobjekt zur Fehlerbeschreibung hinzu                    //
///////////////////////////////////////////////////////////////////////////////
void GeometryError::addErrorGeometry ( _Geometrie * pGeo, std::string desc )
{
  vErrorGeometry.push_back( pGeo );
  vErrorGeometryDescription.push_back( desc );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das unter index referierte Fehlerobjekt                          //
///////////////////////////////////////////////////////////////////////////////
_Geometrie * GeometryError::getErrorGeometry ( size_t index )
{
  if ( index < vErrorGeometry.size() )
    return vErrorGeometry[index];
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die unter index referierte Fehlerbeschreibung                    //
///////////////////////////////////////////////////////////////////////////////
std::string  GeometryError::getErrorGeometryDescription ( size_t index )
{
  if ( index < vErrorGeometryDescription.size() )
    return vErrorGeometryDescription[index];
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Vermerkt, dass die Information aus vErrorGeometry verarbeitet worden ist //
///////////////////////////////////////////////////////////////////////////////
void GeometryError::setErrorGeometryCreated()
{
  errorGeometryCreated = true;
  vErrorGeometry.clear();
  vErrorGeometryDescription.clear();
}


///////////////////////////////////////////////////////////////////////////////
//  NamesValuesList                                                          //
//  Liste von Attribut-Name / Attribut-Wert Paaren			            				 //
///////////////////////////////////////////////////////////////////////////////
NamesValuesList::NamesValuesList ()
{

}

NamesValuesList::NamesValuesList ( NamesValuesList * pListOld )
{
  for ( unsigned int i = 0; i < pListOld->names.size(); i++ )
  {
    names.push_back( pListOld->names[i] );
    values.push_back( pListOld->values[i] );
  }
}


NamesValuesList::~NamesValuesList()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Paar Name - Wert hinzu                                          //
///////////////////////////////////////////////////////////////////////////////
void NamesValuesList::addNameValue ( std::string name, std::string value )
{
	names.push_back ( name );
	values.push_back ( value );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das unter index referierte Paar Name - Wert                      //
///////////////////////////////////////////////////////////////////////////////
bool NamesValuesList::getNameValue ( size_t index, std::string & name, std::string & value )
{
	if ( index < 0 || index >= names.size() ) return false;

	name  = names[index];
	value = values[index];
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl aller Paare                                           //
///////////////////////////////////////////////////////////////////////////////
size_t NamesValuesList::getNamesValuesAnz()
{
	return names.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Namen, durch "," getrennt, in einem Text                    //
///////////////////////////////////////////////////////////////////////////////
string NamesValuesList::getAllNames()
{
  string str = "";

  for ( unsigned int i = 0; i < names.size(); i++ )
  {
    str = str + names[i];
    if ( i < names.size()-1 )
      str = str + ", ";
  }
  return str;
}

///////////////////////////////////////////////////////////////////////////////
//  ValidationError                                                          // 
//  XML-Schema Validierungsfehler                                            //
///////////////////////////////////////////////////////////////////////////////
ValidationError::ValidationError( ERROR_CLASSIFICATION classification, std::string errorTextP, int lineNumberP )
  :Error ( classification, "Validierungsfehler" )
{
  errorText = errorTextP;
  lineNumber = lineNumberP;
}

ValidationError::~ValidationError()
{

}

///////////////////////////////////////////////////////////////////////////////
//  ReadError                                                                //
//  Beim Einlesen einer XML-Datei aufgetretener Fehler                       //
///////////////////////////////////////////////////////////////////////////////
ReadError::ReadError( ERROR_CLASSIFICATION classification, std::string errorTextP, int lineNumberP )
  :Error ( classification, "Fehler beim Einlesen einer GML-Datei" )
{
  errorText = errorTextP;
  lineNumber = lineNumberP;
}

ReadError::~ReadError()
{

}


///////////////////////////////////////////////////////////////////////////////
//  ProfileReader                                                            //
//  Einlesen einer Datei mit Konformitätsbedingungen                         //
///////////////////////////////////////////////////////////////////////////////
ProfileReader::ProfileReader( SemanticRules * pProfileP )
{
	pProfile = pProfileP;
}

ProfileReader:: ~ ProfileReader()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Datei fileName                                              //     
///////////////////////////////////////////////////////////////////////////////
bool ProfileReader::read ( std::string fileName )
{
	String ^ STR;
	string   stdString;
  string   id;
	bool	   success = true;
	bool     first   = true; 

	String ^ fName = gcnew String( fileName.c_str() );
	validReader      = gcnew XmlTextReader( fName );

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( first == true )
			{
				first = false;
				if ( pActElement->CompareTo("Profil") != 0)
				{
					MessageBox::Show( "Ungültige Profil-Datei");
					success = false;
					break;
				}

				STR = validReader->GetAttribute ( "name" );
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pProfile->setProfilName ( stdString );
				}
			}

			if ( pActElement->CompareTo("SrsName") == 0 )
			{
				STR = validReader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pProfile->addSrsName ( stdString );
				}
			}
			else
      if ( pActElement->CompareTo("UomLength") == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pProfile->addUomLength( stdString );
        }
      }
      else
      if ( pActElement->CompareTo("UomArea") == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pProfile->addUomArea( stdString );
        }
      }
      else
      if ( pActElement->CompareTo("UomVolume") == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pProfile->addUomVolume( stdString );
        }
      }
      else
      if ( pActElement->CompareTo("UomAngle") == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pProfile->addUomAngle( stdString );
        }
      }
      else
      if ( pActElement->CompareTo("UomMeasure") == 0 )
      {
        STR = validReader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pProfile->addUomMeasure( stdString );
        }
      }
      else       
			if (pActElement->CompareTo("Rule") == 0 )
			{
				success = readRule();
				if ( !success ) break;
			}
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo("Profil") == 0 )
				break;
		}

	}
	validReader->Close();

	return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer Regel			                          										 //
///////////////////////////////////////////////////////////////////////////////
bool ProfileReader::readRule()
{
	String      ^ STR;
	string        stdString;
	ProfileRule * pRule;

	pRule = new ProfileRule ();

	STR = validReader->GetAttribute ( "class" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceKuerzel ( stdString );
		pRule->setGmlClassName ( stdString );
	}

	STR = validReader->GetAttribute ( "superclassType" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceKuerzel ( stdString );
		pRule->setGmlSuperclassTypeName ( stdString );
	}

  STR = validReader->GetAttribute ( "id" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pRule->setRuleId( stdString );
  }


	pProfile->addProfileRule ( pRule );

	if ( validReader->IsEmptyElement )
		return true;

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

      if ( pActElement->CompareTo( "description" ) == 0 )
      {
        STR = validReader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pRule->setDescription (stdString );
      }
      else   
			if ( pActElement->CompareTo("If") == 0 )
      {
        STR = validReader->GetAttribute( "combination" );        
				readProfileFilter ( pRule, "If", STR );
      }
			else
			if ( pActElement->CompareTo("Required") == 0 )
      {
        STR = validReader->GetAttribute( "combination" );        
				readProfileFilter ( pRule, "Required", STR );
      }
		}
		else
		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo("Rule") == 0 ) break;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen ProfileFilter			                        										 //
///////////////////////////////////////////////////////////////////////////////
bool ProfileReader::readProfileFilter ( ProfileRule * pRule, std::string ruleType, String ^ combination )
{
	String                     ^ STR;
	String                     ^ RULE_TYPE;
	string                       stdString;
	int				                   n;
  double                       d;
	AttributeFilter            * pAttributeFilter = nullptr;
  AttributeCardinalityFilter * pAttributeCardinalityFilter;
	RelationFilter             * pRelationFilter = nullptr;
	GroupFilter                * pGroupFilter;
	GeometryFilter             * pGeometryFilter;

 
	RULE_TYPE = QuConvert::ToString ( ruleType );

	while( validReader->Read() )
	{
		if ( validReader->NodeType == XmlNodeType::Element )
		{
			pActElement = validReader->LocalName;

			if ( pActElement->CompareTo("AttributeFilter") == 0 ) 
			{
				pAttributeFilter = new AttributeFilter ( pRule );
				if ( ruleType == "If" ) 
					pRule->addAttributeFilter ( pAttributeFilter, ProfileRule::IF_FILTER, combination );
				else
				if ( ruleType == "Required" ) 
					pRule->addAttributeFilter ( pAttributeFilter, ProfileRule::REQUIRED_FILTER, combination );

				STR = validReader->GetAttribute ( "attributeName" );
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addDefaultNamespaceKuerzel ( stdString );
					pAttributeFilter->setAttributeName ( stdString );
				}

				STR = validReader->GetAttribute ( "minOccurs" );
				if ( STR != nullptr )
				{
					n = XmlConvert::ToInt32 ( STR );
					pAttributeFilter->setMinOccurs ( n );
				}

				STR = validReader->GetAttribute ( "maxOccurs" );
				if ( STR != nullptr )
				{
					if ( STR->CompareTo("unbound") == 0 )
						pAttributeFilter->setMaxOccurs ( INT_MAX );
					else
					{
						n = XmlConvert::ToInt32 ( STR );
						pAttributeFilter->setMaxOccurs ( n );
					}
        }

        STR = validReader->GetAttribute ( "location" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAttributeFilter->setLocation ( stdString );
        }


			}
			else
			if ( pActElement->CompareTo("AttributeValue") == 0 ) 
			{
				ATTRIBUTE_VALUE_OPERATOR op;
				op = EQ;

				STR = validReader->GetAttribute ( "op" );
				if ( STR != nullptr )
				{
					if ( STR->CompareTo("NOT_EQ") == 0 )
						op = NOT_EQ;
				}

				STR = validReader->ReadString ();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pAttributeFilter->addAttributeValue ( stdString, op );
				}
			}
      else
      if ( pActElement->CompareTo("NumericValue") == 0 ) 
      {
        ATTRIBUTE_VALUE_OPERATOR op;
        op = EQ;

        STR = validReader->GetAttribute ( "op" );
        if ( STR != nullptr )
        {
          if ( STR->CompareTo("NOT_EQ") == 0 ) op = NOT_EQ;
          else if ( STR->CompareTo("LT") == 0 ) op = LT;
          else if ( STR->CompareTo("LE") == 0 ) op = LE;
          else if ( STR->CompareTo("GT") == 0 ) op = GT;
          else if ( STR->CompareTo("GE") == 0 ) op = GE;
        }

        STR = validReader->ReadString ();
        if ( STR != nullptr )
        {
          d = XmlConvert::ToDouble( STR );
          pAttributeFilter->addNumericValue ( d, op );
        }
      }
      else
      if ( pActElement->CompareTo("AttributeCardinalityFilter") == 0 ) 
      {
        pAttributeCardinalityFilter = new AttributeCardinalityFilter ( pRule );
        if ( ruleType == "If" ) 
          pRule->addAttributeCardinalityFilter ( pAttributeCardinalityFilter, ProfileRule::IF_FILTER, combination );
        else
        if ( ruleType == "Required" ) 
          pRule->addAttributeCardinalityFilter ( pAttributeCardinalityFilter, ProfileRule::REQUIRED_FILTER, combination );
        
        STR = validReader->GetAttribute ( "attributeName1" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addDefaultNamespaceKuerzel ( stdString );
          pAttributeCardinalityFilter->setAttributnameA ( stdString );
        }

        STR = validReader->GetAttribute ( "attributeName2" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addDefaultNamespaceKuerzel ( stdString );
          pAttributeCardinalityFilter->setAttributnameB ( stdString );
        }

        STR = validReader->GetAttribute ( "compareRel" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pAttributeCardinalityFilter->setCompareOperator ( stdString );
        }
      }
			else
			if ( pActElement->CompareTo("RelationFilter") == 0 ) 
			{
				pRelationFilter = new RelationFilter ( pRule );
				if ( ruleType == "If" ) 
					pRule->addRelationFilter ( pRelationFilter, ProfileRule::IF_FILTER, combination );
				else
				if ( ruleType == "Required" ) 
					pRule->addRelationFilter ( pRelationFilter, ProfileRule::REQUIRED_FILTER, combination );

				STR = validReader->GetAttribute ( "relationName" );
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addDefaultNamespaceKuerzel ( stdString );
					pRelationFilter->setRelationName ( stdString );
				}

        STR = validReader->GetAttribute ( "attributNameInRelatedObject" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addDefaultNamespaceKuerzel ( stdString );
          pRelationFilter->setAttributNameInRelatedObject ( stdString );
        }

        STR = validReader->GetAttribute ( "attributeValueInRelatedObject" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pRelationFilter->setRequiredAttributWertInRelatedObject ( stdString );
        }

        STR = validReader->GetAttribute ( "relationNameInRelatedObject" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addDefaultNamespaceKuerzel ( stdString );
          pRelationFilter->setRelationNameInRelatedObject ( stdString );
        }

        STR = validReader->GetAttribute ( "geometryPropertyNameInRelatedObject" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addDefaultNamespaceKuerzel ( stdString );
          pRelationFilter->setGeometryPropertyNameInRelatedObject ( stdString );
        }

				STR = validReader->GetAttribute ( "minOccurs" );
				if ( STR != nullptr )
				{
					n = XmlConvert::ToInt32 ( STR );
					pRelationFilter->setMinOccurs ( n );
				}

				STR = validReader->GetAttribute ( "maxOccurs" );
				if ( STR != nullptr )
				{
					if ( STR->CompareTo("unbound") == 0 )
						pRelationFilter->setMaxOccurs ( INT_MAX );
					else
					{
						n = XmlConvert::ToInt32 ( STR );
						pRelationFilter->setMaxOccurs ( n );
					}
				}

        STR = validReader->GetAttribute ( "minOccursProperty" );
        if ( STR != nullptr )
        {
          n = XmlConvert::ToInt32 ( STR );
          pRelationFilter->setMinOccursProperty ( n );
        }

        STR = validReader->GetAttribute ( "maxOccursProperty" );
        if ( STR != nullptr )
        {
          if ( STR->CompareTo("unbound") == 0 )
            pRelationFilter->setMaxOccursProperty ( INT_MAX );
          else
          {
            n = XmlConvert::ToInt32 ( STR );
            pRelationFilter->setMaxOccursProperty ( n );
          }
        }

        STR = validReader->GetAttribute ( "inverseRelationName" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addDefaultNamespaceKuerzel ( stdString );
          pRelationFilter->setInverseRelationName( stdString );
        }

			}
			else
			if ( pActElement->CompareTo("GmlType") == 0 ) 
			{
				STR = validReader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addDefaultNamespaceKuerzel ( stdString );
					pRelationFilter->addGmlTypeName ( stdString );
				}
			}
			else
			if ( pActElement->CompareTo("GmlSuperType") == 0 ) 
			{
				STR = validReader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addDefaultNamespaceKuerzel ( stdString );
					pRelationFilter->addGmlSuperTypeName ( stdString );
				}
			}
      else
      if ( pActElement->CompareTo("AttributeValueRelatedObject") == 0 ) 
      {
        ATTRIBUTE_VALUE_OPERATOR op;
        op = EQ;

        STR = validReader->GetAttribute ( "op" );
        if ( STR != nullptr )
        {
          if ( STR->CompareTo("NOT_EQ") == 0 )
            op = NOT_EQ;
        }

        STR = validReader->ReadString ();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pRelationFilter->addAttributWertInRelatedObject ( stdString, op );
        }       
      }
      else
      if ( pActElement->CompareTo("NumericValueRelatedObject") == 0 ) 
      {
        ATTRIBUTE_VALUE_OPERATOR op;
        op = EQ;

        STR = validReader->GetAttribute ( "op" );
        if ( STR != nullptr )
        {

          if ( STR->CompareTo("NOT_EQ") == 0 ) op = NOT_EQ;
          else if ( STR->CompareTo("LT") == 0 ) op = LT;
          else if ( STR->CompareTo("LE") == 0 ) op = LE;
          else if ( STR->CompareTo("GT") == 0 ) op = GT;
          else if ( STR->CompareTo("GE") == 0 ) op = GE;
        }

        STR = validReader->ReadString ();
        if ( STR != nullptr )
        {
          d = XmlConvert::ToDouble( STR );
          pRelationFilter->addNumericWertInRelatedObject ( d, op );
        }       
      }
			else
			if ( pActElement->CompareTo("GroupFilter") == 0 ) 
			{
				pGroupFilter = new GroupFilter ( pRule );
				if ( ruleType == "If" ) 
					pRule->addGroupFilter ( pGroupFilter, ProfileRule::IF_FILTER, combination );
				else
				if ( ruleType == "Required" ) 
					pRule->addGroupFilter ( pGroupFilter, ProfileRule::REQUIRED_FILTER, combination );
       
        STR = validReader->GetAttribute ( "combination" );
        if ( STR->CompareTo("AND") == 0 )
          pGroupFilter->setCombination(  AND );
        else
        if ( STR->CompareTo("OR") == 0 )
          pGroupFilter->setCombination( OR );
        else
        if ( STR->CompareTo("XOR") == 0 )
          pGroupFilter->setCombination( XOR );
        else
        if ( STR->CompareTo("XOR_OPT") == 0 )
          pGroupFilter->setCombination( XOR_OPT );

				while( validReader->Read() )
				{
					if ( validReader->NodeType == XmlNodeType::Element )
					{
						pActElement = validReader->LocalName;

						if ( pActElement->CompareTo("AttributeName") == 0 ) 
						{
							STR = validReader->ReadString();
							if ( STR != nullptr )
							{
								QuConvert::systemStr2stdStr ( stdString, STR );
                stdString = addDefaultNamespaceKuerzel ( stdString );
								pGroupFilter->addAttributeName ( stdString );
							}
						}
						else
						if ( pActElement->CompareTo("RelationName") == 0 ) 
						{
							STR = validReader->ReadString();
							if ( STR != nullptr )
							{
								QuConvert::systemStr2stdStr ( stdString, STR );
                stdString = addDefaultNamespaceKuerzel ( stdString );
								pGroupFilter->addRelationName ( stdString );
							}
						}
						else
						if ( pActElement->CompareTo("GeometryPropertyName") == 0 ) 
						{
							STR = validReader->ReadString();
							if ( STR != nullptr )
							{
								QuConvert::systemStr2stdStr ( stdString, STR );
                stdString = addDefaultNamespaceKuerzel ( stdString );
								pGroupFilter->addGeometryPropertyName ( stdString );
							}
						}
					}
					else
					if ( validReader->NodeType == XmlNodeType::EndElement )
					{
						pActElement = validReader->LocalName;
						if ( pActElement->CompareTo("GroupFilter") == 0 ) 
							break;
					}
				}
			}
			else 
			if ( pActElement->CompareTo("GeometryFilter") == 0 ) 
			{
				pGeometryFilter = new GeometryFilter ( pRule );
				if ( ruleType == "If" ) 
					pRule->addGeometryFilter ( pGeometryFilter, ProfileRule::IF_FILTER, combination );
				else
				if ( ruleType == "Required" ) 
					pRule->addGeometryFilter ( pGeometryFilter, ProfileRule::REQUIRED_FILTER, combination );

				STR = validReader->GetAttribute ( "geometryPropertyName" );
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addDefaultNamespaceKuerzel ( stdString );
					pGeometryFilter->setGeometryPropertyName ( stdString );
				}

        STR = validReader->GetAttribute ( "minOccurs" );
        if ( STR != nullptr )
        {
          n = XmlConvert::ToInt32 ( STR );
          pGeometryFilter->setMinOccurs ( n );
        }

        STR = validReader->GetAttribute ( "maxOccurs" );
        if ( STR != nullptr )
        {
          if ( STR->CompareTo("unbound") == 0 )
            pGeometryFilter->setMaxOccurs ( INT_MAX );
          else
          {
            n = XmlConvert::ToInt32 ( STR );
            pGeometryFilter->setMaxOccurs ( n );
          }
        }

				STR = validReader->GetAttribute ( "geometryRestriction" );
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pGeometryFilter->setGeometryRestriction ( stdString );
				}


        STR = validReader->GetAttribute ( "surfaceNormal" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pGeometryFilter->setSurfaceNormal ( stdString );
        }
			}
		}

		if ( validReader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = validReader->LocalName;
			if ( pActElement->CompareTo( RULE_TYPE ) == 0 )
				break;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt vor GML Klassen- bzw. Attributname das Default Namespace-Kürzel ein //
///////////////////////////////////////////////////////////////////////////////
string ProfileReader::addDefaultNamespaceKuerzel ( string gmlName )
{
	size_t index;
  string gmlNameAct;

  index = gmlName.find ( ":" );
  if ( index != string::npos )
    return gmlName;

  if ( pProfile->getFeatures()->isXPlanGML() )
    gmlNameAct = "xplan:" + gmlName;
  else
    gmlNameAct = gmlName;

  return gmlNameAct;
}

///////////////////////////////////////////////////////////////////////////////
//  ErrorReportWriter                                                        //
//  Schreiben der festgestellten Konformitätsfehler	              					 //
///////////////////////////////////////////////////////////////////////////////
ErrorReportWriter::ErrorReportWriter ( ErrorReport * pErrorReportP )
{
	m_pErrorReport = pErrorReportP;
}

ErrorReportWriter::~ErrorReportWriter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben des Fehler-Reports auf die Datei fileName                      //
///////////////////////////////////////////////////////////////////////////////
void ErrorReportWriter::write ( std::string fileName )
{
	unsigned int      i;
  int               lineNumber;
	String          ^ FILE_NAME;
	string            stdString;
  string            errorText;

	System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();
	FILE_NAME						    = QuConvert::ToString ( fileName );
	writer								= gcnew XmlTextWriter( FILE_NAME, utf8 );

	writer->Formatting = Formatting::Indented;
	writer->WriteStartDocument( true );
	writer->WriteComment("Generated by: KIT XPlanGML Conformity Test Software");

  writer->WriteStartElement( "Test-Report" );
 	writer->WriteAttributeString("xmlns:xsi",	"http://www.w3.org/2001/XMLSchema-instance");

  if ( m_pErrorReport->getReadErrorAnz() > 0 )
  {
    writer->WriteStartElement("Gruppe_Einlese-Fehler");
    for (i = 0; i < m_pErrorReport->getReadErrorAnz(); i++)
    {

      ReadError * pReadError = m_pErrorReport->getReadError( i );
      lineNumber = pReadError->getLineNumber();
      errorText = pReadError->getErrorText();

      writer->WriteStartElement( "Einlesefehler" );
      writer->WriteAttributeString ( "Zeilen-Nummer", XmlConvert::ToString ( lineNumber ) );
      writer->WriteString ( QuConvert::ToString ( errorText ) );
      writer->WriteEndElement();
    }
    writer->WriteEndElement();
  }


	if ( m_pErrorReport->getValidationErrorAnz() > 0 )
	{
		writer->WriteStartElement ( "Gruppe_Validierungsfehler" );
		for ( i = 0; i < m_pErrorReport->getValidationErrorAnz(); i++ )
		{
			ValidationError * pValidationError = m_pErrorReport->getValidationError ( i );
      lineNumber = pValidationError->getLineNumber();
      errorText = pValidationError->getErrorText();

      writer->WriteStartElement ( "Validierungsfehler" );
      writer->WriteAttributeString ( "Zeilen-Nummer", XmlConvert::ToString ( lineNumber ) );
      writer->WriteString ( QuConvert::ToString ( errorText ) );
      writer->WriteEndElement();
    }
		writer->WriteEndElement();
	}

	if ( m_pErrorReport->getSemanticErrorAnz() > 0 )
	{
		writer->WriteStartElement ( "Gruppe_Konformitätsfehler" );
		for ( i = 0; i < m_pErrorReport->getSemanticErrorAnz(); i++ )
		{
			SemanticError * pError = m_pErrorReport->getSemanticError ( i );
			writer->WriteStartElement ( "Konformitätsfehler" );
		
			writeSemanticError ( pError );
		
			writer->WriteEndElement();
		}
		writer->WriteEndElement();
	}

  if (m_pErrorReport->getGeometryErrorAnz() > 0)
  {
    writer->WriteStartElement ( "Gruppe_Geometriefehler" );
    for (i = 0; i < m_pErrorReport->getGeometryErrorAnz(); i++)
    {
      GeometryError * pError = m_pErrorReport->getGeometryError ( i );
      writer->WriteStartElement ( "Geometriefehler" );

      writeGeometryError ( pError );

      writer->WriteEndElement();
    }
    writer->WriteEndElement();
  }


	writer->WriteEndElement();

	writer->Flush();
	writer->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines Semantik-Fehlers                                         //
///////////////////////////////////////////////////////////////////////////////
void ErrorReportWriter::writeSemanticError ( SemanticError * pError )
{
	string            stdString;
	ProfileRule     * pBrokenRule;
	NamesValuesList * pNamesValuesList;
	int				        geometryType;

  stdString = pError->getErrorId();
  if ( stdString != "" )
    writer->WriteElementString ( "Konformitätsregel",  QuConvert::ToString ( stdString ) ); 

  stdString = pError->getErrorName();
  if (stdString != "")
    writer->WriteElementString ( "Fehler", QuConvert::ToString ( stdString ) );
  
  stdString = pError->getErrorDescription();
  if (stdString != "")
    writer->WriteElementString ( "Fehlerbeschreibung", QuConvert::ToString ( stdString ) );

	stdString = pError->getFeatureId();
  if ( stdString != "" )
    writer->WriteElementString ( "GML-Id", QuConvert::ToString ( stdString ) );

	stdString = pError->getFeatureType();
  if ( stdString != "" )
    writer->WriteElementString ( "Klasse", QuConvert::ToString ( stdString ) );

	geometryType = pError->getFeatureGeometryType();
	switch ( geometryType )
	{
  case GEO_POINT:
	case MULTI_POINT:
		writer->WriteElementString ( "Geometrie-Typ", "Punkt" );
		break;

  case CURVE:
	case MULTI_CURVE:
  case LINE_STRING:
	case LINE_STRING_SEGMENT:

		writer->WriteElementString ( "Geometrie-Typ", "Linie" );
		break;
  case SURFACE:
  case POLYGON:
  case COMPOSITE_SURFACE:
  case SHELL:
  case ORIENTABLE_SURFACE:
  case TRIANGULATED_SURFACE:
	case MULTI_SURFACE:
		writer->WriteElementString ( "Geometrie-Typ", "Fläche" );
		break;

  case SOLID:
  case MULTI_SOLID:
    writer->WriteElementString ( "Geometrie-Typ", "Volumen" );
    break;

  case ALLG_GEOMETRIE:
  case MULTI_GEOMETRY:
  case GEOMETRIC_COMPLEX:
    writer->WriteElementString ( "Geometrie-Typ", "Gemischte Geometrie" );

	default:
		writer->WriteElementString ( "Geometrie-Typ", "Keine Gemetrie" );
	}

	stdString = pError->getRelatedFeatureId ();
  if ( stdString != "" )
		writer->WriteElementString ( "Referierte_GML-Id", QuConvert::ToString ( stdString ) );

	stdString = pError->getRelatedFeatureType ();
  if ( stdString != "" )
		writer->WriteElementString ( "Refererierter_Klasse", QuConvert::ToString ( stdString ) );

	stdString = pError->getRelationName ();
  if ( stdString != "" )
		writer->WriteElementString ( "Referenz-Name", QuConvert::ToString ( stdString ) );

	pBrokenRule            = pError->getBrokenRule();	
	pNamesValuesList = pError->getAttributes();
	if ( pNamesValuesList != NULL )
		writeNamesValuesList ( "Attributwerte", pNamesValuesList );	
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer Konformitätsregel                                        //
///////////////////////////////////////////////////////////////////////////////
void ErrorReportWriter::writeProfileRule ( ProfileRule * pProfileRule )
{
	string          stdString;
	ProfileFilter * pFilter;

	writer->WriteStartElement ( "BrokenRule" );

	stdString = pProfileRule->getGmlClassName();
  if ( stdString != "" )
		writer->WriteAttributeString ( "class", QuConvert::ToString ( stdString ) );

	pFilter = pProfileRule->getIfFilter();
	if ( pFilter != NULL )
		writeProfileFilter ( "If", pFilter );

	pFilter = pProfileRule->getRequiredFilter();
	if ( pFilter != NULL )
		writeProfileFilter ( "Required", pFilter );

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines Filters                                                  //
///////////////////////////////////////////////////////////////////////////////
void ErrorReportWriter::writeProfileFilter ( String ^ pFilterType, 
													   ProfileFilter * pFilter )
{
	size_t                   i, anz;
	vector<AttributeFilter*> attributeFilters;
	vector<RelationFilter*>  relationFilters;
	AttributeFilter        * pAttributeFilter;
	RelationFilter         * pRelationFilter;

	writer->WriteStartElement ( pFilterType );

	anz = pFilter->getAttributeFilters( attributeFilters );
	for ( i = 0; i < anz; i++ )
	{
		pAttributeFilter = attributeFilters[i];
		writeAttributeFilter ( pAttributeFilter );
	}

	anz = pFilter->getRelationFilters( relationFilters );
	for ( i = 0; i < anz; i++ )
	{
		pRelationFilter =  relationFilters[i];
		writeRelationFilter ( pRelationFilter );
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines Attribut-Filters                                         //
///////////////////////////////////////////////////////////////////////////////
void ErrorReportWriter::writeAttributeFilter ( AttributeFilter * pAttributeFilter )
{
	String ^ STR;
	string   stdString;
	int      attributeValueOperator;
	size_t   i, anz;
	int     n;

	writer->WriteStartElement ( "AttributeFilter" );

	stdString = pAttributeFilter->getAttributeName ();
	STR = QuConvert::ToString ( stdString );
	if ( STR != nullptr )
		writer->WriteAttributeString ( "attributeName", STR );
  
	n = pAttributeFilter->getMinOccurs();
	STR = QuConvert::ToString ( (int)n );
	writer->WriteAttributeString ( "minOccurs", STR );

	n = pAttributeFilter->getMaxOccurs();
	if ( n < INT_MAX )
	{
		STR = QuConvert::ToString ( n );
		writer->WriteAttributeString ( "maxOccurs", STR );
	}
	else
		writer->WriteAttributeString ( "maxOccurs", "unbound" );

	anz = pAttributeFilter->getAttributeValueAnz();
	for ( i = 0; i < anz; i++ )
	{
		stdString = pAttributeFilter->getAttributeValue ( i );
		attributeValueOperator = pAttributeFilter->getAttributeValueOperator ( i  );
		STR = QuConvert::ToString ( stdString );
		if ( STR != nullptr )
		{
			writer->WriteStartElement( "AttributeValue" );
			if ( attributeValueOperator == EQ )
				writer->WriteAttributeString ( "op", "EQ" );
			else
			if ( attributeValueOperator == NOT_EQ )
				writer->WriteAttributeString ( "op", "NOT_EQ" );
			writer->WriteString ( STR );
			writer->WriteEndElement();
		}
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines Relations-Filters                                        //
///////////////////////////////////////////////////////////////////////////////
void ErrorReportWriter::writeRelationFilter ( RelationFilter * pRelationFilter )
{
	String ^ STR;
	string   stdString;
	int      n, i;

	writer->WriteStartElement ( "RelationFilter" );

	stdString = pRelationFilter->getRelationName();
	STR = QuConvert::ToString ( stdString );
	if ( STR != nullptr )
		writer->WriteAttributeString ( "relationName", STR );

	n = pRelationFilter->getMinOccurs();
	STR = QuConvert::ToString ( n );
	writer->WriteAttributeString ( "minOccurs", STR );

	n = pRelationFilter->getMaxOccurs();
	if ( n < INT_MAX )
	{
		STR = QuConvert::ToString ( n );
		writer->WriteAttributeString ( "maxOccurs", STR );
	}
	else
		writer->WriteAttributeString ( "maxOccurs", "unbound" );

  for ( i = 0; i < pRelationFilter->getGmlTypeAnz(); i++ )
  {
	  stdString = pRelationFilter->getGmlTypeName( i );
	  STR = QuConvert::ToString ( stdString );
	  if ( STR != nullptr )
			writer->WriteElementString ( "GmlType", STR );
  }

  for ( i = 0; i < pRelationFilter->getGmlSuperTypeAnz(); i++ )
  {
    stdString = pRelationFilter->getGmlSuperTypeName( i );
    STR = QuConvert::ToString ( stdString );
    if ( STR != nullptr )
      writer->WriteElementString ( "GmlSUperType", STR );
  }

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer Liste von Attribut-Name - Attribut-Wert Paaren           //
///////////////////////////////////////////////////////////////////////////////
void ErrorReportWriter::writeNamesValuesList ( String ^ pListType, NamesValuesList * pList )
{
	int      i;
	string   name, value;
	String ^ STR;

	writer->WriteStartElement ( pListType );

	for ( i = 0; i < pList->getNamesValuesAnz(); i++ )
	{
		pList->getNameValue ( i, name, value );

		writer->WriteStartElement ( "Attribut" );

		STR = QuConvert::ToString ( name );
		if ( STR != nullptr )
			writer->WriteAttributeString ( "name", STR );

		STR = QuConvert::ToString ( value );
		if ( STR != nullptr )
			writer->WriteAttributeString ( "value", STR );

		writer->WriteEndElement();
	}

	writer->WriteEndElement();

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines Geometriefehlers                                         //
///////////////////////////////////////////////////////////////////////////////
void ErrorReportWriter::writeGeometryError (  GeometryError * pError )
{
  double   planarityErrorMax, planarityErrorAvarage;
  string   name, value;
  double   flaecheObj, flaecheObjRelated, flaecheUeberlappung;

  writer->WriteStartElement( "Geometriefehler" );

  writer->WriteElementString( "Fehler-Typ", QuConvert::ToString( pError->getGeometryErrorTypeAsString() ) );
  
  if ( pError->getErrorDescription() != "" )
    writer->WriteElementString( "Fehler-Beschreibung", QuConvert::ToString( pError->getErrorDescription() ) );

  if ( pError->getFeatureId() != ""  )
  {
    writer->WriteStartElement ( "Fehlerhaftes_Objekt" );
    writer->WriteElementString( "ID",     QuConvert::ToString( pError->getFeatureId() ) );
    writer->WriteElementString( "Geometrie-Id",   QuConvert::ToString( pError->getFeatureGeometryId() ) );
    writer->WriteElementString( "Geometrie-Typ", QuConvert::ToString( pError->getFeatureGeometryTypeAsString() ) );
    writer->WriteEndElement();
  }

  if ( pError->getRelatedFeatureId() != "" )
  {
    writer->WriteStartElement ( "Verknüpftes_Objekt" );
    writer->WriteElementString( "Id",     QuConvert::ToString( pError->getRelatedFeatureId() ) );
    writer->WriteElementString( "Geometrie-Id",   QuConvert::ToString( pError->getRelatedFeatureGeometryId() ) );
    writer->WriteElementString( "Geometrie-Typ", QuConvert::ToString( pError->getRelatedFeatureGeometryTypeAsString() ) );
    writer->WriteEndElement();
  }

  pError->getFlaechenangaben( flaecheObj, flaecheObjRelated, flaecheUeberlappung );
  if ( flaecheUeberlappung > 0.0 )
  {
    writer->WriteStartElement ( "Überlappungsfläche" );
    writer->WriteElementString ( "Fläche_Objekt", QuConvert::ToString ( flaecheObj, 0 ) ) ;
    writer->WriteElementString ( "Fläche_Verknüpftes_Pbjekt", QuConvert::ToString ( flaecheObjRelated, 0 ) );
    writer->WriteElementString ( "Flächengröße", QuConvert::ToString ( flaecheUeberlappung, 3 ) );
    writer->WriteEndElement();
  }

  pError->getPlanaritaetsfehler(  planarityErrorMax, planarityErrorAvarage );
  if ( planarityErrorMax > 0.0 )
  {
    writer->WriteStartElement ( "Planaritätsfehler" );
    writer->WriteElementString ( "Maximalabstand", QuConvert::ToString ( planarityErrorMax, 0 ) );
    writer->WriteEndElement();
  }

  writer->WriteEndElement();

}



























