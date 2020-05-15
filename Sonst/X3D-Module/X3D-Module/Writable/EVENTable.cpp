#include "EVENTable.h"

namespace X3D
{

	EVENTable::~EVENTable()
	{

	}

	void EVENTable::write(tinyxml2::XMLPrinter& printer)
	{
		if(!m_onclick.empty())
		{
			printer.PushAttribute("onclick", m_onclick.c_str());
		}
		if(!m_onmouseover.empty())
		{
			printer.PushAttribute("onmouseover", m_onmouseover.c_str());
		}
		if(!m_onmouseout.empty())
		{
			printer.PushAttribute("onmouseout", m_onmouseout.c_str());
		}
		if(!m_onchange.empty())
		{
			printer.PushAttribute("onchange", m_onchange.c_str());
		}
    if(!m_onmousedown.empty())
    {
      printer.PushAttribute("onmousedown", m_onmousedown.c_str());
    }
	}
}