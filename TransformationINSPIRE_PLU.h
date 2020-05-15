#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <fstream>

#include "XPlanGML.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "GmlSchema.h"
#include "KonversionINSPIRE_PLU.h"
#include "Visualisierung_2D.h"
#include "Convert.h"
#include "Constant.h"
#include "Logfile.h"
#include "includes.h"


	/// <summary>
	/// Transformation einer XPlanGML Datei nach INSPIRE PLU
	///
	/// </summary>
	public ref class TransformationINSPIRE_PLU_Dialog : public System::Windows::Forms::Form
	{ 
    
  public:
		TransformationINSPIRE_PLU_Dialog( XPlanGML * pXPlanGML, Visualisierung_2D * pVisual, String ^versionP )
		{
      GmlSchema * pGmlSchemaINSPIRE;
      bool         success;

			InitializeComponent();

      m_pXPlanGML         = pXPlanGML;
      m_pVisual           = pVisual;
      m_pUOMs             = m_pXPlanGML->getUOMList();
      m_pSrsmanager       = m_pXPlanGML->getFeatures()->getSrsManager();
      m_pLogfile          = m_pXPlanGML->getLogfile();

      pProtokoll          = NULL;
      iexplorerProcess    = nullptr;
      transformiert       = false;
      version             = versionP;

      pProtokollDatei = nullptr;
      pZielOrdner     = nullptr;
      pZielDatei      = nullptr;
      pQuellDatei     = nullptr;
      pQuellOrdner    = nullptr;

      pPlanObjekteINSPIRE = new Features( m_pUOMs, m_pSrsmanager, m_pLogfile );
      pPlanObjekteINSPIRE->setSrsDimension( 2 );

      pGmlSchemaINSPIRE = new GmlSchema (  pPlanObjekteINSPIRE );
      success =  pGmlSchemaINSPIRE->readGmlSchema( INSPIRE_LU_PLANNED, NULL );
      if ( success )
        pPlanObjekteINSPIRE->setGmlSchema ( pGmlSchemaINSPIRE );
      else
      {
        delete pGmlSchemaINSPIRE;
        pGmlSchemaINSPIRE = NULL;
      }

      if ( versionP->CompareTo( "XPlanGML 4.1") == 0 )
        versionXPlanGML = XPLANGML_4_1;
      else
      if ( versionP->CompareTo( "XPlanGML 5.0") == 0 )
        versionXPlanGML = XPLANGML_5_0;
      else
      if ( versionP->CompareTo( "XPlanGML 5.1") == 0 )
        versionXPlanGML = XPLANGML_5_1;
      else
      if ( versionP->CompareTo( "XPlanGML 5.2") == 0 )
        versionXPlanGML = XPLANGML_5_2;

      pTransformation = m_pXPlanGML->readINSPIRE_PLUTransformation( versionXPlanGML );

      outputFormat = 1;  // gml:FeatureCollection
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~TransformationINSPIRE_PLU_Dialog()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxQuelle;
  private: System::Windows::Forms::Button^  buttonDateiAuswahl;
  private: System::Windows::Forms::GroupBox^  groupBox1;
  private: System::Windows::Forms::TextBox^  textBoxZielOrdner;
  private: System::Windows::Forms::Button^  buttonAuswahlZielordner;
  private: System::Windows::Forms::TextBox^  textBoxProtokollDatei;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::Button^  buttonTransformieren;


  private: 
    XPlanGML                            * m_pXPlanGML;					// XPlanGML-Modul
    Visualisierung_2D                   * m_pVisual;						// Modul für die 2D-Visualisierung mit SVG 
    UOMList                             * m_pUOMs;							// Modul Units of Measurement
    SrsManager                          * m_pSrsmanager;				// Modul Koordinaten-Transformation
    Logfile                             * m_pLogfile;						// Logfile
    Features                            * pPlanObjekteINSPIRE;  // Aggregation der erzeugten INSPIRE PLU Objekte
    TransformationINSPIRE_PLU_Parameter * pTransformation;			// Transformations-Regeln XPlanGML --> INSPIRE PLU
    TransformationINSPIREProtokoll      * pProtokoll;						// Transformations-Protokoll
    int                                   outputFormat;					// Struktur der FeatureCollection für INSPIRE Features
    String                              ^ version;							// XPlanGML-Version als Text
    GML_SCHEMA_TYPE                       versionXPlanGML;			// XPlanGML-Version
    System::Diagnostics::Process        ^ iexplorerProcess;     // HTML-Viewer Prozess zur Anzeige des Transformations-Protokolls
    bool																	transformiert;				// Gibt an, ob die Transformation durchgeführt wurde
    String															^ pProtokollDatei;			// Pfadname Protokoll-Datei
    String															^ pZielOrdner;					// Ordner zur Speicherung der INSPIRE PLU Datei
    String															^ pQuellOrdner;					// Ordner der zu transformierenden XPlanGML-Datei
    String															^ pQuellDatei;					// Pfadname der XPlanGML-Datei
    String															^ pZielDatei;						// Pfadname der erzeugten INSPIRE PLU Datei

  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::TextBox^  textBoxTransformiertAnz;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::TextBox^  textBoxNichtTransformiertAnz;
  private: System::Windows::Forms::Label^  label6;
  private: System::Windows::Forms::TextBox^  textBoxZielDatei;
  private: System::Windows::Forms::Label^  label5;
  private: System::Windows::Forms::Button^  buttonErgebnisZeigen;
  private: System::Windows::Forms::Button^  buttonProtokollZeigen;
  private: System::Windows::Forms::GroupBox^  groupBox2;

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
      this->textBoxQuelle = (gcnew System::Windows::Forms::TextBox());
      this->buttonDateiAuswahl = (gcnew System::Windows::Forms::Button());
      this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
      this->buttonErgebnisZeigen = (gcnew System::Windows::Forms::Button());
      this->buttonProtokollZeigen = (gcnew System::Windows::Forms::Button());
      this->label6 = (gcnew System::Windows::Forms::Label());
      this->textBoxZielDatei = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBoxProtokollDatei = (gcnew System::Windows::Forms::TextBox());
      this->textBoxZielOrdner = (gcnew System::Windows::Forms::TextBox());
      this->buttonAuswahlZielordner = (gcnew System::Windows::Forms::Button());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->buttonTransformieren = (gcnew System::Windows::Forms::Button());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->textBoxTransformiertAnz = (gcnew System::Windows::Forms::TextBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->textBoxNichtTransformiertAnz = (gcnew System::Windows::Forms::TextBox());
      this->label5 = (gcnew System::Windows::Forms::Label());
      this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
      this->groupBox1->SuspendLayout();
      this->groupBox2->SuspendLayout();
      this->SuspendLayout();
      // 
      // textBoxQuelle
      // 
      this->textBoxQuelle->Location = System::Drawing::Point(12, 13);
      this->textBoxQuelle->Name = L"textBoxQuelle";
      this->textBoxQuelle->ReadOnly = true;
      this->textBoxQuelle->Size = System::Drawing::Size(310, 20);
      this->textBoxQuelle->TabIndex = 0;
      // 
      // buttonDateiAuswahl
      // 
      this->buttonDateiAuswahl->Location = System::Drawing::Point(328, 12);
      this->buttonDateiAuswahl->Name = L"buttonDateiAuswahl";
      this->buttonDateiAuswahl->Size = System::Drawing::Size(75, 23);
      this->buttonDateiAuswahl->TabIndex = 1;
      this->buttonDateiAuswahl->Text = L"Auswahl";
      this->buttonDateiAuswahl->UseVisualStyleBackColor = true;
      this->buttonDateiAuswahl->Click += gcnew System::EventHandler(this, &TransformationINSPIRE_PLU_Dialog::buttonDateiAuswahl_Click);
      // 
      // groupBox1
      // 
      this->groupBox1->Controls->Add(this->buttonErgebnisZeigen);
      this->groupBox1->Controls->Add(this->buttonProtokollZeigen);
      this->groupBox1->Controls->Add(this->label6);
      this->groupBox1->Controls->Add(this->textBoxZielDatei);
      this->groupBox1->Controls->Add(this->label1);
      this->groupBox1->Controls->Add(this->textBoxProtokollDatei);
      this->groupBox1->Controls->Add(this->textBoxZielOrdner);
      this->groupBox1->Controls->Add(this->buttonAuswahlZielordner);
      this->groupBox1->Location = System::Drawing::Point(12, 70);
      this->groupBox1->Name = L"groupBox1";
      this->groupBox1->Size = System::Drawing::Size(393, 132);
      this->groupBox1->TabIndex = 2;
      this->groupBox1->TabStop = false;
      this->groupBox1->Text = L"Ziel";
      // 
      // buttonErgebnisZeigen
      // 
      this->buttonErgebnisZeigen->Location = System::Drawing::Point(324, 51);
      this->buttonErgebnisZeigen->Name = L"buttonErgebnisZeigen";
      this->buttonErgebnisZeigen->Size = System::Drawing::Size(60, 23);
      this->buttonErgebnisZeigen->TabIndex = 13;
      this->buttonErgebnisZeigen->Text = L"Zeigen";
      this->buttonErgebnisZeigen->UseVisualStyleBackColor = true;
      this->buttonErgebnisZeigen->Click += gcnew System::EventHandler(this, &TransformationINSPIRE_PLU_Dialog::buttonErgebnisZeigen_Click);
      // 
      // buttonProtokollZeigen
      // 
      this->buttonProtokollZeigen->Location = System::Drawing::Point(324, 81);
      this->buttonProtokollZeigen->Name = L"buttonProtokollZeigen";
      this->buttonProtokollZeigen->Size = System::Drawing::Size(60, 23);
      this->buttonProtokollZeigen->TabIndex = 12;
      this->buttonProtokollZeigen->Text = L"Zeigen";
      this->buttonProtokollZeigen->UseVisualStyleBackColor = true;
      this->buttonProtokollZeigen->Click += gcnew System::EventHandler(this, &TransformationINSPIRE_PLU_Dialog::buttonProtokollZeigen_Click);
      // 
      // label6
      // 
      this->label6->AutoSize = true;
      this->label6->Location = System::Drawing::Point(201, 56);
      this->label6->Name = L"label6";
      this->label6->Size = System::Drawing::Size(102, 13);
      this->label6->TabIndex = 5;
      this->label6->Text = L"INSPIRE PLU Datei";
      // 
      // textBoxZielDatei
      // 
      this->textBoxZielDatei->Location = System::Drawing::Point(4, 49);
      this->textBoxZielDatei->Name = L"textBoxZielDatei";
      this->textBoxZielDatei->Size = System::Drawing::Size(191, 20);
      this->textBoxZielDatei->TabIndex = 4;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(195, 86);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(126, 13);
      this->label1->TabIndex = 3;
      this->label1->Text = L"Transformations-Protokoll";
      // 
      // textBoxProtokollDatei
      // 
      this->textBoxProtokollDatei->Location = System::Drawing::Point(4, 83);
      this->textBoxProtokollDatei->Name = L"textBoxProtokollDatei";
      this->textBoxProtokollDatei->Size = System::Drawing::Size(188, 20);
      this->textBoxProtokollDatei->TabIndex = 2;
      // 
      // textBoxZielOrdner
      // 
      this->textBoxZielOrdner->Location = System::Drawing::Point(6, 22);
      this->textBoxZielOrdner->Name = L"textBoxZielOrdner";
      this->textBoxZielOrdner->ReadOnly = true;
      this->textBoxZielOrdner->Size = System::Drawing::Size(297, 20);
      this->textBoxZielOrdner->TabIndex = 1;
      // 
      // buttonAuswahlZielordner
      // 
      this->buttonAuswahlZielordner->Location = System::Drawing::Point(309, 20);
      this->buttonAuswahlZielordner->Name = L"buttonAuswahlZielordner";
      this->buttonAuswahlZielordner->Size = System::Drawing::Size(75, 23);
      this->buttonAuswahlZielordner->TabIndex = 0;
      this->buttonAuswahlZielordner->Text = L"Ordner";
      this->buttonAuswahlZielordner->UseVisualStyleBackColor = true;
      this->buttonAuswahlZielordner->Click += gcnew System::EventHandler(this, &TransformationINSPIRE_PLU_Dialog::buttonAuswahlZielordner_Click);
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonBeenden->Location = System::Drawing::Point(12, 341);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 3;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &TransformationINSPIRE_PLU_Dialog::buttonBeenden_Click);
      // 
      // buttonTransformieren
      // 
      this->buttonTransformieren->Location = System::Drawing::Point(308, 321);
      this->buttonTransformieren->Name = L"buttonTransformieren";
      this->buttonTransformieren->Size = System::Drawing::Size(95, 43);
      this->buttonTransformieren->TabIndex = 4;
      this->buttonTransformieren->Text = L"Transformieren";
      this->buttonTransformieren->UseVisualStyleBackColor = true;
      this->buttonTransformieren->Click += gcnew System::EventHandler(this, &TransformationINSPIRE_PLU_Dialog::buttonTransformieren_Click);
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(6, 26);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(206, 13);
      this->label2->TabIndex = 5;
      this->label2->Text = L"Anzahl transformierte XPlanGML-Features:";
      // 
      // textBoxTransformiertAnz
      // 
      this->textBoxTransformiertAnz->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->textBoxTransformiertAnz->ForeColor = System::Drawing::Color::Red;
      this->textBoxTransformiertAnz->Location = System::Drawing::Point(270, 19);
      this->textBoxTransformiertAnz->Name = L"textBoxTransformiertAnz";
      this->textBoxTransformiertAnz->ReadOnly = true;
      this->textBoxTransformiertAnz->Size = System::Drawing::Size(70, 20);
      this->textBoxTransformiertAnz->TabIndex = 6;
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(6, 51);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(232, 13);
      this->label3->TabIndex = 7;
      this->label3->Text = L"Anzahl nicht transformierte XPlanGML-Features:";
      // 
      // textBoxNichtTransformiertAnz
      // 
      this->textBoxNichtTransformiertAnz->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->textBoxNichtTransformiertAnz->ForeColor = System::Drawing::Color::Red;
      this->textBoxNichtTransformiertAnz->Location = System::Drawing::Point(269, 48);
      this->textBoxNichtTransformiertAnz->Name = L"textBoxNichtTransformiertAnz";
      this->textBoxNichtTransformiertAnz->ReadOnly = true;
      this->textBoxNichtTransformiertAnz->Size = System::Drawing::Size(69, 20);
      this->textBoxNichtTransformiertAnz->TabIndex = 8;
      // 
      // label5
      // 
      this->label5->AutoSize = true;
      this->label5->Location = System::Drawing::Point(12, 40);
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size(192, 13);
      this->label5->TabIndex = 11;
      this->label5->Text = L"Zu transformierende XPlanGML 4 Datei";
      // 
      // groupBox2
      // 
      this->groupBox2->Controls->Add(this->label2);
      this->groupBox2->Controls->Add(this->textBoxNichtTransformiertAnz);
      this->groupBox2->Controls->Add(this->label3);
      this->groupBox2->Controls->Add(this->textBoxTransformiertAnz);
      this->groupBox2->Location = System::Drawing::Point(12, 214);
      this->groupBox2->Name = L"groupBox2";
      this->groupBox2->Size = System::Drawing::Size(393, 78);
      this->groupBox2->TabIndex = 12;
      this->groupBox2->TabStop = false;
      this->groupBox2->Text = L"Ergebnis";
      // 
      // TransformationINSPIRE_PLU
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(410, 380);
      this->Controls->Add(this->label5);
      this->Controls->Add(this->buttonTransformieren);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->groupBox1);
      this->Controls->Add(this->buttonDateiAuswahl);
      this->Controls->Add(this->textBoxQuelle);
      this->Controls->Add(this->groupBox2);
      this->Name = L"TransformationINSPIRE_PLU";
      this->Text = L"Transformation XPlanGML nach INSPIRE PLU";
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
//  Auswahl der zu transformierenden XPlanGML Datei													 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonDateiAuswahl_Click(System::Object^  sender, System::EventArgs^  e)
{
  Windows::Forms::OpenFileDialog ^ pQuellDateiDialog;
  Windows::Forms::DialogResult     ok;
  String                         ^ pQuellDateiName;
  String                         ^ pQuellDateiOhneExtension;
  String                         ^ pQuellDateiExtension;
  int                              index;

  pQuellDateiDialog = gcnew Windows::Forms::OpenFileDialog();
  pQuellDateiDialog->FileName = textBoxQuelle->Text;
  ok = pQuellDateiDialog->ShowDialog();
  if ( ok == Windows::Forms::DialogResult::OK )
  {
    textBoxQuelle->Text = pQuellDateiDialog->FileName;
    pQuellDatei = textBoxQuelle->Text;

    index = pQuellDatei->LastIndexOf( "\\");
    pQuellOrdner = pQuellDatei->Substring( 0, index+1 );
    pQuellDateiName = pQuellDatei->Substring( index+1, pQuellDatei->Length - (index+1) );

    index = pQuellDateiName->LastIndexOf( ".");
    pQuellDateiOhneExtension = pQuellDateiName->Substring( 0, index );
    pQuellDateiExtension     = pQuellDateiName->Substring ( index, pQuellDateiName->Length - index );

    if ( textBoxZielOrdner->Text == "" )
    {
      pZielOrdner = pQuellOrdner;
      textBoxZielOrdner->Text = pQuellOrdner;
    }

    pZielDatei = String::Concat( pZielOrdner, pQuellDateiOhneExtension, "_plu", pQuellDateiExtension );
    textBoxZielDatei->Text = String::Concat( pQuellDateiOhneExtension, "_plu", pQuellDateiExtension );

    pProtokollDatei = String::Concat( pZielOrdner, pQuellDateiOhneExtension, "_protokoll.xml" );
    textBoxProtokollDatei->Text = String::Concat( pQuellDateiOhneExtension, "_protokoll.xml" );

    textBoxTransformiertAnz->Text      = nullptr;
    textBoxNichtTransformiertAnz->Text = nullptr;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Ordner"																					 //
//  Auswahl des Ordners der erzeugten INSPIRE PLU Datei											 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAuswahlZielordner_Click(System::Object^  sender, System::EventArgs^  e)
{
  Windows::Forms::FolderBrowserDialog ^ pZielFolderDialog;
  Windows::Forms::DialogResult ok;

  pZielFolderDialog = gcnew Windows::Forms::FolderBrowserDialog();
  pZielFolderDialog->SelectedPath = textBoxZielOrdner->Text;
  ok = pZielFolderDialog->ShowDialog();
  if ( ok == Windows::Forms::DialogResult::OK )
  {
    textBoxZielOrdner->Text = pZielFolderDialog->SelectedPath;
    pZielOrdner = String::Concat ( textBoxZielOrdner->Text, "\\" );
  }
 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"																					 //
//  Schließt den Dialog																											 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
{
  delete pTransformation;
  delete pPlanObjekteINSPIRE;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Transformieren"																	 //
//  Führt die Transformation durch																					 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonTransformieren_Click(System::Object^  sender, System::EventArgs^  e)
{
  string quellDatei;
  string zielDatei;
  string protokollDatei;
  int    objektAnz = 0;

  if ( pQuellDatei == nullptr )
  {
    MessageBox::Show( "Keine zu transformierende Datei spezifiziert" );
    return;
  }
  else
    QuConvert::systemStr2stdStr( quellDatei, pQuellDatei );

  if ( textBoxZielDatei->Text == nullptr )
  {
    MessageBox::Show( "Keine Zieldatei spezifiziert" );
    return;
  }
  else
  {
    pZielDatei = String::Concat( pZielOrdner, textBoxZielDatei->Text );
    QuConvert::systemStr2stdStr( zielDatei, pZielDatei );
  }

  if ( textBoxProtokollDatei == nullptr )
  {
    MessageBox::Show( "Keine Protokolldatei spezifiziert" );
    return;
  }
  else
  {
    pProtokollDatei = String::Concat( pZielOrdner, textBoxProtokollDatei->Text );
    QuConvert::systemStr2stdStr( protokollDatei, pProtokollDatei );
  }

  if ( pProtokoll != NULL )
  {
    delete pProtokoll;
    pProtokoll = NULL;
  }

  pPlanObjekteINSPIRE->clear();
  pPlanObjekteINSPIRE->setSrsDimension( 2 );

  Cursor = Cursors::WaitCursor;
  pProtokoll = m_pXPlanGML->transformationINSPIRE_PLU ( quellDatei, zielDatei, pPlanObjekteINSPIRE, pTransformation, outputFormat, versionXPlanGML );
  Cursor = Cursors::Default;
  transformiert = true;

  if ( pProtokoll != NULL )
  {
    pProtokoll->writeProtokoll ( protokollDatei );

    textBoxTransformiertAnz->Text         = Convert::ToString ( pProtokoll->anzFeaturesTransformiert );
    textBoxNichtTransformiertAnz->Text    = Convert::ToString ( pProtokoll->anzFeaturesNichtTransformiert );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Zeigen" (INSPIRE PLU Datei)											 //
//  Visualisiert die erzeugte INSPIRE PLU Datei															 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonErgebnisZeigen_Click(System::Object^  sender, System::EventArgs^  e)
{
  System::Diagnostics::ProcessStartInfo ^  processStartInfo;
  String                                ^  explorerDatei;
  bool                                     success;

  if ( !transformiert )
  {
    MessageBox::Show( "Es wurde noch keine INSPIRE PLU Datei erzeugt" );
    return;
  }

  try
  {
    if ( iexplorerProcess != nullptr )	iexplorerProcess->CloseMainWindow();
  }
  catch ( System::InvalidOperationException^ )
  {
  }


  success = m_pVisual->generateObjectData ( false );
  success =  m_pVisual->writeInteractiveSVG( 2, false );
  if ( !success ) return;

  explorerDatei = QuConvert::ToString(  Constant::getHTMLFramePath ( pPlanObjekteINSPIRE->getGmlTyp() ) );

  iexplorerProcess = gcnew System::Diagnostics::Process();
  processStartInfo = gcnew System::Diagnostics::ProcessStartInfo;

  processStartInfo->FileName = explorerDatei;
  iexplorerProcess->StartInfo = processStartInfo;
  iexplorerProcess->Start();

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Zeigen" (Transformations-Protokoll)							 //
//  Visualisiert das Transformations-Protokoll															 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonProtokollZeigen_Click(System::Object^  sender, System::EventArgs^  e)
{
  System::Diagnostics::ProcessStartInfo ^  processStartInfo;

  if ( !transformiert )
  {
    MessageBox::Show( "Es wurde noch keine INSPIRE PLU Datei erzeugt" );
    return;
  }

  try
  {
    if ( iexplorerProcess != nullptr )	iexplorerProcess->CloseMainWindow();
  }
  catch ( System::InvalidOperationException^ )
  {
  }

  iexplorerProcess = gcnew System::Diagnostics::Process();
  processStartInfo = gcnew System::Diagnostics::ProcessStartInfo;

  processStartInfo->FileName = pProtokollDatei;
  iexplorerProcess->StartInfo = processStartInfo;
  iexplorerProcess->Start();

}
};

