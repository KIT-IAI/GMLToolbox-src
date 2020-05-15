#include "X3DColor.h"

#include <string>
#include <sstream>
#include <iostream>

using namespace tinyxml2;

namespace X3D
{
	Color::Color()
	{
		m_r = 0.;
		m_g = 0.;
		m_b = 0.;
	}

	Color::Color(double r, double g, double b)
	{
		if(r > 0. && r < 1.)
			m_r = r;
		else if (r <= 0.)
			m_r = 0.;
		else if (r >= 1.)
			m_r = 1.;

		if(g > 0. && g < 1.)
			m_g = g;
		else if (g <= 0.)
			m_g = 0.;
		else if (g >= 1.)
			m_g = 1.;

		if(b > 0. && b < 1.)
			m_b = b;
		else if (b <= 0.)
			m_b = 0.;
		else if (b >= 1.)
			m_b = 1.;
	}

	Color::Color(double rgb)
	{
		if(rgb > 0. && rgb < 1.)
		{
			m_r = rgb;
			m_g = rgb;
			m_b = rgb;
		}
		else if (rgb <= 0.)
		{
			m_r = 0.;
			m_g = 0.;
			m_b = 0.;
		}
		else if (rgb >= 1.)
		{
			m_r = 1.;
			m_g = 1.;
			m_b = 1.;
		}
	}

	Color::~Color()
	{

	}

	void Color::write(tinyxml2::XMLPrinter &printer, const std::string attributeName)
	{
		std::stringstream ss;

		ss << m_r << ' ' << m_g << ' ' << m_b;

		std::string s = ss.str();

		printer.PushAttribute(attributeName.c_str(), s.c_str());
	}

	void Color::setR(double r)
	{
		if(r > 0. && r < 1.)
			m_r = r;
		else if (r <= 0.)
			m_r = 0.;
		else if (r >= 1.)
			m_r = 1.;
	}
	
	void Color::setG(double g)
	{
		if(g > 0. && g < 1.)
			m_g = g;
		else if (g <= 0.)
			m_g = 0.;
		else if (g >= 1.)
			m_g = 1.;
	}
	
	void Color::setB(double b)
	{
		if(b > 0. && b < 1.)
			m_b = b;
		else if (b <= 0.)
			m_b = 0.;
		else if (b >= 1.)
			m_b = 1.;
	}
}