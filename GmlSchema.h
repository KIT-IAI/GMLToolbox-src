#pragma once

#include "GmlNamespaces.h"
#include "includes.h"

class GmlSchemaType;
class GmlSchemaElement;
class GmlKlasse;
class GmlEnumeration;
class GmlAttribut;
class GmlEnumerationItem;
class GmlSchemaSimpleType;
class GmlSchemaComplexType;
class GmlDictionaryEntry;
class GmlSchemaAttribut;
class GmlSchemaElements;
class GmlSchemaGroups;
class GmlNamespaces;
class SchemaTypeNames;
class GmlNamespace;
class GmlSchemaGroup;
class GmlElement;
class TypeDefinitions;
class HierarchicalCodeListEntry;
class GmlDictionary;
class Features;

typedef std::vector<double> DoubleList;
///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaTypes                                                           //
//  Modelliert die Collection aller globalen xs:simpleType und xs:complexType//
//  eines XML-Schemas												                            		 //
///////////////////////////////////////////////////////////////////////////////
class GmlSchemaTypes
{
	public:
		GmlSchemaTypes ();
		virtual ~GmlSchemaTypes();

	public:
		bool addType ( GmlSchemaType * pTypeP );
		GmlSchemaType * getType ( std::string typeNameP );

		void setTargetNamespace ( std::string textP )  {  targetNamespace = textP;  }

		size_t getSimpleTypes ( std::vector<GmlSchemaType*> & simpleTypesP );
		size_t getComplexTypes ( std::vector<GmlSchemaComplexType*> & complexTypesP );

    void resolveReferences( GmlSchemaElements * pSchemaElements, GmlSchemaGroups *pSchemaGroups );

	private:
		std::string							 targetNamespace;  // Target-Namespace des Schemas
		std::map<std::string,GmlSchemaType*> types; // map Type Name --> GmlSchemaType Objekt
};

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaElements                                                        //
//  Modelliert die Collection aller globalen xs:element eines XML-Schemas		 //
///////////////////////////////////////////////////////////////////////////////
class GmlSchemaElements
{
	public:
		GmlSchemaElements ();
		virtual ~GmlSchemaElements();

	public:
		bool addElement ( GmlSchemaElement * pElementP );
		GmlSchemaElement * getElement ( std::string elementNameP );

		size_t getElements ( std::vector<GmlSchemaElement*> & schemaElementsP );
    void resolveReferences();

    void addSubstitutionGroup ( GmlSchemaElement * pElementP );
		size_t getSubstitutionGroup ( std::string substitutionGroupNameP, std::vector<GmlSchemaElement*> &elements );

	private:
		std::map<std::string, GmlSchemaElement*> elements;   // map: element Name --> GmlSchemaElement objekt
    std::multimap<std::string,GmlSchemaElement*> substitutionElements; // multiMap: subsitutionGroup name -->  GmlSchemaElement objekt 
};

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaGroups                                                          //
//  Modelliert die Collection aller xs:group Elemente                    		 //
//  Wird in GML Anwendungsschemata normalerweise nicht verwendet             //
///////////////////////////////////////////////////////////////////////////////
class GmlSchemaGroups
{
public:
  GmlSchemaGroups ();
  virtual ~GmlSchemaGroups();

private:
  std::map<std::string, GmlSchemaGroup*> groups;  //  map: group Name --> GmlSchemaGroup Objekt 

public:
  void addGroup ( GmlSchemaGroup * pGroupP );
  GmlSchemaGroup * getGroup ( std::string groupNameP );

  void resolveReferences( GmlSchemaElements * pSchemaElements );
};

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaType                                                            //
//  Abstrakte Oberklasse von xs:simpleType und xs:complexType	          		 //
///////////////////////////////////////////////////////////////////////////////
class GmlSchemaType
{
	public:
		GmlSchemaType ( std::string nameP );
    GmlSchemaType ( GmlSchemaType * pTypeOld );
		virtual ~ GmlSchemaType();

	public:
    virtual bool isSimpleType() = NULL;

		std::string getName() {  return typeName;  }

