#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
	
#include "GmlToolbox.h"
#include "Feature.h"
#include "Convert.h"
#include "GmlSchema.h"
#include "X3D-Module/X3D-Viewer.h"

#include <Windows.h>
#include <shellapi.h>
#include <string>
#include <fstream>
#include <msclr\marshal_cppstd.h>

using namespace std;

	/// <summary>
	/// Zusammenfassung für X3DErzeugungDialog
	/// </summary>
	public ref class X3DErzeugungDialog : public System::Windows::Forms::Form
	{
	public:
		X3DErzeugungDialog( GmlToolbox * pViewerP, String ^ gmlFileName )
		{
			vector<std::string>		vLODStufen;
			std::string				lodStufe;
			unsigned int			i, anz;
      string            systemFolder;

 			InitializeComponent();

      pViewer   = pViewerP;
			m_gmlFileName = gmlFileName;
	    gmlObjects = pViewer->getFeatures()->getGmlSchema();
	    features = pViewer->getFeatures();

      checkBoxX3DDatei->Checked = false;

      textBoxx3DDatei->Text = QuConvert::ToString( pViewer->getSystemFolder() + "X3D-Viewer\\X3DModel.x3d" );
      textBoxx3DDatei->Visible  = false;
      buttonx3DDatei->Visible   = false;

	    if (gmlObjects->isCityGml())
	    {
		    anz = features->getLODStufenAsString(vLODStufen);
		    for (i = 0; i < anz; ++i)
		    {
 			    lodStufe = vLODStufen[i];
			    comboBoxSelection->Items->Add( QuConvert::ToString(lodStufe));
		    }

		    if (vLODStufen.size() > 0)
		    {
			    comboBoxSelection->SelectedItem = QuConvert::ToString(vLODStufen[0]);
		    }
	    }

		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~X3DErzeugungDialog()
		{
			if (components)
			{
				delete components;
			}
		}

	
	private:
       GmlToolbox * pViewer;
		   GmlSchema  *	gmlObjects;
		   Features   *	features;
		   String     ^ m_gmlFileName;
       String     ^ x3dViewer;
       String     ^ offsetBackup;
       String     ^ selectedLOD;

	protected:


  private: System::Windows::Forms::Button^  buttonAbbrechen;
  private: System::Windows::Forms::Button^  buttonStarten;
  private: System::Windows::Forms::OpenFileDialog^  openX3DFileDialog;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;
  private: System::Windows::Forms::ComboBox^  comboBoxSelection;
  private: System::Windows::Forms::Label^  labelSelection;
  private: System::Windows::Forms::CheckBox^  checkBoxX3DDatei;
  private: System::Windows::Forms::TextBox^  textBoxx3DDatei;
  private: System::Windows::Forms::TextBox^  textBoxMeldung;
  private: System::Windows::Forms::Button^  buttonx3DDatei;

	private:


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->buttonStarten = (gcnew System::Windows::Forms::Button());
      this->openX3DFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
      this->comboBoxSelection = (gcnew System::Windows::Forms::ComboBox());
      this->labelSelection = (gcnew System::Windows::Forms::Label());
      this->checkBoxX3DDatei = (gcnew System::Windows::Forms::CheckBox());
      this->textBoxx3DDatei = (gcnew System::Windows::Forms::TextBox());
      this->buttonx3DDatei = (gcnew System::Windows::Forms::Button());
      this->textBoxMeldung = (gcnew System::Windows::Forms::TextBox());
      this->SuspendLayout();
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonAbbrechen->Location = System::Drawing::Point(12, 244);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 29);
      this->buttonAbbrechen->TabIndex = 6;
      this->buttonAbbrechen->Text = L"Beenden";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &X3DErzeugungDialog::buttonAbbrechen_Click);
      // 
      // buttonStarten
      // 
      this->buttonStarten->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonStarten->Location = System::Drawing::Point(209, 244);
      this->buttonStarten->Name = L"buttonStarten";
      this->buttonStarten->Size = System::Drawing::Size(75, 29);
      this->buttonStarten->TabIndex = 7;
      this->buttonStarten->Text = L"Starten";
      this->buttonStarten->UseVisualStyleBackColor = true;
      this->buttonStarten->Click += gcnew System::EventHandler(this, &X3DErzeugungDialog::buttonStarten_Click);
      // 
      // openX3DFileDialog
      // 
      this->openX3DFileDialog->DefaultExt = L"*.xhtml|(*.xhtml)";
      // 
      // comboBoxSelection
      // 
      this->comboBoxSelection->FormattingEnabled = true;
      this->comboBoxSelection->Location = System::Drawing::Point(15, 43);
      this->comboBoxSelection->Name = L"comboBoxSelection";
      this->comboBoxSelection->Size = System::Drawing::Size(214, 21);
      this->comboBoxSelection->TabIndex = 6;
      // 
      // labelSelection
      // 
      this->labelSelection->AutoSize = true;
      this->labelSelection->Location = System::Drawing::Point(12, 67);
      this->labelSelection->Name = L"labelSelection";
      this->labelSelection->Size = System::Drawing::Size(91, 13);
      this->labelSelection->TabIndex = 7;
      this->labelSelection->Text = L"Dargestellter LOD";
      // 
      // checkBoxX3DDatei
      // 
      this->checkBoxX3DDatei->AutoSize = true;
      this->checkBoxX3DDatei->Location = System::Drawing::Point(15, 105);
      this->checkBoxX3DDatei->Name = L"checkBoxX3DDatei";
      this->checkBoxX3DDatei->Size = System::Drawing::Size(128, 17);
      this->checkBoxX3DDatei->TabIndex = 8;
      this->checkBoxX3DDatei->Text = L"X3D Datei generieren";
      this->checkBoxX3DDatei->UseVisualStyleBackColor = true;
      this->checkBoxX3DDatei->CheckedChanged += gcnew System::EventHandler(this, &X3DErzeugungDialog::checkBoxX3DDatei_CheckedChanged);
      // 
      // textBoxx3DDatei
      // 
      this->textBoxx3DDatei->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxx3DDatei->Location = System::Drawing::Point(12, 128);
      this->textBoxx3DDatei->Name = L"textBoxx3DDatei";
      this->textBoxx3DDatei->ReadOnly = true;
      this->textBoxx3DDatei->Size = System::Drawing::Size(272, 20);
      this->textBoxx3DDatei->TabIndex = 9;
      // 
      // buttonx3DDatei
      // 
      this->buttonx3DDatei->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->buttonx3DDatei->Location = System::Drawing::Point(209, 154);
      this->buttonx3DDatei->Name = L"buttonx3DDatei";
      this->buttonx3DDatei->Size = System::Drawing::Size(75, 23);
      this->buttonx3DDatei->TabIndex = 10;
      this->buttonx3DDatei->Text = L"Ändern";
      this->buttonx3DDatei->UseVisualStyleBackColor = true;
      // 
      // textBoxMeldung
      // 
      this->textBoxMeldung->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxMeldung->BackColor = System::Drawing::SystemColors::Control;
      this->textBoxMeldung->ForeColor = System::Drawing::Color::Red;
      this->textBoxMeldung->Location = System::Drawing::Point(12, 204);
      this->textBoxMeldung->Name = L"textBoxMeldung";
      this->textBoxMeldung->Size = System::Drawing::Size(272, 20);
      this->textBoxMeldung->TabIndex = 11;
      // 
      // X3DErzeugungDialog
      // 
      this->AcceptButton = this->buttonStarten;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->buttonAbbrechen;
      this->ClientSize = System::Drawing::Size(296, 285);
      this->Controls->Add(this->textBoxMeldung);
      this->Controls->Add(this->buttonx3DDatei);
      this->Controls->Add(this->textBoxx3DDatei);
      this->Controls->Add(this->checkBoxX3DDatei);
      this->Controls->Add(this->buttonStarten);
      this->Controls->Add(this->labelSelection);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->comboBoxSelection);
      this->Location = System::Drawing::Point(265, 57);
      this->Name = L"X3DErzeugungDialog";
      this->Text = L"X3D Visualisierung";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

	private: System::Void buttonAbbrechen_Click(System::Object^  sender, System::EventArgs^  e)
         {
           Close();
         }

