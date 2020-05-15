#include "IDable.h"
#include "GlobalDef.h"
#include <algorithm>
#include <functional>

namespace X3D
{
  /*IDable* IDable::getElementById(const std::string ident)
  {
    std::map<std::string, IDable*>::iterator it = m_mX3d.find(ident);
    if (it != m_mX3d.end())
    {
      return it->second;
    }
    return nullptr;
  }*/

  void IDable::setID(const std::string& id)
  {
    m_id = id;
    //std::replace_if(m_id.begin(), m_id.end(), [](char c) { return !isalnum(c); }, '_');
  }

  void IDable::write(tinyxml2::XMLPrinter& printer)
  {
    if (!m_id.empty() && X3DGlobals::X3DFileOnly == false)
    {
      printer.PushAttribute("id", m_id.c_str());
    }
    if(!m_name.empty())
    {
      printer.PushAttribute("name", m_name.c_str());
    }
    if(!m_class.empty())
    {
      printer.PushAttribute("class", m_class.c_str());
    }
  }
}
