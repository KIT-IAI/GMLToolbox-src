#pragma once

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Festlegung des Geometrieprüfungsumfangs eines XPlanGML-Modells
	/// </summary>
	public ref class GeometrietestXPlanGML : public System::Windows::Forms::Form
	{
	public:
		GeometrietestXPlanGML( bool checkFlaechenschlussGrob, bool checkFlaechenschlussExakt, bool checkUeberdeckung )
		{
			InitializeComponent();

      m_checkFlaechenschlussGrob  = checkFlaechenschlussGrob;
      m_checkFlaechenschlussExakt = checkFlaechenschlussExakt;
      m_checkUeberdeckung         = checkUeberdeckung;

      if ( m_checkFlaechenschlussGrob )
        checkBoxUeberlappungstest->Checked = true;
      else
        checkBoxUeberlappungstest->Checked = false;

      if ( m_checkFlaechenschlussGrob )
      {
        if ( m_checkFlaechenschlussExakt )
          checkBoxFlaechenberechnung->Checked = true;
        else
          checkBoxFlaechenberechnung->Checked = false;
      }
      else
      {
        m_checkFlaechenschlussExakt = false;
        checkBoxFlaechenberechnung->Checked = false;
        checkBoxFlaechenberechnung->Enabled = false;
      }

      if ( m_checkUeberdeckung )
        checkBoxUeberdeckung->Checked  = true;
      else
        checkBoxUeberdeckung->Checked  = false;
	
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~GeometrietestXPlanGML()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::CheckBox^  checkBoxUeberlappungstest;
  private: System::Windows::Forms::CheckBox^  checkBoxFlaechenberechnung;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::CheckBox^  checkBoxUeberdeckung;

  public:
    bool m_checkFlaechenschlussGrob;   // Topologieprüfung von Flächenschlussobjekten
    bool m_checkFlaechenschlussExakt;  // Flächenschlussprüfung mit Berechnung von Überlappungsflächen
    bool m_checkUeberdeckung;          // Summe der Flächengrößen der Flächenschlussobjekte =
                                       // Flächengröße des Geltungsbereiches

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
      this->checkBoxUeberlappungstest = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxFlaechenberechnung = (gcnew System::Windows::Forms::CheckBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->checkBoxUeberdeckung = (gcnew System::Windows::Forms::CheckBox());
      this->SuspendLayout();
      // 
      // checkBoxUeberlappungstest
      // 
      this->checkBoxUeberlappungstest->AutoSize = true;
      this->checkBoxUeberlappungstest->Location = System::Drawing::Point(13, 23);
      this->checkBoxUeberlappungstest->Name = L"checkBoxUeberlappungstest";
      this->checkBoxUeberlappungstest->Size = System::Drawing::Size(252, 17);
      this->checkBoxUeberlappungstest->TabIndex = 0;
      this->checkBoxUeberlappungstest->Text = L"Test Überlappung  von Flächenschlussobjekten";
      this->checkBoxUeberlappungstest->UseVisualStyleBackColor = true;
      this->checkBoxUeberlappungstest->CheckedChanged += gcnew System::EventHandler(this, &GeometrietestXPlanGML::checkBoxUeberlappungstest_CheckedChanged);
      // 
      // checkBoxFlaechenberechnung
      // 
      this->checkBoxFlaechenberechnung->AutoSize = true;
      this->checkBoxFlaechenberechnung->Location = System::Drawing::Point(13, 47);
      this->checkBoxFlaechenberechnung->Name = L"checkBoxFlaechenberechnung";
      this->checkBoxFlaechenberechnung->Size = System::Drawing::Size(206, 17);
      this->checkBoxFlaechenberechnung->TabIndex = 1;
      this->checkBoxFlaechenberechnung->Text = L"Berechnung der Überlappungsflächen";
      this->checkBoxFlaechenberechnung->UseVisualStyleBackColor = true;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Location = System::Drawing::Point(197, 106);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 2;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &GeometrietestXPlanGML::buttonBeenden_Click);
      // 
      // checkBoxUeberdeckung
      // 
      this->checkBoxUeberdeckung->AutoSize = true;
      this->checkBoxUeberdeckung->Location = System::Drawing::Point(13, 71);
      this->checkBoxUeberdeckung->Name = L"checkBoxUeberdeckung";
      this->checkBoxUeberdeckung->Size = System::Drawing::Size(241, 17);
      this->checkBoxUeberdeckung->TabIndex = 3;
      this->checkBoxUeberdeckung->Text = L"Test der Überdeckung des Geltungsbereichs ";
      this->checkBoxUeberdeckung->UseVisualStyleBackColor = true;
      // 
      // GeometrietestXPlanGML
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(284, 146);
      this->Controls->Add(this->checkBoxUeberdeckung);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->checkBoxFlaechenberechnung);
      this->Controls->Add(this->checkBoxUeberlappungstest);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"GeometrietestXPlanGML";
      this->Text = L"Einstellung Geometrietests XPlanGML";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: CheckBox checkBoxUeberlappungstest geändert              //
///////////////////////////////////////////////////////////////////////////////
private: System::Void checkBoxUeberlappungstest_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  if ( checkBoxUeberlappungstest->Checked == false )
  {
    m_checkFlaechenschlussExakt = false;
    checkBoxFlaechenberechnung->Checked = false;
    checkBoxFlaechenberechnung->Enabled = false;
  }
  else
  {
    checkBoxFlaechenberechnung->Enabled = true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Beenden                                            //
//  Übernimmt die Ergebnisse                                                 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
{
  if ( checkBoxUeberlappungstest->Checked )
    m_checkFlaechenschlussGrob = true;
  else
    m_checkFlaechenschlussGrob = false;

  if ( checkBoxFlaechenberechnung->Checked )
    m_checkFlaechenschlussExakt = true;
  else
    m_checkFlaechenschlussExakt = false;

  if ( checkBoxUeberdeckung->Checked == true )
    m_checkUeberdeckung = true;
  else
    m_checkUeberdeckung = false;

  Close();
}
};

