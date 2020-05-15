// X3D-Viewer.cpp: Hauptprojektdatei.



#include "X3D-Viewer.h"
#include "Writable\GlobalDef.h"
#include "Writable\X3D\Coordinate.h"
#include "Writable\X3D\PointSet.h"
#include "Writable\X3D\IndexedLineSet.h"
#include "Writable\X3D\IndexedFaceSet.h"
#include "Writable\X3D\IndexedTriangleSet.h"
#include "Writable\X3D\X3DAppearance.h"
#include "Writable\X3D\Shape.h"
#include "Writable\X3D\Group.h"
#include "Writable\X3D\Switch.h"
#include "Writable\X3D\Grouping.h"
#include "Writable\X3D\Viewpoint.h"
#include "Writable\X3D\Background.h"
#include "Writable\X3D\X3DColor.h"
//#include "VisualParams.h"

#include "tinyxml2.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

//using namespace System;

// int main(array<System::String ^> ^args)
// {
//     Console::WriteLine(L"Hello World");
//     return 0;
// }
namespace X3D
{
  X3DViewer::X3DViewer(const std::string& identX3D, const std::string& identScene, const std::string& identBackgrnd,
              const std::string& identViewpnt, std::string& gmlType, const std::string visualPathName, 
              const std::string gmlFileName, const std::string templatePathName )
  {
    m_gmlFileName      = gmlFileName;
    m_templatePathName = templatePathName;
    m_visualPathName   = visualPathName;

    Color	 skyColor(1., 1, 1.);
    double transparency	= 0.0;

    if ( !visualPathName.empty() )
    {
      X3DVisualParamReader visParamRead( gmlType );
      m_visParams	= visParamRead.read(visualPathName);
      ColorWithTransparency* bgSceneColor = m_visParams->getBgSceneColor();
      skyColor		= Color(bgSceneColor->getR(), bgSceneColor->getG(), bgSceneColor->getB());
      transparency	= bgSceneColor->getTransparency();
    }
    else
    {
      m_visParams		= nullptr;
    }
    
    //////////////////
    //	X3D part	//
    //////////////////
    Viewpoint*	viewpnt		= new Viewpoint(identViewpnt);
    Background*	backgrnd	= new Background(identBackgrnd, skyColor, transparency);
    
    Switch*		pSwitchFace		= new Switch("CityModel_Switch", 0);
    Switch*		pSwitchLine		= new Switch("CityModel_Switch_Line", -1);
    Switch*		pSwitchPoint	= new Switch("CityModel_Switch_Point", -1);

    Group*		pGroupFaceRootFeature	= new Group("CityModel");
    Group*		pGroupLineRootFeature	= new Group("CityModel_Line");
    Group*		pGroupPointRootFeature	= new Group("CityModel_Point");

    pGroupFaceRootFeature->setName("CityModel");
    pGroupFaceRootFeature->addMetadataString("X3D:LabelName_CityModel", "Karlsruhe");
    pGroupLineRootFeature->setName("CityModel_Line");
    pGroupPointRootFeature->setName("CityModel_Point");
    
    addX3dMap(pGroupFaceRootFeature);
    addX3dMap(pGroupLineRootFeature);
    addX3dMap(pGroupPointRootFeature);

    pSwitchFace->addGrouping(pGroupFaceRootFeature);
    pSwitchLine->addGrouping(pGroupLineRootFeature);
    pSwitchPoint->addGrouping(pGroupPointRootFeature);

    pSwitchFace->setClass("Face");
    pSwitchLine->setClass("Line");
    pSwitchPoint->setClass("Point");

    addX3dMap(pSwitchFace);
    addX3dMap(pSwitchLine);
    addX3dMap(pSwitchPoint);

    Scene* scene =	new Scene(backgrnd, viewpnt);
    scene->addGrouping(pSwitchFace);
    scene->addGrouping(pSwitchLine);
    scene->addGrouping(pSwitchPoint);

    addX3dMap(scene);

    m_x3d		= new X3D(identX3D, scene);
    addX3dMap(m_x3d);


    //////////////////
    //	XHTML part	//
    //////////////////
    // Scene graph
    Button* pBut = new Button("CityModel_Button", "CityModel", "-");
    pBut->setOnclick("expand(this.value)");
    addSceneGraphMap(pBut);

    Input* pInpu = new Input("CityModel_Layer", "CityModel");
    pInpu->setName("Layer");
    addSceneGraphMap(pInpu);

    A*	pA = new A("CityModel_Link", "CityModel", "Karlsruhe");
    pA->setOnclick("select(this)");
    pA->setOnmouseover("this.style.cursor = \'pointer\'");
    addSceneGraphMap(pA);

    Dt* pDtRootFeature = new Dt("CityModel_Active", "LayerLink0", pBut, pInpu, pA);
    addSceneGraphMap(pDtRootFeature);
    
    Dl*  pDlExpand = new Dl();
    pDlExpand->setID("CityModel_Expand");
    addSceneGraphMap(pDlExpand);

    Dd*  pDdChildElements = new Dd( pDlExpand);
    addSceneGraphMap(pDdChildElements);

    m_sceneGraph	= new Dl(pDtRootFeature,  pDdChildElements);
    addSceneGraphMap(m_sceneGraph);

    // Feature Type
    Dt* pDtFeatureClass = new Dt();
    m_pFeatureType = new Dl();
    m_pFeatureType->addChild(pDtFeatureClass);
  }

