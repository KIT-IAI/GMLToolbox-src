#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "XPlanGML.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "GmlSchema.h"
#include "Dictionary.h"
#include "ShpFileObjekte.h"
#include "Convert.h"
#include "RasterplanBasisDialog.h"
#include "RasterplanBasisV2Dialog.h"
#include "includes.h"

using namespace std;

	/// <summary> 
	/// Festlegung der Attribute für einen Plan-Bereich
	///
	/// </summary>
	public ref class PlanBereiche : public System::Windows::Forms::Form
	{
	public: 
		PlanBereiche( XPlanGML * pXPlanGML, GmlDictionary * pCodeListsP )
		{
			InitializeComponent();

      m_pXPlanGML         = pXPlanGML;
			m_pGmlSchema        = m_pXPlanGML->getFeatures()->getGmlSchema();
      m_pShapeFileObjekte = m_pXPlanGML->getShapeFileObjekte();
      m_pPlanAttribute    = m_pXPlanGML->getPlanAttribute();
			m_pCodeLists        = pCodeListsP;

      if ( m_pGmlSchema->getGmlTyp() == XPLANGML_5_1 ||  
           m_pGmlSchema->getGmlTyp() == XPLANGML_5_2 ||
           m_pGmlSchema->getGmlTyp() == XPLANGML_5_3 )
      {
        labelGesetzADatum->Visible = false;
        labelGesetzAText->Visible = false;
        labelGesetzBDatum->Visible = false;
        labelGesetzBText->Visible = false;

        textBoxGesetzADatum->Visible = false;
        textBoxGesetzAText->Visible = false;
        textBoxGesetzBDatum->Visible = false;
        textBoxGesetzBText->Visible = false;

        comboBoxVersionBauNVO->Visible = false;
      }

			initialize();
		}
        
	private: System::Windows::Forms::ComboBox ^   comboBoxBereiche;
	private: System::Windows::Forms::Label ^  label1;
	private: System::Windows::Forms::TextBox ^  textBoxBereichName;
	private: System::Windows::Forms::Label ^  label2;
	private: System::Windows::Forms::TextBox ^  textBoxBereichBedeutung;
	private: System::Windows::Forms::Label ^  label3;
	private: System::Windows::Forms::Label ^  label4;
	private: System::Windows::Forms::ComboBox ^  comboBoxShapeFiles;
	private: System::Windows::Forms::Button ^  buttonNeuerBereich;
	private: System::Windows::Forms::Button ^  buttonBeenden;
	private: System::Windows::Forms::ComboBox ^  comboBoxBedeutung;
	private: System::Windows::Forms::Label ^  label5;
	private: System::Windows::Forms::TextBox ^  massstab;
	private: System::Windows::Forms::Label ^  label6;
	private: System::Windows::Forms::Label ^  label7;
	private: System::Windows::Forms::Button ^  buttonRasterplanBasis;
  private: System::Windows::Forms::TextBox^  textBoxGesetzAText;
  private: System::Windows::Forms::Label^  labelGesetzAText;
  private: System::Windows::Forms::ComboBox^  comboBoxVersionBauNVO;
  private: System::Windows::Forms::Label^  labelGesetzADatum;
  private: System::Windows::Forms::TextBox^  textBoxGesetzBText;
  private: System::Windows::Forms::Label^  labelGesetzBText;
  private: System::Windows::Forms::Label^  labelGesetzBDatum;
  private: System::Windows::Forms::TextBox^  textBoxGesetzADatum;
  private: System::Windows::Forms::TextBox^  textBoxGesetzBDatum;

	private:
    XPlanGML         * m_pXPlanGML;						// XPlanGML-Modul
		GmlSchema        * m_pGmlSchema;					// XPlanGML-Schema
		GmlDictionary    * m_pCodeLists;					// Dictionary der Enumerationen
		ShapeFileObjekte * m_pShapeFileObjekte;		// Interne Repräsentation der Shapefiles
		PlanAttribute    * m_pPlanAttribute;			// Attributwerte des Gesamtplans
		int                bereichSelected;				// Nummer des aktuell bearbeiteten Plan-Bereichs
		GmlKlasse        * pPlanBereichKlasse;		// Interne Repräsentation der Plan-Bereich GML-Klasse
    int                planTyp;								// Plan-Tyü

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
      this->comboBoxBereiche = ( gcnew System::Windows::Forms::ComboBox() );
      this->label1 = ( gcnew System::Windows::Forms::Label() );
      this->textBoxBereichName = ( gcnew System::Windows::Forms::TextBox() );
      this->label2 = ( gcnew System::Windows::Forms::Label() );
      this->textBoxBereichBedeutung = ( gcnew System::Windows::Forms::TextBox() );
      this->label3 = ( gcnew System::Windows::Forms::Label() );
      this->comboBoxShapeFiles = ( gcnew System::Windows::Forms::ComboBox() );
      this->label4 = ( gcnew System::Windows::Forms::Label() );
      this->buttonNeuerBereich = ( gcnew System::Windows::Forms::Button() );
      this->buttonBeenden = ( gcnew System::Windows::Forms::Button() );
      this->comboBoxBedeutung = ( gcnew System::Windows::Forms::ComboBox() );
      this->label5 = ( gcnew System::Windows::Forms::Label() );
      this->massstab = ( gcnew System::Windows::Forms::TextBox() );
      this->label6 = ( gcnew System::Windows::Forms::Label() );
      this->label7 = ( gcnew System::Windows::Forms::Label() );
      this->buttonRasterplanBasis = ( gcnew System::Windows::Forms::Button() );
      this->textBoxGesetzAText = ( gcnew System::Windows::Forms::TextBox() );
      this->labelGesetzAText = ( gcnew System::Windows::Forms::Label() );
      this->comboBoxVersionBauNVO = ( gcnew System::Windows::Forms::ComboBox() );
      this->labelGesetzADatum = ( gcnew System::Windows::Forms::Label() );
      this->textBoxGesetzBText = ( gcnew System::Windows::Forms::TextBox() );
      this->labelGesetzBText = ( gcnew System::Windows::Forms::Label() );
      this->labelGesetzBDatum = ( gcnew System::Windows::Forms::Label() );
      this->textBoxGesetzADatum = ( gcnew System::Windows::Forms::TextBox() );
      this->textBoxGesetzBDatum = ( gcnew System::Windows::Forms::TextBox() );
      this->SuspendLayout();
      // 
      // comboBoxBereiche
      // 
      this->comboBoxBereiche->Location = System::Drawing::Point( 8, 16 );
      this->comboBoxBereiche->Name = L"comboBoxBereiche";
      this->comboBoxBereiche->Size = System::Drawing::Size( 80, 21 );
      this->comboBoxBereiche->TabIndex = 0;
      this->comboBoxBereiche->SelectedIndexChanged += gcnew System::EventHandler( this, &PlanBereiche::comboBoxBereiche_SelectedIndexChanged );
      // 
      // label1
      // 
      this->label1->Location = System::Drawing::Point( 8, 40 );
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size( 100, 16 );
      this->label1->TabIndex = 1;
      this->label1->Text = L"Bereich-Nummer";
      // 
      // textBoxBereichName
      // 
      this->textBoxBereichName->Location = System::Drawing::Point( 8, 64 );
      this->textBoxBereichName->Name = L"textBoxBereichName";
      this->textBoxBereichName->Size = System::Drawing::Size( 168, 20 );
      this->textBoxBereichName->TabIndex = 2;
      this->textBoxBereichName->Leave += gcnew System::EventHandler( this, &PlanBereiche::textBoxBereichName_Leave );
      // 
      // label2
      // 
      this->label2->Location = System::Drawing::Point( 8, 88 );
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size( 100, 16 );
      this->label2->TabIndex = 3;
      this->label2->Text = L"Name";
      // 
      // textBoxBereichBedeutung
      // 
      this->textBoxBereichBedeutung->Location = System::Drawing::Point( 8, 152 );
      this->textBoxBereichBedeutung->Name = L"textBoxBereichBedeutung";
      this->textBoxBereichBedeutung->Size = System::Drawing::Size( 336, 20 );
      this->textBoxBereichBedeutung->TabIndex = 4;
      this->textBoxBereichBedeutung->Leave += gcnew System::EventHandler( this, &PlanBereiche::textBoxBereichBedeutung_Leave );
      // 
      // label3
      // 
      this->label3->Location = System::Drawing::Point( 8, 176 );
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size( 176, 23 );
      this->label3->TabIndex = 5;
      this->label3->Text = L"Sonstige Bedeutung";
      // 
      // comboBoxShapeFiles
      // 
      this->comboBoxShapeFiles->Location = System::Drawing::Point( 104, 16 );
      this->comboBoxShapeFiles->Name = L"comboBoxShapeFiles";
      this->comboBoxShapeFiles->Size = System::Drawing::Size( 240, 21 );
      this->comboBoxShapeFiles->TabIndex = 6;
      this->comboBoxShapeFiles->SelectedIndexChanged += gcnew System::EventHandler( this, &PlanBereiche::comboBoxShapeFiles_SelectedIndexChanged );
      // 
      // label4
      // 
      this->label4->Location = System::Drawing::Point( 104, 40 );
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size( 216, 23 );
      this->label4->TabIndex = 7;
      this->label4->Text = L"Shapefile Geltungsbereich";
      // 
      // buttonNeuerBereich
      // 
      this->buttonNeuerBereich->Location = System::Drawing::Point( 8, 481 );
      this->buttonNeuerBereich->Name = L"buttonNeuerBereich";
      this->buttonNeuerBereich->Size = System::Drawing::Size( 160, 23 );
      this->buttonNeuerBereich->TabIndex = 8;
      this->buttonNeuerBereich->Text = L"Neuer Bereich";
      this->buttonNeuerBereich->Click += gcnew System::EventHandler( this, &PlanBereiche::buttonNeuerBereich_Click );
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonBeenden->Location = System::Drawing::Point( 265, 475 );
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size( 75, 29 );
      this->buttonBeenden->TabIndex = 10;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->Click += gcnew System::EventHandler( this, &PlanBereiche::buttonBeenden_Click );
      // 
      // comboBoxBedeutung
      // 
      this->comboBoxBedeutung->Location = System::Drawing::Point( 8, 104 );
      this->comboBoxBedeutung->Name = L"comboBoxBedeutung";
      this->comboBoxBedeutung->Size = System::Drawing::Size( 336, 21 );
      this->comboBoxBedeutung->TabIndex = 11;
      this->comboBoxBedeutung->SelectedIndexChanged += gcnew System::EventHandler( this, &PlanBereiche::comboBoxBedeutung_SelectedIndexChanged );
      // 
      // label5
      // 
      this->label5->Location = System::Drawing::Point( 8, 128 );
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size( 136, 23 );
      this->label5->TabIndex = 12;
      this->label5->Text = L"Bedeutung";
      // 
      // massstab
      // 
      this->massstab->Location = System::Drawing::Point( 208, 64 );
      this->massstab->Name = L"massstab";
      this->massstab->Size = System::Drawing::Size( 132, 20 );
      this->massstab->TabIndex = 13;
      this->massstab->Text = L"textBox1";
      this->massstab->Leave += gcnew System::EventHandler( this, &PlanBereiche::massstab_Leave );
      // 
      // label6
      // 
      this->label6->Location = System::Drawing::Point( 184, 63 );
      this->label6->Name = L"label6";
      this->label6->Size = System::Drawing::Size( 24, 23 );
      this->label6->TabIndex = 14;
      this->label6->Text = L"1 :";
      this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
      // 
      // label7
      // 
      this->label7->Location = System::Drawing::Point( 184, 88 );
      this->label7->Name = L"label7";
      this->label7->Size = System::Drawing::Size( 100, 16 );
      this->label7->TabIndex = 15;
      this->label7->Text = L"Massstab";
      // 
      // buttonRasterplanBasis
      // 
      this->buttonRasterplanBasis->Location = System::Drawing::Point( 8, 437 );
      this->buttonRasterplanBasis->Name = L"buttonRasterplanBasis";
      this->buttonRasterplanBasis->Size = System::Drawing::Size( 160, 23 );
      this->buttonRasterplanBasis->TabIndex = 16;
      this->buttonRasterplanBasis->Text = L"Rasterdarstellung";
      this->buttonRasterplanBasis->Click += gcnew System::EventHandler( this, &PlanBereiche::buttonRasterplanBasis_Click );
      // 
      // textBoxGesetzAText
      // 
      this->textBoxGesetzAText->Location = System::Drawing::Point( 8, 201 );
      this->textBoxGesetzAText->Name = L"textBoxGesetzAText";
      this->textBoxGesetzAText->Size = System::Drawing::Size( 336, 20 );
      this->textBoxGesetzAText->TabIndex = 17;
      this->textBoxGesetzAText->Leave += gcnew System::EventHandler( this, &PlanBereiche::textBoxBauNVO_Leave );
      // 
      // labelGesetzAText
      // 
      this->labelGesetzAText->Location = System::Drawing::Point( 8, 224 );
      this->labelGesetzAText->Name = L"labelGesetzAText";
      this->labelGesetzAText->Size = System::Drawing::Size( 224, 23 );
      this->labelGesetzAText->TabIndex = 18;
      this->labelGesetzAText->Text = L"Version der BauNVO (Textform)";
      // 
      // comboBoxVersionBauNVO
      // 
      this->comboBoxVersionBauNVO->FormattingEnabled = true;
      this->comboBoxVersionBauNVO->Location = System::Drawing::Point( 8, 250 );
      this->comboBoxVersionBauNVO->Name = L"comboBoxVersionBauNVO";
      this->comboBoxVersionBauNVO->Size = System::Drawing::Size( 336, 21 );
      this->comboBoxVersionBauNVO->TabIndex = 19;
      this->comboBoxVersionBauNVO->SelectedIndexChanged += gcnew System::EventHandler( this, &PlanBereiche::comboBoxVersionBauNVO_SelectedIndexChanged );
      // 
      // labelGesetzADatum
      // 
      this->labelGesetzADatum->AutoSize = true;
      this->labelGesetzADatum->Location = System::Drawing::Point( 5, 275 );
      this->labelGesetzADatum->Name = L"labelGesetzADatum";
      this->labelGesetzADatum->Size = System::Drawing::Size( 105, 13 );
      this->labelGesetzADatum->TabIndex = 20;
      this->labelGesetzADatum->Text = L"Version der BauNVO";
      // 
      // textBoxGesetzBText
      // 
      this->textBoxGesetzBText->Location = System::Drawing::Point( 8, 301 );
      this->textBoxGesetzBText->Name = L"textBoxGesetzBText";
      this->textBoxGesetzBText->Size = System::Drawing::Size( 336, 20 );
      this->textBoxGesetzBText->TabIndex = 21;
      this->textBoxGesetzBText->Leave += gcnew System::EventHandler( this, &PlanBereiche::textBoxGesetzBText_Leave );
      // 
      // labelGesetzBText
      // 
      this->labelGesetzBText->AutoSize = true;
      this->labelGesetzBText->Location = System::Drawing::Point( 8, 328 );
      this->labelGesetzBText->Name = L"labelGesetzBText";
      this->labelGesetzBText->Size = System::Drawing::Size( 102, 13 );
      this->labelGesetzBText->TabIndex = 22;
      this->labelGesetzBText->Text = L"Version des BauGB ";
      // 
      // labelGesetzBDatum
      // 
      this->labelGesetzBDatum->AutoSize = true;
      this->labelGesetzBDatum->Location = System::Drawing::Point( 8, 378 );
      this->labelGesetzBDatum->Name = L"labelGesetzBDatum";
      this->labelGesetzBDatum->Size = System::Drawing::Size( 151, 13 );
      this->labelGesetzBDatum->TabIndex = 24;
      this->labelGesetzBDatum->Text = L"Datum der Version des BauGB";
      // 
      // textBoxGesetzADatum
      // 
      this->textBoxGesetzADatum->Location = System::Drawing::Point( 8, 248 );
      this->textBoxGesetzADatum->Name = L"textBoxGesetzADatum";
      this->textBoxGesetzADatum->Size = System::Drawing::Size( 336, 20 );
      this->textBoxGesetzADatum->TabIndex = 25;
      this->textBoxGesetzADatum->Leave += gcnew System::EventHandler( this, &PlanBereiche::textBoxGesetzADatum_Leave );
      // 
      // textBoxGesetzBDatum
      // 
      this->textBoxGesetzBDatum->Location = System::Drawing::Point( 8, 355 );
      this->textBoxGesetzBDatum->Name = L"textBoxGesetzBDatum";
      this->textBoxGesetzBDatum->Size = System::Drawing::Size( 336, 20 );
      this->textBoxGesetzBDatum->TabIndex = 26;
      this->textBoxGesetzBDatum->Leave += gcnew System::EventHandler( this, &PlanBereiche::textBoxGesetzBDatum_Leave );
      // 
      // PlanBereiche
      // 
      this->AutoScaleBaseSize = System::Drawing::Size( 5, 13 );
      this->CancelButton = this->buttonBeenden;
      this->ClientSize = System::Drawing::Size( 352, 516 );
      this->Controls->Add( this->textBoxGesetzBDatum );
      this->Controls->Add( this->textBoxGesetzADatum );
      this->Controls->Add( this->labelGesetzBDatum );
      this->Controls->Add( this->labelGesetzBText );
      this->Controls->Add( this->textBoxGesetzBText );
      this->Controls->Add( this->labelGesetzADatum );
      this->Controls->Add( this->comboBoxVersionBauNVO );
      this->Controls->Add( this->labelGesetzAText );
      this->Controls->Add( this->textBoxGesetzAText );
      this->Controls->Add( this->buttonRasterplanBasis );
      this->Controls->Add( this->label7 );
      this->Controls->Add( this->label6 );
      this->Controls->Add( this->massstab );
      this->Controls->Add( this->textBoxBereichBedeutung );
      this->Controls->Add( this->textBoxBereichName );
      this->Controls->Add( this->label5 );
      this->Controls->Add( this->comboBoxBedeutung );
      this->Controls->Add( this->buttonBeenden );
      this->Controls->Add( this->buttonNeuerBereich );
      this->Controls->Add( this->label4 );
      this->Controls->Add( this->comboBoxShapeFiles );
      this->Controls->Add( this->label3 );
      this->Controls->Add( this->label2 );
      this->Controls->Add( this->label1 );
      this->Controls->Add( this->comboBoxBereiche );
      this->Name = L"PlanBereiche";
      this->Text = L"PlanBereiche";
      this->ResumeLayout( false );
      this->PerformLayout();

    }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl eines Bereichs über die ComboBox Bereich-Nummer  //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxBereiche_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	string               stdString;
	String             ^ STR;
	bool                 success;
  GmlDictionaryEntry * pDictionaryEntry;

	bereichSelected = comboBoxBereiche->SelectedIndex;
	Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );
	if ( pBereich == NULL ) return;

	STR = QuConvert::ToString ( pBereich->name );
	textBoxBereichName->Text = STR;

	STR = QuConvert::ToString ( pBereich->bedeutung );
	textBoxBereichBedeutung->Text = STR;

	if ( pBereich->geometrieDatei == "" )
		comboBoxShapeFiles->SelectedIndex = -1;
	else
	{
		STR = QuConvert::ToString ( pBereich->geometrieDatei );
		comboBoxShapeFiles->SelectedItem = STR;
	}

  if ( m_pGmlSchema->getGmlTyp() >=  XPLANGML_3_0 )
	{
		pDictionaryEntry = m_pCodeLists->getDictionaryEntry ( "xplan:XP_BedeutungenBereich" );

		success = pDictionaryEntry->getSchluesselText ( pBereich->bedeutungEnum, stdString );
		if ( success )
		{
			STR = QuConvert::ToString ( stdString );
			comboBoxBedeutung->SelectedItem = STR;
		}
		else
			comboBoxBedeutung->SelectedIndex = -1;

		STR = QuConvert::ToString ( pBereich->massstab );
		if ( STR != "-1" )
			massstab->Text = STR;
		else
			massstab->Text = nullptr;

    if ( m_pGmlSchema->getGmlTyp() ==  XPLANGML_3_0 )
    {
		  STR = QuConvert::ToString ( pBereich->versionBauNVOText );
      textBoxGesetzAText->Text = STR;
    }
    else
    if ( m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_0 || m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_1  || 
         m_pGmlSchema->getGmlTyp() ==  XPLANGML_5_0 || m_pGmlSchema->getGmlTyp() ==  XPLANGML_5_1  ||
         m_pGmlSchema->getGmlTyp() ==  XPLANGML_5_2 || m_pGmlSchema->getGmlTyp() == XPLANGML_5_3  )
    {
      if ( planTyp == BPLAN || planTyp == FPLAN )
      {
        STR = QuConvert::ToString ( pBereich->versionBauNVOText );
        textBoxGesetzAText->Text = STR;

        if ( m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_0 || m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_1 )
        {
          pDictionaryEntry = m_pCodeLists->getDictionaryEntry ( "xplan:XP_VersionBauNVO" );
          success = pDictionaryEntry->getSchluesselText ( pBereich->versionBauNVO, stdString );

          if ( success )
          {
            STR = QuConvert::ToString ( stdString );
            comboBoxVersionBauNVO->SelectedItem = STR;
          }
          else
            comboBoxVersionBauNVO->SelectedIndex = -1;
        }
        else
        {
          STR = QuConvert::ToString ( pBereich->versionBauNVO );
          textBoxGesetzADatum->Text = STR;
        }

        STR = QuConvert::ToString ( pBereich->versionBauGBText );
        textBoxGesetzBText->Text = STR;

        STR = QuConvert::ToString ( pBereich->versionBauGBDatum );
        textBoxGesetzBDatum->Text = STR;
      }
      else
      if ( planTyp == RPLAN )
      {
        STR = QuConvert::ToString ( pBereich->versionBROGText );
        textBoxGesetzAText->Text = STR;

        STR = QuConvert::ToString ( pBereich->versionBROGDatum );
        textBoxGesetzADatum->Text = STR;

        STR = QuConvert::ToString ( pBereich->versionLPLGText );
        textBoxGesetzBText->Text = STR;

        STR = QuConvert::ToString ( pBereich->versionLPLGDatum );
        textBoxGesetzBDatum->Text = STR;
      }
    }
	}

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl eines Shapefiles für den Geltungsbereich         //
//  ComboBox Shapefile Geltungsbereich																			 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxShapeFiles_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	String	      			    ^ STR;
	string                    stdString;
	size_t      						  i, anz;
	vector<ShapeFileObjekt*>  shapeFileObjekte;
	ShapeFileObjekt         * pShapeFileObjektGeltungsbereich;

	STR =  dynamic_cast<String^> ( comboBoxShapeFiles->SelectedItem );
	if ( STR == nullptr ) return;

	QuConvert::systemStr2stdStr ( stdString, STR );
	pShapeFileObjektGeltungsbereich = m_pShapeFileObjekte->getShapeFileObjekt ( stdString );

	Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );
	pBereich->geometrieDatei = stdString;

	anz = m_pShapeFileObjekte->getShapeFileObjekte ( shapeFileObjekte );
	for ( i = 0; i < anz; i++ )
	{
		ShapeFileObjekt * pShapeFileObjekt = shapeFileObjekte[i];
		if ( pShapeFileObjekt->getGmlKlasse() == pPlanBereichKlasse &&
			pShapeFileObjekt->getBereichNummer() == bereichSelected )
		{
			pShapeFileObjekt->setGmlKlasse ( NULL );
			break;
		}
	}

	pShapeFileObjektGeltungsbereich->setGmlKlasse ( pPlanBereichKlasse );
	pShapeFileObjektGeltungsbereich->setBereichNummer ( bereichSelected );
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Neuer Bereich"																		 //
//  Initialisierung eines neuer Plan-Bereichs																 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonNeuerBereich_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	string name;
	int    anz;

	name = "Neuer Bereich";
	m_pPlanAttribute->addBereich ( name, "", "9999", "", -1 );
	anz = (int)m_pPlanAttribute->getBereichAnz();

	comboBoxBereiche->Items->Add ( Convert::ToString ( anz-1 ) );
	comboBoxBereiche->SelectedIndex = anz-1;
}


