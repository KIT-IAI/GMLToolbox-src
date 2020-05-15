#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "XPlanGML.h"
#include "GmlSchema.h"
#include "ShpFileObjekte.h"
#include "Convert.h"
#include "ShpFileNamenKonv.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "GenerateKomplexAttribute.h"
#include "includes.h"

using namespace std;

	/// <summary> 
	/// Manuelle Festlegung von Attributwerten eines XPlanGML Plan-Objektes (z.B. BP_Plan, FP_Plan)
	///
	/// </summary>
	public ref class PlanAttributeNeu : public System::Windows::Forms::Form
	{
	public: 
		PlanAttributeNeu( XPlanGML * pXPlanGML,  GmlDictionary * pCodeListsP,  GmlDictionary * pExternalCodeListsP )
		{
			InitializeComponent();

      m_pXPlanGML          = pXPlanGML;
			m_pGmlSchema         = m_pXPlanGML->getFeatures()->getGmlSchema();
			m_pShapeFileObjekte  = m_pXPlanGML->getShapeFileObjekte();
			m_pCodeLists         = pCodeListsP;
			m_pExternalCodeLists = pExternalCodeListsP;
		
			initialize();
		}
        
	protected: 
	private: System::Windows::Forms::ComboBox ^  comboBoxGmlAttribute;
	private: System::Windows::Forms::Label ^  labelGmlAttribute;
	private: System::Windows::Forms::TextBox ^  textBoxAttributWert;
	private: System::Windows::Forms::Label ^  labelAttributWert;
	private: System::Windows::Forms::ComboBox ^  comboBoxAttributWert;
	private: System::Windows::Forms::Button ^  buttonUebernehmen;
	private: System::Windows::Forms::Button ^  buttonLoeschen;
	private: System::Windows::Forms::ListBox ^  listBoxAttribute;
	private: System::Windows::Forms::Button ^  buttonBeenden;
  private: System::Windows::Forms::Label ^  label1;
  private: System::Windows::Forms::TextBox ^  textBoxGmlAttributTyp;

	private:
    XPlanGML         * m_pXPlanGML;						// XPlanung-Modul
    GmlSchema        * m_pGmlSchema;					// XPlanGML-Schema
		ShapeFileObjekte * m_pShapeFileObjekte;   // Repräsentation der Shapefile-Struktur
		GmlDictionary	   * m_pCodeLists;					// GML-Dictionary der XPlanGML-Enumerationen
		GmlDictionary	   * m_pExternalCodeLists;  // GML-Dictionary der externen Codelisten

		GmlKlasse        * pGmlKlasse;						// GML-Struktur des aktuellen Plan-Objektes
		GmlAttribut      * pGmlAttribut;					// Aktuell bearbeitets GML-Attribut
		PlanAttribute    * pPlanAttribute;				// Aggregation der belegten Plan-Attzribute und der zug. Attributwerte
		int				         planTyp;								// Plan-Typ

		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container^ components;

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung. 
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->comboBoxGmlAttribute = (gcnew System::Windows::Forms::ComboBox());
      this->labelGmlAttribute = (gcnew System::Windows::Forms::Label());
      this->textBoxAttributWert = (gcnew System::Windows::Forms::TextBox());
      this->labelAttributWert = (gcnew System::Windows::Forms::Label());
      this->comboBoxAttributWert = (gcnew System::Windows::Forms::ComboBox());
      this->buttonUebernehmen = (gcnew System::Windows::Forms::Button());
      this->buttonLoeschen = (gcnew System::Windows::Forms::Button());
      this->listBoxAttribute = (gcnew System::Windows::Forms::ListBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->textBoxGmlAttributTyp = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->SuspendLayout();
      // 
      // comboBoxGmlAttribute
      // 
      this->comboBoxGmlAttribute->Location = System::Drawing::Point( 8, 24 );
      this->comboBoxGmlAttribute->Name = L"comboBoxGmlAttribute";
      this->comboBoxGmlAttribute->Size = System::Drawing::Size( 344, 21 );
      this->comboBoxGmlAttribute->Sorted = true;
      this->comboBoxGmlAttribute->TabIndex = 0;
      this->comboBoxGmlAttribute->SelectedIndexChanged += gcnew System::EventHandler( this, &PlanAttributeNeu::comboBoxGmlAttribute_SelectedIndexChanged );
      // 
      // labelGmlAttribute
      // 
      this->labelGmlAttribute->Location = System::Drawing::Point( 8, 48 );
      this->labelGmlAttribute->Name = L"labelGmlAttribute";
      this->labelGmlAttribute->Size = System::Drawing::Size( 216, 23 );
      this->labelGmlAttribute->TabIndex = 1;
      this->labelGmlAttribute->Text = L"GML Attribut-Name";
      // 
      // textBoxAttributWert
      // 
      this->textBoxAttributWert->Location = System::Drawing::Point( 8, 112 );
      this->textBoxAttributWert->Name = L"textBoxAttributWert";
      this->textBoxAttributWert->Size = System::Drawing::Size( 312, 20 );
      this->textBoxAttributWert->TabIndex = 2;
      // 
      // labelAttributWert
      // 
      this->labelAttributWert->Location = System::Drawing::Point( 8, 144 );
      this->labelAttributWert->Name = L"labelAttributWert";
      this->labelAttributWert->Size = System::Drawing::Size( 304, 16 );
      this->labelAttributWert->TabIndex = 3;
      this->labelAttributWert->Text = L"Attribut-Wert";
      // 
      // comboBoxAttributWert
      // 
      this->comboBoxAttributWert->Location = System::Drawing::Point( 8, 112 );
      this->comboBoxAttributWert->Name = L"comboBoxAttributWert";
      this->comboBoxAttributWert->Size = System::Drawing::Size( 344, 21 );
      this->comboBoxAttributWert->TabIndex = 4;
      // 
      // buttonUebernehmen
      // 
      this->buttonUebernehmen->Location = System::Drawing::Point( 8, 168 );
      this->buttonUebernehmen->Name = L"buttonUebernehmen";
      this->buttonUebernehmen->Size = System::Drawing::Size( 80, 23 );
      this->buttonUebernehmen->TabIndex = 5;
      this->buttonUebernehmen->Text = L"Übernehmen";
      this->buttonUebernehmen->Click += gcnew System::EventHandler( this, &PlanAttributeNeu::buttonUebernehmen_Click );
      // 
      // buttonLoeschen
      // 
      this->buttonLoeschen->Location = System::Drawing::Point( 96, 168 );
      this->buttonLoeschen->Name = L"buttonLoeschen";
      this->buttonLoeschen->Size = System::Drawing::Size( 75, 23 );
      this->buttonLoeschen->TabIndex = 6;
      this->buttonLoeschen->Text = L"Löschen";
      this->buttonLoeschen->Click += gcnew System::EventHandler( this, &PlanAttributeNeu::buttonLoeschen_Click );
      // 
      // listBoxAttribute
      // 
      this->listBoxAttribute->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
        | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->listBoxAttribute->BackColor = System::Drawing::SystemColors::InactiveBorder;
      this->listBoxAttribute->Location = System::Drawing::Point( 0, 200 );
      this->listBoxAttribute->Name = L"listBoxAttribute";
      this->listBoxAttribute->ScrollAlwaysVisible = true;
      this->listBoxAttribute->Size = System::Drawing::Size( 360, 147 );
      this->listBoxAttribute->TabIndex = 7;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonBeenden->Location = System::Drawing::Point( 272, 168 );
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size( 75, 23 );
      this->buttonBeenden->TabIndex = 8;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->Click += gcnew System::EventHandler( this, &PlanAttributeNeu::buttonBeenden_Click );
      // 
      // textBoxGmlAttributTyp
      // 
      this->textBoxGmlAttributTyp->Location = System::Drawing::Point( 8, 64 );
      this->textBoxGmlAttributTyp->Name = L"textBoxGmlAttributTyp";
      this->textBoxGmlAttributTyp->ReadOnly = true;
      this->textBoxGmlAttributTyp->Size = System::Drawing::Size( 344, 20 );
      this->textBoxGmlAttributTyp->TabIndex = 9;
      // 
      // label1
      // 
      this->label1->Location = System::Drawing::Point( 8, 88 );
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size( 100, 16 );
      this->label1->TabIndex = 10;
      this->label1->Text = L"GML Attribut-Typ";
      // 
      // PlanAttributeNeu
      // 
      this->AcceptButton = this->buttonBeenden;
      this->AutoScaleBaseSize = System::Drawing::Size( 5, 13 );
      this->ClientSize = System::Drawing::Size( 360, 341 );
      this->Controls->Add( this->label1 );
      this->Controls->Add( this->textBoxGmlAttributTyp );
      this->Controls->Add( this->buttonBeenden );
      this->Controls->Add( this->listBoxAttribute );
      this->Controls->Add( this->buttonLoeschen );
      this->Controls->Add( this->buttonUebernehmen );
      this->Controls->Add( this->comboBoxAttributWert );
      this->Controls->Add( this->labelAttributWert );
      this->Controls->Add( this->textBoxAttributWert );
      this->Controls->Add( this->labelGmlAttribute );
      this->Controls->Add( this->comboBoxGmlAttribute );
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"PlanAttributeNeu";
      this->Text = L"Attribute des Gesamtplans";
      this->ResumeLayout( false );
      this->PerformLayout();

    }
	
	private: 

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert den Dialog																							   //
///////////////////////////////////////////////////////////////////////////////
		System::Void initialize()
		 {
			 size_t				  i, anz;
			 string				  gmlAttributName;
			 String				^ STRattributName;
			 vector<GmlAttribut*> vGmlAttribute;

			 pPlanAttribute = m_pXPlanGML->getPlanAttribute();
			 planTyp        = m_pXPlanGML->getPlanTyp();

			 if ( planTyp == BPLAN )
         pGmlKlasse = m_pGmlSchema->getGmlKlasseFromElementName ( "xplan:BP_Plan" );
			 else
			 if ( planTyp == FPLAN )
				pGmlKlasse = m_pGmlSchema->getGmlKlasseFromElementName ( "xplan:FP_Plan" );
			 else
			 if ( planTyp == RPLAN )
				pGmlKlasse = m_pGmlSchema->getGmlKlasseFromElementName ( "xplan:RP_Plan" );
			 else
			 if ( planTyp == LPLAN )
				pGmlKlasse = m_pGmlSchema->getGmlKlasseFromElementName ( "xplan:LP_Plan" );
       else
       if ( planTyp == SOPLAN )
         pGmlKlasse = m_pGmlSchema->getGmlKlasseFromElementName ( "xplan:SO_Plan" );

			 if ( pGmlKlasse == NULL ) return;

			 anz = pGmlKlasse->getGmlAttribute ( vGmlAttribute );
			 for ( i = 0; i < anz; i++ )
			 {
				 GmlAttribut * pGmlAttribut = vGmlAttribute[i];
				 if ( pGmlAttribut->getGmlAttributTyp () != _REFERENZ        &&
					  pGmlAttribut->getGmlAttributTyp () != _GEOMETRIE         &&
            pGmlAttribut->getGmlAttributName() != "xplan:hatGenerAttribut" &&
					  pGmlAttribut->getGmlAttributTyp () != _SONST )
				 {
					gmlAttributName = pGmlAttribut->getGmlAttributName();
					STRattributName = QuConvert::ToString ( gmlAttributName );
					comboBoxGmlAttribute->Items->Add ( STRattributName );			 
				}
			 }

			 showAttributListe();
		 }

///////////////////////////////////////////////////////////////////////////////
//  Anzeige der aktuell festgelegten Attribute / Attribut-Werte							 //
///////////////////////////////////////////////////////////////////////////////
		System::Void showAttributListe()
		 {
			size_t				  i, anz, anzWerte;
			 double				  h;
			 int				    ih;
			 bool				    bh, success;
			 string				  gmlAttributName;
			 string         stdString;
			 string         schluesselText;
			 string				  gmlAttributWert;
			 String				^ STRattributName;
			 String				^ STRattributWert;
			 String       ^ STR;
			 vector<GmlAttribut*> vGmlAttribute;
       vector<Feature*>     vKomplexAttributWerte;
			 ATTRIBUT_TYP		  gmlAttributTyp;
			 GmlDictionary      * pCodeListsActive;
			 GmlDictionaryEntry * pCodeList = NULL;
			 GmlEnumeration     * pEnumeration;

			 if ( pGmlKlasse == NULL ) return;
			
			 listBoxAttribute->Items->Clear();

			 anz = pGmlKlasse->getGmlAttribute ( vGmlAttribute );
			 for ( i = 0; i < anz; i++ )
			 {
				 GmlAttribut * pGmlAttribut = vGmlAttribute[i];
				 gmlAttributName = pGmlAttribut->getGmlAttributName();
				 STRattributName = QuConvert::ToString ( gmlAttributName );

				 gmlAttributTyp = pGmlAttribut->getGmlAttributTyp();
				 switch ( gmlAttributTyp )
				 {
				 case _ANY_URI:
					success = pPlanAttribute->getURLAttributWert ( gmlAttributName, gmlAttributWert );
					if ( success )
						STRattributWert = QuConvert::ToString ( gmlAttributWert );
					break;
				 case _DATE:
					success = pPlanAttribute->getDateAttributWert ( gmlAttributName, gmlAttributWert );
					if ( success )
						STRattributWert = QuConvert::ToString ( gmlAttributWert );
					break;

				 case _STRING:
					success = pPlanAttribute->getStringAttributWert ( gmlAttributName, gmlAttributWert );
					if ( success )
						STRattributWert = QuConvert::ToString ( gmlAttributWert );
					break;

				 case  _ENUMERATION:
				 case _EXTERNAL_CODE_LIST:
					stdString = pGmlAttribut->getGmlEnumeration();
          pCodeListsActive = m_pCodeLists;
          pEnumeration = m_pGmlSchema->getGmlEnumeration ( stdString );

					if ( pCodeListsActive != NULL )
						pCodeList = pCodeListsActive->getDictionaryEntry ( stdString );

					success = pPlanAttribute->getStringAttributWert ( gmlAttributName, gmlAttributWert );
					if ( success )
					{
						success = pCodeList->getSchluesselText ( gmlAttributWert, schluesselText );
						STRattributWert = QuConvert::ToString ( schluesselText );
					}
					break;

				 case _DOUBLE:
				 case _ANGLE:
					success = pPlanAttribute->getDoubleAttributWert ( gmlAttributName, h );
					if ( success )
						STRattributWert = XmlConvert::ToString ( h );
					break;

				 case  _INTEGER:
					success = pPlanAttribute->getIntegerAttributWert ( gmlAttributName, ih );
					if ( success )
						STRattributWert = XmlConvert::ToString ( ih );
					break;

				 case _BOOLEAN:
					success = pPlanAttribute->getBoolAttributWert ( gmlAttributName, bh );
					if ( success )
						STRattributWert = XmlConvert::ToString ( bh );
					break;

         case _REFERENZ:
         case _DATA_TYPE:
           vKomplexAttributWerte.clear();
           anzWerte = pPlanAttribute->getKomplexAttributWerte( gmlAttributName, vKomplexAttributWerte );
           if ( anzWerte > 0 )
           {
             STRattributWert = String::Concat( Convert::ToString ( anzWerte ), "-mal belegt" );
             success = true;
           }
           else
             success = false;
           break;

				 default:
					 success = false;
				 }

				 if ( success )
				 {
					 STR = String::Concat ( STRattributName, " - ", STRattributWert );
					listBoxAttribute->Items->Add ( STR );
				}
					 
			 }
		 }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl eines neuen Attributs über ComboBox						   //
//  GML Attribut-Name																												 //
///////////////////////////////////////////////////////////////////////////////
		private: System::Void comboBoxGmlAttribute_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
		{
			String         ^ planAttributName;
			String         ^ planAttributWert;
			string           planAttributNameStd;
			string           planAttributWertStd;
			string           stdString;
			string           schluesselText;
			String         ^ STR;
			string           enumerationName;
			GmlEnumeration * pEnumeration;
			ATTRIBUT_TYP     attributTyp;
			size_t           i, anz;
      int              iAttributWert;
			double           dAttributWert;
			bool             success, bAttributWert;
      vector<Feature*> vKomplexAttribute;

			GmlDictionary      * pCodeListsActive;
			GmlDictionaryEntry * pCodeList = NULL;

      GenerateKomplexAttribute ^ pDialog;

			vector<string> itemNames;

			planAttributName = dynamic_cast<String^> ( comboBoxGmlAttribute->SelectedItem );
			QuConvert::systemStr2stdStr ( planAttributNameStd, planAttributName );
				
			pGmlAttribut = pGmlKlasse->getGmlAttribut ( planAttributNameStd );
			if ( pGmlAttribut == NULL ) return;

			textBoxAttributWert->Visible = true;
			comboBoxAttributWert->Visible = false;

			attributTyp = pGmlAttribut->getGmlAttributTyp();
			switch ( attributTyp )
			{
			case _ENUMERATION:
			case _EXTERNAL_CODE_LIST:
				comboBoxAttributWert->Visible = true;
				textBoxAttributWert->Visible = false;
				comboBoxAttributWert->Items->Clear();

				if ( attributTyp == _ENUMERATION ) pCodeListsActive = m_pCodeLists;
				else
				if ( attributTyp == _EXTERNAL_CODE_LIST ) pCodeListsActive = m_pExternalCodeLists;

				enumerationName = pGmlAttribut->getGmlEnumeration();
				STR = QuConvert::ToString ( enumerationName );
				textBoxGmlAttributTyp->Text = STR;
        pEnumeration = m_pGmlSchema->getGmlEnumeration ( enumerationName );

				if ( pCodeListsActive != NULL )
					pCodeList = pCodeListsActive->getDictionaryEntry ( enumerationName );

				if ( pEnumeration != NULL )
				{
					pEnumeration->getEnumerationItemNames ( itemNames );
					for ( i = 0; i < itemNames.size(); i++ )
					{
						stdString = itemNames[i];
						if ( pCodeList != NULL )
						{
							success = pCodeList->getSchluesselText ( stdString, schluesselText );
							if ( success && schluesselText != "" ) stdString = schluesselText;
						}

						STR = QuConvert::ToString ( stdString );
						comboBoxAttributWert->Items->Add ( STR );
					}
				}

				textBoxAttributWert->Text = nullptr;

				success = pPlanAttribute->getStringAttributWert ( planAttributNameStd, planAttributWertStd );
				if ( success )
				{
				  success = pCodeList->getSchluesselText ( planAttributWertStd, schluesselText );
					if ( success && schluesselText != "" ) planAttributWertStd = schluesselText;

					 planAttributWert = QuConvert::ToString ( planAttributWertStd );
					 comboBoxAttributWert->SelectedItem = planAttributWert;
				}
				break;

			 case _STRING:
				 textBoxAttributWert->Text = nullptr;
				 textBoxGmlAttributTyp->Text = "String";

				 success = pPlanAttribute->getStringAttributWert ( planAttributNameStd, planAttributWertStd );
				 if ( success )
				 {
					 planAttributWert = QuConvert::ToString ( planAttributWertStd );
					 if ( attributTyp == _ENUMERATION )
						 comboBoxAttributWert->SelectedItem = planAttributWert;
					 else
						 textBoxAttributWert->Text = planAttributWert;
				 }
				 break;

			 case _DATE:
				 textBoxAttributWert->Text = nullptr;
				 textBoxGmlAttributTyp->Text = "Datum (JJJJ-MM-TT)";

				 success = pPlanAttribute->getDateAttributWert ( planAttributNameStd, planAttributWertStd );
				 if ( success )
				 {
					 planAttributWert = QuConvert::ToString ( planAttributWertStd );
					 textBoxAttributWert->Text = planAttributWert;
				 }
				 break;


			 case _ANY_URI:
				 textBoxAttributWert->Text = nullptr;
				 textBoxGmlAttributTyp->Text = "URL";

				 success = pPlanAttribute->getURLAttributWert ( planAttributNameStd, planAttributWertStd );
				 if ( success )
				 {
					 planAttributWert = QuConvert::ToString ( planAttributWertStd );
					 textBoxAttributWert->Text = planAttributWert;
				 }
				 break;

			 case _DOUBLE:
			 case _ANGLE:
				 textBoxAttributWert->Text = nullptr;
				 textBoxGmlAttributTyp->Text = "Double";

				 success = pPlanAttribute->getDoubleAttributWert ( planAttributNameStd, dAttributWert );
				 if ( success )
				 {
					 planAttributWert = QuConvert::ToString ( dAttributWert );
					 textBoxAttributWert->Text = planAttributWert;
				 }
				 break;

			 case _INTEGER:
				 textBoxAttributWert->Text = nullptr;
				 textBoxGmlAttributTyp->Text = "Integer";

				 success = pPlanAttribute->getIntegerAttributWert ( planAttributNameStd, iAttributWert );
				 if ( success )
				 {
					 planAttributWert = QuConvert::ToString ( iAttributWert );
					 textBoxAttributWert->Text = planAttributWert;
				 }
				 break;

			 case _BOOLEAN:
				 comboBoxAttributWert->Visible = true;
				 textBoxAttributWert->Visible = false;
 				 comboBoxAttributWert->Items->Clear();
				 comboBoxAttributWert->Items->Add ( "true" );
				 comboBoxAttributWert->Items->Add ( "false" );
				 textBoxGmlAttributTyp->Text = "Boolean";

				 success = pPlanAttribute->getBoolAttributWert ( planAttributNameStd, bAttributWert );
				 if ( success )
				 {
					 if ( bAttributWert == true ) 
						comboBoxAttributWert->SelectedItem = "true";
					 else
						comboBoxAttributWert->SelectedItem = "false";
				 }
				 break;

       case _DATA_TYPE:
       case _REFERENZ:
         {
            anz = pPlanAttribute->getKomplexAttributWerte ( planAttributNameStd, vKomplexAttribute );

            FeatureList * pPlanObjektList = new FeatureList;
            for ( i = 0; i < vKomplexAttribute.size(); i++ )
              pPlanObjektList->addCopy( vKomplexAttribute[i] );
            pPlanObjektList->setKlassenName ( pGmlKlasse->getGmlKlassenName() );
            pPlanObjektList->setRelationName( planAttributNameStd );

            string refClassName = pGmlAttribut->getReferenziertesElement();
            GmlKlasse * pRefClassGmlTyp = m_pGmlSchema->getGmlKlasseFromElementName( refClassName );
            pPlanObjektList->setGmlTyp( pRefClassGmlTyp );

            pDialog = gcnew GenerateKomplexAttribute 
                    ( m_pXPlanGML->getFeatures(), m_pCodeLists, m_pExternalCodeLists, pPlanObjektList );
            System::Windows::Forms::DialogResult ok = pDialog->ShowDialog();
            if ( ok == System::Windows::Forms::DialogResult::OK )
            {
              pPlanAttribute->removeKomplexAttribut( planAttributNameStd );
              for ( i = 0; i < pPlanObjektList->count(); i++ )
                pPlanAttribute->addKomplexAttribut( planAttributNameStd, pPlanObjektList->get(i) );
              showAttributListe();
            }
            delete pPlanObjektList;
         }
			}	
		}
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Übernehmen"																			 //
//  Übernimmt einen festgelegten Attributwert															   //
///////////////////////////////////////////////////////////////////////////////
		private: System::Void buttonUebernehmen_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			String     ^ gmlAttributWert;
			string       gmlAttributNameStd;
			string       gmlAttributWertStd;
			string       schluesselNummer;
			string       attributWert;
			string       stdString;
			double       dAttributWert;
			int          iAttributWert;
			bool	     success;
			ATTRIBUT_TYP attributTyp;
			GmlDictionary      * pCodeListsActive;
			GmlDictionaryEntry * pCodeList = NULL;

			if ( pGmlAttribut == NULL ) return;
			gmlAttributNameStd = pGmlAttribut->getGmlAttributName();

			attributTyp = pGmlAttribut->getGmlAttributTyp();
			switch ( attributTyp )
			{
			 case  _ENUMERATION:
			 case _EXTERNAL_CODE_LIST:
				 gmlAttributWert = dynamic_cast<String^> ( comboBoxAttributWert->SelectedItem );

				if ( attributTyp == _ENUMERATION ) pCodeListsActive = m_pCodeLists;
				else
				if ( attributTyp == _EXTERNAL_CODE_LIST ) pCodeListsActive = m_pExternalCodeLists;

				QuConvert::systemStr2stdStr ( attributWert, gmlAttributWert );

				stdString = pGmlAttribut->getGmlEnumeration();
				if ( pCodeListsActive != NULL )
					pCodeList = pCodeListsActive->getDictionaryEntry ( stdString );

				if ( pCodeList != NULL )
				{
					success = pCodeList->getSchluesselNummer ( attributWert, schluesselNummer );
					if ( success )
          {
            if ( pGmlAttribut->getMultiple() == false )
              pPlanAttribute->removeStringAttribut( gmlAttributNameStd );
  				  pPlanAttribute->addStringAttribut ( gmlAttributNameStd, schluesselNummer );
          }
				}

				 break;

			 case _ANY_URI:
				 gmlAttributWert = dynamic_cast<String^> ( textBoxAttributWert->Text );
				 if ( gmlAttributWert == nullptr ) return;

				 QuConvert::systemStr2stdStr ( gmlAttributWertStd, gmlAttributWert );

         if ( pGmlAttribut->getMultiple() == false )
           pPlanAttribute->removeURLAttribut( gmlAttributNameStd );
				 pPlanAttribute->addURLAttribut ( gmlAttributNameStd, gmlAttributWertStd );
				 break;


			 case _STRING:
				 gmlAttributWert = dynamic_cast<String^> ( textBoxAttributWert->Text );
				 if ( gmlAttributWert == nullptr ) return;

				 QuConvert::systemStr2stdStr ( gmlAttributWertStd, gmlAttributWert );

         if ( pGmlAttribut->getMultiple() == false )
           pPlanAttribute->removeStringAttribut( gmlAttributNameStd );
				 pPlanAttribute->addStringAttribut ( gmlAttributNameStd, gmlAttributWertStd );
				 break;


			 case _DATE:
				 gmlAttributWert = dynamic_cast<String^> ( textBoxAttributWert->Text );
				 if ( gmlAttributWert == nullptr ) return;

				 QuConvert::systemStr2stdStr ( gmlAttributWertStd, gmlAttributWert );

         if ( pGmlAttribut->getMultiple() == false )
           pPlanAttribute->removeDateAttribut( gmlAttributNameStd );
				 pPlanAttribute->addDateAttribut ( gmlAttributNameStd, gmlAttributWertStd );
				 break;

			 case _DOUBLE:
			 case _ANGLE:
				gmlAttributWert = textBoxAttributWert->Text;
				if ( gmlAttributWert == nullptr ) return;

				try	{ dAttributWert = Convert::ToDouble ( gmlAttributWert ); }
				catch ( System::FormatException^ )
				{
					System::Windows::Forms::MessageBox::Show ( "Ungültiger Double-Wert" );
					textBoxAttributWert->Text = nullptr;
					return;
				}
				
        if ( pGmlAttribut->getMultiple() == false )
          pPlanAttribute->removeDoubleAttribut( gmlAttributNameStd );
				pPlanAttribute->addDoubleAttribut ( gmlAttributNameStd, dAttributWert );
				break;

			 case _INTEGER:
				gmlAttributWert = textBoxAttributWert->Text;
				if ( gmlAttributWert == nullptr ) return;

				try	{ iAttributWert = Convert::ToInt32 ( gmlAttributWert ); }
				catch ( System::FormatException^ )
				{
					System::Windows::Forms::MessageBox::Show ( "Ungültiger Integer-Wert" );
					textBoxAttributWert->Text = nullptr;
					return;
				}
				
        if ( pGmlAttribut->getMultiple() == false )
          pPlanAttribute->removeIntegerAttribut( gmlAttributNameStd );
				pPlanAttribute->addIntegerAttribut ( gmlAttributNameStd, iAttributWert );
				break;

			 case _BOOLEAN:
				 gmlAttributWert = dynamic_cast<String^> ( comboBoxAttributWert->SelectedItem );
				 if ( gmlAttributWert == nullptr ) return;

				 if ( String::Compare ( gmlAttributWert, "true" ) == 0 )
					pPlanAttribute->setBoolAttribut ( gmlAttributNameStd, true );
				 else
					pPlanAttribute->setBoolAttribut ( gmlAttributNameStd, false );
				 break;

			}
			showAttributListe();
		}

