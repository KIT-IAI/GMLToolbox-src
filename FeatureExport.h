#pragma once

#include "SrsManagerRef.h"
#include "Feature.h"
#include "Convert.h"

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	/// <summary>
	/// Export von 1 oder 2 Features bzw. der im Feature-Baum übergeordneten Features
	/// </summary>
	public ref class FeatureExport : public System::Windows::Forms::Form
	{
	public:
		FeatureExport( Feature * pFeatureP, Feature * pFeatureRelP )
		{
      vector<Feature*> vParents;
			size_t           i, anz;
      string           klassenName;

			InitializeComponent();

      parentArray = gcnew ArrayList;
 
      pFeatures           = pFeatureP->getFeatures();
      pFeature            = pFeatureP;
      pFeatureRel         = pFeatureRelP;
      pFeatureSelected    = NULL;
      pFeatureRelSelected = NULL;

      vParents.push_back ( pFeature );
      anz = pFeature->getParents( vParents );
      for ( i = 0; i < anz; i++ )
      {
        Feature * pFeatureLoc = vParents[i];
        string gmlId = pFeatureLoc->getGmlId();

        parentArray->Add ( QuConvert::ToString( gmlId ) );;
        klassenName = pFeatureLoc->getKlassenName();
        listBoxParents->Items->Add( QuConvert::ToString( klassenName ) );
      }
      listBoxParents->SelectedIndex = 0;		

      if ( pFeatureRel != NULL )
      {
        parentRelArray = gcnew ArrayList;

        vParents.clear();
        vParents.push_back ( pFeatureRel );
        anz = pFeatureRel->getParents( vParents );
        for ( i = 0; i < anz; i++ )
        {
          Feature * pFeatureLoc = vParents[i];
          string gmlId = pFeatureLoc->getGmlId();

          parentRelArray->Add ( QuConvert::ToString( gmlId ) );;
          klassenName = pFeatureLoc->getKlassenName();
          listBoxParentsRel->Items->Add( QuConvert::ToString( klassenName ) );
        }
        listBoxParentsRel->SelectedIndex = 0;		
      }
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~FeatureExport()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::ListBox^  listBoxParents;
  protected: 
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::ListBox^  listBoxEigenschaften;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Button^  buttonAbbrechen;
  private: System::Windows::Forms::Button^  buttonUebernehmen;
  private: System::Windows::Forms::ListBox^  listBoxParentsRel;
  private: System::Windows::Forms::ListBox^  listBoxEigenschaftenRel;

	private:
    Features   * pFeatures;           // Aggregation der Features
    Feature    * pFeature;            // 1.Feature
    Feature    * pFeatureRel;         // 2. Feature 
    Feature    * pFeatureSelected;    // 1. selektiertes Feature 
    Feature    * pFeatureRelSelected; // 2. selektiertes Feature
    ArrayList  ^ parentArray;         // Liste der übergeordneten Features von Feaure 1
    ArrayList  ^ parentRelArray;      // Liste der übergeordneten Features von Feature 2
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
      this->listBoxParents = (gcnew System::Windows::Forms::ListBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->listBoxEigenschaften = (gcnew System::Windows::Forms::ListBox());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
      this->buttonUebernehmen = (gcnew System::Windows::Forms::Button());
      this->listBoxParentsRel = (gcnew System::Windows::Forms::ListBox());
      this->listBoxEigenschaftenRel = (gcnew System::Windows::Forms::ListBox());
      this->SuspendLayout();
      // 
      // listBoxParents
      // 
      this->listBoxParents->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left));
      this->listBoxParents->FormattingEnabled = true;
      this->listBoxParents->Location = System::Drawing::Point(12, 12);
      this->listBoxParents->Name = L"listBoxParents";
      this->listBoxParents->Size = System::Drawing::Size(178, 82);
      this->listBoxParents->TabIndex = 0;
      this->listBoxParents->SelectedIndexChanged += gcnew System::EventHandler(this, &FeatureExport::listBoxParents_SelectedIndexChanged);
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(14, 196);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(107, 13);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Vererbungshierarchie";
      // 
      // listBoxEigenschaften
      // 
      this->listBoxEigenschaften->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->listBoxEigenschaften->FormattingEnabled = true;
      this->listBoxEigenschaften->Location = System::Drawing::Point(211, 13);
      this->listBoxEigenschaften->Name = L"listBoxEigenschaften";
      this->listBoxEigenschaften->Size = System::Drawing::Size(203, 82);
      this->listBoxEigenschaften->TabIndex = 2;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(213, 199);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(75, 13);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Eigenschaften";
      // 
      // buttonAbbrechen
      // 
      this->buttonAbbrechen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->buttonAbbrechen->Location = System::Drawing::Point(12, 227);
      this->buttonAbbrechen->Name = L"buttonAbbrechen";
      this->buttonAbbrechen->Size = System::Drawing::Size(75, 23);
      this->buttonAbbrechen->TabIndex = 4;
      this->buttonAbbrechen->Text = L"Abbrechen";
      this->buttonAbbrechen->UseVisualStyleBackColor = true;
      this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &FeatureExport::buttonAbbrechen_Click);
      // 
      // buttonUebernehmen
      // 
      this->buttonUebernehmen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->buttonUebernehmen->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->buttonUebernehmen->Location = System::Drawing::Point(333, 227);
      this->buttonUebernehmen->Name = L"buttonUebernehmen";
      this->buttonUebernehmen->Size = System::Drawing::Size(80, 23);
      this->buttonUebernehmen->TabIndex = 5;
      this->buttonUebernehmen->Text = L"Übernehmen";
      this->buttonUebernehmen->UseVisualStyleBackColor = true;
      this->buttonUebernehmen->Click += gcnew System::EventHandler(this, &FeatureExport::buttonUebernehmen_Click);
      // 
      // listBoxParentsRel
      // 
      this->listBoxParentsRel->FormattingEnabled = true;
      this->listBoxParentsRel->Location = System::Drawing::Point(15, 110);
      this->listBoxParentsRel->Name = L"listBoxParentsRel";
      this->listBoxParentsRel->Size = System::Drawing::Size(178, 82);
      this->listBoxParentsRel->TabIndex = 6;
      this->listBoxParentsRel->SelectedIndexChanged += gcnew System::EventHandler(this, &FeatureExport::listBoxParentsRel_SelectedIndexChanged);
      // 
      // listBoxEigenschaftenRel
      // 
      this->listBoxEigenschaftenRel->FormattingEnabled = true;
      this->listBoxEigenschaftenRel->Location = System::Drawing::Point(214, 110);
      this->listBoxEigenschaftenRel->Name = L"listBoxEigenschaftenRel";
      this->listBoxEigenschaftenRel->Size = System::Drawing::Size(199, 82);
      this->listBoxEigenschaftenRel->TabIndex = 7;
      // 
      // FeatureExport
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(426, 262);
      this->Controls->Add(this->listBoxEigenschaftenRel);
      this->Controls->Add(this->listBoxParentsRel);
      this->Controls->Add(this->buttonUebernehmen);
      this->Controls->Add(this->buttonAbbrechen);
      this->Controls->Add(this->label2);
      this->Controls->Add(this->listBoxEigenschaften);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->listBoxParents);
      this->Name = L"FeatureExport";
      this->Text = L"Feature Export";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl Feature 1 oder übergeordnetes Feature            //
