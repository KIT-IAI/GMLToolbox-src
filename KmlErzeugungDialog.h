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
#include "Visualisierung_3D.h"
#include "GmlSchema.h"

#include <string>


	/// <summary>
	/// Export von 3D-Geodaten als KLM-Datei
	///
	/// </summary>
	public ref class KmlErzeugungDialog : public System::Windows::Forms::Form
	{
	public:
		KmlErzeugungDialog( Visualisierung_3D * pVisual, std::string gmlFileName, std::string kmlViewerP, 
            bool show, bool showFeatureDataP )
		{
      vector<string>        vLODStufen;
      size_t                i, anz;
      string                lodStufe;
      set<string>           sThemen;
      set<string>::iterator iter;

			InitializeComponent();
			
      m_pVisual  = pVisual;
      m_pFeatures= m_pVisual->getFeatures();
      kmlViewer  = QuConvert::ToString( kmlViewerP );

      zOffset             =  0.0;
      textBoxOffset->Text = Convert::ToString( zOffset );

      checkBoxAnzeige->Checked = show;
      showFeatureData          = showFeatureDataP;

      String ^ GML_FILE_NAME = QuConvert::ToString( gmlFileName );
      int index = GML_FILE_NAME->LastIndexOf( ".");
      GML_FILE_NAME = GML_FILE_NAME->Substring( 0, index+1 );

      textBoxDateiname->Text = String::Concat( GML_FILE_NAME, "kml" );
     

      if ( m_pFeatures->getGmlSchema()->isCityGml() )
      {
        labelSelection->Text    = "Dargestellter LOD";

        anz = m_pFeatures->getLODStufenAsString ( vLODStufen );
        for ( i = 0; i <anz; i++ )
        {
          lodStufe = vLODStufen[i];
          comboBoxSelection->Items->Add( QuConvert::ToString( lodStufe ) );
        }
        if ( vLODStufen.size() > 0 )
          comboBoxSelection->SelectedItem = QuConvert::ToString(  vLODStufen[0] );

      }
      else
      if ( m_pFeatures->getGmlSchema()->isBoreholeML() )
      {
        labelSelection->Text    = "Dargestellter Borehole-Thema";

        comboBoxSelection->Items->Add ( "Stratigraphie" );
        comboBoxSelection->Items->Add ( "Lithologie, 1. Komponente" );
        comboBoxSelection->Items->Add ( "Lithologie, 2. Komponente" );
        comboBoxSelection->Items->Add ( "Lithologie, 3. Komponente" );
        comboBoxSelection->Items->Add ( "Lithologie, 4. Komponente" );
        comboBoxSelection->SelectedIndex = 0;
      }

		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~KmlErzeugungDialog()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxDateiname;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Button^  buttonAendern;
  private: System::Windows::Forms::CheckBox^  checkBoxAnzeige;
  private: System::Windows::Forms::TextBox^  textBoxOffset;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Button^  buttonAbbrechen;
  private: System::Windows::Forms::Button^  buttonStarten;
  private: System::Windows::Forms::SaveFileDialog^  saveKmlFileDialog;
  private: System::Windows::Forms::ComboBox^  comboBoxSelection;
  private: System::Windows::Forms::Label^  labelSelection;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

  private:
    Visualisierung_3D * m_pVisual;
    Features          * m_pFeatures;
    String            ^ kmlViewer;
    double              zOffset;
    String            ^ offsetBackup;
    String            ^ selectedLOD;
    bool                showFeatureData;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->textBoxDateiname = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->buttonAendern = (gcnew System::Windows::Forms::Button());
      this->checkBoxAnzeige = (gcnew System::Windows::Forms::CheckBox());
      this->textBoxOffset = (gcnew System::Windows::Forms::TextBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->buttonStarten = (gcnew System::Windows::Forms::Button());
      this->saveKmlFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
      this->comboBoxSelection = (gcnew System::Windows::Forms::ComboBox());
      this->labelSelection = (gcnew System::Windows::Forms::Label());
      this->SuspendLayout();
      // 
      // textBoxDateiname
      // 
      this->textBoxDateiname->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxDateiname->Location = System::Drawing::Point(13, 31);
      this->textBoxDateiname->Name = L"textBoxDateiname";
      this->textBoxDateiname->Size = System::Drawing::Size(327, 20);
      this->textBoxDateiname->TabIndex = 0;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(12, 57);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(55, 13);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Pfadname";
      // 
      // buttonAendern
      // 
      this->buttonAendern->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonAendern->Location = System::Drawing::Point(265, 57);
      this->buttonAendern->Name = L"buttonAendern";
      this->buttonAendern->Size = System::Drawing::Size(75, 23);
      this->buttonAendern->TabIndex = 2;
      this->buttonAendern->Text = L"Ändern";
      this->buttonAendern->UseVisualStyleBackColor = true;
      this->buttonAendern->Click += gcnew System::EventHandler(this, &KmlErzeugungDialog::buttonAendern_Click);
      // 
      // checkBoxAnzeige
      // 
      this->checkBoxAnzeige->AutoSize = true;
      this->checkBoxAnzeige->Location = System::Drawing::Point(12, 73);
      this->checkBoxAnzeige->Name = L"checkBoxAnzeige";
      this->checkBoxAnzeige->Size = System::Drawing::Size(103, 17);
      this->checkBoxAnzeige->TabIndex = 3;
      this->checkBoxAnzeige->Text = L"Modell anzeigen";
      this->checkBoxAnzeige->UseVisualStyleBackColor = true;
      // 
      // textBoxOffset
      // 
      this->textBoxOffset->Location = System::Drawing::Point(15, 109);
      this->textBoxOffset->Name = L"textBoxOffset";
      this->textBoxOffset->Size = System::Drawing::Size(52, 20);
      this->textBoxOffset->TabIndex = 4;
      this->textBoxOffset->Leave += gcnew System::EventHandler(this, &KmlErzeugungDialog::textBoxOffset_Leave);
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(12, 132);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(82, 13);
      this->label2->TabIndex = 5;
      this->label2->Text = L"Vertikaler Offset";
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonAbbrechen->Location = System::Drawing::Point(15, 169);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 29);
      this->buttonAbbrechen->TabIndex = 6;
      this->buttonAbbrechen->Text = L"Abbrechen";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &KmlErzeugungDialog::buttonAbbrechen_Click);
      // 
      // buttonStarten
      // 
      this->buttonStarten->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonStarten->Location = System::Drawing::Point(265, 169);
      this->buttonStarten->Name = L"buttonStarten";
      this->buttonStarten->Size = System::Drawing::Size(75, 29);
      this->buttonStarten->TabIndex = 7;
      this->buttonStarten->Text = L"Starten";
      this->buttonStarten->UseVisualStyleBackColor = true;
      this->buttonStarten->Click += gcnew System::EventHandler(this, &KmlErzeugungDialog::buttonStarten_Click);
      // 
      // saveKmlFileDialog
      // 
      this->saveKmlFileDialog->DefaultExt = L"*.kml|(*.kml)";
      // 
      // comboBoxSelection
      // 
      this->comboBoxSelection->FormattingEnabled = true;
      this->comboBoxSelection->Location = System::Drawing::Point(150, 107);
      this->comboBoxSelection->Name = L"comboBoxSelection";
      this->comboBoxSelection->Size = System::Drawing::Size(190, 21);
      this->comboBoxSelection->TabIndex = 8;
      // 
      // labelSelection
      // 
      this->labelSelection->AutoSize = true;
      this->labelSelection->Location = System::Drawing::Point(150, 135);
      this->labelSelection->Name = L"labelSelection";
      this->labelSelection->Size = System::Drawing::Size(101, 13);
      this->labelSelection->TabIndex = 9;
      this->labelSelection->Text = L"Angezeigtes Thema";
      // 
      // KmlErzeugungDialog
      // 
      this->AcceptButton = this->buttonStarten;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->buttonAbbrechen;
      this->ClientSize = System::Drawing::Size(352, 222);
      this->Controls->Add(this->labelSelection);
      this->Controls->Add(this->comboBoxSelection);
      this->Controls->Add(this->buttonStarten);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->textBoxOffset);
      this->Controls->Add(this->checkBoxAnzeige);
      this->Controls->Add(this->buttonAendern);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->textBoxDateiname);
      this->Name = L"KmlErzeugungDialog";
      this->Text = L"Kml Erzeugung";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  private: System::Void buttonAendern_Click(System::Object^  sender, System::EventArgs^  e)
           {
             saveKmlFileDialog->FileName = textBoxDateiname->Text;
             if ( saveKmlFileDialog->ShowDialog() == Windows::Forms::DialogResult::OK )
               textBoxDateiname->Text = saveKmlFileDialog->FileName;
           }

