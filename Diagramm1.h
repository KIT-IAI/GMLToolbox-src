#pragma once

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
 	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
  using namespace  System::Windows::Forms::DataVisualization::Charting;

	/// <summary>
	/// Zusammenfassung für Diagramm
	/// </summary>
	public ref class Diagramm : public System::Windows::Forms::Form
	{
  public:
    enum class AGGREGATION {  TAEGLICH, WOECHENTLICH, MONATLICH  };
    enum class AGGREGATION_TYP  {  SUMME, MITTELWERT  };

	  public:
		Diagramm()
		{
			InitializeComponent();

      m_pChartArea = chart1->ChartAreas[0];
      m_pChartArea->AxisX->Title = "Zeit";

      m_valueAnz        = 0;
      m_valueAnzMin     = 24;
      m_missingValue    = 9.99E20;
      m_seriesAnz       = 0;

      m_startTime = nullptr;
      m_endTime   = nullptr;
      m_timeIncrement = nullptr;
      m_values = gcnew ArrayList;

//      comboBoxZeitaufloesung->SelectedIndex = 0;
      radioButtonMittelwert->Checked = true;
      radioButtonSumme->Checked     = false;

      radioButtonMittelwert->Enabled = false;
      radioButtonSumme->Enabled      = false;      
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Diagramm()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
  protected: 

	private:
		/// <summary>
		/// Erforderliche Designervariable.
    DateTime ^  m_startTime;
    DateTime ^  m_endTime;
    TimeSpan ^  m_timeIncrement;
    String   ^  m_uom;
    double      m_missingValue;
    int         m_valueAnz;
    int         m_valueAnzMin;
    int         m_seriesAnz;
    ArrayList ^ m_values;

    int              startIndex;
    int              endIndex;
    int              valuesSpanAct;
    bool             zoomEnabled;
    int              zoomstufeAct;
    int                          zoomstufeMax;

    List<int>                  ^ zoomIncrement;
    List<DateTimeIntervalType> ^ axisTypes;
    List<int>                  ^ axisOffsets;
    ArrayList                  ^ axisFormats;  

    bool        panEnabled;
    List<int> ^ panIncrement;
    int         panIncrementAct;

//    RegularTimeSeries  * m_pTimeSeries;
    ChartArea          ^ m_pChartArea;
//    Series             ^ m_pSeries;
//    Legend             ^ m_pLegend;

		/// </summary>
		System::ComponentModel::Container ^components;
  private: System::Windows::Forms::ComboBox^  comboBoxZeitaufloesung;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::RadioButton^  radioButtonMittelwert;
  private: System::Windows::Forms::RadioButton^  radioButtonSumme;
  private: System::Windows::Forms::Button^  buttonBeenden;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
      this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->comboBoxZeitaufloesung = (gcnew System::Windows::Forms::ComboBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->radioButtonMittelwert = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonSumme = (gcnew System::Windows::Forms::RadioButton());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
      this->SuspendLayout();
      // 
      // chart1
      // 
      this->chart1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->chart1->BackColor = System::Drawing::Color::LightGoldenrodYellow;
      chartArea1->AxisX->Title = L"Zeit";
      chartArea1->AxisX->TitleFont = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold));
      chartArea1->AxisY->TextOrientation = System::Windows::Forms::DataVisualization::Charting::TextOrientation::Rotated270;
      chartArea1->AxisY->TitleFont = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold));
      chartArea1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
        static_cast<System::Int32>(static_cast<System::Byte>(255)));
      chartArea1->Name = L"ChartArea1";
      this->chart1->ChartAreas->Add(chartArea1);
      this->chart1->Location = System::Drawing::Point(-7, 0);
      this->chart1->Name = L"chart1";
      this->chart1->Size = System::Drawing::Size(897, 518);
      this->chart1->TabIndex = 0;
      this->chart1->Text = L"chart1";
      this->chart1->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Diagramm::chart1_KeyUp);
      this->chart1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Diagramm::chart1_MouseClick);
      this->chart1->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &Diagramm::chart1_PreviewKeyDown);
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonBeenden->Location = System::Drawing::Point(780, 537);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 62);
      this->buttonBeenden->TabIndex = 1;
      this->buttonBeenden->Text = L"Schließen";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &Diagramm::buttonBeenden_Click);
      // 
      // comboBoxZeitaufloesung
      // 
      this->comboBoxZeitaufloesung->FormattingEnabled = true;
      this->comboBoxZeitaufloesung->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"Stunden", L"Tage", L"Wochen", L"Monate"});
      this->comboBoxZeitaufloesung->Location = System::Drawing::Point(27, 557);
      this->comboBoxZeitaufloesung->Name = L"comboBoxZeitaufloesung";
      this->comboBoxZeitaufloesung->Size = System::Drawing::Size(121, 21);
      this->comboBoxZeitaufloesung->TabIndex = 2;
      this->comboBoxZeitaufloesung->SelectedIndexChanged += gcnew System::EventHandler(this, &Diagramm::comboBoxZeitaufloesung_SelectedIndexChanged);
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(27, 586);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(97, 13);
      this->label1->TabIndex = 3;
      this->label1->Text = L"Zeitliche Auflösung";
      // 
      // radioButtonMittelwert
      // 
      this->radioButtonMittelwert->AutoSize = true;
      this->radioButtonMittelwert->Location = System::Drawing::Point(165, 560);
      this->radioButtonMittelwert->Name = L"radioButtonMittelwert";
      this->radioButtonMittelwert->Size = System::Drawing::Size(70, 17);
      this->radioButtonMittelwert->TabIndex = 4;
      this->radioButtonMittelwert->TabStop = true;
      this->radioButtonMittelwert->Text = L"Mittelwert";
      this->radioButtonMittelwert->UseVisualStyleBackColor = true;
      // 
      // radioButtonSumme
      // 
      this->radioButtonSumme->AutoSize = true;
      this->radioButtonSumme->Location = System::Drawing::Point(165, 583);
      this->radioButtonSumme->Name = L"radioButtonSumme";
      this->radioButtonSumme->Size = System::Drawing::Size(60, 17);
      this->radioButtonSumme->TabIndex = 5;
      this->radioButtonSumme->TabStop = true;
      this->radioButtonSumme->Text = L"Summe";
      this->radioButtonSumme->UseVisualStyleBackColor = true;
      // 
      // Diagramm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->AutoScroll = true;
      this->ClientSize = System::Drawing::Size(867, 616);
      this->Controls->Add(this->radioButtonSumme);
      this->Controls->Add(this->radioButtonMittelwert);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->comboBoxZeitaufloesung);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->chart1);
      this->KeyPreview = true;
      this->Name = L"Diagramm";
      this->Text = L"Diagramm";
      this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Diagramm::Diagramm_KeyUp);
      this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Diagramm::Diagramm_MouseWheel);
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  

    /////////////////////////////////////////////////////
    //  Public Methoden

  public: System::Void setStartTime ( int year, int month, int day, int hour, int minute, int second )
          {
            m_startTime = gcnew DateTime ( year, month, day, hour, minute, second );
          }

  public: System::Void setEndTime ( int year, int month, int day, int hour, int minute, int second )
          {
            m_endTime = gcnew DateTime ( year, month, day, hour, minute, second );
          }

  public: System::Void setTimeIncrement ( int days, int hours, int minutes, int seconds )
          {
            m_timeIncrement = gcnew TimeSpan ( days, hours, minutes, seconds );
          }

  public: System::Void setValues ( std::vector<double> &values,  std::string theme  )
          {
            List<double> ^ valueList = gcnew List<double>;
            for ( unsigned int i = 0; i < values.size(); i++ )
              valueList->Add( values[i] );

            m_values->Add( valueList );

            m_seriesAnz++;

            System::Windows::Forms::DataVisualization::Charting::Series^  series = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            System::Windows::Forms::DataVisualization::Charting::Legend^  legend = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());

            String ^seriesName = String::Concat ( "Series", Convert::ToString ( m_seriesAnz ) );
            String ^legendName = String::Concat ( "Legend", Convert::ToString ( m_seriesAnz ) );

            series->ChartArea = L"ChartArea1";
            series->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
            series->EmptyPointStyle->Color = System::Drawing::Color::Red;
            series->EmptyPointStyle->MarkerColor = System::Drawing::Color::Fuchsia;
            series->EmptyPointStyle->MarkerStyle = System::Windows::Forms::DataVisualization::Charting::MarkerStyle::Square;
            series->Legend =legendName;
            series->LegendText = gcnew String ( theme.c_str() );
            series->Name = seriesName;
            series->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;
            series->YValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::Double;
            this->chart1->Series->Add(series);

            legend->Alignment = System::Drawing::StringAlignment::Center;
            legend->BackColor = System::Drawing::Color::LightGoldenrodYellow;
            legend->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Top;
            legend->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold));
            legend->IsTextAutoFit = false;
            legend->Name = legendName;
            this->chart1->Legends->Add(legend);

            int valueAnz = valueList->Count;
            if ( valueAnz < m_valueAnz || m_valueAnz == 0 )
            {
              m_valueAnz = valueAnz;
              prepareZoomstufen();
            }
          }

  public: System::Void setUOM ( std::string uom )
          {
            m_uom = gcnew String ( uom.c_str() );

            m_pChartArea->AxisY->Title = m_uom;
          }

  public: System::Void setMissingValue ( double value )
          {
            m_missingValue = value;
          }

  public: System::Void initialize ()
          {
            comboBoxZeitaufloesung->SelectedIndex = 0;
          }


