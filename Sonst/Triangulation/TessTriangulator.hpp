#pragma once

#include "x3dGL.h"

#include <unordered_map>


//#include "../Geometrie.h"

#include "Triangulator.hpp"
#include "ModeConverter.hpp"

class PolygonGeo;
class Triangle;
//class GeoPoint;

typedef ModeConverter<const X3D::Point3D*> MyModeConverter;
typedef std::vector<X3D::Point3D*> PointVector;

class TessTriangulator : public Triangulator,
  public ModeConverterListener<const X3D::Point3D*>
{
public:
  TessTriangulator();
  virtual ~TessTriangulator();

  virtual void triangulate( PolygonGeo * pPoly, bool ori );
  void triangulatePolygons( std::vector<PolygonGeo*> &vPoly, std::vector<int>& vCoordIndex, std::vector<X3D::Point3D*>& vPoints, std::vector<bool> &oriVec);

  void addExtraPoint(X3D::Point3D* pPoint) { m_extraPoints.push_back(pPoint); }
  PointVector getExtraPoints() { return m_extraPoints; }

private:
	void addPoint(X3D::Point3D* point);
  virtual void onTriangle(const X3D::Point3D* i1, const X3D::Point3D* i2, const X3D::Point3D* i3);
  virtual void onError(GLenum err);
  void applyPoints(const Points& points);
  void processErrors();
  int createIndex(const X3D::Point3D* i);
  MyModeConverter m_modeConverter;
  GLUtesselator* m_tess;

  std::vector<GLenum> m_errnos;
  PointVector m_extraPoints;
  std::unordered_map<const X3D::Point3D*, int> m_indices;
};

