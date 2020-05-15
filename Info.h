#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

	/// <summary> 
	/// Anzeige von Informationen über die Software
	/// </summary>
	public ref class Info : public System::Windows::Forms::Form
	{
	public: 
		Info(void)
		{
			String ^ text;

			InitializeComponent();
			
			text = "1. GEWÄHRLEISTUNGSAUSSCHLUSS\r\n";
			text = String::Concat ( text, "Das KIT übernimmt keine Gewährleistung für die Software, " );
			text = String::Concat ( text, "Daten und Dokumentationen, insbesondere nicht für Richtigkeit und Einsetzbarkeit " );
			text = String::Concat ( text, "des Inhalts oder die Freiheit von Rechten Dritter, sowie dafür, dass der Zugriff " );
			text = String::Concat ( text, "darauf verlässlich, virus- und fehlerfrei möglich ist.\r\n\r\n" );

			text = String::Concat ( text, "2. HAFTUNGSAUSSCHLUSS\r\n" );
			text = String::Concat ( text, "Das KIT haftet nur für Schäden, die auf einer vorsätzlichen oder " );
			text = String::Concat ( text, "grobfahrlässigen Pflichtverletzung durch das KIT, seine " );
			text = String::Concat ( text, "gesetzlichen Vertreter oder Erfüllungsgehilfen beruhen. Dies gilt auch für " );
			text = String::Concat ( text, "Schäden aus der Verletzung von Pflichten bei Vertragsverhandlungen sowie aus der " );
			text = String::Concat ( text, "Vornahme von unerlaubten Handlungen.\r\n\r\n" );
			text = String::Concat ( text, "Für Schäden, die auf einer fahrlässigen Verletzung von vertraglichen oder " );
			text = String::Concat ( text, "vorvertraglichen Pflichten durch das KIT, seine gesetzlichen " );
			text = String::Concat ( text, "Vertreter oder Erfüllungsgehilfen beruhen, ist die Haftung für mittelbare " );
			text = String::Concat ( text, "Schäden und Folgeschäden ausgeschlossen.\r\n\r\n" );
			text = String::Concat ( text, "Die zwingende Haftung nach dem Produkthaftungsgesetz bleibt von den vorstehenden " );
			text = String::Concat ( text, "Regelungen unberührt.\r\n\r\n" );

			text = String::Concat ( text, "3. SCHUTZRECHTE\r\n" );
			text = String::Concat ( text, "Für die Software, Daten und Dokumentationen behält sich das KIT " );
			text = String::Concat ( text, "alle Rechte in vollem Umfange vor, insbesondere Eigentum, Urheberrecht sowie " );
			text = String::Concat ( text, "Erwirkung in- und ausländischer Schutzrechte.\r\n" );

			textBoxHaftungsAusschluss->Text = text;

			text = "Kontakt:\r\n\r\n";
			text = String::Concat ( text, "     Dr. Joachim Benner\r\n" );
			text = String::Concat ( text, "     Karlsruher Institut für Technologie (KIT)\r\n" );
			text = String::Concat ( text, "     Institut für Automation und Angewandte Informatik\r\n" );
			text = String::Concat ( text, "     Tel.: 0721-608 22534\r\n" );
			text = String::Concat ( text, "     EMail: Joachim.Benner@kit.edu" );

			textBoxKontakt->Text = text;
		}
        
	protected: 

	private: System::Windows::Forms::PictureBox ^  pictureBoxXPlanungLogo;
	private: System::Windows::Forms::PictureBox ^  pictureBoxFZKLogo;
	private: System::Windows::Forms::Label ^  label1;
	private: System::Windows::Forms::Label ^  label2;
	private: System::Windows::Forms::TextBox ^  textBoxHaftungsAusschluss;
	private: System::Windows::Forms::Button ^  button1;
	private: System::Windows::Forms::TextBox ^  textBoxKontakt;
	private: System::Windows::Forms::Label ^  label3;
	private: System::Windows::Forms::Label ^  label4;
	private: System::Windows::Forms::Label ^  label5;
	private: System::Windows::Forms::GroupBox ^  groupBox1;
  private: System::Windows::Forms::CheckBox^  checkBoxTest;



	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container^ components;

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Info::typeid));
      this->pictureBoxXPlanungLogo = (gcnew System::Windows::Forms::PictureBox());
      this->pictureBoxFZKLogo = (gcnew System::Windows::Forms::PictureBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->textBoxHaftungsAusschluss = (gcnew System::Windows::Forms::TextBox());
      this->button1 = (gcnew System::Windows::Forms::Button());
      this->textBoxKontakt = (gcnew System::Windows::Forms::TextBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->label5 = (gcnew System::Windows::Forms::Label());
      this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxXPlanungLogo))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxFZKLogo))->BeginInit();
      this->groupBox1->SuspendLayout();
      this->SuspendLayout();
      // 
      // pictureBoxXPlanungLogo
      // 
      this->pictureBoxXPlanungLogo->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBoxXPlanungLogo.BackgroundImage")));
      this->pictureBoxXPlanungLogo->ErrorImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBoxXPlanungLogo.ErrorImage")));
      this->pictureBoxXPlanungLogo->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBoxXPlanungLogo.Image")));
      this->pictureBoxXPlanungLogo->InitialImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBoxXPlanungLogo.InitialImage")));
      this->pictureBoxXPlanungLogo->Location = System::Drawing::Point(10, 14);
      this->pictureBoxXPlanungLogo->Name = L"pictureBoxXPlanungLogo";
      this->pictureBoxXPlanungLogo->Size = System::Drawing::Size(421, 135);
      this->pictureBoxXPlanungLogo->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
      this->pictureBoxXPlanungLogo->TabIndex = 0;
      this->pictureBoxXPlanungLogo->TabStop = false;
      // 
      // pictureBoxFZKLogo
      // 
      this->pictureBoxFZKLogo->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBoxFZKLogo.Image")));
      this->pictureBoxFZKLogo->Location = System::Drawing::Point(310, 309);
      this->pictureBoxFZKLogo->Name = L"pictureBoxFZKLogo";
      this->pictureBoxFZKLogo->Size = System::Drawing::Size(116, 101);
      this->pictureBoxFZKLogo->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
      this->pictureBoxFZKLogo->TabIndex = 1;
      this->pictureBoxFZKLogo->TabStop = false;
      // 
      // label1
      // 
      this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->label1->Location = System::Drawing::Point(7, 24);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(175, 19);
      this->label1->TabIndex = 2;
      this->label1->Text = L"GML-Toolbox ";
      // 
      // label2
      // 
      this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->label2->Location = System::Drawing::Point(10, 57);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(175, 18);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Version 11.1";
      // 
      // textBoxHaftungsAusschluss
      // 
      this->textBoxHaftungsAusschluss->Location = System::Drawing::Point(10, 427);
      this->textBoxHaftungsAusschluss->Multiline = true;
      this->textBoxHaftungsAusschluss->Name = L"textBoxHaftungsAusschluss";
      this->textBoxHaftungsAusschluss->ReadOnly = true;
      this->textBoxHaftungsAusschluss->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
      this->textBoxHaftungsAusschluss->Size = System::Drawing::Size(416, 196);
      this->textBoxHaftungsAusschluss->TabIndex = 4;
      // 
      // button1
      // 
      this->button1->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->button1->Location = System::Drawing::Point(174, 643);
      this->button1->Name = L"button1";
      this->button1->Size = System::Drawing::Size(90, 26);
      this->button1->TabIndex = 6;
      this->button1->Text = L"Ok";
      // 
      // textBoxKontakt
      // 
      this->textBoxKontakt->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
      this->textBoxKontakt->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->textBoxKontakt->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
        static_cast<System::Int32>(static_cast<System::Byte>(192)));
      this->textBoxKontakt->Location = System::Drawing::Point(10, 298);
      this->textBoxKontakt->Multiline = true;
      this->textBoxKontakt->Name = L"textBoxKontakt";
      this->textBoxKontakt->ReadOnly = true;
      this->textBoxKontakt->Size = System::Drawing::Size(416, 122);
      this->textBoxKontakt->TabIndex = 7;
      // 
      // label3
      // 
      this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->label3->ForeColor = System::Drawing::Color::Blue;
      this->label3->Location = System::Drawing::Point(193, 27);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(185, 26);
      this->label3->TabIndex = 8;
      this->label3->Text = L"Unterstützte Standards";
      // 
      // label4
      // 
      this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->label4->Location = System::Drawing::Point(10, 84);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(134, 19);
      this->label4->TabIndex = 9;
      this->label4->Text = L"20. April 2020";
      // 
      // label5
      // 
      this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
      this->label5->ForeColor = System::Drawing::SystemColors::ActiveCaption;
      this->label5->Location = System::Drawing::Point(193, 57);
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size(203, 56);
      this->label5->TabIndex = 10;
      this->label5->Text = L"XPlanung,  INSPIRE, CityGML, CityGML-ADE, ALKIS/NAS, BoreholeML";
      this->label5->Click += gcnew System::EventHandler(this, &Info::label5_Click);
      // 
      // groupBox1
      // 
      this->groupBox1->Controls->Add(this->label4);
      this->groupBox1->Controls->Add(this->label3);
      this->groupBox1->Controls->Add(this->label5);
      this->groupBox1->Controls->Add(this->label1);
      this->groupBox1->Controls->Add(this->label2);
      this->groupBox1->Location = System::Drawing::Point(10, 156);
      this->groupBox1->Name = L"groupBox1";
      this->groupBox1->Size = System::Drawing::Size(416, 135);
      this->groupBox1->TabIndex = 12;
      this->groupBox1->TabStop = false;
      // 
      // Info
      // 
      this->AcceptButton = this->button1;
      this->AutoScaleBaseSize = System::Drawing::Size(6, 15);
      this->CancelButton = this->button1;
      this->ClientSize = System::Drawing::Size(444, 678);
      this->Controls->Add(this->button1);
      this->Controls->Add(this->textBoxHaftungsAusschluss);
      this->Controls->Add(this->pictureBoxFZKLogo);
      this->Controls->Add(this->pictureBoxXPlanungLogo);
      this->Controls->Add(this->groupBox1);
      this->Controls->Add(this->textBoxKontakt);
      this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"Info";
      this->Text = L"Info über GML Toolbox";
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxXPlanungLogo))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxFZKLogo))->EndInit();
      this->groupBox1->ResumeLayout(false);
      this->ResumeLayout(false);
      this->PerformLayout();

    }
private: System::Void label5_Click ( System::Object^  sender, System::EventArgs^  e )
{
}
};
