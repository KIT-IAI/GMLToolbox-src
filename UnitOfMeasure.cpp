#include "stdafx.h"

#include "UnitOfMeasure.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  Class UOMList                                                            //
//  Liste/Map on Unit of Measure Beschreibungen                              //
///////////////////////////////////////////////////////////////////////////////
UOMList::UOMList()
{

}

UOMList::~UOMList()
{
  for ( unsigned int i = 0; i < vUOM.size(); i++ )
    delete vUOM[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein UOM-Objekt auf Basis des Identifikators                      //
///////////////////////////////////////////////////////////////////////////////
UOM * UOMList::getUOMFromIdentifier ( std::string identifier )
{
  map<string,UOM*>::iterator pos;

  pos = mUOMFromIdentifier.find ( identifier );
  if ( pos != mUOMFromIdentifier.end() )
    return pos->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein UOM-Objekt auf Basis des Namens                              //
///////////////////////////////////////////////////////////////////////////////
UOM * UOMList::getUOMFromName ( std::string name )
{
  map<string,UOM*>::iterator pos;

  pos = mUOMFromName.find ( name );
  if ( pos != mUOMFromName.end() )
    return pos->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das UOM-Objekt für die Maßeinheit "Radian"                       //
///////////////////////////////////////////////////////////////////////////////
UOM * UOMList::getUomAngle()
{
  return getUOMFromName ( "RADIAN" );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das UOM-Objekt für die Maßeinheit "Meter"                       //
///////////////////////////////////////////////////////////////////////////////
UOM * UOMList::getUomLength()
{
  return getUOMFromName ( "METRE" );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das UOM-Objekt für die Maßeinheit "Quadratmeter"                 //
///////////////////////////////////////////////////////////////////////////////
UOM * UOMList::getUomArea()
{
  return getUOMFromName ( "SQUARE_METRE" );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das UOM-Objekt für die Maßeinheit "Kubikmeter"                   //
///////////////////////////////////////////////////////////////////////////////
UOM * UOMList::getUomVolume()
{
  return getUOMFromName ( "CUBIC_METRE" );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein weiteres UOM-Objekt hinzu                                       //
///////////////////////////////////////////////////////////////////////////////
bool UOMList::addUOM ( UOM * pUOM )
{
  if ( mUOMFromIdentifier.find( pUOM->getIdentifier() ) != mUOMFromIdentifier.end() )
    return false;

  if ( mUOMFromName.find( pUOM->getName() ) != mUOMFromName.end() )
    return false;

  for ( unsigned int i = 0; i < pUOM->getAlternativeIdentifierAnz(); i++  )
    if ( mUOMFromIdentifier.find ( pUOM->getAlternativeIdentifier( i )) !=  mUOMFromIdentifier.end() )
      return false;

  vUOM.push_back ( pUOM );
  mUOMFromIdentifier[pUOM->getIdentifier()] = pUOM;
  for ( unsigned int i = 0; i < pUOM->getAlternativeIdentifierAnz(); i++  )
    mUOMFromIdentifier[pUOM->getAlternativeIdentifier(i)] = pUOM;

  mUOMFromName[pUOM->getName()] = pUOM;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  class UOM                                                                //
//  UOM-Beschreibung                                                         //
///////////////////////////////////////////////////////////////////////////////
UOM::UOM()
{
  name        = "";
  description = "";
  identifier  = "";
  factor      = 1.0;
  offset      = 0.0;
}

UOM::~UOM()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Rechnet den Wert valueOld in die durch pUomNew definierte Maßeinheit um  //
//  Voraussetzung ist, dass beide Maßeinheiten zur gleichen quantity gehören //
///////////////////////////////////////////////////////////////////////////////
bool UOM::changeUOM ( double valueOld, double &valueNew, UOM * pUomNew )
{
  valueNew = valueOld;
  if ( pUomNew == this )
    return true;

  if ( pUomNew == NULL )
    return false;

  valueNew = (((valueOld - offset)/factor) + pUomNew->getOffset() ) * pUomNew->getFactor();
  return true;
}

