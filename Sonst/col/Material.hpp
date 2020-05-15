#pragma once

#include <boost/optional/optional.hpp>

#include "Color.hpp"
#include "Texture.hpp"
#include "ColFwd.hpp"

namespace col
{
    /// Represents a material with color and/or texture.
    class Material
    {
    public:
        Material(const std::string& id) : m_id(id) {}

        const boost::optional<Color>& getColor() const { return m_color; }
        void setColor(const Color& color) { m_color = color; }

        const std::string& getId() const { return m_id; }
        void setId(const std::string& id) { m_id = id; }

        const boost::optional<TexturePtr>& getTexture() const { return m_texture; }
        void setTexture(const TexturePtr& texture) { m_texture = texture; }

    private:
        std::string m_id;

        boost::optional<Color> m_color;
        boost::optional<TexturePtr> m_texture;
    };
}