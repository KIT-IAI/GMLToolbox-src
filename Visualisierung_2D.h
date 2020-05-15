#pragma once

class Checking;
class Features;
class PlanBereich;
class BLeitPlan;
class SvgVisualisierung;

///////////////////////////////////////////////////////////////////////////////
//  Visualisierung_2D                                                        //
//  Visualisierung von 2D Geodaten                                           //
///////////////////////////////////////////////////////////////////////////////
class Visualisierung_2D
{
public:
  Visualisierung_2D ( Features * pFeatures, Checking * pChecking, 
           float planausgabeBreite, float planausgabeHoehe, double globalScale  );
  virtual ~Visualisierung_2D();

protected:
  Features          * m_pFeatures;          // GML-Objekte
  Checking          * m_pChecking;          // Prüfungs-Modul
  SvgVisualisierung * m_pSvgParams;         // SVG-Darstellungsparameter
  BLeitPlan         * m_pPlanAktuell;       // Bei XPlanGL: Aktuell gewählter Plan
  PlanBereich       * m_pPlanBereichAktuell;// Bei XPlanGML: Aktuell bewählter Bereich
  float               m_planausgabeBreite;  // Breite des Ausgabe-Bereiches
  float               m_planausgabeHoehe;   // Höhe des Ausgabe-Bereiches
  double              m_globalScale;        // Skalierungsfaktor für Anpassung der Signaturen an 
                                            // unterschiedliche Maßstäbe

  std::string  m_svgUeberschriftDatei;      // Dateiname Überschrift SVG-Datei
  std::string  m_svgUebersichtDatei;        // Dateiname SVG Übersichtskarte
  std::string  m_svgKartenDatei;            // Dateiname SVG-Plandarstellung
  bool         m_objectDataGenerated;       // Zeigt an, ob die html-Dateien der Sachdaten-Attribute generiert wurden
  double       m_delta;                     // Prozentualer Rand-Anteil in der SVG-Graphik

public:
  Features * getFeatures()  {  return m_pFeatures;  }
  Checking * getChecking()  {  return m_pChecking;  }

  void setPlanausgabeBreite ( float b )  {  m_planausgabeBreite = b;  }
  void setPlanausgabeHoehe ( float h )  {  m_planausgabeHoehe = h;  }
  void setGlobalScale ( double scaleP );

  void setPlanAktuell ( BLeitPlan * pPlan )  {  m_pPlanAktuell = pPlan;  }
  BLeitPlan * getPlanAktuell()  {  return m_pPlanAktuell;  }

  void setPlanBereichAktuell( PlanBereich * pPlanBereich )  {  m_pPlanBereichAktuell = pPlanBereich; }
  PlanBereich * getPlanBereichAktuell()  {  return m_pPlanBereichAktuell;  }

  SvgVisualisierung * getSvgParams();
  void resetSvgParams();
  std::string getSvgDatei()  {  return  m_svgKartenDatei;  }
  void writeStaticSVG ( std::string fileName, bool allePlaeneZeigen );
  bool writeInteractiveSVG ( int darstellungsTyp, bool allePlaeneZeigen );

  bool generateObjectData ( bool allePlaeneZeigen );
  void setObjectDataGenerated ( bool b )  {   m_objectDataGenerated = b;}
  bool getObjectDataGenerated ()  {  return  m_objectDataGenerated;  }

  double getDelta ()  {  return  m_delta;  }
};
