#pragma once
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <vector>
#include "ShpFileNamenKonv.h"
#include "Convert.h"
#include "includes.h"

using namespace std;


namespace XPlanGMLToolbox
{
	/// <summary> 
	/// Zusammenfassung für GenerischePlanAttribute
	///
	/// Achtung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie die Eigenschaft
	///          'Ressourcendateiname' für das Compilertool für verwaltete Ressourcen ändern, 
	///          das allen .resx-Dateien zugewiesen ist, von denen diese Klasse abhängt. 
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class GenerischePlanAttribute : public System::Windows::Forms::Form
	{
	public: 
		GenerischePlanAttribute( PlanAttribute * pPlanAttributeP )
		{
			InitializeComponent();

			pPlanAttribute   = pPlanAttributeP;
			attributTyp      = 0;
			attributSelected = -1;

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
	private: System::Windows::Forms::ComboBox ^  comboBoxAttributTyp;

	private: System::Windows::Forms::TextBox ^  textBoxAttributWert;
	private: System::Windows::Forms::Label ^  label1;
	private: System::Windows::Forms::Label ^  label2;
	private: System::Windows::Forms::Label ^  label3;
	private: System::Windows::Forms::Button ^  buttonNeuesAttribut;
	private: System::Windows::Forms::Button ^  buttonAttributUebernehmen;
	private: System::Windows::Forms::Button ^  buttonAttributLoeschen;
	private: System::Windows::Forms::ListBox ^  listBoxAttribute;
	private: System::Windows::Forms::TextBox ^  textBoxAttributName;
	private: System::Windows::Forms::Button ^  buttonBeenden;

	private:
		PlanAttribute * pPlanAttribute;
		int             attributTyp;
		int             attributSelected;

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
      this->comboBoxAttributTyp = (gcnew System::Windows::Forms::ComboBox());
      this->textBoxAttributWert = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->buttonNeuesAttribut = (gcnew System::Windows::Forms::Button());
      this->buttonAttributUebernehmen = (gcnew System::Windows::Forms::Button());
      this->buttonAttributLoeschen = (gcnew System::Windows::Forms::Button());
      this->listBoxAttribute = (gcnew System::Windows::Forms::ListBox());
      this->textBoxAttributName = (gcnew System::Windows::Forms::TextBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // comboBoxAttributTyp
      // 
      this->comboBoxAttributTyp->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"String", L"Integer", L"Double", L"Datum", 
        L"URL"});
      this->comboBoxAttributTyp->Location = System::Drawing::Point(16, 16);
      this->comboBoxAttributTyp->Name = L"comboBoxAttributTyp";
      this->comboBoxAttributTyp->Size = System::Drawing::Size(121, 21);
      this->comboBoxAttributTyp->TabIndex = 0;
      this->comboBoxAttributTyp->SelectedIndexChanged += gcnew System::EventHandler(this, &GenerischePlanAttribute::comboBoxAttributTyp_SelectedIndexChanged);
      // 
      // textBoxAttributWert
      // 
      this->textBoxAttributWert->Location = System::Drawing::Point(160, 64);
      this->textBoxAttributWert->Name = L"textBoxAttributWert";
      this->textBoxAttributWert->Size = System::Drawing::Size(296, 20);
      this->textBoxAttributWert->TabIndex = 2;
      // 
      // label1
      // 
      this->label1->Location = System::Drawing::Point(16, 40);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(100, 23);
      this->label1->TabIndex = 3;
      this->label1->Text = L"Attribut-Typ";
      // 
      // label2
      // 
      this->label2->Location = System::Drawing::Point(160, 40);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(100, 16);
      this->label2->TabIndex = 4;
      this->label2->Text = L"Attribut-Name";
      // 
      // label3
      // 
      this->label3->Location = System::Drawing::Point(160, 88);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(100, 16);
      this->label3->TabIndex = 5;
      this->label3->Text = L"Attribut-Wert";
      // 
      // buttonNeuesAttribut
      // 
      this->buttonNeuesAttribut->Location = System::Drawing::Point(16, 64);
      this->buttonNeuesAttribut->Name = L"buttonNeuesAttribut";
      this->buttonNeuesAttribut->Size = System::Drawing::Size(88, 23);
      this->buttonNeuesAttribut->TabIndex = 6;
      this->buttonNeuesAttribut->Text = L"Neues Attribut";
      this->buttonNeuesAttribut->Click += gcnew System::EventHandler(this, &GenerischePlanAttribute::buttonNeuesAttribut_Click);
      // 
      // buttonAttributUebernehmen
      // 
      this->buttonAttributUebernehmen->Location = System::Drawing::Point(376, 16);
      this->buttonAttributUebernehmen->Name = L"buttonAttributUebernehmen";
      this->buttonAttributUebernehmen->Size = System::Drawing::Size(80, 23);
      this->buttonAttributUebernehmen->TabIndex = 7;
      this->buttonAttributUebernehmen->Text = L"Übernehmen";
      this->buttonAttributUebernehmen->Click += gcnew System::EventHandler(this, &GenerischePlanAttribute::buttonAttributUebernehmen_Click);
      // 
      // buttonAttributLoeschen
      // 
      this->buttonAttributLoeschen->Location = System::Drawing::Point(8, 136);
      this->buttonAttributLoeschen->Name = L"buttonAttributLoeschen";
      this->buttonAttributLoeschen->Size = System::Drawing::Size(352, 23);
      this->buttonAttributLoeschen->TabIndex = 8;
      this->buttonAttributLoeschen->Text = L"Attribut löschen";
      this->buttonAttributLoeschen->Click += gcnew System::EventHandler(this, &GenerischePlanAttribute::buttonAttributLoeschen_Click);
      // 
      // listBoxAttribute
      // 
      this->listBoxAttribute->Location = System::Drawing::Point(8, 176);
      this->listBoxAttribute->Name = L"listBoxAttribute";
      this->listBoxAttribute->Size = System::Drawing::Size(464, 225);
      this->listBoxAttribute->TabIndex = 9;
      this->listBoxAttribute->SelectedIndexChanged += gcnew System::EventHandler(this, &GenerischePlanAttribute::listBoxAttribute_SelectedIndexChanged);
      // 
      // textBoxAttributName
      // 
      this->textBoxAttributName->Location = System::Drawing::Point(160, 16);
      this->textBoxAttributName->Name = L"textBoxAttributName";
      this->textBoxAttributName->Size = System::Drawing::Size(200, 20);
      this->textBoxAttributName->TabIndex = 1;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonBeenden->Location = System::Drawing::Point(368, 136);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(96, 23);
      this->buttonBeenden->TabIndex = 10;
      this->buttonBeenden->Text = L"Beenden";
      // 
      // GenerischePlanAttribute
      // 
      this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
      this->CancelButton = this->buttonBeenden;
      this->ClientSize = System::Drawing::Size(472, 397);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->listBoxAttribute);
      this->Controls->Add(this->buttonAttributLoeschen);
      this->Controls->Add(this->buttonAttributUebernehmen);
      this->Controls->Add(this->buttonNeuesAttribut);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->textBoxAttributWert);
      this->Controls->Add(this->textBoxAttributName);
      this->Controls->Add(this->comboBoxAttributTyp);
      this->Name = L"GenerischePlanAttribute";
      this->Text = L"GenerischePlanAttribute";
      this->ResumeLayout(false);
      this->PerformLayout();

    }		



