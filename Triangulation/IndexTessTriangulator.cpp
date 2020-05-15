#include "StdAfx.h"

#include "IndexTessTriangulator.hpp"

typedef void(CALLBACK *TessDefaultCallback)();

//           DONUT
//        _.-------._
//      .'    ___    '.
//     /     (___)     \
//     |'._         _.'|
//     |   `'-----'`   |
//      \             /
//       '-.______..-'

void CALLBACK onIndexTessBegin(GLenum type, void* ptr)
{
  // Begin polygon
  IndexTessModeConverter* modeConverter = static_cast<IndexTessModeConverter*>(ptr);
  modeConverter->onBegin(type);
}

void CALLBACK onIndexTessEnd(void* ptr)
{
  // End polygon
  IndexTessModeConverter* modeConverter = static_cast<IndexTessModeConverter*>(ptr);
  // modeConverter->onEnd();
}

void CALLBACK onIndexTessVertex(void* vertexData, void* ptr)
{
  IndexTessModeConverter* modeConverter = static_cast<IndexTessModeConverter*>(ptr);
  // Vertex data (Provided by glTessVertex)
  const size_t* index = reinterpret_cast<const size_t*>(vertexData);
  modeConverter->onIndex(*index);
}

void CALLBACK onIndexTessError(GLenum err, void* ptr)
{
  IndexTessModeConverter* modeConverter = static_cast<IndexTessModeConverter*>(ptr);
  modeConverter->onError(err);

  // GLU_TESS_MISSING_BEGIN_POLYGON 
  // GLU_TESS_MISSING_END_POLYGON
  // GLU_TESS_MISSING_BEGIN_CONTOUR
  // GLU_TESS_MISSING_END_CONTOUR
  // GLU_TESS_COORD_TOO_LARGE
  // GLU_TESS_NEED_COMBINE_CALLBACK
}

IndexTessTriangulator::IndexTessTriangulator(IfcDB::Face& face)
: m_face(face)
{
  m_converter.setListener(this);

  m_tess = gluNewTess();

  gluTessCallback(m_tess, GLU_TESS_BEGIN_DATA, TessDefaultCallback(&onIndexTessBegin));
  gluTessCallback(m_tess, GLU_TESS_END_DATA, TessDefaultCallback(&onIndexTessEnd));
  gluTessCallback(m_tess, GLU_TESS_VERTEX_DATA, TessDefaultCallback(&onIndexTessVertex));
  gluTessCallback(m_tess, GLU_TESS_ERROR_DATA, TessDefaultCallback(&::onIndexTessError));
}

IndexTessTriangulator::~IndexTessTriangulator()
{
  gluDeleteTess(m_tess);
}

void IndexTessTriangulator::onTriangle(size_t i1, size_t i2, size_t i3)
{
  m_indices.push_back(i1);
  m_indices.push_back(i2);
  m_indices.push_back(i3);
}

void IndexTessTriangulator::onError(GLenum err)
{
}

void IndexTessTriangulator::triangulate()
{
  IfcDB::Mesh mesh;
  IfcDB::Indices indices;
  IfcDB::Points allPoints;

  size_t size(m_face.getOuterLoop()->getPointsSize());
  const IfcDB::PolyloopPtrs& holes = m_face.getInnerLoops();
  for (IfcDB::PolyloopPtrs::const_iterator it = holes.begin(); it != holes.end(); ++it)
  {
    size += (*it)->getPointsSize();
  }
  m_sourceIndices.resize(size);
  for (size_t i(0); i < size; ++i)
  {
    m_sourceIndices[i] = i;
  }
  size_t index(0);

  gluTessBeginPolygon(m_tess, &m_converter);
  
  gluTessBeginContour(m_tess);

  const IfcDB::Points& points = m_face.getOuterLoop()->getPoints();

  std::copy(points.begin(), points.end(), std::back_inserter(allPoints));
  
  for (IfcDB::Points::const_iterator it = points.begin(); it != points.end(); ++it)
  {
    GLdouble coord[3] = { it->x, it->y, it->z };
   
    gluTessVertex(m_tess, coord, &m_sourceIndices[index]);

    ++index;
  }

  gluTessEndContour(m_tess);

  for (IfcDB::PolyloopPtrs::const_iterator it = holes.begin(); it != holes.end(); ++it)
  {
    const IfcDB::Points& points = (*it)->getPoints();

    std::copy(points.begin(), points.end(), std::back_inserter(allPoints));

    gluTessBeginContour(m_tess);

    for (IfcDB::Points::const_iterator it = points.begin(); it != points.end(); ++it)
    {
      GLdouble coord[3] = { it->x, it->y, it->z };

      gluTessVertex(m_tess, coord, &m_sourceIndices[index]);

      ++index;
    }

    gluTessEndContour(m_tess);
  }

  gluTessEndPolygon(m_tess);

  mesh.setIndices(m_indices);
  mesh.setVertices(allPoints);
  m_face.setMesh(mesh);
}
