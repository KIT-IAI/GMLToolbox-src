#pragma once
using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Text;
using namespace System::Collections;

#using <mscorlib.dll>


///////////////////////////////////////////////////////////////////////////////
//  OSM-Writer                          				                  					 //
///////////////////////////////////////////////////////////////////////////////
public ref class OSMWriter
{
public:
  OSMWriter ( OSM_Model * pOSMModel );
  virtual ~ OSMWriter();

public:
  virtual int write ( std::string fileNameP );

protected:
  OSM_Model       * m_pOSMModel;
  int			          anzObjWrite;
  XmlTextWriter   ^ writer;

protected:
  void writeBounds ( double minLat, double minLon, double maxLat, double maxLon );
  void writeNode ( OSM_Node * pNode );
  void writeWay ( OSM_Way * pWay );
  void writeRelation ( OSM_Relation * pRel );
  
  void writeTags ( OSM_Object * pObj );



};
