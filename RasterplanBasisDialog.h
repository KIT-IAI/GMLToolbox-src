#pragma once


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Convert.h"
#include "ShpFileNamenKonv.h"

using namespace std;

	/// <summary> 
	/// Nur relevant für XPlanGML-Versionen >= 3.0: Festlegung der Attributwerte von XP_RasterplanBasis bzw. 
	/// XP_ExterneReferenz für XPlanGML 5.1 und 5.2
	///
	/// </summary>
	public ref class RasterplanBasisDialog : public System::Windows::Forms::Form
	{
	public: 
		RasterplanBasisDialog( Bereich * pBereichP, GML_SCHEMA_TYPE gmlSchemaType )
		{
			InitializeComponent();

			pBereich = pBereichP;
			pRasterplanBasis = pBereich->getRasterplanBasis();
			if ( pRasterplanBasis != NULL )
				pRasterplanBasisOld = new RasterplanBasis ( pRasterplanBasis );
			else
			{
				pRasterplanBasis = new RasterplanBasis;
				pBereich->setRasterplanBasis ( pRasterplanBasis );
				pRasterplanBasisOld = NULL;
			}

      if ( gmlSchemaType == XPLANGML_5_1 || 
           gmlSchemaType == XPLANGML_5_2 || 
           gmlSchemaType == XPLANGML_5_3 )
      {
        labelTextuellePlaninhalte->Visible = false;
        labelLegende->Visible = false;
        buttonBearbeitenTexte->Visible = false;
        buttonLoeschenTexte->Visible = false;
        buttonNeuLegende->Visible = false;
        buttonLoeschenLegende->Visible = false;
        comboBoxLegende->Visible = false;
      }

      if ( gmlSchemaType > XPLANGML_5_0 )
        this->Text = "Rasterdarstellung Plan";

			initialize();
		}
  private: System::Windows::Forms::Label^  labelTextuellePlaninhalte;
	private: System::Windows::Forms::Button ^  buttonBearbeitenTexte;
	private: System::Windows::Forms::ComboBox ^  comboBoxLegende;
  private: System::Windows::Forms::Label^  labelLegende;
	private: System::Windows::Forms::Button ^  buttonNeuLegende;
	private: System::Windows::Forms::Button ^  buttonLoeschenLegende;
	private: System::Windows::Forms::ComboBox ^  comboBoxPlan;
	private: System::Windows::Forms::Label ^  label3;
	private: System::Windows::Forms::Button ^  buttonNeuPlan;
	private: System::Windows::Forms::Button ^  buttonLoeschenPlan;
	private: System::Windows::Forms::GroupBox ^  groupBoxExterneReferenz;
	private: System::Windows::Forms::TextBox ^  textBoxURLReferenz;
	private: System::Windows::Forms::Label  ^  labelURLReferenz;
	private: System::Windows::Forms::TextBox ^  textBoxMimeTypeReferenz;
	private: System::Windows::Forms::Label ^  labelMimeTypeReferenz;
	private: System::Windows::Forms::TextBox ^  textBoxURLGeoref;
	private: System::Windows::Forms::Label ^  labelURLGeoref;
	private: System::Windows::Forms::TextBox ^  textBoxMimeTypeGeoref;
	private: System::Windows::Forms::Label ^  labelMimeTypeGeoref;
	private: System::Windows::Forms::Button ^  buttonExterneReferenzUebernehmen;
	private: System::Windows::Forms::Button ^  buttonBeenden;
	private: System::Windows::Forms::Button ^  buttonAbbrechen;
	private: System::Windows::Forms::Button ^  buttonExterneReferenzVerwerfen;
	private: System::Windows::Forms::Button ^  buttonLoeschenTexte;
  private: System::Windows::Forms::Label^  labelReferenzName;
  private: System::Windows::Forms::TextBox^  textBoxReferenzName;
  private: System::Windows::Forms::Label^  labelReferenzDatum;
  private: System::Windows::Forms::TextBox^  textBoxReferenzDatum;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container^ components;

		enum class REFERENZ_TYP  { TEXT, LEGENDE, PLAN, NONE };

		Bereich         * pBereich;								// Zug. Bereichs-Objekt
		RasterplanBasis * pRasterplanBasis;				// Festgelegte XP_RasterplanBasis Attribut-Werte
		RasterplanBasis * pRasterplanBasisOld;		// Bei Aufruf schon festgelegte XP_RasterplanBasis Attribut-Werte
		ExterneReferenz * pExterneReferenzAktiv;  // Aktuell bearbeitete externe Referenz
		REFERENZ_TYP      referenzTypAktiv;				// Typ der aktuell bearbeiteten externen Referenz
    GML_SCHEMA_TYPE   m_gmlSchemaType;				// XPlanGML Schema-Typ

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->labelTextuellePlaninhalte = ( gcnew System::Windows::Forms::Label () );
			this->buttonBearbeitenTexte = ( gcnew System::Windows::Forms::Button () );
			this->comboBoxLegende = ( gcnew System::Windows::Forms::ComboBox () );
			this->labelLegende = ( gcnew System::Windows::Forms::Label () );
			this->buttonNeuLegende = ( gcnew System::Windows::Forms::Button () );
			this->buttonLoeschenLegende = ( gcnew System::Windows::Forms::Button () );
			this->comboBoxPlan = ( gcnew System::Windows::Forms::ComboBox () );
			this->label3 = ( gcnew System::Windows::Forms::Label () );
			this->buttonNeuPlan = ( gcnew System::Windows::Forms::Button () );
			this->buttonLoeschenPlan = ( gcnew System::Windows::Forms::Button () );
			this->groupBoxExterneReferenz = ( gcnew System::Windows::Forms::GroupBox () );
			this->labelReferenzDatum = ( gcnew System::Windows::Forms::Label () );
			this->textBoxReferenzDatum = ( gcnew System::Windows::Forms::TextBox () );
			this->labelReferenzName = ( gcnew System::Windows::Forms::Label () );
			this->textBoxReferenzName = ( gcnew System::Windows::Forms::TextBox () );
			this->buttonExterneReferenzVerwerfen = ( gcnew System::Windows::Forms::Button () );
			this->buttonExterneReferenzUebernehmen = ( gcnew System::Windows::Forms::Button () );
			this->labelMimeTypeGeoref = ( gcnew System::Windows::Forms::Label () );
			this->textBoxMimeTypeGeoref = ( gcnew System::Windows::Forms::TextBox () );
			this->labelURLGeoref = ( gcnew System::Windows::Forms::Label () );
			this->textBoxURLGeoref = ( gcnew System::Windows::Forms::TextBox () );
			this->labelMimeTypeReferenz = ( gcnew System::Windows::Forms::Label () );
			this->textBoxMimeTypeReferenz = ( gcnew System::Windows::Forms::TextBox () );
			this->labelURLReferenz = ( gcnew System::Windows::Forms::Label () );
			this->textBoxURLReferenz = ( gcnew System::Windows::Forms::TextBox () );
			this->buttonBeenden = ( gcnew System::Windows::Forms::Button () );
			this->buttonAbbrechen = ( gcnew System::Windows::Forms::Button () );
			this->buttonLoeschenTexte = ( gcnew System::Windows::Forms::Button () );
			this->groupBoxExterneReferenz->SuspendLayout ();
			this->SuspendLayout ();
			// 
			// labelTextuellePlaninhalte
			// 
			this->labelTextuellePlaninhalte->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>( 0 ) ) );
			this->labelTextuellePlaninhalte->Location = System::Drawing::Point ( 16, 19 );
			this->labelTextuellePlaninhalte->Name = L"labelTextuellePlaninhalte";
			this->labelTextuellePlaninhalte->Size = System::Drawing::Size ( 128, 16 );
			this->labelTextuellePlaninhalte->TabIndex = 0;
			this->labelTextuellePlaninhalte->Text = L"Textuelle Planinhalte";
			// 
			// buttonBearbeitenTexte
			// 
			this->buttonBearbeitenTexte->Location = System::Drawing::Point ( 157, 19 );
			this->buttonBearbeitenTexte->Name = L"buttonBearbeitenTexte";
			this->buttonBearbeitenTexte->Size = System::Drawing::Size ( 75, 23 );
			this->buttonBearbeitenTexte->TabIndex = 1;
			this->buttonBearbeitenTexte->Text = L"Bearbeiten";
			this->buttonBearbeitenTexte->Click += gcnew System::EventHandler ( this, &RasterplanBasisDialog::buttonBearbeitenTexte_Click );
			// 
			// comboBoxLegende
			// 
			this->comboBoxLegende->Location = System::Drawing::Point ( 16, 57 );
			this->comboBoxLegende->Name = L"comboBoxLegende";
			this->comboBoxLegende->Size = System::Drawing::Size ( 104, 21 );
			this->comboBoxLegende->TabIndex = 2;
			this->comboBoxLegende->SelectedIndexChanged += gcnew System::EventHandler ( this, &RasterplanBasisDialog::comboBoxLegende_SelectedIndexChanged );
			// 
			// labelLegende
			// 
			this->labelLegende->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>( 0 ) ) );
			this->labelLegende->Location = System::Drawing::Point ( 16, 80 );
			this->labelLegende->Name = L"labelLegende";
			this->labelLegende->Size = System::Drawing::Size ( 144, 23 );
			this->labelLegende->TabIndex = 3;
			this->labelLegende->Text = L"Legende";
			// 
			// buttonNeuLegende
			// 
			this->buttonNeuLegende->Location = System::Drawing::Point ( 157, 59 );
			this->buttonNeuLegende->Name = L"buttonNeuLegende";
			this->buttonNeuLegende->Size = System::Drawing::Size ( 75, 23 );
			this->buttonNeuLegende->TabIndex = 4;
			this->buttonNeuLegende->Text = L"Neu";
			this->buttonNeuLegende->Click += gcnew System::EventHandler ( this, &RasterplanBasisDialog::buttonNeuLegende_Click );
			// 
			// buttonLoeschenLegende
			// 
			this->buttonLoeschenLegende->Location = System::Drawing::Point ( 261, 59 );
			this->buttonLoeschenLegende->Name = L"buttonLoeschenLegende";
			this->buttonLoeschenLegende->Size = System::Drawing::Size ( 75, 23 );
			this->buttonLoeschenLegende->TabIndex = 5;
			this->buttonLoeschenLegende->Text = L"Löschen";
			this->buttonLoeschenLegende->Click += gcnew System::EventHandler ( this, &RasterplanBasisDialog::buttonLoeschenLegende_Click );
			// 
			// comboBoxPlan
			// 
			this->comboBoxPlan->Location = System::Drawing::Point ( 16, 105 );
			this->comboBoxPlan->Name = L"comboBoxPlan";
			this->comboBoxPlan->Size = System::Drawing::Size ( 104, 21 );
			this->comboBoxPlan->TabIndex = 6;
			this->comboBoxPlan->SelectedIndexChanged += gcnew System::EventHandler ( this, &RasterplanBasisDialog::comboBoxPlan_SelectedIndexChanged );
			// 
			// label3
			// 
			this->label3->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>( 0 ) ) );
			this->label3->Location = System::Drawing::Point ( 16, 128 );
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size ( 100, 23 );
			this->label3->TabIndex = 7;
			this->label3->Text = L"Plandarstellung";
			// 
			// buttonNeuPlan
			// 
			this->buttonNeuPlan->Location = System::Drawing::Point ( 157, 107 );
			this->buttonNeuPlan->Name = L"buttonNeuPlan";
			this->buttonNeuPlan->Size = System::Drawing::Size ( 75, 23 );
			this->buttonNeuPlan->TabIndex = 8;
			this->buttonNeuPlan->Text = L"Neu";
			this->buttonNeuPlan->Click += gcnew System::EventHandler ( this, &RasterplanBasisDialog::buttonNeuPlan_Click );
			// 
			// buttonLoeschenPlan
			// 
			this->buttonLoeschenPlan->Location = System::Drawing::Point ( 261, 107 );
			this->buttonLoeschenPlan->Name = L"buttonLoeschenPlan";
			this->buttonLoeschenPlan->Size = System::Drawing::Size ( 75, 23 );
			this->buttonLoeschenPlan->TabIndex = 9;
			this->buttonLoeschenPlan->Text = L"Löschen";
			this->buttonLoeschenPlan->Click += gcnew System::EventHandler ( this, &RasterplanBasisDialog::buttonLoeschenPlan_Click );
			// 
			// groupBoxExterneReferenz
			// 
			this->groupBoxExterneReferenz->Controls->Add ( this->labelReferenzDatum );
			this->groupBoxExterneReferenz->Controls->Add ( this->textBoxReferenzDatum );
			this->groupBoxExterneReferenz->Controls->Add ( this->labelReferenzName );
			this->groupBoxExterneReferenz->Controls->Add ( this->textBoxReferenzName );
			this->groupBoxExterneReferenz->Controls->Add ( this->buttonExterneReferenzVerwerfen );
			this->groupBoxExterneReferenz->Controls->Add ( this->buttonExterneReferenzUebernehmen );
			this->groupBoxExterneReferenz->Controls->Add ( this->labelMimeTypeGeoref );
			this->groupBoxExterneReferenz->Controls->Add ( this->textBoxMimeTypeGeoref );
			this->groupBoxExterneReferenz->Controls->Add ( this->labelURLGeoref );
			this->groupBoxExterneReferenz->Controls->Add ( this->textBoxURLGeoref );
			this->groupBoxExterneReferenz->Controls->Add ( this->labelMimeTypeReferenz );
			this->groupBoxExterneReferenz->Controls->Add ( this->textBoxMimeTypeReferenz );
			this->groupBoxExterneReferenz->Controls->Add ( this->labelURLReferenz );
			this->groupBoxExterneReferenz->Controls->Add ( this->textBoxURLReferenz );
			this->groupBoxExterneReferenz->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>( 0 ) ) );
			this->groupBoxExterneReferenz->ForeColor = System::Drawing::SystemColors::ControlText;
			this->groupBoxExterneReferenz->Location = System::Drawing::Point ( 16, 154 );
			this->groupBoxExterneReferenz->Name = L"groupBoxExterneReferenz";
			this->groupBoxExterneReferenz->Size = System::Drawing::Size ( 384, 274 );
			this->groupBoxExterneReferenz->TabIndex = 10;
			this->groupBoxExterneReferenz->TabStop = false;
			// 
			// labelReferenzDatum
			// 
			this->labelReferenzDatum->AutoSize = true;
			this->labelReferenzDatum->Location = System::Drawing::Point ( 172, 200 );
			this->labelReferenzDatum->Name = L"labelReferenzDatum";
			this->labelReferenzDatum->Size = System::Drawing::Size ( 43, 13 );
			this->labelReferenzDatum->TabIndex = 13;
			this->labelReferenzDatum->Text = L"Datum";
			// 
			// textBoxReferenzDatum
			// 
			this->textBoxReferenzDatum->Location = System::Drawing::Point ( 172, 179 );
			this->textBoxReferenzDatum->Name = L"textBoxReferenzDatum";
			this->textBoxReferenzDatum->Size = System::Drawing::Size ( 100, 20 );
			this->textBoxReferenzDatum->TabIndex = 12;
			// 
			// labelReferenzName
			// 
			this->labelReferenzName->AutoSize = true;
			this->labelReferenzName->Location = System::Drawing::Point ( 8, 200 );
			this->labelReferenzName->Name = L"labelReferenzName";
			this->labelReferenzName->Size = System::Drawing::Size ( 94, 13 );
			this->labelReferenzName->TabIndex = 11;
			this->labelReferenzName->Text = L"Referenz-Name";
			// 
			// textBoxReferenzName
			// 
			this->textBoxReferenzName->Location = System::Drawing::Point ( 8, 179 );
			this->textBoxReferenzName->Name = L"textBoxReferenzName";
			this->textBoxReferenzName->Size = System::Drawing::Size ( 100, 20 );
			this->textBoxReferenzName->TabIndex = 10;
			this->textBoxReferenzName->Text = L"Rasterplan";
			// 
			// buttonExterneReferenzVerwerfen
			// 
			this->buttonExterneReferenzVerwerfen->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>( 0 ) ) );
			this->buttonExterneReferenzVerwerfen->Location = System::Drawing::Point ( 40, 239 );
			this->buttonExterneReferenzVerwerfen->Name = L"buttonExterneReferenzVerwerfen";
			this->buttonExterneReferenzVerwerfen->Size = System::Drawing::Size ( 75, 23 );
			this->buttonExterneReferenzVerwerfen->TabIndex = 9;
			this->buttonExterneReferenzVerwerfen->Text = L"Verwerfen";
			this->buttonExterneReferenzVerwerfen->Click += gcnew System::EventHandler ( this, &RasterplanBasisDialog::buttonExterneReferenzVerwerfen_Click );
			// 
			// buttonExterneReferenzUebernehmen
			// 
			this->buttonExterneReferenzUebernehmen->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>( 0 ) ) );
			this->buttonExterneReferenzUebernehmen->Location = System::Drawing::Point ( 264, 239 );
			this->buttonExterneReferenzUebernehmen->Name = L"buttonExterneReferenzUebernehmen";
			this->buttonExterneReferenzUebernehmen->Size = System::Drawing::Size ( 80, 23 );
			this->buttonExterneReferenzUebernehmen->TabIndex = 8;
			this->buttonExterneReferenzUebernehmen->Text = L"Übernehmen";
			this->buttonExterneReferenzUebernehmen->Click += gcnew System::EventHandler ( this, &RasterplanBasisDialog::buttonExterneReferenzUebernehmen_Click );
			// 
			// labelMimeTypeGeoref
			// 
			this->labelMimeTypeGeoref->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>( 0 ) ) );
			this->labelMimeTypeGeoref->Location = System::Drawing::Point ( 112, 136 );
			this->labelMimeTypeGeoref->Name = L"labelMimeTypeGeoref";
			this->labelMimeTypeGeoref->Size = System::Drawing::Size ( 224, 23 );
			this->labelMimeTypeGeoref->TabIndex = 7;
			this->labelMimeTypeGeoref->Text = L"Mime Type der Georef-Datei";
			// 
			// textBoxMimeTypeGeoref
			// 
			this->textBoxMimeTypeGeoref->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>( 0 ) ) );
			this->textBoxMimeTypeGeoref->Location = System::Drawing::Point ( 8, 132 );
			this->textBoxMimeTypeGeoref->Name = L"textBoxMimeTypeGeoref";
			this->textBoxMimeTypeGeoref->Size = System::Drawing::Size ( 100, 20 );
			this->textBoxMimeTypeGeoref->TabIndex = 6;
			// 
			// labelURLGeoref
			// 
			this->labelURLGeoref->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>( 0 ) ) );
			this->labelURLGeoref->Location = System::Drawing::Point ( 8, 111 );
			this->labelURLGeoref->Name = L"labelURLGeoref";
			this->labelURLGeoref->Size = System::Drawing::Size ( 312, 23 );
			this->labelURLGeoref->TabIndex = 5;
			this->labelURLGeoref->Text = L"URL der Georeferenzierungsdatei";
			// 
			// textBoxURLGeoref
			// 
			this->textBoxURLGeoref->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>( 0 ) ) );
			this->textBoxURLGeoref->Location = System::Drawing::Point ( 8, 87 );
			this->textBoxURLGeoref->Name = L"textBoxURLGeoref";
			this->textBoxURLGeoref->Size = System::Drawing::Size ( 368, 20 );
			this->textBoxURLGeoref->TabIndex = 4;
			// 
			// labelMimeTypeReferenz
			// 
			this->labelMimeTypeReferenz->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>( 0 ) ) );
			this->labelMimeTypeReferenz->Location = System::Drawing::Point ( 109, 61 );
			this->labelMimeTypeReferenz->Name = L"labelMimeTypeReferenz";
			this->labelMimeTypeReferenz->Size = System::Drawing::Size ( 192, 23 );
			this->labelMimeTypeReferenz->TabIndex = 3;
			this->labelMimeTypeReferenz->Text = L"Mime Type des Rasterplans";
			// 
			// textBoxMimeTypeReferenz
			// 
			this->textBoxMimeTypeReferenz->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>( 0 ) ) );
			this->textBoxMimeTypeReferenz->Location = System::Drawing::Point ( 8, 60 );
			this->textBoxMimeTypeReferenz->Name = L"textBoxMimeTypeReferenz";
			this->textBoxMimeTypeReferenz->Size = System::Drawing::Size ( 100, 20 );
			this->textBoxMimeTypeReferenz->TabIndex = 2;
			// 
			// labelURLReferenz
			// 
			this->labelURLReferenz->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>( 0 ) ) );
			this->labelURLReferenz->Location = System::Drawing::Point ( 8, 41 );
			this->labelURLReferenz->Name = L"labelURLReferenz";
			this->labelURLReferenz->Size = System::Drawing::Size ( 160, 23 );
			this->labelURLReferenz->TabIndex = 1;
			this->labelURLReferenz->Text = L"URL des Rasterplans";
			// 
			// textBoxURLReferenz
			// 
			this->textBoxURLReferenz->Font = ( gcnew System::Drawing::Font ( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>( 0 ) ) );
			this->textBoxURLReferenz->Location = System::Drawing::Point ( 8, 17 );
			this->textBoxURLReferenz->Name = L"textBoxURLReferenz";
			this->textBoxURLReferenz->Size = System::Drawing::Size ( 368, 20 );
			this->textBoxURLReferenz->TabIndex = 0;
			// 
			// buttonBeenden
			// 
			this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->buttonBeenden->Location = System::Drawing::Point ( 320, 434 );
			this->buttonBeenden->Name = L"buttonBeenden";
			this->buttonBeenden->Size = System::Drawing::Size ( 75, 23 );
			this->buttonBeenden->TabIndex = 11;
			this->buttonBeenden->Text = L"Beenden";
			this->buttonBeenden->Click += gcnew System::EventHandler ( this, &RasterplanBasisDialog::buttonBeenden_Click );
			// 
			// buttonAbbrechen
			// 
			this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->buttonAbbrechen->Location = System::Drawing::Point ( 24, 434 );
			this->buttonAbbrechen->Name = L"buttonAbbrechen";
			this->buttonAbbrechen->Size = System::Drawing::Size ( 75, 23 );
			this->buttonAbbrechen->TabIndex = 12;
			this->buttonAbbrechen->Text = L"Abbrechen";
			this->buttonAbbrechen->Click += gcnew System::EventHandler ( this, &RasterplanBasisDialog::buttonAbbrechen_Click );
			// 
			// buttonLoeschenTexte
			// 
			this->buttonLoeschenTexte->Location = System::Drawing::Point ( 261, 19 );
			this->buttonLoeschenTexte->Name = L"buttonLoeschenTexte";
			this->buttonLoeschenTexte->Size = System::Drawing::Size ( 75, 23 );
			this->buttonLoeschenTexte->TabIndex = 13;
			this->buttonLoeschenTexte->Text = L"Löschen";
			this->buttonLoeschenTexte->Click += gcnew System::EventHandler ( this, &RasterplanBasisDialog::buttonLoeschenTexte_Click );
			// 
			// RasterplanBasisDialog
			// 
			this->AutoScaleBaseSize = System::Drawing::Size ( 5, 13 );
			this->ClientSize = System::Drawing::Size ( 416, 472 );
			this->Controls->Add ( this->buttonLoeschenTexte );
			this->Controls->Add ( this->buttonAbbrechen );
			this->Controls->Add ( this->buttonBeenden );
			this->Controls->Add ( this->groupBoxExterneReferenz );
			this->Controls->Add ( this->buttonLoeschenPlan );
			this->Controls->Add ( this->buttonNeuPlan );
			this->Controls->Add ( this->label3 );
			this->Controls->Add ( this->comboBoxPlan );
			this->Controls->Add ( this->buttonLoeschenLegende );
			this->Controls->Add ( this->buttonNeuLegende );
			this->Controls->Add ( this->labelLegende );
			this->Controls->Add ( this->comboBoxLegende );
			this->Controls->Add ( this->buttonBearbeitenTexte );
			this->Controls->Add ( this->labelTextuellePlaninhalte );
			this->Name = L"RasterplanBasisDialog";
			this->Text = L"Rasterdarstellung Basisplan";
			this->groupBoxExterneReferenz->ResumeLayout ( false );
			this->groupBoxExterneReferenz->PerformLayout ();
			this->ResumeLayout ( false );

		}

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Dialogbox																							 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void initialize()
		 {
			 int      i;
			 String ^ STR;

			 for ( i = 0; i < pRasterplanBasis->getExterneReferenzenLegendeAnz(); i++ )
			 {
				 STR = String::Concat ( "Legende Link", Convert::ToString ( i ) );
				 comboBoxLegende->Items->Add ( dynamic_cast<String^>( STR ) );
			 }

			 for ( i = 0; i < pRasterplanBasis->getExterneReferenzenPlanAnz(); i++ )
			 {
				 STR = String::Concat ( "Plan Link", Convert::ToString ( i ) );
				 comboBoxPlan->Items->Add ( dynamic_cast<String^>( STR ) );
			 }

       referenzTypAktiv      = REFERENZ_TYP::NONE;
			 pExterneReferenzAktiv = NULL;	

			 anzeigeExterneReferenzAktiv();
		 }

