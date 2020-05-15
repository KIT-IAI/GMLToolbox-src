#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>
#include <fstream>

#include "includes.h"

class Darstellung;
class FlaechenDarstellung;
class LinienDarstellung;
class PunktDarstellung;
class FlaechenParams;
class LinienParams;
class LineTextParams;
class LineSymbolParams;
class Feature;
class SvgEnumeration;
class Planz;
class PlanzFlaechenDarst;
class PlanzLinienDarst;
class PlanzPunktDarst;
class ErrorReport;
class SvgVisualisierung;
class GmlSchema;
class Visualisierung_2D;

///////////////////////////////////////////////////////////////////////////////
//  class GmlKlassenVisual								                        					 //
//  Basisklasse für die SVG-Visualisierungs Parameter einer GML Klasse       //
///////////////////////////////////////////////////////////////////////////////
class GmlKlassenVisual
{
public:
	GmlKlassenVisual ( SvgVisualisierung * pSvgParamsP );
	virtual ~ GmlKlassenVisual();

private:
	SvgVisualisierung * pSvgParams;                   // Sammlung der SVG-Darstellungsparameter
  std::string         gmlKlassenName;               // Name der GML-Klasse
	int			            priorityFlaechendarstellung;  // Priorität bei flächenhafter Darstellung
	int			            priorityLiniendarstellung;    // Priorotät bei linienhafter Darstellung
	int			            priorityPunktdarstellung;     // Prioriät bei punktförmiger Darstellung
	std::string         selectAttribut1;              // Name des 1. Auswahlattributs
	std::string         selectAttribut2;              // Name des 2. Auswahlattributs
	std::string         selectAttribut3;              // Name des 3. Auswahlattributs
	std::string         defaultSymbol;                // Standard-Symbol
	double              defaultSymbolSize;            // Standard-Größe eines Symbols

	std::vector<FlaechenDarstellung*> vFlaechenDarstellungen; // Parameter der flächenhaften Darstellungen
	std::vector<LinienDarstellung*>	  vLinienDarstellungen;   // Parameter der linienhaften Darstellungen
	std::vector<PunktDarstellung*>    vPunktDarstellungen;    // Parameter der punktförmigen Darstellungen

public:
	void setGmlKlassenName ( std::string klassenNameP )  { gmlKlassenName = klassenNameP;  }
	std::string getGmlKlassenName()  {  return gmlKlassenName;  }

	void setPriorityFlaechendarstellung ( int prioP )  {  priorityFlaechendarstellung = prioP; }
	int  getPriorityFlaechendarstellung()  {  return priorityFlaechendarstellung;  }

	void setPriorityLiniendarstellung ( int prioP )  {  priorityLiniendarstellung = prioP; }
	int  getPriorityLiniendarstellung()  {  return priorityLiniendarstellung;  }

	void setPriorityPunktdarstellung ( int prioP )  {  priorityPunktdarstellung = prioP; }
	int  getPriorityPunktdarstellung()  {  return priorityPunktdarstellung;  }

	int getPriority ( GEOMETRY_TYPE geometrieTypP );
  int getPriority ( Feature * pPlanObjekt );

	void setSelectAttribut1 ( std::string attr )  {  selectAttribut1 = attr;  }
  std::string getSelectAttribut1 ()  {  return selectAttribut1;  }

	void setSelectAttribut2 ( std::string attr )  {  selectAttribut2 = attr;  }
  std::string getSelectAttribut2 ()  {  return selectAttribut2;  }

	void setSelectAttribut3 ( std::string attr )  {  selectAttribut3 = attr;  }
  std::string getSelectAttribut3 ()  {  return selectAttribut3;  }

	void setDefaultSymbol ( std::string attr )  {  defaultSymbol = attr;  }
  std::string getDefaultSymbol ()  {  return defaultSymbol;  }

	void setDefaultSymbolSize ( double symbolSizeP )  {  defaultSymbolSize = symbolSizeP;  }
	double getDefaultSymbolSize()  {  return defaultSymbolSize;  }

	void addDarstellung ( Darstellung * pDarstellungP );

	size_t getFlaechenDarstellungen ( std::vector<Darstellung*> &vFaechenDarstellungenP );
	size_t getLinienDarstellungen ( std::vector<Darstellung*> &vLinienDarstellungenP );
	size_t getPunktDarstellungen ( std::vector<Darstellung*> &vPunktDarstellungenP );
	size_t getDarstellungen ( std::vector<Darstellung*> &vDarstellungenP );

  Darstellung * getDarstellungSelected ( Feature * pPlanObcekt );
};

///////////////////////////////////////////////////////////////////////////////
//  class Darstellung		                            												 //
//  Basisklasse für die SVG-Visualisierungsparameter Klassen	        			 //
///////////////////////////////////////////////////////////////////////////////
class Darstellung
{
public:
	enum DARSTELLUNG {  PUNKT, LINIE, FLAECHE  };
public:
	Darstellung();
	virtual ~Darstellung();

private:
  std::string              id;                    // Id des parametersatzes
	bool		                 defaultValue;          // Gibt an, ob es sich um Default-Darstellungsparameter handelt
  int                      priority;              // Steuerung der Zeihenreihenfolge
  std::vector<std::string> vSelectAttributWert1;  // Selektions-Werte des ersten Attributs
	std::vector<std::string> vSelectAttributWert2;  // Selektions-Werte des zweiten Attributs
	std::vector<std::string> vSelectAttributWert3;  // Selektions-Werte des dritten Attributs

public:
	virtual DARSTELLUNG getDarstellungTyp() = NULL;

  void setPriority ( int prio )  {  priority = prio; }
  int  getPriority()  {  return priority;  }

  void setId ( std::string idP )  {  id = idP;  }
  std::string getId ()  {  return id;  }

	void addSelectAttributWert1 ( std::string attr )  
	{  vSelectAttributWert1.push_back ( attr );  }

	void addSelectAttributWert2 ( std::string attr )
	{  vSelectAttributWert2.push_back ( attr );  }

	void addSelectAttributWert3 ( std::string attr )
	{  vSelectAttributWert3.push_back ( attr );  }

	void setDefault ( bool b )  {  defaultValue = b;  }
	bool getDefault ()  {  return defaultValue;  }

	bool isSelected ( std::string selectAttributWert1P, std::string selectAttributWert2P, 
					  std::string selectAttributWert3P );

	size_t getSelectAttribut1Werte ( std::vector<std::string> & vSelectAttribut1WertP );
	size_t getSelectAttribut2Werte ( std::vector<std::string> & vSelectAttribut2WertP );
	size_t getSelectAttribut3Werte ( std::vector<std::string> & vSelectAttribut3WertP );

};

///////////////////////////////////////////////////////////////////////////////
//  class FlaechenDarstellung									                        			 //
//  Visualisierungs-Parameter für flächenhafte Darstellung     							 //
///////////////////////////////////////////////////////////////////////////////
class FlaechenDarstellung : public Darstellung
{
public:
	FlaechenDarstellung();
	virtual ~FlaechenDarstellung();

private:
	bool						             deckend;           // Gibt an, ob die Darstellung flächendeckend ist (SVG mask aktiviert)
	std::vector<FlaechenParams*> vFlaechenParams;   // Parameter flächenhafte Darstellung
	LineTextParams			       * pLineTextParams;   // Parameter für Randsignaturen auf Basis von Texten
	LineSymbolParams		       * pLineSymbolParams; // Parameter für Randsignaturen auf Basis von Symbolen

public:
	virtual DARSTELLUNG getDarstellungTyp()  {  return FLAECHE;  }

