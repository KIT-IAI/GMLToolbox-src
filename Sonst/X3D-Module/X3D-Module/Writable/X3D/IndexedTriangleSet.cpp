#include "IndexedTriangleSet.h"

#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>

namespace X3D
{
	IndexedTriangleSet::IndexedTriangleSet()
	{

	}

	IndexedTriangleSet::IndexedTriangleSet(const std::string& ident, const std::vector<int>& coordIndex, Coordinate* coordinate)
	{
		setID(ident);
		setCoordIndex(coordIndex);
		setCoordinate(coordinate);
	}

	IndexedTriangleSet::~IndexedTriangleSet()
	{

	}

	void IndexedTriangleSet::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("IndexedTriangleSet");
		
		IDable::write(printer);
		DEFable::write(printer);
		EVENTable::write(printer);

		writeIndex(printer);

		ComposedGeometryNode::getCoordinate()->write(printer);

		printer.CloseElement();
	}

	void IndexedTriangleSet::writeIndex(tinyxml2::XMLPrinter& printer)
	{
		std::stringstream ss;

		for(auto iter=getCoordIndex().begin(); iter!=getCoordIndex().end(); ++iter)
		{
			ss << *iter << ' ';
		}

		std::string s = ss.str();

		printer.PushAttribute("index", s.c_str());
	}
}