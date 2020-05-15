#pragma once

#include "..\EVENTable.h"
#include "..\IDable.h"
#include "XHTMLNode.h"
#include "Input.h"
#include "Dl.h"
#include "A.h"

#include "..\..\tinyxml2.h"
#include <string>

namespace X3D
{
	class A;
	class Dl;

	class Dd : public XHTMLNode
	{
	public:
		Dd();
		Dd(Dl* dl);
		Dd(const std::string& ident, const std::string cl, Input* inpu, A* a);
		Dd(const std::string& ident, const std::string cl, Input* inpu);
		virtual ~Dd();
		
		void	setTextNode(const std::string& textNode)	{ m_textNode = textNode; }

		std::string	getTextNode()	const	{ return m_textNode; }

		virtual void	write(tinyxml2::XMLPrinter& printer);

	private:
		//elemente:
		std::string		m_textNode;
	};
}