	void setDeckend ( bool b )  {  deckend = b;  }
	bool getDeckend()  {  return deckend;  }

	void addFlaechenParams ( FlaechenParams * pFlaechenParamsP )  {  vFlaechenParams.push_back ( pFlaechenParamsP );  }
	void setLineTextParams ( LineTextParams * pLineTextParamsP )  { pLineTextParams = pLineTextParamsP;  } 
	void setLineSymbolParams ( LineSymbolParams * pParamsP )  { pLineSymbolParams = pParamsP;  } 

	size_t getFlaechenParams ( std::vector<FlaechenParams*> & vFlaechenParamsP );
	LineTextParams   * getLineTextParams ()  {  return pLineTextParams;  }
	LineSymbolParams * getLineSymbolParams () {  return pLineSymbolParams;  }
};


///////////////////////////////////////////////////////////////////////////////
//  class LinienDarstellung									                        				 //
//  Visualisierungs-Parameter einer Linie						                  			 //
///////////////////////////////////////////////////////////////////////////////
class LinienDarstellung : public Darstellung
{
public:
	LinienDarstellung();
	virtual ~LinienDarstellung();

private:
	std::vector<LinienParams*> vLinienParams;      // Parameter linienhafte Darstellung
	LineTextParams		    	 * pLineTextParams;    // Liniensignaturen auf Basis von Texten
	LineSymbolParams		     * pLineSymbolParams;  // Liniensignaturen auf Basis von Symbolen
	std::string				         startMarker;        // Marker für den Startpunkt der Linie
	std::string		      		   endMarker;          // Marker für den Endpunkt der Linie

public:
	virtual DARSTELLUNG getDarstellungTyp()  {  return LINIE;  }

	void addLinienParams ( LinienParams * pLinienParamsP )  {  vLinienParams.push_back ( pLinienParamsP );  }
	void setLineTextParams ( LineTextParams * pLineTextParamsP )  { pLineTextParams = pLineTextParamsP;  } 
	void setLineSymbolParams ( LineSymbolParams * pParamsP )  { pLineSymbolParams = pParamsP;  } 
	void setStartMarker ( std::string  markerP )  {  startMarker = markerP;  }
	void setEndMarker ( std::string  markerP )  {  endMarker = markerP;  }

	size_t getLinienParams ( std::vector<LinienParams*> & vLinienParamsP );
	LineTextParams   * getLineTextParams ()  {  return pLineTextParams;  }
	LineSymbolParams * getLineSymbolParams () {  return pLineSymbolParams;  }
	std::string		   getStartMarker ()  {  return startMarker;  }
	std::string		   getEndMarker ()  {  return endMarker;  }

};

///////////////////////////////////////////////////////////////////////////////
//  class PunktDarstellung										                        			 //
//  Visualisierungs-Parameter eines Punktes					                				 //
///////////////////////////////////////////////////////////////////////////////
class PunktDarstellung: public Darstellung
{
public:
	PunktDarstellung();
	virtual ~PunktDarstellung();

private:
	std::string attributA;        // Name des 1. Selektionsattributs
	std::string attributB;        // Name des 2. Selektionsattributs
	std::string textStyle;        // Text-Stil
	std::string angleAttribute;   // Name des Attributs zur den Drehwinkel des Textes / ybbols
	std::string symbol;           // Id des zugeordneten Symbols
	double      symbolSize;       // Skalierung des Symbol
	double      textOffsetX;      // Text-Offset X-Richtung
	double		  textOffsetY;      // Text-Offset Y-Richtung

public:
	virtual DARSTELLUNG getDarstellungTyp()  {  return PUNKT;  }

	void setAttributA ( std::string attrP )  {  attributA = attrP;  }
	std::string getAttributA ()  {  return attributA;  }

	void setAttributB ( std::string attrP )  {  attributB = attrP;  }
	std::string getAttributB ()  {  return attributB;  }

	void setTextStyle ( std::string styleP )  {  textStyle = styleP;  }
	std::string getTextStyle ()  {  return textStyle;  }

	void setAngleAttribute ( std::string attr )  {  angleAttribute = attr;  }
	std::string getAngleAttribute()  {  return angleAttribute;  }

	void setSymbol ( std::string attr )  {  symbol = attr;  }
	std::string getSymbol ()  {  return symbol;  }

	void setSymbolSize ( double sizeP )  {  symbolSize = sizeP;  }
	double getSymbolSize()  {  return symbolSize;  }

	void setTextOffsetX ( double h )  {  textOffsetX = h;  }
	double getTextOffsetX()  {  return textOffsetX;  }

	void setTextOffsetY ( double h )  {  textOffsetY = h;  }
	double getTextOffsetY()  {  return textOffsetY;  }
};

///////////////////////////////////////////////////////////////////////////////
//  class FlaechenParams						                          							 //
//  Klasse für die SVG-Visualisierungs Parameter einer Fläche			        	 //
///////////////////////////////////////////////////////////////////////////////
class FlaechenParams
{
public:
	FlaechenParams ();
	virtual ~ FlaechenParams();

private:
	std::string classId;    // Id des zugehörigen Style Objektes
	std::string patternId;  // Id eines zugehörigen Pattern Objektes
	std::string hatchingId; // Id eines zugehörigen Hatching Objektes
	bool		    clip;       // Steuert, of die Fläche ausgeschnitten wird

public:
	void setClassId ( std::string id )  {  classId = id;  }
	std::string getClassId ()  {  return classId;  }

	std::string getPatternId()  {  return patternId;  }
	void setPatternId ( std::string patternIdP )  {  patternId = patternIdP;  }

	std::string getHatchingId()  {  return hatchingId;  }
	void setHatchingId ( std::string hatchingIdP )  {  hatchingId = hatchingIdP;  }

	bool getClip ()  {  return clip;  }
	void setClip ( bool clipP )  {  clip = clipP;  }
};


///////////////////////////////////////////////////////////////////////////////
//  class LinienParams					                            								 //
//  Klasse für die svg-Visualisierungs Parameter einer Linie        				 //
///////////////////////////////////////////////////////////////////////////////
class LinienParams
{
public:
	LinienParams ();
	virtual ~ LinienParams();

private:
	std::string classId;      // Id des zug. Style Objektes

public:
	void setClassId ( std::string id )  {  classId = id;  }
	std::string getClassId ()  {  return classId;  }
};


///////////////////////////////////////////////////////////////////////////////
//  class LineTextParams				                          									 //
//  Klasse für die SVG-Visualisierungs Parameter einer				          		 //
//  Linie mit Hilfe eines Textpfades									                    	 //
///////////////////////////////////////////////////////////////////////////////
class LineTextParams
{
public:
	LineTextParams ();
	virtual ~ LineTextParams();

private:
	std::string classId;      // Id des zug. Style-Objektes
	std::string textString;   // Text
	double		  charSize;     // Größe der Character in der Textdarstellung
	bool        isWideChar;   // Steuert, ob textString oder textChar benutzt wird
	__wchar_t   textChar;     // Text-Character

public:
	void setClassId ( std::string classIdP )  {  classId = classIdP;  }
	std::string getClassId ()  {  return classId;  }

	void setTextString ( std::string textStringP )  {  textString = textStringP;  }
	std::string getTextString()  {  return textString;  }

