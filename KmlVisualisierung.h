#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

#include "includes.h"
#include "SrsManager.h"

class KmlKlassenVisualParams;
class KmlPolyVisualParams;
class KmlLineVisualParams;
class KmlColor;
class SchemaTypeNames;
class GmlDictionary;
class Features;
class GmlToolbox;
class Feature;
class GmlSchema;
class PolygonGeo;
class _Curve;
class Logfile;

///////////////////////////////////////////////////////////////////////////////
//  KmlVisualParams                                                          //
//  Zusammenfassung aller KML-Darstellungsparameter für GML-Klassen          //
///////////////////////////////////////////////////////////////////////////////
class KmlVisualParams
{
  public:
    KmlVisualParams();
    virtual ~ KmlVisualParams();

  protected:
    std::map<std::string,KmlKlassenVisualParams*> mKlassenVisualParams; // Darstell.parameter für einzelne GML-Klassen
    GML_SCHEMA_TYPE                               gmlType;              // GML-Applikationsschema

  public:
    void setGmlTyp ( GML_SCHEMA_TYPE typ )  {  gmlType = typ;  }
    GML_SCHEMA_TYPE getGmlTyp()  {  return gmlType;  }

    void addKlassenVisualParams ( KmlKlassenVisualParams * pKlassenVisualParams );

    KmlKlassenVisualParams * getKlassenVisualParams ( std::string klassenName );
    KmlPolyVisualParams    * getPolygonKlassenVisualParams ( std::string klassenName, std::string selectAttributName );
    KmlLineVisualParams    * getLineKlassenVisualParams ( std::string klassenName, std::string selectAttributName );
};

///////////////////////////////////////////////////////////////////////////////
//  KmlKlassenVisualParams                                                   //
//  KML-Darstellungsparameter für eine GML-Klasse                            //
//  Es können Parameter für Linien und Flächendarstellung verwaltet werden   //
///////////////////////////////////////////////////////////////////////////////
class KmlKlassenVisualParams
{
  public:
    KmlKlassenVisualParams();
    virtual ~ KmlKlassenVisualParams();

  protected:
    std::string                                klassenName;               // Name der Klasse
    std::map<std::string,KmlPolyVisualParams*> mPolyVisualParams;         // Parameter für flächenhafte Darstellung
    std::map<std::string,KmlLineVisualParams*> mLineVisualParams;         // Parameter für linienhafte Darstellung
    KmlPolyVisualParams                      * pPolyVisualParamsDefault;  // Default-Parameter flächenhafte Darstellung
    KmlLineVisualParams                      * pLineVisualParamsDefault;  // Default-Parameter linienhafte Darstellung

  public: 
    void        setKlassenName ( std::string name )  {  klassenName = name;  }
    std::string getKlassenName ()                    {  return klassenName;  }

    void                  addPolyVisualParams ( KmlPolyVisualParams * pParams, std::string selectAttributName );
    KmlPolyVisualParams * getPolyVisualParams( std::string selectAttributName );

    void                  addLineVisualParams ( KmlLineVisualParams * pParams, std::string selectAttributName  );
    KmlLineVisualParams * getLineVisualParams( std::string selectAttributName  ); 
};

///////////////////////////////////////////////////////////////////////////////
//  KmlPolyVisualParam                                                       //
//  KML Darstellungsparameter für Flächen                                    //
///////////////////////////////////////////////////////////////////////////////
class KmlPolyVisualParams
{
  public:
    KmlPolyVisualParams( std::string selectAttributNameP );
    virtual ~ KmlPolyVisualParams();

  protected:
    KmlColor                      * pFillColor;      // Farbe Flächenfüllung
    KmlColor                      * pOutlineColor;      // Farbe Außenkontor
    double                          physicalWidth;      // Linienstärke Außenkontur
    std::string                     selectAttributName; // Name eines Attributs, dessen Wert durch eine Farbe repräsentiert wird
    std::map<std::string,KmlColor*> colorTable;         // map: Attributwert --> Farbe


