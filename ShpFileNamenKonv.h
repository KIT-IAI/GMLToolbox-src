#pragma once
#include <map>
#include <string>
#include <vector>
#include "includes.h"

class XPlanGML;
class KlassenName;
class Enumeration;
class AttributName;
//class VerfahrensMerkmal;
class ShapeFileObjekt;
class GmlSchema;
class ExterneReferenz;
class Bereich;
class GenerAttribut;
class TextAbschnitt;
class GenerStringAttribut;
class GenerIntegerAttribut;
class GenerDoubleAttribut;
class GenerDateAttribut;
class GenerURLAttribut;
class RasterplanBasis;
class RasterplanAenderung;
class Feature;

///////////////////////////////////////////////////////////////////////////////
// PlanAttribute																														 //
//  Während der Transformation Shapefile --> XPlanGML festgelegte					   //
//  Attributwerte des gesamten Plans																				 //
///////////////////////////////////////////////////////////////////////////////
class PlanAttribute
{
public:
	PlanAttribute ();
	PlanAttribute ( PlanAttribute * pPlanAttributeOld );
	virtual ~ PlanAttribute();

public:
	void addStringAttribut ( std::string attributNameP, std::string attributWertP );
	void addIntegerAttribut ( std::string attributNameP, int attributWertP );
	void addDoubleAttribut ( std::string attributNameP, double attributWertP );
	void setBoolAttribut ( std::string attributNameP, bool attributWertP );
	void addDateAttribut ( std::string attributNameP, std::string attributWertP );
	void addURLAttribut ( std::string attributNameP, std::string attributWertP );
	void addKomplexAttribut ( std::string attributNameP, Feature * pAttr );

	void removeStringAttribut ( std::string attributNameP, std::string attributWertP );
	void removeStringAttribut ( std::string attributNameP );
	void removeIntegerAttribut ( std::string attributNameP );
	void removeIntegerAttribut ( std::string attributNameP, int attributWertP );
	void removeDoubleAttribut ( std::string attributNameP );
	void removeDoubleAttribut ( std::string attributNameP, double attributWertP );
	void removeBoolAttribut ( std::string attributNameP );
	void removeDateAttribut ( std::string attributNameP );
	void removeDateAttribut ( std::string attributNameP, std::string attributWertP );
	void removeURLAttribut ( std::string attributNameP );
	void removeURLAttribut ( std::string attributNameP, std::string attributWertP );
	void removeKomplexAttribut ( std::string attributNameP );

	bool getStringAttributWert ( std::string attributName, std::string & attributWert );
	bool getIntegerAttributWert ( std::string attributName, int & attributWert );
	bool getDoubleAttributWert ( std::string attributName, double & attributWert );
	bool getBoolAttributWert ( std::string attributName, bool & attributWert );
	bool getDateAttributWert ( std::string attributName, std::string & attributWert );
	bool getURLAttributWert ( std::string attributName, std::string & attributWert );
	Feature * getKomplexAttributWert ( std::string attributName );

	size_t getStringAttributWerte ( std::vector<std::string> & vAttributName, std::vector<std::string> & vStringP );
	size_t getIntegerAttributWerte ( std::vector<std::string> & vAttributName, std::vector<int> & vIntP );
	size_t getDoubleAttributWerte ( std::vector<std::string> & vAttributName, std::vector<double> & vDoubleP );
	size_t getBoolAttributWerte ( std::vector<std::string> & vAttributName, std::vector<bool> & vBoolP );
	size_t getDateAttributWerte ( std::vector<std::string> & vAttributName, std::vector<std::string> & vStringP );
	size_t getURLAttributWerte ( std::vector<std::string> & vAttributName, std::vector<std::string> & vStringP );
	size_t getKomplexAttributWerte ( std::vector<std::string> & vAttributName, std::vector<Feature*> & vAttrP );
	size_t getKomplexAttributWerte ( std::string & attributName, std::vector<Feature*> &vKomplexAttribute );

