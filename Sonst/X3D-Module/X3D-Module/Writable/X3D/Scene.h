#pragma once

#include "..\Metaable.h"
#include "Background.h"
#include "Viewpoint.h"
#include "Switch.h"
#include "Group.h"
#include "Shape.h"
#include "..\..\EnumDefinition.h"

#include "..\..\tinyxml2.h"

#include <list>
#include <string>



namespace X3D
{

	//class Background;

	class Scene : public Metaable
	{
	public:
		Scene();
		Scene(Background* background, Viewpoint* viewpoint);
		
		virtual ~Scene();

		void setBackground(Background* background) { m_background = background; }
		void setViewpoints(const std::list<Viewpoint*>& viewpointList) { m_viewpoints = viewpointList; }
		void setShapes(const std::list<Shape*>& shapeList) { m_shapes = shapeList; }

		void addViewpoint(Viewpoint* viewpoint)		{ m_viewpoints.push_back(viewpoint); }
		void addGrouping(Grouping* grouping)		{ m_groupings.push_back(grouping); }
		void addShape(Shape* shape)			{ m_shapes.push_back(shape); }

		Background*				getBackground()	const { return m_background; }
		std::list<Viewpoint*>	getViewpoints()	const { return m_viewpoints; }
		std::list<Grouping*>	getGroupings()	const { return m_groupings; }
		std::list<Shape*>		getShapes()		const { return m_shapes; }

		//virtual IDable* getElementById(const std::string ident);

		virtual void write(tinyxml2::XMLPrinter& printer);

	private:
		//attributes

		
		//child elements
		Background*				m_background;
		std::list<Viewpoint*>	m_viewpoints;
		std::list<Grouping*>	m_groupings;
		std::list<Shape*>		m_shapes;

		//write child elements
		void writeViewpoints(tinyxml2::XMLPrinter& printer);
		void writeGroupings(tinyxml2::XMLPrinter& printer);
		void writeShapes(tinyxml2::XMLPrinter& printer);
	};
}