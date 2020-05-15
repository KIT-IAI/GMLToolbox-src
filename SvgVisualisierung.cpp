#include "StdAfx.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Windows::Forms;
using namespace System::Text;
using namespace System::Collections;

#using <mscorlib.dll>

#include <set>
#include <map>
#include <vector>

#include "SrsManagerRef.h"
#include "Feature.h"
#include "Logfile.h"
#include "Convert.h"
#include "constant.h"
#include "GmlSchema.h"
#include "Dictionary.h"
#include "Geometrie.h"
#include "Visualisierung_2D.h"
#include "Checking.h"
#include "TimeX.h"
#include "SvgVisualisierung.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  class GmlKlassenVisual					                            						 //
//  Basisklasse für die svg-Visuasisierungs Parameter einer GML-Klasse       //
///////////////////////////////////////////////////////////////////////////////
GmlKlassenVisual::GmlKlassenVisual ( SvgVisualisierung * pSvgParamsP )
{
  pSvgParams = pSvgParamsP;

	gmlKlassenName	= "";
	priorityFlaechendarstellung = 7;
	priorityLiniendarstellung   = 20;
	priorityPunktdarstellung    = 30;
	selectAttribut1	= "";
	selectAttribut2	= "";
	defaultSymbol   = "";
	defaultSymbolSize = 1.0f;
}

GmlKlassenVisual::~ GmlKlassenVisual()
{
	unsigned int i;
  string       id;

	for ( i = 0; i < vFlaechenDarstellungen.size(); i++ )
  {
    FlaechenDarstellung * pFlaechenDarstellung = vFlaechenDarstellungen[i];
    id = pFlaechenDarstellung->getId();
    if ( id != "" )
      pSvgParams->removeFlaechenStyle ( id );
  		delete pFlaechenDarstellung;
  }

	for ( i = 0; i < vLinienDarstellungen.size(); i++ )
  {
    LinienDarstellung * pLinienDarstellung = vLinienDarstellungen[i];
    id = pLinienDarstellung->getId();
    if ( id != "" )
      pSvgParams->removeLinienStyle ( id );
  	delete pLinienDarstellung;
  }

	for ( i = 0; i < vPunktDarstellungen.size(); i++ )
		delete vPunktDarstellungen[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Priorität der Darstellung in Abhängibkeit vom Geometrietyp   //
///////////////////////////////////////////////////////////////////////////////
int GmlKlassenVisual::getPriority ( GEOMETRY_TYPE geometrieTypP )
{
	if ( geometrieTypP == MULTI_POINT ||
       geometrieTypP == GEO_POINT ) 
		return priorityPunktdarstellung;
	else
	if ( geometrieTypP == MULTI_CURVE ||
       geometrieTypP == CURVE       ||
       geometrieTypP == LINE_STRING ||
    	 geometrieTypP == LINE_STRING_SEGMENT ||
		   geometrieTypP == ARC         ||
       geometrieTypP == CIRCLE  ) 
		return priorityLiniendarstellung;
	else
	if ( geometrieTypP == MULTI_SURFACE          || 
       geometrieTypP == COMPOSITE_SURFACE      || 
       geometrieTypP == SHELL                  || 
       geometrieTypP == SURFACE                || 
       geometrieTypP == POLYGON                || 
       geometrieTypP == ORIENTABLE_SURFACE     || 
       geometrieTypP == SURFACE_REFERENCE      ||
       geometrieTypP == TRIANGULATED_SURFACE   ) 
		return priorityFlaechendarstellung;
	else
		return -1;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Priorität der Darstellung für ein best. Feature              //
///////////////////////////////////////////////////////////////////////////////
int GmlKlassenVisual::getPriority ( Feature * pPlanObjekt )
{
  int priority, priorityLoc;

  if ( pPlanObjekt->getObjektTyp() == RASTER_BASIS || 
       pPlanObjekt->getObjektTyp() == PLAN_BEREICH )
  {
    return getPriorityFlaechendarstellung();   
  }

  GEOMETRY_TYPE geometrieTyp = pPlanObjekt->getGeometrieTyp();
  priority = getPriority( geometrieTyp );

  Darstellung * pDarstellungSelected = getDarstellungSelected ( pPlanObjekt );
  if ( pDarstellungSelected == NULL) return -1;

  priorityLoc = pDarstellungSelected->getPriority();
  if ( priorityLoc != -1 ) priority = priorityLoc;

  if ( geometrieTyp == MULTI_SURFACE     || 
       geometrieTyp == COMPOSITE_SURFACE || 
       geometrieTyp == SHELL             || 
       geometrieTyp == SURFACE           || 
       geometrieTyp == POLYGON           )
  {
    if ( pPlanObjekt->getFlaechenschluss() ) priority = priority - 1;
    else									                   priority = priority + 1;
  }
  
  return priority;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die selektierte Darstellung                                      //
///////////////////////////////////////////////////////////////////////////////
Darstellung *  GmlKlassenVisual::getDarstellungSelected ( Feature * pPlanObjekt )
{
  string            errorText;
  string            selectAttributWert1 = "";
  string	          selectAttributWert2 = "";
  string	          selectAttributWert3 = "";
	size_t            i, anz;
  GmlKlasse       * pGmlKlasse;
  GmlAttribut     * pGmlAttribut;
  Darstellung     * pDarstellung;
  Darstellung     * pDarstellungSelected = NULL;
  UOM             * pUOM;

  GmlSchema      * pGmlObjekte = pPlanObjekt->getFeatures()->getGmlSchema();
  GmlElement      * pGmlElement = pGmlObjekte->getGmlElement( gmlKlassenName );
  if ( pGmlElement != NULL )
     pGmlKlasse  = pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );
  else
    pGmlKlasse = NULL;

  vector<Darstellung*> vDarstellungen;

  if ( pGmlKlasse == NULL )
  {
    errorText = "GML-Klassenname in den Darstellungsvorschriften unbekannt: \"";
    errorText += gmlKlassenName;
    pPlanObjekt->getFeatures()->getLogfile()->writeLogFile ( errorText );
    return NULL;
  }

  if ( selectAttribut1 != "" )
  {
    pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut1 );
    if ( pGmlAttribut != NULL )
      pPlanObjekt->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut1, selectAttributWert1, pUOM );
    else
      selectAttributWert1 = "";
  }
  else
    selectAttributWert1 = "_IIII";

  if ( selectAttribut2 != "" )
  {
    pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut2 );
    if (  pGmlAttribut != NULL )
      pPlanObjekt->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut2, selectAttributWert2, pUOM );
    else
      selectAttributWert2 = "";
  }
  else
    selectAttributWert2 = "_IIII";

  if ( selectAttribut3 != "" )
  {
    pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut3 );
    if (  pGmlAttribut != NULL )
      pPlanObjekt->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut3, selectAttributWert3, pUOM );
    else
      selectAttributWert3 = "";
  }
  else
    selectAttributWert3 = "_IIII";

  switch ( pPlanObjekt->getGeometrieTyp() )
  {
  case MULTI_SURFACE:
  case COMPOSITE_SURFACE:
  case SHELL:
  case SURFACE:
  case POLYGON:
    anz = getFlaechenDarstellungen ( vDarstellungen );
    break;

  case MULTI_CURVE:
  case CURVE:
  case LINE_STRING:
	case LINE_STRING_SEGMENT:
  case ARC:
  case CIRCLE:
    anz = getLinienDarstellungen( vDarstellungen );
    break;

  case MULTI_POINT:
  case GEO_POINT:
    anz = getPunktDarstellungen( vDarstellungen);
    break;

  default:
    anz = getDarstellungen ( vDarstellungen );
}

  for ( i = 0; i < anz; i++ )
  {
    pDarstellung = vDarstellungen[i];
    if ( pDarstellung->isSelected ( selectAttributWert1, selectAttributWert2, selectAttributWert3 ) )
    {
      pDarstellungSelected = pDarstellung;
      break;
    }
  }

  if ( pDarstellungSelected == NULL )
  {
    for ( i = 0; i < anz; i++ )
    {
      pDarstellung = vDarstellungen[i];
      if ( pDarstellung->getDefault() == true )
      {
        pDarstellungSelected = pDarstellung;
        break;
      }
    }
  }

  if ( pDarstellungSelected == NULL )
  {
    errorText = "Keine passende Darstellung vorhanden für GML-Klasse: \"";
    errorText += pPlanObjekt->getKlassenName();
    errorText += "\"\nObjekt-ID:  ";
    errorText+= pPlanObjekt->getGmlId();
    pPlanObjekt->getFeatures()->getLogfile()->writeLogFile ( errorText );

    if ( selectAttribut1 != "" )
    {
      errorText = "Select Attribut 1 \"";
      errorText+= selectAttribut1;
      errorText+= "\" hat den Wert: ";
      errorText += selectAttributWert1;
      pPlanObjekt->getFeatures()->getLogfile()->writeLogFile ( errorText );
    }
    else
    {
      errorText = "Select Attribut 1 nicht definiert ";
      pPlanObjekt->getFeatures()->getLogfile()->writeLogFile ( errorText );
    }

    if ( selectAttribut2 != "" )
    {
      errorText = "Select Attribut 2 \"";
      errorText+= selectAttribut2;
      errorText+= "\" hat den Wert: ";
      errorText += selectAttributWert2;
      pPlanObjekt->getFeatures()->getLogfile()->writeLogFile ( errorText );
    }
    else
    {
      errorText = "Select Attribut 2 nicht definiert ";
      pPlanObjekt->getFeatures()->getLogfile()->writeLogFile ( errorText );
    }

    if ( selectAttribut3 != "" )
    {
      errorText = "Select Attribut 3 \"";
      errorText+= selectAttribut3;
      errorText+= "\" hat den Wert: ";
      errorText += selectAttributWert3;
      pPlanObjekt->getFeatures()->getLogfile()->writeLogFile ( errorText );
    }
    else
    {
      errorText = "Select Attribut 3 nicht definiert ";
      pPlanObjekt->getFeatures()->getLogfile()->writeLogFile ( errorText );
    }

    pPlanObjekt->getFeatures()->getLogfile()->writeLogFile ( "\n" );
    return NULL;
  }

  return pDarstellungSelected;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Darstellung hinzu									                        				 //
