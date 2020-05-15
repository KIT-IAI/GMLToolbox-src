#pragma once


	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

#include "Convert.h"
#include "Checking.h"
#include "GeometryErrorDetails.h"

	/// <summary>
	/// Liefert eine Übersicht der aufgetretenen Geometriefehler, sortiert nach den Fehlertypen
	/// </summary>
	public ref class GeometryErrorUebersicht : public System::Windows::Forms::Form
	{
	public:
		GeometryErrorUebersicht(  Features * pFeatures, Checking * pChecking, String ^ cityGMLViewer )
		{
      std::set<int>           errorTypes;
      std::set<int>::iterator iter;

			InitializeComponent();

      m_pFeatures    = pFeatures;
      m_pChecking    = pChecking;
      m_CityGMLViewer = cityGMLViewer;
      m_pErrorReport = m_pChecking->getErrorReport();
      m_errorTypes   = gcnew ArrayList;

      if ( m_CityGMLViewer == nullptr || m_CityGMLViewer->CompareTo( "" ) == 0  )
        buttonZeigen->Enabled = false;

      errorTypes = m_pErrorReport->getGeometryErrorTypes();
      for ( iter = errorTypes.begin(); iter != errorTypes.end(); iter++ )
        m_errorTypes->Add( *iter );

      geometryErrorTypeAnz =   m_errorTypes->Count;
      if (geometryErrorTypeAnz > 0 )
      {
        indexGeometryError = 0;
        typeGeometryError = (int)m_errorTypes[0];
        typeGeometryErrorString = QuConvert::ToString( GeometryError::getGeometryErrorTypeAsString ( typeGeometryError ) );
        displayGeometryError();
      }
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~GeometryErrorUebersicht()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxFehlerTyp;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  textBoxAnzahl;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Button^  buttonDetails;
  private: System::Windows::Forms::Button^  buttonZurueck;
  private: System::Windows::Forms::TextBox^  textBoxFehlerNummer;
  private: System::Windows::Forms::Button^  buttonVor;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::TextBox^  textBoxFehlerAngaben;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::TextBox^  textBoxErrorClassification;
  private: System::Windows::Forms::Button^  buttonZeigen;
  private: System::Windows::Forms::Button^  Exportieren;
  private: System::Windows::Forms::SaveFileDialog^  saveFileDialog;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

    Features        * m_pFeatures;           // Aggregation aller Features
    Checking        * m_pChecking;           // Prüfungs-Modul
    String          ^ m_CityGMLViewer;       // Generischer GML-Viewer
    ErrorReport     * m_pErrorReport;        // Fehler-Report
    ArrayList       ^ m_errorTypes;          // Liste aller aufgetretene  Fehlertypen (enum GeometryError::GeometryErrorType)
    int              indexGeometryError;     // Index des aktuellen Fehlertyps in m_errorTypes
    int              typeGeometryError;      // Aktueller Fehlertyp (enum GeometryError::GeometryErrorType)
    String         ^ typeGeometryErrorString;// Aktueller Fehlertyp als Text
    int              geometryErrorTypeAnz;   // Anzahl den Fehlertypen


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->textBoxFehlerTyp = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBoxAnzahl = (gcnew System::Windows::Forms::TextBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->buttonDetails = (gcnew System::Windows::Forms::Button());
      this->buttonZurueck = (gcnew System::Windows::Forms::Button());
      this->textBoxFehlerNummer = (gcnew System::Windows::Forms::TextBox());
      this->buttonVor = (gcnew System::Windows::Forms::Button());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->textBoxFehlerAngaben = (gcnew System::Windows::Forms::TextBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->textBoxErrorClassification = (gcnew System::Windows::Forms::TextBox());
      this->buttonZeigen = (gcnew System::Windows::Forms::Button());
      this->Exportieren = (gcnew System::Windows::Forms::Button());
      this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
      this->SuspendLayout();
      // 
      // textBoxFehlerTyp
      // 
      this->textBoxFehlerTyp->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxFehlerTyp->Location = System::Drawing::Point(13, 25);
      this->textBoxFehlerTyp->Name = L"textBoxFehlerTyp";
      this->textBoxFehlerTyp->ReadOnly = true;
      this->textBoxFehlerTyp->Size = System::Drawing::Size(261, 20);
      this->textBoxFehlerTyp->TabIndex = 0;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(13, 52);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(50, 13);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Fehlertyp";
      // 
      // textBoxAnzahl
      // 
      this->textBoxAnzahl->Location = System::Drawing::Point(13, 107);
      this->textBoxAnzahl->Name = L"textBoxAnzahl";
      this->textBoxAnzahl->ReadOnly = true;
      this->textBoxAnzahl->Size = System::Drawing::Size(61, 20);
      this->textBoxAnzahl->TabIndex = 2;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(11, 130);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(171, 13);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Anzahl der betroffenen Geometrien";
      // 
      // buttonDetails
      // 
      this->buttonDetails->Location = System::Drawing::Point(84, 104);
      this->buttonDetails->Name = L"buttonDetails";
      this->buttonDetails->Size = System::Drawing::Size(75, 23);
      this->buttonDetails->TabIndex = 4;
      this->buttonDetails->Text = L"Details";
      this->buttonDetails->UseVisualStyleBackColor = true;
      this->buttonDetails->Click += gcnew System::EventHandler(this, &GeometryErrorUebersicht::buttonDetails_Click);
      // 
      // buttonZurueck
      // 
      this->buttonZurueck->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonZurueck->Location = System::Drawing::Point(12, 252);
      this->buttonZurueck->Name = L"buttonZurueck";
      this->buttonZurueck->Size = System::Drawing::Size(39, 23);
      this->buttonZurueck->TabIndex = 5;
      this->buttonZurueck->Text = L"<--";
      this->buttonZurueck->UseVisualStyleBackColor = true;
      this->buttonZurueck->Click += gcnew System::EventHandler(this, &GeometryErrorUebersicht::buttonZurueck_Click);
      // 
      // textBoxFehlerNummer
      // 
      this->textBoxFehlerNummer->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxFehlerNummer->Location = System::Drawing::Point(67, 255);
      this->textBoxFehlerNummer->Name = L"textBoxFehlerNummer";
      this->textBoxFehlerNummer->ReadOnly = true;
      this->textBoxFehlerNummer->Size = System::Drawing::Size(149, 20);
      this->textBoxFehlerNummer->TabIndex = 6;
      // 
      // buttonVor
      // 
      this->buttonVor->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonVor->Location = System::Drawing::Point(235, 252);
      this->buttonVor->Name = L"buttonVor";
      this->buttonVor->Size = System::Drawing::Size(39, 23);
      this->buttonVor->TabIndex = 7;
      this->buttonVor->Text = L"-->";
      this->buttonVor->UseVisualStyleBackColor = true;
      this->buttonVor->Click += gcnew System::EventHandler(this, &GeometryErrorUebersicht::buttonVor_Click);
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonBeenden->Location = System::Drawing::Point(199, 295);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 8;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &GeometryErrorUebersicht::buttonBeenden_Click);
      // 
      // textBoxFehlerAngaben
      // 
      this->textBoxFehlerAngaben->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxFehlerAngaben->Location = System::Drawing::Point(12, 152);
      this->textBoxFehlerAngaben->Multiline = true;
      this->textBoxFehlerAngaben->Name = L"textBoxFehlerAngaben";
      this->textBoxFehlerAngaben->ReadOnly = true;
      this->textBoxFehlerAngaben->Size = System::Drawing::Size(262, 82);
      this->textBoxFehlerAngaben->TabIndex = 9;
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(13, 79);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(68, 13);
      this->label3->TabIndex = 10;
      this->label3->Text = L"Klassifikation";
      // 
      // textBoxErrorClassification
      // 
      this->textBoxErrorClassification->BackColor = System::Drawing::SystemColors::Menu;
      this->textBoxErrorClassification->BorderStyle = System::Windows::Forms::BorderStyle::None;
      this->textBoxErrorClassification->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->textBoxErrorClassification->Location = System::Drawing::Point(88, 79);
      this->textBoxErrorClassification->Name = L"textBoxErrorClassification";
      this->textBoxErrorClassification->Size = System::Drawing::Size(94, 13);
      this->textBoxErrorClassification->TabIndex = 11;
      // 
      // buttonZeigen
      // 
      this->buttonZeigen->Location = System::Drawing::Point(198, 89);
      this->buttonZeigen->Name = L"buttonZeigen";
      this->buttonZeigen->Size = System::Drawing::Size(75, 23);
      this->buttonZeigen->TabIndex = 12;
      this->buttonZeigen->Text = L"Zeigen";
      this->buttonZeigen->UseVisualStyleBackColor = true;
      this->buttonZeigen->Click += gcnew System::EventHandler(this, &GeometryErrorUebersicht::buttonZeigen_Click);
      // 
      // Exportieren
      // 
      this->Exportieren->Location = System::Drawing::Point(198, 119);
      this->Exportieren->Name = L"Exportieren";
      this->Exportieren->Size = System::Drawing::Size(75, 23);
      this->Exportieren->TabIndex = 13;
      this->Exportieren->Text = L"Exportieren";
      this->Exportieren->UseVisualStyleBackColor = true;
      this->Exportieren->Click += gcnew System::EventHandler(this, &GeometryErrorUebersicht::Exportieren_Click);
      // 
      // saveFileDialog
      // 
      this->saveFileDialog->Filter = L"(*.gml)|*.gml;";
      // 
      // GeometryErrorUebersicht
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(286, 327);
      this->Controls->Add(this->Exportieren);
      this->Controls->Add(this->buttonZeigen);
      this->Controls->Add(this->textBoxErrorClassification);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->textBoxFehlerAngaben);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->buttonVor);
      this->Controls->Add(this->textBoxFehlerNummer);
      this->Controls->Add(this->buttonZurueck);
      this->Controls->Add(this->buttonDetails);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->textBoxAnzahl);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->textBoxFehlerTyp);
      this->Name = L"GeometryErrorUebersicht";
      this->Text = L"Geometriefehler Übersicht";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Anzeige von Informationen über die Geometriefehler des aktuellen Typs    //
