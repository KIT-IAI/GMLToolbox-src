#include "Dt.h"

namespace X3D
{
	Dt::Dt()
	{

	}

	Dt::Dt(const std::string& ident, const std::string& cl, Button* but, Input* inpu, A* a)
	{
		setID(ident);
		setClass(cl);

		addChild(but);
		addChild(inpu);
		addChild(a);
	}
	
	Dt::~Dt()
	{

	}

	void Dt::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("dt");

		IDable::write(printer);
		EVENTable::write(printer);

		XHTMLNode::write(printer);

		printer.CloseElement();
	}
}
