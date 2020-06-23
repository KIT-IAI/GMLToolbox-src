#pragma once

//#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include "includes.h"

class BLeitPlan;
class PlanBereich;
class Features;
class Feature;
class AttributName;
class VerfahrensMerkmal;
class KlassenObjekte;
class GmlSchemaTypes;
class GmlSchemaElements;
class GmlSchema;
class GmlKlasse;
class GmlAttribut;
class GmlSchemaComplexType;
class GmlSchemaSimpleType;
class GmlEnumeration;
class ShapeFileObjekte;
class ShapeFileObjekt;
class GmlDictionary;
class SvgVisualisierung;
class Checking;
class SemanticRules;
class ErrorReport;
class TypeDefinitions;
class MigrationProtokoll;
class GmlNamespaces;
class SchemaTypeNames;
class TransformationINSPIRE_PLU_Parameter;
class TransformationINSPIREProtokoll;
class SrsManager;
class UOMList;
class UOM;
class ConstructionModule;
class Nutzungsprofile;
class Nutzungsprofil;
class Logfile;
class Visualisierung_2D;
class Visualisierung_3D;
class XPlanGML;

using namespace System;

///////////////////////////////////////////////////////////////////////////////
//  Zentrales Interface zwischen der Benutzeroberfläche und den thematischen //
//  Modulen                                                                  //
///////////////////////////////////////////////////////////////////////////////
class GmlToolbox
{
public:
	enum ANREICHERUNG_STEP { SCHRITT_1, SCHRITT_2, SCHRITT_3, SCHRITT_3A, SCHRITT_4, SCHRITT_5, END  };

public:
	GmlToolbox ( int anzObjMaxP );
	virtual ~GmlToolbox();

protected:
  int m_anzObjMax;  //  Maximalzahl einzulesender Features

  Logfile * m_pLogfile;   //  Logfile für Testausgaben und Fehlermeldungen

  UOMList * m_pUOMList;   //  Unity of Measurement Modul

  SrsManager * m_pSrsManager;  //  Koordinaten-Referenzsystem Modul 
		
	Features * m_pFeatures;   //  GML-Features

  XPlanGML * m_pXPlanGML;    //  XPlanGML-Modul

  Visualisierung_2D * m_pVisualisierung_2D;   //  Visualisierung von 2D Geodaten mit SVG

  Visualisierung_3D * m_pVisualisierung_3D;   //  Visualisierung von 3D Geodaten mit KML

  Checking * m_pChecking;  //  Modul zur geometrischen und semantischen Prüfung

  //  Sonstige Attribute
  std::string  m_KMLViewer;       // Aufrufpfad des externen KML-Viewers
  std::string  m_GMLViewer;       // Aufrufpfad des externen GML-Viewers
//  std::string  m_SVGViewer;       // Aufrufpfad des externen SVG-Viewers
  bool         m_showKMLFeatureData; // Anzeige von Feature-Attributdaten in der KML-Szene

  //  Spezialparameter für die SVG-Darstellung von 2D-Daten
  float  m_planausgabeBreite;
  float  m_planausgabeHoehe;
  double m_globalScale;
  double m_delta;
  double m_zOffset;

  std::string m_javaPath;       //  Pfad zur Java-Runtimesystem
  std::string m_batikPath;      //  Pfad zum Batik-Tool
  std::string m_javaHeapSize;   //  Java Heap-Size

  bool m_readCodelistDictionaries;  // Automatisches Einlesen non externen CodeList Dictionaries

  DARSTELLUNGS_TYP m_darstellungsTyp;   // Art der Visualisierung von 2D-Daten
  DARSTELLUNGS_TYP m_darstellungsTyp3D; // Art der Visualisierung von 3D-Daten 

protected:
  XPlanGML           * getXPlanGML();
  Visualisierung_2D  * getVisualisierung_2D();
  Visualisierung_3D  * getVisualisierung_3D();
  Nutzungsprofile    * getNutzungsprofile();
  ConstructionModule * getConstructionModule();

public:
  GML_SCHEMA_TYPE getGmlTyp(); 
  XPLAN_TYP       getPlanTyp();

  bool isXPlanGML();
  bool isINSPIREPLU();
  bool isINSPIRE();
  bool isCityGML();
  bool isALKIS();
  bool isBoreholeML();

  std::string getSystemFolder();
  std::string getSchemaPath();
  std::string getGmlVersion();

