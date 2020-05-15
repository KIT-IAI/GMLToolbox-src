#pragma once

#include "..\EVENTable.h"
#include "XHTMLNode.h"
#include "Button.h"
#include "Input.h"
#include "A.h"

#include "..\..\tinyxml2.h"
#include <string>

namespace X3D
{
	class Dt : public XHTMLNode
	{
	public:
		Dt();
		Dt(const std::string& ident, const std::string& cl, Button* but, Input* inpu, A* a);
		virtual ~Dt();

		virtual void write(tinyxml2::XMLPrinter& printer);

	private:
		//elemente:
	};
}
