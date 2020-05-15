#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Includes.h"

class OSM_Model;
class OSM_VisualParams;
class OSM_FlaechenDarstellung;
class OSM_LinienDarstellung;
class OSM_PunktDarstellung;
class OSM_RelationDarstellung;
class OSM_Darstellung;
class OSM_FlaechenParams;
class OSM_LinienParams;
class OSM_VisualParams;
class OSM_Node;
class OSM_Relation;
class OSM_Way;
class Style;
class Symbol;
class SvgGeometry;
class SvgColor;
class OSM_Object;
class LineSymbolParams;
class MultiWay;
ref class SrsProjection;

///////////////////////////////////////////////////////////////////////////////
//  Class OSM_Visual								                        				      	 //
//  Basisklasse für die svg-Visualisierungsparameter einer OSM Klasse        //
///////////////////////////////////////////////////////////////////////////////
class OSM_Visual
{
public:
  OSM_Visual ( OSM_VisualParams * pParamsP );
  virtual ~ OSM_Visual();

private:
  OSM_VisualParams * pParams;
  std::string        osmClassification;
  std::string        selectAttribut;

  std::vector<OSM_FlaechenDarstellung*> vFlaechenDarstellungen;
  std::vector<OSM_LinienDarstellung*>	  vLinienDarstellungen;
  std::vector<OSM_PunktDarstellung*>    vPunktDarstellungen;
  std::vector<OSM_RelationDarstellung*> vRelationDarstellungen;

public:
  void setOSMClassification ( std::string classP )  { osmClassification = classP;  }
  std::string getOSMClassification()  {  return osmClassification;  }

  void setSelectAttribut ( std::string attr )  {  selectAttribut = attr;  }
  std::string getSelectAttribut ()  {  return selectAttribut;  }

  void addDarstellung ( OSM_Darstellung * pDarstellungP );
  OSM_Darstellung * getDarstellungSelected ( OSM_Object * pOSM_Object );

	size_t getFlaechenDarstellungen ( std::vector<OSM_Darstellung*> &vFaechenDarstellungenP );
	size_t getLinienDarstellungen ( std::vector<OSM_Darstellung*> &vLinienDarstellungenP );
	size_t getPunktDarstellungen ( std::vector<OSM_Darstellung*> &vPunktDarstellungenP );

  OSM_RelationDarstellung * getRelationDarstellung ( std::string relationType );
	size_t getDarstellungen ( std::vector<OSM_Darstellung*> &vDarstellungenP );

protected:
  void adaptRelationDarstellung ( OSM_RelationDarstellung * pRelationDarstellung, std::string selectAttributWert );

};

///////////////////////////////////////////////////////////////////////////////
//  class OSM_Darstellung		                         												 //
//  Basisklasse für die SVG-Visualisierungsparameter Klassen	        			 //
///////////////////////////////////////////////////////////////////////////////
class OSM_Darstellung
{
public:
  enum DARSTELLUNG {  PUNKT, LINIE, FLAECHE, RELATION  };
public:
  OSM_Darstellung();
  virtual ~OSM_Darstellung();

private:
//  std::string              id;
  int                      priority;
  std::vector<std::string> vSelectAttributWert;

public:
  virtual DARSTELLUNG getDarstellungTyp() = NULL;

//  void setId ( std::string idP )  {  id = idP;  }
//  std::string getId ()  {  return id;  }

  void setPriority ( int prio )  {  priority = prio; }
  int  getPriority()  {  return priority;  }

  void addSelectAttributWert ( std::string attr ) {  vSelectAttributWert.push_back ( attr );  }
	size_t getSelectAttributWerte ( std::vector<std::string> & vSelectAttribut1WertP );

  bool isSelected ( std::string selectAttributWertP );
};

///////////////////////////////////////////////////////////////////////////////
//  class FlaechenDarstellung									                        			 //
//  Visualisierungs-Parameter einer Fläche (geschl. OSM-way)   							 //
///////////////////////////////////////////////////////////////////////////////
class OSM_FlaechenDarstellung : public OSM_Darstellung
{
public:
  OSM_FlaechenDarstellung();
  virtual ~OSM_FlaechenDarstellung();

private:
  std::vector<OSM_FlaechenParams*> vFlaechenParams;

public:
  virtual DARSTELLUNG getDarstellungTyp()  {  return FLAECHE;  }

  void addFlaechenParams ( OSM_FlaechenParams * pFlaechenParamsP )  {  vFlaechenParams.push_back ( pFlaechenParamsP );  }
	size_t getFlaechenParams ( std::vector<OSM_FlaechenParams*> & vFlaechenParamsP );
};