  size_t getPlanNamen ( std::vector<std::string> & vPlanNamen );
	size_t getPlanBereichNummern ( int planIndex, std::vector<int> & vPlanBereichNummern );
  void setPlanAktuell (int nummer );
  void setPlanBereichAktuell ( int index );
  void resetPlanBereichAktuell();

  void setExternalCodeListFolder ( GML_SCHEMA_TYPE gmlTyp, std::string folder );

  void writeStaticSVG ( std::string fileName );
  void writeKML ( std::string fileName );

  void transformationReferenzsystem( std::string fileName, std::string zielsystem );

  void showLogfile();
  void showManual();

  void gmlAttribute();

  bool specifyShapefiles ( std::vector<std::string> & vShapefiles, size_t & anzShapefiles, int &anzErrors );
	size_t getShapefileAnz();

  bool generateShapefileConversion();
  bool updateConversion();
  bool openConversionFile( String ^ konvDateiName,std::string & meldungen );
  int editConversion();

  bool existConversion();
  std::string getConversionFile();

  void saveConversion();
  void saveConversionAs( std::string fileName );

  bool convertShapefiles ( int & anzObjRead, int & anzObjNotRead, int & antShapefilesRead, 
          int & anzShapefilesNotRead, String ^ meldungen );

  void INSPIRE_Transformation ( String ^ XPlanGMLVersion );

  void migrationXPlanGML ( String ^ inputFile, String ^ XPlanGMLVersionQuelle, 
                String ^ XPlanGMLVersionZiel, String ^ migrationsfehlerStylesheet );

  void codelistenAendern();

  void visualisierung_2D( bool allePlaeneZeigen, System::Diagnostics::Process ^ iexplorerProcess );
  void visualisierung_3D( String ^  fileName, System::Diagnostics::Process ^ visualization3DProcess );

  void konformitaetspruefung( String ^ fileName );

  void planzeichenListeSteuerdatei();
  void planzeichenListe();

	void updateGML_Ids();

	int readGML ( std::string dateiName, bool dazuladen );
  int readGmlMultiFiles ( std::vector<std::string> &vDateiNamen );

  void showEditFeatures();

//  void setSvgViewer ( std::string nameP );
//  std::string getSvgViewer()  {  return m_SVGViewer;  }

  void setGMLViewer ( std::string nameP )  {  m_GMLViewer = nameP;  }
  std::string getGMLViewer()  {  return m_GMLViewer;  }

  void setKMLViewer ( std::string nameP )  {  m_KMLViewer = nameP;  }
  std::string getKMLViewer ()  {  return m_KMLViewer;  }
  void setShowKMLFeatureData ( bool b ) {  m_showKMLFeatureData = b;  }

  void setJavaPath ( std::string path )  {  m_javaPath = path;  }
  void setBatikPath ( std::string path )  {  m_batikPath = path;  }
  void setJavaHeapSize ( std::string s )  {  m_javaHeapSize = s;  }

  void setBoreholeRadius ( double r );
  void setBoreholeCylinderComplexity ( int c );
  void setBoreholeShowIntervals ( bool b );

  void setPlanausgabeBreite ( float b )  {  m_planausgabeBreite = b;  }
  void setPlanausgabeHoehe ( float h )   {  m_planausgabeHoehe = h;  }
  void setGlobalScale ( double scale );

  void setDarstellungsTyp ( DARSTELLUNGS_TYP typ )   {  m_darstellungsTyp = typ;  }
  void setDarstellungsTypFromString ( std::string typ );

  DARSTELLUNGS_TYP getDarstellungsTyp()  {  return m_darstellungsTyp;  }

  void setDarstellungsTyp3D ( DARSTELLUNGS_TYP typ )   {  m_darstellungsTyp3D = typ;  }
  DARSTELLUNGS_TYP getDarstellungsTyp3D()  {  return m_darstellungsTyp3D;  }

  int writeXPlanGML( std::string fileName );
  int writeXPlanGML( Features * pPlanObjekte, std::string fileName );

  Checking * getChecking()  {  return m_pChecking;  }

  void readUOMList ();

  void setReadCodeListDictionaries ( bool b )  {  m_readCodelistDictionaries = b;  }
  bool getReadCodeListDictionaries() {  return m_readCodelistDictionaries;  }

  void showINSPIRECodelist();

  void generateEnumerationDictionaries();

  void setZOffset ( double z )  {  m_zOffset = z;  }
  double getZOffset()  {  return m_zOffset;  }

  void checkCoordTransformationModule();
};





