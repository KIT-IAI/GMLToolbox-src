#include "Background.h"

using namespace tinyxml2;

namespace X3D
{
	Background::Background()
	{
		m_skyColor		= Color();
		m_transparency	= 0.;
	}

	Background::Background(const std::string& ident)
	{
		setID(ident);
		m_skyColor		= Color();
		m_transparency	= 0.;
	}

	Background::Background(const Color skyColor, double trans)
	{
		m_skyColor		= skyColor;
		m_transparency	= trans;
	}

	Background::Background(const std::string& ident, const Color skyColor, double trans)
	{
		setID(ident);
		m_skyColor		= skyColor;
		m_transparency	= trans;
	}

	Background::~Background()
	{

	}
	void Background::write(XMLPrinter& printer)
	{
		printer.OpenElement("Background");
		
		IDable::write(printer);
		DEFable::write(printer);
		EVENTable::write(printer);

 		if (m_skyColor.getR() != 0 || m_skyColor.getG() != 0 || m_skyColor.getB() != 0)
 		{
			m_skyColor.write(printer, "skyColor");
		}
		
// 		if(m_groundColor)
// 			writeGroundColor(printer);
		if(m_transparency)
			printer.PushAttribute("transparency", m_transparency);
		
		printer.CloseElement();
	}

	/*
	IDable* Background::getElementById(const std::string ident)
	{
		std::string ownId = getID();

		if (ident == ownId)
		{
			return this;
		}
	}
	*/
}