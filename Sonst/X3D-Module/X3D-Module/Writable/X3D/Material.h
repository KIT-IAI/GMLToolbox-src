#pragma once

#include "..\Metaable.h"
#include "X3DColor.h"
#include "..\..\tinyxml2.h"

namespace X3D
{
	class Material : public Metaable
	{
	public:
		Material();
		Material(const std::string& ident, Color* dif);
		Material(const std::string& ident, Color* dif, const double trans);
		Material(const std::string& ident, Color* dif, Color* em);
		Material(const std::string& ident, Color* dif, Color* em, const double trans);
		virtual ~Material();

		void setDiffuseColor(Color* diffuseColor)		{ m_diffuseColor = diffuseColor; }
		void setEmissiveColor(Color* emissiveColor)	{ m_emissiveColor = emissiveColor; }
		void setTransparency(double transparency)			{ m_transparency = transparency; }

		Color* getDiffuseColor()		{ return m_diffuseColor; }
		Color* getEmissiveColor()		{ return m_emissiveColor; }
		double getTransparency()	{ return m_transparency; }

		//virtual IDable* getElementById(const std::string ident);

		virtual void write(tinyxml2::XMLPrinter &printer);


	private:
		//attributes
		Color*		m_diffuseColor;
		Color*		m_emissiveColor;
		double		m_transparency;
	};
}