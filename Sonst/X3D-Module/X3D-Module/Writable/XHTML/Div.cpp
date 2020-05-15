#include "Div.h"

namespace X3D
{
	Div::Div()
	{

	}

	Div::Div(const std::string& ident, Dd* dd)
	{
		setID(ident);

		addChild(dd);
	}

	Div::~Div()
	{

	}

// 	IDable* Div::getElementById(const std::string ident)
// 	{
// 		std::string ownId = getID();
// 
// 		if (ident == ownId)
// 		{
// 			return this;
// 		}
// 
// 		for (auto iter = m_dd.begin(); iter != m_dd.end(); ++iter)
// 		{
// 			if (iter->getElementById(ident))
// 			{
// 				return iter->getElementById(ident);
// 			}
// 		}
// 
// 		return nullptr;
// 	}

	void Div::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("div");

		IDable::write(printer);
		EVENTable::write(printer);

		XHTMLNode::write(printer);

		printer.CloseElement();
	}
}
