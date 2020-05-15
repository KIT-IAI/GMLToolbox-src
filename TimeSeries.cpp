#include "stdafx.h"
#include <time.h>

#include "UnitOfMeasure.h"
#include "Convert.h"
#include "TimeSeries.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  AbstractTimeSeries                                                       //
//  Abstrakte Basisklasse für alle Zeitreihen-Klassen                        //
///////////////////////////////////////////////////////////////////////////////
AbstractTimeSeries::AbstractTimeSeries( string gmlIdP )
{
  gmlId              = "";
  aquisitionMethod   = "";
  qualityDescription = "";
  source             = "";
  interpolationType  = AverageInPrecedingInterval;
  unitOfMeasure      = NULL;
  refCount           = 0;
}

AbstractTimeSeries::AbstractTimeSeries( AbstractTimeSeries * pTimeSeriesOld, std::string gmlIdP )
{
  gmlId              = gmlIdP;
  aquisitionMethod   = pTimeSeriesOld->aquisitionMethod;
  qualityDescription = pTimeSeriesOld->qualityDescription;
  thematicDescription = pTimeSeriesOld->thematicDescription;
  source             = pTimeSeriesOld->source;
  interpolationType  = pTimeSeriesOld->interpolationType;
  unitOfMeasure      = pTimeSeriesOld->unitOfMeasure;
  refCount           = 0;
}