///////////////////////////////////////////////////////////////////////////////
//  Löscht eine Zuordnung Attribut --> Attribut-Wert											   //
///////////////////////////////////////////////////////////////////////////////
		private: System::Void buttonLoeschen_Click(System::Object ^  sender, System::EventArgs ^  e)
		{
			string       gmlAttributNameStd;
			ATTRIBUT_TYP attributTyp;

			if ( pGmlAttribut == NULL ) return;
			gmlAttributNameStd = pGmlAttribut->getGmlAttributName();

			attributTyp = pGmlAttribut->getGmlAttributTyp();
			switch ( attributTyp )
			{
			 case _ANY_URI:
				 pPlanAttribute->removeURLAttribut ( gmlAttributNameStd );
				 break;

			 case _DATE:
				 pPlanAttribute->removeDateAttribut ( gmlAttributNameStd );
				 break;

			 case _STRING:
				 pPlanAttribute->removeStringAttribut ( gmlAttributNameStd );
				 break;

			 case  _ENUMERATION:
			 case _EXTERNAL_CODE_LIST:
				 pPlanAttribute->removeStringAttribut ( gmlAttributNameStd );
				 break;

			 case _DOUBLE:
			 case _ANGLE:
				 pPlanAttribute->removeDoubleAttribut ( gmlAttributNameStd );
				 break;

			 case _INTEGER:
				 pPlanAttribute->removeIntegerAttribut ( gmlAttributNameStd );
				 break;

			 case _BOOLEAN:
				 pPlanAttribute->removeBoolAttribut ( gmlAttributNameStd );
				 break;
			}

			if ( attributTyp == _ENUMERATION || attributTyp == _ENUMERATION )
				comboBoxAttributWert->SelectedItem = nullptr;
			else
				textBoxAttributWert->Text = nullptr;		
		
			showAttributListe();
		}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"																					 //
///////////////////////////////////////////////////////////////////////////////
		private: System::Void buttonBeenden_Click(System::Object ^  sender, System::EventArgs ^  e)
		{

		}

};
