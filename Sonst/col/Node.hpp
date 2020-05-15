#pragma once

#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>

#include "ColFwd.hpp"
#include "Matrix.hpp"

namespace col
{
    enum NodeLock { REFERENCE, INSTANCE };
    typedef boost::tuple<NodePtr, NodeLock> Child;
    typedef std::vector<Child> Children;

    /// Single node in a scene graph with children and parent node.
	class Node
	{
	public:
        Node(const std::string& id);
       
        void setParent(const WeakNodePtr& parent) { m_parent = parent; }
        void addChild(const NodePtr& child, NodeLock nodeLock = INSTANCE) { m_children.push_back(Child(child, nodeLock)); }
        
        const col::WeakMeshPtr& getMesh() const { return m_mesh; }
        void setMesh(const col::WeakMeshPtr& mesh) { m_mesh = mesh; }

        const col::WeakMaterialPtr& getMaterial() const { return m_material; }
        void setMaterial(const col::WeakMaterialPtr& material) { m_material = material; }

        const Children& getChildren() const { return m_children; }

        const std::string& getName() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        const std::string& getLayer() const { return m_layer; }
        void setLayer(const std::string& layer) { m_layer = layer; }

        boost::optional<col::Matrix4> getTransformation() const { return m_transformation; }
        void setTransformation(const col::Matrix4& transformation) { m_transformation = transformation; m_transformed = true; }

        bool isTransformed() const { return m_transformed; }

        const std::string& getTitle() const { return m_title; }
        void setTitle(const std::string& title) { m_title = title; }

        bool hasMesh() const { return m_mesh.lock().get() != 0; }
        bool hasChildren() const { return !m_children.empty(); }

        const std::string& getId() const { return m_id; }

	private:
        std::string m_name;
        std::string m_title;
        std::string m_layer;
        std::string m_id;
     
		WeakNodePtr m_parent;
		Children m_children;

        WeakMeshPtr m_mesh;
        WeakMaterialPtr m_material;
    
        boost::optional<Matrix4> m_transformation;
        bool m_transformed;
    };
}