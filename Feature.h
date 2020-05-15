#pragma once
#include <fstream>

#include "Includes.h"
#include "SrsManager.h"

class BLeitPlan;
class PlanBereich;
class FeatureRelation;
class _Geometrie;
class _Surface;
class CurveSegment;
class MultiSurface;
class MultiCurve;
class MultiPoint;
class AttributName;
class GeoPoint;
class Feature;
class GmlKlasse;
class KlassenObjekte;
class SurfaceValuePair;
class MultiSurfaceCoverage;
class GeometryValuePairCoverage;
class GeometryValuePair;
class GmlSchema;
class GmlNamespaces;
class SvgVisualisierung;
class KmlVisualParams;
class _Solid;
class PolygonGeo;
class _Curve;
class cAppearance;
class AppearanceData;
class TexCoord;
struct GeoAppearanceRef;
class TexCoordPolyloop;
class ErrorReport;
class Error;
class AbstractTime;
class AbstractTimeSeries;
class UOM;
class UOMList;
class Logfile;
class Checking;
ref class SrsProjection;

///////////////////////////////////////////////////////////////////////////////
//  Features                                                                 //
//  Aggregation aller Features  							                               //
///////////////////////////////////////////////////////////////////////////////
class Features
{
public:
	Features ( UOMList * pUOMs, SrsManager * pSrsManager, Logfile * pLogfile );
	virtual ~Features();
	
protected:
	UOMList     * m_pUOMs;          //  Modul "Units of Measurement"
  SrsManager  * m_pSrsManager;    //  Modul "Koordinaten-Referenzsysteme"
  Logfile     * m_pLogfile;       //  Logfile

  std::map<std::string,std::string>           featureDateien;  //  map Feature Id --> Datei, in dem das Feature enthalten ist
	std::map<std::string,Feature*>              features;        //  map Feature Id --> Feature Objekt
  std::map<std::string,_Geometrie*>           mGeometryMap;    //  map Geometrie Id --> Geometrie Objekt
  std::map<unsigned int,cAppearance*>         mAppearance;     //  map Appearance Id --> Appearance Objekt (CityGML)
  std::map<unsigned int,AppearanceData*>      mAppearanceData; //  map AppearanceData Id --> Appearance Data Objekt (CityGML)
  std::map<unsigned int,TexCoord*>            mTexCoord;       //  map TexCoord Id --> TexCoord Objekt (CityGML)
  std::multimap<std::string,GeoAppearanceRef> mGeoAppearanceRef; //  map GeoAppearanceRef Id --> GeoAppearanceRef Objekt (CityGML)

  std::map<GML_SCHEMA_TYPE,std::string> m_mExternalCodeListFolders;  // map GML Applikations-Schema --> Ornder der Externen Codelisten
  std::set<std::string>                 shapeFileBooleanValueTrue; //  Bezeichnungen für Boolean-Wert true in Shapefile-Attribut

  std::vector<BLeitPlan*>			       m_vPlaene;       //  Nur XPlanGML: Liste aller vorhandenen Pläne
  std::map<std::string,BLeitPlan*>   m_mPlanMap;      //  Nur XPlanGML: map Plan-Name --> Plan Objekt

  GmlSchema                        * pGmlSchema;      //  Aktuelles GML Applikationsschema
	std::string						             srsName;         //  Name des Koordinaten-Referenzsystems
  int                                srsDimension;    //  Dimension des Koordinaten-Referenzsystems (2 oder 3)
	std::string						             gmlId;           //  GML Id der Gesamtmodells
  bool                               dazuladen;       // Zeigt an, dass mehrere Dateien nacheinander geladen werden

  //  BoreholeML - Visualisierung
  double boreholeRadius;                 //  Nur BoreholeML: Radius eines Bohrlochs bei der Visualisierung
  int    boreholeCylinderComplexity;     //  Nur BoreholeML: Anzahl der Facetten bei der 3D-Visualisierung eines Bohrlochs
  bool   boreholeShowIntervals;          //  Anzeige der Schichten bei der 3D-Visualisierung eines Bohrlochs

//  std::string htmlViewer;                //  Pfadname des HTML- und SVG-Viewers

public:
	UOMList * getUOMList ()  {  return m_pUOMs;  }
  SrsManager * getSrsManager()  {  return m_pSrsManager;  }
  Logfile   * getLogfile()  {  return m_pLogfile;  }

	void addFeature ( Feature * pFeatureP, std::string dateiName = "" );

  int writeFeatures ( std::string fileName );
  int readFeatures ( std::string fileName, bool dazuladen, bool readCodeListDirectories, Checking * pChecking );

  std::string getFeatureDatei ( std::string gmlId );

  void getChildren ( std::multimap<std::string,Feature*> &mChildren, std::set<std::string> &sKlassenNamen );
	size_t getUsedClasses ( std::set<std::string> &sKlassenNamen );

  void         addGeometry ( _Geometrie * pGeometry );
  _Geometrie * getGeometry ( std::string gmlId );
  void         removeGeometry ( std::string gmlId );

