#pragma once


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "GMLWriter.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "Convert.h"
#include "Constant.h"
#include "Checking.h"
#include "GeometryInspector.h"
#include "GmlNamespaces.h"
#include "includes.h"
#include "FeatureExport.h"

	/// <summary>
	/// Anzeige von detaillierten Geometriefehler-Informationen für einen bestimmten Fehler-Typ
  /// Wenn der Parameter errorNumber >= 0 ist, wird nur die Information über den ausgewählten Fehler angezeigt
	/// </summary>
	public ref class GeometryErrorDetails : public System::Windows::Forms::Form
	{
	public:     
		GeometryErrorDetails(  Features * pFeatures, Checking * pChecking, String ^ cityGMLViewer, int typeGeometryError, int errorNumber  )
		{
      double planarityErrorMax;
      double planarityErrorAvarage;

      vector<GeometryError*>                           vGeometryErrors;
      multimap<double,GeometryError*>                   mGeometryErrors;
      multimap<double,GeometryError*>::reverse_iterator iter;

  	  InitializeComponent();

      m_pFeatures     = pFeatures;
      m_pChecking     = pChecking;
      m_cityGMLViewer = cityGMLViewer;
      m_pErrorReport  = m_pChecking->getErrorReport();
      m_errorNumbers  = gcnew ArrayList;
      m_typeFehlerTyp = typeGeometryError;

      if ( m_cityGMLViewer->CompareTo ( "" ) == 0 )
        buttonShow->Enabled = false;

      index1 = 0;
      index2 = 0;

      m_pFeature        = NULL;
      m_pFeatureRelated = NULL;

      textBoxFehlerTyp->Text =  QuConvert::ToString( GeometryError::getGeometryErrorTypeAsString ( typeGeometryError )  );

      if ( errorNumber < 0 )
      {
        m_geometryErrorAnz = m_pErrorReport->getGeometryErrors( typeGeometryError,  vGeometryErrors );

        for ( int i = 0; i < m_geometryErrorAnz; i++ )
        {

          vGeometryErrors[i]->getPlanaritaetsfehler( planarityErrorMax, planarityErrorAvarage );
          mGeometryErrors.insert(  multimap<double,GeometryError*>::value_type ( planarityErrorMax, vGeometryErrors[i] ) );
        }
      }
      else
      {
        m_geometryErrorAnz = 1;
        GeometryError * pError = m_pErrorReport->getGeometryError( errorNumber );
        pError->getPlanaritaetsfehler( planarityErrorMax, planarityErrorAvarage );
        mGeometryErrors.insert(  multimap<double,GeometryError*>::value_type ( planarityErrorMax,pError ) );
      }

      for (iter = mGeometryErrors.rbegin(); iter != mGeometryErrors.rend(); iter++ )
      {
        int number = iter->second->getErrorNumber();
        m_errorNumbers->Add( number );
      }

      if ( m_geometryErrorAnz > 0  )
      {
        m_indexGeometryError = 0;
        displayGeometryError();
      }
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~GeometryErrorDetails()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxFehlerTyp;
  protected: 
  private: System::Windows::Forms::TextBox^  textBoxFehlerBeschreibung;
  private: System::Windows::Forms::TextBox^  textBoxObjekt1Id;
  private: System::Windows::Forms::TextBox^  textBoxGeometrie1Id;
  private: System::Windows::Forms::Label^  labelObjektId1;
  private: System::Windows::Forms::Label^  labelGeometrieId1;
  private: System::Windows::Forms::Button^  buttonDetails1;
  private: System::Windows::Forms::Button^  buttonZurueck;
  private: System::Windows::Forms::TextBox^  textBoxFehler;
  private: System::Windows::Forms::Button^  buttonVor;
  private: System::Windows::Forms::Button^  buttonBeenden;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

    Features        * m_pFeatures;                // Aggregation aller Features
    Checking        * m_pChecking;                // Prüfungs-Modul
    ErrorReport     * m_pErrorReport;             // Fehler-Report
    String          ^ m_cityGMLViewer;            // Generischer GML-Viewer
    ArrayList       ^ m_errorNumbers;             // Liste aller anzuzeigender Fehler
    int               m_typeFehlerTyp;            // Festgelegter Fehlertyp (Enum GeometryError::GEOMETRY_ERROR_TYPE)
		int               m_indexGeometryError;       // Index des aktuell angezeigten Fehlers
    String          ^ m_typeGeometryErrorString;  // Festgelegter Fehlertyp als Text
		size_t            m_geometryErrorAnz;         // Anzahl der Geometriefehler
    Feature         * m_pFeature;                 // Zugehöriges Feature des aktuellen Fehlers (aktuelles Feature)
    Feature         * m_pRootFeature;             // Basis-Feature im Szenenbaum des aktuellen Features
    Feature         * m_pFeatureRelated;          // Mit dem aktuellen Feature verbundenes Feature
		size_t            index1, index2;             // Indizes zur Lokalisation der Selbstüberlappung von Rind-Konturen
    GeometryError   * pError;                     // Aktueller Geometriefehler

private: System::Windows::Forms::TextBox^  textBoxFehlerDetails;
  private: System::Windows::Forms::TextBox^  textBoxObjekt2Id;
  private: System::Windows::Forms::Label^  labelObjektId2;
  private: System::Windows::Forms::TextBox^  textBoxGeometrie2Id;
  private: System::Windows::Forms::Label^  labelGeometrieId2;
  private: System::Windows::Forms::TextBox^  textBoxGeometryProperty;
  private: System::Windows::Forms::Label^  labelGeometryProperty;
  private: System::Windows::Forms::TextBox^  textBoxErrorClassification;
  private: System::Windows::Forms::Button^  buttonExportieren;
  private: System::Windows::Forms::SaveFileDialog^  saveFileDialog;
  private: System::Windows::Forms::Button^  buttonShow;
  private: System::Windows::Forms::TextBox^  textBoxDatei;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::GroupBox^  groupBox1;
  private: System::Windows::Forms::TextBox^  textBoxGeometryProperty2;
  private: System::Windows::Forms::GroupBox^  groupBox2;
  private: System::Windows::Forms::Label^  labelGeometryId2;
  private: System::Windows::Forms::Label^  labelGeometryProperty2;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::TextBox^  textBoxRootFeatureId;
	private: System::Windows::Forms::Button^  buttonDetails2;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBoxFehlerTyp = ( gcnew System::Windows::Forms::TextBox () );
			this->textBoxFehlerBeschreibung = ( gcnew System::Windows::Forms::TextBox () );
			this->textBoxObjekt1Id = ( gcnew System::Windows::Forms::TextBox () );
			this->textBoxGeometrie1Id = ( gcnew System::Windows::Forms::TextBox () );
			this->labelObjektId1 = ( gcnew System::Windows::Forms::Label () );
			this->labelGeometrieId1 = ( gcnew System::Windows::Forms::Label () );
			this->buttonDetails1 = ( gcnew System::Windows::Forms::Button () );
			this->buttonZurueck = ( gcnew System::Windows::Forms::Button () );
			this->textBoxFehler = ( gcnew System::Windows::Forms::TextBox () );
			this->buttonVor = ( gcnew System::Windows::Forms::Button () );
			this->buttonBeenden = ( gcnew System::Windows::Forms::Button () );
			this->textBoxFehlerDetails = ( gcnew System::Windows::Forms::TextBox () );
			this->textBoxObjekt2Id = ( gcnew System::Windows::Forms::TextBox () );
			this->labelObjektId2 = ( gcnew System::Windows::Forms::Label () );
			this->textBoxGeometrie2Id = ( gcnew System::Windows::Forms::TextBox () );
			this->labelGeometrieId2 = ( gcnew System::Windows::Forms::Label () );
			this->buttonDetails2 = ( gcnew System::Windows::Forms::Button () );
			this->textBoxGeometryProperty = ( gcnew System::Windows::Forms::TextBox () );
			this->labelGeometryProperty = ( gcnew System::Windows::Forms::Label () );
			this->textBoxErrorClassification = ( gcnew System::Windows::Forms::TextBox () );
			this->buttonExportieren = ( gcnew System::Windows::Forms::Button () );
			this->saveFileDialog = ( gcnew System::Windows::Forms::SaveFileDialog () );
			this->buttonShow = ( gcnew System::Windows::Forms::Button () );
			this->textBoxDatei = ( gcnew System::Windows::Forms::TextBox () );
			this->label1 = ( gcnew System::Windows::Forms::Label () );
			this->label2 = ( gcnew System::Windows::Forms::Label () );
			this->groupBox1 = ( gcnew System::Windows::Forms::GroupBox () );
			this->textBoxGeometryProperty2 = ( gcnew System::Windows::Forms::TextBox () );
			this->groupBox2 = ( gcnew System::Windows::Forms::GroupBox () );
			this->labelGeometryId2 = ( gcnew System::Windows::Forms::Label () );
			this->labelGeometryProperty2 = ( gcnew System::Windows::Forms::Label () );
			this->textBoxRootFeatureId = ( gcnew System::Windows::Forms::TextBox () );
			this->label3 = ( gcnew System::Windows::Forms::Label () );
			this->groupBox1->SuspendLayout ();
			this->groupBox2->SuspendLayout ();
			this->SuspendLayout ();
			// 
			// textBoxFehlerTyp
			// 
			this->textBoxFehlerTyp->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->textBoxFehlerTyp->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>( 0 ) ) );
			this->textBoxFehlerTyp->ForeColor = System::Drawing::Color::Red;
			this->textBoxFehlerTyp->Location = System::Drawing::Point ( 9, 12 );
			this->textBoxFehlerTyp->Multiline = true;
			this->textBoxFehlerTyp->Name = L"textBoxFehlerTyp";
			this->textBoxFehlerTyp->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBoxFehlerTyp->Size = System::Drawing::Size ( 430, 39 );
			this->textBoxFehlerTyp->TabIndex = 0;
			// 
			// textBoxFehlerBeschreibung
			// 
			this->textBoxFehlerBeschreibung->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->textBoxFehlerBeschreibung->Location = System::Drawing::Point ( 9, 57 );
			this->textBoxFehlerBeschreibung->Multiline = true;
			this->textBoxFehlerBeschreibung->Name = L"textBoxFehlerBeschreibung";
			this->textBoxFehlerBeschreibung->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBoxFehlerBeschreibung->Size = System::Drawing::Size ( 430, 35 );
			this->textBoxFehlerBeschreibung->TabIndex = 1;
			// 
			// textBoxObjekt1Id
			// 
			this->textBoxObjekt1Id->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->textBoxObjekt1Id->Location = System::Drawing::Point ( 17, 67 );
			this->textBoxObjekt1Id->Name = L"textBoxObjekt1Id";
			this->textBoxObjekt1Id->Size = System::Drawing::Size ( 315, 20 );
			this->textBoxObjekt1Id->TabIndex = 3;
			// 
			// textBoxGeometrie1Id
			// 
			this->textBoxGeometrie1Id->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->textBoxGeometrie1Id->Location = System::Drawing::Point ( 17, 146 );
			this->textBoxGeometrie1Id->Name = L"textBoxGeometrie1Id";
			this->textBoxGeometrie1Id->Size = System::Drawing::Size ( 315, 20 );
			this->textBoxGeometrie1Id->TabIndex = 4;
			// 
			// labelObjektId1
			// 
			this->labelObjektId1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left ) );
			this->labelObjektId1->AutoSize = true;
			this->labelObjektId1->Location = System::Drawing::Point ( 17, 88 );
			this->labelObjektId1->Name = L"labelObjektId1";
			this->labelObjektId1->Size = System::Drawing::Size ( 50, 13 );
			this->labelObjektId1->TabIndex = 5;
			this->labelObjektId1->Text = L"Objekt Id";
			// 
			// labelGeometrieId1
			// 
			this->labelGeometrieId1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left ) );
			this->labelGeometrieId1->AutoSize = true;
			this->labelGeometrieId1->Location = System::Drawing::Point ( 17, 169 );
			this->labelGeometrieId1->Name = L"labelGeometrieId1";
			this->labelGeometrieId1->Size = System::Drawing::Size ( 67, 13 );
			this->labelGeometrieId1->TabIndex = 6;
			this->labelGeometrieId1->Text = L"Geometrie-Id";
			// 
			// buttonDetails1
			// 
			this->buttonDetails1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right ) );
			this->buttonDetails1->Location = System::Drawing::Point ( 338, 63 );
			this->buttonDetails1->Name = L"buttonDetails1";
			this->buttonDetails1->Size = System::Drawing::Size ( 75, 107 );
			this->buttonDetails1->TabIndex = 7;
			this->buttonDetails1->Text = L"Details";
			this->buttonDetails1->UseVisualStyleBackColor = true;
			this->buttonDetails1->Click += gcnew System::EventHandler ( this, &GeometryErrorDetails::buttonDetails_Click );
			// 
			// buttonZurueck
			// 
			this->buttonZurueck->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left ) );
			this->buttonZurueck->Location = System::Drawing::Point ( 84, 598 );
			this->buttonZurueck->Name = L"buttonZurueck";
			this->buttonZurueck->Size = System::Drawing::Size ( 41, 23 );
			this->buttonZurueck->TabIndex = 8;
			this->buttonZurueck->Text = L"<--";
			this->buttonZurueck->UseVisualStyleBackColor = true;
			this->buttonZurueck->Click += gcnew System::EventHandler ( this, &GeometryErrorDetails::buttonZurueck_Click );
			// 
			// textBoxFehler
			// 
			this->textBoxFehler->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->textBoxFehler->Location = System::Drawing::Point ( 131, 601 );
			this->textBoxFehler->Name = L"textBoxFehler";
			this->textBoxFehler->Size = System::Drawing::Size ( 151, 20 );
			this->textBoxFehler->TabIndex = 9;
			this->textBoxFehler->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// buttonVor
			// 
			this->buttonVor->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->buttonVor->Location = System::Drawing::Point ( 288, 601 );
			this->buttonVor->Name = L"buttonVor";
			this->buttonVor->Size = System::Drawing::Size ( 41, 23 );
			this->buttonVor->TabIndex = 10;
			this->buttonVor->Text = L"-->";
			this->buttonVor->UseVisualStyleBackColor = true;
			this->buttonVor->Click += gcnew System::EventHandler ( this, &GeometryErrorDetails::buttonVor_Click );
			// 
			// buttonBeenden
			// 
			this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right ) );
			this->buttonBeenden->Location = System::Drawing::Point ( 347, 589 );
			this->buttonBeenden->Name = L"buttonBeenden";
			this->buttonBeenden->Size = System::Drawing::Size ( 75, 47 );
			this->buttonBeenden->TabIndex = 11;
			this->buttonBeenden->Text = L"Beenden";
			this->buttonBeenden->UseVisualStyleBackColor = true;
			this->buttonBeenden->Click += gcnew System::EventHandler ( this, &GeometryErrorDetails::buttonBeenden_Click );
			// 
			// textBoxFehlerDetails
			// 
			this->textBoxFehlerDetails->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( ( System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom )
				| System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->textBoxFehlerDetails->Location = System::Drawing::Point ( 9, 101 );
			this->textBoxFehlerDetails->Multiline = true;
			this->textBoxFehlerDetails->Name = L"textBoxFehlerDetails";
			this->textBoxFehlerDetails->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBoxFehlerDetails->Size = System::Drawing::Size ( 430, 45 );
			this->textBoxFehlerDetails->TabIndex = 12;
			// 
			// textBoxObjekt2Id
			// 
			this->textBoxObjekt2Id->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->textBoxObjekt2Id->Location = System::Drawing::Point ( 17, 20 );
			this->textBoxObjekt2Id->Name = L"textBoxObjekt2Id";
			this->textBoxObjekt2Id->Size = System::Drawing::Size ( 315, 20 );
			this->textBoxObjekt2Id->TabIndex = 13;
			// 
			// labelObjektId2
			// 
			this->labelObjektId2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left ) );
			this->labelObjektId2->AutoSize = true;
			this->labelObjektId2->Location = System::Drawing::Point ( 16, 43 );
			this->labelObjektId2->Name = L"labelObjektId2";
			this->labelObjektId2->Size = System::Drawing::Size ( 50, 13 );
			this->labelObjektId2->TabIndex = 14;
			this->labelObjektId2->Text = L"Objekt Id";
			// 
			// textBoxGeometrie2Id
			// 
			this->textBoxGeometrie2Id->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->textBoxGeometrie2Id->Location = System::Drawing::Point ( 17, 109 );
			this->textBoxGeometrie2Id->Name = L"textBoxGeometrie2Id";
			this->textBoxGeometrie2Id->Size = System::Drawing::Size ( 315, 20 );
			this->textBoxGeometrie2Id->TabIndex = 15;
			// 
			// labelGeometrieId2
			// 
			this->labelGeometrieId2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left ) );
			this->labelGeometrieId2->AutoSize = true;
			this->labelGeometrieId2->Location = System::Drawing::Point ( 6, 558 );
			this->labelGeometrieId2->Name = L"labelGeometrieId2";
			this->labelGeometrieId2->Size = System::Drawing::Size ( 67, 13 );
			this->labelGeometrieId2->TabIndex = 16;
			this->labelGeometrieId2->Text = L"Geometrie-Id";
			// 
			// buttonDetails2
			// 
			this->buttonDetails2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right ) );
			this->buttonDetails2->Location = System::Drawing::Point ( 338, 21 );
			this->buttonDetails2->Name = L"buttonDetails2";
			this->buttonDetails2->Size = System::Drawing::Size ( 75, 109 );
			this->buttonDetails2->TabIndex = 17;
			this->buttonDetails2->Text = L"Details";
			this->buttonDetails2->UseVisualStyleBackColor = true;
			this->buttonDetails2->Click += gcnew System::EventHandler ( this, &GeometryErrorDetails::buttonDetails2_Click );
			// 
			// textBoxGeometryProperty
			// 
			this->textBoxGeometryProperty->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->textBoxGeometryProperty->Location = System::Drawing::Point ( 17, 105 );
			this->textBoxGeometryProperty->Name = L"textBoxGeometryProperty";
			this->textBoxGeometryProperty->Size = System::Drawing::Size ( 315, 20 );
			this->textBoxGeometryProperty->TabIndex = 19;
			// 
			// labelGeometryProperty
			// 
			this->labelGeometryProperty->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left ) );
			this->labelGeometryProperty->AutoSize = true;
			this->labelGeometryProperty->Location = System::Drawing::Point ( 17, 126 );
			this->labelGeometryProperty->Name = L"labelGeometryProperty";
			this->labelGeometryProperty->Size = System::Drawing::Size ( 114, 13 );
			this->labelGeometryProperty->TabIndex = 20;
			this->labelGeometryProperty->Text = L"Geometrie-Eigenschaft";
			// 
			// textBoxErrorClassification
			// 
			this->textBoxErrorClassification->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left ) );
			this->textBoxErrorClassification->BackColor = System::Drawing::SystemColors::Menu;
			this->textBoxErrorClassification->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>( 0 ) ) );
			this->textBoxErrorClassification->ForeColor = System::Drawing::Color::Red;
			this->textBoxErrorClassification->Location = System::Drawing::Point ( 94, 156 );
			this->textBoxErrorClassification->Name = L"textBoxErrorClassification";
			this->textBoxErrorClassification->Size = System::Drawing::Size ( 111, 20 );
			this->textBoxErrorClassification->TabIndex = 22;
			// 
			// buttonExportieren
			// 
			this->buttonExportieren->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left ) );
			this->buttonExportieren->Location = System::Drawing::Point ( 3, 613 );
			this->buttonExportieren->Name = L"buttonExportieren";
			this->buttonExportieren->Size = System::Drawing::Size ( 70, 23 );
			this->buttonExportieren->TabIndex = 23;
			this->buttonExportieren->Text = L"Exportieren";
			this->buttonExportieren->UseVisualStyleBackColor = true;
			this->buttonExportieren->Click += gcnew System::EventHandler ( this, &GeometryErrorDetails::buttonExportieren_Click );
			// 
			// saveFileDialog
			// 
			this->saveFileDialog->DefaultExt = L"*.gml";
			this->saveFileDialog->Filter = L"(*.gml)|*.gml;";
			// 
			// buttonShow
			// 
			this->buttonShow->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left ) );
			this->buttonShow->Location = System::Drawing::Point ( 5, 579 );
			this->buttonShow->Name = L"buttonShow";
			this->buttonShow->Size = System::Drawing::Size ( 69, 23 );
			this->buttonShow->TabIndex = 24;
			this->buttonShow->Text = L"Zeigen";
			this->buttonShow->UseVisualStyleBackColor = true;
			this->buttonShow->Click += gcnew System::EventHandler ( this, &GeometryErrorDetails::buttonShow_Click );
			// 
			// textBoxDatei
			// 
			this->textBoxDatei->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->textBoxDatei->Location = System::Drawing::Point ( 9, 180 );
			this->textBoxDatei->Name = L"textBoxDatei";
			this->textBoxDatei->ReadOnly = true;
			this->textBoxDatei->Size = System::Drawing::Size ( 430, 20 );
			this->textBoxDatei->TabIndex = 25;
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left ) );
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point ( 6, 206 );
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size ( 32, 13 );
			this->label1->TabIndex = 26;
			this->label1->Text = L"Datei";
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->label2->AutoSize = true;
			this->label2->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>( 0 ) ) );
			this->label2->ForeColor = System::Drawing::Color::Red;
			this->label2->Location = System::Drawing::Point ( 6, 158 );
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size ( 82, 13 );
			this->label2->TabIndex = 27;
			this->label2->Text = L"Klassifikation";
			// 
			// groupBox1
			// 
			this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->groupBox1->Controls->Add ( this->label3 );
			this->groupBox1->Controls->Add ( this->textBoxRootFeatureId );
			this->groupBox1->Controls->Add ( this->labelGeometryProperty );
			this->groupBox1->Controls->Add ( this->textBoxGeometryProperty );
			this->groupBox1->Controls->Add ( this->buttonDetails1 );
			this->groupBox1->Controls->Add ( this->labelGeometrieId1 );
			this->groupBox1->Controls->Add ( this->labelObjektId1 );
			this->groupBox1->Controls->Add ( this->textBoxGeometrie1Id );
			this->groupBox1->Controls->Add ( this->textBoxObjekt1Id );
			this->groupBox1->Location = System::Drawing::Point ( 9, 206 );
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size ( 427, 202 );
			this->groupBox1->TabIndex = 28;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Aktuelles Feature";
			// 
			// textBoxGeometryProperty2
			// 
			this->textBoxGeometryProperty2->Location = System::Drawing::Point ( 17, 66 );
			this->textBoxGeometryProperty2->Name = L"textBoxGeometryProperty2";
			this->textBoxGeometryProperty2->Size = System::Drawing::Size ( 315, 20 );
			this->textBoxGeometryProperty2->TabIndex = 29;
			// 
			// groupBox2
			// 
			this->groupBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left )
				| System::Windows::Forms::AnchorStyles::Right ) );
			this->groupBox2->Controls->Add ( this->labelGeometryId2 );
			this->groupBox2->Controls->Add ( this->labelGeometryProperty2 );
			this->groupBox2->Controls->Add ( this->textBoxGeometryProperty2 );
			this->groupBox2->Controls->Add ( this->textBoxObjekt2Id );
			this->groupBox2->Controls->Add ( this->buttonDetails2 );
			this->groupBox2->Controls->Add ( this->textBoxGeometrie2Id );
			this->groupBox2->Controls->Add ( this->labelObjektId2 );
			this->groupBox2->Location = System::Drawing::Point ( 9, 409 );
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size ( 430, 160 );
			this->groupBox2->TabIndex = 30;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Verbundenes Feature";
			// 
			// labelGeometryId2
			// 
			this->labelGeometryId2->AutoSize = true;
			this->labelGeometryId2->Location = System::Drawing::Point ( 20, 131 );
			this->labelGeometryId2->Name = L"labelGeometryId2";
			this->labelGeometryId2->Size = System::Drawing::Size ( 67, 13 );
			this->labelGeometryId2->TabIndex = 31;
			this->labelGeometryId2->Text = L"Geometrie-Id";
			// 
			// labelGeometryProperty2
			// 
			this->labelGeometryProperty2->AutoSize = true;
			this->labelGeometryProperty2->Location = System::Drawing::Point ( 16, 90 );
			this->labelGeometryProperty2->Name = L"labelGeometryProperty2";
			this->labelGeometryProperty2->Size = System::Drawing::Size ( 114, 13 );
			this->labelGeometryProperty2->TabIndex = 30;
			this->labelGeometryProperty2->Text = L"Geometrie-Eigenschaft";
			// 
			// textBoxRootFeatureId
			// 
			this->textBoxRootFeatureId->Location = System::Drawing::Point ( 17, 17 );
			this->textBoxRootFeatureId->Name = L"textBoxRootFeatureId";
			this->textBoxRootFeatureId->Size = System::Drawing::Size ( 396, 20 );
			this->textBoxRootFeatureId->TabIndex = 21;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point ( 20, 42 );
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size ( 82, 13 );
			this->label3->TabIndex = 22;
			this->label3->Text = L"Base-Feature Id";
			// 
			// GeometryErrorDetails
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF ( 6, 13 );
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size ( 446, 652 );
			this->Controls->Add ( this->groupBox2 );
			this->Controls->Add ( this->groupBox1 );
			this->Controls->Add ( this->label2 );
			this->Controls->Add ( this->label1 );
			this->Controls->Add ( this->textBoxDatei );
			this->Controls->Add ( this->buttonShow );
			this->Controls->Add ( this->buttonExportieren );
			this->Controls->Add ( this->textBoxErrorClassification );
			this->Controls->Add ( this->labelGeometrieId2 );
			this->Controls->Add ( this->textBoxFehlerDetails );
			this->Controls->Add ( this->buttonBeenden );
			this->Controls->Add ( this->buttonVor );
			this->Controls->Add ( this->textBoxFehler );
			this->Controls->Add ( this->buttonZurueck );
			this->Controls->Add ( this->textBoxFehlerBeschreibung );
			this->Controls->Add ( this->textBoxFehlerTyp );
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"GeometryErrorDetails";
			this->Text = L"Geometriefehler";
			this->groupBox1->ResumeLayout ( false );
			this->groupBox1->PerformLayout ();
			this->groupBox2->ResumeLayout ( false );
			this->groupBox2->PerformLayout ();
			this->ResumeLayout ( false );
			this->PerformLayout ();

		}
