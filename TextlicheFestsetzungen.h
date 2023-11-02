#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "ShpFileNamenKonv.h"
#include "Convert.h"

using namespace std;

	/// <summary> 
	/// Manuelle Festlegung textueller Planinhalte (XP_TextAbschnitt und abgeleitete Klassen)
	///
	/// </summary>
	public ref class TextlicheFestsetzungen : public System::Windows::Forms::Form
	{
	public: 
		TextlicheFestsetzungen( PlanAttribute * pPlanAttributeP, XPLAN_TYP planTyp )
		{
			pPlanAttribute = pPlanAttributeP;

			InitializeComponent();

			initialize();

			switch ( planTyp )
			{
			case  BPLAN:
				m_planTyp = "BPlan";
				break;

			case FPLAN:
				m_planTyp = "FPlan";
				break;

			case RPLAN:
				m_planTyp = "RPlan";
				break;

			case LPLAN:
				m_planTyp = "LPlan";
				break;

			case SOPLAN:
				m_planTyp = "SOPlan";
				break;

			default: 
				m_planTyp = "SOPlan";
				break;
			}
		}
        
	private: System::Windows::Forms::ComboBox ^  comboBoxSchluessel;
	private: System::Windows::Forms::Label ^  labelSchluessel;
	private: System::Windows::Forms::Button ^  buttonNeuerEintrag;
	private: System::Windows::Forms::Button ^  buttonEintragLoeschen;
	private: System::Windows::Forms::Button ^  buttonSchluesselAendern;
	private: System::Windows::Forms::TextBox ^  textBoxSchluessel;
	private: System::Windows::Forms::Button ^  buttonUebernehmen;
	private: System::Windows::Forms::TextBox ^  textBoxFestsetzung;
	private: System::Windows::Forms::Button ^  buttonBeenden;

	private:
		PlanAttribute * pPlanAttribute;			// Aggregation der Attribute der Plan-Klasse
		String        ^ m_planTyp;	  		  // Typ des Plans, für den ein textueller Inhalt erzeugt werden soll

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
      this->comboBoxSchluessel = (gcnew System::Windows::Forms::ComboBox());
      this->labelSchluessel = (gcnew System::Windows::Forms::Label());
      this->buttonNeuerEintrag = (gcnew System::Windows::Forms::Button());
      this->buttonEintragLoeschen = (gcnew System::Windows::Forms::Button());
      this->buttonSchluesselAendern = (gcnew System::Windows::Forms::Button());
      this->textBoxSchluessel = (gcnew System::Windows::Forms::TextBox());
      this->buttonUebernehmen = (gcnew System::Windows::Forms::Button());
      this->textBoxFestsetzung = (gcnew System::Windows::Forms::TextBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // comboBoxSchluessel
      // 
      this->comboBoxSchluessel->Location = System::Drawing::Point(8, 16);
      this->comboBoxSchluessel->Name = L"comboBoxSchluessel";
      this->comboBoxSchluessel->Size = System::Drawing::Size(224, 21);
      this->comboBoxSchluessel->TabIndex = 0;
      this->comboBoxSchluessel->SelectedIndexChanged += gcnew System::EventHandler(this, &TextlicheFestsetzungen::comboBoxSchluessel_SelectedIndexChanged);
      // 
      // labelSchluessel
      // 
      this->labelSchluessel->Location = System::Drawing::Point(16, 40);
      this->labelSchluessel->Name = L"labelSchluessel";
      this->labelSchluessel->Size = System::Drawing::Size(100, 16);
      this->labelSchluessel->TabIndex = 1;
      this->labelSchluessel->Text = L"Schlüssel";
      // 
      // buttonNeuerEintrag
      // 
      this->buttonNeuerEintrag->Location = System::Drawing::Point(248, 16);
      this->buttonNeuerEintrag->Name = L"buttonNeuerEintrag";
      this->buttonNeuerEintrag->Size = System::Drawing::Size(48, 23);
      this->buttonNeuerEintrag->TabIndex = 2;
      this->buttonNeuerEintrag->Text = L"Neu";
      this->buttonNeuerEintrag->Click += gcnew System::EventHandler(this, &TextlicheFestsetzungen::buttonNeuerEintrag_Click);
      // 
      // buttonEintragLoeschen
      // 
      this->buttonEintragLoeschen->Location = System::Drawing::Point(304, 16);
      this->buttonEintragLoeschen->Name = L"buttonEintragLoeschen";
      this->buttonEintragLoeschen->Size = System::Drawing::Size(56, 23);
      this->buttonEintragLoeschen->TabIndex = 3;
      this->buttonEintragLoeschen->Text = L"Löschen";
      this->buttonEintragLoeschen->Click += gcnew System::EventHandler(this, &TextlicheFestsetzungen::buttonEintragLoeschen_Click);
      // 
      // buttonSchluesselAendern
      // 
      this->buttonSchluesselAendern->Location = System::Drawing::Point(368, 16);
      this->buttonSchluesselAendern->Name = L"buttonSchluesselAendern";
      this->buttonSchluesselAendern->Size = System::Drawing::Size(104, 23);
      this->buttonSchluesselAendern->TabIndex = 4;
      this->buttonSchluesselAendern->Text = L"Schlüssel ändern";
      this->buttonSchluesselAendern->Click += gcnew System::EventHandler(this, &TextlicheFestsetzungen::buttonSchluesselAendern_Click);
      // 
      // textBoxSchluessel
      // 
      this->textBoxSchluessel->Location = System::Drawing::Point(8, 56);
      this->textBoxSchluessel->Name = L"textBoxSchluessel";
      this->textBoxSchluessel->Size = System::Drawing::Size(224, 20);
      this->textBoxSchluessel->TabIndex = 5;
      // 
      // buttonUebernehmen
      // 
      this->buttonUebernehmen->Location = System::Drawing::Point(248, 56);
      this->buttonUebernehmen->Name = L"buttonUebernehmen";
      this->buttonUebernehmen->Size = System::Drawing::Size(48, 23);
      this->buttonUebernehmen->TabIndex = 6;
      this->buttonUebernehmen->Text = L"Ok";
      this->buttonUebernehmen->Click += gcnew System::EventHandler(this, &TextlicheFestsetzungen::buttonUebernehmen_Click);
      // 
      // textBoxFestsetzung
      // 
      this->textBoxFestsetzung->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxFestsetzung->Location = System::Drawing::Point(0, 96);
      this->textBoxFestsetzung->Multiline = true;
      this->textBoxFestsetzung->Name = L"textBoxFestsetzung";
      this->textBoxFestsetzung->Size = System::Drawing::Size(560, 192);
      this->textBoxFestsetzung->TabIndex = 7;
      this->textBoxFestsetzung->Leave += gcnew System::EventHandler(this, &TextlicheFestsetzungen::textBoxFestsetzung_Leave);
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->buttonBeenden->Location = System::Drawing::Point(480, 48);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 40);
      this->buttonBeenden->TabIndex = 8;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &TextlicheFestsetzungen::buttonBeenden_Click);
      // 
      // TextlicheFestsetzungen
      // 
      this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
      this->ClientSize = System::Drawing::Size(560, 285);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->textBoxFestsetzung);
      this->Controls->Add(this->buttonUebernehmen);
      this->Controls->Add(this->textBoxSchluessel);
      this->Controls->Add(this->buttonSchluesselAendern);
      this->Controls->Add(this->buttonEintragLoeschen);
      this->Controls->Add(this->buttonNeuerEintrag);
      this->Controls->Add(this->labelSchluessel);
      this->Controls->Add(this->comboBoxSchluessel);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"TextlicheFestsetzungen";
      this->Text = L"Textliche Festsetzungen";
      this->ResumeLayout(false);
      this->PerformLayout();

    }		
	
