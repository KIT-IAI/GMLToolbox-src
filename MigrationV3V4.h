#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;



	/// <summary>
	/// Zusammenfassung für MigrationV3V4
	///
	/// Warnung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie auch
	///          die Ressourcendateiname-Eigenschaft für das Tool zur Kompilierung verwalteter Ressourcen ändern,
	///          das allen RESX-Dateien zugewiesen ist, von denen diese Klasse abhängt.
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class MigrationV3V4 : public System::Windows::Forms::Form
	{
	public:
		MigrationV3V4()
		{
			InitializeComponent();


      textBoxProtokollDatei->Text = "MigrationsProtokoll";
      textBoxQuelle->Text = nullptr;
      textBoxZielOrdner->Text = nullptr;

      checkBoxUeberschreiben->Checked = true;

      radioButtonDatei->Checked = true;
      radioButtonOrdner->Checked = false;

			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~MigrationV3V4()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::GroupBox^  groupBox1;
  private: System::Windows::Forms::RadioButton^  radioButtonOrdner;
  private: System::Windows::Forms::RadioButton^  radioButtonDatei;
  private: System::Windows::Forms::Button^  buttonAuswahlQuelle;
  private: System::Windows::Forms::TextBox^  textBoxQuelle;
  private: System::Windows::Forms::GroupBox^  groupBox2;
  private: System::Windows::Forms::CheckBox^  checkBoxUeberschreiben;
  private: System::Windows::Forms::Button^  buttonAuswahlZielordner;
  private: System::Windows::Forms::TextBox^  textBoxZielOrdner;

  private: System::Windows::Forms::Button^  buttonTransformieren;
  private: System::Windows::Forms::Button^  buttonAbbrechen;
  private: System::Windows::Forms::TextBox^  textBoxProtokollDatei;
  private: System::Windows::Forms::Label^  label1;


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
      this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
      this->buttonAuswahlQuelle = (gcnew System::Windows::Forms::Button());
      this->textBoxQuelle = (gcnew System::Windows::Forms::TextBox());
      this->radioButtonOrdner = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonDatei = (gcnew System::Windows::Forms::RadioButton());
      this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBoxProtokollDatei = (gcnew System::Windows::Forms::TextBox());
      this->checkBoxUeberschreiben = (gcnew System::Windows::Forms::CheckBox());
      this->buttonAuswahlZielordner = (gcnew System::Windows::Forms::Button());
      this->textBoxZielOrdner = (gcnew System::Windows::Forms::TextBox());
      this->buttonTransformieren = (gcnew System::Windows::Forms::Button());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->groupBox1->SuspendLayout();
      this->groupBox2->SuspendLayout();
      this->SuspendLayout();
      // 
      // groupBox1
      // 
      this->groupBox1->Controls->Add(this->buttonAuswahlQuelle);
      this->groupBox1->Controls->Add(this->textBoxQuelle);
      this->groupBox1->Controls->Add(this->radioButtonOrdner);
      this->groupBox1->Controls->Add(this->radioButtonDatei);
      this->groupBox1->Location = System::Drawing::Point(9, 16);
      this->groupBox1->Name = L"groupBox1";
      this->groupBox1->Size = System::Drawing::Size(361, 81);
      this->groupBox1->TabIndex = 0;
      this->groupBox1->TabStop = false;
      this->groupBox1->Text = L"Quelle";
      // 
      // buttonAuswahlQuelle
      // 
      this->buttonAuswahlQuelle->Location = System::Drawing::Point(280, 46);
      this->buttonAuswahlQuelle->Name = L"buttonAuswahlQuelle";
      this->buttonAuswahlQuelle->Size = System::Drawing::Size(75, 23);
      this->buttonAuswahlQuelle->TabIndex = 3;
      this->buttonAuswahlQuelle->Text = L"Auswahl";
      this->buttonAuswahlQuelle->UseVisualStyleBackColor = true;
      this->buttonAuswahlQuelle->Click += gcnew System::EventHandler(this, &MigrationV3V4::buttonAuswahlQuelle_Click);
      // 
      // textBoxQuelle
      // 
      this->textBoxQuelle->Location = System::Drawing::Point(7, 49);
      this->textBoxQuelle->Name = L"textBoxQuelle";
      this->textBoxQuelle->ReadOnly = true;
      this->textBoxQuelle->Size = System::Drawing::Size(267, 20);
      this->textBoxQuelle->TabIndex = 2;
      // 
      // radioButtonOrdner
      // 
      this->radioButtonOrdner->AutoSize = true;
      this->radioButtonOrdner->Location = System::Drawing::Point(90, 20);
      this->radioButtonOrdner->Name = L"radioButtonOrdner";
      this->radioButtonOrdner->Size = System::Drawing::Size(57, 17);
      this->radioButtonOrdner->TabIndex = 1;
      this->radioButtonOrdner->TabStop = true;
      this->radioButtonOrdner->Text = L"Ordner";
      this->radioButtonOrdner->UseVisualStyleBackColor = true;
      this->radioButtonOrdner->Click += gcnew System::EventHandler(this, &MigrationV3V4::radioButtonOrdner_Click);
      // 
      // radioButtonDatei
      // 
      this->radioButtonDatei->AutoSize = true;
      this->radioButtonDatei->Location = System::Drawing::Point(7, 20);
      this->radioButtonDatei->Name = L"radioButtonDatei";
      this->radioButtonDatei->Size = System::Drawing::Size(50, 17);
      this->radioButtonDatei->TabIndex = 0;
      this->radioButtonDatei->TabStop = true;
      this->radioButtonDatei->Text = L"Datei";
      this->radioButtonDatei->UseVisualStyleBackColor = true;
      this->radioButtonDatei->Click += gcnew System::EventHandler(this, &MigrationV3V4::radioButtonDatei_Click);
      // 
      // groupBox2
      // 
      this->groupBox2->Controls->Add(this->label1);
      this->groupBox2->Controls->Add(this->textBoxProtokollDatei);
      this->groupBox2->Controls->Add(this->checkBoxUeberschreiben);
      this->groupBox2->Controls->Add(this->buttonAuswahlZielordner);
      this->groupBox2->Controls->Add(this->textBoxZielOrdner);
      this->groupBox2->Location = System::Drawing::Point(9, 103);
      this->groupBox2->Name = L"groupBox2";
      this->groupBox2->Size = System::Drawing::Size(361, 115);
      this->groupBox2->TabIndex = 1;
      this->groupBox2->TabStop = false;
      this->groupBox2->Text = L"Ziel";
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(279, 52);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(48, 13);
      this->label1->TabIndex = 4;
      this->label1->Text = L"Protokoll";
      // 
      // textBoxProtokollDatei
      // 
      this->textBoxProtokollDatei->Location = System::Drawing::Point(6, 45);
      this->textBoxProtokollDatei->Name = L"textBoxProtokollDatei";
      this->textBoxProtokollDatei->Size = System::Drawing::Size(267, 20);
      this->textBoxProtokollDatei->TabIndex = 3;
      // 
      // checkBoxUeberschreiben
      // 
      this->checkBoxUeberschreiben->AutoSize = true;
      this->checkBoxUeberschreiben->Location = System::Drawing::Point(4, 82);
      this->checkBoxUeberschreiben->Name = L"checkBoxUeberschreiben";
      this->checkBoxUeberschreiben->Size = System::Drawing::Size(194, 17);
      this->checkBoxUeberschreiben->TabIndex = 2;
      this->checkBoxUeberschreiben->Text = L"Vorhandene Dateien überschreiben";
      this->checkBoxUeberschreiben->UseVisualStyleBackColor = true;
      // 
      // buttonAuswahlZielordner
      // 
      this->buttonAuswahlZielordner->Location = System::Drawing::Point(277, 19);
      this->buttonAuswahlZielordner->Name = L"buttonAuswahlZielordner";
      this->buttonAuswahlZielordner->Size = System::Drawing::Size(75, 23);
      this->buttonAuswahlZielordner->TabIndex = 1;
      this->buttonAuswahlZielordner->Text = L"Ordner";
      this->buttonAuswahlZielordner->UseVisualStyleBackColor = true;
      this->buttonAuswahlZielordner->Click += gcnew System::EventHandler(this, &MigrationV3V4::buttonAuswahlZielordner_Click);
      // 
      // textBoxZielOrdner
      // 
      this->textBoxZielOrdner->Location = System::Drawing::Point(3, 19);
      this->textBoxZielOrdner->Name = L"textBoxZielOrdner";
      this->textBoxZielOrdner->ReadOnly = true;
      this->textBoxZielOrdner->Size = System::Drawing::Size(268, 20);
      this->textBoxZielOrdner->TabIndex = 0;
      // 
      // buttonTransformieren
      // 
      this->buttonTransformieren->Location = System::Drawing::Point(272, 239);
      this->buttonTransformieren->Name = L"buttonTransformieren";
      this->buttonTransformieren->Size = System::Drawing::Size(97, 24);
      this->buttonTransformieren->TabIndex = 2;
      this->buttonTransformieren->Text = L"Transformieren";
      this->buttonTransformieren->UseVisualStyleBackColor = true;
      this->buttonTransformieren->Click += gcnew System::EventHandler(this, &MigrationV3V4::buttonTransformieren_Click);
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->Location = System::Drawing::Point(9, 238);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 24);
      this->buttonAbbrechen->TabIndex = 3;
      this->buttonAbbrechen->Text = L"Abbrechen";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &MigrationV3V4::buttonAbbrechen_Click);
      // 
      // MigrationV3V4
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(381, 274);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->buttonTransformieren);
      this->Controls->Add(this->groupBox2);
      this->Controls->Add(this->groupBox1);
      this->Name = L"MigrationV3V4";
      this->Text = L"Migration XPlanGML 3.0 -> 4.0";
      this->groupBox1->ResumeLayout(false);
      this->groupBox1->PerformLayout();
      this->groupBox2->ResumeLayout(false);
      this->groupBox2->PerformLayout();
      this->ResumeLayout(false);

    }
