#pragma once

#include "ColFwd.hpp"
#include "Mesh.hpp"

namespace col
{
    /// Describes a scene, holds a scene graph
	class Scene
	{
    public:
        void addNode(const NodePtr& node) { m_nodes.push_back(node); }
        //void addLibraryNode(const NodePtr& node) { m_libraryNodes.push_back(node); }

        const Nodes& getChildren() const { return m_nodes; }
        //const Nodes& getLibraryNodes() const { return m_libraryNodes; }

        bool isEmpty() const { return m_nodes.empty(); }

	private:
		Nodes m_nodes;
        //Nodes m_libraryNodes;
	};
}