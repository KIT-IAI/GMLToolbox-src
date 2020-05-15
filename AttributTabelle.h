#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace XPlanGMLToolbox
{
	/// <summary> 
	/// Zusammenfassung für AttributTabelle
	///
	/// Achtung: Wenn Sie den Namen dieser Klasse ändern, müssen Sie die Eigenschaft
	///          'Ressourcendateiname' für das Compilertool für verwaltete Ressourcen ändern, 
	///          das allen .resx-Dateien zugewiesen ist, von denen diese Klasse abhängt. 
	///          Anderenfalls können die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class AttributTabelle : public System::Windows::Forms::Form
	{
	public: 
		AttributTabelle(void)
		{
			InitializeComponent();

			initTable();
		}


        
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
	private: System::Windows::Forms::DataGrid ^ dataGrid1;
			 System::Data::DataTable          ^ attributeTable;
	private: System::Windows::Forms::ComboBox ^  comboBox1;
	private: System::Windows::Forms::TextBox ^  textBox1;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^ components;

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>

	void initTable ()
	{
		String ^ STR;
		System::Data::DataColumn ^ pDataColums;

		attributeTable = gcnew DataTable ( "Attribute");

		STR = "Shp-File Name";
		pDataColums = attributeTable->Columns->Add (STR );
		pDataColums->DataType = STR->GetType();
		pDataColums->ReadOnly = true;
		pDataColums->MaxLength = 50;

		STR = "Datentyp";
		pDataColums = attributeTable->Columns->Add (STR );
		pDataColums->DataType = STR->GetType();
		pDataColums->ReadOnly = true;
		pDataColums->MaxLength = 20;

		STR = "GML-Attribut";
		pDataColums = attributeTable->Columns->Add ( STR );
		pDataColums->DataType =  STR->GetType();
		pDataColums->ReadOnly = false;
		pDataColums->MaxLength = 70;

		DataRow ^ pRow = attributeTable->NewRow();
		STR = "Bla Bla";
    pRow->ItemArray->SetValue ( = "Shp-File Name", STR  );
		STR =  "Integer";
		pRow->set_Item ( "Datentyp", STR );
		STR = "zweckbestimmung";
		pRow->set_Item ( "GML-Attribut", STR  );
		attributeTable->Rows->Add ( pRow );

		pRow = attributeTable->NewRow();
		STR = "Hallo";
		pRow->set_Item ( "Shp-File Name", STR  );
		STR =  "String";
		pRow->set_Item ( "Datentyp", STR );
		STR = "Test";
		pRow->set_Item ( "GML-Attribut", STR  );
		attributeTable->Rows->Add ( pRow );

		dataGrid1->set_DataSource (  attributeTable );
	}

		void InitializeComponent(void)
		{
			this->dataGrid1 = new System::Windows::Forms::DataGrid();
			this->comboBox1 = new System::Windows::Forms::ComboBox();
			this->textBox1 = new System::Windows::Forms::TextBox();
			(dynamic_cast<System::ComponentModel::ISupportInitialize ^  >(this->dataGrid1))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGrid1
			// 
			this->dataGrid1->AllowSorting = false;
			this->dataGrid1->Anchor = (System::Windows::Forms::AnchorStyles)(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right);
			this->dataGrid1->CaptionVisible = false;
			this->dataGrid1->DataMember = "";
			this->dataGrid1->HeaderForeColor = System::Drawing::SystemColors::ControlText;
			this->dataGrid1->Location = System::Drawing::Point(0, 136);
			this->dataGrid1->Name = "dataGrid1";
			this->dataGrid1->PreferredColumnWidth = 150;
			this->dataGrid1->ReadOnly = true;
			this->dataGrid1->Size = System::Drawing::Size(560, 512);
			this->dataGrid1->TabIndex = 0;
			this->dataGrid1->Navigate += new System::Windows::Forms::NavigateEventHandler(this, dataGrid1_Navigate);
			this->dataGrid1->CurrentCellChanged += new System::EventHandler(this, dataGrid1_CurrentCellChanged);
			// 
			// comboBox1
			// 
			System::Object* __mcTemp__1[] = new System::Object*[3];
			__mcTemp__1[0] = "A";
			__mcTemp__1[1] = "B";
			__mcTemp__1[2] = "C";
			this->comboBox1->Items->AddRange(__mcTemp__1);
			this->comboBox1->Location = System::Drawing::Point(96, 40);
			this->comboBox1->Name = "comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 21);
			this->comboBox1->TabIndex = 1;
			this->comboBox1->Text = "comboBox1";
			this->comboBox1->SelectedIndexChanged += new System::EventHandler(this, comboBox1_SelectedIndexChanged);
			this->comboBox1->DrawItem += new System::Windows::Forms::DrawItemEventHandler(this, comboBox1_DrawItem);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(96, 40);
			this->textBox1->Name = "textBox1";
			this->textBox1->Size = System::Drawing::Size(120, 20);
			this->textBox1->TabIndex = 2;
			this->textBox1->Text = "textBox1";
			// 
			// AttributTabelle
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(560, 573);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->dataGrid1);
			this->Name = "AttributTabelle";
			this->Text = "AttributTabelle";
			(dynamic_cast<System::ComponentModel::ISupportInitialize *  >(this->dataGrid1))->EndInit();
			this->ResumeLayout(false);

		}		
	private: System::Void dataGrid1_Navigate(System::Object ^  sender, System::Windows::Forms::NavigateEventArgs ^  ne)
			 {
			 }

private: System::Void dataGrid1_CurrentCellChanged(System::Object ^  sender, System::EventArgs ^  e)
{
	dataGrid1->get_CurrentCell();
	int row = dataGrid1->get_CurrentCell().get_RowNumber();
	int col = dataGrid1->get_CurrentCell().get_ColumnNumber();

	String * STR = "ha Ha Ha";
	dataGrid1->set_Item ( row, col, STR  );

	if (comboBox1->get_Visible() )
	{
		comboBox1->set_Visible ( false );
		textBox1->set_Visible ( true );
	}
	else
	{
		comboBox1->set_Visible ( true );
		textBox1->set_Visible ( false );
	}

}

private: System::Void comboBox1_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
		 {
			 int i = 1;
		 }

private: System::Void comboBox1_DrawItem(System::Object ^  sender, System::Windows::Forms::DrawItemEventArgs ^  e)
		 {
			 int i = 1;
		 }

};
}
