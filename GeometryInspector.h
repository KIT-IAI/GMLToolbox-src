#pragma once


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "SrsManagerRef.h"
#include "Feature.h"
#include "Checking.h"
#include "convert.h"
#include "Geometrie.h"

	/// <summary>
	/// Anzeige detaillierter Informationen über ein Geometrieobjekt
	///
	/// </summary>
	public ref class GeometryInspector : public System::Windows::Forms::Form
	{
	public:
		GeometryInspector( Checking * pChecking, Feature * pFeature, _Geometrie * pGeometryActualP )
		{
      vector<string>      vProperties;
      vector<_Geometrie*> vGeometrien;
			size_t              i, anz;
      String           ^  STR;

			InitializeComponent();

      bbSw = gcnew cli::array<double>(3);
      bbNo = gcnew cli::array<double>(3);

      m_pFeature   = pFeature;
      m_pFeatures  = m_pFeature->getFeatures();
      m_pCheckParams = pChecking->getCheckingParams();
      checkReferencedGeometryOrig = m_pCheckParams->checkReferencedGeometry;
      m_pCheckParams->checkReferencedGeometry = true;
      
      pGeometryReferenced = NULL;
 
      koordNormieren     = false;

      buttonReverseContour->Visible = false;
      buttonReverseHoles->Visible   = false;
      buttonReversePolygon->Visible = false;

      pGeometryActual = pGeometryActualP;

      if ( pGeometryActual == NULL )
      {
        anz = pFeature->getGeometryProperties( vProperties, vGeometrien );
        if ( anz == 0 )
        {
          textBoxGeometrietyp->Text = "Keine Geometrie vorhanden";
        }
        else
        {
          for ( i = 0; i < anz; i++ )
          {
            STR = QuConvert::ToString( vProperties[i] );
            comboBoxGeometryProperty->Items->Add( STR );
          }

          comboBoxGeometryProperty->SelectedIndex = 0;

        }
      }
      else
      {
        comboBoxGeometryProperty->Enabled = false; 
        initGeometry();
      }
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~GeometryInspector()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  textBoxGeometrietyp;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::TextBox^  textBoxGMLId;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::TextBox^  textBoxSrsName;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::TextBox^  textBoxSrsDimension;
  private: System::Windows::Forms::Label^  labelRefGeometrie;
  private: System::Windows::Forms::TreeView^  treeView1;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::ComboBox^  comboBoxGeometryProperty;
  private: System::Windows::Forms::Label^  label5;
  private: System::Windows::Forms::Label^  label6;
  private: System::Windows::Forms::TextBox^  textBoxBBsw;
  private: System::Windows::Forms::TextBox^  textBoxBBno;
  private: System::Windows::Forms::Label^  label7;
  private: System::Windows::Forms::TextBox^  textBoxAusdehnung;
  private: System::Windows::Forms::Button^  buttonReverseContour;
  private: System::Windows::Forms::Button^  buttonReverseHoles;
  private: System::Windows::Forms::Button^  buttonReversePolygon;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>

    Features       * m_pFeatures;           // Aggregation aller Features
    Feature        * m_pFeature;            // Aktuell untersuchtes Feature
    CheckingParams * m_pCheckParams;        // Prüfparameter

    _Geometrie          * pGeometryActual;      // Aktuell untersuchtes Geometrieobjekt
    _Geometrie          * pGeometryReferenced;  // Referenzierte Geometrie, falls die aktuelle Geometrie ein Referenzobjekt ist
    bool                  koordNormieren;       // Bestimmt, ob Geometriekoordinaten bei der Anzeige normiert werden sollen
    bool                  checkReferencedGeometryOrig;// Wert des Attributs checkReferencedGeometry vor Aufruf des Dialogs
    cli::array<double>  ^ bbSw;                 // Koordinaten der unteren, linken Ecke der Bounding Box
    cli::array<double>  ^ bbNo;                 // Koordinaten der oberen, rechten Ecke der Bounding Box


	System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBoxGeometrietyp = (gcnew System::Windows::Forms::TextBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->textBoxGMLId = (gcnew System::Windows::Forms::TextBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->textBoxSrsName = (gcnew System::Windows::Forms::TextBox());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->textBoxSrsDimension = (gcnew System::Windows::Forms::TextBox());
      this->labelRefGeometrie = (gcnew System::Windows::Forms::Label());
      this->treeView1 = (gcnew System::Windows::Forms::TreeView());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->comboBoxGeometryProperty = (gcnew System::Windows::Forms::ComboBox());
      this->label5 = (gcnew System::Windows::Forms::Label());
      this->label6 = (gcnew System::Windows::Forms::Label());
      this->textBoxBBsw = (gcnew System::Windows::Forms::TextBox());
      this->textBoxBBno = (gcnew System::Windows::Forms::TextBox());
      this->label7 = (gcnew System::Windows::Forms::Label());
      this->textBoxAusdehnung = (gcnew System::Windows::Forms::TextBox());
      this->buttonReverseContour = (gcnew System::Windows::Forms::Button());
      this->buttonReverseHoles = (gcnew System::Windows::Forms::Button());
      this->buttonReversePolygon = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(6, 76);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(69, 13);
      this->label1->TabIndex = 0;
      this->label1->Text = L"Geometrietyp";
      // 
      // textBoxGeometrietyp
      // 
      this->textBoxGeometrietyp->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxGeometrietyp->Location = System::Drawing::Point(101, 73);
      this->textBoxGeometrietyp->Name = L"textBoxGeometrietyp";
      this->textBoxGeometrietyp->Size = System::Drawing::Size(323, 20);
      this->textBoxGeometrietyp->TabIndex = 1;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(6, 106);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(42, 13);
      this->label2->TabIndex = 2;
      this->label2->Text = L"GML-Id";
      // 
      // textBoxGMLId
      // 
      this->textBoxGMLId->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxGMLId->Location = System::Drawing::Point(101, 99);
      this->textBoxGMLId->Name = L"textBoxGMLId";
      this->textBoxGMLId->Size = System::Drawing::Size(323, 20);
      this->textBoxGMLId->TabIndex = 3;
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(6, 132);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(50, 13);
      this->label3->TabIndex = 4;
      this->label3->Text = L"SrsName";
      // 
      // textBoxSrsName
      // 
      this->textBoxSrsName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxSrsName->Location = System::Drawing::Point(101, 125);
      this->textBoxSrsName->Name = L"textBoxSrsName";
      this->textBoxSrsName->Size = System::Drawing::Size(323, 20);
      this->textBoxSrsName->TabIndex = 5;
      // 
      // label4
      // 
      this->label4->AutoSize = true;
      this->label4->Location = System::Drawing::Point(6, 159);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(71, 13);
      this->label4->TabIndex = 6;
      this->label4->Text = L"SrsDimension";
      // 
      // textBoxSrsDimension
      // 
      this->textBoxSrsDimension->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxSrsDimension->Location = System::Drawing::Point(101, 153);
      this->textBoxSrsDimension->Name = L"textBoxSrsDimension";
      this->textBoxSrsDimension->Size = System::Drawing::Size(323, 20);
      this->textBoxSrsDimension->TabIndex = 7;
      // 
      // labelRefGeometrie
      // 
      this->labelRefGeometrie->AutoSize = true;
      this->labelRefGeometrie->Location = System::Drawing::Point(6, 274);
      this->labelRefGeometrie->Name = L"labelRefGeometrie";
      this->labelRefGeometrie->Size = System::Drawing::Size(104, 13);
      this->labelRefGeometrie->TabIndex = 8;
      this->labelRefGeometrie->Text = L"Referierte Geometrie";
      // 
      // treeView1
      // 
      this->treeView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->treeView1->Location = System::Drawing::Point(9, 302);
      this->treeView1->Name = L"treeView1";
      this->treeView1->Size = System::Drawing::Size(422, 286);
      this->treeView1->TabIndex = 9;
      this->treeView1->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &GeometryInspector::treeView1_AfterSelect);
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->buttonBeenden->Location = System::Drawing::Point(9, 683);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(422, 23);
      this->buttonBeenden->TabIndex = 10;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &GeometryInspector::buttonBeenden_Click);
      // 
      // comboBoxGeometryProperty
      // 
      this->comboBoxGeometryProperty->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->comboBoxGeometryProperty->FormattingEnabled = true;
      this->comboBoxGeometryProperty->Location = System::Drawing::Point(9, 12);
      this->comboBoxGeometryProperty->Name = L"comboBoxGeometryProperty";
      this->comboBoxGeometryProperty->Size = System::Drawing::Size(415, 21);
      this->comboBoxGeometryProperty->TabIndex = 11;
      this->comboBoxGeometryProperty->SelectedIndexChanged += gcnew System::EventHandler(this, &GeometryInspector::comboBoxGeometryProperty_SelectedIndexChanged);
      // 
      // label5
      // 
      this->label5->AutoSize = true;
      this->label5->Location = System::Drawing::Point(13, 41);
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size(114, 13);
      this->label5->TabIndex = 12;
      this->label5->Text = L"Geometrie-Eigenschaft";
      // 
      // label6
      // 
      this->label6->AutoSize = true;
      this->label6->Location = System::Drawing::Point(6, 198);
      this->label6->Name = L"label6";
      this->label6->Size = System::Drawing::Size(73, 13);
      this->label6->TabIndex = 13;
      this->label6->Text = L"Bounding Box";
      // 
      // textBoxBBsw
      // 
      this->textBoxBBsw->Location = System::Drawing::Point(101, 180);
      this->textBoxBBsw->Name = L"textBoxBBsw";
      this->textBoxBBsw->Size = System::Drawing::Size(323, 20);
      this->textBoxBBsw->TabIndex = 14;
      // 
      // textBoxBBno
      // 
      this->textBoxBBno->Location = System::Drawing::Point(101, 207);
      this->textBoxBBno->Name = L"textBoxBBno";
      this->textBoxBBno->Size = System::Drawing::Size(323, 20);
      this->textBoxBBno->TabIndex = 15;
      // 
      // label7
      // 
      this->label7->AutoSize = true;
      this->label7->Location = System::Drawing::Point(9, 239);
      this->label7->Name = L"label7";
      this->label7->Size = System::Drawing::Size(67, 13);
      this->label7->TabIndex = 16;
      this->label7->Text = L"Ausdehnung";
      // 
      // textBoxAusdehnung
      // 
      this->textBoxAusdehnung->Location = System::Drawing::Point(101, 231);
      this->textBoxAusdehnung->Name = L"textBoxAusdehnung";
      this->textBoxAusdehnung->Size = System::Drawing::Size(323, 20);
      this->textBoxAusdehnung->TabIndex = 17;
      // 
      // buttonReverseContour
      // 
      this->buttonReverseContour->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonReverseContour->Location = System::Drawing::Point(9, 604);
      this->buttonReverseContour->Name = L"buttonReverseContour";
      this->buttonReverseContour->Size = System::Drawing::Size(80, 46);
      this->buttonReverseContour->TabIndex = 18;
      this->buttonReverseContour->Text = L"Außenkontur umdrehen";
      this->buttonReverseContour->UseVisualStyleBackColor = true;
      this->buttonReverseContour->Click += gcnew System::EventHandler(this, &GeometryInspector::buttonReverseContour_Click);
      // 
      // buttonReverseHoles
      // 
      this->buttonReverseHoles->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
      this->buttonReverseHoles->Location = System::Drawing::Point(176, 604);
      this->buttonReverseHoles->Name = L"buttonReverseHoles";
      this->buttonReverseHoles->Size = System::Drawing::Size(86, 46);
      this->buttonReverseHoles->TabIndex = 19;
      this->buttonReverseHoles->Text = L"Innenkonturen umdrehen";
      this->buttonReverseHoles->UseVisualStyleBackColor = true;
      this->buttonReverseHoles->Click += gcnew System::EventHandler(this, &GeometryInspector::buttonReverseHoles_Click);
      // 
      // buttonReversePolygon
      // 
      this->buttonReversePolygon->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonReversePolygon->Location = System::Drawing::Point(349, 604);
      this->buttonReversePolygon->Name = L"buttonReversePolygon";
      this->buttonReversePolygon->Size = System::Drawing::Size(75, 46);
      this->buttonReversePolygon->TabIndex = 20;
      this->buttonReversePolygon->Text = L"Polygon umdrehen";
      this->buttonReversePolygon->UseVisualStyleBackColor = true;
      this->buttonReversePolygon->Click += gcnew System::EventHandler(this, &GeometryInspector::buttonReversePolygon_Click);
      // 
      // GeometryInspector
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(443, 718);
      this->Controls->Add(this->buttonReversePolygon);
      this->Controls->Add(this->buttonReverseHoles);
      this->Controls->Add(this->buttonReverseContour);
      this->Controls->Add(this->textBoxAusdehnung);
      this->Controls->Add(this->label7);
      this->Controls->Add(this->textBoxBBno);
      this->Controls->Add(this->textBoxBBsw);
      this->Controls->Add(this->label6);
      this->Controls->Add(this->label5);
      this->Controls->Add(this->comboBoxGeometryProperty);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->treeView1);
      this->Controls->Add(this->labelRefGeometrie);
      this->Controls->Add(this->textBoxSrsDimension);
      this->Controls->Add(this->label4);
      this->Controls->Add(this->textBoxSrsName);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->textBoxGMLId);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->textBoxGeometrietyp);
      this->Controls->Add(this->label1);
      this->Name = L"GeometryInspector";
      this->Text = L"GeometryInspector";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl einer Geometrie-Eigenschaft über die ComboBox    //
