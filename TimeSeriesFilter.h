#pragma once

#include "TimeSeriesRepository.h"
#include "Convert.h"

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
  using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Spezifikation zweier Listen von Schlüsselwörtern und Objekt-Ids, die zur Filterung der Zeitreihen-Mengen
  /// in Repository verwendet werden.
	/// </summary>
	public ref class TimeSeriesFilter : public System::Windows::Forms::Form
	{
  public:
    enum class Combination { And, Or  };

	public:
		TimeSeriesFilter( TimeSeriesRepository * pRepository, List<String^> ^ keyWordsList, 
                       List<String^> ^ objectIdList )
		{
			InitializeComponent();

      m_pRepository          = pRepository;
      m_keyWordsList         = keyWordsList;
      m_objectIdList         = objectIdList;
      m_keyWordsSelectedList = nullptr;
      m_objectIdSelectedList = nullptr;

      comboBoxKeyWordsCombination->SelectedIndex = 0;
      comboBoxObjectIdsCombination->SelectedIndex = 1;

      initialize();			
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~TimeSeriesFilter()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::ListBox^  listBoxKeyWords;
  private: System::Windows::Forms::ListBox^  listBoxKeyWordsSelected;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Button^  buttonSelectKeyWord;
  private: System::Windows::Forms::Button^  buttonDeSelectKeyWord;
  private: System::Windows::Forms::ListBox^  listBoxObjectIds;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::Button^  buttonCancel;
  private: System::Windows::Forms::Button^  buttonOk;
  private: System::Windows::Forms::Button^  buttonSelectObjectId;
  private: System::Windows::Forms::Button^  buttonDeSelectObjectId;
  private: System::Windows::Forms::ListBox^  listBoxObjectIdsSelected;
  private: System::Windows::Forms::ComboBox^  comboBoxKeyWordsCombination;
  private: System::Windows::Forms::ComboBox^  comboBoxObjectIdsCombination;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

    TimeSeriesRepository * m_pRepository;         // Repository von Zeitreihen-Mengen
    List<String^>       ^ m_keyWordsList;         // Liste der verwendeten Schlüsselwörter bei Aufruf des Dialogs
    List<String^>       ^ m_objectIdList;         // Liste der verwendeten Objekt-Ids bei Aufruf des Dialogs
    List<String^>       ^ m_keyWordsSelectedList; // Liste der Schlüsselwörter beim Beenden des Dialogs
    List<String^>       ^ m_objectIdSelectedList; // Liste der Objekt-Ids beim Beenden des Dialogs
    Combination           m_keyWordCombination;   // Verknüpfungs-Operator der Schlüsselwörter
    Combination           m_objectIdCombination;  // Verknüpfungs-Operator der Objekt-Ids

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->listBoxKeyWords = (gcnew System::Windows::Forms::ListBox());
      this->listBoxKeyWordsSelected = (gcnew System::Windows::Forms::ListBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->buttonSelectKeyWord = (gcnew System::Windows::Forms::Button());
      this->buttonDeSelectKeyWord = (gcnew System::Windows::Forms::Button());
      this->listBoxObjectIds = (gcnew System::Windows::Forms::ListBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->buttonCancel = (gcnew System::Windows::Forms::Button());
      this->buttonOk = (gcnew System::Windows::Forms::Button());
      this->buttonSelectObjectId = (gcnew System::Windows::Forms::Button());
      this->buttonDeSelectObjectId = (gcnew System::Windows::Forms::Button());
      this->listBoxObjectIdsSelected = (gcnew System::Windows::Forms::ListBox());
      this->comboBoxKeyWordsCombination = (gcnew System::Windows::Forms::ComboBox());
      this->comboBoxObjectIdsCombination = (gcnew System::Windows::Forms::ComboBox());
      this->SuspendLayout();
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->label1->ForeColor = System::Drawing::Color::Red;
      this->label1->Location = System::Drawing::Point(12, 22);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(100, 16);
      this->label1->TabIndex = 0;
      this->label1->Text = L"All key words";
      // 
      // listBoxKeyWords
      // 
      this->listBoxKeyWords->FormattingEnabled = true;
      this->listBoxKeyWords->Location = System::Drawing::Point(12, 38);
      this->listBoxKeyWords->Name = L"listBoxKeyWords";
      this->listBoxKeyWords->Size = System::Drawing::Size(270, 160);
      this->listBoxKeyWords->TabIndex = 1;
      // 
      // listBoxKeyWordsSelected
      // 
      this->listBoxKeyWordsSelected->FormattingEnabled = true;
      this->listBoxKeyWordsSelected->Location = System::Drawing::Point(347, 38);
      this->listBoxKeyWordsSelected->Name = L"listBoxKeyWordsSelected";
      this->listBoxKeyWordsSelected->Size = System::Drawing::Size(270, 160);
      this->listBoxKeyWordsSelected->TabIndex = 2;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->label2->ForeColor = System::Drawing::Color::Red;
      this->label2->Location = System::Drawing::Point(346, 22);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(203, 13);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Selected key words    Combination";
      // 
      // buttonSelectKeyWord
      // 
      this->buttonSelectKeyWord->Location = System::Drawing::Point(288, 69);
      this->buttonSelectKeyWord->Name = L"buttonSelectKeyWord";
      this->buttonSelectKeyWord->Size = System::Drawing::Size(52, 23);
      this->buttonSelectKeyWord->TabIndex = 4;
      this->buttonSelectKeyWord->Text = L"-->";
      this->buttonSelectKeyWord->UseVisualStyleBackColor = true;
      this->buttonSelectKeyWord->Click += gcnew System::EventHandler(this, &TimeSeriesFilter::buttonSelectKeyWord_Click);
      // 
      // buttonDeSelectKeyWord
      // 
      this->buttonDeSelectKeyWord->Location = System::Drawing::Point(288, 112);
      this->buttonDeSelectKeyWord->Name = L"buttonDeSelectKeyWord";
      this->buttonDeSelectKeyWord->Size = System::Drawing::Size(52, 23);
      this->buttonDeSelectKeyWord->TabIndex = 5;
      this->buttonDeSelectKeyWord->Text = L"<--";
      this->buttonDeSelectKeyWord->UseVisualStyleBackColor = true;
      this->buttonDeSelectKeyWord->Click += gcnew System::EventHandler(this, &TimeSeriesFilter::buttonDeSelectKeyWord_Click);
      // 
      // listBoxObjectIds
      // 
      this->listBoxObjectIds->FormattingEnabled = true;
      this->listBoxObjectIds->Location = System::Drawing::Point(12, 231);
      this->listBoxObjectIds->Name = L"listBoxObjectIds";
      this->listBoxObjectIds->Size = System::Drawing::Size(270, 160);
      this->listBoxObjectIds->TabIndex = 6;
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->label3->ForeColor = System::Drawing::Color::Red;
      this->label3->Location = System::Drawing::Point(12, 212);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(98, 16);
      this->label3->TabIndex = 7;
      this->label3->Text = L"All object Ids";
      // 
      // label4
      // 
      this->label4->AutoSize = true;
      this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->label4->ForeColor = System::Drawing::Color::Red;
      this->label4->Location = System::Drawing::Point(349, 212);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(190, 13);
      this->label4->TabIndex = 9;
      this->label4->Text = L"Selected object Ids Combination";
      // 
      // buttonCancel
      // 
      this->buttonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonCancel->Location = System::Drawing::Point(12, 427);
      this->buttonCancel->Name = L"buttonCancel";
      this->buttonCancel->Size = System::Drawing::Size(75, 23);
      this->buttonCancel->TabIndex = 10;
      this->buttonCancel->Text = L"Cancel";
      this->buttonCancel->UseVisualStyleBackColor = true;
      this->buttonCancel->Click += gcnew System::EventHandler(this, &TimeSeriesFilter::buttonCancel_Click);
      // 
      // buttonOk
      // 
      this->buttonOk->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonOk->Location = System::Drawing::Point(541, 426);
      this->buttonOk->Name = L"buttonOk";
      this->buttonOk->Size = System::Drawing::Size(75, 23);
      this->buttonOk->TabIndex = 11;
      this->buttonOk->Text = L"Ok";
      this->buttonOk->UseVisualStyleBackColor = true;
      this->buttonOk->Click += gcnew System::EventHandler(this, &TimeSeriesFilter::buttonOk_Click);
      // 
      // buttonSelectObjectId
      // 
      this->buttonSelectObjectId->Location = System::Drawing::Point(288, 267);
      this->buttonSelectObjectId->Name = L"buttonSelectObjectId";
      this->buttonSelectObjectId->Size = System::Drawing::Size(52, 23);
      this->buttonSelectObjectId->TabIndex = 12;
      this->buttonSelectObjectId->Text = L"-->";
      this->buttonSelectObjectId->UseVisualStyleBackColor = true;
      this->buttonSelectObjectId->Click += gcnew System::EventHandler(this, &TimeSeriesFilter::buttonSelectObjectId_Click);
      // 
      // buttonDeSelectObjectId
      // 
      this->buttonDeSelectObjectId->Location = System::Drawing::Point(288, 319);
      this->buttonDeSelectObjectId->Name = L"buttonDeSelectObjectId";
      this->buttonDeSelectObjectId->Size = System::Drawing::Size(52, 23);
      this->buttonDeSelectObjectId->TabIndex = 13;
      this->buttonDeSelectObjectId->Text = L"<--";
      this->buttonDeSelectObjectId->UseVisualStyleBackColor = true;
      this->buttonDeSelectObjectId->Click += gcnew System::EventHandler(this, &TimeSeriesFilter::buttonDeSelectObjectId_Click);
      // 
      // listBoxObjectIdsSelected
      // 
      this->listBoxObjectIdsSelected->FormattingEnabled = true;
      this->listBoxObjectIdsSelected->Location = System::Drawing::Point(347, 231);
      this->listBoxObjectIdsSelected->Name = L"listBoxObjectIdsSelected";
      this->listBoxObjectIdsSelected->Size = System::Drawing::Size(268, 160);
      this->listBoxObjectIdsSelected->TabIndex = 14;
      // 
      // comboBoxKeyWordsCombination
      // 
      this->comboBoxKeyWordsCombination->ForeColor = System::Drawing::Color::Red;
      this->comboBoxKeyWordsCombination->FormattingEnabled = true;
      this->comboBoxKeyWordsCombination->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"And", L"Or"});
      this->comboBoxKeyWordsCombination->Location = System::Drawing::Point(555, 17);
      this->comboBoxKeyWordsCombination->Name = L"comboBoxKeyWordsCombination";
      this->comboBoxKeyWordsCombination->Size = System::Drawing::Size(60, 21);
      this->comboBoxKeyWordsCombination->TabIndex = 15;
      this->comboBoxKeyWordsCombination->SelectedIndexChanged += gcnew System::EventHandler(this, &TimeSeriesFilter::comboBoxKeyWordsCombination_SelectedIndexChanged);
      // 
      // comboBoxObjectIdsCombination
      // 
      this->comboBoxObjectIdsCombination->ForeColor = System::Drawing::Color::Red;
      this->comboBoxObjectIdsCombination->FormattingEnabled = true;
      this->comboBoxObjectIdsCombination->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"And", L"Or"});
      this->comboBoxObjectIdsCombination->Location = System::Drawing::Point(555, 209);
      this->comboBoxObjectIdsCombination->Name = L"comboBoxObjectIdsCombination";
      this->comboBoxObjectIdsCombination->Size = System::Drawing::Size(60, 21);
      this->comboBoxObjectIdsCombination->TabIndex = 17;
      this->comboBoxObjectIdsCombination->SelectedIndexChanged += gcnew System::EventHandler(this, &TimeSeriesFilter::comboBoxObjectIdsCombination_SelectedIndexChanged);
      // 
      // TimeSeriesFilter
      // 
      this->AcceptButton = this->buttonOk;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->buttonCancel;
      this->ClientSize = System::Drawing::Size(637, 468);
      this->Controls->Add(this->comboBoxObjectIdsCombination);
      this->Controls->Add(this->comboBoxKeyWordsCombination);
      this->Controls->Add(this->listBoxObjectIdsSelected);
      this->Controls->Add(this->buttonDeSelectObjectId);
      this->Controls->Add(this->buttonSelectObjectId);
      this->Controls->Add(this->buttonOk);
      this->Controls->Add(this->buttonCancel);
      this->Controls->Add(this->label4);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->listBoxObjectIds);
      this->Controls->Add(this->buttonDeSelectKeyWord);
      this->Controls->Add(this->buttonSelectKeyWord);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->listBoxKeyWordsSelected);
      this->Controls->Add(this->listBoxKeyWords);
      this->Controls->Add(this->label1);
      this->Name = L"TimeSeriesFilter";
      this->Text = L"TimeSeriesFilter";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
	
