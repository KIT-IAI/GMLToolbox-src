#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Windows::Forms;
using namespace System::Text;
using namespace System::Collections;

#using <mscorlib.dll>

class OSM_Model;
class OSM_StatisticsAll;
class OSM_SemanticClassification;
class OSM_TagFilter;
class OSM_Relation;
class OSM_Way;
class OSM_Node;
class RelationMap;
ref class IdSet;

public ref class OSM_Reader
{
public:
  OSM_Reader ( OSM_Model * pModel, XmlTextReader ^ pReaderP );
  OSM_Reader ( OSM_StatisticsAll * pStat, XmlTextReader ^ pReaderP );
  virtual ~ OSM_Reader();

protected:
  OSM_Model                  * pModel;
  OSM_StatisticsAll          * pStatistics;
  XmlTextReader              ^ pReader;
  String                     ^ pActElement;
  bool                         generateStatistics;
  OSM_SemanticClassification * pSemanticClassification;
  bool                         calcBounds;
  double                       minLat;
  double                       minLon;
  double                       maxLat;
  double                       maxLon;


public:
  int read();
  void setSemanticClassification ( OSM_SemanticClassification * pClass )  {  pSemanticClassification = pClass;  }

protected:
  bool readBounds();
  bool readNode();
  bool readWay();
  bool readRelation();
};

public ref class OSM_ReaderWithFilter
{
public:
  OSM_ReaderWithFilter ( OSM_Model * pModelP, OSM_TagFilter * pFilterP, ListBox ^ listBoxMeldungenP );
  virtual ~OSM_ReaderWithFilter();

protected:
  OSM_Model                  * pModel;
  OSM_TagFilter              * pFilter;
  XmlTextReader              ^ pReader;
  String                     ^ pActElement;
  ListBox                    ^ listBoxMeldungen;
  OSM_SemanticClassification * pSemanticClassification;
  IdSet                      ^ nodesToRead;
  IdSet                      ^ waysToRead;
  IdSet                      ^ relationsToRead;

  RelationMap                * pRelMap;

  bool                         calcBounds;
  double                       minLat;
  double                       minLon;
  double                       maxLat;
  double                       maxLon;

public:
	size_t read ( std::string fileName );
  void setSemanticClassification ( OSM_SemanticClassification * pClass )  {  pSemanticClassification = pClass;  }

protected:
  bool readFirstPass();
  bool readSecondPass();
  bool readThirdPass();

  bool resolveRelations();

  bool readBounds();
  OSM_Relation * readRelation();
  OSM_Way      * readWay();
  OSM_Node     * readNode();

  void addToOSM_Model ( OSM_Relation * pRel );
  void addToOSM_Model ( OSM_Way * pWay );
  void addToOSM_Model ( OSM_Node * pNode );

};

public ref class IdSet
{
public:
  IdSet();
  virtual ~ IdSet();

private:
  System::Collections::Hashtable ^ hashtable;

public:
  void addId ( String ^ id );
  bool existId ( String ^ id );
};

class RelationMap
{
public:
  RelationMap();
  virtual ~ RelationMap();

private:
  std::map<std::string,OSM_Relation*> mRelations;

public:
  void addRelation ( OSM_Relation * pRel );

  OSM_Relation * getRelation ( std::string id );
  void removeRelation ( std::string id );
};