//  "Geometrie-Eigenschaft"                                                  //
///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxGeometryProperty_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
           {
             string        propertyName;

             String ^ STR = dynamic_cast<String^>(comboBoxGeometryProperty->SelectedItem);
             QuConvert::systemStr2stdStr( propertyName, STR );
             pGeometryActual = m_pFeature->getGeometry( propertyName, 0 );
             if ( pGeometryActual != NULL )
             {
               initGeometry();
             }

           }

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Dialogbox für ein bestimmtes Geometrieobjekt           //
///////////////////////////////////////////////////////////////////////////////
private:System::Void initGeometry()
        {
          string        geometryTypeString;
          string        srsName;
          int           i, srsDimension;
          GEOMETRY_TYPE geometryType;
          double        sw[3], no[3];
          bool          success;

          textBoxGMLId->Text = QuConvert::ToString( pGeometryActual->getGmlId() );

          geometryType = pGeometryActual->getGeometryType();
          textBoxGeometrietyp ->Text = QuConvert::ToString( _Geometrie::convertTypeToString( geometryType ) );
          if ( geometryType ==  GEOMETRY_REFERENCE ||
            geometryType ==  SURFACE_REFERENCE  ||
            geometryType ==  CURVE_REFERENCE    ||
            geometryType ==  POINT_REFERENCE    ||
            geometryType ==  SOLID_REFERENCE )
          {
            labelRefGeometrie->Visible = true;
            pGeometryReferenced = ((_GeometryReference*)pGeometryActual)->getRefGeoObjekt();
            if ( pGeometryReferenced->getGeometryType() == POLYGON )
            {
              buttonReverseContour->Visible = true;
              buttonReverseHoles->Visible   = true;
              buttonReversePolygon->Visible = true;
            }
            else
            {
              buttonReverseContour->Visible = false;
              buttonReverseHoles->Visible   = false;
              buttonReversePolygon->Visible = false;
            }
          }
          else
          {
            labelRefGeometrie->Visible = false;
            pGeometryReferenced = NULL;
            if ( pGeometryActual->getGeometryType() == POLYGON )
            {
              buttonReverseContour->Visible = true;
              buttonReverseHoles->Visible   = true;
              buttonReversePolygon->Visible = true;
            }
            else
            {
              buttonReverseContour->Visible = false;
              buttonReverseHoles->Visible   = false;
              buttonReversePolygon->Visible = false;
            }
          }

          success = pGeometryActual->getBoundingBox( sw, no );
          if ( success )
          {
            for ( i = 0; i < 3; i++ )
            {
              bbSw[i] = sw[i];
              bbNo[i] = no[i];
            }

            textBoxBBsw->Text = String::Concat( QuConvert::ToString( sw[0], 2 ), ", ", QuConvert::ToString( sw[1], 2 ), ", ", QuConvert::ToString( sw[2], 2 ), ", " );
            textBoxBBno->Text = String::Concat( QuConvert::ToString( no[0], 2 ), ", ", QuConvert::ToString( no[1], 2 ), ", ", QuConvert::ToString( no[2], 2 ), ", " );
            textBoxAusdehnung->Text = String::Concat( QuConvert::ToString( no[0] - sw[0], 2 ), ", ", QuConvert::ToString( no[1] - sw[1], 2 ), 
              ", ", QuConvert::ToString( no[2] - sw[2], 2 ), ", " );
          }
          else
            textBoxAusdehnung->Text = "Keine Geometrie";

          srsName = pGeometryActual->getSrsName();
          if ( srsName == "" )
            srsName = m_pFeature->getSrsName();
          if ( srsName != "" )
            textBoxSrsName->Text  = QuConvert::ToString( srsName );
          else
            textBoxSrsName->Text = "SrsName ist unbekannt";

          srsDimension = pGeometryActual->getDim();
          if ( srsDimension == -1 )
            srsDimension = m_pFeature->getSrsDimension();
          if ( srsDimension > 0 )
            textBoxSrsDimension->Text = Convert::ToString ( srsDimension );
          else
            textBoxSrsDimension->Text = "SrsDimension ist unbekannt";

          initializeTree();
        }

