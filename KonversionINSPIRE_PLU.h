#pragma once
using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Text;
using namespace System::Collections;

#include "includes.h"

class GmlToolbox;
class BLeitPlan;
class XPlanGML_ClassTransformation;
class XPlanGML_AttributTransformation;
class INSPIRE_AttributZuordnung;
class INSPIRE_AttributZuweisung;
class TransformationINSPIRE_PLU_Parameter;
class TransformationINSPIREProtokoll;
class MultiSurfaceCoverage;
class TransformationINSPIREProtokollXPlanObjekt;
class TransformationINSPIREProtokollXPlanAttribut;


///////////////////////////////////////////////////////////////////////////////
//  TransformationINSPIRE_PLU_Parameter                                      //
//  Parameter zur Steuerung der Transformation XPlanGML 4.0 -> INSPIRE PLU   //
///////////////////////////////////////////////////////////////////////////////
class TransformationINSPIRE_PLU_Parameter
{
public:
  enum VOID_ATTRIBUT_TYP { STRING, INTEGER, DOUBLE, BOOLEAN, DATE, URL, RELATION };

public:
  TransformationINSPIRE_PLU_Parameter ();
  virtual ~TransformationINSPIRE_PLU_Parameter();

protected:
  std::map<std::string,XPlanGML_ClassTransformation*> mPlanklassen;    // Transformationsvorschriften XPlanGML-Klasse --> SpatialPlan
  std::map<std::string,XPlanGML_ClassTransformation*> mFlaechenschluss;// Transformationsvorschriften XPlanGML-Klasse --> ZoningElement
  std::map<std::string,XPlanGML_ClassTransformation*> mUeberlagerung;  // Transformationsvorschriften XPlanGML-Klasse --> SupplementaryRegulation
  std::map<std::string,std::string>                   mDocumentTypes;  // Map: XPlanGML-Atributname --> INSPIRE Document-Name

public:
  std::map<std::string,VOID_ATTRIBUT_TYP> mVoidAttributeSpatialPlan;			        // map:INSPIRE PLU Attribut-Name --> Attribut Typ für Void-Attribute von SpatialPlan
  std::map<std::string,VOID_ATTRIBUT_TYP> mVoidAttributeZoningElement;						// map:INSPIRE PLU Attribut-Name --> Attribut Typ für Void-Attribute von ZoningElement
  std::map<std::string,VOID_ATTRIBUT_TYP> mVoidAttributeSupplementaryRegulation;  // map:INSPIRE PLU Attribut-Name --> Attribut Typ für Void-Attribute von SupplementaryRegulation
  std::map<std::string,VOID_ATTRIBUT_TYP> mVoidAttributeCitation; // map:INSPIRE PLU Attribut-Name --> Attribut Typ für Void-Attribute von DocumentCitation / LegislationCitation

public:
  std::string identifierNamespace;												// Namespace INSPIRE Identifier
  std::string registryHILUCSValue;												// Registry-URL HILUCS-Codelist
  std::string registryLandUseClassificationValue;					// Registry-URL Nationale Land-Use Codelist
  std::string registrySupplementaryRegulationValue;				// Registry-URL SupplementaryRegulation Codelist
  std::string registryLocalSupplementaryRegulationValue;  // Registry-URL Nationale SupplementaryRegulation Codelist
  std::string registryLevelOfSpatialPlanValue;						// Registry-URL LevelOfSpatialPlan Codelist
  std::string registryProcessStepGeneralValue;						// Registry-URL ProcessStepGeneral Codelist
  std::string registryRegulationNatureValue;						  // Registry-URL RegulationNature Codelist
  std::string registryPlanTypeNameValue;								  // Registry-URL Nationale Codelist PlanTypeName
  std::string registryLegislationLevelValue;							// Registry-URL LegislationLevel

public:
  void addXPlanGML_ClassTransformation ( XPlanGML_ClassTransformation * pTransformation, INSPIRE_OBJEKT_TYP typ );

  XPlanGML_ClassTransformation * getXPlanGML_ClassTransformation ( std::string klassenNameXPlanGML, INSPIRE_OBJEKT_TYP typ );
	size_t getXPlanGML_ClassTransformation ( std::vector<XPlanGML_ClassTransformation*> &vClassTransform, INSPIRE_OBJEKT_TYP typ );

  void addDocumentTypeTranslation ( std::string xplanProperty, std::string inspireDocumentName );
  std::string getInspireDocumentName ( std::string xplanProperty );
};

