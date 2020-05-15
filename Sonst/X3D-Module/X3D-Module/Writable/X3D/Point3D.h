#pragma once

namespace X3D
{
  class Point3D
  {
    public:
      Point3D();
      Point3D(double x, double y, double z);
      Point3D(double xyz);
      virtual ~Point3D();

      void set(double x, double y, double z);

      double getX() const { return m_x; }
      double getY() const { return m_y; }
      double getZ() const { return m_z; }

      void setX(double x) { m_x = x; }
      void setY(double y) { m_y = y; }
      void setZ(double z) { m_z = z; }

    private:
      //attributes
      double m_x;
      double m_y;
      double m_z;

    public:
      double length();
      void normalize();

      Point3D operator- (const Point3D& rhs)
      {
        return Point3D(m_x - rhs.getX(), m_y - rhs.getY(), m_z - rhs.getZ());
      }

      Point3D operator+ (const Point3D& rhs)
      {
        return Point3D(m_x + rhs.getX(), m_y + rhs.getY(), m_z + rhs.getZ());
      }
  };

}
