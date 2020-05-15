#include "stdafx.h"

#include <vector>
#include <map>
#include <string>
#include <set>

#include "Convert.h"
#include "constant.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "GmlSchema.h"
#include "Geometrie.h"
#include "SvgVisualisierung.h"
#include "ObjectDataWriter.h"

#include "Visualisierung_2D.h"

///////////////////////////////////////////////////////////////////////////////
//  class Visualisierung_2D                                                  //
//  Visualisierung von 2D Geodaten                                           //
///////////////////////////////////////////////////////////////////////////////
Visualisierung_2D::Visualisierung_2D ( Features * pFeatures, Checking * pChecking, 
          float planausgabeBreite, float planausgabeHoehe, double globalScale  )
{
  m_pFeatures = pFeatures;
  m_pChecking = pChecking;

  m_pSvgParams			    = NULL; 
  m_pPlanAktuell        = NULL;
  m_pPlanBereichAktuell = NULL;

  m_planausgabeBreite = planausgabeBreite;
  m_planausgabeHoehe  = planausgabeHoehe;
  m_globalScale       = globalScale;

  m_svgUeberschriftDatei	    = "Ueberschrift.svg";
  m_svgUebersichtDatei		    = "Uebersicht.svg";
  m_svgKartenDatei			      = "Karte.svg";

  m_objectDataGenerated = false;
  m_delta = 0.3;
}

