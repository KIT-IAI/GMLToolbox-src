#pragma once

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

#include "Convert.h"
#include "UnitOfMeasure.h"
#include "TimeSeriesRepository.h"

	/// <summary>
	///  Kombination und Darstellung ausgewählter Zeitreihen
	/// </summary>
	public ref class TimeSeriesCombination : public System::Windows::Forms::Form
	{
	public:
		TimeSeriesCombination( TimeSeriesRepository * pRepository,  ArrayList ^ vDescriptions, 
                  ArrayList ^ vTimeSeriesSetPropertiesIds, ArrayList ^ vTimeSeriesIndices )
		{
			InitializeComponent();

      m_pRepository                = pRepository;
      m_Descriptions               = vDescriptions;
      m_TimeSeriesSetPropertiesIds = vTimeSeriesSetPropertiesIds;
      m_TimeSeriesIndices          = vTimeSeriesIndices;

      m_pTimeSeriesAct              = NULL;
      m_pTimeSeriesSetPropertiesAct = NULL;
      
      initialize();		
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~TimeSeriesCombination()
		{
			if (components)
			{
				delete components;
			}
		}


  private: System::Windows::Forms::DataGridView^  dataGridViewCombination;
  private: System::Windows::Forms::Button^  buttonDisplay;
  private: System::Windows::Forms::Button^  buttonEnd;
  private: System::Windows::Forms::Button^  buttonBack;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

    TimeSeriesRepository * m_pRepository;     // Zeitreihen-Repository

    ArrayList ^ m_Descriptions;               // Liste der themat. Beschreibungen der übergebenen Zeitreihen
    ArrayList ^ m_TimeSeriesSetPropertiesIds; // Liste der Metadaten-Ids der übergebenen Zeitreihen
    ArrayList ^ m_TimeSeriesIndices;          // Liste der indizes, unter denen die übergebenen Zeitreihen im 
                                              // zug. Metadatensatz referiert sind

    RegularTimeSeries              * m_pTimeSeriesAct;              // Aktuell ausgewählte Zeitreihe
    RegularTimeSeriesSetProperties * m_pTimeSeriesSetPropertiesAct; // Metadatensatz des RegularTimeSeriesSet der aktuell
                                                                    // ausgewählten Zeitreihe

    int m_timeSeriesYear;  // Der Zeitreihen zugeordnetes Kalenderjahr


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->dataGridViewCombination = (gcnew System::Windows::Forms::DataGridView());
      this->buttonDisplay = (gcnew System::Windows::Forms::Button());
      this->buttonEnd = (gcnew System::Windows::Forms::Button());
      this->buttonBack = (gcnew System::Windows::Forms::Button());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewCombination))->BeginInit();
      this->SuspendLayout();
      // 
      // dataGridViewCombination
      // 
      this->dataGridViewCombination->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->dataGridViewCombination->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
      this->dataGridViewCombination->Location = System::Drawing::Point(13, 12);
      this->dataGridViewCombination->Name = L"dataGridViewCombination";
      this->dataGridViewCombination->Size = System::Drawing::Size(628, 281);
      this->dataGridViewCombination->TabIndex = 2;
      // 
      // buttonDisplay
      // 
      this->buttonDisplay->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
      this->buttonDisplay->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->buttonDisplay->Location = System::Drawing::Point(290, 325);
      this->buttonDisplay->Name = L"buttonDisplay";
      this->buttonDisplay->Size = System::Drawing::Size(75, 47);
      this->buttonDisplay->TabIndex = 4;
      this->buttonDisplay->Text = L"Display";
      this->buttonDisplay->UseVisualStyleBackColor = true;
      this->buttonDisplay->Click += gcnew System::EventHandler(this, &TimeSeriesCombination::buttonDisplay_Click);
      // 
      // buttonEnd
      // 
      this->buttonEnd->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonEnd->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonEnd->Location = System::Drawing::Point(565, 337);
      this->buttonEnd->Name = L"buttonEnd";
      this->buttonEnd->Size = System::Drawing::Size(75, 23);
      this->buttonEnd->TabIndex = 5;
      this->buttonEnd->Text = L"End";
      this->buttonEnd->UseVisualStyleBackColor = true;
      this->buttonEnd->Click += gcnew System::EventHandler(this, &TimeSeriesCombination::buttonEnd_Click);
      // 
      // buttonBack
      // 
      this->buttonBack->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonBack->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonBack->Location = System::Drawing::Point(16, 337);
      this->buttonBack->Name = L"buttonBack";
      this->buttonBack->Size = System::Drawing::Size(75, 23);
      this->buttonBack->TabIndex = 6;
      this->buttonBack->Text = L"<-- Back";
      this->buttonBack->UseVisualStyleBackColor = true;
      this->buttonBack->Click += gcnew System::EventHandler(this, &TimeSeriesCombination::buttonBack_Click);
      // 
      // TimeSeriesCombination
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(653, 376);
      this->Controls->Add(this->buttonBack);
      this->Controls->Add(this->buttonEnd);
      this->Controls->Add(this->buttonDisplay);
      this->Controls->Add(this->dataGridViewCombination);
      this->Name = L"TimeSeriesCombination";
      this->Text = L"TimeSeriesCombination";
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewCombination))->EndInit();
      this->ResumeLayout(false);

    }
