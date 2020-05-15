#include "stdafx.h"

#include <vector>
#include <map>
#include <string>
#include <set>

using namespace std;

#include "Convert.h"
#include "constant.h"
#include "Logfile.h"
#include "Geometrie.h"
#include "SrsManager.h"
#include "OSM_Visualisierung.h"
#include "OSM_Reader.h"
#include "Osm.h"

///////////////////////////////////////////////////////////////////////////////
//  OpenStreetmap Modell                                                     //
///////////////////////////////////////////////////////////////////////////////
OSM_Model::OSM_Model( Logfile * pLogfile, SrsManager * pSrsManager, 
              float planausgabeBreite, float planausgabeHoehe, double globalScale  )
{
  m_pLogfile         = pLogfile;
  m_pSrsManager      = pSrsManager;   
  pOSM_StatisticsAll = NULL;

  iterRel   = mRelations.begin();
  iterNodes = mNodes.begin(); 
  iterWays  = mWays.begin();

  pOSM_SvgParams = NULL;

  planausgabeBreite = planausgabeBreite;
  planausgabeHoehe  = planausgabeHoehe;
  globalScale       = globalScale;
  delta             = 0.0;

  svgUeberschriftDatei = "Ueberschrift.svg";
  svgUebersichtDatei	 = "Uebersicht.svg";
  svgKartenDatei			 = "Karte.svg";
  m_objectDataGenerated = false;

  pOSMSemanticClassification = new OSM_SemanticClassification;
  string fileName = Constant::getSchemaPath( OSM ) + "SemanticClassification.xml";
  pOSMSemanticClassification->readSemanticClassification( fileName );
}