private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
           {
             Close();
           }

private: System::Void prepareZoomstufen()
         {
           int valueAnz, zoomstufe, anzHours, anzHoursIncrement;

           zoomIncrement = gcnew List<int>;
           panIncrement  = gcnew List<int>;
           axisTypes     = gcnew List<DateTimeIntervalType>;
           axisOffsets   = gcnew List<int>;
           axisFormats   = gcnew ArrayList;

          anzHoursIncrement = 24*m_timeIncrement->Days + m_timeIncrement->Hours;

           zoomstufe = 0;
           valueAnz = m_valueAnz;
           while ( valueAnz > m_valueAnzMin )
           {
             int inc = int ( valueAnz / 4.0 );
             zoomIncrement->Add(  inc );
//             valuesSpan->Add( valueAnz );
             panIncrement->Add( int ( valueAnz / 10.0) );
             
             zoomstufe++;
             valueAnz = valueAnz - 2*inc;
             anzHours = anzHoursIncrement * valueAnz;
             if ( anzHours > 2160 )
             {
               axisTypes->Add ( DateTimeIntervalType::Months );
               axisFormats->Add ( "d" );
               axisOffsets->Add( 1 );
             }
             else
             if ( anzHours > 720 )
             {
               axisTypes->Add ( DateTimeIntervalType::Days );
               axisFormats->Add ( "d" );
               axisOffsets->Add( 14 );
             }
             else
             if ( anzHours > 360 )
             {
               axisTypes->Add ( DateTimeIntervalType::Days );
               axisOffsets->Add( 7 );
               axisFormats->Add ( "d" );
             }
             else
             if ( anzHours > 180 )
             {
               axisTypes->Add ( DateTimeIntervalType::Days );
               axisOffsets->Add( 3 );
               axisFormats->Add ( "d" );
             }
             else
             if ( anzHours > 90 )
             {
               axisTypes->Add ( DateTimeIntervalType::Days );
               axisOffsets->Add( 1 );
               axisFormats->Add ( "d" );
             }
             else
             if ( anzHours > 45 )
             {
               axisTypes->Add ( DateTimeIntervalType::Hours );
               axisOffsets->Add( 12 );
               axisFormats->Add ( "g" );
             }
             else
             {
               axisTypes->Add ( DateTimeIntervalType::Hours );
               axisOffsets->Add( 6 );
               axisFormats->Add ( "g" );
             }

           }
           zoomstufeMax = zoomstufe;
           zoomstufeAct = 0;
           zoomEnabled = true;

           panEnabled  = false;
           panIncrementAct = 0;

           startIndex      = 0;
           endIndex        = m_valueAnz;
           valuesSpanAct   = m_valueAnz;

           m_pChartArea->AxisX->IntervalOffsetType =  axisTypes[0];
           m_pChartArea->AxisX->IntervalOffset = axisOffsets[0];
           m_pChartArea->AxisX->MajorTickMark->IntervalOffset = axisOffsets[0];
           m_pChartArea->AxisX->MajorTickMark->IntervalOffsetType = axisTypes[0];
           m_pChartArea->AxisX->MajorTickMark->IntervalType = axisTypes[0];
           m_pChartArea->AxisX->LabelStyle->Format = dynamic_cast<String^>(axisFormats[0]);

         }