	void setCharSize ( double charSizeP )  {  charSize = charSizeP;  }
	double getCharSize ()  {  return charSize;  }

	void setIsWideChar ( bool b )  {  isWideChar = b;  }
	bool getIsWideChar ()  {  return isWideChar;  }

	void setTextChar ( __wchar_t textCharP )  {  textChar = textCharP;  }
	__wchar_t getTextChar ()  {  return textChar;  }
};

///////////////////////////////////////////////////////////////////////////////
//  class LineSymbolParams									                        				 //
//  Klasse für die svg-Visualisierungs Parameter  einer	Linie		        		 //
//  mit Hilfe von Symbolen								                        					 //
///////////////////////////////////////////////////////////////////////////////
class LineSymbolParams
{
public:
	LineSymbolParams ();
	virtual ~LineSymbolParams();

private:
	std::string symbolId;     // Id des zug. Symbols
	double      symbolSize;   // Skalierung des Symbols
	double		  symbolDist;   // Abstand der Symbole in der Rand-/Linien-Signatir
	double		  symbolAngle;  // Drehwinkel des Symbols

public:
	void setSymbolId ( std::string id )  {  symbolId = id;  }
	std::string getSymbolId ()  {  return symbolId;  }

	void setSymbolSize ( double size )  {  symbolSize = size;  }
	double getSymbolSize()  {  return symbolSize;  }

	void setSymbolDist ( double dist )  {  symbolDist = dist;  }
	double getSymbolDist ()  {  return symbolDist;  }

	void setSymbolAngle ( double angle )  {  symbolAngle = angle;  }
	double getSymbolAngle ()  {  return symbolAngle;  }
};

///////////////////////////////////////////////////////////////////////////////
//  class EnumItem											                            				 //
// Zuordnung von GML Enumerations-Items zu Texten oder Symbolen              //
///////////////////////////////////////////////////////////////////////////////
class SvgEnumItem
{
public:
	SvgEnumItem( SvgEnumeration * pEnumerationP, std::string schluesselNummerA, std::string schluesselNummerB );
	virtual ~SvgEnumItem();

private:
	SvgEnumeration * pEnumeration;       // Zugeböriges Objekt SvgEnumeration
	std::string      schluesselNummer;   // Schlüsselnummer des GML Enumerations-Items
	std::string      symbolId;           // Id eines zugeordneten Symbols
	double           symbolSize;         // Skalierung eines zugeordneten Symbols
	bool             symbolSizeSet;      // Gibt an, ob die Symbol-Skalierung gesetzt wurde 
	std::string      _text;              // ZugeordneterText
	std::string      textStyle;          // Text-Stil
	std::string      textSymbol;         // Zusätzliches Symbol bei der Textdarstellung
	double           textSymbolSize;     // Skalierung des Textes und des Text-Symbols
  bool			       textSymbolSizeSet;  // Gibt an, ob das Attribut textSymbolSize gesetzt wurde

public:
	std::string getSchluesselNummer ()  {  return schluesselNummer;  }

	void setSymbolId ( std::string symbolIdP )  {  symbolId = symbolIdP;  }
	std::string getSymbolId ()  {  return symbolId;  }

	void setSymbolSize ( double symbolSizeP );  
	double getSymbolSize ();

	void setText ( std::string textP )  {  _text = textP;  }
	std::string getText ()  {  return _text;  }

	void setTextSymbol ( std::string textSymbolP )  {  textSymbol = textSymbolP;  }
	std::string getTextSymbol ();

	void setTextStyle ( std::string textStyleP )  {  textStyle = textStyleP;  }
	std::string getTextStyle ();

	void setTextSymbolSize ( double symbolSizeP );
	double getTextSymbolSize ();
};

///////////////////////////////////////////////////////////////////////////////
//  class SvgEnumeration			                          										 //
//  Darstellungsparameter für eine GML-Enumeration                           //
///////////////////////////////////////////////////////////////////////////////
class SvgEnumeration
{
public:
	SvgEnumeration ( std::string nameP );
	SvgEnumeration ( std::string nameA, std::string nameB );
	virtual ~ SvgEnumeration();

private:
	std::map<std::string,SvgEnumItem*> enumItems; // Darstekkungsparameter für einzelne Enumerations-Items

	std::string name;                   // Name der Enumeration
	std::string defaultTextSymbol;      // Standard Text-Symbol
	std::string defaultTextStyle;       // Standard Text.Stil
	double       defaultSymbolSize;     // Standard Symbol-Skalierung
	double       defaultTextSymbolSize; // Standard Text-Skalierung

public:
	std::string getName ()  { return name;  }

	void setDefaultTextSymbol ( std::string idP )  {  defaultTextSymbol = idP;  }
	std::string getDefaultTextSymbol ()  {  return defaultTextSymbol;  }

	void setDefaultTextStyle ( std::string idP )  {  defaultTextStyle = idP;  }
	std::string getDefaultTextStyle ()  {  return defaultTextStyle;  }

	void setDefaultSymbolSize ( double sizeP )  {  defaultSymbolSize = sizeP;  }
	double getDefaultSymbolSize()  {  return defaultSymbolSize;  }

	void setDefaultTextSymbolSize ( double sizeP )  {  defaultTextSymbolSize = sizeP;  }
	double getDefaultTextSymbolSize()  {  return defaultTextSymbolSize;  }

	void addEnumItem ( SvgEnumItem * pEnumItemP );
	SvgEnumItem * getEnumItem ( std::string schluesselNummerA, std::string schluesselNummerB );
};

///////////////////////////////////////////////////////////////////////////////
//  SVGColor - Farbe								                            						 //
///////////////////////////////////////////////////////////////////////////////
class SvgColor
{
public:
	enum COLOR_TYPE {  NONE, NAME, RGB  };

public:
	SvgColor ();
	SvgColor ( std::string colorNameP );
	SvgColor ( int rP, int gP, int bP );
	virtual ~ SvgColor();

private:
	COLOR_TYPE colorType;     // Typ der Farb-Spefikikation
	std::string colorName;    // Farb-Name
	int         rgb[3];       // RGB-Wert

public:
	COLOR_TYPE getColorType ()  {  return colorType;  }

	std::string getColorName ()  {  return colorName;  }

	void getColorRGB ( int &rP, int &gP, int &bP );
  std::string getColorStringRGB ();
};

///////////////////////////////////////////////////////////////////////////////
//  class Style								                              								 //
//  Klasse zur Modellierung von css-Styles					                 				 //
///////////////////////////////////////////////////////////////////////////////
class Style
{
public:
	Style ( std::string styleIdP );
	virtual ~ Style();

private:
	std::string   styleId;         // Id des Styles
	SvgColor    * pFillColor;      // Füllungsfarbe einer Fläche
	SvgColor    * pStrokeColor;    // Linienfarbe einer Linie oder Flächenkontur
	double		    fillOpacity;     // Durchsichtigkeit einer Flächenfüllung (zwichen 0 und 1)
	double		    strokeWidth;     // Breite einer Linie
	std::string	  strokeLinecap;   // Typ der Linienenden
	std::string   strokeDasharray;  // Linienmuster
	double		    strokeDashoffset; // Offset eines Linienmusters
	double		    fontSize;         // Fontgröße eines Textes
	std::string   fontWeight;       // Font-Gewicht
	std::string   fontFamily;       // Font-Bezeichnung
	std::string   textAnchor;       // Text-Amsatz
	double		    kerning;          // Abstand der Characters des Textes
  std::string		baselineShift;    // Verschiebung des Textes zur Basislinie

public:
	std::string getStyleId ()  {  return styleId;  }

