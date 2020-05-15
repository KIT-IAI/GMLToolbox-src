#pragma once
#include "Includes.h"

class OSM_ShapefileAttributesSchema;

struct attribute
{
public:
  std::string tagName;
  std::string shapeAttributeName;
};

class OSM_ShapefileAttributesSchemata
{
public:
  OSM_ShapefileAttributesSchemata();
  virtual ~ OSM_ShapefileAttributesSchemata();

protected:
  std::map<std::string,OSM_ShapefileAttributesSchema*> mSchemata;

public:
  bool readSchemata ( std::string fileName );

  OSM_ShapefileAttributesSchema * getSchema ( std::string id );
  unsigned int getSchemaIds(std::vector<std::string> & vIds );
};

class OSM_ShapefileAttributesSchema
{
public:
  OSM_ShapefileAttributesSchema ( std::string idP );
  virtual ~ OSM_ShapefileAttributesSchema();

protected:
  std::string            id;
  std::vector<attribute> vNodeAttributes;
  std::vector<attribute> vWayAttributes;
  std::vector<attribute> vAreaAttributes;
  std::vector<attribute> vRelationAttributes;

public:
  std::string getId()  {  return id;  }

  void addNodeAttribute ( attribute nodeAttribute );
  void addWayAttribute ( attribute wayAttribute );
  void addAreaAttribute ( attribute areaAttribute );
  void addRelationAttribute ( attribute relationAttribute );

  vector<attribute> & getNodeAttributes()      {  return vNodeAttributes;   }
  vector<attribute> & getWayAttributes()       {  return vWayAttributes;   }
  vector<attribute> & getAreaAttributes()      {  return vAreaAttributes;   }
  vector<attribute> & getRelationAttributes()  {  return vRelationAttributes;   }
};
