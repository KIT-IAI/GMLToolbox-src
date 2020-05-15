#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "BPlan.h"
#include "GmlSchema.h"
#include "Convert.h"

#include "ShpFileNamenKonv.h"


namespace XPlanGMLToolbox
{
	/// <summary> 
	/// Zusammenfassung für ExterneReferenzDialog
	///
	/// Achtung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie die Eigenschaft
	///          'Ressourcendateiname' für das Compilertool für verwaltete Ressourcen ändern, 
	///          das allen .resx-Dateien zugewiesen ist, von denen diese Klasse abhängt. 
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class ExterneReferenzDialog : public System::Windows::Forms::Form
	{
	public: 
		ExterneReferenzDialog( PlanAttribute * pPlanAttributeP, BLeitplanViewer * pViewerP  )
		{
			string planTypString = pViewerP->getPlanTypString();
			int    gmlVersion =  pViewerP->getGmlObjekte()->getXPlanGmlTyp();

			InitializeComponent();

			if ( planTypString == "BPlan" )
        planTyp = PLAN_TYP::BPLAN;
			else
			if ( planTypString == "FPlan" )
				planTyp = PLAN_TYP::FPLAN;
			else
			if ( planTypString == "RPlan" )
				planTyp = PLAN_TYP::RPLAN;
			else
			if ( planTypString == "LPlan" )
				planTyp = PLAN_TYP::LPLAN;
			else
				planTyp = PLAN_TYP::UNBESTIMMT;

			if ( gmlVersion == 2 )
        xPlanGmlVersion = XPLANGML_VERSION::V_2;
			else
			if ( gmlVersion == 3 )
				xPlanGmlVersion = XPLANGML_VERSION::V_3;
			else
			if ( gmlVersion == 4 )
				xPlanGmlVersion = XPLANGML_VERSION::V_4;

			pViewer           = pViewerP;
			pPlanAttribute    = pPlanAttributeP;
			pPlanAttributeOld = new PlanAttribute ( pPlanAttribute );
	
			pExterneReferenzAktiv = NULL;

			initialize();


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
	private: System::Windows::Forms::Label ^  label1;
	private: System::Windows::Forms::Label ^  label2;
	private: System::Windows::Forms::GroupBox ^  groupBox1;
	private: System::Windows::Forms::Label ^  label7;
	private: System::Windows::Forms::ComboBox ^  comboBoxRelationName;
	private: System::Windows::Forms::TextBox ^  textBoxBeschreibung;
	private: System::Windows::Forms::TextBox ^  textBoxURLReferenz;
	private: System::Windows::Forms::Label ^  labelURLReferenz;
	private: System::Windows::Forms::TextBox ^  textBoxURLGeoref;
	private: System::Windows::Forms::TextBox ^  textBoxMimeTypeReferenz;
	private: System::Windows::Forms::Label ^  labelMimeTypeReferenz;
	private: System::Windows::Forms::Label ^  labelURLGeoref;
	private: System::Windows::Forms::TextBox ^  textBoxMimeTypeGeoref;
	private: System::Windows::Forms::Label ^  labelMimeTypeGeoref;
	private: System::Windows::Forms::ComboBox ^  comboBoxReferenz;
	private: System::Windows::Forms::Button ^  buttonReferenzNeu;
	private: System::Windows::Forms::Button ^  buttonReferenzLoeschen;
	private: System::Windows::Forms::Button ^  buttonExterneReferenzVerwerfen;
	private: System::Windows::Forms::Button ^  buttonExterneReferenzUebernehmen;
	private: System::Windows::Forms::Button ^  buttonBeenden;
	private: System::Windows::Forms::Button ^  buttonAbbrechen;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container^ components;
		
		enum class PLAN_TYP  {  BPLAN, FPLAN, RPLAN, LPLAN, UNBESTIMMT  };
		enum class XPLANGML_VERSION  {  V_2=2, V_3=3, V_4=4  };

		PLAN_TYP         planTyp;
		XPLANGML_VERSION xPlanGmlVersion;

		BLeitplanViewer * pViewer;
		PlanAttribute   * pPlanAttribute;
		PlanAttribute   * pPlanAttributeOld;

		ExterneReferenz * pExterneReferenzAktiv;
		String          ^ relationAktiv;


		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->comboBoxRelationName = (gcnew System::Windows::Forms::ComboBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBoxBeschreibung = (gcnew System::Windows::Forms::TextBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->textBoxURLReferenz = (gcnew System::Windows::Forms::TextBox());
      this->labelURLReferenz = (gcnew System::Windows::Forms::Label());
      this->textBoxURLGeoref = (gcnew System::Windows::Forms::TextBox());
      this->textBoxMimeTypeReferenz = (gcnew System::Windows::Forms::TextBox());
      this->labelMimeTypeReferenz = (gcnew System::Windows::Forms::Label());
      this->labelURLGeoref = (gcnew System::Windows::Forms::Label());
      this->textBoxMimeTypeGeoref = (gcnew System::Windows::Forms::TextBox());
      this->labelMimeTypeGeoref = (gcnew System::Windows::Forms::Label());
      this->comboBoxReferenz = (gcnew System::Windows::Forms::ComboBox());
      this->buttonReferenzNeu = (gcnew System::Windows::Forms::Button());
      this->buttonReferenzLoeschen = (gcnew System::Windows::Forms::Button());
      this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
      this->buttonExterneReferenzUebernehmen = (gcnew System::Windows::Forms::Button());
      this->buttonExterneReferenzVerwerfen = (gcnew System::Windows::Forms::Button());
      this->label7 = (gcnew System::Windows::Forms::Label());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->groupBox1->SuspendLayout();
      this->SuspendLayout();
      // 
      // comboBoxRelationName
      // 
      this->comboBoxRelationName->Location = System::Drawing::Point(16, 17);
      this->comboBoxRelationName->Name = L"comboBoxRelationName";
      this->comboBoxRelationName->Size = System::Drawing::Size(120, 21);
      this->comboBoxRelationName->TabIndex = 0;
      this->comboBoxRelationName->SelectedIndexChanged += gcnew System::EventHandler(this, &ExterneReferenzDialog::comboBoxRelationName_SelectedIndexChanged);
      // 
      // label1
      // 
      this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->label1->Location = System::Drawing::Point(152, 19);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(176, 19);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Typ der Externen  Referenz";
      // 
      // textBoxBeschreibung
      // 
      this->textBoxBeschreibung->Location = System::Drawing::Point(16, 136);
      this->textBoxBeschreibung->Name = L"textBoxBeschreibung";
      this->textBoxBeschreibung->Size = System::Drawing::Size(312, 20);
      this->textBoxBeschreibung->TabIndex = 2;
      // 
      // label2
      // 
      this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->label2->Location = System::Drawing::Point(16, 160);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(100, 23);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Beschreibung";
      // 
      // textBoxURLReferenz
      // 
      this->textBoxURLReferenz->Location = System::Drawing::Point(16, 192);
      this->textBoxURLReferenz->Name = L"textBoxURLReferenz";
      this->textBoxURLReferenz->Size = System::Drawing::Size(312, 20);
      this->textBoxURLReferenz->TabIndex = 4;
      // 
      // labelURLReferenz
      // 
      this->labelURLReferenz->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->labelURLReferenz->Location = System::Drawing::Point(16, 216);
      this->labelURLReferenz->Name = L"labelURLReferenz";
      this->labelURLReferenz->Size = System::Drawing::Size(144, 23);
      this->labelURLReferenz->TabIndex = 5;
      this->labelURLReferenz->Text = L"URL des Dokuments";
      // 
      // textBoxURLGeoref
      // 
      this->textBoxURLGeoref->Location = System::Drawing::Point(16, 288);
      this->textBoxURLGeoref->Name = L"textBoxURLGeoref";
      this->textBoxURLGeoref->Size = System::Drawing::Size(312, 20);
      this->textBoxURLGeoref->TabIndex = 6;
      // 
      // textBoxMimeTypeReferenz
      // 
      this->textBoxMimeTypeReferenz->Location = System::Drawing::Point(16, 240);
      this->textBoxMimeTypeReferenz->Name = L"textBoxMimeTypeReferenz";
      this->textBoxMimeTypeReferenz->Size = System::Drawing::Size(100, 20);
      this->textBoxMimeTypeReferenz->TabIndex = 7;
      // 
      // labelMimeTypeReferenz
      // 
      this->labelMimeTypeReferenz->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->labelMimeTypeReferenz->Location = System::Drawing::Point(128, 240);
      this->labelMimeTypeReferenz->Name = L"labelMimeTypeReferenz";
      this->labelMimeTypeReferenz->Size = System::Drawing::Size(152, 23);
      this->labelMimeTypeReferenz->TabIndex = 8;
      this->labelMimeTypeReferenz->Text = L"Mime Type des Dokuments";
      // 
      // labelURLGeoref
      // 
      this->labelURLGeoref->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->labelURLGeoref->Location = System::Drawing::Point(16, 312);
      this->labelURLGeoref->Name = L"labelURLGeoref";
      this->labelURLGeoref->Size = System::Drawing::Size(208, 23);
      this->labelURLGeoref->TabIndex = 9;
      this->labelURLGeoref->Text = L"URL der Georeferenzierung";
      // 
      // textBoxMimeTypeGeoref
      // 
      this->textBoxMimeTypeGeoref->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->textBoxMimeTypeGeoref->Location = System::Drawing::Point(8, 232);
      this->textBoxMimeTypeGeoref->Name = L"textBoxMimeTypeGeoref";
      this->textBoxMimeTypeGeoref->Size = System::Drawing::Size(100, 20);
      this->textBoxMimeTypeGeoref->TabIndex = 10;
      // 
      // labelMimeTypeGeoref
      // 
      this->labelMimeTypeGeoref->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->labelMimeTypeGeoref->Location = System::Drawing::Point(128, 344);
      this->labelMimeTypeGeoref->Name = L"labelMimeTypeGeoref";
      this->labelMimeTypeGeoref->Size = System::Drawing::Size(184, 23);
      this->labelMimeTypeGeoref->TabIndex = 11;
      this->labelMimeTypeGeoref->Text = L"Mime Type der Georeferenzierung";
      // 
      // comboBoxReferenz
      // 
      this->comboBoxReferenz->Location = System::Drawing::Point(16, 57);
      this->comboBoxReferenz->Name = L"comboBoxReferenz";
      this->comboBoxReferenz->Size = System::Drawing::Size(112, 21);
      this->comboBoxReferenz->TabIndex = 12;
      this->comboBoxReferenz->SelectedIndexChanged += gcnew System::EventHandler(this, &ExterneReferenzDialog::comboBoxReferenz_SelectedIndexChanged);
      // 
      // buttonReferenzNeu
      // 
      this->buttonReferenzNeu->Location = System::Drawing::Point(144, 56);
      this->buttonReferenzNeu->Name = L"buttonReferenzNeu";
      this->buttonReferenzNeu->Size = System::Drawing::Size(75, 23);
      this->buttonReferenzNeu->TabIndex = 13;
      this->buttonReferenzNeu->Text = L"Neu";
      this->buttonReferenzNeu->Click += gcnew System::EventHandler(this, &ExterneReferenzDialog::buttonReferenzNeu_Click);
      // 
      // buttonReferenzLoeschen
      // 
      this->buttonReferenzLoeschen->Location = System::Drawing::Point(248, 56);
      this->buttonReferenzLoeschen->Name = L"buttonReferenzLoeschen";
      this->buttonReferenzLoeschen->Size = System::Drawing::Size(75, 23);
      this->buttonReferenzLoeschen->TabIndex = 14;
      this->buttonReferenzLoeschen->Text = L"Löschen";
      this->buttonReferenzLoeschen->Click += gcnew System::EventHandler(this, &ExterneReferenzDialog::buttonReferenzLoeschen_Click);
      // 
      // groupBox1
      // 
      this->groupBox1->Controls->Add(this->buttonExterneReferenzUebernehmen);
      this->groupBox1->Controls->Add(this->buttonExterneReferenzVerwerfen);
      this->groupBox1->Controls->Add(this->textBoxMimeTypeGeoref);
      this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->groupBox1->Location = System::Drawing::Point(8, 112);
      this->groupBox1->Name = L"groupBox1";
      this->groupBox1->Size = System::Drawing::Size(336, 304);
      this->groupBox1->TabIndex = 15;
      this->groupBox1->TabStop = false;
      this->groupBox1->Text = L"Parameter der Referenz";
      // 
      // buttonExterneReferenzUebernehmen
      // 
      this->buttonExterneReferenzUebernehmen->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->buttonExterneReferenzUebernehmen->Location = System::Drawing::Point(240, 272);
      this->buttonExterneReferenzUebernehmen->Name = L"buttonExterneReferenzUebernehmen";
      this->buttonExterneReferenzUebernehmen->Size = System::Drawing::Size(80, 23);
      this->buttonExterneReferenzUebernehmen->TabIndex = 1;
      this->buttonExterneReferenzUebernehmen->Text = L"Übernehmen";
      this->buttonExterneReferenzUebernehmen->Click += gcnew System::EventHandler(this, &ExterneReferenzDialog::buttonExterneReferenzUebernehmen_Click);
      // 
      // buttonExterneReferenzVerwerfen
      // 
      this->buttonExterneReferenzVerwerfen->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->buttonExterneReferenzVerwerfen->Location = System::Drawing::Point(16, 272);
      this->buttonExterneReferenzVerwerfen->Name = L"buttonExterneReferenzVerwerfen";
      this->buttonExterneReferenzVerwerfen->Size = System::Drawing::Size(75, 23);
      this->buttonExterneReferenzVerwerfen->TabIndex = 0;
      this->buttonExterneReferenzVerwerfen->Text = L"Abbrechen";
      this->buttonExterneReferenzVerwerfen->Click += gcnew System::EventHandler(this, &ExterneReferenzDialog::buttonExterneReferenzVerwerfen_Click);
      // 
      // label7
      // 
      this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->label7->Location = System::Drawing::Point(16, 80);
      this->label7->Name = L"label7";
      this->label7->Size = System::Drawing::Size(152, 23);
      this->label7->TabIndex = 16;
      this->label7->Text = L"Auswahl einer Referenz";
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonBeenden->Location = System::Drawing::Point(264, 440);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 17;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &ExterneReferenzDialog::buttonBeenden_Click);
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonAbbrechen->Location = System::Drawing::Point(16, 440);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 23);
      this->buttonAbbrechen->TabIndex = 18;
      this->buttonAbbrechen->Text = L"Abbrechen";
      this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &ExterneReferenzDialog::buttonAbbrechen_Click);
      // 
      // ExterneReferenzDialog
      // 
      this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
      this->ClientSize = System::Drawing::Size(360, 470);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->label7);
      this->Controls->Add(this->buttonReferenzLoeschen);
      this->Controls->Add(this->buttonReferenzNeu);
      this->Controls->Add(this->comboBoxReferenz);
      this->Controls->Add(this->labelMimeTypeGeoref);
      this->Controls->Add(this->labelURLGeoref);
      this->Controls->Add(this->labelMimeTypeReferenz);
      this->Controls->Add(this->textBoxMimeTypeReferenz);
      this->Controls->Add(this->textBoxURLGeoref);
      this->Controls->Add(this->labelURLReferenz);
      this->Controls->Add(this->textBoxURLReferenz);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->textBoxBeschreibung);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->comboBoxRelationName);
      this->Controls->Add(this->groupBox1);
      this->Name = L"ExterneReferenzDialog";
      this->Text = L"Externe Referenzen des Plans";
      this->groupBox1->ResumeLayout(false);
      this->groupBox1->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }		



