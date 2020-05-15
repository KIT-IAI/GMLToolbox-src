#pragma once

#include "Checking.h"
#include "Convert.h"

	/// <summary>
	/// Anzeige detaillierter Informationen zu einer Konformit�tsregel
	/// </summary>
	public ref class Konformit�tsregelDetails : public System::Windows::Forms::Form
	{
	public:
		Konformit�tsregelDetails( ProfileRule * pRule )
		{
			InitializeComponent();

      textBoxDetails->Text = QuConvert::ToString( pRule->getDescription() );			
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Konformit�tsregelDetails()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxDetails;
  private: System::Windows::Forms::Button^  buttonBeenden;
  protected: 

  protected: 

	private:
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
      this->textBoxDetails = (gcnew System::Windows::Forms::TextBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // textBoxDetails
      // 
      this->textBoxDetails->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxDetails->Location = System::Drawing::Point(0, 0);
      this->textBoxDetails->Multiline = true;
      this->textBoxDetails->Name = L"textBoxDetails";
      this->textBoxDetails->ReadOnly = true;
      this->textBoxDetails->Size = System::Drawing::Size(288, 209);
      this->textBoxDetails->TabIndex = 0;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
      this->buttonBeenden->Location = System::Drawing::Point(115, 236);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 1;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &Konformit�tsregelDetails::buttonBeenden_Click);
      // 
      // Konformit�tsregelDetails
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(284, 262);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->textBoxDetails);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"Konformit�tsregelDetails";
      this->Text = L"Inhalt der Regel";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"                                          //
//  Schlie�t den Dialog                                                      //
///////////////////////////////////////////////////////////////////////////////
  private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e) 
           {
             Close();
           }
  };
