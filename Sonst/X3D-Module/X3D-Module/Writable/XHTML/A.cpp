#include "A.h"

namespace X3D
{
  A::A()
  {
  }

  A::A(const std::string& ident, const std::string& nam, const std::string& textNode)
  {
    setID(ident);
    setName(nam);
    m_textNode = textNode;
  }

  A::~A()
  {
  }

  void A::write(tinyxml2::XMLPrinter& printer)
  {
    printer.OpenElement("a");

    //IDable::write(printer); // überschreiben, da name in xhtml5 nicht unterstützt wird!
    if (!m_id.empty())
    {
      printer.PushAttribute("id", m_id.c_str());
    }
    if (!m_class.empty())
    {
      printer.PushAttribute("class", m_class.c_str());
    }

    EVENTable::write(printer);

    XHTMLNode::write(printer);

    printer.PushText(m_textNode.c_str());

    printer.CloseElement();
  }
}