private: System::Void initialize()
		 {
       if ( xPlanGmlVersion == XPLANGML_VERSION::V_2 )
			 {
				 comboBoxRelationName->Items->Add ( "rechtsverbindlich" );
				 comboBoxRelationName->Items->Add ( "informell" );
			 }

			 if ( xPlanGmlVersion >= XPLANGML_VERSION::V_3 )
			 {
				 comboBoxRelationName->Items->Add ( "rechtsverbindlich" );
				 comboBoxRelationName->Items->Add ( "informell" );
				 comboBoxRelationName->Items->Add ( "refLegende" );
 				 comboBoxRelationName->Items->Add ( "refBegruendung" );
				 comboBoxRelationName->Items->Add ( "refExternalCodeList" );
				 comboBoxRelationName->Items->Add ( "refBescheibung" );
				 comboBoxRelationName->Items->Add ( "refPlangrundlage" );
				 comboBoxRelationName->Items->Add ( "refRechtsplan" );

         if ( planTyp == PLAN_TYP::BPLAN )
				 {
					 comboBoxRelationName->Items->Add ( "refKoordinatenliste" );
					 comboBoxRelationName->Items->Add ( "refPflanzliste" );
					 comboBoxRelationName->Items->Add ( "refGrundstuecksverzeichnis" );
					 comboBoxRelationName->Items->Add ( "refUmweltbericht" );
				 }
				 else
				 if ( planTyp == PLAN_TYP::FPLAN )
				 {
					 comboBoxRelationName->Items->Add ( "refUmweltbericht" );
				 }
			 }

			 comboBoxReferenz->SelectedIndex = -1;
			 pExterneReferenzAktiv = NULL;

			 anzeigeExterneReferenzAktiv();
		 }

