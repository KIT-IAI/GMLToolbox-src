#include "StdAfx.h"
#include <time.h>
#include <string>
#include <map>
#include <sstream>
#include <set>

#include "Convert.h"
#include "Osm.h"

#include "OSM_Writer.h"
#using <mscorlib.dll>

///////////////////////////////////////////////////////////////////////////////
//  OSMWriter                                                                //
///////////////////////////////////////////////////////////////////////////////
OSMWriter::OSMWriter ( OSM_Model * pOSMModel )
{
  m_pOSMModel = pOSMModel;
  anzObjWrite = 0;
}

OSMWriter::~OSMWriter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines OSM-Modells                                              //
///////////////////////////////////////////////////////////////////////////////
int OSMWriter::write( string fileNameP )
{
  String  ^ FILE_NAME;
  String  ^ comment;
  String  ^ date;
  char      buffer[16];


  double   minLat, minLon, maxLat, maxLon;
  _strdate_s ( buffer, 16 );
  date = QuConvert::ToString ( string ( buffer ) );

  FILE_NAME = QuConvert::ToString ( fileNameP );
  anzObjWrite = 0;

  System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();
  writer								            = gcnew XmlTextWriter( FILE_NAME, utf8 );

  writer->Formatting = Formatting::Indented;
  writer->WriteStartDocument( true );

  comment = "Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: ";
  comment = String::Concat ( comment, date );

  writer->WriteComment( comment );
  
  writer->WriteStartElement( "osm" );
  writer->WriteAttributeString ( "version", "0.6" );
  writer->WriteAttributeString ( "generator", "KIT" );
  writer->WriteAttributeString( "copyright", "OpenStreetMap and contributors" );
  writer->WriteAttributeString( "attribution", "http://www.openstreetmap.org/copyright" );
  writer->WriteAttributeString( "license", "http://opendatacommons.org/licenses/odbl/1-0/" );

  m_pOSMModel->getBounds( minLat, minLon, maxLat, maxLon );
  writeBounds ( minLat, minLon, maxLat, maxLon);

  OSM_Node * pNode = m_pOSMModel->getFirstOSM_Node();
  while ( pNode != NULL )
  {
    writeNode ( pNode );
    pNode = m_pOSMModel->getNextOSM_Node();
  }

  OSM_Way * pWay = m_pOSMModel->getFirstOSM_Way();
  while ( pWay != NULL )
  {
    writeWay ( pWay );
    pWay = m_pOSMModel->getNextOSM_Way();
  }

  OSM_Relation * pRel = m_pOSMModel->getFirstOSM_Relation();
  while ( pRel != NULL )
  {
    writeRelation ( pRel );
    pRel = m_pOSMModel->getNextOSM_Relation();
  }

  writer->Flush();
  writer->Close();

  return anzObjWrite;
}

void OSMWriter::writeBounds ( double minLat, double minLon, double maxLat, double maxLon )
{
  writer->WriteStartElement( "bounds" );
  writer->WriteAttributeString( "minLat", QuConvert::ToString( minLat, 7 ) );
  writer->WriteAttributeString( "minLon", QuConvert::ToString( minLon, 7 ) );
  writer->WriteAttributeString( "maxLat", QuConvert::ToString( maxLat, 7 ) );
  writer->WriteAttributeString( "maxLon", QuConvert::ToString( maxLon, 7 ) );
  writer->WriteEndElement(); 
  anzObjWrite++;
}

void OSMWriter::writeNode ( OSM_Node * pNode )
{

  writer->WriteStartElement( "node" );
  writer->WriteAttributeString( "visible", "true" );
  writer->WriteAttributeString( "id", QuConvert::ToString( pNode->getId() ) );
  writer->WriteAttributeString( "lat", QuConvert::ToString( pNode->getLat(), 7 ) );
  writer->WriteAttributeString( "lon", QuConvert::ToString( pNode->getLon(), 7 ) );

  if ( pNode->getTagAnz() > 0 )
    writeTags( pNode );

  writer->WriteEndElement();
  anzObjWrite++;
}

void OSMWriter::writeWay ( OSM_Way * pWay )
{
  writer->WriteStartElement( "way" );
  writer->WriteAttributeString( "visible", "true" );
  writer->WriteAttributeString( "id", QuConvert::ToString( pWay->getId() ) );

  for ( unsigned int i = 0; i < pWay->getPointAnz(); i++ )
  {
    writer->WriteStartElement( "nd" );
    writer->WriteAttributeString( "ref", QuConvert::ToString( pWay->getPoints()[i] ) );
    writer->WriteEndElement();
  }

  if ( pWay->getTagAnz() > 0 )
    writeTags( pWay );

  writer->WriteEndElement();
  anzObjWrite++;
}

void OSMWriter::writeRelation ( OSM_Relation * pRel )
{
  writer->WriteStartElement( "relation" );
  writer->WriteAttributeString( "visible", "true" );
  writer->WriteAttributeString( "id", QuConvert::ToString( pRel->getId() ) );

  for ( unsigned int i = 0; i < pRel->getMemberAnz(); i++ )
  {
    string t_id = pRel->getMembers()[i];
    char   OSM_type   = t_id[0];
    string objectId = t_id.substr( 1 );

    string role = pRel->getRoles()[i];

    writer->WriteStartElement( "member" );

    switch ( OSM_type )
    {
    case 'N':
      writer->WriteAttributeString( "type", "node" );
      break;

    case 'W':
      writer->WriteAttributeString( "type", "way" );
      break;

    case 'R':
      writer->WriteAttributeString( "type", "relation" );
      break;
    }

    writer->WriteAttributeString( "ref", QuConvert::ToString( objectId ) );
    writer->WriteAttributeString( "role", QuConvert::ToString( role ) );
    writer->WriteEndElement();
  }

  if ( pRel->getTagAnz() > 0 )
    writeTags( pRel );

  writer->WriteEndElement();
  anzObjWrite++;
}

void OSMWriter::writeTags ( OSM_Object * pObj )
{
  map<string,string>::iterator iterTags;

  for ( iterTags = (pObj->getTags()).begin(); iterTags != (pObj->getTags()).end(); iterTags++ )
  {
    writer->WriteStartElement( "tag" );
    writer->WriteAttributeString( "k", QuConvert::ToString( iterTags->first ) );
    writer->WriteAttributeString( "v", QuConvert::ToString( iterTags->second ) );
    writer->WriteEndElement();
  }
}


