#include "StdAfx.h"

#include <string>
#include "Constant.h"
#include "CreateGuid_64.h"
#include "Convert.h"
#include "HTMLDatei.h"
#include "AppConfigPath.h"

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt eine universell eindeutige gml:id                                //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::createGmlId ()
{
  GUID         guid;
  std::string  m_guid;
  const int   len = 100;
  char        guidString[len];
  std::string gmlId;

  m_guid = CreateCompressedGuidString (guidString, len);
  GetGuidFromString64 (m_guid.c_str(), &guid);
  CreateReadableStringFromGuid (&guid, (char*)&guidString);
  gmlId = "GML_" + std::string ( guidString );

  return gmlId;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den System-Ordner                                                //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getSystemFolder()
{
  std::string systemFolder;

  AppConfigPath^ appConfigPath = AppConfigPath::getInstance();
  String ^ SystemFolder = appConfigPath->getPath();

  QuConvert::systemStr2stdStr( systemFolder, SystemFolder );
  return systemFolder;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad zur Logfile-Datei                                       //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getLogfilePath()
{
  return getSystemFolder() + "Logfile.txt";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad zum Benutzerhandbuch                                    //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getManualPath()
{
  return getSystemFolder() + "Benutzerhandbuch.pdf";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Ordner des Zeitreihen-Repository                             //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getTimeSeriesRepositoryFolder()
{
	string repositoryFolder = getSystemFolder() + "TimeSeriesRepository";
	String ^ STR = QuConvert::ToString ( repositoryFolder );
	if ( !Directory::Exists ( STR ) )
		Directory::CreateDirectory ( STR );

  return repositoryFolder;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad der Unit-of-Measurement XML-Datei                       //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getUOMListPath()
{
  return getSystemFolder() + "UOMListe.xml";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad der Constructuion XMKL-Datei                            //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getConstructionListPath()
{
  return getSystemFolder() + "ConstructionList.gml";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad der Koordinaten-Referenzsystem XML-Datei                //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getReferenceSystemListPath()
{
  return getSystemFolder() + "ReferenzsystemListe.xml";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad der Nutzungsprofil XML-Datei                            //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getUsageProfileListPath()
{
  return getSystemFolder() + "NutzungsprofilListe.xml";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad zur Abspeicherung von Attribut-Informationen bei der    //
//  2D-Visualisierung                                                        //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getObjectDataPath()
{
  return getSystemFolder() + "__ObjectData\\";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den INSPIRE-PLU Klassenamen                                      //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getKlassenNameINSPIRE ( INSPIRE_OBJEKT_TYP typ )
{
  switch ( typ )
  {
  case SPATIAL_PLAN:
    return "plu:SpatialPlan";

  case ZONING_ELEMENT:
    return "plu:ZoningElement";

  case SUPPLEMENTARY_RESTRICTION:
    return "plu:SupplementaryRegulation";
  }

  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Namen der HTML-Rahmendatei bei der 2D-Visualisierung         //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getHTMLFramePath ( GML_SCHEMA_TYPE typ )
{
  string htmlRahmen;

  if ( typ > XPLANGML_2_0 && typ < CITYGML_0_4_0 )
    htmlRahmen =  "Bebauungsplan.html";
  else
  if ( typ == BOREHOLEML_3_0 )
    htmlRahmen = "Borehole.html";
  else
    return "";

  return getSystemFolder() + htmlRahmen;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad der jeweiligen XML-Schema Dateien                       //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getSchemaPath ( GML_SCHEMA_TYPE typ )
{
  string schemaFolder;
  switch ( typ )
  {
  case XPLANGML_2_0:
    schemaFolder = "GmlSchemata\\XPlanGML_2_0\\";
    break;

  case XPLANGML_3_0:
    schemaFolder = "GmlSchemata\\XPlanGML_3_0\\";
    break;

  case XPLANGML_4_0:
    schemaFolder = "GmlSchemata\\XPlanGML_4_0\\";
    break;

  case XPLANGML_4_1:
    schemaFolder = "GmlSchemata\\XPlanGML_4_1\\";
    break;

  case XPLANGML_5_0:
    schemaFolder = "GmlSchemata\\XPlanGML_5_0\\";
    break;

  case XPLANGML_5_1:
    schemaFolder = "GmlSchemata\\XPlanGML_5_1\\";
    break;

  case XPLANGML_5_2:
    schemaFolder = "GmlSchemata\\XPlanGML_5_2\\";
    break;

  case XPLANGML_5_3:
    schemaFolder = "GmlSchemata\\XPlanGML_5_3\\";
    break;

  case INSPIRE_LU_PLANNED:
    schemaFolder = "GmlSchemata\\INSPIRE_LU_PLANNED\\";
    break;

  case INSPIRE:
    schemaFolder = "GmlSchemata\\INSPIRE\\";
    break;

  case NAS_5_1:
    schemaFolder = "GmlSchemata\\NAS_5_1\\";
    break;

  case NAS_5_1_1:
    schemaFolder = "GmlSchemata\\NAS_5_1_1\\";
    break;

  case NAS_6_0:
    schemaFolder = "GmlSchemata\\NAS_6_0\\";
    break;

  case CITYGML_0_4_0:
    schemaFolder = "GmlSchemata\\CityGML_0_4_0\\";
    break;

  case CITYGML_1_0:
    schemaFolder = "GmlSchemata\\CityGML_1_0\\";
    break;

  case CITYGML_2_0:
    schemaFolder = "GmlSchemata\\CityGML_2_0\\";
    break;

  case CITYGML_3_0:
    schemaFolder = "GmlSchemata\\CityGML_3_0\\";
    break;

  case BOREHOLEML_3_0:
    schemaFolder = "GmlSchemata\\BOREHOLEML_3_0\\";
    break;

  case GML_SCHEMA_DEFAULT:
    schemaFolder = "GmlSchemata\\Default_Schema\\";
    break;

  default:
    schemaFolder = "GmlSchemata\\";
    break;
  }

  return getSystemFolder() + schemaFolder;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad der jeweiligen Stylesheet Dateien                       //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getStylesheetPath ( GML_SCHEMA_TYPE typ )
{
  string stylesheetFolder;

  switch ( typ )
  {
  case XPLANGML_2_0:
    stylesheetFolder = "GmlSchemata\\XPlanGML_2_0\\Stylesheets\\";
    break;

  case XPLANGML_3_0:
    stylesheetFolder = "GmlSchemata\\XPlanGML_3_0\\Stylesheets\\";
    break;

  case XPLANGML_4_0:
    stylesheetFolder = "GmlSchemata\\XPlanGML_4_0\\Stylesheets\\";
    break;

  case XPLANGML_4_1:
    stylesheetFolder = "GmlSchemata\\XPlanGML_4_1\\Stylesheets\\";
    break;

  case XPLANGML_5_0:
    stylesheetFolder = "GmlSchemata\\XPlanGML_5_0\\Stylesheets\\";
    break;

  case XPLANGML_5_1:
    stylesheetFolder = "GmlSchemata\\XPlanGML_5_1\\Stylesheets\\";
    break;

  case XPLANGML_5_2:
    stylesheetFolder = "GmlSchemata\\XPlanGML_5_2\\Stylesheets\\";
    break;

  case XPLANGML_5_3:
    stylesheetFolder = "GmlSchemata\\XPlanGML_5_3\\Stylesheets\\";
    break;

  case INSPIRE_LU_PLANNED:
    stylesheetFolder = "GmlSchemata\\INSPIRE_LU_PLANNED\\Stylesheets\\";
    break;
 
  case NAS_5_1:
    stylesheetFolder = "GmlSchemata\\NAS_5_1\\Stylesheets\\";
    break;

  case NAS_5_1_1:
    stylesheetFolder = "GmlSchemata\\NAS_5_1_1\\Stylesheets\\";
    break;

  case NAS_6_0:
    stylesheetFolder = "GmlSchemata\\NAS_6_0\\Stylesheets\\";
    break;

  case CITYGML_0_4_0:
    stylesheetFolder =  "GmlSchemata\\CityGML_0_4_0\\Stylesheets\\";
    break;

  case CITYGML_1_0:
    stylesheetFolder =  "GmlSchemata\\CityGML_1_0\\Stylesheets\\";
    break;

  case CITYGML_2_0:
    stylesheetFolder =  "GmlSchemata\\CityGML_2_0\\Stylesheets\\";
    break;

  case CITYGML_3_0:
    stylesheetFolder =  "GmlSchemata\\CityGML_3_0\\Stylesheets\\";
    break;
 
  case BOREHOLEML_3_0:
    stylesheetFolder = "GmlSchemata\\BOREHOLEML_3_0\\Stylesheets\\";
    break;

  case GML_SCHEMA_DEFAULT:
    stylesheetFolder = "GmlSchemata\\Default_Schema\\Stylesheets\\";
    break;

  default:
    stylesheetFolder = "\\";
    break;
  }

  return getSystemFolder() + stylesheetFolder;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad der jeweiligen Codelist Dateien                         //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getCodelistPath ( GML_SCHEMA_TYPE typ )
{
  string codelistFolder;

  switch ( typ )
  {
  case XPLANGML_2_0:
    codelistFolder = "GmlSchemata\\XPlanGML_2_0\\Codelists\\";
    break;

  case XPLANGML_3_0:
    codelistFolder =  "GmlSchemata\\XPlanGML_3_0\\Codelists\\";
    break;

  case XPLANGML_4_0:
    codelistFolder =  "GmlSchemata\\XPlanGML_4_0\\Codelists\\";
    break;

  case XPLANGML_4_1:
    codelistFolder =  "GmlSchemata\\XPlanGML_4_1\\Codelists\\";
    break;

  case XPLANGML_5_0:
    codelistFolder =  "GmlSchemata\\XPlanGML_5_0\\Codelists\\";
    break;

  case XPLANGML_5_1:
    codelistFolder =  "GmlSchemata\\XPlanGML_5_1\\Codelists\\";
    break;

  case XPLANGML_5_2:
    codelistFolder =  "GmlSchemata\\XPlanGML_5_2\\Codelists\\";
    break;

  case XPLANGML_5_3:
    codelistFolder =  "GmlSchemata\\XPlanGML_5_3\\Codelists\\";
    break;

  case INSPIRE_LU_PLANNED:
    codelistFolder = "GmlSchemata\\INSPIRE_LU_PLANNED\\Codelists\\";
    break;

  case NAS_5_1:
    codelistFolder = "GmlSchemata\\NAS_5_1\\Codelists\\";
    break;

  case NAS_5_1_1:
    codelistFolder = "GmlSchemata\\NAS_5_1_1\\Codelists\\";
    break;

  case NAS_6_0:
    codelistFolder = "GmlSchemata\\NAS_6_0\\Codelists\\";
    break;

  case CITYGML_0_4_0:
    codelistFolder =  "GmlSchemata\\CityGML_0_4_0\\Codelists\\";
    break;

  case CITYGML_1_0:
    codelistFolder =  "GmlSchemata\\CityGML_1_0\\Codelists\\";
    break;

  case CITYGML_2_0:
    codelistFolder =  "GmlSchemata\\CityGML_2_0\\Codelists\\";
    break;

  case CITYGML_3_0:
    codelistFolder =  "GmlSchemata\\CityGML_3_0\\Codelists\\";
    break;

  default:
    codelistFolder = "\\";
    break;
  }

  return getSystemFolder() + codelistFolder;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad der jeweiligen Definitions Dateien                      //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getDefinitionsPath ( GML_SCHEMA_TYPE typ )
{
  string definitionsFolder;

  switch ( typ )
  {

  case XPLANGML_4_0:
    definitionsFolder =   "GmlSchemata\\XPlanGML_4_0\\Definitions\\";
    break;

  case XPLANGML_4_1:
    definitionsFolder =   "GmlSchemata\\XPlanGML_4_1\\Definitions\\";
    break;

  case XPLANGML_5_0:
    definitionsFolder =   "GmlSchemata\\XPlanGML_5_0\\Definitions\\";
    break;

  case XPLANGML_5_1:
    definitionsFolder =   "GmlSchemata\\XPlanGML_5_1\\Definitions\\";
    break;

  case XPLANGML_5_2:
    definitionsFolder =   "GmlSchemata\\XPlanGML_5_2\\Definitions\\";
    break;

  case XPLANGML_5_3:
    definitionsFolder =   "GmlSchemata\\XPlanGML_5_3\\Definitions\\";
    break;

  case NAS_6_0:
    definitionsFolder = "GmlSchemata\\NAS_6_0\\Definitions\\";
    break;

  default:
    definitionsFolder = "\\";
    break;
  }

  return getSystemFolder() + definitionsFolder;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfad der jeweiligen Konformitätsbedingungen                  //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getConformityPath ( GML_SCHEMA_TYPE typ )
{
  string conformityFolder;

  switch ( typ )
  {
  case XPLANGML_3_0:
    conformityFolder =  "GmlSchemata\\XPlanGML_3_0\\Konformitaet\\";
    break;

  case XPLANGML_4_0:
    conformityFolder =  "GmlSchemata\\XPlanGML_4_0\\Konformitaet\\";
    break;

  case XPLANGML_4_1:
    conformityFolder =  "GmlSchemata\\XPlanGML_4_1\\Konformitaet\\";
    break;

  case XPLANGML_5_0:
    conformityFolder =  "GmlSchemata\\XPlanGML_5_0\\Konformitaet\\";
    break;

  case XPLANGML_5_1:
    conformityFolder =  "GmlSchemata\\XPlanGML_5_1\\Konformitaet\\";
    break;

  case XPLANGML_5_2:
    conformityFolder =  "GmlSchemata\\XPlanGML_5_2\\Konformitaet\\";
    break;

  case XPLANGML_5_3:
    conformityFolder =  "GmlSchemata\\XPlanGML_5_3\\Konformitaet\\";
    break;

  case CITYGML_0_4_0:
    conformityFolder =  "GmlSchemata\\CityGML_0_4_0\\Konformitaet\\";
    break;

  case CITYGML_1_0:
    conformityFolder =  "GmlSchemata\\CityGML_1_0\\Konformitaet\\";
    break;

  case CITYGML_2_0:
    conformityFolder =  "GmlSchemata\\CityGML_2_0\\Konformitaet\\";
    break;

  case CITYGML_3_0:
    conformityFolder =  "GmlSchemata\\CityGML_3_0\\Konformitaet\\";
    break;

  default:
    conformityFolder = "GmlSchemata\\Default_Schema\\Konformitaet\\";
    break;
  }

  return getSystemFolder() + conformityFolder;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfadnamen der CSV-Datei mit den ALKIS Funktionscodes der     //
//  Objektarten 31001 (Attribut "gebaeudefunktion") und 51001 - 51007        // 
//  (Attribut "bauwerkfunktion")                                             //
///////////////////////////////////////////////////////////////////////////////
 std::string Constant::getALKISFunktionenPath()
{
  return getSystemFolder() + "AX_Gebaeude_Funktionen.csv";
}

 ///////////////////////////////////////////////////////////////////////////////
 //  Liefert den Pfadnamen der CSV-Datet mit den ALKIS Funktionscodes der     //
 //  Objektarten 31001, Attribut "weitereGebaeudefunktion"                    // 
 ///////////////////////////////////////////////////////////////////////////////
 std::string Constant::getALKISWeitereFunktionenPath()
 {
   return getSystemFolder() + "AX_Weitere_Gebaeude_Funktionen.csv";
 }

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfadnamen der XML-Datei mit den Standard-Materialparametern  //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getDefaultBuildingParamsPath()
{
  return getSystemFolder() + "DefaultBuildingParams.xml";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfadnamen der XML-Datei mit den Standard-Nutzungsparametern  //
//  und Angaben zur Interpretation von ALKIS-Funktionscodes                  //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getUsageTypologyPath()
{
  return getSystemFolder() + "UsageTypology.xml";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den GML-Schema Typ in Textform                                   //
///////////////////////////////////////////////////////////////////////////////
std::string Constant::getGMLSchemaTypeAsString ( GML_SCHEMA_TYPE typ )
{
  string gmlVersion;

  switch ( typ )
  {
  case XPLANGML_2_0:
    gmlVersion = "XPlanGML Version 2";
    break;

  case XPLANGML_3_0:
    gmlVersion = "XPlanGML Version 3.0";
    break;

  case XPLANGML_4_0:
    gmlVersion ="XPlanGML Version 4.0";
    break;

  case XPLANGML_4_1:
    gmlVersion = "XPlanGML Version 4.1";
    break;

  case XPLANGML_5_0:
    gmlVersion = "XPlanGML Version 5.0";
    break;

  case XPLANGML_5_1:
    gmlVersion = "XPlanGML Version 5.1";
    break;

  case XPLANGML_5_2:
    gmlVersion = "XPlanGML Version 5.2";
    break;

  case XPLANGML_5_3:
    gmlVersion = "XPlanGML Version 5.3";
    break;

  case INSPIRE_LU_PLANNED:
    gmlVersion = "INSPIRE PLU Version 4.0";
    break;

  case INSPIRE:
    gmlVersion = "INSPIRE";
    break;

  case NAS_5_1:
    gmlVersion = "ALKIS Version 5.1";
    break;

  case NAS_5_1_1:
    gmlVersion = "ALKIS Version 5.1.1";
    break;

  case NAS_6_0:
    gmlVersion = "ALKIS Version 6.0";
    break;

  case CITYGML_0_4_0:
    gmlVersion = "CityGML Version 0.4.0";
    break;

  case CITYGML_1_0:
    gmlVersion = "CityGML Version 1.0";
    break;

  case CITYGML_2_0:
    gmlVersion = "CityGML Version 2.0";
    break;

  case CITYGML_3_0:
    gmlVersion = "CityGML Version 3.0";
    break;

  case BOREHOLEML_3_0:
    gmlVersion = "BoreholeML Version 3.0";
    break;

  case GML_SCHEMA_DEFAULT:
    gmlVersion = "Benutzerdefiniertes Schema";
    break;

  default:
    gmlVersion = "Nicht gesetzt";
    break;
  }

  return gmlVersion;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den GML-Schema Typ als Enumerationswert                          //
///////////////////////////////////////////////////////////////////////////////
GML_SCHEMA_TYPE Constant::getGMLSchemaTypeFromString( std::string typString )
{
  if ( typString == "XPlanGML Version 2" )           return XPLANGML_2_0;
  if ( typString == "XPLANGML_V_2" )                 return XPLANGML_2_0;
  if ( typString == "XPlanGML Version 3.0" )         return XPLANGML_3_0;
  if ( typString == "XPLANGML_V_3" )                 return XPLANGML_3_0;
  if ( typString == "XPlanGML Version 4.0" )         return XPLANGML_4_0;
  if ( typString == "XPLANGML_V_4" )                 return XPLANGML_4_0;
  if ( typString == "XPlanGML Version 4.1" )         return XPLANGML_4_1;
  if ( typString == "XPlanGML Version 5.0" )         return XPLANGML_5_0;
	if ( typString == "XPLANGML_5_0")                   return XPLANGML_5_0;
  if ( typString == "XPlanGML Version 5.1" )         return XPLANGML_5_1;
	if ( typString == "XPLANGML_5_1")                  return XPLANGML_5_1;
  if ( typString == "XPlanGML Version 5.2" )         return XPLANGML_5_2;
	if ( typString == "XPLANGML_5_2" )                 return XPLANGML_5_2;
  if ( typString == "XPlanGML Version 5.3" )         return XPLANGML_5_3;
  if ( typString == "XPLANGML_5_3" )                 return XPLANGML_5_3;
  if ( typString == "INSPIRE PLU Version 4.0" )      return INSPIRE_LU_PLANNED;
  if ( typString == "INSPIRE" )                      return INSPIRE;
  if ( typString == "ALKIS Version 5.1" )            return NAS_5_1;
  if ( typString == "ALKIS Version 5.1.1")           return NAS_5_1_1;
  if ( typString == "ALKIS Version 6.0" )            return NAS_6_0;
  if ( typString == "CityGML Version 0.4.0" )        return CITYGML_0_4_0;
  if ( typString == "CityGML Version 1.0" )          return CITYGML_1_0;
  if ( typString == "CityGML Version 2.0" )          return CITYGML_2_0;
  if ( typString == "CityGML Version 3.0" )          return CITYGML_3_0;
  if ( typString == "BoreholeML Version 3.0" )       return BOREHOLEML_3_0;
  if ( typString == "Benutzerdefiniertes Schema" )   return GML_SCHEMA_DEFAULT;

  return GML_SCHEMA_UNKNOWN;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt eine XSLT-Transformation durch                                     //
///////////////////////////////////////////////////////////////////////////////
void Constant::performXsltTransformation ( string xmlPathP, string htmlPathP, string stylesheetPathP )
{
  String                      ^ xmlPath;
  String                      ^ htmlPath;
  String                      ^ stylesheetPath;

  xmlPath        = QuConvert::ToString( xmlPathP );
  htmlPath       = QuConvert::ToString( htmlPathP );
  stylesheetPath = QuConvert::ToString( stylesheetPathP );

  Xsl::XslCompiledTransform ^ xslTransformer = gcnew Xsl::XslCompiledTransform;
  xslTransformer->Load( stylesheetPath );
  xslTransformer->Transform( xmlPath, htmlPath );
}

///////////////////////////////////////////////////////////////////////////////
//  Anzeige eines html-Files                                                 //
///////////////////////////////////////////////////////////////////////////////
void Constant::showHtmlFile ( std::string fileName )
{
  String     ^ FILE_NAME;

  FILE_NAME = QuConvert::ToString( fileName );
  HTMLDatei ^ htmlBrowser = gcnew HTMLDatei( FILE_NAME );

  htmlBrowser->Show();
}

///////////////////////////////////////////////////////////////////////////////
//  Anzeige eines Text-Files                                                 //
///////////////////////////////////////////////////////////////////////////////
 void Constant::showTextFile ( String ^  fileName )
 {
   System::Diagnostics::Process::Start ( fileName );
 }







