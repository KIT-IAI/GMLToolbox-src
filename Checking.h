#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Windows::Forms;
using namespace System::Text;
using namespace System::Collections;

#include "includes.h"
#include "Geometrie.h"

class ProfileRule;
class ProfileFilter;
class AttributeFilter;
class AttributeCardinalityFilter;
class RelationFilter;
class ErrorReport;
class SemanticError;
class CodeListError;
class AttributeValueList;
class NamesValuesList;
class GroupFilter;
class GeometryFilter;
class BLeitPlan;
class PlanBereich;
class Feature;
class GmlAttribut;
class GmlSchema;
class MultiSurface;
class SemanticRules;
class GeometryError;
class GmlNamespaces;
class GmlModelStatistics;
class CheckingMethods;
class ReadError;
class ValidationError;
class UOMList;
class Logfile;

///////////////////////////////////////////////////////////////////////////////
//  Globale Enumerationen															                       //
///////////////////////////////////////////////////////////////////////////////
enum GEOMETRY_RESTRICTION {  ONLY_POINT, ONLY_LINE, ONLY_AREA, NO_POINT, NO_LINE,  NO_AREA, NO_MULTI_AREA, NO_GEOMETRY_RESTRICTION  };
enum COMBINATION_TYPE  { AND, OR, XOR, XOR_OPT };
enum ATTRIBUTE_VALUE_OPERATOR  { EQ, NOT_EQ, LT, LE, GT, GE };

///////////////////////////////////////////////////////////////////////////////
//  Checking                                                                 //
//  Überprüfung von Instanz-Dokumenten                                       //
///////////////////////////////////////////////////////////////////////////////
class Checking
{
  public:
    enum GEOMETRY_CHECK_LEVEL {  NO_GEOMETRY_CHECK, BASIC_CHECKS, ADVANCED_CHECKS, TOPOLOGY_CHECKS, SPECIFIC_CHECKS, ALL_CHECKS };

  public:
    Checking ( Features * pFeaures, UOMList * pUOMList, SrsManager * pSrsManager, Logfile * pLogfile );
    virtual ~ Checking();

  protected:
    Features           * m_pFeatures;     // Aggregation der zu prüfenden Features
    UOMList            * m_pUOMList;      // Modul Units-of-Measurements
    SrsManager         * m_pSrsManager;   // Modul Koordinaten-Transformationen
    Logfile            * m_pLogfile;      // Modul Logfile-Ausgabe

    SemanticRules      * m_pRules;           // Allgemeine Konformitätsregeln eines GML-APplikationsschemas
    SemanticRules      * m_pProfileRules;    // Zusätzliche Konformitätsregeln (Profil-Regeln)
    ErrorReport        * m_pErrorReport;     // Datenstruktur aller festgestellter Konformitätsfehler
    GmlModelStatistics * m_pModelStatistics; // Datenstrukur Modell-Statistik

    CheckingParams     * m_pCheckParams;   // Parameter der Testroutinen

    double               flaecheEinzelobjekte;   // Nur XPlanGML: Summe der Flächengrößen der Flächenschlussobjekte
    double               flaecheGeltungsbereich; // Nur XPlanGML: Flächengröße des Geltungsbereichs
    double               flaecheUeberlappung;    // Nur XPlanGML: Summe der Flächengrößen von Überlappungsbereichen

  public:
    bool fullStatistics;          // true - Ausführliche Modellstatistik; false - Übersicht

    bool checkSemanticRules;      // Überprüfung der Konformitätsregeln
    bool checkCodeLists;          // Überprüfung der Attributwerte, die durch externe Codelisten definiert sind
    bool checkUnitsOfMeasurement; // Überprüfung der Units-of-Measurements
    bool checkReferenzen;         // Überprüfung von Referenzen zwischen Features
    bool checkCoordReferenceSystems; // Überprüfung Vorhandensein Koordinaten-Referenzsystem

    bool checkXPlanGMLRules;           // Nur XPlanGML: Durchführung der spezifischen XPlanGML-Checks
    bool checkXPlanGMLBackwardPointer; // Nur XPlanGML: Überprüfung Existenz von Rückwärts-Referenzen Planobjekt --> Planbereich
    bool checkXPlanGMLUOM;             // Nur XPlanGML: Überprüfung der Zulässigkeit des Koordinaten-Referenzsystems

    GEOMETRY_CHECK_LEVEL geometryCheckLevel;  // Typ / level der Geometrieprüfungen

    bool checkFlaechenschlussExakt;      // Nur XPlanGML: Flächenschlussprüfung mit Berechnung von Überlappungsflächen
    bool checkFlaechenschlussGrob;       // Nur XPlanGML: Topologieprüfung von Flächenschlussobjekten
    bool checkVollstaendigeUeberdeckung; // Nur XPlanGML: Summe der Flächengrößen der Flächenschlussobjekte =
                                         // Flächengröße des Geltungsbereiches
    
    bool checkBoundarySurfaces;          // Nur CityGML: Überprüfung der Topologie von Boundary Surfaces
    bool checkOuterShell;                // Nur CityGML: Überprüfung der Gebäude-Außenhülle

  public: 
    Features * getFeatures()  {  return m_pFeatures;  }

    void initialize();
    void readProfileRules ( std::string profileName );

    CheckingParams * getCheckingParams ()  {  return m_pCheckParams;  }

		ErrorReport   * getErrorReport ()  {  return m_pErrorReport;  }
    SemanticRules * getSemanticRules()  {  return m_pRules;  }

    void resetValidation();
    void resetSemanticErrors();
    void resetGeometryErrors();
    void resetReadErrors();

    void GenerateModelStatistics( std::string outputFile = "" );
    GmlModelStatistics * getModellStatistik ()  {  return m_pModelStatistics;  }

    int getProfiles ( std::vector<std::string> &vProfiles );
    void ValidateModelAgainstXMLSchema( std::string fileName, std::string profileName );
    void ValidateModelAgainstSchematronRules( std::string fileName, std::string profileName );

