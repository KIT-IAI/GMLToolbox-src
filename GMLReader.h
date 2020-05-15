#pragma once

#include "TimeX.h"
#include "Geometrie.h"

class GmlToolbox;
class GmlKlasse;
class Appearances;
class AppearanceColor;
class AppearanceParametrizedTexture;
class AppearanceGeoreferencedTexture;
class TexCoordList;
class TexCoordGen;
class RegularTimeSeries;
class RegularTimeSeriesFile;
class SchemaTypeNames;
class GmlNamespaces;
class Checking;
class Appearance;
class TexCoord;
class AppearanceData;
class cAppearance;
class AbstractTimeSeries;
class GmlSchema;

///////////////////////////////////////////////////////////////////////////////
//  GMLReader                                                                //
//  Einlesen ein oder mehrerer GML-Dateien                                   //
///////////////////////////////////////////////////////////////////////////////
public ref class GMLReader
{
public:
	GMLReader( Features * pFeaturesP, bool readCodeListDictionaries );
	virtual ~ GMLReader();

protected:
  System::Xml::XmlTextReader ^ validReader;   // .NET XML-Reader
	Features              * pFeatures;              // Aggregation der eingelesenen Features
  Appearances           * pAppearances;           // Nur CityGML: Aggregation der eingelesenen Appearances
  GmlSchema             * pGmlSchema;             // Zugehöriges GML-Schema
	XPLAN_TYP               planTyp;                // Nur XPlanGML: Tpy des eingelesenen XPlanGML-Modells
  GmlNamespaces         * pGmlNamespacesAct;      // Namespaces der eingelesenen GML-Datei(en)
  GmlNamespaces         * pGmlNamespacesStandard; // Standard-Namespaces des GML-Schemas
  SchemaTypeNames       * pSchemaTypeNamesStandard; // Schema-Types der Standard-Namespaces
  ErrorReport           * pReadErrors;              // Sammlung von Fehlern, die beim Einlesen der GML-Datei auftreten
  String                ^ pFolderName;              // Ordner der eingelesenen GML-Datei 
	String                ^ gmlIdTag;                 // XML-Tag der gml:id
	String                ^ pActElement;              // Aktuelles XML-Element
  String                ^ pActElementLoc;           // Aktuelles XML-Element ohne Namespace-Kürzel
	String                ^ pActAttrName;             // Aktuelles XML-Attribut
  String                ^ featureDateiName;         // Pfadname der aktuell eingelesenen GML-Datei

  static String         ^ INSPIRECodelistSuffix;    // Nur INSPIRE PLU: Erweiterung, um aus dem Codelist-Namen das 
                                                    // zug. GML-Dictionary zu generieren
  static cli::array<wchar_t> ^ delimiter;           // Delimiter bei der Trennung von GML-Koordinatenlisten in Einzelkoordinaten

 	int	 nErr;                        // Anzahl der aufgetretenen Fehler
	int	 featureAnz;                  // Anzahl der eingelesenen Features
	bool swRead;                      // Beim Einlesen der gml:Envelope: Untere, linke Ecke wurde gelesen
	bool noRead;                      // Beim Einlesen der gml:Envelope: Obere, rechte Ecke wurde gelesen
  bool generateFeatureDateienMap;   // Steuert, ob eine map aufgebaut wird, die jedem Feature die zugehörige Datei zuordnet
  bool m_readCodeListDictionary;    // Steuert, ob automatisch die Dictionaries externer Codelisten eingelesen werden
	bool m_readAppearances;						// Steuert, ob CityGML Appearance Informationen mit eingelesen werden


public:
  void reset();

  int read( std::string fileName, Checking * pChecking );
  int readWithoutModification (  std::string fileName, Checking * pChecking );
  void modifyModel();

	int getFeatureAnz()  {  return featureAnz;  }

  ErrorReport * getReadErrors()  {  return pReadErrors;  }

  static String ^  readCSVFile (  RegularTimeSeriesFile * pRegularTimeSeries, String ^ pFolderName );

  void setGenerateFeatureDateienMap ( bool b  )  {  generateFeatureDateienMap = b; }

  static std::string generateExternalCodelistPath ( String ^ CODE );

protected:
  bool readEnvelope( double * sw, double * no, std::string & srsName, int & srsDimension );
	Feature * readFeatureMember( String ^ gmlTag );
	Feature * readFeature(  System::String ^ pObjektNameP, GmlKlasse * pGmlReferenceType, std::string featureMemberName );
  void readRelatedFeatures ( Feature *  pParent, GmlKlasse * pGmlReferenceType, std::string gmlAttributName, 
                            std::vector<std::string> classifierNamen, std::vector<std::string> classifierValues, 
                            String ^ END_TAG );
 
  void readFeatureGeometrie ( std::string geometryPropertyName, Feature * pObjP, std::string gmlTagP, bool emptyElement );
  void readDirectPosition ( std::string geometryPropertyName, Feature * pObjP, std::string gmlTagP );

  _Geometrie * readGeometrie ( System::String ^ gmlTagP, std::string srsName, int srsDimension );
  void readAddress ( Feature * pFeature, String ^ GML_TAG );

	MultiSurface  * readMultiSurface ( System::String ^ gmlTag, std::string srsName, int srsDimension );
	MultiCurve    * readMultiCurve ( System::String ^ gmlTag, std::string srsName, int srsDimension );
	MultiPoint    * readMultiPoint ( System::String ^ gmlTag, std::string srsName, int srsDimension );
  MultiSolid    * readMultiSolid ( System::String ^ gmlTag, std::string srsName, int srsDimension );
  MultiGeometry * readMultiGeometry ( System::String ^ gmlTag, std::string srsName, int srsDimension );
  GeometricComplex * readGeometricComplex ( System::String ^ gmlTag, std::string srsName, int srsDimension );
  
  void readAppearance ( System::String ^ gmlTag );
  void readSurfaceDataMember( cAppearance * pAppearance );

  void readX3DMaterial ( AppearanceColor * pColor );
  void readGeoreferencedTexture ( AppearanceGeoreferencedTexture * pTexture );
  void readParametrizedTexture ( AppearanceParametrizedTexture * pTexture );
  void readTexCoordList( AppearanceParametrizedTexture * pTexture, TexCoordList * pTexCoordList );
  void readTexCoordGen( AppearanceParametrizedTexture * pTexture, TexCoordGen * pTexCoodGen );


  ImplicitGeometry * readImplicitGeometry ( System::String ^ gmlTag, std::string srsName, int srsDimension );
  Matrix4           * readMatrix4 (  System::String ^ gmlTag );
  OrientableSurface * readOrientableSurface ( System::String ^ gmlTag, std::string srsName, int srsDimension  );

  _Surface   * readSurfaceMember( System::String ^ gmlTag, std::string srsName, int srsDimension );
	_Curve     * readCurveMember  ( System::String ^ gmlTag, std::string srsName, int srsDimension );
  _Solid     * readSolidMember ( System::String ^ gmlTag, std::string srsName, int srsDimension );

  void readCurveMembers  ( System::String ^ gmlTag, std::string srsName, int srsDimension, MultiCurve * pMultiCurve );
  void readSurfaceMembers( System::String ^ gmlTag, std::string srsName, int srsDimension, MultiSurface * pMultiSurface );

	Surface     * readSurface ( System::String ^ gmlTag, std::string srsName, int srsDimension );
  PolygonGeo  * readPolygon ( System::String ^ gmlTag, std::string srsName, int srsDimension );
  _Ring       * readRing ( System::String ^ gmlTag, std::string srsName, int srsDimension );
  LineSegment * readLineSegment ( System::String ^ gmlTag, std::string srsName, int srsDimension, LineSegment::LINE_SEGMENT_TYPE lineSegmentType );
	size_t        readArcSegment ( System::String ^ gmlTag, ArcSegment::ARC_SEGMENT_TYPE arcSegmentType, std::vector<ArcSegment*> &vSegments, 
                                 std::string srsName, int srsDimension  );
	Curve           * readCurve ( System::String ^ gmlTag, std::string srsName, int srsDimension );
	OrientableCurve * readOrientableCurve ( System::String ^ gmlTag, std::string srsName, int srsDimension );
	GeoPoint        * readPoint ( System::String ^ gmlTag, std::string srsName, int srsDimension );
  Solid * readSolid ( System::String ^ gmlTag, std::string srsName, int srsDimension );

  void writeGmlError ( std::string errorMessage, System::String ^ gmlTag );
	void skipElement( System::String ^ gmlTag );

  std::string createGmlNameStandard ( std::string gmlName );
  std::string addDefaultNamespaceKuerzel ( std::string gmlName );
  String ^ addDefaultNamespaceKuerzel ( String ^ gmlName );

  void readTimePeriod ( std::string gmlAttributNameAct, Feature * pFeature, std::string gmlAttributNameOrg );
  void readTimeSeries ( std::string gmlAttributNameAct, Feature * pFeature, std::string gmlAttributNameOrg );

  AbstractTimeSeries * readRegularTimeSeries ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg );
  AbstractTimeSeries * readIrregularTimeSeries ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg );
  AbstractTimeSeries * readRegularTimeSeriesFile ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg );
  AbstractTimeSeries * readIrregularTimeSeriesFile ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg );
  AbstractTimeSeries * readMonthlyTimeSeries ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg );

  TimePeriod * readTimePeriod ( std::string gmlAttributNameAct, std::string gmlAttributNameOrg );
  TimePosition readTimePosition();
  TimeIntervalLength * readTimeIntervalLength();
  void readTimeSeriesValues ( std::string gmlAttributNameActLoc, RegularTimeSeries * pRegularTimeSeries, std::string gmlAttributNameOrgLoc );
  void readTimeSeriesVariableProperties (  std::string gmlAttributNameAct, AbstractTimeSeries * pTimeSeries, std::string gmlAttributNameOrg );
};

