#include "StdAfx.h"

#include <vector>
#include <map>
#include <set>
#include <fstream>

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;

#include "SrsManagerRef.h"
#include "Feature.h"
#include "GmlSchema.h"
#include "Dictionary.h"
#include "Convert.h"
#include "Constant.h"
#include "SvgVisualisierung.h"
#include "UnitOfMeasure.h"
#include "BoreholeVisualisation2D.h"

using namespace std;
///////////////////////////////////////////////////////////////////////////////
//  class BoreholeVisualisation2D																						 //
//  2D Visualisierung eines BoreholeML Objektes Borehole mit SVG             //
///////////////////////////////////////////////////////////////////////////////
BoreholeVisualisation2D::BoreholeVisualisation2D ( Feature * pFeature, BoreholeDarstellung * pBoreholeDarstellungP, string version )
{
  pBorehole            = pFeature;
  pBoreholeDarstellung = pBoreholeDarstellungP;
  layerSeriesVersion   = version;

  pGmlSchema = pBorehole->getFeatures()->getGmlSchema();
  pCodeLists = pGmlSchema->getCodeListsDictionary();

  codelistRegistry = "http://www.infogeo.de/boreholeml/3.0/codelists/";

  extractBoreholeInformation();
}

BoreholeVisualisation2D::~BoreholeVisualisation2D()
{
  unsigned int i;

  for ( i = 0;  i < vLithologyIntervals.size(); i++ )
    delete vLithologyIntervals[i];

  for ( i = 0; i < vStratigraphyIntervals.size(); i++ )
    delete vStratigraphyIntervals[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Lithologie Intervall über den Index                          //
///////////////////////////////////////////////////////////////////////////////
LithologyInterval * BoreholeVisualisation2D::getLithologyInterval ( unsigned int index )
{
  if ( index >= 0 && index < vLithologyIntervals.size() )
    return vLithologyIntervals[index];
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Stratigraphie Intervall über den Index                       //
///////////////////////////////////////////////////////////////////////////////
StratigraphyInterval * BoreholeVisualisation2D::getStratigraphyInterval ( unsigned int index )
{
  if ( index >= 0 && index < vStratigraphyIntervals.size() )
    return vStratigraphyIntervals[index];
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Übernimmt die Informationen aus dem Borehole Feature                     //
///////////////////////////////////////////////////////////////////////////////
bool BoreholeVisualisation2D::extractBoreholeInformation()
{
  vector<Feature*> vIntervals;
  Feature        * pFeature;
  Feature        * pLayerSeries;
  string           attributWert;
  bool             success;
	size_t           i, intervalSeriesAnz;
  
  if ( pBorehole == NULL )
    return false;

  intervalSeriesAnz = pBorehole->getRelatedFeatures( "bml:intervalSeries", vIntervals );
  if ( intervalSeriesAnz == 0 )
    return false;

  if ( intervalSeriesAnz > 1 && layerSeriesVersion != "" )
  {
    pLayerSeries = NULL;
    for ( i = 0; i < intervalSeriesAnz; i++ )
    {
      pFeature = vIntervals[i];
      success = pFeature->getStringAttributWert( "bml:version", attributWert );
      if ( success )
      {
        if ( attributWert == layerSeriesVersion )
        {
          pLayerSeries = pFeature;
          break;
        }
      }
    }
    if ( pLayerSeries == NULL )
      pLayerSeries =  vIntervals[0];
  }
  else
    pLayerSeries = vIntervals[0];

  return extractIntervalSeriesInformation ( pLayerSeries );
}

///////////////////////////////////////////////////////////////////////////////
//  Übernimmt die Informationen aus dem IntervalSeries Feature               //
///////////////////////////////////////////////////////////////////////////////
bool BoreholeVisualisation2D::extractIntervalSeriesInformation( Feature * pIntervalSeries )
{
	size_t                         i, intervalAnz;
  vector<Feature*>               vIntervalsUnsorted;
  map<double,Feature*>           mIntervalsSorted;
  map<double,Feature*>::iterator iter;
  Feature                      * pInterval;
  double                         from, to;
  bool                           success, first;
  LithologyInterval            * pLithologyInterval;
  StratigraphyInterval         * pStratigraphyInterval;
  string                         chronoStratigraphy;
  string                         lithoStratigraphy;
  string                         chronoStratigraphyNew;
  string                         chronoStratigraphyText;
  string                         lithoStratigraphyNew;

  intervalAnz = pIntervalSeries->getRelatedFeatures( "bml:layer", vIntervalsUnsorted );
  if ( intervalAnz == 0 )
    return false;

  for ( i = 0; i < intervalAnz; i++ )
  {
    pInterval = vIntervalsUnsorted[i];
    success = pInterval->getDoubleAttributWert( "bml:from", from );
    if ( success )
      mIntervalsSorted[from] = pInterval;
    else
      return false;
  }
 
  pStratigraphyInterval = new StratigraphyInterval;
  vStratigraphyIntervals.push_back( pStratigraphyInterval );
  first = true;

  for ( iter = mIntervalsSorted.begin(); iter != mIntervalsSorted.end(); iter++ )
  {
    pInterval = iter->second;
    from      = iter->first;
    success = pInterval->getDoubleAttributWert( "bml:to", to );
    if ( !success )
      return false;

    pLithologyInterval = new LithologyInterval;
    pLithologyInterval->setFrom( from );
    pLithologyInterval->setTo( to );

    vLithologyIntervals.push_back( pLithologyInterval );
    success = extractLithologyInformation ( pInterval, pLithologyInterval );

    success = extractStratigraphyInformation (  pInterval, chronoStratigraphyNew, chronoStratigraphyText, lithoStratigraphyNew );

    if ( first )
    {
      pStratigraphyInterval->setFrom( from );
      pStratigraphyInterval->chronoStratigraphy     = chronoStratigraphyNew;
      pStratigraphyInterval->chronoStratigraphyText = chronoStratigraphyText;
      pStratigraphyInterval->lithoStratigraphy      = lithoStratigraphyNew;
      
      chronoStratigraphy = chronoStratigraphyNew;
      lithoStratigraphy = lithoStratigraphyNew;
      first = false;
    }
    else
    {
      if ( (chronoStratigraphy != chronoStratigraphyNew ) || (lithoStratigraphy != lithoStratigraphyNew )  )
      {
        pStratigraphyInterval->setTo( from );

        pStratigraphyInterval = new StratigraphyInterval;
        pStratigraphyInterval->setFrom( from );
        pStratigraphyInterval->chronoStratigraphy     = chronoStratigraphyNew;
        pStratigraphyInterval->chronoStratigraphyText = chronoStratigraphyText;
        pStratigraphyInterval->lithoStratigraphy      = lithoStratigraphyNew;
        vStratigraphyIntervals.push_back( pStratigraphyInterval );

        chronoStratigraphy = chronoStratigraphyNew;
        lithoStratigraphy = lithoStratigraphyNew;
      }
    }
  }
  
  pStratigraphyInterval->setTo( to );


  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Übernimmt die Lithologie-Information aos einem Intetval Feature          //
///////////////////////////////////////////////////////////////////////////////
bool BoreholeVisualisation2D::extractLithologyInformation ( Feature * pInterval, LithologyInterval * pLithologyInterval )
{
  bool             success;
  string           attributWert;
  vector<Feature*> vLithology;
  vector<Feature*> vFeatures;
  Feature        * pFeature;
  Lithology      * pLithology;
	size_t           i, anz;
  double           wertUnten, wertOben, wert;

  success = pInterval->getStringAttributWert( "bml:rockCode", attributWert );
  if ( success ) pLithologyInterval->rockCode = attributWert;

  anz = pInterval->getRelatedFeatures( "bml:rockNameText", vFeatures );
  if ( anz > 0 )
  {
    pFeature = vFeatures[0];
    if ( pFeature->getSimpleContent() != NULL )
      pLithologyInterval->rockNameText = pFeature->getSimpleContent()->value;
  }

  success = pInterval->getStringAttributWert( "bml:intervalType", attributWert );
  if ( success )
  {
    attributWert = codeToText ( "IntervalTypeList.xml", attributWert );
    pLithologyInterval->intervalType = attributWert;
  }

  success = pInterval->getStringAttributWert( "bml:genesis", attributWert );
  if ( success )
  {
    attributWert = codeToText ( "GenesisList.xml", attributWert );
    pLithologyInterval->genesis = attributWert;
  }

  success = pInterval->getStringAttributWert( "bml:geoGenesis", attributWert );
  if ( success )
  {
    attributWert = codeToText ( "GeoGenesisList.xml", attributWert );
    pLithologyInterval->geoGenesis = attributWert;
  }

  success = pInterval->getStringAttributWert(  "bml:carbonatContent", attributWert );
  if ( success )
  {
    attributWert = codeToText ( "CarbonatContentList.xml", attributWert );
    pLithologyInterval->carbonatContent = attributWert;
  }

  success = pInterval->getStringAttributWert( "bml:waterContent", attributWert );
  if ( success )
  {
    attributWert = codeToText ( "WaterContentList.xml", attributWert );
    pLithologyInterval->waterContent = attributWert;
  }

  success = pInterval->getStringAttributWert( "bml:consistency", attributWert );
  if ( success )
  {
    attributWert = codeToText ( "ConsistencyList.xml", attributWert );
    pLithologyInterval->consistency = attributWert;
  }

  success = pInterval->getStringAttributWert( "bml:descriptiveText", attributWert );
  if ( success )
  {
    pLithologyInterval->descriptiveText = attributWert;
  }

  anz = pInterval->getRelatedFeatures( "bml:lithology", vLithology );
  if ( anz == 0 )
    return false;

  for ( i = 0; i < anz; i++ )
  {
    pFeature = vLithology[i];
    pLithology = new Lithology;
    pLithologyInterval->addLithology( pLithology );

    success = pFeature->getStringAttributWert( "bml:rockName", attributWert );
    if ( success )
    {
      pLithology->rockCode = attributWert;
      attributWert = codeToText ( "RockNameList.xml", attributWert );
      pLithology->rockText = attributWert;
    }

    success = pFeature->getStringAttributWert( "bml:rockColor", attributWert );
    if ( success )
    {
      attributWert = codeToText ( "RockColorList.xml", attributWert );
      pLithology->rockColor = attributWert;
    }

    success = pFeature->getStringAttributWert( "bml:rockColorMixture", attributWert );
    if ( success )
    {
      attributWert = codeToText ( "RockColorMixtureList.xml", attributWert );
      pLithology->rockColorMixture = attributWert;
    }

    success = pFeature->getDoubleAttributWert( "bml:percentage", wert );
    if ( success )
      pLithology->setPercentage ( wert );

    success = pFeature->getDoubleAttributWert( "bml:percentageFrom", wertUnten  );
    if ( success )
    {
      success =  pFeature->getDoubleAttributWert( "bml:percentageTo", wertOben  );
      if ( success )
        pLithology->setPercentageFromTo( wertUnten, wertOben );
    }
  }
  return true;

}

///////////////////////////////////////////////////////////////////////////////
//  Übernimmt die Stratigraphie-Information aos einem Interval Feature       //
///////////////////////////////////////////////////////////////////////////////
bool BoreholeVisualisation2D::extractStratigraphyInformation ( Feature * pInterval, std::string & chronoStratigraphy, std::string & chronoStratigraphyText, 
                                                              std::string& lithoStratigraphy )
{
  bool             success;
  string           attributWert;
  vector<Feature*> vStratigraphy;
  vector<Feature*> vFeatures;
  Feature        * pFeature;
	size_t           anz;

  anz = pInterval->getRelatedFeatures( "bml:stratigraphy", vStratigraphy );
  if ( anz != 1 )
    return false;

  pFeature = vStratigraphy[0];

  success = pFeature->getStringAttributWert( "bml:chronoStratigraphy", attributWert );
  if ( success )
  {
    chronoStratigraphy = attributWert;
    chronoStratigraphyText = codeToText ( "ChronoStratigraphyList.xml", attributWert );
  }
  else
    chronoStratigraphy = "";

  anz =  pFeature->getRelatedFeatures( "bml:lithoStratigraphy", vFeatures );
  if ( anz > 0 && vFeatures[0]->getSimpleContent() != NULL )
    lithoStratigraphy = vFeatures[0]->getSimpleContent()->value;
  else
    lithoStratigraphy = "";

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Wandelt einen "code", der zu einer Codelist mit Name "codeListName"      //
//  gehört, in einen lesbaren Text um																				 //
///////////////////////////////////////////////////////////////////////////////
string BoreholeVisualisation2D::codeToText ( string codeListName, string code )
{
  string codeText = code;
  bool   success = false;
  string codelistPath = codelistRegistry +   codeListName;

  GmlDictionaryEntry * pDictionaryEntry = pCodeLists->getDictionaryEntry ( codelistPath );

  if ( pDictionaryEntry != NULL )
    success = pDictionaryEntry->getSchluesselText ( code, codeText );

  return codeText;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die benutzten Gesteins-Codes																		 //
///////////////////////////////////////////////////////////////////////////////
size_t BoreholeVisualisation2D::getRockCodesUsed ( set<string> & sRockCodes )
{
  unsigned int i, j;

  for ( i = 0; i < vLithologyIntervals.size(); i++ )
  {
    LithologyInterval * pInterval = vLithologyIntervals[i];
    for ( j = 0; j < pInterval->getLithologyAnz(); j++ )
    {
      Lithology * pLithology = pInterval->getLithology( j );
      sRockCodes.insert ( pLithology->rockCode );
    }
  }

  return sRockCodes.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die benutzten Stratigraphie-Codes																 //
///////////////////////////////////////////////////////////////////////////////
size_t BoreholeVisualisation2D::getStratigraphyCodesUsed ( set<string> & sStratigraphyCodes )
{
	size_t i;

  for ( i = 0; i < vStratigraphyIntervals.size(); i++ )
  {
    StratigraphyInterval * pInterval = vStratigraphyIntervals[i];
    sStratigraphyCodes.insert ( pInterval->chronoStratigraphy );
  }

  return sStratigraphyCodes.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die SVG-Visualisierung eines Bohrlochs												 //
///////////////////////////////////////////////////////////////////////////////
void BoreholeVisualisation2D::show()
{
  BoreholeSvgWriter ^ pWriter = gcnew BoreholeSvgWriter ( this );

  string outputFile = Constant::getSystemFolder() + "Ueberschrift.svg";
  pWriter->writeUeberschrift( outputFile );

  outputFile = Constant::getSystemFolder() + "borehole.svg";
  pWriter->write( outputFile );
}

///////////////////////////////////////////////////////////////////////////////
//  class Interval																													 //
//  Modelliert ein Intervall einer Borehole IntervalSeries                   //
///////////////////////////////////////////////////////////////////////////////
Interval::Interval ()
{
  from = 0.0;
  to   = 0.0;
}

Interval::~Interval()
{

}

///////////////////////////////////////////////////////////////////////////////
//  class LithologyInterval																									 //
//  Modelliert ein Lithologie-Intervall                                      //
///////////////////////////////////////////////////////////////////////////////
LithologyInterval::LithologyInterval (): Interval()
{
  rockCode        = "";
  rockNameText    = "";
  intervalType    = "";
  genesis         = "";
  geoGenesis      = "";
  carbonatContent = "";
  waterContent    = "";
  consistency     = "";
  descriptiveText = "";

}
LithologyInterval::~LithologyInterval()
{
  unsigned int i;

  for ( i = 0; i < vLithology.size(); i++ )
    delete vLithology[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Lithologie-Schicht hinzu																			 //
///////////////////////////////////////////////////////////////////////////////
void LithologyInterval::addLithology ( Lithology * pLithology )
{
  vLithology.push_back( pLithology );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Lithologie-Schicht über den index													 //
///////////////////////////////////////////////////////////////////////////////
Lithology * LithologyInterval::getLithology ( unsigned int index )
{
  if ( index >= 0 && index < vLithology.size() )
    return vLithology[index];
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  class Lithology																													 //
//  Modelliert die BoreholeML Klasse Lithology                               //
///////////////////////////////////////////////////////////////////////////////
Lithology::Lithology()
{
  rockCode          = "";
  rockColor         = "";
  rockColorMixture  = "";

  percentageFrom    = 0.0;
  percentageTo      = 0.0;
}

Lithology::~Lithology()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Parameter "percentageFrom" und "percentageTo" auf denselben    //
//  Wert																																		 //
///////////////////////////////////////////////////////////////////////////////
void Lithology::setPercentage ( double wert )
{
  percentageFrom = wert;
  percentageTo   = wert;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Parameter "percentageFrom" und "percentageTo"									 //
///////////////////////////////////////////////////////////////////////////////
void Lithology::setPercentageFromTo ( double wertUnten, double wertOben )
{
  percentageFrom = wertUnten;
  percentageTo   = wertOben;
}

///////////////////////////////////////////////////////////////////////////////
//  class StratigraphyInterval																							 //
//  Modelliert ein Stratigraphie-Intervall                                   //
///////////////////////////////////////////////////////////////////////////////
StratigraphyInterval::StratigraphyInterval()
{
  chronoStratigraphy = "";
  lithoStratigraphy  = "";
}

StratigraphyInterval:: ~StratigraphyInterval()
{

}

///////////////////////////////////////////////////////////////////////////////
//  class SchichtDarstParameter																							 //
//  Darstellungsparameter eines Intervalles                                  //
///////////////////////////////////////////////////////////////////////////////
SchichtDarstParameter::SchichtDarstParameter()
{
  pColor        = NULL;
  pPattern      = NULL;
  pHatching     = NULL;
  signaturePath =  "";
}

SchichtDarstParameter::~SchichtDarstParameter()
{
  if ( pColor != NULL )
    delete pColor;

  if ( pPattern != NULL )
    delete pPattern;

  if ( pHatching != NULL )
    delete pHatching;
}

///////////////////////////////////////////////////////////////////////////////
//  class BoreholeDarstellung																								 //
//  Parameter für 2D-Bohrlochdarstellung                                     //
///////////////////////////////////////////////////////////////////////////////
BoreholeDarstellung::BoreholeDarstellung()
{
  beschreibung        = "";
  signatureFolder     = "Bitmaps";

  boreholeDurchmesserLithologie    = 1.2;
  boreholeDurchmesserStratigraphie = 1.2;
  abstandSaeulen                   = 6.0;
  abstandSkala                     = 6.0;
  vertikaleSkalierung              = 0.1;
  szenenrand                       = 1.0;
  abstandSkalenwerte               = 10.0;
  breiteUeberschrift               = 1500.0;
  hoeheUeberschrift                = 30.0;

  showLithology          = true;
  showStratigraphy       = true;
  showSkala              = true;
  showBoreholeAttribute  = true;

  lithologyVisualisationStrategy = MAJOR_COMPONENT;
}

BoreholeDarstellung::~BoreholeDarstellung()
{
  std::map<std::string,SchichtDarstParameter*>::iterator iterSchichtDarst;

  for ( iterSchichtDarst = mLithologyDarstellungMap.begin(); iterSchichtDarst != mLithologyDarstellungMap.end(); iterSchichtDarst++ )
    delete iterSchichtDarst->second;

  for ( iterSchichtDarst = mStratigraphyDarstMap.begin(); iterSchichtDarst != mStratigraphyDarstMap.end(); iterSchichtDarst++ )
    delete iterSchichtDarst->second;
}


///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Lithologie Schichtdarstellung hinzu														 //
///////////////////////////////////////////////////////////////////////////////
void BoreholeDarstellung::addLithologyDarstellung ( std::string rockName, SchichtDarstParameter * pSchichtDarstellung )
{
  if ( mLithologyDarstellungMap.find( rockName ) != mLithologyDarstellungMap.end() )
    delete mLithologyDarstellungMap[rockName];

  mLithologyDarstellungMap[rockName] = pSchichtDarstellung;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Lithologie Schichtdarstellung über den Gesteinsnamen				 //
///////////////////////////////////////////////////////////////////////////////
SchichtDarstParameter * BoreholeDarstellung::getLithologyDarstellung ( std::string rockName )
{
  if ( mLithologyDarstellungMap.find( rockName ) != mLithologyDarstellungMap.end() )
    return mLithologyDarstellungMap[rockName];
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Lithologie-Farbe über den Gesteinsnamen											 //
///////////////////////////////////////////////////////////////////////////////
SvgColor * BoreholeDarstellung::getLithologyColor ( std::string rockName )
{
  if ( mLithologyDarstellungMap.find( rockName ) != mLithologyDarstellungMap.end() )
    return mLithologyDarstellungMap[rockName]->getColor();
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Stratigraphie Schichtdarstellung hinzu												 //
///////////////////////////////////////////////////////////////////////////////
void BoreholeDarstellung::addStratigraphyDarstellung  ( std::string chronoStratigraphy, SchichtDarstParameter * pSchichtDarstellung )
{
  if ( mStratigraphyDarstMap.find( chronoStratigraphy ) != mStratigraphyDarstMap.end() )
    delete mStratigraphyDarstMap[chronoStratigraphy];

  mStratigraphyDarstMap[chronoStratigraphy] = pSchichtDarstellung;

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Stratigraphie Schichtdarstellung über den Namen    				 //
///////////////////////////////////////////////////////////////////////////////
SchichtDarstParameter * BoreholeDarstellung::getStratigraphyDarstellung ( std::string chronoStratigraphy )
{
  if ( mStratigraphyDarstMap.find( chronoStratigraphy ) != mStratigraphyDarstMap.end() )
    return mStratigraphyDarstMap[chronoStratigraphy];
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Stratigraphie-Farbe über den Namen											     //
///////////////////////////////////////////////////////////////////////////////
SvgColor * BoreholeDarstellung::getStratigraphyColor ( std::string chronoStratigraphy )
{
  if ( mStratigraphyDarstMap.find( chronoStratigraphy ) != mStratigraphyDarstMap.end() )
    return mStratigraphyDarstMap[chronoStratigraphy]->getColor();
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Ausdehnung der Scene im SVG-Viewport                       //
///////////////////////////////////////////////////////////////////////////////
void BoreholeDarstellung::getSceneViewbox ( double totalLength, double viewbox[] )
{
  double breiteSzene;
  
  if ( !showLithology  )
  {
    if ( showSkala )
      breiteSzene =  szenenrand + abstandSkala + 0.5*boreholeDurchmesserLithologie;
    else
      breiteSzene = szenenrand + 0.5*boreholeDurchmesserLithologie;
  }
  else
  if ( !showLithology || !showStratigraphy )
  {
    if ( showSkala )
      breiteSzene =  szenenrand + abstandSkala + 0.5*boreholeDurchmesserStratigraphie;
    else
      breiteSzene = szenenrand + 0.5*boreholeDurchmesserStratigraphie;
  }
  else
  {
    if ( showSkala )
      breiteSzene = szenenrand + abstandSkala + 0.5*abstandSaeulen + 0.5*(boreholeDurchmesserLithologie + boreholeDurchmesserStratigraphie);
    else
      breiteSzene = szenenrand + 0.5*abstandSaeulen + 0.5*(boreholeDurchmesserLithologie + boreholeDurchmesserStratigraphie);
  }

  viewbox[0] = -breiteSzene;
  viewbox[1] = -szenenrand;
  viewbox[2] = 2.0*breiteSzene;
  viewbox[3] = totalLength*vertikaleSkalierung + 2.0*szenenrand;
}

///////////////////////////////////////////////////////////////////////////////
//  class BoreholeDarstellungReader																					 //
//  Einlesen der Parameter zur 2D-Bohrlochdarstellung                        //
///////////////////////////////////////////////////////////////////////////////
BoreholeDarstellungReader::BoreholeDarstellungReader()
{

}

BoreholeDarstellungReader::~BoreholeDarstellungReader()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Darstellungsparameter																			 //
///////////////////////////////////////////////////////////////////////////////
bool BoreholeDarstellungReader::read ( std::string fileName, BoreholeDarstellung * pBoreholeDarstellung )
{
  String                ^ fName;
  String                ^ STR;
  string                  stdString;
  bool                    first = true;
  SvgColor              * pColor;
  Pattern               * pPattern;
  Hatching              * pHatching;
  cli::array<String^>   ^ rgb;
  SchichtDarstParameter * pSchichtDarstParameter;

  fName  = QuConvert::ToString ( fileName );
  reader = gcnew XmlTextReader(fName);

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;
      if ( first )
      {
        first = false;
        if ( pActElement->CompareTo("BoreholeDarstellung") != 0 )
        {
         
          System::Windows::Forms::MessageBox::Show( "Ungültige Steuerdatei für SVG-Visualisierung");
          break;
        }
      }

      if ( pActElement->CompareTo( "beschreibung" ) == 0 )
      {
        STR =  reader->ReadString();
        if (STR != nullptr )
        {
          QuConvert::systemStr2stdStr( stdString, STR );
          pBoreholeDarstellung->setBeschreibung ( stdString );
        }
      }
      else
      if ( pActElement->CompareTo( "boreholeDurchmesserLithologie" ) == 0 )
      {
        STR =  reader->ReadString();
        if (STR != nullptr )
        {
          double h = XmlConvert::ToDouble( STR );
          pBoreholeDarstellung->setBoreholeDurchmesserLithologie( h );
        }
      }
      else
      if ( pActElement->CompareTo( "boreholeDurchmesserStratigraphie" ) == 0 )
      {
        STR =  reader->ReadString();
        if (STR != nullptr )
        {
          double h = XmlConvert::ToDouble( STR );
          pBoreholeDarstellung->setBoreholeDurchmesserStratigraphie ( h );
        }
      }
      else
      if ( pActElement->CompareTo( "LithologyDarstellung" ) == 0 )
      {
        string rockCode = "";
        pSchichtDarstParameter = new SchichtDarstParameter;       

        STR = reader->GetAttribute ( "rockCode" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr( rockCode, STR );
          pBoreholeDarstellung->addLithologyDarstellung( rockCode, pSchichtDarstParameter );
        }

        STR = reader->GetAttribute( "color" );
        if ( STR != nullptr )
        {
          rgb = STR->Split ( ',' );
          pColor = new SvgColor ( XmlConvert::ToInt16( rgb[0] ),  XmlConvert::ToInt16( rgb[1] ),  XmlConvert::ToInt16( rgb[2] ) );
          pSchichtDarstParameter->setColor( pColor );
        }                
      }
      else
      if ( pActElement->CompareTo( "Signature" ) == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pSchichtDarstParameter->setSignaturePath( stdString );
        }
      }
      else
      if ( pActElement->CompareTo( "StratigraphyDarstellung" ) == 0 )
      {
        string chronoStratigraphy = "";
        pSchichtDarstParameter = new SchichtDarstParameter;       

        STR = reader->GetAttribute ( "chronoStratigraphyCode" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr( chronoStratigraphy, STR );
          pBoreholeDarstellung->addStratigraphyDarstellung( chronoStratigraphy, pSchichtDarstParameter );
        }

        STR = reader->GetAttribute( "color" );
        if ( STR != nullptr )
        {
          rgb = STR->Split ( ',' );
          pColor = new SvgColor ( XmlConvert::ToInt16( rgb[0] ),  XmlConvert::ToInt16( rgb[1] ),  XmlConvert::ToInt16( rgb[2] ) );
          pSchichtDarstParameter->setColor( pColor );
        }                
      }
      else
      if ( pActElement->CompareTo( "Pattern" ) == 0 )
      {
        pPattern = readPattern();
        if ( pPattern != NULL )
          pSchichtDarstParameter->setPattern( pPattern );
      }
      else
      if ( pActElement->CompareTo( "Pattern" ) == 0 )
      {
        pHatching = readHatching();
        if ( pHatching != NULL )
          pSchichtDarstParameter->setHatching( pHatching );
      }
    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = reader->LocalName;
      if ( pActElement->CompareTo( "BoreholeDarstellung" ) == 0 )
        break;
    }
  }
  reader->Close();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Pattern														                             //
///////////////////////////////////////////////////////////////////////////////
Pattern * BoreholeDarstellungReader::readPattern()
{
  String ^ STR;
  string   stdString;
  double   x, y, width, height;

  STR = reader->GetAttribute ( "name" );
  QuConvert::systemStr2stdStr ( stdString, STR );

  Pattern * pPattern = new Pattern ( stdString );

  STR = reader->GetAttribute ( "x" );
  x = XmlConvert::ToDouble ( STR );
  STR = reader->GetAttribute ( "y" );
  y = XmlConvert::ToDouble ( STR );
  STR = reader->GetAttribute ( "width" );
  width = XmlConvert::ToDouble ( STR );
  STR = reader->GetAttribute ( "height" );
  height = XmlConvert::ToDouble ( STR );

  pPattern->setGeometry ( x, y, width, height );

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("Path") == 0 )
      {
        string pathString;

        STR = reader->GetAttribute ( "path" );
        QuConvert::systemStr2stdStr( pathString, STR );

        SvgPath * pSvgPath = new SvgPath ( "" );
        pSvgPath->setPathString ( pathString );
        readGeometryParams( pSvgPath, "Path" );

        pPattern->addGeometryElement ( pSvgPath );
      }
      else
      if ( pActElement->CompareTo("Circle") == 0 )
      {
        double cx, cy, r;

        STR = reader->GetAttribute ( "cx" );
        cx = XmlConvert::ToDouble ( STR );
        STR = reader->GetAttribute ( "cy" );
        cy = XmlConvert::ToDouble ( STR );
        STR = reader->GetAttribute ( "r" );
        r = XmlConvert::ToDouble ( STR );

        SvgCircle * pSvgCircle = new SvgCircle ( "" );
        pSvgCircle->setGeometry ( cx, cy, r );
        readGeometryParams( pSvgCircle, "Circle" );

        pPattern->addGeometryElement ( pSvgCircle );
      }
      else
      if ( pActElement->CompareTo("Rect") == 0 )
      {
        STR = reader->GetAttribute ( "x" );
        if ( STR != nullptr )	x = XmlConvert::ToDouble ( STR );
        else				x = 0.0;

        STR = reader->GetAttribute ( "y" );
        if ( STR != nullptr ) y = XmlConvert::ToDouble ( STR );
        else			   y = 0.0;

        STR = reader->GetAttribute ( "width" );
        width = XmlConvert::ToDouble ( STR );
        STR = reader->GetAttribute ( "height" );
        height = XmlConvert::ToDouble ( STR );

        SvgRect * pSvgRect = new SvgRect ( "" );
        pSvgRect->setGeometry ( x, y, width, height );
        readGeometryParams( pSvgRect, "Rect" );

        pPattern->addGeometryElement ( pSvgRect );
      }
      else
      if ( pActElement->CompareTo("Text") == 0 )
      {
        string textString;
        double size;

        STR = reader->GetAttribute ( "text" );
        QuConvert::systemStr2stdStr( textString, STR );
        STR = reader->GetAttribute ( "x" );
        x = XmlConvert::ToDouble ( STR );
        STR = reader->GetAttribute ( "y" );
        y = XmlConvert::ToDouble ( STR );
        STR = reader->GetAttribute ( "size" );
        size = XmlConvert::ToDouble ( STR );

        SvgText * pSvgText = new SvgText ( "" );
        pSvgText->setPosition ( x, y );
        pSvgText->setTextSize ( size );
        pSvgText->setTextString ( textString );

        pPattern->addGeometryElement ( pSvgText );
      }
    }
    else
    {
      if ( reader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if (pActElement->CompareTo("Pattern") == 0 ) break;
      }
    }
  }
  return pPattern;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Hatching												                            		 //
///////////////////////////////////////////////////////////////////////////////
Hatching * BoreholeDarstellungReader::readHatching()
{
  String ^   STR;
  string     stdString;
  double     h;

  STR = reader->GetAttribute ( "name" );
  QuConvert::systemStr2stdStr ( stdString, STR );

  Hatching * pHatching = new Hatching ( stdString );

  while( reader->Read() )
  {
    pActElement = reader->LocalName;

    if ( reader->NodeType == XmlNodeType::Element )
    {
      if ( pActElement->CompareTo ( "stroke-width" ) == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          h = XmlConvert::ToDouble ( STR );
          pHatching->setStrokeWidth ( h );
        }
      }
      else
      if ( pActElement->CompareTo ( "line-dist" ) == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          h = XmlConvert::ToDouble ( STR );
          pHatching->setLineDist ( h );
        }
      }     
      else
      if ( pActElement->CompareTo("HatchingType") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pHatching->setHatchingType ( stdString );
        }
      }
    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      if ( pActElement->CompareTo("Hatching") == 0 ) break;
    }
  }
  return pHatching;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen von Darstellungsparametern für SvgPath, SvgCircle und SvgRect   //
///////////////////////////////////////////////////////////////////////////////
void BoreholeDarstellungReader::readGeometryParams ( SvgGeometry * pSvgGeometry, String ^endTag )
{
  String       ^ STR;
  string         stdString;
  double         h;
  vector<double> stroke_dasharray;

  if ( reader->IsEmptyElement == true ) return;

  while( reader->Read() )
  {
    pActElement = reader->LocalName;

    if ( reader->NodeType == XmlNodeType::Element )
    {
      if ( pActElement->CompareTo ( "fill" ) == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          if ( STR->CompareTo( "true") == 0 )
            pSvgGeometry->setFill ( true );
          else
            pSvgGeometry->setFill ( false );
        }
      }
      else
      if ( pActElement->CompareTo ( "Stroke-width" ) == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          h = XmlConvert::ToDouble ( STR );
          pSvgGeometry->setStrokeWidth ( h );
        }
      }
      else
      if ( pActElement->CompareTo("Stroke-dasharray") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          h = XmlConvert::ToDouble ( STR );
          stroke_dasharray.push_back ( h );
        }
      }
            
    }
    else
    {
      if ( reader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if (pActElement->CompareTo( endTag ) == 0 ) break;
      }
    }
  }

  if ( stroke_dasharray.size() > 0 )
  {
    STR = nullptr;
    for ( unsigned int i = 0; i < stroke_dasharray.size(); i++ )
      STR = String::Concat ( STR, XmlConvert::ToString ( stroke_dasharray[i]), " " );
    QuConvert::systemStr2stdStr ( stdString, STR );
    pSvgGeometry->setStrokeDasharray ( stdString );
  }

}

///////////////////////////////////////////////////////////////////////////////
//  class BoreholeSvgWriter																								   //
//  Generierung der SVG-Datei zur 2D-Visualisierung eines Bohrlochs          //
///////////////////////////////////////////////////////////////////////////////
BoreholeSvgWriter::BoreholeSvgWriter ( BoreholeVisualisation2D * pBoreholeVisualisationP )
{
  pBoreholeVisualisation = pBoreholeVisualisationP;
  pBoreholeDarstellung   = pBoreholeVisualisation->getBoreholeDarstellung();
  pBorehole              = pBoreholeVisualisation->getBorehole();

  lithologyAchse    = 0.0;
  stratigraphyAchse = 0.0;
  skalenAchse       = 0.0;
}

BoreholeSvgWriter::~BoreholeSvgWriter ()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Generiert SVG-Darstellung des Borehole                                   //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::write ( std::string fileName )
{
  unsigned int i;

  System::Text::UTF8Encoding ^ utf8	= gcnew System::Text::UTF8Encoding();
  writer = gcnew XmlTextWriter(  QuConvert::ToString ( fileName ), utf8);
  writer->Formatting = Formatting::Indented;

  writeSvgRoot();

  writeSvgStyles();

  if (  pBoreholeDarstellung->getShowLithology() )
  {
    set<string>           sRockCodes;
    set<string>::iterator iter;

    pBoreholeVisualisation->getRockCodesUsed( sRockCodes );
    for ( iter = sRockCodes.begin(); iter != sRockCodes.end(); iter++ )
    {
      string rockCode = *iter;
      SchichtDarstParameter * pParam = pBoreholeDarstellung->getLithologyDarstellung( rockCode );
      if ( pParam != NULL )
      {
        if ( pParam->getPattern() != NULL )
          writeSvgPattern( pParam->getColor(), pParam->getPattern() );

        if ( pParam->getSignaturePath() !="" )
          writeSvgImagePattern  ( pParam->getColor(), pParam->getSignaturePath(), pBoreholeDarstellung->getBoreholeDurchmesserLithologie() );

        if ( pParam->getHatching() != NULL )
          writeSvgHatching( pParam->getColor(), pParam->getHatching() );
      }
    }
  }

  if (  pBoreholeDarstellung->getShowStratigraphy() )
  {
    set<string>           sStratigraphyCodes;
    set<string>::iterator iter;

    pBoreholeVisualisation->getStratigraphyCodesUsed( sStratigraphyCodes );
    for ( iter = sStratigraphyCodes.begin(); iter != sStratigraphyCodes.end(); iter++ )
    {
      string stratigraphyCode = *iter;
      SchichtDarstParameter * pParam = pBoreholeDarstellung->getStratigraphyDarstellung( stratigraphyCode );
      if ( pParam != NULL )
      {
        if ( pParam->getPattern() != NULL )
          writeSvgPattern( pParam->getColor(), pParam->getPattern() );

        if ( pParam->getHatching() != NULL )
          writeSvgHatching( pParam->getColor(), pParam->getHatching() );
      }
    }
  }

  if ( pBoreholeDarstellung->getShowLithology() &&  pBoreholeDarstellung->getShowStratigraphy() )
  {
    lithologyAchse    = -0.5*( pBoreholeDarstellung->getBoreholeDurchmesserLithologie()    + pBoreholeDarstellung->getAbstandSaeulen() );
    stratigraphyAchse =  0.5*( pBoreholeDarstellung->getBoreholeDurchmesserStratigraphie() + pBoreholeDarstellung->getAbstandSaeulen() );
  }

  skalenAchse = lithologyAchse - pBoreholeDarstellung->getAbstandSkala();

  if (  pBoreholeDarstellung->getShowLithology() )
  {
    for ( i = 0; i < pBoreholeVisualisation->getLithologyIntervalAnz(); i++ )
    {
      LithologyInterval * pInterval = pBoreholeVisualisation->getLithologyInterval( i );
      writeLithologyInterval( pInterval );
    }
  }

  if (  pBoreholeDarstellung->getShowStratigraphy() )
  {
    for ( i = 0; i < pBoreholeVisualisation->getStratigraphyIntervalAnz(); i++ )
    {
      StratigraphyInterval * pInterval = pBoreholeVisualisation->getStratigraphyInterval( i );
      writeStratigraphyInterval( pInterval );
    }
  }

  if ( pBoreholeDarstellung->getShowSkala() )
    writeSkala();

  writer->WriteEndElement();
  writer->Flush();
  writer->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Überschrift der interaktiven Karte			            				 //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeUeberschrift ( std::string fileName )
{
  String	                   ^ STR;
  String	                   ^ STR_X;
  String	                   ^ STR_Y;
  String	                   ^ STR_X0;
  String	                   ^ STR_Y0;
  string                       stdString;
  int                          deltaX, deltaY;
	size_t                       anz;
	bool                         success;
  string                       ueberschrift;
  string                       boreholeId;
  string                       fullName;
  vector<Feature*>             vFeatures;
  System::Text::UTF8Encoding ^ utf8	= gcnew System::Text::UTF8Encoding();

  ueberschrift = "";

  success = pBorehole->getStringAttributWert( "bml:id", boreholeId );
  if ( !success ) boreholeId = "";

  anz = pBorehole->getRelatedFeatures( "bml:fullName", vFeatures );
  if ( anz > 0 )
  {
    if ( vFeatures[0]->getSimpleContent() != NULL )
      fullName =  vFeatures[0]->getSimpleContent()->value;
    else
      fullName = "";
  }
  else
    fullName = "";

  ueberschrift = fullName;
  if ( boreholeId != "" )
    ueberschrift = ueberschrift + " (" + boreholeId + ")";

  STR    = QuConvert::ToString ( fileName );
  writer = gcnew XmlTextWriter( STR, utf8);

  writer->Formatting = Formatting::Indented;
  writer->WriteStartDocument( true );
  writer->WriteComment("Generated by: KIT GML-Toolbox");

  writer->WriteStartElement("svg ");

  writer->WriteAttributeString("xmlns", "http://www.w3.org/2000/svg");
  writer->WriteAttributeString("xmlns:xlink", "http://www.w3.org/1999/xlink");
  writer->WriteAttributeString("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  writer->WriteAttributeString ( "zoomAndPan", "disable" );

  deltaX = int ( pBoreholeDarstellung->getBreiteUeberschrift() );
  deltaY = int ( pBoreholeDarstellung->getHoeheUeberschrift() );

  STR = XmlConvert::ToString ( deltaX );
  writer->WriteAttributeString("width", STR );
  STR = XmlConvert::ToString ( deltaY );
  writer->WriteAttributeString("height", STR );

  STR_X0 = XmlConvert::ToString ( 0 );
  STR_Y0 = XmlConvert::ToString ( 0 );
  STR_X = XmlConvert::ToString ( deltaX );
  STR_Y = XmlConvert::ToString ( deltaY );
  STR = System::String::Concat( STR_X0, " ", STR_Y0, " " );
  STR = System::String::Concat( STR, STR_X, " ", STR_Y );
  writer->WriteAttributeString ( "viewBox", STR );

  if ( ueberschrift != "" )
  {
    writer->WriteStartElement ( "text" );
    writer->WriteAttributeString ( "text-anchor", "start" );
    writer->WriteAttributeString ( "x", XmlConvert::ToString (0.0 ) );
    writer->WriteAttributeString ( "y", XmlConvert::ToString ( deltaY ) );
    writer->WriteAttributeString ( "font-size", XmlConvert::ToString ( deltaY ) );
    writer->WriteAttributeString ( "font-family", "Arial" );
    STR = QuConvert::ToString ( ueberschrift );
    writer->WriteString ( STR );

    writer->WriteEndElement();
  }

  writer->WriteEndElement();

  writer->Flush();
  writer->Close();
}


///////////////////////////////////////////////////////////////////////////////
// Schreibt den SVG-Header                                                   //
///////////////////////////////////////////////////////////////////////////////
bool BoreholeSvgWriter::writeSvgRoot ()
{
  String		^ STR;
  String		^ STR_X;
  String		^ STR_Y;
  String		^ STR_X0;
  String		^ STR_Y0;
  bool        success ;
  double      totalLength;
  double      viewbox[4];

  success = pBorehole->getDoubleAttributWert( "bml:totalLength", totalLength );
  if ( !success ) 
    return false;

  pBoreholeDarstellung->getSceneViewbox( totalLength, viewbox );

  writer->WriteStartDocument( true );
  writer->WriteComment("Generated by: KIT GML-Toolbox");

  writer->WriteStartElement("svg ");
  writer->WriteAttributeString("xmlns", "http://www.w3.org/2000/svg");
  writer->WriteAttributeString("xmlns:xlink", "http://www.w3.org/1999/xlink");
  writer->WriteAttributeString("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");

  writer->WriteAttributeString("width", "100%" );
  writer->WriteAttributeString("height", "100%" );

  STR_X0 = XmlConvert::ToString ( viewbox[0] );
  STR_Y0 = XmlConvert::ToString ( viewbox[1] );
  STR_X = XmlConvert::ToString ( viewbox[2] );
  STR_Y = XmlConvert::ToString ( viewbox[3] );
  STR = System::String::Concat( STR_X0, " ", STR_Y0, " ", STR_X, " ", STR_Y );
  writer->WriteAttributeString ( "viewBox", STR  );

  writer->WriteAttributeString ( "id", "borehole" );
  writer->WriteAttributeString ( "zoomAndPan", "disable" );  

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die benutztes CSS-Styles in die SVG-Datei                       //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeSvgStyles()
{
  String ^ STR;

  // Schreiben des css-Stylesheets
  writer->WriteStartElement ( "style" );
  writer->WriteAttributeString ( "type", "text/css" );

  STR = ".StandardLinie {stroke:black; stroke-width:0.05; }\n";
  writer->WriteRaw ( STR );

  STR = ".HighlightLinie {stroke:red; stroke-width:0.05; }\n";
  writer->WriteRaw ( STR );

  STR = ".StandardFlaeche{fill:black; stroke:none; }\n";
  writer->WriteRaw ( STR );  

  STR = ".Tiefenskala {stroke:black; stroke-width:0.1; }";
  writer->WriteRaw ( STR );  

  STR = ".TiefenskalaText { font-size:0.5pt; font-family:Arial; }";
  writer->WriteRaw ( STR );  

  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt ein Lithology-Intervall                                         //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeLithologyInterval    ( LithologyInterval * pInterval )
{
  Lithology * pMajorLithologyComponent;
  double      oben, unten, links, rechts;

  if ( pInterval->getLithologyAnz() == 0  )
    return;

  oben    = pInterval->getFrom()* pBoreholeDarstellung->getVertikaleSkalierung();
  unten   = pInterval->getTo()* pBoreholeDarstellung->getVertikaleSkalierung(); 
  links   = lithologyAchse - 0.5*pBoreholeDarstellung->getBoreholeDurchmesserLithologie();
  rechts  = lithologyAchse + 0.5*pBoreholeDarstellung->getBoreholeDurchmesserLithologie();

  if ( pBoreholeDarstellung->getLithologyVisualisationStrategy() == BoreholeDarstellung::MAJOR_COMPONENT )
  {
    double percentage;
    string signature;

    pMajorLithologyComponent = pInterval->getLithology( 0 );
    percentage = pMajorLithologyComponent->getPercentageTo();
    for ( unsigned int i = 1; i < pInterval->getLithologyAnz(); i++ )
    {
      Lithology * pLithology = pInterval->getLithology( i );
      if ( pLithology->getPercentageTo() > percentage )
      {
        pMajorLithologyComponent = pLithology;
        percentage               = pLithology->getPercentageTo();
      }
    }

    string lithologyCode = pMajorLithologyComponent->rockCode;

    SchichtDarstParameter * pParam = pBoreholeDarstellung->getLithologyDarstellung( lithologyCode );

    writer->WriteStartElement( "rect" );
    writeSvgInterval ( oben, unten, links, rechts, pParam );
    writeTooltipLithology ( pInterval );
    writeAttributeDataLithology ( pInterval );
    writer->WriteEndElement(); 
  }
  else
  if ( pBoreholeDarstellung->getLithologyVisualisationStrategy() == BoreholeDarstellung::ALL_COMPONENTS )
  {
		//  Wird nicht unterstützt
  }

  writeAttributeDataLithology ( pInterval );
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt ein Stratigryphy Intervall                                      //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeStratigraphyInterval ( StratigraphyInterval * pInterval )
{
  double      oben, unten, links, rechts;

  oben    = pInterval->getFrom()* pBoreholeDarstellung->getVertikaleSkalierung();
  unten   = pInterval->getTo()* pBoreholeDarstellung->getVertikaleSkalierung(); 
  links   = stratigraphyAchse - 0.5*pBoreholeDarstellung->getBoreholeDurchmesserStratigraphie();
  rechts  = stratigraphyAchse + 0.5*pBoreholeDarstellung->getBoreholeDurchmesserStratigraphie();

  SchichtDarstParameter * pParam = pBoreholeDarstellung->getStratigraphyDarstellung( pInterval->chronoStratigraphy );

  writer->WriteStartElement( "rect" );
  writeSvgInterval ( oben, unten, links, rechts, pParam );
  writeTooltipStratigraphy ( pInterval );
  writeAttributeDataStratigraphy( pInterval );
  writer->WriteEndElement(); 

  writeTooltipStratigraphy ( pInterval );
  writeAttributeDataStratigraphy ( pInterval );
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Tiefen-Skala                                                //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeSkala()
{
  double boreholeLength;
  double skalenLaenge;
  double skalenstrichLaenge;
  double anz;
  double xA, xE, y;
  int    i, iAnz;
  bool  success;
  String ^ STR;
  String ^ POINT_A;
  String ^ POINT_E;

  success = pBorehole->getDoubleAttributWert( "bml:totalLength", boreholeLength  );
  if ( !success )
    return;
  
  anz = ceil ( boreholeLength / pBoreholeDarstellung->getAbstandSkalenwerte() ) ;
  iAnz = int ( anz );
  skalenLaenge = anz*pBoreholeDarstellung->getAbstandSkalenwerte();

  skalenstrichLaenge = 0.05*pBoreholeDarstellung->getAbstandSkalenwerte();

  writer->WriteStartElement( "path" );
  writer->WriteAttributeString( "class", "Tiefenskala" );

  STR = String::Concat ( "M ", QuConvert::ToString( skalenAchse ) , " 0 L ", QuConvert::ToString( skalenAchse ) , " ", QuConvert::ToString( skalenLaenge*pBoreholeDarstellung->getVertikaleSkalierung() )  );
  writer->WriteAttributeString( "d", STR );
  writer->WriteEndElement();

  for ( i = 0; i <= iAnz; i++ )
  {
    xA = skalenAchse;
    xE = skalenAchse + skalenstrichLaenge;
    y  = i* pBoreholeDarstellung->getAbstandSkalenwerte();

    POINT_A = String::Concat ( QuConvert::ToString( xA ), " ",  QuConvert::ToString( y*pBoreholeDarstellung->getVertikaleSkalierung() ) );
    POINT_E = String::Concat ( QuConvert::ToString( xE ), " ",  QuConvert::ToString( y*pBoreholeDarstellung->getVertikaleSkalierung() ) );
    STR     = String::Concat ( "M ", POINT_A , " L ", POINT_E );

    writer->WriteStartElement( "path" );
    writer->WriteAttributeString( "class", "Tiefenskala" );
    writer->WriteAttributeString( "d", STR );
    writer->WriteEndElement();

    writer->WriteStartElement( "text" );
    writer->WriteAttributeString( "class", "TiefenskalaText" );
    writer->WriteAttributeString( "text-anchor", "start" );
    writer->WriteAttributeString( "x", QuConvert::ToString( xE ) );
    writer->WriteAttributeString( "y", QuConvert::ToString( y*pBoreholeDarstellung->getVertikaleSkalierung() + 0.25 ) );
    writer->WriteString( QuConvert::ToString( y ) );
    writer->WriteEndElement();
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt Tooltips                                                        //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeTooltipLithology ( LithologyInterval * pInterval )
{
  String ^ STR = nullptr;

 if ( pInterval->rockNameText != "" )
 {
   STR = QuConvert::ToString ( pInterval->rockNameText );
   if ( pInterval->rockCode != "" )
     STR = String::Concat( STR, " (", QuConvert::ToString ( pInterval->rockCode ), ")" );
 }
 else
   if ( pInterval->rockCode != "" )
     STR = QuConvert::ToString ( pInterval->rockCode );

 writer->WriteStartElement( "title" );
 writer->WriteString( STR );
 writer->WriteEndElement();

}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die attributiven Daten eines Lithologie-Intervalls							 //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeAttributeDataLithology ( LithologyInterval * pInterval )
{
  String ^ STR = "";

  writer->WriteStartElement( "desc" );

  STR = String::Concat( STR,  "Intervall von ", QuConvert::ToString( pInterval->getFrom(), 2 ),  " bis ",  QuConvert::ToString( pInterval->getTo(), 2 ), "\n" );

  if ( pInterval->intervalType != "" )
    STR = String::Concat ( STR, "intervalType: ", QuConvert::ToString( pInterval->intervalType ), "\n" );

  if ( pInterval->rockCode != "" )
    STR = String::Concat ( STR, "rockCode: ", QuConvert::ToString( pInterval->rockCode ), "\n" );

  if ( pInterval->rockNameText != "" )
    STR = String::Concat ( STR, "rockNameText: ", QuConvert::ToString( pInterval->rockNameText ), "\n" );

  if ( pInterval->descriptiveText != "" )
    STR = String::Concat ( STR, "descriptiveText: ", QuConvert::ToString( pInterval->descriptiveText ), "\n" );

  if ( pInterval->genesis != "" )
    STR = String::Concat ( STR, "genesis: ", QuConvert::ToString( pInterval->genesis ), "\n" );

  if ( pInterval->geoGenesis != "" )
    STR = String::Concat ( STR, "geoGenesis: ", QuConvert::ToString( pInterval->geoGenesis ), "\n" );

  if ( pInterval->carbonatContent != "" )
    STR = String::Concat ( STR, "carbonatContent: ", QuConvert::ToString( pInterval->carbonatContent ), "\n" );

  if ( pInterval->waterContent != "" )
    STR = String::Concat ( STR, "waterContent: ", QuConvert::ToString( pInterval->waterContent ), "\n" );

  if ( pInterval->consistency != "" )
    STR = String::Concat ( STR, "consistency: ", QuConvert::ToString( pInterval->consistency ), "\n" );

  for ( unsigned int i = 0; i < pInterval->getLithologyAnz(); i++ )
  {
    Lithology * pLithology = pInterval->getLithology( i );

    if ( pLithology->getPercentageFrom() != pLithology->getPercentageTo() )
      STR = String::Concat ( STR, QuConvert::ToString ( pLithology->getPercentageFrom(), 2 ), " - ",  
                                  QuConvert::ToString ( pLithology->getPercentageTo(), 2 ), "´% ", 
                                  QuConvert::ToString ( pLithology->rockText ), "\n" );
    else
      STR = String::Concat ( STR, QuConvert::ToString ( pLithology->getPercentageFrom(), 2 ), " % ", 
                             QuConvert::ToString ( pLithology->rockText ) );

    if ( pLithology->rockColor != "" )
      STR = String::Concat ( STR, ", ", QuConvert::ToString( pLithology->rockColor ) );

    if ( pLithology->rockColorMixture != "" )
      STR = String::Concat ( STR, ", ", QuConvert::ToString( pLithology->rockColorMixture ) );

    STR = String::Concat ( STR, "\n" );

  }

  writer->WriteString( STR );
  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert den Tooltip für ein Stratigraphie-Intervall										 //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeTooltipStratigraphy ( StratigraphyInterval * pInterval )
{
  String             ^ STR = nullptr;
  string               chronoStratigraphie = "";
  string               lithoStratigraphie  = "";
  bool                 success;
  GmlDictionary      * pCodelists;
  GmlDictionaryEntry * pCodelistStratigraphie;

  if ( pInterval->chronoStratigraphy != "" )
  {
    pCodelists = pBorehole->getFeatures()->getGmlSchema()->getCodeListsDictionary();
    pCodelistStratigraphie = pCodelists->getDictionaryEntry( "http://www.infogeo.de/boreholeml/3.0/codelists/ChronoStratigraphyList.xml" );
    if ( pCodelistStratigraphie != NULL )
    {
      success = pCodelistStratigraphie->getSchluesselText ( pInterval->chronoStratigraphy, chronoStratigraphie );
      if ( !success )
        chronoStratigraphie = pInterval->chronoStratigraphy;
    }
    STR = QuConvert::ToString ( chronoStratigraphie );
  }

  if ( pInterval->lithoStratigraphy != "" )
  {
    if ( STR != nullptr )
      STR = String::Concat( STR, " / ", QuConvert::ToString( pInterval->lithoStratigraphy) );
    else
      STR = QuConvert::ToString( pInterval->lithoStratigraphy );
  }

  if ( STR != nullptr )
  {
    writer->WriteStartElement( "title" );
    writer->WriteString( STR );
    writer->WriteEndElement();

  }
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die attributiven Daten eines Stratigraphie-Intervalls					 //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeAttributeDataStratigraphy ( StratigraphyInterval * pInterval )
{
  String ^ STR = "";

  writer->WriteStartElement( "desc" );

  STR = String::Concat( STR,  "Intervall von ", QuConvert::ToString( pInterval->getFrom(), 2 ),  " bis ",  QuConvert::ToString( pInterval->getTo(), 2 ), "\n" );

  if ( pInterval->chronoStratigraphy != "" )
    STR = String::Concat ( STR, "chronoStratigraphy: ", QuConvert::ToString( pInterval->chronoStratigraphyText ), "\n" );

  if ( pInterval->lithoStratigraphy != "" )
    STR = String::Concat ( STR, "lithoStratigraphy: ", QuConvert::ToString( pInterval->lithoStratigraphy ), "\n" );

  writer->WriteString( STR );
  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert den SVG-Code zur Darstellung eines Intervalls                  //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeSvgInterval ( double oben, double unten, double links, double rechts, SchichtDarstParameter * pParam )
{
  String ^ STR;
  string   patternId;
  string   colorString;

  writer->WriteAttributeString( "class", "StandardLinie" );
  writer->WriteAttributeString( "x", QuConvert::ToString( links, 5 ) );
  writer->WriteAttributeString( "y", QuConvert::ToString( oben, 5 ) );
  writer->WriteAttributeString( "width", QuConvert::ToString( rechts - links, 5 ) );
  writer->WriteAttributeString( "height", QuConvert::ToString( unten-oben, 5 ) );

  if ( pParam->getSignaturePath() != "" )
  {
		size_t index = pParam->getSignaturePath().find( "." );
    if ( index > 0 )
      patternId = pParam->getSignaturePath().substr( 0, index );
    else
      patternId = pParam->getSignaturePath();

    STR = String::Concat( "url(#", QuConvert::ToString( patternId ), ")" );
    writer->WriteAttributeString( "fill", STR );
  }
  else
  if ( pParam->getPattern() != NULL )
  {
    patternId = pParam->getPattern()->getPatternId();

    STR = String::Concat( "url(#", QuConvert::ToString( patternId ), ")" );
    writer->WriteAttributeString( "fill", STR );
  }
  else
  if ( pParam->getHatching() != NULL )
  {
    patternId = pParam->getHatching()->getHatchingId();

    STR = String::Concat( "url(#", QuConvert::ToString( patternId ), ")" );
    writer->WriteAttributeString( "fill", STR );
  }
  else
  {
    SvgColor * pColor = pParam->getColor();
    if ( pColor != NULL )
    {
      switch ( pColor->getColorType() )
      {
        case SvgColor::NONE:
          writer->WriteAttributeString ( "fill", "none" );
          break;

        case SvgColor::NAME:
          colorString = pColor->getColorName();
          writer->WriteAttributeString ( "fill", QuConvert::ToString ( colorString ) );
          break;

        case SvgColor::RGB:
          colorString = pColor->getColorStringRGB();
          writer->WriteAttributeString ( "fill", QuConvert::ToString ( colorString ) );
          break;
        }   
      }
      else
        writer->WriteAttributeString ( "fill", "none" );

  }

  writer->WriteAttributeString( "onmouseover", "top.karteInfo(evt)");
  writer->WriteAttributeString( "onmouseout", "top.karteOut(evt)");
  writer->WriteAttributeString( "onclick", "top.objInfo(evt)");
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben eines SVG-Patterns	                    									 //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeSvgPattern ( SvgColor * pColor, Pattern * pPattern )
{
  string		    str;
  String		  ^ STR;
  double		    x, y, width, height, angle, scale;
  unsigned int  i;
  SvgGeometry * pSvgGeometry;
  writer->WriteStartElement ( "pattern" );

  str = pPattern->getPatternId ();
  STR = QuConvert::ToString ( str );
  writer->WriteAttributeString ( "id", STR );

  pPattern->getGeometry ( x, y, width, height );
  scale = pPattern->getScale();

  STR = XmlConvert::ToString ( x );
  writer->WriteAttributeString ( "x", STR );
  STR = XmlConvert::ToString ( y );
  writer->WriteAttributeString ( "y", STR );
  STR = XmlConvert::ToString ( width );
  writer->WriteAttributeString ( "width", STR );
  STR = XmlConvert::ToString ( height );
  writer->WriteAttributeString ( "height", STR );
  writer->WriteAttributeString ( "patternUnits", "userSpaceOnUse" );

  STR = String::Concat ( "scale(",  XmlConvert::ToString( scale ), ")" );
  angle = pPattern->getAngle ();
  if ( angle != 0.0 )
    STR = String::Concat ( STR, " rotate(", QuConvert::ToString ( angle ), ")" );
  writer->WriteAttributeString ( "patternTransform", STR );

  if ( pColor != NULL )
  {
    SvgRect * pRect = new SvgRect( "" );
    pRect->setGeometry( 0.0, 0.0, width, height );
    pRect->setFillColor( pColor );
    pRect->setFill( true );
    writeGeometryElement( pRect );
  }

  for ( i = 0; i < pPattern->getGeometryElementAnz(); i++ )
  {
    pSvgGeometry = pPattern->getGeometryElement ( i );
    writeGeometryElement ( pSvgGeometry );
  }

  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines SVG-Geometrieelements					                  				 //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeGeometryElement ( SvgGeometry * pSvgGeometryP )
{
  string     str;
  string     stdString;
  String   ^ STR;
  double     strokeWidth     = pSvgGeometryP->getStrokeWidth();
  string     strokeDasharray = pSvgGeometryP->getStrokeDasharray();
  SvgColor * pStrokeColor    = pSvgGeometryP->getStrokeColor();
  SvgColor * pFillColor      = pSvgGeometryP->getFillColor();

  switch ( pSvgGeometryP->getSvgGeometryType() )
  {
  case SvgGeometry::SVG_PATH:
    {
      writer->WriteStartElement ( "path" );

      str = pSvgGeometryP->getClassId();
      if ( str != "" )
      {
        STR = QuConvert::ToString ( str );
        writer->WriteAttributeString ( "class", STR );
      }
      else
      {
        if ( pStrokeColor != NULL )
        {
          switch ( pStrokeColor->getColorType() )
          {
          case SvgColor::NONE:
            writer->WriteAttributeString ( "stroke", "none" );
            break;

          case SvgColor::NAME:
            stdString = pStrokeColor->getColorName();
            writer->WriteAttributeString ( "stroke", QuConvert::ToString ( stdString ) );
            break;

          case SvgColor::RGB:
            stdString = pStrokeColor->getColorStringRGB();
            writer->WriteAttributeString ( "stroke", QuConvert::ToString ( stdString ) );
            break;
          }   
        }
        else
          writer->WriteAttributeString ( "stroke", "none" );

        if ( pFillColor != NULL )
        {
          switch ( pFillColor->getColorType() )
          {
          case SvgColor::NONE:
            writer->WriteAttributeString ( "fill", "none" );
            break;

          case SvgColor::NAME:
            stdString = pFillColor->getColorName();
            writer->WriteAttributeString ( "fill", QuConvert::ToString ( stdString ) );
            break;

          case SvgColor::RGB:
            stdString = pFillColor->getColorStringRGB();
            writer->WriteAttributeString ( "fill", QuConvert::ToString ( stdString ) );
            break;
          }   
        }
        else
          writer->WriteAttributeString ( "fill", "none" );

        if ( strokeWidth != 0.0 )
        {
          STR = XmlConvert::ToString ( strokeWidth );
          writer->WriteAttributeString ( "stroke-width", STR );
        }

        if ( strokeDasharray != "" )
        {
          STR = QuConvert::ToString ( strokeDasharray );
          writer->WriteAttributeString ( "stroke-dasharray", STR );
        }
      }

      str = ((SvgPath*)pSvgGeometryP)->getPathString();
      STR = QuConvert::ToString ( str );
      writer->WriteAttributeString ( "d", STR );
      writer->WriteEndElement();
      break;
    }

  case SvgGeometry::SVG_CIRCLE:
    {
      double cx, cy, r;

      writer->WriteStartElement ( "circle" );

      str = pSvgGeometryP->getClassId();

      if ( str != "" )
      {
        STR = QuConvert::ToString ( str );
        writer->WriteAttributeString ( "class", STR );
      }
      else
      {
        if ( pStrokeColor != NULL )
        {
          switch ( pStrokeColor->getColorType() )
          {
          case SvgColor::NONE:
            writer->WriteAttributeString ( "stroke", "none" );
            break;

          case SvgColor::NAME:
            stdString = pStrokeColor->getColorName();
            writer->WriteAttributeString ( "stroke", QuConvert::ToString ( stdString ) );
            break;

          case SvgColor::RGB:
            stdString = pStrokeColor->getColorStringRGB();
            writer->WriteAttributeString ( "stroke", QuConvert::ToString ( stdString ) );
            break;
          }   
        }
        else
          writer->WriteAttributeString ( "stroke", "none" );

        if ( pFillColor != NULL )
        {
          switch ( pFillColor->getColorType() )
          {
          case SvgColor::NONE:
            writer->WriteAttributeString ( "fill", "none" );
            break;

          case SvgColor::NAME:
            stdString = pFillColor->getColorName();
            writer->WriteAttributeString ( "fill", QuConvert::ToString ( stdString ) );
            break;

          case SvgColor::RGB:
            stdString = pFillColor->getColorStringRGB();
            writer->WriteAttributeString ( "fill", QuConvert::ToString ( stdString ) );
            break;
          }   
        }
        else
          writer->WriteAttributeString ( "fill", "none" );

        if ( strokeWidth != 0.0 )
        {
          STR = XmlConvert::ToString ( strokeWidth );
          writer->WriteAttributeString ( "stroke-width", STR );
        }

        if ( strokeDasharray != "" )
        {
          STR = QuConvert::ToString ( strokeDasharray );
          writer->WriteAttributeString ( "stroke-dasharray", STR );
        }
      }

      ((SvgCircle*)pSvgGeometryP)->getGeometry ( cx, cy, r );
      writer->WriteAttributeString ( "cx", XmlConvert::ToString ( cx ) );
      writer->WriteAttributeString ( "cy", XmlConvert::ToString ( cy ) );
      writer->WriteAttributeString ( "r", XmlConvert::ToString ( r ) );
      writer->WriteEndElement();
      break;
    }

  case SvgGeometry::SVG_RECT:
    {
      double x, y, width, height;

      writer->WriteStartElement ( "rect" );

      str = pSvgGeometryP->getClassId();

      if ( str != "" )
      {
        STR = QuConvert::ToString ( str );
        writer->WriteAttributeString ( "class", STR );
      }
      else
      {
        if ( pStrokeColor != NULL )
        {
          switch ( pStrokeColor->getColorType() )
          {
          case SvgColor::NONE:
            writer->WriteAttributeString ( "stroke", "none" );
            break;

          case SvgColor::NAME:
            stdString = pStrokeColor->getColorName();
            writer->WriteAttributeString ( "stroke", QuConvert::ToString ( stdString ) );
            break;

          case SvgColor::RGB:
            stdString = pStrokeColor->getColorStringRGB();
            writer->WriteAttributeString ( "stroke", QuConvert::ToString ( stdString ) );
            break;
          }   
        }
        else
          writer->WriteAttributeString ( "stroke", "none" );

        if ( pFillColor != NULL )
        {
          switch ( pFillColor->getColorType() )
          {
          case SvgColor::NONE:
            writer->WriteAttributeString ( "fill", "none" );
            break;

          case SvgColor::NAME:
            stdString = pFillColor->getColorName();
            writer->WriteAttributeString ( "fill", QuConvert::ToString ( stdString ) );
            break;

          case SvgColor::RGB:
            stdString = pFillColor->getColorStringRGB();
            writer->WriteAttributeString ( "fill", QuConvert::ToString ( stdString ) );
            break;
          }   
        }
        else
          writer->WriteAttributeString ( "fill", "none" );

        if ( strokeWidth != 0.0 )
        {
          STR = XmlConvert::ToString ( strokeWidth );
          writer->WriteAttributeString ( "stroke-width", STR );
        }

        if ( strokeDasharray != "" )
        {
          STR = QuConvert::ToString ( strokeDasharray );
          writer->WriteAttributeString ( "stroke-dasharray", STR );
        }
      }

      ((SvgRect*)pSvgGeometryP)->getGeometry ( x, y, width, height );
      writer->WriteAttributeString ( "x", XmlConvert::ToString ( x ) );
      writer->WriteAttributeString ( "y", XmlConvert::ToString ( y ) );
      writer->WriteAttributeString ( "width", XmlConvert::ToString ( width ) );
      writer->WriteAttributeString ( "height", XmlConvert::ToString ( height ) );
      writer->WriteEndElement();
      break;
    }

  case SvgGeometry::SVG_TEXT:
    {
      double   x, y;
      string   textString;
      String ^ size;

      writer->WriteStartElement ( "text" );

      str = pSvgGeometryP->getClassId();
      if ( str != "" )
      {
        STR = QuConvert::ToString ( str );
        writer->WriteAttributeString ( "class", STR );
      }

      ((SvgText*)pSvgGeometryP)->getPosition ( x, y );
      writer->WriteAttributeString ( "x", XmlConvert::ToString ( x ) );
      writer->WriteAttributeString ( "y", XmlConvert::ToString ( y ) );

      size = ((SvgText*)pSvgGeometryP)->getTextSizePoints ();
      writer->WriteAttributeString ( "font-size", size );

      writer->WriteAttributeString ( "text-anchor", "middle" );

      str = ((SvgText*)pSvgGeometryP)->getTextString ();
      STR = QuConvert::ToString ( str );
      writer->WriteString ( STR );

      writer->WriteEndElement();
      break;
    }

  }
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben eines speziellen SVG-Patterns, das ein Hatching darstellt  //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeSvgHatching ( SvgColor * pColor, Hatching * pHatching )
{
	String ^ ID           = nullptr;
	String ^ Width			  = nullptr;
	String ^ Height			  = nullptr;
	String ^ Stroke			  = "black";
	String ^ Fill			    = nullptr;
	String ^ StrokeWidth	= nullptr;
	String ^ PathStringA	= nullptr;
	String ^ PathStringB	= nullptr;
	String ^ PathStringC	= nullptr;
	String ^ PathStringD	= nullptr;
	String ^ X			    	= nullptr;
	String ^ Y				    = nullptr;

	string  stdString;
	double  h;

	string     id           = pHatching->getHatchingId();
	string     hatchingType = pHatching->getHatchingType();
	double     strokeWidth  = pHatching->getStrokeWidth();
	double     lineDist     = pHatching->getLineDist();
  double     width, height;

	ID          = QuConvert::ToString ( id );
	StrokeWidth = XmlConvert::ToString ( strokeWidth );

	if ( hatchingType == "horizontal" )
	{
		Width = XmlConvert::ToString ( largeNumber );
		Height = XmlConvert::ToString ( lineDist );
    width = largeNumber;
    height = lineDist;

		X = XmlConvert::ToString ( 0 );
		Y = XmlConvert::ToString ( 0.5*lineDist );
		PathStringA = String::Concat ( "M ", X, " ", Y, " " );

		X = XmlConvert::ToString ( largeNumber );
		PathStringA = String::Concat ( PathStringA, "L ", X, " ", Y );
	}
	else
	if ( hatchingType == "vertical" )
	{
		Width = XmlConvert::ToString ( lineDist );
		Height = XmlConvert::ToString ( largeNumber );
    width = lineDist;
    height = largeNumber;

		X = XmlConvert::ToString ( 0.5*lineDist );
		Y = XmlConvert::ToString ( 0.0 );
		PathStringA = String::Concat ( "M ", X, " ", Y, " " );

		Y = XmlConvert::ToString ( largeNumber );
		PathStringA = String::Concat ( PathStringA, "L ", X, " ", Y );
	}
	else
	if ( hatchingType == "chequered" )
	{
		Width = XmlConvert::ToString ( lineDist );
		Height = XmlConvert::ToString ( lineDist );
    width = lineDist;
    height = lineDist;

		X = XmlConvert::ToString ( 0.5*lineDist );
		Y = XmlConvert::ToString ( 0.0 );
		PathStringA = String::Concat ( "M ", X, " ", Y, " " );

		Y = XmlConvert::ToString ( largeNumber );
		PathStringA = String::Concat ( PathStringA, "L ", X, " ", Y );

		X = XmlConvert::ToString ( 0.0 );
		Y = XmlConvert::ToString ( 0.5*lineDist );
		PathStringB = String::Concat ( "M ", X, " ", Y, " " );

		X = XmlConvert::ToString ( largeNumber );
		PathStringB = String::Concat ( PathStringB, "L ", X, " ", Y );
	}
	else
	if ( hatchingType == "lowerLeftToUpperRight" )
	{
		h = lineDist*sqrt( 2.0 );
		Width = XmlConvert::ToString  ( h );
		Height = XmlConvert::ToString ( h );
    width = h;
    height = h;

		X = XmlConvert::ToString ( -strokeWidth );
		Y = XmlConvert::ToString ( 0.5*h + strokeWidth );
		PathStringA = String::Concat ( "M ", X, " ", Y, " " );

		X = XmlConvert::ToString ( 0.5*h + strokeWidth );
		Y = XmlConvert::ToString ( - strokeWidth );
		PathStringA = String::Concat ( PathStringA, "L ", X, " ", Y );

		X = XmlConvert::ToString ( 0.5*h - strokeWidth );
		Y = XmlConvert::ToString ( h + strokeWidth );
		PathStringB = String::Concat ( "M ", X, " ", Y, " " );

		X = XmlConvert::ToString ( h + strokeWidth );
		Y = XmlConvert::ToString ( 0.5*h - strokeWidth );
		PathStringB = String::Concat ( PathStringB, "L ", X, " ", Y );
	}
	else
	if ( hatchingType == "upperLeftToLowerRight" )
	{
		h = lineDist*sqrt( 2.0 );
		Width = XmlConvert::ToString  ( h );
		Height = XmlConvert::ToString ( h );
    width = h;
    height = h;

		X = XmlConvert::ToString ( -strokeWidth );
		Y = XmlConvert::ToString ( 0.5*h - strokeWidth );
		PathStringA = String::Concat ( "M ", X, " ", Y, " " );

		X = XmlConvert::ToString ( 0.5*h + strokeWidth );
		Y = XmlConvert::ToString ( h + strokeWidth );
		PathStringA = String::Concat ( PathStringA, "L ", X, " ", Y );

		X = XmlConvert::ToString ( 0.5*h - strokeWidth );
		Y = XmlConvert::ToString ( -strokeWidth );
		PathStringB = String::Concat ( "M ", X, " ", Y, " " );

		X = XmlConvert::ToString ( h + strokeWidth );
		Y = XmlConvert::ToString ( 0.5*h + strokeWidth );
		PathStringB = String::Concat ( PathStringB, "L ", X, " ", Y );
	}
	else
	if ( hatchingType == "crossed" )
	{
		h = lineDist*sqrt( 2.0 );
		Width = XmlConvert::ToString  ( h );
		Height = XmlConvert::ToString ( h );
    width = h;
    height = h;

		X = XmlConvert::ToString ( -strokeWidth );
		Y = XmlConvert::ToString ( 0.5*h + strokeWidth );
		PathStringA = String::Concat ( "M ", X, " ", Y, " " );

		X = XmlConvert::ToString ( 0.5*h + strokeWidth );
		Y = XmlConvert::ToString ( - strokeWidth );
		PathStringA = String::Concat ( PathStringA, "L ", X, " ", Y );

		X = XmlConvert::ToString ( 0.5*h - strokeWidth );
		Y = XmlConvert::ToString ( h + strokeWidth );
		PathStringB = String::Concat ( "M ", X, " ", Y, " " );

		X = XmlConvert::ToString ( h + strokeWidth );
		Y = XmlConvert::ToString ( 0.5*h - strokeWidth );
		PathStringB = String::Concat ( PathStringB, "L ", X, " ", Y );

		X = XmlConvert::ToString ( -strokeWidth );
		Y = XmlConvert::ToString ( 0.5*h - strokeWidth );
		PathStringC = String::Concat ( "M ", X, " ", Y, " " );

		X = XmlConvert::ToString ( 0.5*h + strokeWidth );
		Y = XmlConvert::ToString ( h + strokeWidth );
		PathStringC = String::Concat ( PathStringC, "L ", X, " ", Y );

		X = XmlConvert::ToString ( 0.5*h - strokeWidth );
		Y = XmlConvert::ToString ( -strokeWidth );
		PathStringD = String::Concat ( "M ", X, " ", Y, " " );

		X = XmlConvert::ToString ( h + strokeWidth );
		Y = XmlConvert::ToString ( 0.5*h + strokeWidth );
		PathStringD = String::Concat ( PathStringD, "L ", X, " ", Y );
	}

	writer->WriteStartElement ( "pattern" );
	writer->WriteAttributeString ( "id", ID );
	writer->WriteAttributeString ( "x", "0" );
	writer->WriteAttributeString ( "y", "0" );
	writer->WriteAttributeString ( "width", Width );
	writer->WriteAttributeString ( "height", Height );
	writer->WriteAttributeString ( "patternUnits", "userSpaceOnUse" );

  if ( pColor != NULL )
  {
    SvgRect * pRect = new SvgRect( "" );
    pRect->setGeometry( 0.0, 0.0, width, height );
    pRect->setFillColor( pColor );
    pRect->setFill( true );
    writeGeometryElement( pRect );
  }
	  
	writer->WriteStartElement ( "path" );
	writer->WriteAttributeString ( "d", PathStringA );
	writer->WriteAttributeString ( "stroke", Stroke );
	writer->WriteAttributeString ( "stroke-width", StrokeWidth );
	writer->WriteEndElement();

	if ( PathStringB != nullptr )
	{
		writer->WriteStartElement ( "path" );
		writer->WriteAttributeString ( "d", PathStringB );
		writer->WriteAttributeString ( "stroke", Stroke );
		writer->WriteAttributeString ( "stroke-width", StrokeWidth );
		writer->WriteEndElement();
	}

	if ( PathStringC != nullptr )
	{
		writer->WriteStartElement ( "path" );
		writer->WriteAttributeString ( "d", PathStringC );
		writer->WriteAttributeString ( "stroke", Stroke );
		writer->WriteAttributeString ( "stroke-width", StrokeWidth );
		writer->WriteEndElement();
	}

	if ( PathStringD != nullptr )
	{
		writer->WriteStartElement ( "path" );
		writer->WriteAttributeString ( "d", PathStringD );
		writer->WriteAttributeString ( "stroke", Stroke );
		writer->WriteAttributeString ( "stroke-width", StrokeWidth );
		writer->WriteEndElement();
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben eines patterns, das eine Bitmap benutzt                    //
///////////////////////////////////////////////////////////////////////////////
void BoreholeSvgWriter::writeSvgImagePattern ( SvgColor * pColor, std::string signaturePath, double width )
{
  string		    patternId, bitmapPath;
  String		  ^ STR;
  double		    x, y, height;

	size_t index  = signaturePath.find( "." );
  if ( index > 0 )
    patternId = signaturePath.substr( 0, index );
  else
    patternId = signaturePath;

  bitmapPath   = "file://" + Constant::getSchemaPath( BOREHOLEML_3_0 ) + "Stylesheets\\Bitmaps\\" + signaturePath;

  x = 0.0;
  y = 0.0;
  height = width * 0.441;

  writer->WriteStartElement ( "pattern" );

  STR = QuConvert::ToString ( patternId );
  writer->WriteAttributeString ( "id", STR );

  STR = XmlConvert::ToString ( x );
  writer->WriteAttributeString ( "x", STR );
  STR = XmlConvert::ToString ( y );
  writer->WriteAttributeString ( "y", STR );
  STR = XmlConvert::ToString ( width );
  writer->WriteAttributeString ( "width", STR );
  STR = XmlConvert::ToString ( height );
  writer->WriteAttributeString ( "height", STR );
  writer->WriteAttributeString ( "patternUnits", "userSpaceOnUse" );

  if ( pColor != NULL )
  {
    SvgRect * pRect = new SvgRect( "" );
    pRect->setGeometry( 0.0, 0.0, width, height );
    pRect->setFillColor( pColor );
    pRect->setFill( true );
    writeGeometryElement( pRect );
  }

  writer->WriteStartElement ( "image" );

  STR = QuConvert::ToString( bitmapPath );
  writer->WriteAttributeString ( "xlink:href", STR );

  STR = XmlConvert::ToString ( x );
  writer->WriteAttributeString ( "x", STR );
  STR = XmlConvert::ToString ( y );
  writer->WriteAttributeString ( "y", STR );
  STR = XmlConvert::ToString ( width );
  writer->WriteAttributeString ( "width", STR );
  STR = XmlConvert::ToString ( height );
  writer->WriteAttributeString ( "height", STR );

  writer->WriteEndElement();

  writer->WriteEndElement();
}


















