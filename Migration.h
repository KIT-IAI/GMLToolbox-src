#pragma once
#include "Includes.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Text;
using namespace System::Collections;

class Features;
class MigrationDateiProtokoll;
class MigrationFehler;
class GmlToolbox;

///////////////////////////////////////////////////////////////////////////////
//  class MigrationV4                                                        //
// Transformiert XPlanGML 3.0 in Version 4.0 oder 4.1												 //
///////////////////////////////////////////////////////////////////////////////
class MigrationV4
{
  public:
    MigrationV4 ( Features * pPlanObjekteP, UOMList * pUOMList, MigrationDateiProtokoll * pMigDateiProtP, 
                std::string codeSpaceCodeListsV4, std::string versionXPlanGML );
    virtual ~ MigrationV4();

  protected:
    Features                * m_pPlanObjekte;					// Zu migrierende XPlanGML Features
    UOMList                 * m_pUOMList;							// Modul Units of Measurement
    MigrationDateiProtokoll * m_pMigDateiProt;				// Migrations-Protokoll
    std::string               m_codeSpaceCodeListsV4; // Codelist Codespace von XPlanGML 4.x
    std::string               m_versionXPlanGML;      // Zu erzeugende XPlanGML Version
    int                       m_transObjAnz;					// Anzahl der transformierten Objekte

  public:
    void transformation ();

  protected:
    void korrekturRelationen();
    void transformationPlan ( BLeitPlan * pPlan );
    void transformationPlanBereich ( PlanBereich * pPlan );
    void transformationObjekt ( Feature * pPlanObjekt );
    void transformationBPlanObjekt ( Feature * pPlanObjekt );
    void transformationMassDerBaulichenNutzung ( Feature * pPlanObjekt );
    void transformationFPlanObjekt ( Feature * pPlanObjekt );
    void transformationPraesentationsobjekt ( Feature * pPlanObjekt );
    void transformationRasterdarstellung ( Feature * pPlanObjekt );

    bool istNachrichtlich ( PlanBereich * pPlanBereich, Feature * pPlanObjekt );
};

///////////////////////////////////////////////////////////////////////////////
//  class MigrationV5                                                        //
// Transformiert XPlanGML 4.1 in Version 5.0, 5.0 oder 5.2, sowie von        //
// Version 5.0 nach Version 5.1 oder 5.2                                     //
///////////////////////////////////////////////////////////////////////////////
class MigrationV5
{
public:
  MigrationV5 ( Features * pPlanObjekteP, UOMList * pUOMList,  MigrationDateiProtokoll * pMigDateiProtP, 
         std::string versionXPlanGMLZiel );
  virtual ~ MigrationV5();

protected:
  Features                * m_pPlanObjekte;					// Zu migrierende XPlanGML Features
  UOMList                 * m_pUOMList;							// Modul Units of Measurement
  MigrationDateiProtokoll * m_pMigDateiProt;	      // Migrations-Protokoll
  int                       m_transObjAnz;					// Anzahl der transformierten Features
  std::string               m_versionXPlanGMLZiel;  // XPlanGml-Version der transformierten Features

public:
  void transformation ();

protected:
  void transformation_V5_0 ();

  void transformationPlan ( BLeitPlan * pPlan );
  void transformationPlanBereich ( PlanBereich * pPlan );
  void transformationObjekt ( Feature * pPlanObjekt );
  void transformationBPlanObjekt ( Feature * pPlanObjekt );
  void transformationFPlanObjekt ( Feature * pPlanObjekt );
  void transformationSOPlanObjekt ( Feature * pPlanObjekt );

  void correctBereichRelation ( Feature * pPlanObjekt, std::string rollenName );
  void correctZweckbestimmungen ( Feature * pPlanObjekt  );
  void correctArtDerFestlegung ( Feature * pPlanObjekt );
  bool correctZweckbestimmung ( Feature * pPlanObjekt, std::vector<std::string> &vZweckbestimmungen,  
                                std::vector<std::string> &vDetaillierteZweckbestimmungen,
                                std::vector<std::string> &vCodespaces, std::string suffix );
  void correctMassnahmen ( Feature * pPlanObjekt  );
  void createTextObjekt ( Feature * pPlanObjekt, std::string textKlassenName, std::string schluessel );
  void createBegruendungObjekt ( Feature * pPlanObjekt, std::string schluessel );
  void correctVerEntsorgungCodes ( Feature * pPlanObjekt );
  void checkMasszahlen ( Feature * pPlanObjekt );
  void correctTextAbschnitt (  Feature * pPlanObjekt, std::string textAbschnittKlasse );
  
  void transformRasterBasis ( Feature * pPlanObjekt, Feature * pPlanBereich, Feature * pRasterplanBasis );
};