///////////////////////////////////////////////////////////////////////////////
//  Datenstruktur ESRI World-File                                            //
///////////////////////////////////////////////////////////////////////////////
struct Georef
{
  double orientation[4];
  GeoPoint refrPoint;
};


///////////////////////////////////////////////////////////////////////////////
//  class AppearDataRef                                                      //
//  Referenz auf Appearance Daten                                            //
///////////////////////////////////////////////////////////////////////////////
class AppearDataRef
{
public:
  AppearDataRef();
  virtual ~AppearDataRef();

  std::string  m_geoGmlId;
  std::string  m_texCoordGmlId; 
  unsigned int m_texCoordId;
};

///////////////////////////////////////////////////////////////////////////////
//  class Appearances                                                        //
//  Aggregation von CityGML Appearances (nur CityGML)                        //
///////////////////////////////////////////////////////////////////////////////
class Appearances
{
public:
  Appearances( Features * pFeaturesP );
  virtual ~Appearances();

protected:
  Features   * pFeatures;             // Aggregation der Features
  cAppearance * pDefaultAppearance;   // Standard-Appearance
  
  std::map<std::string,unsigned int> m_appearDataIds;  //  Zuordnung gml:id -> Interne Id von AppearancData  
  std::map<std::string,unsigned int> m_texCoordIds;  //  Zuordnung gml:id -> Interne Id von TextureCoordinates


 
  std::map<std::string,unsigned int> m_simpleTextureFrontMap; //  Veraltet: CityGML TexturedSurface
  std::map<std::string,unsigned int> m_simpleTextureBackMap;  //  Veraltet: CityGML TexturedSurface

 
  std::map<std::string,_Geometrie*>     m_geoMap;  //  Zuordnung gml:id -> Geometrieobjekte 
  std::map<_Geometrie*,std::string>     m_inversGeoMap;  //  Zuordnung Geometrieobjekte -> gml:id
  
