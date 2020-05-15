#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "TimeSeries.h"
#include "Convert.h"

	/// <summary>
	/// Schließen von Lücken in regulären Zeitreihen durch lineare Interpolation
	/// </summary>
public ref class TimeSeriesGaps : public System::Windows::Forms::Form
{
	public:
		TimeSeriesGaps( RegularTimeSeriesSet * pTimeSeriesSet )
		{
			InitializeComponent();
      m_pTimeSeriesSet = pTimeSeriesSet;
      textBoxMaxGapSize->Text = "4";
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~TimeSeriesGaps()
		{
			if (components)
			{
				delete components;
			}
		}

  private: System::Windows::Forms::TextBox^  textBoxMaxGapSize;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  textBoxMessages;
  private: System::Windows::Forms::Button^  buttonRun;
  private: System::Windows::Forms::Button^  buttonEnd;
  private: System::Windows::Forms::SaveFileDialog^  saveFileDialogResult;

private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
	System::ComponentModel::Container ^ components;
  RegularTimeSeriesSet              * m_pTimeSeriesSet;  // Zeitreihen-Menge, die bearbeitet werden soll

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->textBoxMaxGapSize = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBoxMessages = (gcnew System::Windows::Forms::TextBox());
      this->buttonRun = (gcnew System::Windows::Forms::Button());
      this->buttonEnd = (gcnew System::Windows::Forms::Button());
      this->saveFileDialogResult = (gcnew System::Windows::Forms::SaveFileDialog());
      this->SuspendLayout();
      // 
      // textBoxMaxGapSize
      // 
      this->textBoxMaxGapSize->Location = System::Drawing::Point( 192, 16 );
      this->textBoxMaxGapSize->Margin = System::Windows::Forms::Padding( 4, 4, 4, 4 );
      this->textBoxMaxGapSize->Name = L"textBoxMaxGapSize";
      this->textBoxMaxGapSize->Size = System::Drawing::Size( 76, 22 );
      this->textBoxMaxGapSize->TabIndex = 1;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point( 277, 21 );
      this->label1->Margin = System::Windows::Forms::Padding( 4, 0, 4, 0 );
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size( 115, 17 );
      this->label1->TabIndex = 2;
      this->label1->Text = L"Maxímal gap size";
      // 
      // textBoxMessages
      // 
      this->textBoxMessages->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
        | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxMessages->Location = System::Drawing::Point( 17, 169 );
      this->textBoxMessages->Margin = System::Windows::Forms::Padding( 4, 4, 4, 4 );
      this->textBoxMessages->Multiline = true;
      this->textBoxMessages->Name = L"textBoxMessages";
      this->textBoxMessages->ReadOnly = true;
      this->textBoxMessages->Size = System::Drawing::Size( 539, 123 );
      this->textBoxMessages->TabIndex = 3;
      // 
      // buttonRun
      // 
      this->buttonRun->Font = (gcnew System::Drawing::Font( L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0) ));
      this->buttonRun->Location = System::Drawing::Point( 236, 106 );
      this->buttonRun->Margin = System::Windows::Forms::Padding( 4, 4, 4, 4 );
      this->buttonRun->Name = L"buttonRun";
      this->buttonRun->Size = System::Drawing::Size( 100, 55 );
      this->buttonRun->TabIndex = 4;
      this->buttonRun->Text = L"Run";
      this->buttonRun->UseVisualStyleBackColor = true;
      this->buttonRun->Click += gcnew System::EventHandler( this, &TimeSeriesGaps::buttonRun_Click );
      // 
      // buttonEnd
      // 
      this->buttonEnd->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonEnd->Location = System::Drawing::Point( 457, 314 );
      this->buttonEnd->Margin = System::Windows::Forms::Padding( 4, 4, 4, 4 );
      this->buttonEnd->Name = L"buttonEnd";
      this->buttonEnd->Size = System::Drawing::Size( 100, 28 );
      this->buttonEnd->TabIndex = 5;
      this->buttonEnd->Text = L"End";
      this->buttonEnd->UseVisualStyleBackColor = true;
      this->buttonEnd->Click += gcnew System::EventHandler( this, &TimeSeriesGaps::buttonEnd_Click );
      // 
      // saveFileDialogResult
      // 
      this->saveFileDialogResult->DefaultExt = L"(*.csv)|*.csv";
      this->saveFileDialogResult->Title = L"Save corrected time sertie";
      // 
      // TimeSeriesGaps
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF( 8, 16 );
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size( 573, 367 );
      this->Controls->Add( this->buttonEnd );
      this->Controls->Add( this->buttonRun );
      this->Controls->Add( this->textBoxMessages );
      this->Controls->Add( this->label1 );
      this->Controls->Add( this->textBoxMaxGapSize );
      this->Margin = System::Windows::Forms::Padding( 4, 4, 4, 4 );
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"TimeSeriesGaps";
      this->Text = L"TimeSeriesGaps";
      this->ResumeLayout( false );
      this->PerformLayout();

    }
#pragma endregion

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Run": Startet das Füllen der Lücken               //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonRun_Click(System::Object^  sender, System::EventArgs^  e)
           {
               linearInterpolation();
           }

///////////////////////////////////////////////////////////////////////////////
//  Füllen der Lücken                                                        //
///////////////////////////////////////////////////////////////////////////////
private: System::Void linearInterpolation()
         {
           int maxGapSize = Convert::ToInt32( textBoxMaxGapSize->Text );
           String ^ STR = "";

           STR = "Time series status after interpolation procedure:\n";

           for ( unsigned int i = 0; i <  m_pTimeSeriesSet->getTimeSeriesAnz(); ++i )
           {
             RegularTimeSeries * pTimeSeries = m_pTimeSeriesSet->getTimeSeries( i );
             pTimeSeries->interpolateGaps( maxGapSize );

             int missingValueCount      = pTimeSeries->getMissingValuesCount();
             int gapCount               = pTimeSeries->getGapCount();
             std::string description    = pTimeSeries->getThematicDescription();

             String ^ STR_TEMP = String::Concat( QuConvert::ToString( description ), ": ", 
                        Convert::ToString( missingValueCount ), "missing values in", Convert::ToString( gapCount ), " gaps" );
             STR = String::Concat( STR, STR_TEMP, "\n" );
           }
           textBoxMessages->Text = STR;
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "End": Schließt den Dialog                         //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonEnd_Click(System::Object^  sender, System::EventArgs^  e)
         {
           Close();
         }
};
