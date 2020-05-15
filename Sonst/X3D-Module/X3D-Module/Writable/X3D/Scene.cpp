#include "Scene.h"

namespace X3D
{
	Scene::Scene()
	{
		m_background = new Background();
		m_viewpoints.push_back(new Viewpoint());
	}

	Scene::Scene(Background* background, Viewpoint* viewpoint)
	{
		m_background = background;
		m_viewpoints.push_back(viewpoint);
	}

	Scene::~Scene()
	{

	}
	/*
	IDable* Scene::getElementById(const std::string ident)
	{
		std::string ownId = getID();

		if (ident == ownId)
		{
			return this;
		}
		else
		{
			if (m_background.getElementById(ident))
			{
				return m_background.getElementById(ident);
			}

			for (auto iter = m_viewpoints.begin(); iter != m_viewpoints.end(); ++iter)
			{
				if(iter->getElementById(ident))
				{
					return iter->getElementById(ident);
				}
			}

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
		return nullptr;
	}
	*/

	void Scene::write(tinyxml2::XMLPrinter& printer)
	{
		printer.OpenElement("Scene");
		
		IDable::write(printer);
		DEFable::write(printer);

		m_background->write(printer);

		writeViewpoints(printer);
		
		// probably if
		writeShapes(printer);
		writeGroupings(printer);
		
		printer.CloseElement();
	}

	//write child elements
	void Scene::writeViewpoints(tinyxml2::XMLPrinter& printer)
	{
		for (auto iter = m_viewpoints.begin(); iter != m_viewpoints.end(); iter++)
		{
			(*iter)->write(printer);
		}
	}

	void Scene::writeGroupings(XMLPrinter& printer)
	{
		for (auto iter = m_groupings.begin(); iter != m_groupings.end(); iter++)
		{
			(*iter)->write(printer);
		}
	}
	
	void Scene::writeShapes(XMLPrinter& printer)
	{
		for (auto iter = m_shapes.begin(); iter != m_shapes.end(); iter++)
		{
			(*iter)->write(printer);
		}
	}
}