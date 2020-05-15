#include "StdAfx.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Windows::Forms;
using namespace System::Text;

using namespace System::Collections;

#using <mscorlib.dll>

#include <boost/locale.hpp>

#include <col/all.hpp>

#include "Geometrie.h"
#include "ColladaConverter.h"
#include "KmlVisualisierung.h"
#include "Convert.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  Assertion Template convert für GeoPoint                                  //
///////////////////////////////////////////////////////////////////////////////
template <typename T>
T convert(const GeoPoint& p)
{
    assert(false);
    return T();
}

///////////////////////////////////////////////////////////////////////////////
//  Konversion GeoPoint --> col::Vector3                                     //
///////////////////////////////////////////////////////////////////////////////
template <>
col::Vector3 convert<col::Vector3>(  const GeoPoint & p )
{
    double x = p.getX();
    double y = p.getY();
    double z = p.getZ();

    return col::Vector3(x, y, z);
}

///////////////////////////////////////////////////////////////////////////////
//  Konversion GeoPoint --> col::Vertex                                     //
///////////////////////////////////////////////////////////////////////////////
template <>
col::Vertex convert<col::Vertex>(const GeoPoint & p)
{
    col::Vertex v;
    v.setPosition(convert<col::Vector3>(p));

    return v;
}

///////////////////////////////////////////////////////////////////////////////
//  Konversion AppearanceTexture::WrapMode --> col::wrap                     //
///////////////////////////////////////////////////////////////////////////////
col::Wrap convert(AppearanceTexture::WrapMode wrapMode)
{
    switch (wrapMode)
    {
    case AppearanceTexture::NONE:
        return col::NONE;
    case AppearanceTexture::MIRROR:
        return col::MIRROR;
    case AppearanceTexture::CLAMP:
        return col::CLAMP;
    case AppearanceTexture::BORDER:
        return col::BORDER;
    case AppearanceTexture::WRAP:
        return col::WRAP;
    }

    return col::NONE;
}

///////////////////////////////////////////////////////////////////////////////
//  Konversion Matrix4 --> col::Matrix4                                      //
///////////////////////////////////////////////////////////////////////////////
col::Matrix4 convert(const Matrix4 & mat)
{
    col::Matrix4 temp;
    double       h[12], hh;
    int          i, j;

    mat.getMatrix( h );

    for ( i = 0; i < 3; i++ )
    {
      for ( j = 0; j < 4; j++ )
      {
        hh = h[4*i+j];
        temp.set( j, i, hh );
      }
    }

/*
    for (size_t x(0); x < 4; ++x)
    {
        for (size_t y(0); y < 3; ++y)
        {
            double v = mat.get(unsigned int(y), unsigned int(x));
            temp.set(x, y, v);
        }
    }
*/
    temp.set(0, 3, 0.);
    temp.set(1, 3, 0.);
    temp.set(2, 3, 0.);
    temp.set(3, 3, 1.);

    return temp;
}

///////////////////////////////////////////////////////////////////////////////
//  Konversion KmlColor --> col::Color                                       //
///////////////////////////////////////////////////////////////////////////////
col::Color convert ( KmlColor kmlColor )
{
  col::Color color;
  double r, g, b, alpha;

  r     = float(kmlColor.getR()) / 255.0;
  g     = float(kmlColor.getG()) / 255.0;
  b     = float(kmlColor.getB()) / 255.0;
  alpha = float(kmlColor.getAlpha()) / 255.0;

  color.set( r, g, b, alpha );
  return color;
}


///////////////////////////////////////////////////////////////////////////////
//  ColladaConverter                                                         //
//  Konstruktor / Destruktor                                                 //
///////////////////////////////////////////////////////////////////////////////
ColladaConverter::ColladaConverter( col::Context& context, string gmlFolderP, string kmlFolderP )
                                   : m_scene(context.getScene())
                                   , m_meshLib(context.getMeshLibrary())
                                   , m_materialLib(context.getMaterialLibrary())
                                   , m_imageLibrary(context.getImageLibrary())
                                   , m_nodeLib(context.getNodeLibrary())
                                   , m_forceTriangulation(true)
                                   , m_flattenHierarchy(true)
                                   , gmlFolder ( gmlFolderP )
                                   , kmlFolder ( kmlFolderP )
{
}