  std::multimap<unsigned int, unsigned int> m_appearDataAppearRefs;  //  Zuordnung AppearanceData (interne ID) --> Appearance (interne Id) 
  std::multimap<unsigned int, std::string>   m_appearRefs; // Zuordnung Appearance (interne id) -> AppearanceData (gml:id) 
  std::multimap<unsigned int, AppearDataRef> m_appearDataRefs;  //  Zuordnung AppearanceData (interne Id) -> (GeometrieId + TexturKoordinaten Id)

public:
  void resolveReferences();

  void addAppearance( _Geometrie *pRefGeometry, _Geometrie *pCloneGeometry );

  void addAppearanceData ( AppearanceData * pAppearData );

  void addTexCoord ( TexCoord * pTexCoord );
  unsigned int getTexCoordId ( std::string gmlId );

  void addSimpleTexture ( std::string textureMap, unsigned int id, bool orientation );
  unsigned int getSimpleTexture ( std::string textureMap,  bool orientation );

  void addGeometry ( std::string gmlId, _Geometrie * pGeometry );
  _Geometrie * removeGeometry ( std::string gmlId );
  _Geometrie * getGeometry ( std::string gmlId );
  std::string getGmlId ( _Geometrie * pGeometry );

  void addAppearanceRef ( cAppearance * pAppearance, std::string glmId );
	size_t  getAppearanceRefs ( cAppearance * pAppearance, std::vector<unsigned int> & appearDataIds );

  void addAppearanceDataRef ( AppearanceData * pAppearanceData, std::string geoGmlId );
  void addAppearanceDataRef ( AppearanceData * pAppearanceData, std::string geoGmlId, std::string texCoordGmlId );
  void addAppearanceDataRef ( AppearanceData * pAppearanceData, std::string geoGmlId, unsigned int texCoordId );

	size_t getAppearanceDataRefs ( AppearanceData * pAppearanceData, std::vector<AppearDataRef> & appearDataRefs );

  void addAppearDataAppearRef ( unsigned int idAppearData, unsigned int idAppear);
	size_t getAppearDataAppearRefs ( unsigned int idAppearData, std::vector<unsigned int> & idAppears );

	size_t getAllFaces ( _Geometrie * pGeometry, std::vector<PolygonGeo*> &vFaces );
  void createTextureCoordinates (  PolygonGeo * pFace, AppearanceGeoreferencedTexture * pAppearData );
  void createTextureCoordinates (  PolygonGeo * pFace, AppearanceParametrizedTexture * pAppearData, TexCoordGen * pTexCoordGen );
};

