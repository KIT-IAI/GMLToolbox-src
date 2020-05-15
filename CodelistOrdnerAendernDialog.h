#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "SrsManagerRef.h"
#include "Feature.h"
#include "Convert.h"
#include "constant.h"
#include "GmlSchema.h"
#include "ExternalCodeLists.h"

using namespace std;

	/// <summary>
	/// Dialog zur Auswahl und zum Ändern eines Codelisten-Dictionaries
	///
	/// </summary>
	public ref class CodelistOrdnerAendernDialog : public System::Windows::Forms::Form
	{
	public:
		CodelistOrdnerAendernDialog( Features * pFeatures )
		{
      vector<string> vGmlTypes;
			size_t         i, anz;

			InitializeComponent();
			
      m_pFeatures = pFeatures;
      anz = m_pFeatures->getExternalCodeListGmlTypesAsString( vGmlTypes );

      for ( i = 0; i < anz; i++ )
        comboBoxGmlTypen->Items->Add( QuConvert::ToString ( vGmlTypes[i] ) );

      gmlTyp = m_pFeatures->getGmlTyp();
      if ( gmlTyp == GML_SCHEMA_UNKNOWN ) 
        gmlTyp = XPLANGML_5_1;
      
      string gmlTypString = Constant::getGMLSchemaTypeAsString( gmlTyp );
      comboBoxGmlTypen->SelectedItem = QuConvert::ToString ( gmlTypString );

      string codelistFolder = m_pFeatures->getExternalCodeListsFolder( gmlTyp );
      textBoxFolder->Text = QuConvert::ToString( codelistFolder );

      buttonUebernehmen_Click();
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~CodelistOrdnerAendernDialog()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;
  private: System::Windows::Forms::ComboBox^  comboBoxGmlTypen;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  textBoxFolder;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Button^  buttonAuswahlLokalerOrdner;
  private: System::Windows::Forms::Button^  buttonUebernehmen;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::ListBox^  listBoxFiles;
  private: System::Windows::Forms::Button^  buttonCodelistBearbeiten;

  public:
    Features      * m_pFeatures;   //  Features Modul
    GML_SCHEMA_TYPE gmlTyp;        //  Typ der GML-Applikationsschemas

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->comboBoxGmlTypen = (gcnew System::Windows::Forms::ComboBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBoxFolder = (gcnew System::Windows::Forms::TextBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->buttonAuswahlLokalerOrdner = (gcnew System::Windows::Forms::Button());
      this->buttonUebernehmen = (gcnew System::Windows::Forms::Button());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->listBoxFiles = (gcnew System::Windows::Forms::ListBox());
      this->buttonCodelistBearbeiten = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // comboBoxGmlTypen
      // 
      this->comboBoxGmlTypen->FormattingEnabled = true;
      this->comboBoxGmlTypen->Location = System::Drawing::Point(13, 23);
      this->comboBoxGmlTypen->Name = L"comboBoxGmlTypen";
      this->comboBoxGmlTypen->Size = System::Drawing::Size(135, 21);
      this->comboBoxGmlTypen->TabIndex = 0;
      this->comboBoxGmlTypen->SelectedIndexChanged += gcnew System::EventHandler(this, &CodelistOrdnerAendernDialog::comboBoxGmlTypen_SelectedIndexChanged);
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(155, 30);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(51, 13);
      this->label1->TabIndex = 1;
      this->label1->Text = L"GML-Typ";
      // 
      // textBoxFolder
      // 
      this->textBoxFolder->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxFolder->Location = System::Drawing::Point(15, 50);
      this->textBoxFolder->Name = L"textBoxFolder";
      this->textBoxFolder->Size = System::Drawing::Size(510, 20);
      this->textBoxFolder->TabIndex = 2;
      this->textBoxFolder->Text = L"D:\\XPlanung\\XPlanGML-Toolbox_Executable_9_0\\GMLSchemata\\XPlanGML_3_0\\ExternalCode"
        L"lists";
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(13, 77);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(129, 13);
      this->label2->TabIndex = 3;
      this->label2->Text = L"URL des Codelist-Ordners";
      // 
      // buttonAuswahlLokalerOrdner
      // 
      this->buttonAuswahlLokalerOrdner->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonAuswahlLokalerOrdner->Location = System::Drawing::Point(374, 84);
      this->buttonAuswahlLokalerOrdner->Name = L"buttonAuswahlLokalerOrdner";
      this->buttonAuswahlLokalerOrdner->Size = System::Drawing::Size(151, 23);
      this->buttonAuswahlLokalerOrdner->TabIndex = 4;
      this->buttonAuswahlLokalerOrdner->Text = L"Auswahl lokaler Ordner";
      this->buttonAuswahlLokalerOrdner->UseVisualStyleBackColor = true;
      this->buttonAuswahlLokalerOrdner->Click += gcnew System::EventHandler(this, &CodelistOrdnerAendernDialog::buttonAuswahlLokalerOedner_Click);
      // 
      // buttonUebernehmen
      // 
      this->buttonUebernehmen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonUebernehmen->Location = System::Drawing::Point(412, 21);
      this->buttonUebernehmen->Name = L"buttonUebernehmen";
      this->buttonUebernehmen->Size = System::Drawing::Size(113, 23);
      this->buttonUebernehmen->TabIndex = 5;
      this->buttonUebernehmen->Text = L"Übernehmen";
      this->buttonUebernehmen->UseVisualStyleBackColor = true;
      this->buttonUebernehmen->Click += gcnew System::EventHandler(this, &CodelistOrdnerAendernDialog::buttonUebernehmen_Click);
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonBeenden->Location = System::Drawing::Point(412, 365);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(113, 23);
      this->buttonBeenden->TabIndex = 6;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &CodelistOrdnerAendernDialog::buttonBeenden_Click);
      // 
      // listBoxFiles
      // 
      this->listBoxFiles->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
        | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->listBoxFiles->FormattingEnabled = true;
      this->listBoxFiles->Location = System::Drawing::Point(13, 127);
      this->listBoxFiles->Name = L"listBoxFiles";
      this->listBoxFiles->Size = System::Drawing::Size(512, 225);
      this->listBoxFiles->TabIndex = 7;
      // 
      // buttonCodelistBearbeiten
      // 
      this->buttonCodelistBearbeiten->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonCodelistBearbeiten->Location = System::Drawing::Point(13, 359);
      this->buttonCodelistBearbeiten->Name = L"buttonCodelistBearbeiten";
      this->buttonCodelistBearbeiten->Size = System::Drawing::Size(155, 23);
      this->buttonCodelistBearbeiten->TabIndex = 8;
      this->buttonCodelistBearbeiten->Text = L"Codelist bearbeiten";
      this->buttonCodelistBearbeiten->UseVisualStyleBackColor = true;
      this->buttonCodelistBearbeiten->Click += gcnew System::EventHandler(this, &CodelistOrdnerAendernDialog::buttonCodelistBearbeiten_Click);
      // 
      // CodelistOrdnerAendernDialog
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(537, 400);
      this->Controls->Add(this->buttonCodelistBearbeiten);
      this->Controls->Add(this->listBoxFiles);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->buttonUebernehmen);
      this->Controls->Add(this->buttonAuswahlLokalerOrdner);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->textBoxFolder);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->comboBoxGmlTypen);
      this->Name = L"CodelistOrdnerAendernDialog";
      this->Text = L"URL des Codelist-Ordners";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Übernehmen                                         //
