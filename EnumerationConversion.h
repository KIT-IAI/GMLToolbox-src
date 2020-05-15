#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Convert.h"
#include "ShpFileObjekte.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "GmlSchema.h"
#include "ExternalCodeLists.h"

using namespace std;

	/// <summary> 
	/// Konvertiert Werte eines Shapefile-Attributs in die Codes einer GML-Enumeration
	///
	/// </summary>
	public ref class EnumerationConversion : public System::Windows::Forms::Form
	{
	public: 
    EnumerationConversion( Features * pFeatures, ShapeFileObjekt * pShapeFileObjektP, GmlEnumeration * pGmlEnumerationP,
							   ShapeFileAttribut * pShapeFileAttributP,  GmlDictionary * pCodeListsP )
		{
			string stdString;

			InitializeComponent();

			m_pFeatures        = pFeatures;
      m_pGmlObjekte      = m_pFeatures->getGmlSchema();
			pCodeLists         = pCodeListsP;
			pCodeList          = NULL;
			pGmlEnumeration    = pGmlEnumerationP;
			pShapeFileAttribut = pShapeFileAttributP;
			
			stdString = pShapeFileObjektP->getShapeFileName();
			shapeFileName = QuConvert::ToString ( stdString );

			initialize();
		}      

	private:
		Features           * m_pFeatures;						// Modul Features
		GmlDictionary	     * pCodeLists;						// Aggregation der Codelisten
		GmlDictionaryEntry * pCodeList;							// Einzelne Codeliste
		String             ^ shapeFileName;					// Name des zu konvertierenden Shapefiles
		GmlEnumeration     * pGmlEnumeration;				// GML-Enumeration, in die konvertiert werden soll
		ShapeFileAttribut  * pShapeFileAttribut;		// Zu konvertierendes Attribut des Shapefiels
    GmlSchema          * m_pGmlObjekte;					// XPlanGML Schema
		int                  attributWertIndex;			// Index des GML-Attributwertes
    GmlDictionaryReader ^ pReader;							// XML-Reader für die Codelisten-Dictionaries
    bool                localCodelist;					// Gibt an, ob die Codelisten lokal gespeichert werden
		DataTable					^ shapeAttributeTable;		// Zuordnungstabelle

	private: System::Windows::Forms::Label ^  labelShapefile;
	private: System::Windows::Forms::TextBox ^  textBoxShapefile;
	private: System::Windows::Forms::Label   ^ labelAttribut;
	private: System::Windows::Forms::TextBox ^ textBoxAttribut;
	private: System::Windows::Forms::Label   ^ labelGML_Enumeration;
	private: System::Windows::Forms::TextBox  ^ textBoxGML_Enumeration;
	private: System::Windows::Forms::DataGrid ^ dataGridShapeAttribute;
	private: System::Windows::Forms::Button ^  buttonBeenden;
	private: System::Windows::Forms::ComboBox ^  comboBoxEnumerationItems;
  private: System::Windows::Forms::TextBox^  textBoxDictionary;
  private: System::Windows::Forms::Label^  labelDictionary;


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
      this->labelShapefile = (gcnew System::Windows::Forms::Label());
      this->textBoxShapefile = (gcnew System::Windows::Forms::TextBox());
      this->labelAttribut = (gcnew System::Windows::Forms::Label());
      this->textBoxAttribut = (gcnew System::Windows::Forms::TextBox());
      this->labelGML_Enumeration = (gcnew System::Windows::Forms::Label());
      this->textBoxGML_Enumeration = (gcnew System::Windows::Forms::TextBox());
      this->dataGridShapeAttribute = (gcnew System::Windows::Forms::DataGrid());
      this->comboBoxEnumerationItems = (gcnew System::Windows::Forms::ComboBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->textBoxDictionary = (gcnew System::Windows::Forms::TextBox());
      this->labelDictionary = (gcnew System::Windows::Forms::Label());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridShapeAttribute))->BeginInit();
      this->SuspendLayout();
      // 
      // labelShapefile
      // 
      this->labelShapefile->Location = System::Drawing::Point(8, 13);
      this->labelShapefile->Name = L"labelShapefile";
      this->labelShapefile->Size = System::Drawing::Size(64, 16);
      this->labelShapefile->TabIndex = 0;
      this->labelShapefile->Text = L"Shapefile";
      this->labelShapefile->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
      // 
      // textBoxShapefile
      // 
      this->textBoxShapefile->Location = System::Drawing::Point(65, 12);
      this->textBoxShapefile->Name = L"textBoxShapefile";
      this->textBoxShapefile->ReadOnly = true;
      this->textBoxShapefile->Size = System::Drawing::Size(176, 20);
      this->textBoxShapefile->TabIndex = 1;
      // 
      // labelAttribut
      // 
      this->labelAttribut->Location = System::Drawing::Point(250, 16);
      this->labelAttribut->Name = L"labelAttribut";
      this->labelAttribut->Size = System::Drawing::Size(40, 16);
      this->labelAttribut->TabIndex = 2;
      this->labelAttribut->Text = L"Attribut";
      this->labelAttribut->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
      // 
      // textBoxAttribut
      // 
      this->textBoxAttribut->Location = System::Drawing::Point(296, 12);
      this->textBoxAttribut->Name = L"textBoxAttribut";
      this->textBoxAttribut->ReadOnly = true;
      this->textBoxAttribut->Size = System::Drawing::Size(211, 20);
      this->textBoxAttribut->TabIndex = 3;
      // 
      // labelGML_Enumeration
      // 
      this->labelGML_Enumeration->Location = System::Drawing::Point(8, 64);
      this->labelGML_Enumeration->Name = L"labelGML_Enumeration";
      this->labelGML_Enumeration->Size = System::Drawing::Size(100, 16);
      this->labelGML_Enumeration->TabIndex = 4;
      this->labelGML_Enumeration->Text = L"GML-Enumeration";
      this->labelGML_Enumeration->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
      // 
      // textBoxGML_Enumeration
      // 
      this->textBoxGML_Enumeration->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxGML_Enumeration->Location = System::Drawing::Point(104, 64);
      this->textBoxGML_Enumeration->Name = L"textBoxGML_Enumeration";
      this->textBoxGML_Enumeration->ReadOnly = true;
      this->textBoxGML_Enumeration->Size = System::Drawing::Size(403, 20);
      this->textBoxGML_Enumeration->TabIndex = 5;
      // 
      // dataGridShapeAttribute
      // 
      this->dataGridShapeAttribute->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left));
      this->dataGridShapeAttribute->CaptionBackColor = System::Drawing::SystemColors::InactiveBorder;
      this->dataGridShapeAttribute->CaptionFont = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->dataGridShapeAttribute->CaptionForeColor = System::Drawing::SystemColors::ControlText;
      this->dataGridShapeAttribute->CaptionText = L"Shape-Attribut Wert - GM Enumerations-Item";
      this->dataGridShapeAttribute->ColumnHeadersVisible = false;
      this->dataGridShapeAttribute->DataMember = L"";
      this->dataGridShapeAttribute->ForeColor = System::Drawing::SystemColors::WindowFrame;
      this->dataGridShapeAttribute->HeaderForeColor = System::Drawing::SystemColors::ControlText;
      this->dataGridShapeAttribute->LinkColor = System::Drawing::SystemColors::ActiveCaption;
      this->dataGridShapeAttribute->Location = System::Drawing::Point(11, 146);
      this->dataGridShapeAttribute->Name = L"dataGridShapeAttribute";
      this->dataGridShapeAttribute->PreferredColumnWidth = 245;
      this->dataGridShapeAttribute->ReadOnly = true;
      this->dataGridShapeAttribute->RowHeadersVisible = false;
      this->dataGridShapeAttribute->SelectionBackColor = System::Drawing::SystemColors::Highlight;
      this->dataGridShapeAttribute->Size = System::Drawing::Size(248, 183);
      this->dataGridShapeAttribute->TabIndex = 6;
      this->dataGridShapeAttribute->CurrentCellChanged += gcnew System::EventHandler(this, &EnumerationConversion::dataGridShapeAttribute_CurrentCellChanged);
      // 
      // comboBoxEnumerationItems
      // 
      this->comboBoxEnumerationItems->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->comboBoxEnumerationItems->Location = System::Drawing::Point(265, 146);
      this->comboBoxEnumerationItems->Name = L"comboBoxEnumerationItems";
      this->comboBoxEnumerationItems->Size = System::Drawing::Size(241, 21);
      this->comboBoxEnumerationItems->Sorted = true;
      this->comboBoxEnumerationItems->TabIndex = 8;
      this->comboBoxEnumerationItems->SelectedValueChanged += gcnew System::EventHandler(this, &EnumerationConversion::comboBoxEnumerationItems_SelectedValueChanged);
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonBeenden->Location = System::Drawing::Point(431, 305);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 24);
      this->buttonBeenden->TabIndex = 10;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &EnumerationConversion::buttonBeenden_Click);
      // 
      // textBoxDictionary
      // 
      this->textBoxDictionary->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxDictionary->Location = System::Drawing::Point(11, 90);
      this->textBoxDictionary->Name = L"textBoxDictionary";
      this->textBoxDictionary->Size = System::Drawing::Size(495, 20);
      this->textBoxDictionary->TabIndex = 11;
      this->textBoxDictionary->Leave += gcnew System::EventHandler(this, &EnumerationConversion::textBoxDictionary_Leave);
      // 
      // labelDictionary
      // 
      this->labelDictionary->AutoSize = true;
      this->labelDictionary->Location = System::Drawing::Point(11, 117);
      this->labelDictionary->Name = L"labelDictionary";
      this->labelDictionary->Size = System::Drawing::Size(142, 13);
      this->labelDictionary->TabIndex = 12;
      this->labelDictionary->Text = L"Zugehöriges GML-Dictionary";
      // 
      // EnumerationConversion
      // 
      this->AcceptButton = this->buttonBeenden;
      this->CancelButton = this->buttonBeenden;
      this->ClientSize = System::Drawing::Size(512, 341);
      this->Controls->Add(this->labelDictionary);
      this->Controls->Add(this->textBoxDictionary);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->comboBoxEnumerationItems);
      this->Controls->Add(this->dataGridShapeAttribute);
      this->Controls->Add(this->textBoxGML_Enumeration);
      this->Controls->Add(this->labelGML_Enumeration);
      this->Controls->Add(this->textBoxAttribut);
      this->Controls->Add(this->labelAttribut);
      this->Controls->Add(this->textBoxShapefile);
      this->Controls->Add(this->labelShapefile);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"EnumerationConversion";
      this->Text = L"Konversion einer GML-Enumeration";
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridShapeAttribute))->EndInit();
      this->ResumeLayout(false);
      this->PerformLayout();

    }		

	private: 