	void addTextAbschnitt ( std::string schluesselP, std::string wertP, std::string wertURL, std::string planTypP );
	void setTextAbschnitt ( unsigned int indexP, std::string schluesselP, std::string wertP, 
													std::string wertURL, std::string planTypP );
	size_t getTextAbschnitte ( std::vector<TextAbschnitt*> &textAbschnitteP );
	size_t getTextAbschnittAnz()  {  return textAbschnitte.size();  }
	bool getTextAbschnitt ( unsigned int indexP, std::string &schluesselP, std::string &wertP,
							std::string &wertURLP, std::string &planTypP );
	bool schluesselTextAbschnittExist ( std::string &schluesselP );
	void deleteTextAbschnitt ( unsigned int indexP );

	void addBereich ( std::string nameP, std::string bedeutungP, std::string bedeutungEnum,
					  std::string geometrieDateiP, int massstab );
	void addBereich ( Bereich * pBereich );

	size_t getBereiche ( std::vector<Bereich*> &bereicheP );
	size_t getBereichAnz()  {  return bereiche.size();  }
	Bereich * getBereich ( unsigned int index );

	PlanAttribute operator= ( PlanAttribute & planAttributeP );
	void clear();


private:
	std::vector<Bereich*>			             bereiche;						 // Liste der Bereich-Objekte
	std::multimap<std::string,std::string> stringAttributePlan;  // String-Attribut Werte
	std::multimap<std::string,int>		     integerAttributePlan; // Integer-Attribut Werte
	std::multimap<std::string,double>	     doubleAttributePlan;	 // Double-Attribut Werte
	std::map<std::string,bool>		         boolAttributePlan;    // Bool-Attribut Werte
	std::multimap<std::string,std::string> dateAttributePlan;		 // Datum-Attribut Werte
	std::multimap<std::string,std::string> urlAttributePlan;		 // URL-Attribut Werte
  std::multimap<std::string,Feature*>    komplexAttributePlan; // Komplex-Attribute
	std::vector<TextAbschnitt*>		         textAbschnitte;       // Textuelle Planinhalte
};

///////////////////////////////////////////////////////////////////////////////
//  ShapeFileNamenKonvertierung																							 //
//  Konvertierung von Datei- und Attributnamen der Shape-Files in XPlanGML   //
//  Standards																                                 //
///////////////////////////////////////////////////////////////////////////////
class ShapeFileNamenKonvertierung
{
public:
public:
	ShapeFileNamenKonvertierung( XPlanGML * pXPlanGML );
	virtual ~ShapeFileNamenKonvertierung();

protected:
	XPlanGML												 * m_pXPlanGML;				     // XPlanGML-Modul
	std::map<std::string,KlassenName*> klassenNamenMap;		     // map der XPlanGML Klassen-Objekte
	std::map<std::string,Enumeration*> enumerationMap;	       // map der XPlanGML Enumerations-Objekte
	bool															 plaeneAusShapeErzeugen; // Gibt an, ob mehrere Pläne bei einer Konversion erzeugt werden
	int																 errorAnz;							 // Anzahl der aufgetretenen Fehler

public:
  XPlanGML * getXPlanGML()  {  return m_pXPlanGML;  }

	KlassenName * addKlassenName ( std::string & shpDateiNameP, std::string & klassenNameP, 
                                 int bereichNummer, std::string rechtlicheBedeutung );

	KlassenName * addKlassenName ( std::string & shpDateiNameP, std::string & klassenNameP, 
                                 int bereichNummer, int rechtlicheBedeutung );

	Enumeration * addEnumeration ( std::string & enumerationIdP );
	void addEnumeration ( Enumeration * pEnumerationP );

	KlassenName * getKlassenNameObj ( std::string &shapeFileNameP);
	size_t getKlassenNamen ( std::vector<KlassenName*> & klassenNamenP );
	std::string getKlassenName    ( std::string &shpDateiNameP );

	size_t getAttributObjekte (  std::string & shpDateiNameP, std::string & shpAttributNameP,
								  std::vector<AttributName*> & vAttrObjekteP );
	

	Enumeration  * getEnumeration (  std::string & enumerationIdP );
	size_t getEnumerations ( std::vector<Enumeration*> & enumerationsP );

