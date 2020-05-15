#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

#include "includes.h"
#include "Feature.h"
#include "SrsManager.h"
#include "Topologie.h"
#include "X3D-Module/X3D-Viewer.h"
#include "X3D-Module/Writable/X3D/Group.h"
#include "X3D-Module/Writable/X3D/X3DColor.h"
#include "X3D-Module/Writable/X3D\Point3D.h"

//using namespace std;

//class X3DVisualParams;
//class X3DKlassenVisualParams;
//class ColorWithTransparency;
//class X3DAdvanced;
class SchemaTypeNames;
class GmlDictionary;
class Features;
class GmlToolbox;
class Feature;
class GmlSchema;
//class Polygon;
//class _Curve;

///////////////////////////////////////////////////////////////////////////////
//  CityGML2X3D                                                              //
///////////////////////////////////////////////////////////////////////////////
namespace X3D
{
	class CityGML2X3D
	{
		public:
			CityGML2X3D (Features* features, CITYGML_LOD lodStufe, X3DViewer * viewer);
			virtual ~CityGML2X3D () {};

			bool DefineGMLExtend();

			void setSelectionAppearance();

			void iterFeatures(std::multimap<std::string,Feature*>& features, const std::string& parentID, std::list<std::string>& featureTypes);
      void getLineGeometry ( const std::vector<PolygonGeo*>& polVec, std::vector<int>& coordIndex, std::vector<Point3D*>& pointVec );
		
			std::string featureData ( Feature * pFeature, Group* parentGroup );

			void writeX3DViewer(const std::string fileName);
      void writeX3DFile (const std::string fileName);
	private:
			Features    *	m_features;
			GmlSchema   *	m_gmlObjects;
			CITYGML_LOD   m_lod;
			X3DViewer	  * m_x3dViewer;
	};
}
