#include "stdafx.h"
#include <time.h>

#include "TimeX.h"
#include "TimeSeries.h"
#include "Convert.h"
#include "UnitOfMeasure.h"

#include "TimeSeriesRepository.h"

///////////////////////////////////////////////////////////////////////////////
//  TimeSeriesRepository                                                     //
//  Klasse zur XML-basierten Speicherung von Zeitreihen                      //
///////////////////////////////////////////////////////////////////////////////
TimeSeriesRepository::TimeSeriesRepository ( UOMList * pUOMList, std::string repositoryFolderPath )
{
  m_pUOMList = pUOMList;
  m_repositoryFolderPath = repositoryFolderPath;

  m_energyADENamespace     = "http://www.sig3d.org/citygml/2.0/energy/1.0";
  m_timeSeriesADENamespace = "http://www.sig3d.org/citygml/2.0/timeSeries/1.0";

  m_pTimeSeriesSetsToSave = new RegularTimeSeriesSets;

}

TimeSeriesRepository::~TimeSeriesRepository()
{
  std::map<std::string,RegularTimeSeriesSetProperties*>::iterator iter;
  std::map<std::string,RegularTimeSeriesSet*>::iterator iterSets;

  for ( iter = m_mTimeSeriesSetMetaDataMap.begin(); iter != m_mTimeSeriesSetMetaDataMap.end(); ++iter )
    delete iter->second;


  for ( iterSets = m_mTimeSeriesSetMap.begin(); iterSets != m_mTimeSeriesSetMap.end(); ++iterSets )
    delete iterSets->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Geht durch alle Dateien des Repositories und baut die maps auf           //
///////////////////////////////////////////////////////////////////////////////
void TimeSeriesRepository::scanRepository()
{
  cli::array<String^> ^ files;
  String              ^ repositoryFolder = QuConvert::ToString ( m_repositoryFolderPath );

  resetRepository();

  files = Directory::GetFiles( repositoryFolder );

  for ( int i = 0; i < files->Length; i++ )
  {
    String ^ source = files[i];
    scanFile ( source );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liest einen Repository-File ein und speichert die Metadaten              //
///////////////////////////////////////////////////////////////////////////////
void TimeSeriesRepository::scanFile ( String ^ fileNameP )
{
  XmlTextReader                  ^ pReader = gcnew XmlTextReader( fileNameP );
  String                         ^ STR;
  std::string                      timeSeriesSetId;
  std::string                      timeSeriesSetPropertiesId;
  std::string                      fileName;
  RegularTimeSeriesProperties    * pTimeSeriesProperties;
  RegularTimeSeriesSetProperties * pMetaData;
  std::vector<RegularTimeSeriesProperties*> vTimeSeriesProperties;

  QuConvert::systemStr2stdStr ( fileName, fileNameP ); 

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      String ^ pActElement = pReader->LocalName;

      if ( pActElement->CompareTo( "RegularTimeSeriesSet" ) == 0 )
      {
        STR = pReader->GetAttribute( "gml:id" );
        QuConvert::systemStr2stdStr( timeSeriesSetId, STR );
        m_mTimeSeriesFileMap[timeSeriesSetId] = fileName;
      }
      else
      if ( pActElement->CompareTo( "TimeValuesProperties" ) == 0 )
      {
        pTimeSeriesProperties = new RegularTimeSeriesProperties;
        vTimeSeriesProperties.push_back( pTimeSeriesProperties );
      }
      else
      if ( pActElement->CompareTo( "thematicDescription" ) == 0 )
      {
        string str;
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( str, STR );
        pTimeSeriesProperties->setThematicDescription( str );
      }
      else
      if ( pActElement->CompareTo( "values" ) == 0 )
      {
        string   uom;
        String ^ UOMstr = pReader->GetAttribute( "uom" );
        QuConvert::systemStr2stdStr( uom, UOMstr );
        UOM * pUOM = m_pUOMList->getUOMFromIdentifier( uom );
        if ( pUOM != NULL )
          pTimeSeriesProperties->setUomSymbol( pUOM->getSign() );
        else
          pTimeSeriesProperties->setUomSymbol ( uom );
      }
      else
      if ( pActElement->CompareTo( "RegularTimeSeriesSetProperties" ) == 0 )
      {
        pMetaData = new RegularTimeSeriesSetProperties ( "" );
        pMetaData->readFromXML( pReader );
        timeSeriesSetPropertiesId = pMetaData->getGmlId();
        m_mTimeSeriesSetPropertiesMap[timeSeriesSetPropertiesId] = pMetaData;
        m_mTimeSeriesSetMetaDataMap[timeSeriesSetId]             = pMetaData;
        m_mTimeSeriesSetMetaDataMapInv[pMetaData]                = timeSeriesSetId;

        std::vector<std::string> keyWords = pMetaData->getKeyWords();
        for ( unsigned int i = 0; i < keyWords.size(); ++i )
        {
          m_mKeyWordsToProperties.insert(std::multimap<std::string,std::string>::value_type
            ( keyWords[i], timeSeriesSetPropertiesId )  );
          m_sKeyWords.insert( keyWords[i]);
        }

        for ( unsigned int i = 0; i < pMetaData->getRelatedObjectAnz(); ++i )
        {
          RelatedObject * pRelObj = pMetaData->getRelatedObject( i );
         m_mRelObjToProperties.insert( std::multimap<std::string,std::string>::value_type
           ( pRelObj->getId(), timeSeriesSetPropertiesId )  );
         m_sRelObjIds.insert( pRelObj->getId() );
         m_mRelObjIdsToNames[pRelObj->getId()] = pRelObj->getName();
        }
      }
    }
    else
    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      String ^ pActElement = pReader->LocalName;
      if ( pActElement->CompareTo( "RegularTimeSeries" ) == 0 )
      {
        for ( unsigned int i = 0; i < vTimeSeriesProperties.size(); i++ )
          pMetaData->addTimeSeriesProperties( vTimeSeriesProperties[i] );
        vTimeSeriesProperties.clear();
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle im Repository vorkommenden referierten Objekte              //
///////////////////////////////////////////////////////////////////////////////
size_t TimeSeriesRepository::getRelObjs ( std::vector<std::string> &vRelObjIds, std::vector<std::string> &vRelObjNames )
{
  std::set<std::string>::iterator iterSet;

  for ( iterSet = m_sRelObjIds.begin(); iterSet != m_sRelObjIds.end(); ++iterSet )
  {
    std::string relObjId = *iterSet;
    vRelObjIds.push_back( relObjId );
    vRelObjNames.push_back( m_mRelObjIdsToNames[relObjId] );
  }
  return vRelObjIds.size(); 
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle im Repository vorkommenden key words                        //
///////////////////////////////////////////////////////////////////////////////
size_t TimeSeriesRepository::getKeyWords ( std::vector<std::string> &vKeyWords )
{
  std::set<std::string>::iterator iterSet;

  for ( iterSet = m_sKeyWords.begin(); iterSet != m_sKeyWords.end(); ++iterSet )
    vKeyWords.push_back( *iterSet );

  return vKeyWords.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die Ids aller gespeicherter RegularTimeSeriesSet Objekte         //
///////////////////////////////////////////////////////////////////////////////
size_t TimeSeriesRepository::getTimeSeriesSetIds ( std::vector<std::string> & vTimeSeriesSets )
{
  std::map<std::string,RegularTimeSeriesSetProperties*>::iterator iter;

  for ( iter = m_mTimeSeriesSetMetaDataMap.begin(); iter != m_mTimeSeriesSetMetaDataMap.end(); ++iter )
    vTimeSeriesSets.push_back ( iter->first );

  return vTimeSeriesSets.size();
}

////////////////////////////////////////////////////////////////////////////////
//  Liefert die Ids aller gespeicherter RegularTimeSeriesSetProperties Objekte//
////////////////////////////////////////////////////////////////////////////////
size_t TimeSeriesRepository::getPropertyIds ( std::vector<std::string> & vTimeSeriesSetProperties )
{
  std::map<std::string,RegularTimeSeriesSetProperties*>::iterator iter;

  for ( iter = m_mTimeSeriesSetPropertiesMap.begin(); iter != m_mTimeSeriesSetPropertiesMap.end(); ++iter )
    vTimeSeriesSetProperties.push_back ( iter->first );

  return vTimeSeriesSetProperties.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Ids aller RegularTimeSeriesSet-Objekte, die das Objekt       //
//  mit Id objectId  referieren                                              //
///////////////////////////////////////////////////////////////////////////////
size_t TimeSeriesRepository::getPropertyIdsWithObjectFilter ( std::string objectId,
                std::vector<std::string> & vPropertyIds )
{
   std::multimap<std::string,std::string>::iterator iter;

   for ( iter  = m_mRelObjToProperties.lower_bound( objectId ); 
         iter != m_mRelObjToProperties.upper_bound( objectId ); ++iter )
   {
     std::string timeSeriesSetId = iter->second;
     vPropertyIds.push_back ( timeSeriesSetId );
   }

   return vPropertyIds.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Ids aller RegularTimeSeriesSet-Objekte, die das              //
//  Schlüsselwort keyWord referieren                                         //
///////////////////////////////////////////////////////////////////////////////
size_t TimeSeriesRepository::getPropertyIdsWithKeyWordFilter ( std::string keyWord,
              std::vector<std::string> & vPropertyIds )
{
  std::multimap<std::string,string>::iterator iter;

  for ( iter  = m_mKeyWordsToProperties.lower_bound( keyWord ); 
        iter != m_mKeyWordsToProperties.upper_bound( keyWord ); ++iter )
  {
    std::string timeSeriesSetId = iter->second;
    vPropertyIds.push_back ( timeSeriesSetId );
  }

  return vPropertyIds.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht das gesamte Repository                                            //
///////////////////////////////////////////////////////////////////////////////
void TimeSeriesRepository::resetRepository()
{
  std::map<std::string,RegularTimeSeriesSetProperties*>::iterator iter;
  std::map<std::string,RegularTimeSeriesSet*>::iterator      iterSets;


  for ( iter = m_mTimeSeriesSetMetaDataMap.begin(); iter != m_mTimeSeriesSetMetaDataMap.end(); ++iter )
    delete iter->second;

  for ( iterSets = m_mTimeSeriesSetMap.begin(); iterSets != m_mTimeSeriesSetMap.end(); ++iterSets )
    delete iterSets->second;

  m_mTimeSeriesFileMap.clear();
  m_mTimeSeriesSetMetaDataMap.clear();
  m_mTimeSeriesSetPropertiesMap.clear();
  m_mTimeSeriesSetMetaDataMapInv.clear();
  m_mTimeSeriesSetMap.clear();
  m_mRelObjToProperties.clear();
  m_mKeyWordsToProperties.clear();
  m_sKeyWords.clear();
  m_sRelObjIds.clear();
  m_mRelObjIdsToNames.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein RegulatTimeSeriesSet Objekt zum Repository hinzu                //
///////////////////////////////////////////////////////////////////////////////
void TimeSeriesRepository::saveTimeSeriesSet ( RegularTimeSeriesSet * pTimeSeriesSet )
{
  string   pathName;
  String ^ timeSeriesFileName = generateNewFileName();
  String ^ timeSeriesPathName = Path::Combine( QuConvert::ToString( m_repositoryFolderPath ), timeSeriesFileName );
  QuConvert::systemStr2stdStr( pathName, timeSeriesPathName );

  pTimeSeriesSet->writeToXML( timeSeriesPathName, QuConvert::ToString ( m_energyADENamespace ), 
    QuConvert::ToString( m_timeSeriesADENamespace ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Speichert ein Objekt RegularTimeSeriesSet temporär, so dass mehrere      //
//  Objekte in derselben Datei gespeichert werden können                     //
///////////////////////////////////////////////////////////////////////////////
void TimeSeriesRepository::addTimeSeriesSetToSave ( RegularTimeSeriesSet * pTimeSeriesSet )
{
  m_pTimeSeriesSetsToSave->addTimeSeriesSet( pTimeSeriesSet );
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die temporär gespeicherten RegularTimeSeriesSet Objekte in das  //
//  Repository                                                               //
///////////////////////////////////////////////////////////////////////////////
void TimeSeriesRepository::save( bool deleteSavedTimeSeries )
{
  if ( m_pTimeSeriesSetsToSave->getTimeSeriesSetAnz() == 0 )
    return;

  string   pathName;
  String ^ timeSeriesFileName = generateNewFileName();
  String ^ timeSeriesPathName = Path::Combine( QuConvert::ToString( m_repositoryFolderPath ), timeSeriesFileName );
  QuConvert::systemStr2stdStr( pathName, timeSeriesPathName );

  m_pTimeSeriesSetsToSave->writeToXML( timeSeriesPathName, QuConvert::ToString ( m_energyADENamespace ), 
    QuConvert::ToString( m_timeSeriesADENamespace ) );

  if ( deleteSavedTimeSeries )
  {
    delete m_pTimeSeriesSetsToSave;
    m_pTimeSeriesSetsToSave = new RegularTimeSeriesSets;
  }
  else
    m_pTimeSeriesSetsToSave->clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert den Namen einer neuen Datei, in der RegularTimeSeriesSets      //
//  gespeichert werden                                                       //
//  Dazu werden das aktuelle Datum und die aktuelle Uhrzeit verwendet        //
///////////////////////////////////////////////////////////////////////////////
String ^ TimeSeriesRepository::generateNewFileName()
{
  DateTime now = DateTime::Now;

  int year   = now.Year;
  int month  = now.Month;
  int day    = now.Day;
  int hour   = now.Hour;
  int minute = now.Minute;
  int second = now.Second;

  String ^ STR = String::Concat ( Convert::ToString( year ), "_", 
                                  Convert::ToString( month ), "_",
                                  Convert::ToString( day ), "_",
                                  Convert::ToString( hour ), "_",
                                  Convert::ToString( minute ), "_",
                                  Convert::ToString( second ), ".xml" );
  return STR;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den zu einer RegularTimeSeriesSet-Id gehörigen                   //
//  RegularTimeSeriesSet durch Lesen vom Repository                          //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeriesSet * TimeSeriesRepository::getTimeSeriesSet (std::string timeSeriesSetId  )
{
  RegularTimeSeriesSet * pTimeSeriesSet;
  if ( m_mTimeSeriesSetMap.find( timeSeriesSetId ) != m_mTimeSeriesSetMap.end() )
    pTimeSeriesSet = m_mTimeSeriesSetMap[timeSeriesSetId];
  else
  {
    string fileName = m_mTimeSeriesFileMap[timeSeriesSetId];
    pTimeSeriesSet = RegularTimeSeriesSets::readFromXML( fileName, timeSeriesSetId, m_pUOMList );
    m_mTimeSeriesSetMap[timeSeriesSetId] = pTimeSeriesSet;
  }

  return pTimeSeriesSet;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den zu einer RegularTimeSeriesSetProperty-Id gehörigen           //
//  RegularTimeSeriesSet durch Lesen vom Repository                          //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeriesSet * TimeSeriesRepository::getTimeSeriesSetFromPropertyId ( std::string propertyId  )
{
  RegularTimeSeriesSetProperties * pProperties = m_mTimeSeriesSetPropertiesMap[propertyId];
  std::string timeSeriesSetId = m_mTimeSeriesSetMetaDataMapInv[pProperties];

  return getTimeSeriesSet ( timeSeriesSetId );
}


//////////////////////////////////////////////////////////////////////////////////
//  Liefert die zu einer RegularTimeSeriesSetProperties-Id gehörigen Metadaten  //
//////////////////////////////////////////////////////////////////////////////////
RegularTimeSeriesSetProperties * TimeSeriesRepository::getTimeSeriesSetProperties ( std::string propertiesId )
{
  if ( m_mTimeSeriesSetPropertiesMap.find( propertiesId ) != m_mTimeSeriesSetPropertiesMap.end() )
    return m_mTimeSeriesSetPropertiesMap.find( propertiesId )->second;
  else
    return NULL;
}

