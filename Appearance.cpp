#include "StdAfx.h"

#include <vector>
#include <map>
#include <string>

#include "Geometrie.h"
#include "Appearance.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//   Appearance                                                              //
//   Entspricht der CityGML-Klasse Appearance                                //
///////////////////////////////////////////////////////////////////////////////
cAppearance::cAppearance()
{
  m_appearId  = ++AppearanceID;
}

cAppearance::~cAppearance()
{
  for ( unsigned int i = 0; i < m_AppearanceData.size(); i++ )
    delete m_AppearanceData[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen AppearanceDataRef                                             //
///////////////////////////////////////////////////////////////////////////////
void cAppearance::addAppearanceDataRef ( AppearanceDataRef * pRef )
{
  m_AppearanceData.push_back ( pRef );
}

///////////////////////////////////////////////////////////////////////////////
//   AppearanceData                                                          //
//   Entspricht der CityGML-Klasse _SurfaceData                              //
///////////////////////////////////////////////////////////////////////////////
AppearanceData::AppearanceData()
{
  m_appearDataId = ++AppearanceID;
  m_isFront      = true;
}

AppearanceData::~AppearanceData()
{
}


///////////////////////////////////////////////////////////////////////////////
//  class AppearanceDataRef                                                  //
//  Referenz auf Appearance Data                                             //
///////////////////////////////////////////////////////////////////////////////
AppearanceDataRef::AppearanceDataRef ( std::string gmlId )
{
  m_gmlId         = gmlId;
  m_appearDataId  = 0;
}

AppearanceDataRef::AppearanceDataRef ( unsigned int id )
{
  m_appearDataId  = id;
}


///////////////////////////////////////////////////////////////////////////////
//  AppearanceTexture                                                        //
//   Entspricht der CityGML-Klasse ParametrizedTexture                       //
///////////////////////////////////////////////////////////////////////////////
AppearanceTexture::AppearanceTexture()
:AppearanceData()
{
  m_filename    = _T("");
  m_mimeType    = _T("");;
  m_textureType = UNKNOWN;
  m_wrapMode    = WRAP;

  m_borderColorSet = false;
  for ( int i = 0; i < 4; i++ )
    m_borderColor[i] = 1.0;
}

AppearanceTexture::~AppearanceTexture()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt Attribut m_borderColor                                             //
///////////////////////////////////////////////////////////////////////////////
void AppearanceTexture::setBorderColor ( double borderColor[4] )
{
  int i;

  for ( i = 0; i < 4; i++ )
    m_borderColor[i] = borderColor[i];
  m_borderColorSet = true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Attribut m_borderColor                                           //
///////////////////////////////////////////////////////////////////////////////
bool AppearanceTexture::getBorderColor ( double borderColor[4] ) 
{
  int i;

  for ( i = 0; i < 4; i++ )
    borderColor[i] = m_borderColor[i];
  return m_borderColorSet;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt Attribut m_textureType                                             //
///////////////////////////////////////////////////////////////////////////////
void AppearanceTexture::setTextureType ( std::string textureType )
{
  if ( textureType == _T("typical") ) m_textureType = TYPICAL;
  else
  if ( textureType == _T("specific") ) m_textureType = SPECIFIC;
  else
  if ( textureType == _T("unknown") ) m_textureType = UNKNOWN;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Attribut m_textureType als string                                //
///////////////////////////////////////////////////////////////////////////////
std::string AppearanceTexture::getTextureTypeString()
{
  if ( m_textureType == TYPICAL )  return  _T("typical");
  if ( m_textureType == SPECIFIC ) return  _T("specific");
  if ( m_textureType == UNKNOWN )  return  _T("unknown");
  
  return _T("unknown");;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt Attribut m_wrapMode                                                //
///////////////////////////////////////////////////////////////////////////////
void AppearanceTexture::setWrapMode ( std::string wrapMode )
{
  if ( wrapMode == _T("none") ) m_wrapMode = NONE;
  else
  if ( wrapMode == _T("wrap") ) m_wrapMode = WRAP;
  else
  if ( wrapMode == _T("mirror") ) m_wrapMode = MIRROR;
  else
  if ( wrapMode == _T("clamp") ) m_wrapMode = CLAMP;
  else
  if ( wrapMode == _T("border") ) m_wrapMode = BORDER;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Attribut m_wrapMode als string                                   //
///////////////////////////////////////////////////////////////////////////////
std::string AppearanceTexture::getWrapModeString()
{
  if ( m_wrapMode == NONE  )   return _T("none");
  if ( m_wrapMode == WRAP )    return _T("wrap");
  if ( m_wrapMode == MIRROR  ) return _T("mirror");
  if ( m_wrapMode == CLAMP  )  return _T("clamp");
  if ( m_wrapMode == BORDER  ) return _T("border");

  return _T("wrap");
}


///////////////////////////////////////////////////////////////////////////////
//   class AppearanceParametrizedTexture                                     //
//   Entspricht der CityGML-Klasse ParametrizedTexture                       //
///////////////////////////////////////////////////////////////////////////////
AppearanceParametrizedTexture::AppearanceParametrizedTexture ()
: AppearanceTexture()
{
}

AppearanceParametrizedTexture::~AppearanceParametrizedTexture()
{
  unsigned int i;

  for ( i = 0; i < m_texCoordVector.size(); i++ )
    delete m_texCoordVector[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die TexCoordRef Objekte                                          //
///////////////////////////////////////////////////////////////////////////////
size_t AppearanceParametrizedTexture::getTexCoords ( std::vector<TexCoordRef*> &vTexCoords )
{
	size_t i;

  for ( i = 0; i < m_texCoordVector.size(); i++ )
    vTexCoords.push_back( m_texCoordVector[i] );

  return m_texCoordVector.size();
}


///////////////////////////////////////////////////////////////////////////////
//   class AppearanceColor                                                   //
//   Entspricht der CityGML-Klasse X3DMaterial                               //
///////////////////////////////////////////////////////////////////////////////
AppearanceColor::AppearanceColor()
:AppearanceData()
{
    m_ambient_red     = -1.0;
    m_ambient_green   = -1.0;
    m_ambient_blue    = -1.0;
    m_ambient_alpha   =  1.0;

    m_diffuse_red     = -1.0;
    m_diffuse_green   = -1.0;
    m_diffuse_blue    = -1.0;
    m_diffuse_alpha   =  1.0;

    m_specular_red    = -1.0;
    m_specular_green  = -1.0;
    m_specular_blue   = -1.0;
    m_specular_alpha  =  1.0;

    m_emission_red    = -1.0;
    m_emission_green  = -1.0;
    m_emission_blue   = -1.0;
    m_emission_alpha  =  1.0;

    m_shininess       = -1.0;
    m_isSmooth        =  true;
    m_isSmoothSet     = false;
}


///////////////////////////////////////////////////////////////////////////////
//  Setzt bzw. liefert die AmbientColor                                      //
///////////////////////////////////////////////////////////////////////////////
void AppearanceColor::setAmbientColor(double color[3])
{
  m_ambient_red   = color[0];
  m_ambient_green = color[1];
  m_ambient_blue  = color[2];
}

void AppearanceColor::setAmbientColor(double red, double green, double blue, double alpha)
{
  m_ambient_red   = red;
  m_ambient_green = green;
  m_ambient_blue  = blue;
  m_ambient_alpha = alpha;
}

bool AppearanceColor::getAmbientColor(double color[3])
{
  if ( m_ambient_red < 0 ) return false;

  color[0] = m_ambient_red;
  color[1] = m_ambient_green;
  color[2] = m_ambient_blue;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Setzt bzw. liefert die DiffuseColor                                      //
///////////////////////////////////////////////////////////////////////////////
void AppearanceColor::setDiffuseColor(double color[3])
{
  m_diffuse_red   = color[0];
  m_diffuse_green = color[1];
  m_diffuse_blue  = color[2];
}

void AppearanceColor::setDiffuseColor(double red, double green, double blue, double alpha)
{
  m_diffuse_red   = red;
  m_diffuse_green = green;
  m_diffuse_blue  = blue;
  m_diffuse_alpha = alpha;
}

bool AppearanceColor::getDiffuseColor(double color[3])
{
  if ( m_diffuse_red < 0 ) return false;

  color[0] = m_diffuse_red;
  color[1] = m_diffuse_green;
  color[2] = m_diffuse_blue;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Setzt bzw. liefert die SpecularColor                                     //
///////////////////////////////////////////////////////////////////////////////
void AppearanceColor::setSpecularColor(double color[3])
{
  m_specular_red   = color[0];
  m_specular_green = color[1];
  m_specular_blue  = color[2];
}

void AppearanceColor::setSpecularColor(double red, double green, double blue, double alpha)
{
  m_specular_red   = red;
  m_specular_green = green;
  m_specular_blue  = blue;
  m_specular_alpha = alpha;
}

bool AppearanceColor::getSpecularColor(double color[3])
{
  if ( m_specular_red < 0 ) return false;

  color[0] = m_specular_red;
  color[1] = m_specular_green;
  color[2] = m_specular_blue;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Setzt bzw. liefert die EmissiveColor                                     //
///////////////////////////////////////////////////////////////////////////////
void AppearanceColor::setEmissiveColor(double color[3])
{
  m_emission_red   = color[0];
  m_emission_green = color[1];
  m_emission_blue  = color[2];
}

void AppearanceColor::setEmissiveColor(double red, double green, double blue, double alpha)
{
  m_emission_red   = red;
  m_emission_green = green;
  m_emission_blue  = blue;
  m_emission_alpha = alpha;
}

bool AppearanceColor::getEmissiveColor(double color[3])
{
  if ( m_emission_red < 0 ) return false;

  color[0] = m_emission_red;
  color[1] = m_emission_green;
  color[2] = m_emission_blue;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die Shininess                                                    //
///////////////////////////////////////////////////////////////////////////////
bool AppearanceColor::getShininess(double &shininess)
{
  if ( m_shininess < 0 ) return false;

  shininess = m_shininess;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Setzt / Liefert die Transparenz                                          //
///////////////////////////////////////////////////////////////////////////////
void AppearanceColor::setTransparency(double transparency)
{
  m_ambient_alpha  = transparency;
  m_diffuse_alpha  = transparency;
  m_specular_alpha = transparency;
  m_emission_alpha = transparency;
}

bool AppearanceColor::getTransparency(double &transparency)
{
  if ( m_diffuse_alpha < 0 ) return false;

  transparency = m_diffuse_alpha;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Setzt bzw. liefert die AmbientIntensity                                  //
///////////////////////////////////////////////////////////////////////////////
void AppearanceColor::setAmbientIntensity(double intensity)
{
  m_ambient_red   = intensity;
  m_ambient_green = intensity;
  m_ambient_blue  = intensity;
}

bool AppearanceColor::getAmbientIntensity(double &intensity)
{
  if ( m_ambient_red < 0 ) return false;

  intensity = m_ambient_red;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt bzw. liefert Attribut isSmooth                                     //
///////////////////////////////////////////////////////////////////////////////
void AppearanceColor::setIsSmooth(bool b)
{
  m_isSmooth    = b;
  m_isSmoothSet = true;
}

bool AppearanceColor::getIsSmooth(bool &b)
{
  if ( m_isSmoothSet == false ) return false;

  b =  m_isSmooth;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Addiert ein Geometrie-Target                                             //
///////////////////////////////////////////////////////////////////////////////
void AppearanceColor::addTarget ( string target  )
{
  m_targets.push_back ( target );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Geometrie-Target                                             //
///////////////////////////////////////////////////////////////////////////////
size_t AppearanceColor::getTargets ( vector<string> &vTargets )
{
  for ( size_t i = 0; i < m_targets.size(); i++ )
    vTargets.push_back ( m_targets[i] );
  return vTargets.size();
}


///////////////////////////////////////////////////////////////////////////////
//  class AppearanceGeoreferencedTexture                                     //
//   Entspricht der CityGML-Klasse GeoreferencedTexture                      //
///////////////////////////////////////////////////////////////////////////////
AppearanceGeoreferencedTexture::AppearanceGeoreferencedTexture()
: AppearanceTexture()
{
  m_preferWorldFile    = true;
  m_preferWorldFileSet = false;

  m_orientation[0] = 1.0;
  m_orientation[1] = 0.0;
  m_orientation[2] = 0.0;
  m_orientation[3] = 1.0;

  m_referencePoint  = new GeoPoint(2);
}

AppearanceGeoreferencedTexture::~AppearanceGeoreferencedTexture()
{
  if ( m_referencePoint != NULL ) delete m_referencePoint;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt bzw. liefert Attribut m_preferWorldFile                            //
///////////////////////////////////////////////////////////////////////////////
void AppearanceGeoreferencedTexture::setPreferWorldFile ( bool b )
{
  m_preferWorldFileSet = true;
  m_preferWorldFile    = b;
}

bool AppearanceGeoreferencedTexture::getPreferWorldFile (  bool & preferWorldFile )
{
  if ( m_preferWorldFileSet == false ) return false;

  preferWorldFile = m_preferWorldFile;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt die gml:id einer Target-Geometrie hinzu                             //
///////////////////////////////////////////////////////////////////////////////
void AppearanceGeoreferencedTexture::addTarget ( string target )
{
  m_targets.push_back ( target );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Targets                                                      //
///////////////////////////////////////////////////////////////////////////////
size_t AppearanceGeoreferencedTexture::getTargets ( vector<string> &vTargets )
{
  for ( size_t i = 0; i < m_targets.size(); i++ )
    vTargets.push_back ( m_targets[i] );
  return vTargets.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt bzw. liefert Attribut m_orientation                                //
///////////////////////////////////////////////////////////////////////////////
void AppearanceGeoreferencedTexture::setOrientation ( double orientation[4] )
{
  for ( int i = 0; i < 4; i++ )
    m_orientation[i] = orientation[i];
}

void AppearanceGeoreferencedTexture::getOrientation ( double orientation[4] )
{
  for ( int i = 0; i < 4; i++ )
    orientation[i] = m_orientation[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt Attribut m_referencePoint                                          //
///////////////////////////////////////////////////////////////////////////////
void AppearanceGeoreferencedTexture::setReferencePoint ( GeoPoint * referencePoint )
{  
  if ( m_referencePoint != NULL )
    delete m_referencePoint;
  m_referencePoint = referencePoint;  
}

///////////////////////////////////////////////////////////////////////////
//  class TexCoord                                                       //
//  Abstrakte Basisklasse der Texturkoordinaten Klassen                  //
///////////////////////////////////////////////////////////////////////////
TexCoord::TexCoord ()
{
  m_texCoordId = ++AppearanceID;
}


///////////////////////////////////////////////////////////////////////////
//  class TextCoordRef                                                   //
///////////////////////////////////////////////////////////////////////////
TexCoordRef::TexCoordRef()
{
  m_texCoordId = ++AppearanceID;
}


///////////////////////////////////////////////////////////////////////////////
//  class TexCoordGen                                                        //
//   Entspricht der CityGML-Klasse TexCoordGen                               //
///////////////////////////////////////////////////////////////////////////////
TexCoordGen::TexCoordGen()
: TexCoord()
{
  m_worldToTexture = NULL;
}

TexCoordGen::~TexCoordGen()
{
  if ( m_worldToTexture != NULL ) delete m_worldToTexture;
}

///////////////////////////////////////////////////////////////////////////////
//   class TexCoordList                                                      //
//   Entspricht der CityGML-Klasse TexCoordList                              //
///////////////////////////////////////////////////////////////////////////////
TexCoordList::TexCoordList()
: TexCoord()
{
}

TexCoordList::~TexCoordList()
{
  map<string,TexCoordPolyloop*>::iterator iter;

  for ( iter = m_texCoordinates.begin(); iter != m_texCoordinates.end(); iter++ )
    delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Texturkoordinaten für den Ring mit gml:id gmlIdRing hinzu           //
///////////////////////////////////////////////////////////////////////////////
void TexCoordList::addTexCoordPolyloop ( std::string gmlIdRing, TexCoordPolyloop * pTexCoordinates  )
{
  m_texCoordinates[gmlIdRing] = pTexCoordinates;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Texturkoordinaten und die zugehörigen Ring-Ids                   //
///////////////////////////////////////////////////////////////////////////////
size_t TexCoordList::getTexCoordPolyloops ( std::vector<std::string> & vGmlIds,
                                         std::vector<TexCoordPolyloop*> & vTexCoords )
{
  map<string,TexCoordPolyloop*>::iterator iter;

  for ( iter = m_texCoordinates.begin(); iter != m_texCoordinates.end(); iter++ )
  {
    vGmlIds.push_back ( iter->first );
    vTexCoords.push_back ( iter->second );
  }

  return vGmlIds.size();
}


///////////////////////////////////////////////////////////////////////////////
//   class TexCoordPolyloop                                                  //
//  Liste von Texturkoordinaten                                              //
//  Liste von Texturkoordinaten                                          //
TexCoordPolyloop::TexCoordPolyloop(std::vector<double> texCoordinates)
  : TexCoord()
{
  for ( unsigned int i = 0; i < texCoordinates.size(); i++ )
    m_textureCoordinates.push_back( texCoordinates[i] );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Texturkoordinaten                                            //
///////////////////////////////////////////////////////////////////////////////
size_t TexCoordPolyloop::getTextureCoords(std::vector<double>  &vCoords)
{
  for ( size_t i = 0; i < m_textureCoordinates.size(); i++ )
    vCoords.push_back ( m_textureCoordinates[i] );

  return vCoords.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Kehrt die Reihenfolge der Texturkoordinaten um                           //
///////////////////////////////////////////////////////////////////////////////
void TexCoordPolyloop::invertTextureCoords()
{
  std::vector<double> textureCoords;
  std::vector<double>::reverse_iterator it;

  textureCoords.swap(m_textureCoordinates);

  for(it = textureCoords.rbegin(); it != textureCoords.rend(); ++it)
  {
    double tValue = *it;
    double sValue = *(++it);
    m_textureCoordinates.push_back(sValue);
    m_textureCoordinates.push_back(tValue);
  }
}