///////////////////////////////////////////////////////////////////////////////
//  class OSM_LinienDarstellung			 			                           				 //
//  Visualisierungs-Parameter einer Linie (OSM-way)	                  			 //
///////////////////////////////////////////////////////////////////////////////
class OSM_LinienDarstellung : public OSM_Darstellung
{
public:
  OSM_LinienDarstellung();
  virtual ~OSM_LinienDarstellung();

private:
  std::vector<OSM_LinienParams*> vLinienParams;
  LineSymbolParams		         * pLineSymbolParams;

public:
  virtual DARSTELLUNG getDarstellungTyp()  {  return LINIE;  }

  void addLinienParams ( OSM_LinienParams * pLinienParamsP )  {  vLinienParams.push_back ( pLinienParamsP );  }
	size_t getLinienParams ( std::vector<OSM_LinienParams*> & vLinienParamsP );

  void setLineSymbolParams ( LineSymbolParams * pParamsP )  { pLineSymbolParams = pParamsP;  } 
  LineSymbolParams * getLineSymbolParams () {  return pLineSymbolParams;  }

};

///////////////////////////////////////////////////////////////////////////////
//  class OSM_PunktDarstellung								                        			 //
//  Visualisierungs-Parameter eines Punktes (OSM-node)               				 //
///////////////////////////////////////////////////////////////////////////////
class OSM_PunktDarstellung: public OSM_Darstellung
{
public:
  OSM_PunktDarstellung();
  virtual ~OSM_PunktDarstellung();

private:
  std::string attributA;
  std::string attributB;
  std::string textStyle;
  std::string angleAttribute;
  std::string symbol;
  double      symbolSize;
  double      textOffsetX;
  double		  textOffsetY;

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
//  std::string getDefaultSymbol();

  void setSymbolSize ( double sizeP )  {  symbolSize = sizeP;  }
  double getSymbolSize()  {  return symbolSize;  }
//  double getDefaultSymbolSize();

  void setTextOffsetX ( double h )  {  textOffsetX = h;  }
  double getTextOffsetX()  {  return textOffsetX;  }

  void setTextOffsetY ( double h )  {  textOffsetY = h;  }
  double getTextOffsetY()  {  return textOffsetY;  }
};

///////////////////////////////////////////////////////////////////////////////
//  class OSM_RelationDarstellung                                            // 
///////////////////////////////////////////////////////////////////////////////
class OSM_RelationDarstellung: public OSM_Darstellung
{
public:
  OSM_RelationDarstellung();
  virtual ~OSM_RelationDarstellung();

protected:
  OSM_FlaechenDarstellung * pFlaechenDarstellung;
  OSM_LinienDarstellung   * pLinienDarstellung;
  OSM_PunktDarstellung    * pPunktDarstelung;

  std::string               type;

public:
  virtual DARSTELLUNG getDarstellungTyp()  {  return RELATION;  }

  void setType ( std::string typeP )  {  type = typeP;  }
  std::string getType()  {  return type;  }

  void setFlaechenDarstellung ( OSM_FlaechenDarstellung * pFlaechenDarstellungP )  {  pFlaechenDarstellung = pFlaechenDarstellungP;  }
  void setLinienDarstellung ( OSM_LinienDarstellung * pLinienDarstellungP )  { pLinienDarstellung = pLinienDarstellungP;  }
  void setPunktDarstellung ( OSM_PunktDarstellung * pPunktDarstelungP )  { pPunktDarstelung = pPunktDarstelungP;  }

  OSM_FlaechenDarstellung * getFlaechenDarstellung()  {  return pFlaechenDarstellung;  }
  OSM_LinienDarstellung   * getLinienDarstellung() { return pLinienDarstellung;  }
  OSM_PunktDarstellung    * getPunktDarstellung()  {  return pPunktDarstelung;  }

};

///////////////////////////////////////////////////////////////////////////////
//  class OSM_FlaechenParams				                          							 //
//  Klasse für die svg-Visualisierungs Parameter einer OSM-Fläche	        	 //
///////////////////////////////////////////////////////////////////////////////
class OSM_FlaechenParams
{
public:
  OSM_FlaechenParams ();
  virtual ~ OSM_FlaechenParams();

private:
  std::string classId;
//  std::string patternId;
//  std::string hatchingId;
//  bool		    clip;

public:
  void setClassId ( std::string id )  {  classId = id;  }
  std::string getClassId ()  {  return classId;  }

//  std::string getPatternId()  {  return patternId;  }
//  void setPatternId ( std::string patternIdP )  {  patternId = patternIdP;  }

//  std::string getHatchingId()  {  return hatchingId;  }
//  void setHatchingId ( std::string hatchingIdP )  {  hatchingId = hatchingIdP;  }

//  bool getClip ()  {  return clip;  }
//  void setClip ( bool clipP )  {  clip = clipP;  }
};