///////////////////////////////////////////////////////////////////////////////
//  class TransformationINSPIRE_PLU_Basis                                    //
//  Basisklasse für XPlanung-->INSPIRE PLU Transformtionen                   //
///////////////////////////////////////////////////////////////////////////////
class TransformationINSPIRE_PLU_Basis
{
public:
  TransformationINSPIRE_PLU_Basis ( Features * pObjekteXPlanGML, Features * pObjekteINSPIREP, UOMList * pUOMList, 
              TransformationINSPIRE_PLU_Parameter * pRarameterP );
  virtual ~ TransformationINSPIRE_PLU_Basis();

protected:
  GmlSchema                           * m_pGmlObjekteXPlan;			// XML-Schema XPlanGML
  GmlSchema                           * m_pGmlObjekteINSPIRE;		// XML-Schema INSPIRE PLU
  Features                            * m_pObjekteXPlanGML;			// Features XPlanGML
  Features                            * m_pObjekteINSPIRE;			// Features INSPIRE PLU
  UOMList                             * m_pUOMList;							// Modul Units of Measurement
  Feature                             * m_pPlanINSPIRE;					// INSPIRE PLU Feature SpatialPlan
  TransformationINSPIRE_PLU_Parameter * m_pParameter;						// Transformations-Parameter XPlanGML --> INSPIRE PLU
  TransformationINSPIREProtokoll      * m_pProtokoll;						// Transformations-Protokoll

public:
  virtual bool transformationToPLU ( BLeitPlan * pPlanP, std::vector<PlanBereich*> vPlanBereiche, TransformationINSPIREProtokoll * pProtokollP ) = NULL;

  bool generateDictionaries ( Features * pPlanObjekteINSPIRE, TransformationINSPIRE_PLU_Parameter * pParameter );

  void setTransformationsParameter ( TransformationINSPIRE_PLU_Parameter * pParameterP )  {  m_pParameter = pParameterP;  }
  void setGmlObjekteINSPIRE ( GmlSchema * pGmlObjekte )  {  m_pGmlObjekteINSPIRE = pGmlObjekte; }

  Features * getObjekteINSPIRE ()  {  return m_pObjekteINSPIRE;  }

protected:
  void generateINSPIREId ( std::string gmlId, Feature * pObjektINSPIRE );

  std::string convertMimeTypeXPlanToINSPIRE ( std::string mimeTypeXPlan );

  void addVoidAttributes ( Feature * pFeature, std::map<std::string,TransformationINSPIRE_PLU_Parameter::VOID_ATTRIBUT_TYP> &mVoidAttribute );

  Feature * createDocumentDate ( std::string date, std::string dateType );

  bool getAttributwertXPlan ( Feature * pPbjektXPlan, std::string attributNameXPlan, std::string & attributWertXPlan );
  std::string getRegistry ( std::string attributNameINSPIRE );

  ATTRIBUT_TYP getAttributTypINSPIRE ( std::string klassenNameINSPIRE, std::string attributNameINSPIRE, std::string & referenzKlassenName );

  void setAttributINSPIRE ( Feature * pObjektINSPIRE, INSPIRE_AttributZuordnung * pZuordnung, 
    std::string attributWertXPlan, std::string klassenNameInspire, std::string uom );
  void setAttributINSPIRE ( Feature * pObjektINSPIRE, std::string attributNameINSPIRE, std::string attributWertINSPIRE,
    ATTRIBUT_TYP attributTypINSPIRE, std::string uom );

  void generateINSPIRELegislationCitation ( std::string gesetzLevel, std::string gesetzDatum, std::string documentName );    

};


///////////////////////////////////////////////////////////////////////////////
//  class TransformationINSPIRE_PLU                                          //
//  Transformiert einen XPlanGML 4.0 oder 4.1 Datensatz nach INSPIRE PLU     //
///////////////////////////////////////////////////////////////////////////////
class TransformationINSPIRE_PLU: public TransformationINSPIRE_PLU_Basis
{
  public:
    TransformationINSPIRE_PLU ( Features * pObjekteXPlanGML, Features * pObjekteINSPIREP, UOMList * pUOMList,
            TransformationINSPIRE_PLU_Parameter * pRarameterP );
    virtual ~ TransformationINSPIRE_PLU();

  public:
    virtual bool transformationToPLU ( BLeitPlan * pPlanP, std::vector<PlanBereich*> vPlanBereiche, TransformationINSPIREProtokoll * pProtokollP );