  void checkGeometryReferences ( std::vector<Error*> &vErrorReport  );
	void resolveOrientableCurveGeometry();

  void clear();
  void reset();

  bool getDazuladen() { return dazuladen;  }

  void setGmlSchema ( GmlSchema * pGmlSchemaP );
  GmlSchema * exchangeGmlSchema ( GmlSchema * pGmlSchemaeNew );
  GmlSchema * getGmlSchema ()  {  return pGmlSchema; }
  GmlNamespaces * getGmlNamespaces ();
  
  GML_SCHEMA_TYPE getGmlTyp();
  std::string getGmlTypAsString();

  bool isXPlanGML();
  bool isINSPIREPLU();
  bool isINSPIRE();
  bool isCityGML();
  bool isALKIS();
  bool isBoreholeML();

  void setExternalCodeListFolder ( GML_SCHEMA_TYPE gmlTyp, std::string folder );
  std::string getExternalCodeListsFolder( GML_SCHEMA_TYPE gmlTyp );
	size_t getExternalCodeListGmlTypesAsString ( std::vector<std::string> &vStrings );

	void setGmlId ( std::string gmlIdP )  {  gmlId = gmlIdP;  }
	std::string getGmlId()  {  return gmlId;  }

  BLeitPlan * getBLeitPlan ( unsigned int indexP );
  BLeitPlan * getBLeitPlan ( std::string planId );
  PlanBereich * getPlanBereich ( std::string bereichIdP );
  bool createPlanMap();

  bool existGmlId( std::string gmlIdP );
  std::string modifyGmlId ( std::string gmlId );

	size_t getPlaene ( std::vector<BLeitPlan*> &vplaene );
	size_t getPlanAnz()  {  return m_vPlaene.size();  }

	void setSrsName ( std::string name );
  void setSrsNameAndDimension ( std::string name );
	std::string getSrsName ()  {  return srsName;  }

  void setSrsDimension ( int dim ) {  srsDimension = dim;  }
  int getSrsDimension ();
  int getSrsDimensionFromSchemaType ();

  bool existReferenceSystem();

  void modifyAlkisModel();

  void createBMLVisualizationGeometry();

	Feature * getFeature ( std::string objektIdP );
  bool removeFeature ( std::string objId );

	size_t getSortedFeatures ( std::vector<Feature*> &vFeatures, bool sort );
	size_t getFeatures ( std::vector<Feature*> &vFeatures, BLeitPlan * pPlan, PlanBereich * pPlanBereich );

	size_t getFeatures ( std::vector<Feature*> &vFeatures, std::string klassenName );
	size_t getAllFeatures ( std::vector<Feature*> &vFeatures );
	size_t getAllFeatures ( std::multimap<std::string,Feature*> &mFeatures );
	size_t getFeatureAnz ( std::string klassenName );
	size_t getFeatureAnz();

  void offset ( GeoPoint & point );

  bool getBoundingBox (  double * sw, double * no, std::string &srsNameP, int &srsDimension  );

	void updateGmlIds();

  void addAppearance( cAppearance * pAppearance );
  void addAppearanceData( AppearanceData * pAppearanceData );
  void addTexCoord( TexCoord * pTexCoord );

  TexCoord * getTexCoord ( unsigned int id );
  cAppearance * getAppearance ( unsigned int id );
  AppearanceData * getAppearanceData ( unsigned int id );

	size_t getAllThemes ( std::set<std::string> &sThemes );
	size_t getAllAppearances( std::vector<cAppearance*> & appearances );
	size_t getAllAppearanceData( std::vector<AppearanceData*> & appearData );

  void addGeoAppearanceMaterial( std::string geomId, std::string thema, unsigned int appearDataId, bool overwrite );
  void addGeoAppearanceTexture( std::string geomId, std::string thema, unsigned int idAppear, unsigned int idCoords );

	size_t getGeoAppearance( std::string geomId, std::vector<std::string> &thema, std::vector<AppearanceData*> & appearData );
	size_t getGeoAppearanceColor( std::string geomId, std::vector<std::string> &thema, std::vector<AppearanceData*> & appearData );
	size_t getGeoAppearanceTexture( std::string geomId, std::vector<std::string> &thema, 
                               std::vector<AppearanceData*> & appearData, std::vector<TexCoordPolyloop*> & texCoord );

	size_t getLODStufenAsString ( std::vector<std::string> &vLodStufen );
	size_t getLODStufen ( std::vector<CITYGML_LOD> &vLodStufen );
  bool hasAppearances ();

  static std::string lodStufeToString ( CITYGML_LOD lodStufe );
  static CITYGML_LOD lodStufeToEnum ( std::string lodStufe );
  static bool        isLodProperty ( std::string geometryProperty, CITYGML_LOD lodStufe );

  Feature * createCityGMLRepresentationContext ( std::string contextIdentifier, std::string namespaceKuerzel, std::string contextName = "", double precision = 0.0 );