private: System::Void buttonStarten_Click(System::Object^  sender, System::EventArgs^  e) 
       {
          string    templatePath;
          string    outputPath;
          string    visualizationParamPath;
          string    gmlFileName;
          string    selection;
          String  ^ STR;
          string    systemFolder  = pViewer->getSystemFolder();
          X3D::X3DViewer * pViewer;
 
          STR = dynamic_cast<String^>(comboBoxSelection->SelectedItem);
          QuConvert::systemStr2stdStr ( selection, STR );

          templatePath           = systemFolder + "X3D-Viewer\\XHTML_frame.tpl";
          visualizationParamPath = "";
          textBoxMeldung->Text = "";

          QuConvert::systemStr2stdStr( gmlFileName, m_gmlFileName );
			 
          if (  gmlObjects->isCityGml() )
			    {		   
				    multimap<std::string,Feature*> featureMap;
				    set<std::string> klassenNamen;

				    features->getChildren(featureMap, klassenNamen);
				 
//				 std::string	gmlFileName = msclr::interop::marshal_as<std::string>(%*m_gmlFileName);

//				 std::string	configFileName = msclr::interop::marshal_as<std::string>(%*textBoxDateiname->Text);
				   pViewer = new X3D::X3DViewer("x3d", "scene", "bgnd", "view", features->getGmlTypAsString(), visualizationParamPath, gmlFileName, templatePath );

				   X3D::CityGML2X3D fz(features, Features::lodStufeToEnum(selection), pViewer);

				   fz.DefineGMLExtend();

				   std::list<std::string>	featureTypes;

				   fz.iterFeatures(featureMap, "CityModel", featureTypes);
				   fz.setSelectionAppearance();

           if ( checkBoxX3DDatei->Checked )
           {
             QuConvert::systemStr2stdStr( outputPath, textBoxx3DDatei->Text );
             fz.writeX3DFile( outputPath );
             textBoxMeldung->Text = "X3D Datei wurde erzeugt";
           }
           else
           {
             outputPath = systemFolder + "X3D-Viewer\\X3D-Viewer.xhtml";
				     fz.writeX3DViewer( outputPath );
			     }
          }

          if ( !checkBoxX3DDatei->Checked )
			      ShellExecute(NULL, _T("open"), outputPath.c_str(), NULL,NULL, SW_SHOW);

          delete pViewer;
			   //Close();
        }

	//private: System::Void textBoxOffset_Leave(System::Object^  sender, System::EventArgs^  e) 
 //        {
 //          try
 //          {
 //            zOffset = Convert::ToDouble( textBoxOffset->Text );
 //            textBoxOffset->Text = Convert::ToString( zOffset );
 //            offsetBackup = textBoxOffset->Text;
 //         }

 //          catch ( FormatException ^ )
 //          {
 //             textBoxOffset->Text = offsetBackup;
 //          }
 //        }

  private: System::Void checkBoxX3DDatei_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
           {
             if ( checkBoxX3DDatei->Checked == true )
             {
               textBoxx3DDatei->Visible = true;
               buttonx3DDatei->Visible   = true;
             }
             else
             {
               textBoxx3DDatei->Visible = false;
               buttonx3DDatei->Visible   = false;
             }
           }
};
