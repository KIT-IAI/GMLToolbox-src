#include "Grouping.h"

namespace X3D
{

	Grouping::~Grouping()
	{

	}

	/*
	IDable* Grouping::getElementById(const std::string ident)
	{
		std::string ownId = getID();

		if (ident == ownId)
		{
			return this;
		}
		else
		{
			for (auto iter = m_shapes.begin(); iter != m_shapes.end(); ++iter)
			{
				if(iter->getElementById(ident))
				{
					return iter->getElementById(ident);
				}
			}

			for (auto iter = m_groupings.begin(); iter != m_groupings.end(); ++iter)
			{
				if((*iter)->getElementById(ident))
				{
					return (*iter)->getElementById(ident);
				}
			}
		}
	}
	*/

	void X3D::Grouping::write(tinyxml2::XMLPrinter& printer)
	{
		for (auto iter = m_groupings.begin(); iter != m_groupings.end(); iter++)
		{
			(*iter)->write(printer);
		}

		for (auto iter = m_shapes.begin(); iter != m_shapes.end(); iter++)
		{
			(*iter)->write(printer);
		}
	}
}