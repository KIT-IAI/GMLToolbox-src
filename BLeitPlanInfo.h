#pragma once

#include <fstream>

#include "convert.h"
#include "Feature.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

	/// <summary> 
	/// Zusammenfassung für BLeitPlanInfo
	///
	/// Achtung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie die Eigenschaft
	///          'Ressourcendateiname' für das Compilertool für verwaltete Ressourcen ändern, 
	///          das allen .resx-Dateien zugewiesen ist, von denen diese Klasse abhängt. 
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class BLeitPlanInfo : public System::Windows::Forms::Form
	{
	public: 
		BLeitPlanInfo(BLeitPlan * pBLeitPlanP )
		{
			int      gkz;
			bool     success;
			string   stdString;

			InitializeComponent();

			pBLeitPlan = pBLeitPlanP;

			XPLAN_TYP typ = pBLeitPlan->getPlanTyp();
			if ( typ == BPLAN )	PlanTyp->Text = "Bebauungsplan";

			success = pBLeitPlan->getIntegerAttributWert ( "gkz", gkz );
			if ( success ) GKZ->Text = Convert::ToString ( gkz );

			success = pBLeitPlan->getStringAttributWert ( "stadt", stdString );
			if ( success ) Gemeinde->Text = QuConvert::ToString ( stdString );

			success = pBLeitPlan->getStringAttributWert ( "name", stdString );
			if ( success ) PlanName->Text = QuConvert::ToString ( stdString );

			success = pBLeitPlan->getStringAttributWert ( "nummer", stdString );
			if ( success ) PlanNummer->Text = QuConvert::ToString ( stdString );

			success = pBLeitPlan->getStringAttributWert ( "datum", stdString );
			if ( success ) HerstellDatum->Text = QuConvert::ToString ( stdString );

			if ( typ == BPLAN )	
			{
				WirksamkeitsDatumlabel->Visible = false;
				WirksamkeitsDatum->Visible = false;
				AenderungenBisLabel->Visible = false;
				AenderungenBis->Visible = false;
			}
			else
			{

			}

			success = pBLeitPlan->getStringAttributWert ( "hoehenbezug", stdString );
			if ( success ) Hoehenbezug->Text = QuConvert::ToString ( stdString );

			success = pBLeitPlan->getStringAttributWert ( "verfahrensstand", stdString );
			if ( success ) VerfahrensStand->Text = QuConvert::ToString ( stdString );

			success = pBLeitPlan->getStringAttributWert ( "beschreibung", stdString );
			if ( success ) Beschreibung->Text = QuConvert::ToString ( stdString );

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
	private: System::Windows::Forms::Label ^  PlanTyp;
	private: System::Windows::Forms::Label ^  GKZ;
	private: System::Windows::Forms::Label ^  PlanName;
	private: System::Windows::Forms::Label ^  PlanNummer;
	private: System::Windows::Forms::Label ^  HerstellDatumLabel;
	private: System::Windows::Forms::Label ^  HerstellDatum;
	private: System::Windows::Forms::Label ^  WirksamkeitsDatumlabel;
	private: System::Windows::Forms::Label ^  WirksamkeitsDatum;
	private: System::Windows::Forms::Label ^  AenderungenBisLabel;
	private: System::Windows::Forms::Label ^  AenderungenBis;
	private: System::Windows::Forms::TextBox ^  Beschreibung;
	private: System::Windows::Forms::Label ^  VerfehrensStandLabel;
	private: System::Windows::Forms::Label ^  VerfahrensStand;
	private: System::Windows::Forms::ComboBox ^  AttributAuswahl;
	private: System::Windows::Forms::TextBox ^  textBoxAttribut;
	private: System::Windows::Forms::Label ^  HoehenbezugLabel;
	private: System::Windows::Forms::Label ^  Hoehenbezug;
	private: System::Windows::Forms::Label ^  GemeindeLabel;
	private: System::Windows::Forms::Label ^  GkzLabel;
	private: System::Windows::Forms::Label ^  Gemeinde;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container^ components;
		BLeitPlan     * pBLeitPlan;

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->PlanTyp = (gcnew System::Windows::Forms::Label());
      this->GemeindeLabel = (gcnew System::Windows::Forms::Label());
      this->Gemeinde = (gcnew System::Windows::Forms::Label());
      this->GkzLabel = (gcnew System::Windows::Forms::Label());
      this->GKZ = (gcnew System::Windows::Forms::Label());
      this->PlanName = (gcnew System::Windows::Forms::Label());
      this->PlanNummer = (gcnew System::Windows::Forms::Label());
      this->HerstellDatumLabel = (gcnew System::Windows::Forms::Label());
      this->HerstellDatum = (gcnew System::Windows::Forms::Label());
      this->WirksamkeitsDatumlabel = (gcnew System::Windows::Forms::Label());
      this->WirksamkeitsDatum = (gcnew System::Windows::Forms::Label());
      this->AenderungenBisLabel = (gcnew System::Windows::Forms::Label());
      this->AenderungenBis = (gcnew System::Windows::Forms::Label());
      this->Beschreibung = (gcnew System::Windows::Forms::TextBox());
      this->VerfehrensStandLabel = (gcnew System::Windows::Forms::Label());
      this->VerfahrensStand = (gcnew System::Windows::Forms::Label());
      this->AttributAuswahl = (gcnew System::Windows::Forms::ComboBox());
      this->textBoxAttribut = (gcnew System::Windows::Forms::TextBox());
      this->HoehenbezugLabel = (gcnew System::Windows::Forms::Label());
      this->Hoehenbezug = (gcnew System::Windows::Forms::Label());
      this->SuspendLayout();
      // 
      // PlanTyp
      // 
      this->PlanTyp->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->PlanTyp->Location = System::Drawing::Point(16, 56);
      this->PlanTyp->Name = L"PlanTyp";
      this->PlanTyp->Size = System::Drawing::Size(128, 23);
      this->PlanTyp->TabIndex = 0;
      this->PlanTyp->Text = L"Flächennutzungsplan";
      // 
      // GemeindeLabel
      // 
      this->GemeindeLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->GemeindeLabel->Location = System::Drawing::Point(16, 24);
      this->GemeindeLabel->Name = L"GemeindeLabel";
      this->GemeindeLabel->Size = System::Drawing::Size(64, 23);
      this->GemeindeLabel->TabIndex = 2;
      this->GemeindeLabel->Text = L"Gemeinde";
      // 
      // Gemeinde
      // 
      this->Gemeinde->Location = System::Drawing::Point(104, 24);
      this->Gemeinde->Name = L"Gemeinde";
      this->Gemeinde->Size = System::Drawing::Size(216, 23);
      this->Gemeinde->TabIndex = 3;
      this->Gemeinde->Text = L"Eggenstein-Leopoldshafen";
      // 
      // GkzLabel
      // 
      this->GkzLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->GkzLabel->Location = System::Drawing::Point(336, 24);
      this->GkzLabel->Name = L"GkzLabel";
      this->GkzLabel->Size = System::Drawing::Size(32, 23);
      this->GkzLabel->TabIndex = 4;
      this->GkzLabel->Text = L"Gkz";
      // 
      // GKZ
      // 
      this->GKZ->Location = System::Drawing::Point(384, 24);
      this->GKZ->Name = L"GKZ";
      this->GKZ->Size = System::Drawing::Size(100, 23);
      this->GKZ->TabIndex = 5;
      this->GKZ->Text = L"Gkz";
      // 
      // PlanName
      // 
      this->PlanName->Location = System::Drawing::Point(144, 56);
      this->PlanName->Name = L"PlanName";
      this->PlanName->Size = System::Drawing::Size(152, 23);
      this->PlanName->TabIndex = 6;
      this->PlanName->Text = L"Name des Plans";
      // 
      // PlanNummer
      // 
      this->PlanNummer->Location = System::Drawing::Point(336, 56);
      this->PlanNummer->Name = L"PlanNummer";
      this->PlanNummer->Size = System::Drawing::Size(100, 23);
      this->PlanNummer->TabIndex = 7;
      this->PlanNummer->Text = L"Nummer";
      // 
      // HerstellDatumLabel
      // 
      this->HerstellDatumLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->HerstellDatumLabel->Location = System::Drawing::Point(16, 88);
      this->HerstellDatumLabel->Name = L"HerstellDatumLabel";
      this->HerstellDatumLabel->Size = System::Drawing::Size(112, 23);
      this->HerstellDatumLabel->TabIndex = 8;
      this->HerstellDatumLabel->Text = L"Herstellungs Datum";
      // 
      // HerstellDatum
      // 
      this->HerstellDatum->Location = System::Drawing::Point(136, 88);
      this->HerstellDatum->Name = L"HerstellDatum";
      this->HerstellDatum->Size = System::Drawing::Size(64, 23);
      this->HerstellDatum->TabIndex = 9;
      this->HerstellDatum->Text = L"21.07.2005";
      // 
      // WirksamkeitsDatumlabel
      // 
      this->WirksamkeitsDatumlabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->WirksamkeitsDatumlabel->Location = System::Drawing::Point(216, 88);
      this->WirksamkeitsDatumlabel->Name = L"WirksamkeitsDatumlabel";
      this->WirksamkeitsDatumlabel->Size = System::Drawing::Size(120, 23);
      this->WirksamkeitsDatumlabel->TabIndex = 10;
      this->WirksamkeitsDatumlabel->Text = L"Wirksamkeits Datum";
      // 
      // WirksamkeitsDatum
      // 
      this->WirksamkeitsDatum->Location = System::Drawing::Point(336, 88);
      this->WirksamkeitsDatum->Name = L"WirksamkeitsDatum";
      this->WirksamkeitsDatum->Size = System::Drawing::Size(64, 23);
      this->WirksamkeitsDatum->TabIndex = 11;
      this->WirksamkeitsDatum->Text = L"21.07.2005";
      // 
      // AenderungenBisLabel
      // 
      this->AenderungenBisLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->AenderungenBisLabel->Location = System::Drawing::Point(416, 88);
      this->AenderungenBisLabel->Name = L"AenderungenBisLabel";
      this->AenderungenBisLabel->Size = System::Drawing::Size(88, 23);
      this->AenderungenBisLabel->TabIndex = 12;
      this->AenderungenBisLabel->Text = L"Änderungen bi";
      // 
      // AenderungenBis
      // 
      this->AenderungenBis->Location = System::Drawing::Point(528, 88);
      this->AenderungenBis->Name = L"AenderungenBis";
      this->AenderungenBis->Size = System::Drawing::Size(100, 23);
      this->AenderungenBis->TabIndex = 13;
      this->AenderungenBis->Text = L"22.07.2005";
      // 
      // Beschreibung
      // 
      this->Beschreibung->BackColor = System::Drawing::SystemColors::Control;
      this->Beschreibung->Location = System::Drawing::Point(16, 184);
      this->Beschreibung->Multiline = true;
      this->Beschreibung->Name = L"Beschreibung";
      this->Beschreibung->ReadOnly = true;
      this->Beschreibung->Size = System::Drawing::Size(664, 64);
      this->Beschreibung->TabIndex = 14;
      this->Beschreibung->Text = L"Beschreibung";
      // 
      // VerfehrensStandLabel
      // 
      this->VerfehrensStandLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->VerfehrensStandLabel->Location = System::Drawing::Point(16, 152);
      this->VerfehrensStandLabel->Name = L"VerfehrensStandLabel";
      this->VerfehrensStandLabel->Size = System::Drawing::Size(144, 23);
      this->VerfehrensStandLabel->TabIndex = 15;
      this->VerfehrensStandLabel->Text = L"Stand des Verfahrens";
      // 
      // VerfahrensStand
      // 
      this->VerfahrensStand->Location = System::Drawing::Point(152, 152);
      this->VerfahrensStand->Name = L"VerfahrensStand";
      this->VerfahrensStand->Size = System::Drawing::Size(144, 23);
      this->VerfahrensStand->TabIndex = 16;
      this->VerfahrensStand->Text = L"Aufstellungsbeschluss";
      // 
      // AttributAuswahl
      // 
      this->AttributAuswahl->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->AttributAuswahl->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"Begründung", L"Umweltbericht", L"Realisierung", 
        L"Textliche Festsetzungen"});
      this->AttributAuswahl->Location = System::Drawing::Point(16, 280);
      this->AttributAuswahl->Name = L"AttributAuswahl";
      this->AttributAuswahl->Size = System::Drawing::Size(144, 21);
      this->AttributAuswahl->TabIndex = 17;
      this->AttributAuswahl->Text = L"Attribut Auswahl";
      this->AttributAuswahl->SelectedIndexChanged += gcnew System::EventHandler(this, &BLeitPlanInfo::AttributAuswahl_SelectedIndexChanged);
      // 
      // textBoxAttribut
      // 
      this->textBoxAttribut->Location = System::Drawing::Point(16, 304);
      this->textBoxAttribut->Multiline = true;
      this->textBoxAttribut->Name = L"textBoxAttribut";
      this->textBoxAttribut->ReadOnly = true;
      this->textBoxAttribut->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
      this->textBoxAttribut->Size = System::Drawing::Size(664, 136);
      this->textBoxAttribut->TabIndex = 18;
      // 
      // HoehenbezugLabel
      // 
      this->HoehenbezugLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->HoehenbezugLabel->Location = System::Drawing::Point(16, 120);
      this->HoehenbezugLabel->Name = L"HoehenbezugLabel";
      this->HoehenbezugLabel->Size = System::Drawing::Size(80, 23);
      this->HoehenbezugLabel->TabIndex = 19;
      this->HoehenbezugLabel->Text = L"Höhenbezug";
      // 
      // Hoehenbezug
      // 
      this->Hoehenbezug->Location = System::Drawing::Point(112, 120);
      this->Hoehenbezug->Name = L"Hoehenbezug";
      this->Hoehenbezug->Size = System::Drawing::Size(100, 23);
      this->Hoehenbezug->TabIndex = 20;
      this->Hoehenbezug->Text = L"NN";
      // 
      // BLeitPlanInfo
      // 
      this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
      this->ClientSize = System::Drawing::Size(688, 453);
      this->Controls->Add(this->Hoehenbezug);
      this->Controls->Add(this->HoehenbezugLabel);
      this->Controls->Add(this->textBoxAttribut);
      this->Controls->Add(this->AttributAuswahl);
      this->Controls->Add(this->VerfahrensStand);
      this->Controls->Add(this->VerfehrensStandLabel);
      this->Controls->Add(this->Beschreibung);
      this->Controls->Add(this->AenderungenBis);
      this->Controls->Add(this->AenderungenBisLabel);
      this->Controls->Add(this->WirksamkeitsDatum);
      this->Controls->Add(this->WirksamkeitsDatumlabel);
      this->Controls->Add(this->HerstellDatum);
      this->Controls->Add(this->HerstellDatumLabel);
      this->Controls->Add(this->PlanNummer);
      this->Controls->Add(this->PlanName);
      this->Controls->Add(this->GKZ);
      this->Controls->Add(this->GkzLabel);
      this->Controls->Add(this->Gemeinde);
      this->Controls->Add(this->GemeindeLabel);
      this->Controls->Add(this->PlanTyp);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"BLeitPlanInfo";
      this->Text = L"BLeitPlanInfo";
      this->ResumeLayout(false);
      this->PerformLayout();

    }		
	private: System::Void AttributAuswahl_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
			 {
				 string   stdString;
				 bool     success;

				 int index = AttributAuswahl->SelectedIndex;

				 switch ( index )
				 {
				 case 0:  //  Attribut Begruendung
			        success = pBLeitPlan->getStringAttributWert ( "begruendung", stdString );
					if ( success ) textBoxAttribut->Text = QuConvert::ToString ( stdString );
					break;

				 case 1:  //  Attribut Realisierung
			        success = pBLeitPlan->getStringAttributWert ( "realisierung", stdString );
					if ( success ) textBoxAttribut->Text = QuConvert::ToString ( stdString );
					break;

				 case 2:  //  Attribut Umweltbericht
			        success = pBLeitPlan->getStringAttributWert ( "umweltbericht", stdString );
					if ( success ) textBoxAttribut->Text = QuConvert::ToString ( stdString );
					break;

				 case 3:  //  TextlicheFestsetzungen
				 {
					 vector<string> schluessel;
					 vector<string> werte;

					textBoxAttribut->Clear();
				 }

				 }
			 }

};