#pragma endregion

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Anzeige der übergebenen Zeitreihen                     //
//  Für jede Zeitreihe kann ausgewählt werden:                               //
//  - Die Thematische Beschreibung des Diagramms (Spalte 0 DataGridView)     //
//  - Die Nummer der Diagramm-Ebene (1-4) (Spalte 1 DataGridView)            //
///////////////////////////////////////////////////////////////////////////////
System::Void initialize()	
{
  DataGridViewRow          ^ row;

  int anz = m_TimeSeriesSetPropertiesIds->Count;
  if ( anz == 0 )
    return;

  dataGridViewCombination->Rows->Clear();
  dataGridViewCombination->ColumnCount = 2;
  dataGridViewCombination->ColumnHeadersVisible = true;
  dataGridViewCombination->DefaultCellStyle->WrapMode = DataGridViewTriState::True;

  DataGridViewCellStyle ^ columnHeaderStyle = gcnew DataGridViewCellStyle;
  columnHeaderStyle->BackColor = Color::Aqua;
  columnHeaderStyle->Font = gcnew System::Drawing::Font( "Verdana",8,FontStyle::Bold );
  columnHeaderStyle->WrapMode = DataGridViewTriState::True;

  dataGridViewCombination->ColumnHeadersDefaultCellStyle = columnHeaderStyle;
  dataGridViewCombination->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

  dataGridViewCombination->Columns[0]->Name = "Time series label";
  dataGridViewCombination->Columns[1]->Name = "Diagram number";

  dataGridViewCombination->Columns[0]->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::None;
  dataGridViewCombination->Columns[0]->MinimumWidth = 200;
  dataGridViewCombination->Columns[1]->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;

  updateSelection( 0 );
  m_timeSeriesYear = m_pTimeSeriesSetPropertiesAct->getYear();
  bool multipleYear = false;

  for ( int i = 0; i < anz; i++ )
  {
    row = gcnew DataGridViewRow;

    DataGridViewTextBoxCell  ^cell = gcnew DataGridViewTextBoxCell;
    cell->Value = m_Descriptions[i];
    row->Cells->Add( cell );

    DataGridViewComboBoxCell ^ comboBoxCell = gcnew DataGridViewComboBoxCell;
    comboBoxCell->ReadOnly = false;
    comboBoxCell->Items->Add ( "1" );
    comboBoxCell->Items->Add ( "2" );
    comboBoxCell->Items->Add ( "3" );
    comboBoxCell->Items->Add ( "4" );

    comboBoxCell->Value = "1" ;

    row->Cells->Add ( comboBoxCell );
    dataGridViewCombination->Rows->Add( row);

    updateSelection( i );
   if ( m_pTimeSeriesSetPropertiesAct->getYear() != m_timeSeriesYear )
      multipleYear = true;
  } 

  if ( multipleYear )
    m_timeSeriesYear = 2022;
}

////////////////////////////////////////////////////////////////////////////////////
//  Aktualisiert die Attribute m_pTimeSeriesAct und m_pTimeSeriesSetPropertiesAct //
//  nach Auswahl der Zeile index in der Auswahlliste                              //
////////////////////////////////////////////////////////////////////////////////////
System::Void updateSelection ( int index )
{
  std::string id;

  String ^ STR = dynamic_cast<String^>(m_TimeSeriesIndices[index]);
  int timeSeriesIndex = Convert::ToInt32( STR );

  STR = dynamic_cast<String^>(m_TimeSeriesSetPropertiesIds[index]);
  QuConvert::systemStr2stdStr( id, STR );

  RegularTimeSeriesSet * pTimeSeriesSet = m_pRepository->getTimeSeriesSetFromPropertyId ( id );

  m_pTimeSeriesAct              = pTimeSeriesSet->getTimeSeries( timeSeriesIndex );
  m_pTimeSeriesSetPropertiesAct = pTimeSeriesSet->getMetaData();
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Display": Generiert eine Diagrammdarstellung der  //
//  Zeitreihen                                                               //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonDisplay_Click(System::Object^  sender, System::EventArgs^  e)
{
  UOM                     * pUom;
  string                    uom;
  Diagramm::AGGREGATION_TYP aggregationType;

  System::Collections::Generic::List<double>  ^ pValues = gcnew System::Collections::Generic::List<double>;

  Diagramm ^ pDiagramm = gcnew Diagramm;

  pDiagramm->setStartTime ( m_timeSeriesYear, 1, 1, 0, 0, 0 );
  pDiagramm->setEndTime ( m_timeSeriesYear, 12, 31, 23, 0, 0 );
  pDiagramm->setTimeIncrement ( 0, 1, 0, 0 );

  for ( int i = 0; i < m_TimeSeriesSetPropertiesIds->Count; ++i )
  {
    updateSelection( i );

    DataGridViewRow ^ row = dataGridViewCombination->Rows[i];
    String                  ^ thematicDescription = dynamic_cast<String^>(row->Cells[0]->Value);
    int                       diagramNumber = Convert::ToInt32( row->Cells[1]->Value );

    pUom = m_pTimeSeriesAct->getUOM();
    if ( pUom != NULL )
      uom =  pUom->getSign();
    else
      uom = "";

    if ( pUom->getQuantity() == "ENERGY" )
      aggregationType = Diagramm::AGGREGATION_TYP::SUMME;
    else
      aggregationType = Diagramm::AGGREGATION_TYP::MITTELWERT;

    pValues->Clear();
    for ( unsigned int j = 0; j <  m_pTimeSeriesAct->getValueCount(); j++  )
      pValues->Add( m_pTimeSeriesAct->getValue( j ) );

    pDiagramm->setValues( pValues, thematicDescription, QuConvert::ToString( uom ), diagramNumber, aggregationType );

  }

  pDiagramm->Show();
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "<-- Back": Beendet den Dialog und liefert         //
//  "Cancel" als Dialog-Result                                               //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBack_Click(System::Object^  sender, System::EventArgs^  e) 
         {
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "End": Beendet den Dialog und liefert              //
//  "OK" als Dialog-Result                                                   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonEnd_Click(System::Object^  sender, System::EventArgs^  e) 
         {
         }
};