  public:
    void setFillColor ( KmlColor * pColor );
    KmlColor * getFillColor()  {  return pFillColor; }
    bool getFillColorSet ();

    void setOutlineColor ( KmlColor * pColor );
    KmlColor * getOutlineColor()  {  return pOutlineColor; }
    bool getOutlineColorSet();

    void setPhysicalWidth ( double width )  {  physicalWidth = width;  }
    double getPhysicalWidth()  {  return physicalWidth; }

    std::string getSelectAttributName()  {  return selectAttributName;  }

    void addColor ( std::string attributWert, KmlColor * col )  {  colorTable[attributWert] = col;  }
    KmlColor * getColor ( std::string attributWert );
};

///////////////////////////////////////////////////////////////////////////////
//  KmlLineVisualParam                                                       //
//  KML Darstellungsparameter für Linien                                     //
///////////////////////////////////////////////////////////////////////////////
class KmlLineVisualParams
{
  public:
    KmlLineVisualParams( std::string selectAttributNameP );
    virtual ~ KmlLineVisualParams();

  protected:
    KmlColor                      * pLineColor;         // Linienfarbe
    double                          physicalWidth;      // Linienstärke
    std::string                     selectAttributName; // Name eines Attributs, dessen Wert durch eine Farbe repräsentiert wird
    std::map<std::string,KmlColor*> colorTable;         // map: Attributwert --> Farbe

  public:
    void setLineColor ( KmlColor * pColor );
    KmlColor * getLineColor ()  {  return pLineColor; }
    bool getLineColorSet();

    void setPhysicalWidth ( double width )  {  physicalWidth = width;  }
    double getPhysicalWidth()  {  return physicalWidth;  }

    std::string getSelectAttributName()  {  return selectAttributName;  }

    void addColor ( std::string attributWert, KmlColor * col )  {  colorTable[attributWert] = col;  }
    KmlColor * getColor ( std::string attributWert );
};

///////////////////////////////////////////////////////////////////////////////
//  KmlColor                                                                 //
//  R, G, B und Transparenzwert                                              //
///////////////////////////////////////////////////////////////////////////////
class KmlColor
{
  public:
    KmlColor();
    KmlColor ( short r, short g, short b, short alpha );
    virtual ~ KmlColor();

  protected:
    short m_r;        // Rot
    short m_g;        // Grün
    short m_b;        // Blau
    short m_alpha;    // Transparenz

  public:
    void setR ( short r )  {  m_r = r;  }
    short getR ()  {  return m_r;  }

    void setG ( short g )  {  m_g = g;  }
    short getG ()  {  return m_g;  }

    void setB ( short b )  {  m_b = b;  }
    short getB ()  {  return m_b;  }

    void setAlpha ( short alpha )  {  m_alpha = alpha;  }
    short getAlpha ()  {  return m_alpha;  }

    String ^ get();
};

///////////////////////////////////////////////////////////////////////////////
//  KmlVisualParamReader                                                     //
// Einlesen einer Datei mit KML-Darstellungsparametern                       //
///////////////////////////////////////////////////////////////////////////////
public ref class KmlVisualParamReader
{
  public:
    KmlVisualParamReader( String ^ gmlTypeP );
    virtual ~KmlVisualParamReader();

  protected:
    XmlTextReader ^ reader;         // .NET XML-Reader

    String          ^ pActElement;  // Aktuell gelesene XML-Objekt
    String          ^ gmlType;      // GML-Typ der Parameter-Datei
    KmlVisualParams * pKmlParams;   // Eingelesene Darstellungsparameter

  public:
    KmlVisualParams * read( std::string filename );

  protected:
    KmlKlassenVisualParams * readKlassenVisualParams();
    KmlPolyVisualParams    * readPolyVisualParams( std::string selectAttributName );
    KmlLineVisualParams    * readLineVisualParams( std::string selectAttributName );
    KmlColor               * readKmlColor();
    KmlColor               * readColorTableEntry ( std::string &attributWert );
};

