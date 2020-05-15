#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Xml;

#include "TimeX.h"

class GmlToolbox;
class UOM;
class Nutzungsprofil;
class UOMList;
class Feature;
class Features;
class RelatedObject;
class RegularTimeSeriesSetProperties;
class RelatedSet;
class RegularTimeSeriesProperties;

///////////////////////////////////////////////////////////////////////////////
//  AbstractTimeSeries                                                       //
//  Abstrakte Basisklasse für alle Zeitreihen-Klassen                        //
///////////////////////////////////////////////////////////////////////////////
class AbstractTimeSeries
{
public:
  enum InterpolationType   // Art der Ableitung eines Zeitreihenwertes
  {
    AverageInPrecedingInterval,  // Mittelwert des vorhergehenden Zeitintervalls
    AverageInSuccedingInterval,  // Mittelwert des folgenden Zeitintervalls
    ConstantInPrecedingInterval, // Konstanter Wert im vorhergehenden Zeitintervall
    ConstantInSuccedingInterval, // Konstanter Wert im folgenden Zeitintervall
    Continuous,                  // Wert zum aktuellen Zeitpunkt. Eine Interpolation zwischen Zeitpunkten ist möglich
    Discontinuous,               // Wert zum aktuellen Zeitpunkt. Eine Interpolation zwischen Zeitpunkten ist nicht möglich
    InstantaneousTotal,          // Gesamtwert zum aktuellen Zeitpunkt
    MaximumInPrecedingInterval,  // Maximalwert im vorhergehenden Zeitintervall
    MaximumInSuccedingInterval,  // Minimalwert im vorhergehenden Zeitintervall
    MinimumInPrecedingInterval,  // Maximalwert im folgenden Zeitintervall
    MinimumInSuccedingInterval,  // Minimalwert im folgenden Zeitintervall
    PrecedingTotal,              // Akkumulierter Wert im vorhergehenden Zeitintervall
    SuccedingTotal               // Akkumulierter Wert im folgenden Zeitintervall
  };

  enum TimeSeriesType     // Typ einer Zeitreihe
  {
    Regular,          // Zeitreihe mit äquidistanten Zeitintervallen, die Werte werden im Objekt gespeichert
    Irregular,        // Zeitreihe mit unterschiedlichen Zeitintervallen, die Werte werden im Objekt gespeichert
    RegularFile,      // Zeitreihe mit äquidistanten Zeitintervallen, die Werte werden in einer externen CSV-Datei gespeichert
    IrregularFile,    // Zeitreihe mit unterschiedlichen Zeitintervallen, die Werte werden in einer externen CSV-Datei gespeichert
    Daily,            // Auf täglichen Profilen besierende Zeitreihe
    Monthly           // Zeitreihe mit 12 Monatswerten
  };

public:
  AbstractTimeSeries( std::string gmlIdP );
  AbstractTimeSeries( AbstractTimeSeries * pTimeSeriesOld, std::string gmlIdP );

  virtual ~AbstractTimeSeries();

protected:
  std::string       gmlId;              //Id der Zeitreihe
  std::string       aquisitionMethod;    // Beschreibung der Methode, die die Zeitreihe generiert wurde
  std::string       qualityDescription;  // Beschreibung der Qualität der Zeitreihe
  std::string       source;              // Quelle der Zeitreihen-Werte
  std::string       thematicDescription; // Thematische Beschreibung der Zeitreihe
	std::string       property;            // Identifier of the time series property
	std::string       propertyQualifier;   // Additional qualifier of the time series property
  InterpolationType interpolationType;   // Art der Ableitung eines Zeitreihenwertes
  UOM             * unitOfMeasure;       // Physikalische Maßeinheit der Werte
  int               refCount;            // Referenzzähler des Zeitreihen-Objektes

public:
  virtual TimeSeriesType getTimeSeriesType() = NULL;

  std::string getGmlId()  {  return gmlId;  }

  void setAquisitionMethod ( std::string text )   { aquisitionMethod = text;  }
  std::string getAquisitionMethod ()  {  return aquisitionMethod;  }

  void setQualityDescription ( std::string text )   { qualityDescription = text;  }
  std::string getQualityDescription ()  {  return qualityDescription;  }

  void setThematicDescription ( std::string text )   { thematicDescription = text;  }
  std::string getThematicDescription ()  {  return thematicDescription;  }

	void setProperty ( std::string text )   { property = text;  }
	std::string getProperty ()  {  return property;  }

