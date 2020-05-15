#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Xml;

#include "Visualisierung_2D.h"
#include "SrsManagerRef.h"
#include "Feature.h"

#include <string>

	/// <summary> 
	/// Umwandlung einer SVG-Datei in eine PNG-Datei
	///
	/// </summary>
	public ref class Rasterizer : public System::Windows::Forms::Form
	{
	public: 
		Rasterizer( Visualisierung_2D * pVisual, String ^ svgDateiP, String ^ javaPathP, 
					      String ^ batikPathP, String ^ javaHeapSizeP )
		{
			InitializeComponent();
			
			m_pVisual = pVisual;

			comboBoxFormat->SelectedItem = "PDF";
			comboBoxAufloesung->SelectedItem = "300 dpi";
			comboBoxMasstab->SelectedItem = "1:2000";

			textBoxMasstabFrei->Visible = false;
			labelMasstabFrei->Visible = false;

			svgDatei     = svgDateiP;
			javaPath     = javaPathP;
			batikPath    = batikPathP;
			javaHeapSize = javaHeapSizeP;
		}
        
	protected: 
/*		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
*/
	private: System::Windows::Forms::ComboBox ^  comboBoxFormat;
	private: System::Windows::Forms::Label ^  label1;
	private: System::Windows::Forms::ComboBox ^  comboBoxAufloesung;
	private: System::Windows::Forms::Label ^  label2;
	private: System::Windows::Forms::ComboBox ^  comboBoxMasstab;
	private: System::Windows::Forms::Label ^  label3;
	private: System::Windows::Forms::TextBox ^  textBoxMasstabFrei;
	private: System::Windows::Forms::Label ^  labelMasstabFrei;
	private: System::Windows::Forms::Button ^  buttonAusgabeDatei;
	private: System::Windows::Forms::SaveFileDialog ^  saveFileDialogAusgabeDatei;
	private: System::Windows::Forms::TextBox ^  textBoxErrorMessage;
	private: System::Windows::Forms::Button ^  buttonKonvertieren;
	private: System::Windows::Forms::TextBox ^  textBoxAusgabeDatei;

	private:
		Visualisierung_2D * m_pVisual;
    String            ^ javaPath;
    String            ^ svgDatei;
    String            ^ batikPath;
    String            ^ javaHeapSize;

		System::Diagnostics::Process          ^ rasterizerProcess ;
		System::Diagnostics::ProcessStartInfo ^ processStartInfo;

		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container^ components;

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->comboBoxFormat = (gcnew System::Windows::Forms::ComboBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->comboBoxAufloesung = (gcnew System::Windows::Forms::ComboBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->comboBoxMasstab = (gcnew System::Windows::Forms::ComboBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->textBoxMasstabFrei = (gcnew System::Windows::Forms::TextBox());
      this->labelMasstabFrei = (gcnew System::Windows::Forms::Label());
      this->textBoxAusgabeDatei = (gcnew System::Windows::Forms::TextBox());
      this->buttonAusgabeDatei = (gcnew System::Windows::Forms::Button());
      this->saveFileDialogAusgabeDatei = (gcnew System::Windows::Forms::SaveFileDialog());
      this->buttonKonvertieren = (gcnew System::Windows::Forms::Button());
      this->textBoxErrorMessage = (gcnew System::Windows::Forms::TextBox());
      this->SuspendLayout();
      // 
      // comboBoxFormat
      // 
      this->comboBoxFormat->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"PDF", L"JPG"});
      this->comboBoxFormat->Location = System::Drawing::Point(8, 16);
      this->comboBoxFormat->Name = L"comboBoxFormat";
      this->comboBoxFormat->Size = System::Drawing::Size(121, 21);
      this->comboBoxFormat->TabIndex = 0;
      this->comboBoxFormat->Tag = L"";
      // 
      // label1
      // 
      this->label1->Location = System::Drawing::Point(16, 40);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(100, 16);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Ausgabeformat";
      // 
      // comboBoxAufloesung
      // 
      this->comboBoxAufloesung->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"100 dpi", L"200 dpi", L"300 dpi"});
      this->comboBoxAufloesung->Location = System::Drawing::Point(152, 16);
      this->comboBoxAufloesung->Name = L"comboBoxAufloesung";
      this->comboBoxAufloesung->Size = System::Drawing::Size(72, 21);
      this->comboBoxAufloesung->TabIndex = 2;
      // 
      // label2
      // 
      this->label2->Location = System::Drawing::Point(152, 40);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(100, 23);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Auflösung";
      // 
      // comboBoxMasstab
      // 
      this->comboBoxMasstab->Items->AddRange(gcnew cli::array< System::Object^  >(6) {L"1:500", L"1:1000", L"1:2000", L"1:5000", 
        L"1:10.000", L"Benutzerdefiniert"});
      this->comboBoxMasstab->Location = System::Drawing::Point(240, 16);
      this->comboBoxMasstab->Name = L"comboBoxMasstab";
      this->comboBoxMasstab->Size = System::Drawing::Size(121, 21);
      this->comboBoxMasstab->TabIndex = 4;
      this->comboBoxMasstab->SelectedIndexChanged += gcnew System::EventHandler(this, &Rasterizer::comboBoxMasstab_SelectedIndexChanged);
      // 
      // label3
      // 
      this->label3->Location = System::Drawing::Point(240, 40);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(100, 16);
      this->label3->TabIndex = 5;
      this->label3->Text = L"Maßstab";
      // 
      // textBoxMasstabFrei
      // 
      this->textBoxMasstabFrei->Location = System::Drawing::Point(272, 64);
      this->textBoxMasstabFrei->Name = L"textBoxMasstabFrei";
      this->textBoxMasstabFrei->Size = System::Drawing::Size(100, 20);
      this->textBoxMasstabFrei->TabIndex = 6;
      // 
      // labelMasstabFrei
      // 
      this->labelMasstabFrei->Location = System::Drawing::Point(200, 64);
      this->labelMasstabFrei->Name = L"labelMasstabFrei";
      this->labelMasstabFrei->Size = System::Drawing::Size(64, 16);
      this->labelMasstabFrei->TabIndex = 7;
      this->labelMasstabFrei->Text = L"Maßstab 1 :";
      this->labelMasstabFrei->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
      // 
      // textBoxAusgabeDatei
      // 
      this->textBoxAusgabeDatei->Location = System::Drawing::Point(8, 104);
      this->textBoxAusgabeDatei->Name = L"textBoxAusgabeDatei";
      this->textBoxAusgabeDatei->ReadOnly = true;
      this->textBoxAusgabeDatei->Size = System::Drawing::Size(368, 20);
      this->textBoxAusgabeDatei->TabIndex = 8;
      this->textBoxAusgabeDatei->Text = L"<Nicht definiert>";
      // 
      // buttonAusgabeDatei
      // 
      this->buttonAusgabeDatei->Location = System::Drawing::Point(8, 136);
      this->buttonAusgabeDatei->Name = L"buttonAusgabeDatei";
      this->buttonAusgabeDatei->Size = System::Drawing::Size(368, 23);
      this->buttonAusgabeDatei->TabIndex = 9;
      this->buttonAusgabeDatei->Text = L"Ausgabedatei";
      this->buttonAusgabeDatei->Click += gcnew System::EventHandler(this, &Rasterizer::buttonAusgabeDatei_Click);
      // 
      // buttonKonvertieren
      // 
      this->buttonKonvertieren->Location = System::Drawing::Point(296, 176);
      this->buttonKonvertieren->Name = L"buttonKonvertieren";
      this->buttonKonvertieren->Size = System::Drawing::Size(80, 48);
      this->buttonKonvertieren->TabIndex = 10;
      this->buttonKonvertieren->Text = L"Konvertieren";
      this->buttonKonvertieren->Click += gcnew System::EventHandler(this, &Rasterizer::buttonKonvertieren_Click);
      // 
      // textBoxErrorMessage
      // 
      this->textBoxErrorMessage->BackColor = System::Drawing::SystemColors::Control;
      this->textBoxErrorMessage->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
      this->textBoxErrorMessage->ForeColor = System::Drawing::SystemColors::ActiveCaption;
      this->textBoxErrorMessage->Location = System::Drawing::Point(8, 192);
      this->textBoxErrorMessage->Name = L"textBoxErrorMessage";
      this->textBoxErrorMessage->Size = System::Drawing::Size(280, 20);
      this->textBoxErrorMessage->TabIndex = 11;
      // 
      // Rasterizer
      // 
      this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
      this->ClientSize = System::Drawing::Size(384, 237);
      this->Controls->Add(this->textBoxErrorMessage);
      this->Controls->Add(this->buttonKonvertieren);
      this->Controls->Add(this->buttonAusgabeDatei);
      this->Controls->Add(this->textBoxAusgabeDatei);
      this->Controls->Add(this->labelMasstabFrei);
      this->Controls->Add(this->textBoxMasstabFrei);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->comboBoxMasstab);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->comboBoxAufloesung);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->comboBoxFormat);
      this->Name = L"Rasterizer";
      this->Text = L"Umwandlung der SVG-Datei";
      this->ResumeLayout(false);
      this->PerformLayout();

    }		
