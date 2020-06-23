#include "stdafx.h"

#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <fstream>
#include <conio.h>

#include "Convert.h"
#include "Constant.h"
#include "Logfile.h"
#include "SrsManager.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "Geometrie.h"
#include "GMLReader.h"
#include "GmlSchema.h"
#include "GmlNamespaces.h"
#include "Checking.h"
#include "GMLWriter.h"
#include "HTMLDatei.h"
#include "UnitOfMeasure.h"
#include "Visualisierung_2D.h"
#include "TransformationINSPIRE_PLU.h"
#include "Visualisierung_2D.h"
#include "Visualisierung_3D.h"
#include "KmlErzeugungDialog.h"
#include "CoordinateTransformations.h"
#include "CoordSystemTransformation.h"
#include "StrukturDialog.h"
#include "KonvDateiNeu.h"
#include "PlanAttributeNeu.h"
#include "GenerateKonversion.h"
#include "ShapeFileConversion.h"
#include "XPlanGML_Migration.h"
#include "CodelistOrdnerAendernDialog.h"
#include "BoreholeSelection.h"
#include "KonformitaetsTest.h"
#include "CodelistOrdnerAendernDialog.h"
#include "KonformitaetsTest.h"
#include "GmlTypAuswahlDialog.h"
#include "SvgVisualisierung.h"
#include "EnumerationDictionaryGeneration.h"
#include "GmlToolbox.h"
#include "INSPIRECodeList.h"

using namespace std;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;
using namespace System::Collections;


///////////////////////////////////////////////////////////////////////////////
// Zentrales Interface zwischen der Benutzeroberfläche und den thematischen  //
//  Modulen               											                          	 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor und Destruktor												                       //
///////////////////////////////////////////////////////////////////////////////
GmlToolbox::GmlToolbox ( int anzObjMaxP )
{  
  m_anzObjMax = anzObjMaxP;

  m_pLogfile = new Logfile();

  m_pUOMList = new UOMList;
  readUOMList();

  m_pSrsManager = new SrsManager;
  m_pSrsManager->readCoordSystemList( Constant::getReferenceSystemListPath() );

  m_pFeatures = new Features( m_pUOMList, m_pSrsManager, m_pLogfile );

  m_pChecking = new Checking( m_pFeatures, m_pUOMList, m_pSrsManager, m_pLogfile );

  m_pVisualisierung_2D  = NULL;
  m_pVisualisierung_3D  = NULL;
  m_pXPlanGML           = NULL;

  m_readCodelistDictionaries = true;

  m_darstellungsTyp   = DARSTELLUNG_UNKNOWN;
  m_darstellungsTyp3D = DARSTELLUNG_UNKNOWN;

  m_KMLViewer = "";
  m_GMLViewer = "";
//  m_SVGViewer = "";
  m_showKMLFeatureData = true;

  m_planausgabeBreite = 0.4f;
  m_planausgabeHoehe  = 0.2f;
  m_globalScale       = 1.0;
  m_delta             = 0.0;
  m_zOffset           = 0.0;

  m_javaPath     = "";
  m_batikPath    = "";
  m_javaHeapSize = "";
}

GmlToolbox::~GmlToolbox()
{
	map<string,Feature*>::iterator iter;

  delete m_pLogfile;
  delete m_pChecking;
  delete m_pUOMList;
  delete m_pSrsManager;
  delete m_pFeatures;

  if ( m_pVisualisierung_2D != NULL ) delete m_pVisualisierung_2D;
  if ( m_pVisualisierung_3D != NULL ) delete m_pVisualisierung_3D;
  if ( m_pXPlanGML != NULL )          delete m_pXPlanGML;
}

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert bzw. liefert das XPlanGML Modul                            //
///////////////////////////////////////////////////////////////////////////////
XPlanGML * GmlToolbox::getXPlanGML()
{
  if ( m_pXPlanGML == NULL )
    m_pXPlanGML = new XPlanGML ( m_pFeatures, m_pUOMList, m_pLogfile );

  return m_pXPlanGML;
}

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert bzw. liefert das Modul zur Visualisierung von 2D-Geodaten  //
///////////////////////////////////////////////////////////////////////////////
Visualisierung_2D * GmlToolbox::getVisualisierung_2D()
{
  if ( m_pVisualisierung_2D == NULL )
    m_pVisualisierung_2D = new Visualisierung_2D ( m_pFeatures, m_pChecking, m_planausgabeBreite, m_planausgabeHoehe, m_globalScale );

  return m_pVisualisierung_2D;
}

///////////////////////////////////////////////////////////////////////////////
//  Initialisiert bzw. liefert das Modul zur Visualisierung von 3D-Geodaten  //
///////////////////////////////////////////////////////////////////////////////
Visualisierung_3D * GmlToolbox::getVisualisierung_3D()
{
  if ( m_pVisualisierung_3D == NULL )
    m_pVisualisierung_3D = new Visualisierung_3D ( m_pFeatures, m_zOffset );

  return m_pVisualisierung_3D;
}