///////////////////////////////////////////////////////////////////////////////
//  Liest das Codelisten-Dictionary ein und initialisiert die Dialogbox			 //
///////////////////////////////////////////////////////////////////////////////
		System::Void initialize()
		{
			string       stdString;
			string       schluesselText;
      string       gmlDictionaryName;
      string       gmlDictionaryPfad;
			string       shapeFileAttributWert;
			String     ^ STR;
			size_t       i, anz;

			GmlEnumerationItem    * enumItem;
			ShapeFileAttributWert * pAttributWert;

			System::Data::DataColumn ^ pDataColumnShape;
			System::Data::DataRow    ^ pDataRow;

			vector<ShapeFileAttributWert*> shapeFileAttributWerte;
			vector<GmlEnumerationItem*> gmlEnumerationItems;

      vector<string>              schluesselNummern;
      vector<string>              schluesselTexte;

      pReader = gcnew GmlDictionaryReader ( m_pFeatures->getGmlTyp() );

			textBoxShapefile->Text = shapeFileName;

			stdString = pShapeFileAttribut->getShapeFileAttributName();
			STR = QuConvert::ToString ( stdString );
			textBoxAttribut->Text = STR;

			stdString = pGmlEnumeration->getGmlEnumerationName();
			STR = QuConvert::ToString ( stdString );
			textBoxGML_Enumeration->Text = STR;

      if ( !pGmlEnumeration->getIsExternalCodeList() )
      {
        localCodelist = false;
			  if ( pCodeLists != NULL )
				  pCodeList = pCodeLists->getDictionaryEntry ( stdString );
      }
      else
      {
        if ( m_pFeatures->getGmlTyp() == XPLANGML_2_0 ||  m_pFeatures->getGmlTyp() == XPLANGML_3_0 )
        {
          localCodelist = false;
          if ( pCodeLists != NULL )
            pCodeList = pCodeLists->getDictionaryEntry ( stdString );
        }
        else
        {
          localCodelist = true;
          if ( pCodeLists != NULL )
            pCodeList = pCodeLists->getDictionaryEntry ( stdString );

          gmlDictionaryPfad = pGmlEnumeration->getCodeListDictionary();
          if ( gmlDictionaryPfad != "" )
          {
            pCodeList = pReader->readGmlSingleDictionary ( gmlDictionaryPfad, false );
          }
          else
          {
            gmlDictionaryPfad = m_pGmlObjekte->createExternalCodeListsPath( stdString );
          }

          pGmlEnumeration->setCodeListDictionary( gmlDictionaryPfad );
          pGmlEnumeration->clear();

          pCodeList = pReader->readGmlSingleDictionary ( gmlDictionaryPfad, false );
          if ( pCodeList != NULL )
          {
            anz = pCodeList->getCodelistSchluessel( schluesselNummern );
            for ( i = 0; i < schluesselNummern.size(); i++ )
            {
              GmlEnumerationItem * pGmlEnumerationItem = new GmlEnumerationItem ( schluesselNummern[i] );
              pGmlEnumeration->addGmlEnumerationItem( pGmlEnumerationItem);
            }
          }
        }
        textBoxDictionary->Text = QuConvert::ToString ( gmlDictionaryPfad );
      }

      pReader->closeReader();
      if ( !localCodelist )
        textBoxDictionary->Enabled = false;

			anz = pGmlEnumeration->getEnumerationItems ( gmlEnumerationItems );
			STR = " ";
			comboBoxEnumerationItems->Items->Add ( STR );
			for ( i = 0; i < anz; i++ )
			{
				enumItem = gmlEnumerationItems[i];
				stdString = enumItem->getGmlEnumerationItemName();
				if ( pCodeList != NULL )
				{
					bool success = pCodeList->getSchluesselText ( stdString, schluesselText );
					if ( success && schluesselText != "" ) stdString = schluesselText;
				}
				STR = QuConvert::ToString ( stdString );
				comboBoxEnumerationItems->Items->Add ( STR );
			}

			shapeAttributeTable = gcnew DataTable ( "Shape-Attribut");
			dataGridShapeAttribute->DataSource =  shapeAttributeTable;

			STR = "Shape-Attribut";
			pDataColumnShape= shapeAttributeTable->Columns->Add (STR );
			pDataColumnShape->DataType =  STR->GetType();
			pDataColumnShape->MaxLength = 256;

			anz = pShapeFileAttribut->getShapeFileAttributWerte (shapeFileAttributWerte );
			for ( i = 0; i < anz; i++ )
			{
				pAttributWert         = shapeFileAttributWerte[i];			
				shapeFileAttributWert = pAttributWert->getAttributWert();
				enumItem			  = pAttributWert->getEnumerationItem( pGmlEnumeration );

				pDataRow = shapeAttributeTable->NewRow();
				STR = QuConvert::ToString ( shapeFileAttributWert );

				if ( enumItem != NULL )
				{
					stdString = enumItem->getGmlEnumerationItemName();
					if ( pCodeList != NULL )
					{
						bool success = pCodeList->getSchluesselText ( stdString, schluesselText );
						if ( success && schluesselText != "" )
						{
							STR = String::Concat ( STR, " - ", QuConvert::ToString ( schluesselText ),
													" (", QuConvert::ToString ( stdString ), ")"  );
						}
						else
              STR = String::Concat ( STR, " - ??" );
//						   STR = String::Concat ( STR, " - ", QuConvert::ToString ( stdString ) );
					}
					else
						STR = String::Concat ( STR, " - ", QuConvert::ToString ( stdString ) );
				}
				else
					STR = String::Concat ( STR, " - ??" );

				pDataRow->default[0] = STR;
				shapeAttributeTable->Rows->Add ( pDataRow );
			}

			if ( anz > 0 )
			{
				attributWertIndex = 0;
				pAttributWert = shapeFileAttributWerte[attributWertIndex];
				enumItem			  = pAttributWert->getEnumerationItem( pGmlEnumeration );
				if ( enumItem != NULL )
				{
					stdString = enumItem->getGmlEnumerationItemName();
					if ( pCodeList != NULL )
					{
						bool success = pCodeList->getSchluesselText ( stdString, schluesselText );
						if ( success && schluesselText != "" ) stdString = schluesselText;
					}

					STR = QuConvert::ToString ( stdString );
					comboBoxEnumerationItems->SelectedItem = STR;
				}
				else comboBoxEnumerationItems->SelectedIndex = 0;
			}

		}

