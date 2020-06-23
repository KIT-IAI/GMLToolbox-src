#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "includes.h"

	/// <summary>
	/// Festlegung von XPlanGML-Version und Plan-Typ für eine neue Shapefile --> XPlanGML Konversion
	///
	/// </summary>
	public ref class KonvDateiNeu : public System::Windows::Forms::Form
	{
	public:
		KonvDateiNeu(void)
		{
			InitializeComponent();

      gmlTyp = XPLANGML_5_2;
      planTyp = BPLAN;

      radioButtonXPlanGML_5_2->Checked = true;
      radioButtonBPlan->Checked = true;			
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~KonvDateiNeu()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::GroupBox^  groupBoxGmlVersion;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_4_0;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_3_0;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_2_0;
  private: System::Windows::Forms::GroupBox^  groupBoxPlanArt;
  private: System::Windows::Forms::RadioButton^  radioButtonSoPlan;
  private: System::Windows::Forms::RadioButton^  radioButtonLPlan;
  private: System::Windows::Forms::RadioButton^  radioButtonRPlan;
  private: System::Windows::Forms::RadioButton^  radioButtonFPlan;
  private: System::Windows::Forms::RadioButton^  radioButtonBPlan;
  private: System::Windows::Forms::Button^  buttonOk;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_4_1;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_5_0;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_5_1;
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_5_2;

  public: 
    GML_SCHEMA_TYPE gmlTyp;			// Ausgewählte XPlanGML-Version
    XPLAN_TYP       planTyp;		// Ausgewählter Plan-Typ
  private: System::Windows::Forms::RadioButton^  radioButtonXPlanGML_5_3;
  public:

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
      this->groupBoxGmlVersion = ( gcnew System::Windows::Forms::GroupBox() );
      this->radioButtonXPlanGML_5_2 = ( gcnew System::Windows::Forms::RadioButton() );
      this->radioButtonXPlanGML_5_1 = ( gcnew System::Windows::Forms::RadioButton() );
      this->radioButtonXPlanGML_5_0 = ( gcnew System::Windows::Forms::RadioButton() );
      this->radioButtonXPlanGML_4_1 = ( gcnew System::Windows::Forms::RadioButton() );
      this->radioButtonXPlanGML_4_0 = ( gcnew System::Windows::Forms::RadioButton() );
      this->radioButtonXPlanGML_3_0 = ( gcnew System::Windows::Forms::RadioButton() );
      this->radioButtonXPlanGML_2_0 = ( gcnew System::Windows::Forms::RadioButton() );
      this->groupBoxPlanArt = ( gcnew System::Windows::Forms::GroupBox() );
      this->radioButtonSoPlan = ( gcnew System::Windows::Forms::RadioButton() );
      this->radioButtonLPlan = ( gcnew System::Windows::Forms::RadioButton() );
      this->radioButtonRPlan = ( gcnew System::Windows::Forms::RadioButton() );
      this->radioButtonFPlan = ( gcnew System::Windows::Forms::RadioButton() );
      this->radioButtonBPlan = ( gcnew System::Windows::Forms::RadioButton() );
      this->buttonOk = ( gcnew System::Windows::Forms::Button() );
      this->radioButtonXPlanGML_5_3 = ( gcnew System::Windows::Forms::RadioButton() );
      this->groupBoxGmlVersion->SuspendLayout();
      this->groupBoxPlanArt->SuspendLayout();
      this->SuspendLayout();
      // 
      // groupBoxGmlVersion
      // 
      this->groupBoxGmlVersion->Controls->Add( this->radioButtonXPlanGML_5_3 );
      this->groupBoxGmlVersion->Controls->Add( this->radioButtonXPlanGML_5_2 );
      this->groupBoxGmlVersion->Controls->Add( this->radioButtonXPlanGML_5_1 );
      this->groupBoxGmlVersion->Controls->Add( this->radioButtonXPlanGML_5_0 );
      this->groupBoxGmlVersion->Controls->Add( this->radioButtonXPlanGML_4_1 );
      this->groupBoxGmlVersion->Controls->Add( this->radioButtonXPlanGML_4_0 );
      this->groupBoxGmlVersion->Controls->Add( this->radioButtonXPlanGML_3_0 );
      this->groupBoxGmlVersion->Controls->Add( this->radioButtonXPlanGML_2_0 );
      this->groupBoxGmlVersion->Location = System::Drawing::Point( 12, 12 );
      this->groupBoxGmlVersion->Name = L"groupBoxGmlVersion";
      this->groupBoxGmlVersion->Size = System::Drawing::Size( 221, 217 );
      this->groupBoxGmlVersion->TabIndex = 0;
      this->groupBoxGmlVersion->TabStop = false;
      this->groupBoxGmlVersion->Text = L"GML-Version";
      // 
      // radioButtonXPlanGML_5_2
      // 
      this->radioButtonXPlanGML_5_2->AutoSize = true;
      this->radioButtonXPlanGML_5_2->Location = System::Drawing::Point( 7, 164 );
      this->radioButtonXPlanGML_5_2->Name = L"radioButtonXPlanGML_5_2";
      this->radioButtonXPlanGML_5_2->Size = System::Drawing::Size( 94, 17 );
      this->radioButtonXPlanGML_5_2->TabIndex = 6;
      this->radioButtonXPlanGML_5_2->TabStop = true;
      this->radioButtonXPlanGML_5_2->Text = L"XPlanGML 5.2";
      this->radioButtonXPlanGML_5_2->UseVisualStyleBackColor = true;
      this->radioButtonXPlanGML_5_2->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonXPlanGML_5_2_CheckedChanged );
      // 
      // radioButtonXPlanGML_5_1
      // 
      this->radioButtonXPlanGML_5_1->AutoSize = true;
      this->radioButtonXPlanGML_5_1->Location = System::Drawing::Point( 7, 140 );
      this->radioButtonXPlanGML_5_1->Name = L"radioButtonXPlanGML_5_1";
      this->radioButtonXPlanGML_5_1->Size = System::Drawing::Size( 94, 17 );
      this->radioButtonXPlanGML_5_1->TabIndex = 5;
      this->radioButtonXPlanGML_5_1->TabStop = true;
      this->radioButtonXPlanGML_5_1->Text = L"XPlanGML 5.1";
      this->radioButtonXPlanGML_5_1->UseVisualStyleBackColor = true;
      this->radioButtonXPlanGML_5_1->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonXPlanGML_5_1_CheckedChanged );
      // 
      // radioButtonXPlanGML_5_0
      // 
      this->radioButtonXPlanGML_5_0->AutoSize = true;
      this->radioButtonXPlanGML_5_0->Location = System::Drawing::Point( 7, 116 );
      this->radioButtonXPlanGML_5_0->Name = L"radioButtonXPlanGML_5_0";
      this->radioButtonXPlanGML_5_0->Size = System::Drawing::Size( 94, 17 );
      this->radioButtonXPlanGML_5_0->TabIndex = 4;
      this->radioButtonXPlanGML_5_0->TabStop = true;
      this->radioButtonXPlanGML_5_0->Text = L"XPlanGML 5.0";
      this->radioButtonXPlanGML_5_0->UseVisualStyleBackColor = true;
      this->radioButtonXPlanGML_5_0->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonXPlanGML_5_0_CheckedChanged );
      // 
      // radioButtonXPlanGML_4_1
      // 
      this->radioButtonXPlanGML_4_1->AutoSize = true;
      this->radioButtonXPlanGML_4_1->Location = System::Drawing::Point( 7, 92 );
      this->radioButtonXPlanGML_4_1->Name = L"radioButtonXPlanGML_4_1";
      this->radioButtonXPlanGML_4_1->Size = System::Drawing::Size( 94, 17 );
      this->radioButtonXPlanGML_4_1->TabIndex = 3;
      this->radioButtonXPlanGML_4_1->TabStop = true;
      this->radioButtonXPlanGML_4_1->Text = L"XPlanGML 4.1";
      this->radioButtonXPlanGML_4_1->UseVisualStyleBackColor = true;
      this->radioButtonXPlanGML_4_1->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonXPlanGML_4_1_CheckedChanged );
      // 
      // radioButtonXPlanGML_4_0
      // 
      this->radioButtonXPlanGML_4_0->AutoSize = true;
      this->radioButtonXPlanGML_4_0->Location = System::Drawing::Point( 7, 68 );
      this->radioButtonXPlanGML_4_0->Name = L"radioButtonXPlanGML_4_0";
      this->radioButtonXPlanGML_4_0->Size = System::Drawing::Size( 94, 17 );
      this->radioButtonXPlanGML_4_0->TabIndex = 2;
      this->radioButtonXPlanGML_4_0->TabStop = true;
      this->radioButtonXPlanGML_4_0->Text = L"XPlanGML 4.0";
      this->radioButtonXPlanGML_4_0->UseVisualStyleBackColor = true;
      this->radioButtonXPlanGML_4_0->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonXPlanGML_4_0_CheckedChanged );
      // 
      // radioButtonXPlanGML_3_0
      // 
      this->radioButtonXPlanGML_3_0->AutoSize = true;
      this->radioButtonXPlanGML_3_0->Location = System::Drawing::Point( 7, 44 );
      this->radioButtonXPlanGML_3_0->Name = L"radioButtonXPlanGML_3_0";
      this->radioButtonXPlanGML_3_0->Size = System::Drawing::Size( 94, 17 );
      this->radioButtonXPlanGML_3_0->TabIndex = 1;
      this->radioButtonXPlanGML_3_0->TabStop = true;
      this->radioButtonXPlanGML_3_0->Text = L"XPlanGML 3.0";
      this->radioButtonXPlanGML_3_0->UseVisualStyleBackColor = true;
      this->radioButtonXPlanGML_3_0->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonXPlanGML_3_0_CheckedChanged );
      // 
      // radioButtonXPlanGML_2_0
      // 
      this->radioButtonXPlanGML_2_0->AutoSize = true;
      this->radioButtonXPlanGML_2_0->Location = System::Drawing::Point( 7, 20 );
      this->radioButtonXPlanGML_2_0->Name = L"radioButtonXPlanGML_2_0";
      this->radioButtonXPlanGML_2_0->Size = System::Drawing::Size( 94, 17 );
      this->radioButtonXPlanGML_2_0->TabIndex = 0;
      this->radioButtonXPlanGML_2_0->TabStop = true;
      this->radioButtonXPlanGML_2_0->Text = L"XPlanGML 2.0";
      this->radioButtonXPlanGML_2_0->UseVisualStyleBackColor = true;
      this->radioButtonXPlanGML_2_0->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonXPlanGML_2_0_CheckedChanged );
      // 
      // groupBoxPlanArt
      // 
      this->groupBoxPlanArt->Controls->Add( this->radioButtonSoPlan );
      this->groupBoxPlanArt->Controls->Add( this->radioButtonLPlan );
      this->groupBoxPlanArt->Controls->Add( this->radioButtonRPlan );
      this->groupBoxPlanArt->Controls->Add( this->radioButtonFPlan );
      this->groupBoxPlanArt->Controls->Add( this->radioButtonBPlan );
      this->groupBoxPlanArt->Location = System::Drawing::Point( 12, 235 );
      this->groupBoxPlanArt->Name = L"groupBoxPlanArt";
      this->groupBoxPlanArt->Size = System::Drawing::Size( 221, 147 );
      this->groupBoxPlanArt->TabIndex = 1;
      this->groupBoxPlanArt->TabStop = false;
      this->groupBoxPlanArt->Text = L"Plan-Art";
      // 
      // radioButtonSoPlan
      // 
      this->radioButtonSoPlan->AutoSize = true;
      this->radioButtonSoPlan->Location = System::Drawing::Point( 7, 118 );
      this->radioButtonSoPlan->Name = L"radioButtonSoPlan";
      this->radioButtonSoPlan->Size = System::Drawing::Size( 169, 17 );
      this->radioButtonSoPlan->TabIndex = 4;
      this->radioButtonSoPlan->TabStop = true;
      this->radioButtonSoPlan->Text = L"Sonstiger raumbezogener Plan";
      this->radioButtonSoPlan->UseVisualStyleBackColor = true;
      this->radioButtonSoPlan->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonSoPlan_CheckedChanged );
      // 
      // radioButtonLPlan
      // 
      this->radioButtonLPlan->AutoSize = true;
      this->radioButtonLPlan->Location = System::Drawing::Point( 7, 92 );
      this->radioButtonLPlan->Name = L"radioButtonLPlan";
      this->radioButtonLPlan->Size = System::Drawing::Size( 103, 17 );
      this->radioButtonLPlan->TabIndex = 3;
      this->radioButtonLPlan->TabStop = true;
      this->radioButtonLPlan->Text = L"Landschaftsplan";
      this->radioButtonLPlan->UseVisualStyleBackColor = true;
      this->radioButtonLPlan->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonLPlan_CheckedChanged );
      // 
      // radioButtonRPlan
      // 
      this->radioButtonRPlan->AutoSize = true;
      this->radioButtonRPlan->Location = System::Drawing::Point( 7, 68 );
      this->radioButtonRPlan->Name = L"radioButtonRPlan";
      this->radioButtonRPlan->Size = System::Drawing::Size( 87, 17 );
      this->radioButtonRPlan->TabIndex = 2;
      this->radioButtonRPlan->TabStop = true;
      this->radioButtonRPlan->Text = L"Regionalplan";
      this->radioButtonRPlan->UseVisualStyleBackColor = true;
      this->radioButtonRPlan->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonRPlan_CheckedChanged );
      // 
      // radioButtonFPlan
      // 
      this->radioButtonFPlan->AutoSize = true;
      this->radioButtonFPlan->Location = System::Drawing::Point( 7, 44 );
      this->radioButtonFPlan->Name = L"radioButtonFPlan";
      this->radioButtonFPlan->Size = System::Drawing::Size( 126, 17 );
      this->radioButtonFPlan->TabIndex = 1;
      this->radioButtonFPlan->TabStop = true;
      this->radioButtonFPlan->Text = L"Flächennutzungsplan";
      this->radioButtonFPlan->UseVisualStyleBackColor = true;
      this->radioButtonFPlan->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonFPlan_CheckedChanged );
      // 
      // radioButtonBPlan
      // 
      this->radioButtonBPlan->AutoSize = true;
      this->radioButtonBPlan->Location = System::Drawing::Point( 7, 20 );
      this->radioButtonBPlan->Name = L"radioButtonBPlan";
      this->radioButtonBPlan->Size = System::Drawing::Size( 99, 17 );
      this->radioButtonBPlan->TabIndex = 0;
      this->radioButtonBPlan->TabStop = true;
      this->radioButtonBPlan->Text = L"Bebauungsplan";
      this->radioButtonBPlan->UseVisualStyleBackColor = true;
      this->radioButtonBPlan->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonBPlan_CheckedChanged );
      // 
      // buttonOk
      // 
      this->buttonOk->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonOk->Location = System::Drawing::Point( 80, 400 );
      this->buttonOk->Name = L"buttonOk";
      this->buttonOk->Size = System::Drawing::Size( 75, 23 );
      this->buttonOk->TabIndex = 2;
      this->buttonOk->Text = L"Ok";
      this->buttonOk->UseVisualStyleBackColor = true;
      this->buttonOk->Click += gcnew System::EventHandler( this, &KonvDateiNeu::buttonOk_Click );
      // 
      // radioButtonXPlanGML_5_3
      // 
      this->radioButtonXPlanGML_5_3->AutoSize = true;
      this->radioButtonXPlanGML_5_3->Location = System::Drawing::Point( 7, 188 );
      this->radioButtonXPlanGML_5_3->Name = L"radioButtonXPlanGML_5_3";
      this->radioButtonXPlanGML_5_3->Size = System::Drawing::Size( 94, 17 );
      this->radioButtonXPlanGML_5_3->TabIndex = 7;
      this->radioButtonXPlanGML_5_3->TabStop = true;
      this->radioButtonXPlanGML_5_3->Text = L"XPlanGML 5.3";
      this->radioButtonXPlanGML_5_3->UseVisualStyleBackColor = true;
      this->radioButtonXPlanGML_5_3->CheckedChanged += gcnew System::EventHandler( this, &KonvDateiNeu::radioButtonXPlanGML_5_3_CheckedChanged );
      // 
      // KonvDateiNeu
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF( 6, 13 );
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size( 264, 458 );
      this->ControlBox = false;
      this->Controls->Add( this->buttonOk );
      this->Controls->Add( this->groupBoxPlanArt );
      this->Controls->Add( this->groupBoxGmlVersion );
      this->Name = L"KonvDateiNeu";
      this->Text = L"Generierung einer neuen Konversion";
      this->groupBoxGmlVersion->ResumeLayout( false );
      this->groupBoxGmlVersion->PerformLayout();
      this->groupBoxPlanArt->ResumeLayout( false );
      this->groupBoxPlanArt->PerformLayout();
      this->ResumeLayout( false );

    }
