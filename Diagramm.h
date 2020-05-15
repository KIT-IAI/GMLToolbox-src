#pragma once

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
  using namespace System::Windows::Forms::DataVisualization;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für Diagramm
	/// </summary>
	public ref class Diagramm : public System::Windows::Forms::Form
	{
	public:
		Diagramm(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
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
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
      System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
      System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
      this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
      this->SuspendLayout();
      // 
      // chart1
      // 
      chartArea1->Name = L"ChartArea1";
      this->chart1->ChartAreas->Add(chartArea1);
      legend1->Name = L"Legend1";
      this->chart1->Legends->Add(legend1);
      this->chart1->Location = System::Drawing::Point(74, 75);
      this->chart1->Name = L"chart1";
      series1->ChartArea = L"ChartArea1";
      series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
      series1->Legend = L"Legend1";
      series1->Name = L"Series1";
      series1->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::Double;
      series1->YValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::Double;
      this->chart1->Series->Add(series1);
      this->chart1->Size = System::Drawing::Size(555, 300);
      this->chart1->TabIndex = 0;
      this->chart1->Text = L"chart1";
      this->chart1->Click += gcnew System::EventHandler(this, &Diagramm::chart1_Click);
      // 
      // Diagramm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(677, 512);
      this->Controls->Add(this->chart1);
      this->Name = L"Diagramm";
      this->Text = L"Diagramm";
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
      this->ResumeLayout(false);

    }
#pragma endregion
  private: System::Void chart1_Click(System::Object^  sender, System::EventArgs^  e) {
           }
  };
