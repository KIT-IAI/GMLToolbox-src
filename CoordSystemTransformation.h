#pragma once

#include "SrsManagerRef.h"
#include "Feature.h"
#include "Convert.h"

#include <vector>
#include <string>


	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

  using namespace std;

	/// <summary>
	/// Dialog für Koordinaten-Transformationen
	/// </summary>
	public ref class CoordSystemTransformation : public System::Windows::Forms::Form
	{
	public:
		CoordSystemTransformation( Features * pFeatures, String ^ quellDatei, String ^ srsSystemNew )
		{
      vector<CoordSystem*> vCoordSystems;
      string               srsNameNeu;

			InitializeComponent();
		
      m_pFeatures           = pFeatures;
      m_pSrsManager         = pFeatures->getSrsManager();
      abfrageUeberschreiben = true;
      QuConvert::systemStr2stdStr( srsNameNeu, srsSystemNew );

      pCoordSystems = gcnew  System::Collections::SortedList;

     if ( quellDatei->CompareTo( "" ) != 0 )
      {
        textBoxQuelle->Text = quellDatei;
        int index = textBoxQuelle->Text->LastIndexOf( "\\");
        m_quellOrdner = textBoxQuelle->Text->Substring( 0, index );
        textBoxZiel->Text = m_quellOrdner;
      }
      else
      {
        textBoxQuelle->Text         = "";
        textBoxZiel->Text     = "";
      }

		 size_t anz   = m_pSrsManager->getCoordSystems( vCoordSystems );
     int index = -1;
     for ( size_t  i = 0; i < anz; i++ )
     {
       string srsNameShort    = vCoordSystems[i]->getSrsNameShort();
       string srsNameFull     = vCoordSystems[i]->getSrsNameFull();
       String ^ SRS_NAME_FULL = QuConvert::ToString( srsNameFull );

       string srsNameBeschreibung = vCoordSystems[i]->getDescription();
       srsNameBeschreibung = srsNameBeschreibung + " (" + srsNameFull + ")";     
       String ^ STR = QuConvert::ToString ( srsNameBeschreibung );

       comboBoxZielsystem->Items->Add ( STR );
       pCoordSystems->Add( STR, SRS_NAME_FULL );
       if ( srsNameNeu == srsNameFull ) index = (int)i;
     }

     comboBoxZielsystem->SelectedIndex = index;


     checkBoxUeberschreiben->Checked = false;
     radioButtonDatei->Checked = true;
     radioButtonOrdner->Checked = false;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~CoordSystemTransformation()
		{
			if (components)
			{
				delete components;
			}
		}

  protected: 
  private: System::Windows::Forms::ComboBox^  comboBoxZielsystem;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::GroupBox^  groupBoxQuelle;
  private: System::Windows::Forms::TextBox^  textBoxQuelle;
  private: System::Windows::Forms::RadioButton^  radioButtonDatei;
  private: System::Windows::Forms::RadioButton^  radioButtonOrdner;
  private: System::Windows::Forms::Button^  buttonAuswahlQuelle;
  private: System::Windows::Forms::GroupBox^  groupBoxZiel;
  private: System::Windows::Forms::TextBox^  textBoxZiel;
  private: System::Windows::Forms::Button^  buttonZiel;
  private: System::Windows::Forms::CheckBox^  checkBoxUeberschreiben;
  private: System::Windows::Forms::Button^  buttonTransformieren;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::TextBox^  textBoxMeldungen;

  private:
    Features   * m_pFeatures;         //  Features Modul
    SrsManager * m_pSrsManager;       //  Transformations-Modul
    String     ^ m_quellOrdner;       //  Quell-Ordner für die Transformation
    bool         abfrageUeberschreiben; // Steuert die Anzeige der Sicherheitsabfrage zum Überschreiben von Dateien
    System::Collections::SortedList ^ pCoordSystems;  //  Zuordnung Beschreibung eines Koordinatensystems --> Name (Full) des Systems

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
      this->comboBoxZielsystem = (gcnew System::Windows::Forms::ComboBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->groupBoxQuelle = (gcnew System::Windows::Forms::GroupBox());
      this->buttonAuswahlQuelle = (gcnew System::Windows::Forms::Button());
      this->textBoxQuelle = (gcnew System::Windows::Forms::TextBox());
      this->radioButtonDatei = (gcnew System::Windows::Forms::RadioButton());
      this->radioButtonOrdner = (gcnew System::Windows::Forms::RadioButton());
      this->groupBoxZiel = (gcnew System::Windows::Forms::GroupBox());
      this->checkBoxUeberschreiben = (gcnew System::Windows::Forms::CheckBox());
      this->buttonZiel = (gcnew System::Windows::Forms::Button());
      this->textBoxZiel = (gcnew System::Windows::Forms::TextBox());
      this->buttonTransformieren = (gcnew System::Windows::Forms::Button());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->textBoxMeldungen = (gcnew System::Windows::Forms::TextBox());
      this->groupBoxQuelle->SuspendLayout();
      this->groupBoxZiel->SuspendLayout();
      this->SuspendLayout();
      // 
      // comboBoxZielsystem
      // 
      this->comboBoxZielsystem->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->comboBoxZielsystem->FormattingEnabled = true;
      this->comboBoxZielsystem->Location = System::Drawing::Point(12, 12);
      this->comboBoxZielsystem->Name = L"comboBoxZielsystem";
      this->comboBoxZielsystem->Size = System::Drawing::Size(490, 21);
      this->comboBoxZielsystem->TabIndex = 1;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(12, 40);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(56, 13);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Zielsystem";
      // 
      // groupBoxQuelle
      // 
      this->groupBoxQuelle->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBoxQuelle->Controls->Add(this->buttonAuswahlQuelle);
      this->groupBoxQuelle->Controls->Add(this->textBoxQuelle);
      this->groupBoxQuelle->Controls->Add(this->radioButtonDatei);
      this->groupBoxQuelle->Controls->Add(this->radioButtonOrdner);
      this->groupBoxQuelle->Location = System::Drawing::Point(12, 68);
      this->groupBoxQuelle->Name = L"groupBoxQuelle";
      this->groupBoxQuelle->Size = System::Drawing::Size(490, 107);
      this->groupBoxQuelle->TabIndex = 4;
      this->groupBoxQuelle->TabStop = false;
      this->groupBoxQuelle->Text = L"Quelle";
      // 
      // buttonAuswahlQuelle
      // 
      this->buttonAuswahlQuelle->Location = System::Drawing::Point(4, 71);
      this->buttonAuswahlQuelle->Name = L"buttonAuswahlQuelle";
      this->buttonAuswahlQuelle->Size = System::Drawing::Size(75, 23);
      this->buttonAuswahlQuelle->TabIndex = 3;
      this->buttonAuswahlQuelle->Text = L"Auswahl";
      this->buttonAuswahlQuelle->UseVisualStyleBackColor = true;
      this->buttonAuswahlQuelle->Click += gcnew System::EventHandler(this, &CoordSystemTransformation::buttonAuswahlQuelle_Click);
      // 
      // textBoxQuelle
      // 
      this->textBoxQuelle->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxQuelle->Location = System::Drawing::Point(1, 44);
      this->textBoxQuelle->Name = L"textBoxQuelle";
      this->textBoxQuelle->ReadOnly = true;
      this->textBoxQuelle->Size = System::Drawing::Size(490, 20);
      this->textBoxQuelle->TabIndex = 2;
      // 
      // radioButtonDatei
      // 
      this->radioButtonDatei->AutoSize = true;
      this->radioButtonDatei->Location = System::Drawing::Point(133, 20);
      this->radioButtonDatei->Name = L"radioButtonDatei";
      this->radioButtonDatei->Size = System::Drawing::Size(50, 17);
      this->radioButtonDatei->TabIndex = 1;
      this->radioButtonDatei->TabStop = true;
      this->radioButtonDatei->Text = L"Datei";
      this->radioButtonDatei->UseVisualStyleBackColor = true;
      // 
      // radioButtonOrdner
      // 
      this->radioButtonOrdner->AutoSize = true;
      this->radioButtonOrdner->Location = System::Drawing::Point(7, 20);
      this->radioButtonOrdner->Name = L"radioButtonOrdner";
      this->radioButtonOrdner->Size = System::Drawing::Size(57, 17);
      this->radioButtonOrdner->TabIndex = 0;
      this->radioButtonOrdner->TabStop = true;
      this->radioButtonOrdner->Text = L"Ordner";
      this->radioButtonOrdner->UseVisualStyleBackColor = true;
      // 
      // groupBoxZiel
      // 
      this->groupBoxZiel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBoxZiel->Controls->Add(this->checkBoxUeberschreiben);
      this->groupBoxZiel->Controls->Add(this->buttonZiel);
      this->groupBoxZiel->Controls->Add(this->textBoxZiel);
      this->groupBoxZiel->Location = System::Drawing::Point(12, 181);
      this->groupBoxZiel->Name = L"groupBoxZiel";
      this->groupBoxZiel->Size = System::Drawing::Size(489, 100);
      this->groupBoxZiel->TabIndex = 5;
      this->groupBoxZiel->TabStop = false;
      this->groupBoxZiel->Text = L"Ziel";
      // 
      // checkBoxUeberschreiben
      // 
      this->checkBoxUeberschreiben->AutoSize = true;
      this->checkBoxUeberschreiben->Location = System::Drawing::Point(98, 52);
      this->checkBoxUeberschreiben->Name = L"checkBoxUeberschreiben";
      this->checkBoxUeberschreiben->Size = System::Drawing::Size(133, 17);
      this->checkBoxUeberschreiben->TabIndex = 2;
      this->checkBoxUeberschreiben->Text = L"Dateien überschreiben";
      this->checkBoxUeberschreiben->UseVisualStyleBackColor = true;
      // 
      // buttonZiel
      // 
      this->buttonZiel->Location = System::Drawing::Point(7, 47);
      this->buttonZiel->Name = L"buttonZiel";
      this->buttonZiel->Size = System::Drawing::Size(75, 23);
      this->buttonZiel->TabIndex = 1;
      this->buttonZiel->Text = L"Ordner";
      this->buttonZiel->UseVisualStyleBackColor = true;
      this->buttonZiel->Click += gcnew System::EventHandler(this, &CoordSystemTransformation::buttonZiel_Click);
      // 
      // textBoxZiel
      // 
      this->textBoxZiel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxZiel->Location = System::Drawing::Point(0, 20);
      this->textBoxZiel->Name = L"textBoxZiel";
      this->textBoxZiel->ReadOnly = true;
      this->textBoxZiel->Size = System::Drawing::Size(489, 20);
      this->textBoxZiel->TabIndex = 0;
      // 
      // buttonTransformieren
      // 
      this->buttonTransformieren->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonTransformieren->Location = System::Drawing::Point(413, 433);
      this->buttonTransformieren->Name = L"buttonTransformieren";
      this->buttonTransformieren->Size = System::Drawing::Size(89, 34);
      this->buttonTransformieren->TabIndex = 6;
      this->buttonTransformieren->Text = L"Transformieren";
      this->buttonTransformieren->UseVisualStyleBackColor = true;
      this->buttonTransformieren->Click += gcnew System::EventHandler(this, &CoordSystemTransformation::buttonTransformieren_Click);
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Location = System::Drawing::Point(12, 443);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 7;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &CoordSystemTransformation::buttonBeenden_Click);
      // 
      // textBoxMeldungen
      // 
      this->textBoxMeldungen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxMeldungen->Location = System::Drawing::Point(15, 287);
      this->textBoxMeldungen->Multiline = true;
      this->textBoxMeldungen->Name = L"textBoxMeldungen";
      this->textBoxMeldungen->ReadOnly = true;
      this->textBoxMeldungen->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
      this->textBoxMeldungen->Size = System::Drawing::Size(488, 130);
      this->textBoxMeldungen->TabIndex = 8;
      // 
      // CoordSystemTransformation
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(530, 482);
      this->Controls->Add(this->textBoxMeldungen);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->buttonTransformieren);
      this->Controls->Add(this->groupBoxZiel);
      this->Controls->Add(this->groupBoxQuelle);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->comboBoxZielsystem);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"CoordSystemTransformation";
      this->Text = L"Transformation des Koordinatensystem";
      this->groupBoxQuelle->ResumeLayout(false);
      this->groupBoxQuelle->PerformLayout();
      this->groupBoxZiel->ResumeLayout(false);
      this->groupBoxZiel->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