///////////////////////////////////////////////////////////////////////////////
//  class OSM_LinienParams  		                            								 //
//  Klasse für die svg-Visualisierungs Parameter einer OSM-Linie     				 //
///////////////////////////////////////////////////////////////////////////////
class OSM_LinienParams
{
public:
  OSM_LinienParams ();
  virtual ~ OSM_LinienParams();

private:
  std::string        classId;
  LineSymbolParams * pLineSymbolParams;
  

public:
  void setClassId ( std::string id )  {  classId = id;  }
  std::string getClassId ()  {  return classId;  }

  void setLineSymbolParams ( LineSymbolParams * pLineSymbolParamsP  )  {  pLineSymbolParams = pLineSymbolParamsP; }
  LineSymbolParams * getLineSymbolParams() {  return pLineSymbolParams;  }
};

///////////////////////////////////////////////////////////////////////////////
//  class OSM_VisualParams  											                        	 //
///////////////////////////////////////////////////////////////////////////////
class OSM_VisualParams
{
public:
  OSM_VisualParams ();
  virtual ~OSM_VisualParams();

private:
  double breiteUeberschrift;
  double hoeheUeberschrift;
  double breiteDisplay;
  double hoeheDisplay;

  bool   showAllSurfaces;
  bool   showAllLines;
  bool   showAllNodes;

  string datum;

  std::map<std::string,OSM_Visual*>              mVisual;
  std::map<std::string,Style*>		               mStyles;
  std::map<std::string,Symbol*>                  mSymbols;

  OSM_PunktDarstellung    * pDefaultPunktDarstellung;
  OSM_FlaechenDarstellung * pDefaultFlaechenDarstellung;
  OSM_LinienDarstellung   * pDefaultLinienDarstellung;

public:
  void setShowAllSurfaces( bool b )  {  showAllSurfaces = b;  }
  bool getShowAllSurfaces()  {  return showAllSurfaces;  }

  void setShowAllLines( bool b )  {  showAllLines = b;  }
  bool getShowAllLines()  {  return showAllLines;  }

  void setShowAllNodes( bool b )  {  showAllNodes = b;  }
  bool getShowAllNodes()  {  return showAllNodes;  }

  void setBreiteUeberschrift ( double breite )  {  breiteUeberschrift = breite;  }
  double getBreiteUeberschrift ()  {  return breiteUeberschrift;  }

  void setHoeheUeberschrift ( double hoehe )  {  hoeheUeberschrift = hoehe;  }
  double getHoeheUeberschrift ()  {  return hoeheUeberschrift;  }

  void setBreiteDisplay ( double b )  {  breiteDisplay = b;  }
  double getBreiteDisplay()  {  return breiteDisplay;  }

  void setHoeheDisplay( double h )  {  hoeheDisplay = h; }
  double getHoeheDisplay()  {  return hoeheDisplay;  }

  void setDatum ( std::string datumP )   {   datum = datumP;  }
  std::string getDatum()  { return datum;    }

  void addOSM_Visual ( OSM_Visual * pVisual );
  void addStyle ( Style * pStyle );
  void addSymbol ( Symbol * pSymbol );

  void setDefaultFlaechenDarstellung ( OSM_FlaechenDarstellung * pDefaultFlaechenDarstellungP )  { pDefaultFlaechenDarstellung = pDefaultFlaechenDarstellungP; }
  OSM_FlaechenDarstellung * getDefaultFlaechenDarstellung()  {  return pDefaultFlaechenDarstellung; }

  void setDefaultLinienDarstellung ( OSM_LinienDarstellung * pDefaultLinienDarstellungP )  { pDefaultLinienDarstellung = pDefaultLinienDarstellungP; }
  OSM_LinienDarstellung * getDefaultLinienDarstellung()  {  return pDefaultLinienDarstellung; }

  void setDefaultPunktDarstellung ( OSM_PunktDarstellung * pDefaultPunktDarstellungP )  { pDefaultPunktDarstellung = pDefaultPunktDarstellungP; }
  OSM_PunktDarstellung * getDefaultPunktDarstellung()  {  return pDefaultPunktDarstellung; }

  OSM_Visual * getOSM_Visual ( std::string featureType );
  Style * getStyle ( std::string styleId );
  Symbol * getSymbol ( std::string symbolId );

	size_t getOSM_VisualAll ( std::vector<OSM_Visual*> & vVisual );
	size_t getSymbols ( std::vector<Symbol*> &vSymbolsP );
	size_t getStyles ( std::vector<Style*> &vStylesP );