  Feature * createFeature ( std::string klassenName, Feature * pParentFeature, std::string featurePropertyName );
  Feature * createFeature ( std::string klassenName, Feature * pParentFeature,
                            std::string featurePropertyName, std::string gmlId );

  void addShapeFileBooleanValueTrue ( std::string v )  {  shapeFileBooleanValueTrue.insert( v );  }
  bool interpretBooleanAttribute ( std::string v );

  void setBoreholeRadius ( double r )  {  boreholeRadius = r;  }
  void setBoreholeCylinderComplexity ( int n )  {  boreholeCylinderComplexity = n;  }
  void setBoreholeShowIntervals ( bool b )  {  boreholeShowIntervals = b;  }

//  void setHTMLViewer ( std::string nameP )  {  htmlViewer = nameP;  }
//  std::string getHTMLViewer()  {  return htmlViewer;  }
};

///////////////////////////////////////////////////////////////////////////////
//  FeatureList                                                              //
//  Liste von Feature-Objekten eines bestimmten Typs                         //
///////////////////////////////////////////////////////////////////////////////
class FeatureList
{
  public:
    FeatureList();
    FeatureList( FeatureList * pListOld );
    virtual ~FeatureList();

  protected:
    std::vector<Feature*> vFeatures;      //  Liste der Feature Objekte
    std::string           klassenName;    //  Klassen-Name der Objekte
    std::string           relationName;   //  Name der Relation, über die die Objekte von einem anderen Feature referiert werden
    std::string           featureTypName; // FeatureType Name der Objekte
    GmlKlasse           * pFeatureGmlTyp; // FeatureType Objekt 

  public:
    void setKlassenName ( std::string name ) {  klassenName = name; }
    std::string getKlassenName()  {  return klassenName; }

    void setRelationName ( std::string name )  {  relationName = name;  }
    std::string getRelationName()  {  return relationName; }

    void setGmlTyp ( GmlKlasse * pKlasse )  {  pFeatureGmlTyp = pKlasse;  }
    GmlKlasse  * getGmlTyp()  {  return pFeatureGmlTyp; }

    Feature * get ( size_t index );

		size_t count();

		size_t add ( Feature * pFeature );
		size_t addCopy ( Feature * pFeature );
    
		size_t remove ( size_t index );
    void removeAll();
};

///////////////////////////////////////////////////////////////////////////////
//  Attr                                                                     //
//  Basisklasse für die Attributwerte eines Features                         //
///////////////////////////////////////////////////////////////////////////////
class Attr
{
public:
  Attr();
  Attr ( Attr * pAttrOld );
  virtual ~Attr();

public:
  bool                              isNil;      // Gibt an, ob das Attribut "nilable" ist
  std::map<std::string,std::string> classifier; //  map der Classifier-Attribute des Feature Attributs (Attributname --> Attributwert)

public:
  std::string getNilReason();
  void setNilReason( std::string nilReason );
};

///////////////////////////////////////////////////////////////////////////////
//  StringAttr                                                               //
//  Textlicher Attribut-Wert eines Features                                  //
///////////////////////////////////////////////////////////////////////////////
class StringAttr: public Attr
{
public:
  StringAttr();
  StringAttr ( StringAttr * pAttrOld );
  virtual ~StringAttr();

public:
  std::string              value;    // Attributwert
};

///////////////////////////////////////////////////////////////////////////////
//  DoubleAttr                                                               //
//  Double Attribut-Wert eines Features                                      //
///////////////////////////////////////////////////////////////////////////////
class DoubleAttr: public Attr
{
public:
  DoubleAttr();
  DoubleAttr ( DoubleAttr * pAttrOld );
  virtual ~DoubleAttr();

public:
  double value;   //  Attributwert
  UOM  * uom;    //  Unit of Measure bei Measure Attributen
};

///////////////////////////////////////////////////////////////////////////////
//  IntAttr                                                                  //
//  Integer Attribut-Wert eines Features                                     //
///////////////////////////////////////////////////////////////////////////////
class IntAttr: public Attr
{
public:
  IntAttr();
  IntAttr ( IntAttr * pAttrOld );
  virtual ~IntAttr();

public:
  int value;  //  Attributwert
};

///////////////////////////////////////////////////////////////////////////////
//  BoolAttr                                                                 //
//  Boolean Attribut-Wert eines Features                                     //
///////////////////////////////////////////////////////////////////////////////
class BoolAttr: public Attr
{
public:
  BoolAttr();
  BoolAttr ( BoolAttr * pAttrOld );
  virtual ~BoolAttr();

public:
  bool value;  //  Attributwert
};

///////////////////////////////////////////////////////////////////////////////
//  DateAttr                                                                 //
//  Datum / Uhrzeit Attribut-Wert eines Features                             //
///////////////////////////////////////////////////////////////////////////////
class DateAttr: public Attr
{
public:
  DateAttr();
  DateAttr ( DateAttr * pAttrOld );
  virtual ~DateAttr();

public:
  AbstractTime * value;  //  Attributwert
};

