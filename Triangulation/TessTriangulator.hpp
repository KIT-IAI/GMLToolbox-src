#pragma once

#include "x3dGL.h"

#include <unordered_map>


#include "../Geometrie.h"
#include "../Logfile.h"

#include "Triangulator.hpp"
#include "ModeConverter.hpp"

// Declare GLUtesselator as an empty class to get rid of the linker warning (LNK4248)
class GLUtesselator {};


class PolygonGeo;
class Triangle;
class GeoPoint;

typedef ModeConverter<const GeoPoint*> MyModeConverter;
typedef std::vector<GeoPoint*> PointVector;

class TessTriangulator : public Triangulator,
  public ModeConverterListener<const GeoPoint*>
{
public:
  TessTriangulator();
  virtual ~TessTriangulator();

  bool triangulate( PolygonGeo * pPoly, bool ori );
  bool triangulate( PolygonGeo * pPoly, std::vector<int>& vCoordIndex, std::vector<GeoPoint*>& vPoints, bool ori );
  bool  triangulatePolygons( std::vector<PolygonGeo*> &vPoly, std::vector<int>& vCoordIndex, std::vector<GeoPoint*>& vPoints, std::vector<bool> &oriVec);

  void addExtraPoint (GeoPoint* pPoint) { m_extraPoints.push_back(pPoint); }
  PointVector getExtraPoints() { return m_extraPoints; }

private:
	void addPoint(GeoPoint * point);
  virtual void onTriangle(const GeoPoint* i1, const GeoPoint* i2, const GeoPoint* i3 );
  virtual void onError(GLenum err);
  void applyPoints(const Points& points);
  bool processErrors();
//  int createIndex(const GeoPoint* i);
  MyModeConverter m_modeConverter;
  GLUtesselator* m_tess;

  LogfileLokal * pLogfile;

  std::vector<GLenum> m_errnos;
  PointVector m_extraPoints;
  std::unordered_map<const GeoPoint*, int> m_indices;
};

