#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include "includes.h"

const double MINIMAL_POINT_DISTANCE  = 2.0E-4;
const double MINIMAL_ANGLE_DISTANCE  = 1.0E-5;
const double AREA_TOLERANCE_RELATIVE = 1.0E-3;

const double PI = 3.141592653589793238462643;
const double twoPI = 2.0 * PI;
const double halfPI = 0.5 * PI;
const double deg2rad = PI / 180.0;
const double rad2deg = 180.0 / PI;

///////////////////////////////////////////////////////////////////////////////
//  Verschiedene grundlegende statische Funktionen                           //
///////////////////////////////////////////////////////////////////////////////
class Constant
{
public:
  Constant() {}
  virtual ~ Constant()  {}

public:
  static std::string createGmlId ();
  static std::string getSystemFolder();
  static std::string getLogfilePath();
  static std::string getManualPath();
  static std::string getTimeSeriesRepositoryFolder();
  static std::string getUOMListPath();  
  static std::string getConstructionListPath();
  static std::string getReferenceSystemListPath();
  static std::string getUsageProfileListPath();
  static std::string getObjectDataPath();
  static std::string getHTMLFramePath ( GML_SCHEMA_TYPE typ );
  static std::string getSchemaPath ( GML_SCHEMA_TYPE typ );
  static std::string getStylesheetPath ( GML_SCHEMA_TYPE typ );
  static std::string getCodelistPath( GML_SCHEMA_TYPE typ  );
  static std::string getDefinitionsPath( GML_SCHEMA_TYPE typ  );
  static std::string getConformityPath( GML_SCHEMA_TYPE typ  );
  static std::string getALKISFunktionenPath();
  static std::string getALKISWeitereFunktionenPath();

  static std::string     getGMLSchemaTypeAsString ( GML_SCHEMA_TYPE typ );
  static GML_SCHEMA_TYPE getGMLSchemaTypeFromString ( std::string typ );

  static std::string getKlassenNameINSPIRE ( INSPIRE_OBJEKT_TYP typ );

  static void performXsltTransformation ( std::string xmlPath, std::string htmlPath, std::string stylesheetPath );
  static void showHtmlFile ( std::string fileName );
  static void showTextFile ( System::String ^ fileName );

};


#endif



