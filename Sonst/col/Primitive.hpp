#pragma once

#include "ColFwd.hpp"

namespace col
{
    enum PrimitiveType { TRIANGLES, POLYGONS };

    /// All graphical primitives
    class Primitive
    {
    public:
        Primitive(PrimitiveType type) : m_type(type) {}
        virtual ~Primitive() {}

        PrimitiveType getType() const { return m_type; }

        void addIndex(size_t index) { m_indices.push_back(index); }

        const Indices& getIndices() const { return m_indices; }

        void setMaterial(const MaterialPtr& material) { m_material = material; }
        const WeakMaterialPtr& getMaterial() const { return m_material; }

    protected:
        PrimitiveType m_type;
        WeakMaterialPtr m_material;

        Indices m_indices;
    };
}