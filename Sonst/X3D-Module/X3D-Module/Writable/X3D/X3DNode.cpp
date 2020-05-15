#include "X3DNode.h"
#include "..\GlobalDef.h"

namespace X3D
{
  X3D::X3D()
  {
    m_profile						= Full;
    m_version						= "3.3";
    m_xmlns_xsd						= "http://www.w3.org/2001/XMLSchema-instance";
    m_xsd_noNamespaceSchemaLocation	= "http://www.web3d.org/specifications/x3d-3.3.xsd";
    m_width							= 100;
    m_height						= 100;
    m_scene							= new Scene();
  }


  X3D::X3D(const std::string& ident, Scene* scene)
  {
    setID(ident);
    m_profile						= Full;
    m_version						= "3.3";
    m_xmlns_xsd						= "http://www.w3.org/2001/XMLSchema-instance";
    m_xsd_noNamespaceSchemaLocation	= "http://www.web3d.org/specifications/x3d-3.3.xsd";
    m_width							= 100;
    m_height						= 100;
    m_scene							= scene;
  }

  X3D::~X3D()
  {

  }
  
  /*
  IDable* X3D::getElementById(const std::string ident)
  {
    std::string ownId = getID();

    if (ident == ownId)
    {
      return this;
    }
    else
    {
      return m_scene.getElementById(ident);
    }
  }
  */

  void X3D::write(tinyxml2::XMLPrinter &printer)
  {
    printer.OpenElement("X3D");

    IDable::write(printer);
    //DEFable::write(printer);

    printer.PushAttribute("profile", profileValues[m_profile]);
    printer.PushAttribute("version", m_version.c_str());
    printer.PushAttribute("xmlns:xsd", m_xmlns_xsd.c_str());
    printer.PushAttribute("xsd:noNamespaceSchemaLocation", m_xsd_noNamespaceSchemaLocation.c_str());

    if (X3DGlobals::X3DFileOnly == false)
    {

      char buffer[10];
      itoa(m_width, buffer, 10);
      std::string width(buffer);
      width += "%";
      printer.PushAttribute("width", width.c_str());
      itoa(m_height, buffer, 10);
      std::string height(buffer);
      height += "%";
      printer.PushAttribute("height", height.c_str());
    }

    //if (m_head == 0)
    //	m_head->write(printer);
    m_scene->write(printer);
    printer.CloseElement();
  }
}