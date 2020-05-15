#pragma once

#include "GmlModelTest.h"
#include "Convert.h"

	/// <summary>
	/// Anzeige statistischer und sonstiger Informationen über ein eingelesenes GML-Modell
	/// </summary>
	public ref class ModellStatistik : public System::Windows::Forms::Form
	{
	public:
		ModellStatistik( GmlModelStatistics * pModelStatisticsP, String ^ dateiName )
		{
			InitializeComponent();

      pModelStatistics = pModelStatisticsP;
      textBoxDateiName->Text = dateiName;

      initialize();			
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~ModellStatistik()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::TextBox^  textBoxDateiName;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  textBoxBBMin;
  private: System::Windows::Forms::TextBox^  textBoxBBMax;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::TextBox^  textBoxSrsName;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::TextBox^  textBoxSrsDimension;
  private: System::Windows::Forms::Label^  label5;
  private: System::Windows::Forms::GroupBox^  groupBoxFeatureStatistik;
  private: System::Windows::Forms::TreeView^  treeViewKlassenStatistik;
  private: System::Windows::Forms::Label^  label7;
  private: System::Windows::Forms::TextBox^  textBoxAnzahlRaumbezogen;
  private: System::Windows::Forms::Label^  label6;
  private: System::Windows::Forms::TextBox^  textBoxFeatureAnzahl;
  private: System::Windows::Forms::TextBox^  textBoxDefaultNamespace;
  private: System::Windows::Forms::ListBox^  listBoxNamespaces;
  private: System::Windows::Forms::Label^  label8;
  private: System::Windows::Forms::GroupBox^  groupBoxNamespaces;
  private: System::Windows::Forms::Button^  buttonBeenden;
  private: System::Windows::Forms::Label^  label9;

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

    GmlModelStatistics * pModelStatistics;      //  Anzuzeigende Modell-Statistik

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->textBoxDateiName = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->textBoxBBMin = (gcnew System::Windows::Forms::TextBox());
      this->textBoxBBMax = (gcnew System::Windows::Forms::TextBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->textBoxSrsName = (gcnew System::Windows::Forms::TextBox());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->textBoxSrsDimension = (gcnew System::Windows::Forms::TextBox());
      this->label5 = (gcnew System::Windows::Forms::Label());
      this->groupBoxFeatureStatistik = (gcnew System::Windows::Forms::GroupBox());
      this->treeViewKlassenStatistik = (gcnew System::Windows::Forms::TreeView());
      this->label7 = (gcnew System::Windows::Forms::Label());
      this->textBoxAnzahlRaumbezogen = (gcnew System::Windows::Forms::TextBox());
      this->label6 = (gcnew System::Windows::Forms::Label());
      this->textBoxFeatureAnzahl = (gcnew System::Windows::Forms::TextBox());
      this->textBoxDefaultNamespace = (gcnew System::Windows::Forms::TextBox());
      this->listBoxNamespaces = (gcnew System::Windows::Forms::ListBox());
      this->label8 = (gcnew System::Windows::Forms::Label());
      this->groupBoxNamespaces = (gcnew System::Windows::Forms::GroupBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->label9 = (gcnew System::Windows::Forms::Label());
      this->groupBoxFeatureStatistik->SuspendLayout();
      this->groupBoxNamespaces->SuspendLayout();
      this->SuspendLayout();
      // 
      // textBoxDateiName
      // 
      this->textBoxDateiName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxDateiName->Location = System::Drawing::Point(13, 13);
      this->textBoxDateiName->Name = L"textBoxDateiName";
      this->textBoxDateiName->ReadOnly = true;
      this->textBoxDateiName->Size = System::Drawing::Size(484, 20);
      this->textBoxDateiName->TabIndex = 0;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(13, 37);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(58, 13);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Dateiname";
      // 
      // textBoxBBMin
      // 
      this->textBoxBBMin->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxBBMin->Location = System::Drawing::Point(13, 64);
      this->textBoxBBMin->Name = L"textBoxBBMin";
      this->textBoxBBMin->ReadOnly = true;
      this->textBoxBBMin->Size = System::Drawing::Size(230, 20);
      this->textBoxBBMin->TabIndex = 2;
      // 
      // textBoxBBMax
      // 
      this->textBoxBBMax->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxBBMax->Location = System::Drawing::Point(267, 64);
      this->textBoxBBMax->Name = L"textBoxBBMax";
      this->textBoxBBMax->ReadOnly = true;
      this->textBoxBBMax->Size = System::Drawing::Size(230, 20);
      this->textBoxBBMax->TabIndex = 3;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(13, 87);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(117, 13);
      this->label2->TabIndex = 4;
      this->label2->Text = L"Bounding Box Minimum";
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(267, 87);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(120, 13);
      this->label3->TabIndex = 5;
      this->label3->Text = L"Bounding Box Maximum";
      // 
      // textBoxSrsName
      // 
      this->textBoxSrsName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxSrsName->Location = System::Drawing::Point(13, 114);
      this->textBoxSrsName->Name = L"textBoxSrsName";
      this->textBoxSrsName->ReadOnly = true;
      this->textBoxSrsName->Size = System::Drawing::Size(230, 20);
      this->textBoxSrsName->TabIndex = 6;
      // 
      // label4
      // 
      this->label4->AutoSize = true;
      this->label4->Location = System::Drawing::Point(13, 138);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(142, 13);
      this->label4->TabIndex = 7;
      this->label4->Text = L"Koordinaten-Referenzsystem";
      // 
      // textBoxSrsDimension
      // 
      this->textBoxSrsDimension->Anchor = System::Windows::Forms::AnchorStyles::Top;
      this->textBoxSrsDimension->Location = System::Drawing::Point(270, 114);
      this->textBoxSrsDimension->Name = L"textBoxSrsDimension";
      this->textBoxSrsDimension->ReadOnly = true;
      this->textBoxSrsDimension->Size = System::Drawing::Size(56, 20);
      this->textBoxSrsDimension->TabIndex = 8;
      // 
      // label5
      // 
      this->label5->AutoSize = true;
      this->label5->Location = System::Drawing::Point(270, 137);
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size(56, 13);
      this->label5->TabIndex = 9;
      this->label5->Text = L"Dimension";
      // 
      // groupBoxFeatureStatistik
      // 
      this->groupBoxFeatureStatistik->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBoxFeatureStatistik->Controls->Add(this->treeViewKlassenStatistik);
      this->groupBoxFeatureStatistik->Controls->Add(this->label7);
      this->groupBoxFeatureStatistik->Controls->Add(this->textBoxAnzahlRaumbezogen);
      this->groupBoxFeatureStatistik->Controls->Add(this->label6);
      this->groupBoxFeatureStatistik->Controls->Add(this->textBoxFeatureAnzahl);
      this->groupBoxFeatureStatistik->Location = System::Drawing::Point(16, 341);
      this->groupBoxFeatureStatistik->Name = L"groupBoxFeatureStatistik";
      this->groupBoxFeatureStatistik->Size = System::Drawing::Size(481, 281);
      this->groupBoxFeatureStatistik->TabIndex = 11;
      this->groupBoxFeatureStatistik->TabStop = false;
      this->groupBoxFeatureStatistik->Text = L"Feature Statistik";
      // 
      // treeViewKlassenStatistik
      // 
      this->treeViewKlassenStatistik->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->treeViewKlassenStatistik->Location = System::Drawing::Point(0, 66);
      this->treeViewKlassenStatistik->Name = L"treeViewKlassenStatistik";
      this->treeViewKlassenStatistik->Size = System::Drawing::Size(480, 215);
      this->treeViewKlassenStatistik->TabIndex = 4;
      // 
      // label7
      // 
      this->label7->AutoSize = true;
      this->label7->Location = System::Drawing::Point(218, 40);
      this->label7->Name = L"label7";
      this->label7->Size = System::Drawing::Size(159, 13);
      this->label7->TabIndex = 3;
      this->label7->Text = L"Anzahl raumbezogener Features";
      // 
      // textBoxAnzahlRaumbezogen
      // 
      this->textBoxAnzahlRaumbezogen->Location = System::Drawing::Point(162, 33);
      this->textBoxAnzahlRaumbezogen->Name = L"textBoxAnzahlRaumbezogen";
      this->textBoxAnzahlRaumbezogen->ReadOnly = true;
      this->textBoxAnzahlRaumbezogen->Size = System::Drawing::Size(49, 20);
      this->textBoxAnzahlRaumbezogen->TabIndex = 2;
      // 
      // label6
      // 
      this->label6->AutoSize = true;
      this->label6->Location = System::Drawing::Point(43, 40);
      this->label6->Name = L"label6";
      this->label6->Size = System::Drawing::Size(62, 13);
      this->label6->TabIndex = 1;
      this->label6->Text = L"Gesamtzahl";
      // 
      // textBoxFeatureAnzahl
      // 
      this->textBoxFeatureAnzahl->Location = System::Drawing::Point(4, 34);
      this->textBoxFeatureAnzahl->Name = L"textBoxFeatureAnzahl";
      this->textBoxFeatureAnzahl->ReadOnly = true;
      this->textBoxFeatureAnzahl->Size = System::Drawing::Size(32, 20);
      this->textBoxFeatureAnzahl->TabIndex = 0;
      // 
      // textBoxDefaultNamespace
      // 
      this->textBoxDefaultNamespace->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->textBoxDefaultNamespace->Location = System::Drawing::Point(3, 19);
      this->textBoxDefaultNamespace->Name = L"textBoxDefaultNamespace";
      this->textBoxDefaultNamespace->ReadOnly = true;
      this->textBoxDefaultNamespace->Size = System::Drawing::Size(481, 20);
      this->textBoxDefaultNamespace->TabIndex = 0;
      // 
      // listBoxNamespaces
      // 
      this->listBoxNamespaces->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->listBoxNamespaces->FormattingEnabled = true;
      this->listBoxNamespaces->Location = System::Drawing::Point(3, 69);
      this->listBoxNamespaces->Name = L"listBoxNamespaces";
      this->listBoxNamespaces->SelectionMode = System::Windows::Forms::SelectionMode::None;
      this->listBoxNamespaces->Size = System::Drawing::Size(480, 82);
      this->listBoxNamespaces->Sorted = true;
      this->listBoxNamespaces->TabIndex = 1;
      // 
      // label8
      // 
      this->label8->AutoSize = true;
      this->label8->Location = System::Drawing::Point(7, 41);
      this->label8->Name = L"label8";
      this->label8->Size = System::Drawing::Size(110, 13);
      this->label8->TabIndex = 2;
      this->label8->Text = L"Standard-Namespace";
      // 
      // groupBoxNamespaces
      // 
      this->groupBoxNamespaces->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->groupBoxNamespaces->Controls->Add(this->label8);
      this->groupBoxNamespaces->Controls->Add(this->listBoxNamespaces);
      this->groupBoxNamespaces->Controls->Add(this->textBoxDefaultNamespace);
      this->groupBoxNamespaces->Location = System::Drawing::Point(13, 160);
      this->groupBoxNamespaces->Name = L"groupBoxNamespaces";
      this->groupBoxNamespaces->Size = System::Drawing::Size(484, 151);
      this->groupBoxNamespaces->TabIndex = 10;
      this->groupBoxNamespaces->TabStop = false;
      this->groupBoxNamespaces->Text = L"Namespaces";
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
      this->buttonBeenden->Location = System::Drawing::Point(220, 661);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 12;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &ModellStatistik::buttonBeenden_Click);
      // 
      // label9
      // 
      this->label9->AutoSize = true;
      this->label9->Location = System::Drawing::Point(16, 316);
      this->label9->Name = L"label9";
      this->label9->Size = System::Drawing::Size(69, 13);
      this->label9->TabIndex = 13;
      this->label9->Text = L"Namespaces";
      // 
      // ModellStatistik
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(509, 696);
      this->Controls->Add(this->label9);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->groupBoxFeatureStatistik);
      this->Controls->Add(this->label5);
      this->Controls->Add(this->textBoxSrsDimension);
      this->Controls->Add(this->label4);
      this->Controls->Add(this->textBoxSrsName);
      this->Controls->Add(this->label3);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->textBoxBBMax);
      this->Controls->Add(this->textBoxBBMin);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->textBoxDateiName);
      this->Controls->Add(this->groupBoxNamespaces);
      this->Name = L"ModellStatistik";
      this->Text = L"ModellStatistik";
      this->groupBoxFeatureStatistik->ResumeLayout(false);
      this->groupBoxFeatureStatistik->PerformLayout();
      this->groupBoxNamespaces->ResumeLayout(false);
      this->groupBoxNamespaces->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion


