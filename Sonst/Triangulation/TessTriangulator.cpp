#include "StdAfx.h"


#include "TessTriangulator.hpp"

#include "..\Geometrie.h"
#include "..\Topologie.h"

//class GeoPoint;
class PolygonGeo;
class _Ring;


//#ifdef _DEBUG
//  #define new DEBUG_NEW
//  #undef THIS_FILE
//  static char THIS_FILE[] = __FILE__;
//#endif


//using namespace IfcDB;

typedef void(CALLBACK *TessDefaultCallback)();

//           DONUT
//        _.-------._
//      .'    ___    '.
//     /     (___)     \
//     |'._         _.'|
//     |   `'-----'`   |
//      \             /
//       '-.______..-'

void CALLBACK onBegin(GLenum type, void* ptr)
{
  // Begin PolygonGeo
  MyModeConverter* modeConverter = static_cast<MyModeConverter*>(ptr);
  modeConverter->onBegin(type);
}

void CALLBACK onEnd(void* ptr)
{
  // End PolygonGeo
  MyModeConverter* modeConverter = static_cast<MyModeConverter*>(ptr);
  // modeConverter->onEnd();
}

void CALLBACK onVertex(void* vertexData, void* ptr)
{
  MyModeConverter* modeConverter = static_cast<MyModeConverter*>(ptr);
  // Vertex data (Provided by glTessVertex)
  const X3D::Point3D* point = reinterpret_cast<const X3D::Point3D*>(vertexData);
  modeConverter->onIndex(point);
}

void CALLBACK onError(GLenum err, void* ptr)
{
  MyModeConverter* modeConverter = static_cast<MyModeConverter*>(ptr);
  modeConverter->onError(err);

  // GLU_TESS_MISSING_BEGIN_POLYGON
  // GLU_TESS_MISSING_END_POLYGON
  // GLU_TESS_MISSING_BEGIN_CONTOUR
  // GLU_TESS_MISSING_END_CONTOUR
  // GLU_TESS_COORD_TOO_LARGE
  // GLU_TESS_NEED_COMBINE_CALLBACK
}

void CALLBACK onCombineData(GLdouble coords[3], X3D::Point3D* vertex_data[4], GLfloat weight[4], X3D::Point3D** outData, void* ptr)
{
  MyModeConverter* modeConverter = static_cast<MyModeConverter*>(ptr);
  X3D::Point3D* pPoint = new X3D::Point3D(coords[0], coords[1], coords[2]);
  TessTriangulator* tessTriangulator = static_cast<TessTriangulator*>(modeConverter->getListener());
  tessTriangulator->addExtraPoint(pPoint);
  *outData = pPoint;
}

TessTriangulator::TessTriangulator()
: m_tess(0)
{
	m_kdTree = new KdTree();
  m_modeConverter.setListener(this);

  m_tess = gluNewTess();

  gluTessCallback(m_tess, GLU_TESS_BEGIN_DATA, TessDefaultCallback(&onBegin));
  gluTessCallback(m_tess, GLU_TESS_END_DATA, TessDefaultCallback(&onEnd));
  gluTessCallback(m_tess, GLU_TESS_VERTEX_DATA, TessDefaultCallback(&onVertex));
  gluTessCallback(m_tess, GLU_TESS_COMBINE_DATA, TessDefaultCallback(&onCombineData));
  gluTessCallback(m_tess, GLU_TESS_ERROR_DATA, TessDefaultCallback(&::onError));
}

TessTriangulator::~TessTriangulator()
{
  gluDeleteTess(m_tess);

  PointVector::iterator pointIt;
  for(pointIt = m_extraPoints.begin(); pointIt != m_extraPoints.end(); ++pointIt)
  {
    delete *pointIt;
  }
}

