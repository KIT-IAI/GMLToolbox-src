#pragma once
#include "shptypeenum.h"
#include "includes.h"
#include "CoordPair.h"

class Features;
class ShapeFileNamenKonvertierung;
class ShapeFileObjekte;
class PlanAttribute;
class ShapeFileObjekt;
class KlassenName;
class TransformationINSPIRE_PLU_Parameter;
class TransformationINSPIREProtokoll;
class MigrationProtokoll;
class UOMList;
class Logfile;
class SvgVisualisierung;
class BLeitPlan;
class PlanBereich;
class Feature;
class Features;
class UOMList;
class AttributName;
class GmlKlasse;
class Bereich;


///////////////////////////////////////////////////////////////////////////////
//  class XPlanGML                                                           //
//  Verarbeitung von 2D-GML Daten (XPlanGML, INSPIRE-PLU, ALKIS)              //
///////////////////////////////////////////////////////////////////////////////
class XPlanGML
{
public:
  XPlanGML ( Features * pFeatures, UOMList * pUOMList, Logfile * pLogfile );
  virtual ~ XPlanGML();

protected:
  enum SHAPE_FILE_MODE {  ANALYSE, KONVERSION };			//  Modus bei der Verarbeitung von Shapefiles

  Features          * m_pFeatures;					// Modul Features
  UOMList           * m_pUOMs;							// Modul Units of Measurement
  Logfile           * m_pLogfile;						// Fehler/Meldungne Logfile

  //  Klasse zur Konvertierung Shape-Files --> GML
  ShapeFileNamenKonvertierung * m_pShapeFileNamenKonvertierung;
  std::string                   m_konvertierungsDatei;  // Pfadname der Konvertierungs-Datei

  std::string   m_planGMLId;             //  GML-Id des durch Konversion erzeugten Plans
  BLeitPlan   * m_pBLeitPlan;						 // Objekt des erzeugten Plans


  //  Modell des GML-Schemas
  XPLAN_TYP		        m_planTyp;									// Plan-Typ
  std::string         m_planKlassenName;					// Name der Plan-Klasse
  std::string         m_planBereichKlassenName;		// Name der Bereich-Klasse

  //  Referierte Objekte w‰hrend der Konversion
  std::map<std::string,Feature*>  m_mPlanObjekteKonversion;		

  //  Attribute Shapefile-Konvertierung
  std::vector<std::string> m_vShapeFilePaths;  //  Vektor der zu konvertierenden Shapefiles (Pfadnamen)
  std::vector<std::string> m_vShapeFileNames;  //  Vektor der zu konvertierenden Shapefiles (Dateinamen)
  std::string				       m_shapeFilePathRaeumlicherGeltungsbereich;	// Shapefile-Pfad des r‰umlichen Geltungsbereichs

  ShapeFileObjekte       * m_pShapeFileObjekte;		   // Repr‰sentation der Shapefiles
  PlanAttribute          * m_pPlanAttribute;			   // Attribute des Plan-Objektes
  ShapeFileObjekt        * m_pShapeFileObjektAkt;    // Aktuell analysierter Shapefile
  SHAPE_FILE_MODE          m_shapeFileMode;					 // Bearbeitungs-Modus der Shapefiles
  bool					           m_plaeneAusShapeErzeugen; // Steuert, ob mehrere Planobjekte aus den Shapefiles erzeugt werden
  std::vector<Feature*>    m_vObjekteInShapeFile;    // Objekte in einem Shapefile

  std::string	     m_klassenName;						// Name der GML-Klasse, die dem Shapefile zugeordnet ist
  KlassenName    * m_pKlassenName;					// Zug. KlassenName Objekt
  std::string      m_shapeFileName;					// Name des aktuell konvertierten Spapefiles	
  OBJEKT_TYP	     m_objektTyp;							// Geometrie-Typ dieses Files
  std::string		   m_fehlermeldung;					// Fehlermeldung
  bool			       m_readBereichGeometrie;  // Steuert, ob dem aktuellen Plan-Bereich ein Shapefile zugeordnet ist  
  int				       m_bereichNummer;					// Nummer des aktuellen Plan-Bereichs

  //  Attribut-Namem des aktuellen Shapefiles zur Analyse
  std::vector<std::string> m_vStringAttributNamen;		// Namen der String-Attribute
  std::vector<std::string> m_vIntegerAttributNamen;		// Namen der Interger-Attribute 
  std::vector<std::string> m_vDoubleAttributNamen;		// Namen der Double-A>ttribute
  std::vector<std::string> m_vDateAttributNamen;			// Namen der Datum-Attribute