private: System::Void buttonAbbrechen_Click(System::Object^  sender, System::EventArgs^  e)
         {
           Close();
         }
private: System::Void buttonStarten_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           bool                                    success;
           System::Diagnostics::ProcessStartInfo ^ processStartInfo;
           System::Diagnostics::Process          ^ pProcess;
           string                                  dateiname;
           string                                  selection;
           String                                ^ STR;

           QuConvert::systemStr2stdStr( dateiname,textBoxDateiname->Text );
           m_pVisual->setZOffset ( zOffset );

           STR = dynamic_cast<String^>(comboBoxSelection->SelectedItem);
           QuConvert::systemStr2stdStr ( selection, STR );

           if (  m_pFeatures->isCityGML() )
           {
             success =  m_pVisual->writeCityGMLToKML ( dateiname, Features::lodStufeToEnum( selection ), showFeatureData );
             if ( !success || !checkBoxAnzeige->Checked ) Close();
           }
           else
           if (  m_pFeatures->isBoreholeML() )
           {
             success =  m_pVisual->writeBoreholeMLToKML ( dateiname, selection, showFeatureData );
             if ( !success || !checkBoxAnzeige->Checked ) Close();
           }

           textBoxDateiname->Text = QuConvert::ToString( dateiname );

           try
           {
             pProcess = gcnew System::Diagnostics::Process();
             processStartInfo = gcnew System::Diagnostics::ProcessStartInfo;

             processStartInfo->FileName = kmlViewer;
             processStartInfo->Arguments = String::Concat ( "\"", textBoxDateiname->Text, "\"" );
             pProcess->StartInfo = processStartInfo;
             pProcess->Start();
           }

           catch ( Exception ^ )
           {
             MessageBox::Show ( "KML-Viewer konnte nicht gestartet werden" );
           }


           Close();
        }
private: System::Void textBoxOffset_Leave(System::Object^  sender, System::EventArgs^  e) 
         {
           try
           {
             zOffset = Convert::ToDouble( textBoxOffset->Text );
             textBoxOffset->Text = Convert::ToString( zOffset );
             offsetBackup = textBoxOffset->Text;
          }

           catch ( FormatException ^ )
           {
              textBoxOffset->Text = offsetBackup;
           }
         }

};