	void setFillColor ( SvgColor * pColor )  {  pFillColor = pColor;  }
	SvgColor * getFillColor ()  {  return pFillColor;  }

	void setStrokeColor ( SvgColor * pColor )  {  pStrokeColor = pColor;  }
	SvgColor * getStrokeColor ()  {  return pStrokeColor;  }

	void setFillOpacity ( double wert )  {  fillOpacity = wert;  }
	double getFillOpacity ()  {  return fillOpacity;  }

	void setStrokeWidth ( double wert )  {  strokeWidth = wert;  }
	double getStrokeWidth ()  {  return strokeWidth;  }

	void setStrokeLinecap ( std::string wert )  {  strokeLinecap = wert;  }
	std::string getStrokeLinecap ()  {  return strokeLinecap;  }

	void setStrokeDasharray ( std::string wert )  {  strokeDasharray = wert;  }
	std::string getStrokeDasharray ()  {  return strokeDasharray;  }

	void setStrokeDashoffset ( double wert )  {  strokeDashoffset = wert;  }
	double getStrokeDashoffset ()  {  return strokeDashoffset;  }

	void setFontSize ( double wert )  {  fontSize = wert;  }
	double getFontSize ()  {  return fontSize;  }

	void setFontWeight ( std::string weight )  {  fontWeight = weight;  }
	std::string getFontWeight ()  {  return fontWeight;  }

	void setFontFamily ( std::string family )  {  fontFamily = family;  }
	std::string getFontFamily ()  {  return fontFamily;  }

	void setTextAnchor ( std::string anchor )  {  textAnchor = anchor;  }
	std::string getTextAnchor()  {  return textAnchor;  }

	void setKerning ( double wert )  {  kerning = wert;  }
	double getKerning ()  {  return kerning;  }

	void setBaselineShift ( std::string wert )  {  baselineShift = wert;  }
	std::string getBaselineShift ()  {  return baselineShift;  }

  std::string getStyleString();
};

///////////////////////////////////////////////////////////////////////////////
//  class SvgGeometry					                            									 //
//  Basisklasse für SVG Geometrieobjekte                                     //
///////////////////////////////////////////////////////////////////////////////
class SvgGeometry
{
public:
	enum SVG_GEOMETRY_TYPE  {  SVG_PATH, SVG_CIRCLE, SVG_RECT, SVG_TEXT  };

public:
	SvgGeometry ( std::string classIdP );
	virtual ~SvgGeometry();

private:
	std::string classId;        // Zugehöriges Style-Objekt
	SvgColor *	pFillColor;     // Flächenfüllungs-Farbe
	SvgColor *	pStrokeColor;   // Linienfarbe
	double		  strokeWidth;    // Linienstärke
	std::string strokeDasharray;// Linienmuster
  bool        fill;           // Gibt an, ab das Geometriepbjekt mit Flächenfüllung dargestellt werden soll

public:
	virtual SVG_GEOMETRY_TYPE getSvgGeometryType() = NULL;
	std::string getClassId ()  {  return classId;  }

  void setFill ( bool b )  {  fill = b;  }
  bool getFill()  {  return fill;  }

	void setFillColor ( SvgColor * pColor )  {  pFillColor = pColor; fill = true;  }
	SvgColor * getFillColor ()  {  return pFillColor;  }

	void setStrokeColor ( SvgColor * pColor )  {  pStrokeColor = pColor;  }
	SvgColor * getStrokeColor ()  {  return pStrokeColor;  }

	void setStrokeWidth ( double wert )  {  strokeWidth = wert;  }
	double getStrokeWidth ()  {  return strokeWidth;  }

  void setStrokeDasharray ( std::string wert )  {  strokeDasharray = wert;  }
	std::string getStrokeDasharray ()  {  return strokeDasharray;  }
};

///////////////////////////////////////////////////////////////////////////////
//  class SvgPath						                              									 //
//  Text, der entlang einer Linie dargestellt wird                           //
///////////////////////////////////////////////////////////////////////////////
class SvgPath : public SvgGeometry
{
public:
	SvgPath (  std::string classIdP );
	virtual ~SvgPath();

private:
	std::string pathString;   // Text

public:
	virtual SVG_GEOMETRY_TYPE getSvgGeometryType() {  return SVG_PATH;  }

	void setPathString ( std::string pathStringP )  {  pathString = pathStringP;  }
	std::string getPathString ()  {  return pathString;  }
};

///////////////////////////////////////////////////////////////////////////////
//  class SvgCircle				                            											 //
//  Kreis                                                                    //
///////////////////////////////////////////////////////////////////////////////
class SvgCircle : public SvgGeometry
{
public:
	SvgCircle (  std::string classIdP );
	virtual ~SvgCircle();

private:
	double cx;    // X-Koordinate Mittelpunkt
	double cy;    // y-Koordinate Mittelpunkt
	double r;     // Radius

public:
	virtual SVG_GEOMETRY_TYPE getSvgGeometryType() {  return SVG_CIRCLE;  }

	void setGeometry ( double cxP, double cyP, double rP );
	void getGeometry ( double &cxP, double &cyP, double &rP );
};

///////////////////////////////////////////////////////////////////////////////
//  class SvgRect						                              									 //
//  Rechteck                                                                 //
///////////////////////////////////////////////////////////////////////////////
class SvgRect : public SvgGeometry
{
public:
	SvgRect (  std::string classIdP );
	virtual ~SvgRect();

private:
	double x;
	double y;
	double width;
	double height;

public:
	virtual SVG_GEOMETRY_TYPE getSvgGeometryType() {  return SVG_RECT;  }

	void setGeometry ( double xP, double yP, double widthP, double heightP );
	void getGeometry ( double &xP, double &yP, double &widthP, double & heightP );
};

///////////////////////////////////////////////////////////////////////////////
//  class SvgText		                              													 //
//  Text, der an einer bestimmten Position dargestellt wird                  //
///////////////////////////////////////////////////////////////////////////////
class SvgText : public SvgGeometry
{
public:
	SvgText (  std::string classIdP );
	virtual ~SvgText();

private:
	double	    x;
	double		  y;
	double      size;
	std::string textString;

public:
	virtual SVG_GEOMETRY_TYPE getSvgGeometryType() {  return SVG_TEXT;  }

	void setPosition ( double xP, double yP );
	void getPosition ( double &xP, double &yP );

	void setTextSize ( double sizeP )  {  size = sizeP;  }
	double getTextSize () {  return size;  }
  String ^ getTextSizePoints();

	void setTextString ( std::string textStringP )  {  textString = textStringP;  }
	std::string getTextString ()  {  return textString;  }
};

///////////////////////////////////////////////////////////////////////////////
//  class Symbol					                              										 //
//  Symbol, das aus Geometrie-Elementen zusammengesetzt ist                  //
///////////////////////////////////////////////////////////////////////////////
class Symbol
{
public:
	Symbol ( std::string symbolIdP );
	virtual ~Symbol();

private:
	std::string               symbolId;           // Id des Symbols
	std::vector<SvgGeometry*> vGeometryElements;  // Geometrieelemente

public:
	std::string getSymbolId ()  {  return symbolId;  }

