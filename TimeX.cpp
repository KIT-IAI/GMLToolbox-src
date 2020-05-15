#include "stdafx.h"

#include "Convert.h"
#include "TimeX.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  AbstractTime                                                             //
//  Abstrakte Oberklasse für alle Zeit-/Datumsklassen                        //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Kopiert ein Zeit-Objekt                                                  //
///////////////////////////////////////////////////////////////////////////////
AbstractTime * AbstractTime::copy ( AbstractTime * pTimeOld )
{
  switch ( pTimeOld->getTimeType() )
  {
  case TIME_POSITION:
    {
      TimePosition * pTime = new TimePosition ( (TimePosition*)pTimeOld );
      return pTime;
    }

  case TIME_PERIOD:
    {
      TimePeriod * pTimePeriod = new TimePeriod ( (TimePeriod*)pTimeOld );
      return pTimePeriod;
    }

  case TIME_INTERVAL_LENGTH:
    {
      TimeIntervalLength * pLength = new TimeIntervalLength ( (TimeIntervalLength*)pTimeOld );
      return pLength;
    }
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  TimePosition                                                             //
//  Klasse für einen Zeitpunkt                                               //
///////////////////////////////////////////////////////////////////////////////
TimePosition::TimePosition() : AbstractTime()
{
  year                  = 0;
  month                 = 0;
  day                   = 0;
  hour                  = -1;
  minute                = -1;
  second                = -1;
  timeZoneOffsetHours   = 0;
  timeZoneOffsetMinutes = 0;
  qualifier             = UNDEFINED;
  era                   = "";
}

TimePosition::TimePosition ( const TimePosition & posOld ) : AbstractTime ()
{
  year                  = posOld.year;
  month                 = posOld.month;
  day                   = posOld.day;
  hour                  = posOld.hour;
  minute                = posOld.minute;
  second                = posOld.second;
  timeZoneOffsetHours   = posOld.timeZoneOffsetHours;
  timeZoneOffsetMinutes = posOld.timeZoneOffsetMinutes;
  qualifier              = posOld.qualifier;
  era                   = posOld.era;
}


TimePosition::TimePosition ( TimePosition * posOld ) : AbstractTime ()
{
  year                  = posOld->year;
  month                 = posOld->month;
  day                   = posOld->day;
  hour                  = posOld->hour;
  minute                = posOld->minute;
  second                = posOld->second;
  timeZoneOffsetHours   = posOld->timeZoneOffsetHours;
  timeZoneOffsetMinutes = posOld->timeZoneOffsetMinutes;
  qualifier             = posOld->qualifier;
  era                   = posOld->era;
}

TimePosition::TimePosition ( string timeStringP ) : AbstractTime ()
{
  year                  = 0;
  month                 = 0;
  day                   = 0;
  hour                  = -1;
  minute                = -1;
  second                = -1;
  timeZoneOffsetHours   = 0;
  timeZoneOffsetMinutes = 0;
  qualifier             = UNDEFINED;
  era                   = "";

  fromXMLString( timeStringP );
}

TimePosition::TimePosition ( int yearP, int monthP, int dayP ): AbstractTime()
{
  year                  = yearP;
  month                 = monthP;
  day                   = dayP;
  hour                  = -1;
  minute                = -1;
  second                = -1;
  timeZoneOffsetHours   = 0;
  timeZoneOffsetMinutes = 0;
  qualifier             = UNDEFINED;
  era                   = "";
}

TimePosition::TimePosition ( int h, int m, int s, int oh, int om ): AbstractTime()
{
  year                  = 0;
  month                 = 0;
  day                   = 0;
  hour                  = h;
  minute                = m;
  second                = s;
  timeZoneOffsetHours   = oh;
  timeZoneOffsetMinutes = om;
  qualifier             = UNDEFINED;
  era                   = "";
}

TimePosition::TimePosition ( int y, int m, int d, int h, int min, int s, int oh, int om ): AbstractTime()
{
  year                  = y;
  month                 = m;
  day                   = d;
  hour                  = h;
  minute                = min;
  second                = s;
  timeZoneOffsetHours   = oh;
  timeZoneOffsetMinutes = om;
  qualifier             = UNDEFINED;
  era                   = "";
}

TimePosition::~TimePosition()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Zuweisungsoperator                                                       //
///////////////////////////////////////////////////////////////////////////////
TimePosition TimePosition::operator= ( const TimePosition &pos )
{
  year                  = pos.year;
  month                 = pos.month;
  day                   = pos.day;
  hour                  = pos.hour;
  minute                = pos.minute;
  second                = pos.second;
  timeZoneOffsetHours   = pos.timeZoneOffsetHours;
  timeZoneOffsetMinutes = pos.timeZoneOffsetMinutes;
  qualifier             = pos.qualifier;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//  Gleichheitsoperator für Zeitpunkte                                       //
///////////////////////////////////////////////////////////////////////////////
bool TimePosition::operator== ( const TimePosition &pos )
{
  if ( year == pos.year && month == pos.month && day == pos.day && 
       hour == pos.hour && minute == pos.minute && second == pos.second &&
       qualifier == pos.qualifier )
       return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Größer-Operator für Zeitpunkte                                           //
///////////////////////////////////////////////////////////////////////////////
bool TimePosition::operator > ( const TimePosition &pos )
{
  if (  year > pos.year ||
      ( year == pos.year && month > pos.month  ) || 
      ( year == pos.year && month == pos.month && day > pos.day ) ||
      ( year == pos.year && month == pos.month && day == pos.day && hour > pos.hour ) ||
      ( year == pos.year && month == pos.month && day == pos.day && hour == pos.hour && minute > pos.minute ) ||
      ( year == pos.year && month == pos.month && day == pos.day && hour == pos.hour && minute == pos.minute && second > pos.second ) )
      return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Größer/Gleich-Operator für Zeitpunkte                                    //
///////////////////////////////////////////////////////////////////////////////
bool TimePosition::operator >= ( const TimePosition &pos )
{
  if ( year >= pos.year && month >= pos.month && day >= pos.day && 
       hour >= pos.hour && minute >= pos.minute && second >= pos.second )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt eine reine Datumsangabe                                            //
///////////////////////////////////////////////////////////////////////////////
void TimePosition::setDate ( int y, int m, int d )
{
  year  = y;
  month = m;
  day   = d;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt eine reine Uhrzeit-Angabe                                          //
///////////////////////////////////////////////////////////////////////////////
void TimePosition::setTime ( int h, int m, int s, int oh, int om )
{
  hour                  = h;
  minute                = m;
  second                = s;
  timeZoneOffsetHours   = oh;
  timeZoneOffsetMinutes = om;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die XML-Repräsentation                                         //
///////////////////////////////////////////////////////////////////////////////
std::string TimePosition::toXMLString()
{
  bool   hasDate = false;
  bool   hasTime = false;
  string dateTimeString = "";
  string locString;
  char   buffer[10];

  if ( year > 0  || ( month > 0 && month <= 12 ) || ( day > 0 && day <= 31) )
    hasDate = true;

  if ( hour >= 0 && hour <= 24 )
    hasTime = true;

  if ( !hasDate && !hasTime )
    return "";

  if ( hasDate )
  {
    if ( year > 0 )
    {
      sprintf ( buffer, "%04d", year );
      locString= string ( buffer );
      dateTimeString = locString;
    }
    else
      dateTimeString = "-";

    if (  month > 0 && month <= 12 )
    {
      sprintf ( buffer, "%02d", month );
      if ( buffer[0] == ' ' )
        buffer[0] = '0';
      locString= string ( buffer );
      dateTimeString = dateTimeString + "-" + locString;
    }
    else
    if ( dateTimeString == "-")
      dateTimeString = "--";

    if (  day > 0 && day <= 31 )
    {
      sprintf ( buffer, "%02d", day );
      if ( buffer[0] == ' ' )
        buffer[0] = '0';
      locString= string ( buffer );
      dateTimeString = dateTimeString + "-" + locString;
    } 

    if ( hasTime )
      dateTimeString = dateTimeString + "T";
    else
      return dateTimeString;
  }

  sprintf ( buffer, "%02d", hour );
  locString= string ( buffer );
  if ( buffer[0] == ' ' )
    buffer[0] = '0';
  dateTimeString = dateTimeString + locString + ":";

  if ( minute >= 0 && minute <= 59 )
  {
    sprintf ( buffer, "%02d", minute );
    if ( buffer[0] == ' ' )
      buffer[0] = '0';
    locString= string ( buffer );
    dateTimeString = dateTimeString + locString + ":";
  }
  else
    dateTimeString = dateTimeString +  "00:";

  if ( second>= 0.0 && second < 60.0 )
  {
    sprintf ( buffer, "%02d", second );
    locString= string ( buffer );
    dateTimeString = dateTimeString + locString;
  }
  else
    dateTimeString = dateTimeString +  "00";

  if ( timeZoneOffsetHours != 0  )
  {
    if ( timeZoneOffsetHours > 0 && timeZoneOffsetHours < 25 )
    {
      sprintf ( buffer, "%02d", timeZoneOffsetHours );
      if ( buffer[0] == ' ' )
        buffer[0] = '0';
      locString= string ( buffer );
      dateTimeString = dateTimeString + "+" + locString;
    }
    else
    if ( timeZoneOffsetHours < 0 && timeZoneOffsetHours > -25 )
    {
      sprintf ( buffer, "%02d", -timeZoneOffsetHours );
      if ( buffer[0] == ' ' )
        buffer[0] = '0';
      locString= string ( buffer );
      dateTimeString = dateTimeString + "-" + locString;
    }

    if ( timeZoneOffsetMinutes > 0 && timeZoneOffsetMinutes < 60 )
    {
      sprintf ( buffer, "%02d", -timeZoneOffsetMinutes );
      if ( buffer[0] == ' ' )
        buffer[0] = '0';
      locString= string ( buffer );
      dateTimeString = dateTimeString + ":" + locString;
    }
    else
      dateTimeString = dateTimeString + ":00";
  }
  else
  if ( timeZoneOffsetMinutes > 0 && timeZoneOffsetMinutes < 60 )
  {
    sprintf ( buffer, "%02d", timeZoneOffsetMinutes );
    if ( buffer[0] == ' ' )
      buffer[0] = '0';
    locString= string ( buffer );
    dateTimeString = dateTimeString + "+00:" + locString;
  }

  return dateTimeString;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert einen String (aktuell identisch mit XML-Repräsentation)        //
///////////////////////////////////////////////////////////////////////////////
string TimePosition::toString()
{
  return toXMLString();
}


///////////////////////////////////////////////////////////////////////////////
//  Interpretiert die XML-Repräsentation von Datum/Uhrzeit                   //
///////////////////////////////////////////////////////////////////////////////
bool TimePosition::fromXMLString ( std::string str )
{
  string dateString = "";
  string timeString = "";
	size_t index;
  bool   success = true;

  if ( str == "" ) return false;

  index = str.find( "T" );
  if ( index != string::npos )
  {
    dateString = str.substr ( 0, index );
    timeString = str.substr( index + 1 );
  }
  else
  {
    index = str.find( ":" );
    if ( index != string::npos )
    {
      timeString = str;
      dateString = "";
    }
    else
    {
      dateString = str;
      timeString = "";
    }
  }

  if ( dateString != "" )
    success = interpreteDateString ( dateString );

  if ( success && timeString != "" )
    success = interpreteTimeString ( timeString );

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Interpretiert den Datumsanteil der XML-Repräsentation von Datum/Uhrzeit  //
///////////////////////////////////////////////////////////////////////////////
bool TimePosition::interpreteDateString ( std::string dateString )
{
  string yearString;
  string monthString = "";
  string dayString = "";
	size_t index;

  if ( dateString[0] != '-' ||  ( dateString[0] == '-' && dateString[1] != '-' ) )
  {
    index = dateString.find( "-" );
    if ( index == string::npos )
      year = atoi ( dateString.c_str() );
    else
    {
      yearString = dateString.substr( 0, index );
      year = atoi ( yearString.c_str() );
      monthString = dateString.substr( index+1 );
    }
  }
  else
    monthString = dateString.substr( 2 );

  if ( monthString != "" )
  {
    if ( monthString[0] != '-' )
    {
      index = monthString.find( "-" );
      if ( index == string::npos )
        month = atoi ( monthString.c_str() );
      else
      {
        dayString = monthString.substr( index+1 );
        monthString = monthString.substr( 0, index );
        month = atoi ( monthString.c_str() );
      }      
     }
     else
        dayString = monthString.substr( 1 );
    
    if ( dayString != "" )
      day = atoi( dayString.c_str() ); 
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Interpretiert den Uhrzeitanteil der XML-Repräsentation von Datum/Uhrzeit //
///////////////////////////////////////////////////////////////////////////////
bool TimePosition::interpreteTimeString ( std::string timeString )
{
  string hourString;
  string minuteString;
  string secondString;
  string timeZoneString;
  string timeZoneMinuteString;
	size_t index;

  index = timeString.find ( ':' );
  if ( index == string::npos ) return false;

  hourString = timeString.substr( 0, index );
  hour = atoi ( hourString.c_str() );

  minuteString = timeString.substr( index + 1 );
  index = minuteString.find ( ':' );
  if ( index == string::npos ) return false;

  secondString = minuteString.substr( index + 1 );
  minuteString = minuteString.substr( 0, index );
  minute = atoi ( minuteString.c_str() );

  if ( secondString == "" ) return false;

  index = secondString.find ( '+' );
  if ( index == string::npos )
    index = secondString.find ( '-' );

  if ( index == string::npos )
    second = atoi ( secondString.c_str() );
  else
  {
    timeZoneString = secondString.substr( index  );
    secondString = secondString.substr( 0, index );
    second = atoi ( secondString.c_str() );

    index = timeZoneString.find( ':' );
    if ( index == string::npos ) return false;

    timeZoneMinuteString = timeZoneString.substr( index + 1 );
    timeZoneOffsetMinutes = atoi ( timeZoneMinuteString.c_str() );

    timeZoneString = timeZoneString.substr( 0, index );
    timeZoneOffsetHours = atoi ( timeZoneString.c_str() );
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das Attribut qualifier über einen Text                             //
///////////////////////////////////////////////////////////////////////////////

void TimePosition::setQualifier ( std::string qualifier )
{
  if ( qualifier == "after" )
    qualifier = AFTER;
  else
  if ( qualifier == "before" )
    qualifier = BEFORE;
  else
    qualifier = UNDEFINED;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Attribut qualifier als Text                                  //
///////////////////////////////////////////////////////////////////////////////
string TimePosition::getQualifierAsString()
{
  switch ( qualifier )
  {
  case AFTER:
    return "after";

  case BEFORE:
    return "before";
  }

  return "undefined";

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Information, ob die TimePosition ein Datumsinformation hat   //
//  Uhrzeitinformation hat                                                   //
///////////////////////////////////////////////////////////////////////////////
bool TimePosition::hasDate()
{
  if ( year == 0 && month == 0 && day == 0 )
    return false;
  else
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Information, ob die TimePosition eine Uhrzeitinformation hat //
///////////////////////////////////////////////////////////////////////////////
bool TimePosition::hasTime()
{
  if ( hour == -1 && minute == -1 && second == -1 )
    return false;
  else
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Zeit auf 00:00:00                                              //
///////////////////////////////////////////////////////////////////////////////
void TimePosition::setDefaultTime()
{
  hour   = 0;
  minute = 0;
  second = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Liefert das TimePosition Objekt des nächsten Tages                        //
//  Schaltjahre werden dabei ignoriert                                       //
///////////////////////////////////////////////////////////////////////////////
TimePosition TimePosition::incrementOneDay()
{
  int nextDay   = day;
  int nextMonth = month;
  int nextYear  = year;

  if ( month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12 )
  {
    if ( day < 31 ) nextDay = day + 1;
    else
    {
      if ( month < 12 )
      {
        nextMonth = month + 1;
        nextDay   = 1;
      }
      else
      {
        nextYear  = year + 1;
        nextMonth = 1;
        nextDay   = 1;
      }
    }
  }
  else
  if ( month == 4 || month == 6 || month == 9 || month == 11 )
  {
    if ( day < 30 ) nextDay = day + 1;
    else
    {
      nextMonth = month + 1;
      nextDay   = 1;
    }
  }
  else
  if ( month == 2 )
  {
    if ( day < 28 ) nextDay = day + 1;
    else
    {
      nextMonth = month + 1;
      nextDay   = 1;
    }
  }

  return TimePosition ( nextYear, nextMonth, nextDay, hour, minute, second, timeZoneOffsetHours, timeZoneOffsetMinutes );
}

///////////////////////////////////////////////////////////////////////////////
// Liefert das TimePosition Objekt der nächsten Stunde                       //
//  Schaltjahre werden dabei ignoriert                                       //
///////////////////////////////////////////////////////////////////////////////
TimePosition TimePosition::incrementOneHour()
{
  if ( hour < 23 )
    return TimePosition ( year, month, day, hour+1, minute, second, timeZoneOffsetHours, timeZoneOffsetMinutes);
  else
  {
    TimePosition nextHour = incrementOneDay();
    nextHour.setHour( 0 );
    return nextHour;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben im XML-Format                                                  //
///////////////////////////////////////////////////////////////////////////////
void TimePosition::writeToXML ( System::Xml::XmlTextWriter ^ pWriter )
{
  string stdString= toXMLString();

  if ( getQualifier() != TimePosition::UNDEFINED )
  {
    string qualifierString = getQualifierAsString();
    pWriter->WriteAttributeString ( "indeterminatePosition",  QuConvert::ToString ( qualifierString ) );
  }

  if ( getEra() != "" )
    pWriter->WriteAttributeString ( "calendarEraName",  QuConvert::ToString ( getEra() ) );

  pWriter->WriteString ( QuConvert::ToString ( stdString ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Lesen im XML-Format                                                      //
///////////////////////////////////////////////////////////////////////////////
void TimePosition::readFromXML ( System::Xml::XmlTextReader ^ pReader )
{
  String ^ STR;
  string   str;

  STR = pReader->GetAttribute( "indeterminatePosition" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr( str, STR );
    setQualifier( str );
  }

  STR = pReader->GetAttribute ( "calendarEraName" );
  if ( STR != nullptr) 
    QuConvert::systemStr2stdStr( era, STR );

  STR = pReader->ReadString();
  QuConvert::systemStr2stdStr( str, STR );
  fromXMLString( str );
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertiert ein DateTime Objekt in einen string vom Format               //
//                dd.mm.yyyy hh:mm:ss                                        //
///////////////////////////////////////////////////////////////////////////////
std::string TimePosition::convertToString ( DateTime ^ pDateTime )
{
  string str;

  String ^ STR = pDateTime->ToString( "g" );
  QuConvert::systemStr2stdStr( str, STR );

  return str;
}


///////////////////////////////////////////////////////////////////////////////
//  TimeIntervalLength                                                       //
//  Klasse für einen Zeitraum                                                //
///////////////////////////////////////////////////////////////////////////////
TimeIntervalLength::TimeIntervalLength() : AbstractTime()
{
  unit = HOUR;
  value = 1;
}

TimeIntervalLength::TimeIntervalLength( const TimeIntervalLength & timeIntervalLengthOld )
{
  unit = timeIntervalLengthOld.unit;
  value = timeIntervalLengthOld.value;
}

TimeIntervalLength::TimeIntervalLength( TimeIntervalLength * pTimeIntervalLengthOld )
{
  unit = pTimeIntervalLengthOld->unit;
  value = pTimeIntervalLengthOld->value;
}


TimeIntervalLength::~TimeIntervalLength()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Gleichheitsoperator für Zeiträume                                        //
///////////////////////////////////////////////////////////////////////////////
bool TimeIntervalLength::operator== ( const TimeIntervalLength intervalLength )
{
  if ( unit == intervalLength.unit && value == intervalLength.value )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine Text-Repräsentation                                       //
///////////////////////////////////////////////////////////////////////////////
string TimeIntervalLength::toString()
{
  string str;
  int    decimal;
  int    sign;
  int    err;
  char   buffer[64];

  err = _fcvt_s( buffer, 64, value, 1, &decimal, &sign);

  str = string ( buffer );
  if ( decimal > 0 )
    str.insert( decimal, 1, '.' );

  str = str + " " + getTimeUnitString();
  return str;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Attribute des Zweitraumes                                      //
///////////////////////////////////////////////////////////////////////////////
bool TimeIntervalLength::set ( float valueP, std::string unitString )
{
  value = int(floor( valueP + 0.5 ));

  if ( unitString == "day" )
    unit = DAY;
  else
  if ( unitString == "hour" )
    unit = HOUR;
  else
  if ( unitString == "minute" )
    unit = MINUTE;
  else
  if ( unitString == "second" )
    unit = SECOND;
  else
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Attribut unit als Text                                       //
///////////////////////////////////////////////////////////////////////////////
string TimeIntervalLength::getTimeUnitString()
{
  switch ( unit )
  {
    case DAY:
      return "day";

    case  HOUR:
      return "hour";

    case MINUTE:
      return "minute";

    case  SECOND:
      return "second";
  }

  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine XML-Repräsentation                                        //
///////////////////////////////////////////////////////////////////////////////
void TimeIntervalLength::writeToXML ( System::Xml::XmlTextWriter ^ pWriter )
{
  pWriter->WriteAttributeString( "unit", QuConvert::ToString( getTimeUnitString() ) );
  pWriter->WriteString( XmlConvert::ToString ( value ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Interpretiert eine XML-Repräsentation                                    //
///////////////////////////////////////////////////////////////////////////////
void TimeIntervalLength::readFromXML ( System::Xml::XmlTextReader ^ pReader )
{
  std::string unitString;
  double      h;

  String ^ STR = pReader->GetAttribute( "unit" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( unitString, STR );

  STR = pReader->ReadString();
  h = XmlConvert::ToDouble( STR );

  set( (float)h, unitString );
}

///////////////////////////////////////////////////////////////////////////////
//  TimePeriod                                                               //
//  Klasse für ein Zeitintervall                                             //
///////////////////////////////////////////////////////////////////////////////
TimePeriod::TimePeriod()
{
  start = TimePosition();
  end   = TimePosition();
}

TimePeriod::TimePeriod ( const TimePeriod & timePeriodOld )
{
  start = timePeriodOld.start;
  end   = timePeriodOld.end;
}

TimePeriod::TimePeriod (  TimePeriod * pTimePeriodOld )
{
  start = pTimePeriodOld->start;
  end   = pTimePeriodOld->end;
}

TimePeriod::TimePeriod (  TimePosition & startPos, TimePosition & endPos )
{
  start = startPos;
  end   = endPos;
}

TimePeriod::~TimePeriod()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Zuweisungsoperator                                                       //
///////////////////////////////////////////////////////////////////////////////
TimePeriod  TimePeriod::operator= ( TimePeriod &period )
{
  start = period.start;
  end   = period.end;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//  Gleichheitsoperator                                                      //
///////////////////////////////////////////////////////////////////////////////
bool TimePeriod::operator== ( const TimePeriod &period )
{
  if ( start == period.start && end == period.end )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine Textdarstellung                                           //
///////////////////////////////////////////////////////////////////////////////
string TimePeriod::toString()
{
  string resultString;

  resultString = start.toString() + " - " + end.toString();
  return resultString;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob der Zeitraum am 1.1. startet                                //
///////////////////////////////////////////////////////////////////////////////
bool TimePeriod::startsAtFirstDayOfYear()
{
  if ( start.getDay() == 1 && start.getMonth() == 1 )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob der Zeitraum am 31.12. endet                                //
///////////////////////////////////////////////////////////////////////////////
bool TimePeriod::endsAtLastDayOfYear()
{
  if ( end.getDay() == 31 && end.getMonth() == 12 )
    return true;
  else
    return false;
}

/////////////////////////////////////////////////////////////////////
//  Bestimmt, ob der aktuelle Zeitraum einen Tag nach Ende von     //
//  nextPeriod beginnt                                             //
/////////////////////////////////////////////////////////////////////
bool TimePeriod::startsNextDay ( TimePeriod * nextPeriod )
{
  TimePosition nextDay = end.incrementOneDay();
  if ( nextDay == nextPeriod->getStartPosition() )
    return true;
  else
    return false;
}

//////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob der aktuelle Zeitraum nach dem Ende von nextPeriod beginnt //
//////////////////////////////////////////////////////////////////////////////
bool TimePeriod::startsAfter ( TimePeriod * nextPeriod )
{
  if ( start > nextPeriod->getEndPosition() )
    return true;
  else
    return false;
}

//////////////////////////////////////////////////////////////////////////////
//  Rausschreiben im XML-Format                                             //
//////////////////////////////////////////////////////////////////////////////
void TimePeriod::writeToXML ( System::Xml::XmlTextWriter ^ pWriter )
{
  pWriter->WriteStartElement( "gml:TimePeriod" );

  pWriter->WriteStartElement( "gml:beginPosition" );
  start.writeToXML( pWriter );
  pWriter->WriteEndElement();

  pWriter->WriteStartElement( "gml:endPosition" );
  end.writeToXML( pWriter );
  pWriter->WriteEndElement();

  pWriter->WriteEndElement();
}

//////////////////////////////////////////////////////////////////////////////
//  Einlesen im XML-Format                                                  //
//////////////////////////////////////////////////////////////////////////////
void TimePeriod::readFromXML ( System::Xml::XmlTextReader ^ pReader )
{
  while ( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      String ^ pActElement = pReader->LocalName;
      if ( pActElement->CompareTo( "beginPosition") == 0 )
        start.readFromXML ( pReader );
      else
      if ( pActElement->CompareTo( "endPosition") == 0 )
        end.readFromXML ( pReader );
    }
    else
    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      String ^ pActElement = pReader->LocalName;
      if ( pActElement->CompareTo( "TimePeriod" ) == 0 )
        break;
    }
  }
}








