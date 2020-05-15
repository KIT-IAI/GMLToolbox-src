#pragma once

#include "Convert.h"
#include "Constant.h"
#include "GmlSchema.h"
#include "Dictionary.h"
#include "TimeX.h"
#include "UnitOfMeasure.h"
#include "SrsManagerRef.h"
#include "Feature.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

	/// <summary>
	/// Manuelle Erzeugung von Instanzen eines komplexen XPlanGML-Datentyps
	///
	/// </summary>
	public ref class GenerateKomplexAttribute : public System::Windows::Forms::Form
	{
	public:
		GenerateKomplexAttribute( Features * pFeatures, GmlDictionary * pCodeListsP, GmlDictionary * pExternalCodeListsP, 
                              FeatureList * pPlanObjektListP )
		{
			InitializeComponent();

      m_pFeatures          = pFeatures;
      m_pCodeLists         = pCodeListsP;
      m_pPlanObjektList    = pPlanObjektListP;
      m_pGmlSchema         = m_pFeatures->getGmlSchema();
      m_pExternalCodeLists = pExternalCodeListsP;

      pPlanObjektListSave = new FeatureList ( m_pPlanObjektList );

      if ( m_pPlanObjektList->count() > 0 )
      {
        pPlanObjektAktuell = m_pPlanObjektList->get( 0 );
        planObjektAktuellIndex = 1;
      }
      else
      {
        pPlanObjektAktuell = NULL;
        planObjektAktuellIndex = 0;
      }

      pAttributGmlTyp = m_pPlanObjektList->getGmlTyp();

      textBoxKlassenName->Text  = QuConvert::ToString( m_pPlanObjektList->getKlassenName() );
      textBoxRelationsName->Text = QuConvert::ToString( m_pPlanObjektList->getRelationName() );
      textBoxDatentyp->Text      = QuConvert::ToString( m_pPlanObjektList->getGmlTyp()->getGmlKlassenName() );

      attributeTable = gcnew DataTable ( "Komplex Attribut");
      dataGridAttribute->DataSource =  attributeTable;

      generateDataGrid ();
      updateDataGrid();
      updateObjektControl();
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~GenerateKomplexAttribute()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::Label^    label1;
  private: System::Windows::Forms::TextBox^  textBoxKlassenName;
  private: System::Windows::Forms::Label^    label2;
  private: System::Windows::Forms::TextBox^  textBoxRelationsName;
  private: System::Windows::Forms::Label^    label3;
  private: System::Windows::Forms::TextBox^  textBoxDatentyp;
  private: System::Windows::Forms::Button^   buttonNewInstance;
  private: System::Windows::Forms::Button^  buttonNextInstance;
  private: System::Windows::Forms::Button^  buttonPrevInstance;
  private: System::Windows::Forms::Button^  buttonDeleteInstance;
  private: System::Windows::Forms::DataGridView^  dataGridAttribute;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::Label^  label5;
  private: System::Windows::Forms::Label^  label6;
  private: System::Windows::Forms::TextBox^  textBoxSelektiertesAttributName;
  private: System::Windows::Forms::TextBox^  textBoxSelektiertesAttributTyp;
  private: System::Windows::Forms::TextBox^  textBoxSelektiertesAttributWert;
  private: System::Windows::Forms::Button^  buttonAbbrechen;
  private: System::Windows::Forms::Button^  buttonUebernehmen;
  private: System::Windows::Forms::Label^  labelObjektIndex;
  private: System::Windows::Forms::ComboBox^  comboBoxEnumWerte;
	private: System::Windows::Forms::TextBox^  textBoxIndex;
	private: System::Windows::Forms::Label^  label7;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>

    Features       * m_pFeatures;						// Modul Features
    GmlSchema      * m_pGmlSchema;					// XPlanGML-Schema
    GmlDictionary  * m_pCodeLists;					// XPlanGML-Enumerationen
    GmlDictionary  * m_pExternalCodeLists;	// XPlanGML-Codelisten
    FeatureList    * m_pPlanObjektList;			// 

    FeatureList    * pPlanObjektListSave;
    Feature        * pPlanObjektAktuell;
    GmlKlasse      * pAttributGmlTyp;
    int              planObjektAktuellIndex;
    int              selectedRowIndex;
    GmlAttribut    * pGmlAttributAktuell;
    DataTable      ^ attributeTable;

		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxKlassenName = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBoxRelationsName = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBoxDatentyp = (gcnew System::Windows::Forms::TextBox());
			this->buttonNewInstance = (gcnew System::Windows::Forms::Button());
			this->buttonNextInstance = (gcnew System::Windows::Forms::Button());
			this->buttonPrevInstance = (gcnew System::Windows::Forms::Button());
			this->buttonDeleteInstance = (gcnew System::Windows::Forms::Button());
			this->dataGridAttribute = (gcnew System::Windows::Forms::DataGridView());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->textBoxSelektiertesAttributName = (gcnew System::Windows::Forms::TextBox());
			this->textBoxSelektiertesAttributTyp = (gcnew System::Windows::Forms::TextBox());
			this->textBoxSelektiertesAttributWert = (gcnew System::Windows::Forms::TextBox());
			this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
			this->buttonUebernehmen = (gcnew System::Windows::Forms::Button());
			this->labelObjektIndex = (gcnew System::Windows::Forms::Label());
			this->comboBoxEnumWerte = (gcnew System::Windows::Forms::ComboBox());
			this->textBoxIndex = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridAttribute))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 25);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(70, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Klassenname";
			// 
			// textBoxKlassenName
			// 
			this->textBoxKlassenName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxKlassenName->Location = System::Drawing::Point(161, 22);
			this->textBoxKlassenName->Name = L"textBoxKlassenName";
			this->textBoxKlassenName->ReadOnly = true;
			this->textBoxKlassenName->Size = System::Drawing::Size(201, 20);
			this->textBoxKlassenName->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(15, 56);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(124, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Attribut- / Relationsname";
			// 
			// textBoxRelationsName
			// 
			this->textBoxRelationsName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxRelationsName->Location = System::Drawing::Point(161, 48);
			this->textBoxRelationsName->Name = L"textBoxRelationsName";
			this->textBoxRelationsName->ReadOnly = true;
			this->textBoxRelationsName->Size = System::Drawing::Size(201, 20);
			this->textBoxRelationsName->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(15, 81);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(50, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Datentyp";
			// 
			// textBoxDatentyp
			// 
			this->textBoxDatentyp->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxDatentyp->Location = System::Drawing::Point(161, 74);
			this->textBoxDatentyp->Name = L"textBoxDatentyp";
			this->textBoxDatentyp->ReadOnly = true;
			this->textBoxDatentyp->Size = System::Drawing::Size(201, 20);
			this->textBoxDatentyp->TabIndex = 5;
			// 
			// buttonNewInstance
			// 
			this->buttonNewInstance->Location = System::Drawing::Point(161, 130);
			this->buttonNewInstance->Name = L"buttonNewInstance";
			this->buttonNewInstance->Size = System::Drawing::Size(48, 23);
			this->buttonNewInstance->TabIndex = 6;
			this->buttonNewInstance->Text = L"Neu";
			this->buttonNewInstance->UseVisualStyleBackColor = true;
			this->buttonNewInstance->Click += gcnew System::EventHandler(this, &GenerateKomplexAttribute::buttonNewInstance_Click);
			// 
			// buttonNextInstance
			// 
			this->buttonNextInstance->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->buttonNextInstance->Location = System::Drawing::Point(215, 130);
			this->buttonNextInstance->Name = L"buttonNextInstance";
			this->buttonNextInstance->Size = System::Drawing::Size(28, 23);
			this->buttonNextInstance->TabIndex = 7;
			this->buttonNextInstance->Text = L"-->";
			this->buttonNextInstance->UseVisualStyleBackColor = true;
			this->buttonNextInstance->Click += gcnew System::EventHandler(this, &GenerateKomplexAttribute::buttonNextInstance_Click);
			// 
			// buttonPrevInstance
			// 
			this->buttonPrevInstance->Location = System::Drawing::Point(129, 130);
			this->buttonPrevInstance->Name = L"buttonPrevInstance";
			this->buttonPrevInstance->Size = System::Drawing::Size(29, 23);
			this->buttonPrevInstance->TabIndex = 8;
			this->buttonPrevInstance->Text = L"<--";
			this->buttonPrevInstance->UseVisualStyleBackColor = true;
			this->buttonPrevInstance->Click += gcnew System::EventHandler(this, &GenerateKomplexAttribute::buttonPrevInstance_Click);
			// 
			// buttonDeleteInstance
			// 
			this->buttonDeleteInstance->Location = System::Drawing::Point(129, 159);
			this->buttonDeleteInstance->Name = L"buttonDeleteInstance";
			this->buttonDeleteInstance->Size = System::Drawing::Size(114, 23);
			this->buttonDeleteInstance->TabIndex = 9;
			this->buttonDeleteInstance->Text = L"Löschen";
			this->buttonDeleteInstance->UseVisualStyleBackColor = true;
			this->buttonDeleteInstance->Click += gcnew System::EventHandler(this, &GenerateKomplexAttribute::buttonDeleteInstance_Click);
			// 
			// dataGridAttribute
			// 
			this->dataGridAttribute->AllowUserToAddRows = false;
			this->dataGridAttribute->AllowUserToDeleteRows = false;
			this->dataGridAttribute->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridAttribute->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridAttribute->AutoSizeRowsMode = System::Windows::Forms::DataGridViewAutoSizeRowsMode::AllCells;
			this->dataGridAttribute->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridAttribute->ColumnHeadersVisible = false;
			dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Window;
			dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::Desktop;
			dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::ControlLightLight;
			dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
			this->dataGridAttribute->DefaultCellStyle = dataGridViewCellStyle2;
			this->dataGridAttribute->Location = System::Drawing::Point(12, 207);
			this->dataGridAttribute->MultiSelect = false;
			this->dataGridAttribute->Name = L"dataGridAttribute";
			this->dataGridAttribute->ReadOnly = true;
			this->dataGridAttribute->RowHeadersVisible = false;
			this->dataGridAttribute->RowHeadersWidth = 40;
			this->dataGridAttribute->RowTemplate->DefaultCellStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			this->dataGridAttribute->RowTemplate->DefaultCellStyle->BackColor = System::Drawing::Color::Silver;
			this->dataGridAttribute->RowTemplate->DefaultCellStyle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->dataGridAttribute->RowTemplate->DefaultCellStyle->ForeColor = System::Drawing::Color::Black;
			this->dataGridAttribute->RowTemplate->DefaultCellStyle->SelectionBackColor = System::Drawing::Color::Blue;
			this->dataGridAttribute->RowTemplate->DefaultCellStyle->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
			this->dataGridAttribute->RowTemplate->Height = 18;
			this->dataGridAttribute->RowTemplate->ReadOnly = true;
			this->dataGridAttribute->RowTemplate->Resizable = System::Windows::Forms::DataGridViewTriState::True;
			this->dataGridAttribute->Size = System::Drawing::Size(350, 181);
			this->dataGridAttribute->TabIndex = 10;
			this->dataGridAttribute->SelectionChanged += gcnew System::EventHandler(this, &GenerateKomplexAttribute::dataGridAttribute_SelectionChanged);
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(12, 414);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(98, 13);
			this->label4->TabIndex = 11;
			this->label4->Text = L"Selektiertes Attribut";
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(12, 439);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(61, 13);
			this->label5->TabIndex = 12;
			this->label5->Text = L"Attribut-Typ";
			// 
			// label6
			// 
			this->label6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(12, 461);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(66, 13);
			this->label6->TabIndex = 13;
			this->label6->Text = L"Attribut-Wert";
			// 
			// textBoxSelektiertesAttributName
			// 
			this->textBoxSelektiertesAttributName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxSelektiertesAttributName->Location = System::Drawing::Point(134, 406);
			this->textBoxSelektiertesAttributName->Name = L"textBoxSelektiertesAttributName";
			this->textBoxSelektiertesAttributName->ReadOnly = true;
			this->textBoxSelektiertesAttributName->Size = System::Drawing::Size(228, 20);
			this->textBoxSelektiertesAttributName->TabIndex = 14;
			// 
			// textBoxSelektiertesAttributTyp
			// 
			this->textBoxSelektiertesAttributTyp->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxSelektiertesAttributTyp->Location = System::Drawing::Point(134, 432);
			this->textBoxSelektiertesAttributTyp->Name = L"textBoxSelektiertesAttributTyp";
			this->textBoxSelektiertesAttributTyp->ReadOnly = true;
			this->textBoxSelektiertesAttributTyp->Size = System::Drawing::Size(228, 20);
			this->textBoxSelektiertesAttributTyp->TabIndex = 15;
			// 
			// textBoxSelektiertesAttributWert
			// 
			this->textBoxSelektiertesAttributWert->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBoxSelektiertesAttributWert->Location = System::Drawing::Point(134, 458);
			this->textBoxSelektiertesAttributWert->Name = L"textBoxSelektiertesAttributWert";
			this->textBoxSelektiertesAttributWert->Size = System::Drawing::Size(228, 20);
			this->textBoxSelektiertesAttributWert->TabIndex = 16;
			this->textBoxSelektiertesAttributWert->TextChanged += gcnew System::EventHandler(this, &GenerateKomplexAttribute::textBoxSelektiertesAttributWert_TextChanged);
			// 
			// buttonAbbrechen
			// 
			this->buttonAbbrechen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->buttonAbbrechen->Location = System::Drawing::Point(12, 532);
			this->buttonAbbrechen->Name = L"buttonAbbrechen";
			this->buttonAbbrechen->Size = System::Drawing::Size(75, 23);
			this->buttonAbbrechen->TabIndex = 17;
			this->buttonAbbrechen->Text = L"Abbrechen";
			this->buttonAbbrechen->UseVisualStyleBackColor = true;
			this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &GenerateKomplexAttribute::buttonAbbrechen_Click);
			// 
			// buttonUebernehmen
			// 
			this->buttonUebernehmen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->buttonUebernehmen->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->buttonUebernehmen->Location = System::Drawing::Point(286, 532);
			this->buttonUebernehmen->Name = L"buttonUebernehmen";
			this->buttonUebernehmen->Size = System::Drawing::Size(76, 23);
			this->buttonUebernehmen->TabIndex = 18;
			this->buttonUebernehmen->Text = L"Beenden";
			this->buttonUebernehmen->UseVisualStyleBackColor = true;
			this->buttonUebernehmen->Click += gcnew System::EventHandler(this, &GenerateKomplexAttribute::buttonUebernehmen_Click);
			// 
			// labelObjektIndex
			// 
			this->labelObjektIndex->AutoSize = true;
			this->labelObjektIndex->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->labelObjektIndex->ForeColor = System::Drawing::Color::Black;
			this->labelObjektIndex->Location = System::Drawing::Point(170, 111);
			this->labelObjektIndex->Name = L"labelObjektIndex";
			this->labelObjektIndex->Size = System::Drawing::Size(26, 16);
			this->labelObjektIndex->TabIndex = 19;
			this->labelObjektIndex->Text = L"xxx";
			// 
			// comboBoxEnumWerte
			// 
			this->comboBoxEnumWerte->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->comboBoxEnumWerte->FormattingEnabled = true;
			this->comboBoxEnumWerte->Location = System::Drawing::Point(134, 459);
			this->comboBoxEnumWerte->Name = L"comboBoxEnumWerte";
			this->comboBoxEnumWerte->Size = System::Drawing::Size(228, 21);
			this->comboBoxEnumWerte->TabIndex = 20;
			this->comboBoxEnumWerte->SelectedValueChanged += gcnew System::EventHandler(this, &GenerateKomplexAttribute::comboBoxEnumWerte_SelectedValueChanged);
			// 
			// textBoxIndex
			// 
			this->textBoxIndex->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->textBoxIndex->ForeColor = System::Drawing::Color::Red;
			this->textBoxIndex->Location = System::Drawing::Point(286, 130);
			this->textBoxIndex->Name = L"textBoxIndex";
			this->textBoxIndex->ReadOnly = true;
			this->textBoxIndex->Size = System::Drawing::Size(38, 20);
			this->textBoxIndex->TabIndex = 21;
			this->textBoxIndex->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(286, 156);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(38, 13);
			this->label7->TabIndex = 22;
			this->label7->Text = L"Index";
			// 
			// GenerateKomplexAttribute
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(388, 578);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->textBoxIndex);
			this->Controls->Add(this->comboBoxEnumWerte);
			this->Controls->Add(this->labelObjektIndex);
			this->Controls->Add(this->buttonUebernehmen);
			this->Controls->Add(this->buttonAbbrechen);
			this->Controls->Add(this->textBoxSelektiertesAttributWert);
			this->Controls->Add(this->textBoxSelektiertesAttributTyp);
			this->Controls->Add(this->textBoxSelektiertesAttributName);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->dataGridAttribute);
			this->Controls->Add(this->buttonDeleteInstance);
			this->Controls->Add(this->buttonPrevInstance);
			this->Controls->Add(this->buttonNextInstance);
			this->Controls->Add(this->buttonNewInstance);
			this->Controls->Add(this->textBoxDatentyp);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBoxRelationsName);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBoxKlassenName);
			this->Controls->Add(this->label1);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"GenerateKomplexAttribute";
			this->Text = L"Spezifikation Komplexe Objektdaten ";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridAttribute))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


