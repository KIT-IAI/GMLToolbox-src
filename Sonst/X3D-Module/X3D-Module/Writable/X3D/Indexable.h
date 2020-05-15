#pragma once

#include "ComposedGeometryNode.h"
#include "..\..\tinyxml2.h"

namespace X3D
{
	class Indexable : public ComposedGeometryNode
	{
	public:
		virtual ~Indexable();
		
		virtual void write(tinyxml2::XMLPrinter& printer);

		virtual void addCoordIndex(int index);
		virtual void setCoordIndex(const std::vector<int> coordIndex) { m_coordIndex = coordIndex; }
		virtual std::vector<int> getCoordIndex() { return m_coordIndex; }
		
	private:
		//attributes
		std::vector<int>	m_coordIndex;
	};
}