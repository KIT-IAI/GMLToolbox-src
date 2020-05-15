#pragma once
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

#include "Feature.h"
#include "Convert.h"
#include "constant.h"
#include "Geometrie.h"
#include "OSM.h"
#include "OSM_Export.h"
//#include "GmlNamespaces.h"
#include "shpfilereader.h"
#include "shptypeenum.h"

	/// <summary>
	/// Zusammenfassung für ShapefileExport
	/// </summary>
	public ref class ShapefileExport : public System::Windows::Forms::Form
	{
	public:
		ShapefileExport( OSM_Model * pOSMModel, String ^ osmFilepath )
		{
      vector<string> vIds;
			
      InitializeComponent();

      m_pOSMModel = pOSMModel;

      pShapefileExport = new OSM_ShapefileAttributesSchemata ();
      string fileName = Constant::getSystemFolder() + Constant::getSchemaPath( OSM ) + "Export\\ShapefileExport.xml";
      pShapefileExport->readSchemata( fileName );

      pShapefileExport->getSchemaIds ( vIds );
      for ( unsigned int i = 0; i < vIds.size(); i++ )
      {
        String ^ STR = QuConvert::ToString( vIds[i] );
        comboBoxSachdaten->Items->Add ( STR );
      }

      if ( vIds.size() > 0 )
        comboBoxSachdaten->SelectedIndex = 0;

      textBoxOrdner->Text = osmFilepath;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~ShapefileExport()
		{
			if (components)
			{
				delete components;
			}
      delete pShapefileExport;
		}

  protected: 
    OSM_Model                       * m_pOSMModel;
    OSM_ShapefileAttributesSchemata * pShapefileExport;
    OSM_ShapefileAttributesSchema   * pShapefileAttributeSchema;

  private: System::Windows::Forms::Button^  buttonExport;
  private: System::Windows::Forms::TextBox^  textBoxOrdner;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Button^  buttonOrdnerWählen;
  private: System::Windows::Forms::TextBox^  textBoxName;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog;
  private: System::Windows::Forms::CheckBox^  checkBoxNodes;
  private: System::Windows::Forms::CheckBox^  checkBoxWays;
  private: System::Windows::Forms::CheckBox^  checkBoxAreas;
  private: System::Windows::Forms::CheckBox^  checkBoxRelations;
  private: System::Windows::Forms::ComboBox^  comboBoxSachdaten;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::Button^  buttonNodeAttributes;
  private: System::Windows::Forms::Button^  buttonWaysExportieren;
  private: System::Windows::Forms::Button^  buttonAreasExportieren;
  private: System::Windows::Forms::Button^  buttonRelationenExportieren;
  private: System::Windows::Forms::Button^  buttonAbbrechen;

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
      this->buttonExport = (gcnew System::Windows::Forms::Button());
      this->textBoxOrdner = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->buttonOrdnerWählen = (gcnew System::Windows::Forms::Button());
      this->textBoxName = (gcnew System::Windows::Forms::TextBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->folderBrowserDialog = (gcnew System::Windows::Forms::FolderBrowserDialog());
      this->checkBoxNodes = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxWays = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxAreas = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxRelations = (gcnew System::Windows::Forms::CheckBox());
      this->comboBoxSachdaten = (gcnew System::Windows::Forms::ComboBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->buttonNodeAttributes = (gcnew System::Windows::Forms::Button());
      this->buttonWaysExportieren = (gcnew System::Windows::Forms::Button());
      this->buttonAreasExportieren = (gcnew System::Windows::Forms::Button());
      this->buttonRelationenExportieren = (gcnew System::Windows::Forms::Button());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // buttonExport
      // 
      this->buttonExport->Location = System::Drawing::Point(299, 295);
      this->buttonExport->Name = L"buttonExport";
      this->buttonExport->Size = System::Drawing::Size(75, 53);
      this->buttonExport->TabIndex = 0;
      this->buttonExport->Text = L"Shapefiles generieren";
      this->buttonExport->UseVisualStyleBackColor = true;
      this->buttonExport->Click += gcnew System::EventHandler(this, &ShapefileExport::buttonExportieren_Click);
      // 
      // textBoxOrdner
      // 
      this->textBoxOrdner->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxOrdner->Location = System::Drawing::Point(13, 22);
      this->textBoxOrdner->Name = L"textBoxOrdner";
      this->textBoxOrdner->ReadOnly = true;
      this->textBoxOrdner->Size = System::Drawing::Size(280, 20);
      this->textBoxOrdner->TabIndex = 1;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(13, 43);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(86, 13);
      this->label1->TabIndex = 2;
      this->label1->Text = L"Shapefile-Ordner";
      // 
      // buttonOrdnerWählen
      // 
      this->buttonOrdnerWählen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->buttonOrdnerWählen->Location = System::Drawing::Point(299, 22);
      this->buttonOrdnerWählen->Name = L"buttonOrdnerWählen";
      this->buttonOrdnerWählen->Size = System::Drawing::Size(75, 20);
      this->buttonOrdnerWählen->TabIndex = 3;
      this->buttonOrdnerWählen->Text = L"Auswählen";
      this->buttonOrdnerWählen->UseVisualStyleBackColor = true;
      this->buttonOrdnerWählen->Click += gcnew System::EventHandler(this, &ShapefileExport::buttonOrdnerWählen_Click);
      // 
      // textBoxName
      // 
      this->textBoxName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxName->Location = System::Drawing::Point(129, 69);
      this->textBoxName->Name = L"textBoxName";
      this->textBoxName->Size = System::Drawing::Size(247, 20);
      this->textBoxName->TabIndex = 4;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(13, 72);
      this->label2->Name = L"label2";
      this->label2->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
      this->label2->Size = System::Drawing::Size(110, 13);
      this->label2->TabIndex = 5;
      this->label2->Text = L"Basisname Shapefiles";
      // 
      // folderBrowserDialog
      // 
      this->folderBrowserDialog->RootFolder = System::Environment::SpecialFolder::DesktopDirectory;
      // 
      // checkBoxNodes
      // 
      this->checkBoxNodes->AutoSize = true;
      this->checkBoxNodes->Location = System::Drawing::Point(13, 146);
      this->checkBoxNodes->Name = L"checkBoxNodes";
      this->checkBoxNodes->Size = System::Drawing::Size(144, 17);
      this->checkBoxNodes->TabIndex = 6;
      this->checkBoxNodes->Text = L"Punktobjekte exportieren";
      this->checkBoxNodes->UseVisualStyleBackColor = true;
      // 
      // checkBoxWays
      // 
      this->checkBoxWays->AutoSize = true;
      this->checkBoxWays->Location = System::Drawing::Point(12, 178);
      this->checkBoxWays->Name = L"checkBoxWays";
      this->checkBoxWays->Size = System::Drawing::Size(144, 17);
      this->checkBoxWays->TabIndex = 7;
      this->checkBoxWays->Text = L"Linienobjekte exportieren";
      this->checkBoxWays->UseVisualStyleBackColor = true;
      // 
      // checkBoxAreas
      // 
      this->checkBoxAreas->AutoSize = true;
      this->checkBoxAreas->Location = System::Drawing::Point(12, 210);
      this->checkBoxAreas->Name = L"checkBoxAreas";
      this->checkBoxAreas->Size = System::Drawing::Size(154, 17);
      this->checkBoxAreas->TabIndex = 8;
      this->checkBoxAreas->Text = L"Flächenobjekte exportieren";
      this->checkBoxAreas->UseVisualStyleBackColor = true;
      // 
      // checkBoxRelations
      // 
      this->checkBoxRelations->AutoSize = true;
      this->checkBoxRelations->Location = System::Drawing::Point(13, 242);
      this->checkBoxRelations->Name = L"checkBoxRelations";
      this->checkBoxRelations->Size = System::Drawing::Size(132, 17);
      this->checkBoxRelations->TabIndex = 9;
      this->checkBoxRelations->Text = L"Relationen exportieren";
      this->checkBoxRelations->UseVisualStyleBackColor = true;
      // 
      // comboBoxSachdaten
      // 
      this->comboBoxSachdaten->FormattingEnabled = true;
      this->comboBoxSachdaten->Location = System::Drawing::Point(129, 105);
      this->comboBoxSachdaten->Name = L"comboBoxSachdaten";
      this->comboBoxSachdaten->Size = System::Drawing::Size(245, 21);
      this->comboBoxSachdaten->TabIndex = 10;
      this->comboBoxSachdaten->SelectedIndexChanged += gcnew System::EventHandler(this, &ShapefileExport::comboBoxSachdaten_SelectedIndexChanged);
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(13, 112);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(101, 13);
      this->label3->TabIndex = 11;
      this->label3->Text = L"Sachdaten-Schema";
      // 
      // buttonNodeAttributes
      // 
      this->buttonNodeAttributes->Location = System::Drawing::Point(186, 142);
      this->buttonNodeAttributes->Name = L"buttonNodeAttributes";
      this->buttonNodeAttributes->Size = System::Drawing::Size(75, 23);
      this->buttonNodeAttributes->TabIndex = 12;
      this->buttonNodeAttributes->Text = L"Sachdaten";
      this->buttonNodeAttributes->UseVisualStyleBackColor = true;
      this->buttonNodeAttributes->Click += gcnew System::EventHandler(this, &ShapefileExport::buttonNodeAttributes_Click);
      // 
      // buttonWaysExportieren
      // 
      this->buttonWaysExportieren->Location = System::Drawing::Point(186, 174);
      this->buttonWaysExportieren->Name = L"buttonWaysExportieren";
      this->buttonWaysExportieren->Size = System::Drawing::Size(75, 23);
      this->buttonWaysExportieren->TabIndex = 13;
      this->buttonWaysExportieren->Text = L"Sachdaten";
      this->buttonWaysExportieren->UseVisualStyleBackColor = true;
      this->buttonWaysExportieren->Click += gcnew System::EventHandler(this, &ShapefileExport::buttonWaysExportieren_Click);
      // 
      // buttonAreasExportieren
      // 
      this->buttonAreasExportieren->Location = System::Drawing::Point(186, 207);
      this->buttonAreasExportieren->Name = L"buttonAreasExportieren";
      this->buttonAreasExportieren->Size = System::Drawing::Size(75, 23);
      this->buttonAreasExportieren->TabIndex = 14;
      this->buttonAreasExportieren->Text = L"Sachdaten";
      this->buttonAreasExportieren->UseVisualStyleBackColor = true;
      this->buttonAreasExportieren->Click += gcnew System::EventHandler(this, &ShapefileExport::buttonAreasExportieren_Click);
      // 
      // buttonRelationenExportieren
      // 
      this->buttonRelationenExportieren->Location = System::Drawing::Point(186, 240);
      this->buttonRelationenExportieren->Name = L"buttonRelationenExportieren";
      this->buttonRelationenExportieren->Size = System::Drawing::Size(75, 23);
      this->buttonRelationenExportieren->TabIndex = 15;
      this->buttonRelationenExportieren->Text = L"Sachdaten";
      this->buttonRelationenExportieren->UseVisualStyleBackColor = true;
      this->buttonRelationenExportieren->Click += gcnew System::EventHandler(this, &ShapefileExport::buttonRelationenExportieren_Click);
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->Location = System::Drawing::Point(16, 324);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 23);
      this->buttonAbbrechen->TabIndex = 16;
      this->buttonAbbrechen->Text = L"Abbrechen";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &ShapefileExport::buttonAbbrechen_Click);
      // 
      // ShapefileExport
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(386, 360);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->buttonRelationenExportieren);
      this->Controls->Add(this->buttonAreasExportieren);
      this->Controls->Add(this->buttonWaysExportieren);
      this->Controls->Add(this->buttonNodeAttributes);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->comboBoxSachdaten);
      this->Controls->Add(this->checkBoxRelations);
      this->Controls->Add(this->checkBoxAreas);
      this->Controls->Add(this->checkBoxWays);
      this->Controls->Add(this->checkBoxNodes);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->textBoxName);
      this->Controls->Add(this->buttonOrdnerWählen);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->textBoxOrdner);
      this->Controls->Add(this->buttonExport);
      this->Name = L"ShapefileExport";
      this->Text = L"OSM-Daten als Shapefile exportieren";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  private: System::Void buttonOrdnerWählen_Click(System::Object^  sender, System::EventArgs^  e)
           {
             System::Windows::Forms::DialogResult ok = folderBrowserDialog->ShowDialog();
             if ( ok == System::Windows::Forms::DialogResult::OK )
             {
               textBoxOrdner->Text = folderBrowserDialog->SelectedPath;
             }
           }        

