#pragma once


#include "Checking.h"
#include "Convert.h"

	/// <summary>
	/// Anzeige von Fehlern der XML-Schema und Schematron Validierung, sowie von
  /// Fehlern, die während des Einlesens einer GML-Datei aufgetreten sind
	/// </summary>
	public ref class ValidierungsfehlerDialog : public System::Windows::Forms::Form
	{
	public:
		ValidierungsfehlerDialog( ErrorReport * pErrorReportP, String ^ gmlDateiNameP  )
		{
			InitializeComponent();

      pErrorReport = pErrorReportP;
      gmlDateiName = gmlDateiNameP;

      initialize();
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~ValidierungsfehlerDialog()
		{
			if (components)
			{
				delete components;
			}
		}

  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::TextBox^  textBoxValidierungsfehler;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

    ErrorReport * pErrorReport;     //  Fehler-Report
    String      ^ gmlDateiName;     //  Pfadname der zu validierenden XML-Datei

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->textBoxValidierungsfehler = (gcnew System::Windows::Forms::TextBox());
      this->SuspendLayout();
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Location = System::Drawing::Point(203, 360);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 1;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &ValidierungsfehlerDialog::buttonBeenden_Click);
      // 
      // textBoxValidierungsfehler
      // 
      this->textBoxValidierungsfehler->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxValidierungsfehler->Location = System::Drawing::Point(0, 0);
      this->textBoxValidierungsfehler->Multiline = true;
      this->textBoxValidierungsfehler->Name = L"textBoxValidierungsfehler";
      this->textBoxValidierungsfehler->ReadOnly = true;
      this->textBoxValidierungsfehler->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
      this->textBoxValidierungsfehler->Size = System::Drawing::Size(480, 327);
      this->textBoxValidierungsfehler->TabIndex = 2;
      // 
      // ValidierungsfehlerDialog
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(480, 395);
      this->Controls->Add(this->textBoxValidierungsfehler);
      this->Controls->Add(this->buttonBeenden);
      this->Name = L"ValidierungsfehlerDialog";
      this->Text = L"Validierungsfehler";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
 
///////////////////////////////////////////////////////////////////////////////
//  Anzeige aller Fehler                                                     //
///////////////////////////////////////////////////////////////////////////////
    System::Void initialize()
    {
      unsigned int i;
      String     ^ STR;

      if ( pErrorReport->getReadErrorAnz() + pErrorReport->getValidationErrorAnz() > 0 )
      {
        STR = String::Concat ( "Geprüfte Datei: ", gmlDateiName, Environment::NewLine );
        textBoxValidierungsfehler->AppendText( STR );
        textBoxValidierungsfehler->AppendText ( Environment::NewLine );
      }

      for ( i = 0; i < pErrorReport->getReadErrorAnz(); i++ )
      {
        ReadError * pReadError = pErrorReport->getReadError( i );
        int    lineNumber      = pReadError->getLineNumber();
        string errorText       = pReadError->getErrorText();

        STR = String::Concat( "Fehler in Zeile ", Convert::ToString( lineNumber ), Environment::NewLine );
        textBoxValidierungsfehler->AppendText( STR );

        textBoxValidierungsfehler->AppendText( QuConvert::ToString( errorText ) );
        textBoxValidierungsfehler->AppendText ( Environment::NewLine );
        textBoxValidierungsfehler->AppendText ( Environment::NewLine );
      }

      for ( i = 0; i < pErrorReport->getValidationErrorAnz(); i++ )
      {
        ValidationError * pValidationError = pErrorReport->getValidationError( i );
        int    lineNumber      = pValidationError->getLineNumber ();
        string errorText       = pValidationError->getErrorText();

        if ( lineNumber >= 0 )
        {
          STR = String::Concat( "Validierungsfehler in Zeile ", Convert::ToString( lineNumber ), Environment::NewLine );
          textBoxValidierungsfehler->AppendText( STR );
        }

        textBoxValidierungsfehler->AppendText( QuConvert::ToString( errorText ) );
        textBoxValidierungsfehler->AppendText ( Environment::NewLine );
        textBoxValidierungsfehler->AppendText ( Environment::NewLine );
      }

      if ( pErrorReport->getReadErrorAnz() + pErrorReport->getValidationErrorAnz() == 0 )
        textBoxValidierungsfehler->AppendText( "Keine Fehler aufgetreten" );
    }


///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Buttob "Beenden"                                          //
//  Schließt den Dialog                                                      //
///////////////////////////////////////////////////////////////////////////////
  private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
           {
             Close();
           }
};