  X3DViewer::~X3DViewer()
  {
    delete m_x3d;
    delete m_sceneGraph;
    delete m_pFeatureType;
    delete m_visParams;
  }

  IDable* X3DViewer::getElementById(const std::string ident, const e_dataType dataType)
  {
    if (dataType == x3d || dataType == all)
    {
      std::map<std::string, IDable*>::iterator it = m_mX3d.find(ident);
      if (it != m_mX3d.end())
      {
        return it->second;
      }
    }
    else if (dataType == sceneGraph || dataType == all)
    {
      std::map<std::string, IDable*>::iterator it = m_mSceneGraph.find(ident);
      if (it != m_mSceneGraph.end())
      {
        return it->second;
      }
    }

    return nullptr;
  }

  void X3DViewer::setViewpointAttributes(const Point3D& min, const Point3D& max)
  {
    Point3D center( 0.5*(max.getX() - min.getX()), 0.5*(max.getY() - min.getY()), 0.5*(max.getZ() - min.getZ()) );

    double width = __max(max.getX() - min.getX(), max.getY() - min.getY());

    std::list<Viewpoint*> viewpoints = m_x3d->getScene()->getViewpoints();

    Point3D  *	position = new Point3D(-center.getX(), center.getZ() + 1.5*width, center.getY());
    Point3D  *	centerOfRotation = new Point3D(-center.getX(), center.getZ(), center.getY());
    Point3D  * direction = new Point3D(-1., 0., 0.);
    Rotation * orientation = new Rotation(direction, 1.570796327);

    std::list<Viewpoint*>::iterator iter = viewpoints.begin();
    (*iter)->setPosition(position);
    (*iter)->setCenterOfRotation(centerOfRotation);
    (*iter)->setOrientation(orientation);

    Scene *scene = m_x3d->getScene();

    scene->setViewpoints(viewpoints);
  }

  const bool X3DViewer::existID(const std::string ident)
  {
    IDable* elem = getElementById(ident, x3d);

    if (elem != nullptr)
    {
      return true;
    }
    elem = getElementById(ident, sceneGraph);
    if (elem != nullptr)
    {
      return true;
    }

    return false;
  }

