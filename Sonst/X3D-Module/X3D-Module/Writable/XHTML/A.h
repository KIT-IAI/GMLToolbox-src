#pragma once

#include "..\EVENTable.h"
#include "..\IDable.h"
#include "XHTMLNode.h"
#include "Dl.h"

#include "..\..\tinyxml2.h"
#include <string>

namespace X3D
{
  class A : public XHTMLNode
  {
    public:
      A();
      A(const std::string& ident, const std::string& nam, const std::string& textNode);
      virtual ~A();

      void setTextNode(const std::string& textNode)	{ m_textNode = textNode; }

      std::string getTextNode() const { return m_textNode; }

      virtual void write(tinyxml2::XMLPrinter& printer);

    private:
      //element:
      std::string m_textNode;
  };
}