  OSM_Darstellung * getVisualization ( OSM_Object * pObj, std::string & classification, int & priority );
};

///////////////////////////////////////////////////////////////////////////////
//  __gc class OSM_SvgParamReader						                     						 //
///////////////////////////////////////////////////////////////////////////////
public ref class OSM_SvgParamReader
{
public:
  OSM_SvgParamReader ( double globalScaleP );
  virtual ~OSM_SvgParamReader();

private:
  XmlTextReader ^ reader;

  String ^ pActElement;
  String ^ pActAttrName;

  OSM_VisualParams * pOSM_SvgParams;

  double globalScale;

public:
  bool read ( std::string fileName, OSM_VisualParams * pOSM_SvgParamsP );

private:
  void readOSM_Visual();

  OSM_FlaechenDarstellung * readFlaechenDarstellung ( String ^ tag );
  void readFlaechenParams ( OSM_FlaechenDarstellung * pFlaechenDarstellung );

  OSM_LinienDarstellung * readLinienDarstellung ( String ^ tag  );
  void readLinienParams ( OSM_LinienDarstellung * pLinienDarstellung );
  void readLineSymbolParams ( OSM_LinienDarstellung * pDarstellung );

  OSM_PunktDarstellung * readPunktDarstellung ( String ^ tag );

  OSM_RelationDarstellung * readRelationDarstellung ( String ^ tag );

  Style * readStyle();
  Symbol * readSymbol();

  void readGeometryParams ( SvgGeometry * pSvgGeometry, String ^endTag );

  SvgColor * readColor ( String ^ endTagP );
};

///////////////////////////////////////////////////////////////////////////////
//  Schreiben des OSM-Modells als SVG                                        //
///////////////////////////////////////////////////////////////////////////////
public ref class OSM_SvgWriter
{
public:
  OSM_SvgWriter ( OSM_VisualParams * pOSM_SvgParamsP, OSM_Model * pOSM_Model, double deltaP );
  virtual ~OSM_SvgWriter ();

protected:
  XmlTextWriter    ^ writer;
  OSM_Model        * m_pOSM_Model;
  OSM_VisualParams * m_pOSM_SvgParams;
  SrsProjection    ^ m_pSrsProjection;
//  GeoPoint         * pNormalization;

  int           anzError;
  int				    anzObjWriteMax;
  int           EPSG_Code;

  double        swX, swY, noX, noY;
  double        delta;

public:
  void write ( std::string fileName, double globalScale );

  void writeSvgRootKarte ( std::string fileName, double globalScale );
  void writeUeberschrift ( std::string fileName );
  void writeUebersicht ( std::string fileName, double globalScale );

  void writeOSM_Objekt ( OSM_Object * pOSMObjekt, OSM_Darstellung * pDarstellungP );

protected:
  bool initializeNormalization();
  int  getUTM_Zone ( double laenge );

  void writeSvgRoot ( std::string fileName, double _swX, double _swY, double _noX, double _noY );

//  void writeSvgSymbol ( Symbol * pSymbol ); 
//  void writeGeometryElement ( SvgGeometry * pSvgGeometryP );

  void writePunktDarstellung    ( OSM_Node * pOSM_Object, OSM_PunktDarstellung * pPunktDarstellung );
  void writeLinienDarstellung   ( OSM_Way * pOSM_Object, OSM_LinienDarstellung * pLinienDarstellung );
  void writeFlaechenDarstellung ( OSM_Way * pOSM_Object, OSM_FlaechenDarstellung * pFlaechenDarstellung );
  void writeRelationDarstellung ( OSM_Relation * pOSM_Object, OSM_RelationDarstellung * pRelationDarstellung );
  bool writeMultiPolygon ( OSM_Relation * pOSM_Object, OSM_RelationDarstellung * pRelationDarstellung );
  void writeMultiPolygon ( std::string objId, std::vector<std::string> &vOuterNodeIds, std::vector<MultiWay*> &vInner, OSM_RelationDarstellung * pRelationDarstellung );


  String ^ createPathString ( std::vector<double> &vX, std::vector<double> &vY );
};

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die OSM-Tags in eine HTML-Datei                                 //
///////////////////////////////////////////////////////////////////////////////
public ref class OSM_ObjectDataWriter
{
public:
  OSM_ObjectDataWriter ( String ^ objFolderP );
  virtual ~ OSM_ObjectDataWriter();

public:
  void write ( OSM_Object * pOSM_Object );

protected:
  String       ^ objFolder;
  StreamWriter ^ pWriter;

};





