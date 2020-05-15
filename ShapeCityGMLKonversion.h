#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Feature.h"
#include "GmlSchema.h"
#include "Convert.h"


	/// <summary>
	/// Zusammenfassung für ShapeCityGMLKonversion
	///
	/// Warnung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie auch
	///          die Ressourcendateiname-Eigenschaft für das Tool zur Kompilierung verwalteter Ressourcen ändern,
	///          das allen RESX-Dateien zugewiesen ist, von denen diese Klasse abhängt.
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class ShapeCityGMLKonversion : public System::Windows::Forms::Form
	{
	public:
		ShapeCityGMLKonversion( GmlToolbox * pViewer )
		{
			InitializeComponent();

      m_pViewer = pViewer;

      gmlTyp = CITYGML_2_0;
      radioButtonCityGML2_0->Checked = true;

      comboBoxMode->SelectedIndex = 0;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~ShapeCityGMLKonversion()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::RadioButton^  radioButtonCityGML2_0;

  private: System::Windows::Forms::GroupBox^  groupBoxVersion;
  private: System::Windows::Forms::TextBox^  textBoxShapefiles;
  private: System::Windows::Forms::Label^  label1;

  private: System::Windows::Forms::Button^  buttonShapefilesLesen;
  private: System::Windows::Forms::ComboBox^  comboBoxMode;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Button^  buttonOk;

  protected: 
  GmlToolbox * m_pViewer;
  GML_SCHEMA_TYPE  gmlTyp;
  private: System::Windows::Forms::OpenFileDialog^  shapefileDialog;
  private: System::Windows::Forms::Button^  buttonAbbrechen;
  protected: 

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
      this->radioButtonCityGML2_0 = (gcnew System::Windows::Forms::RadioButton());
      this->groupBoxVersion = (gcnew System::Windows::Forms::GroupBox());
      this->textBoxShapefiles = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->buttonShapefilesLesen = (gcnew System::Windows::Forms::Button());
      this->comboBoxMode = (gcnew System::Windows::Forms::ComboBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->buttonOk = (gcnew System::Windows::Forms::Button());
      this->shapefileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->groupBoxVersion->SuspendLayout();
      this->SuspendLayout();
      // 
      // radioButtonCityGML2_0
      // 
      this->radioButtonCityGML2_0->AutoSize = true;
      this->radioButtonCityGML2_0->Location = System::Drawing::Point(6, 21);
      this->radioButtonCityGML2_0->Name = L"radioButtonCityGML2_0";
      this->radioButtonCityGML2_0->Size = System::Drawing::Size(83, 17);
      this->radioButtonCityGML2_0->TabIndex = 0;
      this->radioButtonCityGML2_0->TabStop = true;
      this->radioButtonCityGML2_0->Text = L"CityGML 2.0";
      this->radioButtonCityGML2_0->UseVisualStyleBackColor = true;
      this->radioButtonCityGML2_0->CheckedChanged += gcnew System::EventHandler(this, &ShapeCityGMLKonversion::radioButtonCityGML2_0_CheckedChanged);
      // 
      // groupBoxVersion
      // 
      this->groupBoxVersion->Controls->Add(this->radioButtonCityGML2_0);
      this->groupBoxVersion->Location = System::Drawing::Point(13, 22);
      this->groupBoxVersion->Name = L"groupBoxVersion";
      this->groupBoxVersion->Size = System::Drawing::Size(292, 72);
      this->groupBoxVersion->TabIndex = 2;
      this->groupBoxVersion->TabStop = false;
      this->groupBoxVersion->Text = L"CityGML BVersion";
      // 
      // textBoxShapefiles
      // 
      this->textBoxShapefiles->Location = System::Drawing::Point(13, 123);
      this->textBoxShapefiles->Name = L"textBoxShapefiles";
      this->textBoxShapefiles->ReadOnly = true;
      this->textBoxShapefiles->Size = System::Drawing::Size(292, 20);
      this->textBoxShapefiles->TabIndex = 3;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(13, 150);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(56, 13);
      this->label1->TabIndex = 4;
      this->label1->Text = L"Shapefiles";
      // 
      // buttonShapefilesLesen
      // 
      this->buttonShapefilesLesen->Location = System::Drawing::Point(230, 145);
      this->buttonShapefilesLesen->Name = L"buttonShapefilesLesen";
      this->buttonShapefilesLesen->Size = System::Drawing::Size(75, 23);
      this->buttonShapefilesLesen->TabIndex = 5;
      this->buttonShapefilesLesen->Text = L"Lesen";
      this->buttonShapefilesLesen->UseVisualStyleBackColor = true;
      this->buttonShapefilesLesen->Click += gcnew System::EventHandler(this, &ShapeCityGMLKonversion::buttonShapefilesLesen_Click);
      // 
      // comboBoxMode
      // 
      this->comboBoxMode->FormattingEnabled = true;
      this->comboBoxMode->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"Geonovum-Auftrag"});
      this->comboBoxMode->Location = System::Drawing::Point(13, 197);
      this->comboBoxMode->Name = L"comboBoxMode";
      this->comboBoxMode->Size = System::Drawing::Size(121, 21);
      this->comboBoxMode->TabIndex = 6;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(13, 222);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(102, 13);
      this->label2->TabIndex = 7;
      this->label2->Text = L"Abbildungsvorschrift";
      // 
      // buttonOk
      // 
      this->buttonOk->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonOk->Location = System::Drawing::Point(230, 192);
      this->buttonOk->Name = L"buttonOk";
      this->buttonOk->Size = System::Drawing::Size(75, 48);
      this->buttonOk->TabIndex = 8;
      this->buttonOk->Text = L"Ok";
      this->buttonOk->UseVisualStyleBackColor = true;
      this->buttonOk->Click += gcnew System::EventHandler(this, &ShapeCityGMLKonversion::buttonOk_Click);
      // 
      // shapefileDialog
      // 
      this->shapefileDialog->Filter = L"(*.shp)|*.shp";
      this->shapefileDialog->Multiselect = true;
      this->shapefileDialog->Title = L"Einlesen Shapefiles";
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonAbbrechen->Location = System::Drawing::Point(140, 217);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 23);
      this->buttonAbbrechen->TabIndex = 9;
      this->buttonAbbrechen->Text = L"Abbrechen";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &ShapeCityGMLKonversion::buttonAbbrechen_Click);
      // 
      // ShapeCityGMLKonversion
      // 
      this->AcceptButton = this->buttonOk;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(317, 250);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->buttonOk);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->comboBoxMode);
      this->Controls->Add(this->buttonShapefilesLesen);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->textBoxShapefiles);
      this->Controls->Add(this->groupBoxVersion);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"ShapeCityGMLKonversion";
      this->Text = L"Shapefile CityGML Konversion";
      this->groupBoxVersion->ResumeLayout(false);
      this->groupBoxVersion->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion



