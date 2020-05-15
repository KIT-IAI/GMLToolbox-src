#pragma once

#include "..\EVENTable.h"
#include "..\IDable.h"
#include "XHTMLNode.h"
#include "Dd.h"
#include "Dt.h"

#include "..\..\tinyxml2.h"
#include <vector>
#include <list>

class Dd;
class Dt;

namespace X3D
{

	class Div;

	class Dl : public XHTMLNode
	{
	public:
		Dl();
		Dl(Dt* dt, Dd* dd);
		virtual ~Dl();

		//virtual IDable* getElementById(const std::string ident);

		virtual void write(tinyxml2::XMLPrinter& printer);
	};
}