private: System::Void anzeigeExterneReferenzAktiv ()
		{
			 string   stdString;
			 String ^ STR;

			if ( pExterneReferenzAktiv == NULL )
			{
				textBoxBeschreibung->Hide();
				textBoxURLReferenz->Hide();
				labelURLReferenz->Hide();
				textBoxURLGeoref->Hide();
				textBoxMimeTypeReferenz->Hide();
				labelMimeTypeReferenz->Hide();
				labelURLGeoref->Hide();
				textBoxMimeTypeGeoref->Hide();
				labelMimeTypeGeoref->Hide();

				buttonExterneReferenzVerwerfen->Enabled = false;
				buttonExterneReferenzUebernehmen->Enabled = false;
			}
			else
			{
				buttonExterneReferenzVerwerfen->Enabled = true;
				buttonExterneReferenzUebernehmen->Enabled = true;

				STR = QuConvert::ToString ( pExterneReferenzAktiv->beschreibung );
				textBoxBeschreibung->Text = dynamic_cast<String^>( STR );
				textBoxBeschreibung->Show();

				STR = QuConvert::ToString ( pExterneReferenzAktiv->referenzURL );
				textBoxURLReferenz->Text = dynamic_cast<String^>( STR );
				textBoxURLReferenz->Show();
				labelURLReferenz->Show();

  				STR = QuConvert::ToString ( pExterneReferenzAktiv->referenzMimeType );
				textBoxMimeTypeReferenz->Text = dynamic_cast<String^>( STR );
				textBoxMimeTypeReferenz->Show();
				labelMimeTypeReferenz->Show();

				if ( pExterneReferenzAktiv->relationName == "refPlangrundlage" ||
					 pExterneReferenzAktiv->relationName == "refRechtsplan" )
				{
					labelURLReferenz->Text = "URL des Plans";
					labelMimeTypeReferenz->Text = "Mime Type des Plans";

  					STR = QuConvert::ToString ( pExterneReferenzAktiv->georefURL );
					textBoxURLGeoref->Text = dynamic_cast<String^>( STR );
					textBoxURLGeoref->Show();
					labelURLGeoref->Show();

					STR = QuConvert::ToString ( pExterneReferenzAktiv->georefMimeType );
					textBoxMimeTypeGeoref->Text = dynamic_cast<String^>( STR );
					textBoxMimeTypeGeoref->Show();
					labelMimeTypeGeoref->Show();
				}
				else
				{
					labelURLReferenz->Text = "URL des Dokuments";
					labelMimeTypeReferenz->Text = "Mime Type des Dokuments";

					textBoxURLGeoref->Text = nullptr;
					textBoxMimeTypeGeoref->Text = nullptr;

					textBoxURLGeoref->Hide();
					labelURLGeoref->Hide();
					textBoxMimeTypeGeoref->Hide();
					labelMimeTypeGeoref->Hide();
				}

				
			}	
				
		}

