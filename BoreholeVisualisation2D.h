#pragma once

class Feature;
class GmlSchema;
class GmlDictionary;
class LithologyInterval;
class StratigraphyInterval;
class Lithology;
class SvgColor;
class Pattern;
class Hatching;
class SvgGeometry;
class BoreholeDarstellung;

///////////////////////////////////////////////////////////////////////////////
//  2D Visualisierung eines BoreholeML Objektes Borehole mit SVG             //
///////////////////////////////////////////////////////////////////////////////
class BoreholeVisualisation2D
{
  public: 
    BoreholeVisualisation2D ( Feature * pFeature, BoreholeDarstellung * pBoreholeDarstellungP,  std::string version = "" );
    virtual ~ BoreholeVisualisation2D();

  protected:
    Feature             * pBorehole;						// Darzustellendes Borehole Objekt
    GmlSchema           * pGmlSchema;						// BoreholeML GML-Schema
    GmlDictionary       * pCodeLists;						// Dictionary der Codelisten
    std::string           layerSeriesVersion;		// Versionsbezeichnung der darzustellenden Schichten
    BoreholeDarstellung * pBoreholeDarstellung; // Darstellungsparameter Bohrloch
    std::string           codelistRegistry;			// Registry den BoreholeML Codelisten

    std::vector<LithologyInterval*>    vLithologyIntervals;			// Schichtfolge Lithographie
    std::vector<StratigraphyInterval*> vStratigraphyIntervals;	// Schichtfolge Stratigraphie

  public:
    BoreholeDarstellung * getBoreholeDarstellung()  {  return pBoreholeDarstellung;  }
    Feature             * getBorehole()  {  return pBorehole;  }

		size_t getLithologyIntervalAnz()  {  return vLithologyIntervals.size();  }
    LithologyInterval * getLithologyInterval ( unsigned int index );

		size_t getStratigraphyIntervalAnz()  {  return vStratigraphyIntervals.size();  }
    StratigraphyInterval * getStratigraphyInterval ( unsigned int index );

		size_t getRockCodesUsed ( std::set<std::string> & vRockCodes );
		size_t getStratigraphyCodesUsed ( std::set<std::string> & vStratigraphyCodes );

    void show();

  protected:
    bool extractBoreholeInformation();
    bool extractIntervalSeriesInformation( Feature * pIntervalSeries );
    bool extractLithologyInformation ( Feature * pInterval, LithologyInterval * pLithologyInterval );
    bool extractStratigraphyInformation ( Feature * pInterval, std::string & chronoStratigrapgy, std::string & chronoStratigrapgyText, std::string& lithoStratigraphy );


    std::string codeToText ( std::string codeListName, std::string code );
};

///////////////////////////////////////////////////////////////////////////////
//  Modelliert ein Intervall einer Borehole IntervalSeries                   //
///////////////////////////////////////////////////////////////////////////////
class Interval
{
  public:
    Interval ();
    virtual ~ Interval();

  private:
    double from;		// Anfangs-Tiefe des Intervalls
    double to;			// Endtiefe des Intervalls

  public:
    void setFrom ( double f )  {  from = f;  }
    double getFrom()  {  return  from;  }

    void setTo ( double t )  {  to = t;  }
    double getTo()    {  return to;  }

};

///////////////////////////////////////////////////////////////////////////////
//  Modelliert ein Lithologie-Intervall                                      //
///////////////////////////////////////////////////////////////////////////////
class LithologyInterval: public Interval
{
  public:
    LithologyInterval ();
    virtual ~LithologyInterval();

  public:
    std::string rockCode;					// Gesteins-Code (BoreholeML)
    std::string rockNameText;			// Gesteinsname (BoreholeML)
    std::string intervalType;     // Schicht-Typ (BoreholeML)
    std::string genesis;					// Genese der Schicht (BoreholeML)
    std::string geoGenesis;				// Petrogenetische Bezeichnung der Schicht (BoreholeML)
    std::string carbonatContent;	// Karbonatanteil (BoreholeML)
    std::string waterContent;			// Wasseranteil (BoreholeML)
    std::string consistency;			// Zustandsform (BoreholeML)
    std::string descriptiveText;	// Beschreibender Text (BoreholeML)