private: System::Void initialize()
{
	vector<GenerAttribut*> vGenerAttribute;

	comboBoxAttributTyp->SelectedIndex = attributTyp;

	displayAttribute();

	buttonAttributUebernehmen->Enabled = false;
}

private: ATTRIBUT_TYP getAttributTyp ()
{
	switch ( attributTyp )
	{
	case 0: return _GENER_STRING;
	case 1: return _GENER_INTEGER;
	case 2: return _GENER_DOUBLE;
	case 3: return _GENER_DATE;
	case 4: return _GENER_URL;
	}
	 return _GENER_URL;

}


private: System::Void displayAttribute()
{
	vector<GenerAttribut*> generAttribute;
	int                    i, anz;
	String               ^ attributName;
	String               ^ attributWert;
	String               ^ STR;

	listBoxAttribute->Items->Clear();

	anz = pPlanAttribute->getGenerAttribute (  getAttributTyp (), generAttribute );
	for ( i = 0; i < anz; i++ )
	{
		GenerAttribut * pGenerAttribut = generAttribute[i];

		attributName = QuConvert::ToString ( pGenerAttribut->attributName );	
		attributWert = QuConvert::ToString ( pGenerAttribut->getAttributWert() );
		STR = String::Concat ( attributName, ": ", attributWert );

		listBoxAttribute->Items->Add ( STR );
	}
	if ( anz > 0 )
	{
		listBoxAttribute->SelectedIndex = 0;
		attributSelected = 0;
		buttonAttributLoeschen->Enabled = true;
	}
	else
	{
		attributSelected = -1;
		buttonAttributLoeschen->Enabled = false;
	}
}


