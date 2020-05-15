#include "Switch.h"
#include "..\GlobalDef.h"

namespace X3D
{
  Switch::Switch()
  {
    m_whichChoice = -1;
  }

  Switch::Switch(int wc)
  {
    m_whichChoice = wc;
  }

  Switch::Switch(std::list<Shape*> sh)
  {
    defineShapes(sh);
    m_whichChoice	= -1;
  }

  Switch::Switch(std::list<Shape*> sh, int wc)
  {
    defineShapes(sh);
    
    if (wc < -1)
    {
      m_whichChoice = -1;
    }
    else if (wc >= -1)
    {
      m_whichChoice = wc;
    }
  }

  Switch::Switch(std::list<Grouping*> groupings)
  {
    defineChildren(groupings);
    m_whichChoice			= -1;
  }

  Switch::Switch(std::list<Grouping*> groupings, int wc)
  {
    defineChildren(groupings);
    
    if (wc < -1)
    {
      m_whichChoice = -1;
    }
    else if (wc >= -1)
    {
      m_whichChoice = wc;
    }
  }

  Switch::Switch(std::list<Grouping*> groupings, std::list<Shape*> sh, int wc)
  {
    defineChildren(groupings);
    defineShapes(sh);

    if (wc < -1)
    {
      m_whichChoice = -1;
    }
    else if (wc >= -1)
    {
      m_whichChoice = wc;
    }
  }

  Switch::Switch(std::list<Grouping*> groupings, std::list<Shape*> sh)
  {
    defineChildren(groupings);
    defineShapes(sh);
    m_whichChoice	= -1;
  }




  Switch::Switch(const std::string& ident, std::list<Grouping*> groupings, std::list<Shape*> sh)
  {
    setID(ident);
    defineChildren(groupings);
    defineShapes(sh);
    m_whichChoice	= -1;
  }

  Switch::Switch(const std::string& ident, std::list<Grouping*> groupings, std::list<Shape*> sh, int wc)
  {
    setID(ident);
    defineChildren(groupings);
    defineShapes(sh);

    if (wc < -1)
    {
      m_whichChoice = -1;
    }
    else if (wc >= -1)
    {
      m_whichChoice = wc;
    }
  }

  Switch::Switch(const std::string& ident, std::list<Grouping*> groupings, int wc)
  {
    setID(ident);
    defineChildren(groupings);

    if (wc < -1)
    {
      m_whichChoice = -1;
    }
    else if (wc >= -1)
    {
      m_whichChoice = wc;
    }
  }

  Switch::Switch(const std::string& ident, std::list<Grouping*> groupings)
  {
    setID(ident);
    defineChildren(groupings);
  }

  Switch::Switch(const std::string& ident, std::list<Shape*> sh, int wc)
  {
    setID(ident);
    defineShapes(sh);

    if (wc < -1)
    {
      m_whichChoice = -1;
    }
    else if (wc >= -1)
    {
      m_whichChoice = wc;
    }
  }

  Switch::Switch(const std::string& ident, std::list<Shape*> sh)
  {
    setID(ident);
    defineShapes(sh);
  }

  Switch::Switch(const std::string& ident, int wc)
  {
    setID(ident);

    if (wc < -1)
    {
      m_whichChoice = -1;
    }
    else if (wc >= -1)
    {
      m_whichChoice = wc;
    }
  }

  Switch::~Switch()
  {
  }

  void Switch::write(tinyxml2::XMLPrinter& printer)
  {
    printer.OpenElement("Switch");

    if (X3DGlobals::X3DFileOnly == true)
    {
      if (!m_name.empty())
      {
        printer.PushAttribute("name", m_name.c_str());
      }
    }
    else
    {
      IDable::write(printer);
    }

    DEFable::write(printer);

    printer.PushAttribute("whichChoice", m_whichChoice);

    Grouping::write(printer);

    printer.CloseElement();
  }

  void Switch::setWhichChoice(int wc)
  {
    if (wc < -1)
    {
      m_whichChoice = -1;
    }
    else if (wc >= -1)
    {
      m_whichChoice = wc;
    }
  }

}