	void setPlaeneErzeugen ( bool b )  {  plaeneAusShapeErzeugen = b;  }
	bool getPlaeneErzeugen ()  {  return plaeneAusShapeErzeugen;  }

	int adaptShapeFileObjekte();
};

///////////////////////////////////////////////////////////////////////////////
//  AttributName																														 //
//  Modelliert die Abbildung von Shapefile-Attributen auf XPlanGML-Attribute //
//  (nameAktuell != "", defaultWert == "") bzw. die Belegung von XPlanGML-   //
//  Attributen mit Standardwerten (nameAktuell = "", default != "")					 //
///////////////////////////////////////////////////////////////////////////////
class AttributName
{
public:
  AttributName();
  virtual ~AttributName();

public:
	std::string	 nameAktuell;						// Name des abzubildenden Shapefile-Attributs
	std::string	 nameStandard;					// Name des XPlanGML-Attributs, auf das abgebildet wird
	ATTRIBUT_TYP typ;										// Datentyp des XPlanGML-Attributs
	std::string	 enumerationId;					// Bei Abbildung auf XPlanGML Enumerationen oder Codelisten: Name der zug. Enumeration/Codeliste
	std::string  generAttributName;		  // Bei Abbildung auf XPlanGML Generische Attribute: Name des Generischen Attributs
	std::string	 defaultWert;						// Standardwert des XPlanGML Attributs "nameStandard" 
	ATTRIBUT_TYP subAttributTyp;				// Bei Abbildung auf ein komplexes XPlanGML Attribut: Typ des Sub-Attributs, auf das abgebildet wird
	std::string  subAttributName;				// Bei Abbildung auf ein komplexes XPlanGML Attribut: Name des Sub-Attributs
  Feature * defaultWertKomplexAttribut; // Bei Abbildung auf ein komplexes XPlanGML Attribut: Standardwert des komplexen Attributs

protected:
	int					 index;								  // Index bei der Mehrfach-Belegung von XPlanGML Attributen

public:
	int getIndex()  {  return index;  }
	void setIndex ( int indexP )  {  index = indexP;  }
};

///////////////////////////////////////////////////////////////////////////////
//  KlassenName														                                	 //
//  Modelliert die Abbildung eines Shapefiles auf eine XPlanGML-Klasse       //
///////////////////////////////////////////////////////////////////////////////
class KlassenName
{
public:
	KlassenName ( ShapeFileNamenKonvertierung * pParantP, 
    std::string & nameAktuellP, std::string & nameStandardP, int bereichNummerP, int rechtlicheBedeutung );
    virtual ~ KlassenName();

protected:
	ShapeFileNamenKonvertierung * pParant;								// Zug. Konvertierungs-Objekt
	std::string	                  nameAktuell;						// Name des Shapefiles
	std::string	                  nameStandard;						// Name der XPlanGML-Klasse
	int                           bereichNummer;					// Nummer des Plan-Bereichs, dem das erzeugte XPlanGML-Objekt zugeordnet wird
  int                           rechtlicheBedeutung;		// Rechtliche Bedeutung des XPlanGML-Objektes

	std::multimap<std::string,AttributName*> attributMap; // Zuordnung von shp-Attributen zu AttributenName Objekten
	std::multimap<std::string,AttributName*> standardMap; // Zuordnung von XPlanGML Attributen zu AttributName Objekten

public:
	void addAttributName ( std::string shpAttributNameP, std::string AttributNameP, int indexP, 
		    ATTRIBUT_TYP attributTypP, std::string generAttributNameP, 
				std::string enumerationIdP, std::string defaultP, 
				std::string subAttributNameP, ATTRIBUT_TYP subAttributTypP, Feature * pComplexAttribut );

	std::string getKlassenName()  {  return nameStandard;  }
	std::string getShapeFileName() {  return nameAktuell;  }
	int getBereichNummer ()  {  return bereichNummer;  }
  int getRechtlicheBedeutung()  {  return rechtlicheBedeutung;  }