///////////////////////////////////////////////////////////////////////////////
//  Initialisierung der Dialogbox beim Starten des Auswahlprozesses          //
///////////////////////////////////////////////////////////////////////////////
    System::Void initialize()
    {
      std::vector<string> vKeyWords;
      std::vector<string> vObjectIds;
      std::vector<string> vObjectNames;

      m_pRepository->getKeyWords( vKeyWords );
      for ( unsigned int i = 0; i < vKeyWords.size(); ++i )
      {
        String ^ STR = QuConvert::ToString( vKeyWords[i] );
        listBoxKeyWords->Items->Add ( STR );
      }

      m_pRepository->getRelObjs( vObjectIds, vObjectNames );
      for ( unsigned int i = 0; i < vObjectIds.size(); ++i )
      {
        String ^ Id = QuConvert::ToString( vObjectIds[i] );
        String ^ Name = QuConvert::ToString(vObjectNames[i]);
        String ^ STR = String::Concat(Id, " (", Name, ")");
        listBoxObjectIds->Items->Add( STR );

      }

      for ( int i = 0; i < m_keyWordsList->Count; i++ )
        listBoxKeyWordsSelected->Items->Add( m_keyWordsList[i] );

      for ( int i = 0; i < m_objectIdList->Count; i++ )
        listBoxObjectIdsSelected->Items->Add( m_objectIdList[i] );     
      
    }
