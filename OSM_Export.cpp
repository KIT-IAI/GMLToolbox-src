#include "stdafx.h"

#include <vector>
#include <map>
#include <string>
#include <set>

using namespace std;

#include "Convert.h"
#include "OSM_Export.h"

using namespace System;
using namespace System::Xml;

using namespace System::Text;
using namespace System::Collections;

///////////////////////////////////////////////////////////////////////////////
//  OSM_ShapefileAttributesSchemata                                          //
///////////////////////////////////////////////////////////////////////////////
OSM_ShapefileAttributesSchemata::OSM_ShapefileAttributesSchemata()
{

}

OSM_ShapefileAttributesSchemata::~ OSM_ShapefileAttributesSchemata()
{
  map<string,OSM_ShapefileAttributesSchema*>::iterator iter;

  for ( iter = mSchemata.begin(); iter != mSchemata.end(); iter++ )
    delete iter->second;
}

unsigned int OSM_ShapefileAttributesSchemata::getSchemaIds(vector<string> &vIds )
{
  map<string,OSM_ShapefileAttributesSchema*>::iterator iter;

  for ( iter = mSchemata.begin(); iter != mSchemata.end(); iter++ )
    vIds.push_back( iter->first );

  return vIds.size();
}


bool OSM_ShapefileAttributesSchemata::readSchemata ( string fileName )
{
  bool     firstElement = true;
  String ^ pActElement;
  String ^ STR;
  string   stdString;
  enum    { NODES, WAYS, AREAS, RELATIONS  } mode;

  OSM_ShapefileAttributesSchema * pSchema;

  String        ^ FILE_NAME = QuConvert::ToString( fileName );
  XmlTextReader ^ pReader   = gcnew XmlTextReader( FILE_NAME );

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      pActElement = pReader->Name;

      if ( firstElement )
      {
        if ( pActElement->CompareTo( "SachdatenSchemata" ) != 0 ) 
        {
          pReader->Close();
          return false;
        }
        firstElement = false;
      }

      if ( pActElement->CompareTo( "SachdatenSchema" ) == 0 )
      {
        STR = pReader->GetAttribute ( "name" );
        QuConvert::systemStr2stdStr( stdString, STR );

        pSchema = new OSM_ShapefileAttributesSchema ( stdString );
        mSchemata[stdString] = pSchema;
      }
      else
      if ( pActElement->CompareTo( "NodeAttributes" ) == 0 )
        mode = NODES;
      else
      if ( pActElement->CompareTo( "WayAttributes" ) == 0 )
        mode = WAYS;
      else
      if ( pActElement->CompareTo( "AreaAttributes" ) == 0 )
        mode = AREAS;
      else
      if ( pActElement->CompareTo( "RelationAttributes" ) == 0 )
        mode = RELATIONS;
      else
      if ( pActElement->CompareTo( "attribute" ) == 0 )
      {
        attribute a;

        STR = pReader->GetAttribute( "tagName" );
        QuConvert::systemStr2stdStr( a.tagName, STR );

        STR = pReader->GetAttribute( "shapeAttributName" );
        QuConvert::systemStr2stdStr( a.shapeAttributeName, STR );

        switch ( mode )
        {
        case NODES:
          pSchema->addNodeAttribute( a );
          break;

        case WAYS:
          pSchema->addWayAttribute( a );
          break;

        case AREAS:
          pSchema->addAreaAttribute( a );
          break;

        case RELATIONS:
          pSchema->addRelationAttribute( a );
          break;
        }
      }  
    }
    else
    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = pReader->Name;
      if ( pActElement->CompareTo( "SachdatenSchemata" ) == 0 )
        break;
    }
  }
  return true;
}

 OSM_ShapefileAttributesSchema * OSM_ShapefileAttributesSchemata::getSchema ( std::string id )
{
   map<string,OSM_ShapefileAttributesSchema*>::iterator iter;

   iter =  mSchemata.find( id );

  if ( iter != mSchemata.end() )
    return iter->second;
  else
    return NULL;
}

 //////////////////////////////////////////////////////////////////////////////
 //  OSM_ShapefileAttributesSchema                                           //
 //////////////////////////////////////////////////////////////////////////////
 OSM_ShapefileAttributesSchema::OSM_ShapefileAttributesSchema ( std::string idP )
 {
   id = idP;
 }

 OSM_ShapefileAttributesSchema::~OSM_ShapefileAttributesSchema()
 {

 }

 void OSM_ShapefileAttributesSchema::addNodeAttribute ( attribute nodeAttribute )
 {
   vNodeAttributes.push_back( nodeAttribute );
 }

 void OSM_ShapefileAttributesSchema::addWayAttribute ( attribute wayAttribute )
 {
   vWayAttributes.push_back( wayAttribute );
 }

 void OSM_ShapefileAttributesSchema::addAreaAttribute ( attribute areaAttribute )
 {
   vAreaAttributes.push_back( areaAttribute );
 }

 void OSM_ShapefileAttributesSchema::addRelationAttribute ( attribute relationAttribute )
 {
   vRelationAttributes.push_back( relationAttribute );
 }