//  Liest die im angezeigten Codelist-Ordner enthaltenen Dateien ein         // 
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonUebernehmen_Click() 
         {
           bool                                exist;
           int                                 i;
           cli::array<System::IO::FileInfo^> ^ files;
           string                              externalCodeListsFolder;
           bool                                success = true;
           System::Uri                       ^ uri;
           listBoxFiles->Items->Clear();

           try
           {
             uri = gcnew System::Uri ( textBoxFolder->Text ); 

             if ( uri->IsFile )
             {
               System::IO::DirectoryInfo ^ dictionaryInfo = gcnew System::IO::DirectoryInfo ( textBoxFolder->Text );

               exist = dictionaryInfo->Exists;
               if ( !exist )
               {
                 listBoxFiles->Items->Add( "Der angegebene Ordner existiert nicht" );
                 success = false;
               }
               else
               {
                 files = dictionaryInfo->GetFiles();
                 for ( i = 0; i < files->Length; i++ )
                   listBoxFiles->Items->Add( files[i] );
                 if ( files->Length == 0 )
                 {
                   listBoxFiles->Items->Add( "Der angegebene Ordner ist leer" );
                   success = false;
                 }
               }
             }
             else
             {
               System::Windows::Forms::DialogResult result;
               result =  MessageBox::Show ( "Soll die angegebene URI übernommen werden?", "", System::Windows::Forms::MessageBoxButtons::YesNo );
               if ( result == System::Windows::Forms::DialogResult::No ) success = false;
             }

             if ( success )
             {
               QuConvert::systemStr2stdStr( externalCodeListsFolder, textBoxFolder->Text );
               m_pFeatures->setExternalCodeListFolder ( gmlTyp, externalCodeListsFolder );
             }
           }

           catch ( System::Exception ^ )
           {
             listBoxFiles->Items->Add( "Die angegebene URI ist ungültig" );
             success = false;
           }

         }