private: System::Void radioButtonCityGML2_0_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  gmlTyp = CITYGML_2_0;
}


private: System::Void buttonShapefilesLesen_Click(System::Object^  sender, System::EventArgs^  e) 
{
  int				       i, nFiles;
  vector<string>   shapeFileNames;
  string           stdString;
  array<String^> ^ shapeFileArray;
  String         ^ STR;

  if( shapefileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
  {		
    shapeFileArray = shapefileDialog->FileNames;
    nFiles = shapeFileArray->Length;

    for ( i = 0; i < nFiles; i++)
    {
      STR = shapeFileArray[i];
      QuConvert::systemStr2stdStr ( stdString, STR );
      shapeFileNames.push_back ( stdString );
    }
    m_pViewer->setShapeFiles ( shapeFileNames );

    STR = String::Concat ( Convert::ToString ( nFiles ), " Shape-File(s) spezifiziert" );
    textBoxShapefiles->Text= STR;
  }
}

private: System::Void buttonOk_Click(System::Object^  sender, System::EventArgs^  e) 
{
  GmlSchema * pGmlSchema;
  bool        success;

  m_pViewer->getFeatures()->clear();
  pGmlSchema = new GmlSchema ( m_pViewer );
  success = pGmlSchema->readGmlSchema( gmlTyp, NULL );
  if ( success )
    m_pViewer->getFeatures()->setGmlSchema( pGmlSchema );
  else
  {
    delete pGmlSchema;
    pGmlSchema = NULL; 
    return;
  }

  switch ( comboBoxMode->SelectedIndex )
  {
  case 0:
    m_pViewer->geonovumAuftrag();
    Close();
    break;

  default:
    break;
  }

}
private: System::Void buttonAbbrechen_Click(System::Object^  sender, System::EventArgs^  e) 
{
  Close();
}
};


