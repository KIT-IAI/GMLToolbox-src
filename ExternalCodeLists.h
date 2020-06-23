#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "GmlSchema.h"
#include "Dictionary.h"
#include "Convert.h"

using namespace std;

	/// <summary> 
	/// Dialog zur Pflege eines Codelist-Dictionaries
	///
	/// </summary>
	public ref class ExternalCodeLists : public System::Windows::Forms::Form
	{
	public: 
		ExternalCodeLists( String ^ CodelistDateiP, GML_SCHEMA_TYPE gmlTypP, GML_VERSION gmlVersionP )
		{
			InitializeComponent();

			pExternalCodeLists = NULL;
			pEintragSelected   = nullptr;
			neuerEintrag	     = false;
      CodelistDatei      = CodelistDateiP;
      gmlTyp             = gmlTypP;
      gmlVersion         = gmlVersionP;

      if ( gmlTyp == XPLANGML_2_0 || gmlTyp == XPLANGML_3_0  || gmlTyp == CITYGML_0_4_0 )
        dictionaryTyp = DICTIONARY_COLLECTION;
      else
      if ( gmlTyp == XPLANGML_4_0 || gmlTyp == XPLANGML_4_1  || gmlTyp == XPLANGML_5_0  ||
           gmlTyp == XPLANGML_5_1 || gmlTyp == XPLANGML_5_2  || gmlTyp == XPLANGML_5_3  ||
           gmlTyp == INSPIRE_LU_PLANNED ||  gmlTyp == CITYGML_1_0                       || 
           gmlTyp == CITYGML_2_0  || gmlTyp == CITYGML_3_0 || gmlTyp == GML_SCHEMA_DEFAULT )
        dictionaryTyp = SINGLE_DICTIONARY;

			initialize();

		}
        
	protected: 
	private: System::Windows::Forms::ComboBox ^  comboBoxExternalCodeLists;
	private: System::Windows::Forms::Label ^  label1;
	private: System::Windows::Forms::ListBox ^  listBoxCodeListEintraege;
	private: System::Windows::Forms::Label ^  label2;
	private: System::Windows::Forms::TextBox ^  textBoxSchluessel;
	private: System::Windows::Forms::TextBox ^  textBoxBeschreibung;
	private: System::Windows::Forms::Label ^  LabelSchluessel;
	private: System::Windows::Forms::Label ^  label3;
	private: System::Windows::Forms::Button ^  buttonNeuerEintrag;
	private: System::Windows::Forms::Button ^  buttonEintragUebernehmen;
	private: System::Windows::Forms::Button ^  buttonEintragLoeschen;
	private: System::Windows::Forms::TextBox ^  textBoxMeldung;
	private: System::Windows::Forms::Button ^  buttonBeenden;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^ components;
		
		GmlDictionary      * pExternalCodeLists;  //  Das zu bearbeitende Codelist-Dictionary
    GmlDictionaryEntry * pCodeListAct;        //  Aktuell bearbeiteter Eintarg
    GML_SCHEMA_TYPE      gmlTyp;              //  Typ des GML-Applikationsschemas
    GML_VERSION          gmlVersion;          //  GML-Typ (3.1.1 oder 3.1.2)
    DICTIONARY_TYP       dictionaryTyp;       //  Typ des Dictionaries
		String             ^ pEintragSelected;    //  Selektierter Eintrag
		bool				         neuerEintrag;        //  Ein neuer Eintrag wird erstellt
    String             ^ CodelistDatei;       //  Pfadname des Codelist-Dictionaries

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->comboBoxExternalCodeLists = (gcnew System::Windows::Forms::ComboBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->listBoxCodeListEintraege = (gcnew System::Windows::Forms::ListBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->textBoxSchluessel = (gcnew System::Windows::Forms::TextBox());
      this->textBoxBeschreibung = (gcnew System::Windows::Forms::TextBox());
      this->LabelSchluessel = (gcnew System::Windows::Forms::Label());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->buttonNeuerEintrag = (gcnew System::Windows::Forms::Button());
      this->buttonEintragUebernehmen = (gcnew System::Windows::Forms::Button());
      this->buttonEintragLoeschen = (gcnew System::Windows::Forms::Button());
      this->textBoxMeldung = (gcnew System::Windows::Forms::TextBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // comboBoxExternalCodeLists
      // 
      this->comboBoxExternalCodeLists->Location = System::Drawing::Point(16, 16);
      this->comboBoxExternalCodeLists->Name = L"comboBoxExternalCodeLists";
      this->comboBoxExternalCodeLists->Size = System::Drawing::Size(320, 21);
      this->comboBoxExternalCodeLists->Sorted = true;
      this->comboBoxExternalCodeLists->TabIndex = 0;
      this->comboBoxExternalCodeLists->SelectedIndexChanged += gcnew System::EventHandler(this, &ExternalCodeLists::comboBoxExternalCodeLists_SelectedIndexChanged);
      // 
      // label1
      // 
      this->label1->Location = System::Drawing::Point(16, 40);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(100, 23);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Aufzählung";
      // 
      // listBoxCodeListEintraege
      // 
      this->listBoxCodeListEintraege->Location = System::Drawing::Point(16, 64);
      this->listBoxCodeListEintraege->Name = L"listBoxCodeListEintraege";
      this->listBoxCodeListEintraege->ScrollAlwaysVisible = true;
      this->listBoxCodeListEintraege->Size = System::Drawing::Size(176, 160);
      this->listBoxCodeListEintraege->TabIndex = 2;
      this->listBoxCodeListEintraege->SelectedIndexChanged += gcnew System::EventHandler(this, &ExternalCodeLists::listBoxCodeListEintraege_SelectedIndexChanged);
      // 
      // label2
      // 
      this->label2->Location = System::Drawing::Point(16, 232);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(100, 16);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Einträge";
      // 
      // textBoxSchluessel
      // 
      this->textBoxSchluessel->Enabled = false;
      this->textBoxSchluessel->Location = System::Drawing::Point(208, 64);
      this->textBoxSchluessel->Name = L"textBoxSchluessel";
      this->textBoxSchluessel->Size = System::Drawing::Size(128, 20);
      this->textBoxSchluessel->TabIndex = 4;
      this->textBoxSchluessel->Text = L"textBox1";
      // 
      // textBoxBeschreibung
      // 
      this->textBoxBeschreibung->Location = System::Drawing::Point(208, 104);
      this->textBoxBeschreibung->Name = L"textBoxBeschreibung";
      this->textBoxBeschreibung->Size = System::Drawing::Size(128, 20);
      this->textBoxBeschreibung->TabIndex = 5;
      this->textBoxBeschreibung->Text = L"textBox1";
      this->textBoxBeschreibung->Leave += gcnew System::EventHandler(this, &ExternalCodeLists::textBoxBeschreibung_Leave);
      // 
      // LabelSchluessel
      // 
      this->LabelSchluessel->Location = System::Drawing::Point(344, 64);
      this->LabelSchluessel->Name = L"LabelSchluessel";
      this->LabelSchluessel->Size = System::Drawing::Size(72, 20);
      this->LabelSchluessel->TabIndex = 6;
      this->LabelSchluessel->Text = L"Schlüssel";
      this->LabelSchluessel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
      // 
      // label3
      // 
      this->label3->Location = System::Drawing::Point(344, 104);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(100, 20);
      this->label3->TabIndex = 7;
      this->label3->Text = L"Beschreibung";
      this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
      // 
      // buttonNeuerEintrag
      // 
      this->buttonNeuerEintrag->Location = System::Drawing::Point(208, 136);
      this->buttonNeuerEintrag->Name = L"buttonNeuerEintrag";
      this->buttonNeuerEintrag->Size = System::Drawing::Size(88, 23);
      this->buttonNeuerEintrag->TabIndex = 8;
      this->buttonNeuerEintrag->Text = L"Neuer Eintrag";
      this->buttonNeuerEintrag->Click += gcnew System::EventHandler(this, &ExternalCodeLists::buttonNeuerEintrag_Click);
      // 
      // buttonEintragUebernehmen
      // 
      this->buttonEintragUebernehmen->Location = System::Drawing::Point(320, 136);
      this->buttonEintragUebernehmen->Name = L"buttonEintragUebernehmen";
      this->buttonEintragUebernehmen->Size = System::Drawing::Size(88, 23);
      this->buttonEintragUebernehmen->TabIndex = 9;
      this->buttonEintragUebernehmen->Text = L"Übernehmen";
      this->buttonEintragUebernehmen->Click += gcnew System::EventHandler(this, &ExternalCodeLists::buttonEintragUebernehmen_Click);
      // 
      // buttonEintragLoeschen
      // 
      this->buttonEintragLoeschen->Location = System::Drawing::Point(208, 200);
      this->buttonEintragLoeschen->Name = L"buttonEintragLoeschen";
      this->buttonEintragLoeschen->Size = System::Drawing::Size(96, 23);
      this->buttonEintragLoeschen->TabIndex = 10;
      this->buttonEintragLoeschen->Text = L"Eintrag löschen";
      this->buttonEintragLoeschen->Click += gcnew System::EventHandler(this, &ExternalCodeLists::buttonEintragLoeschen_Click);
      // 
      // textBoxMeldung
      // 
      this->textBoxMeldung->BackColor = System::Drawing::SystemColors::Menu;
      this->textBoxMeldung->BorderStyle = System::Windows::Forms::BorderStyle::None;
      this->textBoxMeldung->ForeColor = System::Drawing::SystemColors::ActiveCaption;
      this->textBoxMeldung->Location = System::Drawing::Point(208, 176);
      this->textBoxMeldung->Name = L"textBoxMeldung";
      this->textBoxMeldung->Size = System::Drawing::Size(200, 13);
      this->textBoxMeldung->TabIndex = 11;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonBeenden->Location = System::Drawing::Point(352, 232);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 32);
      this->buttonBeenden->TabIndex = 12;
      this->buttonBeenden->Text = L"Beenden";
      // 
      // ExternalCodeLists
      // 
      this->AcceptButton = this->buttonBeenden;
      this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
      this->CancelButton = this->buttonBeenden;
      this->ClientSize = System::Drawing::Size(440, 273);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->textBoxMeldung);
      this->Controls->Add(this->textBoxBeschreibung);
      this->Controls->Add(this->textBoxSchluessel);
      this->Controls->Add(this->buttonEintragLoeschen);
      this->Controls->Add(this->buttonEintragUebernehmen);
      this->Controls->Add(this->buttonNeuerEintrag);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->LabelSchluessel);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->listBoxCodeListEintraege);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->comboBoxExternalCodeLists);
      this->Name = L"ExternalCodeLists";
      this->Text = L"Pflege der externen Aufzählungen";
      this->Closed += gcnew System::EventHandler(this, &ExternalCodeLists::ExternalCodeLists_Closed);
      this->ResumeLayout(false);
      this->PerformLayout();

    }		


