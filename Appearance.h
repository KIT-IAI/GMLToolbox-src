#pragma once

#include "includes.h"

class TexCoordListPolyloop;
class AppearanceDataRef;
class TexCoordRef;
class TexCoordList;
class TexCoordPolyloop;
class Matrix4;
class GeoPoint;

static unsigned int AppearanceID = 1;

struct GeoAppearanceRef
{
  std::string   m_thema;
  unsigned int m_appearData;
  unsigned int m_texCoordId;
};


////////////////////////////////////////////////////////////////////////////////
//  class cAppearance                                                         //
//   Modellierung der CityGML-Klasse Appearance                               //
////////////////////////////////////////////////////////////////////////////////
class cAppearance
{
  public:
    cAppearance();
    virtual ~cAppearance();

  protected:
    unsigned int m_appearId;   //  Interne Id der Appearance
    std::string  m_thema;      //  Thema (CityGML Attribut theme)
    std::string  m_gmlId;      //  gml:id

    std::vector<AppearanceDataRef*> m_AppearanceData;

  public:
    void setAppearId ( unsigned int idP )  { m_appearId = idP;  }
    unsigned int getAppearId ()  {  return m_appearId;  }

    void setThema ( std::string thema )  {  m_thema = thema;  }
    std::string getThema ()  {  return m_thema;  }

    void setGmlId ( std::string id )  {  m_gmlId = id;  }
    std::string getGmlId ()  {  return m_gmlId;  }

    void addAppearanceDataRef ( AppearanceDataRef * pRef );
};


////////////////////////////////////////////////////////////////////////////////
//  class AppearanceDataRef                                                   //
//  Referenz auf AppearanceData                                              //
////////////////////////////////////////////////////////////////////////////////
class AppearanceDataRef
{
  public:
    AppearanceDataRef ( std::string gmlId );
    AppearanceDataRef ( unsigned int id );
    virtual ~ AppearanceDataRef()  {  }

  public:
    std::string  m_gmlId;
    unsigned int m_appearDataId;
};


////////////////////////////////////////////////////////////////////////////////
//  Apperance_Set                                                             // 
//  Menge von cAppearance Objekten                                            //
////////////////////////////////////////////////////////////////////////////////
 class Apperance_Set : public cAppearance
{
  public:
    Apperance_Set() {}
    virtual ~Apperance_Set() {}

  protected:
    std::vector<cAppearance*> m_appearances;
};


////////////////////////////////////////////////////////////////////////////////
//  ArrearanceData - abstrakte Klasse                                         //
//   Entspricht der CityGML-Klasse _SurfaceData                               //
////////////////////////////////////////////////////////////////////////////////
class AppearanceData
{
  public:
    enum AppearanceDataType
      { PARAMETRIZED_TEXTURE,
        GEOREFERENCED_TEXTURE,
        MATERIAL
        };

  public:
    AppearanceData ();
    virtual ~ AppearanceData();

  protected:
    unsigned int m_appearDataId;   //  Interne Id des Objektes
    std::string  m_gmlId;          //  gml:id
    bool         m_isFront;        //  CityGML Attribut isFront

  public:
    virtual AppearanceDataType getAppearanceDataType() = NULL;

    void setAppearDataId  ( unsigned int id )  {  m_appearDataId = id; }
    unsigned int getAppearDataId ()  {  return m_appearDataId;  }

    void setGmlId ( std::string gmlId )  {  m_gmlId = gmlId;  }
    std::string getGmlId ()  {   return m_gmlId;  }

    void setIsFront ( bool b )  {  m_isFront = b;  }
    bool getIsFront ()  {  return m_isFront;  }
};


////////////////////////////////////////////////////////////////////////////////
//   class AppearanceTexture - abstrakte Klasse                               //
//   Entspricht der CityGML-Klasse _AbstractTexture                           //
////////////////////////////////////////////////////////////////////////////////
class AppearanceTexture : public AppearanceData
{
  public:
    enum TextureType { TYPICAL, SPECIFIC, UNKNOWN };
    enum WrapMode    { NONE, WRAP, MIRROR, CLAMP, BORDER };

  public:
    AppearanceTexture ();
    virtual ~AppearanceTexture();

  protected:
    std::string m_filename;       //  imageURI
    std::string m_mimeType;       // mimeType
    TextureType m_textureType;    //  textureType
    WrapMode    m_wrapMode;       //  wrapMode
    bool        m_borderColorSet; //  Existenz des Attribute borderColor 
    double      m_borderColor[4]; //  borderColor

  public:
    void setImageURI ( std::string filename )  {  m_filename = filename;  }
    std::string getImageURI ()  {  return m_filename;  }

    void setMimeType ( std::string mimeType )  {  m_mimeType = mimeType;  }
    std::string getMimeType ()  {  return m_mimeType;  }

    void setTextureType ( TextureType textureType )  {  m_textureType = textureType;  }
    void setTextureType ( std::string textureType );
    TextureType getTextureType ()  {  return m_textureType;  }
    std::string getTextureTypeString();

