#pragma once

#include "SrsManagerRef.h"
#include "Feature.h"
#include "Checking.h"
#include "Convert.h"
#include <vector>

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

  using namespace std;

	/// <summary>
	/// Durchf�hrung spezifischer geometrischer Korrekturen
  /// Aktuell: Umwandlung von Einschn�rungen der der Polygon-Au�enkontur in Innenkonturen in allen Features
	/// </summary>
	public ref class GeometryKorrektur : public System::Windows::Forms::Form
	{
	public:
		GeometryKorrektur( Features * pFeaturesP, Checking * pCheckingP )
		{
			InitializeComponent();

      pChecking    = pCheckingP;
      pFeatures    = pFeaturesP;
      pErrorReport = pChecking->getErrorReport();

      checkBoxContourTouchSingle->Checked = true;		
      checkBoxContourTouchMulti->Checked = true;		
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~GeometryKorrektur()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::ListBox^  listBoxMeldungen;
  private: System::Windows::Forms::Button^  buttonAusf�hren;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::CheckBox^  checkBoxContourTouchSingle;
  private: System::Windows::Forms::CheckBox^  checkBoxContourTouchMulti;

	private:
    Checking    * pChecking;        //  Pr�fungs-Modul
    Features    * pFeatures;        //  Aggregation der Features
    ErrorReport * pErrorReport;     // Fehler-Report

		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode f�r die Designerunterst�tzung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor ge�ndert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->listBoxMeldungen = (gcnew System::Windows::Forms::ListBox());
      this->buttonAusf�hren = (gcnew System::Windows::Forms::Button());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->checkBoxContourTouchSingle = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxContourTouchMulti = (gcnew System::Windows::Forms::CheckBox());
      this->SuspendLayout();
      // 
      // listBoxMeldungen
      // 
      this->listBoxMeldungen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->listBoxMeldungen->FormattingEnabled = true;
      this->listBoxMeldungen->Location = System::Drawing::Point(1, 144);
      this->listBoxMeldungen->Name = L"listBoxMeldungen";
      this->listBoxMeldungen->Size = System::Drawing::Size(308, 95);
      this->listBoxMeldungen->TabIndex = 0;
      // 
      // buttonAusf�hren
      // 
      this->buttonAusf�hren->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonAusf�hren->Location = System::Drawing::Point(233, 258);
      this->buttonAusf�hren->Name = L"buttonAusf�hren";
      this->buttonAusf�hren->Size = System::Drawing::Size(75, 23);
      this->buttonAusf�hren->TabIndex = 1;
      this->buttonAusf�hren->Text = L"Ausf�hren";
      this->buttonAusf�hren->UseVisualStyleBackColor = true;
      this->buttonAusf�hren->Click += gcnew System::EventHandler(this, &GeometryKorrektur::buttonAusf�hren_Click);
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonBeenden->Location = System::Drawing::Point(1, 258);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 2;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &GeometryKorrektur::buttonBeenden_Click);
      // 
      // checkBoxContourTouchSingle
      // 
      this->checkBoxContourTouchSingle->AutoSize = true;
      this->checkBoxContourTouchSingle->Location = System::Drawing::Point(1, 26);
      this->checkBoxContourTouchSingle->Name = L"checkBoxContourTouchSingle";
      this->checkBoxContourTouchSingle->Size = System::Drawing::Size(253, 17);
      this->checkBoxContourTouchSingle->TabIndex = 3;
      this->checkBoxContourTouchSingle->Text = L"EinfacheEinschn�rung in Polygon-Aussenkontur";
      this->checkBoxContourTouchSingle->UseVisualStyleBackColor = true;
      // 
      // checkBoxContourTouchMulti
      // 
      this->checkBoxContourTouchMulti->AutoSize = true;
      this->checkBoxContourTouchMulti->Location = System::Drawing::Point(1, 49);
      this->checkBoxContourTouchMulti->Name = L"checkBoxContourTouchMulti";
      this->checkBoxContourTouchMulti->Size = System::Drawing::Size(287, 17);
      this->checkBoxContourTouchMulti->TabIndex = 4;
      this->checkBoxContourTouchMulti->Text = L"Mehrfache Einschn�rung von Polygon Aussenkonturen";
      this->checkBoxContourTouchMulti->UseVisualStyleBackColor = true;
      // 
      // GeometryKorrektur
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(309, 300);
      this->Controls->Add(this->checkBoxContourTouchMulti);
      this->Controls->Add(this->checkBoxContourTouchSingle);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->buttonAusf�hren);
      this->Controls->Add(this->listBoxMeldungen);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"GeometryKorrektur";
      this->Text = L"Automatische Korrektur von Geometriefehlern";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Ausf�hren"                                        //
//  Startet die Korrektur                                                    //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAusf�hren_Click(System::Object^  sender, System::EventArgs^  e)
  {
    if ( checkBoxContourTouchSingle->Checked || checkBoxContourTouchMulti->Checked )
      correctContourTouch();
  }

///////////////////////////////////////////////////////////////////////////////
//  Durchf�hrung der Korrekturen                                             //
///////////////////////////////////////////////////////////////////////////////
  private: System::Void correctContourTouch()
  {
    vector<GeometryError*> vErrors;
		size_t                 i, anz;
		size_t                 index1, index2;
    GeometryError        * pError;
    string                 geometryId;
    PolygonGeo              * pPolygon;
    String               ^ STR;
    bool                   success;

    if ( checkBoxContourTouchSingle->Checked || checkBoxContourTouchMulti->Checked )
      pErrorReport->getGeometryErrors( GeometryError::RING_SELF_OVERLAP, vErrors  );

    anz = vErrors.size();
    STR = String::Concat ( "Korrektur von ", Convert::ToString( anz ), " Einschn�rungen von Polygon-Aussenkonturen" );

    listBoxMeldungen->Items->Add( STR );

    for ( i = 0; i < anz; i++ )
    {
      pError = vErrors[i];


      geometryId = pError->getFeatureGeometryId();
      index1 = pError->getSelfOverlapIndex1();
      index2 = pError->getSelfOverlapIndex2();

      STR = String::Concat ( "Polygon mit id; ", QuConvert::ToString ( geometryId ), "( ", Convert::ToString( index1 ), " - ", 
                              Convert::ToString( index2 ), " ): " );

      pPolygon = dynamic_cast<PolygonGeo*>(pFeatures->getGeometry( geometryId ));
      if ( pPolygon == NULL || index1 == index2 )
      {
        STR = String::Concat( STR, "Nicht korrigiert" );
        listBoxMeldungen->Items->Add( STR );
        continue;
      }

      success = pPolygon->changeContourNarrowingIntoVoid ( index1, index2, pChecking->getCheckingParams() );
      if ( success )
        STR = String::Concat( STR, "Korrigiert" );
      else
        STR = String::Concat( STR, "Nicht korrigiert" );

      listBoxMeldungen->Items->Add( STR );
    }

    buttonAusf�hren->Enabled = false;

  }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"                                          //
//  Schlie�t den Dialog                                                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
 {
   Close();
 }
};
