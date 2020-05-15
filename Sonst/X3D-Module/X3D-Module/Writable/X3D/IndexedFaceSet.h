#pragma once

#include "Indexable.h"
#include "..\..\tinyxml2.h"

namespace X3D
{
  class IndexedFaceSet : public Indexable
  {
  public:
    IndexedFaceSet();
    IndexedFaceSet(const std::string& ident, const std::vector<int>& coordIndex, Coordinate* coordinate);
    virtual ~IndexedFaceSet();

    void setSolid(bool state) { m_Solid = state; }

    void write(tinyxml2::XMLPrinter& printer);

  private:
    bool m_Solid;
  };
}