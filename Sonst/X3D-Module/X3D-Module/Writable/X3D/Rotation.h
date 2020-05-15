#pragma once

#include "..\..\tinyxml2.h"
#include "Point3D.h"

namespace X3D
{
  class Rotation
  {
  public:
    Rotation();
    Rotation(double x, double y, double z, double a);
    Rotation(Point3D* direction, double a);
    virtual ~Rotation();

    double getX() const { return m_direction->getX(); }
    double getY() const { return m_direction->getY(); }
    double getZ() const { return m_direction->getZ(); }
    double getA() const { return m_angle; }

    //void setX(double x);
    //void setY(double y);
    //void setZ(double z);
    //void setR(double r);

  private:
    //attributes
    Point3D*	m_direction;
    double		m_angle;
  };
}
