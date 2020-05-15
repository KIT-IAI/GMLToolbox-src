#include "X3DAppearance.h"

using namespace tinyxml2;

namespace X3D
{
	Appearance::Appearance()
	{
		setID("");
		setUSE("");
	}

	Appearance::Appearance(const std::string& ident, const std::string& use)
	{
		setID(ident);
		setUSE(use);
	}

	Appearance::Appearance(const std::string& ident, Material* mat)
	{
		setID(ident);

		m_material = mat;
	}

	Appearance::~Appearance()
	{

	}

	/*
	IDable* Appearance::getElementById(const std::string ident)
	{
		std::string ownId = getID();

		if (ident == ownId)
		{
			return this;
		}
		return m_material.getElementById(ident);
	}
	*/

	void Appearance::write(XMLPrinter& printer)
	{
		printer.OpenElement("Appearance");
		
		IDable::write(printer);
		DEFable::write(printer);

		if(getUSE().empty())
		{
			m_material->write(printer);
		}

		printer.CloseElement();
	}
}