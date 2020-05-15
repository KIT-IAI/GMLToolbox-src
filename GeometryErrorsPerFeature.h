#pragma once

#include "SrsManagerRef.h"
#include "Convert.h"
#include "Checking.h"
#include "GeometryErrorDetails.h"

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Anzeige aller Geometriefehler einzelner Features
	/// </summary>
	public ref class GeometryErrorsPerFeature : public System::Windows::Forms::Form
	{
	public:
		GeometryErrorsPerFeature( Features * pFeatures, Checking * pChecking, String ^ kITViewer )
		{
			InitializeComponent();

      m_pFeatures    = pFeatures;
      m_pChecking    = pChecking;
      m_pErrorReport = m_pChecking->getErrorReport();
      m_KITViewer    = kITViewer;
      m_selectedNode = nullptr;

      if ( kITViewer == nullptr || kITViewer->CompareTo( "" ) == 0 )
        buttonZeigen->Enabled = false;

      initialize();
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~GeometryErrorsPerFeature()
		{
			if (components)
			{
				delete components;
			}
		}

  protected: 

  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::TreeView^  treeView1;
  private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
  private: System::Windows::Forms::ToolStripMenuItem^  detailsToolStripMenuItem;
  private: System::Windows::Forms::SaveFileDialog^  saveFileDialog;
  private: System::Windows::Forms::TextBox^  textBoxErrorDetails;
  private: System::Windows::Forms::Button^  buttonZeigen;
  private: System::Windows::Forms::Button^  buttonExport;

  protected: 
    Features        * m_pFeatures;    // Aggregation aller Features
    Checking        * m_pChecking;    // Prüfungs-Modul
    ErrorReport     * m_pErrorReport; // Fehler-Report
    String          ^ m_KITViewer;    // Generischer GML-Viewer
    TreeNode        ^ m_selectedNode; // Aktuell ausgewählter Knoten im Fehler-Baum

  private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->components = (gcnew System::ComponentModel::Container());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->treeView1 = (gcnew System::Windows::Forms::TreeView());
      this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->detailsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
      this->textBoxErrorDetails = (gcnew System::Windows::Forms::TextBox());
      this->buttonZeigen = (gcnew System::Windows::Forms::Button());
      this->buttonExport = (gcnew System::Windows::Forms::Button());
      this->contextMenuStrip1->SuspendLayout();
      this->SuspendLayout();
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
      this->buttonBeenden->Location = System::Drawing::Point(157, 405);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 2;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &GeometryErrorsPerFeature::buttonBeenden_Click);
      // 
      // treeView1
      // 
      this->treeView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->treeView1->ContextMenuStrip = this->contextMenuStrip1;
      this->treeView1->Location = System::Drawing::Point(-1, 2);
      this->treeView1->Name = L"treeView1";
      this->treeView1->Size = System::Drawing::Size(393, 316);
      this->treeView1->TabIndex = 3;
      this->treeView1->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &GeometryErrorsPerFeature::treeView1_AfterSelect);
      // 
      // contextMenuStrip1
      // 
      this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->detailsToolStripMenuItem});
      this->contextMenuStrip1->Name = L"contextMenuStrip1";
      this->contextMenuStrip1->Size = System::Drawing::Size(110, 26);
      this->contextMenuStrip1->Text = L"ABC";
      // 
      // detailsToolStripMenuItem
      // 
      this->detailsToolStripMenuItem->Name = L"detailsToolStripMenuItem";
      this->detailsToolStripMenuItem->Size = System::Drawing::Size(109, 22);
      this->detailsToolStripMenuItem->Text = L"Details";
      this->detailsToolStripMenuItem->Click += gcnew System::EventHandler(this, &GeometryErrorsPerFeature::detailsToolStripMenuItem_Click);
      // 
      // saveFileDialog
      // 
      this->saveFileDialog->DefaultExt = L"*.gml";
      this->saveFileDialog->Filter = L"(*.gml)|*.gml;";
      // 
      // textBoxErrorDetails
      // 
      this->textBoxErrorDetails->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxErrorDetails->Location = System::Drawing::Point(-1, 338);
      this->textBoxErrorDetails->Multiline = true;
      this->textBoxErrorDetails->Name = L"textBoxErrorDetails";
      this->textBoxErrorDetails->Size = System::Drawing::Size(393, 47);
      this->textBoxErrorDetails->TabIndex = 4;
      // 
      // buttonZeigen
      // 
      this->buttonZeigen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonZeigen->Location = System::Drawing::Point(12, 403);
      this->buttonZeigen->Name = L"buttonZeigen";
      this->buttonZeigen->Size = System::Drawing::Size(75, 34);
      this->buttonZeigen->TabIndex = 5;
      this->buttonZeigen->Text = L"Alle Fehler zeigen";
      this->buttonZeigen->UseVisualStyleBackColor = true;
      this->buttonZeigen->Click += gcnew System::EventHandler(this, &GeometryErrorsPerFeature::buttonZeigen_Click);
      // 
      // buttonExport
      // 
      this->buttonExport->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonExport->Location = System::Drawing::Point(293, 403);
      this->buttonExport->Name = L"buttonExport";
      this->buttonExport->Size = System::Drawing::Size(75, 34);
      this->buttonExport->TabIndex = 6;
      this->buttonExport->Text = L"Alle Fehler exportieren";
      this->buttonExport->UseVisualStyleBackColor = true;
      this->buttonExport->Click += gcnew System::EventHandler(this, &GeometryErrorsPerFeature::buttonExport_Click);
      // 
      // GeometryErrorsPerFeature
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(390, 451);
      this->Controls->Add(this->buttonExport);
      this->Controls->Add(this->buttonZeigen);
      this->Controls->Add(this->textBoxErrorDetails);
      this->Controls->Add(this->treeView1);
      this->Controls->Add(this->buttonBeenden);
      this->Name = L"GeometryErrorsPerFeature";
      this->Text = L"Geometriefehler";
      this->contextMenuStrip1->ResumeLayout(false);
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Beenden                                            //
//  Löscht eine eventuell erzeugte Fehler-Geometrie und schließt den Dialog  //
///////////////////////////////////////////////////////////////////////////////
  private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e) 
           {
             vector<GeometryError*> vGeometryErrors;
             size_t                 i, anz;

             anz = m_pErrorReport->getGeometryErrors( vGeometryErrors );

             for ( i = 0; i < anz; i++ )
             {
               GeometryError *pError = vGeometryErrors[i];
               if ( pError->getErrorGeometryCreated() )
               {
                 Feature * pFeature = pError->getFeature();
                 if ( pFeature != NULL )
                  pFeature->deleteCityGMLGeometryRepresentation ( "Geometriefehler" );
               }

             }
             Close();
           }

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Baumdarstellung der Geometriefehler                    //
///////////////////////////////////////////////////////////////////////////////
  private: System::Void initialize()
           {
             set<string>             sFeatureClassNames;
             set<Feature*>           sFeatures;
             set<string>::iterator   iterNames;
             set<Feature*>::iterator iterFeatures;
             vector<GeometryError*>  vGeometryErrors;
             String                ^ nodeText;
             TreeNode              ^ pTreeNode;
             TreeNode              ^ pChildNode1Level;
             TreeNode              ^ pChildNode2Level;
						 size_t                  errorFeatureAnz, errorAnz, i;

             sFeatureClassNames = m_pErrorReport->getGeometryErrorParentFeatureClasses();

             for ( iterNames = sFeatureClassNames.begin(); iterNames != sFeatureClassNames.end(); iterNames++ )
             {
               string className = *iterNames;
               sFeatures.clear();

               errorFeatureAnz = m_pErrorReport->getGeometryErrorParentFeatures ( className, sFeatures );

               nodeText = String::Concat ( QuConvert::ToString( className ), " - ", Convert::ToString( (int)errorFeatureAnz ), " fehlerhafte Objekte" );
               pTreeNode = gcnew TreeNode ( nodeText );
               pTreeNode->Tag = QuConvert::ToString( className );
               treeView1->Nodes->Add( pTreeNode );
               
               for (iterFeatures = sFeatures.begin(); iterFeatures != sFeatures.end(); iterFeatures++ )
               {
                 Feature * pFeature = *iterFeatures;
                 vGeometryErrors.clear();

                 errorAnz = m_pErrorReport->getGeometryErrors ( pFeature, vGeometryErrors );

                 nodeText = String::Concat(  QuConvert::ToString( className ), " ", QuConvert::ToString( pFeature->getGmlId() ), "; ",
                                             Convert::ToString ( errorAnz ), " Geometriefehler" );
                 pChildNode1Level = gcnew TreeNode ( nodeText );
                 pChildNode1Level->Tag = QuConvert::ToString( pFeature->getGmlId() );
                 pTreeNode->Nodes->Add( pChildNode1Level );

                 for ( i = 0; i < errorAnz; i++ )
                 {
                   GeometryError * pGeometryError = vGeometryErrors[i];

                   nodeText = String::Concat ( QuConvert::ToString ( pGeometryError->getGeometryErrorTypeAsString() ) );
                   pChildNode2Level = gcnew TreeNode ( nodeText );
                   pChildNode2Level->Tag = Convert::ToString( pGeometryError->getErrorNumber() );
                   pChildNode1Level->Nodes->Add( pChildNode2Level );
                 }
               }
             }
           }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl eines bestimmten Fehlers im Baum                 //
