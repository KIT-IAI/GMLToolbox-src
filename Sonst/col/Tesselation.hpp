#pragma once

#include "Polygons.hpp"
#include "Triangles.hpp"
#include "Mesh.hpp"

namespace col
{
    TrianglesPtr tesselate(Context& context, const MeshPtr& mesh, const PolygonsPtr& polygons);
}