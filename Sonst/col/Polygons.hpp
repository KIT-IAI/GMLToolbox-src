#pragma once

#include "ColFwd.hpp"
#include "Primitive.hpp"

namespace col
{
    /// Primitive describing a polygon with holes.
    class Polygons : public Primitive
    {
    public:
        Polygons() : Primitive(POLYGONS) {}

        void addHole(const Indices& hole) { m_holes.push_back(hole); }

        bool hasHoles() const { return !m_holes.empty(); }

        const Holes& getHoles() const { return m_holes; }

    private:
        Holes m_holes;
    };
}