#pragma once

public ref class AppConfigPath
{
  public:
    AppConfigPath() {}

    ~AppConfigPath() {}

    static AppConfigPath^ getInstance()
    {
      if (m_AppPathInstance == nullptr)
      {
        m_AppPathInstance = gcnew AppConfigPath();
        m_AppPathInstance->createPath();
      }

      return m_AppPathInstance;
    }

    String^ getPath() { return m_Path; }

  private:
    AppConfigPath(AppConfigPath^) {}
    AppConfigPath^ operator=(AppConfigPath^) { return this; }

    void createPath()
    {
      String^ appPath = System::Windows::Forms::Application::ExecutablePath;

      int index;
      index = appPath->LastIndexOf("\\");
      m_Path = appPath->Substring(0, index + 1);

      if (System::IO::File::Exists(m_Path + "GML-Toolbox.ini") == false)
      {
        m_Path += "..\\..\\GML-Toolbox\\AppConfig\\";
      }
    }

  private:
    String^ m_Path;
    static AppConfigPath^ m_AppPathInstance = nullptr;
};

