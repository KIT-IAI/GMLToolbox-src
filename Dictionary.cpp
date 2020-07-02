#include "StdAfx.h"

#include <time.h>
#include "Convert.h"
#include "Dictionary.h"
#include "constant.h"

using namespace System;

///////////////////////////////////////////////////////////////////////////////
//  GmlDictionary                                                            //
//  Modelliert ein GML-Dictionary                                            //
///////////////////////////////////////////////////////////////////////////////
GmlDictionary::GmlDictionary()
{

}

GmlDictionary::~GmlDictionary()
{
  std::map<std::string,GmlDictionaryEntry*>::iterator iter;

  for ( iter = mEntries.begin(); iter != mEntries.end(); iter++ )
    delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen neuen Dictionary-Entry hinzu                                  //
///////////////////////////////////////////////////////////////////////////////
void GmlDictionary::addDictionaryEntry ( GmlDictionaryEntry * pEntryP )
{
  std::string entryId = pEntryP->getName();

  std::map<std::string, GmlDictionaryEntry*>::iterator it = mEntries.find(entryId);

  if(it != mEntries.end())
  {
    delete it->second;
    mEntries.erase(it);
  }

  mEntries[entryId] = pEntryP;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen Dictionary-Entry                                           //
///////////////////////////////////////////////////////////////////////////////
GmlDictionaryEntry * GmlDictionary::getDictionaryEntry ( std::string idP )
{
	map<string,GmlDictionaryEntry*>::iterator iter;

  iter = mEntries.find ( idP );
  if ( iter != mEntries.end() ) return iter->second;
  else                          return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt ein oder alle Dictionary-Entry                                  //
///////////////////////////////////////////////////////////////////////////////
void GmlDictionary::removeDictionaryEntry (  std::string idP )
{
  mEntries.erase( idP );
}

void GmlDictionary::removeDictionaryEntries()
{
  mEntries.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob es zu einer id schon einen Dictionary-Entry gibt            //
///////////////////////////////////////////////////////////////////////////////
bool GmlDictionary::existEntryId ( std::string idP )
{
	map<string,GmlDictionaryEntry*>::iterator iter;

  iter = mEntries.find ( idP );
  if ( iter != mEntries.end() )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Dictionary-Entries                                          //
///////////////////////////////////////////////////////////////////////////////
size_t GmlDictionary::getDictionaryEntries ( std::vector<GmlDictionaryEntry*> & vEntriesP )
{
	map<string,GmlDictionaryEntry*>::iterator iter;

  for ( iter = mEntries.begin(); iter != mEntries.end(); iter++ )
  {
    GmlDictionaryEntry * pEntry = iter->second;
    vEntriesP.push_back( pEntry );
  }

  return mEntries.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten Entry in der map                                      //
///////////////////////////////////////////////////////////////////////////////
GmlDictionaryEntry * GmlDictionary::getFirstEntry()
{
  if ( mEntries.size() > 0 )
    return mEntries.begin()->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  GmlDictionaryEntry                                                       //
//  Modelliert einen GML-Dictionary Entry                                    //
///////////////////////////////////////////////////////////////////////////////
GmlDictionaryEntry::GmlDictionaryEntry()
{

}

GmlDictionaryEntry::~GmlDictionaryEntry()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob eine schluesselNummer schon verwendet wurde                //
///////////////////////////////////////////////////////////////////////////////
bool GmlDictionaryEntry::existSchluesselNummer( std::string schluesselNummerP )
{
	map<string,GmlDictionaryEntryDefinition>::iterator iter;

  iter = mDictionaryEntryDefinitions.find ( schluesselNummerP );
  if ( iter != mDictionaryEntryDefinitions.end() ) return true;
  else                                             return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob ein schluesselText schon verwendet wurde                   //
///////////////////////////////////////////////////////////////////////////////
bool GmlDictionaryEntry::existSchluesselText ( std::string schluesselTextP )
{
	map<string,string>::iterator iter;

  iter = mSchluesselTexte.find ( schluesselTextP );
  if ( iter != mSchluesselTexte.end() ) return true;
  else                                  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Definition, d.h. ein Paar schluesselNummer und                 //
//  GmlDictionaryEntryDefinition hinzu                                       //
///////////////////////////////////////////////////////////////////////////////
bool GmlDictionaryEntry::addDefinition ( std::string schluesselNummerP, GmlDictionaryEntryDefinition dictionaryEntryDefinition )
{
  if ( existSchluesselNummer ( schluesselNummerP ) ||
     existSchluesselText ( dictionaryEntryDefinition.name ) ) return false;

  mSchluesselTexte[dictionaryEntryDefinition.name] = schluesselNummerP;
  mDictionaryEntryDefinitions[schluesselNummerP]   = dictionaryEntryDefinition;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Ändert die Beschreibung (schluesselText), die einem Schlüssel zugeordnet //
//  ist                                                                      //
///////////////////////////////////////////////////////////////////////////////
bool GmlDictionaryEntry::setSchluesselText ( std::string schluesselNummerP, std::string schluesselTextP )
{
	string schluesselTextOld;

  if ( mDictionaryEntryDefinitions.find ( schluesselNummerP ) == mDictionaryEntryDefinitions.end() ) return false;

  schluesselTextOld = mDictionaryEntryDefinitions[schluesselNummerP].beschreibung;
  mSchluesselTexte.erase ( schluesselTextOld );

  mDictionaryEntryDefinitions[schluesselNummerP].beschreibung = schluesselTextP;
  mSchluesselTexte[schluesselTextP]                           = schluesselNummerP;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt einen Eintrag                                                   //
///////////////////////////////////////////////////////////////////////////////
bool GmlDictionaryEntry::removeDefinition ( std::string schluesselNummerP )
{
	string					     schluesselText;
	map<string,GmlDictionaryEntryDefinition>::iterator iterSchluesselNummern;
	map<string,string>::iterator                        iterSchluesselTexte;

  iterSchluesselNummern = mDictionaryEntryDefinitions.find ( schluesselNummerP );
  if ( iterSchluesselNummern != mDictionaryEntryDefinitions.end() )
  {
    schluesselText = iterSchluesselNummern->second.beschreibung;
    iterSchluesselTexte = mSchluesselTexte.find ( schluesselText );
    if ( iterSchluesselTexte != mSchluesselTexte.end() )
    {
      mDictionaryEntryDefinitions.erase ( iterSchluesselNummern );
      mSchluesselTexte.erase ( iterSchluesselTexte );
      return true;
    }
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die einer schluesselNummer zugeordneten schluesselText           //
///////////////////////////////////////////////////////////////////////////////
bool GmlDictionaryEntry::getSchluesselNummer ( std::string schluesselNummerP, std::string & schluesselTextP )
{
	map<string,string>::iterator iter;

  iter = mSchluesselTexte.find ( schluesselNummerP );
  if ( iter != mSchluesselTexte.end() )
  {
    schluesselTextP = iter->second;
    return true;
  }
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den einer schluesselNummer zugeordneten schluesselText aus einer //
//  GmlDictionaryEntryDefinition                                             //
///////////////////////////////////////////////////////////////////////////////
bool GmlDictionaryEntry::getSchluesselText ( std::string schluesselNummerP, std::string & schluesselTextP )
{
	map<string,GmlDictionaryEntryDefinition>::iterator iter;

  iter = mDictionaryEntryDefinitions.find ( schluesselNummerP );
  if ( iter != mDictionaryEntryDefinitions.end() )
  {
    schluesselTextP = iter->second.name;
		if ( schluesselTextP == "" )
      schluesselTextP = schluesselNummerP;
    return true;
  }
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Einträge der map mDictionaryEntryDefinitions                //
///////////////////////////////////////////////////////////////////////////////
size_t GmlDictionaryEntry::getCodelistEintraege ( vector<string> &vSchluesselNummernP, vector<GmlDictionaryEntryDefinition> &vDictionaryEntryDefinitions )
{
	map<string,GmlDictionaryEntryDefinition>::iterator iter;
	string                                             schluesselNummer;
  GmlDictionaryEntryDefinition                                   dictionaryEntryDefinition;

  for ( iter = mDictionaryEntryDefinitions.begin(); iter != mDictionaryEntryDefinitions.end(); iter++ )
  {
    schluesselNummer = iter->first;
    dictionaryEntryDefinition = iter->second;

    vSchluesselNummernP.push_back ( schluesselNummer );
    vDictionaryEntryDefinitions.push_back ( dictionaryEntryDefinition );
  }

  return mDictionaryEntryDefinitions.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Schlüssel                                                   //
///////////////////////////////////////////////////////////////////////////////
size_t GmlDictionaryEntry::getCodelistSchluessel ( vector<string> &vSchluesselNummernP )
{
  map<string,GmlDictionaryEntryDefinition>::iterator iter;
  string                                             schluesselNummer;

  for ( iter = mDictionaryEntryDefinitions.begin(); iter != mDictionaryEntryDefinitions.end(); iter++ )
  {
    schluesselNummer = iter->first;
    vSchluesselNummernP.push_back ( schluesselNummer );
  }

  return mDictionaryEntryDefinitions.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Einträge in mDictionaryEntryDefinitions           //
///////////////////////////////////////////////////////////////////////////////
size_t GmlDictionaryEntry::getCodelistEintraegeAnz()
{
  return mDictionaryEntryDefinitions.size();
}

///////////////////////////////////////////////////////////////////////////////
//  GmlDictionaryEntryDefinition                                             //
//  Komplexer Datentyp für einen Eintrag einer Liste von                     //
//  Schlüsselnummer / Langtext Paaren                                        //
///////////////////////////////////////////////////////////////////////////////
GmlDictionaryEntryDefinition::GmlDictionaryEntryDefinition()
{
  id                    = "";
  name                  = "";
  beschreibung          = "";
  identifier            = "";
  identifierCodespace   = "";

}
GmlDictionaryEntryDefinition::~GmlDictionaryEntryDefinition()
{

}


///////////////////////////////////////////////////////////////////////////////
//  HierarchicalCodeList                                                     //
//  Modelliert eine hierarchische Codeliste für BoreholeML                   //
///////////////////////////////////////////////////////////////////////////////
HierarchicalCodeList::HierarchicalCodeList(): GmlDictionaryEntry()
{

}

HierarchicalCodeList::~HierarchicalCodeList()
{
 
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Definition, d.h. ein Paar schluesselNummer, schluesselText     //
//  hinzu                                                                    //
///////////////////////////////////////////////////////////////////////////////
bool HierarchicalCodeList::addDefinition ( std::string schluesselNummerP, GmlDictionaryEntryDefinition dictionaryEntryDefinition )
{
  if ( existSchluesselNummer ( schluesselNummerP ) || 
    existSchluesselText ( dictionaryEntryDefinition.name ) ) return false;

  mSchluesselTexte[dictionaryEntryDefinition.name] = schluesselNummerP;
  mDictionaryEntryDefinitions[schluesselNummerP]   = dictionaryEntryDefinition;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Fügt einen CodeList-Entry hinzu                                          //
///////////////////////////////////////////////////////////////////////////////
bool HierarchicalCodeList::addCodeListEntry ( HierarchicalCodeListEntry pCodeListEntry )
{
  if ( pCodeListEntry.keyID == "" || pCodeListEntry.identifier == "" )
    return false;

  if ( pCodeListEntry.name == "" )
    pCodeListEntry.name = pCodeListEntry.identifier;

  addDefinition( pCodeListEntry.identifier, pCodeListEntry );
  
  mKey2CodeListEntries[pCodeListEntry.keyID]   = pCodeListEntry;
  mId2CodeListEntry[pCodeListEntry.identifier] = pCodeListEntry;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Ändert die Beschreibung (schluesselText), die einem Schlüssel zugeordnet //
//  ist                                                                      //
///////////////////////////////////////////////////////////////////////////////
bool HierarchicalCodeList::setSchluesselText ( std::string schluesselNummerP, std::string schluesselTextP )
{
  string schluesselTextOld;

  if ( mDictionaryEntryDefinitions.find ( schluesselNummerP ) == mDictionaryEntryDefinitions.end() ) return false;

  schluesselTextOld = mDictionaryEntryDefinitions[schluesselNummerP].name;
  mSchluesselTexte.erase ( schluesselTextOld );

  mDictionaryEntryDefinitions[schluesselNummerP].name = schluesselTextP;
  mSchluesselTexte[schluesselTextP]                   = schluesselNummerP;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den einer schluesselNummer zugeordneten schluesselText           //
///////////////////////////////////////////////////////////////////////////////
bool HierarchicalCodeList::getSchluesselText ( std::string schluesselNummerP, std::string & schluesselTextP )
{
  map<string,GmlDictionaryEntryDefinition>::iterator iter;

  iter = mDictionaryEntryDefinitions.find ( schluesselNummerP );
  if ( iter != mDictionaryEntryDefinitions.end() )
  {
    schluesselTextP = iter->second.beschreibung;
    if ( schluesselTextP == "" )
      schluesselTextP = schluesselNummerP;
    return true;
  }
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen CodeList-Entry ueber eine Id                               //
///////////////////////////////////////////////////////////////////////////////
bool HierarchicalCodeList::getCodeListEntryFromId( std::string identifier, HierarchicalCodeListEntry & codelistEntry )
{
  std::map<std::string, HierarchicalCodeListEntry>::iterator iter;

  iter = mId2CodeListEntry.find( identifier );
  if ( iter != mId2CodeListEntry.end() )
  {
    codelistEntry = iter->second;
    return true;
  }
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen CodeList-Entry ueber dessen Key                            //
///////////////////////////////////////////////////////////////////////////////
 bool HierarchicalCodeList::getCodeListEntryFromKey ( std::string keyId, HierarchicalCodeListEntry & codelistEntry )
{
  map<string,HierarchicalCodeListEntry>::iterator iter;

  iter = mKey2CodeListEntries.find( keyId );
  if ( iter != mKey2CodeListEntries.end() )
  {
    codelistEntry = iter->second;
    return true;
  }
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Parent eines CodeList Entry                                  //
///////////////////////////////////////////////////////////////////////////////
bool HierarchicalCodeList::getParentCodeListEntryFromKey ( std::string keyId, HierarchicalCodeListEntry & codelistEntry )
{
  HierarchicalCodeListEntry  codelistEntryLoc;

  bool success = getCodeListEntryFromKey( keyId, codelistEntryLoc );
  if (success )
    return getCodeListEntryFromKey ( codelistEntryLoc.parentID, codelistEntry );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle CodeList Entries                                            //
///////////////////////////////////////////////////////////////////////////////
size_t HierarchicalCodeList::getCodeListEntries ( std::vector<HierarchicalCodeListEntry> &vCodeListEntries )
{
  map<string,HierarchicalCodeListEntry>::iterator iter;

  for ( iter = mKey2CodeListEntries.begin(); iter != mKey2CodeListEntries.end(); iter++ )
    vCodeListEntries.push_back( iter->second );

  return vCodeListEntries.size();
}

///////////////////////////////////////////////////////////////////////////////
//  HierarchicalCodeListEntry                                                //
//  Modelliert einen Eintrag einer BoreholeML-Codeliste                      //
///////////////////////////////////////////////////////////////////////////////
HierarchicalCodeListEntry::HierarchicalCodeListEntry()
  :GmlDictionaryEntryDefinition()
{
  keyID            = "";
  parentID         = "0";
  hierarchyLevel   = 1;
  colorCode[0]     = -1;
  colorCode[1]     = -1;
  colorCode[2]     = -1;
  signaturePath    = "";
}

HierarchicalCodeListEntry::~HierarchicalCodeListEntry()
{
}

///////////////////////////////////////////////////////////////////////////////
//  GmlDictionaryReader                                                      //
//  Liest ein GML-Dictionary ein                                             //
///////////////////////////////////////////////////////////////////////////////
GmlDictionaryReader::GmlDictionaryReader( )
{
  reader = nullptr;
}

GmlDictionaryReader::~GmlDictionaryReader()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schließt den Reader und gibt das eingelesene Dokument frei               //
///////////////////////////////////////////////////////////////////////////////
void GmlDictionaryReader::closeReader()
{
  if ( reader != nullptr ) reader->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Liest ein Gml-Dictionary, das eine Sammlung von Dictionaries enthält     //
///////////////////////////////////////////////////////////////////////////////
bool GmlDictionaryReader::readGmlDictionaryCollection ( std::string fileName, GmlDictionary * pDictionary,  bool isCityGML )
{
  String             ^ STR;
  string               stdString;
  string               dictionaryName;
  bool                 firstElement = true;
  GmlDictionaryEntry * pEntry;

  try
  {
    String ^ fName = gcnew String( fileName.c_str() );
    reader         = gcnew System::Xml::XmlTextReader( fName );

    while( reader->Read() )
    {
      if ( reader->NodeType == System::Xml::XmlNodeType::Element )
      {
        pActElement = reader->LocalName;

        if ( firstElement )
        {
          if (pActElement->CompareTo("Dictionary") != 0 )
            return false;
          else
          {
            STR = reader->GetAttribute( "id" );
            if ( STR != nullptr )
            {
              QuConvert::systemStr2stdStr ( stdString, STR );
              pDictionary->setId ( stdString );
            }

          }
          firstElement = false;
        }

        if (pActElement->CompareTo( "description" ) == 0 )
        {
          STR = reader->ReadString();
          QuConvert::systemStr2stdStr ( stdString, STR );
          pDictionary->setBeschreibung ( stdString );
        }
        else
        if ( pActElement->CompareTo("identifier") == 0 )
        {
          STR = reader->GetAttribute( "codeSpace" );
          if ( STR != nullptr )
          {
            QuConvert::systemStr2stdStr( stdString,  STR );
            pDictionary->setIdentifierCodespace( stdString );

            STR = reader->ReadString();
            QuConvert::systemStr2stdStr ( stdString, STR );
            pDictionary->setName ( stdString );
          }
        }
        else
        if (pActElement->CompareTo( "name" ) == 0 )
        {
          STR = reader->ReadString();
          QuConvert::systemStr2stdStr ( dictionaryName, STR );
          pDictionary->setName ( dictionaryName );
        }
        else
        if (pActElement->CompareTo( "DefinitionCollection" ) == 0 )
        {
          pEntry = readGmlDefinitionCollection ( isCityGML );
          if ( pEntry != NULL )
          {
            if ( pEntry->getName() != "" && pEntry->getId() != pEntry->getName() )
              pEntry->setId( pEntry->getName() );
            pDictionary->addDictionaryEntry( pEntry );
          }
        }
      }
      else
      if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if ( pActElement->CompareTo( "Dictionary" ) == 0 ) break;
      }
    }
  }

  catch ( System::IO::FileNotFoundException ^ )
  {
    return false;
  }

  catch ( System::IO::DirectoryNotFoundException ^ )
  {
    return false;
  }

  catch ( System::Exception ^ )
  {
    return false;
  }

  return true;
}

 ///////////////////////////////////////////////////////////////////////////////
 //  Einlesen DefinitionCollection                                            //
 ///////////////////////////////////////////////////////////////////////////////
 GmlDictionaryEntry * GmlDictionaryReader::readGmlDefinitionCollection ( bool isCityGML )
 {
   String                     ^ STR;
   string                       stdString;
   string                       schluesselNummer = "0";
   string                       schluesselText   = "";
   bool                         success;
   GmlDictionaryEntry         * pEntry;
   GmlDictionaryEntryDefinition entryDefinition;

   pEntry = new GmlDictionaryEntry;

   STR = reader->GetAttribute ( "gml:id" );
   if ( STR != nullptr )
   {
     QuConvert::systemStr2stdStr ( stdString, STR );
     pEntry->setId ( stdString );
   }

   while( reader->Read() )
   {
     if ( reader->NodeType == System::Xml::XmlNodeType::Element )
     {
       pActElement = reader->LocalName;

       if (pActElement->CompareTo( "Definition" ) == 0 )
       {
         success = readDefinition ( schluesselNummer, entryDefinition, isCityGML );
         if ( success )
           pEntry->addDefinition( schluesselNummer, entryDefinition );
       }
       else
       if ( pActElement->CompareTo( "description" ) == 0 )
       {
         STR = reader->ReadString();
         QuConvert::systemStr2stdStr ( stdString, STR );
         pEntry->setBeschreibung ( stdString );
       }
       else
       if (pActElement->CompareTo( "name" ) == 0 )
       {
         STR = reader->ReadString();
         QuConvert::systemStr2stdStr ( stdString, STR );
         pEntry->setName ( stdString );
       }

     }
     else
     if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
     {
       pActElement = reader->LocalName;
       if ( pActElement->CompareTo( "DefinitionCollection" ) == 0 )
         break;
     }
   }

   return pEntry;
 }

 ///////////////////////////////////////////////////////////////////////////////
 //  Einlesen eines einzelnen GML-Dictionaries                                //
 ///////////////////////////////////////////////////////////////////////////////
 bool GmlDictionaryReader::readGmlSingleDictionary ( std::string fileName,  GmlDictionary * pDictionary,  bool isCityGML )
 {
   GmlDictionaryEntry * pEntry;

   pEntry = readGmlSingleDictionary ( fileName, isCityGML );
   if ( pEntry != NULL )
   {
     pDictionary->addDictionaryEntry ( pEntry );
     return true;
   }
   else
     return false;
 }


 ///////////////////////////////////////////////////////////////////////////////
 //  Liest ein einzelnes Gml-Dictionary                                       //
 ///////////////////////////////////////////////////////////////////////////////
GmlDictionaryEntry * GmlDictionaryReader::readGmlSingleDictionary ( std::string fileName, bool isCityGML )
{
  String                     ^ STR;
  string                       stdString;
  string                       dictionaryName;
  bool	                       firstElement = true;
  string                       schluesselNummer = "0";
  string                       schluesselText   = "";
  bool                         success;
  GmlDictionaryEntry         * pEntry;
  GmlDictionaryEntryDefinition entryDefinition;

  try
  {
    String ^ fName = gcnew String( fileName.c_str() );
    reader         = gcnew System::Xml::XmlTextReader( fName );

    pEntry = new GmlDictionaryEntry;

    while( reader->Read() )
    {
      if ( reader->NodeType == System::Xml::XmlNodeType::Element )
      {
        pActElement = reader->LocalName;

        if ( firstElement )
        {
          if (pActElement->CompareTo("Dictionary") != 0 )
            return NULL;
          else
          {
            STR = reader->GetAttribute( "gml:id" );
            if ( STR != nullptr )
            {
              QuConvert::systemStr2stdStr ( stdString, STR );
              pEntry->setId ( stdString );
            }

          }
          firstElement = false;
        }

        if (pActElement->CompareTo( "description" ) == 0 )
        {
          STR = reader->ReadString();
          QuConvert::systemStr2stdStr ( stdString, STR );
          pEntry->setBeschreibung ( stdString );
        }
        else
        if ( pActElement->CompareTo("identifier") == 0 )
        {
          STR = reader->GetAttribute( "codeSpace" );
          if ( STR != nullptr )
          {
            QuConvert::systemStr2stdStr( stdString,  STR );
            pEntry->setIdentifierCodespace( stdString );
            
            STR = reader->ReadString();
            QuConvert::systemStr2stdStr ( stdString, STR );
            pEntry->setIdentifier ( stdString );
          }
        }
        else
        if (pActElement->CompareTo( "name" ) == 0 )
        {
          STR = reader->ReadString();
          QuConvert::systemStr2stdStr ( dictionaryName, STR );
          pEntry->setName ( dictionaryName );
        }
        else
        if (pActElement->CompareTo( "Definition" ) == 0 )
        {
          success = readDefinition ( schluesselNummer, entryDefinition, isCityGML );
          if ( success )
            pEntry->addDefinition( schluesselNummer, entryDefinition );
        }
      }
      else
      if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if ( pActElement->CompareTo( "Dictionary" ) == 0 ) break;
      }
    }
  }
  catch ( System::IO::FileNotFoundException ^ )
  {
    return NULL;
  }

  catch ( System::IO::DirectoryNotFoundException ^ )
  {
    return NULL;
  }

  catch ( System::Exception ^ )
  {
    return NULL;
  }

  return pEntry;
}


///////////////////////////////////////////////////////////////////////////////
//  Liest eine Zuordnung eines GML-Dictionary Entry                          //
///////////////////////////////////////////////////////////////////////////////
bool GmlDictionaryReader::readDefinition ( string &schluesselNummer, GmlDictionaryEntryDefinition &definitionEntry, bool isCityGML )
{
  String ^ STR;
  String ^ CODESPACE;
  string   stdString;

  STR = reader->GetAttribute( "gml:id" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( definitionEntry.id,  STR );

  while( reader->Read() )
  {
    if ( reader->NodeType == System::Xml::XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if (pActElement->CompareTo( "description" ) == 0 )
      {
        if ( !isCityGML )
        {
          STR = reader->ReadString();
          QuConvert::systemStr2stdStr ( definitionEntry.beschreibung, STR );
        }
      }
      else
      if (pActElement->CompareTo( "name" ) == 0 )
      {    
        CODESPACE = reader->GetAttribute( "codeSpace" );

        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        if ( isCityGML )
        {
          if ( CODESPACE != nullptr ) 
            definitionEntry.name = stdString;
          else
            definitionEntry.beschreibung = stdString;
        }
        else
          definitionEntry.name = stdString;
      }
      else
      if (pActElement->CompareTo( "identifier" ) == 0 )
      {
        STR = reader->GetAttribute( "codeSpace" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr( definitionEntry.identifierCodespace,  STR );

          STR = reader->ReadString();
          QuConvert::systemStr2stdStr ( definitionEntry.identifier, STR );
        }

      }

    }
    else
    if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
    {
      pActElement = reader->LocalName;
      if ( pActElement->CompareTo( "Definition" ) == 0 ) break;
    }
  }

  schluesselNummer = definitionEntry.name;

  return true;
}

//////////////////////////////////////////////////////////////////////////////
// Einlesen einer BoreholeML CodeList                                       //
//////////////////////////////////////////////////////////////////////////////
HierarchicalCodeList * GmlDictionaryReader::readBMLCodeList ( string fileName )
{
  String                    ^ STR;
  string                      stdString;
  bool	                      success, firstElement = true;
  HierarchicalCodeList      * pCodeList = nullptr;
  HierarchicalCodeListEntry   codelistEntry;

  try
  {
    String ^ fName = gcnew String( fileName.c_str() );
    reader         = gcnew System::Xml::XmlTextReader( fName );

    while( reader->Read() )
    {
      if ( reader->NodeType == System::Xml::XmlNodeType::Element )
      {
        pActElement = reader->LocalName;

        if ( firstElement )
        {
          if (pActElement->CompareTo("ML_CodeListDictionary") != 0 )
          {
            System::Windows::Forms::MessageBox::Show( "Gelesene Datei enthält keine BoreholeML Codeliste");
            return NULL;
          }
          else
          {
            pCodeList = new HierarchicalCodeList();
            STR = reader->GetAttribute( "gml:id" );
            if ( STR != nullptr )
            {
              QuConvert::systemStr2stdStr ( stdString, STR );
              pCodeList->setId ( stdString );
            }

          }
          firstElement = false;
        }

        if ( pActElement->CompareTo( "description" ) == 0 )
        {
          STR = reader->ReadString();
          QuConvert::systemStr2stdStr ( stdString, STR );
          pCodeList->setBeschreibung ( stdString );
        }
        else
          if ( pActElement->CompareTo( "name" ) == 0 )
          {
            STR = reader->ReadString();
            QuConvert::systemStr2stdStr ( stdString, STR );
            pCodeList->setName ( stdString );
          }
          else
            if ( pActElement->CompareTo( "codeEntry" ) == 0 )
            {
              success = readHierarchicalCodeListEntry( codelistEntry );
              if ( success )
                pCodeList->addCodeListEntry( codelistEntry );
            }
      }
      else
        if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
        {
          pActElement = reader->LocalName;
          if ( pActElement->CompareTo( "ML_CodeListDictionary" ) == 0 ) break;
        }
    }
  }

  catch ( System::IO::FileNotFoundException ^ )
  {
    return NULL;
  }

  catch ( System::IO::DirectoryNotFoundException ^ )
  {
    return NULL;
  }

  catch ( System::Exception ^ )
  {
    return NULL;
  }

  return pCodeList;
}

//////////////////////////////////////////////////////////////////////////////
// Einlesen eines BoreholeML CodeList Entry                                 //
//////////////////////////////////////////////////////////////////////////////
bool GmlDictionaryReader::readHierarchicalCodeListEntry ( HierarchicalCodeListEntry & codelistEntry )
{
  String              ^ STR;
  string                stdString;
  int                   level;
  int                   col;
  cli::array<String^> ^ splittedString;

  while( reader->Read() )
  {
    if ( reader->NodeType == System::Xml::XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if (pActElement->CompareTo( "name" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        codelistEntry.name = stdString;
      }
      else
      if (pActElement->CompareTo( "description" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        codelistEntry.beschreibung = stdString;
      }
      else
      if (pActElement->CompareTo( "identifier" ) == 0 )
      {
        STR = reader->GetAttribute( "codeSpace" );
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr( stdString, STR );
          codelistEntry.identifierCodespace = stdString;
        }
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        codelistEntry.identifier = stdString;
      }
      else
      if (pActElement->CompareTo( "keyID" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        codelistEntry.keyID = stdString;
      }
      else
      if (pActElement->CompareTo( "parentID" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        codelistEntry.parentID = stdString;
      }
      else
      if (pActElement->CompareTo( "hierarchyLevel" ) == 0 )
      {
        STR = reader->ReadString();
        level = XmlConvert::ToInt32 ( STR );
        codelistEntry.hierarchyLevel = level;
      }
      else
      if (pActElement->CompareTo( "signaturePath" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        codelistEntry.signaturePath = stdString;
      }
      else
      if (pActElement->CompareTo( "colorCode" ) == 0 )
      {
        STR = reader->ReadString();
        splittedString = STR->Split ( ',' );
        for ( int i = 0; i < 3; i++ )
        {
          col = System::Xml::XmlConvert::ToInt32( splittedString[i] );
          codelistEntry.colorCode[i] = col;
        }
      }
      else
      if (pActElement->CompareTo( "alternativeExpression" ) == 0 )
      {
        readAlternativeExpression ( codelistEntry );
      }
    }
    else
    if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
    {
      pActElement = reader->LocalName;
      if ( pActElement->CompareTo( "codeEntry" ) == 0 )
        break;
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest alternativeExpression                                              //
///////////////////////////////////////////////////////////////////////////////
void GmlDictionaryReader::readAlternativeExpression ( HierarchicalCodeListEntry & codelistEntry )
{
  String ^ STR;
  string   stdString;
  string   alternativeSchluesselText = "";
  string   languageCode;

  while( reader->Read() )
  {
    if ( reader->NodeType == System::Xml::XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if (pActElement->CompareTo( "name" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( alternativeSchluesselText, STR );
      }
      else
        if (pActElement->CompareTo( "locale" ) == 0 )
        {
          STR = reader->GetAttribute ( "xlink:href" );
          if ( STR != nullptr )
          {
            STR = STR->ToUpper();
            if ( STR->CompareTo( "EN.XML" ) == 0 )
              languageCode = "EN";
            else
              if ( STR->CompareTo( "DE.XML" ) == 0 )
                languageCode = "DE";
              else
                languageCode = "Unknown";
          }
        }
    }
    else
      if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if ( pActElement->CompareTo( "alternativeExpression" ) == 0 )
          break;
      }
  }

  if ( languageCode != "Unknown" && alternativeSchluesselText != "" )
    codelistEntry.mAlternativeText[languageCode] = alternativeSchluesselText;
}


//////////////////////////////////////////////////////////////////////////////
// Einlesen einer INSPIRE CodeList; Übergeben ein XMLReader                 //
//////////////////////////////////////////////////////////////////////////////
GmlDictionaryEntry * GmlDictionaryReader::readINSPIRECodeList ( System::Xml::XmlTextReader ^ reader, bool useFullCode )
{
  String              ^ STR;
  string                stdString;
  bool	                success, firstElement = true;
  GmlDictionaryEntry  * pCodeList = nullptr;

  try
  {
    while( reader->Read() )
    {
      if ( reader->NodeType == System::Xml::XmlNodeType::Element )
      {
        pActElement = reader->LocalName;

        if ( firstElement )
        {
          if (pActElement->CompareTo("codelist") != 0 )
          {
            System::Windows::Forms::MessageBox::Show( "Gelesene Datei enthält keine INSPIRE Codeliste");
            return NULL;
          }
          else
          {
            pCodeList = new GmlDictionaryEntry();
            STR = reader->GetAttribute( "id" );
            if ( STR != nullptr )
            {
              QuConvert::systemStr2stdStr ( stdString, STR );
              pCodeList->setId ( stdString );
            }

          }
          firstElement = false;
        }

        if ( pActElement->CompareTo( "definition" ) == 0 )
        {
          STR = reader->ReadString();
          QuConvert::systemStr2stdStr ( stdString, STR );
          pCodeList->setBeschreibung ( stdString );
        }
        else
        if ( pActElement->CompareTo( "label" ) == 0 )
        {
          STR = reader->ReadString();
          QuConvert::systemStr2stdStr ( stdString, STR );
          pCodeList->setName ( stdString );
        }
        else
        if ( pActElement->CompareTo( "value" ) == 0 )
        {
          success = readINSPIRECodeListEntry( pCodeList, useFullCode );
        }
      }
      else
      if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if ( pActElement->CompareTo( "codelist" ) == 0 ) break;
      }
    }
  }

  catch ( System::IO::FileNotFoundException ^ /*e*/ )
  {
    return NULL;
  }

  catch ( System::IO::DirectoryNotFoundException ^ /*e*/ )
  {
    return NULL;
  }

  catch ( System::Exception ^ /*e*/ )
  {
    return NULL;
  }

  return pCodeList;
}

//////////////////////////////////////////////////////////////////////////////
// Einlesen einer INSPIRE CodeList; Übergeben wird die Registry-URL         //
//////////////////////////////////////////////////////////////////////////////
GmlDictionaryEntry * GmlDictionaryReader::readINSPIRECodeList (  String ^ URL, bool useFullCode )
{
  System::Net::WebRequest  ^ request;
  System::Net::WebResponse ^ response;

  if ( URL[URL->Length - 1] == '/' )
    URL = URL->Remove ( URL->Length - 1 );

  int index =URL->LastIndexOf ( "/" );
  String ^ codelistName = URL->Substring ( index + 1, URL->Length - index - 1 );
  String ^ REGISTRY_URL = String::Concat (URL, "/", codelistName, ".de.xml" );

  try
  {
    request = System::Net::WebRequest::Create ( REGISTRY_URL );
    request->Credentials = System::Net::CredentialCache::DefaultNetworkCredentials;

    response = request->GetResponse();
    System::Net::HttpWebResponse ^ httpResponse = dynamic_cast<System::Net::HttpWebResponse^>( response );
    String ^ status = httpResponse->StatusDescription;

    if ( status == "OK" )
    {
      System::IO::Stream ^responseStream =  httpResponse->GetResponseStream();
      GmlDictionaryReader ^ pReader = gcnew GmlDictionaryReader();
      GmlDictionaryEntry * pCodelist = pReader->readINSPIRECodeList ( responseStream, useFullCode );
      return pCodelist;
    }
    else
      return NULL;
  }
  catch ( System::Exception ^ )
  {
    return NULL;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Einlesen einer INSPIRE CodeList; Übergeben wird der Dateiname bzw. die   //
//  URL der XML-Datei                                                       //
//////////////////////////////////////////////////////////////////////////////
GmlDictionaryEntry * GmlDictionaryReader::readINSPIRECodeList ( string fileName, bool useFullCode )
{
  try
  {
    String ^ fName = gcnew String( fileName.c_str() );
    reader         = gcnew System::Xml::XmlTextReader( fName );

    return readINSPIRECodeList  ( reader, useFullCode );
  }
  catch ( System::Exception ^ /*e*/ )
  {
    return NULL;
  }

}

//////////////////////////////////////////////////////////////////////////////
// Einlesen einer INSPIRE CodeList; Übergeben wird ein Stream               //
//////////////////////////////////////////////////////////////////////////////
GmlDictionaryEntry * GmlDictionaryReader::readINSPIRECodeList (   System::IO::Stream ^stream, bool useFullCode )
{
  try
  {
    reader = gcnew System::Xml::XmlTextReader( stream );

    return readINSPIRECodeList  ( reader, useFullCode );
  }
  catch ( System::Exception ^ /*e*/ )
  {
    return NULL;
  }

}


//////////////////////////////////////////////////////////////////////////////
// Einlesen eines INSPIRE CodeList Entry                                     //
//////////////////////////////////////////////////////////////////////////////
bool GmlDictionaryReader::readINSPIRECodeListEntry ( GmlDictionaryEntry * codelistEntry, bool useFullCode )
{
  String         ^ STR;
  String         ^ SCHLUESSEL;
  string           schluessel = "";
  string           label;
  string           definition;

  GmlDictionaryEntryDefinition def;


  SCHLUESSEL = reader->GetAttribute( "id" );
  if ( SCHLUESSEL->CompareTo( "" ) != 0 )
    QuConvert::systemStr2stdStr( schluessel, SCHLUESSEL );


  while( reader->Read() )
  {
    if ( reader->NodeType == System::Xml::XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if (pActElement->CompareTo( "label" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( def.name, STR );
      }
      else
      if (pActElement->CompareTo( "definition" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( def.beschreibung, STR );
      }   
      else
        skipElement ( pActElement );
    }
    else
    if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
    {
      pActElement = reader->LocalName;
      if ( pActElement->CompareTo( "value" ) == 0 )
        break;
    }
  }

  if ( schluessel != "" )
  {
    if ( !useFullCode )
    {
      int index = SCHLUESSEL->LastIndexOf ( "/" );
      String ^ CODE = SCHLUESSEL->Substring ( index + 1, SCHLUESSEL->Length - index - 1 );
      QuConvert::systemStr2stdStr ( schluessel, CODE );
    }
    codelistEntry->addDefinition( schluessel, def );
    return true;
  }
  else
    return false;
}

//////////////////////////////////////////////////////////////////////////////
// Ünerlesen eines XML-Elements                                             //
//////////////////////////////////////////////////////////////////////////////
void GmlDictionaryReader::skipElement ( String ^ pSkipElement )
{
  String ^ pActElement;

  while ( reader->Read() )
  {
    if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
    {
      pActElement = reader->LocalName;
      if ( pActElement->CompareTo( pSkipElement ) == 0 )
        break;
    }
  }
}



///////////////////////////////////////////////////////////////////////////////
//  GmlDictionaryWriter                                                      //
//  Schreiben einer CodeList                                                 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor                                                 //
///////////////////////////////////////////////////////////////////////////////
GmlDictionaryWriter::GmlDictionaryWriter ( GmlDictionary * pDictionary, GML_VERSION gmlVersionP )
{
  pExternalCodeLists = pDictionary;
  gmlVersion         = gmlVersionP;
}

GmlDictionaryWriter:: ~GmlDictionaryWriter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben der ExternalCodeList                                           //
///////////////////////////////////////////////////////////////////////////////
void GmlDictionaryWriter::write( std::string fileNameP, GML_SCHEMA_TYPE schemaType    )
{
  if ( schemaType == XPLANGML_2_0 ||  schemaType == XPLANGML_3_0 )
    writeDictionaryCollection ( fileNameP );
  else
  if ( schemaType == XPLANGML_4_0 || schemaType == XPLANGML_4_1 || schemaType == XPLANGML_5_0 ||
       schemaType == XPLANGML_5_1 || schemaType == XPLANGML_5_2 || schemaType == XPLANGML_5_3 ||
    schemaType == INSPIRE_LU_PLANNED )
    writeSingleDictionary ( fileNameP );
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben alle Codelists in einem Dictionary                             //
///////////////////////////////////////////////////////////////////////////////
void GmlDictionaryWriter::writeDictionaryCollection( std::string fileNameP )
{
  String			      ^ fileName;
  String					  ^ STR;
  String					  ^ comment;
  String					  ^ date;
  string				      stdString;
  char					      buffer[16];
	size_t  			      i, anz;
  vector<GmlDictionaryEntry*> codeLists;
  GmlDictionaryEntry        * pCodeList;

  _strdate_s ( buffer );
  date = QuConvert::ToString ( string ( buffer ) );

  fileName = QuConvert::ToString ( fileNameP );

  System::Text::UTF8Encoding ^ utf8 = gcnew System::Text::UTF8Encoding();
  writer                            = gcnew System::Xml::XmlTextWriter( fileName, utf8);

  writer->Formatting = System::Xml::Formatting::Indented;
  writer->WriteStartDocument( true );

  comment = "Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: ";
  comment = String::Concat ( comment, date );

  writer->WriteComment( comment );

  writer->WriteStartElement("Dictionary  ");

  writer->WriteAttributeString("gml:id",	    "CodeLists");
  if ( gmlVersion == GML_3_1 )
  {
    writer->WriteAttributeString("xmlns",	    "http://www.opengis.net/gml");
    writer->WriteAttributeString("xmlns:gml",   "http://www.opengis.net/gml");
  }
  else
  if ( gmlVersion == GML_3_2 )
  {
    writer->WriteAttributeString("xmlns",	    "http://www.opengis.net/gml/3.2");
    writer->WriteAttributeString("xmlns:gml",   "http://www.opengis.net/gml/3.2");
  }
  writer->WriteAttributeString("xmlns:xsi",	"http://www.w3.org/2001/XMLSchema-instance");

  stdString = pExternalCodeLists->getBeschreibung();
  if ( stdString != "" )
  {
    STR = QuConvert::ToString ( stdString );
    writer->WriteStartElement ( "description" );
    writer->WriteString ( STR );
    writer->WriteEndElement();
  }

  if ( gmlVersion == GML_3_2 )
  {
    writer->WriteStartElement( "identifier" );

    stdString = pExternalCodeLists->getIdentifierCodespace();
    if ( stdString != "" )
    {
      STR = QuConvert::ToString ( stdString );
      writer->WriteAttributeString( "codeSpace", STR );
    }

    stdString = pExternalCodeLists->getIdentifier();
    if ( stdString == "" )
      stdString = pExternalCodeLists->getName();
    STR = QuConvert::ToString ( stdString );

    writer->WriteString ( STR );
    writer->WriteEndElement();
  }

  stdString = pExternalCodeLists->getName();
  if ( stdString != "" )
  {
    STR = QuConvert::ToString ( stdString );
    writer->WriteStartElement ( "name" );
    writer->WriteString ( STR );
    writer->WriteEndElement();
  }


  anz = pExternalCodeLists->getDictionaryEntries ( codeLists );
  for ( i = 0; i < anz; i++ )
  {
    pCodeList = codeLists[i];
    
      writeDictionaryCollectionEntry ( pCodeList );
  }

  writer->WriteEndElement();

  writer->Flush();
  writer->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt ein einzelnes Dictionary                                        //
///////////////////////////////////////////////////////////////////////////////
void GmlDictionaryWriter::writeSingleDictionary( std::string fileNameP )
{
  String			        ^ fileName;
  String					    ^ STR;
  String              ^ STR_beschreibung;
  String              ^ STR_codespace;
  String					    ^ comment;
  String					    ^ date;
  string				        stdString;
  string                codeListName;
  string                codeSpace;
  char					        buffer[16];
	size_t				        i, anz;
  GmlDictionaryEntry  * pCodeList;

  vector<string>                       schluessel;
  vector<GmlDictionaryEntryDefinition> vDictionaryEntryDefinitions;
  GmlDictionaryEntryDefinition         entryDefinition;

  pCodeList =  pExternalCodeLists->getFirstEntry();
  if ( pCodeList == NULL )
    return;

  _strdate_s ( buffer );
  date = QuConvert::ToString ( string ( buffer ) );

  fileName = QuConvert::ToString ( fileNameP );

  System::Text::UTF8Encoding ^ utf8 = gcnew System::Text::UTF8Encoding();
  writer                            = gcnew System::Xml::XmlTextWriter( fileName, utf8);

  writer->Formatting = System::Xml::Formatting::Indented;
  writer->WriteStartDocument( true );

  comment = "Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: ";
  comment = String::Concat ( comment, date );

  writer->WriteComment( comment );

  writer->WriteStartElement("Dictionary");

  writer->WriteAttributeString("gml:id",	    QuConvert::ToString( pCodeList->getId() ) );

  if ( gmlVersion == GML_3_1 )
  {
    writer->WriteAttributeString("xmlns",	    "http://www.opengis.net/gml");
    writer->WriteAttributeString("xmlns:gml",   "http://www.opengis.net/gml");
  }
  else
  if ( gmlVersion == GML_3_2 )
  {
    writer->WriteAttributeString("xmlns",	    "http://www.opengis.net/gml/3.2");
    writer->WriteAttributeString("xmlns:gml",   "http://www.opengis.net/gml/3.2");
  }

  writer->WriteAttributeString("xmlns:xsi",	"http://www.w3.org/2001/XMLSchema-instance");


  stdString = pCodeList->getBeschreibung();
  if ( stdString != "" )
  {
    STR = QuConvert::ToString ( stdString );
    writer->WriteStartElement ( "description" );
    writer->WriteString ( STR );
    writer->WriteEndElement();
  }

  if ( gmlVersion == GML_3_2 )
  {
    stdString = pCodeList->getIdentifier();
    if ( stdString == "" )
      stdString =  pCodeList->getName();
    
    STR = QuConvert::ToString ( stdString );
    writer->WriteStartElement ( "identifier" );

    if ( pCodeList->getIdentifierCodespace() != "" )
    {
      STR_codespace = QuConvert::ToString ( pCodeList->getIdentifierCodespace() );
      writer->WriteAttributeString( "codeSpace", STR_codespace );
    }
    writer->WriteString ( STR );
    writer->WriteEndElement();   
  }

  codeListName = pCodeList->getName();
  if ( stdString != "" )
  {
    STR = QuConvert::ToString ( stdString );
    writer->WriteStartElement ( "name" );
    writer->WriteString ( STR );
    writer->WriteEndElement();
  }

  anz = pCodeList->getCodelistEintraege ( schluessel, vDictionaryEntryDefinitions );
  for ( i = 0; i < anz; i++ )
  {
    entryDefinition = vDictionaryEntryDefinitions[i];

    writer->WriteStartElement ( "dictionaryEntry" );
    writer->WriteStartElement ( "Definition" );
    
    if ( gmlVersion == GML_3_2 )
    {
      STR = QuConvert::ToString ( entryDefinition.id );
      writer->WriteAttributeString ( "gml:id", STR );
    }

    writer->WriteStartElement ( "description" );
    STR_beschreibung = QuConvert::ToString ( entryDefinition.beschreibung );
    writer->WriteString ( STR_beschreibung );
    writer->WriteEndElement();

    if ( gmlVersion == GML_3_2 )
    {
      stdString = entryDefinition.identifier;
      if ( stdString == "" )
        stdString = entryDefinition.name;


      STR = QuConvert::ToString ( stdString );
      writer->WriteStartElement ( "identifier" );

      if ( entryDefinition.identifierCodespace != "" )
      {
        STR_codespace = QuConvert::ToString ( entryDefinition.identifierCodespace );
        writer->WriteAttributeString( "codeSpace", STR_codespace );
      }

      writer->WriteString ( STR );
      writer->WriteEndElement();
    }

    writer->WriteStartElement ( "name" );
    writer->WriteString (QuConvert::ToString( entryDefinition.name ) );
    writer->WriteEndElement();

    writer->WriteEndElement();
    writer->WriteEndElement();
  }

  writer->WriteEndElement();

  writer->Flush();
  writer->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer CodeList                                                 //
///////////////////////////////////////////////////////////////////////////////
void GmlDictionaryWriter::writeDictionaryCollectionEntry ( GmlDictionaryEntry * pCodeListP )
{
  String		 ^ STR_schluessel;
  String		 ^ STR_beschreibung;
//  string		   stdString;
  string		   codeListName;
	string       identifier;
	string       identifierCodespace;
	string        entryId;
	size_t        i, anz;
  vector<string> schluessel;

  vector<GmlDictionaryEntryDefinition> vEntryDefinitions;
  GmlDictionaryEntryDefinition         entryDefinition;

  writer->WriteStartElement ( "dictionaryEntry" );
  writer->WriteStartElement ( "DefinitionCollection" );

  codeListName = pCodeListP->getId();
  if ( codeListName == "" )
    codeListName = pCodeListP->getName();

  writer->WriteAttributeString ( "gml:id", QuConvert::ToString ( Constant::createGmlId() ) );

  if ( gmlVersion == GML_3_2 )
  {
    identifier = pCodeListP->getIdentifier();
    if ( identifier == "" )
			identifier =  pCodeListP->getName();

    writer->WriteStartElement ( "identifier" );
    if ( pCodeListP->getIdentifierCodespace() != "" )
    {
      writer->WriteAttributeString( "codeSpace", QuConvert::ToString ( pCodeListP->getIdentifierCodespace() ) );
    }
    writer->WriteString ( QuConvert::ToString ( identifier ) );
    writer->WriteEndElement();   
  }

  anz = pCodeListP->getCodelistEintraege ( schluessel, vEntryDefinitions );
  for ( i = 0; i < anz; i++ )
  {
    entryDefinition = vEntryDefinitions[i];

    writer->WriteStartElement ( "definitionMember" );
    writer->WriteStartElement ( "Definition" );

    if ( entryDefinition.id != "" )
			STR_schluessel = QuConvert::ToString ( entryDefinition.id );
    else
			STR_schluessel = QuConvert::ToString ( codeListName + "_" + schluessel[i] );
    writer->WriteAttributeString ( "gml:id", QuConvert::ToString ( Constant::createGmlId() ) );

    writer->WriteStartElement ( "description" );
    STR_beschreibung = QuConvert::ToString ( entryDefinition.beschreibung );
    writer->WriteString ( STR_beschreibung );
    writer->WriteEndElement();

    identifier = entryDefinition.identifier;
		if ( identifier == "" )
			identifier = codeListName + ":" + schluessel[i];

		writer->WriteStartElement ( "identifier" );

    if ( entryDefinition.identifierCodespace != "" )
		  identifierCodespace = entryDefinition.identifierCodespace;
		else
			identifierCodespace = pCodeListP->getIdentifierCodespace();
    writer->WriteAttributeString( "codeSpace", QuConvert::ToString (identifierCodespace ) );    

    writer->WriteString ( QuConvert::ToString ( identifier ) );
    writer->WriteEndElement();    

    writer->WriteStartElement ( "name" );
    writer->WriteString ( QuConvert::ToString ( entryDefinition.name ) );
    writer->WriteEndElement();

    writer->WriteEndElement();
    writer->WriteEndElement();
  }
  writer->WriteEndElement();
  writer->WriteEndElement();
}