	void addGeometryElement ( SvgGeometry * pElement );
	SvgGeometry * getGeometryElement ( unsigned int index );
	size_t getGeometryElementAnz()  {  return vGeometryElements.size();  }
};

///////////////////////////////////////////////////////////////////////////////
//  class Marker			                                											 //
//  Symbole für den Anfang und das Ende einer Linie                          //
///////////////////////////////////////////////////////////////////////////////
class Marker
{
public:
	Marker ( std::string markerIdP );
	virtual ~ Marker();

private:
	std::string markerId; // Id des Markers
	std::string viewbox;  // View-Box des Markers
	double      refX;     // X-Koordinate des Marker-Referenzpunktes
	double      refY;     // Y-Koordinate des Marker-Referenzpunktes
	double      width;    // Breite des Marker-Viewports
	double      height;   // Höhe des Marker-Viewports

	std::vector<SvgGeometry*> vGeometryElements;  // Geoetrie-Elemente des Markers

public:
	std::string getMarkerId ()  { return markerId;  }

	void addGeometryElement ( SvgGeometry * pElement );
	SvgGeometry * getGeometryElement ( unsigned int index );
	size_t getGeometryElementAnz()  {  return vGeometryElements.size();  }

	void setViewbox ( std::string viewboxP )  {  viewbox = viewboxP;  }
	std::string getViewbox ()  { return viewbox;  }

	void setRefX ( double xP )  {  refX = xP;  }
	double getRefX ()  {  return refX;  }

	void setRefY ( double yP )  {  refY = yP;  }
	double getRefY ()  {  return refY;  }

	void setWidth ( double widthP )  {  width = widthP;  }
	double getWidth ()  {  return width;  }

	void setHeight ( double heightP )  {  height = heightP;  }
	double getHeight ()  {  return height;  }
};

///////////////////////////////////////////////////////////////////////////////
//  class Pattern														                              	 //
//  SVG Muster                                                               //
///////////////////////////////////////////////////////////////////////////////
class Pattern
{
public:
	Pattern ( std::string patternIdP );
	virtual ~Pattern();

private:
	std::string       patternId;  // Id des Musters
	double					  x;          // Horizotale Position des Muster-Rechtecks
	double					  y;          // Vertikale Position des Muster-Rechtecks
	double					  width;      // Breite des Muster-Rechtecks
	double					  height;     // Höhe des Muster-Rechtecks
	double					  angle;      // Rotationswinkel der Geometrie-Elemente
  double            scale;      // Skalierung der Geometrie-Elemente
	std::vector<SvgGeometry*> vGeometryElements; // Geometrie-Elemente des Musters

public:
	std::string getPatternId ()  {  return patternId;  }

	void setGeometry ( double xP, double yP, double widthP, double heightP );
	void getGeometry ( double &xP, double &yP, double &widthP, double &heightP );

	void setAngle ( double angleP )  {  angle = angleP;  }
	double getAngle ()  {  return angle;  }

  void setScale ( double scaleP )  { scale = scaleP; }
  double getScale ()  {  return scale;  }

	void addGeometryElement ( SvgGeometry * pElement );
	SvgGeometry * getGeometryElement ( unsigned int index );
	size_t getGeometryElementAnz()  {  return vGeometryElements.size();  }
};

///////////////////////////////////////////////////////////////////////////////
//  class Hatching										                            					 //
//  Schraffur                                                                //
///////////////////////////////////////////////////////////////////////////////
class Hatching
{
public:
	Hatching ( std::string id );
	virtual ~ Hatching();

private:
	std::string   hatchingId;     // Id der Schraffur
	double        strokeWidth;    // Linienstärke
	double        lineDist;       // Abstand der Schraffur-Linien
	SvgColor    * pLineColor;     // Linienfarbe
	SvgColor    * pBaseColor;     // Basisfarbe des schraffieten Bereiches
	std::string   hatchingType;   // Typ der Schraffur

public:
	std::string getHatchingId ()  {  return hatchingId;  }

	void setStrokeWidth ( double width )  {  strokeWidth = width;  }
	double getStrokeWidth ()  {  return strokeWidth;  }

	void setLineDist ( double dist )  {  lineDist = dist;  }
	double getLineDist ()  {  return lineDist;  }

	void setLineColor ( SvgColor * pColor )  {  pLineColor = pColor;  }
	SvgColor * getLineColor ()  {  return pLineColor;  }

	void setBaseColor ( SvgColor * pColor )  {  pBaseColor = pColor;  }
	SvgColor * getBaseColor ()  {  return pBaseColor;  }

	void setHatchingType ( std::string type )  {  hatchingType = type;  }
	std::string getHatchingType ()  {  return hatchingType;  }
};


///////////////////////////////////////////////////////////////////////////////
//  class SvgVisualisierung												                        	 //
//  Zusammenfassung aller SVG-Darstellungsparameter                          //
///////////////////////////////////////////////////////////////////////////////
class SvgVisualisierung
{
public:
	SvgVisualisierung ();
	virtual ~SvgVisualisierung();

private:
  GML_SCHEMA_TYPE gmlTyp;       // GML-Typ
	std::string     planTyp;      // Bei XPlanGML: Plan-Typ
	std::string     datum;        // Datum der Parameterdatei

	int    breiteUeberschrift;    // Breite der Überschrift
	int    hoeheUeberschrift;     // Höhe der Überschrift
  double breiteDisplay;         // Breite der Plandarstellung
  double hoeheDisplay;          // Höhe der Plandarstellung

	std::map<std::string,GmlKlassenVisual*>    mGmlKlassen;     // Parameter der einzelnen GML-Klassen
	std::map<std::string,Style*>		           mStyles;         // CSS-Styles
	std::map<std::string,Symbol*>              mSymbols;        // Symbole
	std::map<std::string,Marker*>              mMarkers;        // Marker
	std::map<std::string,SvgEnumeration*>      mEnumerations;   // GML-Enumeratinen
	std::map<std::string,Pattern*>             mPatterns;       // Muster
	std::map<std::string,Hatching*>            mHatchings;      // Schraffuren
  std::map<std::string,FlaechenDarstellung*> mFlaechenStyles; // Flächendarstellungen
  std::map<std::string,LinienDarstellung*>   mLinienStyles;   // Liniendarstellungeb

public:
  void setGmlTyp ( GML_SCHEMA_TYPE gmlTypP )  {  gmlTyp = gmlTypP;  }
  GML_SCHEMA_TYPE getGmlTyp()  {  return gmlTyp;  }

	void setPlanTyp ( std::string planTypP )  {  planTyp = planTypP;  }
	std::string getPlanTyp ()  { return planTyp;  }

	void setDatum ( std::string datumP )  {  datum = datumP;  }
	std::string getDatum ()  {  return datum;  }

	void setBreiteUeberschrift ( int breite )  {  breiteUeberschrift = breite;  }
	int getBreiteUeberschrift ()  {  return breiteUeberschrift;  }

	void setHoeheUeberschrift ( int hoehe )  {  hoeheUeberschrift = hoehe;  }
	int getHoeheUeberschrift ()  {  return hoeheUeberschrift;  }

  void setBreiteDisplay ( double b )  {  breiteDisplay = b;  }
  double getBreiteDisplay()  {  return breiteDisplay;  }

  void setHoeheDisplay( double h )  {  hoeheDisplay = h; }
  double getHoeheDisplay()  {  return hoeheDisplay;  }

