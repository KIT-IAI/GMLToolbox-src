#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace std;

#include "GmlSchema.h"
#include "ShpFileObjekte.h"
#include "Convert.h"
#include "ShpFileNamenKonv.h"

	/// <summary> 
	/// Zusammenfassung für PlanAttributeBearbeiten
	///
	/// Achtung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie die Eigenschaft
	///          'Ressourcendateiname' für das Compilertool für verwaltete Ressourcen ändern, 
	///          das allen .resx-Dateien zugewiesen ist, von denen diese Klasse abhängt. 
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class PlanAttributeBearbeiten : public System::Windows::Forms::Form
	{
	public: 
		PlanAttributeBearbeiten( ShapeFileObjekte * pShapeFileObjekteP, GmlObjekte * pGmlObjekteP  )
		{
			InitializeComponent();

			pGmlObjekte   = pGmlObjekteP;
			pShapeFileObjekte = pShapeFileObjekteP;
		
			initialize();
		}
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}

	private:
		ShapeFileObjekte * pShapeFileObjekte;
		GmlObjekte       * pGmlObjekte;

		String           * beschreibung;
		String           * realisierung;
		String			 * umweltbericht;
		String			 * begruendung;

	private: System::Windows::Forms::Label *  labelGKZ;
	private: System::Windows::Forms::TextBox *  textBoxGKZ;
	private: System::Windows::Forms::Label *  labelGemeinde;
	private: System::Windows::Forms::TextBox *  textBoxGemeinde;
	private: System::Windows::Forms::Label *  labelPlanNummer;
	private: System::Windows::Forms::TextBox *  textBoxPlanNummer;
	private: System::Windows::Forms::Label *  labelPlanName;
	private: System::Windows::Forms::TextBox *  textBoxPlanName;
	private: System::Windows::Forms::Label *  labelVerfahrensStand;
	private: System::Windows::Forms::ComboBox *  comboBoxVerfahrensStand;
	private: System::Windows::Forms::Label *  labelHoehenBezug;
	private: System::Windows::Forms::TextBox *  textBoxHoehenBezug;
	private: System::Windows::Forms::ComboBox *  comboBoxWeitereAttribute;
	private: System::Windows::Forms::TextBox *  textBoxWeitereAttribute;

	private: System::Windows::Forms::Button *  buttonBeenden;
	private: System::Windows::Forms::Label *  labelHerstellungsDatum;
	private: System::Windows::Forms::Label *  labelWirksamkeitsDatum;
	private: System::Windows::Forms::TextBox *  textBoxHerstellungsDatum;
	private: System::Windows::Forms::TextBox *  textBoxWirksamkeitsDatum;
	private: System::Windows::Forms::TextBox *  textBoxAenderungenBis;
	private: System::Windows::Forms::Label *  labelAenderungenBis;
	private: System::Windows::Forms::Label *  label1;
	private: System::Windows::Forms::Label *  label2;





	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container* components;

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->labelGKZ = new System::Windows::Forms::Label();
			this->textBoxGKZ = new System::Windows::Forms::TextBox();
			this->labelGemeinde = new System::Windows::Forms::Label();
			this->textBoxGemeinde = new System::Windows::Forms::TextBox();
			this->labelPlanNummer = new System::Windows::Forms::Label();
			this->textBoxPlanNummer = new System::Windows::Forms::TextBox();
			this->labelPlanName = new System::Windows::Forms::Label();
			this->textBoxPlanName = new System::Windows::Forms::TextBox();
			this->labelVerfahrensStand = new System::Windows::Forms::Label();
			this->comboBoxVerfahrensStand = new System::Windows::Forms::ComboBox();
			this->labelHoehenBezug = new System::Windows::Forms::Label();
			this->textBoxHoehenBezug = new System::Windows::Forms::TextBox();
			this->comboBoxWeitereAttribute = new System::Windows::Forms::ComboBox();
			this->textBoxWeitereAttribute = new System::Windows::Forms::TextBox();
			this->buttonBeenden = new System::Windows::Forms::Button();
			this->labelHerstellungsDatum = new System::Windows::Forms::Label();
			this->labelWirksamkeitsDatum = new System::Windows::Forms::Label();
			this->textBoxHerstellungsDatum = new System::Windows::Forms::TextBox();
			this->textBoxWirksamkeitsDatum = new System::Windows::Forms::TextBox();
			this->textBoxAenderungenBis = new System::Windows::Forms::TextBox();
			this->labelAenderungenBis = new System::Windows::Forms::Label();
			this->label1 = new System::Windows::Forms::Label();
			this->label2 = new System::Windows::Forms::Label();
			this->SuspendLayout();
			// 
			// labelGKZ
			// 
			this->labelGKZ->Location = System::Drawing::Point(16, 8);
			this->labelGKZ->Name = S"labelGKZ";
			this->labelGKZ->Size = System::Drawing::Size(40, 20);
			this->labelGKZ->TabIndex = 0;
			this->labelGKZ->Text = S"GKZ";
			this->labelGKZ->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// textBoxGKZ
			// 
			this->textBoxGKZ->Location = System::Drawing::Point(104, 8);
			this->textBoxGKZ->Name = S"textBoxGKZ";
			this->textBoxGKZ->Size = System::Drawing::Size(400, 20);
			this->textBoxGKZ->TabIndex = 1;
			this->textBoxGKZ->Text = S"";
			this->textBoxGKZ->Leave += new System::EventHandler(this, textBoxGKZ_Leave);
			// 
			// labelGemeinde
			// 
			this->labelGemeinde->Location = System::Drawing::Point(16, 40);
			this->labelGemeinde->Name = S"labelGemeinde";
			this->labelGemeinde->Size = System::Drawing::Size(72, 20);
			this->labelGemeinde->TabIndex = 2;
			this->labelGemeinde->Text = S"Gemeinde(n)";
			this->labelGemeinde->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// textBoxGemeinde
			// 
			this->textBoxGemeinde->Location = System::Drawing::Point(104, 40);
			this->textBoxGemeinde->Name = S"textBoxGemeinde";
			this->textBoxGemeinde->Size = System::Drawing::Size(400, 20);
			this->textBoxGemeinde->TabIndex = 3;
			this->textBoxGemeinde->Text = S"";
			// 
			// labelPlanNummer
			// 
			this->labelPlanNummer->Location = System::Drawing::Point(16, 72);
			this->labelPlanNummer->Name = S"labelPlanNummer";
			this->labelPlanNummer->Size = System::Drawing::Size(80, 20);
			this->labelPlanNummer->TabIndex = 4;
			this->labelPlanNummer->Text = S"Plan-Nummer";
			this->labelPlanNummer->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// textBoxPlanNummer
			// 
			this->textBoxPlanNummer->Location = System::Drawing::Point(104, 72);
			this->textBoxPlanNummer->Name = S"textBoxPlanNummer";
			this->textBoxPlanNummer->Size = System::Drawing::Size(400, 20);
			this->textBoxPlanNummer->TabIndex = 5;
			this->textBoxPlanNummer->Text = S"";
			// 
			// labelPlanName
			// 
			this->labelPlanName->Location = System::Drawing::Point(16, 104);
			this->labelPlanName->Name = S"labelPlanName";
			this->labelPlanName->Size = System::Drawing::Size(80, 20);
			this->labelPlanName->TabIndex = 6;
			this->labelPlanName->Text = S"Plan-Bez.";
			this->labelPlanName->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// textBoxPlanName
			// 
			this->textBoxPlanName->Location = System::Drawing::Point(104, 104);
			this->textBoxPlanName->Name = S"textBoxPlanName";
			this->textBoxPlanName->Size = System::Drawing::Size(400, 20);
			this->textBoxPlanName->TabIndex = 7;
			this->textBoxPlanName->Text = S"";
			// 
			// labelVerfahrensStand
			// 
			this->labelVerfahrensStand->Location = System::Drawing::Point(16, 136);
			this->labelVerfahrensStand->Name = S"labelVerfahrensStand";
			this->labelVerfahrensStand->Size = System::Drawing::Size(64, 24);
			this->labelVerfahrensStand->TabIndex = 8;
			this->labelVerfahrensStand->Text = S"Verfahrens-Stand";
			this->labelVerfahrensStand->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// comboBoxVerfahrensStand
			// 
			this->comboBoxVerfahrensStand->Location = System::Drawing::Point(104, 136);
			this->comboBoxVerfahrensStand->Name = S"comboBoxVerfahrensStand";
			this->comboBoxVerfahrensStand->Size = System::Drawing::Size(112, 21);
			this->comboBoxVerfahrensStand->TabIndex = 9;
			// 
			// labelHoehenBezug
			// 
			this->labelHoehenBezug->Location = System::Drawing::Point(224, 136);
			this->labelHoehenBezug->Name = S"labelHoehenBezug";
			this->labelHoehenBezug->Size = System::Drawing::Size(72, 24);
			this->labelHoehenBezug->TabIndex = 10;
			this->labelHoehenBezug->Text = S"Höhenbezug";
			this->labelHoehenBezug->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// textBoxHoehenBezug
			// 
			this->textBoxHoehenBezug->Location = System::Drawing::Point(296, 136);
			this->textBoxHoehenBezug->Name = S"textBoxHoehenBezug";
			this->textBoxHoehenBezug->Size = System::Drawing::Size(208, 20);
			this->textBoxHoehenBezug->TabIndex = 11;
			this->textBoxHoehenBezug->Text = S"";
			// 
			// comboBoxWeitereAttribute
			// 
			this->comboBoxWeitereAttribute->Location = System::Drawing::Point(16, 240);
			this->comboBoxWeitereAttribute->Name = S"comboBoxWeitereAttribute";
			this->comboBoxWeitereAttribute->Size = System::Drawing::Size(121, 21);
			this->comboBoxWeitereAttribute->TabIndex = 18;
			this->comboBoxWeitereAttribute->SelectedValueChanged += new System::EventHandler(this, comboBoxWeitereAttribute_SelectedValueChanged);
			// 
			// textBoxWeitereAttribute
			// 
			this->textBoxWeitereAttribute->Location = System::Drawing::Point(16, 272);
			this->textBoxWeitereAttribute->Multiline = true;
			this->textBoxWeitereAttribute->Name = S"textBoxWeitereAttribute";
			this->textBoxWeitereAttribute->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBoxWeitereAttribute->Size = System::Drawing::Size(488, 152);
			this->textBoxWeitereAttribute->TabIndex = 20;
			this->textBoxWeitereAttribute->Text = S"";
			this->textBoxWeitereAttribute->Leave += new System::EventHandler(this, textBoxWeitereAttribute_Leave);
			// 
			// buttonBeenden
			// 
			this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->buttonBeenden->Location = System::Drawing::Point(424, 440);
			this->buttonBeenden->Name = S"buttonBeenden";
			this->buttonBeenden->TabIndex = 22;
			this->buttonBeenden->Text = S"Beenden";
			this->buttonBeenden->Click += new System::EventHandler(this, buttonBeenden_Click);
			// 
			// labelHerstellungsDatum
			// 
			this->labelHerstellungsDatum->Location = System::Drawing::Point(24, 208);
			this->labelHerstellungsDatum->Name = S"labelHerstellungsDatum";
			this->labelHerstellungsDatum->Size = System::Drawing::Size(104, 16);
			this->labelHerstellungsDatum->TabIndex = 24;
			this->labelHerstellungsDatum->Text = S"Herstellungs-Datum";
			this->labelHerstellungsDatum->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// labelWirksamkeitsDatum
			// 
			this->labelWirksamkeitsDatum->Location = System::Drawing::Point(144, 208);
			this->labelWirksamkeitsDatum->Name = S"labelWirksamkeitsDatum";
			this->labelWirksamkeitsDatum->Size = System::Drawing::Size(112, 16);
			this->labelWirksamkeitsDatum->TabIndex = 26;
			this->labelWirksamkeitsDatum->Text = S"Wirksamkeits-Datum";
			this->labelWirksamkeitsDatum->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// textBoxHerstellungsDatum
			// 
			this->textBoxHerstellungsDatum->Location = System::Drawing::Point(24, 184);
			this->textBoxHerstellungsDatum->Name = S"textBoxHerstellungsDatum";
			this->textBoxHerstellungsDatum->Size = System::Drawing::Size(96, 20);
			this->textBoxHerstellungsDatum->TabIndex = 27;
			this->textBoxHerstellungsDatum->Text = S"";
			this->textBoxHerstellungsDatum->Leave += new System::EventHandler(this, textBoxHerstellungsDatum_Leave);
			// 
			// textBoxWirksamkeitsDatum
			// 
			this->textBoxWirksamkeitsDatum->Location = System::Drawing::Point(144, 184);
			this->textBoxWirksamkeitsDatum->Name = S"textBoxWirksamkeitsDatum";
			this->textBoxWirksamkeitsDatum->Size = System::Drawing::Size(96, 20);
			this->textBoxWirksamkeitsDatum->TabIndex = 28;
			this->textBoxWirksamkeitsDatum->Text = S"";
			this->textBoxWirksamkeitsDatum->Leave += new System::EventHandler(this, textBoxWirksamkeitsDatum_Leave);
			// 
			// textBoxAenderungenBis
			// 
			this->textBoxAenderungenBis->Location = System::Drawing::Point(256, 184);
			this->textBoxAenderungenBis->Name = S"textBoxAenderungenBis";
			this->textBoxAenderungenBis->Size = System::Drawing::Size(96, 20);
			this->textBoxAenderungenBis->TabIndex = 29;
			this->textBoxAenderungenBis->Text = S"";
			this->textBoxAenderungenBis->Leave += new System::EventHandler(this, textBoxAenderungenBis_Leave);
			// 
			// labelAenderungenBis
			// 
			this->labelAenderungenBis->Location = System::Drawing::Point(256, 208);
			this->labelAenderungenBis->Name = S"labelAenderungenBis";
			this->labelAenderungenBis->Size = System::Drawing::Size(88, 16);
			this->labelAenderungenBis->TabIndex = 30;
			this->labelAenderungenBis->Text = S"Änderungen bis";
			this->labelAenderungenBis->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(376, 192);
			this->label1->Name = S"label1";
			this->label1->Size = System::Drawing::Size(88, 24);
			this->label1->TabIndex = 31;
			this->label1->Text = S"Datum als  YYYY-MM-DD";
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(144, 240);
			this->label2->Name = S"label2";
			this->label2->Size = System::Drawing::Size(100, 16);
			this->label2->TabIndex = 32;
			this->label2->Text = S"Weitere Attribute";
			// 
			// PlanAttributeBearbeiten
			// 
			this->AcceptButton = this->buttonBeenden;
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->CancelButton = this->buttonBeenden;
			this->ClientSize = System::Drawing::Size(512, 469);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->labelAenderungenBis);
			this->Controls->Add(this->textBoxAenderungenBis);
			this->Controls->Add(this->textBoxWirksamkeitsDatum);
			this->Controls->Add(this->textBoxHerstellungsDatum);
			this->Controls->Add(this->textBoxWeitereAttribute);
			this->Controls->Add(this->textBoxHoehenBezug);
			this->Controls->Add(this->textBoxPlanName);
			this->Controls->Add(this->textBoxPlanNummer);
			this->Controls->Add(this->textBoxGemeinde);
			this->Controls->Add(this->textBoxGKZ);
			this->Controls->Add(this->labelWirksamkeitsDatum);
			this->Controls->Add(this->labelHerstellungsDatum);
			this->Controls->Add(this->buttonBeenden);
			this->Controls->Add(this->comboBoxWeitereAttribute);
			this->Controls->Add(this->labelHoehenBezug);
			this->Controls->Add(this->comboBoxVerfahrensStand);
			this->Controls->Add(this->labelVerfahrensStand);
			this->Controls->Add(this->labelPlanName);
			this->Controls->Add(this->labelPlanNummer);
			this->Controls->Add(this->labelGemeinde);
			this->Controls->Add(this->labelGKZ);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = S"PlanAttributeBearbeiten";
			this->Text = S"Plan-Attribute bearbeiten";
			this->ResumeLayout(false);

		}		
	



