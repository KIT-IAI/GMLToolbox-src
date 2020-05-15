#pragma once

///////////////////////////////////////////////////////////////////////////////
//  Visualisierung_3D                                                        //
//  Visualisierung von 3D Geodaten                                           //
///////////////////////////////////////////////////////////////////////////////
class Visualisierung_3D
{
public:
  Visualisierung_3D ( Features * pFeatures, double zOffset );
  virtual ~Visualisierung_3D();

protected:
  Features         * m_pFeatures;
  KmlVisualParams  * m_pKmlParams;
  double             m_zOffset;

public:
  Features * getFeatures()  {  return m_pFeatures;  }
  KmlVisualParams * getKmlParams();
  void resetKmlParams();

  bool writeCityGMLToKML( std::string &fileName, CITYGML_LOD lodStufe, bool showFeatureData );
  bool writeBoreholeMLToKML( std::string &fileName, std::string geologThema, bool showFeatureData );

  void setZOffset ( double h )  {  m_zOffset = h;  }
  double getZOffset()  {  return m_zOffset;  }
};