OSM_Model::~OSM_Model()
{
  std::map<std::string,OSM_Node*>::iterator     iterNodes;
  std::map<std::string,OSM_Way*>::iterator      iterWays;
  std::map<std::string,OSM_Relation*>::iterator iterRelations;

  for ( iterNodes = mNodes.begin(); iterNodes != mNodes.end(); iterNodes++ )
    delete iterNodes->second;

  for ( iterWays = mWays.begin(); iterWays != mWays.end(); iterWays++ )
    delete iterWays->second;

  for ( iterRelations = mRelations.begin(); iterRelations != mRelations.end(); iterRelations++ )
    delete iterRelations->second;

  if ( pOSMSemanticClassification != NULL ) delete pOSMSemanticClassification;
  if ( pOSM_StatisticsAll != NULL ) delete pOSM_StatisticsAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das OSM-Modell zurück                                              //
///////////////////////////////////////////////////////////////////////////////
void OSM_Model::resetOSM_Model()
{
  std::map<std::string,OSM_Node*>::iterator     iterNodes;
  std::map<std::string,OSM_Way*>::iterator      iterWays;
  std::map<std::string,OSM_Relation*>::iterator iterRelations;

  for ( iterNodes = mNodes.begin(); iterNodes != mNodes.end(); iterNodes++ )
    delete iterNodes->second;
  mNodes.clear();

  for ( iterWays = mWays.begin(); iterWays != mWays.end(); iterWays++ )
    delete iterWays->second;
  mWays.clear();

  for ( iterRelations = mRelations.begin(); iterRelations != mRelations.end(); iterRelations++ )
    delete iterRelations->second;
  mRelations.clear();

  if ( pOSMSemanticClassification != NULL )
  {
      delete pOSMSemanticClassification;
      pOSMSemanticClassification = NULL;
  }

  if ( pOSM_StatisticsAll != NULL )
  {
      delete pOSM_StatisticsAll;
      pOSM_StatisticsAll = NULL;
  }
}



///////////////////////////////////////////////////////////////////////////////
//  Objekte hinzufügen                                                       //
///////////////////////////////////////////////////////////////////////////////
void OSM_Model::addOSM_Node ( OSM_Node * pNode )
{
  mNodes[pNode->getId()] = pNode;
}

void OSM_Model::addOSM_Way ( OSM_Way * pWay )
{
  mWays[pWay->getId()] = pWay;
}

void OSM_Model::addOSM_Relation ( OSM_Relation * pRelation )
{
  mRelations[pRelation->getId()] = pRelation;
}

///////////////////////////////////////////////////////////////////////////////
//  Objekte herausholen                                                      //
///////////////////////////////////////////////////////////////////////////////
OSM_Node * OSM_Model::getOSM_Node ( std::string id )
{
  map<string,OSM_Node*>::iterator iter;
  
  iter = mNodes.find( id );
  if ( iter != mNodes.end() ) return iter->second;
  else                        return NULL;
}

OSM_Way * OSM_Model::getOSM_Way ( std::string id )
{
  map<string,OSM_Way*>::iterator iter;

  iter = mWays.find( id );
  if ( iter != mWays.end() ) return iter->second;
  else                       return NULL;
}

OSM_Relation * OSM_Model::getOSM_Relation ( std::string id )
{
  map<string,OSM_Relation*>::iterator iter;

  iter = mRelations.find( id );
  if ( iter != mRelations.end() ) return iter->second;
  else                            return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Iterator über Objekte                                                    //
///////////////////////////////////////////////////////////////////////////////
OSM_Relation * OSM_Model::getFirstOSM_Relation()
{
  iterRel = mRelations.begin();
  if ( iterRel != mRelations.end() )
    return iterRel->second;
  else
    return NULL;
}

OSM_Relation * OSM_Model::getNextOSM_Relation()
{
  iterRel++;
  if ( iterRel != mRelations.end() )
    return iterRel->second;
  else
    return NULL;
}

OSM_Node * OSM_Model::getFirstOSM_Node()
{
  iterNodes = mNodes.begin();
  if ( iterNodes != mNodes.end() )
    return iterNodes->second;
  else
    return NULL;

}

OSM_Node * OSM_Model::getNextOSM_Node()
{
  iterNodes++;
  if ( iterNodes != mNodes.end() )
    return iterNodes->second;
  else
    return NULL;
}

OSM_Way * OSM_Model::getFirstOSM_Way()
{
  iterWays = mWays.begin();
  if ( iterWays != mWays.end() )
    return iterWays->second;
  else
    return NULL;

}

OSM_Way * OSM_Model::getNextOSM_Way()
{
  iterWays++;
  if ( iterWays != mWays.end() )
    return iterWays->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob im Modell nicht-aufgelöste Referenzen enthalten sind       //
///////////////////////////////////////////////////////////////////////////////
void OSM_Model::checkReferences()
{
  map<string,OSM_Way*>::iterator      iterWays;
  map<string,OSM_Relation*>::iterator iterRelations;

  for ( iterWays = mWays.begin(); iterWays != mWays.end(); iterWays++ )
  {
    OSM_Way * pWay = iterWays->second;

    if ( !pWay->checkReferences( this ) )
    {
      vIncompleteWayIds.push_back( iterWays->first );
      m_pLogfile->writeLogFile( "Unvollständiger OSM-Way, id = " + pWay->getId() );
    }
  }

  for ( iterRelations = mRelations.begin(); iterRelations != mRelations.end(); iterRelations++ )
  {
    OSM_Relation * pRelation = iterRelations->second;

    if ( !pRelation->checkReferences( this ) )
    {
      vIncompleteRelationIds.push_back( iterRelations->first );
      m_pLogfile->writeLogFile( "Unvollständige OSM-Relation, id = " + pRelation->getId() );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die MultiPolygone und korrigiert fehlende tags                 //
///////////////////////////////////////////////////////////////////////////////
void OSM_Model::checkAndCorrect()
{
  map<string,OSM_Relation*>::iterator iterRelations;
  map<string,OSM_Way*>::iterator      iterWays;
  string                              classification;

  for ( iterWays = mWays.begin(); iterWays != mWays.end(); iterWays++ )
  {
    OSM_Way * pWay = iterWays->second;
    classification = pWay->getClassification();
    if ( classification != "" ) continue;

    for ( unsigned int i = 0; i < pWay->getRelationAnz(); i++ )
    {
      OSM_Relation * pRel = pWay->getRelation( i );
      if ( pRel->getClassification() == "" || 
           ( pRel->getIsMultipolygon() &&  pRel->getRole ( pWay ) == "inner" ) ) continue;
      pWay->setClassification( pRel->getClassification() );
      pWay->transferTags( pRel );
      break;
    }
  }

  for ( iterRelations = mRelations.begin(); iterRelations != mRelations.end(); iterRelations++ )
  {
    OSM_Relation * pRelation = iterRelations->second;
    string relationType = pRelation->getTagValue( "type" );
    if ( relationType == "multipolygon" )
    {
      bool success = pRelation->checkAndCorrectMultiPolygon( this );
      if ( !success )
        vIncompleteRelationIds.push_back( iterRelations->first );
      else
      if ( pRelation->getClassification() == "" )
      {
        string idOuterPolygon = pRelation->getOuterPolygonId();
        if ( idOuterPolygon != "" )
        {
          pRelation->setClassification ( mWays[idOuterPolygon]->getClassification() );
        }      
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Bounding Bux des OSM-Modells                                             //
///////////////////////////////////////////////////////////////////////////////
void OSM_Model::setBounds ( double minLatP, double minLonP, double maxLatP, double maxLonP )
{
  minLat = minLatP;
  minLon = minLonP;
  maxLat = maxLatP;
  maxLon = maxLonP;
}

void OSM_Model::getBounds ( double &minLatP, double &minLonP, double &maxLatP, double &maxLonP )
{
  minLatP = minLat;
  minLonP = minLon;
  maxLatP = maxLat;
  maxLonP = maxLon;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt eine Fehlermeldung in den Logfile                               //
///////////////////////////////////////////////////////////////////////////////
void OSM_Model::addErrorMessage ( std::string errorMessage )
{
  m_pLogfile->writeLogFile( errorMessage );
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert bzw. liefert die OSM-Statistik                                 //
///////////////////////////////////////////////////////////////////////////////
OSM_StatisticsAll * OSM_Model::getOSM_Statistics()
{
  if ( pOSM_StatisticsAll != NULL )
    return pOSM_StatisticsAll;

  pOSM_StatisticsAll = new OSM_StatisticsAll;
  pOSM_StatisticsAll->pNodeStatistics = generateNodeStatistics();
  pOSM_StatisticsAll->pWayStatistics = generateWayStatistics();
  pOSM_StatisticsAll->pRelationStatistics = generateRelationStatistics();

  return pOSM_StatisticsAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die OSM Klassenstatistik                                       //
///////////////////////////////////////////////////////////////////////////////
OSM_Statistics * OSM_Model::generateNodeStatistics()
{
  map<string,OSM_Node*>::iterator iter;
  map<string,string>              tagMap;
  map<string,string>::iterator    iterTagMap;

  OSM_Statistics * pStat = new OSM_Statistics ( "node", mNodes.size() );

  for ( iter = mNodes.begin(); iter != mNodes.end(); iter++ )
  {
    OSM_Node * pNode = iter->second;
    tagMap = pNode->getTags();
    for ( iterTagMap = tagMap.begin(); iterTagMap != tagMap.end(); iterTagMap++ )
    {
      string key   = iterTagMap->first;
      string value = iterTagMap->second;
      pStat->addTag( iterTagMap->first, iterTagMap->second );
    }
  }

  return pStat;
}

OSM_Statistics * OSM_Model::generateWayStatistics()
{
  map<string,OSM_Way*>::iterator iter;
  map<string,string>              tagMap;
  map<string,string>::iterator    iterTagMap;

  OSM_Statistics * pStat = new OSM_Statistics ( "way", mWays.size() );

  for ( iter = mWays.begin(); iter != mWays.end(); iter++ )
  {
    OSM_Way * pWay = iter->second;
    tagMap = pWay->getTags();
    for ( iterTagMap = tagMap.begin(); iterTagMap != tagMap.end(); iterTagMap++ )
      pStat->addTag( iterTagMap->first, iterTagMap->second );
  }

  return pStat;
}

OSM_Statistics * OSM_Model::generateRelationStatistics()
{
  map<string,OSM_Relation*>::iterator iter;
  map<string,string>              tagMap;
  map<string,string>::iterator    iterTagMap;

  OSM_Statistics * pStat = new OSM_Statistics ( "relation", mRelations.size() );

  for ( iter = mRelations.begin(); iter != mRelations.end(); iter++ )
  {
    OSM_Relation * pRelation = iter->second;
    tagMap = pRelation->getTags();
    for ( iterTagMap = tagMap.begin(); iterTagMap != tagMap.end(); iterTagMap++ )
      pStat->addTag( iterTagMap->first, iterTagMap->second );
  }

  return pStat;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle OSM-Objekte                                                 //
///////////////////////////////////////////////////////////////////////////////
size_t OSM_Model::getOSM_Objects ( std::vector<OSM_Object*> &vOSM_Objects )
{
  map<string,OSM_Node*>::iterator iterNodes;
  map<string,OSM_Way*>::iterator iterWays;
  map<string,OSM_Relation*>::iterator iterRelations;

  for ( iterNodes = mNodes.begin(); iterNodes != mNodes.end(); iterNodes++ )
    vOSM_Objects.push_back( iterNodes->second );

  for ( iterWays = mWays.begin(); iterWays != mWays.end(); iterWays++ )
    vOSM_Objects.push_back( iterWays->second );

  for ( iterRelations = mRelations.begin(); iterRelations != mRelations.end(); iterRelations++ )
    vOSM_Objects.push_back( iterRelations->second );

  return vOSM_Objects.size();
}

size_t OSM_Model::getOSM_ObjectsSorted ( std::vector<OSM_Object*> &vOSM_Objects )
{
  map<string,OSM_Node*>::iterator        iterNodes;
  map<string,OSM_Way*>::iterator         iterWays;
  map<string,OSM_Relation*>::iterator    iterRelations;
  multimap<string,OSM_Object*>           mSortedObjects;
  multimap<string,OSM_Object*>::iterator iterSortedObjects;
  string                                 classification;
  OSM_Object                           * pObject;

  for ( iterNodes = mNodes.begin(); iterNodes != mNodes.end(); iterNodes++ )
  {
    pObject = iterNodes->second;
    classification = pObject->getClassificationWithGeometry();
    mSortedObjects.insert(  multimap<string,OSM_Object*>::value_type ( classification, pObject ) );
  }

  for ( iterWays = mWays.begin(); iterWays != mWays.end(); iterWays++ )
  {
    pObject = iterWays->second;
    classification = pObject->getClassificationWithGeometry();
    mSortedObjects.insert(  multimap<string,OSM_Object*>::value_type ( classification, pObject ) );
  }

  for ( iterRelations = mRelations.begin(); iterRelations != mRelations.end(); iterRelations++ )
  {
    pObject = iterRelations->second;
    classification = pObject->getClassificationWithGeometry();
    mSortedObjects.insert(  multimap<string,OSM_Object*>::value_type ( classification, pObject ) );
  }

  for ( iterSortedObjects = mSortedObjects.begin(); iterSortedObjects != mSortedObjects.end(); iterSortedObjects++ )
    vOSM_Objects.push_back( iterSortedObjects->second );

  return vOSM_Objects.size();
}



///////////////////////////////////////////////////////////////////////////////
//  Liefert die Geometrie eines way als MultiCurve                           //
///////////////////////////////////////////////////////////////////////////////
size_t OSM_Model::getLineGeometry ( OSM_Way * pWay, vector<double> &vLat, vector<double> &vLon )
{
  if ( pWay->getIsComplete() == false )  return 0;

  vector<string> vPointIds = pWay->getPoints();

  if ( vPointIds.size() <= 1 ) return 0;

  for ( unsigned int i = 0; i < vPointIds.size(); i++ )
  {
    string pointId = vPointIds[i];
    OSM_Node * pNode = mNodes[pointId];

    vLat.push_back( pNode->getLat() );
    vLon.push_back( pNode->getLon() );
  }

  return vLat.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Geometrie eines geschlossenen way als Polygon                //
///////////////////////////////////////////////////////////////////////////////
size_t OSM_Model::getSurfaceGeometry ( OSM_Way * pWay, vector<double> &vLat, vector<double> &vLon )
{
  bool                            error = false;

  if ( pWay->getIsComplete() == false ) return 0;

  vector<string> vPointIds = pWay->getPoints();
  if ( vPointIds.size() <= 2 || vPointIds[0] != vPointIds[vPointIds.size()-1] ) return 0;

  for ( unsigned int i = 0; i < vPointIds.size(); i++ )
  {
    string     pointId = vPointIds[i];
    OSM_Node * pNode   = mNodes[pointId];

    vLat.push_back( pNode->getLat() );
    vLon.push_back( pNode->getLon() );
  }

  return vLat.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert bzw. liest die SVG-Visualisierungsparameter für OSM              //
///////////////////////////////////////////////////////////////////////////////
OSM_VisualParams * OSM_Model::getOSM_SvgParams()
{
  string               stylesheetFolder;
  string               stylesheetDatei;
  OSM_SvgParamReader ^ pReader;
  String             ^ STYLESHEET_FOLDER;
  cli::array<String^>^ FILES;

  System::Collections::IEnumerator ^ FILES_ENUM;

  if ( pOSM_SvgParams != NULL )
    return pOSM_SvgParams;

  pOSM_SvgParams = new OSM_VisualParams ();
  pOSM_SvgParams->setBreiteDisplay( planausgabeBreite );
  pOSM_SvgParams->setHoeheDisplay( planausgabeHoehe );

  pReader = gcnew OSM_SvgParamReader ( globalScale );

  stylesheetFolder = Constant::getStylesheetPath( OSM );

  STYLESHEET_FOLDER = QuConvert::ToString ( stylesheetFolder );
  bool exist = Directory::Exists ( STYLESHEET_FOLDER );
  if ( exist )
  {
    FILES = Directory::GetFiles ( STYLESHEET_FOLDER );

    FILES_ENUM = FILES->GetEnumerator();
    while ( FILES_ENUM->MoveNext() )
    {
      String^ FILE_NAME = safe_cast<String^>(FILES_ENUM->Current);
      QuConvert::systemStr2stdStr ( stylesheetDatei, FILE_NAME );
      pReader->read ( stylesheetDatei , pOSM_SvgParams );
    }
  }

  return pOSM_SvgParams;
}



///////////////////////////////////////////////////////////////////////////////
//  Schreibt ein OSM_Modell                                                  //
///////////////////////////////////////////////////////////////////////////////
bool  OSM_Model::writeOSM_SVG ()
{
  pOSM_SvgParams = getOSM_SvgParams();
  if ( pOSM_SvgParams == NULL ) return false;

  OSM_SvgWriter ^ pWriter = gcnew OSM_SvgWriter ( pOSM_SvgParams, this, delta );

  string ueberschriftPfad = Constant::getSystemFolder() + svgUeberschriftDatei;
  pWriter->writeUeberschrift ( ueberschriftPfad );

  string uebersichtPfad =  Constant::getSystemFolder() + svgUebersichtDatei;
  pWriter->writeUebersicht( uebersichtPfad,globalScale );

  string kartePfad = Constant::getSystemFolder() + svgKartenDatei;
  pWriter->write ( kartePfad, globalScale );

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest ein OSM-Modell ein                                                 //
///////////////////////////////////////////////////////////////////////////////
int OSM_Model::readOsmModel ( XmlTextReader ^ pXmlReader, OSM_StatisticsAll * pOSM_Statistics )
{
  OSM_Reader   ^ pReader;

  if ( pOSM_Statistics != NULL )
  {
    pReader = gcnew OSM_Reader ( pOSM_Statistics, pXmlReader );
    return pReader->read();
  }
  else
  {
    pReader = gcnew OSM_Reader ( this, pXmlReader );
    pReader->setSemanticClassification( pOSMSemanticClassification );
    int anz = pReader->read();

    checkReferences();
    checkAndCorrect();

    return anz;
  }
}

size_t OSM_Model::readOsmModelWithFilter ( OSM_TagFilter * pFilter, string fileName, System::Windows::Forms::ListBox ^ listBoxMeldungen  )
{
  OSM_ReaderWithFilter ^ pReader = gcnew OSM_ReaderWithFilter ( this, pFilter, listBoxMeldungen );
  pReader->setSemanticClassification( pOSMSemanticClassification );
	size_t anz = pReader->read( fileName );

  checkReferences();
  checkAndCorrect();

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  OpenStreetmap Basisobjekt                                                //
///////////////////////////////////////////////////////////////////////////////
OSM_Object::OSM_Object ( std::string idP )
{
  id = idP;
}

OSM_Object::~ OSM_Object()
{

}

void OSM_Object::addTag ( std::string key, std::string value )
{
  mTags[key] = value;
}

/*
void OSM_Object::addTags ( std::map<std::string,std::string> &mTagsP )
{
  mTags = mTagsP;
}
*/
std::string OSM_Object::getTagValue ( std::string key )
{
  map<string,string>::iterator iter;

  iter = mTags.find( key );
  if ( iter != mTags.end() ) return iter->second;
  else                       return "";
}

void OSM_Object::clearTags()
{
  mTags.clear();
}

void OSM_Object::transferTags( OSM_Object * pOSM_Object )
{
  map<string,string>            mTagsToTransfer;
   map<string,string>::iterator iter;

  mTagsToTransfer = pOSM_Object->getTags();

  for ( iter = mTagsToTransfer.begin(); iter != mTagsToTransfer.end(); iter++ )
  {
    if ( mTags.find( iter->first ) == mTags.end() )
      addTag ( iter->first, iter->second );
  }
}

OSM_Relation * OSM_Object::getRelation ( unsigned int index )
{
  if ( index < 0 || index > mUsedInRelation.size() )
    return NULL;
  else
    return mUsedInRelation[index];
}

///////////////////////////////////////////////////////////////////////////////
//  OpenStreetmap Node-Objekt                                                //
///////////////////////////////////////////////////////////////////////////////
OSM_Node::OSM_Node( std::string idP, double latP, double lonP ): OSM_Object ( idP )
{
  lat = latP;
  lon = lonP;
}

OSM_Node::~OSM_Node()
{

}

void OSM_Node::addReference ( OSM_Object * pObj )
{
  sRefIds.insert ( pObj->getId() );
}

void OSM_Node::getGeometry ( double &latP, double &lonP )
{
  latP = lat;
  lonP = lon;
}

///////////////////////////////////////////////////////////////////////////////
//  OpenStreetmap Way-Objekt                                                 //
///////////////////////////////////////////////////////////////////////////////
OSM_Way::OSM_Way ( std::string idP ) : OSM_Object ( idP )
{
  isComplete = false;
}

OSM_Way::~OSM_Way()
{

}

void OSM_Way::addPoint ( std::string id )
{
  vPointIds.push_back( id );
}

void OSM_Way::addPoints ( std::vector<std::string> &vPointIdsP )
{
  for ( unsigned int i = 0; i < vPointIdsP.size(); i++ )
    vPointIds.push_back( vPointIdsP[i] );
}

OSM_OBJECT_TYPE OSM_Way::getType()
{  
  if ( vPointIds.size() > 1 )
  {
    if ( vPointIds[0] == vPointIds[ vPointIds.size()-1] )
      return OSM_AREA;
    else
      return OSM_WAY;
  }
  return OSM_INVALID;  
}

string OSM_Way::getTypeString()
{  
  if ( vPointIds.size() > 1 )
  {
    if ( vPointIds[0] == vPointIds[ vPointIds.size()-1] )
      return "_FL_";
    else
      return "_LI_";
  }
  return "";  
}

std::string OSM_Way::getClassificationWithGeometry() 
{  
  if ( vPointIds.size() > 1 )
  {
    if ( vPointIds[0] == vPointIds[ vPointIds.size()-1] )
      return classification + "_area"; 
    else
      return classification + "_way";
  }

  return ""; 
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüfung, ob alle ref. Way-Punkte vorhanden sind                      //
///////////////////////////////////////////////////////////////////////////////
bool OSM_Way::checkReferences(   OSM_Model * pOSM_Model )
{
  unsigned int i;

  for ( i = 0; i < vPointIds.size(); i++ )
  {
    OSM_Node * pNode = pOSM_Model->getOSM_Node( vPointIds[i] );
    if ( pNode == NULL )
    {
      isComplete = false;
      return isComplete;
    }
  }

  isComplete = true;
  return isComplete;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob der way geschlossen ist                                    //
///////////////////////////////////////////////////////////////////////////////
bool OSM_Way::isClosed()
{
  if ( vPointIds.size() > 0 && vPointIds[0] == vPointIds[vPointIds.size()-1] )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die id des Start- bzw. Endpunktes des way                        //
///////////////////////////////////////////////////////////////////////////////
string OSM_Way::getStartPointId()
{
  if ( vPointIds.size() >  0 )
    return vPointIds[0];
  else
    return "";
}

string OSM_Way::getEndPointId()
{
  if ( vPointIds.size() >  0 )
    return vPointIds[vPointIds.size()-1];
  else
    return "";
}


///////////////////////////////////////////////////////////////////////////////
//  MultiWay                                                                 //
///////////////////////////////////////////////////////////////////////////////
MultiWay::MultiWay()
{

}


MultiWay::~MultiWay()
{

}

void MultiWay::addWay ( OSM_Way * pWay, bool ori )
{
  vWays.push_back( pWay );
  vOrientations.push_back( ori );
}

vector<string> MultiWay::getNodeIds ()
{
  vector<string> vNodeIds;
  vector<string> vNodeIdsLoc;
	size_t				 i;
	int		         j;

  for ( i = 0; i < vWays.size(); i++ )
  {
    vNodeIdsLoc.clear();
    OSM_Way * pWay = vWays[i];
    vNodeIdsLoc = pWay->getPoints();

    if ( vOrientations[i] == true )
    {
      for ( j = 0; j < (int)vNodeIdsLoc.size(); j++ )
        vNodeIds.push_back( vNodeIdsLoc[j] );
    }
    else
    {
      for ( j = (int)vNodeIdsLoc.size()-1; j >= 0; j-- )
        vNodeIds.push_back( vNodeIdsLoc[j] );
    }
  }
  return vNodeIds;
}

bool MultiWay::isClosed()
{
  string startNodeId;
  string endNodeId;

	size_t anz = vWays.size();
  if ( anz == 0 )
    return false;

  if ( vOrientations[0] == true )
    startNodeId = vWays[0]->getStartPointId();
  else
    startNodeId = vWays[0]->getEndPointId();

  if ( vOrientations[anz-1] == false )
    endNodeId = vWays[anz-1]->getStartPointId();
  else
    endNodeId = vWays[anz-1]->getEndPointId();

  if ( startNodeId == endNodeId && startNodeId != "" )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  OpenStreetmap Relation-Objekt                                            //
///////////////////////////////////////////////////////////////////////////////
OSM_Relation::OSM_Relation ( std::string idP ) : OSM_Object ( idP )
{
  isComplete     = false;
  isMultipolygon = false;
}

OSM_Relation::~OSM_Relation()
{

}

void OSM_Relation::addMember ( std::string type, std::string id, std::string role )
{
  string idString;

  if ( type == "node" )
    idString = "N";
  else
  if ( type == "way" )
    idString = "W";
  else
  if ( type == "relation" )
    idString = "R";
  else
    idString = ";";

  idString = idString + id;
  vObjectIds.push_back( idString );
  vRoles.push_back( role );
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob alle Relations-Members im Datensatz vorhanden sind         //
//  dabei wird vorausgesetzt, dass die Vollständigkeit der ways bereits      //
//  überprüft wurde                                                          //
///////////////////////////////////////////////////////////////////////////////
bool OSM_Relation::checkReferences(  OSM_Model * pOSM_Model )
{
  unsigned int i;
  bool         isComplete = true;
  bool         b;

  for ( i = 0; i < vObjectIds.size(); i++ )
  {
    string objectIdWithType = vObjectIds[i];

    char OSM_type   = objectIdWithType[0];
    string objectId = objectIdWithType.substr( 1 );

    switch ( OSM_type )
    {
    case 'N':   // Node
      {
        OSM_Node * pNode = pOSM_Model->getOSM_Node( objectId );
        if ( pNode == NULL )
          isComplete = false;          
        else
          pNode->addRelation( this );       
      }

    case 'W':   // Way
      {
        OSM_Way * pWay = pOSM_Model->getOSM_Way( objectId );
        if ( pWay != NULL )
        {
          pWay->addRelation( this );
          b = pWay->getIsComplete();
          if (!b ) isComplete = false;
        }
        else
          isComplete = false;
      }
      break;

    case 'R':  //  Relation
      {
        OSM_Relation * pRel = pOSM_Model->getOSM_Relation( objectId );
        if ( pRel != NULL )
        {
          pRel->addRelation( this );
          b = pRel->checkReferences( pOSM_Model );
          if (!b ) return isComplete = false;
        }
        else
          isComplete = false;
      }
      break;

    default:
      isComplete = false;
      errorCode = "Ungültiger type in relation";
    }   
  }

  return isComplete;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Korrektheit eines Multi_Polygon. Restriktionen:            //
//  - role entweder "outer" (genau einmal) oder "inner"                      //
//  - Alle referierten ways sind geschlossen                                 //
//  Die tags des "outer" Polygons werden übernommen und im Ursprung gelöscht //
///////////////////////////////////////////////////////////////////////////////
bool OSM_Relation::checkAndCorrectMultiPolygon ( OSM_Model * pOSM_Model )
{
  string         relationType;
  unsigned int   i;
  vector<string> vOuter;
  vector<string> vInner;

  if ( errorCode != "" )
    return false;

  relationType = getTagValue( "type" );
  if ( relationType != "multipolygon" )
  {
    isMultipolygon = false;
    return true;
  }

  isMultipolygon = true;

  int anzOuter = 0;
  for ( i = 0; i < vObjectIds.size(); i++  )
  {
    string objectIdWithType = vObjectIds[i];

    char OSM_type   = objectIdWithType[0];
    string objectId = objectIdWithType.substr( 1 );
    string role     = vRoles[i];

    if ( OSM_type != 'W' )
    {
      errorCode = "Ungültiger member-typ in MultiPolygon";
      return false;
    }

    if ( role != "outer" && role != "inner" )
    {
      errorCode = "Ungültiges role-Tag in MultiPolygon";
      return false;
    }


    if ( role == "outer" )
      vOuter.push_back( objectId );      
    else
    {
      OSM_Way * pInnerRing = pOSM_Model->getOSM_Way( objectId );
      if ( pInnerRing != NULL )
      {
        if ( pInnerRing->isClosed() )
        {
          MultiWay * pInner = new MultiWay;
          pInner->addWay( pInnerRing, true );
          vInnerContours.push_back( pInner );
        }
        else
          vInner.push_back( objectId );
      }
    }
  }

  if ( vOuter.size() == 0 )
  {
    errorCode = "MultiPolygon hat keine outer-Geometrie";
    return false;
  }
  else
  if ( vOuter.size() == 1 )
  {
    OSM_Way * pOuterRing = pOSM_Model->getOSM_Way( vOuter[0] );
    if ( pOuterRing == NULL )
    {
      errorCode = "Unvollständige Relation";
      return false;
    }

    if ( pOuterRing->getClassification() != "" )
    {
      if ( classification == "" )
        classification =  pOuterRing->getClassification();
    }
    pOuterRing->setClassification( "MultiPolygonPart" );

    transferTags ( pOuterRing );
    pOuterRing->clearTags(); 

    MultiWay * pOuter = new MultiWay;
    pOuter->addWay( pOuterRing, true );
    vOuterContours.push_back( pOuter );
  }
  else
    generateMultiWays ( vOuter, pOSM_Model );

  return true;
}

void OSM_Relation::generateMultiWays ( vector<string> vOuterIds, OSM_Model * pOSMModel )
{
  map<OSM_Way*,int>    mWays;
  map<string,OSM_Way*> mStartNodes;
  map<string,OSM_Way*> mEndNodes;
  vector<bool>         vWaysUsed;
	int                  anzWaysUsed, anzWays;
	int                  i, j;

  anzWays     =  (int)vOuterIds.size();
  anzWaysUsed = 0;

  for ( i = 0; i < anzWays; i++ )
  {
    vWaysUsed.push_back( false );
    OSM_Way * pWay = pOSMModel->getOSM_Way( vOuterIds[i] );
    if ( pWay != NULL )
    {
      mWays[pWay]    = i;
      mStartNodes[pWay->getStartPointId()] = pWay;
      mEndNodes[pWay->getEndPointId()]     = pWay;
    }
  }

  while ( anzWaysUsed < anzWays )
  {
    int startIndex = -1;
    for ( j = 0; j < anzWays; j++ )
    {
      if ( vWaysUsed[j] == false )
      {
        startIndex = j;
        break;
      }
    }
    if ( startIndex == -1 ) break;

    OSM_Way * pStartWay = pOSMModel->getOSM_Way( vOuterIds[startIndex] );
    string startPointId = pStartWay->getStartPointId();
    string endPointId   = pStartWay->getEndPointId();

    MultiWay * pMultiWay = new MultiWay;
    pMultiWay->addWay( pStartWay, true );
    vWaysUsed[startIndex] = true;

    while ( startPointId != endPointId )
    {
      map<string,OSM_Way*>::iterator iter;

      iter =  mStartNodes.find( endPointId );
      if ( iter != mStartNodes.end() )
      {
        OSM_Way * pNextWay = iter->second;
        int index = mWays[pNextWay];
        if ( vWaysUsed[index] == false )
        {
          vWaysUsed[index] = true;
          pMultiWay->addWay( pNextWay, true );
          endPointId = pNextWay->getEndPointId();
        }
        else
          break;
      }
      else
      if ( iter != mEndNodes.end() )
      {
        OSM_Way * pNextWay = iter->second;
        int index = mWays[pNextWay];
        if ( vWaysUsed[index] == false )
        {
          vWaysUsed[index] = true;
          pMultiWay->addWay( pNextWay, false );
          endPointId = pNextWay->getStartPointId();
        }
        else
          break;
      }

    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert für ein Multi-Polygon die Aussen- und Innenkonturen              //
///////////////////////////////////////////////////////////////////////////////
/*OSM_Way * OSM_Relation::getRings( std::vector<OSM_Way*> &vInner, OSM_Model * pOSM_Model )
{
  OSM_Way * pOuter = NULL;

  if ( errorCode != "" )
    return NULL;

  for ( unsigned i = 0; i < vObjectIds.size(); i++ )
  {
    string objectIdWithType = vObjectIds[i];
    string objectId = objectIdWithType.substr( 1 );
    string role     = vRoles[i];

    if ( role == "outer" )
      pOuter = pOSM_Model->getOSM_Way( objectId );
    else
      vInner.push_back( pOSM_Model->getOSM_Way( objectId ) );
  }
  return pOuter;
}
*/
///////////////////////////////////////////////////////////////////////////////
//  Liefert die Rolle des OSM-Objektes  innerhalb der Relation               //
///////////////////////////////////////////////////////////////////////////////
std::string OSM_Relation::getRole ( OSM_Object * pObj )
{
  for ( unsigned int i = 0; i < vObjectIds.size(); i++ )
  {
    if ( pObj->getId() == vObjectIds[i] )
      return vRoles[i];
  }

  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den OSM_Way mit der Rolle "outer"                                //
///////////////////////////////////////////////////////////////////////////////
string OSM_Relation::getOuterPolygonId()
{
  if ( !isMultipolygon )
    return "";

  for ( unsigned int i = 0; i < vRoles.size(); i++ )
  {
    if ( vRoles[i] == "outer" )
    {
      string id = vObjectIds[i];
      return id.substr( 1 );
    }
  }

  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Ids aller aller referierter OSM_Relations                    //
///////////////////////////////////////////////////////////////////////////////
void OSM_Relation::getReferencesRelations ( std::set<std::string> &sRelIds )
{
  for ( unsigned int i = 0; i < vObjectIds.size(); i++ )
  {
    string objectIdWithType = vObjectIds[i];

    char OSM_type   = objectIdWithType[0];
    string objectId = objectIdWithType.substr( 1 );

    switch ( OSM_type )
    {
    case 'R':  //  Relation
      sRelIds.insert ( objectId );
      break;
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Statistik der benutzten Tags (key,value) für eine OSM-Klasse             //
///////////////////////////////////////////////////////////////////////////////
OSM_Statistics::OSM_Statistics ( std::string classNameP, size_t objAnzP )
{
  className = classNameP;
  objAnz    = objAnzP;
}

OSM_Statistics::~OSM_Statistics()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines tags                                                    //
///////////////////////////////////////////////////////////////////////////////
void OSM_Statistics::addTag ( std::string key, std::string value )
{
//  map<string,int>                                           mValueCountMap;
  map<string,int>::iterator                                 iter;
  std::map<std::string,std::map<std::string,int>>::iterator pos;

  pos = mTagMap.find( key );
  if ( pos != mTagMap.end() )
  {
//    mValueCountMap = mTagMap[key];

    iter = mTagMap[key].find( value );
    if ( iter == mTagMap[key].end() )
       mTagMap[key][value] = 1;
    else
       mTagMap[key][value]++;
  }
else
  mTagMap[key][value] = 1;


}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Statistik                                                    //
///////////////////////////////////////////////////////////////////////////////
size_t OSM_Statistics::getStat ( std::vector<std::string> &vKeys, std::vector<std::map<std::string,int>> &vValueCounts )
{
  std::map<std::string,std::map<std::string,int>>::iterator iter;

  for ( iter = mTagMap.begin(); iter != mTagMap.end(); iter++ )
  {
    vKeys.push_back( iter->first );
    vValueCounts.push_back( iter->second );
  }
  return vKeys.size();
}

/////////////////////////////////////////////////////////////////////////////////
//  Liefet, wie häufig ein bestimmter key (mit beliebigen values) benutzt wird //
/////////////////////////////////////////////////////////////////////////////////
size_t OSM_Statistics::getTagValueAnz ( std::string key )
{
  map<string,int>           mValueCountMap;
  map<string,int>::iterator iter;

	size_t anz = 0;
  for ( iter = (mTagMap[key]).begin(); iter != (mTagMap[key]).end(); iter++ )
    anz = anz + iter->second;

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Zusammenfassung der einzelnen Statistiken                                //
///////////////////////////////////////////////////////////////////////////////
OSM_StatisticsAll::OSM_StatisticsAll()
{
  pNodeStatistics     = new OSM_Statistics ( "node" );
  pWayStatistics      = new OSM_Statistics ( "way" );
  pRelationStatistics = new OSM_Statistics ( "relation" );

}

OSM_StatisticsAll::~OSM_StatisticsAll()
{
  delete pNodeStatistics;
  delete pWayStatistics;
  delete pRelationStatistics;
}

///////////////////////////////////////////////////////////////////////////////
//  OSM-Klassifikationsschema                                                //
///////////////////////////////////////////////////////////////////////////////
OSM_Class::OSM_Class()
{
  osm_key     = "";
  featureType = "";
  priority    = 1;
  pointGeometry = true;
  lineGeometry  = true;
  areaGeometry  = true;
  relation      = true;
}

OSM_Class::~OSM_Class()
{

}



OSM_SemanticClassification::OSM_SemanticClassification ()
{

}

OSM_SemanticClassification::~OSM_SemanticClassification()
{

}

void OSM_SemanticClassification::readSemanticClassification ( std::string fileName )
{
  String        ^ pActElement;
  String        ^ STR;
  string          stdString;
  String        ^ fName   = gcnew String( fileName.c_str() );
  XmlTextReader ^ pReader = gcnew XmlTextReader( fName );
  OSM_Class       osmClass;

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      pActElement = pReader->Name;

      if ( pActElement->CompareTo( "OSM_Class") == 0 )
      {
        STR = pReader->GetAttribute( "pointGeometry" );
        if ( STR->CompareTo( "true" ) == 0 )
          osmClass.pointGeometry = true;
        else
          osmClass.pointGeometry = false;

        STR = pReader->GetAttribute( "lineGeometry" );
        if ( STR->CompareTo( "true" ) == 0 )
          osmClass.lineGeometry = true;
        else
          osmClass.lineGeometry = false;

        STR = pReader->GetAttribute( "areaGeometry" );
        if ( STR->CompareTo( "true" ) == 0 )
          osmClass.areaGeometry = true;
        else
          osmClass.areaGeometry = false;

        STR = pReader->GetAttribute( "relation" );
        if ( STR->CompareTo( "true" ) == 0 )
          osmClass.relation = true;
        else
          osmClass.relation = false;

        STR = pReader->GetAttribute ( "priority" );
        osmClass.priority = XmlConvert::ToInt32( STR );

        STR = pReader->GetAttribute ( "FeatureType" );
        QuConvert::systemStr2stdStr( stdString, STR );
        osmClass.featureType = stdString;

        STR = pReader->GetAttribute ( "OSM_key" );
        QuConvert::systemStr2stdStr( stdString, STR );
        osmClass.osm_key = stdString;

        mClassification[stdString] = osmClass;
      }

    }
  }

}

std::string OSM_SemanticClassification::getFeatureType ( OSM_Object * pObject )
{
  map<string,string>                        mTags;
  map<string,string>::iterator              iter;
  std::map<std::string,OSM_Class>::iterator iterClasses;
  map<int,OSM_Class>                        mClasses;
  OSM_Class                                 osmClass;

  mTags = pObject->getTags();
  for ( iter = mTags.begin(); iter != mTags.end(); iter++ )
  {
    string key = iter->first;
    iterClasses = mClassification.find ( key );
    if ( iterClasses != mClassification.end() )
    {
      osmClass = iterClasses->second;
      switch ( pObject->getType() )
      {
      case OSM_NODE:
        if (osmClass.pointGeometry == true )
          mClasses[osmClass.priority] = osmClass;
        break;

      case OSM_WAY:
        if (osmClass.lineGeometry == true )
          mClasses[osmClass.priority] = osmClass;

      case OSM_AREA:
        if (osmClass.areaGeometry == true )
          mClasses[osmClass.priority] = osmClass;

      case OSM_RELATION:
        if (osmClass.relation == true )
          mClasses[osmClass.priority] = osmClass;
      }
    }    
  }

  if ( mClasses.size() == 0 )
    return "";
  else 
  {
    osmClass = mClasses.begin()->second;
    return osmClass.featureType;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Definiert einen Filter für mehrere OSM-Tags                              //
///////////////////////////////////////////////////////////////////////////////
OSM_TagFilter::OSM_TagFilter( string nameP, TAG_FILTER_COMBINATION combinationP )
{
  filterName = nameP;
  combination = combinationP;
}

OSM_TagFilter::~OSM_TagFilter()
{
  map<string,OSM_SingleTagFilter*>::iterator iter;

  for ( iter = mSingleFilters.begin(); iter != mSingleFilters.end(); iter++ )
    delete iter->second;
}

OSM_SingleTagFilter * OSM_TagFilter::addSingleFilter ( std::string osmKeyP, SINGLE_FILTER_TYPE typeP )
{
  if ( mSingleFilters.find( osmKeyP ) != mSingleFilters.end() )
    return NULL;

  OSM_SingleTagFilter * pFilter = new OSM_SingleTagFilter ( osmKeyP, typeP );
  mSingleFilters[osmKeyP] = pFilter;

  return pFilter;
}

bool OSM_TagFilter::check ( std::map<std::string,std::string> & mTags )
{
  int                                        anzSuccess = 0;
  map<string,OSM_SingleTagFilter*>::iterator iter;

  if ( mSingleFilters.size() == 0 ) return true;

  for ( iter = mSingleFilters.begin(); iter != mSingleFilters.end(); iter++ )
  {
    string                key = iter->first;
    OSM_SingleTagFilter * pFilter = iter->second;

    if ( mTags.find ( key ) != mTags.end() )
    {
      bool success = pFilter->check( key, mTags[key] );
      if ( success && combination == OR ) return true;
    }
    else
      if ( combination == AND ) return false;
  }

  if ( combination == AND )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Definiert einen Filter für einen einzelnen OSM-Tag                       //
///////////////////////////////////////////////////////////////////////////////
OSM_SingleTagFilter::OSM_SingleTagFilter( std::string osmKeyP, OSM_TagFilter::SINGLE_FILTER_TYPE typeP )
{
  osmKey = osmKeyP;
  type   = typeP; 
}
OSM_SingleTagFilter::~OSM_SingleTagFilter()
{

}

void OSM_SingleTagFilter::addValue ( std::string valueP )
{
  tagValues.insert( valueP );
}

bool OSM_SingleTagFilter::check ( string osmKeyP, string valueP )
{
  bool found;

  if ( osmKey != osmKeyP ) return false;
  if ( tagValues.size() == 0 ) return true;

  if ( tagValues.find ( valueP) != tagValues.end() )
    found = true;
  else
    found = false;

  if ( ( found && type == OSM_TagFilter::POSITIVE ) ||
       ( !found && type == OSM_TagFilter::NEGATIVE ) )
       return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Collection von OSM_TagFilter Objekten                                    //
///////////////////////////////////////////////////////////////////////////////
OSM_TagFilterCollection::OSM_TagFilterCollection ()
{

}
OSM_TagFilterCollection::~OSM_TagFilterCollection()
{
   map<string,OSM_TagFilter*>::iterator iter;

   for ( iter = mTagFilters.begin(); iter != mTagFilters.end(); iter++ )
   {
     delete iter->second;
   }
}

void OSM_TagFilterCollection::readFilterCollection ( std::string filename )
{
  XmlTextReader       ^ pXMLReader = gcnew XmlTextReader ( QuConvert::ToString( filename ) );
  OSM_TagFilter       * pOSMTagFilter       = NULL;
  OSM_SingleTagFilter * pOSMSingleTagFilter = NULL;
  String              ^ pActElement;


  while( pXMLReader->Read() )
  {
    if ( pXMLReader->NodeType == XmlNodeType::Element )
    {
      pActElement = pXMLReader->Name;

      if ( pActElement->CompareTo( "OSM_TagFilter" ) == 0 )
      {
        string filterNameStd;

        String ^ filterName = pXMLReader->GetAttribute( "filterName" );
        QuConvert::systemStr2stdStr( filterNameStd, filterName );

        String ^ combination = pXMLReader->GetAttribute( "combination" );

        if ( combination->CompareTo( "AND" ) == 0 )
          pOSMTagFilter = new OSM_TagFilter ( filterNameStd, OSM_TagFilter::AND );
        else
        if ( combination->CompareTo( "OR" ) == 0 )
          pOSMTagFilter = new OSM_TagFilter ( filterNameStd, OSM_TagFilter::OR );

        mTagFilters[filterNameStd] = pOSMTagFilter;
      }
      else
      if ( pActElement->CompareTo( "OSM_SingleTagFilter" ) == 0 )
      {
        string                            osmKeyStd;

        String ^ osmKey = pXMLReader->GetAttribute ( "osmKey" );
        QuConvert::systemStr2stdStr( osmKeyStd, osmKey );

        String ^ filterType = pXMLReader->GetAttribute( "type" );
        if ( filterType-> CompareTo ( "POSITIVE" ) == 0 )
          pOSMSingleTagFilter = pOSMTagFilter->addSingleFilter( osmKeyStd, OSM_TagFilter::POSITIVE );
        else
        if ( filterType-> CompareTo ( "NEGATIVE" ) == 0 )
          pOSMSingleTagFilter = pOSMTagFilter->addSingleFilter( osmKeyStd, OSM_TagFilter::NEGATIVE );
      }
      else
      if ( pActElement->CompareTo( "tagValue" ) == 0 )
      {
        string tagValueStd;

        String ^ tagValue = pXMLReader->ReadString();
        QuConvert::systemStr2stdStr( tagValueStd, tagValue );

        pOSMSingleTagFilter->addValue ( tagValueStd );
      }
    }
    else
    if ( pXMLReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = pXMLReader->Name;
      if ( pActElement->CompareTo( "OSM_TagFilterCollection" ) == NULL )
        break;
    }
  }
}

size_t OSM_TagFilterCollection::getTagFilterNames ( std::vector<std::string> & vNames )
{ 
  map<string,OSM_TagFilter*>::iterator iter;

  for ( iter = mTagFilters.begin(); iter != mTagFilters.end(); iter++ )
    vNames.push_back( iter->first );

  return vNames.size();
}

OSM_TagFilter * OSM_TagFilterCollection::getTagFilter ( std::string tagFilterName )
{
  map<string,OSM_TagFilter*>::iterator iter;

  iter = mTagFilters.find( tagFilterName );
  if ( iter != mTagFilters.end() )
    return iter->second;
  else
    return NULL;
}



   








