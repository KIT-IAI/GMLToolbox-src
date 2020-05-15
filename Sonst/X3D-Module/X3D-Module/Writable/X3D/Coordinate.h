#pragma once

#include "..\Metaable.h"
#include "Point3D.h"
#include "..\..\EnumDefinition.h"

#include "..\..\tinyxml2.h"

#include <vector>

namespace X3D
{
	class Coordinate : public Metaable
	{
	public:
		Coordinate();
		Coordinate(const std::string& ident, const std::vector<Point3D*> points, Point3D * minCoordinateP );
		Coordinate(const std::string& use);
		virtual ~Coordinate();

		void addPoint(Point3D* point) { m_points.push_back(point); }

		//virtual IDable* getElementById(const std::string ident);

		virtual void write(tinyxml2::XMLPrinter& printer);
		
	private:
		//attributes
		std::vector<Point3D*>		m_points;
    Point3D               * minCoordinate;

		void writePoints(tinyxml2::XMLPrinter& printer);
	};
}