	void setPropertyQualifier ( std::string text )   { propertyQualifier = text;  }
	std::string getPropertyQualifier ()  {  return propertyQualifier;  }

  void setSource ( std::string text )   { source = text;  }
  std::string getSource ()  {  return source;  }

  void setInterpolationType ( InterpolationType t )  {  interpolationType = t;  }
  void setInterpolationTypeFromString ( std::string interpolationType );

  InterpolationType getInterpolationType()  {   return interpolationType;  }
  std::string getInterpolationTypeAsString();

  void setUOM ( UOM * uom )  {  unitOfMeasure = uom;  }
  UOM * getUOM()   {   return unitOfMeasure;  }

  virtual std::string getTimeSeriesMetadata();

  void readPropertiesFromXML ( XmlTextReader ^ pReader );

  void incRefCount()  {  refCount++;  }
  void decRefCount()  {  refCount--;  }
  int getRefCount()   {  return refCount;  }
};

///////////////////////////////////////////////////////////////////////////////
//  RegularTimeSeries                                                        //
//  Reguläre Zeitreihe, d.h. Messwerte mit konstantem Zeitinkrement,         //
//  wobei die Werte in Objekt gespeichert werden                             //
///////////////////////////////////////////////////////////////////////////////
class RegularTimeSeries : public AbstractTimeSeries
{
public:
  RegularTimeSeries( std::string gmlId );
  RegularTimeSeries( RegularTimeSeries * pTimeSeriesOld, std::string gmlId );
  virtual ~RegularTimeSeries();

protected:
  TimePeriod         * temporalExtent;    // Gesamter von den Zeitreihenwerten überspannter Zeitraum
  TimeIntervalLength * timeInterval;      // Zeitinkrement
  std::vector<double>  values;            // Zeitreihen-Werte
  double               missingValue;      // Double-Tahl, die einen fehlenden Wert codiert

public:
  virtual TimeSeriesType getTimeSeriesType()  { return Regular;  }

  virtual void setTemporalExtent ( TimePeriod * period );
  TimePeriod * getTemporalExtent ()  {  return temporalExtent; }

  virtual void setTimeIntervalLength ( TimeIntervalLength * interval );
  TimeIntervalLength * getTimeIntervalLength ()  {  return timeInterval;  }

  void setValues ( std::vector<double> &v );
  std::vector<double> & getValues()  {  return values;  }
  void getValues ( System::Collections::Generic::List<double> ^ vValues );
  std::string getValuesAsString();
  void addValue ( double value )  {  values.push_back( value );  }

	size_t getValueCount()  {  return values.size();  }
  double getValue ( size_t index );

  DateTime ^ getDateTime (  unsigned int i );

  bool setValue ( int index, double value );

  void setMissingValue ( double v )  {   missingValue = v;  }
  double getMissingValue ()  {  return missingValue;  }

  virtual std::string getTimeSeriesMetadata();

  double getMaximalValue();
  double getSum();
  double getAverage();
  void   scale ( double factor );

  int getMissingValuesCount();
  int getGapCount();

  void interpolateGaps( int maxGapSize );

  void writeToXML ( XmlTextWriter ^ pWriter );
  void readFromXML ( XmlTextReader ^ pReader, UOMList * pUOMList );

  static RegularTimeSeries * aggregateTimeSeries ( std::vector<RegularTimeSeries*> &vTimeSeries, 
    int aggregationTyp );
};

///////////////////////////////////////////////////////////////////////////////
//  RegularTimeSeriesSet                                                     //
//  Menge von thematisch zusammenhängenden Zeitreihen, z.B. Satz von         //
//  Simulationsergebnissen.                                                  //
//  Aktuell beschränkt auf Zeitreihen, die ein gesamtes Jahr mit 1h Inkrement//
//  überspannen                                                              //
///////////////////////////////////////////////////////////////////////////////
class RegularTimeSeriesSet
{
public:
  RegularTimeSeriesSet ( std::string gmlId, RegularTimeSeriesSetProperties * pProperties );
  RegularTimeSeriesSet ( std::string gmlId );

  virtual ~RegularTimeSeriesSet();

protected:
  std::string                      m_gmlId;       // Id der Zeitreihen-Menge
  RegularTimeSeriesSetProperties * m_pMetaData;   // Metadaten-Beschreibung der Zeitreihen-Menge
  std::vector<RegularTimeSeries*>  m_vTimeSeries; // Liste der zugehörigen Zeitreihen

public:
  RegularTimeSeriesSetProperties * getMetaData ()  {  return m_pMetaData;  }
  void setMetaData ( RegularTimeSeriesSetProperties * pMetadata );

