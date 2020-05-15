#pragma once

#include "shptypeenum.h"
#include "ShpFileNamenKonv.h"
#include "includes.h"

class ShapeFileObjekt;
class ShapeFileAttribut;
class ShapeFileAttributWert;
class PlanAttribute;
class GmlKlasse;
class GmlSchema;
class GmlAttribut;
class StandardAttribut;
class GmlEnumeration;
class GmlEnumerationItem;
class Bereich;

///////////////////////////////////////////////////////////////////////////////
//  class ShapeFileObjekte						                        							 //
//  Modelliert die Struktur einer Anzahl von Shape-Files			            	 //
///////////////////////////////////////////////////////////////////////////////
class ShapeFileObjekte
{
public:
	ShapeFileObjekte ( XPlanGML * pXPlanGML);
	virtual ~ShapeFileObjekte();

public:
	bool addShapeFileObjekt ( ShapeFileObjekt * pShapeFileObjekt );
	
	size_t getShapeFileObjekte ( std::vector<ShapeFileObjekt*> & shapeFileObjekteP );
	ShapeFileObjekt * getShapeFileObjekt ( std::string shapeFileNameP );

	void initEnumConversion ( GmlSchema * pGmlObjekteP );
	size_t getUsedEnums ( std::vector<GmlEnumeration*> &gmlEnumerationsP, 
					   std::vector<ShapeFileObjekt*> & shapeFilesP,
					   std::vector<ShapeFileAttribut*> &shapeFileAttributeP );

	void resetGmlZuordnung();
	ShapeFileNamenKonvertierung * generateConversion();

	void setPlaeneAusShapeFileErzeugen ( bool b )  {  plaeneAusShapeErzeugen = b;  }
	bool getPlaeneAusShapeFileErzeugen ()  {  return plaeneAusShapeErzeugen;  }

private:
	XPlanGML					                   * m_pXPlanGML;						 // XPlanGML Modul
	bool								                   plaeneAusShapeErzeugen; // Steuert, ob mehrere Planobjekte in einer Konversion erzeugt werden 
	std::map<std::string,ShapeFileObjekt*> shapeFileObjekte;			 // Map der Shapefile-Repräsentationen

	std::vector<GmlEnumeration*>    gmlEnumerations;				// Liste der Enumerationen
	std::vector<ShapeFileObjekt*>   shapeFiles;							// Liste der Shapefile-Objekte
	std::vector<ShapeFileAttribut*> shapeFileAttribute;			// Liste der Shapefile-Attribute

};

///////////////////////////////////////////////////////////////////////////////
//  class ShapeFileObjekt					                          								 //
//  Modelliert die Struktur eines einzelnen Shapefiles				          		 //
///////////////////////////////////////////////////////////////////////////////
class ShapeFileObjekt
{
public:
	ShapeFileObjekt ( std::string shapeFileNameP );
	virtual ~ ShapeFileObjekt ();

public:
	std::string getShapeFileName ()  {  return shapeFileName;  }

	void setShapeGeometrieTyp ( SHPTypeEnum geometrieTypP )  { geometrieTyp = geometrieTypP;  }
	SHPTypeEnum getShapeGeometrieTyp ()  {  return geometrieTyp;  }

	void setGmlKlasse ( GmlKlasse * pGmlKlasseP )
	{ 
		if ( pGmlKlasseP != pGmlKlasse ) resetGmlZuordnung();
		pGmlKlasse = pGmlKlasseP;  
	}
	GmlKlasse * getGmlKlasse()  {  return pGmlKlasse;  }

	bool addShapeFileAttribut ( ShapeFileAttribut * pShapeFileAttributP );

	size_t getShapeFileAttribute ( std::vector<ShapeFileAttribut*> & shapeFileAttributeP );
	ShapeFileAttribut * getShapeFileAttribut ( std::string shapeAttributNameP );

	void addStandardAttribut ( StandardAttribut * pStandardAttributP );
	bool removeStandardAttribut ( GmlAttribut * pGmlAttributP );

	size_t getStandardAttribute ( std::vector<StandardAttribut*> & standardAttributeP );
	size_t getStandardAttributAnz()  {  return standardAttribute.size();  }
	StandardAttribut * getStandardAttribut ( unsigned int index );
	StandardAttribut * getStandardAttribut ( GmlAttribut * pGmlAttributP );
	size_t getStandardAttribute ( GmlAttribut * pGmlAttributP, std::vector<StandardAttribut*> & standardAttributeP );

	void resetGmlZuordnung();
	void generateConversion( ShapeFileNamenKonvertierung * pKonvP );

	void setBereichNummer ( int nummerP )  {  bereichNummer = nummerP;  }
	int getBereichNummer ()  {  return bereichNummer;  }

	void setNameAttributPlanId ( std::string name )  {  attributNamePlanId = name;  }
	std::string  getAttributNamePlanId ()  {  return attributNamePlanId;  }

  void setRechtlichebedeutung ( int  b )  {  rechtlicheBedeutung = b;  }
  int getRechtlicheBedeutung() { return rechtlicheBedeutung;  }


private:
	std::string          shapeFileName;				// Name des Shapefiles
	SHPTypeEnum          geometrieTyp;				// Geometrie-Typ
	int		               bereichNummer;				// Nummer des zugeordneten Plan-Bereichs
	std::string          attributNamePlanId;	// Name des Attributes, das eine zugeordnete Plan-Id enthält
  int                  rechtlicheBedeutung;	// Rechtliche Bedeutung der Inhalte des Shapefiles

	std::map<std::string,ShapeFileAttribut*> shapeFileAttribute;  // Map der Shapafile Attribut-Objekte
	std::vector<StandardAttribut*> standardAttribute;							// Liste der vorbelegten XPlanGML-Attribute