private: System::Void comboBoxMasstab_SelectedIndexChanged(System::Object ^  sender, System::EventArgs^  e)
{
	String ^ STR;

	STR = dynamic_cast<String^> ( comboBoxMasstab->SelectedItem );
	if ( String::Compare ( STR, "Benutzerdefiniert" ) == 0 )
	{
		textBoxMasstabFrei->Visible = true;
		labelMasstabFrei->Visible = true;
	}
	else
	{
		textBoxMasstabFrei->Visible = false;
		labelMasstabFrei->Visible = false;
	}

}

private: System::Void buttonAusgabeDatei_Click(System::Object ^  sender, System::EventArgs^  e)
{
	String ^ FILTER;

	if ( comboBoxFormat->SelectedIndex == 0 )
		FILTER = "PDF-Datei|*.pdf";
	else
	if ( comboBoxFormat->SelectedIndex == 1 )
		FILTER = "JPG-Datei|*.jpg";

	saveFileDialogAusgabeDatei->Filter = FILTER;

  if ( saveFileDialogAusgabeDatei->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		textBoxAusgabeDatei->Text = saveFileDialogAusgabeDatei->FileName;
}

private: System::Void buttonKonvertieren_Click(System::Object ^  sender, System::EventArgs^  e)
{
	String ^ STR;
	String ^ PARAMS;
	String ^ BATIK_PATH;
	String ^ OUTPUT_PATH;
	String ^ RASTER_EXTENSION;
	String ^ RASTER_RESOLUTION;
	String ^ OUTPUT_FORMAT;
	String ^ SVG_DATEI;

	double sw[3], no[3];
	double delta, sizeX, sizeY, pixelX, pixelY;
	double resulutionFactor;
	double massstab;
	bool   success;
  std::string srsName;
  int         srsDimension;

	PlanBereich * pPlanBereichSvg;

	if ( String::Compare ( textBoxAusgabeDatei->Text, "<Nicht definiert>" ) == 0 )
	{
		textBoxErrorMessage->Text = "Keine Ausgabedatei spezifiziert";
		return;
	}

	PARAMS = String::Concat ( "-Xm", javaHeapSize, " " );
	BATIK_PATH  = String::Concat ( "-jar \"", batikPath, "\" " );
	OUTPUT_PATH = String::Concat ( "-d \"",  textBoxAusgabeDatei->Text, "\" " );
	SVG_DATEI   = String::Concat ( "\"",  svgDatei, "\"" );

	pPlanBereichSvg = m_pVisual->getPlanBereichAktuell();
	success = pPlanBereichSvg->getBoundingBox ( sw, no, srsName, srsDimension );
	if ( !success ) return;
//	pPlanBereichSvg->getBoundingBox ( swX, swY, noX, noY );

	switch ( comboBoxAufloesung->SelectedIndex )
	{
	case 0:		// 100 dpi
		resulutionFactor = 100 / 0.0254;
		RASTER_RESOLUTION = "-dpi 100 ";
		break;

	case 1:		// 200 dpi
		resulutionFactor = 200 / 0.0254;
		RASTER_RESOLUTION = "-dpi 200 ";
		break;
	case 2:		// 300 dpi
		resulutionFactor = 300 / 0.0254;
		RASTER_RESOLUTION = "-dpi 300 ";
		break;
	}

	delta = m_pVisual->getDelta();
	sizeX = (1.0 + 2.0*delta)*(no[0] - sw[0]);
	sizeY = (1.0 + 2.0*delta)*(no[1] - sw[1]);

	switch ( comboBoxMasstab->SelectedIndex )
	{
	case 0:		// 1 : 500
		pixelX = ( sizeX * resulutionFactor ) / 500;
		pixelY = ( sizeY * resulutionFactor ) / 500;
		break;

	case 1:		// 1 : 1000
		pixelX = ( sizeX * resulutionFactor ) / 1000;
		pixelY = ( sizeY * resulutionFactor ) / 1000;
		break;

	case 2:		// 1 : 2000
		pixelX = ( sizeX * resulutionFactor ) / 2000;
		pixelY = ( sizeY * resulutionFactor ) / 2000;
		break;

	case 3:		// 1 : 5000
		pixelX = ( sizeX * resulutionFactor ) / 5000;
		pixelY = ( sizeY * resulutionFactor ) / 5000;
		break;

	case 4:		// 1 : 10.000
		pixelX = ( sizeX * resulutionFactor ) / 10000;
		pixelY = ( sizeY * resulutionFactor ) / 10000;
		break;

	case 5:		// Benutzerdefiniert
		STR = textBoxMasstabFrei->Text;
		if ( STR == nullptr )
		{
			textBoxErrorMessage->Text = "Bitte Maßstab angeben";
			return;
		}
		massstab = Convert::ToDouble ( STR );
		if ( massstab == 0.0 )
		{
			textBoxErrorMessage->Text = "Ungültiger Maßstab";
			return;
		}
		pixelX = ( sizeX * resulutionFactor ) / massstab;
		pixelY = ( sizeY * resulutionFactor ) / massstab;
		break;
	}

	RASTER_EXTENSION = String::Concat ( "-w ", XmlConvert::ToString ( pixelX ), " -h ",
										XmlConvert::ToString ( pixelY ), " " );


	switch ( comboBoxFormat->SelectedIndex )
	{
	case 0:		// pdf
		OUTPUT_FORMAT = "-q 0.99 -m application/pdf ";
		break;
	case 1:		// jpg
		OUTPUT_FORMAT = "-q 0.99 -m image/jpg ";
		break;
	}

	STR = String::Concat ( PARAMS, BATIK_PATH, OUTPUT_PATH, RASTER_EXTENSION );
	STR = String::Concat ( STR, RASTER_RESOLUTION, OUTPUT_FORMAT, svgDatei );

  try
  {
	  rasterizerProcess = gcnew System::Diagnostics::Process();
	  processStartInfo = gcnew System::Diagnostics::ProcessStartInfo();

	  processStartInfo->FileName = javaPath;
	  processStartInfo->Arguments = STR;

	  rasterizerProcess->StartInfo = processStartInfo;
	  rasterizerProcess->Start();
  }

  catch ( Exception ^ )
  {
    MessageBox::Show ( "Java konnte nicht gestartet werden" );
  }
	Close();
}

};
