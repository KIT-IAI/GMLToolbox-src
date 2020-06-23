#pragma once

#include "Dictionary.h"
#include "Convert.h"

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für INSPIRECodeList
	/// </summary>
	public ref class INSPIRECodeList : public System::Windows::Forms::Form
	{
	public:
		INSPIRECodeList(void)
		{
			InitializeComponent();

      pCodelist = NULL;
      radioButtonINSPIRE->Checked = true;
      initialize();
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~INSPIRECodeList()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxURL;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Button^  buttonEinlesen;
  private: System::Windows::Forms::DataGridView^  dataGridViewCodes;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::TextBox^  textBoxXML;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::TextBox^  textBoxFehler;
  private: System::Windows::Forms::RadioButton^  radioButtonDictionary;
  private: System::Windows::Forms::RadioButton^  radioButtonINSPIRE;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

    GmlDictionaryEntry                 * pCodelist;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->textBoxURL = ( gcnew System::Windows::Forms::TextBox() );
      this->label1 = ( gcnew System::Windows::Forms::Label() );
      this->buttonEinlesen = ( gcnew System::Windows::Forms::Button() );
      this->dataGridViewCodes = ( gcnew System::Windows::Forms::DataGridView() );
      this->buttonBeenden = ( gcnew System::Windows::Forms::Button() );
      this->textBoxXML = ( gcnew System::Windows::Forms::TextBox() );
      this->label2 = ( gcnew System::Windows::Forms::Label() );
      this->textBoxFehler = ( gcnew System::Windows::Forms::TextBox() );
      this->radioButtonDictionary = ( gcnew System::Windows::Forms::RadioButton() );
      this->radioButtonINSPIRE = ( gcnew System::Windows::Forms::RadioButton() );
      ( cli::safe_cast<System::ComponentModel::ISupportInitialize^>( this->dataGridViewCodes ) )->BeginInit();
      this->SuspendLayout();
      // 
      // textBoxURL
      // 
      this->textBoxURL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left )
        | System::Windows::Forms::AnchorStyles::Right ) );
      this->textBoxURL->Location = System::Drawing::Point( 13, 13 );
      this->textBoxURL->Name = L"textBoxURL";
      this->textBoxURL->Size = System::Drawing::Size( 529, 20 );
      this->textBoxURL->TabIndex = 0;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point( 13, 36 );
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size( 69, 13 );
      this->label1->TabIndex = 1;
      this->label1->Text = L"Codelist URL";
      // 
      // buttonEinlesen
      // 
      this->buttonEinlesen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right ) );
      this->buttonEinlesen->Location = System::Drawing::Point( 467, 36 );
      this->buttonEinlesen->Name = L"buttonEinlesen";
      this->buttonEinlesen->Size = System::Drawing::Size( 75, 23 );
      this->buttonEinlesen->TabIndex = 2;
      this->buttonEinlesen->Text = L"Einlesen";
      this->buttonEinlesen->UseVisualStyleBackColor = true;
      this->buttonEinlesen->Click += gcnew System::EventHandler( this, &INSPIRECodeList::buttonEinlesen_Click );
      // 
      // dataGridViewCodes
      // 
      this->dataGridViewCodes->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( ( System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom )
        | System::Windows::Forms::AnchorStyles::Left )
        | System::Windows::Forms::AnchorStyles::Right ) );
      this->dataGridViewCodes->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
      this->dataGridViewCodes->Location = System::Drawing::Point( 0, 137 );
      this->dataGridViewCodes->Name = L"dataGridViewCodes";
      this->dataGridViewCodes->Size = System::Drawing::Size( 555, 172 );
      this->dataGridViewCodes->TabIndex = 3;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left ) );
      this->buttonBeenden->Location = System::Drawing::Point( 170, 325 );
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size( 75, 23 );
      this->buttonBeenden->TabIndex = 4;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler( this, &INSPIRECodeList::buttonBeenden_Click );
      // 
      // textBoxXML
      // 
      this->textBoxXML->Anchor = static_cast<System::Windows::Forms::AnchorStyles>( ( ( System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left )
        | System::Windows::Forms::AnchorStyles::Right ) );
      this->textBoxXML->Location = System::Drawing::Point( 13, 66 );
      this->textBoxXML->Name = L"textBoxXML";
      this->textBoxXML->Size = System::Drawing::Size( 529, 20 );
      this->textBoxXML->TabIndex = 5;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point( 13, 88 );
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size( 147, 13 );
      this->label2->TabIndex = 6;
      this->label2->Text = L"URL der XML-Repräsentation";
      // 
      // textBoxFehler
      // 
      this->textBoxFehler->ForeColor = System::Drawing::Color::Red;
      this->textBoxFehler->Location = System::Drawing::Point( 13, 111 );
      this->textBoxFehler->Name = L"textBoxFehler";
      this->textBoxFehler->ReadOnly = true;
      this->textBoxFehler->Size = System::Drawing::Size( 529, 20 );
      this->textBoxFehler->TabIndex = 7;
      // 
      // radioButtonDictionary
      // 
      this->radioButtonDictionary->AutoSize = true;
      this->radioButtonDictionary->Location = System::Drawing::Point( 89, 40 );
      this->radioButtonDictionary->Name = L"radioButtonDictionary";
      this->radioButtonDictionary->Size = System::Drawing::Size( 98, 17 );
      this->radioButtonDictionary->TabIndex = 8;
      this->radioButtonDictionary->TabStop = true;
      this->radioButtonDictionary->Text = L"GML-Dictionary";
      this->radioButtonDictionary->UseVisualStyleBackColor = true;
      // 
      // radioButtonINSPIRE
      // 
      this->radioButtonINSPIRE->AutoSize = true;
      this->radioButtonINSPIRE->Location = System::Drawing::Point( 229, 42 );
      this->radioButtonINSPIRE->Name = L"radioButtonINSPIRE";
      this->radioButtonINSPIRE->Size = System::Drawing::Size( 109, 17 );
      this->radioButtonINSPIRE->TabIndex = 9;
      this->radioButtonINSPIRE->TabStop = true;
      this->radioButtonINSPIRE->Text = L"INSPIRE Registry";
      this->radioButtonINSPIRE->UseVisualStyleBackColor = true;
      // 
      // INSPIRECodeList
      // 
      this->AcceptButton = this->buttonBeenden;
      this->AutoScaleDimensions = System::Drawing::SizeF( 6, 13 );
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size( 554, 360 );
      this->Controls->Add( this->radioButtonINSPIRE );
      this->Controls->Add( this->radioButtonDictionary );
      this->Controls->Add( this->textBoxFehler );
      this->Controls->Add( this->label2 );
      this->Controls->Add( this->textBoxXML );
      this->Controls->Add( this->buttonBeenden );
      this->Controls->Add( this->dataGridViewCodes );
      this->Controls->Add( this->buttonEinlesen );
      this->Controls->Add( this->label1 );
      this->Controls->Add( this->textBoxURL );
      this->Name = L"INSPIRECodeList";
      this->Text = L"Codelisten im INSPIRE Format";
      ( cli::safe_cast<System::ComponentModel::ISupportInitialize^>( this->dataGridViewCodes ) )->EndInit();
      this->ResumeLayout( false );
      this->PerformLayout();

    }
