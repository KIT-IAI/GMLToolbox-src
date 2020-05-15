#include "Point3D.h"
#include <math.h>

namespace X3D
{
  Point3D::Point3D()
  {
    m_x = 0;
    m_y = 0;
    m_z = 0;
  }

  Point3D::Point3D(double x, double y, double z)
  {
    m_x = x;
    m_y = y;
    m_z = z;
  }

  Point3D::Point3D(double xyz)
  {
    m_x = xyz;
    m_y = xyz;
    m_z = xyz;
  }

  Point3D::~Point3D()
  {
  }

  void Point3D::set(double x, double y, double z)
  {
    m_x = x;
    m_y = y;
    m_z = z;
  }

  double Point3D::length()
  {
    return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
  }

  void Point3D::normalize()
  {
    m_x = m_x / length();
    m_y = m_y / length();
    m_z = m_z / length();
  }
}