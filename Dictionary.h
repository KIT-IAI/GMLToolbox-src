#pragma once

#include "includes.h"

class HierarchicalCodeListEntry;
class GmlDictionaryEntry;
class GmlDictionaryEntryDefinition;

///////////////////////////////////////////////////////////////////////////////
//  GmlDictionary                                                            //
//  Modelliert ein GML-Dictionary                                            //
///////////////////////////////////////////////////////////////////////////////
class GmlDictionary
{
public:
  GmlDictionary();
  virtual ~GmlDictionary( );

public:
  void setId ( std::string idP )  {  id = idP;  }
  std::string getId()  {  return id;  }

  void setIdentifier ( std::string ident )  {  identifier = ident;  }
  std::string getIdentifier()  {  return identifier;  }

  void setIdentifierCodespace ( std::string codespace )  {  identifierCodespace = codespace;  }
  std::string getIdentifierCodespace()  {  return identifierCodespace;  }

  void setName ( std::string nameP )  {  name = nameP;  }
  std::string getName()  {  return name;  }

  void setBeschreibung ( std::string beschreibungP )  {  beschreibung = beschreibungP;  }
  std::string getBeschreibung()  {  return beschreibung;  }

  void addDictionaryEntry ( GmlDictionaryEntry * pEntryP );
  GmlDictionaryEntry * getDictionaryEntry ( std::string idP );
  bool existEntryId ( std::string idP );
  void removeDictionaryEntry (  std::string idP );
  void removeDictionaryEntries();

	size_t getDictionaryEntries ( std::vector<GmlDictionaryEntry*> & vEntriesP );

  GmlDictionaryEntry * getFirstEntry();

private:
  std::string id;                  //  gml:id des Dictionaries
  std::string name;                //  Name des Dictionaries
  std::string beschreibung;        //  Beschreibung des Dictionaries
  std::string identifier;          //  Identifier des Dictionaries
  std::string identifierCodespace; //  Codespace des Identifiers

  std::map<std::string,GmlDictionaryEntry*> mEntries; //  Dictionary Einträge
};

///////////////////////////////////////////////////////////////////////////////
//  GmlDictionaryEntry                                                       //
//  Modelliert einen GML-Dictionary Eintrag                                  //
//  Wahlweise kann ein Eintrag aus einem einzelnen Paar Schlüsselnummer /    //
//  Langtext bestehen, oder eine Liste solcher Paare enthalten               //
///////////////////////////////////////////////////////////////////////////////
class GmlDictionaryEntry
{
public:
  GmlDictionaryEntry();
  virtual ~GmlDictionaryEntry();

public:
  virtual DICTIONARY_ENTRY_TYPE getType()  {  return STANDARD_CODELIST;    }

  void setId ( std::string idP )  {  id = idP;  }
  std::string getId()  {  return id;  }

  void setIdentifier ( std::string ident )  {  identifier = ident;  }
  std::string getIdentifier()  {  return identifier;  }

  void setIdentifierCodespace ( std::string codespace )  {  identifierCodespace = codespace;  }
  std::string getIdentifierCodespace()  {  return identifierCodespace;  }

  void setName ( std::string nameP )  {  name = nameP;  }
  std::string getName()  {  return name;  }

  void setBeschreibung ( std::string beschreibungP )  {  beschreibung = beschreibungP;  }
  std::string getBeschreibung()  {  return beschreibung;  }

  bool existSchluesselNummer( std::string schluesselNummerP );
  bool existSchluesselText ( std::string schluesselTextP );

  virtual bool addDefinition ( std::string schluesselNummerP, GmlDictionaryEntryDefinition dictionaryEntryDefinition );
  virtual bool setSchluesselText ( std::string schluesselNummerP, std::string schluesselText );
  bool removeDefinition ( std::string schluesselNummerP );

