#pragma once

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Spezifikation der 12 Werte einer monatlichen Zeitreihe
	/// </summary>
	public ref class MonthlyValues : public System::Windows::Forms::Form
	{
	public:
		MonthlyValues( Generic::List<double> ^ vValues )
		{
			InitializeComponent();

      m_vValues = vValues;

      textBoxJanuary->Text = Convert::ToString( vValues[0] );
      textBoxFebruary->Text = Convert::ToString( vValues[1] );
      textBoxMarch->Text = Convert::ToString( vValues[2] );
      textBoxApril->Text = Convert::ToString( vValues[3] );
      textBoxMay->Text = Convert::ToString( vValues[4] );
      textBoxJune->Text = Convert::ToString( vValues[5] );
      textBoxJuly->Text = Convert::ToString( vValues[6] );
      textBoxAugust->Text = Convert::ToString( vValues[7] );
      textBoxSeptember->Text = Convert::ToString( vValues[8] );
      textBoxOctober->Text = Convert::ToString( vValues[9] );
      textBoxNovember->Text = Convert::ToString( vValues[10] );
      textBoxDecember->Text = Convert::ToString( vValues[11] );
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~MonthlyValues()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxJanuary;
  private: System::Windows::Forms::TextBox^  textBoxFebruary;
  private: System::Windows::Forms::TextBox^  textBoxMarch;
  private: System::Windows::Forms::TextBox^  textBoxApril;
  private: System::Windows::Forms::TextBox^  textBoxMay;
  private: System::Windows::Forms::TextBox^  textBoxJune;
  private: System::Windows::Forms::TextBox^  textBoxJuly;
  private: System::Windows::Forms::TextBox^  textBoxAugust;
  private: System::Windows::Forms::TextBox^  textBoxSeptember;
  private: System::Windows::Forms::TextBox^  textBoxOctober;
  private: System::Windows::Forms::TextBox^  textBoxNovember;
  private: System::Windows::Forms::TextBox^  textBoxDecember;
  private: System::Windows::Forms::Button^  buttonCancel;
  private: System::Windows::Forms::Button^  buttonOk;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::Label^  label5;
  private: System::Windows::Forms::Label^  label6;
  private: System::Windows::Forms::Label^  label7;
  private: System::Windows::Forms::Label^  label8;
  private: System::Windows::Forms::Label^  label9;
  private: System::Windows::Forms::Label^  label10;
  private: System::Windows::Forms::Label^  label11;
  private: System::Windows::Forms::Label^  label12;
  private: System::Windows::Forms::TextBox^  textBoxValue;
  private: System::Windows::Forms::Button^  buttonSetzen;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

  public:
   Generic::List<double> ^ m_vValues;  //  Liste der spezifizierten Werte

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->textBoxJanuary = (gcnew System::Windows::Forms::TextBox());
      this->textBoxFebruary = (gcnew System::Windows::Forms::TextBox());
      this->textBoxMarch = (gcnew System::Windows::Forms::TextBox());
      this->textBoxApril = (gcnew System::Windows::Forms::TextBox());
      this->textBoxMay = (gcnew System::Windows::Forms::TextBox());
      this->textBoxJune = (gcnew System::Windows::Forms::TextBox());
      this->textBoxJuly = (gcnew System::Windows::Forms::TextBox());
      this->textBoxAugust = (gcnew System::Windows::Forms::TextBox());
      this->textBoxSeptember = (gcnew System::Windows::Forms::TextBox());
      this->textBoxOctober = (gcnew System::Windows::Forms::TextBox());
      this->textBoxNovember = (gcnew System::Windows::Forms::TextBox());
      this->textBoxDecember = (gcnew System::Windows::Forms::TextBox());
      this->buttonCancel = (gcnew System::Windows::Forms::Button());
      this->buttonOk = (gcnew System::Windows::Forms::Button());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->label5 = (gcnew System::Windows::Forms::Label());
      this->label6 = (gcnew System::Windows::Forms::Label());
      this->label7 = (gcnew System::Windows::Forms::Label());
      this->label8 = (gcnew System::Windows::Forms::Label());
      this->label9 = (gcnew System::Windows::Forms::Label());
      this->label10 = (gcnew System::Windows::Forms::Label());
      this->label11 = (gcnew System::Windows::Forms::Label());
      this->label12 = (gcnew System::Windows::Forms::Label());
      this->textBoxValue = (gcnew System::Windows::Forms::TextBox());
      this->buttonSetzen = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // textBoxJanuary
      // 
      this->textBoxJanuary->Location = System::Drawing::Point(12, 28);
      this->textBoxJanuary->Name = L"textBoxJanuary";
      this->textBoxJanuary->Size = System::Drawing::Size(100, 20);
      this->textBoxJanuary->TabIndex = 0;
      // 
      // textBoxFebruary
      // 
      this->textBoxFebruary->Location = System::Drawing::Point(12, 54);
      this->textBoxFebruary->Name = L"textBoxFebruary";
      this->textBoxFebruary->Size = System::Drawing::Size(100, 20);
      this->textBoxFebruary->TabIndex = 1;
      // 
      // textBoxMarch
      // 
      this->textBoxMarch->Location = System::Drawing::Point(12, 80);
      this->textBoxMarch->Name = L"textBoxMarch";
      this->textBoxMarch->Size = System::Drawing::Size(100, 20);
      this->textBoxMarch->TabIndex = 2;
      // 
      // textBoxApril
      // 
      this->textBoxApril->Location = System::Drawing::Point(12, 106);
      this->textBoxApril->Name = L"textBoxApril";
      this->textBoxApril->Size = System::Drawing::Size(100, 20);
      this->textBoxApril->TabIndex = 3;
      // 
      // textBoxMay
      // 
      this->textBoxMay->Location = System::Drawing::Point(12, 132);
      this->textBoxMay->Name = L"textBoxMay";
      this->textBoxMay->Size = System::Drawing::Size(100, 20);
      this->textBoxMay->TabIndex = 4;
      // 
      // textBoxJune
      // 
      this->textBoxJune->Location = System::Drawing::Point(12, 158);
      this->textBoxJune->Name = L"textBoxJune";
      this->textBoxJune->Size = System::Drawing::Size(100, 20);
      this->textBoxJune->TabIndex = 5;
      // 
      // textBoxJuly
      // 
      this->textBoxJuly->Location = System::Drawing::Point(12, 184);
      this->textBoxJuly->Name = L"textBoxJuly";
      this->textBoxJuly->Size = System::Drawing::Size(100, 20);
      this->textBoxJuly->TabIndex = 6;
      // 
      // textBoxAugust
      // 
      this->textBoxAugust->Location = System::Drawing::Point(12, 210);
      this->textBoxAugust->Name = L"textBoxAugust";
      this->textBoxAugust->Size = System::Drawing::Size(100, 20);
      this->textBoxAugust->TabIndex = 7;
      // 
      // textBoxSeptember
      // 
      this->textBoxSeptember->Location = System::Drawing::Point(12, 236);
      this->textBoxSeptember->Name = L"textBoxSeptember";
      this->textBoxSeptember->Size = System::Drawing::Size(100, 20);
      this->textBoxSeptember->TabIndex = 8;
      // 
      // textBoxOctober
      // 
      this->textBoxOctober->Location = System::Drawing::Point(12, 262);
      this->textBoxOctober->Name = L"textBoxOctober";
      this->textBoxOctober->Size = System::Drawing::Size(100, 20);
      this->textBoxOctober->TabIndex = 9;
      // 
      // textBoxNovember
      // 
      this->textBoxNovember->Location = System::Drawing::Point(12, 288);
      this->textBoxNovember->Name = L"textBoxNovember";
      this->textBoxNovember->Size = System::Drawing::Size(100, 20);
      this->textBoxNovember->TabIndex = 10;
      // 
      // textBoxDecember
      // 
      this->textBoxDecember->Location = System::Drawing::Point(12, 314);
      this->textBoxDecember->Name = L"textBoxDecember";
      this->textBoxDecember->Size = System::Drawing::Size(100, 20);
      this->textBoxDecember->TabIndex = 11;
      // 
      // buttonCancel
      // 
      this->buttonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonCancel->Location = System::Drawing::Point(12, 389);
      this->buttonCancel->Name = L"buttonCancel";
      this->buttonCancel->Size = System::Drawing::Size(75, 23);
      this->buttonCancel->TabIndex = 12;
      this->buttonCancel->Text = L"Cencel";
      this->buttonCancel->UseVisualStyleBackColor = true;
      // 
      // buttonOk
      // 
      this->buttonOk->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonOk->Location = System::Drawing::Point(122, 389);
      this->buttonOk->Name = L"buttonOk";
      this->buttonOk->Size = System::Drawing::Size(75, 23);
      this->buttonOk->TabIndex = 13;
      this->buttonOk->Text = L"Ok";
      this->buttonOk->UseVisualStyleBackColor = true;
      this->buttonOk->Click += gcnew System::EventHandler(this, &MonthlyValues::buttonOk_Click);
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(119, 34);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(44, 13);
      this->label1->TabIndex = 14;
      this->label1->Text = L"January";
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(119, 57);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(48, 13);
      this->label2->TabIndex = 15;
      this->label2->Text = L"February";
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(119, 87);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(37, 13);
      this->label3->TabIndex = 16;
      this->label3->Text = L"March";
      // 
      // label4
      // 
      this->label4->AutoSize = true;
      this->label4->Location = System::Drawing::Point(119, 109);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(27, 13);
      this->label4->TabIndex = 17;
      this->label4->Text = L"April";
      // 
      // label5
      // 
      this->label5->AutoSize = true;
      this->label5->Location = System::Drawing::Point(119, 135);
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size(27, 13);
      this->label5->TabIndex = 18;
      this->label5->Text = L"May";
      // 
      // label6
      // 
      this->label6->AutoSize = true;
      this->label6->Location = System::Drawing::Point(119, 161);
      this->label6->Name = L"label6";
      this->label6->Size = System::Drawing::Size(30, 13);
      this->label6->TabIndex = 19;
      this->label6->Text = L"June";
      // 
      // label7
      // 
      this->label7->AutoSize = true;
      this->label7->Location = System::Drawing::Point(119, 187);
      this->label7->Name = L"label7";
      this->label7->Size = System::Drawing::Size(25, 13);
      this->label7->TabIndex = 20;
      this->label7->Text = L"July";
      // 
      // label8
      // 
      this->label8->AutoSize = true;
      this->label8->Location = System::Drawing::Point(119, 213);
      this->label8->Name = L"label8";
      this->label8->Size = System::Drawing::Size(40, 13);
      this->label8->TabIndex = 21;
      this->label8->Text = L"August";
      // 
      // label9
      // 
      this->label9->AutoSize = true;
      this->label9->Location = System::Drawing::Point(119, 236);
      this->label9->Name = L"label9";
      this->label9->Size = System::Drawing::Size(58, 13);
      this->label9->TabIndex = 22;
      this->label9->Text = L"Spetember";
      // 
      // label10
      // 
      this->label10->AutoSize = true;
      this->label10->Location = System::Drawing::Point(119, 265);
      this->label10->Name = L"label10";
      this->label10->Size = System::Drawing::Size(45, 13);
      this->label10->TabIndex = 23;
      this->label10->Text = L"October";
      // 
      // label11
      // 
      this->label11->AutoSize = true;
      this->label11->Location = System::Drawing::Point(119, 291);
      this->label11->Name = L"label11";
      this->label11->Size = System::Drawing::Size(56, 13);
      this->label11->TabIndex = 24;
      this->label11->Text = L"November";
      // 
      // label12
      // 
      this->label12->AutoSize = true;
      this->label12->Location = System::Drawing::Point(119, 317);
      this->label12->Name = L"label12";
      this->label12->Size = System::Drawing::Size(56, 13);
      this->label12->TabIndex = 25;
      this->label12->Text = L"December";
      // 
      // textBoxValue
      // 
      this->textBoxValue->Location = System::Drawing::Point(12, 351);
      this->textBoxValue->Name = L"textBoxValue";
      this->textBoxValue->Size = System::Drawing::Size(100, 20);
      this->textBoxValue->TabIndex = 26;
      // 
      // buttonSetzen
      // 
      this->buttonSetzen->Location = System::Drawing::Point(118, 351);
      this->buttonSetzen->Name = L"buttonSetzen";
      this->buttonSetzen->Size = System::Drawing::Size(59, 20);
      this->buttonSetzen->TabIndex = 27;
      this->buttonSetzen->Text = L"Set";
      this->buttonSetzen->UseVisualStyleBackColor = true;
      this->buttonSetzen->Click += gcnew System::EventHandler(this, &MonthlyValues::buttonSetzen_Click);
      // 
      // MonthlyValues
      // 
      this->AcceptButton = this->buttonOk;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->buttonCancel;
      this->ClientSize = System::Drawing::Size(205, 442);
      this->Controls->Add(this->buttonSetzen);
      this->Controls->Add(this->textBoxValue);
      this->Controls->Add(this->label12);
      this->Controls->Add(this->label11);
      this->Controls->Add(this->label10);
      this->Controls->Add(this->label9);
      this->Controls->Add(this->label8);
      this->Controls->Add(this->label7);
      this->Controls->Add(this->label6);
      this->Controls->Add(this->label5);
      this->Controls->Add(this->label4);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->buttonOk);
      this->Controls->Add(this->buttonCancel);
      this->Controls->Add(this->textBoxDecember);
      this->Controls->Add(this->textBoxNovember);
      this->Controls->Add(this->textBoxOctober);
      this->Controls->Add(this->textBoxSeptember);
      this->Controls->Add(this->textBoxAugust);
      this->Controls->Add(this->textBoxJuly);
      this->Controls->Add(this->textBoxJune);
      this->Controls->Add(this->textBoxMay);
      this->Controls->Add(this->textBoxApril);
      this->Controls->Add(this->textBoxMarch);
      this->Controls->Add(this->textBoxFebruary);
      this->Controls->Add(this->textBoxJanuary);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"MonthlyValues";
      this->Text = L"Monthly Time Series";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Ok": Übernimmt die spezifizierten Werte und       //
//  schließt den Dialog                                                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonOk_Click(System::Object^  sender, System::EventArgs^  e) 
           {
             m_vValues[0] = Convert::ToDouble( textBoxJanuary->Text );
             m_vValues[1] = Convert::ToDouble( textBoxFebruary->Text );
             m_vValues[2] = Convert::ToDouble( textBoxMarch->Text );
             m_vValues[3] = Convert::ToDouble( textBoxApril->Text );
             m_vValues[4] = Convert::ToDouble( textBoxMay->Text );
             m_vValues[5] = Convert::ToDouble( textBoxJune->Text );
             m_vValues[6] = Convert::ToDouble( textBoxJuly->Text );
             m_vValues[7] = Convert::ToDouble( textBoxAugust->Text );
             m_vValues[8] = Convert::ToDouble( textBoxSeptember->Text );
             m_vValues[9] = Convert::ToDouble( textBoxOctober->Text );
             m_vValues[10] = Convert::ToDouble( textBoxNovember->Text );
             m_vValues[11] = Convert::ToDouble( textBoxDecember->Text );
           }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Set": Setzt alle 12 Monatswerte auf den spez.     //
//  Wert                                                                     //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonSetzen_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           textBoxJanuary->Text = textBoxValue->Text;
           textBoxFebruary->Text = textBoxValue->Text;
           textBoxMarch->Text = textBoxValue->Text;
           textBoxApril->Text = textBoxValue->Text;
           textBoxMay->Text = textBoxValue->Text;
           textBoxJune->Text = textBoxValue->Text;
           textBoxJuly->Text = textBoxValue->Text;
           textBoxAugust->Text = textBoxValue->Text;
           textBoxSeptember->Text = textBoxValue->Text;
           textBoxOctober->Text = textBoxValue->Text;
           textBoxNovember->Text = textBoxValue->Text;
           textBoxDecember->Text = textBoxValue->Text;
         }
};