private: System::Void updateExterneReferenzAktiv ()
		 {
			 String ^ STR;
			 string   stdString;

			 if ( pExterneReferenzAktiv == NULL ) return;

			 STR = textBoxBeschreibung->Text;
			 if ( STR != nullptr )
				 QuConvert::systemStr2stdStr ( stdString, STR );
			 else
				 stdString = "";
			 pExterneReferenzAktiv->beschreibung = stdString;


			 STR = textBoxURLReferenz->Text;
			 if ( STR != nullptr )
				 QuConvert::systemStr2stdStr ( stdString, STR );
			 else
				 stdString = "";
			 pExterneReferenzAktiv->referenzURL = stdString;

			 STR = textBoxMimeTypeReferenz->Text;
			 if ( STR != nullptr )
				 QuConvert::systemStr2stdStr ( stdString, STR );
			 else
				 stdString = "";
			 pExterneReferenzAktiv->referenzMimeType = stdString;


			 STR = textBoxURLGeoref->Text;
			 if ( STR != nullptr )
				 QuConvert::systemStr2stdStr ( stdString, STR );
			 else
				 stdString = "";
			 pExterneReferenzAktiv->georefURL = stdString;


			 STR = textBoxMimeTypeGeoref->Text;
			 if ( STR != nullptr )
				 QuConvert::systemStr2stdStr ( stdString, STR );
			 else
				 stdString = "";
			 pExterneReferenzAktiv->georefMimeType = stdString;
		 }

