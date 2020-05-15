#pragma once

#include "..\Metaable.h"
#include "Point3D.h"
#include "Rotation.h"

namespace X3D
{
  class Viewpoint : public Metaable
  {
  public:
    Viewpoint();
    Viewpoint(const std::string& ident);
    Viewpoint(Point3D* position, Rotation* orientation, double& fieldOfView);
    Viewpoint(const std::string& ident, Point3D* position, Rotation* orientation, double& fieldOfView);
    virtual ~Viewpoint();

    void setDescription(const std::string description)		{ m_description = description; }
    void setPosition(Point3D* position)						{ m_position = position; }
    void setCenterOfRotation(Point3D* centerOfRotation)		{ m_centerOfRotation = centerOfRotation; }
    void setFieldOfView(double fieldOfView)					{ m_fieldOfView = fieldOfView; }
    void setOrientation(Rotation* orientation)				{ m_orientation = orientation; }

    std::string	getDescription()		const { return m_description; }
    Point3D*		getPosition()			const { return m_position; }
    Point3D*		getCenterOfRotation()	const { return m_centerOfRotation; }
    double			getFieldOfView()		const { return m_fieldOfView; }
    Rotation*		getOrientation()		const { return m_orientation; }

    //virtual IDable* getElementById(const std::string ident);

    void write(tinyxml2::XMLPrinter& printer);

  private:
    //attributes
    std::string		m_description;
    Point3D*		m_position;
    Point3D*		m_centerOfRotation;
    double			m_fieldOfView;
    Rotation*		m_orientation;

    void writePosition(tinyxml2::XMLPrinter& printer);
    void writeCenterOfRotation(tinyxml2::XMLPrinter& printer);
    void writeOrientation(tinyxml2::XMLPrinter& printer);
  };
}
