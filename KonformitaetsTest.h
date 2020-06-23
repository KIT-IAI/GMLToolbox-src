#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Convert.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "GmlSchema.h"
#include "GmlModelTest.h"
#include "KonformitaetsFehler.h"
#include "ModellStatistik.h"
#include "ValidierungsfehlerDialog.h"
#include "GeometryErrorUebersicht.h"
#include "GeometrietestXPlanGML.h"
#include "SemantiktestXPlanGML.h"
#include "Checking.h"
#include "CheckingRef.h"
#include "GeometryTestParams.h"
#include "GeometryKorrektur.h"
#include "GeometryErrorsPerFeature.h"
#include "GeometrietestCityGML.h"

using namespace std;

	/// <summary> 
	/// Zentraler Dialog zur Durchführung der Modellprüfung
	///
	/// </summary>
	public ref class KonformitaetsTest : public System::Windows::Forms::Form
	{
	public: 
		KonformitaetsTest( Features * pFeatures, Checking * pChecking, String ^ cityGMLViewer, String ^ gmlDateiName )
		{
      std::string stdString;

      InitializeComponent();

      m_pFeatures = pFeatures;
      m_pChecking = pChecking;
      m_pChecking->initialize();
      m_CityGMLViewer = cityGMLViewer;

      QuConvert::systemStr2stdStr( stdString, gmlDateiName );

      m_gmlDateiName      = gmlDateiName;

      String ^ fileName = System::IO::Path::GetFileName( gmlDateiName );
      this->Text = String::Concat ( "Überprüfung ", fileName );

      validated               = false;
      conformityErrorsChecked = false;
      geometryErrorsChecked   = false;

      fullStatistics = true;
      radioButtonFullStatistics->Checked = true;

      radioButtonFullStatistics->Visible = false;
      radioButtonShortStatistics->Visible = false;

      if ( !m_pFeatures->getGmlSchema()->isCityGml() )
        checkBoxSchematron->Visible = false;

      checkBoxProtokoll->Checked = true;

      if ( !m_pFeatures->getGmlSchema()->isXPlanGml() )
      {
        checkBoxSpezSyntaxCheck->Enabled = false;
        buttonSyntaxChecksSpezifisch->Enabled = false;
      }

		  initialize();
		}
  private: System::Windows::Forms::GroupBox^  groupBoxSyntaxChecks;
	private: System::Windows::Forms::Button ^  buttonAbbrechen;
	private: System::Windows::Forms::Button ^  buttonTest;
	private: System::Windows::Forms::CheckBox ^  checkBoxRegeln;
  private: System::Windows::Forms::Label^  labelTest;
  private: System::Windows::Forms::ProgressBar^  progressBarTest;
  private: System::Windows::Forms::TextBox^  textBoxFehlerAnzahl;
  private: System::Windows::Forms::GroupBox^  groupBoxModellStatistik;
  private: System::Windows::Forms::GroupBox^  groupBoxSchemaValidierung;
  private: System::Windows::Forms::CheckBox^  checkBoxKoordinatenReferenzsystem;
  private: System::Windows::Forms::CheckBox^  checkBoxCodelists;
  private: System::Windows::Forms::CheckBox^  checkBoxSpezSyntaxCheck;
  private: System::Windows::Forms::GroupBox^  groupBoxGeometrieChecks;
  private: System::Windows::Forms::Button^  buttonSyntaxChecks;
  private: System::Windows::Forms::Button^  buttonSyntaxChecksSpezifisch;
  private: System::Windows::Forms::Button^  buttonModellStatistik;
  private: System::Windows::Forms::Button^  buttonSchemaValidierung;
  private: System::Windows::Forms::Button^  buttonGeometrieChecks;
  private: System::Windows::Forms::Button^  buttonGeometrieChecksSpezifisch;
  private: System::Windows::Forms::CheckBox^  checkBoxProtokoll;
  private: System::Windows::Forms::Button^  buttonReadProfil;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::ComboBox^  comboBoxProfil;
  private: System::Windows::Forms::ComboBox^  comboBoxGeometryCheckLevel;
  private: System::Windows::Forms::Button^  buttonCheckParams;
  private: System::Windows::Forms::Button^  buttonKorrektur;
  private: System::Windows::Forms::CheckBox^  checkBoxSchematron;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::ComboBox^  comboBoxAnzeigeGeometriefehler;
  private: System::Windows::Forms::CheckBox^  checkBoxReferenzen;
  private: System::Windows::Forms::OpenFileDialog ^  KonformRegelnDialog;	 
  private: System::Windows::Forms::RadioButton^  radioButtonFullStatistics;
  private: System::Windows::Forms::RadioButton^  radioButtonShortStatistics;

	public:
    Features   * m_pFeatures;     // Aggregation aller Features
    BLeitPlan  * m_pPlanAktuell;  // Nur XPlanGML: Aktueller Plan
    Checking   * m_pChecking;     // Prüfungs-Modul
    String     ^ m_CityGMLViewer; // Generischer GML-Viewer

    String ^ m_gmlDateiName;      // Pfadname der eingelesenen GML-Datei

    bool checkXPlanGMLBackwardPointer; // Nur XPlanGML: Prüfung der Rückwärtspointer Planobjekt --> Pklanbereich
    bool checkXPlanGMLUOM;             // Nur XPlanGML: Prüfung der uom-Identifier
    bool validated;                    // Zeigt an, ob die Schemavalidierung durchgeführt wurde
    bool conformityErrorsChecked;      // Zeigt an, ob die Konformitätsregeln überprüft wurden
    bool geometryErrorsChecked;        // Zeigt an, ob die Geometrieprüfung durchgeführt wurde
    bool fullStatistics;               // Zeigt an, on eine ausführliche Modellstatistik erzeugt werden soll

    int  profilAnz;                    // Anzahl der Schema-Profile
    int geometryCheckLevel;            // Durchzuführende Geometrieprüfung (enum GEOMETRY_CHECK)

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
      this->groupBoxSyntaxChecks = (gcnew System::Windows::Forms::GroupBox());
      this->checkBoxReferenzen = (gcnew System::Windows::Forms::CheckBox());
      this->buttonReadProfil = (gcnew System::Windows::Forms::Button());
      this->buttonSyntaxChecks = (gcnew System::Windows::Forms::Button());
      this->buttonSyntaxChecksSpezifisch = (gcnew System::Windows::Forms::Button());
      this->checkBoxSpezSyntaxCheck = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxKoordinatenReferenzsystem = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxCodelists = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxRegeln = (gcnew System::Windows::Forms::CheckBox());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->buttonTest = (gcnew System::Windows::Forms::Button());
      this->KonformRegelnDialog = (gcnew System::Windows::Forms::OpenFileDialog());
      this->labelTest = (gcnew System::Windows::Forms::Label());
      this->progressBarTest = (gcnew System::Windows::Forms::ProgressBar());
      this->textBoxFehlerAnzahl = (gcnew System::Windows::Forms::TextBox());
      this->groupBoxModellStatistik = (gcnew System::Windows::Forms::GroupBox());
      this->radioButtonFullStatistics = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonShortStatistics = (gcnew System::Windows::Forms::RadioButton());
      this->buttonModellStatistik = (gcnew System::Windows::Forms::Button());
      this->groupBoxSchemaValidierung = (gcnew System::Windows::Forms::GroupBox());
      this->checkBoxSchematron = (gcnew System::Windows::Forms::CheckBox());
      this->buttonSchemaValidierung = (gcnew System::Windows::Forms::Button());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->comboBoxProfil = (gcnew System::Windows::Forms::ComboBox());
      this->groupBoxGeometrieChecks = (gcnew System::Windows::Forms::GroupBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->comboBoxAnzeigeGeometriefehler = (gcnew System::Windows::Forms::ComboBox());
      this->buttonKorrektur = (gcnew System::Windows::Forms::Button());
      this->buttonCheckParams = (gcnew System::Windows::Forms::Button());
      this->comboBoxGeometryCheckLevel = (gcnew System::Windows::Forms::ComboBox());
      this->buttonGeometrieChecks = (gcnew System::Windows::Forms::Button());
      this->buttonGeometrieChecksSpezifisch = (gcnew System::Windows::Forms::Button());
      this->checkBoxProtokoll = (gcnew System::Windows::Forms::CheckBox());
      this->groupBoxSyntaxChecks->SuspendLayout();
      this->groupBoxModellStatistik->SuspendLayout();
      this->groupBoxSchemaValidierung->SuspendLayout();
      this->groupBoxGeometrieChecks->SuspendLayout();
      this->SuspendLayout();
      // 
      // groupBoxSyntaxChecks
      // 
      this->groupBoxSyntaxChecks->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBoxSyntaxChecks->Controls->Add(this->checkBoxReferenzen);
      this->groupBoxSyntaxChecks->Controls->Add(this->buttonReadProfil);
      this->groupBoxSyntaxChecks->Controls->Add(this->buttonSyntaxChecks);
      this->groupBoxSyntaxChecks->Controls->Add(this->buttonSyntaxChecksSpezifisch);
      this->groupBoxSyntaxChecks->Controls->Add(this->checkBoxSpezSyntaxCheck);
      this->groupBoxSyntaxChecks->Controls->Add(this->checkBoxKoordinatenReferenzsystem);
      this->groupBoxSyntaxChecks->Controls->Add(this->checkBoxCodelists);
      this->groupBoxSyntaxChecks->Controls->Add(this->checkBoxRegeln);
      this->groupBoxSyntaxChecks->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->groupBoxSyntaxChecks->Location = System::Drawing::Point(8, 190);
      this->groupBoxSyntaxChecks->Name = L"groupBoxSyntaxChecks";
      this->groupBoxSyntaxChecks->Size = System::Drawing::Size(499, 146);
      this->groupBoxSyntaxChecks->TabIndex = 3;
      this->groupBoxSyntaxChecks->TabStop = false;
      this->groupBoxSyntaxChecks->Text = L"Konformitäts-Checks";
      // 
      // checkBoxReferenzen
      // 
      this->checkBoxReferenzen->AutoSize = true;
      this->checkBoxReferenzen->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->checkBoxReferenzen->Location = System::Drawing::Point(339, 28);
      this->checkBoxReferenzen->Name = L"checkBoxReferenzen";
      this->checkBoxReferenzen->Size = System::Drawing::Size(81, 17);
      this->checkBoxReferenzen->TabIndex = 23;
      this->checkBoxReferenzen->Text = L"Referenzen";
      this->checkBoxReferenzen->UseVisualStyleBackColor = true;
      // 
      // buttonReadProfil
      // 
      this->buttonReadProfil->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->buttonReadProfil->Location = System::Drawing::Point(157, 117);
      this->buttonReadProfil->Name = L"buttonReadProfil";
      this->buttonReadProfil->Size = System::Drawing::Size(158, 23);
      this->buttonReadProfil->TabIndex = 22;
      this->buttonReadProfil->Text = L"Weitere Konformitätsregeln";
      this->buttonReadProfil->UseVisualStyleBackColor = true;
      this->buttonReadProfil->Click += gcnew System::EventHandler(this, &KonformitaetsTest::buttonReadProfil_Click);
      // 
      // buttonSyntaxChecks
      // 
      this->buttonSyntaxChecks->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonSyntaxChecks->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->buttonSyntaxChecks->Location = System::Drawing::Point(382, 53);
      this->buttonSyntaxChecks->Name = L"buttonSyntaxChecks";
      this->buttonSyntaxChecks->Size = System::Drawing::Size(93, 23);
      this->buttonSyntaxChecks->TabIndex = 8;
      this->buttonSyntaxChecks->Text = L"Ausführen";
      this->buttonSyntaxChecks->UseVisualStyleBackColor = true;
      this->buttonSyntaxChecks->Click += gcnew System::EventHandler(this, &KonformitaetsTest::buttonSyntaxChecks_Click);
      // 
      // buttonSyntaxChecksSpezifisch
      // 
      this->buttonSyntaxChecksSpezifisch->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->buttonSyntaxChecksSpezifisch->Location = System::Drawing::Point(172, 81);
      this->buttonSyntaxChecksSpezifisch->Name = L"buttonSyntaxChecksSpezifisch";
      this->buttonSyntaxChecksSpezifisch->Size = System::Drawing::Size(75, 23);
      this->buttonSyntaxChecksSpezifisch->TabIndex = 7;
      this->buttonSyntaxChecksSpezifisch->Text = L"Details";
      this->buttonSyntaxChecksSpezifisch->UseVisualStyleBackColor = true;
      this->buttonSyntaxChecksSpezifisch->Click += gcnew System::EventHandler(this, &KonformitaetsTest::buttonSyntaxChecksSpezifisch_Click);
      // 
      // checkBoxSpezSyntaxCheck
      // 
      this->checkBoxSpezSyntaxCheck->AutoSize = true;
      this->checkBoxSpezSyntaxCheck->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->checkBoxSpezSyntaxCheck->Location = System::Drawing::Point(6, 85);
      this->checkBoxSpezSyntaxCheck->Name = L"checkBoxSpezSyntaxCheck";
      this->checkBoxSpezSyntaxCheck->Size = System::Drawing::Size(156, 17);
      this->checkBoxSpezSyntaxCheck->TabIndex = 6;
      this->checkBoxSpezSyntaxCheck->Text = L"Schemaspezifische Checks";
      this->checkBoxSpezSyntaxCheck->UseVisualStyleBackColor = true;
      // 
      // checkBoxKoordinatenReferenzsystem
      // 
      this->checkBoxKoordinatenReferenzsystem->AutoSize = true;
      this->checkBoxKoordinatenReferenzsystem->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->checkBoxKoordinatenReferenzsystem->Location = System::Drawing::Point(140, 30);
      this->checkBoxKoordinatenReferenzsystem->Name = L"checkBoxKoordinatenReferenzsystem";
      this->checkBoxKoordinatenReferenzsystem->Size = System::Drawing::Size(161, 17);
      this->checkBoxKoordinatenReferenzsystem->TabIndex = 4;
      this->checkBoxKoordinatenReferenzsystem->Text = L"Koordinaten-Referenzsystem";
      this->checkBoxKoordinatenReferenzsystem->UseVisualStyleBackColor = true;
      // 
      // checkBoxCodelists
      // 
      this->checkBoxCodelists->AutoSize = true;
      this->checkBoxCodelists->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->checkBoxCodelists->Location = System::Drawing::Point(7, 53);
      this->checkBoxCodelists->Name = L"checkBoxCodelists";
      this->checkBoxCodelists->Size = System::Drawing::Size(114, 17);
      this->checkBoxCodelists->TabIndex = 3;
      this->checkBoxCodelists->Text = L"Externe Codelisten";
      this->checkBoxCodelists->UseVisualStyleBackColor = true;
      // 
      // checkBoxRegeln
      // 
      this->checkBoxRegeln->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->checkBoxRegeln->Location = System::Drawing::Point(6, 22);
      this->checkBoxRegeln->Name = L"checkBoxRegeln";
      this->checkBoxRegeln->Size = System::Drawing::Size(128, 24);
      this->checkBoxRegeln->TabIndex = 2;
      this->checkBoxRegeln->Text = L"Regel-Konformität";
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonAbbrechen->Location = System::Drawing::Point(436, 580);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 39);
      this->buttonAbbrechen->TabIndex = 8;
      this->buttonAbbrechen->Text = L"Beenden";
      this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &KonformitaetsTest::buttonAbbrechen_Click);
      // 
      // buttonTest
      // 
      this->buttonTest->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonTest->Location = System::Drawing::Point(17, 580);
      this->buttonTest->Name = L"buttonTest";
      this->buttonTest->Size = System::Drawing::Size(75, 39);
      this->buttonTest->TabIndex = 9;
      this->buttonTest->Text = L"Gesamter Modelltest";
      this->buttonTest->Click += gcnew System::EventHandler(this, &KonformitaetsTest::buttonTest_Click);
      // 
      // KonformRegelnDialog
      // 
      this->KonformRegelnDialog->Filter = L"(*.xml)|*.xml";
      this->KonformRegelnDialog->Multiselect = true;
      this->KonformRegelnDialog->Title = L"Einlesen Profildateien";
      // 
      // labelTest
      // 
      this->labelTest->AutoSize = true;
      this->labelTest->Location = System::Drawing::Point(8, 217);
      this->labelTest->Name = L"labelTest";
      this->labelTest->Size = System::Drawing::Size(0, 13);
      this->labelTest->TabIndex = 13;
      // 
      // progressBarTest
      // 
      this->progressBarTest->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->progressBarTest->Location = System::Drawing::Point(6, 484);
      this->progressBarTest->Name = L"progressBarTest";
      this->progressBarTest->Size = System::Drawing::Size(489, 23);
      this->progressBarTest->TabIndex = 14;
      // 
      // textBoxFehlerAnzahl
      // 
      this->textBoxFehlerAnzahl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
        | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxFehlerAnzahl->Location = System::Drawing::Point(4, 517);
      this->textBoxFehlerAnzahl->Multiline = true;
      this->textBoxFehlerAnzahl->Name = L"textBoxFehlerAnzahl";
      this->textBoxFehlerAnzahl->ReadOnly = true;
      this->textBoxFehlerAnzahl->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
      this->textBoxFehlerAnzahl->Size = System::Drawing::Size(503, 45);
      this->textBoxFehlerAnzahl->TabIndex = 15;
      // 
      // groupBoxModellStatistik
      // 
      this->groupBoxModellStatistik->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBoxModellStatistik->Controls->Add(this->radioButtonFullStatistics);
      this->groupBoxModellStatistik->Controls->Add(this->radioButtonShortStatistics);
      this->groupBoxModellStatistik->Controls->Add(this->buttonModellStatistik);
      this->groupBoxModellStatistik->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->groupBoxModellStatistik->Location = System::Drawing::Point(8, 13);
      this->groupBoxModellStatistik->Name = L"groupBoxModellStatistik";
      this->groupBoxModellStatistik->Size = System::Drawing::Size(499, 54);
      this->groupBoxModellStatistik->TabIndex = 18;
      this->groupBoxModellStatistik->TabStop = false;
      this->groupBoxModellStatistik->Text = L"Modell-Statistik";
      // 
      // radioButtonFullStatistics
      // 
      this->radioButtonFullStatistics->AutoSize = true;
      this->radioButtonFullStatistics->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->radioButtonFullStatistics->Location = System::Drawing::Point(113, 25);
      this->radioButtonFullStatistics->Name = L"radioButtonFullStatistics";
      this->radioButtonFullStatistics->Size = System::Drawing::Size(76, 17);
      this->radioButtonFullStatistics->TabIndex = 17;
      this->radioButtonFullStatistics->TabStop = true;
      this->radioButtonFullStatistics->Text = L"Vollständig";
      this->radioButtonFullStatistics->UseVisualStyleBackColor = true;
      // 
      // radioButtonShortStatistics
      // 
      this->radioButtonShortStatistics->AutoSize = true;
      this->radioButtonShortStatistics->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->radioButtonShortStatistics->Location = System::Drawing::Point(13, 24);
      this->radioButtonShortStatistics->Name = L"radioButtonShortStatistics";
      this->radioButtonShortStatistics->Size = System::Drawing::Size(70, 17);
      this->radioButtonShortStatistics->TabIndex = 16;
      this->radioButtonShortStatistics->TabStop = true;
      this->radioButtonShortStatistics->Text = L"Übersicht";
      this->radioButtonShortStatistics->UseVisualStyleBackColor = true;
      // 
      // buttonModellStatistik
      // 
      this->buttonModellStatistik->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonModellStatistik->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->buttonModellStatistik->Location = System::Drawing::Point(382, 19);
      this->buttonModellStatistik->Name = L"buttonModellStatistik";
      this->buttonModellStatistik->Size = System::Drawing::Size(93, 23);
      this->buttonModellStatistik->TabIndex = 15;
      this->buttonModellStatistik->Text = L"Generieren";
      this->buttonModellStatistik->UseVisualStyleBackColor = true;
      this->buttonModellStatistik->Click += gcnew System::EventHandler(this, &KonformitaetsTest::buttonModellStatistik_Click);
      // 
      // groupBoxSchemaValidierung
      // 
      this->groupBoxSchemaValidierung->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBoxSchemaValidierung->Controls->Add(this->checkBoxSchematron);
      this->groupBoxSchemaValidierung->Controls->Add(this->buttonSchemaValidierung);
      this->groupBoxSchemaValidierung->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->groupBoxSchemaValidierung->Location = System::Drawing::Point(6, 117);
      this->groupBoxSchemaValidierung->Name = L"groupBoxSchemaValidierung";
      this->groupBoxSchemaValidierung->Size = System::Drawing::Size(501, 67);
      this->groupBoxSchemaValidierung->TabIndex = 19;
      this->groupBoxSchemaValidierung->TabStop = false;
      this->groupBoxSchemaValidierung->Text = L"Validierung";
      // 
      // checkBoxSchematron
      // 
      this->checkBoxSchematron->AutoSize = true;
      this->checkBoxSchematron->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->checkBoxSchematron->Location = System::Drawing::Point(10, 25);
      this->checkBoxSchematron->Name = L"checkBoxSchematron";
      this->checkBoxSchematron->Size = System::Drawing::Size(120, 17);
      this->checkBoxSchematron->TabIndex = 2;
      this->checkBoxSchematron->Text = L"Schematron-Regeln";
      this->checkBoxSchematron->UseVisualStyleBackColor = true;
      // 
      // buttonSchemaValidierung
      // 
      this->buttonSchemaValidierung->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonSchemaValidierung->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->buttonSchemaValidierung->Location = System::Drawing::Point(382, 19);
      this->buttonSchemaValidierung->Name = L"buttonSchemaValidierung";
      this->buttonSchemaValidierung->Size = System::Drawing::Size(93, 23);
      this->buttonSchemaValidierung->TabIndex = 1;
      this->buttonSchemaValidierung->Text = L"Ausführen";
      this->buttonSchemaValidierung->UseVisualStyleBackColor = true;
      this->buttonSchemaValidierung->Click += gcnew System::EventHandler(this, &KonformitaetsTest::buttonSchemaValidierung_Click);
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->label2->Location = System::Drawing::Point(13, 90);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(134, 13);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Berücksichtigtes Profil";
      // 
      // comboBoxProfil
      // 
      this->comboBoxProfil->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->comboBoxProfil->FormattingEnabled = true;
      this->comboBoxProfil->Location = System::Drawing::Point(180, 90);
      this->comboBoxProfil->Name = L"comboBoxProfil";
      this->comboBoxProfil->Size = System::Drawing::Size(313, 21);
      this->comboBoxProfil->TabIndex = 2;
      // 
      // groupBoxGeometrieChecks
      // 
      this->groupBoxGeometrieChecks->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBoxGeometrieChecks->Controls->Add(this->label1);
      this->groupBoxGeometrieChecks->Controls->Add(this->comboBoxAnzeigeGeometriefehler);
      this->groupBoxGeometrieChecks->Controls->Add(this->buttonKorrektur);
      this->groupBoxGeometrieChecks->Controls->Add(this->buttonCheckParams);
      this->groupBoxGeometrieChecks->Controls->Add(this->comboBoxGeometryCheckLevel);
      this->groupBoxGeometrieChecks->Controls->Add(this->buttonGeometrieChecks);
      this->groupBoxGeometrieChecks->Controls->Add(this->buttonGeometrieChecksSpezifisch);
      this->groupBoxGeometrieChecks->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->groupBoxGeometrieChecks->Location = System::Drawing::Point(10, 342);
      this->groupBoxGeometrieChecks->Name = L"groupBoxGeometrieChecks";
      this->groupBoxGeometrieChecks->Size = System::Drawing::Size(499, 136);
      this->groupBoxGeometrieChecks->TabIndex = 20;
      this->groupBoxGeometrieChecks->TabStop = false;
      this->groupBoxGeometrieChecks->Text = L"Geometrie-Checks";
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->label1->Location = System::Drawing::Point(170, 115);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(122, 13);
      this->label1->TabIndex = 21;
      this->label1->Text = L"Geometriefehler-Anzeige";
      // 
      // comboBoxAnzeigeGeometriefehler
      // 
      this->comboBoxAnzeigeGeometriefehler->FormattingEnabled = true;
      this->comboBoxAnzeigeGeometriefehler->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Nach Fehlertypen", L"Nach Base-Features" });
      this->comboBoxAnzeigeGeometriefehler->Location = System::Drawing::Point(194, 92);
      this->comboBoxAnzeigeGeometriefehler->Name = L"comboBoxAnzeigeGeometriefehler";
      this->comboBoxAnzeigeGeometriefehler->Size = System::Drawing::Size(146, 21);
      this->comboBoxAnzeigeGeometriefehler->TabIndex = 20;
      // 
      // buttonKorrektur
      // 
      this->buttonKorrektur->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonKorrektur->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->buttonKorrektur->Location = System::Drawing::Point(393, 107);
      this->buttonKorrektur->Name = L"buttonKorrektur";
      this->buttonKorrektur->Size = System::Drawing::Size(75, 23);
      this->buttonKorrektur->TabIndex = 19;
      this->buttonKorrektur->Text = L"Korrektur";
      this->buttonKorrektur->UseVisualStyleBackColor = true;
      this->buttonKorrektur->Click += gcnew System::EventHandler(this, &KonformitaetsTest::buttonKorrektur_Click);
      // 
      // buttonCheckParams
      // 
      this->buttonCheckParams->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->buttonCheckParams->Location = System::Drawing::Point(7, 107);
      this->buttonCheckParams->Name = L"buttonCheckParams";
      this->buttonCheckParams->Size = System::Drawing::Size(75, 23);
      this->buttonCheckParams->TabIndex = 18;
      this->buttonCheckParams->Text = L"Parameter";
      this->buttonCheckParams->UseVisualStyleBackColor = true;
      this->buttonCheckParams->Click += gcnew System::EventHandler(this, &KonformitaetsTest::buttonCheckParams_Click);
      // 
      // comboBoxGeometryCheckLevel
      // 
      this->comboBoxGeometryCheckLevel->FormattingEnabled = true;
      this->comboBoxGeometryCheckLevel->Items->AddRange(gcnew cli::array< System::Object^  >(6) {
        L"Keine", L"Basis-Tests", L"Fortgeschrittene Tests, Planarität",
          L"Topologie-Tests", L"Schemaspezifische Tests", L"Alle"
      });
      this->comboBoxGeometryCheckLevel->Location = System::Drawing::Point(2, 31);
      this->comboBoxGeometryCheckLevel->Name = L"comboBoxGeometryCheckLevel";
      this->comboBoxGeometryCheckLevel->Size = System::Drawing::Size(158, 21);
      this->comboBoxGeometryCheckLevel->TabIndex = 16;
      this->comboBoxGeometryCheckLevel->SelectedIndexChanged += gcnew System::EventHandler(this, &KonformitaetsTest::comboBoxGeometryCheckLevel_SelectedIndexChanged);
      // 
      // buttonGeometrieChecks
      // 
      this->buttonGeometrieChecks->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonGeometrieChecks->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->buttonGeometrieChecks->Location = System::Drawing::Point(382, 43);
      this->buttonGeometrieChecks->Name = L"buttonGeometrieChecks";
      this->buttonGeometrieChecks->Size = System::Drawing::Size(93, 23);
      this->buttonGeometrieChecks->TabIndex = 15;
      this->buttonGeometrieChecks->Text = L"Ausführen";
      this->buttonGeometrieChecks->UseVisualStyleBackColor = true;
      this->buttonGeometrieChecks->Click += gcnew System::EventHandler(this, &KonformitaetsTest::buttonGeometrieChecks_Click);
      // 
      // buttonGeometrieChecksSpezifisch
      // 
      this->buttonGeometrieChecksSpezifisch->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->buttonGeometrieChecksSpezifisch->Location = System::Drawing::Point(4, 63);
      this->buttonGeometrieChecksSpezifisch->Name = L"buttonGeometrieChecksSpezifisch";
      this->buttonGeometrieChecksSpezifisch->Size = System::Drawing::Size(156, 23);
      this->buttonGeometrieChecksSpezifisch->TabIndex = 14;
      this->buttonGeometrieChecksSpezifisch->Text = L"Schemaspezifische Tests";
      this->buttonGeometrieChecksSpezifisch->UseVisualStyleBackColor = true;
      this->buttonGeometrieChecksSpezifisch->Click += gcnew System::EventHandler(this, &KonformitaetsTest::buttonGeometrieChecksSpezifisch_Click);
      // 
      // checkBoxProtokoll
      // 
      this->checkBoxProtokoll->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->checkBoxProtokoll->AutoSize = true;
      this->checkBoxProtokoll->Location = System::Drawing::Point(116, 592);
      this->checkBoxProtokoll->Name = L"checkBoxProtokoll";
      this->checkBoxProtokoll->Size = System::Drawing::Size(139, 17);
      this->checkBoxProtokoll->TabIndex = 21;
      this->checkBoxProtokoll->Text = L"XML-Protokoll erzeugen";
      this->checkBoxProtokoll->UseVisualStyleBackColor = true;
      // 
      // KonformitaetsTest
      // 
      this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
      this->ClientSize = System::Drawing::Size(531, 643);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->checkBoxProtokoll);
      this->Controls->Add(this->comboBoxProfil);
      this->Controls->Add(this->groupBoxGeometrieChecks);
      this->Controls->Add(this->groupBoxSchemaValidierung);
      this->Controls->Add(this->groupBoxModellStatistik);
      this->Controls->Add(this->textBoxFehlerAnzahl);
      this->Controls->Add(this->progressBarTest);
      this->Controls->Add(this->labelTest);
      this->Controls->Add(this->buttonTest);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->groupBoxSyntaxChecks);
      this->Name = L"KonformitaetsTest";
      this->Text = L"Prüfung von GML-Daten";
      this->groupBoxSyntaxChecks->ResumeLayout(false);
      this->groupBoxSyntaxChecks->PerformLayout();
      this->groupBoxModellStatistik->ResumeLayout(false);
      this->groupBoxModellStatistik->PerformLayout();
      this->groupBoxSchemaValidierung->ResumeLayout(false);
      this->groupBoxSchemaValidierung->PerformLayout();
      this->groupBoxGeometrieChecks->ResumeLayout(false);
      this->groupBoxGeometrieChecks->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"                                          //
