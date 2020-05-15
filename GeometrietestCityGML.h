#pragma once

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Festlegung des Pr�fungsumfangs eines CityGML-Modells
	/// </summary>
	public ref class GeometrietestCityGML : public System::Windows::Forms::Form
	{
	public:
		GeometrietestCityGML( bool checkBoundarySurfacesP, bool checkOuterShellP )
		{
			InitializeComponent();

      checkBoundarySurfaces  = checkBoundarySurfacesP;
      checkOuterShell        = checkOuterShellP;

      checkBoxTopologyBoundarySurfaces->Checked = checkBoundarySurfaces;
      checkBoxTopologyExteriorShell->Checked    = checkOuterShell;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~GeometrietestCityGML()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::CheckBox^  checkBoxTopologyBoundarySurfaces;
  protected: 
  private: System::Windows::Forms::CheckBox^  checkBoxTopologyExteriorShell;


  private: System::Windows::Forms::Button^  buttonBeenden;


  public:
    bool checkBoundarySurfaces;  // �berpr�fung der Topologie von Boundary Surfaces
    bool checkOuterShell;        // �berpr�fung der Geb�ude-Au�enh�lle

	private:
		/// <summary>
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode f�r die Designerunterst�tzung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor ge�ndert werden.
		/// </summary>
		void InitializeComponent(void)
		{
      this->checkBoxTopologyBoundarySurfaces = (gcnew System::Windows::Forms::CheckBox());
      this->checkBoxTopologyExteriorShell = (gcnew System::Windows::Forms::CheckBox());
      this->buttonBeenden = (gcnew System::Windows::Forms::Button());
      this->SuspendLayout();
      // 
      // checkBoxTopologyBoundarySurfaces
      // 
      this->checkBoxTopologyBoundarySurfaces->AutoSize = true;
      this->checkBoxTopologyBoundarySurfaces->Location = System::Drawing::Point(13, 29);
      this->checkBoxTopologyBoundarySurfaces->Name = L"checkBoxTopologyBoundarySurfaces";
      this->checkBoxTopologyBoundarySurfaces->Size = System::Drawing::Size(184, 17);
      this->checkBoxTopologyBoundarySurfaces->TabIndex = 0;
      this->checkBoxTopologyBoundarySurfaces->Text = L"Topologie einzelner Grenzfl�chen";
      this->checkBoxTopologyBoundarySurfaces->UseVisualStyleBackColor = true;
      // 
      // checkBoxTopologyExteriorShell
      // 
      this->checkBoxTopologyExteriorShell->AutoSize = true;
      this->checkBoxTopologyExteriorShell->Location = System::Drawing::Point(13, 53);
      this->checkBoxTopologyExteriorShell->Name = L"checkBoxTopologyExteriorShell";
      this->checkBoxTopologyExteriorShell->Size = System::Drawing::Size(182, 17);
      this->checkBoxTopologyExteriorShell->TabIndex = 1;
      this->checkBoxTopologyExteriorShell->Text = L"Topologie Grenzfl�chen-Verbund";
      this->checkBoxTopologyExteriorShell->UseVisualStyleBackColor = true;
      // 
      // buttonBeenden
      // 
      this->buttonBeenden->Location = System::Drawing::Point(202, 96);
      this->buttonBeenden->Name = L"buttonBeenden";
      this->buttonBeenden->Size = System::Drawing::Size(75, 23);
      this->buttonBeenden->TabIndex = 4;
      this->buttonBeenden->Text = L"Beenden";
      this->buttonBeenden->UseVisualStyleBackColor = true;
      this->buttonBeenden->Click += gcnew System::EventHandler(this, &GeometrietestCityGML::buttonBeenden_Click);
      // 
      // GeometrietestCityGML
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(289, 138);
      this->Controls->Add(this->buttonBeenden);
      this->Controls->Add(this->checkBoxTopologyExteriorShell);
      this->Controls->Add(this->checkBoxTopologyBoundarySurfaces);
      this->Name = L"GeometrietestCityGML";
      this->Text = L"Geometrietest CityGML";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
///////////////////////////////////////////////////////////////////////////////
//  Commandhandler Button Beenden                                            //
//  �bernimmt die spezifizierten Daten                                       //
///////////////////////////////////////////////////////////////////////////////
  private: System::Void buttonBeenden_Click(System::Object^  sender, System::EventArgs^  e) 
           {
             checkBoundarySurfaces  = checkBoxTopologyBoundarySurfaces->Checked;
             checkOuterShell        = checkBoxTopologyExteriorShell->Checked;

             Close();
           }

 
};
