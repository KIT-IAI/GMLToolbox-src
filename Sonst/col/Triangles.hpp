#pragma once

#include "ColFwd.hpp"

#include "Primitive.hpp"

namespace col
{
    /// Graphical primitive (face) for a triangle.
    class Triangles : public Primitive
    {
    public:
        Triangles() : Primitive(TRIANGLES) {}
    };
}