  std::string getGmlId()  {   return m_gmlId; }

  bool addTimeSeries( RegularTimeSeries * pTimeSeries, bool adjustYear );
  
	size_t getTimeSeriesAnz()  {  return m_vTimeSeries.size();  }
  RegularTimeSeries * getTimeSeries ( unsigned int index );


  void writeToXML ( String ^ fileNameString, String ^ energyADENamespace, String ^ timeSeriesADENamespace  );
  void writeToXML ( XmlTextWriter ^ pWriter );
  void readFromXML ( XmlTextReader ^ pReader, UOMList * pUOMList );

  void writeToCSV ( std::string fileName );
};

///////////////////////////////////////////////////////////////////////////////
//  RelatedSet                                                               //
//  Modellierung einer Referenz auf einen  RegularTimeSeriesSet              //
///////////////////////////////////////////////////////////////////////////////
class RelatedSet
{
public:
  RelatedSet ();
  RelatedSet ( std::string relatedSetId, std::string relatedSetRole, std::string description );
  virtual ~RelatedSet();

protected:
  std::string m_relatedSetId;   // Id des referierten RegularTimeSeriesSet
  std::string m_relatedSetRole; // Name der zugehörigen Relation
  std::string m_description;    // Informelle Beschreibung der Relation

public:
  void setRelatedSetId ( std::string str ) { m_relatedSetId = str;  }
  std::string getRelatedSetId()    {  return m_relatedSetId;  }

  void setRelatedSetRole ( std::string str )  {  m_relatedSetRole = str;  }
  std::string getRelatedSetRole()  {  return m_relatedSetRole;  }

  void setDescription ( std::string str )  {  m_description = str;  }
  std::string getDescription()     {  return m_description;  }

  void writeToXML ( XmlTextWriter ^ pWriter );
  void readFromXML ( XmlTextReader ^ pReader );
};

///////////////////////////////////////////////////////////////////////////////
//  RegularTimeSeriesSetProperties                                           //
//  Metadaten eines RegularTimeSeriesSet                                     //
///////////////////////////////////////////////////////////////////////////////
class RegularTimeSeriesSetProperties
{
public:
  RegularTimeSeriesSetProperties ( std::string gmlId );
  virtual ~RegularTimeSeriesSetProperties();

protected:
  std::string                               m_gmlId;             // Id des Metadatensatzes
  std::string                               m_timeSeriesSetType; // Typ der Zeitreihen 
  TimePeriod                              * m_temporalExtent;    // Von den Zeitreihen überspannter Zeitraum
  TimeIntervalLength                      * m_timeInterval;      // Zeitinkrement der Zeitreihen
  int                                       m_year;              // Kalenderjahr der Zeitreihen-Werte
  TimePosition                              m_generationDate;    // Zeitpunkt, an dem der RegularTimeSeriesSet erzeugt wurde
  std::string                               m_application;       // Applikation, mit der der RegularTimeSeriesSet erzeugt wurde
  std::string                               m_description;       // Informelle Beschreibung des RegularTimeSeriesSet
  std::vector<std::string>                  m_vKeyWords;         // Schlüsselwörter zur Charakterisierung des RegularTimeSeriesSet
  std::vector<RelatedObject*>               m_vRelatedObjects;   // Mit dem RegularTimeSeriesSet verbundene Objekte, z.B. GML-Features
  std::vector<RelatedSet*>                  m_vRelatedTimeSeriesSets; // Mit dem RegularTimeSeriesSet verbundene weitere RegularTimeSeriesSet
  std::vector<RegularTimeSeriesProperties*> m_vTimeSeriesMetaData;    // Liste der Metadaten der einzelnen Zeitreihen

public:
  std::string getGmlId()  {  return m_gmlId;  }

  void setTimeSeriesSetType (  std::string type )  {   m_timeSeriesSetType = type;  }
  std::string getTimeSeriesSetType()  {  return m_timeSeriesSetType;  }

  void setYear ( int year );
  int getYear ()  {  return m_year;  }

  void setStartYear ( int year );
  int getStartYear ();

  void setEndYear ( int year );
  int getEndYear ();

