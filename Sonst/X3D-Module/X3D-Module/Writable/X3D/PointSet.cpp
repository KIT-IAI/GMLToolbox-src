#include "PointSet.h"

namespace X3D
{
	PointSet::PointSet()
	{

	}

	PointSet::PointSet(const std::string& ident, Coordinate* coordinate)
	{
		setID(ident);
		setCoordinate(coordinate);
	}

	PointSet::~PointSet()
	{

	}
	void PointSet::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("PointSet");
		
		IDable::write(printer);
		DEFable::write(printer);

		ComposedGeometryNode::getCoordinate()->write(printer);

		printer.CloseElement();
	}
}