///////////////////////////////////////////////////////////////////////////////
//  Feature																                                   //
//  Raumbezogenes, semantisches GML-Objekt (Feature)                         //
///////////////////////////////////////////////////////////////////////////////
class Feature
{
public:
  Feature ( OBJEKT_TYP objektTyp, std::string gmlIdP, Features * pFeaturesP, std::string bPlanKlassenNameP, std::string featureMemberNameP = "" );
	Feature ( Feature * pObjOld, std::string gmlIdP );
	virtual ~Feature();

protected:
	Features    * pFeatures;    //  Aggregation der Features
	std::string   bereichId;    //  Nur XPlanGML: Id des Bereichs, zu dem das Feature gehört

	OBJEKT_TYP objektTyp;       //  Typ des Features

	std::string klassenName;         //  Name der Feature Klasse
  std::string featureMemberName;   //  Name des FeatureMembers, unter dem das Feature in einer FeatureCollection referiert wird
	std::string gmlId;               //  Id des Features
  std::string srsName;             //  Name der zugehörigen Koordinaten-Referenzsystems
  int         srsDimension;        //  Dimension des zugehörigen Koordinaten-Referenzsystems

  std::string           parentId;     //  Id des übergeordneten Features im Feature-Baum
  std::vector<std::string> vChildIds; //  Ids der untergeordneten Features im Feature-Baum

  bool         isSimple;            //  Gibt an, ob die XML-Repräsentation des Features ein Simple Element ist
  StringAttr * pSimpleContent;      //  Inhalt eines Simple Element Features

	std::multimap<std::string,StringAttr*>   stringAttribute;  // Feature-Attribute vom Typ Text
	std::multimap<std::string,IntAttr*>	     integerAttribute; // Feature-Attribute vom Typ Integer
	std::multimap<std::string,DoubleAttr*>   doubleAttribute;  // Feature-Attribute vom Typ Double
	std::map<std::string,BoolAttr*>          boolAttribute;    // Feature-Attribute vom Typ Boolean 
	std::multimap<std::string,DateAttr*>     dateAttribute;    // Feature-Attribute vom Typ Date
	std::multimap<std::string,StringAttr*>   urlAttribute;     // Feature-Attribute vom Typ URL
	std::multimap<std::string,FeatureRelation*> objRelationen; // Relationen mit anderen Features
  std::multimap<std::string,_Geometrie*> mGeometrie;         // Geometrie-Attribute des Features
	std::vector<GeoPoint*>                 symbolPositionen;   // Nur XPlanGML, veraltet

  std::multimap<std::string,AbstractTimeSeries*> mTimeSeries; // Vom Feature referierte Zeitreihen

public:
	virtual OBJEKT_TYP     getObjektTyp()  {  return objektTyp;  }
	GEOMETRY_TYPE          getGeometrieTyp( std::string geometryPropertyName = "" );
  ABSTRACT_GEOMETRY_TYPE getAbstractGeometrieTyp( std::string geometryPropertyName = "" );
	virtual bool           isSimplyConnected () {  return false;  }
  virtual bool           hasOwnGeometry();
  virtual bool           hasRelatedGeometry();         

	Features * getFeatures ()  {  return pFeatures;  }
  void       setFeatures ( Features * pFeaturesP )  {  pFeatures = pFeaturesP;  }

	void setGmlId( std::string gmlIdP )  {  gmlId = gmlIdP;  }
	std::string getGmlId ()  {  return gmlId;  }

  void setParentId ( std::string parentIdP );
  void resetParent ();

  void setSrsName ( std::string name );
  void setSrsNameAndDimension ( std::string name );
  std::string getSrsName ();

  void setSrsDimension ( int dim ) {  srsDimension = dim;  }
  int getSrsDimension ();

  bool coordTransformation ( std::string newCoordSystem );
  bool coordTransformation ( std::string oldCoordSystem, std::string newCoordSystem );
  bool coordTransformation ( std::string oldCoordSystem, std::string newCoordSystem, SrsProjection ^ pSrsProjection, _Geometrie * pGeometrie );

  std::string getParentId()  {  return parentId;  }
  Feature   * getParent();
	size_t      getParents ( std::vector<Feature*> &vParents );
  Feature   * getRootFeature();

  Feature * addNewChildFeature ( std::string className, std::string propertyName, OBJEKT_TYP featureType = ANY_OBJEKT );
  void addRelatedFeature ( Feature * pChildFeature, std::string propertyName, bool relToFeature );

  Feature * getCityGMLRepresentation (  Feature * pRepContext, std::string namespaceKuerzel );
  virtual void addCityGMLGeometryRepresentation ( Feature * pRepContext, _Geometrie *pGeo, std::string namespaceKuerzel );
  virtual void addCityGMLGeometryRepresentationWithContext ( _Geometrie *pGeo, std::string namespaceKuerzel,  std::string contextIdentifier, 
                                                              std::string contextName, double precision = 0.0 );
  virtual void deleteCityGMLGeometryRepresentation ( std::string repContextIdentifier );

