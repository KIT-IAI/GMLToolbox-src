#pragma once

#using <mscorlib.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Convert.h"
#include "Checking.h"
#include "FeatureAttribute.h"
#include "KonformitätsregelDetails.h"

	/// <summary>
	/// Anzeige von Informationen zu den Konformitätsfehlern
	///
	/// </summary>
	public ref class KonformitaetsFehler : public System::Windows::Forms::Form
	{
	public:
		KonformitaetsFehler( Features * pFeatures, Checking * pChecking )
		{
			InitializeComponent();

      m_pFeatures = pFeatures;
      m_pErrorReport = pChecking->getErrorReport();

      if ( m_pErrorReport->getSemanticErrorAnz() > 0 )
      {
        indexConformityError     = 0;
        m_ConformityErrorAct     = m_pErrorReport->getSemanticError( 0 );
        displayConformityError();
      }
      else
      {
        buttonVor->Enabled           = false;
        buttonZurueck->Enabled       = false;
        textBoxFehlerNummer->Enabled = false;
      }    
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~KonformitaetsFehler()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::Button^  buttonZurueck;
  private: System::Windows::Forms::Button^  buttonVor;
  private: System::Windows::Forms::TextBox^  textBoxFehlerNummer;
  private: System::Windows::Forms::GroupBox^  groupBox1;
  private: System::Windows::Forms::TextBox^  textBoxFehlerBeschreibung;
  private: System::Windows::Forms::TextBox^  textBoxFehlerId;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::TextBox^  textBoxObjektTyp;
  private: System::Windows::Forms::Button^  buttonObjektAttribute;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::TextBox^  textBoxRelation;
  private: System::Windows::Forms::Label^  label5;
  private: System::Windows::Forms::Button^  buttonReferiertesObjektAttribute;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::Label^  label7;
  private: System::Windows::Forms::TextBox^  textBoxReferiertesObjektId;
  private: System::Windows::Forms::Label^  label6;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::TextBox^  textBoxObjektId;
  private: System::Windows::Forms::TextBox^  textBoxReferiertesObjektTyp;
  private: System::Windows::Forms::Button^  buttonRegelDetails;
  private: System::Windows::Forms::Label^  label8;
  private: System::Windows::Forms::TextBox^  textBoxAttributName;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

  private:
    Features        * m_pFeatures;          // Aggregation aller Features
    ErrorReport     * m_pErrorReport;       // Vollständiger Fehlerreport
    SemanticError   * m_ConformityErrorAct; // Aktuell angezeigter Konformitätsfehler
    int               indexConformityError; // Index des aktuellen Konformitätsfehlers

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      System::Windows::Forms::Label^  label1;
      this->buttonZurueck = (gcnew System::Windows::Forms::Button());
      this->buttonVor = (gcnew System::Windows::Forms::Button());
      this->textBoxFehlerNummer = (gcnew System::Windows::Forms::TextBox());
      this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
      this->label8 = (gcnew System::Windows::Forms::Label());
      this->textBoxAttributName = (gcnew System::Windows::Forms::TextBox());
      this->buttonRegelDetails = (gcnew System::Windows::Forms::Button());
      this->label7 = (gcnew System::Windows::Forms::Label());
      this->textBoxReferiertesObjektId = (gcnew System::Windows::Forms::TextBox());
      this->label6 = (gcnew System::Windows::Forms::Label());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->textBoxObjektId = (gcnew System::Windows::Forms::TextBox());
      this->buttonReferiertesObjektAttribute = (gcnew System::Windows::Forms::Button());
      this->label5 = (gcnew System::Windows::Forms::Label());
      this->textBoxReferiertesObjektTyp = (gcnew System::Windows::Forms::TextBox());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->textBoxRelation = (gcnew System::Windows::Forms::TextBox());
      this->buttonObjektAttribute = (gcnew System::Windows::Forms::Button());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->textBoxObjektTyp = (gcnew System::Windows::Forms::TextBox());
      this->textBoxFehlerId = (gcnew System::Windows::Forms::TextBox());
      this->textBoxFehlerBeschreibung = (gcnew System::Windows::Forms::TextBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      label1 = (gcnew System::Windows::Forms::Label());
      this->groupBox1->SuspendLayout();
      this->SuspendLayout();
      // 
      // label1
      // 
      label1->AutoSize = true;
      label1->Location = System::Drawing::Point(6, 132);
      label1->Name = L"label1";
      label1->Size = System::Drawing::Size(88, 13);
      label1->TabIndex = 2;
      label1->Text = L"Konformitätsregel";
      // 
      // buttonZurueck
      // 
      this->buttonZurueck->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonZurueck->Location = System::Drawing::Point(107, 441);
      this->buttonZurueck->Name = L"buttonZurueck";
      this->buttonZurueck->Size = System::Drawing::Size(41, 20);
      this->buttonZurueck->TabIndex = 0;
      this->buttonZurueck->Text = L"<--";
      this->buttonZurueck->UseVisualStyleBackColor = true;
      this->buttonZurueck->Click += gcnew System::EventHandler(this, &KonformitaetsFehler::buttonZurueck_Click);
      // 
      // buttonVor
      // 
      this->buttonVor->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonVor->Location = System::Drawing::Point(264, 442);
      this->buttonVor->Name = L"buttonVor";
      this->buttonVor->Size = System::Drawing::Size(39, 20);
      this->buttonVor->TabIndex = 1;
      this->buttonVor->Text = L"-->";
      this->buttonVor->UseVisualStyleBackColor = true;
      this->buttonVor->Click += gcnew System::EventHandler(this, &KonformitaetsFehler::buttonVor_Click);
      // 
      // textBoxFehlerNummer
      // 
      this->textBoxFehlerNummer->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxFehlerNummer->Location = System::Drawing::Point(156, 442);
      this->textBoxFehlerNummer->Name = L"textBoxFehlerNummer";
      this->textBoxFehlerNummer->ReadOnly = true;
      this->textBoxFehlerNummer->Size = System::Drawing::Size(100, 20);
      this->textBoxFehlerNummer->TabIndex = 2;
      // 
      // groupBox1
      // 
      this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBox1->Controls->Add(this->label8);
      this->groupBox1->Controls->Add(this->textBoxAttributName);
      this->groupBox1->Controls->Add(this->buttonRegelDetails);
      this->groupBox1->Controls->Add(this->label7);
      this->groupBox1->Controls->Add(this->textBoxReferiertesObjektId);
      this->groupBox1->Controls->Add(this->label6);
      this->groupBox1->Controls->Add(this->label3);
      this->groupBox1->Controls->Add(this->textBoxObjektId);
      this->groupBox1->Controls->Add(this->textBoxFehlerNummer);
      this->groupBox1->Controls->Add(this->buttonVor);
      this->groupBox1->Controls->Add(this->buttonZurueck);
      this->groupBox1->Controls->Add(this->buttonReferiertesObjektAttribute);
      this->groupBox1->Controls->Add(this->label5);
      this->groupBox1->Controls->Add(this->textBoxReferiertesObjektTyp);
      this->groupBox1->Controls->Add(this->label4);
      this->groupBox1->Controls->Add(this->textBoxRelation);
      this->groupBox1->Controls->Add(this->buttonObjektAttribute);
      this->groupBox1->Controls->Add(this->label2);
      this->groupBox1->Controls->Add(this->textBoxObjektTyp);
      this->groupBox1->Controls->Add(label1);
      this->groupBox1->Controls->Add(this->textBoxFehlerId);
      this->groupBox1->Controls->Add(this->textBoxFehlerBeschreibung);
      this->groupBox1->Location = System::Drawing::Point(2, 12);
      this->groupBox1->Name = L"groupBox1";
      this->groupBox1->Size = System::Drawing::Size(456, 480);
      this->groupBox1->TabIndex = 3;
      this->groupBox1->TabStop = false;
      this->groupBox1->Text = L"Konformitätsfehler";
      // 
      // label8
      // 
      this->label8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->label8->AutoSize = true;
      this->label8->Location = System::Drawing::Point(297, 215);
      this->label8->Name = L"label8";
      this->label8->Size = System::Drawing::Size(66, 13);
      this->label8->TabIndex = 21;
      this->label8->Text = L"Attributname";
      // 
      // textBoxAttributName
      // 
      this->textBoxAttributName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxAttributName->Location = System::Drawing::Point(6, 215);
      this->textBoxAttributName->Multiline = true;
      this->textBoxAttributName->Name = L"textBoxAttributName";
      this->textBoxAttributName->ReadOnly = true;
      this->textBoxAttributName->Size = System::Drawing::Size(284, 89);
      this->textBoxAttributName->TabIndex = 20;
      // 
      // buttonRegelDetails
      // 
      this->buttonRegelDetails->Location = System::Drawing::Point(124, 107);
      this->buttonRegelDetails->Name = L"buttonRegelDetails";
      this->buttonRegelDetails->Size = System::Drawing::Size(75, 23);
      this->buttonRegelDetails->TabIndex = 19;
      this->buttonRegelDetails->Text = L"Details";
      this->buttonRegelDetails->UseVisualStyleBackColor = true;
      this->buttonRegelDetails->Click += gcnew System::EventHandler(this, &KonformitaetsFehler::buttonRegelDetails_Click);
      // 
      // label7
      // 
      this->label7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->label7->AutoSize = true;
      this->label7->Location = System::Drawing::Point(300, 405);
      this->label7->Name = L"label7";
      this->label7->Size = System::Drawing::Size(50, 13);
      this->label7->TabIndex = 18;
      this->label7->Text = L"Objekt-Id";
      // 
      // textBoxReferiertesObjektId
      // 
      this->textBoxReferiertesObjektId->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxReferiertesObjektId->Location = System::Drawing::Point(6, 395);
      this->textBoxReferiertesObjektId->Name = L"textBoxReferiertesObjektId";
      this->textBoxReferiertesObjektId->ReadOnly = true;
      this->textBoxReferiertesObjektId->Size = System::Drawing::Size(285, 20);
      this->textBoxReferiertesObjektId->TabIndex = 17;
      // 
      // label6
      // 
      this->label6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->label6->AutoSize = true;
      this->label6->Location = System::Drawing::Point(297, 379);
      this->label6->Name = L"label6";
      this->label6->Size = System::Drawing::Size(59, 13);
      this->label6->TabIndex = 16;
      this->label6->Text = L"Objekt Typ";
      // 
      // label3
      // 
      this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(297, 189);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(50, 13);
      this->label3->TabIndex = 15;
      this->label3->Text = L"Objekt-Id";
      // 
      // textBoxObjektId
      // 
      this->textBoxObjektId->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxObjektId->Location = System::Drawing::Point(7, 183);
      this->textBoxObjektId->Name = L"textBoxObjektId";
      this->textBoxObjektId->ReadOnly = true;
      this->textBoxObjektId->Size = System::Drawing::Size(284, 20);
      this->textBoxObjektId->TabIndex = 14;
      // 
      // buttonReferiertesObjektAttribute
      // 
      this->buttonReferiertesObjektAttribute->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonReferiertesObjektAttribute->Location = System::Drawing::Point(365, 369);
      this->buttonReferiertesObjektAttribute->Name = L"buttonReferiertesObjektAttribute";
      this->buttonReferiertesObjektAttribute->Size = System::Drawing::Size(75, 42);
      this->buttonReferiertesObjektAttribute->TabIndex = 13;
      this->buttonReferiertesObjektAttribute->Text = L"  Alle  Attribute";
      this->buttonReferiertesObjektAttribute->UseVisualStyleBackColor = true;
      this->buttonReferiertesObjektAttribute->Click += gcnew System::EventHandler(this, &KonformitaetsFehler::buttonReferiertesObjektAttribute_Click);
      // 
      // label5
      // 
      this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->label5->AutoSize = true;
      this->label5->Location = System::Drawing::Point(10, 326);
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size(92, 13);
      this->label5->TabIndex = 11;
      this->label5->Text = L"Referiertes Objekt";
      // 
      // textBoxReferiertesObjektTyp
      // 
      this->textBoxReferiertesObjektTyp->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxReferiertesObjektTyp->Location = System::Drawing::Point(7, 369);
      this->textBoxReferiertesObjektTyp->Name = L"textBoxReferiertesObjektTyp";
      this->textBoxReferiertesObjektTyp->ReadOnly = true;
      this->textBoxReferiertesObjektTyp->Size = System::Drawing::Size(285, 20);
      this->textBoxReferiertesObjektTyp->TabIndex = 10;
      // 
      // label4
      // 
      this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->label4->AutoSize = true;
      this->label4->Location = System::Drawing::Point(300, 352);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(46, 13);
      this->label4->TabIndex = 9;
      this->label4->Text = L"Relation";
      // 
      // textBoxRelation
      // 
      this->textBoxRelation->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxRelation->Location = System::Drawing::Point(7, 342);
      this->textBoxRelation->Name = L"textBoxRelation";
      this->textBoxRelation->ReadOnly = true;
      this->textBoxRelation->Size = System::Drawing::Size(285, 20);
      this->textBoxRelation->TabIndex = 8;
      // 
      // buttonObjektAttribute
      // 
      this->buttonObjektAttribute->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonObjektAttribute->Location = System::Drawing::Point(365, 156);
      this->buttonObjektAttribute->Name = L"buttonObjektAttribute";
      this->buttonObjektAttribute->Size = System::Drawing::Size(75, 46);
      this->buttonObjektAttribute->TabIndex = 7;
      this->buttonObjektAttribute->Text = L"  Alle  Attribute";
      this->buttonObjektAttribute->UseVisualStyleBackColor = true;
      this->buttonObjektAttribute->Click += gcnew System::EventHandler(this, &KonformitaetsFehler::buttonObjektAttribute_Click);
      // 
      // label2
      // 
      this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(297, 159);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(59, 13);
      this->label2->TabIndex = 4;
      this->label2->Text = L"Objekt Typ";
      // 
      // textBoxObjektTyp
      // 
      this->textBoxObjektTyp->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxObjektTyp->Location = System::Drawing::Point(6, 156);
      this->textBoxObjektTyp->Name = L"textBoxObjektTyp";
      this->textBoxObjektTyp->ReadOnly = true;
      this->textBoxObjektTyp->Size = System::Drawing::Size(285, 20);
      this->textBoxObjektTyp->TabIndex = 3;
      // 
      // textBoxFehlerId
      // 
      this->textBoxFehlerId->Location = System::Drawing::Point(6, 109);
      this->textBoxFehlerId->Name = L"textBoxFehlerId";
      this->textBoxFehlerId->ReadOnly = true;
      this->textBoxFehlerId->Size = System::Drawing::Size(88, 20);
      this->textBoxFehlerId->TabIndex = 1;
      // 
      // textBoxFehlerBeschreibung
      // 
      this->textBoxFehlerBeschreibung->AcceptsReturn = true;
      this->textBoxFehlerBeschreibung->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxFehlerBeschreibung->BackColor = System::Drawing::SystemColors::Control;
      this->textBoxFehlerBeschreibung->ForeColor = System::Drawing::Color::Red;
      this->textBoxFehlerBeschreibung->HideSelection = false;
      this->textBoxFehlerBeschreibung->Location = System::Drawing::Point(6, 19);
      this->textBoxFehlerBeschreibung->Multiline = true;
      this->textBoxFehlerBeschreibung->Name = L"textBoxFehlerBeschreibung";
      this->textBoxFehlerBeschreibung->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
      this->textBoxFehlerBeschreibung->Size = System::Drawing::Size(444, 75);
      this->textBoxFehlerBeschreibung->TabIndex = 0;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonBeenden->Location = System::Drawing::Point(377, 527);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 5;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      // 
      // KonformitaetsFehler
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(462, 560);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->groupBox1);
      this->Name = L"KonformitaetsFehler";
      this->Text = L"Nicht-geometrische Konformitäts-Fehler ";
      this->groupBox1->ResumeLayout(false);
      this->groupBox1->PerformLayout();
      this->ResumeLayout(false);

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Anzeige des aktuellen Konformitätsfehlers im Dialog                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void displayConformityError ()
         {
           String          ^ STR;

           STR = String::Concat( "Fehler ", Convert::ToString(indexConformityError+1), " von ", 
             Convert::ToString(m_pErrorReport->getSemanticErrorAnz()) );
           textBoxFehlerNummer->Text = STR;

           NamesValuesList * pNamesValueList = m_ConformityErrorAct->getAttributes();
           if (  pNamesValueList != NULL )
             textBoxAttributName->Text = QuConvert::ToString( pNamesValueList->getAllNames() );
           else
             textBoxAttributName->Text = "";

           STR = QuConvert::ToString ( m_ConformityErrorAct->getErrorName() );
           if ( m_ConformityErrorAct->getErrorDescription() != "" )
           {
             STR = String::Concat( STR, Environment::NewLine, Environment::NewLine,  QuConvert::ToString ( m_ConformityErrorAct->getErrorDescription() ) );
           }
           textBoxFehlerBeschreibung->Text   = STR;

           textBoxFehlerId->Text             = QuConvert::ToString ( m_ConformityErrorAct->getErrorId() );
           textBoxObjektTyp->Text            = QuConvert::ToString( m_ConformityErrorAct->getFeatureType() );
           textBoxObjektId->Text  =  QuConvert::ToString( m_ConformityErrorAct->getFeatureId() );
           textBoxRelation->Text             = QuConvert::ToString( m_ConformityErrorAct->getRelationName() );
           textBoxReferiertesObjektTyp->Text = QuConvert::ToString( m_ConformityErrorAct->getRelatedFeatureType() );
           textBoxReferiertesObjektId->Text =  QuConvert::ToString( m_ConformityErrorAct->getRelatedFeatureId() );

           if ( m_ConformityErrorAct->getBrokenRule() == NULL )
           {
             buttonRegelDetails->Enabled = false;
           }
           else
           {
             buttonRegelDetails->Enabled = true;
           }

         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "<--"                                              //
//  Auswahl des vorhergehenden Fehlers in der Liste                          //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonZurueck_Click(System::Object^  sender, System::EventArgs^  e) 
{
  if ( indexConformityError == 0 ) return;

  indexConformityError = indexConformityError--;
  m_ConformityErrorAct = m_pErrorReport->getSemanticError( indexConformityError );
  displayConformityError();

  if ( indexConformityError == 0 )
  {
    buttonZurueck->Enabled = false;
    buttonVor->Enabled     = true;
  }
  else
  if ( indexConformityError == m_pErrorReport->getSemanticErrorAnz()-1 )
  {
    buttonZurueck->Enabled = true;
    buttonVor->Enabled     = false;
  }
  else
  {
    buttonZurueck->Enabled = true;
    buttonVor->Enabled     = true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "-->"                                              //
//  Auswahl des nächsten Fehlers in der Liste                                //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonVor_Click(System::Object^  sender, System::EventArgs^  e) 
{
  if ( indexConformityError == m_pErrorReport->getSemanticErrorAnz()-1 ) return;

  indexConformityError = indexConformityError++;
  m_ConformityErrorAct = m_pErrorReport->getSemanticError( indexConformityError );
  displayConformityError();

  if ( indexConformityError == 0 )
  {
    buttonZurueck->Enabled = false;
    buttonVor->Enabled     = true;
  }
  else
    if ( indexConformityError == m_pErrorReport->getSemanticErrorAnz()-1 )
    {
      buttonZurueck->Enabled = true;
      buttonVor->Enabled     = false;
    }
    else
    {
      buttonZurueck->Enabled = true;
      buttonVor->Enabled     = true;
    }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Alle Attribute" (Fehlerhaftes Objekt)             //
//  Anzeige von Informationen über das Objekt, in dem der Fehler aufgetreten //
//  ist                                                                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonObjektAttribute_Click(System::Object^  sender, System::EventArgs^  e) 
{
  string             gmlId;
  Feature       * pFeature;
  FeatureAttribute ^ pFeatureAttribute;
  NamesValuesList  * pList;
  int                i;
  string             name, wert;
  ArrayList        ^ highlight;

  gmlId = m_ConformityErrorAct->getFeatureId();
  pFeature = m_pFeatures->getFeature( gmlId );

  highlight = gcnew ArrayList;
  pList = m_ConformityErrorAct->getAttributes();
  if ( pList != NULL )
  {
    for ( i = 0; i < pList->getNamesValuesAnz(); i++ )
    {
      pList->getNameValue( i, name, wert );
      highlight->Add( QuConvert::ToString(name) );
    }
  }

  pFeatureAttribute  = gcnew FeatureAttribute ( m_pFeatures, pFeature, highlight );
  pFeatureAttribute->ShowDialog();
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Alle Attribute" (Referiertes Objekt)              //
//  Anzeige von Informationen über ein vom fehlerhaften Objekt evtl.         //
//  referiertes Objekt                                                       //                                                                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonReferiertesObjektAttribute_Click(System::Object^  sender, System::EventArgs^  e) 
{
  string             gmlId;
  Feature       * pFeature;
  FeatureAttribute ^ pFeatureAttribute;
  NamesValuesList  * pList;
  int                i;
  string             name, wert;
  ArrayList        ^ highlight;

  gmlId = m_ConformityErrorAct->getRelatedFeatureId();
  pFeature = m_pFeatures->getFeature( gmlId );

  highlight = gcnew ArrayList;
  pList = m_ConformityErrorAct->getAttributes();
  if ( pList != NULL )
  {
    for ( i = 0; i < pList->getNamesValuesAnz(); i++ )
    {
      pList->getNameValue( i, name, wert );
      highlight->Add( QuConvert::ToString(name) );
    }
  }

  pFeatureAttribute  = gcnew FeatureAttribute ( m_pFeatures, pFeature, highlight );
  pFeatureAttribute->ShowDialog();
}
         
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Details"                                          //
//  Anzeige detaillierter Informationen über die verletzte Konformitätsregel //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonRegelDetails_Click(System::Object^  sender, System::EventArgs^  e)
         {
           KonformitätsregelDetails ^ pDialog = gcnew KonformitätsregelDetails ( m_ConformityErrorAct->getBrokenRule() );
           pDialog->ShowDialog();
         }
};
