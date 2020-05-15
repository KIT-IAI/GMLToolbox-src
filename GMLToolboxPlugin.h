#pragma once

using namespace System;

namespace KIT {
  namespace GMLToolbox {
    namespace Plugin {

      public interface class IGMLToolboxHost
      {
        enum class MenuPos { mpTools, mpVisualizer, mpGenerator };
        void AddMenuItem(String^ menuText, bool bSeparator, MenuPos ePos, EventHandler^ onClick);
      };

      public interface class IGMLToolboxCommand
      {
        void RegisterPlugin(IGMLToolboxHost^ theHost);
        //void Dummy();
        //property int dummyNum { int get(); }
      };
    }
  }
}