#pragma endregion

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Informationen über den aktuellen Fehler in die Dialogbox    //
///////////////////////////////////////////////////////////////////////////////
private: System::Void displayGeometryError()
         {
           double planaritaetsfehlerMax, planaritaetsfehlerAvarage;


           int errorNumber = (int)m_errorNumbers[m_indexGeometryError];
           pError = m_pErrorReport->getGeometryError( errorNumber );
           index1 = pError->getSelfOverlapIndex1();
           index2 = pError->getSelfOverlapIndex2();

           m_pFeature = pError->getFeature();  
           if ( m_pFeature != NULL )
           {
              m_pRootFeature             = m_pFeature->getRootFeature();
              string featureDatei        = m_pFeature->getFeatures()->getFeatureDatei( m_pFeature->getGmlId() );
              textBoxDatei->Text         = QuConvert::ToString( featureDatei );
							textBoxRootFeatureId->Text = QuConvert::ToString ( m_pRootFeature->getGmlId() );
           }
           else
             m_pRootFeature = NULL;

           pError->getPlanaritaetsfehler( planaritaetsfehlerMax, planaritaetsfehlerAvarage );

           textBoxFehlerBeschreibung->Text  = QuConvert::ToString ( pError->getErrorDescription() );
           if ( planaritaetsfehlerMax > 0.0 )
           {
             String ^ STR = String::Concat ( "Planaritätsfehler: ", QuConvert::ToString( planaritaetsfehlerMax, 4 ) );
             if ( textBoxFehlerBeschreibung->Text->CompareTo( "" ) == 0 )
              textBoxFehlerBeschreibung->Text = STR;
             else
               textBoxFehlerBeschreibung->Text = String::Concat ( textBoxFehlerBeschreibung->Text, "; ", STR );
           }

           Error::ERROR_CLASSIFICATION errorClass = pError->getErrorClassification();
           textBoxErrorClassification->Text  = QuConvert::ToString ( Error::getErrorClassificationAsString( errorClass ) );
           if ( errorClass == Error::ERROR )
             textBoxErrorClassification->ForeColor = System::Drawing::Color::Red;
           else
             textBoxErrorClassification->ForeColor = System::Drawing::Color::Black;

           textBoxObjekt1Id->Text           = QuConvert::ToString( pError->getFeatureId() );
           textBoxGeometrie1Id->Text        = QuConvert::ToString( pError->getFeatureGeometryId() ); 
           textBoxGeometryProperty->Text    = QuConvert::ToString ( pError->getPropertyName() );
           textBoxFehler->Text              = String::Concat( Convert::ToString ( m_indexGeometryError+1 ), " (",  Convert::ToString ( m_geometryErrorAnz ), ")" );
          
           if ( m_pFeature != NULL  )
            labelObjektId1->Text = String::Concat ( "Objekt Id (", QuConvert::ToString ( m_pFeature->getKlassenName() ), ")" );
           else
             labelObjektId1->Text = "";

           if ( pError->getFeatureGeometryType() != NO_GEOMETRY )
             labelGeometrieId1->Text = String::Concat( "Geometrie-Id (", QuConvert::ToString( pError->getFeatureGeometryTypeAsString() ), ")" );
           else
             labelGeometrieId1->Text = "";

           if ( pError->getRelatedFeatureId() != "" )
           {
             m_pFeatureRelated = pError->getRelatedFeature();
             textBoxObjekt2Id->Text = QuConvert::ToString( pError->getRelatedFeatureId() );
             labelObjektId2->Text = String::Concat ( "ObjektId (", QuConvert::ToString ( m_pFeatureRelated->getKlassenName() ), ")" );
             textBoxGeometrie2Id->Text = QuConvert::ToString( pError->getRelatedFeatureGeometryId() ); 
             labelGeometryId2->Text = String::Concat( "Geometrie-Id (", QuConvert::ToString( pError->getRelatedFeatureGeometryTypeAsString() ), ")" );
             textBoxGeometryProperty2->Text    = QuConvert::ToString ( pError->getRelatedPropertyName() );
             buttonDetails2->Enabled = true;
           }
           else
           {
             m_pFeatureRelated = NULL;
             textBoxObjekt2Id->Text = "";
             labelObjektId2->Text = "";
             textBoxGeometrie2Id->Text = "";
             labelGeometryId2->Text   = "";
             textBoxGeometryProperty2->Text = "";
             buttonDetails2->Enabled = false;
           }


           switch ( pError->getGeometryErrorType() )
           {

           case GeometryError::SURFACE_OVERLAP_ERROR: 
           case GeometryError::XPLANGML_FLAECHENSCHLUSS_OVERLAPP:
             {
               double flaecheObj, flaecheObjRelated, flaecheUeberlappung;
               pError->getFlaechenangaben( flaecheObj, flaecheObjRelated, flaecheUeberlappung );
               if ( flaecheObj != 0.0 )
               {
                 String ^ STR;

                 STR = String::Concat( "Fläche 1: ", Convert::ToString( flaecheObj ), "\r\nFläche 2;",  Convert::ToString( flaecheObjRelated ),
                                        "\r\nÜberlappungsfläche: ", Convert::ToString ( flaecheUeberlappung ) );
                 textBoxFehlerDetails->Text = STR;
               }
               else
                 textBoxFehlerDetails->Text = "";

               break;
             }

           case GeometryError::SURFACE_TOTAL_COVERAGE_ERROR:
             {
               double flaecheEinzelobjekte, flaecheGeltungsbereich, flaecheUeberlappung, differenzflaeche, differenzflaecheProzentual;
               pError->getFlaechenangaben( flaecheEinzelobjekte, flaecheGeltungsbereich, flaecheUeberlappung );

               differenzflaeche =  fabs ( flaecheGeltungsbereich - flaecheEinzelobjekte );
               differenzflaecheProzentual =  (100.0 * differenzflaeche) / flaecheGeltungsbereich;

               String ^ STR = String::Concat( "Gesamtfläche der Flächenschlussobjekt (abzüglich Überlappung): ", Convert::ToString( flaecheEinzelobjekte ), " qm",
                                      "\r\nFläche des Geltungsbereichs",  Convert::ToString( flaecheGeltungsbereich ), " qm",
                                      "\r\nGesamte Überlappungsfläche: ", Convert::ToString ( flaecheUeberlappung ), " qm" );

               STR = String::Concat( STR, "\r\nDifferenzfläche: ", differenzflaeche, " qm (", differenzflaecheProzentual, " %)" );
               textBoxFehlerDetails->Text = STR;

               break;
             }

           case GeometryError::SOLID_EULER_CHECK_FAILED:
             textBoxFehlerDetails->Text = "Achtung, dies ist nicht in jedem Fall ein Fehler, da die Euler-Formel nur für konvexe Polytope erfüllt sein muss";
             break;


           default:
             textBoxFehlerDetails->Text = "";
           }           

        }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Details (Aktuelles Feature)                        //
