#pragma once

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
  using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Definition von Schlüsselwörtern für die Speicherung von Zeitreihen-Mengen im Repository
	/// </summary>
	public ref class KeyWords : public System::Windows::Forms::Form
	{
	public:
		KeyWords( List<String^> ^ keyWordsExisting, List<String^> ^ keyWordsSelected )
		{
			List<String^> ^ keyWordsNew = gcnew List<String^>;
			int             i;

			InitializeComponent();
			
      for ( i = 0; i < keyWordsExisting->Count; ++i )
        listBoxKeyWordsAvailable->Items->Add( keyWordsExisting[i] );

			for ( i = 0; i < keyWordsSelected->Count; ++i )
			{
				if ( keyWordsExisting->Contains (  keyWordsSelected[i] ) )
					listBoxKeyWordsSelected->Items->Add ( keyWordsSelected[i] );
				else
					keyWordsNew->Add ( keyWordsSelected[i] );
			}


			for ( i = 0; i < keyWordsNew->Count; ++i )
			{
				if ( i < keyWordsNew->Count - 1 )
					textBoxKeyWordsNew->Text = String::Concat( textBoxKeyWordsNew->Text, keyWordsNew[i], "; " );
				else
					textBoxKeyWordsNew->Text = String::Concat( textBoxKeyWordsNew->Text, keyWordsNew[i] );
			}
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~KeyWords()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::ListBox^  listBoxKeyWordsAvailable;
  protected: 
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::ListBox^  listBoxKeyWordsSelected;
  private: System::Windows::Forms::Button^  buttonSelect;
  private: System::Windows::Forms::Button^  buttonDeSelect;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::TextBox^  textBoxKeyWordsNew;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::Button^  buttonUebernehmen;
  private: System::Windows::Forms::Button^  buttonCancel;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;
    List<String^>                     ^ m_KeyWordsSelected;  //  Liste der ausgewählten key words

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->listBoxKeyWordsAvailable = (gcnew System::Windows::Forms::ListBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->listBoxKeyWordsSelected = (gcnew System::Windows::Forms::ListBox());
      this->buttonSelect = (gcnew System::Windows::Forms::Button());
      this->buttonDeSelect = (gcnew System::Windows::Forms::Button());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->textBoxKeyWordsNew = (gcnew System::Windows::Forms::TextBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->buttonUebernehmen = (gcnew System::Windows::Forms::Button());
      this->buttonCancel = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // listBoxKeyWordsAvailable
      // 
      this->listBoxKeyWordsAvailable->FormattingEnabled = true;
      this->listBoxKeyWordsAvailable->ItemHeight = 16;
      this->listBoxKeyWordsAvailable->Location = System::Drawing::Point(17, 16);
      this->listBoxKeyWordsAvailable->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->listBoxKeyWordsAvailable->Name = L"listBoxKeyWordsAvailable";
      this->listBoxKeyWordsAvailable->Size = System::Drawing::Size(243, 260);
      this->listBoxKeyWordsAvailable->TabIndex = 0;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(17, 283);
      this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(108, 17);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Used key words";
      // 
      // listBoxKeyWordsSelected
      // 
      this->listBoxKeyWordsSelected->FormattingEnabled = true;
      this->listBoxKeyWordsSelected->ItemHeight = 16;
      this->listBoxKeyWordsSelected->Location = System::Drawing::Point(364, 16);
      this->listBoxKeyWordsSelected->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->listBoxKeyWordsSelected->Name = L"listBoxKeyWordsSelected";
      this->listBoxKeyWordsSelected->Size = System::Drawing::Size(243, 260);
      this->listBoxKeyWordsSelected->TabIndex = 2;
      // 
      // buttonSelect
      // 
      this->buttonSelect->Location = System::Drawing::Point(271, 53);
      this->buttonSelect->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->buttonSelect->Name = L"buttonSelect";
      this->buttonSelect->Size = System::Drawing::Size(85, 28);
      this->buttonSelect->TabIndex = 3;
      this->buttonSelect->Text = L"-->";
      this->buttonSelect->UseVisualStyleBackColor = true;
      this->buttonSelect->Click += gcnew System::EventHandler(this, &KeyWords::buttonSelect_Click);
      // 
      // buttonDeSelect
      // 
      this->buttonDeSelect->Location = System::Drawing::Point(271, 121);
      this->buttonDeSelect->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->buttonDeSelect->Name = L"buttonDeSelect";
      this->buttonDeSelect->Size = System::Drawing::Size(85, 28);
      this->buttonDeSelect->TabIndex = 4;
      this->buttonDeSelect->Text = L"<--";
      this->buttonDeSelect->UseVisualStyleBackColor = true;
      this->buttonDeSelect->Click += gcnew System::EventHandler(this, &KeyWords::buttonDeSelect_Click);
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(364, 288);
      this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(130, 17);
      this->label2->TabIndex = 5;
      this->label2->Text = L"Selected key words";
      // 
      // textBoxKeyWordsNew
      // 
      this->textBoxKeyWordsNew->Location = System::Drawing::Point(21, 319);
      this->textBoxKeyWordsNew->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->textBoxKeyWordsNew->Name = L"textBoxKeyWordsNew";
      this->textBoxKeyWordsNew->Size = System::Drawing::Size(585, 22);
      this->textBoxKeyWordsNew->TabIndex = 6;
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(21, 352);
      this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(217, 17);
      this->label3->TabIndex = 7;
      this->label3->Text = L"New key words (separated by \";\")";
      // 
      // buttonUebernehmen
      // 
      this->buttonUebernehmen->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonUebernehmen->Location = System::Drawing::Point(507, 412);
      this->buttonUebernehmen->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->buttonUebernehmen->Name = L"buttonUebernehmen";
      this->buttonUebernehmen->Size = System::Drawing::Size(100, 28);
      this->buttonUebernehmen->TabIndex = 8;
      this->buttonUebernehmen->Text = L"Save";
      this->buttonUebernehmen->UseVisualStyleBackColor = true;
      this->buttonUebernehmen->Click += gcnew System::EventHandler(this, &KeyWords::buttonUebernehmen_Click);
      // 
      // buttonCancel
      // 
      this->buttonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonCancel->Location = System::Drawing::Point(17, 412);
      this->buttonCancel->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->buttonCancel->Name = L"buttonCancel";
      this->buttonCancel->Size = System::Drawing::Size(100, 28);
      this->buttonCancel->TabIndex = 9;
      this->buttonCancel->Text = L"Cancel";
      this->buttonCancel->UseVisualStyleBackColor = true;
      // 
      // KeyWords
      // 
      this->AcceptButton = this->buttonUebernehmen;
      this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->buttonCancel;
      this->ClientSize = System::Drawing::Size(639, 470);
      this->Controls->Add(this->buttonCancel);
      this->Controls->Add(this->buttonUebernehmen);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->textBoxKeyWordsNew);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->buttonDeSelect);
      this->Controls->Add(this->buttonSelect);
      this->Controls->Add(this->listBoxKeyWordsSelected);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->listBoxKeyWordsAvailable);
      this->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"KeyWords";
      this->Text = L"KeyWords";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "-->": Auwahl eines key word aus der Liste schon   //
//  vorhandener key words                                                    //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonSelect_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           if ( listBoxKeyWordsAvailable->SelectedIndex == -1 )
             return;

           String ^ keyWord = dynamic_cast<String^>(listBoxKeyWordsAvailable->SelectedItem);
           listBoxKeyWordsSelected->Items->Add( keyWord );
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "<--": Entfernen eines key word aus der Liste      //
//  der ausgewählten key words                                               //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonDeSelect_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           if ( listBoxKeyWordsSelected->SelectedIndex == -1 )
             return;

           int index = listBoxKeyWordsSelected->SelectedIndex;
           listBoxKeyWordsSelected->Items->RemoveAt( index );
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Ok": Übernimmt die ausgewählten key words und     //
//  beendet den Dialog                                                       //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonUebernehmen_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           m_KeyWordsSelected = gcnew List<String ^>;

           for ( int i = 0; i < listBoxKeyWordsSelected->Items->Count; ++i )
             m_KeyWordsSelected->Add( dynamic_cast<String^>(listBoxKeyWordsSelected->Items[i]));

           if ( textBoxKeyWordsNew->Text->CompareTo( "" ) != 0 )
           {
             cli::array<String^> ^ delimiter = { ";" };
             cli::array<String^> ^ split;

             split = textBoxKeyWordsNew->Text->Split ( delimiter, StringSplitOptions::None );
             for ( int i = 0; i < split->Length; ++i )
             { 
               String ^ keyWord = split[i]->Trim();
               m_KeyWordsSelected->Add ( keyWord );
             }
           }
         }
 
///////////////////////////////////////////////////////////////////////////////
//  Interface: Liefert die ausgewählten key words                            //
///////////////////////////////////////////////////////////////////////////////
public: List<String^> ^ getKeyWordsSelected() {  return m_KeyWordsSelected; }
};
