#include "IndexedFaceSet.h"

namespace X3D
{
  IndexedFaceSet::IndexedFaceSet()
    : m_Solid(false)
  {
  }

  IndexedFaceSet::IndexedFaceSet(const std::string& ident, const std::vector<int>& coordIndex, Coordinate* coordinate)
    : m_Solid(false)
  {
    setID(ident);
    setCoordIndex(coordIndex);
    setCoordinate(coordinate);
  }

  IndexedFaceSet::~IndexedFaceSet()
  {
  }

  void IndexedFaceSet::write(tinyxml2::XMLPrinter& printer)
  {
    printer.OpenElement("IndexedFaceSet");

    IDable::write(printer);
    DEFable::write(printer);
    EVENTable::write(printer);

    std::string solid("false");
    if (m_Solid == true) solid = "true";

    printer.PushAttribute("solid", solid.c_str());

    Coordinate * pCoord = getCoordinate();

    Indexable::write(printer);

    pCoord->write(printer);

    printer.CloseElement();
  }
}