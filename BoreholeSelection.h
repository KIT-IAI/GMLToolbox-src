#pragma once

#include "SrsManagerRef.h"
#include "Feature.h"
#include "Convert.h"
#include "constant.h"
#include "FeatureAttribute.h"
#include "BoreholeVisualisation2D.h"

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

  using namespace std;

	/// <summary>
	/// Auswahl eines Bohrloches und Anzeige der Parameter und der Schichten
	/// </summary>
	public ref class BoreholeSelection : public System::Windows::Forms::Form
	{
	public:
		BoreholeSelection( Features * pFeaturesP )
		{
			InitializeComponent();

      pFeatures = pFeaturesP;
//      svgViewer = svgViewerP;
      pBoreholeSelected = NULL;

      initialize();			
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~BoreholeSelection()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::ComboBox^  comboBoxBoreholeId;
  protected: 
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  textBoxBoreholeName;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::TextBox^  textBoxBoreholeShortName;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::Button^  buttonAttribute;
  private: System::Windows::Forms::GroupBox^  groupBoxVisualisierung;
  private: System::Windows::Forms::Button^  buttonVisualisierung;
  private: System::Windows::Forms::Button^  buttonAbbrechen;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::TextBox^  skalierungVertikal;
  private: System::Windows::Forms::CheckBox^  checkBoxSkale;
  private: System::Windows::Forms::CheckBox^  checkBoxStratigraphie;
  private: System::Windows::Forms::CheckBox^  checkBoxLithologie;
  private: System::Windows::Forms::TextBox^  stratigraphieDurchmesser;
  private: System::Windows::Forms::TextBox^  lithologieDurchmesser;
  private: System::Windows::Forms::CheckBox^  checkBoxLithologieAlleKomponenten;
  private: System::Windows::Forms::Label^  label6;
  private: System::Windows::Forms::Label^  label5;
  private: System::Windows::Forms::Label^  label7;
  private: System::Windows::Forms::TextBox^  textBoxSkalenabstand;

	private:
    Features											  * pFeatures;					  // Modul Features
    Feature												  * pBoreholeSelected;		// Aktuell gewähltes Borehole Feature  
    BoreholeDarstellung							* pBoreholeDarstellung; // Borehole Darstellungsparameter
//    String											    ^ svgViewer;						// SVG-Viewer
    System::Collections::SortedList ^ boreholeList;					// Liste aller Borehole Features

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
      this->comboBoxBoreholeId = (gcnew System::Windows::Forms::ComboBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBoxBoreholeName = (gcnew System::Windows::Forms::TextBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->textBoxBoreholeShortName = (gcnew System::Windows::Forms::TextBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->buttonAttribute = (gcnew System::Windows::Forms::Button());
      this->groupBoxVisualisierung = (gcnew System::Windows::Forms::GroupBox());
      this->checkBoxLithologieAlleKomponenten = (gcnew System::Windows::Forms::CheckBox());
      this->label6 = (gcnew System::Windows::Forms::Label());
      this->label5 = (gcnew System::Windows::Forms::Label());
      this->stratigraphieDurchmesser = (gcnew System::Windows::Forms::TextBox());
      this->lithologieDurchmesser = (gcnew System::Windows::Forms::TextBox());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->skalierungVertikal = (gcnew System::Windows::Forms::TextBox());
      this->checkBoxSkale = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxStratigraphie = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxLithologie = (gcnew System::Windows::Forms::CheckBox());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->buttonVisualisierung = (gcnew System::Windows::Forms::Button());
      this->textBoxSkalenabstand = (gcnew System::Windows::Forms::TextBox());
      this->label7 = (gcnew System::Windows::Forms::Label());
      this->groupBoxVisualisierung->SuspendLayout();
      this->SuspendLayout();
      // 
      // comboBoxBoreholeId
      // 
      this->comboBoxBoreholeId->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->comboBoxBoreholeId->FormattingEnabled = true;
      this->comboBoxBoreholeId->Location = System::Drawing::Point(13, 13);
      this->comboBoxBoreholeId->Name = L"comboBoxBoreholeId";
      this->comboBoxBoreholeId->Size = System::Drawing::Size(390, 21);
      this->comboBoxBoreholeId->TabIndex = 0;
      this->comboBoxBoreholeId->SelectedIndexChanged += gcnew System::EventHandler(this, &BoreholeSelection::comboBoxBoreholeId_SelectedIndexChanged);
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(12, 37);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(61, 13);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Borehole-Id";
      // 
      // textBoxBoreholeName
      // 
      this->textBoxBoreholeName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxBoreholeName->Location = System::Drawing::Point(13, 65);
      this->textBoxBoreholeName->Name = L"textBoxBoreholeName";
      this->textBoxBoreholeName->ReadOnly = true;
      this->textBoxBoreholeName->Size = System::Drawing::Size(281, 20);
      this->textBoxBoreholeName->TabIndex = 2;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(10, 88);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(35, 13);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Name";
      // 
      // textBoxBoreholeShortName
      // 
      this->textBoxBoreholeShortName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxBoreholeShortName->Location = System::Drawing::Point(303, 65);
      this->textBoxBoreholeShortName->Name = L"textBoxBoreholeShortName";
      this->textBoxBoreholeShortName->ReadOnly = true;
      this->textBoxBoreholeShortName->Size = System::Drawing::Size(100, 20);
      this->textBoxBoreholeShortName->TabIndex = 4;
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(271, 88);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(54, 13);
      this->label3->TabIndex = 5;
      this->label3->Text = L"Kurzname";
      // 
      // buttonAttribute
      // 
      this->buttonAttribute->Location = System::Drawing::Point(12, 114);
      this->buttonAttribute->Name = L"buttonAttribute";
      this->buttonAttribute->Size = System::Drawing::Size(75, 23);
      this->buttonAttribute->TabIndex = 6;
      this->buttonAttribute->Text = L"Alle Attribute";
      this->buttonAttribute->UseVisualStyleBackColor = true;
      this->buttonAttribute->Click += gcnew System::EventHandler(this, &BoreholeSelection::buttonAttribute_Click);
      // 
      // groupBoxVisualisierung
      // 
      this->groupBoxVisualisierung->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBoxVisualisierung->Controls->Add(this->label7);
      this->groupBoxVisualisierung->Controls->Add(this->textBoxSkalenabstand);
      this->groupBoxVisualisierung->Controls->Add(this->checkBoxLithologieAlleKomponenten);
      this->groupBoxVisualisierung->Controls->Add(this->label6);
      this->groupBoxVisualisierung->Controls->Add(this->label5);
      this->groupBoxVisualisierung->Controls->Add(this->stratigraphieDurchmesser);
      this->groupBoxVisualisierung->Controls->Add(this->lithologieDurchmesser);
      this->groupBoxVisualisierung->Controls->Add(this->label4);
      this->groupBoxVisualisierung->Controls->Add(this->skalierungVertikal);
      this->groupBoxVisualisierung->Controls->Add(this->checkBoxSkale);
      this->groupBoxVisualisierung->Controls->Add(this->checkBoxStratigraphie);
      this->groupBoxVisualisierung->Controls->Add(this->checkBoxLithologie);
      this->groupBoxVisualisierung->Controls->Add(this->buttonAbbrechen);
      this->groupBoxVisualisierung->Controls->Add(this->buttonVisualisierung);
      this->groupBoxVisualisierung->Location = System::Drawing::Point(12, 158);
      this->groupBoxVisualisierung->Name = L"groupBoxVisualisierung";
      this->groupBoxVisualisierung->Size = System::Drawing::Size(391, 197);
      this->groupBoxVisualisierung->TabIndex = 7;
      this->groupBoxVisualisierung->TabStop = false;
      this->groupBoxVisualisierung->Text = L"2D-Visualisierung";
      // 
      // checkBoxLithologieAlleKomponenten
      // 
      this->checkBoxLithologieAlleKomponenten->AutoSize = true;
      this->checkBoxLithologieAlleKomponenten->Location = System::Drawing::Point(244, 35);
      this->checkBoxLithologieAlleKomponenten->Name = L"checkBoxLithologieAlleKomponenten";
      this->checkBoxLithologieAlleKomponenten->Size = System::Drawing::Size(146, 17);
      this->checkBoxLithologieAlleKomponenten->TabIndex = 11;
      this->checkBoxLithologieAlleKomponenten->Text = L"Alle Komponenten zeigen";
      this->checkBoxLithologieAlleKomponenten->UseVisualStyleBackColor = true;
      // 
      // label6
      // 
      this->label6->AutoSize = true;
      this->label6->Location = System::Drawing::Point(168, 58);
      this->label6->Name = L"label6";
      this->label6->Size = System::Drawing::Size(69, 13);
      this->label6->TabIndex = 10;
      this->label6->Text = L"Durchmesser";
      // 
      // label5
      // 
      this->label5->AutoSize = true;
      this->label5->Location = System::Drawing::Point(168, 35);
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size(69, 13);
      this->label5->TabIndex = 9;
      this->label5->Text = L"Durchmesser";
      // 
      // stratigraphieDurchmesser
      // 
      this->stratigraphieDurchmesser->Location = System::Drawing::Point(108, 54);
      this->stratigraphieDurchmesser->Name = L"stratigraphieDurchmesser";
      this->stratigraphieDurchmesser->Size = System::Drawing::Size(53, 20);
      this->stratigraphieDurchmesser->TabIndex = 8;
      // 
      // lithologieDurchmesser
      // 
      this->lithologieDurchmesser->Location = System::Drawing::Point(108, 29);
      this->lithologieDurchmesser->Name = L"lithologieDurchmesser";
      this->lithologieDurchmesser->Size = System::Drawing::Size(53, 20);
      this->lithologieDurchmesser->TabIndex = 7;
      // 
      // label4
      // 
      this->label4->AutoSize = true;
      this->label4->Location = System::Drawing::Point(168, 81);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(101, 13);
      this->label4->TabIndex = 6;
      this->label4->Text = L"Vertikale Skalierung";
      // 
      // skalierungVertikal
      // 
      this->skalierungVertikal->Location = System::Drawing::Point(108, 79);
      this->skalierungVertikal->Name = L"skalierungVertikal";
      this->skalierungVertikal->Size = System::Drawing::Size(53, 20);
      this->skalierungVertikal->TabIndex = 5;
      // 
      // checkBoxSkale
      // 
      this->checkBoxSkale->AutoSize = true;
      this->checkBoxSkale->Location = System::Drawing::Point(7, 76);
      this->checkBoxSkale->Name = L"checkBoxSkale";
      this->checkBoxSkale->Size = System::Drawing::Size(81, 17);
      this->checkBoxSkale->TabIndex = 4;
      this->checkBoxSkale->Text = L"Tiefenskala";
      this->checkBoxSkale->UseVisualStyleBackColor = true;
      // 
      // checkBoxStratigraphie
      // 
      this->checkBoxStratigraphie->AutoSize = true;
      this->checkBoxStratigraphie->Location = System::Drawing::Point(7, 52);
      this->checkBoxStratigraphie->Name = L"checkBoxStratigraphie";
      this->checkBoxStratigraphie->Size = System::Drawing::Size(85, 17);
      this->checkBoxStratigraphie->TabIndex = 3;
      this->checkBoxStratigraphie->Text = L"Stratigraphie";
      this->checkBoxStratigraphie->UseVisualStyleBackColor = true;
      // 
      // checkBoxLithologie
      // 
      this->checkBoxLithologie->AutoSize = true;
      this->checkBoxLithologie->Location = System::Drawing::Point(7, 29);
      this->checkBoxLithologie->Name = L"checkBoxLithologie";
      this->checkBoxLithologie->Size = System::Drawing::Size(71, 17);
      this->checkBoxLithologie->TabIndex = 2;
      this->checkBoxLithologie->Text = L"Lithologie";
      this->checkBoxLithologie->UseVisualStyleBackColor = true;
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonAbbrechen->Location = System::Drawing::Point(1, 155);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 23);
      this->buttonAbbrechen->TabIndex = 1;
      this->buttonAbbrechen->Text = L"Beenden";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &BoreholeSelection::buttonAbbrechen_Click);
      // 
      // buttonVisualisierung
      // 
      this->buttonVisualisierung->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonVisualisierung->Location = System::Drawing::Point(310, 155);
      this->buttonVisualisierung->Name = L"buttonVisualisierung";
      this->buttonVisualisierung->Size = System::Drawing::Size(75, 23);
      this->buttonVisualisierung->TabIndex = 0;
      this->buttonVisualisierung->Text = L"Zeigen";
      this->buttonVisualisierung->UseVisualStyleBackColor = true;
      this->buttonVisualisierung->Click += gcnew System::EventHandler(this, &BoreholeSelection::buttonVisualisierung_Click);
      // 
      // textBoxSkalenabstand
      // 
      this->textBoxSkalenabstand->Location = System::Drawing::Point(108, 104);
      this->textBoxSkalenabstand->Name = L"textBoxSkalenabstand";
      this->textBoxSkalenabstand->Size = System::Drawing::Size(53, 20);
      this->textBoxSkalenabstand->TabIndex = 12;
      // 
      // label7
      // 
      this->label7->AutoSize = true;
      this->label7->Location = System::Drawing::Point(171, 104);
      this->label7->Name = L"label7";
      this->label7->Size = System::Drawing::Size(66, 13);
      this->label7->TabIndex = 13;
      this->label7->Text = L"Beschriftung";
      // 
      // BoreholeSelection
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(415, 377);
      this->Controls->Add(this->groupBoxVisualisierung);
      this->Controls->Add(this->buttonAttribute);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->textBoxBoreholeShortName);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->textBoxBoreholeName);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->comboBoxBoreholeId);
      this->Name = L"BoreholeSelection";
      this->Text = L"Bohrloch Auswahl";
      this->groupBoxVisualisierung->ResumeLayout(false);
      this->groupBoxVisualisierung->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
	