#pragma endregion
  private: System::Void buttonEinlesen_Click( System::Object^  sender, System::EventArgs^  e )
  {
    System::Net::WebRequest  ^ request;
    System::Net::WebResponse ^ response;
    std::string                registryURL;
    String                   ^ REGISTRY_URL;
    String                   ^ status;

    if ( pCodelist != NULL )
      delete pCodelist;
    pCodelist = NULL;

    if ( radioButtonINSPIRE->Checked )
    {
      if ( textBoxURL->Text[textBoxURL->Text->Length - 1] == '/' )
        textBoxURL->Text = textBoxURL->Text->Remove ( textBoxURL->Text->Length - 1 );

      int index = textBoxURL->Text->LastIndexOf ( "/" );
      String ^ codelistName = textBoxURL->Text->Substring ( index + 1, textBoxURL->Text->Length - index - 1 );

      REGISTRY_URL = String::Concat ( textBoxURL->Text, "/", codelistName, ".de.xml" );
      textBoxXML->Text = REGISTRY_URL;
    }

    if ( radioButtonDictionary->Checked )
    {
      REGISTRY_URL = textBoxURL->Text;
      textBoxXML->Text = REGISTRY_URL;
    }

    try
    {
      System::Net::ServicePointManager::Expect100Continue = true;
      System::Net::ServicePointManager::SecurityProtocol = 
        System::Net::SecurityProtocolType::Tls
        | System::Net::SecurityProtocolType::Ssl3;

      request = System::Net::WebRequest::Create ( REGISTRY_URL );
      System::Net::HttpWebRequest ^ httpRequest = dynamic_cast<System::Net::HttpWebRequest^>( request );

      request->Timeout = -1;
      request->Credentials = System::Net::CredentialCache::DefaultNetworkCredentials;
      
      response = request->GetResponse();
      System::Net::HttpWebResponse ^ httpResponse = dynamic_cast<System::Net::HttpWebResponse^>( response );
      status = httpResponse->StatusDescription;
      textBoxFehler->Text = String::Concat ( "Status: ", status );

      if ( status == "OK" )
      {
        System::IO::Stream ^responseStream =  httpResponse->GetResponseStream();
        GmlDictionaryReader ^ pReader = gcnew GmlDictionaryReader();
        pCodelist = pReader->readINSPIRECodeList ( responseStream, true );
        if ( pCodelist != NULL )
          showCodelistEntries();
      } 
    }
    catch ( System::Exception ^ e )
    {
      textBoxFehler->Text =  e->Message;
    }

  }