	size_t getAttributObjekte (  std::string & shpAttributNameP,
							  std::vector<AttributName*> & vAttrObjekteP );

	size_t getAttributObjekteFromStandardNamen ( std::string & standardNameP,
									std::vector<AttributName*> & vAttrObjekteP );
	bool existAttributObjektToStandardName (  std::string & standardNameP );

	size_t getBPlanAttributObjekte ( std::vector<AttributName*> &vAttributeP );

	int adaptShapeFileObjekt( ShapeFileObjekt * pShapeFileObjektP, GmlSchema * pGmlObjekteP );

};

///////////////////////////////////////////////////////////////////////////////
//  Enumeration																															 //
//  Unterstützt die Transformation von Shapefile Attributwerten auf		       //
//  XPlanGML Enumerations- oder Codelist-Einträge								             //
///////////////////////////////////////////////////////////////////////////////
class Enumeration
{
public:
	Enumeration ( std::string & enumerationIdP );
	virtual ~ Enumeration();

protected:
	std::string					              enumerationId;					// Name der XPlanGML Enumeration
	std::string						            enumerationDictionary;  // Name des Codelist Dictionaries
	std::map<std::string,std::string> enumerationItems;				// Zuordnung: Shapefile-Attributwerte --> XPlanGML-Attributwerte
	
public:
	std::string getEnumerationId ()  {  return enumerationId;  }
	void addEnumerationItem ( std::string & nameAktuellP, std::string & nameStandardP );

	void setDictionary ( std::string name )   {  enumerationDictionary = name;  }
	std::string getDictionary()  {  return enumerationDictionary;  }

	std::string getEnumerationItem ( std::string & shpEnumItemNameP );
	size_t getEnumerationItems ( std::vector<std::string> & namenAktuellP, 
							  std::vector<std::string> & namenStandardP );
};

///////////////////////////////////////////////////////////////////////////////
//  TextAbschnitt																                             //
//  Modelliert einen Objekt XP_TextAbschnitt																 //
///////////////////////////////////////////////////////////////////////////////
class TextAbschnitt
{
	public:
		TextAbschnitt();
		TextAbschnitt ( TextAbschnitt * pTextAbschnittOldP );
		virtual ~ TextAbschnitt();

	public:
		std::string schluessel;		// XPlanGML-Attribut "schluessel"
		std::string text;					// XPlanGML-Attribut "text"
		std::string textURL;			// XPlanGML-Attribut "refText"
		std::string planTyp;      // Steuert, welche von XP_TextAbschnitt abgeleitete Klasse generiert wird:
};

///////////////////////////////////////////////////////////////////////////////
//  ExterneReferenz												                            		   //
//  Modelliert ein XPlanGML Objekt XP_ExterneReferenz											   //
///////////////////////////////////////////////////////////////////////////////
class ExterneReferenz
{
	public:
		ExterneReferenz();
		ExterneReferenz ( ExterneReferenz * pExterneReferenzOldP );
		virtual ~ ExterneReferenz();

	public:
		std::string  relationName;					// Name der zug. XPlanGML Relation
		std::string  informationssystemURL; // XPlanGML Attribut informationssystemURL
		std::string  referenzName;					// XPlanGML Attribut referenzName
		std::string  referenzURL;						// XPlanGML Attribut referenzURL
		std::string  beschreibung;					// XPlanGML Attribut beschreibung
		std::string  referenzMimeType;			// XPlanGML Attribut referenzMimeType
		std::string  georefURL;							// XPlanGML Attribut georefURL
		std::string  georefMimeType;				// XPlanGML Attribut georefMimeType
    std::string  datum;									// XPlanGML Attribut datum
};

///////////////////////////////////////////////////////////////////////////////
//  Bereich										                                							 //
//  Modelliert ein XPlanGML Objekt BP_Plan, FP_Plan, LP_Plan oder LP_Plan		 //
///////////////////////////////////////////////////////////////////////////////
class Bereich
{
	public:
		Bereich();
		Bereich ( Bereich * pBereichOldP );
		virtual ~ Bereich();

