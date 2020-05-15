#pragma once

namespace XPlanGMLToolbox {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für LayerMaterialADE
	/// </summary>
	public ref class LayerEditor : public System::Windows::Forms::Form
	{
	public:
		LayerEditor(void)
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
		~LayerEditor()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxSchichtdicke;
  protected: 
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::ComboBox^  comboBoxMaterial;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Button^  buttonMaterialEditor;
  private: System::Windows::Forms::GroupBox^  groupBox1;
  private: System::Windows::Forms::Label^  label6;
  private: System::Windows::Forms::TextBox^  textBoxSüezWaerme;
  private: System::Windows::Forms::Label^  label5;
  private: System::Windows::Forms::TextBox^  textBoxMueWert;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::TextBox^  textBox1;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::TextBox^  textBoxLamda;
  private: System::Windows::Forms::Button^  buttonAbbrechen;
  private: System::Windows::Forms::Button^  buttonUebernehmen;

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
      this->textBoxSchichtdicke = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->comboBoxMaterial = (gcnew System::Windows::Forms::ComboBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->buttonMaterialEditor = (gcnew System::Windows::Forms::Button());
      this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
      this->textBoxLamda = (gcnew System::Windows::Forms::TextBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->textBox1 = (gcnew System::Windows::Forms::TextBox());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->textBoxMueWert = (gcnew System::Windows::Forms::TextBox());
      this->label5 = (gcnew System::Windows::Forms::Label());
      this->textBoxSüezWaerme = (gcnew System::Windows::Forms::TextBox());
      this->label6 = (gcnew System::Windows::Forms::Label());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->buttonUebernehmen = (gcnew System::Windows::Forms::Button());
      this->groupBox1->SuspendLayout();
      this->SuspendLayout();
      // 
      // textBoxSchichtdicke
      // 
      this->textBoxSchichtdicke->Location = System::Drawing::Point(13, 32);
      this->textBoxSchichtdicke->Name = L"textBoxSchichtdicke";
      this->textBoxSchichtdicke->Size = System::Drawing::Size(100, 20);
      this->textBoxSchichtdicke->TabIndex = 0;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(115, 38);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(69, 13);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Schichtdicke";
      // 
      // comboBoxMaterial
      // 
      this->comboBoxMaterial->FormattingEnabled = true;
      this->comboBoxMaterial->Location = System::Drawing::Point(12, 72);
      this->comboBoxMaterial->Name = L"comboBoxMaterial";
      this->comboBoxMaterial->Size = System::Drawing::Size(186, 21);
      this->comboBoxMaterial->TabIndex = 2;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(12, 96);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(44, 13);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Material";
      // 
      // buttonMaterialEditor
      // 
      this->buttonMaterialEditor->Location = System::Drawing::Point(214, 72);
      this->buttonMaterialEditor->Name = L"buttonMaterialEditor";
      this->buttonMaterialEditor->Size = System::Drawing::Size(75, 23);
      this->buttonMaterialEditor->TabIndex = 4;
      this->buttonMaterialEditor->Text = L"Editor";
      this->buttonMaterialEditor->UseVisualStyleBackColor = true;
      // 
      // groupBox1
      // 
      this->groupBox1->Controls->Add(this->label6);
      this->groupBox1->Controls->Add(this->textBoxSüezWaerme);
      this->groupBox1->Controls->Add(this->label5);
      this->groupBox1->Controls->Add(this->textBoxMueWert);
      this->groupBox1->Controls->Add(this->label4);
      this->groupBox1->Controls->Add(this->textBox1);
      this->groupBox1->Controls->Add(this->label3);
      this->groupBox1->Controls->Add(this->textBoxLamda);
      this->groupBox1->Location = System::Drawing::Point(13, 139);
      this->groupBox1->Name = L"groupBox1";
      this->groupBox1->Size = System::Drawing::Size(370, 120);
      this->groupBox1->TabIndex = 5;
      this->groupBox1->TabStop = false;
      this->groupBox1->Text = L"Material-Parameter";
      // 
      // textBoxLamda
      // 
      this->textBoxLamda->Location = System::Drawing::Point(7, 29);
      this->textBoxLamda->Name = L"textBoxLamda";
      this->textBoxLamda->Size = System::Drawing::Size(54, 20);
      this->textBoxLamda->TabIndex = 0;
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(64, 34);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(82, 13);
      this->label3->TabIndex = 1;
      this->label3->Text = L"λ-Wert [W/m*K]";
      // 
      // textBox1
      // 
      this->textBox1->Location = System::Drawing::Point(171, 29);
      this->textBox1->Name = L"textBox1";
      this->textBox1->Size = System::Drawing::Size(54, 20);
      this->textBox1->TabIndex = 2;
      // 
      // label4
      // 
      this->label4->AutoSize = true;
      this->label4->Location = System::Drawing::Point(227, 34);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(75, 13);
      this->label4->TabIndex = 3;
      this->label4->Text = L"Dichte [kg/m³]";
      // 
      // textBoxMueWert
      // 
      this->textBoxMueWert->Location = System::Drawing::Point(7, 87);
      this->textBoxMueWert->Name = L"textBoxMueWert";
      this->textBoxMueWert->Size = System::Drawing::Size(54, 20);
      this->textBoxMueWert->TabIndex = 4;
      // 
      // label5
      // 
      this->label5->AutoSize = true;
      this->label5->Location = System::Drawing::Point(67, 93);
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size(39, 13);
      this->label5->TabIndex = 5;
      this->label5->Text = L"µ-Wert";
      // 
      // textBoxSüezWaerme
      // 
      this->textBoxSüezWaerme->Location = System::Drawing::Point(171, 87);
      this->textBoxSüezWaerme->Name = L"textBoxSüezWaerme";
      this->textBoxSüezWaerme->Size = System::Drawing::Size(54, 20);
      this->textBoxSüezWaerme->TabIndex = 6;
      // 
      // label6
      // 
      this->label6->AutoSize = true;
      this->label6->Location = System::Drawing::Point(232, 93);
      this->label6->Name = L"label6";
      this->label6->Size = System::Drawing::Size(114, 13);
      this->label6->TabIndex = 7;
      this->label6->Text = L"Spez. Wärme [J/Kg*K]";
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->Location = System::Drawing::Point(15, 295);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 23);
      this->buttonAbbrechen->TabIndex = 6;
      this->buttonAbbrechen->Text = L"Abbrechen";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      // 
      // buttonUebernehmen
      // 
      this->buttonUebernehmen->Location = System::Drawing::Point(304, 294);
      this->buttonUebernehmen->Name = L"buttonUebernehmen";
      this->buttonUebernehmen->Size = System::Drawing::Size(78, 23);
      this->buttonUebernehmen->TabIndex = 7;
      this->buttonUebernehmen->Text = L"Übernehmen";
      this->buttonUebernehmen->UseVisualStyleBackColor = true;
      // 
      // LayerMaterialADE
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(401, 336);
      this->Controls->Add(this->buttonUebernehmen);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->groupBox1);
      this->Controls->Add(this->buttonMaterialEditor);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->comboBoxMaterial);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->textBoxSchichtdicke);
      this->MaximizeBox = false;
      this->Name = L"LayerMaterialADE";
      this->Text = L"Layer Editor";
      this->groupBox1->ResumeLayout(false);
      this->groupBox1->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
	};
}