///////////////////////////////////////////////////////////////////////////////
    Void displayGeometryError()
    {
      vector<GeometryError*> vGeometryErrors;
      size_t                 i, anz;
      String               ^ STR;

      textBoxFehlerAngaben->Text = "";
      textBoxFehlerTyp->Text     = typeGeometryErrorString;
      textBoxFehlerNummer->Text  = String::Concat( "Geometriefehler ", Convert::ToString( indexGeometryError+1 ), " (", Convert::ToString( geometryErrorTypeAnz ),  ")");
      textBoxAnzahl->Text        = Convert::ToString ( m_pErrorReport->getGeometryErrorAnz( typeGeometryError ) );

      anz = m_pErrorReport->getGeometryErrors( typeGeometryError,  vGeometryErrors );
      Error::ERROR_CLASSIFICATION errorClass = vGeometryErrors[0]->getErrorClassification();
      textBoxErrorClassification->Text = QuConvert::ToString( Error::getErrorClassificationAsString( errorClass) );
      if ( errorClass == Error::ERROR )
        textBoxErrorClassification->ForeColor = System::Drawing::Color::Red;
      else
        textBoxErrorClassification->ForeColor = System::Drawing::Color::Black;

      if ( typeGeometryError == GeometryError::POLYGON_NON_PLANAR )
      {
        double distAvarage, distMax, distMaxGes;

        distMaxGes = 0.0;

        for ( i = 0; i < anz; i++ )
        {
          GeometryError * pGeometryError = vGeometryErrors[i];
          pGeometryError->getPlanaritaetsfehler( distMax, distAvarage );
          if ( distMax > distMaxGes )
            distMaxGes = distMax;
        }

        Double ^ d = gcnew Double( distMaxGes );


        STR = String::Concat( "Maximaler Planaritätsfehler: ",d->ToString( "F05" ), " [m]" );
        textBoxFehlerAngaben->Text = STR;
      }
    }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button <--                                                //