    void setWrapMode ( WrapMode wrapMode )  {  m_wrapMode = wrapMode;  }
    void setWrapMode ( std::string wrapMode );
    WrapMode getWrapMode ()  {  return m_wrapMode;  }
    std::string getWrapModeString();

    void setBorderColor ( double borderColor[4] );
    bool getBorderColor ( double borderColor[4] ); 

    const std::string& getFilename() const { return m_filename; }
    void setFilename(const std::string& filename) { m_filename = filename; }
};


////////////////////////////////////////////////////////////////////////////////
//   class AppearanceParametrizedTexture                                      //
//   Entspricht der CityGML-Klasse ParametrizedTexture                        //
////////////////////////////////////////////////////////////////////////////////
class AppearanceParametrizedTexture : public AppearanceTexture
{
  public:
    AppearanceParametrizedTexture ();
    virtual ~AppearanceParametrizedTexture();

  protected:
    std::vector<TexCoordRef*> m_texCoordVector;  //  Referierte Texturkoordinaten

  public:
    virtual AppearanceDataType getAppearanceDataType() {  return PARAMETRIZED_TEXTURE;  }

    void addTexCoord ( TexCoordRef * pTexCoordRef )  {  m_texCoordVector.push_back ( pTexCoordRef );  }
		size_t getTexCoords ( std::vector<TexCoordRef*> &vTexCoords );
};


////////////////////////////////////////////////////////////////////////////////
//   class AppearanceColor                                                    //
//  Entspricht der CityGML-Klasse X3DMaterial                                 //
////////////////////////////////////////////////////////////////////////////////
class AppearanceColor : public AppearanceData
{
  public:
    AppearanceColor();
    virtual ~AppearanceColor() {}

    virtual AppearanceColor* getImpl() { return this; }

  protected:
    double m_ambient_red;    //  ambientIntensity
    double m_ambient_green;  //  ambientIntensity
    double m_ambient_blue;   //  ambientIntensity
    double m_ambient_alpha;  //  transparency

    double m_diffuse_red;    //  diffuseColor[0]
    double m_diffuse_green;  //  diffuseColor[1]
    double m_diffuse_blue;   //  diffuseColor[2]
    double m_diffuse_alpha;  //  transparency

    double m_specular_red;    //  specularColor[0]
    double m_specular_green;  //  specularColor[1]
    double m_specular_blue;   //  specularColor[2]
    double m_specular_alpha;  //  transparency

    double m_emission_red;     //  emissiveColor[0]
    double m_emission_green;   //  emissiveColor[1]
    double m_emission_blue;    //  emissiveColor[2]
    double m_emission_alpha;   //  transparency

    double m_shininess;       // shineness

    bool   m_isSmooth;        //  isSmooth
    bool   m_isSmoothSet;     //  Variable m_isSmooth wurde gesetzt

    std::vector<std::string> m_targets;  //  gml:ids der zugeordneten Geometrieobjekte

  public:
    virtual AppearanceDataType getAppearanceDataType()  {  return MATERIAL;  }

    void setAmbientColor(double color[3]);
    void setAmbientColor(double red, double green, double blue, double alpha);
    bool getAmbientColor(double color[3]);
    double getAmbientAlpha() { return m_ambient_alpha; }

    void setDiffuseColor(double color[3]);
    void setDiffuseColor(double red, double green, double blue, double alpha);
    bool getDiffuseColor(double color[3]);
    double getDiffuseAlpha() { return m_diffuse_alpha; }

    void setSpecularColor(double color[3]);
    void setSpecularColor(double red, double green, double blue, double alpha);
    bool getSpecularColor(double color[3]);
    double getSpecularAlpha() { return m_specular_alpha; }

    void setEmissiveColor(double color[3]);
    void setEmissiveColor(double red, double green, double blue, double alpha);
    bool getEmissiveColor(double color[3]);
    double getEmissiveAlpha() { return m_emission_alpha; }

    void setShininess(double shininess)  {  m_shininess = shininess;  }
    bool getShininess(double &shininess);

    void setTransparency( double transparency );
    bool getTransparency( double &transparency );

    void setAmbientIntensity( double intensity );
    bool getAmbientIntensity( double &intensity );

    void setIsSmooth( bool b );
    bool getIsSmooth( bool &b );

    void addTarget( std::string target );
		size_t getTargets( std::vector<std::string> &vTargets );
};


////////////////////////////////////////////////////////////////////////////////
//  class AppearanceGeoreferencedTexture                                      //
//   Entspricht der CityGML-Klasse GeoreferencedTexture                       //
////////////////////////////////////////////////////////////////////////////////
class AppearanceGeoreferencedTexture : public AppearanceTexture
{
  public:
    AppearanceGeoreferencedTexture();
    virtual ~ AppearanceGeoreferencedTexture();

  protected:
    bool      m_preferWorldFile;  // preferWorldFile
    double    m_orientation[4];   //  orientation
    GeoPoint * m_referencePoint;  //  referencePoint

