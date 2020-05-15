#pragma once

#include "InputNode.h"
#include "..\IDable.h"
#include "XHTMLNode.h"
#include "..\..\EnumDefinition.h"

#include "..\..\tinyxml2.h"
#include <string>

namespace X3D
{
	class Button : public InputNode
	{
	public:
		Button();
		Button(const std::string& ident, const std::string& val, const std::string& textNode);
		virtual ~Button();

		void	setTextNode(const std::string& textNode)	{ m_textNode = textNode; }

		std::string	getTextNode()	const { return m_textNode; }

		//virtual IDable* getElementById(const std::string ident);

		virtual void write(tinyxml2::XMLPrinter& printer);

	private:
		//elemente:
		std::string		m_textNode;
	};
}