  void addChild ( std::string childId );

	size_t getChildren ( std::multimap<std::string,Feature*> &mChildren, std::set<std::string> &sKlassenNamen );
	size_t getChildrenIds ( std::vector<std::string> &vChildrenIds );
	size_t getChildrenIdsRecursive ( std::set<std::string> &vChildrenIds );

  bool removeChild ( std::string childId );
  void removeAllChildren();

	void setKlassenName ( std::string klassenNameP )  {  klassenName = klassenNameP;  }
	std::string getKlassenName ()  {  return klassenName;  }

  std::string getFeatureMemberName()  {  return featureMemberName;  }

	void setBereichId ( std::string idP )  {  bereichId = idP;  }
	PlanBereich * getPlanBereich (); 
  void propagateBereichId();

  void getPropertyStatistics ( std::map<std::string,std::vector<int>> &mPropertyStatistics );

	void setStringAttribut ( std::string attributNameP, std::string attributWertP );
  void setStringAttribut ( std::string attributNameP, StringAttr * pStringAttr );
	void setStringAttribut ( std::string attributNameP, std::string attributWertP, 
		                     std::string classifierNameP, std::string classifierValueP );
  void setStringAttribut ( std::string attributNameP, std::string attributWertP, 
                           std::vector<std::string> classifierNameP, std::vector<std::string> classifierValueP );
  void exchangeStringAttribut ( std::string attributNameP, std::string attributWertAltP, std::string attributWertNeuP,
                                std::vector<std::string> classifierNameP, std::vector<std::string> classifierValueP );
  void exchangeStringAttribut ( std::string attributNameP, std::string attributWertAltP, std::string attributWertNeuP );

	void setIntegerAttribut ( std::string attributNameP, int attributWertP );
  void setIntegerAttribut ( std::string attributNameP, IntAttr * pIntAttr );
  void setIntegerAttribut ( std::string attributNameP, int attributWertP, 
                            std::string classifierNameP, std::string classifierValueP );
  void setIntegerAttribut ( std::string attributNameP, int attributWertP, 
                            std::vector<std::string> classifierNameP, std::vector<std::string> classifierValueP );
  void exchangeIntegerAttribut ( std::string attributNameP, std::string attributWertAltP, int attributWertNeuP,
                                 std::vector<std::string> classifierNameP, std::vector<std::string> classifierValueP );
  void exchangeIntegerAttribut ( std::string attributNameP, int attributWertAltP, int attributWertNeuP );


	void setDoubleAttribut ( std::string attributNameP, double attributWertP );
  void setDoubleAttribut ( std::string attributNameP, double attributWertP, UOM * pUOM );
  void setDoubleAttribut ( std::string attributNameP, DoubleAttr * pDoubleAttr );
	void setDoubleAttribut ( std::string attributNameP, double attributWertP, 
		                       std::string classifierNameP, std::string classifierValueP, UOM * pUOM );
  void setDoubleAttribut ( std::string attributNameP, double attributWertP, 
                           std::vector<std::string> classifierNameP, std::vector<std::string> classifierValueP, UOM * pUOM );
  void exchangeDoubleAttribut ( std::string attributNameP, std::string attributWertAltP, double attributWertNeuP,
                                std::vector<std::string> classifierNameP, std::vector<std::string> classifierValueP );
  void exchangeDoubleAttribut ( std::string attributNameP, double attributWertAltP, double attributWertNeuP, UOM * pUOM );

	void setBoolAttribut ( std::string attributNameP, bool attributWertP );
  void setBoolAttribut ( std::string attributNameP, BoolAttr * pBoolAttr );
  void setBoolAttribut ( std::string attributNameP, bool attributWertP, 
                         std::string classifierNameP, std::string classifierValueP );
  void setBoolAttribut ( std::string attributNameP, bool attributWertP, 
                         std::vector<std::string> classifierNameP, std::vector<std::string> classifierValueP );

	void setDateAttribut ( std::string attributNameP, DateAttr * pDateTime );
  void exchangeDateAttribut ( std::string attributNameP, DateAttr * pDateTimeOld, DateAttr * pDateTimeNew );


	void setURLAttribut ( std::string attributNameP, std::string attributWertP );
  void setURLAttribut ( std::string attributNameP, StringAttr * pURLAttr );
  void setURLAttribut ( std::string attributNameP, std::string attributWertP, 
                        std::string classifierNameP, std::string classifierValueP );
  void setURLAttribut ( std::string attributNameP, std::string attributWertP, 
                        std::vector<std::string> classifierNameP, std::vector<std::string> classifierValueP );
  void exchangeURLAttribut ( std::string attributNameP, std::string attributWertAltP, std::string attributWertNeuP,
                             std::vector<std::string> classifierNameP, std::vector<std::string> classifierValueP );

