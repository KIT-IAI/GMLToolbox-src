#include "Primitive.hpp"

#include "Mesh.hpp"

namespace col
{
    void col::Mesh::addPrimitive(const col::PrimitivePtr& primitive)
    {
        m_primtives.push_back(primitive);
    }

    void col::Mesh::addVertex(const col::Vertex& vertex)
    {
        m_vertices.push_back(vertex);
    }

    bool Mesh::isTextured() const
    {
        // TODO: Workaround

        for (Vertices::const_iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
        {
            if (!it->getTextureCoordinates())
            {
                return false;
            }
        }

        return true;
    }

    bool Mesh::hasMaterials() const
    {
        // Not thread save, materials may expire during a loop
        for (Primitives::const_iterator it = m_primtives.begin(); it != m_primtives.end(); ++it)
        {
            const PrimitivePtr& primitive = *it;
            
            if (!primitive->getMaterial().expired())
            {
                return true;
            }
        }

        return false;
    }

}