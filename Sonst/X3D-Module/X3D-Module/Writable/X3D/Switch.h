#pragma once

#include "..\Metaable.h"
#include "Grouping.h"
#include "Group.h"
#include "Shape.h"
#include "..\..\tinyxml2.h"

#include <list>

namespace X3D
{
	class Switch : public Grouping
	{
	public:
		Switch();
		Switch(std::list<Grouping*> groupings, std::list<Shape*> sh);
		Switch(std::list<Grouping*> groupings, std::list<Shape*> sh, int wc);
		Switch(std::list<Grouping*> groupings, int wc);
		Switch(std::list<Grouping*> groupings);
		Switch(std::list<Shape*> sh, int wc);
		Switch(std::list<Shape*> sh);
		Switch(int wc);

		Switch(const std::string& ident, std::list<Grouping*> groupings, std::list<Shape*> sh);
		Switch(const std::string& ident, std::list<Grouping*> groupings, std::list<Shape*> sh, int wc);
		Switch(const std::string& ident, std::list<Grouping*> groupings, int wc);
		Switch(const std::string& ident, std::list<Grouping*> groupings);
		Switch(const std::string& ident, std::list<Shape*> sh, int wc);
		Switch(const std::string& ident, std::list<Shape*> sh);
		Switch(const std::string& ident, int wc);

		virtual ~Switch();

		void write(tinyxml2::XMLPrinter& printer);
		void setWhichChoice(int wc);
		int getWhichChoice() const { return m_whichChoice; }

	private:
		int		m_whichChoice;

		void writeGroups(tinyxml2::XMLPrinter& printer);
	};
}