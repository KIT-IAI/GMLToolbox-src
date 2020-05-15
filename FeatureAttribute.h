#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "SrsManagerRef.h"
#include "Feature.h"
#include "GmlSchema.h"
#include "GmlReader.h"
#include "Dictionary.h"
#include "GmlNamespaces.h"
#include "Convert.h"
#include "Geometrie.h"
#include "TimeX.h"
#include "TimeSeries.h"
#include "UnitOfMeasure.h"

using namespace std;

	/// <summary>
	/// Anzeige/Änderung der Attribute und Attributwerte eines Features
	/// </summary>
	public ref class FeatureAttribute : public System::Windows::Forms::Form
	{
	public:
		FeatureAttribute( Features * pFeatures, Feature * pFeature, ArrayList ^ highlight )
		{
			InitializeComponent();

      m_pFeatures                = pFeatures;
      m_pFeature                 = pFeature;
      m_pGmlSchema               = m_pFeatures->getGmlSchema();
      m_pSchemaTypeNamesStandard = m_pGmlSchema->getSchemaTypeNames();

//      string htmlViewer = m_pFeatures->getHTMLViewer();
//      m_HTMLViewer = QuConvert::ToString( htmlViewer );

      attributeColor      = Color::Yellow;
      dataTypeColor       = Color::LightSteelBlue;
      highlightColor      = Color::Red;
      editColor           = Color::White;
      m_ShowEmptyAttributes = true;
      m_EditEnabled         = false;
      m_ExpandDataTypes     = true;

      m_Highlight = highlight;
      if ( m_pFeature != NULL )
        textBoxFeatureClass->Text = QuConvert::ToString( m_pFeature->getKlassenName() );

      pRowSelected            = nullptr; 
      m_AttributeNameSelected = nullptr;
      m_AttributeTypeSelected = nullptr;
      m_AttributeValueSelected = nullptr;

      initialize();
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~FeatureAttribute()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::DataGridView^  dataGridViewAttribute;
  private: System::Windows::Forms::TextBox^  textBoxFeatureClass;
  private: System::Windows::Forms::Button^  buttonAbbrechen;
  private: System::Windows::Forms::Button^  buttonBeenden;


	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

  private:
    GmlSchema      *  m_pGmlSchema;               // GML-Applikationsschema
    Feature         * m_pFeature;                 // Betrachtetes Feature
    Features        * m_pFeatures;                // Aggregation aller Features
    SchemaTypeNames * m_pSchemaTypeNamesStandard; // Schemaabhängige GML Klassen- und Attributnamen

    Color attributeColor;  // Hintergrundfarbe GML-Attribute
    Color dataTypeColor;   // Hintergrundfarbe komplexe Datentypen / Features
    Color highlightColor;  // Farbe für ausgewählte Objekte
    Color editColor;       // Farbe für editierbare Objekte

    ArrayList ^ m_Highlight;  // Liste ausgewählter Objekte

    bool m_ShowEmptyAttributes;  // Steuert Anzeige nicht-belegter Objekte
    bool m_EditEnabled;          // Steuert, ob der Editier-Modus aktiv ist
    bool m_ExpandDataTypes;      // Steuert, ob komplexe Datentypen in der Liste expandiert werden

    DataGridViewRow ^ pRowSelected;                  // Ausgewählte Zeile der Anzeigeliste
    String          ^ m_AttributeNameSelected;       // Name des selektierten AAtributs
    String          ^ m_AttributeTypeSelected;       // Typ des selektierten Attributs
    String          ^ m_AttributeValueSelected;      // Wert des selektierten Attributs
    String          ^ m_AttributeClassifierSelected; // Classifier des selektierten Attributs

 //   String          ^ m_HTMLViewer; // HTML-Browser

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->dataGridViewAttribute = (gcnew System::Windows::Forms::DataGridView());
      this->textBoxFeatureClass = (gcnew System::Windows::Forms::TextBox());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewAttribute))->BeginInit();
      this->SuspendLayout();
      // 
      // dataGridViewAttribute
      // 
      this->dataGridViewAttribute->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->dataGridViewAttribute->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
      this->dataGridViewAttribute->Location = System::Drawing::Point(12, 39);
      this->dataGridViewAttribute->Name = L"dataGridViewAttribute";
      this->dataGridViewAttribute->Size = System::Drawing::Size(798, 486);
      this->dataGridViewAttribute->TabIndex = 0;
      this->dataGridViewAttribute->CellBeginEdit += gcnew System::Windows::Forms::DataGridViewCellCancelEventHandler(this, &FeatureAttribute::dataGridViewAttribute_CellBeginEdit);
      this->dataGridViewAttribute->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &FeatureAttribute::dataGridViewAttribute_CellContentClick);
      this->dataGridViewAttribute->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &FeatureAttribute::dataGridViewAttribute_CellEndEdit);
      // 
      // textBoxFeatureClass
      // 
      this->textBoxFeatureClass->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxFeatureClass->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, 
        System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
      this->textBoxFeatureClass->ForeColor = System::Drawing::Color::Red;
      this->textBoxFeatureClass->Location = System::Drawing::Point(12, 13);
      this->textBoxFeatureClass->Name = L"textBoxFeatureClass";
      this->textBoxFeatureClass->ReadOnly = true;
      this->textBoxFeatureClass->Size = System::Drawing::Size(798, 20);
      this->textBoxFeatureClass->TabIndex = 1;
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonAbbrechen->Location = System::Drawing::Point(12, 552);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 23);
      this->buttonAbbrechen->TabIndex = 2;
      this->buttonAbbrechen->Text = L"Abbrechen";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonBeenden->Location = System::Drawing::Point(733, 552);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(77, 23);
      this->buttonBeenden->TabIndex = 3;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      // 
      // FeatureAttribute
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(822, 587);
      this->ControlBox = false;
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->textBoxFeatureClass);
      this->Controls->Add(this->dataGridViewAttribute);
      this->Name = L"FeatureAttribute";
      this->Text = L"Attribute eines GML-Features";
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewAttribute))->EndInit();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

Void initialize()
{
  dataGridViewAttribute->ColumnCount = 4;
  dataGridViewAttribute->ColumnHeadersVisible = true;

  dataGridViewAttribute->DefaultCellStyle->WrapMode = DataGridViewTriState::True;

  DataGridViewCellStyle ^ columnHeaderStyle = gcnew DataGridViewCellStyle;
  columnHeaderStyle->BackColor = Color::Aqua;
  columnHeaderStyle->Font = gcnew System::Drawing::Font( "Verdana",8,FontStyle::Bold );
  columnHeaderStyle->WrapMode = DataGridViewTriState::True;

  dataGridViewAttribute->ColumnHeadersDefaultCellStyle = columnHeaderStyle;
  dataGridViewAttribute->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

  dataGridViewAttribute->Columns[0]->Name = "Attribut-Name";
  dataGridViewAttribute->Columns[0]->Width = 200;

  dataGridViewAttribute->Columns[1]->Name = "Attribut-Typ";
  dataGridViewAttribute->Columns[1]->Width = 100;

  dataGridViewAttribute->Columns[2]->Name = "Attribut-Wert";
  dataGridViewAttribute->Columns[2]->Width = 200;

  dataGridViewAttribute->Columns[3]->Name = "Klassifikatoren";
  dataGridViewAttribute->Columns[3]->Width = 300;

  if ( m_pFeature != NULL )
    writeRows ( m_pFeature, false, 0 );
}

///////////////////////////////////////////////////////////////////////////////
//  Zeigen bzw. Verbergen der Ok und Cancel Buttons                          //
///////////////////////////////////////////////////////////////////////////////
public: Void showButtons()
{
  buttonAbbrechen->Show();
  buttonBeenden->Show();
}