  protected:
    std::vector<Lithology*> vLithology; // Zugeordnete Gesteinstypen

  public:
    void addLithology ( Lithology * pLithology );
    
		size_t getLithologyAnz()  {  return vLithology.size();  }
    Lithology * getLithology ( unsigned int index );
};

///////////////////////////////////////////////////////////////////////////////
//  Modelliert die BoreholeML Klasse Lithology                               //
///////////////////////////////////////////////////////////////////////////////
class Lithology
{
  public:
    Lithology();
    virtual ~Lithology();

  public:
    std::string rockCode;					// Gesteins-Code (BoreholeML)
    std::string rockText;					// Gesteins-Bezeichnung (BoreholeML)
    std::string rockColor;				// Farbton und Helligkeit des Gesteins (BoreholeML)
    std::string rockColorMixture;	// Verteilung der Farbe innerhalb des Gesteintyps (BoreholeML)

  protected:
    double percentageFrom;				// Untergrenze des prozentualen Anteils des Gesteins (BoreholeML)
    double percentageTo;					// Obergrenze des prozentualen Anteils des Gesteins (BoreholeML)

  public:
    void setPercentage ( double wert );
    void setPercentageFromTo ( double wertUnten, double wertOben );

    double getPercentageFrom()  {  return percentageFrom;  }
    double getPercentageTo()  {  return percentageTo;  }
};

///////////////////////////////////////////////////////////////////////////////
//  Modelliert ein Stratigraphie-Intervall                                   //
///////////////////////////////////////////////////////////////////////////////
class StratigraphyInterval: public Interval
{
  public:
    StratigraphyInterval();
    virtual ~StratigraphyInterval();

  public:
    std::string chronoStratigraphy;     // Chronostratigraphische Einstufung der Schicht (Code)
    std::string chronoStratigraphyText; // Chronostratigraphische Einstufung der Schicht (Text)
    std::string lithoStratigraphy;			// Länderspezifische Lithostratigraphie als Text
};

///////////////////////////////////////////////////////////////////////////////
//  SVG-Darstellungsparameter eines Intervalles                              //
///////////////////////////////////////////////////////////////////////////////
class SchichtDarstParameter
{
  public:
    SchichtDarstParameter();
    virtual ~ SchichtDarstParameter();

  protected:
    SvgColor  * pColor;				  // Farbe
    Pattern   * pPattern;				// Füllung
    Hatching  * pHatching;			// Schraffur
    std::string signaturePath;	// Rasterbild-Signatur

  public:
    void setColor(  SvgColor * pColorP )  {  pColor = pColorP;  }
    SvgColor * getColor()  {  return pColor;  }

    void setPattern ( Pattern  * pPatternP )  {  pPattern = pPatternP;  }
    Pattern * getPattern()  {   return pPattern;  }

    void setHatching( Hatching * pHatchingP )  {  pHatching = pHatchingP;  }
    Hatching * getHatching()  {  return pHatching;  }

    void setSignaturePath ( std::string p )  {  signaturePath = p;  }
    std::string getSignaturePath ()  {  return signaturePath; }
};

///////////////////////////////////////////////////////////////////////////////
//  Parameter für 2D-Bohrlochdarstellung                                     //
///////////////////////////////////////////////////////////////////////////////
class BoreholeDarstellung
{
  public:
    enum LITHOLOGY_VISUALISATION_STRATEGY  {  ALL_COMPONENTS,      // Anzeige aller Komponenten (noch nicht unterstützt)
																							MAJOR_COMPONENT  };	 // Nur Anzeige der Haupt-Komponenten
  
public:
    BoreholeDarstellung();
    virtual ~BoreholeDarstellung();

