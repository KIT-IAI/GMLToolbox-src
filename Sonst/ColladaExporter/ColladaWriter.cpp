#include "StdAfx.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Windows::Forms;
using namespace System::Text;
using namespace System::Collections;

#include "KmlVisualisierung.h"

#include <fstream>
#include <col/all.hpp>

#include "ColladaConverter.h"

#include "ColladaWriter.h"

using namespace std;

ColladaWriter::ColladaWriter( Features * pFeaturesP, string gmlFolderP, string kmlFolderP, string kmlDateiNameP )
: pFeatures ( pFeaturesP ),
  kmlFolder ( kmlFolderP ),
  kmlDateiName ( kmlDateiNameP ),
  gmlFolder ( gmlFolderP )
{

}

void ColladaWriter::write( Feature * pFeature, KmlColor kmlColor, CITYGML_LOD lodStufe, string thema )
{
  double offset[3], no[3];
  string srsName;
  int    srsDimension;
  bool   success;
  string kmlPath;

  success = pFeature->getBoundingBox( offset, no, srsName, srsDimension );
  if ( !success ) return;

  if ( srsDimension == 2 )
  {
    offset[2] = 0.0;
    no[2]     = 0.0;
  }

  col::Context context;

  ColladaConverter converter( context, gmlFolder, kmlFolder );
    
  m_flattenHierarchy    = true;
  m_forceTriangulation  = true;
  m_ignoreSpaces        = true;
  m_writeSchemaLocation = false;


  converter.setForceTriangulation( m_forceTriangulation );
  converter.setFlattenHierarchy( m_flattenHierarchy );
//    visitor.setIgnoreSpaces( m_ignoreSpaces );
        
  converter.toCollada( pFeature, kmlColor, offset, lodStufe, thema );

  col::Writer writer(context);
  writer.setSchemaLocationEnabled( m_writeSchemaLocation );

  kmlPath = kmlFolder + "\\" + kmlDateiName;
  col::write( writer, kmlPath );
}