  void exchangeAttributWert ( ATTRIBUT_TYP attributTyp, std::string attributName, std::string attributWertAlt,std::string attributWertNeu,
                              std::vector<std::string> classifierNameP, std::vector<std::string> classifierValueP );

	void deleteStringAttribut ( std::string attributName, std::string attributWert = "" );
	void deleteIntegerAttribut ( std::string attributName, std::string attributWert = ""  );
	void deleteDoubleAttribut ( std::string attributName, std::string attributWert = "" );
	void deleteBoolAttribut ( std::string attributName );
	void deleteDateAttribut ( std::string attributName );
	void deleteURLAttribut ( std::string attributName, std::string attributWert = ""  );
  void deleteAttributWert ( ATTRIBUT_TYP attributTyp, std::string attributName, std::string attributWert = "" );

	bool getStringAttributWert ( std::string attributName, std::string & attributWert );
  StringAttr* getStringAttributWert ( std::string attributName );
	size_t getStringAttributWerte ( std::string attributName, std::vector<std::string> & vStringP );
	size_t getStringAttributWerte ( std::string attributName, std::vector<StringAttr*> & vAttributWerteP );
	size_t getStringAttributWerte ( std::vector<std::string> &vAttributNamen, std::vector<StringAttr*> & vAttributWerteP );
	size_t getStringAttributWertAnz ( std::string attributName );


	bool getIntegerAttributWert ( std::string attributName, int & attributWert );
  IntAttr * getIntegerAttributWert ( std::string attributName );
	size_t getIntegerAttributWerte ( std::string attributName, std::vector<int> & vIntP );
	size_t getIntegerAttributWerte ( std::string attributName, std::vector<IntAttr*> & vAttributWerteP );
	size_t getIntegerAttributWerte ( std::vector<std::string> &vAttributNamen, std::vector<IntAttr*> & vAttributWerteP );
	size_t getIntegerAttributWertAnz ( std::string attributName );


  bool getDoubleAttributWert ( std::string attributName, double & attributWert );
	DoubleAttr * getDoubleAttributWert ( std::string attributName );
	size_t getDoubleAttributWerte ( std::string attributName, std::vector<double> & vAttributWerteP );
	size_t getDoubleAttributWerte ( std::string attributName, std::vector<DoubleAttr*> & vAttributWerteP );
	size_t getDoubleAttributWerte ( std::vector<std::string> &vAttributNamen, std::vector<DoubleAttr*> & vAttributWerteP );
	size_t getDoubleAttributWertAnz ( std::string attributName );


	bool getBoolAttributWert ( std::string attributName, bool & attributWert );
  BoolAttr * getBoolAttributWert ( std::string attributName );

	DateAttr * getDateAttributWert ( std::string attributName );
	size_t getDateAttributWerte ( std::string attributName, std::vector<DateAttr*> & vAttributWwerte );
	size_t getDateAttributWerte ( std::vector<std::string> &vAttributNamen, std::vector<DateAttr*> & vAttributWerteP );
	size_t getDateAttributWertAnz ( std::string attributName );


	bool getURLAttributWert ( std::string attributName, std::string & attributWert );
  StringAttr* getURLAttributWert ( std::string attributName );
	size_t getURLAttributWerte ( std::string attributName, std::vector<std::string> & vStringP  );
	size_t getURLAttributWerte ( std::string attributName, std::vector<StringAttr*> & vAttributWerteP );
	size_t getURLAttributWerte ( std::vector<std::string> &vAttributNamen, std::vector<StringAttr*> & vAttributWerteP );
	size_t getURLAttributWertAnz ( std::string attributName );


	bool getGenericStringAttributWert ( std::string attributName, std::string & attributWert);
	bool getGenericIntegerAttributWert ( std::string attributName, int & attributWert );
	bool getGenericDoubleAttributWert ( std::string attributName, double & attributWert, UOM *& pUOM );
	DateAttr * getGenericDateAttributWert ( std::string attributName );
	bool getGenericURLAttributWert ( std::string attributName, std::string & attributWert );

	bool getAttributWert ( ATTRIBUT_TYP attributTyp, std::string attributName, std::string & attributWert, UOM *& pUOM  );

  bool getReferenzWert ( std::string referenziertesElement, std::string attributName, 
                         ATTRIBUT_TYP & referenzAttributTyp, std::string & attributWert, UOM *& pUOM );

	size_t getAttributWerte ( ATTRIBUT_TYP attributTyp, std::string attributName, 
				          		   std::vector<std::string> & vStringP, 
                         std::vector<std::string> & vCodeSpaces, 
                         std::vector<UOM*>        & vUOMs );

	size_t getAttributWerte ( std::vector<ATTRIBUT_TYP> & vAttributTyp, 
                         std::vector<std::string> & vAttributName, 
                         std::vector<std::string> & vAttributWerte,
                         std::vector<UOM*>        & vUOMs );

