#include "Dd.h"

namespace X3D
{

	Dd::Dd()
	{

	}

	Dd::Dd(const std::string& ident, const std::string cl, Input* inpu, A* a)
	{
		setID(ident);
		setClass(cl);

		addChild(inpu);
		addChild(a);
	}

	Dd::Dd(const std::string& ident, const std::string cl, Input* inpu)
	{
		setID(ident);
		setClass(cl);

		addChild(inpu);
	}

	Dd::Dd(Dl* dl)
	{
		addChild(dl);
	}

	Dd::~Dd()
	{

	}

// 	IDable* Dd::getElementById(const std::string ident)
// 	{
// 		std::string ownId = getID();
// 
// 		if (ident == ownId)
// 		{
// 			return this;
// 		}
// 
// 		if (m_input.getElementById(ident))
// 		{
// 			return m_input.getElementById(ident);
// 		}
// 		
// 		return m_sceneGraph.getElementById(ident);
// 	}

	void Dd::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("dd");

		IDable::write(printer);
		EVENTable::write(printer);

		XHTMLNode::write(printer);
		
		printer.PushText(m_textNode.c_str());

		printer.CloseElement();
	}
}