///////////////////////////////////////////////////////////////////////////
//  Initialisiert die Dialogbox im Konstruktor													 //
///////////////////////////////////////////////////////////////////////////
		void initialize()
    {
      vector<Feature*> vFeatures;
			size_t           i, anz;
      Feature        * pBorehole;
      string           boreholeId;

      boreholeList = gcnew System::Collections::SortedList;

      bool success = getBoreholeVisualisationParams();
      if ( !success )
      {
        textBoxBoreholeName->Text = "Einlesen der Darstellungsparameter ist nicht möglich";
        return;
      }

      checkBoxSkale->Checked         = pBoreholeDarstellung->getShowSkala();
      checkBoxLithologie->Checked    = pBoreholeDarstellung->getShowLithology();
      checkBoxStratigraphie->Checked = pBoreholeDarstellung->getShowStratigraphy();

      lithologieDurchmesser->Text    = QuConvert::ToString( pBoreholeDarstellung->getBoreholeDurchmesserLithologie() );
      stratigraphieDurchmesser->Text = QuConvert::ToString( pBoreholeDarstellung->getBoreholeDurchmesserLithologie() );
      skalierungVertikal->Text       = QuConvert::ToString( pBoreholeDarstellung->getVertikaleSkalierung() );
      textBoxSkalenabstand->Text     =  QuConvert::ToString( pBoreholeDarstellung->getAbstandSkalenwerte() );

      if ( pBoreholeDarstellung->getLithologyVisualisationStrategy() == BoreholeDarstellung::ALL_COMPONENTS )
        checkBoxLithologieAlleKomponenten->Checked = true;
      else
        checkBoxLithologieAlleKomponenten->Checked = false;

      anz = pFeatures->getFeatures( vFeatures, "bml:Borehole" );
      for ( i = 0; i < vFeatures.size(); i++ )
      {
        pBorehole = vFeatures[i];

        bool success = pBorehole->getStringAttributWert( "bml:id", boreholeId );
        if ( success )
        {
          String ^ STR =  QuConvert::ToString( boreholeId );
          if ( !boreholeList->Contains( STR ) )
          {
            comboBoxBoreholeId->Items->Add ( STR );
            string gmlId = pBorehole->getGmlId();
           
            boreholeList->Add( STR, QuConvert::ToString( gmlId ) );
          }
        }
      }
      if ( boreholeList->Count > 0 )
        comboBoxBoreholeId->SelectedIndex = 0;
    }


