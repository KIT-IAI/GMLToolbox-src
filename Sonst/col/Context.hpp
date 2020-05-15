#pragma once

#include "ColFwd.hpp"
#include "Scene.hpp"
#include "MeshLibrary.hpp"
#include "MaterialLibrary.hpp"
#include "ImageLibrary.hpp"

namespace col
{
    class Context
    {
    public:
        Context();

        /// Creates a new material.
        /// The material is deleted if it is not added to a MaterialLibrary.
        /// See MaterialLibrary::addMaterial
        MaterialPtr createMaterial();
        
        TexturePtr Context::createTexture();
        ImagePtr Context::createImage();

        NodePtr createNode();

        /// Creates a new mesh.
        /// The mesh will be lost if it is not added to a library.
        /// see MeshLibrary::addMesh
        MeshPtr createMesh();

        TrianglesPtr createTriangles();
        PolygonsPtr createPolygons();

        const Scene& getScene() const { return m_scene; }
        const MeshLibrary& getMeshLibrary() const { return m_meshLibrary; }
        const ImageLibrary& getImageLibrary() const { return m_imageLibrary; }
        const MaterialLibrary& getMaterialLibrary() const { return m_materialLibrary; }
        const NodeLibrary& getNodeLibrary() const { return m_nodeLibrary; }

        Scene& getScene() { return m_scene; }
        MeshLibrary& getMeshLibrary() { return m_meshLibrary; }
        ImageLibrary& getImageLibrary() { return m_imageLibrary; }
        MaterialLibrary& getMaterialLibrary() { return m_materialLibrary; }
        NodeLibrary& getNodeLibrary() { return m_nodeLibrary; }

    private:
        size_t m_materialCounter;
        size_t m_textureCounter;
        size_t m_imageCounter;
        size_t m_nodeCounter;
        size_t m_meshCounter;

        col::Scene m_scene;
        col::MeshLibrary m_meshLibrary;
        col::ImageLibrary m_imageLibrary;
        col::MaterialLibrary m_materialLibrary;    
        col::NodeLibrary m_nodeLibrary;
    };
}