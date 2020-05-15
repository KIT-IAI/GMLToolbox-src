#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "includes.h"

	/// <summary>
	/// Auswahl einer XPlanGML oder INSPIRE PLU Version
	///
	/// </summary>
	public ref class GmlTypAuswahlDialog : public System::Windows::Forms::Form
	{
	public:
		GmlTypAuswahlDialog( GML_SCHEMA_TYPE glmTyp )
		{
			InitializeComponent();
			//
      typLocal = glmTyp;

      if ( typLocal == XPLANGML_2_0 )
        radioButtonXPlanGML_2_0->Checked = true;
      else
      if ( typLocal == XPLANGML_3_0 )
        radioButtonXPlanGML_3_0->Checked = true;
      else
      if ( typLocal ==  XPLANGML_4_0 )
        radioButtonXPlanGML_4_0->Checked = true;
      else
      if ( typLocal ==  XPLANGML_4_1 )
        radioButtonXPlanGML_4_1->Checked = true;
      else
      if ( typLocal ==  XPLANGML_5_0 )
        radioButtonXPlanGML_5_0->Checked = true;
      else
      if ( typLocal ==  XPLANGML_5_1 )
        radioButtonXPlanGML_5_1->Checked = true;
      else
      if ( typLocal ==  XPLANGML_5_2 )
        radioButtonXPlanGML_5_2->Checked = true;
      else
      if ( typLocal == INSPIRE_LU_PLANNED )
        radioButtonINSPIRE_Planned_LU->Checked = true;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~GmlTypAuswahlDialog()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_2_0;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_3_0;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_4_0;
  private: System::Windows::Forms::RadioButton^  radioButtonINSPIRE_Planned_LU;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_4_1;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_5_0;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_5_1;
   private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_5_2;
  
  public:
    GML_SCHEMA_TYPE typLocal;			// Ausgewählter Schema-Typ

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GmlTypAuswahlDialog::typeid));
      this->radioButtonXPlanGML_2_0 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonXPlanGML_3_0 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonXPlanGML_4_0 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonINSPIRE_Planned_LU = (gcnew System::Windows::Forms::RadioButton());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->radioButtonXPlanGML_4_1 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonXPlanGML_5_0 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonXPlanGML_5_1 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonXPlanGML_5_2 = (gcnew System::Windows::Forms::RadioButton());
      this->SuspendLayout();
      // 
      // radioButtonXPlanGML_2_0
      // 
      this->radioButtonXPlanGML_2_0->AutoSize = true;
      this->radioButtonXPlanGML_2_0->Location = System::Drawing::Point(13, 22);
      this->radioButtonXPlanGML_2_0->Name = L"radioButtonXPlanGML_2_0";
      this->radioButtonXPlanGML_2_0->Size = System::Drawing::Size(94, 17);
      this->radioButtonXPlanGML_2_0->TabIndex = 0;
      this->radioButtonXPlanGML_2_0->TabStop = true;
      this->radioButtonXPlanGML_2_0->Text = L"XPlanGML 2.0";
      this->radioButtonXPlanGML_2_0->UseVisualStyleBackColor = true;
      // 
      // radioButtonXPlanGML_3_0
      // 
      this->radioButtonXPlanGML_3_0->AutoSize = true;
      this->radioButtonXPlanGML_3_0->Location = System::Drawing::Point(13, 46);
      this->radioButtonXPlanGML_3_0->Name = L"radioButtonXPlanGML_3_0";
      this->radioButtonXPlanGML_3_0->Size = System::Drawing::Size(94, 17);
      this->radioButtonXPlanGML_3_0->TabIndex = 1;
      this->radioButtonXPlanGML_3_0->TabStop = true;
      this->radioButtonXPlanGML_3_0->Text = L"XPlanGML 3.0";
      this->radioButtonXPlanGML_3_0->UseVisualStyleBackColor = true;
      // 
      // radioButtonXPlanGML_4_0
      // 
      this->radioButtonXPlanGML_4_0->AutoSize = true;
      this->radioButtonXPlanGML_4_0->Location = System::Drawing::Point(13, 70);
      this->radioButtonXPlanGML_4_0->Name = L"radioButtonXPlanGML_4_0";
      this->radioButtonXPlanGML_4_0->Size = System::Drawing::Size(94, 17);
      this->radioButtonXPlanGML_4_0->TabIndex = 2;
      this->radioButtonXPlanGML_4_0->TabStop = true;
      this->radioButtonXPlanGML_4_0->Text = L"XPlanGML 4.0";
      this->radioButtonXPlanGML_4_0->UseVisualStyleBackColor = true;
      // 
      // radioButtonINSPIRE_Planned_LU
      // 
      this->radioButtonINSPIRE_Planned_LU->AutoSize = true;
      this->radioButtonINSPIRE_Planned_LU->Location = System::Drawing::Point(13, 182);
      this->radioButtonINSPIRE_Planned_LU->Name = L"radioButtonINSPIRE_Planned_LU";
      this->radioButtonINSPIRE_Planned_LU->Size = System::Drawing::Size(159, 17);
      this->radioButtonINSPIRE_Planned_LU->TabIndex = 3;
      this->radioButtonINSPIRE_Planned_LU->TabStop = true;
      this->radioButtonINSPIRE_Planned_LU->Text = L"INSPIRE Planned Land Use";
      this->radioButtonINSPIRE_Planned_LU->UseVisualStyleBackColor = true;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonBeenden->Location = System::Drawing::Point(109, 221);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 4;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &GmlTypAuswahlDialog::buttonBeenden_Click);
      // 
      // radioButtonXPlanGML_4_1
      // 
      this->radioButtonXPlanGML_4_1->AutoSize = true;
      this->radioButtonXPlanGML_4_1->Location = System::Drawing::Point(13, 93);
      this->radioButtonXPlanGML_4_1->Name = L"radioButtonXPlanGML_4_1";
      this->radioButtonXPlanGML_4_1->Size = System::Drawing::Size(94, 17);
      this->radioButtonXPlanGML_4_1->TabIndex = 5;
      this->radioButtonXPlanGML_4_1->TabStop = true;
      this->radioButtonXPlanGML_4_1->Text = L"XPlanGML 4.1";
      this->radioButtonXPlanGML_4_1->UseVisualStyleBackColor = true;
      // 
      // radioButtonXPlanGML_5_0
      // 
      this->radioButtonXPlanGML_5_0->AutoSize = true;
      this->radioButtonXPlanGML_5_0->Location = System::Drawing::Point(13, 117);
      this->radioButtonXPlanGML_5_0->Name = L"radioButtonXPlanGML_5_0";
      this->radioButtonXPlanGML_5_0->Size = System::Drawing::Size(94, 17);
      this->radioButtonXPlanGML_5_0->TabIndex = 6;
      this->radioButtonXPlanGML_5_0->TabStop = true;
      this->radioButtonXPlanGML_5_0->Text = L"XPlanGML 5.0";
      this->radioButtonXPlanGML_5_0->UseVisualStyleBackColor = true;
      // 
      // radioButtonXPlanGML_5_1
      // 
      this->radioButtonXPlanGML_5_1->AutoSize = true;
      this->radioButtonXPlanGML_5_1->Location = System::Drawing::Point(13, 140);
      this->radioButtonXPlanGML_5_1->Name = L"radioButtonXPlanGML_5_1";
      this->radioButtonXPlanGML_5_1->Size = System::Drawing::Size(94, 17);
      this->radioButtonXPlanGML_5_1->TabIndex = 7;
      this->radioButtonXPlanGML_5_1->TabStop = true;
      this->radioButtonXPlanGML_5_1->Text = L"XPlanGML 5.1";
      this->radioButtonXPlanGML_5_1->UseVisualStyleBackColor = true;
      // 
      // radioButtonXPlanGML_5_2
      // 
      this->radioButtonXPlanGML_5_2->AutoSize = true;
      this->radioButtonXPlanGML_5_2->Location = System::Drawing::Point(13, 161);
      this->radioButtonXPlanGML_5_2->Name = L"radioButtonXPlanGML_5_2";
      this->radioButtonXPlanGML_5_2->Size = System::Drawing::Size(94, 17);
      this->radioButtonXPlanGML_5_2->TabIndex = 8;
      this->radioButtonXPlanGML_5_2->TabStop = true;
      this->radioButtonXPlanGML_5_2->Text = L"XPlanGML 5.2";
      this->radioButtonXPlanGML_5_2->UseVisualStyleBackColor = true;
      // 
      // GmlTypAuswahlDialog
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(292, 262);
      this->Controls->Add(this->radioButtonXPlanGML_5_2);
      this->Controls->Add(this->radioButtonXPlanGML_5_1);
      this->Controls->Add(this->radioButtonXPlanGML_5_0);
      this->Controls->Add(this->radioButtonXPlanGML_4_1);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->radioButtonINSPIRE_Planned_LU);
      this->Controls->Add(this->radioButtonXPlanGML_4_0);
      this->Controls->Add(this->radioButtonXPlanGML_3_0);
      this->Controls->Add(this->radioButtonXPlanGML_2_0);
      this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
      this->Name = L"GmlTypAuswahlDialog";
      this->Text = L"Auswahl GML-Typ";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
 
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"																					 //
//  Übernimmt die Auswahl in das Attribut "typLocal"												 //
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Änderung CheckBox "Pläne aus Shape erzeugen"							 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
{
  if ( radioButtonXPlanGML_2_0->Checked )
    typLocal = XPLANGML_2_0;
  else
  if ( radioButtonXPlanGML_3_0->Checked )
    typLocal = XPLANGML_3_0;
  else
  if ( radioButtonXPlanGML_4_0->Checked )
    typLocal = XPLANGML_4_0;
  else
  if ( radioButtonXPlanGML_4_1->Checked )
    typLocal = XPLANGML_4_1;
  else
  if ( radioButtonXPlanGML_5_0->Checked )
    typLocal = XPLANGML_5_0;
  else
  if ( radioButtonXPlanGML_5_1->Checked )
    typLocal = XPLANGML_5_1;
  else
  if ( radioButtonXPlanGML_5_2->Checked )
    typLocal = XPLANGML_5_2;
  else
  if ( radioButtonINSPIRE_Planned_LU->Checked )
    typLocal = INSPIRE_LU_PLANNED;

}

};