private: System::Void disableAuswahl ()
		 {
			 label1->Enabled = false;
			 label2->Enabled = false;

			 comboBoxRelationName->Enabled = false;
			 comboBoxReferenz->Enabled = false;

			 buttonReferenzNeu->Enabled = false;
			 buttonReferenzLoeschen->Enabled = false;
		 }

private: System::Void enableAuswahl ()
		 {
			 label1->Enabled = true;
			 label2->Enabled = true;

			 comboBoxRelationName->Enabled = true;
			 comboBoxReferenz->Enabled = true;

			 buttonReferenzNeu->Enabled = true;
			 buttonReferenzLoeschen->Enabled = true;
		 }


private: System::Void comboBoxRelationName_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
		 {
			String ^ STR;
			string   relationName;
			int      i, anz;

			relationAktiv = dynamic_cast<String^>( comboBoxRelationName->SelectedItem );
			if ( relationAktiv == nullptr ) return;

			QuConvert::systemStr2stdStr ( relationName, relationAktiv );

			anz =  pPlanAttribute->getExterneReferenzenAnz ( relationName );
			comboBoxReferenz->Items->Clear();
			for ( i = 0; i < anz; i++ )
				comboBoxReferenz->Items->Add ( String::Concat ( "Referenz ", Convert::ToString ( i ) ) );

			if ( anz > 0 )
				comboBoxReferenz->SelectedIndex = 0;
			else
			{
				comboBoxReferenz->SelectedIndex = -1;
				comboBoxReferenz->Text = nullptr;
			}


		 }