  protected:
    virtual bool transformationPlan ( BLeitPlan * pPlanP  );
    virtual bool transformRasterplanBasis ( Feature * pRasterplan );
    virtual bool transformationFlaechenschlussobjekt ( Feature * pPlanObjekt );
    virtual bool transformationUeberlagerungsobjekt ( Feature * pPlanObjekt );

    virtual bool transformationAttributeUndRelationen ( Feature * pPlanObjektXPlan, Feature * pPlanObjektINSPIRE, INSPIRE_OBJEKT_TYP typ, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt );
    virtual bool generateAndCorrectSpecificAttributes ( Feature * pPlanObjektXPlan, Feature * pPlanObjektINSPIRE );

    virtual bool transformTextlicheFestsetzung ( Feature * pTextAbschnitt );
    virtual Feature * transformExterneReferenz ( std::string rollenName, Feature * pTextAbschnitt, Feature * pParent, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt );
    void transformBereichAttribute ( BLeitPlan * pPlanP, PlanBereich * pPlanBereichP,  Feature * pPlanObjektINSPIRE, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt );

};

///////////////////////////////////////////////////////////////////////////////
//  class TransformationINSPIRE_PLU_V5                                       //
//  Transformiert einen XPlanGML 5.0 Datensatz nach INSPIRE PLU              //
//  Aktuell wird nur das XPlanung-Profil Raster-Umring unterstützt           //
///////////////////////////////////////////////////////////////////////////////
class TransformationINSPIRE_PLU_V5: public TransformationINSPIRE_PLU_Basis
{
public:
  TransformationINSPIRE_PLU_V5 ( Features * pObjekteXPlanGML, Features * pObjekteINSPIREP, UOMList * pUOMList,
                TransformationINSPIRE_PLU_Parameter * pRarameterP );
  virtual ~ TransformationINSPIRE_PLU_V5();

public:
  virtual bool transformationToPLU ( BLeitPlan * pPlanP, std::vector<PlanBereich*> vPlanBereiche, TransformationINSPIREProtokoll * pProtokollP );

protected:
  virtual bool transformationPlan ( BLeitPlan * pPlanP  );
  void transformBereichAttribute ( BLeitPlan * pPlanP, PlanBereich * pPlanBereichP,  Feature * pPlanObjektINSPIRE, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt );
  virtual bool transformRasterdarstellung ( Feature * pRasterplan );
  virtual Feature * transformGenerDatumAttribut ( Feature * pDatumAttribut, Feature * pPlanObjektINSPIRE, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt );
  virtual Feature * transformGenerURLAttribut ( Feature * pDatumAttribut, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt );

  virtual bool transformationAttributeUndRelationen ( Feature * pPlanObjektXPlan, Feature * pPlanObjektINSPIRE, INSPIRE_OBJEKT_TYP typ, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt );
  virtual bool generateAndCorrectSpecificAttributes ( Feature * pPlanObjektXPlan, Feature * pPlanObjektINSPIRE );

  virtual bool transformTextlicheFestsetzung ( Feature * pTextAbschnitt );
  virtual Feature * transformExterneReferenz ( std::string rollenName, Feature * pTextAbschnitt, Feature * pParent, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt );
  virtual Feature * transformSpezExterneReferenz ( std::string rollenName, Feature * pTextAbschnitt, Feature * pParent, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt );
};


///////////////////////////////////////////////////////////////////////////////
//  XPlanGML_ClassTransformation                                             //
//  Parameter zur Transformation einer XPlanGML 4.0 Klasse nach INSPIRE PLU  //
///////////////////////////////////////////////////////////////////////////////
class XPlanGML_ClassTransformation
{
  public:
    XPlanGML_ClassTransformation();
    virtual ~XPlanGML_ClassTransformation();

   protected:
    std::string klassenNameXPlanGML;			// Name der XPlanGML Klasse
    bool        kannAbgebildetWerden;			// Gibt an, ob die Klasse auf INSPIRE PLU abgebildet werden kann
    bool        wirdErwartet;						  // Gibt an, ob die Klasse transformiert werden soll

    std::vector<INSPIRE_AttributZuweisung*>                standardAttributZuweisung;
    std::map<std::string,XPlanGML_AttributTransformation*> mAttributTransformation;

  public:
    void setKlassenNameXPlanGML ( std::string klassenName )  {  klassenNameXPlanGML = klassenName; }
    std::string getKlassenNameXPlanGML ()  {  return klassenNameXPlanGML;  }

