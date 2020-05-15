#pragma once

#include "Mesh.hpp"

namespace col
{
    /// Stores all meshs
    class MeshLibrary
    {
    public:
        void addMesh(const MeshPtr& mesh) { m_meshs.push_back(mesh); }

        const Meshs& getMeshs() const { return m_meshs; }

    private:
        Meshs m_meshs;
    };
}