///////////////////////////////////////////////////////////////////////////////
//  Schließt den Dialog                                                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           m_pCheckParams->checkReferencedGeometry = checkReferencedGeometryOrig;
           Close();
         }

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert die Darstellung der Geometrie in Form eines Baumes         //
///////////////////////////////////////////////////////////////////////////////
private: System::Void initializeTree()
         {
           vector<PolygonGeo*> vPolygone;
           vector<_Curve*>  vCurves;
           vector<bool>     vOrientations;
           TreeNode       ^ pTreeNode;
           TreeNode       ^ pChildNode;
           TreeNode       ^ pSubChildNode;
           String         ^ treeNodename;
           String         ^ childNodeName;
           String         ^ geometryType;
           vector<double>   vX, vY, vZ;
           double           volume;
           double           flaeche;
           double           laenge;
           size_t           i, j, anz;
           _Ring          * pRing;
           vector<Error*>   vErrors;

           treeView1->Nodes->Clear();         

           geometryType = QuConvert::ToString( _Geometrie::convertTypeToString( pGeometryActual->getGeometryType() ) );

           switch ( pGeometryActual->getGeometryType() )
           {
           case MULTI_SOLID:
             {
               vector<_Solid*> vSolids;

               MultiSolid * pMultiSolid = dynamic_cast<MultiSolid*>(pGeometryActual);
               anz =  pMultiSolid->getSolids( vSolids );
               if ( anz > 0 )
               {
                 treeNodename = geometryType;
                 pTreeNode = gcnew TreeNode ( treeNodename );
                 treeView1->Nodes->Add( pTreeNode );
                 
                 volume = pMultiSolid->getVolume();
                 childNodeName = String::Concat ( "Volumen: ", QuConvert::ToString( volume, 2 ) );
                 pChildNode = gcnew TreeNode ( childNodeName );
                 pTreeNode->Nodes->Add( pChildNode );
               }
               for ( i = 0; i < anz; i++ )
               {
                 treeNodename = "Solid";
                 pChildNode = gcnew TreeNode ( treeNodename );
                 pChildNode->Tag = QuConvert::ToString( vSolids[i]->getGmlId() );
                 pTreeNode->Nodes->Add( pChildNode );
               }
               break;
             }

           case SOLID:
             {
               Solid * pSolid = dynamic_cast<Solid*>(pGeometryActual);
               anz =  pSolid->getAussenkonturPolygone( vPolygone, vOrientations );
               if ( anz > 0 )
               {
                 treeNodename = geometryType;
                 pTreeNode = gcnew TreeNode ( treeNodename );
                 treeView1->Nodes->Add( pTreeNode );
                 
                   volume = pSolid->getVolume();
                   childNodeName = String::Concat ( "Volumen: ", QuConvert::ToString( volume, 2 ) );
                   pChildNode = gcnew TreeNode ( childNodeName );
                   pTreeNode->Nodes->Add( pChildNode );

                   flaeche = pSolid->getOberflaeche();
                   childNodeName = String::Concat ( "Oberfläche: ", QuConvert::ToString( flaeche, 2 ) );
                   pChildNode = gcnew TreeNode ( childNodeName );
                   pTreeNode->Nodes->Add( pChildNode );
               }
               for ( i = 0; i < anz; i++ )
               {
                 if ( vOrientations[i] == true )
                   treeNodename = "Polygon (normal)";
                 else
                   treeNodename = "Polygon (invertiert)";
                 pChildNode = gcnew TreeNode ( treeNodename );
                 pChildNode->Tag = QuConvert::ToString( vPolygone[i]->getGmlId() );
                 pTreeNode->Nodes->Add( pChildNode );
               }

               break;
             }

           case MULTI_SURFACE:
           case COMPOSITE_SURFACE:
           case SHELL:
           case TRIANGULATED_SURFACE:
           case SURFACE:
           case ORIENTABLE_SURFACE:
             {
               _Surface * pSurface = dynamic_cast<_Surface*>(pGeometryActual);
               anz =  pSurface->getPolygone( vPolygone, vOrientations );
               if ( anz > 0 )
               {
                 treeNodename = geometryType;
                 pTreeNode = gcnew TreeNode ( treeNodename );
                 treeView1->Nodes->Add( pTreeNode );

                 flaeche = pSurface->getFlaeche();
                 childNodeName = String::Concat ( "Oberfläche: ", QuConvert::ToString( flaeche, 2 ) );
                 pChildNode = gcnew TreeNode ( childNodeName );
                 pTreeNode->Nodes->Add( pChildNode );
               }
               for ( i = 0; i < anz; i++ )
               {
                 if ( vOrientations[i] == true )
                    treeNodename = "Polygon (normal)";
                 else
                   treeNodename = "Polygon (invertiert)";
                  pChildNode = gcnew TreeNode ( treeNodename );
                  pChildNode->Tag = QuConvert::ToString( vPolygone[i]->getGmlId() );
                  pTreeNode->Nodes->Add( pChildNode );
               }
               break;
             } 

           case POLYGON:
           case TRIANGLE:
             {
               PolygonGeo * pPolygon =  dynamic_cast<PolygonGeo*>(pGeometryActual);
               double       x, y, z;
							 size_t       lochAnz;
               Direction *  normal;
               GeoPoint     center;
               TreeNode   ^ pNodeLoch;
               TreeNode   ^ pNodeStuetzpunkt;

               treeNodename = geometryType;
               pTreeNode = gcnew TreeNode ( treeNodename );
               treeView1->Nodes->Add( pTreeNode );

                 flaeche = pPolygon->getFlaecheMitLoechern();
                 childNodeName = String::Concat ( "Fläche: ", QuConvert::ToString( flaeche, 2 ) );
                 pChildNode = gcnew TreeNode ( childNodeName );
                 pTreeNode->Nodes->Add( pChildNode );

                 normal = pPolygon->getNormal();
                 if ( normal != NULL )
                 {
                   normal->get( x, y, z );
                   childNodeName = String::Concat ( "Normale: (", QuConvert::ToString ( x, 4 ) , ", ", QuConvert::ToString ( y, 4 ) , ", ",
                     QuConvert::ToString ( z, 4 ) , ")" );
                   pChildNode = gcnew TreeNode ( childNodeName );
                   pTreeNode->Nodes->Add( pChildNode );
                 }

               childNodeName = "Stützpunkte der Außenkontur";
               pChildNode = gcnew TreeNode ( childNodeName );
               pTreeNode->Nodes->Add( pChildNode );

               pRing = pPolygon->getAussenkontur();
               if ( pRing == NULL )
                 break;

               anz = pRing->getPoints3D( vX, vY, vZ );
               for ( i = 0; i < anz; i++ )
               {
                 if ( koordNormieren )
                 {
                   vX[i] = vX[i] - bbSw[0];
                   vY[i] = vY[i] - bbSw[1];
                   vZ[i] = vZ[i] - bbSw[2];
                 }
                 childNodeName = String::Concat ( "( ", QuConvert::ToString ( vX[i], 4 ) , ", ", QuConvert::ToString ( vY[i], 4 ) , ", ",
                   QuConvert::ToString ( vZ[i], 4 ) , ")" );
                 pNodeStuetzpunkt = gcnew TreeNode ( childNodeName );
                 pChildNode->Nodes->Add( pNodeStuetzpunkt );
               }

               lochAnz = pPolygon->getLochAnz();
               if ( lochAnz > 0 )
               {
                 childNodeName = "Inselflächen";
                 pNodeLoch = gcnew TreeNode ( childNodeName );
                 pTreeNode->Nodes->Add( pNodeLoch );


                 for ( i = 0; i < lochAnz; i++ )
                 {
                   vX.clear();
                   vY.clear();
                   vZ.clear();

                   anz =  pPolygon->getLoch( i )->getPoints3D( vX, vY, vZ );

                   childNodeName = String::Concat( "Inselfläche ", Convert::ToString( i ) );
                   pChildNode = gcnew TreeNode ( childNodeName );
                   pNodeLoch->Nodes->Add( pChildNode );

                   for ( j = 0; j < anz; j++ )
                   {
                     if ( koordNormieren )
                     {
                       vX[j] = vX[j] - bbSw[0];
                       vY[j] = vY[j] - bbSw[1];
                       vZ[j] = vZ[j] - bbSw[2];
                     }
                     childNodeName = String::Concat ( "( ", QuConvert::ToString ( vX[j], 4 ) , ", ", QuConvert::ToString ( vY[j], 4 ) , ", ",
                       QuConvert::ToString ( vZ[j], 4 ) , ")" );
                     pNodeStuetzpunkt = gcnew TreeNode ( childNodeName );
                     pChildNode->Nodes->Add( pNodeStuetzpunkt );
                   }
                 }
               }
               break;
             }

           case MULTI_CURVE:
             {
               MultiCurve * pMultiCurve = dynamic_cast<MultiCurve*>(pGeometryActual);
               anz =  pMultiCurve->getCurves( vCurves );
               if ( anz > 0 )
               {
                 treeNodename = geometryType;
                 pTreeNode = gcnew TreeNode ( treeNodename );
                 treeView1->Nodes->Add( pTreeNode );

                 laenge = pMultiCurve->getLength();
                 childNodeName = String::Concat ( "Gesamtlänge: ", QuConvert::ToString( laenge, 2 ) );
                 pChildNode = gcnew TreeNode ( childNodeName );
                 pTreeNode->Nodes->Add( pChildNode );
               }
               for ( i = 0; i < anz; i++ )
               {
                 _Curve * pCurve = vCurves[i];

                 treeNodename = "Segment";
                 pChildNode = gcnew TreeNode ( treeNodename );
                 pTreeNode->Nodes->Add( pChildNode );

                 laenge = pCurve->getLength();
                 childNodeName = String::Concat ( "Länge: ", QuConvert::ToString( laenge, 2 ) );
                 pSubChildNode = gcnew TreeNode ( childNodeName );
                 pChildNode->Nodes->Add( pSubChildNode );

                 vX.clear();
                 vY.clear();
                 vZ.clear();
                 anz = pCurve->getPoints3D( vX, vY, vZ );
                 for ( i = 0; i < anz; i++ )
                 {
                   if ( koordNormieren )
                   {
                     vX[i] = vX[i] - bbSw[0];
                     vY[i] = vY[i] - bbSw[1];
                     vZ[i] = vZ[i] - bbSw[2];
                   }

                   childNodeName = String::Concat ( "( ", QuConvert::ToString ( vX[i], 4 ) , ", ", QuConvert::ToString ( vY[i], 4 ) , ", ",
                     QuConvert::ToString ( vZ[i], 4 ) , ")" );
                   pSubChildNode = gcnew TreeNode ( childNodeName );
                   pChildNode->Nodes->Add( pSubChildNode );
                 }
               }
               break;
             }

           case CURVE:
           case LINE_STRING:
					 case LINE_STRING_SEGMENT:
					 case CIRCLE:
           case ARC:
             {
               _Curve * pCurve = dynamic_cast<_Curve*>(pGeometryActual);

               treeNodename = geometryType;
               pTreeNode = gcnew TreeNode ( treeNodename );
               treeView1->Nodes->Add( pTreeNode );

               laenge = pCurve->getLength();
               childNodeName = String::Concat ( "Länge: ", QuConvert::ToString( laenge, 2 ) );
               pChildNode = gcnew TreeNode ( childNodeName );
               pTreeNode->Nodes->Add( pChildNode );

               treeNodename = "Stützpunkte";
               pChildNode = gcnew TreeNode ( treeNodename );
               pTreeNode->Nodes->Add( pChildNode );

               anz = pCurve->getPoints3D( vX, vY, vZ );
               for ( i = 0; i < anz; i++ )
               {
                 if ( koordNormieren )
                 {
                   vX[i] = vX[i] - bbSw[0];
                   vY[i] = vY[i] - bbSw[1];
                   vZ[i] = vZ[i] - bbSw[2];
                 }

                 childNodeName = String::Concat ( "( ", QuConvert::ToString ( vX[i], 4 ) , ", ", QuConvert::ToString ( vY[i], 4 ) , ", ",
                   QuConvert::ToString ( vZ[i], 4 ) , ")" );
                 pSubChildNode = gcnew TreeNode ( childNodeName );
                 pChildNode->Nodes->Add( pSubChildNode );
               }

             }

           }
           treeView1->ExpandAll();
         }

