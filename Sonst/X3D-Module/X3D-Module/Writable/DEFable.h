#pragma once

#include "EVENTable.h"
#include "..\EnumDefinition.h"

#include "..\tinyxml2.h"

namespace X3D
{
	class DEFable : public EVENTable
	{
	public:
		virtual ~DEFable();

		virtual void setUSE(const std::string& use)	{ m_use = use; }
		
		virtual std::string getUSE() const { return m_use; }

		//virtual void setIdentifier(const std::string& ident, const e_defId& defId);

		virtual void write(tinyxml2::XMLPrinter& printer);

	private:
		std::string m_use;
	};
}