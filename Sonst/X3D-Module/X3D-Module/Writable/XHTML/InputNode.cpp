#include "InputNode.h"

namespace X3D
{

	InputNode::~InputNode()
	{

	}

	void InputNode::write(tinyxml2::XMLPrinter& printer)
	{
		printer.PushAttribute("type", inputTypeValues[m_type]);
		if (!m_value.empty())
		{
			printer.PushAttribute("value", m_value.c_str());
		}
	
	}
}