void updateObjektControl()
{
	textBoxIndex->Text = Convert::ToString ( planObjektAktuellIndex-1 );
  if ( m_pPlanObjektList->count() == 0 )
  {
    buttonDeleteInstance->Enabled = false;
    buttonNextInstance->Enabled   = false;
    buttonPrevInstance->Enabled   = false;
    dataGridAttribute->Visible    = false;
  }
  else
  {
    dataGridAttribute->Visible    = true;
    buttonDeleteInstance->Enabled = true;
    if ( planObjektAktuellIndex == 1 && m_pPlanObjektList->count() > 1 )
    {
      buttonNextInstance->Enabled   = true;
      buttonPrevInstance->Enabled   = false;
    }
    else
    if ( planObjektAktuellIndex == m_pPlanObjektList->count() && m_pPlanObjektList->count() > 1  )
    {
      buttonNextInstance->Enabled   = false;
      buttonPrevInstance->Enabled   = true;
    }
    else
      if ( m_pPlanObjektList->count() == 1 )
      {
        buttonNextInstance->Enabled   = false;
        buttonPrevInstance->Enabled   = false;
      }
    else
    {
      buttonNextInstance->Enabled   = true;
      buttonPrevInstance->Enabled   = true;
    }

  }
  labelObjektIndex->Text = String::Concat( Convert::ToString( planObjektAktuellIndex), "(", 
    Convert::ToString( m_pPlanObjektList->count()), ")" );

}
///////////////////////////////////////////////////////////////////////////////
//      initializeDataGrid                                                   //
///////////////////////////////////////////////////////////////////////////////
void generateDataGrid()
{
	size_t       i, anz;
  DataColumn ^ pDataColumnAttributName;
  DataColumn ^ pDataColumnAttributWert;
  DataRow    ^ pDataRow;
  string       attributName;
  String     ^ STR;

  vector<GmlAttribut*> vGmlAttribute;

  attributeTable->Columns->Clear();
  attributeTable->Rows->Clear();


  STR = "Attribut-Name";
  pDataColumnAttributName= attributeTable->Columns->Add (STR );
  pDataColumnAttributName->DataType =  STR->GetType();
  pDataColumnAttributName->MaxLength = 256;

  STR = "Attribut-Wert";
  pDataColumnAttributWert= attributeTable->Columns->Add (STR );
  pDataColumnAttributWert->DataType =  STR->GetType();
  pDataColumnAttributWert->MaxLength = 256;

  anz = pAttributGmlTyp->getGmlAttribute( vGmlAttribute );
  for ( i = 0; i < anz; i++ )
  {
    GmlAttribut * pGmlAttribut = vGmlAttribute[i];

    attributName = pGmlAttribut->getGmlAttributName();
    STR = QuConvert::ToString ( pGmlAttribut->getGmlAttributName() ); 

    pDataRow = attributeTable->NewRow();
    pDataRow->default[0]= STR;
    pDataRow->default[1]= nullptr;

    attributeTable->Rows->Add ( pDataRow );
  }

  pGmlAttributAktuell                      = NULL;
  selectedRowIndex                         = -1;
  textBoxSelektiertesAttributWert->Visible = true;
  comboBoxEnumWerte->Visible               = false;
}