///////////////////////////////////////////////////////////////////////////////
//  Aktiviert / Deaktiviert Dialogelemente und ändert Beschriftungen				 //
//  in Anhängigkeit, welche externe Referenz (Textliche Festsetzungne,       //
//  Legende oder Planinhalt) gerade spezifiziert wird.	     						  	 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void anzeigeExterneReferenzAktiv ()
		 {
			 string   stdString;
			 String ^ STR;

       if ( pExterneReferenzAktiv == NULL || referenzTypAktiv == REFERENZ_TYP::NONE )
			 {
				buttonExterneReferenzUebernehmen->Enabled = false;
				buttonExterneReferenzVerwerfen->Enabled = false;

				textBoxURLReferenz->Hide();
				labelURLReferenz->Hide();
				textBoxMimeTypeReferenz->Hide();
				labelMimeTypeReferenz->Hide();
				textBoxURLGeoref->Hide();
				labelURLGeoref->Hide();
				textBoxMimeTypeGeoref->Hide();
				labelMimeTypeGeoref->Hide();
        textBoxReferenzName->Hide();
        labelReferenzName->Hide();
        textBoxReferenzDatum->Hide();
        labelReferenzDatum->Hide();

				groupBoxExterneReferenz->Text = nullptr;
			  }
			  else
			  {
				buttonExterneReferenzUebernehmen->Enabled = true;
				buttonExterneReferenzVerwerfen->Enabled = true;
				
				STR = QuConvert::ToString ( pExterneReferenzAktiv->referenzURL );
				textBoxURLReferenz->Text = STR;
				textBoxURLReferenz->Show();

  			STR = QuConvert::ToString ( pExterneReferenzAktiv->referenzMimeType );
				textBoxMimeTypeReferenz->Text = STR;
				textBoxMimeTypeReferenz->Show();

 				STR = QuConvert::ToString ( pExterneReferenzAktiv->georefURL );
				textBoxURLGeoref->Text = STR;
				textBoxURLGeoref->Show();

 				STR = QuConvert::ToString ( pExterneReferenzAktiv->georefMimeType );
				textBoxMimeTypeGeoref->Text = STR;
				textBoxMimeTypeGeoref->Show();

        STR = QuConvert::ToString ( pExterneReferenzAktiv->referenzName );
        textBoxReferenzName->Text = STR;
        textBoxReferenzName->Show();
        labelReferenzName->Show();

        STR = QuConvert::ToString ( pExterneReferenzAktiv->datum );
        textBoxReferenzDatum->Text = STR;
        textBoxReferenzDatum->Show();
        labelReferenzDatum->Show();

				if ( referenzTypAktiv == REFERENZ_TYP::TEXT )
				{
					groupBoxExterneReferenz->Text = "Spezifikation Referenz auf Texte";

					labelURLReferenz->Show();
					labelURLReferenz->Text = "Referenz auf Textdokument";

					labelMimeTypeReferenz->Show();
					labelMimeTypeReferenz->Text = "MimeType Textdokument";

					labelURLGeoref->Hide();
					labelMimeTypeGeoref->Hide();
					textBoxURLGeoref->Hide();
					textBoxMimeTypeGeoref->Hide();
				}
				else
          if ( referenzTypAktiv == REFERENZ_TYP::LEGENDE )
				{
					groupBoxExterneReferenz->Text = "Spezifikation Referenz auf Legende";

					labelURLReferenz->Show();
					labelURLReferenz->Text = "Referenz auf Legende";

					labelMimeTypeReferenz->Show();
					labelMimeTypeReferenz->Text = "MimeType Legendendokument";

					labelURLGeoref->Hide();
					labelMimeTypeGeoref->Hide();
					textBoxURLGeoref->Hide();
					textBoxMimeTypeGeoref->Hide();
				}
				else
				if ( referenzTypAktiv == REFERENZ_TYP::PLAN )
				{
					groupBoxExterneReferenz->Text = "Spezifikation Referenz auf Plan";

					labelURLReferenz->Show();
					labelURLReferenz->Text = "Referenz auf Plan";

					labelMimeTypeReferenz->Show();
					labelMimeTypeReferenz->Text = "MimeType Plandokument";

					labelURLGeoref->Show();
					labelURLGeoref->Text = "Referenz auf Georeferenzierungsdatei";

					labelMimeTypeGeoref->Show();
					labelMimeTypeGeoref->Text = "MimeType Georeferenzierungsdatei";
				}
			 }
		 }