  TimePosition getGenerationDate()  {  return m_generationDate;  }

  void setApplication ( std::string application )  {  m_application = application;  }
  std::string getApplication()  {  return m_application;  }

  void setDescription ( std::string description )  {  m_description = description;  }
  std::string getDescription()  {  return m_description;  }

  void addKeyWord ( std::string keyWord )  {  m_vKeyWords.push_back( keyWord );  }
  std::vector<std::string> & getKeyWords ()  {  return m_vKeyWords;  }
  std::string getKeyWordsCombined();
  bool hasKeyWord ( std::string keyWord );
   
  void addRelatedObject ( RelatedObject * pRelObject )  {  m_vRelatedObjects.push_back( pRelObject );  }
	size_t getRelatedObjectAnz()  {  return m_vRelatedObjects.size();  }
  RelatedObject * getRelatedObject ( unsigned int index );
  bool isRelatedWithObject ( std::string id );

  void addTimeSeriesProperties ( RegularTimeSeriesProperties * pProperties )  {  m_vTimeSeriesMetaData.push_back( pProperties );  }
	size_t getTimeSeriesPropertiesAnz()  {  return m_vTimeSeriesMetaData.size();  }
  RegularTimeSeriesProperties * getTimeSeriesProperties ( unsigned int index );

  void addRelatedSet ( RelatedSet * pRelSet )  {  m_vRelatedTimeSeriesSets.push_back( pRelSet );  }

	size_t getRelatedSetAnz()  {  return m_vRelatedTimeSeriesSets.size();  }
  RelatedSet * getRelatedSet ( unsigned int index );

  void writeToXml ( XmlTextWriter ^ pWriter );
  void readFromXML ( XmlTextReader ^ pReader );
};

///////////////////////////////////////////////////////////////////////////////
//  RegularTimeSeriesProperties                                              //
//  Metadaten einer einzelnen RegularTimeSeries                              //
///////////////////////////////////////////////////////////////////////////////
class RegularTimeSeriesProperties
{
public:
  RegularTimeSeriesProperties ();
  virtual ~RegularTimeSeriesProperties();

protected:
  std::string m_thematicDescription;  // Thematische Beschreibung
  std::string m_uomSymbol;            // Text-String zur Darstellung der physikalischen Maßeinheit

public:
  void setThematicDescription ( std::string thematicDescription )  { m_thematicDescription = thematicDescription;  }
  std::string getThematicDescription()  {  return m_thematicDescription;  }

  void setUomSymbol ( std::string uomSymbol )  {  m_uomSymbol = uomSymbol;  }
  std::string getUOMSymbol()  {  return m_uomSymbol;  }
};

///////////////////////////////////////////////////////////////////////////////
//  RelatedObject                                                            //
//  Information über ein anderes Objekt, das mit einem RegularTimeSeriesSet  //
//  logisch verknüpft ist                                                    //
///////////////////////////////////////////////////////////////////////////////
class RelatedObject
{
public:
  RelatedObject();
  virtual ~RelatedObject();

protected:
  std::string m_relObjId;   // Id des verknüpften Objektes
  std::string m_relObjName; // Property-Name der Verknüpfung
  std::string m_relObjURI;  // URL der Verknüpfung

public:
  void setId ( std::string id )  {  m_relObjId = id;  }
  std::string getId()  {   return m_relObjId;  }

  void setName ( std::string name )  { m_relObjName = name;  }
  std::string getName()  {  return m_relObjName;  }

  void setURI ( std::string uri )  {  m_relObjURI = uri;  }
  std::string getURI ()  {return m_relObjURI;  }
};



///////////////////////////////////////////////////////////////////////////////
//  RegularTimerSeriesSets                                                   //
//  Liste von RegularTimeSeriesSet Objekten                                  //
///////////////////////////////////////////////////////////////////////////////
class RegularTimeSeriesSets
{
public:
  RegularTimeSeriesSets();
  virtual ~RegularTimeSeriesSets();

protected:
  std::vector<RegularTimeSeriesSet*> m_vTimeSeriesSets;  // Liste der RegularTimeSeriesSet Objekte

public:
  void addTimeSeriesSet ( RegularTimeSeriesSet * pTimeSeriesSet );
  
	size_t getTimeSeriesSetAnz()  {  return m_vTimeSeriesSets.size();  }
  RegularTimeSeriesSet * getTimeSeriesSet ( unsigned int index );