	void addGmlKlassenVisual ( GmlKlassenVisual * pGmlKlasse );
	void addStyle ( Style * pStyle );
	void addSymbol ( Symbol * pSymbol );
	void addMarker ( Marker * pMarker );
	void addEnumeration ( SvgEnumeration * pEnumerationP );
	void addPattern ( Pattern * pPattern );
	void addHatching ( Hatching * pHatching );
  void addFlaechenStyle ( FlaechenDarstellung * pFlaechenStyle );
  void addLinienStyle ( LinienDarstellung * pLinienStyle );

  void removeFlaechenStyle ( std::string id );
  void removeLinienStyle (  std::string id );

	GmlKlassenVisual * getGmlKlassenVisual ( std::string gmlKlassenName );
	Style * getStyle ( std::string styleId );
	Symbol * getSymbol ( std::string symbolId );
	Marker * getMarker ( std::string markerId );
	Pattern * getPattern ( std::string patternId );
	Hatching * getHatching ( std::string hatchingId );

	size_t getGmlKlassenVisualAll ( std::vector<GmlKlassenVisual*> & vVisual );

	SvgEnumeration * getEnumeration ( std::string enumNameP );
	SvgEnumeration * getEnumeration ( std::string enumNameA, std::string enumNameB );
	SvgEnumItem * getEnumItem ( std::string enumNameP, std::string schluesselNummerP );
	SvgEnumItem * getEnumItem ( std::string enumNameA, std::string enumNameB,
								std::string schluesselNummerA, std::string schluesselNummerB );

  FlaechenDarstellung * getFlaechenDarstellung ( std::string id );
  LinienDarstellung * getLinienDarstellung ( std::string id );

	size_t getSymbols ( std::vector<Symbol*> &vSymbolsP );
	size_t getStyles ( std::vector<Style*> &vStylesP );
	size_t getMarkers ( std::vector<Marker*> &vMarkersP );
	size_t getPatterns ( std::vector<Pattern*> &vPatternP );
	size_t getHatchings ( std::vector<Hatching*> &vHatchingP );
};

///////////////////////////////////////////////////////////////////////////////
//  __gc class SvgParamReader						                        						 //
//  Einlesen der SVG-Darstellungsvprschriften                                //
///////////////////////////////////////////////////////////////////////////////
public ref class SvgParamReader
{
public:
	SvgParamReader ( double globalScaleP, String ^ gmlType );
	virtual ~SvgParamReader();

private:
	XmlTextReader     ^ reader;          // .NET XML-Reader
	String            ^ pActElement;     // Aktuell gelesenes XML-Objekt
	String            ^ pActAttrName;    // Name des aktuellen XML-Attributs
	String            ^ pGmlType;        // Name des aktuellen GML-Typs
	SvgVisualisierung * pSvgParams;      // Eingelesene Darstellungsparameter
	double              globalScale;     // Globale Skalierung aller Objekte

public:
	bool read ( std::string fileName, SvgVisualisierung * pSvgParamsP );

private:
	void readGmlKlassenVisual();

	FlaechenDarstellung * readFlaechenDarstellung ();
	void readFlaechenParams ( FlaechenDarstellung * pFlaechenDarstellung );
	void readLineTextParams ( Darstellung * Darstellung );
	void readLineSymbolParams ( Darstellung * pDarstellung );

	LinienDarstellung * readLinienDarstellung ();
	void readLinienParams ( LinienDarstellung * pLinienDarstellung );

	PunktDarstellung * readPunktDarstellung ();

	void readStyle();
	void readSymbol();
	void readMarker();
	void readPattern();
	void readHatching();
	void readEnumeration();
  void readGeometryParams ( SvgGeometry * pSvgGeometry, String ^endTag );
	SvgEnumItem * readEnumItem ( SvgEnumeration * pEnumerationP );

	SvgColor * readColor ( String ^ endTagP );
  std::string addDefaultNamespaceAttribut ( std::string name );
};

///////////////////////////////////////////////////////////////////////////////
//  __gc class SvgWriter										                           			 //
//  Transformation von 2D GML-Daten in eine SVG-Datei                        //
///////////////////////////////////////////////////////////////////////////////
public ref class SvgWriter
{
public:
	SvgWriter ( Visualisierung_2D * pVisualisierun_2D, 	bool allePlaeneZeigen, int anzObjWriteMaxP, double deltaP );
	virtual ~SvgWriter ();

public:
	void write ( std::string fileName, int darstellungsTyp, double globalScale );
  void initializeNormalization();
	void writeSvgHeader ( std::string fileName, 
				double _swX, double _swY, double _noX, double _noY );
  void writeSvgRootPlanbereich ( std::string fileName, double globalScale );
  void writeSvgRootUebersicht ( std::string fileName, double globalScale );
  void writeSvgRoot ( std::string fileName, double _swX, double _swY, double _noX, double _noY );
	void writeSvgEnd();
	void writeUeberschrift ( std::string fileName );
	void writeUebersicht ( std::string fileName, double globalScale );
  void writeUeberschriftStandard ( std::string fileName );
  void writeUebersichtStandard ( std::string fileName, double globalScale );
	void writePlanObjekt ( Feature * pPlanObjekt, bool isError, int objNr );
  void writeRasterdarstellung ( Feature * pPlanObjekt );
	void close ();

private:
	XmlTextWriter     ^ writer;                 //.NET XML-Writer
  Visualisierung_2D * m_pVisualisierung_2D;   // Allgemeine Parameter 2D-Visualisierung
	Features          * m_pFeatures;            // GML-Features, die nach SVG transformiert werden
	GmlSchema         * m_pGmlObjekte;          // GML-Schema
	SvgVisualisierung * m_pSvgParams;           // SVG-Darstellungsvorschriften
	ErrorReport       * m_pErrorReport;         // Fehlermeldungen
	int                 anzError;               // Anzahl der aufgetretenen Fehler
	int				          anzObjWriteMax;         // Anzahl der transformierten GML-Objekte
	bool                m_interactiveMap;       // Anzeige der interaktiven SVG-Karte
	bool                m_fehleranzeige;        // Anzeige von Objekten mit geometrischen Fehlern
	bool                m_allePlaeneZeigen;     // ANzeige aller eingelesener XPlanGML Plan-Objekte
	double              swX, swY, noX, noY;     // Geographische Umgrenzung des angezeigten Bereichs
	double              delta;                  // Größe des Karten-Randes
	static const double largeNumber = 1000;

private:
	void writeKlassenNamen ( std::set<GmlKlassenVisual*> & klassenNamenP );
	void writeExtraFont();
	void writeSvgPattern ( Pattern * pPattern );
	void writeSvgHatching ( Hatching * pHatching );
	void writeSvgSymbol ( Symbol * pSymbol );
	void writeSvgMarker ( Marker * pMarker );
	void writeGeometryElement ( SvgGeometry * pSvgGeometryP );

	void writePunktDarstellung ( Feature * pPlanObjektP, GmlKlassenVisual * pKlassenVisualP, bool isError );

	void writeLinienDarstellung ( Feature * pPlanObjektP, GmlKlassenVisual * pKlassenVisualP, bool isError );
	void writeLinienDarstellung ( Feature * pPlanObjektP,LinienDarstellung * pLinienDatstellung );

  void writeFlaechenDarstellung ( Feature * pPlanObjektP, GmlKlassenVisual * pKlassenVisualP, bool isError );
  void writeFlaechenDarstellung ( Feature * pPlanObjektP, FlaechenDarstellung * pFlaechenDarst );

	void writePPO ( Feature * pPlanObjektP );
	void writePPOFrei ( Feature * pPlanObjektP );
	void writePTO ( Feature * pPlanObjektP );
	void writePTOFrei ( Feature * pPlanObjektP );
	void writeLPO ( Feature * pPlanObjektP );
	void writeLPOFrei ( Feature * pPlanObjektP );
	void writeLTO ( Feature * pPlanObjektP );
	void writeLTOFrei ( Feature * pPlanObjektP );
	void writeFPO ( Feature * pPlanObjektP );
	void writeFPOFrei ( Feature * pPlanObjektP );

	void writeLineText (  Feature * pPlanObjektP, LineTextParams * pLineTextParamsP );
	void writeLineSymbol (  Feature * pPlanObjektP, LineSymbolParams * pLineSymbolParamsP );
  void writeSymbols (	std::string gmlId, std::vector<double> vX, std::vector<double> vY, std::vector<double> vAngle, 
						std::string symbolId, double symbolSize, double angleOffset );

  std::string addDefaultNamespaceAttribut ( std::string name );
  std::string removeDefaultNamespaceAttribut ( std::string name );

  bool readGeorefWerte ( std::vector<double> &georefWerte, StreamReader ^ pReader );
};

