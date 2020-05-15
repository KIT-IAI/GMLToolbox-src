#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


	/// <summary>
	/// Zusammenfassung für HTMLDatei
  /// Web-Browser
  ///  
	///
	/// </summary>
	public ref class HTMLDatei : public System::Windows::Forms::Form
	{
	public:
		HTMLDatei(String ^ fileName )
		{
			InitializeComponent();

      webBrowser1->Navigate( fileName );
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~HTMLDatei()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::WebBrowser^  webBrowser1;
  protected: 

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->webBrowser1 = (gcnew System::Windows::Forms::WebBrowser());
      this->SuspendLayout();
      // 
      // webBrowser1
      // 
      this->webBrowser1->Dock = System::Windows::Forms::DockStyle::Fill;
      this->webBrowser1->Location = System::Drawing::Point(0, 0);
      this->webBrowser1->MinimumSize = System::Drawing::Size(20, 20);
      this->webBrowser1->Name = L"webBrowser1";
      this->webBrowser1->Size = System::Drawing::Size(638, 633);
      this->webBrowser1->TabIndex = 0;
      // 
      // HTMLDatei
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(638, 633);
      this->Controls->Add(this->webBrowser1);
      this->Name = L"HTMLDatei";
      this->Text = L"HTML-Datei";
      this->ResumeLayout(false);

    }
#pragma endregion
	};