///////////////////////////////////////////////////////////////////////////////
// Commandhandler Button "-->" (für Schlüsselwörter)                         //
// Wählt ein Schlüsselwort aus der Liste aller vorhandener Schlüsselwörter   //
// aus                                                                       //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonSelectKeyWord_Click(System::Object^  sender, System::EventArgs^  e)
         {
           if ( listBoxKeyWords->SelectedIndex == -1 )
             return;

           String ^ keyWord = dynamic_cast<String^>(listBoxKeyWords->SelectedItem);
           listBoxKeyWordsSelected->Items->Add( keyWord );
         }

///////////////////////////////////////////////////////////////////////////////
// Commandhandler Button "<--" (für Schlüsselwörter)                         //
// Macht die Auswahl eines Schlüsselwortes wieder rückgängig                 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonDeSelectKeyWord_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           if ( listBoxKeyWordsSelected->SelectedIndex == -1 )
             return;

           int index = listBoxKeyWordsSelected->SelectedIndex;
           listBoxKeyWordsSelected->Items->RemoveAt( index );
         }

///////////////////////////////////////////////////////////////////////////////
// Commandhandler Button "-->" (für Objekt-Ids)                              //
// Wählt eine Objekt-Id aus der Liste aller vorhandener Objekt-Ids aus       //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonSelectObjectId_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           if ( listBoxObjectIds->SelectedIndex == -1 )
             return;

           String ^ objectId = dynamic_cast<String^>(listBoxObjectIds->SelectedItem);
           listBoxObjectIdsSelected->Items->Add( objectId );
         }