    void CheckSemanticRules( std::string profileName );
    void CheckCodeLists();
    void CheckUnitsOfMeasurement();
    void CheckReferenzen();
    void CheckXPlanGMLRules();
    void CheckXPlanGMLBackwardPointer();
    void CheckCoordReferenceSystems();

    void CheckGeometry( System::Windows::Forms::ProgressBar ^ testProgress );
    void checkXPLanGMLGeometry(  System::Windows::Forms::ProgressBar ^ testProgress );
    void CheckCityGMLGeometry(  System::Windows::Forms::ProgressBar ^ testProgress );

    void setGeometryCheckLevel( int level ) {  geometryCheckLevel = (GEOMETRY_CHECK_LEVEL)level;  }
    GEOMETRY_CHECK_LEVEL getGeometryCheckLevel()  {  return geometryCheckLevel;  }

  private:
    void CheckEnumValues ( Feature * pObj, std::vector<GmlAttribut*> vEnumAttribute );
    void CheckBereichRelation ( PlanBereich * pPlanBereich, std::string relationName, std::string inverseRelationName );
    void CheckReferenzen ( Feature * pFeature );

    void analyzeSchematromErrors ( String ^ schematronErrorPath );

    void checkCityGMLBoundarySurfaces ( System::Windows::Forms::ProgressBar ^testProgress );
    void checkCityGMLOuterShell ( System::Windows::Forms::ProgressBar ^testProgress );
};

///////////////////////////////////////////////////////////////////////////////
//  CheckingParams                                                           //
//  Parameter zur Konfigurierung der geometrischen Prüfung                   //
///////////////////////////////////////////////////////////////////////////////
class CheckingParams
{
public:
  CheckingParams();
  virtual ~ CheckingParams();

public:
  double minimalPointDistance;    // Haben 2 Punkte einen kleineren Abstand, werden sie als identisch betrachtet
  double planarityLimit;          // Maximal zulässiger Abstand der Stützpunkte einer ebenen Fläche von der Flächenebene
  double minimalAngleDistance;    // Haben zwei Polygons / Ringe eine kleinere Winkeldifferenz (in rad), werden sie als parallel betrachtet
  bool   eliminateDoublePoints;   // Automatische Entfernung doppelter Stützpunkte in Linien- und Flächenkonturen während der Ürüfung
  bool   checkReferencedGeometry; // Automatische Prüfung einer referenzierten Geometrie

  bool   checkBoundarySurfaces;   // Nur Energy ADE: Prüfung der geometrischen Korrektheit einzelner Boundary Surfaces
  bool   checkSolid;              // Nur Energy ADE: Prüfung das Vereinigung der Boundary Surface Geometrie die Solid-Geometrie ergibt
  bool   checkUeberlappung;       // Nur Energy ADE: Prüfung das Boundary Surfaces sich nicht gegenseitig überlappen
};

///////////////////////////////////////////////////////////////////////////////
//  SemanticRules                                                            //
//  Sammlung aller Regeln, mit denen ein geladenes Modell überprüft          //
//  werden kann.												                              			 //
///////////////////////////////////////////////////////////////////////////////
class SemanticRules
{
	public:
		SemanticRules ( Features * pFeatures, CheckingParams * pCheckParams,  ErrorReport * pErrorReportP );
		virtual ~ SemanticRules();

	protected:
		Features  						                * m_pFeatures;      // Zu prüfendes Modell
    CheckingParams                        * m_pCheckParams;   // Prüfparameter
    ErrorReport                           * m_pErrorReport;   // Fehler-Report
		std::string							              	profilName;       // Name eines Profils des Schemas
		std::set<std::string>					          srsNamen;         // Menge der zulässigen Kennungen von Koordinaten-Referenzsystemen
    std::set<std::string>					          uomLength;        // Zulässige uom für eine Länge
    std::set<std::string>					          uomArea;          // Zulässige uom für eine Flächengröße
    std::set<std::string>					          uomVolume;        // Zulässige uom für eine Vo0lumenangabe
    std::set<std::string>					          uomAngle;         // Zulässige uom für eine Winkelangabe
		std::multimap<std::string,ProfileRule*> profileRulesClasses;      // Zuordnung von Klassennamen zu semantischen Regeln
		std::multimap<std::string,ProfileRule*> profileRulesSuperclasses; // Zuordnung von Basisklassen-Name zu semantischen Regeln

public:
    Features * getFeatures()  {  return m_pFeatures;  }
    CheckingParams * getCheckingParams ()  {  return m_pCheckParams;  }

    void initialize();

		void addError ( SemanticError * pError );
		void addSrsName( std::string srsName );
    void addUomLength ( std::string name );
    void addUomArea ( std::string name );
    void addUomVolume ( std::string name );
    void addUomAngle ( std::string name );
		void addProfileRule( ProfileRule *pRule ); 

    void checkRules ( Feature * pFeature );
    void checkUnitsOfMeasurement ( Feature * pFeature );

    void uomPruefung ( BLeitPlan * pPlan, bool allePlaenePruefen );

		void setProfilName ( std::string nameP )  {  profilName = nameP;  }
		std::string getProfilName ()  {  return profilName;  }

	protected:
		size_t  getRules ( std::string klassenName, std::vector<ProfileRule*> & vRules );
};

///////////////////////////////////////////////////////////////////////////////
//  ProfileRule                                                              //
//  Einzelne Konformitätsregel  	                                  				 //
///////////////////////////////////////////////////////////////////////////////
class ProfileRule
{
	public:
		enum FILTER_PART  {  IF_FILTER, REQUIRED_FILTER  };

	public:
		ProfileRule();
		virtual ~ ProfileRule();

	protected:
		std::string          gmlClassName;          // Name der zug. GML-Klasse
		std::string          gmlSuperclassTypeName; // Name der zugehörigen GML-Basisklasse
    std::string          ruleId;                // Kennung der Regel
    std::string          description;           // Informelle Beschreibung der Regel

