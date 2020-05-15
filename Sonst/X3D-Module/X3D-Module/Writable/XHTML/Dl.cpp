#include "Dl.h"

namespace X3D
{
	Dl::Dl()
	{

	}

	Dl::Dl(Dt* dt, Dd* dd)
	{
		addChild(dt);
		addChild(dd);
	}

	Dl::~Dl()
	{
		
	}

// 	IDable* Dl::getElementById(const std::string ident)
// 	{
// 		std::string ownId = getID();
// 
// 		if (ident == ownId)
// 		{
// 			return this;
// 		}
// 
// 		for (auto iter = m_dt.begin(); iter != m_dt.end(); ++iter)
// 		{
// 			if (iter->getElementById(ident))
// 			{
// 				return iter->getElementById(ident);
// 			}
// 		}
// 
// 		for (auto iter = m_div.begin(); iter != m_div.end(); ++iter)
// 		{
// 			if (iter->getElementById(ident))
// 			{
// 				return iter->getElementById(ident);
// 			}
// 		}
// 
// 		return nullptr;
// 	}

	void Dl::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("dl");

		IDable::write(printer);
		EVENTable::write(printer);

		XHTMLNode::write(printer);

		printer.CloseElement();
	}
}
