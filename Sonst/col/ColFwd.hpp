#pragma once

#include <vector>
#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/lexical_cast.hpp>

namespace col
{
	class Mesh;
    typedef boost::shared_ptr<Mesh> MeshPtr;
    typedef boost::weak_ptr<Mesh> WeakMeshPtr;
	typedef std::vector<MeshPtr> Meshs;

    class Node;
    typedef boost::shared_ptr<Node> NodePtr;
    typedef boost::weak_ptr<Node> WeakNodePtr;
    typedef std::vector<NodePtr> Nodes;
    typedef std::vector<WeakNodePtr> WeakNodes;

    class Material;
    /// test
    typedef boost::shared_ptr<Material> MaterialPtr;
    typedef boost::weak_ptr<Material> WeakMaterialPtr;
    typedef std::vector<MaterialPtr> Materials;

    class Texture;
    typedef boost::shared_ptr<Texture> TexturePtr;

    class Image;
    typedef boost::shared_ptr<Image> ImagePtr;

    // Geometry primitives
	class Primitive;
    typedef boost::shared_ptr<Primitive> PrimitivePtr;
	typedef std::vector<PrimitivePtr> Primitives;

    class Polygons;
    typedef boost::shared_ptr<Polygons> PolygonsPtr;

    class Triangles;
    typedef boost::shared_ptr<Triangles> TrianglesPtr;
    
    class Vertex;
	typedef std::vector<Vertex> Vertices;

    typedef std::vector<size_t> Indices;
    
    /// Holes is a std::vector of Indices    
    typedef std::vector<Indices> Holes;

    class Scene;

    class MeshLibrary;
    class MaterialLibrary;
    
    template <typename T> class Library;
    typedef Library<ImagePtr> ImageLibrary;
    typedef Library<NodePtr> NodeLibrary;

    class Color;

    class Context;
}