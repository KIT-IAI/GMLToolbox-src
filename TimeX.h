#pragma once

class TimePosition;
class TimeIntervalLength;

///////////////////////////////////////////////////////////////////////////////
//  AbstractTime                                                             //
//  Abstrakte Oberklasse für alle Zeit-/Datumsklassen                        //
///////////////////////////////////////////////////////////////////////////////
class AbstractTime
{
public:
  enum TimeValueType   // Typ der Zeitangabe
  {
    TIME_POSITION,        // Zeitpunkt
    TIME_PERIOD,          // Zeitspanne (Anfangszeit, Endzeit)
    TIME_INTERVAL_LENGTH  // Länge einer Zeitspanne
  };

public:
  AbstractTime()
  {

  }

  virtual ~AbstractTime()
  {

  }

public:
  virtual TimeValueType getTimeType() = NULL;
  virtual std::string toString() = NULL;
  static AbstractTime * copy ( AbstractTime * pTimeOld );
};

///////////////////////////////////////////////////////////////////////////////
//  TimePosition                                                             //
//  Klasse für einen Zeitpunkt                                               //
///////////////////////////////////////////////////////////////////////////////
class TimePosition: public AbstractTime
{
public:
  enum TimePositionQualifier //  Zusätzliche Qualifizierung einer Zeitangabe
  {
    AFTER,          // Nach der spez. Zeit
    BEFORE,         // Vor der spez. Zeit
    UNDEFINED       //  Undefiniert
  };

public:
  TimePosition();
  TimePosition ( const TimePosition & posOld );
  TimePosition (  TimePosition * posOld );
  TimePosition ( std::string timeString );
  TimePosition ( int year, int month, int day );
  TimePosition ( int h, int m, int s, int oh, int om  );
  TimePosition ( int year, int month, int day, int h, int m, int s, int oh = 0, int om = 0 );
  virtual ~TimePosition();

private:
  int                   year;                   // Jahr
  int                   month;                  // Monat (1-12)
  int                   day;                    // Tag (1-31)
  int                   hour;                   // Stunde (0-23)
  int                   minute;                 // Minute (0-59)
  int                   second;                 // Sekunde (0-59)
  int                   timeZoneOffsetHours;    // Stunden-Offset der aktuellen Zeitzone gegen GMT
  int                   timeZoneOffsetMinutes;  // Minuten-Offset der aktuellen Zeitzone gegen GMT
  TimePositionQualifier qualifier;              // Qualifizierung der Zeitangabe
  std::string           era;                    // Textliche Spezifikation einer Zeitangabe

public:
  virtual TimeValueType getTimeType() {  return TIME_POSITION;  }

  virtual std::string toString();

  std::string toXMLString();
  virtual bool fromXMLString ( std::string str );

  TimePosition operator= ( const TimePosition &pos );

  bool operator== ( const TimePosition &pos );
  bool operator > ( const TimePosition &pos );
  bool operator >= ( const TimePosition &pos );

  void setYear ( int y )  {  year = y;  }
  int getYear ()  {  return year;  }

  void setMonth ( int m )  {  month = m;  }
  int getMonth ()  {  return month;  }

  void setDay ( int d )  {  day = d;  }
  int getDay ()  {  return day;  }

  void setDate ( int year, int month, int day );

  void setHour ( int h )  {  hour = h;  }
  int getHour ()  {  return hour;  }

  void setMinute ( int m )  {  minute = m;  }
  int getMinute ()  {  return minute;  }

  void setSecond ( int s )  {  second = s;  }
  int getSecond ()  {  return second;  }

  void setTimeZoneOffsetHours ( int o )  { timeZoneOffsetHours = o;  }
  int getTimeZoneOffsetHours ()  {  return timeZoneOffsetHours;  }

  void setTimeZoneOffsetMinutes ( int o )  {  timeZoneOffsetMinutes = o;  }
  int getTimeZoneOffsetMinutes ()  {  return timeZoneOffsetMinutes;  }

