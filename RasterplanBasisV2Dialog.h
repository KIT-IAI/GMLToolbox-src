#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Convert.h"
#include "ShpFileNamenKonv.h"

	/// <summary> 
	///  Nur relevant für XPlanGML 2.0: Festlegung der Attributwerte von XP_RasterplanBasis
	///
	/// </summary>
	public ref class RasterplanBasisV2Dialog : public System::Windows::Forms::Form
	{
	public: 
		RasterplanBasisV2Dialog( Bereich * pBereichP )
		{
			InitializeComponent();

			pBereich = pBereichP;
			pRasterplanBasis = pBereich->getRasterplanBasis();
			if ( pRasterplanBasis == NULL )
			{
				pRasterplanBasis = new RasterplanBasis();
				pBereich->setRasterplanBasis ( pRasterplanBasis );
			}

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
	private: System::Windows::Forms::Label ^  label1;
	private: System::Windows::Forms::Label ^  label2;
	private: System::Windows::Forms::Label ^  label3;
	private: System::Windows::Forms::Label ^  label4;
	private: System::Windows::Forms::TextBox ^  textBoxURLPlan;
	private: System::Windows::Forms::TextBox ^  textBoxURLGeoref;
	private: System::Windows::Forms::TextBox ^  textBoxURLLegende;
	private: System::Windows::Forms::TextBox ^  textBoxURLTexte;
	private: System::Windows::Forms::Button ^  buttonBeenden;
	private: System::Windows::Forms::Button ^  buttonAbbrechen;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container^ components;

		Bereich         * pBereich;						// Plan-Bereich
		RasterplanBasis * pRasterplanBasis;		// Attribute XP_RasterplanBasis

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBoxURLPlan = gcnew System::Windows::Forms::TextBox();
			this->label1 = gcnew System::Windows::Forms::Label();
			this->textBoxURLGeoref = gcnew System::Windows::Forms::TextBox();
			this->label2 = gcnew System::Windows::Forms::Label();
			this->textBoxURLLegende = gcnew System::Windows::Forms::TextBox();
			this->label3 = gcnew System::Windows::Forms::Label();
			this->textBoxURLTexte = gcnew System::Windows::Forms::TextBox();
			this->label4 = gcnew System::Windows::Forms::Label();
			this->buttonBeenden = gcnew System::Windows::Forms::Button();
			this->buttonAbbrechen = gcnew System::Windows::Forms::Button();
			this->SuspendLayout();
			// 
			// textBoxURLPlan
			// 
			this->textBoxURLPlan->Location = System::Drawing::Point(16, 32);
			this->textBoxURLPlan->Name = "textBoxURLPlan";
			this->textBoxURLPlan->Size = System::Drawing::Size(304, 20);
			this->textBoxURLPlan->TabIndex = 0;
			this->textBoxURLPlan->Text = "";
			// 
			// label1
			// 
			this->label1->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->label1->Location = System::Drawing::Point(16, 56);
			this->label1->Name = "label1";
			this->label1->TabIndex = 1;
			this->label1->Text = "URL des Plans";
			// 
			// textBoxURLGeoref
			// 
			this->textBoxURLGeoref->Location = System::Drawing::Point(16, 80);
			this->textBoxURLGeoref->Name = "textBoxURLGeoref";
			this->textBoxURLGeoref->Size = System::Drawing::Size(304, 20);
			this->textBoxURLGeoref->TabIndex = 2;
			this->textBoxURLGeoref->Text = "";
			// 
			// label2
			// 
			this->label2->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->label2->Location = System::Drawing::Point(16, 104);
			this->label2->Name = "label2";
			this->label2->Size = System::Drawing::Size(200, 23);
			this->label2->TabIndex = 3;
			this->label2->Text = "URL der Georeferenzierungsdatei";
			// 
			// textBoxURLLegende
			// 
			this->textBoxURLLegende->Location = System::Drawing::Point(16, 136);
			this->textBoxURLLegende->Name = "textBoxURLLegende";
			this->textBoxURLLegende->Size = System::Drawing::Size(304, 20);
			this->textBoxURLLegende->TabIndex = 4;
			this->textBoxURLLegende->Text = "";
			// 
			// label3
			// 
			this->label3->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->label3->Location = System::Drawing::Point(16, 160);
			this->label3->Name = "label3";
			this->label3->TabIndex = 5;
			this->label3->Text = "URL der Legende";
			// 
			// textBoxURLTexte
			// 
			this->textBoxURLTexte->Location = System::Drawing::Point(16, 192);
			this->textBoxURLTexte->Name = "textBoxURLTexte";
			this->textBoxURLTexte->Size = System::Drawing::Size(304, 20);
			this->textBoxURLTexte->TabIndex = 6;
			this->textBoxURLTexte->Text = "";
			// 
			// label4
			// 
			this->label4->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->label4->Location = System::Drawing::Point(16, 216);
			this->label4->Name = "label4";
			this->label4->Size = System::Drawing::Size(232, 23);
			this->label4->TabIndex = 7;
			this->label4->Text = "URL der textuellen Planinhalte";
			// 
			// buttonBeenden
			// 
			this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->buttonBeenden->Location = System::Drawing::Point(248, 264);
			this->buttonBeenden->Name = "buttonBeenden";
			this->buttonBeenden->TabIndex = 8;
			this->buttonBeenden->Text = "Beenden";
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &RasterplanBasisV2Dialog::buttonBeenden_Click);
			// 
			// buttonAbbrechen
			// 
			this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->buttonAbbrechen->Location = System::Drawing::Point(16, 264);
			this->buttonAbbrechen->Name = "buttonAbbrechen";
			this->buttonAbbrechen->TabIndex = 9;
			this->buttonAbbrechen->Text = "Abbrechen";
			this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &RasterplanBasisV2Dialog::buttonAbbrechen_Click);
			// 
			// RasterplanBasisV2Dialog
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(336, 302);
			this->Controls->Add(this->buttonAbbrechen);
			this->Controls->Add(this->buttonBeenden);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->textBoxURLTexte);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBoxURLLegende);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBoxURLGeoref);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBoxURLPlan);
			this->Name = "RasterplanBasisV2Dialog";
			this->Text = "Rasterdarstellung Basisplan";
			this->ResumeLayout(false);

		}		
	
