#include "StdAfx.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Windows::Forms;
using namespace System::Text;
using namespace System::Collections;

#include <set>
#include <map>
#include <vector>

#include "SrsManagerRef.h"
#include "Feature.h"
#include "Convert.h"
#include "Logfile.h"
#include "GmlSchema.h"
#include "Dictionary.h"
#include "GmlNamespaces.h"
#include "Geometrie.h"
#include "TimeX.h"
#include "KmlVisualisierung.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  KmlVisualParams                                                          //
//  Zusammenfassung aller KML-Darstellungsparameter für GML-Klassen          //
///////////////////////////////////////////////////////////////////////////////
KmlVisualParams::KmlVisualParams()
{ 
}

KmlVisualParams::~KmlVisualParams()
{
  map<string,KmlKlassenVisualParams*>::iterator iter;

  for ( iter = mKlassenVisualParams.begin(); iter != mKlassenVisualParams.end(); iter++ )
  {
    delete iter->second;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Darstellungs-Objekt zur map hinzufügen                                   //
///////////////////////////////////////////////////////////////////////////////
void KmlVisualParams::addKlassenVisualParams (  KmlKlassenVisualParams * pKlassenVisualParams )
{
  map<string,KmlKlassenVisualParams*>::iterator iter;
  string                                        klassenName = pKlassenVisualParams->getKlassenName();

  iter = mKlassenVisualParams.find( klassenName );
  if ( iter != mKlassenVisualParams.end() ) 
    delete iter->second;

  mKlassenVisualParams[klassenName] = pKlassenVisualParams;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Darstellungsobjekt für eine best. Klasse                     //
///////////////////////////////////////////////////////////////////////////////
KmlKlassenVisualParams * KmlVisualParams::getKlassenVisualParams ( std::string klassenName )
{
  map<string,KmlKlassenVisualParams*>::iterator iter;

  iter = mKlassenVisualParams.find( klassenName );

  if ( iter != mKlassenVisualParams.end() )
    return iter->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Flächen-Darstellungsobjekt für eine best. Klasse             //
///////////////////////////////////////////////////////////////////////////////
KmlPolyVisualParams * KmlVisualParams::getPolygonKlassenVisualParams ( std::string klassenName, std::string selectAttributName )
{
  map<string,KmlKlassenVisualParams*>::iterator iter;
  KmlKlassenVisualParams                     * pParams;

  iter = mKlassenVisualParams.find( klassenName );
  if ( iter ==  mKlassenVisualParams.end() )
    return NULL;
  else
  {
    pParams = iter->second;
    return pParams->getPolyVisualParams( selectAttributName );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Linien-Darstellungsobjekt für eine best. Klasse              //
///////////////////////////////////////////////////////////////////////////////
KmlLineVisualParams * KmlVisualParams::getLineKlassenVisualParams ( std::string klassenName, std::string selectAttributName )
{
  map<string,KmlKlassenVisualParams*>::iterator iter;
  KmlKlassenVisualParams                     * pParams;

  iter = mKlassenVisualParams.find( klassenName );
  if ( iter ==  mKlassenVisualParams.end() )
    return NULL;
  else
  {
    pParams = iter->second;
    return pParams->getLineVisualParams( selectAttributName );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  KmlKlassenVisualParams                                                   //
//  KML-Darstellungsparameter für eine GML-Klasse                            //
//  Es können Parameter für Linien und Flächendarstellung verwaltet werden   //
///////////////////////////////////////////////////////////////////////////////

KmlKlassenVisualParams::KmlKlassenVisualParams()
{
  pPolyVisualParamsDefault = NULL;
  pLineVisualParamsDefault = NULL;

  klassenName        = "";
}

KmlKlassenVisualParams::~KmlKlassenVisualParams()
{
  if ( pPolyVisualParamsDefault != NULL ) delete pPolyVisualParamsDefault;
  if ( pLineVisualParamsDefault != NULL ) delete pLineVisualParamsDefault;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen flächenhafter Darstellungsparameter                           //
///////////////////////////////////////////////////////////////////////////////
void KmlKlassenVisualParams::addPolyVisualParams ( KmlPolyVisualParams * pParams, std::string selectAttributName )
{
  if ( selectAttributName == "" )
  {
    if ( pPolyVisualParamsDefault != NULL ) delete pPolyVisualParamsDefault;
    pPolyVisualParamsDefault = pParams;
  }
  else
  {
    mPolyVisualParams[selectAttributName] = pParams;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen linienhafter Darstellungsparameter                            //
///////////////////////////////////////////////////////////////////////////////
void KmlKlassenVisualParams::addLineVisualParams ( KmlLineVisualParams * pParams, std::string selectAttributName )
{
  if ( selectAttributName == "" )
  {
    if ( pLineVisualParamsDefault != NULL ) delete pLineVisualParamsDefault;
    pLineVisualParamsDefault = pParams;
  }
  else
  {
    mLineVisualParams[selectAttributName] = pParams;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert flächenhafte Visualisierugsparameter                             //
///////////////////////////////////////////////////////////////////////////////
KmlPolyVisualParams * KmlKlassenVisualParams::getPolyVisualParams( std::string selectAttributName )
{
  if ( selectAttributName == "" )
    return pPolyVisualParamsDefault;
  else
  {
    if ( mPolyVisualParams.find ( selectAttributName ) != mPolyVisualParams.end() )
      return mPolyVisualParams[selectAttributName];
    else
      return NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert linienhafte Visualisierugsparameter                              //
///////////////////////////////////////////////////////////////////////////////
KmlLineVisualParams * KmlKlassenVisualParams::getLineVisualParams( std::string selectAttributName )
{
  if ( selectAttributName == "" )
    return pLineVisualParamsDefault;
  else
  {
    if ( mLineVisualParams.find ( selectAttributName ) != mLineVisualParams.end() )
      return mLineVisualParams[selectAttributName];
    else
      return NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  KmlPolyVisualParam                                                       //
//  KML Darstellungsparameter für Flächen                                    //
///////////////////////////////////////////////////////////////////////////////
KmlPolyVisualParams::KmlPolyVisualParams(  std::string selectAttributNameP )
{
  pFillColor         = NULL;
  pOutlineColor      = NULL;
  physicalWidth      = 0.0;
  selectAttributName = selectAttributNameP;
}

KmlPolyVisualParams::~KmlPolyVisualParams()
{
  map<string,KmlColor*>::iterator iter;

  if ( pFillColor != NULL ) delete pFillColor;
  if ( pOutlineColor != NULL ) delete pOutlineColor;

  for ( iter = colorTable.begin(); iter != colorTable.end(); iter++ )
    delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Flächenfüllungs-Farbe                                          //
///////////////////////////////////////////////////////////////////////////////
void KmlPolyVisualParams::setFillColor ( KmlColor * pColor )
{
  if ( pFillColor != NULL ) delete pFillColor;
  pFillColor = pColor;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert, ob eine Flächenfüllungsfarbe gesetzt ist                        //
///////////////////////////////////////////////////////////////////////////////
bool KmlPolyVisualParams::getFillColorSet ()
{
  if ( pFillColor != NULL ) return true;
  else                      return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Farbe der Flächen-Außenkontur                                  //
///////////////////////////////////////////////////////////////////////////////
void KmlPolyVisualParams::setOutlineColor ( KmlColor * pColor )
{
  if ( pOutlineColor != NULL ) delete pOutlineColor;
  pOutlineColor = pColor;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert, ob eine Farbe der Aßenkontur gesetzt ist                        //
///////////////////////////////////////////////////////////////////////////////
bool KmlPolyVisualParams::getOutlineColorSet()
{
  if ( pOutlineColor != NULL ) return true;
  else                         return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Farbe                                                            //
///////////////////////////////////////////////////////////////////////////////
KmlColor * KmlPolyVisualParams::getColor ( std::string attributWert )
{
  map<string,KmlColor*>::iterator iter;

  iter = colorTable.find( attributWert );
  if ( iter != colorTable.end() )
    return iter->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  KmlLineVisualParam                                                       //
//  Kml Darstellungsparameter für Linien                                     //
///////////////////////////////////////////////////////////////////////////////
KmlLineVisualParams::KmlLineVisualParams( std::string selectAttributNameP )
{
  pLineColor         = NULL;
  physicalWidth      = 0.0;
  selectAttributName = selectAttributNameP;
}

KmlLineVisualParams::~KmlLineVisualParams()
{
  map<string,KmlColor*>::iterator iter;

  if ( pLineColor != NULL )  delete pLineColor;

  for ( iter = colorTable.begin(); iter != colorTable.end(); iter++ )
    delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Linienfarbe                                                    //
///////////////////////////////////////////////////////////////////////////////
void KmlLineVisualParams::setLineColor ( KmlColor * pColor )
{
  if ( pLineColor != NULL ) delete pLineColor;
  pLineColor = pColor;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert, ob eine Linienfarbe gesetzt ist                                 //
///////////////////////////////////////////////////////////////////////////////
bool KmlLineVisualParams::getLineColorSet()
{
  if ( pLineColor != NULL ) return true;
  else                      return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Farbe                                                            //
///////////////////////////////////////////////////////////////////////////////
KmlColor * KmlLineVisualParams::getColor ( std::string attributWert )
{
  map<string,KmlColor*>::iterator iter;

  iter = colorTable.find( attributWert );
  if ( iter != colorTable.end() )
    return iter->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  KmlColor                                                                 //
//  R, G, B und Transparenzwert                                              //
///////////////////////////////////////////////////////////////////////////////
KmlColor::KmlColor()
{
  m_r     = 0;
  m_g     = 0;
  m_b     = 0;
  m_alpha = 255;
}

KmlColor::KmlColor ( short r, short g, short b, short alpha )
{
  m_r     = r;
  m_g     = g;
  m_b     = b;
  m_alpha = alpha;
}

KmlColor::~KmlColor()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Generierung der Hex-Darstellung                                          //
///////////////////////////////////////////////////////////////////////////////
String ^  KmlColor::get()
{
  String ^ STR;

  STR = String::Concat ( Int16 ( m_alpha ).ToString( "X2" ),
                         Int16 ( m_b ).ToString( "X2" ),
                         Int16 ( m_g ).ToString( "X2" ),
                         Int16 ( m_r ).ToString( "X2" )
                          );

  return STR;
}

///////////////////////////////////////////////////////////////////////////////
//  KmlVisualParamReader                                                     //
// Einlesen einer Datei mit KML-Darstellungsparametern                       //
///////////////////////////////////////////////////////////////////////////////
KmlVisualParamReader::KmlVisualParamReader( String ^ gmlTypeP )
{
  gmlType    = gmlTypeP;
  pKmlParams = NULL;
}

KmlVisualParamReader::~KmlVisualParamReader()
{

}

///////////////////////////////////////////////////////////////////////////////
//  inlesen des XML-Files der KML-Visualisierungsparameter                   //
///////////////////////////////////////////////////////////////////////////////
KmlVisualParams * KmlVisualParamReader::read( std::string filename )
{
  String                 ^ fName;
  string                   stdString;
  bool                     first = true;
  KmlKlassenVisualParams * pKmlKlassenVisualParams;

  pKmlParams = new KmlVisualParams;
  fName  = QuConvert::ToString ( filename );
  reader = gcnew XmlTextReader(fName);

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;
      if ( first )
      {
        first = false;
        if ( pActElement->CompareTo("KmlVisualisierung") != 0 )
        {
          delete pKmlParams;
          pKmlParams = NULL;
          break;
        }      
      }

      if ( pActElement->CompareTo("KmlKlassenVisualParams") == 0 )
      {
        pKmlKlassenVisualParams = readKlassenVisualParams();
        if ( pKmlKlassenVisualParams != NULL )
          pKmlParams->addKlassenVisualParams( pKmlKlassenVisualParams );
      }
    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = reader->LocalName;
      if ( pActElement->CompareTo("KmlVisualisierung") == 0 ) break;
    }
  }
  return pKmlParams;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Visualisierungsparameter für eine Klasse                    //
///////////////////////////////////////////////////////////////////////////////
KmlKlassenVisualParams * KmlVisualParamReader::readKlassenVisualParams()
{
  KmlKlassenVisualParams * pKlassenVisualParams = new KmlKlassenVisualParams;
  KmlPolyVisualParams    * pPolyVisualParams;
  KmlLineVisualParams    * pLineVisualParams;
  String                 ^ STR;
  string                   stdString;
  string                   selectAttributName;

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("klassenName") == 0 )
      {
        STR =  reader->ReadString();
        QuConvert::systemStr2stdStr( stdString, STR );
        pKlassenVisualParams->setKlassenName( stdString );
      }
      else
      if ( pActElement->CompareTo("polyVisualParams") == 0 )
      {
        STR = reader->GetAttribute( "selectAttributName" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( selectAttributName, STR );
        else
          selectAttributName = "";
        
        pPolyVisualParams = readPolyVisualParams( selectAttributName );
        if ( pPolyVisualParams != NULL )
          pKlassenVisualParams->addPolyVisualParams( pPolyVisualParams, selectAttributName );
      }
      else
      if ( pActElement->CompareTo("lineVisualParams") == 0 )
      {
        STR = reader->GetAttribute( "selectAttributName" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( selectAttributName, STR );
        else
          selectAttributName = "";

        pLineVisualParams = readLineVisualParams( selectAttributName );
        if ( pLineVisualParams != NULL )
          pKlassenVisualParams->addLineVisualParams( pLineVisualParams, selectAttributName );
      }

    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = reader->LocalName;
      if ( pActElement->CompareTo("KmlKlassenVisualParams") == 0 ) break;
    }
  }
  return pKlassenVisualParams;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Visualisierungsparameter für Linien                         //
///////////////////////////////////////////////////////////////////////////////
KmlLineVisualParams * KmlVisualParamReader::readLineVisualParams( string selectAttributName )
{
  KmlLineVisualParams    * pLineVisualParams = new KmlLineVisualParams ( selectAttributName );
  String                 ^ STR;
  double                   d;
  KmlColor               * pColor;
  string                   attributWert;

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("lineColor") == 0 )
      {
        pColor = readKmlColor();
        if ( pColor != NULL ) pLineVisualParams->setLineColor( pColor );
      }
      else
      if ( pActElement->CompareTo("physicalWidth") == 0 )
      {
        STR = reader->ReadString();
        d = XmlConvert::ToDouble( STR );
        pLineVisualParams->setPhysicalWidth( d );
      }
      else
      if ( pActElement->CompareTo("colorTableEntry") == 0 )
      {
        pColor = readColorTableEntry( attributWert );
        pLineVisualParams->addColor( attributWert, pColor );
      }
    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = reader->LocalName;
      if ( pActElement->CompareTo("lineVisualParams") == 0 ) break;
    }
  }

  return pLineVisualParams;

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Visualisierungsparameter für Flächen                        //
///////////////////////////////////////////////////////////////////////////////
KmlPolyVisualParams * KmlVisualParamReader::readPolyVisualParams( string selectAttributName )
{
  KmlPolyVisualParams    * pPolyVisualParams = new KmlPolyVisualParams ( selectAttributName );
  String                 ^ STR;
  double                   d;
  KmlColor               * pColor;
  string                   attributWert;

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("fillColor") == 0 )
      {
        pColor = readKmlColor();
        if ( pColor != NULL ) pPolyVisualParams->setFillColor( pColor );
      }
      else
      if ( pActElement->CompareTo("lineColor") == 0 )
      {
        pColor = readKmlColor();
        if ( pColor != NULL ) pPolyVisualParams->setOutlineColor( pColor );
      }
      else
      if ( pActElement->CompareTo("physicalWidth") == 0 )
      {
        STR = reader->ReadString();
        d = XmlConvert::ToDouble( STR );
        pPolyVisualParams->setPhysicalWidth( d );
      }
      else
      if ( pActElement->CompareTo("colorTableEntry") == 0 )
      {
        pColor = readColorTableEntry( attributWert );
        pPolyVisualParams->addColor( attributWert, pColor );
      }
    }
    else
      if ( reader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if ( pActElement->CompareTo("polyVisualParams") == 0 ) break;
      }
  }

  return pPolyVisualParams;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer Farbdefinition                                            //
///////////////////////////////////////////////////////////////////////////////
KmlColor * KmlVisualParamReader::readKmlColor()
{
  String ^ STR;
  short    h;

  KmlColor * pKmlColor = new KmlColor;

  STR = reader->GetAttribute( "r" );
  if ( STR != nullptr )
  {
    h = XmlConvert::ToInt16( STR );
    pKmlColor->setR(h);
  }

  STR = reader->GetAttribute( "g" );
  if ( STR != nullptr )
  {
    h = XmlConvert::ToInt16( STR );
    pKmlColor->setG(h);
  }

  STR = reader->GetAttribute( "b" );
  if ( STR != nullptr )
  {
    h = XmlConvert::ToInt16( STR );
    pKmlColor->setB(h);
  }

  STR = reader->GetAttribute( "alpha" );
  if ( STR != nullptr )
  {
    h = XmlConvert::ToInt16( STR );
    pKmlColor->setAlpha(h);
  }

  return pKmlColor;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest eine Farbtabelle, die Attributwerten Farben zuordnet               //
///////////////////////////////////////////////////////////////////////////////
KmlColor * KmlVisualParamReader::readColorTableEntry ( string &attributWert )
{
  String   ^ STR;
  short      h;
  KmlColor * pKmlColor;

  pKmlColor = new KmlColor;

  STR = reader->GetAttribute ( "attributWert" );
  QuConvert::systemStr2stdStr( attributWert, STR );

  STR = reader->GetAttribute( "r" );
  if ( STR != nullptr )
  {
    h = XmlConvert::ToInt16( STR );
    pKmlColor->setR(h);
  }

  STR = reader->GetAttribute( "g" );
  if ( STR != nullptr )
  {
    h = XmlConvert::ToInt16( STR );
    pKmlColor->setG(h);
  }

  STR = reader->GetAttribute( "b" );
  if ( STR != nullptr )
  {
    h = XmlConvert::ToInt16( STR );
    pKmlColor->setB(h);
  }

  STR = reader->GetAttribute( "alpha" );
  if ( STR != nullptr )
  {
    h = XmlConvert::ToInt16( STR );
    pKmlColor->setAlpha(h);
  }

  return pKmlColor;
}

///////////////////////////////////////////////////////////////////////////////
//  KmlWriter                                                                //
//  Rausschreiben der Features als KML-Datei                                 //
///////////////////////////////////////////////////////////////////////////////
KmlWriter::KmlWriter ( KmlVisualParams * pVisualParamsP, Features * pFeaturesP )
{
  pKmlVisualParams   = pVisualParamsP;
  pFeatures          = pFeaturesP;
  pLogfile           = pFeatures->getLogfile();
  pSrsManager        = pFeatures->getSrsManager();
  pGmlObjekte        = pFeatures->getGmlSchema();
  pSchemaTypeNames   = pGmlObjekte->getSchemaTypeNames();
  pCodeLists         = pGmlObjekte->getCodeListsDictionary();

  pSrsProjection = gcnew SrsProjection ( pSrsManager );

  writer          = nullptr;
  zOffset         = 0.0;
  showEdges       = false;
  showFeatureData = false;

  lodStufe       = ALL;
}

KmlWriter::~KmlWriter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt GML-Features als KML-File                                       //
///////////////////////////////////////////////////////////////////////////////
bool KmlWriter::write ( std::string &fileName, string selectAttributName, int index )
{
  String                            ^ featureName;
  String                            ^ featureNameLoc;
  multimap<string,Feature*>           mFeatures;
  multimap<string,Feature*>::iterator iter;
  set<string>                         sKlassenNamen;
  Feature                           * pFeature;

  if ( pSrsProjection->isInitialized() == false )
    return false;

  kmlFilePath = QuConvert::ToString( fileName );

  System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();
  writer = gcnew XmlTextWriter( kmlFilePath, utf8);
  writer->Formatting = Formatting::Indented;

  writer->WriteStartDocument( true );
  writer->WriteComment("Generated by: KIT GML-Toolbox KML-Writer");

  writeKmlRoot();

  writer->WriteStartElement( "Document" );
  writer->WriteElementString( "visibility", "1" );

  writeCameraPosition();
  writeStyles( selectAttributName );

  writer->WriteStartElement( "Folder" );
  writer->WriteElementString ( "name", "CityModel" );

  featureName = "";

  pFeatures->getChildren( mFeatures, sKlassenNamen );
  for ( iter = mFeatures.begin(); iter != mFeatures.end(); iter++ )
  {
    pFeature = iter->second;
    if ( pFeature->getKlassenName() == "genobj:stringAttribute"   ||
         pFeature->getKlassenName() == "genobj:dateAttribute"     ||
         pFeature->getKlassenName() == "genobj:uriAttribute"      ||
         pFeature->getKlassenName() == "genobj:intAttribute"      ||
         pFeature->getKlassenName() == "genobj:doubleAttribute"   ||
         pFeature->getKlassenName() == "genobj:measureAttribute"  ||
         pFeature->getKlassenName() == "genobj:genericAttributeSet" 
       )
       continue;

    featureNameLoc = QuConvert::ToString ( pFeature->getKlassenName() );
    if ( featureNameLoc->CompareTo( featureName ) != 0 )
    {
      if ( featureName->CompareTo( "" ) != 0 )
        writer->WriteEndElement();  
      writer->WriteStartElement( "Folder" );
      writer->WriteElementString( "name", String::Concat( featureNameLoc, "Layer" ) );
      featureName = featureNameLoc;
    }

    writeFeature( pFeature, selectAttributName, index );
  }
  writer->WriteEndElement();  

  writer->WriteEndElement();

  writer->WriteEndElement();
  writer->Flush();
  writer->Close();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die XML-Root für KML V. 2.2                                     //
///////////////////////////////////////////////////////////////////////////////
void KmlWriter::writeKmlRoot()
{
  writer->WriteStartElement("kml ");

  writer->WriteAttributeString("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  writer->WriteAttributeString("xsi:schemaLocation", "http://www.opengis.net/kml/2.2 http://schemas.opengis.net/kml/2.2.0/ogckml22.xsd");
  writer->WriteAttributeString("xmlns", "http://www.opengis.net/kml/2.2");
  writer->WriteAttributeString("xmlns:gx", "http://www.google.com/kml/ext/2.2");
}


///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Startposition / Blickrichtung der Szenenkamera              //
///////////////////////////////////////////////////////////////////////////////
bool KmlWriter::writeCameraPosition()
{
  double sw[3], no[3], center[3], width;
  bool   success;
  int    i;
  double laengeBreite[3];
  string srsName;
  int    srsDimension;

  success = pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
  if ( !success ) return false;

  for ( i = 0; i < 2; i++ )
    center[i] = 0.5*(sw[i] + no[i] );
  center[2] = no[2];

  if ( no[0] - sw[0] > no[1] - sw[1] )
    width =  no[0] - sw[0];
  else
    width =  no[1] - sw[1];

  success = transformToLaengeBreite ( srsName, center, laengeBreite );
  if ( !success ) return false;

  writer->WriteStartElement( "LookAt" );
  writer->WriteElementString("longitude", XmlConvert::ToString( laengeBreite[0] ) );
  writer->WriteElementString("latitude", XmlConvert::ToString( laengeBreite[1] ) );
  writer->WriteElementString("altitude", XmlConvert::ToString( laengeBreite[2] + width ) );
  writer->WriteElementString("heading", "0.0" );
  writer->WriteElementString("tilt", XmlConvert::ToString( 0.0 ) );
  writer->WriteElementString("range", XmlConvert::ToString(  width ) );
  writer->WriteElementString("altitudeMode", "relativeToGround" );

  writer->WriteEndElement();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Darstellungs-Styles der vorkommenden Feaures                //
///////////////////////////////////////////////////////////////////////////////
void KmlWriter::writeStyles( string selectAttributName )
{
  set<string>              usedClasses;
  set<string>::iterator    iter;
  string                   klassenName;
  String ^                 styleName;
  KmlKlassenVisualParams * pClassParams;
  KmlPolyVisualParams    * pPolyParams;
  KmlLineVisualParams    * pLineParams;

  pFeatures->getUsedClasses( usedClasses );
  for ( iter = usedClasses.begin(); iter != usedClasses.end(); iter++ )
  {
    klassenName = *iter;
    pClassParams = pKmlVisualParams->getKlassenVisualParams( klassenName );
    if ( pClassParams == NULL ) continue;

    styleName   = getStyleName ( QuConvert::ToString ( klassenName ) );
    pPolyParams = pClassParams->getPolyVisualParams( selectAttributName );
    pLineParams = pClassParams->getLineVisualParams( selectAttributName );
    if ( pPolyParams == NULL && pLineParams == NULL ) continue;

    writer->WriteStartElement ( "Style" );
    writer->WriteAttributeString( "id", styleName );

    if ( pLineParams != NULL )
    {
      writer->WriteStartElement ( "LineStyle" );
      writer->WriteElementString( "color", pLineParams->getLineColor()->get() );
      writer->WriteElementString( "width", XmlConvert::ToString( pLineParams->getPhysicalWidth() ) );
      writer->WriteEndElement();
    }

    if ( pPolyParams != NULL )
    {
      if ( pPolyParams->getOutlineColorSet() )
      {
        writer->WriteStartElement ( "LineStyle" );
        writer->WriteElementString( "color", pPolyParams->getOutlineColor()->get() );
        writer->WriteElementString( "width", XmlConvert::ToString( pPolyParams->getPhysicalWidth() ) );
        writer->WriteEndElement();
      }

      if ( pPolyParams->getFillColorSet() )
      {
        writer->WriteStartElement( "PolyStyle" );
        writer->WriteElementString( "color", pPolyParams->getFillColor()->get() );
        writer->WriteElementString ( "fill", "1" );
        if ( pPolyParams->getOutlineColorSet() && showEdges )
          writer->WriteElementString ( "outline", "1" );
        else
          writer->WriteElementString ( "outline", "0" );
        writer->WriteEndElement();
      }
    }
    writer->WriteEndElement();
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Style-Namen zu einem GML-Klassennamen                        //
///////////////////////////////////////////////////////////////////////////////
String ^ KmlWriter::getStyleName ( String ^ klassenName )
{
  String ^ styleName;

  styleName = klassenName->Replace( ":", "_" );
  return styleName;
}


///////////////////////////////////////////////////////////////////////////////
//  Schreibt ein Feature                                                     //
///////////////////////////////////////////////////////////////////////////////
void KmlWriter::writeFeature ( Feature * pFeature, string selectAttributName, int index )
{
  multimap<string,Feature*>           mChildren;
  multimap<string,Feature*>::iterator iter;
  set<string>                         sKlassenNamen;
	size_t                              childAnz;
  string                              folderName;
  string                              klassenName;
  bool                                success;
  Feature                           * childFeature;
  String                            ^ featureName;
  String                            ^ featureNameLoc;

  childAnz    = pFeature->getChildren( mChildren, sKlassenNamen );
  klassenName = pFeature->getKlassenName();

  if ( childAnz > 0 )
  {
    writer->WriteStartElement( "Folder" );
    success = pFeature->getStringAttributWert( "gml:name", folderName );
    if ( success )
      folderName = klassenName + " - " + folderName;
    else
      folderName = klassenName;
    writer->WriteElementString( "name", QuConvert::ToString ( folderName ) );

    writer->WriteStartElement( "Snippet" );
    writer->WriteAttributeString( "maxLines", "0" );
    writer->WriteEndElement();

    if ( showFeatureData )
      writeKmlDescription ( pFeature );

    if ( pFeature->getGeometryPropertyAnz() > 0 )
    {
      writeKmlPlacemark( pFeature, selectAttributName, index );
    }

    featureName = "";

    for ( iter =  mChildren.begin(); iter != mChildren.end(); iter++ )
    {
      childFeature = iter->second;

      featureNameLoc = QuConvert::ToString ( childFeature->getKlassenName() );
      if ( featureNameLoc->CompareTo( featureName ) != 0 )
      {
        if ( featureName->CompareTo( "" ) != 0 )
          writer->WriteEndElement();  
        writer->WriteStartElement( "Folder" );
        writer->WriteElementString( "name", String::Concat( featureNameLoc, "Layer" ) );
        featureName = featureNameLoc;
      }
      writeFeature( childFeature, selectAttributName, index );
    }
    writer->WriteEndElement();  

    writer->WriteEndElement();  
  }
  else
  {
    writeKmlPlacemark ( pFeature, selectAttributName, index );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt ein KML-Placemark                                               //
///////////////////////////////////////////////////////////////////////////////
void KmlWriter::writeKmlPlacemark ( Feature * pFeature, string selectAttributName, int index )
{
  string   klassenName;
  string   placemarkName;
  String ^ styleUrl;
  bool     success;

  writer->WriteStartElement ( "Placemark ");
  klassenName = pFeature->getKlassenName();

  success = pFeature->getStringAttributWert( "gml:name", placemarkName );
  if ( success )
    placemarkName = klassenName + " - " + placemarkName;
  else
    placemarkName = klassenName;
  writer->WriteElementString( "name", QuConvert::ToString ( placemarkName ) );

  writer->WriteStartElement( "Snippet" );
  writer->WriteAttributeString( "maxLines", "0" );
  writer->WriteEndElement();

  if ( showFeatureData )
    writeKmlDescription ( pFeature );

  success = writeAttributeDependentColor( pFeature, selectAttributName, index );
  if ( !success ) 
  {
    styleUrl = String::Concat("#", getStyleName ( QuConvert::ToString ( klassenName ) ) );
    writer->WriteElementString( "styleUrl", styleUrl );
  }

  writeFeatureGeometry ( pFeature );

  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt und schreibt die Darstellungsfarbe in Abhängigkeit von einem    //
//  Attributwert.                                                            //
///////////////////////////////////////////////////////////////////////////////
bool KmlWriter::writeAttributeDependentColor( Feature * pFeature, string selectAttributName, size_t index )
{
  string                   klassenName;
  string                   attributName;
  string                   attributWert;
  string                   dataTypeName = "";
  bool                     success;
  size_t                   indexPos, anz;
  ATTRIBUT_TYP             attributTyp;
  KmlColor               * pColor;
  GmlKlasse              * pGmlKlasse;
  GmlAttribut            * pGmlAttribut;
  KmlKlassenVisualParams * pKlassenVisualParams;
  KmlPolyVisualParams    * pPolyVisualParams;
  KmlLineVisualParams    * pLineVisualParams = NULL;
  vector<Feature*>         vSubFeatures;
  UOM                    * pUOM;

  if ( selectAttributName == "" ) return false;

  klassenName = pFeature->getKlassenName();

  pKlassenVisualParams = pKmlVisualParams->getKlassenVisualParams( klassenName );

  if ( pKlassenVisualParams == NULL ) return false;

  pPolyVisualParams = pKlassenVisualParams->getPolyVisualParams( selectAttributName );
  if ( pPolyVisualParams != NULL )
  {
    indexPos = selectAttributName.find( "::" );
    if ( indexPos != string::npos )
    {
      dataTypeName = selectAttributName.substr ( 0, indexPos );
      attributName = selectAttributName.substr ( indexPos+2, attributName.length() - indexPos -2 );
    }
    else
      attributName = selectAttributName;

    if ( dataTypeName != "" )
    {
      anz = pFeature->getRelatedFeatures( dataTypeName, vSubFeatures );
      if ( anz > 0 && index < anz )
      {
        pFeature = vSubFeatures[index];
        if ( pFeature == NULL ) return false;

        klassenName = pFeature->getKlassenName();
      }
      else
        return false;
    }

    pGmlKlasse = pGmlObjekte->getGmlKlasse( klassenName );
    if ( pGmlKlasse != NULL )
    {
      pGmlAttribut = pGmlKlasse->getGmlAttribut( attributName );
      if ( pGmlAttribut != NULL )
        attributTyp = pGmlAttribut->getGmlAttributTyp();
      else
        attributTyp = _STRING;
    }
    else
      attributTyp = _STRING;

    success = pFeature->getAttributWert( attributTyp, attributName, attributWert, pUOM );
    if ( !success ) return false;

    pColor = pPolyVisualParams->getColor( attributWert );
    if ( pColor == NULL ) return false;

    writer->WriteStartElement( "Style" );
    writer->WriteStartElement( "PolyStyle" );
    writer->WriteElementString( "color" ,pColor->get() );
    writer->WriteElementString ( "fill", "true" );
    if ( !pPolyVisualParams->getOutlineColorSet() )
      writer->WriteElementString ( "outline", "false" );
    writer->WriteEndElement();
    writer->WriteEndElement();
  }
  else
    pLineVisualParams = pKlassenVisualParams->getLineVisualParams( selectAttributName );

  if ( pLineVisualParams != NULL )
  {
    index = selectAttributName.find( "::" );
    if ( index != string::npos )
    {
      dataTypeName = selectAttributName.substr ( 0, index );
      attributName = selectAttributName.substr ( index+2, attributName.length() - index -2 );
    }
    else
      attributName = selectAttributName;

    if ( dataTypeName != "" )
    {
      anz = pFeature->getRelatedFeatures( dataTypeName, vSubFeatures );
      if ( anz > 0 )
      {
        pFeature = vSubFeatures[0];
        if ( pFeature == NULL ) return false;

        klassenName = pFeature->getKlassenName();
      }
    }

    pGmlKlasse = pGmlObjekte->getGmlKlasseFromElementName( klassenName );
    if ( pGmlKlasse != NULL )
    {
      pGmlAttribut = pGmlKlasse->getGmlAttribut( attributName );
      if ( pGmlAttribut != NULL )
        attributTyp = pGmlAttribut->getGmlAttributTyp();
      else
        attributTyp = _STRING;
    }
    else
      attributTyp = _STRING;

    success = pFeature->getAttributWert( attributTyp, attributName, attributWert, pUOM );
    if ( !success ) return false;

    pColor = pLineVisualParams->getColor( attributWert );
    if ( pColor == NULL ) return false;

    writer->WriteStartElement( "Style" );
    writer->WriteStartElement( "LineStyle" );
    writer->WriteElementString( "color",pColor->get() );
    writer->WriteEndElement();
    writer->WriteEndElement();
  } 
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Standard-Flächenfarbe des Features nach den                 //
//  Darstellungsvorschriften                                                 //
///////////////////////////////////////////////////////////////////////////////
KmlColor KmlWriter::getStandardFillColor ( Feature * pFeature, string selectAttributName )
{
  string                   klassenName;
  KmlColor               * pColor = NULL;
  KmlKlassenVisualParams * pKlassenVisualParams;
  KmlPolyVisualParams    * pPolyVisualParams;

  klassenName = pFeature->getKlassenName();

  pKlassenVisualParams = pKmlVisualParams->getKlassenVisualParams( klassenName );
  if ( pKlassenVisualParams != NULL )
  {
    pPolyVisualParams = pKlassenVisualParams->getPolyVisualParams( selectAttributName );
    if ( pPolyVisualParams != NULL )
      pColor = pPolyVisualParams->getFillColor();
  }

  if ( pColor == NULL  )
    return KmlColor ( 0, 0, 0, 1 );
  else
    return *pColor;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die KML description                                             //
///////////////////////////////////////////////////////////////////////////////
void KmlWriter::writeKmlDescription ( Feature * pFeature )
{
  String ^ cdataString;

  writer->WriteStartElement( "description" );

  cdataString =  writeFeatureData( pFeature );
  writer->WriteCData ( cdataString );

  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Sachdaten eines Features                                    //
///////////////////////////////////////////////////////////////////////////////
String ^ KmlWriter::writeFeatureData ( Feature * pFeature )
{
	string   gmlId;
	string   klassenName;
	string   attributName;
	string   enumerationName;
	string   enumSchluessel;
	string   enumText;
	string   stdString;
	string   textInhalt;
  string   genericAttributName;
  string   genericAttributWert;
	String ^ STR;
	String ^ STR1;
	size_t       attributAnz, i, j, index, attributWertAnz;
	bool         success;
  ATTRIBUT_TYP attributTyp;
  StringAttr * pSimpleContent;
  DoubleAttr * doubleAttributWert;
  IntAttr    * intAttributWert;
  BoolAttr   * boolAttributWert;
  StringAttr * stringAttributWert;

	GmlKlasse          * pGmlKlasse;
	GmlAttribut        * pGmlAttribut;
	GmlDictionaryEntry * pDictionaryEntry;

	vector<GmlAttribut*>     gmlAttribute;
  vector<int>              classifierAnz;
  vector<string>           classifierNamen;
  vector<string>           classifierValues;
  vector<string>           classifierNamenLoc;
  vector<string>           classifierValuesLoc;
  vector<string>           genericAttributNamen;
  vector<StringAttr*>      stringAttributWerte;
  vector<IntAttr*>         intAttributWerte;
  vector<DoubleAttr*>      doubleAttributWerte;
  vector<DateAttr*>        dateAttributWerte;
  vector<UOM*>             uomAttributWerte;
  FeatureRelation        * pRelation;
  Feature                * pRelationZiel;
  vector<FeatureRelation*> relationen;
  vector<Feature*>         adressen;

  map<string,string>::iterator iterClassifier;

  vector<string> attributNamen;
  vector<string> attributWerte;
  vector<ATTRIBUT_TYP> attributTypen;

  GmlElement * pGmlElement;
  StringWriter ^ stringWriter = gcnew StringWriter;

  klassenName = pFeature->getKlassenName ();

  pGmlElement = pGmlObjekte->getGmlElement( klassenName );
  if ( pGmlElement == NULL )
  {
    if ( klassenName == "core:externalReference" )
      pGmlKlasse = pGmlObjekte->getGmlKlasse ( "core:ExternalReferenceType" );
    else
    if ( klassenName == "citygml:externalReference" )
      pGmlKlasse = pGmlObjekte->getGmlKlasse ( "citygml:ExternalReferenceType" );
    else
    if ( klassenName == "core:externalObject" )
      pGmlKlasse = pGmlObjekte->getGmlKlasse ( "core:ExternalObjectReferenceType" );
    else
    if ( klassenName == "citygml:externalObject" )
      pGmlKlasse = pGmlObjekte->getGmlKlasse ( "citygml:ExternalObjectReferenceType" );
    else
    if ( klassenName == "core:xalAddress" )
    {
      writeAddress ( stringWriter, pFeature );
      return stringWriter->ToString();
    }
  }
  else
    pGmlKlasse = pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );

  if ( pGmlKlasse == NULL )
    return "";

	gmlId = pFeature->getGmlId();
  index = 0;

	stringWriter->WriteLine ( "<html>" );

	stringWriter->WriteLine ( "<style type=\"text/css\">\n" );
	stringWriter->WriteLine ( "h1 {font-family: Arial, Helvetica, sans-serif; text-align: center;}" );
	stringWriter->WriteLine ( "h2 {font-family: Arial, Helvetica, sans-serif; text-align: center;}" );
	stringWriter->WriteLine ( "td {font-family: Arial, Helvetica, sans-serif;text-indent: 4px;}" );
	stringWriter->WriteLine ( "</style>" );

	STR = String::Concat ( "<h1>", QuConvert::ToString ( klassenName ), "</h1>" );
	stringWriter->WriteLine ( STR );

	stringWriter->WriteLine ( "<table width=\"100%\" border=\"1\" cellspacing=\"0\" bordercolor=\"#000000\">" );
	stringWriter->WriteLine ( "<tr>" );
	stringWriter->WriteLine ( "<td> <strong>Attribut-Name</strong></td>" );
	stringWriter->WriteLine ( "<td> <strong>Attribut-Wert</strong></td>" );
	stringWriter->WriteLine ( "</tr>" );

	attributName = "gml:id";

	stringWriter->WriteLine ( "<tr>" );
	STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
	stringWriter->WriteLine ( STR );

	STR1 = QuConvert::ToString ( pFeature->getGmlId() );
	STR1 = String::Concat ( "<td>", STR1, "</td>" );
	stringWriter->WriteLine ( STR1 );
	stringWriter->WriteLine ( "</tr>" );
 
  attributAnz = pGmlKlasse->getGmlAttribute ( gmlAttribute );
  for ( i = 0; i < attributAnz; i++ )
	{
    stringAttributWerte.clear();
    intAttributWerte.clear();
    doubleAttributWerte.clear();
    relationen.clear();

    pGmlAttribut    = gmlAttribute[i];
    attributTyp  = pGmlAttribut->getGmlAttributTyp();
    attributName = pGmlAttribut->getGmlAttributName();

		switch ( pGmlAttribut->getGmlAttributTyp() )
		{
		case _STRING:
      attributWertAnz = pFeature->getStringAttributWerte ( attributName, stringAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];

        STR1 = QuConvert::ToHtml ( stringAttributWert->value );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
			break;

		case _DATE:
    case _DATE_TIME:
      attributWertAnz = pFeature->getDateAttributWerte ( attributName, dateAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        AbstractTime * pTime = dateAttributWerte[j]->value;
        if ( pTime == NULL )
          continue;
        stdString = pTime->toString();

        STR1 = QuConvert::ToHtml ( stdString );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
			break;

		case _DOUBLE:
		case _ANGLE:
    case _LENGTH:
    case _AREA:
    case _VOLUME:
    case _MEASURE:
      attributWertAnz = pFeature->getDoubleAttributWerte ( attributName, doubleAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        doubleAttributWert = doubleAttributWerte[j];

        STR1 = Convert::ToString( doubleAttributWert->value );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
		break;

		case _INTEGER:
      attributWertAnz = pFeature->getIntegerAttributWerte ( attributName, intAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        intAttributWert = intAttributWerte[j];

        STR1 = Convert::ToString( intAttributWert->value );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
			break;

		case _ANY_URI:
      attributWertAnz = pFeature->getURLAttributWerte ( attributName, stringAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];

        STR1 = QuConvert::ToHtml ( stringAttributWert->value );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
			break;

    case _EXTERNAL_CODE_LIST_GML3_3:
      attributWertAnz = pFeature->getStringAttributWerte ( attributName, stringAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];
        enumText = "";
        for ( iterClassifier = stringAttributWert->classifier.begin(); iterClassifier != stringAttributWert->classifier.end(); iterClassifier++ )
        {
          if ( iterClassifier->first == "xlink:title" )
          {
            enumText =  iterClassifier->second;
            break;
          }
        }

        if ( enumText != "" )  stringAttributWert->value = enumText;

        STR1 = QuConvert::ToHtml ( stringAttributWert->value );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
      break;

		case _ENUMERATION:
    case _EXTERNAL_CODE_LIST:
      attributWertAnz = pFeature->getStringAttributWerte ( attributName, stringAttributWerte );
      enumerationName = pGmlAttribut->getGmlEnumeration();
      pDictionaryEntry = pCodeLists->getDictionaryEntry ( enumerationName );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];
        if ( pDictionaryEntry != NULL )
        {
          enumSchluessel = stringAttributWert->value;
          success = pDictionaryEntry->getSchluesselText ( enumSchluessel, enumText );
          if ( success ) stringAttributWert->value = enumText;
        }

        STR1 = QuConvert::ToHtml ( stringAttributWert->value );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
			break;

		case  _REFERENZ:
      break;

		case _DATA_TYPE:
      attributWertAnz = pFeature->getFeatureRelationen (attributName, relationen );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        pRelation = relationen[j];
        pRelationZiel = pFeatures->getFeature ( pRelation->featureId );

        if ( pRelationZiel != NULL )
        {
          pSimpleContent = pRelationZiel->getSimpleContent();
          if ( pSimpleContent != NULL )
          {
            STR1 = QuConvert::ToString ( pSimpleContent->value );

            stringWriter->WriteLine ( "<tr>" );
            STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
            stringWriter->WriteLine ( STR );
            STR1 = String::Concat ( "<td>", STR1, "</td>" );
            stringWriter->WriteLine ( STR1 );
            stringWriter->WriteLine ( "</tr>" );
          }
          else
          {
            stringWriter->WriteLine ( "<tr>" );
            STR = String::Concat ( "<td><strong>", QuConvert::ToString ( attributName ), "</strong></td>" );
            stringWriter->WriteLine ( STR );
            STR = String::Concat ( "<td>", "  ", "</td>" );
            stringWriter->WriteLine ( STR );
            stringWriter->WriteLine ( "</tr>" );
            writeDataType ( 1, stringWriter, pRelationZiel );
          }
        }
      }
			break;

		case _BOOLEAN:
      boolAttributWert = pFeature->getBoolAttributWert ( attributName );
      if ( boolAttributWert != NULL )
      {
        if ( boolAttributWert->value == true )
          stdString = "ja";
        else
          stdString = "nein";

        STR1 = QuConvert::ToHtml ( stdString );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
			break;
    
    default:
      break;
		}
  }
				
  stringWriter->WriteLine ( "</html>" );

  return stringWriter->ToString();

}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt einen komplexen Datentyp                                        //
///////////////////////////////////////////////////////////////////////////////
void KmlWriter::writeDataType ( int level, StringWriter ^ stringWriter, Feature * pDataType )
{
  string   gmlId;
  string   klassenName;
  string   attributName;
  string   attributWert;
  string   enumerationName;
  string   enumSchluessel;
  string   enumText;
  string   stdString;
  string   textInhalt;
  string   genericAttributName;
  string   genericAttributWert;
  String ^ STR;
  String ^ STR1;
  String ^ ATTRIBUT_NAME;
	size_t    attributAnz, i, j, attributWertAnz;
	size_t    k;
  bool         success;
  ATTRIBUT_TYP attributTyp;
  StringAttr * pSimpleContent;
  StringAttr * stringAttributWert;
  DoubleAttr * doubleAttributWert;
  IntAttr    * intAttributWert;
  BoolAttr   * boolAttributWert;

  GmlKlasse          * pGmlKlasse;
  GmlAttribut        * pGmlAttribut;
  GmlDictionaryEntry * pDictionaryEntry;

  vector<GmlAttribut*>     gmlAttribute;
  vector<int>              classifierAnz;
  vector<string>           classifierNamen;
  vector<string>           classifierValues;
  vector<string>           classifierNamenLoc;
  vector<string>           classifierValuesLoc;
  vector<string>           genericAttributNamen;
  vector<StringAttr*>      stringAttributWerte;
  vector<IntAttr*>         intAttributWerte;
  vector<DoubleAttr*>      doubleAttributWerte;
  vector<DateAttr*>        dateAttributWerte;
  vector<UOM*>             uomAttributWerte;
  FeatureRelation        * pRelation;
  Feature                * pRelationZiel;
  vector<FeatureRelation*> relationen;
  vector<Feature*>         adressen;

  map<string,string>::iterator iterClassifier;

  vector<string> attributNamen;
  vector<string> attributWerte;
  vector<ATTRIBUT_TYP> attributTypen;

  GmlElement * pGmlElement;

  klassenName = pDataType->getKlassenName ();

  pGmlElement = pGmlObjekte->getGmlElement( klassenName );
  if ( pGmlElement == NULL )
  {
    if ( klassenName == "core:externalReference" )
      pGmlKlasse = pGmlObjekte->getGmlKlasse ( "core:ExternalReferenceType" );
    else
    if ( klassenName == "citygml:externalReference" )
      pGmlKlasse = pGmlObjekte->getGmlKlasse ( "citygml:ExternalReferenceType" );
    else
    if ( klassenName == "core:externalObject" )
      pGmlKlasse = pGmlObjekte->getGmlKlasse ( "core:ExternalObjectReferenceType" );
    else
    if ( klassenName == "citygml:externalObject" )
      pGmlKlasse = pGmlObjekte->getGmlKlasse ( "citygml:ExternalObjectReferenceType" );
    else
    if ( klassenName == "core:xalAddress" )
    {
              //      writeXalAddress ( pPlanObjekt );
              //      return;
    }
  }
  else
    pGmlKlasse = pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );

  if ( pGmlKlasse == NULL )
    return ;

//  index = 0;

  attributAnz = pGmlKlasse->getGmlAttribute ( gmlAttribute );
  for ( i = 0; i < attributAnz; i++ )
  {
    stringAttributWerte.clear();
    intAttributWerte.clear();
    doubleAttributWerte.clear();
    relationen.clear();

    pGmlAttribut    = gmlAttribute[i];
    attributTyp  = pGmlAttribut->getGmlAttributTyp();

    attributName = pGmlAttribut->getGmlAttributName();
    ATTRIBUT_NAME = QuConvert::ToString( attributName );
    for ( k = 0; k < level; k++ )
      ATTRIBUT_NAME = String::Concat( "-", ATTRIBUT_NAME );

    switch ( pGmlAttribut->getGmlAttributTyp() )
    {
    case _STRING:
      attributWertAnz = pDataType->getStringAttributWerte ( attributName, stringAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];

        STR1 = QuConvert::ToHtml ( stringAttributWert->value );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", ATTRIBUT_NAME, "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
      break;

    case _DATE:
    case _DATE_TIME:
      attributWertAnz = pDataType->getDateAttributWerte ( attributName, dateAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        AbstractTime * pTime = dateAttributWerte[i]->value;
        if ( pTime == NULL )
          continue;
        stdString = pTime->toString();

        STR1 = QuConvert::ToHtml ( stdString );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>",ATTRIBUT_NAME, "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
      break;

    case _DOUBLE:
    case _ANGLE:
    case _LENGTH:
    case _AREA:
    case _VOLUME:
    case _MEASURE:
      attributWertAnz = pDataType->getDoubleAttributWerte ( attributName, doubleAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        doubleAttributWert = doubleAttributWerte[j];

        STR1 = Convert::ToString( doubleAttributWert->value );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", ATTRIBUT_NAME, "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
      break;

    case _INTEGER:
      attributWertAnz = pDataType->getIntegerAttributWerte ( attributName, intAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        intAttributWert = intAttributWerte[j];

        STR1 = Convert::ToString( intAttributWert->value );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", ATTRIBUT_NAME, "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
      break;

    case _ANY_URI:
      attributWertAnz = pDataType->getURLAttributWerte ( attributName, stringAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];

        STR1 = QuConvert::ToHtml ( stringAttributWert->value );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", ATTRIBUT_NAME, "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
      break;

    case _EXTERNAL_CODE_LIST_GML3_3:
      attributWertAnz = pDataType->getStringAttributWerte ( attributName, stringAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];
        enumSchluessel =  stringAttributWert->value;
        enumText = "";

        for ( iterClassifier = stringAttributWert->classifier.begin(); iterClassifier != stringAttributWert->classifier.end(); iterClassifier++ )
        {
          if ( iterClassifier->first == "xlink:title" )
          {
            enumText =  iterClassifier->second;
            break;
          }
        }

        if ( enumText != "" )  enumSchluessel = enumText;

        STR1 = QuConvert::ToHtml ( enumSchluessel );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
      break;

    case _ENUMERATION:
    case _EXTERNAL_CODE_LIST:
      attributWertAnz = pDataType->getStringAttributWerte ( attributName, stringAttributWerte );
      enumerationName = pGmlAttribut->getGmlEnumeration();
      pDictionaryEntry = pCodeLists->getDictionaryEntry ( enumerationName );

      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];
        enumSchluessel = stringAttributWert->value;
        if ( pDictionaryEntry != NULL )
        {
          success = pDictionaryEntry->getSchluesselText ( enumSchluessel, enumText );
          if ( success ) enumSchluessel = enumText;
        }

        STR1 = QuConvert::ToHtml ( enumSchluessel );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", ATTRIBUT_NAME, "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
      break;

    case  _REFERENZ:
      break;

    case _DATA_TYPE:
      attributWertAnz = pDataType->getFeatureRelationen (attributName, relationen );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        pRelation = relationen[j];
        pRelationZiel = pFeatures->getFeature ( pRelation->featureId );

        if ( pRelationZiel != NULL )
        {
          pSimpleContent = pRelationZiel->getSimpleContent();
          if ( pSimpleContent != NULL )
          {
            STR1 = QuConvert::ToString ( pSimpleContent->value );

            stringWriter->WriteLine ( "<tr>" );
            STR = String::Concat ( "<td>",ATTRIBUT_NAME, "</td>" );
            stringWriter->WriteLine ( STR );
            STR1 = String::Concat ( "<td>", STR1, "</td>" );
            stringWriter->WriteLine ( STR1 );
            stringWriter->WriteLine ( "</tr>" );
          }
          else
          {
            stringWriter->WriteLine ( "<tr>" );
            STR = String::Concat ( "<td><strong>", ATTRIBUT_NAME, "</strong></td>" );
            stringWriter->WriteLine ( STR );
            STR = String::Concat ( "<td>", "  ", "</td>" );
            stringWriter->WriteLine ( STR );
            stringWriter->WriteLine ( "</tr>" );
            writeDataType ( level + 1, stringWriter, pRelationZiel );
          }
        }
      }
      break;

    case _BOOLEAN:
      boolAttributWert = pDataType->getBoolAttributWert ( attributName );
      if ( boolAttributWert != NULL )
      {
        if ( boolAttributWert->value == true )
          attributWert = "ja";
        else
          attributWert = "nein";

        STR1 = QuConvert::ToHtml ( attributWert );
        STR1 = String::Concat ( "<td>", STR1, "</td>" );

        stringWriter->WriteLine ( "<tr>" );
        STR = String::Concat ( "<td>", ATTRIBUT_NAME, "</td>" );
        stringWriter->WriteLine ( STR );
        stringWriter->WriteLine ( STR1 );
        stringWriter->WriteLine ( "</tr>" );
      }
      break;

    default:
      break;
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben der Adresse in die Bubble-Tabelle                          //
///////////////////////////////////////////////////////////////////////////////
void KmlWriter::writeAddress ( StringWriter ^ stringWriter, Feature * pAddress )
{
  vector<string>      vAttributNamen;
  vector<StringAttr*> vAttributWerte;
  size_t              i, anz;
  String            ^ STR;
  String            ^ GML_ATTRIBUT_NAME;
  String            ^ GML_ATTRIBUT_WERT;

  stringWriter->WriteLine ( "<tr>" );
  STR = String::Concat ( "<td>", "Adresse", "</td>" );
  stringWriter->WriteLine ( STR );
  STR = String::Concat ( "<td>", " ", "</td>" );
  stringWriter->WriteLine ( STR );
  stringWriter->WriteLine ( "</tr>" );

  anz = pAddress->getStringAttributWerte( vAttributNamen, vAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    GML_ATTRIBUT_NAME = QuConvert::ToString ( vAttributNamen[i] );
    GML_ATTRIBUT_NAME = String::Concat( "   ", GML_ATTRIBUT_NAME );
    GML_ATTRIBUT_WERT = QuConvert::ToString ( vAttributWerte[i]->value );

    stringWriter->WriteLine ( "<tr>" );
    STR = String::Concat ( "<td>", GML_ATTRIBUT_NAME, "</td>" );
    stringWriter->WriteLine ( STR );
    STR = String::Concat ( "<td>", GML_ATTRIBUT_WERT, "</td>" );
    stringWriter->WriteLine ( STR );
    stringWriter->WriteLine ( "</tr>" );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Geometriedaten eines Features                               //
///////////////////////////////////////////////////////////////////////////////
void KmlWriter::writeFeatureGeometry ( Feature * pFeature )
{
  vector<_Geometrie*> vGeometrie;
  _Geometrie        * pGeometrie;
  _Surface          * pSurface;
  _Curve            * pCurve;
  _Solid            * pSolid;
  ImplicitGeometry  * pImplicitGeometry;
  GeometricComplex  * pGeometricComplex;
  PolygonGeo           * pPolygon;
  MultiCurve        * pMultiCurve;
  MultiSurface      * pMultiSurface;
  MultiSolid        * pMultiSolid;
	size_t              i, j, anz;
  vector<PolygonGeo*> vPolygone;
  vector<bool>        vOrientations;
  vector<_Curve*>     vCurves;
  vector<PolygonGeo*> vPolygoneImpl;
  vector<bool>        vOrientationsImpl;
  vector<_Curve*>     vCurvesImpl;
  bool                success;
  string              srsName;

  anz = pFeature->getGeometry( vGeometrie, lodStufe );
  for ( i = 0; i < anz; i++ )
  {
    pGeometrie = vGeometrie[i];
    srsName    = pGeometrie->getSrsName();

    switch ( pGeometrie->getAbstractGeometryType() )
    {
    case ABSTRACT_SURFACE:
      pSurface = dynamic_cast<_Surface*>(pGeometrie);
      pSurface->getPolygone( vPolygone, vOrientations );
      break;

    case ABSTRACT_CURVE:
      pCurve = dynamic_cast<_Curve*>(pGeometrie);
      vCurves.push_back( pCurve );
      break;

    case ABSTRACT_SOLID:
      pSolid = dynamic_cast<_Solid*>(pGeometrie);
      pSolid->getAussenkonturPolygone( vPolygone, vOrientations );
      break;

    case ABSTRACT_IMPLICIT_GEOMETRY:
      pImplicitGeometry = dynamic_cast<ImplicitGeometry*>(pGeometrie);
      pImplicitGeometry->getExplicitGeometry( vCurvesImpl, vPolygoneImpl, vOrientationsImpl );
//      pViewer->writeLogFile( "Implizite Geometrie wird noch nicht unterstützt" );
      break;

    case ABSTRACT_MULTI_GEOMETRY:
      {
        switch ( pGeometrie->getGeometryType() )
        {
        case MULTI_CURVE:
          pMultiCurve = dynamic_cast<MultiCurve*>(pGeometrie);
          for ( i = 0; i < pMultiCurve->getCurveMemberAnz(); i++ )
          {
            pCurve = pMultiCurve->getCurve( i );
            vCurves.push_back( pCurve );
          }
          break;

        case MULTI_SURFACE:
          pMultiSurface =  dynamic_cast<MultiSurface*>(pGeometrie);
          pMultiSurface->getPolygone( vPolygone, vOrientations );
          break;

        case MULTI_SOLID:
          pMultiSolid = dynamic_cast<MultiSolid*>(pGeometrie);
          for ( i = 0; i < pMultiSolid->getSolidMemberAnz(); i++ )
          {
            pSolid = pMultiSolid->getSolid( i );
           pSolid->getAussenkonturPolygone( vPolygone, vOrientations );
          }
          break;
        }
        break;
      }

    case ABSTRACT_GEOMETRIC_COMPLEX:
      {
        pGeometricComplex = dynamic_cast<GeometricComplex*>(pGeometrie);
        for ( i = 0; i < pGeometricComplex->getGeometryMemberAnz(); i++ )
        {
          pGeometrie = pGeometricComplex->getGeometry( i );
          switch ( pGeometrie->getAbstractGeometryType() )
          {
          case ABSTRACT_SURFACE:
            pSurface = dynamic_cast<_Surface*>(pGeometrie);
            pSurface->getPolygone( vPolygone, vOrientations );
            break;

          case ABSTRACT_CURVE:
            pCurve = dynamic_cast<_Curve*>(pGeometrie);
            vCurves.push_back( pCurve );
            break;

          case ABSTRACT_SOLID:
            pSolid = dynamic_cast<_Solid*>(pGeometrie);
            pSolid->getAussenkonturPolygone( vPolygone, vOrientations );
            break;
          }
        }
      }
    }
  }

  if (  vPolygone.size() == 0 && vCurves.size() == 0 && vPolygoneImpl.size() == 0 && vCurvesImpl.size() == 0 ) 
    return;

  writer->WriteStartElement( "MultiGeometry" );

  for ( i = 0; i < vPolygone.size(); i++ )
  {
    pPolygon  = vPolygone[i];
    success = writePolygon ( pFeature, pPolygon,srsName );
    if ( !success )
      break;
  }

  for ( i = 0; i < vCurves.size(); i++ )
  {
    pCurve = vCurves[i];
    success = writeCurve (  pFeature, pCurve,srsName );
    if ( !success )
      break;
  }

  for ( i = 0; i < vPolygoneImpl.size(); i++ )
  {
    pPolygon  = vPolygoneImpl[i];
    success = writePolygon ( pFeature,  pPolygon,srsName );
    if ( !success )
    {
      for ( j = i+1; j < vPolygoneImpl.size(); j++ )
        delete vPolygoneImpl[j];
      break;
    }

    delete pPolygon;
  }

  for ( i = 0; i < vCurvesImpl.size(); i++ )
  {
    pCurve = vCurvesImpl[i];
    success = writeCurve (  pFeature, pCurve, srsName );
    if ( !success )
    {
      for ( j = i+1; j < vCurvesImpl.size(); j++ )
        delete vCurvesImpl[j];
      break;
    }

    delete pCurve;
  }

  writer->WriteEndElement();

}

///////////////////////////////////////////////////////////////////////////////
//  Schrteibt ein Polygon                                                    //
///////////////////////////////////////////////////////////////////////////////
bool KmlWriter::writePolygon ( Feature * pFeature, PolygonGeo * pPolygon, string srsNameP )
{
  string         stdString;
  String       ^ STR;
  _Ring        * pRing;
  vector<double> vX, vY, vZ;
  double         p[3], pTrans[3];
  unsigned int   i, iLoch;
  string         srsName;
  bool           success;

  srsName = pPolygon->getSrsName();
  if ( srsName == "" )
  {
    srsName = srsNameP;
    if ( srsName == "" )
      srsName = pFeature->getSrsName();
  }

  if ( srsName =="" )
  {
    pLogfile->writeLogFile( "Fehlendes  Koordinaten-Referenzsystem" );
    return false;
  }

  writer->WriteStartElement( "Polygon");

  stdString = pPolygon->getGmlId();
  if ( stdString != "" )
    writer->WriteAttributeString( "id", QuConvert::ToString( stdString ) );
  writer->WriteElementString( "altitudeMode", "absolute" );

  writer->WriteStartElement( "outerBoundaryIs" );
  writer->WriteStartElement( "LinearRing" );
  writer->WriteStartElement( "coordinates" );

  pRing =  pPolygon->getAussenkontur();
  pRing->getPoints3D( vX, vY, vZ );
  for ( i = 0; i < vX.size(); i++ )
  {
    p[0] = vX[i];
    p[1] = vY[i];
    p[2] = vZ[i] + zOffset;
    success = transformToLaengeBreite ( srsName, p, pTrans );
    if ( success )
    {
      STR =  String::Concat( XmlConvert::ToString( pTrans[0]), ",", XmlConvert::ToString( pTrans[1]), ",", XmlConvert::ToString( pTrans[2]), " \n"  );
      writer->WriteString( STR );
    }
    else
      pLogfile->writeLogFile( "Unbekanntes Koordinaten-Referenzsystem" );
  }

  writer->WriteEndElement();
  writer->WriteEndElement();
  writer->WriteEndElement();

  for ( iLoch = 0; iLoch < pPolygon->getLochAnz(); iLoch++ )
  {
    pRing = pPolygon->getLoch( iLoch );
    vX.clear();
    vY.clear();
    vZ.clear();

    writer->WriteStartElement( "innerBoundaryIs" );
    writer->WriteStartElement( "LinearRing" );
    writer->WriteStartElement( "coordinates" );

    pRing->getPoints3D( vX, vY, vZ );
    for ( i = 0; i < vX.size(); i++ )
    {
      p[0] = vX[i];
      p[1] = vY[i];
      p[2] = vZ[i] + zOffset;
      success = transformToLaengeBreite ( srsName, p, pTrans );
      if ( success )
      {
        STR =  String::Concat( XmlConvert::ToString( pTrans[0]),  ",", XmlConvert::ToString( pTrans[1]), ",", XmlConvert::ToString( pTrans[2]), " \n" );
        writer->WriteString( STR );
      }
      else
        pLogfile->writeLogFile( "Unbekanntes Koordinaten-Referenzsystem" );
    }

    writer->WriteEndElement();
    writer->WriteEndElement();
    writer->WriteEndElement();
  }

  writer->WriteEndElement();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt eine Curve                                                      //
///////////////////////////////////////////////////////////////////////////////
bool KmlWriter::writeCurve ( Feature * pFeature, _Curve * pCurve, string srsNameP )
{
  string         stdString;
  String       ^ STR;
  vector<double> vX, vY, vZ;
  double         p[3], pTrans[3];
  unsigned int   i;
  string         srsName;
  bool           success;

  srsName = pCurve->getSrsName();
  if ( srsName == "" )
  {
    srsName = srsNameP;
    if ( srsName == "" )
      srsName = pFeature->getSrsName();
  }

  if ( srsName == "" )
  {
    pLogfile->writeLogFile( "Fehlendes Koordinaten-Referenzsystem" );
    return false;
  }

  writer->WriteStartElement( "LineString");

  stdString = pCurve->getGmlId();
  if ( stdString != "" )
    writer->WriteAttributeString( "id", QuConvert::ToString( stdString ) );
  writer->WriteElementString( "altitudeMode", "absolute" );

  writer->WriteStartElement( "coordinates" );

  pCurve->getPoints3D( vX, vY, vZ );
  for ( i = 0; i < vX.size(); i++ )
  {
    p[0] = vX[i];
    p[1] = vY[i];
    p[2] = vZ[i] + zOffset;
    success = transformToLaengeBreite ( srsName, p, pTrans );
    if ( success )
    {
      STR =  String::Concat( XmlConvert::ToString( pTrans[0]), ",", XmlConvert::ToString( pTrans[1]), ",", XmlConvert::ToString( pTrans[2]), " \n"  );
      writer->WriteString( STR );
    }
    else
      pLogfile->writeLogFile( "Unbekanntes Koordinaten-Referenzsystem" );
  }
  writer->WriteEndElement();
  writer->WriteEndElement();
  
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert den Punkt fromCoord im projiziertem Koordinatens<ystem     //
//  in das Länge/Breite System (toCoord)                                     //
///////////////////////////////////////////////////////////////////////////////
bool KmlWriter::transformToLaengeBreite ( string srsNameShort, double fromCoord[3], double toCoord[3] )
{
  bool   success;
  GeoPoint fromPoint ( fromCoord[0], fromCoord[1], fromCoord[2] );
  GeoPoint toPoint;

  success = pSrsProjection->convertSrs2LatLon( srsNameShort, &fromPoint, &toPoint );
  if ( success )
  {
    toCoord[0] = toPoint.getX();
    toCoord[1] = toPoint.getY();
    toCoord[2] = fromCoord[2];
  }
  return success;
}