///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Dialogbox																							 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void initialize ()
{
	size_t  			           i, anz;
	bool					           success;
	string				           planBereichKlassenName;
	string				           planKlassenName;
	string                   stdString;
	string					         schluesselText;
	String				         ^ STR;
	vector<Bereich*>		     bereiche;
	vector<ShapeFileObjekt*> shapeFileObjekte;
	GmlKlasse			         * pPlanKlasse;
	GmlKlasse			         * pGmlKlasse;
  GmlEnumeration         * pEnumeration;
  vector<string>           enumItemNames;
  GmlDictionaryEntry     * pDictionaryEntry;

	planTyp = m_pXPlanGML->getPlanTyp();

  if ( planTyp == BPLAN )
  {
    planKlassenName        = "xplan:BP_Plan";
    planBereichKlassenName = "xplan:BP_Bereich";
  }
  else
  if ( planTyp == FPLAN )
  {
    planKlassenName        = "xplan:FP_Plan";
    planBereichKlassenName = "xplan:FP_Bereich";
  }
  else
  if ( planTyp == RPLAN )
  {
    planKlassenName        = "xplan:RP_Plan";
    planBereichKlassenName = "xplan:RP_Bereich";
  }
  else
  if ( planTyp == LPLAN )
  {
    planKlassenName        = "xplan:LP_Plan";
    planBereichKlassenName = "xplan:LP_Bereich";
  }
  else
  if ( planTyp == SOPLAN )
  {
    planKlassenName        = "xplan:SO_Plan";
    planBereichKlassenName = "xplan:SO_Bereich";
  }

  if ( m_pGmlSchema->getGmlTyp() ==  XPLANGML_3_0 )
  {
    comboBoxVersionBauNVO->Visible   = false;
    textBoxGesetzADatum->Visible     = true;

    labelGesetzBDatum->Visible      = false;
    labelGesetzBText->Visible     = false;
    textBoxGesetzBDatum->Visible = false;
    textBoxGesetzBText->Visible = false;
  }
  else
  if ( m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_0 || m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_1 ||
       m_pGmlSchema->getGmlTyp() ==  XPLANGML_5_0 || m_pGmlSchema->getGmlTyp() ==  XPLANGML_5_1 ||
       m_pGmlSchema->getGmlTyp() ==  XPLANGML_5_2 || m_pGmlSchema->getGmlTyp() == XPLANGML_5_3 )
  {
    if ( planTyp == BPLAN || planTyp == FPLAN )
    {
      labelGesetzBDatum->Text = "Datum der relevanten Version des BauGB (JJJJ-MM-TT)";
      labelGesetzBText->Text  = "Relevante Version des BauGB";

      if ( m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_0 || m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_1 )
      {
        textBoxGesetzADatum->Visible     = false;
        comboBoxVersionBauNVO->Visible   = true;

        labelGesetzADatum->Text = "Datum der relevanten Version der BauNVO";
        labelGesetzAText->Text  = "Relevante Version der BauNVO"; 

        enumItemNames.clear();
        pDictionaryEntry = m_pCodeLists->getDictionaryEntry ( "xplan:XP_VersionBauNVO" );
        pEnumeration = m_pGmlSchema->getGmlEnumeration ( "xplan:XP_VersionBauNVO" );
        if ( pEnumeration != NULL )
        {
          anz = pEnumeration->getEnumerationItemNames ( enumItemNames );
          for ( i = 0; i < anz; i++ )
          {
            stdString = enumItemNames[i];
            success = pDictionaryEntry->getSchluesselText ( stdString, schluesselText );
            comboBoxVersionBauNVO->Items->Add ( QuConvert::ToString ( schluesselText ) );
          }
        }
      }
      else
      if ( m_pGmlSchema->getGmlTyp() ==  XPLANGML_5_0 )
      {
        textBoxGesetzADatum->Visible     = true;
        comboBoxVersionBauNVO->Visible   = false;

        labelGesetzADatum->Text = "Datum der relevanten Version der BauNVO (JJJJ-MM-TT)";
        labelGesetzAText->Text  = "Relevante Version der BauNVO"; 
      }
    }
    else
	  if ( planTyp == RPLAN )
	  { 
      textBoxGesetzADatum->Visible     = true;
      comboBoxVersionBauNVO->Visible   = false;

      labelGesetzAText->Text  = "Relevante Version des Bundes-Raumordnungsgesetzes";
      labelGesetzADatum->Text = "Datum der relevanten Version des Bundes-Raumordnungsgesetzes (JJJJ-MM-TT)";
      labelGesetzBText->Text  = "Relevante Version des Landes-Planungsgesetzes";
      labelGesetzBDatum->Text = "Datum der relevanten Version des Landes-Planungsgesetzes (JJJJ-MM-TT)";
    }
    else
    {
      textBoxGesetzADatum->Visible     = false;
      comboBoxVersionBauNVO->Visible   = false;

       labelGesetzAText->Visible = false;
       labelGesetzADatum->Visible = false;
       labelGesetzBText->Visible = false;
       labelGesetzBDatum->Visible = false;

       textBoxGesetzAText->Visible = false;
       textBoxGesetzADatum->Visible = false;
       textBoxGesetzBText->Visible = false;
       textBoxGesetzBDatum->Visible = false;
    }
  }
		
	pPlanKlasse        = m_pGmlSchema->getGmlKlasseFromElementName ( planKlassenName );
	pPlanBereichKlasse = m_pGmlSchema->getGmlKlasseFromElementName ( planBereichKlassenName );

	anz = m_pPlanAttribute->getBereichAnz();
	for ( i = 0; i < anz; i++ )
	{
		STR = Convert::ToString ( i );
		comboBoxBereiche->Items->Add ( STR );
	}

	anz = m_pShapeFileObjekte->getShapeFileObjekte ( shapeFileObjekte );
	for ( i = 0; i < anz; i++ )
	{
		ShapeFileObjekt * pShapeFileObjekt = shapeFileObjekte[i];
		stdString = pShapeFileObjekt->getShapeFileName ();
		STR = QuConvert::ToString ( stdString );

		if ( pShapeFileObjekt->getShapeGeometrieTyp() == SHPT_POLYGON ||
			 pShapeFileObjekt->getShapeGeometrieTyp() == SHPT_POLYGON_M ||
			 pShapeFileObjekt->getShapeGeometrieTyp() == SHPT_POLYGON_Z )
		{

			 pGmlKlasse = pShapeFileObjekt->getGmlKlasse();
			 if ( pGmlKlasse != pPlanKlasse )
				comboBoxShapeFiles->Items->Add ( STR );
		}
	}

  pDictionaryEntry = m_pCodeLists->getDictionaryEntry ( "xplan:XP_BedeutungenBereich" );
  pEnumeration = m_pGmlSchema->getGmlEnumeration ( "xplan:XP_BedeutungenBereich" );
	if ( pEnumeration != NULL )
	
	anz = pEnumeration->getEnumerationItemNames ( enumItemNames );
	for ( i = 0; i < anz; i++ )
	{
		stdString = enumItemNames[i];
		success = pDictionaryEntry->getSchluesselText ( stdString, schluesselText );
		comboBoxBedeutung->Items->Add ( QuConvert::ToString ( schluesselText ) );
	}
	
	comboBoxBereiche->SelectedIndex = 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Verlassen der TextBox "Name"														 //
//  Setzt das Attribut "name" des aktuellen Bereichs												 // 
///////////////////////////////////////////////////////////////////////////////
private: System::Void textBoxBereichName_Leave(System::Object ^  sender, System::EventArgs ^  e)
{
	String ^ STR;
	string   stdString;

	STR = textBoxBereichName->Text;
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( stdString, STR );
	else
		stdString = "";

	Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );
	pBereich->name = stdString;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Verlassen der TextBox "Sonstige Bedeutung"							 //
//  Setzt das Text-Attribut "bedeutung" des aktuellen Bereichs							 // 
///////////////////////////////////////////////////////////////////////////////
private: System::Void textBoxBereichBedeutung_Leave(System::Object ^  sender, System::EventArgs ^  e)
{
	String ^ STR;
	string   stdString;

	STR = textBoxBereichBedeutung->Text;
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( stdString, STR );
	else
		stdString = "";

	Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );
	pBereich->bedeutung = stdString;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler ComboBox "Bedeutung"																			 //
//  Setzt das Attribut bedeutungEnum des Bereichs														 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxBedeutung_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	String ^ STR;
	string   stdString;
	string   schluesselNummer;
	int		 i;

	GmlDictionaryEntry * pDictionaryEntry = 
			m_pCodeLists->getDictionaryEntry ( "xplan:XP_BedeutungenBereich" );
	Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );

	i = comboBoxBedeutung->SelectedIndex;
	STR = dynamic_cast<String^>( comboBoxBedeutung->SelectedItem );
	QuConvert::systemStr2stdStr ( stdString, STR );
	pDictionaryEntry->getSchluesselNummer ( stdString, schluesselNummer );

	pBereich->bedeutungEnum = schluesselNummer;

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Verlassen der TextBox "Masstab"													 //
//  Setzt das Text-Attribut "masstab" des aktuellen Bereichs	  						 // 
///////////////////////////////////////////////////////////////////////////////
private: System::Void massstab_Leave(System::Object ^  sender, System::EventArgs ^  e)
{
	String ^ STR;
	string   stdString;
	int      m;

	Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );

	STR = massstab->Text;
	if ( STR != nullptr )
	{
		try
		{
			m = XmlConvert::ToInt32 ( STR );
		}
		catch (  System::FormatException^ )
		{
			massstab->Text = QuConvert::ToString ( pBereich->massstab );
			m = pBereich->massstab;
		}
	}	
	else
		m = -1;

	pBereich->massstab = m;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Button "Rasterdarstellung"															 //