		void setComment ( std::string commentP )  {  comment = commentP;  }
		std::string getComment ()  {  return comment;  }

    void setIsComplex ( bool b )  {  complex = b;  }
    bool getIsComplex()  {  return complex;  }

	private:
		std::string typeName;  //  Name des Types
		std::string comment;   // Kommentar innerhalb der Typdefinitien

  protected:
    bool        complex;   //  simple oder complex
};

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaGroup                                                           //
//  Modelliert eine xs:group (kommt in GML-Schemata normalerweise nicht vor) //
///////////////////////////////////////////////////////////////////////////////
class GmlSchemaGroup
{
public:
  enum GROUP_TYPE  {  SEQUENCE, CHOICE, ALL  };

public:
  GmlSchemaGroup();
  GmlSchemaGroup ( GmlSchemaGroup * pGroupOld );
  virtual ~ GmlSchemaGroup();

private:
  std::string groupName;   //  Name der Gruppe (xs:name)
  std::string comment;     //  Kommentar (xs:documentation) innerhalb der Gruppe
  GROUP_TYPE groupType;    //  Aggregations-Typ

  std::vector<GmlSchemaElement*> elements;


public: 
  void setName ( std::string nameP )  {  groupName = nameP;  }
  std::string getName() {  return groupName;  }

  void setComment ( std::string commentP )  {  comment = commentP;  }
  std::string getComment ()  {  return comment;  }

  void addSchemaElement ( GmlSchemaElement* pSchemaElement ) {  elements.push_back ( pSchemaElement );  }
	size_t getSchemaElements (	std::vector<GmlSchemaElement*> &elementsP );

  bool resolveGroupReferences ( GmlSchemaGroups * pSchemaGroups );

  void setGroupType ( GROUP_TYPE groupTypeP  )  {  groupType = groupTypeP;  }
  int getGroupType ()  {  return groupType;  }
};

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaComplexType                                                     //
//  Modelliert globale komplexe Datentypen (xs:complexType)  des XML-Schemas //
///////////////////////////////////////////////////////////////////////////////
class GmlSchemaComplexType: public GmlSchemaType
{
	public:
		GmlSchemaComplexType (  std::string nameP );
    GmlSchemaComplexType (  GmlSchemaComplexType * pTypeOld );
		virtual ~GmlSchemaComplexType();

	public:
    virtual bool isSimpleType()  {  return false;  }

    bool getIsAbstract()  {  return isAbstract;  }
    void setIsAbstract ( bool b )  {  isAbstract = b;  }

		void setParant ( std::string parantTypeP );
    void setComplex ( bool c )  {  complex = c;  }

		void addElement ( GmlSchemaElement* pElementP );
    void insertElements ( size_t index, std::vector<GmlSchemaElement*> vElements );
    void addAttribut ( GmlSchemaAttribut * pAttributP );

		GmlSchemaComplexType * getParantType ( GmlSchemaTypes * pTypes );
    std::string getParantTypeName ()  {  return parantTypeName;  }
    std::string getParantTypeNamespace ()  {  return parantTypeNamespace;  }

		GmlSchemaElement * getChildElement ( GmlSchemaTypes * pTypesP, std::string childElementNameP );
		size_t getChildElements ( std::vector<GmlSchemaElement*> & childElementsP );
    size_t getChildElementAnz()  {  return elements.size();  }
    void setChildElements ( std::vector<GmlSchemaElement*> childElementsP );
    GmlSchemaElement * getChildElement ( int index ); 
    void clearChildElements();

    GmlSchemaAttribut * getAttribut ( GmlSchemaTypes * pTypesP, std::string attribueNameP );
		size_t getAttributes ( std::vector<GmlSchemaAttribut*> & attributesP );

    std::string getNamespace ()  {  return typeNamespace;  }

    void setIsReference ( bool b )  {  isReference = b;  }
    bool getIsReference ()   {  return isReference;  }


	private:
		std::string typeNamespace;                     // Namespace des complexType

		std::string parantTypeName;                    // Name der parent-Types
		std::string parantTypeNamespace;               // Namespace des parnet-Type