  void X3DViewer::setPredefinedAppearance(const std::string& ident) //, const std::string visualPathName)
  {
    Color* difColor = new Color(0.8);
    double  transparency = 0.0;

    if (!m_visParams || !m_visParams->getKlassenVisualParams(ident))
    {
      if (ident == "bldg:RoofSurface")
      {
        difColor->setR(1.0);
        difColor->setG(0.0);
        difColor->setB(0.0);
      }
      else if (ident == "bldg:WallSurface")
      {
        difColor->setR( 0.8 );
        difColor->setG( 0.8 );
        difColor->setB( 0.8 );
      }
      else if (ident == "bldg:GroundSurface")
      {
        difColor->setR( 0.4 );
        difColor->setG( 0.4 );
        difColor->setB( 0.4 );
      }
      else if (ident == "bldg:BuildingInstallation")
      {
        difColor->setR( 0.87 );
        difColor->setG( 0.72 );
        difColor->setB( 0.53 );
      }
      else if (ident == "bldg:Building" || ident == "bldg:BuildingPart" )
      {
        difColor->setR( 0.0 );
        difColor->setG( 0.9 );
        difColor->setB( 0.93 );
      }
      else if (ident == "bldg:Room")
      {
        difColor->setR( 0.68 );
        difColor->setG( 0.85 );
        difColor->setB( 0.90 );
      }
      else if (ident == "bldg:CeilingSurface" || ident == "bldg:OuterCeilingSurface" )
      {
        difColor->setR( 0.8 );
        difColor->setG( 0.8 );
        difColor->setB( 0.8 );
      }
      else if (ident == "bldg:Floor")
      {
        difColor->setR( 0.4 );
        difColor->setG( 0.4 );
        difColor->setB( 0.4 );
      }
      else if (ident == "bldg:Door")
      {
        difColor->setR( 0.0 );
        difColor->setG( 0.0 );
        difColor->setB( 1.0 );
        transparency = 0.5;
      }
      else if (ident == "bldg:Window")
      {
        difColor->setR( 0.0 );
        difColor->setG( 1.0 );
        difColor->setB( 1.0 );
        transparency = 0.5;
      }
      else if (ident == "bldg:ClosureSurface")
      {
        difColor->setR( 0.8 );
        difColor->setG( 0.8 );
        difColor->setB( 0.8 );
      }

    }
    else
    {
      X3DKlassenVisualParams* pClassVisParams = m_visParams->getKlassenVisualParams(ident);

      ColorWithTransparency*	pColorWithTrans = pClassVisParams->getKlassenColor();
      difColor->setR(pColorWithTrans->getR());
      difColor->setG(pColorWithTrans->getG());
      difColor->setB(pColorWithTrans->getB());
      transparency = pColorWithTrans->getTransparency();
    }

    ColorWithTransparency*	pColorWithTrans;
    Color* emisColor = new Color(0.);

    if (ident == "Select")
    {
      if (!m_visParams || !m_visParams->getSelectElementColor())
      {
        difColor->setR(1.);
        difColor->setG(1.);
        difColor->setB(0.);
        transparency = 0,5;
      }
      else
      {
        pColorWithTrans = m_visParams->getSelectElementColor();
        difColor->setR(pColorWithTrans->getR());
        difColor->setG(pColorWithTrans->getG());
        difColor->setB(pColorWithTrans->getB());
        transparency = pColorWithTrans->getTransparency();
      }
    }
    else if (ident == "InfoSelect")
    {
      if (!m_visParams || !m_visParams->getSelectInfoParam())
      {
        emisColor->setR(1.);
        emisColor->setG(1.);
        emisColor->setB(0.);
      }
      else
      {
        X3DAdvanced*	pColorAdvanced = m_visParams->getSelectInfoParam();
        emisColor->setR(pColorAdvanced->getR());
        emisColor->setG(pColorAdvanced->getG());
        emisColor->setB(pColorAdvanced->getB());
        transparency = pColorAdvanced->getTransparency();
      }
    }

    Scene* scene = m_x3d->getScene();

    Material* ma = new Material(ident + "_Material", difColor);
    ma->setEmissiveColor(emisColor);
    ma->setTransparency(transparency);

    Appearance* ap = new Appearance(ident + "_Appearance", ma);
    Shape* sh = new Shape(ident, ap);

    if (ident == "Select")
    {
      sh->setOnclick("select(this)");
    }

    scene->addShape(sh);
    m_x3d->setScene(scene);

    addX3dMap( sh );
  }