private: System::Void comboBoxZeitaufloesung_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
         {
           int                            i, j, anz;
           double                         value;
           DateTime                     ^ dateTime;
           DataPoint                    ^ dataPoint;
           String                       ^ zeitaufloesung;

           if ( comboBoxZeitaufloesung->SelectedIndex != 0 )
           {
             radioButtonMittelwert->Enabled = true;
             radioButtonSumme->Enabled      = true;
           }
           else
           {
             radioButtonMittelwert->Enabled = false;
             radioButtonSumme->Enabled      = false;
           }

           if ( comboBoxZeitaufloesung->SelectedIndex == 0 )
           {
             zoomEnabled = true;

             fillChart();
             chart1->Focus();
           }
           else
           {
             AGGREGATION       aggregation;
             AGGREGATION_TYP   aggregationTyp;
             List<double>    ^ values = gcnew List<double>;
             List<int>       ^ dateTimeIndices = gcnew List<int>;
             zoomEnabled = false;

             switch ( comboBoxZeitaufloesung->SelectedIndex )
             {
               case 1: 
                 aggregation = AGGREGATION::TAEGLICH;
                 if ( radioButtonMittelwert->Checked )
                   zeitaufloesung = "Tagesmittelwerte";
                 else
                   zeitaufloesung = "Tagessummenwerte";
                 break;
               case 2:
                 aggregation = AGGREGATION::WOECHENTLICH;
                 if ( radioButtonMittelwert->Checked )
                   zeitaufloesung = "Wochenmittelwerte";
                 else
                   zeitaufloesung = "Wochensummenwerte";
                 break;
                 break;
               case 3:
                 aggregation = AGGREGATION::MONATLICH;
                 if ( radioButtonMittelwert->Checked )
                   zeitaufloesung = "Monatsmittelwerte";
                 else
                   zeitaufloesung = "Monatsummenwerte";
                 break;
                 break;
             }

//             m_pSeries->LegendText = String::Concat( m_theme, ", ", zeitaufloesung );
             m_pChartArea->AxisX->IntervalOffsetType =  axisTypes[0];
             m_pChartArea->AxisX->IntervalOffset = axisOffsets[0];
             m_pChartArea->AxisX->MajorTickMark->IntervalOffset = axisOffsets[0];
             m_pChartArea->AxisX->MajorTickMark->IntervalOffsetType = axisTypes[0];
             m_pChartArea->AxisX->MajorTickMark->IntervalType = axisTypes[0];
             m_pChartArea->AxisX->LabelStyle->Format = dynamic_cast<String^>(axisFormats[0]);

             if ( radioButtonMittelwert->Checked )
               aggregationTyp = AGGREGATION_TYP::MITTELWERT;
             else
               aggregationTyp = AGGREGATION_TYP::SUMME;

             for ( i = 0; i < m_seriesAnz; i++ )
             {
               Series       ^ series = dynamic_cast<Series^>(this->chart1->Series[i]);

               series->Points->Clear();
               values->Clear();
               dateTimeIndices->Clear();

               anz = getValues( i, aggregation, aggregationTyp, values, dateTimeIndices );
               for ( j = 0; j <anz; j++ )
               {
                 value = values[j];
                 dateTime = getTimePosition ( dateTimeIndices[j] );

                 dataPoint = gcnew DataPoint ();
                 if ( value == m_missingValue )
                 {
                   dataPoint->SetValueXY( dateTime, 0.0 );
                   dataPoint->IsEmpty = true;
                 }
                 else
                   dataPoint->SetValueXY( dateTime, value );

                 series->Points->Add( dataPoint);
               }
             }
           }
           chart1->Refresh();
         }