///////////////////////////////////////////////////////////////////////////////
//  class PlanzListe				                                  							 //
//  Liste aller Signaturen, die in Form von svg-Planzeichen in einer html-   //
//  Tabelle dokumentiert werden sollen										                   //
///////////////////////////////////////////////////////////////////////////////
class PlanzListe
{
	public:
		PlanzListe( Visualisierung_2D * pVisualisierung_2D );
		virtual ~ PlanzListe();

	private:
    Visualisierung_2D  * m_pVisualisierung; // Basisparameter 2D-Visualisierung
		Features           * m_pPlanObjekte;    // GML-Objekte
		SvgVisualisierung  * m_pSvgParams;      // SVG-Darstellungsvorschriften
    GML_SCHEMA_TYPE      gmlTyp;            // Typ des GML-Applikationsschemas
    std::string          svgParamDatei;     // Steuerdatei zur Erzeugug der Planzeichen-Liste
		std::string          ausgabePfad;       // Pfadname der Ordners der erzeugten Dateie
		std::string          pngFolder;         // Ordner-Name PNG-Dateien
		std::string          svgFolder;         // Ordner-Name SVG-Dateien
		std::string          indexDateiName;    // Dateiname HTML Root-Datei
		std::string          pngPath;           // Pfadname des Ordners der PNG-Dateien
		std::string          svgPath;           // Pfadname des ordners der SVG-Dateien
		std::string          indexDateiPath;    // Pfadname der HTML Root-Datei
		std::string          javaPath;          // Pfadname Java
		std::string          batikPath;         // Pfadname Batik
		std::string          javaHeapSize;      // Java Heap Size   
		std::ofstream        indexDatei;        // Ausgabe Stream HTML Root Datei
		double               sizeX, sizeY;      // Räumliche Ausdehnung der Signatur-Rechtecke
		int					         pixelX, pixelY;    // Raster-Auflösung der Signatur-Rectecke
    Feature            * pPlanFlaechenObjekt;// Hilfs-Feature für Flächenobjekte
    Feature            * pPlanLinienObjekt;  // Hilfs-Feature für Linienobjekte
    Feature            * pPlanPunktObjekt;   // Hilfs-Feature für Punkt-Objekte
    Feature            * pPPOObjekt;         // Hilfs-Feature für Präsentationsobjekte
		std::vector<Planz*> vPlanzListe;         // Liste der erzeugten Planzeichen

	public:
    Visualisierung_2D * getVisualisierung()  {  return m_pVisualisierung;  }
		std::ofstream & getIndexDatei()  {  return indexDatei;  }

    void setGmlTyp ( GML_SCHEMA_TYPE gmlTypP ) {  gmlTyp = gmlTypP;  }
    GML_SCHEMA_TYPE getGmlTyp() {  return gmlTyp; }

		void setAusgabePfad ( std::string pfad )  {  ausgabePfad = pfad;  }
		std::string getAusgabePfad ()  {   return ausgabePfad;  }

		void setPngFolder ( std::string folder )  {  pngFolder = folder;  }
		std::string getPngFolder ()  { return pngFolder;  }
		std::string getPngPath ()  {  return pngPath;  }

		void setSvgFolder ( std::string folder )  {  svgFolder = folder;  }
		std::string getSvgFolder ()  {  return svgFolder;  }
		std::string getSvgPath ()  {  return svgPath;  }

		void setIndexDateiName ( std::string name )  {  indexDateiName = name;  }
		std::string getIndexDateiName ()  {  return indexDateiName;  }

		void addPlanz ( Planz * pPlanz ); 
		void createPlanzeichen ( std::string planzListePfadP );
		
		void writeIndexDatei ( std::string textP );

		void setJavaPath ( std::string path )  { javaPath = path;  }
		std::string getJavaPath ()  {  return javaPath;  }

		void setBatikPath ( std::string path )  { batikPath = path;  }
		std::string getBatikPath ()  {  return batikPath;  }

		void setJavaHeapSize ( std::string path )  { javaHeapSize = path;  }
		std::string getJavaHeapSize ()  {  return javaHeapSize;  }

		void createGeometrieObjekte();
		void createGeometrieObjekte( double sizeX, double sizeY );

		void setSizeX ( double x ) {  sizeX = x;  }
		void setSizeY ( double y ) {  sizeY = y;  }
		void getSize ( double & x, double & y );

		void setPixelX ( int x ) {  pixelX = x;  }
		void setPixelY ( int y ) {  pixelY = y;  }
		void getPixel ( int & x, int & y );

    Feature * getFlaechenObjekt()  {  return pPlanFlaechenObjekt;  }
    Feature * getLinienObjekt()    {  return pPlanLinienObjekt;  }
    Feature * getPunktObjekt()     {  return pPlanPunktObjekt;  }
    Feature * getPPOObjekt()       {  return pPPOObjekt;  }

private:
		void writeIndexDateiHeader();
		void writeIndexDateiEnd();
		void convertSvgPng ( std::string svgDateiPath, std::string pngDateiPath );
};

///////////////////////////////////////////////////////////////////////////////
//  class Planz			                              													 //
//  Signaturen, die zu einer GML-Klasse gehören						              		 //
///////////////////////////////////////////////////////////////////////////////
class Planz
{
	public:
		Planz( PlanzListe * pPlanzListeP );
		virtual ~ Planz();

	private:
		PlanzListe * pPlanzListe;       // Planzeichen-Liste
		std::string  gmlClass;          // Name der zugehörigen GML-Klasse
		std::string  selectAttribut1;   // Name des 1. Auswahlattributs
		std::string  selectAttribut2;   // Name des 2. Auswahlattributs
		std::string  selectAttribut3;   // Name des 3. Auswahlattributs
		std::string  symbolAttribut;    // Name des Symbol-Attributs
		double       sizeX, sizeY;      // Räumliche Ausdehnung der Signatur-Rechtecke

		std::vector<PlanzFlaechenDarst*> vFlaechenDarst;  // Darstellungsparameter flächehafte Signaturen
		std::vector<PlanzLinienDarst*>   vLinienDarst;    // Darstellungsparameter linienhafte Signaturen
		std::vector<PlanzPunktDarst*>    vPunktDarst;     // Darstellungsparameter punktförmige Signaturen

