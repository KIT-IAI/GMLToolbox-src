#pragma once

#include "..\Metaable.h"
#include "Shape.h"
#include "Grouping.h"
#include "..\..\tinyxml2.h"

#include <list>

namespace X3D
{
  class Group : public Grouping
  {
  public:
    Group();
    Group(const std::string ident);
    Group(const std::string ident, const std::list<Shape*> shapes, const std::list<Grouping*> groupings);
    Group(const std::string ident, const std::list<Shape*> shapes);
    Group(const std::string ident, const std::list<Grouping*> groupings);
    virtual ~Group();

    virtual void write(tinyxml2::XMLPrinter& printer);
  };
}