private: System::Void fillChart()
         {
           int           i, j;
           double        value;
           DateTime    ^ dateTime;
           DataPoint   ^ dataPoint;

           for ( i = 0; i < m_seriesAnz; i++ )
           {
             Series       ^ series = dynamic_cast<Series^>(this->chart1->Series[i]);
             List<double> ^ values  = dynamic_cast<List<double>^>(m_values[i]);

             series->Points->Clear();
             for ( j = startIndex; j < endIndex;  j++ )
             {
               value    = values[j];          
               dateTime =  getTimePosition ( j );

               dataPoint = gcnew DataPoint ();
               if ( value ==m_missingValue )
               {
                 dataPoint->SetValueXY( dateTime, 0.0 );
                 dataPoint->IsEmpty = true;
               }
               else
                 dataPoint->SetValueXY( dateTime, value );

               series->Points->Add( dataPoint);
             }		
           }
         }


private: System::Void Diagramm_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^ e)
         {
           if ( !zoomEnabled )
             return;

            int delta = e->Delta;
            changeZoomstufe( delta );

         }

private: System::Void changeZoomstufe ( int delta )
         {
           if ( delta > 0 && zoomstufeAct < zoomstufeMax )
           {
             startIndex = startIndex + zoomIncrement[zoomstufeAct];
             endIndex   = endIndex   - zoomIncrement[zoomstufeAct];
             valuesSpanAct = valuesSpanAct - 2*zoomIncrement[zoomstufeAct];

             zoomstufeAct++;
             panIncrementAct = panIncrement[zoomstufeAct];             

             m_pChartArea->AxisX->IntervalOffsetType =  axisTypes[zoomstufeAct];
             m_pChartArea->AxisX->IntervalOffset = axisOffsets[zoomstufeAct];
             m_pChartArea->AxisX->MajorTickMark->IntervalOffset = axisOffsets[zoomstufeAct];
             m_pChartArea->AxisX->MajorTickMark->IntervalOffsetType = axisTypes[zoomstufeAct];
             m_pChartArea->AxisX->MajorTickMark->IntervalType = axisTypes[zoomstufeAct];
             m_pChartArea->AxisX->LabelStyle->Format = dynamic_cast<String^>(axisFormats[zoomstufeAct]);

             fillChart();
           }
           else
           if ( delta < 0 && zoomstufeAct > 0 )              
           {
             zoomstufeAct = zoomstufeAct - 1;
             valuesSpanAct      = valuesSpanAct + 2*zoomIncrement[zoomstufeAct];
             panIncrementAct = panIncrement[zoomstufeAct];             

             if ( endIndex + zoomIncrement[zoomstufeAct] <= m_valueAnz && startIndex - zoomIncrement[zoomstufeAct] >= 0 )
             {
               startIndex      = startIndex -  zoomIncrement[zoomstufeAct];
               endIndex        = endIndex   + zoomIncrement[zoomstufeAct];
             }
             else
             if ( endIndex + zoomIncrement[zoomstufeAct] > m_valueAnz )
             {
               startIndex = m_valueAnz - valuesSpanAct;
               endIndex   = m_valueAnz;
             }
             else
             {
               startIndex = 0;
               endIndex   = valuesSpanAct;
             }

             m_pChartArea->AxisX->IntervalOffsetType =  axisTypes[zoomstufeAct];
             m_pChartArea->AxisX->IntervalOffset = axisOffsets[zoomstufeAct];
             m_pChartArea->AxisX->MajorTickMark->IntervalOffset = axisOffsets[zoomstufeAct];
             m_pChartArea->AxisX->MajorTickMark->IntervalOffsetType = axisTypes[zoomstufeAct];
             m_pChartArea->AxisX->MajorTickMark->IntervalType = axisTypes[zoomstufeAct];
             m_pChartArea->AxisX->LabelStyle->Format = dynamic_cast<String^>(axisFormats[zoomstufeAct]);

             fillChart();
           }
         }

