#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "XPlanGML.h"
#include "Convert.h"
#include "Migration.h"

using namespace std;

	/// <summary>
	/// Migration von XPlanGML Daten auf eine neuere Version
	///
	/// </summary>
	public ref class XPlanGML_Migration : public System::Windows::Forms::Form
	{
	public:
		XPlanGML_Migration( XPlanGML * pXPlanGML, String ^ quellDatei, String ^ versionQuelleP, String ^ versionZielP,
												String ^ migrationsFehlerStylesheetP )
		{
			InitializeComponent();

      m_pXPlanGML         = pXPlanGML;
      versionQuelle       = versionQuelleP;
      versionZiel         = versionZielP;
			migrationStylesheet = migrationsFehlerStylesheetP;

      this->Text = String::Concat ( "Migration XPlanung ", versionQuelle,  " nach ", versionZiel );

      if ( quellDatei->CompareTo( "" ) != 0 )
      {
        textBoxQuelle->Text = quellDatei;
        int index = textBoxQuelle->Text->LastIndexOf( "\\");
        pQuellOrdner = textBoxQuelle->Text->Substring( 0, index );
        textBoxZielOrdner->Text = pQuellOrdner;
      }
      else
      {
        textBoxQuelle->Text         = "";
        textBoxZielOrdner->Text     = "";
      }

      abfrageUeberschreiben = true;

      textBoxProtokollDatei->Text = "MigrationsProtokoll.xml";
      pProtokolldatei             = nullptr;

      checkBoxUeberschreiben->Checked  = false;
      checkBoxShowErrorReport->Checked = true;
      radioButtonDatei->Checked        = true;
      radioButtonOrdner->Checked       = false;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~XPlanGML_Migration()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::GroupBox^  groupBox1;
  private: System::Windows::Forms::RadioButton^  radioButtonOrdner;
  private: System::Windows::Forms::TextBox^  textBoxQuelle;
  private: System::Windows::Forms::RadioButton^  radioButtonDatei;
  private: System::Windows::Forms::Button^  buttonAuswahlQuelle;
  private: System::Windows::Forms::GroupBox^  groupBox2;
  private: System::Windows::Forms::TextBox^  textBoxZielOrdner;
  private: System::Windows::Forms::Button^  buttonAuswahlZielOrdner;
  private: System::Windows::Forms::TextBox^  textBoxProtokollDatei;
  private: System::Windows::Forms::CheckBox^  checkBoxUeberschreiben;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Button^  buttonAbbrechen;
  private: System::Windows::Forms::Button^  buttonTransformieren;
  private: System::Windows::Forms::TextBox^  textBoxErgebnis;
  private: System::Windows::Forms::CheckBox^  checkBoxShowErrorReport;

//  public: 
	private:
    String   ^ pProtokolldatei;				// Pfadname des Migrations-Protokolls
    String   ^ migrationStylesheet;		// Stylesheet für die Erzeugung des HTML-Migrationsprotokolls
    String   ^ versionZiel;						// XPlanGML-Version der Ausgangsdatei
    String   ^ versionQuelle;					// XPlanGML-Version der migrierten Datei
    XPlanGML * m_pXPlanGML;						// XPlanGML-Modul
    String   ^ pQuellOrdner;					// Ordner der XPlanGML Ausgangsdatei
    bool       abfrageUeberschreiben; // Abfrage, ob die Ausgangsdatei überschrieben werden soll

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
      this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
      this->buttonAuswahlQuelle = (gcnew System::Windows::Forms::Button());
      this->textBoxQuelle = (gcnew System::Windows::Forms::TextBox());
      this->radioButtonDatei = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonOrdner = (gcnew System::Windows::Forms::RadioButton());
      this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
      this->checkBoxShowErrorReport = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxUeberschreiben = (gcnew System::Windows::Forms::CheckBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBoxProtokollDatei = (gcnew System::Windows::Forms::TextBox());
      this->buttonAuswahlZielOrdner = (gcnew System::Windows::Forms::Button());
      this->textBoxZielOrdner = (gcnew System::Windows::Forms::TextBox());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->buttonTransformieren = (gcnew System::Windows::Forms::Button());
      this->textBoxErgebnis = (gcnew System::Windows::Forms::TextBox());
      this->groupBox1->SuspendLayout();
      this->groupBox2->SuspendLayout();
      this->SuspendLayout();
      // 
      // groupBox1
      // 
      this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBox1->Controls->Add(this->buttonAuswahlQuelle);
      this->groupBox1->Controls->Add(this->textBoxQuelle);
      this->groupBox1->Controls->Add(this->radioButtonDatei);
      this->groupBox1->Controls->Add(this->radioButtonOrdner);
      this->groupBox1->Location = System::Drawing::Point(17, 15);
      this->groupBox1->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->groupBox1->Name = L"groupBox1";
      this->groupBox1->Padding = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->groupBox1->Size = System::Drawing::Size(581, 122);
      this->groupBox1->TabIndex = 0;
      this->groupBox1->TabStop = false;
      this->groupBox1->Text = L"Quelle";
      // 
      // buttonAuswahlQuelle
      // 
      this->buttonAuswahlQuelle->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonAuswahlQuelle->Location = System::Drawing::Point(469, 69);
      this->buttonAuswahlQuelle->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->buttonAuswahlQuelle->Name = L"buttonAuswahlQuelle";
      this->buttonAuswahlQuelle->Size = System::Drawing::Size(100, 28);
      this->buttonAuswahlQuelle->TabIndex = 3;
      this->buttonAuswahlQuelle->Text = L"Auswahl";
      this->buttonAuswahlQuelle->UseVisualStyleBackColor = true;
      this->buttonAuswahlQuelle->Click += gcnew System::EventHandler(this, &XPlanGML_Migration::buttonAuswahlQuelle_Click);
      // 
      // textBoxQuelle
      // 
      this->textBoxQuelle->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxQuelle->Location = System::Drawing::Point(5, 73);
      this->textBoxQuelle->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->textBoxQuelle->Name = L"textBoxQuelle";
      this->textBoxQuelle->Size = System::Drawing::Size(455, 22);
      this->textBoxQuelle->TabIndex = 2;
      // 
      // radioButtonDatei
      // 
      this->radioButtonDatei->AutoSize = true;
      this->radioButtonDatei->Location = System::Drawing::Point(156, 23);
      this->radioButtonDatei->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->radioButtonDatei->Name = L"radioButtonDatei";
      this->radioButtonDatei->Size = System::Drawing::Size(62, 21);
      this->radioButtonDatei->TabIndex = 1;
      this->radioButtonDatei->TabStop = true;
      this->radioButtonDatei->Text = L"Datei";
      this->radioButtonDatei->UseVisualStyleBackColor = true;
      // 
      // radioButtonOrdner
      // 
      this->radioButtonOrdner->AutoSize = true;
      this->radioButtonOrdner->Location = System::Drawing::Point(9, 25);
      this->radioButtonOrdner->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->radioButtonOrdner->Name = L"radioButtonOrdner";
      this->radioButtonOrdner->Size = System::Drawing::Size(74, 21);
      this->radioButtonOrdner->TabIndex = 0;
      this->radioButtonOrdner->TabStop = true;
      this->radioButtonOrdner->Text = L"Ordner";
      this->radioButtonOrdner->UseVisualStyleBackColor = true;
      // 
      // groupBox2
      // 
      this->groupBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBox2->Controls->Add(this->checkBoxShowErrorReport);
      this->groupBox2->Controls->Add(this->checkBoxUeberschreiben);
      this->groupBox2->Controls->Add(this->label1);
      this->groupBox2->Controls->Add(this->textBoxProtokollDatei);
      this->groupBox2->Controls->Add(this->buttonAuswahlZielOrdner);
      this->groupBox2->Controls->Add(this->textBoxZielOrdner);
      this->groupBox2->Location = System::Drawing::Point(17, 161);
      this->groupBox2->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->groupBox2->Name = L"groupBox2";
      this->groupBox2->Padding = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->groupBox2->Size = System::Drawing::Size(581, 142);
      this->groupBox2->TabIndex = 1;
      this->groupBox2->TabStop = false;
      this->groupBox2->Text = L"Ziel";
      // 
      // checkBoxShowErrorReport
      // 
      this->checkBoxShowErrorReport->AutoSize = true;
      this->checkBoxShowErrorReport->Location = System::Drawing::Point(280, 106);
      this->checkBoxShowErrorReport->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->checkBoxShowErrorReport->Name = L"checkBoxShowErrorReport";
      this->checkBoxShowErrorReport->Size = System::Drawing::Size(141, 21);
      this->checkBoxShowErrorReport->TabIndex = 6;
      this->checkBoxShowErrorReport->Text = L"Fehlerliste zeigen";
      this->checkBoxShowErrorReport->UseVisualStyleBackColor = true;
      // 
      // checkBoxUeberschreiben
      // 
      this->checkBoxUeberschreiben->AutoSize = true;
      this->checkBoxUeberschreiben->Location = System::Drawing::Point(9, 106);
      this->checkBoxUeberschreiben->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->checkBoxUeberschreiben->Name = L"checkBoxUeberschreiben";
      this->checkBoxUeberschreiben->Size = System::Drawing::Size(256, 21);
      this->checkBoxUeberschreiben->TabIndex = 5;
      this->checkBoxUeberschreiben->Text = L"Vorhandene Dateien überschreiben";
      this->checkBoxUeberschreiben->UseVisualStyleBackColor = true;
      // 
      // label1
      // 
      this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(472, 82);
      this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(63, 17);
      this->label1->TabIndex = 4;
      this->label1->Text = L"Protokoll";
      // 
      // textBoxProtokollDatei
      // 
      this->textBoxProtokollDatei->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxProtokollDatei->Location = System::Drawing::Point(8, 74);
      this->textBoxProtokollDatei->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->textBoxProtokollDatei->Name = L"textBoxProtokollDatei";
      this->textBoxProtokollDatei->Size = System::Drawing::Size(455, 22);
      this->textBoxProtokollDatei->TabIndex = 3;
      // 
      // buttonAuswahlZielOrdner
      // 
      this->buttonAuswahlZielOrdner->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonAuswahlZielOrdner->Location = System::Drawing::Point(469, 34);
      this->buttonAuswahlZielOrdner->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->buttonAuswahlZielOrdner->Name = L"buttonAuswahlZielOrdner";
      this->buttonAuswahlZielOrdner->Size = System::Drawing::Size(100, 25);
      this->buttonAuswahlZielOrdner->TabIndex = 2;
      this->buttonAuswahlZielOrdner->Text = L"Ordner";
      this->buttonAuswahlZielOrdner->UseVisualStyleBackColor = true;
      this->buttonAuswahlZielOrdner->Click += gcnew System::EventHandler(this, &XPlanGML_Migration::buttonAuswahlZielOrdner_Click);
      // 
      // textBoxZielOrdner
      // 
      this->textBoxZielOrdner->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxZielOrdner->Location = System::Drawing::Point(7, 34);
      this->textBoxZielOrdner->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->textBoxZielOrdner->Name = L"textBoxZielOrdner";
      this->textBoxZielOrdner->ReadOnly = true;
      this->textBoxZielOrdner->Size = System::Drawing::Size(455, 22);
      this->textBoxZielOrdner->TabIndex = 0;
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonAbbrechen->Location = System::Drawing::Point(17, 454);
      this->buttonAbbrechen->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(100, 28);
      this->buttonAbbrechen->TabIndex = 2;
      this->buttonAbbrechen->Text = L"Beenden";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &XPlanGML_Migration::buttonAbbrechen_Click);
      // 
      // buttonTransformieren
      // 
      this->buttonTransformieren->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonTransformieren->Location = System::Drawing::Point(465, 437);
      this->buttonTransformieren->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->buttonTransformieren->Name = L"buttonTransformieren";
      this->buttonTransformieren->Size = System::Drawing::Size(148, 46);
      this->buttonTransformieren->TabIndex = 3;
      this->buttonTransformieren->Text = L"Transformieren";
      this->buttonTransformieren->UseVisualStyleBackColor = true;
      this->buttonTransformieren->Click += gcnew System::EventHandler(this, &XPlanGML_Migration::buttonTransformieren_Click);
      // 
      // textBoxErgebnis
      // 
      this->textBoxErgebnis->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
        | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxErgebnis->ForeColor = System::Drawing::Color::Red;
      this->textBoxErgebnis->Location = System::Drawing::Point(17, 310);
      this->textBoxErgebnis->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->textBoxErgebnis->Multiline = true;
      this->textBoxErgebnis->Name = L"textBoxErgebnis";
      this->textBoxErgebnis->ReadOnly = true;
      this->textBoxErgebnis->Size = System::Drawing::Size(583, 109);
      this->textBoxErgebnis->TabIndex = 4;
      // 
      // XPlanGML_Migration
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(617, 492);
      this->Controls->Add(this->textBoxErgebnis);
      this->Controls->Add(this->buttonTransformieren);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->groupBox2);
      this->Controls->Add(this->groupBox1);
      this->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
      this->Name = L"XPlanGML_Migration";
      this->Text = L"Migration XPlanGML 3.0 nach 4.0";
      this->groupBox1->ResumeLayout(false);
      this->groupBox1->PerformLayout();
      this->groupBox2->ResumeLayout(false);
      this->groupBox2->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Auswahl"																					 //
//  Festlegen der zu migrierenden Datei(en)																   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAuswahlQuelle_Click(System::Object^  sender, System::EventArgs^  e)
{
  Windows::Forms::OpenFileDialog      ^ pQuellDateiDialog;
  Windows::Forms::FolderBrowserDialog ^ pQuellFolderDialog;
  Windows::Forms::DialogResult          ok;

  if ( radioButtonDatei->Checked == true )
  {
    pQuellDateiDialog = gcnew Windows::Forms::OpenFileDialog();
    pQuellDateiDialog->FileName = textBoxQuelle->Text;
    ok = pQuellDateiDialog->ShowDialog();
    if ( ok == Windows::Forms::DialogResult::OK )
    {
      textBoxQuelle->Text = pQuellDateiDialog->FileName;
      int index = textBoxQuelle->Text->LastIndexOf( "\\");
      pQuellOrdner = textBoxQuelle->Text->Substring( 0, index );

      if ( textBoxZielOrdner->Text == "" )
        textBoxZielOrdner->Text = pQuellOrdner;
    }
  }

  if ( radioButtonOrdner->Checked == true )
  {
    pQuellFolderDialog = gcnew Windows::Forms::FolderBrowserDialog();
    pQuellFolderDialog->SelectedPath = textBoxQuelle->Text;
    ok = pQuellFolderDialog->ShowDialog();
    if ( ok == Windows::Forms::DialogResult::OK )
    {
      textBoxQuelle->Text = pQuellFolderDialog->SelectedPath;
      pQuellOrdner = textBoxQuelle->Text;
      if (  textBoxZielOrdner->Text == "" )
        textBoxZielOrdner->Text = pQuellOrdner;
    }
  }
  
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Ordner"																	         //
//  Auswahl des Ordners der migrierten Datei(en)													   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAuswahlZielOrdner_Click(System::Object^  sender, System::EventArgs^  e)
{
  Windows::Forms::FolderBrowserDialog ^ pZielFolderDialog;
  Windows::Forms::DialogResult ok;

  pZielFolderDialog = gcnew Windows::Forms::FolderBrowserDialog();
  pZielFolderDialog->SelectedPath = textBoxZielOrdner->Text;
  ok = pZielFolderDialog->ShowDialog();
  if ( ok == Windows::Forms::DialogResult::OK )
  {
    textBoxZielOrdner->Text = pZielFolderDialog->SelectedPath;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"																					 //
//  Schließt den Dialog																											 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAbbrechen_Click(System::Object^  sender, System::EventArgs^  e)
{

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Transformieren"																	 //
//  Führt die Transformation durch																					 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonTransformieren_Click(System::Object^  sender, System::EventArgs^  e)
{
  string quellDatei;
  string quellOrdner;
  string zielOrdner;
  string protokollDatei;
  string protokollDateiHtml;
  string migrationStylesheetStd;
  int    objektAnz = 0;
  string versionXPlanGMLQuelle;
  string versionXPlanGMLZiel;

  QuConvert::systemStr2stdStr( versionXPlanGMLQuelle, versionQuelle );
  QuConvert::systemStr2stdStr( versionXPlanGMLZiel, versionZiel );

  if ( textBoxQuelle->Text->CompareTo( "" ) == 0 )
  {
    textBoxErgebnis->Text = "Keine Datei oder Ordner gewählt";
    return;
  }

  if ( checkBoxUeberschreiben->Checked == true )
  {
    if ( pQuellOrdner->CompareTo( textBoxZielOrdner->Text ) == 0 && abfrageUeberschreiben )
    {
      Windows::Forms::DialogResult ok;
      ok = MessageBox::Show ( "Sollen die Ausgangsdateien tatsächlich überschrieben werden?", "", System::Windows::Forms::MessageBoxButtons::YesNo );
      if ( ok == Windows::Forms::DialogResult::No )
        checkBoxUeberschreiben->Checked = false;
      abfrageUeberschreiben = false;
    }
  }

  Cursor = Cursors::WaitCursor;

  QuConvert::systemStr2stdStr ( zielOrdner, textBoxZielOrdner->Text );
  pProtokolldatei =  String::Concat( textBoxZielOrdner->Text, "\\", textBoxProtokollDatei->Text );
  QuConvert::systemStr2stdStr( protokollDatei, pProtokolldatei );

  MigrationProtokoll * pMigProtokoll;

  if ( radioButtonDatei->Checked == true )
  {
    QuConvert::systemStr2stdStr( quellDatei, textBoxQuelle->Text );
    pMigProtokoll = m_pXPlanGML->migrationDatei (  quellDatei, zielOrdner, protokollDatei, checkBoxUeberschreiben->Checked, versionXPlanGMLQuelle, versionXPlanGMLZiel );

  }
  else
  if ( radioButtonOrdner->Checked == true )
  {
    QuConvert::systemStr2stdStr( quellOrdner, textBoxQuelle->Text );
    pMigProtokoll = m_pXPlanGML->migrationOrdner ( quellOrdner, zielOrdner, protokollDatei, checkBoxUeberschreiben->Checked, versionXPlanGMLQuelle, versionXPlanGMLZiel );
  }

  Cursor = Cursors::Default;

  if ( pMigProtokoll != NULL )
  {
		size_t objektAnz         = pMigProtokoll->getAnzahlObjekte();
		size_t informationAnz    = pMigProtokoll->getMigrationFehlerAnz ( INFORMATION );
		size_t warnungAnz        = pMigProtokoll->getMigrationFehlerAnz ( WARNUNG );
		size_t fehlerAnz         = pMigProtokoll->getMigrationFehlerAnz ( FEHLER );
		size_t schwererFehlerAnz = pMigProtokoll->getMigrationFehlerAnz ( SCHWERER_FEHLER );

    textBoxErgebnis->Text = String::Concat( "Anzahl der migrierten Objekte:  ", Convert::ToString( (int)objektAnz ), "\r\n", 
                                            "Anzahl der Hinweise:            ",  Convert::ToString( (int)informationAnz ), "\r\n", 
                                            "Anzahl der Warnungen:           ",  Convert::ToString( (int)warnungAnz ), "\r\n", 
                                            "Anzahl der korrigierten Fehler: ",  Convert::ToString( (int)fehlerAnz ), "\r\n", 
                                            "Anzahl der schweren Fehler:     ",  Convert::ToString( (int)schwererFehlerAnz ) );
                                           
   if ( migrationStylesheet != nullptr &&  pMigProtokoll->getMigrationFehlerAnz() > 0 )
   {
     QuConvert::systemStr2stdStr( migrationStylesheetStd, migrationStylesheet );
     protokollDateiHtml = protokollDatei + ".htm";

     Constant::performXsltTransformation ( protokollDatei, protokollDateiHtml, migrationStylesheetStd );
     if ( checkBoxShowErrorReport->Checked )
      Constant::showHtmlFile( protokollDateiHtml );
   }
   delete pMigProtokoll;
  }
  else
    textBoxErgebnis->Text = "Keine Migration möglich";
 
}

};
