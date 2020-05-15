#include "StdAfx.h"

#include <time.h>

#include "Convert.h"
#include "Dictionary.h"
#include "Definitions.h"
#include "GmlNamespaces.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "constant.h"
#include "Logfile.h"
#include "GmlSchema.h"

using namespace std;
using namespace System;
using namespace System::IO;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaTypes                                                           //
//  Modelliert die Collection aller globalen xs:simpleType und xs:complexType//
//  eines XML-Schemas												                            		 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaTypes::GmlSchemaTypes ()
{

}

GmlSchemaTypes::~GmlSchemaTypes()
{
	map<string,GmlSchemaType*>::iterator iter;

	for ( iter = types.begin(); iter != types.end(); iter++ )
		delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Typs													                           //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchemaTypes::addType ( GmlSchemaType * pTypeP )
{
	string name;
	name = pTypeP->getName();

  if ( types.find( name ) == types.end() )
  {
    types[name] = pTypeP;
    return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern eines Typs mit dem zug. Namen					                  				 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaType * GmlSchemaTypes::getType ( std::string typeNameP )
{
	map<string,GmlSchemaType*>::iterator iter;

	iter = types.find ( typeNameP );
	if ( iter != types.end() ) return iter->second;
	else					   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Simple Types							                        					 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlSchemaTypes::getSimpleTypes ( vector<GmlSchemaType*> & simpleTypesP )
{
	map<string,GmlSchemaType*>::iterator iter;

	for ( iter = types.begin(); iter != types.end(); iter++ )
	{
		GmlSchemaType * pType = iter->second;
		if ( pType->isSimpleType() ) 
    {
       simpleTypesP.push_back ( pType );
    }
	}

	return simpleTypesP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Complex Types						                      						 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlSchemaTypes::getComplexTypes ( vector<GmlSchemaComplexType*> & complexTypesP )
{
	map<string,GmlSchemaType*>::iterator iter;

	for ( iter = types.begin(); iter != types.end(); iter++ )
	{
		GmlSchemaType * pType = iter->second;
		if ( !pType->isSimpleType() ) complexTypesP.push_back ( (GmlSchemaComplexType*)pType );
	}

	return complexTypesP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Löst Referenzen auf, wenn innerhalb der GmlSchemaType auf globale        //
// Elements verwiesen wird                                                   //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaTypes::resolveReferences( GmlSchemaElements * pSchemaElements, GmlSchemaGroups *pSchemaGroups )
{
  GmlSchemaType           * pType;
  GmlSchemaComplexType    * pComplexType;
  vector<GmlSchemaElement*> vElements;
  vector<GmlSchemaElement*> vElementsNew;
  vector<GmlSchemaElement*> vGroupElements;
	size_t                    i, j, anz, groupElementAnz;
  GmlSchemaElement        * pElement;
  GmlSchemaElement        * pElementRef;
  GmlSchemaElement        * pGroupElement;
  GmlSchemaElement        * pGroupElementCopy;
  GmlSchemaGroup          * pGroupRef;
  string                    elementRefName;

  map<string,GmlSchemaType*>::iterator iter;

  for ( iter = types.begin(); iter != types.end(); iter++ )
  {
    pType = iter->second;
    if ( pType->getIsComplex () )
    {
      pComplexType = (GmlSchemaComplexType*)pType;
      vElements.clear();
      vElementsNew.clear();
      anz = pComplexType->getChildElements ( vElements );
      pComplexType->clearChildElements();
      for ( i = 0; i < anz; i++ )
      {
        pElement = vElements[i];
        elementRefName = pElement->getRefName();
        if ( elementRefName != "" )
        {
          if ( !pElement->isGroupRef() )
          {
            pElementRef = pSchemaElements->getElement ( elementRefName );
            if ( pElementRef != NULL )
            {
              pElement->setName ( pElementRef->getName() );
              pElement->setElementTypeName ( pElementRef->getElementTypeName() );
              pElement->setElementTypeNamespace ( pElementRef->getElementTypeNamespace() );
              pElement->setOptionalElement ( pElementRef->getOptionalElement() );
              pElement->setAbstractElement ( pElementRef->getAbstractElement() );
              pElement->setMultiElement ( pElementRef->getMultiElement() );
            }
            vElementsNew.push_back ( pElement );
          }
          else
          {
            delete pElement;
            pGroupRef = pSchemaGroups->getGroup ( elementRefName );
            if ( pGroupRef != NULL )
            {
              vGroupElements.clear();
              groupElementAnz = pGroupRef->getSchemaElements ( vGroupElements );
              for ( j = 0; j < groupElementAnz; j++ )
              {
                pGroupElement = vGroupElements[j];
                pGroupElementCopy = new GmlSchemaElement ( pGroupElement );
                vElementsNew.push_back ( pGroupElementCopy );
              }
            }
          }
        }
        else
          vElementsNew.push_back ( pElement );
      }
      pComplexType->setChildElements ( vElementsNew );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Modelliert die Collection aller globalen xs:element eines XML-Schemas		 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaElements::GmlSchemaElements ()
{

}

GmlSchemaElements::~GmlSchemaElements ()
{
	map<string, GmlSchemaElement*>::iterator iter;

	for ( iter = elements.begin(); iter != elements.end(); iter++ )
		delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Elements										                        		 //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchemaElements::addElement ( GmlSchemaElement * pElementP )
{
  map<string, GmlSchemaElement*>::iterator pos;
  string                                   name;

  name = pElementP->getName();
  pos  = elements.find ( name );
  if ( pos == elements.end() )
  {
    elements[name] = pElementP;
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Element mit einem best. Namen		                						 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaElement * GmlSchemaElements::getElement ( std::string elementNameP )
{
	map<string, GmlSchemaElement*>::iterator iter;

	iter = elements.find ( elementNameP );
	if ( iter != elements.end() ) return iter->second;
	else						              return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Elemente								                          					 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlSchemaElements::getElements ( vector<GmlSchemaElement*> & schemaElementsP )
{
	map<string, GmlSchemaElement*>::iterator iter;

	for ( iter = elements.begin(); iter != elements.end(); iter++ )
	{
		GmlSchemaElement * pSchemaElement = iter->second;
		schemaElementsP.push_back ( pSchemaElement );
	}

	return elements.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Elements zur Substitution Group												 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaElements::addSubstitutionGroup ( GmlSchemaElement * pElementP )
{
  string substitutionGroup = pElementP->getSubstitutionGroup();
  if ( substitutionGroup == "" ) return;

  substitutionElements.insert ( multimap<string,GmlSchemaElement*>::value_type ( substitutionGroup, pElementP ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Elemente einer Substitution Group                           //
///////////////////////////////////////////////////////////////////////////////
size_t GmlSchemaElements::getSubstitutionGroup ( string substitutionGroupNameP, vector<GmlSchemaElement*> &elements )
{
  multimap<string,GmlSchemaElement*>::iterator iter;
  GmlSchemaElement                           * pElement;

  for ( iter = substitutionElements.lower_bound ( substitutionGroupNameP );
    iter != substitutionElements.upper_bound ( substitutionGroupNameP );
    iter++ )
  {
    pElement = iter->second;
    elements.push_back ( pElement );
  }
  return elements.size();
}

////////////////////////////////////////////////////////////////////////////////
//  Löst Referenzen auf, wenn ein Element intern ein anderes Element referiert//
////////////////////////////////////////////////////////////////////////////////
void GmlSchemaElements::resolveReferences()
{
  GmlSchemaElement * pElement;
  GmlSchemaElement * pElementRef; 
  string             elementRefName;

  map<string, GmlSchemaElement*>::iterator iter;
  map<string, GmlSchemaElement*>::iterator pos;
  vector<GmlSchemaElement*>                vSubstitutionGroup;

  for ( iter = elements.begin(); iter != elements.end(); iter++ )
  {
    pElement = iter->second;
    elementRefName = pElement->getRefName();
    if ( elementRefName != "" )
    {
      pos = elements.find ( elementRefName );
      if ( pos != elements.end() )
      {
        pElementRef = pos->second;
        pElement->setName ( elementRefName );
        pElement->setElementTypeName ( pElementRef->getElementTypeName() );
        pElement->setElementTypeNamespace ( pElementRef->getElementTypeNamespace() );
        pElement->setOptionalElement ( pElementRef->getOptionalElement() );
        pElement->setAbstractElement ( pElementRef->getAbstractElement() );
        pElement->setMultiElement ( pElementRef->getMultiElement() );
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaGroups                                                          //
//  Modelliert die Collection aller xs:group Elemente                    		 //
//  Wird in GML Anwendungsschemata normalerweise nicht verwendet             //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaGroups::GmlSchemaGroups ()
{

}

GmlSchemaGroups::~GmlSchemaGroups()
{
  map<string, GmlSchemaGroup*>::iterator iter;

  for ( iter = groups.begin(); iter != groups.end(); iter++ )
    delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Gruppe hinzu                                                   //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaGroups::addGroup ( GmlSchemaGroup * pGroupP )
{
  string groupName;

  groupName = pGroupP->getName();
  groups[groupName] = pGroupP;

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Gruppe mit einem bestimmten Namen                           //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaGroup * GmlSchemaGroups::getGroup ( std::string groupNameP )
{
  map<string, GmlSchemaGroup*>::iterator iter;

  iter = groups.find ( groupNameP );
  if ( iter != groups.end() )
    return iter->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Löst Referenzen von GmlSchemaGroup Objekten auf                          //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaGroups::resolveReferences( GmlSchemaElements * pSchemaElements )
{
  map<string, GmlSchemaGroup*>::iterator iter;
  vector<GmlSchemaElement* >             elements;
  vector<GmlSchemaElement*>              elementsNew;
  GmlSchemaGroup                       * pGroup;
  GmlSchemaElement                     * pElement;
  GmlSchemaElement                     * pElementRef;
  string                                 groupRefName;
  string                                 elementRefName;
	size_t                                 i, elementAnz;
  bool                                   hasGroup;

  for ( iter = groups.begin(); iter != groups.end(); iter++ )
  {
    pGroup = iter->second;

    elements.clear();
    elementAnz = pGroup->getSchemaElements ( elements );
    for ( i = 0; i < elements.size(); i++ )
    {
      pElement = elements[i];
      if ( !pElement->isGroupRef() && pElement->getRefName() != "" && pElement->getName() == "" )
      {
        elementRefName = pElement->getRefName();
        pElementRef = pSchemaElements->getElement ( elementRefName );
        if ( pElementRef != NULL )
        {
          pElement->setName ( elementRefName );
          pElement->setElementTypeName ( pElementRef->getElementTypeName() );
          pElement->setElementTypeNamespace ( pElementRef->getElementTypeNamespace() );
          pElement->setOptionalElement ( pElementRef->getOptionalElement() );
          pElement->setAbstractElement ( pElementRef->getAbstractElement() );
          pElement->setMultiElement ( pElementRef->getMultiElement() );
        }
      }
    }
  }

  for ( iter = groups.begin(); iter != groups.end(); iter++ )
  {
    pGroup = iter->second;
    hasGroup = true;
    while ( hasGroup )
    {
      hasGroup = pGroup->resolveGroupReferences ( this );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaType                                                            //
//  Abstrakte Oberklasse von xs:simpleType und xs:complexType	          		 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaType::GmlSchemaType ( std::string nameP )
{
	typeName = nameP;
}

GmlSchemaType::GmlSchemaType ( GmlSchemaType * pTypeOld )
{
  typeName = pTypeOld->typeName;
  comment  = pTypeOld->comment;
  complex = pTypeOld->complex;
}

GmlSchemaType::~GmlSchemaType()
{

}

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaGroup                                                           //
//  Modelliert eine xs:group (kommt in GML-Schemata normalerweise nicht vor) //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaGroup::GmlSchemaGroup()
{

}

GmlSchemaGroup::GmlSchemaGroup ( GmlSchemaGroup * pGroupOld )
{
  unsigned int i;

  groupName = pGroupOld->groupName;

  for ( i = 0; i < pGroupOld->elements.size(); i++ )
    elements.push_back ( pGroupOld->elements[i] );
}

GmlSchemaGroup::~GmlSchemaGroup()
{
  unsigned int i;

  for ( i = 0; i < elements.size(); i++ )
    delete elements[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle GmlSchemaElement Objekte der Gruppe                         //
///////////////////////////////////////////////////////////////////////////////
size_t GmlSchemaGroup::getSchemaElements (	std::vector<GmlSchemaElement*> &elementsP )
{
  unsigned int i;

  for ( i = 0; i < elements.size(); i++ )
    elementsP.push_back (  elements[i] );

  return elementsP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Löst externe Referenzen der Gruppe auf                                   //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchemaGroup::resolveGroupReferences ( GmlSchemaGroups * pSchemaGroups )
{
  vector<GmlSchemaElement*>  elementsNew;
  vector<GmlSchemaElement*>  elementsRef;
  GmlSchemaElement         * pElement;
  GmlSchemaElement         * pElementCopy = nullptr;
  GmlSchemaGroup           * pGroupRef;
  string                     groupRefName;
	size_t                     i, j, anz;
  bool                       hasGroup = false;

  for ( i = 0; i < elements.size(); i++ )
  {
    pElement = elements[i];
    if ( !pElement->isGroupRef() )
      elementsNew.push_back ( pElement );
    else
    {
      elementsRef.clear();
      groupRefName = ((GmlSchemaGroupRef*)pElement)->getRefName();
      delete pElement;
      pGroupRef = pSchemaGroups->getGroup ( groupRefName );
      anz = pGroupRef->getSchemaElements ( elementsRef );
      for ( j = 0; j < anz; j++ )
      {
        pElement = elementsRef[j];
        if ( pElement->isGroupRef() == true )
        {
          pElementCopy = new GmlSchemaGroupRef ( (GmlSchemaGroupRef*)pElement );
          hasGroup = true;
        }
        else
          pElementCopy = new GmlSchemaElement ( pElementCopy );

        elementsNew.push_back ( pElementCopy );
      }
    }
  }

  elements.clear();
  for ( i = 0; i < elementsNew.size(); i++ )
    elements.push_back ( elementsNew[i] );

  return hasGroup;
}


///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaComplexType                                                     //
//  Modelliert globale komplexe Datentypen (xs:complexType)  des XML-Schemas //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaComplexType::GmlSchemaComplexType ( string nameP ):
GmlSchemaType ( nameP )
{
  isReference = false;
  complex     = true;
  isAbstract  = false;
}

GmlSchemaComplexType::GmlSchemaComplexType ( GmlSchemaComplexType * pTypeOld ):
GmlSchemaType ( pTypeOld )
{
  unsigned int i;

  typeNamespace       = pTypeOld->typeNamespace;
  parantTypeName      = pTypeOld->parantTypeName;
  parantTypeNamespace = pTypeOld->parantTypeNamespace;
  isReference         = pTypeOld->isReference;
  isAbstract          = pTypeOld->isAbstract;

  for ( i = 0; i < elements.size(); i++ )
  {
    GmlSchemaElement * pElement = new GmlSchemaElement ( pTypeOld->elements[i] );
    elements.push_back ( pElement );
  }

  for ( i = 0; i < attributes.size(); i++ )
  {
    GmlSchemaAttribut * pAttribute = new GmlSchemaAttribut ( pTypeOld->attributes[i] );
    attributes.push_back ( pAttribute );
  }
}

GmlSchemaComplexType::~GmlSchemaComplexType()
{
	unsigned int i;

	for ( i = 0; i < elements.size(); i++ )
	{
		delete elements[i];
	}

  for ( i = 0; i < attributes.size(); i++ )
  {
    delete attributes[i];
  }
}

///////////////////////////////////////////////////////////////////////////////
// Setzt den Datentyp der Oberklasse			                    							 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaComplexType::setParant ( std::string parantTypeP )
{
	size_t    index;

  parantTypeName = parantTypeP;

  index = parantTypeP.find ( ":" );
	if ( index != string::npos )
	{
		parantTypeNamespace = parantTypeP.substr ( 0, index );
	}
	else
	{
		parantTypeNamespace = "";
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Element hinzu												                        	 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaComplexType::addElement ( GmlSchemaElement* pElementP )
{
	elements.push_back ( pElementP );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Liste von Elementen an einer bestimmten Position des          //
//  Element-Vektors hinzu                                                    //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaComplexType::insertElements ( size_t index, std::vector<GmlSchemaElement*> vElements )
{
	size_t anzInsert = vElements.size();
	size_t anz       = elements.size();
	size_t k;
  GmlSchemaElement * pElement;
  GmlSchemaElement * pElementCopy;

  if ( index < 0 || index >= anz || anzInsert == 0 )
    return;

  elements.resize ( anz + anzInsert );

  for ( k = anz-1; k >= index+1; k-- )
    elements[k + anzInsert] = elements[k];

  for ( k = index + 1; k <= index + anzInsert; k++ )
  {
    pElement =  vElements[k-index-1];
    pElementCopy = new GmlSchemaElement ( pElement );
    elements[k] = pElementCopy;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Attribut hinzu							                        						 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaComplexType::addAttribut ( GmlSchemaAttribut * pAttributP )
{
  attributes.push_back ( pAttributP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Objekt des übergeordneten Typs		              						 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaComplexType * GmlSchemaComplexType::getParantType ( GmlSchemaTypes * pTypesP )
{
	GmlSchemaType * pType = pTypesP->getType ( parantTypeName );
	if ( pType != NULL )
	{
		if ( !pType->isSimpleType() ) return (GmlSchemaComplexType*)pType;
		else					                return NULL;
	}
	else return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Child-Element mit dem übergebenen Namen		          				 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaElement * GmlSchemaComplexType::getChildElement ( GmlSchemaTypes * pTypesP, string childElementNameP )
{
	unsigned int		       i;
	GmlSchemaElement     * pElement;
	GmlSchemaElement     * pChildElement;
	GmlSchemaComplexType * pParantType;

	for ( i = 0; i < elements.size(); i++ )
	{
		pElement = elements[i];
		pChildElement = pElement->getChildElement ( pTypesP, childElementNameP ); 
		if ( pChildElement != NULL ) return pChildElement;
	}

	pParantType = getParantType ( pTypesP );
	if ( pParantType != NULL ) pChildElement = pParantType->getChildElement ( pTypesP, childElementNameP );
	else pChildElement = NULL;

	return pChildElement;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Attribut-Objekt mit dem übergebenen Namen	            			 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaAttribut * GmlSchemaComplexType::getAttribut ( GmlSchemaTypes * pTypesP, string attributNameP )
{
  unsigned int				i;
  GmlSchemaAttribut * pAttribut;
  string              attributName;

  for ( i = 0; i < attributes.size(); i++ )
  {
    pAttribut = attributes[i];
    attributName = pAttribut->getAttributName();
    if ( attributNameP == attributName  ) return pAttribut;
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Child-Elemente							                      					 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlSchemaComplexType::getChildElements ( std::vector<GmlSchemaElement*> & childElementsP )
{
	unsigned int i;

	for ( i = 0; i < elements.size(); i++ )
	{
		GmlSchemaElement * pChildElement = elements[i];
		childElementsP.push_back ( pChildElement );
	}

	return elements.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt alle Child-Elemente						                      				  		 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaComplexType::setChildElements ( std::vector<GmlSchemaElement*>  childElementsP )
{
  unsigned int i;

  for ( i = 0; i < childElementsP.size(); i++ )
  {
    GmlSchemaElement * pChildElement = childElementsP[i];
    elements.push_back ( pChildElement );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Child-Element an Position index                                  //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaElement * GmlSchemaComplexType::getChildElement ( int index )
{
  if ( index < 0 || index >= (int)elements.size() ) return NULL;
  else                                         return elements[index];
}


///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Child-Elemente					                     				  		 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaComplexType::clearChildElements()
{
  elements.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Attribute                          												 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlSchemaComplexType::getAttributes ( std::vector<GmlSchemaAttribut*> & attributesP )
{
  unsigned int i;

  for ( i = 0; i < attributes.size(); i++ )
  {
    GmlSchemaAttribut * pAttribut = attributes[i];
    attributesP.push_back ( pAttribut );
  }

  return attributes.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Modelliert einfache Datentypen (xs:simpleType) des XML-Schemas     			 //
//  Im Augenblick: Nur Enumerationen von Strings					              		 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaSimpleType::GmlSchemaSimpleType  (  std::string nameP ):
GmlSchemaType ( nameP )
{
  complex = false;

}

GmlSchemaSimpleType::~GmlSchemaSimpleType()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Enumerations-Item hinzu										                      	 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaSimpleType::addEnumerationItem ( string itemP )
{
	enumerationItems.push_back ( itemP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Enumerations-Items								                    			 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlSchemaSimpleType::getEnumerationItems ( vector<string> & enumerationItemsP )
{
	unsigned int i;
	string       item;

	for ( i = 0; i < enumerationItems.size(); i++ )
	{
		item = enumerationItems[i];
		enumerationItemsP.push_back ( item );
	}

	return enumerationItems.size();
}


//////////////////////////////////////////////////////////////////////////////
//  GmlSchemaElement                                                        //
//  Modelliert globale und lokale Elemente (xs:element) des XML-Schemas     //
//////////////////////////////////////////////////////////////////////////////
GmlSchemaElement::GmlSchemaElement ()
{
	pUnnamedType = NULL;

	optionalElement = false;
	multiElement    = false;
	abstractElement = false;
  nillable        = false;
}

GmlSchemaElement::GmlSchemaElement ( GmlSchemaElement * pElementOld )
{
  elementName          = pElementOld->elementName;
  refName              = pElementOld->refName;
  substitutionGroup    = pElementOld->substitutionGroup;
  elementTypeName      = pElementOld->elementTypeName;
  elementTypeNamespace = pElementOld->elementTypeNamespace;
  defaultValue         = pElementOld->defaultValue;
  optionalElement      = pElementOld->optionalElement;
  multiElement         = pElementOld->multiElement;
  abstractElement      = pElementOld->abstractElement;
  externalCodeListName = pElementOld->externalCodeListName;
  nillable             = pElementOld->nillable;
  referenziertesElement = pElementOld->referenziertesElement;

  if ( pElementOld->pUnnamedType != NULL )
    pUnnamedType = new GmlSchemaComplexType ( pElementOld->pUnnamedType );
  else
    pUnnamedType = NULL;
}

GmlSchemaElement::~GmlSchemaElement()
{
	if ( pUnnamedType != NULL ) delete pUnnamedType;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Typ des Elements und extrahiert das Namespace-Kürzel    			 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaElement::setType ( std::string typeP )
{
	size_t    index;

  elementTypeName      = typeP;

  index = typeP.find ( ":" );
  if ( index != string::npos )
    elementTypeNamespace = typeP.substr ( 0, index );
  else
    elementTypeNamespace = "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Objekt GmlSchemaComplexType, das zum elementTypeName gehört	 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaType * GmlSchemaElement::getElementTypeObject ( GmlSchemaTypes * pTypesP )
{
	GmlSchemaType * pType = pTypesP->getType ( elementTypeName );
	return pType;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert, ob das Element einen komplexen Datentyp hat                     //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchemaElement::isComplexType ( GmlSchemaTypes * pTypesP )
{
  GmlSchemaType * pType = getElementTypeObject ( pTypesP );
  if ( pType != NULL ) return pType->getIsComplex();
  else return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Liefert Child-Element mit dem übergebenen Namen			        		 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaElement * GmlSchemaElement::getChildElement ( GmlSchemaTypes * pTypesP, 
				string childElementNameP )
{
	GmlSchemaElement * pChildElement = NULL;

	if ( childElementNameP == elementName ) return this;

	if ( elementTypeName != "" )
	{
		GmlSchemaType * pType = pTypesP->getType ( elementTypeName );
		if ( pType != NULL && !pType->isSimpleType() ) 
			pChildElement = ((GmlSchemaComplexType*)pType)->getChildElement (  pTypesP, childElementNameP );
	}

	if ( pChildElement == NULL && pUnnamedType != NULL )
		pChildElement = pUnnamedType->getChildElement(  pTypesP, childElementNameP );

	return pChildElement;
}

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaGroupRef                                                        //
//  Modelliert eine Referenz auf eine xs:group	  				                   //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaGroupRef::GmlSchemaGroupRef (): GmlSchemaElement()
{

}

GmlSchemaGroupRef::GmlSchemaGroupRef ( GmlSchemaGroupRef * pRefOld ):
GmlSchemaElement ( pRefOld )
{

}

GmlSchemaGroupRef::~GmlSchemaGroupRef()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Modelliert ein Attribut (xs:attribute) oder eines Attribut-Gruppe        //
//  eines xs:complexType                                                     //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaAttribut::GmlSchemaAttribut ()
{
  isOptional = true;
  isGroup    = false;
}

GmlSchemaAttribut::GmlSchemaAttribut ( GmlSchemaAttribut * pAttributP )
{
  attributName          = pAttributP->attributName;
  refName               = pAttributP->refName;
  attributTypName       = pAttributP->attributTypName;
  attributTypNamespace  = pAttributP->attributTypNamespace;
  defaultValue          = pAttributP->defaultValue;
  isGroup               = pAttributP->isGroup;
  isOptional            = pAttributP->isOptional;
}


GmlSchemaAttribut::~GmlSchemaAttribut()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Typ des Attributs und extrahiert das Namespace-Kürzel    			 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaAttribut::setAttributType ( std::string typeP )
{
	size_t    index;

  attributTypName = typeP;

  index = typeP.find ( ":" );
  if ( index != string::npos )
    attributTypNamespace = typeP.substr ( 0, index );
  else
    attributTypNamespace = "";
}

///////////////////////////////////////////////////////////////////////////////
//  Ausgabe der Daten des Attributs                                   			 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaAttribut::write ( System::IO::StreamWriter ^ writer )
{
  String ^STR;

  writer->WriteLine();

  writer->Write ( "SchemaAttribut: " );
  STR = QuConvert::ToString ( attributName );
  writer->WriteLine ( STR );

  writer->Write ( "Typ: " );
  STR = QuConvert::ToString ( attributTypName );
  writer->WriteLine ( STR );

  writer->Write ( "Typ-Namespace: " );
  STR = QuConvert::ToString ( attributTypNamespace );
  writer->WriteLine ( STR );

  writer->Write ( "Default: " );
  STR = QuConvert::ToString ( defaultValue );
  writer->WriteLine ( STR );

  writer->Write ( "Optional:" );
  writer->WriteLine ( isOptional );

  writer->Write ( "IsGroup:" );
  writer->WriteLine ( isGroup );

  writer->WriteLine();
}

///////////////////////////////////////////////////////////////////////////////
//  Modelliert eine ein oder mehrere GML-Schemata    	                       //
///////////////////////////////////////////////////////////////////////////////
GmlSchema::GmlSchema(  Features * pFeatures )
{
  m_pFeatures = pFeatures;

  gmlTyp                   = GML_SCHEMA_UNKNOWN;
  pGmlNamespaces           = NULL;
  pGmlNamespacesStandard   = NULL;
  pSchemaTypeNamesStandard = NULL;

  pCodeLists         = NULL;
  pTypeDefinitions   = NULL;
}

GmlSchema::~GmlSchema()
{
    map<string,GmlElement*>::iterator     iterElements;
	map<string,GmlKlasse*>::iterator      iterKlassen;
	map<string,GmlEnumeration*>::iterator iterEnumerationen;

  for ( iterElements = gmlElements.begin(); iterElements != gmlElements.end(); iterElements++ )
    delete iterElements->second;

  for ( iterKlassen = gmlKlassen.begin(); iterKlassen != gmlKlassen.end(); iterKlassen++ )
    delete iterKlassen->second;

	for ( iterEnumerationen = gmlEnumerationen.begin(); iterEnumerationen != gmlEnumerationen.end();		  iterEnumerationen++ )
	  delete iterEnumerationen->second;

  if ( pGmlNamespaces != NULL ) delete pGmlNamespaces;
  if ( pSchemaTypeNamesStandard != NULL ) delete pSchemaTypeNamesStandard;

  if ( pCodeLists != NULL ) delete pCodeLists;
  if ( pTypeDefinitions != NULL ) delete pTypeDefinitions;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert true, wenn es sich um ein CityGML Modell handelt                 //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchema::isCityGml()
{
  if ( gmlTyp ==  CITYGML_0_4_0 ||  gmlTyp == CITYGML_1_0 || gmlTyp == CITYGML_2_0 || gmlTyp == CITYGML_3_0 )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert true, wenn es sich um ein XPlanGML Modell handelt                //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchema::isXPlanGml()
{
  if ( gmlTyp == XPLANGML_2_0 || gmlTyp == XPLANGML_3_0 || gmlTyp == XPLANGML_4_0 || gmlTyp == XPLANGML_4_1 ||
       gmlTyp == XPLANGML_5_0 || gmlTyp == XPLANGML_5_1 || gmlTyp == XPLANGML_5_2 )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert true, wenn es sich um ein Alkis Modell handelt                //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchema::isAlkis()
{
  if ( gmlTyp == NAS_5_1 || gmlTyp == NAS_6_0 )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert true, wenn es sich um ein INSPIRE PLU Modell handelt             //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchema::isInspirePlu()
{
  if ( gmlTyp == INSPIRE_LU_PLANNED )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert true, wenn es sich um ein INSPIRE Modell handelt                 //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchema::isInspire()
{
  if ( gmlTyp == INSPIRE  )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert true, wenn es sich um ein BoreholeML Modell handelt              //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchema::isBoreholeML()
{
  if ( gmlTyp == BOREHOLEML_3_0 )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Objekt GmlKlasse hinzu												                       //
///////////////////////////////////////////////////////////////////////////////
void GmlSchema::addGmlKlasse ( GmlKlasse* pGmlKlasseP )
{
	string klassenName = pGmlKlasseP->getGmlKlassenName();

	gmlKlassen[klassenName] = pGmlKlasseP;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Objekt GmlElement hinzu		                     										 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchema::addGmlElement ( GmlElement * pGmlElementP )
{
  map<string,GmlElement*>::iterator pos;
  string                            elementName = pGmlElementP->getGmlElementName();

  pos = gmlElements.find ( elementName );
  if ( pos !=  gmlElements.end() )
    delete pos->second;

  gmlElements[elementName] = pGmlElementP;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Objekte GmlKlasse							                          	 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlSchema::getGmlKlassen ( vector<GmlKlasse*> &gmlKlassenP )
{
	map<string,GmlKlasse*>::iterator iter;

	for ( iter = gmlKlassen.begin(); iter != gmlKlassen.end(); iter++ )
	{
		GmlKlasse * pGmlKlasse = iter->second;
		gmlKlassenP.push_back ( pGmlKlasse );
	}
	return gmlKlassenP.size();
}
///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML                                                             //
//  Stellt fest, ob eine bestimmte GML-Klasse zum übergebenen planTyp gehört //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchema::classOccursInPlan ( int planTyp, GmlKlasse * pGmlKlasse )
{
	int  planTypLoc = pGmlKlasse->getPlanTyp();
	bool occur = false;

	switch ( planTyp )
	{
	case BPLAN:
		if ( planTypLoc == BPLAN ) occur = true;
		break;

	case FPLAN:
		if ( planTypLoc == FPLAN ) occur = true;
		break;

	case RPLAN:
		if ( planTypLoc == RPLAN ) occur = true;
		break;

	case LPLAN:
		if ( planTypLoc == LPLAN ) occur = true;
		break;

  case SOPLAN:
    if ( planTypLoc == SOPLAN ) occur = true;
    break;

	case KEIN_XPLAN:
		occur = true;
		break;
	}
	return occur;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Element zum Namen		                        										 //
///////////////////////////////////////////////////////////////////////////////
GmlElement * GmlSchema::getGmlElement ( std::string gmlElementNameP )
{
  map<string,GmlElement*>::iterator iter;

  iter = gmlElements.find ( gmlElementNameP );
  if ( iter != gmlElements.end() )
    return iter->second;
  else
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML                                                             //
//  Liefert GmlKlasse Objekte (alle oder planTyp-bezogen), die Punktgeometrie//
//  haben können                                                           	 //
///////////////////////////////////////////////////////////////////////////////
size_t  GmlSchema::getGmlPunktObjekte ( vector<GmlKlasse*> &gmlPunktObjekteP, int planTyp, bool all )
{
	map<string,GmlKlasse*>::iterator iter;

	for ( iter = gmlKlassen.begin(); iter != gmlKlassen.end(); iter++ )
	{
		GmlKlasse * pGmlKlasse = iter->second;

		if ( pGmlKlasse->getGmlObjektTyp () == XP_PUNKT_OBJEKT ||
		     pGmlKlasse->getGmlObjektTyp () == XP_GEOMETRIE_OBJEKT ||
		     pGmlKlasse->getGmlObjektTyp () == XP_PTO ||
		     pGmlKlasse->getGmlObjektTyp () == XP_PPO  )
		{
      if ( all || classOccursInPlan ( planTyp, pGmlKlasse ) || 
        pGmlKlasse->getGmlObjektTyp () == XP_PTO || pGmlKlasse->getGmlObjektTyp () == XP_PPO  )
				gmlPunktObjekteP.push_back ( pGmlKlasse );
		}
	}
	return gmlPunktObjekteP.size();
}

////////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML                                                              //
//  Liefert GmlKlasse Objekte (alle oder planTyp-bezogen), die Liniengeometrie//
//  haben können                                                            	//
////////////////////////////////////////////////////////////////////////////////
size_t GmlSchema::getGmlLinienObjekte ( vector<GmlKlasse*> &gmlLinienObjekteP, int planTyp, bool all )
{
	map<string,GmlKlasse*>::iterator iter;

	for ( iter = gmlKlassen.begin(); iter != gmlKlassen.end(); iter++ )
	{
		GmlKlasse * pGmlKlasse = iter->second;

		if ( pGmlKlasse->getGmlObjektTyp () == XP_LINIEN_OBJEKT ||
		     pGmlKlasse->getGmlObjektTyp () == XP_GEOMETRIE_OBJEKT ||
		     pGmlKlasse->getGmlObjektTyp () == XP_LTO ||
		     pGmlKlasse->getGmlObjektTyp () == XP_LPO  )
		{
			if ( all || classOccursInPlan ( planTyp, pGmlKlasse ) || 
           pGmlKlasse->getGmlObjektTyp () == XP_LTO || pGmlKlasse->getGmlObjektTyp () == XP_LPO )
				gmlLinienObjekteP.push_back ( pGmlKlasse );
		}
	}

	return gmlLinienObjekteP.size();
}

/////////////////////////////////////////////////////////////////////////////////
//  Nur XPlanGML                                                               //
//  Liefert GmlKlasse Objekte (alle oder planTyp-bezogen), die Flächengeometrie//
//  haben können                                                             	 //
/////////////////////////////////////////////////////////////////////////////////
size_t GmlSchema::getGmlFlaechenObjekte ( vector<GmlKlasse*> &gmlFlaechenObjekteP, int planTyp, bool all )
{
	map<string,GmlKlasse*>::iterator iter;

	for ( iter = gmlKlassen.begin(); iter != gmlKlassen.end(); iter++ )
	{
		GmlKlasse * pGmlKlasse = iter->second;


		if ( pGmlKlasse->getGmlObjektTyp () == XP_FLAECHEN_OBJEKT    ||
		     pGmlKlasse->getGmlObjektTyp () == XP_GEOMETRIE_OBJEKT   ||
		     pGmlKlasse->getGmlObjektTyp () == XP_FPO                ||
			   pGmlKlasse->getGmlObjektTyp () == RASTER_BASIS     ||
			   pGmlKlasse->getGmlObjektTyp () == RASTER_AENDERUNG ||
			   pGmlKlasse->getGmlObjektTyp () == BLEIT_PLAN )
		{
			if (  all || classOccursInPlan ( planTyp, pGmlKlasse ) || 
        pGmlKlasse->getGmlObjektTyp () == XP_FPO || pGmlKlasse->getGmlObjektTyp () == RASTER_BASIS || pGmlKlasse->getGmlObjektTyp () == RASTER_AENDERUNG )
				gmlFlaechenObjekteP.push_back ( pGmlKlasse );
		}
	}

	return gmlFlaechenObjekteP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert GmlKlasse Objekt zum xs:complexType Namen   										 //
///////////////////////////////////////////////////////////////////////////////
GmlKlasse * GmlSchema::getGmlKlasse ( std::string gmlKlassenNameP )
{
	map<string,GmlKlasse*>::iterator iter;

	iter = gmlKlassen.find ( gmlKlassenNameP );
	if ( iter != gmlKlassen.end() ) return iter->second;
	else							return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert GmlKlasse Objekt zum xs:element Namen      											 //
///////////////////////////////////////////////////////////////////////////////
GmlKlasse * GmlSchema::getGmlKlasseFromElementName ( std::string gmlElementName )
{
  GmlElement * pGmlElement = getGmlElement( gmlElementName );
  if ( pGmlElement != NULL )
    return getGmlKlasse( pGmlElement->getGmlTypeName() );
  else
  {
    if ( gmlElementName == "core:externalReference" )
      return  getGmlKlasse ( "core:ExternalReferenceType" );
    else
    if ( gmlElementName == "core:externalObject" )
      return  getGmlKlasse ( "core:ExternalObjectReferenceType" );
    else
      return NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Enumeration zum Namen										                      	 //
///////////////////////////////////////////////////////////////////////////////
GmlEnumeration * GmlSchema::getGmlEnumeration ( std::string gmlEnumerationNameP )
{
	map<string,GmlEnumeration*>::iterator iter;

	iter = gmlEnumerationen.find ( gmlEnumerationNameP );
	if ( iter != gmlEnumerationen.end() ) return iter->second;
	else								  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen Enumerations-Namen zum Klassen-Namen                  		 //
///////////////////////////////////////////////////////////////////////////////
string GmlSchema::getGmlEnumerationsName ( string gmlKlassenNameP, string gmlAttributNameP )
{
	GmlKlasse * pGmlKlasse = getGmlKlasseFromElementName ( gmlKlassenNameP );
	if ( pGmlKlasse == NULL ) return "";

	GmlAttribut * pGmlAttribut = pGmlKlasse->getGmlAttribut ( gmlAttributNameP );
	if ( pGmlAttribut == NULL ) return "";

	return pGmlAttribut->getGmlEnumeration();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den GML-Typ als String                                           //
///////////////////////////////////////////////////////////////////////////////
string GmlSchema::getGmlTypAsString() 
{
  return Constant::getGMLSchemaTypeAsString( gmlTyp );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den GML Attribut Typ als String                                  //
///////////////////////////////////////////////////////////////////////////////
std::string GmlSchema::gmlAttributTypToString( ATTRIBUT_TYP typ )
{
  switch ( typ )
  {  
  case _ENUMERATION:               return string ( "Enumeration" );
  case _EXTERNAL_CODE_LIST:        return string ( "CodeList" );
  case _EXTERNAL_CODE_LIST_GML3_3: return string ( "CodeList " );
  case _REFERENZ:                  return string ( "Referenz" );
  case _DATA_TYPE:                 return string ( "DataType" );
  case _ANY_URI:                   return string ( "URL" );
  case _STRING:                    return string ( "Text" );
  case _DOUBLE:                    return string ( "Double" );
  case _INTEGER:                   return string ( "Integer" );
  case _BOOLEAN:                   return string ( "Boolean" );
  case _DATE:                      return string ( "Datum" );
  case _MEASURE_LISTE:             return string ( "Werteliste" );
  case _QUANTITY_EXTENT:           return string ( "Wertebereich" );
  case _DATE_TIME:                 return string ( "Datum und Uhrzeit" );
  case TIME_POSITION:              return string ( "Zeitpunkt" );
  case TIME_PERIOD:                return string ( "Zeitraum" );
  case TIME_INTERVAL_LENGTH:       return string ( "Zeitdauer" );
  case REGULAR_TIME_SERIES:        return string ( "Reguläre Zeitreihe" );
  case IRREGULAR_TIME_SERIES:      return string ( "Irreguläre Zeitreihe" );
  case REGULAR_TIME_SERIES_FILE:   return string ("Filebasierte reguläre Zeitreihe" );
  case IRREGULAR_TIME_SERIES_FILE: return string ( "Filebasierte irreguläre Zeitreihe" );
  case TIME_SERIES:                return string ( "Zeitreihe" );
  case _GEOMETRIE:                 return string ( "Geometrie" );
  case _ANGLE:                     return string ( "Winkel" );
  case _LENGTH:                    return string ( "Länge" );
  case _AREA:                      return string ( "Fläche" );
  case _VOLUME:                    return string ( "Volumen" );
  case _MEASURE:                   return string ( "Messwert" );
  case _POS_X:                     return string ( "X-Koordinate" );
  case _POS_Y:                     return string ( "Y-Koordinate" );
  case _POS_XY:                    return string ( "X- und Y-Koordinate" );
  case _DOMAIN_SET:                return string ( "Coverage Domain Set" );
  case _RANGE_SET:                 return string ( "Coverage Range Set" );
  case _RANGE_TYPE:                return string ( "Coverage Range Type" );
  case _CVGVP_DOMAIN_EXTENT:       return string ( "Coverage Domain Extent" );
  case _CVGVP_ELEMENT:             return string ( "Coverage Element" );
  case _GENER_STRING:              return string ( "String Attribut" );
  case _GENER_DATE:                return string ( "Datum Attribut" );
  case _GENER_URL:                 return string ( "URL Attribut" );
  case _GENER_INTEGER:             return string ( "Integer Attribut" );
  case _GENER_DOUBLE:              return string ( "Double Attribut" );
  case _GENER_MEASURE:             return string ( "Measure Attribut" );
  case _GENER_LIST:                return string ( "Liste Gener. Attribute" );
  case _SONST:                     return string ( "Unbekannt" );
  };
  return "Unbekannt";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den GML Attribut Typ zu einem String                             //
///////////////////////////////////////////////////////////////////////////////
ATTRIBUT_TYP GmlSchema::gmlStringToAttributTyp ( std::string typString )
{
  if ( typString == "Enumeration" )              return _ENUMERATION;
  if ( typString == "CodeList" )                 return _EXTERNAL_CODE_LIST;
  if ( typString == "CodeList " )                return _EXTERNAL_CODE_LIST_GML3_3;
  if ( typString == "Referenz" )                 return _REFERENZ;
  if ( typString == "DataType" )                 return _DATA_TYPE;
  if ( typString == "URL" )                      return _ANY_URI;
  if ( typString == "Text" )                     return _STRING;
  if ( typString == "Double" )                   return _DOUBLE;
  if ( typString == "Integer" )                  return _INTEGER;
  if ( typString == "Boolean" )                  return _BOOLEAN;
  if ( typString == "Datum" )                    return _DATE;
  if ( typString == "Datum und Uhrzeit" )        return _DATE_TIME;
  if ( typString == "Geometrie" )                return _GEOMETRIE;
  if ( typString == "Winkel" )                   return _ANGLE;
  if ( typString == "Länge" )                    return _LENGTH;
  if ( typString == "Fläche" )                   return _AREA;
  if ( typString == "Volumen" )                  return _VOLUME;
  if ( typString == "X-Koordinate" )             return _POS_X;
  if ( typString == "Y-Koordinate" )             return _POS_Y;
  if ( typString == "X- und Y-Koordinate" )      return _POS_XY;
  if ( typString == "Coverage Domain Set" )      return _DOMAIN_SET;
  if ( typString == "Coverage Range Set" )       return _RANGE_SET;
  if ( typString == "Coverage Range Type" )      return _RANGE_TYPE;
  if ( typString == "Coverage Domain Extent" )   return _CVGVP_DOMAIN_EXTENT;
  if ( typString ==  "Coverage Element" )        return _CVGVP_ELEMENT;
  if ( typString == "String Attribut" )          return _GENER_STRING;
  if ( typString == "Datum Attribut" )           return _GENER_DATE;
  if ( typString == "URL Attribut" )             return _GENER_URL;
  if ( typString == "Integer Attribut" )         return _GENER_INTEGER;
  if ( typString == "Double Attribut" )          return _GENER_DOUBLE;
  if ( typString == "Measure Attribut" )         return _GENER_MEASURE;

  return _SONST;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Version des Bassisstandards GML                              //
///////////////////////////////////////////////////////////////////////////////
GML_VERSION GmlSchema::getGmlVersion()
{
  return getGmlVersion ( gmlTyp );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die GML-Version für verschiedenen Applikations-Schemata          //
///////////////////////////////////////////////////////////////////////////////
GML_VERSION GmlSchema::getGmlVersion( GML_SCHEMA_TYPE gmlTypP )
{
  switch ( gmlTypP )
  {
  case  XPLANGML_2_0:        return GML_3_1;
  case  XPLANGML_3_0:        return GML_3_1;
  case  XPLANGML_4_0:        return GML_3_2;
  case  XPLANGML_4_1:        return GML_3_2;
  case  XPLANGML_5_0:        return GML_3_2;
  case  XPLANGML_5_1:        return GML_3_2;
  case  XPLANGML_5_2:        return GML_3_2;
  case  INSPIRE_LU_PLANNED:  return GML_3_2;
  case  INSPIRE:             return GML_3_2;
  case  NAS_5_1:             return GML_3_1;
  case  NAS_6_0:             return GML_3_2;
  case  CITYGML_0_4_0:       return GML_3_1;
  case  CITYGML_1_0:         return GML_3_1;
  case  CITYGML_2_0:         return GML_3_1;
  case  CITYGML_3_0:         return GML_3_2;
  case BOREHOLEML_3_0:       return GML_3_2;
  default:                   return GML_VERSION_UNKNOWN;
  }

  return GML_VERSION_UNKNOWN;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Dictionaries der CodeLists und ExternalCodeLists bzw.        //
//  legt sie an.                                                             //
///////////////////////////////////////////////////////////////////////////////
GmlDictionary * GmlSchema::getCodeListsDictionary ()  
{  
  if ( pCodeLists == NULL )
    pCodeLists = new GmlDictionary ();

  return pCodeLists;  
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Ordner mit den Standard ExternalCodeLists                    //
///////////////////////////////////////////////////////////////////////////////
std::string GmlSchema::getDefaultCodeListsFolder()
{
  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt die Dictionaries für Enumerationen und CodeLists  	             //
///////////////////////////////////////////////////////////////////////////////
void GmlSchema::generateEnumerationsAndCodeLists()
{
  vector<GmlKlasse*>   vGmlKlassen;
  vector<GmlAttribut*> vGmlProperties;

  GmlKlasse          * pGmlKlasse;
  GmlAttribut	       * pGmlProperty;
  GmlDictionaryEntry * pGmlDictionaryEntry;
  string		           elementName;
  string		           propertyName;
  string		           valueTypeName;
	size_t			         i, j, klassenAnz, propertyAnz;

  klassenAnz = getGmlKlassen( vGmlKlassen );
  for ( i = 0; i < klassenAnz; i++ )
  {
    pGmlKlasse = vGmlKlassen[i];
    elementName = pGmlKlasse->getGmlElementName();

    vGmlProperties.clear();
    propertyAnz = pGmlKlasse->getGmlAttribute ( vGmlProperties );
    for ( j = 0; j < propertyAnz; j++ )
    {
      pGmlProperty = vGmlProperties[j];
      propertyName = pGmlProperty->getGmlAttributName();

      if ( pGmlProperty->getGmlAttributTyp() == _ENUMERATION )
      {
        PropertyDefinition * pPropertyDefinition = 
          pTypeDefinitions->getPropertyDefinition ( elementName, propertyName );
        if ( pPropertyDefinition != NULL )
        {
          valueTypeName = pPropertyDefinition->getValueTypeName();
          if ( pCodeLists->getDictionaryEntry ( valueTypeName ) == NULL )
          {
            pGmlDictionaryEntry = new GmlDictionaryEntry;
            pGmlDictionaryEntry->setName ( valueTypeName );
            pGmlDictionaryEntry->setId ( pPropertyDefinition->getGmlId() );
						pGmlDictionaryEntry->setIdentifier ( pPropertyDefinition->getGmlId() );
						pGmlDictionaryEntry->setIdentifierCodespace ( pCodeLists->getIdentifierCodespace() );
						pGmlDictionaryEntry->setBeschreibung ( pPropertyDefinition->getDescription() );
            pPropertyDefinition->addDictionaryItems ( pGmlDictionaryEntry );
            pCodeLists->addDictionaryEntry ( pGmlDictionaryEntry );
          }
        }
      }

      if ( pGmlProperty->getGmlAttributTyp() == _EXTERNAL_CODE_LIST )
      {
        PropertyDefinition * pPropertyDefinition = 
          pTypeDefinitions->getPropertyDefinition ( elementName, propertyName );
        if ( pPropertyDefinition != NULL )
        {
          valueTypeName = pPropertyDefinition->getValueTypeName();
          pGmlProperty->setGmlEnumeration ( valueTypeName );
          if ( pCodeLists->getDictionaryEntry ( valueTypeName ) == NULL )
          {
            pGmlDictionaryEntry = new GmlDictionaryEntry;
            pGmlDictionaryEntry->setName ( valueTypeName );
						pGmlDictionaryEntry->setId ( pPropertyDefinition->getGmlId() );
						pGmlDictionaryEntry->setIdentifier ( pPropertyDefinition->getGmlId() );
						pGmlDictionaryEntry->setIdentifierCodespace ( pCodeLists->getIdentifierCodespace() );
						pGmlDictionaryEntry->setBeschreibung ( pPropertyDefinition->getDescription() );
						pPropertyDefinition->addDictionaryItems ( pGmlDictionaryEntry );
            pCodeLists->addDictionaryEntry ( pGmlDictionaryEntry );
          }
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt aus einem GmlDictionaryEntry die zug. GmlEnumeration             //
///////////////////////////////////////////////////////////////////////////////
void GmlSchema::generateEnumerationFromExternalCodeList ( GmlDictionaryEntry * pEntry )
{
  GmlEnumeration	   * pEnumeration;
  GmlEnumerationItem * pEnumerationItem;
  vector<string>	     vSchluesselNummern;
  string			         enumerationName;
  string			         schluesselNummer;
  string			         schluesselText;
	size_t			         j, schluesselAnz;

  enumerationName = pEntry->getId();
  pEnumeration = new GmlEnumeration ( enumerationName, true );

  schluesselAnz = pEntry->getCodelistSchluessel ( vSchluesselNummern );
  for ( j = 0; j < schluesselAnz; j++ )
  {
    schluesselNummer = vSchluesselNummern[j];
    pEnumerationItem = new GmlEnumerationItem ( schluesselNummer );
    pEnumeration->addGmlEnumerationItem ( pEnumerationItem );
  }

  addGmlEnumeration ( pEnumeration );
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt die GmlEnumeration zu einem DictionaryEntry                      //
///////////////////////////////////////////////////////////////////////////////
GmlEnumeration * GmlSchema::generateGmlEnumeration ( GmlDictionaryEntry * pDictionaryEntry, std::string codeSpace )
{
  GmlEnumeration	    * pEnumeration;
  GmlEnumerationItem	* pEnumerationItem;
  vector<string>		    vSchluesselNummern;
  string				        enumerationName;
  string				        schluesselNummer;
  string				        schluesselText;
	size_t					      j, schluesselAnz;

  enumerationName = pDictionaryEntry->getId();
  pEnumeration = new GmlEnumeration ( enumerationName, true );
  pEnumeration->setCodeListDictionary ( codeSpace );

  schluesselAnz = pDictionaryEntry->getCodelistSchluessel ( vSchluesselNummern );
  for ( j = 0; j < schluesselAnz; j++ )
  {
    schluesselNummer = vSchluesselNummern[j];
    pEnumerationItem = new GmlEnumerationItem ( schluesselNummer );
    pEnumeration->addGmlEnumerationItem ( pEnumerationItem );
  }
  return pEnumeration;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine GmlEnumeration zur map gmlEnumerationen hinzu                  //
///////////////////////////////////////////////////////////////////////////////
void GmlSchema::addGmlEnumeration ( GmlEnumeration * pGmlEnumeration )
{
  std::map<std::string,GmlEnumeration*>::iterator iter;

  string enumerationId = pGmlEnumeration->getGmlEnumerationName();
  iter = gmlEnumerationen.find( enumerationId );

  if ( iter != gmlEnumerationen.end() )
    delete iter->second;

  gmlEnumerationen[enumerationId] = pGmlEnumeration;
}


///////////////////////////////////////////////////////////////////////////////
//  Einlesen der XML-Schema Dateien für ein bestimmtes Applikationsschema    //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchema::readGmlSchema ( GML_SCHEMA_TYPE xPlanGmlVersion, GmlNamespaces * pGmlNamespacesAct )
{
	GmlSchemaTypes    * pGmlSchemaTypes;
	GmlSchemaElements * pGmlSchemaElements;
  GmlSchemaGroups   * pGmlSchemaGroups;
  GmlKlasse         * pGmlKlasse;
  GmlAttribut       * pGmlAttribut;
  SchemaTypeNames   * pSchemaTypeNames;

  vector<GmlKlasse*>   vGmlKlassen;
  vector<GmlAttribut*> vGmlAttribute;

  IEnumerator       ^ FILES_ENUM;

  string              schemaFolder;
  string              codeListsFolder;
	string              codeListsIdentifier;
	string              codeListsIdentifierCodespace;
  string              externalCodeListsFolder;
  string              definitionsFolder;
	string              schemaDatei;
	string              xplanSchemaPfad;
	string              definitionPath;
	string              codeListDatei;
	string              externalCodeListDatei;
  string              klassenName;
  string              attributName;
  string              targetNamespace;
  String            ^ SCHEMA_FOLDER;
  String            ^ CODE_LISTS_FOLDER;
	String            ^ DEFINITIONS_FOLDER;
  String            ^ TARGET_NAMESPACE;
  cli::array<String^> ^ FILES;
  bool                success;
	string              message, extension;
	size_t              i, j, anz, index, typeAnz, attributAnz, anzSchemaRead = 0;
  bool                isDictionaryCollection;
  bool                isCityGMLSpecial;

  gmlTyp = xPlanGmlVersion;
  
  pSchemaTypeNames          = NULL;
  if ( pSchemaTypeNamesStandard != NULL )
    delete pSchemaTypeNamesStandard;
  pSchemaTypeNamesStandard = NULL;

	pGmlSchemaTypes          = new GmlSchemaTypes;
	pGmlSchemaElements       = new GmlSchemaElements;
  pGmlSchemaGroups         = new GmlSchemaGroups;
  if ( pGmlNamespacesStandard != NULL )
    delete pGmlNamespacesStandard;
  pGmlNamespacesStandard   = new GmlNamespaces ( xPlanGmlVersion );

  if ( pGmlNamespacesAct == NULL )
    pGmlNamespacesAct = new GmlNamespaces ( xPlanGmlVersion );
  pGmlNamespaces = pGmlNamespacesAct;

	GmlSchemaReader ^ pReader = gcnew GmlSchemaReader( pGmlSchemaTypes, pGmlSchemaElements, pGmlSchemaGroups, pGmlNamespacesStandard );
  GmlDictionaryReader ^ pDictionaryReader = gcnew GmlDictionaryReader ( xPlanGmlVersion );

  switch ( xPlanGmlVersion )
  {
  case XPLANGML_2_0:
    schemaFolder            = Constant::getSchemaPath( XPLANGML_2_0 );
    codeListsFolder         = Constant::getCodelistPath( XPLANGML_2_0 );
		codeListsIdentifier			     = "Enumerationen_XPlanGML_2_0";
		codeListsIdentifierCodespace = "urn:xplan";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( XPLANGML_2_0 );
    definitionsFolder       = "";
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
    break;

  case XPLANGML_3_0:
    schemaFolder            = Constant::getSchemaPath( XPLANGML_3_0 );
    codeListsFolder         = Constant::getCodelistPath( XPLANGML_3_0 );
		codeListsIdentifier			     = "Enumerationen_XPlanGML_3_0";
		codeListsIdentifierCodespace = "urn:xplan";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( XPLANGML_3_0 );
    definitionsFolder       = "";
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
    break;

  case XPLANGML_4_0:
    schemaFolder            = Constant::getSchemaPath( XPLANGML_4_0 );
    codeListsFolder         = Constant::getCodelistPath( XPLANGML_4_0 );
		codeListsIdentifier			     = "Enumerationen_XPlanGML_4_0";
		codeListsIdentifierCodespace = "urn:xplan";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( XPLANGML_4_0 );
    definitionsFolder       = Constant::getDefinitionsPath( XPLANGML_4_0 );
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
    break;

  case XPLANGML_4_1:
    schemaFolder            = Constant::getSchemaPath( XPLANGML_4_1 );
    codeListsFolder         = Constant::getCodelistPath( XPLANGML_4_1 );
		codeListsIdentifier			     = "Enumerationen_XPlanGML_4_1";
		codeListsIdentifierCodespace = "urn:xplan";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( XPLANGML_4_1 );
    definitionsFolder       = Constant::getDefinitionsPath( XPLANGML_4_1 );
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
    break;

  case XPLANGML_5_0:
    schemaFolder            = Constant::getSchemaPath( XPLANGML_5_0 );
    codeListsFolder         = Constant::getCodelistPath( XPLANGML_5_0 );
		codeListsIdentifier			     = "Enumerationen_XPlanGML_5_0";
		codeListsIdentifierCodespace = "urn:xplan";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( XPLANGML_5_0 );
    definitionsFolder       = Constant::getDefinitionsPath( XPLANGML_5_0 );
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
    break;

  case XPLANGML_5_1:
    schemaFolder            = Constant::getSchemaPath( XPLANGML_5_1 );
    codeListsFolder         = Constant::getCodelistPath( XPLANGML_5_1 );
		codeListsIdentifier			     = "Enumerationen_XPlanGML_5_1";
		codeListsIdentifierCodespace = "urn:xplan";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( XPLANGML_5_1 );
    definitionsFolder       = Constant::getDefinitionsPath( XPLANGML_5_1 );
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
    break;

  case XPLANGML_5_2:
    schemaFolder            = Constant::getSchemaPath( XPLANGML_5_2 );
    codeListsFolder         = Constant::getCodelistPath( XPLANGML_5_2 );
		codeListsIdentifier			     = "Enumerationen_XPlanGML_5_2";
		codeListsIdentifierCodespace = "urn:xplan";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( XPLANGML_5_2 );
    definitionsFolder       = Constant::getDefinitionsPath( XPLANGML_5_2 );
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
    break;

  case INSPIRE_LU_PLANNED:
    schemaFolder            = Constant::getSchemaPath( INSPIRE_LU_PLANNED );
    codeListsFolder         = "";
		codeListsIdentifier			     = "Enumerationen_INSPIRE_PLU";
		codeListsIdentifierCodespace = "urn:inspire";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( INSPIRE_LU_PLANNED );
    definitionsFolder       = "";
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
    break;

  case INSPIRE:
    schemaFolder            = Constant::getSchemaPath( INSPIRE );
    codeListsFolder         = "";
		codeListsIdentifier			     = "";
		codeListsIdentifierCodespace = "";
		externalCodeListsFolder = "";
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
    break;

  case CITYGML_0_4_0:
    schemaFolder            = Constant::getSchemaPath( CITYGML_0_4_0 );
    codeListsFolder         = Constant::getCodelistPath( CITYGML_0_4_0 );
		codeListsIdentifier			     = "Enumerationen_CityGML_0_4_0";
		codeListsIdentifierCodespace = "urn:citygml";
		externalCodeListsFolder = "";
    definitionsFolder       = "";
    isDictionaryCollection  = true;
    isCityGMLSpecial        = true;
    break;

  case CITYGML_1_0:
    schemaFolder            = Constant::getSchemaPath( CITYGML_1_0 );
    codeListsFolder         = Constant::getCodelistPath( CITYGML_1_0 );
		codeListsIdentifier			     = "Enumerationen_CityGML_1_0";
		codeListsIdentifierCodespace = "urn:citygml";
		externalCodeListsFolder = "";
    definitionsFolder       = "";
    isDictionaryCollection  = false;
    isCityGMLSpecial        = true;
    break;

  case CITYGML_2_0:
    schemaFolder            = Constant::getSchemaPath( CITYGML_2_0 );
    codeListsFolder         = Constant::getCodelistPath( CITYGML_2_0 );
		codeListsIdentifier			     = "Enumerationen_CityGML_2_0";
		codeListsIdentifierCodespace = "urn:citygml";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( CITYGML_2_0 );
    definitionsFolder       = "";
    isDictionaryCollection  = false;
    isCityGMLSpecial        = false;
    break;

  case CITYGML_3_0:
    schemaFolder            = Constant::getSchemaPath( CITYGML_3_0 );
    codeListsFolder         = Constant::getCodelistPath( CITYGML_3_0 );
		codeListsIdentifier			     = "Enumerationen_CityGML_3_0";
		codeListsIdentifierCodespace = "urn:citygml";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( CITYGML_3_0 );
    definitionsFolder       = "";
    isDictionaryCollection  = false;
    isCityGMLSpecial        = false;
    break;

  case BOREHOLEML_3_0:
    codeListsFolder         = "";
    schemaFolder            = Constant::getSchemaPath( BOREHOLEML_3_0 );
    externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( BOREHOLEML_3_0 );
		codeListsIdentifier			     = "";
		codeListsIdentifierCodespace = "";
		definitionsFolder       = "";
    isDictionaryCollection  = false;
    isCityGMLSpecial        = false;
    break;

  case NAS_5_1:
    schemaFolder            = Constant::getSchemaPath( NAS_5_1 );
    codeListsFolder         = Constant::getCodelistPath( NAS_5_1 );
		codeListsIdentifier			     = "Enumerationen_ALKIS_5_1";
		codeListsIdentifierCodespace = "urn:alkis";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( NAS_5_1 );
    definitionsFolder       = "";
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
    break;

  case NAS_5_1_1:
    schemaFolder            = Constant::getSchemaPath( NAS_5_1_1 );
    codeListsFolder         = Constant::getCodelistPath( NAS_5_1_1 );
		codeListsIdentifier			     = "Enumerationen_ALKIS_5_1_1";
		codeListsIdentifierCodespace = "urn:alkis";
		externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( NAS_5_1 );
    definitionsFolder       = "";
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
    break;

  case NAS_6_0:
    schemaFolder            = Constant::getSchemaPath( NAS_6_0 );
    codeListsFolder         = "";
		codeListsIdentifier			     = "Enumerationen_ALKIS_6_0";
		codeListsIdentifierCodespace = "urn:alkis";
		externalCodeListsFolder = Constant::getCodelistPath( NAS_6_0 );
    definitionsFolder       = Constant::getDefinitionsPath( NAS_6_0 );
    isDictionaryCollection  = false;
    isCityGMLSpecial        = false;
    break;

  case GML_SCHEMA_DEFAULT:
    schemaFolder            = Constant::getSchemaPath( GML_SCHEMA_DEFAULT );
    codeListsFolder         = "";
		codeListsIdentifier			     = "";
		codeListsIdentifierCodespace = "";
		externalCodeListsFolder =  m_pFeatures->getExternalCodeListsFolder( GML_SCHEMA_DEFAULT );
    definitionsFolder       = "";
    isDictionaryCollection  = true;
    isCityGMLSpecial        = false;
  }

  SCHEMA_FOLDER = QuConvert::ToString ( schemaFolder );
  bool exist = Directory::Exists ( SCHEMA_FOLDER );
  if ( exist )
  {
    FILES = Directory::GetFiles ( SCHEMA_FOLDER );

    FILES_ENUM = FILES->GetEnumerator();
    while ( FILES_ENUM->MoveNext() )
    {
      String^ FILE_NAME = safe_cast<String^>(FILES_ENUM->Current);
      QuConvert::systemStr2stdStr ( schemaDatei, FILE_NAME );
      success = pReader->read ( schemaDatei );
      if ( success )
      {
        anzSchemaRead++;
        TARGET_NAMESPACE = pReader->getTargetNamespace();
        QuConvert::systemStr2stdStr( targetNamespace, TARGET_NAMESPACE );
        pGmlNamespacesStandard->addSchemadatei ( targetNamespace, schemaDatei );
      }
    }
  }

  if ( anzSchemaRead == 0 )
  {
    delete pGmlSchemaTypes;
    delete pGmlSchemaElements;
    delete pGmlNamespacesStandard;
    delete pGmlNamespacesAct;
    delete pGmlSchemaGroups;
    return false;
  }

  pGmlSchemaElements->resolveReferences();
  pGmlSchemaGroups->resolveReferences ( pGmlSchemaElements );
  pGmlSchemaTypes->resolveReferences ( pGmlSchemaElements, pGmlSchemaGroups );

  if ( pCodeLists != NULL ) 
    delete pCodeLists;
  pCodeLists = new GmlDictionary;
	pCodeLists->setId ( "Enumerationen" );
	pCodeLists->setIdentifier ( codeListsIdentifier );
	pCodeLists->setIdentifierCodespace ( codeListsIdentifierCodespace );
	
  if ( codeListsFolder != "" )
  {
    CODE_LISTS_FOLDER = QuConvert::ToString ( codeListsFolder );
    if ( Directory::Exists ( CODE_LISTS_FOLDER ) )
    {
      FILES = Directory::GetFiles ( CODE_LISTS_FOLDER );
      FILES_ENUM = FILES->GetEnumerator();
      while ( FILES_ENUM->MoveNext() )
      {
        String^ FILE_NAME = safe_cast<String^>(FILES_ENUM->Current);
        FILE_NAME = FILE_NAME->ToUpper();
        QuConvert::systemStr2stdStr ( codeListDatei, FILE_NAME );
        index = codeListDatei.find_last_of( "." );
        extension = codeListDatei.substr( index+1, codeListDatei.length() );
        if ( extension == "XML" )
        {
          if ( isDictionaryCollection )
            pDictionaryReader->readGmlDictionaryCollection ( codeListDatei, pCodeLists, isCityGMLSpecial );
          else
            pDictionaryReader->readGmlSingleDictionary ( codeListDatei, pCodeLists, isCityGMLSpecial );
        }
      }
    }
  }

  pReader->closeReader();

  pSchemaTypeNames   = new SchemaTypeNames ( pGmlNamespacesAct );
  pSchemaTypeNamesStandard = new SchemaTypeNames ( pGmlNamespacesStandard );

	analyzeGmlSchema ( pGmlSchemaTypes, pGmlSchemaElements, pSchemaTypeNamesStandard );

  if ( xPlanGmlVersion == XPLANGML_2_0 || xPlanGmlVersion == XPLANGML_3_0 )
  {
    GmlDictionary             * pExternalCodeLists = new GmlDictionary;
    GmlDictionaryEntry        * pEntry;
    vector<GmlDictionaryEntry*> vEntries;

    CODE_LISTS_FOLDER = QuConvert::ToString ( externalCodeListsFolder );
    if ( Directory::Exists ( CODE_LISTS_FOLDER ) )
    {
      FILES = Directory::GetFiles ( CODE_LISTS_FOLDER );
      FILES_ENUM = FILES->GetEnumerator();
      while ( FILES_ENUM->MoveNext() )
      {
        String^ FILE_NAME = safe_cast<String^>(FILES_ENUM->Current);
        FILE_NAME = FILE_NAME->ToUpper();

        QuConvert::systemStr2stdStr ( codeListDatei, FILE_NAME );
        index = codeListDatei.find_last_of( "." );
        extension = codeListDatei.substr( index+1, codeListDatei.length() );
        if ( extension == "XML" )
          pDictionaryReader->readGmlDictionaryCollection ( codeListDatei, pExternalCodeLists, false );
      }
      anz = pExternalCodeLists->getDictionaryEntries( vEntries );
      for ( i = 0; i < anz; i++ )
      {
        pEntry = vEntries[i]; 
        generateEnumerationFromExternalCodeList ( pEntry );
        pCodeLists->addDictionaryEntry( pEntry );
      }
      pExternalCodeLists->removeDictionaryEntries();
      delete pExternalCodeLists;
    }
  }

  pDictionaryReader->closeReader();

  if ( gmlTyp == XPLANGML_4_0       || 
       gmlTyp == XPLANGML_4_1       ||
       gmlTyp == XPLANGML_5_0       ||
       gmlTyp == XPLANGML_5_1       ||
       gmlTyp == XPLANGML_5_2       ||
       gmlTyp == INSPIRE_LU_PLANNED || 
       gmlTyp == NAS_6_0 )
	{
    if ( pTypeDefinitions != NULL )
      delete pTypeDefinitions;
		pTypeDefinitions = new TypeDefinitions;

		DefinitionReader ^ pDefReader = gcnew DefinitionReader ( pTypeDefinitions );

		DEFINITIONS_FOLDER = QuConvert::ToString ( definitionsFolder );
		bool exist = Directory::Exists ( DEFINITIONS_FOLDER );
		if ( exist )
		{
			FILES_ENUM = Directory::GetFiles ( DEFINITIONS_FOLDER )->GetEnumerator();
			while ( FILES_ENUM->MoveNext() )
			{
				String ^ FILE_NAME = dynamic_cast<String^>(FILES_ENUM->Current);
        FILE_NAME = FILE_NAME->ToUpper();
				QuConvert::systemStr2stdStr ( definitionPath, FILE_NAME );
        index = definitionPath.find_last_of( "." );
        extension = definitionPath.substr( index+1, definitionPath.length() );
        if ( extension == "XML" )
				  pDefReader->read ( definitionPath );
			 }
      pTypeDefinitions->analyzeSuperClasses();
			generateEnumerationsAndCodeLists();
		}
    pDefReader->closeReader();

    typeAnz = getGmlKlassen ( vGmlKlassen );
    for ( i = 0; i < typeAnz; i++ )
    {
      pGmlKlasse = vGmlKlassen[i];
      klassenName = pGmlKlasse->getGmlElementName();
      vGmlAttribute.clear();
      attributAnz =  pGmlKlasse->getGmlAttribute( vGmlAttribute );
      for ( j = 0; j < attributAnz; j++ )
      {
        pGmlAttribut = vGmlAttribute[j];
        if ( pGmlAttribut->getGmlAttributTypName() == "gml:CodeType" )
        {
          attributName = pGmlAttribut->getGmlAttributName();
          PropertyDefinition * pPropertyDefinition = pTypeDefinitions->getPropertyDefinition( klassenName, attributName );
          if ( pPropertyDefinition != NULL )
          {
            string valueTypeName = pPropertyDefinition->getValueTypeName();
            pGmlAttribut->setGmlAttributTypName( valueTypeName );
            pGmlAttribut->setGmlEnumeration( valueTypeName );
          }

        }
      }
    }
	}

	delete pGmlSchemaTypes;
	delete pGmlSchemaElements;
  delete pSchemaTypeNames;
  delete pGmlSchemaGroups;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Liest das als String übergebene GML-Applikationsschema ein                // 
///////////////////////////////////////////////////////////////////////////////
bool  GmlSchema::readGmlSchemaFromString ( std::string gmlTypString, GmlNamespaces * pGmlNamespacesAct )
{
  gmlTyp = Constant::getGMLSchemaTypeFromString( gmlTypString );
  return readGmlSchema( gmlTyp, pGmlNamespacesAct );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Ordner der XML-Schemata                                      //
///////////////////////////////////////////////////////////////////////////////

std::string GmlSchema::getSchemaFolder()
{
  return Constant::getSchemaPath( gmlTyp );
}

///////////////////////////////////////////////////////////////////////////////
//  Baut die Klasse GmlSxchema auf											                     //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchema::analyzeGmlSchema (  GmlSchemaTypes * pGmlSchemaTypesP, 
                                          GmlSchemaElements * pGmlSchemaElementsP, SchemaTypeNames * pSchemaTypeNames )
{
	size_t  			  i, k, elementAnz, attributAnz, typeAnz, complexTypeAnz, substitutionElementAnz;
	int							j;
	size_t				  stringLength;
	bool					  abstract;
	string				  klassenName;
  string          attributName;
	string          stdString;
  string	        klassenReferenz;
  string	        parantTypeName;
	string				  typName;
  string          typNamespace;
  string          elementType;
  string          elementTypeNamespace;
  string	        elementName;
	bool					  isFlaechenschlussObjekt;
	bool					  isUeberlagenungsObjekt;

	GmlSchemaElement		    * pSchemaElement;
  GmlSchemaElement        * pSchemaElementRef;
  GmlElement              * pElement;

	GmlKlasse			        	* pGmlKlasse;
	GmlAttribut				      * pGmlAttribut;
	GmlSchemaType           * pSchemaType;
	GmlSchemaComplexType    * pParantType;
  GmlSchemaComplexType    * pComplexType;
	GmlSchemaType           * pSchemaSimpleType;

  vector<GmlSchemaElement*>     schemaElements;
  vector<GmlSchemaType*>        simpleTypes;
  vector<GmlKlasse*>            vGmlKlassen;
  vector<GmlSchemaComplexType*> vComplexTypes;
  vector<GmlSchemaComplexType*> parantTypes;
  vector<GmlSchemaAttribut*>    vSchemaAttributes;
  vector<GmlSchemaElement*>     vChildElements;
  vector<GmlSchemaElement*>     vSubstitutionGroup;
  vector<GmlAttribut*>          vGmlAttribute;

  complexTypeAnz = pGmlSchemaTypesP->getComplexTypes ( vComplexTypes );
  for ( i = 0; i < complexTypeAnz; i++ )
  {
    pComplexType = vComplexTypes[i];
    vChildElements.clear();
    elementAnz = pComplexType->getChildElements ( vChildElements );
    for ( j = 0; j < (int) vChildElements.size(); j++ )
    {
      pSchemaElement = vChildElements[j];
      if ( pSchemaElement->getRefName() != "" )
      {
        vSubstitutionGroup.clear();
        substitutionElementAnz = pGmlSchemaElementsP->getSubstitutionGroup ( pSchemaElement->getRefName(), vSubstitutionGroup );
        if ( substitutionElementAnz > 0 )
        {
          elementAnz = vChildElements.size();

          pComplexType->insertElements ( j, vSubstitutionGroup );
          vChildElements.resize ( vChildElements.size() + substitutionElementAnz );
          for ( k = elementAnz-1; k >= j+1; k-- )
            vChildElements[k+substitutionElementAnz] = vChildElements[k];

          for ( k = j + 1; k <= j + substitutionElementAnz; k++ )
            vChildElements[k] = vSubstitutionGroup[k-j-1];
          j = j + (int)substitutionElementAnz;
        }
      }

    }
  }

  typeAnz = pGmlSchemaTypesP->getSimpleTypes ( simpleTypes );
  for ( i = 0; i < typeAnz; i++ )
  {
    pSchemaSimpleType = simpleTypes[i];
    klassenName = pSchemaSimpleType->getName();

    if ( pSchemaSimpleType->isSimpleType() == false )
    {
      pComplexType = (GmlSchemaComplexType*)pSchemaSimpleType;
      vComplexTypes.push_back( pComplexType );
    }
  }

  complexTypeAnz = vComplexTypes.size();


  for ( i = 0; i < complexTypeAnz; i++ )
  {
    pComplexType = vComplexTypes[i];

    pGmlKlasse = new GmlKlasse ( pComplexType->getName() );
    pGmlKlasse->setAbstract ( pComplexType->getIsAbstract() );
    addGmlKlasse ( pGmlKlasse );

    vSchemaAttributes.clear();
    attributAnz = pComplexType->getAttributes ( vSchemaAttributes );
    for ( j = 0; j < attributAnz; j++ )
    {
      GmlSchemaAttribut * pSchemaAttribut = vSchemaAttributes[j];
      GmlSchemaAttribut * pSchemaAttributCopy = new GmlSchemaAttribut ( pSchemaAttribut );
      pGmlKlasse->addGmlSchemaAttribut ( pSchemaAttributCopy );
    }

    parantTypes.clear();
    parantTypes.push_back ( pComplexType );
    pParantType = pComplexType->getParantType ( pGmlSchemaTypesP );

    isFlaechenschlussObjekt = false;
    isUeberlagenungsObjekt  = false;

    while ( pParantType != NULL )
    {
      typName = pParantType->getName();
      pGmlKlasse->addParentTypeName(  typName );
      parantTypes.push_back ( pParantType );
      pParantType = pParantType->getParantType ( pGmlSchemaTypesP );
    }		

    for ( j = (int)parantTypes.size() - 1; j >= 0; j-- )
    {

      pParantType = parantTypes[j];
      typName = pParantType->getName();
      parantTypeName = pParantType->getParantTypeName();

      if ( parantTypeName == pSchemaTypeNames->gmlAbstractObject )
      {
        pGmlKlasse->setIsGmlFeature( false );
      }

      if ( parantTypeName == pSchemaTypeNames->gmlAbstractGML )
      {
        pGmlKlasse->setIsGmlFeature( true );

        pGmlAttribut = new GmlAttribut ( pSchemaTypeNames->gmlDescription  );  // "gml:description"
        pGmlAttribut->setGmlAttributTyp ( _STRING );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( true );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );

        pGmlAttribut = new GmlAttribut ( pSchemaTypeNames->gmlName );    // "gml:name"
        pGmlAttribut->setGmlAttributTyp ( _STRING );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( true );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );

        pGmlAttribut = new GmlAttribut ( pSchemaTypeNames->gmlIdentifier );    // "gml:Identifier"
        pGmlAttribut->setGmlAttributTyp ( _STRING );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( false );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );
      }


      if ( parantTypeName == pSchemaTypeNames->gmlAbstractFeature || 
           parantTypeName == pSchemaTypeNames->gmlAbstractFeatureCollection )
      {
        pGmlKlasse->setIsGmlFeature( true );

        pGmlAttribut = new GmlAttribut ( pSchemaTypeNames->gmlDescription  );  // "gml:description"
        pGmlAttribut->setGmlAttributTyp ( _STRING );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( true );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );

        pGmlAttribut = new GmlAttribut ( pSchemaTypeNames->gmlName );    // "gml:name"
        pGmlAttribut->setGmlAttributTyp ( _STRING );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( true );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );

        pGmlAttribut = new GmlAttribut ( pSchemaTypeNames->gmlIdentifier );    // "gml:Identifier"
        pGmlAttribut->setGmlAttributTyp ( _STRING );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( false );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );

        pGmlAttribut = new GmlAttribut ( pSchemaTypeNames->gmlBoundedBy );    //  "gml:boundedBy"
        pGmlAttribut->setGmlAttributTyp ( _GEOMETRIE );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( false );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );
      }

      addAttribute ( pGmlKlasse, pParantType, pGmlSchemaTypesP, pSchemaTypeNames, pGmlSchemaElementsP );

      if (  gmlTyp == CITYGML_0_4_0 || 
            gmlTyp == CITYGML_1_0   ||
            gmlTyp == CITYGML_2_0 )
      {
        if (  typName == "genobj:StringAttributeType"  ||
              typName == "genobj:DoubleAttributeType"  ||
              typName == "genobj:IntAttributeType"     ||
              typName == "genobj:UriAttributeType"     ||
              typName == "genobj:DateAttributeType"    ||
              typName == "genobj:MeasureAttributeType" ||
              typName == "genobj:GenericAttributeSetType"   )
        {
          pGmlAttribut = new GmlAttribut ( "genobj:name" );    //  genobj:name
          pGmlAttribut->setGmlAttributTyp ( _STRING );
          pGmlAttribut->setOptional ( false );
          pGmlAttribut->setMultiple ( false );
          pGmlKlasse->addGmlAttribut ( pGmlAttribut );
        }
      }

      if ( typName == "xplan:XP_PlanType" ) pGmlKlasse->setGmlObjektTyp( BLEIT_PLAN );

      if ( typName == "xplan:BP_PlanType" ) pGmlKlasse->setPlanTyp ( BPLAN );
      if ( typName == "xplan:FP_PlanType" ) pGmlKlasse->setPlanTyp ( FPLAN );
      if ( typName == "xplan:RP_PlanType" ) pGmlKlasse->setPlanTyp ( RPLAN );
      if ( typName == "xplan:LP_PlanType" ) pGmlKlasse->setPlanTyp ( LPLAN );
      if ( typName == "xplan:SO_PlanType" ) pGmlKlasse->setPlanTyp ( SOPLAN );

      if ( typName == "xplan:BP_FlaechenschlussobjektType" ||
           typName == "xplan:FP_FlaechenschlussobjektType"      ) isFlaechenschlussObjekt = true;

      if ( typName == "xplan:BP_UeberlagerungsobjektType" ||
           typName == "xplan:FP_UeberlagerungsobjektType"   )	isUeberlagenungsObjekt = true;

      if ( typName == "xplan:XP_BereichType" ) pGmlKlasse->setGmlObjektTyp( PLAN_BEREICH );
      if ( typName == "xplan:XP_LTOType" )
        pGmlKlasse->setGmlObjektTyp( XP_LTO );
      if ( typName == "xplan:XP_PTOType" )
        pGmlKlasse->setGmlObjektTyp( XP_PTO );
      if ( typName == "xplan:XP_FPOType" )
        pGmlKlasse->setGmlObjektTyp( XP_FPO );
      if ( typName == "xplan:XP_LPOType" ) 
        pGmlKlasse->setGmlObjektTyp( XP_LPO );
      if ( typName == "xplan:XP_PPOType" )
        pGmlKlasse->setGmlObjektTyp( XP_PPO );

      if ( typName == "xplan:XP_RasterplanBasisType" || 
           typName == "xplan:XP_RasterdarstellungType")
      {
        pGmlKlasse->setGmlObjektTyp ( RASTER_BASIS );		
        pGmlKlasse->setPlanTyp ( XPLAN );
      }
      if ( typName == "xplan:XP_RasterplanAenderungType" ) pGmlKlasse->setGmlObjektTyp ( RASTER_AENDERUNG );

      if ( typName == "xplan:BP_RasterplanAenderungType" )	pGmlKlasse->setPlanTyp ( BPLAN );
      if ( typName == "xplan:FP_RasterplanAenderungType" )	pGmlKlasse->setPlanTyp ( FPLAN );
      if ( typName == "xplan:LP_RasterplanAenderungType" )	pGmlKlasse->setPlanTyp ( LPLAN );
      if ( typName == "xplan:RP_RasterplanAenderungType" )	pGmlKlasse->setPlanTyp ( RPLAN );
      if ( typName == "xplan:SO_RasterplanAenderungType" )	pGmlKlasse->setPlanTyp ( SOPLAN );

      if ( typName == "xplan:BP_ObjektType" )	pGmlKlasse->setPlanTyp ( BPLAN );
      if ( typName == "xplan:FP_ObjektType" )	pGmlKlasse->setPlanTyp ( FPLAN );
      if ( typName == "xplan:RP_ObjektType" )	pGmlKlasse->setPlanTyp ( RPLAN );
      if ( typName == "xplan:LP_ObjektType" )	pGmlKlasse->setPlanTyp ( LPLAN );
      if ( typName == "xplan:SO_ObjektType" ) pGmlKlasse->setPlanTyp ( SOPLAN );

      if ( typName == "xplan:XP_AbstraktesPraesentationsobjektType" )	pGmlKlasse->setPlanTyp ( XPLAN );

      if ( typName == "xplan:XP_ObjektType" || typName == "xplan:XP_PlanType" )
      {
        pGmlAttribut = new GmlAttribut ( "xplan:hatGenerStringAttribut" );
        if ( gmlTyp <= XPLANGML_3_0 )
          pGmlAttribut->setGmlAttributTyp ( _REFERENZ );
        else
          pGmlAttribut->setGmlAttributTyp ( _DATA_TYPE );
        pGmlAttribut->setGmlAttributTypName( "xplan:XP_StringAttributType");
        pGmlAttribut->setReferenziertesElement( "xplan:XP_StringAttribut" );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( true );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );

        pGmlAttribut = new GmlAttribut ( "xplan:hatGenerIntegerAttribut" );
        if (gmlTyp <= XPLANGML_3_0 )
          pGmlAttribut->setGmlAttributTyp ( _REFERENZ );
        else
          pGmlAttribut->setGmlAttributTyp ( _DATA_TYPE );
        pGmlAttribut->setGmlAttributTypName( "xplan:XP_IntegerAttributType" );
        pGmlAttribut->setReferenziertesElement( "xplan:XP_IntegerAttribut" );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( true );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );

        pGmlAttribut = new GmlAttribut ( "xplan:hatGenerDoubleAttribut" );
        if ( gmlTyp <= XPLANGML_3_0 )
          pGmlAttribut->setGmlAttributTyp ( _REFERENZ );
        else
          pGmlAttribut->setGmlAttributTyp ( _DATA_TYPE );
        pGmlAttribut->setGmlAttributTypName( "xplan:XP_DoubleAttributType" );
        pGmlAttribut->setReferenziertesElement( "xplan:XP_DoubleAttribut" );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( true );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );

        pGmlAttribut = new GmlAttribut ( "xplan:hatGenerDatumAttribut" );
        if ( gmlTyp <= XPLANGML_3_0 )
          pGmlAttribut->setGmlAttributTyp ( _REFERENZ );
        else
          pGmlAttribut->setGmlAttributTyp ( _DATA_TYPE );
        pGmlAttribut->setGmlAttributTypName( "xplan:XP_DatumAttributType" );
        pGmlAttribut->setReferenziertesElement( "xplan:XP_DatumAttribut" );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( true );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );

        pGmlAttribut = new GmlAttribut ( "xplan:hatGenerURLAttribut" );
        if ( gmlTyp <= XPLANGML_3_0 )
          pGmlAttribut->setGmlAttributTyp ( _REFERENZ );
        else
          pGmlAttribut->setGmlAttributTyp ( _DATA_TYPE );
        pGmlAttribut->setGmlAttributTypName( "xplan:XP_URLAttributType" );
        pGmlAttribut->setReferenziertesElement( "xplan:XP_URLAttribut" );
        pGmlAttribut->setOptional ( true );
        pGmlAttribut->setMultiple ( true );
        pGmlKlasse->addGmlAttribut ( pGmlAttribut );
      }

      if ( typName == pSchemaTypeNames->zoningElementType )
      {
        pGmlKlasse->setGmlObjektTyp( XP_FLAECHEN_OBJEKT );
      }

      pParantType = pParantType->getParantType ( pGmlSchemaTypesP );
    }		

    if ( isFlaechenschlussObjekt || isUeberlagenungsObjekt )
    {
      pGmlAttribut = pGmlKlasse->getGmlAttribut ( "xplan:flaechenschluss" );
      if ( isFlaechenschlussObjekt ) pGmlAttribut->setDefaultWert ( "true" );
      if ( isUeberlagenungsObjekt )  pGmlAttribut->setDefaultWert ( "false" );
    }

  }

  elementAnz = pGmlSchemaElementsP->getElements ( schemaElements );
  for ( i = 0; i < elementAnz; i++ )
  {
    pSchemaElement = schemaElements[i];
    klassenReferenz = pSchemaElement->getRefName();

    if ( klassenReferenz != "" )
    {
      pSchemaElementRef = pGmlSchemaElementsP->getElement ( klassenReferenz );
      if ( pSchemaElementRef != NULL )
      {
        typName = pSchemaElementRef->getElementTypeName();
        pSchemaElement->setElementTypeName ( typName );

        typNamespace = pSchemaElementRef->getElementTypeNamespace();
        if ( typNamespace != "" ) pSchemaElement->setElementTypeNamespace ( typNamespace );
      }
    }
  }

  schemaElements.clear();
  elementAnz = pGmlSchemaElementsP->getElements ( schemaElements );
  for ( i = 0; i < elementAnz; i++ )
  {
    pSchemaElement = schemaElements[i];
    elementName = pSchemaElement->getName();
    elementType = pSchemaElement->getElementTypeName();
    abstract    = pSchemaElement->getAbstractElement();
    elementTypeNamespace = pSchemaElement->getElementTypeNamespace();

    pSchemaType = pSchemaElement->getElementTypeObject ( pGmlSchemaTypesP );
    if ( pSchemaType != NULL /* && pSchemaType->isComplex() */ )
    {
      pElement = new GmlElement( elementName );
      pElement->setAbstract ( abstract );
      addGmlElement ( pElement );

      typName = pSchemaType->getName();
      pElement->setGmlTypeName ( typName );

      pGmlKlasse = getGmlKlasse( typName );
      if ( pGmlKlasse != NULL )
        pGmlKlasse->setAbstract( abstract );
      pElement->setGmlTypeNamespace ( elementTypeNamespace );
    }
    else
      if ( elementTypeNamespace == "gml" || elementTypeNamespace == "xs"  /*&& elementType == "FeaturePropertyType" */ )
      {
        pElement = new GmlElement( elementName );
        pElement->setGmlTypeName ( elementType );
        pElement->setGmlTypeNamespace ( elementTypeNamespace );
        addGmlElement ( pElement );
      }
  }

	for ( i = 0; i < typeAnz; i++ )
	{
		pSchemaSimpleType = simpleTypes[i];
		klassenName = pSchemaSimpleType->getName();

    if ( pSchemaSimpleType->isSimpleType() == true )
    {
      stringLength = klassenName.length();
      stdString    = klassenName.substr ( stringLength-4, 4 );
      if ( stdString == "Type" )
        klassenName = klassenName.substr ( 0, stringLength-4 );

      GmlEnumeration * pGmlEnumeration = new GmlEnumeration ( klassenName, false );
      addGmlEnumeration ( pGmlEnumeration );

      addEnumerationItems ( pGmlEnumeration, ((GmlSchemaSimpleType*)pSchemaSimpleType) );
    }
    else
    {

    }

	}

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt die Attribute aus dem GmlSchemaComplexType zur GmlKlasse hinzu      //
///////////////////////////////////////////////////////////////////////////////
void GmlSchema::addAttribute (  GmlKlasse * pGmlKlasseP, 
                                    GmlSchemaComplexType * pSchemaComplexTypeP, GmlSchemaTypes * pGmlSchemaTypesP,
                                    SchemaTypeNames * pSchemaTypeNames, GmlSchemaElements * pGmlSchemaElementsP )
{
	size_t  			            j, childElementAnz, unnamedChildElementAnz, k;
	vector<GmlSchemaElement*> childElements;
  vector<GmlSchemaElement*> vUnnamedChildElements;
	GmlSchemaElement		    * pChildElement;
	GmlSchemaType			      * pChildElementType;
  GmlSchemaComplexType    * pChildElementComplexType;
	GmlAttribut             * pAttribut;
	string					          attributName;
	string					          typName;
	string                    gmlEnumeration;
	string					          childElementNamespace;
	string					          childElementTypeName;
	string					          childElementExternalCodeList;
  string                    refName;
	ATTRIBUT_TYP			        childElementTyp;

	childElementAnz = pSchemaComplexTypeP->getChildElements ( childElements );
	for ( j = 0; j < childElementAnz; j++ )
	{
		pChildElement                = childElements[j];
		attributName			           = pChildElement->getName();
		childElementNamespace	       = pChildElement->getElementTypeNamespace();
		childElementTypeName         = pChildElement->getElementTypeName();
		childElementExternalCodeList = pChildElement->getExternalCodeListName();
    childElementTyp			         = _SONST;

      if (  (  gmlTyp == CITYGML_0_4_0 || 
               gmlTyp == CITYGML_1_0   ||
                gmlTyp == CITYGML_2_0 ) && 
              pChildElement->getRefName() == pSchemaTypeNames->genericAttribute )  //  "_genericAttribute"
      {
        pAttribut = new GmlAttribut ( pSchemaTypeNames->genericStringAttribute );  // "stringAttribute"
        pAttribut->setGmlAttributTyp ( _GENER_STRING );
        pAttribut->setOptional ( true );
        pAttribut->setMultiple ( true );
        pGmlKlasseP->addGmlAttribut ( pAttribut );

        pAttribut = new GmlAttribut ( pSchemaTypeNames->genericDoubleAttribute );   //"doubleAttribute"
        pAttribut->setGmlAttributTyp ( _GENER_DOUBLE );
        pAttribut->setOptional ( true );
        pAttribut->setMultiple ( true );
        pGmlKlasseP->addGmlAttribut ( pAttribut );

        pAttribut = new GmlAttribut ( pSchemaTypeNames->genericIntAttribute );   //  "intAttribute"
        pAttribut->setGmlAttributTyp ( _GENER_INTEGER );
        pAttribut->setOptional ( true );
        pAttribut->setMultiple ( true );
        pGmlKlasseP->addGmlAttribut ( pAttribut );

        pAttribut = new GmlAttribut ( pSchemaTypeNames->genericDateAttribute );    //  "dateAttribute"
        pAttribut->setGmlAttributTyp ( _GENER_DATE );
        pAttribut->setOptional ( true );
        pAttribut->setMultiple ( true );
        pGmlKlasseP->addGmlAttribut ( pAttribut );

        pAttribut = new GmlAttribut ( pSchemaTypeNames->genericUrlAttribute );    //  "urlAttribute"
        pAttribut->setGmlAttributTyp ( _GENER_URL );
        pAttribut->setOptional ( true );
        pAttribut->setMultiple ( true );
        pGmlKlasseP->addGmlAttribut ( pAttribut );

        if ( gmlTyp == CITYGML_2_0 )
        {
          pAttribut = new GmlAttribut ( pSchemaTypeNames->genericMeasureAttribute );    //  "measureAttribute"
          pAttribut->setGmlAttributTyp ( _GENER_MEASURE );
          pAttribut->setOptional ( true );
          pAttribut->setMultiple ( true );
          pGmlKlasseP->addGmlAttribut ( pAttribut );

          pAttribut = new GmlAttribut ( pSchemaTypeNames->genericAttributeSet );    //  attributeSet"
          pAttribut->setGmlAttributTyp ( _GENER_LIST );
          pAttribut->setOptional ( true );
          pAttribut->setMultiple ( true );
          pGmlKlasseP->addGmlAttribut ( pAttribut );
        }
        continue;
      }
      if (  pChildElement->getName() == pSchemaTypeNames->genericStringAttribute  ||
        pChildElement->getName() == pSchemaTypeNames->genericDoubleAttribute  ||
        pChildElement->getName() == pSchemaTypeNames->genericIntAttribute     ||
        pChildElement->getName() == pSchemaTypeNames->genericUrlAttribute     ||
        pChildElement->getName() == pSchemaTypeNames->genericDateAttribute    ||
        pChildElement->getName() == pSchemaTypeNames->genericMeasureAttribute ||
        pChildElement->getName() == pSchemaTypeNames->genericAttributeSet  
        )
        continue;

      if ( pChildElement->getRefName() == "_genericAttribute" )
      {

        pAttribut = new GmlAttribut ( pSchemaTypeNames->appearanceMember );  //  "appearanceMember"
        pAttribut->setGmlAttributTyp ( _APPEARANCE );
        pAttribut->setOptional ( true );
        pAttribut->setMultiple ( true );
        pGmlKlasseP->addGmlAttribut ( pAttribut );

        continue;
      }


    if ( childElementNamespace == pSchemaTypeNames->getXmlSchemaKuerzel() )
    {
      if ( childElementTypeName == pSchemaTypeNames->xmlStringType )
      {
        if ( childElementExternalCodeList != "" )
        {
          childElementTyp = _EXTERNAL_CODE_LIST;
        }
        else
          childElementTyp = _STRING;
      }
      else
      if ( childElementTypeName == pSchemaTypeNames->xmlBooleanType ) 
        childElementTyp = _BOOLEAN;
      else
        if ( childElementTypeName == pSchemaTypeNames->xmlDoubleType       || 
          childElementTypeName == pSchemaTypeNames->xmlDecimalType ||
          childElementTypeName == pSchemaTypeNames->xmlFloatType) 
        childElementTyp = _DOUBLE;
        else
      if ( childElementTypeName == pSchemaTypeNames->xmlAngleType )
        childElementTyp = _DOUBLE;
      else
      if ( childElementTypeName == pSchemaTypeNames->xmlDateType    ||
        childElementTypeName == pSchemaTypeNames->xmlTimeType       || 
        childElementTypeName == pSchemaTypeNames->xmlDurationType   || 
        childElementTypeName == pSchemaTypeNames->xmlGYearType      ||
        childElementTypeName == pSchemaTypeNames->xmlGYearMonthType ||
        childElementTypeName == pSchemaTypeNames->xmlGMonthType     ||
        childElementTypeName == pSchemaTypeNames->xmlGMonthDayType  ||
        childElementTypeName == pSchemaTypeNames->xmlGDayType       )
        childElementTyp = _DATE;
      else
        if ( childElementTypeName == pSchemaTypeNames->xmlDateTimeType  )
          childElementTyp = _DATE_TIME;
      else
      if ( childElementTypeName == pSchemaTypeNames->xmlIntegerType ||
        childElementTypeName == pSchemaTypeNames->xmlIntType                ||
        childElementTypeName == pSchemaTypeNames->xmlLongType               ||
        childElementTypeName == pSchemaTypeNames->xmlShortType              ||
        childElementTypeName == pSchemaTypeNames->xmlByteType               ||
        childElementTypeName == pSchemaTypeNames->xmlUnsignedIntType        ||
        childElementTypeName == pSchemaTypeNames->xmlUnsignedLongType       ||
        childElementTypeName == pSchemaTypeNames->xmlUnsignedShortType      ||
        childElementTypeName == pSchemaTypeNames->xmlUnsignedByteType       ||
        childElementTypeName == pSchemaTypeNames->xmlNonNegativeIntegerType ||
        childElementTypeName == pSchemaTypeNames->xmlNonPositiveIntegerType ||
        childElementTypeName == pSchemaTypeNames->xmlNegativeIntegerType    ||
        childElementTypeName == pSchemaTypeNames->xmlPositiveIntegerType  ) 
        childElementTyp = _INTEGER;
      else
      if ( childElementTypeName == pSchemaTypeNames->xmlAnyUriType ) 
        childElementTyp = _ANY_URI;
    }
    else     
    if ( childElementNamespace == "gml" )
    {
      if ( childElementTypeName == pSchemaTypeNames->gmlAngleType ) 
        childElementTyp = _ANGLE;     //"gml:AngleType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlLengthType ) 
        childElementTyp = _LENGTH;    //  "gml:LengthType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlAreaType ) 
        childElementTyp = _AREA;    //  "gml:LengthType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlMeasureType ) 
        childElementTyp = _MEASURE;    //  "gml:MeasureType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlSpeedType ) 
        childElementTyp = _MEASURE;    //  "gml:SpeedType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlVolumeType ) 
        childElementTyp = _VOLUME;    //  "gml:LengthType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlScaleType ) 
        childElementTyp = _MEASURE;    //  "gml:ScaleType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlAssociationType ) 
        childElementTyp = _REFERENZ;   //  "gml:AssociationType"
      else
      if ( childElementTypeName ==  pSchemaTypeNames->gmlReferenceType)  //  "gml:ReferenceType"
      {
        if ( childElementExternalCodeList != "" )
        {
          childElementTyp = _EXTERNAL_CODE_LIST_GML3_3;
        }
        else
          childElementTyp = _REFERENZ;  
      }
      else
      if ( childElementTypeName ==  pSchemaTypeNames->gmlFeaturePropertyType) 
        childElementTyp = _REFERENZ;   //  "gml:FeaturePropertyType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlMultiCurvePropertyType )	
      {
        childElementTyp = _GEOMETRIE;  //  "gml:MultiCurvePropertyType"
        pGmlKlasseP->setGmlObjektTyp ( XP_LINIEN_OBJEKT );
      }
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlCurvePropertyType )	
      {
        childElementTyp = _GEOMETRIE;   //  "gml:CurvePropertyType"
        pGmlKlasseP->setGmlObjektTyp ( XP_LINIEN_OBJEKT );
      }
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlMultiPointPropertyType )	
      {
        childElementTyp = _GEOMETRIE;  //  "gml:MultiPointPropertyType"
        pGmlKlasseP->setGmlObjektTyp ( XP_PUNKT_OBJEKT );
      }
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlPolygonPropertyType )	
      {
        childElementTyp = _GEOMETRIE;  //  "gmlPolygontPropertyType"
        pGmlKlasseP->setGmlObjektTyp ( XP_FLAECHEN_OBJEKT );
        }
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlPointPropertyType )	
      {
        if ( attributName == "xplan:symbolPosition" )
          childElementTyp = _POS_XY;
        else
        {
          childElementTyp = _GEOMETRIE;
          if ( pGmlKlasseP->getGmlObjektTyp() != BLEIT_PLAN && 
            pGmlKlasseP->getGmlObjektTyp() != PLAN_BEREICH )
            pGmlKlasseP->setGmlObjektTyp ( XP_PUNKT_OBJEKT );
        }
      }
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlGeometricComplexPropertyType )	
        childElementTyp = _GEOMETRIE;  //  "gml:GeometricComplexPropertyType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlDirectPositionType )	
        childElementTyp = _DIRECT_POSITION;  //  "gml:DirectPositionType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlCalDate )	
        childElementTyp = _DATE;  //  "gml:CalDate"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlGeometricPrimitivePropertyType )
        childElementTyp = _GEOMETRIE; 
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlAbstractGeometricPrimitiveType )
        childElementTyp = _GEOMETRIE; 
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlGeometryPropertyType )
      {
        childElementTyp = _GEOMETRIE;  //  "gml:GeometricComplexPropertyType"

        typName = pSchemaComplexTypeP->getName();

        if ( typName == "xplan:BP_FlaechenobjektType" ||
          typName == "xplan:FP_FlaechenobjektType" ||
          typName == "xplan:RP_FlaechenobjektType" ||
          typName == "xplan:LP_FlaechenobjektType" ||
          typName == "xplan:SO_FlaechenobjektType" )
          pGmlKlasseP->setGmlObjektTyp ( XP_FLAECHEN_OBJEKT );
        else
        if ( typName == "xplan:BP_LinienobjektType" ||
            typName == "xplan:FP_LinienobjektType" ||
            typName == "xplan:RP_LinienobjektType" ||
            typName == "xplan:LP_LinienobjektType" ||
            typName == "xplan:SO_LinienobjektType")
          pGmlKlasseP->setGmlObjektTyp ( XP_LINIEN_OBJEKT );
        else
        if ( typName == "xplan:BP_PunktobjektType" ||
             typName == "xplan:FP_PunktobjektType" ||
             typName == "xplan:RP_PunktobjektType" ||
             typName == "xplan:LP_PunktobjektType" ||
             typName == "xplan:SO_PunktobjektType" )
          pGmlKlasseP->setGmlObjektTyp ( XP_PUNKT_OBJEKT );
        else
           pGmlKlasseP->setGmlObjektTyp ( XP_GEOMETRIE_OBJEKT );
      }
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlMultiSurfacePropertyType )	
      {
        childElementTyp = _GEOMETRIE;  //  "gml:MultiSurfacePropertyType"
        pGmlKlasseP->setGmlObjektTyp ( XP_FLAECHEN_OBJEKT );
      }
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlSurfacePropertyType )	
      {
        childElementTyp = _GEOMETRIE;  //  "gml:SurfacePropertyType" 
        pGmlKlasseP->setGmlObjektTyp ( XP_FLAECHEN_OBJEKT );
      }
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlTINPropertyType )
        childElementTyp = _GEOMETRIE;   //  "relief:tinPropertyType" 
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlSolidPropertyType )   
        childElementTyp = _GEOMETRIE;   //  "gml:SolidPropertyType" 
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlMultiSolidPropertyType ) 
        childElementTyp = _GEOMETRIE;  //  "gml:MultiSolidPropertyType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlMeasureOrNullListType || 
           childElementTypeName == pSchemaTypeNames->gmlMeasureListType ) 
        childElementTyp = _MEASURE_LISTE;  // "gml:MeasureOrNullListType", "gml:MeasureListType"
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlIntegerListType    ||          
           childElementTypeName == pSchemaTypeNames->gmlSignType ) 
        childElementTyp = _STRING;
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlQuantityExtentType) 
          childElementTyp = _QUANTITY_EXTENT;
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlCodeType ) 
        childElementTyp = _EXTERNAL_CODE_LIST;
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlTimePositionType )
        childElementTyp = TIME_POSITION;
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlTimePeriodType )
        childElementTyp = TIME_PERIOD;
      else
      if ( childElementTypeName == pSchemaTypeNames->gmlTimeIntervalType )
        childElementTyp = TIME_INTERVAL_LENGTH;
    }
    else
    if ( childElementTypeName == pSchemaTypeNames->lodType )
      childElementTyp = _INTEGER;
		else
  	if ( childElementTypeName == pSchemaTypeNames->xpVariableGeometrieType )
		{
			childElementTyp = _GEOMETRIE;
			pGmlKlasseP->setGmlObjektTyp ( XP_GEOMETRIE_OBJEKT );
		}
    else
		if ( childElementTypeName == pSchemaTypeNames->xpFlaechenGeometrieType )
		{
			childElementTyp = _GEOMETRIE;
			if ( pGmlKlasseP->getGmlObjektTyp() != BLEIT_PLAN && 
				 pGmlKlasseP->getGmlObjektTyp() != PLAN_BEREICH )
			   pGmlKlasseP->setGmlObjektTyp ( XP_FLAECHEN_OBJEKT );
		}
		else
		if ( childElementTypeName == pSchemaTypeNames->xpLinienGeometrieType )
		{
			childElementTyp = _GEOMETRIE;
			if ( pGmlKlasseP->getGmlObjektTyp() != BLEIT_PLAN && 
				 pGmlKlasseP->getGmlObjektTyp() != PLAN_BEREICH )
				pGmlKlasseP->setGmlObjektTyp ( XP_LINIEN_OBJEKT );
		}
		else
		if ( childElementTypeName == pSchemaTypeNames->xpPunktGeometrieType )
		{
			childElementTyp = _GEOMETRIE;
			if ( pGmlKlasseP->getGmlObjektTyp() != BLEIT_PLAN && 
				 pGmlKlasseP->getGmlObjektTyp() != PLAN_BEREICH )
				pGmlKlasseP->setGmlObjektTyp ( XP_PUNKT_OBJEKT );
		}
    else
    if ( childElementTypeName == pSchemaTypeNames->advTAPointComponentType         ||
      childElementTypeName == pSchemaTypeNames->advTACurveComponentType         ||
      childElementTypeName == pSchemaTypeNames->advTASurfaceComponentType       ||
      childElementTypeName == pSchemaTypeNames->advTAMultiSurfaceComponentType  ||
      childElementTypeName == pSchemaTypeNames->advAGPunktobjektType            ||
      childElementTypeName == pSchemaTypeNames->advAGLinienobjektType           ||
      childElementTypeName == pSchemaTypeNames->advAGFlaechenobjektType         ||
      childElementTypeName == pSchemaTypeNames->advAGObjektType       )

      childElementTyp = _GEOMETRIE;
    else
    if ( childElementTypeName == pSchemaTypeNames->addressPropertyType )  
         childElementTyp = _ADDRESS;  //  "xAL:AddressPropertyType
    else
    if ( childElementTypeName == pSchemaTypeNames->tinPropertyType )   //   "relief:tinPropertyType"
         childElementTyp = _GEOMETRIE;
    else
    if ( childElementTypeName == pSchemaTypeNames->implicitRepresentationPropertyType )   //   "core:implicitRepresentationPropertyType"
         childElementTyp = _GEOMETRIE;
    else
    if ( childElementTypeName == pSchemaTypeNames->abstractTimeSeriesType )
        childElementTyp = ABSTRACT_TIME_SERIES;
    else
    if ( childElementTypeName == pSchemaTypeNames->abstractTimeSeriesType )
      childElementTyp = ABSTRACT_TIME_SERIES;
    else
    if ( childElementTypeName == pSchemaTypeNames->regularTimeSeriesType )
      childElementTyp = REGULAR_TIME_SERIES;
    else
    if ( childElementTypeName == pSchemaTypeNames->regularTimeSeriesFileType )
      childElementTyp = REGULAR_TIME_SERIES_FILE;  
    else
    if ( childElementTypeName == pSchemaTypeNames->irregularTimeSeriesType )
      childElementTyp = IRREGULAR_TIME_SERIES;
    else
    if ( childElementTypeName == pSchemaTypeNames->irregularTimeSeriesFileType )
      childElementTyp = IRREGULAR_TIME_SERIES_FILE;  
    else
    if ( pChildElement->getReferenziertesElement() != "" )
    {
      childElementTyp = _DATA_TYPE;
    }
    else
    if ( childElementTypeName == "" && pChildElement->getUnnamedType() != NULL )  
      {
        vUnnamedChildElements.clear();
        unnamedChildElementAnz = pChildElement->getUnnamedType()->getChildElements( vUnnamedChildElements );
        for ( k = 0; k < unnamedChildElementAnz; k++ )
        {
          GmlSchemaElement * pSchemaElement;
          refName = vUnnamedChildElements[k]->getRefName();

          pSchemaElement = pGmlSchemaElementsP->getElement( refName );
          if ( pSchemaElement != NULL )
          {
            pChildElement->setElementTypeName ( pSchemaElement->getElementTypeName() );
            pChildElement->setElementTypeNamespace(  pSchemaElement->getElementTypeNamespace() );
            if ( !pChildElement->getOptionalElement() )
              pChildElement->setOptionalElement (  pSchemaElement->getOptionalElement() );
            if ( !pChildElement->getMultiElement() )
              pChildElement->setMultiElement (  pSchemaElement->getMultiElement() );
            pChildElement->setDefaultValue( pSchemaElement->getDefaultValue() );
            childElementTyp = _DATA_TYPE;
          }
          else
          if ( refName == "gml:Point"           ||
               refName == "gml:MultiPoint"      ||
               refName == "gml:Curve"           ||
               refName == "gml:MultiCurve"      ||
               refName == "gml:CompositeCurve"  ||
               refName == "gml:Surface"         ||
               refName == "gml:MultiSurface"        )
          {
            childElementTyp = _GEOMETRIE;
            break;
          }
        }
      }
		else
		{
			pChildElementType = pGmlSchemaTypesP->getType ( childElementTypeName );
      if ( pChildElementType != NULL )
      {
        if ( pChildElementType->isSimpleType() ) 
          childElementTyp = _ENUMERATION;
        else
        {
          pChildElementComplexType = (GmlSchemaComplexType*)pChildElementType;
          if ( pChildElementComplexType->getIsReference() == true )
            childElementTyp = _REFERENZ;
          else
          {
            childElementTyp = _DATA_TYPE;
						size_t index = childElementTypeName.rfind( "Property" );
            if ( index >= 0 )
            {
              string referenziertesElement = childElementTypeName.substr( 0, index );
              pChildElement->setReferenziertesElement( referenziertesElement );
            }
          }
        }
      }	
		}

		if ( childElementTyp != _SONST )
		{
			pAttribut = new GmlAttribut ( attributName );
			pAttribut->setGmlAttributTyp (  (ATTRIBUT_TYP)childElementTyp );
      pAttribut->setReferenziertesElement( pChildElement->getReferenziertesElement() );
			pAttribut->setDefaultWert ( pChildElement->getDefaultValue() );
			pAttribut->setOptional ( pChildElement->getOptionalElement() );
      pAttribut->setGmlAttributTypName ( childElementTypeName );
			pAttribut->setMultiple ( pChildElement->getMultiElement() );
      pAttribut->setNillable ( pChildElement->getNillable() );

			if ( childElementTyp == _ENUMERATION || 
				 childElementTyp == _EXTERNAL_CODE_LIST )
			{
				size_t stringLength;
				string stdString;
				string enumerationName;

				if (  childElementTyp == _ENUMERATION ) 
					enumerationName = childElementTypeName;
				else
				if (  childElementTyp == _EXTERNAL_CODE_LIST ||
              childElementTyp == _EXTERNAL_CODE_LIST_GML3_3 ) 
					enumerationName = childElementExternalCodeList;

        if ( enumerationName != "" )
        {
          if (gmlTyp != XPLANGML_4_0   &&
              gmlTyp != XPLANGML_4_1   &&
              gmlTyp != XPLANGML_5_0   &&
              gmlTyp != XPLANGML_5_1   &&
              gmlTyp != XPLANGML_5_2   &&
              gmlTyp != XPLANGML_3_0  )
          {
            stringLength = enumerationName.find ( ":" );
            if ( stringLength != enumerationName.npos )
              enumerationName = enumerationName.substr ( stringLength+1, enumerationName.length() );

            if ( gmlTyp == NAS_5_1  ||
                 gmlTyp == NAS_6_0 )
              {
              stringLength = enumerationName.length();
              stdString    = enumerationName.substr ( stringLength-4, 4 );
              if ( stdString == "Type" )
                enumerationName = enumerationName.substr ( 0, stringLength-4 );
            }
          }

          if ( gmlTyp == XPLANGML_4_0  ||
               gmlTyp == XPLANGML_4_1  ||
               gmlTyp == XPLANGML_5_0  ||
               gmlTyp == XPLANGML_5_1  ||
               gmlTyp == XPLANGML_5_2  ||
               gmlTyp == XPLANGML_3_0  ||
               gmlTyp == XPLANGML_2_0 )
          {
    				stringLength = enumerationName.length();
				    stdString    = enumerationName.substr ( stringLength-4, 4 );
				    if ( stdString == "Type" )
					     enumerationName = enumerationName.substr ( 0, stringLength-4 );
          }
  		    pAttribut->setGmlEnumeration ( enumerationName );
        }
        else
        if ( childElementTypeName == pSchemaTypeNames->gmlCodeType )
        {
          if ( gmlTyp == CITYGML_2_0  )
          {
            enumerationName = GmlNamespaces::getCityGML20CodelistName ( pGmlKlasseP->getGmlKlassenName(), pAttribut->getGmlAttributName() );
            if ( enumerationName != "Unknown")
              pAttribut->setGmlEnumeration( enumerationName );
          }
        }
			}

			pGmlKlasseP->addGmlAttribut ( pAttribut );
		}
    else
    {
      if ( attributName.find ( "GenericApplicationProperty" ) == string::npos )
      {
        m_pFeatures->getLogfile()->writeLogFile ( "Schema-Bestandteil konnte nicht interpretiert werden" );
        m_pFeatures->getLogfile()->writeLogFile ( "    complexTypeName = " + pSchemaComplexTypeP->getName() );
        m_pFeatures->getLogfile()->writeLogFile( "    attributName = " + attributName );
        m_pFeatures->getLogfile()->writeLogFile ( " " );
      }
    }
	}

}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Enumerations-Eintrag zur Enumeration hinzu                    //
///////////////////////////////////////////////////////////////////////////////
void GmlSchema::addEnumerationItems ( GmlEnumeration * pGmlEnumerationP,  GmlSchemaSimpleType * pSimpleTypeP )
{
  unsigned int	 i;
  string         enumerationItem;
  vector<string> enumerationItems;


  pSimpleTypeP->getEnumerationItems ( enumerationItems );
  for ( i = 0; i < enumerationItems.size(); i++ )
  {
    enumerationItem = enumerationItems[i];
    GmlEnumerationItem * pItem = new GmlEnumerationItem ( enumerationItem );
    pGmlEnumerationP->addGmlEnumerationItem ( pItem );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Zugriff aud die Map mit den Ordnernamen der Externen Codelisten          //
///////////////////////////////////////////////////////////////////////////////
std::string GmlSchema::getExternalCodeListsFolder()
{
  return m_pFeatures->getExternalCodeListsFolder( gmlTyp );
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert den Pfadnamen einer ExternalCodeList                           //
///////////////////////////////////////////////////////////////////////////////
std::string GmlSchema::createExternalCodeListsPath ( std::string externalCodeListName )
{ 
  string externalCodeListPath;
	size_t index;

  index = externalCodeListName.find( ':' );
  if ( index > 0 )
    externalCodeListName.replace ( index, 1, "_" );

  externalCodeListPath = getExternalCodeListsFolder () + externalCodeListName + ".xml";   
  return externalCodeListPath;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert den Standard-Pfadnamen der Toolbox einer ExternalCodeList      //
///////////////////////////////////////////////////////////////////////////////
std::string GmlSchema::createDefaultExternalCodeListsPath ( std::string externalCodeListName )
{
  string externalCodeListPath;
	size_t index;

  if ( externalCodeListName == "" ) return "";

  index = externalCodeListName.find( ':' );
  if ( index >= 0 )
    externalCodeListName.replace ( index, 1, "_" );

  externalCodeListPath = Constant::getSystemFolder() + getDefaultCodeListsFolder() + externalCodeListName + ".xml";   
  return externalCodeListPath;
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML 2.0                                                          //
//  Generiert für CityGML 2.0 Codelisten den Standardpfad                    //
///////////////////////////////////////////////////////////////////////////////
string GmlSchema::createCityGML20CodeListPath ( GmlKlasse * pGmlKlasse, GmlAttribut * pGmlAttribut )
{
  string path                 = m_pFeatures->getExternalCodeListsFolder( CITYGML_2_0 );
  string klassenNameQualified = pGmlKlasse->getGmlKlassenName();
  string attributNameQualified= pGmlAttribut->getGmlAttributName();
  string namespaceKuerzel;
  string namespaceKuerzelAttribut;
  string klassenName;
  string attributName;
  string moduleFolder;
  string codelistName;
	size_t index;

  index = klassenNameQualified.find( ":" );
  namespaceKuerzel = klassenNameQualified.substr( 0, index );
  klassenName      = klassenNameQualified.substr ( index + 1, klassenNameQualified.length() - index );

  moduleFolder = GmlNamespaces::getCityGML20CodelistModuleFolder ( namespaceKuerzel );
  codelistName = GmlNamespaces::getCityGML20CodelistName ( klassenNameQualified, attributNameQualified );

  if ( codelistName != "Unknown" )
  {
    pGmlAttribut->setGmlEnumeration( codelistName );
    path = path + moduleFolder + codelistName + ".xml";
  }
  else
  {
    pGmlAttribut->setGmlEnumeration( "" );
    path = "";
  }

  return path;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert true, wenn das GML-Schema extern referierte CodeList-Dictionaries//
// unterstützt. Dies ist nicht der Fall für XPlanGML 2.0 und 3.1,            //
//  CityGML 0.4.0 und 1.0, sowie NAS 5.1 und 5.1.1                           //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchema::externalCodeListDictionariesExist()
{
  if ( gmlTyp == XPLANGML_2_0  || 
       gmlTyp == XPLANGML_3_0  ||
       gmlTyp == CITYGML_0_4_0 ||
       gmlTyp == CITYGML_1_0   ||
       gmlTyp == NAS_5_1       ||
       gmlTyp == NAS_5_1_1 )
       return false;
  else
    return true;
}


///////////////////////////////////////////////////////////////////////////////
//  GmlKlasse                                                                //
//  Modelliert einen GML FeatureType oder DataType , abgeleitet das den      //
//  globalen xs:complexType Elementen der Schema-Dateien                     //
///////////////////////////////////////////////////////////////////////////////
GmlKlasse::GmlKlasse ( string klassenNameP )
{
	gmlKlassenName = klassenNameP;
	gmlObjektTyp   = ANY_OBJEKT;
	planTyp        = KEIN_XPLAN;
	abstract       = false;
  isGmlFeature     = false;
}

GmlKlasse::~GmlKlasse ()
{
	map<string,GmlAttribut*>::iterator iter;
  unsigned int                       i;

	for ( iter = gmlAttribute.begin(); iter != gmlAttribute.end(); iter++ )
		delete iter->second;

  for ( i = 0; i < vGmlSchemaAttribute.size(); i++ )
    delete vGmlSchemaAttribute[i];
}
///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines GML-Attributes										                    	 //
///////////////////////////////////////////////////////////////////////////////
void GmlKlasse::addGmlAttribut ( GmlAttribut * pGmlAttributP )
{
	string name = pGmlAttributP->getGmlAttributName();
	gmlAttribute[name] = pGmlAttributP;

	vGmlAttribute.push_back ( pGmlAttributP );
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines GML-SchemaAttributes							                    	 //
///////////////////////////////////////////////////////////////////////////////
void GmlKlasse::addGmlSchemaAttribut ( GmlSchemaAttribut * pGmlSchemaAttributP )
{
  vGmlSchemaAttribute.push_back ( pGmlSchemaAttributP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle GML-Attribute											                      	 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlKlasse::getGmlAttribute ( std::vector<GmlAttribut*> & gmlAttributeP )
{
	size_t i;

	for ( i = 0; i < vGmlAttribute.size(); i++ )
	{
		GmlAttribut * pGmlAttribut = vGmlAttribute[i];
		gmlAttributeP.push_back ( pGmlAttribut );
	}
	return vGmlAttribute.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle GML-SchemaAttribute							                      		 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlKlasse::getGmlSchemaAttribute ( std::vector<GmlSchemaAttribut*> & gmlSchemaAttributeP )
{
	size_t i;

  for ( i = 0; i < vGmlSchemaAttribute.size(); i++ )
  {
    GmlSchemaAttribut * pGmlSchemaAttribut = vGmlSchemaAttribute[i];
    gmlSchemaAttributeP.push_back ( pGmlSchemaAttribut );
  }
  return vGmlSchemaAttribute.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein GML-Attribut						 			                        			 //
///////////////////////////////////////////////////////////////////////////////
GmlAttribut * GmlKlasse::getGmlAttribut ( std::string gmlAttributNameP )
{
	map<string,GmlAttribut*>::iterator iter;

	size_t index = gmlAttributNameP.find( "." );
  if ( index != string::npos )
    gmlAttributNameP = gmlAttributNameP.substr( 0, index );

	iter = gmlAttribute.find ( gmlAttributNameP );
	if ( iter != gmlAttribute.end() ) return iter->second;
	else							  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein GML-SchemaAttribut			                  									 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaAttribut * GmlKlasse::getGmlSchemaAttribut ( std::string gmlSchemaAttributNameP )
{
  unsigned int i;

  for ( i = 0; i < vGmlSchemaAttribute.size(); i++ )
  {
    GmlSchemaAttribut * pGmlSchemaAttribut = vGmlSchemaAttribute[i];
    if ( pGmlSchemaAttribut->getAttributName() == gmlSchemaAttributNameP )
      return pGmlSchemaAttribut;
  }

  return NULL;
}
///////////////////////////////////////////////////////////////////////////////
//  Überprüft ob der spez. Typname ein ParentType Name ist			        		 //
///////////////////////////////////////////////////////////////////////////////
bool GmlKlasse::isParentTypeName ( std::string typeName )
{
	unsigned int i;

	for ( i = 0; i < parentTypeNames.size(); i++ )
	{
		if ( parentTypeNames[i] == typeName ) return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle ParentType Namen									                        	 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlKlasse::getParentTypeNames ( vector<string> &parentTypeNamesP )
{
	size_t i;

	for ( i = 0; i < parentTypeNames.size(); i++ )
		parentTypeNamesP.push_back ( parentTypeNames[i] );

	return parentTypeNamesP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Namen des zug. GML-Typs					                  					 //
///////////////////////////////////////////////////////////////////////////////
std::string GmlKlasse::getGmlTypName()
{
	if ( parentTypeNames.size() > 0 )
		return parentTypeNames[parentTypeNames.size()-1];
	else
		return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Namen eines GML-Elements vom Typ                             //
///////////////////////////////////////////////////////////////////////////////
string GmlKlasse::getGmlElementName()
{
  string elementName, suffix;

  suffix = gmlKlassenName.substr( gmlKlassenName.length()-4, 4 );
  if ( suffix == "Type" )
    return gmlKlassenName.substr( 0, gmlKlassenName.length()-4 );
  else
    return gmlKlassenName;
}

///////////////////////////////////////////////////////////////////////////////
//  GmlElement                                                               //
//  Modelliert ein GML-Element, abgeleitet aus den globalen xs:element       //
//  Elementen der Schema-Dateien                                             //
///////////////////////////////////////////////////////////////////////////////
GmlElement::GmlElement ( std::string elementNameP )
{
  gmlElementName = elementNameP;
  _abstract      = false;
}

GmlElement::~ GmlElement()
{

}

///////////////////////////////////////////////////////////////////////////////
//  GmlAttribut                                                              //
//  Modelliert ein Attribut einer GML Feature-Klasse		            				 //
///////////////////////////////////////////////////////////////////////////////
GmlAttribut::GmlAttribut ( std::string attributNameP )
{
	gmlAttributName       = attributNameP;
	gmlAttributTyp        = _SONST;
	optional		      = true;
	multiple			  = false;
  nilable     		  = false;
	defaultWert		      = "";
  referenziertesElement = _T("");
	gmlEnumeration        = "";
  gmlAttributTypName    = "";
}

GmlAttribut::~GmlAttribut()
{

}

///////////////////////////////////////////////////////////////////////////////
//  GmlEnumeration                                                           //
//  Modelliert eine GML-Enumeration oder Codeliste             	 						 //
///////////////////////////////////////////////////////////////////////////////
GmlEnumeration::GmlEnumeration ( std::string enumerationNameP, bool isExternalCodeListP )
{
	gmlEnumerationName = enumerationNameP;
	isExternalCodeList = isExternalCodeListP;
}

GmlEnumeration::~GmlEnumeration()
{
	map<string,GmlEnumerationItem*>::iterator iter;

	for ( iter = gmlEnumerationItems.begin(); iter != gmlEnumerationItems.end(); iter++ )
		delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Items					                          									 //
///////////////////////////////////////////////////////////////////////////////
void GmlEnumeration::clear()
{
	map<string,GmlEnumerationItem*>::iterator iter;

	for ( iter = gmlEnumerationItems.begin(); iter != gmlEnumerationItems.end(); iter++ )
		delete iter->second;

	gmlEnumerationItems.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Enumeration-Item hinzu					                      							 //
///////////////////////////////////////////////////////////////////////////////
void GmlEnumeration::addGmlEnumerationItem( GmlEnumerationItem * pItemP )
{
	string name = pItemP->getGmlEnumerationItemName();

	gmlEnumerationItems[name] = pItemP;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Enumeration-Items						                    					 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlEnumeration::getEnumerationItems ( std::vector<GmlEnumerationItem*> &gmlEnumerationItemsP )
{
	std::map<std::string,GmlEnumerationItem*>::iterator iter;

	for ( iter = gmlEnumerationItems.begin(); iter != gmlEnumerationItems.end(); iter++ )
	{
		GmlEnumerationItem * pItem = iter->second;
		gmlEnumerationItemsP.push_back ( pItem );
	}

	return gmlEnumerationItems.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Namen aller Enumeration-Items	                    					 //
///////////////////////////////////////////////////////////////////////////////
size_t GmlEnumeration::getEnumerationItemNames ( std::vector<std::string> &gmlEnumerationItemNamesP )
{
	std::map<std::string,GmlEnumerationItem*>::iterator iter;

	for ( iter = gmlEnumerationItems.begin(); iter != gmlEnumerationItems.end(); iter++ )
	{
		GmlEnumerationItem * pItem = iter->second;
		gmlEnumerationItemNamesP.push_back ( pItem->getGmlEnumerationItemName() );
	}

	return gmlEnumerationItems.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Enumeration-Item							                      				 //
///////////////////////////////////////////////////////////////////////////////
GmlEnumerationItem * GmlEnumeration::getEnumerationItem ( std::string itemNameP )
{
	map<string,GmlEnumerationItem*>::iterator iter;

	iter = gmlEnumerationItems.find ( itemNameP );
	if ( iter != gmlEnumerationItems.end() ) return iter->second;
	else									 return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  GmlEnumerationItem                                                       //
//  Modelliert einen Eintrag einer Enumeration  oder einer  Codeliste        //
///////////////////////////////////////////////////////////////////////////////
GmlEnumerationItem::GmlEnumerationItem ( std::string enumerationItemNameP )
{
	gmlEnumerationItemName = enumerationItemNameP;
}

GmlEnumerationItem::~GmlEnumerationItem()
{

}

///////////////////////////////////////////////////////////////////////////////
//  GmlSchemaReader								                                					 //
//  Liest die XML-Schema-Dateien eines GML-Applikationsschemas ein           //
//  und analysiert Elemente und Typen	         		                           //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaReader::GmlSchemaReader ( GmlSchemaTypes    * pGmlSchemaTypesP, 
 					                         GmlSchemaElements * pGmlSchemaElementsP,
                                   GmlSchemaGroups   * pGmlSchemaGroupsP,
                                   GmlNamespaces     * pGmlNamespacesStandardP   )
{
	pGmlSchemaTypes		 = pGmlSchemaTypesP;
	pGmlSchemaElements	 = pGmlSchemaElementsP;
  pGmlSchemaGroups    	 = pGmlSchemaGroupsP;
  pGmlNamespacesStandard = pGmlNamespacesStandardP;
  pGmlNamespaces         = NULL;
}

GmlSchemaReader::~GmlSchemaReader()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer Schema-Datei   			                                		 //
///////////////////////////////////////////////////////////////////////////////
bool GmlSchemaReader::read( std::string fileName )
{
  String ^ STR;
  String         ^ QUALIFIED_NAME;
  String         ^ VERSION = nullptr;
  std::string      stdString;
  bool		         firstElement = true;
  bool             success;

	GmlSchemaElement     * pElement = nullptr;
  GmlSchemaGroup       * pGroup;
	GmlSchemaComplexType * pComplexType;
	GmlSchemaSimpleType  * pSimpleType;

	System::String ^ fName = gcnew System::String( fileName.c_str() );
	reader                 = gcnew System::Xml::XmlTextReader( fName );

  if ( !File::Exists ( fName ) )
  {
    MessageBox::Show ( "GML-Schema not found" );
    return false;
  }

  if ( pGmlNamespaces == NULL )
    pGmlNamespaces = new GmlNamespaces();

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
   	  pActElement = reader->LocalName;
	  if ( (firstElement) )
	  {
        if ( pActElement->CompareTo("schema") != 0 )
        {
		  MessageBox::Show( "Gelesene Datei enthält kein XML-Schema");
		  return false;
	    }
						
        while ( reader->MoveToNextAttribute() )
        {
          String ^ATTRIBUT_NAME;
          String ^ATTRIBUT_WERT;
          String ^XMLNS = "xmlns";
          String ^KUERZEL;
          string  attributName;
          string  attributWert;
          string  kuerzel;
          string  defaultNamespace;

          ATTRIBUT_NAME = reader->Name;
          ATTRIBUT_WERT = reader->Value;
          QuConvert::systemStr2stdStr ( attributWert, ATTRIBUT_WERT );

          if ( ATTRIBUT_NAME->Contains ( XMLNS ) )
          {
            if ( ATTRIBUT_NAME->Length >= 6 )
            {
              KUERZEL = ATTRIBUT_NAME->Substring ( 6 );
              QuConvert::systemStr2stdStr ( kuerzel, KUERZEL );
              pGmlNamespaces->addNamespace ( kuerzel, attributWert );
            }
            else
            {
              QuConvert::systemStr2stdStr ( defaultNamespace, ATTRIBUT_WERT );
              pGmlNamespaces->setDefaultNamespace( defaultNamespace );
            }
          }
          else
          if ( ATTRIBUT_NAME->CompareTo ( "targetNamespace" ) == 0 )
          {
            TARGET_NAMESPACE = ATTRIBUT_WERT;
          }   
          else
          if ( ATTRIBUT_NAME->CompareTo ( "version" ) == 0 )
            VERSION = ATTRIBUT_WERT;
        }
        
        firstElement = false;
			}

			if (pActElement->CompareTo( "element" ) == 0 )
			{
        pElement = new GmlSchemaElement ();

				STR = reader->GetAttribute( "name" );
        if ( STR != nullptr )
        {
          QUALIFIED_NAME = generateStandardTargetName ( STR );
				  QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
				  pElement->setName ( stdString );
				  success = pGmlSchemaElements->addElement ( pElement );
        }

        if ( !success )
        {
          delete pElement;
          pElement = NULL;
          continue;
        }

        STR = reader->GetAttribute( "ref" );
        if ( STR != nullptr )
        {
          MessageBox::Show( "Fehlerhaftes GML-Schema");
          return false;
        }

				STR = reader->GetAttribute( "type" );
				if ( STR != nullptr )
				{
          QUALIFIED_NAME = generateStandardName( STR );
					QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
					pElement->setType ( stdString );
				}

        STR = reader->GetAttribute( "substitutionGroup" );
        if ( STR != nullptr )
        {
          QUALIFIED_NAME = generateStandardName( STR );
          QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
          pElement->setSubstitutionGroup ( stdString );
          pGmlSchemaElements->addSubstitutionGroup ( pElement );
        }

				STR = reader->GetAttribute( "default" );
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pElement->setDefaultValue ( stdString );
				}

				STR = reader->GetAttribute( "minOccurs" );
				if ( STR != nullptr )
				{
					int i = Convert::ToInt16 ( STR );
					if ( i == 0 ) pElement->setOptionalElement ( true );
					else		  pElement->setOptionalElement ( false );				
				}
				else pElement->setOptionalElement ( false );

				STR = reader->GetAttribute( "maxOccurs" );
				if ( STR != nullptr )
				{
					if ( STR->CompareTo( "0" ) == 0 ) pElement->setMultiElement ( false );
					else							  pElement->setMultiElement ( true );				
				}

				STR = reader->GetAttribute( "abstract" );
				if ( STR != nullptr )
				{
					if ( STR->CompareTo( "true" ) == 0 ) pElement->setAbstractElement ( true );
					else							     pElement->setAbstractElement ( false );				
				}
				else pElement->setAbstractElement ( false );

        STR = reader->GetAttribute( "nillable" );
        if ( STR != nullptr )
        {
          if ( STR->CompareTo( "true" ) == 0 )
            pElement->setNillable ( true );
          else							                   pElement->setNillable ( false );				
        }
        else
          pElement->setNillable ( false );

        if ( !reader->IsEmptyElement ) 
          readGmlSchemaElement( pElement );
			}

			if ( pActElement->CompareTo( "referenziertesElement" ) == 0 || 
				 pActElement->CompareTo( "targetElement" ) == 0 )
			{
				STR = reader->ReadString ();
				if ( STR != nullptr && pElement != NULL )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pElement->setReferenziertesElement ( stdString );
				}
			}


      if (pActElement->CompareTo( "group" ) == 0 )
      {
        STR = reader->GetAttribute( "name" );
        QUALIFIED_NAME = generateStandardTargetName ( STR );
        QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
        pGroup = new GmlSchemaGroup ();
        pGroup->setName ( stdString );
        pGmlSchemaGroups->addGroup ( pGroup );

        readGmlSchemaGroup ( pGroup );
      }

			if (pActElement->CompareTo( "complexType" ) == 0 )
			{
				STR = reader->GetAttribute( "name" );
        QUALIFIED_NAME = generateStandardTargetName ( STR );
				QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
				pComplexType = new GmlSchemaComplexType ( stdString );
				success = pGmlSchemaTypes->addType ( pComplexType );
        if ( !success ) 
        {
          delete pComplexType;
          pComplexType = NULL;
          continue;
        }

        STR = reader->GetAttribute( "abstract" );
        if ( STR != nullptr )
        {
          if ( STR->CompareTo( "true" ) == 0 )
            pComplexType->setIsAbstract( true );
        }

        if ( !reader->IsEmptyElement )
				  readGmlSchemaComplexType ( pComplexType );
			}

			if ( pActElement->CompareTo( "simpleType" ) == 0 )
			{
				STR = reader->GetAttribute( "name" );
        QUALIFIED_NAME = generateStandardTargetName ( STR );
				QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
				pSimpleType = new GmlSchemaSimpleType ( stdString );
				success = pGmlSchemaTypes->addType ( pSimpleType );
        if ( !success )
        {
          delete pSimpleType;
          pSimpleType = NULL;
          continue;
        }
				readGmlSchemaSimpleType ( pSimpleType );
			}
		}
		else
		if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
		{
			pActElement = reader->LocalName;
			if ( pActElement->CompareTo( "schema" ) == 0 ) break;
		}
	}

  delete pGmlNamespaces;
  pGmlNamespaces = NULL;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Schließt den Reader und gibt das eingelesene Dokument frei               //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaReader::closeReader()
{
  if ( reader != nullptr ) reader->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines xs:element 		                      										 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaReader::readGmlSchemaElement( GmlSchemaElement * pElementP )
{
	String                  ^ STR;
	string                    stdString;
	string                    externalCodeListName;
	size_t                    anz;
  vector<GmlSchemaElement*> vChildElements;
  GmlSchemaElement        * pChildElement;

	while( reader->Read() )
	{
		if ( reader->NodeType == System::Xml::XmlNodeType::Element )
		{
			pActElement = reader->LocalName;

			if ( pActElement->CompareTo("complexType") == 0 )
			{
				stdString = "";
				GmlSchemaComplexType * pUnnamedType = new GmlSchemaComplexType ( stdString );

        STR = reader->GetAttribute( "abstract" );
        if ( STR != nullptr )
        {
          if ( STR->CompareTo( "true" ) == 0 )
            pUnnamedType->setIsAbstract( true );
        }

				pElementP->setUnnamedType ( pUnnamedType );
				readGmlSchemaComplexType ( pUnnamedType );
        if ( pUnnamedType->getIsComplex() == false )
        {
          pElementP->setElementTypeName( pUnnamedType->getParantTypeName() );
          pElementP->setElementTypeNamespace( pUnnamedType->getParantTypeNamespace() );
        }
        else
        {
          vChildElements.clear();
          anz = pUnnamedType->getChildElements( vChildElements );
          if ( anz == 1 )
          {
            pChildElement = vChildElements[0];
            pElementP->setReferenziertesElement( pChildElement->getRefName() );
          }
          else
          if ( anz == 0 && pUnnamedType->getParantTypeName() != "" )
          {
            if ( pUnnamedType->getIsReference() == true  )
              pElementP->setType( "gml:ReferenceType" );
            else
            {
              pElementP->setElementTypeName( pUnnamedType->getParantTypeName() );
              pElementP->setElementTypeNamespace( pUnnamedType->getParantTypeNamespace() );
            }
          }

        }
			}

			if ( pActElement->CompareTo("annotation") == 0 )
			{
				continue;

			}

			if ( pActElement->CompareTo("appinfo") == 0 )
			{
				continue;
			}

			if ( pActElement->CompareTo( "referenziertesElement" )  == 0 || 
				 pActElement->CompareTo( "targetElement" )== 0 )
			{
				STR = reader->ReadString ();
				if ( STR != nullptr  )
				{
					QuConvert::systemStr2stdStr ( stdString, STR );
					pElementP->setReferenziertesElement ( stdString );
				}
			}

			if ( pActElement->CompareTo ( "referenzierteCodeList") == 0 || 
           pActElement->CompareTo ( "referenzierteCodelist") == 0 ||
           pActElement->CompareTo( "targetCodeList")         == 0 ) 
			{
				STR = reader->ReadElementString ();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( externalCodeListName, STR );
					pElementP->setExternalCodeListName ( externalCodeListName );
				}
			}

		}
		else
		if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
		{
			pActElement = reader->LocalName;
			if ( pActElement->CompareTo( "element" ) == 0 ) break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines xs:complexType   			                    							 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaReader::readGmlSchemaComplexType( GmlSchemaComplexType * pTypeP )
{
	System::String   ^ STR;
  System::String           ^ QUALIFIED_NAME;
	std::string        stdString;
  GmlSchemaElement * pElement;

	while( reader->Read() )
	{
		if ( reader->NodeType == System::Xml::XmlNodeType::Element )
		{
			pActElement = reader->LocalName;

			if ( pActElement->CompareTo("documentation") == 0 )
			{
				STR = reader->ReadString ();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR  );
					pTypeP->setComment ( stdString );
				}
			}

      if ( pActElement->CompareTo("attribute") == 0 )
        readGmlSchemaAttribut ( pTypeP, false );

      if ( pActElement->CompareTo("attributeGroup") == 0 )
        readGmlSchemaAttribut ( pTypeP, true );

			if ( pActElement->CompareTo("complexContent") == 0 )
				readComplexContent ( pTypeP );

      if ( pActElement->CompareTo( "simpleContent") == 0 )
        readSimpleContent ( pTypeP );

      if ( pActElement->CompareTo("element") == 0 )
      {
        pElement = readElement();
        if ( pElement !=  NULL )
          pTypeP->addElement ( pElement );
      }

      if (  pActElement->CompareTo("group") == 0 )
      {
        GmlSchemaGroupRef * pGroupRef = new GmlSchemaGroupRef;
        STR = reader->GetAttribute( "ref" );
        QUALIFIED_NAME = generateStandardName ( STR );
        QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
        pGroupRef->setRefName ( stdString );
        pTypeP->addElement ( pGroupRef );
      }
  	}
		else
		if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
		{
			pActElement = reader->LocalName;
			if ( pActElement->CompareTo( "complexType" ) == 0 ) break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen des xs:complexContent innerhalb eines xs:complexType      			 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaReader::readComplexContent (  GmlSchemaComplexType * pTypeP )
{
	System::String   ^ STR;
  String           ^ QUALIFIED_NAME;
	std::string        stdString;
  std::string        derivationType;
  GmlSchemaElement * pElement = nullptr;

	while( reader->Read() )
	{
		if ( reader->NodeType == System::Xml::XmlNodeType::Element )
		{
			pActElement = reader->LocalName;

      if ( pActElement->CompareTo("extension") == 0 || 
        pActElement->CompareTo("restriction") == 0 )
			{
        QuConvert::systemStr2stdStr ( derivationType, pActElement );
        STR = reader->GetAttribute( "base" );
        QUALIFIED_NAME = generateStandardName ( STR );
        QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
        pTypeP->setParant ( stdString );

        if ( derivationType == "restriction" && 
           ( stdString == "gml:AssociationType" ||   stdString == "gml:ReferenceType" )  ) 
          pTypeP->setIsReference ( true );
			}

      if ( pActElement->CompareTo("element") == 0 )
      {
        pElement = readElement();
        if ( pElement != NULL ) pTypeP->addElement ( pElement );
      }

      if (  pActElement->CompareTo("group") == 0 )
      {
        GmlSchemaGroupRef * pGroupRef = new GmlSchemaGroupRef;
        STR = reader->GetAttribute( "ref" );
        QUALIFIED_NAME = generateStandardTargetName ( STR );
        QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
        pGroupRef->setRefName ( stdString );
        pTypeP->addElement ( pElement );
      }

      if ( pActElement->CompareTo("attributeGroup") == 0 )
      {
        STR = reader->GetAttribute ( "ref" );
        if ( STR->CompareTo ( "gml:AssociationAttributeGroup" ) == 0 )
          pTypeP->setIsReference ( true );     
      }
		}
		else
		if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
		{
			pActElement = reader->LocalName;
			if ( pActElement->CompareTo( "complexContent" ) == 0 ) break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen des xs:simpleContent innerhalb eines xs:complexType      			 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaReader::readSimpleContent (  GmlSchemaComplexType * pTypeP )
{
  System::String   ^ STR;
  String           ^ QUALIFIED_NAME;
  std::string        stdString;

  pTypeP->setComplex( false );

  while( reader->Read() )
  {
    if ( reader->NodeType == System::Xml::XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("extension") == 0 )
      {
        STR = reader->GetAttribute( "base" );
        QUALIFIED_NAME = generateStandardName ( STR );
        QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
        pTypeP->setParant ( stdString );
      }

      if ( pActElement->CompareTo("attribute") == 0 )
        readGmlSchemaAttribut ( pTypeP, false );

      if ( pActElement->CompareTo("attributeGroup") == 0 )
      {
        STR = reader->GetAttribute ( "ref" );
        if ( STR->CompareTo ( "gml:AssociationAttributeGroup" ) == 0 )
          pTypeP->setIsReference ( true );     
      }
    }
    else
      if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if ( pActElement->CompareTo( "simpleContent" ) == 0 ) break;
      }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer xs:group                              										 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaReader::readGmlSchemaGroup( GmlSchemaGroup * pGroupP )
{
  System::String   ^ STR;
  System::String   ^ QUALIFIED_NAME;
  std::string        stdString;
  GmlSchemaElement * pElement;

  if ( reader->IsEmptyElement ) return;

  while( reader->Read() )
  {
    if ( reader->NodeType == System::Xml::XmlNodeType::Element )
    {
      pActElement = reader->LocalName;

      if ( pActElement->CompareTo("documentation") == 0 )
      {
        STR = reader->ReadString ();
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR  );
          pGroupP->setComment ( stdString );
        }
      }

      if ( pActElement->CompareTo("sequence") == 0 )
      {
        pGroupP->setGroupType ( GmlSchemaGroup::SEQUENCE );
      }

      if ( pActElement->CompareTo("choice") == 0 )
      {
        pGroupP->setGroupType ( GmlSchemaGroup::CHOICE );
      }

      if ( pActElement->CompareTo("all") == 0 )
      {
        pGroupP->setGroupType ( GmlSchemaGroup::ALL );
      }

      if ( pActElement->CompareTo("element") == 0 )
      {
        pElement = readElement ();
        if ( pElement != NULL )
          pGroupP->addSchemaElement ( pElement );
      }

      if (  pActElement->CompareTo("group") == 0 )
      {
        GmlSchemaGroupRef * pGroupRef = new GmlSchemaGroupRef;
        STR = reader->GetAttribute( "ref" );
        QUALIFIED_NAME = generateStandardName ( STR );
        QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
        pGroupRef->setRefName ( stdString );
        pGroupP->addSchemaElement ( pGroupRef );
      }
    }
    else
      if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
      {
        pActElement = reader->LocalName;
        if ( pActElement->CompareTo( "group" ) == 0 ) break;
      }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen und liefern eines untergeordneten xs:elements     							 //
///////////////////////////////////////////////////////////////////////////////
GmlSchemaElement * GmlSchemaReader::readElement ()
{
  String           ^ STR;
  String           ^ QUALIFIED_NAME;
  std::string        stdString;
  GmlSchemaElement * pElement;

  pElement = new GmlSchemaElement ();

  STR = reader->GetAttribute( "name" );
  if ( STR != nullptr )
  {
    QUALIFIED_NAME = generateStandardTargetName ( STR );
    QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
    pElement->setName ( stdString );
  }

  STR = reader->GetAttribute( "ref" );
  if ( STR != nullptr )
  {
    QUALIFIED_NAME = generateStandardName ( STR );
    QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
    pElement->setRefName ( stdString );
  }

  STR = reader->GetAttribute( "type" );
  if ( STR != nullptr )
  {
    QUALIFIED_NAME = generateStandardName ( STR );
    QuConvert::systemStr2stdStr ( stdString, QUALIFIED_NAME );
    pElement->setType ( stdString );
  }

  STR = reader->GetAttribute( "default" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pElement->setDefaultValue ( stdString );
  }

  STR = reader->GetAttribute( "minOccurs" );
  if ( STR != nullptr )
  {
    int i = Convert::ToInt16 ( STR );
    if ( i == 0 ) pElement->setOptionalElement ( true );
    else		  pElement->setOptionalElement ( false );				
  }
  else pElement->setOptionalElement ( false );

  STR = reader->GetAttribute( "maxOccurs" );
  if ( STR != nullptr )
  {
    if ( STR->CompareTo( "0" ) == 0 ) pElement->setMultiElement ( false );
    else							  pElement->setMultiElement ( true );				
  }
  else pElement->setMultiElement ( false );

  STR = reader->GetAttribute( "nillable" );
  if ( STR != nullptr )
  {
    if ( STR->CompareTo( "true" ) == 0 ) pElement->setNillable ( true );
    else							                   pElement->setNillable ( false );				
  }
  else pElement->setNillable ( false );

  if ( !reader->IsEmptyElement ) 
    readGmlSchemaElement( pElement );

  return pElement;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines xs:attribute eines xs:complexType                         //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaReader::readGmlSchemaAttribut (  GmlSchemaComplexType * pTypeP, bool isGroupP )
{
  String ^ STR;
  string   stdString;

  GmlSchemaAttribut * pAttribut = new GmlSchemaAttribut;
  pAttribut->setIsGroup ( isGroupP );
  pTypeP->addAttribut ( pAttribut );

  STR = reader->GetAttribute ( "name" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pAttribut->setAttributName ( stdString );
  }

  STR = reader->GetAttribute ( "type" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pAttribut->setAttributType ( stdString );
  }

  STR = reader->GetAttribute ( "use" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    if ( stdString == "required" )
      pAttribut->setOptionalElement ( false );
    else
      pAttribut->setOptionalElement ( true );
  }

  STR = reader->GetAttribute ( "default" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pAttribut->setDefaultValue ( stdString );
  }

  STR = reader->GetAttribute ( "ref" );
  if ( STR != nullptr )
  {
    QuConvert::systemStr2stdStr ( stdString, STR );
    pAttribut->setRefName ( stdString );
    pAttribut->setAttributName ( stdString );
    if ( stdString == "gml:AssociationAttributeGroup" )
      pTypeP->setIsReference ( true );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines xs:simpleType  								                    			 //
///////////////////////////////////////////////////////////////////////////////
void GmlSchemaReader::readGmlSchemaSimpleType( GmlSchemaSimpleType * pTypeP )
{
	System::String   ^ STR;
	std::string        stdString;

	while( reader->Read() )
	{
		if ( reader->NodeType == System::Xml::XmlNodeType::Element )
		{
			pActElement = reader->LocalName;

			if ( pActElement->CompareTo("documentation") == 0 )
			{
				STR = reader->ReadString ();
				if ( STR != nullptr )
				{
					QuConvert::systemStr2stdStr ( stdString, STR  );
					pTypeP->setComment ( stdString );
				}
			}

			if ( pActElement->CompareTo("enumeration") == 0 )
			{
				STR = reader->GetAttribute( "value" );
				QuConvert::systemStr2stdStr ( stdString, STR );
				pTypeP->addEnumerationItem ( stdString );
			}
		}
		else
		if ( reader->NodeType == System::Xml::XmlNodeType::EndElement )
		{
			pActElement = reader->LocalName;
			if ( pActElement->CompareTo( "simpleType" ) == 0 ) break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert den qualifizierten Namen des tartetNamespace mit dem           //
//  Standard-Kürzel des Schemas                                              //
///////////////////////////////////////////////////////////////////////////////
String ^ GmlSchemaReader::generateStandardTargetName ( String ^ NAME )
{
  string   targetNamespace;
  string   kuerzel;
  int      index;
  String ^ NEW_NAME;

  QuConvert::systemStr2stdStr( targetNamespace, TARGET_NAMESPACE );
  kuerzel = pGmlNamespacesStandard->getKuerzel( targetNamespace );
  if ( kuerzel == "" )
  {
    kuerzel = pGmlNamespaces->getKuerzel ( targetNamespace );
    if ( kuerzel != "" )
      pGmlNamespacesStandard->addNamespace ( kuerzel, targetNamespace );
  }

  if ( NAME->Contains(":") )
  {
    index = NAME->IndexOf ( ":" );
    NAME = NAME->Substring ( index + 1 );
  }

  if ( kuerzel != "" )
    NEW_NAME = String::Concat( QuConvert::ToString( kuerzel), ":", NAME );
  else
    NEW_NAME = NAME;
  return NEW_NAME;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert einen qualifizierten Namen  mit dem                            //
//  Standard-Kürzel des Schemas                                              //
///////////////////////////////////////////////////////////////////////////////
String ^ GmlSchemaReader::generateStandardName ( String ^ NAME )
{
  int      index;
  String ^ KUERZEL;
  String ^ LOCAL_NAME;
  String ^ NEW_NAME;
  string   kuerzel;
  string   namespaceName;
  string   standardKuerzel;

  if ( NAME->Contains(":") )
  {
    index      = NAME->IndexOf ( ":" );
    KUERZEL    = NAME->Substring( 0, index );
    LOCAL_NAME = NAME->Substring( index+1 );

    QuConvert::systemStr2stdStr( kuerzel, KUERZEL );
    namespaceName = pGmlNamespaces->getNamespace( kuerzel );
    standardKuerzel = pGmlNamespacesStandard->getKuerzel ( namespaceName );
    if ( standardKuerzel == "" )
    {
      pGmlNamespacesStandard->addNamespace( kuerzel, namespaceName );
      standardKuerzel = kuerzel;
    }
  }
  else
  {
    LOCAL_NAME      = NAME;
    standardKuerzel = pGmlNamespacesStandard->getKuerzel ( pGmlNamespaces->getDefaultNamespace() );
  }

  NEW_NAME = String::Concat( QuConvert::ToString( standardKuerzel ), ":", LOCAL_NAME );
  return NEW_NAME;
}




