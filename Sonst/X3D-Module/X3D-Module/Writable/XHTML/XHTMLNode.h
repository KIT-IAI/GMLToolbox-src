#pragma once

#include "../EVENTable.h"
#include "../IDable.h"

#include <list>
#include "..\..\tinyxml2.h"

class IDable;

namespace X3D
{
	class XHTMLNode : public EVENTable
	{
	public:
		virtual ~XHTMLNode();

		virtual void	setChildList(const std::list<XHTMLNode*>& childList)	{ m_childList = childList; }

		virtual void	addChild(XHTMLNode* child)	{ m_childList.push_back(child); }

		virtual std::list<XHTMLNode*>	getChildren()	const	{ return m_childList; }

		//virtual IDable* getElementById(const std::string ident);

		virtual void	write(tinyxml2::XMLPrinter& printer);

	private:
		//elemente:
		std::list<XHTMLNode*>	m_childList;
	};
}

