#pragma once

#include "Indexable.h"
#include "..\..\EnumDefinition.h"

#include "..\..\tinyxml2.h"

namespace X3D
{
	class IndexedTriangleSet : public Indexable
	{
	public:
		IndexedTriangleSet();
		IndexedTriangleSet(const std::string& ident, const std::vector<int>& coordIndex, Coordinate* coordinate);
		virtual ~IndexedTriangleSet();
		
		void write(tinyxml2::XMLPrinter& printer);

	private:
		void writeIndex(XMLPrinter& printer);
	};
}