//  Auswahl des vorhergehenden Fehlertyps bei der Iteration                  //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonZurueck_Click(System::Object^  sender, System::EventArgs^  e)
  {
    if ( indexGeometryError > 0 )
    {
      indexGeometryError--;
      typeGeometryError = (int)m_errorTypes[indexGeometryError];
      typeGeometryErrorString = QuConvert::ToString( GeometryError::getGeometryErrorTypeAsString ( typeGeometryError ) );
      displayGeometryError();
    }
  }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button -->                                                //
//  Auswahl des nächsten Fehlertyps bei der Iteration                        //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonVor_Click(System::Object^  sender, System::EventArgs^  e)  
  {
    if ( indexGeometryError < geometryErrorTypeAnz - 1 )
    {
      indexGeometryError++;
      typeGeometryError = (int)m_errorTypes[indexGeometryError];
      typeGeometryErrorString = QuConvert::ToString( GeometryError::getGeometryErrorTypeAsString ( typeGeometryError ) );
      displayGeometryError();
    }
  }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Beenden                                            //
//  Schließt den Dialog                                                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
  {
    Close();
  }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Details                                            //
//  Öffnet den GeometryErrorDetails Dialog                                   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonDetails_Click(System::Object^  sender, System::EventArgs^  e)
  {
    GeometryErrorDetails ^ pErrorDetails = gcnew GeometryErrorDetails ( m_pFeatures, m_pChecking, m_CityGMLViewer, 
                typeGeometryError, -1 );
    pErrorDetails->ShowDialog();
  }

