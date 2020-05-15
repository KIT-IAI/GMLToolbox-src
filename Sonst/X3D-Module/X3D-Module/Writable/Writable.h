#pragma once

#include "..\tinyxml2.h"

namespace X3D
{
	class Writable
	{
	public:
		virtual ~Writable() {}
		virtual void write(tinyxml2::XMLPrinter &printer) = 0;
	};
}

