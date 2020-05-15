#include "StdAfx.h"

#include "Convert.h"
#include "GmlSchema.h"
#include "GmlNamespaces.h"
#include "GmlModelTest.h"

///////////////////////////////////////////////////////////////////////////////
//  GmlModelStatisticsGeneration                                             //
//  Erzeugt eine Analyse des übergebenen GML-Instanzmodells und schreibt die //
//  Ergebnisse als XML-Dokument gemäß Schema-File Testing.xsd heraus         //
///////////////////////////////////////////////////////////////////////////////
GmlModelStatisticsGeneration::GmlModelStatisticsGeneration (  GmlModelStatistics * pModelStatisticsP )
{
  pGmlModel        = NULL;
  pWriter          = nullptr;
  pModelStatistics = pModelStatisticsP;

  fullStatistics   = true;
  generateXML      = true;
}

GmlModelStatisticsGeneration::~GmlModelStatisticsGeneration()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Statistik als separate XML-Datei heraus                     //
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatisticsGeneration::generateStatistics ( Features * pGmlModelP, std::string outputFileP )
{
  System::String ^ comment;

  pGmlModel   = pGmlModelP;

  generateXML = true;
  analysisTime = System::DateTime::Now;

  System::Text::UTF8Encoding ^ utf8	= gcnew  System::Text::UTF8Encoding();
  pWriter								            = gcnew System::Xml::XmlTextWriter( QuConvert::ToString( outputFileP ), utf8 );

  pWriter->Formatting = Formatting::Indented;
  pWriter->WriteStartDocument( true );

  comment = "Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: ";
  comment = String::Concat ( comment, analysisTime->ToString() );
  pWriter->WriteComment( comment );

  pWriter->WriteStartElement( "GmlDateiAnalyse" );
  pWriter->WriteAttributeString( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );

  generateStatistics();

  pWriter->WriteEndElement();


  pWriter->Flush();
  pWriter->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Statistik in eine vorhandene XML-Datei                      // 
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatisticsGeneration::generateStatistics (  Features * pGmlModelP, System::Xml::XmlTextWriter ^ pWriterP )
{
   pGmlModel    = pGmlModelP;
   pWriterP     = pWriterP;
   generateXML  = true;
   analysisTime = System::DateTime::Now;

   pWriter->WriteStartElement( "GmlDateiAnalyse" );
   generateStatistics();
   pWriter->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die Statistik nur im Hauptspeicher                             //
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatisticsGeneration::generateStatistics( Features * pGmlModelP )
{
  pGmlModel    = pGmlModelP;
  generateXML  = false;
  analysisTime = System::DateTime::Now;

  generateStatistics();
}


///////////////////////////////////////////////////////////////////////////////
//  Generiert die Modell-Statistik                                           //
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatisticsGeneration::generateStatistics()
{
  double                    sw[3], no[3];
  string                    srsName;
  int                       srsDimension;
  int                       anzFeatures;
  int                       anzKlassen;
  int                       anzFeatRaumbez, anzFeatNRaumbez, anzFeatRaumbezLoc, anzFeatNRaumbezLoc;
  bool                      success;
  multimap<string,Feature*> mFeatureMap;
  set<string>               sClassNameMap;
  set<string>::iterator     iterClassNames;

   //  Analysedatum
  if ( generateXML )
    pWriter->WriteElementString( "analyseDatum", analysisTime->ToString() );

  // Name der zu überprüfenden Datei
  if ( generateXML )
  {
    pWriter->WriteStartElement( "analyse" );
  }

  //  BoundingBox, srs-Name und srs-Dimension
  success = pGmlModel->getBoundingBox(  sw, no, srsName, srsDimension );
  if ( success )
  {
    pModelStatistics->setSrsDimension( srsDimension );
    pModelStatistics->setBoundingBox( sw, no );
    pModelStatistics->setSrsName( srsName );

    if ( generateXML )
      writeBoundingBox ( sw, no, srsName, srsDimension );
  }

  //  Namespaces, incl. Default-Namespace
  GmlSchema * pGmlSchema = pGmlModel->getGmlSchema();
  GmlNamespaces * pGmlNamespaces = pGmlSchema->getGmlNamespaces();
  writeGmlNamespaces ( pGmlNamespaces );
  
  //  Einzelklassen
  anzFeatures = (int)pGmlModel->getAllFeatures( mFeatureMap );
  anzFeatRaumbez  = 0;
  anzFeatNRaumbez = 0;

  anzKlassen  = (int)pGmlModel->getUsedClasses( sClassNameMap );

  for ( iterClassNames = sClassNameMap.begin(); iterClassNames != sClassNameMap.end(); iterClassNames++ )
  {
    multimap<string,Feature*>::iterator iterFeatureMap;
    vector<Feature*> vFeatures;

    string klassenName = *iterClassNames;
    vFeatures.clear();

    for ( iterFeatureMap = mFeatureMap.lower_bound( klassenName ); iterFeatureMap != mFeatureMap.upper_bound( klassenName ); iterFeatureMap++ )
    {
      vFeatures.push_back( iterFeatureMap->second );       
    }
    
    writeClassStatistics ( klassenName, vFeatures, anzFeatRaumbezLoc, anzFeatNRaumbezLoc );
    
    anzFeatRaumbez  = anzFeatRaumbez  + anzFeatRaumbezLoc;
    anzFeatNRaumbez = anzFeatNRaumbez + anzFeatNRaumbezLoc;
  }

  pModelStatistics->setFeatureNumbers( anzFeatRaumbez, anzFeatNRaumbez );

  if ( generateXML )
  {
    pWriter->WriteElementString( "anzFeat", System::Xml::XmlConvert::ToString( anzFeatures ) );
    pWriter->WriteElementString( "anzFeatRbez", System::Xml::XmlConvert::ToString( anzFeatRaumbez ) );
    pWriter->WriteElementString( "anzFeatNRbez", System::Xml::XmlConvert::ToString( anzFeatNRaumbez ) );

    pWriter->WriteEndElement();
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Angaben zur BoundingBox raus                                //
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatisticsGeneration::writeBoundingBox ( double sw[], double no[], std::string srsName, int srsDimension )
{
  String ^ STR;
  int      i;
  
  pWriter->WriteStartElement( "boundingBox" );

  STR = "";
  for ( i = 0; i < srsDimension; i++ )
    STR = String::Concat( STR, XmlConvert::ToString( sw[i]), " " );

  pWriter->WriteElementString( "lowerCorner", STR );

  STR = "";
  for ( i = 0; i < srsDimension; i++ )
    STR = String::Concat( STR, XmlConvert::ToString( no[i]), " " );

  pWriter->WriteElementString( "upperCorner", STR );

  pWriter->WriteEndElement();

  if ( srsName != "" )
    pWriter->WriteElementString( "srsName", QuConvert::ToString( srsName ) );

  pWriter->WriteElementString( "srsDimension", System::Xml::XmlConvert::ToString( srsDimension ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Angaben zu Namespaces raus                                  //
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatisticsGeneration::writeGmlNamespaces ( GmlNamespaces * pGmlNamespaces )
{
  vector<string> kuerzel;
  vector<string> namespaces;
  int            i, anz;
  string         defaultNamespace;

  anz =  (int)pGmlNamespaces->getNamespaces( kuerzel, namespaces );
  for ( i = 0; i < anz; i++ )
  {
    pModelStatistics->addNamespace( namespaces[i], kuerzel[i] );

    if ( generateXML )
    {
      pWriter->WriteStartElement( "namespace" );
      pWriter->WriteAttributeString ( "kuerzel", QuConvert::ToString( kuerzel[i]) );
      pWriter->WriteAttributeString ( "uri", QuConvert::ToString( namespaces[i]) );
      pWriter->WriteEndElement();
    }
  }

  defaultNamespace = pGmlNamespaces->getDefaultNamespace();
  if ( defaultNamespace != "" )
  {
    pModelStatistics->setDefaultNamespace( defaultNamespace );
    if ( generateXML )
      pWriter->WriteElementString ( "defaultNamespace", QuConvert::ToString( defaultNamespace ) );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt die Angaben für einzelne Klassen raus                           //
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatisticsGeneration::writeClassStatistics ( std::string className, std::vector<Feature*> vFeatures, int &anzFeatRaumbez, int &anzFeatNRaumbez )
{
  unsigned int i;

  std::map<std::string,vector<int>>           mPropertyStatistics;
  std::map<std::string,vector<int>>::iterator iterStatistics;
  
  anzFeatRaumbez = 0;
  anzFeatNRaumbez = 0;

  GmlClassStatistics * pClassStatistics = new GmlClassStatistics( className, vFeatures.size() );
  pModelStatistics->addClass( pClassStatistics );

  if ( generateXML )
  {
  pWriter->WriteStartElement( "klasse" );

  pWriter->WriteAttributeString( "name", QuConvert::ToString( className ) );
  pWriter->WriteAttributeString( "anzFeat", System::Xml::XmlConvert::ToString( vFeatures.size() ) );
  }

  for ( i = 0; i < vFeatures.size(); i++ )
  {
    Feature * pFeature = vFeatures[i];
    if ( pFeature->hasOwnGeometry() || pFeature->hasRelatedGeometry() )
      anzFeatRaumbez++;
    else
      anzFeatNRaumbez++;

    if ( fullStatistics )
      pFeature->getPropertyStatistics ( mPropertyStatistics );
  }

  for ( iterStatistics = mPropertyStatistics.begin(); iterStatistics != mPropertyStatistics.end(); iterStatistics++ )
  {
    string      propertyName = iterStatistics->first;
    vector<int> propertyAnz  = iterStatistics->second;

    GmlAttributeStatistics * pAttributeStatistics = new GmlAttributeStatistics ( propertyName, propertyAnz[1], propertyAnz[0] );
    pClassStatistics->addAtribute( pAttributeStatistics );

    if ( generateXML )
    {
      pWriter->WriteStartElement( "property" );
      pWriter->WriteAttributeString( "name", QuConvert::ToString ( propertyName ) );
      pWriter->WriteAttributeString ( "anzVerwendet",  System::Xml::XmlConvert::ToString( propertyAnz[0]) );
      pWriter->WriteAttributeString ( "anzFeat",  System::Xml::XmlConvert::ToString( propertyAnz[1]) );

      pWriter->WriteEndElement();
    }
  }

  if ( generateXML )
    pWriter->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  GmlModelStatistics                                                       //
//  Speichert statistische Informationen über ein GML-Modell                 //
///////////////////////////////////////////////////////////////////////////////
GmlModelStatistics::GmlModelStatistics ()
{
  for ( unsigned int i = 0; i < 3; i++ )
  {
    bbMin[i] = 0.0;
    bbMax[i] = 0.0;
  }
  boundingBoxExist = false;

  srsName      = "";
  srsDimension = -1;

  anzFeaturesRaumbezogen      = 0;
  anzFeaturesNichtRaumbezogen = 0;

  defaultNamespace = "";
}

GmlModelStatistics::~GmlModelStatistics()
{
  for ( unsigned int i = 0; i < vKlassen.size(); i++ )
    delete vKlassen[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt Informationen über die BoundingBox des Modells                     //
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatistics::setBoundingBox ( double bbMinP[], double bbMaxP[] )
{
  if ( srsDimension < 0  )
    return;

  for ( int i = 0; i < srsDimension; i++ )
  {
    bbMin[i] = bbMinP[i];
    bbMax[i] = bbMaxP[i];
  }
  boundingBoxExist = true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Informationen über die BoundingBox des Modells                   //
///////////////////////////////////////////////////////////////////////////////
bool GmlModelStatistics::getBoundingBox (  double bbMinP[], double bbMaxP[] )
{
  if ( !boundingBoxExist )
    return false;

  for ( int i = 0; i < 3; i++ )
  {
    bbMinP[i] = bbMin[i];
    bbMaxP[i] = bbMax[i];
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt Angaben über die Anzahl der Objekts                                //
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatistics::setFeatureNumbers ( int anzRaumbez, int anzNRaumbez )
{
  anzFeaturesRaumbezogen      = anzRaumbez;
  anzFeaturesNichtRaumbezogen = anzNRaumbez;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Angaben über die Anzahl der Objekte                              //
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatistics::getFeatureNumbers ( int &anzRaumbez, int &anzNRaumbez )
{
  anzRaumbez  =  anzFeaturesRaumbezogen;
  anzNRaumbez = anzFeaturesNichtRaumbezogen;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Informationen über einen Namespace hinzu                            //
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatistics::addNamespace ( std::string text, std::string kuerzel )
{
  vNamespaces.push_back( text );
  vNamespaceKuerzel.push_back( kuerzel );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Namespace (Langform) an einer bestimmten Indexposition       //
///////////////////////////////////////////////////////////////////////////////
std::string GmlModelStatistics::getNamespace ( unsigned int index )
{
  if ( index < 0 || index >= vNamespaces.size() )
    return "";
  else
    return vNamespaces[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Namespace (Kürzel) an einer bestimmten Indexposition         //
///////////////////////////////////////////////////////////////////////////////
std::string GmlModelStatistics::getNamespaceKuerzel ( unsigned int index )
{
  if ( index < 0 || index >= vNamespaces.size() )
    return "";
  else
    return vNamespaceKuerzel[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Klassen-Statistik hinzu                                             //
///////////////////////////////////////////////////////////////////////////////
void GmlModelStatistics::addClass ( GmlClassStatistics * pClass )
{
  vKlassen.push_back( pClass );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Klassen-Statistik                                                //
///////////////////////////////////////////////////////////////////////////////
GmlClassStatistics * GmlModelStatistics::getClass ( unsigned int index )
{
  if ( index < 0 || index >= vKlassen.size() )
    return NULL;
  else
    return vKlassen[index];

}

///////////////////////////////////////////////////////////////////////////////
//  GmlClassStatistics                                                       //
//  Speichert statistische Informationen über die Verwendung einer Klasse    //
//  in einem Instanzmodell                                                   //
///////////////////////////////////////////////////////////////////////////////
GmlClassStatistics::GmlClassStatistics ( std::string klassenNameP, size_t anzP )
{
  klassenName = klassenNameP;
  anz         = anzP;
}

GmlClassStatistics::~GmlClassStatistics()
{
  for ( unsigned int i = 0; i < vAttribute.size(); i++ )
    delete vAttribute[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Attribut-Statistik hinzu                                            //
///////////////////////////////////////////////////////////////////////////////
void GmlClassStatistics::addAtribute ( GmlAttributeStatistics * pAttribute )
{
  vAttribute.push_back( pAttribute );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Attribut-Statistik an einer bestimmten Inxexposition             //
///////////////////////////////////////////////////////////////////////////////
GmlAttributeStatistics * GmlClassStatistics::getAttribute ( unsigned int index )
{
  if ( index < 0 || index >= vAttribute.size() )
    return NULL;
  else
    return vAttribute[index];
}

///////////////////////////////////////////////////////////////////////////////
//  GmlAttributeStatistics                                                   //
//  Speichert statistische Informationen über die Verwendung eines           //
//  Attributes oder einer Relation innerhalb einer Klasse                    //
///////////////////////////////////////////////////////////////////////////////
GmlAttributeStatistics::GmlAttributeStatistics ( std::string attributNameP, int anzObjekteP, int anzVerwendungenP )
{
  attributName = attributNameP;
  anzObjekte   = anzObjekteP;
  anzVerwendungen = anzVerwendungenP;
}

GmlAttributeStatistics::~GmlAttributeStatistics()
{

}