		std::vector<GmlSchemaElement*>  elements;     // Vektir der Elemente (xs:element) des Types
    std::vector<GmlSchemaAttribut*> attributes;   // Attribute (xs:Attribute) des Types
    bool                            isReference;  // true, wenn der complexType gml:ReferenceType ist
    bool                            isAbstract;   // true bei einem abstrakten complexType
};

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaSimpleType                                                      //
//  Modelliert einfache Datentypen (xs:simpleType) des XML-Schemas     			 //
//  Im Augenblick: Nur Enumerationen von Strings					              		 //
///////////////////////////////////////////////////////////////////////////////
class GmlSchemaSimpleType: public GmlSchemaType
{
	public:
		GmlSchemaSimpleType  (  std::string nameP );
		virtual ~GmlSchemaSimpleType();

	public:
    virtual bool isSimpleType()  {  return true;  }

		void addEnumerationItem ( std::string itemP );

		size_t getEnumerationItems ( std::vector<std::string> & enumerationItemsP );

	private:	
		std::vector<std::string> enumerationItems;  // Vector der Enumerations-Items
};


//////////////////////////////////////////////////////////////////////////////
//  GmlSchemaElement                                                        //
//  Modelliert globale und lokale Elemente (xs:element) des XML-Schemas     //
//////////////////////////////////////////////////////////////////////////////
class GmlSchemaElement
{
	public:
		GmlSchemaElement ();
    GmlSchemaElement ( GmlSchemaElement * pElementOld);
		virtual ~GmlSchemaElement();

	public:	
    void setName ( std::string name )  {  elementName = name;  }
		std::string getName()  {  return elementName;  }

    void setRefName ( std::string name )  {  refName = name;  }
    std::string getRefName()  {  return refName;  }

    virtual bool isGroupRef()  {  return false;  }

    void setSubstitutionGroup ( std::string name )  {  substitutionGroup = name;  }
    std::string getSubstitutionGroup()  {  return substitutionGroup;  }

		void setType ( std::string typeP );

    void setElementTypeName ( std::string name )  {  elementTypeName = name;  }
		std::string getElementTypeName ()  {  return elementTypeName;  }

    void setElementTypeNamespace ( std::string name )  {  elementTypeNamespace = name;  }
		std::string getElementTypeNamespace ()  {  return elementTypeNamespace;  }


		void setDefaultValue ( std::string defaultP )  {  defaultValue = defaultP;  }
		std::string getDefaultValue()  {  return defaultValue;  }

		void setOptionalElement ( bool b )  { optionalElement = b;  }
		bool getOptionalElement ()			{  return optionalElement;  }

		void setMultiElement ( bool b )  { multiElement = b;  }
		bool getMultiElement ()			 {  return multiElement;  }

		void setAbstractElement ( bool b )  {  abstractElement = b;  }
		bool getAbstractElement ()  {  return abstractElement;  }

		void setUnnamedType ( GmlSchemaComplexType * pUnnamedTypeP )					{ pUnnamedType = pUnnamedTypeP;  }
    GmlSchemaComplexType * getUnnamedType()  {  return pUnnamedType;  }

		GmlSchemaType * getElementTypeObject ( GmlSchemaTypes * pTypesP );
    bool isComplexType ( GmlSchemaTypes * pTypesP );
		GmlSchemaElement * getChildElement ( GmlSchemaTypes * pTypesP, std::string childElementNameP );

		void setExternalCodeListName ( std::string nameP )  {  externalCodeListName = nameP;  }
		std::string getExternalCodeListName ()  {  return externalCodeListName;  }

		void setReferenziertesElement ( std::string nameP )  {  referenziertesElement = nameP;  }
		std::string getReferenziertesElement()  {  return referenziertesElement;  }

    void setNillable ( bool b )  {  nillable = b;  }
    bool getNillable ()  {  return nillable;  }

	private:
		std::string elementName;             // Name des Elements (xs:name)
    std::string refName;                 // Referenzname (xs:ref) in einem lokalen Element
    std::string substitutionGroup;       // xs:substitutionGroup