////////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Zeigen                                              //
//  Generiert für alle Geometriefehler die Fehlergeometrie und visualisiert   //
//  sie, zusammen mit den zugehörigen Features, mit dem generischen GML-Viewer//
////////////////////////////////////////////////////////////////////////////////
private: System::Void buttonZeigen_Click(System::Object^  sender, System::EventArgs^  e)
         {
           vector<GeometryError*> vGeometryErrors;
					 size_t                 i, geometryErrorAnz, geoAnz;
           Feature              * pFeature;
           string                 fileName;
           set<Feature*>          sFeatures;
           vector<Feature*>       vFeatures;

           geometryErrorAnz = m_pErrorReport->getGeometryErrors( typeGeometryError,  vGeometryErrors );
           if ( geometryErrorAnz == 0 )
             return;

           for ( i = 0; i < geometryErrorAnz; ++i  )
           {
             GeometryError * pGeometryError = vGeometryErrors[i];
             GeometryError::GEOMETRY_ERROR_TYPE errType = pGeometryError->getGeometryErrorType();
             pFeature = pGeometryError->getFeature();

             geoAnz =  pGeometryError->getErrorGeometryAnz();
             if ( geoAnz > 0 && pGeometryError->getErrorGeometryCreated() == false )
             {
               for ( unsigned int j = 0; j < geoAnz; j++ )
                 pFeature->addCityGMLGeometryRepresentationWithContext( pGeometryError->getErrorGeometry( j ), GmlNamespaces::cityGml_2_0_georepNamespaceKuerzel,
                 "Geometriefehler", GeometryError::getGeometryErrorTypeAsString ( errType ));

               pGeometryError->setErrorGeometryCreated();
             }
             if ( sFeatures.find( pFeature) == sFeatures.end() )
             {
               vFeatures.push_back( pFeature );
               sFeatures.insert( pFeature );
             }
           }

           fileName = Constant::getSystemFolder() + "TempObj.gml";

           GMLWriter ^ pWriter = gcnew GMLWriter ( m_pFeatures );
           pWriter->setWriteXLinks ( false );
           pWriter->writeSingleFeatures ( fileName, 0,  vFeatures );
           delete pWriter;      

           try
           {
             System::Diagnostics::Process          ^ pProcess = gcnew System::Diagnostics::Process();
             System::Diagnostics::ProcessStartInfo ^ processStartInfo = gcnew System::Diagnostics::ProcessStartInfo;

             processStartInfo->FileName =  m_CityGMLViewer;
             processStartInfo->Arguments = String::Concat ( "\"", QuConvert::ToString ( fileName ), "\"" );
             pProcess->StartInfo = processStartInfo;
             pProcess->Start();
           }

           catch ( Exception ^ )
           {
             MessageBox::Show ( "GML-Viewer kann nicht gestartet werden");
           }
         }

////////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Zeigen                                              //
//  Generiert für alle Geometriefehler die Fehlergeometrie und speichert      //
//  sie, zusammen mit den zugehörigen Features, in einer benutzerdefinierten  //
//  Datei                                                                     //
////////////////////////////////////////////////////////////////////////////////
private: System::Void Exportieren_Click(System::Object^  sender, System::EventArgs^  e)
         {
           vector<GeometryError*> vGeometryErrors;
					 size_t                 i, geometryErrorAnz, geoAnz;
           Feature              * pFeature;
           string                 fileName;
           set<Feature*>          sFeatures;
           vector<Feature*>       vFeatures;

           geometryErrorAnz = m_pErrorReport->getGeometryErrors( typeGeometryError,  vGeometryErrors );
           if ( geometryErrorAnz == 0 )
             return;

           if ( saveFileDialog->ShowDialog() != System::Windows::Forms::DialogResult::OK )
             return;
           
           QuConvert::systemStr2stdStr( fileName, saveFileDialog->FileName );

           for ( i = 0; i < geometryErrorAnz; ++i  )
           {
             GeometryError * pGeometryError = vGeometryErrors[i];
             GeometryError::GEOMETRY_ERROR_TYPE errType = pGeometryError->getGeometryErrorType();
             pFeature = pGeometryError->getFeature();

             geoAnz =  pGeometryError->getErrorGeometryAnz();
             if ( geoAnz > 0 && pGeometryError->getErrorGeometryCreated() == false )
             {
               for ( unsigned int j = 0; j < geoAnz; j++ )
                 pFeature->addCityGMLGeometryRepresentationWithContext( pGeometryError->getErrorGeometry( j ), GmlNamespaces::cityGml_2_0_georepNamespaceKuerzel,
                 "Geometriefehler", GeometryError::getGeometryErrorTypeAsString ( errType ));

               pGeometryError->setErrorGeometryCreated();
             }
             if ( sFeatures.find( pFeature) == sFeatures.end() )
             {
               vFeatures.push_back( pFeature );
               sFeatures.insert( pFeature );
             }
           }

           GMLWriter ^ pWriter = gcnew GMLWriter ( m_pFeatures );
           pWriter->setWriteXLinks ( false );
           pWriter->writeSingleFeatures ( fileName, 0,  vFeatures );
           delete pWriter;             
         }
};