///////////////////////////////////////////////////////////////////////////////
//  Co0mmandhandler zur Auswahl der Quelldatei bzw. des Quellordners         //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAuswahlQuelle_Click(System::Object^  sender, System::EventArgs^  e) 
  {
    Windows::Forms::OpenFileDialog      ^ pQuellDateiDialog;
    Windows::Forms::FolderBrowserDialog ^ pQuellFolderDialog;
    Windows::Forms::DialogResult ok;

    if ( radioButtonDatei->Checked == true )
    {
      pQuellDateiDialog = gcnew Windows::Forms::OpenFileDialog();
      pQuellDateiDialog->FileName = textBoxQuelle->Text;
      ok = pQuellDateiDialog->ShowDialog();
      if ( ok == Windows::Forms::DialogResult::OK )
      {
        textBoxQuelle->Text = pQuellDateiDialog->FileName;
        int index = textBoxQuelle->Text->LastIndexOf( "\\");
        m_quellOrdner = textBoxQuelle->Text->Substring( 0, index );

        if ( textBoxZiel->Text == "" )
          textBoxZiel->Text = m_quellOrdner;
      }
    }

    if ( radioButtonOrdner->Checked == true )
    {
      pQuellFolderDialog = gcnew Windows::Forms::FolderBrowserDialog();
      pQuellFolderDialog->SelectedPath = textBoxQuelle->Text;
      ok = pQuellFolderDialog->ShowDialog();
      if ( ok == Windows::Forms::DialogResult::OK )
      {
        textBoxQuelle->Text = pQuellFolderDialog->SelectedPath;
        m_quellOrdner = textBoxQuelle->Text;
        if (  textBoxZiel->Text == "" )
          textBoxZiel->Text = m_quellOrdner;
      }
    }
  }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Auswahl des Zielordners                                   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonZiel_Click(System::Object^  sender, System::EventArgs^  e) 
  {
    Windows::Forms::FolderBrowserDialog ^ pZielFolderDialog;
    Windows::Forms::DialogResult ok;

    pZielFolderDialog = gcnew Windows::Forms::FolderBrowserDialog();
    pZielFolderDialog->SelectedPath = textBoxZiel->Text;
    ok = pZielFolderDialog->ShowDialog();
    if ( ok == Windows::Forms::DialogResult::OK )
    {
      textBoxZiel->Text = pZielFolderDialog->SelectedPath;
    }
  }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Durchführung der Transformation                           //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonTransformieren_Click(System::Object^  sender, System::EventArgs^  e) 
  {
    String ^ pfadNameIn;
    String ^ dateiNameIn;
    String ^ extensionIn;
    String ^ EXTENSION_IN;
    String ^ dateiNameOhneExtension;
    String ^ pfadNameOut;

    textBoxMeldungen->Text = "";

    if ( textBoxQuelle->Text->CompareTo( "" ) == 0 )
    {
      textBoxMeldungen->Text = "Keine Datei oder Ordner gewählt";
      return;
    }

    if ( checkBoxUeberschreiben->Checked == true )
    {
      if ( m_quellOrdner->CompareTo( textBoxZiel->Text ) == 0 && abfrageUeberschreiben )
      {
        Windows::Forms::DialogResult ok;

        ok = MessageBox::Show ( "Sollen die Ausgangsdateien tatsächlich überschrieben werden?", "", System::Windows::Forms::MessageBoxButtons::YesNo );
        if ( ok == Windows::Forms::DialogResult::No )
          checkBoxUeberschreiben->Checked = false;
        abfrageUeberschreiben = false;
      }
    }

    pfadNameIn = textBoxQuelle->Text;
    dateiNameIn = System::IO::Path::GetFileName( pfadNameIn );
    extensionIn = System::IO::Path::GetExtension( pfadNameIn );

    Cursor = Cursors::WaitCursor;

    if ( radioButtonDatei->Checked == true )
    {
      if ( checkBoxUeberschreiben->Checked == true )
        pfadNameOut = String::Concat( textBoxZiel->Text, "\\", dateiNameIn );
      else
      {
        int index =  dateiNameIn->IndexOf( "." );
        if ( index >= 0 )
          dateiNameOhneExtension = dateiNameIn->Substring( 0, index );
        else
           dateiNameOhneExtension = dateiNameIn;
        pfadNameOut = String::Concat( textBoxZiel->Text, "\\", dateiNameOhneExtension, "_trans", extensionIn );
      }

      transformation (  pfadNameIn, pfadNameOut );
    }
    else
    if ( radioButtonOrdner->Checked == true )
    {
       System::Collections::IEnumerator ^ FILES_ENUM = System::IO::Directory::GetFiles ( textBoxQuelle->Text )->GetEnumerator();
      while ( FILES_ENUM->MoveNext() )
      {
        pfadNameIn = dynamic_cast<String^>(FILES_ENUM->Current);
        dateiNameIn = System::IO::Path::GetFileName( pfadNameIn );
        extensionIn = System::IO::Path::GetExtension( pfadNameIn );

        EXTENSION_IN = extensionIn->ToUpper();
          if ( EXTENSION_IN->CompareTo( ".XML") != 0 &&
               EXTENSION_IN->CompareTo( ".GML") != 0 ) continue;

        if ( checkBoxUeberschreiben->Checked == false )
          pfadNameOut = String::Concat( textBoxZiel->Text, "\\", dateiNameIn );
        else
        {
          int index =  dateiNameIn->IndexOf( "." );
          if ( index >= 0 )
            dateiNameOhneExtension = dateiNameIn->Substring( 0, index );
          else
            dateiNameOhneExtension = dateiNameIn;
          pfadNameOut = String::Concat( textBoxZiel->Text, "\\", dateiNameOhneExtension, "_trans", extensionIn );
        }
       
        transformation ( pfadNameIn, pfadNameOut );
      }
    }

    Cursor = Cursors::Default;
  }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Schließen des Dialogs                                     //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e) 
  {
    Close();
  }