		std::string elementTypeName;         // Typ-Name des Elements (xs:type)
		std::string elementTypeNamespace;    // Namespace des Types
		std::string referenziertesElement;   // Bei einem lokalen Element: Name des zugehörigen globalen Elements
		
		std::string defaultValue;            // Standardwert des Elements
		bool		    optionalElement;         // Gibt bei einem lokalen Element an, ob die Verwendung optional ist
		bool		    multiElement;            // Gibt bei einem lokalen Element an, ob es mehrfach benutzt werden kann
		bool		    abstractElement;         // Gibt an, ob das Element abstrakt ist
		std::string externalCodeListName;    // Name der Externen Codeliste, wenn das Element einen entsprechenden Typ hat
    bool        nillable; 

		GmlSchemaComplexType * pUnnamedType; // das r zum Element gehörige GmlSchemaComplexType-Objekt
};

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaGroupRef                                                        //
//  Modelliert eine Referenz auf eine xs:group	  				                   //
///////////////////////////////////////////////////////////////////////////////
class GmlSchemaGroupRef: public GmlSchemaElement
{
public:
  GmlSchemaGroupRef ();
  GmlSchemaGroupRef ( GmlSchemaGroupRef * pRefOld );
  virtual ~GmlSchemaGroupRef();

public:
  virtual bool isGroupRef()  {  return true;  }
};

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaAttribut                                                        //
//  Modelliert ein Attribut (xs:attribute) oder eines Attribut-Gruppe        //
//  eines xs:complexType                                                     //
///////////////////////////////////////////////////////////////////////////////
class GmlSchemaAttribut
{
public:
  GmlSchemaAttribut();
  GmlSchemaAttribut( GmlSchemaAttribut * pAttributP );
  virtual ~GmlSchemaAttribut();

public:
  void setAttributName ( std::string name )  {  attributName = name;  }
  std::string getAttributName()  {  return attributName;  }

  void setRefName ( std::string name )  {  refName = name;  }
  std::string getRefName()  {  return refName;  }

  void setAttributType ( std::string typeP );

  void setAttributTypeName ( std::string name )  {  attributTypName = name;  }
  std::string getAttributTypeName ()  {  return attributTypName;  }

  void setAttributTypeNamespace ( std::string name )  {  attributTypNamespace = name;  }
  std::string getAttributTypeNamespace ()  {  return attributTypNamespace;  }

  void setDefaultValue ( std::string defaultP )  {  defaultValue = defaultP;  }
  std::string getDefaultValue()  {  return defaultValue;  }

  void setOptionalElement ( bool b )  { isOptional = b;  }
  bool getOptionalElement ()			{  return isOptional;  }

  void setIsGroup ( bool b )  { isGroup = b;  }
  bool getIsGroup ()			{  return isGroup;  }

  void write (  System::IO::StreamWriter ^ writer );

private:
  std::string attributName;      // Name des Attributs (xs:name)
  std::string refName;           // Referenz (xs:ref) auf ein global definiertes Attribut

  std::string attributTypName;       // Attribut-Typ (xs:type)
  std::string attributTypNamespace;  // Namespace des Atribut-Typs

  std::string defaultValue;          // Defaultwert (xs:default)
  bool        isGroup;               // Gibt an, ob s sich um eine Attribut-Gruppe handelt
  bool        isOptional;            // Gibt an, ob die Benutzung des Attributs optional ist

};

///////////////////////////////////////////////////////////////////////////////
//  GmlSchema                                                                //
//  Modelliert eine ein oder mehrere GML-Schemata    	                       //
///////////////////////////////////////////////////////////////////////////////
class GmlSchema
{
	public:
		GmlSchema( Features * pFeatures );
		virtual ~GmlSchema();

	public:
		void addGmlKlasse ( GmlKlasse* pGmlKlasseP );
    void addGmlElement ( GmlElement * pGmlElementP );
		void addGmlEnumeration ( GmlEnumeration * pGmlEnumeration );