///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Dialogbox im Konstruktor															 //
///////////////////////////////////////////////////////////////////////////////
	private: System::Void initialize()
	{
		size_t                 i, anz;
		vector<TextAbschnitt*> vTextAbschnitte;
		string				         schluessel;
		string			           wert;
		String				       ^ STR;

		anz = pPlanAttribute->getTextAbschnitte( vTextAbschnitte );
		for ( i = 0; i < anz; i++ )
		{
			schluessel = vTextAbschnitte[i]->schluessel;
			STR = QuConvert::ToString ( schluessel );
			comboBoxSchluessel->Items->Add ( STR );
		}

		if ( anz > 0 )
		{
			comboBoxSchluessel->SelectedIndex = 0;
			wert =  vTextAbschnitte[0]->text;
			STR = QuConvert::ToString ( wert );
			textBoxFestsetzung->Text = STR;
		}
		else
		{
			buttonEintragLoeschen->Enabled = false;
			buttonSchluesselAendern->Enabled = false;
		}
		buttonUebernehmen->Enabled = false;
		textBoxSchluessel->Enabled = false;
	}

//////////////////////////////////////////////////////////////////////////////
//  Commndhandler ComboBox "Schlüssel"																		  //
//  Auswahl eines bereits definierten textlichen Planinhalts über den       //
//  Schlüssel																																//
///////////////////////////////////////////////////////////////////////////////
	private: System::Void comboBoxSchluessel_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
	{
		size_t    					   anz;
		int                    index;
		vector<TextAbschnitt*> vTextAbschnitte;
		string				         wert;
		String			      	 ^ STR;

		anz = pPlanAttribute->getTextAbschnitte( vTextAbschnitte );

		index = comboBoxSchluessel->SelectedIndex;
		if ( index >= 0 )
		{
			wert = vTextAbschnitte[index]->text;
			STR = QuConvert::ToString ( wert );
			textBoxFestsetzung->Text = STR;
		}
		else
			textBoxFestsetzung->Text =  "";

		buttonEintragLoeschen->Enabled = true;
		buttonSchluesselAendern->Enabled = true;
	}