//  Schreibt Informationen über das aktuelle Feature in die Dialogbox        //
///////////////////////////////////////////////////////////////////////////////
 private: System::Void buttonDetails_Click(System::Object^  sender, System::EventArgs^  e)
          {
            int errorNumber = (int)m_errorNumbers[m_indexGeometryError];
            GeometryError * pError = m_pErrorReport->getGeometryError( errorNumber );

            string id = pError->getFeatureGeometryId();
            _Geometrie * pGeo = m_pFeatures->getGeometry( id );

            GeometryInspector ^ pInspector = gcnew GeometryInspector( m_pChecking, m_pFeature, pGeo );

            pInspector->ShowDialog();

          }

////////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button <--                                                ///
//  Wählt den vorhergehenden Fehler bei der Iteration über die Fehlerliste aus//
////////////////////////////////////////////////////////////////////////////////
private: System::Void buttonZurueck_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           if ( m_indexGeometryError > 0 )
           {
             m_indexGeometryError--;
             displayGeometryError();
           }
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button -->                                                //
//  Wählt den nächsten Fehler bei der Iteration über die Fehlerliste aus     //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonVor_Click(System::Object^  sender, System::EventArgs^  e) 
         {  
           if ( m_indexGeometryError < m_geometryErrorAnz - 1 )
           {
             m_indexGeometryError++;
             displayGeometryError();
           }
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Beenden                                            //
//  Schließt den Dialog                                                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           Close();
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Details (Verbundenes Feature)                      //
//  Schreibt Informationen über das aktuelle Feature in die Dialogbox        //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonDetails2_Click(System::Object^  sender, System::EventArgs^  e)
         {
           GeometryInspector ^ pInspector = gcnew GeometryInspector( m_pChecking, m_pFeatureRelated, NULL );
           pInspector->ShowDialog();
         }


///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Exportieren                                        //
//  Generiert eine GML-Datei mit der fehlerhaften Geometrie sowie den        //
//  zugehörigen Features, und speichert diese Datei unter einem benutzer-    //
//  definierten Namen                                                        //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonExportieren_Click(System::Object^  sender, System::EventArgs^  e)
         {
           string                 fileName;
           vector<Feature*>       vFeatures;
					 size_t                 geoAnz;
					 int                    i;
           vector<GeometryError*> vErrors;
           GeometryError        * pErrorLoc;

           for ( i = 0; i < m_geometryErrorAnz; i++ )
           {
             int errorNumber = (int)m_errorNumbers[i];
             pErrorLoc = m_pErrorReport->getGeometryError( errorNumber );
             if ( pErrorLoc->getFeature() == m_pFeature )
               vErrors.push_back( pErrorLoc );
           }

           vFeatures.push_back( m_pFeature );
           if ( m_pFeatureRelated != NULL )
             vFeatures.push_back( m_pFeatureRelated );          

           if ( saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )
           {
             QuConvert::systemStr2stdStr( fileName, saveFileDialog->FileName );

             for ( i = 0; i < (int)vErrors.size(); i++ )
             {
               pErrorLoc = vErrors[i];
               geoAnz =  pErrorLoc->getErrorGeometryAnz();
               if ( geoAnz > 0 && pErrorLoc->getErrorGeometryCreated() == false )
               {
                 for ( unsigned int j = 0; j < geoAnz; j++ )
                   m_pFeature->addCityGMLGeometryRepresentationWithContext( pErrorLoc->getErrorGeometry( j ), GmlNamespaces::cityGml_2_0_georepNamespaceKuerzel,
                   "Geometriefehler", GeometryError::getGeometryErrorTypeAsString ( m_typeFehlerTyp ));

                 pErrorLoc->setErrorGeometryCreated();
               }
             }
             GMLWriter ^ pWriter = gcnew GMLWriter ( m_pFeatures );
             pWriter->setWriteXLinks ( false );
             pWriter->writeSingleFeatures ( fileName, 0,  vFeatures );
             delete pWriter;             
           }
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Zeigen                                             //
//  Generiert eine GML-Datei mit der fehlerhaften Geometrie sowie den        //
//  zugehörigen Features, und öffnet diese Datei mit dem generischen         //
//  GML-Viewer                                                               //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonShow_Click(System::Object^  sender, System::EventArgs^  e)
         {
           string                 fileName;
           vector<Feature*>       vFeatures;
					 size_t                 geoAnz;
					 int                    i;
           vector<GeometryError*> vErrors;
           GeometryError        * pErrorLoc;

           for ( i = 0; i < m_geometryErrorAnz; i++ )
           {
             int errorNumber = (int)m_errorNumbers[i];
             pErrorLoc = m_pErrorReport->getGeometryError( errorNumber );
             if ( pErrorLoc->getFeature() == m_pFeature )
               vErrors.push_back( pErrorLoc );
           }

           vFeatures.push_back( m_pFeature );
           if ( m_pFeatureRelated != NULL )
             vFeatures.push_back( m_pFeatureRelated );

           for ( i = 0; i < vErrors.size(); i++ )
           {
             pErrorLoc = vErrors[i];
             geoAnz =  pErrorLoc->getErrorGeometryAnz();
             if ( geoAnz > 0 && pErrorLoc->getErrorGeometryCreated() == false )
             {
               for ( unsigned int j = 0; j < geoAnz; j++ )
                 m_pFeature->addCityGMLGeometryRepresentationWithContext( pErrorLoc->getErrorGeometry( j ), GmlNamespaces::cityGml_2_0_georepNamespaceKuerzel,
                         "Geometriefehler", GeometryError::getGeometryErrorTypeAsString ( m_typeFehlerTyp ));
               pErrorLoc->setErrorGeometryCreated();
             }
           }

           fileName = Constant::getSystemFolder() + "TempObj.gml";

           GMLWriter ^ pWriter = gcnew GMLWriter ( m_pFeatures );
           pWriter->setWriteXLinks( false );
           pWriter->writeSingleFeatures ( fileName, 0,  vFeatures );
           delete pWriter; 

           try
           {
             System::Diagnostics::Process          ^ pProcess = gcnew System::Diagnostics::Process();
             System::Diagnostics::ProcessStartInfo ^ processStartInfo = gcnew System::Diagnostics::ProcessStartInfo;

             processStartInfo->FileName =  m_cityGMLViewer;
             processStartInfo->Arguments = String::Concat ( "\"", QuConvert::ToString ( fileName ), "\"" );
             pProcess->StartInfo = processStartInfo;
             pProcess->Start();
           }

           catch ( Exception ^ )
           {
             MessageBox::Show ( "GML-Viewer kann nicht gestartet werden" );
           }

         }
        
};