///////////////////////////////////////////////////////////////////////////////
//  KmlWriter                                                                //
//  Rausschreiben der Features als KML-Datei                                 //
///////////////////////////////////////////////////////////////////////////////
public ref class KmlWriter
{
  public:
    KmlWriter ( KmlVisualParams * pVisualParamsP, Features * pFeaturesP );
    virtual ~KmlWriter();

  protected:
    Features         * pFeatures;         //  GML-Features
    Logfile          * pLogfile;          // Logfile für Fehlermeldungen 
    SrsManager       * pSrsManager;       // Modul Koordinaten-Transformationen
    GmlSchema        * pGmlObjekte;       // GML-Applikationsschema
    SchemaTypeNames  * pSchemaTypeNames;  // Wichtige XML-Tags der eingelesenen XML-Schemadateien
    KmlVisualParams  * pKmlVisualParams;  // Darstellungsparameter
    GmlDictionary    * pCodeLists;        // GML-Dictionar der Enumerationen
    SrsProjection    ^ pSrsProjection;    // Aktuelles Koordinaten-Referenzsystem
    XmlTextWriter    ^ writer;            // .NET XML-Writer

    bool               showEdges;         // Anzeige der Kanten in der KML-Szene
    bool               showFeatureData;   // Anzeige der Attributdaten in der XML-Szene
    double             zOffset;           // Vertikaler Offset
    CITYGML_LOD        lodStufe;          // Bei CityGML: Angezeigte LOD-Stufe
//    String           ^ thema;             // 
//    String           ^ colladaTempFolder;
    String           ^ kmlFilePath;       // Pfadname der erzeugten KML-Datei
    String           ^ kmlFileName;       // Name der erzeugten KML-Datei
//    String           ^ kmzFilePath;
//    String           ^ gmlFolder;


  public:
    bool write ( std::string &fileNameP, std::string selectAttributName, int index );

    void setZOffset ( double z )  {  zOffset = z;  }
    double getZOffset ()  {  return zOffset;  }

    void setLODStufe ( CITYGML_LOD lodStufeP )  {  lodStufe = lodStufeP;  }
//    void setThema ( String ^ themaP )  {  thema = themaP;  }

    void setShowEdges ( bool b )  {  showEdges = b;  }
    bool getShowEdges()  {  return showEdges;  }

    void setShowFeatureData ( bool b )  {  showFeatureData = b;  }
    bool getShowFeatureData()  {  return showFeatureData;  }

  protected:
    void writeKmlRoot();
    bool writeCameraPosition();
    void writeStyles( std::string selectAttributName );
    
    void writeFeature ( Feature * pFeature, std::string  selectAttributName, int index );
    void writeKmlPlacemark ( Feature * pFeature, std::string selectAttributName, int index );
    void writeKmlDescription ( Feature * pFeature );
    void writeFeatureGeometry ( Feature * pFeature );
    String ^ writeFeatureData ( Feature * pFeature );
    void writeDataType ( int level, StringWriter ^ pStringWriter, Feature * pDataType );
    bool writePolygon( Feature * pFeature, PolygonGeo * pPolygon, std::string srsName );
    bool writeCurve ( Feature * pFeature, _Curve * pCurve, std::string srsName );

    void writeAddress ( StringWriter ^ stringWriter, Feature * pAddress );

    bool transformToLaengeBreite ( std::string srsNameShort, double fromCoord[3], double toCoord[3] );
    String ^ getStyleName ( String ^ klassenName );

    bool writeAttributeDependentColor( Feature * pFeature, std::string selectAttributName, size_t index );
    KmlColor getStandardFillColor ( Feature * pFeature, std::string selectAttributName );
};



