#pragma once
#include "Includes.h"


class OSM_Node;
class OSM_Way;
class OSM_Area;
class OSM_Object;
class OSM_Relation;
class OSM_Statistics;
class OSM_StatisticsAll;
class OSM_VisualParams;
class OSM_SemanticClassification;
class MultiCurve;
class PolygonGeo;
class OSM_SingleTagFilter;
class OSM_TagFilter;
class Logfile;
class SrsManager;

///////////////////////////////////////////////////////////////////////////////
//  OpenStreetmap Modell                                                     //
///////////////////////////////////////////////////////////////////////////////
class OSM_Model
{
public:
  OSM_Model( Logfile * pLogfile, SrsManager * pSrsManager, float planausgabeBreite, 
            float planausgabeHoehe, double globalScale );
  virtual ~ OSM_Model();

protected:
  Logfile                    * m_pLogfile;
  SrsManager                 * m_pSrsManager;
  OSM_SemanticClassification * pOSMSemanticClassification;

  std::map<std::string,OSM_Node*>     mNodes;
  std::map<std::string,OSM_Way*>      mWays;
  std::map<std::string,OSM_Relation*> mRelations;

  std::map<std::string,OSM_Relation*>::iterator iterRel;
  std::map<std::string,OSM_Node*>::iterator     iterNodes;
  std::map<std::string,OSM_Way*>::iterator      iterWays;
   
  std::vector<std::string> vIncompleteWayIds;
  std::vector<std::string> vIncompleteRelationIds;

  OSM_StatisticsAll          * pOSM_StatisticsAll;
  OSM_VisualParams           * pOSM_SvgParams;

  float  planausgabeBreite;
  float  planausgabeHoehe;
  double globalScale;  // Skalierungsfaktor für Anpassung der Signaturen an unterschiedliche Maßstäbe
  double delta;

  std::string m_svgViewer;
  std::string svgUeberschriftDatei;
  std::string svgUebersichtDatei;
  std::string svgKartenDatei;

  bool         m_objectDataGenerated;

  double minLat;
  double minLon;
  double maxLat;
  double maxLon;

public:
  SrsManager * getSrsManager()  {  return m_pSrsManager;  }

  void setPlanausgabeBreite ( float b )  {  planausgabeBreite = b;  }
  void setPlanausgabeHoehe ( float h )  {  planausgabeHoehe = h;  }
  void setGlobalScale ( double scale  )  {  globalScale = scale;  }
  void setDelta ( double d )  {  delta = d;  }


  void addOSM_Node ( OSM_Node * pNode );
  OSM_Node * getOSM_Node ( std::string id );

  void addOSM_Way ( OSM_Way * pWay );
  OSM_Way * getOSM_Way ( std::string id );

  void addOSM_Relation ( OSM_Relation * pRelation );
  OSM_Relation * getOSM_Relation ( std::string id );

  OSM_Relation * getFirstOSM_Relation();
  OSM_Relation * getNextOSM_Relation();

  OSM_Node * getFirstOSM_Node();
  OSM_Node * getNextOSM_Node();

  OSM_Way * getFirstOSM_Way();
  OSM_Way * getNextOSM_Way();

  void setBounds ( double minLatP, double minLonP, double maxLatP, double maxLonP );
  void getBounds ( double &minLatP, double &minLonP, double &maxLatP, double &maxLonP );

	size_t getNodeAnz()      {  return mNodes.size();  }
	size_t getWayAnz()       {  return mWays.size();  }
	size_t getRelationAnz()  {  return mRelations.size();  }

	size_t getOSM_Objects ( std::vector<OSM_Object*> &vOSM_Objects );
	size_t getOSM_ObjectsSorted ( std::vector<OSM_Object*> &vOSM_Objects );

	size_t getLineGeometry ( OSM_Way * pWay, std::vector<double> &vLat, std::vector<double> &vLon );
	size_t getSurfaceGeometry ( OSM_Way * pWay, std::vector<double> &vLat, std::vector<double> &vLon );

  OSM_StatisticsAll * getOSM_Statistics();

  void addErrorMessage ( std::string errorMessage );

  void checkReferences();
  void checkAndCorrect();

	size_t getIncompleteWayAnz()  {  return vIncompleteWayIds.size();  }
	size_t getIncompleteRelationAnz()  {  return vIncompleteRelationIds.size();  }

	int readOsmModel ( System::Xml::XmlTextReader ^ pXmlReader, OSM_StatisticsAll * pOSM_Statistics );
	size_t readOsmModelWithFilter ( OSM_TagFilter * pFilter, std::string fileName, System::Windows::Forms::ListBox ^listBoxMeldungen );

  void resetOSM_Model();

  void setSvgViewer ( std::string nameP )  {  m_svgViewer = nameP;  }
  std::string getSvgViewer()  {  return m_svgViewer;  }

  OSM_VisualParams * getOSM_SvgParams();
  bool writeOSM_SVG ();

