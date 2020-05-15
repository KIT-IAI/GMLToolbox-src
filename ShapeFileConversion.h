#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "XPlanGML.h"
#include "GmlSchema.h"
#include "ShpFileObjekte.h"
#include "Convert.h"
#include "shptypeenum.h"
#include "ShpFileNamenKonv.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "GenerateKomplexAttribute.h"

using namespace std;

	/// <summary> 
	/// Festlegung der Abbildungsregeln eines Shapefiles auf eine XPlanGML-Klasse
	///
	/// </summary>
	public ref class ShapeFileConversion : public System::Windows::Forms::Form
	{
	public: 
		ShapeFileConversion( XPlanGML * pXPlanGML, ShapeFileObjekt * pShapeFileObjektP,
							           GmlDictionary * pCodeListsP,  GmlDictionary * pExternalCodeListsP,
							           PlanAttribute * pPlanAttributeP, bool plaeneAusShapeErzeugenP, bool nachrichtlicheUebernahmeP )
		{
			std::vector<Bereich*> bereiche;
			Bereich				      * pBereich;
			unsigned int          i, bereichNummer;
			String              ^ STR;

			InitializeComponent();

      m_pXPlanGML              = pXPlanGML;
      m_pFeatures              = m_pXPlanGML->getFeatures();
      m_pGmlSchema             = m_pFeatures->getGmlSchema();
			pShapeFileObjekt         = pShapeFileObjektP;
			pGmlKlasse		           = pShapeFileObjekt->getGmlKlasse();
			pCodeLists               = pCodeListsP;
			pExternalCodeLists       = pExternalCodeListsP;
			plaeneAusShapeErzeugen   = plaeneAusShapeErzeugenP;
      nachrichtlicheUebernahme = nachrichtlicheUebernahmeP;

      if ( m_pFeatures->getGmlTyp() == XPLANGML_4_0 ||  m_pFeatures->getGmlTyp() == XPLANGML_4_1  )
      {
        if ( nachrichtlicheUebernahme == true )
          labelRechtscharakter->Text = "Nachrichtliche Übernahme";
        else
          labelRechtscharakter->Text = "Planinhalt";
      }
      else
      {
        labelRechtscharakter->Visible = false;
        label3->Visible               = false;
      }

			initialize();
			
			pPlanAttributeP->getBereiche ( bereiche );
			for ( i = 0; i < bereiche.size(); i++ )
			{
				pBereich      = bereiche[i];
				bereichNummer = pBereich->nummer;

				STR = QuConvert::ToString ( bereichNummer );
				comboBoxBereiche->Items->Add ( STR );
			}

			bereichNummer = pShapeFileObjekt->getBereichNummer();
			STR = QuConvert::ToString ( bereichNummer );

			if ( comboBoxBereiche->Items->Contains ( STR ) )
			{
				comboBoxBereiche->SelectedItem = STR;
			}
			else
			{
				comboBoxBereiche->SelectedIndex = 0;
				bereichNummer = bereiche[0]->nummer;
				pShapeFileObjekt->setBereichNummer ( bereichNummer );
			}
		}
        
	private:
    XPlanGML          * m_pXPlanGML;							// XPlanGML-Modul
    Features          * m_pFeatures;							// Aggregation aller XPlanGML-Features
		GmlSchema         * m_pGmlSchema;							// XPlanGML-Schema
		ShapeFileObjekt   * pShapeFileObjekt;					// Repräsentation des gewählten Shapefiles
		GmlDictionary	    * pCodeLists;								// GML-Dictionary Enumerationen
		GmlDictionary	    * pExternalCodeLists;				// GML-Dictionary externe Codelisten
		GmlKlasse		      * pGmlKlasse;								// Repräsenation der gewählten XPlanGML-Klasse
		ShapeFileAttribut * pAttributActive;					// Aktuell ausgewähltes Shapefile-Attribut
		GmlAttribut       * pGmlAttributActive;				// Aktuell ausgewähltes Attribut der XPlanGML-Klasse
		StandardAttribut  * pStandardAttributActive;  // Aktuell ausgewähltes Attribut der XPlanGML-Klasse für die Belegung mit festen Werten
		int                 indexZuordnung;						// Index bei der Zuordnung von XPlanGML-Attributen zu einem Shapefile-Attribut
		bool				        plaeneAusShapeErzeugen;		// Es werden mehrere XPlanGML Plan-Objekte aus einem Satz von Shapefiles erzeugt 
    bool                nachrichtlicheUebernahme; // Bei true wird das erzeugte XPlanGML-Objekt als nachrichtliche Ünernahme integriert

	private: System::Windows::Forms::TextBox ^  textBoxGmlKlasse;
	private: System::Windows::Forms::Label ^  labelGmlKlasse;
	private: System::Windows::Forms::ListBox ^  listBoxShapeAttribute;
	private: System::Windows::Forms::Label ^  labelShapefileAttribute;
	private: System::Windows::Forms::GroupBox ^  groupBox1;
	private: System::Windows::Forms::TextBox ^  textBoxShapeAttributTyp;
	private: System::Windows::Forms::Label ^  label1;
	private: System::Windows::Forms::RadioButton ^  radioButtonZuordnung1;
	private: System::Windows::Forms::ComboBox ^  comboBoxZuordnung;
	private: System::Windows::Forms::RadioButton ^  radioButtonZuordnung2;
	private: System::Windows::Forms::TextBox ^  textBoxZuordnung1;
	private: System::Windows::Forms::TextBox ^  textBoxZuordnung2;
	private: System::Windows::Forms::Button ^  buttonZuordnungUebernehmen;
	private: System::Windows::Forms::GroupBox ^  groupBoxGmlAttribute;
	private: System::Windows::Forms::ComboBox ^  comboBoxGmlAttribute;
	private: System::Windows::Forms::TextBox ^  textBoxGmlAttributTyp;
	private: System::Windows::Forms::Label ^  label2;
	private: System::Windows::Forms::ComboBox ^  comboBoxVorgabewert;
	private: System::Windows::Forms::TextBox ^  textBoxVorgabewert;
	private: System::Windows::Forms::Button ^  buttonVorgabewertUebernehmen;
	private: System::Windows::Forms::Button ^  buttonVorgabewertLoeschen;
	private: System::Windows::Forms::ListBox ^  listBoxVorgabewerte;
	private: System::Windows::Forms::Button ^  buttonBeenden;
	private: System::Windows::Forms::TextBox ^  generAttributName;
	private: System::Windows::Forms::Label ^  GenerAttributNameLabel;
	private: System::Windows::Forms::ComboBox ^  comboBoxBereiche;
	private: System::Windows::Forms::Label ^  BereichLabel;
	private: System::Windows::Forms::Label ^  labelAttributName;
	private: System::Windows::Forms::ComboBox ^  comboBoxAttributName;
	private: System::Windows::Forms::CheckBox ^  checkBoxPlanId;
  private: System::Windows::Forms::Label^  labelRechtscharakter;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::Label^  labelIndex;
  private: System::Windows::Forms::TextBox^  textBoxIndex;

	private:
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
			this->textBoxGmlKlasse = (gcnew System::Windows::Forms::TextBox());
			this->labelGmlKlasse = (gcnew System::Windows::Forms::Label());
			this->listBoxShapeAttribute = (gcnew System::Windows::Forms::ListBox());
			this->labelShapefileAttribute = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBoxPlanId = (gcnew System::Windows::Forms::CheckBox());
			this->labelAttributName = (gcnew System::Windows::Forms::Label());
			this->comboBoxAttributName = (gcnew System::Windows::Forms::ComboBox());
			this->GenerAttributNameLabel = (gcnew System::Windows::Forms::Label());
			this->generAttributName = (gcnew System::Windows::Forms::TextBox());
			this->buttonZuordnungUebernehmen = (gcnew System::Windows::Forms::Button());
			this->textBoxZuordnung1 = (gcnew System::Windows::Forms::TextBox());
			this->comboBoxZuordnung = (gcnew System::Windows::Forms::ComboBox());
			this->radioButtonZuordnung1 = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxShapeAttributTyp = (gcnew System::Windows::Forms::TextBox());
			this->radioButtonZuordnung2 = (gcnew System::Windows::Forms::RadioButton());
			this->textBoxZuordnung2 = (gcnew System::Windows::Forms::TextBox());
			this->groupBoxGmlAttribute = (gcnew System::Windows::Forms::GroupBox());
			this->labelRechtscharakter = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->listBoxVorgabewerte = (gcnew System::Windows::Forms::ListBox());
			this->buttonVorgabewertLoeschen = (gcnew System::Windows::Forms::Button());
			this->buttonVorgabewertUebernehmen = (gcnew System::Windows::Forms::Button());
			this->textBoxVorgabewert = (gcnew System::Windows::Forms::TextBox());
			this->comboBoxVorgabewert = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBoxGmlAttributTyp = (gcnew System::Windows::Forms::TextBox());
			this->comboBoxGmlAttribute = (gcnew System::Windows::Forms::ComboBox());
			this->buttonBeenden = (gcnew System::Windows::Forms::Button());
			this->comboBoxBereiche = (gcnew System::Windows::Forms::ComboBox());
			this->BereichLabel = (gcnew System::Windows::Forms::Label());
			this->textBoxIndex = (gcnew System::Windows::Forms::TextBox());
			this->labelIndex = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			this->groupBoxGmlAttribute->SuspendLayout();
			this->SuspendLayout();
			// 
			// textBoxGmlKlasse
			// 
			this->textBoxGmlKlasse->Location = System::Drawing::Point(8, 16);
			this->textBoxGmlKlasse->Name = L"textBoxGmlKlasse";
			this->textBoxGmlKlasse->ReadOnly = true;
			this->textBoxGmlKlasse->Size = System::Drawing::Size(200, 20);
			this->textBoxGmlKlasse->TabIndex = 0;
			this->textBoxGmlKlasse->Text = L"Gml-Klasse";
			// 
			// labelGmlKlasse
			// 
			this->labelGmlKlasse->Location = System::Drawing::Point(8, 40);
			this->labelGmlKlasse->Name = L"labelGmlKlasse";
			this->labelGmlKlasse->Size = System::Drawing::Size(144, 16);
			this->labelGmlKlasse->TabIndex = 1;
			this->labelGmlKlasse->Text = L"Zugeordnete GML-Klasse";
			// 
			// listBoxShapeAttribute
			// 
			this->listBoxShapeAttribute->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->listBoxShapeAttribute->Location = System::Drawing::Point(8, 72);
			this->listBoxShapeAttribute->Name = L"listBoxShapeAttribute";
			this->listBoxShapeAttribute->ScrollAlwaysVisible = true;
			this->listBoxShapeAttribute->Size = System::Drawing::Size(96, 134);
			this->listBoxShapeAttribute->TabIndex = 2;
			this->listBoxShapeAttribute->SelectedIndexChanged += gcnew System::EventHandler(this, &ShapeFileConversion::listBoxShapeAttribute_SelectedIndexChanged);
			// 
			// labelShapefileAttribute
			// 
			this->labelShapefileAttribute->Location = System::Drawing::Point(8, 216);
			this->labelShapefileAttribute->Name = L"labelShapefileAttribute";
			this->labelShapefileAttribute->Size = System::Drawing::Size(100, 16);
			this->labelShapefileAttribute->TabIndex = 3;
			this->labelShapefileAttribute->Text = L"Shapefile-Attribute";
			// 
			// groupBox1
			// 
			this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->groupBox1->Controls->Add(this->labelIndex);
			this->groupBox1->Controls->Add(this->textBoxIndex);
			this->groupBox1->Controls->Add(this->checkBoxPlanId);
			this->groupBox1->Controls->Add(this->labelAttributName);
			this->groupBox1->Controls->Add(this->comboBoxAttributName);
			this->groupBox1->Controls->Add(this->GenerAttributNameLabel);
			this->groupBox1->Controls->Add(this->generAttributName);
			this->groupBox1->Controls->Add(this->buttonZuordnungUebernehmen);
			this->groupBox1->Controls->Add(this->textBoxZuordnung1);
			this->groupBox1->Controls->Add(this->comboBoxZuordnung);
			this->groupBox1->Controls->Add(this->radioButtonZuordnung1);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->textBoxShapeAttributTyp);
			this->groupBox1->Controls->Add(this->radioButtonZuordnung2);
			this->groupBox1->Controls->Add(this->textBoxZuordnung2);
			this->groupBox1->Location = System::Drawing::Point(120, 72);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(376, 176);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Zuordnung Shapefile-Attribute -> GML-Attribute";
			// 
			// checkBoxPlanId
			// 
			this->checkBoxPlanId->Location = System::Drawing::Point(216, 24);
			this->checkBoxPlanId->Name = L"checkBoxPlanId";
			this->checkBoxPlanId->Size = System::Drawing::Size(104, 24);
			this->checkBoxPlanId->TabIndex = 13;
			this->checkBoxPlanId->Text = L"Ist Plan-Id";
			this->checkBoxPlanId->CheckedChanged += gcnew System::EventHandler(this, &ShapeFileConversion::checkBoxPlanId_CheckedChanged);
			// 
			// labelAttributName
			// 
			this->labelAttributName->Location = System::Drawing::Point(216, 144);
			this->labelAttributName->Name = L"labelAttributName";
			this->labelAttributName->Size = System::Drawing::Size(100, 16);
			this->labelAttributName->TabIndex = 12;
			this->labelAttributName->Text = L"Attribut-Name";
			// 
			// comboBoxAttributName
			// 
			this->comboBoxAttributName->Location = System::Drawing::Point(208, 120);
			this->comboBoxAttributName->Name = L"comboBoxAttributName";
			this->comboBoxAttributName->Size = System::Drawing::Size(160, 21);
			this->comboBoxAttributName->TabIndex = 11;
			// 
			// GenerAttributNameLabel
			// 
			this->GenerAttributNameLabel->Location = System::Drawing::Point(24, 144);
			this->GenerAttributNameLabel->Name = L"GenerAttributNameLabel";
			this->GenerAttributNameLabel->Size = System::Drawing::Size(160, 23);
			this->GenerAttributNameLabel->TabIndex = 10;
			this->GenerAttributNameLabel->Text = L"Name Generisches Attribut";
			// 
			// generAttributName
			// 
			this->generAttributName->Location = System::Drawing::Point(24, 120);
			this->generAttributName->Name = L"generAttributName";
			this->generAttributName->Size = System::Drawing::Size(128, 20);
			this->generAttributName->TabIndex = 9;
			// 
			// buttonZuordnungUebernehmen
			// 
			this->buttonZuordnungUebernehmen->Location = System::Drawing::Point(288, 86);
			this->buttonZuordnungUebernehmen->Name = L"buttonZuordnungUebernehmen";
			this->buttonZuordnungUebernehmen->Size = System::Drawing::Size(80, 23);
			this->buttonZuordnungUebernehmen->TabIndex = 8;
			this->buttonZuordnungUebernehmen->Text = L"Übernehmen";
			this->buttonZuordnungUebernehmen->Click += gcnew System::EventHandler(this, &ShapeFileConversion::buttonZuordnungUebernehmen_Click);
			// 
			// textBoxZuordnung1
			// 
			this->textBoxZuordnung1->Location = System::Drawing::Point(24, 56);
			this->textBoxZuordnung1->Name = L"textBoxZuordnung1";
			this->textBoxZuordnung1->ReadOnly = true;
			this->textBoxZuordnung1->Size = System::Drawing::Size(160, 20);
			this->textBoxZuordnung1->TabIndex = 6;
			this->textBoxZuordnung1->Text = L"Zuordnung 1";
			// 
			// comboBoxZuordnung
			// 
			this->comboBoxZuordnung->Location = System::Drawing::Point(24, 88);
			this->comboBoxZuordnung->Name = L"comboBoxZuordnung";
			this->comboBoxZuordnung->Size = System::Drawing::Size(258, 21);
			this->comboBoxZuordnung->Sorted = true;
			this->comboBoxZuordnung->TabIndex = 3;
			this->comboBoxZuordnung->Text = L" ";
			this->comboBoxZuordnung->SelectedIndexChanged += gcnew System::EventHandler(this, &ShapeFileConversion::comboBoxZuordnung_SelectedIndexChanged);
			// 
			// radioButtonZuordnung1
			// 
			this->radioButtonZuordnung1->Location = System::Drawing::Point(8, 56);
			this->radioButtonZuordnung1->Name = L"radioButtonZuordnung1";
			this->radioButtonZuordnung1->Size = System::Drawing::Size(16, 24);
			this->radioButtonZuordnung1->TabIndex = 2;
			this->radioButtonZuordnung1->CheckedChanged += gcnew System::EventHandler(this, &ShapeFileConversion::radioButtonZuordnung1_CheckedChanged);
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(88, 24);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(128, 16);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Datentyp Shape-Attribut";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// textBoxShapeAttributTyp
			// 
			this->textBoxShapeAttributTyp->Location = System::Drawing::Point(16, 20);
			this->textBoxShapeAttributTyp->Name = L"textBoxShapeAttributTyp";
			this->textBoxShapeAttributTyp->ReadOnly = true;
			this->textBoxShapeAttributTyp->Size = System::Drawing::Size(64, 20);
			this->textBoxShapeAttributTyp->TabIndex = 0;
			this->textBoxShapeAttributTyp->Text = L"Datentyp";
			// 
			// radioButtonZuordnung2
			// 
			this->radioButtonZuordnung2->Location = System::Drawing::Point(192, 56);
			this->radioButtonZuordnung2->Name = L"radioButtonZuordnung2";
			this->radioButtonZuordnung2->Size = System::Drawing::Size(16, 24);
			this->radioButtonZuordnung2->TabIndex = 5;
			this->radioButtonZuordnung2->CheckedChanged += gcnew System::EventHandler(this, &ShapeFileConversion::radioButtonZuordnung2_CheckedChanged);
			// 
			// textBoxZuordnung2
			// 
			this->textBoxZuordnung2->Location = System::Drawing::Point(208, 56);
			this->textBoxZuordnung2->Name = L"textBoxZuordnung2";
			this->textBoxZuordnung2->ReadOnly = true;
			this->textBoxZuordnung2->Size = System::Drawing::Size(160, 20);
			this->textBoxZuordnung2->TabIndex = 7;
			this->textBoxZuordnung2->Text = L"Zuordnung 2";
			// 
			// groupBoxGmlAttribute
			// 
			this->groupBoxGmlAttribute->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBoxGmlAttribute->Controls->Add(this->labelRechtscharakter);
			this->groupBoxGmlAttribute->Controls->Add(this->label3);
			this->groupBoxGmlAttribute->Controls->Add(this->listBoxVorgabewerte);
			this->groupBoxGmlAttribute->Controls->Add(this->buttonVorgabewertLoeschen);
			this->groupBoxGmlAttribute->Controls->Add(this->buttonVorgabewertUebernehmen);
			this->groupBoxGmlAttribute->Controls->Add(this->textBoxVorgabewert);
			this->groupBoxGmlAttribute->Controls->Add(this->comboBoxVorgabewert);
			this->groupBoxGmlAttribute->Controls->Add(this->label2);
			this->groupBoxGmlAttribute->Controls->Add(this->textBoxGmlAttributTyp);
			this->groupBoxGmlAttribute->Controls->Add(this->comboBoxGmlAttribute);
			this->groupBoxGmlAttribute->Location = System::Drawing::Point(8, 248);
			this->groupBoxGmlAttribute->Name = L"groupBoxGmlAttribute";
			this->groupBoxGmlAttribute->Size = System::Drawing::Size(488, 241);
			this->groupBoxGmlAttribute->TabIndex = 5;
			this->groupBoxGmlAttribute->TabStop = false;
			this->groupBoxGmlAttribute->Text = L"Vorbesetzung von GML-Attributen";
			// 
			// labelRechtscharakter
			// 
			this->labelRechtscharakter->AutoSize = true;
			this->labelRechtscharakter->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->labelRechtscharakter->ForeColor = System::Drawing::Color::Red;
			this->labelRechtscharakter->Location = System::Drawing::Point(124, 93);
			this->labelRechtscharakter->Name = L"labelRechtscharakter";
			this->labelRechtscharakter->Size = System::Drawing::Size(41, 13);
			this->labelRechtscharakter->TabIndex = 13;
			this->labelRechtscharakter->Text = L"label4";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(9, 93);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(109, 13);
			this->label3->TabIndex = 12;
			this->label3->Text = L"Rechtscharakter: ";
			// 
			// listBoxVorgabewerte
			// 
			this->listBoxVorgabewerte->BackColor = System::Drawing::SystemColors::InactiveBorder;
			this->listBoxVorgabewerte->Location = System::Drawing::Point(12, 109);
			this->listBoxVorgabewerte->Name = L"listBoxVorgabewerte";
			this->listBoxVorgabewerte->ScrollAlwaysVisible = true;
			this->listBoxVorgabewerte->Size = System::Drawing::Size(472, 121);
			this->listBoxVorgabewerte->TabIndex = 11;
			// 
			// buttonVorgabewertLoeschen
			// 
			this->buttonVorgabewertLoeschen->Location = System::Drawing::Point(400, 48);
			this->buttonVorgabewertLoeschen->Name = L"buttonVorgabewertLoeschen";
			this->buttonVorgabewertLoeschen->Size = System::Drawing::Size(75, 23);
			this->buttonVorgabewertLoeschen->TabIndex = 10;
			this->buttonVorgabewertLoeschen->Text = L"Löschen";
			this->buttonVorgabewertLoeschen->Click += gcnew System::EventHandler(this, &ShapeFileConversion::buttonVorgabewertLoeschen_Click);
			// 
			// buttonVorgabewertUebernehmen
			// 
			this->buttonVorgabewertUebernehmen->Location = System::Drawing::Point(272, 48);
			this->buttonVorgabewertUebernehmen->Name = L"buttonVorgabewertUebernehmen";
			this->buttonVorgabewertUebernehmen->Size = System::Drawing::Size(80, 23);
			this->buttonVorgabewertUebernehmen->TabIndex = 9;
			this->buttonVorgabewertUebernehmen->Text = L"Übernehmen";
			this->buttonVorgabewertUebernehmen->Click += gcnew System::EventHandler(this, &ShapeFileConversion::buttonVorgabewertUebernehmen_Click);
			// 
			// textBoxVorgabewert
			// 
			this->textBoxVorgabewert->ImeMode = System::Windows::Forms::ImeMode::NoControl;
			this->textBoxVorgabewert->Location = System::Drawing::Point(272, 24);
			this->textBoxVorgabewert->Name = L"textBoxVorgabewert";
			this->textBoxVorgabewert->Size = System::Drawing::Size(200, 20);
			this->textBoxVorgabewert->TabIndex = 8;
			// 
			// comboBoxVorgabewert
			// 
			this->comboBoxVorgabewert->Location = System::Drawing::Point(272, 24);
			this->comboBoxVorgabewert->Name = L"comboBoxVorgabewert";
			this->comboBoxVorgabewert->Size = System::Drawing::Size(200, 21);
			this->comboBoxVorgabewert->TabIndex = 7;
			this->comboBoxVorgabewert->Text = L"Vorgabewert";
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(192, 24);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(80, 21);
			this->label2->TabIndex = 6;
			this->label2->Text = L"GML-Attribut";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// textBoxGmlAttributTyp
			// 
			this->textBoxGmlAttributTyp->Location = System::Drawing::Point(8, 56);
			this->textBoxGmlAttributTyp->Name = L"textBoxGmlAttributTyp";
			this->textBoxGmlAttributTyp->ReadOnly = true;
			this->textBoxGmlAttributTyp->Size = System::Drawing::Size(256, 20);
			this->textBoxGmlAttributTyp->TabIndex = 5;
			this->textBoxGmlAttributTyp->Text = L"Datentyp GML-Attribut";
			// 
			// comboBoxGmlAttribute
			// 
			this->comboBoxGmlAttribute->Location = System::Drawing::Point(8, 24);
			this->comboBoxGmlAttribute->Name = L"comboBoxGmlAttribute";
			this->comboBoxGmlAttribute->Size = System::Drawing::Size(176, 21);
			this->comboBoxGmlAttribute->Sorted = true;
			this->comboBoxGmlAttribute->TabIndex = 4;
			this->comboBoxGmlAttribute->Text = L"GML-Attribut";
			this->comboBoxGmlAttribute->SelectedIndexChanged += gcnew System::EventHandler(this, &ShapeFileConversion::comboBoxGmlAttribute_SelectedIndexChanged);
			// 
			// buttonBeenden
			// 
			this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->buttonBeenden->Location = System::Drawing::Point(416, 16);
			this->buttonBeenden->Name = L"buttonBeenden";
			this->buttonBeenden->Size = System::Drawing::Size(75, 23);
			this->buttonBeenden->TabIndex = 6;
			this->buttonBeenden->Text = L"Beenden";
			this->buttonBeenden->Click += gcnew System::EventHandler(this, &ShapeFileConversion::buttonBeenden_Click);
			// 
			// comboBoxBereiche
			// 
			this->comboBoxBereiche->Location = System::Drawing::Point(232, 16);
			this->comboBoxBereiche->Name = L"comboBoxBereiche";
			this->comboBoxBereiche->Size = System::Drawing::Size(121, 21);
			this->comboBoxBereiche->TabIndex = 7;
			this->comboBoxBereiche->Text = L"comboBox1";
			// 
			// BereichLabel
			// 
			this->BereichLabel->Location = System::Drawing::Point(232, 40);
			this->BereichLabel->Name = L"BereichLabel";
			this->BereichLabel->Size = System::Drawing::Size(100, 16);
			this->BereichLabel->TabIndex = 8;
			this->BereichLabel->Text = L"Bereich Nummer";
			// 
			// textBoxIndex
			// 
			this->textBoxIndex->Location = System::Drawing::Point(159, 120);
			this->textBoxIndex->Name = L"textBoxIndex";
			this->textBoxIndex->Size = System::Drawing::Size(43, 20);
			this->textBoxIndex->TabIndex = 14;
			// 
			// labelIndex
			// 
			this->labelIndex->AutoSize = true;
			this->labelIndex->Location = System::Drawing::Point(159, 142);
			this->labelIndex->Name = L"labelIndex";
			this->labelIndex->Size = System::Drawing::Size(33, 13);
			this->labelIndex->TabIndex = 15;
			this->labelIndex->Text = L"Index";
			// 
			// ShapeFileConversion
			// 
			this->AcceptButton = this->buttonBeenden;
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->CancelButton = this->buttonBeenden;
			this->ClientSize = System::Drawing::Size(504, 501);
			this->Controls->Add(this->BereichLabel);
			this->Controls->Add(this->comboBoxBereiche);
			this->Controls->Add(this->buttonBeenden);
			this->Controls->Add(this->groupBoxGmlAttribute);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->labelShapefileAttribute);
			this->Controls->Add(this->listBoxShapeAttribute);
			this->Controls->Add(this->labelGmlKlasse);
			this->Controls->Add(this->textBoxGmlKlasse);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"ShapeFileConversion";
			this->Text = L"Konversion der Shape-Datei";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBoxGmlAttribute->ResumeLayout(false);
			this->groupBoxGmlAttribute->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