//////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl eines Enumerations-Items										    //
//////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxEnumerationItems_SelectedValueChanged(System::Object ^  sender, System::EventArgs ^  e)
		 {
			vector<ShapeFileAttributWert*> shapeFileAttributWerte;
			ShapeFileAttributWert		 * pShapeFileAttributWert;
			GmlEnumerationItem           * pEnumerationItem;
			bool						   success;
			String						 ^ STR;
			String						 ^ STR_ENUM;
			string						   stdString;
			string						   schluesselNummer;

			if ( attributWertIndex < 0 ) return;
	
			pShapeFileAttribut->getShapeFileAttributWerte ( shapeFileAttributWerte );
			pShapeFileAttributWert = shapeFileAttributWerte[attributWertIndex];
			STR = QuConvert::ToString ( pShapeFileAttributWert->getAttributWert() );

			STR_ENUM = dynamic_cast<String^> ( comboBoxEnumerationItems->SelectedItem );
			if ( String::Compare ( STR_ENUM , " " ) == 0 )
			{
				pShapeFileAttributWert->setEnumerationItem ( pGmlEnumeration, NULL );
				STR = String::Concat ( STR, " - ??" );
			}
			else
			{
				QuConvert::systemStr2stdStr ( stdString, STR_ENUM );
				if ( pCodeList != NULL )
				{
					success = pCodeList->getSchluesselNummer ( stdString, schluesselNummer );
					if ( success )
					{
						STR_ENUM = String::Concat ( QuConvert::ToString ( stdString ),
							" (", QuConvert::ToString ( schluesselNummer ), ")"  );
						stdString = schluesselNummer;
					}
				}

				pEnumerationItem = pGmlEnumeration->getEnumerationItem ( stdString );

				pShapeFileAttributWert->setEnumerationItem ( pGmlEnumeration, pEnumerationItem );
				STR = String::Concat ( STR, " - ", STR_ENUM );
			}
			dataGridShapeAttribute->default[attributWertIndex,0] = STR;
		 }