  bool getSchluesselNummer  ( std::string schluesselTextP, std::string & schluesselNummerP );
  virtual bool getSchluesselText ( std::string schluesselNummerP, std::string & schluesselText );

	size_t getCodelistEintraege ( std::vector<std::string> &vSchluesselNummernP, std::vector<GmlDictionaryEntryDefinition> &DictionaryEntryDefinitions );
	size_t getCodelistSchluessel ( std::vector<std::string> &vSchluesselNummernP );
	size_t getCodelistEintraegeAnz();

protected:
  std::string id;                  //  gml:id des Eintrags
  std::string name;                // Code bzw. Schlüsselnummer des Eintrags
  std::string beschreibung;        // Langtext zum Code / Schlüsselnummer
  std::string identifier;          //  Identifier des Eintrags
  std::string identifierCodespace; //  Codespace des Identifiers

  std::map<std::string,std::string>                  mSchluesselTexte;            // map Code --> Langtext
  std::map<std::string,GmlDictionaryEntryDefinition> mDictionaryEntryDefinitions; // map Code --> GmlDictionaryEntryDefinition
};

///////////////////////////////////////////////////////////////////////////////
//  GmlDictionaryEntryDefinition                                             //
//  Komplexer Datentyp für einen Eintrag einer Liste von                     //
//  Schlüsselnummer / Langtext Paaren                                        //
///////////////////////////////////////////////////////////////////////////////
class GmlDictionaryEntryDefinition
{
public:
  GmlDictionaryEntryDefinition();
  virtual ~ GmlDictionaryEntryDefinition();

public:
  std::string id;                 //  gml:id des Eintrags
  std::string name;               //  Name des Eintrags
  std::string beschreibung;       //  Beschreibung / Definition des Eintrags
  std::string identifier;         //  Identifier
  std::string identifierCodespace;//  Codespace des Idnetifiers 
};

///////////////////////////////////////////////////////////////////////////////
//  HierarchicalCodeList                                                     //
//  Modelliert eine hierarchische Codeliste für BoreholeML                   //
///////////////////////////////////////////////////////////////////////////////
class HierarchicalCodeList: public GmlDictionaryEntry
{
public:
  HierarchicalCodeList();
  virtual ~HierarchicalCodeList();

private:
  std::map<std::string, HierarchicalCodeListEntry> mKey2CodeListEntries; //  map Schlüssel --> Entries
  std::map<std::string, HierarchicalCodeListEntry> mId2CodeListEntry;    //  map Ids --> Entries

public:
  virtual DICTIONARY_ENTRY_TYPE getType()  {  return HIERARCHICAL_CODELIST;    }

  virtual bool addDefinition ( std::string schluesselNummerP, GmlDictionaryEntryDefinition dictionaryEntryDefinition );
  virtual bool setSchluesselText ( std::string schluesselNummerP, std::string schluesselText );
  bool addCodeListEntry ( HierarchicalCodeListEntry pCodeListEntry );

  virtual bool getSchluesselText ( std::string schluesselNummerP, std::string & schluesselText );

  bool getCodeListEntryFromKey       ( std::string keyId,      HierarchicalCodeListEntry &codelistEntry );
  bool getParentCodeListEntryFromKey ( std::string keyId,      HierarchicalCodeListEntry &codelistEntry );
  bool getCodeListEntryFromId        ( std::string identifier, HierarchicalCodeListEntry &codelistEntry );

	size_t getCodeListEntries ( std::vector<HierarchicalCodeListEntry> &vCodeListEntries );
};

///////////////////////////////////////////////////////////////////////////////
//  HierarchicalCodeListEntry                                                //
//  Modelliert einen Eintrag einer BoreholeML-Codeliste                      //
///////////////////////////////////////////////////////////////////////////////
class HierarchicalCodeListEntry: public GmlDictionaryEntryDefinition
{
public:
  HierarchicalCodeListEntry();
  virtual ~HierarchicalCodeListEntry();

public:
  std::string keyID;                                  // Schlüsel / Id des Eintrags
  std::string parentID;                               //  Id des übergeordneten Eintrags
  int         hierarchyLevel;                         //  Hierarchiestufe
  int         colorCode[3];                           //  Zugeordnete RGB-Farbe
  std::string signaturePath;                          //  ???
  std::map<std::string,std::string> mAlternativeText; // Dem Eintrag zugeordnete Langtexte
};

