#pragma once

#include "IDable.h"
#include "..\tinyxml2.h"

namespace X3D
{
	class EVENTable : public IDable
	{
	public:
		virtual ~EVENTable();

		virtual void write(tinyxml2::XMLPrinter& printer);

		virtual void setOnclick(const std::string onclick)			{ m_onclick = onclick; }
		virtual void setOnmouseover(const std::string& onmouseover)	{ m_onmouseover = onmouseover; }
		virtual void setOnmouseout(const std::string& onmouseout)	{ m_onmouseout = onmouseout; }
		virtual void setOnchange(const std::string& onchange)		{ m_onchange = onchange; }
    virtual void setOnmousedown(const std::string& onmousedown)	{ m_onmousedown = onmousedown; }

		virtual std::string	getOnclick()		  const	{ return m_onclick; }
		virtual std::string	getOnmouseover()	const	{ return m_onmouseover; }
		virtual std::string	getOnmouseout()		const	{ return m_onmouseout; }
		virtual std::string	getOnchange()		  const	{ return m_onchange; }
    virtual std::string	getOnmousedown()	const	{ return m_onmousedown; }

	private:
		std::string		m_onclick;
		std::string		m_onmouseover;
		std::string		m_onmouseout;
    std::string   m_onmousedown;
		std::string		m_onchange;
	};
}