		size_t getGmlKlassen ( std::vector<GmlKlasse*> &gmlKlassen );
		size_t getGmlPunktObjekte ( std::vector<GmlKlasse*> &gmlPunktObjekte, int planTyp, bool all );
		size_t getGmlLinienObjekte ( std::vector<GmlKlasse*> &gmlLinienObjekte, int planTyp, bool all );
		size_t getGmlFlaechenObjekte ( std::vector<GmlKlasse*> &gmlFlaechenObjekte, int planTyp, bool all );

		GmlKlasse * getGmlKlasse ( std::string gmlKlassenNameP );
    GmlKlasse * getGmlKlasseFromElementName ( std::string gmlElementName );
    GmlElement     * getGmlElement ( std::string gmlElementNameP );
		GmlEnumeration * getGmlEnumeration ( std::string gmlEnumerationNameP );

		GmlAttribut * getGmlAttribut ( std::string gmlKlassenNameP, std::string gmlAttributNameP );
		std::string getGmlEnumerationsName ( std::string gmlKlassenNameP, std::string gmlAttributNameP );

    GML_SCHEMA_TYPE  getGmlTyp () {  return gmlTyp;  }
    std::string      getGmlTypAsString ();

    GML_VERSION        getGmlVersion();
    static GML_VERSION getGmlVersion( GML_SCHEMA_TYPE gmlTyp );

    static std::string gmlAttributTypToString( ATTRIBUT_TYP typ );
    static ATTRIBUT_TYP gmlStringToAttributTyp ( std::string typString );

    void adaptRechtscharakterGmlKlassen ( XPLAN_TYP planTyp );

    GmlNamespaces * getGmlNamespaces ()  {  return pGmlNamespaces;  }

    GmlNamespaces * getGmlNamespacesStandard ()  {  return pGmlNamespacesStandard;  }

     void generateEnumerationFromExternalCodeList ( GmlDictionaryEntry * pEntry );

    void generateEnumerationsAndCodeLists();
    GmlEnumeration * generateGmlEnumeration ( GmlDictionaryEntry * pDictionaryEntry, std::string codeSpace );

    GmlDictionary * getCodeListsDictionary ();

    bool isCityGml();
    bool isXPlanGml();
    bool isAlkis();
    bool isInspirePlu();
    bool isBoreholeML();
    bool isInspire();

    std::string getDefaultCodeListsFolder();

    TypeDefinitions * getTypeDefinitions()  {  return pTypeDefinitions;  }

    SchemaTypeNames * getSchemaTypeNames ()  {  return pSchemaTypeNamesStandard;  }

    bool readGmlSchema ( GML_SCHEMA_TYPE gmlTyp, GmlNamespaces * pGmlNamespacesAct );
    bool readGmlSchemaFromString ( std::string gmlTyp, GmlNamespaces * pGmlNamespacesAct );

    static std::string getSchemaFolder ( std::string systemFolder, GML_SCHEMA_TYPE gmlTyp );
    std::string getSchemaFolder();


     std::string getExternalCodeListsFolder();

    std::string createExternalCodeListsPath ( std::string externalCodeListName );
    std::string createDefaultExternalCodeListsPath ( std::string externalCodeListName );
    std::string createCityGML20CodeListPath ( GmlKlasse * pGmlKlasseP, GmlAttribut * pGmlAttributP );

    bool externalCodeListDictionariesExist();

	private:
    Features                            * m_pFeatures;        // Aggregation der GML-Features
	  GML_SCHEMA_TYPE                       gmlTyp;             // Typ des GML-Schemas
	  std::map<std::string,GmlKlasse*>      gmlKlassen;         //  map der GML GmlKlasse-Objekte
    std::map<std::string,GmlElement*>     gmlElements;        // map der GmlElement-Objekte
	  std::map<std::string,GmlEnumeration*> gmlEnumerationen;   // map der GML-Enumerationen
    GmlNamespaces                       * pGmlNamespaces;          // Aktuelle Namespaces der eingelesenen Schemadateien
    GmlNamespaces                       * pGmlNamespacesStandard;  //  Standard-Namespaces der GML-Schemadateien
    GmlDictionary                       * pCodeLists;              // Dictionary der externen Codelisten
    TypeDefinitions                     * pTypeDefinitions;        // Typ-Definitionen
    SchemaTypeNames                     * pSchemaTypeNamesStandard;  // SchemaTypeNames