//  Übernimmt die Prüfungs-Einstellungen und schließt den Dialog             //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAbbrechen_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  m_pChecking->fullStatistics             = fullStatistics;
  m_pChecking->checkSemanticRules         = checkBoxRegeln->Checked;
  m_pChecking->checkCodeLists             = checkBoxCodelists->Checked;
  m_pChecking->checkReferenzen            = checkBoxReferenzen->Checked;
  m_pChecking->checkCoordReferenceSystems = checkBoxKoordinatenReferenzsystem->Checked;

  switch ( m_pFeatures->getGmlTyp() )
  {
  case XPLANGML_2_0:
  case XPLANGML_3_0:
  case XPLANGML_4_0:
  case XPLANGML_4_1:
  case XPLANGML_5_0:
  case XPLANGML_5_1:
  case XPLANGML_5_2:
  case XPLANGML_5_3:
    m_pChecking->checkXPlanGMLRules = checkBoxSpezSyntaxCheck->Checked;
    m_pChecking->checkXPlanGMLBackwardPointer = checkXPlanGMLBackwardPointer;
    m_pChecking->checkXPlanGMLUOM = checkXPlanGMLUOM;
    break;

  case INSPIRE_LU_PLANNED:
  case INSPIRE:
     break;

  case NAS_5_1:
  case NAS_5_1_1:
  case NAS_6_0:
    break;

  case CITYGML_0_4_0:
  case CITYGML_1_0:
  case CITYGML_2_0:
  case CITYGML_3_0:
    break;

  case BOREHOLEML_3_0:
    break;

  case GML_SCHEMA_DEFAULT:
    break;

  default:
    break;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Gesamter Modelltest"                              //
//  Führt alle spez. Modelltest durch und speichert das Ergebnis ggf.        //
//  in einer XML-Datei                                                       //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonTest_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  String ^ PROFILE_NAME = nullptr;
  String ^ STR = "";

  if ( profilAnz > 0 )
    PROFILE_NAME = dynamic_cast<String^>(comboBoxProfil->SelectedItem);

  checkValidation();  
     
  int anzValidationError = (int)m_pChecking->getErrorReport()->getValidationErrorAnz();
  int anzReadError       = (int)m_pChecking->getErrorReport()->getReadErrorAnz();

  if (  anzReadError > 0 )
    STR = String::Concat ( STR, Convert::ToString( anzReadError ), " Fehler beim Einlesen der GML-Datei festgestellt\r\n" );

  if (  anzValidationError > 0 )
    STR = String::Concat ( STR, Convert::ToString( anzValidationError ), " Validierungsfehler festgestellt\r\n" );
  else
    STR = String::Concat ( STR,"Keine Validierungsfehler festgestellt\r\n" );
  

  checkConformity();

  int  anzSemanticError =  (int)m_pChecking->getErrorReport()->getSemanticErrorAnz();
  if (  anzSemanticError > 0 )
    STR = String::Concat ( STR, Convert::ToString( anzSemanticError ), " Semantikfehler festgestellt\r\n" );
  else
    STR = String::Concat ( STR,"Keine Semantikfehler festgestellt\r\n" );

  checkGeometry();

  int  anzGeometryError =  (int)m_pChecking->getErrorReport()->getGeometryErrorAnz();
  if (  anzGeometryError > 0 )
  {
    STR = String::Concat ( STR, Convert::ToString( anzGeometryError ), " Geometriefehler festgestellt\r\n" );
    buttonKorrektur->Enabled = true;
  }
  else
    STR = String::Concat ( STR, "Keine Geometriefehler festgestellt\r\n" );

  if ( checkBoxProtokoll->Checked )
  {
    String ^ directoryName = System::IO::Path::GetDirectoryName( m_gmlDateiName );
    String ^ fileName      = System::IO::Path::GetFileNameWithoutExtension( m_gmlDateiName );
    string   name;

    fileName =  String::Concat( directoryName, "\\", fileName, ".rep" );
    QuConvert::systemStr2stdStr( name, fileName );

    ErrorReportWriter ^ pWriter = gcnew ErrorReportWriter ( m_pChecking->getErrorReport() );
    pWriter->write( name );

    STR = String::Concat ( STR, "Fehlerprotokoll gespeichert unter\r\n", fileName );
  }

  textBoxFehlerAnzahl->Text = STR;
}

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Dialogbox                                              //
///////////////////////////////////////////////////////////////////////////////
private: System::Void initialize()
{
	 string         stdString;
	 string         ausgabeName, ausgabeFolder;
   vector<string> vProfiles;

   comboBoxAnzeigeGeometriefehler->SelectedIndex = 1;

   profilAnz = m_pChecking->getProfiles ( vProfiles );
   if ( profilAnz > 0  )
   {
     comboBoxProfil->Items->Add ( " " );
     for ( int i = 0; i < profilAnz; i++ )
       comboBoxProfil->Items->Add( QuConvert::ToString( vProfiles[i] ) );
   }
   else
     comboBoxProfil->Enabled = false;

   if ( m_pChecking->fullStatistics )
   {
    radioButtonFullStatistics->Checked = true;
    radioButtonShortStatistics->Checked = false;
   }
   else
   {
     radioButtonShortStatistics->Checked = true;
     radioButtonFullStatistics->Checked = false;
   }

   if (  m_pChecking->checkSemanticRules )
     checkBoxRegeln->Checked = true;
   else
     checkBoxRegeln->Checked = false;
   
   if ( m_pChecking->checkCodeLists )
     checkBoxCodelists->Checked = true;
   else
     checkBoxCodelists->Checked = false;

   if ( m_pChecking->checkReferenzen )
     checkBoxReferenzen->Checked = true;
   else
     checkBoxReferenzen->Checked = false;

   if ( m_pChecking->checkCoordReferenceSystems )
     checkBoxKoordinatenReferenzsystem->Checked = true;
   else
     checkBoxKoordinatenReferenzsystem->Checked = false;

   switch ( m_pFeatures->getGmlTyp() )
   {
   case XPLANGML_2_0:
   case XPLANGML_3_0:
   case XPLANGML_4_0:
   case XPLANGML_4_1:
   case XPLANGML_5_0:
   case XPLANGML_5_1:
   case XPLANGML_5_2:
   case XPLANGML_5_3:
     if ( m_pChecking->checkXPlanGMLRules )
     checkBoxSpezSyntaxCheck->Checked = true;
    else
     checkBoxSpezSyntaxCheck->Checked = false;

    checkXPlanGMLBackwardPointer = m_pChecking->checkXPlanGMLBackwardPointer;
    checkXPlanGMLUOM             = m_pChecking->checkXPlanGMLUOM;
    break;

   case INSPIRE_LU_PLANNED:
   case INSPIRE:
     checkBoxSpezSyntaxCheck->Enabled = false;
     break;

   case NAS_5_1:
   case NAS_5_1_1:
   case NAS_6_0:
     checkBoxSpezSyntaxCheck->Enabled = false;
     break;

   case CITYGML_0_4_0:
   case CITYGML_1_0:
   case CITYGML_2_0:
   case CITYGML_3_0:
     break;

   case BOREHOLEML_3_0:
     checkBoxSpezSyntaxCheck->Enabled = false;
     break;

   case GML_SCHEMA_DEFAULT:
     checkBoxSpezSyntaxCheck->Enabled = false;

   default:
     checkBoxSpezSyntaxCheck->Enabled = false;
     break;
   }

   geometryCheckLevel = m_pChecking->getGeometryCheckLevel();
   comboBoxGeometryCheckLevel->SelectedIndex = geometryCheckLevel;

   buttonKorrektur->Enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Generieren" (Modell-Statistik)                    //
//  Generierung Modell-Statistik                                             //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonModellStatistik_Click(System::Object^  sender, System::EventArgs^  e) 
{
  m_pChecking->GenerateModelStatistics( );

 ModellStatistik ^ pModellStatistik = gcnew ModellStatistik ( m_pChecking->getModellStatistik(), m_gmlDateiName );
 pModellStatistik->ShowDialog();
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Ausführen" (Validierung)                          //
//  Schema-Validierung                                                       //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonSchemaValidierung_Click(System::Object^  sender, System::EventArgs^  e) 
{

  checkValidation();  

  if (  m_pChecking->getErrorReport()->getValidationErrorAnz() > 0 )
  {
    ValidierungsfehlerDialog ^ pDialog = gcnew ValidierungsfehlerDialog ( m_pChecking->getErrorReport(), m_gmlDateiName );
    pDialog->ShowDialog();
  }
  else
    textBoxFehlerAnzahl->Text = "Kein Validierungsfehler festgestellt";
}

///////////////////////////////////////////////////////////////////////////////
//  Durchführung der Schema-Validierung                                      //
///////////////////////////////////////////////////////////////////////////////
private:System::Void checkValidation()
{
  string fileName;
  String ^ PROFILE_NAME;
  string profileName = "";

  if ( profilAnz > 0 )
  {
    PROFILE_NAME = dynamic_cast<String^>(comboBoxProfil->SelectedItem);
    if ( PROFILE_NAME != nullptr )
      QuConvert::systemStr2stdStr( profileName, PROFILE_NAME );
  } 
  else 
    profileName = "";

  validated = false;
  m_pChecking->resetValidation();

  if ( m_gmlDateiName == nullptr )
  {
    fileName =  Constant::getSystemFolder() + "\\TempFile.gml" ;
    m_pFeatures->writeFeatures ( fileName );
  }
  else
    QuConvert::systemStr2stdStr ( fileName, m_gmlDateiName );

    m_pChecking->ValidateModelAgainstXMLSchema( fileName, profileName );

  if ( m_pFeatures->getGmlSchema()->isCityGml() && checkBoxSchematron->Checked == true )
    m_pChecking->ValidateModelAgainstSchematronRules ( fileName, profileName );

  validated = true;
}


///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Ausführen" (Konformitäts-Checks)                  //
//  Semantische Validierung                                                  //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonSyntaxChecks_Click(System::Object^  sender, System::EventArgs^  e)
{
  checkConformity();

  if ( m_pChecking->getErrorReport()->getSemanticErrorAnz() > 0 )
  {
    KonformitaetsFehler ^ pKonformitaetsFehler = gcnew KonformitaetsFehler ( m_pFeatures, m_pChecking );
    pKonformitaetsFehler->ShowDialog();
  }
  else
    textBoxFehlerAnzahl->Text = "Kein Konformitätsfehler festgestellt";
}

///////////////////////////////////////////////////////////////////////////////
//  Durchführung der semantischen Konformitätsregeln                         //
///////////////////////////////////////////////////////////////////////////////
private: System::Void checkConformity()
{
  String ^ PROFILE_NAME;
  string profileName = "";

  if ( profilAnz > 0 )
  {
    PROFILE_NAME = dynamic_cast<String^>(comboBoxProfil->SelectedItem);
    if ( PROFILE_NAME != nullptr )
      QuConvert::systemStr2stdStr( profileName, PROFILE_NAME );
  } 
  else 
    profileName = "";

  m_pChecking->resetSemanticErrors();

  if (  checkBoxRegeln->Checked )
    m_pChecking->CheckSemanticRules( profileName );

  if ( checkBoxKoordinatenReferenzsystem->Checked )
    m_pChecking->CheckCoordReferenceSystems();

  if ( checkBoxCodelists->Checked )
    m_pChecking->CheckCodeLists();


  if ( checkBoxReferenzen->Checked )
    m_pChecking->CheckReferenzen();

  if ( checkBoxSpezSyntaxCheck->Checked )
  {
    switch ( m_pFeatures->getGmlTyp() )
    {
    case XPLANGML_2_0:
    case XPLANGML_3_0:
    case XPLANGML_4_0:
    case XPLANGML_4_1:
    case XPLANGML_5_0:
    case XPLANGML_5_1:
    case XPLANGML_5_2:
    case XPLANGML_5_3:
      m_pChecking->CheckXPlanGMLRules();
      break;

    case INSPIRE_LU_PLANNED:
    case INSPIRE:
      break;

    case NAS_5_1:
    case NAS_5_1_1:
    case NAS_6_0:
      break;

    case CITYGML_0_4_0:
    case CITYGML_1_0:
    case CITYGML_2_0:
    case CITYGML_3_0:
      break;

    case BOREHOLEML_3_0:
      break;

    case GML_SCHEMA_DEFAULT:
      break;

    default:
      break;
    }
  }

  conformityErrorsChecked = true;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Details"                                          //
//  Einstellen der speziellen Prüfungen für XPlanGML und CityGML             //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonSyntaxChecksSpezifisch_Click(System::Object^  sender, System::EventArgs^  e)
{
  if ( m_pFeatures->isXPlanGML() )
  {
    SemantiktestXPlanGML ^ pDialog = gcnew SemantiktestXPlanGML ( checkXPlanGMLBackwardPointer, checkXPlanGMLUOM );
    pDialog->ShowDialog();

    checkXPlanGMLBackwardPointer = pDialog->m_checkBackwardReferences;
    m_pChecking->checkXPlanGMLBackwardPointer = checkXPlanGMLBackwardPointer;

    checkXPlanGMLUOM = pDialog->m_checkUOM;
    m_pChecking->checkXPlanGMLUOM = checkXPlanGMLUOM;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Ausführen" (Geometrie-Checks)                     //
//  Geometrische Validierung                                                 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonGeometrieChecks_Click(System::Object^  sender, System::EventArgs^  e)
{
  textBoxFehlerAnzahl->Text = "";
  textBoxFehlerAnzahl->Refresh();
   
  checkGeometry();

  int anz = (int)m_pChecking->getErrorReport()->getGeometryErrorAnz();
  if (  anz > 0 )
  {
    textBoxFehlerAnzahl->Text = String::Concat ( Convert::ToString( anz ), " Geometriefehler festgestellt" );

    if ( comboBoxAnzeigeGeometriefehler->SelectedIndex == 0 )
    {
      GeometryErrorUebersicht ^ pDialog = gcnew GeometryErrorUebersicht ( m_pFeatures, m_pChecking, m_CityGMLViewer );
      pDialog->ShowDialog();
    }
    else
    if ( comboBoxAnzeigeGeometriefehler->SelectedIndex == 1 )
    {
      GeometryErrorsPerFeature ^ pDialog = gcnew GeometryErrorsPerFeature (  m_pFeatures, m_pChecking, m_CityGMLViewer );
      pDialog->ShowDialog();
    }

    buttonKorrektur->Enabled = true;
  }
  else
    textBoxFehlerAnzahl->Text = "Kein Geometriefehler festgestellt";
}

///////////////////////////////////////////////////////////////////////////////
//  Durchführung der geometrischen Prüfungen                                 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void checkGeometry()
{
  m_pChecking->resetGeometryErrors();

  m_pChecking->setGeometryCheckLevel( comboBoxGeometryCheckLevel->SelectedIndex );
  m_pChecking->CheckGeometry( progressBarTest );

  geometryErrorsChecked = true;
}


///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Schemaspezifische Checks"                         //
//  Einstellen der speziellen Geometrieprüfungen für XPlanGML und CityGML    //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonGeometrieChecksSpezifisch_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( m_pFeatures->isXPlanGML() )
   {
     GeometrietestXPlanGML ^ pDialog = gcnew GeometrietestXPlanGML ( m_pChecking->checkFlaechenschlussGrob,
            m_pChecking->checkFlaechenschlussExakt, m_pChecking->checkVollstaendigeUeberdeckung );
     pDialog->ShowDialog();

     m_pChecking->checkFlaechenschlussGrob       = pDialog->m_checkFlaechenschlussGrob;
     m_pChecking->checkFlaechenschlussExakt      = pDialog->m_checkFlaechenschlussExakt;
     m_pChecking->checkVollstaendigeUeberdeckung = pDialog->m_checkUeberdeckung;
   }

   if ( m_pFeatures->isCityGML() )
   {
     GeometrietestCityGML ^ pDialog = gcnew GeometrietestCityGML (  m_pChecking->checkBoundarySurfaces, m_pChecking->checkOuterShell );
     pDialog->ShowDialog();

     m_pChecking->checkBoundarySurfaces  = pDialog->checkBoundarySurfaces;
     m_pChecking->checkOuterShell        = pDialog->checkOuterShell;
   }
}
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Weitere Konformitätsregeln"                       //
//  Einlesen zusätzlicher Konformitätsregeln                                 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonReadProfil_Click(System::Object^  sender, System::EventArgs^  e)
{
  SemanticRules * pSemanticRules = m_pChecking->getSemanticRules();
  string          fileName;
  int             i;

  ProfileReader ^ pProfileReader = gcnew ProfileReader ( pSemanticRules );

  System::Windows::Forms::OpenFileDialog ^ pDialog = gcnew System::Windows::Forms::OpenFileDialog;
  pDialog->Filter = "xml Dateien (*.xml)|*.xml|Alle Dateien (*.*)|*.*";

  System::Windows::Forms::DialogResult ok = pDialog->ShowDialog();

  if (  ok != System::Windows::Forms::DialogResult::OK )
    return;

  for ( i = 0; i < pDialog->FileNames->Length; i++ )
  {
    String ^ STR = pDialog->FileNames[i];
    QuConvert::systemStr2stdStr( fileName, STR );
    pProfileReader->read( fileName );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Parameter"                                        //
//  Einstellung allgemeiner Parameter für die Geometrieprüfung               //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonCheckParams_Click(System::Object^  sender, System::EventArgs^  e)
{
  
  GeometryTestParams ^ pTestParams = gcnew GeometryTestParams ( m_pChecking->getCheckingParams() );
  pTestParams->ShowDialog();
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Korrektur"                                        //
//  Durchführung spezieller geometrischer Korrekturen                        //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonKorrektur_Click(System::Object^  sender, System::EventArgs^  e)
{
  GeometryKorrektur ^ pDialog = gcnew GeometryKorrektur ( m_pFeatures, m_pChecking );

  pDialog->ShowDialog();
  buttonKorrektur->Enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl einer Geometrieprüfung mit ComboBox              //
//  Geometrie-Checks                                                         //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxGeometryCheckLevel_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
         {
           geometryCheckLevel =  comboBoxGeometryCheckLevel->SelectedIndex;
         }
};
