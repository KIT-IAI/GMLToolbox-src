#pragma once

#include <boost/optional/optional.hpp>

#include "Vector.hpp"
#include "Vector2.hpp"
#include "Color.hpp"

namespace col
{
    /// Single vertex.
	class Vertex
	{
	public:
		const Vector3& getPosition() const { return m_position; }
		void setPosition(const col::Vector3& position) { m_position = position; }

        const Vector3& getNormal() const { return m_normal; }
        void setNormal(const Vector3& normal) { m_normal = normal; }

        const boost::optional<Vector2>& getTextureCoordinates() const { return m_textureCoordinates; }
        void setTextureCoordinates(const Vector2& textureCoordinates) { m_textureCoordinates = textureCoordinates; }

	private:
		Vector3 m_position;
        Vector3 m_normal;
        boost::optional<Vector2> m_textureCoordinates;        
    };
}