  void writeToXML ( String ^ fileNameString, String ^ energyADENamespace, String ^ timeSeriesADENamespace  );

  static RegularTimeSeriesSet * readFromXML ( std::string fileName, std::string timeSeriesSetId, UOMList * pUOMList );

  void clear();
};
 
///////////////////////////////////////////////////////////////////////////////
//  DailyTimeSeries                                                          //
//  Tägliche Zeitreihe mit 1h Inkrement                                      //
///////////////////////////////////////////////////////////////////////////////
class DailyTimeSeries: public RegularTimeSeries
{
  
public:
  enum DAY_TYP { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY, DESIGNDAY, WEEKDAY, WEEKEND, TYPICALDAY }; // Typ es modellierten Tages

public:
  DailyTimeSeries ( std::string gmlId );
  DailyTimeSeries ( DailyTimeSeries * pTimeSeriesOld, std::string gmlId );

public:
  double   m_minValue;      // Minimalwert der zulässigen Tageswerte
  double   m_maxValue;      // Maximalwert der zulässigen Tageswerte
  double   m_defaultValue;  // Standardwert
  DAY_TYP  m_dayType;       // Typ des modellierten Tages

public:
  virtual TimeSeriesType getTimeSeriesType()  { return Regular;  }

  virtual void setTemporalExtent ( TimePeriod * period )  {    }
  virtual void setTimeIntervalLength ( TimeIntervalLength * interval )   {     }

  static std::string getDayTypAsString ( DAY_TYP dayTyp );
  static DAY_TYP getDayTypFromString ( std::string dayTyp );

  std::string getDayTypAsString();

  virtual std::string getTimeSeriesMetadata();
};

///////////////////////////////////////////////////////////////////////////////
//  RegularTimeSeriesFile                                                    //
//  Reguläre Zeitreihe, deren Werte auf einer CSV-Datei gespeichert sind     //
//  Die Datei kann mehrere Zeitreihen enthalten, von denen eine ausgewählt   //
//  wird                                                                     //
///////////////////////////////////////////////////////////////////////////////
class RegularTimeSeriesFile: public RegularTimeSeries
{
public:
  RegularTimeSeriesFile ( std::string gmlId );
  RegularTimeSeriesFile (  RegularTimeSeriesFile * pTimeSeriesOld, std::string gmlId  );
  virtual ~ RegularTimeSeriesFile();

private:
  std::string file;                 // Pfadname der Datei
  int         numberOfHeaderLines;  // Anzahl der Header-Zeilen der Datei
  std::string fieldSeparator;       // Separator für die Werte, die einem Zeitpunkt zugeordnet sind (z.B. ";")
  std::string recordSeparator;      // Separator für die Records unterschiedlicher Zeitpunkte (z.B. "\n")
  std::string missingCSVValue;      // String, der einen fehlenden Zeitreihenwert anzeige (z.B. "NaN")
  std::string decimalSymbol;        // Dezimaltrennzeichen (z.B. ",")
  int         valueColumnNumber;    // Spalten-Index der aktuell ausgewählten Zeitreihe

public:
  virtual TimeSeriesType getTimeSeriesType()  { return RegularFile;  }

  void setFile ( std::string f )  {  file = f;  }
  std::string getFile()  {  return file;  }

  void setNumberOfHeaderLines ( int n )  {  numberOfHeaderLines = n;  }
  int getNumberOfHeaderLines()  {  return numberOfHeaderLines;  }

  void setFieldSeparator ( std::string s )  {  fieldSeparator = s;  }
  std::string getFieldSeparator ()  {  return fieldSeparator;  }

  void setRecordSeparator ( std::string r )  {  recordSeparator = r;  }
  std::string getRecordSeparator()  {  return recordSeparator;  }

  void setDecimalSymbol ( std::string c )  {  decimalSymbol = c;  }
  std::string getDecimalSymbol()  {  return decimalSymbol;  }

  void setValueColumnNumber ( int n )  {  valueColumnNumber = n;  }
  int getValueColumnNumber()  {  return valueColumnNumber;  }

  void setMissingValue ( std::string v )  {  missingCSVValue = v;  }
  std::string getMissingValue()   {  return missingCSVValue;  }

  virtual std::string getTimeSeriesMetadata();
};