	public:
		int         nummer;							// XPlanGML Attribut nummer
		std::string name;								// XPlanGML Attribut name
		std::string bedeutung;					// XPlanGML Attribut detailliertteBedeutung
		std::string bedeutungEnum;			// XPlanGML Attribut bedeutung
		int			    massstab;						// XPlanGML-Attribut erstellungsMasstab
		std::string geometrieDatei;			// Shapefile-Name mit dem räumlichen Geltungsbereich des Bereichs
		std::string versionBauNVOText;	// XPlanGML Attribut versionBauNVOText (nur für Version <= 5.0)
    std::string versionBauNVO;			// XPlanGML Attribut versionBauNVO (Version <= 4.1) oder versionBauNVO (Version 5.0)
    std::string versionBauGBDatum;  // XPlanGML Attribut versionBauGBText (nur für Version <= 5.0)
    std::string versionBauGBText;		// XPlanGML Attribut versionBauGBDatum (nur für Version <= 5.0)
    std::string versionBROGDatum;		// XPlanGML Attribut versionBROGDatum
    std::string versionBROGText;		// XPlanGML Attribut versionBROGText
    std::string versionLPLGDatum;		// XPlanGML Attribut versionLPLGDatum
    std::string versionLPLGText;		// XPlanGML Attribut versionLPLGText

		RasterplanBasis                 * pRasterplanBasis;					// XPlanGML Objekt XP_RasterplanBasis
		std::vector<RasterplanAenderung*> vRasterplanAenderungen;		// XPlanGML Objekte XP_RasterplanAenderung

	public: 
		void setRasterplanBasis ( RasterplanBasis * pRasterplanBasisP ); 
		RasterplanBasis * getRasterplanBasis ()  {  return pRasterplanBasis;  }

		void addRasterplanAenderung ( RasterplanAenderung * pRasterplanAenderungP );
		RasterplanAenderung * getRasterplanAenderung ( unsigned int index );		
		size_t getRasterplanAenderungenAnz ()  {  return vRasterplanAenderungen.size();  }
};

///////////////////////////////////////////////////////////////////////////////
//  RasterplanBasis													                            		 //
//  Modelliert ein Objekt XP_RastenplanBasis (Version <= 4.1) oder           //
//  XP_Rasterdarstellung (XPlanGML 5.0)																			 //
///////////////////////////////////////////////////////////////////////////////
class RasterplanBasis
{
	public:
		RasterplanBasis();
		RasterplanBasis ( RasterplanBasis * pRasterplanBasisOld );
		virtual ~ RasterplanBasis();

	protected:
		ExterneReferenz			      	* pExterneReferenzText;					// XPlanGML Attribut refTect
		std::vector<ExterneReferenz*> vExterneReferenzenLegende;		// XPlanGML Attribut refLegende
		std::vector<ExterneReferenz*> vExterneReferenzenPlan;				// XPlanGML Attribut refScan

	public:
		void addExterneReferenz ( ExterneReferenz * pExterneReferenz );

		size_t getExterneReferenzen ( std::vector<ExterneReferenz*> & vExterneReferenzenP );

		ExterneReferenz * getExterneReferenzText()  {  return pExterneReferenzText;  }
		void deleteExterneReferenzText();

		size_t getExterneReferenzenLegendeAnz() {  return vExterneReferenzenLegende.size();  }
		ExterneReferenz *  getExterneReferenzLegende ( unsigned int index );
		void deleteExterneReferenzLegende ( unsigned int index );

		size_t getExterneReferenzenPlanAnz() {  return vExterneReferenzenPlan.size();  }
		ExterneReferenz *  getExterneReferenzPlan ( unsigned int index );
		void deleteExterneReferenzPlan ( unsigned int index );
};

///////////////////////////////////////////////////////////////////////////////
//  RasterplanAenderung							                          							 //
//  Dummy-Implementierung							                          						 //
///////////////////////////////////////////////////////////////////////////////
class RasterplanAenderung
{
	public:
		RasterplanAenderung();
		RasterplanAenderung( RasterplanAenderung * pRasterplanAenderungOld );
		virtual ~ RasterplanAenderung();

};






