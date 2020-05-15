#pragma once

#include "TimeX.h"
#include "TimeSeries.h"

///////////////////////////////////////////////////////////////////////////////
//  TimeSeriesRepository                                                     //
//  Klasse zur XML-basierten Speicherung von Zeitreihen                      //
///////////////////////////////////////////////////////////////////////////////
class TimeSeriesRepository
{
public:
  TimeSeriesRepository ( UOMList * pUOMList, std::string repositoryFolderPath );
  virtual ~ TimeSeriesRepository();

protected:
  UOMList    * m_pUOMList;                  // Modul Units of Measurement
  std::string  m_repositoryFolderPath;      // Pfad des Repository-Ordners
  std::string  m_energyADENamespace;        // Namespace CityGML Energy ADE
  std::string  m_timeSeriesADENamespace;    // Namespace TimeSeries ADE

  RegularTimeSeriesSets * m_pTimeSeriesSetsToSave;  // Menge von RegularTimeSeriesSet-Objekten, die in das Repository
                                                    // geschfieben werden sollen

  //  Dateiname --> IDs der gespeicherten RegularTimeSeriesSet Objekte
  std::map<std::string,std::string>                          m_mTimeSeriesFileMap;

  //  RegularTimeSeriesSet-IDs --> RegularTimeSeriesSetProperties
  std::map<std::string,RegularTimeSeriesSetProperties*>      m_mTimeSeriesSetMetaDataMap;

  //  RegularTimeSeriesSetProperties-IDs --> RegularTimeSeriesSetProperties
  std::map<std::string,RegularTimeSeriesSetProperties*>      m_mTimeSeriesSetPropertiesMap;

  //  RegularTimeSeriesSet-IDs --> RegularTimeSeriesSet
  std::map<std::string,RegularTimeSeriesSet*>      m_mTimeSeriesSetMap;

  //  RegularTimeSeriesSetProperties --> RegularTimeSeriesSet-IDs
  std::map<RegularTimeSeriesSetProperties*,std::string>      m_mTimeSeriesSetMetaDataMapInv;

  //  RelatedObject-Ids --> RegularTimeSeriesSetProperteis-Ids
  std::multimap<std::string,std::string> m_mRelObjToProperties;

  //  Key Words --> RegularTimeSeriesSetProperty-IDs
  std::multimap<std::string,std::string> m_mKeyWordsToProperties;

  // Set aller benutzten Key Words
  std::set<std::string>                                      m_sKeyWords;

  //  Set aller benutzten RelatedObject-Ids
  std::set<std::string>                                      m_sRelObjIds;

  //  RelatedObject-Ids --> RelatedObject-Names
  std::map<std::string,std::string>                          m_mRelObjIdsToNames;

public:
  void scanRepository();

	size_t getRelObjs ( std::vector<std::string> &vRelObjIds, std::vector<std::string> &vRelObjNames );
	size_t getKeyWords ( std::vector<std::string> &vKeyWords );

	size_t getTimeSeriesSetIds ( std::vector<std::string> & vTimeSeriesSetIds );

	size_t getPropertyIds ( std::vector<std::string> & vTimeSeriesSetProperties );
	size_t getPropertyIdsWithObjectFilter ( std::string objectId, std::vector<std::string> & vPropertyIds );
	size_t getPropertyIdsWithKeyWordFilter ( std::string keyWord, std::vector<std::string> & vPropertyIds );

  RegularTimeSeriesSetProperties * getTimeSeriesSetProperties ( std::string propertiesId );
  RegularTimeSeriesSet * getTimeSeriesSet ( std::string timeSeriesSetId  );
  RegularTimeSeriesSet * getTimeSeriesSetFromPropertyId ( std::string PropertyId  );


  void saveTimeSeriesSet ( RegularTimeSeriesSet * pTimeseriesSets );

  void addTimeSeriesSetToSave ( RegularTimeSeriesSet * pTimeseriesSets );
  void save( bool deleteSavedTimeSeries = false );

protected:
  void resetRepository();
  void scanFile ( String ^ fileName );
  String ^ generateNewFileName();
};