////////////////////////////////////////////////////////////////////////////////
//  Commandhandler ListBox "Shapefile-Attribute"														  //
//  Auswahl eines Shapefile-Attributes zur Abbildung auf ein XPlanGML-Attribut//
////////////////////////////////////////////////////////////////////////////////
private: System::Void listBoxShapeAttribute_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	String ^ STR;
	string   stdString;

	STR =  dynamic_cast<String^> ( listBoxShapeAttribute->SelectedItem );
	QuConvert::systemStr2stdStr ( stdString, STR );

	pAttributActive = pShapeFileObjekt->getShapeFileAttribut ( stdString );
	if ( pAttributActive != NULL ) 
	{
		displayAttributParameter();
		if ( plaeneAusShapeErzeugen )
		{
			if ( pAttributActive->getShapeFileAttributName() == pShapeFileObjekt->getAttributNamePlanId() )
				checkBoxPlanId->Checked  = true;
			else
				checkBoxPlanId->Checked  = false;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler RadioButton "Zuordnung 1																   //
//  Zuordnung von bis zu 2 XPlanGML-Attributen zu einem Shapefile-Attribut   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonZuordnung1_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	if ( radioButtonZuordnung1->Checked )
		indexZuordnung = 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler RadioButton "Zuordnung 2																   //
//  Zuordnung von bis zu 2 XPlanGML-Attributen zu einem Shapefile-Attribut   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonZuordnung2_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	if ( radioButtonZuordnung2->Checked )
		indexZuordnung = 1;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler ComboBox "Auswahl eines XPlanGML-Attributs"							 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxZuordnung_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	String			   ^ STR;
	string				 stdString;
	vector<GmlAttribut*> gmlAttribute;
	size_t               i;
	ATTRIBUT_TYP         shapeAttributTyp, gmlAttributTyp;
	

	STR =  dynamic_cast<String^> ( comboBoxZuordnung->SelectedItem );
	QuConvert::systemStr2stdStr ( stdString, STR );

	comboBoxAttributName->Items->Clear();

	if ( stdString == " " ) pGmlAttributActive = NULL;
	else		  		    pGmlAttributActive = pGmlKlasse->getGmlAttribut ( stdString );

	if ( pGmlAttributActive != NULL )
	{
    if ( pGmlAttributActive->getGmlAttributTyp () == _REFERENZ ||
	  		 pGmlAttributActive->getGmlAttributTyp () == _DATA_TYPE )
		{
      if ( pGmlAttributActive->getGmlAttributName() == "xplan:hatGenerStringAttribut" || 
           pGmlAttributActive->getGmlAttributName() == "xplan:hatGenerIntegerAttribut" ||
           pGmlAttributActive->getGmlAttributName() == "xplan:hatGenerDoubleAttribut" ||
           pGmlAttributActive->getGmlAttributName() == "xplan:hatGenerDateAttribut"    ||
           pGmlAttributActive->getGmlAttributName() == "xplan:hatGenerURLAttribut" )
      {
        generAttributName->Visible = true;
        GenerAttributNameLabel->Visible = true;
        generAttributName->Text = nullptr;
        comboBoxAttributName->Visible = false;
        comboBoxAttributName->SelectedIndex = -1;
        labelAttributName->Visible = false;
				textBoxIndex->Visible = false;
				labelIndex->Visible   = false;
      }
      else
      {
			  generAttributName->Visible = false;
			  GenerAttributNameLabel->Visible = false;
			  comboBoxAttributName->Visible = true;
			  labelAttributName->Visible = true;
        generAttributName->Text = nullptr;
        comboBoxAttributName->SelectedIndex = -1;
				textBoxIndex->Visible = true;
				labelIndex->Visible   = true;
			}

			string referenzTyp = pGmlAttributActive->getReferenziertesElement();
			GmlKlasse * pReferenzierteKlasse = m_pGmlSchema->getGmlKlasseFromElementName ( referenzTyp );
			size_t attributAnz = pReferenzierteKlasse->getGmlAttribute ( gmlAttribute );
			for ( i = 0; i < attributAnz; i++ )
			{
				GmlAttribut * pGmlAttribut = gmlAttribute[i];
				gmlAttributTyp   = pGmlAttribut->getGmlAttributTyp();
				shapeAttributTyp = pAttributActive->getShapeAttributDatentyp();

				if ( ( shapeAttributTyp == _STRING &&
					  ( gmlAttributTyp == _STRING || gmlAttributTyp == _ENUMERATION || gmlAttributTyp == _BOOLEAN || gmlAttributTyp == _DATE ||
              gmlAttributTyp == _EXTERNAL_CODE_LIST || gmlAttributTyp == _ANY_URI ) )   ||
					 ( shapeAttributTyp == _DOUBLE &&
					  ( gmlAttributTyp == _DOUBLE || gmlAttributTyp == _ANGLE  ||  gmlAttributTyp == _LENGTH || gmlAttributTyp == _AREA  ||  gmlAttributTyp == _VOLUME  || gmlAttributTyp == _STRING ) ) ||
					 ( shapeAttributTyp == _DATE &&
					  ( gmlAttributTyp == _DATE || gmlAttributTyp == _STRING ) ) || 
					  ( shapeAttributTyp == _INTEGER && 
			          ( gmlAttributTyp == _INTEGER || gmlAttributTyp == _ANGLE  ||  gmlAttributTyp == _LENGTH || gmlAttributTyp == _AREA  ||  gmlAttributTyp == _VOLUME   || gmlAttributTyp == _STRING ) ) )
				{
					stdString = pGmlAttribut->getGmlAttributName();
					comboBoxAttributName->Items->Add ( QuConvert::ToString ( stdString ) );
				}
			}
			comboBoxAttributName->SelectedItem = nullptr;
		}
		else
		{
			generAttributName->Visible = false;
			GenerAttributNameLabel->Visible = false;
			comboBoxAttributName->Visible = false;
			labelAttributName->Visible = false;
			textBoxIndex->Visible = false;
			labelIndex->Visible = false;
		}
	}
	else
	{
		generAttributName->Visible = false;
		GenerAttributNameLabel->Visible = false;
		comboBoxAttributName->Visible = false;
		labelAttributName->Visible = false;
		textBoxIndex->Visible = false;
		labelIndex->Visible = false;
	}

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler ComboBox "Attribut-Name"																	 //
//  Bei der Zuordnung com komplexen XPlanGML-Datentypen wird hier das        //
//  Unter-Attribut ausgewählt, auf das ein Shapefile-Attribut abgebildet wird//
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxGmlAttribute_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	String			       ^ STR;
	String			       ^ STANDARD_ATTRIBUT_WERT;
	string               attributName;
	string			      	 stdString;
	string               schluesselText;
	string               schluesselNummer;
	unsigned int         i;
	bool				         success;
	GmlAttribut        * pGmlAttribut;
	GmlEnumeration     * pEnumeration;
	GmlDictionary      * pCodeListsActive;
	GmlDictionaryEntry * pCodeList = NULL;

	vector<string>   itemNames;
	ATTRIBUT_TYP     typ;


	STR =  dynamic_cast<String^> ( comboBoxGmlAttribute->SelectedItem );
	QuConvert::systemStr2stdStr ( attributName, STR );

	pGmlAttribut = pGmlKlasse->getGmlAttribut ( attributName );
	if ( pGmlAttribut != NULL ) typ = pGmlAttribut->getGmlAttributTyp();
	else						typ = _SONST;

	pStandardAttributActive = pShapeFileObjekt->getStandardAttribut ( pGmlAttribut );
	if ( pStandardAttributActive != NULL )
	{
		stdString = pStandardAttributActive->getDefaultWert();
		STANDARD_ATTRIBUT_WERT = QuConvert::ToString ( stdString );
	}
	else STANDARD_ATTRIBUT_WERT = nullptr;



	switch ( typ )
	{
	case _ENUMERATION:
		pCodeListsActive = pCodeLists;

		stdString = pGmlAttribut->getGmlEnumeration();
		STR = QuConvert::ToString ( stdString );
		textBoxGmlAttributTyp->Text = nullptr;

		if ( pCodeListsActive != NULL )
			pCodeList = pCodeListsActive->getDictionaryEntry ( stdString );
    else
      pCodeList = NULL;

		comboBoxVorgabewert->Visible = true;
		textBoxVorgabewert->Visible = false;
		comboBoxVorgabewert->Items->Clear();
    pEnumeration = m_pGmlSchema->getGmlEnumeration ( stdString );

		if ( pEnumeration != NULL )
		{
			pEnumeration->getEnumerationItemNames ( itemNames );
			for ( i = 0; i < itemNames.size(); i++ )
			{
				stdString = itemNames[i];
				if ( pCodeList != NULL )
				{
					success = pCodeList->getSchluesselText ( stdString, schluesselText );
					if ( success && schluesselText != "" ) stdString = schluesselText;
				}

				STR = QuConvert::ToString ( stdString );
				comboBoxVorgabewert->Items->Add ( STR );
			}
			if ( STANDARD_ATTRIBUT_WERT != nullptr )
			{
				if ( pCodeList != NULL )
				{
					QuConvert::systemStr2stdStr ( schluesselNummer, STANDARD_ATTRIBUT_WERT );
					success = pCodeList->getSchluesselText ( schluesselNummer, schluesselText );
					if ( success && schluesselText != "" )
						STANDARD_ATTRIBUT_WERT = QuConvert::ToString ( schluesselText );
				}
				comboBoxVorgabewert->SelectedItem = STANDARD_ATTRIBUT_WERT;
			}
			else comboBoxVorgabewert->Text = nullptr;
		}

		break;

	  case _EXTERNAL_CODE_LIST:
    pCodeListsActive = pExternalCodeLists;

    stdString = pGmlAttribut->getGmlEnumeration();
    STR = QuConvert::ToString ( stdString );
    textBoxGmlAttributTyp->Text = nullptr;

		if ( pCodeListsActive != NULL )
			pCodeList = pCodeListsActive->getDictionaryEntry ( stdString );
    else
      pCodeList = NULL;

    if ( attributName == "xplan:stylesheetId" || pCodeList == NULL || pCodeList->getCodelistEintraegeAnz() == 0 )
		{
  		comboBoxVorgabewert->Visible = false;
			textBoxVorgabewert->Visible = true;

		  if ( STANDARD_ATTRIBUT_WERT != nullptr )
        textBoxVorgabewert->Text = STANDARD_ATTRIBUT_WERT;
			else
         textBoxVorgabewert->Text = nullptr;

			return;
		}
	
		comboBoxVorgabewert->Visible = true;
		textBoxVorgabewert->Visible = false;
		comboBoxVorgabewert->Items->Clear();
    pEnumeration = m_pGmlSchema->getGmlEnumeration ( stdString );

		if ( pEnumeration != NULL )
		{
			pEnumeration->getEnumerationItemNames ( itemNames );
			for ( i = 0; i < itemNames.size(); i++ )
			{
				stdString = itemNames[i];
				if ( pCodeList != NULL )
				{
					success = pCodeList->getSchluesselText ( stdString, schluesselText );
					if ( success && schluesselText != "" ) stdString = schluesselText;
				}

				STR = QuConvert::ToString ( stdString );
				comboBoxVorgabewert->Items->Add ( STR );
			}
			if ( STANDARD_ATTRIBUT_WERT != nullptr )
			{
				if ( pCodeList != NULL )
				{
					QuConvert::systemStr2stdStr ( schluesselNummer, STANDARD_ATTRIBUT_WERT );
					success = pCodeList->getSchluesselText ( schluesselNummer, schluesselText );
					if ( success && schluesselText != "" )
						STANDARD_ATTRIBUT_WERT = QuConvert::ToString ( schluesselText );
				}
				comboBoxVorgabewert->SelectedItem = STANDARD_ATTRIBUT_WERT;
			}
			else comboBoxVorgabewert->Text = nullptr;
		}

		break;

	case _STRING:
		comboBoxVorgabewert->Visible = false;
		textBoxVorgabewert->Visible = true;
		textBoxGmlAttributTyp->Text = "String";

		if ( STANDARD_ATTRIBUT_WERT != nullptr ) textBoxVorgabewert->Text = STANDARD_ATTRIBUT_WERT;
		else							                       textBoxVorgabewert->Text = nullptr;
		break;

	case _ANY_URI:
		comboBoxVorgabewert->Visible = false;
		textBoxVorgabewert->Visible = true;
		textBoxGmlAttributTyp->Text = "URL";

		if ( STANDARD_ATTRIBUT_WERT != nullptr ) textBoxVorgabewert->Text = STANDARD_ATTRIBUT_WERT;
		else							                       textBoxVorgabewert->Text = nullptr;
		break;

	case _DATE:
		comboBoxVorgabewert->Visible = false;
		textBoxVorgabewert->Visible = true;
		textBoxGmlAttributTyp->Text = "Datum (JJJJ-MM-TT)";

		if ( STANDARD_ATTRIBUT_WERT != nullptr ) textBoxVorgabewert->Text = STANDARD_ATTRIBUT_WERT;
		else							                       textBoxVorgabewert->Text = nullptr;
		break;

	case _DOUBLE:
		comboBoxVorgabewert->Visible = false;
		textBoxVorgabewert->Visible = true;
		textBoxGmlAttributTyp->Text = "Double";

		if ( STANDARD_ATTRIBUT_WERT != nullptr ) textBoxVorgabewert->Text = STANDARD_ATTRIBUT_WERT;
		else							                       textBoxVorgabewert->Text = nullptr;
		break;

	case _INTEGER:
		comboBoxVorgabewert->Visible = false;
		textBoxVorgabewert->Visible = true;
		textBoxGmlAttributTyp->Text = "Integer";

		if ( STANDARD_ATTRIBUT_WERT != nullptr ) textBoxVorgabewert->Text = STANDARD_ATTRIBUT_WERT;
		else							                       textBoxVorgabewert->Text = nullptr;
		break;

	case _BOOLEAN:
		comboBoxVorgabewert->Visible = true;
		textBoxVorgabewert->Visible = false;
		comboBoxVorgabewert->Items->Clear();

		comboBoxVorgabewert->Items->Add ( "true" );
		comboBoxVorgabewert->Items->Add ( "false" );

		if ( STANDARD_ATTRIBUT_WERT != nullptr ) comboBoxVorgabewert->SelectedItem = STANDARD_ATTRIBUT_WERT;
		else							                       comboBoxVorgabewert->Text = nullptr;
		break;

	case _ANGLE:
  case _LENGTH:
  case _AREA:
  case _VOLUME:
		comboBoxVorgabewert->Visible = false;
		textBoxVorgabewert->Visible = true;
		textBoxGmlAttributTyp->Text = "Measure";

		if ( STANDARD_ATTRIBUT_WERT != nullptr ) textBoxVorgabewert->Text = STANDARD_ATTRIBUT_WERT;
		else							                       textBoxVorgabewert->Text = nullptr;
		break;

  case _DATA_TYPE:
  case _REFERENZ:
    {
      Feature              * pComplexAttribut;
      FeatureList          * pPlanObjektList;
      GmlKlasse               * pRefClassGmlTyp;
      string                    refClassName;
      vector<StandardAttribut*> vStandardAttribute;

      GenerateKomplexAttribute ^ pDialog;

      pShapeFileObjekt->getStandardAttribute( pGmlAttribut, vStandardAttribute );

      pPlanObjektList = new FeatureList;

      for ( i = 0; i < vStandardAttribute.size(); i++ )
      {
         pComplexAttribut = vStandardAttribute[i]->getComplexDefaultWert();
				 if ( pComplexAttribut != NULL )
					pPlanObjektList->addCopy( pComplexAttribut );
      }

      pPlanObjektList->setKlassenName ( pGmlKlasse->getGmlKlassenName() );
      pPlanObjektList->setRelationName( attributName );

      refClassName = pGmlAttribut->getReferenziertesElement();
      pRefClassGmlTyp = m_pGmlSchema->getGmlKlasseFromElementName( refClassName );
      pPlanObjektList->setGmlTyp( pRefClassGmlTyp );

      pDialog = gcnew GenerateKomplexAttribute  ( m_pFeatures, pCodeLists, pExternalCodeLists, pPlanObjektList );
      System::Windows::Forms::DialogResult ok = pDialog->ShowDialog();
      if ( ok == System::Windows::Forms::DialogResult::OK )
      {
        pShapeFileObjekt->removeStandardAttribut ( pGmlAttribut );
        for ( i = 0; i < pPlanObjektList->count(); i++ )
        {
            pComplexAttribut = new Feature ( pPlanObjektList->get(i), pPlanObjektList->get(i)->getGmlId() );
            pStandardAttributActive = new StandardAttribut ( pGmlAttribut );
						pStandardAttributActive->setIndex ( i );
            pStandardAttributActive->setComplexDefaultWert( pComplexAttribut );
            pShapeFileObjekt->addStandardAttribut ( pStandardAttributActive );
        }
      }

      delete pPlanObjektList;
      break;
    }

	}
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Übernehmen" (Zuordnung Shapefile-Attribute -->    //
//  GML-Attribute																														 //
//  Übernimmt eine festgelegte Zuordnung																		 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonZuordnungUebernehmen_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	String			   ^ STR;
	string				 stdString;
	GmlAttribut		   * pGmlAttribut;
	size_t    					 gmlAttributAnz;
	vector<GmlAttribut*> gmlAttribute;

	if ( pGmlAttributActive == NULL )
	{
		pAttributActive->removeGmlAttribut ( indexZuordnung );
		textBoxZuordnung2->Text = "";

		gmlAttributAnz = pAttributActive->getGmlAttribute ( gmlAttribute );
		if ( gmlAttributAnz == 0 )
		{
			radioButtonZuordnung2->Enabled = false;
			textBoxZuordnung2->Enabled = false;
			textBoxZuordnung1->Text = "";
		}
		else
		{
			pGmlAttribut = gmlAttribute[0];
			stdString    = pGmlAttribut->getGmlAttributName();
			STR          = QuConvert::ToString ( stdString );

			textBoxZuordnung1->Text = STR;
			comboBoxZuordnung->SelectedItem = STR;
		}
		radioButtonZuordnung1->Checked  = true;
		radioButtonZuordnung2->Checked  = false;
		indexZuordnung = 0;
	}
	else
	{
		pAttributActive->setGmlAttribut ( indexZuordnung, pGmlAttributActive );
		STR = generAttributName->Text;
		if ( STR != nullptr )
		{
			QuConvert::systemStr2stdStr ( stdString, STR );
			pAttributActive->setGenerAttributName ( stdString );
      pAttributActive->setRefAttributName( "xplan:wert" );
      if ( pGmlAttributActive->getReferenziertesElement() == "xplan:XP_StringAttribut" )
         pAttributActive->setRefAttributTyp ( _STRING );
      else
      if ( pGmlAttributActive->getReferenziertesElement() == "xplan:XP_DatumAttribut" )
        pAttributActive->setRefAttributTyp ( _DATE );
      else
      if ( pGmlAttributActive->getReferenziertesElement() == "xplan:XP_URLAttribut" )
        pAttributActive->setRefAttributTyp ( _ANY_URI );
      else
      if ( pGmlAttributActive->getReferenziertesElement() == "xplan:XP_DoubleAttribut" )
        pAttributActive->setRefAttributTyp ( _DOUBLE );
      else
      if ( pGmlAttributActive->getReferenziertesElement() == "xplan:XP_IntegerAttribut" )
        pAttributActive->setRefAttributTyp ( _INTEGER );
	 	}
     else
       pAttributActive->setGenerAttributName ( "" );

		if ( pGmlAttributActive->getGmlAttributTyp() == _REFERENZ  || 
			 pGmlAttributActive->getGmlAttributTyp() == _DATA_TYPE )
		{
			STR = dynamic_cast<String^> (comboBoxAttributName->SelectedItem );
			if ( STR != nullptr )
			{
				QuConvert::systemStr2stdStr ( stdString, STR );
				pAttributActive->setRefAttributName ( stdString );

				pAttributActive->setRefAttributIndex ( Convert::ToInt32 ( textBoxIndex->Text ) );

        string referenzTyp = pGmlAttributActive->getReferenziertesElement();

				GmlKlasse * pReferenzierteKlasse = m_pGmlSchema->getGmlKlasseFromElementName ( referenzTyp );
        if (  pReferenzierteKlasse != NULL )
        {
				  GmlAttribut * pRefAttribut = pReferenzierteKlasse->getGmlAttribut ( stdString );
          if ( pRefAttribut != NULL )
				    pAttributActive->setRefAttributTyp ( pRefAttribut->getGmlAttributTyp() );
        }
			}

		}
		else
		{
			pAttributActive->setRefAttributName ( "" );
			pAttributActive->setRefAttributIndex ( 0 );
			pAttributActive->setRefAttributTyp ( _SONST );
		}


		stdString    = pGmlAttributActive->getGmlAttributName();
		STR          = QuConvert::ToString ( stdString );
		if ( indexZuordnung == 0 ) textBoxZuordnung1->Text = STR;
		else					   textBoxZuordnung2->Text = STR;

		radioButtonZuordnung2->Enabled = true;
		textBoxZuordnung2->Enabled = true;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Übernehmen" (Vorbesetzung von GML-Attributen)		 //
//  Übernimmt eine festgelegte Vorbesetzung																	 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonVorgabewertUebernehmen_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	String		       ^ STR;
	StandardAttribut   * pStandardAttributNew = NULL;
	string		         stdString;
	string				 attributName;
	string               schluesselNummer;
	string               attributWert;
	double               d;
	bool                 success;
	GmlAttribut        * pGmlAttribut;
	ATTRIBUT_TYP         typ;
	GmlDictionary      * pCodeListsActive;
	GmlDictionaryEntry * pCodeList = NULL;

	STR =  dynamic_cast<String^> ( comboBoxGmlAttribute->SelectedItem );
	QuConvert::systemStr2stdStr ( attributName, STR );

	pGmlAttribut = pGmlKlasse->getGmlAttribut ( attributName );
	if ( pGmlAttribut != NULL ) typ = pGmlAttribut->getGmlAttributTyp();
	else						typ = _SONST;

	switch ( typ )
	{
	case _ENUMERATION:
		{
			pCodeListsActive = pCodeLists;

			STR = dynamic_cast<String^> ( comboBoxVorgabewert->SelectedItem );
			QuConvert::systemStr2stdStr ( attributWert, STR );

			stdString = pGmlAttribut->getGmlEnumeration();
			if ( pCodeListsActive != NULL )
				pCodeList = pCodeListsActive->getDictionaryEntry ( stdString );
      else
        pCodeList = NULL;

			if ( pCodeList != NULL )
			{
				success = pCodeList->getSchluesselNummer ( attributWert, schluesselNummer );
				if ( success ) STR = QuConvert::ToString ( schluesselNummer );
			}		
		}
		break;

  case _EXTERNAL_CODE_LIST:
    {
      pCodeListsActive = pExternalCodeLists;

      stdString = pGmlAttribut->getGmlEnumeration();

      if ( pCodeListsActive != NULL )
        pCodeList = pCodeListsActive->getDictionaryEntry ( stdString );
      else
        pCodeList = NULL;

      if ( attributName == "xplan:stylesheetId" || pCodeList == NULL || pCodeList->getCodelistEintraegeAnz() == 0 )
        STR = textBoxVorgabewert->Text;
      else
      {	
        STR = dynamic_cast<String^> ( comboBoxVorgabewert->SelectedItem );
        QuConvert::systemStr2stdStr ( attributWert, STR );

        if ( pCodeList != NULL )
        {
          success = pCodeList->getSchluesselNummer ( attributWert, schluesselNummer );
          if ( success ) STR = QuConvert::ToString ( schluesselNummer );
        }
      }
    }
    break;

	case _BOOLEAN:
		STR = dynamic_cast<String^> ( comboBoxVorgabewert->SelectedItem );
		 break;

  case _INTEGER:
    STR = textBoxVorgabewert->Text;
    if ( STR != nullptr )
    {
      try	{ d = Convert::ToInt32 ( STR ); }
      catch ( System::FormatException^ )
      {
        System::Windows::Forms::MessageBox::Show ( "Ungültiger Integer-Wert" );
        STR = nullptr;
        textBoxVorgabewert->Text = nullptr;
      }    
    }

    break;  

	case _DOUBLE:
  case _ANGLE:
  case _LENGTH:
  case _AREA:
  case _VOLUME:
  case _MEASURE:
		STR = textBoxVorgabewert->Text;
		if ( STR != nullptr )
		{
			if ( typ == _DOUBLE )
			{
				try	{ d = Convert::ToDouble ( STR ); }
				catch ( System::FormatException^ )
				{
					System::Windows::Forms::MessageBox::Show ( "Ungültiger Double-Wert" );
					STR = nullptr;
					textBoxVorgabewert->Text = nullptr;
				}
			}
		}	    
		break;

  default:
    STR = textBoxVorgabewert->Text;
    break;
	}

	if ( STR != nullptr )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		if ( pStandardAttributActive != NULL ) pStandardAttributActive->setDefaultWert  ( stdString );
		else
		{
			pStandardAttributActive = new StandardAttribut ( pGmlAttribut );
			pStandardAttributActive->setDefaultWert  ( stdString );
			pShapeFileObjekt->addStandardAttribut ( pStandardAttributActive );
		}

		displayStandardAttribute();
	}

}

private: System::Void buttonVorgabewertLoeschen_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	String		^ STR;
	string	      stdString;
	GmlAttribut * pGmlAttribut;
	bool		  success;

	STR =  dynamic_cast<String^> ( comboBoxGmlAttribute->SelectedItem );
	QuConvert::systemStr2stdStr ( stdString, STR );

	pGmlAttribut = pGmlKlasse->getGmlAttribut ( stdString );
	success = pShapeFileObjekt->removeStandardAttribut ( pGmlAttribut );
	if ( success ) displayStandardAttribute();
	pStandardAttributActive = NULL;
}

