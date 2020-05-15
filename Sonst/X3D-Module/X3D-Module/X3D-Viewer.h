#pragma once
#include "Writable\X3D\Point3D.h"
#include "Writable\X3D\Group.h"
#include "Writable\X3D\Grouping.h"
#include "Writable\X3D\X3DNode.h"
#include "Writable\X3D\Scene.h"
#include "VisualParams.h"

#include "Writable/XHTML/Dl.h"

#include <string>
#include <vector>
//using namespace System;

namespace X3D
{
  class X3D;

  class X3DViewer
  {
  public:
    X3DViewer() {}
    X3DViewer(const std::string& identX3D, const std::string& identScene, const std::string& identBackgrnd, const std::string& identViewpnt, std::string& gmlType, const std::string visualPathName, const std::string gmlFileName, const std::string staticPathName);

    virtual ~X3DViewer();

    void addSceneGraphMap(IDable* idableSceneGraph) { m_mSceneGraph[idableSceneGraph->getID()] = idableSceneGraph; }
    void addX3dMap(IDable* idableX3d) { m_mX3d[idableX3d->getID()] = idableX3d; }

    std::map<std::string, IDable*> getSceneGraphMap() const { return m_mSceneGraph; }
    std::map<std::string, IDable*> getX3dMap() const { return m_mX3d; }

    IDable* getElementById(const std::string ident, const e_dataType dataType);

    void setViewpointAttributes(const Point3D& min, const Point3D& max);
    const bool existID(const std::string ident);
    void setPredefinedAppearance(const std::string& ident); //, const std::string visualPathName);
    void writeX3D(const std::string fileName);
    void writeX3DOM(const std::string fileName);

    Dl*	getFeatureType() { return m_pFeatureType; }

    Group* getFeature(const std::string& ident);
    void setFeature(const std::string ident, const std::string parentID, const std::string classname, const std::string featureName, const bool hasChildren, const int childCount );
    void setXHTMLFeatureWithoutChild(const std::string ident, const std::string textNode, Input* inpu, Dl* parentDl, const bool hasLink);

    void setPointGeometry(const std::string& ident, const std::vector<Point3D*>& points, const std::string KlassenName);
    void setPointGeometry(const std::string& ident, const std::string& coordUse, const std::string klassenName);

    void setLineGeometry(const std::string& ident, const std::vector<Point3D*>& points, const std::vector<int>& coordIndex, const std::string parentID);
    void setLineGeometry(const std::string& ident, const std::string& coordUse, const std::vector<int>& coordIndex, const std::string KlassenName);

    void setFaceGeometry(const std::string& ident, const std::vector<Point3D*>& points, const std::vector<int>& coordIndex, const std::string parentID);

    Point3D*	getMinCoord() { return m_minCoord; }
    void		setMinCoord(Point3D* minCoord) { m_minCoord = minCoord; }

  private:
    std::string			m_gmlFileName;
    std::string			m_templatePathName;
    std::string			m_visualPathName;

    X3D*				m_x3d;
    Dl*					m_sceneGraph;
    Dl*					m_pFeatureType;
    X3DVisualParams*	m_visParams;

    Point3D*			m_minCoord;

    std::map<std::string, IDable*>	m_mSceneGraph;
    std::map<std::string, IDable*>	m_mX3d;
  };
}
