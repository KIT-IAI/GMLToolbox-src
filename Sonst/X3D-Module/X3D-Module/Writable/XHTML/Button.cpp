#include "Button.h"

namespace X3D
{

	Button::Button()
	{
		setType(button);
	}

	Button::Button(const std::string& ident, const std::string& val, const std::string& textNode)
	{
		setID(ident);
		setValue(val);
		m_textNode = textNode;
		setType(button);
	}

	Button::~Button()
	{

	}

// 	IDable* Button::getElementById(const std::string ident)
// 	{
// 		std::string ownId = getID();
// 
// 		if (ident == ownId)
// 		{
// 			return this;
// 		}
// 
// 		return nullptr;
// 	}

	void Button::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("button");

		IDable::write(printer);
		EVENTable::write(printer);
		InputNode::write(printer);

		XHTMLNode::write(printer);

		printer.PushText(m_textNode.c_str());

		printer.CloseElement();
	}
}