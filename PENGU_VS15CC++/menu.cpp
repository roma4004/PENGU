#include "menu.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void mainMenu(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	PENGU::menu form;
	Application::Run(%form);
}