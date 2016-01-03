#include "MenuForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void MainMenu(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	VS15CppNET452::MyForm form;
	Application::Run(%form);
}