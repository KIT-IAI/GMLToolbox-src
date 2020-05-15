#include "stdafx.h"

#include <vector>
#include <map>
#include <string>
#include <set>

#include "Convert.h"
#include "constant.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "KmlVisualisierung.h"

#include "Visualisierung_3D.h"

///////////////////////////////////////////////////////////////////////////////
//  class Visualisierung_3D                                                  //
//  Visualisierung von 3D Geodaten                                           //
///////////////////////////////////////////////////////////////////////////////
Visualisierung_3D::Visualisierung_3D ( Features * pFeatures, double zOffset )
{
  m_pFeatures  = pFeatures;
  m_pKmlParams = NULL;
  m_zOffset    = zOffset;
}

Visualisierung_3D::~Visualisierung_3D()
{
  if ( m_pKmlParams == NULL )
    delete m_pKmlParams;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben CityGML als KML                                               //
///////////////////////////////////////////////////////////////////////////////
bool Visualisierung_3D::writeCityGMLToKML( std::string &fileName, CITYGML_LOD lodStufe, bool showFeatureData )
{
  KmlWriter ^ pKmlWriter;
  bool        success;

  m_pKmlParams = getKmlParams();
  if ( m_pKmlParams == NULL ) return false;

  pKmlWriter = gcnew KmlWriter ( m_pKmlParams, m_pFeatures );

  pKmlWriter->setZOffset ( m_zOffset );
  pKmlWriter->setShowFeatureData ( showFeatureData );
  pKmlWriter->setLODStufe( lodStufe );

  success =  pKmlWriter->write ( fileName, "", 0 );

  delete pKmlWriter;
  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die KML Darstellungsparameter                                    //
///////////////////////////////////////////////////////////////////////////////
KmlVisualParams * Visualisierung_3D::getKmlParams()
{
  string                             stylesheetPath;
  string                             stylesheetDatei;
  String		                       ^ GML_VERSION;
  KmlVisualParamReader             ^ pReader;
  String                           ^ STYLESHEET_PATH;
  cli::array<String^>              ^ FILES;
  System::Collections::IEnumerator ^ FILES_ENUM;


  if ( m_pKmlParams != NULL )
  {
    if ( m_pKmlParams->getGmlTyp() == m_pFeatures->getGmlTyp() )
      return m_pKmlParams;
    else 
      delete m_pKmlParams;
  }

  m_pKmlParams = new KmlVisualParams ();
  GML_VERSION = QuConvert::ToString ( m_pFeatures->getGmlTypAsString() );

  pReader = gcnew KmlVisualParamReader ( GML_VERSION );

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
      m_pKmlParams = pReader->read ( stylesheetDatei );
      if ( m_pKmlParams != NULL )
        break;
    }
  }

  return m_pKmlParams;
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht die aktuellen Kml-Visualisierungsparameter                        //
///////////////////////////////////////////////////////////////////////////////
void Visualisierung_3D::resetKmlParams()
{
  if ( m_pKmlParams != NULL ) delete m_pKmlParams;
  m_pKmlParams = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben BoreholeML als KML                                             //
///////////////////////////////////////////////////////////////////////////////
bool Visualisierung_3D::writeBoreholeMLToKML( std::string &fileName, std::string geologThema, bool showFeatureData )
{
  KmlWriter ^ pKmlWriter;
  bool        success;
  string      selectAttributName;
  int         index = 0;

  m_pKmlParams = getKmlParams();
  if ( m_pKmlParams == NULL ) return false;

  pKmlWriter = gcnew KmlWriter ( m_pKmlParams, m_pFeatures );

  pKmlWriter->setZOffset ( m_zOffset );
  pKmlWriter->setShowFeatureData ( showFeatureData );

  if ( geologThema == "Stratigraphie")
    selectAttributName = "bml:stratigraphy::bml:chronoStratigraphy";
  else
  {
    selectAttributName = "bml:lithology::bml:rockName";

    if ( geologThema == "Lithologie, 1. Komponente")
      index = 0;
    else
    if ( geologThema == "Lithologie, 2. Komponente")
      index = 1;
    else
    if ( geologThema == "Lithologie, 3. Komponente")
      index = 2;
    else
    if ( geologThema == "Lithologie, 4. Komponente")
      index = 3; 
  }

  success =  pKmlWriter->write ( fileName, selectAttributName, index );

  delete pKmlWriter;

  return success;
}