//////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Alle Attribute"														  //
//  Anzeige aller Attribute des aktuell gewählten Bohrlochs mit dem     //
//  Feature-Attribute Dialog																					  //
///////////////////////////////////////////////////////////////////////////
private: System::Void buttonAttribute_Click(System::Object^  sender, System::EventArgs^  e)
{
  if ( pBoreholeSelected == NULL )
    return; 

  ArrayList        ^ highlight = gcnew ArrayList;
  FeatureAttribute ^ pDialog   = gcnew FeatureAttribute ( pFeatures, pBoreholeSelected, highlight );
  pDialog->ShowDialog();
}

//////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"																			//
//  Schließt den Dialog																								  //
//////////////////////////////////////////////////////////////////////////
private: System::Void buttonAbbrechen_Click(System::Object^  sender, System::EventArgs^  e) 
{
  Close();
}

//////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Zeigen"																			//
//  Startet die Visualisierung der Schichten mit dem SVG-Viewer					//
//////////////////////////////////////////////////////////////////////////
private: System::Void buttonVisualisierung_Click(System::Object^  sender, System::EventArgs^  e) 
{ 
  System::Diagnostics::ProcessStartInfo ^ processStartInfo;
  System::Diagnostics::Process          ^ htmlProcess;

  if ( pBoreholeSelected == NULL )
    return;

  pBoreholeDarstellung->setShowSkala( checkBoxSkale->Checked );
  pBoreholeDarstellung->setShowLithology(checkBoxLithologie->Checked);
  pBoreholeDarstellung->setShowStratigraphy( checkBoxStratigraphie->Checked );

  pBoreholeDarstellung->setBoreholeDurchmesserLithologie( XmlConvert::ToDouble( lithologieDurchmesser->Text ) );
  pBoreholeDarstellung->setBoreholeDurchmesserStratigraphie( XmlConvert::ToDouble( stratigraphieDurchmesser->Text ) );
  pBoreholeDarstellung->setVertikaleSkalierung(  XmlConvert::ToDouble( skalierungVertikal->Text ) );
  pBoreholeDarstellung->setAbstandSkalenwerte( XmlConvert::ToDouble (textBoxSkalenabstand->Text ) );

  if ( checkBoxLithologieAlleKomponenten->Checked == true )
    pBoreholeDarstellung->setLithologyVisualisationStrategy( BoreholeDarstellung::ALL_COMPONENTS );
  else
    pBoreholeDarstellung->setLithologyVisualisationStrategy( BoreholeDarstellung::MAJOR_COMPONENT );

  BoreholeVisualisation2D * pBoreholeVisualisation = new BoreholeVisualisation2D ( pBoreholeSelected, pBoreholeDarstellung );
  pBoreholeVisualisation->show();

  String ^ kartenDatei = QuConvert::ToString(  Constant::getHTMLFramePath ( pFeatures->getGmlTyp() ) );

  htmlProcess = gcnew System::Diagnostics::Process();
  processStartInfo = gcnew System::Diagnostics::ProcessStartInfo;

  //processStartInfo->FileName = svgViewer;
  processStartInfo->FileName = kartenDatei;
  htmlProcess->StartInfo = processStartInfo;
  htmlProcess->Start();
}