///////////////////////////////////////////////////////////////////////////////
//  Schreibt Informationen über einen Ring (Normalenrichtung) in den         //
//  Darstellungs-Baum                                                        //
///////////////////////////////////////////////////////////////////////////////
private: System::Void writeRingParameter ( TreeNode ^ pTreeNode, _Ring * pRing )       
        {
          double         maximalError, avarageError;
          double         x, y, z;
          Plane        * e;
          Direction      normal;
          String       ^ childNodeName;
          TreeNode     ^ pChildNode;
          unsigned int   i;
          vector<Error*> vErrors;

          if ( pRing->getCheckResult() == ADVANCED_CHECK_FULFILLED )
          {
            normal = pRing->getPlane()->getNormal();
            normal.get( x, y, z );
            childNodeName = String::Concat ( "Gültiger Ring, Normale: (", QuConvert::ToString ( x, 4 ) , ", ", QuConvert::ToString ( y, 4 ) , ", ",
              QuConvert::ToString ( z, 4 ) , ")" );
            pChildNode = gcnew TreeNode ( childNodeName );
            pTreeNode->Nodes->Add( pChildNode );

          }
          else
          {
            pRing->checkPlanarity( m_pFeature, "", m_pCheckParams, vErrors, maximalError, avarageError );
            for ( i = 0; i < vErrors.size(); i++ )
              delete vErrors[i];

            e = pRing->getPlane();
            if ( e != NULL )
            {
              normal = pRing->getPlane()->getNormal();
              normal.get( x, y, z );
              childNodeName = String::Concat ( "!!! Ungültiger Ring, Normale: (", QuConvert::ToString ( x, 4 ) , ", ", QuConvert::ToString ( y, 4 ) , ", ",
                QuConvert::ToString ( z, 4 ) , ")" );
              pChildNode = gcnew TreeNode ( childNodeName );
              pTreeNode->Nodes->Add( pChildNode );

              childNodeName = String::Concat( "Maximaler Planaritätsfehler = ", QuConvert::ToString ( maximalError, 6 ),
                ", Mittlerer Planaritätsfehler =  ", QuConvert::ToString ( avarageError, 6 ) );
              pChildNode = gcnew TreeNode ( childNodeName );
              pTreeNode->Nodes->Add( pChildNode );
            }
            else
            {
              childNodeName = "Ungültiger Ring, Normalenrerechnung nicht möglich";
              pChildNode = gcnew TreeNode ( childNodeName );
              pTreeNode->Nodes->Add( pChildNode );
            }
          }
        }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Selektieren eines Knotens im Baum                        //
