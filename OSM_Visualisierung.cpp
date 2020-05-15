#include "stdafx.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;

#include <vector>
#include <map>
#include <string>
#include <set>
#include <sstream>

using namespace std;

#include "Convert.h"
#include "Constant.h"
#include "Osm.h"
#include "Geometrie.h"
#include "SvgVisualisierung.h"
#include "OSM_Visualisierung.h"
#include "SrsManager.h"

///////////////////////////////////////////////////////////////////////////////
//  class OSM_Visual					                            			      			 //
//  Basisklasse für die svg-Visuasisierungs Parameter eines OSM Modells      //
///////////////////////////////////////////////////////////////////////////////
OSM_Visual::OSM_Visual ( OSM_VisualParams * pParamsP )
{
  pParams = pParamsP;

  osmClassification	          = "";
  selectAttribut	            = "";
}

OSM_Visual::~ OSM_Visual()
{
  unsigned int i;
  string       id;

  for ( i = 0; i < vFlaechenDarstellungen.size(); i++ )
  {
    OSM_FlaechenDarstellung * pFlaechenDarstellung = vFlaechenDarstellungen[i];
    delete pFlaechenDarstellung;
  }

  for ( i = 0; i < vLinienDarstellungen.size(); i++ )
  {
    OSM_LinienDarstellung * pLinienDarstellung = vLinienDarstellungen[i];
    delete pLinienDarstellung;
  }

  for ( i = 0; i < vPunktDarstellungen.size(); i++ )
    delete vPunktDarstellungen[i];

  for ( i = 0; i < vRelationDarstellungen.size(); i++ )
    delete vRelationDarstellungen[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Darstellung hinzu									                        				 //
///////////////////////////////////////////////////////////////////////////////
void OSM_Visual::addDarstellung ( OSM_Darstellung * pDarstellungP )
{
  switch ( pDarstellungP->getDarstellungTyp() )
  {
  case OSM_Darstellung::FLAECHE:
    vFlaechenDarstellungen.push_back ( (OSM_FlaechenDarstellung*)pDarstellungP );
    break;

  case OSM_Darstellung::LINIE:
    vLinienDarstellungen.push_back ( (OSM_LinienDarstellung*)pDarstellungP );
    break;

  case OSM_Darstellung::PUNKT:
    vPunktDarstellungen.push_back ( (OSM_PunktDarstellung*)pDarstellungP );
    break;

  case OSM_Darstellung::RELATION:
    vRelationDarstellungen.push_back ( (OSM_RelationDarstellung*)pDarstellungP );
    break;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die selektierte Darstellung                                      //
///////////////////////////////////////////////////////////////////////////////
OSM_Darstellung *  OSM_Visual::getDarstellungSelected ( OSM_Object * pObj )
{
  string            errorText;
  string            selectAttributWert = "";
	size_t             i, anz;
  OSM_Darstellung   * pDarstellung;
  OSM_Darstellung   * pDarstellungSelected = NULL;


  vector<OSM_Darstellung*> vDarstellungen;

  if ( selectAttribut != "" )
    selectAttributWert = pObj->getTagValue( selectAttribut );
  else
    selectAttributWert = "_IIII";

  switch ( pObj->getType() )
  {
  case   OSM_AREA :
    anz = getFlaechenDarstellungen ( vDarstellungen );
    break;

  case OSM_WAY:
    anz = getLinienDarstellungen( vDarstellungen );
    break;

  case OSM_NODE:
    anz = getPunktDarstellungen( vDarstellungen);
    break;

  case OSM_RELATION:
  {
    string type = pObj->getTagValue( "type" );
    OSM_RelationDarstellung * pRelDarstellung = getRelationDarstellung( type );
    if ( pRelDarstellung != NULL )
    {
       adaptRelationDarstellung ( pRelDarstellung, selectAttributWert );
       return pRelDarstellung;
    }
    else
      return NULL; 
  }

  default:
    anz = 0;
  }

  for ( i = 0; i < anz; i++ )
  {
    pDarstellung = vDarstellungen[i];
    if ( pDarstellung->isSelected ( selectAttributWert ) )
    {
      pDarstellungSelected = pDarstellung;
      break;
    }
  } 

  return pDarstellungSelected;
}

void OSM_Visual::adaptRelationDarstellung ( OSM_RelationDarstellung * pRelationDarstellung, std::string selectAttributWert )
{
  unsigned int i;

  for ( i = 0; i < vFlaechenDarstellungen.size(); i++ )
  {
    OSM_FlaechenDarstellung * pFlaechenDarstellung = vFlaechenDarstellungen[i];
    if ( pFlaechenDarstellung->isSelected( selectAttributWert ) )
    {
      pRelationDarstellung->setFlaechenDarstellung( pFlaechenDarstellung );
      break;
    }
  }

  for ( i = 0; i < vLinienDarstellungen.size(); i++ )
  {
    OSM_LinienDarstellung * pLinienDarstellung = vLinienDarstellungen[i];
    if ( pLinienDarstellung->isSelected( selectAttributWert ) )
    {
      pRelationDarstellung->setLinienDarstellung( pLinienDarstellung );
      break;
    }
  }

  for ( i = 0; i < vPunktDarstellungen.size(); i++ )
  {
    OSM_PunktDarstellung * pPunktDarstellung = vPunktDarstellungen[i];
    if ( pPunktDarstellung->isSelected( selectAttributWert ) )
    {
      pRelationDarstellung->setPunktDarstellung( pPunktDarstellung );
      break;
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert Darstellungen		                          											 //
///////////////////////////////////////////////////////////////////////////////
size_t OSM_Visual::getFlaechenDarstellungen ( vector<OSM_Darstellung*> &vFlaechenDarstellungenP )
{
  unsigned int i;

  for ( i = 0; i < vFlaechenDarstellungen.size(); i++ )
    vFlaechenDarstellungenP.push_back ( vFlaechenDarstellungen[i] );

  return vFlaechenDarstellungen.size();
}

size_t OSM_Visual::getLinienDarstellungen ( vector<OSM_Darstellung*> &vLinienDarstellungenP )
{
  unsigned int i;

  for ( i = 0; i < vLinienDarstellungen.size(); i++ )
    vLinienDarstellungenP.push_back ( vLinienDarstellungen[i] );

  return vLinienDarstellungen.size();
}


size_t OSM_Visual::getPunktDarstellungen ( vector<OSM_Darstellung*> &vPunktDarstellungenP )
{
  unsigned int i;

  for ( i = 0; i < vPunktDarstellungen.size(); i++ )
    vPunktDarstellungenP.push_back ( vPunktDarstellungen[i] );

  return vPunktDarstellungen.size();
}

OSM_RelationDarstellung * OSM_Visual::getRelationDarstellung ( string relationTypeP )
{
  unsigned int i;

  for ( i = 0; i < vRelationDarstellungen.size(); i++ )
  {
    OSM_RelationDarstellung * pRelDarstellung = vRelationDarstellungen[i];
    string                    type = pRelDarstellung->getType();

    if ( type == relationTypeP )
      return pRelDarstellung;
  }
  return NULL;
}


size_t OSM_Visual::getDarstellungen ( vector<OSM_Darstellung*> &vDarstellungenP )
{
	size_t i;

  for ( i = 0; i < vFlaechenDarstellungen.size(); i++ )
    vDarstellungenP.push_back ( vFlaechenDarstellungen[i] );

  for ( i = 0; i < vLinienDarstellungen.size(); i++ )
    vDarstellungenP.push_back ( vLinienDarstellungen[i] );

  for ( i = 0; i < vPunktDarstellungen.size(); i++ )
    vDarstellungenP.push_back ( vPunktDarstellungen[i] );

  for ( i = 0; i < vRelationDarstellungen.size(); i++ )
    vDarstellungenP.push_back ( vRelationDarstellungen[i] );

  return vDarstellungenP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  class OSM_Darstellung		                         												 //
//  Basisklasse für die SVG-Visualisierungsparameter Klassen	        			 //
///////////////////////////////////////////////////////////////////////////////
OSM_Darstellung::OSM_Darstellung()
{
  priority = 10;

}

OSM_Darstellung::~OSM_Darstellung()
{

}

size_t OSM_Darstellung::getSelectAttributWerte ( std::vector<std::string> & vSelectAttribut1WertP )
{
  unsigned int i;

  for ( i = 0; i < vSelectAttributWert.size(); i++ )
    vSelectAttribut1WertP.push_back( vSelectAttributWert[i] );

  return vSelectAttribut1WertP.size();
}


bool OSM_Darstellung::isSelected ( std::string selectAttributWertP )
{
  bool         found = false;
  unsigned int i;
  string       attributWertLoc;

  if ( selectAttributWertP == "_IIII" )
    found = true;
  else
  for ( i = 0; i < vSelectAttributWert.size(); i++ )
  {
    attributWertLoc = vSelectAttributWert[i];
    if ( attributWertLoc == "IIII"									                ||
        ( attributWertLoc == "BBBB" && selectAttributWertP != "" ) ||
        ( attributWertLoc == "NNNN" && selectAttributWertP == "" ) ||
        attributWertLoc == selectAttributWertP 
        )
    {
      found = true;
      break;
    }
  }

  return found;
}

///////////////////////////////////////////////////////////////////////////////
//  class FlaechenDarstellung									                        			 //
//  Visualisierungs-Parameter einer Fläche (geschl. OSM-way)   							 //
///////////////////////////////////////////////////////////////////////////////
OSM_FlaechenDarstellung::OSM_FlaechenDarstellung(): OSM_Darstellung()
{

}

OSM_FlaechenDarstellung::~OSM_FlaechenDarstellung()
{
  unsigned int i;

  for ( i = 0; i < vFlaechenParams.size(); i++ )
    delete vFlaechenParams[i];
}

size_t OSM_FlaechenDarstellung::getFlaechenParams ( std::vector<OSM_FlaechenParams*> & vFlaechenParamsP )
{
  unsigned int i;

  for ( i = 0; i < vFlaechenParams.size(); i++ )
    vFlaechenParamsP.push_back( vFlaechenParams[i] );

  return vFlaechenParamsP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  class OSM_LinienDarstellung			 			                           				 //
//  Visualisierungs-Parameter einer Linie (OSM-way)	                  			 //
///////////////////////////////////////////////////////////////////////////////
OSM_LinienDarstellung::OSM_LinienDarstellung()
{
  pLineSymbolParams = NULL;
}

OSM_LinienDarstellung::~OSM_LinienDarstellung()
{
  unsigned int i;

  for ( i = 0; i < vLinienParams.size(); i++ )
    delete vLinienParams[i];
}

size_t OSM_LinienDarstellung::getLinienParams ( std::vector<OSM_LinienParams*> & vLinienParamsP )
{
  unsigned int i;

  for ( i = 0; i < vLinienParams.size(); i++ )
    vLinienParamsP.push_back( vLinienParams[i] );

  return vLinienParamsP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  class OSM_PunktDarstellung								                        			 //
//  Visualisierungs-Parameter eines Punktes (OSM-node)               				 //
///////////////////////////////////////////////////////////////////////////////
OSM_PunktDarstellung::OSM_PunktDarstellung()
{
  attributA      = "";
  attributB      = "";
  textStyle      = "";
  angleAttribute = "";
  symbol         = "";
  symbolSize     = 1.0;
  textOffsetX    = 0.0;
  textOffsetY    = 0.0;
}

OSM_PunktDarstellung::~OSM_PunktDarstellung()
{

}

///////////////////////////////////////////////////////////////////////////////
//  class OSM_RelationDarstellung                                            //
///////////////////////////////////////////////////////////////////////////////
OSM_RelationDarstellung::OSM_RelationDarstellung()
{
  pFlaechenDarstellung = NULL;
  pLinienDarstellung   = NULL;
  pPunktDarstelung     = NULL;

  type = "";
}

OSM_RelationDarstellung::~OSM_RelationDarstellung()
{
}

///////////////////////////////////////////////////////////////////////////////
//  class OSM_FlaechenParams				                          							 //
//  Klasse für die svg-Visualisierungs Parameter einer OSM-Fläche	        	 //
///////////////////////////////////////////////////////////////////////////////
OSM_FlaechenParams::OSM_FlaechenParams ()
{
  classId    = "";
//  patternId  = "";
//  hatchingId = "";
//  clip       = false;;
}

OSM_FlaechenParams::~ OSM_FlaechenParams()
{

}

///////////////////////////////////////////////////////////////////////////////
//  class OSM_LinienParams  		                            								 //
//  Klasse für die svg-Visualisierungs Parameter einer OSM-Linie     				 //
///////////////////////////////////////////////////////////////////////////////
OSM_LinienParams::OSM_LinienParams ()
{
  classId = "";
}

OSM_LinienParams::~OSM_LinienParams()
{

}

///////////////////////////////////////////////////////////////////////////////
//  class OSM_VisualParams  											                        	 //
///////////////////////////////////////////////////////////////////////////////
OSM_VisualParams::OSM_VisualParams ()
{
  breiteUeberschrift = 800;
  hoeheUeberschrift  = 30;
  breiteDisplay      = 0.3f;
  hoeheDisplay       = 0.2f;

  showAllSurfaces = false;
  showAllLines    = false;
  showAllNodes    = false;

  pDefaultPunktDarstellung    = NULL;
  pDefaultFlaechenDarstellung = NULL;
  pDefaultLinienDarstellung   = NULL;

}

OSM_VisualParams::~OSM_VisualParams()
{
  map<string,OSM_Visual*>::iterator              iterVisual;
  map<string,Style*>::iterator			             iterStyles;
  map<string,Symbol*>::iterator			             iterSymbols;

  for ( iterVisual = mVisual.begin(); iterVisual != mVisual.end(); iterVisual++ )
    delete iterVisual->second;

  for ( iterStyles = mStyles.begin(); iterStyles != mStyles.end(); iterStyles++ )
    delete iterStyles->second;

  for ( iterSymbols = mSymbols.begin(); iterSymbols != mSymbols.end(); iterSymbols++ )
    delete iterSymbols->second;

  if ( pDefaultFlaechenDarstellung != NULL ) delete pDefaultFlaechenDarstellung;
  if ( pDefaultLinienDarstellung != NULL )   delete pDefaultLinienDarstellung;
  if ( pDefaultPunktDarstellung != NULL )    delete pDefaultPunktDarstellung;
}

void OSM_VisualParams::addOSM_Visual ( OSM_Visual * pVisual )
{
  string id = pVisual->getOSMClassification();
  mVisual[id] = pVisual;
}

void OSM_VisualParams::addStyle ( Style * pStyle )
{
  string id = pStyle->getStyleId();
  mStyles[id] = pStyle;
}

void OSM_VisualParams::addSymbol ( Symbol * pSymbol )
{
  string id = pSymbol->getSymbolId();
  mSymbols[id] = pSymbol;
}

OSM_Visual * OSM_VisualParams::getOSM_Visual ( std::string featureType )
{
  map<string,OSM_Visual*>::iterator iter;

  iter = mVisual.find( featureType );
  if ( iter != mVisual.end() )
    return iter->second;
  else
    return NULL;
}

Style * OSM_VisualParams::getStyle ( std::string styleId )
{
  map<string,Style*>::iterator iter;

  iter = mStyles.find( styleId );
  if ( iter != mStyles.end() )
    return iter->second;
  else
    return NULL;
}

Symbol * OSM_VisualParams::getSymbol ( std::string symbolId )
{
  map<string,Symbol*>::iterator iter;

  iter = mSymbols.find( symbolId );
  if ( iter != mSymbols.end() )
    return iter->second;
  else
    return NULL;
}

size_t OSM_VisualParams:: getOSM_VisualAll ( std::vector<OSM_Visual*> & vVisual )
{
  map<string,OSM_Visual*>::iterator iter;

  for ( iter = mVisual.begin(); iter != mVisual.end(); iter++ )
    vVisual.push_back( iter->second );

  return vVisual.size();
}

size_t OSM_VisualParams::getSymbols ( std::vector<Symbol*> &vSymbolsP )
{
  map<string,Symbol*>::iterator iter;

  for ( iter = mSymbols.begin(); iter != mSymbols.end(); iter++ )
    vSymbolsP.push_back( iter->second );

  return vSymbolsP.size();
}

size_t OSM_VisualParams::getStyles ( std::vector<Style*> &vStylesP )
{
  map<string,Style*>::iterator iter;

  for ( iter = mStyles.begin(); iter != mStyles.end(); iter++ )
    vStylesP.push_back( iter->second );

  return vStylesP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob es zu einem OSM-Objekt eine Darstellungsvorschrift gibt.    //
//  Geliefert wird die OSM-Klassifikation des Objektes, ergänzt um den       //
//  OSM-Typ, sowie die Priorität der Visualisierungsvorschrift               //
///////////////////////////////////////////////////////////////////////////////
OSM_Darstellung * OSM_VisualParams::getVisualization ( OSM_Object * pObj, std::string & classification, int & priority )
{
  OSM_Visual      * pVisul;
  OSM_Darstellung * pDarstellung = NULL;

  priority       = -1;
  classification = pObj->getClassification();
  if (  classification != "" )
  {
    pVisul = getOSM_Visual ( classification );
    if ( pVisul != NULL )
      pDarstellung = pVisul->getDarstellungSelected( pObj );
  }

  if ( pDarstellung == NULL )
  {
    switch ( pObj->getType() )
    {
    case OSM_WAY:
      if ( showAllLines ) 
      {
        priority = pDefaultLinienDarstellung->getPriority();
        classification = "_default_Line";
        return pDefaultLinienDarstellung;
      }
      else
        return NULL;

    case OSM_AREA:
      if ( showAllSurfaces )
      {
        priority = pDefaultFlaechenDarstellung->getPriority();
        classification = "_default_Area";
        return pDefaultFlaechenDarstellung;
      }
      else
        return NULL;

    case OSM_NODE:
      if ( showAllNodes )
      {
        priority = pDefaultPunktDarstellung->getPriority();
        classification = "_default_Node";
        return pDefaultPunktDarstellung;
      }
      else
        return NULL;

    default:
      return NULL;
    }    

    return NULL;
  }

  classification = pObj->getClassificationWithGeometry();
  priority = pDarstellung->getPriority();

  return pDarstellung;
}

///////////////////////////////////////////////////////////////////////////////
//  __gc class OSM_SvgParamReader						                     						 //
///////////////////////////////////////////////////////////////////////////////
OSM_SvgParamReader::OSM_SvgParamReader ( double globalScaleP )
{
  pOSM_SvgParams = NULL;
  globalScale    = globalScaleP;
}

OSM_SvgParamReader::~OSM_SvgParamReader()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen						                                										 //
///////////////////////////////////////////////////////////////////////////////
bool OSM_SvgParamReader::read( string fileName, OSM_VisualParams * pOSM_SvgParamsP  )
{
  String ^ fName;
  String ^ STR;
  string   stdString;
  double   d;
  bool     first = true;

  pOSM_SvgParams = pOSM_SvgParamsP;
  fName          = QuConvert::ToString ( fileName );
  reader         = gcnew XmlTextReader( fName );

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;
      if ( first )
      {
        first = false;
        if ( pActElement->CompareTo( "OSM_SvgVisualisierung" ) != 0 )
        {
          System::Windows::Forms::MessageBox::Show( "Ungültige Steuerdatei für OSM-Visualisierung");
          break;
        }
        else
        {          
          STR = reader->GetAttribute ( "datum" );
          if ( STR != nullptr )
          {
            QuConvert::systemStr2stdStr ( stdString, STR );
            pOSM_SvgParams->setDatum ( stdString );
          }

          STR = reader->GetAttribute( "breiteUeberschrift" );
          if ( STR != nullptr )
          {
            d = XmlConvert::ToDouble( STR );
            pOSM_SvgParams->setBreiteUeberschrift( d );
          }

          STR = reader->GetAttribute( "hoeheUeberschrift" );
          if ( STR != nullptr )
          {
            d = XmlConvert::ToDouble( STR );
            pOSM_SvgParams->setHoeheUeberschrift( d );
          }

          STR = reader->GetAttribute( "breiteDisplay" );
          if ( STR != nullptr )
          {
            d = XmlConvert::ToDouble( STR );
            pOSM_SvgParams->setBreiteDisplay( d );
          }

          STR = reader->GetAttribute( "hoeheDisplay" );
          if ( STR != nullptr )
          {
            d = XmlConvert::ToDouble( STR );
            pOSM_SvgParams->setHoeheDisplay( d );
          }

          STR = reader->GetAttribute( "showAllSurfaces" );
          if ( STR != nullptr )
          {
            if ( STR->CompareTo( "true" ) == 0 )
              pOSM_SvgParams->setShowAllSurfaces( true );
            else
              pOSM_SvgParams->setShowAllSurfaces( false );
          }

          STR = reader->GetAttribute( "showAllLines" );
          if ( STR != nullptr )
          {
            if ( STR->CompareTo( "true" ) == 0 )
              pOSM_SvgParams->setShowAllLines( true );
            else
              pOSM_SvgParams->setShowAllLines( false );
          }

          STR = reader->GetAttribute( "showAllNodes" );
          if ( STR != nullptr )
          {
            if ( STR->CompareTo( "true" ) == 0 )
              pOSM_SvgParams->setShowAllNodes( true );
            else
              pOSM_SvgParams->setShowAllNodes( false );
          }

        }
      }

      if ( pActElement->CompareTo("OSMVisual") == 0 )
        readOSM_Visual();
      else
      if ( pActElement->CompareTo("Style") == 0 )
        readStyle();
      else
      if ( pActElement->CompareTo("Symbol") == 0 )
        readSymbol();
      else
      if ( pActElement->CompareTo("DefaultFlaechenDarstellung") == 0 )
      {
        OSM_FlaechenDarstellung * pFlaechenDarstellung = readFlaechenDarstellung ( "DefaultFlaechenDarstellung");
        pOSM_SvgParams->setDefaultFlaechenDarstellung( pFlaechenDarstellung );
      }
      else
      if ( pActElement->CompareTo("DefaultLinienDarstellung") == 0 )
      {
        OSM_LinienDarstellung * pLinienDarstellung = readLinienDarstellung ( "DefaultLinienDarstellung");
        pOSM_SvgParams->setDefaultLinienDarstellung( pLinienDarstellung );
      }
      else
      if ( pActElement->CompareTo("DefaultPunktDarstellung") == 0 )
      {
        OSM_PunktDarstellung * pPunktDarstellung = readPunktDarstellung ( "DefaultPunktDarstellung");
        pOSM_SvgParams->setDefaultPunktDarstellung( pPunktDarstellung );
      }
    }
    else
    {
      if ( reader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if ( pActElement->CompareTo( "OSM_SvgVisualisierung" ) == 0 ) break;
      }
    }
  }

  reader->Close();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Visualisierungspapameter für eine OSM-Klassifikation  			 //
///////////////////////////////////////////////////////////////////////////////
void OSM_SvgParamReader::readOSM_Visual()
{
  String ^ STR;
  string   stdString;
  string   classification;
  string   id;

  OSM_Visual * pOSM_Visual = new OSM_Visual( pOSM_SvgParams );

  STR = reader->GetAttribute ( "osmClassification" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( classification, STR );
    pOSM_Visual->setOSMClassification ( classification );
    pOSM_SvgParams->addOSM_Visual ( pOSM_Visual );
  }

  STR = reader->GetAttribute ( "selectAttribut" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pOSM_Visual->setSelectAttribut ( stdString );
  }

  if ( reader->IsEmptyElement )
    return;

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;
      if ( pActElement->CompareTo ( "FlaechenDarstellung") == 0 )
      {
        OSM_FlaechenDarstellung * pFlaechenDarstellung = readFlaechenDarstellung ( "FlaechenDarstellung");
        pOSM_Visual->addDarstellung ( pFlaechenDarstellung );
      }
      else
      if ( pActElement->CompareTo ( "LinienDarstellung" ) == 0 )
      {
        OSM_LinienDarstellung * pLinienDarstellung = readLinienDarstellung ( "LinienDarstellung" );
        pOSM_Visual->addDarstellung ( pLinienDarstellung );
      }
      else
      if ( pActElement->CompareTo ( "PunktDarstellung" ) == 0 )
      {
        OSM_PunktDarstellung * pPunktDarstellung = readPunktDarstellung ( "PunktDarstellung" );
        pOSM_Visual->addDarstellung ( pPunktDarstellung );
      }
      else
        if ( pActElement->CompareTo ( "RelationDarstellung" ) == 0 )
        {
          OSM_RelationDarstellung * pRelationDarstellung = readRelationDarstellung ( "RelationDarstellung" );
          pOSM_Visual->addDarstellung ( pRelationDarstellung );
        }
    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = reader->LocalName;
      if (pActElement->CompareTo("OSMVisual") == 0 ) break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Parameter für eine Relations-Darstellung            				 //
///////////////////////////////////////////////////////////////////////////////
OSM_RelationDarstellung * OSM_SvgParamReader::readRelationDarstellung ( String ^ tag )
{
  String ^ STR;
  string   stdString;
  int      intValue;

  OSM_RelationDarstellung * pRelationDarstellung = new OSM_RelationDarstellung;

  STR = reader->GetAttribute( "priority" );
  if ( STR != nullptr )
  {
    intValue = XmlConvert::ToInt32 ( STR );
    pRelationDarstellung->setPriority( intValue );
  }

  STR = reader->GetAttribute( "type" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr( stdString, STR );
    pRelationDarstellung->setType( stdString );
  }

  if ( reader->IsEmptyElement ) return pRelationDarstellung;

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("SelectAttributWert") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pRelationDarstellung->addSelectAttributWert ( stdString );
        }
      }     
    }
  }

  return pRelationDarstellung;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Parameter für eine Flächendarstellung               				 //
///////////////////////////////////////////////////////////////////////////////
OSM_FlaechenDarstellung * OSM_SvgParamReader::readFlaechenDarstellung ( String ^ tag  )
{
  String ^ STR;
  string   stdString;
  int      intValue;

  OSM_FlaechenDarstellung * pFlaechenDarstellung = new OSM_FlaechenDarstellung;

/*  STR = reader->GetAttribute ( "default" );
  if ( STR != nullptr )
  {
    boolValue = XmlConvert::ToBoolean ( STR );
    pFlaechenDarstellung->setDefault ( boolValue );
  }
*/
  STR = reader->GetAttribute( "priority" );
  if ( STR != nullptr )
  {
    intValue = XmlConvert::ToInt32 ( STR );
    pFlaechenDarstellung->setPriority( intValue );
  }

/*  STR = reader->GetAttribute ( "deckend" );
  if ( STR != nullptr )
  {
    boolValue = XmlConvert::ToBoolean ( STR );
    pFlaechenDarstellung->setDeckend ( boolValue );
  }
*/

  if ( reader->IsEmptyElement ) return pFlaechenDarstellung;

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("SelectAttributWert") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pFlaechenDarstellung->addSelectAttributWert ( stdString );
        }
      }
      else    
      if ( pActElement->CompareTo("FlaechenParams") == 0 )
        readFlaechenParams ( pFlaechenDarstellung );     
    }
    else
    {
      if ( reader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if (pActElement->CompareTo( tag ) == 0 ) break;
      }
    }
  }
  return pFlaechenDarstellung;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Tag FlaechenParams				                      								 //
///////////////////////////////////////////////////////////////////////////////
void OSM_SvgParamReader::readFlaechenParams ( OSM_FlaechenDarstellung * pFlaechenDarstellung )
{
  String ^ STR;
  string   stdString;

  OSM_FlaechenParams * pFlaechenParams = new OSM_FlaechenParams;
  pFlaechenDarstellung->addFlaechenParams ( pFlaechenParams );

  STR = reader->GetAttribute ( "class" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pFlaechenParams->setClassId ( stdString );
  }

/*  STR = reader->GetAttribute ( "clip" );
  if ( STR != nullptr )
  {
    boolValue = XmlConvert::ToBoolean ( STR );
    pFlaechenParams->setClip ( boolValue );
  }

  STR = reader->GetAttribute ( "pattern" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pFlaechenParams->setPatternId ( stdString );
  }

  STR = reader->GetAttribute ( "hatching" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pFlaechenParams->setHatchingId ( stdString );
  }
*/
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Tag LinienDarstellung	                      									 //
///////////////////////////////////////////////////////////////////////////////
OSM_LinienDarstellung *  OSM_SvgParamReader::readLinienDarstellung ( String ^ tag )
{
  String ^ STR;
  string   stdString;
  int      intValue;

  OSM_LinienDarstellung * pLinienDarstellung = new OSM_LinienDarstellung;

/*  STR = reader->GetAttribute ( "default" );
  if ( STR != nullptr )
  {
    boolValue = XmlConvert::ToBoolean ( STR );
    pLinienDarstellung->setDefault ( boolValue );
  }
*/

  STR = reader->GetAttribute( "priority" );
  if ( STR != nullptr )
  {
    intValue = XmlConvert::ToInt32 ( STR );
    pLinienDarstellung->setPriority( intValue );
  }

  if ( reader->IsEmptyElement ) return pLinienDarstellung;

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("SelectAttributWert") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pLinienDarstellung->addSelectAttributWert ( stdString );
        }
      }    
      else
      if ( pActElement->CompareTo("LinienParams") == 0 )
        readLinienParams ( pLinienDarstellung );
      else
      if ( pActElement->CompareTo("LineSymbolParams") == 0 )
        readLineSymbolParams ( pLinienDarstellung );

    }
    else
    {
      if ( reader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if (pActElement->CompareTo( tag ) == 0 ) break;
      }
    }
  }
  return pLinienDarstellung;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Tag PunktDarstellung											                       //
///////////////////////////////////////////////////////////////////////////////
void OSM_SvgParamReader::readLinienParams ( OSM_LinienDarstellung * pLinienDarstellung )
{
  String ^ STR;
  string   stdString;

  OSM_LinienParams * pLinienParams = new OSM_LinienParams;
  pLinienDarstellung->addLinienParams ( pLinienParams );

  STR = reader->GetAttribute ( "class" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pLinienParams->setClassId ( stdString );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Tag LineSymbolParams		                      									 //
///////////////////////////////////////////////////////////////////////////////
void OSM_SvgParamReader::readLineSymbolParams ( OSM_LinienDarstellung * pDarstellung )
{
  String ^ STR;
  string   stdString;
  double   doubleValue;

  LineSymbolParams * pLineSymbolParams = new LineSymbolParams;
  pDarstellung->setLineSymbolParams ( pLineSymbolParams );

  STR = reader->GetAttribute ( "symbolId" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pLineSymbolParams->setSymbolId ( stdString );
  }

  STR = reader->GetAttribute ( "symbolDist" );
  if ( STR != nullptr )
  {
    doubleValue = XmlConvert::ToDouble ( STR );
    pLineSymbolParams->setSymbolDist ( doubleValue*globalScale );
  }
  else
    pLineSymbolParams->setSymbolDist ( 20*globalScale );


  STR = reader->GetAttribute ( "symbolSize" );
  if ( STR != nullptr )
  {
    doubleValue = XmlConvert::ToDouble ( STR );
    pLineSymbolParams->setSymbolSize ( doubleValue*globalScale );
  }
  else
    pLineSymbolParams->setSymbolSize ( globalScale );

  STR = reader->GetAttribute ( "symbolAngle" );
  if ( STR != nullptr )
  {
    doubleValue = XmlConvert::ToDouble ( STR );
    pLineSymbolParams->setSymbolAngle ( doubleValue );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Parameter für eine Punktdarstellung   											 //
///////////////////////////////////////////////////////////////////////////////
OSM_PunktDarstellung * OSM_SvgParamReader::readPunktDarstellung ( String ^ tag )
{
  String ^ STR;
  string   stdString;
  double   h;
  int      intValue;

  OSM_PunktDarstellung * pPunktDarstellung = new OSM_PunktDarstellung;

/*  STR = reader->GetAttribute ( "default" );
  if ( STR != nullptr )
  {
    boolValue = XmlConvert::ToBoolean ( STR );
    pPunktDarstellung->setDefault ( boolValue );
  }
*/
  STR = reader->GetAttribute( "priority" );
  if ( STR != nullptr )
  {
    intValue = XmlConvert::ToInt32 ( STR );
    pPunktDarstellung->setPriority( intValue );
  }

  STR = reader->GetAttribute ( "attributA" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pPunktDarstellung->setAttributA ( stdString );
  }

  STR = reader->GetAttribute ( "attributB" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pPunktDarstellung->setAttributB ( stdString );
  }

  STR = reader->GetAttribute ( "textStyle" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pPunktDarstellung->setTextStyle ( stdString );
  }

  STR = reader->GetAttribute ( "angleAttribute" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pPunktDarstellung->setAngleAttribute ( stdString );
  }

  STR = reader->GetAttribute ( "symbol" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pPunktDarstellung->setSymbol ( stdString );
  }

  STR = reader->GetAttribute ( "symbolSize" );
  if ( STR != nullptr )
  {
    h = XmlConvert::ToDouble ( STR );
    pPunktDarstellung->setSymbolSize ( h *globalScale );
  }
  else
    pPunktDarstellung->setSymbolSize ( globalScale );


  STR = reader->GetAttribute ( "textOffsetX" );
  if ( STR != nullptr )
  {
    h = XmlConvert::ToDouble ( STR );
    pPunktDarstellung->setTextOffsetX ( h );
  }

  STR = reader->GetAttribute ( "textOffsetY" );
  if ( STR != nullptr )
  {
    h = XmlConvert::ToDouble ( STR );
    pPunktDarstellung->setTextOffsetY ( h );
  }

  if ( reader->IsEmptyElement ) return pPunktDarstellung;

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("SelectAttributWert") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pPunktDarstellung->addSelectAttributWert ( stdString );
        }
      }     
    }
    else
    {
      if ( reader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if (pActElement->CompareTo( tag ) == 0 ) break;
      }
    }
  }
  return pPunktDarstellung;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Style									                            						 //
///////////////////////////////////////////////////////////////////////////////
Style * OSM_SvgParamReader::readStyle()
{
  String      ^ STR;
  string        stdString;
  SvgColor    * pColor;
  double        h;
  double        baselineShift;
  double        globalScaleLoc = 1.0;
  unsigned int  i;
  String      ^ BASELINE_SHIFT_UOM;
  bool	        baselineShiftRead = false;
  bool          isScalable;
  vector<double> stroke_dasharray;


  STR = reader->GetAttribute ( "styleName" );

  QuConvert::systemStr2stdStr ( stdString, STR );
  Style * pStyle = new Style ( stdString );
  pOSM_SvgParams->addStyle ( pStyle );

  STR = reader->GetAttribute ( "scalable" );
  if ( STR != nullptr )
  {
    isScalable = XmlConvert::ToBoolean ( STR );
    if ( isScalable ) globalScaleLoc = globalScale;
    else			  globalScaleLoc = 1.0;
  }
  else globalScaleLoc = globalScale;

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("FillColor") == 0 )
      {
        pColor = readColor ( "FillColor" );
        pStyle->setFillColor ( pColor );
      }
      else
      if ( pActElement->CompareTo("StrokeColor") == 0 )
      {
        pColor = readColor ( "StrokeColor" );
        pStyle->setStrokeColor ( pColor );
      }
      else
      if ( pActElement->CompareTo("Fill-opacity") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          h = XmlConvert::ToDouble ( STR );
          pStyle->setFillOpacity ( h );
        }
      }
      else
      if ( pActElement->CompareTo("Stroke-width") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          h = XmlConvert::ToDouble ( STR );
          pStyle->setStrokeWidth ( h*globalScaleLoc );
        }
        else
          pStyle->setStrokeWidth ( globalScaleLoc );
      }
      else
      if ( pActElement->CompareTo("Stroke-linecap") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pStyle->setStrokeLinecap ( stdString );
        }
      }
      else
      if ( pActElement->CompareTo("Stroke-dasharray") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
          h = XmlConvert::ToDouble ( STR );
          stroke_dasharray.push_back ( h*globalScaleLoc );
        }
      }
      else
      if ( pActElement->CompareTo("Stroke-dashoffset") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
           h = XmlConvert::ToDouble ( STR );
           pStyle->setStrokeDashoffset ( h*globalScaleLoc );
        }
      }
      else
      if ( pActElement->CompareTo("Font-size") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
           h = XmlConvert::ToDouble ( STR );
           pStyle->setFontSize ( h*globalScaleLoc );
        }
      }
      else
      if ( pActElement->CompareTo("Font-weight") == 0 )
      {
         STR = reader->ReadString();
         if ( STR != nullptr )
         {
            QuConvert::systemStr2stdStr ( stdString, STR );
            pStyle->setFontWeight ( stdString );
         }
      }
      else
      if ( pActElement->CompareTo("Text-anchor") == 0 )
      {
         STR = reader->ReadString();
         if ( STR != nullptr )
         {
           QuConvert::systemStr2stdStr ( stdString, STR );
           pStyle->setTextAnchor ( stdString );
         }
      }
      else
      if ( pActElement->CompareTo("Font-family") == 0 )
      {
         STR = reader->ReadString();
         if ( STR != nullptr )
         {
            QuConvert::systemStr2stdStr ( stdString, STR );
            pStyle->setFontFamily ( stdString );
         }
      }
      else
      if ( pActElement->CompareTo("Kerning") == 0 )
      {
        STR = reader->ReadString();
        if ( STR != nullptr )
        {
           h = XmlConvert::ToDouble ( STR );
           pStyle->setKerning ( h*globalScaleLoc );
         }
      }
      else
      if ( pActElement->CompareTo("Baseline-shift") == 0 )
      {
         STR = reader->ReadString();
         if ( STR != nullptr )
         {
            baselineShift = XmlConvert::ToDouble ( STR );
            baselineShiftRead = true;
         }
      }
      else
      if ( pActElement->CompareTo("Baseline-shift-uom") == 0 )
      {
         BASELINE_SHIFT_UOM = reader->ReadString();
      }
    }
    else
    {
      if ( reader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if (pActElement->CompareTo("Style") == 0 ) break;
      }
    }
  }

  if ( stroke_dasharray.size() > 0 )
  {
    STR = nullptr;
    for ( i = 0; i < stroke_dasharray.size(); i++ )
      STR = String::Concat ( STR, XmlConvert::ToString ( stroke_dasharray[i]), " " );
    QuConvert::systemStr2stdStr ( stdString, STR );
    pStyle->setStrokeDasharray ( stdString );
  }

  if ( baselineShiftRead )
  {
    if ( BASELINE_SHIFT_UOM != nullptr )
      STR = String::Concat ( XmlConvert::ToString ( baselineShift ), BASELINE_SHIFT_UOM );
    else
      STR = XmlConvert::ToString ( baselineShift*globalScaleLoc );

    QuConvert::systemStr2stdStr ( stdString, STR );	
    pStyle->setBaselineShift ( stdString );
  }

  return pStyle;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Color							                            								 //