///////////////////////////////////////////////////////////////////////////////
 private: System::Void listBoxParents_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
           {
             string   gmlId;
             String ^ STR;

             String ^ GML_ID = dynamic_cast<String^> ( parentArray[listBoxParents->SelectedIndex] );
             QuConvert::systemStr2stdStr( gmlId, GML_ID );
             pFeatureSelected = pFeatures->getFeature( gmlId );

             listBoxEigenschaften->Items->Clear();
             STR = String::Concat ( "Id: ", GML_ID );
             listBoxEigenschaften->Items->Add( STR );
           }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Auswahl Feature 2 oder übergeordnetes Feature            //
///////////////////////////////////////////////////////////////////////////////
private: System::Void listBoxParentsRel_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
         {
           string   gmlId;
           String ^ STR;

           String ^ GML_ID = dynamic_cast<String^> ( parentRelArray[listBoxParents->SelectedIndex] );
           QuConvert::systemStr2stdStr( gmlId, GML_ID );
           pFeatureRelSelected = pFeatures->getFeature( gmlId );

           listBoxEigenschaftenRel->Items->Clear();
           STR = String::Concat ( "Id: ", GML_ID );
           listBoxEigenschaftenRel->Items->Add( STR );
         }


///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Button Abbrechen                                         //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAbbrechen_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           pFeatureSelected = NULL;
           Close();
         }

///////////////////////////////////////////////////////////////////////////////
//  Commandhandler: Button Übernehmen                                        //
///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonUebernehmen_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           Close();
         }

///////////////////////////////////////////////////////////////////////////////
//  Interface: Liefert das 1 bzw. 2 selektierte Feature                      //
///////////////////////////////////////////////////////////////////////////////
public: Feature * getFeatureSelected ( )
        {
          return pFeatureSelected;
        }


public: Feature * getFeatureRelSelected ( )
        {
          return pFeatureRelSelected;
        }

};
