#include "StdAfx.h"

#include <vector>
#include <map>
#include <string>
#include <set>


#include "includes.h"
#include "Geometrie.h"
#include "comdef.h"
#include "Convert.h"
#include "SrsManagerRef.h"
#include "SrsManager.h"
#include <gps/gps.hpp>

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Text;
using namespace System::Collections;


#using <mscorlib.dll>


///////////////////////////////////////////////////////////////////////////////
//  SrsManager                                                               //
///////////////////////////////////////////////////////////////////////////////
SrsManager::SrsManager()
{

}

SrsManager::~SrsManager()
{
  map<string,CoordSystem*>::iterator iter;

  for ( iter = m_CoordSystems.begin(); iter != m_CoordSystems.end(); iter++ )
    delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Liste von Koordinatensystemen                               //
///////////////////////////////////////////////////////////////////////////////
void SrsManager::readCoordSystemList ( std::string fileName )
{
  String        ^ pActElement;
  String        ^ STR;
  int             i;
  string          stdString;
  CoordSystem   * pCoordSystem = nullptr;

  String        ^ fName  = gcnew String( fileName.c_str() );
  XmlTextReader ^ reader = gcnew XmlTextReader( fName );

  while( reader->Read() )
  {
    if ( reader->NodeType == XmlNodeType::Element )
    {
      pActElement = reader->Name;
      
      if ( pActElement->CompareTo( "Referenzsystem" ) == 0 )
        pCoordSystem = new CoordSystem();
      else
      if ( pActElement->CompareTo( "srsNameShort" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pCoordSystem->setSrsNameShort( stdString );
        m_CoordSystems[stdString] = pCoordSystem;
      }
      else
      if ( pActElement->CompareTo( "internalCode" ) == 0 )
      {
        STR = reader->ReadString();
        i = XmlConvert::ToInt32( STR );
        pCoordSystem->setInternalCode( i );
      }
      else
      if ( pActElement->CompareTo( "srsNameFull" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pCoordSystem->setSrsNameFull( stdString );
      }
      else
      if ( pActElement->CompareTo( "srsDimension" ) == 0 )
      {
        STR = reader->ReadString();
        i = XmlConvert::ToInt32( STR );
        pCoordSystem->setSrsDimension( i );
      }
      else
      if ( pActElement->CompareTo( "description" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pCoordSystem->setDescription( stdString );
      }
      else
      if ( pActElement->CompareTo( "art" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pCoordSystem->setCoordSystemType( stdString );
      }
      else
      if ( pActElement->CompareTo( "country" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr ( stdString, STR );
        pCoordSystem->setCountry( stdString );
      }
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Koordinatensystems                                      //
///////////////////////////////////////////////////////////////////////////////
bool SrsManager::addCoordSystem ( CoordSystem * coordSystem )
{
  string srsNameShort;

  srsNameShort =  coordSystem->getSrsNameShort();
  if ( m_CoordSystems.find( srsNameShort ) == m_CoordSystems.end() )
  {
    m_CoordSystems[srsNameShort] = coordSystem;
    return true;
  }
  else return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Koordinatensystem                                            //
///////////////////////////////////////////////////////////////////////////////
CoordSystem * SrsManager::getCoordSystem ( string srsName )
{
  string srsNameShort;

  srsName = makeUpperString( srsName );
  map<string,CoordSystem*>::iterator iter;

  iter = m_CoordSystemsUsed.find( srsName );
  if ( iter != m_CoordSystemsUsed.end() )
    return iter->second;

  for ( iter = m_CoordSystems.begin(); iter != m_CoordSystems.end(); iter++ )
  {
    srsNameShort = iter->first;
    if ( srsName.find( srsNameShort ) != string::npos )
    {
      m_CoordSystemsUsed[srsName] = iter->second;
      return iter->second;
    }
  }

  return NULL;
}

bool SrsManager::existCoordSystem ( std::string srsName )
{
  CoordSystem * pCoordSystem = getCoordSystem( srsName );
  if ( pCoordSystem != NULL )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Koordinatensysteme                                          //
///////////////////////////////////////////////////////////////////////////////
size_t SrsManager::getCoordSystems ( std::vector<CoordSystem*> &vCoordSystems )
{
  map<string,CoordSystem*>::iterator iter;

  for ( iter = m_CoordSystems.begin(); iter != m_CoordSystems.end(); iter++ )
  {
    CoordSystem * pSystem = iter->second;
    vCoordSystems.push_back( pSystem );
  }
  return vCoordSystems.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Code eines Koordinatensystems                                //
///////////////////////////////////////////////////////////////////////////////
int SrsManager::getCoordSystemInternalCode ( std::string srsName )
{
  CoordSystem * pCoordSystem = getCoordSystem( srsName );
  
  if ( pCoordSystem != NULL )
    return pCoordSystem->getInternalCode();
  else return -1;
}

///////////////////////////////////////////////////////////////////////////////
//  Hilfsroutine: Umwandlung in Kleinbuchstaben                              //
///////////////////////////////////////////////////////////////////////////////
string SrsManager::makeLowerString( string const & baseString )
{
  string                 newString;
  string::const_iterator it;

  for ( it=baseString.begin(); it != baseString.end(); ++it )
  {
    int iChar = *it;

    if ( iChar >= 65 && iChar <= 90 ) // Characters A - Z
      newString += iChar + 32;
    else
      newString += iChar;
  }

  return newString;
}

///////////////////////////////////////////////////////////////////////////////
//  Hilfsroutine: Umwandlung in Großbuchstaben                               //
///////////////////////////////////////////////////////////////////////////////
string SrsManager::makeUpperString( string const & baseString )
{
  string                 newString;
  string::const_iterator it;

  for ( it=baseString.begin(); it != baseString.end(); ++it )
  {
    int iChar = *it;

    if ( iChar >= 97 && iChar <= 122 ) // Characters a - z
      newString += iChar - 32;
    else
      newString += iChar;
  }

  return newString;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die srsDimension                                                 //
///////////////////////////////////////////////////////////////////////////////
int SrsManager::getSrsDimension( std::string srsName )
{
  String              ^ SRS_NAME;
  String              ^ STR;
  int                   crsAnz = 0;
  int                   i;
  int                   srsDimension;
  cli::array<String^> ^ split;
  cli::array<wchar_t> ^ delimiter = { ',' };

  srsName = makeUpperString( srsName );
  CoordSystem * pCoordSystem = getCoordSystem( srsName );
  if ( pCoordSystem != NULL )
  {
    srsDimension = pCoordSystem->getSrsDimension();
    if ( srsDimension == 3 ) return srsDimension;
  }

  SRS_NAME = QuConvert::ToString( srsName );
  SRS_NAME = SRS_NAME->Trim();

  if ( SRS_NAME->Contains( "URN:ADV:CRS") )
  {
    if ( SRS_NAME->Contains( "*" ) )
      return 3;
    else
      return 2;
  }

  split = SRS_NAME->Split( delimiter, StringSplitOptions::RemoveEmptyEntries );

  for ( i = 0; i < split->Length; i++ )
  {
    STR = split[i];
    if ( STR->Contains( "CRS:" ) || STR->Contains( "EPSG" ) )
      crsAnz++;
  }

  if ( crsAnz == 2 )
    return 3;
  else
    return 2;
}

///////////////////////////////////////////////////////////////////////////////
//  SrsProjection                                                            //
///////////////////////////////////////////////////////////////////////////////
SrsProjection::SrsProjection( SrsManager * pSrsManagerP )
{
  try
  {
	  gps::Factory factory;
  
	  pProjection = factory.createProjection(); 

	  pSrcGrid = factory.createGridParameters();
	  pDestGrid = factory.createGridParameters();
      pSrsManager = pSrsManagerP;
      initialized = true;
  }

  catch ( System::Exception ^ )
  {
    Windows::Forms::MessageBox::Show( "Initialisierung Koordinatentransformations-Modul fehlgeschlagen.\nActiveX Control nicht registriert?",
      "", Windows::Forms::MessageBoxButtons::OK, Windows::Forms::MessageBoxIcon::Error );
    initialized = false;
  }

}

SrsProjection::~SrsProjection() 
{
}

/*
void SrsProjection::CheckCoordinateTransformModule()
{
  System::Windows::Forms::DialogResult weiter;

  try
  {
    GpsToolkit::GpsProjection ^ pProjectionLoc = gcnew GpsToolkit::GpsProjection;
    pProjectionLoc->RegistrationCode = "DTBU7UZST81I71XN";
  }

  catch(  System::Exception ^ )
  {
    weiter = System::Windows::Forms::MessageBox::Show ( "Soll die DLL zur Koordinatentransformation registriert werden", "", 
                                    System::Windows::Forms::MessageBoxButtons::YesNo,
                                    System::Windows::Forms::MessageBoxIcon::Question );
    if ( weiter == System::Windows::Forms::DialogResult::Yes )
    {
      HMODULE hMod = LoadLibrary(_T("GpsCtl32.dll"));
      if (hMod != NULL)
      {
        FARPROC  proc = (FARPROC )GetProcAddress( hMod, "DllRegisterServer");
        if (proc != NULL)
        {
          HRESULT hr = (*proc)();
          if (hr != S_OK)
          {
            System::Windows::Forms::MessageBox::Show ( "Registrierung der DLL ist fehlgeschlagen\nWurde das Programm als Administrator gestartet? " );          }
        }
        FreeLibrary(hMod);
      }
      else
      {
        DWORD err = GetLastError();

//        LPTSTR pMsg;
//        FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//          NULL, err, LANG_USER_DEFAULT, (LPTSTR)&pMsg, 0, NULL);

          System::Windows::Forms::MessageBox::Show( "Registrierung des Controls fehlgeschlagen" );
//        msg.Format( _T("Registrierung des Controls fehlgeschlagen. Grund: %s\r\nGetLastError = 0x%x"), pMsg, err);
//        LocalFree(pMsg);

//        AfxMessageBox(msg);
      }
    }
  }
}

*/
bool SrsProjection::convertSrs2LatLon ( string srsNameShort, GeoPoint * pnt, GeoPoint * pntLatLon )
{
  int srsType = pSrsManager->getCoordSystemInternalCode( srsNameShort );
  if ( srsType == -1 ) return false;

  if ( srsNameShort == "325833" )
  {
    double x = pnt->getX();
    pnt->set(  x - 3000000.0, pnt->getY(), pnt->getZ() );
  }

  try
  {
    pSrcGrid->LoadFromId ( srsType );
    pDestGrid->LoadFromId ( 4326 );

    pProjection->putEasting ( pnt->getX() );
    pProjection->putNorthing ( pnt->getY() );

    pProjection->transformGrid( *pSrcGrid, *pDestGrid );

	pntLatLon->set(pProjection->getLongitude(), pProjection->getLatitude(), pnt->getZ());
	if (srsNameShort == "325833")
	{
		double x = pnt->getX();
		pnt->set(x + 3000000.0, pnt->getY(), pnt->getZ());
	}
	return true;

  }
  catch (const std::exception&)
  {
	  return false;
  }
}

bool SrsProjection::convertLatLon2Srs ( string srsNameShort, GeoPoint * pntLatLon, GeoPoint * pnt )
{
  int srsType = pSrsManager->getCoordSystemInternalCode( srsNameShort );
  if ( srsType == -1 ) return false;

  try
  {
    pSrcGrid->LoadFromId ( 4326 );
    pDestGrid->LoadFromId ( srsType );

    pProjection->putLongitude ( pntLatLon->getX() );
    pProjection->putLatitude  ( pntLatLon->getY() );

    pProjection->transformGrid( *pSrcGrid, *pDestGrid );

    if ( srsNameShort == "325833" )
      pnt->set ( pProjection->getEasting() + 3000000.0, pProjection->getNorthing() );
    else
      pnt->set ( pProjection->getEasting(), pProjection->getNorthing() );
    return true;
  }

  catch (const std::exception&)
  {
	  return false;
  }
}

bool SrsProjection::convertLatLon2Srs ( int srsType, GeoPoint * pntLatLon, GeoPoint * pnt )
{

	try
	{
	  pSrcGrid->LoadFromId ( 4326 );
    pDestGrid->LoadFromId ( srsType );

    pProjection->putLongitude ( pntLatLon->getX() );
    pProjection->putLatitude ( pntLatLon->getY() );

    pProjection->transformGrid( *pSrcGrid, *pDestGrid );

    if ( srsType == 325833 )
      pnt->set ( pProjection->getEasting() + 3000000.0, pProjection->getNorthing() );
    else
      pnt->set ( pProjection->getEasting(), pProjection->getNorthing() );
    return true;
	}
	catch (const std::exception&)
	{
    return false;
	}
}


bool SrsProjection::convertSrs2Srs ( std::string srsNameShortInput, std::string srsNameShortOutput, GeoPoint * p )
{
  int srsTypeInput  = pSrsManager->getCoordSystemInternalCode( srsNameShortInput );
  int srsTypeOutput = pSrsManager->getCoordSystemInternalCode( srsNameShortOutput );

  if ( srsTypeInput == -1 || srsTypeOutput == -1 ) 
    return false;

  if ( srsNameShortInput == "EPSG:325833" )
  {
    double x = p->getX();
    p->set(  x - 3000000.0, p->getY(), p->getZ() );
  }

  if (  srsTypeInput == srsTypeOutput )
  {
    if ( srsNameShortOutput == "325833" )
    {
      double x = p->getX();
      p->set(  x + 3000000.0, p->getY(), p->getZ() );
    }
     return true;
  }

  try
  {
    pSrcGrid->LoadFromId ( srsTypeInput );
   pDestGrid->LoadFromId ( srsTypeOutput );

    pProjection->putEasting  ( p->getX() );
    pProjection->putNorthing ( p->getY() );

    pProjection->transformGrid( *pSrcGrid, *pDestGrid );

    if ( srsNameShortOutput == "325833" )
      p->set ( pProjection->getEasting() + 3000000.0, pProjection->getNorthing() );
    else
      p->set ( pProjection->getEasting(), pProjection->getNorthing() );
    return true;
  }
  catch (const std::exception&)
  {
    return false;
  } 
}


///////////////////////////////////////////////////////////////////////////////
//  KoordSystem                                                              //
//  Beschreibung eines Koordinaten-Referenzsystems                           //
///////////////////////////////////////////////////////////////////////////////
CoordSystem::CoordSystem()
{
  srsNameShort    = "";
  internalCode    = -1;
  srsNameFull     = "";
  srsDimension    = 2;
  description     = "";
  coordSystemType = "";
  country         = "";
}

CoordSystem::~CoordSystem()
{

}

