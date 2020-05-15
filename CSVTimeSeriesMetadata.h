#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "TimeSeriesRepository.h"
#include "Constant.h"
#include "KeyWords.h"
#include "Convert.h"

	/// <summary>
	/// Ermöglicht es, für die Speicherung einer Zeitreihen-Menge (RegularTimeSeriesSet) im TimeSeriesRepository
  /// Metadaten zu spezifizieren, und den RegularTimeSeriesSet in einem TimeSeriesRepository zu speichern
	/// </summary>
	public ref class CSVTimeSeriesMetadata : public System::Windows::Forms::Form
	{
	public:
		CSVTimeSeriesMetadata( RegularTimeSeriesSet * pTimeSeriesSet, UOMList * pUOMList, String ^ timeSeriesRepositoryFolder )
		{
			InitializeComponent();

      m_pTimeSeriesSet = pTimeSeriesSet;
      comboBoxAquisitionMethod->SelectedIndex = 1;
      comboBoxType->SelectedIndex = 0;

      std::string str;
      QuConvert::systemStr2stdStr( str, timeSeriesRepositoryFolder );
      m_pTimeSeriesRepository =  new TimeSeriesRepository ( pUOMList, str );
      m_pTimeSeriesRepository->scanRepository();	

			lKeyWordsSelected = gcnew List<String^>;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~CSVTimeSeriesMetadata()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxSource;
  protected: 
  private: System::Windows::Forms::ComboBox^  comboBoxAquisitionMethod;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::TextBox^  textBoxDescription;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::TextBox^  textBoxKeyWords;
  private: System::Windows::Forms::Button^  buttonKeyWords;
  private: System::Windows::Forms::Button^  buttonCancel;
  private: System::Windows::Forms::Button^  buttonOK;
  private: System::Windows::Forms::TextBox^  textBoxApplication;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::ComboBox^  comboBoxType;
  private: System::Windows::Forms::Label^  label5;

  protected: 

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

    RegularTimeSeriesSet             * m_pTimeSeriesSet;        // Mit Metadaten zu beschreibender RegularTimeSeriesSet
    TimeSeriesRepository             * m_pTimeSeriesRepository; // Benutztes TimeSeriesRepository
		List<String^>										 ^ lKeyWordsSelected;       // Key Words zur Beschreibung der Zeitreihen


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->textBoxSource = (gcnew System::Windows::Forms::TextBox());
      this->comboBoxAquisitionMethod = (gcnew System::Windows::Forms::ComboBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->textBoxDescription = (gcnew System::Windows::Forms::TextBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->textBoxKeyWords = (gcnew System::Windows::Forms::TextBox());
      this->buttonKeyWords = (gcnew System::Windows::Forms::Button());
      this->buttonCancel = (gcnew System::Windows::Forms::Button());
      this->buttonOK = (gcnew System::Windows::Forms::Button());
      this->textBoxApplication = (gcnew System::Windows::Forms::TextBox());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->comboBoxType = (gcnew System::Windows::Forms::ComboBox());
      this->label5 = (gcnew System::Windows::Forms::Label());
      this->SuspendLayout();
      // 
      // textBoxSource
      // 
      this->textBoxSource->Location = System::Drawing::Point(12, 118);
      this->textBoxSource->Name = L"textBoxSource";
      this->textBoxSource->Size = System::Drawing::Size(244, 20);
      this->textBoxSource->TabIndex = 0;
      // 
      // comboBoxAquisitionMethod
      // 
      this->comboBoxAquisitionMethod->FormattingEnabled = true;
      this->comboBoxAquisitionMethod->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"measurement", L"simulation", L"calibratedSimulation", 
        L"estimation", L"unknown"});
      this->comboBoxAquisitionMethod->Location = System::Drawing::Point(12, 41);
      this->comboBoxAquisitionMethod->Name = L"comboBoxAquisitionMethod";
      this->comboBoxAquisitionMethod->Size = System::Drawing::Size(244, 21);
      this->comboBoxAquisitionMethod->TabIndex = 1;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(260, 45);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(90, 13);
      this->label1->TabIndex = 2;
      this->label1->Text = L"Aquisition method";
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(264, 122);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(41, 13);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Source";
      // 
      // textBoxDescription
      // 
      this->textBoxDescription->Location = System::Drawing::Point(13, 148);
      this->textBoxDescription->Multiline = true;
      this->textBoxDescription->Name = L"textBoxDescription";
      this->textBoxDescription->Size = System::Drawing::Size(243, 101);
      this->textBoxDescription->TabIndex = 4;
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(265, 146);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(60, 13);
      this->label3->TabIndex = 5;
      this->label3->Text = L"Description";
      // 
      // textBoxKeyWords
      // 
      this->textBoxKeyWords->Location = System::Drawing::Point(12, 262);
      this->textBoxKeyWords->Multiline = true;
      this->textBoxKeyWords->Name = L"textBoxKeyWords";
      this->textBoxKeyWords->ReadOnly = true;
      this->textBoxKeyWords->Size = System::Drawing::Size(244, 63);
      this->textBoxKeyWords->TabIndex = 6;
      // 
      // buttonKeyWords
      // 
      this->buttonKeyWords->Location = System::Drawing::Point(268, 262);
      this->buttonKeyWords->Name = L"buttonKeyWords";
      this->buttonKeyWords->Size = System::Drawing::Size(75, 23);
      this->buttonKeyWords->TabIndex = 7;
      this->buttonKeyWords->Text = L"Key words";
      this->buttonKeyWords->UseVisualStyleBackColor = true;
      this->buttonKeyWords->Click += gcnew System::EventHandler(this, &CSVTimeSeriesMetadata::buttonKeyWords_Click);
      // 
      // buttonCancel
      // 
      this->buttonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonCancel->Location = System::Drawing::Point(12, 338);
      this->buttonCancel->Name = L"buttonCancel";
      this->buttonCancel->Size = System::Drawing::Size(75, 23);
      this->buttonCancel->TabIndex = 8;
      this->buttonCancel->Text = L"Cancel";
      this->buttonCancel->UseVisualStyleBackColor = true;
      this->buttonCancel->Click += gcnew System::EventHandler(this, &CSVTimeSeriesMetadata::buttonCancel_Click);
      // 
      // buttonOK
      // 
      this->buttonOK->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonOK->Location = System::Drawing::Point(274, 337);
      this->buttonOK->Name = L"buttonOK";
      this->buttonOK->Size = System::Drawing::Size(75, 23);
      this->buttonOK->TabIndex = 9;
      this->buttonOK->Text = L"OK";
      this->buttonOK->UseVisualStyleBackColor = true;
      this->buttonOK->Click += gcnew System::EventHandler(this, &CSVTimeSeriesMetadata::buttonOK_Click);
      // 
      // textBoxApplication
      // 
      this->textBoxApplication->Location = System::Drawing::Point(13, 13);
      this->textBoxApplication->Name = L"textBoxApplication";
      this->textBoxApplication->Size = System::Drawing::Size(243, 20);
      this->textBoxApplication->TabIndex = 10;
      // 
      // label4
      // 
      this->label4->AutoSize = true;
      this->label4->Location = System::Drawing::Point(261, 19);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(59, 13);
      this->label4->TabIndex = 11;
      this->label4->Text = L"Application";
      // 
      // comboBoxType
      // 
      this->comboBoxType->FormattingEnabled = true;
      this->comboBoxType->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"simulationOutput", L"simulationInput", L"simulationInputOutput", 
        L"weatherData", L"other"});
      this->comboBoxType->Location = System::Drawing::Point(13, 69);
      this->comboBoxType->Name = L"comboBoxType";
      this->comboBoxType->Size = System::Drawing::Size(243, 21);
      this->comboBoxType->TabIndex = 12;
      // 
      // label5
      // 
      this->label5->AutoSize = true;
      this->label5->Location = System::Drawing::Point(263, 76);
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size(31, 13);
      this->label5->TabIndex = 13;
      this->label5->Text = L"Type";
      // 
      // CSVTimeSeriesMetadata
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(378, 394);
      this->Controls->Add(this->label5);
      this->Controls->Add(this->comboBoxType);
      this->Controls->Add(this->label4);
      this->Controls->Add(this->textBoxApplication);
      this->Controls->Add(this->buttonOK);
      this->Controls->Add(this->buttonCancel);
      this->Controls->Add(this->buttonKeyWords);
      this->Controls->Add(this->textBoxKeyWords);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->textBoxDescription);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->comboBoxAquisitionMethod);
      this->Controls->Add(this->textBoxSource);
      this->Name = L"CSVTimeSeriesMetadata";
      this->Text = L"Time series set metadata";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Key words"                                        //