void updateDataGrid()
{
	size_t               i, anz;
	int									 ih;
  bool                 exist;
  double               h;
  string               attributName;
  string               stringAttributWert;
  string               schluesselText;
  string               enumName;
  GmlDictionaryEntry * pCodeList;

  vector<GmlAttribut*> vGmlAttribute;
  if ( pPlanObjektAktuell == NULL ) return;

  anz = pAttributGmlTyp->getGmlAttribute( vGmlAttribute );
  for ( i = 0; i < anz; i++ )
  {
    GmlAttribut * pGmlAttribut = vGmlAttribute[i];
    attributName = pGmlAttribut->getGmlAttributName();

    switch ( pGmlAttribut->getGmlAttributTyp())
    {
    case _STRING:
      exist = pPlanObjektAktuell->getStringAttributWert ( attributName, stringAttributWert );
      if ( exist )
        attributeTable->Rows[i]->default[1]= QuConvert::ToString( stringAttributWert );
      else
        attributeTable->Rows[i]->default[1]= nullptr;
      break;

    case _INTEGER:
      exist = pPlanObjektAktuell->getIntegerAttributWert ( attributName, ih );
      if ( exist )
        attributeTable->Rows[i]->default[1]= QuConvert::ToString( ih );
      else
        attributeTable->Rows[i]->default[1]= nullptr;
      break;

    case _DOUBLE:
    case _LENGTH:
    case _AREA:
    case _VOLUME:
    case _ANGLE:
      exist = pPlanObjektAktuell->getDoubleAttributWert ( attributName, h );
      if ( exist )
        attributeTable->Rows[i]->default[1]= QuConvert::ToString( h );
      else
        attributeTable->Rows[i]->default[1]= nullptr;
      break;

    case _DATE:
      {
        TimePosition * pTime = (TimePosition*)pPlanObjektAktuell->getDateAttributWert ( attributName );
        if ( pTime != NULL )        
          attributeTable->Rows[i]->default[1]= QuConvert::ToString( pTime->toXMLString() );
				else
					attributeTable->Rows[i]->default[1]= nullptr;
        break;

      }

    case _ANY_URI:
      exist = pPlanObjektAktuell->getURLAttributWert ( attributName, stringAttributWert );
      if ( exist )
        attributeTable->Rows[i]->default[1]= QuConvert::ToString( stringAttributWert );
      else
        attributeTable->Rows[i]->default[1]= nullptr;
      break;

    case _ENUMERATION:
    case _EXTERNAL_CODE_LIST:
      exist = pPlanObjektAktuell->getStringAttributWert ( attributName, stringAttributWert );

      if ( exist )
      {
        enumName = pGmlAttribut->getGmlEnumeration();

        if ( pGmlAttribut->getGmlAttributTyp() == _ENUMERATION)
          pCodeList = m_pCodeLists->getDictionaryEntry ( enumName );
        else
          pCodeList = m_pExternalCodeLists->getDictionaryEntry ( enumName );

        if ( pCodeList != NULL )
        {
          pCodeList->getSchluesselText( stringAttributWert, schluesselText );
          attributeTable->Rows[i]->default[1]= QuConvert::ToString( schluesselText );
        }
      }
      else
        attributeTable->Rows[i]->default[1]= nullptr;
      break;
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//      buttonUebernehmen_Click                                              //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonUebernehmen_Click(System::Object^  sender, System::EventArgs^  e)
{
  if ( pGmlAttributAktuell != NULL )
    saveAttributeDataToObjekt();

 delete pPlanObjektListSave;
}

///////////////////////////////////////////////////////////////////////////////
//      buttonAbbrechen_Click                                                //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAbbrechen_Click(System::Object^  sender, System::EventArgs^  e)
{
  m_pPlanObjektList->removeAll();
  m_pPlanObjektList = pPlanObjektListSave;
}


///////////////////////////////////////////////////////////////////////////////
//      dataGridAttribute_SelectionChanged                                   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void dataGridAttribute_SelectionChanged(System::Object^  sender, System::EventArgs^  e)
{
  string   attributNameAktuell;
  String ^ STR;

  if ( pGmlAttributAktuell != NULL )
    saveAttributeDataToObjekt();

  if ( dataGridAttribute->CurrentRow != nullptr )
  {
    selectedRowIndex = dataGridAttribute->CurrentRow->Index;
    STR = dynamic_cast<String^>(attributeTable->Rows[selectedRowIndex]->default[0]);
    if ( STR != nullptr )
    {
      QuConvert::systemStr2stdStr ( attributNameAktuell, STR );
      pGmlAttributAktuell = pAttributGmlTyp->getGmlAttribut( attributNameAktuell );
      getAttributeDataFromObjekt();
    }

    if ( pGmlAttributAktuell->getGmlAttributTyp() != _ENUMERATION &&
      pGmlAttributAktuell->getGmlAttributTyp() != _EXTERNAL_CODE_LIST )
      STR = textBoxSelektiertesAttributWert->Text;
    else
      STR = dynamic_cast<String^> (comboBoxEnumWerte->SelectedItem);

      attributeTable->Rows[selectedRowIndex]->default[1] = STR;

  }
  else
  {
    selectedRowIndex    = -1;
    pGmlAttributAktuell = NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
//      saveAttributeDataToObjekt                                            //
///////////////////////////////////////////////////////////////////////////////
void saveAttributeDataToObjekt()
{
  String             ^ STR;
  string               gmlAttributNameAktuell ;
  string               gmlAttributWertAktuell;
  string               enumName;
  string               schluesselNummer;
  GmlDictionaryEntry * pCodeList;


  if ( pGmlAttributAktuell == NULL || pPlanObjektAktuell == NULL ) return;
  
  gmlAttributNameAktuell = pGmlAttributAktuell->getGmlAttributName();

  if ( pGmlAttributAktuell->getGmlAttributTyp() != _ENUMERATION &&
       pGmlAttributAktuell->getGmlAttributTyp() != _EXTERNAL_CODE_LIST )
    STR = textBoxSelektiertesAttributWert->Text;
  else
    STR = dynamic_cast<String^> (comboBoxEnumWerte->SelectedItem);

  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( gmlAttributWertAktuell, STR );
  else
    gmlAttributWertAktuell = "";

  switch ( pGmlAttributAktuell->getGmlAttributTyp() )
  {
  case _STRING:
    pPlanObjektAktuell->deleteStringAttribut( gmlAttributNameAktuell );
    if ( gmlAttributWertAktuell != "")
      pPlanObjektAktuell->setStringAttribut( gmlAttributNameAktuell, gmlAttributWertAktuell);
    break;

  case _DOUBLE:
  case _LENGTH:
  case _ANGLE:
  case _AREA:
  case _VOLUME:
    pPlanObjektAktuell->deleteStringAttribut( gmlAttributNameAktuell );
    if ( gmlAttributWertAktuell != "")
    {
      try
      {
        double d = Convert::ToDouble( STR );
        if ( pGmlAttributAktuell->getGmlAttributTyp() == _DOUBLE )
          pPlanObjektAktuell->setDoubleAttribut( gmlAttributNameAktuell, d );
        else
        if ( pGmlAttributAktuell->getGmlAttributTyp() == _LENGTH )
          pPlanObjektAktuell->setDoubleAttribut( gmlAttributNameAktuell, d,  m_pFeatures->getUOMList()->getUomLength() );
        else
        if ( pGmlAttributAktuell->getGmlAttributTyp() == _ANGLE )
          pPlanObjektAktuell->setDoubleAttribut( gmlAttributNameAktuell, d,  m_pFeatures->getUOMList()->getUomAngle() );
        else
        if ( pGmlAttributAktuell->getGmlAttributTyp() == _AREA )
          pPlanObjektAktuell->setDoubleAttribut( gmlAttributNameAktuell, d,  m_pFeatures->getUOMList()->getUomArea() );
        else
        if ( pGmlAttributAktuell->getGmlAttributTyp() == _VOLUME )
          pPlanObjektAktuell->setDoubleAttribut( gmlAttributNameAktuell, d,  m_pFeatures->getUOMList()->getUomVolume() );
      }
      catch ( System::FormatException^ )
      {
        textBoxSelektiertesAttributWert->Text = nullptr;
      }
    }
    break;

  case _INTEGER:
    pPlanObjektAktuell->deleteIntegerAttribut( gmlAttributNameAktuell );
    if ( gmlAttributWertAktuell != "")
    {
      try
      {
        int i = Convert::ToInt32( STR );
        pPlanObjektAktuell->setIntegerAttribut( gmlAttributNameAktuell, i );
     }
      catch ( System::FormatException^ )
      {
        textBoxSelektiertesAttributWert->Text = nullptr;
      }

    }
    break;

  case _DATE:
    pPlanObjektAktuell->deleteDateAttribut( gmlAttributNameAktuell );
    if ( gmlAttributWertAktuell != "")
    {
      TimePosition * pDate = new TimePosition;
      pDate->fromXMLString( gmlAttributWertAktuell );
      DateAttr * pDateAttr = new DateAttr;
      pDateAttr->value = pDate;
      pPlanObjektAktuell->setDateAttribut( gmlAttributNameAktuell, pDateAttr );
    }
    break;

  case _ANY_URI:
    pPlanObjektAktuell->deleteURLAttribut( gmlAttributNameAktuell );
    if ( gmlAttributWertAktuell != "")
      pPlanObjektAktuell->setURLAttribut( gmlAttributNameAktuell, gmlAttributWertAktuell );
    break;

  case _ENUMERATION:
  case _EXTERNAL_CODE_LIST:
    pPlanObjektAktuell->deleteStringAttribut( gmlAttributNameAktuell );

    enumName = pGmlAttributAktuell->getGmlEnumeration();

    if ( pGmlAttributAktuell->getGmlAttributTyp() == _ENUMERATION)
      pCodeList = m_pCodeLists->getDictionaryEntry ( enumName );
    else
      pCodeList = m_pExternalCodeLists->getDictionaryEntry ( enumName );

    if ( pCodeList != NULL )
    {
      pCodeList->getSchluesselNummer( gmlAttributWertAktuell, schluesselNummer );
      pPlanObjektAktuell->setStringAttribut( gmlAttributNameAktuell, schluesselNummer );
    }

  }

}

void getAttributeDataFromObjekt()
{
  String             ^ STR;
  bool                 exist;
  double               d;
  unsigned int         i;
  int                  ih;
  ATTRIBUT_TYP         typ;
  string               gmlAttributNameAktuell;
  string               gmlAttributWertAktuell;
  string               schluesselText;
  string               enumName;
  string               stdString;
  GmlEnumeration     * pEnumeration;
  GmlDictionary      * pCodeListsActive;
  GmlDictionaryEntry * pCodeList = NULL;
  vector<string>       itemNames;

  if ( pPlanObjektAktuell == NULL || pGmlAttributAktuell == NULL ) return;
  
  gmlAttributNameAktuell = pGmlAttributAktuell->getGmlAttributName();

  textBoxSelektiertesAttributName->Text = QuConvert::ToString( gmlAttributNameAktuell );

  switch ( pGmlAttributAktuell->getGmlAttributTyp() )
  {
  case _STRING:
    textBoxSelektiertesAttributWert->Visible = true;
    comboBoxEnumWerte->Visible              = false;

    textBoxSelektiertesAttributTyp->Text = "String";
    exist = pPlanObjektAktuell->getStringAttributWert( gmlAttributNameAktuell, gmlAttributWertAktuell );
    if ( exist )
      textBoxSelektiertesAttributWert->Text = QuConvert::ToString( gmlAttributWertAktuell );
    else
      textBoxSelektiertesAttributWert->Text = nullptr;
    break;

  case _DOUBLE:
  case _LENGTH:
  case _AREA:
  case _ANGLE:
    textBoxSelektiertesAttributWert->Visible = true;
    comboBoxEnumWerte->Visible              = false;

    if (  pGmlAttributAktuell->getGmlAttributTyp() == _DOUBLE )
      textBoxSelektiertesAttributTyp->Text = "Double";
    else
    if (  pGmlAttributAktuell->getGmlAttributTyp() == _LENGTH )
      textBoxSelektiertesAttributTyp->Text = "Länge";
    else
    if (  pGmlAttributAktuell->getGmlAttributTyp() == _AREA )
      textBoxSelektiertesAttributTyp->Text = "Fläche";
    else
    if (  pGmlAttributAktuell->getGmlAttributTyp() == _VOLUME )
      textBoxSelektiertesAttributTyp->Text = "Volumen";
    else
    if (  pGmlAttributAktuell->getGmlAttributTyp() == _ANGLE )
      textBoxSelektiertesAttributTyp->Text = "Winkel";

    exist = pPlanObjektAktuell->getDoubleAttributWert( gmlAttributNameAktuell, d );
    if ( exist )
      textBoxSelektiertesAttributWert->Text = QuConvert::ToString( d );
    else
      textBoxSelektiertesAttributWert->Text = nullptr;
    break;

  case _INTEGER:
    textBoxSelektiertesAttributWert->Visible = true;
    comboBoxEnumWerte->Visible              = false;

    textBoxSelektiertesAttributTyp->Text = "Integer";
    exist = pPlanObjektAktuell->getIntegerAttributWert( gmlAttributNameAktuell, ih );
    if ( exist )
      textBoxSelektiertesAttributWert->Text = QuConvert::ToString( ih );
    else
      textBoxSelektiertesAttributWert->Text = nullptr;
    break;

  case _ANY_URI:
    textBoxSelektiertesAttributWert->Visible = true;
    comboBoxEnumWerte->Visible              = false;

    textBoxSelektiertesAttributTyp->Text = "URL";
    exist = pPlanObjektAktuell->getURLAttributWert( gmlAttributNameAktuell, gmlAttributWertAktuell );
    if ( exist )
      textBoxSelektiertesAttributWert->Text = QuConvert::ToString( gmlAttributWertAktuell );
    else
      textBoxSelektiertesAttributWert->Text = nullptr;
    break;

  case _DATE:
    {
      textBoxSelektiertesAttributWert->Visible = true;
      comboBoxEnumWerte->Visible              = false;

      textBoxSelektiertesAttributTyp->Text = "Datum";
      DateAttr * pDateAttr = pPlanObjektAktuell->getDateAttributWert( gmlAttributNameAktuell );
      if ( pDateAttr != NULL )
      {
        TimePosition * pTime = dynamic_cast<TimePosition*>(pDateAttr->value);
        if ( pTime != NULL )
          textBoxSelektiertesAttributWert->Text = QuConvert::ToString( pTime->toXMLString() );
        else
          textBoxSelektiertesAttributWert->Text = nullptr;
      }
      else
        textBoxSelektiertesAttributWert->Text = nullptr;

      break;
    }

  case _EXTERNAL_CODE_LIST:
    typ       = pGmlAttributAktuell->getGmlAttributTyp();
    enumName  = pGmlAttributAktuell->getGmlEnumeration();
    pCodeListsActive = m_pCodeLists;

    pCodeList = pCodeListsActive->getDictionaryEntry ( enumName );

    if ( pCodeList != NULL )
    {
      textBoxSelektiertesAttributWert->Visible = false;
      textBoxSelektiertesAttributTyp->Text     = "Externe Codeliste";
      comboBoxEnumWerte->Visible               = true;
      comboBoxEnumWerte->Items->Clear();
			comboBoxEnumWerte->SelectedItem = nullptr;
			comboBoxEnumWerte->Text = nullptr;

      pCodeList->getCodelistSchluessel ( itemNames );
      for ( i = 0; i < itemNames.size(); i++ )
      {
        stdString = itemNames[i];
        if ( pCodeList != NULL )
        {
          exist = pCodeList->getSchluesselText ( stdString, schluesselText );
          if ( exist && schluesselText != "" ) stdString = schluesselText;
        }

        STR = QuConvert::ToString ( stdString );
        comboBoxEnumWerte->Items->Add ( STR );
      }

      exist = pPlanObjektAktuell->getStringAttributWert( gmlAttributNameAktuell, gmlAttributWertAktuell );
      if ( exist &&  pCodeList != NULL )
      {
        exist = pCodeList->getSchluesselText ( gmlAttributWertAktuell, schluesselText );
				if (exist && schluesselText != "")
				{
          STR = QuConvert::ToString ( schluesselText );
          comboBoxEnumWerte->SelectedItem = STR;
				}
       }
    }
    else
    {
      textBoxSelektiertesAttributWert->Visible = true;
      comboBoxEnumWerte->Visible              = false;
      textBoxSelektiertesAttributTyp->Text = "String";

      exist = pPlanObjektAktuell->getStringAttributWert( gmlAttributNameAktuell, gmlAttributWertAktuell );
      if ( exist )
        textBoxSelektiertesAttributWert->Text = QuConvert::ToString( gmlAttributWertAktuell );
      else
        textBoxSelektiertesAttributWert->Text = nullptr;
    }
    break;

  case _ENUMERATION:
    textBoxSelektiertesAttributTyp->Text     = "Enumeration";
    textBoxSelektiertesAttributWert->Visible = false;
    comboBoxEnumWerte->Visible               = true;
    comboBoxEnumWerte->Items->Clear();
		comboBoxEnumWerte->SelectedItem = nullptr;
		comboBoxEnumWerte->Text = nullptr;

    typ       = pGmlAttributAktuell->getGmlAttributTyp();
    enumName  = pGmlAttributAktuell->getGmlEnumeration();
    pEnumeration     = m_pGmlSchema->getGmlEnumeration ( enumName );
    pCodeListsActive = m_pCodeLists;
    pCodeList = pCodeListsActive->getDictionaryEntry ( enumName );

    if ( pEnumeration != NULL )
    {
      pEnumeration->getEnumerationItemNames ( itemNames );
      for ( i = 0; i < itemNames.size(); i++ )
      {
        stdString = itemNames[i];
        if ( pCodeList != NULL )
        {
          exist = pCodeList->getSchluesselText ( stdString, schluesselText );
          if ( exist && schluesselText != "" ) stdString = schluesselText;
        }

        STR = QuConvert::ToString ( stdString );
        comboBoxEnumWerte->Items->Add ( STR );
      }

      exist = pPlanObjektAktuell->getStringAttributWert( gmlAttributNameAktuell, gmlAttributWertAktuell );
      if ( exist &&  pCodeList != NULL )
      {
        exist = pCodeList->getSchluesselText ( gmlAttributWertAktuell, schluesselText );
        if ( exist && schluesselText != "" )
          STR = QuConvert::ToString ( schluesselText );
        comboBoxEnumWerte->SelectedItem = STR;
       }
    }
    break;
  }
      
}


private: System::Void buttonNewInstance_Click(System::Object^  sender, System::EventArgs^  e)
{
  saveAttributeDataToObjekt();
  string gmlId;
  
  textBoxSelektiertesAttributWert->Text = nullptr;
  comboBoxEnumWerte->SelectedItem       = nullptr;

  gmlId = Constant::createGmlId();
  pPlanObjektAktuell = new Feature( ANY_OBJEKT, gmlId, m_pFeatures, pAttributGmlTyp->getGmlElementName() );
  planObjektAktuellIndex = (int)m_pPlanObjektList->add( pPlanObjektAktuell );

  updateObjektControl();
  updateDataGrid();
}

private: System::Void buttonNextInstance_Click(System::Object^  sender, System::EventArgs^  e)
{
  saveAttributeDataToObjekt();

  planObjektAktuellIndex++;
  pPlanObjektAktuell = m_pPlanObjektList->get( planObjektAktuellIndex-1 );

  getAttributeDataFromObjekt();

  updateObjektControl();
  updateDataGrid();
}

private: System::Void buttonPrevInstance_Click(System::Object^  sender, System::EventArgs^  e)
{
  saveAttributeDataToObjekt();

  planObjektAktuellIndex--;
  pPlanObjektAktuell = m_pPlanObjektList->get( planObjektAktuellIndex-1 );

  getAttributeDataFromObjekt();

  updateObjektControl();
  updateDataGrid();
}

private: System::Void buttonDeleteInstance_Click(System::Object^  sender, System::EventArgs^  e)
{
	size_t anz = m_pPlanObjektList->remove( planObjektAktuellIndex-1 );
  if ( anz > 0 )
  {
    planObjektAktuellIndex = 1;
    pPlanObjektAktuell = m_pPlanObjektList->get( planObjektAktuellIndex-1 );
  }
  else
  {
    planObjektAktuellIndex = 0;
    pPlanObjektAktuell = NULL;
  }

  updateObjektControl();
  updateDataGrid();

}

private: System::Void comboBoxEnumWerte_SelectedValueChanged(System::Object^  sender, System::EventArgs^  e )
{
		String ^ STR = dynamic_cast<String^> (comboBoxEnumWerte->SelectedItem);
		attributeTable->Rows[selectedRowIndex]->default[1] = STR;
//		updateDataGrid();
}
private: System::Void textBoxSelektiertesAttributWert_TextChanged(System::Object^  sender, System::EventArgs^  e) 
{
	String ^ STR = textBoxSelektiertesAttributWert->Text;
	attributeTable->Rows[selectedRowIndex]->default[1] = STR;

//	updateDataGrid();
}

};