//////////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl einer Zuordnung Shape-Attributwert - Enumerationsitem/
//////////////////////////////////////////////////////////////////////////////////
private: System::Void dataGridShapeAttribute_CurrentCellChanged(System::Object ^  sender, System::EventArgs ^  e)
		 {
			vector<ShapeFileAttributWert*> shapeFileAttributWerte;
			ShapeFileAttributWert		     * pShapeFileAttributWert;
			GmlEnumerationItem           * pEnumerationItem;
			String						           ^ STR;
			string						             stdString;
			string						             schluesselText;
			bool						               success;

			if ( dataGridShapeAttribute->CurrentCell.RowNumber == attributWertIndex ) return;

			pShapeFileAttribut->getShapeFileAttributWerte ( shapeFileAttributWerte );

			attributWertIndex = dataGridShapeAttribute->CurrentCell.RowNumber;
			pShapeFileAttributWert = shapeFileAttributWerte[attributWertIndex];
			pEnumerationItem = pShapeFileAttributWert->getEnumerationItem ( pGmlEnumeration );

			if ( pEnumerationItem == NULL ) comboBoxEnumerationItems->SelectedIndex = 0;
			else
			{
				stdString = pEnumerationItem->getGmlEnumerationItemName();
				if ( pCodeList != NULL )
				{
					success = pCodeList->getSchluesselText ( stdString, schluesselText );
					if ( success && schluesselText != "" ) stdString = schluesselText;
				}
				STR = QuConvert::ToString ( stdString );
				comboBoxEnumerationItems->SelectedItem = STR;
			}

   }


