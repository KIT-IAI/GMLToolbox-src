#pragma once

#include <gl/GLU.h>

#include "ModeConverter.hpp"

typedef ModeConverter<size_t> IndexTessModeConverter;
typedef ModeConverterListener<size_t> IndexTessModeConverterListener;

class IndexTessTriangulator : public IndexTessModeConverterListener
{
public:
  IndexTessTriangulator(IfcDB::Face& face);
  ~IndexTessTriangulator();

  virtual void onTriangle(size_t i1, size_t i2, size_t i3);
  virtual void onError(GLenum err);

  void triangulate();

private:
  IfcDB::Face& m_face;

  IfcDB::Indices m_sourceIndices;
  IfcDB::Indices m_indices;

  GLUtesselator* m_tess;

  IndexTessModeConverter m_converter;
};