private: System::Void buttonWeitereAttributeUebernehmen_Click(System::Object *  sender, System::EventArgs *  e)
{
	String * auswahl;
	string   stdString;

	auswahl = dynamic_cast<String*> ( comboBoxWeitereAttribute->get_SelectedItem () );

	if ( String::Compare ( auswahl, "beschreibung" ) == 0 )
		beschreibung = textBoxWeitereAttribute->get_Text ();
	else
	if ( String::Compare ( auswahl, "begruendung" ) == 0 )
		begruendung = textBoxWeitereAttribute->get_Text ();
	else
	if ( String::Compare ( auswahl, "realisierung" ) == 0 )
		realisierung = textBoxWeitereAttribute->get_Text ();
	else
	if ( String::Compare ( auswahl, "umweltbericht" ) == 0 )
		umweltbericht = textBoxWeitereAttribute->get_Text ();
}

private: System::Void buttonBeenden_Click(System::Object *  sender, System::EventArgs *  e)
{
	string   stdString;
	String * STR;
	int      i;

	PlanAttribute * pPlanAttribute = pShapeFileObjekte->getBLeitplanViewer()->getPlanAttribute();
	
	int planTyp = pGmlObjekte->getGmlSchemaTyp();


	STR = textBoxGKZ->get_Text();
	if ( String::Compare ( STR, "" ) != 0 )
	{
		i = Convert::ToInt64 ( STR );
		pPlanAttribute->setIntegerAttribut ( "gkz", i );
	}

	STR = textBoxPlanName->get_Text();
	if ( String::Compare ( STR, "" ) != 0 )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pPlanAttribute->setStringAttribut ( "name", stdString );
	}

	STR = textBoxPlanNummer->get_Text();
	if ( String::Compare ( STR, "" ) != 0 )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pPlanAttribute->setStringAttribut ( "nummer", stdString );
	}

	STR = textBoxGemeinde->get_Text();
	if ( String::Compare ( STR, "" ) != 0 )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pPlanAttribute->setStringAttribut ( "stadt", stdString );
	}

	if ( planTyp == BPLAN )
	{
		STR = textBoxHoehenBezug->get_Text();
		if ( String::Compare ( STR, "" ) != 0 )
		{
			QuConvert::systemStr2stdStr ( stdString, STR );
			pPlanAttribute->setStringAttribut ( "hoehenbezug", stdString );
		}
	}

	STR = textBoxHerstellungsDatum->get_Text();
	if (String::Compare ( STR, "" ) != 0 )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		if ( planTyp == BPLAN )
			pPlanAttribute->setStringAttribut ( "datum", stdString );
		else
			pPlanAttribute->setStringAttribut ( "technHerstellDatum", stdString );
	}

	if (  planTyp == FPLAN )
	{
		STR = textBoxWirksamkeitsDatum->get_Text();
		if (String::Compare ( STR, "" ) != 0 )
		{
			QuConvert::systemStr2stdStr ( stdString, STR );
			pPlanAttribute->setStringAttribut ( "wirksamkeitsDatum", stdString );
		}

		STR = textBoxAenderungenBis->get_Text();
		if (String::Compare ( STR, "" ) != 0 )
		{
			QuConvert::systemStr2stdStr ( stdString, STR );
			pPlanAttribute->setStringAttribut ( "aenderungenBis", stdString );
		}
	}

	STR = dynamic_cast<String*> ( comboBoxVerfahrensStand->get_SelectedItem() );
	if ( STR != NULL )
	{
		QuConvert::systemStr2stdStr ( stdString, STR );
		pPlanAttribute->setStringAttribut ( "verfahrensstand", stdString );
	}

	if ( beschreibung != NULL ) 
	{
		QuConvert::systemStr2stdStr ( stdString, beschreibung );
		pPlanAttribute->setStringAttribut ( "beschreibung", stdString );
	}

	if ( begruendung != NULL ) 
	{
		QuConvert::systemStr2stdStr ( stdString, begruendung );
		pPlanAttribute->setStringAttribut ( "begruendung", stdString );
	}

	if ( realisierung != NULL ) 
	{
		QuConvert::systemStr2stdStr ( stdString, realisierung );
		pPlanAttribute->setStringAttribut ( "realisierung", stdString );
	}

	if ( umweltbericht != NULL ) 
	{
		QuConvert::systemStr2stdStr ( stdString, umweltbericht );
		pPlanAttribute->setStringAttribut ( "umweltbericht", stdString );
	}

}