	GmlKlasse * pGmlKlasse;		// XPlamGML-Klasse, die den Shapefile zugeordnet ist
};

///////////////////////////////////////////////////////////////////////////////
//  class ShapeFileAttribut					                        								 //
//  Modelliert ein Attribut eines Shapefiles		                						 //
///////////////////////////////////////////////////////////////////////////////
class ShapeFileAttribut
{
public:
	ShapeFileAttribut ( std::string shapeFileAttributNameP );
	virtual ~ ShapeFileAttribut ();

public:
	std::string getShapeFileAttributName ()  {  return shapeFileAttributName;  }

	void setShapeAttributDatentyp ( ATTRIBUT_TYP datentypP )  {  datentyp = datentypP;  }
	ATTRIBUT_TYP getShapeAttributDatentyp ()  {  return datentyp;  }


	void addGmlAttribut ( GmlAttribut * pGmlAttribut );
	void setGmlAttribut ( unsigned int index, GmlAttribut * pGmlAttribut );
	void removeGmlAttribut ( unsigned int index );
	size_t getGmlAttribute ( std::vector<GmlAttribut*> & gmlAttributeP );

	bool addShapeFileAttributWert ( ShapeFileAttributWert * pShapeFileAttributWertP );

	bool existShapeFileAttributWert (  std::string shapeFileAttributWert );
	ShapeFileAttributWert * getShapeFileAttributWert ( std::string shapeFileAttributWert );
	size_t getShapeFileAttributWerte ( std::vector<ShapeFileAttributWert*> & shapeFileAttributWerteP );

	void resetGmlZuordnung();
  void resetAttributWerte();

	void setGenerAttributName ( std::string nameP )  {  generAttributName = nameP;  }
	std::string getGenerAttributName ()  {  return generAttributName;  }

	void setRefAttributName ( std::string nameP )  {  refAttributName = nameP;  }
	std::string getRefAttributName ()  {  return refAttributName;  }

	void setRefAttributTyp ( ATTRIBUT_TYP typP )  {  refAttributTyp = typP;  }
	ATTRIBUT_TYP getRefAttributTyp ()  {  return refAttributTyp;  }

	void setRefAttributIndex ( int index )  {  refAttributIndex = index;  }
	int  getRefAttributIndex()  {  return refAttributIndex;  }

private:
	std::string shapeFileAttributName;					// Attribut-Name im Shapefile
	ATTRIBUT_TYP datentyp;											// Typ des Shapefile Attributs

	std::vector<GmlAttribut*> gmlAttribute;					// Zugeordnete XPlanGML-Attribute
	std::string               generAttributName;			// Bei Zuordnung Generisches Attribut: Attribut-Name
//	std::string								generAttributBedeutung;	
	std::string								refAttributName;			// Bei Zoordnung komplexer Attribute: Attribut-Name im Komplex-Attribut
	ATTRIBUT_TYP							refAttributTyp;			  // Bei Zoordnung komplexer Attribute: Attribut-Typ im Komplex-Attribut
	int												refAttributIndex;			// Bei Zoordnung komplexer Attribute: Index im Komplex-Attribut

	std::map<std::string,ShapeFileAttributWert*> shapeFileAttributWerte ;  // map: Shapefile Attributwert --> Attributwert-Objekt
};


///////////////////////////////////////////////////////////////////////////////
//  class StandardAttribut								                        					 //
//  Vorgabe eines Attribut-Standardwertes für alle aus dem Shapefile         //
//  konvertierten Objekte						                          							 //
///////////////////////////////////////////////////////////////////////////////
class StandardAttribut
{
public:
	StandardAttribut ( GmlAttribut * pGmlAttributP );
  StandardAttribut ( StandardAttribut * StandardAttributOld );

	virtual ~StandardAttribut();

public:
	GmlAttribut * getGmlAttribut()  {  return pGmlAttribut;  }

	void setDefaultWert ( std::string defaultWertP )  {  defaultWert = defaultWertP;  }
	std::string getDefaultWert ()  {  return defaultWert;  }

  void setComplexDefaultWert (Feature * pComplexDefaultWertP );
  Feature * getComplexDefaultWert ()  {  return pComplexDefaultWert; }

	void setIndex ( int i ) {  index = i;  }
	int getIndex()  {  return index;  }

private:
	GmlAttribut * pGmlAttribut;					// Zugeordnetes XPlanGML Attribut

	std::string defaultWert;						// Standardwert (Simpler Attributtyp)
  Feature   * pComplexDefaultWert;    // Standardwert (Komplexer Attributtyp)
	int         index;									// Index
};



///////////////////////////////////////////////////////////////////////////////
//  class ShapeFileAttributWert			                      									 //
//  Modelliert einen Wert, den ein Shapefile-Attribut annimmt		        		 //
///////////////////////////////////////////////////////////////////////////////
class ShapeFileAttributWert
{
public:
	ShapeFileAttributWert ( std::string wertP );
	virtual ~ShapeFileAttributWert();

public:
	std::string getAttributWert ()  {  return attributWert;  }

	void setEnumerationItem ( GmlEnumeration * pEnumerationP, GmlEnumerationItem * pEnumerationItemP );
	GmlEnumerationItem * getEnumerationItem ( GmlEnumeration * pEnumerationP );
	void resetGmlZuordnung( GmlEnumeration * pEnumerationP );
	void resetGmlZuordnung();

  std::string getCodeListDictionary();

private:
	std::string attributWert;			// Shapefile Attributwert

	std::vector<GmlEnumeration*>      vEnumerationen;				// Dem Attributwert zugeordnete XPlanGML Enumerationen
	std::vector<GmlEnumerationItem* > vEnumerationsItems;	  // Dem Attributwert zugeordnete XPlanGML Enumerations-Einträge
};