///////////////////////////////////////////////////////////////////////////////
//  Übernimmt die Eingaben für die aktuell bearbeitete externe Referenz			 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void updateExterneReferenzAktiv ()
		 {
			 String ^ STR;
			 string   stdString;

			 if ( pExterneReferenzAktiv == NULL ) return;

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

       if ( referenzTypAktiv != REFERENZ_TYP::PLAN ) return;

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

       STR = textBoxReferenzName->Text;
       if ( STR != nullptr )
         QuConvert::systemStr2stdStr ( stdString, STR );
       else
         stdString = "";

       pExterneReferenzAktiv->referenzName = stdString;

       STR = textBoxReferenzDatum->Text;
       if ( STR != nullptr )
         QuConvert::systemStr2stdStr ( stdString, STR );
       else
         stdString = "";

       pExterneReferenzAktiv->datum = stdString;
		 }

///////////////////////////////////////////////////////////////////////////////
//  Deaktiviert alle Dialogelemente zur Auswahl einer externen Referenz			 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void disableAuswahl ()
		 {
			 labelTextuellePlaninhalte->Enabled = false;
			 buttonBearbeitenTexte->Enabled = false;
			 buttonLoeschenTexte->Enabled = false; 
			 comboBoxLegende->Enabled = false;
			 labelLegende->Enabled = false;
			 buttonNeuLegende->Enabled = false;
			 buttonLoeschenLegende->Enabled = false;
			 comboBoxPlan->Enabled = false;
			 label3->Enabled = false;
			 buttonNeuPlan->Enabled = false;
			 buttonLoeschenPlan->Enabled = false;
		 }