///////////////////////////////////////////////////////////////////////////////
//  IrregularTimeSeries                                                      //
//  Zeitreihe in Form einer Liste von Zeitpunkt - Wert Paaren                //
///////////////////////////////////////////////////////////////////////////////
class IrregularTimeSeries : public AbstractTimeSeries
{
public:
  IrregularTimeSeries( std::string gmlId );
  IrregularTimeSeries ( IrregularTimeSeries * pTimeSeriesOld, std::string gmlId );
  virtual ~IrregularTimeSeries();

protected:
  std::vector<TimePosition> times;    // Liste der Zeitpunkte
  std::vector<double>       values;   // Liste der zugehörigen Werte

public:
  virtual TimeSeriesType getTimeSeriesType()  { return Irregular;  }

  bool set ( std::vector<TimePosition> & timesP, std::vector<double> & valuesP );

  void addData (TimePosition & pos, double value );

  std::vector<TimePosition> & getTimes()   {  return times;  }
  std::vector<double>       & getValues()  {  return values;  }

	size_t getDataCount ()  {  return values.size();  }
  bool getData ( unsigned int index,  TimePosition & pos, double &value );

  virtual std::string getTimeSeriesMetadata();
};

///////////////////////////////////////////////////////////////////////////////
//  MonthlyTimeSeries                                                        //
//  Liste mit 12 monatlichen Werten                                          //
///////////////////////////////////////////////////////////////////////////////
class MonthlyTimeSeries: public IrregularTimeSeries
{
public:
  MonthlyTimeSeries( std::string gmlId );
  MonthlyTimeSeries( std::string gmlId, int year );
  MonthlyTimeSeries ( MonthlyTimeSeries * pTimeSeriesOld, std::string gmlId );
  virtual ~MonthlyTimeSeries();

private:
  int m_year;   // Zugehöriges Jahr

public:
  virtual TimeSeriesType getTimeSeriesType()  { return Monthly;  }

  void setValues ( std::vector<double> &vValues );

  void setYear ( int year )  {  m_year = year;  }
  int getYear()  {  return m_year;  }
};

///////////////////////////////////////////////////////////////////////////////
//  IrregularTimeSeriesFile                                                  //
//  Zeitreihe mit wechselnden Zeitintervallen, deren Werte auf einer         //
//  Datei gespeichert sind                                                   //
///////////////////////////////////////////////////////////////////////////////
class IrregularTimeSeriesFile: public IrregularTimeSeries
{
public: 
  IrregularTimeSeriesFile (  std::string gmlId );
  IrregularTimeSeriesFile ( IrregularTimeSeriesFile * pTimeSeriesOld, std::string gmlId );
  virtual ~ IrregularTimeSeriesFile();

private:
  std::string file;                 // Pfadname der Datei
  int         numberOfHeaderLines;  // Anzahl der Header-Zeilen der Datei
  std::string fieldSeparator;       // Separator für die Werte, die einem Zeitpunkt zugeordnet sind (z.B. ";")
  std::string recordSeparator;      // Separator für die Records unterschiedlicher Zeitpunkte (z.B. "\n")
  char        decimalSymbol;        // Dezimaltrennzeichen (z.B. ",")
  int         timeColumnNumber;     // Spalten-Index der aktuell ausgewählten Zeitreihen-Zeitpunkte
  int         valueColumnNumber;    // Spalten-Index der aktuell ausgewählten Zeitreihenwerte

public:
  virtual TimeSeriesType getTimeSeriesType()  { return IrregularFile;  }

  void setFile ( std::string f )  {  file = f;  }
  std::string getFile()  {  return file;  }

  void setNumberOfHeaderLines ( int n )  {  numberOfHeaderLines = n;  }
  int getNumberOfHeaderLines()  {  return numberOfHeaderLines;  }

  void setFieldSeparator ( std::string s )  {  fieldSeparator = s;  }
  std::string getFieldSeparator ()  {  return fieldSeparator;  }

  void setRecordSeparator ( std::string r )  {  recordSeparator = r;  }
  std::string getRecordSeparator()  {  return recordSeparator;  }

  void setDecimalSymbol ( char c )  {  decimalSymbol = c;  }
  char getDecimalSymbol()  {  return decimalSymbol;  }

  void setTimeColumnNumber ( int n )  {  timeColumnNumber = n;  }
  int getTimeColumnNumber()  {  return timeColumnNumber;  }

  void setValueColumnNumber ( int n )  {  valueColumnNumber = n;  }
  int getValueColumnNumber()  {  return valueColumnNumber;  }

  virtual std::string getTimeSeriesMetadata();
};