///////////////////////////////////////////////////////////////////////////////
// Commandhandler Button "<--" (für Objekt-Ids)                              //
// Macht die Auswahl einer Objekt-Id wieder rückgängig                       //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonDeSelectObjectId_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           if ( listBoxObjectIdsSelected->SelectedIndex == -1 )
             return;

           int index = listBoxObjectIdsSelected->SelectedIndex;
           listBoxObjectIdsSelected->Items->RemoveAt( index );
         }

///////////////////////////////////////////////////////////////////////////////
//  Interface: Liefert die Liste der ausgewählten Schlüsselwörter            //
///////////////////////////////////////////////////////////////////////////////
public: List<String^> ^ getKeyWordsSelected() {  return m_keyWordsSelectedList; }

///////////////////////////////////////////////////////////////////////////////
//  Interface: Liefert die Liste der ausgewählten Objekt-Ids                 //
///////////////////////////////////////////////////////////////////////////////
public: List<String^> ^ getObjectIdsSelected()  {  return m_objectIdSelectedList;  }

///////////////////////////////////////////////////////////////////////////////
//  Interface: Liefert den Verknüpfungs-Operator für Schlüsselwörter         //
///////////////////////////////////////////////////////////////////////////////
public: Combination getKeyWordsCombination()  {  return m_keyWordCombination;  }

