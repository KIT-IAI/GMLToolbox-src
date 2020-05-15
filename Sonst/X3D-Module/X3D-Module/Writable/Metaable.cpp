#include "Metaable.h"
#include "GlobalDef.h"

namespace X3D
{
  void Metaable::write(tinyxml2::XMLPrinter& printer)
  {
    if(!m_metadataString.empty() || !m_metadataDouble.empty() || !m_metadataFloat.empty() || !m_metadataInt.empty())
    {
      printer.OpenElement("MetadataSet");

      std::string sMeta = getID() + "_Meta";

      if (X3DGlobals::X3DFileOnly == false)
      {
        printer.PushAttribute("id", sMeta.c_str());
        printer.PushAttribute("style", "display: none;");
      }

      printer.PushAttribute("containerField", "metadata");

      for (auto iter = m_metadataString.begin(); iter != m_metadataString.end(); ++iter)
      {
        printer.OpenElement("MetadataString");

        printer.PushAttribute("name", iter->first.c_str());
        printer.PushAttribute("containerField", "value");

        printer.PushText(iter->second.c_str());

        printer.CloseElement();
      }

      for (auto iter = m_metadataDouble.begin(); iter != m_metadataDouble.end(); ++iter)
      {
        printer.OpenElement("MetadataDouble");

        printer.PushAttribute("name", iter->first.c_str());
        printer.PushAttribute("containerField", "value");

        printer.PushText(iter->second);

        printer.CloseElement();
      }

      for (auto iter = m_metadataFloat.begin(); iter != m_metadataFloat.end(); ++iter)
      {
        printer.OpenElement("MetadataFloat");

        printer.PushAttribute("name", iter->first.c_str());
        printer.PushAttribute("containerField", "value");

        printer.PushText(iter->second);

        printer.CloseElement();
      }

      for (auto iter = m_metadataInt.begin(); iter != m_metadataInt.end(); ++iter)
      {
        printer.OpenElement("MetadataInteger");

        printer.PushAttribute("name", iter->first.c_str());
        printer.PushAttribute("containerField", "value");

        printer.PushText(iter->second);

        printer.CloseElement();
      }

      for (auto iter = m_metadataBool.begin(); iter != m_metadataBool.end(); ++iter)
      {
        printer.OpenElement("MetadataBoolean");

        printer.PushAttribute("name", iter->first.c_str());
        printer.PushAttribute("containerField", "value");

        printer.PushText(iter->second);

        printer.CloseElement();
      }

      printer.CloseElement();
    }
  }
}