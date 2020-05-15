#pragma once

#include "ColFwd.hpp"

namespace col
{
    enum Wrap { WRAP, MIRROR, CLAMP, BORDER, NONE };

    class Texture
    {
    public:
        Texture(const std::string& id) : m_id(id), m_wrap(WRAP) {}

        void setImage(const ImagePtr& image) { m_image = image; }
        const ImagePtr& getImage() const { return m_image; }

        const std::string& getId() const { return m_id; }

        Wrap getWrap() const { return m_wrap; }
        void setWrap(Wrap wrap) { m_wrap = wrap; }
        
    private:
        std::string m_id;

        ImagePtr m_image;

        Wrap m_wrap;
    };
}