	private:
		bool classOccursInPlan ( int planTyp, GmlKlasse * pGmlKlasse );
    bool analyzeGmlSchema ( GmlSchemaTypes * pGmlSchemaTypesP, 
      GmlSchemaElements * pGmlSchemaElements, SchemaTypeNames * pSchemaTypeNames );
    void addAttribute ( GmlKlasse * pGmlKlasseP, GmlSchemaComplexType * pSchemaComplexType,
      GmlSchemaTypes * pGmlSchemaTypesP, SchemaTypeNames * pSchemaTypeNames, GmlSchemaElements * pGmlSchemaElements );
    void addEnumerationItems ( GmlEnumeration * pGmlEnumerationP, GmlSchemaSimpleType * pSimpleTypeP );

};

///////////////////////////////////////////////////////////////////////////////
//  GmlKlasse                                                                //
//  Modelliert einen GML FeatureType oder DataType, abgeleitet aus den       //
//  globalen xs:complexType Elementen der Schema-Dateien                     //
///////////////////////////////////////////////////////////////////////////////
class GmlKlasse
{
	public:
		GmlKlasse ( std::string klassenNameP );
		virtual ~GmlKlasse();

	public:
		std::string getGmlKlassenName()  {  return gmlKlassenName;  }
		std::string getGmlTypName();
    std::string getGmlElementName();

		void addParentTypeName( std::string name )  {  parentTypeNames.push_back ( name );  }
		size_t getParentTypeNames ( std::vector<std::string> &parentTypeNamesP );
		bool isParentTypeName ( std::string typeName );

		void setGmlObjektTyp ( OBJEKT_TYP typP )  {  gmlObjektTyp = typP;  }
		OBJEKT_TYP getGmlObjektTyp ()  {  return gmlObjektTyp;  }

		void setPlanTyp ( XPLAN_TYP typP )  {  planTyp = typP;  }
		XPLAN_TYP getPlanTyp ()  {  return planTyp;  }

		void setAbstract ( bool b )  {  abstract = b;  }
		bool getAbstract ()  {  return abstract;  }

		void addGmlAttribut ( GmlAttribut * pGmlAttributP );
    void addGmlSchemaAttribut ( GmlSchemaAttribut * pGmlSchemaAttributP );

		size_t getGmlAttribute ( std::vector<GmlAttribut*> & gmlAttributeP );
		GmlAttribut * getGmlAttribut ( std::string gmlAttributName );

		size_t getGmlSchemaAttribute ( std::vector<GmlSchemaAttribut*> & gmlSchemaAttributeP );
    GmlSchemaAttribut * getGmlSchemaAttribut ( std::string gmlSchemaAttributName );

    void setIsGmlFeature ( bool b )  { isGmlFeature = b; }
    bool getIsGmlFeature()  {  return isGmlFeature;  }

	private:
		std::string gmlKlassenName;   // Typ-Name der GML-Klasse
		OBJEKT_TYP  gmlObjektTyp;     // Klassifikation des Typs
		XPLAN_TYP   planTyp;          // Nur XPlanGML: Klassifikation des Plan-Typs
		bool		    abstract;         // Gibt an, ob es sich um eine abstrakte Klasse handelt
    bool        isGmlFeature;     //  Gibt an, ob ein GML-Feature abgebildet wird

		std::vector<std::string>           parentTypeNames;  //  Vektor der Typ-Namen der Oberklassen
		std::vector<GmlAttribut*>          vGmlAttribute;    //  Vektor aller GML-Attribute, abgeleitet aus den internen xs:element Tags
		std::map<std::string,GmlAttribut*> gmlAttribute;     //  map der GML-Attribute

    std::vector<GmlSchemaAttribut*> vGmlSchemaAttribute;  // Vektor der Schema-Attribute, bgeleitet aus den internen xs:attribute Tags
};

