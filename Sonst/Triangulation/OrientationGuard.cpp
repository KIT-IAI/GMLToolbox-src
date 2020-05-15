#include "StdAfx.h"

#include "OrientationGuard.hpp"

using namespace IfcDB;

OrientationGuard::OrientationGuard()
: m_inverted(false)
{

}


void OrientationGuard::invertContour(Polyloop& contour)
{
  bool area(true);

  bool oriented = isPositivelyOriented(Matrix4(), contour.getPoints(), area);

  if (!area)
  {
    throw std::exception("OuterLoop is not a valid area");
  }

  if (!oriented)
  {
    contour.inverse();
    m_inverted = true;
  }
}


void OrientationGuard::invertHole(Polyloop& hole)
{
  bool area(true);

  bool oriented = isPositivelyOriented(Matrix4(), hole.getPoints(), area);

  if (!area)
  {
    throw std::exception("InnerLoop is not a valid area");
  }

  if (oriented)
  {
    hole.inverse();
  }
}


void OrientationGuard::rollback(Polyloop& polyloop)
{
  if (m_inverted)
  {
    std::vector<Point>& points = polyloop.getPoints();
    std::reverse(points.begin(), points.end());
  }
}