//////////////////////////////////////////////////////////////////////////
//  Commandhandler Answahl eines Bohrloches über die ComboBox				    //
//  "Borehole-Id"																												//
//////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxBoreholeId_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
  string                   gmlId;
  String                 ^ id = dynamic_cast<String^>(comboBoxBoreholeId->SelectedItem);
	size_t                   anz;
  vector<FeatureRelation*> relationen;
  Feature                * pRelationZiel;
  StringAttr             * pSimpleContent;



  int index = boreholeList->IndexOfKey( id );
  if ( index >= 0 )
  {
    String ^ GML_ID = dynamic_cast<String^>(boreholeList->GetByIndex( index ) );
    QuConvert::systemStr2stdStr( gmlId, GML_ID );
    pBoreholeSelected =  pFeatures->getFeature( gmlId );
  }
  else
    pBoreholeSelected = NULL;

  textBoxBoreholeName->Text = "";
  textBoxBoreholeShortName->Text = "";

  if ( pBoreholeSelected != NULL )
  {
    anz = pBoreholeSelected->getFeatureRelationen ( "bml:fullName", relationen );
    if ( anz > 0 )
    {
      pRelationZiel = pFeatures->getFeature ( relationen[0]->featureId );
      if ( pRelationZiel != NULL )
      {
        pSimpleContent = pRelationZiel->getSimpleContent();
        if ( pSimpleContent != NULL )
          textBoxBoreholeName->Text = QuConvert::ToString ( pSimpleContent->value );
      }
    }

    relationen.clear();

    anz = pBoreholeSelected->getFeatureRelationen ( "bml:shortName", relationen );
    if ( anz > 0 )
    {
      pRelationZiel = pFeatures->getFeature ( relationen[0]->featureId );
      if ( pRelationZiel != NULL )
      {
        pSimpleContent = pRelationZiel->getSimpleContent();
        if ( pSimpleContent != NULL )
          textBoxBoreholeShortName->Text = QuConvert::ToString ( pSimpleContent->value );
      }
    }

  }
}

//////////////////////////////////////////////////////////////////////////
//  Einlesen der Darstellungsparameter für BoreholeML 3.0               //
//////////////////////////////////////////////////////////////////////////
private: bool getBoreholeVisualisationParams() 
{
  string systemFolder = Constant::getSystemFolder();
  string schemaFolder = Constant::getSchemaPath( BOREHOLEML_3_0 );
  string stylesheet   = schemaFolder + "\\Stylesheets\\VisualParams2D.xml";

  pBoreholeDarstellung = new BoreholeDarstellung();

  BoreholeDarstellungReader ^ pParamReader = gcnew BoreholeDarstellungReader ();
  bool success = pParamReader->read( stylesheet, pBoreholeDarstellung );
  return success;
}

};
