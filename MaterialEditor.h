#pragma once

namespace XPlanGMLToolbox {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für MaterialEditor
	/// </summary>
	public ref class MaterialEditor : public System::Windows::Forms::Form
	{
	public:
		MaterialEditor(void)
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
		~MaterialEditor()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::ComboBox^  comboBoxMaterial;
  protected: 
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Button^  buttonNeu;
  private: System::Windows::Forms::Button^  buttonDuplizieren;
  private: System::Windows::Forms::Button^  buttonLoeschen;

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
      this->comboBoxMaterial = (gcnew System::Windows::Forms::ComboBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->buttonNeu = (gcnew System::Windows::Forms::Button());
      this->buttonDuplizieren = (gcnew System::Windows::Forms::Button());
      this->buttonLoeschen = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // comboBoxMaterial
      // 
      this->comboBoxMaterial->FormattingEnabled = true;
      this->comboBoxMaterial->Location = System::Drawing::Point(13, 30);
      this->comboBoxMaterial->Name = L"comboBoxMaterial";
      this->comboBoxMaterial->Size = System::Drawing::Size(216, 21);
      this->comboBoxMaterial->TabIndex = 0;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(13, 58);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(44, 13);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Material";
      // 
      // buttonNeu
      // 
      this->buttonNeu->Location = System::Drawing::Point(235, 28);
      this->buttonNeu->Name = L"buttonNeu";
      this->buttonNeu->Size = System::Drawing::Size(48, 23);
      this->buttonNeu->TabIndex = 2;
      this->buttonNeu->Text = L"Neu";
      this->buttonNeu->UseVisualStyleBackColor = true;
      // 
      // buttonDuplizieren
      // 
      this->buttonDuplizieren->Location = System::Drawing::Point(294, 30);
      this->buttonDuplizieren->Name = L"buttonDuplizieren";
      this->buttonDuplizieren->Size = System::Drawing::Size(75, 23);
      this->buttonDuplizieren->TabIndex = 3;
      this->buttonDuplizieren->Text = L"Duplizieren";
      this->buttonDuplizieren->UseVisualStyleBackColor = true;
      // 
      // buttonLoeschen
      // 
      this->buttonLoeschen->Location = System::Drawing::Point(385, 30);
      this->buttonLoeschen->Name = L"buttonLoeschen";
      this->buttonLoeschen->Size = System::Drawing::Size(75, 23);
      this->buttonLoeschen->TabIndex = 4;
      this->buttonLoeschen->Text = L"Löschen";
      this->buttonLoeschen->UseVisualStyleBackColor = true;
      // 
      // MaterialEditor
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(481, 262);
      this->Controls->Add(this->buttonLoeschen);
      this->Controls->Add(this->buttonDuplizieren);
      this->Controls->Add(this->buttonNeu);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->comboBoxMaterial);
      this->Name = L"MaterialEditor";
      this->Text = L"Material Editor";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
	};
}
