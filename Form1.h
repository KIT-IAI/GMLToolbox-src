#pragma once

#include <stdio.h>
#include <conio.h>

#include "Info.h"
#include "Options1.h"
#include "GmlToolbox.h"
#include "PluginLoader.h"
#include "GMLToolboxPlugin.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Xml;
using namespace System::Xml::Schema;

using namespace std;

//	using namespace System::Drawing;

/// <summary> 
/// Zentrale Benutzerschnittstelle
///
/// </summary>
public ref class Form1 : public System::Windows::Forms::Form, public KIT::GMLToolbox::Plugin::IGMLToolboxHost
{
public:

private: System::Windows::Forms::Button ^  ButtonKonversion;
private: System::Windows::Forms::SaveFileDialog ^  saveConvDateiDialog;
private: System::Windows::Forms::MenuItem ^  menuItem11;
private: System::Windows::Forms::MenuItem ^  menuItem14;
private: System::Windows::Forms::PictureBox ^  pictureBox2;
private: System::Windows::Forms::TextBox ^  textBoxPlanTyp;
private: System::Windows::Forms::MenuItem ^  menuItem5;
private: System::Windows::Forms::Button ^  buttonShowSvg;
private: System::Windows::Forms::MenuItem ^  menuItem6;
private: System::Windows::Forms::ComboBox^  comboBoxMassstab;
private: System::Windows::Forms::Label ^  label8;

private: System::Windows::Forms::MenuItem ^  menuItem23;
private: System::Windows::Forms::MenuItem ^  menuItem24;
private: System::Windows::Forms::MenuItem ^  menuItem25;
private: System::Windows::Forms::MenuItem ^  menuItem26;
private: System::Windows::Forms::TextBox ^  textBoxXPlanGMLVersion;
private: System::Windows::Forms::Button ^  buttonUeberpruefen;
private: System::Xml::Schema::XmlSchemaSet ^ schema;
private: System::Windows::Forms::MenuItem ^  menuItem32;
private: System::Windows::Forms::ComboBox ^  comboBoxDarstellung;
private: System::Windows::Forms::CheckBox ^  checkBoxAllePlaene;
private: System::Windows::Forms::TextBox ^  konvDateiName;
private: System::Windows::Forms::Button ^  buttonKonvDatei;
private: System::Windows::Forms::Label ^  label1;
private: System::Windows::Forms::TextBox ^  shapeDateiNamen;
private: System::Windows::Forms::Button ^  buttonShapeDateien;
private: System::Windows::Forms::Label ^  label2;
private: System::Windows::Forms::TextBox^  ausgabeDateiName;
private: System::Windows::Forms::Button ^  buttonAusgabeFolder;
private: System::Windows::Forms::Button ^  buttonAbbrechen;
private: System::Windows::Forms::TextBox ^  meldungen;
private: System::Windows::Forms::OpenFileDialog ^  shapeFileDialog;
private: System::Windows::Forms::Label ^  label3;
private: System::Windows::Forms::Label ^  label4;
private: System::Windows::Forms::Label ^  label5;
private: System::Windows::Forms::Label ^  label6;
private: System::Windows::Forms::TextBox ^  shapeFileAnz;
private: System::Windows::Forms::TextBox ^  objektReadAnz;
private: System::Windows::Forms::TextBox ^  objektNotReadAnz;
private: System::Windows::Forms::TextBox ^  shapeFileNotReadAnz;
private: System::Windows::Forms::Label ^  label7;
private: System::Windows::Forms::SaveFileDialog ^  ausgabeDateiDialog;
private: System::Windows::Forms::GroupBox ^  groupBox1;
private: System::Windows::Forms::GroupBox ^  groupBox2;
private: System::Windows::Forms::GroupBox ^  groupBox3;
private: System::Windows::Forms::TextBox ^  BPlanGmldatei;
private: System::Windows::Forms::Button ^  buttonBPlanGMLDatei;
private: System::Windows::Forms::Label ^  label9;
private: System::Windows::Forms::GroupBox ^  groupBox4;
private: System::Windows::Forms::OpenFileDialog ^  BPlanGmlDialog;
private: System::Windows::Forms::SaveFileDialog ^  svgDateiDialog;
private: System::Windows::Forms::ComboBox ^  comboBPlan;
private: System::Windows::Forms::Label ^  label14;
private: System::Windows::Forms::Label ^  label15;
private: System::Windows::Forms::ComboBox ^  comboBPlanEbene;
private: System::Windows::Forms::MainMenu ^  mainMenu1;
private: System::Windows::Forms::MenuItem ^  menuItem1;
private: System::Windows::Forms::MenuItem ^  menuItem9;
private: System::Windows::Forms::Button^  buttonAttributeBearbeiten;
private: System::Windows::Forms::MenuItem^  menuItemCodelistOrdner;
private: System::Windows::Forms::MenuItem^  menuItem7;
private: System::Windows::Forms::MenuItem^  menuItem10;
private: System::Windows::Forms::MenuItem^  menuItem20;
private: System::Windows::Forms::MenuItem^  menuItemShapefilesSpezifizieren;
private: System::Windows::Forms::MenuItem^  menuItem30;
private: System::Windows::Forms::MenuItem^  menuItemKonversionErzeugen;
private: System::Windows::Forms::MenuItem^  menuItemKonversionOeffnen;
private: System::Windows::Forms::MenuItem^  menuItemKonversionBearbeiten;
private: System::Windows::Forms::MenuItem^  menuItemKonversionSchliessen;
private: System::Windows::Forms::MenuItem^  menuItem37;
private: System::Windows::Forms::MenuItem^  menuItemKonversionSpeichern;
private: System::Windows::Forms::MenuItem^  menuItemKonversionSpeichernUnter;
private: System::Windows::Forms::MenuItem^  menuItem40;
private: System::Windows::Forms::MenuItem^  menuItemShapefilesKonvertieren;
private: System::Windows::Forms::MenuItem^  menuItemGmlLesen;
private: System::Windows::Forms::MenuItem^  menuItem3;
private: System::Windows::Forms::MenuItem^  menuItemGmlSpeichern;
private: System::Windows::Forms::MenuItem^  menuItemGmlSpeichernUnter;
private: System::Windows::Forms::MenuItem^  menuItem21;
private: System::Windows::Forms::MenuItem^  menuItemLogfile;
private: System::Windows::Forms::MenuItem^  menuItem41;
private: System::Windows::Forms::MenuItem^  menuItemBeenden;
private: System::Windows::Forms::MenuItem^  menuItemAttributeBearbeiten;
private: System::Windows::Forms::MenuItem^  menuItemShowSvg;
private: System::Windows::Forms::MenuItem^  menuItemShowKml;
private: System::Windows::Forms::Button^  buttonKmlGenerieren;
private: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::MenuItem^  menuItem2;
private: System::Windows::Forms::MenuItem^  menuItemPruefenXPlanGMLKonformitaet;
private: System::Windows::Forms::MenuItem^  menuItemGenerierenINSPIREPLU;
private: System::Windows::Forms::MenuItem^  menuItem4;
private: System::Windows::Forms::MenuItem^  menuItemMigrationXPlanGML3_0;




private: System::Windows::Forms::MenuItem^  menuItem17;
private: System::Windows::Forms::CheckBox^  checkBoxAdd;


private: System::Windows::Forms::PictureBox^  pictureBox1;


private: System::Windows::Forms::MenuItem^  menuItem34;
private: System::Windows::Forms::MenuItem^  menuItem35;
private: System::Windows::Forms::MenuItem^  menuItem325833To25833;
private: System::Windows::Forms::MenuItem^  menuItem29;
private: System::Windows::Forms::ComboBox^  comboBox3DViewer;
private: System::Windows::Forms::Label^  label11;



private: System::Windows::Forms::MenuItem^  menuItem48;
private: System::Windows::Forms::MenuItem^  menuItem55;
private: System::Windows::Forms::MenuItem^  menuItem56;



private: System::Windows::Forms::MenuItem^  menuItemShowCityGML;
private: System::Windows::Forms::MenuItem^  menuItem15;
private: System::Windows::Forms::MenuItem^  menuItem33;
private: System::Windows::Forms::MenuItem^  menuItem38;
private: System::Windows::Forms::MenuItem^  menuItem50;
private: System::Windows::Forms::MenuItem^  menuItem65;
private: System::Windows::Forms::MenuItem^  menuItem66;
private: System::Windows::Forms::MenuItem^  menuItem67;
private: System::Windows::Forms::MenuItem^  menuItem68;
private: System::Windows::Forms::MenuItem^  menuItem27;
private: System::Windows::Forms::MenuItem^  menuItem69;

private: System::Windows::Forms::MenuItem^  menuItem36;
private: System::Windows::Forms::MenuItem^  menuItem39;
private: System::Windows::Forms::MenuItem^  menuItem42;
private: System::Windows::Forms::GroupBox ^  groupBox5;

public:
  Form1(cli::array<System::String ^> ^ commandLineAttributesP)
  {
    string systemFolder;
    string folder;
    string referenzsystemListeDatei;
    string constructionListDatei;
    string defaultConstructionDatei;
    string nutzungsprofileDatei;

    InitializeComponent();

    auto plugins = KIT::GMLToolbox::Plugin::PluginLoader::LoadPlugins();
    for each(auto plugin in plugins)
    {
      plugin->RegisterPlugin(this);
    }

    pBLeitplanViewer = new GmlToolbox(9999999);
    SystemFolder = ToString(pBLeitplanViewer->getSystemFolder());

    this->Text = "GML Toolbox Freeware";

    conversionSaved = true;

    checkTransformModule = true;

    ToolboxVersion = "11.1";

    menuItemGmlSpeichern->Enabled = false;
    menuItemGmlSpeichernUnter->Enabled = false;
    buttonKmlGenerieren->Enabled = false;
    buttonAusgabeFolder->Enabled = false;
    buttonShowSvg->Enabled = false;

    buttonUeberpruefen->Enabled = false;
    menuItemPruefenXPlanGMLKonformitaet->Enabled = false;


    menuItemAttributeBearbeiten->Enabled = false;
    buttonAttributeBearbeiten->Enabled = false;

    menuItemKonversionErzeugen->Enabled = false;
    menuItemKonversionBearbeiten->Enabled = false;
    menuItemKonversionSchliessen->Enabled = false;
    menuItemKonversionSpeichern->Enabled = false;
    menuItemKonversionSpeichernUnter->Enabled = false;
    menuItemShapefilesKonvertieren->Enabled = false;
    ButtonKonversion->Enabled = false;

    menuItemMigrationXPlanGML3_0->Enabled = true;
    menuItemShowKml->Enabled = false;
    menuItemShowSvg->Enabled = false;
    menuItemShowCityGML->Enabled = false;

    comboBoxMassstab->SelectedIndex = 2;
    pBLeitplanViewer->setGlobalScale(1.0);
    comboBoxDarstellung->SelectedItem = "Interaktive Karte";

    setDaten(0, 0, 0, 0);

    GmlDateiName = nullptr;
    FehlerStylesheet = nullptr;
    MigrationStylesheet = nullptr;

    m_iexplorerProcess = nullptr;
    m_visualization3DProcess = nullptr;

    readIniFile();

    if (pBLeitplanViewer->getKMLViewer() == "" &&  pBLeitplanViewer->getGMLViewer() == "")
      visualization3D = false;
    else
      visualization3D = true;

    visualization2D = true;

    if (checkTransformModule)
      pBLeitplanViewer->checkCoordTransformationModule();
  }


public:
  virtual void AddMenuItem(String^ menuText,
    bool bAddSeparator,
    KIT::GMLToolbox::Plugin::IGMLToolboxHost::MenuPos ePos,
    EventHandler^ onClick)
  {
    auto menu = menuItem29;

    if (bAddSeparator)
    {
      auto menuSeparator = gcnew MenuItem();
      menuSeparator->Text = "-";
      menu->MenuItems->Add(menuSeparator);
    }

    auto menuItem = gcnew MenuItem();
    menuItem->Text = menuText;
    menuItem->Click += onClick;
    menu->MenuItems->Add(menuItem);
  }


public:
  void setDaten(int anzShapeFileReadP, int anzShapeFileNotReadP, int anzObjReadP, int anzObjNotReadP)
  {
    shapeFileAnz->Text = Convert::ToString(anzShapeFileReadP);
    shapeFileNotReadAnz->Text = Convert::ToString(anzShapeFileNotReadP);
    objektReadAnz->Text = Convert::ToString(anzObjReadP);
    objektNotReadAnz->Text = Convert::ToString(anzObjNotReadP);
  }

private: System::ComponentModel::IContainer^  components;

private:
  /// <summary>
  /// Erforderliche Designervariable.
  /// </summary>

  GmlToolbox      * pBLeitplanViewer;   //  Zentrales Software-Modul, über das alle anderen Module erreichbar sind

  bool conversionSaved;             // Zeig an, ob eine neu generierte oder geänderte Konversion Shapefile-->XPlanGML schon gesichert wurde
  bool checkTransformModule;        // Steuert, ob die Funktionsfähigkeit des Koordinatentransformations-Moduls getestet werden soll
  bool visualization2D;             // Zeigt an, ob eine 2D-Visualisierung mit SVG möglich ist
  bool visualization3D;             // Zeigt an, ob eine 3D-Visualisierung möglich ist

  String ^ ToolboxVersion;          // Aktuelle Version der GML-Toolbox
  String ^ SystemFolder;            // System-Ordner der GML-Toolbox
  String ^ GmlDateiName;            // Pfadname der aktuell eingelesenen GML-Datei
  String ^ FehlerStylesheet;        // XSLT-Styleshet zur Darstellung von Konformitätsfehlern
  String ^ MigrationStylesheet;     // XSLT-Stylesheet zur Darstellung von Fehlern bei der XPlanGML-Migration

  System::Diagnostics::Process ^ m_iexplorerProcess;        // Prozess zum Starten des HTML-Browsers
  System::Diagnostics::Process ^ m_visualization3DProcess;  // Prozess zum Starten des KML-Viewers

  /// <summary>
  /// Erforderliche Methode für die Designerunterstützung. 
  /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
  /// </summary>
  void InitializeComponent(void)
  {
    this->components = (gcnew System::ComponentModel::Container());
    System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
    this->konvDateiName = (gcnew System::Windows::Forms::TextBox());
    this->buttonKonvDatei = (gcnew System::Windows::Forms::Button());
    this->label1 = (gcnew System::Windows::Forms::Label());
    this->shapeDateiNamen = (gcnew System::Windows::Forms::TextBox());
    this->buttonShapeDateien = (gcnew System::Windows::Forms::Button());
    this->label2 = (gcnew System::Windows::Forms::Label());
    this->ausgabeDateiName = (gcnew System::Windows::Forms::TextBox());
    this->buttonAusgabeFolder = (gcnew System::Windows::Forms::Button());
    this->buttonAbbrechen = (gcnew System::Windows::Forms::Button());
    this->meldungen = (gcnew System::Windows::Forms::TextBox());
    this->label3 = (gcnew System::Windows::Forms::Label());
    this->shapeFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
    this->label4 = (gcnew System::Windows::Forms::Label());
    this->label5 = (gcnew System::Windows::Forms::Label());
    this->label6 = (gcnew System::Windows::Forms::Label());
    this->shapeFileAnz = (gcnew System::Windows::Forms::TextBox());
    this->objektReadAnz = (gcnew System::Windows::Forms::TextBox());
    this->objektNotReadAnz = (gcnew System::Windows::Forms::TextBox());
    this->shapeFileNotReadAnz = (gcnew System::Windows::Forms::TextBox());
    this->label7 = (gcnew System::Windows::Forms::Label());
    this->ausgabeDateiDialog = (gcnew System::Windows::Forms::SaveFileDialog());
    this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
    this->ButtonKonversion = (gcnew System::Windows::Forms::Button());
    this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
    this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
    this->checkBoxAdd = (gcnew System::Windows::Forms::CheckBox());
    this->buttonUeberpruefen = (gcnew System::Windows::Forms::Button());
    this->label9 = (gcnew System::Windows::Forms::Label());
    this->buttonBPlanGMLDatei = (gcnew System::Windows::Forms::Button());
    this->BPlanGmldatei = (gcnew System::Windows::Forms::TextBox());
    this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
    this->label11 = (gcnew System::Windows::Forms::Label());
    this->comboBox3DViewer = (gcnew System::Windows::Forms::ComboBox());
    this->buttonKmlGenerieren = (gcnew System::Windows::Forms::Button());
    this->label10 = (gcnew System::Windows::Forms::Label());
    this->comboBoxDarstellung = (gcnew System::Windows::Forms::ComboBox());
    this->label8 = (gcnew System::Windows::Forms::Label());
    this->comboBoxMassstab = (gcnew System::Windows::Forms::ComboBox());
    this->buttonShowSvg = (gcnew System::Windows::Forms::Button());
    this->buttonAttributeBearbeiten = (gcnew System::Windows::Forms::Button());
    this->label15 = (gcnew System::Windows::Forms::Label());
    this->label14 = (gcnew System::Windows::Forms::Label());
    this->comboBPlanEbene = (gcnew System::Windows::Forms::ComboBox());
    this->comboBPlan = (gcnew System::Windows::Forms::ComboBox());
    this->BPlanGmlDialog = (gcnew System::Windows::Forms::OpenFileDialog());
    this->svgDateiDialog = (gcnew System::Windows::Forms::SaveFileDialog());
    this->mainMenu1 = (gcnew System::Windows::Forms::MainMenu(this->components));
    this->menuItem7 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemGmlLesen = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem3 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemGmlSpeichern = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemGmlSpeichernUnter = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem21 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem34 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem325833To25833 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem35 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemLogfile = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem41 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemBeenden = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem10 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemAttributeBearbeiten = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem1 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem20 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemShapefilesSpezifizieren = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem30 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemKonversionErzeugen = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemKonversionOeffnen = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemKonversionBearbeiten = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemKonversionSchliessen = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem37 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemKonversionSpeichern = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemKonversionSpeichernUnter = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem40 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemShapefilesKonvertieren = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemGenerierenINSPIREPLU = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem55 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem56 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem4 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemMigrationXPlanGML3_0 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem67 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem68 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem48 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem15 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem33 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem38 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem50 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem65 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem66 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem5 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemCodelistOrdner = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem9 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemShowSvg = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemShowKml = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemShowCityGML = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem2 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItemPruefenXPlanGMLKonformitaet = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem29 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem6 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem23 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem24 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem25 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem26 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem32 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem17 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem11 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem27 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem69 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem14 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem36 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem39 = (gcnew System::Windows::Forms::MenuItem());
    this->menuItem42 = (gcnew System::Windows::Forms::MenuItem());
    this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
    this->textBoxXPlanGMLVersion = (gcnew System::Windows::Forms::TextBox());
    this->textBoxPlanTyp = (gcnew System::Windows::Forms::TextBox());
    this->saveConvDateiDialog = (gcnew System::Windows::Forms::SaveFileDialog());
    this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
    this->checkBoxAllePlaene = (gcnew System::Windows::Forms::CheckBox());
    this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
    this->groupBox1->SuspendLayout();
    this->groupBox2->SuspendLayout();
    this->groupBox3->SuspendLayout();
    this->groupBox4->SuspendLayout();
    this->groupBox5->SuspendLayout();
    (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
    (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
    this->SuspendLayout();
    // 
    // konvDateiName
    // 
    this->konvDateiName->BackColor = System::Drawing::SystemColors::Control;
    this->konvDateiName->ForeColor = System::Drawing::Color::Red;
    resources->ApplyResources(this->konvDateiName, L"konvDateiName");
    this->konvDateiName->Name = L"konvDateiName";
    this->konvDateiName->ReadOnly = true;
    // 
    // buttonKonvDatei
    // 
    resources->ApplyResources(this->buttonKonvDatei, L"buttonKonvDatei");
    this->buttonKonvDatei->Name = L"buttonKonvDatei";
    this->buttonKonvDatei->Click += gcnew System::EventHandler(this, &Form1::buttonKonvDatei_Click);
    // 
    // label1
    // 
    resources->ApplyResources(this->label1, L"label1");
    this->label1->Name = L"label1";
    // 
    // shapeDateiNamen
    // 
    this->shapeDateiNamen->ForeColor = System::Drawing::Color::Red;
    resources->ApplyResources(this->shapeDateiNamen, L"shapeDateiNamen");
    this->shapeDateiNamen->Name = L"shapeDateiNamen";
    this->shapeDateiNamen->ReadOnly = true;
    // 
    // buttonShapeDateien
    // 
    resources->ApplyResources(this->buttonShapeDateien, L"buttonShapeDateien");
    this->buttonShapeDateien->Name = L"buttonShapeDateien";
    this->buttonShapeDateien->Click += gcnew System::EventHandler(this, &Form1::buttonShapeDateien_Click);
    // 
    // label2
    // 
    resources->ApplyResources(this->label2, L"label2");
    this->label2->Name = L"label2";
    // 
    // ausgabeDateiName
    // 
    resources->ApplyResources(this->ausgabeDateiName, L"ausgabeDateiName");
    this->ausgabeDateiName->Name = L"ausgabeDateiName";
    this->ausgabeDateiName->ReadOnly = true;
    // 
    // buttonAusgabeFolder
    // 
    resources->ApplyResources(this->buttonAusgabeFolder, L"buttonAusgabeFolder");
    this->buttonAusgabeFolder->Name = L"buttonAusgabeFolder";
    this->buttonAusgabeFolder->Click += gcnew System::EventHandler(this, &Form1::buttonAusgabeFolder_Click);
    // 
    // buttonAbbrechen
    // 
    this->buttonAbbrechen->DialogResult = System::Windows::Forms::DialogResult::Cancel;
    resources->ApplyResources(this->buttonAbbrechen, L"buttonAbbrechen");
    this->buttonAbbrechen->Name = L"buttonAbbrechen";
    this->buttonAbbrechen->Click += gcnew System::EventHandler(this, &Form1::buttonAbbrechen_Click);
    // 
    // meldungen
    // 
    this->meldungen->BackColor = System::Drawing::SystemColors::Control;
    this->meldungen->ForeColor = System::Drawing::Color::Red;
    resources->ApplyResources(this->meldungen, L"meldungen");
    this->meldungen->Name = L"meldungen";
    // 
    // label3
    // 
    resources->ApplyResources(this->label3, L"label3");
    this->label3->Name = L"label3";
    // 
    // shapeFileDialog
    // 
    resources->ApplyResources(this->shapeFileDialog, L"shapeFileDialog");
    this->shapeFileDialog->Multiselect = true;
    // 
    // label4
    // 
    resources->ApplyResources(this->label4, L"label4");
    this->label4->Name = L"label4";
    // 
    // label5
    // 
    resources->ApplyResources(this->label5, L"label5");
    this->label5->Name = L"label5";
    // 
    // label6
    // 
    resources->ApplyResources(this->label6, L"label6");
    this->label6->Name = L"label6";
    // 
    // shapeFileAnz
    // 
    resources->ApplyResources(this->shapeFileAnz, L"shapeFileAnz");
    this->shapeFileAnz->Name = L"shapeFileAnz";
    this->shapeFileAnz->ReadOnly = true;
    // 
    // objektReadAnz
    // 
    resources->ApplyResources(this->objektReadAnz, L"objektReadAnz");
    this->objektReadAnz->Name = L"objektReadAnz";
    this->objektReadAnz->ReadOnly = true;
    // 
    // objektNotReadAnz
    // 
    resources->ApplyResources(this->objektNotReadAnz, L"objektNotReadAnz");
    this->objektNotReadAnz->Name = L"objektNotReadAnz";
    this->objektNotReadAnz->ReadOnly = true;
    // 
    // shapeFileNotReadAnz
    // 
    resources->ApplyResources(this->shapeFileNotReadAnz, L"shapeFileNotReadAnz");
    this->shapeFileNotReadAnz->Name = L"shapeFileNotReadAnz";
    this->shapeFileNotReadAnz->ReadOnly = true;
    // 
    // label7
    // 
    resources->ApplyResources(this->label7, L"label7");
    this->label7->Name = L"label7";
    // 
    // ausgabeDateiDialog
    // 
    this->ausgabeDateiDialog->DefaultExt = L"*.gml";
    resources->ApplyResources(this->ausgabeDateiDialog, L"ausgabeDateiDialog");
    // 
    // groupBox1
    // 
    this->groupBox1->Controls->Add(this->ButtonKonversion);
    this->groupBox1->Controls->Add(this->shapeDateiNamen);
    this->groupBox1->Controls->Add(this->buttonShapeDateien);
    this->groupBox1->Controls->Add(this->konvDateiName);
    this->groupBox1->Controls->Add(this->buttonKonvDatei);
    this->groupBox1->Controls->Add(this->label1);
    this->groupBox1->Controls->Add(this->label2);
    this->groupBox1->Controls->Add(this->shapeFileNotReadAnz);
    this->groupBox1->Controls->Add(this->label7);
    this->groupBox1->Controls->Add(this->label4);
    this->groupBox1->Controls->Add(this->shapeFileAnz);
    this->groupBox1->Controls->Add(this->label5);
    this->groupBox1->Controls->Add(this->label6);
    this->groupBox1->Controls->Add(this->objektReadAnz);
    this->groupBox1->Controls->Add(this->objektNotReadAnz);
    resources->ApplyResources(this->groupBox1, L"groupBox1");
    this->groupBox1->Name = L"groupBox1";
    this->groupBox1->TabStop = false;
    // 
    // ButtonKonversion
    // 
    resources->ApplyResources(this->ButtonKonversion, L"ButtonKonversion");
    this->ButtonKonversion->Name = L"ButtonKonversion";
    this->ButtonKonversion->Click += gcnew System::EventHandler(this, &Form1::ButtonKonversion_Click);
    // 
    // groupBox2
    // 
    this->groupBox2->Controls->Add(this->buttonAusgabeFolder);
    this->groupBox2->Controls->Add(this->label3);
    this->groupBox2->Controls->Add(this->ausgabeDateiName);
    resources->ApplyResources(this->groupBox2, L"groupBox2");
    this->groupBox2->Name = L"groupBox2";
    this->groupBox2->TabStop = false;
    // 
    // groupBox3
    // 
    this->groupBox3->Controls->Add(this->checkBoxAdd);
    this->groupBox3->Controls->Add(this->buttonUeberpruefen);
    this->groupBox3->Controls->Add(this->label9);
    this->groupBox3->Controls->Add(this->buttonBPlanGMLDatei);
    this->groupBox3->Controls->Add(this->BPlanGmldatei);
    resources->ApplyResources(this->groupBox3, L"groupBox3");
    this->groupBox3->Name = L"groupBox3";
    this->groupBox3->TabStop = false;
    // 
    // checkBoxAdd
    // 
    resources->ApplyResources(this->checkBoxAdd, L"checkBoxAdd");
    this->checkBoxAdd->Name = L"checkBoxAdd";
    this->checkBoxAdd->UseVisualStyleBackColor = true;
    // 
    // buttonUeberpruefen
    // 
    resources->ApplyResources(this->buttonUeberpruefen, L"buttonUeberpruefen");
    this->buttonUeberpruefen->Name = L"buttonUeberpruefen";
    this->buttonUeberpruefen->Click += gcnew System::EventHandler(this, &Form1::buttonUeberpruefen_Click);
    // 
    // label9
    // 
    resources->ApplyResources(this->label9, L"label9");
    this->label9->Name = L"label9";
    // 
    // buttonBPlanGMLDatei
    // 
    resources->ApplyResources(this->buttonBPlanGMLDatei, L"buttonBPlanGMLDatei");
    this->buttonBPlanGMLDatei->Name = L"buttonBPlanGMLDatei";
    this->buttonBPlanGMLDatei->Click += gcnew System::EventHandler(this, &Form1::buttonBPlanGMLDatei_Click);
    // 
    // BPlanGmldatei
    // 
    resources->ApplyResources(this->BPlanGmldatei, L"BPlanGmldatei");
    this->BPlanGmldatei->Name = L"BPlanGmldatei";
    this->BPlanGmldatei->ReadOnly = true;
    // 
    // groupBox4
    // 
    this->groupBox4->Controls->Add(this->label11);
    this->groupBox4->Controls->Add(this->comboBox3DViewer);
    this->groupBox4->Controls->Add(this->buttonKmlGenerieren);
    this->groupBox4->Controls->Add(this->label10);
    this->groupBox4->Controls->Add(this->comboBoxDarstellung);
    this->groupBox4->Controls->Add(this->label8);
    this->groupBox4->Controls->Add(this->comboBoxMassstab);
    this->groupBox4->Controls->Add(this->buttonShowSvg);
    resources->ApplyResources(this->groupBox4, L"groupBox4");
    this->groupBox4->Name = L"groupBox4";
    this->groupBox4->TabStop = false;
    // 
    // label11
    // 
    resources->ApplyResources(this->label11, L"label11");
    this->label11->Name = L"label11";
    // 
    // comboBox3DViewer
    // 
    this->comboBox3DViewer->FormattingEnabled = true;
    resources->ApplyResources(this->comboBox3DViewer, L"comboBox3DViewer");
    this->comboBox3DViewer->Name = L"comboBox3DViewer";
    this->comboBox3DViewer->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox3DViewer_SelectedIndexChanged);
    // 
    // buttonKmlGenerieren
    // 
    resources->ApplyResources(this->buttonKmlGenerieren, L"buttonKmlGenerieren");
    this->buttonKmlGenerieren->Name = L"buttonKmlGenerieren";
    this->buttonKmlGenerieren->UseVisualStyleBackColor = true;
    this->buttonKmlGenerieren->Click += gcnew System::EventHandler(this, &Form1::buttonKmlGenerieren_Click);
    // 
    // label10
    // 
    resources->ApplyResources(this->label10, L"label10");
    this->label10->Name = L"label10";
    // 
    // comboBoxDarstellung
    // 
    this->comboBoxDarstellung->Items->AddRange(gcnew cli::array< System::Object^  >(2) {
      resources->GetString(L"comboBoxDarstellung.Items"),
        resources->GetString(L"comboBoxDarstellung.Items1")
    });
    resources->ApplyResources(this->comboBoxDarstellung, L"comboBoxDarstellung");
    this->comboBoxDarstellung->Name = L"comboBoxDarstellung";
    this->comboBoxDarstellung->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBoxDarstellung_SelectedIndexChanged);
    // 
    // label8
    // 
    resources->ApplyResources(this->label8, L"label8");
    this->label8->Name = L"label8";
    // 
    // comboBoxMassstab
    // 
    this->comboBoxMassstab->Items->AddRange(gcnew cli::array< System::Object^  >(11) {
      resources->GetString(L"comboBoxMassstab.Items"),
        resources->GetString(L"comboBoxMassstab.Items1"), resources->GetString(L"comboBoxMassstab.Items2"), resources->GetString(L"comboBoxMassstab.Items3"),
        resources->GetString(L"comboBoxMassstab.Items4"), resources->GetString(L"comboBoxMassstab.Items5"), resources->GetString(L"comboBoxMassstab.Items6"),
        resources->GetString(L"comboBoxMassstab.Items7"), resources->GetString(L"comboBoxMassstab.Items8"), resources->GetString(L"comboBoxMassstab.Items9"),
        resources->GetString(L"comboBoxMassstab.Items10")
    });
    resources->ApplyResources(this->comboBoxMassstab, L"comboBoxMassstab");
    this->comboBoxMassstab->Name = L"comboBoxMassstab";
    this->comboBoxMassstab->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
    // 
    // buttonShowSvg
    // 
    resources->ApplyResources(this->buttonShowSvg, L"buttonShowSvg");
    this->buttonShowSvg->Name = L"buttonShowSvg";
    this->buttonShowSvg->Click += gcnew System::EventHandler(this, &Form1::buttonShowSvg_Click);
    // 
    // buttonAttributeBearbeiten
    // 
    resources->ApplyResources(this->buttonAttributeBearbeiten, L"buttonAttributeBearbeiten");
    this->buttonAttributeBearbeiten->Name = L"buttonAttributeBearbeiten";
    this->buttonAttributeBearbeiten->Click += gcnew System::EventHandler(this, &Form1::buttonAttributeBearbeiten_Click);
    // 
    // label15
    // 
    resources->ApplyResources(this->label15, L"label15");
    this->label15->Name = L"label15";
    // 
    // label14
    // 
    resources->ApplyResources(this->label14, L"label14");
    this->label14->Name = L"label14";
    // 
    // comboBPlanEbene
    // 
    resources->ApplyResources(this->comboBPlanEbene, L"comboBPlanEbene");
    this->comboBPlanEbene->Name = L"comboBPlanEbene";
    this->comboBPlanEbene->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBPlanEbene_SelectedIndexChanged);
    // 
    // comboBPlan
    // 
    this->comboBPlan->BackColor = System::Drawing::SystemColors::Window;
    resources->ApplyResources(this->comboBPlan, L"comboBPlan");
    this->comboBPlan->Name = L"comboBPlan";
    this->comboBPlan->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBPlan_SelectedIndexChanged);
    // 
    // BPlanGmlDialog
    // 
    resources->ApplyResources(this->BPlanGmlDialog, L"BPlanGmlDialog");
    this->BPlanGmlDialog->Multiselect = true;
    // 
    // svgDateiDialog
    // 
    resources->ApplyResources(this->svgDateiDialog, L"svgDateiDialog");
    // 
    // mainMenu1
    // 
    this->mainMenu1->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(10) {
      this->menuItem7, this->menuItem10,
        this->menuItem1, this->menuItem4, this->menuItem5, this->menuItem9, this->menuItem2, this->menuItem29, this->menuItem6, this->menuItem11
    });
    // 
    // menuItem7
    // 
    this->menuItem7->Index = 0;
    this->menuItem7->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(11) {
      this->menuItemGmlLesen, this->menuItem3,
        this->menuItemGmlSpeichern, this->menuItemGmlSpeichernUnter, this->menuItem21, this->menuItem34, this->menuItem325833To25833,
        this->menuItem35, this->menuItemLogfile, this->menuItem41, this->menuItemBeenden
    });
    resources->ApplyResources(this->menuItem7, L"menuItem7");
    // 
    // menuItemGmlLesen
    // 
    this->menuItemGmlLesen->Index = 0;
    resources->ApplyResources(this->menuItemGmlLesen, L"menuItemGmlLesen");
    this->menuItemGmlLesen->Click += gcnew System::EventHandler(this, &Form1::menuItemGmlLesen_Click);
    // 
    // menuItem3
    // 
    this->menuItem3->Index = 1;
    resources->ApplyResources(this->menuItem3, L"menuItem3");
    // 
    // menuItemGmlSpeichern
    // 
    this->menuItemGmlSpeichern->Index = 2;
    resources->ApplyResources(this->menuItemGmlSpeichern, L"menuItemGmlSpeichern");
    this->menuItemGmlSpeichern->Click += gcnew System::EventHandler(this, &Form1::menuItemGmlSpeichern_Click);
    // 
    // menuItemGmlSpeichernUnter
    // 
    this->menuItemGmlSpeichernUnter->Index = 3;
    resources->ApplyResources(this->menuItemGmlSpeichernUnter, L"menuItemGmlSpeichernUnter");
    this->menuItemGmlSpeichernUnter->Click += gcnew System::EventHandler(this, &Form1::menuItemGmlSpeichernUnter_Click);
    // 
    // menuItem21
    // 
    this->menuItem21->Index = 4;
    resources->ApplyResources(this->menuItem21, L"menuItem21");
    // 
    // menuItem34
    // 
    this->menuItem34->Index = 5;
    resources->ApplyResources(this->menuItem34, L"menuItem34");
    this->menuItem34->Click += gcnew System::EventHandler(this, &Form1::coordSystemTransformation_Click);
    // 
    // menuItem325833To25833
    // 
    this->menuItem325833To25833->Index = 6;
    resources->ApplyResources(this->menuItem325833To25833, L"menuItem325833To25833");
    this->menuItem325833To25833->Click += gcnew System::EventHandler(this, &Form1::menuItem325833To25833_Click);
    // 
    // menuItem35
    // 
    this->menuItem35->Index = 7;
    resources->ApplyResources(this->menuItem35, L"menuItem35");
    // 
    // menuItemLogfile
    // 
    this->menuItemLogfile->Index = 8;
    resources->ApplyResources(this->menuItemLogfile, L"menuItemLogfile");
    this->menuItemLogfile->Click += gcnew System::EventHandler(this, &Form1::menuItemLogfile_Click);
    // 
    // menuItem41
    // 
    this->menuItem41->Index = 9;
    resources->ApplyResources(this->menuItem41, L"menuItem41");
    // 
    // menuItemBeenden
    // 
    this->menuItemBeenden->Index = 10;
    resources->ApplyResources(this->menuItemBeenden, L"menuItemBeenden");
    this->menuItemBeenden->Click += gcnew System::EventHandler(this, &Form1::menuItemBeenden_Click);
    // 
    // menuItem10
    // 
    this->menuItem10->Index = 1;
    this->menuItem10->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(1) { this->menuItemAttributeBearbeiten });
    resources->ApplyResources(this->menuItem10, L"menuItem10");
    // 
    // menuItemAttributeBearbeiten
    // 
    this->menuItemAttributeBearbeiten->Index = 0;
    resources->ApplyResources(this->menuItemAttributeBearbeiten, L"menuItemAttributeBearbeiten");
    this->menuItemAttributeBearbeiten->Click += gcnew System::EventHandler(this, &Form1::menuItemAttributeBearbeiten_Click);
    // 
    // menuItem1
    // 
    this->menuItem1->Index = 2;
    this->menuItem1->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(2) { this->menuItem20, this->menuItemGenerierenINSPIREPLU });
    resources->ApplyResources(this->menuItem1, L"menuItem1");
    // 
    // menuItem20
    // 
    this->menuItem20->Index = 0;
    this->menuItem20->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(11) {
      this->menuItemShapefilesSpezifizieren,
        this->menuItem30, this->menuItemKonversionErzeugen, this->menuItemKonversionOeffnen, this->menuItemKonversionBearbeiten, this->menuItemKonversionSchliessen,
        this->menuItem37, this->menuItemKonversionSpeichern, this->menuItemKonversionSpeichernUnter, this->menuItem40, this->menuItemShapefilesKonvertieren
    });
    resources->ApplyResources(this->menuItem20, L"menuItem20");
    // 
    // menuItemShapefilesSpezifizieren
    // 
    this->menuItemShapefilesSpezifizieren->Index = 0;
    resources->ApplyResources(this->menuItemShapefilesSpezifizieren, L"menuItemShapefilesSpezifizieren");
    this->menuItemShapefilesSpezifizieren->Click += gcnew System::EventHandler(this, &Form1::menuItemShapefilesSpezifizieren_Click);
    // 
    // menuItem30
    // 
    this->menuItem30->Index = 1;
    resources->ApplyResources(this->menuItem30, L"menuItem30");
    // 
    // menuItemKonversionErzeugen
    // 
    this->menuItemKonversionErzeugen->Index = 2;
    resources->ApplyResources(this->menuItemKonversionErzeugen, L"menuItemKonversionErzeugen");
    this->menuItemKonversionErzeugen->Click += gcnew System::EventHandler(this, &Form1::menuItemKonversionErzeugen_Click);
    // 
    // menuItemKonversionOeffnen
    // 
    this->menuItemKonversionOeffnen->Index = 3;
    resources->ApplyResources(this->menuItemKonversionOeffnen, L"menuItemKonversionOeffnen");
    this->menuItemKonversionOeffnen->Click += gcnew System::EventHandler(this, &Form1::menuItemKonversionOeffnen_Click);
    // 
    // menuItemKonversionBearbeiten
    // 
    this->menuItemKonversionBearbeiten->Index = 4;
    resources->ApplyResources(this->menuItemKonversionBearbeiten, L"menuItemKonversionBearbeiten");
    this->menuItemKonversionBearbeiten->Click += gcnew System::EventHandler(this, &Form1::menuItemKonversionBearbeiten_Click);
    // 
    // menuItemKonversionSchliessen
    // 
    this->menuItemKonversionSchliessen->Index = 5;
    resources->ApplyResources(this->menuItemKonversionSchliessen, L"menuItemKonversionSchliessen");
    this->menuItemKonversionSchliessen->Click += gcnew System::EventHandler(this, &Form1::menuItemKonversionSchliessen_Click);
    // 
    // menuItem37
    // 
    this->menuItem37->Index = 6;
    resources->ApplyResources(this->menuItem37, L"menuItem37");
    // 
    // menuItemKonversionSpeichern
    // 
    this->menuItemKonversionSpeichern->Index = 7;
    resources->ApplyResources(this->menuItemKonversionSpeichern, L"menuItemKonversionSpeichern");
    this->menuItemKonversionSpeichern->Click += gcnew System::EventHandler(this, &Form1::menuItemKonversionSpeichern_Click);
    // 
    // menuItemKonversionSpeichernUnter
    // 
    this->menuItemKonversionSpeichernUnter->Index = 8;
    resources->ApplyResources(this->menuItemKonversionSpeichernUnter, L"menuItemKonversionSpeichernUnter");
    this->menuItemKonversionSpeichernUnter->Click += gcnew System::EventHandler(this, &Form1::menuItemKonversionSpeichernUnter_Click);
    // 
    // menuItem40
    // 
    this->menuItem40->Index = 9;
    resources->ApplyResources(this->menuItem40, L"menuItem40");
    // 
    // menuItemShapefilesKonvertieren
    // 
    this->menuItemShapefilesKonvertieren->Index = 10;
    resources->ApplyResources(this->menuItemShapefilesKonvertieren, L"menuItemShapefilesKonvertieren");
    this->menuItemShapefilesKonvertieren->Click += gcnew System::EventHandler(this, &Form1::menuItemShapefilesKonvertieren_Click);
    // 
    // menuItemGenerierenINSPIREPLU
    // 
    this->menuItemGenerierenINSPIREPLU->Index = 1;
    this->menuItemGenerierenINSPIREPLU->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(2) {
      this->menuItem55,
        this->menuItem56
    });
    resources->ApplyResources(this->menuItemGenerierenINSPIREPLU, L"menuItemGenerierenINSPIREPLU");
    // 
    // menuItem55
    // 
    this->menuItem55->Index = 0;
    resources->ApplyResources(this->menuItem55, L"menuItem55");
    this->menuItem55->Click += gcnew System::EventHandler(this, &Form1::menuItemGenerierenINSPIREPLU_Click);
    // 
    // menuItem56
    // 
    this->menuItem56->Index = 1;
    resources->ApplyResources(this->menuItem56, L"menuItem56");
    this->menuItem56->Click += gcnew System::EventHandler(this, &Form1::menuINSPIREPLU_XPlanung_5_0_Click);
    // 
    // menuItem4
    // 
    this->menuItem4->Index = 3;
    this->menuItem4->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(3) {
      this->menuItemMigrationXPlanGML3_0,
        this->menuItem48, this->menuItem50
    });
    resources->ApplyResources(this->menuItem4, L"menuItem4");
    // 
    // menuItemMigrationXPlanGML3_0
    // 
    this->menuItemMigrationXPlanGML3_0->Index = 0;
    this->menuItemMigrationXPlanGML3_0->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(2) {
      this->menuItem67,
        this->menuItem68
    });
    resources->ApplyResources(this->menuItemMigrationXPlanGML3_0, L"menuItemMigrationXPlanGML3_0");
    // 
    // menuItem67
    // 
    this->menuItem67->Index = 0;
    resources->ApplyResources(this->menuItem67, L"menuItem67");
    this->menuItem67->Click += gcnew System::EventHandler(this, &Form1::menuItem67_Click);
    // 
    // menuItem68
    // 
    this->menuItem68->Index = 1;
    resources->ApplyResources(this->menuItem68, L"menuItem68");
    this->menuItem68->Click += gcnew System::EventHandler(this, &Form1::menuItem68_Click);
    // 
    // menuItem48
    // 
    this->menuItem48->Index = 1;
    this->menuItem48->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(3) {
      this->menuItem15, this->menuItem33,
        this->menuItem38
    });
    resources->ApplyResources(this->menuItem48, L"menuItem48");
    // 
    // menuItem15
    // 
    this->menuItem15->Index = 0;
    resources->ApplyResources(this->menuItem15, L"menuItem15");
    this->menuItem15->Click += gcnew System::EventHandler(this, &Form1::menuItem15_Click);
    // 
    // menuItem33
    // 
    this->menuItem33->Index = 1;
    resources->ApplyResources(this->menuItem33, L"menuItem33");
    this->menuItem33->Click += gcnew System::EventHandler(this, &Form1::menuItem33_Click);
    // 
    // menuItem38
    // 
    this->menuItem38->Index = 2;
    resources->ApplyResources(this->menuItem38, L"menuItem38");
    this->menuItem38->Click += gcnew System::EventHandler(this, &Form1::menuItem38_Click);
    // 
    // menuItem50
    // 
    this->menuItem50->Index = 2;
    this->menuItem50->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(2) { this->menuItem65, this->menuItem66 });
    resources->ApplyResources(this->menuItem50, L"menuItem50");
    // 
    // menuItem65
    // 
    this->menuItem65->Index = 0;
    resources->ApplyResources(this->menuItem65, L"menuItem65");
    this->menuItem65->Click += gcnew System::EventHandler(this, &Form1::menuItem65_Click);
    // 
    // menuItem66
    // 
    this->menuItem66->Index = 1;
    resources->ApplyResources(this->menuItem66, L"menuItem66");
    this->menuItem66->Click += gcnew System::EventHandler(this, &Form1::menuItem66_Click);
    // 
    // menuItem5
    // 
    this->menuItem5->Index = 4;
    this->menuItem5->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(1) { this->menuItemCodelistOrdner });
    resources->ApplyResources(this->menuItem5, L"menuItem5");
    // 
    // menuItemCodelistOrdner
    // 
    this->menuItemCodelistOrdner->Index = 0;
    resources->ApplyResources(this->menuItemCodelistOrdner, L"menuItemCodelistOrdner");
    this->menuItemCodelistOrdner->Click += gcnew System::EventHandler(this, &Form1::menuItemCodelistOrdner_Click);
    // 
    // menuItem9
    // 
    this->menuItem9->Index = 5;
    this->menuItem9->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(3) {
      this->menuItemShowSvg, this->menuItemShowKml,
        this->menuItemShowCityGML
    });
    resources->ApplyResources(this->menuItem9, L"menuItem9");
    // 
    // menuItemShowSvg
    // 
    this->menuItemShowSvg->Index = 0;
    resources->ApplyResources(this->menuItemShowSvg, L"menuItemShowSvg");
    this->menuItemShowSvg->Click += gcnew System::EventHandler(this, &Form1::menuItemShowSvg_Click);
    // 
    // menuItemShowKml
    // 
    this->menuItemShowKml->Index = 1;
    resources->ApplyResources(this->menuItemShowKml, L"menuItemShowKml");
    this->menuItemShowKml->Click += gcnew System::EventHandler(this, &Form1::menuItemShowKml_Click);
    // 
    // menuItemShowCityGML
    // 
    this->menuItemShowCityGML->Index = 2;
    resources->ApplyResources(this->menuItemShowCityGML, L"menuItemShowCityGML");
    this->menuItemShowCityGML->Click += gcnew System::EventHandler(this, &Form1::menuItemShowCityGML_Click);
    // 
    // menuItem2
    // 
    this->menuItem2->Index = 6;
    this->menuItem2->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(1) { this->menuItemPruefenXPlanGMLKonformitaet });
    resources->ApplyResources(this->menuItem2, L"menuItem2");
    // 
    // menuItemPruefenXPlanGMLKonformitaet
    // 
    this->menuItemPruefenXPlanGMLKonformitaet->Index = 0;
    resources->ApplyResources(this->menuItemPruefenXPlanGMLKonformitaet, L"menuItemPruefenXPlanGMLKonformitaet");
    this->menuItemPruefenXPlanGMLKonformitaet->Click += gcnew System::EventHandler(this, &Form1::menuItemPruefenXPlanGMLKonformitaet_Click);
    // 
    // menuItem29
    // 
    this->menuItem29->Index = 7;
    resources->ApplyResources(this->menuItem29, L"menuItem29");
    // 
    // menuItem6
    // 
    this->menuItem6->Index = 8;
    this->menuItem6->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(6) {
      this->menuItem23, this->menuItem24,
        this->menuItem25, this->menuItem26, this->menuItem32, this->menuItem17
    });
    resources->ApplyResources(this->menuItem6, L"menuItem6");
    // 
    // menuItem23
    // 
    this->menuItem23->Index = 0;
    resources->ApplyResources(this->menuItem23, L"menuItem23");
    this->menuItem23->Click += gcnew System::EventHandler(this, &Form1::menuItem23_Click);
    // 
    // menuItem24
    // 
    this->menuItem24->Index = 1;
    resources->ApplyResources(this->menuItem24, L"menuItem24");
    this->menuItem24->Click += gcnew System::EventHandler(this, &Form1::menuItem24_Click);
    // 
    // menuItem25
    // 
    this->menuItem25->Index = 2;
    resources->ApplyResources(this->menuItem25, L"menuItem25");
    // 
    // menuItem26
    // 
    this->menuItem26->Index = 3;
    resources->ApplyResources(this->menuItem26, L"menuItem26");
    this->menuItem26->Click += gcnew System::EventHandler(this, &Form1::menuItem26_Click);
    // 
    // menuItem32
    // 
    this->menuItem32->Index = 4;
    resources->ApplyResources(this->menuItem32, L"menuItem32");
    // 
    // menuItem17
    // 
    this->menuItem17->Index = 5;
    resources->ApplyResources(this->menuItem17, L"menuItem17");
    this->menuItem17->Click += gcnew System::EventHandler(this, &Form1::menuItem17_Click);
    // 
    // menuItem11
    // 
    this->menuItem11->Index = 9;
    this->menuItem11->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(3) {
      this->menuItem27, this->menuItem69,
        this->menuItem14
    });
    resources->ApplyResources(this->menuItem11, L"menuItem11");
    // 
    // menuItem27
    // 
    this->menuItem27->Index = 0;
    resources->ApplyResources(this->menuItem27, L"menuItem27");
    this->menuItem27->Click += gcnew System::EventHandler(this, &Form1::menuItem27_Click);
    // 
    // menuItem69
    // 
    this->menuItem69->Index = 1;
    resources->ApplyResources(this->menuItem69, L"menuItem69");
    // 
    // menuItem14
    // 
    this->menuItem14->Index = 2;
    resources->ApplyResources(this->menuItem14, L"menuItem14");
    this->menuItem14->Click += gcnew System::EventHandler(this, &Form1::menuItem14_Click_1);
    // 
    // menuItem36
    // 
    this->menuItem36->Index = -1;
    resources->ApplyResources(this->menuItem36, L"menuItem36");
    // 
    // menuItem39
    // 
    this->menuItem39->Index = -1;
    resources->ApplyResources(this->menuItem39, L"menuItem39");
    // 
    // menuItem42
    // 
    this->menuItem42->Index = -1;
    resources->ApplyResources(this->menuItem42, L"menuItem42");
    // 
    // groupBox5
    // 
    this->groupBox5->Controls->Add(this->textBoxXPlanGMLVersion);
    this->groupBox5->Controls->Add(this->textBoxPlanTyp);
    resources->ApplyResources(this->groupBox5, L"groupBox5");
    this->groupBox5->Name = L"groupBox5";
    this->groupBox5->TabStop = false;
    // 
    // textBoxXPlanGMLVersion
    // 
    this->textBoxXPlanGMLVersion->BackColor = System::Drawing::SystemColors::Control;
    resources->ApplyResources(this->textBoxXPlanGMLVersion, L"textBoxXPlanGMLVersion");
    this->textBoxXPlanGMLVersion->ForeColor = System::Drawing::Color::Red;
    this->textBoxXPlanGMLVersion->HideSelection = false;
    this->textBoxXPlanGMLVersion->Name = L"textBoxXPlanGMLVersion";
    // 
    // textBoxPlanTyp
    // 
    this->textBoxPlanTyp->BackColor = System::Drawing::SystemColors::Control;
    this->textBoxPlanTyp->ForeColor = System::Drawing::Color::Blue;
    this->textBoxPlanTyp->HideSelection = false;
    resources->ApplyResources(this->textBoxPlanTyp, L"textBoxPlanTyp");
    this->textBoxPlanTyp->Name = L"textBoxPlanTyp";
    // 
    // saveConvDateiDialog
    // 
    this->saveConvDateiDialog->DefaultExt = L"*.xml";
    resources->ApplyResources(this->saveConvDateiDialog, L"saveConvDateiDialog");
    // 
    // pictureBox2
    // 
    resources->ApplyResources(this->pictureBox2, L"pictureBox2");
    this->pictureBox2->Name = L"pictureBox2";
    this->pictureBox2->TabStop = false;
    // 
    // checkBoxAllePlaene
    // 
    resources->ApplyResources(this->checkBoxAllePlaene, L"checkBoxAllePlaene");
    this->checkBoxAllePlaene->Name = L"checkBoxAllePlaene";
    // 
    // pictureBox1
    // 
    resources->ApplyResources(this->pictureBox1, L"pictureBox1");
    this->pictureBox1->Name = L"pictureBox1";
    this->pictureBox1->TabStop = false;
    // 
    // Form1
    // 
    resources->ApplyResources(this, L"$this");
    this->BackColor = System::Drawing::SystemColors::Control;
    this->Controls->Add(this->checkBoxAllePlaene);
    this->Controls->Add(this->pictureBox2);
    this->Controls->Add(this->pictureBox1);
    this->Controls->Add(this->groupBox5);
    this->Controls->Add(this->groupBox2);
    this->Controls->Add(this->buttonAbbrechen);
    this->Controls->Add(this->groupBox1);
    this->Controls->Add(this->meldungen);
    this->Controls->Add(this->label15);
    this->Controls->Add(this->comboBPlanEbene);
    this->Controls->Add(this->comboBPlan);
    this->Controls->Add(this->label14);
    this->Controls->Add(this->groupBox4);
    this->Controls->Add(this->groupBox3);
    this->Controls->Add(this->buttonAttributeBearbeiten);
    this->MaximizeBox = false;
    this->Menu = this->mainMenu1;
    this->MinimizeBox = false;
    this->Name = L"Form1";
    this->TransparencyKey = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
      static_cast<System::Int32>(static_cast<System::Byte>(128)));
    this->groupBox1->ResumeLayout(false);
    this->groupBox1->PerformLayout();
    this->groupBox2->ResumeLayout(false);
    this->groupBox2->PerformLayout();
    this->groupBox3->ResumeLayout(false);
    this->groupBox3->PerformLayout();
    this->groupBox4->ResumeLayout(false);
    this->groupBox4->PerformLayout();
    this->groupBox5->ResumeLayout(false);
    this->groupBox5->PerformLayout();
    (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
    (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
    this->ResumeLayout(false);
    this->PerformLayout();

  }

  ///////////////////////////////////////////////////////////////////////////////
  //  Einlesen und Interpretation der Datei GmlToolbox.ini                     //
  ///////////////////////////////////////////////////////////////////////////////
private: System::Void readIniFile()
{
  String ^ fName;
  String ^ pActElement;
  String ^ VERSION;
  String ^ STR;
  String ^ STR1;
  String ^ STR2;

  System::Windows::Forms::DialogResult weiter;

  string   srsName;
  string   srsDescription;
  string   stdString;
  string   codeListsPfad;
  string   externalCodeListsPfad;
  int      i;
  double   h;
  bool     isRoot = true;


  fName = String::Concat(SystemFolder, "GML-Toolbox.ini");
  if (!System::IO::File::Exists(fName))
  {
    MessageBox::Show("GML-Toolbox.ini Datei nicht gefunden");
    exit(-1);
  }
  XmlTextReader ^ reader = gcnew XmlTextReader(fName);

  try
  {
    while (reader->Read())
    {
      if (reader->NodeType == XmlNodeType::Element)
      {
        pActElement = reader->LocalName;

        if (isRoot)
        {
          if (pActElement->CompareTo("GML-Toolbox") != 0)
          {
            MessageBox::Show("Ungültige Ini-Datei");
            exit(-1);
          }

          VERSION = reader->GetAttribute("version");
          if (VERSION == nullptr || String::Compare(VERSION, ToolboxVersion) != 0)
          {
            STR = "Falsche Version der ini-Datei\n\n";
            STR1 = String::Concat("Benötigt: Version ", ToolboxVersion, "\n");
            STR2 = String::Concat("Gelesen:  Version ", VERSION, "\n");
            STR = String::Concat(STR, STR1, STR2, "Weiter?");

            weiter = System::Windows::Forms::MessageBox::Show(STR, "",
              MessageBoxButtons::YesNo, MessageBoxIcon::Question);
            if (weiter == System::Windows::Forms::DialogResult::No)exit(0);
          }
        }
        isRoot = false;

        if (pActElement->CompareTo("CodeSpaceAttributeAsGMLDictionary") == 0)
        {
          STR = reader->ReadString();
          if (STR->CompareTo("true") == 0)
            pBLeitplanViewer->setReadCodeListDictionaries(true);
          else
            pBLeitplanViewer->setReadCodeListDictionaries(false);
        }

        if (pActElement->CompareTo("CheckTransformModule") == 0)
        {
          STR = reader->ReadString();
          if (STR->CompareTo("true") == 0)
            checkTransformModule = true;
          else
            checkTransformModule = false;
        }

        if (pActElement->CompareTo("ExternalCodeListsFolderV2") == 0)
        {
          STR = reader->ReadString();
          systemStr2stdStr(stdString, STR);
          if (stdString != "")
            pBLeitplanViewer->setExternalCodeListFolder(XPLANGML_2_0, stdString);
        }

        if (pActElement->CompareTo("ExternalCodeListsFolderV3") == 0)
        {
          STR = reader->ReadString();
          systemStr2stdStr(stdString, STR);
          if (stdString != "")
            pBLeitplanViewer->setExternalCodeListFolder(XPLANGML_3_0, stdString);
        }

        if (pActElement->CompareTo("ExternalCodeListsFolderV4") == 0)
        {
          STR = reader->ReadString();
          systemStr2stdStr(stdString, STR);
          if (stdString != "")
            pBLeitplanViewer->setExternalCodeListFolder(XPLANGML_4_0, stdString);
        }

        if (pActElement->CompareTo("ExternalCodeListsFolderV4_1") == 0)
        {
          STR = reader->ReadString();
          systemStr2stdStr(stdString, STR);
          if (stdString != "")
            pBLeitplanViewer->setExternalCodeListFolder(XPLANGML_4_1, stdString);
        }

        if (pActElement->CompareTo("ExternalCodeListsFolderV5") == 0)
        {
          STR = reader->ReadString();
          systemStr2stdStr(stdString, STR);
          if (stdString != "")
            pBLeitplanViewer->setExternalCodeListFolder(XPLANGML_5_0, stdString);
        }

        if (pActElement->CompareTo("ExternalCodeListsFolderV5_1") == 0)
        {
          STR = reader->ReadString();
          systemStr2stdStr(stdString, STR);
          if (stdString != "")
            pBLeitplanViewer->setExternalCodeListFolder(XPLANGML_5_1, stdString);
        }

        if (pActElement->CompareTo("ExternalCodeListsFolderV5_2") == 0)
        {
          STR = reader->ReadString();
          systemStr2stdStr(stdString, STR);
          if (stdString != "")
            pBLeitplanViewer->setExternalCodeListFolder(XPLANGML_5_2, stdString);
        }

        if (pActElement->CompareTo("ExternalCodeListsFolderInspireLUPlanned") == 0)
        {
          STR = reader->ReadString();
          systemStr2stdStr(stdString, STR);
          if (stdString != "") pBLeitplanViewer->setExternalCodeListFolder(INSPIRE_LU_PLANNED, stdString);
        }

        if (pActElement->CompareTo("KMLViewer") == 0)
        {
          STR = reader->ReadString();
          systemStr2stdStr(stdString, STR);
          pBLeitplanViewer->setKMLViewer(stdString);
          comboBox3DViewer->Items->Add("KML");
        }

        if (pActElement->CompareTo("GMLViewer") == 0)
        {
          STR = reader->ReadString();
          systemStr2stdStr(stdString, STR);
          pBLeitplanViewer->setGMLViewer(stdString);
          comboBox3DViewer->Items->Add("GML");
        }

        /* if ( pActElement->CompareTo("SVGViewer") == 0 )
         {
           STR = reader->ReadString();
           systemStr2stdStr( stdString, STR );
           pBLeitplanViewer->setSvgViewer( stdString );
         }
         */

        if (pActElement->CompareTo("Viewer3D") == 0)
        {
          STR = reader->ReadString();
          if (STR->CompareTo("KML") == 0)
          {
            pBLeitplanViewer->setDarstellungsTyp3D(KML);
            comboBox3DViewer->SelectedItem = "KML";
          }
          else
            if (STR->CompareTo("KIT") == 0)
            {
              pBLeitplanViewer->setDarstellungsTyp3D(GML);
              comboBox3DViewer->SelectedItem = "GML";
            }
        }

        if (pActElement->CompareTo("ExternalCodeListsFolderCityGML_2_0") == 0)
        {
          STR = reader->ReadString();
          systemStr2stdStr(stdString, STR);
          if (stdString != "") pBLeitplanViewer->setExternalCodeListFolder(CITYGML_2_0, stdString);
        }

        if (pActElement->CompareTo("ProfilFehlerStylesheet") == 0)
        {
          FehlerStylesheet = reader->ReadString();
          FehlerStylesheet = String::Concat(SystemFolder, "xsl\\", FehlerStylesheet);
        }

        if (pActElement->CompareTo("MigrationsFehlerStylesheet") == 0)
        {
          MigrationStylesheet = reader->ReadString();
          MigrationStylesheet = String::Concat(SystemFolder, "xsl\\", MigrationStylesheet);
        }

        if (pActElement->CompareTo("JavaHome") == 0)
        {
          String ^ JavaPath = reader->ReadString();
          systemStr2stdStr(stdString, JavaPath);
          pBLeitplanViewer->setJavaPath(stdString);
        }

        if (pActElement->CompareTo("BatikPath") == 0)
        {
          String ^ BatikPath = reader->ReadString();
          systemStr2stdStr(stdString, BatikPath);
          pBLeitplanViewer->setBatikPath(stdString);
        }

        if (pActElement->CompareTo("JavaHeapSize") == 0)
        {
          String ^ JavaHeapSize = reader->ReadString();
          systemStr2stdStr(stdString, JavaHeapSize);
          pBLeitplanViewer->setJavaHeapSize(stdString);
        }

        if (pActElement->CompareTo("BoreholeRadius") == 0)
        {
          STR = reader->ReadString();
          h = XmlConvert::ToDouble(STR);
          pBLeitplanViewer->setBoreholeRadius(h);
        }

        if (pActElement->CompareTo("BoreholeCylinderComplexity") == 0)
        {
          STR = reader->ReadString();
          i = XmlConvert::ToInt32(STR);
          pBLeitplanViewer->setBoreholeCylinderComplexity(i);
        }

        if (pActElement->CompareTo("BoreholeShowIntervals") == 0)
        {
          STR = reader->ReadString();
          if (STR->CompareTo("false") == 0)
            pBLeitplanViewer->setBoreholeShowIntervals(false);
          else
            pBLeitplanViewer->setBoreholeShowIntervals(true);
        }

        if (pActElement->CompareTo("PlanausgabeBreite") == 0)
        {
          STR = reader->ReadString();
          h = XmlConvert::ToDouble(STR);
          pBLeitplanViewer->setPlanausgabeBreite(float(h));
        }
        else
          if (pActElement->CompareTo("PlanausgabeHoehe") == 0)
          {
            STR = reader->ReadString();
            h = XmlConvert::ToDouble(STR);
            pBLeitplanViewer->setPlanausgabeHoehe(float(h));
          }
      }
      else
        if (reader->NodeType == XmlNodeType::EndElement)
        {
          pActElement = reader->LocalName;
          if (pActElement->CompareTo("GML-Toolbox") == 0) break;
        }
    }
  }
  catch (System::Xml::XmlException ^ ex)
  {
    int      lineNumber, position;
    String ^ STR;

    lineNumber = ex->LineNumber;
    position = ex->LinePosition;
    STR = String::Concat("Fehlerhafte INI-Datei, Zeile ",
      Convert::ToString(lineNumber),
      " Position ",
      Convert::ToString(position)
    );
    MessageBox::Show(STR);
    exit(-1);
  }

  textBoxXPlanGMLVersion->Text = "Nicht gesetzt";
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Button "Konvertierungs-Datei Lesen                        //
         //  Einlesen einer Konversions-Datei									                    	 //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonKonvDatei_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  string message;

  bool success = pBLeitplanViewer->openConversionFile(konvDateiName->Text, message);
  konvDateiName->Text = ToString(message);
  if (success)
  {
    displayGmlType();
    displayXPlanGMLPlanArt();

    if (pBLeitplanViewer->getShapefileAnz() > 0)
    {
      menuItemKonversionBearbeiten->Enabled = true;
      menuItemShapefilesKonvertieren->Enabled = true;
      ButtonKonversion->Enabled = true;
    }
    menuItemKonversionSchliessen->Enabled = true;
    menuItemKonversionSpeichern->Enabled = true;
    menuItemKonversionSpeichernUnter->Enabled = true;
  }

}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler für Button Shapefiles spezifizieren                       // 
         //  Spezifikation zu konvertierender Shape-Dateien							             //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonShapeDateien_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  System::String      ^ STR;
  int				            i, nFiles;
  bool		              success;
  vector<string>        shapeFileNames;
  string                stdString;
  cli::array<String^> ^ shapeFileArray;


  if (shapeFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
  {

    shapeFileArray = shapeFileDialog->FileNames;
    nFiles = shapeFileArray->Length;

    for (i = 0; i < nFiles; i++)
    {
      STR = shapeFileArray[i];
      systemStr2stdStr(stdString, STR);
      shapeFileNames.push_back(stdString);
    }

    size_t anzShapefiles;
    int    anzErrors;
    success = pBLeitplanViewer->specifyShapefiles(shapeFileNames, anzShapefiles, anzErrors);
    if (success)
      shapeDateiNamen->Text = String::Concat(Convert::ToString(anzShapefiles), " Shapefiles spezififiert");
    else
      shapeDateiNamen->Text = String::Concat("Fehler bei der Spezifikation von Shapefiles", Convert::ToString(anzErrors), "von ", Convert::ToString(anzShapefiles), " nicht gelesen");


    if (success)
    {
      menuItemKonversionErzeugen->Enabled = true;
      if (pBLeitplanViewer->existConversion())
      {
        menuItemShapefilesKonvertieren->Enabled = true;
        menuItemKonversionBearbeiten->Enabled = true;
        ButtonKonversion->Enabled = true;
      }
    }
    else meldungen->Text = "Fehler bei der Analyse der Shapefiles";
  }
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler für Button GML erzeugen --> schreiben
         //  Schreiben des aktuell ausgewählten Plans auf eine GML-Datei				       //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAusgabeFolder_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  System::String ^ pfadName;
  System::String ^ fileName;
  System::String ^ meldung;
  std::string      name;
  int				       anz;

  if (ausgabeDateiDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
  {
    pfadName = ausgabeDateiDialog->FileName;


    int index = pfadName->LastIndexOf("\\");
    fileName = pfadName->Substring(index + 1);
    ausgabeDateiName->Text = fileName;
    BPlanGmldatei->Text = pfadName;

    systemStr2stdStr(name, pfadName);
    anz = pBLeitplanViewer->writeXPlanGML(name);

    buttonUeberpruefen->Enabled = true;
    menuItemPruefenXPlanGMLKonformitaet->Enabled = true;

    meldung = "Anzahl der geschriebenen XPlanGML-Objekte: ";
    meldung = String::Concat(meldung, System::Convert::ToString(anz));
    meldungen->Text = meldung;
  }
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Beendet die Anwendung													                           //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAbbrechen_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  if (conversionSaved == false)
  {
    System::Windows::Forms::DialogResult result = System::Windows::Forms::MessageBox::Show("Soll die Konversion gesichert werden?",
      "Sichern einer Konv-Datei", System::Windows::Forms::MessageBoxButtons::YesNo);

    if (result == System::Windows::Forms::DialogResult::Yes)
    {
      menuItemKonversionSpeichern_Click(sender, e);
      conversionSaved = true;
    }
  }

  Close();
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler für Menue Datei-->Lesen und Button "GML einlesen --> Lesen//
         //////////////////////////////////////////////////////////////////////////////
private: System::Void buttonBPlanGMLDatei_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  string dateiName, bPlanName;
  size_t            i, anz, anzPlanBereiche, nummer, anzPlaene;
  vector<string>   vPlanNamen;
  vector<int>      vPlanBereichNummern;
  System::String ^ STR;
  System::String ^ meldung;

  if (BPlanGmlDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
  {
    int fileAnz = BPlanGmlDialog->FileNames->Length;
    if (fileAnz == 0)
      return;
    else
      if (fileAnz == 1)
      {
        BPlanGmldatei->Text = BPlanGmlDialog->FileName;
        GmlDateiName = BPlanGmlDialog->FileName;

        systemStr2stdStr(dateiName, BPlanGmldatei->Text);
        anz = pBLeitplanViewer->readGML(dateiName, checkBoxAdd->Checked);
      }
      else
      {
        vector<string> vDateiNamen;


        for (i = 0; i < fileAnz; i++)
        {
          String ^ fileName = BPlanGmlDialog->FileNames[i];
          systemStr2stdStr(dateiName, fileName);
          vDateiNamen.push_back(dateiName);
        }
        anz = pBLeitplanViewer->readGmlMultiFiles(vDateiNamen);

        BPlanGmldatei->Text = nullptr;
        GmlDateiName = nullptr;
      }

    meldung = "Anzahl der gelesenen Objekte: ";
    meldung = String::Concat(meldung, System::Convert::ToString(anz));
    meldungen->Text = meldung;

    if (anz == 0) return;

    displayGmlType();
    buttonAttributeBearbeiten->Enabled = true;
    buttonUeberpruefen->Enabled = true;
    menuItemPruefenXPlanGMLKonformitaet->Enabled = true;

    comboBPlan->Items->Clear();

    if (pBLeitplanViewer->isXPlanGML())
    {
      displayXPlanGMLPlanArt();
      enableCityGMLToolElements(false);
      enableXPLanGMLToolElements(true);

      anzPlaene = pBLeitplanViewer->getPlanNamen(vPlanNamen);

      for (i = 0; i < anzPlaene; i++)
      {
        bPlanName = vPlanNamen[i];
        if (bPlanName != "") STR = ToString(bPlanName);
        else		               STR = String::Concat("Plan Nr ", Convert::ToString(i));
        comboBPlan->Items->Add(STR);
      }

      if (anzPlaene > 0)
      {
        comboBPlan->SelectedIndex = 0;
        pBLeitplanViewer->setPlanAktuell(0);

        anzPlanBereiche = pBLeitplanViewer->getPlanBereichNummern(0, vPlanBereichNummern);
        comboBPlanEbene->Items->Clear();

        for (i = 0; i < anzPlanBereiche; i++)
        {
          nummer = vPlanBereichNummern[i];
          comboBPlanEbene->Items->Add(Convert::ToString(nummer));
        }
        if (anzPlanBereiche > 0)
        {
          pBLeitplanViewer->setPlanBereichAktuell(vPlanBereichNummern[0]);
          comboBPlanEbene->SelectedIndex = 0;
        }
        else
        {
          pBLeitplanViewer->resetPlanBereichAktuell();
          comboBPlanEbene->SelectedIndex = -1;
        }
      }
    }
    else
      if (pBLeitplanViewer->isCityGML())
      {
        enableXPLanGMLToolElements(false);
        enableCityGMLToolElements(true);
      }
      else
        if (pBLeitplanViewer->isALKIS() || pBLeitplanViewer->isINSPIREPLU())
        {
          enableCityGMLToolElements(false);
          enableXPLanGMLToolElements(true);
        }
        else
          if (pBLeitplanViewer->isBoreholeML())
          {
            enableXPLanGMLToolElements(false);
            buttonShowSvg->Enabled = true;
            enableCityGMLToolElements(true);
          }
          else
            if (pBLeitplanViewer->isINSPIRE())
            {
              enableXPLanGMLToolElements(false);
              enableCityGMLToolElements(false);
            }
            else
            {
              enableXPLanGMLToolElements(false);
              enableCityGMLToolElements(false);

              buttonAttributeBearbeiten->Enabled = true;
              buttonShowSvg->Enabled = true;
              buttonKmlGenerieren->Enabled = true;
              comboBoxDarstellung->Enabled = true;
              comboBoxMassstab->Enabled = true;
            }
  }
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Zeigt bzw. verbirgt XPlanGML-spezifische Bedienelemente                  //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void enableXPLanGMLToolElements(bool enable)
{
  if (enable)
  {
    comboBPlan->Enabled = true;
    comboBPlanEbene->Enabled = true;
    checkBoxAllePlaene->Enabled = true;
    comboBoxDarstellung->Enabled = true;
    comboBoxMassstab->Enabled = true;
    menuItemGmlSpeichern->Enabled = true;
    menuItemGmlSpeichernUnter->Enabled = true;
    buttonAusgabeFolder->Enabled = true;
    menuItemAttributeBearbeiten->Enabled = true;
    if (visualization2D)
    {
      menuItemShowSvg->Enabled = true;
      buttonShowSvg->Enabled = true;
    }
  }
  else
  {
    comboBPlan->Enabled = false;
    comboBPlanEbene->Enabled = false;
    checkBoxAllePlaene->Enabled = false;
    comboBoxDarstellung->Enabled = false;
    comboBoxMassstab->Enabled = false;
    menuItemGmlSpeichern->Enabled = false;
    menuItemGmlSpeichernUnter->Enabled = false;
    buttonAusgabeFolder->Enabled = false;
    menuItemAttributeBearbeiten->Enabled = false;
    menuItemShowSvg->Enabled = false;
    buttonShowSvg->Enabled = false;
  }
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Zeigt bzw. verbirgt CityGML-spezifische Bedienelemente                   //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void enableCityGMLToolElements(bool enable)
{
  if (enable)
  {
    menuItemGmlSpeichern->Enabled = true;
    menuItemGmlSpeichernUnter->Enabled = true;
    buttonAusgabeFolder->Enabled = true;
    menuItemAttributeBearbeiten->Enabled = true;
    if (pBLeitplanViewer->getKMLViewer() != "")
      menuItemShowKml->Enabled = true;
    if (pBLeitplanViewer->getGMLViewer() != "")
      menuItemShowCityGML->Enabled = true;
    if (visualization3D)
      buttonKmlGenerieren->Enabled = true;
    comboBox3DViewer->Enabled = true;
  }
  else
  {
    menuItemGmlSpeichern->Enabled = false;
    menuItemGmlSpeichernUnter->Enabled = false;
    buttonAusgabeFolder->Enabled = false;
    menuItemAttributeBearbeiten->Enabled = false;
    menuItemShowKml->Enabled = false;
    buttonKmlGenerieren->Enabled = false;
    comboBox3DViewer->Enabled = false;
  }
}


         ///////////////////////////////////////////////////////////////////////////////
         //  Wird aufgerufen nach Auswahl eines Plans in der Combo-Box				         //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBPlan_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
  size_t      anzPlanBereiche, i;
  int         planIndex, nummer;
  string      stdString;
  vector<int> vPlanBereichNummern;

  planIndex = comboBPlan->SelectedIndex;
  pBLeitplanViewer->setPlanAktuell(planIndex);

  comboBPlanEbene->Items->Clear();

  anzPlanBereiche = pBLeitplanViewer->getPlanBereichNummern(planIndex, vPlanBereichNummern);
  for (i = 0; i < anzPlanBereiche; i++)
  {
    nummer = vPlanBereichNummern[i];
    comboBPlanEbene->Items->Add(Convert::ToString(nummer));
  }
  if (anzPlanBereiche > 0)
  {
    comboBPlanEbene->SelectedIndex = 0;
    nummer = Convert::ToInt32(comboBPlanEbene->SelectedItem);
    pBLeitplanViewer->setPlanBereichAktuell(nummer);
  }
  else
  {
    pBLeitplanViewer->resetPlanBereichAktuell();
    comboBPlanEbene->SelectedIndex = -1;
  }
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Wird aufgerufen nach Auswahl einer Ebene in der Combo-Box				         //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void comboBPlanEbene_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
  int      planBereichNummer;

  planBereichNummer = Convert::ToInt32(comboBPlanEbene->SelectedItem);
  pBLeitplanViewer->setPlanBereichAktuell(planBereichNummer);
}

         /////////////////////////////////////////////////////////////////////////////////////////////////
         //  Commadhandler für Button Konv                                                              //
         /////////////////////////////////////////////////////////////////////////////////////////////////
private: System::Void ButtonKonversion_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  menuItemShapefilesKonvertieren_Click(sender, e);
}

         //////////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Info                                                         //
         //////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem14_Click_1(System::Object ^  sender, System::EventArgs ^  e)
{
  Info ^ pInfoDialog = gcnew Info();

  pInfoDialog->ShowDialog();
}

         ////////////////////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Button 2D-Darstellung                                                  //
         ////////////////////////////////////////////////////////////////////////////////////////////
private: System::Void buttonShowSvg_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  try
  {
    if (m_iexplorerProcess != nullptr)
      m_iexplorerProcess->CloseMainWindow();
  }
  catch (System::InvalidOperationException^)
  {
  }

  m_iexplorerProcess = gcnew System::Diagnostics::Process();
  pBLeitplanViewer->visualisierung_2D(checkBoxAllePlaene->Checked, m_iexplorerProcess);
}

         ////////////////////////////////////////////////////////////////////////////////////////////
         //  Wird nach Auswahl einer neuen globalen Skalierung aufgerufen                          //
         ////////////////////////////////////////////////////////////////////////////////////////////
private: System::Void comboBox1_SelectedIndexChanged(System::Object ^  sender, System::EventArgs ^  e)
{
  double globalScale;
  int    index;

  index = comboBoxMassstab->SelectedIndex;
  switch (index)
  {
  case 0:		// 1 : 500
    globalScale = 0.25;
    break;

  case 1:		// 1 : 1.000
    globalScale = 0.5;
    break;

  case 2:		// 1 : 2.000
    globalScale = 1.0;
    break;

  case 3:		// 1 : 5.000
    globalScale = 2.5;
    break;

  case 4:		// 1 : 10.000
    globalScale = 5.0;
    break;

  case 5:		// 1 : 15.000
    globalScale = 7.5;
    break;

  case 6:		// 1 : 20.000
    globalScale = 10.0;
    break;

  case 7:		// 1 : 25.000
    globalScale = 12.5;
    break;

  case 8:		// 1 : 50.000
    globalScale = 25.0;
    break;

  case 9:		// 1 : 75.000
    globalScale = 37.5;
    break;

  case 10:		// 1 : 100.000
    globalScale = 50.0;
    break;
  }

  pBLeitplanViewer->setGlobalScale(globalScale);
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Extras --> Planzeichen-Liste Steuerdatei                  //
         //  Erzeugt eine Steuerdatei für eine Planzeichen-Liste                      //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem23_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  pBLeitplanViewer->planzeichenListeSteuerdatei();
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Extras --> Planzeichen-Liste erzeugen                     //      
         //  Erzeugt eine Planzeichen-Liste                                           //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem24_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  pBLeitplanViewer->planzeichenListe();
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Extras --> GML-IDs erzeugen                               //
         //  Alle GML-Ids neu erzeugen												                         //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem26_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  pBLeitplanViewer->updateGML_Ids();
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Button Prüfung                                            //
         //   Konformität testen																		                   //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonUeberpruefen_Click(System::Object ^  sender, System::EventArgs ^  e)
{
  pBLeitplanViewer->konformitaetspruefung(BPlanGmldatei->Text);
}


         ///////////////////////////////////////////////////////////////////////////////
         //  Anzeige des Typs der aktuellen GML-Datei                                 //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void displayGmlType()
{
  GML_SCHEMA_TYPE gmlType = pBLeitplanViewer->getGmlTyp();
  if (gmlType == GML_SCHEMA_UNKNOWN)
    return;

  String ^ schemaPath = ToString(pBLeitplanViewer->getSchemaPath());
  String ^ bitmapStr = String::Concat(schemaPath, "logo\\logo.jpg");

  String ^ gmlVersion = ToString(pBLeitplanViewer->getGmlVersion());
  textBoxXPlanGMLVersion->Text = gmlVersion;

  Bitmap ^ bitmap = gcnew Bitmap(bitmapStr);
  pictureBox1->SizeMode = PictureBoxSizeMode::CenterImage;
  pictureBox1->Image = bitmap;
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Anzeige des aktuellen XPlanGMLPlan-Art                                   //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void displayXPlanGMLPlanArt()
{
  switch (pBLeitplanViewer->getPlanTyp())
  {
  case BPLAN:
    textBoxPlanTyp->Text = "Bebauungsplan";
    break;

  case FPLAN:
    textBoxPlanTyp->Text = "Flächennutzungsplan";
    break;

  case RPLAN:
    textBoxPlanTyp->Text = "Regionalplan";
    break;

  case LPLAN:
    textBoxPlanTyp->Text = "Landschaftsplan";
    break;

  case SOPLAN:
    textBoxPlanTyp->Text = "Sonstiger Plan";
    break;

  default:
    textBoxPlanTyp->Text = "";
    break;
  }
}

         ////////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Codelisten --> Codelisten bearbeiten                       //
         ////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemCodelistOrdner_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->codelistenAendern();
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Callback für Schema-Validierung                                          //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void ValidationCallback(System::Object ^ sender, ValidationEventArgs ^ args)
{
  std::string errText;
  int         lineNumber;

  systemStr2stdStr(errText, args->Message);
  lineNumber = args->Exception->LineNumber;

  //  pBLeitplanViewer->addValidationError ( errText, lineNumber );
}

         ////////////////////////////////////////////////////////////////////////////////////////
         //  Wird aufgerufen nach Änderung des 2D-Visualisierungs-Typs                         //
         ////////////////////////////////////////////////////////////////////////////////////////
private: System::Void comboBoxDarstellung_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
  string darstellungsTyp;

  String ^ darstellungTypStr = dynamic_cast<String^>(comboBoxDarstellung->SelectedItem);
  systemStr2stdStr(darstellungsTyp, darstellungTypStr);

  pBLeitplanViewer->setDarstellungsTypFromString(darstellungsTyp);
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Menue Bearbeiten --> Attribute                            //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonAttributeBearbeiten_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->gmlAttribute();
}

         //////////////////////////////////////////////////////////////////////////////////////////
         //  Menuehandler für Generieren --> XPlanung aus Shapefile --> Shapefiles spezifizieren //
         //  Shapefiles für Konversion spezifizieren                                             //
         //////////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemShapefilesSpezifizieren_Click(System::Object^  sender, System::EventArgs^  e)
{
  buttonShapeDateien_Click(sender, e);
}

         /////////////////////////////////////////////////////////////////////////////////////////////
         //  Commandhandler für Menue Generieren --> XPlanung aus Shapefile --> Konversion erzeugen //
         //  Interaktive Erzeugung von Abbildungsvorschriften Shapefiles --> XPlanGML               //
         /////////////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemKonversionErzeugen_Click(System::Object^  sender, System::EventArgs^  e)
{
  bool         success;

  success = pBLeitplanViewer->generateShapefileConversion();
  if (success)
  {
    displayGmlType();
    displayXPlanGMLPlanArt();

    menuItemKonversionSpeichern->Enabled = true;
    menuItemKonversionSpeichernUnter->Enabled = true;
    menuItemKonversionBearbeiten->Enabled = true;
    menuItemKonversionSchliessen->Enabled = true;
    menuItemShapefilesKonvertieren->Enabled = true;
    ButtonKonversion->Enabled = true;
  }
}

         ////////////////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Menue Generieren --> XPlanung aus Shapefiles --> Konversion öffnen //
         //  Öffnen einer Konversionsdatei Shapefiles --> XPlanGML                             //
         ////////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemKonversionOeffnen_Click(System::Object^  sender, System::EventArgs^  e)
{
  buttonKonvDatei_Click(sender, e);
}

         ////////////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Generien --> XPlanung aus Shapefiles --> Konversion bearbeiten //
         //  Bearbeiten einer Konversionsdatei Shapefiles --> XPlanGML                     //
         ////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemKonversionBearbeiten_Click(System::Object^  sender, System::EventArgs^  e)
{
  bool weiter;
  int errorAnz = pBLeitplanViewer->editConversion();
  if (errorAnz > 0)
  {
    String ^ message = "Soll der Logfile angezeigt werden?";
    String ^ caption = "Fehler in Konversionsdatei";
    System::Windows::Forms::MessageBoxButtons buttons = System::Windows::Forms::MessageBoxButtons::YesNo;
    System::Windows::Forms::DialogResult result;

    result = MessageBox::Show(this, message, caption, buttons);
    if (result == System::Windows::Forms::DialogResult::Yes)
      pBLeitplanViewer->showLogfile();

    message = "Konversion fortsetzen?";
    result = MessageBox::Show(this, message, caption, buttons);
    if (result == System::Windows::Forms::DialogResult::Yes)
      weiter = true;
    else
      weiter = false;
  }
  else weiter = true;

  if (weiter)
  {
    bool success = pBLeitplanViewer->updateConversion();
    if (success)
      conversionSaved = false;
  }
}

         /////////////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Generieren --> XPlanung aus Shapefiles --> Konversion schließen //
         //  Schließen und Speichern einer Konversionsdatei Shapefiles --> XPlanGML         //
         /////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemKonversionSchliessen_Click(System::Object^  sender, System::EventArgs^  e)
{
  if (conversionSaved == false)
  {
    System::Windows::Forms::DialogResult result = System::Windows::Forms::MessageBox::Show("Soll die Konversion gesichert werden?",
      "Sichern einer Konv-Datei", System::Windows::Forms::MessageBoxButtons::YesNo);

    if (result == System::Windows::Forms::DialogResult::Yes)
    {
      menuItemKonversionSpeichern_Click(sender, e);
      conversionSaved = true;
    }
  }
}

         /////////////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Generieren --> XPlanung aus Shapefiles --> Konversion speichern //
         //  Speichern einer Konversionsdatei Shapefiles --> XPlanGML                       //
         /////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemKonversionSpeichern_Click(System::Object^  sender, System::EventArgs^  e)
{
  string   fileName = pBLeitplanViewer->getConversionFile();

  if (fileName == "") menuItemKonversionSpeichernUnter_Click(sender, e);
  else
  {
    String ^ Filename;
    String ^ BackupFileName;
    int      index;

    Filename = ToString(fileName);
    if (System::IO::File::Exists(Filename))
    {
      index = Filename->LastIndexOf(".");
      BackupFileName = Filename->Substring(0, index + 1);
      BackupFileName = String::Concat(BackupFileName, "bak");
      System::IO::FileInfo ^ pF = gcnew System::IO::FileInfo(Filename);
      pF->CopyTo(BackupFileName, true);
    }

    pBLeitplanViewer->saveConversion();
    conversionSaved = true;
  }
}

         ///////////////////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Generieren --> XPlanung aus Shapefiles --> Konversion speichern unter //
         //  Speichern einer Konversionsdatei Shapefiles --> XPlanGML                             //
         ///////////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemKonversionSpeichernUnter_Click(System::Object^  sender, System::EventArgs^  e)
{
  String ^ STR;
  string   fileName = pBLeitplanViewer->getConversionFile();

  if (fileName != "")
  {
    STR = ToString(fileName);
    saveConvDateiDialog->FileName = STR;
  }

  System::Windows::Forms::DialogResult ok = saveConvDateiDialog->ShowDialog();
  if (ok == System::Windows::Forms::DialogResult::OK)
  {
    String ^ Filename;
    String ^ BackupFileName;
    int      index;

    Filename = saveConvDateiDialog->FileName;
    if (System::IO::File::Exists(Filename))
    {
      index = Filename->LastIndexOf(".");
      BackupFileName = Filename->Substring(0, index + 1);
      BackupFileName = String::Concat(BackupFileName, "bak");
      System::IO::File::Copy(Filename, BackupFileName, true);
    }

    systemStr2stdStr(fileName, Filename);
    pBLeitplanViewer->saveConversionAs(fileName);
    conversionSaved = true;
  }
}

         ///////////////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Generieren --> XPlanung aus Shapefiles --> Shapefiles konvrtieren //
         //  Durchführung der Konversion Shapefiles --> XPlanGML                              //
         ///////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemShapefilesKonvertieren_Click(System::Object^  sender, System::EventArgs^  e)
{
  bool                success;
  size_t              i, anz, anzPlanBereiche;
  int                 nummer;
  int				          anzObjRead;
  int					        anzObjNotRead;
  int			        		anzShapeFileRead;
  int					        anzShapeFileNotRead;
  string              bPlanName;
  string              stdString;
  vector<string>      vPlanNamen;
  vector<int>         vPlanBereichNummern;
  System::String    ^ STR;

  setDaten(0, 0, 0, 0);
  Cursor = Cursors::WaitCursor;

  success = pBLeitplanViewer->convertShapefiles(anzObjRead, anzObjNotRead, anzShapeFileRead,
    anzShapeFileNotRead, meldungen->Text);
  setDaten(anzShapeFileRead, anzShapeFileNotRead, anzObjRead, anzObjNotRead);

  if (success)
  {
    menuItemGmlSpeichern->Enabled = true;
    buttonAusgabeFolder->Enabled = true;
    if (visualization2D)
    {
      buttonShowSvg->Enabled = true;
      menuItemShowSvg->Enabled = true;
    }

    buttonUeberpruefen->Enabled = true;
    menuItemPruefenXPlanGMLKonformitaet->Enabled = true;
    menuItemAttributeBearbeiten->Enabled = true;
    buttonAttributeBearbeiten->Enabled = true;

    comboBPlan->Items->Clear();
    anz = pBLeitplanViewer->getPlanNamen(vPlanNamen);
    for (i = 0; i < anz; i++)
    {
      bPlanName = vPlanNamen[i];
      if (bPlanName != "") STR = ToString(bPlanName);
      else		               STR = String::Concat("Plan Nr ", Convert::ToString(i));
      comboBPlan->Items->Add(STR);
    }

    if (anz > 0)
    {
      comboBPlan->SelectedIndex = 0;
      pBLeitplanViewer->setPlanAktuell(0);
      comboBPlanEbene->Items->Clear();
      anzPlanBereiche = pBLeitplanViewer->getPlanBereichNummern(0, vPlanBereichNummern);

      for (i = 0; i < anzPlanBereiche; i++)
      {
        nummer = vPlanBereichNummern[i];
        comboBPlanEbene->Items->Add(Convert::ToString(nummer));
      }

      if (anzPlanBereiche > 0)
      {
        comboBPlanEbene->SelectedIndex = 0;
        pBLeitplanViewer->setPlanBereichAktuell(0);
      }
      else
      {
        pBLeitplanViewer->resetPlanBereichAktuell();
        comboBPlanEbene->SelectedIndex = -1;
      }
    }
  }
  Cursor = Cursors::Default;
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler für Menue Datei-->Lesen und Button "GML einlesen --> Lesen//
         //////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemGmlLesen_Click(System::Object^  sender, System::EventArgs^  e)
{
  buttonBPlanGMLDatei_Click(sender, e);
}
         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler für Menu Datei-->Speichern                                //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemGmlSpeichern_Click(System::Object^  sender, System::EventArgs^  e)
{
  string   name;
  int      anz;
  String ^ meldung;

  if (GmlDateiName == nullptr)
    menuItemGmlSpeichernUnter_Click(sender, e);
  else
  {
    systemStr2stdStr(name, GmlDateiName);
    anz = pBLeitplanViewer->writeXPlanGML(name);

    meldung = "Anzahl der geschriebenen XPlanGML-Objekte: ";
    meldung = String::Concat(meldung, System::Convert::ToString(anz));
    meldungen->Text = meldung;
  }
}

         ///////////////////////////////////////////////////////////////////////////////
         //   Commandhandler für Menu Datei-->Speichern unter                         //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemGmlSpeichernUnter_Click(System::Object^  sender, System::EventArgs^  e)
{
  buttonAusgabeFolder_Click(sender, e);
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Menuehandler für Datei --> Logfile                                       //
         //  Anzeige des Logfiles                                                     //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemLogfile_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->showLogfile();
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Beenden des Programmes                                                   //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemBeenden_Click(System::Object^  sender, System::EventArgs^  e)
{
  buttonAbbrechen_Click(sender, e);
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Öffnen des Strukturdialogs                                               //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemAttributeBearbeiten_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->showEditFeatures();
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Button 3D-Darstellung                                     //
         //  Visualisierung mit Hilfe vom KML                                         //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void buttonKmlGenerieren_Click(System::Object^  sender, System::EventArgs^  e)
{
  if (m_visualization3DProcess != nullptr)
  {
    try
    {
      m_visualization3DProcess->CloseMainWindow();
      m_visualization3DProcess->Close();
    }
    catch (Exception ^)
    {
    }
  }
  else
  {
    m_visualization3DProcess = gcnew System::Diagnostics::Process();
  }

  pBLeitplanViewer->visualisierung_3D(BPlanGmldatei->Text, m_visualization3DProcess);
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Visualisieren --> Als SVG-Modell                          //                
         // Visualisierung mit Hilfe von SVG (Menuaufruf)                             //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemShowSvg_Click(System::Object^  sender, System::EventArgs^  e)
{
  buttonShowSvg_Click(sender, e);
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Visualisieren --> Als KML-Modell                          //
         // Visualisierung mit Hilfe von KML (Menuaufruf)                             //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemShowKml_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->setDarstellungsTyp3D(KML);
  buttonKmlGenerieren_Click(sender, e);
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Visualisieren --> Als CityGML-Modell                      //
         // Visualisierung mit Hilfe von KML (Menuaufruf)                             //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemShowCityGML_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->setDarstellungsTyp3D(GML);
  buttonKmlGenerieren_Click(sender, e);
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Menue Prüfen --> Modellprüfung                            //
         // Prüfung der XPlanGML-Konformität (Menuaufruf)                             //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemPruefenXPlanGMLKonformitaet_Click(System::Object^  sender, System::EventArgs^  e)
{
  buttonUeberpruefen_Click(sender, e);
}

         ////////////////////////////////////////////////////////////////////////////////////////
         // Commandhandler für Menue Generieren --> INSPIRE PLU aus XPlanung --> XPlanung 4.1  //
         // Transformation XPlanGML --> INSPIRE PLU                                            //
         ////////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItemGenerierenINSPIREPLU_Click(System::Object^  sender, System::EventArgs^  e)
{
  String ^ text = "Die verwendeten Transformationsregeln von XPlanung 4.1 nach INSPIRE PLU sind rein experimentell und werden nicht gepflegt.\r\n\r\n";
  text = String::Concat(text, "Vollständigkeit und Korrektheit der Transformation können nicht garantiert werden!");

  System::Windows::Forms::MessageBox::Show(text, "INSPIRE Erzeugung aus XPlanung Version 4.1",
    System::Windows::Forms::MessageBoxButtons::OK,
    System::Windows::Forms::MessageBoxIcon::Exclamation);

  pBLeitplanViewer->INSPIRE_Transformation("XPlanGML 4.1");

}

         ////////////////////////////////////////////////////////////////////////////////////////
         // Commandhandler für Menue Generieren --> INSPIRE PLU aus XPlanung --> XPlanung 4.1  //
         // Transformation XPlanGML --> INSPIRE PLU                                            //
         ////////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuINSPIREPLU_XPlanung_5_0_Click(System::Object^  sender, System::EventArgs^  e)
{
  String ^ text = "Aktuell werden nur XPlanung Dokumente im Profil \"Raster-Umring\" vollständig nach INSPIRE PLU transformiert";

  System::Windows::Forms::MessageBox::Show(text, "INSPIRE Erzeugung aus XPlanung Version 5.0",
    System::Windows::Forms::MessageBoxButtons::OK,
    System::Windows::Forms::MessageBoxIcon::Exclamation);

  pBLeitplanViewer->INSPIRE_Transformation("XPlanGML 5.0");
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler Extras --> Optionen                                       //
         //  Aufruf eines Options-Dialogs                                             //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem17_Click(System::Object^  sender, System::EventArgs^  e)
{
  Options1 ^ pDialog = gcnew Options1(pBLeitplanViewer);
  pDialog->ShowDialog();
}

         ///////////////////////////////////////////////////////////////////////////////
         // Menuehandler für Datei --> Transformation Referenzsystem                  //
         //  Transformation Koordinaten-Referenzsystemm                               //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void coordSystemTransformation_Click(System::Object^  sender, System::EventArgs^  e)
{
  string fileName;
  systemStr2stdStr(fileName, BPlanGmldatei->Text);
  pBLeitplanViewer->transformationReferenzsystem(fileName, "");
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Menuehandler für Datei --> Transformation 325833 -> 25833                //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem325833To25833_Click(System::Object^  sender, System::EventArgs^  e)
{
  string fileName;
  systemStr2stdStr(fileName, BPlanGmldatei->Text);
  pBLeitplanViewer->transformationReferenzsystem(fileName, "EPSG:25833");
}

         ///////////////////////////////////////////////////////////////////////////////
         //  Commandhandler für Menue Korrektur --> Bodenplatte                       //
         ///////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem44_Click(System::Object^  sender, System::EventArgs^  e)
{
}

         ////////////////////////////////////////////////////////////////////////////////
         //  Wird aufgerufen, wenn der 3D-Viewer geändert wird                         //
         ////////////////////////////////////////////////////////////////////////////////
private: System::Void comboBox3DViewer_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
  if (comboBox3DViewer->SelectedIndex == 0)
    pBLeitplanViewer->setDarstellungsTyp3D(KML);
  else
    if (comboBox3DViewer->SelectedIndex == 1)
      pBLeitplanViewer->setDarstellungsTyp3D(GML);
}

         ///////////////////////////////////////////////////////////////////////////////////////
         //  Umwandlung std::string --> String ^                                              //
         ///////////////////////////////////////////////////////////////////////////////////////
private: String ^ ToString(std::string str)
{
  System::String ^ systemStr = gcnew System::String(str.c_str());
  return(systemStr);
}

         ///////////////////////////////////////////////////////////////////////////////////////
         //  Umwandlung String ^ --> std::string                                              //
         ///////////////////////////////////////////////////////////////////////////////////////
private: void systemStr2stdStr(std::string& toStr, System::String ^ fromStr)
{
  if (!fromStr)
  {
    toStr.clear();
    return;
  }
  IntPtr intPtr = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(fromStr);
  char* pStr;

  void* p = intPtr.ToPointer();
  pStr = (char*)p;
  toStr = pStr;

  System::Runtime::InteropServices::Marshal::FreeCoTaskMem(intPtr);
}

         ///////////////////////////////////////////////////////////////////////////////////////
         //  Migration XPlanGML 4.0/4.1 --> 5.0                                               //
         ///////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem15_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->migrationXPlanGML(BPlanGmldatei->Text, "4.x,", "5.0", MigrationStylesheet);
}

         ///////////////////////////////////////////////////////////////////////////////////////
         //  Migration XPlanGML 4.0/4.1 --> 5.1                                               //
         ///////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem33_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->migrationXPlanGML(BPlanGmldatei->Text, "4.x", "5.1", MigrationStylesheet);
}

         ///////////////////////////////////////////////////////////////////////////////////////
         //  Migration XPlanGML 4.0/4.1 --> 5.2                                               //
         ///////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem38_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->migrationXPlanGML(BPlanGmldatei->Text, "4.x", "5.2", MigrationStylesheet);
}

         ///////////////////////////////////////////////////////////////////////////////////////
         //  Migration XPlanGML 5.0 --> 5.1                                                   //
         ///////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem65_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->migrationXPlanGML(BPlanGmldatei->Text, "5.0", "5.1", MigrationStylesheet);
}

         ///////////////////////////////////////////////////////////////////////////////////////
         //  Migration XPlanGML 5.0 --> 5.2                                                   //
         ///////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem66_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->migrationXPlanGML(BPlanGmldatei->Text, "5.0", "5.2", MigrationStylesheet);
}

         ///////////////////////////////////////////////////////////////////////////////////////
         //  Migration XPlanGML 3.0 --> 4.0                                                   //
         ///////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem67_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->migrationXPlanGML(BPlanGmldatei->Text, "3.0", "4.0", MigrationStylesheet);
}

         ///////////////////////////////////////////////////////////////////////////////////////
         //  Migration XPlanGML 3.0 --> 4.1                                                   //
         ///////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem68_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->migrationXPlanGML(BPlanGmldatei->Text, "3.1", "4.1", MigrationStylesheet);
}
         ///////////////////////////////////////////////////////////////////////////////////////
         //  Öffnet das Benutzerhandbuch mit pdf-Viewer                                       //
         ///////////////////////////////////////////////////////////////////////////////////////
private: System::Void menuItem27_Click(System::Object^  sender, System::EventArgs^  e)
{
  pBLeitplanViewer->showManual();
}
};