///////////////////////////////////////////////////////////////////////////////
//  Aktiviert alle Dialogelemente zur Auswahl einer externen Referenz			   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void enableAuswahl ()
		 {
			 labelTextuellePlaninhalte->Enabled = true;
			 buttonBearbeitenTexte->Enabled = true;
			 buttonLoeschenTexte->Enabled = true;
			 comboBoxLegende->Enabled = true;
			 labelLegende->Enabled = true;
			 buttonNeuLegende->Enabled = true;
			 buttonLoeschenLegende->Enabled = true;
			 comboBoxPlan->Enabled = true;
			 label3->Enabled = true;
			 buttonNeuPlan->Enabled = true;
			 buttonLoeschenPlan->Enabled = true;
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Bearbeiten" (Textuelle Planinhalte)							 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBearbeitenTexte_Click(System::Object ^  sender, System::EventArgs^  e)
		 {
			 pExterneReferenzAktiv = pRasterplanBasis->getExterneReferenzText();
			 if ( pExterneReferenzAktiv == NULL )
			 {
				 pExterneReferenzAktiv = new ExterneReferenz();
         pExterneReferenzAktiv->relationName = "xplan:refText";
				 pRasterplanBasis->addExterneReferenz ( pExterneReferenzAktiv );
			 }
       referenzTypAktiv = REFERENZ_TYP::TEXT;
			 disableAuswahl();
			 anzeigeExterneReferenzAktiv();
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Löschen" (Textuelle Planinhalte)					    		 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonLoeschenTexte_Click(System::Object ^  sender, System::EventArgs^  e)
		 {
			 pExterneReferenzAktiv = pRasterplanBasis->getExterneReferenzText();
			 if ( pExterneReferenzAktiv != NULL )
				 pRasterplanBasis->deleteExterneReferenzText();

			 pExterneReferenzAktiv = NULL;
			 referenzTypAktiv      = REFERENZ_TYP::NONE;
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Neu" (Legende)																		 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonNeuLegende_Click(System::Object ^  sender, System::EventArgs^  e)
		 {
			String ^ STR;
			int      anz;

			pExterneReferenzAktiv = new ExterneReferenz();
      pExterneReferenzAktiv->relationName = "xplan:refLegende";
			pRasterplanBasis->addExterneReferenz ( pExterneReferenzAktiv );

			anz = (int)pRasterplanBasis->getExterneReferenzenLegendeAnz();
			STR = String::Concat ( "Legende Link", Convert::ToString ( anz-1 ) );
			comboBoxLegende->Items->Add ( dynamic_cast<String^>( STR ) );
			comboBoxLegende->SelectedIndex = anz-1;

			referenzTypAktiv = REFERENZ_TYP::LEGENDE;
			disableAuswahl();
			buttonLoeschenLegende->Enabled = true;
			anzeigeExterneReferenzAktiv();
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Löschen" (Legende)																 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonLoeschenLegende_Click(System::Object ^  sender, System::EventArgs^  e)
		 {
			 int      i;
			 String ^ STR;
			 int      index = comboBoxLegende->SelectedIndex;
			 
			 if ( index < 0 ) return;

			 pExterneReferenzAktiv = pRasterplanBasis->getExterneReferenzLegende ( index );
			 if ( pExterneReferenzAktiv != NULL )
			 {
				 pRasterplanBasis->deleteExterneReferenzLegende ( index );
				 comboBoxLegende->Items->Clear();

				for ( i = 0; i < pRasterplanBasis->getExterneReferenzenLegendeAnz(); i++ )
				{
					 STR = String::Concat ( "Legende Link", Convert::ToString ( i ) );
					 comboBoxLegende->Items->Add ( dynamic_cast<String^>( STR ) );
				}
			 }

			 comboBoxLegende->SelectedIndex = -1;
			 comboBoxLegende->Text = "";
       referenzTypAktiv = REFERENZ_TYP::NONE;
			 pExterneReferenzAktiv = NULL;
			 enableAuswahl();
 			 anzeigeExterneReferenzAktiv();
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler ComboBox "Legende"																				 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxLegende_SelectedIndexChanged(System::Object ^  sender, System::EventArgs^  e)
		 {
			 int index = comboBoxLegende->SelectedIndex;
			 if ( index < 0 ) return;

			 pExterneReferenzAktiv = pRasterplanBasis->getExterneReferenzLegende ( index );
			 if ( pExterneReferenzAktiv != NULL )
			 {
				referenzTypAktiv = REFERENZ_TYP::LEGENDE;
				disableAuswahl();
				buttonLoeschenLegende->Enabled = true;
				anzeigeExterneReferenzAktiv();
			 }
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler ComboBox "Plandarstellung"  															 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxPlan_SelectedIndexChanged(System::Object ^  sender, System::EventArgs^  e)
		 {
			 int index = comboBoxPlan->SelectedIndex;
			 if ( index < 0 ) return;

			 pExterneReferenzAktiv = pRasterplanBasis->getExterneReferenzPlan ( index );
			 if ( pExterneReferenzAktiv != NULL )
			 {
				referenzTypAktiv = REFERENZ_TYP::PLAN;
				disableAuswahl();
				buttonLoeschenPlan->Enabled = true;
				anzeigeExterneReferenzAktiv();
			 }

			referenzTypAktiv = REFERENZ_TYP::PLAN;
			disableAuswahl();
			buttonLoeschenPlan->Enabled = true;
			anzeigeExterneReferenzAktiv();
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Neu" (Plandarstellung)														 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonNeuPlan_Click(System::Object ^  sender, System::EventArgs^  e)
		 {
			String ^ STR;
			int      anz;
      string   date;

			pExterneReferenzAktiv = new ExterneReferenz();
      pExterneReferenzAktiv->relationName = "xplan:refScan";
      pExterneReferenzAktiv->referenzName = "Rasterplan";

      DateTime   pDate = DateTime::Now;
      STR              = XmlConvert::ToString ( pDate, "yyyy-MM-dd" );
      QuConvert::systemStr2stdStr( date, STR );
      pExterneReferenzAktiv->datum = date;

			pRasterplanBasis->addExterneReferenz ( pExterneReferenzAktiv );

			anz = (int)pRasterplanBasis->getExterneReferenzenPlanAnz();
			STR = String::Concat ( "PLan Link", Convert::ToString ( anz-1 ) );
			comboBoxPlan->Items->Add ( dynamic_cast<String^>( STR ) );
			comboBoxPlan->SelectedIndex = anz-1;

			referenzTypAktiv = REFERENZ_TYP::PLAN;
			disableAuswahl();
			buttonLoeschenPlan->Enabled = true;
			anzeigeExterneReferenzAktiv();
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Löschen" (Plandarstellung)												 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonLoeschenPlan_Click(System::Object ^  sender, System::EventArgs^  e)
		 {
			 int      i;
			 String ^ STR;
			 int      index = comboBoxPlan->SelectedIndex;
			 
			 if ( index < 0 ) return;

			 pExterneReferenzAktiv = pRasterplanBasis->getExterneReferenzPlan ( index );
			 if ( pExterneReferenzAktiv != NULL )
			 {
				 pRasterplanBasis->deleteExterneReferenzPlan ( index );
				 comboBoxPlan->Items->Clear();

				for ( i = 0; i < pRasterplanBasis->getExterneReferenzenPlanAnz(); i++ )
				{
					 STR = String::Concat ( "Plan Link", Convert::ToString ( i ) );
					 comboBoxPlan->Items->Add ( dynamic_cast<String^>( STR ) );
				}
			 }

			 comboBoxPlan->SelectedIndex = -1;
			 comboBoxPlan->Text = "";
			 referenzTypAktiv = REFERENZ_TYP::NONE;
			 pExterneReferenzAktiv = NULL;
			 enableAuswahl();
 			 anzeigeExterneReferenzAktiv();
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Übernehmen"																			 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonExterneReferenzUebernehmen_Click(System::Object ^  sender, System::EventArgs^  e)
		 {
			 updateExterneReferenzAktiv();
			 enableAuswahl();

			 referenzTypAktiv      = REFERENZ_TYP::NONE;
			 pExterneReferenzAktiv = NULL;	

			 comboBoxLegende->SelectedIndex = -1;
			 comboBoxPlan->SelectedIndex = -1;

			 anzeigeExterneReferenzAktiv();
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Verwerfen"		  																	 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonExterneReferenzVerwerfen_Click(System::Object ^  sender, System::EventArgs^  e)
		 {
			 enableAuswahl();

			 referenzTypAktiv      = REFERENZ_TYP::NONE;
			 pExterneReferenzAktiv = NULL;	

			 comboBoxLegende->SelectedIndex = -1;
			 comboBoxPlan->SelectedIndex = -1;

			 anzeigeExterneReferenzAktiv();
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Abbrechen" 																			 //
//  Beendet den Dialog																											 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAbbrechen_Click(System::Object ^  sender, System::EventArgs^  e)
		 {
			pBereich->setRasterplanBasis ( pRasterplanBasisOld );
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"   																			 //
//  Beendet den Dialog																											 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object ^  sender, System::EventArgs^  e)
		 {
			 delete pRasterplanBasisOld;
		 }

};
