#include "stdafx.h"

#include <vector>

#include "Triangulation.h"
#include "Geometrie.h"
#include "interface.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  Triangulation ebener Polygone mit Innenkonturen                          //
///////////////////////////////////////////////////////////////////////////////
Triangulation::Triangulation()
{
  pPolyToTriangulate = NULL;
  pTransformation     = NULL;

  anzContours = 0;          // Anzahl der Innenkonturen + 1
  anzVertices = 0;          // Anzahl der Kanten
  anzTriangles = 0;         // Gesamtzahl Dreiecke
  anzOverflowTriangles = 20; // Sicherheit gegen Fehler im Alg.

  contours = NULL;
  vertices = NULL;
  triangles = NULL;
}

Triangulation::~Triangulation()
{
  if ( contours != NULL )  delete [] contours;
  if ( vertices != NULL )  delete [] vertices;
  if ( triangles != NULL ) delete [] triangles;
}

void Triangulation::triangulate ( PolygonGeo * pPoly, vector<Triangle*> &vTriangles )
{
  vector<double> vX;
  vector<double> vY;
  vector<double> vZ;
  vector<double> vZVertices;
  unsigned int   i, j, iOffset;
  Matrix4        mat;
  Matrix4        matInv;
  bool           isHorizontal;


  if ( contours != NULL )  delete [] contours;
  if ( vertices != NULL )  delete [] vertices;
  if ( triangles != NULL ) delete [] triangles;

  pPolyToTriangulate = pPoly;
  Plane * pPlane = pPolyToTriangulate->getPlane();
  if ( !pPlane->isHorizontal() )
  {
    mat = pPlane->getTransformationToHorizontal();
    matInv = mat.invers();
    pPolyToTriangulate->transform ( mat );
    isHorizontal = false;
  }
  else
    isHorizontal = true;

  anzContours = pPolyToTriangulate->getLochAnz() + 1;
  contours = new int[anzContours];

  contours[0] = pPolyToTriangulate->getAussenkontur()->getPointAnz() - 1;
  anzVertices =  contours[0];

  for ( unsigned int i = 0; i < pPolyToTriangulate->getLochAnz(); i++ )
  {
    _Ring * pHole = pPolyToTriangulate->getLoch( i );
    contours[i+1] = pHole->getPointAnz() - 1;
    anzVertices   = anzVertices + contours[i+1];
  }

  anzTriangles = anzVertices + 2*anzContours - 4;

  vertices  = new double[anzVertices+1][2];
  triangles = new int[anzTriangles + anzOverflowTriangles][3];

  pPolyToTriangulate->getAussenkontur()->getPoints3D( vX, vY, vZ );
  vertices[0][0] = 0.0;
  vertices[0][1] = 0.0;
  vZVertices.push_back( 0.0 );
  for ( i = 0; i < vX.size()-1; i++  )
  {
    vertices[i+1][0] = vX[i];
    vertices[i+1][1] = vY[i];
    vZVertices.push_back ( vZ[i] );
  }

  iOffset = vX.size();
  for ( i = 0; i < pPolyToTriangulate->getLochAnz(); i++ )
  {
    _Ring * pHole = pPolyToTriangulate->getLoch( i );
    vX.clear();
    vY.clear();
    vZ.clear();

    pHole->getPoints3D( vX, vY, vZ );
    for ( j = 0; j < vX.size() - 1; j++ )
    {
      vertices[iOffset+j][0] = vX[j];
      vertices[iOffset+j][1] = vY[j];
      vZVertices.push_back ( vZ[i] );
    }
    iOffset = iOffset + vX.size() - 1;
 }

  triangulate_polygon ( anzContours, contours, vertices, triangles );

  for ( i = 0; i < anzTriangles; i++ )
  {
    GeoPoint p0;
    GeoPoint p1;
    GeoPoint p2;

    p0.set( vertices[triangles[i][0]][0], vertices[triangles[i][0]][1], vZVertices[triangles[i][0]] );
    p1.set( vertices[triangles[i][1]][0], vertices[triangles[i][1]][1], vZVertices[triangles[i][1]] );
    p2.set( vertices[triangles[i][2]][0], vertices[triangles[i][2]][1], vZVertices[triangles[i][2]] );

    Triangle * pTria = new Triangle ( 3 );
    pTria->set( p0, p1, p2 );
    pTria->setCheckResult( ADVANCED_CHECK_FULFILLED );
    vTriangles.push_back( pTria );
  }

  if ( !isHorizontal )
    pPolyToTriangulate->transform ( matInv );
}
