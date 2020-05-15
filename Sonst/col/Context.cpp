#include <boost/bind.hpp>

#include "Triangles.hpp"
#include "Polygons.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Image.hpp"
#include "Node.hpp"

#include "Context.hpp"

namespace col
{
    Context::Context()
        : m_materialCounter(0)
        , m_textureCounter(0)
        , m_imageCounter(0)
        , m_meshCounter(0)
        , m_nodeCounter(0)
    {

    }

    MaterialPtr Context::createMaterial()
    {
        std::string materialId = "material-" + boost::lexical_cast<std::string>(m_materialCounter);
        ++m_materialCounter;

        MaterialPtr material(new Material(materialId));
        return material;
    }

    TexturePtr Context::createTexture()
    {
        std::string id = "texture-" + boost::lexical_cast<std::string>(m_textureCounter);
        ++m_textureCounter;

        TexturePtr texture(new Texture(id));        
        return texture;
    }

    ImagePtr Context::createImage()
    {
        std::string id = "image-"
            + boost::lexical_cast<std::string>(m_imageCounter);
        ++m_imageCounter;

        ImagePtr image(new Image(id), boost::bind(&Image::deleteMe, _1));
        return image;
    }

    NodePtr Context::createNode()
    {
        std::string id = "node-" + boost::lexical_cast<std::string>(m_nodeCounter);
        ++m_nodeCounter;

        NodePtr node(new Node(id));
        return node;
    }

    MeshPtr Context::createMesh()
    {
        std::string id = "mesh-" + boost::lexical_cast<std::string>(m_meshCounter);
        ++m_meshCounter;

        MeshPtr mesh(new Mesh(id));
        return mesh;
    }

    TrianglesPtr Context::createTriangles()
    {
        TrianglesPtr triangles(new Triangles());
        return triangles;
    }

    PolygonsPtr Context::createPolygons()
    {
        PolygonsPtr polygons(new Polygons());
        return polygons;
    }
}