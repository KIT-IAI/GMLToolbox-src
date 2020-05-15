#pragma once



	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Einstellung der speziellen semantischen Konformitätsprüfungen für XPlanGML
	/// </summary>
	public ref class SemantiktestXPlanGML : public System::Windows::Forms::Form
	{
	public:
		SemantiktestXPlanGML( bool checkBackwardReferences, bool checkUOM  )
		{
			InitializeComponent();

      m_checkBackwardReferences = checkBackwardReferences;
      m_checkUOM                = checkUOM;

      if ( m_checkBackwardReferences )
        checkBoxBachwardReferences->Checked = true;
      else
        checkBoxBachwardReferences->Checked = false;			

      if ( m_checkUOM )
        checkBoxUOM->Checked = true;
      else
        checkBoxUOM->Checked = false;			
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~SemantiktestXPlanGML()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::CheckBox^  checkBoxBachwardReferences;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::CheckBox^  checkBoxUOM;

  public: 
    bool m_checkBackwardReferences;   //  Überprüfung von Rückwärts-Referenzen
    bool m_checkUOM;                  //  Überprüfung vom uom-Identifiern

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
      this->checkBoxBachwardReferences = (gcnew System::Windows::Forms::CheckBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->checkBoxUOM = (gcnew System::Windows::Forms::CheckBox());
      this->SuspendLayout();
      // 
      // checkBoxBachwardReferences
      // 
      this->checkBoxBachwardReferences->AutoSize = true;
      this->checkBoxBachwardReferences->Location = System::Drawing::Point(13, 23);
      this->checkBoxBachwardReferences->Name = L"checkBoxBachwardReferences";
      this->checkBoxBachwardReferences->Size = System::Drawing::Size(215, 17);
      this->checkBoxBachwardReferences->TabIndex = 0;
      this->checkBoxBachwardReferences->Text = L"Überprüfung der Rückwärts-Referenzen";
      this->checkBoxBachwardReferences->UseVisualStyleBackColor = true;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Location = System::Drawing::Point(197, 87);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 1;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &SemantiktestXPlanGML::buttonBeenden_Click);
      // 
      // checkBoxUOM
      // 
      this->checkBoxUOM->AutoSize = true;
      this->checkBoxUOM->Location = System::Drawing::Point(13, 47);
      this->checkBoxUOM->Name = L"checkBoxUOM";
      this->checkBoxUOM->Size = System::Drawing::Size(191, 17);
      this->checkBoxUOM->TabIndex = 2;
      this->checkBoxUOM->Text = L"Überprüfung Units of Measurement";
      this->checkBoxUOM->UseVisualStyleBackColor = true;
      // 
      // SemantiktestXPlanGML
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(284, 136);
      this->Controls->Add(this->checkBoxUOM);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->checkBoxBachwardReferences);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"SemantiktestXPlanGML";
      this->Text = L"Einstellung Semantiktests XPlanGML";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"                                          //
//  Übernimmt die Einstellungen und beendet den Dialog                       //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
           {
             if ( checkBoxBachwardReferences->Checked )
               m_checkBackwardReferences = true;
             else
               m_checkBackwardReferences = false;

             if ( checkBoxUOM->Checked )
               m_checkUOM = true;
             else
               m_checkUOM = false;
             Close();
           }
  };