  protected:
    std::string beschreibung;										// Beschreibung
    std::string signatureFolder;								// Ordner-Pfad der Rasterbild-Sognaturen

    double      boreholeDurchmesserLithologie;		// Bohrloch-Durchmesser für die Visualisierung der Lithologie
    double      boreholeDurchmesserStratigraphie;	// Bohrloch-Durchmesser für die Visualisierung der Stratigraphie
    double      abstandSaeulen;										// Abstand der beiden Bohrloch-Säulen
    double      abstandSkala;											// Abstand der Tiefen-Skala vom Bohrloch-Zentrum
    double      vertikaleSkalierung;							// Vertikale Skalierung der Bohrlochsäulen
    double      szenenrand;												// Größe des Szenenrandes
    double      abstandSkalenwerte;								// Abstand der Beschriftungen 
    double      breiteUeberschrift;								// Breite der Szenen-Überschrift
    double      hoeheUeberschrift;								// Höhe der Szenen-Überschrift

    bool        showLithology;										// Anzeige der Lithologie in der Szene
    bool        showStratigraphy;									// Anzeige der Stratigraphie in der Szene
    bool        showSkala;												// Anzeige der Bohrsäulen-Skala
    bool        showBoreholeAttribute;						// Anzeige der Bohrloch-Attribute

    LITHOLOGY_VISUALISATION_STRATEGY lithologyVisualisationStrategy;  // Typ der Lithologie Visualisierung

    std::map<std::string,SchichtDarstParameter*> mLithologyDarstellungMap;	// Darstellungsparameter Lithologische Schichten
    std::map<std::string,SchichtDarstParameter*> mStratigraphyDarstMap;			// Darstellungsparameter stratigraphische Schichten

  public:
    void setShowLithology ( bool b ) {  showLithology = b;  }
    bool getShowLithology()  {  return showLithology;  }
    
    void setShowStratigraphy ( bool b ) {  showStratigraphy = b;  }
    bool getShowStratigraphy()  {  return showStratigraphy;  }

    void setShowSkala ( bool b ) {  showSkala = b;  }
    bool getShowSkala()  {  return showSkala;  }

    void setShowBoreholeAttribute ( bool b ) {  showBoreholeAttribute = b;  }
    bool getShowShowBoreholeAttribute()  {  return showBoreholeAttribute;  }

    void setLithologyVisualisationStrategy ( LITHOLOGY_VISUALISATION_STRATEGY s )  {  lithologyVisualisationStrategy = s;  }
    LITHOLOGY_VISUALISATION_STRATEGY getLithologyVisualisationStrategy ()  {  return lithologyVisualisationStrategy;  }

    void setBeschreibung ( std::string beschreibungP )  {  beschreibung = beschreibungP;  }
    std::string getBeschreibung ()  {  return beschreibung;  }

    void setSignatureFolder ( std::string s )  {  signatureFolder = s;  }
    std::string getSignatureFolder()  {  return signatureFolder;  }

    void setBoreholeDurchmesserLithologie ( double d )  {  boreholeDurchmesserLithologie = d;  }
    double getBoreholeDurchmesserLithologie()  {  return boreholeDurchmesserLithologie;  }

    void setBoreholeDurchmesserStratigraphie ( double d )  {  boreholeDurchmesserStratigraphie = d;  }
    double getBoreholeDurchmesserStratigraphie()  {  return boreholeDurchmesserStratigraphie;  }

    void setAbstandSaeulen ( double d )  {  abstandSaeulen = d;  }
    double getAbstandSaeulen()  {  return abstandSaeulen;  }

    void setAbstandSkala ( double d )  {  abstandSkala = d;  }
    double getAbstandSkala()  {  return abstandSkala;  }

    void setAbstandSkalenwerte ( double d )  {  abstandSkalenwerte = d;  }
    double getAbstandSkalenwerte()  {  return abstandSkalenwerte;  }

