#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace BPlanGMLViewer
{
	/// <summary> 
	/// Zusammenfassung für DatumsDialog
	///
	/// Achtung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie die Eigenschaft
	///          'Ressourcendateiname' für das Compilertool für verwaltete Ressourcen ändern, 
	///          das allen .resx-Dateien zugewiesen ist, von denen diese Klasse abhängt. 
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public __gc class DatumsDialog : public System::Windows::Forms::Form
	{
	public: 
		DatumsDialog(void)
		{
			InitializeComponent();
		}
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}

	private: System::Windows::Forms::Button *  buttonUebernehmen;
	private: System::Windows::Forms::DateTimePicker *  dateTimePicker1;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container* components;

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->buttonUebernehmen = new System::Windows::Forms::Button();
			this->dateTimePicker1 = new System::Windows::Forms::DateTimePicker();
			this->SuspendLayout();
			// 
			// buttonUebernehmen
			// 
			this->buttonUebernehmen->Location = System::Drawing::Point(48, 160);
			this->buttonUebernehmen->Name = S"buttonUebernehmen";
			this->buttonUebernehmen->TabIndex = 1;
			this->buttonUebernehmen->Text = S"Übernehmen";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->CustomFormat = S"yyyy-MM-d";
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker1->Location = System::Drawing::Point(8, 16);
			this->dateTimePicker1->Name = S"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(80, 20);
			this->dateTimePicker1->TabIndex = 2;
			// 
			// DatumsDialog
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(496, 181);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->buttonUebernehmen);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = S"DatumsDialog";
			this->Text = S"DatumsDialog";
			this->ResumeLayout(false);

		}		
	};
}