  void setTime ( int h, int m, int s, int oh = 0, int om = 0 );

  void setQualifier ( TimePositionQualifier q )  {  qualifier = q;  }
  TimePositionQualifier getQualifier ()  {  return qualifier;  }

  void setQualifier ( std::string qualifier );
  std::string getQualifierAsString();

  void setEra ( std::string eraP )  {  era = eraP;  }
  std::string getEra()  {   return era;  }

  static std::string convertToString ( System::DateTime ^ pDateTime );

  void setDefaultTime();

  bool hasDate();
  bool hasTime();

  TimePosition incrementOneDay(); 
  TimePosition incrementOneHour(); 

  void writeToXML ( System::Xml::XmlTextWriter ^ pWriter );
  void readFromXML ( System::Xml::XmlTextReader ^ pReader );

private:
  bool interpreteDateString ( std::string dateString );
  bool interpreteTimeString ( std::string timeString );

};

///////////////////////////////////////////////////////////////////////////////
//  TimePeriod                                                               //
//  Klasse für ein Zeitintervall                                             //
///////////////////////////////////////////////////////////////////////////////
class TimePeriod :public  AbstractTime
{

public:
  TimePeriod();
  TimePeriod ( const TimePeriod & timePeriodOld );
  TimePeriod (  TimePeriod * pTimePeriodOld );
  TimePeriod (  TimePosition & startPos, TimePosition & endPos );
  virtual ~TimePeriod();

private:
  TimePosition start;   // Anfangs-Zeitpunkt
  TimePosition end;     // End-Zeitpunkt

public:
  virtual TimeValueType getTimeType() {  return TIME_PERIOD;  }

  virtual std::string toString();

  TimePeriod  operator= ( TimePeriod &period );
  bool operator== ( const TimePeriod &period );

  void setStartPosition ( TimePosition & startPos )  {  start = startPos;  }
  TimePosition & getStartPosition ()  {  return start;  }

  void setEndPosition ( TimePosition & endPos )  {  end = endPos;  }
  TimePosition & getEndPosition()  {  return end;  }

  bool startsAtFirstDayOfYear();
  bool endsAtLastDayOfYear();
  bool startsNextDay ( TimePeriod * nextPeriod );
  bool startsAfter ( TimePeriod * nextPeriod );

  void writeToXML ( System::Xml::XmlTextWriter ^ pWriter );
  void readFromXML ( System::Xml::XmlTextReader ^ pReader );
};

///////////////////////////////////////////////////////////////////////////////
//  TimeIntervalLength                                                       //
//  Klasse für einen Zeitraum                                                //
///////////////////////////////////////////////////////////////////////////////
class TimeIntervalLength: public AbstractTime
{
public:
  enum TimeUnit  // Einheit des Zeitraums
  {
    DAY,          // Tage
    HOUR,         // Stunden
    MINUTE,       // Minuten
    SECOND        // Sekunden
  };

public:
  TimeIntervalLength();
  TimeIntervalLength( const TimeIntervalLength & timeIntervalLengthOld );
  TimeIntervalLength( TimeIntervalLength * pTimeIntervalLengthOld );
  virtual ~TimeIntervalLength();

private:
  TimeUnit unit;    // Einheit
  int     value;    // Wert

public:
  virtual TimeValueType getTimeType() {  return TIME_INTERVAL_LENGTH;  }

  bool operator== ( const TimeIntervalLength intervalLength );

  virtual std::string toString();
  virtual bool set ( float valueP, std::string unit );

  void setUnit ( TimeUnit u )  {  unit = u;  }
  TimeUnit getUnit ()  {  return unit;  }

  void setValue ( int v )  {  value = v;  }
  int getValue ()   {  return value;  }

  std::string getTimeUnitString();

  void writeToXML ( System::Xml::XmlTextWriter ^ pWriter );
  void readFromXML ( System::Xml::XmlTextReader ^ pReader );
};