  void X3DViewer::writeX3DOM(const std::string fileName)
  {
    FILE* fi = nullptr;

    errno_t err = fopen_s(&fi, fileName.c_str(), "w");

    std::string   line;
    std::ifstream fRead( m_templatePathName );

    tinyxml2::XMLPrinter printer(fi);

    if (fRead.is_open())
    {
      while(getline (fRead, line))
      {
        if(line == "#X3D")
        {
          m_x3d->write(printer);
        }
        else if (line == "#SceneGraph")
        {
          m_sceneGraph->write(printer);
        }
        else if (line == "#FeatureType")
        {
          m_pFeatureType->write(printer);
        }
        else if (line == "#FileName")
        {
          fwrite(m_gmlFileName.c_str(), sizeof(char), m_gmlFileName.length(), fi);
        }
        else
        {
          line += wchar_t('\n');
          fwrite(line.c_str(), sizeof(char), line.size(), fi);
        }
      }
    }

    fclose(fi);
  }

  void X3DViewer::writeX3D(const std::string fileName)
  {
    X3DGlobals::X3DFileOnly = true;

    FILE* fi = nullptr;
    errno_t err = fopen_s(&fi, fileName.c_str(), "w");

    tinyxml2::XMLPrinter printer(fi);

    printer.PushHeader(false, true);
    m_x3d->write(printer);

    fclose(fi);
  }

  Group* X3DViewer::getFeature(const std::string& ident)
  {
    std::string id(ident);
//    std::replace_if(id.begin(), id.end(), [](char c) { return !isalnum(c); }, '_');

    IDable* pGroupIDable = getElementById(id, x3d);
    if ( pGroupIDable != nullptr)
    {
      Group* pGroup = dynamic_cast<Group *>(pGroupIDable);
      return pGroup;
    }

    return nullptr;
  }

  void X3DViewer::setFeature(const std::string ident, const std::string parentID, const std::string classname, const std::string featureName, const bool hasChildren, const int childCount )
  {
    Group* pGroup;

    std::string id(ident);
//    std::replace_if(id.begin(), id.end(), [](char c) { return !isalnum(c); }, '_');

    std::string idParent(parentID);
//    std::replace_if(idParent.begin(), idParent.end(), [](char c) { return !isalnum(c); }, '_');

    if(!existID(id))
    {
      //////////////////
      // X3D part     //
      //////////////////

      pGroup = new Group(id);
      pGroup->setClass(classname);
      pGroup->setName(id);
      pGroup->addMetadataString("X3D:LabelName_" + id, featureName);
      addX3dMap(pGroup);

      if (getFeature(idParent) )
      {
        getFeature(idParent)->addGrouping( pGroup);
      }

      pGroup = new Group(id + "_Line");
      pGroup->setClass(classname);
      pGroup->setName(id + "_Line");
      addX3dMap(pGroup);

      if (getFeature(idParent + "_Line"))
      {
        getFeature(idParent + "_Line")->addGrouping( pGroup);
      }

      pGroup = new Group(id + "_Point");
      pGroup->setClass(classname);
      pGroup->setName(id + "_Point");
      addX3dMap(pGroup);

      if (getFeature(idParent + "_Point"))
      {
        getFeature(idParent + "_Point")->addGrouping( pGroup);
      }

      //////////////////
      // XHTML part   //
      //////////////////
      // Scene graph
      Input* pInpu = new Input(id + "_Layer", id);
      pInpu->setName("Layer");
      pInpu->setOnchange("visibility(this)");
      addSceneGraphMap(pInpu);

      IDable* pParentIDable = getElementById(idParent + "_Expand", sceneGraph);
      Dl* pParentDl = nullptr;
      if ( pParentIDable != nullptr)
      {
        pParentDl = dynamic_cast<Dl*>(pParentIDable);
      }

      if (hasChildren )
      {
        Button* pBut = new Button(id + "_Button", id, "-");
        pBut->setOnclick("expand(this.value)");
        addSceneGraphMap(pBut);

        std::string textNode;
        std::ostringstream stream;
        stream << featureName << " (" << childCount << ")";
        textNode = stream.str();

        A* pA = new A(id + "_Link", id, textNode);
        pA->setOnclick("select(this)");
        pA->setOnmouseover("this.style.cursor = \'pointer\'");
        addSceneGraphMap(pA);

        Dt* pDtFeature = new Dt(id + "_Active", "LayerLink0", pBut, pInpu, pA);
        addSceneGraphMap(pDtFeature);

        Dt* pDt = new Dt();
        addSceneGraphMap(pDt);

        Dl* pDlExpand = new Dl();
        pDlExpand->setID(id + "_Expand");
        pDlExpand->addChild(pDt);
        addSceneGraphMap(pDlExpand);

        Dd* pDdChildElements = new Dd(pDlExpand);
        addSceneGraphMap(pDdChildElements);

         pParentDl->addChild(pDtFeature);
         pParentDl->addChild(pDdChildElements);
      }
      else
      {
        setXHTMLFeatureWithoutChild(id, featureName, pInpu, pParentDl, true);
        /*
        A*	a = new A(ident + "_Link", ident, featureName);
        a->setOnclick("select(this)");
        a->setOnmouseover("this.style.cursor = \'pointer\'");

        Dd*	dd = new Dd(ident + "_Active", "LayerLink0", pInpu, pA);

        pParentDl->addChild(dd);*/
      }
    }
  }

