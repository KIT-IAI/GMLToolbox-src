#include "Group.h"
#include "..\GlobalDef.h"

namespace X3D
{
  Group::Group()
  {
  }

  Group::Group(const std::string ident, const std::list<Grouping*> groupings)
  {
    setID(ident);
    defineChildren(groupings);
  }

  Group::Group(const std::string ident, const std::list<Shape*> shapes)
  {
    setID(ident);
    defineShapes(shapes);
  }

  Group::Group(const std::string ident, const std::list<Shape*> shapes, const std::list<Grouping*> groupings)
  {
    setID(ident);
    defineShapes(shapes);
    defineChildren(groupings);
  }

  Group::Group(const std::string ident)
  {
    setID(ident);
  }

  Group::~Group()
  {
  }

  void Group::write(tinyxml2::XMLPrinter& printer)
  {
    printer.OpenElement("Group");

    if (X3DGlobals::X3DFileOnly == true)
    {
      if (!m_class.empty())
      {
        printer.PushAttribute("class", m_class.c_str());
      }
    }
    else
    {
      IDable::write(printer);
    }

    DEFable::write(printer);

    Metaable::write(printer);

    Grouping::write(printer);

    printer.CloseElement();
  }
}