    void setKannABgebildetWerden ( bool b )  {  kannAbgebildetWerden = b;  }
    bool getKannAbgebildetWerden ()  {  return kannAbgebildetWerden;  }

    void setWirdErwartet ( bool b )  {  wirdErwartet = b;  }
    bool getWirdErwartet ()  {  return wirdErwartet;  }

    void addStandardAttributZuweisung ( INSPIRE_AttributZuweisung *pZuweisung );
		size_t  getStandardAttributZuweisung ( std::vector<INSPIRE_AttributZuweisung*> &vZuweisung );

    void addXPlanGML_AttributTransformation ( XPlanGML_AttributTransformation * pTransform );

		size_t getXPlanGML_AttributTransformation ( std::vector<XPlanGML_AttributTransformation*> &vAttributTransform );
		size_t getXPlanGML_AttributTransformationNotwendig ( std::vector<XPlanGML_AttributTransformation*> &vAttributTransform );
    XPlanGML_AttributTransformation * getXPlanGML_AttributTransformation ( std::string attributNameXPlanGML );
};

///////////////////////////////////////////////////////////////////////////////
//  XPlanGML_AttributTransformation                                          //
//  Abbildung eines XPlanGML-Attributs auf ein oder mehrere INSPIRE PLU      //
//  Attribute                                                                //
///////////////////////////////////////////////////////////////////////////////
class XPlanGML_AttributTransformation
{
  public:
    XPlanGML_AttributTransformation();
    XPlanGML_AttributTransformation( XPlanGML_AttributTransformation * pTrans );
    virtual ~XPlanGML_AttributTransformation();

  protected:
    std::string attributNameXPlanGML;			// Name des XPlanGML-Attributs
    std::string klassenNameINSPIRE;				// Name der INSPIRE PLU Klasse, auf die abgebildet wird
    std::string uom;											// uom-Identifier eines Measure-Attributs
    bool        kannAbgebildetWerden;			// Gibt an, ob das Attribut abgebildet werden kann
    bool        istNotwendig;							// Gibt an, ob es sich um ein notwendides XPlanGML-Attribut handelt

    std::vector<INSPIRE_AttributZuordnung*> vAttributZuordnung;		// Zugeordnete INSPIRE PLU Attribute

  public:
    void setKlassenNameINSPIRE ( std::string name )  {  klassenNameINSPIRE = name;  }
    std::string getKlassenNameINSPIRE()  {  return klassenNameINSPIRE;  }

    void setUom ( std::string name )  {  uom = name;  }
    std::string getUom()  {  return uom;  }


    void setAttributNameXPlanGML ( std::string name )  {  attributNameXPlanGML = name;  }
    std::string getAttributNameXPlanGML ()  {  return attributNameXPlanGML;  }

    void setIstNotwendig ( bool b )  {  istNotwendig = b;  }
    bool getIstNotwendig()  {  return istNotwendig;  }

    void setKannAbgebildetWerden ( bool b )  {  kannAbgebildetWerden = b;  }
    bool getKannAbgebildetWerden ()  {  return kannAbgebildetWerden;  }

    void addINSPIRE_AttributZuordnung ( INSPIRE_AttributZuordnung * pZoordnung );
		size_t getINSPIRE_AttributZuordnung ( std::vector<INSPIRE_AttributZuordnung*> &vZuordnung );
};

///////////////////////////////////////////////////////////////////////////////
//  INSPIRE_AttributZuordnung                                                //
//  Parameter zur Abbildung von XPlanGML Attributwerten auf INSPIRE PLU      //
//  Attributwerte                                                            //
///////////////////////////////////////////////////////////////////////////////
class INSPIRE_AttributZuordnung
{
  public:
    INSPIRE_AttributZuordnung();
    INSPIRE_AttributZuordnung( INSPIRE_AttributZuordnung * pZuordnung );
    virtual ~INSPIRE_AttributZuordnung();

  protected:
    std::string                             attributNameINSPIRE;					// Name des INSPIRE PLU Attributs
    std::string                             referenzAttributNameINSPIRE;	// Name des referierten INSPIRE Attributs
    std::vector<std::string>                vStandardAttributWerteINSPIRE;// Standardwerte des INSPIRE Attributs
    std::vector<INSPIRE_AttributZuweisung*> vAttributZusatzINSPIRE;				// Attribut-Zusatz
    std::multimap<std::string,std::string>  mAttributwertZuordnung;				// multeMap: XPlanGML-Attributwerte --> INSPIRE Attributwerte

