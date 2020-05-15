#pragma once

#include <stack>

#include <col/all.hpp>

#include "EntityVisitor.h"
#include "Geometrie.h"
#include "Appearance.h"

class AppearanceData;
class KmlColor;

class ColladaConverter
{
public:
    typedef std::map<AppearanceData*, col::MaterialPtr> MaterialPool;

    ColladaConverter( col::Context& context, std::string gmlFolderP, std::string kmlFolderP );

    virtual ~ColladaConverter();

    bool toCollada( Feature * pFeature, KmlColor & kmlColor, double offsetP[], CITYGML_LOD lodStufe, std::string thema );

    virtual void onBegin();
    virtual void onEnd();

    virtual void onBeginGeometry();
    virtual void onEndGeometry();

    virtual void onFace( Polygon& face, TexCoordPolyloop* texCoord = 0, AppearanceData* appearance = 0 );

    void setForceTriangulation(bool forceTriangulation) { m_forceTriangulation = forceTriangulation; }
    void setFlattenHierarchy(bool flattenHierarchy) { m_flattenHierarchy = flattenHierarchy; }


private:
    Features   * pFeatures;
    double       offset[3];
    col::Context m_context;

    std::string kmlFolder;
    std::string gmlFolder;

    col::Scene           & m_scene;
    col::MeshLibrary     & m_meshLib;
    col::MaterialLibrary & m_materialLib;
    col::ImageLibrary    & m_imageLibrary;
    col::NodeLibrary     & m_nodeLib;

    col::MeshPtr     m_mesh;
    col::MaterialPtr m_colorMaterial;
    col::NodePtr     m_node;

    std::stack<col::NodePtr> m_nodes;

    MaterialPool m_knownMaterial;

    col::Color m_color;
    
    bool m_colored;

    bool m_forceTriangulation;
    bool m_flattenHierarchy;

    std::string m_thema;

  private:
    bool toCollada( _Geometrie * geometry  );
    bool toCollada( Polygon * face );

};
