#include "Indexable.h"
#include <sstream>
#include <string>
#include <iostream>


namespace X3D
{
	Indexable::~Indexable()
	{

	}

	void Indexable::addCoordIndex(int index)
	{
		if(index >= -1)
		{
			m_coordIndex.push_back(index);
		}
		else if (index < -1)
		{
			m_coordIndex.push_back(-1);
		}
	}
	
	void Indexable::write(tinyxml2::XMLPrinter& printer)
	{
		std::stringstream ss;

		for(auto iter=m_coordIndex.begin(); iter!=m_coordIndex.end(); ++iter)
		{
			ss << *iter << ' ';
		}

		std::string s = ss.str();

		printer.PushAttribute("coordIndex", s.c_str());
	}
}