///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Dialogbox																							 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void initialize()
		 {
			String ^ STR;
			string   stdString;
			ExterneReferenz * pExterneReferenz;

			pExterneReferenz = pRasterplanBasis->getExterneReferenzText();
			if ( pExterneReferenz == NULL )
			{
				pExterneReferenz = new ExterneReferenz ();
        pExterneReferenz->relationName = "xplan:refText";
				pRasterplanBasis->addExterneReferenz( pExterneReferenz );
			}
			else
			{
				stdString = pExterneReferenz->referenzURL;
				if ( stdString != "" )
					STR = QuConvert::ToString ( stdString );
				else
					STR = nullptr;

				textBoxURLTexte->Text = STR;
			}

			if ( pRasterplanBasis->getExterneReferenzenLegendeAnz() == 0 )
			{
				pExterneReferenz = new ExterneReferenz ();
        pExterneReferenz->relationName = "xplan:refLegende";
				pRasterplanBasis->addExterneReferenz( pExterneReferenz );
			}
			else
			{
				pExterneReferenz = pRasterplanBasis->getExterneReferenzLegende ( 0 );
				stdString = pExterneReferenz->referenzURL;
				if ( stdString != "" )
					STR = QuConvert::ToString ( stdString );
				else
					STR = nullptr;

				textBoxURLLegende->Text = STR;

			}

			if ( pRasterplanBasis->getExterneReferenzenPlanAnz() == 0 )
			{
				pExterneReferenz = new ExterneReferenz ();
        pExterneReferenz->relationName = "xplan:refScan";
				pRasterplanBasis->addExterneReferenz( pExterneReferenz );
			}
			else
			{
				pExterneReferenz = pRasterplanBasis->getExterneReferenzPlan ( 0 );
				stdString = pExterneReferenz->referenzURL;
				if ( stdString != "" )
					STR = QuConvert::ToString ( stdString );
				else
					STR = nullptr;

				textBoxURLPlan->Text = STR;

				stdString = pExterneReferenz->georefURL;
				if ( stdString != "" )
					STR = QuConvert::ToString ( stdString );
				else
					STR = nullptr;

				textBoxURLGeoref->Text = STR;
			}
			
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Abbrechen"																				 //
//  Beendet den Dialog																										   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAbbrechen_Click(System::Object ^  sender, System::EventArgs ^  e)
		 {
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Abbrechen"																				 //
//  Beendet den Dialog und übernimmt die Eingaben													   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object ^  sender, System::EventArgs ^  e)
		 {
			string            stdString;
 			ExterneReferenz * pExterneReferenz;

			pExterneReferenz = pRasterplanBasis->getExterneReferenzPlan( 0 );
			QuConvert::systemStr2stdStr ( stdString, textBoxURLPlan->Text );
			pExterneReferenz->referenzURL = stdString;
			QuConvert::systemStr2stdStr ( stdString, textBoxURLGeoref->Text );
			pExterneReferenz->georefURL = stdString;

			pExterneReferenz = pRasterplanBasis->getExterneReferenzText();
			QuConvert::systemStr2stdStr ( stdString, textBoxURLTexte->Text );
			pExterneReferenz->referenzURL = stdString;

			pExterneReferenz = pRasterplanBasis->getExterneReferenzLegende( 0 );
			QuConvert::systemStr2stdStr ( stdString, textBoxURLLegende->Text );
			pExterneReferenz->referenzURL = stdString;
		 }

};