///////////////////////////////////////////////////////////////////////////////
//  class MigrationProtokoll                                                 //
//  Protokoll der Migration von XPlanGML Dateien		    										 // 
///////////////////////////////////////////////////////////////////////////////
class MigrationProtokoll
{
  public:
    MigrationProtokoll ();
    virtual ~ MigrationProtokoll();

  protected:
    std::vector<MigrationDateiProtokoll*> vDateiProtokolle;  // Liste der Migrations-Protokolle der einzelnen Dateien
    std::string                             datum;					 // Datum der Migration

  public:
    void addDateiProtokoll ( MigrationDateiProtokoll * pMigDateiProt );
    
    MigrationDateiProtokoll * getDateiProtokoll ( unsigned int index );
		size_t getDateiProtokollAnz()  {  return vDateiProtokolle.size();  }

    void setDatum ( std::string datumP )  {  datum = datumP;  }
    std::string getDatum()  {  return datum;  }

		size_t getMigrationFehlerAnz();
		size_t getMigrationFehlerAnz( FEHLER_KLASSIFIKATION fehlerKlassifikation );
		size_t getAnzahlObjekte();
};

///////////////////////////////////////////////////////////////////////////////
//  class MigrationDateiProtokoll                                            //
//  Protokoll der Migration einer einzelnen XPlanGML Datei									 //
///////////////////////////////////////////////////////////////////////////////
class MigrationDateiProtokoll
{
  public:
    MigrationDateiProtokoll ();
    virtual ~ MigrationDateiProtokoll();

  protected:
    int                             anzahlObjekte;							// Anzahl der vorhandenen XPlanGML Features
    int                             anzahlObjekteTransformiert; // Anzahl der migrierten Featutes
    std::string                     dateiName;									// Pfadname der migrierten Datei
    std::vector<MigrationFehler*> vMigrationFehler;							// Liste der aufgetretenen Migrationsfehler

  public:
    void setAnzahlObjekte ( int anz )  { anzahlObjekte = anz; }
    int getAnzahlObjekte ()  {  return anzahlObjekte;  }

    void setAnzahlObjekteTransformiert ( int anz )  { anzahlObjekteTransformiert = anz;  }
    int getAnzahlObjekteTransformiert ()  {  return anzahlObjekteTransformiert;  }

    void addMigrationFehler ( MigrationFehler * pFehler );
    MigrationFehler * getMigrationFehler ( unsigned int index );
		size_t getMigrationFehlerAnz();
		size_t getMigrationFehlerAnz ( FEHLER_KLASSIFIKATION fehlerKlassifikation );

    void setDateiName ( std::string name )  {  dateiName = name;  }
    std::string getDateiName()  {  return dateiName;  }

    MigrationFehler * createMigrationFehler ( std::string gmlId, std::string klassenNameV3, std::string kattributNameV3,
                                                FEHLER_KLASSIFIKATION klassifikation, std::string klassenNameV4, std::string attributNameV4,
                                                std::string fehlerText, std::string aktionText );
};

///////////////////////////////////////////////////////////////////////////////
//  class MigrationFehler                                                    //
//  Beschreibung von Fehlern bei der Migration															 //
///////////////////////////////////////////////////////////////////////////////
class MigrationFehler
{
  public:
    MigrationFehler();
    virtual ~ MigrationFehler();

  public:
    FEHLER_KLASSIFIKATION fehlerKlassifikation;

    std::string objectId;								// Id des zu migrierenden Features
    std::string klassenNameQuelle;			// Klassenname im Ausgangs-Format
    std::string attributNameQuelle;			// Attributname im Ausgangs-Format
    std::string klassenNameZiel;				// Klassenname im Zielformat
    std::string attributNameZiel;				// Attributname im Zielformat
    std::string fehlerBeschreibung;			// Beschreibung des Fehlers
    std::string aktionBeschreibung;     // Beschreibung, was bei der Migration gemacht wurde
};

///////////////////////////////////////////////////////////////////////////////
//  class MigrationProtokollWriter                                           //
//  Export der Migrationsfehler in einem XML-Format													 //
///////////////////////////////////////////////////////////////////////////////
public ref class MigrationProtokollWriter
{
public:
  MigrationProtokollWriter( MigrationProtokoll * pProtP );
  virtual ~ MigrationProtokollWriter();

public:
  int write ( std::string fileNameP, std::string schemaQuelle, std::string schemaZiel );

protected:
  MigrationProtokoll          * pProt;			// Migrations-Protokoll
  System::Xml::XmlTextWriter ^ writer;			// .NET XML-Writer
  int                          anzFehler;		// Anzahl der Migrationsfehler

protected:
  void writeDateiProtokoll ( MigrationDateiProtokoll * pDateiProt );
  void writeFehler ( MigrationFehler * pFehler );
};


