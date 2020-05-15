#include "Input.h"

namespace X3D
{

	Input::Input()
	{
		setType(checkbox);
		m_checked	= "checked";
	}

	Input::Input(const std::string& ident, const std::string& val)
	{
		setID(ident);
		setValue(val);
		setType(checkbox);
		m_checked	= "checked";
	}

	Input::~Input()
	{

	}

// 	IDable* Input::getElementById(const std::string ident)
// 	{
// 		std::string ownId = getID();
// 
// 		if (ident == ownId)
// 		{
// 			return this;
// 		}
// 		return nullptr;
// 	}

	void Input::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("input");

		IDable::write(printer);
		EVENTable::write(printer);
		InputNode::write(printer);

		printer.PushAttribute("checked", m_checked.c_str());

		XHTMLNode::write(printer);

		printer.CloseElement();
	}
}
