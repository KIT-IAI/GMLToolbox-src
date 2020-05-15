#include "Rotation.h"

//#include <stdio.h>
#include <cmath>

namespace X3D
{
  const double PI = acos(-1.);
  Rotation::Rotation()
  {
    m_direction = new Point3D(0., 0., 1.);
    m_angle = 0;
  }

  Rotation::Rotation(double x, double y, double z, double a)
  {
    m_direction = new Point3D(x, y, z);
    m_direction->normalize();

    if(a >= -PI && a <= PI)
      m_angle = a;
    else if (a < -PI)
      m_angle = -PI;
    else if (a > PI)
      m_angle = PI;
  }

  Rotation::Rotation(Point3D* direction, double a)
  {
    m_direction = new Point3D(*direction);
    m_direction->normalize();

    if(a >= -PI && a <= PI)
      m_angle = a;
    else if (a < -PI)
      m_angle = -PI;
    else if (a > PI)
      m_angle = PI;
  }

  Rotation::~Rotation()
  {
    delete m_direction;
  }

  /*void Rotation::setX(double x)
  {
    if(x >= -1 && x <= 1)
      m_x = x;
    else
      printf("Wrong value for x. The value should be between -1 and 1.");
  }
  
  void Rotation::setY(double y)
  {
    if(y >= -1 && y <= 1)
      m_y = y;
    else
      printf("Wrong value for y. The value should be between -1 and 1.");
  }
  
  void Rotation::setZ(double z)
  {
    if(z >= -1 && z <= 1)
      m_z = z;
    else
      printf("Wrong value for z. The value should be between -1 and 1.");
  }

  void Rotation::setR(double r)
  {
    if(r >= -(acos(-1.)) && r <= -(acos(-1.)))
      m_r = r;
    else
      printf("Wrong value for r. The value should be between -1 and 1.");
  }*/
}