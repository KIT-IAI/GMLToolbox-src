#include "StdAfx.h"


#include "TessTriangulator.hpp"

#include "..\Geometrie.h"
#include "..\Logfile.h"
#include "..\Convert.h"
//#include "..\Topologie.h"

class GeoPoint;
class PolygonGeo;
class _Ring;


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
  const GeoPoint* point = reinterpret_cast<const GeoPoint*>(vertexData);
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

void CALLBACK onCombineData(GLdouble coords[3], GeoPoint* vertex_data[4], GLfloat weight[4], GeoPoint** outData, void* ptr)
{
  MyModeConverter* modeConverter = static_cast<MyModeConverter*>(ptr);
  GeoPoint* pPoint = new GeoPoint( coords[0], coords[1], coords[2]) ;
  TessTriangulator* tessTriangulator = static_cast<TessTriangulator*>(modeConverter->getListener());
  tessTriangulator->addExtraPoint(pPoint);
  *outData = pPoint;
}

TessTriangulator::TessTriangulator()
: m_tess(0)
{
  m_modeConverter.setListener(this);

  m_tess = gluNewTess();

//  string pathName = "C:\\Daten\\FZJ-CityGML-Daten\\Energiesimulation\\triangulationPoints.txt";
//  pLogfile = new LogfileLokal( pathName );
  pLogfile = NULL;


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

bool TessTriangulator::triangulate( PolygonGeo * pPoly, bool ori  )
{
  bool             success;
  std::string      outputLine;
  System::String ^ xSTR, ^ ySTR, ^ zSTR;
  std::string      xStr, yStr, zStr;
  Direction      * normal = NULL;

	gluTessBeginPolygon(m_tess, &m_modeConverter);

//	std::vector<double> vX;
//	std::vector<double> vY;
//	std::vector<double> vZ;
  
  std::vector<GeoPoint> originalPoints;
	Points                points;
  Points                pointsLoc;
  std::vector<Points>   vLochPoints;
	size_t                pointCount, i, j;

  if ( ori == true )
  {
    pointCount = pPoly->getAussenkontur()->getPoints( originalPoints );
    if ( pLogfile != NULL )
      normal = pPoly->getNormal();
  }
  else
  {
    pointCount = pPoly->getAussenkontur()->getPointsReverse( originalPoints );
    if ( pLogfile != NULL )
    {
      normal = pPoly->getNormal();
      normal->reverse();
    }
  }

  if ( pLogfile != NULL )
  {
    outputLine = "Polygon " + pPoly->getGmlId();
    pLogfile->writeLogFile ( outputLine );
  }

  _Curve::deleteCollinearPoints ( originalPoints );

	for ( i = 0; i <originalPoints.size(); ++i )
	{
		GeoPoint * pGp = new GeoPoint ( originalPoints[i] );
		points.push_back( pGp );

    if ( pLogfile != NULL )
    {
      xSTR = System::Convert::ToString ( originalPoints[i].getX() );
      ySTR = System::Convert::ToString ( originalPoints[i].getY() );
      zSTR = System::Convert::ToString ( originalPoints[i].getZ() );

      QuConvert::systemStr2stdStr ( xStr, xSTR );
      QuConvert::systemStr2stdStr ( yStr, ySTR );
      QuConvert::systemStr2stdStr ( zStr, zSTR );

      outputLine = xStr + ", " + yStr + ", " + zStr;
      pLogfile->writeLogFile ( outputLine );
    }
  }

  if (pLogfile != NULL )
  { 
    xSTR = System::Convert::ToString ( normal->getX() );
    ySTR = System::Convert::ToString ( normal->getY() );
    zSTR = System::Convert::ToString ( normal->getZ() );

    QuConvert::systemStr2stdStr ( xStr, xSTR );
    QuConvert::systemStr2stdStr ( yStr, ySTR );
    QuConvert::systemStr2stdStr ( zStr, zSTR );

    outputLine = "Normale: " + xStr + ", " + yStr + ", " + zStr;
    pLogfile->writeLogFile ( outputLine );
  }
  
	applyPoints(points);

	size_t lochAnz = pPoly->getLochAnz();

	for( i = 0; i < lochAnz; ++i)
	{
    originalPoints.clear(); 
    pointsLoc.clear();

    if ( ori == true )
      pointCount = pPoly->getLoch(i)->getPoints( originalPoints );
    else
      pointCount = pPoly->getLoch(i)->getPointsReverse( originalPoints );

    _Curve::deleteCollinearPoints ( originalPoints );

		for ( j = 0; j < originalPoints.size(); ++j )
		{
			GeoPoint * pGp = new GeoPoint(originalPoints[j] );
			pointsLoc.push_back( pGp );
		}
  
		applyPoints(pointsLoc);
    vLochPoints.push_back( pointsLoc );
	}

	gluTessEndPolygon(m_tess);

	success = processErrors();

  for ( i = 0; i < points.size(); i++ )
    delete points[i];

  for ( i = 0; i < vLochPoints.size(); i++ )
  {
    for ( j = 0; j < vLochPoints[i].size(); j++ )
      delete vLochPoints[i][j];
  }

	m_errnos.clear();
  return success;
}

bool TessTriangulator::triangulate( PolygonGeo * pPoly, std::vector<int>& vCoordIndex, std::vector<GeoPoint*>& vPoints, bool ori )
{
  bool success = triangulate( pPoly, ori );
  if (success )
  {
    vCoordIndex	= getCoordIndex();
    vPoints		  = getPoints();
  }

  return success;
}


void TessTriangulator::applyPoints(const Points& points)
{
  gluTessBeginContour(m_tess);

  for (size_t i(0); i < points.size(); ++i)
  {
	GeoPoint* point = points[i];

    GLdouble vertex[3] = { point->getX(), point->getY(), point->getZ() };
    gluTessVertex(m_tess, vertex, points[i]);
  }

  gluTessEndContour(m_tess);
}


void TessTriangulator::addPoint( GeoPoint * point)
{
	double poi [] = {point->getX(), point->getY(), point->getZ()};
    m_vCoordIndex.push_back( (int)getPoints().size() );
		addPoint3D( new GeoPoint ( point ) );

    if ( pLogfile != NULL )
    {
      String    ^ xSTR, ^ ySTR, ^ zSTR, ^ indexSTR;
      std::string xStr, yStr, zStr, indexStr;

      xSTR = System::Convert::ToString ( poi[0] );
      ySTR = System::Convert::ToString ( poi[1] );
      zSTR = System::Convert::ToString ( poi[2] );
      indexSTR = System::Convert::ToString ( m_vCoordIndex[m_vCoordIndex.size()-1] );

      QuConvert::systemStr2stdStr ( xStr, xSTR );
      QuConvert::systemStr2stdStr ( yStr, ySTR );
      QuConvert::systemStr2stdStr ( zStr, zSTR );
      QuConvert::systemStr2stdStr ( indexStr, indexSTR );


      string outputLine = indexStr + ": (" + xStr + ", " + yStr + ", " + zStr + ")";
      pLogfile->writeLogFile ( outputLine );
    }
}


void TessTriangulator::onTriangle(const GeoPoint* i1, const GeoPoint* i2, const GeoPoint* i3 )
{
  System::String ^ xSTR, ^ ySTR, ^ zSTR;
  std::string      xStr, yStr, zStr;

  addPoint(const_cast<GeoPoint*>(i1));
 	addPoint(const_cast<GeoPoint*>(i2));
 	addPoint(const_cast<GeoPoint*>(i3));

 if ( pLogfile != NULL )
  {
  pLogfile->writeLogFile ( "Dreieck" );

  Triangle * pTria = new Triangle ( 3 );
  pTria->set ( *i1, *i2, *i3 );

  if ( pLogfile != NULL )
  {
    Direction * normal = pTria->getNormal();

    if ( normal != NULL )
    {
      xSTR = System::Convert::ToString ( normal->getX() );
      ySTR = System::Convert::ToString ( normal->getY() );
      zSTR = System::Convert::ToString ( normal->getZ() );
    }
    else
    {
      xSTR = "0.0";
      ySTR = "0.0";
      zSTR = "0.0";
    }

    QuConvert::systemStr2stdStr ( xStr, xSTR );
    QuConvert::systemStr2stdStr ( yStr, ySTR );
    QuConvert::systemStr2stdStr ( zStr, zSTR );

    string outputLine = "Normale: " + xStr + ", " + yStr + ", " + zStr;
    pLogfile->writeLogFile ( outputLine );
    delete pTria;

    pLogfile->writeLogFile ( "********" );
  }
  }

  m_vCoordIndex.push_back(-1);
}

void TessTriangulator::onError(GLenum err)
{
  m_errnos.push_back(err);
}

bool TessTriangulator::processErrors()
{
  bool success = true;

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
        success = false;
        break;
      }
      case GLU_TESS_NEED_COMBINE_CALLBACK:
      {
        throw std::exception("GLU_TESS_NEED_COMBINE_CALLBACK");
        success = false;
        break;
      }
      default:
      {
        throw std::exception("Unknown GLU error");
        success = false;
        break;
      }
    }
    if ( !success )
      break;
  }
  return success;
}

bool TessTriangulator::triangulatePolygons( std::vector<PolygonGeo*> &vPoly, std::vector<int>& vCoordIndex, std::vector<GeoPoint*>& vPoints, std::vector<bool> &oriVec )
{
  bool success = true;


  for ( unsigned int i = 0; i < vPoly.size(); i++ )
  {
    PolygonGeo * pPoly       = vPoly[i];
    bool         orientation = oriVec[i];

	  success = triangulate( pPoly, orientation );
    if ( !success )
      break;
  }

  delete pLogfile;

	vCoordIndex	= getCoordIndex();
	vPoints		  = getPoints();

  return success;
}