private: System::Void comboBoxAttributTyp_SelectedIndexChanged(System::Object ^  sender, System::EventArgs^  e)
{
	attributTyp = comboBoxAttributTyp->SelectedIndex;

	displayAttribute();
}

private: System::Void buttonNeuesAttribut_Click(System::Object ^  sender, System::EventArgs^  e)
{
	comboBoxAttributTyp->Enabled = false;
	listBoxAttribute->Enabled = false;
	buttonNeuesAttribut->Enabled = false;
	buttonAttributLoeschen->Enabled = false;

	buttonAttributUebernehmen->Enabled = true;
}


private: System::Void listBoxAttribute_SelectedIndexChanged(System::Object ^  sender, System::EventArgs^  e)
{
	attributSelected = listBoxAttribute->SelectedIndex;
	if ( attributSelected >= 0 )
		buttonAttributLoeschen->Enabled = true;
	else
		buttonAttributLoeschen->Enabled = false;
}

private: System::Void buttonAttributUebernehmen_Click(System::Object ^  sender, System::EventArgs^  e)
{
	String ^ STR;
	string   attributName;
	string   attributWert;
	bool     success;

	STR = textBoxAttributName->Text;
	if ( STR == nullptr ) return;
	QuConvert::systemStr2stdStr ( attributName, STR );

	STR = textBoxAttributWert->Text;
	if ( STR == nullptr ) return;
	success = checkAttributWert ( STR );
	if ( !success ) return;

	QuConvert::systemStr2stdStr ( attributWert, STR );

	pPlanAttribute->addGenerAttribut ( getAttributTyp(), attributName, "", attributWert );

	comboBoxAttributTyp->Enabled = true;
	listBoxAttribute->Enabled = true;
	buttonNeuesAttribut->Enabled = true;
	buttonAttributLoeschen->Enabled = true;

	buttonAttributUebernehmen->Enabled = false;
	
	textBoxAttributName->Text = nullptr;
	textBoxAttributWert->Text = nullptr;

	displayAttribute();	
}

private: System::Void buttonAttributLoeschen_Click(System::Object ^  sender, System::EventArgs^  e)
{
	if ( attributSelected < 0 ) return;

	pPlanAttribute->deleteGenerAttribut ( getAttributTyp(), attributSelected );

	displayAttribute();	
}

private: bool checkAttributWert ( String ^ STR )
{
	bool success = true;

	try
	{
		switch ( attributTyp )
		{
		case 1:
			XmlConvert::ToInt32 ( STR );
			break;

		case 2:
			XmlConvert::ToDouble ( STR );
			break;

		case 3:
			XmlConvert::ToDateTime ( STR );
			break;
		}
	}
	catch ( System::FormatException^ )
	{
		success = false;
	}

	return success;
}


};
}
