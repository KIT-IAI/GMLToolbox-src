#pragma once


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <stdlib.h>
#include "GmlSchema.h"
#include "ShpFileObjekte.h"
#include "Convert.h"
#include "includes.h"
#include "shptypeenum.h"
#include "ShapeFileConversion.h"
#include "PlanAttributeNeu.h"
#include "EnumerationConversion.h"
#include "TextlicheFestsetzungen.h"
#include "PlanBereiche.h"

using namespace std;

	/// <summary> 
	/// Interaktive Definition von Abbildungsregeln Shapefile --> XPlanGML
	///
	/// </summary>
	public ref class GenerateKonversion : public System::Windows::Forms::Form
	{
	public: 
		GenerateKonversion( XPlanGML * pXPlanGML, GmlDictionary * pCodeListsP )
		{
			InitializeComponent();

      m_pXPlanGML         = pXPlanGML;
			m_pCodeLists        = pCodeListsP;
			m_pGmlObjekte	      = m_pXPlanGML->getFeatures()->getGmlSchema();
			m_pShapeFileObjekte = m_pXPlanGML->getShapeFileObjekte();

      pCoordSystems = gcnew  System::Collections::SortedList;

			initialize();
		}

    virtual ~GenerateKonversion()
    {
      if ( m_pExternalCodeLists != NULL )
        delete m_pExternalCodeLists;
    }

        
	protected: 

	private:
		enum class OPERATION  {  
								TEXTUELLE_FESTSETZUNGEN, // Abbildung von textuell definierten Planinhalten
								PLAN_ATTRIBUTE,					 // Festlegung der Planattribute
								PLAN_BEREICHE,					 // Festlegung der Planbereiche
								SHAPEFILE,							 // Abbildungsregeln für einzelne Shapefiles
								ENUMERATION  };					 // Abbildungsregeln für Enumerations- und Codelist-Werte

    XPlanGML          * m_pXPlanGML;						// Modul XPlanGML
		GmlDictionary	    * m_pCodeLists;						// Dictionary der Enumerationen
		ShapeFileObjekte  * m_pShapeFileObjekte;		// Repräsentation der Shapefile-Struktur
    GmlSchema         * m_pGmlObjekte;					// Repräsentation des XPlanGML Schemas
		GmlDictionary	    * m_pExternalCodeLists;		// Dictionary der externen Codelisten		

		OPERATION			      operation;												// Aktuell ausgewählter Teilschritt bei der Generierung der Abbildungsregeln 
		ShapeFileObjekt   * pShapeFileObjektActive;						// Aktuell bearbeiteter Shapefile
		GmlKlasse		      * pGmlKlasseActive;									// Dem aktuellen Shapefile zugeordnete XPlanGML-Klasse
		GmlKlasse         * pPlanKlasse;											// Dem Gesamtplan zugeordnete XPlanGML-Klasse
		GmlEnumeration    * pGmlEnumerationActive;						// Aktuell bearbeitete Enumeration
		ShapeFileObjekt   * pShapeFileObjektFromEnumActive;		// Der aktuell bearbeiteten Enumeration zugeordnetes Shapefile-Objekt
		ShapeFileAttribut * pShapeFileAttributFromEnumActive; // Der aktuell bearbeiteten Enumeration zugeordnetes Shapefile-Attribut

    System::Collections::SortedList ^ pCoordSystems;			// Liste der unterstützten Koordinatensysteme

	private: System::Windows::Forms::RadioButton ^  radioButtonTextlicheFestsetzungen;
	private: System::Windows::Forms::RadioButton ^  radioButtonPlanAttribute;
	private: System::Windows::Forms::RadioButton ^  radioButtonShapeDatei;
	private: System::Windows::Forms::RadioButton ^  radioButtonKonversionAufzaehlung;
	private: System::Windows::Forms::ComboBox ^  comboBoxShapeDateien;
	private: System::Windows::Forms::Label ^  labelShapeDatei;
	private: System::Windows::Forms::ComboBox ^  comboBoxGmlKlasse;
	private: System::Windows::Forms::Label ^  LabelGmlKlasse;
	private: System::Windows::Forms::ComboBox ^  comboBoxGmlEnumerationen;
	private: System::Windows::Forms::Label ^  labelGmlEnumeration;
	private: System::Windows::Forms::Button ^  buttonWeiter;
	private: System::Windows::Forms::Button ^  buttonBeenden;
	private: System::Windows::Forms::TextBox ^  textBoxShapeDatei;
	private: System::Windows::Forms::TextBox ^  textBoxShapeAttribut;
	private: System::Windows::Forms::Label ^  label1;
	private: System::Windows::Forms::Label ^  label2;
	private: System::Windows::Forms::Label ^  labelDateiAttribut;
	private: System::Windows::Forms::Button ^  buttonAbbrechen;
	private: System::Windows::Forms::GroupBox ^  groupBox1;
	private: System::Windows::Forms::ComboBox ^  comboBoxRaeumlicherGeltungsbereich;
	private: System::Windows::Forms::ComboBox ^  comboBoxSRS;
	private: System::Windows::Forms::Label ^  label4;
	private: System::Windows::Forms::RadioButton ^  radioButtonBereiche;
	private: System::Windows::Forms::CheckBox ^ checkBoxAlleKlassen;
	private: System::Windows::Forms::CheckBox ^ checkBoxPlaeneAusShape;

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
      this->radioButtonTextlicheFestsetzungen = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonPlanAttribute = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonShapeDatei = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonKonversionAufzaehlung = (gcnew System::Windows::Forms::RadioButton());
      this->comboBoxShapeDateien = (gcnew System::Windows::Forms::ComboBox());
      this->labelShapeDatei = (gcnew System::Windows::Forms::Label());
      this->comboBoxGmlKlasse = (gcnew System::Windows::Forms::ComboBox());
      this->LabelGmlKlasse = (gcnew System::Windows::Forms::Label());
      this->comboBoxGmlEnumerationen = (gcnew System::Windows::Forms::ComboBox());
      this->labelGmlEnumeration = (gcnew System::Windows::Forms::Label());
      this->buttonWeiter = (gcnew System::Windows::Forms::Button());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->textBoxShapeDatei = (gcnew System::Windows::Forms::TextBox());
      this->textBoxShapeAttribut = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
      this->checkBoxPlaeneAusShape = (gcnew System::Windows::Forms::CheckBox());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->comboBoxSRS = (gcnew System::Windows::Forms::ComboBox());
      this->labelDateiAttribut = (gcnew System::Windows::Forms::Label());
      this->comboBoxRaeumlicherGeltungsbereich = (gcnew System::Windows::Forms::ComboBox());
      this->radioButtonBereiche = (gcnew System::Windows::Forms::RadioButton());
      this->checkBoxAlleKlassen = (gcnew System::Windows::Forms::CheckBox());
      this->groupBox1->SuspendLayout();
      this->SuspendLayout();
      // 
      // radioButtonTextlicheFestsetzungen
      // 
      this->radioButtonTextlicheFestsetzungen->Location = System::Drawing::Point(16, 32);
      this->radioButtonTextlicheFestsetzungen->Name = L"radioButtonTextlicheFestsetzungen";
      this->radioButtonTextlicheFestsetzungen->Size = System::Drawing::Size(144, 24);
      this->radioButtonTextlicheFestsetzungen->TabIndex = 0;
      this->radioButtonTextlicheFestsetzungen->Text = L"Text Abschnitte";
      this->radioButtonTextlicheFestsetzungen->CheckedChanged += gcnew System::EventHandler(this, &GenerateKonversion::radioButtonTextlicheFestsetzungen_CheckedChanged);
      // 
      // radioButtonPlanAttribute
      // 
      this->radioButtonPlanAttribute->Location = System::Drawing::Point(16, 54);
      this->radioButtonPlanAttribute->Name = L"radioButtonPlanAttribute";
      this->radioButtonPlanAttribute->Size = System::Drawing::Size(135, 23);
      this->radioButtonPlanAttribute->TabIndex = 2;
      this->radioButtonPlanAttribute->Text = L"Sonstige Plan-Attribute";
      this->radioButtonPlanAttribute->CheckedChanged += gcnew System::EventHandler(this, &GenerateKonversion::radioButtonPlanAttribute_CheckedChanged);
      // 
      // radioButtonShapeDatei
      // 
      this->radioButtonShapeDatei->Location = System::Drawing::Point(16, 174);
      this->radioButtonShapeDatei->Name = L"radioButtonShapeDatei";
      this->radioButtonShapeDatei->Size = System::Drawing::Size(152, 24);
      this->radioButtonShapeDatei->TabIndex = 3;
      this->radioButtonShapeDatei->Text = L"Konversion Shape-Datei";
      this->radioButtonShapeDatei->CheckedChanged += gcnew System::EventHandler(this, &GenerateKonversion::radioButtonShapeDatei_CheckedChanged);
      // 
      // radioButtonKonversionAufzaehlung
      // 
      this->radioButtonKonversionAufzaehlung->Location = System::Drawing::Point(16, 206);
      this->radioButtonKonversionAufzaehlung->Name = L"radioButtonKonversionAufzaehlung";
      this->radioButtonKonversionAufzaehlung->Size = System::Drawing::Size(144, 24);
      this->radioButtonKonversionAufzaehlung->TabIndex = 4;
      this->radioButtonKonversionAufzaehlung->Text = L"Konversion Aufzählung";
      this->radioButtonKonversionAufzaehlung->CheckedChanged += gcnew System::EventHandler(this, &GenerateKonversion::radioButtonKonversionAufzaehlung_CheckedChanged);
      // 
      // comboBoxShapeDateien
      // 
      this->comboBoxShapeDateien->Location = System::Drawing::Point(168, 174);
      this->comboBoxShapeDateien->Name = L"comboBoxShapeDateien";
      this->comboBoxShapeDateien->Size = System::Drawing::Size(200, 21);
      this->comboBoxShapeDateien->Sorted = true;
      this->comboBoxShapeDateien->TabIndex = 5;
      this->comboBoxShapeDateien->SelectedIndexChanged += gcnew System::EventHandler(this, &GenerateKonversion::comboBoxShapeDateien_SelectedIndexChanged);
      // 
      // labelShapeDatei
      // 
      this->labelShapeDatei->Location = System::Drawing::Point(168, 278);
      this->labelShapeDatei->Name = L"labelShapeDatei";
      this->labelShapeDatei->Size = System::Drawing::Size(100, 16);
      this->labelShapeDatei->TabIndex = 6;
      this->labelShapeDatei->Text = L"Shape-Datei";
      // 
      // comboBoxGmlKlasse
      // 
      this->comboBoxGmlKlasse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->comboBoxGmlKlasse->Location = System::Drawing::Point(376, 174);
      this->comboBoxGmlKlasse->Name = L"comboBoxGmlKlasse";
      this->comboBoxGmlKlasse->Size = System::Drawing::Size(317, 21);
      this->comboBoxGmlKlasse->TabIndex = 7;
      this->comboBoxGmlKlasse->SelectedIndexChanged += gcnew System::EventHandler(this, &GenerateKonversion::comboBoxGmlKlasse_SelectedIndexChanged);
      // 
      // LabelGmlKlasse
      // 
      this->LabelGmlKlasse->Location = System::Drawing::Point(376, 198);
      this->LabelGmlKlasse->Name = L"LabelGmlKlasse";
      this->LabelGmlKlasse->Size = System::Drawing::Size(100, 16);
      this->LabelGmlKlasse->TabIndex = 8;
      this->LabelGmlKlasse->Text = L"GML-Klasse";
      // 
      // comboBoxGmlEnumerationen
      // 
      this->comboBoxGmlEnumerationen->Location = System::Drawing::Point(168, 214);
      this->comboBoxGmlEnumerationen->Name = L"comboBoxGmlEnumerationen";
      this->comboBoxGmlEnumerationen->Size = System::Drawing::Size(328, 21);
      this->comboBoxGmlEnumerationen->TabIndex = 9;
      this->comboBoxGmlEnumerationen->SelectedIndexChanged += gcnew System::EventHandler(this, &GenerateKonversion::comboBoxGmlEnumerationen_SelectedIndexChanged);
      // 
      // labelGmlEnumeration
      // 
      this->labelGmlEnumeration->Location = System::Drawing::Point(168, 238);
      this->labelGmlEnumeration->Name = L"labelGmlEnumeration";
      this->labelGmlEnumeration->Size = System::Drawing::Size(128, 16);
      this->labelGmlEnumeration->TabIndex = 10;
      this->labelGmlEnumeration->Text = L"GML-Enumeration";
      // 
      // buttonWeiter
      // 
      this->buttonWeiter->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonWeiter->Location = System::Drawing::Point(621, 16);
      this->buttonWeiter->Name = L"buttonWeiter";
      this->buttonWeiter->Size = System::Drawing::Size(75, 48);
      this->buttonWeiter->TabIndex = 11;
      this->buttonWeiter->Text = L"Weiter ->";
      this->buttonWeiter->Click += gcnew System::EventHandler(this, &GenerateKonversion::buttonWeiter_Click);
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonBeenden->Location = System::Drawing::Point(614, 286);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 12;
      this->buttonBeenden->Text = L"Beenden";
      // 
      // textBoxShapeDatei
      // 
      this->textBoxShapeDatei->Location = System::Drawing::Point(168, 254);
      this->textBoxShapeDatei->Name = L"textBoxShapeDatei";
      this->textBoxShapeDatei->ReadOnly = true;
      this->textBoxShapeDatei->Size = System::Drawing::Size(200, 20);
      this->textBoxShapeDatei->TabIndex = 14;
      // 
      // textBoxShapeAttribut
      // 
      this->textBoxShapeAttribut->Location = System::Drawing::Point(376, 254);
      this->textBoxShapeAttribut->Name = L"textBoxShapeAttribut";
      this->textBoxShapeAttribut->ReadOnly = true;
      this->textBoxShapeAttribut->Size = System::Drawing::Size(120, 20);
      this->textBoxShapeAttribut->TabIndex = 15;
      // 
      // label1
      // 
      this->label1->Location = System::Drawing::Point(168, 198);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(100, 16);
      this->label1->TabIndex = 16;
      this->label1->Text = L"Shape-Datei";
      // 
      // label2
      // 
      this->label2->Location = System::Drawing::Point(376, 278);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(40, 16);
      this->label2->TabIndex = 17;
      this->label2->Text = L"Atribut";
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonAbbrechen->Location = System::Drawing::Point(493, 286);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 23);
      this->buttonAbbrechen->TabIndex = 18;
      this->buttonAbbrechen->Text = L"Abbrechen";
      // 
      // groupBox1
      // 
      this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBox1->Controls->Add(this->checkBoxPlaeneAusShape);
      this->groupBox1->Controls->Add(this->label4);
      this->groupBox1->Controls->Add(this->comboBoxSRS);
      this->groupBox1->Controls->Add(this->labelDateiAttribut);
      this->groupBox1->Controls->Add(this->comboBoxRaeumlicherGeltungsbereich);
      this->groupBox1->Location = System::Drawing::Point(0, 8);
      this->groupBox1->Name = L"groupBox1";
      this->groupBox1->Size = System::Drawing::Size(605, 136);
      this->groupBox1->TabIndex = 19;
      this->groupBox1->TabStop = false;
      this->groupBox1->Text = L"Attribute des Gesamtplans";
      // 
      // checkBoxPlaeneAusShape
      // 
      this->checkBoxPlaeneAusShape->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->checkBoxPlaeneAusShape->Location = System::Drawing::Point(493, 24);
      this->checkBoxPlaeneAusShape->Name = L"checkBoxPlaeneAusShape";
      this->checkBoxPlaeneAusShape->Size = System::Drawing::Size(104, 32);
      this->checkBoxPlaeneAusShape->TabIndex = 6;
      this->checkBoxPlaeneAusShape->Text = L"Pläne aus Shape erzeugen";
      this->checkBoxPlaeneAusShape->CheckedChanged += gcnew System::EventHandler(this, &GenerateKonversion::checkBoxPlaeneAusShape_CheckedChanged);
      // 
      // label4
      // 
      this->label4->Location = System::Drawing::Point(165, 96);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(248, 16);
      this->label4->TabIndex = 5;
      this->label4->Text = L"Koordinaten-Referenzsystem";
      // 
      // comboBoxSRS
      // 
      this->comboBoxSRS->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->comboBoxSRS->Location = System::Drawing::Point(168, 72);
      this->comboBoxSRS->Name = L"comboBoxSRS";
      this->comboBoxSRS->Size = System::Drawing::Size(429, 21);
      this->comboBoxSRS->TabIndex = 4;
      this->comboBoxSRS->SelectedIndexChanged += gcnew System::EventHandler(this, &GenerateKonversion::comboBoxSRS_SelectedIndexChanged);
      // 
      // labelDateiAttribut
      // 
      this->labelDateiAttribut->Location = System::Drawing::Point(168, 48);
      this->labelDateiAttribut->Name = L"labelDateiAttribut";
      this->labelDateiAttribut->Size = System::Drawing::Size(224, 21);
      this->labelDateiAttribut->TabIndex = 1;
      this->labelDateiAttribut->Text = L"Shape-Datei für Räumlichen Geltungsbereich";
      // 
      // comboBoxRaeumlicherGeltungsbereich
      // 
      this->comboBoxRaeumlicherGeltungsbereich->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->comboBoxRaeumlicherGeltungsbereich->Location = System::Drawing::Point(168, 24);
      this->comboBoxRaeumlicherGeltungsbereich->Name = L"comboBoxRaeumlicherGeltungsbereich";
      this->comboBoxRaeumlicherGeltungsbereich->Size = System::Drawing::Size(317, 21);
      this->comboBoxRaeumlicherGeltungsbereich->TabIndex = 0;
      this->comboBoxRaeumlicherGeltungsbereich->Text = L"<Nicht definiert>";
      this->comboBoxRaeumlicherGeltungsbereich->SelectedIndexChanged += gcnew System::EventHandler(this, &GenerateKonversion::comboBoxRaeumlicherGeltungsbereich_SelectedIndexChanged);
      // 
      // radioButtonBereiche
      // 
      this->radioButtonBereiche->Location = System::Drawing::Point(16, 76);
      this->radioButtonBereiche->Name = L"radioButtonBereiche";
      this->radioButtonBereiche->Size = System::Drawing::Size(104, 24);
      this->radioButtonBereiche->TabIndex = 2;
      this->radioButtonBereiche->Text = L"Plan-Bereiche";
      this->radioButtonBereiche->CheckedChanged += gcnew System::EventHandler(this, &GenerateKonversion::radioButtonBereiche_CheckedChanged);
      // 
      // checkBoxAlleKlassen
      // 
      this->checkBoxAlleKlassen->Location = System::Drawing::Point(458, 150);
      this->checkBoxAlleKlassen->Name = L"checkBoxAlleKlassen";
      this->checkBoxAlleKlassen->Size = System::Drawing::Size(166, 24);
      this->checkBoxAlleKlassen->TabIndex = 20;
      this->checkBoxAlleKlassen->Text = L"Nachrichtliche Übernahme";
      this->checkBoxAlleKlassen->CheckedChanged += gcnew System::EventHandler(this, &GenerateKonversion::checkBoxAlleKlassen_CheckedChanged);
      // 
      // GenerateKonversion
      // 
      this->AcceptButton = this->buttonBeenden;
      this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
      this->CancelButton = this->buttonAbbrechen;
      this->ClientSize = System::Drawing::Size(701, 333);
      this->Controls->Add(this->checkBoxAlleKlassen);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->textBoxShapeAttribut);
      this->Controls->Add(this->textBoxShapeDatei);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->buttonWeiter);
      this->Controls->Add(this->labelGmlEnumeration);
      this->Controls->Add(this->comboBoxGmlEnumerationen);
      this->Controls->Add(this->LabelGmlKlasse);
      this->Controls->Add(this->comboBoxGmlKlasse);
      this->Controls->Add(this->labelShapeDatei);
      this->Controls->Add(this->comboBoxShapeDateien);
      this->Controls->Add(this->radioButtonKonversionAufzaehlung);
      this->Controls->Add(this->radioButtonShapeDatei);
      this->Controls->Add(this->radioButtonTextlicheFestsetzungen);
      this->Controls->Add(this->radioButtonBereiche);
      this->Controls->Add(this->radioButtonPlanAttribute);
      this->Controls->Add(this->groupBox1);
      this->Location = System::Drawing::Point(544, 160);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"GenerateKonversion";
      this->Text = L"Konversion von Shape-Dateien";
      this->groupBox1->ResumeLayout(false);
      this->ResumeLayout(false);
      this->PerformLayout();

    }		

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl einer Shape-Datei über ComboBox Shape-Datei			 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxShapeDateien_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	String			     ^ STR;
	string			       stdString;

	STR = dynamic_cast<String^> ( comboBoxShapeDateien->SelectedItem );
	QuConvert::systemStr2stdStr ( stdString, STR );
	pShapeFileObjektActive = m_pShapeFileObjekte->getShapeFileObjekt ( stdString );
  pGmlKlasseActive = NULL;

  if ( pShapeFileObjektActive->getRechtlicheBedeutung() == NACHR_UEBERNAHME                      ||
        m_pGmlObjekte->getGmlTyp() == XPLANGML_5_0 || m_pGmlObjekte->getGmlTyp() == XPLANGML_5_1 ||
        m_pGmlObjekte->getGmlTyp() == XPLANGML_5_2  )
  {
    if ( checkBoxAlleKlassen->Checked == true )
      updateComboBoxGmlKlassen();
    else
      checkBoxAlleKlassen->Checked = true;
  }
  else
  {
    if ( checkBoxAlleKlassen->Checked == false )
      updateComboBoxGmlKlassen();
    else
      checkBoxAlleKlassen->Checked = false;
  }

  pGmlKlasseActive = pShapeFileObjektActive->getGmlKlasse();
	if ( pGmlKlasseActive != NULL )
	{
		stdString = pGmlKlasseActive->getGmlElementName();
		STR = QuConvert::ToString ( stdString);
		comboBoxGmlKlasse->SelectedItem = STR;
	}
	else
		comboBoxGmlKlasse->SelectedIndex = 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl einer zugeordneten XPlanGML-Klasse über   			 //
