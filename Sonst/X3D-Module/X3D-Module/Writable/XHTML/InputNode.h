#pragma once

#include "..\EVENTable.h"
#include "XHTMLNode.h"
#include "..\..\EnumDefinition.h"

#include "..\..\tinyxml2.h"
#include <string>


namespace X3D
{
	class InputNode : public XHTMLNode
	{
	public:
		virtual ~InputNode();

		virtual void	setType(e_inputType typ)	{ m_type = typ; }
		virtual void	setValue(const std::string& val)	{ m_value = val; }
		
		virtual e_inputType	getType()	{ return m_type; }
		virtual std::string	getValue()	{ return m_value; }
		
		virtual void	write(tinyxml2::XMLPrinter& printer);

	private:
		//attribute:
		e_inputType		m_type;
		std::string		m_value;
	};
}
