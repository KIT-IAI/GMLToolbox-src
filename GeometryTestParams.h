#pragma once

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

#include "Checking.h"

	/// <summary>
	/// Festlegung zentraler Parameter für die Geometrieprüfung
	/// </summary>
	public ref class GeometryTestParams : public System::Windows::Forms::Form
	{
	public:
		GeometryTestParams( CheckingParams * pParamsP )
		{
			InitializeComponent();

      pParams = pParamsP;

      textBoxMinimalPointDistance->Text = Convert::ToString( pParams->minimalPointDistance );
      textBoxMinimalAngleDistance->Text = Convert::ToString( pParams->minimalAngleDistance );
      textBoxPlanarityLimit->Text       = Convert::ToString( pParams->planarityLimit );

      checkBoxRemoveDoublePoints->Checked = pParams->eliminateDoublePoints;			
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~GeometryTestParams()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxMinimalPointDistance;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  textBoxMinimalAngleDistance;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::TextBox^  textBoxPlanarityLimit;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::CheckBox^  checkBoxRemoveDoublePoints;
  private: System::Windows::Forms::Button^  buttonOk;
  private: System::Windows::Forms::Button^  buttonAbbrechen;

	private:
		/// <summary>
		CheckingParams * pParams;           // Prüf-Parameter
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->textBoxMinimalPointDistance = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBoxMinimalAngleDistance = (gcnew System::Windows::Forms::TextBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->textBoxPlanarityLimit = (gcnew System::Windows::Forms::TextBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->checkBoxRemoveDoublePoints = (gcnew System::Windows::Forms::CheckBox());
      this->buttonOk = (gcnew System::Windows::Forms::Button());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // textBoxMinimalPointDistance
      // 
      this->textBoxMinimalPointDistance->Location = System::Drawing::Point(13, 13);
      this->textBoxMinimalPointDistance->Name = L"textBoxMinimalPointDistance";
      this->textBoxMinimalPointDistance->Size = System::Drawing::Size(100, 20);
      this->textBoxMinimalPointDistance->TabIndex = 0;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(130, 19);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(120, 13);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Minimaler Punktabstand";
      // 
      // textBoxMinimalAngleDistance
      // 
      this->textBoxMinimalAngleDistance->Location = System::Drawing::Point(13, 40);
      this->textBoxMinimalAngleDistance->Name = L"textBoxMinimalAngleDistance";
      this->textBoxMinimalAngleDistance->Size = System::Drawing::Size(100, 20);
      this->textBoxMinimalAngleDistance->TabIndex = 2;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(130, 43);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(125, 13);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Minimaler Winkelabstand";
      // 
      // textBoxPlanarityLimit
      // 
      this->textBoxPlanarityLimit->Location = System::Drawing::Point(13, 67);
      this->textBoxPlanarityLimit->Name = L"textBoxPlanarityLimit";
      this->textBoxPlanarityLimit->Size = System::Drawing::Size(100, 20);
      this->textBoxPlanarityLimit->TabIndex = 4;
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(130, 73);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(107, 13);
      this->label3->TabIndex = 5;
      this->label3->Text = L"Planaritäts-Parameter";
      // 
      // checkBoxRemoveDoublePoints
      // 
      this->checkBoxRemoveDoublePoints->AutoSize = true;
      this->checkBoxRemoveDoublePoints->Location = System::Drawing::Point(12, 113);
      this->checkBoxRemoveDoublePoints->Name = L"checkBoxRemoveDoublePoints";
      this->checkBoxRemoveDoublePoints->Size = System::Drawing::Size(154, 17);
      this->checkBoxRemoveDoublePoints->TabIndex = 10;
      this->checkBoxRemoveDoublePoints->Text = L"Doppelte Punkte entfernen";
      this->checkBoxRemoveDoublePoints->UseVisualStyleBackColor = true;
      // 
      // buttonOk
      // 
      this->buttonOk->Location = System::Drawing::Point(288, 170);
      this->buttonOk->Name = L"buttonOk";
      this->buttonOk->Size = System::Drawing::Size(84, 23);
      this->buttonOk->TabIndex = 11;
      this->buttonOk->Text = L"Übernehmen";
      this->buttonOk->UseVisualStyleBackColor = true;
      this->buttonOk->Click += gcnew System::EventHandler(this, &GeometryTestParams::buttonOk_Click);
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonAbbrechen->Location = System::Drawing::Point(13, 169);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 23);
      this->buttonAbbrechen->TabIndex = 12;
      this->buttonAbbrechen->Text = L"Abbrechen";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &GeometryTestParams::buttonAbbrechen_Click);
      // 
      // GeometryTestParams
      // 
      this->AcceptButton = this->buttonOk;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->buttonAbbrechen;
      this->ClientSize = System::Drawing::Size(384, 207);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->buttonOk);
      this->Controls->Add(this->checkBoxRemoveDoublePoints);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->textBoxPlanarityLimit);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->textBoxMinimalAngleDistance);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->textBoxMinimalPointDistance);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"GeometryTestParams";
      this->Text = L"GeometryTestParams";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Übernehmer"                                       //
//  Übernimmt die spezifizierten Parameter                                   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonOk_Click(System::Object^  sender, System::EventArgs^  e)
           {
             pParams->minimalPointDistance = Convert::ToDouble ( textBoxMinimalPointDistance->Text );
             pParams->minimalAngleDistance =  Convert::ToDouble ( textBoxMinimalAngleDistance->Text );
             pParams->planarityLimit       =  Convert::ToDouble ( textBoxPlanarityLimit->Text );
             pParams->eliminateDoublePoints = checkBoxRemoveDoublePoints->Checked ;			

             Close();
           }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Abbrechen"                                        //
//  Schließt den Dialog                                                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAbbrechen_Click(System::Object^  sender, System::EventArgs^  e)
         {
           Close();
         }
};
