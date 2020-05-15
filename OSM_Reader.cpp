#include "StdAfx.h"

#include <vector>
#include <map>
#include <string>
#include <set>

using namespace System;
using namespace System::Xml;

#include "Convert.h"
#include "Feature.h"
#include "Geometrie.h"
#include "Convert.h"
#include "Osm.h"

#include "OSM_Reader.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer OSM-Datei                                                 //
///////////////////////////////////////////////////////////////////////////////
OSM_Reader::OSM_Reader ( OSM_Model * pModelP, XmlTextReader ^ pReaderP )
{
  pModel             = pModelP;
  pStatistics        = NULL;
  generateStatistics = false;
  pReader            = pReaderP;

  calcBounds         = true;
  minLat             = 90.0;
  minLon             = 360.0;
  maxLat             = -90.0;
  maxLon             = -360.0;
}

OSM_Reader::OSM_Reader ( OSM_StatisticsAll * pStatisticsP, XmlTextReader ^ pReaderP )
{
  pModel             = NULL;
  pStatistics        = pStatisticsP;
  generateStatistics = true;
  pReader            = pReaderP;
}


OSM_Reader::~OSM_Reader()
{

}

int OSM_Reader::read()
{
  int osmObjectAnz = 0;
  bool success;

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      pActElement = pReader->Name;

      if ( pActElement->CompareTo( "bounds") == 0 )
        readBounds();
      else
      if ( pActElement->CompareTo( "node") == 0 )
      {
        success = readNode();
        if ( success ) osmObjectAnz++;
      }
      else
      if ( pActElement->CompareTo( "way") == 0 )
      {
        success = readWay();
        if ( success ) osmObjectAnz++;
      }
      else
      if ( pActElement->CompareTo( "relation") == 0 )
      {
        success = readRelation();
        if ( success ) osmObjectAnz++;
      }
      else
      {
        string stdString;
        string errorMessage;

        if ( !generateStatistics )
        {
          QuConvert::systemStr2stdStr( stdString, pActElement );
          errorMessage = "Unbekanntes OSM-Objekt: " + stdString;
          pModel->addErrorMessage( errorMessage );
        }
      }
    }
    else
    {
      if ( pReader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = pReader->Name;
        if ( pActElement->CompareTo( "osm" ) == 0 ) break;
      }
    }
  }
  if ( calcBounds )
    pModel->setBounds( minLat, minLon, maxLat, maxLon );

  return osmObjectAnz;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen des OSM-Elements bounds                                         //
///////////////////////////////////////////////////////////////////////////////
bool OSM_Reader::readBounds()
{
  String ^ STR;
  string   attributName;
  string   attributWert;
  double   minLat, minLon, maxLat, maxLon;

  if ( generateStatistics ) return true;

  while ( pReader->MoveToNextAttribute() )
  {
    STR = pReader->Name;
    QuConvert::systemStr2stdStr ( attributName, STR );

    if ( attributName == "minlat" )
      minLat = XmlConvert::ToDouble( pReader->Value );
    else
    if ( attributName == "minlon" )
      minLon = XmlConvert::ToDouble( pReader->Value );
    else
    if ( attributName == "maxlat" )
      maxLat = XmlConvert::ToDouble( pReader->Value );
    else
    if ( attributName == "maxlon" )
      maxLon = XmlConvert::ToDouble( pReader->Value );
  }

  pModel->setBounds( minLat, minLon, maxLat, maxLon );
  calcBounds = false;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen des OSM-Elements node                                           //
///////////////////////////////////////////////////////////////////////////////
bool OSM_Reader::readNode()
{
  String   ^ STR;
  bool       isEmpty = pReader->IsEmptyElement;
  bool       success = true;
  string     id;
  double     lat, lon;
  string     key, value;
  OSM_Node * pNode = NULL;

  if ( !generateStatistics )
  {
    STR = pReader->GetAttribute( "id" );
    if ( STR != nullptr )
      QuConvert::systemStr2stdStr( id, STR );
    else
      success = false;

    if ( success )
    {
      STR = pReader->GetAttribute( "lat" );
      if ( STR != nullptr )
      {
        lat = XmlConvert::ToDouble( STR );
        if ( calcBounds && lat < minLat ) minLat = lat;
        if ( calcBounds && lat > maxLat ) maxLat = lat;
      }
      else
        success = false;
    }

    if ( success )
    {
      STR = pReader->GetAttribute( "lon" );
      if ( STR != nullptr )
      {
        lon = XmlConvert::ToDouble( STR );
        if ( calcBounds && lon < minLon ) minLon = lon;
        if ( calcBounds && lon > maxLon ) maxLon = lon;
      }
      else
        success = false;
    }

    if ( success )
    {
      pNode = new OSM_Node ( id, lat, lon );
      pModel->addOSM_Node( pNode );
    }
  }
  else
    pStatistics->pNodeStatistics->incObjAnz();

  if ( isEmpty ) return success;

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element && success )
    {
      pActElement = pReader->Name;
      if ( pActElement->CompareTo( "tag") == 0 )
      {
        STR = pReader->GetAttribute( "k" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( key, STR );
        else
          success = false;

        if ( success )
        {
          STR = pReader->GetAttribute( "v" );
          if ( STR != nullptr )
            QuConvert::systemStr2stdStr( value, STR );
          else
            success = false;
        }
        if ( success && key != "created_by" )
        {
          if ( generateStatistics )
            pStatistics->pNodeStatistics->addTag( key, value );
          else
            pNode->addTag( key, value );
        }
      }
    }
     
    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = pReader->Name;
      if ( pActElement->CompareTo( "node" ) == 0 )
      {
        if ( pSemanticClassification != NULL )
        {
          string featureClass = pSemanticClassification->getFeatureType( pNode );
          pNode->setClassification( featureClass );
        }
        break;
      }
    }
  }
  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen des OSM-Elements way                                            //