///////////////////////////////////////////////////////////////////////////////
//  Commandhandler für die Auswahl einer Codeliste in der ComboBox           //
//  Aufzählung                                                               //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxExternalCodeLists_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	String			        ^ STR;
	string		  		     stdString;
	size_t               i, anz;
	vector<string>       schluessel;
  
  vector<GmlDictionaryEntryDefinition> entryDefinitions;

	STR = dynamic_cast<String^> ( comboBoxExternalCodeLists->SelectedItem );
	QuConvert::systemStr2stdStr ( stdString, STR );

	pCodeListAct = pExternalCodeLists->getDictionaryEntry ( stdString );
	if ( pCodeListAct == NULL ) return;

	listBoxCodeListEintraege->Items->Clear();

	anz = pCodeListAct->getCodelistEintraege ( schluessel, entryDefinitions );
	for ( i = 0; i < anz; i++ )
	{
		STR = QuConvert::ToString ( schluessel[i] );
		listBoxCodeListEintraege->Items->Add ( STR );
	}

	if ( anz > 0 )
		listBoxCodeListEintraege->SelectedIndex = 0;
	else
	{
		textBoxBeschreibung->Text = nullptr;
		textBoxSchluessel->Text = nullptr ;
	}

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler für die Auswahl einer Codeliste in der Liste              //
//  Einträge                                                                 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void listBoxCodeListEintraege_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	String ^ STR;
	string   schluessel;
	string   beschreibung;
	bool     success;

	pEintragSelected = dynamic_cast<String^> ( listBoxCodeListEintraege->SelectedItem );
	if ( pEintragSelected == nullptr ) return;

	textBoxSchluessel->Text = pEintragSelected;
	QuConvert::systemStr2stdStr ( schluessel, pEintragSelected );
	success = pCodeListAct->getSchluesselText ( schluessel, beschreibung );

	if ( success )
	{
		STR = QuConvert::ToString ( beschreibung );
		textBoxBeschreibung->Text = STR;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Beendigung der Eigabe einer Beschreibung                  //
///////////////////////////////////////////////////////////////////////////////
private: System::Void textBoxBeschreibung_Leave(System::Object ^  sender, System::EventArgs ^  e)
{
	String ^ STR;
	string   schluessel, beschreibung;
	if ( pEintragSelected == nullptr || neuerEintrag ) return;

	STR = textBoxBeschreibung->Text;
	if ( STR != nullptr )
		QuConvert::systemStr2stdStr ( beschreibung, STR );
	else
		beschreibung = "";

	QuConvert::systemStr2stdStr ( schluessel, pEintragSelected );
	pCodeListAct->setSchluesselText ( schluessel, beschreibung );

	listBoxCodeListEintraege_SelectedIndexChanged ( sender, e );
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Neuer Eintrag                                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonNeuerEintrag_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	comboBoxExternalCodeLists->Enabled =  false;
	listBoxCodeListEintraege->Enabled =  false;
	textBoxSchluessel->Enabled =  true;
	buttonNeuerEintrag->Enabled =  false;
	buttonEintragUebernehmen->Enabled =  true;
	buttonEintragLoeschen->Enabled =  false;
	buttonBeenden->Enabled =  false;

	textBoxMeldung->Text = nullptr;
	textBoxSchluessel->Text = nullptr;
	textBoxBeschreibung->Text = nullptr;

	neuerEintrag = true;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Übernehmen                                         //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonEintragUebernehmen_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	String ^ STR_schluessel;
	String ^ STR_bedeutung;
	string   schluessel, bedeutung;

  string identifierCodespace = pCodeListAct->getIdentifierCodespace();
  string codeListName        = pCodeListAct->getName();
  string codeListIdentifier  = pCodeListAct->getIdentifier();

	STR_schluessel = textBoxSchluessel->Text;
	if ( STR_schluessel == nullptr )
	{
		textBoxMeldung->Text = "Bitte Schlüssel spezifizieren";
		return;
	}

	QuConvert::systemStr2stdStr ( schluessel, STR_schluessel );
	if ( pCodeListAct->existSchluesselNummer ( schluessel ) )
	{
		textBoxMeldung->Text = "Schlüssel ist schon vergeben" ;
		return;
	}

	STR_bedeutung = textBoxBeschreibung->Text;
	if ( STR_bedeutung == nullptr )
	{
		textBoxMeldung->Text = "Bitte Beschreibung spezifizieren";
		return;
	}

	QuConvert::systemStr2stdStr ( bedeutung, STR_bedeutung );
	if ( bedeutung == "" )
	{
		textBoxMeldung->Text = "Bitte Beschreibung spezifizieren";
		return;
	}

	if ( pCodeListAct->existSchluesselText ( bedeutung ) )
	{
		textBoxMeldung->Text = "Schlüssel-Text ist schon vergeben";
		return;
	}
	
	textBoxMeldung->Text = nullptr;

  
  GmlDictionaryEntryDefinition entryDefinition;
  
  entryDefinition.id           = codeListName + schluessel;

  if ( gmlTyp == XPLANGML_4_0 || gmlTyp == XPLANGML_4_1 || gmlTyp == XPLANGML_5_0 ||
       gmlTyp == XPLANGML_5_1 || gmlTyp == XPLANGML_5_2 || gmlTyp == XPLANGML_5_3 ||
       gmlTyp == XPLANGML_3_0 || gmlTyp == CITYGML_2_0 || gmlTyp == CITYGML_3_0 )
  {
    entryDefinition.name         = schluessel;
    entryDefinition.beschreibung = bedeutung;
  }

  if ( codeListIdentifier != "" )
  {
    entryDefinition.identifierCodespace = identifierCodespace;
    if ( gmlTyp ==  BOREHOLEML_3_0 || gmlTyp == INSPIRE_LU_PLANNED  )
    {
      entryDefinition.identifier = schluessel;
      entryDefinition.name = bedeutung;
    }
    else
    {
      entryDefinition.identifier = codeListIdentifier + ":" + schluessel;
      entryDefinition.name = schluessel;
      entryDefinition.beschreibung = bedeutung;
    }
  }

	pCodeListAct->addDefinition ( schluessel, entryDefinition );

	comboBoxExternalCodeLists->Enabled = true ;
	listBoxCodeListEintraege->Enabled = true ;
	textBoxSchluessel->Enabled = false;
	buttonNeuerEintrag->Enabled = true ;
	buttonEintragUebernehmen->Enabled = false;
	buttonEintragLoeschen->Enabled = true ;
	buttonBeenden->Enabled = true ;

	listBoxCodeListEintraege->Items->Add ( STR_schluessel );
	listBoxCodeListEintraege->SelectedItem = STR_schluessel;
	neuerEintrag = false;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Eintrag Löschen                                    //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonEintragLoeschen_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	string schluessel;
	bool   success;

	if ( pEintragSelected == nullptr ) return;

	QuConvert::systemStr2stdStr ( schluessel, pEintragSelected );
	success = pCodeListAct->removeDefinition ( schluessel );
	if ( success )
	{
		listBoxCodeListEintraege->Items->Remove ( pEintragSelected );
		if ( pCodeListAct->getCodelistEintraegeAnz() > 0 )
			listBoxCodeListEintraege->SelectedIndex = 0;
		else
		{
			textBoxSchluessel->Text = nullptr;
			textBoxBeschreibung->Text = nullptr;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Initialisierung des Dialogs                                              //
///////////////////////////////////////////////////////////////////////////////
private:System::Void initialize()
{
	size_t  			              codeListAnz, i;
	string						          codeListName;
	String					          ^ STR;
	vector<GmlDictionaryEntry*> dictionaryEntries;
	GmlDictionaryEntry        * pEntry;
  GmlDictionaryReader       ^ pDictionaryReader;

  QuConvert::systemStr2stdStr( codeListName, CodelistDatei );

  pDictionaryReader = gcnew GmlDictionaryReader ();
  pExternalCodeLists = new GmlDictionary;

  if ( dictionaryTyp == DICTIONARY_COLLECTION )
   pDictionaryReader->readGmlDictionaryCollection( codeListName, pExternalCodeLists, false );
  else
  if ( dictionaryTyp == SINGLE_DICTIONARY )
    pDictionaryReader->readGmlSingleDictionary( codeListName, pExternalCodeLists, false );

  pDictionaryReader->closeReader();

	codeListAnz = pExternalCodeLists->getDictionaryEntries ( dictionaryEntries );

	for ( i = 0; i < codeListAnz; i++ )
	{
		pEntry = dictionaryEntries[i];
		codeListName = pEntry->getName();
		STR = QuConvert::ToString ( codeListName );
		comboBoxExternalCodeLists->Items->Add ( STR );
	}

	if ( codeListAnz > 0 )
	{
		if ( pCodeListAct != NULL )
		{
			codeListName = pCodeListAct->getName();
			STR = QuConvert::ToString ( codeListName );
			comboBoxExternalCodeLists->SelectedItem = STR;
		}
		else
			comboBoxExternalCodeLists->SelectedIndex = 0;
	}

	buttonEintragUebernehmen->Enabled = false;
	textBoxMeldung->Text = nullptr;

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Schließen des Dialogs                                     //
///////////////////////////////////////////////////////////////////////////////
private: System::Void ExternalCodeLists_Closed(System::Object ^  sender, System::EventArgs ^  e)
{
	String ^ BackupFileName;
	int      index;
  string   fileName;

  QuConvert::systemStr2stdStr( fileName, CodelistDatei );

	if ( File::Exists ( CodelistDatei ) )
	{
		index = CodelistDatei->LastIndexOf ( "." );
		BackupFileName = CodelistDatei->Substring ( 0, index+1 );
		BackupFileName = String::Concat ( BackupFileName, "bak" );
		FileInfo ^ pF = gcnew FileInfo ( CodelistDatei );
		pF->CopyTo ( BackupFileName, true );
	}

	GmlDictionaryWriter ^ pWriter = gcnew GmlDictionaryWriter ( pExternalCodeLists, gmlVersion );
	pWriter->write ( fileName, gmlTyp );

}

};