  void setObjectDataGenerated ( bool b )  {   m_objectDataGenerated = b;}
  bool getObjectDataGenerated ()  {  return  m_objectDataGenerated;  }

private:
  OSM_Statistics * generateNodeStatistics();
  OSM_Statistics * generateWayStatistics();
  OSM_Statistics * generateRelationStatistics();
};

///////////////////////////////////////////////////////////////////////////////
//  OpenStreetmap Basisobjekt                                                //
///////////////////////////////////////////////////////////////////////////////
class OSM_Object
{
public:
  OSM_Object ( std::string idP );
  virtual ~ OSM_Object();

protected:
  std::string                       id;
  std::string                       classification;
  std::map<std::string,std::string> mTags;
  std::vector<OSM_Relation*>        mUsedInRelation;

public:
  std::string getId()  {  return id;  }

  virtual OSM_OBJECT_TYPE getType() = NULL;
  virtual std::string getTypeString() = NULL;

  void addTag ( std::string key, std::string value );
//  void addTags ( std::map<std::string,std::string> &mTags );

  std::map<std::string,std::string> & getTags ()  {  return mTags;  }
  std::string getTagValue ( std::string key );
	size_t getTagAnz()  {  return mTags.size();  }

  void transferTags ( OSM_Object * pOSM_Object );

  void clearTags();

  void setClassification ( std::string classificationP )  {  classification = classificationP;  }
  std::string getClassification()  {  return classification;  }
  virtual std::string getClassificationWithGeometry() = NULL;

  void addRelation ( OSM_Relation * pRel )  {  mUsedInRelation.push_back( pRel ); }
	size_t getRelationAnz()  {  return mUsedInRelation.size();  }
  OSM_Relation * getRelation ( unsigned int index );

};

///////////////////////////////////////////////////////////////////////////////
//  OpenStreetmap Node-Objekt                                                //
///////////////////////////////////////////////////////////////////////////////
class OSM_Node: public OSM_Object
{
public:
  OSM_Node( std::string idP, double lat, double lon );
  virtual ~OSM_Node();

protected:
  double                lat;
  double                lon;
  std::set<std::string> sRefIds;

public:
  virtual OSM_OBJECT_TYPE getType() {  return OSM_NODE;  }
  virtual std::string getTypeString()  {  return "_PO_";   }

  virtual std::string getClassificationWithGeometry() {  return classification + "_node"; }

 
  void addReference ( OSM_Object * pObj );

  void getGeometry ( double &latP, double &lonP );

  double getLat()  { return lat;  }
  double getLon()  { return lon;  }
};

///////////////////////////////////////////////////////////////////////////////
//  OpenStreetmap Way-Objekt                                                 //
///////////////////////////////////////////////////////////////////////////////
class OSM_Way: public OSM_Object
{
public:
  OSM_Way ( std::string idP );
  virtual ~OSM_Way();

protected:
  std::vector<std::string> vPointIds;
  bool                     isComplete;

public:
  virtual OSM_OBJECT_TYPE getType();
  virtual std::string getTypeString();

  virtual std::string getClassificationWithGeometry();

  bool checkReferences (  OSM_Model * osmModelP );
  bool getIsComplete()  {  return isComplete;  }

  void addPoint ( std::string id );
  void addPoints ( std::vector<std::string> &vPointIdsP );

	size_t getPointAnz()  {  return vPointIds.size();  }
  std::vector<std::string> & getPoints()  {  return vPointIds;  }

  std::string getStartPointId();
  std::string getEndPointId();

  bool isClosed();
};

class MultiWay
{
public:
  MultiWay();
  virtual ~MultiWay();

protected:
  std::vector<OSM_Way*> vWays;
  std::vector<bool>     vOrientations;

public:
  void addWay ( OSM_Way * pWay, bool ori );
   std::vector<std::string> getNodeIds ();
  bool isClosed();
};


///////////////////////////////////////////////////////////////////////////////
//  OpenStreetmap Relation-Objekt                                            //
///////////////////////////////////////////////////////////////////////////////
class OSM_Relation: public OSM_Object
{
public:
  OSM_Relation ( std::string idP );
  virtual ~OSM_Relation();

protected:
  std::vector<std::string> vObjectIds;
  std::vector<std::string> vRoles;
  std::vector<MultiWay*>   vOuterContours;
  std::vector<MultiWay*>   vInnerContours;

  bool        isComplete;
  std::string errorCode;
  bool        isMultipolygon;

public:
  virtual OSM_OBJECT_TYPE getType() {  return OSM_RELATION;  }
  virtual std::string getTypeString()  {  return "_RE_";   }

  virtual std::string getClassificationWithGeometry() {  return classification + "_relation"; }

	size_t getMemberAnz()  {  return vObjectIds.size();  }
  void addMember ( std::string type, std::string id, std::string role );

  bool checkReferences (  OSM_Model * pOSM_Model );
  bool getIsComplete()  {  return isComplete;  }