AbstractTimeSeries::~AbstractTimeSeries()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das Attribut interpolationType                                     //
///////////////////////////////////////////////////////////////////////////////
void AbstractTimeSeries::setInterpolationTypeFromString ( std::string interpolationTypeP )
{
  if ( interpolationTypeP == "averageInPrecedingInterval" )
    interpolationType = AverageInPrecedingInterval;
  else
  if ( interpolationTypeP == "averageInSucceedingInterval" )
    interpolationType = AverageInSuccedingInterval;
  else
  if ( interpolationTypeP == "constantInPrecedingInterval" )
    interpolationType = ConstantInPrecedingInterval;
  else
  if ( interpolationTypeP == "constantInSucceedingInterval" )
    interpolationType = ConstantInSuccedingInterval;
  else
  if ( interpolationTypeP == "continuous" )
    interpolationType = Continuous;
  else
  if ( interpolationTypeP == "discontinuous" )
    interpolationType = Discontinuous;
  else
  if ( interpolationTypeP == "instantaneousTotal" )
    interpolationType = InstantaneousTotal;
  else
  if ( interpolationTypeP == "maximumInPrecedingInterval" )
    interpolationType = MaximumInPrecedingInterval;
  else
  if ( interpolationTypeP == "maximumInSucceedingInterval" )
    interpolationType = MaximumInSuccedingInterval;
  else
  if ( interpolationTypeP == "minimumInPrecedingInterval" )
    interpolationType = MinimumInPrecedingInterval;
  else
  if ( interpolationTypeP == "minimumInSucceedingInterval" )
    interpolationType = MinimumInSuccedingInterval;
  else
  if ( interpolationTypeP == "precedingTotal" )
    interpolationType = PrecedingTotal;
  else
  if ( interpolationTypeP == "succeedingTotal" )
    interpolationType = SuccedingTotal;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Attribut interpolationType als Text                          //
///////////////////////////////////////////////////////////////////////////////
string AbstractTimeSeries::getInterpolationTypeAsString ()
{
  switch ( interpolationType )
  {
  case AverageInPrecedingInterval:
    return "averageInPrecedingInterval" ;
    break;

  case AverageInSuccedingInterval:
    return "averageInSucceedingInterval" ;
    break;

  case ConstantInPrecedingInterval:
    return  "constantInPrecedingInterval";
    break;

  case ConstantInSuccedingInterval:
    return "constantInSucceedingInterval";
    break;

  case Continuous:
    return "continuous";
    break;

  case Discontinuous:
    return  "discontinuous";
    break;

  case InstantaneousTotal:
    return  "instantaneousTotal";
    break;

  case MaximumInPrecedingInterval:
    return "maximumInPrecedingInterval";
    break;

  case MaximumInSuccedingInterval:
    return "maximumInSucceedingInterval";
    break;

  case MinimumInPrecedingInterval:
    return "minimumInPrecedingInterval";
    break;

  case MinimumInSuccedingInterval:
    return "minimumInSucceedingInterval";
    break;

  case PrecedingTotal:
    return "precedingTotal";
    break;

  case SuccedingTotal:
    return  "succeedingTotal";
    break;
  }
  return "";
}

///////////////////////////////////////////////////////////////////////////////
// Liefert die Metadaten einer Zeitreihe als Text                            //
///////////////////////////////////////////////////////////////////////////////
std::string AbstractTimeSeries::getTimeSeriesMetadata()
{
  string outString;

  if ( aquisitionMethod != "" )
    outString = "Erzeugungsmethode: " + aquisitionMethod + "\r\n";

  if (qualityDescription != "" )
    outString = outString + "Qualität:" + qualityDescription + "\r\n";

  if ( source != "" )
    outString = outString + "Datenquelle:" + source + "; ";

  if ( thematicDescription != "" )
    outString = outString + "Thematische Beschreibung:" + thematicDescription + "; ";

  outString = outString + "Interpolation:" + getInterpolationTypeAsString() + "\r\n";

  if ( unitOfMeasure != NULL )
    outString = outString + "UOM der Werte: " + unitOfMeasure->getSign() + "\r\n";

  return outString;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest die metadaten einer Zeitreihe im Energy ADE XML-Format ein         //
///////////////////////////////////////////////////////////////////////////////
void AbstractTimeSeries::readPropertiesFromXML ( XmlTextReader ^ pReader )
{
  String ^ STR;

  while ( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      String ^ pActElement = pReader->LocalName;

      if ( pActElement->CompareTo( "acquisitionMethod" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( aquisitionMethod, STR );
      }
      else
      if ( pActElement->CompareTo( "interpolationType" ) == 0 )
      {
        string str;
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( str, STR );
        setInterpolationTypeFromString ( str );
      }
      else
      if ( pActElement->CompareTo( "qualityDescription" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( qualityDescription, STR );
      }
      else
      if ( pActElement->CompareTo( "source" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( source, STR );
      }
      else
      if ( pActElement->CompareTo( "thematicDescription" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( thematicDescription, STR );
      }
			else
			if ( pActElement->CompareTo( "property" ) == 0 )
			{
				STR = pReader->ReadString();
				QuConvert::systemStr2stdStr( property, STR );
			}
			else
			if ( pActElement->CompareTo( "propertyQualifier" ) == 0 )
			{
				STR = pReader->ReadString();
				QuConvert::systemStr2stdStr( propertyQualifier, STR );
			}
		}
    else
    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      String ^ pActElement = pReader->LocalName;
      if ( pActElement->CompareTo( "TimeValuesProperties" ) == 0 )
        break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  RegularTimeSeries                                                        //
//  Reguläre Zeitreihe, d.h. Messwerte mit konstantem Zeitinkrement,         //
//  wobei die Werte in Objekt gespeichert werden                             //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeries::RegularTimeSeries( std::string gmlId  ) : AbstractTimeSeries ( gmlId )
{
  temporalExtent = NULL;
  timeInterval   = NULL;
  missingValue   = 9.99E20;
}
RegularTimeSeries::RegularTimeSeries( RegularTimeSeries * pTimeSeriesOld, std::string gmlIdP ) : AbstractTimeSeries ( pTimeSeriesOld, gmlIdP )
{
  temporalExtent = new TimePeriod ( pTimeSeriesOld->temporalExtent );
  timeInterval   = new TimeIntervalLength ( pTimeSeriesOld->timeInterval );
  for ( unsigned int i = 0; i < pTimeSeriesOld->values.size(); i++ )
    values.push_back( pTimeSeriesOld->values[i] );
}


RegularTimeSeries::~RegularTimeSeries()
{
  if ( temporalExtent != NULL ) delete temporalExtent;
  if ( timeInterval != NULL ) delete timeInterval;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die zeitliche Ausdehnung einer regulären Zeitreihe                 //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeries::setTemporalExtent ( TimePeriod * period )
{
  if ( temporalExtent != NULL ) delete temporalExtent;
  temporalExtent = period;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das Zeitintervall einer regulären Zeitreihe                        //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeries::setTimeIntervalLength ( TimeIntervalLength * interval )
{
  if ( timeInterval != NULL ) delete timeInterval;
  timeInterval = interval;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Werte einer regulären Zeitreihe                                //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeries::setValues ( std::vector<double> &v )
{
  values.clear();
  for ( unsigned int i = 0; i < v.size(); i++ )
    values.push_back( v[i] );

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Wert einer regulären Zeitreihe an Position index             //
///////////////////////////////////////////////////////////////////////////////
double RegularTimeSeries::getValue ( size_t index )
{
  if ( index < 0 || index >= values.size() )
    return 0.0;
  else
    return values[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Zeitpunkt eines Zeitreihen-Wertes an Position i              //
///////////////////////////////////////////////////////////////////////////////
DateTime ^ RegularTimeSeries::getDateTime (  unsigned int i )
{
  int year = temporalExtent->getStartPosition().getYear();
  int month = temporalExtent->getStartPosition().getMonth();
  int day = temporalExtent->getStartPosition().getDay();
  int hour = temporalExtent->getStartPosition().getHour();

  DateTime ^ pStartTime = gcnew DateTime ( year, month, day, hour, 0, 0 );
  TimeSpan ^  pTimeIncrement;

  int incrementValue = timeInterval->getValue();
  switch ( timeInterval->getUnit() )
  {
  case TimeIntervalLength::DAY:
    pTimeIncrement = gcnew TimeSpan ( i*incrementValue, 0, 0, 0, 0 );
    break;

  case TimeIntervalLength::HOUR:
    pTimeIncrement = gcnew TimeSpan ( 0, i*incrementValue, 0, 0, 0 );
    break;

  case TimeIntervalLength::MINUTE:
    pTimeIncrement = gcnew TimeSpan ( 0, 0, i*incrementValue, 0, 0 );
    break;

  case TimeIntervalLength::SECOND:
    pTimeIncrement = gcnew TimeSpan ( 0, 0, 0, i*incrementValue, 0 );
    break;
  }

  DateTime ^ pDateTime = gcnew DateTime;
  pDateTime = *pStartTime + *pTimeIncrement;
 
  return pDateTime;         
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Zeitreihen-Werte                                            //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeries::getValues ( System::Collections::Generic::List<double> ^ vValues )
{
  for ( unsigned int i = 0; i < values.size(); ++i )
    vValues->Add ( values[i] );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Wert einer regulären Zeitreihe an Position index               //
///////////////////////////////////////////////////////////////////////////////
bool RegularTimeSeries::setValue ( int index, double value )
{
  if ( index < 0 || index >= (int)values.size() )
    return false;
  else
  {
    values[index] = value;
    return true;
  }
}

////////////////////////////////////////////////////////////////////////////////
//  Liefert alle Werte einer regulären Zeitreide, mit " " Characters getrennt,//
//  als Text                                                                  //
////////////////////////////////////////////////////////////////////////////////
string RegularTimeSeries::getValuesAsString()
{
  string   str = "";
  String ^ STR;
  string   strLoc;

  for ( unsigned int i = 0; i < values.size(); i++ )
  {
    STR = Double ( values[i] ).ToString( "F3" );
    QuConvert::systemStr2stdStr( strLoc, STR );
    if ( i < values.size()-1 )
      str = str + strLoc + " ";
    else
      str = str + strLoc;
  }

  return str;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Metadaten einer regulären Zeitreihe plus eventuelle Fehler-  //
//  informationen als Text                                                   //
///////////////////////////////////////////////////////////////////////////////
std::string RegularTimeSeries::getTimeSeriesMetadata()
{
  std::string outString = "Reguläre Zeitreihe\r\n" + AbstractTimeSeries::getTimeSeriesMetadata() + "\r\n";

  if ( temporalExtent != NULL )
    outString = outString + temporalExtent->toString() + "\r\n";
  else
    outString = outString + "Kein Start-/Endzeitpunkt definiert\r\n";

  if ( timeInterval != NULL )
    outString = outString + "Zeitintervall " + timeInterval->toString() + "\r\n";
  else
    outString = outString + "Kein Zeitintervall definiert\r\n";

  return outString;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den maximalen gesetzten Wert einer regulären Zeitreihe           //
///////////////////////////////////////////////////////////////////////////////
double RegularTimeSeries::getMaximalValue()
{
  double result = -1.0E10;

  for ( unsigned int i = 0; i < values.size(); i++ )
  {
    if ( fabs( values[i] - missingValue ) < 1.0 )
      continue;

    if ( fabs ( values[i] ) > result )
      result = fabs ( values[i] );
  }
  return result;
}

///////////////////////////////////////////////////////////////////////////////
//  Skaliert alle gesetzten Werte einer regulären Zeitreihe                  //
///////////////////////////////////////////////////////////////////////////////
void   RegularTimeSeries::scale ( double factor )
{
  for ( unsigned int i = 0; i < values.size(); i++ )
  {
    if ( fabs( values[i] - missingValue ) < 1.0 )
      continue;

    values[i] = values[i] * factor;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Summe aller gesetzten Werte einer regulären Zeitreihe        //
///////////////////////////////////////////////////////////////////////////////
double RegularTimeSeries::getSum()
{
  double result = 0.0;
  for ( unsigned int i = 0; i < values.size(); i++ )
  {
    if ( fabs( values[i] - missingValue ) < 1.0 )
      continue;

    result = result + values[i];
  }
  return result;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Mittelwert aller gesetzten Werte einer regulären Zeitreihe   //
///////////////////////////////////////////////////////////////////////////////
double RegularTimeSeries::getAverage()
{
  double result = 0.0;
  int    anz = 0;
  for ( unsigned int i = 0; i < values.size(); i++ )
  {
    if ( fabs( values[i] - missingValue ) < 1.0 )
      continue;

    result = result + values[i];
    anz++;    
  }

  if ( anz > 0 )
    return result / anz;
  else
    return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der nicht gesetzten Werte einer regulären Zeitreihe   //
///////////////////////////////////////////////////////////////////////////////
int RegularTimeSeries::getMissingValuesCount()
{
  int anz = 0;

  for ( unsigned int i = 0; i < values.size(); i++ )
  {
    if ( fabs( values[i] - missingValue ) < 1.0 )
     anz++;
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Bereiche mit nicht gesetzten Werten               //
///////////////////////////////////////////////////////////////////////////////
int RegularTimeSeries::getGapCount()
{
  bool gapActive = false;
  int  gapCount = 0;

  if ( fabs( values[0] - missingValue ) < 1.0 )
  {
    gapActive = true;
    gapCount = 1;
  }

  for ( unsigned int i = 0; i < values.size(); i++ )
  {
    if ( gapActive && fabs( values[0] - missingValue ) > 1.0 )
      gapActive = false;

    if ( !gapActive && fabs( values[0] - missingValue ) < 1.0 )
    {
      gapActive = true;
      gapCount++;
    }
  }

  return gapCount;
}

///////////////////////////////////////////////////////////////////////////////
//  Füllt Löcher in der Zeitreihe durch eine lineare Interpolation zwischen  //
//  letzten vorhandenen Werten. Bei einem Loch am Anfang oder Ende der Zeit- //
//  reihe wird der erste bzw. letzte Wert kopiert.                           //
//  Voraussetzung ist, dass die Größe des Loches den übergebenen Maximal-    //
//  wert nicht überschreitet                                                 //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeries::interpolateGaps( int maxGapSize )
{
  struct Gap
  {
		size_t startIndex;
		size_t endIndex;
    double startValue;
    double endValue;
  };
  vector<Gap> vGaps;
  bool        gapActive;
  Gap         activeGap;
  int         i;

  if ( values.size() == 0 )
    return;

  if ( values[0] == missingValue )
  {
    gapActive = true;
    activeGap = Gap();
    activeGap.startIndex = 0;
    activeGap.startValue = missingValue;
  }
  else
    gapActive = false;

  for ( i = 1; i < (int)values.size(); ++i )
  {
    if ( gapActive && values[i] != missingValue )
    {
      gapActive = false;
      activeGap.endIndex = i;
      activeGap.endValue = values[i];
      vGaps.push_back( activeGap );
    }
    else
    if ( !gapActive && values[i] == missingValue )
    {
      gapActive = true;
      activeGap = Gap();
      activeGap.startIndex = i;
      activeGap.startValue = values[i-1];
    }
  }

  if ( gapActive )
  {
    activeGap.endIndex = values.size();
    activeGap.endValue = missingValue;
    vGaps.push_back( activeGap );
  }

  for ( i = 0; i < (int)vGaps.size(); ++i )
  {
    activeGap = vGaps[i];

    if ( activeGap.endIndex - activeGap.startIndex > maxGapSize )
      continue;

    if (  activeGap.endIndex == values.size() )
    {
      for ( size_t j = activeGap.startIndex; j < activeGap.endIndex; ++j )
        values[j] = activeGap.startValue;
    }
    else
    {
      double d = (activeGap.endValue - activeGap.startValue)/(activeGap.endIndex - activeGap.startIndex + 1);
      for ( size_t j = activeGap.startIndex; j < activeGap.endIndex; ++j )
        values[j] = activeGap.startValue + (j-activeGap.startIndex+1)*d;
    }
  }  
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die reguläre Zeitreihe im Energy ADE XML-Format                 //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeries::writeToXML ( XmlTextWriter ^ pWriter )
{
  pWriter->WriteStartElement( "energy:RegularTimeSeries" );

  pWriter->WriteStartElement( "energy:variableProperties" );
  pWriter->WriteStartElement( "energy:TimeValuesProperties" );

  pWriter->WriteElementString( "energy:acquisitionMethod", QuConvert::ToString( getAquisitionMethod() ) );
  pWriter->WriteElementString( "energy:interpolationType", QuConvert::ToString( getInterpolationTypeAsString() ) );
  if ( getQualityDescription() != "" )
    pWriter->WriteElementString( "energy:qualityDescription", QuConvert::ToString( getQualityDescription() ) );
  if ( getSource() != "" )
    pWriter->WriteElementString( "energy:source", QuConvert::ToString( getSource() ) );
  if ( getThematicDescription() != "" )
    pWriter->WriteElementString( "energy:thematicDescription", QuConvert::ToString( getThematicDescription() ) );
	if ( getProperty() != "" )
		pWriter->WriteElementString( "energy:property", QuConvert::ToString( getProperty() ) );
	if ( getPropertyQualifier() != "" )
		pWriter->WriteElementString( "energy:propertyQualifier", QuConvert::ToString( getPropertyQualifier() ) );

  pWriter->WriteEndElement();
  pWriter->WriteEndElement();

  pWriter->WriteStartElement( "energy:temporalExtent" );
  getTemporalExtent()->writeToXML( pWriter );
  pWriter->WriteEndElement();

  pWriter->WriteStartElement( "energy:timeInterval" );
  getTimeIntervalLength()->writeToXML( pWriter );
  pWriter->WriteEndElement();

  pWriter->WriteStartElement( "energy:values" );
  if ( getUOM() != NULL )
    pWriter->WriteAttributeString( "uom", QuConvert::ToString( getUOM()->getIdentifier() ) );
  else
    pWriter->WriteAttributeString( "uom", "unknown" );


  String ^ STR = "";
  for ( unsigned int i = 0; i < getValueCount(); i++ )
  {
    STR = String::Concat ( STR, XmlConvert::ToString( getValue( i ) ), " " );
    if ( i > 0 && i % 20 == 0 )
      STR = String::Concat ( STR, "\n" );
  }

  pWriter->WriteString( STR );
  pWriter->WriteEndElement();

  pWriter->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Liest die reguläre Zeitreihe im Energy ADE XML-Format                    //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeries::readFromXML ( XmlTextReader ^ pReader, UOMList * pUOMList )
{
  String ^ STR;

  STR = pReader->GetAttribute( "gml:id" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( gmlId, STR );

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      String ^ pActElement = pReader->LocalName;

      if (  pActElement->CompareTo( "variableProperties" ) == 0 )
        readPropertiesFromXML ( pReader);
      else
      if ( pActElement->CompareTo( "temporalExtent" ) == 0 )
      {
        TimePeriod * pTimePeriod = new TimePeriod;
        pTimePeriod->readFromXML( pReader );
        setTemporalExtent( pTimePeriod );
      }
      else
      if ( pActElement->CompareTo( "timeInterval" ) == 0 )
      {
        TimeIntervalLength * pTimeIntervalLength = new TimeIntervalLength;
        pTimeIntervalLength->readFromXML( pReader );
        setTimeIntervalLength( pTimeIntervalLength );
      }
      else
      if ( pActElement->CompareTo( "values" ) == 0 )
      {
        cli::array<String^> ^ split;
        cli::array<wchar_t> ^ delimiter = gcnew cli::array<wchar_t> {_T(' '), _T('\n'), _T('\t'), _T('\r'), _T('\f') };
        vector<double>   values;

        string uom = "";
        while ( pReader->MoveToNextAttribute() )
        {
          STR = pReader->Name;
          if ( STR->CompareTo( "uom" ) == 0 )
          {
            STR = pReader->Value;
            QuConvert::systemStr2stdStr( uom, STR );
            break;
          }
        }

        UOM * pUOM = pUOMList->getUOMFromIdentifier( uom );
        setUOM( pUOM );

        STR = pReader->ReadString();
        split = STR->Split(delimiter, StringSplitOptions::RemoveEmptyEntries );
        for (int i = 0; i < split->Length; i++)
          addValue(XmlConvert::ToDouble(split[i]));
      }
    }
    else
    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      String ^ pActElement    = pReader->LocalName;
      if ( pActElement->CompareTo( "RegularTimeSeries" ) == 0 )
        break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Aggregation von Zeitreihen                                               //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeries * RegularTimeSeries::aggregateTimeSeries ( std::vector<RegularTimeSeries*> &vTimeSeries, 
              int aggregationTyp )
{
  if ( vTimeSeries.size() == 0 )
    return NULL;

  string gmlId =  vTimeSeries[0]->getGmlId() + "_aggregated";

  RegularTimeSeries * pTimeSeries = new RegularTimeSeries ( vTimeSeries[0], gmlId );
  vector<double> vValuesAggregated;

	size_t anz = vTimeSeries[0]->getValueCount();

  for ( size_t j = 0; j < anz; ++j )
  {
    double h = 0.0;
    for ( unsigned int i = 0; i < vTimeSeries.size(); ++i )
    {
      h = h + vTimeSeries[i]->getValue( j );
    }
    if ( aggregationTyp == (int)Diagramm::AGGREGATION_TYP::MITTELWERT )
      h = h / vTimeSeries.size();
    vValuesAggregated.push_back( h );
  }
  pTimeSeries->setValues( vValuesAggregated );
  return pTimeSeries;
}

///////////////////////////////////////////////////////////////////////////////
//  RegularTimeSeriesSet                                                     //
//  Menge von thematisch zusammenhängenden Zeitreihen, z.B. Satz von         //
//  Simulationsergebnissen.                                                  //
//  Aktuell beschränkt auf Zeitreihen, die ein gesamtes Jahr mit 1h Inkrement//
//  überspannen                                                              //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeriesSet::RegularTimeSeriesSet ( std::string gmlId, RegularTimeSeriesSetProperties * pProperties )
{
  m_gmlId     = gmlId;
  m_pMetaData = pProperties;
}

RegularTimeSeriesSet::RegularTimeSeriesSet ( std::string gmlId )
{
  m_gmlId     = gmlId;
  m_pMetaData = NULL;
}

RegularTimeSeriesSet:: ~RegularTimeSeriesSet()
{
  if ( m_pMetaData != NULL )
    delete m_pMetaData;

  for ( unsigned int i = 0; i < m_vTimeSeries.size(); ++i )
  {
    m_vTimeSeries[i]->decRefCount();
    if ( m_vTimeSeries[i]->getRefCount() <= 0 )
      delete m_vTimeSeries[i];
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Metadaten des RegularTimeSeriesSet                             //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSet::setMetaData ( RegularTimeSeriesSetProperties * pMetadata )
{
  if ( m_pMetaData != NULL )
    delete m_pMetaData;

  m_pMetaData = pMetadata;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Zeitreihe hinzu und setzt ggf. das Jahr der Zeitreihe auf das  //
//  Jahr der Zeitreihen-Menge.                                               //
///////////////////////////////////////////////////////////////////////////////
bool RegularTimeSeriesSet::addTimeSeries( RegularTimeSeries * pTimeSeries, bool adjustYear )
{
  TimeIntervalLength * pIntervalLength = pTimeSeries->getTimeIntervalLength();
  if ( pIntervalLength->getUnit() != TimeIntervalLength::HOUR || pIntervalLength->getValue() != 1.0 )
    return false;

  TimePeriod * pTimePeriod = pTimeSeries->getTemporalExtent();
  if ( !pTimePeriod->startsAtFirstDayOfYear() || !pTimePeriod->endsAtLastDayOfYear() )
    return false;

  pTimeSeries->incRefCount();

  if ( m_pMetaData != NULL && adjustYear )
  {
    TimePosition startPos = pTimePeriod->getStartPosition();
    startPos.setYear ( m_pMetaData->getYear() );
    pTimePeriod->setStartPosition( startPos );

    TimePosition endPos = pTimePeriod->getEndPosition();
    endPos.setYear( m_pMetaData->getYear() );
    pTimePeriod->setEndPosition( endPos );
  }

  m_vTimeSeries.push_back( pTimeSeries );
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die an einer best. Indexposition stehende Zeitreihe              //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeries * RegularTimeSeriesSet::getTimeSeries ( unsigned int index )
{
  if ( index < 0 || index >= m_vTimeSeries.size() )
    return NULL;
  else
    return m_vTimeSeries[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Scheibt das Objekt im TimeSeries-ADE Format auf eine XML-Datei           //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSet::writeToXML ( String ^ fileName, String ^ energyADENamespace, String ^ timeSeriesADENamespace )
{
  char              buffer[16];
  _strdate_s ( buffer, 16 );
  String ^ date = QuConvert::ToString ( string ( buffer ) );

  System::Text::UTF8Encoding ^ utf8	    = gcnew System::Text::UTF8Encoding();
  XmlTextWriter              ^ pWriter	=  gcnew XmlTextWriter( fileName, utf8);

  pWriter->Formatting = Formatting::Indented;
  pWriter->WriteStartDocument( true );

  String ^ comment = "Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: ";
  comment = String::Concat ( comment, date );
  pWriter->WriteComment( comment );

  pWriter->WriteStartElement( "gml:FeatureCollection" );
  pWriter->WriteAttributeString( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );
  pWriter->WriteAttributeString( "xmlns:xs", "http://www.w3.org/2001/XMLSchema" );
  pWriter->WriteAttributeString( "xmlns:xlink", "http://www.w3.org/1999/xlink" );
  pWriter->WriteAttributeString( "xmlns:gml", "http://www.opengis.net/gml" );
  pWriter->WriteAttributeString( "xmlns:energy", energyADENamespace );
  pWriter->WriteAttributeString( "xmlns:ts", timeSeriesADENamespace );

  pWriter->WriteStartElement( "gml:featureMember" );
  writeToXML( pWriter );
  pWriter->WriteEndElement();

  pWriter->WriteEndElement();

  pWriter->Flush();
  pWriter->Close();
}

void RegularTimeSeriesSet::writeToXML ( XmlTextWriter ^ pWriter )
{
  pWriter->WriteStartElement( "ts:RegularTimeSeriesSet" );
  pWriter->WriteAttributeString( "gml:id", QuConvert::ToString( m_gmlId ) );

  pWriter->WriteStartElement( "ts:properties" );
  m_pMetaData->writeToXml( pWriter );
  pWriter->WriteEndElement();

  for ( unsigned int i = 0; i < m_vTimeSeries.size(); ++i )
  {
    RegularTimeSeries * pTimeSeries = m_vTimeSeries[i];

    pWriter->WriteStartElement( "ts:timeSeries" );
    pTimeSeries->writeToXML( pWriter );
    pWriter->WriteEndElement();
  }


  pWriter->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines RegularTimeSeriesSet im TimeSeries-ADE Format             //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSet::readFromXML ( XmlTextReader ^ pReader, UOMList * pUOMList )
{
  String ^ STR;

  STR = pReader->GetAttribute( "gml:id" );
  if ( STR != nullptr )
   QuConvert::systemStr2stdStr ( m_gmlId, STR );

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      String ^ pActElement = pReader->LocalName;

      if ( pActElement->CompareTo( "RegularTimeSeriesSetProperties" ) == 0 )
      {
        if ( m_pMetaData != NULL )
          delete m_pMetaData;
        m_pMetaData = new RegularTimeSeriesSetProperties( "" );
        m_pMetaData->readFromXML ( pReader );
      }
      else
      if ( pActElement->CompareTo( "RegularTimeSeries" ) == 0 )
      {
        RegularTimeSeries * pTimeSeries = new RegularTimeSeries ( "" );
        pTimeSeries->readFromXML ( pReader, pUOMList );
        m_vTimeSeries.push_back( pTimeSeries );
        pTimeSeries->incRefCount();
      }
    }
    else
    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      String ^ pActElement = pReader->LocalName;
      if ( pActElement->CompareTo( "RegularTimeSeriesSet" ) == 0 )
        break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt das Objekt als CSV-Datei                                        //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSet::writeToCSV ( std::string fileName )
{
  StreamWriter ^ pWriter = gcnew StreamWriter( QuConvert::ToString( fileName ) );
  unsigned int   i, j;
  String        ^ STR_OUT;
  String        ^ cell1Content;
  String        ^ cell2Content;


	size_t maxValueAnz = 0;
  STR_OUT = "";
  for ( i = 0; i < m_vTimeSeries.size(); i++ )
  {
    RegularTimeSeries * pTimeSeries = m_vTimeSeries[i];
		size_t valueAnz = pTimeSeries->getValueCount();

    if ( maxValueAnz < valueAnz )
      maxValueAnz = valueAnz;

    string thematicDescription = pTimeSeries->getThematicDescription();
    cell1Content = "Date;";
    cell2Content = QuConvert::ToString ( thematicDescription );

		if ( i == 0 )
			STR_OUT = String::Concat( STR_OUT, cell1Content, cell2Content, ";" );
		else
		if ( i <  m_vTimeSeries.size() - 1 )
      STR_OUT = String::Concat( STR_OUT, cell2Content, ";" );
    else
      STR_OUT = String::Concat( STR_OUT, cell2Content );
  }

  pWriter->WriteLine( STR_OUT );

  for ( i = 0; i < maxValueAnz; i++ )
  {
    STR_OUT = "";
    for ( j = 0; j < m_vTimeSeries.size(); ++j )
    {
      RegularTimeSeries * pTimeSeries = m_vTimeSeries[j];

      if ( i < pTimeSeries->getValueCount() )
      {
        double value = pTimeSeries->getValue( i );
        DateTime ^ pDateTime = pTimeSeries->getDateTime( i );
        string dateTimeString = TimePosition::convertToString( pDateTime );
				if ( j == 0 )
				{
					cell1Content = QuConvert::ToString( dateTimeString );
					cell2Content = Convert::ToString( value );
				}
				else
				{
					cell1Content = "";
					cell2Content = Convert::ToString( value );
				}
      }
      else
      {
        cell1Content = "";
        cell2Content = "";
      }

			if ( j == 0 )
				STR_OUT = String::Concat( STR_OUT, cell1Content, ";", cell2Content, ";" );
			else
      if ( j <  m_vTimeSeries.size() - 1 )
        STR_OUT = String::Concat( STR_OUT, cell2Content, ";" );
      else
        STR_OUT = String::Concat( STR_OUT, cell2Content );
    }
    pWriter->WriteLine( STR_OUT );
  }

  pWriter->Flush();
  pWriter->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  RegularTimeSeriesSetProperties                                           //
//  Metadaten eines RegularTimeSeriesSet                                     //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeriesSetProperties::RegularTimeSeriesSetProperties ( std::string gmlId )
{
  m_gmlId             = gmlId;
  m_timeSeriesSetType = "Unknown";

  TimePosition start = TimePosition ( 2022, 1, 1 );
  TimePosition end   = TimePosition ( 2022, 12, 31 );
  m_temporalExtent = new TimePeriod ( start, end );
  m_year           = 2022;

  m_timeInterval = new TimeIntervalLength ();
  m_timeInterval->setUnit ( TimeIntervalLength::HOUR );
  m_timeInterval->setValue( 1 );

  DateTime today = DateTime::Now;

  m_generationDate = new TimePosition ( today.Year, today.Month, today.Day, today.Hour, today.Minute, today.Second );

  m_application = "Unknown";
  m_description = "";
}

RegularTimeSeriesSetProperties::~RegularTimeSeriesSetProperties()
{
  delete m_temporalExtent;
  delete m_timeInterval;

  for ( unsigned int i = 0; i < m_vRelatedObjects.size(); ++i )
    delete m_vRelatedObjects[i];

  for ( unsigned int i = 0; i < m_vRelatedTimeSeriesSets.size(); ++i )
    delete m_vRelatedTimeSeriesSets[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die zeitliche Ausdehnung der Zeitreihen-Menge auf year             //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSetProperties::setYear ( int year )
{
  m_year = year;

  m_temporalExtent->getStartPosition().setYear( year );
  m_temporalExtent->getEndPosition().setYear( year );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das Startjahr der Zeitreihen-Menge                                 //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSetProperties::setStartYear ( int year )
{
  m_temporalExtent->getStartPosition().setYear( year );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Startjahr der Zeitreihen-Menge                               //
///////////////////////////////////////////////////////////////////////////////
int RegularTimeSeriesSetProperties::getStartYear ()
{
  return m_temporalExtent->getStartPosition().getYear();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das Endjahr der Zeitreihen-Menge                                   //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSetProperties::setEndYear ( int year )
{
  m_temporalExtent->getEndPosition().setYear( year );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Endjahr der Zeitreihen-Menge                                 //
///////////////////////////////////////////////////////////////////////////////
int RegularTimeSeriesSetProperties::getEndYear ()
{
  return m_temporalExtent->getEndPosition().getYear();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das an Position index referierte verbundenes Objekt              //
///////////////////////////////////////////////////////////////////////////////
RelatedObject * RegularTimeSeriesSetProperties::getRelatedObject ( unsigned int index )
{
  if ( index < 0 || index >= m_vRelatedObjects.size() )
    return NULL;
  else
    return m_vRelatedObjects[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Metadaten der Zeitreihe an Position index                    //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeriesProperties * RegularTimeSeriesSetProperties::getTimeSeriesProperties ( unsigned int index )
{
  if ( index < 0 || index >= m_vTimeSeriesMetaData.size() )
    return NULL;
  else
    return m_vTimeSeriesMetaData[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle key Words, getrennt durch ";"                               //
///////////////////////////////////////////////////////////////////////////////
std::string RegularTimeSeriesSetProperties::getKeyWordsCombined()
{
  std::string str = "";

  for ( unsigned int i = 0; i < m_vKeyWords.size(); ++i )
  {
    if ( i < m_vKeyWords.size() - 1 )
      str = str + m_vKeyWords[i] + "; ";
    else
      str = str + m_vKeyWords[i];
  }
  return str;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob ein bestimmtes key word vorhanden ist                       //
///////////////////////////////////////////////////////////////////////////////
bool RegularTimeSeriesSetProperties::hasKeyWord ( std::string keyWord )
{
  for ( unsigned int i = 0; i < m_vKeyWords.size(); ++i )
  {
    if ( keyWord == m_vKeyWords[i] )
      return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein an einer best. Indexposition stehendes RelatedSet            //
///////////////////////////////////////////////////////////////////////////////
RelatedSet * RegularTimeSeriesSetProperties::getRelatedSet ( unsigned int index )
{
  if ( index < 0 || index >= m_vRelatedTimeSeriesSets.size() )
    return NULL;
  else
    return m_vRelatedTimeSeriesSets[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob eine Verbindung mit einem bestimmten Objekt, identifiziert  //
//  über die id, vorhanden ist                                               //
///////////////////////////////////////////////////////////////////////////////
bool RegularTimeSeriesSetProperties::isRelatedWithObject ( std::string id )
{
  for ( unsigned int i = 0; i < m_vRelatedObjects.size(); ++i )
  {
    RelatedObject * pRel = m_vRelatedObjects[i];
    string name = pRel->getId();
    if ( name == id )
      return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Scheiben des Objektes im TimeSeries-ADE XML-Format                       //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSetProperties::writeToXml ( XmlTextWriter ^ pWriter )
{
  pWriter->WriteStartElement( "ts:RegularTimeSeriesSetProperties" );
  if ( m_gmlId != "" )
    pWriter->WriteAttributeString( "gml:id", QuConvert::ToString( m_gmlId ) );

  pWriter->WriteElementString( "ts:type", QuConvert::ToString( m_timeSeriesSetType ) );
  
  pWriter->WriteStartElement( "ts:temporalExtent" );
  m_temporalExtent->writeToXML( pWriter );
  pWriter->WriteEndElement();

  pWriter->WriteElementString( "ts:year", XmlConvert::ToString( m_year ) );

  pWriter->WriteStartElement ( "ts:timeInterval" );
  m_timeInterval->writeToXML( pWriter );
  pWriter->WriteEndElement();

  pWriter->WriteStartElement( "ts:generationDate" );
  m_generationDate.writeToXML( pWriter );
  pWriter->WriteEndElement();

  if ( m_application != "" )
    pWriter->WriteElementString( "ts:application", QuConvert::ToString( m_application ) );

  if ( m_description != " ")
    pWriter->WriteElementString( "ts:description", QuConvert::ToString( m_description ) );

  for ( unsigned int i = 0; i < m_vKeyWords.size(); ++i )
    pWriter->WriteElementString( "ts:keyWord", QuConvert::ToString( m_vKeyWords[i] ) );

  for ( unsigned int i = 0; i < m_vRelatedObjects.size(); ++i )
  {
    RelatedObject * pRelObj = m_vRelatedObjects[i];

    pWriter->WriteStartElement( "ts:relatedObject" );
    pWriter->WriteStartElement( "ts:RelatedObject" );

    pWriter->WriteElementString( "ts:id", QuConvert::ToString( pRelObj->getId() ) );
    if ( pRelObj->getName() != "" )
      pWriter->WriteElementString( "ts:name", QuConvert::ToString( pRelObj->getName() ) );
    if ( pRelObj->getURI() != "" )
      pWriter->WriteElementString( "ts:link", QuConvert::ToString( pRelObj->getURI() ) );

    pWriter->WriteEndElement();
    pWriter->WriteEndElement();
  }

  for ( unsigned int i = 0; i < m_vRelatedTimeSeriesSets.size(); ++i )
  {
    RelatedSet * pRelatedSet = m_vRelatedTimeSeriesSets[i];

    pWriter->WriteStartElement( "ts:relatedSet" );
    pRelatedSet->writeToXML( pWriter );
    pWriter->WriteEndElement();
  }

  pWriter->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Lesen eines Objektes im TimeSeries-ADE XML-Format                        //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSetProperties::readFromXML ( XmlTextReader ^ pReader )
{
  RelatedObject * pRelatedObject;
  RelatedSet    * pRelatedSet;
  string          str;

  String ^ STR = pReader->GetAttribute ( "gml:id" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( m_gmlId, STR );

  while ( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      String ^ pActElement = pReader->LocalName;

      if ( pActElement->CompareTo( "type" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( m_timeSeriesSetType, STR );
      }
      else
      if ( pActElement->CompareTo( "temporalExtent" ) == 0 )
      {
        if ( m_temporalExtent != NULL )
          delete m_temporalExtent;
        m_temporalExtent = new TimePeriod;
        m_temporalExtent->readFromXML( pReader );
      }
      else
      if ( pActElement->CompareTo( "year" ) == 0 )
      {
        STR = pReader->ReadString();
        m_year = XmlConvert::ToInt32( STR );
      }
      else
      if ( pActElement->CompareTo( "timeInterval" ) == 0 )
      {
        if ( m_timeInterval != NULL )
          delete m_timeInterval;
        m_timeInterval = new TimeIntervalLength;
        m_timeInterval->readFromXML( pReader );
      }
      else
      if ( pActElement->CompareTo( "generationDate" ) == 0 )
        m_generationDate.readFromXML( pReader );
      else
      if ( pActElement->CompareTo( "application" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( m_application, STR );
      }
      else
      if ( pActElement->CompareTo( "description" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( m_description, STR );
      }
      else
      if ( pActElement->CompareTo( "keyWord" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( str, STR );
        m_vKeyWords.push_back( str );
      }
      else
      if ( pActElement->CompareTo( "relatedObject" ) == 0 )
      {
        pRelatedObject = new RelatedObject;
        m_vRelatedObjects.push_back( pRelatedObject );
      }
      else
      if ( pActElement->CompareTo( "id" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( str, STR );
        pRelatedObject->setId ( str );
      }
      else
      if ( pActElement->CompareTo( "name" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( str, STR );
        pRelatedObject->setName ( str );
      }
      else
      if ( pActElement->CompareTo( "link" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( str, STR );
        pRelatedObject->setURI ( str );
      }
      else
      if ( pActElement->CompareTo( "relatedSet" ) == 0 )
      {
        pRelatedSet = new RelatedSet;
        pRelatedSet->readFromXML ( pReader );
        m_vRelatedTimeSeriesSets.push_back( pRelatedSet );
      }

    }
    else
    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      String ^ pActElement = pReader->LocalName;
      if ( pActElement->CompareTo( "RegularTimeSeriesSetProperties" ) == 0 )
        break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  RegularTimeSeriesProperties                                              //
//  Metadaten einer einzelnen RegularTimeSeries                              //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeriesProperties::RegularTimeSeriesProperties ()
{
  m_thematicDescription = "";
  m_uomSymbol           = "";
}

RegularTimeSeriesProperties::~RegularTimeSeriesProperties()
{

}

///////////////////////////////////////////////////////////////////////////////
//  RelatedObject                                                            //
//  Information über ein anderes Objekt, das mit einem RegularTimeSeriesSet  //
//  logisch verknüpft ist                                                    //
///////////////////////////////////////////////////////////////////////////////
RelatedObject::RelatedObject()
{
  m_relObjId   = "";
  m_relObjName = "";
  m_relObjURI  = "";
}

RelatedObject::~RelatedObject()
{

}

///////////////////////////////////////////////////////////////////////////////
//  RelatedSet                                                               //
//  Modellierung einer Referenz auf einen  RegularTimeSeriesSet              //
///////////////////////////////////////////////////////////////////////////////
RelatedSet::RelatedSet ()
{
  m_relatedSetId   = "";
  m_relatedSetRole = "";
  m_description    = "";
}

RelatedSet::RelatedSet ( std::string relatedSetId, std::string relatedSetRole, std::string description )
{
  m_relatedSetId   = relatedSetId;
  m_relatedSetRole = relatedSetRole;
  m_description    = description;
}

RelatedSet::~RelatedSet()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben des Objektes im TimeSeries-ADE XML-Format                      //
///////////////////////////////////////////////////////////////////////////////
void RelatedSet::writeToXML ( XmlTextWriter ^ pWriter )
{
  pWriter->WriteStartElement( "ts:RelatedSet" );
  pWriter->WriteElementString( "ts:role", QuConvert::ToString ( m_relatedSetRole )  );
    if ( m_description != "" )
  pWriter->WriteElementString( "ts:description", QuConvert::ToString ( m_description )  );
  pWriter->WriteStartElement( "ts:relatedSet" );
  String ^ STR = String::Concat( "#", QuConvert::ToString( m_relatedSetId ) );
  pWriter->WriteAttributeString( "xlink:href", STR );
  pWriter->WriteEndElement();
  pWriter->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Lesen des Objektes im TimeSeries-ADE XML-Format                          //
///////////////////////////////////////////////////////////////////////////////
void RelatedSet::readFromXML ( XmlTextReader ^ pReader )
{
  String  ^ STR;

  while ( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      String ^ pActElement = pReader->LocalName;

      if ( pActElement->CompareTo( "role" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( m_relatedSetRole, STR );
      }
      else
      if ( pActElement->CompareTo( "description" ) == 0 )
      {
        STR = pReader->ReadString();
        QuConvert::systemStr2stdStr( m_description, STR );
      }
      else
      if ( pActElement->CompareTo( "relatedSet" ) == 0 )
      {
        STR = pReader->GetAttribute( "xlink:href" );
        STR = STR->Substring( 1 );
        QuConvert::systemStr2stdStr( m_relatedSetId, STR );
      }
    }
    else
    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      String ^ pActElement = pReader->LocalName;
      if ( pActElement->CompareTo( "RelatedSet" ) == 0 )
        break;
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//  RegularTimerSeriesSets                                                   //
//  Liste von RegularTimeSeriesSet Objekten                                  //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeriesSets::RegularTimeSeriesSets()
{

}

RegularTimeSeriesSets::~RegularTimeSeriesSets()
{
  for ( unsigned int i = 0; i < m_vTimeSeriesSets.size(); i++ )
    delete m_vTimeSeriesSets[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Objekt an Listenposition index                               //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeriesSet * RegularTimeSeriesSets::getTimeSeriesSet ( unsigned int index )
{
  if ( index < 0 || index >= m_vTimeSeriesSets.size() )
    return NULL;
  else
    return m_vTimeSeriesSets[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Objekt zur Liste hinzu                                          //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSets::addTimeSeriesSet ( RegularTimeSeriesSet * pTimeSeriesSet )
{
  m_vTimeSeriesSets.push_back( pTimeSeriesSet );
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt alle in der Liste enthaltenen Objekte als GML FeatureCollection //
//  im TimeSeries ADE XML-Format                                             //
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSets::writeToXML ( String ^ fileName, String ^ energyADENamespace,
                                         String ^ timeSeriesADENamespace )
{
  char              buffer[16];
  _strdate_s ( buffer, 16 );
  String ^ date = QuConvert::ToString ( string ( buffer ) );

  System::Text::UTF8Encoding ^ utf8	    = gcnew System::Text::UTF8Encoding();
  XmlTextWriter              ^ pWriter	=  gcnew XmlTextWriter( fileName, utf8);

  pWriter->Formatting = Formatting::Indented;
  pWriter->WriteStartDocument( true );

  String ^ comment = "Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: ";
  comment = String::Concat ( comment, date );
  pWriter->WriteComment( comment );

  pWriter->WriteStartElement( "gml:FeatureCollection" );
  pWriter->WriteAttributeString( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );
  pWriter->WriteAttributeString( "xmlns:xs", "http://www.w3.org/2001/XMLSchema" );
  pWriter->WriteAttributeString( "xmlns:xlink", "http://www.w3.org/1999/xlink" );
  pWriter->WriteAttributeString( "xmlns:gml", "http://www.opengis.net/gml" );
  pWriter->WriteAttributeString( "xmlns:energy", energyADENamespace );
  pWriter->WriteAttributeString( "xmlns:ts", timeSeriesADENamespace );

  for ( unsigned int i = 0; i < m_vTimeSeriesSets.size(); ++i )
  {
    RegularTimeSeriesSet * pTimeSeriesSet = m_vTimeSeriesSets[i];
    pWriter->WriteStartElement( "gml:featureMember" );
    pTimeSeriesSet->writeToXML( pWriter );
    pWriter->WriteEndElement();
  }

  pWriter->WriteEndElement();

  pWriter->Flush();
  pWriter->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Liest von der angegebenen Datei das RegularTimeSeriesSet Objekt mit der  //
//  angegebenen gml:id                                                       //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeriesSet * RegularTimeSeriesSets::readFromXML ( std::string fileName, std::string timeSeriesSetId,
                                  UOMList * pUOMList)
{
  RegularTimeSeriesSet * pTimeSeriesSet = NULL;
  XmlTextReader ^ pReader = gcnew XmlTextReader( QuConvert::ToString( fileName ) );

  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      String ^ pActElement = pReader->LocalName;
      if ( pActElement->CompareTo ( "RegularTimeSeriesSet" ) == 0 )
      {
        String ^ STR = pReader->GetAttribute( "gml:id" );
        if ( STR != nullptr && STR->CompareTo( QuConvert::ToString( timeSeriesSetId ) ) == 0 )
        {
          pTimeSeriesSet = new RegularTimeSeriesSet ( timeSeriesSetId );
          pTimeSeriesSet->readFromXML( pReader, pUOMList );
          break;
        }
      }
    }
  }
  pReader->Close();
  return pTimeSeriesSet;
}

///////////////////////////////////////////////////////////////////////////////
//  Leert den vector m_vTimeSeriesSets, ohne die einzelnen Objekte zu löschen//
///////////////////////////////////////////////////////////////////////////////
void RegularTimeSeriesSets::clear()
{
  m_vTimeSeriesSets.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  DailyTimeSeries                                                          //
//  Tägliche Zeitreihe mit 1h Inkrement                                      //
///////////////////////////////////////////////////////////////////////////////
DailyTimeSeries::DailyTimeSeries ( std::string gmlId ): RegularTimeSeries ( gmlId )
{
  temporalExtent = new TimePeriod( );
  temporalExtent->setStartPosition ( TimePosition ( 0, 0, 0, 0, 0 ) );
  temporalExtent->setEndPosition ( TimePosition ( 0, 0, 23, 0, 0 ) );

  timeInterval = new TimeIntervalLength ( );
  timeInterval->setUnit( TimeIntervalLength::HOUR );
  timeInterval->setValue( 1 );

  m_minValue     = 0.0;
  m_maxValue     = 0.0;
  m_defaultValue = 0.0;

  m_dayType = TYPICALDAY;

  setInterpolationType ( AbstractTimeSeries::AverageInSuccedingInterval );
}

DailyTimeSeries::DailyTimeSeries ( DailyTimeSeries * pTimeSeriesOld, std::string gmlId ): RegularTimeSeries ( pTimeSeriesOld, gmlId )
{

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den übergebenen dayTyp als Text                                  //
///////////////////////////////////////////////////////////////////////////////
std::string DailyTimeSeries::getDayTypAsString ( DAY_TYP dayTyp )
{
  switch ( dayTyp )
  {
  case MONDAY:
    return "monday";

  case TUESDAY:
    return "tuesday";

  case WEDNESDAY:
    return "wednesday";

  case THURSDAY:
    return "thursday";

  case FRIDAY:
    return "friday";

  case SATURDAY:
    return "saturday";

  case SUNDAY:
    return "sunday";

  case DESIGNDAY:
    return "designDay";

  case WEEKDAY:
    return "weekDay";

  case WEEKEND:
    return "weekEnd";

  case TYPICALDAY:
    return "typicalDay";
  }
  return "";
}

string DailyTimeSeries::getDayTypAsString()
{
  return getDayTypAsString( m_dayType );
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert den übergebenen dayTyp als Enumerations-Wert                     //
///////////////////////////////////////////////////////////////////////////////
DailyTimeSeries::DAY_TYP DailyTimeSeries::getDayTypFromString ( std::string dayTyp )
 {
   if ( dayTyp == "monday" )
     return MONDAY;
   else
   if ( dayTyp == "tuesday" )
     return TUESDAY;
   else
   if ( dayTyp == "wednesday" )
     return WEDNESDAY;
   else
   if ( dayTyp == "thursday" )
     return THURSDAY;
   else
   if ( dayTyp == "friday" )
     return FRIDAY;
   else
   if ( dayTyp == "saturday" )
     return SATURDAY;
   else
   if ( dayTyp == "sunday" )
     return SUNDAY;
   else
   if ( dayTyp == "designDay" )
     return DESIGNDAY;
   else
   if ( dayTyp == "weekDay" )
     return WEEKDAY;
   else
   if ( dayTyp == "weekEnd" )
     return WEEKEND;
   else
   if ( dayTyp == "typicalDay" )
     return TYPICALDAY;

   return TYPICALDAY;

 }

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Metadaten des Objektes als Text                             //
///////////////////////////////////////////////////////////////////////////////
std::string DailyTimeSeries::getTimeSeriesMetadata()
 {
   std::string outString = "Tägliche Zeitreihe für " + getDayTypAsString() + " " + RegularTimeSeries::getTimeSeriesMetadata();

   return outString;
 }
 

///////////////////////////////////////////////////////////////////////////////
//  RegularTimeSeriesFile                                                    //
//  Reguläre Zeitreihe, deren Werte auf einer CSV-Datei gespeichert sind     //
//  Die Datei kann mehrere Zeitreihen enthalten, von denen eine ausgewählt   //
//  wird                                                                     //
///////////////////////////////////////////////////////////////////////////////
RegularTimeSeriesFile::RegularTimeSeriesFile ( std::string gmlId ): RegularTimeSeries ( gmlId )
{
  file                = "";
  numberOfHeaderLines = 0;
  fieldSeparator      = ";";
  recordSeparator     = "\n";
  decimalSymbol       = '.';
  valueColumnNumber   = 1;
  missingCSVValue     = "NaN";
}

RegularTimeSeriesFile::RegularTimeSeriesFile (  RegularTimeSeriesFile * pTimeSeriesOld, std::string gmlIdP ):RegularTimeSeries ( pTimeSeriesOld, gmlIdP )
{
  file                = pTimeSeriesOld->file;
  numberOfHeaderLines = pTimeSeriesOld->numberOfHeaderLines;
  fieldSeparator      = pTimeSeriesOld->fieldSeparator;
  recordSeparator     = pTimeSeriesOld->recordSeparator;
  decimalSymbol       = pTimeSeriesOld->decimalSymbol;
  valueColumnNumber   = pTimeSeriesOld->valueColumnNumber;
}


RegularTimeSeriesFile::~RegularTimeSeriesFile()
{

}

////////////////////////////////////////////////////////////////////////////////
//  Liefert die Metadaten des Objektes plus evtl. Fehlerinformationen als Text//
////////////////////////////////////////////////////////////////////////////////
std::string RegularTimeSeriesFile::getTimeSeriesMetadata()
{
  std::string outString = "Reguläre Zeitreihe File\r\n" + AbstractTimeSeries::getTimeSeriesMetadata() + "\r\n";

  if ( temporalExtent != NULL )
    outString = outString + temporalExtent->toString() + "\r\n";
  else
    outString = outString + "Kein Start-/Endzeitpunkt definiert\r\n";

  if ( timeInterval != NULL )
    outString = outString + "Zeitintervall " + timeInterval->toString() + "\r\n";
  else
    outString = outString + "Kein Zeitintervall definiert\r\n";

  return outString;
}

///////////////////////////////////////////////////////////////////////////////
//  IrregularTimeSeries                                                      //
//  Zeitreihe in Form einer Liste von Zeitpunkt - Wert Paaren                //
///////////////////////////////////////////////////////////////////////////////
IrregularTimeSeries::IrregularTimeSeries( std::string gmlId ) : AbstractTimeSeries ( gmlId )
{

}

IrregularTimeSeries::IrregularTimeSeries ( IrregularTimeSeries * pTimeSeriesOld, std::string gmlIdP ) : AbstractTimeSeries ( pTimeSeriesOld, gmlIdP )
{
  for ( unsigned int i = 0; i < pTimeSeriesOld->times.size(); i++  )
  {
    times.push_back( pTimeSeriesOld->times[i] );
    values.push_back( pTimeSeriesOld->values[i] );
  }
}


IrregularTimeSeries::~IrregularTimeSeries()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt alle Zeitpunkte und Werte                                          //
///////////////////////////////////////////////////////////////////////////////
bool IrregularTimeSeries::set ( std::vector<TimePosition> & timesP, std::vector<double> & valuesP )
{
  if ( timesP.size() != valuesP.size() ) return false;

  times.clear();
  values.clear();

  for ( unsigned int i = 0; i < timesP.size(); i++ )
  {
    times.push_back( timesP[i] );
    values.push_back( valuesP[i] );
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Paar Zeitpunkt-Wert hinzu                                       //
///////////////////////////////////////////////////////////////////////////////
void IrregularTimeSeries::addData (TimePosition & pos, double value )
{
  times.push_back( pos );
  values.push_back( value );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Zeitpunkt und Wert von Position index                            //
///////////////////////////////////////////////////////////////////////////////
bool IrregularTimeSeries::getData ( unsigned int index,  TimePosition & pos, double &value )
{
  if ( index < 0 || index > times.size() ) return false;

  pos   = times[index];
  value = values[index];

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Metadaten des Objektes als Text                              //
///////////////////////////////////////////////////////////////////////////////
std::string IrregularTimeSeries::getTimeSeriesMetadata()
{
  std::string outString = "Irreguläre Zeitreihe\r\n" + AbstractTimeSeries::getTimeSeriesMetadata() + "\r\n";
  return outString;
}

///////////////////////////////////////////////////////////////////////////////
//  MonthlyTimeSeries                                                        //
//  Liste mit 12 monatlichen Werten                                          //
///////////////////////////////////////////////////////////////////////////////
MonthlyTimeSeries::MonthlyTimeSeries( std::string gmlId ):
IrregularTimeSeries ( gmlId )
{
  m_year = 2022;
}

MonthlyTimeSeries::MonthlyTimeSeries( std::string gmlId, int year ):
IrregularTimeSeries ( gmlId )
{
  m_year = year;
}

MonthlyTimeSeries::MonthlyTimeSeries ( MonthlyTimeSeries * pTimeSeriesOld, std::string gmlId ):
IrregularTimeSeries ( pTimeSeriesOld , gmlId )
{
  m_year = pTimeSeriesOld->m_year;
}

MonthlyTimeSeries::~MonthlyTimeSeries()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die 12 Monatswerte                                                 //
///////////////////////////////////////////////////////////////////////////////
void MonthlyTimeSeries::setValues ( std::vector<double> &vValues )
{
  values.clear();

  for ( unsigned int i = 0; i < vValues.size(); ++i )
    values.push_back( vValues[i] );
}

///////////////////////////////////////////////////////////////////////////////
//  IrregularTimeSeriesFile                                                  //
//  Zeitreihe mit wechselnden Zeitintervallen, deren Werte auf einer         //
//  Datei gespeichert sind                                                   //
///////////////////////////////////////////////////////////////////////////////
IrregularTimeSeriesFile::IrregularTimeSeriesFile (  std::string gmlId ): IrregularTimeSeries ( gmlId )
{
  file                = "";
  numberOfHeaderLines = 0;
  fieldSeparator      = ";";
  recordSeparator     = "\n";
  decimalSymbol       = '.';
  timeColumnNumber    = 1;
  valueColumnNumber   = 2;
}

IrregularTimeSeriesFile::IrregularTimeSeriesFile ( IrregularTimeSeriesFile * pTimeSeriesOld, std::string gmlIdP ): IrregularTimeSeries ( pTimeSeriesOld, gmlIdP )
{
  file                = pTimeSeriesOld->file;
  numberOfHeaderLines = pTimeSeriesOld->numberOfHeaderLines;
  fieldSeparator      = pTimeSeriesOld->fieldSeparator;
  recordSeparator     = pTimeSeriesOld->recordSeparator;
  decimalSymbol       = pTimeSeriesOld->decimalSymbol;
  timeColumnNumber    = pTimeSeriesOld->timeColumnNumber;
  valueColumnNumber   = pTimeSeriesOld->valueColumnNumber;
}

IrregularTimeSeriesFile::~IrregularTimeSeriesFile()
{

}

std::string IrregularTimeSeriesFile::getTimeSeriesMetadata()
{
  std::string outString = "Filebasierte Irreguläre Zeitreihe\r\n" + AbstractTimeSeries::getTimeSeriesMetadata() + "\r\n";
  return outString;
}









