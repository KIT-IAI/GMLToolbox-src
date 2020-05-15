#pragma once

#include "Feature.h"
#include "constant.h"
#include "Osm.h"
#include "OSM_Reader.h"
#include "OSM_Writer.h"
#include "OSM_ClassStatistics.h"

using namespace std;

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für OSM_LargeModelReader
	/// </summary>
	public ref class OSM_LargeModelReader : public System::Windows::Forms::Form
	{
	public:
		OSM_LargeModelReader( OSM_Model * pOSMModel )
		{
       vector<string> vFilterNames;
			 size_t         i, filterAnz;

			InitializeComponent();

      m_pOSMModel = pOSMModel;

      checkBoxStatistics->Checked = false;
      osmObjAnz = 0;

      checkBoxUseFilter->Checked = false;
      comboBoxFilter->Visible    = false;
      labelFilter->Visible       = false;

      string str = Constant::getSystemFolder() + "GmlSchemata\\OSM\\Filter\\" + "Filters.xml";

      pTagFilterCollection = new OSM_TagFilterCollection();
      pTagFilterCollection->readFilterCollection( str );

      filterAnz = pTagFilterCollection->getTagFilterNames( vFilterNames );
      for ( i = 0; i < filterAnz; i++ )
      {
        str = vFilterNames[i];
        comboBoxFilter->Items->Add( QuConvert::ToString( str ) );
      }

      if ( filterAnz > 0 )
        comboBoxFilter->SelectedIndex = 0;

      exportAsShapefile = false;

//      pFilterSelected = NULL;

//      System::Collections::ArrayList ^ pArrayList = gcnew System::Collections::ArrayList;
//      pArrayList->Add(  pViewer );
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~OSM_LargeModelReader()
		{
			if (components)
			{
				delete components;
			}

      if ( pTagFilterCollection != NULL )
        delete pTagFilterCollection;
		}

  private: System::Windows::Forms::TextBox^  textBoxFilename;
  private: System::Windows::Forms::Button^  buttonReadOSM;
  private: System::Windows::Forms::CheckBox^  checkBoxStatistics;
  private: System::Windows::Forms::ListBox^  listBoxMeldungen;
  private: System::Windows::Forms::OpenFileDialog^  openFileDialogOSM;
  private: System::Windows::Forms::ComboBox^  comboBoxFilter;
  private: System::Windows::Forms::Label^  labelFilter;
  private: System::Windows::Forms::CheckBox^  checkBoxUseFilter;
  private: System::Windows::Forms::Button^  buttonStart;
  private: System::Windows::Forms::ProgressBar^  progressBarEinlesen;
  private: System::Windows::Forms::Button^  buttonUebernehmen;
  private: System::Windows::Forms::Button^  buttonSichern;
  private: System::Windows::Forms::SaveFileDialog^  saveFileDialogOSM;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

    OSM_Model               * m_pOSMModel;;
    OSM_TagFilterCollection * pTagFilterCollection;
    OSM_TagFilter           * pFilterSelected;
		size_t                    osmObjAnz;
  private: System::Windows::Forms::Button^  buttonVisualisieren;
           bool                      exportAsShapefile;




#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->textBoxFilename = (gcnew System::Windows::Forms::TextBox());
      this->buttonReadOSM = (gcnew System::Windows::Forms::Button());
      this->checkBoxStatistics = (gcnew System::Windows::Forms::CheckBox());
      this->listBoxMeldungen = (gcnew System::Windows::Forms::ListBox());
      this->openFileDialogOSM = (gcnew System::Windows::Forms::OpenFileDialog());
      this->comboBoxFilter = (gcnew System::Windows::Forms::ComboBox());
      this->labelFilter = (gcnew System::Windows::Forms::Label());
      this->checkBoxUseFilter = (gcnew System::Windows::Forms::CheckBox());
      this->buttonStart = (gcnew System::Windows::Forms::Button());
      this->progressBarEinlesen = (gcnew System::Windows::Forms::ProgressBar());
      this->buttonUebernehmen = (gcnew System::Windows::Forms::Button());
      this->buttonSichern = (gcnew System::Windows::Forms::Button());
      this->saveFileDialogOSM = (gcnew System::Windows::Forms::SaveFileDialog());
      this->buttonVisualisieren = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // textBoxFilename
      // 
      this->textBoxFilename->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxFilename->Location = System::Drawing::Point(13, 13);
      this->textBoxFilename->Name = L"textBoxFilename";
      this->textBoxFilename->ReadOnly = true;
      this->textBoxFilename->Size = System::Drawing::Size(331, 20);
      this->textBoxFilename->TabIndex = 0;
      // 
      // buttonReadOSM
      // 
      this->buttonReadOSM->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->buttonReadOSM->Location = System::Drawing::Point(13, 40);
      this->buttonReadOSM->Name = L"buttonReadOSM";
      this->buttonReadOSM->Size = System::Drawing::Size(331, 23);
      this->buttonReadOSM->TabIndex = 1;
      this->buttonReadOSM->Text = L"OSM-Datei auswählen";
      this->buttonReadOSM->UseVisualStyleBackColor = true;
      this->buttonReadOSM->Click += gcnew System::EventHandler(this, &OSM_LargeModelReader::buttonReadOSM_Click);
      // 
      // checkBoxStatistics
      // 
      this->checkBoxStatistics->AutoSize = true;
      this->checkBoxStatistics->Location = System::Drawing::Point(12, 69);
      this->checkBoxStatistics->Name = L"checkBoxStatistics";
      this->checkBoxStatistics->Size = System::Drawing::Size(112, 17);
      this->checkBoxStatistics->TabIndex = 2;
      this->checkBoxStatistics->Text = L"Nur Modellstatistik";
      this->checkBoxStatistics->UseVisualStyleBackColor = true;
      // 
      // listBoxMeldungen
      // 
      this->listBoxMeldungen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->listBoxMeldungen->FormattingEnabled = true;
      this->listBoxMeldungen->Location = System::Drawing::Point(13, 195);
      this->listBoxMeldungen->Name = L"listBoxMeldungen";
      this->listBoxMeldungen->Size = System::Drawing::Size(331, 199);
      this->listBoxMeldungen->TabIndex = 4;
      // 
      // openFileDialogOSM
      // 
      this->openFileDialogOSM->Filter = L"\"OSM-Dateien|*.osm|XML-Dateien|*.xml\"";
      // 
      // comboBoxFilter
      // 
      this->comboBoxFilter->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->comboBoxFilter->FormattingEnabled = true;
      this->comboBoxFilter->Location = System::Drawing::Point(12, 92);
      this->comboBoxFilter->Name = L"comboBoxFilter";
      this->comboBoxFilter->Size = System::Drawing::Size(332, 21);
      this->comboBoxFilter->TabIndex = 6;
      this->comboBoxFilter->SelectedIndexChanged += gcnew System::EventHandler(this, &OSM_LargeModelReader::comboBoxFilter_SelectedIndexChanged);
      // 
      // labelFilter
      // 
      this->labelFilter->AutoSize = true;
      this->labelFilter->Location = System::Drawing::Point(13, 120);
      this->labelFilter->Name = L"labelFilter";
      this->labelFilter->Size = System::Drawing::Size(29, 13);
      this->labelFilter->TabIndex = 7;
      this->labelFilter->Text = L"Filter";
      // 
      // checkBoxUseFilter
      // 
      this->checkBoxUseFilter->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->checkBoxUseFilter->AutoSize = true;
      this->checkBoxUseFilter->Location = System::Drawing::Point(238, 69);
      this->checkBoxUseFilter->Name = L"checkBoxUseFilter";
      this->checkBoxUseFilter->Size = System::Drawing::Size(106, 17);
      this->checkBoxUseFilter->TabIndex = 8;
      this->checkBoxUseFilter->Text = L"OSM-Datei filtern";
      this->checkBoxUseFilter->UseVisualStyleBackColor = true;
      this->checkBoxUseFilter->CheckedChanged += gcnew System::EventHandler(this, &OSM_LargeModelReader::checkBoxUseFilter_CheckedChanged);
      // 
      // buttonStart
      // 
      this->buttonStart->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonStart->Location = System::Drawing::Point(269, 119);
      this->buttonStart->Name = L"buttonStart";
      this->buttonStart->Size = System::Drawing::Size(75, 23);
      this->buttonStart->TabIndex = 9;
      this->buttonStart->Text = L"Einlesen";
      this->buttonStart->UseVisualStyleBackColor = true;
      this->buttonStart->Click += gcnew System::EventHandler(this, &OSM_LargeModelReader::buttonStart_Click);
      // 
      // progressBarEinlesen
      // 
      this->progressBarEinlesen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->progressBarEinlesen->Location = System::Drawing::Point(13, 149);
      this->progressBarEinlesen->Name = L"progressBarEinlesen";
      this->progressBarEinlesen->Size = System::Drawing::Size(331, 23);
      this->progressBarEinlesen->TabIndex = 3;
      // 
      // buttonUebernehmen
      // 
      this->buttonUebernehmen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonUebernehmen->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonUebernehmen->Location = System::Drawing::Point(255, 411);
      this->buttonUebernehmen->Name = L"buttonUebernehmen";
      this->buttonUebernehmen->Size = System::Drawing::Size(79, 52);
      this->buttonUebernehmen->TabIndex = 10;
      this->buttonUebernehmen->Text = L"Beenden";
      this->buttonUebernehmen->UseVisualStyleBackColor = true;
      this->buttonUebernehmen->Click += gcnew System::EventHandler(this, &OSM_LargeModelReader::buttonUebernehmen_Click);
      // 
      // buttonSichern
      // 
      this->buttonSichern->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonSichern->Location = System::Drawing::Point(16, 427);
      this->buttonSichern->Name = L"buttonSichern";
      this->buttonSichern->Size = System::Drawing::Size(75, 36);
      this->buttonSichern->TabIndex = 11;
      this->buttonSichern->Text = L"Sichern als OSM";
      this->buttonSichern->UseVisualStyleBackColor = true;
      this->buttonSichern->Click += gcnew System::EventHandler(this, &OSM_LargeModelReader::buttonSichern_Click);
      // 
      // saveFileDialogOSM
      // 
      this->saveFileDialogOSM->Filter = L"OSM-Dateien|*.osm";
      this->saveFileDialogOSM->Title = L"OSM-Modell sichern";
      // 
      // buttonVisualisieren
      // 
      this->buttonVisualisieren->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonVisualisieren->Location = System::Drawing::Point(111, 427);
      this->buttonVisualisieren->Name = L"buttonVisualisieren";
      this->buttonVisualisieren->Size = System::Drawing::Size(75, 36);
      this->buttonVisualisieren->TabIndex = 12;
      this->buttonVisualisieren->Text = L"Visualisieren";
      this->buttonVisualisieren->UseVisualStyleBackColor = true;
      this->buttonVisualisieren->Click += gcnew System::EventHandler(this, &OSM_LargeModelReader::buttonVisualisieren_Click);
      // 
      // OSM_LargeModelReader
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(356, 475);
      this->Controls->Add(this->buttonVisualisieren);
      this->Controls->Add(this->buttonSichern);
      this->Controls->Add(this->buttonUebernehmen);
      this->Controls->Add(this->buttonStart);
      this->Controls->Add(this->checkBoxUseFilter);
      this->Controls->Add(this->labelFilter);
      this->Controls->Add(this->comboBoxFilter);
      this->Controls->Add(this->listBoxMeldungen);
      this->Controls->Add(this->progressBarEinlesen);
      this->Controls->Add(this->checkBoxStatistics);
      this->Controls->Add(this->buttonReadOSM);
      this->Controls->Add(this->textBoxFilename);
      this->Name = L"OSM_LargeModelReader";
      this->Text = L"Große OSM-Modelle";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

///////////////////////////////////////////////////////////////////////////////
// OSM-Datei spezifizieren                                                   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonReadOSM_Click(System::Object^  sender, System::EventArgs^  e)
{
  System::Windows::Forms::DialogResult ok =  openFileDialogOSM->ShowDialog();
  if ( ok != System::Windows::Forms::DialogResult::OK )
    return;

  textBoxFilename->Text =  openFileDialogOSM->FileName;

}

///////////////////////////////////////////////////////////////////////////////
// Dialog beenden, ohne die OSM-Datei zu übernehmen                          //
 ///////////////////////////////////////////////////////////////////////////////

private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
{
  m_pOSMModel->resetOSM_Model();
  Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Umschalten der CheckBox "Filterbenutzung"                                //
///////////////////////////////////////////////////////////////////////////////
private: System::Void checkBoxUseFilter_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
  if ( checkBoxUseFilter->Checked )
  {
    comboBoxFilter->Visible = true;
    labelFilter->Visible    = true;
  }
  else
  {
    comboBoxFilter->Visible = false;
    labelFilter->Visible    = false;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Änderung des benutzten Filters                                           //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxFilter_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
  string filterName;

  String ^ STR = dynamic_cast<String^>(comboBoxFilter->SelectedItem);
  QuConvert::systemStr2stdStr( filterName, STR );
  pFilterSelected = pTagFilterCollection->getTagFilter( filterName );
}

 ///////////////////////////////////////////////////////////////////////////////
 //  Einlesen/Analyse der OSM-Datei                                           //
 ///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonStart_Click(System::Object^  sender, System::EventArgs^  e)
{
  String            ^ STR;
  OSM_StatisticsAll * pOSM_Statistics = NULL;

  if ( String::Compare (openFileDialogOSM->FileName, "" ) == 0 )
  {
    listBoxMeldungen->Items->Add(  "Es wurde keine OSM-Datei spezifiziert" );
    return;
  }

  if ( checkBoxStatistics->Checked )
  {
    pOSM_Statistics = new OSM_StatisticsAll;
    listBoxMeldungen->Items->Add(  "Es wird nur die Modell-Statistik generiert" );
  }
  else
    listBoxMeldungen->Items->Add(  "Das OSM-Modell wird eingelesen" );

  if ( checkBoxUseFilter->Checked )
  {
    if ( pFilterSelected == NULL )
    {
      listBoxMeldungen->Items->Add ( "Kein Filter spezifiziert " );
      return;
    }

    string fileName;
    QuConvert::systemStr2stdStr( fileName, openFileDialogOSM->FileName );
    osmObjAnz = m_pOSMModel->readOsmModelWithFilter ( pFilterSelected, fileName, listBoxMeldungen );

    STR = String::Concat( "Einlesen der OSM-Datei beendet, Anzahl der OSM-Objekte: ", Convert::ToString( (int)osmObjAnz ) );
    listBoxMeldungen->Items->Add( STR );
    return;
  }

  XmlTextReader ^ pXMLReader = gcnew XmlTextReader ( openFileDialogOSM->FileName );

  while( pXMLReader->Read() )
  {
    if ( pXMLReader->NodeType == XmlNodeType::Element )
    {
      String ^ pActElement = pXMLReader->Name;
      if ( pActElement->CompareTo( "osm" ) == 0 )
      {
        listBoxMeldungen->Items->Add( "Einlesen der OSM-Datei gestartet ...");
        listBoxMeldungen->Items->Add ( " " );
        listBoxMeldungen->Refresh();

        osmObjAnz = m_pOSMModel->readOsmModel ( pXMLReader, pOSM_Statistics );

        STR = String::Concat( "Einlesen der OSM-Datei beendet, Anzahl der OSM-Objekte: ", Convert::ToString( osmObjAnz ) );
        listBoxMeldungen->Items->Add( STR );
        listBoxMeldungen->Items->Add ( " " );

        if ( m_pOSMModel->getIncompleteWayAnz() > 0  )
        {
          STR = String::Concat (  Convert::ToString(  m_pOSMModel->getIncompleteWayAnz() ),  " unvollständige OSM-Ways eingelesen" );
          listBoxMeldungen->Items->Add( STR );
        }

        if ( m_pOSMModel->getIncompleteRelationAnz() > 0 )
        {
          STR = String::Concat (  Convert::ToString(  m_pOSMModel->getIncompleteRelationAnz() ),  " unvollständige OSM-Relations eingelesen" );
          listBoxMeldungen->Items->Add( STR );

        }
        break;
      }
      else
        break;
    }
  }

  if ( checkBoxStatistics->Checked )
  {
    OSM_ClassStatistics ^ pClassStatistics = gcnew OSM_ClassStatistics ( pOSM_Statistics );
    pClassStatistics->ShowDialog();
    delete pClassStatistics;
  }

}
private: System::Void buttonUebernehmen_Click(System::Object^  sender, System::EventArgs^  e)
{
  Close();
}

public: size_t getReadAnz()
        {
          return osmObjAnz;
        }

public: String ^ getOSMDateiName()
        {
          return openFileDialogOSM->FileName;
        }
private: System::Void buttonSichern_Click(System::Object^  sender, System::EventArgs^  e)
         {
           string fileName;

           System::Windows::Forms::DialogResult ok =  saveFileDialogOSM->ShowDialog();
           if ( ok != System::Windows::Forms::DialogResult::OK )
             return;

           QuConvert::systemStr2stdStr( fileName, saveFileDialogOSM->FileName );

           OSMWriter ^ pWriter = gcnew OSMWriter ( m_pOSMModel );
           int anz = pWriter->write( fileName );

           String ^ STR = String::Concat(  Convert::ToString( anz ), " OSM-Objeke geschrieben " );
           listBoxMeldungen->Items->Add( STR );
         }
public: bool getExportAsShapefile()
        {
          return exportAsShapefile;
        }

private: System::Void buttonVisualisieren_Click(System::Object^  sender, System::EventArgs^  e)
         {
            System::Diagnostics::ProcessStartInfo ^  processStartInfo;
            System::Diagnostics::Process          ^ iexplorerProcess;

            m_pOSMModel->writeOSM_SVG();

            String ^ kartenDatei = QuConvert::ToString( Constant::getHTMLFramePath ( OSM ) );

            iexplorerProcess = gcnew System::Diagnostics::Process();
            processStartInfo = gcnew System::Diagnostics::ProcessStartInfo;

            processStartInfo->FileName = QuConvert::ToString( m_pOSMModel->getSvgViewer() );
            processStartInfo->Arguments = kartenDatei;
            iexplorerProcess->StartInfo = processStartInfo;
            iexplorerProcess->Start();
         }
};
