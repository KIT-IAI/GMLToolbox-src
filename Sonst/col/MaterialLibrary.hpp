#pragma once

#include "ColFwd.hpp"

namespace col
{
    /// Stores all materials
    class MaterialLibrary
    {
    public:
        void addMaterial(const MaterialPtr& material);

        const Materials& getMaterials() const { return m_materials; }
        col::MaterialPtr find(const col::Color& color) const;

    private:
        Materials m_materials;
    };
}