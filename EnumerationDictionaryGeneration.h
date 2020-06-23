#pragma once

#include "constant.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "GmlSchema.h"
#include "Definitions.h"
#include "Dictionary.h"
#include "Convert.h"

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Erzeugung eines GML-Dictionaries für alle XPlanGML-Enumerationen
	/// </summary>
	public ref class EnumerationDictionaryGeneration : public System::Windows::Forms::Form
	{
	public:
		EnumerationDictionaryGeneration( Features * pFeatures )
		{
			InitializeComponent();

      m_pFeatures = pFeatures;
      comboBoxXPlanGMLVersion->SelectedIndex = 4;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~EnumerationDictionaryGeneration(  )
		{
			if (components)
			{
				delete components;
			}
		}

  private: System::Windows::Forms::Button^  buttonErzeugen;
  private: System::Windows::Forms::Button^  buttonAbbrechen;
  private: System::Windows::Forms::FolderBrowserDialog^  dictionaryFolderDialog;
  private: System::Windows::Forms::SaveFileDialog^  gmlDictionaryDialog;
  private: System::Windows::Forms::ComboBox^  comboBoxXPlanGMLVersion;
  private: System::Windows::Forms::Label^  label1;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

    Features * m_pFeatures;			//	Modul Features

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->buttonErzeugen = ( gcnew System::Windows::Forms::Button() );
      this->buttonAbbrechen = ( gcnew System::Windows::Forms::Button() );
      this->dictionaryFolderDialog = ( gcnew System::Windows::Forms::FolderBrowserDialog() );
      this->gmlDictionaryDialog = ( gcnew System::Windows::Forms::SaveFileDialog() );
      this->comboBoxXPlanGMLVersion = ( gcnew System::Windows::Forms::ComboBox() );
      this->label1 = ( gcnew System::Windows::Forms::Label() );
      this->SuspendLayout();
      // 
      // buttonErzeugen
      // 
      this->buttonErzeugen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right ) );
      this->buttonErzeugen->Location = System::Drawing::Point( 219, 108 );
      this->buttonErzeugen->Name = L"buttonErzeugen";
      this->buttonErzeugen->Size = System::Drawing::Size( 75, 23 );
      this->buttonErzeugen->TabIndex = 6;
      this->buttonErzeugen->Text = L"Erzeugen";
      this->buttonErzeugen->UseVisualStyleBackColor = true;
      this->buttonErzeugen->Click += gcnew System::EventHandler( this, &EnumerationDictionaryGeneration::buttonErzeugen_Click );
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->Location = System::Drawing::Point( 12, 107 );
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size( 75, 23 );
      this->buttonAbbrechen->TabIndex = 7;
      this->buttonAbbrechen->Text = L"Abbrechen";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      this->buttonAbbrechen->Click += gcnew System::EventHandler( this, &EnumerationDictionaryGeneration::buttonAbbrechen_Click );
      // 
      // comboBoxXPlanGMLVersion
      // 
      this->comboBoxXPlanGMLVersion->FormattingEnabled = true;
      this->comboBoxXPlanGMLVersion->Items->AddRange( gcnew cli::array< System::Object^  >( 8 )
      {
        L"XPlanGML 2.0", L"XPlanGML 3.0",
          L"XPlanGML 4.0", L"XPlanGML 4.1", L"XPlanGML 5.0", L"XPlanGML 5.1", L"XPlanGML 5.2", L"XPlanGML 5.3"
      } );
      this->comboBoxXPlanGMLVersion->Location = System::Drawing::Point( 13, 28 );
      this->comboBoxXPlanGMLVersion->Name = L"comboBoxXPlanGMLVersion";
      this->comboBoxXPlanGMLVersion->Size = System::Drawing::Size( 148, 21 );
      this->comboBoxXPlanGMLVersion->TabIndex = 8;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point( 168, 35 );
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size( 96, 13 );
      this->label1->TabIndex = 9;
      this->label1->Text = L"XPlanGML-Version";
      // 
      // EnumerationDictionaryGeneration
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF( 6, 13 );
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size( 310, 148 );
      this->Controls->Add( this->label1 );
      this->Controls->Add( this->comboBoxXPlanGMLVersion );
      this->Controls->Add( this->buttonAbbrechen );
      this->Controls->Add( this->buttonErzeugen );
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"EnumerationDictionaryGeneration";
      this->Text = L"GML-Dictionary der Enumerationen erzeugen";
      this->ResumeLayout( false );
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Erzeugen"																				 //
//  Erzeugt das Dictionary																									 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonErzeugen_Click(System::Object^  sender, System::EventArgs^  e)
         {
           GML_SCHEMA_TYPE schemaType;

           switch ( comboBoxXPlanGMLVersion->SelectedIndex )
           {
           case 0:
             schemaType = XPLANGML_2_0;
             break;

           case 1:
             schemaType = XPLANGML_3_0;
             break;

           case 2:
             schemaType = XPLANGML_4_0;
             break;

           case 3:
             schemaType = XPLANGML_4_1;
             break;

           case 4:
             schemaType = XPLANGML_5_0;
             break;

           case 5:
             schemaType = XPLANGML_5_1;
             break;

           case 6:
             schemaType = XPLANGML_5_2;
             break;

           case 7:
             schemaType = XPLANGML_5_3;
             break;
           }

           string ausgabeDatei = Constant::getCodelistPath( schemaType ) + "XPlanGml_CodeLists.xml";

           GmlNamespaces * pGmlNamespacesAct = new GmlNamespaces();
           GmlSchema * pGmlSchema = new GmlSchema ( m_pFeatures );
           bool success = pGmlSchema->readGmlSchema( schemaType, pGmlNamespacesAct );

           GmlDictionary * pEnumerationsDictionary = pGmlSchema->getCodeListsDictionary();
           if ( pEnumerationsDictionary == NULL )
             return;

           GmlDictionaryWriter ^ pWriter = gcnew GmlDictionaryWriter ( pEnumerationsDictionary, pGmlSchema->getGmlVersion() );
           pWriter->writeDictionaryCollection( ausgabeDatei );

           delete pGmlSchema;

           Close();
         }
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Abbrechen"																			   //
//	Schließt den Dialog																											 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAbbrechen_Click(System::Object^  sender, System::EventArgs^  e)
         {
           Close();
         }
};
