#include "stdafx.h"

#include <string>
#include <vector>
#include <set>

//#include "options.h"
#include "Form1.h"

#include <windows.h>


[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
  if ( args->Length == 0 )
  {
	  // Aktivieren visueller Effekte von Windows XP, bevor Steuerelemente erstellt werden
	  Application::EnableVisualStyles();
	  Application::SetCompatibleTextRenderingDefault(false); 


	  // Hauptfenster erstellen und ausführen
    Application::Run(gcnew Form1( args ));
  }
  else
  {
    AttachConsole( ATTACH_PARENT_PROCESS );
    gcnew Form1( args );
  }
	return 0;
}
/*
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	System::String ^ schemaFolder;
//	schemaFolder = lpCmdLine;

	System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;
	Form1 ^ myForm = gcnew Form1( schemaFolder );
	Application::Run( myForm );
	return 0;
}
*/