  public: 
    void setAttributNameINSPIRE ( std::string name )  {  attributNameINSPIRE = name;  }
    std::string getAttributNameINSPIRE ()  {  return attributNameINSPIRE;  }

    void setReferenzAttributNameINSPIRE ( std::string name )  {  referenzAttributNameINSPIRE = name;  }
    std::string getReferenzAttributNameINSPIRE ()  {  return referenzAttributNameINSPIRE;  }

    void addStandardAttributWertINSPIRE ( std::string wert )  {  vStandardAttributWerteINSPIRE.push_back ( wert );  }
    std::string getStandardAttributWertINSPIRE ( int index )  {  return vStandardAttributWerteINSPIRE[index];  }
		size_t      getStandardAttributWertINSPIREAnz()  {  return vStandardAttributWerteINSPIRE.size(); }

    void addAttributZusatzINSPIRE ( INSPIRE_AttributZuweisung * pZuweisung );
		size_t getAttributZusatzINSPIRE ( std::vector<INSPIRE_AttributZuweisung*> & vZuweisung );

    void addAttributwertZuordnung ( std::string attributwertXplan, std::string attributwertINSPIRE );
		size_t getAttributwerteINSPIRE ( std::string attributwertXPlan, std::vector<std::string> &vAttributWerteInspire );
};

///////////////////////////////////////////////////////////////////////////////
// INSPIRE_AttributZuweisung																								 //
//  Name und Wert eines INSPIRE Attributs                                    //
///////////////////////////////////////////////////////////////////////////////
class INSPIRE_AttributZuweisung
{
  public:
    INSPIRE_AttributZuweisung();
    INSPIRE_AttributZuweisung( INSPIRE_AttributZuweisung * pZuweisung );
    virtual ~INSPIRE_AttributZuweisung();

  public:
    std::string attributName;			// Attribut-Name
    std::string attributWert;			// Attribut-Wert
};

///////////////////////////////////////////////////////////////////////////////
//  XPlanGML_INSPIRE_Transformation_Reader                                   //
//  Einlesen der Transformationsparameter von einem XML-File                 //
///////////////////////////////////////////////////////////////////////////////
public ref class XPlanGML_INSPIRE_Transformation_Reader
{
  public:
    XPlanGML_INSPIRE_Transformation_Reader( TransformationINSPIRE_PLU_Parameter * pTransformP );
    virtual ~ XPlanGML_INSPIRE_Transformation_Reader( );

  protected:
    TransformationINSPIRE_PLU_Parameter * pTransform;	  // Transformations-Parameter

    XmlTextReader ^ reader;					// .NET XML-Reader
    String        ^ pActElement;		// Aktuell eingelesenes XML-Tag


  public:
    bool read( std::string fileName );

  protected:
    bool readGlobaleParameter();
    bool readFlaechenschlussobjektTransformation();
    bool readUeberlagerungsobjektTransformation();
    bool readPlanklassenTransformation();
    bool readOberklassenTransformationFlaechenschluss();
    bool readOberklassenTransformationUeberlagerung();
    bool readOberklassenTransformationPlan();
    bool readMassBaulicheNutzungTransformation();
    bool readVerfahrensangabenTransformation();
    bool readExterneReferenzTransformation();

    bool readFlaechenschlussobjektAttributTransformation ( XPlanGML_ClassTransformation * pClassTransformation );
    bool readUeberlagerungsobjektAttributTransformation ( XPlanGML_ClassTransformation * pClassTransformation );

    XPlanGML_ClassTransformation * readKlassenTransformation( String ^ END_TAG);
    bool readKlassenAttributTransformation ( XPlanGML_ClassTransformation * pClassTransformation );
    bool readKlassenAttributZuordnung ( XPlanGML_AttributTransformation * pAttributTransformation );

};

