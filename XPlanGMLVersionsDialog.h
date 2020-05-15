#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "includes.h"

	/// <summary>
	/// Auswahl einer XPlanGML-Version
	///
	/// </summary>
	public ref class XPlanGMLVersionsDialog : public System::Windows::Forms::Form
	{
	public:
		XPlanGMLVersionsDialog( GML_SCHEMA_TYPE typ )
		{
			InitializeComponent();

      gmlTyp = typ;

      switch ( gmlTyp )
      {
      case XPLANGML_2_0:
        radioButtonV2->Checked = true;
        break;

      case XPLANGML_3_0:
        radioButtonV3->Checked = true;
        break;

      case XPLANGML_4_0:
        radioButtonV4->Checked = true;
        break;

      case XPLANGML_4_1:
        radioButtonV4_1->Checked = true;
        break;

      case XPLANGML_5_0:
        radioButtonV5_0->Checked = true;
        break;

      case XPLANGML_5_1:
        radioButtonV5_1->Checked = true;
        break;

      case XPLANGML_5_2:
        radioButtonV5_2->Checked = true;
        break;

      default:
        radioButtonV5_0->Checked = true;
        break;
      }			
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~XPlanGMLVersionsDialog()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::RadioButton^  radioButtonV2;
  private: System::Windows::Forms::RadioButton^  radioButtonV3;
  private: System::Windows::Forms::RadioButton^  radioButtonV4;
  private: System::Windows::Forms::Button^  buttonUebernehmen;
  private: System::Windows::Forms::RadioButton^  radioButtonV4_1;
  private: System::Windows::Forms::RadioButton^  radioButtonV5_0;
  private: System::Windows::Forms::RadioButton^  radioButtonV5_1;
  private: System::Windows::Forms::RadioButton^  radioButtonV5_2;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

  public:
    GML_SCHEMA_TYPE gmlTyp;			//  XPlanGML Version

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->radioButtonV2 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonV3 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonV4 = (gcnew System::Windows::Forms::RadioButton());
      this->buttonUebernehmen = (gcnew System::Windows::Forms::Button());
      this->radioButtonV4_1 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonV5_0 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonV5_1 = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonV5_2 = (gcnew System::Windows::Forms::RadioButton());
      this->SuspendLayout();
      // 
      // radioButtonV2
      // 
      this->radioButtonV2->AutoSize = true;
      this->radioButtonV2->Location = System::Drawing::Point(22, 13);
      this->radioButtonV2->Name = L"radioButtonV2";
      this->radioButtonV2->Size = System::Drawing::Size(94, 17);
      this->radioButtonV2->TabIndex = 0;
      this->radioButtonV2->TabStop = true;
      this->radioButtonV2->Text = L"XPlanGML 2.0";
      this->radioButtonV2->UseVisualStyleBackColor = true;
      // 
      // radioButtonV3
      // 
      this->radioButtonV3->AutoSize = true;
      this->radioButtonV3->Location = System::Drawing::Point(22, 37);
      this->radioButtonV3->Name = L"radioButtonV3";
      this->radioButtonV3->Size = System::Drawing::Size(94, 17);
      this->radioButtonV3->TabIndex = 1;
      this->radioButtonV3->TabStop = true;
      this->radioButtonV3->Text = L"XPlanGML 3.0";
      this->radioButtonV3->UseVisualStyleBackColor = true;
      // 
      // radioButtonV4
      // 
      this->radioButtonV4->AutoSize = true;
      this->radioButtonV4->Location = System::Drawing::Point(22, 61);
      this->radioButtonV4->Name = L"radioButtonV4";
      this->radioButtonV4->Size = System::Drawing::Size(94, 17);
      this->radioButtonV4->TabIndex = 2;
      this->radioButtonV4->TabStop = true;
      this->radioButtonV4->Text = L"XPlanGML 4.0";
      this->radioButtonV4->UseVisualStyleBackColor = true;
      // 
      // buttonUebernehmen
      // 
      this->buttonUebernehmen->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonUebernehmen->Location = System::Drawing::Point(22, 219);
      this->buttonUebernehmen->Name = L"buttonUebernehmen";
      this->buttonUebernehmen->Size = System::Drawing::Size(81, 23);
      this->buttonUebernehmen->TabIndex = 3;
      this->buttonUebernehmen->Text = L"Übernehmen";
      this->buttonUebernehmen->UseVisualStyleBackColor = true;
      this->buttonUebernehmen->Click += gcnew System::EventHandler(this, &XPlanGMLVersionsDialog::buttonUebernehmen_Click);
      // 
      // radioButtonV4_1
      // 
      this->radioButtonV4_1->AutoSize = true;
      this->radioButtonV4_1->Location = System::Drawing::Point(22, 85);
      this->radioButtonV4_1->Name = L"radioButtonV4_1";
      this->radioButtonV4_1->Size = System::Drawing::Size(94, 17);
      this->radioButtonV4_1->TabIndex = 4;
      this->radioButtonV4_1->TabStop = true;
      this->radioButtonV4_1->Text = L"XPlanGML 4.1";
      this->radioButtonV4_1->UseVisualStyleBackColor = true;
      // 
      // radioButtonV5_0
      // 
      this->radioButtonV5_0->AutoSize = true;
      this->radioButtonV5_0->Location = System::Drawing::Point(22, 109);
      this->radioButtonV5_0->Name = L"radioButtonV5_0";
      this->radioButtonV5_0->Size = System::Drawing::Size(94, 17);
      this->radioButtonV5_0->TabIndex = 5;
      this->radioButtonV5_0->TabStop = true;
      this->radioButtonV5_0->Text = L"XPlanGML 5.0";
      this->radioButtonV5_0->UseVisualStyleBackColor = true;
      // 
      // radioButtonV5_1
      // 
      this->radioButtonV5_1->AutoSize = true;
      this->radioButtonV5_1->Location = System::Drawing::Point(22, 133);
      this->radioButtonV5_1->Name = L"radioButtonV5_1";
      this->radioButtonV5_1->Size = System::Drawing::Size(94, 17);
      this->radioButtonV5_1->TabIndex = 6;
      this->radioButtonV5_1->TabStop = true;
      this->radioButtonV5_1->Text = L"XPlanGML 5.1";
      this->radioButtonV5_1->UseVisualStyleBackColor = true;
      // 
      // radioButtonV5_2
      // 
      this->radioButtonV5_2->AutoSize = true;
      this->radioButtonV5_2->Location = System::Drawing::Point(22, 157);
      this->radioButtonV5_2->Name = L"radioButtonV5_2";
      this->radioButtonV5_2->Size = System::Drawing::Size(94, 17);
      this->radioButtonV5_2->TabIndex = 7;
      this->radioButtonV5_2->TabStop = true;
      this->radioButtonV5_2->Text = L"XPlanGML 5.2";
      this->radioButtonV5_2->UseVisualStyleBackColor = true;
      // 
      // XPlanGMLVersionsDialog
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(138, 254);
      this->ControlBox = false;
      this->Controls->Add(this->radioButtonV5_2);
      this->Controls->Add(this->radioButtonV5_1);
      this->Controls->Add(this->radioButtonV5_0);
      this->Controls->Add(this->radioButtonV4_1);
      this->Controls->Add(this->buttonUebernehmen);
      this->Controls->Add(this->radioButtonV4);
      this->Controls->Add(this->radioButtonV3);
      this->Controls->Add(this->radioButtonV2);
      this->Name = L"XPlanGMLVersionsDialog";
      this->Text = L"XPlanGML Version";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Übernehmen"																			 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonUebernehmen_Click(System::Object^  sender, System::EventArgs^  e)
{
  if ( radioButtonV2->Checked ) gmlTyp = XPLANGML_2_0;
  else
  if ( radioButtonV3->Checked ) gmlTyp = XPLANGML_3_0;
  else
  if ( radioButtonV4->Checked ) gmlTyp = XPLANGML_4_0;
  else
  if ( radioButtonV4_1->Checked ) gmlTyp = XPLANGML_4_1;
  else
  if ( radioButtonV5_0->Checked ) gmlTyp = XPLANGML_5_0;
  else
  if ( radioButtonV5_1->Checked ) gmlTyp = XPLANGML_5_1;
  else
  if ( radioButtonV5_2->Checked ) gmlTyp = XPLANGML_5_2;
}

};