    bool      m_preferWorldFileSet; //  m_preferWorldFile wurde gesetzt

    std::vector<std::string> m_targets; //  gml:ids der zugeordneten Geometrieobjekte

  public:
    virtual AppearanceDataType getAppearanceDataType()  {  return GEOREFERENCED_TEXTURE;  }

    void setPreferWorldFile ( bool b );
    bool getPreferWorldFile ( bool & preferWorldFile );

    void setOrientation ( double orientation[4] ); 
    void getOrientation ( double orientation[4] );

    void setReferencePoint ( GeoPoint * referencePoint );
    GeoPoint * getReferencePoint ()  {  return m_referencePoint;  }

    void addTarget ( std::string target );
		size_t getTargets ( std::vector<std::string> &vTargets );
//    void addTarget ( tGeomID target );
};


///////////////////////////////////////////////////////////////////////////
//  class TexCoordRef                                                    //
//  Referenz auf ein Texturkoordinaten-Objekt                            //
///////////////////////////////////////////////////////////////////////////
class TexCoordRef
{
  public:
    TexCoordRef();
    virtual ~ TexCoordRef()  {  }

  public:
    std::string   m_targetGmlId;    //  Attribut uri in TextureAssociation
    unsigned int m_texCoordId;      //  Interne Id des referierten Texturkoordinaten-Objektes
    std::string   m_texCoordGmlId;  //  gml:id des referierten texturkoordinaten-Objektes
};


///////////////////////////////////////////////////////////////////////////
//  class TexCoord                                                       //
//  Abstrakte Basisklasse der Texturkoordinaten Klassen                  //
///////////////////////////////////////////////////////////////////////////
class TexCoord
{
  public:
    enum TextureCoordType
      { TEXTURE_COORD_GENERATION, TEXTURE_COORD_LIST, TEXTURE_COORD_POLYLOOP  };
 
  public:
    TexCoord ();
    virtual ~ TexCoord() {  }

  protected:
    unsigned int m_texCoordId;  //  Interne id des Objektes
    std::string  m_gmlId;       //  gml:id des Objektes

  public:
    virtual TextureCoordType getTextureCoordType() = NULL;

    void setTexCoordId ( unsigned int id )  {  m_texCoordId = id;  }
    unsigned int getTexCoordId ()  {  return m_texCoordId;  }

    void setGmlId ( std::string gmlId )  {  m_gmlId = gmlId;  }
    std::string getGmlId ()  {  return m_gmlId;  }
};


///////////////////////////////////////////////////////////////////////////
//  class TexCoordGen                                                    //
//   Entspricht der CityGML-Klasse TexCoordGen                           //
///////////////////////////////////////////////////////////////////////////
class TexCoordGen: public TexCoord
{
  public:
    TexCoordGen();
    virtual ~ TexCoordGen();

  protected:
    Matrix4 * m_worldToTexture;  //  worldToTexture

  public:
    virtual TextureCoordType getTextureCoordType()  {  return TEXTURE_COORD_GENERATION;  }

    void setTransformation ( Matrix4 * transformation )  {  m_worldToTexture = transformation;  }
    Matrix4 * getTransformation ()  {  return m_worldToTexture;  }
};


///////////////////////////////////////////////////////////////////////////
//   class TexCoordList                                                  //
//   Entspricht der CityGML-Klasse TexCoordList                          //
///////////////////////////////////////////////////////////////////////////
class TexCoordList : public TexCoord
{
  public:
    TexCoordList();
    virtual ~ TexCoordList();

  protected:
    std::map<std::string,TexCoordPolyloop*> m_texCoordinates; // ordnet einem Ring (ring) eine Liste von Texturkoordinaten (textureCoordinates) zu

  public:
    virtual TextureCoordType getTextureCoordType()  {  return TEXTURE_COORD_LIST;  }

    void addTexCoordPolyloop ( std::string gmlIdRing, TexCoordPolyloop * pTexCoords );
		size_t  getTexCoordPolyloops ( std::vector<std::string>       & vGmlIds,
                                std::vector<TexCoordPolyloop*> & vTexCoords );
};


///////////////////////////////////////////////////////////////////////////
//   class TexCoordPolyloop                                              //
//  Liste von Texturkoordinaten                                          //
///////////////////////////////////////////////////////////////////////////
class TexCoordPolyloop : public TexCoord
{
  public:
    TexCoordPolyloop( std::vector<double> texCoordinates  );
    virtual ~ TexCoordPolyloop()  {   }

  protected:
    std::vector<double> m_textureCoordinates;  //  Texturkoordinaten

  public:
    virtual TextureCoordType getTextureCoordType()  {  return TEXTURE_COORD_POLYLOOP;  }

		size_t getTextureCoords(  std::vector<double>  &vCoords );
    double getTextureCoord(int i) { return m_textureCoordinates.at(i); }
    void invertTextureCoords();

    const std::vector<double>& getTextureCoordinates() const { return m_textureCoordinates; }
};