///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"																					 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
{
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Spezifikation eines Codelisten-Dictionaries							 //
///////////////////////////////////////////////////////////////////////////////
private: System::Void textBoxDictionary_Leave(System::Object^  sender, System::EventArgs^  e)
 {
   string                  gmlDictionaryPfad;
   vector<string>          schluesselNummern;
   vector<string>          schluesselTexte;
   string                  stdString;
   string                  schluesselText;
	 size_t                  i, anz;
   String                ^ STR;
   GmlEnumerationItem        * enumItem;
   vector<GmlEnumerationItem*> gmlEnumerationItems;
   vector<GmlDictionaryEntry*> dictionaryEntries;

   if ( !localCodelist ) return;

   pGmlEnumeration->clear();
   pShapeFileAttribut->resetAttributWerte();

   QuConvert::systemStr2stdStr ( gmlDictionaryPfad, textBoxDictionary->Text );
   pCodeList = pReader->readGmlSingleDictionary( gmlDictionaryPfad, false );
   if ( pCodeList != NULL )
   {
     pGmlEnumeration->setCodeListDictionary ( gmlDictionaryPfad );

     anz = pCodeList->getCodelistSchluessel( schluesselNummern );
     for ( i = 0; i < schluesselNummern.size(); i++ )
     {
       GmlEnumerationItem * pGmlEnumerationItem = new GmlEnumerationItem ( schluesselNummern[i] );
       pGmlEnumeration->addGmlEnumerationItem( pGmlEnumerationItem);
     }
   }

   pReader->closeReader();
   comboBoxEnumerationItems->Items->Clear();

   anz = pGmlEnumeration->getEnumerationItems ( gmlEnumerationItems );
   STR = " ";
   comboBoxEnumerationItems->Items->Add ( STR );
   for ( i = 0; i < anz; i++ )
   {
     enumItem = gmlEnumerationItems[i];
     stdString = enumItem->getGmlEnumerationItemName();
     if ( pCodeList != NULL )
     {
       bool success = pCodeList->getSchluesselText ( stdString, schluesselText );
       if ( success && schluesselText != "" ) stdString = schluesselText;
     }
     STR = QuConvert::ToString ( stdString );
     comboBoxEnumerationItems->Items->Add ( STR );
   }
 }

};
