#pragma once

#include "InputNode.h"
#include "..\IDable.h"
#include "XHTMLNode.h"

#include "..\..\tinyxml2.h"
#include <string>


namespace X3D
{
	class Input : public InputNode
	{
	public:
		Input();
		Input(const std::string& ident, const std::string& val);
		virtual ~Input();

		void	setChecked(const std::string& checked)	{ m_checked = checked; }

		std::string		getChecked()	const	{ return m_checked; }

		//virtual IDable* getElementById(const std::string ident);

		virtual void write(tinyxml2::XMLPrinter& printer);

	private:
		//attribute:
		std::string		m_checked;
	};
}