///////////////////////////////////////////////////////////////////////////////
//  GmlDictionaryReader                                                      //
//  Liest ein GML-Dictionary ein                                             //
///////////////////////////////////////////////////////////////////////////////
public ref class GmlDictionaryReader
{
public:
  GmlDictionaryReader();

  virtual ~GmlDictionaryReader();

public:
  bool readGmlDictionaryCollection ( std::string fileNameP,  GmlDictionary * pDictionary,  bool isCityGML );

  GmlDictionaryEntry * readGmlDefinitionCollection ( bool isCityGML );
  bool                 readDefinition ( std::string &schluesselNummer, GmlDictionaryEntryDefinition &definitionEntry, bool isCityGML );

  bool readGmlSingleDictionary ( std::string fileName,  GmlDictionary * pDictionary,  bool isCityGML );
  GmlDictionaryEntry * readGmlSingleDictionary ( std::string fileName, bool isCityGML );

  HierarchicalCodeList * readBMLCodeList (  std::string fileNameP );

  GmlDictionaryEntry * readINSPIRECodeList ( System::String ^ registryURL, bool useFullCode );
  GmlDictionaryEntry * readINSPIRECodeList (  std::string fileNameP, bool useFullCode );
  GmlDictionaryEntry * readINSPIRECodeList (   System::IO::Stream ^stream, bool useFullCode );

  void skipElement ( System::String ^ pActElement );


  void closeReader();

private:
  System::Xml::XmlTextReader  ^ reader;   // .NET XML-Reader
 // GML_SCHEMA_TYPE  gmlTyp;                // Typ des Applikationsschemas
  DICTIONARY_TYP   dictionaryTyp;         //  Typ des Dictionaries
  System::String ^ pActElement;           //  Aktuell gelesenes XML-Element
  System::String ^ pActAttrName;          //  Aktuell gelesenes Attribut

  bool readHierarchicalCodeListEntry( HierarchicalCodeListEntry & codelistEntry );
  bool readINSPIRECodeListEntry(  GmlDictionaryEntry * codelistEntry, bool useFullCode );
  void readAlternativeExpression ( HierarchicalCodeListEntry & pEntry );

  GmlDictionaryEntry * readINSPIRECodeList (  System::Xml::XmlTextReader ^ pReader, bool useFullCode );

};

///////////////////////////////////////////////////////////////////////////////
//  GmlDictionaryWriter                                                      //
//  Schreiben eines Dictionaries                                             //
///////////////////////////////////////////////////////////////////////////////
public ref class GmlDictionaryWriter
{
public:
  GmlDictionaryWriter (  GmlDictionary * pDictionary, GML_VERSION gmlVersion );
  virtual ~GmlDictionaryWriter();

public:
  void write( std::string fileNameP, GML_SCHEMA_TYPE schemaType );

  void writeDictionaryCollection( std::string fileNameP );
  void writeSingleDictionary( std::string fileNameP );

private:
  GmlDictionary              * pExternalCodeLists;  //  Zu schreibendes Dictionary
  GML_VERSION                  gmlVersion;          //  GML-Version (3.1. oder 3.2)
  System::Xml::XmlTextWriter ^ writer;              //  .NET XML-Writer
  System::String             ^ pActElement;         //  Aktuell geschriebenes XML-Element
  System::String             ^ pActAttrName;        //  Aktuell geschriebenes XML-Attribut

private:
  void writeDictionaryCollectionEntry ( GmlDictionaryEntry * pCodeListP );
};
