#pragma once

#include "..\Metaable.h"
#include "Material.h"
#include "..\..\EnumDefinition.h"

#include "..\..\tinyxml2.h"
#include <string>

using namespace tinyxml2;

namespace X3D
{
	class Appearance : public Metaable
	{
	public:
		Appearance();
		Appearance(const std::string& ident, Material* mat);
		Appearance(const std::string& ident, const std::string& use);
		virtual ~Appearance();

		void setMaterial(Material* material) { m_material = material; }

		//virtual IDable* getElementById(const std::string ident);

		virtual void write(XMLPrinter& printer);
		
	private:
		//attributes

		
		//child elements
		Material*				m_material;
	};
}