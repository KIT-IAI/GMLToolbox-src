#pragma once



namespace BPlanGMLViewer
{
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
//using namespace System::Drawing;
	/// <summary> 
	/// Zusammenfassung für Options
	///
	/// Achtung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie die Eigenschaft
	///          'Ressourcendateiname' für das Compilertool für verwaltete Ressourcen ändern, 
	///          das allen .resx-Dateien zugewiesen ist, von denen diese Klasse abhängt. 
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class Options : public System::Windows::Forms::Form
	{
	public: 
		Options(void)
		{
			InitializeComponent();
		}


	public: System::Windows::Forms::TextBox ^  SchemaVerzeichnis;



	private: System::Windows::Forms::Label ^  label2;
	private: System::Windows::Forms::Button ^  buttonSVGVerzeichnis;
	private: System::Windows::Forms::Button ^  buttonOK;

	public: System::Windows::Forms::TextBox ^  SystemVerzeichnis;
	private: System::Windows::Forms::FolderBrowserDialog ^  SystemVerzeichnisDialog;
	private: System::Windows::Forms::Button ^  buttonCancel;

	protected: 
/*		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
*/
	private: System::Windows::Forms::Button ^  buttonSchemaVerzeichnis;
	private: System::Windows::Forms::Label ^  label8;
	private: System::Windows::Forms::FolderBrowserDialog ^  SchemaVerzeichnisDialog;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^ components;

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->buttonSchemaVerzeichnis = gcnew System::Windows::Forms::Button();
			this->SchemaVerzeichnis = gcnew System::Windows::Forms::TextBox();
			this->label8 = gcnew System::Windows::Forms::Label();
			this->SchemaVerzeichnisDialog = gcnew System::Windows::Forms::FolderBrowserDialog();
			this->SystemVerzeichnis = gcnew System::Windows::Forms::TextBox();
			this->label2 = gcnew System::Windows::Forms::Label();
			this->buttonSVGVerzeichnis = gcnew System::Windows::Forms::Button();
			this->buttonOK = gcnew System::Windows::Forms::Button();
			this->buttonCancel = gcnew System::Windows::Forms::Button();
			this->SystemVerzeichnisDialog = gcnew System::Windows::Forms::FolderBrowserDialog();
			this->SuspendLayout();
			// 
			// buttonSchemaVerzeichnis
			// 
			this->buttonSchemaVerzeichnis->Location = System::Drawing::Point(232, 56);
			this->buttonSchemaVerzeichnis->Name = "buttonSchemaVerzeichnis";
			this->buttonSchemaVerzeichnis->Size = System::Drawing::Size(64, 20);
			this->buttonSchemaVerzeichnis->TabIndex = 38;
			this->buttonSchemaVerzeichnis->Text = "Ändern";
      this->buttonSchemaVerzeichnis->Click += gcnew System::EventHandler(this, &Options::buttonSchemaVerzeichnis_Click);
			// 
			// SchemaVerzeichnis
			// 
			this->SchemaVerzeichnis->Location = System::Drawing::Point(16, 32);
			this->SchemaVerzeichnis->Name = "SchemaVerzeichnis";
			this->SchemaVerzeichnis->ReadOnly = true;
			this->SchemaVerzeichnis->Size = System::Drawing::Size(280, 20);
			this->SchemaVerzeichnis->TabIndex = 36;
			this->SchemaVerzeichnis->Text = "<Schema Verzeichnis>";
			// 
			// label8
			// 
			this->label8->Location = System::Drawing::Point(16, 56);
			this->label8->Name = "label8";
			this->label8->Size = System::Drawing::Size(248, 16);
			this->label8->TabIndex = 37;
			this->label8->Text = "BPlanGML Schema-Verzeichnis";
			// 
			// SchemaVerzeichnisDialog
			// 
			this->SchemaVerzeichnisDialog->ShowNewFolderButton = false;
			// 
			// SystemVerzeichnis
			// 
			this->SystemVerzeichnis->Location = System::Drawing::Point(16, 96);
			this->SystemVerzeichnis->Name = "SystemVerzeichnis";
			this->SystemVerzeichnis->ReadOnly = true;
			this->SystemVerzeichnis->Size = System::Drawing::Size(280, 20);
			this->SystemVerzeichnis->TabIndex = 43;
			this->SystemVerzeichnis->Text = "";
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(16, 120);
			this->label2->Name = "label2";
			this->label2->Size = System::Drawing::Size(200, 16);
			this->label2->TabIndex = 44;
			this->label2->Text = "System-Verzeichnis";
			// 
			// buttonSVGVerzeichnis
			// 
			this->buttonSVGVerzeichnis->Location = System::Drawing::Point(232, 120);
			this->buttonSVGVerzeichnis->Name = "buttonSVGVerzeichnis";
			this->buttonSVGVerzeichnis->Size = System::Drawing::Size(64, 20);
			this->buttonSVGVerzeichnis->TabIndex = 45;
			this->buttonSVGVerzeichnis->Text = "Ändern";
			this->buttonSVGVerzeichnis->Click += gcnew System::EventHandler(this, &Options::buttonSVGVerzeichnis_Click);
			// 
			// buttonOK
			// 
			this->buttonOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->buttonOK->Location = System::Drawing::Point(224, 160);
			this->buttonOK->Name = "buttonOK";
			this->buttonOK->TabIndex = 46;
			this->buttonOK->Text = "OK";
			this->buttonOK->Click += gcnew System::EventHandler(this, &Options::OK_Click);
			// 
			// buttonCancel
			// 
			this->buttonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->buttonCancel->Location = System::Drawing::Point(120, 160);
			this->buttonCancel->Name = "buttonCancel";
			this->buttonCancel->TabIndex = 47;
			this->buttonCancel->Text = "Cancel";
			this->buttonCancel->Click += gcnew System::EventHandler(this, &Options::buttonCancel_Click);
			// 
			// SystemVerzeichnisDialog
			// 
			this->SystemVerzeichnisDialog->ShowNewFolderButton = false;
			// 
			// Options
			// 
			this->AcceptButton = this->buttonOK;
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->CancelButton = this->buttonCancel;
			this->ClientSize = System::Drawing::Size(312, 197);
			this->Controls->Add(this->buttonCancel);
			this->Controls->Add(this->buttonOK);
			this->Controls->Add(this->buttonSVGVerzeichnis);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->SystemVerzeichnis);
			this->Controls->Add(this->buttonSchemaVerzeichnis);
			this->Controls->Add(this->SchemaVerzeichnis);
			this->Controls->Add(this->label8);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = "Options";
			this->Text = "Options";
			this->ResumeLayout(false);

		}		

private: System::Void buttonSchemaVerzeichnis_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	System::String ^ folderName;

	SchemaVerzeichnisDialog->SelectedPath = SchemaVerzeichnis->Text;

  if ( SchemaVerzeichnisDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )
	 {
		folderName = SchemaVerzeichnisDialog->SelectedPath;
		SchemaVerzeichnis->Text = folderName;
	 }

}

private: System::Void buttonSVGVerzeichnis_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	System::String ^ folderName;

	SystemVerzeichnisDialog->SelectedPath = SystemVerzeichnis->Text;
  if ( SystemVerzeichnisDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )
	 {
		folderName = SystemVerzeichnisDialog->SelectedPath;
		SystemVerzeichnis->Text = folderName;
	 }
}

private: System::Void OK_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	Close();
}

private: System::Void buttonCancel_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	Close();
}

};
}