//  Ermöglicht es, Schlüsselwörter zur Beschreibung des RegularTimeSeriesSet //
//  zu definieren                                                            //
///////////////////////////////////////////////////////////////////////////////
  private: System::Void buttonKeyWords_Click(System::Object^  sender, System::EventArgs^  e) 
           {
             std::vector<std::string> vKeyWords;
             List<String^>          ^ lKeyWords = gcnew List<String^>;					 

             m_pTimeSeriesRepository->getKeyWords( vKeyWords );
             for ( unsigned int i = 0; i < vKeyWords.size(); i++ )
               lKeyWords->Add( QuConvert::ToString ( vKeyWords[i] ) );

             KeyWords ^ pDialog = gcnew KeyWords ( lKeyWords, lKeyWordsSelected );
             if ( pDialog->ShowDialog() ==  System::Windows::Forms::DialogResult::OK )
             {
               lKeyWordsSelected = pDialog->getKeyWordsSelected();

               textBoxKeyWords->Text = "";
               for ( int i = 0; i < lKeyWordsSelected->Count; ++i )
               {
                 if ( i < lKeyWordsSelected->Count - 1 )
                   textBoxKeyWords->Text = String::Concat( textBoxKeyWords->Text, lKeyWordsSelected[i], "; " );
                 else
                   textBoxKeyWords->Text = String::Concat( textBoxKeyWords->Text, lKeyWordsSelected[i] );
               }
             }

           }
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "OK"                                               //
//  Fügt dem RegularTimeSeriesSet die spezifizieren Metadaten hinzu und      //
//  speichert ihn im TimeSeriesRepository                                    //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonOK_Click(System::Object^  sender, System::EventArgs^  e)
         {
           RegularTimeSeriesSetProperties * pTimeSeriesSetProperties;
           String                         ^ STR;
           std::string                      description = "";
           std::string                      str;
           std::string                      gmlId;
           std::string                      application = "";
           std::string                      type = "";
           std::string                      acquisitionMethod = "";
           std::string                      source = "";
           std::vector<std::string>         vKeyWords;

           STR = dynamic_cast<String^>(comboBoxAquisitionMethod->SelectedItem);
           if ( STR != nullptr )
             QuConvert::systemStr2stdStr( acquisitionMethod, STR );

           if ( textBoxSource->Text->CompareTo( "") != 0 )
             QuConvert::systemStr2stdStr( source, textBoxSource->Text );

           STR = dynamic_cast<String^>(comboBoxType->SelectedItem);
           if ( STR != nullptr )
             QuConvert::systemStr2stdStr( type, STR );

           if ( textBoxDescription->Text->CompareTo( "" ) != 0 )
             QuConvert::systemStr2stdStr( description, textBoxDescription->Text );

           if ( textBoxApplication->Text->CompareTo( "" ) != 0 )
             QuConvert::systemStr2stdStr( application, textBoxApplication->Text );


           if ( textBoxKeyWords->Text->CompareTo( "" ) != 0 )
           {
             cli::array<String^> ^ delimiter = { ";" };
             cli::array<String^> ^ split;

             split = textBoxKeyWords->Text->Split ( delimiter, StringSplitOptions::None );
             for ( int i = 0; i < split->Length; ++i )
             { 
               split[i] = split[i]->Trim();
               QuConvert::systemStr2stdStr( str,split[i] );
               vKeyWords.push_back ( str );
             }
           }

           gmlId = Constant::createGmlId();
           pTimeSeriesSetProperties = new RegularTimeSeriesSetProperties ( gmlId );
           pTimeSeriesSetProperties->setApplication ( application );
           pTimeSeriesSetProperties->setTimeSeriesSetType( type );
           pTimeSeriesSetProperties->setDescription( description );
           for ( unsigned int j = 0; j < vKeyWords.size(); j++ )
             pTimeSeriesSetProperties->addKeyWord ( vKeyWords[j] );

           int firstYear = 9999;
           int lastYear  = 0;
           for ( unsigned int i = 0; i < m_pTimeSeriesSet->getTimeSeriesAnz(); ++i )
           {
             RegularTimeSeries * pTimeSeries = m_pTimeSeriesSet->getTimeSeries(  i );
             pTimeSeries->setAquisitionMethod( acquisitionMethod );
             pTimeSeries->setSource( source );

             int year = pTimeSeries->getTemporalExtent()->getStartPosition().getYear();
             if ( year > lastYear )
               lastYear = year;
             if ( year < firstYear )
               firstYear = year;
           }
           pTimeSeriesSetProperties->setStartYear ( firstYear );
           pTimeSeriesSetProperties->setEndYear( lastYear );

           m_pTimeSeriesSet->setMetaData ( pTimeSeriesSetProperties );

           m_pTimeSeriesRepository->addTimeSeriesSetToSave( m_pTimeSeriesSet );         
           m_pTimeSeriesRepository->save( false );

           Close();
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Cancel":                                          //
//  Schließt den Dialog, ohne den RegualarTimeSeriesSet zu verändern         //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonCancel_Click(System::Object^  sender, System::EventArgs^  e)
         {
           Close();
         }
};
