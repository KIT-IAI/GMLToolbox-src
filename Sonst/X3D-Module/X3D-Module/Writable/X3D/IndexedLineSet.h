#pragma once

#include "Indexable.h"
#include "..\..\tinyxml2.h"

namespace X3D
{
	class IndexedLineSet : public Indexable
	{
	public:
		IndexedLineSet();
		IndexedLineSet(const std::string& ident, const std::vector<int>& coordIndex, Coordinate* coordinate);
		virtual ~IndexedLineSet();

		virtual void write(tinyxml2::XMLPrinter& printer);
	private:
		//attributes
	};
}