//  Festlegung der Parameter für die Rasterdarstellung			  							 // 
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonRasterplanBasis_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );
	if ( pBereich == NULL ) return;

  if ( m_pGmlSchema->getGmlTyp() >=  XPLANGML_3_0 )
	{
		RasterplanBasisDialog ^ pDialog = gcnew RasterplanBasisDialog ( pBereich, m_pGmlSchema->getGmlTyp() );
		pDialog->ShowDialog();
	}

	if ( m_pGmlSchema->getGmlTyp() ==  XPLANGML_2_0 )
	{
		RasterplanBasisV2Dialog ^ pDialog = gcnew RasterplanBasisV2Dialog ( pBereich );
		pDialog->ShowDialog();
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Verlassen der TextBox "Verson der BauNVO"								 //
//  Setzt das Text-Attribut "versionBauNVOText" des aktuellen Bereichs			 // 
///////////////////////////////////////////////////////////////////////////////
private: System::Void textBoxBauNVO_Leave(System::Object ^  sender, System::EventArgs ^  e)
{
  if ( textBoxGesetzAText->Text->CompareTo( "" ) != 0  )
  {
    string str;

    Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );
    QuConvert::systemStr2stdStr ( str,  textBoxGesetzAText->Text );
    pBereich->versionBauNVOText     = str;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: ComboBox "Version der BauNVO"														 //
//  Setzt das Attribut "versionBauNVO" des aktuellen Bereichs								 // 
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxVersionBauNVO_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
 {
   String ^ STR;
   string   versionBauNVO = "";
   string   schluesselNummer;

   if ( comboBoxVersionBauNVO->SelectedItem != nullptr )
   {
     STR = comboBoxVersionBauNVO->SelectedItem->ToString();
     if ( STR != nullptr )
       QuConvert::systemStr2stdStr ( versionBauNVO, STR );
   }
   if ( STR == nullptr ) 
     return;

   GmlDictionaryEntry * pDictionaryEntry = 
     m_pCodeLists->getDictionaryEntry ( "xplan:XP_VersionBauNVO" );
   pDictionaryEntry->getSchluesselNummer ( versionBauNVO, schluesselNummer );

   Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );
   pBereich->versionBauNVO     = schluesselNummer;
 }
         
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"																					 //
//  Übernimmt die eingestellten Attribute und schließt den Dialog						 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
{
  String  ^ STR;
  string    stdString;
  string    schluesselNummer;
  Bereich * pBereich;

  pBereich = m_pPlanAttribute->getBereich ( bereichSelected );
  if ( pBereich == NULL ) return;

  if ( m_pGmlSchema->getGmlTyp() ==  XPLANGML_3_0 )
  {
    STR = textBoxGesetzAText->Text;
    if ( STR != nullptr )
    {
      QuConvert::systemStr2stdStr ( stdString, STR );
      pBereich->versionBauNVOText = stdString;		 
    }
  }
  else
  if ( m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_0 ||
       m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_1 || 
       m_pGmlSchema->getGmlTyp() ==  XPLANGML_5_0 ||
       m_pGmlSchema->getGmlTyp() ==  XPLANGML_5_1 ||
       m_pGmlSchema->getGmlTyp() ==  XPLANGML_5_2 ||
       m_pGmlSchema->getGmlTyp() ==  XPLANGML_5_3 )
  {
    if ( planTyp == BPLAN || planTyp == FPLAN )
    {
      STR = textBoxGesetzAText->Text;
      if ( STR != nullptr )
      {
        QuConvert::systemStr2stdStr ( stdString, STR );
        pBereich->versionBauNVOText = stdString;		 
      }

      if ( m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_0 || m_pGmlSchema->getGmlTyp() ==  XPLANGML_4_1  )
      {
        if ( comboBoxVersionBauNVO->SelectedItem != nullptr )
        {
          STR = comboBoxVersionBauNVO->SelectedItem->ToString();
          QuConvert::systemStr2stdStr ( stdString, STR );
          GmlDictionaryEntry * pDictionaryEntry =  m_pCodeLists->getDictionaryEntry ( "xplan:XP_VersionBauNVO" );
          if ( pDictionaryEntry != NULL )
          {
            pDictionaryEntry->getSchluesselNummer ( stdString, schluesselNummer );
            pBereich->versionBauNVO     = schluesselNummer;
          }
        }
      }
      else
      {
        STR = textBoxGesetzADatum->Text;
        if ( STR != nullptr )
        {
          QuConvert::systemStr2stdStr ( stdString, STR );
          pBereich->versionBauNVO = stdString;		 
        }
      }

      STR = textBoxGesetzBText->Text;
      if ( STR != nullptr )
      {
        QuConvert::systemStr2stdStr ( stdString, STR );
        pBereich->versionBauGBText = stdString;		 
      }

      STR = textBoxGesetzBDatum->Text;
      if ( STR != nullptr )
      {
        QuConvert::systemStr2stdStr ( stdString, STR );
        pBereich->versionBauGBDatum = stdString;		 
      }
    }
    else
    if ( planTyp == RPLAN  )
    {
      STR = textBoxGesetzAText->Text;
      if ( STR != nullptr )
      {
        QuConvert::systemStr2stdStr ( stdString, STR );
        pBereich->versionBROGText = stdString;		 
      }

      STR = textBoxGesetzADatum->Text;
      if ( STR != nullptr )
      {
        QuConvert::systemStr2stdStr ( stdString, STR );
        pBereich->versionBROGDatum = stdString;		 
      }

      STR = textBoxGesetzBText->Text;
      if ( STR != nullptr )
      {
        QuConvert::systemStr2stdStr ( stdString, STR );
        pBereich->versionLPLGText = stdString;		 
      }

      STR = textBoxGesetzBText->Text;
      if ( STR != nullptr )
      {
        QuConvert::systemStr2stdStr ( stdString, STR );
        pBereich->versionLPLGDatum = stdString;		 
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Verlassen der TextBox "Version der BauNVO (Textform)"	   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void textBoxGesetzADatum_Leave(System::Object^  sender, System::EventArgs^  e) 
         {
           if ( textBoxGesetzADatum->Text->CompareTo( "" ) != 0  )
           {
             string str;

             Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );
             QuConvert::systemStr2stdStr ( str,  textBoxGesetzADatum->Text );
             pBereich->versionBauNVO  = str;
           }
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Verlassen der TextBox "Version des BauGB"							   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void textBoxGesetzBText_Leave(System::Object^  sender, System::EventArgs^  e)
         {
           if ( textBoxGesetzBText->Text->CompareTo( "" ) != 0  )
           {
             string str;

             Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );
             QuConvert::systemStr2stdStr ( str,  textBoxGesetzBText->Text );
             pBereich->versionBauGBText     = str;
           }
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Verlassen der TextBox "Datum der Version des BauGB"  	   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void textBoxGesetzBDatum_Leave(System::Object^  sender, System::EventArgs^  e) 
         {
           if ( textBoxGesetzBDatum->Text->CompareTo( "" ) != 0  )
           {
             string str;

             Bereich * pBereich = m_pPlanAttribute->getBereich ( bereichSelected );
             QuConvert::systemStr2stdStr ( str,  textBoxGesetzBDatum->Text );
             pBereich->versionBauGBDatum     = str;
           }
         }
         
};