  void X3DViewer::setXHTMLFeatureWithoutChild(const std::string ident, const std::string textNode, Input* pInpu, Dl*  pParentDl, const bool hasLink)
  {
    std::string id(ident);
//    std::replace_if(id.begin(), id.end(), [](char c) { return !isalnum(c); }, '_');

    Dd* pDdFeature;
    if(hasLink)
    {
      A* pA = new A(id + "_Link", id, textNode);
      pA->setOnclick("select(this)");
      pA->setOnmouseover("this.style.cursor = \'pointer\'");
      addSceneGraphMap(pA);

      pDdFeature = new Dd(id + "_Active", "LayerLink0", pInpu, pA);
      addSceneGraphMap(pDdFeature);
    }
    else
    {
      pDdFeature = new Dd(id + "_Active", "LayerLink1", pInpu);
      pDdFeature->setTextNode(textNode);
      addSceneGraphMap(pDdFeature);
    }

     pParentDl->addChild(pDdFeature);
  }

  void X3DViewer::setPointGeometry(const std::string& ident, const std::vector<Point3D*>& points, const std::string klassenName)
  {
    std::string id(ident);
//    std::replace_if(id.begin(), id.end(), [](char c) { return !isalnum(c); }, '_');

    std::string			identBuffer	= id + "_Points";
    Coordinate*			coord		= new Coordinate(identBuffer, points, m_minCoord );
    addX3dMap(coord);

    identBuffer	= id + "_PointSet";
    PointSet*			ps			= new PointSet(identBuffer, coord);
    addX3dMap(ps);

    identBuffer	= id + "_Point_Appearance";
    Appearance*			ap			= new Appearance(identBuffer, klassenName + "_Appearance");
    addX3dMap(ap);

    identBuffer	= id + "_Point_Geometry";
    Shape*				sh			= new Shape(identBuffer, ap, ps);
    //sh.setName(ident);
    addX3dMap(sh);

    if ( !existID( klassenName ) )
    {
      setPredefinedAppearance(klassenName);
    }

    if (getFeature(id))
    {
      getFeature(id)->addShape(sh);
    }
  }

  void X3DViewer::setPointGeometry(const std::string& ident, const std::string& coordUse, const std::string klassenName)
  {
    std::string id(ident);
//    std::replace_if(id.begin(), id.end(), [](char c) { return !isalnum(c); }, '_');

    std::string			identBuffer	= id + "_Points";
    Coordinate*			coord		= new Coordinate(coordUse);
    addX3dMap(coord);

    identBuffer	= id + "_PointSet";
    PointSet*			ps			= new PointSet(identBuffer, coord);
    addX3dMap(ps);

    identBuffer	= id + "_Point_Appearance";
    Appearance*			ap			= new Appearance(identBuffer, klassenName + "_Appearance");
    addX3dMap(ap);

    identBuffer	= id + "_Point_Geometry";
    Shape*				sh			= new Shape(identBuffer, ap, ps);
    //sh.setName(ident);
    addX3dMap(sh);

    if (!existID(klassenName ) )
    {
      setPredefinedAppearance(klassenName);
    }

    if (getFeature(id))
    {
      getFeature(id)->addShape(sh);
    }
  }