#pragma endregion

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton XPlanGML 2.0												 //
///////////////////////////////////////////////////////////////////////////////
 private: System::Void radioButtonXPlanGML_2_0_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  if ( radioButtonXPlanGML_2_0->Checked ) gmlTyp = XPLANGML_2_0;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton XPlanGML 3.0												 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonXPlanGML_3_0_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  if ( radioButtonXPlanGML_3_0->Checked ) gmlTyp = XPLANGML_3_0;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton XPlanGML 4.0												 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonXPlanGML_4_0_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( radioButtonXPlanGML_4_0->Checked ) gmlTyp = XPLANGML_4_0;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton XPlanGML 4.1												 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonXPlanGML_4_1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( radioButtonXPlanGML_4_1->Checked ) gmlTyp = XPLANGML_4_1;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton XPlanGML 5.0												 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonXPlanGML_5_0_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  if ( radioButtonXPlanGML_5_0->Checked ) gmlTyp = XPLANGML_5_0;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton Bebauungsplan											 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonBPlan_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( radioButtonBPlan->Checked ) planTyp = BPLAN;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton Flächennutzungsplan								 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonFPlan_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( radioButtonFPlan->Checked ) planTyp = FPLAN;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton Regionalplan	  										 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonRPlan_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( radioButtonRPlan->Checked ) planTyp = RPLAN;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton Landschaftsplan										 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonLPlan_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( radioButtonLPlan->Checked ) planTyp = LPLAN;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton Sonstiger raumbezogener Plan  			 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonSoPlan_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( radioButtonSoPlan->Checked ) planTyp = SOPLAN;
}

private: System::Void buttonOk_Click(System::Object^  sender, System::EventArgs^  e) 
{
  
}
				 
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton XPlanGML 5.1												 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonXPlanGML_5_1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( radioButtonXPlanGML_5_1->Checked ) gmlTyp = XPLANGML_5_1;

}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton XPlanGML 5.2												 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonXPlanGML_5_2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
  if ( radioButtonXPlanGML_5_2->Checked ) gmlTyp = XPLANGML_5_2;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Änderung RadioButton XPlanGML 5.3												 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void radioButtonXPlanGML_5_3_CheckedChanged( System::Object^  sender, System::EventArgs^  e )
{
  if ( radioButtonXPlanGML_5_3->Checked ) gmlTyp = XPLANGML_5_3;
}
};
