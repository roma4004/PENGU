#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void MainMenu(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Launcher::MainForm form;
	Application::Run(%form);
}