///////////////////////////////////////////////////////////////////////////////
//  GmlElement                                                               //
//  Modelliert ein GML-Element, abgeleitet aus den globalen xs:element       //
//  Elementen der Schema-Dateien                                             //
///////////////////////////////////////////////////////////////////////////////
class GmlElement
{
public:
  GmlElement ( std::string elementNameP );
  virtual ~ GmlElement();

public:
  std::string getGmlElementName()  {  return gmlElementName;  }

  void setGmlTypeName ( std::string nameP )  {  gmlTypeName = nameP;  }
  std::string getGmlTypeName ()  {  return gmlTypeName;  }

  void setGmlTypeNamespace ( std::string nameP )  {  gmlTypeNamespace = nameP;  }
  std::string getGmlTypeNamespace ()  {  return gmlTypeNamespace;  }

  void setAbstract ( bool b )  {  _abstract = b;  }
  bool getAbstract ()  {  return _abstract;  }

  void write ( System::IO::StreamWriter ^ writer );

private:
  std::string gmlElementName;   //  Element-Name
  std::string gmlTypeName;      // Typ-Name des Elements
  std::string gmlTypeNamespace; // Typ-Namespace des Elements

  bool _abstract;               // Gibt an, ob es sich um ein abstraktes Element handelt
};

///////////////////////////////////////////////////////////////////////////////
//  GmlAttribut                                                              //
//  Modelliert ein Attribut einer GML Feature-Klasse		            				 //
///////////////////////////////////////////////////////////////////////////////
class GmlAttribut
{
	public:
		GmlAttribut ( std::string attributNameP );
		virtual ~GmlAttribut();

	public:
		std::string getGmlAttributName()  {  return gmlAttributName;  }

		void setGmlAttributTyp( ATTRIBUT_TYP attributTypP )  {  gmlAttributTyp = attributTypP;  }
		ATTRIBUT_TYP getGmlAttributTyp()  {  return  gmlAttributTyp;  }

    void        setGmlAttributTypName ( std::string nameP )  {  gmlAttributTypName = nameP;  }
    std::string getGmlAttributTypName()  {  return gmlAttributTypName;  }

		void setOptional ( bool b )  {  optional = b;  }
		bool getOptional ()  {  return optional;  }

		void setMultiple ( bool b )  {  multiple = b;  }
		bool getMultiple ()  {  return multiple;  }

		void setDefaultWert ( std::string defaultWertP )  {  defaultWert = defaultWertP;  }
		std::string getDefaultWert ()  {  return defaultWert;  }

		void setGmlEnumeration ( std::string nameP )  {  gmlEnumeration = nameP;  }
		std::string getGmlEnumeration()  {  return gmlEnumeration;  }

    void setReferenziertesElement ( std::string nameP )  {  referenziertesElement = nameP;  }
    std::string getReferenziertesElement()  {  return referenziertesElement;  }

    void setNillable ( bool b )  {  nilable = b;  }
    bool getNillable ()  {  return nilable;  }

	private:
		std::string  gmlAttributName;           //  Name des GML-Attributs
		ATTRIBUT_TYP gmlAttributTyp;            // Klassifikation des Attribut-Typs
    std::string  gmlAttributTypName;        // Bezeichnung des Attribut-Typs
    std::string  referenziertesElement;     // Name des referenzierten Elements bei Verweisen
		bool		     optional;                  // Gibt an, ob es sich um ein optionales Attribut handelt
		bool         multiple;                  // Gibt an, ob es sich um ein mehrfach belegbares Attribut handelt
    bool         nilable;                   // Gibt an, ob das Attribut "nillable" ist
		std::string  defaultWert;               // Standardwert des Attributs
		std::string gmlEnumeration;             // Name der zug. Enumeration bei Enumerations-Attributen
};

///////////////////////////////////////////////////////////////////////////////
//  GmlEnumeration                                                           //
//  Modelliert eine GML-Enumeration oder Codeliste             	 						 //
///////////////////////////////////////////////////////////////////////////////
class GmlEnumeration
{
	public:
		GmlEnumeration ( std::string enumerationNameP, bool isExternalCodeListP );
		virtual ~ GmlEnumeration();

	public:
		std::string getGmlEnumerationName()  {  return gmlEnumerationName;  }