///////////////////////////////////////////////////////////////////////////////
//  Durchführung der Koordinaten-Transformation                              //
///////////////////////////////////////////////////////////////////////////////
private: void transformation ( String ^ quellDatei, String ^ zielDatei )
  {
    vector<Feature*> vFeatures;
    String         ^ errText;
    string           srsNameNew;
    string           stdString;

    if ( comboBoxZielsystem->SelectedItem == nullptr )
    {
      textBoxMeldungen->Text = "Es wurde kein Zielsystem gewählt";;
      return;
    }

    String ^ SRS_DESCRIPTION = dynamic_cast<String^>(comboBoxZielsystem->SelectedItem);
    int index = pCoordSystems->IndexOfKey ( SRS_DESCRIPTION );
    if ( index > 0 )
    {
      String ^ SRS_NAME = dynamic_cast<String^>(pCoordSystems->GetByIndex( index ));
      QuConvert::systemStr2stdStr( srsNameNew, SRS_NAME );
    }

    QuConvert::systemStr2stdStr( stdString, quellDatei );
		size_t anz = m_pFeatures->readFeatures( stdString, false, false, NULL );
    if ( anz == 0 )
    {
      textBoxMeldungen->Text = "GML-Datei konnte nicht gelesen werden";
      return;
    }

    anz = m_pFeatures->getAllFeatures( vFeatures );
    for ( int i = 0; i < anz; i++ )
    {
      Feature * pFeature = vFeatures[i];
      string srsName = pFeature->getSrsName();
      if ( !m_pSrsManager->existCoordSystem( srsName ) )
      {
        errText = String::Concat( "Unbekannter srsname: ", QuConvert::ToString( srsName) );
        continue;
      }

      bool success = pFeature->coordTransformation( srsName, srsNameNew );
    }

    m_pFeatures->setSrsName( srsNameNew );

    QuConvert::systemStr2stdStr( stdString, zielDatei );
    m_pFeatures->writeFeatures( stdString );

    textBoxMeldungen->Text = String::Concat( textBoxMeldungen->Text, quellDatei, " wurde erfolgreich  transformiert" );
  }
};