  //  Attribute des aktuellen Shapefiles nach GML-Konversion
  std::vector<AttributName*> m_vStringAttributObjekte;		// Abbildungsvorschriften String-Attribute
  std::vector<unsigned int>  m_vStringAttributAnz;				// Anzahl der jeweils zugeordneten String-Attributwerte
  std::vector<AttributName*> m_vIntegerAttributObjekte;		// Abbildungsvorschriften Integer-Attribute
  std::vector<unsigned int>  m_vIntegerAttributAnz;				// Anzahl der jeweils zugeordneten Integer-Attributwerte
  std::vector<AttributName*> m_vDoubleAttributObjekte;		// Abbildungsvorschriften Double-Attribute
  std::vector<unsigned int>  m_vDoubleAttributAnz;				// Anzahl der jeweils zugeordneten Double-Attributwerte
  std::vector<AttributName*> m_vDateAttributObjekte;			// Abbildungsvorschriften Datum-Attribute
  std::vector<unsigned int>  m_vDateAttributAnz;					// Anzahl der jeweils zugeordneten Datum-Attributwerte

  int  m_anzObjRead;						// Anzahl der konvertierten Objekte
  int  m_anzObjNotRead;					// Anzahl der nicht konvertierten Objekte
  int  m_anzShapeFileRead;			// Anzahl der konvertierten Shapefiles
  int  m_anzShapeFileNotRead;		// Anzahl der nicht konvertierten Shapefiles

  TransformationINSPIRE_PLU_Parameter * m_pTransformationINSPIRE_PLU_Parameter;  // Parameter zur Transformation XPlanGML 4.0 -> INSPIRE PLU

public:
  Features * getFeatures()  {  return m_pFeatures;  }
  UOMList  * getUOMList()   {  return m_pUOMs;     }
  Logfile  * getLogfile()   {  return m_pLogfile;  }

  void setPlanTyp( XPLAN_TYP planTypP ); 
  XPLAN_TYP getPlanTyp()  {  return  m_planTyp;  }
  std::string getPlanTypAsString();
  static std::string planTypToString (  XPLAN_TYP planTypP );
  static XPLAN_TYP planStringToTyp ( std::string planTypP );

  GmlKlasse * getGmlPlanKlasse();

  std::string createGmlIdFromShapefile ( std::string shapefileName, int FID );

  std::string getBLeitPlanGMLId ()  {  return  m_planGMLId;  }

  PlanAttribute * createPlanAttribute();

  void addPlanObjektKonversion ( std::string uuidP, Feature * pPlanObjekt );
  Feature * getPlanObjektKonversion ( std::string uuidP );
  void resolveReferencesKonversion();
  void clearPlanObjekteKonversion();

  void setShapeFiles ( std::vector<std::string> shapeFilePathsP );
	size_t getShapefileAnz()  {  return m_vShapeFilePaths.size();  }

  bool updateConversion();
  bool setConversionFile ( std::string dateiP );
  std::string getConversionFile ();
  bool existConversion();
  ShapeFileNamenKonvertierung * getKonv()  {  return m_pShapeFileNamenKonvertierung;  }

  void saveConversion();
  void saveConversionAs ( std::string dateiP );

  bool analyzeShapeFiles( int & anzErrors );
  bool analyzeShapeFile ( std::string shapeFilePathP );

  bool convertShapeFiles();
  ShapeFileObjekte * getShapeFileObjekte()  {  return m_pShapeFileObjekte;  }

  bool setPlanAttribute ();

  bool setBereichAttribute_V2 ( PlanBereich * pPlanBereich, Bereich * pBereich );
  bool setBereichAttribute_V3 ( PlanBereich * pPlanBereich, Bereich * pBereich );
  bool setBereichAttribute_V4 ( PlanBereich * pPlanBereich, Bereich * pBereich );
  bool setBereichAttribute_V5_0 ( PlanBereich * pPlanBereich, Bereich * pBereich );
  bool setBereichAttribute_V5_X ( PlanBereich * pPlanBereich, Bereich * pBereich );


  bool readShapeFile ( std::string shapeFileP );
  PlanAttribute * getPlanAttribute()  {  return m_pPlanAttribute;  }

  int getAnzObjRead ()		       {  return m_anzObjRead;  }
  int getAnzObjNotRead ()        {  return m_anzObjNotRead;  }
  int getAnzShapeFileRead ()     {  return m_anzShapeFileRead;  }
  int getAnzShapeFileNotRead ()  {  return m_anzShapeFileNotRead;  }

  bool setShapeFileStructure
    ( SHPTypeEnum               shapeFileTyp, 
    std::string&              shapeFileName, 
    std::vector<std::string>& vStrAttrN,	// Vektor der String-Attribut Namen
    std::vector<std::string>& vIntAttrN,		// Vektor der Integer-Attribut Namen
    std::vector<std::string>& vDblAttrN,
    std::vector<std::string>& vDateAttrN);