void TessTriangulator::triangulate( PolygonGeo * pPoly, bool ori )
{
	// TODO: Resize points to scale -1 to 1 but retain original coordinates for
	// the callback

	gluTessBeginPolygon(m_tess, &m_modeConverter);

	std::vector<double> vX;
	std::vector<double> vY;
	std::vector<double> vZ;
  
	Points       points;
  unsigned int pointCount;

  if ( ori == true)
	  pointCount = pPoly->getAussenkontur()->getPoints3D(vX, vY, vZ);
  else
    pointCount = pPoly->getAussenkontur()->getPoints3DReverse( vX, vY, vZ );

	for ( int i = 0; i < vX.size()-1; ++i )
	{
		X3D::Point3D* pGp = new X3D::Point3D(vX[i]*-1, vZ[i], vY[i]);
		points.push_back( pGp );
	}
  
	applyPoints(points);


	unsigned int lochAnz = pPoly->getLochAnz();

	for(int i = 0; i < lochAnz; ++i)
	{
    vX.clear();
    vY.clear();
    vZ.clear();
    points.clear();

    if ( ori == true )
      pointCount = pPoly->getLoch(i)->getPoints3D( vX, vY, vZ );
    else
      pointCount = pPoly->getLoch(i)->getPoints3DReverse( vX, vY, vZ );

		//const 6Points& points;

		for ( int j = 0; j < vX.size()-1; ++j )
		{
			X3D::Point3D* pGp = new X3D::Point3D( -vX[j], vZ[j], vY[j] );
			points.push_back( pGp );
		}
  
		applyPoints(points);
	}

	gluTessEndPolygon(m_tess);

	processErrors();

	m_errnos.clear();
}

void TessTriangulator::applyPoints(const Points& points)
{
  gluTessBeginContour(m_tess);

  for (size_t i(0); i < points.size(); ++i)
  {
    //const X3D::Point3D& point = points[i];
	X3D::Point3D* point = points[i];

    GLdouble vertex[3] = { point->getX(), point->getY(), point->getZ() };
    gluTessVertex(m_tess, vertex, points[i]);
	//gluTessVertex(m_tess, vertex, const_cast<X3D::Point3D*>(&points[i]));
  }

  gluTessEndContour(m_tess);
}

/*
void TessTriangulator::addPoint(X3D::Point3D* point)
{
	double poi [] = {point->getX(), point->getY(), point->getZ()};
	bool newNode;
				
	KdTreeNode* treeNode = getKdTree()->generateNode(poi, true, newNode);
				
	if(newNode)
	{
		addKdTreeNode(treeNode);
		addIndex(getPoints().size());
		addPoint3D(point);
	}
	else if(!newNode)
	{
		int index = findKdTreeNode(treeNode);
		addIndex(index);
	}
}
*/

int TessTriangulator::createIndex(const X3D::Point3D* i)
{
  auto it = m_indices.find(i);
  
  if (it == m_indices.end())
  {
    int index = m_vPoints.size();
    m_indices[i] = index;
    m_vPoints.push_back(const_cast<X3D::Point3D*>(i));

    return index;
  }
  else
  {
    return it->second;
  }
}

void TessTriangulator::onTriangle(const X3D::Point3D* i1, const X3D::Point3D* i2, const X3D::Point3D* i3)
{
// 	addPoint(const_cast<X3D::Point3D*>(i1));
// 	addPoint(const_cast<X3D::Point3D*>(i2));
// 	addPoint(const_cast<X3D::Point3D*>(i3));

  m_vCoordIndex.push_back(createIndex(i1));
  m_vCoordIndex.push_back(createIndex(i2));
  m_vCoordIndex.push_back(createIndex(i3));
  m_vCoordIndex.push_back(-1);
}

void TessTriangulator::onError(GLenum err)
{
  m_errnos.push_back(err);
}

void TessTriangulator::processErrors()
{
  for (std::vector<GLenum>::const_iterator it = m_errnos.begin(); it != m_errnos.end(); ++it)
  {
    switch (*it)
    {
      case GLU_TESS_MISSING_BEGIN_POLYGON:
      {
        break;
      }
      case GLU_TESS_MISSING_END_POLYGON:
      {
        break;
      }
      case GLU_TESS_MISSING_BEGIN_CONTOUR:
      {
        break;
      }
      case GLU_TESS_MISSING_END_CONTOUR:
      {
        break;
      }
      case GLU_TESS_COORD_TOO_LARGE:
      {
        throw std::exception("GLU_TESS_COORD_TOO_LARGE");
        break;
      }
      case GLU_TESS_NEED_COMBINE_CALLBACK:
      {
        throw std::exception("GLU_TESS_NEED_COMBINE_CALLBACK");
        break;
      }
      default:
      {
        throw std::exception("Unknown GLU error");
        break;
      }
    }
  }
}

void TessTriangulator::triangulatePolygons( std::vector<PolygonGeo*> &vPoly, std::vector<int>& vCoordIndex, std::vector<X3D::Point3D*>& vPoints, std::vector<bool> &oriVec )
{
  for ( unsigned int i = 0; i < vPoly.size(); i++ )
  {
    PolygonGeo * pPoly       = vPoly[i];
    bool         orientation = oriVec[i];

	  triangulate( pPoly, orientation );
  }

	vCoordIndex	= getCoordIndex();
	vPoints		  = getPoints();
}