Visualisierung_2D::~Visualisierung_2D()
{
  if ( m_pSvgParams != NULL ) delete m_pSvgParams; 
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Skalierung eines SVG-Plans                                     //
///////////////////////////////////////////////////////////////////////////////
void Visualisierung_2D::setGlobalScale ( double scaleP )
{  
  m_globalScale = scaleP;
  resetSvgParams();
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen bzw. Liefern der SVG-Visualisierungsparameter				        	 //
///////////////////////////////////////////////////////////////////////////////
SvgVisualisierung * Visualisierung_2D::getSvgParams()
{
  string                             stylesheetPath;
  string                             stylesheetDatei;
  String		                       ^ GML_VERSION;
  SvgParamReader                   ^ pReader;
  String                           ^ STYLESHEET_PATH;
  cli::array<String^>              ^ FILES;
  System::Collections::IEnumerator ^ FILES_ENUM;

  if ( m_pSvgParams != NULL )
  {
    if ( m_pSvgParams->getGmlTyp() == m_pFeatures->getGmlTyp() )
      return m_pSvgParams;
    else delete  m_pSvgParams;
  }

  m_pSvgParams = new SvgVisualisierung ();
  m_pSvgParams->setBreiteDisplay( m_planausgabeBreite );
  m_pSvgParams->setHoeheDisplay( m_planausgabeHoehe );
  GML_VERSION = QuConvert::ToString (  m_pFeatures->getGmlSchema()->getGmlTypAsString() );

  pReader = gcnew SvgParamReader ( m_globalScale, GML_VERSION );

  stylesheetPath = Constant::getStylesheetPath( m_pFeatures->getGmlTyp() );

  STYLESHEET_PATH = QuConvert::ToString ( stylesheetPath );
  bool exist = Directory::Exists ( STYLESHEET_PATH );
  if ( exist )
  {
    FILES = Directory::GetFiles ( STYLESHEET_PATH );

    FILES_ENUM = FILES->GetEnumerator();
    while ( FILES_ENUM->MoveNext() )
    {
      String^ FILE_NAME = safe_cast<String^>(FILES_ENUM->Current);
      QuConvert::systemStr2stdStr ( stylesheetDatei, FILE_NAME );
      pReader->read ( stylesheetDatei , m_pSvgParams );
    }
  }

  return m_pSvgParams;
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht die aktuellen Svg-Visualisierungsparameter                        //
///////////////////////////////////////////////////////////////////////////////
void Visualisierung_2D::resetSvgParams()
{
  if ( m_pSvgParams != NULL ) delete m_pSvgParams;
  m_pSvgParams = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben der GML-Objekte als statische svg-Datei                        //
///////////////////////////////////////////////////////////////////////////////
void Visualisierung_2D::writeStaticSVG ( std::string fileName, bool allePlaeneZeigen )
{  
  SvgWriter ^ pSvgWriter;

  m_pSvgParams = getSvgParams();
  if ( m_pSvgParams == NULL ) return;

  pSvgWriter = gcnew SvgWriter ( this, allePlaeneZeigen, 9999999, m_delta );
  pSvgWriter->write ( fileName, 1, m_globalScale );

  delete pSvgWriter;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben der GML-Objekte als interaktive svg-Datei                      //
///////////////////////////////////////////////////////////////////////////////
bool Visualisierung_2D::writeInteractiveSVG ( int darstellungsTyp, bool allePlaeneZeigen )
{
  string           fehlerText;
  string           objectDataFolderFull;
  bool             success;
  string           ueberschriftPfad;
  string           uebersichtPfad;
  string           kartePfad;
  string           svgDatei;	
  SvgWriter      ^ pSvgWriter;
  _Geometrie     * pGeltungsbereich;
  vector<Feature*> vFeatures;
	size_t           anz;

  m_pSvgParams = getSvgParams();
  if ( m_pSvgParams == NULL ) return false;

  pSvgWriter = gcnew SvgWriter ( this, allePlaeneZeigen, 9999999, m_delta );

  if ( m_pFeatures->isXPlanGML() )
  {
    if ( m_pPlanAktuell == NULL && m_pPlanBereichAktuell == NULL )
    {
      fehlerText = "Es wurde kein Plan spezifiziert";
      return false;
    }

    if ( allePlaeneZeigen == false &&
      ( darstellungsTyp == 2 ||  darstellungsTyp == 3 )  )
    {
      pGeltungsbereich = m_pPlanAktuell->getGeometry( "xplan:raeumlicherGeltungsbereich", 0 );

      if ( pGeltungsbereich == NULL )
      {
        pGeltungsbereich = m_pPlanBereichAktuell->getGeometry( "xplan:geltungsbereich", 0 );
        if ( pGeltungsbereich == NULL  )
        {
          fehlerText = "Kein Geltungsbereich festgelegt, interaktive Darstellung nicht möglich";
        }
      }
    }
  }
  else
    if ( m_pFeatures->isINSPIREPLU() )
    {
      anz = m_pFeatures->getFeatures( vFeatures, Constant::getKlassenNameINSPIRE( SPATIAL_PLAN ) );
      if ( anz == 0 )
      {
        fehlerText = "Es wurde kein Plan spezifiziert";
        return false;
      }

      if ( darstellungsTyp == 2 ||  darstellungsTyp == 3 )
      {
        pGeltungsbereich = vFeatures[0]->getGeometry( "plu:extent", 0 );
        if ( pGeltungsbereich == NULL )
        {
          fehlerText = "Kein Geltungsbereich festgelegt, interaktive Darstellung nicht möglich";
          return false;
        }
      }
    }

    if ( darstellungsTyp == 2 || darstellungsTyp == 3 )
    {
      if ( !m_objectDataGenerated )
      {
        success = generateObjectData (  allePlaeneZeigen );
        if ( success ) m_objectDataGenerated = true;
      }

      ueberschriftPfad = Constant::getSystemFolder() + m_svgUeberschriftDatei;
      if ( m_pFeatures->isXPlanGML() || m_pFeatures->isINSPIREPLU() )
        pSvgWriter->writeUeberschrift ( ueberschriftPfad );
      else
        pSvgWriter->writeUeberschriftStandard ( ueberschriftPfad );

      uebersichtPfad =  Constant::getSystemFolder() + m_svgUebersichtDatei;
      pSvgWriter->writeUebersicht( uebersichtPfad, m_globalScale );

      kartePfad = Constant::getSystemFolder() + m_svgKartenDatei;
      pSvgWriter->write ( kartePfad, darstellungsTyp, m_globalScale );

    }
    delete pSvgWriter;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Attribute der einzelnen Objekte als html-Tabellen			     //
///////////////////////////////////////////////////////////////////////////////
bool Visualisierung_2D::generateObjectData ( bool allePlaeneZeigen )
{
  vector<Feature*>    vObjekte;
  Feature           * pPlanObjekt;
	size_t  		        i, anz;
  String            ^ STR;
  ObjectDataWriter  ^ pObjDataWriter;
  string              objectDataFolderGes;

  objectDataFolderGes = Constant::getObjectDataPath();

  STR = QuConvert::ToString ( objectDataFolderGes );
  if ( !Directory::Exists ( STR ) )
    Directory::CreateDirectory ( STR );

  pObjDataWriter = gcnew ObjectDataWriter ( STR, this );

  if ( allePlaeneZeigen )
    anz = m_pFeatures->getAllFeatures ( vObjekte );
  else
    anz = m_pFeatures->getFeatures ( vObjekte, m_pPlanAktuell, m_pPlanBereichAktuell );

  for ( i = 0; i < anz; i++ )
  {
    pPlanObjekt = vObjekte[i];
    if ( pPlanObjekt->getGeometrieTyp() == OTHER || 
      pPlanObjekt->getGeometrieTyp() == NO_GEOMETRY )
      continue;
    pObjDataWriter->write ( pPlanObjekt );
  }
  delete pObjDataWriter;
  return true;
}	