  void addShpPoint (
    SHPTypeEnum							shapeFileTyp,  	// Typ des Geometrie-Objektes:	
    const std::string&					shapeFileName,	// Layer-Name
    const std::vector<CoordPair>&		vCoords,	// Vektor der Punkt-Koordinaten
    const std::vector<std::string>&		vStrAttrV,	// Vektor der String-Attribut Werte
    const std::vector<int>&				vIntAttrV,	// Vektor der Integer-Attribut Werte
    const std::vector<double>&			vDblAttrV,  // Vektor der Double-Attribut Werte
    const std::vector<std::string>&		vDateAttrV ); // Vektor der Date-Attribut Werte

  void addShpLine ( 
    SHPTypeEnum		 					shapeFileTyp,		// Typ des Geometrie-Objektes:
    const std::string&		 			shapeFileName,	// Layer-Name
    const std::vector<CoordPair>& 		vCoords,	// Vektor der Punkt-Koordinaten
    const std::vector<int>&	 			vPartStarts,// Vektor der Anfangs-Indizes der Parts
    const std::vector<std::string>&	 	vStrAttrV,	// Vektor der String-Attribut Werte
    const std::vector<int>&	 			vIntAttrV,	// Vektor der Integer-Attribut Werte
    const std::vector<double>&	 		vDblAttrV,  // Vektor der Double-Attribut Werte
    const std::vector<std::string>&		vDateAttrV ); // Vektor der Date-Attribut Werte

  void addShpPolygon(
    SHPTypeEnum	 						shapeFileTyp,		// Typ des Geometrie-Objektes
    const std::string&		 			shapeFileName,	// Layer-Name
    const std::vector<CoordPair>& 		vCoords,	// Vektor der Punkt-Koordinaten
    const std::vector<int>&	 			vRingStarts,// Vektor der Anf.-Indizes der Ringe
    const std::vector<int>& 	        vRingTypes,	// Vektor der Ring-Typen
    const std::vector<std::string>&	 	vStrAttrV,	// Vektor der String-Attribut Werte
    const std::vector<int>&	 			vIntAttrV,	// Vektor der Integer-Attribut Werte
    const std::vector<double>&	 		vDblAttrV, // Vektor der Double-Attribut-Werte
    const std::vector<std::string>&		vDateAttrV ); // Vektor der Date-Attribut Werte
  //  Bedeutung des Vektors vRingTypes:
  //  Jeder ‰uﬂere Ring erh‰lt eine eindeutige Kennung k > 0
  //  Jedes Loch im ‰uﬂeren Ring mit Kennung k erh‰lt die Kennung -k


  std::string getRelationsNameFachobjekt ();
  std::string getRelationsNamePlanbereich ();

  void setDefaultAttributWerte ( Feature * pObjP, std::string shapeFileNameP );
  void setDefaultKomplexAttributWerte ( Feature * pObjP, std::string shapeFileNameP );


  OBJEKT_TYP getObjektTyp ( std::string objektNameP );

  ATTRIBUT_TYP getAttributTyp ( std::string klassenNameP, 
    std::string attributNameP, std::string & enumerationNameP );
  ATTRIBUT_TYP getAttributTyp ( std::string klassenNameP,	std::string attributNameP );

  bool setStringAttribute ( Feature * pFeature, std::vector<AttributName*> vAttributNamenP, 
              std::vector<unsigned int> vAttributAnzP, std::vector<std::string> vAttributWerteP );
  void setIntegerAttribute ( Feature * pFeature,std::vector<AttributName*> vAttributNamenP,
              std::vector<unsigned int> vAttributAnzP, std::vector<int> vAttributWerteP );
  void setDoubleAttribute ( Feature * pFeature,std::vector<AttributName*> vAttributNamenP, 
              std::vector<unsigned int> vAttributAnzP, std::vector<double> vAttributWerteP );
  void setDateAttribute ( Feature * pFeature,std::vector<AttributName*> vAttributNamenP, 
              std::vector<unsigned int> vAttributAnzP, std::vector<std::string> vAttributWerteP );

  TransformationINSPIRE_PLU_Parameter * readINSPIRE_PLUTransformation( GML_SCHEMA_TYPE versionXPlanGML );

  TransformationINSPIREProtokoll * transformationINSPIRE_PLU(  std::string quellDatei, std::string zielOrdner,
    Features * pPlanObjekteINSPIRE, TransformationINSPIRE_PLU_Parameter * pTransParam, int outputFormat, GML_SCHEMA_TYPE versionXPlanGML );


  MigrationProtokoll *  migrationDatei (  std::string quellDatei, std::string zielOrdner, std::string protokollDatei, 
    bool ueberschreiben, std::string versionXPlanGMLQuelle, std::string versionXPlanGMLZiel );
  MigrationProtokoll *  migrationOrdner ( std::string quellOrdner, std::string zielOrdner, std::string protokollDatei,
    bool ueberschreiben, std::string versionXPlanGMLQuelle, std::string versionXPlanGMLZiel );
};