ColladaConverter::~ColladaConverter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Dummy-Routinen am Anfang und Ende der Konversion eines Features          //
///////////////////////////////////////////////////////////////////////////////
void ColladaConverter::onBegin()
{

}

void ColladaConverter::onEnd()
{

}


///////////////////////////////////////////////////////////////////////////////
//  Konversion eines Polygons                                                //
///////////////////////////////////////////////////////////////////////////////
void ColladaConverter::onFace( Polygon & face, TexCoordPolyloop* texCoord, AppearanceData* appearance)
{
    vector<double>   vX, vY, vZ;
    vector<_Ring*>   vPolyloops;
    vector<GeoPoint> vPoints;
    unsigned int     i, j;
    double           nx, ny, nz;
    String         ^ texturePath;
    String         ^ textureFileName;
    String         ^ texturePathCopy;
    string           textureFileNameStr;

    if ( face.getCheckResult() < PLANARITY_CHECK_FULFILLED )
      return;

    Direction * normal = face.getNormal();
    normal->get( nx, ny, nz );

    col::MaterialPtr textureMaterial;
    col::MaterialPtr colorMaterial;
    col::Color       color;

    if ( appearance != NULL )
    {
      MaterialPool::const_iterator it = m_knownMaterial.find( appearance );

      if (it == m_knownMaterial.end())
      {
        if ( texCoord != NULL &&
          ( appearance->getAppearanceDataType() == AppearanceData::GEOREFERENCED_TEXTURE ||
          appearance->getAppearanceDataType() == AppearanceData::PARAMETRIZED_TEXTURE  ) )
        {
          col::ImagePtr image = m_context.createImage();
          m_imageLibrary.add(image);

          AppearanceTexture * pAppTexture = dynamic_cast<AppearanceTexture*>(appearance);

          texturePath = QuConvert::ToString( pAppTexture->getFilename() );
          if ( Path::GetPathRoot ( texturePath )->CompareTo( "" ) == 0 )
            texturePath = Path::Combine( QuConvert::ToString( gmlFolder ), texturePath );

          textureFileName = Path::GetFileName( texturePath );
          QuConvert::systemStr2stdStr( textureFileNameStr, textureFileName );
          image->setPath( textureFileNameStr );

          texturePathCopy = Path::Combine( QuConvert::ToString( kmlFolder ), textureFileName );
          File::Copy ( texturePath, texturePathCopy, true );

          col::Wrap wrap = convert( pAppTexture->getWrapMode() );

          col::TexturePtr texture = m_context.createTexture();
          texture->setImage(image);
          texture->setWrap(wrap);

          textureMaterial = m_context.createMaterial();
          m_materialLib.addMaterial(textureMaterial);
          textureMaterial->setTexture(texture);

          m_knownMaterial[appearance] = textureMaterial;

        }
        else
          if ( appearance->getAppearanceDataType() == AppearanceData::MATERIAL )
          {
            double diffuseColor[3];
            double alpha;

            AppearanceColor * pAppColor = dynamic_cast<AppearanceColor*>(appearance);
            pAppColor->getDiffuseColor( diffuseColor );
            alpha = pAppColor->getAmbientAlpha();

            col::Color color = col::Color ( diffuseColor[0], diffuseColor[1], diffuseColor[2], alpha );

            colorMaterial = m_context.createMaterial();
            m_materialLib.addMaterial( colorMaterial );
            colorMaterial->setColor(color);
          }
      }
      else
      {
        textureMaterial = it->second;
      }    
    }
    else
      colorMaterial = m_colorMaterial;

    size_t pointsSize = face.getAussenkontur()->getPointAnz();

    _Ring * contour = face.getAussenkontur();
    contour->getPoints3D( vX, vY, vZ );

    assert(vX.size() >= 3);

    col::PolygonsPtr polygons = m_context.createPolygons();

    if ( texCoord == NULL )
    {
       polygons->setMaterial( colorMaterial );
    }
    else
    {
      polygons->setMaterial( textureMaterial );
    }

    for ( i = 0; i < vX.size(); ++i )
    {
        GeoPoint p = GeoPoint ( vX[i]-offset[0], vY[i]-offset[1], vZ[i]-offset[2] );

        polygons->addIndex(m_mesh->getVertices().size());

        col::Vertex v = convert<col::Vertex>(p);
        v.setNormal(col::Vector3( nx, ny, nz ));

        if (texCoord != 0)
        {
            size_t texIndex = i * 2;

            const std::vector<double>& texCoords = texCoord->getTextureCoordinates();

            double s = texCoords[texIndex + 0];
            double t = texCoords[texIndex + 1];

            v.setTextureCoordinates(col::Vector2(s, t));
        }

        m_mesh->addVertex(v);
    }

//    std::vector<_Face*> holes = face.getInnerLoops();
//    for (std::vector<Polyloop*>::const_iterator it = holes.begin(); it != holes.end(); ++it)
    for ( j = 0; j < face.getLochAnz(); j++  )
    {
        _Ring * hole = face.getLoch( j );

        col::Indices colHole;

        vX.clear();
        vY.clear();
        vZ.clear();
        hole->getPoints3D( vX, vY, vZ );

//        for (std::vector<Point>::const_iterator it = points.begin(); it != points.end(); ++it)
        for ( i = 0; i < vX.size(); i++ )
        {
            colHole.push_back(m_mesh->getVertices().size());

            GeoPoint p = GeoPoint ( vX[i]-offset[0], vY[i]-offset[1], vZ[i]-offset[2] );
            col::Vertex v = convert<col::Vertex>( p );
            m_mesh->addVertex( v );
        }

        polygons->addHole(colHole);
    }

    if (m_forceTriangulation)
    {
        col::TrianglesPtr triangles = col::tesselate(m_context, m_mesh, polygons);
        m_mesh->addPrimitive( triangles );
    }
    else
    {
        m_mesh->addPrimitive( polygons );
    }
}