///////////////////////////////////////////////////////////////////////////////
SvgColor * OSM_SvgParamReader::readColor ( String ^ endTagP )
{
  String   ^ STR;
  string     stdString;
  SvgColor * pColor = NULL;

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("None") == 0 )
        pColor = new SvgColor;
      else
        if ( pActElement->CompareTo("ColorName") == 0 )
        {
          STR = reader->ReadString();
          if ( STR != nullptr )
          {
            QuConvert::systemStr2stdStr ( stdString, STR );
            pColor = new SvgColor ( stdString );
          }
        }
        else
          if ( pActElement->CompareTo("Color") == 0 )
          {
            int r, g, b;

            STR = reader->GetAttribute ( "r" );
            r = XmlConvert::ToInt32 ( STR );
            STR = reader->GetAttribute ( "g" );
            g = XmlConvert::ToInt32 ( STR );
            STR = reader->GetAttribute ( "b" );
            b = XmlConvert::ToInt32 ( STR );

            pColor = new SvgColor ( r, g, b );
          }
    }
    else
    {
      if ( reader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if (pActElement->CompareTo( endTagP ) == 0 ) break;
      }
    }
  }

  return pColor;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Symbol										                            					 //
///////////////////////////////////////////////////////////////////////////////
Symbol * OSM_SvgParamReader::readSymbol()
{
  String ^ STR;
  string   stdString;

  STR = reader->GetAttribute ( "name" );

  QuConvert::systemStr2stdStr ( stdString, STR );
  Symbol * pSymbol = new Symbol ( stdString );
  pOSM_SvgParams->addSymbol ( pSymbol );

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("Path") == 0 )
      {
        string classId;
        string pathString;
        STR = reader->GetAttribute ( "class" );
        QuConvert::systemStr2stdStr( classId, STR );
        STR = reader->GetAttribute ( "path" );
        QuConvert::systemStr2stdStr( pathString, STR );

        SvgPath * pSvgPath = new SvgPath ( classId );
        pSvgPath->setPathString ( pathString );

        readGeometryParams ( pSvgPath, "Path" );

        pSymbol->addGeometryElement ( pSvgPath );
      }
      else
        if ( pActElement->CompareTo("Circle") == 0 )
        {
          string classId;
          double cx, cy, r;

          STR = reader->GetAttribute ( "class" );
          QuConvert::systemStr2stdStr( classId, STR );
          STR = reader->GetAttribute ( "cx" );
          cx = XmlConvert::ToDouble ( STR );
          STR = reader->GetAttribute ( "cy" );
          cy = XmlConvert::ToDouble ( STR );
          STR = reader->GetAttribute ( "r" );
          r = XmlConvert::ToDouble ( STR );

          SvgCircle * pSvgCircle = new SvgCircle ( classId );
          pSvgCircle->setGeometry ( cx, cy, r );

          readGeometryParams ( pSvgCircle, "Circle" );

          pSymbol->addGeometryElement ( pSvgCircle );
        }
        else
          if ( pActElement->CompareTo("Rect") == 0 )
          {
            string classId;
            double x, y, width, height;

            STR = reader->GetAttribute ( "class" );
            QuConvert::systemStr2stdStr( classId, STR );
            STR = reader->GetAttribute ( "x" );
            if ( STR != nullptr )
              x = XmlConvert::ToDouble ( STR );
            else x = 0.0;

            STR = reader->GetAttribute ( "y" );
            if ( STR != nullptr )
              y = XmlConvert::ToDouble ( STR );
            else y = 0.0;

            STR = reader->GetAttribute ( "width" );
            width = XmlConvert::ToDouble ( STR );
            STR = reader->GetAttribute ( "height" );
            height = XmlConvert::ToDouble ( STR );

            SvgRect * pSvgRect = new SvgRect ( classId );
            pSvgRect->setGeometry ( x, y, width, height );

            readGeometryParams ( pSvgRect, "Rect" );

            pSymbol->addGeometryElement ( pSvgRect );
          }
          else
            if ( pActElement->CompareTo("Text") == 0 )
            {
              string classId;
              string textString;
              double x, y, size;

              STR = reader->GetAttribute ( "class" );
              QuConvert::systemStr2stdStr( classId, STR );
              STR = reader->GetAttribute ( "text" );
              QuConvert::systemStr2stdStr( textString, STR );
              STR = reader->GetAttribute ( "x" );
              x = XmlConvert::ToDouble ( STR );
              STR = reader->GetAttribute ( "y" );
              y = XmlConvert::ToDouble ( STR );
              STR = reader->GetAttribute ( "size" );
              size = XmlConvert::ToDouble ( STR );

              SvgText * pSvgText = new SvgText ( classId );
              pSvgText->setPosition ( x, y );
              pSvgText->setTextSize ( size );
              pSvgText->setTextString ( textString );

              pSymbol->addGeometryElement ( pSvgText );
            }
    }
    else
    {
      if ( reader->NodeType == XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if (pActElement->CompareTo("Symbol") == 0 ) break;
      }
    }

  }
  return pSymbol;

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen von Darstellungsparametern für SvgPath, SvgCircle und SvgRect   //
///////////////////////////////////////////////////////////////////////////////
void OSM_SvgParamReader::readGeometryParams ( SvgGeometry * pSvgGeometry, String ^endTag )
{
  String       ^ STR;
  string         stdString;
  SvgColor     * pColor;
  double         h;
  vector<double> stroke_dasharray;

  if ( reader->IsEmptyElement == true ) return;

  while( reader->Read() )
  {
    pActElement = reader->LocalName;

    if ( reader->NodeType == XmlNodeType::Element )
    {

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
        else
          if ( pActElement->CompareTo("FillColor") == 0 )
          {
            pColor = readColor ( "FillColor" );
            pSvgGeometry->setFillColor ( pColor );
          }
          else
            if ( pActElement->CompareTo("StrokeColor") == 0 )
            {
              pColor = readColor ( "StrokeColor" );
              pSvgGeometry->setStrokeColor ( pColor );
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
//  __gc class SvgWriter													                           //
//  SVG-Ausgabe																                               //
///////////////////////////////////////////////////////////////////////////////
OSM_SvgWriter::OSM_SvgWriter ( OSM_VisualParams * pOSM_SvgParamsP, OSM_Model * pOSM_Model, double deltaP )
{
  m_pOSM_Model     = pOSM_Model;
  m_pOSM_SvgParams = pOSM_SvgParamsP;
  m_pSrsProjection = gcnew SrsProjection ( m_pOSM_Model->getSrsManager() );

  anzError		     = 0;
  delta			       = deltaP;
  EPSG_Code        = -1;
//  pNormalization   = NULL;
  swX = 0.0;
  swY = 0.0;
  noX = 0.0;
  noY = 0.0;

  initializeNormalization();
}
OSM_SvgWriter::~OSM_SvgWriter ()
{

}

bool OSM_SvgWriter::initializeNormalization()
{
  double     minLat, minLon, maxLat, maxLon;
  double     middleLon, middleLat;
  int        UTM_Zone;
  GeoPoint * pLatLon;
  GeoPoint * pConverted;


  if ( !m_pSrsProjection->isInitialized() ) return false;

  if ( m_pOSM_Model == NULL ) return false;

  m_pOSM_Model->getBounds( minLat, minLon, maxLat, maxLon );

  middleLon = 0.5*( minLon + maxLon );
  middleLat = 0.5*( minLat + maxLat );

  UTM_Zone = getUTM_Zone ( middleLon );
  if ( UTM_Zone == -1 ) return false;

  //  EPSG-Code für WGS 84 UMT Zone
  EPSG_Code = 32600 + UTM_Zone;

  pLatLon     = new GeoPoint ( minLon, minLat );
  pConverted  = new GeoPoint;
  m_pSrsProjection->convertLatLon2Srs( EPSG_Code, pLatLon, pConverted );
  swX = pConverted->getX();
  swY = pConverted->getY();

  pLatLon->set( maxLon, maxLat );
  m_pSrsProjection->convertLatLon2Srs( EPSG_Code, pLatLon, pConverted );
  noX = pConverted->getX();
  noY = pConverted->getY();

  delete pLatLon;
  delete pConverted;

  return true;
}

int OSM_SvgWriter::getUTM_Zone ( double laenge )
{

  if ( laenge < -180.0 || laenge > 180.0) return -1;

  Decimal x = Decimal ( ( 180 + laenge ) / 6.0);
  Decimal d =  System::Decimal::Ceiling( x );
  return Convert::ToInt32( d );
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben des SVG-Files                                                  //
///////////////////////////////////////////////////////////////////////////////
void OSM_SvgWriter::write ( std::string fileName, double globalScale )
{
	size_t                             i, objAnz, anz;
  int                                 priority;
  OSM_Darstellung                   * pOSM_Darstellung;
  string                              osm_classification;
  string                              osm_classificationNeu;
  vector<OSM_Object*>                 vOSM_Objects;
  OSM_Object                        * pObject;
  String						                ^ STR = "";
  set<string>                         sOSM_Classifications;
  set<string>::iterator               iterOSM_Classifications;
  multimap<int,OSM_Object*>           mOSM_Objects;
  multimap<int,OSM_Object*>::iterator iterOSM_Objekte;
  vector<Style*>                      vStyles;
  OSM_ObjectDataWriter              ^ pObjDataWriter;

  if ( !m_pOSM_Model->getObjectDataGenerated() )
  {
    String ^  objectDataPath = QuConvert::ToString (Constant::getObjectDataPath() );
    if ( !Directory::Exists ( objectDataPath ) )
      Directory::CreateDirectory ( objectDataPath );

    pObjDataWriter = gcnew OSM_ObjectDataWriter ( objectDataPath );
  }

  objAnz = m_pOSM_Model->getOSM_ObjectsSorted( vOSM_Objects );

  for ( i = 0; i < objAnz; i++ )
  {
    pObject = vOSM_Objects[i];

    if ( !m_pOSM_Model->getObjectDataGenerated() )
      pObjDataWriter->write ( pObject );

    if ( pObject->getClassification() == "MultiPolygonPart"  )
      continue;

    if ( pObject->getType() == OSM_RELATION && ((OSM_Relation*)pObject)->getErrorCode() != "" )
      continue;

    pOSM_Darstellung = m_pOSM_SvgParams->getVisualization( pObject, osm_classification, priority );
    if ( pOSM_Darstellung == NULL ) 
      continue;

    sOSM_Classifications.insert( osm_classification );
    mOSM_Objects.insert ( multimap<int,OSM_Object*>::value_type ( priority, pObject ) );
  }

  m_pOSM_Model->setObjectDataGenerated( true );

  writeSvgRootKarte( fileName, globalScale );

  writer->WriteStartElement ( "defs" );
  writer->WriteStartElement( "text ");
  writer->WriteAttributeString ( "id", "klassen" );

  i = 0;
  for ( iterOSM_Classifications = sOSM_Classifications.begin(); iterOSM_Classifications != sOSM_Classifications.end(); iterOSM_Classifications++ )
  {
    osm_classification = *iterOSM_Classifications;
    i++;

    if ( i < sOSM_Classifications.size() )
      STR = String::Concat ( STR, QuConvert::ToString ( osm_classification) , " " );
    else
      STR = String::Concat ( STR, QuConvert::ToString ( osm_classification) );
  }
  writer->WriteString ( STR );
  writer->WriteEndElement();

  writer->WriteEndElement();

  // Schreiben des css-Stylesheets
  writer->WriteStartElement ( "style" );
  writer->WriteAttributeString ( "type", "text/css" );

  anz = m_pOSM_SvgParams->getStyles ( vStyles );
  for ( i = 0; i < anz; i++ )
  {
    Style * pStyle = vStyles[i];

    string cssStyleString = pStyle->getStyleString();
    STR = QuConvert::ToString ( cssStyleString );
    STR = String::Concat ( STR, "\n" );
    writer->WriteRaw ( STR );
  }
  writer->WriteEndElement();

  osm_classification = "";

  for ( iterOSM_Objekte = mOSM_Objects.begin(); iterOSM_Objekte != mOSM_Objects.end(); iterOSM_Objekte++ )
  {
    priority = iterOSM_Objekte->first;
    pObject = iterOSM_Objekte->second;

    pOSM_Darstellung = m_pOSM_SvgParams->getVisualization( pObject, osm_classificationNeu, priority );

    if ( osm_classificationNeu != osm_classification )
    {
      if ( osm_classification != "" )  writer->WriteEndElement();
      writer->WriteStartElement ( "g" );
      STR = String::Concat ( QuConvert::ToString( osm_classificationNeu ), QuConvert::ToString( pObject->getTypeString() ), "0" ) ;

      writer->WriteAttributeString ( "id", STR );
      writer->WriteAttributeString ( "display", "inline" );
      osm_classification = osm_classificationNeu;
    }

    writeOSM_Objekt ( pObject, pOSM_Darstellung );
  }

  writer->WriteEndElement();
  writer->Flush();
  writer->Close();

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben des SVG-Headers                                                //
///////////////////////////////////////////////////////////////////////////////
void OSM_SvgWriter::writeSvgRootKarte ( std::string fileName, double globalScale )
{
  double   minLat, minLon, maxLat, maxLon;
  double   sw[3], no[3], groesseDisplay[2], b[2], d[2];
  GeoPoint pLatLon, pCart;

  m_pOSM_Model->getBounds( minLat, minLon, maxLat, maxLon );

  pLatLon.set( minLon, minLat );
  m_pSrsProjection->convertLatLon2Srs( EPSG_Code, &pLatLon, &pCart );
  sw[0] = pCart.getX();
  sw[1] = pCart.getY();

  pLatLon.set( maxLon, maxLat );
  m_pSrsProjection->convertLatLon2Srs( EPSG_Code, &pLatLon, &pCart );
  no[0] = pCart.getX();
  no[1] = pCart.getY();

  d[0] = no[0] - sw[0];
  d[1] = no[1] - sw[1];

  groesseDisplay[0] = m_pOSM_SvgParams->getBreiteDisplay();
  groesseDisplay[1] = m_pOSM_SvgParams->getHoeheDisplay();

  b[0] = groesseDisplay[0]* 2000*globalScale;
  b[1] = groesseDisplay[1]* 2000*globalScale;

  sw[0] = sw[0] + 0.5*( d[0] - b[0] );
  sw[1] = sw[1] + 0.5*( d[1] - b[1] );
  no[0] = sw[0] + b[0];
  no[1] = sw[1] + b[1];

  writeSvgRoot ( fileName, sw[0], sw[1], no[0], no[1] );
}

void OSM_SvgWriter::writeSvgRoot( std::string fileName, double _swX, double _swY, double _noX, double _noY )
{
  String		^ STR;
  String		^ STR_X;
  String		^ STR_Y;
  String		^ STR_X0;
  String		^ STR_Y0;
  double		  deltaX, deltaY;

  deltaX =  delta*(_noX - _swX);
  deltaY =  delta*(_noY - _swY);

  System::Text::UTF8Encoding ^ utf8	= gcnew System::Text::UTF8Encoding();

  STR    = QuConvert::ToString ( fileName );
  writer = gcnew XmlTextWriter( STR, utf8);

  writer->Formatting = Formatting::Indented;
  writer->WriteStartDocument( true );

  //	writer->WriteDocType ( "svg", "-//W3C//DTD SVG 1.1//EN",  "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11-flat-20030114.dtd", nullptr );
  writer->WriteComment("Generated by: GML-Toolbox");

  writer->WriteStartElement("svg ");

  writer->WriteAttributeString("xmlns", "http://www.w3.org/2000/svg");
  writer->WriteAttributeString("xmlns:xlink", "http://www.w3.org/1999/xlink");
  writer->WriteAttributeString("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");

  writer->WriteAttributeString("width", "100%" );
  writer->WriteAttributeString("height", "100%" );

  STR_X0 = XmlConvert::ToString ( _swX - swX -deltaX );
  STR_Y0 = XmlConvert::ToString ( _swY - swY - deltaY );
  STR_X = XmlConvert::ToString ( _noX - _swX + 2.0*deltaX );
  STR_Y = XmlConvert::ToString ( _noY - _swY + 2.0*deltaY );
  STR = System::String::Concat( STR_X0, " ", STR_Y0, " " );
  STR = System::String::Concat( STR, STR_X, " ", STR_Y );
  writer->WriteAttributeString ( "viewBox", STR  );
  writer->WriteAttributeString ( "id", "karte" );
  writer->WriteAttributeString ( "zoomAndPan", "disable" );
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben Überschrift                                                    //
///////////////////////////////////////////////////////////////////////////////
void OSM_SvgWriter::writeUeberschrift ( std::string fileName )
{
  String                     ^ STR;
  System::Text::UTF8Encoding ^ utf8	= gcnew System::Text::UTF8Encoding();

  STR    = QuConvert::ToString ( fileName );
  writer = gcnew XmlTextWriter( STR, utf8 );

  writer->Formatting = Formatting::Indented;
  writer->WriteStartDocument( true );

  writer->WriteComment("Generated by: KIT GML-Toolbox");

  writer->WriteStartElement("svg ");

  writer->WriteAttributeString("xmlns", "http://www.w3.org/2000/svg");
  writer->WriteAttributeString("xmlns:xlink", "http://www.w3.org/1999/xlink");
  writer->WriteAttributeString("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  writer->WriteAttributeString ( "zoomAndPan", "disable" );

  writer->WriteEndElement();
  writer->Flush();
  writer->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  SVG-Datei umhüllendes Rechteck                                           //
///////////////////////////////////////////////////////////////////////////////
void OSM_SvgWriter::writeUebersicht ( std::string fileName, double globalScale )
{

  writeSvgRoot ( fileName, swX, swY, noX, noY );

  writer->WriteStartElement ( "defs" );

  writer->WriteStartElement ( "style" );
  writer->WriteAttributeString ( "type", "text/css" );

  String ^ styleString = ".default {fill:gray; stroke: black; stroke-width:1;}";
  writer->WriteString ( styleString );

  writer->WriteEndElement();
  writer->WriteEndElement();

  writer->WriteStartElement ( "rect" );
  writer->WriteAttributeString ( "id", "rechteck" );
  writer->WriteAttributeString ( "x", "1" );
  writer->WriteAttributeString ( "y", "1" );
  writer->WriteAttributeString ( "width", "1" );
  writer->WriteAttributeString ( "height", "1" );
  writer->WriteAttributeString ( "fill-opacity", "0.3" );
  writer->WriteAttributeString ( "fill", "black" );
  writer->WriteAttributeString ( "stroke", "black" );
  writer->WriteAttributeString ( "stroke-width", "5" );
  writer->WriteAttributeString ( "onmousedown", "top.panStart(evt)" );
  writer->WriteAttributeString ( "onmousemove", "top.panVerlauf(evt)" );
  writer->WriteAttributeString ( "onmouseup", "top.panEnde(evt)" );
  writer->WriteAttributeString ( "onmouseover", "top.rechteckInfo(evt)" );
  writer->WriteAttributeString ( "onmouseout", "top.rechteckOut(evt)" );
  writer->WriteEndElement();

  writer->WriteEndElement();

  writer->Flush();
  writer->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Visualisierung eines OSM-Objektes                                        //
///////////////////////////////////////////////////////////////////////////////
void OSM_SvgWriter::writeOSM_Objekt ( OSM_Object * pOSM_Objekt, OSM_Darstellung * pOSM_Darstellung )
{
  switch ( pOSM_Objekt->getType() )
  {
  case OSM_NODE:
    writePunktDarstellung( (OSM_Node*)pOSM_Objekt, (OSM_PunktDarstellung*)pOSM_Darstellung );
    break;

  case OSM_WAY:
    writeLinienDarstellung( (OSM_Way*)pOSM_Objekt, (OSM_LinienDarstellung*)pOSM_Darstellung );
    break;

  case OSM_AREA:
    writeFlaechenDarstellung( (OSM_Way*)pOSM_Objekt, (OSM_FlaechenDarstellung*)pOSM_Darstellung );
    break;

  case OSM_RELATION:
    writeRelationDarstellung ( (OSM_Relation*)pOSM_Objekt,   (OSM_RelationDarstellung*)pOSM_Darstellung );
  }
}

void OSM_SvgWriter::writePunktDarstellung ( OSM_Node * pOSM_Object, OSM_PunktDarstellung * pPunktDarstellung )
{
  double    lat, lon;
  double    x, y;
  GeoPoint  pLatLon;
  GeoPoint  pCart;
  String  ^ STR;
  double    outputSymbolSize;
  string    symbolId;

  pOSM_Object->getGeometry( lat, lon );
  pLatLon.set ( lon, lat );
  m_pSrsProjection->convertLatLon2Srs( EPSG_Code, &pLatLon, &pCart );

  x = pCart.getX() - swX;
  y = noY - pCart.getY();

  symbolId = pPunktDarstellung->getSymbol();
  if ( symbolId != "" )
    outputSymbolSize = pPunktDarstellung->getSymbolSize();
//  else
//  {
//    symbolId = pPunktDarstellung->getDefaultSymbol();
//    outputSymbolSize = pPunktDarstellung->getDefaultSymbolSize();
//  }

  String ^ OUTPUT_SYMBOL = QuConvert::ToString ( symbolId );
  String ^ ID           = String::Concat ( "OSM_", QuConvert::ToString( pOSM_Object->getId() ), "00" );

  writer->WriteStartElement ( "use" );

  STR = String::Concat ( "#", OUTPUT_SYMBOL );
  writer->WriteAttributeString ( "xlink:href", STR );

  STR = String::Concat ( ID, "_A___" );
  writer->WriteAttributeString ( "id", STR );

  writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
  writer->WriteAttributeString ( "onmouseover", "top.karteInfo(evt)" );
  writer->WriteAttributeString ( "onmouseout", "top.karteOut(evt)" );
  

  STR = String::Concat ( "translate(", XmlConvert::ToString ( x ), "," );
  STR = String::Concat ( STR, XmlConvert::ToString ( y ), ") " );
  
  STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( outputSymbolSize ), ")" );
  writer->WriteAttributeString ( "transform", STR );

  writer->WriteEndElement();
}

void OSM_SvgWriter::writeLinienDarstellung   ( OSM_Way * pOSM_Object, OSM_LinienDarstellung * pLinienDarstellung )
{
  vector<double>            vLat;
  vector<double>            vLon;
  vector<double>            vX;
  vector<double>            vY;
  double                    x, y, lat, lon;
  vector<OSM_LinienParams*>	vLinienParams;
  OSM_LinienParams        * pLinienParams;
	size_t                    i, j, anz, anzPoints, index;
  String                  ^ STR;
  String                  ^ ID;
  String                  ^ PATH_STRING;
  string                    stdString;
  GeoPoint                  pLatLon;
  GeoPoint                  pCart;

  anzPoints = m_pOSM_Model->getLineGeometry( pOSM_Object, vLat, vLon );
  index     = 0;
  if ( anzPoints <= 1 ) return;

  for ( i = 0; i <= anzPoints; i++ )
  {
    if ( i < anzPoints )
    {
      lat = vLat[i];
      lon = vLon[i];
    }
    else
    {
      lat = 999.0;
      lon = 999.0;
    }

    if ( lat < 900.0 )
    {
      pLatLon.set ( lon, lat );
      m_pSrsProjection->convertLatLon2Srs( EPSG_Code, &pLatLon, &pCart );

      x = pCart.getX() - swX;
      y = noY - pCart.getY();

      vX.push_back( x );
      vY.push_back( y );
    }
    else
    {
      if ( vX.size() > 1 )
      {
        PATH_STRING = createPathString ( vX, vY );

        STR =  XmlConvert::ToString ( index );
        if ( STR->Length == 1 )
          STR = String::Concat ( "0", STR );
        ID = String::Concat ( "OSM_", QuConvert::ToString( pOSM_Object->getId() ), STR );

        writer->WriteStartElement ( "defs" );
        writer->WriteStartElement ( "path" );
        writer->WriteAttributeString ( "id", ID );
        writer->WriteAttributeString ( "d", PATH_STRING );
        writer->WriteEndElement();
        writer->WriteEndElement();

        anz = pLinienDarstellung->getLinienParams ( vLinienParams );
        for ( j = 0; j < anz; j++ )
        {
          pLinienParams = vLinienParams[j];
          string classId = pLinienParams->getClassId();

          writer->WriteStartElement ( "use" );
          STR = String::Concat ( "#", ID );
          writer->WriteAttributeString ( "xlink:href", STR );

          STR = String::Concat ( ID, "_A", XmlConvert::ToString ( j ) );
          writer->WriteAttributeString ( "id", STR );

          writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
          writer->WriteAttributeString ( "onmouseover", "top.karteInfo(evt)" );
          writer->WriteAttributeString ( "onmouseout", "top.karteOut(evt)" );
      
          if ( classId != "" )
            writer->WriteAttributeString ( "class", QuConvert::ToString ( classId ) );

          writer->WriteEndElement();
        }
        index++;
      }
      vX.clear();
      vY.clear();
    }
  }

}

void OSM_SvgWriter::writeFlaechenDarstellung ( OSM_Way * pOSM_Object, OSM_FlaechenDarstellung * pFlaechenDarstellung )
{
  vector<double>              vLat;
  vector<double>              vLon;
  vector<double>              vX;
  vector<double>              vY;
  double                      x, y, lat, lon;
  vector<OSM_FlaechenParams*> vFlaechenParams;
	size_t                      i,  anz;
  String                    ^ STR;
  GeoPoint                    pLatLon;
  GeoPoint                    pCart;
  String                    ^ PATH_STRING;

  anz = m_pOSM_Model->getSurfaceGeometry( pOSM_Object, vLat, vLon );
  if ( anz <= 1 ) return;

  String ^ ID = String::Concat ( "OSM_", QuConvert::ToString( pOSM_Object->getId() ), "00" );

  for ( i = 0; i < anz; i++ )
  {
    lat = vLat[i];
    lon = vLon[i];

    pLatLon.set ( lon, lat );
    m_pSrsProjection->convertLatLon2Srs( EPSG_Code, &pLatLon, &pCart );

    x = pCart.getX() - swX;
    y = noY - pCart.getY();

    vX.push_back( x );
    vY.push_back( y );
  }

  PATH_STRING = createPathString ( vX, vY );

  writer->WriteStartElement ( "clipPath" );
  String ^ CLIP_PATH_ID = String::Concat( ID, "_Clip" );
  writer->WriteAttributeString( "id", CLIP_PATH_ID );

  writer->WriteStartElement ( "path" );
  writer->WriteAttributeString ( "id", ID );
  writer->WriteAttributeString ( "d", PATH_STRING );
  writer->WriteEndElement();

  writer->WriteEndElement();

  anz = pFlaechenDarstellung->getFlaechenParams ( vFlaechenParams );
  for ( i = 0; i < anz; i++ )
  {
    OSM_FlaechenParams * pFlaechenParams = vFlaechenParams[i];

    string classId    = pFlaechenParams->getClassId();
    writer->WriteStartElement ( "use" );
    STR = String::Concat ( "#", ID );
    writer->WriteAttributeString ( "xlink:href", STR );

    if ( classId != "" )
    {
      String ^ CLASS_ID = QuConvert::ToString ( classId );
      writer->WriteAttributeString ( "class", CLASS_ID );
    }

    STR = String::Concat ( ID, "_A", XmlConvert::ToString ( i ) );
    writer->WriteAttributeString ( "id", STR );
    writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
    writer->WriteAttributeString ( "onmouseover", "top.karteInfo(evt)" );
    writer->WriteAttributeString ( "onmouseout", "top.karteOut(evt)" );   

    writer->WriteEndElement();  
  }
}

void  OSM_SvgWriter::writeRelationDarstellung ( OSM_Relation * pOSM_Object, OSM_RelationDarstellung * pRelationDarstellung )
{
  if ( pOSM_Object->getIsMultipolygon() == true )
  {
    writeMultiPolygon ( pOSM_Object, pRelationDarstellung );
    return;
  }
}

bool  OSM_SvgWriter::writeMultiPolygon ( OSM_Relation * pOSM_Object, OSM_RelationDarstellung * pRelationDarstellung )
{
  vector<MultiWay*> vOuterContours;
  vector<MultiWay*> vInnerContours;

  vOuterContours = pOSM_Object->getOuterContours();
  if ( vOuterContours.size() != 1 ||  vOuterContours[0]->isClosed() == false )
    return false;

  vInnerContours = pOSM_Object->getInnerContours();
  for ( unsigned int i = 0; i < vInnerContours.size(); i++  )
  {
    if ( vInnerContours[i]->isClosed() == false)
      return false;
  }

  writeMultiPolygon( pOSM_Object->getId(), vOuterContours[0]->getNodeIds(), vInnerContours, pRelationDarstellung );
  return true;
}

void OSM_SvgWriter::writeMultiPolygon ( string objId, vector<string> &vOuterNodeIds, vector<MultiWay*> &vInner, OSM_RelationDarstellung * pRelationDarstellung )
{
//  OSM_Way                   * pOuter;
//  vector<OSM_Way*>            vInner;
  vector<double>              vLat;
  vector<double>              vLon;
  vector<double>              vX;
  vector<double>              vY;
  vector<string>              vInnerNodeIds;
  double                      x, y, lat, lon;
  vector<OSM_FlaechenParams*> vFlaechenParams;
	size_t                      i,  j, anz;
  String                    ^ STR;
  GeoPoint                    pLatLon;
  GeoPoint                    pCart;
  String                    ^ PATH_STRING;
  String                    ^ STR_NUMBER;
  String                    ^ ID;
  double                      sw[2], no[2];

  sw[0] =  1.0E20;
  sw[1] =  1.0E20;
  no[0] = -1.0E20;
  no[1] = -1.0E20;

  for ( i = 0; i < vOuterNodeIds.size(); i++ )
  {
    OSM_Node * pNode = m_pOSM_Model->getOSM_Node( vOuterNodeIds[i] );
    vLat.push_back( pNode->getLat() );
    vLon.push_back( pNode->getLon() );
  }

//  pOuter = pOSM_Object->getRings( vInner, m_pOSM_Model );
//  if ( pOuter == NULL ) return;
  
  anz = vLon.size();
  if ( anz <= 1 ) return;

  OSM_FlaechenDarstellung * pFlaechenDarstellung = pRelationDarstellung->getFlaechenDarstellung();

  ID = String::Concat ( "OSM_", QuConvert::ToString( objId ), "00" );

  for ( i = 0; i < anz; i++ )
  {
    lat = vLat[i];
    lon = vLon[i];

    pLatLon.set ( lon, lat );
    m_pSrsProjection->convertLatLon2Srs( EPSG_Code, &pLatLon, &pCart );

    if ( pCart.getX() < sw[0] ) sw[0] = pCart.getX();
    if ( pCart.getY() < sw[1] ) sw[1] = pCart.getY();
    if ( pCart.getX() > no[0] ) no[0] = pCart.getX();
    if ( pCart.getY() > no[1] ) no[1] = pCart.getY();

    x = pCart.getX() - swX;
    y = noY - pCart.getY();

    vX.push_back( x );
    vY.push_back( y );
  }

  PATH_STRING = createPathString ( vX, vY );

  writer->WriteStartElement ( "clipPath" );
  String ^ CLIP_PATH_ID = String::Concat( ID, "_Clip" );
  writer->WriteAttributeString( "id", CLIP_PATH_ID );

  writer->WriteStartElement ( "path" );
  writer->WriteAttributeString ( "id", ID );
  writer->WriteAttributeString ( "d", PATH_STRING );
  writer->WriteEndElement();

  writer->WriteEndElement();

  writer->WriteStartElement( "defs" );

  for ( i = 0; i < vInner.size(); i++ )
  {
    vInnerNodeIds.clear();
    vX.clear();
    vY.clear();

    MultiWay * pInner = vInner[i];
    vInnerNodeIds = pInner->getNodeIds();

    anz = vInnerNodeIds.size();
    if ( anz <= 1 ) continue;

    STR_NUMBER = System::Convert::ToString ( i );
    String ^ HOLE_ID = String::Concat ( ID, "_hole_", STR_NUMBER  );

    for ( j = 0; j < anz; j++ )
    {
      OSM_Node * pNode = m_pOSM_Model->getOSM_Node( vInnerNodeIds[j] );

      lat = pNode->getLat();
      lon = pNode->getLon();

      pLatLon.set ( lon, lat );
      m_pSrsProjection->convertLatLon2Srs( EPSG_Code, &pLatLon, &pCart );

      x = pCart.getX() - swX;
      y = noY - pCart.getY();

      vX.push_back( x );
      vY.push_back( y );
    }

    PATH_STRING = createPathString ( vX, vY );
    writer->WriteStartElement ( "path" );
    writer->WriteAttributeString ( "id", HOLE_ID );
    writer->WriteAttributeString ( "d", PATH_STRING );
    writer->WriteEndElement();
  }

  writer->WriteEndElement();

  writer->WriteStartElement ( "mask" );
  writer->WriteAttributeString ( "maskUnits", "userSpaceOnUse" );

  String ^ MASK_ID = String::Concat ( ID, "_mask" );
  writer->WriteAttributeString ( "id", MASK_ID );

  writer->WriteAttributeString ( "x", XmlConvert::ToString ( sw[0] - swX ));
  writer->WriteAttributeString ( "y", XmlConvert::ToString ( noY - no[1] ));
  writer->WriteAttributeString ( "width", XmlConvert::ToString ( no[0]- sw[0] ) );
  writer->WriteAttributeString ( "height", XmlConvert::ToString ( no[1]- sw[1] ) );

  writer->WriteStartElement ( "rect" );
  writer->WriteAttributeString ( "fill", "white" );
  writer->WriteAttributeString ( "x", XmlConvert::ToString ( sw[0] - swX ));
  writer->WriteAttributeString ( "y", XmlConvert::ToString ( noY - no[1] ));
  writer->WriteAttributeString ( "width", XmlConvert::ToString ( no[0]- sw[0] ) );
  writer->WriteAttributeString ( "height", XmlConvert::ToString ( no[1]- sw[1] ) );
  writer->WriteEndElement();

  for ( i = 0; i < vInner.size(); i++ )
  {
    STR_NUMBER = System::Convert::ToString ( i );
    STR = String::Concat ( "#", ID, "_hole_", STR_NUMBER );	
    writer->WriteStartElement ( "use" );
    writer->WriteAttributeString ( "xlink:href", STR );
    writer->WriteAttributeString ( "fill", "black" );
    writer->WriteEndElement();
  }
  writer->WriteEndElement();


  anz = pFlaechenDarstellung->getFlaechenParams ( vFlaechenParams );
  for ( i = 0; i < anz; i++ )
  {
    OSM_FlaechenParams * pFlaechenParams = vFlaechenParams[i];

    string classId    = pFlaechenParams->getClassId();
    writer->WriteStartElement ( "use" );
    STR = String::Concat ( "#", ID );
    writer->WriteAttributeString ( "xlink:href", STR );

    if ( classId != "" )
    {
      String ^ CLASS_ID = QuConvert::ToString ( classId );
      writer->WriteAttributeString ( "class", CLASS_ID );
    }

    String ^ STR = String::Concat ( ID, "_A", XmlConvert::ToString ( i ) );
    writer->WriteAttributeString ( "id", STR );
    writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
    writer->WriteAttributeString ( "onmouseover", "top.karteInfo(evt)" );
    writer->WriteAttributeString ( "onmouseout", "top.karteOut(evt)" );   

    STR = String::Concat ( "url(#", ID, "_mask)" );
    writer->WriteAttributeString ( "mask", STR );   

    writer->WriteEndElement();  
  }
}

String ^ OSM_SvgWriter::createPathString ( vector<double> &vX, vector<double> &vY )
{
  string         svgPath;
  ostringstream  os;
  unsigned int	 i;

  for ( i = 0; i < vX.size(); i++ )
  {
    if ( i == 0 )	os << " M ";
    else          os << " L ";

    os << vX[i] << " " << vY[i];
  }

  svgPath = os.str();
  
  return QuConvert::ToString( svgPath );
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die OSM-Tags in eine HTML-Datei                                 //
///////////////////////////////////////////////////////////////////////////////
OSM_ObjectDataWriter::OSM_ObjectDataWriter ( String ^ objFolderP )
{
  objFolder = objFolderP;
}

OSM_ObjectDataWriter::~ OSM_ObjectDataWriter()
{

}

void OSM_ObjectDataWriter::write( OSM_Object * pOSM_Object )
{
  string                       id  = pOSM_Object->getId();
  String                     ^ outputFile = String::Concat ( objFolder, "OSM_", QuConvert::ToString ( id ), ".htm" );
  String                     ^ STR;
  map<string,string>           mTags = pOSM_Object->getTags();
  map<string,string>::iterator iter;
  unsigned int                 i;

  pWriter = gcnew StreamWriter ( outputFile );

  pWriter->WriteLine ( "<html>" );

  pWriter->WriteLine ( "<style type=\"text/css\">" );
  pWriter->WriteLine ( "" );
  pWriter->WriteLine ( "<!--" );
  pWriter->WriteLine ( "h1 {font-family: Arial, Helvetica, sans-serif; text-align: center;}" );
  pWriter->WriteLine ( "h2 {font-family: Arial, Helvetica, sans-serif; text-align: center;}" );
  pWriter->WriteLine ( "td {font-family: Arial, Helvetica, sans-serif;text-indent: 4px;}" );
  pWriter->WriteLine ( "-->" );
  pWriter->WriteLine ( "</style>" );

  pWriter->WriteLine ( "<title>OpenStreetMap-Attribute</title><body>" );

  pWriter->WriteLine ( "<body bgcolor=\"#FFFFAA\">" );

  pWriter->WriteLine ( "<table width=\"100%\" border=\"1\" cellspacing=\"0\" bordercolor=\"#000000\">" );
  pWriter->WriteLine ( "<tr>" );
  pWriter->WriteLine ( "<td width=\"33%\"><font size=\"+1\"><strong>Name</strong></font></td>" );
  pWriter->WriteLine ( "<td width=\"66%\"><font size=\"+1\"><strong>Wert</strong></font></td>" );
  pWriter->WriteLine ( "</tr>" );

  pWriter->WriteLine ( "<tr>" );
  STR = String::Concat ( "<td width=\"33%\"><strong>", QuConvert::ToHtml ( "Id" ), "</strong></td>" );
  pWriter->WriteLine ( STR );
  STR = String::Concat ( "<td>", QuConvert::ToHtml ( pOSM_Object->getId() ), "</td>" );
  pWriter->WriteLine ( STR );
  pWriter->WriteLine ( "</tr>" );

  for ( i = 0; i < pOSM_Object->getRelationAnz(); i++ )
  {
    OSM_Relation * pRel = pOSM_Object->getRelation( i );
    string         relId = pRel->getId();
    String       ^ relatedFile = String::Concat ( "file:\\", objFolder, "OSM_", QuConvert::ToString ( relId ), ".htm" );


    pWriter->WriteLine ( "<tr>" );
    STR = String::Concat ( "<td width=\"33%\"><strong>", "Verwendet in Relation", "</strong></td>" );
    pWriter->WriteLine ( STR );
    STR = String::Concat ( "<td> <a href=\"", relatedFile, "\">", QuConvert::ToString ( relId ), "</a></td>" );
    pWriter->WriteLine ( STR );
    pWriter->WriteLine ( "</tr>" );
  }
  

  for ( iter = mTags.begin(); iter != mTags.end(); iter++ )
  {
    string key   = iter->first;
    string value = iter->second;

    pWriter->WriteLine ( "<tr>" );
    STR = String::Concat ( "<td width=\"33%\"><strong>", QuConvert::ToHtml ( key ), "</strong></td>" );
    pWriter->WriteLine ( STR );
    STR = String::Concat ( "<td>", QuConvert::ToHtml ( value ), "</td>" );
    pWriter->WriteLine ( STR );
    pWriter->WriteLine ( "</tr>" );
  }

  pWriter->WriteLine ( "</body>" );
  pWriter->WriteLine ( "</html>" );

  pWriter->Flush();
  pWriter->Close();
}






































