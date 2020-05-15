#pragma once

#include "convert.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "FeatureAttribute.h"
#include "GeometryInspector.h"
#include "FeatureOffset.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;



	/// <summary> 
	/// Zugriff auf alle aktuell vorhandenen Features zur Anzeige und zum Ändern von Attributen
	///
	/// </summary>
public ref class StrukturDialog : public System::Windows::Forms::Form
{

public: 
	StrukturDialog( Features * pFeatures, Checking * pChecking )
	{
			InitializeComponent();

			m_pFeatures      = pFeatures;
      m_pChecking      = pChecking;
      pObjektSelected  = NULL;

			initializeTree();

      pFeatureAttribute = gcnew FeatureAttribute( m_pFeatures, NULL, nullptr );
      pFeatureAttribute->setShowEmptyAttributes( false );
      pFeatureAttribute->hideButtons();
      pFeatureAttribute->Show();
	}

	protected: 

~StrukturDialog()
{
  pFeatureAttribute->Close();

  if (components)
  {
    delete components;
  }
}

private: 
	System::Windows::Forms::TreeView ^  treeViewPlanStruktur;

	Features         * m_pFeatures;           // Aggregation aller Features
  Checking         * m_pChecking;           // Checking-Modul
  FeatureAttribute ^ pFeatureAttribute;     // Dialog zur Anzeige und zum Ändern der Attribute des selektierten Features
  Feature          * pObjektSelected;       // Aktuell selektiertes Feature

private: System::Windows::Forms::Button ^  buttonUebernehmen;
private: System::Windows::Forms::ContextMenu ^  contextMenu1;
private: System::Windows::Forms::MenuItem ^  menuItem1;
private: System::Windows::Forms::MenuItem ^  menuItem7;
private: System::Windows::Forms::MenuItem^  menuItem2;
private: System::Windows::Forms::MenuItem^  menuItem3;

private:
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
    this->treeViewPlanStruktur = (gcnew System::Windows::Forms::TreeView());
    this->contextMenu1 = (gcnew System::Windows::Forms::ContextMenu());
    this->menuItem1 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem2 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem7 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem3 = (gcnew System::Windows::Forms::MenuItem());
    this->buttonUebernehmen = (gcnew System::Windows::Forms::Button());
    this->SuspendLayout();
    // 
    // treeViewPlanStruktur
    // 
    this->treeViewPlanStruktur->ContextMenu = this->contextMenu1;
    this->treeViewPlanStruktur->Dock = System::Windows::Forms::DockStyle::Fill;
    this->treeViewPlanStruktur->Location = System::Drawing::Point(0, 0);
    this->treeViewPlanStruktur->Name = L"treeViewPlanStruktur";
    this->treeViewPlanStruktur->Size = System::Drawing::Size(312, 511);
    this->treeViewPlanStruktur->TabIndex = 0;
    this->treeViewPlanStruktur->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &StrukturDialog::treeViewPlanStruktur_AfterSelect);
    // 
    // contextMenu1
    // 
    this->contextMenu1->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(4) {this->menuItem1, this->menuItem2, 
      this->menuItem7, this->menuItem3});
    this->contextMenu1->Popup += gcnew System::EventHandler(this, &StrukturDialog::contextMenu1_Popup);
    // 
    // menuItem1
    // 
    this->menuItem1->Index = 0;
    this->menuItem1->Text = L"Attribute bearbeiten";
    this->menuItem1->Click += gcnew System::EventHandler(this, &StrukturDialog::menuItem1_Click);
    // 
    // menuItem2
    // 
    this->menuItem2->Index = 1;
    this->menuItem2->Text = L"Geometrie analysieren";
    this->menuItem2->Click += gcnew System::EventHandler(this, &StrukturDialog::menuItem2_Click);
    // 
    // menuItem7
    // 
    this->menuItem7->Index = 2;
    this->menuItem7->Text = L"-";
    // 
    // menuItem3
    // 
    this->menuItem3->Index = 3;
    this->menuItem3->Text = L"Transformieren";
    this->menuItem3->Click += gcnew System::EventHandler(this, &StrukturDialog::menuItem3_Click);
    // 
    // buttonUebernehmen
    // 
    this->buttonUebernehmen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
    this->buttonUebernehmen->DialogResult = System::Windows::Forms::DialogResult::OK;
    this->buttonUebernehmen->Location = System::Drawing::Point(120, 512);
    this->buttonUebernehmen->Name = L"buttonUebernehmen";
    this->buttonUebernehmen->Size = System::Drawing::Size(80, 23);
    this->buttonUebernehmen->TabIndex = 1;
    this->buttonUebernehmen->Text = L"Übernehmen";
    this->buttonUebernehmen->Click += gcnew System::EventHandler(this, &StrukturDialog::buttonUebernehmen_Click);
    // 
    // StrukturDialog
    // 
    this->AcceptButton = this->buttonUebernehmen;
    this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
    this->ClientSize = System::Drawing::Size(312, 541);
    this->Controls->Add(this->buttonUebernehmen);
    this->Controls->Add(this->treeViewPlanStruktur);
    this->Name = L"StrukturDialog";
    this->Padding = System::Windows::Forms::Padding(0, 0, 0, 30);
    this->Text = L"Modellbaum";
    this->ResumeLayout(false);

  }		