///////////////////////////////////////////////////////////////////////////////
//  Baut die Anzeige auf                                                     //
///////////////////////////////////////////////////////////////////////////////
private: System::Void initialize()
{
  double        bbMin[3], bbMax[3];
  bool          success;
  String      ^ STR;
  unsigned int  i, j;
  int           anzRaumbez, anzNRaumbez;

  System::Windows::Forms::TreeNode  ^ pTreeNodeKlasse;
  System::Windows::Forms::TreeNode  ^ pTreeNodeAttribut;

  success = pModelStatistics->getBoundingBox( bbMin, bbMax );
  if ( success )
  {
    STR = String::Concat( Convert::ToString( bbMin[0] ), ", ", Convert::ToString( bbMin[1] ), ", ", Convert::ToString( bbMin[2] ) );
    textBoxBBMin->Text = STR;
    STR = String::Concat( Convert::ToString( bbMax[0] ), ", ", Convert::ToString( bbMax[1] ), ", ", Convert::ToString( bbMax[2] ) );
    textBoxBBMax->Text = STR;
  }
  else
    textBoxBBMin->Text = "Es kann keine Bounding Box berechnet werden";

  textBoxSrsName->Text      = QuConvert::ToString( pModelStatistics->getSrsName() );
  textBoxSrsDimension->Text = Convert::ToString( pModelStatistics->getSrsDimension() );

  textBoxDefaultNamespace->Text = QuConvert::ToString( pModelStatistics->getDefaultNamespace() );
  for ( i = 0; i < pModelStatistics->getAnzNamespaces(); i++ )
  {
    string namespaceName = pModelStatistics->getNamespace( i );
    string namespaceKuerzel = pModelStatistics->getNamespaceKuerzel( i );

    String ^STR = String::Concat( QuConvert::ToString( namespaceName ), " (",  QuConvert::ToString( namespaceKuerzel ), ")" );
    listBoxNamespaces->Items->Add( STR );
  }

  pModelStatistics->getFeatureNumbers( anzRaumbez, anzNRaumbez );
  textBoxFeatureAnzahl->Text     =  Convert::ToString( anzRaumbez + anzNRaumbez );
  textBoxAnzahlRaumbezogen->Text =  Convert::ToString( anzRaumbez );

  for ( i = 0; i < pModelStatistics->getClassAnz(); i++ )
  {
    GmlClassStatistics * pKlassenStatistik = pModelStatistics->getClass(  i  );

    string klassenName = pKlassenStatistik->getKlassenName();
    int    anzKlassen  = (int)pKlassenStatistik->getAnzahl();

    STR = String::Concat ( QuConvert::ToString( klassenName ), " (", Convert::ToString( anzKlassen ), ")" );

    pTreeNodeKlasse = gcnew System::Windows::Forms::TreeNode ( STR );
    treeViewKlassenStatistik->Nodes->Add( pTreeNodeKlasse );

    for ( j = 0; j < pKlassenStatistik->getAttributeAnz(); j++ )
    {
      GmlAttributeStatistics * pAttributStatistik = pKlassenStatistik->getAttribute( j );

      string attributName = pAttributStatistik->getAttributName();
      int    anzObjekte   = pAttributStatistik->getAnzObjekte();
      int    anzVerwendet = pAttributStatistik->getAnzVerwendungen();

      STR = String::Concat( QuConvert::ToString( attributName ), ": ", Convert::ToString( anzVerwendet ), " mal verwendet in ",
                                Convert::ToString( anzObjekte ) );

      if ( anzObjekte == 1 )
        STR = String::Concat(  STR, " Objekt" );
      else
        STR = String::Concat(  STR, " Objekten" );

      pTreeNodeAttribut = gcnew System::Windows::Forms::TreeNode ( STR );
      pTreeNodeKlasse->Nodes->Add( pTreeNodeAttribut );
    }   
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button "Beenden"                                          //
//  Schließt den Dialog                                                      //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e)
         {
           Close();
         }
};