//  Bestimmt das zugehörige Objekt GeometryError und schreibt bestimmte      //
//  Fehler-Informationen in das Anzeigefeld                                  //
///////////////////////////////////////////////////////////////////////////////
private: System::Void treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
         {
             m_selectedNode = treeView1->SelectedNode;
             textBoxErrorDetails->Text = "";

             if ( m_selectedNode->Level < 2 )
             {
               detailsToolStripMenuItem->Enabled = false;
             }
             else
             {
               String ^ STR = dynamic_cast<String^>(m_selectedNode->Tag);
               int errorNumber = Convert::ToInt32( STR );
               GeometryError * pError = m_pErrorReport->getGeometryError ( errorNumber );
               if ( pError != NULL )
               {                 
                 if ( pError->getGeometryErrorType() == GeometryError::POLYGON_NON_PLANAR )
                 {
                   double distMax, distAvarage;

                   pError->getPlanaritaetsfehler(distMax, distAvarage );
                   String ^ STR = String::Concat( "Planaritätsfehler: ", QuConvert::ToString( distMax, 3 ) );
                   textBoxErrorDetails->Text = STR;
                 }
                 else
                 if ( pError->getErrorDescription() != "" )
                 {
                   string str = pError->getErrorDescription();
                   textBoxErrorDetails->Text = QuConvert::ToString( str );
                 }
               }              
               detailsToolStripMenuItem->Enabled = true;
             }
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl von Details im Kontext-Menü                      //
//  Öffnet den GeometryErrorDetails Dialog für den ausgewählten Fehler       //
///////////////////////////////////////////////////////////////////////////////
private: System::Void detailsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           if ( m_selectedNode->Level <= 1 )
             return;

           String ^ STR = dynamic_cast<String^>(m_selectedNode->Tag);
           int errorNumber = Convert::ToInt32( STR );
           GeometryError * pError = m_pErrorReport->getGeometryError ( errorNumber );
           GeometryError::GEOMETRY_ERROR_TYPE errType = pError->getGeometryErrorType();

           GeometryErrorDetails ^ pDialog = gcnew GeometryErrorDetails ( m_pFeatures, m_pChecking, m_KITViewer, 
                         errType, errorNumber );
           pDialog->ShowDialog();
         }

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt die Fehlergeometrie für pFeature                                 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void generateErrorGeometry( Feature * pFeature )
         {
           vector<GeometryError*> vGeometryErrors;
           unsigned int           i;

           m_pErrorReport->getGeometryErrors( pFeature, vGeometryErrors );

           for ( i = 0; i < vGeometryErrors.size(); i++ )
           {
             GeometryError * pError = vGeometryErrors[i];
             Feature * pFeatureLoc = pError->getFeature();
						 size_t geoAnz =  pError->getErrorGeometryAnz();
             if ( geoAnz > 0 && pError->getErrorGeometryCreated() == false )
             {
               string errorText =  pError->getGeometryErrorTypeAsString();
               if ( pError->getGeometryErrorType() == GeometryError::POLYGON_NON_PLANAR )
               {
                 double distMax, distAvarage;
                 String ^ STR;
                 string str;

                 pError->getPlanaritaetsfehler( distMax, distAvarage );
                 STR = Convert::ToString( distMax );
                 QuConvert::systemStr2stdStr( str, STR );
                 errorText = errorText + ": " + str;                
               }
               for ( int j = 0; j < geoAnz; j++ )
                 pFeatureLoc->addCityGMLGeometryRepresentationWithContext( pError->getErrorGeometry( j ), GmlNamespaces::cityGml_2_0_georepNamespaceKuerzel,
                          "Geometriefehler",errorText );               
               pError->setErrorGeometryCreated();
             }
           }
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Alle Fehler zeigen"                               //
//  Generiert ein GML-Modell mit allen Fehlergeometrien sowie den zugehörigen//
//  Features, und visualisiert dies Modell mit dem generischen GML-Viewer    //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonZeigen_Click(System::Object^  sender, System::EventArgs^  e)
         {
           set<string>             sFeatureClassNames;
           set<Feature*>           sFeatures;
           set<string>::iterator   iterNames;
           set<Feature*>::iterator iterFeatures;
           vector<GeometryError*>  vGeometryErrors;
           vector<Feature*>        vFeatures;
           string                  fileName;
					 size_t                  errorFeatureAnz, errorAnz, i, geoAnz;

           sFeatureClassNames = m_pErrorReport->getGeometryErrorParentFeatureClasses();

           for ( iterNames = sFeatureClassNames.begin(); iterNames != sFeatureClassNames.end(); iterNames++ )
           {
             string className = *iterNames;
             sFeatures.clear();

             errorFeatureAnz = m_pErrorReport->getGeometryErrorParentFeatures ( className, sFeatures );
             for (iterFeatures = sFeatures.begin(); iterFeatures != sFeatures.end(); iterFeatures++ )
             {
               Feature * pFeature = *iterFeatures;
               vGeometryErrors.clear();

               errorAnz = m_pErrorReport->getGeometryErrors ( pFeature, vGeometryErrors );
               for ( i = 0; i < errorAnz; i++ )
               {
                 GeometryError * pGeometryError = vGeometryErrors[i];
                 GeometryError::GEOMETRY_ERROR_TYPE errType = pGeometryError->getGeometryErrorType();

                 geoAnz =  pGeometryError->getErrorGeometryAnz();
                 if ( geoAnz > 0 && pGeometryError->getErrorGeometryCreated() == false )
                 {
                   for ( unsigned int j = 0; j < geoAnz; j++ )
                     pFeature->addCityGMLGeometryRepresentationWithContext( pGeometryError->getErrorGeometry( j ), GmlNamespaces::cityGml_2_0_georepNamespaceKuerzel,
                     "Geometriefehler", GeometryError::getGeometryErrorTypeAsString ( errType ));

                   vFeatures.push_back( pFeature );
                 }
                 pGeometryError->setErrorGeometryCreated();
               }
             }
           }

           fileName = Constant::getSystemFolder() + "TempObj.gml";

           GMLWriter ^ pWriter = gcnew GMLWriter ( m_pFeatures );
           pWriter->setWriteXLinks( false );
           pWriter->writeSingleFeatures ( fileName, 0,  vFeatures );
           delete pWriter; 

           try
           {
             System::Diagnostics::Process          ^ pProcess = gcnew System::Diagnostics::Process();
             System::Diagnostics::ProcessStartInfo ^ processStartInfo = gcnew System::Diagnostics::ProcessStartInfo;

             processStartInfo->FileName =  m_KITViewer;
             processStartInfo->Arguments = String::Concat ( "\"", QuConvert::ToString ( fileName ), "\"" );
             pProcess->StartInfo = processStartInfo;
             pProcess->Start();
           }

           catch ( Exception ^ )
           {
             MessageBox::Show ( "GML-Viewer kann nicht gestartet werden");
           }

         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Alle Fehler exportieren"                          //
//  Generiert ein GML-Modell mit allen Fehlergeometrien sowie den zugehörigen//
//  Features, und exportiert dies Modell unter einem benutzerdefiniertem     //
//  Namen                                                                    //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonExport_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           set<string>             sFeatureClassNames;
           set<Feature*>           sFeatures;
           set<string>::iterator   iterNames;
           set<Feature*>::iterator iterFeatures;
           vector<GeometryError*>  vGeometryErrors;
           vector<Feature*>        vFeatures;
           string                  fileName;
					 size_t                  errorFeatureAnz, errorAnz, i, geoAnz;

           sFeatureClassNames = m_pErrorReport->getGeometryErrorParentFeatureClasses();

           for ( iterNames = sFeatureClassNames.begin(); iterNames != sFeatureClassNames.end(); iterNames++ )
           {
             string className = *iterNames;
             sFeatures.clear();

             errorFeatureAnz = m_pErrorReport->getGeometryErrorParentFeatures ( className, sFeatures );
             for (iterFeatures = sFeatures.begin(); iterFeatures != sFeatures.end(); iterFeatures++ )
             {
               Feature * pFeature = *iterFeatures;
							 vFeatures.push_back(pFeature);
               vGeometryErrors.clear();

               errorAnz = m_pErrorReport->getGeometryErrors ( pFeature, vGeometryErrors );
               for ( i = 0; i < errorAnz; i++ )
               {
                 GeometryError * pGeometryError = vGeometryErrors[i];
                 GeometryError::GEOMETRY_ERROR_TYPE errType = pGeometryError->getGeometryErrorType();

                 geoAnz =  pGeometryError->getErrorGeometryAnz();
                 if ( geoAnz > 0 && pGeometryError->getErrorGeometryCreated() == false )
                 {
                   for ( unsigned int j = 0; j < geoAnz; j++ )
                     pFeature->addCityGMLGeometryRepresentationWithContext( pGeometryError->getErrorGeometry( j ), GmlNamespaces::cityGml_2_0_georepNamespaceKuerzel,
                     "Geometriefehler", GeometryError::getGeometryErrorTypeAsString ( errType ));                  
                 }
                 pGeometryError->setErrorGeometryCreated();
               }
             }
           }

           if ( saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )
           {
             QuConvert::systemStr2stdStr( fileName, saveFileDialog->FileName );

             GMLWriter ^ pWriter = gcnew GMLWriter ( m_pFeatures );
             pWriter->setWriteXLinks ( false );
             pWriter->writeSingleFeatures ( fileName, 0,  vFeatures );
             delete pWriter;             
           }
         }
};