///////////////////////////////////////////////////////////////////////////////
//  Baut den Tree-View mit den vorhandenen Features auf                      //
///////////////////////////////////////////////////////////////////////////////
void initializeTree()
{
  multimap<string,Feature*>           mChildren;
  multimap<string,Feature*>::iterator iterFeatures;
  set<string>                         sKlassenNamen;
  set<string>::iterator               iterKlassenNamen;
  string                              klassenName;
  string                              gmlId;
  TreeNode                          ^ pTreeNodeKlasse;
  TreeNode                          ^ pTreeNodeObjekt;
	size_t                              anz;
  Feature                           * pFeature;
  String                            ^ STR;
  Color                               col;

  m_pFeatures->getChildren( mChildren, sKlassenNamen );

  for ( iterKlassenNamen = sKlassenNamen.begin(); iterKlassenNamen != sKlassenNamen.end(); iterKlassenNamen++ )
  {
    klassenName = *iterKlassenNamen;
    anz         = mChildren.count( klassenName );

    STR = String::Concat ( QuConvert::ToString ( klassenName ), " (",
      Convert::ToString ( anz ), ")" );
    pTreeNodeKlasse = gcnew TreeNode ( STR );
    treeViewPlanStruktur->Nodes->Add ( pTreeNodeKlasse );

    for ( iterFeatures = mChildren.lower_bound ( klassenName ); iterFeatures != mChildren.upper_bound( klassenName ); iterFeatures++ )
    {
      pFeature = iterFeatures->second;
      gmlId    = pFeature->getGmlId();
      STR = QuConvert::ToString ( gmlId );

      pTreeNodeObjekt = gcnew TreeNode ( STR );
      pTreeNodeObjekt->ForeColor = col;
      pTreeNodeObjekt->Tag = STR;

      pTreeNodeKlasse->Nodes->Add ( pTreeNodeObjekt );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commanhandler Button Übernehmen; Beendet den Dialog                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonUebernehmen_Click(System::Object ^  sender, System::EventArgs ^  e)
{
    Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Aktivierung des Kontextmenüs (Rechte Maustaste)          //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void contextMenu1_Popup(System::Object ^  sender, System::EventArgs ^  e)
{
  if ( pObjektSelected != NULL )
    menuItem1->Enabled = true;
  else
    menuItem1->Enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
// Commandhandler "Attribute bearbeiten" im Kontextmenü                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem1_Click(System::Object ^  sender, System::EventArgs ^  e)
{
	pFeatureAttribute->setEditEnabled( true );
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Commandhandler: Selektion eines Features im TreeView                      //
// Die Attribute des Features werden im FeatureAttribute Dialog angezeigt    //
// Der Feature Baum wird expandiert, um die von selektierten Feature         //
//  referierten Features anzuzeigen                                          //
///////////////////////////////////////////////////////////////////////////////
private: System::Void treeViewPlanStruktur_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
{
  TreeNode                          ^ selectedNode;
  String                            ^ id;
  string                              stdString;
  multimap<string,Feature*>           mChildren;
  multimap<string,Feature*>::iterator iterFeatures;
  vector<FeatureRelation*>            vRelationen;
  set<string>                         sKlassenNamen;
  set<string>::iterator               iterKlassenNamen;
  string                              klassenName;
  string                              gmlId;
  TreeNode                          ^ pTreeNodeKlasse;
  TreeNode                          ^ pTreeNodeObjekt;
	size_t                              i, anz;
  Feature                           * pFeature;
  String                            ^ STR;
  Color                               col;
  Feature                           * pGroupFeature;

  pFeatureAttribute->attributwerteUebernehmen();

  pObjektSelected = NULL;

  selectedNode = treeViewPlanStruktur->SelectedNode;
  if ( selectedNode == nullptr )	return;

  id =  dynamic_cast<String^>(selectedNode->Tag );
  if ( id == nullptr ) return;

  QuConvert::systemStr2stdStr( stdString, id );
  pObjektSelected = m_pFeatures->getFeature( stdString );

  pFeatureAttribute->setFeature ( stdString, pObjektSelected, false, false, true );

  if ( pObjektSelected == NULL || selectedNode->Nodes->Count > 0 ) return;

  if ( pObjektSelected->getKlassenName() == "group:CityObjectGroup" )
  {
    anz = pObjektSelected->getFeatureRelationen( "group:groupMember", vRelationen );
    for ( i = 0; i < anz; i++ )
    {
      pGroupFeature = m_pFeatures->getFeature( vRelationen[i]->featureId );
      if ( pGroupFeature != NULL )
      {
        mChildren.insert( multimap<string,Feature*>::value_type ( pGroupFeature->getKlassenName(), pGroupFeature ) );
        sKlassenNamen.insert( pGroupFeature->getKlassenName() );
      }
    }
  }

  for ( iterKlassenNamen = sKlassenNamen.begin(); iterKlassenNamen != sKlassenNamen.end(); iterKlassenNamen++ )
  {
    klassenName = *iterKlassenNamen;
    anz         = mChildren.count( klassenName );

    STR = String::Concat ( QuConvert::ToString ( klassenName ), " (",
      Convert::ToString ( anz ), ")" );
    pTreeNodeKlasse = gcnew TreeNode ( STR );
    selectedNode->Nodes->Add ( pTreeNodeKlasse );

    for ( iterFeatures = mChildren.lower_bound ( klassenName ); iterFeatures != mChildren.upper_bound( klassenName ); iterFeatures++ )
    {
      pFeature = iterFeatures->second;
      gmlId    = pFeature->getGmlId();
      STR = QuConvert::ToString ( gmlId );

      pTreeNodeObjekt = gcnew TreeNode ( STR );
      pTreeNodeObjekt->Checked  = true;
      pTreeNodeObjekt->ForeColor = col;
      pTreeNodeObjekt->Tag = STR;

      pTreeNodeKlasse->Nodes->Add ( pTreeNodeObjekt );
    }
  }

  if ( pObjektSelected->getFeatureRelationenAnz() > 0 )
  {
    vector<FeatureRelation*> vRelationen;
    TreeNode               ^ pTreeNodeRelation;

    anz = pObjektSelected->getFeatureRelationen( vRelationen );
    for ( i = 0; i < anz; i++ )
    {
      FeatureRelation * pRel = vRelationen[i];
      if ( pRel->nil )
        continue;

      STR = QuConvert::ToString ( pRel->rollenName );

      pTreeNodeRelation = gcnew TreeNode ( STR );
      pTreeNodeRelation->Checked  = true;
      pTreeNodeRelation->ForeColor = col;

      STR = QuConvert::ToString ( pRel->featureId );
      pTreeNodeRelation->Tag = STR;

      selectedNode->Nodes->Add ( pTreeNodeRelation );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Commandhandler "Geometrie analysieren" im Kontextmenü                     //
///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem2_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           if ( pObjektSelected == NULL ) return;

           GeometryInspector ^ pGeometryInspector = gcnew GeometryInspector ( m_pChecking, pObjektSelected, NULL );
           pGeometryInspector->ShowDialog();
         }

///////////////////////////////////////////////////////////////////////////////
// Commandhandler "Transformieren" im Kontextmenü                            //
///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem3_Click(System::Object^  sender, System::EventArgs^  e)
         {
           if ( pObjektSelected == NULL ) return;

           FeatureOffset ^ pFeatureOffset = gcnew FeatureOffset ( pObjektSelected );
           pFeatureOffset->ShowDialog();

         }
};