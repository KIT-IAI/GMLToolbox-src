#pragma once

#include "SrsManagerRef.h"
#include "Feature.h"
#include "Geometrie.h"

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Definiert einen 3D Offset zur Verschiebung eines Features
	/// </summary>
	public ref class FeatureOffset : public System::Windows::Forms::Form
	{
	public:
		FeatureOffset( Feature * pFeatureP )
		{
			InitializeComponent();
      pFeature = pFeatureP;
      x = 0.0;
      y = 0.0;
      z = 0.0;

      textBoxXOffset->Text = Convert::ToString( x );
      textBoxYOffset->Text = Convert::ToString( y );
      textBoxZOffset->Text = Convert::ToString( z );		
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~FeatureOffset()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxXOffset;
  protected: 
  private: System::Windows::Forms::TextBox^  textBoxYOffset;
  private: System::Windows::Forms::TextBox^  textBoxZOffset;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::Button^  buttonOk;
  private: System::Windows::Forms::Button^  buttonCancel;

	private:
		/// <summary>
		Feature * pFeature;
    double    x, y, z;
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->textBoxXOffset = (gcnew System::Windows::Forms::TextBox());
      this->textBoxYOffset = (gcnew System::Windows::Forms::TextBox());
      this->textBoxZOffset = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->buttonOk = (gcnew System::Windows::Forms::Button());
      this->buttonCancel = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // textBoxXOffset
      // 
      this->textBoxXOffset->Location = System::Drawing::Point(72, 29);
      this->textBoxXOffset->Name = L"textBoxXOffset";
      this->textBoxXOffset->Size = System::Drawing::Size(122, 20);
      this->textBoxXOffset->TabIndex = 0;
      this->textBoxXOffset->Leave += gcnew System::EventHandler(this, &FeatureOffset::textBoxXOffset_Leave);
      // 
      // textBoxYOffset
      // 
      this->textBoxYOffset->Location = System::Drawing::Point(72, 66);
      this->textBoxYOffset->Name = L"textBoxYOffset";
      this->textBoxYOffset->Size = System::Drawing::Size(122, 20);
      this->textBoxYOffset->TabIndex = 1;
      this->textBoxYOffset->Leave += gcnew System::EventHandler(this, &FeatureOffset::textBoxYOffset_Leave);
      // 
      // textBoxZOffset
      // 
      this->textBoxZOffset->Location = System::Drawing::Point(72, 108);
      this->textBoxZOffset->Name = L"textBoxZOffset";
      this->textBoxZOffset->Size = System::Drawing::Size(122, 20);
      this->textBoxZOffset->TabIndex = 2;
      this->textBoxZOffset->Leave += gcnew System::EventHandler(this, &FeatureOffset::textBoxZOffset_Leave);
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(13, 33);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(45, 13);
      this->label1->TabIndex = 3;
      this->label1->Text = L"X-Offset";
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(16, 70);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(45, 13);
      this->label2->TabIndex = 4;
      this->label2->Text = L"Y-Offset";
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(16, 112);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(45, 13);
      this->label3->TabIndex = 5;
      this->label3->Text = L"Z-Offset";
      // 
      // buttonOk
      // 
      this->buttonOk->Location = System::Drawing::Point(119, 161);
      this->buttonOk->Name = L"buttonOk";
      this->buttonOk->Size = System::Drawing::Size(75, 23);
      this->buttonOk->TabIndex = 6;
      this->buttonOk->Text = L"Ausführen";
      this->buttonOk->UseVisualStyleBackColor = true;
      this->buttonOk->Click += gcnew System::EventHandler(this, &FeatureOffset::buttonOk_Click);
      // 
      // buttonCancel
      // 
      this->buttonCancel->Location = System::Drawing::Point(13, 160);
      this->buttonCancel->Name = L"buttonCancel";
      this->buttonCancel->Size = System::Drawing::Size(75, 23);
      this->buttonCancel->TabIndex = 7;
      this->buttonCancel->Text = L"Abbrechen";
      this->buttonCancel->UseVisualStyleBackColor = true;
      this->buttonCancel->Click += gcnew System::EventHandler(this, &FeatureOffset::buttonCancel_Click);
      // 
      // FeatureOffset
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(206, 205);
      this->Controls->Add(this->buttonCancel);
      this->Controls->Add(this->buttonOk);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->textBoxZOffset);
      this->Controls->Add(this->textBoxYOffset);
      this->Controls->Add(this->textBoxXOffset);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"FeatureOffset";
      this->Text = L"Feature Offset";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Beendigung der Eingabe des x-Wertes                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void textBoxXOffset_Leave(System::Object^  sender, System::EventArgs^  e)
         {
           System::Double result;         

           bool success = System::Double::TryParse( textBoxXOffset->Text, System::Globalization::NumberStyles::Float,
                  System::Globalization::CultureInfo::CurrentCulture, result );
           if ( success )
             x = result;
           else
             textBoxXOffset->Text = "";
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Beendigung der Eingabe des y-Wertes                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void textBoxYOffset_Leave(System::Object^  sender, System::EventArgs^  e)
         {
           System::Double result;         

           bool success = System::Double::TryParse( textBoxYOffset->Text, System::Globalization::NumberStyles::Float,
             System::Globalization::CultureInfo::CurrentCulture, result );
           if ( success )
             y = result;
           else
             textBoxYOffset->Text = "";
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Beendigung der Eingabe des z-Wertes                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void textBoxZOffset_Leave(System::Object^  sender, System::EventArgs^  e) 
         {
           System::Double result;         

           bool success = System::Double::TryParse( textBoxZOffset->Text, System::Globalization::NumberStyles::Float,
             System::Globalization::CultureInfo::CurrentCulture, result );
           if ( success )
             z = result;
           else
             textBoxZOffset->Text = "";
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Button Abbrechen                                         //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonCancel_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           Close();
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Button Ausführen                                         //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonOk_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           if (  textBoxYOffset->Text->CompareTo( "") == 0 ||
                 textBoxXOffset->Text->CompareTo( "") == 0 ||
                 textBoxZOffset->Text->CompareTo( "") == 0  )
           {
             MessageBox::Show ( "Fehlerhafte Eingabe" );
             return;
           }

           GeoPoint              point = GeoPoint ( x, y, z );
           std::set<_Geometrie*> sGeometries;

           pFeature->offset ( point, sGeometries );
           Close();
         }
};
