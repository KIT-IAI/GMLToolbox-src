#pragma once
#include "Coordinate.h"

#include <sstream>
#include <string>
#include <iomanip>

namespace X3D
{
	Coordinate::Coordinate()
	{

	}

	Coordinate::Coordinate(const std::string& use)
	{
		setUSE(use);
	}
	
	Coordinate::Coordinate(const std::string& ident, const std::vector<Point3D*> points, Point3D * minCoordinateP )
	{
		setID(ident);

		m_points      = points;
    minCoordinate = minCoordinateP;
	}

	Coordinate::~Coordinate()
	{
    for ( unsigned int i = 0; i < m_points.size(); i++ )
      delete m_points[i];
	}

	/*
	IDable* Coordinate::getElementById(const std::string ident)
	{
		std::string ownId = getID();

		if (ident == ownId)
		{
			return this;
		}
	}
	*/

	void Coordinate::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("Coordinate");
		
		IDable::write(printer);
		DEFable::write(printer);

		if(!m_points.empty())
			writePoints(printer);
		
		//m_coordinate->write(printer);

		printer.CloseElement();
	}

	void Coordinate::writePoints(tinyxml2::XMLPrinter& printer)
	{
		std::stringstream ss;

		for(auto iter = m_points.begin(); iter != m_points.end(); iter++)
		{
			ss << std::fixed << std::setprecision(3) << (*iter)->getX() - minCoordinate->getX() << ' ';
			ss << std::fixed << std::setprecision(3) << (*iter)->getY() - minCoordinate->getY() << ' ';
			ss << std::fixed << std::setprecision(3) << (*iter)->getZ() - minCoordinate->getZ()  << "   ";
		}

		std::string s = ss.str();

		printer.PushAttribute("point", s.c_str());
	}
}