		void addGmlEnumerationItem( GmlEnumerationItem * pItemP );

		size_t getEnumerationItems ( std::vector<GmlEnumerationItem*> &gmlEnumerationItemsP );
		size_t getEnumerationItemNames ( std::vector<std::string> &gmlEnumerationItemNamesP );

		void setCodeListDictionary ( std::string name )  {  codeListDictionary = name;  }
		std::string getCodeListDictionary()  { return codeListDictionary;  }

		GmlEnumerationItem * getEnumerationItem ( std::string itemNameP );

		bool getIsExternalCodeList()  {  return isExternalCodeList;  }

		void clear();

	private:
		std::string gmlEnumerationName;    // Name der Enumeration / Codeliste
		bool        isExternalCodeList;    // Gibt an, ob es sich um eine Codeliste handelt
		std::string codeListDictionary;    // Zugehöriges Codelist-Dictionary

		std::map<std::string,GmlEnumerationItem*> gmlEnumerationItems;  // map der Enumeration / Codelist einträge
};

///////////////////////////////////////////////////////////////////////////////
//  GmlEnumerationItem                                                       //
//  Modelliert einen Eintrag einer Enumeration  oder einer  Codeliste        //
///////////////////////////////////////////////////////////////////////////////
class GmlEnumerationItem
{
	public:
		GmlEnumerationItem ( std::string enumerationItemName );
		virtual ~GmlEnumerationItem();

	public:
		std::string getGmlEnumerationItemName()  {  return gmlEnumerationItemName;  }

	private:
		std::string gmlEnumerationItemName;  // Eintrag
};

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaReader								                                					 //
//  Liest die XML-Schema-Dateien eines GML-Applikationsschemas ein           //
//  und analysiert Elemente und Typen	         		                           //
///////////////////////////////////////////////////////////////////////////////
ref class GmlSchemaReader
{
	public:
		GmlSchemaReader ( GmlSchemaTypes    * pGmlSchemaTypesP, 
						          GmlSchemaElements * pGmlSchemaElementsP,
                      GmlSchemaGroups   * pGmlSchemaGroupsP,
                      GmlNamespaces     * pGmlNamespacesStandardP );

		virtual ~GmlSchemaReader();

	public:
		bool read( std::string fileName );

	private:
		GmlSchemaTypes    * pGmlSchemaTypes;        // Aggregation der GmlSchemaType Objekte
		GmlSchemaElements * pGmlSchemaElements;     // Aggregation der GmlSchemaElement Objekte
    GmlSchemaGroups   * pGmlSchemaGroups;       // Aggregation der GmlSchemaGroup Objekts 
    GmlNamespaces     * pGmlNamespaces;         // Aktuell benutzte Namespaces 
    GmlNamespaces     * pGmlNamespacesStandard; // Standard-Namespaces
    System::String    ^ TARGET_NAMESPACE;       // Target-Namespace

    System::Xml::XmlTextReader ^ reader;   // .NET XML-Reader
	  System::String ^ pActElement;          // Aktuell gelesenen XML-Element
	  System::String ^ pActAttrName;         // Name des aktuell gelesenen XML-Attributs
    int              schemaType;           // Typ des Schemas (GML_SCHEMA_TYPE)

	public:
		void readGmlSchemaElement( GmlSchemaElement * pElementP );

		void readGmlSchemaComplexType( GmlSchemaComplexType * pTypeP );
    void readGmlSchemaGroup( GmlSchemaGroup * pGroupP );
		void readComplexContent (  GmlSchemaComplexType * pTypeP );
    void readSimpleContent (  GmlSchemaComplexType * pTypeP );

    GmlSchemaElement * readElement ();
    void readGmlSchemaAttribut (  GmlSchemaComplexType * pTypeP, bool isGroupP );

		void readGmlSchemaSimpleType( GmlSchemaSimpleType * pTypeP );
    System::String ^ generateStandardTargetName ( System::String ^ name );
    System::String ^ generateStandardName ( System::String ^ name );

    System::String ^ getTargetNamespace()  {  return TARGET_NAMESPACE; }

    void closeReader();
};




