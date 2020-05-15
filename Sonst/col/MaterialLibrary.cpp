#include "Material.hpp"

#include "MaterialLibrary.hpp"

namespace col
{
    void MaterialLibrary::addMaterial(const MaterialPtr& material)
    {
        m_materials.push_back(material);
    }

    MaterialPtr MaterialLibrary::find(const Color& color) const
    {
        for (Materials::const_iterator it = m_materials.begin(); it != m_materials.end(); ++it)
        {
            if (*(*it)->getColor() == color)
            {
                return *it;
            }
        }

        MaterialPtr temp;
        return temp;
    }

}