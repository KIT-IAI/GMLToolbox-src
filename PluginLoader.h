#pragma once
#include <typeinfo>
#include "GMLToolboxPlugin.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Reflection;
using namespace KIT::GMLToolbox::Plugin;

namespace KIT {
  namespace GMLToolbox {
    namespace Plugin {
      public ref class PluginLoader
      {
      public:
        static Type^ CheckAssembly(String^ assemblyFile)
        {
          Type^ theResult = nullptr;
          Type^ theType = IGMLToolboxCommand::typeid;
          auto assemblyName = gcnew AssemblyName();
          assemblyName->CodeBase = assemblyFile;
          auto appDomain = AppDomain::CreateDomain("PluginReflectionDomain");
          auto assembly = appDomain->Load(assemblyName);
          auto types = assembly->GetTypes();
          for each (auto type in types)
          {
            if (theType->IsAssignableFrom(type) && type != theType)
            {
              theResult = type;
              break;
            }
          }
          AppDomain::Unload(appDomain);

          return theResult;
        }

      public:
        static IGMLToolboxCommand^ LoadAssembly(String^ assemblyFile)
        {
          IGMLToolboxCommand^ theInstance = nullptr;
          auto theType = CheckAssembly(assemblyFile);
          if (theType)
          {
            auto assembly = Assembly::LoadFrom(assemblyFile);
            auto theInst = assembly->CreateInstance(theType->FullName);
            theInstance = dynamic_cast<IGMLToolboxCommand^>(theInst);
          }

          return theInstance;
        }

      public:
        static List<IGMLToolboxCommand^>^ LoadPlugins()
        {
          auto results = gcnew List<IGMLToolboxCommand^>();
          auto appPath = AppDomain::CurrentDomain->BaseDirectory;
          auto files = Directory::EnumerateFiles(Path::GetDirectoryName(appPath), "*.dll");
          for each (auto file in files)
          {
            auto inst = LoadAssembly(file);
            if (inst)
            {
              results->Add(inst);
            }
          }
          return results;
        }
      };
    }
  }
}
