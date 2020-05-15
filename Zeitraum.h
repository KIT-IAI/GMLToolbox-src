#pragma once

namespace XPlanGMLToolbox {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für Zeitraum
	/// </summary>
	public ref class Zeitraum : public System::Windows::Forms::Form
	{
	public:
		Zeitraum(void)
		{
			InitializeComponent();

      for ( unsigned int i = 0; i < 12; i++ )
        comboBoxMonatVon->Items->Add ( Convert::ToString( i+1 ) );

      comboBoxMonatBis->Enabled = false;
      comboBoxTagVon->Enabled = false;
      comboBoxTagBis->Enabled = false;			

      monatVon   = 0;
      monatBis   = 0;
      tagVon     = 0;
      tagBis     = 0;
      anzTageVon = 0;
      anzTageBis = 0;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Zeitraum()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::ComboBox^  comboBoxMonatVon;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::ComboBox^  comboBoxMonatBis;

  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::ComboBox^  comboBoxTagVon;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::ComboBox^  comboBoxTagBis;
  private: System::Windows::Forms::Button^  buttonCancel;
  private: System::Windows::Forms::Button^  buttonUebernehmen;

  public:
     int monatVon, monatBis, tagVon, tagBis, anzTageVon, anzTageBis;

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
      this->comboBoxMonatVon = (gcnew System::Windows::Forms::ComboBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->comboBoxMonatBis = (gcnew System::Windows::Forms::ComboBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->comboBoxTagVon = (gcnew System::Windows::Forms::ComboBox());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->comboBoxTagBis = (gcnew System::Windows::Forms::ComboBox());
      this->buttonCancel = (gcnew System::Windows::Forms::Button());
      this->buttonUebernehmen = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // comboBoxMonatVon
      // 
      this->comboBoxMonatVon->FormattingEnabled = true;
      this->comboBoxMonatVon->Location = System::Drawing::Point(52, 36);
      this->comboBoxMonatVon->Name = L"comboBoxMonatVon";
      this->comboBoxMonatVon->Size = System::Drawing::Size(55, 21);
      this->comboBoxMonatVon->TabIndex = 3;
      this->comboBoxMonatVon->SelectedIndexChanged += gcnew System::EventHandler(this, &Zeitraum::comboBoxMonatVon_SelectedIndexChanged);
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(9, 40);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(37, 13);
      this->label2->TabIndex = 4;
      this->label2->Text = L"Monat";
      // 
      // comboBoxMonatBis
      // 
      this->comboBoxMonatBis->FormattingEnabled = true;
      this->comboBoxMonatBis->Location = System::Drawing::Point(151, 36);
      this->comboBoxMonatBis->Name = L"comboBoxMonatBis";
      this->comboBoxMonatBis->Size = System::Drawing::Size(55, 21);
      this->comboBoxMonatBis->TabIndex = 5;
      this->comboBoxMonatBis->SelectedIndexChanged += gcnew System::EventHandler(this, &Zeitraum::comboBoxMonatBis_SelectedIndexChanged);
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(117, 40);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(20, 13);
      this->label1->TabIndex = 6;
      this->label1->Text = L"bis";
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(13, 85);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(26, 13);
      this->label3->TabIndex = 7;
      this->label3->Text = L"Tag";
      // 
      // comboBoxTagVon
      // 
      this->comboBoxTagVon->FormattingEnabled = true;
      this->comboBoxTagVon->Location = System::Drawing::Point(52, 81);
      this->comboBoxTagVon->Name = L"comboBoxTagVon";
      this->comboBoxTagVon->Size = System::Drawing::Size(55, 21);
      this->comboBoxTagVon->TabIndex = 8;
      this->comboBoxTagVon->SelectedIndexChanged += gcnew System::EventHandler(this, &Zeitraum::comboBoxTagVon_SelectedIndexChanged);
      // 
      // label4
      // 
      this->label4->AutoSize = true;
      this->label4->Location = System::Drawing::Point(117, 85);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(20, 13);
      this->label4->TabIndex = 9;
      this->label4->Text = L"bis";
      // 
      // comboBoxTagBis
      // 
      this->comboBoxTagBis->FormattingEnabled = true;
      this->comboBoxTagBis->Location = System::Drawing::Point(151, 81);
      this->comboBoxTagBis->Name = L"comboBoxTagBis";
      this->comboBoxTagBis->Size = System::Drawing::Size(55, 21);
      this->comboBoxTagBis->TabIndex = 10;
      this->comboBoxTagBis->SelectedIndexChanged += gcnew System::EventHandler(this, &Zeitraum::comboBoxTagBis_SelectedIndexChanged);
      // 
      // buttonCancel
      // 
      this->buttonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonCancel->Location = System::Drawing::Point(12, 173);
      this->buttonCancel->Name = L"buttonCancel";
      this->buttonCancel->Size = System::Drawing::Size(75, 23);
      this->buttonCancel->TabIndex = 11;
      this->buttonCancel->Text = L"Abbrechen";
      this->buttonCancel->UseVisualStyleBackColor = true;
      // 
      // buttonUebernehmen
      // 
      this->buttonUebernehmen->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonUebernehmen->Location = System::Drawing::Point(123, 173);
      this->buttonUebernehmen->Name = L"buttonUebernehmen";
      this->buttonUebernehmen->Size = System::Drawing::Size(83, 23);
      this->buttonUebernehmen->TabIndex = 12;
      this->buttonUebernehmen->Text = L"Übernehmen";
      this->buttonUebernehmen->UseVisualStyleBackColor = true;
      // 
      // Zeitraum
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(226, 216);
      this->Controls->Add(this->buttonUebernehmen);
      this->Controls->Add(this->buttonCancel);
      this->Controls->Add(this->comboBoxTagBis);
      this->Controls->Add(this->label4);
      this->Controls->Add(this->comboBoxTagVon);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->comboBoxMonatBis);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->comboBoxMonatVon);
      this->Name = L"Zeitraum";
      this->Text = L"Zeitraum";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  private: System::Void comboBoxMonatVon_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
           {
             int i;
             
             monatVon = comboBoxMonatVon->SelectedIndex + 1;
             
             comboBoxMonatBis->Enabled = true;
             for ( i = monatVon; i <= 12; i++ )
               comboBoxMonatBis->Items->Add( Convert::ToString( i  ) );
             comboBoxMonatBis->SelectedIndex = 0;

             if ( monatVon == 1 || monatVon == 3 || monatVon == 5 || monatVon == 7 || monatVon == 8 || monatVon == 10 || monatVon == 12 )
               anzTageVon = 31;
             else
             if ( monatVon == 4 || monatVon == 6 || monatVon == 9 || monatVon == 11 )
               anzTageVon = 30;
             else
               anzTageVon = 29;
             
           }

private: System::Void comboBoxMonatBis_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
         {
           monatBis =  Convert::ToInt32 ( comboBoxMonatBis->SelectedItem );
           if ( monatBis == monatVon )
           {
             anzTageBis = anzTageVon;
             comboBoxTagBis->Enabled = False;
             comboBoxTagVon->Items->Clear();
             comboBoxTagVon->Items->Add( ( "Ganzer Monat" ) );
             for ( i = 1; i <= anzTageVon; i++ )
               comboBoxTagVon->Items->Add( ( Convert::ToString( i ) ) );
             comboBoxTagVon->SelectedIndex = 0;
           }
           else
           {
             comboBoxTagBis->Enabled = True;
             comboBoxTagVon->Items->Clear();
             for ( i = 1; i <= anzTageVon; i++ )
               comboBoxTagVon->Items->Add( ( Convert::ToString( i ) ) );
             comboBoxTagVon->SelectedIndex = 0;


           }
           
           
         }
private: System::Void comboBoxTagVon_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
         {
           int i;
           tagVon = comboBoxTagVon->SelectedIndex;
           if ( tagVon > 0 )
           {
             comboBoxTagBis->Enabled = true;
             comboBoxTagBis->Items->Clear();
             if ( monatVon == monatBis )
             {
               anzTageBis = anzTageVon;
               for ( i = tagVon; i <= anzTageBis; i++ )
                 comboBoxTagBis->Items->Add( ( Convert::ToString( i ) ) );
               comboBoxTagBis->SelectedIndex = 0;
             }
             else
             {
               if ( monatBis == 1 || monatBis == 3 || monatBis == 5 || monatBis == 7 || monatBis == 8 || monatBis == 10 || monatBis == 12 )
                 anzTageBis = 31;
               else
                 if ( monatBis == 4 || monatBis == 6 || monatBis == 9 || monatBis == 11 )
                   anzTageBis = 30;
                 else
                   anzTageBis = 29;
               for ( i = 1; i <= anzTageBis; i++ )
                 comboBoxTagBis->Items->Add( ( Convert::ToString( i ) ) );
               comboBoxTagBis->SelectedIndex = anzTageBis - 1;
             }
           }
           else
           {
             comboBoxTagBis->Enabled = false;
             tagBis = 0;
           }
         }
private: System::Void comboBoxTagBis_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
         {
           tagBis =  Convert::ToInt32( comboBoxTagBis->SelectedItem ); 
         }
};
}