		ProfileFilter * pIfFilter;       // Regel wird nur aktiv, wenn die Instanz der GML-Klasse die Filterbedingungen erfüllt
		ProfileFilter * pRequiredFilter; // Zu erfüllende Bedingungen, wenn die Regel aktiv ist

	public:
		void setGmlClassName ( std::string name )  {  gmlClassName = name;  }
		std::string getGmlClassName()  {  return gmlClassName;   }

		void setGmlSuperclassTypeName ( std::string name )  {  gmlSuperclassTypeName = name;  }
		std::string getGmlSuperclassTypeName()  {  return gmlSuperclassTypeName;   }

    void setRuleId ( std::string name )  {  ruleId = name;  }
    std::string getRuleId()  {  return ruleId;   }

    void setDescription ( std::string desc )  {  description = desc;  }
    std::string getDescription()  {  return description;  }

		void addAttributeFilter ( AttributeFilter * pFilter, FILTER_PART part, String ^ combination );
    void addAttributeCardinalityFilter ( AttributeCardinalityFilter * pFilter, FILTER_PART part, String ^ combination );
		void addRelationFilter  ( RelationFilter * pFilter, FILTER_PART part, String ^ combination );
		void addGroupFilter     ( GroupFilter * pFilter, FILTER_PART part, String ^ combination );
		void addGeometryFilter  ( GeometryFilter * pFilter, FILTER_PART part, String ^ combination );

		ProfileFilter * getIfFilter ()  {  return pIfFilter;  }
		ProfileFilter * getRequiredFilter ()  {  return pRequiredFilter;  }

		bool checkRule ( Feature * pObj, SemanticRules * pProfile );

  protected:
    static COMBINATION_TYPE interpretCombination ( String ^ combination );
};

///////////////////////////////////////////////////////////////////////////////
//  ProfileFilter                                                            //
//  Kombination von Bedingungen an Attributwerte, Relationen und Geometrie-  //
//  Eigenschaften, die ein bestimmtes Objekt erfüllen muss                 	 //
///////////////////////////////////////////////////////////////////////////////
class ProfileFilter
{
	public:
		ProfileFilter( ProfileRule * pParentRuleP );
		virtual ~ ProfileFilter();

	protected:
		ProfileRule                            * pParentRule;         // Zugehöriges Objekt ProfileRule
		std::vector<AttributeFilter*>            attributeFilters;    // Zu erfüllende Attribut-Filter
    std::vector<AttributeCardinalityFilter*> attributeCardinalityFilters; // Zu erfüllende Attribut-Kardinalitäts-Filter
		std::vector<RelationFilter*>             relationFilters;     // Zu erfüllende Relations-Filter
		std::vector<GroupFilter*>                groupFilters;        // Zu erfüllende Gruppen-Filter
		std::vector<GeometryFilter*>             geometryFilters;     // Zu erfüllende Geometrieeigenschafts-Filter
    COMBINATION_TYPE	                       combinationType;     // Kombination aller Einzelbedingungen (UND bzw. ODER Verknüpfung)

	public:
		ProfileRule * getParentRule ()  {  return pParentRule;  }

		void addAttributeFilter ( AttributeFilter * pFilter );  
    void addAttributeCardinalityFilter ( AttributeCardinalityFilter * pFilter );
		void addRelationFilter  ( RelationFilter * pFilter );  
		void addGroupFilter     ( GroupFilter * pFilter );  
		void addGeometryFilter  ( GeometryFilter * pFilter );

		size_t getAttributeFilters ( std::vector<AttributeFilter*> & attributeFilters );
		size_t getAttributeCardinalityFilters ( std::vector<AttributeCardinalityFilter*> &vattributeCardinalityFilters );
		size_t getRelationFilters ( std::vector<RelationFilter*> & relationFilters );
		size_t getGroupFilters ( std::vector<GroupFilter*> & groupFilters );
		size_t getGeometryFilters ( std::vector<GeometryFilter*> & geometryFilters );
    
    void setCombinationType ( COMBINATION_TYPE comb ) {  combinationType = comb;  }
    int getCombinationType ()  {  return combinationType;  }


		bool checkRule ( Feature * pObj, SemanticRules * pProfile, bool generateError );
};

///////////////////////////////////////////////////////////////////////////////
//  AttributeFilter                                                          //
//  Bedingungen für den Wertebereich eines einzelnen Attributes	      			 //
///////////////////////////////////////////////////////////////////////////////
class AttributeFilter
{
	public:
    enum LOCATION  {  SELF, PLAN_BEREICH, PLAN  };  // Position des zu zu überprüfenden Attributs

	public:
		AttributeFilter(  ProfileRule * pParentRuleP );
		virtual ~ AttributeFilter();

	protected:
		ProfileRule			                    * pParentRule;              // Zug. Objekt ProfileRule
		std::vector<std::string>              attributeValues;          // Spezifizierte textuelle Attributwerte
		std::vector<ATTRIBUTE_VALUE_OPERATOR> attributeValueOperators;  // Vergleichsoperator für jeden einzelnen textuellen Wert
    std::vector<double>                   numericValues;            // Spezifizierte numerische Attributwerte
    std::vector<ATTRIBUTE_VALUE_OPERATOR> numericValueOperators;    // Vergleichsoperator für jeden einzelnen numerischen Wert
		std::string				                    attributeName;            // Name des zu prüfenden Attributs
		unsigned int				                  minOccurs;                // Minimale Anzahl der vorhandenen Attributwerte
		unsigned int				                  maxOccurs;                // Maximale Anzahl der vorhandenen Attributwerte
    LOCATION                              location;                 // Position des Attributs

	public:
		void setAttributeName ( std::string name )  {  attributeName = name;  }
		std::string getAttributeName ()  {  return attributeName;  }