private: System::Void initialize()
{
	string   stdString;
	int      gkz, anz, i;
	bool     success;
	vector<string> items;
	String * STR;

	PlanAttribute * pPlanAttribute = pShapeFileObjekte->getBLeitplanViewer()->getPlanAttribute();

	int planTyp = pGmlObjekte->getGmlSchemaTyp();

	success = pPlanAttribute->getIntegerAttributWert ( "gkz", gkz );
	if ( success )
	{
		STR = Convert::ToString ( gkz );
		textBoxGKZ->set_Text ( STR );
	}

	success = pPlanAttribute->getStringAttributWert ( "name", stdString );
	if ( success )
	{
		STR = QuConvert::ToString ( stdString );
		textBoxPlanName->set_Text ( STR );
	}

	success = pPlanAttribute->getStringAttributWert ( "nummer", stdString );
	if ( success )
	{
		STR = QuConvert::ToString ( stdString );
		textBoxPlanNummer->set_Text ( STR );
	}

	success = pPlanAttribute->getStringAttributWert ( "stadt", stdString );
	if ( success )
	{
		STR = QuConvert::ToString ( stdString );
		textBoxGemeinde->set_Text ( STR );
	}

	if ( planTyp == BPLAN )
	{
		success = pPlanAttribute->getStringAttributWert ( "hoehenbezug", stdString );
		if ( success )
		{
			STR = QuConvert::ToString ( stdString );
			textBoxHoehenBezug->set_Text ( STR );
		}
	}
	else 
	{
		labelHoehenBezug->set_Visible ( false );
		textBoxHoehenBezug->set_Visible ( false );
	}
	

	if ( planTyp == FPLAN )
	{
		success = pPlanAttribute->getStringAttributWert ( "technHerstellDatum", stdString );
		if ( success )
		{
			STR = QuConvert::ToString ( stdString );
			textBoxHerstellungsDatum->set_Text ( STR );
		}

		success = pPlanAttribute->getStringAttributWert ( "wirksamkeitsDatum", stdString );
		if ( success )
		{
			STR = QuConvert::ToString ( stdString );
			textBoxWirksamkeitsDatum->set_Text ( STR );
		}

		success = pPlanAttribute->getStringAttributWert ( "aenderungenBis", stdString );
		if ( success )
		{
			STR = QuConvert::ToString ( stdString );
			textBoxAenderungenBis->set_Text ( STR );
		}
	}
	else
	{
		success = pPlanAttribute->getStringAttributWert ( "datum", stdString );
		if ( success )
		{
			STR = QuConvert::ToString ( stdString );
			textBoxHerstellungsDatum->set_Text ( STR );
		}
		textBoxWirksamkeitsDatum->set_Visible ( false );
		textBoxAenderungenBis->set_Visible ( false );
		labelWirksamkeitsDatum->set_Visible ( false );
		labelAenderungenBis->set_Visible ( false );
	}

	GmlEnumeration * pEnumeration = pGmlObjekte->getGmlEnumeration ( "VerfahrensstandType" );
	anz = pEnumeration->getEnumerationItemNames ( items );
	for ( i = 0; i < anz; i++ )
	{
		stdString = items[i];
		STR = QuConvert::ToString ( stdString );
		comboBoxVerfahrensStand->get_Items()->Add ( STR );
	}

	success = pPlanAttribute->getStringAttributWert ( "verfahrensstand", stdString );
	if ( success )
	{
		STR = QuConvert::ToString ( stdString );
		comboBoxVerfahrensStand->set_SelectedItem ( STR );
	}

	comboBoxWeitereAttribute->get_Items()->Add ( S"beschreibung" );
	success = pPlanAttribute->getStringAttributWert ( "beschreibung", stdString );
	if ( success ) beschreibung = QuConvert::ToString ( stdString );
	else		   beschreibung = NULL;

	comboBoxWeitereAttribute->get_Items()->Add ( S"begruendung" );
	success = pPlanAttribute->getStringAttributWert ( "begruendung", stdString );
	if ( success ) begruendung = QuConvert::ToString ( stdString );
	else		   begruendung = NULL;

	comboBoxWeitereAttribute->get_Items()->Add ( S"umweltbericht" );
	success = pPlanAttribute->getStringAttributWert ( "umweltbericht", stdString );
	if ( success ) umweltbericht = QuConvert::ToString ( stdString );
	else		   umweltbericht = NULL;

	if (  planTyp == BPLAN )
	{
		comboBoxWeitereAttribute->get_Items()->Add ( S"realisierung" );
		success = pPlanAttribute->getStringAttributWert ( "realisierung", stdString );
		if ( success ) realisierung = QuConvert::ToString ( stdString );
		else		   realisierung = NULL;
	}
}

