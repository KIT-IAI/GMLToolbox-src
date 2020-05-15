#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Windows::Forms;
using namespace System::Text;
using namespace System::Collections;

#include "includes.h"

class TypeDefinition;
class PropertyDefinition;
class ListedValueDefinition;
class GmlDictionaryEntry;

///////////////////////////////////////////////////////////////////////////////
//  class TypeDefinitions                                                    //
//  Liste aller Typ-Dafinitionen von XPlanGML Features und DataTypes         //
///////////////////////////////////////////////////////////////////////////////
class TypeDefinitions
{
	public:
		TypeDefinitions();
		virtual ~ TypeDefinitions();

	private:
		std::map<std::string,TypeDefinition*> mTypeDefinitions;   //  Zuordnung Typ-Name --> Type-Objekt
    std::map<std::string,TypeDefinition*> mTypeIdentifierDefinitions; //  Zuordnung Typ-Identifier --> Type-Objekt

	public:
		void addTypeDefinition ( TypeDefinition * pDef );
		TypeDefinition  * getTypeDefinition ( std::string name );
    TypeDefinition * TypeDefinitions::getTypeDefinitionFromIdentifier ( std::string identifier );

		PropertyDefinition * getPropertyDefinition ( std::string klassenname, 
													 std::string propertyName );

    void analyzeSuperClasses();
};

///////////////////////////////////////////////////////////////////////////////
//  TypeDefinition                                                           //
//  Type Definition im Format ShapeChangeDefinitions.xsd                     //
///////////////////////////////////////////////////////////////////////////////
class TypeDefinition
{
	public:
		TypeDefinition();
		virtual ~ TypeDefinition();

	private:
		std::string gmlId;
		std::string name;							        // GML Feature oder DataType Name
		std::string description;					    // Definition der Gml-Klasse
		std::string identifier;						    // URN-Form des Namens
		std::string classification;					  // featureType / dataType / objectType / unionType
		std::string supertypeRef;					    // Identifier des Oberklassen-Typs
		std::map<std::string,PropertyDefinition*> mProperties;  // Properties des Features od. DataTypes

	public:
		void setGmlId ( std::string id )  {  gmlId = id;  }
		std::string getGmlId ()  {  return gmlId;  }

		void setName ( std::string nameP )  {  name = nameP;  }
		std::string getName ()  {  return name;  }

		void setDescription ( std::string desc )  {  description = desc;  }
		std::string getDescription ()  {  return description;  }

		void setIdentifier ( std::string id )  {  identifier = id;  }
		std::string getIdentifier ()  {  return identifier;  }

		void setClassification ( std::string cl )  {  classification = cl;  }
		std::string getClassification ()  {  return classification;  }

		void setSupertypeRef ( std::string id )  {  supertypeRef = id;  }
		std::string getSupertypeRef ()  {  return supertypeRef;  }

		void addProperty ( PropertyDefinition * pProp );
		PropertyDefinition * getPropertyDefinition ( std::string name );

		size_t getAllProperties ( std::vector<PropertyDefinition*> & vProperties );
};

///////////////////////////////////////////////////////////////////////////////
//  PropertyDefinition                                                       //
//  ShapeChange Property Definition                                          //
///////////////////////////////////////////////////////////////////////////////
class PropertyDefinition
{
	public:
		PropertyDefinition();
    PropertyDefinition( PropertyDefinition * pPropDefOld );
		virtual ~ PropertyDefinition();

	private:
		std::string gmlId;                    //  GML-Id
		std::string name;									    // Property Name
		std::string description;							// Property Definition / Bescheibung
		std::string identifier;								// URN-Form des Namens
		std::string cardinality;              //  Kardinalität
		std::string valueTypeName;					  // Attribut-Typ bei Simple Datatypes
		std::string valueTypeRef;							// Identifier der ref. Klasse bei Relationen
		std::string type;									    //  attribute / assiciationRole
		std::vector<ListedValueDefinition*> vListedValues;  // Liste möglicher Attributwerte

	public:
		void setGmlId ( std::string id )  {  gmlId = id;  }
		std::string getGmlId ()  {  return gmlId;  }

		void setName ( std::string nameP )  {  name = nameP;  }
		std::string getName ()  {  return name;  }

		void setDescription ( std::string desc )  {  description = desc;  }
		std::string getDescription ()  {  return description;  }

		void setIdentifier ( std::string id )  {  identifier = id;  }
		std::string getIdentifier ()  {  return identifier;  }

		void setCardinality ( std::string car )  {  cardinality = car;  }
		std::string getCardinality ()  {  return cardinality;  }

		void setValueTypeName ( std::string name )  {  valueTypeName = name;  }
		std::string getValueTypeName ()  {  return valueTypeName;  }

		void setValueTypeRef ( std::string ref )  {  valueTypeRef = ref;  }
		std::string getValueTypeRef ()  {  return valueTypeRef;  }

		void setType ( std::string id )  {  type = id;  }
		std::string getType ()  {  return type;  }

		void addListedValueDefinition ( ListedValueDefinition * pDef )  {  vListedValues.push_back ( pDef );  }

		void addDictionaryItems ( GmlDictionaryEntry * pEntry );
};

///////////////////////////////////////////////////////////////////////////////
//  ListedValueDefinition                                                    //
//  Item einer Enumeration oder CodeList                                     //
///////////////////////////////////////////////////////////////////////////////
class ListedValueDefinition
{
	public:
		ListedValueDefinition();
    ListedValueDefinition( ListedValueDefinition * pDefOld );
		virtual ~ ListedValueDefinition();

	private:
		std::string gmlId;
		std::string name;			    // Item Langform aus UML-Diagramm
		std::string description;	// Definition der Item-Langform
		std::string identifier;		// URN-Form des Item Identifiers aus UML-Diagramm

	public:
		void setGmlId ( std::string id )  {  gmlId = id;  }
		std::string getGmlId ()  {  return gmlId;  }

		void setName ( std::string nameP )  {  name = nameP;  }
		std::string getName ()  {  return name;  }

		void setDescription ( std::string desc )  {  description = desc;  }
		std::string getDescription ()  {  return description;  }

		void setIdentifier ( std::string id )  {  identifier = id;  }
		std::string getIdentifier ()  {  return identifier;  }
		std::string getIdentifierShort();
};

///////////////////////////////////////////////////////////////////////////////
//  DefinitionReader                                                         //
//  XML-Reader zum Einlesen der Definitionen                                 //
///////////////////////////////////////////////////////////////////////////////
public ref  class DefinitionReader
{
	public:
		DefinitionReader ( TypeDefinitions * pTypeDefinitionsP );
		virtual ~DefinitionReader();

	public:
		bool read( std::string fileName );
    void closeReader();

	private:
		TypeDefinitions  * pTypeDefinitions;
		XmlTextReader    ^ reader;

		System::String ^ pActElement;
		System::String ^ pActAttrName;

	private:
		TypeDefinition        * readTypeDefinition();
		PropertyDefinition    * readPropertyDefinition();
		ListedValueDefinition * readListedValueDefinition();

    std::string addNamespaceKüerzel ( std::string name );

};






