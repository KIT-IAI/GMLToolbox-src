#include "Material.h"

namespace X3D
{
	Material::Material()
	{
		m_diffuseColor	= new Color(0.8);
		m_emissiveColor	= new Color();
		m_transparency	= 0.;
	}

	Material::Material(const std::string& ident, Color* dif, Color* em, const double trans)
	{
		setID(ident);

		m_diffuseColor	= dif;
		m_emissiveColor	= em;
		m_transparency	= trans;
	}

	Material::Material(const std::string& ident, Color* dif, Color* em)
	{
		setID(ident);

		m_diffuseColor	= dif;
		m_emissiveColor	= em;
		m_transparency	= 0.;
	}

	Material::Material(const std::string& ident, Color* dif, const double trans)
	{
		setID(ident);

		m_diffuseColor	= dif;
		m_emissiveColor	= new Color();
		m_transparency	= trans;
	}

	Material::Material(const std::string& ident, Color* dif)
	{
		setID(ident);

		m_diffuseColor	= dif;
		m_emissiveColor	= new Color();
		m_transparency	= 0.;
	}

	Material::~Material()
	{

	}

	/*
	IDable* Material::getElementById(const std::string ident)
	{
		std::string ownId = getID();

		if (ident == ownId)
		{
			return this;
		}
	}
	*/

	void Material::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("Material");
		IDable::write(printer);
		DEFable::write(printer);

		if(m_diffuseColor->getR() != 0.8 && m_diffuseColor->getG() != 0.8 && m_diffuseColor->getB() != 0.8)
		{
			m_diffuseColor->write(printer, "diffuseColor");
		}
		if(m_emissiveColor->getR() != 0. && m_diffuseColor->getG() != 0. && m_diffuseColor->getB() != 0.)
		{
			m_emissiveColor->write(printer, "emissiveColor");
		}
		if(m_transparency != 0.)
		{
			printer.PushAttribute("transparency", m_transparency);
		}

		//m_coordinate->write(printer);

		printer.CloseElement();
	}
}