#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "TimeSeries.h"
#include "UnitOfMeasure.h"
#include "Convert.h"


	/// <summary>
	/// Analysiert die einzelnen Zeitreihen im übergebenen RegularTimeSeriesSet, und gibt
  /// Die Analyseergebnisse aus:
  /// - Thematische Beschreibung der Zeitreihe
  /// - Jahressumme bzw. Jahres-Mittelwert, anhängig vom uom
  /// - Anzahl der fehlenden Werte
  /// - Anzahl der Lücken in den Zeitreihen
	/// </summary>
public ref class CSVTimeSeriesAnalysis : public System::Windows::Forms::Form
{
	public:
		CSVTimeSeriesAnalysis( RegularTimeSeriesSet * pTimeSeriesSet )
		{
			InitializeComponent();

      m_pTimeSeriesSet = pTimeSeriesSet;

      initialize();
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~CSVTimeSeriesAnalysis()
		{
			if (components)
			{
				delete components;
			}
		}
private: System::Windows::Forms::DataGridView^  dataGridViewAnalysis;
private: System::Windows::Forms::Button^  buttonEnd;
private: System::Windows::Forms::Button^  buttonSave;
protected: 

protected: 

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;
    RegularTimeSeriesSet             * m_pTimeSeriesSet;  // Zu annalysierende Zeitreihen

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->dataGridViewAnalysis = (gcnew System::Windows::Forms::DataGridView());
      this->buttonEnd = (gcnew System::Windows::Forms::Button());
      this->buttonSave = (gcnew System::Windows::Forms::Button());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewAnalysis))->BeginInit();
      this->SuspendLayout();
      // 
      // dataGridViewAnalysis
      // 
      this->dataGridViewAnalysis->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->dataGridViewAnalysis->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
      this->dataGridViewAnalysis->Location = System::Drawing::Point(0, 1);
      this->dataGridViewAnalysis->Name = L"dataGridViewAnalysis";
      this->dataGridViewAnalysis->Size = System::Drawing::Size(559, 320);
      this->dataGridViewAnalysis->TabIndex = 0;
      // 
      // buttonEnd
      // 
      this->buttonEnd->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonEnd->Location = System::Drawing::Point(473, 343);
      this->buttonEnd->Name = L"buttonEnd";
      this->buttonEnd->Size = System::Drawing::Size(75, 23);
      this->buttonEnd->TabIndex = 1;
      this->buttonEnd->Text = L"End";
      this->buttonEnd->UseVisualStyleBackColor = true;
      this->buttonEnd->Click += gcnew System::EventHandler(this, &CSVTimeSeriesAnalysis::buttonEnd_Click);
      // 
      // buttonSave
      // 
      this->buttonSave->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonSave->Location = System::Drawing::Point(13, 342);
      this->buttonSave->Name = L"buttonSave";
      this->buttonSave->Size = System::Drawing::Size(75, 23);
      this->buttonSave->TabIndex = 2;
      this->buttonSave->Text = L"Save";
      this->buttonSave->UseVisualStyleBackColor = true;
      this->buttonSave->Click += gcnew System::EventHandler(this, &CSVTimeSeriesAnalysis::buttonSave_Click);
      // 
      // CSVTimeSeriesAnalysis
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(560, 389);
      this->Controls->Add(this->buttonSave);
      this->Controls->Add(this->buttonEnd);
      this->Controls->Add(this->dataGridViewAnalysis);
      this->Name = L"CSVTimeSeriesAnalysis";
      this->Text = L"Time series analysis";
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewAnalysis))->EndInit();
      this->ResumeLayout(false);

    }
#pragma endregion

////////////////////////////////////////////////////////////////////////////////
//  Generiert die Analyseergebnisse und stellt sie im DataGridView Control dar//
////////////////////////////////////////////////////////////////////////////////
private: System::Void initialize()
         {
           DataGridViewRow          ^ row;
           DataGridViewTextBoxCell  ^ cell;
           double                     value;
           String                   ^ uomString;
           bool                       generateSum;

           dataGridViewAnalysis->Rows->Clear();

           dataGridViewAnalysis->ColumnCount = 4;
           dataGridViewAnalysis->ColumnHeadersVisible = true;

           dataGridViewAnalysis->DefaultCellStyle->WrapMode = DataGridViewTriState::True;

           DataGridViewCellStyle ^ columnHeaderStyle = gcnew DataGridViewCellStyle;
           columnHeaderStyle->BackColor = Color::Aqua;
           columnHeaderStyle->Font = gcnew System::Drawing::Font( "Verdana",8,FontStyle::Bold );
           columnHeaderStyle->WrapMode = DataGridViewTriState::True;

           dataGridViewAnalysis->ColumnHeadersDefaultCellStyle = columnHeaderStyle;
           dataGridViewAnalysis->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

           dataGridViewAnalysis->Columns[0]->Name = "Time series";
           dataGridViewAnalysis->Columns[1]->Name = "Yearly sum / average";
           dataGridViewAnalysis->Columns[2]->Name = "# missing values";
           dataGridViewAnalysis->Columns[3]->Name = "# time series gaps";

           for ( unsigned int i = 0; i < m_pTimeSeriesSet->getTimeSeriesAnz(); ++i )
           {
             RegularTimeSeries * pTimeSeries = m_pTimeSeriesSet->getTimeSeries( i );

             row = gcnew DataGridViewRow;

             std::string thematicDescription = pTimeSeries->getThematicDescription();
             cell = gcnew DataGridViewTextBoxCell;
             cell->Value = QuConvert::ToString( thematicDescription );
             row->Cells->Add( cell );

             UOM * pUOM = pTimeSeries->getUOM();
             if ( pUOM != NULL )
             {
               if ( pUOM->getQuantity() == "ENERGY" )
                 generateSum = true;
               else
                 generateSum = false;
               
               uomString = QuConvert:: ToString( pUOM->getSign() ) ;
             }
             else
             {
               generateSum = false;
               uomString = "";
             }

             if ( generateSum )
               value = pTimeSeries->getSum();
             else
               value = pTimeSeries->getAverage();

             cell = gcnew DataGridViewTextBoxCell;
             cell->Value = String::Concat( Double ( value ).ToString( "F1" ), " ", uomString );
             row->Cells->Add( cell );

             int missingValueCount = pTimeSeries->getMissingValuesCount();
             cell = gcnew DataGridViewTextBoxCell;
             cell->Value = Convert::ToString ( missingValueCount );
             row->Cells->Add( cell );

             int gapCount = pTimeSeries->getGapCount();
             cell = gcnew DataGridViewTextBoxCell;
             cell->Value = Convert::ToString ( gapCount );
             row->Cells->Add( cell );

             dataGridViewAnalysis->Rows->Add( row);
           }
         }

////////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Save": Noch nicht implementiert                    //
////////////////////////////////////////////////////////////////////////////////
private: System::Void buttonSave_Click(System::Object^  sender, System::EventArgs^  e)
         {
         }

////////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "End": Schließt den Dialog                          //
////////////////////////////////////////////////////////////////////////////////
private: System::Void buttonEnd_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           Close();
         }
};