///////////////////////////////////////////////////////////////////////////////
private: System::Void treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
         {
           vector<PolygonGeo*> vPolygone;
           vector<bool>     vOrientations;
           TreeNode       ^ pTreeNode;
           TreeNode       ^ pChildNode;
           String         ^ id;
           string           gmlId;
           String         ^ treeNodename;
           String         ^ childNodeName;
           String         ^ geometryType;
           double           volume;
           double           flaeche;
           vector<double>   vX, vY, vZ;
					 size_t           i, j, anz;
           _Ring          * pRing;

           pGeometryActual = NULL;

           pTreeNode = treeView1->SelectedNode;
           if ( pTreeNode == nullptr )	return;

           id =  dynamic_cast<String^>(pTreeNode->Tag );
           if ( id == nullptr ) return;

           QuConvert::systemStr2stdStr( gmlId, id );
           pGeometryActual = m_pFeatures->getGeometry( gmlId );
           if ( pGeometryActual == NULL ) return;

           if ( pTreeNode->Nodes->Count > 0 )
           {
             pTreeNode->Expand();
             return;
           }

           switch ( pGeometryActual->getGeometryType() )
           {
           case MULTI_SOLID:
             {
               vector<_Solid*> vSolids;

               MultiSolid * pMultiSolid = dynamic_cast<MultiSolid*>(pGeometryActual);
               anz =  pMultiSolid->getSolids( vSolids );
               if ( anz > 0 )
               {
                 treeNodename = geometryType;
                 pTreeNode = gcnew TreeNode ( treeNodename );
                 treeView1->Nodes->Add( pTreeNode );

                 if ( pMultiSolid->getCheckResult() == TOPOLOGY_CHECK_FULFILLED )
                 {
                   volume = pMultiSolid->getVolume();
                   childNodeName = String::Concat ( "Volumen: ", QuConvert::ToString( volume, 2 ) );
                 }
                 else
                   childNodeName = "!!! Volumenberechnung nicht möglich";
                 pChildNode = gcnew TreeNode ( childNodeName );
                 pTreeNode->Nodes->Add( pChildNode );
               }
               for ( i = 0; i < anz; i++ )
               {
                 treeNodename = "Solid";
                 pTreeNode = gcnew TreeNode ( treeNodename );
                 pTreeNode->Tag = QuConvert::ToString( vSolids[i]->getGmlId() );
                 treeView1->Nodes->Add( pTreeNode );
               }

               break;
             }

           case SOLID:
             {
               Solid * pSolid = dynamic_cast<Solid*>(pGeometryActual);
               anz =  pSolid->getAussenkonturPolygone( vPolygone, vOrientations );
               if ( anz > 0 )
               {
                 treeNodename = geometryType;
                 pTreeNode = gcnew TreeNode ( treeNodename );
                 treeView1->Nodes->Add( pTreeNode );

                 if ( pSolid->getCheckResult() == TOPOLOGY_CHECK_FULFILLED )
                 {
                   volume = pSolid->getVolume();
                   childNodeName = String::Concat ( "Volumen: ", QuConvert::ToString( volume, 2 ) );
                   pChildNode = gcnew TreeNode ( childNodeName );
                   pTreeNode->Nodes->Add( pChildNode );

                   flaeche = pSolid->getOberflaeche();
                   childNodeName = String::Concat ( "Oberfläche: ", QuConvert::ToString( flaeche, 2 ) );
                   pChildNode = gcnew TreeNode ( childNodeName );
                   pTreeNode->Nodes->Add( pChildNode );
                 }
                 else
                 {
                   childNodeName ="!!! Volumen- und Oberflächenberechnung nicht möglich";
                   pChildNode = gcnew TreeNode ( childNodeName );
                   pTreeNode->Nodes->Add( pChildNode );
                 }
               }
               for ( i = 0; i < anz; i++ )
               {
                 if ( vOrientations[i] == true )
                   treeNodename = "Polygon (normal)";
                 else
                   treeNodename = "Polygon (invertiert)";
                 pTreeNode = gcnew TreeNode ( treeNodename );
                 pTreeNode->Tag = QuConvert::ToString( vPolygone[i]->getGmlId() );
                 treeView1->Nodes->Add( pTreeNode );
               }

               break;
             }

           case MULTI_SURFACE:
           case COMPOSITE_SURFACE:
           case SHELL:
           case TRIANGULATED_SURFACE:
           case SURFACE:
           case ORIENTABLE_SURFACE:
             {
               _Surface * pSurface = dynamic_cast<_Surface*>(pGeometryActual);
               anz =  pSurface->getPolygone( vPolygone, vOrientations );
               if ( anz > 0 )
               {
                 treeNodename = geometryType;
                 pTreeNode = gcnew TreeNode ( treeNodename );
                 treeView1->Nodes->Add( pTreeNode );

                   flaeche = pSurface->getFlaeche();
                   childNodeName = String::Concat ( "Oberfläche: ", QuConvert::ToString( flaeche, 2 ) );
                 pChildNode = gcnew TreeNode ( childNodeName );
                 pTreeNode->Nodes->Add( pChildNode );
               }
               for ( i = 0; i < anz; i++ )
               {
                 if ( vOrientations[i] == true )
                   childNodeName = "Polygon (normal)";
                 else
                   childNodeName = "Polygon (invertiert)";
                 pChildNode = gcnew TreeNode ( childNodeName );
                 pChildNode->Tag = QuConvert::ToString( vPolygone[i]->getGmlId() );
                 pTreeNode->Nodes->Add( pChildNode );
               }
               break;
             }           

           case POLYGON:
           case TRIANGLE:
             {
               PolygonGeo * pPolygon =  dynamic_cast<PolygonGeo*>(pGeometryActual);
               double       flaeche, x, y, z, dist;
							 size_t       lochAnz;
               Plane        p;
               Direction *  normal;
               GeoPoint     center;
               TreeNode   ^ pNodeLoch;
               TreeNode   ^ pNodeStuetzpunkt;
               string       gmlId;

               gmlId = pPolygon->getGmlId();
               childNodeName = String::Concat ( "GML-Id: ", QuConvert::ToString( gmlId ) );
               pChildNode = gcnew TreeNode ( childNodeName );
               pTreeNode->Nodes->Add( pChildNode );

                 flaeche = pPolygon->getFlaecheMitLoechern();
                 childNodeName = String::Concat ( "Fläche: ", QuConvert::ToString( flaeche, 2 ) );
                 pChildNode = gcnew TreeNode ( childNodeName );
                 pTreeNode->Nodes->Add( pChildNode );

                 normal = pPolygon->getNormal();
                 normal->get( x, y, z );
                 childNodeName = String::Concat ( "Normale: (", QuConvert::ToString ( x, 4 ) , ", ", QuConvert::ToString ( y, 4 ) , ", ",
                   QuConvert::ToString ( z, 4 ) , ")" );
                 pChildNode = gcnew TreeNode ( childNodeName );
                 pTreeNode->Nodes->Add( pChildNode );

               childNodeName = "Stützpunkte der Außenkontur";
               pChildNode = gcnew TreeNode ( childNodeName );
               pTreeNode->Nodes->Add( pChildNode );

               pRing = pPolygon->getAussenkontur();
               if ( pRing == NULL )
                 break;

               anz = pPolygon->getAussenkontur()->getPoints3D( vX, vY, vZ );
               for ( i = 0; i < anz; i++ )
               {
                 GeoPoint polyPoint = GeoPoint (  vX[i], vY[i], vZ[i] );
                 dist = p.getDistance( polyPoint );

                 if ( koordNormieren )
                 {
                   vX[i] = vX[i] - bbSw[0];
                   vY[i] = vY[i] - bbSw[1];
                   vZ[i] = vZ[i] - bbSw[2];
                 }

                 childNodeName = String::Concat ( "( ", QuConvert::ToString ( vX[i], 4 ) , ", ", QuConvert::ToString ( vY[i], 4 ) , ", ",
                                                  QuConvert::ToString ( vZ[i], 4 ) , ")" );
                 pNodeStuetzpunkt = gcnew TreeNode ( childNodeName );
                 pChildNode->Nodes->Add( pNodeStuetzpunkt );
               }

               lochAnz = pPolygon->getLochAnz();
               if ( lochAnz > 0 )
               {
                 childNodeName = "Inselflächen";
                 pNodeLoch = gcnew TreeNode ( childNodeName );
                 pChildNode->Nodes->Add( pNodeLoch );

                 for ( i = 0; i < lochAnz; i++ )
                 {
                   vX.clear();
                   vY.clear();
                   vZ.clear();
                   anz =  pPolygon->getLoch( i )->getPoints3D( vX, vY, vZ );

                   childNodeName = String::Concat( "Inselfläche ", Convert::ToString( i ) );
                   pChildNode = gcnew TreeNode ( childNodeName );
                   pNodeLoch->Nodes->Add( pChildNode );

                   for ( j = 0; j < anz; j++ )
                   {
                     GeoPoint polyPoint = GeoPoint (  vX[j], vY[j], vZ[j] );
                     dist = p.getDistance( polyPoint );

                     if ( koordNormieren )
                     {
                       vX[j] = vX[j] - bbSw[0];
                       vY[j] = vY[j] - bbSw[1];
                       vZ[j] = vZ[j] - bbSw[2];                 }

                     childNodeName = String::Concat ( "( ", QuConvert::ToString ( vX[j], 4 ) , ", ", QuConvert::ToString ( vY[j], 4 ) , ", ",
                                                       QuConvert::ToString ( vZ[j], 4 ) , ")"  );
                     pNodeStuetzpunkt = gcnew TreeNode ( childNodeName );
                     pChildNode->Nodes->Add( pNodeStuetzpunkt );
                   }
                 }
               }
             }
             pTreeNode->Expand();
             break;
           case MULTI_CURVE:
             {

             }
           }
          
         }