		void addAttributeValue ( std::string value, ATTRIBUTE_VALUE_OPERATOR op ) 
					{ attributeValues.push_back ( value );
					  attributeValueOperators.push_back ( op );
					}

    void addNumericValue ( double value, ATTRIBUTE_VALUE_OPERATOR op ) 
         { numericValues.push_back ( value );
           numericValueOperators.push_back ( op );
         }

		std::string getAttributeValue ( size_t index );
		int getAttributeValueOperator ( size_t index );
		size_t getAttributeValueAnz();

    double getNumericValue ( size_t index );
    int getNumericValueOperator ( size_t index );
		size_t getNumericValueAnz();

		void setMinOccurs ( int n )  {  minOccurs = n;  }
		int  getMinOccurs ()         {  return minOccurs;  }

		void setMaxOccurs ( int n )  {  maxOccurs = n;  }
		int  getMaxOccurs ()         {  return maxOccurs;  }

    void setLocation ( std::string loc );
    LOCATION getLocation ()  {  return location;  }

		SemanticError * checkRule ( Feature * pObj, SemanticRules * pProfile );

    static bool checkNumericRestriction ( double attributWert, double restriction, ATTRIBUTE_VALUE_OPERATOR op );
};

///////////////////////////////////////////////////////////////////////////////
//  AttributeCardinalityFilter                                               //
//  Bedingungen für die Häufigkeit, mit der zwei unterschiedliche Attribute  //
//  belegt sind                                                              //
///////////////////////////////////////////////////////////////////////////////
class AttributeCardinalityFilter
{
  enum COMPARE_OPERATOR {  less, lessOrEqual, equal, greaterOrEqual, greater  };
  public:
    AttributeCardinalityFilter( ProfileRule * pParentRuleP );
    virtual ~ AttributeCardinalityFilter() ;

  protected:
    ProfileRule    * pParentRule;     // Zugehöriges Objekt ProfileRule
    std::string      attributNameA;   // Name des ersten Attributs
    std::string      attributNameB;   // Name des zweiten Attributs
    COMPARE_OPERATOR compare;         // Vergleichsoperator der Häufigkeiten

  public:
    void setAttributnameA ( std::string str )  {  attributNameA = str;  }
    std::string getAttributNameA ()  {  return attributNameA;  }

    void setAttributnameB ( std::string str )  {  attributNameB = str;  }
    std::string getAttributNameB ()  {  return attributNameB;  }

    void setCompareOperator ( COMPARE_OPERATOR c )  {  compare = c;  }
    void setCompareOperator ( std::string op );
    COMPARE_OPERATOR getCompareOperator ()  {  return compare;  }

    SemanticError * checkRule ( Feature * pObj, SemanticRules * pProfile );		
};

///////////////////////////////////////////////////////////////////////////////
//  RelationFilter                                                           //
//  Bedingungen für den Wertebereich einer Relation		            					 //
///////////////////////////////////////////////////////////////////////////////
class RelationFilter
{
	public:
		RelationFilter( ProfileRule * pParentRuleP );
		virtual ~ RelationFilter();

	protected:
		ProfileRule                         * pParentRule;                        // Zug. Objekt ProfileRule
		std::string                           relationName;                       // Name der zu prüfenden Relation
    std::string                           inverseRelationName;                // Name der inversen Relation
    std::string                           attributNameInRelatedObject;        // Attributname im verbundenen Objekt
    std::string                           requiredAttributWertInRelatedObject;// Notwendigen Attributwert im verbundenen Objekt
    std::string                           relationNameInRelatedObject;        // Relations-Name im verbundenen Objekt
    std::string                           geometryPropertyNameInRelatedObject;// Geometrie-Eigenschaft im verbundenen Objekt
		std::vector<std::string>              gmlType;                            // Typ-Name des verbundenen Objekts
    std::vector<std::string>              gmlSuperType;                       // Basisklassen-Typ-Name des verbundenen Objekts
	  std::vector<std::string>              attributWertInRelatedObject;        // Spezifizierte textuelle Werte des Attributs im verbundenen Objekts
    std::vector<ATTRIBUTE_VALUE_OPERATOR> attributWertInRelatedObjectOperators;// Vergleichsoperatoren für die textuellen Attributwerte
    std::vector<double>                   numericWertInRelatedObject;         // Spezifizierte numerische Werte des Attributs im verbundenen Objekts
    std::vector<ATTRIBUTE_VALUE_OPERATOR> numericWertInRelatedObjectOperators;// Vergleichsoperatoren für die numerische Attributwerte

	  unsigned int	           minOccurs;         // Minimale Anzahl der verbundenen Objekte
		unsigned int             maxOccurs;         // Maximale Anzahl der verbundenen Objekte
    unsigned int	           minOccursProperty; // Minimale Häufigkeit der Attributwerte im verbundenen Objekt
    unsigned int             maxOccursProperty; // Maximale Häufigkeit der Attributwerte im verbundenen Objekt

	public:	
		void setRelationName ( std::string name )  {  relationName = name;  }
		std::string getRelationName()  {  return relationName;  }

    void setInverseRelationName ( std::string name )  {  inverseRelationName = name;  }
    std::string getInverseRelationName()  {  return inverseRelationName;  }

    void addGmlTypeName ( std::string name )  {  gmlType.push_back( name );  }
    std::string getGmlTypeName ( unsigned int index );
		size_t getGmlTypeAnz ()  {  return gmlType.size();  }

    void addGmlSuperTypeName ( std::string name )  {  gmlSuperType.push_back( name );  }
    std::string getGmlSuperTypeName ( unsigned int index );
		size_t getGmlSuperTypeAnz ()  {  return gmlSuperType.size();  }

		void setMinOccurs ( int n )  {  minOccurs = n;  }
		int  getMinOccurs ()         {  return minOccurs;  }

		void setMaxOccurs ( int n )  {  maxOccurs = n;  }
		int  getMaxOccurs ()         {  return maxOccurs;  }

