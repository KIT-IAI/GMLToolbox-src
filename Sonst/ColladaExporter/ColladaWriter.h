#pragma once

#include "Feature.h"

class ColladaWriter
{
public:
  ColladaWriter( Features * pFeaturesP, std::string gmlFolderP, std::string kmlFolderP, std::string kmlDateiNameP );

  void write( Feature * pFeature, KmlColor pKmlColor, CITYGML_LOD lodStufe, std::string thema );

private:
    Features  * pFeatures;
    std::string gmlFolder;
    std::string kmlFolder;
    std::string kmlDateiName;

    bool m_flattenHierarchy;
    bool m_forceTriangulation;
    bool m_ignoreSpaces;
    bool m_writeSchemaLocation;
};