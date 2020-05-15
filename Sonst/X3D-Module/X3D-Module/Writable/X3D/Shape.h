#pragma once

#include "..\Metaable.h"
#include "X3DAppearance.h"
#include "ComposedGeometryNode.h"
#include "..\..\tinyxml2.h"

namespace X3D
{
	class Shape : public Metaable
	{
	public:
		Shape();
		Shape(const Shape& sh);
		Shape(const std::string& ident, Appearance* ap, ComposedGeometryNode* cg);
		Shape(const std::string& ident, Appearance* ap);
		Shape(Appearance* ap, ComposedGeometryNode* cg);
		Shape(Appearance* appearance);
		virtual ~Shape();

		void setAppearance(Appearance* appearance) { m_appearance = appearance; }
		void setComposedGeometryNode(ComposedGeometryNode* compGeomNode) { m_composedGeometryNode = compGeomNode; }

		//virtual IDable* getElementById(const std::string ident);

		virtual void write(tinyxml2::XMLPrinter& printer);
		
	private:
		//attributes

		
		//child elements
		Appearance*				m_appearance;
		ComposedGeometryNode*	m_composedGeometryNode;
	};
}