private: System::Void Diagramm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
         {
         }

private: System::Void radioButtonZoom_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e) 
         {
         }
private: System::Void chart1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
         {
           if ( zoomstufeAct == 0 )
             return;

           if ( e->KeyCode == System::Windows::Forms::Keys::Right )
           {
             if ( endIndex == m_valueAnz ) return;

             if ( endIndex + panIncrementAct <= m_valueAnz )
             {
               startIndex = startIndex + panIncrementAct;
               endIndex   = endIndex   + panIncrementAct;
             }
             else
             {
               endIndex = m_valueAnz;
               startIndex = m_valueAnz - valuesSpanAct;
             }
             fillChart();          
           }
           else
           if ( e->KeyCode == System::Windows::Forms::Keys::Left )
           {
             if ( startIndex == 0 ) return;

             if ( startIndex - panIncrementAct >= 0 )
             {
               startIndex = startIndex - panIncrementAct;
               endIndex   = endIndex   - panIncrementAct;
             }
             else
             {
               startIndex = 0;
               endIndex   = valuesSpanAct;
             }
             fillChart();          
           }
           else
           if ( e->KeyCode == System::Windows::Forms::Keys::Up )
             changeZoomstufe( 120 );
           else
           if ( e->KeyCode == System::Windows::Forms::Keys::Down )
             changeZoomstufe( -120 );
         }

