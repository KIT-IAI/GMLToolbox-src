#pragma once

#include "ColFwd.hpp"
#include "Vertex.hpp"

namespace col
{
    /// A mesh consists of several primitives.
    class Mesh
	{
	public:
        Mesh(const std::string& id) : m_id(id) {}

		void addPrimitive(const PrimitivePtr& primitive);
		void addVertex(const Vertex& vertex);

		const Vertices& getVertices() const { return m_vertices; }
		const Primitives& getPrimitives() const { return m_primtives; }

        const Color& getColor() const { return m_color; }
        void setColor(const Color& color) { m_color = color; }

        const std::string& getId() const { return m_id; }

        bool isTextured() const;
        bool hasMaterials() const;

	private:
        const std::string m_id;

		Vertices m_vertices;
		Primitives m_primtives;

        Color m_color;
	};
}