    void setMinOccursProperty ( int n )  {  minOccursProperty = n;  }
    int  getMinOccursProperty ()         {  return minOccursProperty;  }

    void setMaxOccursProperty ( int n )  {  maxOccursProperty = n;  }
    int  getMaxOccursProperty ()         {  return maxOccursProperty;  }

    void setAttributNameInRelatedObject ( std::string name )  {  attributNameInRelatedObject = name;  }
    std::string getAttributNameInRelatedObject()  {  return attributNameInRelatedObject;  }

    void setRequiredAttributWertInRelatedObject ( std::string v )  {  requiredAttributWertInRelatedObject = v;  }
    std::string getRequiredAttributWertInRelatedObject()  {  return requiredAttributWertInRelatedObject;  }

    void addAttributWertInRelatedObject ( std::string name, ATTRIBUTE_VALUE_OPERATOR op )
                    {  attributWertInRelatedObject.push_back( name );  
                       attributWertInRelatedObjectOperators.push_back( op );
                    }

    void addNumericWertInRelatedObject ( double wert, ATTRIBUTE_VALUE_OPERATOR op )
                    {  numericWertInRelatedObject.push_back( wert );  
                       numericWertInRelatedObjectOperators.push_back( op );
                    }

    std::string getAttributWertInRelatedObject( unsigned int index );
    int getAttributWertInRelatedObjectOperator( unsigned int index );
		size_t getAttributWertInRelatedObjectAnz()  {  return attributWertInRelatedObject.size(); }

    double getNumericWertInRelatedObject( unsigned int index );
    int getNumericWertInRelatedObjectOperator ( unsigned int index );
		size_t getNumericWertInRelatedObjectAnz()  {  return numericWertInRelatedObject.size(); }

    void setRelationNameInRelatedObject ( std::string name )  {  relationNameInRelatedObject = name;  }
    std::string getRelationNameInRelatedObject()  {  return relationNameInRelatedObject;  }

    void setGeometryPropertyNameInRelatedObject ( std::string name )  {  geometryPropertyNameInRelatedObject = name;  }
    std::string getGeometryPropertyNameInRelatedObject()  {  return geometryPropertyNameInRelatedObject;  }

		SemanticError * checkRule ( Feature * pObj, SemanticRules * pProfile );		
};

///////////////////////////////////////////////////////////////////////////////
//  GroupFilter                                                              //
//  Spezifikation von Gruppen von Attributen, Relationen und Geometrie-      //
//  Eigenschaften, die entweder alle belegt sein müssen (AND Verknüpfung),   //
//  von denen mindestens eines belegt sein muss (OR Verknüpfung), oder genau //
//  eines belegt sein muss (XOR Verknüpfung)                                 //
///////////////////////////////////////////////////////////////////////////////
class GroupFilter
{
	public:
		GroupFilter( ProfileRule * pParentRuleP );
		virtual ~ GroupFilter();

	protected:    
		ProfileRule			       * pParentRule;           // Zugehöriges Objekt ProfileRule
		COMBINATION_TYPE	    	 combination;           // Verknüpfungs-Operator
		std::vector<std::string> attributeNames;        // Liste der Attribut-Namen
		std::vector<std::string> relationNames;         // Liste der Relations-Namen
		std::vector<std::string> geometryPropertyNames; // Liste der Namen von Geometrie-Eigenschaften

	public:
		void addAttributeName ( std::string name );
		size_t getAttributeNames ( std::vector<std::string> & vAttributeNames );

		void addRelationName ( std::string name );
		size_t getRelationNames ( std::vector<std::string> & vRelationNames );

		void addGeometryPropertyName ( std::string name );
		size_t getGeometryPropertyNames ( std::vector<std::string> & vGeometryPropertyNames );

		void setCombination ( COMBINATION_TYPE c )  {  combination = c;  }
		COMBINATION_TYPE getCombination ()  {  return combination;  }


		SemanticError * checkRule ( Feature * pObj, SemanticRules * pProfile );		
};

///////////////////////////////////////////////////////////////////////////////
//  GeometryFilter                                                           //
//  Bedingungen an referierte Geometrie							                  			 //
///////////////////////////////////////////////////////////////////////////////
class GeometryFilter
{
	public:
		GeometryFilter ( ProfileRule * pParentRuleP );
		virtual ~ GeometryFilter();

	protected:
		ProfileRule	           * pParentRule;           // Zugehöriges Objekt ProfileRule
		std::string	             geometryPropertyName;  // Name der zu prüfenden Geometrie-Eigenschaft
    unsigned int             minOccurs;             // Minimale Anzahl der referierten Geometrieobjekte
    unsigned int             maxOccurs;             // Maximale Anzahl der zu prüfenden Geometrieobjekte
		GEOMETRY_RESTRICTION     geometryRestriction;   // Restriktion des referierten Geometrie-Typs
    SURFACE_NORMAL_TYPE      surfaceNormal;         // Bei Flächen: Richtung der Flächen-Normale

	public:
		void setGeometryPropertyName ( std::string name )  {  geometryPropertyName = name;  }
		std::string getGeometryPropertyName ()  {  return geometryPropertyName;  }

		void setGeometryRestriction ( std::string rest );
		void setGeometryRestriction ( GEOMETRY_RESTRICTION rest )  {  geometryRestriction = rest;  }
		GEOMETRY_RESTRICTION getGeometryRestriction ()  {  return geometryRestriction;  }

    void setSurfaceNormal ( std::string ori );
    void setSurfaceNormal( SURFACE_NORMAL_TYPE ori )  {  surfaceNormal = ori;  }
    SURFACE_NORMAL_TYPE getSurfaceNormal()  {  return surfaceNormal;  }

    void setMinOccurs ( int n )  {  minOccurs = n;  }
    int  getMinOccurs ()         {  return minOccurs;  }