private: System::Void chart1_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e)
         {
           if ( e->KeyCode == System::Windows::Forms::Keys::Right || 
                e->KeyCode == System::Windows::Forms::Keys::Left ||
                e->KeyCode == System::Windows::Forms::Keys::Up   ||
                e->KeyCode == System::Windows::Forms::Keys::Down )
             e->IsInputKey = true;
         }

private: System::Void chart1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
         {
           chart1->Focus();
         }

private: unsigned int getValues ( int index, AGGREGATION aggregation, AGGREGATION_TYP aggregationTyp, List<double> ^ valuesAggregated, List<int> ^ dateTimeIndices )
         {
           double         value, valueAggregated;
           int            anzValue, anzDay;
           int            month, day, ind;
           List<double> ^ values = dynamic_cast<List<double>^>(m_values[index]);

           valueAggregated = 0;
           anzValue        = 0;
           anzDay          = 0;
           month           = m_startTime->Month;
           day             = m_startTime->Day;
           DateTime ^ startTime = getTimePosition ( 0 );
           ind             = 0;

           for ( int i = 0; i  < m_valueAnz; i++ )
           {
             DateTime ^ actTime = getTimePosition( i );
             value = values[i];

             if ( actTime->Day != day && aggregation != AGGREGATION::MONATLICH )
             {
               anzDay++;
               day =  actTime->Day;
               if ( (aggregation == AGGREGATION::TAEGLICH && anzDay == 1) || (aggregation == AGGREGATION::WOECHENTLICH && anzDay == 7) )
               {
                 if ( anzValue > 0)
                 {
                   if ( aggregationTyp == AGGREGATION_TYP::MITTELWERT  )
                     valueAggregated = valueAggregated / anzValue;
                 }
                 else
                   valueAggregated = m_missingValue;

                 valuesAggregated->Add( valueAggregated );
                 dateTimeIndices->Add ( ind );

                 startTime = actTime;
                 ind       = i;
                 anzDay = 0;
                 anzValue = 0;
                 valueAggregated = 0.0;
               }
             }

             if ( actTime->Month != month && aggregation == AGGREGATION::MONATLICH )
             {
               month = actTime->Month;
               if ( anzValue > 0 )
               {
                 if ( aggregationTyp == AGGREGATION_TYP::MITTELWERT  )
                   valueAggregated = valueAggregated / anzValue;
               }
               else
                 valueAggregated =m_missingValue;

               valuesAggregated->Add( valueAggregated );
               dateTimeIndices->Add( ind );

               startTime = actTime;
               ind = i;
               anzValue = 0;
               valueAggregated = 0.0;
             }

             if ( value !=m_missingValue )
             {
               valueAggregated = valueAggregated + value;
               anzValue++;
             }
           }

           if ( anzValue > 0 )
           {
             if ( aggregationTyp == AGGREGATION_TYP::MITTELWERT  )
               valueAggregated = valueAggregated / anzValue;  
             else
               valueAggregated = m_missingValue;

             valuesAggregated->Add( valueAggregated );
             dateTimeIndices->Add( ind );
           }

           return valuesAggregated->Count;
         }

private: DateTime ^  getTimePosition( unsigned int count )
         {
           int days = m_timeIncrement->Days;
           int hours = m_timeIncrement->Hours;
           int minutes = m_timeIncrement->Minutes;
           int seconds = m_timeIncrement->Seconds;


           TimeSpan ^ timeSpan = gcnew TimeSpan ( days*(count+1), hours*(count+1), minutes*(count+1), seconds*(count+1) );
           DateTime ^ newDateTime = gcnew DateTime;
           newDateTime = *m_startTime + *timeSpan;
           return newDateTime;         
         }



};