///////////////////////////////////////////////////////////////////////////////
//  TransformationINSPIREProtokoll																				   //
//  Protokoll der Transformation XPlanGML 4.0 -> INSPIRE PLU                 //
///////////////////////////////////////////////////////////////////////////////
class TransformationINSPIREProtokoll
{
  public:
    enum TRANSFORMATIONS_GRAD {  TRANSFORMIERT, NICHT_TRANSFORMIERT };
    enum TRANSFORMATIONS_HINDERNIS_KLASSE {  XPLANOBJEKT_NICHT_KORREKT, KLASSE_NICHT_ABBILDBAR, KLASSE_NICHT_ERWARTET  };
    enum TRANSFORMATIONS_HINDERNIS_ATTRIBUT {  ATTRIBUT_NICHT_ABBILDBAR, ATTRIBUTWERT_NICHT_ABBILDBAR, ATTRIBUTWERT_NICHT_KORREKT, ATTRIBUTWERT_NICHT_VORHANDEN  };
    enum INSTANZOBJEKT_TYP  {  FLAECHENSCHLUSSOBJEKT, UEBERLAGERUNGSOBJEKT, PLANOBJEKT, SONST_OBJEKT  };

  public:
    TransformationINSPIREProtokoll( GML_SCHEMA_TYPE versionXPlanGML);
    virtual ~TransformationINSPIREProtokoll();

  public:
    std::vector<TransformationINSPIREProtokollXPlanObjekt*> vInstanzProtokolle; // Protokolle der einzelnen XPlanGML-Objekte
  
  public:
    int anzFeaturesTransformiert;
    int anzFeaturesNichtTransformiert;

    std::map<std::string,int> mFlaechenschlussKlassenVollstaendigTransformiert;
    std::map<std::string,int> mFlaechenschlussKlassenNichtErwartet;
    std::map<std::string,int> mFlaechenschlussKlassenNichtTransformiert;

    std::map<std::string,int> mUeberlagerungsKlassenVollstaendigTransformiert;
    std::map<std::string,int> mUeberlagerungsKlassenNichtErwartet;
    std::map<std::string,int> mUeberlagerungsKlassenNichtTransformiert;

    std::map<std::string,int> mSonstigeKlassenVollstaendigTransformiert;
    std::map<std::string,int> mSonstigeKlassenNichtTransformiert;

  public:
    void generateZusammenfassung();
    void writeProtokoll ( std::string protokollDatei );
};

///////////////////////////////////////////////////////////////////////////////
//  TransformationINSPIREProtokollXPlanObjekt															   //
//  Protokoll der Transformation XPlanGML 4.0 -> INSPIRE PLU für ein         //
//  XPlanGML-Objekt																													 //
///////////////////////////////////////////////////////////////////////////////
class TransformationINSPIREProtokollXPlanObjekt
{
  public:
    TransformationINSPIREProtokollXPlanObjekt();
    virtual ~ TransformationINSPIREProtokollXPlanObjekt();

  public:
    std::string                                                      gmlId;										// Id des XPlanGML-Objektes
    std::string                                                      xplanKlassenName;				// Klassen-Name
    TransformationINSPIREProtokoll::INSTANZOBJEKT_TYP                objektTyp;								// Typ des XPlanGML-Objektes
    TransformationINSPIREProtokoll::TRANSFORMATIONS_GRAD             transformationsGrad;			// Angaben zur Vollständigkeit der Transformation
    TransformationINSPIREProtokoll::TRANSFORMATIONS_HINDERNIS_KLASSE transformationsHindernis;// Angaben zu Transformations-Hindernissen
    std::vector<TransformationINSPIREProtokollXPlanAttribut*>        vAttributProtokolle;			// Protokolle der Attribut-Transformationen

  public:
    void writeInstanzProtokoll ( XmlTextWriter ^ writer );

  protected:
    std::string getObjektTyp();
    std::string getTransformationdGrad();
    std::string getTransformationsHindernis();
};

///////////////////////////////////////////////////////////////////////////////
//  TransformationINSPIREProtokollXPlanAttribut														   //
//  Protokoll der Transformation XPlanGML 4.0 -> INSPIRE PLU für ein         //
//  XPlanGML-Attribut																												 //
///////////////////////////////////////////////////////////////////////////////
class TransformationINSPIREProtokollXPlanAttribut
{
  public:
    TransformationINSPIREProtokollXPlanAttribut();
    virtual ~ TransformationINSPIREProtokollXPlanAttribut();

  public:
    std::string                                                        attributName;						 // Attribut-Name
    std::string                                                        attributWert;						 // Attribut-Wert
    TransformationINSPIREProtokoll::TRANSFORMATIONS_HINDERNIS_ATTRIBUT transformationsHindernis; // Transformations-Hindernis

  public:
    void writeProtokoll ( XmlTextWriter ^ writer );
    
  protected:
    std::string getTransformationsHindernis();
};


