#pragma once

#include "includes.h"
#include "SrsManagerRef.h"
#include "Feature.h"

class GmlModelStatistics;
class GmlClassStatistics;
class GmlAttributeStatistics;
///////////////////////////////////////////////////////////////////////////////
//  GmlModelStatisticsGeneration                                             //
//  Erzeugt eine Analyse des übergebenen GML-Instanzmodells und schreibt die //
//  Ergebnisse als XML-Dokument gemäß Schema-File Testing.xsd heraus         //
///////////////////////////////////////////////////////////////////////////////
ref class GmlModelStatisticsGeneration
{
  public:
    GmlModelStatisticsGeneration ( GmlModelStatistics * pModelStatisticsP );
    virtual ~ GmlModelStatisticsGeneration( );

  private:
    Features                   * pGmlModel;        //  Aggregation aller Features des GML-Instanzmodells
    System::Xml::XmlTextWriter ^ pWriter ;         // .NET XML-Writer
    System::DateTime           ^ analysisTime;     // Zeitpunkt, an dem die Analyse durchgeführt wurde
    GmlModelStatistics         * pModelStatistics; // Generierte Modellstatistik
    bool                         fullStatistics;   // Steuert die Erzeugung einer ausführlichen Statistik
    bool                         generateXML;      // Bei true wird die Statistik in XML-Format exportiert

  public:
    void setFullStatistics ( bool b )  {  fullStatistics = b;  }

    void generateStatistics ( Features * pGmlModelP, std::string outputFile );
    void generateStatistics (  Features * pGmlModelP, System::Xml::XmlTextWriter ^ pWriterP );
    void generateStatistics (  Features * pGmlModelP );

  private:
    void generateStatistics();
    void writeBoundingBox ( double sw[], double no[], std::string srsName, int srsDimension );
    void writeGmlNamespaces ( GmlNamespaces * pGmlNamespaces );
    void writeClassStatistics ( std::string className, std::vector<Feature*> vFeatures, int &anzFeatRaumbez, int &anzFeatNRaumbez );
};

///////////////////////////////////////////////////////////////////////////////
//  GmlModelStatistics                                                       //
//  Speichert statistische Informationen über ein GML-Modell                 //
///////////////////////////////////////////////////////////////////////////////
class GmlModelStatistics
{
  public:
    GmlModelStatistics ();
    virtual ~ GmlModelStatistics();

  private:
    double bbMin[3];          // Untere, linke Ecke der Bounding Box des Modelles
    double bbMax[3];          // Obere, rechte Ecke der Bounding Box des Modelles
    bool   boundingBoxExist;  // Zeigt an, ob eine Bounding Box generiert wurde
    
    std::string  srsName;      // Name des Koordinaten-Referenzsystems des Modells
    int          srsDimension; // Raumdimension des Koordinaten-Referenzsystems

    int    anzFeaturesRaumbezogen;      // Anzahl der raumbezogenen Objekte
    int    anzFeaturesNichtRaumbezogen; // Anzahl der nicht-raumbezogenen Objekte

    std::vector<std::string> vNamespaces;         // Liste aller Namespaces (Langform) des Modells
    std::vector<std::string> vNamespaceKuerzel;   // Liste aller Namespace-Kürzel
    std::string              defaultNamespace;    // Default-Namespace

    std::vector<GmlClassStatistics*> vKlassen;    // Liste aller Klassen-Statistiken des Modells

  public:
    void setBoundingBox ( double bbMin[], double bbMax[] );
    bool getBoundingBox (  double bbMin[], double bbMax[] );

    void setSrsName ( std::string name )  {  srsName = name;  }
    std::string getSrsName()  {  return srsName;  }

    void setSrsDimension ( int dim )  {  srsDimension = dim;  }
    int  getSrsDimension()  {  return srsDimension;  }

    void setFeatureNumbers ( int anzRaumbez, int anzNRaumbez );
    void getFeatureNumbers ( int &anzRaumbez, int &anzNRaumbez );

    void addNamespace ( std::string text, std::string kuerzel );
		size_t  getAnzNamespaces()  {  return vNamespaces.size(); }
    std::string getNamespace ( unsigned int index );
    std::string getNamespaceKuerzel ( unsigned int index );

    void setDefaultNamespace ( std::string text )  {  defaultNamespace = text;  }
    std::string getDefaultNamespace ()  {  return defaultNamespace;  }

    void addClass ( GmlClassStatistics * pClass );
		size_t getClassAnz()  {  return vKlassen.size();  }
    GmlClassStatistics * getClass ( unsigned int index );
};

///////////////////////////////////////////////////////////////////////////////
//  GmlClassStatistics                                                       //
//  Speichert statistische Informationen über die Verwendung einer Klasse    //
//  in einem Instanzmodell                                                   //
///////////////////////////////////////////////////////////////////////////////
class GmlClassStatistics
{
  public:
    GmlClassStatistics ( std::string klassenNameP, size_t anzP );
    virtual ~ GmlClassStatistics();

private:
  std::string klassenName;                            // Name der Klasse
	size_t      anz;                                    // Anzahl der Klassen-Instanzen im Modell
  std::vector<GmlAttributeStatistics*> vAttribute;    // Liste aller Attribut-Statistiken dieser Klasse

  public:
    std::string getKlassenName()  {  return klassenName;  }
    size_t      getAnzahl()       {  return anz;  }

    void addAtribute ( GmlAttributeStatistics * pAttribute );
		size_t getAttributeAnz()  {  return vAttribute.size();  }
    GmlAttributeStatistics * getAttribute ( unsigned  int index );
};

///////////////////////////////////////////////////////////////////////////////
//  GmlAttributeStatistics                                                   //
//  Speichert statistische Informationen über die Verwendung eines           //
//  Attributes oder einer Relation innerhalb einer Klasse                    //
///////////////////////////////////////////////////////////////////////////////
class GmlAttributeStatistics
{
  public:
    GmlAttributeStatistics ( std::string attributNameP, int anzObjekteP, int AnzVerwendungenP );
    virtual ~ GmlAttributeStatistics();

  private:
    std::string attributName;       // Name des Attributs
    int         anzObjekte;         // Anzahl der Klassen-Instanzen, die dies Attribut verwenden
    int         anzVerwendungen;    // Gesamt-Anzahl der Verwendungen dieses Attributs

  public:
    std::string getAttributName()     {  return attributName;     }
    int         getAnzObjekte()       {  return anzObjekte;       }
    int         getAnzVerwendungen()  {  return anzVerwendungen;  }
};