private: System::Void comboBoxReferenz_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
		 {
			 string relationName;
			 int    index = comboBoxReferenz->SelectedIndex;

			 if ( relationAktiv != nullptr )
				 QuConvert::systemStr2stdStr ( relationName, relationAktiv );

			 pExterneReferenzAktiv = pPlanAttribute->getExterneReferenz ( relationName, index );

			 if ( pExterneReferenzAktiv != NULL )
			 {
				disableAuswahl();
				buttonReferenzLoeschen->Enabled = true;
				anzeigeExterneReferenzAktiv();
			 }
		 }

private: System::Void buttonReferenzNeu_Click(System::Object ^  sender, System::EventArgs ^  e)
		 {
			String ^ STR;
			int      anz;
			string relationName;

			if ( relationAktiv != nullptr )
				 QuConvert::systemStr2stdStr ( relationName, relationAktiv );
			if ( relationName == "" ) return;


			anz = pPlanAttribute->getExterneReferenzenAnz( relationName );
			if ( anz > 0 && 
				 ( relationName == "refBegruendung" || relationName == "refExternalCodeList" ||
				   relationName == "refBeschreibung" || relationName == "refKoordinatenListe" ||
				   relationName == "refGrundstuecksverzeichnis" || relationName == "refPflanzliste" ||
				   relationName == "refUmweltbericht" ) )
			{
				MessageBox::Show ( "Die gewählte Referenz darf nur maximal einmal belegt werden" );
				return;
			}

			pExterneReferenzAktiv = new ExterneReferenz();
			pExterneReferenzAktiv->relationName = relationName;
			pPlanAttribute->addExterneReferenz ( pExterneReferenzAktiv );

			STR = String::Concat ( "Referenz", Convert::ToString ( anz-1 ) );
			comboBoxReferenz->Items->Add ( dynamic_cast<String^>( STR ) );
			comboBoxReferenz->SelectedIndex = anz-1;

			disableAuswahl();
			buttonReferenzLoeschen->Enabled = true;
			anzeigeExterneReferenzAktiv();
		 }