#pragma endregion
  private: System::Void buttonAuswahlQuelle_Click(System::Object^  sender, System::EventArgs^  e)
{
  Windows::Forms::OpenFileDialog ^ pQuellDateiDialog;
  Windows::Forms::FolderBrowserDialog ^ pQuellFolderDialog;
  Windows::Forms::DialogResult ok;

  if ( radioButtonDatei->Checked == true )
  {
    pQuellDateiDialog->FileName = textBoxQuelle->Text;
    ok = pQuellDateiDialog->ShowDialog();
    if ( ok == Windows::Forms::DialogResult::OK )
    {
      textBoxQuelle->Text = pQuellDateiDialog->FileName;
      if ( textBoxZielOrdner->Text == nullptr )
      {
        int index = textBoxQuelle->Text->LastIndexOf( "\\");
        textBoxZielOrdner->Text = textBoxQuelle->Text->Substring( index );
      }
    }
  }

  if ( radioButtonOrdner->Checked == true )
  {
    pQuellFolderDialog->SelectedPath = textBoxQuelle->Text;
    ok = pQuellFolderDialog->ShowDialog();
    if ( ok == Windows::Forms::DialogResult::OK )
    {
      textBoxQuelle->Text = pQuellFolderDialog->SelectedPath;
      if (  textBoxZielOrdner->Text == nullptr )
        textBoxZielOrdner->Text = textBoxQuelle->Text;
    }
  }

}

private: System::Void buttonAuswahlZielordner_Click(System::Object^  sender, System::EventArgs^  e)
{
  Windows::Forms::FolderBrowserDialog ^ pZielFolderDialog;
  Windows::Forms::DialogResult ok;

  ok = pZielFolderDialog->ShowDialog();
  if ( ok == Windows::Forms::DialogResult::OK )
  {
    textBoxZielOrdner->Text = pZielFolderDialog->SelectedPath;
  }


}

private: System::Void buttonAbbrechen_Click(System::Object^  sender, System::EventArgs^  e)
{

}

private: System::Void buttonTransformieren_Click(System::Object^  sender, System::EventArgs^  e)
{

}

private: System::Void radioButtonDatei_Click(System::Object^  sender, System::EventArgs^  e) 
{
}

private: System::Void radioButtonOrdner_Click(System::Object^  sender, System::EventArgs^  e)
{
}

};