	size_t getAttributWerte ( std::vector<ATTRIBUT_TYP> & vAttributTyp, 
                         std::vector<std::string> & vAttributName, 
                         std::vector<std::string> & vAttributWerte,
                         std::vector<size_t>      & vClassifierAnz,
                         std::vector<std::string> & vClassifierNamen,
                         std::vector<std::string> & vClassifierValues,
                         std::vector<UOM*>        & vUOMs );

	size_t getAttributWerteAnz ( ATTRIBUT_TYP attributTyp, std::string attributName );
	size_t getAttributWerteAnz ( std::string attributName );

  void mergeAttribute ( Feature * pPlanObjekt );
  void uebernehmeAttribute ( Feature * pPlanObjekt );
  void uebernehmeSachdatenAttribute ( Feature * pFeature );

	bool getFlaechenschluss ();

	virtual void addFeatureRelation ( FeatureRelation * pRelation );

	FeatureRelation * removeFeatureRelation (  Feature * pObjP );
  bool removeFeatureRelation (  FeatureRelation * pRelation );

  void deleteFeatureRelation (  Feature * pObjP );
  void deleteFeatureRelationen ( std::string rollenNameP );

	size_t getFeatureRelationenAnz()  {  return objRelationen.size();  }
	size_t getFeatureRelationen ( std::vector<FeatureRelation*> & objRelationenP );
  FeatureRelation * getFeatureRelation ( Feature * pObjP );

	size_t getFeatureRelationen ( std::string rollenNameP, std::vector<FeatureRelation*> & objRelationenP );
	size_t getFeatureRelationenAnz ( std::string rollenNameP );

	size_t getRelatedFeatures ( std::string rollenNameP, std::vector<Feature*> & vFeatures );
  Feature * getRelatedFeature ( std::string rollenNameP );
  Feature * getRelatedFeature ( FeatureRelation * pRel );

	int checkFeatureIdsExist();

  virtual void addGeometryProperty ( std::string propertyName, _Geometrie * pGeometrie );

  _Geometrie * getGeometry ( std::string propertyName, size_t index  );
	size_t getGeometry ( std::vector<_Geometrie*> &vGeometrie, CITYGML_LOD lodStufe = ALL );

  virtual size_t getGeometryProperties ( std::vector<std::string> &vPropertyNames, std::vector<_Geometrie*> &vGeometrie );
	virtual size_t getGeometryPropertyAnz ( std::string geometryPropertyNameP );
  virtual size_t getGeometryPropertyAnz ( )  {  return mGeometrie.size();  }

  void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );

  void deleteGeometryProperty( std::string propertyName );

  virtual void addShapePolygons2D ( std::string propertyName, std::vector<double> xCoordP, std::vector<double> yCoordP, 
	                                	std::vector<int> ringOffsetsP, std::vector<int> ringTypenP, std::string srsName );
	virtual void addShapeLines2D ( std::string propertyName, std::vector<double> xCoordP, std::vector<double> yCoordP, 
				                         std::vector<int> partOffsetsP, std::string srsName );
	virtual void addShapePoints2D ( std::string propertyName, std::vector<double> xCoordP, std::vector<double> yCoordP, 
			                          	std::string srsName );

  virtual size_t getVolumenGeometrie ( std::string gmlProperty, std::vector<_Solid*> &vSolids );
  virtual size_t getFlaechenGeometrie ( std::string gmlProperty, std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getLinienGeometrie ( std::string gmlProperty, std::vector<_Curve*> &vCurves );
  virtual size_t getPunktGeometrie ( std::string gmlProperty, std::vector<GeoPoint*> &vPoints );
  virtual size_t getGeometrie ( std::string gmlProperty, std::vector<_Geometrie*> &vGeometries );

  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual bool getBoundingBox ( double * sw, double * no, std::string &srsName, int &srsDimension, bool recursive );

	virtual void addSymbolPosition ( GeoPoint * pPointP );
	virtual size_t getSymbolPositionen ( std::vector<GeoPoint*> &symbolPositionenP );
	size_t getSymbolPositionenAnz ()  {  return symbolPositionen.size();  }
	void deleteSymbolPositionen();

  void setIsSimple ( bool b )  {  isSimple = b;  }
  bool getIsSimple ()  {  return isSimple;  }

  void setSimpleContent ( StringAttr * pSimpleContent );
  StringAttr * getSimpleContent()  {  return pSimpleContent; }

  bool addTimeSeries ( std::string propertyName, AbstractTimeSeries * pTimeSeries );
  AbstractTimeSeries * getTimeSeries ( std::string propertyName );
	size_t getAllTimeSeries ( std::string propertyName, std::vector<AbstractTimeSeries*> &vTimeSeries );

  protected:
    std::string getCityGMLRepresentationItemName ( _Geometrie * pGeo );
};


