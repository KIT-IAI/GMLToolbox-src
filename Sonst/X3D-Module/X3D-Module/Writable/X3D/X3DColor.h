#pragma once
#include "../Writable.h"
#include <stdio.h>
#include <string>

#include "..\..\tinyxml2.h"

using namespace tinyxml2;

namespace X3D
{
	class Color //: public Writable
	{
	public:
		Color();
		Color(double r, double g, double b);
		Color(double rgb);
		virtual ~Color();

		virtual void write(tinyxml2::XMLPrinter &printer, const std::string attributeName);

		double getR() const { return m_r; }
		double getG() const { return m_g; }
		double getB() const { return m_b; }

		void setR(double r);
		void setG(double g);
		void setB(double b);

	private:
		//attributes
		double m_r;
		double m_g;
		double m_b;
	};
}