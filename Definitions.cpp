#include "StdAfx.h"

#include <time.h>
#include <vector>
#include <map>
#include <set>
#include <string>

#include "Convert.h"
#include "Dictionary.h"
#include "Definitions.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  DefinitionReader                                                         //
//  XML-Reader zum Einlesen der Definitionen                                 //
///////////////////////////////////////////////////////////////////////////////
DefinitionReader::DefinitionReader ( TypeDefinitions * pTypeDefinitionsP )
{
  pTypeDefinitions = pTypeDefinitionsP;
}

DefinitionReader::~DefinitionReader()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Read-Routine                                                             //
///////////////////////////////////////////////////////////////////////////////
bool DefinitionReader::read( std::string fileName )
{
	String         ^ END_TAG;
	std::string      stdString;
	TypeDefinition * pTypeDefinition;
	bool		        firstElement = true;


	String ^ fName = gcnew System::String( fileName.c_str() );
	reader         = gcnew XmlTextReader( fName );

	while( reader->Read() )
	{
		if ( reader->NodeType == XmlNodeType::Element )
		{
			pActElement = reader->LocalName;
			if ( firstElement)
			{
				END_TAG = pActElement;
				firstElement = false;
			}

			if ( pActElement->CompareTo( "TypeDefinition" ) == 0 )
			{
				pTypeDefinition = readTypeDefinition();
			}

		}
		else
		if ( reader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = reader->LocalName;
			if ( pActElement->CompareTo(END_TAG ) == 0 ) break;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Schließt der Reader und gibt die eingelesenen Dateien frei               //
///////////////////////////////////////////////////////////////////////////////
void DefinitionReader::closeReader()
{
  if ( reader != nullptr ) reader->Close();
}


///////////////////////////////////////////////////////////////////////////////
//  Einlesen DefinitionType		                                               //
///////////////////////////////////////////////////////////////////////////////
TypeDefinition * DefinitionReader::readTypeDefinition()
{
	String		                ^ STR;
  string                      gmlId;
  string                      identifier;
  string                      name;
  string                      description;
  string                      classification;
  string                      supertypeRef;
  string                      stdString;
  unsigned int                i;
	TypeDefinition            * pTypeDefinition;
	PropertyDefinition        * pPropertyDefinition;
  vector<PropertyDefinition*> vPropertyDefinitions;

	STR = reader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( gmlId, STR );
	}
	
	while( reader->Read() )
	{
		if ( reader->NodeType == XmlNodeType::Element )
		{
			pActElement = reader->LocalName;

			if (  pActElement->CompareTo( "name" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
          name = addNamespaceKüerzel( stdString );
				}
			}
			else
			if (  pActElement->CompareTo( "identifier" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( identifier, STR );
				}
			}
			else
			if (  pActElement->CompareTo( "description" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( description, STR );
				}
			}
			else
			if (  pActElement->CompareTo( "classification" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( classification, STR );
				}
			}
      else
        if (  pActElement->CompareTo( "supertypeRef" ) == 0 )
        {
          STR = reader->GetAttribute( "xlink:href" );
          if ( STR != nullptr )
          {
            QuConvert::systemStr2stdStr ( supertypeRef, STR );
          }
        }
			else
			if (  pActElement->CompareTo( "PropertyDefinition" ) == 0 )
			{
				pPropertyDefinition = readPropertyDefinition();
				if ( pPropertyDefinition != NULL )
          vPropertyDefinitions.push_back( pPropertyDefinition );
			}
		}
		else
		if ( reader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = reader->LocalName;
			if ( pActElement->CompareTo( "TypeDefinition" ) == 0 ) break;
		}
	}			

  pTypeDefinition = pTypeDefinitions->getTypeDefinition( name );
  if ( pTypeDefinition == NULL )
  {
    pTypeDefinition = new TypeDefinition();
    pTypeDefinition->setName( name );
    pTypeDefinition->setIdentifier( identifier );
    pTypeDefinitions->addTypeDefinition( pTypeDefinition );
  }
  else
    pPropertyDefinition = NULL;

  if ( gmlId != "" )
    pTypeDefinition->setGmlId( gmlId );
  if ( supertypeRef != "" )
    pTypeDefinition->setSupertypeRef( supertypeRef );
  if ( description != "" )
    pTypeDefinition->setDescription( description );
  if ( classification != "" )
    pTypeDefinition->setClassification( classification );

  for ( i = 0; i < vPropertyDefinitions.size(); i++ )
    pTypeDefinition->addProperty( vPropertyDefinitions[i] );

  return pTypeDefinition;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen PropertyDefinition                                              //
///////////////////////////////////////////////////////////////////////////////
PropertyDefinition * DefinitionReader::readPropertyDefinition()
{
	String		            ^ STR;
	string			            stdString;
	PropertyDefinition    * pPropertyDefinition;
	ListedValueDefinition * pListedValueDefinition;

	pPropertyDefinition = new PropertyDefinition;

	STR = reader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pPropertyDefinition->setGmlId ( stdString );
	}
	
	while( reader->Read() )
	{
		if ( reader->NodeType == XmlNodeType::Element )
		{
			pActElement = reader->LocalName;

			if (  pActElement->CompareTo( "name" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addNamespaceKüerzel( stdString );
					pPropertyDefinition->setName ( stdString );
				}
			}
			else
			if (  pActElement->CompareTo( "identifier" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pPropertyDefinition->setIdentifier ( stdString );
				}
			}
			else
			if (  pActElement->CompareTo( "description" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pPropertyDefinition->setDescription ( stdString );
				}
			}
			else
			if (  pActElement->CompareTo( "cardinality" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pPropertyDefinition->setCardinality ( stdString );
				}
			}
			else
			if (  pActElement->CompareTo( "valueTypeName" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
          stdString = addNamespaceKüerzel( stdString );
					pPropertyDefinition->setValueTypeName ( stdString );
				}
			}
			else
			if (  pActElement->CompareTo( "valueTypeRef" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pPropertyDefinition->setValueTypeRef ( stdString );
				}
			}
			else
			if (  pActElement->CompareTo( "type" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pPropertyDefinition->setType ( stdString );
				}
			}
			else
			if (  pActElement->CompareTo( "ListedValueDefinition" ) == 0 )
			{
				pListedValueDefinition = readListedValueDefinition();
				if ( pListedValueDefinition != NULL )
					pPropertyDefinition->addListedValueDefinition ( pListedValueDefinition );
			}
		}
		else
		if ( reader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = reader->LocalName;
			if ( pActElement->CompareTo( "PropertyDefinition" ) == 0 ) break;
		}
	}

	return pPropertyDefinition;
}


///////////////////////////////////////////////////////////////////////////////
//  Einlesen ListedValueDefinition                                           //
///////////////////////////////////////////////////////////////////////////////
ListedValueDefinition * DefinitionReader::readListedValueDefinition()
{
	String		            ^ STR;
	string			            stdString;
	ListedValueDefinition * pListedValueDefinition;

	pListedValueDefinition = new ListedValueDefinition;

	STR = reader->GetAttribute ( "gml:id" );
	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pListedValueDefinition->setGmlId ( stdString );
	}
	
	while( reader->Read() )
	{
		if ( reader->NodeType == XmlNodeType::Element )
		{
			pActElement = reader->LocalName;

			if (  pActElement->CompareTo( "name" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pListedValueDefinition->setName ( stdString );
				}
			}
			else
			if (  pActElement->CompareTo( "identifier" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pListedValueDefinition->setIdentifier ( stdString );
				}
			}
			else
			if (  pActElement->CompareTo( "description" ) == 0 )
			{
				STR = reader->ReadString();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pListedValueDefinition->setDescription ( stdString );
				}
			}
		}
		else
		if ( reader->NodeType == XmlNodeType::EndElement )
		{
			pActElement = reader->LocalName;
			if ( pActElement->CompareTo( "ListedValueDefinition" ) == 0 ) break;
		}
	}

	return pListedValueDefinition;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt das Kürzel des zug. Namespaces hinzu                                //
///////////////////////////////////////////////////////////////////////////////
std::string DefinitionReader::addNamespaceKüerzel ( std::string name )
{
	size_t index;

  index = name.find ( ":" );
  if ( index != string::npos )
    return name;

  name = "xplan:" + name;

  return name;
}


///////////////////////////////////////////////////////////////////////////////
//  class TypeDefinitions                                                    //
//  Liste aller Typ-Dafinitionen von XPlanGML Features und DataTypes         //
///////////////////////////////////////////////////////////////////////////////
TypeDefinitions::TypeDefinitions()
{

}

TypeDefinitions::~TypeDefinitions()
{
	map<string,TypeDefinition*>::iterator iter;

	for ( iter = mTypeDefinitions.begin(); iter != mTypeDefinitions.end(); iter++ )
		delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine TypeDefinition hinzu                                           //
///////////////////////////////////////////////////////////////////////////////
void TypeDefinitions::addTypeDefinition ( TypeDefinition * pDef )
{
	string name       = pDef->getName();
  string identifier = pDef->getIdentifier();

	mTypeDefinitions[name]                 = pDef;
  mTypeIdentifierDefinitions[identifier] = pDef;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine TypeDefinition                                              //
///////////////////////////////////////////////////////////////////////////////
TypeDefinition * TypeDefinitions::getTypeDefinition ( std::string name )
{
	map<string,TypeDefinition*>::iterator iter;

	iter = mTypeDefinitions.find ( name );
	if ( iter != mTypeDefinitions.end() )
		return iter->second;
	else
		return NULL;
}

TypeDefinition * TypeDefinitions::getTypeDefinitionFromIdentifier ( std::string identifier )
{
  map<string,TypeDefinition*>::iterator iter;

  iter = mTypeIdentifierDefinitions.find ( identifier );
  if ( iter != mTypeIdentifierDefinitions.end() )
    return iter->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine PropertyDefinition von einer TypeDefinition                 //
///////////////////////////////////////////////////////////////////////////////
PropertyDefinition *  TypeDefinitions::getPropertyDefinition ( string klassenname, string propertyName )
{
	TypeDefinition * pTypeDefinition = getTypeDefinition ( klassenname );
	if ( pTypeDefinition == NULL ) return NULL;

	return pTypeDefinition->getPropertyDefinition ( propertyName );
}

///////////////////////////////////////////////////////////////////////////////
//  Kopiert die Property-Definitions der Oberklassen                         //
///////////////////////////////////////////////////////////////////////////////
void TypeDefinitions::analyzeSuperClasses()
{
	size_t               i, propAnz;
  bool                 weiter;
  string               identifier, identifierSuperClass;
  TypeDefinition     * pTypeDefinition;
  TypeDefinition     * pTypeDefinitionSuperClass;
  PropertyDefinition * pPropDefinition;
  PropertyDefinition * pPropDefinitionCopy;

  set<string> hasSuperClasses;
  map<std::string,TypeDefinition*>::iterator iter;
  vector<PropertyDefinition*> vProperties;

  for ( iter = mTypeDefinitions.begin(); iter != mTypeDefinitions.end(); iter++ )
  {
    pTypeDefinition = iter->second;
    identifier = pTypeDefinition->getIdentifier();
    hasSuperClasses.insert( identifier );

    weiter = true;
    identifierSuperClass = pTypeDefinition->getSupertypeRef();
    while ( weiter )
    {
      pTypeDefinitionSuperClass = getTypeDefinitionFromIdentifier(identifierSuperClass );
      if ( pTypeDefinitionSuperClass == NULL ) 
        weiter = false;
      else
      {
        vProperties.clear();
        propAnz = pTypeDefinitionSuperClass->getAllProperties( vProperties );
        for ( i = 0; i <  propAnz; i++ )
        {
          pPropDefinition = vProperties[i];
          pPropDefinitionCopy = new PropertyDefinition ( pPropDefinition );
          pTypeDefinition->addProperty ( pPropDefinitionCopy );
        }
        if ( hasSuperClasses.count( identifierSuperClass ) == 0 )
        {
          identifierSuperClass = pTypeDefinitionSuperClass->getSupertypeRef();
          if ( identifierSuperClass == "" ) weiter = false;
        }
        else
          weiter = false;
      }
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//  TypeDefinition                                                           //
//  Type Definition im Format ShapeChangeDefinitions.xsd                     //
///////////////////////////////////////////////////////////////////////////////
TypeDefinition::TypeDefinition()
{

}

TypeDefinition:: ~TypeDefinition()
{
	map<string,PropertyDefinition*>::iterator iter;
	for ( iter = mProperties.begin(); iter != mProperties.end(); iter++ )
		delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine PropertyDefinition hinzu                                       //
///////////////////////////////////////////////////////////////////////////////
void TypeDefinition::addProperty ( PropertyDefinition * pProp )
{
	string name = pProp->getName();
	mProperties[name] = pProp;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine PropertyDefinition                                          //
///////////////////////////////////////////////////////////////////////////////
PropertyDefinition * TypeDefinition::getPropertyDefinition ( std::string name )
{
	map<string,PropertyDefinition*>::iterator iter;

	iter = mProperties.find ( name );
	if ( iter != mProperties.end() )
		return iter->second;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle PropertyDefinition                                          //
///////////////////////////////////////////////////////////////////////////////
size_t TypeDefinition::getAllProperties ( std::vector<PropertyDefinition*> & vProperties )
{
  map<string,PropertyDefinition*>::iterator iter;

  for ( iter =  mProperties.begin(); iter != mProperties.end(); iter++ )
  {
    vProperties.push_back( iter->second );
  }
  return vProperties.size();
}

///////////////////////////////////////////////////////////////////////////////
//  PropertyDefinition                                                       //
//  ShapeChange Property Definition                                          //
///////////////////////////////////////////////////////////////////////////////
PropertyDefinition::PropertyDefinition()
{
}

PropertyDefinition::PropertyDefinition( PropertyDefinition * pPropDefOld )
{
  gmlId         = pPropDefOld->gmlId;
  name          = pPropDefOld->name;
  description   = pPropDefOld->description;
  identifier    = pPropDefOld->identifier;
  cardinality   = pPropDefOld->cardinality;
  valueTypeName = pPropDefOld->valueTypeName;
  valueTypeRef  = pPropDefOld->valueTypeRef;
  type          = pPropDefOld->type;

  for ( unsigned int i = 0; i < pPropDefOld->vListedValues.size(); i++ )
    vListedValues.push_back( new ListedValueDefinition ( pPropDefOld->vListedValues[i] ) );
}


PropertyDefinition:: ~PropertyDefinition()
{
  for ( unsigned int i = 0; i < vListedValues.size(); i++ )
    delete vListedValues[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert aus den ListedValueDefinitions die Einträge eines              //
//  GmlDictionaryEntry                                                       //
///////////////////////////////////////////////////////////////////////////////
void PropertyDefinition::addDictionaryItems ( GmlDictionaryEntry * pEntry )
{
	string schluesselNummer;
	string schluesselText;

	for ( unsigned int i = 0; i < vListedValues.size(); i++ )
	{
		ListedValueDefinition * pDef = vListedValues[i];
		schluesselNummer = pDef->getIdentifierShort();
		schluesselText   = pDef->getName();

    GmlDictionaryEntryDefinition entryDefinition;
    entryDefinition.name = schluesselNummer;
    entryDefinition.beschreibung = schluesselText;

		pEntry->addDefinition ( schluesselNummer, entryDefinition );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  ListedValueDefinition                                                    //
//  Item einer Enumeration oder CodeList                                     //
///////////////////////////////////////////////////////////////////////////////
ListedValueDefinition::ListedValueDefinition()
{

}

ListedValueDefinition::ListedValueDefinition( ListedValueDefinition * pDefOld )
{
  gmlId       = pDefOld->gmlId;
  name        = pDefOld->name;
  description = pDefOld->description;
  identifier  = pDefOld->identifier;
}


ListedValueDefinition:: ~ListedValueDefinition()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Extrahiert aus der URN des Identifiers die Schlüsselnummer               //
///////////////////////////////////////////////////////////////////////////////
string ListedValueDefinition::getIdentifierShort()
{
	string schluesselNummer;
	size_t index;

	index = identifier.rfind ( ':' );
	schluesselNummer = identifier.substr ( index+1 );
	return schluesselNummer;
}