    void setMaxOccurs ( int n )  {  maxOccurs = n;  }
    int  getMaxOccurs ()         {  return maxOccurs;  }

		SemanticError * checkRule ( Feature * pObj, SemanticRules * pProfile );		
};

///////////////////////////////////////////////////////////////////////////////
//  AbstractReport                                                           //
//  Basisklasse für Fehlerreports			          		                				 //
///////////////////////////////////////////////////////////////////////////////
class AbstractReport
{
  public:
    AbstractReport( Checking * pCheckingP );
    virtual ~AbstractReport();

  protected:
    Checking  * m_pChecking;      // Modul "Prüfung von GML-Modellen
    std::string testDate;         // Datum des Tests

  public:
    virtual std::string getReportType() = NULL;

    void setTestDate ( std::string dateP )  {  testDate = dateP;  }
};

///////////////////////////////////////////////////////////////////////////////
//  ErrorReport                                                              //
//  Konformitätsfehler-Report                				                				 //
///////////////////////////////////////////////////////////////////////////////
class ErrorReport: public AbstractReport
{
	public:
		ErrorReport( Checking * pCheckingP );
		virtual ~ ErrorReport();

	protected:
		std::vector<SemanticError*>   semanticErrors;     // Liste aller Semantik-Fehler
    std::vector<ReadError*>       readErrors;         // Liste aller beim Einlesen der GML-Datei(en) aufgetretenen Fehler
		std::vector<ValidationError*> validationErrors;   // Lists aller Schema-Validierungs-Fehler

    std::set<int>                          geometryErrorTypes;   // Menge aller Typen von Geometriefehlern (enum GeometryError::GeometryErrorType) 
    std::set<std::string>                  parentFeatureTypes;   // Menge aller Basis-Features im Szenenbaum, in denen Geometriefehler auftreten
    std::multimap<int,GeometryError*>      geometryErrors;       // Zuordnung Geometriefehler-Typen --> Geometriefehler-Objekt
    std::map<int,GeometryError*>           geometryErrorMap;     // Zuordnung Fehler-Nummer --> Geometriefehler-Objekt
    std::multimap<std::string,Feature*>    parentMap;            // Zuordnung Klassennamen Basis-Feature --> Feature Objekte
    std::multimap<Feature*,GeometryError*> parentErrorMap;       // Zuordnung Basis-Feature Objekte --> Geometriefehler-Objekte

	public:
    virtual std::string getReportType() {  return "Error Report"; }

    void resetSemanticErrors();
    void resetGeometryErrors();
    void resetReadErrors();
    void resetValidationErrors();

		void addSemanticError ( SemanticError * pError );
    void addGeometryError ( GeometryError * pError );
		void addValidationError ( ValidationError * pError );
    void addReadError ( ReadError * pError );
    void addError ( Error * pError );

		SemanticError * getSemanticError ( size_t index );
		size_t getSemanticErrorAnz();
		size_t getSemanticErrors ( std::string objId, std::vector<SemanticError*> & vConformityErrors );

    std::set<int> & getGeometryErrorTypes ();
		size_t getGeometryErrors ( std::vector<GeometryError*> &vGeometryErrors );
		size_t getGeometryErrors ( int errorType, std::vector<GeometryError*> &vGeometryErrors );
    GeometryError * getGeometryError ( int errorNumber );
		size_t getGeometryErrorAnz ( int errorType );
		size_t getGeometryErrorAnz ();

    std::set<std::string> & getGeometryErrorParentFeatureClasses ();
		size_t getGeometryErrorParentFeatures ( std::string featureClassName, std::set<Feature*> & sFeatures );
		size_t getGeometryErrors ( Feature * pParentFeature, std::vector<GeometryError*> &vGeometryErrors );

		ValidationError * getValidationError ( size_t index );
		size_t getValidationErrorAnz ();

    ReadError * getReadError ( size_t index );
		size_t getReadErrorAnz();
};

///////////////////////////////////////////////////////////////////////////////
//  Error                                                                    //
//  Abstrakte Oberklasse für alle Fehler-Typen                               //
///////////////////////////////////////////////////////////////////////////////
class Error
{
public:
  enum ERROR_TYPE {  VALIDATION_ERROR, GEOMETRY_ERROR, SEMANTIC_ERROR, READ_ERROR  };
  enum ERROR_CLASSIFICATION {  OK, INFORMATION, WARNING, ERROR  };

public:
  Error ( ERROR_CLASSIFICATION classification, std::string name );
  Error ( Error * pErrorOld );
  virtual ~Error();

protected:
  ERROR_CLASSIFICATION errorClassification;   // Fehler-Klassifikation
  std::string          errorName;             // Fehler-Bezeichnung

public: 
  virtual ERROR_TYPE getErrorType() = NULL;
  std::string getErrorName()  {  return errorName;  }
  ERROR_CLASSIFICATION getErrorClassification()  {  return errorClassification;  }

  static std::string getErrorClassificationAsString ( ERROR_CLASSIFICATION errorClassification );
};

///////////////////////////////////////////////////////////////////////////////
//  SemanticError                                                            //
//  Beschreibung eines einzelnen Semantik-Fehlers					              		 //
///////////////////////////////////////////////////////////////////////////////
class SemanticError: public Error
{
	public:
		SemanticError( Feature * pObj, ERROR_CLASSIFICATION classification,  std::string errorDescriptionP );
    SemanticError ( SemanticError * pErrorOld );
		virtual ~ SemanticError();

	protected:
		int				        errorNumber;      // Fehler-Nummer
    std::string       errorId;          // Fehler-Bezeichnung
    std::string       errorDescription; // Fehler-Beschreibung
    Feature         * pFeature;         // Feature, in dem der Fehler aufgetreten ist
    Feature         * pRelatedFeature;  // Weiteres am Fehler beteiligtes Feature
		ProfileRule     * pBrokenRule;      // Verletzte Konformitätsregel
		NamesValuesList * pAttributes;      // Liste der beteiligten Feature-Attribute
		std::string       relatedFeatureId; // Id eines über eine Relation verbundenen Features
		std::string       relationName;     // Name der Relation

