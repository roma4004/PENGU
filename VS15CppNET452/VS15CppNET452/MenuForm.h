#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
namespace VS15CppNET452 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnExit;

	private: System::Windows::Forms::Button^  btnStart;
	protected:

	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnExit = (gcnew System::Windows::Forms::Button());
			this->btnStart = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// btnExit
			// 
			this->btnExit->Location = System::Drawing::Point(164, 188);
			this->btnExit->Name = L"btnExit";
			this->btnExit->Size = System::Drawing::Size(108, 61);
			this->btnExit->TabIndex = 0;
			this->btnExit->Text = L"Выйти";
			this->btnExit->UseVisualStyleBackColor = true;
			this->btnExit->Click += gcnew System::EventHandler(this, &MyForm::btnExit_Click);
			// 
			// btnStart
			// 
			this->btnStart->Location = System::Drawing::Point(12, 188);
			this->btnStart->Name = L"btnStart";
			this->btnStart->Size = System::Drawing::Size(108, 61);
			this->btnStart->TabIndex = 0;
			this->btnStart->Text = L"Старт";
			this->btnStart->UseVisualStyleBackColor = true;
			this->btnStart->Click += gcnew System::EventHandler(this, &MyForm::btnStart_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->btnStart);
			this->Controls->Add(this->btnExit);
			this->Name = L"MyForm";
			this->Text = L"PENGU";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void btnExit_Click(System::Object^  sender, System::EventArgs^  e) {
		Close();
	}
	private: System::Void btnStart_Click(System::Object^  sender, System::EventArgs^  e) {
		sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!"); //код запуска игры 
		sf::CircleShape shape(100.f);
		shape.setFillColor(sf::Color::Green);

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}
			window.clear();
			window.draw(shape);
			window.display();
		}
	}
	};
}