///////////////////////////////////////////////////////////////////////////////
//  Interface: Liefert den Verknüpfungs-Operator für Objekt-Ids              //
///////////////////////////////////////////////////////////////////////////////
public: Combination getObjectIdsCombination()  {  return m_objectIdCombination;  }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Ok"                                               //
//  Übernimmt die ausgewählten Schlüsselwörter / Objekt-Ids in die           //
//  Listen und beendet den Dialog                                            //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonOk_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           m_keyWordsSelectedList = gcnew List<String ^>;
           m_objectIdSelectedList = gcnew List<String ^>;

           for ( int i = 0; i < listBoxKeyWordsSelected->Items->Count; ++i )
             m_keyWordsSelectedList->Add( dynamic_cast<String^>(listBoxKeyWordsSelected->Items[i]));

           for (int i = 0; i < listBoxObjectIdsSelected->Items->Count; ++i)
           {
             String ^ STR = dynamic_cast<String^>(listBoxObjectIdsSelected->Items[i]);
             int index = STR->IndexOf (" (");
             if (index >= 0)
               m_objectIdSelectedList->Add(STR->Substring (0, index));
             else
               m_objectIdSelectedList->Add (STR);
           }

           Close();
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Cancel"                                           //
//  Beendet den Dialog                                                       //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonCancel_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           Close();
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl des Verknüpfungs-Operators für Schlüsselwörter   // 
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxKeyWordsCombination_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
         {
           switch ( comboBoxKeyWordsCombination->SelectedIndex )
           {
           case 0:
             m_keyWordCombination = Combination::And;
             break;

           case 1:
             m_keyWordCombination = Combination::Or;
             break;
           }
         }
 ///////////////////////////////////////////////////////////////////////////////
 //  Commandhandler: Auswahl des Verknüpfungs-Operators für Objekt-Ids        // 
 ///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxObjectIdsCombination_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
         {
           switch ( comboBoxObjectIdsCombination->SelectedIndex )
           {
           case 0:
             m_objectIdCombination = Combination::And;
             break;

           case 1:
             m_objectIdCombination = Combination::Or;
             break;
           }

         }
};
