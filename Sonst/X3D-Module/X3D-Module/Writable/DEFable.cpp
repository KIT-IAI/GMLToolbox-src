#include "DEFable.h"

namespace X3D
{
	DEFable::~DEFable()
	{

	}

	void DEFable::write(tinyxml2::XMLPrinter& printer)
	{
		if(!getID().empty() && m_use.empty())
		{
			printer.PushAttribute("DEF", getID().c_str());
		}
		if(!m_use.empty())
		{
			printer.PushAttribute("USE", m_use.c_str());
		}
	}

	/*void DEFable::setIdentifier(const std::string& ident, const e_defId& defId)
	{
		if (defId == id)
		{
			setID(ident);
		}
		else if (defId == def)
		{
			setDEF(ident);
		}
		else if (defId == both)
		{
			setID(ident);
			setDEF(ident);
		}
	}*/

}