///////////////////////////////////////////////////////////////////////////////
//  Start der Konversion einer Geometrie                                     //
///////////////////////////////////////////////////////////////////////////////
void ColladaConverter::onBeginGeometry()
{
    // TODO: UWAH?

    m_mesh = m_context.createMesh();
    m_meshLib.addMesh(m_mesh);

    col::NodePtr node = m_context.createNode();

    if (m_flattenHierarchy)
    {
        m_node->addChild(node, col::REFERENCE);
        m_nodeLib.add(node);
    }
    else
    {
        m_node->addChild(node);
    }

    m_colorMaterial = m_materialLib.find( m_color );

    if ( m_colorMaterial.get() == 0 )
    {
        m_colorMaterial = m_context.createMaterial();
        m_materialLib.addMaterial(m_colorMaterial);
        m_colorMaterial->setColor(m_color);
    }

    node->setMaterial(m_colorMaterial);

    node->setMesh(m_mesh);

    //m_node = node;
}

///////////////////////////////////////////////////////////////////////////////
//  Ende der Konversion einer Geometrie                                      //
///////////////////////////////////////////////////////////////////////////////
void ColladaConverter::onEndGeometry()
{
    m_mesh = col::MeshPtr();
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertiert die Geometrie eines Features nach Collada                    //
///////////////////////////////////////////////////////////////////////////////
bool ColladaConverter::toCollada( Feature * pFeature, KmlColor & kmlColor, double offsetP[], 
                                 CITYGML_LOD lodStufe, string thema )
{
  vector<_Geometrie*> vGeometries;
  _Geometrie        * pGeometry;
  int                 i, anz;
  bool                geometryExist = false;
  bool                geometryExistLoc;

  m_thema = thema;

  m_node = m_context.createNode();
  m_nodes.push(m_node);

  pFeatures = pFeature->getFeatures();
  m_color   = convert ( kmlColor );

  for ( i = 0; i < 3; i++ )
    offset[i] = offsetP[i];

  onBegin();

  anz = pFeature->getGeometry( vGeometries, lodStufe );

  for ( i = 0; i < anz; i++ )
  {
    pGeometry = vGeometries[i];

    onBeginGeometry();

    geometryExistLoc = toCollada( pGeometry );
    if ( geometryExistLoc == true )
      geometryExist = true;

    onEndGeometry();
  }

  col::NodePtr node = m_nodes.top();
  m_nodes.pop();

  if (node->hasChildren() || node->hasMesh())
  {
    if (m_nodes.empty())
    {
      m_scene.addNode(node);
    }
    else
    {
      col::NodePtr parent = m_nodes.top();
      node->setParent(parent);

      if (m_flattenHierarchy)
      {
        m_nodeLib.add(node);
        parent->addChild(node, col::REFERENCE);
      }
      else
      {
        parent->addChild(node);
      }
    }
  }

  return geometryExist;
}



///////////////////////////////////////////////////////////////////////////////
//  Konvertiert eine beliebige Geometrie nach Collada                        //
///////////////////////////////////////////////////////////////////////////////
bool ColladaConverter::toCollada ( _Geometrie * pGeometry )
{
  unsigned int i, anz;
  bool     geometryExist = false;
  bool     geometryExistLoc;


  GEOMETRY_TYPE type = pGeometry->getGeometryType();

  switch (type)
  {
  case MULTI_GEOMETRY:
    {
      vector<_Geometrie*> vMembers;

      MultiGeometry * pMultiGeometry = dynamic_cast<MultiGeometry*>(pGeometry);
      for ( i = 0; i < pMultiGeometry->getGeometryMemberAnz(); i++ )
      {
        _Geometrie * pGeo = pMultiGeometry->getGeometry( i );
        geometryExistLoc = toCollada ( pGeo );
        if ( geometryExistLoc == true )
          geometryExist = true;
      }

      break;
    }


  case GEOMETRIC_COMPLEX:
    {
      vector<_Geometrie*> vMembers;

      GeometricComplex * pMultiGeometry = dynamic_cast<GeometricComplex*>(pGeometry);
      for ( i = 0; i < pMultiGeometry->getGeometryMemberAnz(); i++ )
      {
        _Geometrie * pGeo = pMultiGeometry->getGeometry( i );
        geometryExistLoc = toCollada ( pGeo );
        if ( geometryExistLoc == true )
          geometryExist = true;
      }

      break;
    }

  case MULTI_SURFACE:
  case COMPOSITE_SURFACE:
  case SHELL:
  case ORIENTABLE_SURFACE:
  case SURFACE:
  case TRIANGULATED_SURFACE:
    {
      vector <Polygon*> vPolygone;
      vector<bool>      vOrientations;

      _Surface * pAbstractSurface = dynamic_cast<_Surface*>(pGeometry);
      anz =  pAbstractSurface->getPolygone( vPolygone, vOrientations );

      for ( i = 0; i < anz; i++ )
      {
        geometryExistLoc = toCollada( vPolygone[i] );
        if ( geometryExistLoc == true )
          geometryExist = true;
      }

      break;
    }

  case SOLID:
  case MULTI_SOLID:
    {
      vector <Polygon*> vPolygone;
      vector<bool>      vOrientations;

      _Solid * pAbstractSolid = dynamic_cast<_Solid*>(pGeometry);
      anz = pAbstractSolid->getAussenkonturPolygone( vPolygone, vOrientations );

      for ( i = 0; i < anz; i++ )
      {
        geometryExistLoc = toCollada( vPolygone[i] );
        if ( geometryExistLoc == true )
          geometryExist = true;
      }

      break;
    }

  case POLYGON:
  case TRIANGLE:
    {
      Polygon * pPolygon = dynamic_cast<Polygon*>(pGeometry);

      geometryExist = toCollada( pPolygon );
      break;
    }

  default:
    {
      break;
    }
  }
  return geometryExist;
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertiert ein Polygon nach Collada                                     //
///////////////////////////////////////////////////////////////////////////////
bool ColladaConverter::toCollada( Polygon * pPolygon )
{
  if ( pPolygon->getAussenkontur() != NULL )
  {
    vector<string>            themen;
    vector<AppearanceData*>   appearanceData;
    vector<TexCoordPolyloop*> texCoords;
    AppearanceData          * appearance;
    TexCoordPolyloop        * texCoord;
    unsigned int              i;

    pFeatures->getGeoAppearanceTexture(pPolygon->getAussenkontur()->getGmlId(), themen, appearanceData, texCoords );
    pFeatures->getGeoAppearanceColor( pPolygon->getAussenkontur()->getGmlId(), themen, appearanceData );

    if ( themen.size() > 0 )
    {
      for ( i = 0; i < themen.size(); i++ )
      {
        string thema = themen[i];
        if ( m_thema != "" && thema != m_thema )
          continue;

        appearance = appearanceData[i];
        if ( i < texCoords.size() )
          texCoord = texCoords[i];
        else
          texCoord = NULL;

        onFace( *pPolygon, texCoord, appearance);
        break;
      }
    }
    else
    {
      onFace( *pPolygon );
    }
    return true;
  }
  else return false;
}