private: System::Void buttonBeenden_Click( System::Object^  sender, System::EventArgs^  e )
  {
  if ( pCodelist != NULL )
    delete pCodelist;

    Close();
  }

private: System::Void initialize()
{
  dataGridViewCodes->ColumnCount = 3;
  dataGridViewCodes->ColumnHeadersVisible = true;

  dataGridViewCodes->DefaultCellStyle->WrapMode = DataGridViewTriState::True;

  DataGridViewCellStyle ^ columnHeaderStyle = gcnew DataGridViewCellStyle;
  columnHeaderStyle->BackColor = Color::Aqua;
  columnHeaderStyle->Font = gcnew System::Drawing::Font( "Verdana",8,FontStyle::Bold );
  columnHeaderStyle->WrapMode = DataGridViewTriState::True;

  dataGridViewCodes->ColumnHeadersDefaultCellStyle = columnHeaderStyle;
  dataGridViewCodes->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

  dataGridViewCodes->Columns[0]->Name = "Code";
  //dataGridViewCodes->Columns[0]->Width = 300;

  dataGridViewCodes->Columns[1]->Name = "Name";
  //dataGridViewCodes->Columns[1]->Width = 500;

  dataGridViewCodes->Columns[2]->Name = "Definition";
  //dataGridViewCodes->Columns[2]->Width = 500;
}

private: System::Void showCodelistEntries()
{
  DataGridViewRow          ^ row;
  DataGridViewTextBoxCell  ^ cell;

  std::vector<std::string>                  vSchluesselNummern;
  std::vector<GmlDictionaryEntryDefinition> vDictionaryEntryDefinitions;
  size_t                                    anz;

  anz = pCodelist->getCodelistEintraege ( vSchluesselNummern, vDictionaryEntryDefinitions );
  if ( anz == 0 )
    return;

  for ( size_t i = 0; i < anz; i++  )
  {
    std::string schluessel                  = vSchluesselNummern[i];
    GmlDictionaryEntryDefinition definition = vDictionaryEntryDefinitions[i];

    String ^ SCHLUESSEL =  QuConvert::ToString ( schluessel );
    int index = SCHLUESSEL->LastIndexOf ( "/" );
    String ^ CODE = SCHLUESSEL->Substring ( index + 1, SCHLUESSEL->Length - index - 1 );

    row = gcnew DataGridViewRow;

    //  1. Zelle
    cell = gcnew DataGridViewTextBoxCell;
    cell->Value    = CODE;
    row->Cells->Add( cell );
    cell->ReadOnly = true;

    //  2. Zelle
    cell = gcnew DataGridViewTextBoxCell;
    cell->Value    = QuConvert::ToString ( definition.name );
    row->Cells->Add( cell );
    cell->ReadOnly = true;


    //  3. Zelle
    cell = gcnew DataGridViewTextBoxCell;
    cell->Value    = QuConvert::ToString ( definition.beschreibung );
    row->Cells->Add( cell );
    cell->ReadOnly = true;

    dataGridViewCodes->Rows->Add ( row );
  
  }


}



};
