#include "stdafx.h"

#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <deque>
#include "math.h"

#include "Constant.h"
#include "Convert.h"
#include "Checking.h"
#include "Topologie.h"
#include "Geometrie.h"
#include "Feature.h"
#include "SrsManager.h"
#include "includes.h"
#include "Logfile.h"
#include "Triangulation\TessTriangulator.hpp"

using namespace std;

struct PatchLine
{
	size_t typ;
  double pa[2];
  double pe[2];
};

class LinearRing;

///////////////////////////////////////////////////////////////////////////////
//  Geometrie						               							 //
//  Abstrakte Basisklasse für alle Geometrien		       					 //
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//  Konstruktoren, Destruktor                                                //
///////////////////////////////////////////////////////////////////////////////
_Geometrie::_Geometrie ( int dimP )
{
  gmlId = Constant::createGmlId();

  dim     = dimP;

  m_sw[0] =  1.0E10;
  m_sw[1] =  1.0E10;
  m_sw[2] =  1.0E10;
  m_no[0] = -1.0E10;
  m_no[1] = -1.0E10;
  m_no[2] = -1.0E10;

  bbExist = false;
  
  checkResult = NO_CHECK_PERFORMED;
}

_Geometrie::_Geometrie ( _Geometrie * pGeoOld )
{
  gmlId = Constant::createGmlId();	
  srsName     = pGeoOld->srsName;
  dim         = pGeoOld->dim;
  checkResult = pGeoOld->checkResult;

  if ( pGeoOld->bbExist )
  {
    m_sw[0]   = pGeoOld->m_sw[0];
    m_sw[1]   = pGeoOld->m_sw[1];
    m_sw[2]   = pGeoOld->m_sw[2];
    m_no[0]   = pGeoOld->m_no[0];
    m_no[1]   = pGeoOld->m_no[1];
    m_no[2]   = pGeoOld->m_no[2];
    bbExist = true;
  }
  else
    bbExist = false;
}

_Geometrie::_Geometrie ( const _Geometrie & geoOld )
{
  gmlId = Constant::createGmlId();
  
  srsName     = geoOld.srsName;
  dim         = geoOld.dim;
  checkResult = geoOld.checkResult;

  if ( geoOld.bbExist )
  {
    m_sw[0]   = geoOld.m_sw[0];
    m_sw[1]   = geoOld.m_sw[1];
    m_sw[2]   = geoOld.m_sw[2];
    m_no[0]   = geoOld.m_no[0];
    m_no[1]   = geoOld.m_no[1];
    m_no[2]   = geoOld.m_no[2];
    bbExist = true;
  }
  else
    bbExist = false;
}

///////////////////////////////////////////////////////////////////////////////
// Liefert die gml:id als String ^                                           //
///////////////////////////////////////////////////////////////////////////////
System::String ^ _Geometrie::getGML_ID()
{
  if ( gmlId == "" )
    return nullptr;
  else
    return QuConvert::ToString ( gmlId );
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft Referenzen auf Geometrie-Objekte                               //
//  Standard-Implementierung                                                 //
///////////////////////////////////////////////////////////////////////////////
bool _Geometrie::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Legt die Kopie eines Geometrieobjektes an                                //
///////////////////////////////////////////////////////////////////////////////
_Geometrie * _Geometrie::copy ( _Geometrie * pGeo )
{
  switch ( pGeo->getGeometryType() )
  {
  case GEOMETRY_REFERENCE:
    {
      GeometryReference * pGeometryReference = (GeometryReference*)pGeo;
      return new GeometryReference ( pGeometryReference );
    }

  case GEO_POINT: 
    {
      GeoPoint * pGeoPoint = (GeoPoint*)pGeo;
      return new GeoPoint ( pGeoPoint );
    }

  case MULTI_POINT:
    {
      MultiPoint * pMultiPoint = (MultiPoint*)pGeo;
      return new MultiPoint ( pMultiPoint );
    }

  case SURFACE:
    {
      Surface * pSurface = (Surface*)pGeo;
      return new Surface ( pSurface );
    }

  case ORIENTABLE_SURFACE:
    {
      OrientableSurface * pSurface = (OrientableSurface*)pGeo;
      return new OrientableSurface ( pSurface );
    }

  case POLYGON:
    {
      PolygonGeo * pPolygon = (PolygonGeo*)pGeo;
      return new PolygonGeo ( pPolygon );
    }

  case SURFACE_REFERENCE:
    {
      SurfaceReference * pSurfaceReference = (SurfaceReference*)pGeo;
      return new SurfaceReference ( pSurfaceReference );
    }

  case MULTI_GEOMETRY:
    {
      MultiGeometry * pMultiGeometry = (MultiGeometry*)pGeo;
      return new MultiGeometry ( pMultiGeometry );
    }

  case MULTI_SURFACE:
    {
      MultiSurface * pMultiSurface = (MultiSurface*)pGeo;
      return new MultiSurface ( pMultiSurface );
    }
  case COMPOSITE_SURFACE:
    {
      CompositeSurface * pCompositeSurface = (CompositeSurface*)pGeo;
      return new CompositeSurface ( pCompositeSurface );
    }

  case SHELL:
    {
      Shell * pCompositeSurface = (Shell*)pGeo;
      return new Shell ( pCompositeSurface );
    }

  case SOLID_REFERENCE:
    {
      SolidReference * pSolidReference = (SolidReference*)pGeo;
      return new SolidReference ( pSolidReference );
    }

case SOLID:
    {
      Solid * pSolid = (Solid*)pGeo;
      return new Solid ( pSolid );
    }

  case MULTI_SOLID:
    {
      MultiSolid * pMultiSolid = (MultiSolid*)pGeo;
      return new MultiSolid ( pMultiSolid );
    }

  case COMPOSITE_SOLID:
    {
      MultiSolid * pMultiSolid = (MultiSolid*)pGeo;
      pMultiSolid->setIsCompositeSolid( true );
      return new MultiSolid ( pMultiSolid );
    }

  case CURVE_REFERENCE:
    {
      CurveReference * pCurveReference = (CurveReference*)pGeo;
      return new CurveReference ( pCurveReference );
    }

case CURVE:
    {
      Curve * pCurve = (Curve*)pGeo;
      return new Curve ( pCurve );
    }

  case MULTI_CURVE:
    {
      MultiCurve * pMultiCurve = (MultiCurve*)pGeo;
      return new MultiCurve ( pMultiCurve );
    }

  case LINE_STRING:
  case LINE_STRING_SEGMENT:
  {
      LineSegment * pLineSegment = (LineSegment*)pGeo;
      return new LineSegment ( pLineSegment );
    }

  case ARC:
  case ARC_STRING:
  case CIRCLE:
    {
      ArcSegment * pArcSegment = (ArcSegment*)pGeo;
      return new ArcSegment ( pArcSegment );
    }

  case LINEAR_RING:
    {
      LinearRing * pLinearRing = (LinearRing*)pGeo;
      return new LinearRing ( pLinearRing );
    }

  case RING:
    {
      Ring * pRing = (Ring*)pGeo;
      return new Ring ( pRing );
    }

  case ALLG_GEOMETRIE:
    {
      Geometrie * pGeometrie = (Geometrie*)pGeo;
      return new Geometrie ( pGeometrie );
    }

  case IMPLICIT_GEOMETRY:
    {
      ImplicitGeometry * pImplicitGeometry = (ImplicitGeometry*)pGeo;
      return new ImplicitGeometry ( pImplicitGeometry );
    }

  case IMPLICIT_GEOMETRY_REFERENCE:
    {
      ImplicitGeometryReference * pImplicitGeometryReference = (ImplicitGeometryReference*)pGeo;
      return new ImplicitGeometryReference ( pImplicitGeometryReference );
    }
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen Geometrietyp als Text                                      //
///////////////////////////////////////////////////////////////////////////////
String ^ _Geometrie::toString ( GEOMETRY_TYPE geoType )
{
  switch ( geoType )
  {
  case GEO_POINT: return "GEO_POINT";
  case MULTI_GEOMETRY: return "MULTI_GEOMETRY";
  case MULTI_POINT: return "MULTI_POINT";
  case SURFACE: return "SURFACE";
  case POLYGON: return "POLYGON";
  case MULTI_SURFACE: return "MULTI_SURFACE";
  case COMPOSITE_SURFACE: return "COMPOSITE_SURFACE";
  case SHELL: return "SHELL";
  case ORIENTABLE_SURFACE: return "ORIENTABLE_SURFACE";
  case SOLID: return "SOLID";
  case MULTI_SOLID: return "MULTI_SOLID";
  case CURVE: return "CURVE";
  case MULTI_CURVE: return "MULTI_CURVE";
  case LINE_STRING: return "LINE_STRING";
  case LINE_STRING_SEGMENT: return "LINE_STRING_SEGMENT";
  case ARC: return "ARC";
  case ARC_STRING: return "ARC_STRING";
  case CIRCLE: return "CIRCLE";
  case LINEAR_RING: return "LINEAR_RING";
  case RING: return "RING";
  case ALLG_GEOMETRIE: return "ALLG_GEOMETRIE";
  case IMPLICIT_GEOMETRY: return "IMPLICIT_GEOMETRY";
  case GEOMETRY_REFERENCE: return "GEOMETRY_REFERENCE";
  case SURFACE_REFERENCE: return "SURFACE_REFERENCE";
  case CURVE_REFERENCE: return "CURVE_REFERENCE";
  case POINT_REFERENCE: return "POINT_REFERENCE";
  case SOLID_REFERENCE: return "SOLID_REFERENCE";
  case COMPOSITE_SOLID: return "COMPOSITE_SOLID";
  case TRIANGULATED_SURFACE: return "TRIANGULATED_SURFACE";
  case TRIANGLE: return "TRIANGLE";
  case OTHER: return "OTHER";
  case NO_GEOMETRY: return "NO_GEOMETRY";
  }
  return nullptr;
}


///////////////////////////////////////////////////////////////////////////////
//  Berechnet, ob sich die Liniesegmente                                     //
//                                                                           //
//        |p1x|       |p2x|       |q1x|       |q2x|                          //
//        |   |  -->  |   | und   |   |  -->  |   |                          //
//        |p1y|       |p2y|       |q1y|       |q2y|                          //
//                                                                           //
//  schneiden oder berühren                                                  //
///////////////////////////////////////////////////////////////////////////////
CUT_RESULT _Geometrie::cutPoints ( double p1x, double p1y, double p2x, double p2y,
                           double q1x, double q1y, double q2x, double q2y, double minDist, double minAngleDist, 
                           vector<GeoPoint> &vCutPoints )
{
    double pStartPoint[2], pEndPoint[2], qStartPoint[2], qEndPoint[2];
    double c1CutPoint[2], c2CutPoint[2];
    double det, skp1, skp2, h, hp, hq;
    int    lp1, lp2, nCutPoint, i;
    double c1Lam, c1Mue, c2Lam, c2Mue; 
    bool   echterSchnitt;

    pStartPoint[0] = p1x;
    pStartPoint[1] = p1y;
    pEndPoint[0]   = p2x;
    pEndPoint[1]   = p2y;
    qStartPoint[0] = q1x;
    qStartPoint[1] = q1y;
    qEndPoint[0]   = q2x;
    qEndPoint[1]   = q2y;

    echterSchnitt = false;
    if ( p1x <= p2x )
    {
      if ( q1x <= q2x )
      {
        if ( ( p2x + minDist < q1x ) || ( q2x + minDist < p1x ) )
          return NO_CUT;
      }
      else
      {
        if ( ( p2x + minDist < q2x ) || ( q1x + minDist < p1x ) )
          return NO_CUT;
      }
    }
    else
    {
      if ( q1x <= q2x )
      {
        if ( ( p1x + minDist < q1x ) || ( q2x + minDist < p2x ) )
          return NO_CUT;
      }
      else
      {
        if ( ( p1x + minDist < q2x ) || ( q1x + minDist < p2x ) )
          return NO_CUT;
      }
    }

    hp = sqrt ( ( pStartPoint[0] - pEndPoint[0] )*( pStartPoint[0] - pEndPoint[0] ) + 
      ( pStartPoint[1] - pEndPoint[1] )*( pStartPoint[1] - pEndPoint[1] ) );
    if ( hp  < minDist )
    {
      echterSchnitt = false;
      return NO_CUT;
    }

    hq = sqrt ( ( qStartPoint[0] - qEndPoint[0] )*( qStartPoint[0] - qEndPoint[0] ) + 
      ( qStartPoint[1] - qEndPoint[1] )*( qStartPoint[1] - qEndPoint[1] ) );
    if ( hq < minDist )
    {
      echterSchnitt = false;
      return NO_CUT;
    }

    det = ( pStartPoint[0] - pEndPoint[0] )*(qEndPoint[1] - qStartPoint[1] ) -
      ( pStartPoint[1] - pEndPoint[1] )*(qEndPoint[0] - qStartPoint[0] );

    if ( fabs ( det ) < minAngleDist*hp*hq ) // Geradenstücke sind parallel
    {
      echterSchnitt = false;
      h = ( pStartPoint[0] - qStartPoint[0] ) *
        ( pStartPoint[1] - qEndPoint[1] )	-
        ( pStartPoint[1] - qStartPoint[1] ) *
        ( pStartPoint[0] - qEndPoint[0] );
      if ( fabs ( h ) < minAngleDist )  // Geradenstücke fluchten
      {
        skp1 = ( qStartPoint[0] - qEndPoint[0] ) *
          ( qStartPoint[0] - pStartPoint[0] ) + 
          ( qStartPoint[1] - qEndPoint[1] ) *
          ( qStartPoint[1] - pStartPoint[1] );
        skp2 = ( qStartPoint[0] - qEndPoint[0] ) *
          ( qEndPoint[0]   - pStartPoint[0] ) + 
          ( qStartPoint[1] - qEndPoint[1] ) *
          ( qEndPoint[1]   - pStartPoint[1] );

        if ( skp1 < 0.0 )     lp1 = 1;  // pStartPoint links von q
        else
          if ( skp2 < 0.0 ) lp1 = 3;  // pStartPoint im Innern von q
          else			  lp1 = 5;  // pStartPoint rechts von q

          if ( fabs ( skp1 ) < minAngleDist )
            lp1 = 2;					 // pStartPoint = qStartPoint
          if ( fabs ( skp2 ) < minAngleDist )
            lp1 = 4;					 // pStartPoint = qEndPoint


          skp1 = ( qStartPoint[0] - qEndPoint[0] ) *
            ( qStartPoint[0] - pEndPoint[0] ) + 
            ( qStartPoint[1] - qEndPoint[1] ) *
            ( qStartPoint[1] - pEndPoint[1] );
          skp2 = ( qStartPoint[0] - qEndPoint[0] ) *
            ( qEndPoint[0]   - pEndPoint[0] ) + 
            ( qStartPoint[1] - qEndPoint[1] ) *
            ( qEndPoint[1]   - pEndPoint[1] );

          if ( skp1 < 0.0 )     lp2 = 1;  // pEndPoint links von q
          else
            if ( skp2 < 0.0 ) lp2 = 3;   // pEndPoint im Innern von q
            else			  lp2 = 5;  // pEndPoint rechts von q
            if ( fabs ( skp1 ) < minAngleDist )
              lp2 = 2;					 // pEndPoint = qStartPoint
            if ( fabs ( skp2 ) < minAngleDist )
              lp2 = 4;					 // pEndPoint = qEndPoint

            nCutPoint = 0;

            switch ( lp1 )
            {
            case 1: switch ( lp2 )
                    {
            case 1:  nCutPoint = 0;
              break;
            case 2:  nCutPoint = 1;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = qStartPoint[i];
              }
              c1Lam = 0.0;
              c1Mue = 1.0;
              break;
            case 3:  nCutPoint = 2;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = qStartPoint[i];
                c2CutPoint[i] = pEndPoint[i];
              }
              c1Lam = lengthRatio ( pEndPoint, pStartPoint, qStartPoint );
              c1Mue = 1.0;
              c2Lam = 0.0;
              c2Mue = lengthRatio ( qEndPoint, qStartPoint, pEndPoint );
              break;
            case 4: case 5: 
              nCutPoint = 2;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = qStartPoint[i];
                c2CutPoint[i] = qEndPoint[i];
              }
              c1Lam = lengthRatio ( pEndPoint, pStartPoint, qStartPoint );
              c1Mue = 1.0;
              c2Lam = lengthRatio ( pEndPoint, pStartPoint, qEndPoint );
              c2Mue = 0.0;
             break;
                    }
                    break;

            case 2:	switch ( lp2 )
                    {
            case 1: case 2: 
              nCutPoint = 1;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = qStartPoint[i];
              }
              c1Lam = 1.0;
              c1Mue = 1.0;
              break;
            case 3:  nCutPoint = 2;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = qStartPoint[i];
                c2CutPoint[i] = pEndPoint[i];
              }
              c1Lam = 1.0;
              c1Mue = 1.0;
              c2Lam = 0.0;
              c2Mue = lengthRatio ( qEndPoint, qStartPoint, pEndPoint );
              break;
            case 4: case 5: 
              nCutPoint = 2;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = qStartPoint[i];
                c2CutPoint[i] = qEndPoint[i];
              }
              c1Lam = 1.0;
              c1Mue = 1.0;
              c2Lam = lengthRatio ( pEndPoint, pStartPoint, qEndPoint );
              c2Mue = 0.0;
              break;
                    }
                    break;

            case 3:	switch ( lp2 )
                    {
            case 1: case 2:  
              nCutPoint = 2;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = pStartPoint[i];
                c2CutPoint[i] = qStartPoint[i];
              }
              c1Lam = 1.0;
              c1Mue = lengthRatio ( qEndPoint, qStartPoint, pStartPoint );
              c2Lam = lengthRatio ( pEndPoint, pStartPoint, qStartPoint );
              c2Mue = 1.0;
              break;
            case 3:  nCutPoint = 2;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = pStartPoint[i];
                c2CutPoint[i] = pEndPoint[i];
              }
              c1Lam = 1.0;
              c1Mue = lengthRatio ( qEndPoint, qStartPoint, pStartPoint );
              c2Lam = 0.0;
              c2Mue = lengthRatio ( qEndPoint, qStartPoint, pEndPoint );
              break;
            case 4: case 5: 
              nCutPoint = 2;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = pStartPoint[i];
                c2CutPoint[i] = qEndPoint[i];
              }
              c1Lam = 1.0;
              c1Mue = lengthRatio ( qEndPoint, qStartPoint, pStartPoint );
              c2Lam = lengthRatio ( pEndPoint, pStartPoint, qEndPoint );
              c2Mue = 0.0;
              break;
                    }
                    break;

            case 4:	switch ( lp2 )
                    {
            case 1: case 2:  
              nCutPoint = 2;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = pStartPoint[i];
                c2CutPoint[i] = qStartPoint[i];
              }
              c1Lam = 1.0;
              c1Mue = 0.0;
              c2Lam = lengthRatio ( pEndPoint, pStartPoint, qStartPoint );
              c2Mue = 1.0;
              break;
            case 3:  nCutPoint = 2;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = pStartPoint[i];
                c2CutPoint[i] = pEndPoint[i];
              }
              c1Lam = 1.0;
              c1Mue = 0.0;
              c2Lam = 0.0;
              c2Mue = lengthRatio ( qEndPoint, qStartPoint, pEndPoint );
              break;
            case 4: case 5: 
              nCutPoint = 1;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = pStartPoint[i];
              }
              c1Lam = 1.0;
              c1Mue = 0.0;
              break;
                    }
                    break;

            case 5:	switch ( lp2 )
                    {
            case 1: case 2:  
              nCutPoint = 2;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = qStartPoint[i];
                c2CutPoint[i] = qEndPoint[i];
              }
              c1Lam = lengthRatio ( pEndPoint, pStartPoint, qStartPoint );
              c1Mue = 1.0;
              c2Lam = lengthRatio ( pEndPoint, pStartPoint, qEndPoint );
              c2Mue = 0.0;
              break;
            case 3:  nCutPoint = 2;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = qEndPoint[i];
                c2CutPoint[i] = pEndPoint[i];
              }
              c1Lam = lengthRatio ( pEndPoint, pStartPoint, qEndPoint );
              c1Mue = 0.0;
              c2Lam = 0.0;
              c2Mue = lengthRatio ( qEndPoint, qStartPoint, pEndPoint );
              break;
            case 4: nCutPoint = 1;
              for ( i = 0; i < 2; i++ )
              {
                c1CutPoint[i] = qEndPoint[i];
              }
              c1Lam = 0.0;
              c1Mue = 0.0;
              break;
            case 5: nCutPoint = 0;
              break;
                    }
                    break;
            }

            if ( nCutPoint > 0 )
            {
              vCutPoints.push_back( GeoPoint( c1CutPoint[0], c1CutPoint[1] ) );
              if ( nCutPoint == 2 )
                vCutPoints.push_back( GeoPoint( c2CutPoint[0], c2CutPoint[1] ) );
            }
            switch ( nCutPoint )
            {
            case 0: return NO_CUT;
            case 1:
              return POINT_ON_POINT;
            case 2: return OVERLAP;
            }
      }
      else return NO_CUT;
    }
    else   // Geradenstücke sind nicht parallel
    {
      c1Lam = ( qEndPoint[0] - pEndPoint[0] )*(qEndPoint[1] - qStartPoint[1] ) -
        ( qEndPoint[1] - pEndPoint[1] )*(qEndPoint[0] - qStartPoint[0] );
      c1Lam = c1Lam / det;

      c1Mue = ( pStartPoint[0] - pEndPoint[0] )*( qEndPoint[1] - pEndPoint[1] ) -
        ( pStartPoint[1] - pEndPoint[1] )*( qEndPoint[0] - pEndPoint[0] );
      c1Mue = c1Mue / det;

      if ( c1Lam*hp < -minDist || ( c1Lam - 1.0)*hp >  minDist ||
           c1Mue*hq < -minDist || ( c1Mue - 1.0)*hq >  minDist )
        return NO_CUT;
      else
      {
        if ( fabs ( c1Lam*hp )         < minDist ||
             fabs ( (1.0 - c1Lam)*hp ) < minDist ||
             fabs ( c1Mue*hq )         < minDist ||
             fabs ( (1.0 - c1Mue)*hq ) < minDist )	
          echterSchnitt = false;
        else	
          echterSchnitt = true;

        for ( i = 0; i < 2; i++ )
          c1CutPoint[i] = c1Lam*pStartPoint[i] + (1.0-c1Lam)*pEndPoint[i];
        vCutPoints.push_back( GeoPoint( c1CutPoint[0], c1CutPoint[1] ) );
        if ( echterSchnitt ) return TRUE_CUT;
        else
        {
          if ( ( fabs ( c1Lam*hp ) < minDist || fabs ( (1.0 - c1Lam)*hp ) < minDist ) &&
               ( fabs ( c1Mue*hq ) < minDist || fabs ( (1.0 - c1Mue)*hq ) < minDist ) )
               return POINT_ON_POINT;
          else
            return POINT_ON_EDGE;        
        }
      }
    }
    return NO_CUT;
}

///////////////////////////////////////////////////////////////////////////////
//  Hilfsroutinen für die Verschneidungs-Funktion                            //
///////////////////////////////////////////////////////////////////////////////
CUT_RESULT _Geometrie::schneidet ( double p1x, double p1y, double p2x, double p2y,
                          double q1x, double q1y, double q2x, double q2y, double minDist, double minAngleDist )
{
  vector<GeoPoint> vCutPoints;

  return cutPoints ( p1x, p1y, p2x, p2y, q1x, q1y, q2x, q2y, minDist, minAngleDist, vCutPoints );
}

double _Geometrie::lengthRatio ( double pa[], double pe[], double q[] )
{
  double h1 = ( pa[0] - pe[0] ) * (pa[0] - pe[0] ) +  
    ( pa[1] - pe[1] ) * (pa[1] - pe[1] );

  double h2 = ( pa[0] - q[0] ) * (pa[0] - q[0] ) +  
    ( pa[1] - q[1] ) * (pa[1] - q[1] );

  if ( h1 != 0.0 ) return sqrt ( h2 / h1 );
  else			 return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert auf der durch p und normal bestimmten Ebene n Punkte, die einen//
//  Abstand r von p haben                                                    //
///////////////////////////////////////////////////////////////////////////////
void  _Geometrie::generateNPolygon ( GeoPoint p, double r, GeoPoint normal, int n, std::vector<GeoPoint> &points )
{
  GeoPoint vec (3); //  Richtung des ersten Punktes des Kreises
  GeoPoint vecRot ( 3 );
  double   a, b, c;
  double   d = 0.0;
  double   delta;
  int      i;
  Matrix4  mh;
  Matrix4  m1;
  Matrix4  m2;

  if ( n <= 0 ) return;

  delta = (2.0*PI)/n;

  a = normal.getX();
  b = normal.getY();
  c = normal.getZ();

  //  Bestimmung des Vektors, der um Punkt p gedreht werden muss
  if ( fabs( fabs(c) - 1.0 ) < 1.0E5 )
    vec.set( r, 0.0, 0.0 );
  else
  {
    d = sqrt ( a*a + b*b );
    vec.set ( -b/d, a/d, 0.0 );
    m1 = Matrix4 ( 1.0, 0.0, 0.0,  0.0, 
                   0.0, c/d, -a/d, 0.0,
                   0.0, a/d,  c/d, 0.0 );


    m2 = Matrix4 ( d,   0.0, a,   0.0, 
                   0.0, 1.0, 0.0, 0.0,
                   -a,  0.0, d,   0.0 );
  }

  //  Bestimmung der Rotationsmatrizen

  Matrix4 m = m2*m1;
  Matrix4 mT = m.invers();

  for ( i = 0; i < n; i++ )
  {
    Matrix4 m3 = Matrix4 ( cos(i*delta), -sin(i*delta), 0.0, 0.0, 
                            sin(i*delta), cos(i*delta), 0.0, 0.0,
                            0.0,        0.0,        1.0, 0.0 );

    mh = m3*m;
    mh = mT*mh;
    vecRot =mh.transform( vec );
    points.push_back( p + vecRot );
  }
}
///////////////////////////////////////////////////////////////////////////////
//  Bestimmt den Durchstoßpunkt der Linie, die durch Punkt pLine und         //
//  Richtung nLine bestimmt ist, durch die Ebene, die durch Punkt pArea und  //
// Normalenvektor nArea bestimmt ist                                         //
///////////////////////////////////////////////////////////////////////////////
bool  _Geometrie::lineAreaCut ( GeoPoint pLine, GeoPoint nLine, GeoPoint pArea, GeoPoint nArea, GeoPoint & p )
{
  GeoPoint linemArea(3);
  double   lineDirMultAreaNormal;
  double   h2;

  linemArea = pLine - pArea;
  lineDirMultAreaNormal = nLine.mult ( nArea );
  if ( fabs( lineDirMultAreaNormal ) < 1.0E-5 )
    return false;

  h2 = linemArea.mult( nArea );

  p = pLine - nLine*(h2/lineDirMultAreaNormal);
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Geometrietyp als Text                                        //
///////////////////////////////////////////////////////////////////////////////
std::string _Geometrie::convertTypeToString ( GEOMETRY_TYPE type )
{
  switch ( type )
  {
  case GEO_POINT:           return "GEO_POINT";
  case MULTI_GEOMETRY:      return "MULTI_GEOMETRY";
  case MULTI_POINT:         return "MULTI_POINT";
  case SURFACE:             return "SURFACE";
  case POLYGON:             return "POLYGON";
  case MULTI_SURFACE:       return "MULTI_SURFACE";
  case COMPOSITE_SURFACE:   return "COMPOSITE_SURFACE";
  case SHELL:               return "SHELL";
  case ORIENTABLE_SURFACE:  return "ORIENTABLE_SURFACE";
  case SOLID:               return "SOLID";
  case MULTI_SOLID:         return "MULTI_SOLID";
  case CURVE:               return "CURVE";
  case MULTI_CURVE:         return "MULTI_CURVE";
  case LINE_STRING:         return "LINE_STRING";
  case LINE_STRING_SEGMENT: return "LINE_STRING_SEGMENT";
  case ARC:                 return "ARC";
  case ARC_STRING:          return "ARC_STRING";
  case CIRCLE:              return "CIRCLE";
  case LINEAR_RING:         return "LINEAR_RING";
  case DIRECT_POSITION:     return "DIRECT_POSITION";
  case RING:                return "RING";
  case ALLG_GEOMETRIE:      return "ALLG_GEOMETRIE";
  case IMPLICIT_GEOMETRY:   return "IMPLICIT_GEOMETRY";
  case GEOMETRY_REFERENCE:  return "GEOMETRY_REFERENCE";
  case SURFACE_REFERENCE:   return "SURFACE_REFERENCE";
  case CURVE_REFERENCE:     return "CURVE_REFERENCE";
  case POINT_REFERENCE:     return "POINT_REFERENCE";
  case SOLID_REFERENCE:     return "SOLID_REFERENCE";
  case COMPOSITE_SOLID:     return "COMPOSITE_SOLID";
  case TRIANGULATED_SURFACE:return "TRIANGULATED_SURFACE";
  case TRIANGLE:            return "TRIANGLE";
  case GEOMETRIC_COMPLEX:   return "GEOMETRIC_COMPLEX";
  case OTHER:               return "OTHER";
  case NO_GEOMETRY:         return "NO_GEOMETRY";
  }

  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Typ der Berührung bzw. Überlappung zweier Polygone als       //
// Text                                                                      //
///////////////////////////////////////////////////////////////////////////////
std::string _Geometrie::convertTypeToString ( POLYGON_TOPOLOGY type  )
{
  switch ( type )
  {
  case POLYGONE_DISJUNKT:
    return "Disjunkt";

  case PUNKT_AUF_PUNKT:
    return "Vertex auf Vertex";
    
  case PUNKT_AUF_FLAECHE:
    return "Vertex auf Fläche";

  case PUNKT_AUF_LINIE:
    return "Vertex auf Konturlinie";

  case LINIE_AUF_LINIE:
    return "Konturlinie auf Konturlinie";

  case LINIE_AUF_FLAECHE:
    return "Konturlinie auf Fläche";

  case FLAECHE_DURCH_FLAECHE:
    return "Fläche durchdringt Fläche";

  case FLAECHE_AUF_FLAECHE:
    return "Fläche überlappt Fläche";
  }

  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  _GeometryReference                                                       //
//  Basisklasse für alle Geometry-Reference Klassen                          //
///////////////////////////////////////////////////////////////////////////////
_GeometryReference::_GeometryReference (  Features * pFeaturesP )
{
  pFeatures  = pFeaturesP;
  refGeoId   = "";
}

_GeometryReference::_GeometryReference ( _GeometryReference * pGeoReferenceOld )
{
  pFeatures = pGeoReferenceOld->pFeatures;
  refGeoId  = pGeoReferenceOld->refGeoId;
}


_GeometryReference::~_GeometryReference()
{

}

///////////////////////////////////////////////////////////////////////////////
//  GeometryReference                                                        //
//  Referenz auf eine GML-Geometrie                                          //
///////////////////////////////////////////////////////////////////////////////
GeometryReference::GeometryReference( Features * pFeaturesP ): _Geometrie ( 2 ), _GeometryReference ( pFeaturesP )
{
  refGeoType = "";
}

GeometryReference::GeometryReference ( GeometryReference * pGeoReferenceOld )
:_Geometrie ( pGeoReferenceOld ), _GeometryReference ( pGeoReferenceOld )
{
  refGeoType = pGeoReferenceOld->refGeoType;
}

GeometryReference::~GeometryReference()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das referierte Geometrieobjekt                                   //
///////////////////////////////////////////////////////////////////////////////
_Geometrie * _GeometryReference::getRefGeoObjekt()
{
  return pFeatures->getGeometry ( refGeoId );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den abstrakten Geometrietyp der referierten Geometrie            //
///////////////////////////////////////////////////////////////////////////////
ABSTRACT_GEOMETRY_TYPE GeometryReference::getAbstractGeometryType()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getAbstractGeometryType();
  else
    return ABSTRACT_UNKNOWN;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Dimension der referierten Geometrie, oder 0 falls keine      //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
int GeometryReference::getDim()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getDim();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den srsName des referierten Geometrie, oder "" falls keine       //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
std::string GeometryReference::getSrsName ()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getSrsName();
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der referierten Geometrie ein gültiges       //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool GeometryReference::existReferenceSystem( SrsManager * pSrsManager )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->existReferenceSystem( pSrsManager );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den BoundingBox der referierten Geometrie, oder false falls keine//
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool GeometryReference::getBoundingBox ( double * sw, double * no )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getBoundingBox( sw, no );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Center2D der referierten Geometrie, oder false falls keine   //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool GeometryReference::getCenter2D ( double &xP, double &yP )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCenter2D( xP, yP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Center3D der referierten Geometrie, oder false falls keine   //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool GeometryReference::getCenter3D ( double &xP, double &yP, double &zP )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCenter3D( xP, yP, zP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone der referierten Geometrie                //
///////////////////////////////////////////////////////////////////////////////
size_t GeometryReference::getPolygonAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygonAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments der referierten Geometrie           //
///////////////////////////////////////////////////////////////////////////////
size_t GeometryReference::getCurveSegmentAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCurveSegmentAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte der referierten Geometrie                  //
///////////////////////////////////////////////////////////////////////////////
size_t GeometryReference::getPointAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPointAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone der Stützpunkte der referierten Geometrie           //
///////////////////////////////////////////////////////////////////////////////
size_t GeometryReference::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygone( vPolygone );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone der Stützpunkte der referierten Geometrie sowie die //
//  Orientierung dieser Polygone                                             //
///////////////////////////////////////////////////////////////////////////////
size_t GeometryReference::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygone( vPolygone, vOrientations );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den Basic-Check für die referierte Geometrie durch                 //
///////////////////////////////////////////////////////////////////////////////
bool GeometryReference::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams, vector<Error*> &vErrorReport )
{
  if ( !pCheckParams->checkReferencedGeometry )
    return true;

  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return basicCheck (  pObj, featureProperty, pCheckParams, vErrorReport );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den Advanced-Check für die referierte Geometrie durch              //
///////////////////////////////////////////////////////////////////////////////
bool GeometryReference::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * 
        pCheckParams,vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( !pCheckParams->checkReferencedGeometry )
    return true;

  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return advancedCheck (  pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Existenz des referierten Geometrieobjektes sowie die       //
//  Konsistenz der Geometrietypen des GeometryReference-Objektes und des     //
//  referierten Geometrieobjektes                                            //
///////////////////////////////////////////////////////////////////////////////
bool GeometryReference::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  string          errorString;
  SemanticError * pError;
  GEOMETRY_TYPE   geoType;

 _Geometrie * pRefGeo = getRefGeoObjekt();
  if ( pRefGeo == NULL )
  {
    errorString = "Das Geometrieattribut " + propertyName + " hat ein nicht-aufgelöstes xlink: " + refGeoId;
    pError = new SemanticError ( pObj, Error::ERROR,  errorString );
    vErrorReport.push_back( pError );

    return false;
  }

  if ( refGeoType == "" )
    return true;

  geoType = pRefGeo->getGeometryType();

  if ( ( refGeoType == "gml:MultiSolidPropertyType" &&  geoType != MULTI_SOLID  )      ||
       ( refGeoType == "gml:MultiSurfacePropertyType" &&  geoType != MULTI_SURFACE )   ||
       ( refGeoType == "gml:MultiCurvePropertyType" &&  geoType != MULTI_CURVE )       ||
       ( refGeoType == "gml:MultiPointPropertyType" &&  geoType != MULTI_POINT )       ||
       ( refGeoType == "gml:SolidPropertyType" &&  geoType != SOLID && geoType != COMPOSITE_SOLID )        ||
       ( refGeoType == "gml:SurfacePropertyType" &&  geoType != SURFACE && geoType != COMPOSITE_SURFACE &&
          geoType != TRIANGULATED_SURFACE && geoType != POLYGON && geoType != ORIENTABLE_SURFACE        && 
          geoType != TRIANGLE )                                                                            ||
       ( refGeoType == "gml:CurvePropertyType" &&  geoType != CURVE && geoType != LINE_STRING  && geoType != LINE_STRING_SEGMENT  &&  
          geoType != ARC && geoType != CIRCLE  && geoType != ARC_STRING )                                                            ||
       ( refGeoType == "core:ImplicitRepresentationPropertyType" &&  geoType != IMPLICIT_GEOMETRY   ) )
  {
    errorString = "Das Geometrieattribut " + propertyName + " verweist mit " + refGeoId + " auf einen falschen Geometrietyp. ";
    errorString = errorString + "Notwendig ist: " + refGeoType + ", verwiesen wird auf " + _Geometrie::convertTypeToString( geoType );
    pError = new SemanticError ( pObj, Error::ERROR, errorString );   
    vErrorReport.push_back( pError );

    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebung der referierten Geometrie um einen offset                   //
//  Über den übergebenen Set von Geometrieobjekten wird sichergestellt,      //
//  dass ein einzelnes Geometrieobjekt nur einmal verschoben wird            //
///////////////////////////////////////////////////////////////////////////////
void GeometryReference::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  )
{
  _Geometrie * pGeo = getRefGeoObjekt();
  if ( pGeo != NULL && sGeometries.count( pGeo ) == 0 )
  {
    pGeo->offset( point, sGeometries );  
    sGeometries.insert( pGeo );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  _Surface												                                				 //
//  Abstrakte Oberklasse für gml:Surface (aufgebaut aus Polygon-Patches)     //
//  und gml:Polygon									                            						 //
///////////////////////////////////////////////////////////////////////////////
_Surface::_Surface( int dim ): _Geometrie( dim )
{
  pBRep    = NULL;
}

_Surface::_Surface ( _Surface * pSurfaceOld ): _Geometrie ( pSurfaceOld )
{
   pBRep = NULL;
}

_Surface::~_Surface()
{
  if ( pBRep != NULL )
    delete pBRep;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Normalenvektor der Fläche, falls alle Flächenteile im        //
//  Rahmen der Toleranz die gleiche Orientierung haben                       //
//  Wenn nicht, wird NULL geliefert                                          //
///////////////////////////////////////////////////////////////////////////////
Direction * _Surface::getSurfaceNormal ( double angleTolerance  )
{
  vector<PolygonGeo *> vPolygone;
  vector<bool>         vOrientations;
	size_t               i, anz;

  anz = getPolygone( vPolygone, vOrientations );

  if ( anz == 0 ) return NULL;

  Direction * dir = vPolygone[0]->getNormal();
  
  for ( i = 1; i < anz; i++ )
  {
    Direction * dirLoc = vPolygone[i]->getNormal();
    if ( !vOrientations[i] )
      dirLoc->reverse();

    double angleDist = dir->getDifference( *dirLoc );

    if ( !vOrientations[i] )
      dirLoc->reverse();

    if ( angleDist > 2.0 * angleTolerance )
      return NULL;
  }

  return dir;
}

///////////////////////////////////////////////////////////////////////////////
//  Offset auf den zug. BRep                                                 //
///////////////////////////////////////////////////////////////////////////////
void _Surface::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  )
{
  if ( pBRep != NULL )
    pBRep->offset( point, sGeometries );
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt die Knoten-Kantenrepräsentation (BREP) der Surface               //
///////////////////////////////////////////////////////////////////////////////
void  _Surface::generateTopology( double minDist, Features * pFeaturesP )
{
  if ( pBRep != NULL )
    delete pBRep;

  pBRep = new BRep ( this, minDist, pFeaturesP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die BRep-Repräsentation der Fläche                               //
///////////////////////////////////////////////////////////////////////////////
BRep * _Surface::getTopology()
{  
  return pBRep;
}

///////////////////////////////////////////////////////////////////////////////
//  Macht den Knoten-/Kantentest und bestimmt, welche Kanten mehrfach in     //
//  einer Richtung durchlaufen wurden, und ob Stützpunkte nicht übereinander-//
//  liegen. Weiterhin wird überprüft, ob sich einzelne Polygone flächenmäßig //
//  überlappen, durchdringen, oder ob eine Berührung "PunktAufLinie",        //
//  "Punkt auf Fläche" oder "Linie auf Fläche" stattfindet                   //
//  Die entsprechenden Geometrieobjekte werden erzeugt                       // 
///////////////////////////////////////////////////////////////////////////////
bool _Surface::topologyCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
        std::vector<Error*> &vErrorReport, bool checkAnyway )
{
  bool success1, success2;

  if ( checkResult < ADVANCED_CHECK_FULFILLED && !checkAnyway )
    return false;

  generateTopology( pCheckParams->minimalPointDistance, pObj->getFeatures() );
  success1 = pBRep->checkTopology ( pObj, featureProperty, gmlId, pCheckParams, vErrorReport );

  success2 = overlapCheck( pObj, featureProperty, pCheckParams, vErrorReport );

  if ( success1 == false || success2 == false )
    return false;
  else
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft,  ob Stützpunkte nicht übereinanderliegen.                     //
//  Weiterhin wird überprüft, ob sich einzelne Polygone flächenmäßig         //
//  überlappen, durchdringen, oder ob eine Berührung "PunktAufLinie",        //
//  "Punkt auf Fläche" oder "Linie auf Fläche" stattfindet                   //
//  Die entsprechenden Geometrieobjekte werden erzeugt                       // 
///////////////////////////////////////////////////////////////////////////////
bool _Surface::overlapCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway )
{
  vector<PolygonGeo*>  vPolygone;
	size_t               i, j, k, anz;
  IntersectionGeometry intersection;
  GeometryError      * pError;
//  MultiPoint         * pMultiPoint   = NULL;
  MultiCurve         * pMultiCurve   = NULL;
  MultiSurface       * pMultiSurface = NULL;

  if ( checkResult < ADVANCED_CHECK_FULFILLED && !checkAnyway )
    return false;

  anz = getPolygone ( vPolygone );
  for ( i = 0; i < anz; i++ )
  {
    PolygonGeo * pPoly = vPolygone[i];
    for ( j = i + 1; j < anz; j++ )
    {
      PolygonGeo * pPolyLoc = vPolygone[j];
      pPoly->intersect ( pPolyLoc, pCheckParams->minimalPointDistance, pCheckParams->minimalAngleDistance, false, intersection );
    }
  }

  for ( k = 0; k < intersection.vCutSegmentTypes.size(); k++ )
  {
    if ( intersection.vCutSegmentTypes[k] == FLAECHE_DURCH_FLAECHE )
    {
      if ( pMultiCurve == NULL ) pMultiCurve = new MultiCurve ( dim );
      pMultiCurve->addCurve( new LineSegment ( intersection.vCutSegments[k] ) );
    }
  }

  for ( k = 0; k < intersection.vCutRings.size(); k++ )
  {
    if ( pMultiSurface == NULL ) pMultiSurface = new MultiSurface ( dim );
    PolygonGeo * pPoly = new PolygonGeo ( dim );
    pPoly->setAussenkontur( new LinearRing ( intersection.vCutRings[k] ) );
    pMultiSurface->addSurface( pPoly );
  }

  if ( pMultiCurve != NULL )
  {
    pError = new GeometryError ( pObj, Error::ERROR, GeometryError::MULTISURFACE_IRREGULAR_LINE_TOUCH_INTERSECT );
    pError->setPropertyName( featureProperty );
    pError->setFeatureGeometryId( getGmlId() );
    pError->addErrorGeometry( pMultiCurve, "Berührungs- oder Durchdringungslinien" );
    vErrorReport.push_back( pError );
  }

  if ( pMultiSurface != NULL )
  {
    pError = new GeometryError ( pObj, Error::ERROR, GeometryError::MULTISURFACE_IRREGULAR_SURFACE_OVERLAP );
    pError->setPropertyName( featureProperty );
    pError->setFeatureGeometryId( getGmlId() );
    pError->addErrorGeometry( pMultiSurface, "Überlappungsflächen" );
    vErrorReport.push_back( pError );
  }

  if ( pMultiCurve != NULL || pMultiSurface != NULL )
    return false;
  else
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die gegenseitige Überlappung bzw. Durchdringung zweier _Surface//
//  Objekte                                                                  //
///////////////////////////////////////////////////////////////////////////////
POLYGON_TOPOLOGY _Surface::intersect ( _Surface *pSurface, double minDist, double minAngleDist, bool ignoreHoles, IntersectionGeometry & intersectGeo )
{
  vector<PolygonGeo*> vPolygoneA;
  vector<PolygonGeo*> vPolygoneB;
  double              sw[3], no[3];
  double              swLoc[3], noLoc[3];
  unsigned int        i, j;
  POLYGON_TOPOLOGY    polygonTopology = POLYGONE_DISJUNKT;
  POLYGON_TOPOLOGY    polygonTopologyLoc;

  getBoundingBox ( sw, no );
  pSurface->getBoundingBox ( swLoc, noLoc );

  if ( noLoc[0] < sw[0] - minDist || swLoc[0] > no[0] + minDist ||
    noLoc[1] < sw[1] - minDist || swLoc[1] > no[1] + minDist ||
    noLoc[2] < sw[2] - minDist || swLoc[2] > no[2] + minDist   )
    return POLYGONE_DISJUNKT;

  getPolygone( vPolygoneA );
  pSurface->getPolygone( vPolygoneB );

  for ( i = 0; i < vPolygoneA.size(); i++ )
  {
    PolygonGeo * pPolygonA = vPolygoneA[i];
    for ( j = 0; j < vPolygoneB.size(); j++ )
    {
      PolygonGeo * pPolygonB = vPolygoneB[j];
      polygonTopologyLoc = pPolygonA->intersect( pPolygonB, minDist, minAngleDist, ignoreHoles, intersectGeo );
      if ( polygonTopologyLoc > polygonTopology )
        polygonTopology = polygonTopologyLoc;
    }
  }

  return polygonTopology;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Überlappung der einzelnen Multi-Surface Member	      		 //
///////////////////////////////////////////////////////////////////////////////
bool _Surface::checkUeberlappung2D ( double minDist, double minAngleDist )
{
  vector<PolygonGeo*>  vPolygone;
  PolygonGeo         * pPolygonA;
  PolygonGeo         * pPolygonB;
  size_t               loc;
	size_t               i, j, anz;
  bool			           success;

  anz = getPolygone ( vPolygone );
  if ( anz <=1 ) return true;

  success = true;

  for ( i = 0; i < anz; i++ )
  {
    pPolygonA = vPolygone[i];
    for ( j = i + 1; j < anz; j++ )
    {
      pPolygonB =  vPolygone[j];
      loc = pPolygonA->checkTopology2D ( pPolygonB, minDist, minAngleDist );
      if ( loc != DISJUNKT && loc != DISJUNKT_BERUEHREND_SINGLE_POINT && loc != DISJUNKT_BERUEHREND_MULTI_POINT )
      {
        success = false;
        break;
      }
    }
  }

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt für ebene Flächen die Überlappungsfläche der einzelnen          //
//  Polygone der Fläche als Geometrie                                        //
///////////////////////////////////////////////////////////////////////////////
bool _Surface::getUeberlappungsflaeche2D ( double minDist, double minAngleDist,  MultiSurface * pUeberlappungsFlaeche )
{
  vector<PolygonGeo*>  vPolygone;
  PolygonGeo         * pPolygonA;
  PolygonGeo         * pPolygonB;
	size_t               i, j, anz;

  anz = getPolygone( vPolygone );
  if ( anz <= 1 ) return true;

  for ( i = 0; i < anz; i++ )
  {
    pPolygonA = vPolygone[i];
    for ( j = i + 1; j < anz; j++ )
    {
      pPolygonB =  vPolygone[j];
      pPolygonA->getUeberlappungsflaeche2D ( pPolygonB, minDist, minAngleDist, pUeberlappungsFlaeche );
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt für ebene Flächen die gesamte Größe der gegenseitigen Über-     //
//  lappungen der einzelnen Polygone                                         //
///////////////////////////////////////////////////////////////////////////////
bool _Surface::checkUeberlappung2D ( double minDist, double minAngleDist, double & ueberlappungsFlaeche )
{
  MultiSurface * pMultiSurface = new MultiSurface ( 2 );

  bool success = getUeberlappungsflaeche2D( minDist, minAngleDist, pMultiSurface );
  
  if ( success )
    ueberlappungsFlaeche = pMultiSurface->getFlaeche();
  else
    ueberlappungsFlaeche = 0.0;

  delete pMultiSurface;

  if ( ueberlappungsFlaeche > 0.0 )
    return false;
  else
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die gegenseitige Überlappung zweier ebener Flächen        		 //
///////////////////////////////////////////////////////////////////////////////
bool _Surface::checkUeberlappung2D ( _Surface * pSurface, double minDist, double minAngleDist )
{
  vector<PolygonGeo*> vPolygoneA;
  vector<PolygonGeo*> vPolygoneB;
  PolygonGeo        * pPolygonA;
  PolygonGeo        * pPolygonB;
	size_t              loc;
	size_t              i, j, anzA, anzB;
  bool			          success = true;
  double              sw[3], no[3];
  double              swLoc[3], noLoc[3];

  success   = getBoundingBox( swLoc, noLoc );
  bool success_A = pSurface->getBoundingBox( sw, no );

  if ( !success || !success_A )
    return true;

  if ( noLoc[0] < sw[0] - minDist || swLoc[0] > no[0] + minDist ||
    noLoc[1] < sw[1] - minDist || swLoc[1] > no[1] + minDist )
    return true;

  anzA = getPolygone ( vPolygoneA );
  anzB = pSurface->getPolygone ( vPolygoneB );

  if ( anzA == 0 || anzB == 0 ) return true;

  for ( i = 0; i < anzA; i++ )
  {
    pPolygonA = vPolygoneA[i];
    for ( j = 0; j < anzB; j++ )
    {
      pPolygonB = vPolygoneB[j];
      loc = pPolygonA->checkTopology2D ( pPolygonB, minDist, minAngleDist );
      if ( loc != DISJUNKT && loc != DISJUNKT_BERUEHREND_SINGLE_POINT &&  loc != DISJUNKT_BERUEHREND_MULTI_POINT )
      {
        success = false;
        break;
      }
    }
  }

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Überlappungsfläche zweier ebener Flächen              		 //
///////////////////////////////////////////////////////////////////////////////
bool _Surface::getUeberlappungsflaeche2D ( _Surface * pSurface, double minDist, double minAngleDist, MultiSurface * pUeberlappungsFlaeche )
{
  vector<PolygonGeo*> vPolygoneA;
  vector<PolygonGeo*> vPolygoneB;
  PolygonGeo        * pPolygonA;
  PolygonGeo        * pPolygonB;
	size_t              i, j, anzA, anzB;
  double              sw[3], no[3];
  double              swLoc[3], noLoc[3];

  bool success   = getBoundingBox( swLoc, noLoc );
  bool success_A = pSurface->getBoundingBox( sw, no );

  if ( !success || !success_A )
    return true;

  if ( noLoc[0] < sw[0] - minDist || swLoc[0] > no[0] + minDist ||
    noLoc[1] < sw[1] - minDist || swLoc[1] > no[1] + minDist )
    return true;

  anzA = getPolygone ( vPolygoneA );
  anzB = pSurface->getPolygone ( vPolygoneB );
  if ( anzA == 0 || anzB == 0 ) return true;

  for ( i = 0; i < anzA; i++ )
  {
    pPolygonA = vPolygoneA[i];
    for ( j = 0; j < anzB; j++ )
    {
      pPolygonB = vPolygoneB[j];
      pPolygonA->getUeberlappungsflaeche2D ( pPolygonB, minDist, minAngleDist, pUeberlappungsFlaeche );
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Größe der Überlappungsfläche zweier ebener Flächen     		 //
///////////////////////////////////////////////////////////////////////////////
bool _Surface::checkUeberlappung2D ( _Surface * pSurface, double minDist, double minAngleDist, double &ueberlappungsFlaeche )
{
  MultiSurface * pMultiSurface = new MultiSurface ( 2 );

  bool success = getUeberlappungsflaeche2D( pSurface, minDist, minAngleDist, pMultiSurface );
  if ( success )
  {
    ueberlappungsFlaeche = pMultiSurface->getFlaeche();
  }
  else
    ueberlappungsFlaeche = 0.0;

  delete pMultiSurface;

  if ( ueberlappungsFlaeche > 0.0 )
    return false;
  else
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine qualitative Aussage über die Orientierung (Normalenrichtung)//
//  der Fläche in Textform                                                   //
///////////////////////////////////////////////////////////////////////////////
string _Surface::convertOrientationToString ( SURFACE_NORMAL_TYPE type )
{
  switch ( type )
  {
  case VERTICAL_UPWARD:
    return "Senkrecht nach oben";
  case UPWARD:
    return "Nach oben";
  case HORIZONTAL:
    return "horizontal";
  case DOWNWARD:
    return "Nach unten";
  case VERTICAL_DOWNWARD:
    return "Senkrecht nach unten";
  case INDIFFERENT:
    return "Unbestimmt";
  }

  return "Unbestimmt";
}

///////////////////////////////////////////////////////////////////////////////
//  SurfaceReference                                                         //
//  Referenz auf eine _Surface                                               //
///////////////////////////////////////////////////////////////////////////////
SurfaceReference::SurfaceReference( Features * pFeaturesP ): _Surface ( 2 ), _GeometryReference ( pFeaturesP )
{
}

SurfaceReference::SurfaceReference ( SurfaceReference * pSurfaceReferenceOld )
:_Surface ( pSurfaceReferenceOld ), _GeometryReference ( pSurfaceReferenceOld )
{
}


SurfaceReference::~SurfaceReference()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Dimension der referierten Geometrie, oder 0 falls keine      //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
int SurfaceReference::getDim()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getDim();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den srsName des referierten Geometrie, oder "" falls keine       //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
std::string SurfaceReference::getSrsName ()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getSrsName();
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt das Polygon aus dem referierten _Surface Objekt.                77
//  Wenn das referierte Geometrieobjekt ein Polygon ist, wird keine          //
//  Änderung durchgeführt, sondern nur das referierte Geometrieobjekt        //
//  zurücKgeliefert:                                                         //
///////////////////////////////////////////////////////////////////////////////
_Surface * SurfaceReference::removePolygon ( std::string gmlIdP )
{
  if ( refGeoId == gmlIdP )
    return this;
  else
  {
    _Surface * pGeo = dynamic_cast<_Surface*>(pFeatures->getGeometry( refGeoId ));
    if ( pGeo != NULL ) 
      return pGeo->removePolygon( gmlIdP );
    else
      return NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den BoundingBox der referierten Geometrie, oder false falls keine//
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool SurfaceReference::getBoundingBox ( double * sw, double * no )
{

  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getBoundingBox( sw, no );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Center2D der referierten Geometrie, oder false falls keine   //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool SurfaceReference::getCenter2D ( double &xP, double &yP )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCenter2D( xP, yP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Center3D der referierten Geometrie, oder false falls keine   //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool SurfaceReference::getCenter3D ( double &xP, double &yP, double &zP )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCenter3D( xP, yP, zP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone der referierten Geometrie                //
///////////////////////////////////////////////////////////////////////////////
size_t SurfaceReference::getPolygonAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygonAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments der referierten Geometrie           //
///////////////////////////////////////////////////////////////////////////////
size_t SurfaceReference::getCurveSegmentAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCurveSegmentAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte der referierten Geometrie                  //
///////////////////////////////////////////////////////////////////////////////
size_t SurfaceReference::getPointAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPointAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Flächeninhalt mit Berücksichtigung von Löchern               //
//  der referierten Surface oder 0.0; falls keine gültige Referenz existiert //
///////////////////////////////////////////////////////////////////////////////
double SurfaceReference::getFlaecheMitLoechern ()
{
  _Surface * pGeo = dynamic_cast<_Surface*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getFlaecheMitLoechern();
  else
    return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Flächeninhalt ohne Berücksichtigung von Löchern              //
//  der referierten Surface                                                  //
//  oder 0.0; falls keine gültige Referenz existiert                         //
///////////////////////////////////////////////////////////////////////////////
double SurfaceReference::getFlaeche()
{
  _Surface * pGeo = dynamic_cast<_Surface*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getFlaeche();
  else
    return 0.0;
}

/////////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone der referierten Fläche, sowie die Anzahl der Polygone //
/////////////////////////////////////////////////////////////////////////////////
size_t SurfaceReference::getPolygone ( vector<PolygonGeo*> &vPolygone )
{
  _Surface * pGeo = dynamic_cast<_Surface*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getPolygone( vPolygone );
  else
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone der referierten Fläche mit den jeweiligen Orientierungen, //
//  sowie die Anzahl der Polygone                                                  //
/////////////////////////////////////////////////////////////////////////////////////
size_t SurfaceReference::getPolygone ( vector<PolygonGeo*> &vPolygone, vector<bool> &vOrientations )
{
  _Surface * pGeo = dynamic_cast<_Surface*>(pFeatures->getGeometry( refGeoId ));
  if (  pGeo != NULL  && pGeo->getGmlId() == gmlId )
    return 0;

  if ( pGeo != NULL ) 
    return pGeo->getPolygone( vPolygone, vOrientations );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den Basic-Check der referierten Fläche durch, falls dies in den    //
//  CheckingParams eingestellt ist. Der Returnwert ist false, falls die      //
//  referierte Fläche nicht existiert                                        //
///////////////////////////////////////////////////////////////////////////////
bool SurfaceReference::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  if ( !pCheckParams->checkReferencedGeometry )
    return true;

  _Surface * pGeo = dynamic_cast<_Surface*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den Advanved-Check der referierten Fläche durch, falls dies in den //
//  CheckingParams eingestellt ist. Der Returnwert ist false, falls die      //
//  referierte Fläche nicht existiert                                        //
///////////////////////////////////////////////////////////////////////////////
bool SurfaceReference::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( !pCheckParams->checkReferencedGeometry )
    return false;

  _Surface * pGeo = dynamic_cast<_Surface*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Existenz de referierten Geometrie sowie die Konsistenz     //
//  des Geometrietyps mit dem Geometrietyp der referierten Geometrie         //
///////////////////////////////////////////////////////////////////////////////
bool SurfaceReference::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  string          errorString;
  SemanticError * pError;
  GEOMETRY_TYPE   geoType;

  _Geometrie * pRefGeo = getRefGeoObjekt();
  if ( pRefGeo == NULL )
  {
    errorString = "Das Geometrieattribut " + propertyName + " hat ein nicht-aufgelöstes xlink: " + refGeoId;
    pError = new SemanticError ( pObj, Error::ERROR, errorString );  
    vErrorReport.push_back( pError );
    return false;
  }

  geoType = pRefGeo->getGeometryType();

  if ( geoType != SURFACE              && 
       geoType != COMPOSITE_SURFACE    &&
       geoType != TRIANGULATED_SURFACE && 
       geoType != POLYGON              &&
       geoType != ORIENTABLE_SURFACE   && 
       geoType != TRIANGLE    )
  {
    errorString = "Das Geometrieattribut " + propertyName + " verweist mit " + refGeoId + " auf einen falschen Geometrietyp. ";
    errorString = errorString + "Notwendig ist Flächengeometrie, verwiesen wird auf " + _Geometrie::convertTypeToString( geoType );
    pError = new SemanticError ( pObj, Error::ERROR, errorString );   
    vErrorReport.push_back( pError );
    return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der referierten Geometrie ein gültiges       //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool SurfaceReference::existReferenceSystem( SrsManager * pSrsManager )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->existReferenceSystem( pSrsManager );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine globale Angabe über die Orientierung der Fläche             //
///////////////////////////////////////////////////////////////////////////////
SURFACE_NORMAL_TYPE SurfaceReference::getSurfaceNormal(  double angleTolerance, double areaTolerance )
{
  _Surface * pGeo = dynamic_cast<_Surface*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getSurfaceNormal( angleTolerance, areaTolerance );
  else
    return INDIFFERENT;
}

///////////////////////////////////////////////////////////////////////////////
//  Offset auf das referierte Geometrieobjekt                                //
//  Über den Set von Geometrien wird sichergestellt, dass ein bestimmtes     //
//  Geometrieobjekt nur einmal verschoben wird                               //
///////////////////////////////////////////////////////////////////////////////
void SurfaceReference::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  )
{
  _Surface * pSurface = dynamic_cast<_Surface*>(getRefGeoObjekt());

  if ( pSurface != NULL && sGeometries.count( pSurface ) == 0 )
  {
    pSurface->offset( point, sGeometries );
    sGeometries.insert( pSurface );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  OrientableSurface                                                        //
//  Klasse für gml:OrientableSurface                                         //
///////////////////////////////////////////////////////////////////////////////
OrientableSurface::OrientableSurface ( int dimP )
: _Surface ( dimP )
{
  orientation  = true;
  pBaseSurface = NULL;
}

OrientableSurface::OrientableSurface ( OrientableSurface * pSurfaceOld )
:_Surface ( pSurfaceOld )
{
  orientation  = pSurfaceOld->orientation;
  if ( pSurfaceOld->pBaseSurface != NULL )
    pBaseSurface = dynamic_cast<_Surface*>(_Geometrie::copy(pSurfaceOld->pBaseSurface));
  else
    pBaseSurface = NULL;
}

OrientableSurface::~OrientableSurface()
{
  if ( pBaseSurface != NULL ) delete pBaseSurface;
}
///////////////////////////////////////////////////////////////////////////////
//  Liefert die BoundingBox                                                  //
///////////////////////////////////////////////////////////////////////////////
bool OrientableSurface::getBoundingBox ( double * sw, double * no )
{
  if ( pBaseSurface != NULL )
    return pBaseSurface->getBoundingBox ( sw, no );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt                                                  //
///////////////////////////////////////////////////////////////////////////////
bool OrientableSurface::getCenter2D ( double &cxP, double &cyP )
{
  if ( pBaseSurface != NULL )
    return pBaseSurface->getCenter2D ( cxP, cyP );
  else
    return false;
}

bool OrientableSurface::getCenter3D ( double &cxP, double &cyP, double &czP )
{
  if ( pBaseSurface != NULL )
    return pBaseSurface->getCenter3D ( cxP, cyP, czP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone                                          //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableSurface::getPolygonAnz()
{
  if ( pBaseSurface != NULL ) 
    return pBaseSurface->getPolygonAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableSurface::getCurveSegmentAnz()
{
  if ( pBaseSurface != NULL ) 
    return pBaseSurface->getCurveSegmentAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableSurface::getPointAnz()
{
  if ( pBaseSurface != NULL ) 
    return pBaseSurface->getPointAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt ein Polygon aus der Basisfläche                                 //
///////////////////////////////////////////////////////////////////////////////
_Surface * OrientableSurface::removePolygon( std::string gmlIdP )
{
  if ( pBaseSurface == NULL )
    return NULL;

  string refGeoId = pBaseSurface->getGmlId();

  if ( refGeoId == gmlIdP )
    return this;
  else
    return pBaseSurface->removePolygon( gmlIdP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Flächeninhalt (mit Löchern) der BaseSurface                  //
///////////////////////////////////////////////////////////////////////////////
double OrientableSurface::getFlaecheMitLoechern ()
{
  if ( pBaseSurface != NULL )
    return pBaseSurface->getFlaecheMitLoechern();
  else
    return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Flächeninhalt der BaseSurface ohne Berücksichtigung          //
//  von Löchern                                                              //
///////////////////////////////////////////////////////////////////////////////
double OrientableSurface::getFlaeche()
{
  if ( pBaseSurface != NULL )
    return pBaseSurface->getFlaeche();
  else
    return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone, aus denen die Basisfläche aufgebaut ist            //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableSurface::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
  if ( pBaseSurface != NULL )
    return pBaseSurface->getPolygone( vPolygone );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone, aus denen die Basisfläche aufgebaut ist, und die   //
//  zugehörigen Orientierungen                                               //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableSurface::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )
{
	size_t anz;

  if ( pBaseSurface != NULL )
  {
    anz = pBaseSurface->getPolygone( vPolygone,vOrientations );
    if ( orientation == false )
    {
      for ( unsigned int i = 0; i < anz; i++ )
        vOrientations[i] = !vOrientations[i];
    }
    return anz;
  }
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den Basic-Check für die BaseSurface durch                          //
///////////////////////////////////////////////////////////////////////////////
bool OrientableSurface::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  if ( pBaseSurface == NULL )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    vErrorReport.push_back( pError );
    return false;
  }

  return pBaseSurface->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den Advanced-Check für die BaseSurface durch                       //
///////////////////////////////////////////////////////////////////////////////
bool OrientableSurface::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( pBaseSurface != NULL )
    return pBaseSurface->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Existenz der von Basisfläche und die Konsistenz des        //
//  Geometrietyps mit dem Geometrietyp der Basisfläche                       //
///////////////////////////////////////////////////////////////////////////////
bool OrientableSurface::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  string          errorString;
  SemanticError * pError;
  GEOMETRY_TYPE   geoType;

  if ( pBaseSurface == NULL )
  {
    errorString = "Das Geometrieattribut " + propertyName + " verweist auf OrientableSurface ohne BaseSurface ";
    pError = new SemanticError ( pObj, Error::ERROR, errorString );    
    vErrorReport.push_back( pError );

    return false;
  }

  geoType = pBaseSurface->getGeometryType();

  if ( geoType != SURFACE              && 
       geoType != COMPOSITE_SURFACE    &&
       geoType != TRIANGULATED_SURFACE && 
       geoType != POLYGON              &&
       geoType != ORIENTABLE_SURFACE   && 
       geoType != TRIANGLE             &&
       geoType != SURFACE_REFERENCE )
  {
    errorString = "Das Geometrieattribut " + propertyName + " verweist auf eine OrientableSurface mit falschem Geometrietyp der BaseSurface: " +
                  _Geometrie::convertTypeToString( geoType );
    pError = new SemanticError ( pObj, Error::ERROR, errorString ); 
    vErrorReport.push_back( pError );

    return false;
  }

  return pBaseSurface->checkReferences( pObj, propertyName, vErrorReport);
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für die Geometrie ein gültiges                              //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool OrientableSurface::existReferenceSystem( SrsManager * pSrsManager )
{
  bool exist;

  if ( srsName != "" )
  {
    exist = pSrsManager->existCoordSystem ( srsName );
    if ( exist )
      return true;
  }

  if ( pBaseSurface != NULL )
    return pBaseSurface->existReferenceSystem( pSrsManager );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine globale Angabe über die Orientierung der Fläche             //
///////////////////////////////////////////////////////////////////////////////
SURFACE_NORMAL_TYPE OrientableSurface::getSurfaceNormal( double angleTolenance, double areaToleranceRelative )
{
  if ( pBaseSurface != NULL )
    return INDIFFERENT;

  SURFACE_NORMAL_TYPE ori = pBaseSurface->getSurfaceNormal( angleTolenance, areaToleranceRelative );
  switch ( ori )
  {
  case VERTICAL_UPWARD:
    if ( orientation )
      return VERTICAL_UPWARD;
    else
      return VERTICAL_DOWNWARD;

  case UPWARD:
    if ( orientation )
      return UPWARD;
    else
      return DOWNWARD;

  case VERTICAL_DOWNWARD:
    if ( !orientation )
      return VERTICAL_UPWARD;
    else
      return VERTICAL_DOWNWARD;

  case DOWNWARD:
    if ( !orientation )
      return UPWARD;
    else
      return DOWNWARD;

  case HORIZONTAL:
    return HORIZONTAL;
  }

  return INDIFFERENT;
}

///////////////////////////////////////////////////////////////////////////////
//  F                                                                   //
///////////////////////////////////////////////////////////////////////////////
void OrientableSurface::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  _Surface::offset( point, sGeometries );

  if ( pBaseSurface != NULL && sGeometries.count( pBaseSurface ) == 0 )
    pBaseSurface->offset( point, sGeometries );
 
  sGeometries.insert( this );
}

///////////////////////////////////////////////////////////////////////////////
//  _Curve	                                																 //
//  Abstrakte Oberklasse für gml:Curve (aufgebaut aus curve-Segments)	    	 //
//  und gml:LineString			                          											 //
///////////////////////////////////////////////////////////////////////////////
_Curve::_Curve( int dim ): _Geometrie( dim )
{

}

_Curve::_Curve ( _Curve * pCurveOld  ): _Geometrie ( pCurveOld )
{

}

_Curve::_Curve ( const _Curve & curveOld  ): _Geometrie ( curveOld )
{

}


_Curve::~_Curve()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Extrahiert den Teil der Curve, der durch das Längenintervall             //
// [lengthAnf,lengthEnd] bestimmt ist. Die Längenangaben beziehen sich auf   //
//  den Startpunkt der Curve                                                 //
///////////////////////////////////////////////////////////////////////////////
LineSegment * _Curve::extractCurvePart ( double lengthAnf, double lengthEnd )
{
  vector<GeoPoint> vPointsExtracted;
	size_t           pointAnz;

  pointAnz = extractCurvePart( lengthAnf, lengthEnd, vPointsExtracted );

  if ( vPointsExtracted.size() > 1 )
  {
    LineSegment * pLineSegment = new LineSegment( dim, LineSegment::_LINE_STRING );
    pLineSegment->addPoints( vPointsExtracted );
    return pLineSegment;
  }
  else
    return NULL;
}

size_t _Curve::extractCurvePart ( double lengthAnf, double lengthEnd, vector<GeoPoint> &vPointsExtracted )
{
  vector<GeoPoint> vPoints;
	size_t           i, pointAnz;
  double           length = 0;
  double           lengthOld;
  double           segLength;
  GeoPoint         pA(3), pE(3), p(3);
  bool             startSegmentFound = false;
  bool             endSegmentFound   = false;

  pointAnz = getPoints( vPoints );
  if ( pointAnz <= 1 ) return NULL;

  pA = vPoints[0];
  for ( i = 1; i < pointAnz; i++ )
  {
    pE        = vPoints[i];
    segLength = pE.dist( pA );
    lengthOld = length;
    length    = length +segLength;

    if ( !startSegmentFound )
    {
      if ( length >= lengthAnf )
      {
        startSegmentFound = true;
        p = pA + (pE - pA)*(( lengthAnf - lengthOld )/segLength);
        vPointsExtracted.push_back( p );
      }

      if ( length >= lengthEnd )
      {
        p = pA + (pE - pA)*(( lengthEnd - lengthOld )/segLength);
        vPointsExtracted.push_back( p );
        break;
      }
    }
    else
    {
      if ( length >= lengthEnd )
      {
        p = pA + (pE - pA)*(( lengthEnd - lengthOld )/segLength);
        vPointsExtracted.push_back( p );
        break;
      }
      else
        vPointsExtracted.push_back( pE );
    }

    pA = pE;
  }

  return vPointsExtracted.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Legt die Kopie eines _Curve Objektes an                                  //
///////////////////////////////////////////////////////////////////////////////
_Curve * _Curve::copy ( _Curve * pCurve )
{
	switch ( pCurve->getGeometryType() )
	{

	case CURVE_REFERENCE:
	{
		CurveReference * pCurveReference = (CurveReference*)pCurve;
		return new CurveReference ( pCurveReference );
	}

	case CURVE:
	{
		Curve * pCurveA = (Curve*)pCurve;
		return new Curve ( pCurveA );
	}

	case LINE_STRING:
	case LINE_STRING_SEGMENT:
	{
		LineSegment * pLineSegment = (LineSegment*)pCurve;
		return new LineSegment ( pLineSegment );
	}

	case ARC:
	case ARC_STRING:
	case CIRCLE:
	{
		ArcSegment * pArcSegment = (ArcSegment*)pCurve;
		return new ArcSegment ( pArcSegment );
	}

	case ORIENTABLE_CURVE:
	{
		OrientableCurve * pOrientableCurve = (OrientableCurve*)pCurve;
		return new OrientableCurve ( pOrientableCurve );
	}

	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht collineare Punkte aus der Punkteliste sowie den letzten Punkt,    //
///////////////////////////////////////////////////////////////////////////////
 void _Curve::deleteCollinearPoints ( std::vector<GeoPoint> & vPoints )
 {
   GeoPoint p1, p2, p3;
   size_t anz = vPoints.size();
   bool closed;
   bool complete = false;

   if ( anz < 3 )
     return;

   if ( vPoints[0] == vPoints[anz - 1] )
   {
     vPoints.pop_back();
     closed = true;
     anz = anz - 1;
   }
   else
     closed = false;

   if ( anz < 3 )
     return;

   size_t i = 0;
   size_t j = 1;
   size_t k = 2;

   while ( !complete )
   {
     if ( j == 0 )
       complete = true;

     p1 = vPoints[i];
     p2 = vPoints[j];
     p3 = vPoints[k];

     Direction d12 = Direction( p1, p2 );
     d12.normalize();
     Direction d23 = Direction ( p2, p3 );
     d23.normalize();

     double angle = 1.0 - fabs ( d12*d23 );
     if ( angle < MINIMAL_ANGLE_DISTANCE )
     {
       vPoints.erase ( vPoints.begin() + j );
       anz = anz - 1;
     }
     else
     {
       i = j;
       j++;
       k++;
     }

     if ( !closed && k == anz )
       break;
     else
     if ( closed )
     {
       if ( j == anz - 1 )
         k = 0;
       else
       if ( j == anz )
       {
         j = 0;
         k = 1;
       }
     }
  
   }
 
 }



///////////////////////////////////////////////////////////////////////////////
//  CurveReference                                                           //
//  Referenz auf eine _Curve                                                 //
///////////////////////////////////////////////////////////////////////////////
CurveReference::CurveReference( Features * pFeaturesP ): _Curve ( 2 ), _GeometryReference ( pFeaturesP )
{
}

CurveReference::CurveReference ( CurveReference * pCurveReferenceOld )
:_Curve( pCurveReferenceOld ), _GeometryReference ( pCurveReferenceOld )
{
}

CurveReference::~CurveReference()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Typ der referierten _Curve, oder CURVE_REFERENCE falls keine //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
GEOMETRY_TYPE CurveReference::getCurveType()
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getCurveType();
  else
    return CURVE_REFERENCE;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Dimension der referierten Geometrie, oder 0 falls keine      //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
int CurveReference::getDim()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getDim();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den srsName des referierten Geometrie, oder "" falls keine       //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
std::string CurveReference::getSrsName ()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getSrsName();
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den BoundingBox der referierten Geometrie, oder false falls keine//
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool CurveReference::getBoundingBox ( double * sw, double * no )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getBoundingBox( sw, no );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Center2D der referierten Geometrie, oder false falls keine   //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool CurveReference::getCenter2D ( double &xP, double &yP )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCenter2D( xP, yP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Center3D der referierten Geometrie, oder false falls keine   //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool CurveReference::getCenter3D ( double &xP, double &yP, double &zP )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCenter3D( xP, yP, zP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die referierte Curve auf doppelte Punkte                       //
//  Bei remove == true werden doppelte Punkte entfernt                       //
///////////////////////////////////////////////////////////////////////////////
bool CurveReference::checkDoublePoints ( double minDist, bool remove  )
{
  _Curve * pGeo = dynamic_cast<_Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->checkDoublePoints( minDist, remove );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den Basic-Check für die referierte Curve aus, falls dies in den    //
//  CheckingParams eingestellt ist                                           //
///////////////////////////////////////////////////////////////////////////////
bool CurveReference::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
            vector<Error*> &vErrorReport )
{
  if ( !pCheckParams->checkReferencedGeometry )
    return true;

  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den Advanced-Check für die referierte Curve aus, falls dies in den //
//  CheckingParams eingestellt ist                                           //
///////////////////////////////////////////////////////////////////////////////
bool CurveReference::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( !pCheckParams->checkReferencedGeometry )
    return true;

  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Existenz der referierten Curve sowie die Konsistenz        //
//  des Geometrietyps mit dem Geometrietyp des referierten Objektes          //
///////////////////////////////////////////////////////////////////////////////
bool CurveReference::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  string          errorString;
  SemanticError * pError;
  GEOMETRY_TYPE   geoType;

  _Geometrie * pRefGeo = getRefGeoObjekt();
  if ( pRefGeo == NULL )
  {
    errorString = "Das Geometrieattribut " + propertyName + " hat ein nicht-aufgelöstes xlink: " + refGeoId;
    pError = new SemanticError ( pObj, Error::ERROR, errorString );  
    vErrorReport.push_back( pError );

    return false;
  }

  geoType = pRefGeo->getGeometryType();

  if ( geoType != CURVE                && 
       geoType != LINE_STRING          &&
       geoType != LINE_STRING_SEGMENT  &&
       geoType != ARC                  && 
       geoType != ARC_STRING           && 
       geoType != CIRCLE    )
  {
    errorString = "Das Geometrieattribut " + propertyName + " verweist mit " + refGeoId + " auf einen falschen Geometrietyp. ";
    errorString = errorString + "Notwendig ist Flächengeometrie, verwiesen wird auf " + _Geometrie::convertTypeToString( geoType );
    pError = new SemanticError ( pObj, Error::ERROR, errorString );    
    vErrorReport.push_back( pError );

    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone der referierten Geometrie                //
///////////////////////////////////////////////////////////////////////////////
size_t CurveReference::getPolygonAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygonAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone der referierten Geometrie                           //
///////////////////////////////////////////////////////////////////////////////
size_t CurveReference::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygone( vPolygone );
  else
    return 0;
}

size_t CurveReference::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygone( vPolygone, vOrientations );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments der referierten Geometrie           //
///////////////////////////////////////////////////////////////////////////////
size_t CurveReference::getCurveSegmentAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCurveSegmentAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Stützpunkte der referierten Geometrie             //
///////////////////////////////////////////////////////////////////////////////
size_t CurveReference::getPointAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPointAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Dreht die Laufrichtung der referierten Curve um                          //
///////////////////////////////////////////////////////////////////////////////
void CurveReference::reverse()
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    pGeo->reverse();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Stützpunkte der referierten Kurve als Text im SVG-Format     //
///////////////////////////////////////////////////////////////////////////////
string CurveReference::getPointsSVG ( double offsetX, double offsetY )
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getPointsSVG( offsetX, offsetY);
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Stützpunkte der referierten Kurve in umgekehrter Reihenfolge //
//  als Text im SVG-Format                                                   //
///////////////////////////////////////////////////////////////////////////////
string CurveReference::getPointsReverseSVG ( double offsetX, double offsetY )
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getPointsReverseSVG( offsetX, offsetY );
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Stützpunkte eines referierten Kurvensegments als Text        //
//  im SVG-Format.                                                           //
//  Hilfsroutine zur Implementierung von getPointsSVG                        //
///////////////////////////////////////////////////////////////////////////////
string CurveReference::getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode )
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getSegmentPointsSVG( offsetX, offsetY, mode );
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Stützpunkte eines referierten Kurvensegments in umgekehrter  //
//  Reihenfolge als Text im SVG-Format.                                      //
//  Hilfsroutine zur Implementierung von getPointsSVG                        //
///////////////////////////////////////////////////////////////////////////////
string CurveReference::getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode )
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getSegmentPointsReverseSVG( offsetX, offsetY, mode );
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die x und y-Koordinaten der Stützpunkte der referierten Curve    //  
///////////////////////////////////////////////////////////////////////////////
size_t CurveReference::getPoints2D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP )
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getPoints2D( xCoordsP,yCoordsP );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die x und y-Koordinaten der Stützpunkte der referierten Curve    //  
//  in umgekehrter Reihenfolge                                               //
///////////////////////////////////////////////////////////////////////////////
size_t CurveReference::getPoints2DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP )
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getPoints2DReverse( xCoordsP,yCoordsP );
  else
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die x, y und z-Koordinaten der Stützpunkte der referierten       //  
//   Curve                                                                   //
///////////////////////////////////////////////////////////////////////////////
size_t CurveReference::getPoints3D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, 
          std::vector<double> &zCoordsP )
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getPoints3D( xCoordsP,yCoordsP,zCoordsP );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die x, y und z-Koordinaten der Stützpunkte der referierten       //  
//   Curve in umgekehrter Reihenfolge                                        //
///////////////////////////////////////////////////////////////////////////////
size_t CurveReference::getPoints3DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, 
          std::vector<double> &zCoordsP )
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getPoints3DReverse( xCoordsP,yCoordsP,zCoordsP );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Stützpunkte der referierten Curve als GeoPoint Objekte       //
///////////////////////////////////////////////////////////////////////////////
size_t CurveReference::getPoints ( std::vector<GeoPoint> &vPoints )
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getPoints( vPoints );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Anfangs- bzw. Endpunkt der referierten Curve                 //
///////////////////////////////////////////////////////////////////////////////
GeoPoint CurveReference::getAnfangsPunkt ()
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getAnfangsPunkt();
  else
    return GeoPoint();
}

GeoPoint CurveReference::getEndPunkt()
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getEndPunkt();
  else
    return GeoPoint();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Länge der referierten Curve                                  //
///////////////////////////////////////////////////////////////////////////////
double CurveReference::getLength ()
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getLength();
  else
    return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt Punkte auf der referierten Curve mit Abstand scanDist 			     //
//  Ist die Länge der Kurve kein ganzzahliges Vielfaches von scanDist, wird  //
//  dieser Parameter entsprechend angepasst									                 //
///////////////////////////////////////////////////////////////////////////////
size_t CurveReference::scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
                         std::vector<double> &angleP, double scanDist, bool reverse )
{
  _Curve * pGeo = dynamic_cast<Curve*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->scanCurve2D( xP, yP, angleP, scanDist, reverse );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der referierten Geometrie ein gültiges       //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool CurveReference::existReferenceSystem( SrsManager * pSrsManager )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->existReferenceSystem( pSrsManager );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Offset auf die referierte Curve                                          //
//  Über den Set von Geometrieobjekten wird sichergestellt, dass eine        //
//  bestimmte Curve nur einmal verschoben wird                               //
///////////////////////////////////////////////////////////////////////////////
void CurveReference::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  )
{
  _Curve * pCurve = dynamic_cast<_Curve*>(getRefGeoObjekt());

  if ( pCurve != NULL && sGeometries.count( pCurve ) == 0 )
  {
    pCurve->offset( point, sGeometries );
    sGeometries.insert( pCurve );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  _Ring							                                    									 //
//  Implementierung der abstrakten Oberklasse für gml:Ring und      				 //
//  gml:LinearRing													                            		 //
///////////////////////////////////////////////////////////////////////////////
_Ring::_Ring( int dim ): _Geometrie( dim )
{
  e = NULL;
}

_Ring::_Ring ( _Ring * pRingOld ): _Geometrie ( pRingOld )
{
  if ( pRingOld->e != NULL )
    e = new Plane ( pRingOld->e );
  else
    e = NULL;
}

_Ring:: ~_Ring()
{
  if ( e != NULL ) delete e;
}


///////////////////////////////////////////////////////////////////////////////
//  Berechnet den von h1, h2, h3 eingeschlossenen Winkel		          			 //
///////////////////////////////////////////////////////////////////////////////
double _Ring::calcWinkel2D ( double *h1, double * h2, double * h3 )
{
    double k1[2], k2[2];
    double k1Abs, k2Abs;
    double sinAlp;   // sin ( Winkels )
    double cosAlp;   // cos ( Winkels );
    int    i;

    for ( i = 0; i < 2; i++ )
    {
        k1[i] = h2[i] - h1[i];
        k2[i] = h3[i] - h2[i];
    }

    k1Abs = sqrt ( k1[0]*k1[0] + k1[1]*k1[1] );
    if ( k1Abs < 1.0E-5 )
        return 0.0;

    k2Abs = sqrt ( k2[0]*k2[0] + k2[1]*k2[1] );
    if ( k2Abs < 1.0E-5 )
        return 0.0;

    for ( i = 0; i < 2; i++ )
    {
        k1[i] = k1[i] / k1Abs;
        k2[i] = k2[i] / k2Abs;
    }

    sinAlp = k1[0]*k2[1] - k1[1]*k2[0];  // k1 x k2
    cosAlp = k1[0]*k2[0] + k1[1]*k2[1];  // k1 * k2;

	if ( fabs ( cosAlp ) < 1.0E-4 )
	{
		if ( sinAlp > 0 )
			return 0.5*PI;
		else
			return -0.5*PI;
	}
	else
	    return atan2 ( sinAlp, cosAlp );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Umlaufsinn eines ebenen Ringes		                					 //
///////////////////////////////////////////////////////////////////////////////
UMLAUFSINN _Ring::getUmlaufsinn2D()
{
	vector<double> vX;
	vector<double> vY;
	double		     winkelsumme;

	getPoints2D ( vX, vY );
	winkelsumme = getWinkelsumme( vX, vY );
	
	if ( fabs ( winkelsumme ) < 0.5 || fabs ( winkelsumme ) > 7.0 )
	{
		return UNDEFINED;
	}

  if ( winkelsumme > 0.0 )
		return POSITIVE;
  else
		return NEGATIVE;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Winkelsumme der Stützpunkte eines ebenen Ringes    					 //
///////////////////////////////////////////////////////////////////////////////
double _Ring::getWinkelsumme ( vector<double> &vX, vector<double> &vY )
{
  double h[2], h1[2], h2[2];
  double winkelsumme;
	size_t i, i1, i2, anz;

  winkelsumme = 0.0;
  anz         = vX.size();
	if ( anz == 0 )
		return 0.0;

  for ( i = 0; i < anz-1; i++ )
  {
    i1 = i+1;
    if ( i < anz-2 ) i2 = i+2;
    else			 i2 = 1;

    h[0] = vX[i];
    h[1] = vY[i];
    h1[0] = vX[i1];
    h1[1] = vY[i1];
    h2[0] = vX[i2];
    h2[1] = vY[i2];

    winkelsumme = winkelsumme + calcWinkel2D ( h, h1, h2 );
  }
  return winkelsumme;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die gegenseitige Lagebeziehung der beiden 2D Ringe  	         //
///////////////////////////////////////////////////////////////////////////////
AREA_LOCATION _Ring::checkTopology2D ( _Ring * pRingTest, double minDist, double minAngleDist )
{
	vector<double>       x, y;
  vector<double>       xTest, yTest;
  vector<unsigned int> vCutIndices;
  vector<unsigned int> vCutIndicesLoc;

	size_t			   i, j, schnittAnz;
	size_t         punktAnzGesA,  punktAnzInnenA, punktAnzAussenA, punktAnzRandA, punktAnzahlVertexA;
	size_t         punktAnzGesB,  punktAnzInnenB, punktAnzAussenB, punktAnzRandB, punktAnzahlVertexB;
	size_t         punktAnzGesAM1, punktAnzGesBM1;
	unsigned int   cutIndex = 0;
  bool           A1, A2, A3, A4, A5;
  bool           B1, B2, B3, B4, B5;
	POINT_LOCATION loc;
  AREA_LOCATION  areaLoc;
	double         point[2];
	double         sw[3], no[3];
	double         swLoc[3], noLoc[3];
  double         p1x, p1y, p2x, p2y, q1x, q1y, q2x, q2y;
  CUT_RESULT     cutResult;

	getBoundingBox ( sw, no );
	pRingTest->getBoundingBox ( swLoc, noLoc );

	if ( noLoc[0] < sw[0] - minDist || swLoc[0] > no[0] + minDist ||
		 noLoc[1] < sw[1] - minDist || swLoc[1] > no[1] + minDist )
		 return DISJUNKT;

	punktAnzGesA = pRingTest->getPoints2D ( x, y );
	
	punktAnzInnenA     = 0;
	punktAnzAussenA    = 0;
  punktAnzRandA      = 0;
  punktAnzahlVertexA = 0;


	for ( i = 0; i < punktAnzGesA-1; i++ )
	{
		point[0] = x[i];
		point[1] = y[i];

		loc = checkInside2D ( point, minDist, cutIndex, true );
		switch ( loc )
		{
		case IN_POLYGON:
			punktAnzInnenA++;
			break;
		case ON_BOUNDARY:
			punktAnzRandA++;
      vCutIndices.push_back( cutIndex );
			break;
		case OUTSIDE_POLYGON:
			punktAnzAussenA++;
      break;
    case ON_VERTEX:
      punktAnzahlVertexA++;
      break;
		}
	}

  x.clear();
  y.clear();
  punktAnzGesB = getPoints2D ( x, y );

  punktAnzInnenB     = 0;
  punktAnzAussenB    = 0;
  punktAnzRandB      = 0;
  punktAnzahlVertexB = 0;

  for ( i = 0; i < punktAnzGesB-1; i++ )
  {
    point[0] = x[i];
    point[1] = y[i];

    loc = pRingTest->checkInside2D ( point, minDist, cutIndex, true );
    switch ( loc )
    {
    case IN_POLYGON:
      punktAnzInnenB++;
      break;
    case ON_BOUNDARY:
      vCutIndicesLoc.push_back( cutIndex );
      punktAnzRandB++;
      break;
    case OUTSIDE_POLYGON:
      punktAnzAussenB++;
      break;
    case ON_VERTEX:
      punktAnzahlVertexB++;
      break;
    }
  }

  A1 = (punktAnzInnenA == punktAnzGesA-1);
  A2 = (punktAnzahlVertexA + punktAnzRandA > 0 && punktAnzInnenA > 0  && punktAnzInnenA  + punktAnzahlVertexA + punktAnzRandA == punktAnzGesA-1);
  A3 = (punktAnzahlVertexA + punktAnzRandA > 0 && punktAnzAussenA > 0 && punktAnzAussenA + punktAnzahlVertexA + punktAnzRandA == punktAnzGesA-1);
  A4 = (punktAnzAussenA == punktAnzGesA-1);
  A5 = (punktAnzahlVertexA  + punktAnzRandA == punktAnzGesA-1);

  B1 = (punktAnzInnenB == punktAnzGesB-1);
  B2 = (punktAnzahlVertexB + punktAnzRandB > 0 && punktAnzInnenB > 0  && punktAnzInnenB  + punktAnzahlVertexB + punktAnzRandB == punktAnzGesB-1);
  B3 = (punktAnzahlVertexB + punktAnzRandB > 0 && punktAnzAussenB > 0 && punktAnzAussenB + punktAnzahlVertexB + punktAnzRandB  == punktAnzGesB-1);
  B4 = (punktAnzAussenB == punktAnzGesB-1);
  B5 = (punktAnzahlVertexB + punktAnzRandB == punktAnzGesB-1);

  areaLoc = DISJUNKT;

  if (A1 && B4) 
    areaLoc = INNEN;
  else
  if ( (A2 && B4) || (A2 && B3) || (A1 && B3) ) 
  {
    if ( punktAnzahlVertexA + punktAnzRandA > 1 || punktAnzahlVertexB + punktAnzRandB > 1 )
      areaLoc = INNEN_BERUEHREND_MULTI_POINT;
    else
      areaLoc = INNEN_BERUEHREND_SINGLE_POINT;
  }
  else
  if ( A5 && B3)
  {
    IntersectionGeometry intersection;

    bool result = cut2D( pRingTest, minDist, intersection );
    if ( result == true && intersection.vCutRings.size() == 0 )
      areaLoc = DISJUNKT_BERUEHREND_MULTI_POINT;
    else
    {
      areaLoc = INNEN_BERUEHREND_MULTI_POINT;
      for ( i = 0; i < (int)intersection.vCutRings.size(); i++ )
        delete intersection.vCutRings[i];
    }
  }
  else
  if (  (A3 && B1) || (A3 && B2) || ( A4 && B2 ) ) 
  {
    if ( punktAnzahlVertexA + punktAnzRandA > 1 || punktAnzahlVertexB + punktAnzRandB > 1 )
      areaLoc = AUSSEN_BERUEHREND_MULTI_POINT;
    else
      areaLoc = AUSSEN_BERUEHREND_SINGLE_POINT;
  }
  else
  if ( A3 && B5) 
  {
    IntersectionGeometry intersection;

    bool result = cut2D( pRingTest, minDist, intersection );
    if ( result == true && intersection.vCutRings.size() == 0 )
      areaLoc = DISJUNKT_BERUEHREND_MULTI_POINT;
    else
    {
      areaLoc = AUSSEN_BERUEHREND_MULTI_POINT;
      for ( i = 0; i < (int)intersection.vCutRings.size(); i++ )
        delete intersection.vCutRings[i];
    }
  }
  else
  if ( A4 && B1 ) 
    areaLoc = AUSSEN;
  else
  if ( A5 && B5 ) 
    areaLoc = GLEICH;
  else
  if ( (A3 && B4) || (A4 && B3) || (A3 && B3) ) 
  {
    if ( punktAnzahlVertexA + punktAnzRandA > 1 || punktAnzahlVertexB + punktAnzRandB > 1 )
      areaLoc = DISJUNKT_BERUEHREND_MULTI_POINT;
    else
      areaLoc = DISJUNKT_BERUEHREND_SINGLE_POINT;
  }
  else
  if ( A4 && B4 ) 
    areaLoc = DISJUNKT;
  else
    areaLoc = UEBERLAPPEND;

  i = 0;
  j = 0;
  schnittAnz = 0;
  if ( areaLoc == DISJUNKT || areaLoc == DISJUNKT_BERUEHREND_SINGLE_POINT || areaLoc == DISJUNKT_BERUEHREND_MULTI_POINT ||  
       areaLoc == INNEN_BERUEHREND_MULTI_POINT || areaLoc == INNEN_BERUEHREND_SINGLE_POINT || areaLoc == AUSSEN_BERUEHREND_MULTI_POINT || areaLoc == AUSSEN_BERUEHREND_SINGLE_POINT )
  {
    punktAnzGesA = pRingTest->getPoints2D ( xTest, yTest );

    punktAnzGesBM1 = punktAnzGesB -1 ;
    for ( i = 0; i < punktAnzGesBM1; i++ )
    {
      p1x = x[i];
      p1y = y[i];
      p2x = x[i+1];
      p2y = y[i+1];
      punktAnzGesAM1 = punktAnzGesA - 1;
      for ( j = 0; j < punktAnzGesAM1; j++ )
      {
        q1x = xTest[j];
        q1y = yTest[j];
        q2x = xTest[j+1];
        q2y = yTest[j+1];

        cutResult = _Geometrie::schneidet ( p1x, p1y, p2x, p2y, q1x, q1y, q2x, q2y, minDist, minAngleDist );
        if ( cutResult == TRUE_CUT )
        {
          areaLoc = UEBERLAPPEND;
          break;
        }
      }
      if ( areaLoc == UEBERLAPPEND )
        break;
    }
  }

  return areaLoc;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die gegenseitige Lagebeziehung der beiden 2D Ringe nach        //
//  folgender Einteilung: Disjunkt, punktförmige Berührung an Vertices,      // 
//  punktförmige Berührung an Außenkontur,                                   //
//  Berührung, flächenhafte Überlappung                                      //
///////////////////////////////////////////////////////////////////////////////
POLYGON_TOPOLOGY _Ring::checkTopology2DGlobal ( _Ring * pRingTest, double minDist )
{
  vector<double>    x, y;
  vector<double>    xTest, yTest;
  vector<size_t>    vCutIndices;
  vector<size_t>    vCutIndicesLoc;

	size_t            i, j;
	unsigned int      cutIndex = 0;
	size_t            punktAnzGesA,  punktAnzInnenA, punktAnzAussenA, punktAnzRandA, punktAnzVertexA;
	size_t            punktAnzGesB,  punktAnzInnenB, punktAnzAussenB, punktAnzRandB, punktAnzVertexB;
  POINT_LOCATION    loc;
  double            point[2];
  double            sw[3], no[3];
  double            swLoc[3], noLoc[3];
  bool twoPointsOnSegment = false;

  getBoundingBox ( sw, no );
  pRingTest->getBoundingBox ( swLoc, noLoc );

  if ( noLoc[0] < sw[0] - minDist || swLoc[0] > no[0] + minDist ||
    noLoc[1] < sw[1] - minDist || swLoc[1] > no[1] + minDist )
    return POLYGONE_DISJUNKT;

  punktAnzGesA = pRingTest->getPoints2D ( xTest, yTest );

  punktAnzInnenA     = 0;
  punktAnzAussenA    = 0;
  punktAnzRandA      = 0;
  punktAnzVertexA    = 0;

  for ( i = 0; i < punktAnzGesA-1; i++ )
  {
    point[0] = xTest[i];
    point[1] = yTest[i];

    loc = checkInside2D ( point, minDist, cutIndex, true );
    switch ( loc )
    {
    case IN_POLYGON:
      punktAnzInnenA++;
      break;

    case ON_BOUNDARY:
      vCutIndices.push_back( cutIndex );
      punktAnzRandA++;
      break;

    case ON_VERTEX:
      punktAnzVertexA++;
      break;

    case OUTSIDE_POLYGON:
      punktAnzAussenA++;
      break;
    }
  }

  punktAnzGesB = getPoints2D ( x, y );

  punktAnzInnenB     = 0;
  punktAnzAussenB    = 0;
  punktAnzRandB      = 0;
  punktAnzVertexB    = 0;

  for ( i = 0; i < punktAnzGesB-1; i++ )
  {
    point[0] = x[i];
    point[1] = y[i];

    loc = pRingTest->checkInside2D ( point, minDist, cutIndex, true );
    switch ( loc )
    {
    case IN_POLYGON:
      punktAnzInnenB++;
      break;

    case ON_BOUNDARY:
      vCutIndicesLoc.push_back( cutIndex );
      punktAnzRandB++;
      break;

    case ON_VERTEX:
      punktAnzVertexB++;
      break;

    case OUTSIDE_POLYGON:
      punktAnzAussenB++;
      break;
    }
  }

  if ( punktAnzInnenA > 0 || punktAnzInnenB > 0 )
    return FLAECHE_AUF_FLAECHE;

	if (punktAnzRandA + punktAnzVertexA == punktAnzGesA-1 ||
		  punktAnzRandB + punktAnzVertexB == punktAnzGesB-1 )
		return FLAECHE_AUF_FLAECHE;

  if ( punktAnzAussenA == punktAnzGesA-1 && punktAnzAussenB == punktAnzGesB-1 )
    return POLYGONE_DISJUNKT;

  if ( punktAnzRandA > 0 )
  {
    if ( vCutIndices.size() > 1 )
    {
      for ( i = 0; i < vCutIndices.size(); i++ )
        for ( j = i+1; j < vCutIndices.size(); j++ )
          if ( vCutIndices[i] == vCutIndices[j] )
            twoPointsOnSegment = true;
    }

    if ( punktAnzRandB > 0 )
      return LINIE_AUF_LINIE;
    else
    {
      if ( twoPointsOnSegment )
        return LINIE_AUF_LINIE;
      else
        return PUNKT_AUF_LINIE;
    }
  }
  else
  {
    if ( punktAnzRandB > 0 )
    {
      if ( vCutIndicesLoc.size() > 1 )
      {
        for ( i = 0; i < vCutIndicesLoc.size(); i++ )
          for ( j = i+1; j < vCutIndicesLoc.size(); j++ )
            if ( vCutIndicesLoc[i] == vCutIndicesLoc[j] )
              twoPointsOnSegment = true;
      }
      if ( twoPointsOnSegment )
        return LINIE_AUF_LINIE;
      else
        return PUNKT_AUF_LINIE;
    }
    else
      return PUNKT_AUF_PUNKT;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die topologische Beziehung zweier 3D-Ringe                     //
///////////////////////////////////////////////////////////////////////////////
POLYGON_TOPOLOGY _Ring::intersect2D ( _Ring *pRing, double minDist, IntersectionGeometry & intersection )
{
  POLYGON_TOPOLOGY loc = checkTopology2DGlobal( pRing, minDist );
  if ( loc != POLYGONE_DISJUNKT )
  {
    if ( loc != FLAECHE_AUF_FLAECHE )
      touch2D( pRing, minDist, intersection );
    else
      cut2D( pRing, minDist, intersection );
  }

  return loc;     
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Schnittgeometrie zweier 3D Ringe                            //
///////////////////////////////////////////////////////////////////////////////
POLYGON_TOPOLOGY _Ring::intersect3D ( _Ring *pRing, Plane * e1, Plane * e2, double minDist, double minAngleDist, IntersectionGeometry & intersection )
{
	size_t           i;
  Line             cutLine;
	size_t           index;
  double           lineParam;
  double           lineParamLoc;
  double           lineParamStart;
  double           lineParamEnd;
  vector<GeoPoint> vSortedPoints;
  vector<GeoPoint> vSortedPointsLoc;
  vector<double>   vLineParams;
  vector<double>   vLineparamsLoc;
  vector<int>      vPointTypes;
  vector<int>      vPointTypesLoc;
  GeoPoint         normalization;
	size_t           cutPointAnz, cutPoinAnzLoc;
  POLYGON_TOPOLOGY result = POLYGONE_DISJUNKT;
  bool             success;

	vector<GeoPoint> vPoints;
	vector<GeoPoint> vRingPoints;

	getPoints(vPoints);
	pRing->getPoints(vRingPoints);

  //  Bestimmung der Schnittlinie der Ring-Ebenen
  success = e1->cut ( *e2, cutLine, minAngleDist );
  if ( !success )
  {
    double dist = e1->getDistance( e2->getCenter() );
		if (dist < minDist  || fabs ( dist -PI ) < minDist )
		{
			Matrix4 trans = e->getTransformationToHorizontal();
			
			transform( trans);
			pRing->transform(trans);

			intersect2D ( pRing, minDist, intersection );

			for (unsigned int i = 0; i < intersection.vCutRings.size(); i++)
				intersection.vCutRings[i]->transformInvers(trans);

			transformInvers(trans);
			pRing->transformInvers(trans);

			if (intersection.vCutRings.size() > 0)
				return FLAECHE_AUF_FLAECHE;
			else
				POLYGONE_DISJUNKT;
		}
    else
      return POLYGONE_DISJUNKT;
  }


	try
	{
		//  Für beide Ringe:
		//  Bestimmung der Punkte und Linienstücke, in denen die Schnittlinie den jeweiligen Ring schneidet oder berührt
		//  Charakterisierung der Linien als Innere Linien oder Randlinien
		cutPointAnz = getCutPoints(cutLine, vSortedPoints, vLineParams, vPointTypes, minDist);
		cutPoinAnzLoc = pRing->getCutPoints(cutLine, vSortedPointsLoc, vLineparamsLoc, vPointTypesLoc, minDist);
		if (cutPointAnz == 0 || cutPoinAnzLoc == 0)
			return POLYGONE_DISJUNKT;

		//  Bestimmung des Durchschnittes der Linienstücke und Randpunkte, und Charakterisierung der resultierenden Geometrie:
		//  - Linie auf Linie: Bei zwei inneren Linien ist es eine Durchdringung, Bei innerer Linie und Randlinie eine linienhafte 
		//                     Berührung auf der Fläche, ansonsten eine linienhafte Berührung am Rand
		//  - Linie auf Punkt: Bei innerer Linie ist es eine punktförmige Berührung auf der Fläche, ansonsten eine punktförmige
		//                     Berührung auf der Außenkontur
		//  - Punkt auf Punkt: Punktförmige Berührung auf der Außenkontur

		index = 0;
		while (index < vPointTypes.size())
		{
			GeoPoint cutPoint = vSortedPoints[index];

			if (vPointTypes[index] == ISOLATED)
			{
				lineParam = vLineParams[index];

				//  Feststellen, ob lineParam in vLineparamsLoc enthalten ist
				for (i = 0; i < vLineparamsLoc.size(); i++)
				{
					lineParamLoc = vLineparamsLoc[i];

					if (vLineparamsLoc[i] > lineParam + minDist && (vPointTypesLoc[i] == START_CONTOUR_SEGMENT || vPointTypesLoc[i] == START_CONTOUR_SEGMENT ||
						vPointTypesLoc[i] == ISOLATED))
						break;

					if (lineParamLoc < lineParam - minDist && (vPointTypesLoc[i] == END_CONTOUR_SEGMENT || vPointTypesLoc[i] == END_CONTOUR_SEGMENT ||
						vPointTypesLoc[i] == ISOLATED))
						continue;

					if (fabs(lineParam - lineParamLoc) <= minDist)
					{
						intersection.vCutPoints.push_back(cutPoint);
						intersection.vCutPointTypes.push_back(PUNKT_AUF_PUNKT);
						if (result < PUNKT_AUF_PUNKT) result = PUNKT_AUF_PUNKT;
					}

					if (vPointTypesLoc[i] == START_CONTOUR_SEGMENT && lineParamLoc < lineParam - minDist && vLineparamsLoc[i + 1] > lineParam + minDist)
					{
						intersection.vCutPoints.push_back(cutPoint);
						intersection.vCutPointTypes.push_back(PUNKT_AUF_LINIE);
						if (result < PUNKT_AUF_LINIE) result = PUNKT_AUF_LINIE;
						break;
					}

					if (vPointTypesLoc[i] == START_INNER_SEGMENT && lineParamLoc < lineParam - minDist && vLineparamsLoc[i + 1] > lineParam + minDist)
					{
						intersection.vCutPoints.push_back(cutPoint);
						intersection.vCutPointTypes.push_back(PUNKT_AUF_FLAECHE);
						if (result < PUNKT_AUF_FLAECHE) result = PUNKT_AUF_FLAECHE;
						break;
					}
				}
			}

			if (vPointTypes[index] == START_CONTOUR_SEGMENT || vPointTypes[index] == START_INNER_SEGMENT)
			{
				lineParamStart = vLineParams[index];
				lineParamEnd = vLineParams[index + 1];

				//  Feststellen, ob [lineParamStart,lineParamEnd] ein Segment von vLineParamsLoc überlappt oder berührt
				//  oder ob es im Segment einen isolierten Punkt von vLineParamsLoc gibt
				for (i = 0; i < vLineparamsLoc.size(); i++)
				{
					lineParamLoc = vLineparamsLoc[i];

					if (lineParamLoc > lineParamEnd + minDist && (vPointTypesLoc[i] == START_CONTOUR_SEGMENT || vPointTypesLoc[i] == START_CONTOUR_SEGMENT ||
						vPointTypesLoc[i] == ISOLATED))
						break;

					if (lineParamLoc < lineParamStart - minDist && (vPointTypesLoc[i] == END_CONTOUR_SEGMENT || vPointTypesLoc[i] == END_CONTOUR_SEGMENT ||
						vPointTypesLoc[i] == ISOLATED))
						continue;

					if (vPointTypesLoc[i] == ISOLATED && lineParamLoc > lineParamStart - minDist && lineParamLoc < lineParamEnd + minDist)
					{
						intersection.vCutPoints.push_back(vSortedPointsLoc[i]);
						if (vPointTypes[index] == START_CONTOUR_SEGMENT)
						{
							if (fabs(lineParamLoc - lineParamStart) < minDist ||
								fabs(lineParamLoc - lineParamEnd) < minDist)
							{
								intersection.vCutPointTypes.push_back(PUNKT_AUF_PUNKT);
								if (result < PUNKT_AUF_PUNKT) result = PUNKT_AUF_PUNKT;
							}
							else
							{
								intersection.vCutPointTypes.push_back(PUNKT_AUF_LINIE);
								if (result < PUNKT_AUF_LINIE) result = PUNKT_AUF_LINIE;
							}
						}
						else
						{
							if (fabs(lineParamLoc - lineParamStart) < minDist ||
								fabs(lineParamLoc - lineParamEnd) < minDist)
							{
								intersection.vCutPointTypes.push_back(PUNKT_AUF_LINIE);
								if (result < PUNKT_AUF_LINIE) result = PUNKT_AUF_LINIE;
							}
							else
							{
								intersection.vCutPointTypes.push_back(PUNKT_AUF_FLAECHE);
								if (result < PUNKT_AUF_FLAECHE) result = PUNKT_AUF_FLAECHE;
							}
						}
					}
					else
						if ((vPointTypesLoc[i] == START_CONTOUR_SEGMENT || vPointTypesLoc[i] == START_INNER_SEGMENT) &&
							lineParamLoc < lineParamEnd - minDist && vLineparamsLoc[i + 1] > lineParamStart + minDist)
						{
							double   c1 = __max(lineParamLoc, lineParamStart);
							double   c2 = __min(vLineparamsLoc[i + 1], lineParamEnd);
							GeoPoint p1 = cutLine.getPointFromParam(c1);
							GeoPoint p2 = cutLine.getPointFromParam(c2);

							LineSegment lineSegment = LineSegment(3, LineSegment::_LINE_STRING);
							lineSegment.addPoint3D(p1.getX(), p1.getY(), p1.getZ());
							lineSegment.addPoint3D(p2.getX(), p2.getY(), p2.getZ());
							intersection.vCutSegments.push_back(lineSegment);

							if (vPointTypes[index] == START_CONTOUR_SEGMENT)
							{
								if (vPointTypesLoc[i] == START_CONTOUR_SEGMENT)
								{
									intersection.vCutSegmentTypes.push_back(LINIE_AUF_LINIE);
									if (result < LINIE_AUF_LINIE) result = LINIE_AUF_LINIE;
								}
								else
								{
									intersection.vCutSegmentTypes.push_back(LINIE_AUF_FLAECHE);
									if (result < LINIE_AUF_FLAECHE)
										result = LINIE_AUF_FLAECHE;
								}
							}
							else
							{
								if (vPointTypesLoc[i] == START_CONTOUR_SEGMENT)
								{
									intersection.vCutSegmentTypes.push_back(LINIE_AUF_FLAECHE);
									if (result < LINIE_AUF_FLAECHE)
										result = LINIE_AUF_FLAECHE;
								}
								else
								{
									intersection.vCutSegmentTypes.push_back(FLAECHE_DURCH_FLAECHE);
									if (result < FLAECHE_DURCH_FLAECHE)
										result = FLAECHE_DURCH_FLAECHE;
								}
							}
						}
				}
				index++;
			}
			index++;
		}

	}

	catch ( Exception ^ )
	{
		return POLYGONE_DISJUNKT;

	}

  return result;
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertiert die Enum-Werte POINT_CLASSIFICATION in string                //
///////////////////////////////////////////////////////////////////////////////
string _Ring::convertPointTypeToString( _Ring::POINT_CLASSIFICATION type )
{
  switch ( type )
  {
  case ISOLATED:
    return "Einzelpunkt";

  case START_INNER_SEGMENT:
    return "Startpunkt eines Innensegmentes";

  case END_INNER_SEGMENT:
    return "Endpunkt eines Innensegments";

  case START_CONTOUR_SEGMENT:
    return "Startpunkt eines Randsegments";

  case  CONTOUR_SEGMENT:
    return "Innerer Punkt eines Randsegments";

  case END_CONTOUR_SEGMENT:
    return "Endpunkt eines Randsegments";
  }
  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob der Punkt p innerhalb, außerhalb, oder auf dem Rand des    //
//  Ringes liegt                                                             //                        
///////////////////////////////////////////////////////////////////////////////
POINT_LOCATION _Ring::checkInside2D ( GeoPoint p, double minDist, bool noCheck )
{
  double       point[2];
  unsigned int cutIndex = 0;

  point[0] = p.getX();
  point[1] = p.getY();
  return checkInside2D ( point, minDist, cutIndex, noCheck );
}

POINT_LOCATION _Ring::checkInside2D ( double point[], double minDist, unsigned int &cutIndex, bool noCheck )
{
  enum { OBEN, UNTEN };

  vector<double> vX;
  vector<double> vY;
  double         sw[3], no[3];
  double         h, px, h1;
  double         p0, p1;
  unsigned int   j, cutAnz; 
  bool           onBoundary = false;
  bool           vonUnten = false;
  bool		    	 vonOben = false;
  bool           anfangOnBoundary = false;
  bool			     anfangNachUnten = false;
  bool			     anfangNachOben = false;
  bool           check = false;
  POINT_LOCATION result, resultA;

  getBoundingBox ( sw, no );

  if ( point[0] < sw[0] - minDist || point[1] < sw[1] - minDist ||
    point[0] > no[0] + minDist || point[1] > no[1] + minDist )
    return OUTSIDE_POLYGON;

  cutAnz = 0;
  getPoints2D ( vX, vY );
  p0 = point[0];
  p1 = point[1];

  if ( vY[0] == point[1] && vX[0] >=  p0  )
    anfangOnBoundary = true;

  for ( j = 0; j < vX.size()-1; j++ )
  {
    if ( ( fabs ( point[1] - vY[j] ) <= minDist && fabs ( point[0] - vX[j] ) <= minDist ) ) 
    {
      cutIndex = j;
      return ON_VERTEX;
    }
  }

  for ( j = 0; j < vX.size() - 1; j++ )
  {
  	if ( vX[j] < point[0] && vX[j+1] < point[0] )
	  	continue;

    if (  ( vY[j] > point[1] && vY[j+1] > point[1] ) ||
          ( vY[j] < point[1] && vY[j+1] < point[1] ) ) 
    {
      continue;
    }
	
    if ( anfangOnBoundary && vY[j] == point[1] && vY[j+1] != point[1] && vX[j] >=  point[0] ) 
    { 
	    if ( anfangNachUnten == false && anfangNachOben == false )
	    {
	      if ( vY[j+1] < point[1] )
		      anfangNachUnten = true;
	      else
		      anfangNachOben = true;
	    }
    }
		
    if ( vY[j] == point[1] && vY[j+1] != point[1] && vX[j] >=  point[0] ) 
    {
      if ( vonUnten && vY[j+1] > point[1] )
        cutAnz++;

      if ( vonOben && vY[j+1] < point[1] )
        cutAnz++;

      vonUnten = false;
      vonOben = false;
    }
    else
    if ( vY[j] != point[1] && vY[j+1] == point[1] && vX[j+1] >=  point[0] ) 
	  {
	    if ( vY[j] > point[1] )
	    {
		    vonOben = true;
		    if ( j == vX.size() - 2 && anfangNachUnten )
				  cutAnz++;
	    }
	    else
	    {
		    vonUnten = true;
		    if ( j == vX.size() - 2 && anfangNachOben )
				  cutAnz++;
	    }
	  }
	  else
    if ( anfangOnBoundary && j == vX.size() - 2  && vY[j] == point[1] && vY[j+1] == point[1] ) 
	  {
			if ( vonOben && anfangNachUnten )
				cutAnz++;
			if ( vonUnten && anfangNachOben )
				cutAnz++;
	  }
	  else
	  {
		  if ( fabs (  vY[j+1] -  vY[j] ) < minDist ) 
		  {
		    if ( ( vX[j]   < point[0] && point[0] < vX[j+1] ) || 
			       ( vX[j+1] < point[0] && point[0] < vX[j]   ) ) 
        {
          cutIndex = j;
		      return ON_BOUNDARY;
        }
		  }
		  else
		  {
		  	h = ( point[1] - vY[j+1] ) / ( vY[j] - vY[j+1] );
			  px = h*vX[j] + (1.0-h)*vX[j+1];
			  h1 = point[0] - px;
			  if ( fabs (  h1 ) <= minDist ) 
        {
          cutIndex = j;
			    return ON_BOUNDARY;
        }
			  else
        {
          check = true;
          if ( h1 < 0 )
			      cutAnz++;
        }
		  }
	  }
  }

  if ( div ( cutAnz, 2).rem == 1 )
    result = IN_POLYGON;
  else	
    result = OUTSIDE_POLYGON;

  if ( check && noCheck == false )
  {
    set<_Geometrie*> sGeometries;
    vector<double> vXTest, vYTest;

    _Ring * pTestRing = copyRing();
    GeoPoint p = GeoPoint (  point[0], point[1] );
    Matrix4 m = Matrix4();
    m.createRotationXY( p, 0.17453 );  //  Rotation um 10 %

    pTestRing->offset( -p, sGeometries );
    pTestRing->getPoints2D ( vXTest, vYTest );


    pTestRing->transform( m );
    vXTest.clear();
    vYTest.clear();
    pTestRing->getPoints2D ( vXTest, vYTest );

    resultA = pTestRing->checkInside2D( p, minDist, true );
    if ( result != resultA )
    {
      m.createRotationXY( p, 1.047 );  //  Rotation um 60 %
      pTestRing->offset( -p, sGeometries );
      pTestRing->transform( m );

      resultA = pTestRing->checkInside2D( p, minDist, true );
      return resultA;
    }
    else
      return result;
  }
  
  return result;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft den Ring auf folgende Eigenschaften (Basic-Check):             //
//     - Doppelte Stützpunkte                                                //
//     - Identität erster und letzter Punkt                                  //
//     - Mindestens 4 Stützpunkte                                            //
///////////////////////////////////////////////////////////////////////////////
bool _Ring::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  GeometryError * pError;
  vector<double>  vX, vY, vZ;
	size_t          anz;
  double          dist;
  double          p1x, p1y, p1z, p2x, p2y, p2z;
  bool            fehlerSichern = true;
  bool            doublePointsExist;
  string          errString;

  doublePointsExist = checkDoublePoints( pCheckParams->minimalPointDistance, pCheckParams->eliminateDoublePoints );
  if ( doublePointsExist &&  !pCheckParams->eliminateDoublePoints )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::WARNING, GeometryError::RING_DOUBLE_POINTS );
    pError->setFeatureGeometryId ( gmlId );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  anz = getPoints3D( vX, vY, vZ );
  if ( anz < 4 )
  {
    pError = new GeometryError ( pObj, Error::ERROR, GeometryError::RING_NOT_ENOUGH_POINTS );
    if ( doublePointsExist )
      pError->setErrorDescription( "Identische Stützpunkte wurden eliminiert" );

    pError->setFeatureGeometryId( gmlId );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );
  
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  //  Feststellen ob der Ring geschlossen ist;
  p1x = vX[0];
  p1y = vY[0];
  p1z = vZ[0];
  p2x = vX[anz-1];
  p2y = vY[anz-1];
  p2z = vZ[anz-1];

  dist = sqrt ( ( p2x - p1x )*( p2x - p1x ) + ( p2y - p1y )*( p2y - p1y ) + ( p2z - p1z )*( p2z - p1z ) );
  if ( dist >= pCheckParams->minimalPointDistance )
  {
    pError = new GeometryError ( pObj, Error::ERROR, GeometryError::RING_NOT_CLOSED );
    pError->setFeatureGeometryId( gmlId );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );

    checkResult = BASIC_CHECK_FAILED;
    return false;
  } 

  checkResult = BASIC_CHECK_FULFILLED;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft den Ring auf folgende Eigenschaften (Advanced-Check):          //
//  - Planarität                                                             //
//  - Berührung oder Selbstüberschneidung von Kanten des Ringes              //
///////////////////////////////////////////////////////////////////////////////
bool _Ring::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
      vector<Error*> &vErrorReport, bool checkAnyway )
{
  vector<double>        vX, vY;
  double                x, y;
	size_t                pointAnz;
  GeometryError       * pError;
  bool                  success;
  double                flaeche;
  Direction             d;
  GEOMETRY_CHECK_RESULT planarityCheck;
  string                str;
  vector<CutResult>     vCutResults;
  vector<GeoPoint*>     vCutPoints;

  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;

  if ( dim == 3 )
  {
    double maximalError, avarageError; 

    success = checkPlanarity( pObj, featureProperty, pCheckParams, vErrorReport, maximalError, avarageError );
    if ( success )
      planarityCheck = PLANARITY_CHECK_FULFILLED;
    else
    {
      planarityCheck = PLANARITY_CHECK_FAILED;
    }

    if ( e == NULL )
    {
      checkResult = ADVANCED_CHECK_FAILED;
      return false;
    }

    checkResult = ADVANCED_CHECK_FULFILLED;

    _Ring * pPlanarRing = transformToHorizontal ();
    if ( pPlanarRing == NULL )
    {
      checkResult = ADVANCED_CHECK_FAILED;
      return false;
    }

    pointAnz = pPlanarRing->getPoints2D( vX, vY );
    delete pPlanarRing;
  }
  else
  {
    pointAnz = getPoints2D( vX, vY );
    planarityCheck = PLANARITY_CHECK_FULFILLED;
  }


  flaeche = getFlaeche ( vX, vY );
  if ( flaeche == 0.0 )
  {
    checkResult = ADVANCED_CHECK_FAILED;
    pError = new GeometryError ( pObj, Error::ERROR, GeometryError::RING_ZERO_AREA );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );
    return false;
  }

  if ( dim == 2 )
  {
    if ( flaeche > 0.0 )
      d = Direction ( 0.0, 0.0, 1.0 );
    else
      d = Direction ( 0.0, 0.0, -1.0 );

    x = 0.0;
    y = 0.0;

    for ( unsigned int i = 0; i < pointAnz-1; i++ )
    {
      x = x + vX[i];
      y = y + vY[i];
    }
    x = x / (pointAnz-1);
    y = y / (pointAnz-1);

    e = new Plane ( GeoPoint ( x, y, 0.0), d );
  }

 /* cutAnz = checkSelfOverlap( vX, vY, 0.5*pCheckParams->minimalPointDistance, pCheckParams->minimalAngleDistance, vCutResults );
  if ( cutAnz > 0 && ( planarityCheck == PLANARITY_CHECK_FULFILLED || checkAnyway )   )
  {
		size_t pointOnPointAnz = 0;
		size_t pointOnEdgeAnz = 0;
		size_t edgeOnEdgeAnz = 0;
		size_t edgeCutEdgeAnz = 0;
		size_t index1, index2;

    checkResult = ADVANCED_CHECK_FAILED;

    for ( size_t i = 0; i < vCutResults.size(); i++ )
    {
      GeoPoint * pCutPoint = new GeoPoint ( dim );
      *pCutPoint = getPoint( vCutResults[i].index1 );
      vCutPoints.push_back( pCutPoint );

      switch ( vCutResults[i].cutResult )
      {
      case POINT_ON_POINT: 
        pointOnPointAnz++;
        index1 = vCutResults[i].index1;
        index2 = vCutResults[i].index2;
        break;

      case POINT_ON_EDGE:
        pointOnEdgeAnz++;
        break;

      case OVERLAP:
        edgeOnEdgeAnz++;
        break;

      case TRUE_CUT:
        edgeCutEdgeAnz++;
        break;
      }
    }

    pError = NULL;
    if ( edgeOnEdgeAnz > 0 )
    {
      pError = new GeometryError ( pObj, Error::ERROR, GeometryError::RING_SELF_OVERLAP );
      if ( edgeOnEdgeAnz == 1 )
        pError->setErrorDescription( "Genau zwei Kanten einer Ringes überlappen sich" );
      else
        pError->setErrorDescription( "Mehrere Kanten einer Ringes überlappen sich" );

      pError->setPropertyName( featureProperty );
      vErrorReport.push_back( pError );
    }
    else
    if ( edgeCutEdgeAnz > 0 )
    {
      pError = new GeometryError ( pObj, Error::ERROR, GeometryError::RING_SELF_OVERLAP );
      if ( edgeCutEdgeAnz == 1 )
        pError->setErrorDescription( "Genau eine Kante eines Ringes schneidet eine andere Kante" );
      else
        pError->setErrorDescription( "Mehrere Kanten eines Ringes schneiden andere Kanten" );

      pError->setPropertyName( featureProperty );
      vErrorReport.push_back( pError );   
    }
    else
    if ( pointOnPointAnz > 0 )
    {
      pError = new GeometryError ( pObj, Error::ERROR, GeometryError::RING_SELF_OVERLAP );
      if ( pointOnPointAnz == 1 )
        pError->setErrorDescription( "Zwei Kanten eines Ringes berühren sich in genau einem Stützpunkt" );
      else
        pError->setErrorDescription( "Zwei Kanten eines Ringes berühren sich in mehreren Stützpunkten" );

      pError->setSelfOverlapIndeces( index1, index2 ); 
      pError->setPropertyName( featureProperty );
      vErrorReport.push_back( pError );
    }
    else
    if ( pointOnEdgeAnz > 0 )
    {
      pError = new GeometryError ( pObj, Error::ERROR, GeometryError::RING_SELF_OVERLAP );
      if ( pointOnEdgeAnz == 1 )
        pError->setErrorDescription( "Eine Kante eines Ringes berührt genau eine andere Kante ohne gemeinsamen Stützpunkt" );
      else
        pError->setErrorDescription( "Mehrere Kanten eines Ringes berühren andere Kanten ohne gemeinsame Stützpunkte" );

      pError->setPropertyName( featureProperty );
      vErrorReport.push_back( pError );
    }

    if ( pError != NULL )
    {
      MultiPoint * pMultiPoint = new MultiPoint ( dim );
      for ( unsigned int i = 0; i < vCutPoints.size(); i++ )
        pMultiPoint->addPoint( vCutPoints[i] );

      pError->addErrorGeometry( pMultiPoint, "Überlappungspunkte" );
    }
  }

  if ( cutAnz == 0  )
  {
    if ( planarityCheck == PLANARITY_CHECK_FULFILLED )
      checkResult = ADVANCED_CHECK_FULFILLED;
    else
      checkResult = PLANARITY_CHECK_FAILED;
  }
  else
  {
    if ( planarityCheck == PLANARITY_CHECK_FULFILLED )
      checkResult = PLANARITY_CHECK_FULFILLED;
    else
      checkResult = ADVANCED_CHECK_FAILED;
  }
  */

  if ( checkResult == ADVANCED_CHECK_FULFILLED && planarityCheck == PLANARITY_CHECK_FULFILLED )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Hilfsroutine beim Test der Selbst-Überschneidung eines Ringes            //
///////////////////////////////////////////////////////////////////////////////
size_t _Ring::checkSelfOverlap ( vector<double> &vX, vector<double> &vY, double minimalPointDistance, double minAngleDist, vector<CutResult> &vCutResults )
{
	size_t            i, j, pointAnz;
  double            p1x, p2x, q1x, q2x, p1y, p2y, q1y, q2y;
  CUT_RESULT        cutResult;
  CutResult         cutResultStruct;
  vector<GeoPoint>  vCutPoints;
  vector<GeoPoint> vCutPointsLoc;
  
  pointAnz = vX.size();

  for ( i = 0; i < pointAnz-1; i++ )
  {
    p1x = vX[i];
    p1y = vY[i];
    p2x = vX[i+1];
    p2y = vY[i+1];
    if ( sqrt (  (p1x - p2x)* (p1x - p2x) +  (p1y - p2y)* (p1y - p2y) ) < minimalPointDistance )
      continue;

    for ( j = i+1; j < pointAnz-1; j++ )
    {
      q1x = vX[j];
      q1y = vY[j];
      q2x = vX[j+1];
      q2y = vY[j+1];
      if ( sqrt (  (q1x - q2x)* (q1x - q2x) +  (q1y - q2y)* (q1y - q2y) ) < minimalPointDistance )
        continue;

      if ( q1x + minimalPointDistance < p1x && q2x + minimalPointDistance < p1x && q1x + minimalPointDistance < p2x && 
        q2x + minimalPointDistance < p2x )
        continue;

      if ( q1x - minimalPointDistance > p1x && q2x - minimalPointDistance > p1x && q1x - minimalPointDistance > p2x &&
        q2x - minimalPointDistance > p2x )
        continue;

      if ( q1y + minimalPointDistance < p1y && q2y + minimalPointDistance < p1y && q1y + minimalPointDistance < p2y &&
        q2y + minimalPointDistance < p2y )
        continue;

      if ( q1y - minimalPointDistance > p1y && q2y - minimalPointDistance > p1y && q1y - minimalPointDistance > p2y &&
        q2y - minimalPointDistance > p2y )
        continue;

      cutResult = cutPoints ( p1x, p1y, p2x, p2y, q1x, q1y, q2x, q2y, minimalPointDistance, minAngleDist, vCutPointsLoc );
      if ( cutResult != NO_CUT )
      {
        if (  j == i+1 || ( i == 0 && j == pointAnz-2 ) )
        {
          if ( cutResult ==  OVERLAP )
          {
            cutResultStruct.cutResult = OVERLAP;
            cutResultStruct.index1    = i;
            cutResultStruct.index2    = j;
            vCutResults.push_back( cutResultStruct );
          }
          vCutPointsLoc.clear();
        }
        else
        {
          bool exist = false;
          for ( unsigned int k = 0; k < vCutPoints.size(); k++ )
          {
            if ( vCutPoints[k].dist ( vCutPointsLoc[0] ) < minimalPointDistance )
            {
              exist = true;
              break;
            }
          }
          if ( !exist )
          {
            vCutPoints.push_back(  vCutPointsLoc[0] );
            cutResultStruct.cutResult = cutResult;
            cutResultStruct.index1    = i;
            cutResultStruct.index2    = j;
            vCutResults.push_back( cutResultStruct );
          }
          vCutPointsLoc.clear();
        }
      }
    }
  }
  return vCutResults.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Planaritätstest eines Ringes                                             //
///////////////////////////////////////////////////////////////////////////////
bool _Ring::checkPlanarity ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport, 
        double &maximalError, double &avarageError, bool checkAnyway  )
{
  double          dist;
  vector<double>  vX, vY, vZ;
	size_t          i, pointAnz;
  bool            success;
  GeoPoint        point;
  GeometryError * pError;

  maximalError = 0.0;
  avarageError = 0.0;

  success = calcPlane();
  if ( !success )
  {
    pError = new GeometryError ( pObj, Error::ERROR, GeometryError::RING_ZERO_AREA );
    vErrorReport.push_back( pError );
    return false;
  }

  getPoints3D( vX, vY, vZ );
  pointAnz = vX.size();
  for ( i = 0; i < vX.size()-1; i++ )
  {
    point = GeoPoint( vX[i], vY[i], vZ[i] );
    dist = e->getDistance( point );
    if ( dist > maximalError ) maximalError = dist;
    avarageError = avarageError + dist;
  }
  avarageError = avarageError / (vX.size() - 1);

  if ( maximalError > pCheckParams->planarityLimit )
  {
    pError = new GeometryError ( pObj, Error::WARNING, GeometryError::POLYGON_NON_PLANAR );
    pError->setPlanaritaetsfehler( maximalError, avarageError );
    vErrorReport.push_back( pError );
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die gemeinsame Fläche zweier Ringe                              //
///////////////////////////////////////////////////////////////////////////////
bool _Ring::cut2D ( _Ring * pRing, double minDist,IntersectionGeometry & intersection )
{             
  bool               success;
  TopolRing        * pTopolRing;
  TopolRing        * pCutTopolRing;
  vector<TopolRing*> vTopolRings;
	size_t             i, anz;
//  String           ^ STR;
  string             str;

  pTopolRing = new TopolRing();
  pTopolRing->setMinDist( minDist );
  success = pTopolRing->create( this );
  if ( !success )
  {
    delete pTopolRing;
    return false;
  }

  pCutTopolRing = new TopolRing();
  pCutTopolRing->setMinDist( minDist );
  success = pCutTopolRing->create( pRing );
  if ( !success )
  {
    delete pTopolRing;
    delete pCutTopolRing;
    return false;
  }

  pTopolRing->merge( pCutTopolRing );
  anz = pTopolRing->cut( pCutTopolRing, vTopolRings );

  delete pTopolRing;
  delete pCutTopolRing;

  for ( i = 0; i < anz; i++ )
  {
    int crAnz = vTopolRings[i]->createLinearRings( intersection.vCutRings );
    delete vTopolRings[i];
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt Berührungspunkte und -Linien zweier Ringe                       //
///////////////////////////////////////////////////////////////////////////////
bool _Ring::touch2D ( _Ring * pRing, double minDist,IntersectionGeometry & intersection )
{             
  bool                   success;
  TopolRing            * pTopolRing;
  TopolRing            * pCutTopolRing;
  vector<TopolHalfEdge*> vCutSegments;
  vector<TopolPoint*>    vCutPoints;
	size_t                 i, anz;

  pTopolRing = new TopolRing();
  success = pTopolRing->create( this );
  if ( !success )
  {
    delete pTopolRing;
    return false;
  }

  pCutTopolRing = new TopolRing();
  success = pCutTopolRing->create( pRing );
  if ( !success )
  {
    delete pTopolRing;
    delete pCutTopolRing;
    return false;
  }

  pTopolRing->merge( pCutTopolRing );

  anz = pTopolRing->getDoubleEdges( vCutSegments );
  for ( i = 0; i < anz; i++ )
  {
    TopolHalfEdge * pEdge = vCutSegments[i];
    LineSegment     segment = LineSegment ( 3, LineSegment::_LINE_STRING );
    GeoPoint        point = pEdge->getStartPoint()->getPoint();
    segment.addPoint3D( point.getX(), point.getY(), point.getZ() );

    point = pEdge->getEndPoint()->getPoint();
    segment.addPoint3D( point.getX(), point.getY(), point.getZ() );

    intersection.vCutSegments.push_back( segment );
    intersection.vCutSegmentTypes.push_back( LINIE_AUF_LINIE );
  }

  anz = pTopolRing->getCrossPoints( vCutPoints );
  for ( i = 0; i < anz; i++ )
  {
    GeoPoint point = vCutPoints[i]->getPoint();
    intersection.vCutPoints.push_back( point );
    POINT_LOCATION loc1 = checkInside2D( point, minDist, true );
    POINT_LOCATION loc2 = pRing->checkInside2D( point, minDist, true );
    if ( loc1 == ON_VERTEX && loc2 == ON_VERTEX )
      intersection.vCutPointTypes.push_back( PUNKT_AUF_PUNKT );
    else
      intersection.vCutPointTypes.push_back( PUNKT_AUF_LINIE );
  }

  delete pTopolRing;
  delete pCutTopolRing;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Mengen-Differenz des Self-Ringes mit den _Ring pRing        //
//  Es wird vorausgesetzt, dass die beiden Ringe überlappen                  //
///////////////////////////////////////////////////////////////////////////////
bool  _Ring::difference2D ( _Ring * pRing, double minDist, std::vector<LinearRing*> &vDifferenceRings )
{
  bool               success;
  TopolRing        * pTopolRing;
  TopolRing        * pDiffTopolRing;
  vector<TopolRing*> vTopolRings;
	size_t             i, anz;

  pTopolRing = new TopolRing();
  success = pTopolRing->create( this );
  if ( !success )
  {
    delete pTopolRing;
    return false;
  }

  pRing->reverse();
  pDiffTopolRing = new TopolRing();
  success = pDiffTopolRing->create( pRing );
  if ( !success )
  {
    delete pTopolRing;
    delete pDiffTopolRing;
    return false;
  }

  pTopolRing->merge( pDiffTopolRing );
  anz = pTopolRing->difference( pDiffTopolRing, vTopolRings );

  delete pTopolRing;
  delete pDiffTopolRing;

  for ( i = 0; i < anz; i++ )
  {
    vTopolRings[i]->createLinearRings( vDifferenceRings );
    delete vTopolRings[i];
  }
  pRing->reverse();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Fläche der Projektion des Ringes auf die (X,Y)-Ebene       //
///////////////////////////////////////////////////////////////////////////////
double _Ring::getFlaeche( vector<double> vX, vector<double> vY )
{
	double  xA, yA, xE, yE;
  double  flaeche;
	size_t  i, anz;

	anz = vX.size();
	flaeche = 0.0;

	for ( i = 0; i < anz-1; i++ )
	{
		xA = vX[i];
		xE = vX[i+1];
		yA = vY[i];
		yE = vY[i+1];

		flaeche = flaeche + 0.5*(xA - xE)*(yA + yE);
	}
  return flaeche;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Fläche des Ringes                                          //
///////////////////////////////////////////////////////////////////////////////
double _Ring::getFlaeche()
{
  if ( dim == 2 )
    return getFlaeche2D();

  _Ring * kopie = transformToHorizontal();
  if ( kopie == NULL )
    return 0.0;

  double flaeche = kopie->getFlaeche2D();
  delete kopie;

  return flaeche;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Fläche eines 2D Ringes                                     //
///////////////////////////////////////////////////////////////////////////////
double _Ring::getFlaeche2D()
{
  vector<double> vX, vY;
  double         xA, yA, xE, yE;
  double         flaeche;
	size_t         i, anz;

  anz = getPoints2D ( vX, vY );

  flaeche = 0.0;

  for ( i = 0; i < anz-1; i++ )
  {
    xA = vX[i];
    xE = vX[i+1];
    yA = vY[i];
    yE = vY[i+1];

    flaeche = flaeche + 0.5*(xA - xE)*(yA + yE);
  }

  return flaeche;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Ebene der Ring-Stützpunkte                                   //
///////////////////////////////////////////////////////////////////////////////
Plane *  _Ring::getPlane()
{
  bool success;

  if ( e == NULL )
    success = calcPlane();
  else
    success = true;

   if ( success )
     return e;
   else
     return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Ebene der Rind-Stützpunkte                                 //
///////////////////////////////////////////////////////////////////////////////
 bool _Ring::calcPlane()
 {
   double         aXY, aXZ, aYZ, length;
   double         x, y, z;
	 size_t         i, anz;
   vector<double> vX, vY, vZ;
   
  anz = getPoints3D ( vX, vY, vZ );

  aXY = getFlaeche( vX, vY );
  aXZ = getFlaeche( vX, vZ );
  aYZ = getFlaeche( vY, vZ );

  length = sqrt ( aXY*aXY + aXZ*aXZ + aYZ*aYZ );
  if ( length == 0.0 )
    return false;

   Direction d = Direction ( aYZ/length, -aXZ/length, aXY/length );

   x = 0.0;
   y = 0.0;
   z = 0.0;  

   for ( i = 0; i < anz-1; i++ )
   {
     x = x + vX[i];
     y = y + vY[i];
     z = z + vZ[i];
   }
   x = x / (anz-1);
   y = y / (anz-1);
   z = z / (anz-1);

   if ( e != NULL ) delete e;
   e = new Plane ( GeoPoint ( x, y, z), d );

   return true;
 }

/////////////////////////////////////////////////////////////////////////////////
//  Eliminiert doppelte Punkte aus einem projizierten Ring                     //
 /////////////////////////////////////////////////////////////////////////////////
void _Ring::eliminateDoublePoints (  std::vector<double> &vX, std::vector<double> &vY, double minDist )
{
	size_t       i, j, anz;
  GeoPoint     p1(2), p2(2);
  double       dist;

  anz = vX.size();
  p1 = GeoPoint ( vX[0], vY[0] );

  for ( i = 0; i < anz-1; i++ )
  {
    p2 = GeoPoint ( vX[i+1], vY[i+1] );
    dist = p1.dist ( p2 );

    if ( dist < minDist )
    {
      for ( j = i + 1; j < anz; j++  )
      {
        vX[j-1] = vX[j];
        vY[j-1] = vY[j];
      }
      vX.pop_back();
      vY.pop_back();
      anz = anz-1;
      i = i - 1;     
    }

    p1 = p2;
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Transformiert den Ring in einen Linienzug                                //
///////////////////////////////////////////////////////////////////////////////
LineSegment * _Ring::transformToCurve()
{
  string         id;
  vector<double> vX, vY, vZ;

  LineSegment * pLineSegment = new LineSegment ( dim, LineSegment::_LINE_STRING );
  id = gmlId + "_curve";
  pLineSegment->setGmlId( id );

  if ( dim == 2 )
  {
    getPoints2D( vX, vY );
    pLineSegment->addPoints2D( vX, vY );
  }
  else
  {
    getPoints3D( vX, vY, vZ );
    pLineSegment->addPoints3D( vX, vY, vZ );
  }

  return pLineSegment;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Schnittpunkte eines Ringes mit einer Linie, die in der      //
//  Ringebene liegt. Die Schnittpunkte werden wie folgt charakterisiert:     //
//  ISOLATED - Ein isolierter Ring-Stützpunkt auf der Linie                  //
//  START_INNER_SEGMENT - Startpunkt eines Segmentes im Ring-Inneren         //
//  END_INNER_SEGMANT - Endpunkt eines Segmentes im Ring-Inneren             //
//  START_CONTOUR_SEGMENT - Startpunkt eines Segmentes auf der Außenkontur   //
//  CONTOUR_SEGMENT - Innerer Punkt eines Segmentes auf der Außenkontur      //
//  END_CONTOUR_SEGMENT - Endpunkt eines Segmentes auf der Außenkontur       //
///////////////////////////////////////////////////////////////////////////////
size_t _Ring::getCutPoints ( Line & line, vector<GeoPoint> &vSortedPoints, vector<double> &vLineParams, std::vector<int> &vPointTypes, double minDist )
{
  enum POINT_LOC {  LEFT_FROM_LINE, RIGHT_FROM_LINE, ON_LINE, UNKNOWN };
  POINT_LOC            pointLocLastPoint, pointLocActPoint;
	size_t               i, anz;
  vector<GeoPoint>     vPoints;
  vector<GeoPoint>     vCutPointsLoc;
  GeoPoint             lastPoint;
  double               lastDist;
  double               lineParam;
  Direction            normal        = getPlane()->getNormal();
  GeoPoint             lineOrigin    = line.getOrigin();
  Direction            lineDirection = line.getDirection();
  Matrix4              mat = e->getTransformationToHorizontal();
  GeoPoint             center = e->getCenter();
  
  map<double,GeoPoint>            mSortedPoints;
  map<double,GeoPoint>::iterator iter;

	try
	{
		anz = getPoints(vPoints);
		pointLocLastPoint = UNKNOWN;
		lastDist = 0.0;

		//  Schnittpunkte bestimmen
		for (i = 0; i < anz; i++)
		{
			GeoPoint actPoint = vPoints[i];

			Direction h = Direction(lineOrigin, actPoint);
			Direction h1 = lineDirection.crossProduct(h);
			double    actDist = h1 * normal;

			if (actDist < -minDist)
				pointLocActPoint = LEFT_FROM_LINE;
			else
				if (actDist > minDist)
					pointLocActPoint = RIGHT_FROM_LINE;
				else
				{
					pointLocActPoint = ON_LINE;
					if (i < anz - 1)
						vCutPointsLoc.push_back(actPoint);
				}

			if (pointLocActPoint == LEFT_FROM_LINE && pointLocLastPoint == RIGHT_FROM_LINE ||
				pointLocActPoint == RIGHT_FROM_LINE && pointLocLastPoint == LEFT_FROM_LINE)
			{
				double h = fabs(lastDist) / (fabs(lastDist) + fabs(actDist));
				GeoPoint cutPoint = actPoint * h + lastPoint * (1.0 - h);
				vCutPointsLoc.push_back(cutPoint);
			}

			lastPoint = actPoint;
			lastDist = actDist;
			pointLocLastPoint = pointLocActPoint;
		}

		if (vCutPointsLoc.size() == 0)
			return 0;

		if (vCutPointsLoc.size() == 1)
		{
			line.sortPointsOnLine(vCutPointsLoc, mSortedPoints);
			iter = mSortedPoints.begin();
			vPointTypes.push_back(ISOLATED);
			vLineParams.push_back(iter->first);
			vSortedPoints.push_back(iter->second);
			return 1;
		}

		//  Schnittpunkte entlang der Linie sortieren
		line.sortPointsOnLine(vCutPointsLoc, mSortedPoints);

		// Ring in die Ebene projizieren, damit Tests "Punkt in Polygon" gemacht werden können
		_Ring * pHorizontalRing = transformToHorizontal();

		vector<GeoPoint> vPointsTemp;
		pHorizontalRing->getPoints(vPointsTemp);

		iter = mSortedPoints.begin();
		GeoPoint startPoint = iter->second;
		vLineParams.push_back(iter->first);
		vSortedPoints.push_back(startPoint);

		iter++;
		if ( iter == mSortedPoints.end() )
			return 0;

		GeoPoint endPoint = iter->second;

		GeoPoint middlePoint = (startPoint + endPoint)*0.5;
		middlePoint.transform(mat);
		POINT_LOCATION pointLoc = pHorizontalRing->checkInside2D(middlePoint, minDist, true);
		POINT_CLASSIFICATION    pointClass;

		switch (pointLoc)
		{
		case IN_POLYGON:
			pointClass = START_INNER_SEGMENT;
			break;

		case OUTSIDE_POLYGON:
			pointClass = ISOLATED;
			break;

		case ON_BOUNDARY:
		case ON_VERTEX:
			pointClass = START_CONTOUR_SEGMENT;
		}
		vPointTypes.push_back(pointClass);

		startPoint = endPoint;
		lineParam = iter->first;
		iter++;

		while (iter != mSortedPoints.end())
		{
			endPoint = iter->second;

			middlePoint = (startPoint + endPoint)*0.5;
			middlePoint.transform(mat);
			POINT_LOCATION pointLoc = pHorizontalRing->checkInside2D(middlePoint, minDist, true);

			switch (pointLoc)
			{
			case IN_POLYGON:
				pointClass = START_INNER_SEGMENT;
				break;

			case ON_BOUNDARY:
			case ON_VERTEX:
				if (pointClass == START_CONTOUR_SEGMENT || pointClass == CONTOUR_SEGMENT)
					pointClass = CONTOUR_SEGMENT;
				else
					pointClass = START_CONTOUR_SEGMENT;
				break;

			case OUTSIDE_POLYGON:
				if (pointClass == START_CONTOUR_SEGMENT || pointClass == CONTOUR_SEGMENT)
					pointClass = END_CONTOUR_SEGMENT;
				else
					if (pointClass == START_INNER_SEGMENT)
						pointClass = END_INNER_SEGMENT;
					else
						pointClass = ISOLATED;
				break;
			}

			if (pointClass != CONTOUR_SEGMENT)
			{
				vLineParams.push_back(lineParam);
				vSortedPoints.push_back(startPoint);
				vPointTypes.push_back(pointClass);
			}

			startPoint = endPoint;
			lineParam = iter->first;
			iter++;
		}

		delete pHorizontalRing;

		if (pointClass == START_CONTOUR_SEGMENT || pointClass == CONTOUR_SEGMENT)
			pointClass = END_CONTOUR_SEGMENT;
		else
			if (pointClass == START_INNER_SEGMENT)
				pointClass = END_INNER_SEGMENT;
			else
				pointClass = ISOLATED;
		vPointTypes.push_back(pointClass);

		vLineParams.push_back(lineParam);
		vSortedPoints.push_back(endPoint);

		return mSortedPoints.size();

	}

	catch ( Exception ^ )
	{
		return 0;
	}

}

///////////////////////////////////////////////////////////////////////////////
//  RingIntersectionGeometry                                                 //
//  Geometrische Informationen zur Berührung, Überlappung oder               //
//  Durchdringung zweier Ringe                                               //
///////////////////////////////////////////////////////////////////////////////
IntersectionGeometry::IntersectionGeometry()
{
}

IntersectionGeometry::~IntersectionGeometry()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Koordinaten-Transformation der Geometrie                                 //
///////////////////////////////////////////////////////////////////////////////
void IntersectionGeometry::transform ( Matrix4 & mat )
{
  unsigned int i;

  for ( i = 0; i < vCutPoints.size(); i++ )
    vCutPoints[i].transform( mat );

  for ( i = 0; i < vCutSegments.size(); i++ )
    vCutSegments[i].transform( mat );

  for ( i = 0; i < vCutRings.size(); i++ )
    vCutRings[i]->transform( mat );
}

///////////////////////////////////////////////////////////////////////////////
//  Inverse Koordinaten-Transformation der Geometrie                         //
///////////////////////////////////////////////////////////////////////////////
void IntersectionGeometry::transformInvers ( Matrix4 & mat )
{
  unsigned int i;

  for ( i = 0; i < vCutPoints.size(); i++ )
    vCutPoints[i].transformInvers( mat );

  for ( i = 0; i < vCutSegments.size(); i++ )
    vCutSegments[i].transformInvers( mat );

  for ( i = 0; i < vCutRings.size(); i++ )
    vCutRings[i]->transformInvers( mat );
}

///////////////////////////////////////////////////////////////////////////////
//  Kopie des Geometrie-Objektes                                             //
///////////////////////////////////////////////////////////////////////////////
void IntersectionGeometry::copy ( IntersectionGeometry & intersection )
{
  unsigned int i;

  for ( i = 0; i < intersection.vCutPoints.size(); i++ )
    vCutPoints.push_back( intersection.vCutPoints[i] );

  for ( i = 0; i < intersection.vCutPointTypes.size(); i++ )
    vCutPointTypes.push_back( intersection.vCutPointTypes[i] );

  for ( i = 0; i < intersection.vCutSegments.size(); i++ )
    vCutSegments.push_back( intersection.vCutSegments[i] );

  for ( i = 0; i < intersection.vCutSegmentTypes.size(); i++ )
    vCutSegmentTypes.push_back( intersection.vCutSegmentTypes[i] );

  for ( i = 0; i < intersection.vCutRings.size(); i++ )
    vCutRings.push_back( intersection.vCutRings[i] );
}

///////////////////////////////////////////////////////////////////////////////
//  Zurücksetzen der Geometrie                                               //
///////////////////////////////////////////////////////////////////////////////
void IntersectionGeometry::clear()
{
  vCutPoints.clear();
  vCutPointTypes.clear();
  vCutSegments.clear();
  vCutSegmentTypes.clear();
  vCutRings.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  MultiGeometry                                                            //
//  Klasse für gml:MultiGeometry                                             //
///////////////////////////////////////////////////////////////////////////////
MultiGeometry::MultiGeometry ( int dim ) : _Geometrie( dim )
{

}

MultiGeometry::MultiGeometry ( MultiGeometry * pMultiGeometryOld ) : _Geometrie( pMultiGeometryOld )
{
  unsigned int i;

  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = pMultiGeometryOld->getGeometry( i );
    _Geometrie * pGeoCopy = Geometrie::copy( pGeo );
    position.push_back( pGeoCopy );
  }
}

MultiGeometry::~MultiGeometry()
{
  unsigned int i;

  for ( i = 0; i < position.size(); i++ )
    delete position[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding Box										                      	     //
///////////////////////////////////////////////////////////////////////////////
bool MultiGeometry::getBoundingBox ( double * sw, double * no )
{
  bool	success;
  double  swLoc[3], noLoc[3];

  if ( bbExist )
  {
    sw[0] = m_sw[0];
    sw[1] = m_sw[1];
    no[0] = m_no[0];
    no[1] = m_no[1];
    sw[2] = m_sw[2];
    no[2] = m_no[2];
    return true;
  }

  sw[0] =  1.0E20;
  sw[1] =  1.0E20;
  no[0] = -1.0E20;
  no[1] = -1.0E20;
  if ( dim == 3 )
  {
    sw[2] =  1.0E20;
    no[2] = -1.0E20;
  }
  else
  {
    sw[2] = 0.0;
    no[2] = 0.0;
  }

  if ( position.size() == 0 ) 
    return false;

  for ( unsigned int i = 0; i < position.size(); i++ )
  { 
    _Geometrie * pGeo = position[i];
    success = pGeo->getBoundingBox ( swLoc, noLoc );
    if ( !success ) continue;

    if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
    if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
    if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
    if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( dim == 3 )
    {
      if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
      if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
    }
  }

  m_sw[0] = sw[0];
  m_sw[1] = sw[1];
  m_no[0] = no[0];
  m_no[1] = no[1];
  if ( dim == 3 )
  {
    m_sw[2] = sw[2];
    m_no[2] = no[2];
  }
  else
  {
    m_sw[2] = 0.0;
    m_no[2] = 0.0;
  }

  bbExist = true;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den  2D-Schwerpunkt aller Member der Multi-Geometry      			 //
///////////////////////////////////////////////////////////////////////////////
bool MultiGeometry::getCenter2D ( double &xP, double &yP )
{
  double       xLoc, yLoc;
  unsigned int i;
  bool         success;

  if ( position.size() == 0 ) return false;

  xP = 0.0;
  yP = 0.0;

  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    success = pGeo->getCenter2D ( xLoc, yLoc );
    if ( !success ) return false;

    xP = xP + xLoc;
    yP = yP + yLoc;
  }

  xP = xP / position.size();
  yP = yP / position.size();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den  3D-Schwerpunkt aller Member der Multi-Geometry      			 //
///////////////////////////////////////////////////////////////////////////////
bool MultiGeometry::getCenter3D ( double &xP, double &yP, double &zP )
{
  double       xLoc, yLoc, zLoc;
  unsigned int i;
  bool         success;

  success = getCenter2D( xP, yP );
  if ( !success ) return false;

  if ( dim == 2 )
    zP = 0.0;
  else
  {
    zP = 0.0;
    for ( i = 0; i < position.size(); i++ )
    {
      _Geometrie * pGeo = position[i];
      success = pGeo->getCenter3D ( xLoc, yLoc, zLoc );
      if ( !success ) return false;

      zP = zP + zLoc;
    }

    zP = zP / position.size();
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Geometry-Member an position index                            //
///////////////////////////////////////////////////////////////////////////////
_Geometrie * MultiGeometry::getGeometry ( size_t index )
{
  if ( index < 0 || index >= position.size() )
    return NULL;
  else
    return position[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone                                          //
///////////////////////////////////////////////////////////////////////////////
size_t MultiGeometry::getPolygonAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getPolygonAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone                                                     //
///////////////////////////////////////////////////////////////////////////////
size_t MultiGeometry::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getPolygone( vPolygone );
  }
  return anz;
}

size_t MultiGeometry::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations ) 
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getPolygone( vPolygone, vOrientations );
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t MultiGeometry::getCurveSegmentAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getCurveSegmentAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t MultiGeometry::getPointAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getPointAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (Basic-Check)                                    //
///////////////////////////////////////////////////////////////////////////////
bool MultiGeometry::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  unsigned int i, fehlerAnz;
  bool     success;

  if ( position.size() == 0 )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    vErrorReport.push_back( pError );
    pError->setPropertyName( featureProperty );

    pError->setFeatureGeometryId ( gmlId );
    return false;
  }

  fehlerAnz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    success =  pGeo->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );
    if ( !success )
      fehlerAnz++;
  }

  if ( fehlerAnz > 0 )
  {
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }
  else
  {
    checkResult = BASIC_CHECK_FULFILLED;
    return true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (Advanced-Check)                                 //
///////////////////////////////////////////////////////////////////////////////
bool MultiGeometry::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  unsigned int i, fehlerAnz;
  bool     success;

  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;

  fehlerAnz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    success =  pGeo->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
    if ( !success )
      fehlerAnz++;
  }

  if ( fehlerAnz > 0 )
  {
    checkResult = ADVANCED_CHECK_FAILED;
    return false;
  }
  else
  {
    checkResult = ADVANCED_CHECK_FULFILLED;
    return true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Referenzen                                                 //
///////////////////////////////////////////////////////////////////////////////
bool MultiGeometry::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  unsigned int i;
  bool success, successAll;

  successAll = true;

  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    success = pGeo->checkReferences ( pObj, propertyName, vErrorReport );
    if ( !success ) successAll = false;
  }

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der Geometrie ein gültiges                   //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool MultiGeometry::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    if ( position[i]->existReferenceSystem( pSrsManager ) == false )
      return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Geometrischen Offset auf die einzelnen Member                            //
///////////////////////////////////////////////////////////////////////////////
void MultiGeometry::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  )
{
  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    if ( sGeometries.count( pGeo ) == 0 )
    {
      pGeo->offset( point, sGeometries );
    }
  }

  sGeometries.insert ( this );
}

///////////////////////////////////////////////////////////////////////////////
//  GeometricComplex                                                         //
//  Klasse für gml:GeometricComplex                                          //
///////////////////////////////////////////////////////////////////////////////
GeometricComplex::GeometricComplex ( int dim ) : _Geometrie( dim )
{

}

GeometricComplex::GeometricComplex ( GeometricComplex * pMultiGeometryOld ) : _Geometrie( pMultiGeometryOld )
{
  unsigned int i;

  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = pMultiGeometryOld->getGeometry( i );
    _Geometrie * pGeoCopy = Geometrie::copy( pGeo );
    position.push_back( pGeoCopy );
  }
}

GeometricComplex::~GeometricComplex()
{
  unsigned int i;

  for ( i = 0; i < position.size(); i++ )
    delete position[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding Box										                      	     //
///////////////////////////////////////////////////////////////////////////////
bool GeometricComplex::getBoundingBox ( double * sw, double * no )
{
  bool	success;
  double  swLoc[3], noLoc[3];

  if ( bbExist )
  {
    sw[0] = m_sw[0];
    sw[1] = m_sw[1];
    no[0] = m_no[0];
    no[1] = m_no[1];
    sw[2] = m_sw[2];
    no[2] = m_no[2];
    return true;
  }

  sw[0] =  1.0E20;
  sw[1] =  1.0E20;
  no[0] = -1.0E20;
  no[1] = -1.0E20;
  if ( dim == 3 )
  {
    sw[2] =  1.0E20;
    no[2] = -1.0E20;
  }
  else
  {
    sw[2] = 0.0;
    no[2] = 0.0;
  }

  if ( position.size() == 0 ) return false;

  for ( unsigned int i = 0; i < position.size(); i++ )
  { 
    _Geometrie * pGeo = position[i];
    success = pGeo->getBoundingBox ( swLoc, noLoc );
    if ( !success ) continue;

    if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
    if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
    if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
    if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( dim == 3 )
    {
      if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
      if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
    }
  }

  m_sw[0] = sw[0];
  m_sw[1] = sw[1];
  m_no[0] = no[0];
  m_no[1] = no[1];
  if ( dim == 3 )
  {
    m_sw[2] = sw[2];
    m_no[2] = no[2];
  }
  else
  {
    m_sw[2] = 0.0;
    m_no[2] = 0.0;
  }

  bbExist = true;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den 2D-Schwerpunkt aller Member der Multi-Geometry       			 //
///////////////////////////////////////////////////////////////////////////////
bool GeometricComplex::getCenter2D ( double &xP, double &yP )
{
  double       xLoc, yLoc;
  unsigned int i;
  bool         success;

  if ( position.size() == 0 ) return false;

  xP = 0.0;
  yP = 0.0;

  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    success = pGeo->getCenter2D ( xLoc, yLoc );
    if ( !success ) return false;

    xP = xP + xLoc;
    yP = yP + yLoc;
  }

  xP = xP / position.size();
  yP = yP / position.size();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den 3D-Schwerpunkt aller Member der Multi-Geometry       			 //
///////////////////////////////////////////////////////////////////////////////
bool GeometricComplex::getCenter3D ( double &xP, double &yP, double &zP )
{
  double       xLoc, yLoc, zLoc;
  unsigned int i;
  bool         success;

  success = getCenter2D( xP, yP );
  if ( !success ) return false;

  if ( dim == 2 )
    zP = 0.0;
  else
  {
    zP = 0.0;
    for ( i = 0; i < position.size(); i++ )
    {
      _Geometrie * pGeo = position[i];
      success = pGeo->getCenter3D ( xLoc, yLoc, zLoc );
      if ( !success ) return false;

      zP = zP + zLoc;
    }

    zP = zP / position.size();
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone                                          //
///////////////////////////////////////////////////////////////////////////////
size_t GeometricComplex::getPolygonAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getPolygonAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone                                                     //
///////////////////////////////////////////////////////////////////////////////
size_t GeometricComplex::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getPolygone( vPolygone );
  }
  return anz;
}

size_t GeometricComplex::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getPolygone( vPolygone, vOrientations );
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t GeometricComplex::getCurveSegmentAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getCurveSegmentAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t GeometricComplex::getPointAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getPointAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Geometry-Member an position index                            //
///////////////////////////////////////////////////////////////////////////////
_Geometrie * GeometricComplex::getGeometry ( size_t index )
{
  if ( index < 0 || index >= position.size() )
    return NULL;
  else
    return position[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (Basic-Check)                                    //
///////////////////////////////////////////////////////////////////////////////
bool GeometricComplex::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  unsigned int i, fehlerAnz;
  bool     success;

  if ( position.size() == 0 )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );
    pError->setFeatureGeometryId ( gmlId );
    return false;
  }

  fehlerAnz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    success =  pGeo->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );
    if ( !success )
      fehlerAnz++;
  }

  if ( fehlerAnz > 0 )
  {
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }
  else
  {
    checkResult = BASIC_CHECK_FULFILLED;
    return true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (Advanced-Check)                                 //
///////////////////////////////////////////////////////////////////////////////
bool GeometricComplex::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  unsigned int i, fehlerAnz;
  bool     success;

  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;

  fehlerAnz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    success =  pGeo->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
    if ( !success )
      fehlerAnz++;
  }

  if ( fehlerAnz > 0 )
  {
    checkResult = ADVANCED_CHECK_FAILED;
    return false;
  }
  else
  {
    checkResult = ADVANCED_CHECK_FULFILLED;
    return true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Referenzen                                                 //
///////////////////////////////////////////////////////////////////////////////
bool GeometricComplex::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  unsigned int i;
  bool success, successAll;

  successAll = true;

  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    success = pGeo->checkReferences ( pObj, propertyName, vErrorReport );
    if ( !success ) successAll = false;
  }

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der Geometrie ein gültiges                   //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool GeometricComplex::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    if ( position[i]->existReferenceSystem( pSrsManager ) == false )
      return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Geometrischen Offset auf die einzelnen Member                            //
///////////////////////////////////////////////////////////////////////////////
void GeometricComplex::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  )
{
  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    if ( sGeometries.count( pGeo ) == 0 )
    {
      pGeo->offset( point, sGeometries );
    }
  }
  sGeometries.insert ( this );
}


///////////////////////////////////////////////////////////////////////////////
//  MultiSurface						                              									 //
//  Klasse für eine gml:MultiSurface oder gml:CompositeSurface	      			 //
///////////////////////////////////////////////////////////////////////////////
MultiSurface::MultiSurface( int dimP ): _Surface( dimP )
{

}

MultiSurface::MultiSurface ( MultiSurface * pMultiSurfaceOld, Features * pFeatures, bool generateMemberLinks ): _Surface ( pMultiSurfaceOld )
{
  for ( unsigned int i = 0; i < pMultiSurfaceOld->position.size(); i++ )
  {
    _Surface * pSurface = pMultiSurfaceOld->position[i];
    _Surface * pSurfaceCopy;

	  switch ( pSurface->getGeometryType() )
	  {
      case MULTI_SURFACE:
        {
          if ( generateMemberLinks && pFeatures != NULL )
          {
            pSurfaceCopy = new SurfaceReference ( pFeatures );
            ((SurfaceReference*)pSurfaceCopy)->setRefGeoId( pSurface->getGmlId() );
          }
          else
            pSurfaceCopy = new MultiSurface ( (MultiSurface*)pSurface );

          position.push_back ( pSurfaceCopy );
          break;
        }

		  case SURFACE:
			  {
          if ( generateMemberLinks && pFeatures != NULL )
          {
            pSurfaceCopy = new SurfaceReference ( pFeatures );
            ((SurfaceReference*)pSurfaceCopy)->setRefGeoId( pSurface->getGmlId() );
          }
          else
            pSurfaceCopy = new Surface ( (Surface*)pSurface );

          position.push_back ( pSurfaceCopy );
          break;
			  }

      case TRIANGULATED_SURFACE:
        {
          if ( generateMemberLinks && pFeatures != NULL )
          {
            pSurfaceCopy = new SurfaceReference ( pFeatures );
            ((SurfaceReference*)pSurfaceCopy)->setRefGeoId( pSurface->getGmlId() );
          }
          else
            pSurfaceCopy = new TriangulatedSurface ( (TriangulatedSurface*)pSurface );

          position.push_back ( pSurfaceCopy );
          break;
        }

      case COMPOSITE_SURFACE:
        {
          if ( generateMemberLinks && pFeatures != NULL )
          {
            pSurfaceCopy = new SurfaceReference ( pFeatures );
            ((SurfaceReference*)pSurfaceCopy)->setRefGeoId( pSurface->getGmlId() );
          }
          else
            pSurfaceCopy = new CompositeSurface ( (CompositeSurface*)pSurface );

          position.push_back ( pSurfaceCopy );
          break;
        }

      case SHELL:
        {
          if ( generateMemberLinks && pFeatures != NULL )
          {
            pSurfaceCopy = new SurfaceReference ( pFeatures );
            ((SurfaceReference*)pSurfaceCopy)->setRefGeoId( pSurface->getGmlId() );
          }
          else
            pSurfaceCopy = new Shell ( (Shell*)pSurface );

          position.push_back ( pSurfaceCopy );
          break;
        }

		  case POLYGON :
			  {
          if ( generateMemberLinks && pFeatures != NULL )
          {
            pSurfaceCopy = new SurfaceReference ( pFeatures );
            ((SurfaceReference*)pSurfaceCopy)->setRefGeoId( pSurface->getGmlId() );
          }
          else
            pSurfaceCopy = new PolygonGeo ( (PolygonGeo*)pSurface );

          position.push_back ( pSurfaceCopy );
          break;
			  }

      case ORIENTABLE_SURFACE :
        {
          if ( generateMemberLinks && pFeatures != NULL )
          {
            pSurfaceCopy = new SurfaceReference ( pFeatures );
            ((SurfaceReference*)pSurfaceCopy)->setRefGeoId( pSurface->getGmlId() );
          }
          else
            pSurfaceCopy = new OrientableSurface ( (OrientableSurface*)pSurface );

          position.push_back ( pSurfaceCopy );
          break;
        }

      case SURFACE_REFERENCE:
        {
          SurfaceReference * pSurfaceReference = new SurfaceReference ( (SurfaceReference*)pSurface );
          position.push_back( pSurfaceReference );
          break;
        }
		  }
	}
}

MultiSurface::~MultiSurface()
{
	for  ( unsigned int i = 0; i < position.size(); i++ )
		delete position[i];
}

///////////////////////////////////////////////////////////////////////////////
// Kopiert alle SurfaceMember als referenzierte Geometrie                    //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSurface::copyWithReferences ( Features * pFeatures, vector<SurfaceReference*> &vRefs )
{
  unsigned int i;

  for ( i = 0; i < position.size(); i++ )
  {
    _Surface * pSurfaceMember = position[i];
    SurfaceReference * pRef = new SurfaceReference ( pFeatures );
    pRef->setRefGeoId( pSurfaceMember->getGmlId() );
    vRefs.push_back( pRef );
  }

  return vRefs.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine Multi-Surface aus einem Shapefile Polygon-Objekt          //
///////////////////////////////////////////////////////////////////////////////
void MultiSurface::addShapePolygons2D ( std::vector<double> xCoordP, std::vector<double> yCoordP, 
				                                std::vector<int> ringOffsetsP, std::vector<int> ringTypenP, 
                                        Features * pFeatures )
{
	size_t              i, j, polygonAnz, indexStart, ringPunktAnz, indexPolygon;
  int                 ringTyp;
	vector<PolygonGeo*> polygone;
	vector<double>      vX;
	vector<double>      vY;
	PolygonGeo		    * pPoly;
	LinearRing	      * pRing;
	LineSegment       * pLineSegment;

	//  Feststellen der Anzahl der Aussenkonturen
	polygonAnz = 0;
	for ( i = 0; i < ringTypenP.size(); i++ )
	{
		ringTyp = ringTypenP[i];
		if ( ringTyp > 0 ) polygonAnz++;
	}
	if ( polygonAnz == 0 ) return;

	//  Anlegen eines Vectors von Polygon*-Pointern entsprechender Länge
	polygone.assign ( polygonAnz, NULL );

	//  Anlegen der Aussenkonturen
	for ( i = 0; i < ringOffsetsP.size(); i++)
	{
		ringTyp = ringTypenP[i];
		if ( ringTyp > 0)
		{
			indexPolygon = ringTyp-1;
			indexStart = ringOffsetsP[i];
			if ( i  == ringOffsetsP.size()-1) ringPunktAnz = xCoordP.size()-indexStart;
			else                              ringPunktAnz = ringOffsetsP[i+1]-ringOffsetsP[i];//+1?

			vX.clear();
			vY.clear();
			for ( j = indexStart; j < indexStart + ringPunktAnz; j++ )
			{
				vX.push_back ( xCoordP[j] );
				vY.push_back ( yCoordP[j] );
			}
			pLineSegment = new LineSegment ( dim, LineSegment::_LINEAR_RING );
			pLineSegment->addPoints2D ( vX, vY );

			pRing = new LinearRing ( dim );
			pRing->setLineSegment ( pLineSegment );

			pPoly = new PolygonGeo ( dim );

      string gmlId =  Constant::createGmlId();
      pPoly->setGmlId ( gmlId );
      pFeatures->addGeometry( pPoly );

			pPoly->setSrsName ( srsName );
			pPoly->setAussenkontur ( pRing );
			polygone[indexPolygon] = pPoly;
		}
	}

	//  Anlegen der Löcher
	for ( i = 0; i < ringOffsetsP.size(); i++)
	{
		ringTyp = ringTypenP[i];
		if ( ringTyp < 0)
		{
			indexPolygon = -ringTyp - 1;
			indexStart = ringOffsetsP[i];
			if ( i  == ringOffsetsP.size()-1) ringPunktAnz = xCoordP.size()-indexStart;
			else                              ringPunktAnz = ringOffsetsP[i+1]-ringOffsetsP[i];//+1?

			vX.clear();
			vY.clear();
			for ( j = indexStart; j < indexStart + ringPunktAnz; j++ )
			{
				vX.push_back ( xCoordP[j] );
				vY.push_back ( yCoordP[j] );
			}
			pLineSegment = new LineSegment ( dim, LineSegment::_LINEAR_RING );
			pLineSegment->addPoints2D ( vX, vY );

			pRing = new LinearRing ( dim );
			pRing->setLineSegment ( pLineSegment );

			pPoly = polygone[indexPolygon];
			pPoly->addLoch ( pRing );
		}
	}

	for ( i = 0; i < polygone.size(); i++ ) position.push_back ( polygone[i] );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Surface-Member über den index                    						 //
///////////////////////////////////////////////////////////////////////////////
_Surface * MultiSurface::getSurface ( size_t index )
{
	if ( index < 0 || index >= position.size() ) return NULL;
	else										 return position[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding Box										                      	     //
///////////////////////////////////////////////////////////////////////////////
bool MultiSurface::getBoundingBox ( double * sw, double * no )
{
	bool	success;
	double  swLoc[3], noLoc[3];

  if ( bbExist )
  {
    sw[0] = m_sw[0];
    sw[1] = m_sw[1];
    no[0] = m_no[0];
    no[1] = m_no[1];
    sw[2] = m_sw[2];
    no[2] = m_no[2];
    return true;
  }

	sw[0] =  1.0E20;
	sw[1] =  1.0E20;
	no[0] = -1.0E20;
	no[1] = -1.0E20;
  if ( dim == 3 )
  {
    sw[2] =  1.0E20;
    no[2] = -1.0E20;
  }
  else
  {
    sw[2] = 0.0;
    no[2] = 0.0;
  }

	if ( position.size() == 0 ) return false;

	for ( unsigned int i = 0; i < position.size(); i++ )
	{ 
		_Surface * pSurface = position[i];
		success = pSurface->getBoundingBox ( swLoc, noLoc );
		if ( !success ) continue;

		if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
		if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
		if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
		if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( dim == 3 )
    {
      if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
      if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
    }
	}

  m_sw[0] = sw[0];
  m_sw[1] = sw[1];
  m_no[0] = no[0];
  m_no[1] = no[1];
  if ( dim == 3 )
  {
    m_sw[2] = sw[2];
    m_no[2] = no[2];
  }
  else
  {
    m_sw[2] = 0.0;
    m_no[2] = 0.0;
  }
  bbExist = true;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Surface-Members mit Index > 0		                  				 //
///////////////////////////////////////////////////////////////////////////////
void MultiSurface::removeMultiPolygon()
{
	size_t i, anz;

	anz = position.size();

	for ( i = 1; i < anz; i++ )
		position.pop_back ();
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den 2D-Schwerpunkt aller Member der Multi-Surface	      			 //
///////////////////////////////////////////////////////////////////////////////
bool MultiSurface::getCenter2D ( double &xP, double &yP )
{
	double       xLoc, yLoc;
	unsigned int i;
	bool         success;

	if ( position.size() == 0 ) return false;

	xP = 0.0;
	yP = 0.0;

	for ( i = 0; i < position.size(); i++ )
	{
		_Surface * pSurface = position[i];
		success = pSurface->getCenter2D ( xLoc, yLoc );
		if ( !success ) return false;

		xP = xP + xLoc;
		yP = yP + yLoc;
	}

	xP = xP / position.size();
	yP = yP / position.size();

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den 3D-Schwerpunkt aller Member der Multi-Surface	      			 //
///////////////////////////////////////////////////////////////////////////////
bool MultiSurface::getCenter3D ( double &xP, double &yP, double &zP )
{
  double       xLoc, yLoc, zLoc;
  unsigned int i;
  bool         success;

  success = getCenter2D( xP, yP );
  if ( !success ) return false;

  if ( dim == 2 )
    zP = 0.0;
  else
  {
    zP = 0.0;
    for ( i = 0; i < position.size(); i++ )
    {
      _Surface * pSurface = position[i];
      success = pSurface->getCenter3D ( xLoc, yLoc, zLoc );
      if ( !success ) return false;

      zP = zP + zLoc;
    }

    zP = zP / position.size();
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone                                          //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSurface::getPolygonAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getPolygonAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSurface::getCurveSegmentAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getCurveSegmentAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSurface::getPointAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getPointAnz();
  }
  return anz;
}

/////////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob eines der surfaceMember der MultiSurface eine Innenkontur hat //
/////////////////////////////////////////////////////////////////////////////////
bool MultiSurface::hasLoch()
{
  vector<PolygonGeo*> vPolygone;
	size_t              i, anz;

  anz = getPolygone( vPolygone );

  for ( i = 0; i < anz; i++ )
  {
    PolygonGeo * pPolygon = vPolygone[i];
    if ( pPolygon->getLochAnz() > 0 )
      return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////////////
//  Entfernt ein Polygon aus der MultiSurface aud Basis der gml:id             //
/////////////////////////////////////////////////////////////////////////////////
 _Surface * MultiSurface::removePolygon ( std::string gmlIdP )
 {
   _Surface * pPolyRemoved;
   for ( unsigned int i = 0; i < position.size(); i++ )
   {
     _Surface   * pSurface     = position[i];
     if ( pSurface->getGmlId() == gmlIdP )
     {
       for ( unsigned int j = i + 1; j < position.size(); ++j )
         position[j-1] = position[j];
       position.pop_back();
       return pSurface;
     }
     else
     {
       pPolyRemoved = pSurface->removePolygon( gmlIdP );
       if ( pPolyRemoved != NULL)
         return pPolyRemoved;
     }
   }
   return NULL;
 }
 
///////////////////////////////////////////////////////////////////////////////
//  Liefert alle zur Multi-Surface gehörigen Polygone			            			 //
///////////////////////////////////////////////////////////////////////////////
 size_t MultiSurface::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
	 size_t             i, j, patchAnz;
	vector<PolygonGeo*> patches;

	for ( i = 0; i < position.size(); i++ )
	{
		_Surface * pSurface = position[i];

    patches.clear();
    patchAnz = pSurface->getPolygone ( patches );
    for ( j = 0; j < patchAnz; j++ )
      vPolygone.push_back ( patches[j] );
	}

	return vPolygone.size();
}

 size_t MultiSurface::getPolygone ( std::vector<PolygonGeo*> &vPolygone, vector<bool> &vOrientations )
{
	 size_t             i, j, patchAnz;
  vector<PolygonGeo*> patches;
  vector<bool>        vOriLoc;

  for ( i = 0; i < position.size(); i++ )
  {
    _Surface * pSurface = position[i];
    
    patches.clear();
    vOriLoc.clear();
    patchAnz = pSurface->getPolygone ( patches, vOriLoc );
    for ( j = 0; j < patchAnz; j++ )
    {
      vPolygone.push_back ( patches[j] );
      vOrientations.push_back( vOriLoc[j] );
    }
  }

  return vPolygone.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Korrektheit einer Fläche (Basic-Check                      //
///////////////////////////////////////////////////////////////////////////////
bool MultiSurface::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
          vector<Error*> &vErrorReport )
{
  bool success;
  bool successAll = true;

  if ( position.size() == 0 )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );
    pError->setFeatureGeometryId ( gmlId );
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    _Surface * pSurface = position[i];
    success = pSurface->basicCheck ( pObj, featureProperty, pCheckParams, vErrorReport );
    if (!success ) successAll = false;
  }

  if ( successAll == false )
    checkResult = BASIC_CHECK_FAILED;
  else
    checkResult = BASIC_CHECK_FULFILLED;

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Korrektheit einer Fläche (Advanced-Check)                  //
///////////////////////////////////////////////////////////////////////////////
bool MultiSurface::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  bool success;
  bool successAll = true;

  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;

  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    _Surface * pSurface = position[i];
    success = pSurface->advancedCheck ( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
    if (!success ) successAll = false;
  }

  if ( successAll == false )
    checkResult = ADVANCED_CHECK_FAILED;
  else
    checkResult = ADVANCED_CHECK_FULFILLED;

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Überlappung von Multi-Surface Member                       //
///////////////////////////////////////////////////////////////////////////////
bool MultiSurface::topologyCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams, 
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  unsigned int         i, j, k;
  IntersectionGeometry intersectGeo;
  POLYGON_TOPOLOGY     polygonTopology = POLYGONE_DISJUNKT;
  POLYGON_TOPOLOGY     polygonTopologyLoc;
  GeometryError      * pError;
  MultiPoint         * pMultiPoint   = NULL;
  MultiCurve         * pMultiCurve   = NULL;
  MultiSurface       * pMultiSurface = NULL;

  if ( position.size() <= 1 )
    return true;

  if ( checkResult < ADVANCED_CHECK_FULFILLED && !checkAnyway )
    return false;

  for ( i = 0; i < position.size(); i++  )
  {
    _Surface * pSurfaceA = position[i];
    for ( j = i+1; j < position.size(); j++ )
    {
      _Surface * pSurfaceB = position[j];
      polygonTopologyLoc = pSurfaceA->intersect ( pSurfaceB, pCheckParams->minimalPointDistance, pCheckParams->minimalAngleDistance, false, intersectGeo );
      if ( polygonTopologyLoc > polygonTopology )
        polygonTopology = polygonTopologyLoc;
    }
  }

  for ( k = 0; k < intersectGeo.vCutPointTypes.size(); k++ )
  {
    if ( intersectGeo.vCutPointTypes[k] == PUNKT_AUF_FLAECHE /*|| intersectGeo.vCutPointTypes[k] == PUNKT_AUF_LINIE */ )
    {
      if ( pMultiPoint == NULL ) pMultiPoint = new MultiPoint ( dim );
      pMultiPoint->addPoint( new GeoPoint (intersectGeo.vCutPoints[k] ) );
    }
  }

  for ( k = 0; k < intersectGeo.vCutSegmentTypes.size(); k++ )
  {
    if ( intersectGeo.vCutSegmentTypes[k] == LINIE_AUF_FLAECHE || intersectGeo.vCutSegmentTypes[k] == FLAECHE_DURCH_FLAECHE )
    {
      if ( pMultiCurve == NULL ) pMultiCurve = new MultiCurve ( dim );
      pMultiCurve->addCurve( new LineSegment ( intersectGeo.vCutSegments[k] ) );
    }
  }

  for ( k = 0; k < intersectGeo.vCutRings.size(); k++ )
  {
    if ( pMultiSurface == NULL ) pMultiSurface = new MultiSurface ( dim );
    PolygonGeo * pPoly = new PolygonGeo ( dim );
    pPoly->setAussenkontur( new LinearRing ( intersectGeo.vCutRings[k] ) );
    pMultiSurface->addSurface( pPoly );
  } 

  if ( pMultiPoint != NULL )
  {
    pError = new GeometryError ( pObj, Error::WARNING, GeometryError::MULTISURFACE_IRREGULAR_POINT_TOUCH );
    pError->setPropertyName( featureProperty );
    pError->setFeatureGeometryId( getGmlId() );
    pError->addErrorGeometry( pMultiPoint, "Berührungspunkte außerhalb von Flächen-Stützpunkten" );
    vErrorReport.push_back( pError );
  }

  if ( pMultiCurve != NULL )
  {
    pError = new GeometryError ( pObj, Error::ERROR, GeometryError::MULTISURFACE_IRREGULAR_LINE_TOUCH_INTERSECT );
    pError->setPropertyName( featureProperty );
    pError->setFeatureGeometryId( getGmlId() );
    pError->addErrorGeometry( pMultiCurve, "Berührungs- oder Durchdringungslinien" );
    vErrorReport.push_back( pError );
  }

  if ( pMultiSurface != NULL )
  {
    pError = new GeometryError ( pObj, Error::ERROR, GeometryError::MULTISURFACE_IRREGULAR_SURFACE_OVERLAP );
    pError->setPropertyName( featureProperty );
    pError->setFeatureGeometryId( getGmlId() );
    pError->addErrorGeometry( pMultiSurface, "Überlappungsflächen" );
    vErrorReport.push_back( pError );
  }

  if ( pMultiPoint != NULL || pMultiCurve != NULL || pMultiSurface != NULL )
    return false;
  else
    return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Referenzen                                                 //
///////////////////////////////////////////////////////////////////////////////
bool MultiSurface::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  unsigned int i;
  bool success, successAll;

  successAll = true;

  for ( i = 0; i < position.size(); i++ )
  {
    _Surface * pGeo = dynamic_cast<_Surface*>(position[i]);
    success = pGeo->checkReferences ( pObj, propertyName, vErrorReport );
    if ( !success ) successAll = false;
  }

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Fläche der Multi-Surface incl. der Fläche der Löcher       //
///////////////////////////////////////////////////////////////////////////////
double MultiSurface::getFlaecheMitLoechern ()
{
  double flaeche = 0.0;

  for ( unsigned int i = 0; i < position.size(); i++ )
    flaeche = flaeche + position[i]->getFlaecheMitLoechern();

  return flaeche;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Fläche der Multi-Surface ohne die Fläche der Löcher        //
///////////////////////////////////////////////////////////////////////////////
double MultiSurface::getFlaeche()
{
  double flaeche = 0.0;

  for ( unsigned int i = 0; i < position.size(); i++ )
    flaeche = flaeche + position[i]->getFlaeche();

  return flaeche;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der Geometrie ein gültiges                   //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool MultiSurface::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    if ( position[i]->existReferenceSystem( pSrsManager ) == false )
      return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine globale Angabe über die Orientierung der Multifläche        //
///////////////////////////////////////////////////////////////////////////////
SURFACE_NORMAL_TYPE MultiSurface::getSurfaceNormal( double angleTolenance, double areaToleranceRelative )
{
  vector<PolygonGeo*>         vPolygone;
  vector<bool>             vOrientations;
  PolygonGeo                * pPoly;
  bool                     ori;

	size_t     i, anz;
	size_t     anzVerticalUpward    = 0; 
	size_t     anzUpward            = 0; 
	size_t     anzHorizontal        = 0; 
	size_t     anzDownward          = 0; 
	size_t     anzVerticalDownward  = 0; 
  double     areaVerticalUpward   = 0.0;
  double     areaUpward           = 0.0;
  double     areaHorizontal       = 0.0;
  double     areaDownward         = 0.0;
  double     areaVerticalDownward = 0.0;
  double     areaGesamt           = 0.0;
  double     area;
  double     areaTolerance;
  
  anz = getPolygone( vPolygone, vOrientations );
  if ( anz == 0 )
    return INDIFFERENT;

  for ( i = 0; i < anz; i++  )
  {
    pPoly = vPolygone[i];
    ori   = vOrientations[i];

    SURFACE_NORMAL_TYPE surfaceOri = pPoly->getSurfaceNormal( areaToleranceRelative, 0.0 );
    area = fabs ( pPoly->getFlaeche() );
    areaGesamt = areaGesamt + area;

    switch ( surfaceOri )
    {
    case VERTICAL_UPWARD:
      if ( ori )
      {
        anzVerticalUpward++;
        areaVerticalUpward = areaVerticalUpward + area;
      }
      else
      {
        anzVerticalDownward++;
        areaVerticalDownward = areaVerticalDownward + area;
      }
      break;

    case UPWARD:
      if ( ori )
      {
        anzUpward++;
        areaUpward = areaUpward + area;
      }
      else
      {
        anzDownward++;
        areaDownward = areaDownward + area;
      }
      break;

    case HORIZONTAL:
      anzHorizontal++;
      areaHorizontal = areaHorizontal + area;
      break;

    case DOWNWARD:
      if ( !ori )
      {
        anzUpward++;
        areaUpward = areaUpward + area;
      }
      else
      {
        anzDownward++;
        areaDownward = areaDownward + area;
      }
      break;

      if ( ori )
      {
        anzVerticalUpward++;
        areaVerticalUpward = areaVerticalUpward + area;
      }
      else
      {
        anzVerticalDownward++;
        areaVerticalDownward = areaVerticalDownward + area;
      }
      break;
    }
  }

  areaTolerance = areaGesamt*( 1.0 - areaToleranceRelative );

  if ( areaVerticalUpward > areaTolerance  )
    return VERTICAL_UPWARD;

  if ( areaVerticalDownward > areaTolerance  )
    return VERTICAL_DOWNWARD;
  
  if ( areaVerticalUpward + areaUpward > areaTolerance )
    return UPWARD;
  
  if ( areaVerticalDownward + areaDownward > areaTolerance )
    return DOWNWARD;

  if ( areaHorizontal > areaTolerance )
    return HORIZONTAL;

  return INDIFFERENT;    
}

///////////////////////////////////////////////////////////////////////////////
//  Extrahiert alle Kanten, die nur einmal durchlaufen werden, und fügt sie  //
// zu möglichst langen Linien-Segmenten zusammen                             //
///////////////////////////////////////////////////////////////////////////////
MultiCurve * MultiSurface::createBoundaryCurves( double minDist, Features * pFeaturesP )
{
  BRep * pBrep = new BRep ( this, minDist, pFeaturesP );
  pBrep->createIncorrectEdgesForSolid();
  MultiCurve * pBorder = pBrep->getSingleEdges();
  return pBorder;
}

///////////////////////////////////////////////////////////////////////////////
//  Geometrischen Offset auf die einzelnen Member                            //
///////////////////////////////////////////////////////////////////////////////
void MultiSurface::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  )
{
  _Surface::offset( point, sGeometries );

  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    if ( sGeometries.count( pGeo ) == 0 )
    {
      pGeo->offset( point, sGeometries );
    }
  }
  sGeometries.insert ( this );
}

///////////////////////////////////////////////////////////////////////////////
// Clustert die Polygone der MultiSurface an Hand ihrer Orientiertung. Zwei  //
// Polygone gehören zum selben Cluster, wenn sich Ihre Orientierungen um     //
// weniger als maximalAngleDifference (Winkelangabe in Grad) unterscheiden   //
// Der Mittelwert aller Richtungsvektoren wird dem Cluster als Orientierung  //
// zugeordnet                                                                //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSurface::clusterPolygonsByOrientations ( map<Direction*,vector<PolygonGeo*>> &mClusters, 
                                                  double maximalAngleDifference )
{
  vector<PolygonGeo*> vPolygone;
  vector<bool>        vOrientations;
	size_t              i, anz;

  maximalAngleDifference = (maximalAngleDifference*PI)/180.0;

  map<Direction*,vector<PolygonGeo*>>           mClustersLoc;
  map<Direction*,vector<PolygonGeo*>>::iterator iter;

  anz = getPolygone( vPolygone, vOrientations );
  for ( i = 0; i < anz; i++ )
  {
    PolygonGeo * pPolygon = vPolygone[i];
    bool         ori      = vOrientations[i];

    Direction * pNormal = pPolygon->getNormal();
    if ( pNormal == NULL )
      continue;    

    PolygonGeo * pPolygonCopy = new PolygonGeo ( pPolygon );
    if ( ori == false )
    {
      pPolygonCopy->reverse();
      pNormal->reverse();
    }
    
    if ( i == 0 )
    {
      vector<PolygonGeo*> vPolygone;
      vPolygone.push_back( pPolygonCopy );
      mClustersLoc[pNormal] = vPolygone;
    }
    else
    {
      bool found = false;
      for ( iter = mClustersLoc.begin(); iter != mClustersLoc.end(); ++iter )
      {
        Direction * pNormalLoc = iter->first;
        double difference = pNormalLoc->getDifference( *pNormal );
        if ( pNormalLoc->getDifference( *pNormal ) < maximalAngleDifference )
        {
          iter->second.push_back( pPolygonCopy );
          found = true;
          break;
        }
      }
      if ( !found )
      {
        vector<PolygonGeo*> vPolygone;
        vPolygone.push_back( pPolygonCopy );
        mClustersLoc[pNormal] = vPolygone;
      }
    }  
  }

  if ( mClustersLoc.size() == 0 )
    return 0;

  for ( iter = mClustersLoc.begin(); iter != mClustersLoc.end(); ++iter )
  {
    Direction * normalAvarage = new Direction;
    vector<PolygonGeo*> vPolygone = iter->second;
    for ( i = 0; i < vPolygone.size(); ++i )
    {
      Direction * pNormalLoc = vPolygone[i]->getNormal();
      if ( pNormalLoc == NULL )
      {
        vector<double> vX, vY, vZ;
        vPolygone[i]->getAussenkontur()->getPoints3D( vX, vY, vZ );
      }
      else
        *normalAvarage = *normalAvarage + *pNormalLoc;
    }
    normalAvarage->normalize();
    mClusters[normalAvarage] = iter->second;
  }

  return mClustersLoc.size();
}

//////////////////////////////////////////////////////////////////////////////////
// Versucht die Polygone eines Orientierungs-Clusters mengenmäßig zu vereinigen //
// und die Außenkontur der Vereinigungsfläche zu bestimmen. Dabei werden        //
// eventuell vorhandene oder bei der Vereinigung entstehende Löcher ignoriert   //
//////////////////////////////////////////////////////////////////////////////////
size_t MultiSurface::mergeCluster( vector<PolygonGeo*> vCluster, Direction clusterOrientation, 
                vector<PolygonGeo*> &vMergedCluster, double maxPointDistance, double maxOrientationDistance )
{
  unsigned int        i;
  double              x, y, z;
  vector<TopolRing*>  vRingsUnited;
  vector<GeoPoint>      vPoints;

  TopolRingSet * pRingSet = new TopolRingSet;

  PolygonGeo * pPolygon = vCluster[0];
  pPolygon->getCenter3D( x, y, z );
  GeoPoint p = GeoPoint( x, y, z );
  Plane e = Plane( p, clusterOrientation );
  Matrix4 matrix = e.getTransformationToHorizontal();

  for ( i = 0; i < vCluster.size(); i++ )
  {
    pPolygon = vCluster[i];
    pPolygon->transform( matrix );
    vPoints.clear();
    pPolygon->getAussenkontur()->getPoints( vPoints );

    TopolRing * pRing = new TopolRing;
    pRing->setMinDist( maxPointDistance );
    pRing->setMinAngleDist( maxOrientationDistance );
    pRing->create(pPolygon->getAussenkontur() );
    pRingSet->addRing ( pRing );
  }

  pRingSet->_union( vRingsUnited );
  matrix = matrix.invers();

  for ( i = 0; i < vRingsUnited.size(); i++ )
  {
    TopolRing * pTopolRing = vRingsUnited[i];
    LinearRing * pRing = pTopolRing->createLinearRing();
    PolygonGeo * pPolygon = new PolygonGeo ( 3 );
    pPolygon->setAussenkontur( pRing );
    pPolygon->transform ( matrix );
    vMergedCluster.push_back( pPolygon );
  }

  delete pRingSet;

  return vMergedCluster.size();
}

//////////////////////////////////////////////////////////////////////////////////
//  Führt die beiden Routinen clusterPolygonsByOrientations und mergeCluster    //
//  hintereinander aus                                                          //
//////////////////////////////////////////////////////////////////////////////////
int MultiSurface::mergeSurfaceMember ( double maximalAngleDifference, vector<PolygonGeo*> &vMergedPolygons,
                                      double minimalPointDistance, double minimalAngleDistance )
{
  map<Direction*,vector<PolygonGeo*>>           mClusters;
  map<Direction*,vector<PolygonGeo*>>::iterator iter;

  clusterPolygonsByOrientations ( mClusters, maximalAngleDifference );

  for ( iter = mClusters.begin(); iter != mClusters.end(); ++iter )
  {
    Direction         * pDir = iter->first;
    vector<PolygonGeo*> vCulster = iter->second;

    mergeCluster( vCulster, *pDir, vMergedPolygons, minimalPointDistance, minimalAngleDistance );
  }

  for ( iter = mClusters.begin(); iter != mClusters.end(); ++iter )
  {
    delete iter->first;
    for ( unsigned int i = 0; i < iter->second.size(); i++ )
      delete iter->second[i];
  }

  return (int)vMergedPolygons.size();
}

///////////////////////////////////////////////////////////////////////////////
//  CompositeSurface				                              									 //
//  Klasse für eine gml:CompositeSurface	                                   //
///////////////////////////////////////////////////////////////////////////////
CompositeSurface::CompositeSurface( int dimP )
:MultiSurface ( dimP )
{

}
CompositeSurface::CompositeSurface ( CompositeSurface * pCompositeSurfaceOld )
: MultiSurface ( pCompositeSurfaceOld )
{

}
CompositeSurface::~CompositeSurface()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Shell				                                          									 //
//  Klasse für eine gml:Shell	                                               //
///////////////////////////////////////////////////////////////////////////////
Shell::Shell( int dimP )
:CompositeSurface ( dimP )
{

}
Shell::Shell ( Shell * pCompositeSurfaceOld )
: CompositeSurface ( pCompositeSurfaceOld )
{

}
Shell::~Shell()
{

}

///////////////////////////////////////////////////////////////////////////////
//  TriangulatedSurface                                   									 //
//  Klasse für eine gml:TriangulatedSurface                                  //
///////////////////////////////////////////////////////////////////////////////
TriangulatedSurface::TriangulatedSurface( int dimP )
:Surface ( dimP )
{

}
TriangulatedSurface::TriangulatedSurface ( TriangulatedSurface * pTriangulatedSurfaceOld )
: Surface ( pTriangulatedSurfaceOld )
{

}
TriangulatedSurface::~TriangulatedSurface()
{

}

///////////////////////////////////////////////////////////////////////////////
//  MultiCurve										                              						 //
//  Klasse für eine Multi-Curve										                      		 //
///////////////////////////////////////////////////////////////////////////////
MultiCurve::MultiCurve( int dimP ) : _Geometrie( dimP )
{
	isCompositeCurve = true;
}

MultiCurve::MultiCurve ( MultiCurve * pMultiCurveOld ): _Geometrie ( pMultiCurveOld )
{
	isCompositeCurve = pMultiCurveOld->isCompositeCurve;

	for ( unsigned int i = 0; i < pMultiCurveOld->position.size(); i++ )
	{
		_Curve * pCurve = pMultiCurveOld->position[i];
    GEOMETRY_TYPE type = pCurve->getCurveType();
		switch ( type )
		{
		case CURVE:
			{
				Curve * pCurveCopy = new Curve ( (Curve*)pCurve );
				position.push_back ( pCurveCopy );
				break;
			}

		case LINE_STRING:
    case LINE_STRING_SEGMENT:
			{
				LineSegment * pLineSegment = new LineSegment ( (LineSegment*)pCurve );
				position.push_back ( pLineSegment );
				break;
			}

		case ARC:
    case ARC_STRING:
		case CIRCLE:
			{
				ArcSegment * pArcSegment = new ArcSegment ( (ArcSegment*)pCurve );
				position.push_back ( pArcSegment );
				break;
			}

    case CURVE_REFERENCE:
      {
        CurveReference * pCurveRef = new CurveReference ( (CurveReference*)pCurve );
        position.push_back( pCurveRef );
        break;
      }
		}
	}
}

MultiCurve::~MultiCurve()
{
	for ( unsigned int i = 0; i < position.size(); i++ )
		delete position[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Linien-Geometrie aus einem Shape-File hinzu						             //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve::addShapeLines2D ( std::vector<double> xCoordP, std::vector<double> yCoordP, 
				                           std::vector<int> partOffsetsP, Features * pFeatures )
{
	size_t          	    i, j, indexStart, punktAnz;
	vector<double>        vX;
	vector<double>        vY;
  string                gmlId;
	CurveSegment        * pCurveSegment;
	vector<CurveSegment*> vCurveSegments;

	if ( partOffsetsP.size() ==  0 ) return;

	for ( i = 0; i < partOffsetsP.size(); i++ )
	{
		indexStart = partOffsetsP[i];
		if ( i < partOffsetsP.size()-1 ) punktAnz = partOffsetsP[i+1] - partOffsetsP[i];
		else							 punktAnz = xCoordP.size()-indexStart;

		vX.clear();
		vY.clear();
		for ( j = indexStart; j < indexStart + punktAnz; j++ )
		{
			vX.push_back ( xCoordP[j] );
			vY.push_back ( yCoordP[j] );
		}
		pCurveSegment = new LineSegment ( dim, LineSegment::_LINE_STRING );

    string gmlId = Constant::createGmlId();
    pCurveSegment->setGmlId( gmlId );
    pFeatures->addGeometry( pCurveSegment );

		pCurveSegment->setSrsName ( srsName );
		pCurveSegment->addPoints2D ( vX, vY );

		vCurveSegments.push_back ( pCurveSegment );
	}
	joinSegments ( vCurveSegments, MINIMAL_POINT_DISTANCE );
	for ( i = 0; i < vCurveSegments.size(); i++ )
	{
		pCurveSegment = vCurveSegments[i];
		position.push_back ( pCurveSegment );
	}

}

///////////////////////////////////////////////////////////////////////////////
//  Verbindet zusammenhängende Kurvensegmente				                				 //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve::joinSegments( vector<CurveSegment*> & vCurveSegments, double minDist )
{
	unsigned int   i, j;
	CurveSegment * pSegI, *pSegJ;
	GeoPoint		   pAnfI (dim), pEndI(dim), pAnfJ(dim), pEndJ(dim);
	bool           weiter = true;

	while ( weiter )
	{
		weiter = false;
		for ( i = 0; i < vCurveSegments.size(); i++ )
		{
			pSegI = vCurveSegments[i];
			pAnfI = pSegI->getAnfangsPunkt();
			pEndI = pSegI->getEndPunkt();

		    for ( j = i+1; j < vCurveSegments.size(); j++ )
			{
				pSegJ = vCurveSegments[j];
				pAnfJ = pSegJ->getAnfangsPunkt();
				pEndJ = pSegJ->getEndPunkt();

				if ( pAnfI.dist( pAnfJ ) < minDist )
				{
					weiter = true;
					pSegJ->reverse();
					pSegJ->join ( pSegI );
					deleteSegment ( vCurveSegments, i );
					break;
				}

				if ( pAnfI.dist( pEndJ ) < minDist )
				{
					weiter = true;
					pSegJ->join ( pSegI );
					deleteSegment ( vCurveSegments, i );
					break;
				}

				if ( pEndI.dist( pAnfJ ) < minDist )
				{
					weiter = true;
					pSegI->join ( pSegJ );
					deleteSegment ( vCurveSegments, j );
					break;
				}

				if ( pEndI.dist( pEndJ ) < minDist )
				{
					weiter = true;
					pSegJ->reverse();
					pSegI->join ( pSegJ );
					deleteSegment ( vCurveSegments, j );
					break;
				}
			}
			if ( weiter ) break;
		}

		if ( vCurveSegments.size() == 1 ) weiter = false;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht ein Kurvensegment auf Basis des Index                             //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve::deleteSegment ( vector<CurveSegment*> & vCurveSegments, unsigned int indexP )
{
	unsigned int i;

	if ( indexP < 0 || indexP >= vCurveSegments.size() ) return;

	CurveSegment * pSeg = vCurveSegments[indexP];
	for ( i = indexP + 1; i < vCurveSegments.size(); i++ ) vCurveSegments[i-1] = vCurveSegments[i]; 
	vCurveSegments.pop_back();
	delete pSeg;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Curve-Member hinzu										                      		 //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve::addCurve (  _Curve * pCurve )
{
	position.push_back ( pCurve );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Curve-Member mit einem best. Index					            		 //
///////////////////////////////////////////////////////////////////////////////
_Curve * MultiCurve::getCurve ( size_t index )
{
	if ( index < 0 || index >= position.size() ) return NULL;
	else										 return position[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Curve-Members										                        	 //
///////////////////////////////////////////////////////////////////////////////
size_t MultiCurve::getCurves ( std::vector<_Curve*> &vCurves )
{
	unsigned int i;

	for ( i = 0; i < position.size(); i++ )
		vCurves.push_back ( position[i] );

	return vCurves.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding Box										                        		 //
///////////////////////////////////////////////////////////////////////////////
bool MultiCurve::getBoundingBox ( double * sw, double * no )
{
	bool	success;
	double  swLoc[3], noLoc[3];

  if ( bbExist )
  {
    sw[0] = m_sw[0];
    sw[1] = m_sw[1];
    no[0] = m_no[0];
    no[1] = m_no[1];
    sw[2] = m_sw[2];
    no[2] = m_no[2];
    return true;
  }

	sw[0] =  1.0E20;
	sw[1] =  1.0E20;
	no[0] = -1.0E20;
	no[1] = -1.0E20;
  if ( dim == 3 )
  {
    sw[2] =  1.0E20;
    no[2] = -1.0E20;
  }
  else
  {
    sw[2] = 0.0;
    no[2] = 0.0;
  }

	if ( position.size() == 0 ) return false;

	for ( unsigned int i = 0; i < position.size(); i++ )
	{ 
		_Curve * pCurve = position[i];
		success = pCurve->getBoundingBox ( swLoc, noLoc );
		if ( !success ) continue;

		if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
		if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
		if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
		if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( dim == 3 )
    {
      if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
      if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
    }
	}

  m_sw[0] = sw[0];
  m_sw[1] = sw[1];
  m_no[0] = no[0];
  m_no[1] = no[1];
  if ( dim == 3 )
  {
    m_sw[2] = sw[2];
    m_no[2] = no[2];
  }
  else
  {
    m_sw[2] = 0.0;
    m_no[2] = 0.0;
  }

  bbExist = true;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den 2D-Schwerpunkt  					                        					 //
///////////////////////////////////////////////////////////////////////////////
bool MultiCurve::getCenter2D ( double &xP, double &yP )
{
	double       xLoc, yLoc;
	unsigned int i;
	bool         success;

	if ( position.size() == 0 ) return false;

	xP = 0.0;
	yP = 0.0;

	for ( i = 0; i < position.size(); i++ )
	{
		_Curve * pCurve = position[i];
		success = pCurve->getCenter2D ( xLoc, yLoc );
		if ( !success ) return false;

		xP = xP + xLoc;
		yP = yP + yLoc;
	}

	xP = xP / position.size();
	yP = yP / position.size();

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den 3D-Schwerpunkt  					                        					 //
///////////////////////////////////////////////////////////////////////////////
bool MultiCurve::getCenter3D ( double &xP, double &yP, double &zP )
{
  double       xLoc, yLoc, zLoc;
  unsigned int i;
  bool         success;

  success = getCenter2D( xP, yP );
  if ( !success ) return false;

  if ( dim == 2 )
    zP = 0.0;
  else
  {
    zP = 0.0;

    for ( i = 0; i < position.size(); i++ )
    {
      _Curve * pCurve = position[i];
      success = pCurve->getCenter3D ( xLoc, yLoc, zLoc );
      if ( !success ) return false;

      zP = zP + zLoc;
    }

    zP = zP / position.size();
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t MultiCurve::getCurveSegmentAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getCurveSegmentAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t MultiCurve::getPointAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    anz = anz + pGeo->getPointAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (Basic-Check)                                    //
///////////////////////////////////////////////////////////////////////////////
bool MultiCurve::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  unsigned int i, fehlerAnz;
  bool         success;

  if ( position.size() == 0 )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );
    pError->setFeatureGeometryId ( gmlId );
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  fehlerAnz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    success = pGeo->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );
    if ( !success )
      fehlerAnz++;
  }

  if ( fehlerAnz > 0 )
  {
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }
  else
  {
    checkResult = BASIC_CHECK_FULFILLED;
    return true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (Advanced-Check)                                 //
///////////////////////////////////////////////////////////////////////////////
bool MultiCurve::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  unsigned int i, fehlerAnz;
  bool         success;

  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;

  fehlerAnz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    success = pGeo->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
    if ( !success )
      fehlerAnz++;
  }

  if ( fehlerAnz > 0 )
  {
    checkResult = ADVANCED_CHECK_FAILED;
    return false;
  }
  else
  {
    checkResult = ADVANCED_CHECK_FULFILLED;
    return true;
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Referenzen                                                 //
///////////////////////////////////////////////////////////////////////////////
bool MultiCurve::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  unsigned int i;
  bool success, successAll;

  successAll = true;

  for ( i = 0; i < position.size(); i++ )
  {
    _Curve * pGeo = dynamic_cast<_Curve*>(position[i]);
    success = pGeo->checkReferences ( pObj, propertyName, vErrorReport );
    if ( !success ) successAll = false;
  }

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Curve-Member bis auf das Erste		              					 //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve::removeMultiLine()
{
	size_t i, anz;

	anz = position.size();

	for ( i = 1; i < anz; i++ )
		position.pop_back ();
}

///////////////////////////////////////////////////////////////////////////////
//  Dreht die Laufrichtung der Multi-Curve um					                			 //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve::reverse()
{
	size_t   i, ii;
	_Curve * pCurveA;
	_Curve * pCurveB;
	int     anzHalbe = div ( (int)position.size(), 2 ).quot;

	for ( i = 0; i < (size_t)anzHalbe; i++ )
	{
		ii = position.size() - 1 - i;

		pCurveA = position[i];
		pCurveA->reverse();
		pCurveB = position[ii];
		pCurveB->reverse();

		position[i]  = pCurveB;
		position[ii] = pCurveA;
	}

	if (  div ( (int)position.size(), 2 ).rem == 1 )
		position[anzHalbe]->reverse();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von Punkten im SVG-path Format    						              		 //
///////////////////////////////////////////////////////////////////////////////
string MultiCurve::getPointsSVG ( double offsetX, double offsetY )
{
	string         pathStringSegment;
	ostringstream  os;
	unsigned int	 i;
  GeoPoint       p(2);

  if ( position.size() == 0 ) return "";

  p = position[0]->getAnfangsPunkt();

  os << "M ";
  os << p.getX() - offsetX << " " << offsetY - p.getY() << " ";

	for ( i = 0; i < position.size(); i++ )
	{
		_Curve * pCurveSegment = position[i];
    if ( i == 0 || isCompositeCurve == false )
	    pathStringSegment = pCurveSegment->getSegmentPointsSVG ( offsetX, offsetY, MOVE );
    else
      pathStringSegment = pCurveSegment->getSegmentPointsSVG ( offsetX, offsetY, OMIT );
		os << pathStringSegment << " ";
	}
	return os.str();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von Punkten im SVG-path Format in umgekehrter Reihenfolge    		 //
///////////////////////////////////////////////////////////////////////////////
string MultiCurve::getPointsReverseSVG ( double offsetX, double offsetY )
{
	string         pathStringSegment;
	ostringstream  os;
	int   			   i;

	for ( i = (int)position.size()-1; i >= 0 ; i-- )
	{
		_Curve * pCurveSegment = position[i];
    if ( i == (int)position.size()-1 || isCompositeCurve == false )
		  pathStringSegment = pCurveSegment->getSegmentPointsReverseSVG ( offsetX, offsetY, MOVE ); 
    else
      pathStringSegment = pCurveSegment->getSegmentPointsReverseSVG ( offsetX, offsetY, OMIT ); 
		os << pathStringSegment << " ";
	}
	return os.str();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Länge der Multi-Curve								                         //
///////////////////////////////////////////////////////////////////////////////
double MultiCurve::getLength()
{
	unsigned int i;
	double       laenge = 0.0;

	for ( i = 0; i < position.size(); i++ )
	{
		_Curve * pCurve = position[i];
		laenge = laenge + pCurve->getLength();
	}

	return laenge;
}

///////////////////////////////////////////////////////////////////////////////
//  Affine Transformation                                                    //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve::transform ( Matrix4 & matrix )
{
  unsigned int   i;
  _Curve       * pAbstractCurve;
  Curve        * pCurve;
  LineSegment  * pLineSegment;

  bbExist = false;

  for ( i = 0; i < position.size(); i++ )
  {
    pAbstractCurve = position[i];

    switch ( pAbstractCurve->getGeometryType() )
    {
    case CURVE:
      pCurve = dynamic_cast<Curve*> (pAbstractCurve);
      pCurve->transform( matrix );
      break;

    case LINE_STRING:
		case LINE_STRING_SEGMENT:
      pLineSegment = dynamic_cast<LineSegment*> (pAbstractCurve);
      pLineSegment->transform( matrix );
      break;    
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Inverse affine Transformation                                            //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve::transformInvers ( Matrix4 & matrix )
{
  unsigned int   i;
  _Curve       * pAbstractCurve;
  Curve        * pCurve;
  LineSegment  * pLineSegment;

  bbExist = false;

  for ( i = 0; i < position.size(); i++ )
  {
    pAbstractCurve = position[i];

    switch ( pAbstractCurve->getGeometryType() )
    {
    case CURVE:
      pCurve = dynamic_cast<Curve*> (pAbstractCurve);
      pCurve->transformInvers( matrix );
      break;

    case LINE_STRING:
		case LINE_STRING_SEGMENT:
      pLineSegment = dynamic_cast<LineSegment*> (pAbstractCurve);
      pLineSegment->transformInvers( matrix );
      break;    
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der Geometrie ein gültiges                   //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool MultiCurve::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    if ( position[i]->existReferenceSystem( pSrsManager ) == false )
      return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Geometrischen Offset auf die einzelnen Member                            //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  )
{
  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    if ( sGeometries.count( pGeo ) == 0 )
    {
      pGeo->offset( point, sGeometries );
    }
  }
  sGeometries.insert ( this );
}


///////////////////////////////////////////////////////////////////////////////
//  MultiPoint																                               //
//  Klasse für Multi-Points													                         //
///////////////////////////////////////////////////////////////////////////////
MultiPoint::MultiPoint( int dimP ): _Geometrie( dimP )
{
}

MultiPoint::MultiPoint ( MultiPoint * pMultiPointOld ): _Geometrie ( pMultiPointOld )
{
	for ( unsigned int i = 0; i < pMultiPointOld->position.size(); i++ )
	{
		GeoPoint * pPoint = pMultiPointOld->position[i];
    GeoPoint * pPointNew = new GeoPoint ( pPoint ); 
		position.push_back ( pPointNew );
	}
}

MultiPoint::~MultiPoint()
{
  unsigned int i;

  for ( i = 0; i < position.size(); i++  )
    delete position[i];
}


///////////////////////////////////////////////////////////////////////////////
//  Fügt Punkt-Geometrie aus einem Shape-File hinzu		            					 //
///////////////////////////////////////////////////////////////////////////////
void MultiPoint::addShapePoints2D ( std::vector<double> xCoordP, std::vector<double> yCoordP, Features * pFeatures )
{
	GeoPoint   * pPoint;
	unsigned int i;

	if ( xCoordP.size() == 0 ) return;

	for ( i = 0; i < xCoordP.size(); i++ )
	{
		pPoint =  new GeoPoint ( xCoordP[i], yCoordP[i] );

    string gmlId = Constant::createGmlId();
    pPoint->setGmlId( gmlId );
    pFeatures->addGeometry( pPoint );

		pPoint->setSrsName ( srsName );
		addPoint ( pPoint );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Punkt hinzu													                         //
///////////////////////////////////////////////////////////////////////////////
void MultiPoint::addPoint (  GeoPoint * pPoint )
{
	position.push_back ( pPoint );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Punkt-Member zu index								                     		 //
///////////////////////////////////////////////////////////////////////////////
GeoPoint*  MultiPoint::getPoint ( size_t index )
{
	if ( index < 0 || index >= position.size() ) return NULL;
	else										                     return position[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die 2D-Koordinaten aller Punkte	                								 //
///////////////////////////////////////////////////////////////////////////////
size_t MultiPoint::getPoints2D ( std::vector<double> &xCoordP, std::vector<double> &yCoordP )
{
	unsigned int i;
	GeoPoint   * pPoint;

	for ( i = 0; i < position.size(); i++ )
	{
		pPoint = position[i];
		xCoordP.push_back ( pPoint->getX() );
		yCoordP.push_back ( pPoint->getY() );
	}

	return xCoordP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die 3D-Koordinaten aller Punkte	                								 //
///////////////////////////////////////////////////////////////////////////////
size_t MultiPoint::getPoints3D ( std::vector<double> &xCoordP, std::vector<double> &yCoordP, std::vector<double> &zCoordP )
{
  unsigned int i;
  GeoPoint   * pPoint;

  for ( i = 0; i < position.size(); i++ )
  {
    pPoint = position[i];
    xCoordP.push_back ( pPoint->getX() );
    yCoordP.push_back ( pPoint->getY() );
    zCoordP.push_back ( pPoint->getZ() );
  }

  return xCoordP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Punkte als vector von Geopoints             								 //
///////////////////////////////////////////////////////////////////////////////
size_t MultiPoint::getPoints ( std::vector<GeoPoint*> &vPoints )
{
  for ( unsigned int i = 0; i < position.size(); i++ )
    vPoints.push_back( position[i] );

  return vPoints.size();
}

//////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (Basic-Check)                                   //
//////////////////////////////////////////////////////////////////////////////
bool MultiPoint::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  unsigned int i, fehlerAnz;
  bool         success;

  if ( position.size() == 0 )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );
    pError->setFeatureGeometryId ( gmlId );
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  fehlerAnz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    GeoPoint * pPoint = position[i];
    success = pPoint->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );
    if ( !success )
      fehlerAnz++;
  }

  if ( fehlerAnz > 0 )
  {
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }
  else
  {
    checkResult = BASIC_CHECK_FULFILLED;
    return true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (Advanced-Check)                                //
//////////////////////////////////////////////////////////////////////////////
bool MultiPoint::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  unsigned int i, fehlerAnz;
  bool         success;

  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;

  fehlerAnz = 0;
  for ( i = 0; i < position.size(); i++ )
  {
    GeoPoint * pPoint = position[i];
    success = pPoint->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
    if ( !success )
      fehlerAnz++;
  }

  if ( fehlerAnz > 0 )
  {
    checkResult =  ADVANCED_CHECK_FAILED;
    return false;
  }
  else
  {
    checkResult = ADVANCED_CHECK_FULFILLED;
    return true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Referenzen                                                 //
///////////////////////////////////////////////////////////////////////////////
bool MultiPoint::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  unsigned int i;
  bool success, successAll;

  successAll = true;

  for ( i = 0; i < position.size(); i++ )
  {
    GeoPoint * pGeo = dynamic_cast<GeoPoint*>(position[i]);
    if ( pGeo->getGeometryType() == POINT_REFERENCE )
    {
      success = pGeo->checkReferences ( pObj, propertyName, vErrorReport );
      if ( !success ) successAll = false;
    }
  }

  return successAll;
}


//////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding-Box			                        									//
//////////////////////////////////////////////////////////////////////////////
bool MultiPoint::getBoundingBox ( double * sw, double * no )
{
	bool	success;
	double  swLoc[3], noLoc[3];

  if ( bbExist )
  {
    sw[0] = m_sw[0];
    sw[1] = m_sw[1];
    no[0] = m_no[0];
    no[1] = m_no[1];
    sw[2] = m_sw[2];
    no[2] = m_no[2];
    return true;
  }

	sw[0] =  1.0E20;
	sw[1] =  1.0E20;
	no[0] = -1.0E20;
	no[1] = -1.0E20;
  if ( dim == 3 )
  {
    sw[2] =  1.0E20;
    no[2] = -1.0E20;
  }
  else
  {
    sw[2] = 0.0;
    no[2] = 0.0;
  }

	if ( position.size() == 0 ) return false;

	for ( unsigned int i = 0; i < position.size(); i++ )
	{ 
		GeoPoint pPoint = position[i];
		success = pPoint.getBoundingBox ( swLoc, noLoc );
		if ( !success ) continue;

		if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
		if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
		if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
		if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( dim == 3 )
    {
      if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
      if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
    }
	}

  m_sw[0] = sw[0];
  m_sw[1] = sw[1];
  m_no[0] = no[0];
  m_no[1] = no[1];
  if ( dim == 3 )
  {
    m_sw[2] = sw[2];
    m_no[2] = no[2];
  }
  else
  {
    m_sw[2] = 0.0;
    m_no[2] = 0.0;
  }
  bbExist = true;

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//  Liefert den 2D-Schwerpunkt			  			                                //
//////////////////////////////////////////////////////////////////////////////
bool MultiPoint::getCenter2D ( double &xP, double &yP )
{
	unsigned int i;

	if ( position.size() == 0 ) return false;

	xP = 0.0;
	yP = 0.0;

	for ( i = 0; i < position.size(); i++ )
	{
		GeoPoint pPoint = position[i];
		xP = xP + pPoint.getX();
		yP = yP + pPoint.getY();
	}

	xP = xP / position.size();
	yP = yP / position.size();

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//  Liefert den 3D-Schwerpunkt			  			                                //
//////////////////////////////////////////////////////////////////////////////
bool MultiPoint::getCenter3D ( double &xP, double &yP, double &zP )
{
  unsigned int i;
  bool         success;

  success = getCenter2D( xP, yP );
  if ( !success ) return false;

  if ( dim == 2 )
    zP = 0.0;
  else
  {
    zP = 0.0;

    for ( i = 0; i < position.size(); i++ )
    {
      GeoPoint pPoint = position[i];
      zP = zP + pPoint.getZ();
    }

    zP = zP / position.size();
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der Geometrie ein gültiges                   //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool MultiPoint::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    if ( position[i]->existReferenceSystem( pSrsManager ) == false )
      return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Geometrischen Offset auf die einzelnen Member                            //
///////////////////////////////////////////////////////////////////////////////
void MultiPoint::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  )
{
  for ( unsigned int i = 0; i < position.size(); i++ )
  {
    _Geometrie * pGeo = position[i];
    if ( sGeometries.count( pGeo ) == 0 )
    {
      pGeo->offset( point, sGeometries );
      sGeometries.insert ( pGeo );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Surface														                                			 //
//  Implementierung der Klasse gml:Surface								                	 //
///////////////////////////////////////////////////////////////////////////////
Surface::Surface( int dimP ): _Surface( dimP )
{
}

Surface::Surface ( Surface * pSurfaceOld ): _Surface ( pSurfaceOld )
{
	for ( unsigned int i = 0; i < patches.size(); i++ )
	{
		PolygonGeo * pPoly = pSurfaceOld->patches[i];
		PolygonGeo * pPolyCopy = new PolygonGeo ( pPoly );
		patches.push_back ( pPolyCopy );
	}
}

Surface:: ~Surface()
{
	for ( unsigned int i = 0; i < patches.size(); i++ )
		delete patches[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding-Box												                         //
///////////////////////////////////////////////////////////////////////////////
bool Surface::getBoundingBox ( double * sw, double * no )
{
	bool	success;
	double  swLoc[3], noLoc[3];
  
  if ( bbExist )
  {
    sw[0] = m_sw[0];
    sw[1] = m_sw[1];
    no[0] = m_no[0];
    no[1] = m_no[1];
    sw[2] = m_sw[2];
    no[2] = m_no[2];
    return true;
  }

	sw[0] =  1.0E20;
	sw[1] =  1.0E20;
	no[0] = -1.0E20;
	no[1] = -1.0E20;
  if ( dim == 3 )
  {
    sw[2] =  1.0E20;
    no[2] = -1.0E20;
  }
  else
  {
    sw[2] = 0.0;
    no[2] = 0.0;
  }

	if ( patches.size() == 0 ) return false;

	for ( unsigned int i = 0; i < patches.size(); i++ )
	{ 
		PolygonGeo * pSurface = patches[i];
		success = pSurface->getBoundingBox ( swLoc, noLoc );
		if ( !success ) continue;

		if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
		if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
		if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
		if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( dim == 3 )
    {
      if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
      if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
    }
	}

  m_sw[0] = sw[0];
  m_sw[1] = sw[1];
  m_no[0] = no[0];
  m_no[1] = no[1];
  if ( dim == 3 )
  {
    m_sw[2] = sw[2];
    m_no[2] = no[2];
  }
  else
  {
    m_sw[2] = 0.0;
    m_no[2] = 0.0;
  }
  bbExist = true;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den gemeinsamen 2D-Schwerpunkt der Patches	            				 //
///////////////////////////////////////////////////////////////////////////////
bool Surface::getCenter2D ( double &xP, double &yP )
{
	double        xLoc, yLoc;
	unsigned int  i;
	bool          success;

	if ( patches.size() == 0 ) return false;

	xP = 0.0;
	yP = 0.0;

	for ( i = 0; i < patches.size(); i++ )
	{
		PolygonGeo * pSurface = patches[i];
		success = pSurface->getCenter2D ( xLoc, yLoc );
		if ( !success ) return false;

		xP = xP + xLoc;
		yP = yP + yLoc;
	}

	xP = xP / patches.size();
	yP = yP / patches.size();

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den gemeinsamen 3D-Schwerpunkt der Patches	            				 //
///////////////////////////////////////////////////////////////////////////////
bool Surface::getCenter3D ( double &xP, double &yP, double &zP )
{
  double        xLoc, yLoc, zLoc;
  unsigned int  i;
  bool          success;

  success = getCenter2D( xP, yP );
  if ( !success ) return false;

  if ( dim == 2 )
    zP = 0.0;
  else
  {
    zP = 0.0;

    for ( i = 0; i < patches.size(); i++ )
    {
      PolygonGeo * pSurface = patches[i];
      success = pSurface->getCenter3D ( xLoc, yLoc, zLoc );
      if ( !success ) return false;

      zP = zP + zLoc;
    }

    zP = zP / patches.size();
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone                                          //
///////////////////////////////////////////////////////////////////////////////
size_t Surface::getPolygonAnz()
{
  return patches.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t Surface::getCurveSegmentAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < patches.size(); i++ )
  {
    _Geometrie * pGeo = patches[i];
    anz = anz + pGeo->getCurveSegmentAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t Surface::getPointAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < patches.size(); i++ )
  {
    _Geometrie * pGeo = patches[i];
    anz = anz + pGeo->getPointAnz();
  }
  return anz;
}


///////////////////////////////////////////////////////////////////////////////
// Liefert den gesamten Flächeninhalt inklusive der Löcher	        				 //
///////////////////////////////////////////////////////////////////////////////
double Surface::getFlaecheMitLoechern ()
{
  unsigned int i;
  double       flaeche = 0.0;

  for ( i = 0; i < patches.size(); i++ )
    flaeche = flaeche + patches[i]->getFlaecheMitLoechern();

  return flaeche;
}

///////////////////////////////////////////////////////////////////////////////
// Liefert den gesamten realen Flächeninhalt ohne Löcher			          		 //
///////////////////////////////////////////////////////////////////////////////

double Surface::getFlaeche()
{
  unsigned int i;
  double       flaeche = 0.0;

  for ( i = 0; i < patches.size(); i++ )
    flaeche = flaeche + patches[i]->getFlaeche();

  return flaeche;
}

///////////////////////////////////////////////////////////////////////////////
// Liefert die Polygone       											                      	 //
///////////////////////////////////////////////////////////////////////////////
size_t Surface::getPolygone ( vector<PolygonGeo*> &vPatches )
{
	for ( unsigned int i = 0; i < patches.size(); i++ )
		vPatches.push_back ( patches[i] );

	return vPatches.size();
}

size_t Surface::getPolygone ( vector<PolygonGeo*> &vPatches, vector<bool> &vOrientations )
{
  for ( unsigned int i = 0; i < patches.size(); i++ )
  {
    vPatches.push_back ( patches[i] );
    vOrientations.push_back( true );
  }

  return vPatches.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Polygon über den Index                                       //
///////////////////////////////////////////////////////////////////////////////
PolygonGeo * Surface::getPolygon ( unsigned int index )
{
  if ( index < 0 || index >= patches.size() )
    return NULL;
  else
    return patches[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Prüfung einer 2D-Fläche (Basic-Check)                                    //
///////////////////////////////////////////////////////////////////////////////
bool Surface::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  unsigned int    i;
  PolygonGeo       * pPatch;
  bool            okLoc;
  bool            ok = true;

  if ( patches.size() == 0 )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );
    pError->setFeatureGeometryId ( gmlId );
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  for ( i = 0; i < patches.size(); i++ )
  {
    pPatch = patches[i];
    okLoc = pPatch->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );
    if ( !okLoc )
      ok = false;
  }

  if ( ok )
    checkResult = BASIC_CHECK_FULFILLED;
  else
    checkResult = BASIC_CHECK_FAILED;

  return ok;
}

///////////////////////////////////////////////////////////////////////////////
//  Prüfung einer 2D-Fläche (Advanced-Check)                                 //
///////////////////////////////////////////////////////////////////////////////
bool Surface::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  unsigned int    i;
  PolygonGeo       * pPatch;
  bool            okLoc;
  bool            ok = true;

  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;

  for ( i = 0; i < patches.size(); i++ )
  {
    pPatch = patches[i];
    okLoc = pPatch->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
    if ( !okLoc )
      ok = false;
  }

  if ( ok )
    checkResult = ADVANCED_CHECK_FULFILLED;
  else
    checkResult = ADVANCED_CHECK_FAILED;

  return ok;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert die Patches in ein Polygon mit Inselflächen, wenn die      //
//  Topologie der Patches stimmt                                             //
///////////////////////////////////////////////////////////////////////////////
bool Surface::transformToPolygon2D ( double minDist, double minAngleDist )
{

  PatchLine *          patchLine;
  PatchLine *          patchLineA;
  vector<PatchLine*>   patchLines;
  vector<PatchLine*>   outerLines;
  vector<LinearRing*>  rings;
  _Ring              * pLoch;
  _Ring              * pLochCopy;
  LinearRing         * pRing = nullptr;
  LinearRing         * pRingA;
  LinearRing         * pRingB;
  vector<double>       x, y;
  PolygonGeo            * pPatch;
	size_t               i, j, anz, ringNr, outerRingIndex;
  double               dist1, dist2;
  double               paRing[2], peRing[2];
  bool                 found, finish, outerRing;
  int                  loc;
  PolygonGeo            * pPolygon = NULL;

  if ( patches.size() == 1 )
    return true;

  for ( i = 0; i < patches.size(); i++ )
  {
    x.clear();
    y.clear();
    pPatch = patches[i];
    anz = pPatch->getAussenkontur()->getPoints2D( x, y );
    for ( j = 0; j < anz - 1; j++ )
    {
      patchLine = new PatchLine;

      patchLine->typ = -1;
      patchLine->pa[0] = x[j];
      patchLine->pa[1] = y[j];
      patchLine->pe[0] = x[j+1];
      patchLine->pe[1] = y[j+1];
      patchLines.push_back( patchLine );
    }
  }

  for ( i = 0; i < patchLines.size(); i++ )
  {
    patchLine = patchLines[i];
    if ( patchLine->typ != -1 ) continue;

    for ( j = i+1; j < patchLines.size(); j++ )
    {
      patchLineA = patchLines[j];
      if ( patchLineA->typ != -1 ) continue;

      dist1 = sqrt ( (patchLine->pa[0] -  patchLineA->pe[0])*(patchLine->pa[0] -  patchLineA->pe[0]) + 
                     (patchLine->pa[1] -  patchLineA->pe[1])*(patchLine->pa[1] -  patchLineA->pe[1]) );
      dist2 = sqrt ( (patchLine->pe[0] -  patchLineA->pa[0])*(patchLine->pe[0] -  patchLineA->pa[0]) + 
                     (patchLine->pe[1] -  patchLineA->pa[1])*(patchLine->pe[1] -  patchLineA->pa[1]) );

      if ( dist1 < minDist && dist2 < minDist )
      {
        patchLine->typ = 0;
        patchLineA->typ = 0;
        break;
      }
    }
  }

  ringNr = 1;
  while ( true )
  {
    found = false;
    for ( i = 0; i < patchLines.size(); i++ )
    {
      patchLine = patchLines[i];
      if ( patchLine->typ == -1 )
      {
        found = true;
        patchLine->typ = ringNr;
        paRing[0] = patchLine->pa[0];
        paRing[1] = patchLine->pa[1];
        peRing[0] = patchLine->pe[0];
        peRing[1] = patchLine->pe[1];
        outerLines.push_back ( patchLine );
        break;
      }
    }

    if ( !found ) break;

    while ( true )
    {
      found  = false;
      finish = false;
      for ( i = 0; i < patchLines.size(); i++ )
      {
        patchLine = patchLines[i];
        if ( patchLine->typ == -1 )
        {
          dist1 = sqrt ( (patchLine->pa[0] - peRing[0])*(patchLine->pa[0] - peRing[0]) + 
                         (patchLine->pa[1] - peRing[1])*(patchLine->pa[1] - peRing[1]) );
          if ( dist1 < minDist )
          {
            found = true;
            patchLine->typ = ringNr;
            peRing[0]      = patchLine->pe[0];
            peRing[1]      = patchLine->pe[1];
            outerLines.push_back ( patchLine );

            dist2 = sqrt ( (patchLine->pe[0] -  paRing[0])*(patchLine->pe[0] - paRing[0]) + 
                           (patchLine->pe[1] -  paRing[1])*(patchLine->pe[1] - paRing[1]) );
            if ( dist2 < minDist )
                finish = true;
              break;
            }
          }
        }

        if ( finish == true || found == false )
        {
          ringNr++;
          break;
        }
      }
  }

  if ( outerLines.size() > 0 )
  {
    x.clear();
    y.clear();
    pRing = new LinearRing ( 2 );
    ringNr = 1;
  }


  for ( i = 0; i < outerLines.size(); i++ )
  {
    patchLine = outerLines[i];
    if ( patchLine->typ == ringNr )
    {
      x.push_back( patchLine->pa[0] );
      y.push_back( patchLine->pa[1] );
      if ( i == outerLines.size()-1 )
      {
        x.push_back( patchLine->pe[0] );
        y.push_back( patchLine->pe[1] );
        pRing->addPoints2D( x, y );
        rings.push_back( pRing );
      }

    }
    else
    {
      x.push_back( outerLines[i-1]->pe[0] );
      y.push_back( outerLines[i-1]->pe[1] );

      pRing->addPoints2D( x, y );
      rings.push_back( pRing );

      x.clear();
      y.clear();
      x.push_back( patchLine->pa[0] );
      y.push_back( patchLine->pa[1] );
      pRing = new LinearRing ( 2 );
      ringNr++;
    }
  }

  for ( i = 0; i < patchLines.size(); i++ )
    delete patchLines[i];

  if ( rings.size() == 1 )
  {
    pPolygon = new PolygonGeo ( 2 );
    pPolygon->setAussenkontur( rings[0] );
  }
  else
  {
    outerRingIndex = -1;
    for ( i = 0; i < rings.size(); i++ )
    {
      pRingA = rings[i];
      outerRing = true;
      for ( j = 0; j < rings.size(); j++ )
      {
        if ( j == i ) continue;
        pRingB = rings[j];
        loc = pRingA->checkTopology2D( pRingB, minDist, minAngleDist );
        if ( loc != INNEN )
        {
          outerRing = false;
          break;
        }
      }
      if ( outerRing )
      {
        outerRingIndex = i;
        break;
      }
    }

    if ( outerRingIndex >= 0 ) 
    {
      pPolygon = new PolygonGeo ( 2 );
      pPolygon->setAussenkontur( rings[outerRingIndex] );
      for ( i = 0; i < rings.size(); i++ )
        if ( i != outerRingIndex )
          pPolygon->addLoch( rings[i] );
    }
  }

  if ( pPolygon != NULL )
  {
    for ( i = 0; i < patches.size(); i++ )
    {
      pPatch = patches[i];
      for ( j = 0; j = pPatch->getLochAnz(); j++ )
      {
        pLoch = pPatch->getLoch( j );
      
        if ( pLoch->getRingType() == LINEAR_RING )
          pLochCopy = new LinearRing ( dynamic_cast<LinearRing*>(pLoch) );
        else
          pLochCopy = new Ring ( dynamic_cast<Ring*>(pLoch) );
        pPolygon->addLoch( pLochCopy );      
      }
      delete pPatch;
    }
    patches.clear();
    patches.push_back( pPolygon );
    return true;
  }
  else
  {
    for ( i = 0; i < rings.size(); i++ )
      delete rings[i];
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der Geometrie ein gültiges                   //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool Surface::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  for ( unsigned int i = 0; i < patches.size(); i++ )
  {
    if ( patches[i]->existReferenceSystem( pSrsManager ) == false )
      return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine globale Angabe über die Orientierung der Multifläche        //
///////////////////////////////////////////////////////////////////////////////
SURFACE_NORMAL_TYPE Surface::getSurfaceNormal( double angleTolenance, double areaToleranceRelative )
{
  vector<PolygonGeo*>         vPolygone;
  vector<bool>             vOrientations;
  PolygonGeo                * pPoly;
  bool                     ori;

	size_t     i, anz;
	size_t     anzVerticalUpward    = 0; 
	size_t     anzUpward            = 0; 
	size_t     anzHorizontal        = 0; 
	size_t     anzDownward          = 0; 
	size_t     anzVerticalDownward  = 0; 
  double     areaVerticalUpward   = 0.0;
  double     areaUpward           = 0.0;
  double     areaHorizontal       = 0.0;
  double     areaDownward         = 0.0;
  double     areaVerticalDownward = 0.0;
  double     areaGesamt           = 0.0;
  double     area;
  double     areaTolerance;

  anz = getPolygone( vPolygone, vOrientations );
  if ( anz == 0 )
    return INDIFFERENT;

  for ( i = 0; i < anz; i++  )
  {
    pPoly = vPolygone[i];
    ori   = vOrientations[i];

    SURFACE_NORMAL_TYPE surfaceOri = pPoly->getSurfaceNormal( areaToleranceRelative, 0.0 );
    area = fabs ( pPoly->getFlaeche() );
    areaGesamt = areaGesamt + area;

    switch ( surfaceOri )
    {
    case VERTICAL_UPWARD:
      if ( ori )
      {
        anzVerticalUpward++;
        areaVerticalUpward = areaVerticalUpward + area;
      }
      else
      {
        anzVerticalDownward++;
        areaVerticalDownward = areaVerticalDownward + area;
      }
      break;

    case UPWARD:
      if ( ori )
      {
        anzUpward++;
        areaUpward = areaUpward + area;
      }
      else
      {
        anzDownward++;
        areaDownward = areaDownward + area;
      }
      break;

    case HORIZONTAL:
      anzHorizontal++;
      areaHorizontal = areaHorizontal + area;
      break;

    case DOWNWARD:
      if ( !ori )
      {
        anzUpward++;
        areaUpward = areaUpward + area;
      }
      else
      {
        anzDownward++;
        areaDownward = areaDownward + area;
      }
      break;

      if ( ori )
      {
        anzVerticalUpward++;
        areaVerticalUpward = areaVerticalUpward + area;
      }
      else
      {
        anzVerticalDownward++;
        areaVerticalDownward = areaVerticalDownward + area;
      }
      break;
    }
  }

  areaTolerance = areaGesamt*( 1.0 - areaToleranceRelative );

  if ( areaVerticalUpward > areaTolerance  )
    return VERTICAL_UPWARD;

  if ( areaVerticalDownward > areaTolerance  )
    return VERTICAL_DOWNWARD;

  if ( areaVerticalUpward + areaUpward > areaTolerance )
    return UPWARD;

  if ( areaVerticalDownward + areaDownward > areaTolerance )
    return DOWNWARD;

  if ( areaHorizontal > areaTolerance )
    return HORIZONTAL;

  return INDIFFERENT;    
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt die Surface um einen Offset                                   //
///////////////////////////////////////////////////////////////////////////////
void Surface::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  _Surface::offset( point, sGeometries );

  for ( unsigned int i = 0; i < patches.size(); i++ )
  {
    PolygonGeo * pGeo = patches[i];
    if ( sGeometries.count( pGeo ) == 0 ) 
    {
      pGeo->offset( point, sGeometries );
    }
  }
  sGeometries.insert( this );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt ein Polygon auf Basis der gml:id                                //
///////////////////////////////////////////////////////////////////////////////
_Surface * Surface::removePolygon( std::string gmlIdP )
{  
  for ( unsigned int i = 0; i < patches.size(); i++ )
  {
    PolygonGeo * pSurface     = patches[i];
    if ( pSurface->getGmlId() == gmlIdP )
    {
      PolygonGeo * pPolyRemoved = pSurface;
      for ( unsigned int j = i + 1; j < patches.size(); ++j )
        patches[j-1] = patches[j];
      patches.pop_back();
      return pPolyRemoved;
    }   
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Implementierung der Klasse gml:Polygon								                	 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor												                         //
///////////////////////////////////////////////////////////////////////////////
PolygonGeo::PolygonGeo( int dimP ): _Surface( dimP )
{
	aussenkontur = NULL;
}

PolygonGeo::PolygonGeo( PolygonGeo * pPolyOld ): _Surface ( pPolyOld )
{
	unsigned int i;
	_Ring      * pLoch;
	_Ring      * pLochNew;

	if ( pPolyOld->aussenkontur != NULL )
	{
		switch ( pPolyOld->aussenkontur->getRingType() )
		{
		case LINEAR_RING:
			aussenkontur = new LinearRing ( (LinearRing*)pPolyOld->aussenkontur );
			break;
		case RING:
			aussenkontur = new Ring ( (Ring*)pPolyOld->aussenkontur );
			break;
		}
	}
	else
		aussenkontur = NULL;

	for ( i = 0; i < pPolyOld->loecher.size(); i++ )
	{
		pLoch = pPolyOld->loecher[i];
		
		switch ( pLoch->getRingType() )
		{
		case LINEAR_RING:
			pLochNew = new LinearRing ( (LinearRing*)pLoch );
			break;

		case RING:
			pLochNew = new Ring ( (Ring*)pLoch );
			break;
		}
		loecher.push_back ( pLochNew );
	}

}

PolygonGeo::~PolygonGeo()
{
	unsigned int i;

	if ( aussenkontur != NULL ) delete aussenkontur;
	for ( i = 0; i < loecher.size(); i++ ) delete loecher[i];
}

///////////////////////////////////////////////////////////////////////////////////////////
//  Generiert ein Objekt PolygonGeo aus einem Shapefile-Feature mit einer Außenkontur    //
///////////////////////////////////////////////////////////////////////////////////////////
void PolygonGeo::addShapePolygons2D ( std::vector<double> xCoordP, std::vector<double> yCoordP, 
                  std::vector<int> ringOffsetsP, std::vector<int> ringTypenP, Features * pFeatures )
{
	size_t           i, j, polygonAnz, indexStart, ringPunktAnz, indexPolygon;
  int              ringTyp;
  vector<double>   vX;
  vector<double>   vY;
  LinearRing	   * pRing;
  LineSegment    * pLineSegment;

  //  Feststellen der Anzahl der Aussenkonturen
  polygonAnz = 0;
  for ( i = 0; i < ringTypenP.size(); i++ )
  {
    ringTyp = ringTypenP[i];
    if ( ringTyp > 0 ) polygonAnz++;
  }
  if ( polygonAnz != 1) return;

  //  Anlegen der Aussenkonturen
  for ( i = 0; i < ringOffsetsP.size(); i++)
  {
    ringTyp = ringTypenP[i];
    if ( ringTyp > 0)
    {
      indexPolygon = ringTyp-1;
      indexStart = ringOffsetsP[i];
      if ( i  == ringOffsetsP.size()-1) ringPunktAnz = xCoordP.size()-indexStart;
      else                              ringPunktAnz = ringOffsetsP[i+1]-ringOffsetsP[i];//+1?

      vX.clear();
      vY.clear();
      for ( j = indexStart; j < indexStart + ringPunktAnz; j++ )
      {
        vX.push_back ( xCoordP[j] );
        vY.push_back ( yCoordP[j] );
      }
      pLineSegment = new LineSegment ( dim, LineSegment::_LINEAR_RING );
      pLineSegment->addPoints2D ( vX, vY );

      aussenkontur = new LinearRing ( dim );
      ((LinearRing*)aussenkontur)->setLineSegment ( pLineSegment );

      string gmlId =  Constant::createGmlId();
      aussenkontur->setGmlId ( gmlId );
      pFeatures->addGeometry( aussenkontur );

      aussenkontur->setSrsName ( srsName );
      break;
    }
  }

  //  Anlegen der Löcher
  for ( i = 0; i < ringOffsetsP.size(); i++)
  {
    ringTyp = ringTypenP[i];
    if ( ringTyp < 0)
    {
      indexPolygon = -ringTyp - 1;
      indexStart = ringOffsetsP[i];
      if ( i  == ringOffsetsP.size()-1) ringPunktAnz = xCoordP.size()-indexStart;
      else                              ringPunktAnz = ringOffsetsP[i+1]-ringOffsetsP[i];//+1?

      vX.clear();
      vY.clear();
      for ( j = indexStart; j < indexStart + ringPunktAnz; j++ )
      {
        vX.push_back ( xCoordP[j] );
        vY.push_back ( yCoordP[j] );
      }
      pLineSegment = new LineSegment ( dim, LineSegment::_LINEAR_RING );
      pLineSegment->addPoints2D ( vX, vY );

      pRing = new LinearRing ( dim );
      pRing->setLineSegment ( pLineSegment );

      addLoch ( pRing );
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob der Ring in Innern des Polygons liegt                	  	 //
///////////////////////////////////////////////////////////////////////////////
bool PolygonGeo::isHole2D ( _Ring * pRingP, double minDist, double minAngleDist )
{
  int areaLoc;

  areaLoc = checkTopology2D( aussenkontur, minDist, minAngleDist );

  if ( areaLoc == INNEN )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Überlappungsfläche zweier Polygone   						           //
///////////////////////////////////////////////////////////////////////////////
bool PolygonGeo::getUeberlappungsflaeche2D ( PolygonGeo * pPolygon, double minDist, double minAngleDist,  MultiSurface * pUeberlappungsFlaeche )
{
  IntersectionGeometry intersection;
  vector<_Ring*>           vHoles;
  vector<_Ring*>           vVerschneidungsRings;
  vector<PolygonGeo*>      vPolygons;
  unsigned int             i, j;
  int                      loc;
  bool                     success;
  bool                     ueberlappung;
  _Ring                  * pRingReverse;
  _Ring                  * pHole;

  success = aussenkontur->cut2D( pPolygon->getAussenkontur(), minDist, intersection );
  if ( !success ) 
    return false;
  if ( intersection.vCutRings.size() == 0 ) 
    return true;

  for ( i = 0; i < getLochAnz(); i++ )
    vHoles.push_back( getLoch( i ) );

  for ( i = 0; i < pPolygon->getLochAnz(); i++ )
    vHoles.push_back( pPolygon->getLoch( i ) );


  for ( i = 0; i < intersection.vCutRings.size(); i++ )
  {
    PolygonGeo * pCutPolygon = new PolygonGeo ( 2 );
    pCutPolygon->setAussenkontur( intersection.vCutRings[i] );
    vPolygons.push_back( pCutPolygon );
  }

  for ( i = 0; i < vPolygons.size(); i++ )
  {
    pPolygon = vPolygons[i];
    ueberlappung = true;
    vVerschneidungsRings.clear();
    for ( j = 0; j < vHoles.size(); j++  )
    {
      pRingReverse = vHoles[j];
      pRingReverse->reverse();
      loc = pRingReverse->checkTopology2D( pPolygon->getAussenkontur(), minDist, minAngleDist );
      switch ( loc )
      {
      case AUSSEN:
        pHole = vHoles[j]->copyRing();
        pPolygon->addLoch( pHole );
        break;

      case INNEN:
      case INNEN_BERUEHREND_SINGLE_POINT:
      case INNEN_BERUEHREND_MULTI_POINT:
      case GLEICH:
        ueberlappung = false;
        break;

      case AUSSEN_BERUEHREND_SINGLE_POINT:
      case AUSSEN_BERUEHREND_MULTI_POINT:
      case UEBERLAPPEND:
        vVerschneidungsRings.push_back( pRingReverse );
        break;
      }
      pRingReverse->reverse();
    }

    if ( ueberlappung )
    {
      if ( vVerschneidungsRings.size() > 0 )
        pPolygon->getDifferenzflaecheUeberlappend2D ( vVerschneidungsRings, minDist, minAngleDist, pUeberlappungsFlaeche );  
      else
      {
        pUeberlappungsFlaeche->addSurface( new PolygonGeo ( pPolygon ) );
      } 
    }
  }

  for ( i = 0; i < vPolygons.size(); i++ )
    delete vPolygons[i];

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Differenzfläche eines Polygons mit einem Ring               //
//  Es wird vorausgesetzt, dass der Ring disjunkt ist von den                //
//  Inselflächen des Polygons                                                //
///////////////////////////////////////////////////////////////////////////////
bool PolygonGeo::getDifferenzflaeche2D ( _Ring * pRing, double minDist, double minAngleDist, MultiSurface * pUeberlappungsflaeche )
{
  int            areaLocation;
  _Ring        * pCopyRing;
  PolygonGeo      * pPolygon;
  vector<_Ring*> vRinge;

  areaLocation = aussenkontur->checkTopology2D( pRing, minDist, minAngleDist );

  switch ( areaLocation )
  {
  case DISJUNKT:
  case DISJUNKT_BERUEHREND_SINGLE_POINT:
  case DISJUNKT_BERUEHREND_MULTI_POINT:
    pPolygon = new PolygonGeo ( this );
    pUeberlappungsflaeche->addSurface( pPolygon );
    return true;

  case INNEN:
    pPolygon = new PolygonGeo ( this );
    pCopyRing = pRing->copyRing();
    pPolygon->addLoch( pCopyRing );
    pUeberlappungsflaeche->addSurface( pPolygon );
    return true;

  case AUSSEN:
  case AUSSEN_BERUEHREND_SINGLE_POINT:
  case AUSSEN_BERUEHREND_MULTI_POINT:
  case GLEICH:
    return true;

  case INNEN_BERUEHREND_SINGLE_POINT:
  case INNEN_BERUEHREND_MULTI_POINT:
  case UEBERLAPPEND:
    vRinge.push_back( pRing );
    return getDifferenzflaecheUeberlappend2D ( vRinge, minDist, minAngleDist, pUeberlappungsflaeche );   
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Hilfsroutine in getDifferenzflaeche2D                                    //
///////////////////////////////////////////////////////////////////////////////
bool PolygonGeo::getDifferenzflaecheUeberlappend2D ( vector<_Ring*> vRinge, double minDist, double minAngleDist, MultiSurface * pUeberlappungsflaeche )
{
  unsigned int   i, j;
  vector<LinearRing*> vResultRings;
  vector<_Ring*> vTestRings;
  _Ring        * testRing;
  PolygonGeo      * pPolygon;
  LinearRing   * pLinearRing;
  Ring         * pRing;

  vTestRings.push_back( aussenkontur->copyRing() );
  for ( j = 0; j < vRinge.size(); j++ )
  {
    for ( i = 0; i < vTestRings.size(); i++ )
    {
      testRing = vTestRings[i];
      testRing->difference2D( vRinge[j], minDist, vResultRings );
    }
    for ( i = 0; i < vTestRings.size(); i++ )
      delete vTestRings[i];
    vTestRings.clear();
    for ( i = 0; i < vResultRings.size(); i++ )
      vTestRings.push_back( vResultRings[i] );
    vResultRings.clear();
  }

  for ( i = 0; i < vTestRings.size(); i++ )
  {
    switch (vTestRings[i]->getRingType() )
    {
    case LINEAR_RING:
      pLinearRing = (LinearRing*)vTestRings[i];
      break;

    case RING:
      pRing = (Ring*)vTestRings[i];
      break;
    }

    pPolygon = new PolygonGeo ( 2 );
    pPolygon->setAussenkontur( vTestRings[i] );
    for ( j = 0; j < loecher.size(); j++ )
    {
      if ( pPolygon->isHole2D( loecher[j], minDist, minAngleDist ) )
        pPolygon->addLoch( loecher[j]->copyRing() );
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die topolgische Beziehung zweier 2D Polygone  				           //
///////////////////////////////////////////////////////////////////////////////
int PolygonGeo::checkTopology2D ( PolygonGeo * pPolygon, double minDist, double minAngleDist )
{
	int          loc;
	size_t       locLoch;
	double       sw[3], no[3];
	double       swLoc[3], noLoc[3];
	size_t       iLoch, jLoch, lochAnz;
	bool	       gefunden, beruehrend;
	_Ring      * pLoch;
	_Ring      * pLochPoly;
	_Ring      * pAussenkonturTest;

	pAussenkonturTest = pPolygon->getAussenkontur();
	if ( aussenkontur == NULL || pAussenkonturTest == NULL ) return DISJUNKT;

	getBoundingBox ( sw, no );
	pPolygon->getBoundingBox ( swLoc, noLoc );

	if ( noLoc[0] < sw[0] - minDist || swLoc[0] > no[0] + minDist ||
		 noLoc[1] < sw[1] - minDist || swLoc[1] > no[1] + minDist )
		 return DISJUNKT;

	loc = aussenkontur->checkTopology2D ( pAussenkonturTest, minDist, minAngleDist );

	if (  loc == DISJUNKT ||  loc == DISJUNKT_BERUEHREND_SINGLE_POINT ||  loc == DISJUNKT_BERUEHREND_MULTI_POINT ||  loc == UEBERLAPPEND )
		return loc;
	else
	if ( loc == INNEN || loc == INNEN_BERUEHREND_SINGLE_POINT || loc == INNEN_BERUEHREND_MULTI_POINT )
	{
		lochAnz = loecher.size();
		if ( lochAnz == 0 ) return loc;
		beruehrend = false;

		for ( iLoch = 0; iLoch < lochAnz; iLoch++ )
		{
			pLoch = loecher[iLoch];
			locLoch = pLoch->checkTopology2D ( pAussenkonturTest, minDist, minAngleDist );
			switch ( locLoch  )
			{
			case DISJUNKT:			
				break;

			case UEBERLAPPEND:
				return UEBERLAPPEND;

			case DISJUNKT_BERUEHREND_SINGLE_POINT:
        if ( loc == INNEN_BERUEHREND_SINGLE_POINT )
          loc = INNEN_BERUEHREND_MULTI_POINT;

        if ( loc == INNEN )
          loc = INNEN_BERUEHREND_SINGLE_POINT;
        break;
       
      case DISJUNKT_BERUEHREND_MULTI_POINT:
        loc = INNEN_BERUEHREND_MULTI_POINT;
				break;

			case AUSSEN_BERUEHREND_SINGLE_POINT:
      case AUSSEN_BERUEHREND_MULTI_POINT:
      case AUSSEN:
				return UEBERLAPPEND;

			case INNEN_BERUEHREND_SINGLE_POINT:
				return DISJUNKT_BERUEHREND_SINGLE_POINT;
				break;

      case INNEN_BERUEHREND_MULTI_POINT:
        return DISJUNKT_BERUEHREND_MULTI_POINT;
        break;

			case INNEN:
				return DISJUNKT;
				break;

			case GLEICH:
				return DISJUNKT_BERUEHREND_MULTI_POINT;
				break;
			}
		}
		return loc;	
	}
	else
	if ( loc == AUSSEN || loc == AUSSEN_BERUEHREND_SINGLE_POINT || loc == AUSSEN_BERUEHREND_MULTI_POINT )
	{
		lochAnz = pPolygon->getLochAnz();
		if ( lochAnz == 0 ) return loc;
		beruehrend = false;

		for ( iLoch = 0; iLoch < lochAnz; iLoch++ )
		{
			pLoch = pPolygon->getLoch ( iLoch );
			locLoch = pLoch->checkTopology2D ( aussenkontur, minDist, minAngleDist );
			switch ( locLoch  )
			{
			case DISJUNKT:			
				break;

			case UEBERLAPPEND:
				return UEBERLAPPEND;

			case DISJUNKT_BERUEHREND_SINGLE_POINT:
        if ( loc  == AUSSEN_BERUEHREND_SINGLE_POINT )
          loc = AUSSEN_BERUEHREND_MULTI_POINT;

        if ( loc == AUSSEN )
          loc = AUSSEN_BERUEHREND_SINGLE_POINT;
				break;

      case DISJUNKT_BERUEHREND_MULTI_POINT:
        loc = AUSSEN_BERUEHREND_MULTI_POINT;
        break;

			case AUSSEN_BERUEHREND_MULTI_POINT:
      case AUSSEN_BERUEHREND_SINGLE_POINT:
      case AUSSEN:
				return UEBERLAPPEND;

			case INNEN_BERUEHREND_SINGLE_POINT:
				return DISJUNKT_BERUEHREND_SINGLE_POINT;
				break;

      case INNEN_BERUEHREND_MULTI_POINT:
        return DISJUNKT_BERUEHREND_MULTI_POINT;
        break;

			case INNEN:
				return DISJUNKT;
				break;

			case GLEICH:
				return DISJUNKT_BERUEHREND_MULTI_POINT;
				break;
			}
		}
	    return loc;			
	}	
	else
	if ( loc == GLEICH )
	{
		if ( pPolygon->getLochAnz() != getLochAnz() )
			return UEBERLAPPEND;

		if ( pPolygon->getLochAnz() == 0 )
			return GLEICH;

		for ( iLoch = 0; iLoch < getLochAnz(); iLoch++ )
		{
			pLoch = getLoch ( iLoch );
			gefunden = false;
			for ( jLoch = 0; jLoch < pPolygon->getLochAnz(); jLoch++ )
			{
				pLochPoly = pPolygon->getLoch ( jLoch );
				loc = pLoch->checkTopology2D ( pLochPoly, minDist, minAngleDist );
				if ( loc == GLEICH )
				{
					gefunden = true;
					break;
				}
			}
			if ( gefunden == false )
				return UEBERLAPPEND;
		}
		return GLEICH;
	}

  return GLEICH;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die topolgische Beziehung eines 2D Polygons mit einem 2D Ring   //
///////////////////////////////////////////////////////////////////////////////
int PolygonGeo::checkTopology2D ( _Ring * pRing, double minDist, double minAngleDist )
{
	int           loc;
	size_t        locLoch;
  double        sw[3], no[3];
  double        swLoc[3], noLoc[3];
	size_t        iLoch, lochAnz;
  bool	        beruehrend;
  _Ring       * pLoch;

  if ( aussenkontur == NULL || pRing == NULL ) return DISJUNKT;

  getBoundingBox ( sw, no );
  pRing->getBoundingBox ( swLoc, noLoc );

  if ( noLoc[0] < sw[0] - minDist || swLoc[0] > no[0] + minDist ||
    noLoc[1] < sw[1] - minDist || swLoc[1] > no[1] + minDist )
    return DISJUNKT;

  loc = aussenkontur->checkTopology2D ( pRing, minDist, minAngleDist );

  if (  loc == DISJUNKT ||  loc == DISJUNKT_BERUEHREND_SINGLE_POINT || loc == DISJUNKT_BERUEHREND_MULTI_POINT || loc == UEBERLAPPEND ||
        loc == AUSSEN || loc == AUSSEN_BERUEHREND_SINGLE_POINT || loc == AUSSEN_BERUEHREND_MULTI_POINT )
    return loc;
  else
  if ( loc == INNEN || loc == INNEN_BERUEHREND_SINGLE_POINT || loc == INNEN_BERUEHREND_MULTI_POINT )
  {
    lochAnz = loecher.size();
    if ( lochAnz == 0 ) return loc;
    beruehrend = false;

    for ( iLoch = 0; iLoch < lochAnz; iLoch++ )
    {
      pLoch = loecher[iLoch];
      locLoch = pLoch->checkTopology2D ( pRing, minDist, minAngleDist );
      switch ( locLoch  )
      {
      case DISJUNKT:			
        break;

      case UEBERLAPPEND:
        return UEBERLAPPEND;

      case DISJUNKT_BERUEHREND_SINGLE_POINT:
        if ( loc == INNEN_BERUEHREND_SINGLE_POINT )
          loc = INNEN_BERUEHREND_MULTI_POINT;

        if ( loc == INNEN )
          loc = INNEN_BERUEHREND_SINGLE_POINT;
        break;

      case DISJUNKT_BERUEHREND_MULTI_POINT:
        loc = INNEN_BERUEHREND_MULTI_POINT;
        break;

      case AUSSEN_BERUEHREND_SINGLE_POINT:
      case AUSSEN_BERUEHREND_MULTI_POINT:
      case AUSSEN:
        return UEBERLAPPEND;

      case INNEN_BERUEHREND_SINGLE_POINT:
        return DISJUNKT_BERUEHREND_SINGLE_POINT;
        break;

      case INNEN_BERUEHREND_MULTI_POINT:
        return DISJUNKT_BERUEHREND_MULTI_POINT;
        break;

      case INNEN:
        return DISJUNKT;
        break;

      case GLEICH:
        return DISJUNKT_BERUEHREND_MULTI_POINT;
        break;
      }
    }
    return loc;	
  }
  else
  if ( loc == GLEICH )
  {
    if ( getLochAnz() == 0 )
      return GLEICH;
    else
      return UEBERLAPPEND;
  }

  return UEBERLAPPEND;
}

///////////////////////////////////////////////////////////////////////////////
//  Umkehrung der Orientierung eines Polygons                                //
///////////////////////////////////////////////////////////////////////////////
void PolygonGeo::reverse()
{
   unsigned int i;

  if ( aussenkontur == NULL )
    return;

  aussenkontur->reverse();
  for ( i = 0; i < loecher.size(); i++ )
    loecher[i]->reverse();

  for ( i = 0; i < vTriangles.size(); i++ )
    vTriangles[i]->reverse();

  if ( pBRep != NULL )
  {
    delete pBRep;
    pBRep = NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
// Bestimmt die topologische Beziehung zweier Polygone, wobei nur            //
// die Außenkontur berücksichtigt wird                                       //
///////////////////////////////////////////////////////////////////////////////
POLYGON_TOPOLOGY PolygonGeo::intersect ( PolygonGeo *pPolygon, double minDist, double minAngleDist, bool ignoreHoles, IntersectionGeometry & intersection )
{
  double          angleDist, pointDist;   
  double          sw[3], no[3];
  double          swLoc[3], noLoc[3];
  Plane         * e1 = getPlane();
  Plane         * e2 = pPolygon->getPlane();
  POLYGON_TOPOLOGY result;

  if ( e1 == NULL || e2 == NULL )
    return POLYGONE_DISJUNKT;

  getBoundingBox ( sw, no );
  pPolygon->getBoundingBox ( swLoc, noLoc );

  if ( noLoc[0] < sw[0] - minDist || swLoc[0] > no[0] + minDist ||
    noLoc[1] < sw[1] - minDist || swLoc[1] > no[1] + minDist ||
    noLoc[2] < sw[2] - minDist || swLoc[2] > no[2] + minDist   )
    return POLYGONE_DISJUNKT;

  e1->getDifference( e2, angleDist, pointDist );
  if ( angleDist < minAngleDist  || fabs ( angleDist - PI ) < minAngleDist  )
  {
    if ( pointDist < minDist)
    {
      if ( fabs ( angleDist - PI ) < minAngleDist  )
      {
        pPolygon->reverse();
        result = intersect2D ( pPolygon, e1, minDist, ignoreHoles, intersection );
        pPolygon->reverse();
      }
      else
        result = intersect2D ( pPolygon, e1, minDist, ignoreHoles, intersection );
    }
    else
      result = POLYGONE_DISJUNKT;
  }
  else
  {
    result = intersect3D ( pPolygon, e1, e2, minDist, minAngleDist, ignoreHoles, intersection );
  }

  return result;
}

///////////////////////////////////////////////////////////////////////////////
// Bestimmt die topologische Beziehung zweier 2D Polygone                    //
///////////////////////////////////////////////////////////////////////////////
POLYGON_TOPOLOGY PolygonGeo::intersect2D ( PolygonGeo *pPolygon, Plane * e, double minDist, bool ignoreHoles, IntersectionGeometry & intersection )
{
  vector<_Ring*>       vContours;
  vector<_Ring*>       vContoursLoc;
  unsigned int         i, j;
  POLYGON_TOPOLOGY     polygonTopology = POLYGONE_DISJUNKT;
  POLYGON_TOPOLOGY     polygonTopologyLoc;
  _Ring              * pRing2D;
  _Ring              * pRing2DLoc;
  set<_Ring*>          sRings;
	vector<GeoPoint>     vPoints;
	vector<GeoPoint>     vPointsLoc;

  IntersectionGeometry intersectionLoc;

  if ( ignoreHoles || ( getLochAnz() == 0 && pPolygon->getLochAnz() == 0 ) )
  {
    pRing2D    = getAussenkontur()->copyRing();
    pRing2D->transform( e->getTransformationToHorizontal() );

    pRing2DLoc =  pPolygon->getAussenkontur()->copyRing();
    pRing2DLoc->transform( e->getTransformationToHorizontal() );

		pRing2D->getPoints(vPoints);
		pRing2DLoc->getPoints(vPointsLoc);

    polygonTopology = pRing2D->intersect2D( pRing2DLoc, minDist, intersectionLoc );
    intersectionLoc.transformInvers (  e->getTransformationToHorizontal() );
    intersection.copy( intersectionLoc );
    
    delete pRing2D;
    delete pRing2DLoc;

    return polygonTopology;
  }

  if ( getLochAnz() > 0 )
  {
    triangulate();
    for ( i = 0; i < vTriangles.size(); i++ )
    {
      pRing2D    = vTriangles[i]->getAussenkontur()->copyRing();
      pRing2D->transform( e->getTransformationToHorizontal() );
      vContours.push_back( pRing2D );
    }
  }
  else
  {
    pRing2D    = getAussenkontur()->copyRing();
    pRing2D->transform( e->getTransformationToHorizontal() );
    vContours.push_back( pRing2D );
  }

  if ( pPolygon->getLochAnz() > 0 )
  {
    pPolygon->triangulate();
    for ( i = 0; i < pPolygon->vTriangles.size(); i++ )
    {
      pRing2D    = pPolygon->vTriangles[i]->getAussenkontur()->copyRing();
      pRing2D->transform( e->getTransformationToHorizontal() );

      vContoursLoc.push_back( pRing2D );
    }
  }
  else
  {
    pRing2DLoc =  pPolygon->getAussenkontur()->copyRing();
    pRing2DLoc->transform( e->getTransformationToHorizontal() );
    vContoursLoc.push_back( pRing2DLoc );
  }

  for ( i = 0; i < vContours.size(); i++ )
  {
    pRing2D = vContours[i];

    for ( j = 0; j < vContoursLoc.size(); j++ )
    {
      pRing2DLoc = vContoursLoc[j];
      polygonTopologyLoc = pRing2D->intersect2D( pRing2DLoc, minDist, intersectionLoc );
      if ( polygonTopologyLoc > polygonTopology )
        polygonTopology = polygonTopologyLoc;
    }
  }

  intersectionLoc.transformInvers( e->getTransformationToHorizontal() );
  intersection.copy( intersectionLoc );

  for ( i = 0; i < vContours.size(); i++ )
    delete vContours[i];
  for ( i = 0; i < vContoursLoc.size(); i++ )
    delete vContoursLoc[i];

  return polygonTopology;
}


///////////////////////////////////////////////////////////////////////////////
// Bestimmt die topologische Beziehung zweier 3D Polygone                    //
///////////////////////////////////////////////////////////////////////////////
POLYGON_TOPOLOGY PolygonGeo::intersect3D ( PolygonGeo *pPolygon, Plane * e1, Plane * e2, double minDist, double minAngleDist, bool ignoreHoles, IntersectionGeometry & intersection )
{
  vector<PolygonGeo*>  vPolygons;
  vector<PolygonGeo*>  vPolygonLoc;
  unsigned int         i, j;
  POLYGON_TOPOLOGY     polygonTopology = POLYGONE_DISJUNKT;
  POLYGON_TOPOLOGY     polygonTopologyLoc;

//  triangulate();
//  for ( i = 0; i < vTriangles.size(); i++ )
 //   vPolygons.push_back( vTriangles[i] );
	vPolygons.push_back(this);

//  pPolygon->triangulate();
//  for ( i = 0; i < pPolygon->vTriangles.size(); i++ )
//    vPolygonLoc.push_back(  pPolygon->vTriangles[i] );
	vPolygonLoc.push_back(pPolygon);

  for ( i = 0; i < vPolygons.size(); i++ )
  {
    PolygonGeo * pPoly1 = vPolygons[i];
    for ( j = 0; j < vPolygonLoc.size(); j++ )
    {
      PolygonGeo * pPoly2 = vPolygonLoc[j];
      Plane * pPlane1 =  pPoly1->getPlane();
      Plane * pPlane2 =  pPoly2->getPlane();
      if ( pPlane1 != NULL && pPlane2 != NULL )
        polygonTopologyLoc = pPoly1->getAussenkontur()->intersect3D( pPoly2->getAussenkontur(), pPlane1, pPlane2, minDist, minAngleDist, intersection );
      else
        continue;

      if ( polygonTopologyLoc > polygonTopology )
        polygonTopology = polygonTopologyLoc;
    }
  }

  return polygonTopology;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Aussenkontur									                        				 //
///////////////////////////////////////////////////////////////////////////////
void PolygonGeo::setAussenkontur ( _Ring * aussenkonturP )
{
	UMLAUFSINN umlaufsinn;

  if ( dim == 2 )
  {
    umlaufsinn = aussenkonturP->getUmlaufsinn2D();
    if ( umlaufsinn == NEGATIVE )
      aussenkonturP->reverse();
  }
	
	aussenkontur = aussenkonturP;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Loch hinzu										                            			 //
///////////////////////////////////////////////////////////////////////////////
void PolygonGeo::addLoch ( _Ring * lochP )
{
	UMLAUFSINN umlaufsinn;

  if ( dim == 2 )
  {
	  umlaufsinn = lochP->getUmlaufsinn2D();
  	if ( umlaufsinn == POSITIVE )
		  lochP->reverse();
  }

	loecher.push_back ( lochP );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt ein Loch auf Basis der gml:id                            			 //
///////////////////////////////////////////////////////////////////////////////
_Ring * PolygonGeo::removeLoch ( std::string gmlIdP )
{
  for ( unsigned int i = 0; i < loecher.size(); i++ )
  {
    _Ring * pLoch = loecher[i];
    if ( pLoch->getGmlId() == gmlIdP )
    {
      for ( unsigned int j = i+1; j < loecher.size(); j++ )
        loecher[j-1] = loecher[j];
      loecher.pop_back();
      return pLoch;
    }
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Polygon                                                      //
///////////////////////////////////////////////////////////////////////////////
size_t PolygonGeo::getPolygone ( vector<PolygonGeo*> &vPolygone )
{
  vPolygone.push_back( this );
  return 1;
}

size_t PolygonGeo::getPolygone ( vector<PolygonGeo*> &vPolygone, vector<bool> &vOrientations )
{
  vPolygone.push_back( this );
  vOrientations.push_back( true );
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die BoundingBox									                        				 //
///////////////////////////////////////////////////////////////////////////////
bool PolygonGeo::getBoundingBox ( double * sw, double * no )
{
	if ( aussenkontur != NULL )
		return aussenkontur->getBoundingBox ( sw, no );

	sw[0] =  1.0E10;
	sw[1] =  1.0E10;
	no[0] = -1.0E10;
	no[1] = -1.0E10;
  if ( dim == 3 )
  {
    sw[2] =  1.0E10;
    no[2] = -1.0E10;
  }
  else
  {
    sw[2] = 0.0;
    no[2] = 0.0;
  }

	return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt der Aussenkontur				                				 //
///////////////////////////////////////////////////////////////////////////////
bool PolygonGeo::getCenter2D ( double &cxP, double &cyP )
{
	if ( aussenkontur != NULL )	return aussenkontur->getCenter2D ( cxP, cyP );
	else					              return false;
}

bool PolygonGeo::getCenter3D ( double &cxP, double &cyP, double &czP )
{
  if ( aussenkontur != NULL )	return aussenkontur->getCenter3D ( cxP, cyP, czP );
  else					              return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t PolygonGeo::getCurveSegmentAnz()
{
	size_t i, anz;

  if ( aussenkontur == NULL )
    return 0;

  anz = aussenkontur->getCurveSegmentAnz();

  for ( i = 0; i < loecher.size(); i++ )
  {
    _Ring * pGeo = loecher[i];
    anz = anz + pGeo->getCurveSegmentAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t PolygonGeo::getPointAnz()
{
	size_t i, anz;

  if ( aussenkontur == NULL )
    return 0;

  anz = aussenkontur->getPointAnz();

  for ( i = 0; i < loecher.size(); i++ )
  {
    _Ring * pGeo = loecher[i];
    anz = anz + pGeo->getPointAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Fläche der Aussenkontur	                                     //
///////////////////////////////////////////////////////////////////////////////
double PolygonGeo::getFlaecheMitLoechern()
{
  double        flaeche;
  PolygonGeo     * pPolygon2D;

  if ( dim == 3 )
    pPolygon2D = transformToHorizontal();
  else
    pPolygon2D = this;

  if ( pPolygon2D == NULL )
    return 0.0;

  flaeche = pPolygon2D->getAussenkontur()->getFlaeche();

  if (  dim == 3 )
    delete pPolygon2D;

  return flaeche;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Fläche des Polygons (Fläche-Aussenkontur - Fläche Löcher)    //
///////////////////////////////////////////////////////////////////////////////
double PolygonGeo::getFlaeche()
{
	double           flaeche;
	double           flaecheLoch;
	unsigned int     i;
	_Ring          * loch;
  PolygonGeo     * pPolygon2D;

  if ( dim == 3 )
    pPolygon2D = transformToHorizontal();
  else
    pPolygon2D = this;

  if ( pPolygon2D == NULL )
    return 0.0;

	flaeche = pPolygon2D->getAussenkontur()->getFlaeche();
	
	for ( i = 0; i < loecher.size(); i++ )
	{
		loch = pPolygon2D->getLoch( i );
		flaecheLoch = loch->getFlaeche();
		flaeche = flaeche - flaecheLoch;
	}

  if (  dim == 3 )
    delete pPolygon2D;

	return flaeche;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Normalenvektor eines Polygons                                //
///////////////////////////////////////////////////////////////////////////////
Direction * PolygonGeo::getNormal() 
{
  Plane * e = aussenkontur->getPlane();
  if ( e == NULL )
    return NULL;

  return &(e->getNormal());
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Ebene, in der die Polygon-Stützpunkte liegen                 //
///////////////////////////////////////////////////////////////////////////////
Plane * PolygonGeo::getPlane()
{
  return aussenkontur->getPlane();
}

///////////////////////////////////////////////////////////////////////////////
//  Basisprüfung Polygon                                                     //
//  Außenkontur ist vorhanden                                                //
//  Basisprüfung für Außenkontur und alle Innenkonturen                      //
///////////////////////////////////////////////////////////////////////////////
bool PolygonGeo::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams, 
          vector<Error*> &vErrorReport )
{
  GeometryError    * pError;
  bool               success;
  unsigned int       i, j;
  vector<Error*>     vErrorReportLoc;

  if ( aussenkontur == NULL )
  {
    pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    pError->setPropertyName( featureProperty );
    vErrorReport.push_back( pError );
    pError->setFeatureGeometryId( gmlId );
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  success = aussenkontur->basicCheck( pObj, featureProperty, pCheckParams, vErrorReportLoc );
  if ( !success )
  { 
    for ( i = 0; i < vErrorReportLoc.size(); i++ )
    {
      GeometryError * pError = (GeometryError*)vErrorReportLoc[i];
      pError->setFeatureGeometryId( gmlId );
      pError->setPropertyName( featureProperty );
      pError->addErrorGeometry ( aussenkontur->transformToCurve(), "Fehlerhafte Polygon-Aussenkontur" );

      vErrorReport.push_back( pError );
    }
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  // Prüfung von Inselflächen
  for ( i = 0; i < loecher.size(); i++  )
  {
    vErrorReportLoc.clear();
    success = loecher[i]->basicCheck (pObj, featureProperty, pCheckParams, vErrorReportLoc );
    if ( !success )
    {
      for ( j = 0; j < vErrorReportLoc.size(); j++ )
      {
        GeometryError * pError = (GeometryError*)vErrorReportLoc[j];
        pError->setFeatureGeometryId( gmlId );
        pError->setPropertyName( featureProperty );
        pError->addErrorGeometry ( loecher[i]->transformToCurve(), "Fehlerhafte Polygon-Innenkontur" );

        vErrorReport.push_back( pError );
      }
      checkResult = BASIC_CHECK_FAILED;
      return false;
    }
  }

  checkResult = BASIC_CHECK_FULFILLED;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Weitere Prüfungen Polygone                                               //
//  Planarität (3D-Polygone)                                                 //
//  Richtiger Umlaufsinn (2D-Polygone)                                       //
//  Umlaufsinn von Außenkontur und  Löchern ist entgegengesetzt              //
//  Weitere Prüfungen für Innenkonturen                                      //
//  Gegenseitige Überlappung von Innenkonturen                               //
//  Überlappung von Innenkonturen und Außenkonturen                          //
//  Innenkonturen sind im Innern der Außenkonturen                           //
///////////////////////////////////////////////////////////////////////////////

bool PolygonGeo::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  bool            success;
  Plane         * eAussenkontur = NULL;
  Plane         * eLoch;
  GeometryError * pError;
  PolygonGeo    * pPolygon2D;
  AREA_LOCATION   loc;
  unsigned int    i, j;
  vector<Error*>  vErrorReportLoc;
  double          angleDifference, pointDifference;


  GeometryError::GEOMETRY_ERROR_TYPE geometryErrorType;

  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;

  success = aussenkontur->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReportLoc, checkAnyway );
  if ( !success )
  {
    for ( i = 0; i < vErrorReportLoc.size(); i++ )
    {
      pError = (GeometryError*)vErrorReportLoc[i];
      pError->setPropertyName( featureProperty );
      pError->setFeatureGeometryId( gmlId );
      pError->addErrorGeometry ( aussenkontur->transformToCurve(), "Fehlerhafte Polygon-Aussenkontur" );
      vErrorReport.push_back( pError );
    }
    checkResult = ADVANCED_CHECK_FAILED;
    return false;
  }

  if ( dim == 2 )
  {
    eAussenkontur = aussenkontur->getPlane();
    Direction d   = eAussenkontur->getNormal();

    if ( fabs ( d.getZ() - 1.0 ) > pCheckParams->minimalAngleDistance )
    {
      pError = new GeometryError ( pObj, Error::ERROR, GeometryError::POLYGON_WRONG_ORIENTATION );
      pError->setPropertyName( featureProperty );
      pError->setFeatureGeometryId( gmlId );
      pError->addErrorGeometry ( aussenkontur->transformToCurve(), "Fehlerhafte Polygon-Aussenkontur" );
      vErrorReport.push_back( pError );

      checkResult = ADVANCED_CHECK_FAILED;
      return false;
    }
  }

  if ( dim == 3 )
    eAussenkontur = aussenkontur->getPlane();

  for ( i = 0; i < getLochAnz(); i++ )
  {
    _Ring * pLoch = getLoch( i );
    vErrorReportLoc.clear();

    success = pLoch->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReportLoc, checkAnyway );
    if ( !success )
    {
     for ( j = 0; j < vErrorReportLoc.size(); j++ )
      {
        pError = (GeometryError*)vErrorReportLoc[j];
        pError->setPropertyName( featureProperty );
        pError->setFeatureGeometryId( gmlId );
        pError->addErrorGeometry ( pLoch->transformToCurve(), "Fehlerhafte Polygon-Innenkontur" );
        vErrorReport.push_back( pError );
      }
      checkResult = ADVANCED_CHECK_FAILED;
      return false;
    }

    eLoch = pLoch->getPlane();
    eAussenkontur->getDifference( eLoch, angleDifference, pointDifference );
      
    success = true;
    if ( fabs ( angleDifference - PI ) > pCheckParams->minimalAngleDistance  )
    {
      success = false;
      geometryErrorType = GeometryError::POLYGON__VOID_ORIENTATION_ERROR;
    }
     
    if ( pointDifference > pCheckParams->planarityLimit )
    {
      success = false;
      geometryErrorType = GeometryError::POLYGON_VOID_PLANARITY_ERROR;
    }

    if ( !success )
    { 
      _Geometrie * pErrorGeometry;
      if ( pObj != NULL )
      {
        pErrorGeometry = new SurfaceReference ( pObj->getFeatures() );
        ((SurfaceReference*)pErrorGeometry)->setRefGeoId( gmlId );
      }
      else
      {
        pErrorGeometry = new PolygonGeo ( this );
      }

      pError = new GeometryError ( pObj, Error::ERROR, geometryErrorType );
      pError->setPropertyName( featureProperty );
      pError->setFeatureGeometryId( gmlId );
      pError->setPlanaritaetsfehler( pointDifference, pointDifference );
      pError->addErrorGeometry( pErrorGeometry, "Nicht-planares Polygon" );
      vErrorReport.push_back( pError );
      checkResult = ADVANCED_CHECK_FAILED;
      return false;  
    }
  }

  checkResult = PLANARITY_CHECK_FULFILLED;
  if ( getLochAnz() == 0 )
  {
    checkResult = ADVANCED_CHECK_FULFILLED;
    return true;
  }

 if ( dim == 3 )
    pPolygon2D = transformToHorizontal();
  else
    pPolygon2D = this;

 vector<GeoPoint> vKontur, vLoch;
 pPolygon2D->getAussenkontur()->getPoints( vKontur );

  for ( unsigned int i = 0; i < pPolygon2D->getLochAnz(); i++ )
  {
    _Ring * pAussenkontur = pPolygon2D->getAussenkontur();
    _Ring * pLoch = pPolygon2D->getLoch( i );
    
    vLoch.clear();
    pLoch->getPoints( vLoch );

    loc = pAussenkontur->checkTopology2D( pLoch, pCheckParams->minimalPointDistance, pCheckParams->minimalAngleDistance );
    if ( loc != INNEN && loc != INNEN_BERUEHREND_SINGLE_POINT )
    {
      MultiCurve * pMultiCurve = new MultiCurve ( dim );
      pMultiCurve->addCurve( pAussenkontur->transformToCurve() );
      pMultiCurve->addCurve( pLoch->transformToCurve() );

      pError = new GeometryError ( pObj, Error::ERROR, GeometryError::POLYGON_VOID_ERROR );

      if ( loc == AUSSEN || loc == AUSSEN_BERUEHREND_SINGLE_POINT  || loc == AUSSEN_BERUEHREND_MULTI_POINT )
        pError->setErrorDescription( "Die Außenkontur eines Polygons liegt in einer Innenkontur" );
      else
      if ( loc == DISJUNKT || loc == DISJUNKT_BERUEHREND_SINGLE_POINT || loc == DISJUNKT_BERUEHREND_MULTI_POINT )
        pError->setErrorDescription( "Eine Innenkontur eines Polygons liegt außerhalb der Außenkontur" );
      else
      if ( loc == GLEICH )
        pError->setErrorDescription( "Eine Innenkontur eines Polygons ist identisch mit der Außenkontur" );
      else
        pError ->setErrorDescription( "Innenkonturen eines Polygons überlappen die Außenkontur" );

      pError->setPropertyName( featureProperty );
      pError->setFeatureGeometryId( gmlId );
      pError->addErrorGeometry( pMultiCurve, "Fehlerhaftes Polygon" );
      vErrorReport.push_back( pError );

      checkResult = ADVANCED_CHECK_FAILED;
      return false;
    }
    for ( unsigned int j = i+1; j < getLochAnz(); j++ )
    {
      _Ring * pLochA =  pPolygon2D->getLoch(j);
      loc = pLoch->checkTopology2D( pLochA, pCheckParams->minimalPointDistance, pCheckParams->minimalAngleDistance );
      if ( loc != DISJUNKT && loc != DISJUNKT_BERUEHREND_SINGLE_POINT )
      {
        MultiCurve * pMultiCurve = new MultiCurve ( dim );
        pMultiCurve->addCurve( pLochA->transformToCurve() );
        pMultiCurve->addCurve( pLoch->transformToCurve() );

        pError = new GeometryError ( pObj, Error::ERROR, GeometryError::POLYGON_VOID_ERROR );
        if ( loc == INNEN || loc == INNEN_BERUEHREND_SINGLE_POINT  || loc == INNEN_BERUEHREND_MULTI_POINT )
             pError->setErrorDescription( "Eine Innenkontur eines Polygons liegt innerhalb einer anderen Innenkontur" );
        else
        if ( loc == AUSSEN || loc == AUSSEN_BERUEHREND_SINGLE_POINT || loc == AUSSEN_BERUEHREND_MULTI_POINT )
            continue;
        else
        if ( loc == GLEICH )
          pError->setErrorDescription( "Zwei Innenkonturen eines Polygons sind identisch" );
        else
          pError->setErrorDescription( "Einzelne Innenkonturen eines Polygons überlappen sich" );

        pError->setPropertyName( featureProperty );
        pError->setFeatureGeometryId( gmlId );
        pError->addErrorGeometry( pMultiCurve, "Fehlerhaftes Polygon" );

        vErrorReport.push_back( pError );

        checkResult = ADVANCED_CHECK_FAILED;
        return false;
      }
    }
  }

  if ( dim == 3 )
    delete pPolygon2D;

  checkResult = ADVANCED_CHECK_FULFILLED;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Affine Transformation eines Polygons                                     //
///////////////////////////////////////////////////////////////////////////////
void PolygonGeo::transform ( Matrix4 & matrix )
{
  unsigned int i;
  
  bbExist = false;

  if ( aussenkontur != NULL )
    aussenkontur->transform ( matrix );

  for ( i = 0; i < loecher.size(); i++ )
  {
    loecher[i]->transform (  matrix );
  }

  for ( i = 0; i < vTriangles.size(); i++ )
    vTriangles[i]->transform( matrix );
}

///////////////////////////////////////////////////////////////////////////////
//  Inverse affine Transformation eines Polygons                             //
///////////////////////////////////////////////////////////////////////////////
void PolygonGeo::transformInvers ( Matrix4 & matrix )
{
  unsigned int i;

  bbExist = false;

  if ( aussenkontur != NULL )
    aussenkontur->transformInvers ( matrix );

  for ( i = 0; i < loecher.size(); i++ )
  {
    loecher[i]->transformInvers (  matrix );
  }

  for ( i = 0; i < vTriangles.size(); i++ )
    vTriangles[i]->transformInvers( matrix );
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert das Polygon in die (x,y) Ebene                             //
///////////////////////////////////////////////////////////////////////////////
PolygonGeo * PolygonGeo::transformToHorizontal()
{
  PolygonGeo * pPolygon = new PolygonGeo ( this );

  Plane    * ebene = getPlane();
  if ( ebene == NULL )
    return NULL;

  Matrix4  trans = ebene->getTransformationToHorizontal();

  pPolygon->transform ( trans );
  pPolygon->setDim( 2 );
  pPolygon->getAussenkontur()->setDim( 2 );
  for ( unsigned int i = 0; i < pPolygon->getLochAnz(); i++ )
    pPolygon->getLoch( i )->setDim( 2 );

  return pPolygon;
}

PolygonGeo * PolygonGeo::transformToHorizontal( Plane * pPlane )
{
  PolygonGeo * pPolygon = new PolygonGeo ( this );
  Matrix4  trans = pPlane->getTransformationToHorizontal();

  pPolygon->transform ( trans );
  pPolygon->setDim( 2 );
  pPolygon->getAussenkontur()->setDim( 2 );
  for ( unsigned int i = 0; i < pPolygon->getLochAnz(); i++ )
    pPolygon->getLoch( i )->setDim( 2 );

  return pPolygon;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für die Geometrie ein gültiges                              //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool PolygonGeo::existReferenceSystem( SrsManager * pSrsManager )
{
  bool exist;

  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  if ( aussenkontur == NULL )
    return false;

  exist = aussenkontur->existReferenceSystem( pSrsManager );
  if ( !exist )
    return false;

  for ( unsigned int i = 0; i < loecher.size(); i++ )
  {
    if ( loecher[i]->existReferenceSystem( pSrsManager ) == false )
      return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine globale Angabe über die Orientierung der Fläche             //
///////////////////////////////////////////////////////////////////////////////
SURFACE_NORMAL_TYPE PolygonGeo::getSurfaceNormal( double angleTolenance, double areaToleranceRelative )
{
  double x, y, z;

  if ( checkResult < ADVANCED_CHECK_FULFILLED ) 
    return INDIFFERENT;

  Direction normal = aussenkontur->getPlane()->getNormal();
  normal.get ( x, y, z );

  if ( fabs ( 1.0 - z ) < angleTolenance )
    return VERTICAL_UPWARD;

  if ( fabs ( 1.0 + z ) < angleTolenance )
    return VERTICAL_DOWNWARD;

  if ( z > angleTolenance )
    return UPWARD;

  if ( z < -angleTolenance )
    return DOWNWARD;

  return HORIZONTAL;
}

///////////////////////////////////////////////////////////////////////////////
//  Korrigiert eine Einschnürung in der 2D-Polygon-Außenkontur durch         //
//  Umwandlung in ein Loch                                                   //
///////////////////////////////////////////////////////////////////////////////
bool PolygonGeo::changeContourNarrowingIntoVoid ( size_t index1, size_t index2, CheckingParams * pParams )
{
  GeoPoint         p1A;
  GeoPoint         p1E;
  GeoPoint         p2A;
  GeoPoint         p2E;
	size_t           anz, anzExtracted;
  vector<GeoPoint> vPointsExtracted;

  if ( dim == 2 )
    return false;

  LinearRing * pContour = dynamic_cast<LinearRing*>(aussenkontur);
  if ( pContour == NULL )
    return false;

  anz = pContour->getPointAnz();

  LineSegment * pRingPoints = pContour->getCurve();

  if ( index1 > index2 )
  {
		size_t i  = index1;
    index1    = index2;
    index2    = i;
  }

  p1A = pRingPoints->getPoint ( index1 );
  if ( index1 < anz - 1 )
    p1E = pRingPoints->getPoint ( index1 + 1 );
  else
    p1E = pRingPoints->getPoint ( 0 );

  p2A = pRingPoints->getPoint ( index2 );
  if ( index2 < anz - 1 )
    p2E = pRingPoints->getPoint ( index2 + 1 );
  else
    p2E = pRingPoints->getPoint ( 0 );

  if ( p1A.dist(p2A) >= pParams->minimalPointDistance )
  {
    if (  p1A.dist(p2E)< pParams->minimalPointDistance)
    {
      if ( index2 < anz - 1 )
        index2 = index2 + 1;
      else
        index2 = 0;
    }
    else
    if (  p1E.dist(p2A) < pParams->minimalPointDistance )
    {
      index1 = index1 + 1;
    }
    else
    if ( p1E.dist(p2E) < pParams->minimalPointDistance )
    {
      index1 = index1 + 1;
      if ( index2 < anz - 1 )
        index2 = index2 + 1;
      else
        index2 = 0;
    }
    else
      return false;
  }

  if ( index1 > index2 )
  {
		size_t i  = index1;
    index1    = index2;
    index2    = i;
  }

  anzExtracted = pRingPoints->getPoints( index1, index2, vPointsExtracted, true );
  if ( anzExtracted < 3 )
    return false;

  vPointsExtracted.push_back( vPointsExtracted[0] );

  LineSegment * pNewLineSegment = new LineSegment ( dim, LineSegment::_LINEAR_RING );
  pNewLineSegment->addPoints( vPointsExtracted );

  LinearRing * pNewRing = new LinearRing ( dim );
  pNewRing->setLineSegment( pNewLineSegment );

  AREA_LOCATION loc = pContour->checkTopology2D ( pNewRing, pParams->minimalPointDistance, pParams->minimalAngleDistance  );
  if ( loc == INNEN_BERUEHREND_SINGLE_POINT)
  {
    pNewRing->reverse();
    addLoch( pNewRing );
    return true;
  }
  else
  if ( loc == AUSSEN_BERUEHREND_SINGLE_POINT )
  {
    aussenkontur = pNewRing;
    pContour->reverse();
    addLoch( pContour );
    return true;
  }
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Triangulation des Polygons                                               //
///////////////////////////////////////////////////////////////////////////////
void PolygonGeo::triangulate()
{
  vector<int>       vTriaPointIndex;
  vector<GeoPoint*> vPoints;
	size_t            j, triaAnz;
  bool              success;

 if ( vTriangles.size() > 0 )
   return;
  
  TessTriangulator * pTriang = new TessTriangulator;
  success = pTriang->triangulate( this, vTriaPointIndex, vPoints, true );
  if ( !success )
    return;

  triaAnz = vTriaPointIndex.size() / 4;

  for ( j = 0; j < triaAnz; j++ )
  {
    GeoPoint * p0 = nullptr, * p1 = nullptr, * p2 = nullptr;
    bool       success;
    
    success = true;

    if ( vTriaPointIndex[4*j] < 0 ||  vTriaPointIndex[4*j] >= (int)vPoints.size() )
      success = false;
    else
      p0 = vPoints[vTriaPointIndex[4*j]];


    if ( vTriaPointIndex[4*j+1] < 0 ||  vTriaPointIndex[4*j+1] >= (int)vPoints.size() )
      success = false;
    else
      p1 = vPoints[vTriaPointIndex[4*j+1]];


    if ( vTriaPointIndex[4*j+2] < 0 ||  vTriaPointIndex[4*j+2] >= (int)vPoints.size() )
      success = false;
    else
      p2 = vPoints[vTriaPointIndex[4*j+2]];

    if ( success )
    {
      Triangle * pTria = new Triangle ( 3 );
      pTria->set( *p0, *p1, *p2 );
      vTriangles.push_back( pTria );
    }
  }

  for ( j = 0; j < (int)vPoints.size();  j++ )
    delete vPoints[j];
  delete pTriang;
}

size_t PolygonGeo::getTriangles ( std::vector<Triangle*> & vTrianglesP )  
{  
  vTrianglesP = vTriangles;
  return vTriangles.size();
}  

///////////////////////////////////////////////////////////////////////////////
//  Offset auf die Polygon-Geometrie                                         //
///////////////////////////////////////////////////////////////////////////////
void PolygonGeo::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  unsigned int i; 

  _Surface::offset ( point, sGeometries );

  if ( aussenkontur != NULL && sGeometries.count( aussenkontur ) == 0 )
    aussenkontur->offset ( point, sGeometries );

  for ( i = 0; i < loecher.size(); i++ )
  {
    if ( loecher[i] != NULL && sGeometries.count(  loecher[i] ) == 0 )
       loecher[i]->offset ( point, sGeometries );
  }

  for ( i = 0; i < vTriangles.size(); i++ )
  {
    if ( vTriangles[i] != NULL && sGeometries.count(  vTriangles[i] ) == 0 )
      vTriangles[i]->offset ( point, sGeometries );
  }
  sGeometries.insert ( this );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Polygon, wenn die übergebene gml:id mit der Polygon-id       //
//  übereinstimmt                                                            //
///////////////////////////////////////////////////////////////////////////////
_Surface * PolygonGeo::removePolygon( std::string gmlIdP )
{
  if ( gmlId == gmlIdP )
    return this;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Triangle                                                                 //
//  Implementierung der Klasse gml:Triangle                                  //
///////////////////////////////////////////////////////////////////////////////
Triangle::Triangle( int dimP )
:PolygonGeo ( dimP )
{

}

Triangle::Triangle ( Triangle * pPolyOld )
:PolygonGeo ( pPolyOld )
{

}
Triangle::~Triangle()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Dreiecks-Eckpunkte                                             //
///////////////////////////////////////////////////////////////////////////////
void Triangle::set ( GeoPoint p0, GeoPoint p1, GeoPoint p2 )
{
  vector<GeoPoint> vPoints;
  if ( aussenkontur != NULL ) delete aussenkontur;

  aussenkontur = new LinearRing ( dim );
  vPoints.push_back( p0 );
  vPoints.push_back( p1 );
  vPoints.push_back( p2 );
  vPoints.push_back( p0 );
  
  ((LinearRing*)aussenkontur)->addPoints ( vPoints );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Dreiecks-Eckpunkte                                           //
///////////////////////////////////////////////////////////////////////////////
void Triangle::get ( GeoPoint &p0, GeoPoint &p1, GeoPoint &p2 )
{
  vector<GeoPoint> vPoints;
	size_t           anz;

  if ( aussenkontur == NULL ) return;

  anz = aussenkontur->getPoints( vPoints );
  if ( anz < 4 ) return;

  p0 = vPoints[0];
  p1 = vPoints[1];
  p2 = vPoints[2];
}

///////////////////////////////////////////////////////////////////////////////
//  Implementierung der Klasse gml:Ring								                	     //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor												                         //
///////////////////////////////////////////////////////////////////////////////
Ring::Ring( int dimP ) : _Ring( dimP )
{
	pGeometrie = NULL;
}

Ring::Ring( Ring * pRingOld ): _Ring ( pRingOld )
{
	if ( pRingOld->pGeometrie != NULL )
		pGeometrie = new MultiCurve ( pRingOld->pGeometrie );
	else
		pGeometrie = NULL;
}

Ring::~Ring()
{
	if ( pGeometrie != NULL )delete pGeometrie;
}

///////////////////////////////////////////////////////////////////////////////
//  Kopiert den Ring                                                         //
///////////////////////////////////////////////////////////////////////////////
_Ring * Ring::copyRing ()
{
  Ring * pCopyRing = new Ring ( this );
  return pCopyRing; 
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines curveMembers des Rings									                 //
///////////////////////////////////////////////////////////////////////////////
void Ring::addRingMember ( _Curve * pCurve )
{
	if ( pGeometrie == NULL )
  {
    pGeometrie = new MultiCurve( dim );
    pGeometrie->setIsCompositeCurve( true );
  }

	pGeometrie->addCurve ( pCurve );
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert den Ring aus einer MultiCurve                                  //
///////////////////////////////////////////////////////////////////////////////
void Ring::setRingGeometrie ( MultiCurve * pMultiCurve )
{
	if ( pGeometrie != NULL ) delete pGeometrie;
	pGeometrie = pMultiCurve;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert den Ring aus einer _Curve                                      //
///////////////////////////////////////////////////////////////////////////////
void Ring::setRingGeometrie ( _Curve * pCurveMember )
{
  if ( pGeometrie != NULL ) delete pGeometrie;

  pGeometrie = new MultiCurve( dim );
  pGeometrie->setIsCompositeCurve( true );
  pGeometrie->addCurve ( pCurveMember );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert curveMember mit index						  					                     //
///////////////////////////////////////////////////////////////////////////////
_Curve * Ring::getRingMember( size_t indexP )
{
	if ( pGeometrie != NULL )
		return pGeometrie->getCurve( indexP );  
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Segmente											                     //
///////////////////////////////////////////////////////////////////////////////
size_t Ring::getRingMemberAnz()
{  
	if ( pGeometrie != NULL )
		return pGeometrie->getCurveMemberAnz();  
	else
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 2D-Punkte des Ringes, wobei der erste und letzte Punkt      //
//  identisch sind															                             //
///////////////////////////////////////////////////////////////////////////////
size_t Ring::getPoints2D ( std::vector<double> &vXP, std::vector<double> &vYP )
{
	vector<double> vXloc;
	vector<double> vYloc;
	double		     x, y;
	unsigned int   i, j, indexAnf;

	for ( i = 0; i < getRingMemberAnz(); i++ )
	{
		vXloc.clear();
		vYloc.clear();
		_Curve * pSeg = getRingMember( i );
		pSeg->getPoints2D ( vXloc, vYloc );
		if ( i == 0 ) indexAnf = 0;
		else		  indexAnf = 1;
		
		for ( j = indexAnf; j < vXloc.size(); j++ )
		{
			x = vXloc[j];
			y = vYloc[j];
			vXP.push_back ( x );
			vYP.push_back ( y );
		}
	}

	return vXP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 2D-Punkte des Ringes in umgekehrter Reihenfolge             //
//  Der erste und letzte Punkt sind identisch                                //
///////////////////////////////////////////////////////////////////////////////
size_t Ring::getPoints2DReverse ( std::vector<double> &vXP, std::vector<double> &vYP )
{
  vector<double> vXloc;
  vector<double> vYloc;
  double		     x, y;
	size_t         j, indexAnf, pointAnz;
	int            i;

  for ( i = (int)getRingMemberAnz()-1; i >= 0; i-- )
  {
    vXloc.clear();
    vYloc.clear();
    
    _Curve * pSeg = getRingMember( (size_t)i );
    pointAnz = pSeg->getPoints2DReverse ( vXloc, vYloc );
    if ( i == getRingMemberAnz()-1 ) indexAnf = 0;
    else		                         indexAnf = 1;

    for ( j = indexAnf; j < pointAnz; j++ )
    {
      x = vXloc[j];
      y = vYloc[j];
      vXP.push_back ( x );
      vYP.push_back ( y );
    }
  }

  return vXP.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 3D-Punkte des Ringes, wobei der erste und letzte Punkt      //
//  identisch sind															                             //
///////////////////////////////////////////////////////////////////////////////
size_t Ring::getPoints3D ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP )
{
  vector<double> vXloc;
  vector<double> vYloc;
  vector<double> vZloc;
  double		     x, y, z;
  unsigned int   i, j, indexAnf;

  for ( i = 0; i < getRingMemberAnz(); i++ )
  {
    vXloc.clear();
    vYloc.clear();
    vZloc.clear();
    _Curve * pSeg = getRingMember( i );
    pSeg->getPoints3D ( vXloc, vYloc, vZloc );
    if ( i == 0 ) indexAnf = 0;
    else		  indexAnf = 1;

    for ( j = indexAnf; j < vXloc.size(); j++ )
    {
      x = vXloc[j];
      y = vYloc[j];
      z = vZloc[j];
      vXP.push_back ( x );
      vYP.push_back ( y );
      vZP.push_back ( z );
    }
  }

  return vXP.size();
}

////////////////////////////////////////////////////////////////////////////////
//  Liefert alle Punkte des Ringes als GeoPoint Objekte                       //
////////////////////////////////////////////////////////////////////////////////
size_t Ring::getPoints ( std::vector<GeoPoint> &vPoints )
{
  vector<GeoPoint> vPointsLoc;

  unsigned int   i, j, indexAnf;

  for ( i = 0; i < getRingMemberAnz(); i++ )
  {
    vPointsLoc.clear();
    _Curve * pSeg = getRingMember( i );
    pSeg->getPoints ( vPointsLoc );
    if ( i == 0 ) indexAnf = 0;
    else		  indexAnf = 1;

    for ( j = indexAnf; j < vPointsLoc.size(); j++ )
      vPoints.push_back ( vPointsLoc[j] );
  }

  return vPoints.size();
}

////////////////////////////////////////////////////////////////////////////////
//  Liefert alle Punkte des Ringes als GeoPoint Objekte in umgekehrter        //
//  Reihenfolge                                                               //
////////////////////////////////////////////////////////////////////////////////
size_t Ring::getPointsReverse ( std::vector<GeoPoint> &vPoints )
{
  size_t anz = getPoints ( vPoints );
  if ( anz == 0 )
    return 0;

  for ( size_t i = 0; i < anz % 2; i++ )
  {
    size_t j = anz - 1 - i;
    GeoPoint h = vPoints[i];
    vPoints[i] = vPoints[j];
    vPoints[j] = h;
  }

  return anz;
}


////////////////////////////////////////////////////////////////////////////////
//  Liefert einen Punkt des Ringes auf Basis des Index                        //
////////////////////////////////////////////////////////////////////////////////
GeoPoint Ring::getPoint ( size_t index )
{
  vector<GeoPoint> vPoints;
	size_t           anz;

  anz = getPoints ( vPoints );
  if ( index >= 0 && index < vPoints.size() )
    return vPoints[index];
  else
    return GeoPoint ( dim );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 3D-Punkte des Ringes in umgekehrter Reihenfolge             //
//  Der erste und letzte Punkt sind identisch                                //
///////////////////////////////////////////////////////////////////////////////
size_t Ring::getPoints3DReverse ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP )
{
  vector<double> vXloc;
  vector<double> vYloc;
  vector<double> vZloc;
  double		     x, y, z;
	size_t         j, indexAnf, pointAnz;
	int            i;

  for ( i = (int)getRingMemberAnz()-1; i >= 0; i-- )
  {
    vXloc.clear();
    vYloc.clear();
    vZloc.clear();
    _Curve * pSeg = getRingMember( (size_t)i );

    pointAnz = pSeg->getPoints3DReverse ( vXloc, vYloc, vZloc );
    if ( i == 0 ) indexAnf = 0;
    else		  indexAnf = 1;

    for ( j = indexAnf; j < pointAnz; j++ )
    {
      x = vXloc[j];
      y = vYloc[j];
      z = vZloc[j];
      vXP.push_back ( x );
      vYP.push_back ( y );
      vZP.push_back ( z );
    }
  }

  return vXP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Kehrt die Laufrichtung des Ringes um  		                        			 //
///////////////////////////////////////////////////////////////////////////////
void Ring::reverse()
{
	if ( pGeometrie != NULL ) pGeometrie->reverse();
  if ( e != NULL ) e->reverse();
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Bounding-Box									                      			 //
///////////////////////////////////////////////////////////////////////////////
bool Ring::getBoundingBox ( double * sw, double * no )
{
	if ( pGeometrie != NULL )
		return pGeometrie->getBoundingBox ( sw, no );
  else
	  return false;
}
///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt                                                  //
///////////////////////////////////////////////////////////////////////////////
bool Ring::getCenter2D ( double &xP, double &yP )
{
  if ( pGeometrie != NULL )
    return pGeometrie->getCenter2D ( xP, yP );
  else
    return false;
}

bool Ring::getCenter3D ( double &xP, double &yP, double &zP )
{
  if ( pGeometrie != NULL )
    return pGeometrie->getCenter3D ( xP, yP, zP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t Ring::getCurveSegmentAnz()
{
  if ( pGeometrie == NULL )
    return 0;
  else
    return pGeometrie->getCurveSegmentAnz();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t Ring::getPointAnz()
{
  if ( pGeometrie == NULL )
    return 0;
  else
    return pGeometrie->getPointAnz();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die 2D-Koordinaten als SVG-String	  			                			 //
///////////////////////////////////////////////////////////////////////////////
std::string Ring::getPointsSVG ( double offsetX, double offsetY )
{
	string str;

	if ( pGeometrie != NULL )
	{
		str = pGeometrie->getPointsSVG( offsetX, offsetY );
		str += " Z";
	}
	return str;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die 2D-Koordinaten in umgekehrter Reihenfolge als SVG-String		 //
///////////////////////////////////////////////////////////////////////////////
std::string Ring::getPointsReverseSVG ( double offsetX, double offsetY )
{
	string str;

	if ( pGeometrie != NULL )
	{
    str = pGeometrie->getPointsReverseSVG( offsetX, offsetY );
		str += " Z";
	}
	return str;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Länge des Umfangs							                    				 //
///////////////////////////////////////////////////////////////////////////////
double Ring::getUmfang()
{
	if ( pGeometrie != NULL ) return pGeometrie->getLength();
	else					  return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Affine Transformation eines Ringes                                       //
///////////////////////////////////////////////////////////////////////////////
void Ring::transform ( Matrix4 & matrix  )
{
  if (  pGeometrie != NULL )
  {
    pGeometrie->transform ( matrix );
    bbExist = false;

    calcPlane();
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Inverse affine Transformation eines Ringes                               //
///////////////////////////////////////////////////////////////////////////////
void Ring::transformInvers ( Matrix4 & matrix  )
{
  if (  pGeometrie != NULL )
  {
    pGeometrie->transformInvers ( matrix );
    bbExist = false;

    calcPlane();
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert den Ring in die (x,y)-Ebene                                //
///////////////////////////////////////////////////////////////////////////////
 _Ring *  Ring::transformToHorizontal ()
 {
   if ( e == NULL )
   {
     bool success = calcPlane();
     if ( !success )
       return NULL;
   }

   Matrix4  trans = e->getTransformationToHorizontal();

   Ring * pTransformedRing = new Ring ( this );
   pTransformedRing->transform ( trans );
   pTransformedRing->setDim( 2 );
   pTransformedRing->calcPlane();

   return pTransformedRing;
 }
 
///////////////////////////////////////////////////////////////////////////////
//  Berechnet gleichmäßig verteilte Stützpunkte auf dem Ring		        		 //
///////////////////////////////////////////////////////////////////////////////
 size_t Ring::scanRing2D ( std::vector<double> &xP, std::vector<double> &yP, 
							std::vector<double> &angleP, double scanDist, bool reverse )
{
	vector<double> xLoc;
	vector<double> yLoc;
	vector<double> angleLoc;
	unsigned int   i, j, jAnf;

	if ( pGeometrie == NULL ) return 0;

	for ( i = 0; i < pGeometrie->getCurveMemberAnz(); i++ )
	{
		xLoc.clear();
		yLoc.clear();
		angleLoc.clear();

		_Curve * pCurve = pGeometrie->getCurve ( i );
		pCurve->scanCurve2D ( xLoc, yLoc, angleLoc, scanDist, reverse );
		if ( i == 0 ) jAnf = 0;
		else		  jAnf = 1;
		for ( j = jAnf; j < xLoc.size(); j++ )
		{
			xP.push_back ( xLoc[j] );
			yP.push_back ( yLoc[j] );
			angleP.push_back ( angleLoc[j] );
		}
	}

	return xP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Prüft den Ring auf doppelte Punkte und entfernt diese ggf.               //
///////////////////////////////////////////////////////////////////////////////
bool Ring::checkDoublePoints ( double minDist, bool remove )
{
	size_t          i, anz;
  vector<_Curve*> vCurves;
  bool            success = true;
  bool            successLoc;

  anz = pGeometrie->getCurves( vCurves );
  for ( i = 0; i < anz; i++ )
  {
    successLoc = vCurves[i]->checkDoublePoints ( minDist, remove );
    if ( !successLoc )
      success = false;
  }
  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für die Geometrie ein gültiges                              //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool Ring::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  if ( pGeometrie != NULL )
    return pGeometrie->existReferenceSystem ( pSrsManager );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt die Ring-Geometrie um einen Offset                            //
///////////////////////////////////////////////////////////////////////////////
void Ring::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  if ( pGeometrie != NULL && sGeometries.count( pGeometrie ) == 0 )
    pGeometrie->offset ( point, sGeometries );

  sGeometries.insert( this );
}

///////////////////////////////////////////////////////////////////////////////
//  LinearRing													                              			 //
//  Implementierung der Klasse gml:LinearRing								                 //
///////////////////////////////////////////////////////////////////////////////
LinearRing::LinearRing( int dimP ): _Ring( dimP  )
{
	points = NULL;
}

LinearRing::LinearRing( LinearRing * pRingOld ) : _Ring ( pRingOld )
{
	if ( pRingOld->points != NULL )
		points = new LineSegment ( pRingOld->points );
	else
		points = NULL;
}

LinearRing::~LinearRing()
{
	if ( points != NULL ) delete points;
}

///////////////////////////////////////////////////////////////////////////////
//  Kopiert den LinearRing                                                   //
///////////////////////////////////////////////////////////////////////////////
_Ring * LinearRing::copyRing ()
{
  LinearRing * pCopyRing = new LinearRing ( this );
  return pCopyRing; 
}

///////////////////////////////////////////////////////////////////////////////
//  Definiert die 2D-Stützpunkte des LinearRing			               					 //
///////////////////////////////////////////////////////////////////////////////
void LinearRing::addPoints2D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP )
{
	if ( points != NULL ) delete points;

	points = new LineSegment( dim, LineSegment::_LINEAR_RING );
	points->addPoints2D ( xCoordsP, yCoordsP );
}

///////////////////////////////////////////////////////////////////////////////
//  Definiert die 3D-Stützpunkte des LinearRing			               					 //
///////////////////////////////////////////////////////////////////////////////
void LinearRing::addPoints3D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP )
{
  if ( points != NULL ) delete points;

  points = new LineSegment( dim, LineSegment::_LINEAR_RING );
  points->addPoints3D ( xCoordsP, yCoordsP, zCoordsP );
}

///////////////////////////////////////////////////////////////////////////////
//  Definiert die Stützpunkte des LinearRing                                 //
///////////////////////////////////////////////////////////////////////////////
void LinearRing::addPoints ( vector<GeoPoint> &vPoints )
{
  if ( points != NULL ) delete points;

  points = new LineSegment( dim, LineSegment::_LINEAR_RING );
  points->addPoints ( vPoints );
}

///////////////////////////////////////////////////////////////////////////////
//  Definiert die Geometrie des LinearRing                                   //
///////////////////////////////////////////////////////////////////////////////
void LinearRing::setLineSegment ( LineSegment * pLineSegment )
{
	if ( points != NULL ) delete points;
	points = pLineSegment;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die 2D-Stützpunkte des LinearRing			                    			 //
///////////////////////////////////////////////////////////////////////////////
size_t LinearRing::getPoints2D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP )
{
	if ( points != NULL ) return points->getPoints2D ( xCoordsP, yCoordsP );
	else		  		        return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die 2D-Stützpunkte des LinearRing	in umgekehrter Reihenfolge		 //
///////////////////////////////////////////////////////////////////////////////
size_t LinearRing::getPoints2DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP )
{
  if ( points != NULL ) return points->getPoints2DReverse ( xCoordsP, yCoordsP );
  else		  		        return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die 3D-Stützpunkte des LinearRing			                    			 //
///////////////////////////////////////////////////////////////////////////////
size_t LinearRing::getPoints3D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP  )
{
  if ( points != NULL ) return points->getPoints3D ( xCoordsP, yCoordsP, zCoordsP );
  else		  		        return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die 3D-Stützpunkte des LinearRing	in umgekehrter Reihenfolge		 //
///////////////////////////////////////////////////////////////////////////////
size_t LinearRing::getPoints3DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP  )
{
  if ( points != NULL ) return points->getPoints3DReverse ( xCoordsP, yCoordsP, zCoordsP );
  else		  		        return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Stützpunkte des LinearRing als GeoPoint Objekte        			 //
///////////////////////////////////////////////////////////////////////////////
size_t LinearRing::getPoints ( std::vector<GeoPoint> & vPoints )
{
  if ( points != NULL ) return points->getPoints ( vPoints );
  else		  		        return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Stützpunkte des LinearRing als GeoPoint Objekte in     			 //
//  umgekehrter Reihenfolge                                                  //
///////////////////////////////////////////////////////////////////////////////
size_t LinearRing::getPointsReverse ( std::vector<GeoPoint> & vPoints )
{
  size_t anz = getPoints ( vPoints );
  if ( anz == 0 )
    return 0;

  for ( size_t i = 0; i < anz % 2; i++ )
  {
    size_t j = anz - 1 - i;
    GeoPoint h = vPoints[i];
    vPoints[i] = vPoints[j];
    vPoints[j] = h;
  }
  return anz;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert den Stützpunkt an Indexposition index                            //
///////////////////////////////////////////////////////////////////////////////
GeoPoint LinearRing::getPoint ( size_t index )
{
  return points->getPoint ( index );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding-Box							                        					 //
///////////////////////////////////////////////////////////////////////////////
bool LinearRing::getBoundingBox ( double * sw, double * no )
{
	if ( points != NULL )
		return points->getBoundingBox( sw, no );
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt                                                  //
///////////////////////////////////////////////////////////////////////////////
bool LinearRing::getCenter2D ( double &xP, double &yP )
{
  if ( points != NULL )
    return points->getCenter2D( xP, yP );
  else
    return false;
}

bool LinearRing::getCenter3D ( double &xP, double &yP, double &zP )
{
  if ( points != NULL )
    return points->getCenter3D( xP, yP, zP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t LinearRing::getPointAnz()
{
  if ( points == NULL )
    return 0;
  else
    return points->getPointAnz();
}

///////////////////////////////////////////////////////////////////////////////
//  Dreht die Laufrichtung des LinearRing um					                			 //
///////////////////////////////////////////////////////////////////////////////
void LinearRing::reverse()
{
	if ( points != NULL ) points->reverse();
  if ( e != NULL )
    e->reverse();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die z-Koordinaten aller Stützpunkte auf einen konstanten Wert      //
///////////////////////////////////////////////////////////////////////////////
void LinearRing::setZKoordinate ( double zP )
{
  if ( points == NULL )
    return;

  points->setZKoordinate( zP );
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die 2D-Stützpunkte als SVG-String			                  				 //
///////////////////////////////////////////////////////////////////////////////
std::string LinearRing::getPointsSVG ( double offsetX, double offsetY )
{
	string         str;
  ostringstream  os;
  GeoPoint       p(2);

	if ( points != NULL )
	{
    os << "M ";
    p = points->getAnfangsPunkt();
    os << p.getX() - offsetX << " " << offsetY - p.getY() << " ";
    str = os.str();

		str += points->getPointsSVG( offsetX, offsetY );
		str += " Z";
	}
	
	return str;	
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die 2D-Stützpunkte in umgekehrter Reihenfolge als SVG-String  	 //
///////////////////////////////////////////////////////////////////////////////
std::string LinearRing::getPointsReverseSVG ( double offsetX, double offsetY )
{
	string   str;
  ostringstream  os;
  GeoPoint p(2);

	if ( points != NULL )
	{
    os << "M ";
    p = points->getEndPunkt();
    os << p.getX() - offsetX << " " << offsetY - p.getY();
    str = os.str();

		str += points->getPointsReverseSVG( offsetX, offsetY );
		str += " Z";
	}
	
	return str;	
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die 2D-Stützpunkte als String			                      				 //
///////////////////////////////////////////////////////////////////////////////
std::string LinearRing::getPointsAsString ( double offsetX, double offsetY )
{
  string         str, strX, strY;
  vector<double> vX, vY;
  String       ^ STR;

  str = "";

  if ( points == NULL )
    return str;

	size_t anz = points->getPoints2D( vX, vY );
  for ( unsigned int i = 0; i < anz; i++ )
  {
    double x = vX[i] - offsetX;
    double y = vY[i] - offsetY;

    STR = Convert::ToString( x );
    QuConvert::systemStr2stdStr( strX, STR );
    STR = Convert::ToString( y );
    QuConvert::systemStr2stdStr( strY, STR );

    str = str + "(" + strX + ", " + strY + ")" + " ";
  }
  return str;

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Umfang des LinearRing										                     //
///////////////////////////////////////////////////////////////////////////////
double LinearRing::getUmfang()
{
	if ( points != NULL ) return points->getLength();
	else				  return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet gleichmäßig verteilte Stützpunkte auf dem LinearRing        	 //
///////////////////////////////////////////////////////////////////////////////
size_t LinearRing::scanRing2D ( std::vector<double> &xP, std::vector<double> &yP, 
						   std::vector<double> &angleP, double scanDist, bool reverse )
{
	if ( points != NULL )
		return points->scanCurve2D ( xP, yP, angleP, scanDist, reverse );
	else
		return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft den LinearRing auf doppelte Punkte, und entfernt diese Punkte  //
//  ggf.                                                                     //
///////////////////////////////////////////////////////////////////////////////
bool LinearRing::checkDoublePoints ( double minDist, bool remove )
{
  if ( points != NULL )
    return points->checkDoublePoints ( minDist, remove );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Affine Transformation eines LinearRing                                   //
///////////////////////////////////////////////////////////////////////////////
void LinearRing::transform ( Matrix4 & matrix )
{
  if ( points != NULL )
  {
    dim = 3;
    points->transform ( matrix );
    bbExist = false;

    calcPlane();   
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Inverse affine Transformation eines LinearRing                           //
///////////////////////////////////////////////////////////////////////////////
void LinearRing::transformInvers ( Matrix4 & matrix )
{
  if ( points != NULL )
  {
    dim = 3;
    points->transformInvers ( matrix );
    bbExist = false;

    calcPlane();   
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert den Ring in die (x,y)-Ebene                                //
///////////////////////////////////////////////////////////////////////////////
_Ring  * LinearRing::transformToHorizontal ()
{
  if ( e == NULL )
  {
    bool success = calcPlane();
    if ( !success )
      return NULL;
  }

  Matrix4  trans = e->getTransformationToHorizontal();

  LinearRing * pTransformedRing = new LinearRing ( this );
  pTransformedRing->transform ( trans );
  pTransformedRing->setDim( 2 );
  pTransformedRing->calcPlane();

  return pTransformedRing;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für die Geometrie ein gültiges                              //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool LinearRing::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  if ( points != NULL )
    return points->existReferenceSystem ( pSrsManager );
  else
    return false;
}


///////////////////////////////////////////////////////////////////////////////
//  Verschiebt den LinearRing um einen Offset                                //
///////////////////////////////////////////////////////////////////////////////
void LinearRing::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  if ( points != NULL && sGeometries.count( points ) == 0 )
    points->offset( point, sGeometries );

  sGeometries.insert( this );
}

///////////////////////////////////////////////////////////////////////////////
//  Implementierung der Klasse gml:Curve									                   //
///////////////////////////////////////////////////////////////////////////////
Curve::Curve( int dimP ): _Curve( dimP )
{
}

Curve::Curve ( Curve * pCurveOld ): _Curve ( pCurveOld )
{
	unsigned int i;

	for ( i = 0; i < pCurveOld->segments.size(); i++ )
	{
		CurveSegment * pCurveSegment    = pCurveOld->segments[i];
		if ( pCurveSegment->getCurveType() == LINE_STRING || 
         pCurveSegment->getCurveType() == LINE_STRING_SEGMENT ||
         pCurveSegment->getCurveType() == LINEAR_RING )      
		{
			LineSegment * pCurveSegmentNew = new LineSegment ( (LineSegment*)pCurveSegment );
			segments.push_back ( pCurveSegmentNew );
		}
		else
		if ( pCurveSegment->getCurveType() == ARC ||
         pCurveSegment->getCurveType() == ARC_STRING ||
				 pCurveSegment->getCurveType() == CIRCLE )
		{
			ArcSegment * pCurveSegmentNew = new ArcSegment ( (ArcSegment*)pCurveSegment );
			segments.push_back ( pCurveSegmentNew );
		}
	}
}

Curve::~Curve()
{
	for ( unsigned int i = 0; i < segments.size(); i++ ) delete segments[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Dreht die Laufrichtung der Linie um										                   //
///////////////////////////////////////////////////////////////////////////////
void Curve::reverse()
{
	size_t         i, ii;
	CurveSegment * pCurveSegmentA;
	CurveSegment * pCurveSegmentB;
	int            anzHalbe = div ( (int)segments.size(), 2 ).quot;

	for ( i = 0; i < anzHalbe; i++ )
	{
		ii = segments.size() - 1 - i;

		pCurveSegmentA = segments[i];
		pCurveSegmentA->reverse();
		pCurveSegmentB = segments[ii];
		pCurveSegmentB->reverse();

		segments[i]  = pCurveSegmentB;
		segments[ii] = pCurveSegmentA;
	}

	if (  div ( (int)segments.size(), 2 ).rem == 1 )
		segments[anzHalbe]->reverse();

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 2D-Stützpunkte				  						                         //
///////////////////////////////////////////////////////////////////////////////
size_t Curve::getPoints2D ( std::vector<double> &vXP, std::vector<double> &vYP )
{
	vector<double> vXloc;
	vector<double> vYloc;
	double		     x, y;
  int            i;
	unsigned int   j, indexAnf;

	for ( i = 0; i < getCurveSegmentAnz(); i++ )
	{
		vXloc.clear();
		vYloc.clear();
		CurveSegment * pSeg = getCurveSegment( i );
		pSeg->getPoints2D ( vXloc, vYloc );
		if ( i == 0 ) indexAnf = 0;
		else		  indexAnf = 1;
		
		for ( j = indexAnf; j < vXloc.size(); j++ )
		{
			x = vXloc[j];
			y = vYloc[j];
			vXP.push_back ( x );
			vYP.push_back ( y );
		}
	}

	return vXP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 2D-Stützpunkte	in umgekehrter Reihenfolge                   //
///////////////////////////////////////////////////////////////////////////////
size_t Curve::getPoints2DReverse ( std::vector<double> &vXP, std::vector<double> &vYP )
{
  vector<double> vXloc;
  vector<double> vYloc;
  double		     x, y;
	int            i;
	size_t         j, indexAnf;

  for ( i = (int)getCurveSegmentAnz()-1; i >= 0; i-- )
  {
    vXloc.clear();
    vYloc.clear();
    CurveSegment * pSeg = getCurveSegment( (size_t)i );
    pSeg->getPoints2DReverse ( vXloc, vYloc );
    if ( i == 0 ) indexAnf = 0;
    else		  indexAnf = 1;

    for ( j = indexAnf; j < vXloc.size(); j++ )
    {
      x = vXloc[j];
      y = vYloc[j];
      vXP.push_back ( x );
      vYP.push_back ( y );
    }
  }

  return vXP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 3D-Stützpunkte				  						                         //
///////////////////////////////////////////////////////////////////////////////
size_t Curve::getPoints3D ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP )
{
  vector<double> vXloc;
  vector<double> vYloc;
  vector<double> vZloc;
  double		     x, y, z;
  int            i;
  unsigned int   j, indexAnf;

  for ( i = 0; i < getCurveSegmentAnz(); i++ )
  {
    vXloc.clear();
    vYloc.clear();
    vZloc.clear();
    CurveSegment * pSeg = getCurveSegment( i );
    pSeg->getPoints3D ( vXloc, vYloc, vZloc );
    if ( i == 0 ) indexAnf = 0;
    else		  indexAnf = 1;

    for ( j = indexAnf; j < vXloc.size(); j++ )
    {
      x = vXloc[j];
      y = vYloc[j];
      z = vZloc[j];
      vXP.push_back ( x );
      vYP.push_back ( y );
      vZP.push_back ( z );
    }
  }

  return vXP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 3D-Stützpunkte	in umgekehrter Reihenfolge                   //
///////////////////////////////////////////////////////////////////////////////
size_t Curve::getPoints3DReverse ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP )
{
  vector<double> vXloc;
  vector<double> vYloc;
  vector<double> vZloc;
  double		     x, y, z;
	int            i;
	size_t         j, indexAnf;

  for ( i = (int)getCurveSegmentAnz()-1; i >= 0; i-- )
  {
    vXloc.clear();
    vYloc.clear();
    vZloc.clear();
    CurveSegment * pSeg = getCurveSegment( (size_t)i );
    pSeg->getPoints3DReverse ( vXloc, vYloc, vZloc );
    if ( i == 0 ) indexAnf = 0;
    else		  indexAnf = 1;

    for ( j = indexAnf; j < vXloc.size(); j++ )
    {
      x = vXloc[j];
      y = vYloc[j];
      z = vZloc[j];
      vXP.push_back ( x );
      vYP.push_back ( y );
      vZP.push_back ( z );
    }
  }

  return vXP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Stützpunkte als GeoPoint Objekte                            //
///////////////////////////////////////////////////////////////////////////////
size_t Curve::getPoints ( std::vector<GeoPoint> &vPoints )
{
  vector<GeoPoint> vPointLoc;
  int              i;
  unsigned int     j, indexAnf;

  for ( i = 0; i < getCurveSegmentAnz(); i++ )
  {
    vPointLoc.clear();
    CurveSegment * pSeg = getCurveSegment( i );
    pSeg->getPoints (vPointLoc );
    if ( i == 0 ) indexAnf = 0;
    else		  indexAnf = 1;

    for ( j = indexAnf; j < vPointLoc.size(); j++ )
      vPoints.push_back ( vPointLoc[j] );
  }

  return vPoints.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Anfangs- und Endpunkt der Curve                                  //
///////////////////////////////////////////////////////////////////////////////
GeoPoint  Curve::getAnfangsPunkt ()
{
  if ( segments.size() > 0 ) return segments[0]->getAnfangsPunkt();
  else					             return GeoPoint(0,0);
}

GeoPoint  Curve::getEndPunkt()
{
  if ( segments.size() > 0 ) return segments[0]->getEndPunkt();
  else					             return GeoPoint(0,0);
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von Punkten zum Einbau in SVG-path							              	 //
///////////////////////////////////////////////////////////////////////////////
string Curve::getPointsSVG ( double offsetX, double offsetY )
{
	string         pathStringSegment;
	ostringstream  os;
	unsigned int   i;
//  GeoPoint       p;
  FIRST_POINT    mode;

  if ( getCurveSegmentAnz() == 0 ) return "";

	for ( i = 0; i < segments.size(); i++ )
	{
    if ( i == 0 ) mode = MOVE;
    else          mode = OMIT;

		CurveSegment * pCurveSegment = segments[i];
		pathStringSegment = pCurveSegment->getSegmentPointsSVG ( offsetX, offsetY, mode ); 
		os << pathStringSegment << " ";
	}
	return os.str();
}

string Curve::getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode )
{
  string         pathStringSegment;
  ostringstream  os;
  unsigned int   i;
  //  GeoPoint       p;
  FIRST_POINT    modeLoc;

  if ( getCurveSegmentAnz() == 0 ) return "";

  for ( i = 0; i < segments.size(); i++ )
  {
    if ( i == 0 ) modeLoc = mode;
    else          modeLoc = OMIT;

    CurveSegment * pCurveSegment = segments[i];
    pathStringSegment = pCurveSegment->getSegmentPointsSVG ( offsetX, offsetY, modeLoc ); 
    os << pathStringSegment << " ";
  }
  return os.str();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefern von Punkten in umgekehrter Reihenfolge zum Einbau in SVG-path  	 //
///////////////////////////////////////////////////////////////////////////////
string Curve::getPointsReverseSVG ( double offsetX, double offsetY )
{
	string         pathStringSegment;
	ostringstream  os;
	int   			   i;
  FIRST_POINT    mode;

  if ( getCurveSegmentAnz() == 0 ) return "";


	for ( i = (int)segments.size()-1; i >= 0 ; i-- )
	{
    if ( i == (int)segments.size()-1 ) mode = MOVE;
    else                               mode = OMIT;

		CurveSegment * pCurveSegment = segments[i];
		pathStringSegment = pCurveSegment->getSegmentPointsReverseSVG ( offsetX, offsetY, mode ); 
		os << pathStringSegment << " ";
	}
	return os.str();
}


string Curve::getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode )
{
  string         pathStringSegment;
  ostringstream  os;
	int   	       i;
  FIRST_POINT    modeLoc;

  if ( getCurveSegmentAnz() == 0 ) return "";


  for ( i = (int)segments.size()-1; i >= 0 ; i-- )
  {
    if ( i == (int)segments.size()-1 ) modeLoc = mode;
    else                               modeLoc = OMIT;

    CurveSegment * pCurveSegment = segments[i];
    pathStringSegment = pCurveSegment->getSegmentPointsReverseSVG ( offsetX, offsetY, modeLoc ); 
    os << pathStringSegment << " ";
  }
  return os.str();
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Bounding-Box								                          		 //
///////////////////////////////////////////////////////////////////////////////
bool Curve::getBoundingBox ( double * sw, double * no )
{
	CurveSegment * pSeg;
	unsigned int   i;
	bool		       success;
	double         swLoc[3], noLoc[3];

  if ( bbExist )
  {
    sw[0] = m_sw[0];
    sw[1] = m_sw[1];
    no[0] = m_no[0];
    no[1] = m_no[1];
    sw[2] = m_sw[2];
    no[2] = m_no[2];
    return true;
  }

	sw[0] =  1.0E20;
	sw[1] =  1.0E20;
	no[0] = -1.0E20;
	no[1] = -1.0E20;
  if ( dim == 3 )
  {
    sw[2] =  1.0E20;
    no[2] = -1.0E20;
  }
  else
  {
    sw[2] = 0.0;
    no[2] = 0.0;
  }

	if ( segments.size() == 0 ) return false;

	for ( i = 0; i < segments.size(); i++ )
	{ 
		pSeg = segments[i];
		success = pSeg->getBoundingBox ( swLoc, noLoc );
		if ( !success ) continue;

		if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
		if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
		if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
		if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( dim == 3 )
    {
      if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
      if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
    }
	}

  m_sw[0] = sw[0];
  m_sw[1] = sw[1];
  m_no[0] = no[0];
  m_no[1] = no[1];
  if ( dim == 3 )
  {
    m_sw[2] = sw[2];
    m_no[2] = no[2];
  }
  else
  {
    m_sw[2] = 0.0;
    m_no[2] = 0.0;
  }
  bbExist = true;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Länge der Kurve									                      		 //
///////////////////////////////////////////////////////////////////////////////
double Curve::getLength()
{
	CurveSegment * pSeg;
	unsigned int   i;
	double		     length = 0.0;

	for ( i = 0; i < segments.size(); i++ )
	{ 
		pSeg = segments[i];
		length = length + pSeg->getLength();
	}

	return length;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den Schwerpunkt der Kurve						                  				 //
///////////////////////////////////////////////////////////////////////////////
bool Curve::getCenter2D ( double &xP, double &yP )
{
	double sw[3], no[3];
	bool   success;

	success = getBoundingBox ( sw, no );
	if ( !success ) return false;

	xP = 0.5*(sw[0] + no[0] );
	yP = 0.5*(sw[1] + no[1] );

	return true;
}

bool Curve::getCenter3D ( double &xP, double &yP, double &zP )
{
  double sw[3], no[3];
  bool   success;

  success = getBoundingBox ( sw, no );
  if ( !success ) return false;

  xP = 0.5*(sw[0] + no[0] );
  yP = 0.5*(sw[1] + no[1] );
  zP = 0.5*(sw[2] + no[2] );

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t Curve::getPointAnz()
{
	size_t anz;
	size_t i;

  anz = 0;

  for ( i = 0; i < segments.size(); i++ )
  {
    CurveSegment * pCurveSegment = segments[i];
    anz = anz + pCurveSegment->getPointAnz();
  }

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der Geometrie ein gültiges                   //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool Curve::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  for ( unsigned int i = 0; i < segments.size(); i++ )
  {
    if ( segments[i]->existReferenceSystem( pSrsManager ) == false )
      return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den BasicCheck für die Curve durch                                 //
///////////////////////////////////////////////////////////////////////////////
bool Curve::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  bool success;
  bool successAll = true;

  if ( segments.size() == 0 )
  {
      GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
      pError->setPropertyName( featureProperty );

      vErrorReport.push_back( pError );
      pError->setFeatureGeometryId ( gmlId );

    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  for ( unsigned int i = 0; i < segments.size(); i++ )
  {
    CurveSegment * pCurveSegment = segments[i];
    success = pCurveSegment->basicCheck ( pObj, featureProperty, pCheckParams, vErrorReport );
    if (!success ) successAll = false;
  }

  if ( successAll )
    checkResult = BASIC_CHECK_FULFILLED;
  else
    checkResult = BASIC_CHECK_FAILED;

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den AdvancedCheck für die Curve durch                              //
///////////////////////////////////////////////////////////////////////////////
bool Curve::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  bool success;
  bool successAll = true;

  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;

  for ( unsigned int i = 0; i < segments.size(); i++ )
  {
    CurveSegment * pCurveSegment = segments[i];
    success = pCurveSegment->advancedCheck ( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
    if (!success ) successAll = false;
  }

  if ( successAll )
    checkResult = ADVANCED_CHECK_FULFILLED;
  else
    checkResult = ADVANCED_CHECK_FAILED;

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt Punkte auf der Kurve mit Abstand scanDist			        		     //
//  Ist die Länge der Kurve kein ganzzahliges Vielfaches von scanDist, wird  //
//  dieser Parameter entsprechend angepasst				                					 //
///////////////////////////////////////////////////////////////////////////////
size_t Curve::scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
					   std::vector<double> &angleP, double scanDist, bool reverse )
{
	size_t         iSeg, iPoint, anz, iAnf, i, anz_h;
	double		     h;
	vector<double> xLoc, yLoc, angleLoc;
	CurveSegment * pSeg;

	for ( iSeg = 0; iSeg < segments.size(); iSeg++ )
	{
		pSeg = segments[iSeg];
		xLoc.clear();
		yLoc.clear();
		angleLoc.clear();
		pSeg->scanCurve2D( xLoc, yLoc, angleLoc, scanDist, true );

		if ( iSeg == 0 ) iAnf = 0;
		else	 	     iAnf = 1;
		for ( iPoint = iAnf; iPoint < xLoc.size(); iPoint++ )
		{
			xP.push_back ( xLoc[iPoint] );
			yP.push_back ( yLoc[iPoint] );
			angleP.push_back ( angleLoc[iPoint] );
		}
	}

	if ( reverse )
	{
		anz   = xP.size();
		anz_h = div ( (int)anz, 2 ).quot;
		for ( i = 0; i < anz_h; i++ )
		{
			h = xP[i];
			xP[i] = xP[anz-i-1];
			xP[anz-i-1] = h;

			h = yP[i];
			yP[i] = yP[anz-i-1];
			yP[anz-i-1] = h;

			h = angleP[i];
			
			angleP[i] = angleP[anz-i-1] + PI;
			angleP[anz-i-1] = h + PI;
		}
		if ( div ( (int)anz, 2 ).rem == 1 )
		{
			angleP[anz_h] = angleP[anz_h] + PI;
		}
	}

	return xP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Curve auf aufeinander folgende, doppelte Stützpunkte und   //
//  entfernt diese ggf.                                                      //
///////////////////////////////////////////////////////////////////////////////
bool Curve::checkDoublePoints ( double minDist, bool remove )
{
  unsigned int i;
  bool         success = true;
  bool         successLoc;

  for ( i = 0; i < segments.size(); i++ )
  {
    successLoc = segments[i]->checkDoublePoints( minDist, remove );
    if ( !successLoc )
      success = false;
  }
  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Affine Transformation der Curve                                          //
///////////////////////////////////////////////////////////////////////////////
void Curve::transform ( Matrix4 & matrix )
{
  for ( unsigned int i = 0; i < segments.size(); i++ )
    segments[i]->transform( matrix );
}

///////////////////////////////////////////////////////////////////////////////
//  Inverse affine Transformation der Curve                                  //
///////////////////////////////////////////////////////////////////////////////
void Curve::transformInvers ( Matrix4 & matrix )
{
  for ( unsigned int i = 0; i < segments.size(); i++ )
    segments[i]->transformInvers( matrix );
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt die Curve um einen Offset                                     //
///////////////////////////////////////////////////////////////////////////////
void Curve::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  for ( unsigned int i = 0; i < segments.size(); i++ )
  {
    if ( sGeometries.count( segments[i] ) == 0 )
      segments[i]->offset ( point, sGeometries );
  }
  sGeometries.insert( this );
}

///////////////////////////////////////////////////////////////////////////////
//  OrientableCurve																													 //
//  Implementierung der Klasse gml:OrientableCurve				                   //
///////////////////////////////////////////////////////////////////////////////
OrientableCurve::OrientableCurve( int dimP ): _Curve( dimP )
{
	pBaseCurve  = NULL;
	orientation = true;
}

OrientableCurve::OrientableCurve ( OrientableCurve * pCurveOld ): _Curve ( pCurveOld )
{
	orientation = pCurveOld->orientation;

	if ( pCurveOld->pBaseCurve != NULL )
		pBaseCurve = dynamic_cast<_Curve*>(_Geometrie::copy(pCurveOld->pBaseCurve));
	else
		pBaseCurve = NULL;
}

OrientableCurve::~OrientableCurve()
{
	if ( pBaseCurve != NULL ) delete pBaseCurve;
}

///////////////////////////////////////////////////////////////////////////////
//  Dreht die Laufrichtung der OrientableCurve um					                   //
///////////////////////////////////////////////////////////////////////////////
void OrientableCurve::reverse()
{
	if ( orientation == true )
		orientation = false;
	else
		orientation = true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 2D-Stützpunkte				  						                         //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableCurve::getPoints2D ( std::vector<double> &vXP, std::vector<double> &vYP )
{
	if ( pBaseCurve == NULL )
		return 0;

	if ( orientation == true )
		return pBaseCurve->getPoints2D ( vXP, vYP );
	else
		return pBaseCurve->getPoints2DReverse ( vXP, vYP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 2D-Stützpunkte	in umgekehrter Reihenfolge                   //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableCurve::getPoints2DReverse ( std::vector<double> &vXP, std::vector<double> &vYP )
{
	if ( pBaseCurve == NULL )
		return 0;

	if ( orientation == false )
		return pBaseCurve->getPoints2D ( vXP, vYP );
	else
		return pBaseCurve->getPoints2DReverse ( vXP, vYP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 3D-Stützpunkte				  						                         //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableCurve::getPoints3D ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP )
{
	if ( pBaseCurve == NULL )
		return 0;

	if ( orientation == true )
		return pBaseCurve->getPoints3D ( vXP, vYP, vZP );
	else
		return pBaseCurve->getPoints3DReverse ( vXP, vYP, vZP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle 3D-Stützpunkte	in umgekehrter Reihenfolge                   //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableCurve::getPoints3DReverse ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP )
{
	if ( pBaseCurve == NULL )
		return 0;

	if ( orientation == false )
		return pBaseCurve->getPoints3D ( vXP, vYP, vZP );
	else
		return pBaseCurve->getPoints3DReverse ( vXP, vYP, vZP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Stützpunkte als GeoPoint Objekte                            //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableCurve::getPoints ( std::vector<GeoPoint> &vPoints )
{
	if ( pBaseCurve == NULL )
		return 0;

	pBaseCurve->getPoints ( vPoints );
	if ( orientation == false )
		std::reverse ( vPoints.begin(), vPoints.end() );

	return vPoints.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Anfangs- und Endpunkt der OrientableCurve                        //
///////////////////////////////////////////////////////////////////////////////
GeoPoint  OrientableCurve::getAnfangsPunkt ()
{
	if ( pBaseCurve == NULL )
		return GeoPoint(0,0);

	if ( orientation == true )
		return pBaseCurve->getAnfangsPunkt();
	else
		return pBaseCurve->getEndPunkt();
}

GeoPoint  OrientableCurve::getEndPunkt()
{
	if ( pBaseCurve == NULL )
		return GeoPoint(0,0);

	if ( orientation == false )
		return pBaseCurve->getAnfangsPunkt();
	else
		return pBaseCurve->getEndPunkt();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von Punkten zum Einbau in SVG-path							              	 //
///////////////////////////////////////////////////////////////////////////////
string OrientableCurve::getPointsSVG ( double offsetX, double offsetY )
{
	if ( pBaseCurve == NULL )
		return "";

	if ( orientation == true )
		return pBaseCurve->getPointsSVG ( offsetX, offsetY );
	else
		return pBaseCurve->getPointsReverseSVG ( offsetX, offsetY );
}

string OrientableCurve::getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode )
{
	if ( pBaseCurve == NULL )
		return "";

	if ( orientation == true )
		return pBaseCurve->getSegmentPointsSVG ( offsetX, offsetY, mode );
	else
		return pBaseCurve->getSegmentPointsReverseSVG ( offsetX, offsetY, mode );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von Punkten in umgekehrter Reihenfolge zum Einbau in SVG-path  	 //
///////////////////////////////////////////////////////////////////////////////
string OrientableCurve::getPointsReverseSVG ( double offsetX, double offsetY )
{
	if ( pBaseCurve == NULL )
		return "";

	if ( orientation == false )
		return pBaseCurve->getPointsSVG ( offsetX, offsetY );
	else
		return pBaseCurve->getPointsReverseSVG ( offsetX, offsetY );
}

string OrientableCurve::getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode )
{
	if ( pBaseCurve == 0 )
		return "";

	if ( orientation == false )
		return pBaseCurve->getSegmentPointsSVG ( offsetX, offsetY, mode );
	else
		return pBaseCurve->getSegmentPointsReverseSVG ( offsetX, offsetY, mode );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die BoundingBox                                                  //
///////////////////////////////////////////////////////////////////////////////
bool OrientableCurve::getBoundingBox ( double * sw, double * no )
{
	if ( pBaseCurve != NULL )
		return pBaseCurve->getBoundingBox ( sw, no );
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Länge der Kurve									                      		 //
///////////////////////////////////////////////////////////////////////////////
double OrientableCurve::getLength()
{
	if ( pBaseCurve == NULL )
		return 0.0;
	else
		return pBaseCurve->getLength();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt                                                  //
///////////////////////////////////////////////////////////////////////////////
bool OrientableCurve::getCenter2D ( double &cxP, double &cyP )
{
	if ( pBaseCurve != NULL )
		return pBaseCurve->getCenter2D ( cxP, cyP );
	else
		return false;
}

bool OrientableCurve::getCenter3D ( double &cxP, double &cyP, double &czP )
{
	if ( pBaseCurve != NULL )
		return pBaseCurve->getCenter3D ( cxP, cyP, czP );
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Kurven-Segmente                                   //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableCurve::getCurveSegmentAnz()
{
	if ( pBaseCurve == NULL )
		return 0;
	else
		return pBaseCurve->getCurveSegmentAnz();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableCurve::getPointAnz()
{
	if ( pBaseCurve == NULL )
		return 0;
	else
		return pBaseCurve->getPointAnz();
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der Geometrie ein gültiges                   //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool OrientableCurve::existReferenceSystem( SrsManager * pSrsManager )
{
	if ( srsName != "" )
		return pSrsManager->existCoordSystem( srsName );

	if ( pBaseCurve == NULL )
		return true;

	else
		return pBaseCurve->existReferenceSystem ( pSrsManager );
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den BasicCheck für die OrientableCurve durch                       //
///////////////////////////////////////////////////////////////////////////////
bool OrientableCurve::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
	bool successAll = true;

	if ( pBaseCurve == NULL )
	{
		GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
		pError->setPropertyName( featureProperty );

		vErrorReport.push_back( pError );
		pError->setFeatureGeometryId ( gmlId );

		checkResult = BASIC_CHECK_FAILED;
		return false;
	}

	return pBaseCurve->basicCheck ( pObj, featureProperty, pCheckParams, vErrorReport );
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den AdvancedCheck für die Curve durch                              //
///////////////////////////////////////////////////////////////////////////////
bool OrientableCurve::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
	vector<Error*> &vErrorReport, bool checkAnyway )
{
	bool successAll = true;

	if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
		return false;

	if ( pBaseCurve == NULL )
	{
		GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
		pError->setPropertyName( featureProperty );

		vErrorReport.push_back( pError );
		pError->setFeatureGeometryId ( gmlId );

		checkResult = BASIC_CHECK_FAILED;
		return false;
	}

	return pBaseCurve->advancedCheck ( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt Punkte auf der Kurve mit Abstand scanDist			        		     //
//  Ist die Länge der Kurve kein ganzzahliges Vielfaches von scanDist, wird  //
//  dieser Parameter entsprechend angepasst				                					 //
///////////////////////////////////////////////////////////////////////////////
size_t OrientableCurve::scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
	std::vector<double> &angleP, double scanDist, bool reverse )
{
	if ( pBaseCurve == NULL )
		return 0;

	if ( orientation == false )
		reverse = !reverse;

	return pBaseCurve->scanCurve2D ( xP, yP, angleP, scanDist, reverse );
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Curve auf aufeinander folgende, doppelte Stützpunkte und   //
//  entfernt diese ggf.                                                      //
///////////////////////////////////////////////////////////////////////////////
bool OrientableCurve::checkDoublePoints ( double minDist, bool remove )
{
	if ( pBaseCurve == NULL )
		return true;
	else
		return pBaseCurve->checkDoublePoints ( minDist, remove );
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt die OrientableCurve um einen Offset                           //
///////////////////////////////////////////////////////////////////////////////
void OrientableCurve::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
	if ( pBaseCurve != NULL )
		pBaseCurve->offset ( point, sGeometries );
}





///////////////////////////////////////////////////////////////////////////////
//  Implementierung der Klasse gml:CurveSegment							              	 //
///////////////////////////////////////////////////////////////////////////////
CurveSegment::CurveSegment( int dimP ): _Curve ( dimP )
{
	interpolation = "";
}

CurveSegment::CurveSegment( CurveSegment * pCurveSegmentOld ): _Curve ( pCurveSegmentOld )
{
	unsigned int i;

	for ( i = 0; i < pCurveSegmentOld->points.size(); i++ )
	{
		GeoPoint point = pCurveSegmentOld->points[i];
		points.push_back ( point );
	}
	interpolation = pCurveSegmentOld->interpolation;
}

CurveSegment::CurveSegment( const CurveSegment & curveSegmentOld ) : _Curve ( curveSegmentOld )
{
  unsigned int i;

  for ( i = 0; i < curveSegmentOld.points.size(); i++ )
  {
    GeoPoint point = curveSegmentOld.points[i];
    points.push_back ( point );
  }
	interpolation = curveSegmentOld.interpolation;
}

CurveSegment::~CurveSegment()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen von 2D-Punkten				  							                         //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment::addPoints2D ( std::vector<double> xCoordsP, std::vector<double> yCoordsP )
{
	for ( unsigned int i = 0; i < xCoordsP.size(); i++ )
	{
		GeoPoint * p = new GeoPoint ( xCoordsP[i], yCoordsP[i] );
		points.push_back ( p );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen von 3D-Punkten				  							                         //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment::addPoints3D ( std::vector<double> xCoordsP, std::vector<double> yCoordsP, std::vector<double> zCoordsP )
{
  for ( unsigned int i = 0; i < xCoordsP.size(); i++ )
  {
    GeoPoint * p = new GeoPoint ( xCoordsP[i], yCoordsP[i], zCoordsP[i] );
    points.push_back ( p );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen von GeoPoint Objekten   							                         //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment::addPoints ( std::vector<GeoPoint> &vPoints )
{
  for ( unsigned int i = 0; i < vPoints.size(); i++ )
  {
    points.push_back ( vPoints[i] );
  }
}

void CurveSegment::addPoints ( std::deque<GeoPoint> &dPoints )
{
  for ( unsigned int i = 0; i < dPoints.size(); i++ )
  {
    points.push_back ( dPoints[i] );
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines 2D-Punktes			  							                         //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment::addPoint2D ( double x, double y )
{
  GeoPoint * p = new GeoPoint ( x, y );
  points.push_back ( p );
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines 3D-Punktes			  							                         //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment::addPoint3D ( double x, double y, double z )
{
  GeoPoint * p = new GeoPoint ( x, y, z );
  points.push_back ( p );
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines GeoPoint Punktes			  				                         //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment::addPoint ( GeoPoint &point )
{
  points.push_back ( point );
}


///////////////////////////////////////////////////////////////////////////////
//  Prüft das CurveSegment auf hintereinander auftretende, doppelte          //
//  Stützpunkte und entfernt diese ggf.                                      //
///////////////////////////////////////////////////////////////////////////////
bool CurveSegment::checkDoublePoints ( double minDist, bool remove )
{
  size_t          i, j, anz;
  GeoPoint     p1(dim), p2(dim);
  double       dist;
  bool         doublePointsExist = false;

  anz = points.size();
  for ( i = 0; i < anz-1; i++ )
  {
    p1 = points[i];
    p2 = points[i+1];
    dist = p1.dist ( p2 );

    if ( dist <= minDist )
    {
      if ( remove )
      {
        for ( j = i + 1; j < anz; j++  )
        {
          points[j-1] = points[j];
        }
        points.pop_back();
        anz = anz-1;
        i = i - 1;
      }
      doublePointsExist = true;
    }
  }
  return doublePointsExist;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von 2D-Punkten			  								                           //
///////////////////////////////////////////////////////////////////////////////
size_t CurveSegment::getPoints2D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP )
{
	xCoordsP.clear();
	yCoordsP.clear();

	for ( unsigned int i = 0; i < points.size(); i++ )
	{
		GeoPoint p = points[i];
		xCoordsP.push_back ( p.getX() );
		yCoordsP.push_back ( p.getY() );
	}
	return points.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von 2D-Punkten in umgekehrter Reihenfolge                        //
///////////////////////////////////////////////////////////////////////////////
size_t CurveSegment::getPoints2DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP )
{
  xCoordsP.clear();
  yCoordsP.clear();

  for ( int i = (int)points.size()-1; i >= 0; i-- )
  {
    GeoPoint p = points[i];
    xCoordsP.push_back ( p.getX() );
    yCoordsP.push_back ( p.getY() );
  }
  return points.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von 3D-Punkten			  								                           //
///////////////////////////////////////////////////////////////////////////////
size_t CurveSegment::getPoints3D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP )
{
  xCoordsP.clear();
  yCoordsP.clear();
  zCoordsP.clear();

  for ( unsigned int i = 0; i < points.size(); i++ )
  {
    GeoPoint p = points[i];
    xCoordsP.push_back ( p.getX() );
    yCoordsP.push_back ( p.getY() );
    zCoordsP.push_back ( p.getZ() );
  }
  return points.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von 3D-Punkten in umgekehrter Reihenfolge                        //
///////////////////////////////////////////////////////////////////////////////
size_t CurveSegment::getPoints3DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP )
{
  xCoordsP.clear();
  yCoordsP.clear();
  zCoordsP.clear();

  int anz = (int)points.size()-1;

  for ( int i =  anz; i >= 0; i-- )
  {
    GeoPoint p = points[i];
    xCoordsP.push_back ( p.getX() );
    yCoordsP.push_back ( p.getY() );
    zCoordsP.push_back ( p.getZ() );
  }
  return points.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von GeoPoint Objekten  								                           //
///////////////////////////////////////////////////////////////////////////////
size_t CurveSegment::getPoints ( std::vector<GeoPoint> &vPoints )
{
  for ( size_t i = 0; i < points.size(); i++ )
  {
    GeoPoint p = points[i];
    vPoints.push_back( p );
  }

  return vPoints.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Kopiert die Stützpunkte zwischen index1 und index2 in den vector vPoints //
//  Wenn extract == true werden die Originalpunkte aus dem CurveSegment      //
//  entfernt.                                                                //
///////////////////////////////////////////////////////////////////////////////
size_t CurveSegment::getPoints ( size_t index1, size_t index2, std::vector<GeoPoint> &vPoints, bool extract  )
{
  if ( index1 < 0 || index2 < 0 || index1 > points.size() || index2 >  points.size() || index1 >= index2 )
    return 0;

  for ( size_t i = index1; i < index2; i++ )
    vPoints.push_back ( points[i] );

  if ( extract )
    points.erase( points.begin() + index1, points.begin() + index2 );

  return vPoints.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Punkt an einer bestimmen Indexposition                       //
///////////////////////////////////////////////////////////////////////////////
GeoPoint CurveSegment::getPoint ( size_t index )
{
  if ( index < 0 || index >= points.size() )
    return GeoPoint ( dim );
  else
    return points[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Überschreibt einen Punkt an einer bestimmten Indexposition               //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment::setPoint ( unsigned int index, GeoPoint & point )
{
  if ( index < 0 || index >= points.size() )
    return;

  points[index] = point;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Anfangs- bzw. Endpunkt des Segments		            					 //
///////////////////////////////////////////////////////////////////////////////
GeoPoint CurveSegment::getAnfangsPunkt ()
{
	if ( points.size() > 0 ) return points[0];
	else					           return GeoPoint(0,0);
}

GeoPoint  CurveSegment::getEndPunkt()
{
	if ( points.size() > 0 ) return points[points.size()-1];
	else					           return GeoPoint (0,0);
}

///////////////////////////////////////////////////////////////////////////////
//  Kehrt die Laufrichtung des Segments um		                							 //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment::reverse()
{
	size_t    i, ii;
	GeoPoint  p(dim);
	int       anzHalbe = div ( (int)points.size(), 2 ).quot;

	for ( i = 0; i < anzHalbe; i++ )
	{
		ii = points.size() - 1 - i;

		p          = points[i];
		points[i]  = points[ii];
		points[ii] = p;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Fügt pSegP am Ende an und lässt des ersten Punkt weg				          		 //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment::join ( CurveSegment * pSegP )
{
	unsigned int i;
	for ( i = 1; i < pSegP->points.size(); i++ )
	{
		GeoPoint p    =  pSegP->points[i];
		points.push_back ( p );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die BoundingBox								                        					 //
///////////////////////////////////////////////////////////////////////////////
bool CurveSegment::getBoundingBox ( double * sw, double * no )
{
	vector<double> vX, vY, vZ;
	size_t         i, anz;

  if ( bbExist )
  {
    sw[0] = m_sw[0];
    sw[1] = m_sw[1];
    no[0] = m_no[0];
    no[1] = m_no[1];
    sw[2] = m_sw[2];
    no[2] = m_no[2];
    return true;
  }

	sw[0] =  1.0E20;
	sw[1] =  1.0E20;
	no[0] = -1.0E20;
	no[1] = -1.0E20;
  if ( dim == 3 )
  {
    sw[2] =  1.0E20;
    no[2] = -1.0E20;
  }
  else
  {
    sw[2] = 0.0;
    no[2] = 0.0;
  }

  if ( dim == 2 )
	  anz = getPoints2D ( vX, vY );
  else
    anz = getPoints3D ( vX, vY, vZ );

  if ( anz == 0 ) 
    return false;

	for ( i = 0; i < anz; i++ )
	{
		if ( vX[i]< sw[0] ) sw[0] = vX[i];
		if ( vY[i]< sw[1] ) sw[1] = vY[i];
		if ( vX[i]> no[0] ) no[0] = vX[i];
		if ( vY[i]> no[1] ) no[1] = vY[i];
    if ( dim == 3 )
    {
      if ( vZ[i]< sw[2] ) sw[2] = vZ[i];
      if ( vZ[i]> no[2] ) no[2] = vZ[i];
    }
	}

  m_sw[0] = sw[0];
  m_sw[1] = sw[1];
  m_no[0] = no[0];
  m_no[1] = no[1];
  if ( dim == 3 )
  {
    m_sw[2] = sw[2];
    m_no[2] = no[2];
  }
  else
  {
    m_sw[2] = 0.0;
    m_no[2] = 0.0;
  }
  bbExist = true;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den 2D-Schwerpunkt der Kurv				                    					 //
///////////////////////////////////////////////////////////////////////////////
bool CurveSegment::getCenter2D ( double &xP, double &yP )
{
	double sw[3], no[3];
	bool   success;

	success = getBoundingBox ( sw, no );
	if ( !success ) return false;

	xP = 0.5*(sw[0] + no[0] );
	yP = 0.5*(sw[1] + no[1] );

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den 3D-Schwerpunkt der Kurv				                    					 //
///////////////////////////////////////////////////////////////////////////////
bool CurveSegment::getCenter3D ( double &xP, double &yP, double &zP )
{
  double sw[3], no[3];
  bool   success;

  success = getBoundingBox ( sw, no );
  if ( !success ) return false;

  xP = 0.5*(sw[0] + no[0] );
  yP = 0.5*(sw[1] + no[1] );
  zP = 0.5*(sw[2] + no[2] );

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für die Geometrie ein gültiges                              //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool CurveSegment::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName == "" )
    return false;

  return pSrsManager->existCoordSystem( srsName );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die z-Koordinaten aller Stützpunkte auf einen konstanten Wert      //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment::setZKoordinate ( double zP )
{
  for ( unsigned int i = 0; i < points.size(); i++ )
  {
    points[i].setZ( zP );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt das CurveSegment um einen Offset                              //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  for ( unsigned int i = 0; i < points.size(); i++ )
  {
    points[i].offset( point, sGeometries );
  }
  sGeometries.insert( this );
}

///////////////////////////////////////////////////////////////////////////////
//  Implementierung der Klasse gml:LineSegment						               		 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor											                        	 //
///////////////////////////////////////////////////////////////////////////////
LineSegment::LineSegment( int dimP, LINE_SEGMENT_TYPE lineSegmentTypeP ) : CurveSegment( dimP )
{
  lineSegmentType = lineSegmentTypeP;
}

LineSegment::LineSegment( LineSegment * pLineSegmentOld ):CurveSegment ( pLineSegmentOld )
{
  lineSegmentType = pLineSegmentOld->lineSegmentType;
}

LineSegment::LineSegment ( const LineSegment & lineSegmentOld ) : CurveSegment ( lineSegmentOld )
{
  lineSegmentType = lineSegmentOld.lineSegmentType;
}

LineSegment::~LineSegment()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von 2D-Punkten zum Einbau in SVG-path			              				 //
///////////////////////////////////////////////////////////////////////////////
string LineSegment::getPointsSVG ( double offsetX, double offsetY )
{
	string         svgPath;
	ostringstream  os;
	unsigned int	 i;

	for ( i = 0; i < points.size(); i++ )
	{
    if ( i == 0 )	os << " M ";
    else          os << " L ";

		GeoPoint pPoint = points[i];
		os << pPoint.getX() - offsetX << " " << offsetY - pPoint.getY();
	}

	svgPath = os.str();
	return svgPath;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von 2D-Punkten in umgekehrter Reihenfolge zum Einbau in SVG-path //
///////////////////////////////////////////////////////////////////////////////
string LineSegment::getPointsReverseSVG ( double offsetX, double offsetY )
{
	string         svgPath;
	ostringstream  os;
	int            i;

	for ( i = (int)points.size() - 1; i >= 0; i-- )
	{
    if ( i == (int)points.size() - 1 ) os << " M ";
    else                               os << " L ";

		GeoPoint pPoint = points[i];
		os << pPoint.getX() - offsetX << " " << offsetY - pPoint.getY();
	}

	svgPath = os.str();
	return svgPath;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von 2D-Punkten zum Einbau in SVG-path			              				 //
///////////////////////////////////////////////////////////////////////////////
string LineSegment::getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode )
{
  string         svgPath;
  ostringstream  os;
  unsigned int	 i;

  for ( i = 0; i < points.size(); i++ )
  {
    if ( i == 0 )
    {  
      if ( mode == MOVE ) os << " M ";
      else continue;
    }
    else os << " L ";

    GeoPoint pPoint = points[i];
    os << pPoint.getX() - offsetX << " " << offsetY - pPoint.getY();
  }

  svgPath = os.str();
  return svgPath;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von 2D-Punkten in umgekehrter Reihenfolge zum Einbau in SVG-path //
///////////////////////////////////////////////////////////////////////////////
string LineSegment::getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode )
{
  string         svgPath;
  ostringstream  os;
	int            i;

  for ( i = (int)points.size() - 1; i >= 0; i-- )
  {
    if ( i == (int)points.size() - 1 )
    {
      if ( mode == MOVE ) os << " M ";
      else continue;
    }
    else os << " L ";

    GeoPoint pPoint = points[i];
    os << pPoint.getX() - offsetX << " " << offsetY - pPoint.getY();
  }

  svgPath = os.str();
  return svgPath;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Länge des Segmentes					                    					 //
///////////////////////////////////////////////////////////////////////////////
double LineSegment::getLength()
{
	unsigned int i;
	double       length = 0.0;
	GeoPoint     pA(dim);
	GeoPoint     pB(dim);

	if ( points.size() <= 1 ) return 0.0;

	pA = points[0];
	for ( i = 1; i < points.size(); i++ )
	{
		pB = points[i];
		length = length + (pB.getX() - pA.getX())*(pB.getX() - pA.getX()) +
								 (pB.getY() - pA.getY())*(pB.getY() - pA.getY());
    if ( dim == 3 )
      length = length + (pB.getZ() - pA.getZ())*(pB.getZ() - pA.getZ());
		pA = pB;
	}
	return sqrt ( length );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anfangs- bzw. End-Steigung des Segments						           //
///////////////////////////////////////////////////////////////////////////////
double LineSegment::getAnfangsSteigung2D()
{
	GeoPoint  pAnf(dim);
	GeoPoint  pEnd(dim);
	size_t    anz;
	double    len, steigung, pRicht[2];

	anz = points.size();
	if ( anz <= 1 ) return 0.0;

	pAnf = points[0];
	pEnd = points[1];

	len = sqrt ( (pEnd.getX()-pAnf.getX())*(pEnd.getX()-pAnf.getX()) +
				  (pEnd.getY()-pAnf.getY())*(pEnd.getY()-pAnf.getY()) );

	pRicht[0] = (pEnd.getX()-pAnf.getX())/len;
	pRicht[1] = (pEnd.getY()-pAnf.getY())/len;

	steigung = atan2 ( pRicht[1], pRicht[0] );

	return steigung;
}

double LineSegment::getEndSteigung2D()
{
	GeoPoint pAnf(dim);
	GeoPoint pEnd(dim);
	size_t   anz;
	double  len, steigung, pRicht[2];

	anz = points.size();
	if ( anz <= 1 ) return 0.0;

	pAnf = points[anz-2];
	pEnd = points[anz-1];

	len = sqrt ( (pEnd.getX()-pAnf.getX())*(pEnd.getX()-pAnf.getX()) +
				       (pEnd.getY()-pAnf.getY())*(pEnd.getY()-pAnf.getY()) );

	pRicht[0] = (pEnd.getX()-pAnf.getX())/len;
	pRicht[1] = (pEnd.getY()-pAnf.getY())/len;

	steigung = atan2 ( pRicht[1], pRicht[0] );

	return steigung;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt Punkte auf der Kurve mit Abstand scanDist		        			     //
//  Ist die Länge der Kurve kein ganzzahliges Vielfaches von scanDist, wird  //
//  dieser Parameter entsprechend angepasst									                 //
///////////////////////////////////////////////////////////////////////////////
size_t LineSegment::scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
				   std::vector<double> &angleP, double scanDist, bool reverse )
{
	double         laengeSegment, x, y, steigung, steigungEcke, laengeSegmentVorher, scanDistLoc;
	double         pAnf[2], pEnd[2], pRicht[2];
	double         pAnfVorher[2], pEndVorher[2], pRichtVorher[2], pRichtEcke[2];
	vector<double> xCoords, yCoords;
	size_t         iSeg, iPoint, anz, anzLoc;

	anz = getPoints2D ( xCoords, yCoords );

	for ( iSeg = 0; iSeg < anz-1; iSeg++ )
	{
		pAnf[0] = xCoords[iSeg];
		pAnf[1] = yCoords[iSeg];
		pEnd[0] = xCoords[iSeg+1];
		pEnd[1] = yCoords[iSeg+1];

		if ( iSeg == 0 )
		{
			pAnfVorher[0] = xCoords[anz-2];
			pAnfVorher[1] = yCoords[anz-2];
			pEndVorher[0] = xCoords[0];
			pEndVorher[1] = yCoords[0];
		}
		else
		{
			pAnfVorher[0] = xCoords[iSeg-1];
			pAnfVorher[1] = yCoords[iSeg-1];
			pEndVorher[0] = xCoords[iSeg];
			pEndVorher[1] = yCoords[iSeg];
		}

		laengeSegment = sqrt ( (pEnd[0]-pAnf[0])*(pEnd[0]-pAnf[0]) +
							   (pEnd[1]-pAnf[1])*(pEnd[1]-pAnf[1]) );
		laengeSegmentVorher = sqrt ( (pEndVorher[0]-pAnfVorher[0])*(pEndVorher[0]-pAnfVorher[0]) +
							  	     (pEndVorher[1]-pAnfVorher[1])*(pEndVorher[1]-pAnfVorher[1]) );


		if ( laengeSegment > 0 )
		{
			pRicht[0] = (pEnd[0]-pAnf[0])/laengeSegment;
			pRicht[1] = (pEnd[1]-pAnf[1])/laengeSegment;

			pRichtVorher[0] = (pEndVorher[0]-pAnfVorher[0])/laengeSegmentVorher;
			pRichtVorher[1] = (pEndVorher[1]-pAnfVorher[1])/laengeSegmentVorher;

			pRichtEcke[0] = ( pRicht[0] + pRichtVorher[0] ) / sqrt ( 2.0 );
			pRichtEcke[1] = ( pRicht[1] + pRichtVorher[1] ) / sqrt ( 2.0 );

			steigung	   = atan2 ( pRicht[1], pRicht[0] );
			steigungEcke = atan2 ( pRichtEcke[1], pRichtEcke[0] );

			anzLoc = int ( floor ( laengeSegment / scanDist + 0.5 ) );
			if ( anzLoc < 1 ) anzLoc = 1;
			scanDistLoc = laengeSegment / anzLoc;

			
			for ( iPoint = 0; iPoint < anzLoc; iPoint++ )
			{
				x = pAnf[0]  + (iPoint+0.5)*pRicht[0]*scanDistLoc;
				y = pAnf[1]  + (iPoint+0.5)*pRicht[1]*scanDistLoc;

				if ( iPoint != 0 )
				{
					xP.push_back ( x );
					yP.push_back ( y );
					angleP.push_back ( steigung );
				}
				else
				{
					xP.push_back ( x );
					yP.push_back ( y );
					angleP.push_back ( steigung );
				}
			}
		}

		}

	if ( reverse )
	{
		double h;
		int    i, anz_h;
		const double pi = 4.0*atan ( 1.0 );

		anz   = xP.size();
		anz_h = div ( (int)anz, 2 ).quot;
		for ( i = 0; i < anz_h; i++ )
		{
			h = xP[i];
			xP[i] = xP[anz-i-1];
			xP[anz-i-1] = h;

			h = yP[i];
			yP[i] = yP[anz-i-1];
			yP[anz-i-1] = h;

			h = angleP[i];
			
			angleP[i] = angleP[anz-i-1] + pi;
			angleP[anz-i-1] = h + pi;
		}
		if ( div ( (int)anz, 2 ).rem == 1 )
		{
			angleP[anz_h] = angleP[anz_h] + pi;
		}
	}

	return xP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Affine Transformation                                                    //
///////////////////////////////////////////////////////////////////////////////
void LineSegment::transform ( Matrix4 & matrix )
{
  unsigned int i;
  GeoPoint     point (3);

  bbExist = false;
  dim     = 3;

  for ( i = 0; i < points.size(); i++ )
  {
    point = points[i];
    point = matrix.transform( point );
    points[i] = point;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Inverse affine Transformation                                            //
///////////////////////////////////////////////////////////////////////////////
void LineSegment::transformInvers ( Matrix4 & matrix )
{
  unsigned int i;
  GeoPoint     point (3);

  bbExist = false;
  dim     = 3;

  for ( i = 0; i < points.size(); i++ )
  {
    point = points[i];
    point = matrix.transformInvers( point );
    points[i] = point;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Extrudiert die einzelnen Liniensegmente zu Zylinderflächen               //
///////////////////////////////////////////////////////////////////////////////
_Geometrie * LineSegment::extrude ( double r, int anz, bool generateVolume )
{
  unsigned int     i;
  int              j;
  GeoPoint         pA, pE;
  GeoPoint         nLineA, nLineE;
  GeoPoint         nArea;
  GeoPoint         pLine;
  GeoPoint         pArea;
  GeoPoint         p;
  GeoPoint         s1, s2, s3, s4;
  PolygonGeo        * pPoly;
  LinearRing     * pRing;
  vector<GeoPoint> vPointsA;
  vector<GeoPoint> vPointsE;
  vector<GeoPoint> vPolyPoints;
  vector<PolygonGeo*> vPolygone;

  pA = points[0];
  pE = points[1];
  nLineA =  pE - pA;
  nLineA.normalize();

  _Geometrie::generateNPolygon( pA, r, nLineA, anz, vPointsA );
  if ( generateVolume )
  {
    pPoly = new PolygonGeo ( 3 );
    vPolygone.push_back( pPoly );

    pRing = new LinearRing ( 3 );
    pPoly->setAussenkontur( pRing );

    for ( j = 0; j < anz; j++  )
      vPolyPoints.push_back( vPointsA[j] );
    vPolyPoints.push_back( vPointsA[0] );
    pRing->addPoints( vPolyPoints );
  }

  pA = pE;

  for ( i = 1; i < points.size(); i++ )
  {
    if ( i < points.size() - 1 )
    {
      pE = points[i+1];
      nLineE =  pE - pA;
      nLineE.normalize();

      nArea = nLineA + nLineE;
      nArea.normalize();
    }
    else
      nArea = nLineA;

    for ( j = 0; j < anz; j++ )
    {
      pLine = vPointsA[j];
      _Geometrie::lineAreaCut( pLine, nLineA, pA, nArea, p );
      vPointsE.push_back( p );
    }

    //  Zylindermantel Erzeugen
    for ( j = 0; j < anz; j++ )
    {
      s1 = vPointsA[j];
      s2 = vPointsE[j];
      if ( j < anz - 1 )
      {
        s4 = vPointsA[j+1];
        s3 = vPointsE[j+1];
      }
      else
      {
        s4 = vPointsA[0];
        s3 = vPointsE[0];
      }
      vPolyPoints.clear();
      pPoly = new PolygonGeo ( 3 );
      vPolygone.push_back( pPoly );

      pRing = new LinearRing ( 3 );
      pPoly->setAussenkontur( pRing );

      vPolyPoints.push_back( s1 );
      vPolyPoints.push_back( s2 );
      vPolyPoints.push_back( s4 );
      vPolyPoints.push_back( s1 );
      pRing->addPoints( vPolyPoints );

      vPolyPoints.clear();
      pPoly = new PolygonGeo ( 3 );
      vPolygone.push_back( pPoly );
      pRing = new LinearRing ( 3 );
      pPoly->setAussenkontur( pRing );

      vPolyPoints.push_back( s2 );
      vPolyPoints.push_back( s3 );
      vPolyPoints.push_back( s4 );
      vPolyPoints.push_back( s2 );
      pRing->addPoints( vPolyPoints );
    }

    pA     = pE;
    nLineA = nLineE;
    vPointsA.clear();
    for ( j = 0; j < anz; j++ )
      vPointsA.push_back(vPointsE[j]);
    vPointsE.clear();
  }

  if ( generateVolume )
  {
    pPoly = new PolygonGeo ( 3 );
    vPolygone.push_back( pPoly );

    pRing = new LinearRing ( 3 );
    pPoly->setAussenkontur( pRing );

    for ( j = anz-1; j >= 0; j--  )
      vPolyPoints.push_back( vPointsA[j] );
    vPolyPoints.push_back( vPointsA[anz-1] );
    pRing->addPoints( vPolyPoints );
  }
 
  Surface * pSurface = new Surface ( 3 );
  for ( i = 0; i < vPolygone.size(); i++ )
    pSurface->addPolygon( vPolygone[i] );

  if ( generateVolume )
  {
    Solid *  pSolid = new Solid;
    pSolid->setAussenkontur( pSurface );
    return pSolid;
  }
  else
    return pSurface;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (BasicCheck)                                     //
///////////////////////////////////////////////////////////////////////////////
bool LineSegment::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport )
{
  if ( points.size() < 2 )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );
    pError->setFeatureGeometryId ( gmlId );

    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  bool success = checkDoublePoints ( pCheckParams->minimalPointDistance, pCheckParams->eliminateDoublePoints );

  if ( success )
    checkResult = BASIC_CHECK_FULFILLED;
  else
    checkResult = BASIC_CHECK_FAILED;
  
  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (AdvancedCheck)                                  //
///////////////////////////////////////////////////////////////////////////////
bool LineSegment::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( checkResult < BASIC_CHECK_FULFILLED )
    return false;

  checkResult = ADVANCED_CHECK_FULFILLED;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Implementierung der Klasse gml:ArcSegment							                	 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor									                        			 //
///////////////////////////////////////////////////////////////////////////////
ArcSegment::ArcSegment( ArcSegment::ARC_SEGMENT_TYPE arcSegmentTypeP ) : CurveSegment( 2 )
{
	arcSegmentType = arcSegmentTypeP;

	arcCenter  = NULL;
	radius     = 0.0;
	startAngle = 0.0;
	endAngle   = 0.0;
	gesAngle   = 0.0;
	
	ellipsenbogen = 0;
	richtung      = 0;

  interpolation = "";
  numArc        = 0;
}

ArcSegment::ArcSegment( ArcSegment * pArcSegmentOld ):CurveSegment ( pArcSegmentOld )
{
  if ( pArcSegmentOld->arcCenter != NULL )
    arcCenter = new GeoPoint ( pArcSegmentOld->arcCenter );
  else
    arcCenter = NULL;

  arcSegmentType = pArcSegmentOld->arcSegmentType;
  radius         = pArcSegmentOld->radius;
  startAngle     = pArcSegmentOld->startAngle;
  endAngle       = pArcSegmentOld->endAngle;
  gesAngle       = pArcSegmentOld->gesAngle;
  ellipsenbogen  = pArcSegmentOld->ellipsenbogen;
  richtung       = pArcSegmentOld->richtung;
  interpolation  = pArcSegmentOld->interpolation;
  numArc         = pArcSegmentOld->numArc;
}

ArcSegment::~ArcSegment()
{
	if ( arcCenter != NULL ) delete arcCenter;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den CurveType des Arc						                        				 //
///////////////////////////////////////////////////////////////////////////////
GEOMETRY_TYPE ArcSegment::getCurveType()
{
  switch ( arcSegmentType )
  {
  case _ARC_STRING:
    return ARC_STRING;

  case _ARC:
    return ARC;

  case _CIRCLE:
    return CIRCLE;
  }

	return ARC;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den GeometryType des Arc				                        				 //
///////////////////////////////////////////////////////////////////////////////
GEOMETRY_TYPE ArcSegment::getGeometryType()
{
  switch ( arcSegmentType )
  {
  case _ARC_STRING:
    return ARC_STRING;

  case _ARC:
    return ARC;

  case _CIRCLE:
    return CIRCLE;
  }

  return ARC;
}

///////////////////////////////////////////////////////////////////////////////
//  Definiert die Stützpunkte											                        	 //
///////////////////////////////////////////////////////////////////////////////
void ArcSegment::addPoints2D ( std::vector<double> xCoordsP,  std::vector<double> yCoordsP )
{
	CurveSegment::addPoints2D ( xCoordsP, yCoordsP );
	calcArcParameters();
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet aus den 3 Kontrollpunten der GML-Darstellung des Kreisbogens   //
//  die Geometrie-Parameter			                        										 //
///////////////////////////////////////////////////////////////////////////////
bool ArcSegment::calcArcParameters()
{
	GeoPoint pAnf(dim);
	GeoPoint pMitte(dim);
	GeoPoint pEnd(dim);
	double   p1[2], p2[2], p21[2], pCenter[2], n1[2], n2[2];
	double   n1Abs, n2Abs, lam, det, midAngle;

	if ( arcCenter != NULL )
	{
		delete arcCenter;
		arcCenter = NULL;
	}

	if ( points.size() != 3 )
		return false;

	pAnf   = points[0];
	pMitte = points[1];
	pEnd   = points[2];

	p1[0] = 0.5*( pAnf.getX() + pMitte.getX() );
	p1[1] = 0.5*( pAnf.getY() + pMitte.getY() );
	p2[0] = 0.5*( pEnd.getX() + pMitte.getX() );
	p2[1] = 0.5*( pEnd.getY() + pMitte.getY() );

	n1[0] =  pMitte.getY() - pAnf.getY();
	n1[1] = -pMitte.getX() + pAnf.getX();
	n1Abs = sqrt ( n1[0]*n1[0] + n1[1]*n1[1] );
	if ( n1Abs < MINIMAL_POINT_DISTANCE )
		return false;

	n2[0] = -pMitte.getY() + pEnd.getY();
	n2[1] =  pMitte.getX() - pEnd.getX();
	n2Abs = sqrt ( n2[0]*n2[0] + n2[1]*n2[1] );
	if ( n2Abs < MINIMAL_POINT_DISTANCE )
		return false;

	p21[0] = p2[0] - p1[0];
	p21[1] = p2[1] - p1[1];

	det = -n1[0]*n2[1] + n1[1]*n2[0];
	if ( fabs(det) < n1Abs*n2Abs*PI/180.0 )
		return false;

	lam = (-p21[0]*n2[1] + p21[1]*n2[0]) / det;
	
	pCenter[0] = p1[0] + lam*n1[0];
	pCenter[1] = p1[1] + lam*n1[1];
	arcCenter = new GeoPoint ( pCenter[0], pCenter[1] );
	radius = sqrt ( ( pCenter[0] - pAnf.getX())*(pCenter[0] - pAnf.getX() ) +
					( pCenter[1] - pAnf.getY())*(pCenter[1] - pAnf.getY() ) );

	if ( arcSegmentType == _CIRCLE )
		return true;

	startAngle = System::Math::Atan2 ( pAnf.getY() - pCenter[1] , pAnf.getX() - pCenter[0] );
	endAngle   = System::Math::Atan2 ( pEnd.getY() - pCenter[1] , pEnd.getX() - pCenter[0] );
	midAngle   = System::Math::Atan2 ( pMitte.getY() - pCenter[1] , pMitte.getX() - pCenter[0] );

	if ( startAngle < endAngle )
	{
		if ( midAngle > startAngle && midAngle < endAngle )
		{
			richtung = 0;
			gesAngle = endAngle - startAngle;
		}
		else
		{
			richtung = 1;
			gesAngle = 2*PI - endAngle + startAngle;
		}
		if ( gesAngle < PI)	ellipsenbogen = 0;
		else				        ellipsenbogen = 1;

	}
	else
	{
		if ( midAngle > startAngle && midAngle < endAngle )
		{
			richtung = 1;
			gesAngle = startAngle - endAngle;
		}
		else
		{
			richtung = 0;
			gesAngle = 2*PI + endAngle - startAngle;
		}
		if ( gesAngle < PI)	ellipsenbogen = 0;
		else				ellipsenbogen = 1;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von Punkten zum Einbau in SVG-path		              						 //
///////////////////////////////////////////////////////////////////////////////
string ArcSegment::getPointsSVG ( double offsetX, double offsetY )
{
	string        svgPath;
	GeoPoint	    pAnf(2);
	GeoPoint	    pEnd(2);
	ostringstream os;
	int			      ellipsenbogenA;
	
	if ( arcCenter == NULL ) return svgPath;

	pAnf = points[0];
	pEnd = points[2];

	os << " A ";
	os << radius << " " << radius << " 0 " << ellipsenbogen << " " << richtung << " ";
	os << pEnd.getX() - offsetX << " " << offsetY - pEnd.getY();

	if ( arcSegmentType == _CIRCLE )
	{
		if ( ellipsenbogen == 0 ) ellipsenbogenA = 1;
		else					            ellipsenbogen  = 0;

		os << " A ";
		os << radius << " " << radius << " 0 " << ellipsenbogenA << " " << richtung << " ";
		os << pAnf.getX() - offsetX << " " << offsetY - pAnf.getY();
	}

	svgPath = os.str();
	return svgPath;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von Punkten in umgekehrter Reihenfolge zum Einbau in SVG-path 	 //
///////////////////////////////////////////////////////////////////////////////
string ArcSegment::getPointsReverseSVG ( double offsetX, double offsetY )
{
	string        svgPath;
	GeoPoint	    pAnf(2);
	GeoPoint	    pEnd(2);
	ostringstream os;
	int			      ellipsenbogenA;
  int           richtungA;
	
	if ( arcCenter == NULL ) return svgPath;

	pAnf   = points[2];
	pEnd   = points[0];

  if ( richtung == 0 ) richtungA = 1;
  else					       richtungA = 0;

	os << " A ";
	os << radius << " " << radius << " 0 " << ellipsenbogen << " " << richtungA << " ";
	os << pEnd.getX() - offsetX << " " << offsetY - pEnd.getY();

	if ( arcSegmentType == _CIRCLE )
	{
		if ( ellipsenbogen == 0 ) ellipsenbogenA = 1;
		else					            ellipsenbogen  = 0;

		os << " A ";
		os << radius << " " << radius << " 0 " << ellipsenbogenA << " " << richtung << " ";
		os << pAnf.getX() - offsetX << " " << offsetY - pAnf.getY();
	}

	svgPath = os.str();
	return svgPath;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von Punkten zum Einbau in SVG-path		              						 //
///////////////////////////////////////////////////////////////////////////////
string ArcSegment::getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode )
{
  string         svgPath;
  ostringstream  os;
  unsigned int	 i;

  for ( i = 0; i < points.size(); i++ )
  {
    if ( i == 0 )
    {  
      if ( mode == MOVE ) os << " M ";
      else continue;
    }
    else os << " L ";

    GeoPoint pPoint = points[i];
    os << pPoint.getX() - offsetX << " " << offsetY - pPoint.getY();
  }

  svgPath = os.str();
  return svgPath;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von Punkten in umgekehrter Reihenfolge zum Einbau in SVG-path 	 //
///////////////////////////////////////////////////////////////////////////////
string ArcSegment::getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode )
{
  string         svgPath;
  ostringstream  os;
	int            i;

  for ( i = (int)points.size() - 1; i >= 0; i-- )
  {
    if ( i == (int)points.size() - 1 )
    {
      if ( mode == MOVE ) os << " M ";
      else continue;
    }
    else os << " L ";

    GeoPoint pPoint = points[i];
    os << pPoint.getX() - offsetX << " " << offsetY - pPoint.getY();
  }

  svgPath = os.str();
  return svgPath;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Länge des Segmentes										                     //
///////////////////////////////////////////////////////////////////////////////
double ArcSegment::getLength()
{
	if ( arcSegmentType == _CIRCLE ) return 2.0*PI*radius;
	else			                       return gesAngle*radius;
}

///////////////////////////////////////////////////////////////////////////////
//  Ausführen des BasicCheck                                                 //
///////////////////////////////////////////////////////////////////////////////
bool ArcSegment::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  if ( points.size() <3 )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );
    pError->setFeatureGeometryId ( gmlId );

    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  bool success = checkDoublePoints ( pCheckParams->minimalPointDistance, pCheckParams->eliminateDoublePoints );

  if ( success )
    checkResult = BASIC_CHECK_FULFILLED;
  else
    checkResult = BASIC_CHECK_FULFILLED;

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Ausführen des AdvancedCheck                                              //
///////////////////////////////////////////////////////////////////////////////
bool ArcSegment::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( checkResult < BASIC_CHECK_FULFILLED )
    return false;

  checkResult = ADVANCED_CHECK_FULFILLED;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt Punkte auf der Kurve mit Abstand scanDist					             //
//  Ist die Länge der Kurve kein ganzzahliges Vielfaches von scanDist, wird  //
//  dieser Parameter entsprechend angepasst									                 //
///////////////////////////////////////////////////////////////////////////////
size_t ArcSegment::scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
							std::vector<double> &angleP, double scanDist, bool reverse )
{
	double deltaAngle, x, y, h, angle;
	size_t i, anz_h, anz;

	if ( arcCenter == NULL ) return 0;

	anz = int ( floor ( getLength() / scanDist + 0.5 ) );
	deltaAngle = gesAngle / anz;

	for ( i = 0; i <= anz; i++ )
	{
		if ( richtung == 0 ) angle = startAngle + i*deltaAngle;
		else				 angle = startAngle - i*deltaAngle;

		x = arcCenter->getX() + cos ( angle)*radius;
		y = arcCenter->getX() + sin ( angle)*radius;
		xP.push_back ( x );
		yP.push_back ( y );
		angleP.push_back ( angle + 0.5*PI );
	}

	if ( reverse )
	{

		anz   = xP.size();
		anz_h = div ( (int)anz, 2 ).quot;
		for ( i = 0; i < anz_h; i++ )
		{
			h = xP[i];
			xP[i] = xP[anz-i-1];
			xP[anz-i-1] = h;

			h = yP[i];
			yP[i] = yP[anz-i-1];
			yP[anz-i-1] = h;

			h = angleP[i];
			
			angleP[i] = angleP[anz-i-1] + PI;
			angleP[anz-i-1] = h + PI;
		}
		if ( div ( (int)anz, 2 ).rem == 1 )
		{
			angleP[anz_h] = angleP[anz_h] + PI;
		}
	}
	return xP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Anfangs- und Endsteigung							                    			 //
///////////////////////////////////////////////////////////////////////////////
double ArcSegment::getAnfangsSteigung2D() 
{
	return startAngle + 0.5*PI;	
}

double ArcSegment::getEndSteigung2D()
{
	return endAngle + 0.5*PI;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformation - Dummy Implementierung                                   //
///////////////////////////////////////////////////////////////////////////////
void ArcSegment::transform ( Matrix4 & matrix )
{

}

void ArcSegment::transformInvers ( Matrix4 & matrix )
{

}

///////////////////////////////////////////////////////////////////////////////
//  Implementierung der Klasse gml:GeoPoint		                							 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor							                        					 //
///////////////////////////////////////////////////////////////////////////////
GeoPoint::GeoPoint (): _Geometrie ( 2 )
{
  x = 0;
  y = 0;
  z = 0;
}

GeoPoint::GeoPoint( int dimP ) : _Geometrie( dimP )
{
	x = 0;
	y = 0;
  z = 0;
}

GeoPoint::GeoPoint ( double xP, double yP ) : _Geometrie( 2 )
{
	x = xP;
	y = yP;
  z = 0.0;
}

GeoPoint::GeoPoint ( double xP, double yP, double zP ) : _Geometrie( 3 )
{
  x = xP;
  y = yP;
  z = zP;
}

GeoPoint::GeoPoint ( GeoPoint * pPointOld ): _Geometrie ( pPointOld )
{
	x = pPointOld->x;
	y = pPointOld->y;
  z = pPointOld->z;
}

GeoPoint::GeoPoint ( const GeoPoint & pointOld ): _Geometrie ( pointOld )
{
  x = pointOld.x;
  y = pointOld.y;
  z = pointOld.z;
}

GeoPoint::~GeoPoint()
{
}

///////////////////////////////////////////////////////////////////////////////
//  + Operator - Vektoraddition 									                           //
///////////////////////////////////////////////////////////////////////////////
GeoPoint GeoPoint::operator+ ( GeoPoint p  )
{
  GeoPoint result(dim);

  result.x = x + p.x;
  result.y = y + p.y;
  if ( dim == 3 )
    result.z = z + p.z;

  return result;
}

///////////////////////////////////////////////////////////////////////////////
//  - Operator - Vektorsubtraktion								                           //
///////////////////////////////////////////////////////////////////////////////
GeoPoint GeoPoint::operator- ( GeoPoint p  )
{
  GeoPoint result(dim);

  result.x = x - p.x;
  result.y = y - p.y;
  if ( dim == 3 )
    result.z = z - p.z;

  return result;
}

///////////////////////////////////////////////////////////////////////////////
//  - Operator (unitär) - Negatives Vorzeichen		                           //
///////////////////////////////////////////////////////////////////////////////
GeoPoint GeoPoint::operator-()
{
  GeoPoint result(dim);

  result.x = -x;
  result.y = -y;
  if ( dim == 3 )
    result.z = -z;

  return result;
}

///////////////////////////////////////////////////////////////////////////////
//  * Operator - Vektor * Skalar              		                           //
///////////////////////////////////////////////////////////////////////////////
GeoPoint GeoPoint::operator* ( double d )
{
  GeoPoint result(dim);

  result.x = d*x;
  result.y = d*y;
  if ( dim == 3 )
    result.z = d*z;

  return result;
}

///////////////////////////////////////////////////////////////////////////////
//  [] Operator - Zugriff auf Vektor-Koordinate		                           //
///////////////////////////////////////////////////////////////////////////////
double GeoPoint::operator[] ( int index )
{
  switch ( index )
  {
  case 0: return x;
  case 1: return y;
  case 2: return z;
  default: return 0.0;
  }

  return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  = Operator - Zuweisung												                           //
///////////////////////////////////////////////////////////////////////////////
GeoPoint GeoPoint::operator= ( GeoPoint p )
{
  x = p.x;
  y = p.y;
  if ( p.dim == 3 )
  {
    dim = 3;
    z = p.z;
  }
  else
  {
    dim = 2;
    z = 0.0;
  }

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//  >= Operator - Vergleichsoperator alle Koordinaten                        //
///////////////////////////////////////////////////////////////////////////////
bool GeoPoint::operator>= ( const GeoPoint p ) const
{
  bool success;

  success = x >= p.x && y >= p.y;
  if ( dim == 3 )
    success = success && z >= p.z;

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  <= Operator - Vergleichsoperator alle Koordinaten                        //
///////////////////////////////////////////////////////////////////////////////
bool GeoPoint::operator<= ( const GeoPoint p ) const
{
  bool success;

  success = x <= p.x && y <= p.y;
  if ( dim == 3 )
    success = success && z <= p.z;

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  == Operator - Gleichheit von Vektoren (mit Toleranz)                     //
///////////////////////////////////////////////////////////////////////////////
bool GeoPoint::operator== ( const GeoPoint p ) const
{
  double dist = (x - p.x)*(x - p.x) + (y - p.y)*(y - p.y);
  if ( dim == 3 )
    dist = dist + (z - p.z)*(z - p.z);
  dist = sqrt ( dist );
  if ( dist < MINIMAL_POINT_DISTANCE )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  += Operator                                                              //
///////////////////////////////////////////////////////////////////////////////
GeoPoint GeoPoint::operator+= ( const GeoPoint p )
{
  x = x + p.x;
  y = y + p.y;
  if ( dim == 3 )
    z = z + p.z;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//  -= Operator                                                              //
///////////////////////////////////////////////////////////////////////////////
GeoPoint GeoPoint::operator-= ( const GeoPoint p )
{
  x = x - p.x;
  y = y - p.y;
  if ( dim == 3 )
    z = z - p.z;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//  Abstand zweier Punkte                                                    //
///////////////////////////////////////////////////////////////////////////////
double GeoPoint::dist (  GeoPoint p )
{
  double dist;

  if ( dim == 2 )
    dist = sqrt ( (x - p.x)*(x - p.x) + (y - p.y)*(y - p.y) );
  else
    dist = sqrt ( (x - p.x)*(x - p.x) + (y - p.y)*(y - p.y) + (z - p.z)*(z - p.z) );

  return dist;
}

///////////////////////////////////////////////////////////////////////////////
//  Skalarprodukt zweier Vektoren                                            //
///////////////////////////////////////////////////////////////////////////////
double GeoPoint::mult ( GeoPoint p )
{
  double h = x*p.x + y*p.y;
  if ( dim == 3 )
    h = h + z*p.z;

  return h;
}

///////////////////////////////////////////////////////////////////////////////
//  Normalisiert den Punkt (Vektor) auf Länge 1                              //
///////////////////////////////////////////////////////////////////////////////
void GeoPoint::normalize()
{
  double h = sqrt ( x*x + y*y + z*z );
  if ( h > 0 )
  {
    x = x / h;
    y = y / h;
    if ( dim == 3 ) z = z / h;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Koordinaten												                          	 //
///////////////////////////////////////////////////////////////////////////////
void GeoPoint::set ( double xP, double yP )
{
	x = xP;
	y = yP;
}

void GeoPoint::set ( double xP, double yP, double zP )
{
  x = xP;
  y = yP;
  z = zP;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding-Box												 //
///////////////////////////////////////////////////////////////////////////////
bool GeoPoint::getBoundingBox ( double * sw, double * no )
{
	sw[0] = x;
	sw[1] = y;
	no[0] = x;
	no[1] = y;
  if ( dim == 3 )
  {
    sw[2] = z;
    no[2] = z;
  }
  else
  {
    sw[2] = 0.0;
    no[2] = 0.0;
  }

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt                                                  //
///////////////////////////////////////////////////////////////////////////////
bool GeoPoint::getCenter2D ( double &xP, double &yP )
{
  xP = x;
  yP = y;
  return true;
}

bool GeoPoint::getCenter3D ( double &xP, double &yP, double &zP )
{
  xP = x;
  yP = y;
  zP = z;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone                                          //
///////////////////////////////////////////////////////////////////////////////
size_t GeoPoint::getPolygonAnz()
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Polygone                                                         //
///////////////////////////////////////////////////////////////////////////////
size_t GeoPoint::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
  return 0;
}

size_t GeoPoint::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )
{
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t GeoPoint::getCurveSegmentAnz()
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t GeoPoint::getPointAnz()
{
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
//  Prüft die Geometrie                                                      //
///////////////////////////////////////////////////////////////////////////////
bool GeoPoint::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  checkResult = BASIC_CHECK_FULFILLED;
  return true;
}

bool GeoPoint::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  checkResult = ADVANCED_CHECK_FULFILLED;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für die Geometrie ein gültiges  Referenzsystem              //
//  definiert ist                                                            //
///////////////////////////////////////////////////////////////////////////////
bool GeoPoint::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName == "" )
    return false;

  return pSrsManager->existCoordSystem( srsName );
}

///////////////////////////////////////////////////////////////////////////////
//  Affine Transformation Punkt                                              //
///////////////////////////////////////////////////////////////////////////////
void GeoPoint::transform ( Matrix4 & pMatrix )
{
  double p[3], pTrans[3];

  p[0] = x;
  p[1] = y;
  p[2] = z;

  pMatrix.transform( p, pTrans );
 
  x = pTrans[0];
  y = pTrans[1];
  z = pTrans[2];

  dim = 3;
}

///////////////////////////////////////////////////////////////////////////////
//  Inverse affine Transformation Punkt                                      //
///////////////////////////////////////////////////////////////////////////////
void GeoPoint::transformInvers ( Matrix4 & pMatrix )
{
  double p[3], pTrans[3];

  p[0] = x;
  p[1] = y;
  p[2] = z;

  pMatrix.transformInvers( p, pTrans );

  x = pTrans[0];
  y = pTrans[1];
  z = pTrans[2];

  dim = 3;
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebung des Punktes um offset                                       //
///////////////////////////////////////////////////////////////////////////////
void GeoPoint::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  )
{
  x = x + point.getX();
  y = y + point.getY();
  if ( dim == 3 )
    z = z + point.getZ();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Punptkoordinaten als Text                                    //
///////////////////////////////////////////////////////////////////////////////
std::string GeoPoint::toStr ()
{
  std::string xStr, yStr, zStr, resultStr;

  String ^ xSTR = Convert::ToString ( x );
  String ^ ySTR = Convert::ToString ( y );
  String ^ zSTR = Convert::ToString ( z );

  QuConvert::systemStr2stdStr ( xStr, xSTR );
  QuConvert::systemStr2stdStr ( yStr, ySTR );
  QuConvert::systemStr2stdStr ( zStr, zSTR );

  resultStr = "(" + xStr + ", " + yStr + ", " + zStr + ")";
  return resultStr;
}



///////////////////////////////////////////////////////////////////////////////
//  Implementierung der Klasse gml:DirectPosition                            //
///////////////////////////////////////////////////////////////////////////////
DirectPosition::DirectPosition()
:_Geometrie( 0 )
{

}
DirectPosition::~DirectPosition()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die DirectPosition                                                 //
///////////////////////////////////////////////////////////////////////////////
void DirectPosition::set ( std::vector<double> vPosP )
{
	size_t i;

  vPos.clear();
  for ( i = 0; i < vPosP.size(); i++ )
    vPos.push_back( vPosP[i] );

  dim = (int)vPos.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die DirectPosition                                               //
///////////////////////////////////////////////////////////////////////////////
size_t DirectPosition::get ( std::vector<double> &vPosP )
{
	size_t i;

  for ( i = 0; i < vPos.size(); i++ )
    vPosP.push_back( vPos[i] );

  return vPos.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für die Geometrie ein gültiges  Referenzsystem              //
//  definiert ist                                                            //
///////////////////////////////////////////////////////////////////////////////
bool DirectPosition::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName == "" )
    return false;

  return pSrsManager->existCoordSystem( srsName );
}

///////////////////////////////////////////////////////////////////////////////
//  Prüft die Geometrie (BasicCheck)                                         //
///////////////////////////////////////////////////////////////////////////////
bool DirectPosition::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  if ( vPos.size() > 0 )
  {
    checkResult = BASIC_CHECK_FULFILLED;
    return true;
  }
  else
  {
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Prüft die Geometrie (AdvancedCheck)                                      //
///////////////////////////////////////////////////////////////////////////////
bool DirectPosition::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( checkResult < BASIC_CHECK_FULFILLED ) 
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt die DirectPosition um einen Offset                            //
///////////////////////////////////////////////////////////////////////////////
void DirectPosition::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  )
{
  if ( vPos.size() > 0 )
    vPos[0] = vPos[0] + point.getX();

  if ( vPos.size() > 1 )
    vPos[1] = vPos[1] + point.getY();
  else
    return;

  if ( vPos.size() > 2 )
    vPos[2] = vPos[2] + point.getZ();
}

///////////////////////////////////////////////////////////////////////////////
//  _Solid                                                                   //
//  Abstrakte Oberklasse von Solid und CompositeSolid                        //
///////////////////////////////////////////////////////////////////////////////
_Solid::_Solid(): _Geometrie(3)
{

}

_Solid::_Solid ( _Solid * pSolidOld )
:_Geometrie ( pSolidOld )
{

}

_Solid:: ~_Solid()
{

}

///////////////////////////////////////////////////////////////////////////////
//  SolidReference                                                           //
//  Referenz auf einen _Solid                                                //
///////////////////////////////////////////////////////////////////////////////
SolidReference::SolidReference( Features * pFeaturesP ): _Solid (), _GeometryReference ( pFeaturesP )
{

}

SolidReference::SolidReference ( SolidReference * pSolidReferenceOld )
:_Solid (  pSolidReferenceOld ), _GeometryReference ( pSolidReferenceOld )
{
}

SolidReference::~SolidReference()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone der Aussenkontur bzw. Innenkonturen                 //
//  des referierten Solids                                                   //
///////////////////////////////////////////////////////////////////////////////
size_t SolidReference::getAussenkonturPolygone( vector<PolygonGeo*> &vPolygone, vector<bool> &vOrientations )
{
  _Solid * pGeo = dynamic_cast<_Solid*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getAussenkonturPolygone( vPolygone,vOrientations );
  else
    return 0;
}

size_t SolidReference::getInnenkonturPolygone( vector<PolygonGeo*> &vPolygone, vector<bool> &vOrientations )
{
  _Solid * pGeo = dynamic_cast<_Solid*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getInnenkonturPolygone( vPolygone, vOrientations );
  else
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die Dimension der referierten Geometrie, oder 0 falls keine      //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
int SolidReference::getDim()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getDim();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den srsName des referierten Geometrie, oder "" falls keine       //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
std::string SolidReference::getSrsName ()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getSrsName();
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den BoundingBox der referierten Geometrie, oder false falls keine//
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool SolidReference::getBoundingBox ( double * sw, double * no )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getBoundingBox( sw, no );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Center2D der referierten Geometrie, oder false falls keine   //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool SolidReference::getCenter2D ( double &xP, double &yP )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCenter2D( xP, yP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Center3D der referierten Geometrie, oder false falls keine   //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool SolidReference::getCenter3D ( double &xP, double &yP, double &zP )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCenter3D( xP, yP, zP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone der referierten Geometrie                //
///////////////////////////////////////////////////////////////////////////////
size_t SolidReference::getPolygonAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygonAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone der referierten Geometrie                           //
///////////////////////////////////////////////////////////////////////////////
size_t SolidReference::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygone( vPolygone );
  else
    return 0;
}

size_t SolidReference::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygone( vPolygone, vOrientations );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments der referierten Geometrie           //
///////////////////////////////////////////////////////////////////////////////
size_t SolidReference::getCurveSegmentAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCurveSegmentAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte der referierten Geometrie                  //
///////////////////////////////////////////////////////////////////////////////
size_t SolidReference::getPointAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPointAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Volumen der referierten Geometrie                            //
///////////////////////////////////////////////////////////////////////////////
double SolidReference::getVolume()
{
  _Solid * pGeo = dynamic_cast<Solid*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getVolume();
  else
    return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Größe der Oberfläche der referierten Geometrie               //
///////////////////////////////////////////////////////////////////////////////
double SolidReference::getOberflaeche()
{
  _Solid * pGeo = dynamic_cast<Solid*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->getOberflaeche();
  else
    return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie der referierten Geometrie (basicCheck)           //
///////////////////////////////////////////////////////////////////////////////
bool SolidReference::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  if ( !pCheckParams->checkReferencedGeometry )
    return true;

  _Solid * pGeo = dynamic_cast<Solid*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );
  else
    return false;

}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie der referierten Geometrie (advancedCheck)        //
///////////////////////////////////////////////////////////////////////////////
bool SolidReference::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( !pCheckParams->checkReferencedGeometry )
    return true;

  _Solid * pGeo = dynamic_cast<Solid*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Führt den ToplogyCheck für die referierte Geometrie durch                //
///////////////////////////////////////////////////////////////////////////////
bool SolidReference::topologyCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  _Solid * pGeo = dynamic_cast<Solid*>(pFeatures->getGeometry( refGeoId ));
  if ( pGeo != NULL ) 
    return pGeo->topologyCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Referenzen                                                 //
///////////////////////////////////////////////////////////////////////////////
bool SolidReference::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  string          errorString;
  SemanticError * pError;
  GEOMETRY_TYPE   geoType;

  _Geometrie * pRefGeo = getRefGeoObjekt();
  if ( pRefGeo == NULL )
  {
    errorString = "Das Geometrieattribut " + propertyName + " hat ein nicht-aufgelöstes xlink: " + refGeoId;
    pError = new SemanticError ( pObj, Error::ERROR, errorString );    
    vErrorReport.push_back( pError );

    return false;
  }

  geoType = pRefGeo->getGeometryType();

  if ( geoType != SOLID              && 
       geoType != COMPOSITE_SOLID    )
  {
    errorString = "Das Geometrieattribut " + propertyName + " verweist mit " + refGeoId + " auf einen falschen Geometrietyp. ";
    errorString = errorString + "Notwendig ist Flächengeometrie, verwiesen wird auf " + _Geometrie::convertTypeToString( geoType );
    pError = new SemanticError ( pObj, Error::ERROR, errorString );    
    vErrorReport.push_back( pError );

    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der referierten Geometrie ein gültiges       //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool SolidReference::existReferenceSystem( SrsManager * pSrsManager )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->existReferenceSystem( pSrsManager );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Offset auf das referierte Solid                                          //
//  Über den Set von Geometrieobjekten wird sichergestellt, dass ein         //
//  bestimmtes Geometrieobjekt nur einmal verschoben wird                    //
///////////////////////////////////////////////////////////////////////////////
void SolidReference::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  _Geometrie * pGeo = getRefGeoObjekt();

  if ( pGeo != NULL && sGeometries.count( pGeo ) == 0 )
  {
    pGeo->offset( point, sGeometries );
    sGeometries.insert( pGeo );
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Solid				                                												     //
//  Klassen für gml:Solid                                                    //
///////////////////////////////////////////////////////////////////////////////
Solid::Solid(): _Solid()
{
  pAussenkontur = NULL;
}

Solid::Solid ( Solid * pSolidOld ): _Solid ( pSolidOld )
{
  if ( pSolidOld->pAussenkontur != NULL )
  {
    pAussenkontur =  dynamic_cast<_Surface*>(Geometrie::copy( pSolidOld->pAussenkontur ));
  }
  else
    pAussenkontur = NULL;

  for (  int i = 0; i < pSolidOld->getInnenkonturAnz(); i++ )
  {
    _Surface * pInnenkonturCopy =  dynamic_cast<_Surface*>(Geometrie::copy( pSolidOld->getInnenkontur( i ) ));
    vInnenkonturen.push_back( pInnenkonturCopy );
  }
}

Solid::~Solid()
{
  if ( pAussenkontur != NULL ) delete pAussenkontur;

  for ( unsigned int i = 0; i < vInnenkonturen.size(); i++ )
    delete vInnenkonturen[i];

}
///////////////////////////////////////////////////////////////////////////////
//  Setzt die Außenkontur des Solids                                         //
///////////////////////////////////////////////////////////////////////////////
void Solid::setAussenkontur ( _Surface * pAussenkonturP )
{
  if ( pAussenkontur != NULL ) delete pAussenkontur;
  pAussenkontur = pAussenkonturP;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Innenkontur des Solids über den Index                       //
///////////////////////////////////////////////////////////////////////////////
_Surface * Solid::getInnenkontur ( unsigned int index )
{
  if ( index < 0 || index >= vInnenkonturen.size() ) return NULL;

  return vInnenkonturen[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die BoundingBox                                                  //
///////////////////////////////////////////////////////////////////////////////
bool Solid::getBoundingBox ( double * sw, double * no )
{
  if ( pAussenkontur != NULL )
    return pAussenkontur->getBoundingBox( sw, no );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den 2D-Schwerpunkt                                               //
///////////////////////////////////////////////////////////////////////////////
bool Solid::getCenter2D ( double &xP, double &yP )
{
  if ( pAussenkontur != NULL )
    return pAussenkontur->getCenter2D( xP, yP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den 3D-Schwerpunkt                                               //
///////////////////////////////////////////////////////////////////////////////
bool Solid::getCenter3D ( double &xP, double &yP, double &zP )
{
  if ( pAussenkontur != NULL )
    return pAussenkontur->getCenter3D( xP, yP, zP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone (mit Orientierungen) der Solid-Außenkontur          //
///////////////////////////////////////////////////////////////////////////////
size_t Solid::getAussenkonturPolygone( vector<PolygonGeo*> &vPolygone, vector<bool> &vOrientations )
{
  if ( pAussenkontur != NULL )
    pAussenkontur->getPolygone ( vPolygone, vOrientations );

  return vPolygone.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone (mit Orientierungen) aller Solid-Innenkonturen      //
///////////////////////////////////////////////////////////////////////////////
size_t Solid::getInnenkonturPolygone( vector<PolygonGeo*> &vPolygone, vector<bool> &vOrientations )
{
	size_t i;

  for ( i = 0; i < vInnenkonturen.size(); i++ )
  {
    _Surface * pSurface = vInnenkonturen[i];
    pSurface->getPolygone( vPolygone, vOrientations );
  }
  return vPolygone.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone von Außenkontur und Innenkonturen        //
///////////////////////////////////////////////////////////////////////////////
size_t Solid::getPolygonAnz()
{
	size_t i, anz;

  if ( pAussenkontur == NULL ) 
    return 0;

  anz = pAussenkontur->getPolygonAnz();

  for ( i = 0; i < vInnenkonturen.size(); i++ )
  {
    _Surface * pGeo = vInnenkonturen[i];
    anz = anz + pGeo->getPolygonAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone von AUßenkontur und Innenkonturen                   //
///////////////////////////////////////////////////////////////////////////////
size_t Solid::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
	size_t i, anz;

  if ( pAussenkontur == NULL ) 
    return 0;

  anz = pAussenkontur->getPolygone( vPolygone );

  for ( i = 0; i < vInnenkonturen.size(); i++ )
  {
    _Surface * pGeo = vInnenkonturen[i];
    anz = anz + pGeo->getPolygone( vPolygone );
  }
  return anz;
}

size_t Solid::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )
{
	size_t i, anz;

  if ( pAussenkontur == NULL ) 
    return 0;

  anz = pAussenkontur->getPolygone( vPolygone, vOrientations );

  for ( i = 0; i < vInnenkonturen.size(); i++ )
  {
    _Surface * pGeo = vInnenkonturen[i];
    anz = anz + pGeo->getPolygone( vPolygone, vOrientations );
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t Solid::getCurveSegmentAnz()
{
	size_t i, anz;

  if ( pAussenkontur == NULL ) 
    return 0;

  anz = pAussenkontur->getCurveSegmentAnz();

  for ( i = 0; i < vInnenkonturen.size(); i++ )
  {
    _Surface * pGeo = vInnenkonturen[i];
    anz = anz + pGeo->getCurveSegmentAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t Solid::getPointAnz()
{
	size_t i, anz;

  if ( pAussenkontur == NULL ) 
    return 0;

  anz = pAussenkontur->getPointAnz();

  for ( i = 0; i < vInnenkonturen.size(); i++ )
  {
    _Surface * pGeo = vInnenkonturen[i];
    anz = anz + pGeo->getPointAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Volumen des durch vPolygone/vOrientations bestimmten Solids  //
///////////////////////////////////////////////////////////////////////////////
double Solid::getVolume( vector<PolygonGeo*> &vPolygone, vector<bool> &vOrientations )
{
  double                 volume = 0.0;
	size_t                 j, triaAnz;
  bool                   success;
  bool                   orientation;
  std::vector<int>       vTriaPointIndex;
  std::vector<GeoPoint*> vTriaPoints;
  std::string            strDreieck;

  //string pathName = "C:\\Daten\\FZJ-CityGML-Daten\\Energiesimulation\\triangulation.txt";
  //LogfileLokal * pLogfile = new LogfileLokal( pathName );

  TessTriangulator * pTriangulator = new TessTriangulator();
  success = pTriangulator->triangulatePolygons( vPolygone, vTriaPointIndex, vTriaPoints, vOrientations );
  if ( success )
  {
    triaAnz = vTriaPointIndex.size() / 4;
    for ( j = 0; j < triaAnz; j++ )
    {
      if ( vTriaPointIndex[4*j] >= (int)vTriaPoints.size() ||   vTriaPointIndex[4*j] < 0 ||
           vTriaPointIndex[4*j+1] >= (int)vTriaPoints.size() ||  vTriaPointIndex[4*j+1] < 0 ||
           vTriaPointIndex[4*j+2] >= (int)vTriaPoints.size() || vTriaPointIndex[4*j+2] < 0 )
      {
        success = false;
        break;
      }
    }
  }

  if ( !success )
    return 0.0;

  volume = 0.0;
  for ( j = 0; j < triaAnz; j++ )
  {
    GeoPoint * p0, * p1, * p2;
    double     a, b, c, s, A;
    double     h0, h1, h2;
    double     nz;

    p0 = vTriaPoints[vTriaPointIndex[4*j]];
    p1 = vTriaPoints[vTriaPointIndex[4*j+1]];
    p2 = vTriaPoints[vTriaPointIndex[4*j+2]];

  //  strDreieck = p0->toStr() + " | " + p1->toStr() + " | " + p2->toStr();
  //  pLogfile->writeLogFile ( strDreieck );

    a = sqrt((p1->x - p0->x) * (p1->x - p0->x) + (p1->y - p0->y) * (p1->y - p0->y));
    b = sqrt((p2->x - p1->x) * (p2->x - p1->x) + (p2->y - p1->y) * (p2->y - p1->y));
    c = sqrt((p0->x - p2->x) * (p0->x - p2->x) + (p0->y - p2->y) * (p0->y - p2->y));

    nz = (p0->x - p1->x)*(p1->y - p2->y) - (p0->y - p1->y)*(p1->x - p2->x);
    if ( nz > 0.0 )
      orientation = true;
    else
      orientation = false;

    s = (a + b + c) * 0.5;
    A = s * ((s - a) * (s - b) * (s - c));
    if ( A < 0.0 ) 
      A = 0.0;
    A = sqrt( A );

    h0 = p0->z;
    h1 = p1->z;
    h2 = p2->z;

    if ( orientation )
    {
      volume = volume + (A * (h0 + h1 + h2)) / 3.0;
    }
    else
    {
      volume = volume - (A * (h0 + h1 + h2)) / 3.0;
    }
  }

  for ( unsigned int i = 0; i < vTriaPoints.size(); i++ )
    delete vTriaPoints[i];

//  delete pLogfile;
    
  return volume;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Volumen des Solids                                           //
///////////////////////////////////////////////////////////////////////////////
double Solid::getVolume()
{
  vector<PolygonGeo*> vPolygone;
  vector<bool>        vOrientations;

  getAussenkonturPolygone( vPolygone, vOrientations );
  return getVolume( vPolygone, vOrientations );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Oberfläche des Solids                                        //
///////////////////////////////////////////////////////////////////////////////
double Solid::getOberflaeche()
{
  if ( pAussenkontur != NULL )
    return pAussenkontur->getFlaeche();
  else
    return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (BasicCheck)                                     //
///////////////////////////////////////////////////////////////////////////////
bool Solid::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  if ( pAussenkontur == NULL )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    pError->setPropertyName( featureProperty );

    pError->setFeatureGeometryId ( gmlId );
    vErrorReport.push_back( pError );

    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  bool success = pAussenkontur->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );

  if ( success )
    checkResult = BASIC_CHECK_FULFILLED;
  else
    checkResult = BASIC_CHECK_FULFILLED;
  
  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (AdvancedCheck)                                  //
///////////////////////////////////////////////////////////////////////////////
bool Solid::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;

  bool success = pAussenkontur->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
	bool successA = pAussenkontur->topologyCheck(pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );

  if ( success && successA )
    checkResult = ADVANCED_CHECK_FULFILLED;
  else
    checkResult = ADVANCED_CHECK_FULFILLED;

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (TopologyCheck)                                  //
//  Es wird überprüft, ob alle Kanten des Solids gleich häufig in            //
//  umgekehrter Laufrichtung vorhanden sind. Wenn dies mehr als einmal       //
//  passiert, wird eine Warnung erzeugt                                      //
///////////////////////////////////////////////////////////////////////////////
bool Solid::topologyCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  BRep * pBRep;
  bool   success1;

  if ( checkResult < ADVANCED_CHECK_FULFILLED && !checkAnyway )
    return false;

  generateTopology( pCheckParams->minimalPointDistance, pObj->getFeatures() );
  pBRep = getTopology();
  success1 = pBRep->isVolume ( pObj, featureProperty, gmlId, pCheckParams, vErrorReport );

  if ( success1 == false )  
    return false;
  else
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Referenz                                                   //
///////////////////////////////////////////////////////////////////////////////
bool Solid::checkReferences ( Feature * pObj, std::string propertyName, vector<Error*> &vErrorReport )
{
  string          errorString;
  SemanticError * pError;
  bool            success, successsAll;

  if ( pAussenkontur == NULL )
  {
    errorString = "Das Geometrieattribut " + propertyName + " zeigt auf eine Solid-Geometrie ohne Außenkontur";
    pError = new SemanticError ( pObj, Error::ERROR, errorString );    
    vErrorReport.push_back( pError );

    return false;
  }

  success = pAussenkontur->checkReferences( pObj, propertyName, vErrorReport );
  if ( !success ) return false;

  successsAll = true;

  for ( unsigned int i = 0; i < vInnenkonturen.size(); i++ )
  {
    _Surface * pSurface = vInnenkonturen[i];
    success = pSurface->checkReferences( pObj, propertyName, vErrorReport );
    if ( !success ) successsAll = false;
  }

  return successsAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die Knoten-Kanten Struktur der Solid-Außenkontur               //
///////////////////////////////////////////////////////////////////////////////
void Solid::generateTopology( double minDist, Features * pFeaturesP )
{
  if ( pAussenkontur != NULL )
    pAussenkontur->generateTopology( minDist, pFeaturesP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Knoten-Kanten Struktur der Solid-Außenkontur                 //
///////////////////////////////////////////////////////////////////////////////
BRep * Solid::getTopology()
{
  if ( pAussenkontur != NULL )
    return pAussenkontur->getTopology();
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für die Geometrie ein gültiges                              //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool Solid::existReferenceSystem( SrsManager * pSrsManager )
{
  bool exist;

  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  if ( pAussenkontur == NULL )
    return false;

  exist = pAussenkontur->existReferenceSystem( pSrsManager );
  if ( !exist )
    return false;

  for ( unsigned int i = 0; i < vInnenkonturen.size(); i++ )
  {
    if ( vInnenkonturen[i]->existReferenceSystem( pSrsManager ) == false )
      return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt den Solid um einen Offset                                     //
///////////////////////////////////////////////////////////////////////////////
void Solid::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  unsigned int i;

  if ( pAussenkontur != NULL && sGeometries.count( pAussenkontur) == 0 )
    pAussenkontur->offset( point, sGeometries );

  for ( i = 0; i < vInnenkonturen.size(); i++  )
  {
    _Surface * pLoch = vInnenkonturen[i];
    if ( sGeometries.count( pLoch ) == 0 )
      pLoch->offset( point, sGeometries );
  }
  
  sGeometries.insert( this );
}

///////////////////////////////////////////////////////////////////////////////
//  MultiSolid	                                												     //
//  Klasse für gml:MultiSolid                                                //
///////////////////////////////////////////////////////////////////////////////
MultiSolid::MultiSolid(): _Solid ()
{
  isCompositeSolid = false;
}

MultiSolid::MultiSolid ( MultiSolid * pMultiSolidOld ): _Solid ( pMultiSolidOld )
{
  for ( unsigned int i =0; i < pMultiSolidOld->vSolids.size(); i++ )
  {
    _Solid * pSolidNew = dynamic_cast<_Solid*>(_Geometrie::copy ( pMultiSolidOld->vSolids[i] ));
    vSolids.push_back( pSolidNew );
  }
}

MultiSolid::~MultiSolid()
{
  for ( unsigned int i = 0; i < vSolids.size(); i++ )
    delete vSolids[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Solid-Member über den Index                                  //
///////////////////////////////////////////////////////////////////////////////
_Solid * MultiSolid::getSolid ( size_t index )
{
  if ( index < 0 || index >= vSolids.size() ) 
    return NULL;
  else
    return vSolids[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Solid-Member                                                //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSolid::getSolids ( std::vector<_Solid*> &vSolidsP )
{
  for ( unsigned int i = 0; i < vSolids.size(); i++ )
    vSolidsP.push_back( vSolids[i] );

  return vSolids.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die BoundingBox                                                  //
///////////////////////////////////////////////////////////////////////////////
bool MultiSolid::getBoundingBox ( double * sw, double * no )
{
  bool	success;
  double  swLoc[3], noLoc[3];

  if ( bbExist )
  {
    sw[0] = m_sw[0];
    sw[1] = m_sw[1];
    sw[2] = m_sw[2];
    no[0] = m_no[0];
    no[1] = m_no[1];
    no[2] = m_no[2];
    return true;
  }

  sw[0] =  1.0E20;
  sw[1] =  1.0E20;
  sw[2] =  1.0E20;
  no[0] = -1.0E20;
  no[1] = -1.0E20;
  no[2] = -1.0E20;

  if ( vSolids.size() == 0 ) return false;

  for ( unsigned int i = 0; i < vSolids.size(); i++ )
  { 
    _Solid * pSolid = vSolids[i];
    success = pSolid->getBoundingBox ( swLoc, noLoc );
    if ( !success ) continue;

    if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
    if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
    if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
    if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
    if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
  }

  m_sw[0] = sw[0];
  m_sw[1] = sw[1];
  m_sw[2] = sw[2];
  m_no[0] = no[0];
  m_no[1] = no[1];
  m_no[2] = no[2];
  bbExist = true;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt                                                  //
///////////////////////////////////////////////////////////////////////////////
bool MultiSolid::getCenter2D ( double &xP, double &yP )
{
  double       xLoc, yLoc;
  unsigned int i;
  bool         success;

  if ( vSolids.size() == 0 ) return false;

  xP = 0.0;
  yP = 0.0;

  for ( i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pSolid = vSolids[i];
    success = pSolid->getCenter2D ( xLoc, yLoc );
    if ( !success ) return false;

    xP = xP + xLoc;
    yP = yP + yLoc;
  }

  xP = xP / vSolids.size();
  yP = yP / vSolids.size();

  return true;
}

bool MultiSolid::getCenter3D ( double &xP, double &yP, double &zP )
{
  double       xLoc, yLoc, zLoc;
  unsigned int i;
  bool         success;

  if ( vSolids.size() == 0 ) return false;

  xP = 0.0;
  yP = 0.0;
  zP = 0.0;

  for ( i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pSolid = vSolids[i];
    success = pSolid->getCenter3D ( xLoc, yLoc, zLoc );
    if ( !success ) return false;

    xP = xP + xLoc;
    yP = yP + yLoc;
    zP = zP + zLoc;
  }

  xP = xP / vSolids.size();
  yP = yP / vSolids.size();
  zP = zP / vSolids.size();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone aller Solid-Außenkonturen                           //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSolid::getAussenkonturPolygone( vector<PolygonGeo*> &vPolygone, vector<bool> &vOrientations )
{
	size_t i;

  for ( i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pSolid = vSolids[i];
    pSolid->getAussenkonturPolygone ( vPolygone, vOrientations );
  }
  return vPolygone.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone aller Solid-Innenkonturen                           //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSolid::getInnenkonturPolygone( vector<PolygonGeo*> &vPolygone, vector<bool> &vOrientations )
{
	size_t i;

  for ( i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pSolid = vSolids[i];
    pSolid->getInnenkonturPolygone ( vPolygone, vOrientations );
  }
  return vPolygone.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl alle Polygone                                         //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSolid::getPolygonAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pGeo = vSolids[i];
    anz = anz + pGeo->getPolygonAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Polygone                                                    //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSolid::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pGeo = vSolids[i];
    anz = anz + pGeo->getPolygone( vPolygone );
  }
  return anz;
}

size_t MultiSolid::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pGeo = vSolids[i];
    anz = anz + pGeo->getPolygone( vPolygone, vOrientations );
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSolid::getCurveSegmentAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pGeo = vSolids[i];
    anz = anz + pGeo->getCurveSegmentAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t MultiSolid::getPointAnz()
{
	size_t i, anz;

  anz = 0;
  for ( i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pGeo = vSolids[i];
    anz = anz + pGeo->getPointAnz();
  }
  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Summe der Volumina der einzelnen Solid-Member                //
///////////////////////////////////////////////////////////////////////////////
double MultiSolid::getVolume()
{
  double volume = 0.0;

  for ( unsigned int i = 0; i < vSolids.size(); i++ )
    volume = volume + vSolids[i]->getVolume();

  return volume;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Summe der Oberflächen der einzelnen Solid-Member             //
///////////////////////////////////////////////////////////////////////////////
double MultiSolid::getOberflaeche()
{
  double flaeche = 0.0;

  for ( unsigned int i = 0; i < vSolids.size(); i++ )
    flaeche = flaeche + vSolids[i]->getOberflaeche();

  return flaeche;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie  (BasicCheck)                                    //
///////////////////////////////////////////////////////////////////////////////
bool MultiSolid::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  bool success;
  bool successAll = true;

  if ( vSolids.size() == 0 )
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    pError->setPropertyName( featureProperty );

    vErrorReport.push_back( pError );
    pError->setFeatureGeometryId ( gmlId );

    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  for ( unsigned int i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pSolid = vSolids[i];
    success = pSolid->basicCheck ( pObj, featureProperty, pCheckParams, vErrorReport );
    if (!success ) successAll = false;
  }

  if ( successAll )
    checkResult = BASIC_CHECK_FULFILLED;
  else
    checkResult = BASIC_CHECK_FAILED;

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie  (AdvancedCheck)                                 //
///////////////////////////////////////////////////////////////////////////////
bool MultiSolid::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  bool success;
  bool successAll = true;

  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;  

  for ( unsigned int i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pSolid = vSolids[i];
    success = pSolid->advancedCheck ( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
    if (!success ) successAll = false;
  }

  if ( successAll )
    checkResult = ADVANCED_CHECK_FULFILLED;
  else
    checkResult = ADVANCED_CHECK_FAILED;

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie  (TopologyCheck)                                 //
///////////////////////////////////////////////////////////////////////////////
bool MultiSolid::topologyCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
          vector<Error*> &vErrorReport, bool checkAnyway )
{
  bool success;
  bool successAll = true;

  if ( checkResult < ADVANCED_CHECK_FULFILLED && !checkAnyway )
    return false;  

  for ( unsigned int i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pSolid = vSolids[i];
    success = pSolid->topologyCheck ( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
    if (!success ) successAll = false;
  }

  if ( successAll )
    checkResult = TOPOLOGY_CHECK_FULFILLED;
  else
    checkResult = TOPOLOGY_CHECK_FULFILLED;

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Referenzen                                                 //
///////////////////////////////////////////////////////////////////////////////
bool MultiSolid::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  unsigned int i;
  bool success, successAll;

  successAll = true;

  for ( i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pGeo = dynamic_cast<_Solid*>(vSolids[i]);
    success = pGeo->checkReferences ( pObj, propertyName, vErrorReport );
    if ( !success ) successAll = false;    
  }

  return successAll;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der Geometrie ein gültiges                   //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool MultiSolid::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  for ( unsigned int i = 0; i < vSolids.size(); i++ )
  {
    if ( vSolids[i]->existReferenceSystem( pSrsManager ) == false )
      return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt den MultiSolid um einen Offset                                //
///////////////////////////////////////////////////////////////////////////////
void MultiSolid::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  for ( unsigned int i = 0; i < vSolids.size(); i++ )
  {
    _Solid * pSolid = vSolids[i];
    if ( sGeometries.count( pSolid ) == 0 )
      pSolid->offset( point, sGeometries );
  }
  sGeometries.insert ( this );
}

///////////////////////////////////////////////////////////////////////////////
//  MultiGeometrie	                             												     //
//  Klasse für gml:AbstractGeometry                                          //
///////////////////////////////////////////////////////////////////////////////
Geometrie::Geometrie( int dimP ):_Geometrie ( dimP )
{
  pGeometrie = NULL;
}

Geometrie::Geometrie ( Geometrie * pGeometrieOld ): _Geometrie ( pGeometrieOld )
{
  if ( pGeometrieOld->pGeometrie != NULL )
    pGeometrie = _Geometrie::copy( pGeometrieOld ) ;
  else
    pGeometrie = NULL;
}

Geometrie::~Geometrie()
{
  if ( pGeometrie != NULL ) delete pGeometrie;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den abstrakten GeometryType                                      //
///////////////////////////////////////////////////////////////////////////////
ABSTRACT_GEOMETRY_TYPE Geometrie::getAbstractGeometryType()
{
  if ( pGeometrie != NULL )
    return pGeometrie->getAbstractGeometryType();
  else
    return ABSTRACT_UNKNOWN;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Geometrie                                                      //
///////////////////////////////////////////////////////////////////////////////
void Geometrie::setGeometrie ( _Geometrie * pGeo )
{
  if ( pGeometrie != NULL ) delete pGeometrie;
  pGeometrie = pGeo;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding Box                                                 //
///////////////////////////////////////////////////////////////////////////////
bool Geometrie::getBoundingBox ( double * sw, double * no )
{
 
  sw[0] =  1.0E20;
  sw[1] =  1.0E20;
  no[0] = -1.0E20;
  no[1] = -1.0E20;
  if ( dim == 3 )
  {
    sw[2] =  1.0E20;
    no[2] = -1.0E20;
  }
  else
  {
    sw[2] = 0.0;
    no[2] = 0.0;
  }

  if ( pGeometrie != NULL ) return pGeometrie->getBoundingBox( sw, no );
  else                      return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt                                                  //
///////////////////////////////////////////////////////////////////////////////
bool Geometrie::getCenter2D ( double &xP, double &yP )
{
  if ( pGeometrie != NULL ) return pGeometrie->getCenter2D( xP, yP );
  else                      return false;
}

bool Geometrie::getCenter3D ( double &xP, double &yP, double &zP )
{
  if ( pGeometrie != NULL ) return pGeometrie->getCenter3D( xP, yP, zP );
  else                      return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone                                          //
///////////////////////////////////////////////////////////////////////////////
size_t Geometrie::getPolygonAnz()
{
  if (  pGeometrie != NULL )
    return pGeometrie->getPolygonAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone                                                     //
///////////////////////////////////////////////////////////////////////////////
size_t Geometrie::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
  if (  pGeometrie != NULL )
    return pGeometrie->getPolygone( vPolygone );
  else
    return 0;
}

size_t Geometrie::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )
{
  if (  pGeometrie != NULL )
    return pGeometrie->getPolygone( vPolygone, vOrientations );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t Geometrie::getCurveSegmentAnz()
{
  if (  pGeometrie != NULL )
    return pGeometrie->getCurveSegmentAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t Geometrie::getPointAnz()
{
  if (  pGeometrie != NULL )
    return pGeometrie->getPointAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (BasicCheck)                                     //
///////////////////////////////////////////////////////////////////////////////
bool Geometrie::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  if (  pGeometrie != NULL )
  {
    bool success =  pGeometrie->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );

    if ( success )
      checkResult = BASIC_CHECK_FULFILLED;
    else
      checkResult = BASIC_CHECK_FAILED;
    return success;
  }
  else
  {
    GeometryError * pError = new GeometryError ( pObj, Error::ERROR, GeometryError::NO_GEOMETRY_SPECIFIED );
    vErrorReport.push_back( pError );
//    pErrorReport->addGeometryError ( pError );

    checkResult = BASIC_CHECK_FAILED;
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (AdvancedCheck)                                  //
///////////////////////////////////////////////////////////////////////////////
bool Geometrie::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( checkResult < BASIC_CHECK_FAILED && !checkAnyway )
    return false;

  bool success =  pGeometrie-advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );

  if ( success )
    checkResult = ADVANCED_CHECK_FULFILLED;
  else
    checkResult = ADVANCED_CHECK_FAILED;

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der Geometrie ein gültiges                   //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool Geometrie::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  if ( pGeometrie != NULL )
    return pGeometrie->existReferenceSystem ( pSrsManager );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//   Verschiebt die Geometrie um einen Offset                                //
///////////////////////////////////////////////////////////////////////////////
void Geometrie::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  if ( pGeometrie != NULL && sGeometries.count( pGeometrie ) == 0 )
    pGeometrie->offset ( point, sGeometries );

  sGeometries.insert( this );
}

///////////////////////////////////////////////////////////////////////////////
//  ImplicitGeometry                                                         //
//  Klasse für CityGML ImplicitGeometry                                      //
///////////////////////////////////////////////////////////////////////////////
ImplicitGeometry::ImplicitGeometry ( int dim ) : _Geometrie ( dim )
{
  pTransformationMatrix = NULL;
  pRelativeGMLGeometry  = NULL;
  pReferencePoint       = NULL;
}

ImplicitGeometry::ImplicitGeometry ( ImplicitGeometry * pImplicitGeometryOld )
:_Geometrie ( pImplicitGeometryOld )
{
  mimeType      = pImplicitGeometryOld->mimeType;
  libraryObject = pImplicitGeometryOld->libraryObject;

  if ( pImplicitGeometryOld->pTransformationMatrix != NULL )
    pTransformationMatrix = new Matrix4 ( pImplicitGeometryOld->pTransformationMatrix );
  else
    pTransformationMatrix = NULL;

  if ( pImplicitGeometryOld->pRelativeGMLGeometry!= NULL )
    pRelativeGMLGeometry = _Geometrie::copy( pImplicitGeometryOld->pRelativeGMLGeometry );
  else
    pRelativeGMLGeometry = NULL;

  if ( pImplicitGeometryOld->pReferencePoint != NULL  )
    pReferencePoint = new GeoPoint ( pImplicitGeometryOld->pReferencePoint );
}

ImplicitGeometry::~ImplicitGeometry()
{
  if ( pTransformationMatrix != NULL ) delete pTransformationMatrix;
  if ( pRelativeGMLGeometry != NULL )  delete pRelativeGMLGeometry;
  if ( pReferencePoint != NULL )       delete pReferencePoint;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die BoundingBox                                                //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometry::getBoundingBox ( double * sw, double * no )
{
  double swRel[3], noRel[3];
  bool   success;

  if ( bbExist )
  {
    sw[0] = m_sw[0];
    sw[1] = m_sw[1];
    no[0] = m_no[0];
    no[1] = m_no[1];
    sw[2] = m_sw[2];
    no[2] = m_no[2];
    return true;
  }

  sw[0] =  1.0E20;
  sw[1] =  1.0E20;
  no[0] = -1.0E20;
  no[1] = -1.0E20;

  if ( pRelativeGMLGeometry == NULL  ) return false;

  success =  pRelativeGMLGeometry->getBoundingBox( swRel, noRel );
  if ( !success ) return false;

  //  1. Eckpunkt
  GeoPoint p = GeoPoint ( swRel[0],  swRel[1], swRel[2] );
  if ( pTransformationMatrix != NULL )
    p = pTransformationMatrix->transform( p );

  if ( pReferencePoint != NULL )
    p = p + *pReferencePoint;

  sw[0] = p.getX();
  sw[1] = p.getY();
  sw[2] = p.getZ();
  no[0] = p.getX();
  no[1] = p.getY();
  no[2] = p.getZ();


  //  2. Eckpunkt
  p = GeoPoint (  swRel[0],   swRel[1], noRel[2] );
  if ( pTransformationMatrix != NULL )
    p = pTransformationMatrix->transform( p );

  if ( pReferencePoint != NULL )
    p = p + *pReferencePoint;

  if ( p.getX() < sw[0] ) sw[0] = p.getX();
  if ( p.getY() < sw[1] ) sw[1] = p.getY();
  if ( p.getZ() < sw[2] ) sw[2] = p.getZ();
  if ( p.getX() > no[0] ) no[0] = p.getX();
  if ( p.getY() > no[1] ) no[1] = p.getY();
  if ( p.getZ() > no[2] ) no[2] = p.getZ();


  //  3. Eckpunkt
  p = GeoPoint (  swRel[0],   noRel[1], swRel[2] );
  if ( pTransformationMatrix != NULL )
    p = pTransformationMatrix->transform( p );

  if ( pReferencePoint != NULL )
    p = p + *pReferencePoint;

  if ( p.getX() < sw[0] ) sw[0] = p.getX();
  if ( p.getY() < sw[1] ) sw[1] = p.getY();
  if ( p.getZ() < sw[2] ) sw[2] = p.getZ();
  if ( p.getX() > no[0] ) no[0] = p.getX();
  if ( p.getY() > no[1] ) no[1] = p.getY();
  if ( p.getZ() > no[2] ) no[2] = p.getZ();


  //  4. Eckpunkt
  p = GeoPoint (  swRel[0],   noRel[1], noRel[2] );
  if ( pTransformationMatrix != NULL )
    p = pTransformationMatrix->transform( p );

  if ( pReferencePoint != NULL )
    p = p + *pReferencePoint;

  if ( p.getX() < sw[0] ) sw[0] = p.getX();
  if ( p.getY() < sw[1] ) sw[1] = p.getY();
  if ( p.getZ() < sw[2] ) sw[2] = p.getZ();
  if ( p.getX() > no[0] ) no[0] = p.getX();
  if ( p.getY() > no[1] ) no[1] = p.getY();
  if ( p.getZ() > no[2] ) no[2] = p.getZ();


  //  5. Eckpunkt
  p = GeoPoint ( noRel[0],  swRel[1], swRel[2] );
  if ( pTransformationMatrix != NULL )
    p = pTransformationMatrix->transform( p );

  if ( pReferencePoint != NULL )
    p = p + *pReferencePoint;

  if ( p.getX() < sw[0] ) sw[0] = p.getX();
  if ( p.getY() < sw[1] ) sw[1] = p.getY();
  if ( p.getZ() < sw[2] ) sw[2] = p.getZ();
  if ( p.getX() > no[0] ) no[0] = p.getX();
  if ( p.getY() > no[1] ) no[1] = p.getY();
  if ( p.getZ() > no[2] ) no[2] = p.getZ();


  //  6. Eckpunkt
  p = GeoPoint (  noRel[0],   swRel[1], noRel[2] );
  if ( pTransformationMatrix != NULL )
    p = pTransformationMatrix->transform( p );

  if ( pReferencePoint != NULL )
    p = p + *pReferencePoint;

  if ( p.getX() < sw[0] ) sw[0] = p.getX();
  if ( p.getY() < sw[1] ) sw[1] = p.getY();
  if ( p.getZ() < sw[2] ) sw[2] = p.getZ();
  if ( p.getX() > no[0] ) no[0] = p.getX();
  if ( p.getY() > no[1] ) no[1] = p.getY();
  if ( p.getZ() > no[2] ) no[2] = p.getZ();


  //  7. Eckpunkt
  p = GeoPoint (  noRel[0],   noRel[1], swRel[2] );
  if ( pTransformationMatrix != NULL )
    p = pTransformationMatrix->transform( p );

  if ( pReferencePoint != NULL )
    p = p + *pReferencePoint;

  if ( p.getX() < sw[0] ) sw[0] = p.getX();
  if ( p.getY() < sw[1] ) sw[1] = p.getY();
  if ( p.getZ() < sw[2] ) sw[2] = p.getZ();
  if ( p.getX() > no[0] ) no[0] = p.getX();
  if ( p.getY() > no[1] ) no[1] = p.getY();
  if ( p.getZ() > no[2] ) no[2] = p.getZ();


  //  8. Eckpunkt
  p = GeoPoint (  noRel[0],   noRel[1], noRel[2] );
  if ( pTransformationMatrix != NULL )
    p = pTransformationMatrix->transform( p );

  if ( pReferencePoint != NULL )
    p = p + *pReferencePoint;

  if ( p.getX() < sw[0] ) sw[0] = p.getX();
  if ( p.getY() < sw[1] ) sw[1] = p.getY();
  if ( p.getZ() < sw[2] ) sw[2] = p.getZ();
  if ( p.getX() > no[0] ) no[0] = p.getX();
  if ( p.getY() > no[1] ) no[1] = p.getY();
  if ( p.getZ() > no[2] ) no[2] = p.getZ();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den 2D-Schwerpunkt                                             //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometry::getCenter2D ( double &xP, double &yP )
{
  bool success;
  double x, y;

  if ( pRelativeGMLGeometry == NULL  ) return false;

  success =  pRelativeGMLGeometry->getCenter2D( x, y );
  if ( !success ) return false;

  GeoPoint p = GeoPoint ( x,y );

  if ( pTransformationMatrix != NULL )
    p = pTransformationMatrix->transform( p );

  if ( pReferencePoint != NULL )
    p = p + *pReferencePoint;

  xP = p.getX();
  yP = p.getY();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den 3D-Schwerpunkt                                             //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometry::getCenter3D ( double &xP, double &yP, double &zP )
{
  bool success;
  double x, y, z;

  if ( pRelativeGMLGeometry == NULL  ) return false;

  success =  pRelativeGMLGeometry->getCenter3D( x, y, z );
  if ( !success ) return false;

  GeoPoint p = GeoPoint ( x,y, z );

  if ( pTransformationMatrix != NULL )
    p = pTransformationMatrix->transform( p );

  if ( pReferencePoint != NULL )
    p = p + *pReferencePoint;

  xP = p.getX();
  yP = p.getY();
  zP = p.getZ();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone                                          //
///////////////////////////////////////////////////////////////////////////////
size_t ImplicitGeometry::getPolygonAnz()
{
  if (  pRelativeGMLGeometry != NULL )
    return pRelativeGMLGeometry->getPolygonAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone                                                     //
///////////////////////////////////////////////////////////////////////////////
size_t ImplicitGeometry::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
  if (  pRelativeGMLGeometry != NULL )
    return pRelativeGMLGeometry->getPolygone( vPolygone );
  else
    return 0;
}

size_t ImplicitGeometry::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )
{
  if (  pRelativeGMLGeometry != NULL )
    return pRelativeGMLGeometry->getPolygone( vPolygone, vOrientations );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
size_t ImplicitGeometry::getCurveSegmentAnz()
{
  if (  pRelativeGMLGeometry != NULL )
    return pRelativeGMLGeometry->getCurveSegmentAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
size_t ImplicitGeometry::getPointAnz()
{
  if (  pRelativeGMLGeometry != NULL )
    return pRelativeGMLGeometry->getPointAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzen der Pointer-Attribute                                             //
///////////////////////////////////////////////////////////////////////////////
void ImplicitGeometry::setTransformationMatrix ( Matrix4 * pMatrixP )
{
  if ( pTransformationMatrix != NULL ) delete pTransformationMatrix;
  pTransformationMatrix = pMatrixP;
}

void ImplicitGeometry::setRelativeGMLGeometry ( _Geometrie * pGeo )
{
  if ( pRelativeGMLGeometry != NULL ) delete pRelativeGMLGeometry;
  pRelativeGMLGeometry = pGeo;
}

void ImplicitGeometry::setReferencePoint ( GeoPoint * pPoint )
{
  if ( pReferencePoint != NULL ) delete pReferencePoint;
  pReferencePoint = pPoint;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die zugehörige explizite Geometrie                               //
///////////////////////////////////////////////////////////////////////////////
void ImplicitGeometry::getExplicitGeometry ( vector<_Curve*> & vCurves, vector<PolygonGeo*> &vPolygone, vector<bool> &vOrientations )
{
  _Surface          * pSurface;
  _Curve            * pCurve;
  _Solid            * pSolid;
  _Geometrie        * pGeometrie;
  PolygonGeo           * pPolygon;
  PolygonGeo           * pPolygonTransformed;
  Curve             * pCurveTransformed;
  MultiCurve        * pMultiCurve;
  MultiSurface      * pMultiSurface;
  MultiSolid        * pMultiSolid;
  unsigned int       i;
  vector<PolygonGeo*>   vPolygoneLoc;
  vector<_Curve*>    vCurvesLoc;

  if ( pRelativeGMLGeometry == NULL ) return;

  if ( pRelativeGMLGeometry->getGeometryType() == GEOMETRY_REFERENCE )
  {
    GeometryReference * pGeoReference = dynamic_cast<GeometryReference*>(pRelativeGMLGeometry);
    pGeometrie = pGeoReference->getRefGeoObjekt();  
  }
  else
    pGeometrie = pRelativeGMLGeometry;

  switch ( pGeometrie->getAbstractGeometryType() )
  {
  case ABSTRACT_CURVE:
    pCurve = dynamic_cast<_Curve*>(pGeometrie);
    vCurvesLoc.push_back( pCurve );
    break;

  case ABSTRACT_SURFACE:
    pSurface = dynamic_cast<_Surface*>(pGeometrie);
    pSurface->getPolygone( vPolygoneLoc, vOrientations );
    break;

  case ABSTRACT_SOLID:
    pSolid = dynamic_cast<_Solid*>(pGeometrie);
    pSolid->getAussenkonturPolygone( vPolygoneLoc, vOrientations );
    break;

  case ABSTRACT_MULTI_GEOMETRY:
    {
      switch ( pGeometrie->getGeometryType() )
      {
      case MULTI_CURVE:
        pMultiCurve = dynamic_cast<MultiCurve*>(pGeometrie);
        for ( i = 0; i < pMultiCurve->getCurveMemberAnz(); i++ )
        {
          pCurve = pMultiCurve->getCurve( i );
          vCurvesLoc.push_back( pCurve );
        }
        break;

      case MULTI_SURFACE:
        pMultiSurface =  dynamic_cast<MultiSurface*>(pGeometrie);
        pMultiSurface->getPolygone( vPolygoneLoc, vOrientations );
        break;

      case MULTI_SOLID:
        pMultiSolid = dynamic_cast<MultiSolid*>(pGeometrie);
        for ( i = 0; i < pMultiSolid->getSolidMemberAnz(); i++ )
        {
          pSolid = pMultiSolid->getSolid( i );
          pSolid->getAussenkonturPolygone( vPolygoneLoc, vOrientations );
        }
        break;
      }
      break;
    }
  }

  for ( i = 0; i < vCurves.size(); i++ )
  {
    pCurve = vCurvesLoc[i];

    switch ( pCurve->getGeometryType() )
    {
    case CURVE:
      pCurveTransformed = new Curve ( dynamic_cast<Curve*>(pCurve) );
      pCurveTransformed->transform ( *pTransformationMatrix );
      vCurves.push_back( pCurveTransformed );
      break;
    }
  }

  for ( i = 0; i < vPolygoneLoc.size(); i++ )
  {
    pPolygon = vPolygoneLoc[i];
    pPolygonTransformed = new PolygonGeo ( pPolygon );
    pPolygonTransformed->transform ( *pTransformationMatrix );
    vPolygone.push_back( pPolygonTransformed );
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (BasicCheck)                                     //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometry::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,vector<Error*> &vErrorReport )
{
  if ( pRelativeGMLGeometry == NULL || pTransformationMatrix == NULL )
  {
    checkResult = BASIC_CHECK_FAILED;
    return false;
  }

  bool success = pRelativeGMLGeometry->basicCheck( pObj, featureProperty, pCheckParams, vErrorReport );
  
  if ( success )
    checkResult = BASIC_CHECK_FULFILLED;
  else
    checkResult = BASIC_CHECK_FAILED;
 
  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Geometrie (AdvancedCheck)                                  //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometry::advancedCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( checkResult < BASIC_CHECK_FULFILLED && !checkAnyway )
    return false;

  bool success = pRelativeGMLGeometry->advancedCheck( pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );

  if ( success )
    checkResult = ADVANCED_CHECK_FULFILLED;
  else
    checkResult = ADVANCED_CHECK_FAILED;

    return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für die Geometrie ein gültiges                              //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometry::existReferenceSystem( SrsManager * pSrsManager )
{
  if ( srsName != "" )
    return pSrsManager->existCoordSystem( srsName );

  if ( pReferencePoint != NULL )
    return pReferencePoint->existReferenceSystem( pSrsManager );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt die ImplicitGeometry um einen Offset                          //
///////////////////////////////////////////////////////////////////////////////
void ImplicitGeometry::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  if ( pRelativeGMLGeometry != NULL && sGeometries.count( pRelativeGMLGeometry ) == 0 )
    pRelativeGMLGeometry->offset( point, sGeometries );

  sGeometries.insert( this );
}

///////////////////////////////////////////////////////////////////////////////
//  GeometryReference                                                        //
//  Referenz auf eine ImplicitGeometry                                       //
///////////////////////////////////////////////////////////////////////////////
ImplicitGeometryReference::ImplicitGeometryReference( Features * pFeaturesP ): _Geometrie ( 2 ), _GeometryReference (pFeaturesP )
{
  pFeatures = pFeaturesP;
}

ImplicitGeometryReference::ImplicitGeometryReference ( ImplicitGeometryReference * pGeoReferenceOld )
  :_Geometrie ( pGeoReferenceOld ), _GeometryReference ( pGeoReferenceOld )
{

}

ImplicitGeometryReference::~ImplicitGeometryReference()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den abstrakten Geometrietyp der referierten Geometrie            //
///////////////////////////////////////////////////////////////////////////////
ABSTRACT_GEOMETRY_TYPE ImplicitGeometryReference::getAbstractGeometryType()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getAbstractGeometryType();
  else
    return ABSTRACT_UNKNOWN;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die Dimension der referierten Geometrie, oder 0 falls keine      //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
int ImplicitGeometryReference::getDim()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getDim();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den srsName des referierten Geometrie, oder "" falls keine       //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
std::string ImplicitGeometryReference::getSrsName ()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getSrsName();
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob für alle Teile der referierten Geometrie ein gültiges       //
//  Referenzsystem definiert ist                                             //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometryReference::existReferenceSystem( SrsManager * pSrsManager )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->existReferenceSystem( pSrsManager );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den BoundingBox der referierten Geometrie, oder false falls keine//
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometryReference::getBoundingBox ( double * sw, double * no )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getBoundingBox( sw, no );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Center2D der referierten Geometrie, oder false falls keine   //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometryReference::getCenter2D ( double &xP, double &yP )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCenter2D( xP, yP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Center3D der referierten Geometrie, oder false falls keine   //
//  gültige Referenz existiert                                               //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometryReference::getCenter3D ( double &xP, double &yP, double &zP )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCenter3D( xP, yP, zP );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone der referierten Geometrie                //
///////////////////////////////////////////////////////////////////////////////
size_t ImplicitGeometryReference::getPolygonAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygonAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Polygone der referierten Geometrie                           //
///////////////////////////////////////////////////////////////////////////////
size_t ImplicitGeometryReference::getPolygone ( std::vector<PolygonGeo*> &vPolygone )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygone( vPolygone );
  else
    return 0;
}

size_t ImplicitGeometryReference::getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPolygone( vPolygone, vOrientations );
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments der referierten Geometrie           //
///////////////////////////////////////////////////////////////////////////////
size_t ImplicitGeometryReference::getCurveSegmentAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getCurveSegmentAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte der referierten Geometrie                  //
///////////////////////////////////////////////////////////////////////////////
size_t ImplicitGeometryReference::getPointAnz()
{
  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return pGeo->getPointAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Prüft die Geometrie (BasicCheck)                                         //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometryReference::basicCheck ( Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
          vector<Error*> &vErrorReport  )
{
  if ( !pCheckParams->checkReferencedGeometry )
    return true;

  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return basicCheck (  pObj, featureProperty, pCheckParams, vErrorReport );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Prüft die Geometrie (AdvancedCheck)                                      //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometryReference::advancedCheck (  Feature * pObj, string featureProperty, CheckingParams * pCheckParams,
        vector<Error*> &vErrorReport, bool checkAnyway )
{
  if ( !pCheckParams->checkReferencedGeometry )
    return true;

  _Geometrie * pGeo = pFeatures->getGeometry( refGeoId );
  if ( pGeo != NULL ) 
    return advancedCheck (  pObj, featureProperty, pCheckParams, vErrorReport, checkAnyway );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Referenzen                                                 //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometryReference::checkReferences ( Feature * pObj, std::string propertyName,vector<Error*> &vErrorReport )
{
  string          errorString;
  SemanticError * pError;
  GEOMETRY_TYPE   geoType;

  _Geometrie * pRefGeo = pFeatures->getGeometry( refGeoId );
  if ( pRefGeo == NULL )
  {
    errorString = "Das Geometrieattribut " + propertyName + " hat ein nicht-aufgelöstes xlink: " + refGeoId;
    pError = new SemanticError ( pObj, Error::ERROR, errorString );    
    vErrorReport.push_back( pError );

    return false;
  }

  geoType = pRefGeo->getGeometryType();

  if ( geoType != IMPLICIT_GEOMETRY )
  {
    errorString = "Das Geometrieattribut " + propertyName + " verweist mit " + refGeoId + " auf einen falschen Geometrietyp. ";
    errorString = errorString + "Notwendig ist Flächengeometrie, verwiesen wird auf " + _Geometrie::convertTypeToString( geoType );
    pError = new SemanticError ( pObj, Error::ERROR, errorString );    
    vErrorReport.push_back( pError );

    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Offset auf die referierte implizite Geometrie                            //
//  Über den Set von Geometrieobjekten wird sichergestellt, dass ein         //
//  bestimmtes Objekt nur einmal verschoben wird                             //
///////////////////////////////////////////////////////////////////////////////
void ImplicitGeometryReference::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  _Geometrie * pGeo = getRefGeoObjekt();

  if ( pGeo != NULL && sGeometries.count( pGeo ) == 0 )
  {
    pGeo->offset( point, sGeometries );
    sGeometries.insert( pGeo );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Matrix4                                                                  //
//  Klasse für eine homogene Matrix                                          //
///////////////////////////////////////////////////////////////////////////////
Matrix4::Matrix4()
{
  m[0][0] = 1.0;
  m[0][1] = 0.0;
  m[0][2] = 0.0;
  m[0][3] = 0.0;

  m[1][0] = 0.0;
  m[1][1] = 1.0;
  m[1][2] = 0.0;
  m[1][3] = 0.0;

  m[2][0] = 0.0;
  m[2][1] = 0.0;
  m[2][2] = 1.0;
  m[2][3] = 0.0;
}

Matrix4::Matrix4 ( double xx, double xy, double xz, double xt,
                   double yx, double yy, double yz, double yt,
                   double zx, double zy, double zz, double zt )
{
  m[0][0] =  xx; 
  m[0][1] =  xy; 
  m[0][2] =  xz;
  m[0][3] =  xt;

  m[1][0] =  yx;
  m[1][1] =  yy;
  m[1][2] =  yz;
  m[1][3] =  yt;

  m[2][0] =  zx;
  m[2][1] =  zy; 
  m[2][2] =  zz;
  m[2][3] =  zt;
}

Matrix4::Matrix4 ( Matrix4 * pMatrixOld )
{
  m[0][0] =  pMatrixOld->m[0][0]; 
  m[0][1] =  pMatrixOld->m[0][1]; 
  m[0][2] =  pMatrixOld->m[0][2];
  m[0][3] =  pMatrixOld->m[0][3];

  m[1][0] =  pMatrixOld->m[1][0];
  m[1][1] =  pMatrixOld->m[1][1];
  m[1][2] =  pMatrixOld->m[1][2];
  m[1][3] =  pMatrixOld->m[1][3];

  m[2][0] =  pMatrixOld->m[2][0];
  m[2][1] =  pMatrixOld->m[2][1]; 
  m[2][2] =  pMatrixOld->m[2][2];
  m[2][3] =  pMatrixOld->m[2][3];
}

Matrix4:: ~Matrix4()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Elemente der Matrix zeilenweise als vector                   //
///////////////////////////////////////////////////////////////////////////////
void Matrix4::getMatrix ( double matrix[] ) const
{
  int i, j;

  for ( j = 0; j < 3; j++ )
    for ( i = 0; i < 4; i++ )
      matrix[4*j+i] = m[j][i];
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert einen GeoPoint mit der Matrix                              //
///////////////////////////////////////////////////////////////////////////////
GeoPoint Matrix4::transform ( GeoPoint & p )
{
  double   coord[3];
  double   coordTrans[3];
  unsigned int i, j;

  coord[0] = p.getX();
  coord[1] = p.getY();
  coord[2] = p.getZ();

  for ( i = 0; i < 3; i++ )
  {
    coordTrans[i] = 0.0;
    for ( j = 0; j < 3; j++ )
      coordTrans[i] = coordTrans[i] + m[i][j]*coord[j];
    coordTrans[i] = coordTrans[i] + m[i][3];
  }
  
  return GeoPoint ( coordTrans[0],  coordTrans[1], coordTrans[2] );
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert einen GeoPoint mit der Matrix                              //
///////////////////////////////////////////////////////////////////////////////
void Matrix4::transform ( double p[], double pTrans[] )
{
  unsigned int i, j;

  for ( i = 0; i < 3; i++ )
  {
    pTrans[i] = 0.0;
    for ( j = 0; j < 3; j++ )
      pTrans[i] = pTrans[i] + m[i][j]*p[j];
    pTrans[i] = pTrans[i] + m[i][3];
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert einen GeoPoint mit der inversen Matrix                     //
///////////////////////////////////////////////////////////////////////////////
GeoPoint Matrix4::transformInvers ( GeoPoint & p )
{
  Matrix4 matInvers = invers();
  return matInvers.transform ( p );
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert einen GeoPoint mit der inversen Matrix                     //
///////////////////////////////////////////////////////////////////////////////
void Matrix4::transformInvers ( double p[], double pTrans[] )
{
  Matrix4 matInvers = invers();
  matInvers.transform( p, pTrans );
}

///////////////////////////////////////////////////////////////////////////////
//  Multiplikation zweier Matrizen                                           //
///////////////////////////////////////////////////////////////////////////////
Matrix4 Matrix4::operator* ( Matrix4 & mat )
{
  double multMat[3][4];
  int i, j, k;

  for ( i = 0; i < 3; i++ )
  {
    for ( j = 0; j < 4; j++ )
    {
      multMat[i][j] = 0.0;
      for ( k = 0; k < 3; k++ )
        multMat[i][j] = multMat[i][j] + m[i][k]*mat.m[k][j];
      if ( j == 3 )
        multMat[i][3] = multMat[i][3] + m[i][3];
    }
  }

  return Matrix4 (  multMat[0][0], multMat[0][1], multMat[0][2], multMat[0][3],
                    multMat[1][0], multMat[1][1], multMat[1][2], multMat[1][3],
                    multMat[2][0], multMat[2][1], multMat[2][2], multMat[2][3] );
}

///////////////////////////////////////////////////////////////////////////////
//  Inverse  Matrix                                                          //
///////////////////////////////////////////////////////////////////////////////
Matrix4 Matrix4::invers()
{
  double mTranspon[3][3];
  double x[3];
  int    i, j;

  for ( i = 0; i < 3; i++ )
  for ( j = 0; j < 3; j++ )
   mTranspon[i][j] = m[j][i];

  for ( i = 0; i < 3; i++ )
  {
    x[i] = 0.0;
    for ( j = 0; j < 3; j++ )
      x[i] = x[i] + mTranspon[i][j]*m[j][3];
  }

  return Matrix4 (  mTranspon[0][0], mTranspon[0][1], mTranspon[0][2], -x[0],
                    mTranspon[1][0], mTranspon[1][1], mTranspon[1][2], -x[1],
                    mTranspon[2][0], mTranspon[2][1], mTranspon[2][2], -x[2] );
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine Matrix zur Rotation um die Z-Achse und Verschiebung um  p //
//  angle in Radian
///////////////////////////////////////////////////////////////////////////////
void Matrix4::createRotationXY ( GeoPoint &p, double angle)
{
  m[0][0] = cos ( angle );
  m[0][1] = sin ( angle );
  m[0][2] = 0.0;
  m[0][3] = p.getX();

  m[1][0] = -sin ( angle );
  m[1][1] = cos ( angle );
  m[1][2] = 0.0;
  m[1][3] = p.getY();

  m[2][0] = 0.0;
  m[2][1] = 0.0;
  m[2][2] = 1.0;
  m[2][3] = 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Klasse für Richtungsvektoren                                             //
///////////////////////////////////////////////////////////////////////////////
Direction::Direction()
{
  x = 0.0;
  y = 0.0;
  z = 0.0;
}

Direction::Direction ( const double xP, const double yP, const double zP )
{
  x = xP;
  y = yP;
  z = zP;
}

Direction::Direction ( const GeoPoint &from, const GeoPoint & to )
{
  x = to.getX() - from.getX();
  y = to.getY() - from.getY();
  z = to.getZ() - from.getZ();
}

Direction::Direction ( const Direction  & dir )
{
  x = dir.x;
  y = dir.y;
  z = dir.z;
}

///////////////////////////////////////////////////////////////////////////////
//  Operator =  Zuweisung                                                   //
///////////////////////////////////////////////////////////////////////////////
Direction & Direction::operator= ( const Direction& rhs )
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;

  return  *this;
}
///////////////////////////////////////////////////////////////////////////////
//  Liefert den Richtungsvektor                                              //
///////////////////////////////////////////////////////////////////////////////
void Direction::get ( double &xP, double &yP, double &zP )
{
  xP = x;
  yP = y;
  zP = z;
}

///////////////////////////////////////////////////////////////////////////////
//  Operator +  Vektorielle Addition                                         //
///////////////////////////////////////////////////////////////////////////////
Direction Direction::operator+ ( const Direction& rhs )
{
  Direction d ( x + rhs.x,  y + rhs.y, z + rhs.z );
  return d;
 }

///////////////////////////////////////////////////////////////////////////////
//  Operator -  Vektorielle Subtraktion                                      //
///////////////////////////////////////////////////////////////////////////////
Direction Direction::operator- (  const Direction& rhs )
{
  Direction d ( x - rhs.x,  y - rhs.y, z - rhs.z );
  return d;
}

///////////////////////////////////////////////////////////////////////////////
//  Operator *  Skalarprodukt                                                //
///////////////////////////////////////////////////////////////////////////////
double Direction::operator* (  const Direction& rhs )
{
  double d = x*rhs.x + y*rhs.y + z*rhs.z;
  return d;
}

///////////////////////////////////////////////////////////////////////////////
//  Normalisiert den Vektor auf Länge 1                                      //
///////////////////////////////////////////////////////////////////////////////
void Direction::normalize()
{
  double d = sqrt ( x*x + y*y + z*z );
  if ( d != 0.0 )
  {
    x = x / d;
    y = y / d;
    z = z / d;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Kehrt die Richtung des Vektors um                                        //
///////////////////////////////////////////////////////////////////////////////
void Direction::reverse()
{
  x = -x;
  y = -y;
  z = -z;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Länge des Vektors                                            //
///////////////////////////////////////////////////////////////////////////////
double Direction::getLength()
{
  double d = sqrt ( x*x + y*y + z*z );
  return d;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Raumwinkel-Differenz (in Radian) der Vektoren                //
///////////////////////////////////////////////////////////////////////////////
double Direction::getDifference ( Direction& rhs )
{
  double n2x, n2y, n2z;
  double cos_angle;

  n2x = rhs.getX();
  n2y = rhs.getY();
  n2z = rhs.getZ();

  cos_angle = x*n2x + y*n2y + z*n2z;
  if ( cos_angle < -1.0 ) cos_angle = -1.0;
  if ( cos_angle >  1.0 ) cos_angle =  1.0;

  double angleDifference = acos( cos_angle );
  return angleDifference;
}

///////////////////////////////////////////////////////////////////////////////
//  Vektorprodukt der Vektoren                                               //
///////////////////////////////////////////////////////////////////////////////
Direction Direction::crossProduct ( Direction & dir )
{
  double xLoc, yLoc, zLoc;

  xLoc =  y*dir.z - z*dir.y;
  yLoc = -x*dir.z + z*dir.x;
  zLoc =  x*dir.y - y*dir.x;

  return Direction ( xLoc, yLoc, zLoc );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Wert und index der größten Komponente des Vektors                //
///////////////////////////////////////////////////////////////////////////////
double Direction::getMaxComponent( int &index)
{
  double max;

  if ( fabs ( x )  > fabs( y ) )
  {
    index = 0;
    max = x;
  }
  else
  {
    index = 1;
    max = y;
  }

  if ( fabs( z ) > fabs( max ) )
  {
    index = 2;
    return z;
  }
  else
    return max;
}

///////////////////////////////////////////////////////////////////////////////
//  Operator [}  Zugriff auf Vektor-Komponente                               //
///////////////////////////////////////////////////////////////////////////////
double Direction::operator[] ( int index )
{
  switch ( index )
  {
  case 0: return x;
  case 1: return y;
  case 2: return z;
  }

  return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//  Skalierung des Vektors                                                   //
///////////////////////////////////////////////////////////////////////////////
Direction Direction::scale ( double c )
{
  return Direction ( c*x, c*y, c*z );
}

///////////////////////////////////////////////////////////////////////////////
//  Umwandlung in einen GeoPoint                                             //
///////////////////////////////////////////////////////////////////////////////
GeoPoint Direction::toPoint()
{
  return GeoPoint ( x, y, z );
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet Azimut und Erhebungswinkel des Vektors                         //
///////////////////////////////////////////////////////////////////////////////
void Direction::getAzimutAndInclination ( double & azimut, double & inclination )
{
  double nxy = sqrt (x*x + y*y );
  if ( nxy < 1.0E-3 )
  {
    azimut = 0.0;
    if ( z > 0.0 )
      inclination = 0.0;
    else
      inclination = 180.0;
  }
  else
  {
    azimut = ( acos ( y/ nxy ) * 180.0) / PI;
    if ( x < 0.0)
      azimut = 360.0 - azimut;

    inclination = ( asin ( nxy ) * 180.0 ) / PI;
    if ( inclination < 0.0 )
      inclination = 90.0 - inclination;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Klasse für Ebenen                                                        //
///////////////////////////////////////////////////////////////////////////////
Plane::Plane ()
{
  point = GeoPoint(3);
  normal = Direction();

}
Plane::Plane ( const GeoPoint &p, const Direction &n )
{ 
  point  = GeoPoint ( p );
  normal = Direction ( n );
  normal.normalize();
}

Plane::Plane ( const Plane &pl )
{
  point  = GeoPoint ( pl.point );
  normal = Direction ( pl.normal );
}

Plane::Plane ( Plane *plOld )
{
  point = GeoPoint ( plOld->point );
  normal = Direction ( plOld->normal );
}

Plane::~Plane()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den Abstand eines Punktes von der Ebene                        //
///////////////////////////////////////////////////////////////////////////////
double Plane::getDistance ( GeoPoint p )
{
  double d;

  Direction dist = Direction ( p, point );
  d = dist * normal;

  return fabs ( d );
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den Abstand der Ebene vom Nullpunkt                            //
///////////////////////////////////////////////////////////////////////////////
double Plane::getDistance ()
{
  return point.getX()*normal.getX() +  point.getY()*normal.getY() + point.getZ()*normal.getZ();
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Transformationsmatrix, mit der die Ebene in die (x,y)-Ebene//
//  gedreht wird                                                             //
///////////////////////////////////////////////////////////////////////////////
Matrix4 Plane::getTransformationToHorizontal()
{
  double  nx, ny, nz;
  double  px, py, pz;
  double  xx, xy, xz, xt, yx, yy, yz, yt, zx, zy, zz, zt;
  double  d;

  normal.get( nx, ny, nz );
  px = point.getX ();
  py = point.getY ();
  pz = point.getZ ();

  d = sqrt ( nx*nx + ny*ny );

  if ( d > MINIMAL_ANGLE_DISTANCE )
  {
    if ( fabs( ny ) < MINIMAL_ANGLE_DISTANCE ) // Normale in (x,z) Ebene
    {
      xx = nz;
      xy = 0.0;
      xz = -nx;
      xt = nx*pz - nz*px;

      yx = 0.0;
      yy = 1,0;
      yz = 0.0;
      yt = -py;

      zx = nx;
      zy = 0.0;
      zz = nz;
      zt = -nx*px -nz*pz;
    }
    else
    if ( fabs( nx ) < MINIMAL_ANGLE_DISTANCE ) // Normale in (y,z) Ebene
    {
      xx = 1.0;
      xy = 0.0;
      xz = 0.0;
      xt = -px;

      yx = 0.0; 
      yy = nz; 
      yz = -ny;
      yt = ny*pz - nz*py;

      zx = 0.0;
      zy = ny;
      zz = nz;
      zt = -ny*py - nz*pz;
    }
    else  //  Normale mit x, y und z-Komponente
    {
      xx = ny / d;
      xy = -nx / d;
      xz = 0.0;
      xt = ( nx*py - ny*px ) / d;

      yx = nx*nz / d;
      yy = ny*nz / d;
      yz = -d;
      yt = d*pz - (( nx*px + ny*py )*nz / d);

      zx = nx;
      zy = ny;
      zz = nz;
      zt = -( nx*px + ny*py + nz*pz );
    }
  }
  else //  Normale in +z oder -z Richtung
  {
    xx = 1.0;
    xy = 0.0;
    xz = 0.0;
    xt = -px;

    yx = 0.0;
    if ( nz > 0.0 ) yy = 1.0;
    else            yy = -1.0;
    yz = 0.0;
    if ( nz > 0.0 ) yt = -py;
    else            yt = py;

    zx = 0.0;
    zy = 0.0;
    if ( nz > 0.0 )
    {
      zz = 1.0;
      zt = -pz;
    }
    else
    {
      zz = -1.0;
      zt = pz;
    }
  }

  Matrix4 trans = Matrix4 ( xx, xy, xz, xt, yx, yy, yz, yt, zx, zy, zz, zt );

  return trans;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob die Ebene horizontal ist                                    //
///////////////////////////////////////////////////////////////////////////////
bool Plane::isHorizontal ()
{
  double h = normal.getX()*normal.getX() + normal.getY()*normal.getY();
  if ( h < MINIMAL_ANGLE_DISTANCE )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob die Ebene parallel zur x-z Ebene ist                        //
///////////////////////////////////////////////////////////////////////////////
bool Plane::isXZPlane ()
{
  double h = normal.getX()*normal.getX() + normal.getZ()*normal.getZ();
  if ( h < MINIMAL_ANGLE_DISTANCE )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob die Ebene parallel zur y-z Ebene ist                        //
///////////////////////////////////////////////////////////////////////////////
bool Plane::isYZPlane ()
{
  double h = normal.getY()*normal.getY() + normal.getZ()*normal.getZ();
  if ( h < MINIMAL_ANGLE_DISTANCE )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Kehrt die Normalenrichtung der Ebene um                                  //
///////////////////////////////////////////////////////////////////////////////
void Plane::reverse()
{
  normal.reverse();
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Winkeldifferenz der Normalenvektoren sowie den senkrechten  //
//  Abstand des Definitionspunktes von pPlane von der Objektebene            //
///////////////////////////////////////////////////////////////////////////////
void Plane::getDifference ( Plane * pPlane, double &angleDifference, double &pointDifference )
{
  double n1x, n1y, n1z, n2x, n2y, n2z;
  double cos_angle;
  Direction normalA = pPlane->getNormal();

  n1x = normal.getX();
  n1y = normal.getY();
  n1z = normal.getZ();
  n2x = normalA.getX();
  n2y = normalA.getY();
  n2z = normalA.getZ();

  cos_angle = n1x*n2x + n1y*n2y + n1z*n2z;
  if ( cos_angle < -1.0 ) cos_angle = -1.0;
  if ( cos_angle >  1.0 ) cos_angle =  1.0;

  angleDifference = acos( cos_angle );
  pointDifference = getDistance( pPlane->getCenter() );
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Schnittlinie zweier Ebenen                                  //
///////////////////////////////////////////////////////////////////////////////
bool Plane::cut ( Plane & plane, Line & cutLine, double minimalAngleDistance )
{
  double d1, d2, s1, s2, n1_n2;
  Direction n1 = getNormal();
  Direction n2 = plane.getNormal();

  Direction dir = n1.crossProduct( n2 );
  if ( dir.getLength() < minimalAngleDistance )
    return false;

  d1 = getDistance() - point.mult( n1.toPoint() );
  d2 = plane.getDistance() - point.mult ( n2.toPoint() );

  n1_n2 = n1.getX()*n2.getX() + n1.getY()*n2.getY() + n1.getZ()*n2.getZ();

  s1 = ( d1 - n1_n2*d2 ) / ( 1.0 - n1_n2*n1_n2 );
  s2 = ( d2 - d1*n1_n2 ) / ( 1.0 - n1_n2*n1_n2 );

  GeoPoint p = GeoPoint ( s1*n1.getX() + s2*n2.getX(),  s1*n1.getY() + s2*n2.getY(),  s1*n1.getZ() + s2*n2.getZ() );

  cutLine.set ( p, dir );
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Klasse für Linien                                                        //
///////////////////////////////////////////////////////////////////////////////
Line::Line ()
{
  point = GeoPoint(3);
  dir = Direction();

}
Line::Line ( const GeoPoint &p, const Direction &line )
{ 
  point  = GeoPoint ( p );
  dir    = Direction ( line );
  dir.normalize();
}

Line::Line ( const Line & line )
{
  point = GeoPoint ( line.point );
  dir   = Direction ( line.dir );
}
Line::~Line()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Definitionsparameter der Linie                                 //
///////////////////////////////////////////////////////////////////////////////
void Line::set ( GeoPoint p, Direction d )
{
  point = p;
  dir   = d;
  dir.normalize();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Abstand des Punktes von der Linie                            //
///////////////////////////////////////////////////////////////////////////////
double Line::getDistance ( GeoPoint p )
{
  Direction h = Direction ( point, p );
  Direction distD = dir.crossProduct( h );

  return distD.getLength();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die relative Position des Punktes auf der Linie                  //
///////////////////////////////////////////////////////////////////////////////
double Line::getPositionOnLine ( GeoPoint p )
{
  int index;

  double h = dir.getMaxComponent( index );
  double c = ( p[index] - point[index] ) / h;

  return c;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die absolute Position von c (0 <= c <= 1)                        //
///////////////////////////////////////////////////////////////////////////////
GeoPoint Line::getPointFromParam ( double c )
{
  Direction h = dir.scale( c );
  return point + h.toPoint();
}

///////////////////////////////////////////////////////////////////////////////
//  Sortiert die übergebenen Punkte entlang der Linie                        //
///////////////////////////////////////////////////////////////////////////////
void Line::sortPointsOnLine ( std::vector<GeoPoint> &vPoints, std::map<double,GeoPoint> &mSortedPoints )
{
  int          index;
  unsigned int i;

  double h = dir.getMaxComponent( index );

  for ( i = 0; i < vPoints.size(); i++ )
  {
    GeoPoint p = vPoints[i];

    double c = ( p[index] - point[index] ) / h;
    mSortedPoints[c] = p;
  }
}




