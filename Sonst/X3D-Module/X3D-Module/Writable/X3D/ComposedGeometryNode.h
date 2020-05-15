#pragma once

#include "..\Metaable.h"
#include "..\IDable.h"
#include "Coordinate.h"
#include "..\..\tinyxml2.h"

using namespace tinyxml2;

namespace X3D{
	class ComposedGeometryNode : public Metaable
	{
	public:
		virtual ~ComposedGeometryNode() {}

		void setCoordinate(Coordinate* coordinate) { m_coordinate = coordinate; }
		Coordinate* getCoordinate() const { return m_coordinate; }
		
		//virtual IDable* getElementById(const std::string ident);

	private:
		//attributes
		

		//Child elements
		Coordinate*				m_coordinate;
		
	};
}