	public:
    virtual ERROR_TYPE getErrorType()  {  return Error::SEMANTIC_ERROR;  }

		void setErrorNumber ( int num )  {  errorNumber = num;  }
		int getErrorNumber ()  {  return errorNumber;  }

    void setErrorDescription ( std::string errText )  {  errorDescription = errText;  }
    std::string getErrorDescription()  {  return errorDescription;  }

    void setErrorId ( std::string id )  { errorId = id;  }
    std::string getErrorId ()  {  return errorId;  }

    void setFeature ( Feature * pFeatureP )  {  pFeature = pFeatureP;  }
    Feature * getFeature ()   {  return pFeature;  }

		std::string getFeatureId ();

		GEOMETRY_TYPE getFeatureGeometryType ();
    std::string getFeatureGeometryTypeAsString ();

		std::string getFeatureType ();

		void setBrokenRule ( ProfileRule * pRule );
		ProfileRule * getBrokenRule ()  {  return pBrokenRule;  }

		void setAttributes ( Feature * pObj );
    void setAttribute ( std::string attributName, std::string attributWert );

		NamesValuesList * getAttributes ()  {  return pAttributes;  }

    void setRelatedFeature( Feature * pFeatureP )  {  pRelatedFeature = pFeatureP;  }
    Feature * getRelatedFeature()  {  return pRelatedFeature;  }

		void setRelatedFeatureId ( std::string id )  {  relatedFeatureId = id;  }
		std::string getRelatedFeatureId (); 

		std::string getRelatedFeatureType ();

		void setRelationName ( std::string name )  {  relationName = name;  }
		std::string getRelationName ()  {  return relationName;  }

		void setEnumError ( std::string codeListName, std::string attributeName, 	std::string attributeValue );

    virtual bool isAggregatedError()  {  return false;  }
};

///////////////////////////////////////////////////////////////////////////////
//  AggregatedSemanticError                                                  //
//  Beschreibung einer Aggregation von Semantikfehlern                       //
///////////////////////////////////////////////////////////////////////////////
class AggregatedSemanticError: public SemanticError
{
  public:
    AggregatedSemanticError ( Feature * pObj, ERROR_CLASSIFICATION classification,  std::string errorNameP );
    virtual ~ AggregatedSemanticError();

  protected:
    std::vector<SemanticError*> vErrors;  //  Aggregation von Semantikfehlern

  public:
    void addError ( SemanticError * pError )  {  vErrors.push_back( pError );  }
    void generateOverallError();
    virtual bool isAggregatedError()  {  return true;  }
};

///////////////////////////////////////////////////////////////////////////////
//  GeometryError                                                            //
//  Beschreibung eines einzelnen Geometriefehlers                            //
///////////////////////////////////////////////////////////////////////////////
class GeometryError: public Error
{
public:
  enum GEOMETRY_ERROR_TYPE {  NO_GEOMETRY_SPECIFIED, RING_ZERO_AREA, 
                    RING_NOT_ENOUGH_POINTS, RING_NOT_CLOSED, RING_DOUBLE_POINTS, RING_SELF_OVERLAP,
                    POLYGON_VOID_ERROR,                   
                    POLYGON_NON_PLANAR, POLYGON_VOID_PLANARITY_ERROR, 
                    POLYGON_WRONG_ORIENTATION, POLYGON__VOID_ORIENTATION_ERROR,
                    SURFACE_OVERLAP_ERROR, SURFACE_TOTAL_COVERAGE_ERROR,
                    MULTISURFACE_IRREGULAR_POINT_TOUCH, MULTISURFACE_IRREGULAR_LINE_TOUCH_INTERSECT, MULTISURFACE_IRREGULAR_SURFACE_OVERLAP,                   
                    SOLID_EULER_CHECK_FAILED, SOLID_EDGE_CHECK_FAILED_MISSING_COUNTERPART, 
                    SOLID_EDGE_CHECK_FAILED_DOUBLE_EDGE, SOLID_EDGE_CHECK_FAILED_MULTIPLE_EDGE,
                    SOLID_EDGE_CHECK_INFO_BACK_FORTH_EDGE,
                    CITYGML_MISSING_ROOF_SURFACE, CITYGML_MISSING_WALL_SURFACE, CITYGML_MISSING_GROUND_SURFACE,
                    CITYGML_BOUNDARY_SURFACE_OVERLAPP, CITYGML_BOUNDARY_SURFACES_NO_SOLID,
                    XPLANGML_FLAECHENSCHLUSS_OVERLAPP
                    };
public:
  GeometryError( Feature * pObj, ERROR_CLASSIFICATION classification, GEOMETRY_ERROR_TYPE errorTypeP );
  GeometryError ( GeometryError * pErrorOld );
  virtual ~ GeometryError();

protected:
  int				               errorNumber;                 // Fehlernummer
  GEOMETRY_ERROR_TYPE      errorType;                   // Fehler-Typ
  std::string              errorDescription;            // Fehler-Beschreibung
  Feature                * pFeature;                    // Feature, in dem der Fehler aufgetreten ist
  Feature                * pRelatedFeature;             // Weiteres am Fehler beteiligtes Feature
  std::string              geometryId;                  // Id des fehlerhaften Geometrieobjektes
  std::string              relatedGeometryId;           // Id eines weiteren am Fehler beteiligten Geometrieobjektes
  std::string              propertyName;                // Geometrie-Eigenschaft
  std::string              relatedPropertyName;         // Geometrie-Eigenschaft im beteiligten Feature
  double                   flaecheObj;                  // Flächengröße Geometrieobjekt
  double                   flaecheObjRelated;           // Flächengröße beteiligtes Geometrieobjekt
  double                   flaecheUeberlappung;         // Größe der Überlappung beider Geometrieobjekte
  double                   planaritaetsfehlerMaximal;   // Maximaler Planaritätsfehler
  double                   planaritaetsfehlerDurchschnitt; // Mittlerer Planaritätsfehler
	size_t                   index1, index2;              // Indices
  std::vector<_Geometrie*> vErrorGeometry;              // Liste von fehlerfaften Geometrieobjekten
  std::vector<std::string> vErrorGeometryDescription;   // Zugehörige Fehlerbeschreibungen
  bool                     errorGeometryCreated;        // Gibt an, ob die Fehlergeometrie schon erzeugt wurde

public:
  virtual ERROR_TYPE getErrorType()  {  return Error::GEOMETRY_ERROR;  }