//////////////////////////////////////////////////////////////////////////////
//  Commandhandler "Verlassen der TextBox für den Inhalt der textlichen			//
//  Festsetzung"																														//
//////////////////////////////////////////////////////////////////////////////
	private: System::Void textBoxFestsetzung_Leave(System::Object ^  sender, System::EventArgs ^  e)
	{
		int      index;
		bool     success;
		string   schluessel, wert, wertURL, planTyp;
		String ^ STR;

		index = comboBoxSchluessel->SelectedIndex;
		if ( index >= 0 )
		{
			success = pPlanAttribute->getTextAbschnitt ( index, schluessel, wert, wertURL, planTyp ) ;
			if ( !success ) return;

			STR = textBoxFestsetzung->Text;
			if ( STR != nullptr )
			{
				QuConvert::systemStr2stdStr ( wert, STR );
				pPlanAttribute->setTextAbschnitt ( index, schluessel, wert, wertURL, planTyp );

			}
		}
 
	 }

 //////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Neu"																						   //
//  Definition einer neuen textlichen Festsetzung														 //
//////////////////////////////////////////////////////////////////////////////
	private: System::Void buttonNeuerEintrag_Click(System::Object ^  sender, System::EventArgs ^  e)
	{
		comboBoxSchluessel->SelectedIndex = -1;
		comboBoxSchluessel->Enabled = false;
		buttonNeuerEintrag->Enabled = false;
		buttonEintragLoeschen->Enabled = false;
		buttonSchluesselAendern->Enabled = false;

		textBoxSchluessel->Enabled = true;
		textBoxSchluessel->Text =  "";
		buttonUebernehmen->Enabled = true;
	}