///////////////////////////////////////////////////////////////////////////////
//  BLeitPlan                                                                //
//  Nur XPlanGML: Klasse für einen XPlanGML-Plan                        		 //
///////////////////////////////////////////////////////////////////////////////
class BLeitPlan: public Feature
{
public:
  BLeitPlan ( Features * pFeaturesP, std::string gmlIdP, std::string bPlanKlassenNameP );
	virtual ~BLeitPlan();

protected:
	XPLAN_TYP                         planTyp;              // XPlanGML Plan-Typ
	std::map<int,PlanBereich*>        bereiche;             // map Plan-Bereich-Nummer --> Plan-Bereich Objekt
	std::map<std::string,Feature*> textlicheFestsetzungen;  // Textliche Festsetzungen
	std::map<std::string,Feature*> textlicheBegruendungen;  // Textliche Begründungen

public:
	XPLAN_TYP getPlanTyp() {  return planTyp;  }

	size_t getPlanBereiche ( std::vector<PlanBereich*> & vPlanBereiche );
	PlanBereich * getPlanBereich ( int bereichNummerP );

	virtual void addFeatureRelation ( FeatureRelation * pRelation );
	void createTextMaps();

	size_t getTextlicheFestsetzungen ( std::vector<Feature*> &objekteP );
	size_t getVerfahrensMerkmale ( std::vector<Feature*> &merkmaleP );

	Feature * getTextlicheFestsetzung ( std::string schluessel );
	Feature * getTextlicheBegruendung ( std::string schluessel );

	void addPlanBereich ( std::string planBereichIdP );
	void removeObjekt ( Feature * pObjP );

	virtual std::string getSrsName ();
};


///////////////////////////////////////////////////////////////////////////////
//  PlanBereich                                                              //
//  Nur XPlanGML: Klasse für eine Ebene eines XPlanGML-Plans         				 //
///////////////////////////////////////////////////////////////////////////////
class PlanBereich: public Feature
{
public:
  PlanBereich ( Features * pFeaturesP, std::string gmlIdP, std::string bPlanKlassenNameP );
	virtual ~PlanBereich();

private:
	std::map<std::string,KlassenObjekte*> bPlanKlassen;  //  map Klassen-Name --> XPlanGML Objekte dieser Klasse
  BLeitPlan                           * pBLeitPlan;    //  Zugehöriges XPlanGML Plan-Objekt

public:
	void  setBPlan ( BLeitPlan * pBLeitPlanP );
	BLeitPlan * getBPlan ();

	int getBereichNummer();

	size_t getBPlanObjekte ( std::vector<Feature*> & vplanObjekte,	bool allObjectsP );

	size_t getKlassen ( std::vector<KlassenObjekte*> &vKlassenP );
	KlassenObjekte * getKlasse ( std::string & klassenName );

//	size_t getObjektAnz(); 

	void addObjekt ( Feature * pObjP );
	void removeObjekt ( Feature * pObjP );

//	void addObjektId ( std::string objIdP );
	void resolveObjektIds( BLeitPlan * pPlan );

	virtual bool getBoundingBox ( double * sw, double * no, std::string &srsNameP, int &srsDimensionP );
	virtual std::string getSrsName ();
};

///////////////////////////////////////////////////////////////////////////////
//  FeatureRelation													                              	 //
//  Relation eines Features mit einem anderen Feature				                 //
///////////////////////////////////////////////////////////////////////////////
class FeatureRelation
{
public:
	FeatureRelation();
  FeatureRelation ( FeatureRelation * pRelOld );
	virtual ~FeatureRelation();

public:
	std::string              rollenName;
	std::string              featureId;
	std::string              uuid;
	bool		                 relToFeature;
  bool                     nil;
  std::string              nilReason;
  std::vector<std::string> classifierName;
  std::vector<std::string> classifierValue;
};


///////////////////////////////////////////////////////////////////////////////
//  KlassenObjekte                                                           //
//  Nur XPlanGML: Zusammenfassung von Features einer Klasse             		 //
///////////////////////////////////////////////////////////////////////////////
class KlassenObjekte
{
	public:
		KlassenObjekte ( Feature * pBPlanObjektP );
		virtual ~KlassenObjekte();

	private:
		std::string				    klassenName;  // Klassen-Name
		OBJEKT_TYP				    objektTyp;    // Objekt-Typ
		bool					        showKlasse;   // Steuerung, ob die zugehörigen Features in der Szene gezeigt werden
		int						        priority;     // Steuerung der Reihenfolge, mit der Features in eine SVG-Datei geschrieben werden
		std::vector<Feature*> objekte;      // Liste der Features

	public:
		std::string getKlassenName()  {  return klassenName;  }
		OBJEKT_TYP  getObjektTyp()  {  return objektTyp;  }

		void setShowKlasse ( bool b )  { showKlasse = b;  }
		bool getShowKlasse ()  {  return showKlasse;  }

		void setPriority ( int prioP )  {  priority = prioP;  }
		int  getPriority ()  {  return priority;  }

		void addObjekt ( Feature * objP );
		void removeObjekt ( Feature * objP );

		size_t getObjekte ( std::vector<Feature*> & objekteP );
		size_t getObjektAnz();
};

