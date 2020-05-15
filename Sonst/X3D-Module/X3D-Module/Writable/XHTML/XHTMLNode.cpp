#include "XHTMLNode.h"

#include "..\IDable.h"

namespace X3D
{
	XHTMLNode::~XHTMLNode()
	{
 		for (auto iter = m_childList.begin(); iter != m_childList.end(); ++iter)
 		{
 			delete *iter;
 		}
	}

	/*
	IDable* XHTMLNode::getElementById(const std::string ident)
	{
		std::string ownId = getID();

		if (ident == ownId)
		{
			return this;
		}

		if (!m_childList.empty())
		{
			for (auto iter = m_childList.begin(); iter != m_childList.end(); ++iter)
			{
				if ((*iter)->getElementById(ident))
				{
					return (*iter)->getElementById(ident);
				}
			}
		}

		return nullptr;
	}
	*/

	void XHTMLNode::write(tinyxml2::XMLPrinter& printer)
	{
// 		if (!m_childList.empty())
// 		{
			for (auto iter = m_childList.begin(); iter != m_childList.end(); ++iter)
			{
				(*iter)->write(printer);
			}
		//}
	}
}