  void X3DViewer::setLineGeometry(const std::string& ident, const std::vector<Point3D*>& points, const std::vector<int>& coordIndex, const std::string klassenName)
  {
    std::string id(ident);
//    std::replace_if(id.begin(), id.end(), [](char c) { return !isalnum(c); }, '_');

    std::string			identBuffer	= id + "_Points";
    Coordinate*			coord		= new Coordinate(identBuffer, points, m_minCoord );
    addX3dMap(coord);

    identBuffer	= id + "_IndexedLineSet";
    IndexedLineSet*		ils			= new IndexedLineSet(identBuffer, coordIndex, coord);
    addX3dMap(ils);

    identBuffer	= id + "_Line_Appearance";
    Appearance*			ap			= new Appearance(identBuffer, klassenName + "_Appearance");
    addX3dMap(ap);

    identBuffer	= id + "_Line_Geometry";
    Shape*				sh			= new Shape(identBuffer, ap, ils);
    //sh.setName(ident);
    addX3dMap(sh);

    if (!existID(klassenName ) )
    {
      setPredefinedAppearance(klassenName);
    }

    if (getFeature(id))
    {
      getFeature(id)->addShape(sh);
    }
  }

  void X3DViewer::setLineGeometry(const std::string& ident, const std::string& coordUse, const std::vector<int>& coordIndex, const std::string KlassenName)
  {
    std::string id(ident);
//    std::replace_if(id.begin(), id.end(), [](char c) { return !isalnum(c); }, '_');

    std::string			identBuffer	= id + "_Points";
    Coordinate*			coord		= new Coordinate(coordUse);
    addX3dMap(coord);

    identBuffer	= id + "_IndexedLineSet";
    IndexedLineSet*		ils			= new IndexedLineSet(identBuffer, coordIndex, coord);
    addX3dMap(ils);

    identBuffer	= id + "_Line_Appearance";
    Appearance*			ap			= new Appearance(identBuffer, KlassenName + "_Appearance");
    addX3dMap(ap);

    identBuffer	= id + "_Line_Geometry";
    Shape*				sh			= new Shape(identBuffer, ap, ils);
    //sh.setName(ident);
    addX3dMap(sh);

    if ( !existID( KlassenName ) )
    {
      setPredefinedAppearance(KlassenName);
    }

    if (getFeature(id))
    {
      getFeature(id)->addShape(sh);
    }
  }

 

  void X3DViewer::setFaceGeometry(const std::string& ident, const std::vector<Point3D*>& points, const std::vector<int>& coordIndex, const std::string klassenName)
  {
    std::string id(ident);
//    std::replace_if(id.begin(), id.end(), [](char c) { return !isalnum(c); }, '_');

    std::string			identBuffer	= id + "_Points";
    Coordinate*			coord		= new Coordinate(identBuffer, points, m_minCoord );
    addX3dMap(coord);

    identBuffer	= id + "_IndexedFaceSet";
    IndexedFaceSet*		ifs			= new IndexedFaceSet(identBuffer, coordIndex, coord);
    addX3dMap(ifs);

    identBuffer	= id + "_Face_Appearance";
    Appearance*			ap = new Appearance(identBuffer, klassenName + "_Appearance");
    addX3dMap(ap);

    identBuffer	= id + "_Face_Geometry";
    Shape*				sh = new Shape(identBuffer, ap, ifs);
    //sh.setName(ident);
    sh->setOnclick("select(this.parentNode, event)");
    sh->setOnmouseover("this.style.cursor = 'pointer';getInfo(this.parentNode, event)");
    sh->setOnmouseout("deleteInfo(this.parentNode, event)");
    sh->setOnmousedown("deleteInfo(this.parentNode, event);setMousePosition(event)");
    addX3dMap(sh);

    if ( !existID(klassenName ) )
    {
      setPredefinedAppearance(klassenName);
    }

    if (getFeature(id))
    {
      getFeature(id)->addShape(sh);
    }
  }

}