private: System::Void buttonBeenden_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	String ^ STR;
	int      bereichNummer;

	STR = dynamic_cast<String^> ( comboBoxBereiche->SelectedItem );
	if ( STR != nullptr )
	{
		bereichNummer = Convert::ToInt32 ( STR );
		pShapeFileObjekt->setBereichNummer ( bereichNummer );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Dialogbox im Konstruktor														   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void initialize ()
{
	String									 ^ STR;
	string										 stdString;
	string										 defaultWert;
	size_t									   i, anz;
	vector<ShapeFileAttribut*> shapeFileAttribute;
	vector<GmlAttribut*>			 gmlAttribute;
	GmlAttribut							 * pGmlAttribut;
  GML_SCHEMA_TYPE						 typ = m_pFeatures->getGmlTyp();

  if ( typ <  XPLANGML_4_0 )
  {
    label3->Visible               = false;
    labelRechtscharakter->Visible = false;
  }

	stdString = pShapeFileObjekt->getShapeFileName();
	STR = QuConvert::ToString ( stdString );
	STR = String::Concat ( "Konversion Shape-Datei ", STR );
	this->Text = STR;

	stdString = pGmlKlasse->getGmlElementName ();
	STR = QuConvert::ToString ( stdString );
	textBoxGmlKlasse->Text = STR;

	anz = pShapeFileObjekt->getShapeFileAttribute ( shapeFileAttribute );
	for ( i = 0; i < anz; i++ )
	{
		ShapeFileAttribut * pShapeFileAttribut = shapeFileAttribute[i];
		stdString = pShapeFileAttribut->getShapeFileAttributName();
		STR = QuConvert::ToString ( stdString );
		listBoxShapeAttribute->Items->Add ( STR );
	}

	if ( anz > 0 )
	{
		pAttributActive = shapeFileAttribute[0];
		listBoxShapeAttribute->SelectedIndex = 0;
	}

	anz = pGmlKlasse->getGmlAttribute ( gmlAttribute );
	for ( i = 0; i < anz; i++ )
	{
		pGmlAttribut = gmlAttribute[i];
		ATTRIBUT_TYP attributTyp = pGmlAttribut->getGmlAttributTyp();

		if ( attributTyp != _GEOMETRIE )
		{
			stdString = pGmlAttribut->getGmlAttributName ();
			STR = QuConvert::ToString ( stdString );
			comboBoxGmlAttribute->Items->Add ( STR );

      defaultWert = pGmlAttribut->getDefaultWert();
      if ( m_pFeatures->getGmlTyp() <  XPLANGML_4_0 && 
           ( m_pXPlanGML->getPlanTyp() == BPLAN ||  m_pXPlanGML->getPlanTyp() == FPLAN ) &&
        ( pGmlAttribut->getGmlAttributName() == "xplan:rechtscharakter" || 
          pGmlAttribut->getGmlAttributName() == "xplan:rechtlicheCharakterisierung" ) )
      {
        if ( defaultWert != "2000" )
          defaultWert = "2000";
      }

			if ( defaultWert  != "" )
			{
				StandardAttribut * pStandardAttribut = pShapeFileObjekt->getStandardAttribut( pGmlAttribut );
				if ( pStandardAttribut == NULL )
				{
					pStandardAttribut = new StandardAttribut ( pGmlAttribut );
					pStandardAttribut->setDefaultWert ( defaultWert );
					pShapeFileObjekt->addStandardAttribut ( pStandardAttribut );
				}
			}
		}
	}

	displayStandardAttribute();

	if ( plaeneAusShapeErzeugen && pAttributActive != NULL )
	{
		if ( pShapeFileObjekt->getAttributNamePlanId() == pAttributActive->getShapeFileAttributName() )
			checkBoxPlanId->Checked  = true;
		else
			checkBoxPlanId->Checked  = false;
	}
	else
			checkBoxPlanId->Enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
//  Aktualisiert die Dialogbox nach Auswahl eines Shapefile-Attributs über   //
//  die ListBox Shapefile-Attribute																					 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void displayAttributParameter ()
{
	String			       ^ STR;
	string				       stdString;
	vector<GmlAttribut*> gmlAttribute;
	size_t				    	 gmlAttributAnz, i, anz;
	ATTRIBUT_TYP         shapeAttributTyp, gmlAttributTyp;
	GmlAttribut		     * pGmlAttribut;

	shapeAttributTyp = pAttributActive->getShapeAttributDatentyp();

	switch ( shapeAttributTyp )
	{
	case _STRING:
		STR = "Text";
		break;	
	case  _DOUBLE:
		STR = "Double";
		break;
	case _INTEGER:
		STR = "Integer";
		break;
	case _DATE:
		STR = "Datum";
		break;
	}
	textBoxShapeAttributTyp->Text = STR;

	textBoxZuordnung1->Text = "";
	textBoxZuordnung2->Text = "";
	comboBoxZuordnung->Items->Clear();
	comboBoxZuordnung->Items->Add ( " " );


	anz = pGmlKlasse->getGmlAttribute ( gmlAttribute );
	for ( i = 0; i < anz; i++ )
	{
		pGmlAttribut = gmlAttribute[i];
		stdString    = pGmlAttribut->getGmlAttributName();
		STR          = QuConvert::ToString ( stdString );

		gmlAttributTyp  = pGmlAttribut->getGmlAttributTyp();
		if ( ( shapeAttributTyp == _STRING &&
			 ( gmlAttributTyp == _STRING || gmlAttributTyp == _DATE || gmlAttributTyp == _ENUMERATION || 
			  gmlAttributTyp == _REFERENZ ||  gmlAttributTyp == _DATA_TYPE ||
			   gmlAttributTyp == _BOOLEAN || gmlAttributTyp == _EXTERNAL_CODE_LIST ||
			   gmlAttributTyp == _POS_XY || gmlAttributTyp == _ANY_URI ) ) ||
			 ( shapeAttributTyp == _DOUBLE && 
			 ( gmlAttributTyp == _DOUBLE  ||  gmlAttributTyp == _REFERENZ  || gmlAttributTyp == _DATA_TYPE  ||
			   gmlAttributTyp == _ANGLE  || gmlAttributTyp == _LENGTH  || gmlAttributTyp == _AREA || gmlAttributTyp == _VOLUME ||
			   gmlAttributTyp == _STRING ) ) ||
			( shapeAttributTyp == _DATE && 
			 ( gmlAttributTyp == _DATE  ) ) ||
			 ( shapeAttributTyp == _INTEGER && 
			 ( gmlAttributTyp == _INTEGER  ||  gmlAttributTyp == _REFERENZ || 
			   gmlAttributTyp == _DATA_TYPE || gmlAttributTyp == _STRING || gmlAttributTyp == _ENUMERATION || gmlAttributTyp == _EXTERNAL_CODE_LIST ) ) )
		{
			comboBoxZuordnung->Items->Add ( STR );
		}
	}

	gmlAttribute.clear();
	gmlAttributAnz = pAttributActive->getGmlAttribute ( gmlAttribute );
	if ( gmlAttributAnz == 0 )
	{
		radioButtonZuordnung2->Enabled = false;
		textBoxZuordnung2->Enabled = false;

		comboBoxZuordnung->SelectedIndex = 0;
	}
	else
	{
		radioButtonZuordnung2->Enabled = true;
		textBoxZuordnung2->Enabled = true;
	}

	for ( i = 0; i < gmlAttributAnz; i++ )
	{
		pGmlAttribut = gmlAttribute[i];
		stdString    = pGmlAttribut->getGmlAttributName();
		STR          = QuConvert::ToString ( stdString );

		if ( i == 0 )
		{
			pGmlAttributActive = pGmlAttribut;
			textBoxZuordnung1->Text = STR;
			comboBoxZuordnung->SelectedItem = STR;
		}
		else  textBoxZuordnung2->Text = STR;

		stdString = pAttributActive->getGenerAttributName();
		if ( stdString != "" )
		{
			STR = QuConvert::ToString ( stdString );
			generAttributName->Text = STR;
		}
	  else
		  generAttributName->Text = nullptr;

		if ( pGmlAttribut->getGmlAttributTyp() == _REFERENZ || 
			 pGmlAttribut->getGmlAttributTyp() == _DATA_TYPE ) 
		{
			stdString = pAttributActive->getRefAttributName();
			if ( stdString != "" )
			{
				STR = QuConvert::ToString ( stdString );
				comboBoxAttributName->SelectedItem = STR;
			}

			int index = pAttributActive->getRefAttributIndex();
			textBoxIndex->Text = Convert::ToString ( index );
		}
		else
			comboBoxAttributName->SelectedItem = nullptr;
	}

	radioButtonZuordnung1->Checked  = true;
	radioButtonZuordnung2->Checked  = false;
	indexZuordnung = 0;

}

///////////////////////////////////////////////////////////////////////////////
//  Listet alle mit festen Werten belegte XPlanGML-Attribute					       //
///////////////////////////////////////////////////////////////////////////////
private: System::Void displayStandardAttribute()
{
	int				   i;
	string			   standardAttributName;
	string			   standardAttributWert;
	String			 ^ STR;
	StandardAttribut * pStandardAttribut;

	listBoxVorgabewerte->Items->Clear();
	for ( i = 0; i < pShapeFileObjekt->getStandardAttributAnz(); i++ )
	{
		pStandardAttribut = pShapeFileObjekt->getStandardAttribut ( i );
		standardAttributName = pStandardAttribut->getGmlAttribut()->getGmlAttributName();
		standardAttributWert = pStandardAttribut->getDefaultWert();

		STR = String::Concat ( QuConvert::ToString ( standardAttributName ), 
							   " - ",
							   QuConvert::ToString ( standardAttributWert ) );
		listBoxVorgabewerte->Items->Add ( STR );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler CheckBox "Ist Plan-Id"																	   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void checkBoxPlanId_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	if ( pAttributActive == NULL ) return;

	if ( checkBoxPlanId->Checked == true )
		pShapeFileObjekt->setNameAttributPlanId ( pAttributActive->getShapeFileAttributName() );
}

};