//////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Ok"																						  //
//  Beendet die Spezifikation oder Änderung einer textlichen Festsetzung    //
//  und übernimmt die Eingaben																							//
//////////////////////////////////////////////////////////////////////////////
	private: System::Void buttonUebernehmen_Click(System::Object ^  sender, System::EventArgs ^  e)
	{
		String ^ STR_schluessel;
		String ^ STR_wert;
		string   schluessel, wert, wertURL, planTyp;
		int		 index;

		wertURL = "";
		QuConvert::systemStr2stdStr ( planTyp, m_planTyp );

		STR_schluessel = textBoxSchluessel->Text;
		if ( STR_schluessel == nullptr ) return;

		QuConvert::systemStr2stdStr ( schluessel, STR_schluessel );
		if ( pPlanAttribute->schluesselTextAbschnittExist ( schluessel ) == true )
		{
			MessageBox::Show ( "Schlüssel ist nicht eindeutig" );
			return;
		}

		STR_wert = textBoxFestsetzung->Text;
		if ( STR_wert != nullptr ) QuConvert::systemStr2stdStr ( wert, STR_wert );
		else			   wert = "";

		index = comboBoxSchluessel->SelectedIndex;
		if ( index == -1 )
		{
			pPlanAttribute->addTextAbschnitt ( schluessel, wert, wertURL, planTyp );
			comboBoxSchluessel->Items->Add ( STR_schluessel );
		}
		else
		{
			Object ^ OBJ = comboBoxSchluessel->SelectedItem;
			comboBoxSchluessel->Items->Remove ( OBJ );
			comboBoxSchluessel->Items->Insert ( index, STR_schluessel );

			pPlanAttribute->setTextAbschnitt ( index, schluessel, wert, wertURL, planTyp );
		}
		
		comboBoxSchluessel->SelectedItem = STR_schluessel;

		comboBoxSchluessel->Enabled = true;
		buttonNeuerEintrag->Enabled = true;
		buttonEintragLoeschen->Enabled = true;
		buttonSchluesselAendern->Enabled = true;

		textBoxSchluessel->Enabled = false;
		buttonUebernehmen->Enabled = false;
	}

//////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Schlüssel ändern"																//
//  Ändert den Schlüssel der aktuell bearbeiteten textlichen Festsetzung    //
//////////////////////////////////////////////////////////////////////////////
	private: System::Void buttonSchluesselAendern_Click(System::Object ^  sender, System::EventArgs ^  e)
	{
		String ^ STR;
		int      index;

		index = comboBoxSchluessel->SelectedIndex;
		if ( index == -1 ) return;

		STR = dynamic_cast<String^> ( comboBoxSchluessel->SelectedItem );

		comboBoxSchluessel->Enabled = false;
		buttonNeuerEintrag->Enabled = false;
		buttonEintragLoeschen->Enabled = false;
		buttonSchluesselAendern->Enabled = false;

		textBoxSchluessel->Enabled = true;
		textBoxSchluessel->Text = STR;

		buttonUebernehmen->Enabled = true;

	}

//////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Löschen"																				  //
//  Löscht die aktuell bearbeitete textliche Festsetzung										//
//////////////////////////////////////////////////////////////////////////////
	private: System::Void buttonEintragLoeschen_Click(System::Object ^  sender, System::EventArgs ^  e)
	{
		int index;
		Object ^ OBJ;

		index = comboBoxSchluessel->SelectedIndex;
		if ( index == -1 ) return;

		OBJ = comboBoxSchluessel->SelectedItem;
		comboBoxSchluessel->Items->Remove ( OBJ );
		pPlanAttribute->deleteTextAbschnitt ( index );

		if ( pPlanAttribute->getTextAbschnittAnz() > 0 )
			comboBoxSchluessel->SelectedIndex = 0;
		else
		{
			comboBoxSchluessel->SelectedIndex = -1;
			comboBoxSchluessel->Text =  "";
			textBoxSchluessel->Text =  "";
			textBoxFestsetzung->Text =  "";
		}
	}

//////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"																				  //
//  Schließt den Dialog																										  //
//////////////////////////////////////////////////////////////////////////////
	private: System::Void buttonBeenden_Click(System::Object ^  sender, System::EventArgs ^  e)
	{
		Close();	
	}

};