///////////////////////////////////////////////////////////////////////////////
bool OSM_Reader::readWay()
{
  String           ^ STR;
  bool               isEmpty = pReader->IsEmptyElement;
  bool               success = true;
  bool               successLoc;
  string             id;
  string             key, value;
  string             ref;
  OSM_Way         * pWay = NULL;

  if ( !generateStatistics )
  {
    STR = pReader->GetAttribute( "id" );
    if ( STR != nullptr )
      QuConvert::systemStr2stdStr( id, STR );
    else
      success = false;
  }

  if ( isEmpty ) return false;

  if ( generateStatistics )
    pStatistics->pWayStatistics->incObjAnz();
  else
  {
    pWay = new OSM_Way ( id );
    pModel->addOSM_Way( pWay );
  }

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element && success )
    {
      pActElement = pReader->Name;

      if ( pActElement->CompareTo( "nd" ) == 0 && !generateStatistics )
      {
        STR = pReader->GetAttribute( "ref" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr( ref, STR );
          pWay->addPoint( ref );
        }
      }
      else
      if ( pActElement->CompareTo( "tag") == 0 )
      {
        successLoc = true;
        STR = pReader->GetAttribute( "k" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( key, STR );
        else
          successLoc = false;

        if ( successLoc )
        {
          STR = pReader->GetAttribute( "v" );
          if ( STR != nullptr )
            QuConvert::systemStr2stdStr( value, STR );
          else
            successLoc = false;
        }
        if ( successLoc && key != "created_by" && key != "source" )
        {           
           if ( generateStatistics )
             pStatistics->pWayStatistics->addTag( key, value );
           else
             pWay->addTag ( key, value );
        }
      }
    }

    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = pReader->Name;
      if ( pActElement->CompareTo( "way" ) == 0 )
      {
        if ( pSemanticClassification != NULL )
        {
          string featureClass = pSemanticClassification->getFeatureType( pWay );
          pWay->setClassification( featureClass );
        }
        break;
      }
    }
  }


  if ( !success ) return false;

  if ( !generateStatistics )
  {
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen des OSM-Elements relation                                       //
///////////////////////////////////////////////////////////////////////////////
bool OSM_Reader::readRelation()
{
  String           ^ STR;
  bool               isEmpty = pReader->IsEmptyElement;
  bool               success = true;
  bool               successLoc;
  string             id;
  string             key, value;
  string             type, ref, role;
  OSM_Relation     * pRelation = NULL;

  if ( !generateStatistics )
  {
    STR = pReader->GetAttribute( "id" );
    if ( STR != nullptr )
      QuConvert::systemStr2stdStr( id, STR );
    else
      success = false;
  }

  if ( isEmpty ) return false;


  if ( success )
  {
    if ( generateStatistics )
      pStatistics->pRelationStatistics->incObjAnz();
    else
    {
      pRelation = new OSM_Relation ( id );
      pModel->addOSM_Relation( pRelation );
    }
  }

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element && success )
    {
      pActElement = pReader->Name;

      if ( pActElement->CompareTo( "member" ) == 0 && !generateStatistics )
      {
        successLoc = true;

        STR = pReader->GetAttribute( "ref" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( ref, STR );
        else
          successLoc = false;

        if ( successLoc )
        {
          STR = pReader->GetAttribute( "type" );
          if ( STR != nullptr )
            QuConvert::systemStr2stdStr( type, STR );
          else
            type = "";

          STR = pReader->GetAttribute( "role" );
          if ( STR != nullptr )
            QuConvert::systemStr2stdStr( role, STR );
          else
            role = "";
        }

        if ( successLoc )
          pRelation->addMember( type, ref, role );
      }
      else
      if ( pActElement->CompareTo( "tag" ) == 0 )
      {
        successLoc = true;
        STR = pReader->GetAttribute( "k" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( key, STR );
        else
          successLoc = false;

        if ( successLoc )
        {
          STR = pReader->GetAttribute( "v" );
          if ( STR != nullptr )
            QuConvert::systemStr2stdStr( value, STR );
          else
            successLoc = false;
        }
        if ( successLoc &&   key != "created_by" )
        {
          if ( generateStatistics )
            pStatistics->pRelationStatistics->addTag( key, value );
          else
            pRelation->addTag( key, value );
        }
      }
    }

    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = pReader->Name;
      if ( pActElement->CompareTo( "relation" ) == 0 )
      {
        if ( pSemanticClassification != NULL )
        {
          string featureClass = pSemanticClassification->getFeatureType( pRelation );
          if ( featureClass != "" )
            pRelation->setClassification( featureClass );
        }
        break;
      }
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines OSM-Files mit Filterung                                   //
///////////////////////////////////////////////////////////////////////////////
OSM_ReaderWithFilter::OSM_ReaderWithFilter ( OSM_Model * pModelP, OSM_TagFilter * pFilterP, ListBox ^ listBoxMeldungenP )
{
  pModel                  = pModelP;
  pFilter                 = pFilterP;
  pSemanticClassification = NULL;

  listBoxMeldungen = listBoxMeldungenP;

  nodesToRead     = gcnew IdSet;
  waysToRead      = gcnew IdSet;
  relationsToRead = gcnew IdSet;

  calcBounds         = true;
  minLat             = 90.0;
  minLon             = 360.0;
  maxLat             = -90.0;
  maxLon             = -360.0;
}

OSM_ReaderWithFilter::~OSM_ReaderWithFilter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Einleseroutine                                                           //
///////////////////////////////////////////////////////////////////////////////
size_t OSM_ReaderWithFilter::read ( std::string fileName )
{
  bool success;

  if ( !File::Exists ( QuConvert::ToString( fileName ) ) )
    return 0;

  pReader = gcnew XmlTextReader ( QuConvert::ToString( fileName ) );

  pRelMap = new RelationMap;

  listBoxMeldungen->Items->Add ( "Start 1 Durchgang: Relationen lesen ..." );
  listBoxMeldungen->SelectedIndex = listBoxMeldungen->Items->Count-1;

  success = readFirstPass();
  if ( !success ) return 0;

  listBoxMeldungen->Items->Add ( "1 Durchgang beendet\r\n" );
  listBoxMeldungen->Items->Add ( "Geschachtelte Relationen auflösen" );
  listBoxMeldungen->SelectedIndex = listBoxMeldungen->Items->Count-1;

  success = resolveRelations();
  if ( !success ) return 0;

  listBoxMeldungen->Items->Add ( "Beendet\r\n" );
  listBoxMeldungen->Items->Add ( "Start 2 Durchgang: Ways lesen ..." );
  listBoxMeldungen->SelectedIndex = listBoxMeldungen->Items->Count-1;

  //pReader->ResetState();
  pReader->Close();
  pReader = gcnew XmlTextReader ( QuConvert::ToString( fileName ) );

  success = readSecondPass();
  if ( !success ) return 0;

  listBoxMeldungen->Items->Add ( "2 Durchgang beendet\r\n" );
  listBoxMeldungen->Items->Add ( "Start 3 Durchgang: Relations lesen ..." );
  listBoxMeldungen->SelectedIndex = listBoxMeldungen->Items->Count-1;
  
//  pReader->ResetState();
  pReader->Close();
  pReader = gcnew XmlTextReader ( QuConvert::ToString( fileName ) );

  success = readThirdPass();
  if ( !success ) return 0;

  listBoxMeldungen->Items->Add ( "Beendet\r\n" );
  listBoxMeldungen->SelectedIndex = listBoxMeldungen->Items->Count-1;

  return pModel->getNodeAnz() + pModel->getWayAnz() + pModel->getRelationAnz();
}

///////////////////////////////////////////////////////////////////////////////
//  Überlesen des gesamten Dateiinhalts bis zu den Relations                 //
//  Einlesen der Relation-Objekte, die die Filter-Bedingungen erfüllen       //
//  Speichern der daraus resultierenden, benötigten Relations, Ways und      //
//  Nodes                                                                    //
///////////////////////////////////////////////////////////////////////////////
bool OSM_ReaderWithFilter::readFirstPass()
{
  bool success;
  bool filterSuccess;

  while( pReader->Read() )
  {
    pActElement = pReader->Name;
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      if ( pActElement->CompareTo( "osm") == 0 )
        continue;
      else
      if ( pActElement->CompareTo( "bounds") == 0 )
      {
        readBounds();
        break;
      }
      else
        break;
    }
  }

  success = pReader->ReadToFollowing( "relation" );
  while ( success )
  {
    OSM_Relation * pRel = readRelation();
    if ( pRel != NULL )
    {
      filterSuccess = pFilter->check ( pRel->getTags() );
      if ( filterSuccess )
        addToOSM_Model ( pRel );
      else
        pRelMap->addRelation( pRel );
    }
    success = pReader->ReadToFollowing( "relation" );
  }

  return true;
}

bool OSM_ReaderWithFilter::readSecondPass()
{
  bool      success;
  bool      filterSuccess;
  OSM_Way * pWay;

  success = pReader->ReadToFollowing( "way" );
  if ( !success ) return false;
  
  pWay = readWay();
  while ( pWay != NULL )
  {
    string id = pWay->getId();
    if ( waysToRead->existId( QuConvert::ToString ( id ) ) )
      addToOSM_Model( pWay );
    else
    {
      filterSuccess = pFilter->check ( pWay->getTags() );
      if ( filterSuccess )
        addToOSM_Model( pWay );
      else
        delete pWay;
    }

    pWay = NULL;   
    while( pReader->Read() )
    {
      if ( pReader->NodeType == XmlNodeType::Element )
      { 
        pActElement = pReader->Name;
        if ( pActElement->CompareTo( "way" ) == 0 )
        {
          pWay = readWay();
          break;
        }
        else
          break;
      }
    }
  }


  return true;
}

bool OSM_ReaderWithFilter::readThirdPass()
{
  bool       success;
  bool       filterSuccess;
  OSM_Node * pNode;

  success = pReader->ReadToFollowing( "node" );
  if ( !success ) return false;

  pNode = readNode();
  while ( pNode != NULL )
  {
    string id = pNode->getId();
    if ( nodesToRead->existId( QuConvert::ToString ( id ) ) )
      addToOSM_Model( pNode );
    else
    {
      filterSuccess = pFilter->check ( pNode->getTags() );
      if ( filterSuccess )
        addToOSM_Model( pNode );
      else
        delete pNode;
    }

    pNode = NULL;   
    while( pReader->Read() )
    {
      if ( pReader->NodeType == XmlNodeType::Element )
      { 
        pActElement = pReader->Name;
        if ( pActElement->CompareTo( "node" ) == 0 )
        {
          pNode = readNode();
          break;
        }
        else
          break;
      }
    }
  }

  if ( calcBounds )
    pModel->setBounds( minLat, minLon, maxLat, maxLon );

  return true;
}


OSM_Relation * OSM_ReaderWithFilter::readRelation()
{
  String ^ STR;
  string   id, ref, type, role, key, value;
  bool     success = true;

  STR = pReader->GetAttribute( "id" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( id, STR );
  else
    return NULL;

  if (  pReader->IsEmptyElement ) return NULL;

  OSM_Relation * pRelation = new OSM_Relation ( id );

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      pActElement = pReader->Name;

      if ( pActElement->CompareTo( "member" ) == 0 )
      {
        STR = pReader->GetAttribute( "ref" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( ref, STR );
        else
        {
          success = false;
          break;
        }

        STR = pReader->GetAttribute( "type" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( type, STR );
        else
          type = "";

        STR = pReader->GetAttribute( "role" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( role, STR );
        else
          role = "";

        pRelation->addMember( type, ref, role );
      }
      else
      if ( pActElement->CompareTo( "tag" ) == 0 )
      {
        STR = pReader->GetAttribute( "k" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( key, STR );
        else
        {
          success = false;
          break;
        }

        STR = pReader->GetAttribute( "v" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( value, STR );
        else
        {
          success = false;
          break;
        }

        if ( key != "created_by" )
          pRelation->addTag( key, value );
      }
    }
    else
    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = pReader->Name;
      if ( pActElement->CompareTo( "relation" ) == 0 )
      {
        if ( pSemanticClassification != NULL )
        {
          string featureClass = pSemanticClassification->getFeatureType( pRelation );
          if ( featureClass != "" )
            pRelation->setClassification( featureClass );
        }
        break;
      }
    }
  }

  if ( !success )
  {
    delete pRelation;
    return NULL;
  }
  else
    return pRelation;
}

OSM_Way * OSM_ReaderWithFilter::readWay()
{
  String           ^ STR;
  bool               isEmpty = pReader->IsEmptyElement;
  bool               success = true;
  string             id;
  string             key, value;
  string             ref;

  STR = pReader->GetAttribute( "id" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( id, STR );
  else
      return NULL;

  if ( isEmpty ) return NULL;

  OSM_Way *pWay = new OSM_Way ( id );

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      pActElement = pReader->Name;

      if ( pActElement->CompareTo( "nd" ) == 0 )
      {
        STR = pReader->GetAttribute( "ref" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr( ref, STR );
          pWay->addPoint( ref );
        }
      }
      else
      if ( pActElement->CompareTo( "tag") == 0 )
      {
        STR = pReader->GetAttribute( "k" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( key, STR );
        else
        {
          success = false;
          break;
        }

        STR = pReader->GetAttribute( "v" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( value, STR );
        else
        {
          success = false;
          break;
        }
      }

      if ( key != "created_by" && key != "source" )
        pWay->addTag ( key, value );
    }

    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = pReader->Name;
      if ( pActElement->CompareTo( "way" ) == 0 )
      {
        if ( pSemanticClassification != NULL )
        {
          string featureClass = pSemanticClassification->getFeatureType( pWay );
          pWay->setClassification( featureClass );
        }
        break;
      }
    }
  }

  if ( !success )
  {
    delete pWay;
    return NULL;
  }
  else
    return pWay;
}

OSM_Node * OSM_ReaderWithFilter::readNode()
{
  String   ^ STR;
  bool       isEmpty = pReader->IsEmptyElement;
  bool       success = true;
  string     id;
  double     lat, lon;
  string     key, value;

  STR = pReader->GetAttribute( "id" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( id, STR );
  else
      return NULL;

  STR = pReader->GetAttribute( "lat" );
  if ( STR != nullptr )
  {
    lat = XmlConvert::ToDouble( STR );
    if ( calcBounds && lat < minLat ) minLat = lat;
    if ( calcBounds && lat > maxLat ) maxLat = lat;
  }
  else
    return NULL;

  STR = pReader->GetAttribute( "lon" );
  if ( STR != nullptr )
  {
    lon = XmlConvert::ToDouble( STR );
    if ( calcBounds && lon < minLon ) minLon = lon;
    if ( calcBounds && lon > maxLon ) maxLon = lon;
  }
  else
    return NULL;

  OSM_Node * pNode = new OSM_Node ( id, lat, lon );

  if ( isEmpty ) return pNode;

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      pActElement = pReader->Name;
      if ( pActElement->CompareTo( "tag") == 0 )
      {
        STR = pReader->GetAttribute( "k" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( key, STR );
        else
        {
          success = false;
          break;
        }

        STR = pReader->GetAttribute( "v" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( value, STR );
        else
        {
          success = false;
          break;
        }

        if ( key != "created_by" )
          pNode->addTag( key, value );
      }
    }

    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = pReader->Name;
      if ( pActElement->CompareTo( "node" ) == 0 )
      {
        if ( pSemanticClassification != NULL )
        {
          string featureClass = pSemanticClassification->getFeatureType( pNode );
          pNode->setClassification( featureClass );
        }
        break;
      }
    }
  }

  if ( !success )
  {
    delete pNode;
    return NULL;
  }
  else
    return pNode;
}



void OSM_ReaderWithFilter::addToOSM_Model ( OSM_Relation * pRel )
{
  vector<string> vObjectIds;

  pModel->addOSM_Relation( pRel );
  vObjectIds = pRel->getMembers();

  for ( unsigned int i = 0; i < vObjectIds.size(); i++ )
  {
    string   objectIdWithType = vObjectIds[i];

    char     OSM_type         = objectIdWithType[0];
    string   objectId_std     = objectIdWithType.substr( 1 );
    String ^ objectId         = QuConvert::ToString( objectId_std );

    switch ( OSM_type )
    {
    case 'N':   // Node
      nodesToRead->addId( objectId );
      break;

    case 'W':   // Way
      waysToRead->addId( objectId );
      break;
    }
  }
}

void OSM_ReaderWithFilter::addToOSM_Model ( OSM_Way * pWay )
{
  vector<string> vPointIds;

  pModel->addOSM_Way( pWay );

  vPointIds = pWay->getPoints();

  for ( unsigned int i = 0; i < vPointIds.size(); i++ )
  {
    string id = vPointIds[i];
    nodesToRead->addId( QuConvert::ToString ( id ) );
  }
}

void OSM_ReaderWithFilter::addToOSM_Model ( OSM_Node * pNode )
{
  pModel->addOSM_Node( pNode );
}


bool OSM_ReaderWithFilter::readBounds()
{
  String ^ STR;
  string   attributName;
  string   attributWert;
  double   minLat, minLon, maxLat, maxLon;

  while ( pReader->MoveToNextAttribute() )
  {
    STR = pReader->Name;
    QuConvert::systemStr2stdStr ( attributName, STR );

    if ( attributName == "minlat" )
      minLat = XmlConvert::ToDouble( pReader->Value );
    else
    if ( attributName == "minlon" )
      minLon = XmlConvert::ToDouble( pReader->Value );
    else
    if ( attributName == "maxlat" )
      maxLat = XmlConvert::ToDouble( pReader->Value );
    else
    if ( attributName == "maxlon" )
      maxLon = XmlConvert::ToDouble( pReader->Value );
  }

  pModel->setBounds( minLat, minLon, maxLat, maxLon );
  calcBounds = false;
  return true;
}

bool OSM_ReaderWithFilter::resolveRelations()
{
  OSM_Relation        * pRel;
  set<string>           sRelIds;
  set<string>::iterator iter;

  pRel = pModel->getFirstOSM_Relation();
  while ( pRel != NULL )
  {
    pRel->getReferencesRelations( sRelIds );
    pRel = pModel->getNextOSM_Relation();
  }

  for ( iter = sRelIds.begin(); iter != sRelIds.end(); iter++ )
  {
    string relId = *iter;
    if ( pModel->getOSM_Relation( relId ) == NULL )
    {
      pRel = pRelMap->getRelation( relId );
      pRelMap->removeRelation( relId );
      addToOSM_Model( pRel );
    }
  }

  delete pRelMap;
  pRelMap = NULL;

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Verwaltung von String-Listen mit schnellem Zugriff                       //
///////////////////////////////////////////////////////////////////////////////
IdSet::IdSet()
{
  hashtable = gcnew System::Collections::Hashtable();
}

IdSet::~IdSet()
{

}

void IdSet::addId ( String ^ id )
{
  if ( !hashtable->Contains( id ) )
    hashtable->Add( id, nullptr );
}

bool IdSet::existId ( String ^ id )
{
  return hashtable->Contains( id );
}

///////////////////////////////////////////////////////////////////////////////
//  Verwaltung einer map von relations                                       //
///////////////////////////////////////////////////////////////////////////////
RelationMap::RelationMap()
{

}

RelationMap::~RelationMap()
{
  map<string,OSM_Relation*>::iterator iter;

  for ( iter = mRelations.begin(); iter != mRelations.end(); iter++ )
    delete iter->second;
}

void RelationMap::addRelation ( OSM_Relation * pRel )
{
  mRelations[pRel->getId()] = pRel;
}

OSM_Relation * RelationMap::getRelation ( std::string id )
{
  map<string,OSM_Relation*>::iterator iter;
  
  iter = mRelations.find( id );
  if ( iter != mRelations.end() )
    return iter->second;
  else
    return NULL;
}

void RelationMap::removeRelation ( std::string id )
{
  mRelations.erase( id );
}