/////////////////////////////////////////////////////////////////////////////////
//  Einlesen einer einzelnen GML-Datei													               //
//  Bei dazuladen == true wird der Datensatz zur vorhandener Szene hinzugefügt //
/////////////////////////////////////////////////////////////////////////////////
int GmlToolbox::readGML ( std::string dateiName, bool dazuladen )
{
	int anz;

  if ( dazuladen )
    m_pFeatures->reset();
  else
    m_pFeatures->clear();  

  if ( m_pVisualisierung_2D != NULL )
    m_pVisualisierung_2D->setObjectDataGenerated( false );

	GMLReader ^ pReader = gcnew GMLReader ( m_pFeatures, m_readCodelistDictionaries );
	anz = pReader->read ( dateiName, m_pChecking );

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen mehrerer GML-Dateien     													             //
///////////////////////////////////////////////////////////////////////////////
int GmlToolbox::readGmlMultiFiles ( std::vector<std::string> &vDateiNamen )
{
  int anz = 0;

  m_pFeatures->clear();      
  GMLReader ^ pReader = gcnew GMLReader ( m_pFeatures, m_readCodelistDictionaries );
  pReader->setGenerateFeatureDateienMap( true );

  for ( unsigned i = 0; i < vDateiNamen.size(); i++ ) 
  {
    string dateiName = vDateiNamen[i];

    m_pFeatures->reset();
    pReader->reset();
    int anzLoc = pReader->readWithoutModification ( dateiName, m_pChecking );
    anz = anz + anzLoc;
  } 
  pReader->modifyModel();
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Öffnet den Strukturdialog zur textuellen Anzeige und zum Editieren der   //
//  GML-Features                                                             //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::showEditFeatures()
{
  StrukturDialog ^ pDialog = gcnew StrukturDialog( m_pFeatures, m_pChecking );
  pDialog->Show();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt im Modul Visualisierung-2D den aktuell gewählten Plan              //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::setPlanAktuell ( int index )
{
  m_pVisualisierung_2D = getVisualisierung_2D();

  BLeitPlan * pPlan = m_pFeatures->getBLeitPlan( index );
  m_pVisualisierung_2D->setPlanAktuell( pPlan );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt im Modul Visualisierung-2D den aktuell gewählten Planbereich       //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::setPlanBereichAktuell ( int nummer )
{
  m_pVisualisierung_2D = getVisualisierung_2D();

  BLeitPlan * pPlan = m_pVisualisierung_2D->getPlanAktuell();
  if ( pPlan != NULL )
  {
    PlanBereich * pPlanBereich = pPlan->getPlanBereich( nummer );
    m_pVisualisierung_2D->setPlanBereichAktuell( pPlanBereich );
		m_pVisualisierung_2D->setObjectDataGenerated ( false );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt im Modul Visualisierung_2D den aktuell gewählten Planbereich auf   //
//  NULL                                                                     //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::resetPlanBereichAktuell()
{
  m_pVisualisierung_2D->setPlanBereichAktuell( NULL );
}


///////////////////////////////////////////////////////////////////////////////
//  Setzt den Ordner der Externe Codelisten für einen GML-Typ                //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::setExternalCodeListFolder ( GML_SCHEMA_TYPE gmlTyp, std::string folder )
{
  m_pFeatures->setExternalCodeListFolder( gmlTyp, folder );
}

///////////////////////////////////////////////////////////////////////////////
//  Exportiert den aktuell gewählten Plan als statische SVG-Datei            //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::writeStaticSVG ( std::string fileName )
{
  m_pVisualisierung_2D = getVisualisierung_2D();
  m_pVisualisierung_2D->writeStaticSVG( fileName, false );
}

///////////////////////////////////////////////////////////////////////////////
//  Exportiert die aktuell eingelesene CityGML Datei als KML                 //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::writeKML ( std::string fileName )
{
  m_pVisualisierung_3D = getVisualisierung_3D();
  KmlErzeugungDialog ^ pDialog = gcnew KmlErzeugungDialog ( m_pVisualisierung_3D, fileName, m_KMLViewer, 
    false, m_showKMLFeatureData );
  pDialog->ShowDialog();
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert das Koordinaten-Referenzsystem                             //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::transformationReferenzsystem( string fileName, string zielsystem )
{
  String ^ FILE_NAME;
  String ^ ZIELSYSTEM;

  FILE_NAME =   QuConvert::ToString( fileName );
  if ( zielsystem != "" )
    ZIELSYSTEM = QuConvert::ToString ( zielsystem );
  else
    ZIELSYSTEM = nullptr;

  CoordSystemTransformation ^ pTrans = gcnew CoordSystemTransformation ( m_pFeatures, FILE_NAME, ZIELSYSTEM );
  pTrans->ShowDialog();
}

///////////////////////////////////////////////////////////////////////////////
//  Zeigt den Logfile an                                                     //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::showLogfile()
{
  string   stdString;
  String ^ STR;

  stdString = Constant::getLogfilePath();
  STR = QuConvert::ToString ( stdString );

  System::Diagnostics::Process::Start ( STR );
}

///////////////////////////////////////////////////////////////////////////////
//  Zeigt das Benutzerhandbuch an                                            //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::showManual()
{
  string   stdString;
  String ^ STR;

  try
  {
    stdString = Constant::getManualPath();
    STR = QuConvert::ToString ( stdString );

    System::Diagnostics::Process::Start ( STR );
  }
  catch ( Exception ^)
  {
    MessageBox::Show ( "Bitte öffnen Sie die Datei Benutzerhandbuch.pdf im Systemverzeichnis" );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Öffnet einen Dialog zum Anzeigen und zur Bearbeitung der                 //
//  Feature-Attribute                                                        //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::gmlAttribute()
{
  StrukturDialog ^ pDialog = gcnew StrukturDialog( m_pFeatures, m_pChecking );
  pDialog->Show();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den aktuellen GML-Typ der eingelesenen Daten                     //
///////////////////////////////////////////////////////////////////////////////
GML_SCHEMA_TYPE GmlToolbox::getGmlTyp() 
{  
  return m_pFeatures->getGmlTyp();  
}

bool GmlToolbox::isXPlanGML()       {  return m_pFeatures->isXPlanGML();    }
bool GmlToolbox::isINSPIREPLU()     {  return m_pFeatures->isINSPIREPLU();  }
bool GmlToolbox::isINSPIRE()        {  return m_pFeatures->isINSPIRE();     }
bool GmlToolbox::isCityGML()        {  return m_pFeatures->isCityGML();     }
bool GmlToolbox::isALKIS()          {  return m_pFeatures->isALKIS();       }
bool GmlToolbox::isBoreholeML()     {  return m_pFeatures->isBoreholeML();  }

///////////////////////////////////////////////////////////////////////////////
//  Liefert des System-Folder                                                //
///////////////////////////////////////////////////////////////////////////////
std::string GmlToolbox::getSystemFolder()
{
  return Constant::getSystemFolder();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schema-Pfad                                                  //
///////////////////////////////////////////////////////////////////////////////
std::string GmlToolbox::getSchemaPath()
{
  GML_SCHEMA_TYPE type = m_pFeatures->getGmlTyp();
  return Constant::getSchemaPath( type );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die GML-Version                                                  //
///////////////////////////////////////////////////////////////////////////////
std::string GmlToolbox::getGmlVersion()
{
  GML_SCHEMA_TYPE type = m_pFeatures->getGmlTyp();
  return Constant::getGMLSchemaTypeAsString( type );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den aktuellen Plan-Typ für die Konversion Shapefile --> XPlanGML //
///////////////////////////////////////////////////////////////////////////////
XPLAN_TYP GmlToolbox::getPlanTyp()
{
  m_pXPlanGML = getXPlanGML();
  return m_pXPlanGML->getPlanTyp();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur für XPlanGML: Liefert die Namen aller eingelesenen Plan-Objekte      //
///////////////////////////////////////////////////////////////////////////////
size_t GmlToolbox::getPlanNamen ( std::vector<string> & vPlanNamen )
{  
  string planName;

  for ( int i = 0; i < m_pFeatures->getPlanAnz(); ++i )
  {
    BLeitPlan * pPlan = m_pFeatures->getBLeitPlan ( i );
    bool success = pPlan->getStringAttributWert ( "xplan:name", planName );
    if ( !success )
      planName = "";
    vPlanNamen.push_back( planName );
  }

  return  m_pFeatures->getPlanAnz();  
}

///////////////////////////////////////////////////////////////////////////////
//  Nur für XPlanGML: Liefert die Bereichs-Nummern aller Planbereiche des    //
//  Plans an Position planIndex                                              //
///////////////////////////////////////////////////////////////////////////////
size_t GmlToolbox::getPlanBereichNummern ( int planIndex, vector<int> & vPlanBereichNummern )
{
  vector<PlanBereich*> vPlanBereiche;
  int                  bereichNummer;

  BLeitPlan * pPlan = m_pFeatures->getBLeitPlan ( planIndex );
  if ( pPlan == NULL )
    return 0;

	size_t anzPlanBereiche = pPlan->getPlanBereiche( vPlanBereiche );
  for ( size_t i = 0; i < anzPlanBereiche; ++i )
  {
    PlanBereich * pPlanBereich = vPlanBereiche[i];
    bool success = pPlanBereich->getIntegerAttributWert( "xplan:nummer", bereichNummer );
    if ( !success )
      bereichNummer = 0;
    vPlanBereichNummern.push_back( bereichNummer );
  }
  return anzPlanBereiche;
}

///////////////////////////////////////////////////////////////////////////////
//  Spezifiziert die Liste der Shapefiles, die nach XPlanGML konvertiert     //
//  werden                                                                   //
///////////////////////////////////////////////////////////////////////////////
bool GmlToolbox::specifyShapefiles ( std::vector<std::string> & vShapefiles, size_t & anzShapefiles, int &anzErrors )
{
  m_pXPlanGML = getXPlanGML();
  m_pXPlanGML->setShapeFiles ( vShapefiles );

  anzShapefiles = vShapefiles.size();

  bool success = m_pXPlanGML->analyzeShapeFiles( anzErrors );
  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der spezifizierten Shapefiles                         //
///////////////////////////////////////////////////////////////////////////////
size_t GmlToolbox::getShapefileAnz()
{
  m_pXPlanGML = getXPlanGML();
  return m_pXPlanGML->getShapefileAnz();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert, ob bereits eine Konversion Shapefile --> XPlanGML angelegt wurde//
///////////////////////////////////////////////////////////////////////////////
bool GmlToolbox::existConversion()
{
  m_pXPlanGML = getXPlanGML();
  return m_pXPlanGML->existConversion();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Pfadnamen der aktuellen Konvertierungsdatei                  //
///////////////////////////////////////////////////////////////////////////////
std::string GmlToolbox::getConversionFile()
{
  m_pXPlanGML = getXPlanGML();
  return m_pXPlanGML->getConversionFile();
}

///////////////////////////////////////////////////////////////////////////////
//  Sichert die aktuelle Konvertierung in der spezifizierten Datei           //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::saveConversion()
{
  m_pXPlanGML = getXPlanGML();
  return m_pXPlanGML->saveConversion();
}

///////////////////////////////////////////////////////////////////////////////
//  Sichert die aktuelle Konvertierung in einer neuen Datei                  //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::saveConversionAs( string fileName)
{
  m_pXPlanGML = getXPlanGML();
  return m_pXPlanGML->saveConversionAs( fileName );
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt eine neue Konversion Shapefile --> XPlanGML                      //
///////////////////////////////////////////////////////////////////////////////
bool GmlToolbox::generateShapefileConversion()
{
  GmlSchema        * pGmlSchma;
  ShapeFileObjekte * pShapeFileObjekte;
  PlanAttribute    * pPlanAttribute;
  bool               success = false;

  m_pXPlanGML = getXPlanGML();

  pShapeFileObjekte = m_pXPlanGML->getShapeFileObjekte();
  if (  pShapeFileObjekte == NULL )
    return false;

  KonvDateiNeu ^ pDialog = gcnew KonvDateiNeu;
  if ( pDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )
  {
    m_pFeatures->clear();

    pGmlSchma = new GmlSchema ( m_pFeatures );
    success = pGmlSchma->readGmlSchema( pDialog->gmlTyp, NULL );
    if ( success )
    {   
      m_pFeatures->setGmlSchema( pGmlSchma );
      m_pXPlanGML->setPlanTyp( pDialog->planTyp );

      pPlanAttribute =  m_pXPlanGML->createPlanAttribute();
      m_pFeatures->setSrsName ( "" );
      pPlanAttribute->addBereich ( "defaultBereich", "", "9999", "", -1 );

      success = updateConversion();      
    }
    else
      delete pGmlSchma;
  }
  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine neue Konvertierung nach Änderung der Abbildungsregeln     //
///////////////////////////////////////////////////////////////////////////////
bool GmlToolbox::updateConversion()
{
  bool success;
  m_pXPlanGML = getXPlanGML();

  GmlDictionary * pCodeLists = m_pFeatures->getGmlSchema()->getCodeListsDictionary();

  GenerateKonversion ^ pDialog = gcnew GenerateKonversion	( m_pXPlanGML, pCodeLists );
  System::Windows::Forms::DialogResult ok = pDialog->ShowDialog();
  if ( ok == System::Windows::Forms::DialogResult::OK )
    success = m_pXPlanGML->updateConversion();
  else
    success = false;
  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen einer Konvertierungsdatei Shapefiles --> XPlanGML               //
///////////////////////////////////////////////////////////////////////////////
bool GmlToolbox::openConversionFile( String ^ konvDateiName, std::string & meldungen )
{

  System::Windows::Forms::OpenFileDialog ^ konvDateiDialog = gcnew System::Windows::Forms::OpenFileDialog();
  konvDateiDialog->DefaultExt = L"*.xml";

  konvDateiName = "";
  meldungen     = "";

  if ( konvDateiDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
  {
    System::String ^ fileName;	
    System::String ^ pathName;
    string           stdString;
    bool             success;
    string           name;
    
    pathName = konvDateiDialog->FileName;
    int index = pathName->LastIndexOf ( "\\" );
    fileName = pathName->Substring ( index+1 );

    konvDateiName = fileName;
    m_pXPlanGML = getXPlanGML();

    QuConvert::systemStr2stdStr ( stdString, pathName );
    success = m_pXPlanGML->setConversionFile ( stdString );
    if ( !success ) 
    {
      meldungen = "Fehler beim Einlesen der Konvertierungs-Datei";
      konvDateiName = "";
      return false;
    }
    else
    {
      QuConvert::systemStr2stdStr( meldungen, konvDateiName );
      return true;    
    }
  }
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Editieren einer Konvertierungsdatei                                      //   
///////////////////////////////////////////////////////////////////////////////
int GmlToolbox::editConversion()
{
  m_pXPlanGML = getXPlanGML();
  ShapeFileNamenKonvertierung * pKonv = m_pXPlanGML->getKonv();
  if ( pKonv == NULL ) return 0;

  int errorAnz = pKonv->adaptShapeFileObjekte();
  return errorAnz;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt die Konvertierung Shapefiles --> XPlanGML durch                    //
///////////////////////////////////////////////////////////////////////////////
bool GmlToolbox::convertShapefiles ( int & anzObjRead, int & anzObjNotRead, int & anzShapefilesRead, 
                int & anzShapefilesNotRead, String ^ meldungen )
{
  m_pXPlanGML = getXPlanGML();

  anzObjRead           = 0;
  anzObjNotRead        = 0;
  anzShapefilesRead    = 0;
  anzShapefilesNotRead = 0;

  if ( !m_pXPlanGML->existConversion() )
  {
    meldungen = "Es wurde keine Konvertierungsdatei spezifiziert";
    return false;
  }

  if ( m_pXPlanGML->getShapefileAnz() == 0 )
  {
    meldungen = "Es wurden keine Shape-Dateien spezifiziert";
    return false;
  }

  string srsName = m_pFeatures->getSrsName();
  m_pFeatures->clear();
  m_pFeatures->setSrsName( srsName );

  bool success = m_pXPlanGML->convertShapeFiles();
  if ( !success )
  {
    meldungen = "Fehler bei der Shapefile Konvertierung";
    return false;
  }
  else
  {
    anzObjRead           = m_pXPlanGML->getAnzObjRead();
    anzObjNotRead        = m_pXPlanGML->getAnzObjNotRead();
    anzShapefilesRead    = m_pXPlanGML->getAnzShapeFileRead();
    anzShapefilesNotRead = m_pXPlanGML->getAnzShapeFileNotRead();
    return true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertiert eine XPlanGML-Datei nach INSPIRE PLU                         //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::INSPIRE_Transformation ( String ^ XPlanGMLVersion )
{
  m_pXPlanGML = getXPlanGML();
  m_pVisualisierung_2D = getVisualisierung_2D();

  TransformationINSPIRE_PLU_Dialog ^ pDialog = gcnew TransformationINSPIRE_PLU_Dialog ( m_pXPlanGML, 
                  m_pVisualisierung_2D, XPlanGMLVersion );
  pDialog->ShowDialog();
}

///////////////////////////////////////////////////////////////////////////////
//  Migration von XPlanGML                                                   //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::migrationXPlanGML ( String ^ inputFile, String ^ XPlanGMLVersionQuelle, 
        String ^ XPlanGMLVersionZiel, String ^ migrationsfehlerStylesheet )
{
  XPlanGML_Migration ^ pDialog;

  m_pXPlanGML = getXPlanGML();

  pDialog = gcnew XPlanGML_Migration ( m_pXPlanGML,  inputFile, XPlanGMLVersionQuelle, XPlanGMLVersionZiel,
						migrationsfehlerStylesheet );
  pDialog->ShowDialog();
}

///////////////////////////////////////////////////////////////////////////////
//  Codelisten ändern                                                        //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::codelistenAendern()
{
  CodelistOrdnerAendernDialog ^ pDialog = gcnew CodelistOrdnerAendernDialog ( m_pFeatures );
  pDialog->ShowDialog();
}

///////////////////////////////////////////////////////////////////////////////
//  Visualisierung der aktuellen GML-Daten mit SVG                           //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::visualisierung_2D( bool allePlaeneZeigen, System::Diagnostics::Process ^ iexplorerProcess )
{
  System::Diagnostics::ProcessStartInfo ^  processStartInfo;

  String	      ^ kartenDatei;
  string          stdString;
  string          svgDatei;
  bool            success;
  GML_SCHEMA_TYPE schemaTyp;

  if ( m_darstellungsTyp == KML &&  m_pFeatures->isBoreholeML() )
  {
    schemaTyp = BOREHOLEML_3_0;
    BoreholeSelection ^ pDialog = gcnew BoreholeSelection ( m_pFeatures );
    pDialog->ShowDialog();
    return;
  }
  else
  if ( m_pFeatures->isXPlanGML() || m_pFeatures->isINSPIREPLU() )
  {
    schemaTyp = m_pFeatures->getGmlTyp();
    m_pVisualisierung_2D = getVisualisierung_2D();

    if (m_darstellungsTyp == INTERAKTIVE_KARTE )
    {
      success = m_pVisualisierung_2D->writeInteractiveSVG ( m_darstellungsTyp, allePlaeneZeigen );
      if ( !success ) return;
    }
    else
    if ( m_darstellungsTyp == STATISCHE_KARTE )
    {
      SaveFileDialog ^ pDialog = gcnew SaveFileDialog;
      pDialog->DefaultExt  = "*.svg";
      pDialog->Filter = "svg-Datei|*.svg";
      System::Windows::Forms::DialogResult ok = pDialog->ShowDialog();
      if ( ok == System::Windows::Forms::DialogResult::OK )
      {
        QuConvert::systemStr2stdStr( svgDatei, pDialog->FileName );
        m_pVisualisierung_2D->writeStaticSVG ( svgDatei, allePlaeneZeigen );
      }

      return;
    }
  }
  else
  if ( m_pFeatures->isALKIS() )
  {
    m_pVisualisierung_2D = getVisualisierung_2D();

    SaveFileDialog ^ pDialog = gcnew SaveFileDialog;
    pDialog->Filter = "svg-Datei|*.svg";
    pDialog->DefaultExt  = "*.svg";
    System::Windows::Forms::DialogResult ok = pDialog->ShowDialog();
    if ( ok == System::Windows::Forms::DialogResult::OK )
    {
      QuConvert::systemStr2stdStr( svgDatei, pDialog->FileName );
      m_pVisualisierung_2D->writeStaticSVG ( svgDatei, true );
    }

    return;
  }

  kartenDatei = QuConvert::ToString(  Constant::getHTMLFramePath ( schemaTyp ) );

  try
  {
    processStartInfo = gcnew System::Diagnostics::ProcessStartInfo;

//    processStartInfo->FileName = QuConvert::ToString( m_SVGViewer );
//    processStartInfo->Arguments = kartenDatei;
		processStartInfo->FileName = kartenDatei;

		iexplorerProcess->StartInfo = processStartInfo;
    iexplorerProcess->Start();
 }

  catch ( Exception ^ )
  {
    MessageBox::Show ( "SVG-Viewer kann nicht gestartet werden" );
  }

}

///////////////////////////////////////////////////////////////////////////////
//  3D-Visualisierung der aktuellen GML-Daten                                //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::visualisierung_3D( String ^  fileName, System::Diagnostics::Process ^ visualization3DProcess )
{
  System::Diagnostics::ProcessStartInfo ^ pProcessStartInfo;

  m_pVisualisierung_3D = getVisualisierung_3D();

  if ( m_darstellungsTyp3D == KML )
  {
    string str;
    QuConvert::systemStr2stdStr( str, fileName );
    KmlErzeugungDialog ^ pDialog = gcnew KmlErzeugungDialog (  m_pVisualisierung_3D, str, m_KMLViewer, 
            true, m_showKMLFeatureData );
    pDialog->ShowDialog();    
  }
  else
  if ( m_darstellungsTyp3D == GML )
  {
    try
    {
      pProcessStartInfo = gcnew System::Diagnostics::ProcessStartInfo;
      pProcessStartInfo->FileName = QuConvert::ToString( m_GMLViewer );
      pProcessStartInfo->Arguments = String::Concat ( "\"", fileName, "\"" );

      visualization3DProcess->StartInfo = pProcessStartInfo;
      visualization3DProcess->Start();
    }

    catch ( Exception ^ )
    {
      MessageBox::Show ( "GML-Viewer kann nicht gestartet werden" );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Konformitätsprüfung                                                      //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::konformitaetspruefung( String ^ fileName )
{
  KonformitaetsTest ^ pKonformitaetsTestDialog;

  if ( m_pFeatures->getFeatureAnz() == 0 ) return;

  pKonformitaetsTestDialog = gcnew KonformitaetsTest (m_pFeatures, m_pChecking, QuConvert::ToString( m_GMLViewer ), 
              fileName );
  pKonformitaetsTestDialog->ShowDialog();
}

/////////////////////////////////////////////////////////////////////////////////
//  Erzeugt eine Steuerdatei für eine Planzeichenliste                         //
/////////////////////////////////////////////////////////////////////////////////
void GmlToolbox::planzeichenListeSteuerdatei()
{
  GmlTypAuswahlDialog ^ pGmlTypAuwahlDialog;
  GmlSchema           * pGmlObjekte;
  SvgVisualisierung   * pSvgParams;
  bool                  success;

  System::Windows::Forms::SaveFileDialog ^ generatePlanzListeDialog = gcnew System::Windows::Forms::SaveFileDialog();

  pGmlTypAuwahlDialog = gcnew GmlTypAuswahlDialog ( XPLANGML_5_1 );
  pGmlTypAuwahlDialog->ShowDialog();

  System::Windows::Forms::DialogResult ok = generatePlanzListeDialog->ShowDialog ();
  if (  ok == System::Windows::Forms::DialogResult::OK )
  {
    string ausgabeDatei;
    string svgParamDatei;

    m_pVisualisierung_2D = getVisualisierung_2D();

    pGmlObjekte = new GmlSchema ( m_pFeatures );
    success = pGmlObjekte->readGmlSchema( pGmlTypAuwahlDialog->typLocal, NULL );
    if ( success )
      m_pFeatures->setGmlSchema ( pGmlObjekte );
    else
    {
      delete pGmlObjekte;
      return;
    }

    pSvgParams = m_pVisualisierung_2D->getSvgParams();

    PlanzListeGeneration ^ pGen = gcnew PlanzListeGeneration ( pSvgParams );
    QuConvert::systemStr2stdStr ( ausgabeDatei, generatePlanzListeDialog->FileName );
    pGen->write ( ausgabeDatei );

    delete pGen;
  }
}

/////////////////////////////////////////////////////////////////////////////////
//  Erzeugt eine Planzeichenliste                                              //
/////////////////////////////////////////////////////////////////////////////////
void GmlToolbox::planzeichenListe()
{
  PlanzListe * pPlanzListe = NULL;
  string       stdString;

  System::Windows::Forms::OpenFileDialog ^ PlanzeichenListeDialog = gcnew System::Windows::Forms::OpenFileDialog();

  System::Windows::Forms::DialogResult ok = PlanzeichenListeDialog->ShowDialog ();
  if ( ok == System::Windows::Forms::DialogResult::OK )
  {
    m_pVisualisierung_2D = getVisualisierung_2D();

    pPlanzListe = new PlanzListe ( m_pVisualisierung_2D );

    pPlanzListe->setJavaPath ( m_javaPath );
    pPlanzListe->setBatikPath ( m_batikPath );
    pPlanzListe->setJavaHeapSize ( m_javaHeapSize );

    QuConvert::systemStr2stdStr ( stdString, PlanzeichenListeDialog->FileName );
    pPlanzListe->createPlanzeichen( stdString );

    delete pPlanzListe;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt für alle Objekte neue, eindeutige gml:id Parameter      				 //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::updateGML_Ids()
{
  System::Windows::Forms::DialogResult ok;

  ok = MessageBox::Show ( "Ok generiert alle gml:id Attribute neu", "", 
    System::Windows::Forms::MessageBoxButtons::OKCancel, System::Windows::Forms::MessageBoxIcon::Exclamation );

  if ( ok == System::Windows::Forms::DialogResult::OK )
  {
    if ( m_pFeatures != NULL ) m_pFeatures->updateGmlIds();
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die BPlanGML-Datei raus										                     //
///////////////////////////////////////////////////////////////////////////////
int GmlToolbox::writeXPlanGML( string ausgabePath )
{
  return writeXPlanGML( m_pFeatures, ausgabePath );
}

int GmlToolbox::writeXPlanGML ( Features * pPlanObjekteP, string ausgabePath )
{
 GMLWriter ^ pWriter = gcnew GMLWriter ( pPlanObjekteP );
 int         outputFormat;

 if ( pPlanObjekteP->getGmlSchema()->isInspirePlu() )
   outputFormat = 1;
 else
   outputFormat = 0;

  int anz = pWriter->write ( ausgabePath, outputFormat );
  delete pWriter;

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Hnadhabung von Units of Measurement                                      //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::readUOMList ()
{

  String        ^ pActElement;
  string          attributName;
  string          attributWert;
  string          str;
  double          h;
  UOM           * pUOM;
  bool            isEmptyElement;

  string fileName = Constant::getUOMListPath();

  System::Xml::XmlTextReader ^ pReader = gcnew System::Xml::XmlTextReader ( QuConvert::ToString ( fileName ) );
  while( pReader->Read() )
  {
    if ( pReader->NodeType == XmlNodeType::Element )
    {
      pActElement = pReader->LocalName;

      if ( pActElement->CompareTo( "description" ) == 0 )
      {
        QuConvert::systemStr2stdStr ( str, pReader->ReadString() );
        m_pUOMList->setDescription( str );
      }
      else
      if ( pActElement->CompareTo( "version" ) == 0 )
      {
        QuConvert::systemStr2stdStr ( str, pReader->ReadString() );
        m_pUOMList->setVersion( str );
      }
      else
      if ( pActElement->CompareTo( "UOM" ) == 0 )
      {
        if ( pReader->IsEmptyElement )
          isEmptyElement = true;
        else
          isEmptyElement = false;

        pUOM = new UOM();

        for ( int i = 0; i < pReader->AttributeCount; i++ )
        {
          pReader->MoveToAttribute ( i );

          QuConvert::systemStr2stdStr( attributName, pReader->Name );

          if ( attributName == "name" )
          {
            QuConvert::systemStr2stdStr( attributWert, pReader->Value );
            pUOM->setName( attributWert );
          }
          else
          if ( attributName == "id" )
          {
            QuConvert::systemStr2stdStr( attributWert, pReader->Value );
            pUOM->setIdentifier( attributWert );
          }
          else
          if ( attributName == "description" )
          {
            QuConvert::systemStr2stdStr( attributWert, pReader->Value );
            pUOM->setDescription( attributWert );
          }
          else
          if ( attributName == "quantity" )
           {
            QuConvert::systemStr2stdStr( attributWert, pReader->Value );
            pUOM->setQuantity( attributWert );
          }
          else
          if ( attributName == "sign" )
          {
            QuConvert::systemStr2stdStr( attributWert, pReader->Value );
            pUOM->setSign( attributWert );         
          }
          else
          if ( attributName == "factor" )
          {
            h = XmlConvert::ToDouble( pReader->Value );
            pUOM->setFactor( h );
          }
          else
          if ( attributName == "offset" )
          {
            h = XmlConvert::ToDouble( pReader->Value );
            pUOM->setOffset( h );
          }
        }
        if ( isEmptyElement )
          m_pUOMList->addUOM( pUOM );
      }
      else
      if ( pActElement->CompareTo( "altId" ) == 0 )
      {
        QuConvert::systemStr2stdStr ( attributWert, pReader->ReadString() );
        pUOM->addAlternativeIdentifier( attributWert );
      }
    }
    else
    if ( pReader->NodeType == XmlNodeType::EndElement )
    {
      pActElement = pReader->LocalName;

      if ( pActElement->CompareTo( "UOM" ) == 0 )
        m_pUOMList->addUOM( pUOM );
      else
      if ( pActElement->CompareTo( "UOMList" ) == 0 )
        break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die globale Skalierung bei der 2D-Planvisualisierung               //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::setGlobalScale ( double scale )
{
  m_globalScale = scale;

  if ( m_pVisualisierung_2D != NULL )
    m_pVisualisierung_2D->setGlobalScale( scale );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt den Typ der 2D- oder 3D-Darstellung auf Basis eines Textes         //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::setDarstellungsTypFromString ( std::string typ )
{
  if ( typ == "Interaktive Karte" )
    m_darstellungsTyp = INTERAKTIVE_KARTE;
  else
  if ( typ == "Statische Karte" )
    m_darstellungsTyp = STATISCHE_KARTE;
  else
  if ( typ == "KML" )
    m_darstellungsTyp = KML;
  else
  if ( typ == "GML" )
    m_darstellungsTyp = GML;
  else
    m_darstellungsTyp = KEINE;
}

///////////////////////////////////////////////////////////////////////////////
// Setzt verschiedene Parameter zur 3D-Visualisierung von Bohrlöchern        //
// (BoreholeML)                                                              //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::setBoreholeRadius ( double r )
{  
  m_pFeatures->setBoreholeRadius( r );  
}

void GmlToolbox::setBoreholeCylinderComplexity ( int c )  
{  
  m_pFeatures->setBoreholeCylinderComplexity( c );  
}

void GmlToolbox::setBoreholeShowIntervals ( bool b )  
{  
  m_pFeatures->setBoreholeShowIntervals( b );  
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Funktionsfähigkeit des Transformationsmoduls               //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::checkCoordTransformationModule()
{
//  SrsProjection::CheckCoordinateTransformModule();      
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt das GML-Dictionary der Enumerationen                             //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::generateEnumerationDictionaries()
{
  EnumerationDictionaryGeneration ^ pGen = gcnew EnumerationDictionaryGeneration ( m_pFeatures );
  pGen->ShowDialog();
}

///////////////////////////////////////////////////////////////////////////////
//  Öffnet den Dialog zum Anzeigen von Codelisten im INSPIRE Format          //
///////////////////////////////////////////////////////////////////////////////
void GmlToolbox::showINSPIRECodelist()
{
  INSPIRECodeList ^ pINSPIRECodelistDialog = gcnew INSPIRECodeList();
  pINSPIRECodelistDialog->ShowDialog();
}
