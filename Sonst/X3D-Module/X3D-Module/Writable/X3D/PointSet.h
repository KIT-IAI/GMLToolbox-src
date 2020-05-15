#pragma once

#include "ComposedGeometryNode.h"
#include "..\..\tinyxml2.h"

namespace X3D
{
	class PointSet : public ComposedGeometryNode
	{
	public:
		PointSet();
		PointSet(const std::string& ident, Coordinate* coordinate);
		virtual ~PointSet();

		void write(XMLPrinter& printer);

	private:
		//attributes

		
		//child elements
	};
}