private: System::Void buttonReferenzLoeschen_Click(System::Object ^  sender, System::EventArgs ^  e)
		 {
			 int      i;
			 String ^ STR;
			 string   relationName;
			 int      index = comboBoxReferenz->SelectedIndex;
			 
			 if ( index < 0 ) return;

			 if ( relationAktiv != nullptr )
				 QuConvert::systemStr2stdStr ( relationName, relationAktiv );
			 if ( relationName == "" ) return;

			 pExterneReferenzAktiv = pPlanAttribute->getExterneReferenz ( relationName, index );
			 if ( pExterneReferenzAktiv != NULL )
			 {
				 pPlanAttribute->deleteExterneReferenz ( relationName, index );
				 comboBoxReferenz->Items->Clear();

				for ( i = 0; i < pPlanAttribute->getExterneReferenzenAnz( relationName ); i++ )
				{
					 STR = String::Concat ( "Referenz", Convert::ToString ( i ) );
					 comboBoxReferenz->Items->Add ( dynamic_cast<String^>( STR ) );
				}
			 }

			 comboBoxReferenz->SelectedIndex = -1;
			 comboBoxReferenz->Text = "";
			 pExterneReferenzAktiv = NULL;
			 enableAuswahl();
 			 anzeigeExterneReferenzAktiv();		 
		 }

private: System::Void buttonExterneReferenzVerwerfen_Click(System::Object ^  sender, System::EventArgs ^  e)
		 {
			 enableAuswahl();
			 pExterneReferenzAktiv = NULL;	
			 comboBoxReferenz->SelectedIndex = -1;
			 anzeigeExterneReferenzAktiv();		 
		 }

private: System::Void buttonExterneReferenzUebernehmen_Click(System::Object ^  sender, System::EventArgs ^  e)
		 {
			 updateExterneReferenzAktiv();

			 enableAuswahl();
			 pExterneReferenzAktiv = NULL;	
			 comboBoxReferenz->SelectedIndex = -1;
			 anzeigeExterneReferenzAktiv();		 
		 }

private: System::Void buttonAbbrechen_Click(System::Object ^  sender, System::EventArgs ^  e)
		 {
//			 *pPlanAttribute = *pPlanAttributeOld;
			 delete pPlanAttribute;
			 pViewer->setPlanAttribute ( pPlanAttributeOld );
		 }

private: System::Void buttonBeenden_Click(System::Object ^  sender, System::EventArgs ^  e)
		 {
			 delete pPlanAttributeOld;
		 }

};
}