///////////////////////////////////////////////////////////////////////////////
void GmlKlassenVisual::addDarstellung ( Darstellung * pDarstellungP )
{
	switch ( pDarstellungP->getDarstellungTyp() )
	{
	case Darstellung::FLAECHE:
		vFlaechenDarstellungen.push_back ( (FlaechenDarstellung*)pDarstellungP );
		break;

	case Darstellung::LINIE:
		vLinienDarstellungen.push_back ( (LinienDarstellung*)pDarstellungP );
		break;

	case Darstellung::PUNKT:
		vPunktDarstellungen.push_back ( (PunktDarstellung*)pDarstellungP );
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Flächen-Darstellungen		             											 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlKlassenVisual::getFlaechenDarstellungen ( vector<Darstellung*> &vFlaechenDarstellungenP )
{
	unsigned int i;

	for ( i = 0; i < vFlaechenDarstellungen.size(); i++ )
		vFlaechenDarstellungenP.push_back ( vFlaechenDarstellungen[i] );

	return vFlaechenDarstellungen.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Linien-Darstellungen		             									  		 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlKlassenVisual::getLinienDarstellungen ( vector<Darstellung*> &vLinienDarstellungenP )
{
	unsigned int i;

	for ( i = 0; i < vLinienDarstellungen.size(); i++ )
		vLinienDarstellungenP.push_back ( vLinienDarstellungen[i] );

	return vLinienDarstellungen.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Punkt-Darstellungen		             									  		 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlKlassenVisual::getPunktDarstellungen ( vector<Darstellung*> &vPunktDarstellungenP )
{
	unsigned int i;

	for ( i = 0; i < vPunktDarstellungen.size(); i++ )
		vPunktDarstellungenP.push_back ( vPunktDarstellungen[i] );

	return vPunktDarstellungen.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Darstellungen		                   									  		 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlKlassenVisual::getDarstellungen ( vector<Darstellung*> &vDarstellungenP )
{
	unsigned int i;

	for ( i = 0; i < vFlaechenDarstellungen.size(); i++ )
		vDarstellungenP.push_back ( vFlaechenDarstellungen[i] );

	for ( i = 0; i < vLinienDarstellungen.size(); i++ )
		vDarstellungenP.push_back ( vLinienDarstellungen[i] );

	for ( i = 0; i < vPunktDarstellungen.size(); i++ )
		vDarstellungenP.push_back ( vPunktDarstellungen[i] );

	return vDarstellungenP.size();
}


///////////////////////////////////////////////////////////////////////////////
//  class Darstellung											                            			 //
//  Basisklasse für die SVG-Visualisierungsparameter Klassen	        			 //
///////////////////////////////////////////////////////////////////////////////
Darstellung::Darstellung()
{
	defaultValue = false;
  priority     = -1;
}

Darstellung::~Darstellung()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob die übergebenen Attribut-Werte diese Darstellung	    		 //
//  selektieren			                              													 //
///////////////////////////////////////////////////////////////////////////////
bool Darstellung::isSelected ( string selectAttributWert1P, string selectAttributWert2P,
							   string selectAttributWert3P )
{
//  Spezielle Codierungen:
//
//  IIII - Der Filter akzeptiert jeden bel. Attributwert, auch die Nicht-Belegung
//  BBBB - Das Attribut muss gesetzt seit, der Wert ist aber beliebig
//  NNNN - Das Attribut darf nicht belegt sein

	bool         found1 = false;
	bool         found2 = false;
	bool         found3 = false;
	unsigned int i;
	string       attributWertLoc;

	if ( selectAttributWert1P == "_IIII" )
		found1 = true;
	else
	for ( i = 0; i < vSelectAttributWert1.size(); i++ )
	{
		attributWertLoc = vSelectAttributWert1[i];
		if (  attributWertLoc == "IIII"									  ||
		      ( attributWertLoc == "BBBB" && selectAttributWert1P != "" ) ||
			  ( attributWertLoc == "NNNN" && selectAttributWert1P == "" ) ||
			    attributWertLoc == selectAttributWert1P 
																	)
		{
			found1 = true;
			break;
		}
	}

	if ( selectAttributWert2P == "_IIII" )
		found2 = true;
	else
	for ( i = 0; i < vSelectAttributWert2.size(); i++ )
	{
		attributWertLoc = vSelectAttributWert2[i];
		if (  attributWertLoc == "IIII"									  ||
		      ( attributWertLoc == "BBBB" && selectAttributWert2P != "" ) ||
			  ( attributWertLoc == "NNNN" && selectAttributWert2P == "" ) ||
			    attributWertLoc == selectAttributWert2P 
																	)
		{
			found2 = true;
			break;
		}
	}

	if ( selectAttributWert3P == "_IIII" )
		found3 = true;
	else
	for ( i = 0; i < vSelectAttributWert3.size(); i++ )
	{
		attributWertLoc = vSelectAttributWert3[i];
		if (  attributWertLoc == "IIII"									  ||
		      ( attributWertLoc == "BBBB" && selectAttributWert3P != "" ) ||
			  ( attributWertLoc == "NNNN" && selectAttributWert3P == "" ) ||
			    attributWertLoc == selectAttributWert3P 
																	)
		{
			found3 = true;
			break;
		}
	}

	if ( found1 == true && found2 == true && found3 == true ) return true;
	else													  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle festgelegten Werte des 1. Select-Attributs 								 //
///////////////////////////////////////////////////////////////////////////////
size_t Darstellung::getSelectAttribut1Werte ( std::vector<std::string> & vSelectAttribut1WertP )
{
	unsigned int i;

	for ( i = 0; i < vSelectAttributWert1.size(); i++ )
		vSelectAttribut1WertP.push_back ( vSelectAttributWert1[i] );
	return vSelectAttributWert1.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle festgelegten Werte des 2. Select-Attributs 								 //
///////////////////////////////////////////////////////////////////////////////
size_t Darstellung::getSelectAttribut2Werte ( std::vector<std::string> & vSelectAttribut2WertP )
{
	unsigned int i;

	for ( i = 0; i < vSelectAttributWert2.size(); i++ )
		vSelectAttribut2WertP.push_back ( vSelectAttributWert2[i] );
	return vSelectAttributWert2.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle festgelegten Werte des 3. Select-Attributs 								 //
///////////////////////////////////////////////////////////////////////////////
size_t Darstellung::getSelectAttribut3Werte ( std::vector<std::string> & vSelectAttribut3WertP )
{
	unsigned int i;

	for ( i = 0; i < vSelectAttributWert3.size(); i++ )
		vSelectAttribut3WertP.push_back ( vSelectAttributWert3[i] );
	return vSelectAttributWert3.size();
}

///////////////////////////////////////////////////////////////////////////////
//  class FlaechenDarstellung			                        									 //
//  Visualisierungs-Parameter einer Fläche			                						 //
///////////////////////////////////////////////////////////////////////////////
FlaechenDarstellung::FlaechenDarstellung()
: Darstellung()
{
	deckend				= false;
	pLineTextParams		= NULL;
	pLineSymbolParams   = NULL;

}

FlaechenDarstellung::~FlaechenDarstellung()
{
	unsigned int i;

	for ( i = 0; i < vFlaechenParams.size(); i++ ) delete vFlaechenParams[i];
	if ( pLineTextParams != NULL ) delete pLineTextParams;
	if ( pLineSymbolParams != NULL ) delete pLineSymbolParams;

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Flächen-Visualisierungsparameter		              					 //
///////////////////////////////////////////////////////////////////////////////
size_t FlaechenDarstellung::getFlaechenParams ( std::vector<FlaechenParams*> & vFlaechenParamsP )
{
	unsigned int i;

	for ( i = 0; i < vFlaechenParams.size(); i++ )
		vFlaechenParamsP.push_back ( vFlaechenParams[i] );

	return vFlaechenParams.size();
}


///////////////////////////////////////////////////////////////////////////////
//  class LinienDarstellung								                        					 //
//  Visualisierungs-Parameter einer Linie			                  						 //
///////////////////////////////////////////////////////////////////////////////
LinienDarstellung::LinienDarstellung()
: Darstellung ()
{
	pLineTextParams		= NULL;
	pLineSymbolParams   = NULL;
}

LinienDarstellung::~LinienDarstellung()
{
	unsigned int i;

	for ( i = 0; i < vLinienParams.size(); i++ ) delete vLinienParams[i];
	if ( pLineTextParams != NULL ) delete pLineTextParams;
	if ( pLineSymbolParams != NULL ) delete pLineSymbolParams;

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Linien-Visualisierungsparameter			              					 //
///////////////////////////////////////////////////////////////////////////////
size_t LinienDarstellung::getLinienParams ( std::vector<LinienParams*> & vLinienParamsP )
{
	unsigned int i;

	for ( i = 0; i < vLinienParams.size(); i++ )
		vLinienParamsP.push_back ( vLinienParams[i] );

	return vLinienParams.size();
}

///////////////////////////////////////////////////////////////////////////////
//  class PunktDarstellung							                        						 //
//  Visualisierungs-Parameter eines Punktes				                					 //
///////////////////////////////////////////////////////////////////////////////
PunktDarstellung::PunktDarstellung()
:Darstellung()
{
  symbolSize  = 1.0;
	textOffsetX = 0.0;;
	textOffsetY = 0.0;

}

PunktDarstellung::~PunktDarstellung()
{
}


///////////////////////////////////////////////////////////////////////////////
//  class FlaechenParams				                          									 //
//  Klasse für die SVG-Visualisierungs Parameter einer Fläche			        	 //
///////////////////////////////////////////////////////////////////////////////
FlaechenParams::FlaechenParams ()
{
	classId    = "";
	patternId  = "";
	hatchingId = "";
	clip	   = true;
}
FlaechenParams::~ FlaechenParams()
{
}

///////////////////////////////////////////////////////////////////////////////
//  class LinienParams									                          					 //
//  Klasse für die svg-Visualisierungs Parameter einer Linie	        			 //
///////////////////////////////////////////////////////////////////////////////
LinienParams::LinienParams ()
{
	classId   = "";
}
LinienParams::~ LinienParams()
{
}

///////////////////////////////////////////////////////////////////////////////
//  class LineTextParams						                          							 //
//  Klasse für die svg-Visualisierungs Parameter einer				          		 //
//  Linie mit Hilfe eines Textpfades			                    							 //
///////////////////////////////////////////////////////////////////////////////
LineTextParams::LineTextParams ()
{
	classId    = "";
	textString = "";
	charSize   = 1.0f;
	isWideChar = false;
	textChar   = 0;
}

LineTextParams::~ LineTextParams()
{

}

///////////////////////////////////////////////////////////////////////////////
//  class LineSymbolParams			                          									 //
//  Klasse für die svg-Visualisierungs Parameter  einer	Linie       				 //
//  mit Hilfe von Symbolen						                        							 //
///////////////////////////////////////////////////////////////////////////////
LineSymbolParams::LineSymbolParams ()
{
	symbolId    = "";
	symbolSize  = 1.0f;
	symbolDist  = 20.0f;
	symbolAngle = 0.0f;
}

LineSymbolParams::~ LineSymbolParams()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Color - Farbe									                              						 //
///////////////////////////////////////////////////////////////////////////////
SvgColor::SvgColor ()
{
	colorType = NONE;
	colorName = "";
	for ( int i = 0; i < 3; i++ ) rgb[i] = 0;
}

SvgColor::SvgColor ( std::string colorNameP )
{
	colorType = NAME;
	colorName = colorNameP;
	for ( int i = 0; i < 3; i++ ) rgb[i] = 0;
}

SvgColor::SvgColor ( int rP, int gP, int bP )
{
	colorType = RGB;
	colorName = "";
	rgb[0] = rP;
	rgb[1] = gP;
	rgb[2] = bP;
}

SvgColor::~ SvgColor()
{

}

///////////////////////////////////////////////////////////////////////////////
// Liefert die r, g und b Werte als einzelne Integer-Werte                   //
///////////////////////////////////////////////////////////////////////////////
void SvgColor::getColorRGB ( int &rP, int &gP, int &bP )
{
	rP = rgb[0];
	gP = rgb[1];
	bP = rgb[2];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den rgb-Wert als Text                                            //
///////////////////////////////////////////////////////////////////////////////
string SvgColor::getColorStringRGB()
{
	string   colString;
	String ^ STR;
	string   str;

	colString = "rgb(";

	STR = Convert::ToString ( rgb[0] );
	QuConvert::systemStr2stdStr ( str, STR );
	colString = colString + str + ",";

	STR = Convert::ToString ( rgb[1] );
	QuConvert::systemStr2stdStr ( str, STR );
	colString = colString + str + ",";

	STR = Convert::ToString ( rgb[2] );
	QuConvert::systemStr2stdStr ( str, STR );
	colString = colString + str + ")";

	return colString;
}

///////////////////////////////////////////////////////////////////////////////
//  class Style													                              			 //
//  Klasse zur Modllierung von css-Styles					                  				 //
///////////////////////////////////////////////////////////////////////////////
Style::Style ( std::string styleIdP )
{
	styleId			= styleIdP;
	pFillColor		= NULL;
	pStrokeColor	= NULL;
	fillOpacity		= 1.0;
	strokeWidth		= 1.0;
	strokeLinecap	= "butt";
	strokeDasharray	= "";
	strokeDashoffset= 0;;
	fontSize		= 0.0;
	fontWeight      = "normal";
	textAnchor      = "start";
	fontFamily		= "";
	kerning			= 0.0;
	baselineShift	= "";
}

Style::~ Style()
{
	if ( pFillColor != NULL ) delete pFillColor;
	if ( pStrokeColor != NULL ) delete pStrokeColor;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert einen Eintrag für ein css-Styleshet				              			 //
///////////////////////////////////////////////////////////////////////////////
string Style::getStyleString()
{
	string   styleString;
	string   colString;
	string   str;
	String ^ STR;

	styleString = "." + styleId + " {";

	styleString += "fill:";
	switch ( pFillColor->getColorType() )
	{
	case SvgColor::NONE:
			styleString += "none; ";
			break;

	case SvgColor::NAME:
		styleString += pFillColor->getColorName() + "; ";
		break;

	case SvgColor::RGB:
		colString = pFillColor->getColorStringRGB();
		styleString += colString + "; ";
		break;
	}

	styleString += "stroke:";
	switch ( pStrokeColor->getColorType() )
	{
	case SvgColor::NONE:
			styleString += "none; ";
			break;

	case SvgColor::NAME:
		styleString += pStrokeColor->getColorName() + "; ";
		break;

	case SvgColor::RGB:
		colString = pStrokeColor->getColorStringRGB();
		styleString += colString + "; ";
		break;
	}

	if ( fillOpacity != 1.0 )
	{
		STR = XmlConvert::ToString ( fillOpacity );
		QuConvert::systemStr2stdStr ( str, STR );
		styleString += "fill-opacity:" + str + "; ";
	}

	if ( strokeWidth != 1.0 )
	{
		STR = XmlConvert::ToString ( strokeWidth );
		QuConvert::systemStr2stdStr ( str, STR );
		styleString += "stroke-width:" + str + "; ";
	}

	if ( strokeLinecap != "butt" )
	{
		styleString += "stroke-linecap:" + strokeLinecap + "; ";
	}

	if ( strokeDasharray != "" )
	{
		styleString += "stroke-dasharray:" + strokeDasharray + "; ";
	}

	if ( strokeDashoffset != 0.0 )
	{
		STR = XmlConvert::ToString ( strokeDashoffset );
		QuConvert::systemStr2stdStr ( str, STR );
		styleString += "stroke-dashoffset:" + str + "; ";
	}

	if ( fontSize != 0.0 )
	{
		STR = XmlConvert::ToString ( fontSize );
		QuConvert::systemStr2stdStr ( str, STR );
		styleString += "font-size:" + str + "pt; ";
	}

	if ( fontWeight != "normal" )
	{
		STR = QuConvert::ToString ( fontWeight );
		QuConvert::systemStr2stdStr ( str, STR );
		styleString += "font-weight:" + str + "; ";
	}

	if ( fontFamily != "" )
	{
		STR = QuConvert::ToString ( fontFamily );
		QuConvert::systemStr2stdStr ( str, STR );
		styleString += "font-family:" + str + "; ";
	}

	if ( textAnchor != "start" )
	{
		STR = QuConvert::ToString ( textAnchor );
		QuConvert::systemStr2stdStr ( str, STR );
		styleString += "text-anchor:" + str + "; ";
	}


	if ( kerning != 0.0 )
	{
		STR = XmlConvert::ToString ( kerning );
		QuConvert::systemStr2stdStr ( str, STR );
		styleString += "kerning:" + str + "; ";
	}

	if ( baselineShift != "" )
	{
		STR = QuConvert::ToString ( baselineShift );
		QuConvert::systemStr2stdStr ( str, STR );
		styleString += "baseline-shift:" + str + "; ";
	}

	styleString += "}";

	return styleString;
}

///////////////////////////////////////////////////////////////////////////////
//  class SvgGeometry					                            									 //
//  Basisklasse für SVG Geometrieobjekte                                     //
///////////////////////////////////////////////////////////////////////////////
SvgGeometry::SvgGeometry ( std::string classIdP )
{
	classId = classIdP;
 	
  pFillColor      = NULL;
	pStrokeColor    = NULL;
	strokeWidth     = 0.0;
	strokeDasharray = "";
  fill            = false;
}

SvgGeometry::~SvgGeometry()
{
}

///////////////////////////////////////////////////////////////////////////////
//  class SvgPath		                              													 //
//  Text, der entlang einer Linie dargestellt wird                           //
///////////////////////////////////////////////////////////////////////////////
SvgPath::SvgPath (  std::string classIdP )
:SvgGeometry ( classIdP )
{
	pathString = "";
}

SvgPath::~SvgPath()
{

}

///////////////////////////////////////////////////////////////////////////////
//  class SvgCircle									                            						 //
//  Kreis                                                                    //
///////////////////////////////////////////////////////////////////////////////
SvgCircle::SvgCircle (  std::string classIdP )
:SvgGeometry ( classIdP )
{
	cx = 0.0;
	cy = 0.0;
	r  = 0.0;
}

SvgCircle::~SvgCircle()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Parameter des Kreises                                          //
///////////////////////////////////////////////////////////////////////////////
void SvgCircle::setGeometry ( double cxP, double cyP, double rP )
{
	cx = cxP;
	cy = cyP;
	r  = rP;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Parameter des Kreises                                        //
///////////////////////////////////////////////////////////////////////////////
void SvgCircle::getGeometry ( double &cxP, double &cyP, double &rP )
{
	cxP = cx;
	cyP = cy;
	rP  = r;
}

///////////////////////////////////////////////////////////////////////////////
//  class SvgRect							                              								 //
//  Rechteck                                                                 //
///////////////////////////////////////////////////////////////////////////////
SvgRect::SvgRect (  std::string classIdP )
:SvgGeometry ( classIdP )
{
	x      = 0.0;
	y      = 0.0;
	width  = 0.0;
	height = 0.0;
}

SvgRect::~SvgRect()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Parameter des Rechtecks                                        //
///////////////////////////////////////////////////////////////////////////////
void SvgRect::setGeometry ( double xP, double yP, double widthP, double heightP )
{
	x	   = xP;
	y	   = yP;
	width  = widthP;
	height = heightP;
}
///////////////////////////////////////////////////////////////////////////////
//  Liefert die Parameter des Rechtecks                                      //
///////////////////////////////////////////////////////////////////////////////
void SvgRect::getGeometry ( double &xP, double &yP, double &widthP, double &heightP )
{
	xP      = x;
	yP      = y;
	widthP  = width;
	heightP = height;
}

///////////////////////////////////////////////////////////////////////////////
//  class SvgText						                              									 //
//  Text, der an einer bestimmten Position dargestellt wird                  //
///////////////////////////////////////////////////////////////////////////////
SvgText::SvgText (  std::string classIdP )
:SvgGeometry ( classIdP )
{
	x          = 0.0;
	y          = 0.0;
	size       = 1.0;
	textString = "";
}

SvgText::~SvgText()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Text-Position                                                  //
///////////////////////////////////////////////////////////////////////////////
void SvgText::setPosition ( double xP, double yP )
{
	x = xP;
	y = yP;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Text-Position                                                //
///////////////////////////////////////////////////////////////////////////////
void SvgText::getPosition ( double &xP, double &yP )
{
	xP = x;
	yP = y;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Text-Größe                                                   //
///////////////////////////////////////////////////////////////////////////////
String ^ SvgText::getTextSizePoints()
{
  String ^ STR = XmlConvert::ToString( size );
  STR = String::Concat( STR, "pt" );
  return STR;
}

///////////////////////////////////////////////////////////////////////////////
//  class Symbol								                              							 //
//  Symbol, das aus Geometrie-Elementen zusammengesetzt ist                  //
///////////////////////////////////////////////////////////////////////////////
Symbol::Symbol ( std::string symbolIdP )
{
	symbolId = symbolIdP;
}

Symbol::~Symbol()
{
	for ( unsigned int i = 0; i < vGeometryElements.size(); i++ )
		delete vGeometryElements[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Geometrie-Element hinzu                                         //
///////////////////////////////////////////////////////////////////////////////
void Symbol::addGeometryElement ( SvgGeometry * pElement )
{
	vGeometryElements.push_back ( pElement );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Geometrie-Element über den Index                             //
///////////////////////////////////////////////////////////////////////////////
SvgGeometry * Symbol::getGeometryElement ( unsigned int index )
{
	if ( index < 0 || index >= vGeometryElements.size() ) return NULL;
	else												  return vGeometryElements[index];
}

///////////////////////////////////////////////////////////////////////////////
//  class Marker															                               //
//  Symbole für den Anfang und das Ende einer Linie                          //
///////////////////////////////////////////////////////////////////////////////
Marker::Marker ( std::string markerIdP )
{
	markerId = markerIdP;

	refX	= 0.0;
	refY	= 0.0;
	width	= 0.0;
	height	= 0.0;

}
Marker::~ Marker()
{
	unsigned int i;

	for (  i = 0; i < vGeometryElements.size(); i++ )
		delete vGeometryElements[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Geometrie-Element hinzu                                         //
///////////////////////////////////////////////////////////////////////////////
void Marker::addGeometryElement ( SvgGeometry * pElement )
{
	vGeometryElements.push_back ( pElement );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Geometrie-Element über den Index                             //
///////////////////////////////////////////////////////////////////////////////
SvgGeometry * Marker::getGeometryElement ( unsigned int index )
{
	if ( index < 0 || index >= vGeometryElements.size() ) return NULL;
	else												                          return vGeometryElements[index];
}

///////////////////////////////////////////////////////////////////////////////
//  class Pattern															                               //
//  SVG Muster                                                               //
///////////////////////////////////////////////////////////////////////////////

Pattern::Pattern ( std::string patternIdP )
{
	patternId = patternIdP;
	x		      = 0.0;
	y		      = 0.0;
	width	    = 0.0;
	height    = 0.0;
	angle	    = 0.0;
  scale     = 1.0;

}

Pattern::~Pattern()
{
	for ( unsigned int i = 0; i < vGeometryElements.size(); i++ )
		delete vGeometryElements[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Geometrie-Element hinzu                                         //
///////////////////////////////////////////////////////////////////////////////
void Pattern::addGeometryElement ( SvgGeometry * pElement )
{
	vGeometryElements.push_back ( pElement );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Geometrie-Element über den Index                             //
///////////////////////////////////////////////////////////////////////////////
SvgGeometry * Pattern::getGeometryElement ( unsigned int index )
{
	if ( index < 0 || index >= vGeometryElements.size() ) return NULL;
	else												  return vGeometryElements[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Parameter des Muster-Rechtecks                                 //
///////////////////////////////////////////////////////////////////////////////
void Pattern::setGeometry ( double xP, double yP, double widthP, double heightP )
{
	x	   = xP;
	y	   = yP;
	width  = widthP;
	height = heightP;
}
///////////////////////////////////////////////////////////////////////////////
//  Liefert die Parameter des Muster-Rechtecks                               //
///////////////////////////////////////////////////////////////////////////////
void Pattern::getGeometry ( double &xP, double &yP, double &widthP, double &heightP )
{
	xP      = x;
	yP      = y;
	widthP  = width;
	heightP = height;
}

///////////////////////////////////////////////////////////////////////////////
//  class Hatching							                            								 //
//  Schraffur                                                                //
///////////////////////////////////////////////////////////////////////////////
Hatching::Hatching ( std::string id )
{
	hatchingId = id;

	strokeWidth  = 1.0;
	lineDist     = 10.0;
	pLineColor   = NULL;
	pBaseColor   = NULL;
	hatchingType = "";
}

Hatching::~ Hatching()
{
	if ( pLineColor != NULL ) delete pLineColor;
	if ( pBaseColor != NULL ) delete pBaseColor;
}

///////////////////////////////////////////////////////////////////////////////
//  class SvgEnumItem									                            					 //
// Zuordnung von GML Enumerations-Items zu Texten oder Symbolen              //
///////////////////////////////////////////////////////////////////////////////
SvgEnumItem::SvgEnumItem( SvgEnumeration * pEnumerationP, std::string schluesselNummerA, std::string schluesselNummerB )
{
	pEnumeration     = pEnumerationP;

	schluesselNummer = schluesselNummerA + "_" + schluesselNummerB;
	symbolSize       = 1.0;
	textSymbolSize   = 1.0;
	symbolSizeSet    = false;
	textSymbolSizeSet = false;
}

SvgEnumItem:: ~SvgEnumItem()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Skalierung des zugeordneten Symbols                            //
///////////////////////////////////////////////////////////////////////////////
void SvgEnumItem::setSymbolSize ( double symbolSizeP )
{
	symbolSize    = symbolSizeP;
	symbolSizeSet = true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Skalierung des zugeordneten Symbols                          //
///////////////////////////////////////////////////////////////////////////////
double SvgEnumItem::getSymbolSize ()
{
	if ( symbolSizeSet == true ) return symbolSize;
	else						 return pEnumeration->getDefaultSymbolSize();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das zusätzliche Text-Symbol                                      //
///////////////////////////////////////////////////////////////////////////////
std::string  SvgEnumItem::getTextSymbol ()
{
	if ( textSymbol != "" ) return textSymbol;
	else					          return pEnumeration->getDefaultTextSymbol();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Text-Stil                                                    //
///////////////////////////////////////////////////////////////////////////////
std::string SvgEnumItem::getTextStyle ()
{
	if ( textStyle != "" ) return textStyle;
	else				   return pEnumeration->getDefaultTextStyle();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Text- und TextSymbol Skalierung                                //
///////////////////////////////////////////////////////////////////////////////
void  SvgEnumItem::setTextSymbolSize ( double symbolSizeP )
{
	textSymbolSize    = symbolSizeP;
	textSymbolSizeSet = true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Text- und ext-Symbol Skalierung                              //
///////////////////////////////////////////////////////////////////////////////
double  SvgEnumItem::getTextSymbolSize ()
{
	if ( textSymbolSizeSet == true ) return textSymbolSize;
	else						     return pEnumeration->getDefaultTextSymbolSize();
}

///////////////////////////////////////////////////////////////////////////////
//  class Enumeration						                            								 //
//  Darstellungsparameter für eine GML-Enumeration                           //
///////////////////////////////////////////////////////////////////////////////
SvgEnumeration::SvgEnumeration ( string nameP )
{
	name                  = nameP;
	defaultSymbolSize     = 1.0;
	defaultTextSymbolSize = 1.0; 
}

SvgEnumeration::SvgEnumeration ( std::string nameA, std::string nameB )
{
	name				  = nameA + "_" + nameB;
	defaultSymbolSize     = 1.0;
	defaultTextSymbolSize = 1.0; 
}

SvgEnumeration::~SvgEnumeration()
{
	map<string,SvgEnumItem*>::iterator iter;

	for ( iter = enumItems.begin(); iter != enumItems.end(); iter++ )
		delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt die Parameter für ein Enumerations-Item hinzu                       //
///////////////////////////////////////////////////////////////////////////////
void SvgEnumeration::addEnumItem ( SvgEnumItem * pEnumItemP )
{
	string schluesselNummer = pEnumItemP->getSchluesselNummer();
	enumItems[schluesselNummer] = pEnumItemP;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das SvgEnumItem Objekt, das den beiden übergebenen Schlüssel-    //
//  numern zugeordnet ist                                                    //
///////////////////////////////////////////////////////////////////////////////
SvgEnumItem * SvgEnumeration::getEnumItem ( std::string schluesselNummerA, std::string schluesselNummerB )
{
	map<string,SvgEnumItem*>::iterator iter;

	string schluesselNummer = schluesselNummerA + "_" + schluesselNummerB;

	iter = enumItems.find ( schluesselNummer );
	if ( iter != enumItems.end() ) return iter->second;
	else						   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  class SvgVisualisierung		                        											 //
//  Zusammenfassung aller SVG-Darstellungsparameter                          //
///////////////////////////////////////////////////////////////////////////////
SvgVisualisierung::SvgVisualisierung ()
{
	planTyp = "";
	datum   = "";

	breiteUeberschrift = 800;
	hoeheUeberschrift  = 30;

  breiteDisplay = 0.3f;
  hoeheDisplay  = 0.2f;
}

SvgVisualisierung::~SvgVisualisierung()
{
	map<string,GmlKlassenVisual*>::iterator    iterGmlKlassen;
	map<string,Style*>::iterator			         iterStyles;
	map<string,Symbol*>::iterator			         iterSymbols;
	map<string,SvgEnumeration*>::iterator      iterEnumerations;      
  map<string,FlaechenDarstellung*>::iterator iterFlaechenDarstellungen;
  map<string,LinienDarstellung*>::iterator   iterLinienDarstellungen;
  map<string,Marker*>::iterator              iterMarker;
  map<string,Pattern*>::iterator             iterPatterns;
  map<string,Hatching*>::iterator            iterHatchings;

	for ( iterGmlKlassen = mGmlKlassen.begin(); iterGmlKlassen != mGmlKlassen.end(); iterGmlKlassen++ )
		delete iterGmlKlassen->second;

	for ( iterStyles = mStyles.begin(); iterStyles != mStyles.end(); iterStyles++ )
		delete iterStyles->second;

	for ( iterSymbols = mSymbols.begin(); iterSymbols != mSymbols.end(); iterSymbols++ )
		delete iterSymbols->second;

	for ( iterEnumerations = mEnumerations.begin(); iterEnumerations != mEnumerations.end(); iterEnumerations++ )
		delete iterEnumerations->second;

	for ( iterPatterns = mPatterns.begin(); iterPatterns != mPatterns.end(); iterPatterns++ )
		delete iterPatterns->second;

  for ( iterMarker = mMarkers.begin(); iterMarker != mMarkers.end(); iterMarker++ )
		delete iterMarker->second;

  for ( iterHatchings = mHatchings.begin(); iterHatchings != mHatchings.end(); iterHatchings++ )
		delete iterHatchings->second;

  for ( iterFlaechenDarstellungen = mFlaechenStyles.begin(); iterFlaechenDarstellungen != mFlaechenStyles.end(); iterFlaechenDarstellungen++ )
		delete iterFlaechenDarstellungen->second;

  for ( iterLinienDarstellungen = mLinienStyles.begin(); iterLinienDarstellungen != mLinienStyles.end(); iterLinienDarstellungen++ )
		delete iterLinienDarstellungen->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen einer Visualisierungsvorschrift           										 //
///////////////////////////////////////////////////////////////////////////////
void SvgVisualisierung::addGmlKlassenVisual ( GmlKlassenVisual * pGmlKlasse )
{
	string id = pGmlKlasse->getGmlKlassenName();
	mGmlKlassen[id] = pGmlKlasse;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Styles                              										 //
///////////////////////////////////////////////////////////////////////////////
void SvgVisualisierung::addStyle ( Style * pStyle )
{
	string id = pStyle->getStyleId();
	mStyles[id] = pStyle;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Symbols                             										 //
///////////////////////////////////////////////////////////////////////////////
void SvgVisualisierung::addSymbol ( Symbol * pSymbol )
{
	string id = pSymbol->getSymbolId();
	mSymbols[id] = pSymbol;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Markers                             										 //
///////////////////////////////////////////////////////////////////////////////
void SvgVisualisierung::addMarker ( Marker * pMarker )
{
	string id = pMarker->getMarkerId();
	mMarkers[id] = pMarker;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen einer Enumeration                         										 //
///////////////////////////////////////////////////////////////////////////////
void SvgVisualisierung::addEnumeration ( SvgEnumeration * pEnumerationP )
{
	string id = pEnumerationP->getName();
	mEnumerations[id] = pEnumerationP;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Musters                             										 //
///////////////////////////////////////////////////////////////////////////////
void SvgVisualisierung::addPattern ( Pattern * pPattern )
{
	string id = pPattern->getPatternId();
	mPatterns[id] = pPattern;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen einer Schraffur                           										 //
///////////////////////////////////////////////////////////////////////////////
void SvgVisualisierung::addHatching ( Hatching * pHatching )
{
	string id = pHatching->getHatchingId();
	mHatchings[id] = pHatching;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Flächen-Darstellungsstils           										 //
///////////////////////////////////////////////////////////////////////////////
void SvgVisualisierung::addFlaechenStyle ( FlaechenDarstellung * pFlaechenStyle )
{
  string id = pFlaechenStyle->getId();
  if ( id != "" )
    mFlaechenStyles[id] = pFlaechenStyle;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Linien-Darstellungsstils           										 //
///////////////////////////////////////////////////////////////////////////////
void SvgVisualisierung::addLinienStyle ( LinienDarstellung * pLinienStyle )
{
  string id = pLinienStyle->getId();
  if ( id != "" )
    mLinienStyles[id] = pLinienStyle;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernen eines Flächen-Darstellungsstils           										 //
///////////////////////////////////////////////////////////////////////////////
void SvgVisualisierung::removeFlaechenStyle ( std::string id )
{
  mFlaechenStyles.erase ( id );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernen eines Linien-Darstellungsstils            										 //
///////////////////////////////////////////////////////////////////////////////
void SvgVisualisierung::removeLinienStyle (  std::string id )
{
  mLinienStyles.erase ( id );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die zu einer GML-Klasse gehörigen Darstellungsparameter  				 //
///////////////////////////////////////////////////////////////////////////////
GmlKlassenVisual * SvgVisualisierung::getGmlKlassenVisual ( std::string gmlKlassenName )
{
	map<string,GmlKlassenVisual*>::iterator iter;
	
	iter = mGmlKlassen.find ( gmlKlassenName );
	if ( iter != mGmlKlassen.end() )
		return iter->second;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen css-Style                                                  //
///////////////////////////////////////////////////////////////////////////////
Style * SvgVisualisierung::getStyle ( std::string styleId )
{
	map<string,Style*>::iterator iter;

	iter = mStyles.find ( styleId );
	if ( iter != mStyles.end() ) 
		return iter->second;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Symbol                                                       //
///////////////////////////////////////////////////////////////////////////////
Symbol * SvgVisualisierung::getSymbol ( std::string symbolId )
{
	std::map<std::string,Symbol*>::iterator iter;

	iter = mSymbols.find ( symbolId );
	if ( iter != mSymbols.end() )
		return iter->second;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen Marker                                                     //
///////////////////////////////////////////////////////////////////////////////
Marker * SvgVisualisierung::getMarker ( std::string markerId )
{
	std::map<std::string,Marker*>::iterator iter;

	iter = mMarkers.find ( markerId );
	if ( iter != mMarkers.end() )
		return iter->second;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Muster                                                       //
///////////////////////////////////////////////////////////////////////////////
Pattern * SvgVisualisierung::getPattern ( std::string patternId )
{
	std::map<std::string,Pattern*>::iterator iter;

	iter = mPatterns.find ( patternId );
	if ( iter != mPatterns.end() )
		return iter->second;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Schraffur                                                   //
///////////////////////////////////////////////////////////////////////////////
Hatching * SvgVisualisierung::getHatching ( std::string hatchingId )
{
	std::map<std::string,Hatching*>::iterator iter;

	iter = mHatchings.find ( hatchingId );
	if ( iter != mHatchings.end() )
		return iter->second;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Darstellungsvorschriften für GML-Klassen                    //
///////////////////////////////////////////////////////////////////////////////
size_t SvgVisualisierung::getGmlKlassenVisualAll ( std::vector<GmlKlassenVisual*> & vVisual )
{
	map<string,GmlKlassenVisual*>::iterator iter;

	for ( iter = mGmlKlassen.begin(); iter != mGmlKlassen.end(); iter++ )
		vVisual.push_back ( iter->second );

	return vVisual.size();

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Symbole							                          						 //
///////////////////////////////////////////////////////////////////////////////
size_t SvgVisualisierung::getSymbols ( vector<Symbol*> &vSymbolsP )
{
	map<string,Symbol*>::iterator iter;

	for ( iter = mSymbols.begin(); iter != mSymbols.end(); iter++ )
	{
		Symbol * pSymbol = iter->second;
		vSymbolsP.push_back ( pSymbol );
	}

	return mSymbols.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Marker				                          										 //
///////////////////////////////////////////////////////////////////////////////
size_t SvgVisualisierung::getMarkers ( vector<Marker*> &vMarkersP )
{
	map<string,Marker*>::iterator iter;

	for ( iter = mMarkers.begin(); iter != mMarkers.end(); iter++ )
	{
		Marker * pMarker = iter->second;
		vMarkersP.push_back ( pMarker );
	}

	return mMarkers.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Styles		                            											 //
///////////////////////////////////////////////////////////////////////////////
size_t SvgVisualisierung::getStyles ( vector<Style*> &vStylesP )
{
	map<string,Style*>::iterator iter;

	for ( iter = mStyles.begin(); iter != mStyles.end(); iter++ )
	{
		Style * pStyle = iter->second;
		vStylesP.push_back ( pStyle );
	}

	return mStyles.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Muster 			                          										 //
///////////////////////////////////////////////////////////////////////////////
size_t SvgVisualisierung::getPatterns ( vector<Pattern*> &vPatternsP )
{
	map<string,Pattern*>::iterator iter;

	for ( iter = mPatterns.begin(); iter != mPatterns.end(); iter++ )
	{
		Pattern * pPattern = iter->second;
		vPatternsP.push_back ( pPattern );
	}

	return mPatterns.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Schraffuren					                        							 //
///////////////////////////////////////////////////////////////////////////////
size_t SvgVisualisierung::getHatchings ( vector<Hatching*> &vHatchingsP )
{
	map<string,Hatching*>::iterator iter;

	for ( iter = mHatchings.begin(); iter != mHatchings.end(); iter++ )
	{
		Hatching * pHatching = iter->second;
		vHatchingsP.push_back ( pHatching );
	}

	return mHatchings.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Darstellungsparameter für eine Enumeration     							 //
///////////////////////////////////////////////////////////////////////////////
SvgEnumeration * SvgVisualisierung::getEnumeration ( std::string enumNameP )
{
	map<string,SvgEnumeration*>::iterator iter;

	iter = mEnumerations.find ( enumNameP );
	if ( iter != mEnumerations.end() ) return iter->second;
	else							   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die kombinierten Darstellungsparameter für zwei Enumerationen		 //
///////////////////////////////////////////////////////////////////////////////
SvgEnumeration * SvgVisualisierung::getEnumeration ( std::string enumNameA, std::string enumNameB )
{
	map<string,SvgEnumeration*>::iterator iter;
	
	string name = enumNameA + "_" + enumNameB;

	iter = mEnumerations.find ( name );
	if ( iter != mEnumerations.end() ) return iter->second;
	else							   return NULL;

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Darstellungsparameter für ein Enumerations-Item	einer best. 		 //
//  Enumeration                                                              //    
///////////////////////////////////////////////////////////////////////////////
SvgEnumItem * SvgVisualisierung::getEnumItem ( string enumNameP, string schluesselNummerP )
{
	SvgEnumeration * pEnumeration;
	SvgEnumItem    * pEnumItem;

	pEnumeration = getEnumeration ( enumNameP );
	if ( pEnumeration == NULL ) return NULL;

	pEnumItem = pEnumeration->getEnumItem ( schluesselNummerP, "" );
	return pEnumItem;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Darstellungsparameter für kombinierte Enumerationen              //    
///////////////////////////////////////////////////////////////////////////////
SvgEnumItem * SvgVisualisierung::getEnumItem ( string enumNameA, string enumNameB,
											   string schluesselNummerA, string schluesselNummerB )
{
	SvgEnumeration * pEnumeration;
	SvgEnumItem    * pEnumItem;

	pEnumeration = getEnumeration ( enumNameA, enumNameB );
	if ( pEnumeration == NULL ) return NULL;

	pEnumItem = pEnumeration->getEnumItem ( schluesselNummerA, schluesselNummerB );
	return pEnumItem;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Flächen-Darstellung                                         //
///////////////////////////////////////////////////////////////////////////////
FlaechenDarstellung * SvgVisualisierung::getFlaechenDarstellung ( std::string id )
{
  map<string,FlaechenDarstellung*>::iterator pos;

  pos = mFlaechenStyles.find ( id );
  if ( pos != mFlaechenStyles.end() )
    return pos->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Linien-Darstellung                                          //
///////////////////////////////////////////////////////////////////////////////
LinienDarstellung * SvgVisualisierung::getLinienDarstellung ( std::string id )
{
  map<string,LinienDarstellung*>::iterator pos;

  pos = mLinienStyles.find ( id );
  if ( pos != mLinienStyles.end() )
    return pos->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  __gc class SvgParamReader					                        							 //
//  Einlesen der SVG-Darstellungsvprschriften                                //
///////////////////////////////////////////////////////////////////////////////
SvgParamReader::SvgParamReader ( double globalScaleP, String ^ gmlTypeP)
{
	pSvgParams  = NULL;
	globalScale = globalScaleP;

	pGmlType    = gmlTypeP;
}

SvgParamReader::~SvgParamReader()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen						                                										 //
///////////////////////////////////////////////////////////////////////////////
bool SvgParamReader::read( string fileName, SvgVisualisierung * pSvgParamsP  )
{
	String ^ fName;
	String ^ STR;
	string   stdString;
	bool     first = true;
//	System::Windows::Forms::DialogResult weiter;

	pSvgParams = pSvgParamsP;
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
				if ( pActElement->CompareTo("SvgVisualisierung") != 0 )
				{
					MessageBox::Show( "Ungültige Steuerdatei für SVG-Visualisierung");
					break;
				}
				else
				{
					STR = reader->GetAttribute ( "datum" );
					if ( STR != nullptr )
					{
						QuConvert::systemStr2stdStr ( stdString, STR );
						pSvgParams->setDatum ( stdString );
					}
				}
			}

			if ( pActElement->CompareTo("GmlKlassenVisual") == 0 )
			{
				readGmlKlassenVisual();
			}
			else
			if ( pActElement->CompareTo("Style") == 0 )
			{
				readStyle();
			}
			else
			if ( pActElement->CompareTo("Symbol") == 0 )
			{
				readSymbol();
			}
			else
			if ( pActElement->CompareTo("Marker") == 0 )
			{
				readMarker();
			}
			else
			if ( pActElement->CompareTo("Enumeration") == 0 )
			{
				readEnumeration();
			}
			else
			if ( pActElement->CompareTo("Pattern") == 0 )
			{
				readPattern();
			}
			else
			if ( pActElement->CompareTo("Hatching") == 0 )
			{
				readHatching();
			}
			else
			if ( pActElement->CompareTo("FlaechenDarstellung") == 0 )
			{
				FlaechenDarstellung * pFlaechenDarstellung = readFlaechenDarstellung();
        pSvgParams->addFlaechenStyle ( pFlaechenDarstellung );
			}
			else
			if ( pActElement->CompareTo("LinienDarstellung") == 0 )
			{
				LinienDarstellung * pLinienDarstellung = readLinienDarstellung();
        pSvgParams->addLinienStyle ( pLinienDarstellung );
			}
		}
		else
		{
			if ( reader->NodeType == XmlNodeType::EndElement )
			{
				pActElement = reader->LocalName;
				if ( pActElement->CompareTo("SvgVisualisierung") == 0 ) break;
			}
		}
	}

	reader->Close();

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen SvgFlaeche					                          									 //
///////////////////////////////////////////////////////////////////////////////
void SvgParamReader::readGmlKlassenVisual()
{
	String ^ STR;
	string   stdString;
  string   id;

	GmlKlassenVisual * pGmlKlassenVisual = new GmlKlassenVisual( pSvgParams );

	STR = reader->GetAttribute ( "gmlKlassenName" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceAttribut( stdString );
		pGmlKlassenVisual->setGmlKlassenName ( stdString );
		pSvgParams->addGmlKlassenVisual ( pGmlKlassenVisual );
	}

	STR = reader->GetAttribute ( "priorityFlaechendarstellung" );
	if ( STR != nullptr )
	{
		int ih = XmlConvert::ToInt32 ( STR );
		pGmlKlassenVisual->setPriorityFlaechendarstellung ( ih );
	}

	STR = reader->GetAttribute ( "priorityLiniendarstellung" );
	if ( STR != nullptr )
	{
		int ih = XmlConvert::ToInt32 ( STR );
		pGmlKlassenVisual->setPriorityLiniendarstellung ( ih );
	}

	STR = reader->GetAttribute ( "priorityPunktdarstellung" );
	if ( STR != nullptr )
	{
		int ih = XmlConvert::ToInt32 ( STR );
		pGmlKlassenVisual->setPriorityPunktdarstellung ( ih );
	}

	STR = reader->GetAttribute ( "selectAttribut1" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceAttribut( stdString );
		pGmlKlassenVisual->setSelectAttribut1 ( stdString );
	}

	STR = reader->GetAttribute ( "selectAttribut2" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceAttribut( stdString );
		pGmlKlassenVisual->setSelectAttribut2 ( stdString );
	}

	STR = reader->GetAttribute ( "selectAttribut3" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceAttribut( stdString );
		pGmlKlassenVisual->setSelectAttribut3 ( stdString );
	}

	STR = reader->GetAttribute ( "defaultSymbol" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pGmlKlassenVisual->setDefaultSymbol ( stdString );
	}

	STR = reader->GetAttribute ( "defaultSymbolSize" );
	if ( STR != nullptr )
	{
		double dh = XmlConvert::ToDouble ( STR );
		pGmlKlassenVisual->setDefaultSymbolSize ( dh*globalScale );
	}
  else
    pGmlKlassenVisual->setDefaultSymbolSize ( globalScale );

	if ( reader->IsEmptyElement )
		return;
	
	while( reader->Read() )
	{
		if ( reader->NodeType == XmlNodeType::Element )
		{
			pActElement = reader->LocalName;
			if ( pActElement->CompareTo ( "FlaechenDarstellung") == 0 )
      {
				FlaechenDarstellung * pFlaechenDarstellung = readFlaechenDarstellung ();
        pGmlKlassenVisual->addDarstellung ( pFlaechenDarstellung );
        id = pFlaechenDarstellung->getId();
        if ( id != "" ) pSvgParams->addFlaechenStyle ( pFlaechenDarstellung );
      }
			else
			if ( pActElement->CompareTo ( "LinienDarstellung") == 0 )
      {
				LinienDarstellung * pLinienDarstellung = readLinienDarstellung ();
        pGmlKlassenVisual->addDarstellung ( pLinienDarstellung );
        id = pLinienDarstellung->getId();
        if ( id != "" ) pSvgParams->addLinienStyle ( pLinienDarstellung );
      }
			else
			if ( pActElement->CompareTo ( "PunktDarstellung") == 0 )
      {
				PunktDarstellung * pPunktDarstellung = readPunktDarstellung ();
        pGmlKlassenVisual->addDarstellung ( pPunktDarstellung );
      }
		}
		else
		if ( reader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = reader->LocalName;
			if (pActElement->CompareTo("GmlKlassenVisual") == 0 ) break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Tag FlaechenDarstellung						                    				 //
///////////////////////////////////////////////////////////////////////////////
FlaechenDarstellung * SvgParamReader::readFlaechenDarstellung ()
{
	String ^ STR;
	string   stdString;
	bool     boolValue;
  int      intValue;

	FlaechenDarstellung * pFlaechenDarstellung = new FlaechenDarstellung;

	STR = reader->GetAttribute ( "default" );
	if ( STR != nullptr )
	{
		boolValue = XmlConvert::ToBoolean ( STR );
		pFlaechenDarstellung->setDefault ( boolValue );
	}

  STR = reader->GetAttribute( "priority" );
  if ( STR != nullptr )
  {
    intValue = XmlConvert::ToInt32 ( STR );
    pFlaechenDarstellung->setPriority( intValue );
  }
  
  STR = reader->GetAttribute ( "deckend" );
	if ( STR != nullptr )
	{
		boolValue = XmlConvert::ToBoolean ( STR );
		pFlaechenDarstellung->setDeckend ( boolValue );
	}

  STR = reader->GetAttribute ( "id" );
	if ( STR != nullptr )
	{
    QuConvert::systemStr2stdStr( stdString,  STR );
		pFlaechenDarstellung->setId ( stdString );
	}

	if ( reader->IsEmptyElement ) return pFlaechenDarstellung;

	while( reader->Read() )
	{
		if ( reader->NodeType == XmlNodeType::Element )
		{
			pActElement = reader->LocalName;

			if ( pActElement->CompareTo("SelectAttributWert1") == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pFlaechenDarstellung->addSelectAttributWert1 ( stdString );
				}
			}
			else
			if ( pActElement->CompareTo("SelectAttributWert2") == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pFlaechenDarstellung->addSelectAttributWert2 ( stdString );
				}
			}
			else
			if ( pActElement->CompareTo("SelectAttributWert3") == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pFlaechenDarstellung->addSelectAttributWert3 ( stdString );
				}
			}
			else
			if ( pActElement->CompareTo("FlaechenParams") == 0 )
				readFlaechenParams ( pFlaechenDarstellung );
			else
			if ( pActElement->CompareTo("LineTextParams") == 0 )
				readLineTextParams ( pFlaechenDarstellung );
			else
			if ( pActElement->CompareTo("LineSymbolParams") == 0 )
				readLineSymbolParams ( pFlaechenDarstellung );
		}
		else
		{
			if ( reader->NodeType == XmlNodeType::EndElement )
			{
				pActElement = reader->LocalName;
				if (pActElement->CompareTo("FlaechenDarstellung") == 0 ) break;
			}
		}
	}
  return pFlaechenDarstellung;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Tag FlaechenParams				                      								 //
///////////////////////////////////////////////////////////////////////////////
void SvgParamReader::readFlaechenParams ( FlaechenDarstellung * pFlaechenDarstellung )
{
	String ^ STR;
	string   stdString;
	bool     boolValue;

	FlaechenParams * pFlaechenParams = new FlaechenParams;
	pFlaechenDarstellung->addFlaechenParams ( pFlaechenParams );

	STR = reader->GetAttribute ( "class" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pFlaechenParams->setClassId ( stdString );
	}

	STR = reader->GetAttribute ( "clip" );
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
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Tag LineTextParams		                      										 //
///////////////////////////////////////////////////////////////////////////////
void SvgParamReader::readLineTextParams ( Darstellung * pDarstellung )
{
	String ^ STR;
	String ^ TEXT_STRING;
	string   stdString;
	bool     boolValue;
	double   doubleValue;

	LineTextParams * pLineTextParams = new LineTextParams;

	if ( pDarstellung->getDarstellungTyp() == Darstellung::FLAECHE )
		((FlaechenDarstellung*)pDarstellung)->setLineTextParams ( pLineTextParams );
	else
	if ( pDarstellung->getDarstellungTyp() == Darstellung::LINIE )
		((LinienDarstellung*)pDarstellung)->setLineTextParams ( pLineTextParams );
	else
		return;

	STR = reader->GetAttribute ( "class" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pLineTextParams->setClassId ( stdString );
	}

	TEXT_STRING = reader->GetAttribute ( "text" );
	if ( TEXT_STRING != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, TEXT_STRING );
		pLineTextParams->setTextString ( stdString );
	}

	STR = reader->GetAttribute ( "charSize" );
	if ( STR != nullptr )
	{
		doubleValue = XmlConvert::ToDouble ( STR );
		pLineTextParams->setCharSize ( doubleValue*globalScale );
	}
    pLineTextParams->setCharSize ( globalScale );

	STR = reader->GetAttribute ( "wideChar" );
	if ( STR != nullptr )
	{
		boolValue = XmlConvert::ToBoolean ( STR );
		pLineTextParams->setIsWideChar ( boolValue );
	}

	if ( pLineTextParams->getIsWideChar() )
	{
		__wchar_t i = XmlConvert::ToInt32( TEXT_STRING );
		pLineTextParams->setTextChar ( i );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Tag LineSymbolParams		                      									 //
///////////////////////////////////////////////////////////////////////////////
void SvgParamReader::readLineSymbolParams ( Darstellung * pDarstellung )
{
	String ^ STR;
	string   stdString;
	double   doubleValue;

	LineSymbolParams * pLineSymbolParams = new LineSymbolParams;

	if ( pDarstellung->getDarstellungTyp() == Darstellung::FLAECHE )
		((FlaechenDarstellung*)pDarstellung)->setLineSymbolParams ( pLineSymbolParams );
	else
	if ( pDarstellung->getDarstellungTyp() == Darstellung::LINIE )
		((LinienDarstellung*)pDarstellung)->setLineSymbolParams ( pLineSymbolParams );
	else
		return;

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
//  Einlesen Tag LinienDarstellung	                      									 //
///////////////////////////////////////////////////////////////////////////////
LinienDarstellung *  SvgParamReader::readLinienDarstellung ()
{
	String ^ STR;
	string   stdString;
	bool     boolValue;
  int      intValue;

	LinienDarstellung * pLinienDarstellung = new LinienDarstellung;

	STR = reader->GetAttribute ( "default" );
	if ( STR != nullptr )
	{
		boolValue = XmlConvert::ToBoolean ( STR );
		pLinienDarstellung->setDefault ( boolValue );
	}

  STR = reader->GetAttribute ( "id" );
	if ( STR != nullptr )
	{
    QuConvert::systemStr2stdStr( stdString,  STR );
		pLinienDarstellung->setId ( stdString );
	}

  STR = reader->GetAttribute( "priority" );
  if ( STR != nullptr )
  {
    intValue = XmlConvert::ToInt32 ( STR );
    pLinienDarstellung->setPriority( intValue );
  }

	STR = reader->GetAttribute ( "startMarker" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pLinienDarstellung->setStartMarker ( stdString );
	}

	STR = reader->GetAttribute ( "endMarker" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pLinienDarstellung->setEndMarker ( stdString );
	}

	if ( reader->IsEmptyElement ) return pLinienDarstellung;

	while( reader->Read() )
	{
		if ( reader->NodeType == XmlNodeType::Element )
		{
			pActElement = reader->LocalName;

			if ( pActElement->CompareTo("SelectAttributWert1") == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pLinienDarstellung->addSelectAttributWert1 ( stdString );
				}
			}
			else
			if ( pActElement->CompareTo("SelectAttributWert2") == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pLinienDarstellung->addSelectAttributWert2 ( stdString );
				}
			}
			else
			if ( pActElement->CompareTo("SelectAttributWert3") == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pLinienDarstellung->addSelectAttributWert3 ( stdString );
				}
			}
			else
			if ( pActElement->CompareTo("LinienParams") == 0 )
				readLinienParams ( pLinienDarstellung );
			else
			if ( pActElement->CompareTo("LineTextParams") == 0 )
				readLineTextParams ( pLinienDarstellung );
			else
			if ( pActElement->CompareTo("LineSymbolParams") == 0 )
				readLineSymbolParams ( pLinienDarstellung );
		}
		else
		{
			if ( reader->NodeType == XmlNodeType::EndElement )
			{
				pActElement = reader->LocalName;
				if (pActElement->CompareTo("LinienDarstellung") == 0 ) break;
			}
		}
	}
  return pLinienDarstellung;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Tag LinienParams   											                       //
///////////////////////////////////////////////////////////////////////////////
void SvgParamReader::readLinienParams ( LinienDarstellung * pLinienDarstellung )
{
	String ^ STR;
	string   stdString;

	LinienParams * pLinienParams = new LinienParams;
	pLinienDarstellung->addLinienParams ( pLinienParams );

	STR = reader->GetAttribute ( "class" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pLinienParams->setClassId ( stdString );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Tag PunktDarstellung                     											 //
///////////////////////////////////////////////////////////////////////////////
PunktDarstellung * SvgParamReader::readPunktDarstellung ()
{
	String ^ STR;
	string   stdString;
	bool     boolValue;
	double   h;
  int      intValue;

	PunktDarstellung * pPunktDarstellung = new PunktDarstellung;

	STR = reader->GetAttribute ( "default" );
	if ( STR != nullptr )
	{
		boolValue = XmlConvert::ToBoolean ( STR );
		pPunktDarstellung->setDefault ( boolValue );
	}

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
    stdString = addDefaultNamespaceAttribut( stdString );
		pPunktDarstellung->setAttributA ( stdString );
	}

	STR = reader->GetAttribute ( "attributB" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceAttribut( stdString );
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
    stdString = addDefaultNamespaceAttribut( stdString );
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

			if ( pActElement->CompareTo("SelectAttributWert1") == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pPunktDarstellung->addSelectAttributWert1 ( stdString );
				}
			}
			else
			if ( pActElement->CompareTo("SelectAttributWert2") == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pPunktDarstellung->addSelectAttributWert2 ( stdString );
				}
			}
			else
			if ( pActElement->CompareTo("SelectAttributWert3") == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pPunktDarstellung->addSelectAttributWert3 ( stdString );
				}
			}
		}
		else
		{
			if ( reader->NodeType == XmlNodeType::EndElement )
			{
				pActElement = reader->LocalName;
				if (pActElement->CompareTo("PunktDarstellung") == 0 ) break;
			}
		}
	}
  return pPunktDarstellung;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Style									                            						 //
///////////////////////////////////////////////////////////////////////////////
void SvgParamReader::readStyle()
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
	pSvgParams->addStyle ( pStyle );

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
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Color							                            								 //
///////////////////////////////////////////////////////////////////////////////
SvgColor * SvgParamReader::readColor ( String ^ endTagP )
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
void SvgParamReader::readSymbol()
{
	String ^ STR;
	string   stdString;

	STR = reader->GetAttribute ( "name" );

	QuConvert::systemStr2stdStr ( stdString, STR );
	Symbol * pSymbol = new Symbol ( stdString );
	pSvgParams->addSymbol ( pSymbol );

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
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen von Darstellungsparametern für SvgPath, SvgCircle und SvgRect   //
///////////////////////////////////////////////////////////////////////////////
void SvgParamReader::readGeometryParams ( SvgGeometry * pSvgGeometry, String ^endTag )
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
//  Einlesen Marker													                            		 //
///////////////////////////////////////////////////////////////////////////////
void SvgParamReader::readMarker()
{
	String ^ STR;
	string   stdString;
	double   h;

	STR = reader->GetAttribute ( "name" );
	QuConvert::systemStr2stdStr ( stdString, STR );
	Marker * pMarker = new Marker ( stdString );
	pSvgParams->addMarker ( pMarker );

	STR = reader->GetAttribute ( "viewBox" );
	QuConvert::systemStr2stdStr ( stdString, STR );
	pMarker->setViewbox ( stdString );

	STR = reader->GetAttribute ( "refX" );
	h = XmlConvert::ToDouble ( STR );
	pMarker->setRefX ( h );

	STR = reader->GetAttribute ( "refY" );
	h = XmlConvert::ToDouble ( STR );
	pMarker->setRefY ( h );

	STR = reader->GetAttribute ( "width" );
	h = XmlConvert::ToDouble ( STR );
	pMarker->setWidth ( h );

	STR = reader->GetAttribute ( "height" );
	h = XmlConvert::ToDouble ( STR );
	pMarker->setHeight ( h );

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

				pMarker->addGeometryElement ( pSvgPath );
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

				pMarker->addGeometryElement ( pSvgCircle );
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

				pMarker->addGeometryElement ( pSvgRect );
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

				pMarker->addGeometryElement ( pSvgText );
			}
		}
		else
		{
			if ( reader->NodeType == XmlNodeType::EndElement )
			{
				pActElement = reader->LocalName;
				if (pActElement->CompareTo("Marker") == 0 ) break;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Pattern														                             //
///////////////////////////////////////////////////////////////////////////////
void SvgParamReader::readPattern()
{
	String ^ STR;
	string   stdString;
	double   x, y, width, height, angle;

	STR = reader->GetAttribute ( "name" );
	QuConvert::systemStr2stdStr ( stdString, STR );

	Pattern * pPattern = new Pattern ( stdString );
	pSvgParams->addPattern ( pPattern );

	STR = reader->GetAttribute ( "x" );
	x = XmlConvert::ToDouble ( STR );
	STR = reader->GetAttribute ( "y" );
	y = XmlConvert::ToDouble ( STR );
	STR = reader->GetAttribute ( "width" );
	width = XmlConvert::ToDouble ( STR );
	STR = reader->GetAttribute ( "height" );
	height = XmlConvert::ToDouble ( STR );

	pPattern->setGeometry ( x, y, width, height );
  pPattern->setScale ( globalScale );

	STR = reader->GetAttribute ( "angle" );
	if ( STR != nullptr )
	{
		angle = XmlConvert::ToDouble ( STR );
		pPattern->setAngle ( angle );
	}

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
        readGeometryParams( pSvgPath, "Path" );

				pPattern->addGeometryElement ( pSvgPath );
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
        readGeometryParams( pSvgCircle, "Circle" );

				pPattern->addGeometryElement ( pSvgCircle );
			}
			else
			if ( pActElement->CompareTo("Rect") == 0 )
			{
				string classId;

				STR = reader->GetAttribute ( "class" );
				QuConvert::systemStr2stdStr( classId, STR );
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

				SvgRect * pSvgRect = new SvgRect ( classId );
				pSvgRect->setGeometry ( x, y, width, height );
        readGeometryParams( pSvgRect, "Rect" );

        pPattern->addGeometryElement ( pSvgRect );
			}
			else
			if ( pActElement->CompareTo("Text") == 0 )
			{
				string classId;
				string textString;
				double size;

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
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Hatching												                            		 //
///////////////////////////////////////////////////////////////////////////////
void SvgParamReader::readHatching()
{
	String ^   STR;
	string     stdString;
	SvgColor * pColor;
	double     h;

	STR = reader->GetAttribute ( "name" );
	QuConvert::systemStr2stdStr ( stdString, STR );

	Hatching * pHatching = new Hatching ( stdString );
	pSvgParams->addHatching ( pHatching );

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
					pHatching->setStrokeWidth ( h*globalScale );
				}
        else
          pHatching->setStrokeWidth ( globalScale );
			}
			else
			if ( pActElement->CompareTo ( "line-dist" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					h = XmlConvert::ToDouble ( STR );
					pHatching->setLineDist ( h*globalScale );
				}
        else
          pHatching->setLineDist ( 10*globalScale );
			}
			else
			if ( pActElement->CompareTo("StrokeColor") == 0 )
			{
				pColor = readColor ( "StrokeColor" );
				pHatching->setLineColor ( pColor );
			}
			else
			if ( pActElement->CompareTo("FillColor") == 0 )
			{
				pColor = readColor ( "FillColor" );
				pHatching->setBaseColor ( pColor );
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
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Enumeration								                          					 //
///////////////////////////////////////////////////////////////////////////////
void SvgParamReader::readEnumeration()
{
	string   enumNameA;
	string   enumNameB;
	string   defaultTextSymbol;
	string   defaultTextStyle;
	double   defaultSymbolSize     = 1.0;
	double   defaultTextSymbolSize = 1.0;
	String ^ STR;

	SvgEnumeration * pEnumeration; 
	SvgEnumItem    * pEnumItem;

	STR = reader->GetAttribute ( "nameA" );
	QuConvert::systemStr2stdStr ( enumNameA, STR );
  enumNameA = addDefaultNamespaceAttribut( enumNameA );

	STR = reader->GetAttribute ( "nameB" );
	if ( STR != nullptr )
  {
		QuConvert::systemStr2stdStr ( enumNameB, STR );
    enumNameB = addDefaultNamespaceAttribut( enumNameB );
  }

	STR = reader->GetAttribute ( "defaultTextSymbol" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( defaultTextSymbol, STR );

	STR = reader->GetAttribute ( "defaultTextStyle" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( defaultTextStyle, STR );

	STR = reader->GetAttribute ( "defaultSymbolSize" );
	if ( STR != nullptr )
		defaultSymbolSize = XmlConvert::ToDouble ( STR );

	STR = reader->GetAttribute ( "defaultTextSymbolSize" );
	if ( STR != nullptr )
		defaultTextSymbolSize = XmlConvert::ToDouble ( STR );

	if ( enumNameB != "" )
		pEnumeration = new SvgEnumeration ( enumNameA, enumNameB );
	else
		pEnumeration = new SvgEnumeration ( enumNameA );

	if ( defaultTextSymbol != "" )
		pEnumeration->setDefaultTextSymbol ( defaultTextSymbol );
	if ( defaultTextStyle != "" )
		pEnumeration->setDefaultTextStyle ( defaultTextStyle );

		pEnumeration->setDefaultSymbolSize ( defaultSymbolSize*globalScale );
		pEnumeration->setDefaultTextSymbolSize ( defaultTextSymbolSize*globalScale );

	pSvgParams->addEnumeration ( pEnumeration );

	if ( reader->IsEmptyElement ) return;

	while( reader->Read() )
	{
		pActElement = reader->LocalName;
		if ( reader->NodeType == XmlNodeType::Element )
		{
			if ( pActElement->CompareTo("EnumItem") == 0 )
			{
				pEnumItem = readEnumItem( pEnumeration );
				pEnumeration->addEnumItem ( pEnumItem );
			}
		}
		else
		if ( reader->NodeType == XmlNodeType::EndElement )
		{
			if ( pActElement->CompareTo("Enumeration") == 0 ) break;
		}
	}		
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen EnumItem								                            						 //
///////////////////////////////////////////////////////////////////////////////
SvgEnumItem * SvgParamReader::readEnumItem ( SvgEnumeration * pEnumerationP )
{
	string   schluesselNummerA;
	string   schluesselNummerB;
	string   symbolId;
	string   _text;
	string   textStyle;
	string   textSymbol;
	double   symbolSize;
	double   textSymbolSize;
	bool     symbolSizeSet = false;
	bool	   textSymbolSizeSet = false;
	String ^ STR;

	SvgEnumItem * pEnumItem;

	STR = reader->GetAttribute ( "schluesselNummerA" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( schluesselNummerA, STR );

	STR = reader->GetAttribute ( "schluesselNummerB" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( schluesselNummerB, STR );

	STR = reader->GetAttribute ( "symbolId" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( symbolId, STR );

	STR = reader->GetAttribute ( "text" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( _text, STR );

	STR = reader->GetAttribute ( "textStyle" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( textStyle, STR );

	STR = reader->GetAttribute ( "textSymbol" );
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( textSymbol, STR );

	STR = reader->GetAttribute ( "symbolSize" );
	if ( STR != nullptr )
	{
		symbolSize = XmlConvert::ToDouble ( STR );
		symbolSizeSet = true;
	}

	STR = reader->GetAttribute ( "textSymbolSize" );
	if ( STR != nullptr )
	{
		textSymbolSize = XmlConvert::ToDouble ( STR );
		textSymbolSizeSet = true;
	}

	pEnumItem = new SvgEnumItem ( pEnumerationP, schluesselNummerA, schluesselNummerB );

	if ( symbolId != "" )	pEnumItem->setSymbolId ( symbolId );
	if ( _text != "" )		pEnumItem->setText ( _text );
	if ( textStyle != "" )  pEnumItem->setTextStyle ( textStyle );
	if ( textSymbol != "" ) pEnumItem->setTextSymbol ( textSymbol );
	if ( symbolSizeSet )
		pEnumItem->setSymbolSize ( symbolSize*globalScale );
	if ( textSymbolSizeSet )
		pEnumItem->setTextSymbolSize ( textSymbolSize*globalScale );

	if ( reader->IsEmptyElement ) return pEnumItem;

	while( reader->Read() )
	{
		pActElement = reader->LocalName;

		if ( reader->NodeType == XmlNodeType::EndElement )
		{
			if ( pActElement->CompareTo("EnumItem") == 0 ) break;
		}
	}

	return pEnumItem;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt vor GML Klassen- bzw. Attributname das Default Namespace-Kürzel ein //
///////////////////////////////////////////////////////////////////////////////
string SvgParamReader::addDefaultNamespaceAttribut ( string gmlName )
{
	size_t index;
  string gmlNameAct;

  index = gmlName.find ( ":" );
  if ( index != string::npos )
    return gmlName;

  gmlNameAct = "xplan:" + gmlName;
  return gmlNameAct;
}

///////////////////////////////////////////////////////////////////////////////
//  __gc class SvgWriter													                           //
//  Transformation von 2D GML-Daten in eine SVG-Datei                        //
///////////////////////////////////////////////////////////////////////////////
SvgWriter::SvgWriter (Visualisierung_2D * pVisualisierung_2D, 
					   bool allePlaeneZeigenP, int anzObjWriteMaxP, double deltaP )
{
  m_pVisualisierung_2D = pVisualisierung_2D;
  m_pFeatures          = m_pVisualisierung_2D->getFeatures();
  m_pGmlObjekte        = m_pFeatures->getGmlSchema();
  m_pSvgParams         = m_pVisualisierung_2D->getSvgParams();
	m_pErrorReport       = NULL;

	anzError		     = 0;
	anzObjWriteMax   = anzObjWriteMaxP;
	delta			       = deltaP;
	m_allePlaeneZeigen = allePlaeneZeigenP;

  initializeNormalization();
}

SvgWriter::~SvgWriter ()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Ausgabe										                                							 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::write ( std::string fileName, int darstellungsTyp, double globalScale )
{
	int                  prio;
	size_t					     i, anz, anzObjWrite, anzScans;
	int                  prioLochObjekte = 7;
	Feature			       * pPlanObjekt;
	GEOMETRY_TYPE			   geometrieTyp;
	OBJEKT_TYP           objektTyp;
	string					     klassenName;
	string					     klassenNameNeu;
	string               errorText;
	string					     svgClassId;
	string					     svgSymbolId;
	string					     cssStyleString;
	string					     svgPatternId;
  string               klassenNameOhneKuerzel;
	String				    	^ STR;
  String              ^ EXTENSION;
	GmlKlassenVisual    * pGmlKlassenVisual;
	Style               * pSvgClass;
	Symbol				    	* pSvgSymbol;
	Marker				    	* pMarker;
	Pattern				    	* pSvgPattern;
	Hatching			    	* pSvgHatching;
	SemanticError		  	* pError;

	vector<Pattern*>			 vPatterns;
	vector<Hatching*>      vHatchings;
	vector<string>				 vSymbolIds;

	vector<Feature*>       vObjekte;
  vector<Feature*>       vRasterbilder;
  vector<Style*>         vStyles;
	vector<Symbol*>        vSymbols;
	vector<Marker*>        vMarkers;
	multimap<int,Feature*> mObjekte;
	set<GmlKlassenVisual*> gmlKlassenBenutzt;

  multimap<string,int>            mapKlassenPrioritaeten;
  multimap<string,int>::iterator iterMapKlassenPrioritaeten;

	set<string>::iterator     iterSvgClasses;
	set<string>::iterator     iterSvgSymbols;

	multimap<int,Feature*>::iterator iterObjekte;

	multimap<double,Feature*>				   flaechenMitLoch;
	multimap<double,Feature*>::reverse_iterator iterFlaechenMitLoch;

  vector<GeometryError*> vGeometryErrors;

  set<string> fehlertexte;

	if ( darstellungsTyp == 0 ) return;

	if ( darstellungsTyp == 2 || darstellungsTyp == 3 ) m_interactiveMap = true;
	else											                          m_interactiveMap = false;

	if ( darstellungsTyp == 3 ) m_fehleranzeige = true;
	else						            m_fehleranzeige = false;

  if ( m_allePlaeneZeigen )
    anz = m_pFeatures->getSortedFeatures ( vObjekte, true );
  else
  {
    BLeitPlan * pBLeitplan = m_pVisualisierung_2D->getPlanAktuell();
    PlanBereich * pBereich = m_pVisualisierung_2D->getPlanBereichAktuell();
    anz = m_pFeatures->getFeatures( vObjekte, pBLeitplan, pBereich );
		if ( pBereich != NULL )
			vObjekte.push_back( pBereich );
  }

	for ( i = 0; i <vObjekte.size(); i++ )
	{
		pPlanObjekt    = vObjekte[i];
		klassenName    = pPlanObjekt->getKlassenName();
		geometrieTyp   = pPlanObjekt->getGeometrieTyp();
		objektTyp      = pPlanObjekt->getObjektTyp();

    if ( objektTyp != RASTER_BASIS && 
         objektTyp != PLAN_BEREICH )
    {
    	if ( geometrieTyp == OTHER ||
        geometrieTyp == NO_GEOMETRY ) continue;
    }

		pGmlKlassenVisual = m_pSvgParams->getGmlKlassenVisual ( klassenName );
		if ( pGmlKlassenVisual == NULL )
		{
      if ( fehlertexte.find ( klassenName ) == fehlertexte.end()  )
      {
			  anzError++;
			  errorText = "Fehlende Visualisierungsvorschrift für \n GML-Klasse ";
			  errorText += klassenName;
			  m_pFeatures->getLogfile()->writeLogFile ( errorText );
        fehlertexte.insert( klassenName );
      }
      continue;
		}
    else
      gmlKlassenBenutzt.insert ( pGmlKlassenVisual );

    prio = pGmlKlassenVisual->getPriority( pPlanObjekt );
    if ( prio < 0 )
      anzError++;
    else
    {
      bool flaechenschluss;
      bool success = pPlanObjekt->getBoolAttributWert( "xplan:flaechenschluss", flaechenschluss );
      if ( success && !flaechenschluss )
        prio = prio + 1;

		  mObjekte.insert ( multimap<int,Feature*>::value_type ( prio, pPlanObjekt ) );
      bool found = false;
      for ( iterMapKlassenPrioritaeten = mapKlassenPrioritaeten.lower_bound( klassenName );
        iterMapKlassenPrioritaeten != mapKlassenPrioritaeten.upper_bound( klassenName );
        iterMapKlassenPrioritaeten++ )
      {
        if ( iterMapKlassenPrioritaeten->second == prio )
        {
          found = true;
          break;
        }
      }
      if ( found == false )
        mapKlassenPrioritaeten.insert( multimap<string,int>::value_type ( klassenName, prio ) );
    }
	}

	//  Schreiben svg Root-Element
	writeSvgRootPlanbereich( fileName, globalScale );

	//  Schreiben der Defs-Section
	writer->WriteStartElement ( "defs" );

	//  Schreiben der Klassen-Namen für die Interaktive Karte
	if ( m_interactiveMap )
		writeKlassenNamen ( gmlKlassenBenutzt );

	if ( m_pErrorReport == NULL ) m_fehleranzeige = false;

	if ( m_fehleranzeige &&  m_pErrorReport->getSemanticErrorAnz() + m_pErrorReport->getGeometryErrorAnz() > 0 )
	{
		writer->WriteStartElement ( "text" );
		writer->WriteAttributeString ( "id", "FehlerAnz" );
		writer->WriteString ( XmlConvert::ToString ( m_pErrorReport->getSemanticErrorAnz() + m_pErrorReport->getGeometryErrorAnz() ) );
		writer->WriteEndElement();
	}

	//  Schreiben des Extra-Fonts
	writeExtraFont();

	// Schreiben des css-Stylesheets
	writer->WriteStartElement ( "style" );
	writer->WriteAttributeString ( "type", "text/css" );

	anz = m_pSvgParams->getStyles ( vStyles );
	for ( i = 0; i < anz; i++ )
	{
		pSvgClass = vStyles[i];

		cssStyleString = pSvgClass->getStyleString();
		STR = QuConvert::ToString ( cssStyleString );
		STR = String::Concat ( STR, "\n" );
		writer->WriteRaw ( STR );
	}

	writer->WriteEndElement();

	//  Rausschreiben Pattern-Definitionen
	anz = m_pSvgParams->getPatterns ( vPatterns );
	for ( i = 0; i < anz; i++ )
	{
		pSvgPattern = vPatterns[i];
		writeSvgPattern ( pSvgPattern );
	}

	anz = m_pSvgParams->getHatchings ( vHatchings );
	for ( i = 0; i < anz; i++ )
	{
		pSvgHatching = vHatchings[i];
		writeSvgHatching ( pSvgHatching );
	}

	//  Rausschreiben symbol-Definitionen
	anz = m_pSvgParams->getSymbols ( vSymbols );
	for ( i = 0; i < anz; i++ )
	{
		pSvgSymbol = vSymbols[i];
		writeSvgSymbol ( pSvgSymbol );
	}

	//  Rausschreiben marker-Definitionen
	anz = m_pSvgParams->getMarkers ( vMarkers );
	for ( i = 0; i < anz; i++ )
	{
		pMarker = vMarkers[i];
		writeSvgMarker ( pMarker );
	}

	//  Ende Defs-Section
	writer->WriteEndElement();


	klassenName = "";
	anzObjWrite = 0;
	STR = nullptr;

	//  Rausschreiben der svg-Objekte 
	for ( iterObjekte = mObjekte.begin();
		  iterObjekte != mObjekte.end();
		  iterObjekte++ )
	{
    anzScans = 0;

		pPlanObjekt = iterObjekte->second;
    objektTyp      = pPlanObjekt->getObjektTyp();

		klassenNameNeu = pPlanObjekt->getKlassenName ();
		if ( klassenNameNeu != klassenName )
		{		  
      if ( mapKlassenPrioritaeten.count( klassenNameNeu ) == 1 )
        EXTENSION = "_0";
      else
      {
        int index = 0;
        for ( iterMapKlassenPrioritaeten = mapKlassenPrioritaeten.lower_bound( klassenNameNeu );
              iterMapKlassenPrioritaeten != mapKlassenPrioritaeten.upper_bound( klassenNameNeu );
              iterMapKlassenPrioritaeten++ )
        {
          if ( iterObjekte->first == iterMapKlassenPrioritaeten->second )
            break;
          index++;
        }
        EXTENSION = String::Concat( "_", XmlConvert::ToString( index ));
      }

      if ( objektTyp == RASTER_BASIS )
      {
        klassenNameOhneKuerzel = removeDefaultNamespaceAttribut( klassenNameNeu );
        STR = String::Concat ( QuConvert::ToString ( klassenNameOhneKuerzel  ), "_RA" );
      }
      else
      if ( objektTyp == PLAN_BEREICH )
      {
        if ( m_pGmlObjekte->getGmlTyp() == XPLANGML_5_1 || m_pGmlObjekte->getGmlTyp() == XPLANGML_5_2 )
        {
          anzScans = pPlanObjekt->getRelatedFeatures( "xplan:refScan", vRasterbilder );
          if ( anzScans > 0 )
          {
            klassenNameOhneKuerzel = removeDefaultNamespaceAttribut( klassenNameNeu );
            STR = String::Concat ( QuConvert::ToString ( klassenNameOhneKuerzel  ), "_RA" );
          }
        }
      }
      else
      {
        geometrieTyp   = pPlanObjekt->getGeometrieTyp();
        klassenNameOhneKuerzel = removeDefaultNamespaceAttribut( klassenNameNeu );
        switch ( geometrieTyp )
        {
        case MULTI_SURFACE:
        case COMPOSITE_SURFACE:
        case SHELL:
        case SURFACE:
        case POLYGON:
          EXTENSION = String::Concat( "_FL", EXTENSION );
          STR = String::Concat ( QuConvert::ToString ( klassenNameOhneKuerzel  ), EXTENSION );
          break;
        case MULTI_CURVE:
        case CURVE:
        case LINE_STRING:
				case LINE_STRING_SEGMENT:
        case ARC:
        case CIRCLE:
          EXTENSION = String::Concat( "_LI", EXTENSION );
          STR = String::Concat ( QuConvert::ToString ( klassenNameOhneKuerzel  ), EXTENSION );
          break;
        case MULTI_POINT:
        case GEO_POINT:
          EXTENSION = String::Concat( "_PO", EXTENSION );
          STR = String::Concat ( QuConvert::ToString ( klassenNameOhneKuerzel  ), EXTENSION );
          break;
        }
      }

			if ( STR == nullptr )
				continue;

			if ( klassenName != "" )
				writer->WriteEndElement();
			writer->WriteStartElement ( "g" );

			writer->WriteAttributeString ( "id", STR );
			writer->WriteAttributeString ( "display", "inline" );
			klassenName = klassenNameNeu;

      if ( objektTyp == RASTER_BASIS )
        writeRasterdarstellung ( pPlanObjekt );
      else
      if ( objektTyp == PLAN_BEREICH && anzScans > 0 )
        writeRasterdarstellung( pPlanObjekt );
      else
			  writePlanObjekt ( pPlanObjekt, false, 0 );
		}
		else
    {
      if ( objektTyp == RASTER_BASIS )
        writeRasterdarstellung ( pPlanObjekt );
      else
      if ( objektTyp == PLAN_BEREICH && anzScans > 0 )
          writeRasterdarstellung( pPlanObjekt );
      else
        writePlanObjekt ( pPlanObjekt, false, 0 );
    }

		anzObjWrite++;
		if ( anzObjWrite >= anzObjWriteMax )
			break;
	}

	if ( m_fehleranzeige )
	{
		for ( i = 0; i < (int)m_pErrorReport->getSemanticErrorAnz(); i++ )
		{
			Feature * pObj;
			Feature * pObjRelation;

			pError = m_pErrorReport->getSemanticError ( i );
			pObj     = pError->getFeature();  
			pObjRelation = pError->getRelatedFeature();

			if ( pObj != NULL )
			{
				writer->WriteStartElement ( "g" );
				STR = String::Concat ( QuConvert::ToString ( "Fehler_" ), XmlConvert::ToString ( pError->getErrorNumber() ) );
				writer->WriteAttributeString ( "id", STR );
				writer->WriteAttributeString ( "display", "none" );

				if ( pObjRelation == NULL )
					writePlanObjekt ( pObj, true, 0 );
				else
					writePlanObjekt ( pObjRelation, true, 1 );

				writer->WriteEndElement();
			}
		}

    m_pErrorReport->getGeometryErrors( vGeometryErrors );
    for ( i = 0; i < (int)vGeometryErrors.size(); i++ )
    {
      Feature * pObj;
      Feature * pObjRelation;

      GeometryError * pGeometryError = vGeometryErrors[i];

//      objectId = pGeometryError->getObjectId();
      pObj = pGeometryError->getFeature();

      pObjRelation = pGeometryError->getRelatedFeature();

      if ( pObj != NULL )
      {
        writer->WriteStartElement ( "g" );
        STR = String::Concat ( QuConvert::ToString ( "Fehler_" ), XmlConvert::ToString ( pGeometryError->getErrorNumber() ) );
        writer->WriteAttributeString ( "id", STR );
        writer->WriteAttributeString ( "display", "none" );

        if ( pObjRelation == NULL )
          writePlanObjekt ( pObj, true, 0 );
        else
          writePlanObjekt ( pObjRelation, true, 1 );

        writer->WriteEndElement();
      }
    }
	}

	if ( anzObjWrite >= anzObjWriteMax )
	{
		STR = String::Concat ( "In dieser Version können nur", Convert::ToString ( anzObjWriteMax ), " GML-Objekte geschrieben werden" );
		MessageBox::Show ( STR );
	}

	writer->WriteEndElement();
	writer->Flush();
	writer->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben SVG-Header                                                 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeSvgHeader ( std::string fileName,
								double _swX, double _swY, double _noX, double _noY )
{
	size_t    			  anz, i;
	string					  klassenName;
	string            errorText;
	string					  cssStyleString;
	string					  svgPatternId;
	String					^ STR;
	Style           * pSvgClass;
	Symbol					* pSvgSymbol;
	Marker					* pMarker;
	Pattern					* pSvgPattern;
	Hatching				* pSvgHatching;
	vector<Pattern*>	 vPatterns;
	vector<Hatching*>  vHatchings;
	vector<Style*>     vStyles;
	vector<Symbol*>    vSymbols;
	vector<Marker*>    vMarkers;

	//  Schreiben svg Root-Element
	writeSvgRoot( fileName, _swX, _swY, _noX, _noY );

	//  Schreiben der Defs-Section
	writer->WriteStartElement ( "defs" );

	//  Schreiben des Extra-Fonts
	writeExtraFont();

	// Schreiben des css-Stylesheets
	writer->WriteStartElement ( "style" );
	writer->WriteAttributeString ( "type", "text/css" );

	anz = m_pSvgParams->getStyles ( vStyles );
	for ( i = 0; i < anz; i++ )
	{
		pSvgClass = vStyles[i];

		cssStyleString = pSvgClass->getStyleString();
		STR = QuConvert::ToString ( cssStyleString );
		STR = String::Concat ( STR, "\n" );
		writer->WriteRaw ( STR );
	}

	writer->WriteEndElement();

	//  Rausschreiben Pattern-Definitionen
	anz = m_pSvgParams->getPatterns ( vPatterns );
	for ( i = 0; i < anz; i++ )
	{
		pSvgPattern = vPatterns[i];
		writeSvgPattern ( pSvgPattern );
	}

	anz = m_pSvgParams->getHatchings ( vHatchings );
	for ( i = 0; i < anz; i++ )
	{
		pSvgHatching = vHatchings[i];
		writeSvgHatching ( pSvgHatching );
	}

	//  Rausschreiben symbol-Definitionen
	anz = m_pSvgParams->getSymbols ( vSymbols );
	for ( i = 0; i < anz; i++ )
	{
		pSvgSymbol = vSymbols[i];
		writeSvgSymbol ( pSvgSymbol );
	}

	//  Rausschreiben marker-Definitionen
	anz = m_pSvgParams->getMarkers ( vMarkers );
	for ( i = 0; i < anz; i++ )
	{
		pMarker = vMarkers[i];
		writeSvgMarker ( pMarker );
	}

	//  Ende Defs-Section
	writer->WriteEndElement();
}

void SvgWriter::close ()
{
	writer->Flush();
	writer->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Überschrift für XPlanGML und INSPIRE PLU Modelle    				 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeUeberschrift ( std::string fileName )
{
	String	                   ^ STR;
	String	                   ^ STR_X;
	String	                   ^ STR_Y;
	String	                   ^ STR_X0;
	String	                   ^ STR_Y0;
	string                       stdString;
	int                          deltaX, deltaY;
  bool                         success;
	BLeitPlan                  * pPlan;
  string                       ueberschrift;
  Feature                    * pSpatialPlan;
  vector<Feature*>             vFeatures;
	size_t                       anz;

  System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();

  ueberschrift = "";
  if (m_pFeatures->isXPlanGML() )
  {
    if ( !m_allePlaeneZeigen )
    {
      pPlan = m_pVisualisierung_2D->getPlanAktuell();
      pPlan->getStringAttributWert ( "xplan:name", ueberschrift );
    }
  }
  else
  if (  m_pFeatures->getGmlTyp() == INSPIRE_LU_PLANNED )
  {
    anz = m_pFeatures->getFeatures( vFeatures, Constant::getKlassenNameINSPIRE( SPATIAL_PLAN ) );
    if ( anz >= 1 )
      pSpatialPlan = vFeatures[0];
    else
      return;

    success = pSpatialPlan->getStringAttributWert ( "plu:officialTitle", ueberschrift );
  }

	STR    = QuConvert::ToString ( fileName );
	writer = gcnew XmlTextWriter( STR, utf8);

	writer->Formatting = Formatting::Indented;
	writer->WriteStartDocument( true );

//	writer->WriteDocType ( "svg", "-//W3C//DTD SVG 1.1//EN",  "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11-flat-20030114.dtd", nullptr );
	writer->WriteComment("Generated by: KIT GML-Toolbox");

  writer->WriteStartElement("svg ");

  writer->WriteAttributeString("xmlns", "http://www.w3.org/2000/svg");
	writer->WriteAttributeString("xmlns:xlink", "http://www.w3.org/1999/xlink");
	writer->WriteAttributeString("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	writer->WriteAttributeString ( "zoomAndPan", "disable" );

	deltaX = m_pSvgParams->getBreiteUeberschrift();
	deltaY = m_pSvgParams->getHoeheUeberschrift();

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
		writer->WriteAttributeString ( "text-anchor", "middle" );
		writer->WriteAttributeString ( "x", XmlConvert::ToString ( deltaX/2 ) );
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
//  Schreibt die Überschrift für andere Modelle                              //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeUeberschriftStandard ( std::string fileName )
{
  String                     ^ STR;
  System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();

  STR    = QuConvert::ToString ( fileName );
  writer = gcnew XmlTextWriter( STR, utf8 );

  writer->Formatting = Formatting::Indented;
  writer->WriteStartDocument( true );

  writer->WriteDocType ( "svg", "-//W3C//DTD SVG 1.1//EN",  "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11-flat-20030114.dtd", nullptr );
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
//  Schreibt eine Übersichtskarte	für XPlanGML und INSPIRE PLU Modelle  		 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeUebersicht ( std::string fileName, double globalScale )
{
	size_t                      i, j, anz, anzPlaene;
	String                    ^ styleString;
	String                    ^ PATH_STRING;
	string                      stdString;
	vector<PolygonGeo*>            polygone;
	vector<PolygonGeo*>            polygoneLoc;
	vector<BLeitPlan*>          plaene;
	BLeitPlan                 * pPlan;
	PlanBereich               * pPlanBereich;
  Feature                   * pSpatialPlan;
  vector<Feature*>            vFeatures;

  anz = 0;

	if ( !m_allePlaeneZeigen && m_pFeatures->isXPlanGML() == true )
	{
		pPlan        = m_pVisualisierung_2D->getPlanAktuell();
		pPlanBereich = m_pVisualisierung_2D->getPlanBereichAktuell();
	}
	else
	{
		pPlan        = NULL;
		pPlanBereich = NULL;
	}
	writeSvgRootUebersicht( fileName, globalScale );

	writer->WriteStartElement ( "defs" );

	writer->WriteStartElement ( "style" );
	writer->WriteAttributeString ( "type", "text/css" );

	styleString = ".default {fill:gray; stroke: black; stroke-width:1;}";
	writer->WriteString ( styleString );

	writer->WriteEndElement();
	writer->WriteEndElement();


  if ( m_pFeatures->isXPlanGML() )
  {
	  if ( m_allePlaeneZeigen )
	  {
		  anzPlaene = m_pFeatures->getPlaene ( plaene );
		  for ( i = 0; i < anzPlaene; i++ )
		  {
			  polygoneLoc.clear();
			  BLeitPlan * pPlan = plaene[i];
			  anz = pPlan->getFlaechenGeometrie("", polygoneLoc );
			  for ( j = 0; j < anz; j++ )
			  {
  				polygone.push_back ( polygoneLoc[j] );
	  		}
		  }
		  anz = polygone.size();
	  }
	  else
	  {
      if ( pPlanBereich != NULL )
      {
        anz = pPlanBereich->getFlaechenGeometrie( "", polygone );
        if ( anz == 0 )
			    anz =  pPlan->getFlaechenGeometrie( "", polygone );
      }
      else
        anz =  pPlan->getFlaechenGeometrie( "", polygone );
	  }
  }
  else
  if ( m_pFeatures->isINSPIREPLU() )
  {
    anz = m_pFeatures->getFeatures( vFeatures, Constant::getKlassenNameINSPIRE( SPATIAL_PLAN ) );
    if ( anz >= 1 )
      pSpatialPlan = vFeatures[0];
    else
      return;

    anz = pSpatialPlan->getFlaechenGeometrie( "", polygone );
  }

	for ( i = 0; i < anz; i++ )
	{
		PolygonGeo * pPoly =polygone[i];
		_Ring   * pRing = pPoly->getAussenkontur();
		stdString = pRing->getPointsSVG ( swX, noY );
		PATH_STRING = QuConvert::ToString ( stdString );

		writer->WriteStartElement ( "path" );
		writer->WriteAttributeString ( "class", "default" );
		writer->WriteAttributeString ( "d", PATH_STRING );
		writer->WriteEndElement();
	}

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
//  Schreibt eine Übersichtskarte	für andere Modelle                    		 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeUebersichtStandard ( std::string fileName, double globalScale )
{
  writeSvgRoot( fileName, 0.0, 0.0, 0.0, 0.0 );
  writer->WriteEndElement();

  writer->Flush();
  writer->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Namen aller benutzten Klassen in die svg-Datei	      			 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeKlassenNamen ( std::set<GmlKlassenVisual*> & klassenNamenP )
{
	GmlKlassenVisual			         * pKlassenVisual;
	string						            	 gmlKlassenName;
	set<GmlKlassenVisual*>::iterator iterPointer;
	set<string>::iterator			       iter;
	String						             ^ STR = "";
	set<string>					             gmlKlassenNamen;
	size_t  					               i, anz;

	for ( iterPointer = klassenNamenP.begin(); iterPointer != klassenNamenP.end(); iterPointer++ )
	{
		pKlassenVisual = *iterPointer;
		gmlKlassenName = pKlassenVisual->getGmlKlassenName();
    if ( gmlKlassenName != "xplan:BP_Bereich" && gmlKlassenName != "xplan:FP_Bereich" && 
         gmlKlassenName != "xplan:RP_Bereich" && gmlKlassenName != "xplan:LP_Bereich" && gmlKlassenName != "xplan:SO_Bereich" )
			gmlKlassenNamen.insert ( gmlKlassenName );
	}

	writer->WriteStartElement ( "text" );
	writer->WriteAttributeString ( "id", "klassen" );

	anz = gmlKlassenNamen.size();
	i = 0;
	for ( iter = gmlKlassenNamen.begin(); iter != gmlKlassenNamen.end(); iter++ )
	{
		gmlKlassenName = *iter;
    gmlKlassenName = removeDefaultNamespaceAttribut( gmlKlassenName );
		i++;

		if ( i < anz )
			STR = String::Concat ( STR, QuConvert::ToString ( gmlKlassenName) , " " );
		else
			STR = String::Concat ( STR, QuConvert::ToString ( gmlKlassenName) );
	}
	writer->WriteString ( STR );
	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt das svg Root-Element	der Übersichts-Karte           						 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeSvgRootUebersicht( std::string fileName, double globalScale )
{
  PlanBereich          * pPlanBereichSvg;
  double		             sw[3], no[3];
  bool                   success = false;
  string                 srsName;
  int                    srsDimension;

  if ( m_pFeatures->isXPlanGML() )
  {
    if ( m_allePlaeneZeigen )
      success = m_pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
    else
    {
      pPlanBereichSvg = m_pVisualisierung_2D->getPlanBereichAktuell();
      if ( pPlanBereichSvg != NULL )
      {
        success = pPlanBereichSvg->getBoundingBox ( sw, no, srsName, srsDimension );
        if ( !success )
          success = m_pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
      }
      else
        success = m_pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
    }
  }
  else
  {
    success = m_pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
  }

  if ( success )
    writeSvgRoot ( fileName, sw[0], sw[1], no[0], no[1] );
}		

///////////////////////////////////////////////////////////////////////////////
//  Schreibt das svg Root-Element		                      									 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeSvgRootPlanbereich( std::string fileName, double globalScale )
{
	PlanBereich  * pPlanBereichSvg;
	double		     sw[3], no[3], groesseDisplay[2], b[2], d[2];
  string         srsName;
  int            srsDimension;
  bool           success;

  if ( m_pFeatures->isXPlanGML() )
  {
  	if ( m_allePlaeneZeigen )
	  	m_pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
	  else
	  {
      pPlanBereichSvg = m_pVisualisierung_2D->getPlanBereichAktuell();
      if ( pPlanBereichSvg != NULL  )
      {
		    success = pPlanBereichSvg->getBoundingBox ( sw, no, srsName, srsDimension );
        if ( !success )
          success =  m_pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
      }
      else
        success =  m_pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
	  }
  }
  else
  if ( m_pFeatures->isINSPIREPLU() || m_pFeatures->isALKIS() )
  {
    success = m_pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
  }

  d[0] = no[0] - sw[0];
  d[1] = no[1] - sw[1];

  groesseDisplay[0] = m_pSvgParams->getBreiteDisplay();
  groesseDisplay[1] = m_pSvgParams->getHoeheDisplay();

  b[0] = groesseDisplay[0]* 2000*globalScale;
  b[1] = groesseDisplay[1]* 2000*globalScale;

  sw[0] = sw[0] + 0.5*( d[0] - b[0] );
  sw[1] = sw[1] + 0.5*( d[1] - b[1] );
  no[0] = sw[0] + b[0];
  no[1] = sw[1] + b[1];

	writeSvgRoot ( fileName, sw[0], sw[1], no[0], no[1] );
}

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Normalisierung auf die minimalen x/y Koordinaten			 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::initializeNormalization()
{
  PlanBereich * pPlanBereichSvg;
  double		    sw[3], no[3];
  bool          success = false;
  string        srsName;
  int           srsDimension;

  if ( m_pFeatures->isXPlanGML() )
  {
    if ( m_allePlaeneZeigen )
      success = m_pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
    else
    {
      pPlanBereichSvg = m_pVisualisierung_2D->getPlanBereichAktuell();
      if ( pPlanBereichSvg != NULL )
      {
        success = pPlanBereichSvg->getBoundingBox ( sw, no, srsName, srsDimension );
        if ( !success )
          success = m_pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
      }
      else
        success = m_pFeatures->getBoundingBox( sw, no, srsName, srsDimension );
    }
  }
  else
  if ( m_pFeatures->isINSPIREPLU() ||  m_pFeatures->isALKIS() )
    success = m_pFeatures->getBoundingBox ( sw, no, srsName, srsDimension );

  if ( success )
  {
    swX = sw[0];
    swY = sw[1];
    noX = no[0];
    noY = no[1];
  }
  else
  {
    sw[0] = 0.0;
    sw[1] = 0.0;
    no[0] = 0.0;
    no[1] = 0.0;
  }
}		

///////////////////////////////////////////////////////////////////////////////
//  Schreibt das svg Root-Element		                      									 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeSvgRoot( std::string fileName, double _swX, double _swY, double _noX, double _noY )
{
	String		^ STR;
	String		^ STR_X;
	String		^ STR_Y;
	String		^ STR_X0;
	String		^ STR_Y0;
	double		  deltaX, deltaY;

	deltaX =  delta*(_noX - _swX);
	deltaY =  delta*(_noY - _swY);

  System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();

	STR    = QuConvert::ToString ( fileName );
	writer = gcnew XmlTextWriter( STR, utf8);

	writer->Formatting = Formatting::Indented;
	writer->WriteStartDocument( true );

//	writer->WriteDocType ( "svg", "-//W3C//DTD SVG 1.1//EN",  "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11-flat-20030114.dtd", nullptr );
	writer->WriteComment("Generated by: KIT GML-Toolbox");

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

	if ( m_interactiveMap )
	{
		writer->WriteAttributeString ( "id", "karte" );
		writer->WriteAttributeString ( "zoomAndPan", "disable" );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Schließt die Erzeugung der SVG-Datei ab                                  //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeSvgEnd()
{
	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt den Extra-Font		                        											 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeExtraFont()
{
	writer->WriteStartElement ( "font" );
	writer->WriteAttributeString ( "id", "IDfontWWF" );
	writer->WriteAttributeString ( "horiz-adv-x", "100" );

	writer->WriteStartElement ( "font-face" );
	writer->WriteAttributeString ( "font-family", "MyFonts" );
	writer->WriteAttributeString ( "font-weight", "bold" );
	writer->WriteAttributeString ( "units-per-em", "256" );
	writer->WriteEndElement();

	writer->WriteStartElement ( "missing-glyph" );
	writer->WriteEndElement();

	writer->WriteStartElement ( "glyph" );  //  WasserwirtschaftsFlaeche
	writer->WriteAttributeString ( "unicode", "A" );
	writer->WriteAttributeString ( "horiz-adv-x", "100" );
	writer->WriteAttributeString ( "orientation", "h" );
	writer->WriteAttributeString ( "d", "M 0,0 L 100, 0, L 100, 20 A 33, 33, 0, 0, 0, 75, 25 A 33, 33, 0, 0, 1, 50, 30  A 33, 33, 0, 0, 1, 25, 25  A 33, 33, 0, 0, 0, 0, 20 L 0,0" );
	writer->WriteEndElement();

	writer->WriteStartElement ( "glyph" );   //  ImmissionsschutzFlaeche
	writer->WriteAttributeString ( "unicode", "B" );
	writer->WriteAttributeString ( "horiz-adv-x", "150" );
	writer->WriteAttributeString ( "orientation", "h" );
	writer->WriteAttributeString ( "d", "M 0,0 L 100, 0 L 50,50 L 0,0" );
	writer->WriteEndElement();

	writer->WriteStartElement ( "glyph" );   //  LuftreinhalteFlaeche
	writer->WriteAttributeString ( "unicode", "C" );
	writer->WriteAttributeString ( "horiz-adv-x", "150" );
	writer->WriteAttributeString ( "orientation", "h" );
	writer->WriteStartElement ( "path" );
	writer->WriteAttributeString ( "style", "fill:none; stroke:black; stroke-width:5" );
	writer->WriteAttributeString ( "d", "M 0,0 L 100, 0 L 50,50 L 0,0" );
	writer->WriteEndElement();
	writer->WriteEndElement();

	writer->WriteStartElement ( "glyph" );   //  FreiFlaeche
	writer->WriteAttributeString ( "unicode", "D" );
	writer->WriteAttributeString ( "horiz-adv-x", "80" );
	writer->WriteAttributeString ( "orientation", "h" );
	writer->WriteStartElement ( "path" );
	writer->WriteAttributeString ( "style", "fill:none; stroke:black; stroke-width:3" );
	writer->WriteAttributeString ( "d", "M 0,0 L 80, 0 L 40,40 L 0,0" );
	writer->WriteEndElement();
	writer->WriteEndElement();

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben eines svg-Patterns	                    									 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeSvgPattern (Pattern * pPattern )
{
	string		    errorText;
	string		    str;
	String		  ^ STR;
	double		    x, y, width, height, angle, scale;
	unsigned int  i;
	SvgGeometry * pSvgGeometry;

	if ( pPattern == NULL )
	{
		anzError++;
		errorText = "Fehlende Pattern-Definition: ";
		str = pPattern->getPatternId();
		errorText += str;
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}

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

	for ( i = 0; i < pPattern->getGeometryElementAnz(); i++ )
	{
		pSvgGeometry = pPattern->getGeometryElement ( i );
		writeGeometryElement ( pSvgGeometry );
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben eines speziellen svg-Patterns, das ein Hatching darstellt  //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeSvgHatching ( Hatching * pHatching )
{
	String ^ ID           = nullptr;
	String ^ Width			  = nullptr;
	String ^ Height			  = nullptr;
	String ^ Stroke			  = nullptr;
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
	SvgColor * pLineColor   = pHatching->getLineColor();
	SvgColor * pBaseColor   = pHatching->getBaseColor();

	ID          = QuConvert::ToString ( id );
	StrokeWidth = XmlConvert::ToString ( strokeWidth );

	if ( hatchingType == "horizontal" )
	{
		Width = XmlConvert::ToString ( largeNumber );
		Height = XmlConvert::ToString ( lineDist );

		switch ( pLineColor->getColorType() )
		{
		case SvgColor::NONE:
			break;

		case SvgColor::NAME:
			stdString = pLineColor->getColorName();
			Stroke = QuConvert::ToString ( stdString );
			break;

		case SvgColor::RGB:
			stdString = pLineColor->getColorStringRGB();
			Stroke = QuConvert::ToString ( stdString );
			break;
		}

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

		switch ( pLineColor->getColorType() )
		{
		case SvgColor::NONE:
			break;

		case SvgColor::NAME:
			stdString = pLineColor->getColorName();
			Stroke = QuConvert::ToString ( stdString );
			break;

		case SvgColor::RGB:
			stdString = pLineColor->getColorStringRGB();
			Stroke = QuConvert::ToString ( stdString );
			break;
		}

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

		switch ( pLineColor->getColorType() )
		{
		case SvgColor::NONE:
			break;

		case SvgColor::NAME:
			stdString = pLineColor->getColorName();
			Stroke = QuConvert::ToString ( stdString );
			break;

		case SvgColor::RGB:
			stdString = pLineColor->getColorStringRGB();
			Stroke = QuConvert::ToString ( stdString );
			break;
		}

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

		switch ( pLineColor->getColorType() )
		{
		case SvgColor::NONE:
			break;

		case SvgColor::NAME:
			stdString = pLineColor->getColorName();
			Stroke = QuConvert::ToString ( stdString );
			break;

		case SvgColor::RGB:
			stdString = pLineColor->getColorStringRGB();
			Stroke = QuConvert::ToString ( stdString );
			break;
		}

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

		switch ( pLineColor->getColorType() )
		{
		case SvgColor::NONE:
			break;

		case SvgColor::NAME:
			stdString = pLineColor->getColorName();
			Stroke = QuConvert::ToString ( stdString );
			break;

		case SvgColor::RGB:
			stdString = pLineColor->getColorStringRGB();
			Stroke = QuConvert::ToString ( stdString );
			break;
		}

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

		switch ( pLineColor->getColorType() )
		{
		case SvgColor::NONE:
			break;

		case SvgColor::NAME:
			stdString = pLineColor->getColorName();
			Stroke = QuConvert::ToString ( stdString );
			break;

		case SvgColor::RGB:
			stdString = pLineColor->getColorStringRGB();
			Stroke = QuConvert::ToString ( stdString );
			break;
		}

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

	if ( pBaseColor->getColorType() != SvgColor::NONE )
	{
		writer->WriteStartElement ( "rect" );
		writer->WriteAttributeString ( "x", "0" );
		writer->WriteAttributeString ( "y", "0" );
		writer->WriteAttributeString ( "width", Width );
		writer->WriteAttributeString ( "height", Height );

		switch ( pBaseColor->getColorType() )
		{
		case SvgColor::NAME:
			stdString = pBaseColor->getColorName();
			break;

		case SvgColor::RGB:
			stdString = pBaseColor->getColorStringRGB();
			break;
		}
		Fill = QuConvert::ToString ( stdString );
		writer->WriteAttributeString ( "fill", Fill );
		writer->WriteEndElement();
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
//  Rausschreiben eines svg-Symbols			                    								 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeSvgSymbol ( Symbol * pSymbol )
{
	string		    errorText;
	string		    str;
	String		  ^ STR;
	unsigned int  i;
	SvgGeometry * pSvgGeometry;

	writer->WriteStartElement ( "symbol" );
	
	str = pSymbol->getSymbolId ();
	STR = QuConvert::ToString ( str );
	writer->WriteAttributeString ( "id", STR );
	writer->WriteAttributeString ( "overflow", "visible" );

	for ( i = 0; i < pSymbol->getGeometryElementAnz(); i++ )
	{
		pSvgGeometry = pSymbol->getGeometryElement ( i );
		writeGeometryElement ( pSvgGeometry );
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben eines svg-Markers		                    									 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeSvgMarker ( Marker * pMarker )
{
	string		    errorText;
	string		    str;
	String		  ^ STR;
	unsigned int  i;
	double        h;
	SvgGeometry * pSvgGeometry;

	writer->WriteStartElement ( "marker" );
	
	str = pMarker->getMarkerId ();
	STR = QuConvert::ToString ( str );
	writer->WriteAttributeString ( "id", STR );

	writer->WriteAttributeString ( "orient", "auto" );

	str = pMarker->getViewbox();
	STR = QuConvert::ToString ( str );
	writer->WriteAttributeString ( "viewBox", STR );

	h = pMarker->getRefX();
	STR = XmlConvert::ToString ( h );
	writer->WriteAttributeString ( "refX", STR );

	h = pMarker->getRefY();
	STR = XmlConvert::ToString ( h );
	writer->WriteAttributeString ( "refY", STR );

	h = pMarker->getWidth();
	STR = XmlConvert::ToString ( h );
	writer->WriteAttributeString ( "markerWidth", STR );

	h = pMarker->getHeight();
	STR = XmlConvert::ToString ( h );
	writer->WriteAttributeString ( "markerHeight", STR );

	for ( i = 0; i < pMarker->getGeometryElementAnz(); i++ )
	{
		pSvgGeometry = pMarker->getGeometryElement ( i );
		writeGeometryElement ( pSvgGeometry );
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines svg-Geometrieelements					                  				 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeGeometryElement ( SvgGeometry * pSvgGeometryP )
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
//  Schreiben eines GML-Features       					                  					 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writePlanObjekt ( Feature * pPlanObjektP, bool isError, int objNr )
{
	string			      klassenName;
	GmlKlassenVisual * pKlassenVisual;
	string             selectAttribut1;
	string			       selectAttribut2;
	string			       selectAttributWert1;
	string			       selectAttributWert2;
	string			       errorText;
	vector<GeoPoint*>  symbolPositionen;

	if ( !isError )
	{
		klassenName = pPlanObjektP->getKlassenName();
		pKlassenVisual = m_pSvgParams->getGmlKlassenVisual ( klassenName );
		if ( pKlassenVisual == NULL && ( 
			pPlanObjektP->getObjektTyp() == XP_GEOMETRIE_OBJEKT || pPlanObjektP->getObjektTyp() == XP_PUNKT_OBJEKT ||
			pPlanObjektP->getObjektTyp() == XP_LINIEN_OBJEKT || pPlanObjektP->getObjektTyp() == XP_FLAECHEN_OBJEKT  ) )
		{	
			anzError++;
			errorText = "Fehlende Visualisierungsvorschrift für \n GML-Klasse ";
			errorText += klassenName;
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
			return;
		}
	}
	else
	{
		switch ( pPlanObjektP->getGeometrieTyp() )
		{
		case MULTI_SURFACE:
    case COMPOSITE_SURFACE:
    case SHELL:
    case SURFACE:
    case POLYGON:
			if ( objNr == 0 )
        pKlassenVisual = m_pSvgParams->getGmlKlassenVisual ( "xplan:FehlerFlaechenobjektA" );
			else
			if ( objNr == 1 )
				pKlassenVisual = m_pSvgParams->getGmlKlassenVisual ( "xplan:FehlerFlaechenobjektB" );
			break;
		case MULTI_CURVE:
    case CURVE:
    case LINE_STRING:
		case LINE_STRING_SEGMENT:
    case ARC:
    case CIRCLE:
			if ( objNr == 0 )
				pKlassenVisual = m_pSvgParams->getGmlKlassenVisual ( "xplan:FehlerLinienobjektA" );
			else
			if ( objNr == 1 )
				pKlassenVisual = m_pSvgParams->getGmlKlassenVisual ( "xplan:FehlerLinienobjektB" );
			break;
		case MULTI_POINT:
    case GEO_POINT:
			if ( objNr == 0 )
				pKlassenVisual = m_pSvgParams->getGmlKlassenVisual ( "xplan:FehlerPunktobjektA" );
			else
			if ( objNr == 1 )
				pKlassenVisual = m_pSvgParams->getGmlKlassenVisual ( "xplan:FehlerPunktobjektB" );
			break;
		}
	}
	
	switch ( pPlanObjektP->getGeometrieTyp() )
	{
	case MULTI_POINT:
  case GEO_POINT:
		if ( pPlanObjektP->getObjektTyp() == XP_PPO )
			writePPO ( pPlanObjektP );
		else
		if ( pPlanObjektP->getObjektTyp() == XP_PTO )
			writePTO ( pPlanObjektP );
		else
		if ( pKlassenVisual != NULL )
			writePunktDarstellung( pPlanObjektP, pKlassenVisual, isError );
		break;

	case MULTI_CURVE:
  case CURVE:
  case LINE_STRING:
	case LINE_STRING_SEGMENT:
	case ARC:
  case CIRCLE:
		if ( pPlanObjektP->getObjektTyp() == XP_LPO )
			writeLPO ( pPlanObjektP );
		else
		if ( pPlanObjektP->getObjektTyp() == XP_LTO )
			writeLTO ( pPlanObjektP );
		else
		if ( pKlassenVisual != NULL )
			writeLinienDarstellung( pPlanObjektP, pKlassenVisual, isError );
		break;

	case MULTI_SURFACE:
  case COMPOSITE_SURFACE:
  case SHELL:
  case SURFACE:
  case POLYGON:
		if ( pPlanObjektP->getObjektTyp() == XP_FPO )
			writeFPO ( pPlanObjektP );
		else
		if ( pKlassenVisual != NULL )
			writeFlaechenDarstellung( pPlanObjektP, pKlassenVisual, isError );
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML                                                             //
//  Schreiben der Rasterdarstellung eines Planbereichs             					 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeRasterdarstellung ( Feature * pPlanObjekt )
{
  vector<Feature*> vFeatures;
  string           rasterDatei, georefDatei;
  string           errorText;
  String         ^ RASTER_DATEI; 
  String         ^ GEOREF_DATEI;
  String         ^ STR;
  bool             success;
  vector<double>   georefWerte;
  int              width, height;
  StreamReader   ^ pReader;
  bool             isLocal;

	size_t anz = pPlanObjekt->getRelatedFeatures( "xplan:refScan", vFeatures );
  if ( anz == 0 )
  {
    anzError++;
    errorText = "Rasterdarstellung Plan: Property refScan wurde nicht spezifiziert";    
    m_pFeatures->getLogfile()->writeLogFile ( errorText );

    return;
  }

  Feature * pFasterdarstellung = vFeatures[0];

  success = pFasterdarstellung->getURLAttributWert( "xplan:referenzURL", rasterDatei ) ;
  if ( !success )
  {
    anzError++;
    errorText = "Rasterdarstellung Plan: Keine Referenz auf eine Rasterdatei vorhanden";    
    m_pFeatures->getLogfile()->writeLogFile ( errorText );
    return;
  }

  success = pFasterdarstellung->getURLAttributWert( "xplan:georefURL", georefDatei ) ;
  if ( !success )
  {
    anzError++;
    errorText = "Rasterdarstellung Plan: Keine Referenz auf eine Georeferenzierungsdatei (World-File) vorhanden";    
    m_pFeatures->getLogfile()->writeLogFile ( errorText );
    return;
  }

  RASTER_DATEI = QuConvert::ToString( rasterDatei );

  if ( Uri::IsWellFormedUriString (RASTER_DATEI, UriKind::Absolute ) )
  {
    Uri ^ pUri = gcnew Uri (RASTER_DATEI );
    System::Net::WebClient ^ pWebClient = gcnew System::Net::WebClient;
    Stream ^ pStream = pWebClient->OpenRead ( pUri );
    System::Drawing::Bitmap ^pBitmap = gcnew System::Drawing::Bitmap ( pStream );

    height = pBitmap->Height;
    width = pBitmap->Width;
    isLocal = false;
  }
  else
  {
    if ( Path::IsPathRooted( RASTER_DATEI ) == false )
    {
      string planDatei = m_pFeatures->getFeatureDatei( pFasterdarstellung->getGmlId() );
      STR = QuConvert::ToString( planDatei );
      STR = Path::GetDirectoryName( STR );
      RASTER_DATEI = Path::Combine( STR, RASTER_DATEI );
    }

    if (! File::Exists( RASTER_DATEI ) )
    {
      anzError++;
      errorText = "Rasterdarstellung Plan: Spezifizierte Rasterdatei existiert nicht";    
      m_pFeatures->getLogfile()->writeLogFile ( errorText );
      return;
    }

    System::Drawing::Bitmap ^pBitmap = gcnew System::Drawing::Bitmap ( RASTER_DATEI );
    height = pBitmap->Height;
    width = pBitmap->Width;
    isLocal = true;
  }

  GEOREF_DATEI = QuConvert::ToString( georefDatei );
  if ( Uri::IsWellFormedUriString (RASTER_DATEI, UriKind::Absolute ) )
  {
    Uri ^ pUri = gcnew Uri (GEOREF_DATEI );
    System::Net::WebClient ^ pWebClient = gcnew System::Net::WebClient;
    Stream ^ pStream = pWebClient->OpenRead ( pUri );
    pReader = gcnew StreamReader ( pStream );
    success = readGeorefWerte( georefWerte, pReader  );
  }
  else
  {
    if ( Path::IsPathRooted( GEOREF_DATEI ) == false )
    {
      string planDatei = m_pFeatures->getFeatureDatei( pFasterdarstellung->getGmlId() );
      STR = QuConvert::ToString( planDatei );
      STR = Path::GetDirectoryName( STR );
      GEOREF_DATEI = Path::Combine( STR, GEOREF_DATEI );
    }

    if (! File::Exists( GEOREF_DATEI ) )
    {
      anzError++;
      errorText = "Rasterdarstellung Plan: Spezifizierte Georeferenzierungsdatei existiert nicht";    
      m_pFeatures->getLogfile()->writeLogFile ( errorText );
      return;
    }

    pReader = gcnew StreamReader( GEOREF_DATEI );
    success = readGeorefWerte( georefWerte, pReader  );
  }

  if ( !success || georefWerte.size() < 6 )
  {
    anzError++;
    errorText = "Rasterdarstellung Plan: Falscher Inhalt der Georeferenzierungsdatei";    
    m_pFeatures->getLogfile()->writeLogFile ( errorText );
    return;
  }

  if ( fabs ( georefWerte[1] ) > 1.0E-3 ||  fabs ( georefWerte[2] ) > 1.0E-3 )
  {
    anzError++;
    errorText = "Rasterdarstellung Plan: Drehung des Rasterbildes wird nicht unterstützt";    
    m_pFeatures->getLogfile()->writeLogFile ( errorText );
    return;
  }

  double bitmapWidth  = georefWerte[0] * width;
  double bitmapHeight = -georefWerte[3] * height;
  double xMin = georefWerte[4] - swX;
  double yMin = -georefWerte[5] + noY;

  if ( isLocal )
    RASTER_DATEI = String::Concat( "file:///", RASTER_DATEI );

  writer->WriteStartElement( "image" );
  writer->WriteAttributeString( "xlink:href", RASTER_DATEI );
  writer->WriteAttributeString( "x", XmlConvert::ToString( xMin ) );
  writer->WriteAttributeString( "y", XmlConvert::ToString( yMin ) );
  writer->WriteAttributeString( "width", XmlConvert::ToString( bitmapWidth ) );
  writer->WriteAttributeString( "height", XmlConvert::ToString( bitmapHeight ) );
  writer->WriteAttributeString( "preserveAspectRatio", "none" );
  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Liest eine Georeferenzierungs-Datei                                      //
///////////////////////////////////////////////////////////////////////////////
bool SvgWriter::readGeorefWerte ( vector<double> &georefWerte, StreamReader ^ pReader )
{
  int      zaehler = 0;
  String ^ zeile;
  string   errorText;

  while ( true )
  {
    zeile = pReader->ReadLine();
    if ( zeile == nullptr )
      break;
    zeile = zeile->Trim();
    zeile = zeile->Replace( ".", "," );

    if ( zeile->CompareTo( "" ) != 0 )
    {
      try
      {
        double x = Convert::ToDouble( zeile );
        georefWerte.push_back( x );
        zaehler++;
      }
      catch ( Exception ^ )
      {
        anzError++;
        errorText = "Rasterdarstellung Plan: Falscher Inhalt der Georeferenzierungsdatei";    
        m_pFeatures->getLogfile()->writeLogFile ( errorText );
        return false;
      }
    }
    if ( zaehler > 6 )
      break;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer Punkt-Darstellung des Objektes		            					 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writePunktDarstellung ( Feature * pPlanObjektP, GmlKlassenVisual * pKlassenVisualP, bool isError )
{
	size_t               i, anz;
	bool						      success;
  ATTRIBUT_TYP          refAttributTyp;
	vector<Darstellung*>  vDarstellungen;
	PunktDarstellung    * pPunktDarst;
	PunktDarstellung   * pPunktDarstAct = NULL;
	GmlKlasse          * pGmlKlasse;
	GmlAttribut        * pGmlAttribut;
	GmlAttribut        * pGmlAttributA;
	SvgEnumItem				 * pEnumItem;
  SvgEnumeration     * pEnum;
	string						 selectAttribut1;
	string						 selectAttribut2;
	string						 selectAttribut3;
	string             symbolAttribut;
	string             defaultSymbol;
	string             symbolAttributWert = "";
	string             textAttribut;
	string						 textAttributWert;
	string						 selectAttributWert1 = "";
	string						 selectAttributWert2 = "";
	string						 selectAttributWert3 = "";
	string						 errorText;
	string						 classId;
	string						 gmlId;
	string						 patternId;
	string						 stdString;
	string             symbolId;
	string             klassenName;
	string						 attributA, attributB;
	string						 schluesselNummerA, schluesselNummerB;
	string						 enumA, enumB, enumText;
	string             angleAttribute;
	double             angle = 0;
  vector<GeoPoint*>  vPoints;
	vector<double>     x;
	vector<double>		 y;
	double						 h;
	int							   ih;
	double						 outputSymbolSize = 1.0;
	double						 outputTextSymbolSize = 1.0;

	String					  ^ STR;
	String					  ^ GML_ID;
	String            ^ OUTPUT_STRING = nullptr;
	String					  ^ OUTPUT_SYMBOL = nullptr;
	String					  ^ OUTPUT_TEXT_SYMBOL = nullptr;
	String            ^ OUTPUT_TEXT_STYLE = "TextDefault";
  UOM               * pUOM;

	GmlDictionary * pCodeLists         = m_pFeatures->getGmlSchema()->getCodeListsDictionary();

	writer->Flush();

	if ( !isError )
	{
		klassenName = pPlanObjektP->getKlassenName();
		gmlId = pPlanObjektP->getGmlId();
		GML_ID = QuConvert::ToString ( gmlId );

    GmlSchema  * pGmlObjekte = m_pFeatures->getGmlSchema();
    GmlElement  * pGmlElement = pGmlObjekte->getGmlElement( pKlassenVisualP->getGmlKlassenName() );
    if ( pGmlElement != NULL )
      pGmlKlasse  = pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );
    else
      pGmlKlasse = NULL;

		if ( pGmlKlasse == NULL )
		{
			anzError++;
			errorText = "GML-Klassenname in den Darstellungsvorschriften unbekannt: \"";
			errorText += pKlassenVisualP->getGmlKlassenName();
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
			return;
		}

		selectAttribut1 = pKlassenVisualP->getSelectAttribut1();
		if ( selectAttribut1 != "" )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut1 );
			pPlanObjektP->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut1, selectAttributWert1, pUOM );
		}
		else
			selectAttributWert1 = "_IIII";

		selectAttribut2 = pKlassenVisualP->getSelectAttribut2();
		if ( selectAttribut2 != "" )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut2 );
			pPlanObjektP->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut2, selectAttributWert2, pUOM );
		}
		else
			selectAttributWert2 = "_IIII";

		selectAttribut3 = pKlassenVisualP->getSelectAttribut3();
		if ( selectAttribut3 != "" )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut3 );
			pPlanObjektP->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut3, selectAttributWert3, pUOM );
		}
		else
			selectAttributWert3 = "_IIII";
	}

	anz = pKlassenVisualP->getPunktDarstellungen ( vDarstellungen );
	if ( anz == 0 ) return;

	for ( i = 0; i < anz; i++ )
	{
		pPunktDarst = dynamic_cast<PunktDarstellung*> (vDarstellungen[i]);
		if ( pPunktDarst->isSelected ( selectAttributWert1, selectAttributWert2, selectAttributWert3 ) )
		{
			pPunktDarstAct = pPunktDarst;
			break;
		}
	}

	if ( pPunktDarstAct == NULL )
	{
		for ( i = 0; i < anz; i++ )
		{
			pPunktDarst = dynamic_cast<PunktDarstellung*>(vDarstellungen[i]);
			if ( pPunktDarst->getDefault() == true )
			{
				pPunktDarstAct = pPunktDarst;
				break;
			}
		}
	}

	if ( pPunktDarstAct == NULL )
	{
		anzError++;
		errorText = "Keine passende Punktdarstellung vorhanden für GML-Klasse: \"";
		errorText += pPlanObjektP->getKlassenName();
		errorText += "\"\nObjekt-ID: ";
		errorText+= pPlanObjektP->getGmlId();
		m_pFeatures->getLogfile()->writeLogFile ( errorText );

		if ( selectAttribut1 != "" )
		{
			errorText = "Select Attribut 1 \"";
			errorText+= selectAttribut1;
			errorText+= "\" hat den Wert: ";
			errorText += selectAttributWert1;
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}
		else
		{
			errorText = "Select Attribut 1 nicht definiert ";
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}

		if ( selectAttribut2 != "" )
		{
			errorText = "Select Attribut 2 \"";
			errorText+= selectAttribut2;
			errorText+= "\" hat den Wert: ";
			errorText += selectAttributWert2;
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}
		else
		{
			errorText = "Select Attribut 2 nicht definiert ";
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}

		if ( selectAttribut3 != "" )
		{
			errorText = "Select Attribut 3 \"";
			errorText+= selectAttribut3;
			errorText+= "\" hat den Wert: ";
			errorText += selectAttributWert3;
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}
		else
		{
			errorText = "Select Attribut 3 nicht definiert ";
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}

		m_pFeatures->getLogfile()->writeLogFile ( "\n" );
		return;
	}

	pPlanObjektP->getPunktGeometrie( "", vPoints );

	if ( vPoints.size() == 0 )
	{
		anzError++;
		errorText = "Ungültige Punkt-Geometrie für \n GML-Klasse ";
		errorText += pPlanObjektP->getKlassenName();
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}

	for ( i = 0; i < vPoints.size(); i++ )
	{
		x.push_back( vPoints[i]->x - swX );
		y.push_back( noY - vPoints[i]->y );
	}

	symbolId = pPunktDarstAct->getSymbol();
	if ( symbolId != "" )
		outputSymbolSize = pPunktDarstAct->getSymbolSize();
	else
	{
		symbolId = pKlassenVisualP->getDefaultSymbol();
		outputSymbolSize = pKlassenVisualP->getDefaultSymbolSize();
	}

	if ( symbolId != "" )
		OUTPUT_SYMBOL = QuConvert::ToString ( symbolId );		

	stdString = pPunktDarstAct->getTextStyle();
	if ( stdString != "" ) OUTPUT_TEXT_STYLE = QuConvert::ToString ( stdString );

	angleAttribute = pPunktDarstAct->getAngleAttribute();
	if ( angleAttribute != "" )
		pPlanObjektP->getDoubleAttributWert ( angleAttribute, angle );
	else
		angle = 0.0;

	attributA = pPunktDarstAct->getAttributA ();
	pGmlAttributA = pGmlKlasse->getGmlAttribut ( attributA );
	if ( pGmlAttributA == NULL )
	{

		for ( i = 0; i < x.size(); i++ )
		{
			writer->WriteStartElement ( "use" );

			STR = String::Concat ( "#", OUTPUT_SYMBOL );
			writer->WriteAttributeString ( "xlink:href", STR );

			if ( m_interactiveMap )
			{
				STR = String::Concat ( GML_ID, "_A___" );
				writer->WriteAttributeString ( "id", STR );

				writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
				writer->WriteAttributeString ( "onmouseover", "top.karteInfo(evt)" );
				writer->WriteAttributeString ( "onmouseout", "top.karteOut(evt)" );
			}

			STR = String::Concat ( "translate(", XmlConvert::ToString ( x[i] ), "," );
			STR = String::Concat ( STR, XmlConvert::ToString ( y[i] ), ") " );
			if ( angle != 0.0 )
			{
				STR = String::Concat ( STR, "rotate(", XmlConvert::ToString ( angle ), ") " );
			}
			STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( outputSymbolSize ), ")" );
			writer->WriteAttributeString ( "transform", STR );

			writer->WriteEndElement();
		}

		return;
	}

	switch ( pGmlAttributA->getGmlAttributTyp() )
	{
  case _DATA_TYPE:
  case _REFERENZ:
    success = pPlanObjektP->getReferenzWert ( pGmlAttributA->getReferenziertesElement(), attributA, refAttributTyp, stdString, pUOM );
    if ( success )
      OUTPUT_STRING = QuConvert::ToString ( stdString );
    else
      OUTPUT_STRING = nullptr;


	case _ANY_URI:
	case _STRING:
	case _DATE:
		pPlanObjektP->getStringAttributWert ( attributA, stdString );
		OUTPUT_STRING = QuConvert::ToString ( stdString );

		break;

	case _DOUBLE:
		pPlanObjektP->getDoubleAttributWert ( attributA, h );
		OUTPUT_STRING = QuConvert::ToString ( h, 3 );
		break;

	case _INTEGER:
		pPlanObjektP->getIntegerAttributWert ( attributA, ih );
		OUTPUT_STRING = QuConvert::ToString ( ih );
		break;

	case _BOOLEAN:
		m_pFeatures->getLogfile()->writeLogFile ( "!!!! Anzeige von Boolean Attribute im Plan wird nicht unterstützt" );
		break;


  case _EXTERNAL_CODE_LIST_GML3_3:	
  case _ENUMERATION:
	case _EXTERNAL_CODE_LIST:
		pPlanObjektP->getStringAttributWert ( attributA, schluesselNummerA );
		enumA = pGmlAttributA->getGmlEnumeration();
		attributB = pPunktDarstAct->getAttributB ();
		if ( attributB != "" )
      enumB = m_pGmlObjekte->getGmlEnumerationsName ( klassenName, attributB );
		else
      enumB = "";
				
		if  ( enumB != "" )
		{
			pPlanObjektP->getStringAttributWert ( attributB, schluesselNummerB );
			pEnumItem = m_pSvgParams->getEnumItem ( enumA, enumB, schluesselNummerA, schluesselNummerB );
      pEnum = m_pSvgParams->getEnumeration(  enumA, enumB );
		}
		else
		{
			pPlanObjektP->getStringAttributWert ( attributA, schluesselNummerA );
			pEnumItem = m_pSvgParams->getEnumItem ( enumA, schluesselNummerA );
      pEnum = m_pSvgParams->getEnumeration(  enumA );
		}
		if ( pEnumItem != NULL )
		{
			stdString = pEnumItem->getSymbolId();
			outputSymbolSize = pEnumItem->getSymbolSize();
			if ( stdString != "" ) 
			{
				OUTPUT_SYMBOL = QuConvert::ToString ( stdString );
			}
			else
			{
				stdString = pEnumItem->getText();
				if ( stdString != "" )
				{
					OUTPUT_STRING = QuConvert::ToString ( stdString );
					stdString = pEnumItem->getTextStyle();
					if ( stdString != "" && stdString != "Default" )	OUTPUT_TEXT_STYLE = QuConvert::ToString ( stdString );
					outputTextSymbolSize = pEnumItem->getTextSymbolSize ();
					stdString = pEnumItem->getTextSymbol();
					if ( stdString != "" ) OUTPUT_TEXT_SYMBOL =  QuConvert::ToString ( stdString );
				}
			}
		}
		else
		if ( schluesselNummerA != "" )
		{
      if ( pEnum != NULL)
      {
        outputSymbolSize = pEnum->getDefaultSymbolSize();
        outputTextSymbolSize = pEnum->getDefaultTextSymbolSize();
      }

			GmlDictionaryEntry * pDictionaryEntry = pCodeLists->getDictionaryEntry ( enumA );
			if ( pDictionaryEntry != NULL )
			{
				success = pDictionaryEntry->getSchluesselText ( schluesselNummerA, enumText );
				if ( success )
					OUTPUT_STRING = QuConvert::ToString ( enumText );		
				else
					OUTPUT_STRING = QuConvert::ToString ( schluesselNummerA );
			}
			else
				OUTPUT_STRING = QuConvert::ToString ( schluesselNummerA );
		}
		else
		if ( schluesselNummerB != "" )
		{
			GmlDictionaryEntry * pDictionaryEntry = pCodeLists->getDictionaryEntry ( enumB );
			if ( pDictionaryEntry != NULL )
			{
				success = pDictionaryEntry->getSchluesselText ( schluesselNummerB, enumText );
				if ( success )
					OUTPUT_STRING = QuConvert::ToString ( enumText );		
				else
					OUTPUT_STRING = QuConvert::ToString ( schluesselNummerB );
			}
			else
				OUTPUT_STRING = QuConvert::ToString ( schluesselNummerB );
		}


		break;
	}

	if ( OUTPUT_SYMBOL == nullptr && OUTPUT_STRING == nullptr )
	{
		OUTPUT_SYMBOL = QuConvert::ToString ( "DefaultSymbolPointObject" );		
		outputSymbolSize = pKlassenVisualP->getDefaultSymbolSize();
	}

	if ( OUTPUT_SYMBOL != nullptr )
	{
		for ( i = 0; i < x.size(); i++ )
		{
			writer->WriteStartElement ( "use" );

			STR = String::Concat ( "#", OUTPUT_SYMBOL );
			writer->WriteAttributeString ( "xlink:href", STR );

			if ( m_interactiveMap )
			{
				STR = String::Concat ( GML_ID, "_A___" );
				writer->WriteAttributeString ( "id", STR );

				writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
				writer->WriteAttributeString ( "onmouseover", "top.karteInfo(evt)" );
				writer->WriteAttributeString ( "onmouseout", "top.karteOut(evt)" );
			}

			STR = String::Concat ( "translate(", XmlConvert::ToString ( x[i] ), "," );
			STR = String::Concat ( STR, XmlConvert::ToString ( y[i] ), ") " );
			if ( angle != 0.0 )
			{
				STR = String::Concat ( STR, "rotate(", XmlConvert::ToString ( angle ), ") " );
			}
			STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( outputSymbolSize ), ")" );
			writer->WriteAttributeString ( "transform", STR );

			writer->WriteEndElement();
		}
	}
 
	if (  OUTPUT_STRING != nullptr )
	{
		for ( i = 0; i < x.size(); i++ )
		{
			if ( OUTPUT_TEXT_SYMBOL != nullptr )
			{
				writer->WriteStartElement ( "use" );

				STR = String::Concat ( "#", OUTPUT_TEXT_SYMBOL );
				writer->WriteAttributeString ( "xlink:href", STR );

				STR = String::Concat ( "translate(", XmlConvert::ToString ( x[i] ), "," );
				STR = String::Concat ( STR, XmlConvert::ToString ( y[i] ), ") " );
				STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( outputTextSymbolSize ), ")" );
				writer->WriteAttributeString ( "transform", STR );

				writer->WriteEndElement();
			}

			writer->WriteStartElement ( "text" );
			writer->WriteAttributeString ( "class", OUTPUT_TEXT_STYLE );

			if ( m_interactiveMap )
			{
				STR = String::Concat ( GML_ID, "_A___" );
				writer->WriteAttributeString ( "id", STR );

				writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
				writer->WriteAttributeString ( "onmouseover", "top.karteInfo(evt)" );
				writer->WriteAttributeString ( "onmouseout", "top.karteOut(evt)" );
			}

			STR = String::Concat ( "translate(", XmlConvert::ToString ( x[i] + pPunktDarstAct->getTextOffsetX() ), "," );
			STR = String::Concat ( STR, XmlConvert::ToString ( y[i] + pPunktDarstAct->getTextOffsetY() ), ") " );
			if ( angle != 0.0 )
			{
				STR = String::Concat ( STR, "rotate(", XmlConvert::ToString ( angle ), ") " );
			}
			STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( outputSymbolSize ), ")" );
			writer->WriteAttributeString ( "transform", STR );

			writer->WriteString ( OUTPUT_STRING );
			writer->WriteEndElement();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben XP_PPO                                                     //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writePPO ( Feature * pPlanObjektP )
{
	Feature             * pRefObjekt;
	GmlKlasse			         * pGmlKlasse;
	GmlAttribut			       * pGmlAttribut;
	GeoPoint			         * pPoint;
  vector<GeoPoint*>        vPoints;
	vector<FeatureRelation*> objRelationen;
	string					         attributName;
	string					         klassenNameRef;
	string					         objId;
	string                   errorText;
	string					         stdString;
  string                   textStyle;
	string					         schluesselNummer;
	string					         enumA, enumText;
	string                   stylesheetId;
	string                   defaultSymbol;
	double				        	 defaultSymbolSize = 1.0;
	SvgEnumItem			       * pEnumItem;
	bool			          		 success;
	double		        			 x, y, h;
	size_t		          		 anz;
	size_t                   pointAnz;
	int                      ih;
	double        					 outputSymbolSize = 1.0;
	double				        	 outputTextSymbolSize = 1.0;
	double					         drehwinkel = 0.0;
	double					         skalierung = 1.0;
	ATTRIBUT_TYP		      	 attributTyp;
	String					  ^ STR;
	String            ^ OUTPUT_STRING = nullptr;
	String					  ^ OUTPUT_SYMBOL = nullptr;
	String					  ^ OUTPUT_TEXT_SYMBOL = nullptr;
	String            ^ OUTPUT_TEXT_STYLE;

	GmlDictionary * pCodeLists         = m_pFeatures->getGmlSchema()->getCodeListsDictionary();

	 pointAnz = pPlanObjektP->getPunktGeometrie( "", vPoints );
	if (pointAnz > 0 ) pPoint = vPoints[0];
	else						   pPoint = NULL;

	if ( pPoint == NULL )
	{
		anzError++;
		errorText = "Ungültige Punkt-Geometrie für \n GML-Klasse ";
		errorText += pPlanObjektP->getKlassenName();
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}

	x = pPoint->getX() - swX;
	y = noY - pPoint->getY();

  anz = pPlanObjektP->getFeatureRelationen ( "xplan:dientZurDarstellungVon", objRelationen );
	if ( anz == 0 )
	{
		writePPOFrei ( pPlanObjektP );
		return;
	}

  pPlanObjektP->getStringAttributWert ( "xplan:stylesheetId", stylesheetId );
	OUTPUT_TEXT_STYLE = QuConvert::ToString ( stylesheetId );

  pPlanObjektP->getStringAttributWert ( "xplan:art", attributName );
  attributName = addDefaultNamespaceAttribut( attributName );
  pPlanObjektP->getDoubleAttributWert ( "xplan:drehwinkel", drehwinkel );
  pPlanObjektP->getDoubleAttributWert ( "xplan:skalierung", skalierung );

	
	objId = objRelationen[0]->featureId;
	pRefObjekt = m_pFeatures->getFeature ( objId );
	if ( pRefObjekt == NULL  )
	{
		anzError++;
		errorText = "Falsche Referenz in Klasse XP_PPO ";
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}
	klassenNameRef = pRefObjekt->getKlassenName();
	pGmlKlasse = m_pGmlObjekte->getGmlKlasseFromElementName ( klassenNameRef );
	if ( pGmlKlasse == NULL )
	{
		anzError++;
		errorText = "GML-Klassenname in den Darstellungsvorschriften unbekannt: \"";
		errorText += klassenNameRef;
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}

	pGmlAttribut = pGmlKlasse->getGmlAttribut( attributName );

	if ( pGmlAttribut == NULL )
	{
    anzError++;
    errorText = "GML-Attribut existiert nicht: \"";
    errorText += attributName;
    m_pFeatures->getLogfile()->writeLogFile ( errorText );
    return;
	}

  attributTyp = pGmlAttribut->getGmlAttributTyp();

	if ( attributTyp == _SONST )
	{
		GmlKlassenVisual * pVisual = m_pSvgParams->getGmlKlassenVisual ( klassenNameRef );
		defaultSymbol = pVisual->getDefaultSymbol();
		defaultSymbolSize = pVisual->getDefaultSymbolSize() * skalierung;

		if ( defaultSymbol != "" )
		{
			OUTPUT_SYMBOL = QuConvert::ToString ( defaultSymbol );
			writer->WriteStartElement ( "use" );

			STR = String::Concat ( "#", OUTPUT_SYMBOL );
			writer->WriteAttributeString ( "xlink:href", STR );	
			STR = String::Concat ( "translate(", XmlConvert::ToString ( x ), "," );
			STR = String::Concat ( STR, XmlConvert::ToString ( y ), ") " );
			if ( drehwinkel != 0.0 )
				STR = String::Concat ( STR, "rotate(", XmlConvert::ToString ( -drehwinkel ), ") " );
			STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( defaultSymbolSize ), ")" );
			writer->WriteAttributeString ( "transform", STR );
	
			writer->WriteEndElement();
		}
		return;
	}
	
	switch ( attributTyp )
	{
	case _ANY_URI:
	case _STRING:
	case _DATE:
		pRefObjekt->getStringAttributWert ( attributName, stdString );
		OUTPUT_STRING = QuConvert::ToString ( stdString );
		outputSymbolSize = skalierung;
		break;

	case _DOUBLE:
		pRefObjekt->getDoubleAttributWert ( attributName, h );
		//OUTPUT_STRING = QuConvert::ToString ( h, 3 );
		OUTPUT_STRING = XmlConvert::ToString ( h );

		break;

	case _INTEGER:
		pRefObjekt->getIntegerAttributWert ( attributName, ih );
		OUTPUT_STRING = QuConvert::ToString ( ih );
		break;

	case _BOOLEAN:
		m_pFeatures->getLogfile()->writeLogFile ( "!!!! Anzeige von Boolean Attribute im Plan wird nicht unterstützt" );
		break;
	
	case _ENUMERATION:
	case _EXTERNAL_CODE_LIST:
  case _EXTERNAL_CODE_LIST_GML3_3:
		pRefObjekt->getStringAttributWert ( attributName, schluesselNummer );
		enumA = pGmlAttribut->getGmlEnumeration();
		pEnumItem = m_pSvgParams->getEnumItem ( enumA, schluesselNummer );	
		
		if ( pEnumItem != NULL )
		{
			stdString = pEnumItem->getSymbolId();
			outputSymbolSize = pEnumItem->getSymbolSize();
			if ( stdString != "" ) 
			{
				OUTPUT_SYMBOL = QuConvert::ToString ( stdString );
        outputSymbolSize = outputSymbolSize*skalierung;
			}
			else
			{
				stdString = pEnumItem->getText();
				if ( stdString != "" )
				{
					OUTPUT_STRING = QuConvert::ToString ( stdString );
					textStyle = pEnumItem->getTextStyle();
					if ( textStyle != "" )
          {
						OUTPUT_TEXT_STYLE = QuConvert::ToString ( textStyle );
          }

					outputTextSymbolSize = pEnumItem->getTextSymbolSize ();
          outputTextSymbolSize = outputTextSymbolSize*skalierung;
					stdString = pEnumItem->getTextSymbol();
					if ( stdString != "" ) OUTPUT_TEXT_SYMBOL =  QuConvert::ToString ( stdString );
				}
			}
		}
		else
		{
      OUTPUT_SYMBOL      = nullptr;
      OUTPUT_TEXT_SYMBOL = nullptr;
      OUTPUT_TEXT_STYLE  = "TextBold4";
      outputSymbolSize   = 1.0;

			GmlDictionaryEntry * pDictionaryEntry = pCodeLists->getDictionaryEntry ( enumA );
			if ( pDictionaryEntry != NULL )
			{
				success = pDictionaryEntry->getSchluesselText ( schluesselNummer, enumText );
				if ( success )
					OUTPUT_STRING = QuConvert::ToString ( enumText );		
				else
					OUTPUT_STRING = QuConvert::ToString ( schluesselNummer );
			}
			else
				OUTPUT_STRING = QuConvert::ToString ( schluesselNummer );
		}
		break;
	}

	if ( OUTPUT_SYMBOL != nullptr )
	{
		writer->WriteStartElement ( "use" );

		STR = String::Concat ( "#", OUTPUT_SYMBOL );
		writer->WriteAttributeString ( "xlink:href", STR );	
		STR = String::Concat ( "translate(", XmlConvert::ToString ( x ), "," );
		STR = String::Concat ( STR, XmlConvert::ToString ( y ), ") " );
		if ( drehwinkel != 0.0 )
			STR = String::Concat ( STR, "rotate(", XmlConvert::ToString ( -drehwinkel ), ") " );
		STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( outputSymbolSize ), ")" );
		writer->WriteAttributeString ( "transform", STR );

		writer->WriteEndElement();
	}
	
	if (  OUTPUT_STRING != nullptr )
	{
		if ( OUTPUT_TEXT_SYMBOL != nullptr )
		{
			writer->WriteStartElement ( "use" );

			STR = String::Concat ( "#", OUTPUT_TEXT_SYMBOL );
			writer->WriteAttributeString ( "xlink:href", STR );

			STR = String::Concat ( "translate(", XmlConvert::ToString ( x ), "," );
			STR = String::Concat ( STR, XmlConvert::ToString ( y ), ") " );
			if ( drehwinkel != 0.0 )
				STR = String::Concat ( STR, "rotate(", XmlConvert::ToString ( -drehwinkel ), ") " );
			STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( outputTextSymbolSize ), ")" );
			writer->WriteAttributeString ( "transform", STR );

			writer->WriteEndElement();
		}		
		
		writer->WriteStartElement ( "text" );
		writer->WriteAttributeString ( "class", OUTPUT_TEXT_STYLE );	
		
		STR = String::Concat ( "translate(", XmlConvert::ToString ( x ), "," );
		STR = String::Concat ( STR, XmlConvert::ToString ( y ), ") " );
		if ( drehwinkel != 0.0 )
			STR = String::Concat ( STR, "rotate(", XmlConvert::ToString ( -drehwinkel ), ") " );
		STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( outputSymbolSize ), ")" );
		writer->WriteAttributeString ( "transform", STR );

		writer->WriteString ( OUTPUT_STRING );
		writer->WriteEndElement();

		
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben eines freien Präsentationsobjektes XP_PPO                  //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writePPOFrei ( Feature * pPlanObjektP )
{
	GeoPoint        * pPoint;
	string            stdString;
	string            errorText;
	double            x, y;
	bool              success;
	double            drehwinkel = 0.0;
	double            skalierung = 1.0;
  vector<GeoPoint*> vPoints;
	size_t            pointAnz;

	String ^ STYLE_ID;
	String ^ STR;

	pointAnz = pPlanObjektP->getPunktGeometrie( "", vPoints );
	if ( pointAnz > 0 ) pPoint = vPoints[0];
	else						   pPoint = NULL;

	if ( pPoint == NULL )
	{
		anzError++;
		errorText = "Ungültige Punkt-Geometrie für \n GML-Klasse ";
		errorText += pPlanObjektP->getKlassenName();
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}

	x = pPoint->getX() - swX;
	y = noY - pPoint->getY();

  success = pPlanObjektP->getStringAttributWert ( "xplan:stylesheetId", stdString );
	if ( success ) STYLE_ID = QuConvert::ToString ( stdString );
	else		       STYLE_ID = "TextBold2";

  pPlanObjektP->getDoubleAttributWert ( "xplan:drehwinkel", drehwinkel );
  pPlanObjektP->getDoubleAttributWert ( "xplan:skalierung", skalierung );

	writer->WriteStartElement ( "use" );

	STR = String::Concat ( "#", STYLE_ID );
	writer->WriteAttributeString ( "xlink:href", STR );	
	STR = String::Concat ( "translate(", XmlConvert::ToString ( x ), "," );
	STR = String::Concat ( STR, XmlConvert::ToString ( y ), ") " );
	if ( drehwinkel != 0.0 )
		STR = String::Concat ( STR, "rotate(", XmlConvert::ToString ( -drehwinkel ), ") " );
	STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( skalierung ), ")" );
	writer->WriteAttributeString ( "transform", STR );

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben eines Präsentationsobjektes XP_PTO                         //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writePTO ( Feature * pPlanObjektP )
{
	GeoPoint               * pPoint;
	string                   errorText;
	double                   x, y;
	size_t                   anz;
	size_t                   pointAnz;
	vector<FeatureRelation*> objRelationen;
  vector<GeoPoint*>        vPoints;

  pointAnz = pPlanObjektP->getPunktGeometrie( "", vPoints );
	if ( pointAnz > 0 ) pPoint = vPoints[0];
	else						    pPoint = NULL;

	if ( pPoint == NULL )
	{
		anzError++;
		errorText = "Ungültige Punkt-Geometrie für \n GML-Klasse ";
		errorText += pPlanObjektP->getKlassenName();
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}

	x = pPoint->getX() - swX;
	y = noY - pPoint->getY();

  anz = pPlanObjektP->getFeatureRelationen ( "xplan:dientZurDarstellungVon", objRelationen );
	if ( anz == 0 )
	{
		writePTOFrei ( pPlanObjektP );
		return;
	}

	writePPO ( pPlanObjektP );
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben eines freien Präsentationsobjektes XP_PTO                  //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writePTOFrei ( Feature * pPlanObjektP )
{
	GeoPoint        * pPoint;
	string            stdString;
	string            errorText;
	double            x, y;
	bool              success;
	double            drehwinkel = 0.0;
	double            skalierung = 1.0;
	string            schriftinhalt;
	String          ^ STYLE_ID;
	String          ^ OUTPUT_STRING;
	String          ^ STR;
  vector<GeoPoint*> vPoints;
	size_t            pointAnz;

	pointAnz = pPlanObjektP->getPunktGeometrie( "", vPoints );
	if ( pointAnz > 0 ) pPoint = vPoints[0];
	else						    pPoint = NULL;

	if ( pPoint == NULL )
	{
		anzError++;
		errorText = "Ungültige Punkt-Geometrie für \n GML-Klasse ";
		errorText += pPlanObjektP->getKlassenName();
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}

	x = pPoint->getX() - swX;
	y = noY - pPoint->getY();

  success = pPlanObjektP->getStringAttributWert ( "xplan:stylesheetId", stdString );
	if ( success ) STYLE_ID = QuConvert::ToString ( stdString );
	else		   STYLE_ID = "DefaultSymbol";

  pPlanObjektP->getDoubleAttributWert ( "xplan:drehwinkel", drehwinkel );
  pPlanObjektP->getDoubleAttributWert ( "xplan:skalierung", skalierung );
  pPlanObjektP->getStringAttributWert ( "xplan:schriftinhalt", schriftinhalt );
	OUTPUT_STRING = QuConvert::ToString ( schriftinhalt );

	writer->WriteStartElement ( "text" );
	writer->WriteAttributeString ( "class", STYLE_ID );	
		
	STR = String::Concat ( "translate(", XmlConvert::ToString ( x ), "," );
	STR = String::Concat ( STR, XmlConvert::ToString ( y ), ") " );
	if ( drehwinkel != 0.0 )
		STR = String::Concat ( STR, "rotate(", XmlConvert::ToString ( -drehwinkel ), ") " );
	STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( skalierung ), ")" );
	writer->WriteAttributeString ( "transform", STR );

	writer->WriteString ( OUTPUT_STRING );
	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer Linien-Darstellung des Objektes		            					 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeLinienDarstellung ( Feature * pPlanObjektP, GmlKlassenVisual * pKlassenVisualP, bool isError )
{
	size_t                 i, anz;
  int                    k;
	vector<Darstellung*>   vDarstellungen;
	vector<LinienParams*>  vLinienParams;
	LinienDarstellung    * pLinienDarst;
	LinienParams		     * pLinienParams;
	LinienDarstellung    * pLinienDarstAct = NULL;
	_Curve					     * pCurve;
  GmlKlasse            * pGmlKlasse;
  GmlAttribut          * pGmlAttribut;
	string						     selectAttribut1;
	string						     selectAttribut2;
	string						     selectAttribut3;
	string                 symbolAttribut;
	string                 symbolAttributWert = "";
	string                 textAttribut;
	string						     textAttributWert;
	string						     selectAttributWert1 = "";
	string						     selectAttributWert2 = "";
	string						     selectAttributWert3 = "";
	string						     errorText;
	string						     classId;
	string						     gmlId;
	string						     startMarker;
	string						     endMarker;
	string					    	 stdString;
	String					     ^ STR;
	String					     ^ GML_ID;
	String					     ^ GML_ID_A;
	String					     ^ PATH_STRING;
	String					     ^ CLASS_ID;
  vector<_Curve*>        vCurves;
	size_t                 curveAnz;
  UOM                 * pUOM;

	writer->Flush();

	anz = pKlassenVisualP->getLinienDarstellungen ( vDarstellungen );
	if ( !isError )
	{
		gmlId = pPlanObjektP->getGmlId();
		GML_ID = QuConvert::ToString ( gmlId );

    GmlSchema  * pGmlObjekte = m_pFeatures->getGmlSchema();
    GmlElement  * pGmlElement = pGmlObjekte->getGmlElement( pKlassenVisualP->getGmlKlassenName() );
    if ( pGmlElement != NULL )
      pGmlKlasse  = pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );
    else
      pGmlKlasse = NULL;

		if ( pGmlKlasse == NULL )
		{
			anzError++;
			errorText = "GML-Klassenname in den Darstellungsvorschriften unbekannt: \"";
			errorText += pKlassenVisualP->getGmlKlassenName();
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
			return;
		}

		selectAttribut1 = pKlassenVisualP->getSelectAttribut1();
		if ( selectAttribut1 != "" )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut1 );
      if ( pGmlAttribut != NULL )
			  pPlanObjektP->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut1, selectAttributWert1, pUOM );
      else
        selectAttributWert1 = "";
		}
		else
			selectAttributWert1 = "_IIII";

		selectAttribut2 = pKlassenVisualP->getSelectAttribut2();
		if ( selectAttribut2 != "" )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut2 );
      if ( pGmlAttribut != NULL )
  			pPlanObjektP->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut2, selectAttributWert2, pUOM );
      else
        selectAttributWert2 = "";
		}
		else
			selectAttributWert2 = "_IIII";

		selectAttribut3 = pKlassenVisualP->getSelectAttribut3();
		if ( selectAttribut3 != "" )
		{	
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut3 );
      if ( pGmlAttribut != NULL )
  			pPlanObjektP->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut3, selectAttributWert3, pUOM );
      else
        selectAttributWert3 = "";
		}
		else
			selectAttributWert3 = "_IIII";

		for ( i = 0; i < anz; i++ )
		{
			pLinienDarst = dynamic_cast<LinienDarstellung*>(vDarstellungen[i]);
			if ( pLinienDarst->isSelected ( selectAttributWert1, selectAttributWert2, selectAttributWert3 ) )
			{
				pLinienDarstAct = pLinienDarst;
				break;
			}
		}
	}

	if ( pLinienDarstAct == NULL )
	{
		for ( i = 0; i < anz; i++ )
		{
			pLinienDarst = dynamic_cast<LinienDarstellung*>(vDarstellungen[i]);
			if ( pLinienDarst->getDefault() == true )
			{
				pLinienDarstAct = pLinienDarst;
				break;
			}
		}
	}

	if ( pLinienDarstAct == NULL )
	{
		anzError++;
		errorText = "Keine passende Liniendarstellung vorhanden für GML-Klasse: \"";
		errorText += pPlanObjektP->getKlassenName();
		errorText += "\"\nObjekt-ID: ";
		errorText+= pPlanObjektP->getGmlId();
		m_pFeatures->getLogfile()->writeLogFile ( errorText );

		if ( selectAttribut1 != "" )
		{
			errorText = "Select Attribut 1 \"";
			errorText+= selectAttribut1;
			errorText+= "\" hat den Wert: ";
			errorText += selectAttributWert1;
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}
		else
		{
			errorText = "Select Attribut 1 nicht definiert ";
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}

		if ( selectAttribut2 != "" )
		{
			errorText = "Select Attribut 2 \"";
			errorText+= selectAttribut2;
			errorText+= "\" hat den Wert: ";
			errorText += selectAttributWert2;
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}
		else
		{
			errorText = "Select Attribut 2 nicht definiert ";
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}

		if ( selectAttribut3 != "" )
		{
			errorText = "Select Attribut 3 \"";
			errorText+= selectAttribut3;
			errorText+= "\" hat den Wert: ";
			errorText += selectAttributWert3;
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}
		else
		{
			errorText = "Select Attribut 3 nicht definiert ";
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}

		m_pFeatures->getLogfile()->writeLogFile ( "\n" );


		return;
	}

	curveAnz = pPlanObjektP->getLinienGeometrie( "", vCurves );
	if ( curveAnz == 0 )
	{
		anzError++;
		errorText = "Ungültige Linien-Geometrie für \n GML-Klasse ";
		errorText += pPlanObjektP->getKlassenName();
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}	

	for ( k = 0; k < curveAnz; k++ )
	{
		pCurve = vCurves[k];
		if ( k > 99 ) break;

		STR =  XmlConvert::ToString ( k );
		if ( STR->Length == 1 )
			STR = String::Concat ( "0", STR );

		GML_ID_A = String::Concat ( GML_ID, STR );

		stdString = pCurve->getPointsSVG ( swX, noY );
		PATH_STRING = QuConvert::ToString ( stdString );

		writer->WriteStartElement ( "defs" );
		writer->WriteStartElement ( "path" );
		writer->WriteAttributeString ( "id", GML_ID_A );
		writer->WriteAttributeString ( "d", PATH_STRING );
		writer->WriteEndElement();
		writer->WriteEndElement();

		anz = pLinienDarstAct->getLinienParams ( vLinienParams );
		for ( i = 0; i < anz; i++ )
		{
			pLinienParams = vLinienParams[i];

			classId   = pLinienParams->getClassId();

			writer->WriteStartElement ( "use" );
			STR = String::Concat ( "#", GML_ID_A );
			writer->WriteAttributeString ( "xlink:href", STR );

			if ( m_interactiveMap )
			{
				STR = String::Concat ( GML_ID_A, "_A", XmlConvert::ToString ( i ) );
				writer->WriteAttributeString ( "id", STR );

				writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
				writer->WriteAttributeString ( "onmouseover", "top.karteInfo(evt)" );
				writer->WriteAttributeString ( "onmouseout", "top.karteOut(evt)" );
			}

      if ( classId != "" )
      {
			  CLASS_ID = QuConvert::ToString ( classId );
			  writer->WriteAttributeString ( "class", CLASS_ID );
      }

			startMarker = pLinienDarstAct->getStartMarker();
			if ( startMarker != "" )
			{
				STR = String::Concat ( "url(#", QuConvert::ToString ( startMarker ), ")" );
				writer->WriteAttributeString ( "marker-start", STR );
			}

			endMarker = pLinienDarstAct->getEndMarker();
			if ( endMarker != "" )
			{
				STR = String::Concat ( "url(#", QuConvert::ToString ( endMarker ), ")" );
				writer->WriteAttributeString ( "marker-end", STR );
			}

			writer->WriteEndElement();
		}
	}

	if ( pLinienDarstAct->getLineTextParams() != NULL )
		writeLineText ( pPlanObjektP, pLinienDarstAct->getLineTextParams() );

	if ( pLinienDarstAct->getLineSymbolParams() !=NULL )
		writeLineSymbol ( pPlanObjektP, pLinienDarstAct->getLineSymbolParams() );


}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer Linien-Darstellung des Objektes		            					 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeLinienDarstellung ( Feature * pPlanObjektP, LinienDarstellung * pLinienDarstellung )
{
	size_t                i, anz;
  int                   k;
	vector<LinienParams*>	vLinienParams;
	LinienParams		    * pLinienParams;
	_Curve					    * pCurve;
	string                symbolAttribut;
	string                symbolAttributWert = "";
	string                textAttribut;
	string						    textAttributWert;
	string						    errorText;
	string						    classId;
	string						    gmlId;
	string						    startMarker;
	string						    endMarker;
	string						    stdString;
	String					     ^ STR;
	String					     ^ GML_ID;
	String					     ^ GML_ID_A;
	String					     ^	PATH_STRING;
	String					     ^ CLASS_ID;
  vector<_Curve*>        vCurves;
	size_t                 curveAnz;

	writer->Flush();

	gmlId = pPlanObjektP->getGmlId();
	GML_ID = QuConvert::ToString ( gmlId );

	curveAnz = pPlanObjektP->getLinienGeometrie( "", vCurves );
	if ( curveAnz == 0 )
	{
		anzError++;
		errorText = "Ungültige Linien-Geometrie für \n GML-Klasse ";
		errorText += pPlanObjektP->getKlassenName();
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}	

	for ( k = 0; k < curveAnz; k++ )
	{
		pCurve = vCurves[k];
		if ( k > 99 ) break;

		STR =  XmlConvert::ToString ( k );
		if ( STR->Length == 1 )
			STR = String::Concat ( "0", STR );

		GML_ID_A = String::Concat ( GML_ID, STR );

		stdString = pCurve->getPointsSVG ( swX, noY );
		PATH_STRING = QuConvert::ToString ( stdString );

		writer->WriteStartElement ( "defs" );
		writer->WriteStartElement ( "path" );
		writer->WriteAttributeString ( "id", GML_ID_A );
		writer->WriteAttributeString ( "d", PATH_STRING );
		writer->WriteEndElement();
		writer->WriteEndElement();

		anz = pLinienDarstellung->getLinienParams ( vLinienParams );
		for ( i = 0; i < anz; i++ )
		{
			pLinienParams = vLinienParams[i];

			classId   = pLinienParams->getClassId();

			writer->WriteStartElement ( "use" );
			STR = String::Concat ( "#", GML_ID_A );
			writer->WriteAttributeString ( "xlink:href", STR );

			if ( m_interactiveMap )
			{
				STR = String::Concat ( GML_ID_A, "_A", XmlConvert::ToString ( i ) );
				writer->WriteAttributeString ( "id", STR );

				writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
				writer->WriteAttributeString ( "onmouseover", "top.karteInfo(evt)" );
				writer->WriteAttributeString ( "onmouseout", "top.karteOut(evt)" );
			}

      if ( classId != "" )
      {
			  CLASS_ID = QuConvert::ToString ( classId );
			  writer->WriteAttributeString ( "class", CLASS_ID );
      }

			startMarker = pLinienDarstellung->getStartMarker();
			if ( startMarker != "" )
			{
				STR = String::Concat ( "url(#", QuConvert::ToString ( startMarker ), ")" );
				writer->WriteAttributeString ( "marker-start", STR );
			}

			endMarker = pLinienDarstellung->getEndMarker();
			if ( endMarker != "" )
			{
				STR = String::Concat ( "url(#", QuConvert::ToString ( endMarker ), ")" );
				writer->WriteAttributeString ( "marker-end", STR );
			}

			writer->WriteEndElement();
		}
	}

	if ( pLinienDarstellung->getLineTextParams() != NULL )
		writeLineText ( pPlanObjektP, pLinienDarstellung->getLineTextParams() );

	if ( pLinienDarstellung->getLineSymbolParams() !=NULL )
		writeLineSymbol ( pPlanObjektP, pLinienDarstellung->getLineSymbolParams() );

}

///////////////////////////////////////////////////////////////////////////////
//  Scheiben eines Präsentationsobjektes XP_LPO                              //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeLPO ( Feature * pPlanObjektP )
{
	vector<FeatureRelation*> objRelationen;
	size_t        					 anz;

  anz = pPlanObjektP->getFeatureRelationen ( "xplan:dientZurDarstellungVon", objRelationen );
	if ( anz == 0 )
	{
		writeLPOFrei ( pPlanObjektP );
		return;
	}

	m_pFeatures->getLogfile()->writeLogFile ( "!!! Linienhafte Präsentationsobjekte mit Referenz auf Fachobjekte werden nicht untertstützt" );
}

///////////////////////////////////////////////////////////////////////////////
//  Scheiben eines freien Präsentationsobjektes XP_LPO                       //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeLPOFrei ( Feature * pPlanObjektP )
{
  string              idLinienDarstellung;
	string              errorText;
	bool	              success;
  LinienDarstellung * pLinienDarstellung;

  success = pPlanObjektP->getStringAttributWert ( "xplan:stylesheetId", idLinienDarstellung );
  if ( !success )
  {
		anzError++;
		errorText = "Ungültiger Linien-Style in XP_LPO ";
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
  }

  pLinienDarstellung = m_pSvgParams->getLinienDarstellung ( idLinienDarstellung );
  if ( pLinienDarstellung == NULL )
  {
		anzError++;
		errorText = "Ungültiger Linien-Style in XP_LPO ";
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
  }

  writeLinienDarstellung ( pPlanObjektP, pLinienDarstellung );	
}

///////////////////////////////////////////////////////////////////////////////
//  Scheiben eines Präsentationsobjektes XP_LTO                              //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeLTO ( Feature * pPlanObjektP )
{
  string                   errorText;
	size_t                   anz;
	size_t                   curveAnz;
	vector<FeatureRelation*> objRelationen;
  vector<_Curve*>          vCurves;

  curveAnz = pPlanObjektP->getLinienGeometrie( "", vCurves );
	if ( curveAnz == 0 )
	{
		anzError++;
		errorText = "Ungültige Linien-Geometrie für \n GML-Klasse ";
		errorText += pPlanObjektP->getKlassenName();
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}	

  anz = pPlanObjektP->getFeatureRelationen ( "xplan:dientZurDarstellungVon", objRelationen );
	if ( anz == 0 )
	{
		writeLTOFrei ( pPlanObjektP );
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Scheiben eines freien Präsentationsobjektes XP_LTO                              //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeLTOFrei ( Feature * pPlanObjektP )
{
	_Curve        * pCurve;
	string          stdString;
	string          errorText;
	bool            success;
	double          skalierung = 1.0;
	string          schriftinhalt;
	String        ^ STYLE_ID;
	String        ^ OUTPUT_STRING;
	String        ^ PATH_STRING;
  String        ^ GML_ID;
  vector<_Curve*> vCurves;
	size_t          curveAnz;

  stdString = pPlanObjektP->getGmlId();
  GML_ID    = QuConvert::ToString ( stdString );

 curveAnz = pPlanObjektP->getLinienGeometrie( "", vCurves );
 if ( curveAnz > 0 )
    pCurve = vCurves[0];
 else
   return;

  success = pPlanObjektP->getStringAttributWert ( "xplan:stylesheetId", stdString );
	if ( success ) STYLE_ID = QuConvert::ToString ( stdString );
	else		   STYLE_ID = "DefaultText";

	pPlanObjektP->getDoubleAttributWert ( "xplan:skalierung", skalierung );
	pPlanObjektP->getStringAttributWert ( "xplan:schriftinhalt", schriftinhalt );
	OUTPUT_STRING = QuConvert::ToString ( schriftinhalt );

	stdString = pCurve->getPointsSVG ( swX, noY );
	PATH_STRING = QuConvert::ToString ( stdString );

  writer->WriteStartElement ( "defs" );
  writer->WriteStartElement ( "path" );
	writer->WriteAttributeString ( "id", GML_ID );
	writer->WriteAttributeString ( "d", PATH_STRING );
	writer->WriteEndElement();
	writer->WriteEndElement();

	GML_ID = String::Concat ( "#", GML_ID );

	writer->WriteStartElement ( "text" );
	writer->WriteAttributeString ( "class", STYLE_ID );

	writer->WriteStartElement ( "textPath" );
	writer->WriteAttributeString ( "xlink:href", GML_ID );

	writer->WriteStartElement ( "tspan" );
	writer->WriteString ( OUTPUT_STRING );

	writer->WriteEndElement();
	writer->WriteEndElement();
	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer Flächen-Darstellung des Objektes				            		 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeFlaechenDarstellung ( Feature * pPlanObjektP, GmlKlassenVisual * pKlassenVisualP, bool isError )
{
	size_t                       i, j, anz, iPoly, polyAnz;
	vector<Darstellung*>         vDarstellungen;
	vector<FlaechenParams*>		   vFlaechenParams;
	vector<PolygonGeo*>			       polygone;
	FlaechenDarstellung        * pFlaechenDarst;
	FlaechenParams		         * pFlaechenParams;
	FlaechenDarstellung        * pFlaechenDarstAct = NULL;
	string						           selectAttribut1;
	string						           selectAttribut2;
	string						           selectAttribut3;
	string                       symbolAttribut;
	string                       symbolAttributWert = "";
	string                       textAttribut;
	string						           textAttributWert;
	string						           selectAttributWert1 = "";
	string						           selectAttributWert2 = "";
	string						           selectAttributWert3 = "";
	string						           errorText;
	string					          	 classId;
	string					          	 gmlId;
	string                       klassenName;
	string					          	 patternId;
	string                       hatchingId;
	string						           stdString;
	bool						             clip, useMask;
	double						           no[3], sw[3];
	String					           ^ STR;
	String					           ^ GML_ID;
	String                     ^ GML_ID_A;
	String				             ^ PATH_STRING;
	String                     ^ STR_NUMBER;
	String					           ^ STYLE_STRING;
	String					           ^ CLASS_ID;
	GmlAttribut                * pGmlAttribut;
	PolygonGeo					           * pPoly;
	_Ring					             * pRing;
  GmlKlasse                  * pGmlKlasse;
  string                       srsName;
  int                          srsDimension;
  UOM                        * pUOM;

	writer->Flush();

	polyAnz = pPlanObjektP->getFlaechenGeometrie( "", polygone );

	if ( isError )
	{
		gmlId = pPlanObjektP->getGmlId();
		GML_ID = QuConvert::ToString ( gmlId );
		
		anz = pKlassenVisualP->getFlaechenDarstellungen ( vDarstellungen );
		if ( anz == 0 ) return;

		anz = dynamic_cast<FlaechenDarstellung*>(vDarstellungen[0])->getFlaechenParams ( vFlaechenParams );
		if ( anz == 0 ) return;

		classId = vFlaechenParams[0]->getClassId();


		for ( iPoly = 0; iPoly < polyAnz; iPoly++ )
		{
			pPoly = polygone[iPoly];
			pRing = pPoly->getAussenkontur();

			STR =  XmlConvert::ToString ( iPoly );
			if ( STR->Length == 1 )
				STR = String::Concat ( "0", STR );

			GML_ID_A = String::Concat ( GML_ID, STR );

			writer->WriteStartElement ( "use" );

			STR = String::Concat ( "#", GML_ID_A );
			writer->WriteAttributeString ( "xlink:href", STR );

      if ( classId != "" )
      {
			  STR = QuConvert::ToString ( classId );
			  writer->WriteAttributeString ( "class", STR );
      }

			STR = String::Concat ( GML_ID_A, "_A0" );
			writer->WriteAttributeString ( "id", STR );
		
			writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );

			if ( pPoly->getLochAnz() > 0 )
			{
				STR = String::Concat ( "url(#", GML_ID_A, "_mask)" );
				writer->WriteAttributeString ( "mask", STR );
			}

			writer->WriteEndElement();
		}
		return;
	}

	gmlId = pPlanObjektP->getGmlId();
	GML_ID = QuConvert::ToString ( gmlId );

	GmlSchema  * pGmlObjekte = m_pFeatures->getGmlSchema();
  GmlElement  * pGmlElement = pGmlObjekte->getGmlElement( pKlassenVisualP->getGmlKlassenName() );
  if ( pGmlElement != NULL )
	  pGmlKlasse  = pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );
  else
    pGmlKlasse = NULL;

	if ( pGmlKlasse == NULL )
	{
		anzError++;
		errorText = "GML-Klassenname in den Darstellungsvorschriften unbekannt: \"";
		errorText += pKlassenVisualP->getGmlKlassenName();
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
	}
	klassenName = pGmlKlasse->getGmlElementName();

	selectAttribut1 = pKlassenVisualP->getSelectAttribut1();
	if ( selectAttribut1 != "" )
	{
		pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut1 );
		if ( pGmlAttribut != NULL )
			pPlanObjektP->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut1, selectAttributWert1, pUOM );
		else
			selectAttributWert1 = "";
	}
	else
		selectAttributWert1 = "_IIII";

	selectAttribut2 = pKlassenVisualP->getSelectAttribut2();
	if ( selectAttribut2 != "" )
	{
		pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut2 );
		if (  pGmlAttribut != NULL )
			pPlanObjektP->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut2, selectAttributWert2, pUOM );
		else
			selectAttributWert2 = "";
	}
	else
		selectAttributWert2 = "_IIII";

	selectAttribut3 = pKlassenVisualP->getSelectAttribut3();
	if ( selectAttribut3 != "" )
	{
		pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut3 );
		if (  pGmlAttribut != NULL )
			pPlanObjektP->getAttributWert ( pGmlAttribut->getGmlAttributTyp(), selectAttribut3, selectAttributWert3, pUOM );
		else
			selectAttributWert3 = "";
	}
	else
		selectAttributWert3 = "_IIII";

  anz = pKlassenVisualP->getFlaechenDarstellungen ( vDarstellungen );

	for ( i = 0; i < anz; i++ )
	{
		pFlaechenDarst = dynamic_cast<FlaechenDarstellung*>(vDarstellungen[i]);
		if ( pFlaechenDarst->isSelected ( selectAttributWert1, selectAttributWert2, selectAttributWert3 ) )
		{
			pFlaechenDarstAct = pFlaechenDarst;
			break;
		}
	}

	if ( pFlaechenDarstAct == NULL )
	{
		for ( i = 0; i < anz; i++ )
		{
			pFlaechenDarst = dynamic_cast<FlaechenDarstellung*>(vDarstellungen[i]);
			if ( pFlaechenDarst->getDefault() == true )
			{
				pFlaechenDarstAct = pFlaechenDarst;
				break;
			}
		}
	}

	if ( pFlaechenDarstAct == NULL )
	{
			anzError++;
			errorText = "Keine passende Flächendarstellung vorhanden für GML-Klasse: \"";
			errorText += pPlanObjektP->getKlassenName();
			errorText += "\"\nObjekt-ID:  ";
			errorText+= pPlanObjektP->getGmlId();
			m_pFeatures->getLogfile()->writeLogFile ( errorText );

		if ( selectAttribut1 != "" )
		{
			errorText = "Select Attribut 1 \"";
			errorText+= selectAttribut1;
			errorText+= "\" hat den Wert: ";
			errorText += selectAttributWert1;
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}
		else
		{
			errorText = "Select Attribut 1 nicht definiert ";
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}

		if ( selectAttribut2 != "" )
		{
			errorText = "Select Attribut 2 \"";
			errorText+= selectAttribut2;
			errorText+= "\" hat den Wert: ";
			errorText += selectAttributWert2;
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}
		else
		{
			errorText = "Select Attribut 2 nicht definiert ";
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}

		if ( selectAttribut3 != "" )
		{
			errorText = "Select Attribut 3 \"";
			errorText+= selectAttribut3;
			errorText+= "\" hat den Wert: ";
			errorText += selectAttributWert3;
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}
		else
		{
			errorText = "Select Attribut 3 nicht definiert ";
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
		}

		m_pFeatures->getLogfile()->writeLogFile ( "\n" );
			return;
	}

	if ( polyAnz == 0 )
	{
		if ( pPlanObjektP->getKlassenName() == "xplan:XP_RasterplanBasis" ||
         pPlanObjektP->getKlassenName() == "xplan:XP_Rasterdarstellung" )
		{
			PlanBereich * pPlanBereich = pPlanObjektP->getPlanBereich ();
			if ( pPlanBereich->getGeometryPropertyAnz( "" ) > 0 )
				pPlanObjektP = pPlanBereich;
			else
				pPlanObjektP = pPlanBereich->getBPlan();
		}
		else
		{
			anzError++;
			errorText = "Ungültige Flächen-Geometrie für \n GML-Klasse ";
			errorText += pPlanObjektP->getKlassenName();
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
			return;
		}
	}

	if ( m_interactiveMap && klassenName != "xplan:BP_Plan" && klassenName != "xplan:FP_Plan" &&
						   klassenName != "xplan:RP_Plan" && klassenName != "xplan:LP_Plan" && klassenName != "xplan:SO_Plan" )
		useMask = true;
	else
	{
		if ( pPlanObjektP->getFlaechenschluss() == false && pFlaechenDarstAct->getDeckend() == true )
			useMask = true;
		else
			useMask = false;
	}

	for ( iPoly = 0; iPoly < polyAnz; iPoly++ )
	{
	pPoly = polygone[iPoly];
	pRing = pPoly->getAussenkontur();

	stdString = pRing->getPointsReverseSVG ( swX, noY );
	PATH_STRING = QuConvert::ToString ( stdString );

	if ( iPoly > 99 ) break;

	STR =  XmlConvert::ToString ( iPoly );
	if ( STR->Length == 1 )
		STR = String::Concat ( "0", STR );

	GML_ID_A = String::Concat ( GML_ID, STR );

	STR = String::Concat ( GML_ID_A, "_clip" );
	writer->WriteStartElement ( "clipPath" );
	writer->WriteAttributeString ( "id", STR );

	writer->WriteStartElement ( "path" );
	writer->WriteAttributeString ( "id", GML_ID_A );
	writer->WriteAttributeString ( "d", PATH_STRING );

	writer->WriteEndElement();
	writer->WriteEndElement();

	//  Pfade eventueller Löcher in einen defs-Block schreiben
	if ( pPoly->getLochAnz() > 0 )
	{
		writer->WriteStartElement ( "defs" );
	 
		for ( i = 0; i < pPoly->getLochAnz(); i++ )
		{
			STR_NUMBER = System::Convert::ToString ( i );

			pRing = pPoly->getLoch ( i );
				stdString = pRing->getPointsSVG ( swX, noY );
			PATH_STRING = QuConvert::ToString ( stdString );

			writer->WriteStartElement ( "path" );
			STR = String::Concat ( GML_ID_A, "_hole_", STR_NUMBER );
			writer->WriteAttributeString ( "id", STR );
			writer->WriteAttributeString ( "d", PATH_STRING );
			writer->WriteEndElement();
		}
		writer->WriteEndElement();

		if ( useMask )
		{
			writer->WriteStartElement ( "mask" );
			writer->WriteAttributeString ( "maskUnits", "userSpaceOnUse" );
			
			STR = String::Concat ( GML_ID_A, "_mask" );
			writer->WriteAttributeString ( "id", STR );

			pPlanObjektP->getBoundingBox ( sw, no, srsName, srsDimension, false );
			writer->WriteAttributeString ( "x", XmlConvert::ToString ( sw[0] - swX ));
			writer->WriteAttributeString ( "y", XmlConvert::ToString ( noY - no[1] ));
			writer->WriteAttributeString ( "width", XmlConvert::ToString ( no[0]- sw[0] ));
			writer->WriteAttributeString ( "height", XmlConvert::ToString ( no[1]- sw[1] ));

			writer->WriteStartElement ( "rect" );
			writer->WriteAttributeString ( "fill", "white" );
			writer->WriteAttributeString ( "x", XmlConvert::ToString ( sw[0] - swX ));
			writer->WriteAttributeString ( "y", XmlConvert::ToString ( noY - no[1] ));
			writer->WriteAttributeString ( "width", XmlConvert::ToString ( no[0]- sw[0] ));
			writer->WriteAttributeString ( "height", XmlConvert::ToString ( no[1]- sw[1] ));
			writer->WriteEndElement();

			for ( i = 0; i < pPoly->getLochAnz(); i++ )
			{
				STR_NUMBER = System::Convert::ToString ( i );
				STR = String::Concat ( "#", GML_ID_A, "_hole_", STR_NUMBER );	
				writer->WriteStartElement ( "use" );
				writer->WriteAttributeString ( "xlink:href", STR );
				writer->WriteAttributeString ( "fill", "black" );
				writer->WriteEndElement();
			}
			writer->WriteEndElement();
		}		
	} 

	if ( pPlanObjektP->getObjektTyp() == BLEIT_PLAN )
	{
		writer->WriteStartElement ( "mask" );
		writer->WriteAttributeString ( "maskUnits", "userSpaceOnUse" );
			
		STR = String::Concat ( GML_ID_A, "_mask" );
		writer->WriteAttributeString ( "id", STR );

		pPlanObjektP->getBoundingBox ( sw, no, srsName, srsDimension, false );
		writer->WriteAttributeString ( "x", XmlConvert::ToString ( sw[0] - swX - 2.0 ));
		writer->WriteAttributeString ( "y", XmlConvert::ToString ( noY - no[1] - 2.0 ));
		writer->WriteAttributeString ( "width", XmlConvert::ToString ( no[0]- sw[0] + 4.0 ));
		writer->WriteAttributeString ( "height", XmlConvert::ToString ( no[1]- sw[1] + 4.0 ));

		writer->WriteStartElement ( "rect" );
		writer->WriteAttributeString ( "fill", "white" );
		writer->WriteAttributeString ( "x", XmlConvert::ToString ( sw[0] - swX - 2.0 ));
		writer->WriteAttributeString ( "y", XmlConvert::ToString ( noY - no[1] - 2.0 ));
		writer->WriteAttributeString ( "width", XmlConvert::ToString ( no[0]- sw[0] + 4.0 ));
		writer->WriteAttributeString ( "height", XmlConvert::ToString ( no[1]- sw[1] + 4.0 ));
		writer->WriteEndElement();

		STR = String::Concat ( "#", GML_ID_A );	
		writer->WriteStartElement ( "use" );
		writer->WriteAttributeString ( "xlink:href", STR );
		writer->WriteAttributeString ( "fill", "black" );

		writer->WriteEndElement();
		writer->WriteEndElement();
	}

	anz = pFlaechenDarstAct->getFlaechenParams ( vFlaechenParams );
	for ( i = 0; i < anz; i++ )
	{
		pFlaechenParams = vFlaechenParams[i];

		classId    = pFlaechenParams->getClassId();
		patternId  = pFlaechenParams->getPatternId();
		hatchingId = pFlaechenParams->getHatchingId();
		clip	   = pFlaechenParams->getClip();

		writer->WriteStartElement ( "use" );


		STR = String::Concat ( "#", GML_ID_A );
		writer->WriteAttributeString ( "xlink:href", STR );

    if ( classId != "" )
    {
		  CLASS_ID = QuConvert::ToString ( classId );
		  writer->WriteAttributeString ( "class", CLASS_ID );
    }

		if ( m_interactiveMap )
		{
			STR = String::Concat ( GML_ID_A, "_A", XmlConvert::ToString ( i ) );
			writer->WriteAttributeString ( "id", STR );
			writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
      if ( patternId != "" || hatchingId != "" )
      {
			  writer->WriteAttributeString ( "onmouseover", "top.karteInfoHatch(evt)" );
			  writer->WriteAttributeString ( "onmouseout", "top.karteOutHatch(evt)" );
      }
      else
      {
			  writer->WriteAttributeString ( "onmouseover", "top.karteInfo(evt)" );
			  writer->WriteAttributeString ( "onmouseout", "top.karteOut(evt)" );
      }
		}

		STYLE_STRING = "";
		if ( hatchingId != "" )
			STYLE_STRING = String::Concat ( "fill: url(#", QuConvert::ToString ( hatchingId ), "); " );
		else
		if ( patternId != "" )
			STYLE_STRING = String::Concat ( "fill: url(#", QuConvert::ToString ( patternId ), "); " );

		if ( clip )
			STYLE_STRING = String::Concat ( STYLE_STRING, "clip-path:url(#", GML_ID_A, "_clip);" );

		if ( String::Compare ( STYLE_STRING, "" ) != 0 )
			writer->WriteAttributeString ( "style", STYLE_STRING );

		if ( useMask  && pPoly->getLochAnz() > 0 )
		{
			STR = String::Concat ( "url(#", GML_ID_A, "_mask)" );
			writer->WriteAttributeString ( "mask", STR );
		}

		if ( pPlanObjektP->getObjektTyp() == BLEIT_PLAN )
		{
			STR = String::Concat ( "url(#", GML_ID_A, "_mask)" );
			writer->WriteAttributeString ( "mask", STR );
		}

		writer->WriteEndElement();

		if ( !useMask )
		{
			for ( j = 0; j < pPoly->getLochAnz(); j++ )
			{
				STR_NUMBER = System::Convert::ToString ( j );

				writer->WriteStartElement ( "use" );
				STR = String::Concat ( "#", GML_ID_A, "_hole_", STR_NUMBER );
				writer->WriteAttributeString ( "xlink:href", STR );

        if ( classId != "" )
				  writer->WriteAttributeString ( "class", CLASS_ID );
				writer->WriteEndElement();
			}
		}

	}
	LineTextParams * pLineTextParams = pFlaechenDarstAct->getLineTextParams();
	if ( pLineTextParams != NULL ) 
		writeLineText ( pPlanObjektP, pLineTextParams );

	LineSymbolParams * pLineSymbolParams = pFlaechenDarstAct->getLineSymbolParams();
	if ( pLineSymbolParams != NULL ) 
		writeLineSymbol ( pPlanObjektP, pLineSymbolParams );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer Flächen-Darstellung des Objektes				            		 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeFlaechenDarstellung ( Feature * pPlanObjektP, FlaechenDarstellung * pFlaechenDarst )
{
	size_t                   i, j, anz, iPoly, polyAnz;
	vector<FlaechenParams*>  vFlaechenParams;
	vector<PolygonGeo*>	 	   polygone;
	FlaechenParams		     * pFlaechenParams;
	string                   symbolAttribut;
	string                   symbolAttributWert = "";
	string                   textAttribut;
	string			      			 textAttributWert;
	string						       errorText;
	string						       classId;
	string						       gmlId;
	string                   klassenName;
	string						       patternId;
	string                   hatchingId;
	string						       stdString;
	bool						         clip, useMask;
	double						       no[3], sw[3];

	String					  ^ STR;
	String					  ^ GML_ID;
	String            ^ GML_ID_A;
	String					  ^	PATH_STRING;
	String            ^ STR_NUMBER;
	String					  ^ STYLE_STRING;
	String					  ^ CLASS_ID;
	PolygonGeo					  * pPoly;
	_Ring					    * pRing;
  string              srsName;
  int                 srsDimension;
	
  writer->Flush();

	polyAnz = pPlanObjektP->getFlaechenGeometrie( "", polygone );

	gmlId = pPlanObjektP->getGmlId();
	GML_ID = QuConvert::ToString ( gmlId );

	if ( polyAnz == 0 )
	{
			anzError++;
			errorText = "Ungültige Flächen-Geometrie für \n GML-Klasse ";
			errorText += pPlanObjektP->getKlassenName();
			m_pFeatures->getLogfile()->writeLogFile ( errorText );
			return;
	}

	if ( m_interactiveMap )	useMask = true;
	else
	{
		if ( pPlanObjektP->getFlaechenschluss() == false && pFlaechenDarst->getDeckend() == true )
			useMask = true;
		else
			useMask = false;
	}

	for ( iPoly = 0; iPoly < polyAnz; iPoly++ )
	{
  	pPoly = polygone[iPoly];
	  pRing = pPoly->getAussenkontur();

	  stdString = pRing->getPointsReverseSVG ( swX, noY );
	  PATH_STRING = QuConvert::ToString ( stdString );
	  if ( iPoly > 99 ) break;

	  STR =  XmlConvert::ToString ( iPoly );
	  if ( STR->Length == 1 )
		  STR = String::Concat ( "0", STR );

	  GML_ID_A = String::Concat ( GML_ID, STR );

	  STR = String::Concat ( GML_ID_A, "_clip" );
	  writer->WriteStartElement ( "clipPath" );
	  writer->WriteAttributeString ( "id", STR );

	  writer->WriteStartElement ( "path" );
	  writer->WriteAttributeString ( "id", GML_ID_A );
	  writer->WriteAttributeString ( "d", PATH_STRING );

	  writer->WriteEndElement();
	  writer->WriteEndElement();

	  //  Pfade eventueller Löcher in einen defs-Block schreiben
	  if ( pPoly->getLochAnz() > 0 )
	  {
		  writer->WriteStartElement ( "defs" );
	 
		  for ( i = 0; i < pPoly->getLochAnz(); i++ )
		  {
			  STR_NUMBER = System::Convert::ToString ( i );

			  pRing = pPoly->getLoch ( i );
				  stdString = pRing->getPointsSVG ( swX, noY );
			  PATH_STRING = QuConvert::ToString ( stdString );

			  writer->WriteStartElement ( "path" );
			  STR = String::Concat ( GML_ID_A, "_hole_", STR_NUMBER );
			  writer->WriteAttributeString ( "id", STR );
			  writer->WriteAttributeString ( "d", PATH_STRING );
			  writer->WriteEndElement();
		  }
		  writer->WriteEndElement();

		  if ( useMask )
		  {
			  writer->WriteStartElement ( "mask" );
			  writer->WriteAttributeString ( "maskUnits", "userSpaceOnUse" );
			
			  STR = String::Concat ( GML_ID_A, "_mask" );
			  writer->WriteAttributeString ( "id", STR );

			  pPlanObjektP->getBoundingBox ( sw, no, srsName, srsDimension, false );
			  writer->WriteAttributeString ( "x", XmlConvert::ToString ( sw[0] - swX ));
			  writer->WriteAttributeString ( "y", XmlConvert::ToString ( noY - no[1] ));
			  writer->WriteAttributeString ( "width", XmlConvert::ToString ( no[0]- sw[0] ));
			  writer->WriteAttributeString ( "height", XmlConvert::ToString ( no[1]- sw[1] ));

			  writer->WriteStartElement ( "rect" );
			  writer->WriteAttributeString ( "fill", "white" );
			  writer->WriteAttributeString ( "x", XmlConvert::ToString ( sw[0] - swX ));
			  writer->WriteAttributeString ( "y", XmlConvert::ToString ( noY - no[1] ));
			  writer->WriteAttributeString ( "width", XmlConvert::ToString ( no[0]- sw[0] ));
			  writer->WriteAttributeString ( "height", XmlConvert::ToString ( no[1]- sw[1] ));
			  writer->WriteEndElement();

			  for ( i = 0; i < pPoly->getLochAnz(); i++ )
			  {
  				STR_NUMBER = System::Convert::ToString ( i );
	  			STR = String::Concat ( "#", GML_ID_A, "_hole_", STR_NUMBER );	
		  		writer->WriteStartElement ( "use" );
			  	writer->WriteAttributeString ( "xlink:href", STR );
				  writer->WriteAttributeString ( "fill", "black" );
				  writer->WriteEndElement();
			  }
			  writer->WriteEndElement();
		  }		
	  } 	

	  anz = pFlaechenDarst->getFlaechenParams ( vFlaechenParams );
	  for ( i = 0; i < anz; i++ )
	  {
		  pFlaechenParams = vFlaechenParams[i];

		  classId    = pFlaechenParams->getClassId();
		  patternId  = pFlaechenParams->getPatternId();
		  hatchingId = pFlaechenParams->getHatchingId();
		  clip	   = pFlaechenParams->getClip();

		  writer->WriteStartElement ( "use" );


  		STR = String::Concat ( "#", GML_ID_A );
	  	writer->WriteAttributeString ( "xlink:href", STR );

      if ( classId != "" )
      {
		    CLASS_ID = QuConvert::ToString ( classId );
		    writer->WriteAttributeString ( "class", CLASS_ID );
      }

		  if ( m_interactiveMap )
		  {
			  STR = String::Concat ( GML_ID_A, "_A", XmlConvert::ToString ( i ) );
			  writer->WriteAttributeString ( "id", STR );
			  writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
        if ( patternId != "" || hatchingId != "" )
        {
			    writer->WriteAttributeString ( "onmouseover", "top.karteInfoHatch(evt)" );
			    writer->WriteAttributeString ( "onmouseout", "top.karteOutHatch(evt)" );
        }
        else
        {
  			  writer->WriteAttributeString ( "onmouseover", "top.karteInfo(evt)" );
	  		  writer->WriteAttributeString ( "onmouseout", "top.karteOut(evt)" );
        }
		  }

		  STYLE_STRING = "";
		  if ( hatchingId != "" )
			  STYLE_STRING = String::Concat ( "fill: url(#", QuConvert::ToString ( hatchingId ), "); " );
		  else
		  if ( patternId != "" )
			  STYLE_STRING = String::Concat ( "fill: url(#", QuConvert::ToString ( patternId ), "); " );

		  if ( clip )
			  STYLE_STRING = String::Concat ( STYLE_STRING, "clip-path:url(#", GML_ID_A, "_clip);" );

		  if ( String::Compare ( STYLE_STRING, "" ) != 0 )
			  writer->WriteAttributeString ( "style", STYLE_STRING );

		  if ( useMask  && pPoly->getLochAnz() > 0 )
		  {
			  STR = String::Concat ( "url(#", GML_ID_A, "_mask)" );
			  writer->WriteAttributeString ( "mask", STR );
		  }

		  if ( pPlanObjektP->getObjektTyp() == BLEIT_PLAN )
		  {
			  STR = String::Concat ( "url(#", GML_ID_A, "_mask)" );
			  writer->WriteAttributeString ( "mask", STR );
		  }

  		writer->WriteEndElement();

	  	if ( !useMask )
		  {
  			for ( j = 0; j < pPoly->getLochAnz(); j++ )
	  		{
		  		STR_NUMBER = System::Convert::ToString ( j );

			  	writer->WriteStartElement ( "use" );
  				STR = String::Concat ( "#", GML_ID_A, "_hole_", STR_NUMBER );
	  			writer->WriteAttributeString ( "xlink:href", STR );

          if ( classId != "" )
		  		  writer->WriteAttributeString ( "class", CLASS_ID );
			  	writer->WriteEndElement();
			  }
		  }
  	}
	  LineTextParams * pLineTextParams = pFlaechenDarst->getLineTextParams();
	  if ( pLineTextParams != NULL ) 
		  writeLineText ( pPlanObjektP, pLineTextParams );

	  LineSymbolParams * pLineSymbolParams = pFlaechenDarst->getLineSymbolParams();
	  if ( pLineSymbolParams != NULL ) 
		  writeLineSymbol ( pPlanObjektP, pLineSymbolParams );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Scheiben eines  Präsentationsobjektes XP_FPO                             //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeFPO ( Feature * pPlanObjektP )
{
	vector<FeatureRelation*> objRelationen;
	size_t      						 anz;

  anz = pPlanObjektP->getFeatureRelationen ( "xplan:dientZurDarstellungVon", objRelationen );
	if ( anz == 0 )
	{
		writeFPOFrei ( pPlanObjektP );
		return;
	}

	m_pFeatures->getLogfile()->writeLogFile ( "!!! Flächenhafte Präsentationsobjekte mit Referenz auf Fachobjekte sind nicht zulässig" );
}


///////////////////////////////////////////////////////////////////////////////
//  Scheiben eines freien Präsentationsobjektes XP_FPO                       //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeFPOFrei ( Feature * pPlanObjektP )
{
  string    idFlaechendarstellung;
	string    errorText;
	bool	  success;
  FlaechenDarstellung * pFlaechenDarstellung;

	success = pPlanObjektP->getStringAttributWert ( "xplan:stylesheetId", idFlaechendarstellung );
  if ( !success )
  {
		anzError++;
		errorText = "Ungültiger Flächen-Style in XP_FPO ";
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
  }

  pFlaechenDarstellung = m_pSvgParams->getFlaechenDarstellung ( idFlaechendarstellung );
  if ( pFlaechenDarstellung == NULL )
  {
		anzError++;
		errorText = "Ungültiger Flächen-Style in XP_FPO ";
		m_pFeatures->getLogfile()->writeLogFile ( errorText );
		return;
  }

  writeFlaechenDarstellung ( pPlanObjektP, pFlaechenDarstellung );	
}


///////////////////////////////////////////////////////////////////////////////
//  Generiert eine Randsignatur durch einen svg-Textpfad		          			 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeLineText (  Feature * pPlanObjektP, LineTextParams * pLineTextParamsP )
{
	string        id;
	string        classId;
	string        textString;
	string        stdString;
	__wchar_t     textChar;
	double        charSize, length;
	size_t        charAnz, i, j, k, polyAnz, curveAnz;
	bool          isWideChar;
	String      ^ STR;
	String      ^ STR_Number;
	String      ^ GML_ID;
	String      ^ GML_ID_A;
	String      ^ TEXT_STRING;

	id         = pPlanObjektP->getGmlId();
	GML_ID     = QuConvert::ToString ( id );
	
	vector<PolygonGeo*> polygone;
	vector<_Curve*>  curves;

	classId    = pLineTextParamsP->getClassId();
	charSize   = pLineTextParamsP->getCharSize();
	isWideChar = pLineTextParamsP->getIsWideChar();

	if ( isWideChar )
	{
		textChar = pLineTextParamsP->getTextChar();
		TEXT_STRING = Convert::ToString ( __wchar_t (textChar) );
	}
	else
	{
		textString = pLineTextParamsP->getTextString();
		TEXT_STRING = QuConvert::ToString ( textString );
	}

	if ( pPlanObjektP->getGeometrieTyp() == MULTI_SURFACE     ||
       pPlanObjektP->getGeometrieTyp() == SURFACE           ||
       pPlanObjektP->getGeometrieTyp() == POLYGON           ||
       pPlanObjektP->getGeometrieTyp() == SHELL             ||
       pPlanObjektP->getGeometrieTyp() == COMPOSITE_SURFACE )
	{
		polyAnz = pPlanObjektP->getFlaechenGeometrie( "", polygone );
		for ( k = 0; k < polyAnz; k++ )
		{
			if ( k > 99 ) break;

			PolygonGeo * pPolygon = polygone[k];
			length = pPolygon->getAussenkontur()->getUmfang();
			charAnz = int ( length / charSize );

			if ( charAnz > 100000 )
				charAnz = 100000;

			writer->WriteStartElement ( "text" );
			writer->WriteAttributeString ( "class", QuConvert::ToString ( classId ) );

			STR =  XmlConvert::ToString ( k );
			if ( STR->Length == 1 )
				STR = String::Concat ( "0", STR );

			GML_ID_A = String::Concat ( "#", GML_ID, STR );

			writer->WriteStartElement ( "textPath" );
			writer->WriteAttributeString ( "xlink:href", GML_ID_A );

			STR = TEXT_STRING;
			for ( i = 1; i < charAnz; i++ ) STR = String::Concat ( STR, TEXT_STRING );
			writer->WriteString ( STR );

			writer->WriteEndElement();
			writer->WriteEndElement();

			for ( i = 0; i < pPolygon->getLochAnz(); i++ )
			{

				length = pPolygon->getLoch ( i )->getUmfang();
				charAnz = int ( length / charSize );

				if ( charAnz > 100000 )
					charAnz = 100000;

				STR_Number = System::Convert::ToString ( i );
				STR = String::Concat ( GML_ID_A, "_hole_", STR_Number );	

				writer->WriteStartElement ( "text" );
				writer->WriteAttributeString ( "class", QuConvert::ToString ( classId ) );
				writer->WriteStartElement ( "textPath" );
				writer->WriteAttributeString ( "xlink:href", STR );

				STR = TEXT_STRING;
				for ( j = 1; j < charAnz; j++ ) STR = String::Concat ( STR, TEXT_STRING );
				writer->WriteString ( STR );
		
				writer->WriteEndElement();
				writer->WriteEndElement();
			}
		}
	}
	else
	if ( pPlanObjektP->getGeometrieTyp() == MULTI_CURVE ||
       pPlanObjektP->getGeometrieTyp() == CURVE       ||
       pPlanObjektP->getGeometrieTyp() == LINE_STRING ||
       pPlanObjektP->getGeometrieTyp() == ARC         ||
       pPlanObjektP->getGeometrieTyp() == CIRCLE )
	{
		curveAnz = pPlanObjektP->getLinienGeometrie( "", curves );
		for ( k = 0; k < curveAnz; k++ )
		{
			_Curve * pCurve = curves[k];
			if ( k > 99 ) break;

			STR =  XmlConvert::ToString ( k );
			if ( STR->Length == 1 )
				STR = String::Concat ( "0", STR );

			GML_ID_A = String::Concat ( GML_ID, STR );

			length = pCurve->getLength();
			charAnz = int ( length / charSize );

			writer->WriteStartElement ( "text" );
			writer->WriteAttributeString ( "class", QuConvert::ToString ( classId ) );

			STR = String::Concat ( "#", GML_ID_A );
			writer->WriteStartElement ( "textPath" );
			writer->WriteAttributeString ( "xlink:href", STR );

			STR = TEXT_STRING;
			for ( i = 1; i < charAnz; i++ ) STR = String::Concat ( STR, TEXT_STRING );
			writer->WriteString ( STR );

			writer->WriteEndElement();
			writer->WriteEndElement();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine Randsignatur durch svg-Symbole		              					 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeLineSymbol (  Feature * pPlanObjektP, LineSymbolParams * pLineSymbolParamsP )
{
	size_t       iLoch, anz, k, polyAnz, curveAnz;
	string       symbolId    = pLineSymbolParamsP->getSymbolId();
	double       symbolDist  = pLineSymbolParamsP->getSymbolDist();
	double       symbolSize  = pLineSymbolParamsP->getSymbolSize();
	double       angleOffset = pLineSymbolParamsP->getSymbolAngle();

	vector<double> vX;
	vector<double> vY;
	vector<double> vAngle;
	PolygonGeo   * pPoly;
	_Ring        * pRing;
	_Curve       * pCurve;
	string         gmlId;

	vector<PolygonGeo*> polygone;
	vector<_Curve*>     curves;

	gmlId = pPlanObjektP->getGmlId();

  if ( pPlanObjektP->getGeometrieTyp() == MULTI_SURFACE      ||
       pPlanObjektP->getGeometrieTyp() == SURFACE            ||
       pPlanObjektP->getGeometrieTyp() == POLYGON            ||
       pPlanObjektP->getGeometrieTyp() == SHELL              ||
       pPlanObjektP->getGeometrieTyp() == COMPOSITE_SURFACE )
	{
		polyAnz = pPlanObjektP->getFlaechenGeometrie( "", polygone );
		for ( k = 0; k < polyAnz; k++ )
		{
			pPoly = polygone[k];
			pRing = pPoly->getAussenkontur();
			anz = pRing->scanRing2D ( vX, vY, vAngle, symbolDist, false );
			writeSymbols ( gmlId, vX, vY, vAngle, symbolId, symbolSize, angleOffset );

			for ( iLoch = 0; iLoch < pPoly->getLochAnz(); iLoch++ )
			{
				vX.clear();
				vY.clear();
				vAngle.clear();

				pRing = pPoly->getLoch ( iLoch );
				anz = pRing->scanRing2D ( vX, vY, vAngle, symbolDist, true );
				writeSymbols ( gmlId, vX, vY, vAngle, symbolId, symbolSize, angleOffset );
			}
		}
	}
	else
    if ( pPlanObjektP->getGeometrieTyp() == MULTI_CURVE         ||
         pPlanObjektP->getGeometrieTyp() == CURVE               ||
         pPlanObjektP->getGeometrieTyp() == LINE_STRING         ||
			   pPlanObjektP->getGeometrieTyp() == LINE_STRING_SEGMENT ||
         pPlanObjektP->getGeometrieTyp() == ARC                 ||
         pPlanObjektP->getGeometrieTyp() == CIRCLE )
	{
		curveAnz = pPlanObjektP->getLinienGeometrie( "", curves );
		for ( k = 0; k < curveAnz; k++ )
		{
			pCurve = curves[k];
			if ( k > 99 ) break;

			anz = pCurve->scanCurve2D ( vX, vY, vAngle, symbolDist, false );
			writeSymbols ( gmlId, vX, vY, vAngle, symbolId, symbolSize, angleOffset );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert Randsignatur-Symbole					                    						 //
///////////////////////////////////////////////////////////////////////////////
void SvgWriter::writeSymbols ( string gmlId, vector<double> vX, vector<double> vY, vector<double> vAngle, 
							   string symbolId, double symbolSize, double angleOffset )
{
	double    x, y, angle;
	size_t    i,anz;
	String  ^ STR;
	String  ^ GML_ID = QuConvert::ToString ( gmlId );

	GML_ID = String::Concat ( GML_ID, "00_A0" );

	anz = vX.size();
	for ( i = 0; i < anz; i++ )
	{
		x     = vX[i] - swX;
		y     = noY - vY[i];
		angle = angleOffset -(vAngle[i] * 180.0 ) / PI;

		writer->WriteStartElement ( "use" );

		STR = String::Concat ( "#", QuConvert::ToString ( symbolId) );
		writer->WriteAttributeString ( "xlink:href", STR );

		if ( m_interactiveMap )
		{
			writer->WriteAttributeString ( "id", GML_ID );

			writer->WriteAttributeString ( "onclick", "top.objInfo(evt)" );
			writer->WriteAttributeString ( "onmouseover", "karteInfo(evt)" );
			writer->WriteAttributeString ( "onmouseout", "karteOut(evt)" );
		}

		STR = String::Concat ( "translate(", XmlConvert::ToString ( x ), "," );
		STR = String::Concat ( STR, XmlConvert::ToString ( y ), ") " );
		STR = String::Concat ( STR, "rotate(", XmlConvert::ToString ( angle ), ") " );
		STR = String::Concat ( STR, "scale(", XmlConvert::ToString ( symbolSize ), ")" );
		writer->WriteAttributeString ( "transform", STR );

		writer->WriteEndElement();
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt vor GML Klassen- bzw. Attributname das Default Namespace-Kürzel ein //
///////////////////////////////////////////////////////////////////////////////
string SvgWriter::addDefaultNamespaceAttribut ( string gmlName )
{
	size_t index;
  string gmlNameAct;

  index = gmlName.find ( ":" );
  if ( index != string::npos )
    return gmlName;

  gmlNameAct = "xplan:" + gmlName;
  return gmlNameAct;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt vor GML Klassen- bzw. Attributname das Default Namespace-Kürzel //
///////////////////////////////////////////////////////////////////////////////
string SvgWriter::removeDefaultNamespaceAttribut ( string gmlName )
{
	size_t index;
  string kuerzel;

  index = gmlName.find ( ":" );
  if ( index != string::npos )
  {
    kuerzel = gmlName.substr( 0, index );
    if ( kuerzel == "xplan" )
      gmlName = gmlName.substr ( index+1, gmlName.length() );
  }

  return gmlName;
}

///////////////////////////////////////////////////////////////////////////////
//  class PlanzListe					                            									 //
//  Liste aller Signaturen, die in Form von svg-Planzeichen in einer html-   //
//  Tabelle dokumentiert werden sollen	                  									 //
///////////////////////////////////////////////////////////////////////////////
PlanzListe::PlanzListe(  Visualisierung_2D * pVisualisierung )
{
  m_pVisualisierung = pVisualisierung;
  m_pPlanObjekte = m_pVisualisierung->getFeatures();
  gmlTyp  = XPLANGML_5_1;
	m_pVisualisierung->setGlobalScale ( 1.0 );
  m_pSvgParams = NULL;

	ausgabePfad = "";
	pngFolder   = "png\\";
	svgFolder	= "svg\\";
	indexDateiName = "PlanzListe.html";

	pPlanFlaechenObjekt = NULL;
	pPlanLinienObjekt	= NULL;
	pPlanPunktObjekt	= NULL;
	pPPOObjekt			= NULL;

	sizeX = 20;
	sizeY = 10;
	pixelX = 80;
	pixelY = 40;
}

PlanzListe::~PlanzListe()
{
	unsigned int i;
	
	for ( i = 0; i < vPlanzListe.size(); i++ )
		delete vPlanzListe[i];
};


///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Planzeichen-Darstellung zur Liste hinzu                  			 //
///////////////////////////////////////////////////////////////////////////////
void PlanzListe::addPlanz ( Planz * pPlanz )
{
	vPlanzListe.push_back ( pPlanz );
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die Planzeichen-Liste                   											 //
///////////////////////////////////////////////////////////////////////////////
void PlanzListe::createPlanzeichen ( std::string planzListePfadP )
{
	Planz      * pPlanz;
	String     ^ STR;
	string       svgDateien;
	string       pngDateien;
	unsigned int i;
  bool         success;
  GmlSchema * pGmlObjekte;

	PlanzReader ^ pPlanzReader = gcnew PlanzReader ( this );
	pPlanzReader->read ( planzListePfadP );

  pGmlObjekte = new GmlSchema ( m_pPlanObjekte );
  success = pGmlObjekte->readGmlSchema ( gmlTyp, NULL );
  if ( success )
    m_pPlanObjekte->setGmlSchema( pGmlObjekte );
  else
  {
    delete pGmlObjekte;
    pGmlObjekte = NULL;
  }

  m_pSvgParams = m_pVisualisierung->getSvgParams();

	svgPath        = ausgabePfad + svgFolder;
	pngPath        = ausgabePfad + pngFolder;
	indexDateiPath = ausgabePfad + indexDateiName;

	STR = QuConvert::ToString ( svgPath );
	if ( Directory::Exists ( STR ) )
		Directory::Delete ( STR, true );
	Directory::CreateDirectory ( STR );

	STR = QuConvert::ToString ( pngPath );
	if ( Directory::Exists ( STR ) )
		Directory::Delete ( STR, true );
	Directory::CreateDirectory ( STR );

	writeIndexDateiHeader();

	for ( i = 0; i < vPlanzListe.size(); i++ )
	{
		pPlanz = vPlanzListe[i];
		pPlanz->createPlanzeichen();
	}

	writeIndexDateiEnd();

	svgDateien = svgPath + "*.svg";
	pngDateien = pngPath.substr ( 0, pngPath.size()-1 );
	convertSvgPng ( svgDateien, pngDateien );
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt Text auf Index-Datei					                      						 //
///////////////////////////////////////////////////////////////////////////////
 void PlanzListe::writeIndexDatei ( std::string textP )
{
	indexDatei << textP << endl;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt Haeder der Index-Datei				                    							 //
///////////////////////////////////////////////////////////////////////////////
void PlanzListe::writeIndexDateiHeader()
{
	if ( indexDatei.is_open() ) indexDatei.close();
	indexDatei.open ( indexDateiPath.c_str() );

	indexDatei << "<html>" << endl;

	indexDatei << "<style type=\"text/css\">" << endl << endl;
	indexDatei << "<!--" << endl;
	indexDatei << "h1 {font-family: Arial, Helvetica, sans-serif; text-align: center;}" << endl;
	indexDatei << "h2 {font-family: Arial, Helvetica, sans-serif; text-align: center; font-size: medium; margin: 0px; background-color: #FFFF00;}" << endl;
	indexDatei << "h3 {font-family: Arial, Helvetica, sans-serif; font-size: small; margin: 0px;}" << endl;
	indexDatei << "img {border: none;}" << endl;
	indexDatei << "td {font-family: Arial, Helvetica, sans-serif;text-indent: 4px;}" << endl;
	indexDatei << "-->" << endl;
	indexDatei << "</style>" << endl;

	indexDatei << "<title>Planzeichen-Liste</title><body>" << endl;
	indexDatei << "<body>" << endl;

	indexDatei << "<h1>" << "Planzeichen-Liste" << "</h1>" << endl;	
	
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt des Schluß der Index-Datei                 										 //
///////////////////////////////////////////////////////////////////////////////
void PlanzListe::writeIndexDateiEnd()
{
	indexDatei << "</body>" << endl;
	indexDatei << "</html>" << endl;

	indexDatei.flush();
	indexDatei.close();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die geographische Größe eines Planzeichens           						 //
///////////////////////////////////////////////////////////////////////////////
void PlanzListe::getSize ( double & x, double & y )
{
	x = sizeX;
	y = sizeY;
}

///////////////////////////////////////////////////////////////////////////////
//  Die die Pixelauflösung der Rasterdarstellung eines Planzeichens          //
///////////////////////////////////////////////////////////////////////////////
void PlanzListe::getPixel ( int & x, int & y )
{
	x = pixelX;
	y = pixelY;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die Hilfs-Objekte zur Erzeugung der Planzeichen-Liste 		     //
///////////////////////////////////////////////////////////////////////////////
void PlanzListe::createGeometrieObjekte()
{
	createGeometrieObjekte ( sizeX, sizeY );
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die Hilfs-Objekte zur Erzeugung der Planzeichen-Liste 		     //
///////////////////////////////////////////////////////////////////////////////
void PlanzListe::createGeometrieObjekte( double sizeXP, double sizeYP )
{
	double         xMin, xMax, yMin, yMax;
	vector<double> xCoords;
	vector<double> yCoords;

	xMin = -0.5*sizeXP;
	xMax =  0.5*sizeXP;
	yMin = -0.5*sizeYP;
	yMax =  0.5*sizeYP;

	if ( pPlanFlaechenObjekt != NULL) delete pPlanFlaechenObjekt;
	if ( pPlanLinienObjekt != NULL)   delete pPlanLinienObjekt;
	if ( pPlanPunktObjekt != NULL)    delete pPlanPunktObjekt;
	if ( pPPOObjekt != NULL)		  delete pPPOObjekt;

	xCoords.push_back ( xMin );
	yCoords.push_back ( yMin );
	
	xCoords.push_back ( xMax );
	yCoords.push_back ( yMin );
	
	xCoords.push_back ( xMax );
	yCoords.push_back ( yMax );
	
	xCoords.push_back ( xMin );
	yCoords.push_back ( yMax );
	
	xCoords.push_back ( xMin );
	yCoords.push_back ( yMin );

	LinearRing  * pRing  = new LinearRing ( 2 );
	pRing->addPoints2D ( xCoords, yCoords );
	PolygonGeo * pPoly = new PolygonGeo ( 2 );
	pPoly->setAussenkontur ( pRing );
	MultiSurface * pFlaechenGeometrie = new MultiSurface ( 2 );
	pFlaechenGeometrie->addSurface ( pPoly );

	pPlanFlaechenObjekt = new Feature ( ANY_OBJEKT, "flaeche", m_pPlanObjekte, "" );
  pPlanFlaechenObjekt->addGeometryProperty ( "xplan:position", pFlaechenGeometrie );
	
	m_pPlanObjekte->addFeature ( pPlanFlaechenObjekt );

	//  Linienobjekt
	xCoords.clear();
	yCoords.clear();

	xCoords.push_back ( xMin );
	yCoords.push_back ( 0.0 );
	
	xCoords.push_back ( xMax );
	yCoords.push_back ( 0.0 );

	LineSegment * pLineSegment = new LineSegment( 2, LineSegment::_LINEAR_RING );
	pLineSegment->addPoints2D ( xCoords, yCoords );
	MultiCurve * pLinienGeometrie = new MultiCurve( 2 );
	pLinienGeometrie->addCurve ( pLineSegment );

  pPlanLinienObjekt = new Feature ( ANY_OBJEKT, "linie", m_pPlanObjekte, "" );
  pPlanLinienObjekt->addGeometryProperty ( "xplan:position", pLinienGeometrie );
  m_pPlanObjekte->addFeature ( pPlanLinienObjekt );

	//  Punktobjekte
	GeoPoint * pPoint = new GeoPoint ( 0, 0 );
	MultiPoint * pPunktGeometrie = new MultiPoint ( 2 );
	pPunktGeometrie->addPoint ( pPoint );

  pPlanPunktObjekt = new Feature ( ANY_OBJEKT, "punkt", m_pPlanObjekte, "" );
	pPlanPunktObjekt->addGeometryProperty ( "xplan:position", pPunktGeometrie );
  m_pPlanObjekte->addFeature ( pPlanPunktObjekt );

	//  Präsentationsobjekt
	pPoint = new GeoPoint ( 0, 0 );
	pPunktGeometrie = new MultiPoint ( 2 );
	pPunktGeometrie->addPoint ( pPoint );
  pPPOObjekt = new Feature ( XP_PPO, "xplan:PPO", m_pPlanObjekte, "" );
	pPPOObjekt->addGeometryProperty ( "xplan:position", pPunktGeometrie );
  m_pPlanObjekte->addFeature ( pPPOObjekt );

	FeatureRelation * pRel = new FeatureRelation;
  pRel->rollenName = "xplan:dientZurDarstellungVon";
	pRel->featureId = "flaeche";

	pPPOObjekt->addFeatureRelation ( pRel );
  pPPOObjekt->setStringAttribut ( "xplan:stylesheetId", "TextDefault" );
  pPPOObjekt->setDoubleAttribut ( "xplan:drehwinkel", 0.0 );
  pPPOObjekt->setDoubleAttribut ( "xplan:skalierung", 1.0 );
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertiert eine SVG-Darstellung in eine Raster-Darstellungv						 //
///////////////////////////////////////////////////////////////////////////////
void PlanzListe::convertSvgPng ( std::string svgDateiPath, std::string pngDateiPath )
{
	System::Diagnostics::Process          ^ rasterizerProcess;
	System::Diagnostics::ProcessStartInfo ^ processStartInfo;

	String ^ STR;
	String ^ PARAMS;
	String ^ BATIK_PATH;
	String ^ OUTPUT_PATH;
	String ^ RASTER_EXTENSION;
	String ^ RASTER_RESOLUTION;
	String ^ OUTPUT_FORMAT;
	String ^ SVG_DATEI;

	string  stdString;

  PARAMS = String::Concat ( "-Xm", QuConvert::ToString ( javaHeapSize ), " " );

	BATIK_PATH  = String::Concat ( "-jar \"", QuConvert::ToString ( batikPath ), "\" " );

	OUTPUT_PATH = String::Concat ( "-d \"",   QuConvert::ToString ( pngDateiPath ), "\" " );

	SVG_DATEI   = String::Concat ( "\"",  QuConvert::ToString ( svgDateiPath ), "\"" );

	RASTER_RESOLUTION = "-dpi 50 ";

	RASTER_EXTENSION = String::Concat ( "-w ", XmlConvert::ToString ( pixelX ), " -h ",
										XmlConvert::ToString ( pixelY ), " " );

	OUTPUT_FORMAT = "-q 0.99 -m image/png ";

	STR = String::Concat ( PARAMS, BATIK_PATH, OUTPUT_PATH, RASTER_EXTENSION );
	STR = String::Concat ( STR, RASTER_RESOLUTION, OUTPUT_FORMAT, QuConvert::ToString ( svgDateiPath ) );

  try
  {
    rasterizerProcess = gcnew System::Diagnostics::Process();
    processStartInfo = gcnew System::Diagnostics::ProcessStartInfo();

    processStartInfo->FileName = QuConvert::ToString ( javaPath );
    processStartInfo->Arguments = STR;

    rasterizerProcess->StartInfo = processStartInfo;
    rasterizerProcess->Start();
  }

  catch ( Exception ^ )
  {
    MessageBox::Show ( "Java konnte nicht gestartet werden" );
  }

}
///////////////////////////////////////////////////////////////////////////////
//  class Planz															                                 //
//  Signaturen, die zu einer GML-Klasse gehören						              		 //
///////////////////////////////////////////////////////////////////////////////
Planz::Planz( PlanzListe * pPlanzListeP )
{
	pPlanzListe = pPlanzListeP;

	sizeX = -1;
	sizeY = -1;
}

Planz::~ Planz()
{
	unsigned int i;

	for ( i = 0; i < vFlaechenDarst.size(); i++ )
		delete vFlaechenDarst[i];

	for ( i = 0; i < vLinienDarst.size(); i++ )
		delete vLinienDarst[i];

	for ( i = 0; i < vPunktDarst.size(); i++ )
		delete vPunktDarst[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert ein neues Planzeichen	als Einträge in der Indexdatei und als   //
//  SVG-Objekte                                                              //
///////////////////////////////////////////////////////////////////////////////
void Planz::createPlanzeichen()
{
	unsigned int i, tabType;

	if ( sizeX > 0.0 && sizeY > 0 )
		pPlanzListe->createGeometrieObjekte ( sizeX, sizeY );
	else
		pPlanzListe->createGeometrieObjekte();

	tabType = 0;  //  Kein Attribut in Tabelle

	if ( selectAttribut1 != "" && selectAttribut2 == "" && selectAttribut3 == "" && symbolAttribut == "" ) 
		tabType = 1; // 1 Spalte (selectAttribut1)

	if ( selectAttribut1 != "" && selectAttribut2 != "" && selectAttribut3 == "" && symbolAttribut == "" )
		tabType = 3;  //  2 Spalten (selectAttribut1 und selectAttribut2)

	if ( selectAttribut1 == "" && selectAttribut2 == "" && selectAttribut3 == "" && symbolAttribut != "" )
		tabType = 2;  //  1 Spalte (symbolAttribut)

	if ( selectAttribut1 != "" && selectAttribut2 == "" && symbolAttribut != "" )
	{
		if ( selectAttribut1 == symbolAttribut ) tabType = 1;  
		else									 tabType = 4; // 2 Spalten (selectAttribut1 und symbolAttribut)
	}
	if ( selectAttribut1 != "" && selectAttribut2 != "" && symbolAttribut != "" )
	{
		if ( selectAttribut1 == symbolAttribut ||
			   selectAttribut2 == symbolAttribut)   tabType = 3;
		else									  tabType = 5; //  3 Spalten
	}

  if ( selectAttribut1 != "" && selectAttribut2 != "" && selectAttribut3 != "" && symbolAttribut == "" )
		tabType = 6; //  3 Spalten ( selectAttribut1, selectAttribut2, selectAttribut3 )
	

	if ( selectAttribut1 != "" && selectAttribut2 != "" && selectAttribut3 != "" && symbolAttribut != "" )
	{
		if ( selectAttribut1 == symbolAttribut ||
			   selectAttribut2 == symbolAttribut ||
			   selectAttribut3 == symbolAttribut )   tabType = 6;
		else									                     tabType = 7; 
	}

	getIndexDatei() << "<table width=\"100%\" border=\"1\" cellspacing=\"0\" bordercolor=\"#000000\">" << endl;

	if ( vFlaechenDarst.size() > 0 )
	{
		getIndexDatei() << "<tr>" << endl;
		getIndexDatei() << "<td colspan=\"5\"><h2>" << gmlClass <<
			 	      " - Flaechendarstellung" << "</h2></td>" << endl;
	    getIndexDatei() << "</tr>" << endl;

		writeTabellenUeberschrift ( tabType );
	}
	for ( i = 0; i < vFlaechenDarst.size(); i++ )
	{
		PlanzFlaechenDarst * pPlanzFlaechenDarst = vFlaechenDarst[i];
		pPlanzFlaechenDarst->createPlanzeichen( tabType );
	}

	if ( vLinienDarst.size() > 0 )
	{
		getIndexDatei() << "<tr>" << endl;
		getIndexDatei() << "<td colspan=\"5\"><h2>" << gmlClass <<
			 	      " - Liniendarstellung" << "</h2></td>" << endl;
	    getIndexDatei() << "</tr>" << endl;

		writeTabellenUeberschrift ( tabType );
	}
	for ( i = 0; i < vLinienDarst.size(); i++ )
	{
		PlanzLinienDarst * pPlanzLinienDarst = vLinienDarst[i];
		pPlanzLinienDarst->createPlanzeichen( tabType );
	}


	if ( vPunktDarst.size() > 0 )
	{
		getIndexDatei() << "<tr>" << endl;
		getIndexDatei() << "<td colspan=\"5\"><h2>" << gmlClass <<
			 	      " - Punktdarstellung" << "</h2></td>" << endl;
	    getIndexDatei() << "</tr>" << endl;

		writeTabellenUeberschrift ( tabType );
	}
	for ( i = 0; i < vPunktDarst.size(); i++ )
	{
		PlanzPunktDarst * pPlanzPunktDarst = vPunktDarst[i];
		pPlanzPunktDarst->createPlanzeichen( tabType );
	}
	getIndexDatei()<< "</table>" << endl;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt eine Tabellen-Überschrift in die Indexdatei   									 //
///////////////////////////////////////////////////////////////////////////////
void Planz::writeTabellenUeberschrift ( int tabType )
{
	string wert1, wert2, wert3, symbol;

	if ( tabType == 0 ) return;

	if ( selectAttribut1 == "" ) wert1 = "&nbsp;";
	else						             wert1 = selectAttribut1;

	if ( selectAttribut2 == "" ) wert2 = "&nbsp;";
	else						             wert2 = selectAttribut2;

	if ( selectAttribut3 == "" ) wert3 = "&nbsp;";
	else						             wert3 = selectAttribut3;

	if ( symbolAttribut == "" ) symbol = "&nbsp;";
	else						            symbol = symbolAttribut;

	getIndexDatei() << "<tr>" << endl;
	getIndexDatei() << "<td width=\"10%\">" << "&nbsp;" << "</td>" << endl;

	switch ( tabType )
	{ 
	case 1:
		getIndexDatei() << "<td width=\"90%\"><h3>" << wert1 << "</h3></td>" << endl;
	    getIndexDatei() << "</tr colspan=\"3\">" << endl;
		break;

	case 2:
		getIndexDatei() << "<td width=\"90%\"><h3>" << symbol << "</h3></td>" << endl;
	    getIndexDatei() << "</h3></tr colspan=\"3\">" << endl;
		break;

	case 3:
		getIndexDatei() << "<td width=\"45%\"><h3>" << wert1 << "</h3></td>" << endl;
		getIndexDatei() << "<td width=\"45%\"><h3>" << wert2 << "</h3></td>" <<	endl;
	    getIndexDatei() << "</h3></tr colspan=\"2\">" << endl;
		break;

	case 4:
		getIndexDatei() << "<td width=\"45%\"><h3>" << wert1  << "</h3></td>" << endl;
		getIndexDatei() << "<td width=\"45%\"><h3>" << symbol << "</h3></td>" <<	endl;
	    getIndexDatei() << "</h3></tr colspan=\"2\">" << endl;
		break;

	case 5:
		getIndexDatei() << "<td width=\"30%\"><h3>" << wert1 <<  "</h3></td>" << endl;
		getIndexDatei() << "<td width=\"30%\"><h3>" << wert2 <<  "</h3></td>" <<	endl;
		getIndexDatei() << "<td width=\"30%\"><h3>" << symbol << "</h3></td>" <<	endl;
	    getIndexDatei() << "</h3></tr>" << endl;
		break;

  case 6:
		getIndexDatei() << "<td width=\"30%\"><h3>" << wert1 <<  "</h3></td>" << endl;
		getIndexDatei() << "<td width=\"30%\"><h3>" << wert2 <<  "</h3></td>" <<	endl;
		getIndexDatei() << "<td width=\"30%\"><h3>" << wert3 << "</h3></td>" <<	endl;
	    getIndexDatei() << "</h3></tr colspan=\"4\">" << endl;
		break;

	case 7:
		getIndexDatei() << "<td width=\"22%\"><h3>" << wert1 <<  "</h3></td>" << endl;
		getIndexDatei() << "<td width=\"22%\"><h3>" << wert2 <<  "</h3></td>" <<	endl;
		getIndexDatei() << "<td width=\"23%\"><h3>" << wert3 << "</h3></td>" <<	endl;
		getIndexDatei() << "<td width=\"23%\"><h3>" << symbol << "</h3></td>" <<	endl;
	    getIndexDatei() << "</h3></tr>" << endl;
		break;
	}

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die aktuell definierte Größe des Planzeichens				             //
///////////////////////////////////////////////////////////////////////////////
void Planz::getSize ( double & x, double & y )
{
	if ( sizeX <= 0 || sizeY <= 0 )
		pPlanzListe->getSize ( x, y );
	else
	{
		x = sizeX;
		y = sizeY;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  class PlanzDarst				                            										 //
//  Basisklasse für Darstellungsparameter von Planzeichen                    //
///////////////////////////////////////////////////////////////////////////////
PlanzDarst::PlanzDarst( Planz * pPlanzP )
{
	pPlanz = pPlanzP;

	symbolScale = 1.0;
}

PlanzDarst::~ PlanzDarst()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert einen Dateinamen für die von der Indexdatei referierten 			 //
//  Dokumente                                                                //
///////////////////////////////////////////////////////////////////////////////
std::string PlanzDarst::createDateiName ( int tabType, std::string geoPraefix )
{
	string dateiName;
  string selectAttribut1;
  string selectAttribut2;
  string selectAttribut3;
  string symbolAttribut;

	dateiName = geoPraefix + generateDateiName ( pPlanz->getGmlClass() );
  selectAttribut1 = generateDateiName ( pPlanz->getSelectAttribut1() );
  selectAttribut2 = generateDateiName ( pPlanz->getSelectAttribut2() );
  selectAttribut3 = generateDateiName ( pPlanz->getSelectAttribut3() );
  symbolAttribut  = generateDateiName( pPlanz->getSymbolAttribut() );

	switch ( tabType )
	{
	case 1:
		dateiName = dateiName + "_" + selectAttribut1 + "_" + selectAttribut1Wert;
		break;

	case 2:
		dateiName = dateiName + "_" + symbolAttribut + "_" + symbolAttributWert;
		break;

	case 3:
		dateiName = dateiName + "_" + selectAttribut1 + "_" + selectAttribut1Wert
			                    + "_" + selectAttribut2 + "_" + selectAttribut2Wert;
		break;

	case 4:
		dateiName = dateiName + "_" + selectAttribut1 + "_" + selectAttribut1Wert
			                    + "_" + symbolAttribut  + "_" + symbolAttributWert;
		break;
	
	case 5:
		dateiName = dateiName + "_" + selectAttribut1 + "_" + selectAttribut1Wert
			                    + "_" + selectAttribut2 + "_" + selectAttribut2Wert
			                    + "_" + symbolAttribut  + "_" + symbolAttributWert;
		break;
	
	case 6:
		dateiName = dateiName + "_" + selectAttribut1 + "_" + selectAttribut1Wert
			                    + "_" + selectAttribut2 + "_" + selectAttribut2Wert
			                    + "_" + selectAttribut3 + "_" + selectAttribut3Wert;
		break;
	
	case 7:
		dateiName = dateiName + "_" + selectAttribut1 + "_" + selectAttribut1Wert
			                    + "_" + selectAttribut2 + "_" + selectAttribut2Wert
			                    + "_" + selectAttribut3 + "_" + selectAttribut3Wert 
			                    + "_" + symbolAttribut  + "_" + symbolAttributWert;
		break;
	}

	return dateiName;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt ein : aus dem qualifizierten Klassennamen                       //
///////////////////////////////////////////////////////////////////////////////
std::string PlanzDarst::generateDateiName ( std::string name )
{
	size_t index;

  index = name.find ( ":" );
  if ( index != string::npos )
    name.replace( index, 1, "" );

  return name;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt eine Tabellen-Zeile drer Indexdatei           									 //
///////////////////////////////////////////////////////////////////////////////
void PlanzDarst::writeTabellenZeile ( int tabType, std::string svgDateiPath, std::string pngDateiPath )
{
	string wert1, wert2, wert3, symbol;

	if ( selectAttribut1WertText != "" )
		wert1 = selectAttribut1WertText + " (" + selectAttribut1Wert + ")";
	else
		wert1 = selectAttribut1Wert;
	if ( wert1 == "" ) wert1 = "&nbsp;";

	if ( selectAttribut2Wert == "9999" )
		wert2 = "&nbsp;";
	else
	if ( selectAttribut2WertText != "" )
		wert2 = selectAttribut2WertText + " (" + selectAttribut2Wert + ")";
	else
		wert2 = selectAttribut2Wert;
	if ( wert2 == "" ) wert2 = "&nbsp;";

	if ( selectAttribut3Wert == "9999" )
		wert3 = "&nbsp;";
	else
	if ( selectAttribut3WertText != "" )
		wert3 = selectAttribut3WertText + " (" + selectAttribut3Wert + ")";
	else
		wert3 = selectAttribut3Wert;
	if ( wert3 == "" ) wert3 = "&nbsp;";

	if ( symbolAttributWertText != "" )
		symbol = symbolAttributWertText + " (" + symbolAttributWert + ")";
	else
		symbol = symbolAttributWert;
	if ( symbol == "" ) symbol = "&nbsp;";


	getPlanz()->getIndexDatei() << "<tr>" << endl;
	getPlanz()->getIndexDatei() << "<td width=\"10%\"><div align=\"center\">";
	getPlanz()->getIndexDatei() << "<a href=\"" << svgDateiPath << "\">";
	getPlanz()->getIndexDatei() << "<img src=\"" << pngDateiPath << "\"></a></div></td>" << endl;

	switch ( tabType )
	{
	case 0:
		getPlanz()->getIndexDatei() << "<td width=\"90%\" colspan=\"3\">" << "&nbsp;" << "</td>" << endl;
	    getPlanz()->getIndexDatei() << "</tr>" << endl;
		break;

	case 1:
		getPlanz()->getIndexDatei() << "<td width=\"90%\" colspan=\"3\">" << wert1 << "</td>" << endl;
	    getPlanz()->getIndexDatei() << "</tr>" << endl;
		break;

	case 2:
		getPlanz()->getIndexDatei() << "<td width=\"90%\" colspan=\"3\">" << symbol << "</td>" << endl;
	    getPlanz()->getIndexDatei() << "</tr>" << endl;
		break;

	case 3:
		getPlanz()->getIndexDatei() << "<td width=\"45%\">" << wert1 << "</td>" << endl;
		getPlanz()->getIndexDatei() << "<td width=\"45%\" colspan=\"2\">" << wert2 << "</td>" << endl;
	    getPlanz()->getIndexDatei() << "</tr>" << endl;
		break;

	case 4:
		getPlanz()->getIndexDatei() << "<td width=\"45%\">" << wert1 << "</td>" << endl;
		getPlanz()->getIndexDatei() << "<td width=\"45%\" colspan=\"2\">" << symbol << "</td>" << endl;
	    getPlanz()->getIndexDatei() << "</tr>" << endl;
		break;

	case 5:
		getPlanz()->getIndexDatei() << "<td width=\"30%\">" << wert1 << "</td>" << endl;
		getPlanz()->getIndexDatei() << "<td width=\"30%\">" << wert2 << "</td>" << endl;
		getPlanz()->getIndexDatei() << "<td width=\"30%\">" << symbol << "</td>" << endl;
	    getPlanz()->getIndexDatei() << "</tr>" << endl;
		break;

	case 6:
		getPlanz()->getIndexDatei() << "<td width=\"30%\">" << wert1 << "</td>" << endl;
		getPlanz()->getIndexDatei() << "<td width=\"30%\">" << wert2 << "</td>" << endl;
		getPlanz()->getIndexDatei() << "<td width=\"30%\" colspan=\"3\">" << wert3 << "</td>" << endl;
	    getPlanz()->getIndexDatei() << "</tr>" << endl;
		break;

	case 7:
		getPlanz()->getIndexDatei() << "<td width=\"22%\">" << wert1 << "</td>" << endl;
		getPlanz()->getIndexDatei() << "<td width=\"22%\">" << wert2 << "</td>" << endl;
		getPlanz()->getIndexDatei() << "<td width=\"23%\">" << wert3 << "</td>" << endl;
		getPlanz()->getIndexDatei() << "<td width=\"23%\">" << symbol << "</td>" << endl;
	    getPlanz()->getIndexDatei() << "</tr>" << endl;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//   Liefert eine textuelle Präzisoerung eines Attributwertes 					     //
///////////////////////////////////////////////////////////////////////////////
std::string PlanzDarst::getAttributWertText ( std::string attributName, std::string attributWert )
{
	bool			        success;
	string            attributWertText;
	string            enumerationName;
	string            enumText;
	string            klassenName =  getPlanz()->getGmlClass();

	Visualisierung_2D  * pVisualisierung = getPlanz()->getPlanzListe()->getVisualisierung();
	GmlSchema      * pGmlObjekte = pVisualisierung->getFeatures()->getGmlSchema();
	GmlKlasse       * pGmlKlasse = pGmlObjekte->getGmlKlasseFromElementName ( klassenName );
	GmlDictionary   * pCodeLists = pGmlObjekte->getCodeListsDictionary();
//	GmlDictionary   * pExternalCodeLists = pGmlObjekte->getExternalCodeListsDictionary();

	GmlDictionaryEntry * pDictionaryEntry;
	GmlAttribut        * pGmlAttribut;

	attributWertText = "";
	if ( pGmlKlasse == NULL ) return attributWertText;

	pGmlAttribut = pGmlKlasse->getGmlAttribut ( attributName );
	if ( pGmlAttribut == NULL ) return attributWertText;

	switch ( pGmlAttribut->getGmlAttributTyp() )
	{
	case _ENUMERATION:
			enumerationName = pGmlAttribut->getGmlEnumeration();
			pDictionaryEntry = pCodeLists->getDictionaryEntry ( enumerationName );
			if ( pDictionaryEntry != NULL )
			{
				success = pDictionaryEntry->getSchluesselText ( attributWert, enumText );
				if ( success ) attributWertText = enumText;
			}
			break;

	case _EXTERNAL_CODE_LIST:
			enumerationName = pGmlAttribut->getGmlEnumeration();
			pDictionaryEntry = pCodeLists->getDictionaryEntry ( enumerationName );
			if ( pDictionaryEntry != NULL )
			{
				success = pDictionaryEntry->getSchluesselText ( attributWert, enumText );
				if ( success ) attributWertText = enumText;
			}
			break;

  case _EXTERNAL_CODE_LIST_GML3_3:
    attributWertText = attributWert;
    break;

	}

	return attributWertText;
}

///////////////////////////////////////////////////////////////////////////////
//  class PlanzFlachenDarst			                        										 //
//  Darstellung einer Flächensignatur		                    								 //
///////////////////////////////////////////////////////////////////////////////
PlanzFlaechenDarst::PlanzFlaechenDarst( Planz * pPlanzP )
:PlanzDarst ( pPlanzP )
{
}

PlanzFlaechenDarst::~ PlanzFlaechenDarst()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die SVG-Darstellung eines Planzeichens                         //
///////////////////////////////////////////////////////////////////////////////
void PlanzFlaechenDarst::createPlanzeichen( int tabType )
{
	string dateiName;
	string svgDateiName;
	string svgPath;
	string svgDateiPath;
	string pngDateiName;
	string pngPath;
	string pngDateiPath;
	string selectAttribut1Name;
	string selectAttribut2Name;
	string selectAttribut3Name;
	string symbolAttributName;
  double sizeX, sizeY, h;
	int    ih;
	bool   createPPOObjekt = false;

	SvgVisualisierung  * pSvgParams;
	SvgWriter   		   ^ pSvgWriter;
	Feature         * pFlaechenObj;
	Feature         * pPPOObj;
	GmlSchema         * pGmlObjekte;
	GmlKlasse          * pGmlKlasse;
	GmlAttribut        * pGmlAttribut;
	ATTRIBUT_TYP         selectAttribut1Typ;
	ATTRIBUT_TYP         selectAttribut2Typ;
	ATTRIBUT_TYP         selectAttribut3Typ;
	ATTRIBUT_TYP         symbolAttributTyp;

	pSvgParams = pPlanz->getPlanzListe()->getVisualisierung()->getSvgParams();

	pFlaechenObj  = pPlanz->getPlanzListe()->getFlaechenObjekt();
	pFlaechenObj->setKlassenName ( pPlanz->getGmlClass() );
	
	pPPOObj  = pPlanz->getPlanzListe()->getPPOObjekt();
	pGmlObjekte = pPlanz->getPlanzListe()->getVisualisierung()->getFeatures()->getGmlSchema();
	pGmlKlasse  = pGmlObjekte->getGmlKlasseFromElementName (  pPlanz->getGmlClass() );

	symbolAttributName = pPlanz->getSymbolAttribut();
	selectAttribut1Name = pPlanz->getSelectAttribut1();
	selectAttribut2Name = pPlanz->getSelectAttribut2();
	selectAttribut3Name = pPlanz->getSelectAttribut3();

	if ( selectAttribut1Name != "" )
	{
		if ( pGmlKlasse != NULL )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut1Name );
			if ( pGmlAttribut != NULL )
				selectAttribut1Typ = pGmlAttribut->getGmlAttributTyp();
			else
				selectAttribut1Typ = _STRING;
		}
		else
				selectAttribut1Typ = _STRING;

		switch ( selectAttribut1Typ )
		{
		case _BOOLEAN:
			pFlaechenObj->deleteBoolAttribut ( selectAttribut1Name );
			if ( selectAttribut1Wert == "true" )
				pFlaechenObj->setBoolAttribut ( selectAttribut1Name, true );
			else
			if ( selectAttribut1Wert == "false" )
				pFlaechenObj->setBoolAttribut ( selectAttribut1Name, false );
			break;

		case _INTEGER:
			pFlaechenObj->deleteIntegerAttribut ( selectAttribut1Name );
      if ( selectAttribut1Wert == "" || selectAttribut1Wert == "IIII" || selectAttribut1Wert == "BBBB" )
        ih = 0;
      else
			  ih = XmlConvert::ToInt32 ( QuConvert::ToString ( selectAttribut1Wert ) );
			pFlaechenObj->setIntegerAttribut ( selectAttribut1Name, ih );
			break;

		default:
			pFlaechenObj->deleteStringAttribut ( selectAttribut1Name );
			pFlaechenObj->setStringAttribut ( selectAttribut1Name, selectAttribut1Wert );
			selectAttribut1WertText = getAttributWertText( selectAttribut1Name, selectAttribut1Wert );
			break;
		}
	}

	if ( selectAttribut2Name != "" )
	{
		if ( pGmlKlasse != NULL )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut2Name );
			if ( pGmlAttribut != NULL )
				selectAttribut2Typ = pGmlAttribut->getGmlAttributTyp();
			else
				selectAttribut2Typ = _STRING;
		}
		else
				selectAttribut2Typ = _STRING;

		switch ( selectAttribut2Typ )
		{
		case _BOOLEAN:
			pFlaechenObj->deleteBoolAttribut ( selectAttribut2Name );
			if ( selectAttribut2Wert == "true" )
				pFlaechenObj->setBoolAttribut ( selectAttribut2Name, true );
			else
			if ( selectAttribut2Wert == "false" )
				pFlaechenObj->setBoolAttribut ( selectAttribut2Name, false );
			break;

		case _INTEGER:
			pFlaechenObj->deleteIntegerAttribut ( selectAttribut2Name );
			if ( selectAttribut2Wert != "" && selectAttribut1Wert != "IIII" && selectAttribut1Wert != "BBBB")
				ih = XmlConvert::ToInt32 ( QuConvert::ToString ( selectAttribut2Wert ) );
			else
				ih = 0;
			pFlaechenObj->setIntegerAttribut ( selectAttribut2Name, ih );
			break;

		default:
			pFlaechenObj->deleteStringAttribut ( selectAttribut2Name );
			pFlaechenObj->setStringAttribut ( selectAttribut2Name, selectAttribut2Wert );
			selectAttribut2WertText = getAttributWertText( selectAttribut2Name, selectAttribut2Wert );
			break;
		}
	}

	if ( selectAttribut3Name != "" )
	{
		if ( pGmlKlasse != NULL )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut3Name );
			if ( pGmlAttribut != NULL )
				selectAttribut3Typ = pGmlAttribut->getGmlAttributTyp();
			else
				selectAttribut3Typ = _STRING;
		}
		else
				selectAttribut3Typ = _STRING;

		switch ( selectAttribut3Typ )
		{
		case _BOOLEAN:
			pFlaechenObj->deleteBoolAttribut ( selectAttribut3Name );
			if ( selectAttribut3Wert == "true" )
				pFlaechenObj->setBoolAttribut ( selectAttribut3Name, true );
			else
			if ( selectAttribut3Wert == "false" )
				pFlaechenObj->setBoolAttribut ( selectAttribut3Name, false );
			break;

		case _INTEGER:
			pFlaechenObj->deleteIntegerAttribut ( selectAttribut3Name );
			if ( selectAttribut3Wert != ""  && selectAttribut1Wert != "IIII" && selectAttribut1Wert != "BBBB" )
				ih = XmlConvert::ToInt32 ( QuConvert::ToString ( selectAttribut3Wert ) );
			else
				ih = 0;
			pFlaechenObj->setIntegerAttribut ( selectAttribut3Name, ih );
			break;

		default:
			pFlaechenObj->deleteStringAttribut ( selectAttribut3Name );
			pFlaechenObj->setStringAttribut ( selectAttribut3Name, selectAttribut3Wert );
			selectAttribut3WertText = getAttributWertText( selectAttribut3Name, selectAttribut3Wert );
			break;
		}
	}

	if (  symbolAttributName != "" )
	{
		if ( pGmlKlasse != NULL )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( symbolAttributName );
			if ( pGmlAttribut != NULL )
				symbolAttributTyp = pGmlAttribut->getGmlAttributTyp();
			else
				symbolAttributTyp = _STRING;
		}
		else
				symbolAttributTyp = _STRING;

		switch ( symbolAttributTyp )
		{
		case _BOOLEAN:
			pFlaechenObj->deleteBoolAttribut ( symbolAttributName );
			if ( selectAttribut2Wert == "true" )
				pFlaechenObj->setBoolAttribut ( symbolAttributName, true );
			else
			if ( selectAttribut2Wert == "false" )
				pFlaechenObj->setBoolAttribut ( symbolAttributName, false );
			break;

		case _INTEGER:
			pFlaechenObj->deleteIntegerAttribut ( symbolAttributName );
      if ( symbolAttributWert != "" ) 
			  ih = XmlConvert::ToInt32 ( QuConvert::ToString ( symbolAttributWert ) );
      else
        ih = 0;
			pFlaechenObj->setIntegerAttribut ( symbolAttributName, ih );
			break;

		case _DOUBLE:
			pFlaechenObj->deleteIntegerAttribut ( symbolAttributName );
			h = XmlConvert::ToDouble ( QuConvert::ToString ( symbolAttributWert ) );
			pFlaechenObj->setDoubleAttribut ( symbolAttributName, h );
			break;

		case _DATE:
      {
        pFlaechenObj->deleteDateAttribut ( symbolAttributName );

        TimePosition * pTime = new TimePosition;
        pTime->fromXMLString( symbolAttributWert );
        DateAttr * pDateAttr = new DateAttr;
        pDateAttr->value = pTime;
			  pFlaechenObj->setDateAttribut ( symbolAttributName, pDateAttr );
			  symbolAttributWertText = symbolAttributWert;
			  break;
      }

		case _ANY_URI:
			pFlaechenObj->deleteURLAttribut ( symbolAttributName );
			pFlaechenObj->setURLAttribut ( symbolAttributName, symbolAttributWert );
			symbolAttributWertText = symbolAttributWert;
			break;

		default:
			pFlaechenObj->deleteStringAttribut ( symbolAttributName );
			pFlaechenObj->setStringAttribut ( symbolAttributName, symbolAttributWert );
			symbolAttributWertText = getAttributWertText( symbolAttributName, symbolAttributWert );
			break;
		}

    pPPOObj->deleteStringAttribut ( "xplan:art" );
    pPPOObj->setStringAttribut ( "xplan:art", symbolAttributName );

    pPPOObj->deleteDoubleAttribut ( "xplan:skalierung" );
    pPPOObj->setDoubleAttribut ( "xplan:skalierung", symbolScale );
	}

	dateiName = createDateiName ( tabType, "F_" );

	svgPath = pPlanz->getPlanzListe()->getSvgPath();
	svgDateiName = dateiName + ".svg";
	svgDateiPath = svgPath + svgDateiName;

	pSvgWriter = gcnew SvgWriter ( pPlanz->getPlanzListe()->getVisualisierung(), false, 2, 0  );

	getPlanz()->getSize ( sizeX, sizeY );
	pSvgWriter->writeSvgHeader ( svgDateiPath, -0.5*sizeX, -0.5*sizeY, 0.5*sizeX, 0.5*sizeY );
	pSvgWriter->writePlanObjekt ( pFlaechenObj, false, 0 );
	if ( symbolAttributWert != "" || createPPOObjekt )
		pSvgWriter->writePlanObjekt ( pPPOObj, false, 0 );
	pSvgWriter->writeSvgEnd ();
	pSvgWriter->close();

	//  Konvertieren svp --> png
	pngPath = pPlanz->getPlanzListe()->getPngPath();
	pngDateiName = dateiName + ".png";
	pngDateiPath = pngPath + pngDateiName;

	svgDateiPath = pPlanz->getPlanzListe()->getSvgFolder() + svgDateiName;
	pngDateiPath = pPlanz->getPlanzListe()->getPngFolder() + pngDateiName;
	writeTabellenZeile ( tabType, svgDateiPath, pngDateiPath );
}

///////////////////////////////////////////////////////////////////////////////
//  class PlanzLinienDarst			                        										 //
//  Darstllung einer Liniensignatur							                    				 //
///////////////////////////////////////////////////////////////////////////////
PlanzLinienDarst::PlanzLinienDarst( Planz * pPlanzP )
:PlanzDarst ( pPlanzP )
{
}

PlanzLinienDarst::~ PlanzLinienDarst()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die SVG-Darstellung des Planzeichens             							 //
///////////////////////////////////////////////////////////////////////////////
void PlanzLinienDarst::createPlanzeichen( int tabType )
{
	string dateiName;
	string svgDateiName;
	string svgPath;
	string svgDateiPath;
	string pngDateiName;
	string pngPath;
	string pngDateiPath;
	string selectAttribut1Name;
	string selectAttribut2Name;
	string selectAttribut3Name;
	double sizeX, sizeY;
	int    ih;

	SvgWriter		       ^ pSvgWriter;
	Feature         * pLinienObj;
//  PlanLinienObjekt   * pLinienObj;
	GmlSchema         * pGmlObjekte;
	GmlKlasse          * pGmlKlasse;
	GmlAttribut        * pGmlAttribut;
	ATTRIBUT_TYP         selectAttribut1Typ;
	ATTRIBUT_TYP         selectAttribut2Typ;
	ATTRIBUT_TYP         selectAttribut3Typ;

	pLinienObj  = pPlanz->getPlanzListe()->getLinienObjekt();
	pLinienObj->setKlassenName ( pPlanz->getGmlClass() );

	pGmlObjekte = pPlanz->getPlanzListe()->getVisualisierung()->getFeatures()->getGmlSchema();
	pGmlKlasse  = pGmlObjekte->getGmlKlasseFromElementName (  pPlanz->getGmlClass() );

	selectAttribut1Name = pPlanz->getSelectAttribut1();
	selectAttribut2Name = pPlanz->getSelectAttribut2();
	selectAttribut3Name = pPlanz->getSelectAttribut3();

	if ( selectAttribut1Name != "" )
	{
		if ( pGmlKlasse != NULL )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut1Name );
			if ( pGmlAttribut != NULL )
				selectAttribut1Typ = pGmlAttribut->getGmlAttributTyp();
			else
				selectAttribut1Typ = _STRING;
		}
		else
				selectAttribut1Typ = _STRING;

		switch ( selectAttribut1Typ )
		{
		case _BOOLEAN:
			pLinienObj->deleteBoolAttribut ( selectAttribut1Name );
			if ( selectAttribut1Wert == "true" )
				pLinienObj->setBoolAttribut ( selectAttribut1Name, true );
			else
			if ( selectAttribut1Wert == "false" )
				pLinienObj->setBoolAttribut ( selectAttribut1Name, false );
			break;

		case _INTEGER:
			pLinienObj->deleteIntegerAttribut ( selectAttribut1Name );
      if ( selectAttribut1Wert != ""  && selectAttribut1Wert != "IIII" && selectAttribut1Wert != "BBBB" )
      {
			  ih = XmlConvert::ToInt32 ( QuConvert::ToString ( selectAttribut1Wert ) );
			  pLinienObj->setIntegerAttribut ( selectAttribut1Name, ih );
      }
			break;

		default:
			pLinienObj->deleteStringAttribut ( selectAttribut1Name );
			pLinienObj->setStringAttribut ( selectAttribut1Name, selectAttribut1Wert );
			selectAttribut1WertText = getAttributWertText( selectAttribut1Name, selectAttribut1Wert );
			break;
		}	
	}

	if ( selectAttribut2Name != "" )
	{
		if ( pGmlKlasse != NULL )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut2Name );
			if ( pGmlAttribut != NULL )
				selectAttribut2Typ = pGmlAttribut->getGmlAttributTyp();
			else
				selectAttribut2Typ = _STRING;
		}
		else
				selectAttribut2Typ = _STRING;

		switch ( selectAttribut2Typ )
		{
		case _BOOLEAN:
			pLinienObj->deleteBoolAttribut ( selectAttribut2Name );
			if ( selectAttribut2Wert == "true" )
				pLinienObj->setBoolAttribut ( selectAttribut2Name, true );
			else
			if ( selectAttribut2Wert == "false" )
				pLinienObj->setBoolAttribut ( selectAttribut2Name, false );
			break;

		case _INTEGER:
			pLinienObj->deleteIntegerAttribut ( selectAttribut2Name );
      if ( selectAttribut2Wert != ""  && selectAttribut1Wert != "IIII" && selectAttribut1Wert != "BBBB" )
			  ih = XmlConvert::ToInt32 ( QuConvert::ToString ( selectAttribut2Wert ) );
      else
        ih = 0;
			pLinienObj->setIntegerAttribut ( selectAttribut2Name, ih );
			break;

		default:
			pLinienObj->deleteStringAttribut ( selectAttribut2Name );
			pLinienObj->setStringAttribut ( selectAttribut2Name, selectAttribut2Wert );
			selectAttribut2WertText = getAttributWertText( selectAttribut2Name, selectAttribut2Wert );
			break;
		}	
	}

	if ( selectAttribut3Name != "" )
	{
		if ( pGmlKlasse != NULL )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut3Name );
			if ( pGmlAttribut != NULL )
				selectAttribut3Typ = pGmlAttribut->getGmlAttributTyp();
			else
				selectAttribut3Typ = _STRING;
		}
		else
				selectAttribut3Typ = _STRING;

		switch ( selectAttribut2Typ )
		{
		case _BOOLEAN:
			pLinienObj->deleteBoolAttribut ( selectAttribut3Name );
			if ( selectAttribut3Wert == "true" )
				pLinienObj->setBoolAttribut ( selectAttribut3Name, true );
			else
			if ( selectAttribut3Wert == "false" )
				pLinienObj->setBoolAttribut ( selectAttribut3Name, false );
			break;

		case _INTEGER:
			pLinienObj->deleteIntegerAttribut ( selectAttribut3Name );
      if (selectAttribut3Wert != ""  && selectAttribut1Wert != "IIII" && selectAttribut1Wert != "BBBB" )
			  ih = XmlConvert::ToInt32 ( QuConvert::ToString ( selectAttribut3Wert ) );
      else
        ih = 0;
			pLinienObj->setIntegerAttribut ( selectAttribut3Name, ih );
			break;

		default:
			pLinienObj->deleteStringAttribut ( selectAttribut3Name );
			pLinienObj->setStringAttribut ( selectAttribut3Name, selectAttribut3Wert );
			selectAttribut3WertText = getAttributWertText( selectAttribut3Name, selectAttribut3Wert );
			break;
		}	
	}

	dateiName = createDateiName ( tabType, "L_" );

	svgPath = pPlanz->getPlanzListe()->getSvgPath();
	svgDateiName = dateiName + ".svg";
	svgDateiPath = svgPath + svgDateiName;

	pSvgWriter = gcnew SvgWriter ( pPlanz->getPlanzListe()->getVisualisierung(), false, 2, 0  );

	getPlanz()->getSize ( sizeX, sizeY );
	pSvgWriter->writeSvgHeader ( svgDateiPath, -0.5*sizeX, -0.5*sizeY, 0.5*sizeX, 0.5*sizeY );
	pSvgWriter->writePlanObjekt ( pLinienObj, false, 0 );
	pSvgWriter->writeSvgEnd ();
	pSvgWriter->close();

	//  Konvertieren svp --> png
	pngPath = pPlanz->getPlanzListe()->getPngPath();
	pngDateiName = dateiName + ".png";
	pngDateiPath = pngPath + pngDateiName;

	svgDateiPath = pPlanz->getPlanzListe()->getSvgFolder() + svgDateiName;
	pngDateiPath = pPlanz->getPlanzListe()->getPngFolder() + pngDateiName;
	writeTabellenZeile ( tabType, svgDateiPath, pngDateiPath );
}

///////////////////////////////////////////////////////////////////////////////
//  class PlanzPunktDarst						                          							 //
//  Darstllung einer Punktsignatur					                      					 //
///////////////////////////////////////////////////////////////////////////////
PlanzPunktDarst::PlanzPunktDarst( Planz * pPlanzP )
:PlanzDarst ( pPlanzP )
{
}

PlanzPunktDarst::~ PlanzPunktDarst()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die SVG-Darstellung des Planzeichens           								 //
///////////////////////////////////////////////////////////////////////////////
void PlanzPunktDarst::createPlanzeichen( int tabType )
{
	string dateiName;
	string svgDateiName;
	string svgPath;
	string svgDateiPath;
	string pngDateiName;
	string pngPath;
	string pngDateiPath;
	string selectAttribut1Name;
	string selectAttribut2Name;
	string selectAttribut3Name;
	string symbolAttributName;
	double sizeX, sizeY, h;
	int    ih;

	SvgWriter	      ^ pSvgWriter;
	Feature         * pPunktObj;
	GmlSchema       * pGmlObjekte;
	GmlKlasse       * pGmlKlasse;
	GmlAttribut     * pGmlAttribut;
	ATTRIBUT_TYP      selectAttribut1Typ;
	ATTRIBUT_TYP      selectAttribut2Typ;
	ATTRIBUT_TYP      selectAttribut3Typ;
	ATTRIBUT_TYP      symbolAttributTyp;

	pPunktObj  = pPlanz->getPlanzListe()->getPunktObjekt();
	pPunktObj->setKlassenName ( pPlanz->getGmlClass() );

	pGmlObjekte = pPlanz->getPlanzListe()->getVisualisierung()->getFeatures()->getGmlSchema();
	pGmlKlasse  = pGmlObjekte->getGmlKlasseFromElementName (  pPlanz->getGmlClass() );

	selectAttribut1Name = pPlanz->getSelectAttribut1();
	selectAttribut2Name = pPlanz->getSelectAttribut2();
	selectAttribut3Name = pPlanz->getSelectAttribut3();
	symbolAttributName  = pPlanz->getSymbolAttribut();

	if ( selectAttribut1Name != "" )
	{
		if ( pGmlKlasse != NULL )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut1Name );
			if ( pGmlAttribut != NULL )
				selectAttribut1Typ = pGmlAttribut->getGmlAttributTyp();
			else
				selectAttribut1Typ = _STRING;
		}
		else
				selectAttribut1Typ = _STRING;

		switch ( selectAttribut1Typ )
		{
		case _BOOLEAN:
			pPunktObj->deleteBoolAttribut ( selectAttribut1Name );
			if ( selectAttribut1Wert == "true" )
				pPunktObj->setBoolAttribut ( selectAttribut1Name, true );
			else
			if ( selectAttribut1Wert == "false" )
				pPunktObj->setBoolAttribut ( selectAttribut1Name, false );
			break;

		case _INTEGER:
			pPunktObj->deleteIntegerAttribut ( selectAttribut1Name );
      if ( selectAttribut1Wert != "" && selectAttribut1Wert != "IIII" && selectAttribut1Wert != "BBBB")
			  ih = XmlConvert::ToInt32 ( QuConvert::ToString ( selectAttribut1Wert ) );
      else
        ih = 0;
			pPunktObj->setIntegerAttribut ( selectAttribut1Name, ih );
			break;

		default:
			pPunktObj->deleteStringAttribut ( selectAttribut1Name );
			pPunktObj->setStringAttribut ( selectAttribut1Name, selectAttribut1Wert );
			selectAttribut1WertText = getAttributWertText( selectAttribut1Name, selectAttribut1Wert );
			break;
		}
	}

	if ( selectAttribut2Name != "" )
	{
		if ( pGmlKlasse != NULL )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut2Name );
			if ( pGmlAttribut != NULL )
				selectAttribut2Typ = pGmlAttribut->getGmlAttributTyp();
			else
				selectAttribut2Typ = _STRING;
		}
		else
				selectAttribut2Typ = _STRING;

		switch ( selectAttribut2Typ )
		{
		case _BOOLEAN:
			pPunktObj->deleteBoolAttribut ( selectAttribut2Name );
			if ( selectAttribut2Wert == "true" )
				pPunktObj->setBoolAttribut ( selectAttribut2Name, true );
			else
			if ( selectAttribut2Wert == "false" )
				pPunktObj->setBoolAttribut ( selectAttribut2Name, false );
			break;

		case _INTEGER:
			pPunktObj->deleteIntegerAttribut ( selectAttribut2Name );
      if ( selectAttribut2Wert != "" && selectAttribut1Wert != "IIII" && selectAttribut1Wert != "BBBB" )
			  ih = XmlConvert::ToInt32 ( QuConvert::ToString ( selectAttribut2Wert ) );
      else
        ih = 0;
			pPunktObj->setIntegerAttribut ( selectAttribut2Name, ih );
			break;

		default:
			pPunktObj->deleteStringAttribut ( selectAttribut2Name );
			pPunktObj->setStringAttribut ( selectAttribut2Name, selectAttribut2Wert );
			selectAttribut2WertText = getAttributWertText( selectAttribut2Name, selectAttribut2Wert );
			break;
		}
	}

	if ( selectAttribut3Name != "" )
	{
		if ( pGmlKlasse != NULL )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( selectAttribut3Name );
			if ( pGmlAttribut != NULL )
				selectAttribut3Typ = pGmlAttribut->getGmlAttributTyp();
			else
				selectAttribut3Typ = _STRING;
		}
		else
				selectAttribut3Typ = _STRING;

		switch ( selectAttribut3Typ )
		{
		case _BOOLEAN:
			pPunktObj->deleteBoolAttribut ( selectAttribut3Name );
			if ( selectAttribut2Wert == "true" )
				pPunktObj->setBoolAttribut ( selectAttribut3Name, true );
			else
			if ( selectAttribut2Wert == "false" )
				pPunktObj->setBoolAttribut ( selectAttribut3Name, false );
			break;

		case _INTEGER:
			pPunktObj->deleteIntegerAttribut ( selectAttribut3Name );
      if ( selectAttribut3Wert != "" && selectAttribut1Wert != "IIII" && selectAttribut1Wert != "BBBB" )
			  ih = XmlConvert::ToInt32 ( QuConvert::ToString ( selectAttribut3Wert ) );
      else
        ih = 0;
			pPunktObj->setIntegerAttribut ( selectAttribut3Name, ih );
			break;

		default:
			pPunktObj->deleteStringAttribut ( selectAttribut3Name );
			pPunktObj->setStringAttribut ( selectAttribut3Name, selectAttribut3Wert );
			selectAttribut3WertText = getAttributWertText( selectAttribut3Name, selectAttribut3Wert );
			break;
		}
	}

	if (  symbolAttributName != "" )
	{
		if ( pGmlKlasse != NULL )
		{
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( symbolAttributName );
			if ( pGmlAttribut != NULL )
				symbolAttributTyp = pGmlAttribut->getGmlAttributTyp();
			else
				symbolAttributTyp = _STRING;
		}
		else
				symbolAttributTyp = _STRING;

		switch ( symbolAttributTyp )
		{
		case _BOOLEAN:
			pPunktObj->deleteBoolAttribut ( selectAttribut2Name );
			if ( selectAttribut2Wert == "true" )
				pPunktObj->setBoolAttribut ( symbolAttributName, true );
			else
			if ( selectAttribut2Wert == "false" )
				pPunktObj->setBoolAttribut ( symbolAttributName, false );
			break;

		case _INTEGER:
			pPunktObj->deleteIntegerAttribut ( symbolAttributName );
			ih = XmlConvert::ToInt32 ( QuConvert::ToString ( symbolAttributWert ) );
			pPunktObj->setIntegerAttribut ( symbolAttributName, ih );
			break;

		case _DOUBLE:
			pPunktObj->deleteDoubleAttribut ( symbolAttributName );
			h = XmlConvert::ToDouble ( QuConvert::ToString ( symbolAttributWert ) );
			pPunktObj->setDoubleAttribut ( symbolAttributName, h );
			break;

		case _DATE:
    {
			pPunktObj->deleteDateAttribut ( symbolAttributName );

      TimePosition * pTime = new TimePosition;
      pTime->fromXMLString( symbolAttributWert );
      DateAttr * pDateAttr = new DateAttr;
      pDateAttr->value = pTime;
      pPunktObj->setDateAttribut ( symbolAttributName, pDateAttr );
      symbolAttributWertText = symbolAttributWert;
      break;
    }

		case _ANY_URI:
			pPunktObj->deleteURLAttribut ( symbolAttributName );
			pPunktObj->setURLAttribut ( symbolAttributName, symbolAttributWert );
			symbolAttributWertText = symbolAttributWert;
			break;

		default:
			pPunktObj->deleteStringAttribut ( symbolAttributName );
			pPunktObj->setStringAttribut ( symbolAttributName, symbolAttributWert );
			symbolAttributWertText = getAttributWertText( symbolAttributName, symbolAttributWert );
			break;
		}
	}

	dateiName = createDateiName ( tabType, "P_" );

	svgPath = pPlanz->getPlanzListe()->getSvgPath();
	svgDateiName = dateiName + ".svg";
	svgDateiPath = svgPath + svgDateiName;

	pSvgWriter = gcnew SvgWriter ( pPlanz->getPlanzListe()->getVisualisierung(), false, 2, 0  );

	getPlanz()->getSize ( sizeX, sizeY );
	sizeX = sizeX / symbolScale;
	sizeY = sizeY / symbolScale;

	pSvgWriter->writeSvgHeader ( svgDateiPath, -0.5*sizeX, -0.5*sizeY, 0.5*sizeX, 0.5*sizeY );
	pSvgWriter->writePlanObjekt ( pPunktObj, false, 0 );
	pSvgWriter->writeSvgEnd ();
	pSvgWriter->close();

	//  Konvertieren svp --> png
	pngPath = pPlanz->getPlanzListe()->getPngPath();
	pngDateiName = dateiName + ".png";
	pngDateiPath = pngPath + pngDateiName;

	svgDateiPath = pPlanz->getPlanzListe()->getSvgFolder() + svgDateiName;
	pngDateiPath = pPlanz->getPlanzListe()->getPngFolder() + pngDateiName;
	writeTabellenZeile ( tabType, svgDateiPath, pngDateiPath );}


///////////////////////////////////////////////////////////////////////////////
//  class PlanzReader		                              											 //
//  Einlesen der XML-Struktur PlanzListe							                  		 //
///////////////////////////////////////////////////////////////////////////////
PlanzReader::PlanzReader ( PlanzListe * pPlanzListeP )
{
	pPlanzListe = pPlanzListeP;
	pReader     = nullptr;
}

PlanzReader::~ PlanzReader()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der XML-Datei                                                   //
///////////////////////////////////////////////////////////////////////////////
void PlanzReader::read ( std::string planzListePfad )
{
	String ^ fName;
	String ^ STR;
	string   stdString;
	int      ih;
	double   h;
	bool     first = true;
	Planz  * pPlanz;

	fName  = QuConvert::ToString ( planzListePfad );
	pReader = gcnew XmlTextReader( fName );

	while( pReader->Read() )
	{
		if ( pReader->NodeType == XmlNodeType::Element )
		{
			pActElement = pReader->LocalName;
			if ( first )
			{
				first = false;
				if ( pActElement->CompareTo("PlanzListe") != 0 )
				{
					MessageBox::Show( "Ungültige Steuerdatei für Planzeichen-Liste");
					break;
				}
				else
				{
          STR = pReader->GetAttribute ( "gmlTyp" );
          if ( STR != nullptr )
          {
            if ( STR->CompareTo( "XPLANGML_V2") == 0 )
              pPlanzListe->setGmlTyp( XPLANGML_2_0 );
            else
            if ( STR->CompareTo( "XPLANGML_V3") == 0 )
              pPlanzListe->setGmlTyp( XPLANGML_3_0 );
            else
            if ( STR->CompareTo( "XPLANGML_V4") == 0 )
              pPlanzListe->setGmlTyp( XPLANGML_4_0 );
            else
            if ( STR->CompareTo( "XPLANGML_V4_1") == 0 )
              pPlanzListe->setGmlTyp( XPLANGML_4_1 );
            else
            if ( STR->CompareTo( "XPLANGML_V5_0") == 0 )
              pPlanzListe->setGmlTyp( XPLANGML_5_0 );
            else
            if ( STR->CompareTo( "XPLANGML_V5_1") == 0 )
              pPlanzListe->setGmlTyp( XPLANGML_5_1 );
            else
            if ( STR->CompareTo( "XPLANGML_V5_2") == 0 )
              pPlanzListe->setGmlTyp( XPLANGML_5_2 );
            else
            if ( STR->CompareTo( "INSPIRE_PLU") == 0 )
              pPlanzListe->setGmlTyp( INSPIRE_LU_PLANNED );          
          }

					STR = pReader->GetAttribute ( "ausgabePfad" );
					if ( STR != nullptr )
					{
						QuConvert::systemStr2stdStr ( stdString, STR );
						pPlanzListe->setAusgabePfad ( stdString );
					}

					STR = pReader->GetAttribute ( "pngFolder" );
					if ( STR != nullptr )
					{
						QuConvert::systemStr2stdStr ( stdString, STR );
						pPlanzListe->setPngFolder ( stdString );
					}

					STR = pReader->GetAttribute ( "svgFolder" );
					if ( STR != nullptr )
					{
						QuConvert::systemStr2stdStr ( stdString, STR );
						pPlanzListe->setSvgFolder ( stdString );
					}

					STR = pReader->GetAttribute ( "indexDateiName" );
					if ( STR != nullptr )
					{
						QuConvert::systemStr2stdStr ( stdString, STR );
						pPlanzListe->setIndexDateiName ( stdString );
					}

					STR = pReader->GetAttribute ( "sizeX" );
					if ( STR != nullptr )
					{
						h = XmlConvert::ToDouble ( STR );
						pPlanzListe->setSizeX ( h );
					}

					STR = pReader->GetAttribute ( "sizeY" );
					if ( STR != nullptr )
					{
						h = XmlConvert::ToDouble ( STR );
						pPlanzListe->setSizeY ( h );
					}

					STR = pReader->GetAttribute ( "pixelX" );
					if ( STR != nullptr )
					{
						ih = XmlConvert::ToInt32 ( STR );
						pPlanzListe->setPixelX ( ih );
					}

					STR = pReader->GetAttribute ( "pixelY" );
					if ( STR != nullptr )
					{
						ih = XmlConvert::ToInt32 ( STR );
						pPlanzListe->setPixelY ( ih );
					}
				
				}
			}

			if ( pActElement->CompareTo("Planz") == 0 )
			{
				pPlanz = readPlanz();
				if ( pPlanz != NULL )
					pPlanzListe->addPlanz( pPlanz );
			}
		}
		else
		{
			if ( pReader->NodeType == XmlNodeType::EndElement )
			{
				pActElement = pReader->LocalName;
				if ( pActElement->CompareTo("PlanzListe") == 0 ) break;
			}
		}
	}

	pReader->Close();
}

///////////////////////////////////////////////////////////////////////////////
//   Einlesen der Parameter eines einzelnen Planzeichens           					 //
///////////////////////////////////////////////////////////////////////////////
Planz * PlanzReader::readPlanz()
{
	String ^ STR;
	string   stdString;
	double   h;
	PlanzFlaechenDarst * pFlaechenDarst;
	PlanzLinienDarst   * pLinienDarst;
	PlanzPunktDarst    * pPunktDarst;

	Planz  * pPlanz = new Planz ( pPlanzListe );

	STR = pReader->GetAttribute ( "gmlClass" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceKuerzel ( stdString );
		pPlanz->setGmlClass ( stdString );
	}

	STR = pReader->GetAttribute ( "selectAttribut1" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceKuerzel ( stdString );
		pPlanz->setSelectAttribut1 ( stdString );
	}

	STR = pReader->GetAttribute ( "selectAttribut2" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceKuerzel ( stdString );
		pPlanz->setSelectAttribut2 ( stdString );
	}

	STR = pReader->GetAttribute ( "selectAttribut3" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceKuerzel ( stdString );
		pPlanz->setSelectAttribut3 ( stdString );
	}

	STR = pReader->GetAttribute ( "symbolAttribut" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
    stdString = addDefaultNamespaceKuerzel ( stdString );
		pPlanz->setSymbolAttribut ( stdString );
	}

	STR = pReader->GetAttribute ( "sizeX" );
	if ( STR != nullptr )
	{
		h = XmlConvert::ToDouble ( STR );
		pPlanz->setSizeX ( h );
	}

	STR = pReader->GetAttribute ( "sizeY" );
	if ( STR != nullptr )
	{
		h = XmlConvert::ToDouble ( STR );
		pPlanz->setSizeY ( h );
	}

	while( pReader->Read() )
	{
		if ( pReader->NodeType == XmlNodeType::Element )
		{
		    pActElement = pReader->LocalName;
			if ( pActElement->CompareTo("PlanzFlaechenDarst") == 0 )
			{
				pFlaechenDarst = readPlanzFlaechenDarst( pPlanz );
				pPlanz->addFlaechenDarst ( pFlaechenDarst );
			}
			else
			if ( pActElement->CompareTo("PlanzLinienDarst") == 0 )
			{
				pLinienDarst = readPlanzLinienDarst( pPlanz );
				pPlanz->addLinienDarst ( pLinienDarst );
			}
			else
			if ( pActElement->CompareTo("PlanzPunktDarst") == 0 )
			{
				pPunktDarst = readPlanzPunktDarst( pPlanz );
				pPlanz->addPunktDarst ( pPunktDarst );
			}
		}
		else
		{
			if ( pReader->NodeType == XmlNodeType::EndElement )
			{
				pActElement = pReader->LocalName;
				if ( pActElement->CompareTo("Planz") == 0 ) break;
			}
		}

	}

	return pPlanz;
}

///////////////////////////////////////////////////////////////////////////////
//  Lesen der Parameter eines flächenhaften Planzeichens         						 //
///////////////////////////////////////////////////////////////////////////////
PlanzFlaechenDarst * PlanzReader::readPlanzFlaechenDarst( Planz * pPlanz )
{
	String ^ STR;
	string   stdString;
	double   h;

	PlanzFlaechenDarst * pDarst = new PlanzFlaechenDarst ( pPlanz );

	STR = pReader->GetAttribute ( "selectAttribut1Wert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSelectAttribut1Wert ( stdString );
	}

	STR = pReader->GetAttribute ( "selectAttribut2Wert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSelectAttribut2Wert ( stdString );
	}

	STR = pReader->GetAttribute ( "selectAttribut3Wert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSelectAttribut3Wert ( stdString );
	}

	STR = pReader->GetAttribute ( "symbolAttributWert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSymbolAttributWert ( stdString );
	}

	STR = pReader->GetAttribute ( "symbolScale" );
	if ( STR != nullptr )
	{
		h = XmlConvert::ToDouble ( STR );
		pDarst->setSymbolScale ( h );
	}

	return pDarst;
}

///////////////////////////////////////////////////////////////////////////////
//  Lesen der Parameter eines linienhaften Planzeichens         						 //
///////////////////////////////////////////////////////////////////////////////
PlanzLinienDarst   * PlanzReader::readPlanzLinienDarst( Planz * pPlanz )
{
	String ^ STR;
	string   stdString;
	double   h;

	PlanzLinienDarst * pDarst = new PlanzLinienDarst ( pPlanz );

	STR = pReader->GetAttribute ( "selectAttribut1Wert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSelectAttribut1Wert ( stdString );
	}

	STR = pReader->GetAttribute ( "selectAttribut2Wert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSelectAttribut2Wert ( stdString );
	}

	STR = pReader->GetAttribute ( "selectAttribut3Wert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSelectAttribut3Wert ( stdString );
	}
	
	STR = pReader->GetAttribute ( "symbolAttributWert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSymbolAttributWert ( stdString );
	}
	
	STR = pReader->GetAttribute ( "symbolScale" );
	if ( STR != nullptr )
	{
		h = XmlConvert::ToDouble ( STR );
		pDarst->setSymbolScale ( h );
	}
	return pDarst;
}

///////////////////////////////////////////////////////////////////////////////
//  Lesen der Parameter eines unkktförmigen Planzeichens         						 //
///////////////////////////////////////////////////////////////////////////////
PlanzPunktDarst    * PlanzReader::readPlanzPunktDarst( Planz * pPlanz )
{
	String ^ STR;
	string   stdString;
	double   h;

	PlanzPunktDarst * pDarst = new PlanzPunktDarst ( pPlanz );

	STR = pReader->GetAttribute ( "selectAttribut1Wert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSelectAttribut1Wert ( stdString );
	}

	STR = pReader->GetAttribute ( "selectAttribut2Wert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSelectAttribut2Wert ( stdString );
	}

	STR = pReader->GetAttribute ( "selectAttribut3Wert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSelectAttribut3Wert ( stdString );
	}
	
	STR = pReader->GetAttribute ( "symbolAttributWert" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pDarst->setSymbolAttributWert ( stdString );
	}

	STR = pReader->GetAttribute ( "symbolScale" );
	if ( STR != nullptr )
	{
		h = XmlConvert::ToDouble ( STR );
		pDarst->setSymbolScale ( h );
	}

	return pDarst;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt im Bedarffall des Kürzel xplan: hinzu                               //
///////////////////////////////////////////////////////////////////////////////
std::string PlanzReader::addDefaultNamespaceKuerzel ( std::string name )
{
	size_t index;

  index = name.find ( ":" );
  if ( index != string::npos )
    return name;

  name = "xplan:" + name;
  return name;
}

///////////////////////////////////////////////////////////////////////////////
//  class PlanzListeGeneration								                      				 //
//  Generierung einer Planzeichen-Liste aus den								               //
//  SVG-Visualisierungsparametern									                           //
///////////////////////////////////////////////////////////////////////////////
PlanzListeGeneration::PlanzListeGeneration ( SvgVisualisierung * pSvgParamsP )
{
	pSvgParams = pSvgParamsP;
}

PlanzListeGeneration::~ PlanzListeGeneration()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die Planzeichen-Liste                                          //
///////////////////////////////////////////////////////////////////////////////
void PlanzListeGeneration::write ( std::string fileNameP )
{
	String ^ fileName;
	String ^ STR;
	String ^ date;
	String ^ comment;
	String ^ DARST_STRING;
	string   stdString;
	string   schemaPfad;
	string   ausgabePfad;
	string   selectAttribut1Wert;
	string   selectAttribut2Wert;
	string   selectAttribut3Wert;
	size_t	 i, j, k1, k2, k3, visualAnz, darstAnz, selectAttributWert1Anz, selectAttributWert2Anz, selectAttributWert3Anz;

	vector<GmlKlassenVisual*>  vVisual;
	vector<Darstellung*>       vDarstellungen;
	vector<string>             vSelectAttribut1Wert;
	vector<string>             vSelectAttribut2Wert;
	vector<string>             vSelectAttribut3Wert;

	GmlKlassenVisual * pGmlKlassenVisual;
	Darstellung      * pDarst;

	fileName = QuConvert::ToString ( fileNameP );

	System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();
	writer								            = gcnew XmlTextWriter( fileName, utf8);

	writer->Formatting = Formatting::Indented;
	writer->WriteStartDocument( true );

	comment = "Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: ";
	comment = String::Concat ( comment, date );

	writer->WriteComment( comment );

  writer->WriteStartElement("PlanzListe");
	writer->WriteAttributeString("xmlns:xsi",	"http://www.w3.org/2001/XMLSchema-instance");

	visualAnz = pSvgParams->getGmlKlassenVisualAll ( vVisual );
	for ( i = 0; i < visualAnz; i++ )
	{
		pGmlKlassenVisual = vVisual[i];
		writer->WriteStartElement ( "Planz" );

		stdString = pGmlKlassenVisual->getGmlKlassenName();
		STR = QuConvert::ToString ( stdString );
		writer->WriteAttributeString ( "gmlClass", STR );

		stdString = pGmlKlassenVisual->getSelectAttribut1();
		if ( stdString != "" )
		{
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "selectAttribut1", STR );
		}

		stdString = pGmlKlassenVisual->getSelectAttribut2();
		if ( stdString != "" )
		{
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "selectAttribut2", STR );
		}

		stdString = pGmlKlassenVisual->getSelectAttribut3();
		if ( stdString != "" )
		{
			STR = QuConvert::ToString ( stdString );
			writer->WriteAttributeString ( "selectAttribut3", STR );
		}

		vDarstellungen.clear();
		darstAnz = pGmlKlassenVisual->getDarstellungen ( vDarstellungen );
		for ( j = 0; j < darstAnz; j++ )
		{
			pDarst = vDarstellungen[j];
			if ( pDarst->getDarstellungTyp() == Darstellung::FLAECHE )
				DARST_STRING = "PlanzFlaechenDarst";
			else
			if ( pDarst->getDarstellungTyp() == Darstellung::LINIE )
				DARST_STRING = "PlanzLinienDarst";
			else
				DARST_STRING = "PlanzPunktDarst";

			vSelectAttribut1Wert.clear();
			vSelectAttribut2Wert.clear();

			selectAttributWert1Anz = pDarst->getSelectAttribut1Werte( vSelectAttribut1Wert );
			selectAttributWert2Anz = pDarst->getSelectAttribut2Werte( vSelectAttribut2Wert );
			selectAttributWert3Anz = pDarst->getSelectAttribut3Werte( vSelectAttribut3Wert );

			if ( selectAttributWert1Anz == 0 )
			{
				writer->WriteStartElement ( DARST_STRING );
				writer->WriteEndElement();
			}

			for ( k1 = 0; k1 < selectAttributWert1Anz; k1++ )
			{
				selectAttribut1Wert = vSelectAttribut1Wert[k1];
				if ( selectAttributWert2Anz == 0 )
				{
					writer->WriteStartElement ( DARST_STRING );
					writer->WriteAttributeString 
						( "selectAttribut1Wert", QuConvert::ToString ( selectAttribut1Wert ) );
					writer->WriteEndElement();
				}
				for ( k2 = 0; k2 < selectAttributWert2Anz; k2++ )
				{
					selectAttribut2Wert = vSelectAttribut2Wert[k2];
					if ( selectAttributWert3Anz == 0 )
					{
  					writer->WriteStartElement ( DARST_STRING );
	  				writer->WriteAttributeString 
		  				( "selectAttribut1Wert", QuConvert::ToString ( selectAttribut1Wert ) );
			  		writer->WriteAttributeString 
				  		( "selectAttribut2Wert", QuConvert::ToString ( selectAttribut2Wert ) );
					  writer->WriteEndElement();
          }
          else
          {
  					for ( k3 = 0; k3 < selectAttributWert3Anz; k3++ )
	  				{
		  				selectAttribut3Wert = vSelectAttribut3Wert[k3];
			  			writer->WriteStartElement ( DARST_STRING );
				  		writer->WriteAttributeString 
					  		( "selectAttribut1Wert", QuConvert::ToString ( selectAttribut1Wert ) );
						  writer->WriteAttributeString 
							  ( "selectAttribut2Wert", QuConvert::ToString ( selectAttribut2Wert ) );
						  writer->WriteAttributeString 
							  ( "selectAttribut3Wert", QuConvert::ToString ( selectAttribut3Wert ) );
						  writer->WriteEndElement();
					  }
          }
				}
			}
		}

		writer->WriteEndElement();
	}
	writer->WriteEndElement();

	writer->Close();
}











