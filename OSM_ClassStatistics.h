#pragma once

#include "Osm.h"
#include "Convert.h"

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

  using namespace std;

	/// <summary>
	/// Zusammenfassung für OSM_ClassStatistics
	/// </summary>
	public ref class OSM_ClassStatistics : public System::Windows::Forms::Form
	{
	public:
		OSM_ClassStatistics( OSM_StatisticsAll * pStat )
		{
			InitializeComponent();

      pOSM_Statistics = pStat;
      generateTreeView();
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~OSM_ClassStatistics()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TreeView^  treeViewOSM;
  protected: 
    OSM_StatisticsAll * pOSM_Statistics;

  private: System::Windows::Forms::Button^  buttonOk;
  protected: 

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
      this->treeViewOSM = (gcnew System::Windows::Forms::TreeView());
      this->buttonOk = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // treeViewOSM
      // 
      this->treeViewOSM->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->treeViewOSM->Location = System::Drawing::Point(-1, 2);
      this->treeViewOSM->Name = L"treeViewOSM";
      this->treeViewOSM->Size = System::Drawing::Size(382, 217);
      this->treeViewOSM->TabIndex = 0;
      // 
      // buttonOk
      // 
      this->buttonOk->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
      this->buttonOk->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonOk->Location = System::Drawing::Point(158, 227);
      this->buttonOk->Name = L"buttonOk";
      this->buttonOk->Size = System::Drawing::Size(75, 23);
      this->buttonOk->TabIndex = 1;
      this->buttonOk->Text = L"Ok";
      this->buttonOk->UseVisualStyleBackColor = true;
      this->buttonOk->Click += gcnew System::EventHandler(this, &OSM_ClassStatistics::buttonOk_Click);
      // 
      // OSM_ClassStatistics
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(379, 262);
      this->Controls->Add(this->buttonOk);
      this->Controls->Add(this->treeViewOSM);
      this->Name = L"OSM_ClassStatistics";
      this->Text = L"Übersicht eingelesenes OSM-Modell";
      this->ResumeLayout(false);

    }

    void generateTreeView()
    {
      TreeNode       ^ pTreeNode;

      treeViewOSM->Nodes->Clear();

      pTreeNode = generateSubTree ( pOSM_Statistics->pNodeStatistics, "node", pOSM_Statistics->pNodeStatistics->getObjAnz() );
      treeViewOSM->Nodes->Add ( pTreeNode );

      pTreeNode = generateSubTree ( pOSM_Statistics->pWayStatistics, "way", pOSM_Statistics->pWayStatistics->getObjAnz() );
      treeViewOSM->Nodes->Add ( pTreeNode );

      pTreeNode = generateSubTree ( pOSM_Statistics->pRelationStatistics, "relation", pOSM_Statistics->pRelationStatistics->getObjAnz() );
      treeViewOSM->Nodes->Add ( pTreeNode );
    }

    TreeNode ^ generateSubTree( OSM_Statistics * pStat, String ^ className, size_t objAnz )
    {
      String                  ^ STR;
      vector<string>            vKeys;
      TreeNode                ^ pSubtreeNode;
      TreeNode                ^ pSubSubtreeNode;
      vector<map<string,int>>   vValueCounts;
      map<string,int>           mValueCount;
      map<string,int>::iterator iter;

      STR = String::Concat ( "Anzahl ", className, " Objekte: ", Convert::ToString( objAnz ) );
      TreeNode ^ pTreeNode = gcnew TreeNode ( STR );

      pStat->getStat( vKeys, vValueCounts );
      for ( unsigned int i = 0; i < vKeys.size(); i++ )
      {
        String ^ key = QuConvert::ToString( vKeys[i] );
        mValueCount = vValueCounts[i];
				size_t valueCount  = pStat->getTagValueAnz( vKeys[i] );

        STR = String::Concat( "Tag \"", key, "\" (", Convert::ToString( valueCount), ")" );
        pSubtreeNode = gcnew TreeNode ( STR );
        pTreeNode->Nodes->Add( pSubtreeNode );
        for ( iter = mValueCount.begin(); iter != mValueCount.end(); iter++  )
        {
          STR = String::Concat( "Value \"", QuConvert::ToString( iter->first ), "\": ", Convert::ToString( iter->second ) );
          pSubSubtreeNode = gcnew TreeNode ( STR );
          pSubtreeNode->Nodes->Add( pSubSubtreeNode );
        }
      }
      return pTreeNode;
    }

#pragma endregion
  private: System::Void buttonOk_Click(System::Object^  sender, System::EventArgs^  e)
           {
             Close();
           }
};
