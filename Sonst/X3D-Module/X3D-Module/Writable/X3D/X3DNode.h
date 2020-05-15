#pragma once

#include "..\Metaable.h"
#include "Scene.h"
#include "..\..\EnumDefinition.h"

#include "..\..\tinyxml2.h"

using namespace tinyxml2;



namespace X3D{
  //class Scene;

  class X3D : public Metaable
  {
  public:
    X3D();
    X3D(const std::string& ident, Scene* scene);

    virtual ~X3D();

    void setProfile(const e_profile& profile) { m_profile = profile; }
    void setVersion(const std::string& version) { m_version = version; }
    void setXmlns_xsd(const std::string& xmlns_xsd) { m_xmlns_xsd = xmlns_xsd; }
    void setXsd_noNamespaceSchemaLocation(const std::string& xsd_noNamespaceSchemaLocation) { m_xsd_noNamespaceSchemaLocation = xsd_noNamespaceSchemaLocation; }
    void setWidth(const int& width) { m_width = width; }
    void setHeight(const int& height) { m_height = height; }
    
    //void setHead(const Head &head) { m_head = head; }
    void setScene(Scene* scene) { m_scene = scene; }

    Scene* getScene()	const { return m_scene; }

    //virtual IDable* getElementById(const std::string ident);

    void write(XMLPrinter &printer);

  private:
    //attributes
    e_profile   m_profile;
    std::string m_version;
    std::string m_xmlns_xsd;
    std::string m_xsd_noNamespaceSchemaLocation;
    int m_width;
    int m_height;

    //Child elements
    //Head			m_head;
    Scene*			m_scene;
  };
}
