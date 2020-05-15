#include "IndexedLineSet.h"

namespace X3D
{
	IndexedLineSet::IndexedLineSet()
	{

	}

	IndexedLineSet::IndexedLineSet(const std::string& ident, const std::vector<int>& coordIndex, Coordinate* coordinate)
	{
		setID(ident);
		setCoordIndex(coordIndex);
		setCoordinate(coordinate);
	}

	IndexedLineSet::~IndexedLineSet()
	{

	}
	void IndexedLineSet::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("IndexedLineSet");
		
		IDable::write(printer);
		DEFable::write(printer);

		Indexable::write(printer);

		ComposedGeometryNode::getCoordinate()->write(printer);

		printer.CloseElement();
	}
}