private: System::Void textBoxHerstellungsDatum_Leave(System::Object *  sender, System::EventArgs *  e)
{
	String * STR;

	STR = textBoxHerstellungsDatum->get_Text();
	if ( String::Compare ( STR, "" ) != 0 )
	{
		try
		{
			XmlConvert::ToDateTime ( STR );
		}
		catch ( System::FormatException* )
		{
			System::Windows::Forms::MessageBox::Show ( "Ungültiges Datum" );
			textBoxHerstellungsDatum->set_Text ( NULL );
		}
	}

}

private: System::Void textBoxWirksamkeitsDatum_Leave(System::Object *  sender, System::EventArgs *  e)
{
	String * STR;

	STR = textBoxWirksamkeitsDatum->get_Text();
	if ( String::Compare ( STR, "" ) != 0 )
	{
		try
		{
			XmlConvert::ToDateTime ( STR );
		}
		catch ( System::FormatException* )
		{
			System::Windows::Forms::MessageBox::Show ( "Ungültiges Datum" );
			textBoxWirksamkeitsDatum->set_Text ( NULL );
		}
	}
}

private: System::Void textBoxAenderungenBis_Leave(System::Object *  sender, System::EventArgs *  e)
{
	String * STR;

	STR = textBoxAenderungenBis->get_Text();
	if ( String::Compare ( STR, "" ) != 0 )
	{
		try
		{
			XmlConvert::ToDateTime ( STR );
		}
		catch ( System::FormatException* )
		{
			System::Windows::Forms::MessageBox::Show ( "Ungültiges Datum" );
			textBoxAenderungenBis->set_Text ( NULL );
		}
	}
}