//  ComboBox GML-Klasse																											 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxGmlKlasse_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	String              ^ STR;
	string                stdString;
	string                defaultWert;
	vector<GmlAttribut*>  gmlAttribute;
	GmlAttribut			    * pGmlAttribut;
	size_t			          i, anz;

	if ( comboBoxGmlKlasse->SelectedIndex == 0 )
	{
		pGmlKlasseActive = NULL;
		pShapeFileObjektActive->setGmlKlasse ( NULL );
		return;
	}

	STR = dynamic_cast<String^> ( comboBoxGmlKlasse->SelectedItem );
	QuConvert::systemStr2stdStr ( stdString, STR );
	pGmlKlasseActive = m_pGmlObjekte->getGmlKlasseFromElementName ( stdString );
	
	pShapeFileObjektActive->setGmlKlasse ( pGmlKlasseActive );

	anz = pGmlKlasseActive->getGmlAttribute ( gmlAttribute );
	for ( i = 0; i < anz; i++ )
	{
		pGmlAttribut = gmlAttribute[i];
		defaultWert = pGmlAttribut->getDefaultWert();
		if ( defaultWert  != "" && pGmlAttribut->getOptional() == false )
		{
			StandardAttribut * pStandardAttribut = pShapeFileObjektActive->getStandardAttribut( pGmlAttribut );
			if ( pStandardAttribut == NULL )
			{
				pStandardAttribut = new StandardAttribut ( pGmlAttribut );
				pStandardAttribut->setDefaultWert ( defaultWert );
				pShapeFileObjektActive->addStandardAttribut ( pStandardAttribut );
			}
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl einer Enumeration über ComboBox GML-Enumeration	 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxGmlEnumerationen_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	string   stdString;
	String ^ STR;

	vector<GmlEnumeration*>    gmlEnumerations;
	vector<ShapeFileObjekt*>   shapeFiles;
	vector<ShapeFileAttribut*> shapeFileAttribute;

	size_t anz = m_pShapeFileObjekte->getUsedEnums ( gmlEnumerations, shapeFiles, shapeFileAttribute );

	int index = comboBoxGmlEnumerationen->SelectedIndex;
	if ( index >= 0 )
	{
		pGmlEnumerationActive			 = gmlEnumerations[index];
		pShapeFileObjektFromEnumActive   = shapeFiles[index];
		pShapeFileAttributFromEnumActive = shapeFileAttribute[index];

		stdString = pShapeFileObjektFromEnumActive->getShapeFileName();
		STR = QuConvert::ToString ( stdString );
		textBoxShapeDatei->Text = STR;

		stdString = pShapeFileAttributFromEnumActive->getShapeFileAttributName();
		STR = QuConvert::ToString ( stdString );
		textBoxShapeAttribut->Text = STR;
	}
	else
	{
		pGmlEnumerationActive			 = NULL;
		pShapeFileObjektFromEnumActive   = NULL;
		pShapeFileAttributFromEnumActive = NULL;
	}

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung des RadioButtons "Text-Abschnitte"  						 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonTextlicheFestsetzungen_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	if ( radioButtonTextlicheFestsetzungen->Checked )
	{
		operation = OPERATION::TEXTUELLE_FESTSETZUNGEN;

		comboBoxShapeDateien->Enabled = false;
		comboBoxGmlKlasse->Enabled = false;
		comboBoxGmlEnumerationen->Enabled = false;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung des RadioButtons "Sonstige Plan-Attribute			 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonPlanAttribute_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	if ( radioButtonPlanAttribute->Checked )
	{
		operation = OPERATION::PLAN_ATTRIBUTE;

		comboBoxShapeDateien->Enabled = false;
		comboBoxGmlKlasse->Enabled = false;
		comboBoxGmlEnumerationen->Enabled = false;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung des RadioButtons "Konversion Shape-Datei"			 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonShapeDatei_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	if ( radioButtonShapeDatei->Checked )
	{
		operation = OPERATION::SHAPEFILE;

		comboBoxShapeDateien->Enabled = true;
		comboBoxGmlKlasse->Enabled = true;
		comboBoxGmlEnumerationen->Enabled = false;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung des RadioButtons "Plan-Bereiche"								 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonBereiche_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	if ( radioButtonBereiche->Checked )
	{
		operation = OPERATION::PLAN_BEREICHE;

		comboBoxShapeDateien->Enabled = false;
		comboBoxGmlKlasse->Enabled = false;
		comboBoxGmlEnumerationen->Enabled = false;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung des RadioButtons "Konversion Aufzählung"				 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonKonversionAufzaehlung_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	size_t				             i, anz;
	int												 suffix;
	string					           stdString;
	String					         ^ STR;
	vector<GmlEnumeration*>    gmlEnumerations;
	vector<ShapeFileObjekt*>   shapeFiles;
	vector<ShapeFileAttribut*> shapeFileAttribute;
  set<string>                usedEnums;
  char                       buffer[3];

	if ( !radioButtonKonversionAufzaehlung->Checked )
		return;
  
  suffix = 0;

	operation = OPERATION::ENUMERATION;

	comboBoxShapeDateien->Enabled = false;
	comboBoxGmlKlasse->Enabled = false;
	comboBoxGmlEnumerationen->Enabled = true;

	comboBoxGmlEnumerationen->Items->Clear();

	m_pShapeFileObjekte->initEnumConversion ( m_pGmlObjekte );
	anz = m_pShapeFileObjekte->getUsedEnums ( gmlEnumerations, shapeFiles, shapeFileAttribute );
	
	for ( i = 0; i < anz; i++ )
	{
		GmlEnumeration * pEnumeration = gmlEnumerations[i];
		stdString = pEnumeration->getGmlEnumerationName ();
    if ( usedEnums.find( stdString ) == usedEnums.end() )
      usedEnums.insert ( stdString );
    else
    {
      suffix++;
      _itoa_s ( suffix, buffer, 3, 10 );
      stdString = stdString + "_" + string ( buffer);
    }
		STR = QuConvert::ToString ( stdString );
		comboBoxGmlEnumerationen->Items->Add ( STR );
	}

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Weiter"																					 //
//  Öffnet je nach ausgewählter Operation einen neuen Dialog							   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonWeiter_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	switch ( operation )
	{
	case OPERATION::TEXTUELLE_FESTSETZUNGEN:
		{
			PlanAttribute * pPlanAttribute = m_pXPlanGML->getPlanAttribute();
			TextlicheFestsetzungen ^ dialog = gcnew TextlicheFestsetzungen ( pPlanAttribute, m_pXPlanGML->getPlanTyp() );
			dialog->ShowDialog();

			break;
		}

	case OPERATION::PLAN_BEREICHE:
		{
			PlanAttribute * pPlanAttribute = m_pXPlanGML->getPlanAttribute();
			PlanBereiche ^ pDialog = gcnew PlanBereiche ( m_pXPlanGML, m_pCodeLists );
			pDialog->ShowDialog();

			break;
		}

	case OPERATION::PLAN_ATTRIBUTE:
		{
			PlanAttributeNeu ^ dialog 
				= gcnew PlanAttributeNeu ( m_pXPlanGML, m_pCodeLists, m_pExternalCodeLists );
			dialog->ShowDialog();			
			break;
		}

	case OPERATION::SHAPEFILE:
		{
			if ( pShapeFileObjektActive == NULL )
				System::Windows::Forms::MessageBox::Show ( "Keine Shape-Datei ausgewählt" );
			else
			if ( pGmlKlasseActive == NULL )
				System::Windows::Forms::MessageBox::Show ( "Keine GML-Klasse zugeordnet" );
			else
			{
				PlanAttribute * pPlanAttribute = m_pXPlanGML->getPlanAttribute();
				ShapeFileConversion ^ dialog = gcnew ShapeFileConversion
					(  m_pXPlanGML, pShapeFileObjektActive, m_pCodeLists, m_pExternalCodeLists, 
					  pPlanAttribute, m_pShapeFileObjekte->getPlaeneAusShapeFileErzeugen(), checkBoxAlleKlassen->Checked );
				dialog->ShowDialog();
			}

			break;
		}

	case OPERATION::ENUMERATION:
		{
			GmlDictionary * pDictionary;

			if ( pGmlEnumerationActive == NULL )
				System::Windows::Forms::MessageBox::Show ( "Keine Enumeration ausgewählt" );
			else
				if ( pGmlEnumerationActive->getIsExternalCodeList() )
					pDictionary = m_pExternalCodeLists;
				else
					pDictionary = m_pCodeLists;

			if ( pGmlEnumerationActive != NULL )
			{
				EnumerationConversion ^ dialog =
					gcnew EnumerationConversion ( m_pXPlanGML->getFeatures(), pShapeFileObjektFromEnumActive, pGmlEnumerationActive, 
												pShapeFileAttributFromEnumActive,	pDictionary ); 

				dialog->ShowDialog();
			}

			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Dialogbox im Konstruktor															 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void initialize()
{
	size_t  			           i, anz;
	size_t 									 index;
	vector<ShapeFileObjekt*> shapeFileObjekte;
	string					         stdString;
	string                   planKlassenName;
	string                   srsNameShort;
  string                   srsNameFull;
	string                   srsNameAktuell;
	string                   srsNameBeschreibung;
	String				         ^ STR;
  String                 ^ SRS_NAME_FULL;
	String                 ^ SHAPE_FILE_RAEUMLICHER_GELTUNGSBEREICH;
	GmlKlasse              * pGmlKlasse;
  GML_SCHEMA_TYPE          typ;
  vector<CoordSystem*>     vCoordSystems;

	operation			   = OPERATION::SHAPEFILE;
	pShapeFileObjektActive = NULL;
	pGmlKlasseActive	   = NULL;
	pPlanKlasse            = NULL;

	pGmlEnumerationActive			 = NULL;
	pShapeFileObjektFromEnumActive	 = NULL;
	pShapeFileAttributFromEnumActive = NULL;

	SHAPE_FILE_RAEUMLICHER_GELTUNGSBEREICH = nullptr;

  typ = m_pGmlObjekte->getGmlTyp();
	STR = Text;

	int planTyp = m_pXPlanGML->getPlanTyp();
	if ( planTyp == BPLAN )
	{
		STR = String::Concat ( STR, ": Bebauungsplan" );
    planKlassenName = "xplan:BP_Plan";
	}
	else
	if ( planTyp == FPLAN )
	{
		STR = String::Concat ( STR, ": Flächennutzungsplan" );
    planKlassenName = "xplan:FP_Plan";
	}
	else
	if ( planTyp == RPLAN )
	{
		STR = String::Concat ( STR, ": Regionalplan" );
    planKlassenName = "xplan:RP_Plan";
	}
	else
	if ( planTyp == LPLAN )
	{
		STR = String::Concat ( STR, ": Landschaftsplan" );
    planKlassenName = "xplan:LP_Plan";
	}
  else
  if ( planTyp == SOPLAN )
  {
    STR = String::Concat ( STR, ": Sonstiger Plan" );
    planKlassenName = "xplan:SO_Plan";
  }
		
	pPlanKlasse = m_pGmlObjekte->getGmlKlasseFromElementName ( planKlassenName );
	Text = STR;

  if ( typ <  XPLANGML_4_0 || typ >= XPLANGML_5_0 )
    checkBoxAlleKlassen->Visible = false;

	radioButtonTextlicheFestsetzungen->Checked = false;
	radioButtonPlanAttribute->Checked = false;
	radioButtonShapeDatei->Checked = true;
	radioButtonKonversionAufzaehlung->Checked = false;
	radioButtonBereiche->Checked = false;

	comboBoxGmlEnumerationen->Enabled = false;

	if ( m_pShapeFileObjekte->getPlaeneAusShapeFileErzeugen() == true )
		plaeneAusShapeErzeugen( true );
	else
		plaeneAusShapeErzeugen( false );

	anz = m_pShapeFileObjekte->getShapeFileObjekte ( shapeFileObjekte );
	for ( i = 0; i < anz; i++ )
	{
		ShapeFileObjekt * pShapeFileObjekt = shapeFileObjekte[i];
		stdString = pShapeFileObjekt->getShapeFileName ();
		STR = QuConvert::ToString ( stdString );
		comboBoxShapeDateien->Items->Add ( STR );

		if ( pShapeFileObjekt->getShapeGeometrieTyp() == SHPT_POLYGON ||
			 pShapeFileObjekt->getShapeGeometrieTyp() == SHPT_POLYGON_M ||
			 pShapeFileObjekt->getShapeGeometrieTyp() == SHPT_POLYGON_Z )
		{

			 comboBoxRaeumlicherGeltungsbereich->Items->Add ( STR );
			 pGmlKlasse = pShapeFileObjekt->getGmlKlasse();
			 if ( pGmlKlasse != NULL )
			 {
				 string name = pGmlKlasse->getGmlElementName();
				 if (  name == planKlassenName )
					 SHAPE_FILE_RAEUMLICHER_GELTUNGSBEREICH = STR;
			 }
		}
	}

  anz   = m_pXPlanGML->getFeatures()->getSrsManager()->getCoordSystems( vCoordSystems );
	index = -1;
	srsNameAktuell = m_pXPlanGML->getFeatures()->getSrsName();
	for ( i = 0; i < anz; i++ )
	{
    srsNameShort = vCoordSystems[i]->getSrsNameShort();

    srsNameFull = vCoordSystems[i]->getSrsNameFull();
    SRS_NAME_FULL = QuConvert::ToString( srsNameFull );

    srsNameBeschreibung = vCoordSystems[i]->getDescription();
    srsNameBeschreibung = srsNameBeschreibung + " (" + srsNameFull + ")";
		STR = QuConvert::ToString ( srsNameBeschreibung );

		comboBoxSRS->Items->Add ( STR );
    pCoordSystems->Add( STR, SRS_NAME_FULL );
		if ( srsNameAktuell == srsNameFull ) index = i;
	}
	comboBoxSRS->SelectedIndex = (int)index;

	if ( SHAPE_FILE_RAEUMLICHER_GELTUNGSBEREICH != nullptr )
		comboBoxRaeumlicherGeltungsbereich->SelectedItem = SHAPE_FILE_RAEUMLICHER_GELTUNGSBEREICH;

  // Standard-Codelisten einlesen für XPlanGML 4,0 oder 4.1
  if ( m_pGmlObjekte->getGmlTyp() >= XPLANGML_4_0 )
  {
    GmlDictionaryEntry        * pEntry;
    vector<GmlDictionaryEntry*> vEntries;

    m_pExternalCodeLists = new GmlDictionary;
    string folder = m_pXPlanGML->getFeatures()->getExternalCodeListsFolder( m_pGmlObjekte->getGmlTyp() );

    String ^ CODE_LISTS_FOLDER = QuConvert::ToString ( folder );
    if ( Directory::Exists ( CODE_LISTS_FOLDER ) )
    {
      string         codeListDatei;
      cli::array<String^> ^ FILES = Directory::GetFiles ( CODE_LISTS_FOLDER );
      System::Collections::IEnumerator    ^ FILES_ENUM = FILES->GetEnumerator();

      GmlDictionaryReader ^ pDictionaryReader = gcnew GmlDictionaryReader ( m_pGmlObjekte->getGmlTyp() );

      while ( FILES_ENUM->MoveNext() )
      {
        String^ FILE_NAME = safe_cast<String^>(FILES_ENUM->Current);
        FILE_NAME = FILE_NAME->ToUpper();

        QuConvert::systemStr2stdStr ( codeListDatei, FILE_NAME );
        index = codeListDatei.find_last_of( "." );
        string extension = codeListDatei.substr( index+1, codeListDatei.length() );
        if ( extension == "XML" )
          pDictionaryReader->readGmlSingleDictionary ( codeListDatei, m_pExternalCodeLists, false );
      }

      anz = m_pExternalCodeLists->getDictionaryEntries( vEntries );
      for ( i = 0; i < anz; i++ )
      {
        pEntry = vEntries[i]; 
        m_pGmlObjekte->generateEnumerationFromExternalCodeList ( pEntry );
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////
//  Steuert die Verfügbarkeit bestimmter Elements der Dialogbox in						 //
//  Abhängigkeit davon, ob ein einziges XPlanGML-Planobjekt (erzeugen == false)//
//  oder mehrere Planobjekte erzeugt werden sollen (erzeugen == true)					 //
/////////////////////////////////////////////////////////////////////////////////
private: System::Void plaeneAusShapeErzeugen( bool erzeugen )
{
	if ( erzeugen == true )
	{
		radioButtonTextlicheFestsetzungen->Enabled = false;
		radioButtonPlanAttribute->Enabled = false;
		radioButtonBereiche->Enabled = false;
		
		comboBoxRaeumlicherGeltungsbereich->Enabled = false;
		checkBoxPlaeneAusShape->Checked = true;
	}
	else
	{
		radioButtonTextlicheFestsetzungen->Enabled = true;
		radioButtonPlanAttribute->Enabled = true;
		radioButtonBereiche->Enabled = true;
		
		comboBoxRaeumlicherGeltungsbereich->Enabled = true;
		checkBoxPlaeneAusShape->Checked = false;
	}
}
				 				 				
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler ComboBox "Shape-Datei für Räumlichen Geltungsbereich      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxRaeumlicherGeltungsbereich_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	String				          ^ STR;
	string                    stdString;
	vector<ShapeFileObjekt*>  shapeFileObjekte;
	ShapeFileObjekt         * pShapeFileObjektRaeumlGeltungsbereich;

	STR =  dynamic_cast<String^> ( comboBoxRaeumlicherGeltungsbereich->SelectedItem );
	QuConvert::systemStr2stdStr ( stdString, STR );
	pShapeFileObjektRaeumlGeltungsbereich = m_pShapeFileObjekte->getShapeFileObjekt ( stdString );

	pShapeFileObjektRaeumlGeltungsbereich->setGmlKlasse ( pPlanKlasse );
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler ComboBox "Koordinaten-Referenzsystem"									   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxSRS_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
  String          ^ SRS_DESCRIPTION;
  String          ^ SRS_NAME;
	string			      srsName;
	int				        index;

  SRS_DESCRIPTION = dynamic_cast<String^>(comboBoxSRS->SelectedItem);
  if ( SRS_DESCRIPTION != nullptr )
  {
    index = pCoordSystems->IndexOfKey ( SRS_DESCRIPTION );
    if ( index > 0 )
    {
      SRS_NAME = dynamic_cast<String^>(pCoordSystems->GetByIndex( index ));
      QuConvert::systemStr2stdStr( srsName, SRS_NAME );
      m_pXPlanGML->getFeatures()->setSrsName ( srsName );
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Änderung CheckBox "Nachrichtliche Übernahme"							 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void checkBoxAlleKlassen_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e)
{
  string stdString;
  String ^ STR;

  if ( pShapeFileObjektActive == NULL ) return;
  
  if ( checkBoxAlleKlassen->Checked == true )
    pShapeFileObjektActive->setRechtlichebedeutung(NACHR_UEBERNAHME);
  else
    pShapeFileObjektActive->setRechtlichebedeutung(PLANINHALT);

  updateComboBoxGmlKlassen();

  if ( pGmlKlasseActive != NULL )
  {
    stdString = pGmlKlasseActive->getGmlElementName();
    STR = QuConvert::ToString( stdString );
    comboBoxGmlKlasse->SelectedIndex = 0;
    comboBoxGmlKlasse->SelectedItem = STR;
    if ( comboBoxGmlKlasse->SelectedIndex == 0 )
    {
      pGmlKlasseActive = NULL;
      pShapeFileObjektActive->setGmlKlasse ( NULL );
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
// Füllt die ComboBox "GML-Klassen" in Abhängigkeit vom Typ der aktuell      //
//  gewählten Shape-Datei und der CheckBox "Nachrichtliche Übernahme"				 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void updateComboBoxGmlKlassen()
{
	size_t             i, anz;
  string             stdString;
  String           ^ STR;
  vector<GmlKlasse*> gmlObjekte;
  int				         planTyp = m_pXPlanGML->getPlanTyp();

  comboBoxGmlKlasse->Items->Clear();
  comboBoxGmlKlasse->Items->Add ( "Nicht zugeordnet" );

  SHPTypeEnum typ = pShapeFileObjektActive->getShapeGeometrieTyp ( );
  if ( typ == SHPT_POINT || typ == SHPT_POINT_M || typ == SHPT_POINT_Z || 
    typ == SHPT_MULTIPOINT || typ == SHPT_MULTIPOINT_Z || typ == SHPT_MULTIPOINT_M )
  {
    anz = m_pGmlObjekte->getGmlPunktObjekte ( gmlObjekte, planTyp, checkBoxAlleKlassen->Checked );
  }
  else
    if ( typ == SHPT_ARC || typ == SHPT_ARC_Z || typ == SHPT_ARC_M )
    {
      anz = m_pGmlObjekte->getGmlLinienObjekte ( gmlObjekte, planTyp, checkBoxAlleKlassen->Checked );
    }
    else
      if ( typ == SHPT_POLYGON || typ == SHPT_POLYGON_Z || typ == SHPT_POLYGON_M )
      {
        anz = m_pGmlObjekte->getGmlFlaechenObjekte ( gmlObjekte, planTyp, checkBoxAlleKlassen->Checked );
      }

      for ( i = 0; i < anz; i++ )
      {
        GmlKlasse * pGmlKlasse = gmlObjekte[i];
        if ( !pGmlKlasse->getAbstract() )
        {
          stdString = pGmlKlasse->getGmlElementName();
          STR = QuConvert::ToString ( stdString );
          comboBoxGmlKlasse->Items->Add ( STR );
        }
      }
         
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Änderung CheckBox "Pläne aus Shape erzeugen"							 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void checkBoxPlaeneAusShape_CheckedChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	if ( checkBoxPlaeneAusShape->Checked == true )
	{
		m_pShapeFileObjekte->setPlaeneAusShapeFileErzeugen( true );
		plaeneAusShapeErzeugen( true );
	}
	else
	{
		m_pShapeFileObjekte->setPlaeneAusShapeFileErzeugen( false );
		plaeneAusShapeErzeugen( false );
	}

}

};