///////////////////////////////////////////////////////////////////////////////
//  Setzt nach Azuswahl eines Applikationsschemas den Standard-Ordner der    //
//  externen Codelisten                                                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxGmlTypen_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
  String ^ GmlTypAsString = dynamic_cast<String^>(comboBoxGmlTypen->SelectedItem);
  string   gmlTypAsString;
  string   externalCodeListsFolder;

  QuConvert::systemStr2stdStr( gmlTypAsString, GmlTypAsString );
  gmlTyp = Constant::getGMLSchemaTypeFromString( gmlTypAsString );

  externalCodeListsFolder = m_pFeatures->getExternalCodeListsFolder( gmlTyp );
  textBoxFolder->Text = QuConvert::ToString( externalCodeListsFolder );
  buttonUebernehmen_Click();
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Übernehmen"                                       //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonUebernehmen_Click(System::Object^  sender, System::EventArgs^  e) 
{
  buttonUebernehmen_Click();
}
         

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"                                          //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e) 
{
 
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler für Auswahl eines lokalen Ordners der externen Codelisten //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAuswahlLokalerOedner_Click(System::Object^  sender, System::EventArgs^  e)
{
  System::Windows::Forms::FolderBrowserDialog ^ pDialog = gcnew System::Windows::Forms::FolderBrowserDialog;
  System::Windows::Forms::DialogResult          ok;
  System::Uri                                 ^ uri;
  string                                        codeListFolder;

  uri = gcnew System::Uri ( textBoxFolder->Text ); 

  if ( uri->IsFile )
  {
    System::IO::DirectoryInfo ^ dictionaryInfo = gcnew System::IO::DirectoryInfo ( textBoxFolder->Text );
    bool exist = dictionaryInfo->Exists;
    if ( exist )
      pDialog->SelectedPath =  textBoxFolder->Text;
    else
      pDialog->SelectedPath = QuConvert::ToString ( Constant::getSystemFolder() );
  }
  else
  {
    pDialog->SelectedPath = QuConvert::ToString ( Constant::getSystemFolder() );
  }

  ok = pDialog->ShowDialog();
  if ( ok == Windows::Forms::DialogResult::OK )
  {
    textBoxFolder->Text = pDialog->SelectedPath;
    QuConvert::systemStr2stdStr ( codeListFolder, pDialog->SelectedPath );
    m_pFeatures->setExternalCodeListFolder( gmlTyp, codeListFolder );
    buttonUebernehmen_Click();
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Commanhandler für Bearbeitung einer selektierten Codeliste               //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonCodelistBearbeiten_Click(System::Object^  sender, System::EventArgs^  e)
{
  System::IO::FileInfo ^ selectedValue;
  ExternalCodeLists ^    pExternalCodeLists;  

  selectedValue = dynamic_cast<System::IO::FileInfo^>(listBoxFiles->SelectedItem);

  if ( !File::Exists( selectedValue->FullName ) )
    return;

  pExternalCodeLists = gcnew ExternalCodeLists( selectedValue->FullName, gmlTyp, GmlSchema::getGmlVersion( gmlTyp ) );
  pExternalCodeLists->ShowDialog();  
}

};
