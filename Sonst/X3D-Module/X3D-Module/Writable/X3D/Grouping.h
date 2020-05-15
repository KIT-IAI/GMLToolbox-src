#pragma once

#include "..\Metaable.h"
#include "Optional.h"
#include "Shape.h"

#include <list>

namespace X3D
{
	class Grouping : public Metaable
	{
	public:
		virtual ~Grouping();

		void addGrouping(Grouping* grouping)	{ m_groupings.push_back(grouping); }
		void addShape(Shape* shape)				{ m_shapes.push_back(shape); }

		void defineChildren(const std::list<Grouping*>& groupings)	{ m_groupings = groupings; }
		void defineShapes(const std::list<Shape*>& sh)				{ m_shapes = sh; }

		//virtual IDable* getElementById(const std::string ident);

		virtual void write(tinyxml2::XMLPrinter& printer);

	private:
		//child elements
		std::list<Shape*>		m_shapes;
		std::list<Grouping*>	m_groupings;

		void writeGroups(tinyxml2::XMLPrinter& printer);
	};
}