  bool checkAndCorrectMultiPolygon ( OSM_Model * pOSM_Model );
  bool getIsMultipolygon()  {  return isMultipolygon;  }

  std::string getErrorCode()  {  return errorCode;  }

//  OSM_Way * getRings( std::vector<OSM_Way*> &vInner, OSM_Model * pOSM_Model ); 

  std::vector<MultiWay*> & getOuterContours ()  {  return vOuterContours;  }
  std::vector<MultiWay*> & getInnerContours ()  {  return vInnerContours;  }

  std::vector<std::string> & getMembers ()  {  return vObjectIds; }
  std::vector<std::string> & getRoles ()  {  return vRoles; }

  std::string getRole ( OSM_Object * pObj );
  std::string getOuterPolygonId();

  void getReferencesRelations ( std::set<std::string> &sRelIds );

protected:
  void generateMultiWays ( std::vector<std::string> vOuterIds, OSM_Model * pOSMModel );

};

///////////////////////////////////////////////////////////////////////////////
//  Statistik der benutzten Tags (key,value) für eine OSM-Klasse             //
///////////////////////////////////////////////////////////////////////////////
class OSM_Statistics
{
public:
  OSM_Statistics ( std::string classNameP, size_t objAnzP = 0 );
  virtual ~ OSM_Statistics();

protected:
  std::string className;
	size_t      objAnz;
  std::map<std::string,std::map<std::string,int>> mTagMap;

public:
  void addTag ( std::string key, std::string value );
  
  void incObjAnz()  {   objAnz++;  }
	size_t getObjAnz()  {  return objAnz;  }


	size_t getStat ( std::vector<std::string> &vKeys, std::vector<std::map<std::string,int>> &vValueCounts );
	size_t getTagValueAnz ( std::string key );
};

class OSM_StatisticsAll
{
public:
  OSM_StatisticsAll();
  virtual ~ OSM_StatisticsAll();

public:
  OSM_Statistics * pNodeStatistics;
  OSM_Statistics * pWayStatistics;
  OSM_Statistics * pRelationStatistics;
};

///////////////////////////////////////////////////////////////////////////////
//  OSM-Klassifikationsschema                                                //
///////////////////////////////////////////////////////////////////////////////
class OSM_Class
{
public:
  OSM_Class();
  virtual ~OSM_Class();

public:
  std::string osm_key;
  std::string featureType;
  int         priority;
  bool        pointGeometry;
  bool        lineGeometry;
  bool        areaGeometry;
  bool        relation;
};

class OSM_SemanticClassification
{
public:
  OSM_SemanticClassification ();
  virtual ~OSM_SemanticClassification();

protected:
  std::map<std::string,OSM_Class> mClassification;

public:
  void readSemanticClassification ( std::string fileName );

  std::string getFeatureType ( OSM_Object * pObject );
};

///////////////////////////////////////////////////////////////////////////////
//  Definiert einen Filter für mehrere OSM-Tags                              //
///////////////////////////////////////////////////////////////////////////////
class OSM_TagFilter
{
public:
  enum SINGLE_FILTER_TYPE {  POSITIVE, NEGATIVE  };
  enum TAG_FILTER_COMBINATION  {  AND, OR };

public:
  OSM_TagFilter( std::string nameP, TAG_FILTER_COMBINATION combinationP );
  virtual ~OSM_TagFilter();

protected:
  std::map<std::string,OSM_SingleTagFilter*> mSingleFilters;
  std::string                                filterName;
  TAG_FILTER_COMBINATION                     combination;

public:
  OSM_SingleTagFilter * addSingleFilter ( std::string osmKeyP, SINGLE_FILTER_TYPE typeP  );

  bool check ( std::map<std::string,std::string> & mTags );
};


///////////////////////////////////////////////////////////////////////////////
//  Definiert einen Filter für einen einzelnen OSM-Tag                       //
///////////////////////////////////////////////////////////////////////////////
class OSM_SingleTagFilter
{
public:
  OSM_SingleTagFilter( std::string osmKeyP, OSM_TagFilter::SINGLE_FILTER_TYPE typeP );
  virtual ~OSM_SingleTagFilter();

protected:
  std::string                       osmKey;
  OSM_TagFilter::SINGLE_FILTER_TYPE type; 
  std::set<std::string>             tagValues;

public:
  void addValue ( std::string valueP );

  bool check ( std::string osmKeyP, std::string valueP="" );
};

///////////////////////////////////////////////////////////////////////////////
//  Collection von OSM_TagFilter Objekten                                    //
///////////////////////////////////////////////////////////////////////////////
class OSM_TagFilterCollection
{
public:
  OSM_TagFilterCollection ();
  virtual ~OSM_TagFilterCollection();

protected:
  std::map<std::string,OSM_TagFilter*> mTagFilters;

public:
  void readFilterCollection ( std::string filename );
	size_t getTagFilterNames ( std::vector<std::string> & vNames );

   OSM_TagFilter * getTagFilter ( std::string tagFilterName );
};




