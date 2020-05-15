#include "Shape.h"
#include "ComposedGeometryNode.h"

namespace X3D
{
	Shape::Shape()
	{
		m_composedGeometryNode = nullptr;
	}

	Shape::Shape(const Shape& sh)
		: Metaable(sh)
	{
		setID(sh.getID());
		setClass(sh.getClass());
		//setDEF(sh.getDEF());
		setUSE(sh.getUSE());

		m_appearance = sh.m_appearance;
		m_composedGeometryNode = sh.m_composedGeometryNode;
	}

	Shape::Shape(Appearance* ap)
	{
		m_appearance = ap;
		m_composedGeometryNode = nullptr;
	}

	Shape::Shape(Appearance* ap, ComposedGeometryNode* cg)
	{
		m_appearance = ap;
		m_composedGeometryNode = cg;
	}

	Shape::Shape(const std::string& ident, Appearance* ap, ComposedGeometryNode* cg)
	{
		setID(ident);

		m_appearance = ap;
		m_composedGeometryNode = cg;
	}

	Shape::Shape(const std::string& ident, Appearance* ap)
	{
		setID(ident);

		m_appearance = ap;
		m_composedGeometryNode = nullptr;
	}

	Shape::~Shape()
	{

	}

	/*
	IDable* Shape::getElementById(const std::string ident)
	{
		std::string ownId = getID();

		if (ident == ownId)
		{
			return this;
		}
		else
		{
			if(m_appearance.getElementById(ident))
			{
				return m_appearance.getElementById(ident);
			}
			if(m_composedGeometryNode != nullptr)
			{
				return m_composedGeometryNode->getElementById(ident);
			}
		}
		return nullptr;
	}
	*/

	void Shape::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("Shape");
		
		IDable::write(printer);
		DEFable::write(printer);
		EVENTable::write(printer);

		m_appearance->write(printer);
		if (m_composedGeometryNode != nullptr)
		{
			m_composedGeometryNode->write(printer);
		}

		printer.CloseElement();
	}
}