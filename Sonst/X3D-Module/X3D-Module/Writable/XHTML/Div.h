#pragma once

#include "..\EVENTable.h"
#include "..\IDable.h"
#include "XHTMLNode.h"
#include "Dd.h"

#include "..\..\tinyxml2.h"
#include <vector>

namespace X3D
{
	class Dd;
	class Div : public XHTMLNode
	{
	public:
		Div();
		Div(const std::string& ident, Dd* dd);
		virtual ~Div();

		//virtual IDable* getElementById(const std::string ident);

		virtual void write(tinyxml2::XMLPrinter& printer);
	};
}