///////////////////////////////////////////////////////////////////////////////
//  Interface: Setzen des Attributs koordNormieren                           // 
///////////////////////////////////////////////////////////////////////////////
public: System::Void setKoordNormierten ( bool b )
        {
          koordNormieren = b;
        }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Außenkontur umdrehen"                             //
//  Kehrt die Laufrichtung der Außenkontur-Stützpunkte eines Polygons um     //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonReverseContour_Click(System::Object^  sender, System::EventArgs^  e)
         {
           PolygonGeo * pPolygon;

           if ( pGeometryReferenced != NULL )
             pPolygon = (PolygonGeo*)pGeometryReferenced;
           else
             pPolygon = (PolygonGeo*)pGeometryActual;

           pPolygon->getAussenkontur()->reverse();
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Innenkonturen umdrehen"                           //
//  Kehrt die Laufrichtung der Innenkontur-Stützpunkte eines Polygons um     //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonReverseHoles_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           PolygonGeo * pPolygon;

           if ( pGeometryReferenced != NULL )
             pPolygon = (PolygonGeo*)pGeometryReferenced;
           else
             pPolygon = (PolygonGeo*)pGeometryActual;

           for ( unsigned int i = 0; i < pPolygon->getLochAnz(); i++ )
             pPolygon->getLoch( i )->reverse();
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Polygon umdrehen"                                 //
//  Kehrt die Laufrichtung con Außenkotur und Innenkontur-Stützpunkten eines //
//  Polygons um                                                              //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonReversePolygon_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           PolygonGeo * pPolygon;

           if ( pGeometryReferenced != NULL )
             pPolygon = (PolygonGeo*)pGeometryReferenced;
           else
             pPolygon = (PolygonGeo*)pGeometryActual;

           pPolygon->reverse();
         }
};