private: System::Void textBoxGKZ_Leave(System::Object *  sender, System::EventArgs *  e)
{
	String * STR;

	if ( pGmlObjekte->getGmlSchemaTyp() == BPLAN )
	{
		STR = textBoxGKZ->get_Text();

		if ( String::Compare ( STR, "" ) != 0 )
		{
			try
			{
				Convert::ToInt64 ( STR );
			}
			catch ( System::FormatException* )
			{
				System::Windows::Forms::MessageBox::Show ( "Ungültiger GKZ-Wert" );
				textBoxGKZ->set_Text ( NULL );
			}
		}
	}

}
private: System::Void comboBoxWeitereAttribute_SelectedValueChanged(System::Object *  sender, System::EventArgs *  e)
{
	String * auswahl;
	string   stdString;

	auswahl = dynamic_cast<String*> ( comboBoxWeitereAttribute->get_SelectedItem () );

	if ( String::Compare ( auswahl, "beschreibung" ) == 0 )
		textBoxWeitereAttribute->set_Text ( beschreibung );
	else
	if ( String::Compare ( auswahl, "begruendung" ) == 0 )
		textBoxWeitereAttribute->set_Text ( begruendung );
	else
	if ( String::Compare ( auswahl, "realisierung" ) == 0 )
		textBoxWeitereAttribute->set_Text ( realisierung );
	else
	if ( String::Compare ( auswahl, "umweltbericht" ) == 0 )
		textBoxWeitereAttribute->set_Text ( umweltbericht );
}

private: System::Void textBoxWeitereAttribute_Leave(System::Object *  sender, System::EventArgs *  e)
{
	String * auswahl;
	string   stdString;

	auswahl = dynamic_cast<String*> ( comboBoxWeitereAttribute->get_SelectedItem () );

	if ( String::Compare ( auswahl, "beschreibung" ) == 0 )
		beschreibung = textBoxWeitereAttribute->get_Text ();
	else
	if ( String::Compare ( auswahl, "begruendung" ) == 0 )
		begruendung = textBoxWeitereAttribute->get_Text ();
	else
	if ( String::Compare ( auswahl, "realisierung" ) == 0 )
		realisierung = textBoxWeitereAttribute->get_Text ();
	else
	if ( String::Compare ( auswahl, "umweltbericht" ) == 0 )
		umweltbericht = textBoxWeitereAttribute->get_Text ();
}