  void setErrorNumber ( int num )  {  errorNumber = num;  }
  int getErrorNumber ()  {  return errorNumber;  }

  GEOMETRY_ERROR_TYPE getGeometryErrorType()  {  return errorType; }
  static std::string getGeometryErrorTypeAsString ( int errorType );
  std::string getGeometryErrorTypeAsString();

  void setErrorDescription ( std::string desc )  { errorDescription = desc;  }
  std::string getErrorDescription ()  {  return errorDescription;  }

  void setFeature ( Feature * pFeatureP )  {  pFeature = pFeatureP;  }
  Feature * getFeature()  {  return pFeature;  }

  void setRelatedFeature ( Feature * pFeatureP )  {  pRelatedFeature = pFeatureP;  }
  Feature * getRelatedFeature()  {  return pRelatedFeature;  }

  std::string getFeatureId ();

  void setFeatureGeometryId ( std::string id )  {  geometryId = id;  }
  std::string getFeatureGeometryId ()  {  return geometryId;  }

  void setRelatedFeatureGeometryId ( std::string id )  {  relatedGeometryId = id;  }
  std::string getRelatedFeatureGeometryId ()  {  return relatedGeometryId;  }

  GEOMETRY_TYPE getFeatureGeometryType ();
  std::string getFeatureGeometryTypeAsString ();

  GEOMETRY_TYPE getRelatedFeatureGeometryType();
  std::string getRelatedFeatureGeometryTypeAsString();

  std::string getFeatureType ();

  std::string getRelatedFeatureId (); 

  std::string getRelatedFeatureType ();

  void setPropertyName ( std::string name )  {  propertyName = name;  }
  std::string getPropertyName ()  {  return propertyName;  }

  void setRelatedPropertyName ( std::string name )  {  relatedPropertyName = name;  }
  std::string getRelatedPropertyName ()  {  return relatedPropertyName;  }

  void setFlaechenangaben ( double flaecheObjP,  double flaecheObjRelatedP,  double flaecheUeberlappungP );
  void getFlaechenangaben ( double &flaecheObjP, double &flaecheObjRelatedP, double &flaecheUeberlappungP );

  void setPlanaritaetsfehler ( double distMax,  double distAvarage );
  void getPlanaritaetsfehler ( double &distMax, double &distAvarage );

  void setSelfOverlapIndeces ( size_t index1P, size_t  index2P );
	size_t getSelfOverlapIndex1 ()  {  return index1;  }
	size_t getSelfOverlapIndex2 ()  {  return index2;  }

  void addErrorGeometry ( _Geometrie * pGeo, std::string desc );
  
  _Geometrie * getErrorGeometry ( size_t index ) ;
  std::string  getErrorGeometryDescription ( size_t index );
	size_t getErrorGeometryAnz()  {  return vErrorGeometry.size(); }

  void setErrorGeometryCreated();
  bool getErrorGeometryCreated()  {  return errorGeometryCreated;  }
};


///////////////////////////////////////////////////////////////////////////////
//  NamesValuesList                                                          //
//  Liste von Attribut-Name / Attribut-Wert Paaren			            				 //
///////////////////////////////////////////////////////////////////////////////
class NamesValuesList
{
	public:
		NamesValuesList ();
    NamesValuesList ( NamesValuesList * pListOld );
		virtual ~ NamesValuesList();

	protected:
		std::vector<std::string> names;  // Liste von Attribut-Namen
		std::vector<std::string> values; // Liste von Attribut-Werten

	public:
		void addNameValue ( std::string name, std::string value );

		bool getNameValue ( size_t index, std::string & name, std::string & value );
		size_t getNamesValuesAnz();

    std::string getAllNames();
};

///////////////////////////////////////////////////////////////////////////////
//  ValidationError                                                          // 
//  XML-Schema Validierungsfehler                                            //
///////////////////////////////////////////////////////////////////////////////
class ValidationError: public Error
{
  public: 
    ValidationError( ERROR_CLASSIFICATION classification, std::string errorTextP, int lineNumber );
    virtual ~ValidationError();

  protected:
    std::string errorText;    // Fehlertext
    int         lineNumber;   // Nummer der Zeile der XML-Datei, in der der Fehler aufgetreten ist

  public:
    virtual ERROR_TYPE getErrorType()  {  return Error::VALIDATION_ERROR;  }

    int getLineNumber()  {  return lineNumber;  }
    std::string getErrorText()  {  return errorText;  }
};

///////////////////////////////////////////////////////////////////////////////
//  ReadError                                                                //
//  Beim Einlesen einer XML-Datei aufgetretener Fehler                       //
///////////////////////////////////////////////////////////////////////////////
class ReadError: public Error
{
  public:  
    ReadError( ERROR_CLASSIFICATION classification, std::string errorTextP, int lineNumber );
    virtual ~ReadError();

  protected:
    std::string errorText;    // Fehlertext
    int         lineNumber;   // Nummer der Zeile der XML-Datei, in der der Fehler aufgetreten ist

  public:
    virtual ERROR_TYPE getErrorType()  {  return Error::READ_ERROR;  }

    int getLineNumber()  {  return lineNumber;  }
    std::string getErrorText()  {  return errorText;  }
};
