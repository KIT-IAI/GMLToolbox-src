#pragma once

#include <ostream>
#include <string>

#include "ColFwd.hpp"

namespace col
{
    /// Writes out a full scene graph and meshes/materials into a stream.
	class Writer
	{
	public:
        Writer(const Context& context);

		void write(std::ostream& out);

        void setSchemaLocationEnabled(bool schemaLocationEnabled) { m_schemaLocationEnabled = schemaLocationEnabled; }
	
	private:
        // Must be initialized first, leave m_context always at the top of all
        // other variables!
        const Context& m_context;

		const Scene& m_scene;
        const MeshLibrary& m_meshLib;
        const MaterialLibrary& m_materialLib;
        const ImageLibrary& m_imageLib;
        const NodeLibrary& m_nodeLib;

        bool m_schemaLocationEnabled;
        bool m_writeLibraryNodes;
	};

    void write(Writer& writer, const std::string& path);
    void write(Writer& writer, const std::wstring& path);
}