	public:
		PlanzListe    * getPlanzListe()  {  return pPlanzListe;  }
		std::ofstream & getIndexDatei()  {  return pPlanzListe->getIndexDatei();  }

		void setGmlClass ( std::string gmlClassP )  {  gmlClass = gmlClassP; }
		std::string getGmlClass ()  {  return gmlClass;  }

		void addFlaechenDarst ( PlanzFlaechenDarst * pDarst )  {  vFlaechenDarst.push_back ( pDarst );  }
		void addLinienDarst ( PlanzLinienDarst * pDarst )  {  vLinienDarst.push_back ( pDarst );  }
		void addPunktDarst ( PlanzPunktDarst * pDarst )  {  vPunktDarst.push_back ( pDarst );  }

		void setSelectAttribut1 ( std::string attr )  {  selectAttribut1 = attr;  }
		std::string getSelectAttribut1 ()  {  return selectAttribut1;  }

		void setSelectAttribut2 ( std::string attr )  {  selectAttribut2 = attr;  }
		std::string getSelectAttribut2 ()  {  return selectAttribut2;  }

		void setSelectAttribut3 ( std::string attr )  {  selectAttribut3 = attr;  }
		std::string getSelectAttribut3 ()  {  return selectAttribut3;  }

		void setSymbolAttribut  ( std::string attr )  {  symbolAttribut = attr;  }
		std::string getSymbolAttribut ()  {  return symbolAttribut;  }

		void setSizeX ( double x ) {  sizeX = x;  }
		void setSizeY ( double y ) {  sizeY = y;  }
		void getSize ( double & x, double & y );

		void createPlanzeichen();
		void writeTabellenUeberschrift ( int tabType );
};

///////////////////////////////////////////////////////////////////////////////
//  class PlanzDarst						                            								 //
//  Basisklasse für Darstellungsparameter von Planzeichen                    //
///////////////////////////////////////////////////////////////////////////////
class PlanzDarst
{
	public:
		PlanzDarst( Planz * pPlanzP );
		virtual ~ PlanzDarst();

	protected:
		Planz	  * pPlanz;                   // Zugehöriges Planzeichen-Objekt
		std::string selectAttribut1Wert;    // Wert des 1. Auswahl-Attributs
		std::string selectAttribut2Wert;    // Wert des 2. Auswahl-Attributs
		std::string selectAttribut3Wert;    // Wert des 3. Auswahl-Attributs
		std::string symbolAttributWert;     // Wert des SymbolAttributs
		std::string selectAttribut1WertText;// Textliche Ergänzung des 1. Auswahl-Attribut Wertes
		std::string selectAttribut2WertText;// Textliche Ergänzung des 2. Auswahl-Attribut Wertes
		std::string selectAttribut3WertText;// Textliche Ergänzung des 3. Auswahl-Attribut Wertes
		std::string symbolAttributWertText; // Textliche Ergänzung des Symbol-Attribut Wertes
		double      symbolScale;            // Skalierung des Symbols

	public:
		Planz * getPlanz()  {  return pPlanz;  }

		void setSelectAttribut1Wert ( std::string attr )  {  selectAttribut1Wert = attr;  }
		std::string getSelectAttribut1Wert ()  {  return selectAttribut1Wert;  }

		void setSelectAttribut2Wert ( std::string attr )  {  selectAttribut2Wert = attr;  }
		std::string getSelectAttribut2Wert ()  {  return selectAttribut2Wert;  }

		void setSelectAttribut3Wert ( std::string attr )  {  selectAttribut3Wert = attr;  }
		std::string getSelectAttribut3Wert ()  {  return selectAttribut3Wert;  }

		void setSymbolAttributWert ( std::string wert )  {  symbolAttributWert = wert;  }
		std::string getSymbolAttributWert ()  {  return symbolAttributWert;  }

		virtual void createPlanzeichen ( int tabType ) = NULL;

		std::string createDateiName ( int tabType, std::string geoPraefix );
		std::string getAttributWertText ( std::string attributName, std::string attributWert );

		void setSymbolScale ( double h ) {  symbolScale = h;  }
		
		void writeTabellenZeile ( int tabType, std::string svgDateiPath, std::string pngDateiPath );

  private:
    std::string generateDateiName ( std::string name );
};

///////////////////////////////////////////////////////////////////////////////
//  class PlanzFlachenDarst									                        				 //
//  Darstellung einer Flächensignatur						                    				 //
///////////////////////////////////////////////////////////////////////////////
class PlanzFlaechenDarst: public PlanzDarst
{
	public:
		PlanzFlaechenDarst( Planz * pPlanzP );
		virtual ~ PlanzFlaechenDarst();

	public:
		virtual void createPlanzeichen( int tabType );
};

///////////////////////////////////////////////////////////////////////////////
//  class PlanzLinienDarst							                        						 //
//  Darstllung einer Liniensignatur	   									                     //
///////////////////////////////////////////////////////////////////////////////
class PlanzLinienDarst: public PlanzDarst
{
	public:
		PlanzLinienDarst( Planz * pPlanzP );
		virtual ~ PlanzLinienDarst( );

	public:
		virtual void createPlanzeichen( int tabType );
};

///////////////////////////////////////////////////////////////////////////////
//  class PlanzPunktDarst											                          		 //
//  Darstllung einer Punktsignatur	   							                  	     //
///////////////////////////////////////////////////////////////////////////////
class PlanzPunktDarst: public PlanzDarst
{
	public:
		PlanzPunktDarst( Planz * pPlanzP );
		virtual ~ PlanzPunktDarst();

	protected:

	public:
		virtual void createPlanzeichen( int tabType );
};

///////////////////////////////////////////////////////////////////////////////
//  class PlanzReader													                            	 //
//  Einlesen der XML-Struktur PlanzListe						                  			 //
///////////////////////////////////////////////////////////////////////////////
public ref class PlanzReader
{
	public:
		PlanzReader ( PlanzListe * pPlanzListeP );
		virtual ~ PlanzReader();

	private:
		PlanzListe    * pPlanzListe;     // Eingelesene Planzeichen-Liste
		XmlTextReader ^ pReader;         // .NET XML-Reader
		String        ^ pActElement;     // Aktuell gelesenes XML-Objekt

	public:
		void read ( std::string planzListePfad );

	private:
		Planz              * readPlanz();
		PlanzFlaechenDarst * readPlanzFlaechenDarst( Planz * pPlanz );
		PlanzLinienDarst   * readPlanzLinienDarst( Planz * pPlanz );
		PlanzPunktDarst    * readPlanzPunktDarst( Planz * pPlanz );

    std::string addDefaultNamespaceKuerzel ( std::string name );
};

///////////////////////////////////////////////////////////////////////////////
//  class PlanzListeGeneration			                      									 //
//  Generierung einer Planzeichen-Liste aus den	              							 //
//  SVG-Visualisierungsparametern									                           //
///////////////////////////////////////////////////////////////////////////////
public ref class PlanzListeGeneration
{
	public:
		PlanzListeGeneration ( SvgVisualisierung * pSvgParamsP );
		virtual ~ PlanzListeGeneration();

	private:
		SvgVisualisierung * pSvgParams;   // SVG Darstellungsparameter
		XmlTextWriter     ^ writer;       // .NET XML-Writer

	public:
		void write ( std::string filName );
};









	