private: System::Void buttonExportieren_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           std::vector<std::string>  vFieldNames;
           std::vector<DBFFieldType> vFieldTypes;

           if ( String::IsNullOrWhiteSpace ( textBoxName->Text )  )
           {
             System::Windows::Forms::MessageBox::Show( "Bitte den Basisnamen der Shapefiles angeben" ) ;
             return;
           }

           if ( String::IsNullOrWhiteSpace ( textBoxOrdner->Text ) )
           {
             System::Windows::Forms::MessageBox::Show( "Bitte den Ausgabeordner der Shapefiles angeben" ) ;
             return;
           }


           //  Test
           vFieldNames.push_back( "STRTEST" );
           vFieldTypes.push_back( FTString );

           if ( checkBoxNodes->Checked )
             writeNodes( vFieldNames, vFieldTypes );

           if ( checkBoxWays->Checked )
             writeWays ( vFieldNames, vFieldTypes );

           if ( checkBoxAreas->Checked )
             writeAreas ( vFieldNames, vFieldTypes );
         }

private: System::Void writeNodes ( vector<string> &vFieldNames, vector<DBFFieldType> &vFieldTypes )
         {
           string        fileName;
           String      ^ shapefileBasename = String::Concat( textBoxOrdner->Text,"\\",  textBoxName->Text, "_nodes"  );
           vector<void*> vValues;

           QuConvert::systemStr2stdStr( fileName, shapefileBasename );

           char strAtribute[] = "ABC";
           vValues.push_back( strAtribute );

           SHPFileReaderWriter * pShapefileWriter = new SHPFileReaderWriter ( NULL, fileName );

           pShapefileWriter->createShapefile( SHPT_POINT, vFieldNames, vFieldTypes );

           OSM_Node * pNode   = m_pOSMModel->getFirstOSM_Node();
           int nShapeId       = 0;
           int nNodesWritten  = 0;

           while ( pNode != NULL )
           {
             GeoPoint p;
             if ( pNode->getTagAnz() > 0 )
             {
               p.set(  pNode->getLon(), pNode->getLat() );
               pShapefileWriter->writeShapefilePoint( nShapeId, &p );
               pShapefileWriter->writeShapefileAttributes( nNodesWritten, vValues  );
               nNodesWritten++;
             }
             pNode = m_pOSMModel->getNextOSM_Node();
             nShapeId++;
           }

           pShapefileWriter->closeShapefile();
         }

private: System::Void writeWays ( vector<string> &vFieldNames, vector<DBFFieldType> &vFieldTypes )
         {


         }

private: System::Void writeAreas ( vector<string> &vFieldNames, vector<DBFFieldType> &vFieldTypes )
         {


         }


private: System::Void comboBoxSachdaten_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
         {
           string id;

           String ^ ID = dynamic_cast<String^>(comboBoxSachdaten->SelectedItem);
           QuConvert::systemStr2stdStr( id, ID );

           pShapefileAttributeSchema = pShapefileExport->getSchema( id );
         }
private: System::Void buttonNodeAttributes_Click(System::Object^  sender, System::EventArgs^  e)
         {

         }

private: System::Void buttonWaysExportieren_Click(System::Object^  sender, System::EventArgs^  e) 
         {

         }
private: System::Void buttonAreasExportieren_Click(System::Object^  sender, System::EventArgs^  e) 
         {
         }

private: System::Void buttonRelationenExportieren_Click(System::Object^  sender, System::EventArgs^  e) 
         {
         }

private: System::Void buttonAbbrechen_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           Close();
         }
};
