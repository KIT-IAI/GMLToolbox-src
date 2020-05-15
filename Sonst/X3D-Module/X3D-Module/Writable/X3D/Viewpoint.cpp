#include "Viewpoint.h"

#include <string>
#include <sstream>
#include <iostream>

namespace X3D
{
  Viewpoint::Viewpoint()
  {
    m_position		= new Point3D(0., 0., 10.);
    m_fieldOfView	= 0.7854;
    m_orientation	= new Rotation(0., 0., 1., 0.);
    m_centerOfRotation = new Point3D(0., 0., 0.);
  }

  Viewpoint::Viewpoint(const std::string& ident)
  {
    setID(ident);
    m_position		= new Point3D(0., 0., 10.);
    m_fieldOfView	= 0.7854;
    m_orientation	= new Rotation(0., 0., 1., 0.);
    m_centerOfRotation = new Point3D(0., 0., 0.);
  }

  Viewpoint::Viewpoint(Point3D* position, Rotation* orientation, double& fieldOfView)
  {
    m_position		= position;
    m_fieldOfView	= fieldOfView;
    m_orientation	= orientation;
    m_centerOfRotation = new Point3D(0., 0., 0.);
  }

  Viewpoint::Viewpoint(const std::string& ident, Point3D* position, Rotation* orientation, double& fieldOfView)
  {
    setID(ident);
    m_position		= position;
    m_fieldOfView	= fieldOfView;
    m_orientation	= orientation;
    m_centerOfRotation = new Point3D(0., 0., 0.);
  }

  Viewpoint::~Viewpoint()
  {
    delete m_position;
    delete m_centerOfRotation;
    delete m_orientation;
  }


  /*
  IDable* Viewpoint::getElementById(const std::string ident)
  {
    std::string ownId = getID();

    if (ident == ownId)
    {
      return this;
    }
    return nullptr;
  }
  */

  void Viewpoint::write(tinyxml2::XMLPrinter& printer)
  {
    printer.OpenElement("Viewpoint");
    
    IDable::write(printer);
    DEFable::write(printer);
    EVENTable::write(printer);

    printer.PushAttribute("description", m_description.c_str());
    writePosition(printer);
    writeCenterOfRotation(printer);
    printer.PushAttribute("fieldOfView", m_fieldOfView);
    writeOrientation(printer);

    printer.CloseElement();
  }

  void Viewpoint::writePosition(tinyxml2::XMLPrinter& printer)
  {
    std::stringstream ss;

    ss << m_position->getX() << ' ';
    ss << m_position->getY() << ' ';
    ss << m_position->getZ();

    std::string s = ss.str();

    printer.PushAttribute("position", s.c_str());
  }

  void Viewpoint::writeCenterOfRotation(tinyxml2::XMLPrinter& printer)
  {
    std::stringstream ss;

    ss << m_centerOfRotation->getX() << ' ';
    ss << m_centerOfRotation->getY() << ' ';
    ss << m_centerOfRotation->getZ();

    std::string s = ss.str();

    printer.PushAttribute("centerOfRotation", s.c_str());
  }

  void Viewpoint::writeOrientation(tinyxml2::XMLPrinter& printer)
  {
    std::stringstream ss;

    ss << m_orientation->getX() << ' ';
    ss << m_orientation->getY() << ' ';
    ss << m_orientation->getZ() << ' ';
    ss << m_orientation->getA();

    std::string s = ss.str();

    printer.PushAttribute("orientation", s.c_str());
  }
}
