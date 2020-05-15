#pragma once

#include "..\Metaable.h"
#include "X3DColor.h"
#include "..\..\EnumDefinition.h"
#include "..\IDable.h"
//#include "VisualParams.h"

#include "..\..\tinyxml2.h"

#include <string>
#include <sstream>

namespace X3D
{
	class Background : public Metaable
	{
	public:
		Background();
		Background(const std::string& ident);
		Background(const Color skyColor, double trans);
		Background(const std::string& ident, const Color skyColor, double trans);
		virtual ~Background();
		
		void write(tinyxml2::XMLPrinter& printer);
		void setSkyColor(const Color& skyColor)		{ m_skyColor = skyColor; }
		void setTransparency(double transparency)		{ m_transparency = transparency; }

		Color	getSkyColor()		const { return m_skyColor; }
		double					getTransparency()	const { return m_transparency; }

		//virtual IDable* getElementById(const std::string ident);

	private:
		//attributes
		Color	m_skyColor;
		//Color*		m_groundColor;
		double		m_transparency;
	};
}