public: Void hideButtons()
{
  buttonAbbrechen->Hide();
  buttonBeenden->Hide();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das Feature, dessen Attribute dargestellt werden sollen            //
///////////////////////////////////////////////////////////////////////////////
public: Void setFeature (  std::string featureId, Feature * pFeature, bool showEmptyAttributes, bool editEnabled, bool expandDataTypes )
{
  if ( pFeature == NULL )
  {
    String ^ STR;
    STR = "Fehlerhafte Relation, Feature-id =";
    STR = String::Concat( STR, QuConvert::ToString ( featureId) );
    textBoxFeatureClass->Text = STR;

    return;
  }

  m_pFeature = pFeature;

  textBoxFeatureClass->Text = QuConvert::ToString( m_pFeature->getKlassenName() );

  m_EditEnabled = editEnabled;
  if ( editEnabled )
  {
    m_ShowEmptyAttributes = true;
    m_ExpandDataTypes     = false;
  }
  else
  {
    m_ShowEmptyAttributes = showEmptyAttributes;
    m_ExpandDataTypes     = expandDataTypes;
  }

  dataGridViewAttribute->Rows->Clear();
  writeRows ( m_pFeature, false, 0 );
}

///////////////////////////////////////////////////////////////////////////////
//  Stellt ein, ob auch nicht-belegte Attribute in der Liste erscheinen      //
//  sollen                                                                   //
///////////////////////////////////////////////////////////////////////////////
public: Void setShowEmptyAttributes ( bool show )
{
  if ( m_ShowEmptyAttributes == show ) return;

  m_ShowEmptyAttributes = show;

  dataGridViewAttribute->Rows->Clear();
  if ( m_pFeature != NULL )
    writeRows ( m_pFeature, false, 0 );
}

///////////////////////////////////////////////////////////////////////////////
// Stellt ein, ob eine Änderung von Attributwerten möglich ist               //
///////////////////////////////////////////////////////////////////////////////
public: Void setEditEnabled ( bool b )
{
  if ( m_EditEnabled == b ) return;

  m_EditEnabled         = b;
  if ( m_EditEnabled )
  {
    m_ShowEmptyAttributes = true;
    m_ExpandDataTypes     = false;
  }

  dataGridViewAttribute->Rows->Clear();
  if ( m_pFeature != NULL )
    writeRows ( m_pFeature, false, 0 );
};

///////////////////////////////////////////////////////////////////////////////
//  Stellt ein, ob auch Sub-Attribute komplexer datentypen angezeigt werden  //
//  sollen                                                                   //
///////////////////////////////////////////////////////////////////////////////
public: Void setExpandDataTypes ( bool b )
{
  if (m_ExpandDataTypes == b ) return;

  m_ExpandDataTypes = b;

  dataGridViewAttribute->Rows->Clear();
  if ( m_pFeature != NULL )
    writeRows ( m_pFeature, false, 0 );
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Attribute als Rows raus                                     //
///////////////////////////////////////////////////////////////////////////////
void writeRows ( Feature * pPlanObjekt, bool subtype, int level )
{
  vector<GmlAttribut*>	 gmlAttribute;
  GmlAttribut			     * pGmlAttribut;
  GmlDictionary        * pCodeLists;
  GmlDictionaryEntry   * pEntry;
	size_t                 i, ii, j, anz;
	size_t			           gmlAttributAnz;
	size_t				       	 attributWertAnz;
  bool                   success;
	size_t                 k;
  ATTRIBUT_TYP		    	 gmlAttributTyp;
  string				      	 gmlAttributName;
  StringAttr           * stringAttributWert;
  IntAttr						   * intAttributWert;
  DoubleAttr					 * doubleAttributWert;
  BoolAttr					   * boolAttributWert;
  FeatureRelation		   * pRelation;
  Feature			         * pRelationZiel;
  vector<StringAttr*>	   stringAttributWerte;
  vector<IntAttr*>	     intAttributWerte;
  vector<DoubleAttr*>    doubleAttributWerte;
  vector<DateAttr*>      dateAttributWerte;
  vector<UOM>            uomAttributWerte;
  vector<FeatureRelation*> relationen;
  vector<Feature*>        planObjekte;
  vector<string>          genericAttributNamen;
  vector<string>          classifierNamen;
  vector<string>          classifierValues;
  vector<double>          vDirectPosition;
  DirectPosition        * pDirectPosition;
  string                  klassenName;
  string                  klassenNameRelationsZiel;
  string                  enumerationsName;
  string                  stdString;
  string                  enumText;
  string                  genericAttributName; 
  string                  genericAttributWert;
  int                     genericIntAttributWert;
  double                  genericDoubleAttributWert;
  String                ^ STR;
  String                ^ GML_ATTRIBUT_NAME;
  String                ^ GML_ATTRIBUT_TYP;
  String                ^ GML_ATTRIBUT_WERT;
  String                ^ ATTRIBUT_NAME;
  String                ^ ATTRIBUT_WERT;
  String                ^ CLASSIFIER_STRING;
  String                ^ GML_ATTRIBUT_LISTEN_NAME;
  String                ^ DIRECT_POSITION;
  bool                    highlighted;
  GmlKlasse             * pBPlanKlasse = NULL;
  StringAttr            * pSimpleContent;
  Features              * pFeatures;

  map<string,string>::iterator iterClassifier;

  DataGridViewRow          ^ row;

  klassenName = pPlanObjekt->getKlassenName ();
  pFeatures   = pPlanObjekt->getFeatures();

  GmlElement * pGmlElement = m_pGmlSchema->getGmlElement( klassenName );
  if ( pGmlElement == NULL )
  {
    if ( klassenName == "core:externalReference" )
        pBPlanKlasse = m_pGmlSchema->getGmlKlasse ( "core:ExternalReferenceType" );
    else
    if ( klassenName == "citygml:externalReference" )
      pBPlanKlasse = m_pGmlSchema->getGmlKlasse ( "citygml:ExternalReferenceType" );
    else
    if ( klassenName == "core:externalObject" )
      pBPlanKlasse = m_pGmlSchema->getGmlKlasse ( "core:ExternalObjectReferenceType" );
    else
    if ( klassenName == "citygml:externalObject" )
        pBPlanKlasse = m_pGmlSchema->getGmlKlasse ( "citygml:ExternalObjectReferenceType" );
    else
    if ( klassenName == "core:xalAddress" )
    {
      writeXalAddress ( pPlanObjekt );
      return;
    }
  }
  else
    pBPlanKlasse = m_pGmlSchema->getGmlKlasse ( pGmlElement->getGmlTypeName() );

  if ( pBPlanKlasse == NULL )
    return;

  pCodeLists         = m_pGmlSchema->getCodeListsDictionary();

  // gml:id
  if ( !subtype )
  {
    GML_ATTRIBUT_NAME = "gml:id";
    row = generateTextRow ( attributeColor, false, GML_ATTRIBUT_NAME, QuConvert::ToString(  pPlanObjekt->getGmlId() ), "Identifier", nullptr, true );
    dataGridViewAttribute->Rows->Add( row );
  }

  gmlAttributAnz = pBPlanKlasse->getGmlAttribute ( gmlAttribute );
  for ( i = 0; i < gmlAttributAnz; i++ )
  {
    stringAttributWerte.clear();
    intAttributWerte.clear();
    doubleAttributWerte.clear();
    dateAttributWerte.clear();
    relationen.clear();

    pGmlAttribut    = gmlAttribute[i];
    gmlAttributTyp  = pGmlAttribut->getGmlAttributTyp();
    gmlAttributName = pGmlAttribut->getGmlAttributName();
    GML_ATTRIBUT_NAME = QuConvert::ToString ( gmlAttributName );
    highlighted = isHighlighted( GML_ATTRIBUT_NAME );

    for ( j = 0; j < level; j++ )
      GML_ATTRIBUT_NAME = String::Concat( "----", GML_ATTRIBUT_NAME );
    GML_ATTRIBUT_TYP = QuConvert::ToString( GmlSchema::gmlAttributTypToString( gmlAttributTyp ) );

    switch ( gmlAttributTyp )
    {
    case _STRING:
      attributWertAnz = pPlanObjekt->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          stringAttributWert = stringAttributWerte[j];
          if ( stringAttributWert->isNil )
          {
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil", 
                                    GML_ATTRIBUT_TYP, QuConvert::ToString( stringAttributWert->getNilReason() ), false );
          }
          else
          {
            CLASSIFIER_STRING = nullptr;
//            for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
            for ( iterClassifier = stringAttributWert->classifier.begin(); iterClassifier != stringAttributWert->classifier.end(); iterClassifier++  )
            {
              ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
              ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
              if (iterClassifier == stringAttributWert->classifier.begin() )
                CLASSIFIER_STRING = String::Concat( ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
              else
                CLASSIFIER_STRING = String::Concat( CLASSIFIER_STRING, ";", ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
            }
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, QuConvert::ToString( stringAttributWert->value ), 
              GML_ATTRIBUT_TYP, CLASSIFIER_STRING, false );
         }
          dataGridViewAttribute->Rows->Add( row );
        }
      else
      if ( m_ShowEmptyAttributes )
      {
        row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, nullptr, false );
        dataGridViewAttribute->Rows->Add( row );
      }
      break;

    case _MEASURE_LISTE:
    case _QUANTITY_EXTENT:
      attributWertAnz = pPlanObjekt->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
      if ( attributWertAnz > 0 )
      {
          doubleAttributWert = doubleAttributWerte[0];
          if ( doubleAttributWert->isNil )
          {
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil", 
              GML_ATTRIBUT_TYP, QuConvert::ToString( doubleAttributWert->getNilReason() ), false );           
          }
          else
          {
            STR = "";
            for ( j = 0; j < attributWertAnz; j++ )
            {
              doubleAttributWert = doubleAttributWerte[j];
              String ^ STR_LOC = XmlConvert::ToString( doubleAttributWert->value );
              STR = String::Concat ( STR, STR_LOC, " " );
            }

            if ( doubleAttributWerte[0]->uom != NULL )
            {
              CLASSIFIER_STRING = QuConvert::ToString( doubleAttributWerte[0]->uom->getSign() );
              if ( doubleAttributWerte[0]-> classifier.size() > 0 )
                CLASSIFIER_STRING = String::Concat( CLASSIFIER_STRING, ";" );
            }
            
            for ( iterClassifier = doubleAttributWerte[0]->classifier.begin(); iterClassifier != doubleAttributWerte[0]->classifier.end(); iterClassifier++ )
            {
              ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
              ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
              if (iterClassifier == doubleAttributWerte[0]->classifier.begin() )
                CLASSIFIER_STRING = String::Concat( ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
              else
                CLASSIFIER_STRING = String::Concat( CLASSIFIER_STRING, ";", ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
            }            
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, STR, GML_ATTRIBUT_TYP, CLASSIFIER_STRING, false );
          }         
          dataGridViewAttribute->Rows->Add( row );
      }
      else
        if ( m_ShowEmptyAttributes )
        {
          row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, nullptr, false );
          dataGridViewAttribute->Rows->Add( row );
        }
        break;



    case _EXTERNAL_CODE_LIST_GML3_3:
      attributWertAnz = pPlanObjekt->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
      
      if ( attributWertAnz > 0 )
      {
        for ( j = 0; j < attributWertAnz; j++ )
        {
          stringAttributWert = stringAttributWerte[j];

          if ( stringAttributWert->isNil )
          {
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil", 
              GML_ATTRIBUT_TYP, QuConvert::ToString( stringAttributWert->getNilReason() ), false );
          }
          else
          {
            CLASSIFIER_STRING = QuConvert::ToString( stringAttributWert->value );
            STR = nullptr;

            for ( iterClassifier = stringAttributWert->classifier.begin(); iterClassifier != stringAttributWert->classifier.end(); iterClassifier++ )
            {
              if ( iterClassifier->first == "xlink:title" )
              {
                STR = QuConvert::ToString( iterClassifier->second );
                break;
              }
            }
          
            if ( STR == nullptr )
            {
              cli::array<String^> ^ splitString;
              string                codelistName, code;     

              splitString = CLASSIFIER_STRING->Split( '#' );
              if ( splitString->Length >= 2 )
              {
                QuConvert::systemStr2stdStr( codelistName, splitString[0] );
                QuConvert::systemStr2stdStr( code, splitString[1] );
              }
              else
              {
                codelistName = GMLReader::generateExternalCodelistPath ( CLASSIFIER_STRING );
                QuConvert::systemStr2stdStr( code, CLASSIFIER_STRING );
              }

              pEntry = pCodeLists->getDictionaryEntry ( codelistName );
              if ( pEntry != NULL )
              {
                success = pEntry->getSchluesselText ( code, enumText );
                if ( success )
                  STR = String::Concat( QuConvert::ToString ( enumText ) );
              }              
            }

            if ( STR != nullptr )
              row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, STR, GML_ATTRIBUT_TYP, CLASSIFIER_STRING, false );
            else
              row = generateLinkRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, CLASSIFIER_STRING, GML_ATTRIBUT_TYP, nullptr, false );
          }
          dataGridViewAttribute->Rows->Add( row );
        }
      }
      else
      if ( m_ShowEmptyAttributes )
      {
        row = generateTextRow( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, nullptr, false );
        dataGridViewAttribute->Rows->Add( row );
      }
      break;

    case _ENUMERATION:
    case _EXTERNAL_CODE_LIST:
      attributWertAnz = pPlanObjekt->getStringAttributWerte ( gmlAttributName, stringAttributWerte  );
      enumerationsName = pGmlAttribut->getGmlEnumeration();
      pEntry = pCodeLists->getDictionaryEntry ( enumerationsName );

      if ( attributWertAnz > 0 )
      {
        for ( j = 0; j < attributWertAnz; j++ )
        {
          stringAttributWert = stringAttributWerte[j];

          if ( stringAttributWert->isNil )
          {
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil", 
              GML_ATTRIBUT_TYP, QuConvert::ToString( stringAttributWert->getNilReason() ), false );
          }
          else
          {
            CLASSIFIER_STRING = nullptr;
            GmlDictionaryEntry * pCodelistEntry = NULL;
            for ( iterClassifier = stringAttributWert->classifier.begin(); iterClassifier != stringAttributWert->classifier.end(); iterClassifier++   )
            {
              ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
              ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );

              if ( iterClassifier->first == "codeSpace" )
              {
                string codeListPath = iterClassifier->second;
               pCodelistEntry = pCodeLists->getDictionaryEntry( codeListPath );
              }

              if ( iterClassifier == stringAttributWert->classifier.begin() )
                CLASSIFIER_STRING = String::Concat( ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
              else
                CLASSIFIER_STRING = String::Concat( CLASSIFIER_STRING, ";", ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
            }

            if ( pCodelistEntry == NULL )
              pCodelistEntry = pEntry;

            if ( pCodelistEntry != NULL  )
            {
              success = pCodelistEntry->getSchluesselText ( stringAttributWert->value, enumText );
              if ( success )
              {
                STR = String::Concat( QuConvert::ToString ( enumText ), " (", QuConvert::ToString(stringAttributWert->value) , ")" );
                if ( m_EditEnabled )
                  row = generateComboBoxRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, STR, GML_ATTRIBUT_TYP, CLASSIFIER_STRING, pEntry );
                else
                  row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, STR, GML_ATTRIBUT_TYP, CLASSIFIER_STRING, false );
              }
              else
              {
                STR = QuConvert::ToString( stringAttributWert->value );
                row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, STR, GML_ATTRIBUT_TYP, CLASSIFIER_STRING, false );
              }
            }
            else
            {
              STR = QuConvert::ToString( stringAttributWert->value );
              row = generateLinkRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, STR, GML_ATTRIBUT_TYP, CLASSIFIER_STRING, false );
            }

          }
          dataGridViewAttribute->Rows->Add( row );
        }
      }
      else
      if ( m_ShowEmptyAttributes )
      {
        if ( pEntry != NULL  )
        {
          if ( m_EditEnabled )
            row = generateComboBoxRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, "", pEntry );
          else
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, "", false );
        }
        else
          row = generateTextRow( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, nullptr, false );
        dataGridViewAttribute->Rows->Add( row );
      }
      break;

    case _ANY_URI:
      attributWertAnz = pPlanObjekt->getURLAttributWerte ( gmlAttributName, stringAttributWerte );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          stringAttributWert = stringAttributWerte[j];

          if ( stringAttributWert->isNil )
          {
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil", 
              GML_ATTRIBUT_TYP, QuConvert::ToString( stringAttributWert->getNilReason() ), false );
          }
          else
          {
            CLASSIFIER_STRING = nullptr;
            for ( iterClassifier = stringAttributWert->classifier.begin(); iterClassifier != stringAttributWert->classifier.end(); iterClassifier++   )
            {
              ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first);
              ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
              if (iterClassifier == stringAttributWert->classifier.begin() )
              CLASSIFIER_STRING = String::Concat( ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
            else
              CLASSIFIER_STRING = String::Concat( CLASSIFIER_STRING, ";", ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
            }
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, QuConvert::ToString( stringAttributWert->value ), 
              GML_ATTRIBUT_TYP, CLASSIFIER_STRING, false );
         }

          dataGridViewAttribute->Rows->Add( row );
        }
      else
      if ( m_ShowEmptyAttributes )
      {
        row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, nullptr, false );
        dataGridViewAttribute->Rows->Add( row );
      }
      break;

    case _DATE:
    case _DATE_TIME:
    case TIME_POSITION:
      attributWertAnz = pPlanObjekt->getDateAttributWerte ( gmlAttributName, dateAttributWerte );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          DateAttr * pDateAttr = dateAttributWerte[j];
          if ( pDateAttr->isNil )
          {
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil", 
              GML_ATTRIBUT_TYP, QuConvert::ToString( pDateAttr->getNilReason() ), false );
          }
          else
          if ( pDateAttr->value != NULL )
          {
            TimePosition * pTime = (TimePosition*)(dateAttributWerte[j]->value);
            string stdString = pTime->toXMLString();                    
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, QuConvert::ToString( stdString ), 
                                  GML_ATTRIBUT_TYP, nullptr, false );
          }
          dataGridViewAttribute->Rows->Add( row );
        }
      else
      if ( m_ShowEmptyAttributes )
      {
        row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, nullptr, false );
        dataGridViewAttribute->Rows->Add( row );
      }
      break;

    case TIME_PERIOD:
      attributWertAnz = pPlanObjekt->getDateAttributWerte ( gmlAttributName, dateAttributWerte );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          DateAttr * pDateAttr = dateAttributWerte[j];
          if ( pDateAttr->isNil )
          {
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, QuConvert::ToString( "Nil" ), 
              GML_ATTRIBUT_TYP, QuConvert::ToString( pDateAttr->getNilReason() ), false );
          }
          else
          {
            TimePeriod * pTimePeriod = (TimePeriod*)(pDateAttr->value);
            string startDate = pTimePeriod->getStartPosition().toXMLString();
            string endDate   = pTimePeriod->getEndPosition().toXMLString();
            string displayString = startDate + " - " + endDate;

            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, QuConvert::ToString( displayString ), 
              GML_ATTRIBUT_TYP, nullptr, false );
          }
          dataGridViewAttribute->Rows->Add( row );
        }
      else
        if ( m_ShowEmptyAttributes )
        {
          row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, nullptr, false );
          dataGridViewAttribute->Rows->Add( row );
        }
        break;

    case REGULAR_TIME_SERIES:
    case REGULAR_TIME_SERIES_FILE:
    case IRREGULAR_TIME_SERIES:
    case IRREGULAR_TIME_SERIES_FILE:
    case ABSTRACT_TIME_SERIES:
      {
        vector<AbstractTimeSeries*> vTimeSeries;
        map<UOM*, int>              mChartMap;
        List<double>              ^ pValues = gcnew List<double>;

        attributWertAnz = pPlanObjekt->getAllTimeSeries ( gmlAttributName, vTimeSeries );
        string displayString ="";

        row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, QuConvert::ToString( displayString ), 
          GML_ATTRIBUT_TYP, nullptr, false );
        dataGridViewAttribute->Rows->Add( row );

        if ( level == 0 )
        {
          TimePeriod * pTimePeriod = getTemporalExtent ( vTimeSeries );
          if ( pTimePeriod == NULL )
            break;

          TimeIntervalLength * pTimeSpan = getTimeSpan ( vTimeSeries );
          if ( pTimeSpan == NULL )
            break;

          createChartMap ( vTimeSeries, mChartMap );

          Diagramm ^ pDiagramm = gcnew Diagramm;

          int year   = pTimePeriod->getStartPosition().getYear();
          int month  = pTimePeriod->getStartPosition().getMonth();
          int day    = pTimePeriod->getStartPosition().getDay();
          int hour   = pTimePeriod->getStartPosition().getHour();
          int minute = pTimePeriod->getStartPosition().getMinute();
          int second =  pTimePeriod->getStartPosition().getSecond();
          pDiagramm->setStartTime ( year, month, day, hour, minute, second );

          year   = pTimePeriod->getEndPosition().getYear();
          month  = pTimePeriod->getEndPosition().getMonth();
          day    = pTimePeriod->getEndPosition().getDay();
          hour   = pTimePeriod->getEndPosition().getHour();
          minute = pTimePeriod->getEndPosition().getMinute();
          second =  pTimePeriod->getEndPosition().getSecond();
          pDiagramm->setEndTime ( year, month, day, hour, minute, second );

          switch ( pTimeSpan->getUnit() )
          {
          case TimeIntervalLength::DAY:
            pDiagramm->setTimeIncrement ( pTimeSpan->getValue(), 0, 0, 0 );
            break;

          case TimeIntervalLength::HOUR:
            pDiagramm->setTimeIncrement ( 0, pTimeSpan->getValue(), 0, 0 );
            break;

          case TimeIntervalLength::MINUTE:
            pDiagramm->setTimeIncrement ( 0, 0, pTimeSpan->getValue(), 0 );
            break;

          case TimeIntervalLength::SECOND:
            pDiagramm->setTimeIncrement ( 0, 0, 0, pTimeSpan->getValue() );
            break;
          }

          for ( j = 0; j < attributWertAnz; j++ )
          {
            AbstractTimeSeries * pTimeSeries = vTimeSeries[j];
            if ( pTimeSeries->getTimeSeriesType() == AbstractTimeSeries::Irregular || 
                 pTimeSeries->getTimeSeriesType() == AbstractTimeSeries::IrregularFile )
                 continue;

            pValues->Clear();
            UOM * pUom = pTimeSeries->getUOM();
            int chartNum = mChartMap[pUom];

            string uom;
            if ( pUom != NULL )
              uom =  pUom->getSign();
            else
              uom = "";

            Diagramm::AGGREGATION_TYP aggregationTyp = Diagramm::AGGREGATION_TYP::MITTELWERT;
            if ( pUom != NULL )
            {
              if ( pUom->getQuantity() == "ENERGY" || pUom->getQuantity() == "ENERGY_PER_AREA" )
                aggregationTyp = Diagramm::AGGREGATION_TYP::SUMME;
            }

            for ( unsigned int k = 0; k <  ((RegularTimeSeries*)pTimeSeries)->getValueCount(); k++  )
              pValues->Add( ((RegularTimeSeries*)pTimeSeries)->getValue( k ) );
            pDiagramm->setValues( pValues, QuConvert::ToString (  pTimeSeries->getThematicDescription() ), QuConvert::ToString( uom ), chartNum, aggregationTyp );
          }

          pDiagramm->Show();
        }
        break;

      }

    case _ANGLE:
    case _LENGTH:
    case _AREA:
    case _VOLUME:
    case _DOUBLE:
    case _MEASURE:
      attributWertAnz = pPlanObjekt->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          doubleAttributWert = doubleAttributWerte[j];

          if ( doubleAttributWert->isNil )
          {
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil", 
              GML_ATTRIBUT_TYP, QuConvert::ToString( doubleAttributWert->getNilReason() ), false );
          }
          else
          {
            CLASSIFIER_STRING = nullptr;

            if ( doubleAttributWert->uom != NULL )
            {
              CLASSIFIER_STRING = QuConvert::ToString( doubleAttributWert->uom->getSign() );
              CLASSIFIER_STRING = String::Concat( "uom=",CLASSIFIER_STRING, ";" );
            }

            for ( iterClassifier = doubleAttributWert->classifier.begin(); iterClassifier != doubleAttributWert->classifier.end(); iterClassifier++ )
            {
              ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
              ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
              if (iterClassifier == doubleAttributWert->classifier.begin() )
                CLASSIFIER_STRING = String::Concat( ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
              else
                CLASSIFIER_STRING = String::Concat( CLASSIFIER_STRING, ";", ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
            }

            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, XmlConvert::ToString( doubleAttributWert->value ), 
              GML_ATTRIBUT_TYP, CLASSIFIER_STRING, false );
          }
          dataGridViewAttribute->Rows->Add( row );
        }
      else
      if ( m_ShowEmptyAttributes )
      {
        row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, nullptr, false );
        dataGridViewAttribute->Rows->Add( row );
      }
      break;

    case _INTEGER:
      attributWertAnz = pPlanObjekt->getIntegerAttributWerte ( gmlAttributName, intAttributWerte );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          intAttributWert = intAttributWerte[j];

          if ( intAttributWert->isNil )
          {
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil", 
              GML_ATTRIBUT_TYP, QuConvert::ToString( intAttributWert->getNilReason() ), false );
          }
          else
          {
            CLASSIFIER_STRING = nullptr;
            for ( iterClassifier = intAttributWert->classifier.begin(); iterClassifier != intAttributWert->classifier.end(); iterClassifier++ )
            {
              ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
              ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second);
              if (iterClassifier == intAttributWert->classifier.begin() )
                CLASSIFIER_STRING = String::Concat( ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
              else
                CLASSIFIER_STRING = String::Concat( CLASSIFIER_STRING, ";", ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
            }

            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, XmlConvert::ToString( intAttributWert->value ), 
              GML_ATTRIBUT_TYP, CLASSIFIER_STRING, false );
          }

          dataGridViewAttribute->Rows->Add( row );
        }
      else
      if ( m_ShowEmptyAttributes )
      {
        row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, nullptr, false );
        dataGridViewAttribute->Rows->Add( row );
      }
      break;

    case _BOOLEAN:
      boolAttributWert = pPlanObjekt->getBoolAttributWert ( gmlAttributName );
      if ( boolAttributWert != NULL )
      {
        if ( boolAttributWert->isNil )
        {
          row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil", 
            GML_ATTRIBUT_TYP, QuConvert::ToString( boolAttributWert->getNilReason() ), false );
        }
        else
        {
          CLASSIFIER_STRING = nullptr;
          for ( iterClassifier = boolAttributWert->classifier.begin(); iterClassifier != boolAttributWert->classifier.end(); iterClassifier++ )
          {
            ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
            ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second);
            if (iterClassifier == boolAttributWert->classifier.begin() )
              CLASSIFIER_STRING = String::Concat( ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
            else
              CLASSIFIER_STRING = String::Concat( CLASSIFIER_STRING, ";", ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
          }

          if ( m_EditEnabled )
            row = generateBooleanRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, boolAttributWert->value, 
            GML_ATTRIBUT_TYP, CLASSIFIER_STRING );
          else
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, XmlConvert::ToString( boolAttributWert->value ), 
            GML_ATTRIBUT_TYP, CLASSIFIER_STRING, false );
        }
        dataGridViewAttribute->Rows->Add( row );
      }
      else
      if ( m_ShowEmptyAttributes )
      {
        row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", GML_ATTRIBUT_TYP, nullptr, false );
        dataGridViewAttribute->Rows->Add( row );
      }
      break;


    case _GENER_LIST:
      attributWertAnz = pPlanObjekt->getFeatureRelationen ( "_GenericAttributeSet", relationen );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          pRelation = relationen[j];
          klassenNameRelationsZiel = pGmlAttribut->getReferenziertesElement();

          pRelationZiel = m_pFeatures->getFeature ( pRelation->featureId );
          if ( pRelationZiel != NULL )
          {
            pRelationZiel->getStringAttributWert ( m_pSchemaTypeNamesStandard->genericNameAttribute, genericAttributName );
            STR = QuConvert::ToString( genericAttributName );

            genericAttributNamen.clear();
            stringAttributWerte.clear();
            anz = pRelationZiel->getStringAttributWerte( genericAttributNamen, stringAttributWerte );
            if ( anz > 0 )
            {
              pRelationZiel->getStringAttributWert ( m_pSchemaTypeNamesStandard->genericNameAttribute, genericAttributName );
              GML_ATTRIBUT_LISTEN_NAME = QuConvert::ToString ( genericAttributName );
              row = generateTextRow ( attributeColor, highlighted, "Attribut-Set", GML_ATTRIBUT_LISTEN_NAME , "Listen-Name", "", false );
              dataGridViewAttribute->Rows->Add( row );
              for ( ii = 0; ii < anz; ii++ )
              {
                GML_ATTRIBUT_NAME = QuConvert::ToString( genericAttributNamen[ii] );
                GML_ATTRIBUT_WERT = QuConvert::ToString( stringAttributWerte[ii]->value );
                if ( String::Compare( GML_ATTRIBUT_WERT, GML_ATTRIBUT_LISTEN_NAME ) == 0 ) continue;

                GML_ATTRIBUT_NAME = String::Concat ( "  ", STR, ":", GML_ATTRIBUT_NAME );

                row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , "String-Attribut", "", false );
                dataGridViewAttribute->Rows->Add( row );
              }
            }

            genericAttributNamen.clear();
            dateAttributWerte.clear();
            anz = pRelationZiel->getDateAttributWerte( genericAttributNamen, dateAttributWerte );
            for ( ii = 0; ii < anz; ii++ )
            {
              DateAttr * pDateAttr = dateAttributWerte[ii];
              GML_ATTRIBUT_NAME = String::Concat ( STR, ":", QuConvert::ToString( genericAttributNamen[ii] ) );

              if ( pDateAttr->isNil )
              {
                row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil" , "Datum-Attribut",
                  QuConvert::ToString( pDateAttr->getNilReason()), false );
              }
              else
              {
                TimePosition * pTimePosition = (TimePosition*)pDateAttr->value;


                GML_ATTRIBUT_WERT = QuConvert::ToString( pTimePosition->toXMLString() );
                row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , "Datum-Attribut", "", false );
              }
              dataGridViewAttribute->Rows->Add( row );
           }

            genericAttributNamen.clear();
            stringAttributWerte.clear();
            anz = pRelationZiel->getURLAttributWerte( genericAttributNamen, stringAttributWerte );
            for ( ii = 0; ii < anz; ii++ )
            {
              GML_ATTRIBUT_NAME = String::Concat ( STR, ":", QuConvert::ToString( genericAttributNamen[ii] ) );
              GML_ATTRIBUT_WERT = QuConvert::ToString( stringAttributWerte[ii]->value );
              row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , "URL-Attribut", "", false );
              dataGridViewAttribute->Rows->Add( row );
            }

            genericAttributNamen.clear();
            intAttributWerte.clear();
            anz = pRelationZiel->getIntegerAttributWerte( genericAttributNamen, intAttributWerte );
            for ( ii = 0; ii < anz; ii++ )
            {
              GML_ATTRIBUT_NAME = String::Concat ( STR, ":", QuConvert::ToString( genericAttributNamen[ii] ) );
              GML_ATTRIBUT_WERT = QuConvert::ToString( intAttributWerte[ii]->value );
              row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , "Integer-Attribut", "", false );
              dataGridViewAttribute->Rows->Add( row );
            }

            genericAttributNamen.clear();
            doubleAttributWerte.clear();
            anz = pRelationZiel->getDoubleAttributWerte( genericAttributNamen, doubleAttributWerte );
            for ( ii = 0; ii < anz; ii++ )
            {
              doubleAttributWert = doubleAttributWerte[ii];
              GML_ATTRIBUT_NAME = String::Concat ( STR, ":", QuConvert::ToString( genericAttributNamen[ii] ) );
              GML_ATTRIBUT_WERT = QuConvert::ToString( doubleAttributWert->value );

              CLASSIFIER_STRING = nullptr;
              for ( iterClassifier = doubleAttributWert->classifier.begin(); iterClassifier != doubleAttributWert->classifier.end(); iterClassifier++ )
              {
                ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
                ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
                if ( iterClassifier == doubleAttributWert->classifier.begin() )
                  CLASSIFIER_STRING = String::Concat( ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
                else
                  CLASSIFIER_STRING = String::Concat( CLASSIFIER_STRING, ";", ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
              }

              if ( CLASSIFIER_STRING == nullptr )
                row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , "Double-Attribut", "", false );
              else
                row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , "Measure-Attribut", CLASSIFIER_STRING, false );
              dataGridViewAttribute->Rows->Add( row );
            }
          }
        }
        break;


    case _GENER_STRING:
      attributWertAnz = pPlanObjekt->getFeatureRelationen ( "_GenericStringAttribute", relationen );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          pRelation = relationen[j];
          klassenNameRelationsZiel = pGmlAttribut->getReferenziertesElement();

          pRelationZiel = m_pFeatures->getFeature ( pRelation->featureId );
          if ( pRelationZiel != NULL )
          {
            pRelationZiel->getStringAttributWert ( m_pSchemaTypeNamesStandard->genericNameAttribute, genericAttributName );
            GML_ATTRIBUT_NAME = QuConvert::ToString( genericAttributName );
            pRelationZiel->getStringAttributWert ( m_pSchemaTypeNamesStandard->genericValueAttribute, genericAttributWert );
            GML_ATTRIBUT_WERT = QuConvert::ToString( genericAttributWert );

            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , 
              QuConvert::ToString ( GmlSchema::gmlAttributTypToString (gmlAttributTyp) ), "", false );
            dataGridViewAttribute->Rows->Add( row );
          }
        }
        break;

    case _GENER_DATE:
      attributWertAnz = pPlanObjekt->getFeatureRelationen ( "_GenericDateAttribute", relationen );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          pRelation = relationen[j];
          klassenNameRelationsZiel = pGmlAttribut->getReferenziertesElement();

          pRelationZiel = m_pFeatures->getFeature ( pRelation->featureId );
          if ( pRelationZiel != NULL )
          {
            pRelationZiel->getStringAttributWert ( m_pSchemaTypeNamesStandard->genericNameAttribute, genericAttributName );
            GML_ATTRIBUT_NAME = QuConvert::ToString( genericAttributName );
            TimePosition * pTime = (TimePosition*)pRelationZiel->getDateAttributWert ( m_pSchemaTypeNamesStandard->genericValueAttribute );
            if ( pTime != NULL )
            {
              GML_ATTRIBUT_WERT = QuConvert::ToString( pTime->toXMLString() );

              row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , 
                                    QuConvert::ToString ( GmlSchema::gmlAttributTypToString (gmlAttributTyp) ), "", false );
              dataGridViewAttribute->Rows->Add( row );
            }
          }
        }
        break;


    case _GENER_URL:
      attributWertAnz = pPlanObjekt->getFeatureRelationen ( "_GenericURLAttribute", relationen );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          pRelation = relationen[j];
          klassenNameRelationsZiel = pGmlAttribut->getReferenziertesElement();

          pRelationZiel = m_pFeatures->getFeature ( pRelation->featureId );
          if ( pRelationZiel != NULL )
          {
            pRelationZiel->getStringAttributWert ( m_pSchemaTypeNamesStandard->genericNameAttribute, genericAttributName );
            GML_ATTRIBUT_NAME = QuConvert::ToString( genericAttributName );
            pRelationZiel->getURLAttributWert ( m_pSchemaTypeNamesStandard->genericValueAttribute, genericAttributWert );
            GML_ATTRIBUT_WERT = QuConvert::ToString( genericAttributWert );

            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , 
                                    QuConvert::ToString ( GmlSchema::gmlAttributTypToString (gmlAttributTyp) ), "", false );
            dataGridViewAttribute->Rows->Add( row );
          }
        }
        break;


    case _GENER_INTEGER:
      attributWertAnz = pPlanObjekt->getFeatureRelationen ( "_GenericIntegerAttribute", relationen );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          pRelation = relationen[j];
          klassenNameRelationsZiel = pGmlAttribut->getReferenziertesElement();

          pRelationZiel = m_pFeatures->getFeature ( pRelation->featureId );
          if ( pRelationZiel != NULL )
          {
            pRelationZiel->getStringAttributWert ( m_pSchemaTypeNamesStandard->genericNameAttribute, genericAttributName );
            GML_ATTRIBUT_NAME = QuConvert::ToString( genericAttributName );
            pRelationZiel->getIntegerAttributWert ( m_pSchemaTypeNamesStandard->genericValueAttribute, genericIntAttributWert );
            GML_ATTRIBUT_WERT = QuConvert::ToString( genericIntAttributWert );

            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , 
                                    QuConvert::ToString ( GmlSchema::gmlAttributTypToString (gmlAttributTyp) ), "", false );
            dataGridViewAttribute->Rows->Add( row );
          }
        }
        break;


    case _GENER_DOUBLE:
      attributWertAnz = pPlanObjekt->getFeatureRelationen ( "_GenericDoubleAttribute", relationen );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          pRelation = relationen[j];
          klassenNameRelationsZiel = pGmlAttribut->getReferenziertesElement();

          pRelationZiel = m_pFeatures->getFeature ( pRelation->featureId );
          if ( pRelationZiel != NULL )
          {
            pRelationZiel->getStringAttributWert ( m_pSchemaTypeNamesStandard->genericNameAttribute, genericAttributName );
            GML_ATTRIBUT_NAME = QuConvert::ToString( genericAttributName );
            pRelationZiel->getDoubleAttributWert ( m_pSchemaTypeNamesStandard->genericValueAttribute, genericDoubleAttributWert );
            GML_ATTRIBUT_WERT = QuConvert::ToString( genericDoubleAttributWert );

            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , 
                                    QuConvert::ToString ( GmlSchema::gmlAttributTypToString (gmlAttributTyp) ), "", false );
            dataGridViewAttribute->Rows->Add( row );
          }
        }
        break;


    case _GENER_MEASURE:
      attributWertAnz = pPlanObjekt->getFeatureRelationen ( "_GenericMeasureAttribute", relationen );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          pRelation = relationen[j];
          klassenNameRelationsZiel = pGmlAttribut->getReferenziertesElement();

          pRelationZiel = m_pFeatures->getFeature ( pRelation->featureId );
          if ( pRelationZiel != NULL )
          {
            DoubleAttr * pDoubleAttr;
            classifierNamen.clear();
            classifierValues.clear();
            pRelationZiel->getStringAttributWert ( m_pSchemaTypeNamesStandard->genericNameAttribute, genericAttributName );
            GML_ATTRIBUT_NAME = QuConvert::ToString( genericAttributName );
            pDoubleAttr = pRelationZiel->getDoubleAttributWert ( m_pSchemaTypeNamesStandard->genericValueAttribute );
            GML_ATTRIBUT_WERT = QuConvert::ToString( pDoubleAttr->value );

            if ( pDoubleAttr->uom != NULL )
            {
              CLASSIFIER_STRING = QuConvert::ToString( pDoubleAttr->uom->getName() );
            }
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT , 
                                    QuConvert::ToString ( GmlSchema::gmlAttributTypToString (gmlAttributTyp) ), CLASSIFIER_STRING, false );
            dataGridViewAttribute->Rows->Add( row );
          }
        }
        break;

    case _DATA_TYPE:
      if ( !m_ExpandDataTypes ) break;

      attributWertAnz = pPlanObjekt->getFeatureRelationen ( gmlAttributName, relationen );
      if ( attributWertAnz > 0 )
        for ( j = 0; j < attributWertAnz; j++ )
        {
          pRelation = relationen[j];

          if ( pRelation->nil )
          {
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil", 
              GML_ATTRIBUT_TYP, QuConvert::ToString( pRelation->nilReason ), false );
            dataGridViewAttribute->Rows->Add( row );
            continue;
          }


          CLASSIFIER_STRING = nullptr;
          for ( k = 0; k < pRelation->classifierName.size(); k++ )
          {
            ATTRIBUT_NAME = QuConvert::ToString ( pRelation->classifierName[k] );
            ATTRIBUT_WERT = QuConvert::ToString ( pRelation->classifierValue[k] );
            if ( k == 0 )
              CLASSIFIER_STRING = String::Concat( ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
            else
              CLASSIFIER_STRING = String::Concat( CLASSIFIER_STRING, ";", ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
          }

          pRelationZiel = m_pFeatures->getFeature ( pRelation->featureId );
          if ( pRelationZiel != NULL )
          {
            pSimpleContent = pRelationZiel->getSimpleContent();
            if ( pSimpleContent != NULL )
            {
              String ^ attributWert;
              attributWert = QuConvert::ToString ( pSimpleContent->value );

              CLASSIFIER_STRING = nullptr;
              for ( iterClassifier = pSimpleContent->classifier.begin(); iterClassifier != pSimpleContent->classifier.end(); iterClassifier++ )
              {
                ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
                ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
                if ( iterClassifier == pSimpleContent->classifier.begin() )
                  CLASSIFIER_STRING = String::Concat( ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
                else
                  CLASSIFIER_STRING = String::Concat( CLASSIFIER_STRING, ";", ATTRIBUT_NAME, "=", ATTRIBUT_WERT );
              }

              row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, attributWert , "Text", CLASSIFIER_STRING, true );
              dataGridViewAttribute->Rows->Add( row );
           }
           else
           {
             string relZielTyp = pRelationZiel->getKlassenName();
             row = generateTextRow ( dataTypeColor, highlighted, GML_ATTRIBUT_NAME, QuConvert::ToString( relZielTyp), GML_ATTRIBUT_TYP, CLASSIFIER_STRING, true );
             dataGridViewAttribute->Rows->Add( row );
             if ( level == 0 )
               writeRows ( pRelationZiel, true, level + 1 );
           }
         }
          else
          {
            STR = QuConvert::ToString(  pRelation->featureId );
            row = generateLinkRow ( dataTypeColor, highlighted, GML_ATTRIBUT_NAME, STR , GML_ATTRIBUT_TYP, CLASSIFIER_STRING, true );
            dataGridViewAttribute->Rows->Add( row );
          }
        }
      else
      if ( m_ShowEmptyAttributes )
      {
        klassenNameRelationsZiel = pGmlAttribut->getReferenziertesElement();
        row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "", QuConvert::ToString( klassenNameRelationsZiel ), 
                                 nullptr, true );
        dataGridViewAttribute->Rows->Add( row );
      }
      break;

    case _REFERENZ:
      attributWertAnz = pPlanObjekt->getFeatureRelationen ( gmlAttributName, relationen );
      if ( attributWertAnz > 0 )
      {
        for ( j = 0; j < attributWertAnz; j++ )
        {
          FeatureRelation * pRel =  relationen[j];

          if ( pRel->nil )
          {
            row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, "Nil", 
              GML_ATTRIBUT_TYP, QuConvert::ToString( pRel->nilReason ), false );
            dataGridViewAttribute->Rows->Add( row );
          }
          else
          {
            string            objId = pRel->featureId;
            Feature         * pRelatedFeature = pFeatures->getFeature( objId );
            if ( pRelatedFeature != NULL )
              STR = QuConvert::ToString(  pRelatedFeature->getKlassenName() );
            else
              STR = "";
            row = generateLinkRow ( dataTypeColor, highlighted, GML_ATTRIBUT_NAME, STR , 
                GML_ATTRIBUT_TYP, QuConvert::ToString( objId ), false );
            dataGridViewAttribute->Rows->Add( row );            
          }
        }
      }
      break;

    case _GEOMETRIE:
      {
        _Geometrie * pGeo = pPlanObjekt->getGeometry( gmlAttributName, 0 );
        if ( pGeo != NULL )
        {       
          STR =  QuConvert::ToString ( _Geometrie::convertTypeToString ( pGeo->getGeometryType() ) );
          row = generateTextRow ( dataTypeColor, highlighted, GML_ATTRIBUT_NAME, STR , GML_ATTRIBUT_TYP, CLASSIFIER_STRING, true );
          dataGridViewAttribute->Rows->Add( row );
        }
      }
      break;

    case _DIRECT_POSITION:
      pDirectPosition =  dynamic_cast<DirectPosition*>(pPlanObjekt->getGeometry (gmlAttributName, 0 ));
      if ( pDirectPosition != NULL )
      {
        vDirectPosition.clear();
        pDirectPosition->get( vDirectPosition );

        DIRECT_POSITION = "";
        for ( k = 0; k < vDirectPosition.size(); k++ )
        {
          DIRECT_POSITION = String::Concat( DIRECT_POSITION, QuConvert::ToString( vDirectPosition[k]) );
          if ( k < vDirectPosition.size() - 1 )
            DIRECT_POSITION = String::Concat( DIRECT_POSITION,", " );
        }

        row = generateTextRow ( attributeColor, highlighted, GML_ATTRIBUT_NAME, DIRECT_POSITION, "Position", nullptr, true );
        dataGridViewAttribute->Rows->Add( row );
      }
      break;


    case _POS_XY:
      break;

    case _SONST:
      break;
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Anzeige einer XAL-Adresse                                                //
///////////////////////////////////////////////////////////////////////////////
Void writeXalAddress ( Feature * pAddress )
{
  vector<string>        vAttributNamen; 
  vector<StringAttr*>   vAttributWerte;
	size_t                i, anz;
  String              ^ GML_ATTRIBUT_NAME;
  String              ^ GML_ATTRIBUT_TYP;
  String              ^ GML_ATTRIBUT_WERT;
  DataGridViewRow     ^ row;


  anz = pAddress->getStringAttributWerte( vAttributNamen, vAttributWerte );
  for ( i = 0; i < anz; i++ )
  {
    GML_ATTRIBUT_NAME = QuConvert::ToString ( vAttributNamen[i] );
    GML_ATTRIBUT_WERT = QuConvert::ToString ( vAttributWerte[i]->value );
    GML_ATTRIBUT_TYP  = "Addressangabe";

    row = generateTextRow ( attributeColor, false, GML_ATTRIBUT_NAME, GML_ATTRIBUT_WERT, GML_ATTRIBUT_TYP, "", false );
    dataGridViewAttribute->Rows->Add( row );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine Zeile (Row) der Anzeigetabelle                            //
///////////////////////////////////////////////////////////////////////////////
DataGridViewRow ^ generateTextRow ( Color rowColor, bool highlighted, String ^ attributName, String ^ attributWert, String ^ attributTyp,
                                   String ^ classifier, bool disableEdit )
{
  DataGridViewRow          ^ row;
  DataGridViewTextBoxCell  ^ cell;
  DataGridViewCellStyle    ^ cellStyle;
  DataGridViewCellStyle    ^ cellStyleEdit;

  row = gcnew DataGridViewRow;

  cellStyle = gcnew DataGridViewCellStyle;
  cellStyle->BackColor = rowColor;
  if ( highlighted )
    cellStyle->ForeColor = highlightColor;
  else
    cellStyle->ForeColor = Color::Black;

  if ( m_EditEnabled )
  {
    cellStyleEdit = gcnew DataGridViewCellStyle;
    cellStyleEdit->BackColor = editColor;
    if ( highlighted )
      cellStyleEdit->ForeColor = highlightColor;
    else
      cellStyleEdit->ForeColor = Color::Black;
  }

  //  1. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Style    = cellStyle;
  cell->Value    = attributName;
  row->Cells->Add( cell );
  cell->ReadOnly = true;


  //  2. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Style    = cellStyle;
  cell->Value    =  attributTyp;
  row->Cells->Add( cell );
  cell->ReadOnly = true;

  //  3. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Value    =  attributWert;
  row->Cells->Add( cell );
  if ( m_EditEnabled && !disableEdit )
  {
    cell->ReadOnly = false;
    cell->Style    = cellStyleEdit;
  }
 else
 {
    cell->ReadOnly = true;
    cell->Style    = cellStyle;
  }

  // 4. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Value    =  classifier;
  row->Cells->Add( cell );
  if ( m_EditEnabled && !disableEdit )
  {
    cell->ReadOnly = false;
    cell->Style    = cellStyleEdit;
  }
  else
  {
    cell->ReadOnly = true;
    cell->Style    = cellStyle;
  }

  return row;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine Zeile der Anzeigetabelle, bei der in der 4. Zelle Links   //
//  vorhanden sind                                                           //
///////////////////////////////////////////////////////////////////////////////
DataGridViewRow ^ generateLinkRow ( Color rowColor, bool highlighted, String ^ attributName, String ^ attributWert, String ^ attributTyp,
  String ^ classifier, bool disableEdit )
{
  DataGridViewRow          ^ row;
  DataGridViewTextBoxCell  ^ cell;
  DataGridViewLinkCell     ^ linkCell;
  DataGridViewCellStyle    ^ cellStyle;
  DataGridViewCellStyle    ^ cellStyleEdit;

  row = gcnew DataGridViewRow;

  cellStyle = gcnew DataGridViewCellStyle;
  cellStyle->BackColor = rowColor;
  if ( highlighted )
    cellStyle->ForeColor = highlightColor;
  else
    cellStyle->ForeColor = Color::Black;

  if ( m_EditEnabled )
  {
    cellStyleEdit = gcnew DataGridViewCellStyle;
    cellStyleEdit->BackColor = editColor;
    if ( highlighted )
      cellStyleEdit->ForeColor = highlightColor;
    else
      cellStyleEdit->ForeColor = Color::Black;
  }

  //  1. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Style    = cellStyle;
  cell->Value    = attributName;
  row->Cells->Add( cell );
  cell->ReadOnly = true;


  //  2. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Style    = cellStyle;
  cell->Value    =  attributTyp;
  row->Cells->Add( cell );
  cell->ReadOnly = true;

  //  3. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Value    =  attributWert;
  row->Cells->Add( cell );
  if ( m_EditEnabled && !disableEdit )
  {
    cell->ReadOnly = false;
    cell->Style    = cellStyleEdit;
  }
  else
  {
    cell->ReadOnly = true;
    cell->Style    = cellStyle;
  }

  // 4. Zelle
  linkCell = gcnew DataGridViewLinkCell;
  linkCell->Value    =  classifier;
  row->Cells->Add( linkCell );
  if ( m_EditEnabled && !disableEdit )
  {
    linkCell->ReadOnly = false;
    linkCell->Style    = cellStyleEdit;
  }
  else
  {
    linkCell->ReadOnly = true;
    linkCell->Style    = cellStyle;
  }

  return row;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine Zeile der Anzeigetabelle, bei der in der 3. Zelle         //
//  eine ComboBox für die Auswahl ja/nein enthalten ist                      //
///////////////////////////////////////////////////////////////////////////////
DataGridViewRow ^ generateBooleanRow ( Color rowColor, bool highlighted, String ^ attributName, bool attributWert, 
                                       String ^ attributTyp, String ^ classifier )
{
  DataGridViewRow           ^ row;
  DataGridViewTextBoxCell   ^ cell;
  DataGridViewComboBoxCell  ^ comboBoxCell;
  DataGridViewCellStyle     ^ cellStyle;
  DataGridViewCellStyle     ^ cellStyleEdit;
  String                    ^ STR;

  row = gcnew DataGridViewRow;

  cellStyle = gcnew DataGridViewCellStyle;
  cellStyle->BackColor = rowColor;
  if ( highlighted )
    cellStyle->ForeColor = highlightColor;
  else
    cellStyle->ForeColor = Color::Black;

  cellStyleEdit = gcnew DataGridViewCellStyle;
  cellStyleEdit->BackColor = editColor;
  if ( highlighted )
    cellStyleEdit->ForeColor = highlightColor;
  else
    cellStyleEdit->ForeColor = Color::Black;


  //  1. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Style    = cellStyle;
  cell->Value    = attributName;
  row->Cells->Add( cell );
  cell->ReadOnly = true;

  //  2. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Style    = cellStyle;
  cell->Value    =  attributTyp;
  row->Cells->Add( cell );
  cell->ReadOnly = true;

  //  3. Zelle
  comboBoxCell = gcnew DataGridViewComboBoxCell;
  comboBoxCell->ReadOnly = false;
  comboBoxCell->Style    = cellStyleEdit;

  STR = "true";
  comboBoxCell->Items->Add( STR );
  STR = "false";
  comboBoxCell->Items->Add( STR );
  
  comboBoxCell->Value = XmlConvert::ToString ( attributWert);
  row->Cells->Add( comboBoxCell );

  // 4. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Value    =  classifier;
  row->Cells->Add( cell );
  if ( m_EditEnabled )
  {
    cell->ReadOnly = false;
    cell->Style    = cellStyleEdit;
  }
  else
  {
    cell->ReadOnly = true;
    cell->Style    = cellStyle;
  }

  return row;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine Zeile der Anzeigetabelle, bei der in der 3. Zelle         //
//  eine ComboBoy für Enumerationswerte enthalten ist                        //
///////////////////////////////////////////////////////////////////////////////
DataGridViewRow ^ generateComboBoxRow ( Color rowColor, bool highlighted, String ^ attributName, String ^ attributWert, 
                                       String ^ attributTyp, String ^ classifier, GmlDictionaryEntry * pEntry )
{
  DataGridViewRow           ^ row;
  DataGridViewTextBoxCell   ^ cell;
  DataGridViewComboBoxCell  ^ comboBoxCell;
  DataGridViewCellStyle     ^ cellStyle;
  DataGridViewCellStyle     ^ cellStyleEdit;
  vector<string>              schluesselNummern;
	size_t                      k, index, enumerationItemAnz;
  String                    ^ STR;
  string                      schluesselText;

  row = gcnew DataGridViewRow;

  cellStyle = gcnew DataGridViewCellStyle;
  cellStyle->BackColor = rowColor;
  if ( highlighted )
    cellStyle->ForeColor = highlightColor;
  else
    cellStyle->ForeColor = Color::Black;

  cellStyleEdit = gcnew DataGridViewCellStyle;
  cellStyleEdit->BackColor = editColor;
  if ( highlighted )
    cellStyleEdit->ForeColor = highlightColor;
  else
    cellStyleEdit->ForeColor = Color::Black;

  //  1. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Style    = cellStyle;
  cell->Value    = attributName;
  row->Cells->Add( cell );
  cell->ReadOnly = true;

  //  2. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Style    = cellStyle;
  cell->Value    =  attributTyp;
  row->Cells->Add( cell );
  cell->ReadOnly = true;

  //  3. Zelle
  comboBoxCell = gcnew DataGridViewComboBoxCell;
  comboBoxCell->ReadOnly = false;
  comboBoxCell->Style    = cellStyleEdit;

  index = -1;
  if ( pEntry != NULL )
  {
    comboBoxCell->Items->Add ( "" );

    enumerationItemAnz = pEntry->getCodelistSchluessel( schluesselNummern );
    for ( k = 0; k < enumerationItemAnz; k++ )
    {
      bool success = pEntry->getSchluesselText( schluesselNummern[k], schluesselText );

      STR = String::Concat( QuConvert::ToString ( schluesselText ), " (", QuConvert::ToString(schluesselNummern[k]) , ")" );
      comboBoxCell->Items->Add( STR );
    }
  }
  comboBoxCell->Value = attributWert;
  row->Cells->Add( comboBoxCell );

  // 4. Zelle
  cell = gcnew DataGridViewTextBoxCell;
  cell->Value    =  classifier;
  row->Cells->Add( cell );
  if ( m_EditEnabled )
  {
    cell->ReadOnly = false;
    cell->Style    = cellStyleEdit;
  }
  else
  {
    cell->ReadOnly = true;
    cell->Style    = cellStyle;
  }

  return row;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert, ob ein bestimmtes Attribut ausgewählt ist                       //
///////////////////////////////////////////////////////////////////////////////
bool isHighlighted ( String ^ attributName )
{
  String ^ highlightedName;

  if ( m_Highlight == nullptr ) return false;

  System::Collections::IEnumerator ^iter = m_Highlight->GetEnumerator();
  while ( iter->MoveNext() )
  {
    highlightedName = dynamic_cast<String^>(iter->Current);
    if ( attributName->CompareTo(highlightedName) == 0 )
      return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Start Editieren des Inhalts einer Zelle                  //
///////////////////////////////////////////////////////////////////////////////
private: System::Void dataGridViewAttribute_CellBeginEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellCancelEventArgs^  e)
{
  pRowSelected = dataGridViewAttribute->CurrentRow;
  if ( pRowSelected != nullptr )
  {
    m_AttributeNameSelected       = dynamic_cast<String^>(pRowSelected->Cells[0]->Value);
    m_AttributeTypeSelected       = dynamic_cast<String^>(pRowSelected->Cells[1]->Value);
    m_AttributeValueSelected      = dynamic_cast<String^>(pRowSelected->Cells[2]->Value);
    if ( m_AttributeValueSelected == nullptr ) m_AttributeValueSelected = "";
    m_AttributeClassifierSelected = dynamic_cast<String^>(pRowSelected->Cells[3]->Value);
    if ( m_AttributeClassifierSelected == nullptr ) m_AttributeClassifierSelected = "";
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Ende Editieren des Inhalts einer Zelle                   //
///////////////////////////////////////////////////////////////////////////////
private: System::Void dataGridViewAttribute_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e)
{
  attributwerteUebernehmen();
}

/////////////////////////////////////////////////////////////////////////////
//  Übernimmt geänderte Attributwerte                                      //
/////////////////////////////////////////////////////////////////////////////
public: System::Void attributwerteUebernehmen()
{
  String       ^ attributeValueNew;
  String       ^ attributeClassifierNew;
  string         attributName;
  string         attributTyp;
  string         attributWertAlt;
  string         attributWertNeu;
  ATTRIBUT_TYP   attrTyp;
  vector<string> classifierName;
  vector<string> classifierValue;

  if (pRowSelected == nullptr) return;

  String ^ tempValue = dynamic_cast<String^>(pRowSelected->Cells[2]->EditedFormattedValue);
  attributeValueNew = dynamic_cast<String^>(pRowSelected->Cells[2]->Value);
  if (tempValue != nullptr && tempValue->CompareTo(attributeValueNew) != 0)
    attributeValueNew = tempValue;

  tempValue     = dynamic_cast<String^>(pRowSelected->Cells[3]->EditedFormattedValue);
  attributeClassifierNew = dynamic_cast<String^>(pRowSelected->Cells[3]->Value);
  if (tempValue != nullptr && tempValue->CompareTo(attributeClassifierNew) != 0)
    attributeClassifierNew = tempValue;

  if (m_AttributeValueSelected->CompareTo(attributeValueNew) == 0 &&
    m_AttributeClassifierSelected->CompareTo(attributeClassifierNew) == 0)
    return;

  QuConvert::systemStr2stdStr(attributTyp, m_AttributeTypeSelected);
  attrTyp = GmlSchema::gmlStringToAttributTyp(attributTyp);

  QuConvert::systemStr2stdStr(attributName, m_AttributeNameSelected);
  QuConvert::systemStr2stdStr(attributWertAlt, m_AttributeValueSelected);
  QuConvert::systemStr2stdStr(attributWertNeu, attributeValueNew);

  extractClassifier(attributeClassifierNew, classifierName, classifierValue);

  if (attrTyp == _ENUMERATION || attrTyp == _EXTERNAL_CODE_LIST)
  {
    attributWertAlt = extractKey(attributWertAlt);
    attributWertNeu = extractKey(attributWertNeu);
  }

  if (attributWertNeu == "")
  {
    m_pFeature->deleteAttributWert(attrTyp, attributName, attributWertAlt);
    return;
  }

  m_pFeature->exchangeAttributWert(attrTyp, attributName, attributWertAlt, attributWertNeu, classifierName, classifierValue);
  m_AttributeValueSelected      = attributeValueNew;
  m_AttributeClassifierSelected = attributeClassifierNew;
  if (m_AttributeClassifierSelected == nullptr) 
    m_AttributeClassifierSelected = "";

}


///////////////////////////////////////////////////////////////////////////////
//  Extrahiert die einzelnen Classifier aus dem zusammengesetzten            //
//  classifierString                                                         //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void extractClassifier ( String ^ classifierString, vector<string> &vClassifierNamen, vector<string> &vClassifierValues )
{
  cli::array<String^> ^ splittedString;
  cli::array<String^> ^ splittedNameValuePair;
  int                   i;
  String              ^ nameValuePair;
  string                stdString;

  if ( classifierString == nullptr ) return;

  classifierString->Trim();
  if ( classifierString->Length == 0 ) return;

  splittedString = classifierString->Split ( ';' );
  for ( i = 0; i < splittedString->Length; i++ )
  {
    nameValuePair = splittedString[i];

    if ( !nameValuePair->Contains( "=") )
      break;

    splittedNameValuePair = nameValuePair->Split( '=' );
    if ( splittedNameValuePair->Length > 2 )
      break;

    QuConvert::systemStr2stdStr( stdString, splittedNameValuePair[0] );
    vClassifierNamen.push_back( stdString );

    QuConvert::systemStr2stdStr( stdString, splittedNameValuePair[1] );
    vClassifierValues.push_back( stdString );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Extrahiert den Schlüssel aus dem zusammengesetzten Schlüssel-Wert String //
///////////////////////////////////////////////////////////////////////////////
private: string extractKey ( string keyValueString )
{
	size_t indexA, indexE;
  string key;

  indexA = keyValueString.find( "(" );
  indexE = keyValueString.find( ")" );

  key = keyValueString.substr ( indexA+1, indexE - indexA - 1 );
  return key;
}

///////////////////////////////////////////////////////////////////////////////
//  Extrahiert die Zeitspanne /Temporal Extent) einer Menge regulärer        //    
//  Zeitreihen, die alle den gleichen Temporal Extent haben                  //
///////////////////////////////////////////////////////////////////////////////
private: TimePeriod * getTemporalExtent ( vector<AbstractTimeSeries*> vTimeSeries )
{
 unsigned int i;

  if ( vTimeSeries.size() == 0 )
    return NULL;

  for ( i = 0; i < vTimeSeries.size(); i++  )
  {
    AbstractTimeSeries * pTimeSeries = vTimeSeries[i];
    if ( pTimeSeries->getTimeSeriesType() == AbstractTimeSeries::Irregular || 
         pTimeSeries->getTimeSeriesType() == AbstractTimeSeries::IrregularFile )
         return NULL;
  }
  RegularTimeSeries * pTimeSeries = (RegularTimeSeries*)vTimeSeries[0];
  TimePeriod * pTimePeriod = pTimeSeries->getTemporalExtent();

  for ( i = 1; i < vTimeSeries.size(); i++ )
  {
    RegularTimeSeries * pTimeSeriesLoc = (RegularTimeSeries*)vTimeSeries[i];
    TimePeriod * pTimePeriodLoc = pTimeSeriesLoc->getTemporalExtent();
    if ( ! (*pTimePeriod == *pTimePeriodLoc) )
      return NULL;
  }

  return  pTimePeriod;
}

 ///////////////////////////////////////////////////////////////////////////////
 //  Extrahiert das Zeit-Inkrement (Time Span) einer Menge regulärer          //    
 //  Zeitreihen, die alle das gleiche Zeitz-Inkrement haben                   //
 ///////////////////////////////////////////////////////////////////////////////
private: TimeIntervalLength * getTimeSpan (  vector<AbstractTimeSeries*> vTimeSeries )
{
  unsigned int i;

  if ( vTimeSeries.size() == 0 )
    return NULL;

  for ( i = 0; i < vTimeSeries.size(); i++  )
  {
    AbstractTimeSeries * pTimeSeries = vTimeSeries[i];
    if ( pTimeSeries->getTimeSeriesType() == AbstractTimeSeries::Irregular || 
      pTimeSeries->getTimeSeriesType() == AbstractTimeSeries::IrregularFile )
      return NULL;
  }
  RegularTimeSeries * pTimeSeries = (RegularTimeSeries*)vTimeSeries[0];
  TimeIntervalLength * pTimeSpan = pTimeSeries->getTimeIntervalLength();

  for ( i = 1; i < vTimeSeries.size(); i++ )
  {
    RegularTimeSeries * pTimeSeriesLoc = (RegularTimeSeries*)vTimeSeries[i];
    TimeIntervalLength * pTimeSpanLoc = pTimeSeriesLoc->getTimeIntervalLength();
    if ( ! (*pTimeSpan == *pTimeSpanLoc) )
      return NULL;
  }

  return  pTimeSpan;
}

///////////////////////////////////////////////////////////////////////////////
//  Sortiert eine Menge von Zeitreihen nach den physuikalischen Maßeinheiten,//
//  und baut eine entsprechende map auf                                      //
///////////////////////////////////////////////////////////////////////////////
private: void createChartMap ( vector<AbstractTimeSeries*> vTimeSeries, map<UOM*,int> & mChartMap )
{
  map<UOM*,int>::iterator iter;
  int                     chartNum = 0;

  for ( unsigned int i = 0; i < vTimeSeries.size(); i++ )
  {
    AbstractTimeSeries * pTimeSeries = vTimeSeries[i];
    UOM * pUOM = pTimeSeries->getUOM();

    iter = mChartMap.find( pUOM );
    if ( iter == mChartMap.end() )
    {
      mChartMap[pUOM] = chartNum;
      chartNum++;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Anklicken einer Diagrammzelle. Falls diese Zelle einen   //
//  enthält, wird der Link mit dem HTML-Browser geöffnet                     //
///////////////////////////////////////////////////////////////////////////////
private: System::Void dataGridViewAttribute_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e)
         {
           System::Diagnostics::ProcessStartInfo ^ processStartInfo;
           System::Diagnostics::Process          ^ htmlProcess;

           int row = e->RowIndex;
           int column = e->ColumnIndex;

           DataGridViewCell ^ pCell = dataGridViewAttribute->Rows[row]->Cells[column];
           String ^ link = dynamic_cast<String^> (pCell->Value);

           if ( link->Substring ( 0, 7) != "http://" && 
             ( link->Length < 8 || link->Substring ( 0, 8) != "https://" ) )
             return;
           try
           {
             htmlProcess = gcnew System::Diagnostics::Process();
             processStartInfo = gcnew System::Diagnostics::ProcessStartInfo;

//             processStartInfo->FileName = m_HTMLViewer;
             processStartInfo->FileName = link;
             htmlProcess->StartInfo = processStartInfo;
             htmlProcess->Start();
           }

           catch ( Exception ^)
           {
             System::Windows::Forms::MessageBox::Show ( "HTML-Viewer konnte nicht gestartet werden" );
           }

         }
};
