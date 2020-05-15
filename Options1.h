#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "GmlToolbox.h"

	/// <summary>
	/// Einstellung allgemeiner Optionen
	///
	/// </summary>
	public ref class Options1 : public System::Windows::Forms::Form
	{
	public:
		Options1( GmlToolbox * pViewer )
		{
			InitializeComponent( );

      m_pViewer = pViewer;
      if ( m_pViewer->getReadCodeListDictionaries() )
        checkBoxCodeListDictionaries->Checked = true;
      else
        checkBoxCodeListDictionaries->Checked = false;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Options1()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::CheckBox^  checkBoxCodeListDictionaries;
  private: System::Windows::Forms::Button^  buttonBeenden;
  protected: 

	private:
    GmlToolbox * m_pViewer;
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
      this->checkBoxCodeListDictionaries = (gcnew System::Windows::Forms::CheckBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // checkBoxCodeListDictionaries
      // 
      this->checkBoxCodeListDictionaries->AutoSize = true;
      this->checkBoxCodeListDictionaries->Location = System::Drawing::Point(13, 29);
      this->checkBoxCodeListDictionaries->Name = L"checkBoxCodeListDictionaries";
      this->checkBoxCodeListDictionaries->Size = System::Drawing::Size(220, 17);
      this->checkBoxCodeListDictionaries->TabIndex = 0;
      this->checkBoxCodeListDictionaries->Text = L"Codelisten als GML-Dictionaries einlesen ";
      this->checkBoxCodeListDictionaries->UseVisualStyleBackColor = true;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonBeenden->Location = System::Drawing::Point(205, 102);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 1;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &Options1::buttonBeenden_Click);
      // 
      // Options1
      // 
      this->AcceptButton = this->buttonBeenden;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->buttonBeenden;
      this->ClientSize = System::Drawing::Size(292, 147);
      this->ControlBox = false;
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->checkBoxCodeListDictionaries);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"Options1";
      this->Text = L"Allgemeine Einstellungen";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
 
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"                                          //
//  Steuert das automatische Einlesen externer Codelist-Dictionaries         //
///////////////////////////////////////////////////////////////////////////////
  private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
  {
   if ( checkBoxCodeListDictionaries->Checked )
     m_pViewer->setReadCodeListDictionaries( true );
   else
     m_pViewer->setReadCodeListDictionaries( false );

   Close();
  }
  };