    void setVertikaleSkalierung ( double d )  {  vertikaleSkalierung = d;  }
    double getVertikaleSkalierung()  {  return vertikaleSkalierung;  }

    void setBreiteUeberschrift ( double d )  {  breiteUeberschrift = d;  }
    double getBreiteUeberschrift()  {  return breiteUeberschrift;  }

    void setHoeheUeberschrift ( double d )  {  hoeheUeberschrift = d;  }
    double getHoeheUeberschrift()  {  return hoeheUeberschrift;  }

    void addLithologyDarstellung ( std::string rockName, SchichtDarstParameter * pSchichtDarstellung );
    SchichtDarstParameter * getLithologyDarstellung ( std::string rockName );
    SvgColor              * getLithologyColor ( std::string rockName );

    void addStratigraphyDarstellung  ( std::string rockName, SchichtDarstParameter * pSchichtDarstellung );
    SchichtDarstParameter * getStratigraphyDarstellung ( std::string chronoStratigraphy );
    SvgColor              * getStratigraphyColor ( std::string chronoStratigraphy );

    void getSceneViewbox ( double totalLength, double viewbox[] );

};

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Parameter zur 2D-Bohrlochdarstellung                        //
///////////////////////////////////////////////////////////////////////////////
public ref class BoreholeDarstellungReader
{
  public:
    BoreholeDarstellungReader();
    virtual ~BoreholeDarstellungReader();

  protected:
    XmlTextReader ^ reader;				// .NET XML-Textreader
    String        ^ pActElement;	// Aktuell gelesenen XML element

  public:
    bool read ( std::string fileName, BoreholeDarstellung * pBoreholeDarstellung );

  protected:  
    Pattern  * readPattern ();
    Hatching * readHatching ();
    void readGeometryParams ( SvgGeometry * pSvgGeometry, String ^endTag );

};

///////////////////////////////////////////////////////////////////////////////
//  Generierung der SVG-Datei zur 2D-Visualisierung eines Bohrlochs          //
///////////////////////////////////////////////////////////////////////////////
public ref class BoreholeSvgWriter
{
  public:
    BoreholeSvgWriter ( BoreholeVisualisation2D * pBoreholeVisualisationP );
    virtual ~BoreholeSvgWriter ();

  protected:
    XmlTextWriter ^ writer;

    BoreholeVisualisation2D * pBoreholeVisualisation;	// Objekt zur 2D-Visualisierung von Bohrlöchern
    BoreholeDarstellung     * pBoreholeDarstellung;		// Visualisierungsparameter
    Feature                 * pBorehole;							// Darzustellendes Bohrloch

    double lithologyAchse;		// Position der Lithologie-Achse
    double stratigraphyAchse;	// Position der Stratigraphie-Achse
    double skalenAchse;				// Position der Skalen-Achse

    static const double largeNumber = 1000;  // Hilfs-Zahl bei der Generierung von Schraffuren

  public:
    void write ( std::string fileName );
    void writeUeberschrift ( std::string fileName );

  protected:
    bool writeSvgRoot ();
    void writeSvgStyles();

    void writeLithologyInterval    ( LithologyInterval * pInterval );
    void writeStratigraphyInterval ( StratigraphyInterval * pInterval );
    void writeSkala();

    void writeSvgPattern ( SvgColor * pColor, Pattern * pPattern );
    void writeGeometryElement ( SvgGeometry * pSvgGeometryP );
    void writeSvgHatching ( SvgColor * pColor, Hatching * pHatching );
    void writeSvgImagePattern ( SvgColor * pColor, std::string signaturePath, double width );
    void writeSvgInterval ( double oben, double unten, double links, double rechts, SchichtDarstParameter * pParam );
    
    void writeTooltipLithology ( LithologyInterval * pInterval );
    void writeAttributeDataLithology ( LithologyInterval * pInterval );

    void writeTooltipStratigraphy ( StratigraphyInterval * pInterval );
    void writeAttributeDataStratigraphy ( StratigraphyInterval * pInterval );
};












