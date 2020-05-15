#include "stdafx.h"

#include <vector>
#include <map>
#include <set>
#include <sstream>
#include "math.h"

#include "Constant.h"
#include "Convert.h"
#include "Geometrie.h"
#include "Geometrie3D.h"


///////////////////////////////////////////////////////////////////////////////
//  Geometrie3D															                                 //
//  Basisklasse für alle 3D-Geometrien				  				                     //
///////////////////////////////////////////////////////////////////////////////
Geometrie3D::Geometrie3D ()
{
  bbExist = false;
}

Geometrie3D::Geometrie3D ( Geometrie3D * pGeoOld )
{
  if ( pGeoOld->gmlId != "" )
    gmlId = pGeoOld->gmlId + "_C";
  else
    gmlId = "";
  srsName = pGeoOld->srsName;

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

Geometrie3D::Geometrie3D ( const Geometrie3D &geoOld )
{
  if ( geoOld.gmlId != "" )
    gmlId = geoOld.gmlId + "_C";
  else
    gmlId = "";
  srsName = geoOld.srsName;

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
//  GeoPoint3D                                                               //
//  Implementierung der Klasse gml:Point mit 3D-Koordinaten                  //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor							                        					 //
///////////////////////////////////////////////////////////////////////////////
GeoPoint3D::GeoPoint3D() : Geometrie3D()
{
  x = 0;
  y = 0;
  z = 0;
}

GeoPoint3D::GeoPoint3D ( double xP, double yP, double zP ) : Geometrie3D()
{
  x = xP;
  y = yP;
  z = zP;
}

GeoPoint3D::GeoPoint3D ( GeoPoint3D * pPointOld ): Geometrie3D ( pPointOld )
{
  x = pPointOld->x;
  y = pPointOld->y;
  z = pPointOld->z;
}

GeoPoint3D::GeoPoint3D ( const GeoPoint3D & pointOld ): Geometrie3D ( pointOld )
{
  x = pointOld.x;
  y = pointOld.y;
  z = pointOld.z;
}

GeoPoint3D::~ GeoPoint3D()
{
}

///////////////////////////////////////////////////////////////////////////////
//  +  und - Operator 														                           //
///////////////////////////////////////////////////////////////////////////////
GeoPoint3D GeoPoint3D::operator+ ( GeoPoint3D p)
{
  GeoPoint3D result;

  result.x = x + p.x;
  result.y = y + p.y;
  result.z = z + p.z;

  return result;
}

GeoPoint3D GeoPoint3D::operator- ( GeoPoint3D p)
{
  GeoPoint3D result;

  result.x = x - p.x;
  result.y = y - p.y;
  result.z = z - p.z;

  return result;
}

///////////////////////////////////////////////////////////////////////////////
//  = Operator        														                           //
///////////////////////////////////////////////////////////////////////////////
GeoPoint3D GeoPoint3D::operator= ( GeoPoint3D p )
{
  x = p.x;
  y = p.y;
  z = p.z;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//  Abstand zweier Punkte                                                    //
///////////////////////////////////////////////////////////////////////////////
double GeoPoint3D::dist (  GeoPoint3D p )
{
  double dist = sqrt ( (x - p.x)*(x - p.x) + (y - p.y)*(y - p.y) + (z - p.z)*(z - p.z) );
  return dist;
}


///////////////////////////////////////////////////////////////////////////////
//  Setzt die Koordinate												                          	 //
///////////////////////////////////////////////////////////////////////////////
void GeoPoint3D::set ( double xP, double yP, double zP )
{
  x = xP;
  y = yP;
  z = zP;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding-Box												 //
///////////////////////////////////////////////////////////////////////////////
bool GeoPoint3D::getBoundingBox ( double * sw, double * no )
{
  sw[0] = x;
  sw[1] = y;
  sw[2] = z;
  no[0] = x;
  no[1] = y;
  no[2] = z;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  _Surface3D											                                				 //
//  Abstrakte Oberklasse für gml:Surface (aufgebaut aus Polygon-Patches)     //
//  und gml:Polygon mit 3D-Koordinaten                           						 //
///////////////////////////////////////////////////////////////////////////////
_Surface3D::_Surface3D(): Geometrie3D()
{
}

_Surface3D::_Surface3D ( _Surface3D * pSurfaceOld ): Geometrie3D ( pSurfaceOld )
{
}

_Surface3D::~ _Surface3D()
{
}

///////////////////////////////////////////////////////////////////////////////
//  _Curve3D	                                															 //
//  Abstrakte Oberklasse für gml:Curve (aufgebaut aus curve-Segments)	    	 //
//  und gml:LineString mit 3D-Koordinaten              											 //
///////////////////////////////////////////////////////////////////////////////
_Curve3D::_Curve3D(): Geometrie3D()
{
}

_Curve3D::_Curve3D ( _Curve3D * pCurveOld  ): Geometrie3D ( pCurveOld )
{
}

_Curve3D::~ _Curve3D()
{
}

///////////////////////////////////////////////////////////////////////////////
//  _Ring3D						                                    									 //
//  Implementierung der abstrakten Oberklasse für gml:Ring und      				 //
//  gml:LinearRing mit 3D Stützpunkten  		                            		 //
///////////////////////////////////////////////////////////////////////////////
_Ring3D::_Ring3D(): Geometrie3D()
{
}

_Ring3D::_Ring3D ( _Ring3D * pRingOld ): Geometrie3D ( pRingOld )
{
}

_Ring3D:: ~_Ring3D()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den Schwerpunkt des Ringes									                   //
///////////////////////////////////////////////////////////////////////////////
bool _Ring3D::getCenter ( double &cxP, double &cyP, double &czP )
{
  double sw[3], no[3];
  bool   success;

  success = getBoundingBox ( sw, no );
  if ( success ) return false;

  cxP = 0.5*(sw[0] + no[0] );
  cyP = 0.5*(sw[1] + no[1] );
  czP = 0.5*(sw[2] + no[2] );

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  MultiSurface3D					                              									 //
//  Klasse für eine gml:MultiSurface oder gml:CompositeSurface	      			 //
///////////////////////////////////////////////////////////////////////////////
MultiSurface3D::MultiSurface3D(): _Surface3D()
{
  isCompositeSurface = false;
}

MultiSurface3D::MultiSurface3D ( MultiSurface3D * pMultiSurfaceOld ): _Surface3D ( pMultiSurfaceOld )
{
  isCompositeSurface = pMultiSurfaceOld->isCompositeSurface;

  for ( unsigned int i = 0; i < pMultiSurfaceOld->position.size(); i++ )
  {
    _Surface3D * pSurface = pMultiSurfaceOld->position[i];
    switch ( pSurface->getSurfaceType() )
    {
    case SURFACE_3D:
      {
        Surface3D * pSurfaceCopy = new Surface3D ( (Surface3D*)pSurface );
        position.push_back ( pSurfaceCopy );
        break;
      }
    case POLYGON_3D :
      {
        Polygon3D * pSurfaceCopy = new Polygon3D ( (Polygon3D*)pSurface );
        position.push_back ( pSurfaceCopy );
        break;
      }
    }
  }
}

MultiSurface3D::~MultiSurface3D()
{
  for  ( unsigned int i = 0; i < position.size(); i++ )
    delete position[i];
}

GEOMETRY_TYPE MultiSurface3D::getSurfaceType()
{ 
  if ( isCompositeSurface )
    return COMPOSITE_SURFACE_3D;
  else
    return MULTI_SURFACE_3D;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Surface-Member						                      						 //
///////////////////////////////////////////////////////////////////////////////
_Surface3D * MultiSurface3D::getSurface ( unsigned int index )
{
  if ( index < 0 || index >= position.size() ) return NULL;
  else										 return position[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der explizit gekennzeichneten Löcher der Multifläche	 //
///////////////////////////////////////////////////////////////////////////////
unsigned int MultiSurface3D::getLochAnz()
{
  unsigned int lochAnz = 0;

  for ( unsigned int i = 0; i < position.size(); i++ )
    lochAnz = lochAnz + position[i]->getLochAnz();

  return lochAnz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding Box										                      	     //
///////////////////////////////////////////////////////////////////////////////
bool MultiSurface3D::getBoundingBox ( double * sw, double * no )
{
  unsigned int i;
  bool	success;
  double  swLoc[2], noLoc[2];

  if ( bbExist )
  {
    for ( i = 0; i < 3; i++ )
    {
      sw[i] = m_sw[i];
      no[i] = m_no[i];
    }
    return true;
  }

  for ( i = 0; i < 3; i++ )
  {
    sw[i] =  1.0E20;
    no[i] = -1.0E20;
  }

  if ( position.size() == 0 ) return false;

  for ( i = 0; i < position.size(); i++ )
  { 
    _Surface3D * pSurface = position[i];
    success = pSurface->getBoundingBox ( swLoc, noLoc );
    if ( !success ) return false;

    if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
    if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
    if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
    if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
    if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
  }

  for ( i = 0; i < 3; i++ )
  {
    m_sw[i] = sw[i];
    m_no[i] = no[i];
  }
  bbExist = true;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet den Schwerpunkt aller Member der Multi-Surface	        			 //
///////////////////////////////////////////////////////////////////////////////
bool MultiSurface3D::getCenter ( double &xP, double &yP, double &zP )
{
  double       xLoc, yLoc, zLoc;
  unsigned int i;
  bool         success;

  if ( position.size() == 0 ) return false;

  xP = 0.0;
  yP = 0.0;
  zP = 0.0;

  for ( i = 0; i < position.size(); i++ )
  {
    _Surface3D * pSurface = position[i];
    success = pSurface->getCenter ( xLoc, yLoc, zLoc );
    if ( !success ) return false;

    xP = xP + xLoc;
    yP = yP + yLoc;
    zP = zP + zLoc;
  }

  xP = xP / position.size();
  yP = yP / position.size();
  zP = zP / position.size();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle zur Multi-Surface gehörigen Polygone			            			 //
///////////////////////////////////////////////////////////////////////////////
unsigned int MultiSurface3D::getPolygone ( std::vector<Polygon3D*> &vPolygone )
{
  unsigned int       i, j, patchAnz;
  vector<Polygon3D*> patches;

  for ( i = 0; i < position.size(); i++ )
  {
    _Surface3D * pSurface = position[i];

    switch ( pSurface->getSurfaceType() )
    {
    case SURFACE_3D:
      patches.clear();
      patchAnz = ((Surface3D*)pSurface)->getPatches ( patches );
        for ( j = 0; j < patchAnz; j++ )
          vPolygone.push_back ( patches[j] );
      break;

    case POLYGON:
      vPolygone.push_back ( (Polygon3D*)position[i] );
      break;
    }
  }

  return vPolygone.size();
}

///////////////////////////////////////////////////////////////////////////////
//  MultiCurve3D									                              						 //
//  Klasse für eine Multi-Curve										                      		 //
///////////////////////////////////////////////////////////////////////////////
MultiCurve3D::MultiCurve3D() : Geometrie3D()
{
  isCompositeCurve = true;
}

MultiCurve3D::MultiCurve3D ( MultiCurve3D * pMultiCurveOld ): Geometrie3D ( pMultiCurveOld )
{
  isCompositeCurve = pMultiCurveOld->isCompositeCurve;

  for ( unsigned int i = 0; i < pMultiCurveOld->position.size(); i++ )
  {
    _Curve3D * pCurve = pMultiCurveOld->position[i];
    switch ( pCurve->getCurveType() )
    {
    case CURVE_3D:
      {
        Curve3D * pCurveCopy = new Curve3D ( (Curve3D*)pCurve );
        position.push_back ( pCurveCopy );
        break;
      }

    case LINE_STRING_3D:
      {
        LineSegment3D * pLineSegment = new LineSegment3D ( (LineSegment3D*)pCurve );
        position.push_back ( pLineSegment );
        break;
      }
    }
  }
}

MultiCurve3D::~MultiCurve3D()
{
  for ( unsigned int i = 0; i < position.size(); i++ )
    delete position[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Verbindet zusammenhängende Kurvensegmente				                				 //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve3D::joinSegments( vector<CurveSegment3D*> & vCurveSegments, double minDist )
{
  unsigned int     i, j;
  CurveSegment3D * pSegI, *pSegJ;
  GeoPoint3D		   pAnfI, pEndI, pAnfJ, pEndJ;
  bool             weiter = true;

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

void MultiCurve3D::deleteSegment ( vector<CurveSegment3D*> & vCurveSegments, unsigned int indexP )
{
  unsigned int i;

  if ( indexP < 0 || indexP >= vCurveSegments.size() ) return;

  CurveSegment3D * pSeg = vCurveSegments[indexP];
  for ( i = indexP + 1; i < vCurveSegments.size(); i++ ) vCurveSegments[i-1] = vCurveSegments[i]; 
  vCurveSegments.pop_back();
  delete pSeg;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Curve-Member hinzu										                      		 //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve3D::addCurve (  _Curve3D * pCurve )
{
  position.push_back ( pCurve );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Curve-Member mit einem best. Index					            		 //
///////////////////////////////////////////////////////////////////////////////
_Curve3D * MultiCurve3D::getCurve ( unsigned int index )
{
  if ( index < 0 || index >= position.size() ) return NULL;
  else										 return position[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Curve-Members										                        	 //
///////////////////////////////////////////////////////////////////////////////
unsigned int MultiCurve3D::getCurves ( std::vector<_Curve3D*> &vCurves )
{
  unsigned int i;

  for ( i = 0; i < position.size(); i++ )
    vCurves.push_back ( position[i] );

  return vCurves.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding Box										                        		 //
///////////////////////////////////////////////////////////////////////////////
bool MultiCurve3D::getBoundingBox ( double * sw, double * no )
{
  bool	       success;
  double       swLoc[3], noLoc[3];
  unsigned int i;

  if ( bbExist )
  {
    for ( i = 0; i < 3; i++ )
    {
      sw[i] = m_sw[i];
      no[i] = m_no[i];
    }
    return true;
  }

  for ( i = 0; i < 3; i++ )
  {
    sw[i] =  1.0E20;
    no[i] = -1.0E20;
  }

  if ( position.size() == 0 ) return false;

  for ( i = 0; i < position.size(); i++ )
  { 
    _Curve3D * pCurve = position[i];
    success = pCurve->getBoundingBox ( swLoc, noLoc );
    if ( !success ) return false;

    if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
    if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
    if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
    if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
    if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
  }

  for ( i = 0; i < 3; i++ )
  {
    m_sw[i] = sw[i];
    m_no[i] = no[i];
  }
  bbExist = true;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt								                        					 //
///////////////////////////////////////////////////////////////////////////////
bool MultiCurve3D::getCenter ( double &xP, double &yP, double &zP )
{
  double       xLoc, yLoc, zLoc;
  unsigned int i;
  bool         success;

  if ( position.size() == 0 ) return false;

  xP = 0.0;
  yP = 0.0;
  zP = 0.0;

  for ( i = 0; i < position.size(); i++ )
  {
    _Curve3D * pCurve = position[i];
    success = pCurve->getCenter ( xLoc, yLoc, zLoc );
    if ( !success ) return false;

    xP = xP + xLoc;
    yP = yP + yLoc;
    zP = zP + zLoc;
  }

  xP = xP / position.size();
  yP = yP / position.size();
  zP = zP / position.size();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Dreht die Laufrichtung der Multi-Curve um					                			 //
///////////////////////////////////////////////////////////////////////////////
void MultiCurve3D::reverse()
{
  int        i, ii;
  _Curve3D * pCurveA;
  _Curve3D * pCurveB;
  int        anzHalbe = div ( position.size(), 2 ).quot;

  for ( i = 0; i < anzHalbe; i++ )
  {
    ii = position.size() - 1 - i;

    pCurveA = position[i];
    pCurveA->reverse();
    pCurveB = position[ii];
    pCurveB->reverse();

    position[i]  = pCurveB;
    position[ii] = pCurveA;
  }

  if (  div ( position.size(), 2 ).rem == 1 )
    position[anzHalbe]->reverse();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Länge der Multi-Curve								                        //
///////////////////////////////////////////////////////////////////////////////
double MultiCurve3D::getLength()
{
  unsigned int i;
  double       laenge = 0.0;

  for ( i = 0; i < position.size(); i++ )
  {
    _Curve3D * pCurve = position[i];
    laenge = laenge + pCurve->getLength();
  }

  return laenge;
}

///////////////////////////////////////////////////////////////////////////////
//  MultiPoint3D															                               //
//  Klasse für Multi-Points													                         //
///////////////////////////////////////////////////////////////////////////////
MultiPoint3D::MultiPoint3D(): Geometrie3D()
{

}

MultiPoint3D::MultiPoint3D ( MultiPoint3D * pMultiPointOld ): Geometrie3D ( pMultiPointOld )
{
  for ( unsigned int i = 0; i < pMultiPointOld->position.size(); i++ )
  {
    GeoPoint3D * pPoint = pMultiPointOld->position[i];
    GeoPoint3D * pPointNew = new GeoPoint3D ( pPoint ); 
    position.push_back ( pPointNew );
  }
}

MultiPoint3D::~ MultiPoint3D()
{
  unsigned int i;

  for ( i = 0; i < position.size(); i++  )
    delete position[i];  
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Punkt hinzu													                         //
///////////////////////////////////////////////////////////////////////////////
void MultiPoint3D::addPoint (  GeoPoint3D * pPoint )
{
  position.push_back ( pPoint );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Punkt-Member zu index								                     		 //
///////////////////////////////////////////////////////////////////////////////
GeoPoint3D*  MultiPoint3D::getPoint ( unsigned int index )
{
  if ( index < 0 || index >= position.size() ) return NULL;
  else										                     return position[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Koordinaten aller Punkte	                  								 //
///////////////////////////////////////////////////////////////////////////////
unsigned int MultiPoint3D::getPoints ( std::vector<double> &xCoordP, std::vector<double> &yCoordP, 
                                       std::vector<double> &zCoordP )
{
  unsigned int i;
  GeoPoint3D * pPoint;

  for ( i = 0; i < position.size(); i++ )
  {
    pPoint = position[i];
    xCoordP.push_back ( pPoint->getX() );
    yCoordP.push_back ( pPoint->getY() );
    zCoordP.push_back ( pPoint->getZ() );
  }

  return xCoordP.size();
}

//////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding-Box			                        									//
//////////////////////////////////////////////////////////////////////////////
bool MultiPoint3D::getBoundingBox ( double * sw, double * no )
{
  bool	       success;
  double       swLoc[3], noLoc[3];
  unsigned int i;

  if ( bbExist )
  {
    for ( i = 0; 0 < 3; i++ )
    {
      sw[i] = m_sw[i];
      no[i] = m_no[i];
    }
    return true;
  }

  for ( i = 0; i < 3; i++ )
  {
    sw[i] =  1.0E20;
    no[i] = -1.0E20;
  }

  if ( position.size() == 0 ) return false;

  for ( i = 0; i < position.size(); i++ )
  { 
    GeoPoint3D pPoint = position[i];
    success = pPoint.getBoundingBox ( swLoc, noLoc );
    if ( !success ) return false;

    if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
    if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
    if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
    if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
    if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
  }

  for ( i = 0; i < 3; i++ )
  {
    m_sw[i] = sw[i];
    m_no[i] = no[i];
  }
  bbExist = true;

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt									                                //
//////////////////////////////////////////////////////////////////////////////
bool MultiPoint3D::getCenter ( double &xP, double &yP, double &zP )
{
  unsigned int i;

  if ( position.size() == 0 ) return false;

  xP = 0.0;
  yP = 0.0;
  zP = 0.0;

  for ( i = 0; i < position.size(); i++ )
  {
    GeoPoint3D pPoint = position[i];
    xP = xP + pPoint.getX();
    yP = yP + pPoint.getY();
    zP = zP + pPoint.getZ();
  }

  xP = xP / position.size();
  yP = yP / position.size();
  zP = zP / position.size();

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Surface3D														                                			 //
//  Implementierung der Klasse gml:Surface								                	 //
///////////////////////////////////////////////////////////////////////////////
Surface3D::Surface3D(): _Surface3D()
{
}

Surface3D::Surface3D ( Surface3D * pSurfaceOld ): _Surface3D ( pSurfaceOld )
{
  for ( unsigned int i = 0; i < patches.size(); i++ )
  {
    Polygon3D * pPoly = pSurfaceOld->patches[i];
    Polygon3D * pPolyCopy = new Polygon3D ( pPoly );
    patches.push_back ( pPolyCopy );
  }
}

Surface3D:: ~Surface3D()
{
  for ( unsigned int i = 0; i < patches.size(); i++ )
    delete patches[i];

}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding-Box												                         //
///////////////////////////////////////////////////////////////////////////////
bool Surface3D::getBoundingBox ( double * sw, double * no )
{
  bool	       success;
  double       swLoc[3], noLoc[3];
  unsigned int i;

  if ( bbExist )
  {
    for ( i = 0; i < 3; i++ )
    {
      sw[i] = m_sw[i];
      no[i] = m_no[i];
    }
    return true;
  }

  for ( i = 0; i < 3; i++ )
  {
    sw[i] =  1.0E20;
    no[i] = -1.0E20;
  }

  if ( patches.size() == 0 ) return false;

  for ( unsigned int i = 0; i < patches.size(); i++ )
  { 
    Polygon3D * pSurface = patches[i];
    success = pSurface->getBoundingBox ( swLoc, noLoc );
    if ( !success ) return false;

    if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
    if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
    if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
    if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
    if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
  }

  for ( i = 0; i < 3; i++ )
  {
    m_sw[i] = sw[i];
    m_no[i] = no[i];
  }
  bbExist = true;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den gemeinsamen Schwerpunkt der Patches			            				 //
///////////////////////////////////////////////////////////////////////////////
bool Surface3D::getCenter ( double &xP, double &yP, double &zP )
{
  double        xLoc, yLoc, zLoc;
  unsigned int  i;
  bool          success;

  if ( patches.size() == 0 ) return false;

  xP = 0.0;
  yP = 0.0;
  zP = 0.0;

  for ( i = 0; i < patches.size(); i++ )
  {
    Polygon3D * pSurface = patches[i];
    success = pSurface->getCenter ( xLoc, yLoc, zLoc );
    if ( !success ) return false;

    xP = xP + xLoc;
    yP = yP + yLoc;
    zP = zP + zLoc;
  }

  xP = xP / patches.size();
  yP = yP / patches.size();
  zP = zP / patches.size();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Gesamt-Anzahl aller Löcher der Patches			          			 // 
///////////////////////////////////////////////////////////////////////////////
unsigned int Surface3D::getLochAnz()
{
  unsigned int i, lochAnz;

  lochAnz = 0;

  for ( i = 0; i < patches.size(); i++ )
    lochAnz = lochAnz + patches[i]->getLochAnz();

  return lochAnz;
}

///////////////////////////////////////////////////////////////////////////////
// Liefert die Surface-Patches											                      	 //
///////////////////////////////////////////////////////////////////////////////
int Surface3D::getPatches ( std::vector<Polygon3D*> &vPatches )
{
  for ( unsigned int i = 0; i < patches.size(); i++ )
    vPatches.push_back ( patches[i] );

  return vPatches.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Polygon3D                                                                //
//  Implementierung der Klasse gml:Polygon								                	 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor												                         //
///////////////////////////////////////////////////////////////////////////////
Polygon3D::Polygon3D()
{
  aussenkontur = NULL;
}

Polygon3D::Polygon3D( Polygon3D * pPolyOld )
{
  unsigned int i;
  _Ring3D      * pLoch;
  _Ring3D      * pLochNew;

  if ( pPolyOld->aussenkontur != NULL )
  {
    switch ( pPolyOld->aussenkontur->getRingType() )
    {
    case LINEAR_RING_3D:
      aussenkontur = new LinearRing3D ( (LinearRing3D*)pPolyOld->aussenkontur );
      break;
    case RING_3D:
      aussenkontur = new Ring3D ( (Ring3D*)pPolyOld->aussenkontur );
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
    case LINEAR_RING_3D:
      pLochNew = new LinearRing3D ( (LinearRing3D*)pLoch );
      break;

    case RING_3D:
      pLochNew = new Ring3D ( (Ring3D*)pLoch );
      break;
    }
    loecher.push_back ( pLochNew );
  }
}

Polygon3D::~Polygon3D()
{
  unsigned int i;

  if ( aussenkontur != NULL ) delete aussenkontur;
  for ( i = 0; i < loecher.size(); i++ ) delete loecher[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt die Aussenkontur									                        				 //
///////////////////////////////////////////////////////////////////////////////
void Polygon3D::setAussenkontur ( _Ring3D * aussenkonturP )
{
  aussenkontur = aussenkonturP;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Loch hinzu										                            			 //
///////////////////////////////////////////////////////////////////////////////
void Polygon3D::addLoch ( _Ring3D * lochP )
{
  loecher.push_back ( lochP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die BoundingBox									                        				 //
///////////////////////////////////////////////////////////////////////////////
bool Polygon3D::getBoundingBox ( double * sw, double * no )
{
  unsigned int i;

  if ( aussenkontur != NULL )
    return aussenkontur->getBoundingBox ( sw, no );

  for ( i = 0; i < 3; i++ )
  {
    sw[i] =  1.0E10;
    no[i] = -1.0E10;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt der Aussenkontur				                				 //
///////////////////////////////////////////////////////////////////////////////
bool Polygon3D::getCenter ( double &cxP, double &cyP, double &czP )
{
  if ( aussenkontur != NULL )	return aussenkontur->getCenter ( cxP, cyP, czP );
  else					              return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Ring3D                                                                   //
//  Implementierung der Klasse gml:Ring								                	     //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor												                         //
///////////////////////////////////////////////////////////////////////////////
Ring3D::Ring3D() : _Ring3D()
{
  pGeometrie = NULL;
}

Ring3D::Ring3D( Ring3D * pRingOld ): _Ring3D ( pRingOld )
{
  if ( pRingOld->pGeometrie != NULL )
    pGeometrie = new MultiCurve3D ( pRingOld->pGeometrie );
  else
    pGeometrie = NULL;
}

Ring3D::~Ring3D()
{
  if ( pGeometrie != NULL )delete pGeometrie;
}

///////////////////////////////////////////////////////////////////////////////
//  Kopiert den Ring                                                         //
///////////////////////////////////////////////////////////////////////////////
_Ring3D * Ring3D::copyRing ()
{
  Ring3D * pCopyRing = new Ring3D ( this );
  return pCopyRing; 
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines curveMembers des Rings									                 //
///////////////////////////////////////////////////////////////////////////////
void Ring3D::addRingMember ( _Curve3D * pCurve )
{
  if ( pGeometrie == NULL ) pGeometrie = new MultiCurve3D();

  pGeometrie->addCurve ( pCurve );
}

void Ring3D::setRingGeometrie ( MultiCurve3D * pMultiCurve )
{
  if ( pGeometrie != NULL ) delete pGeometrie;
  pGeometrie = pMultiCurve;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert curveMembers mit index											                     //
///////////////////////////////////////////////////////////////////////////////
_Curve3D * Ring3D::getRingMember( int indexP )
{
  if ( pGeometrie != NULL )
    return pGeometrie->getCurve( indexP );  
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Segmente											                     //
///////////////////////////////////////////////////////////////////////////////
unsigned int Ring3D::getRingMemberAnz()
{  
  if ( pGeometrie != NULL )
    return pGeometrie->getCurveMemberAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Punkte der Kontur, wobei der erste und letzte Punkt		     //
//  identisch sind															                             //
///////////////////////////////////////////////////////////////////////////////
unsigned int Ring3D::getPoints ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP )
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
    _Curve3D * pSeg = getRingMember( i );
    pSeg->getPoints ( vXloc, vYloc, vZloc );
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
//  Liefert die Anzahl der Stützpunkte							                  			 //
///////////////////////////////////////////////////////////////////////////////
unsigned int Ring3D::getPointAnz()
{
  unsigned int i, anz, anzLoc;

  anz = 0;

  for ( i = 0; i < getRingMemberAnz(); i++ )
  {
    anzLoc = getRingMember( i )->getPointAnz();
    if ( i == 0 ) anz = anz + anzLoc;
    else		  anz = anz + anzLoc - 1;
  }

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Kehrt die Laufrichtung um									                        			 //
///////////////////////////////////////////////////////////////////////////////
void Ring3D::reverse()
{
  if ( pGeometrie != NULL ) pGeometrie->reverse();
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Bounding-Box									                      			 //
///////////////////////////////////////////////////////////////////////////////
bool Ring3D::getBoundingBox ( double * sw, double * no )
{
  if ( pGeometrie != NULL )
    return pGeometrie->getBoundingBox ( sw, no );

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Länge des Umfangs							                    				 //
///////////////////////////////////////////////////////////////////////////////
double Ring3D::getUmfang()
{
  if ( pGeometrie != NULL ) return pGeometrie->getLength();
  else					            return 0.0;
}
///////////////////////////////////////////////////////////////////////////////
//  Entfernt aufeinander folgende, doppelte Stützpunkte                      //
///////////////////////////////////////////////////////////////////////////////
void Ring3D::removeDoublePoints ( double minDist )
{
  unsigned          i, anz;
  vector<_Curve3D*> vCurves;

  anz = pGeometrie->getCurves( vCurves );
  for ( i = 0; i < anz; i++ )
    vCurves[i]->removeDoublePoints ( minDist );
}

///////////////////////////////////////////////////////////////////////////////
//  LinearRing3D													                              			 //
//  Implementierung der Klasse gml:LinearRing								                 //
///////////////////////////////////////////////////////////////////////////////
LinearRing3D::LinearRing3D(): _Ring3D()
{
  points = NULL;
}

LinearRing3D::LinearRing3D( LinearRing3D * pRingOld ) : _Ring3D ( pRingOld )
{
  if ( pRingOld->points != NULL )
    points = new LineSegment3D ( pRingOld->points );
  else
    points = NULL;
}

LinearRing3D::~LinearRing3D()
{
  if ( points != NULL ) delete points;
}

///////////////////////////////////////////////////////////////////////////////
//  Kopiert den Ring                                                         //
///////////////////////////////////////////////////////////////////////////////
_Ring3D * LinearRing3D::copyRing ()
{
  LinearRing3D * pCopyRing = new LinearRing3D ( this );
  return pCopyRing; 
}

///////////////////////////////////////////////////////////////////////////////
//  Definiert die Stützpunkte des LinearRing			                					 //
///////////////////////////////////////////////////////////////////////////////
void LinearRing3D::addPoints ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, 
                               std::vector<double> &zCoordsP )
{
  if ( points != NULL ) delete points;

  points = new LineSegment3D();
  points->addPoints ( xCoordsP, yCoordsP, zCoordsP );
}

void LinearRing3D::addPoints ( vector<GeoPoint3D> &vPoints )
{
  if ( points != NULL ) delete points;

  points = new LineSegment3D();
  points->addPoints ( vPoints );
}

void LinearRing3D::setLineSegment ( LineSegment3D * pLineSegment )
{
  if ( points != NULL ) delete points;
  points = pLineSegment;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Stützpunkte des LinearRing					                  			 //
///////////////////////////////////////////////////////////////////////////////
unsigned int LinearRing3D::getPoints ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, 
                                       std::vector<double> &zCoordsP  )
{
  if ( points != NULL ) return points->getPoints ( xCoordsP, yCoordsP, zCoordsP );
  else		  		        return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Stützpunkte des LinearRing					            	 //
///////////////////////////////////////////////////////////////////////////////
unsigned int LinearRing3D::getPointAnz()
{
  if ( points != NULL ) return points->getPointAnz();
  else				          return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding-Box							                        					 //
///////////////////////////////////////////////////////////////////////////////
bool LinearRing3D::getBoundingBox ( double * sw, double * no )
{
  if ( points != NULL )
    return points->getBoundingBox( sw, no );
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Dreht die Laufrichtung des LinearRing um					                			 //
///////////////////////////////////////////////////////////////////////////////
void LinearRing3D::reverse()
{
  if ( points != NULL ) points->reverse();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Umfang des LinearRing										                     //
///////////////////////////////////////////////////////////////////////////////
double LinearRing3D::getUmfang()
{
  if ( points != NULL ) return points->getLength();
  else				  return 0.0;
}

void LinearRing3D::removeDoublePoints ( double minDist )
{
  if ( points != NULL )
    points->removeDoublePoints ( minDist );
}

///////////////////////////////////////////////////////////////////////////////
//  Curve3D                                                                  //
//  Implementierung der Klasse gml:Curve									                   //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor												                         //
///////////////////////////////////////////////////////////////////////////////
Curve3D::Curve3D(): _Curve3D()
{

}

Curve3D::Curve3D ( Curve3D * pCurveOld ): _Curve3D ( pCurveOld )
{
  unsigned int i;

  for ( i = 0; i < segments.size(); i++ )
  {
    CurveSegment3D * pCurveSegment    = pCurveOld->segments[i];
    if ( pCurveSegment->getCurveType() == LINE_STRING_3D )
    {
      LineSegment3D * pCurveSegmentNew = new LineSegment3D ( (LineSegment3D*)pCurveSegment );
      segments.push_back ( pCurveSegmentNew );
    }
  }
}

Curve3D::~Curve3D()
{
  for ( unsigned int i = 0; i < segments.size(); i++ ) delete segments[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Dreht die Laufrichtung der Linie um										                   //
///////////////////////////////////////////////////////////////////////////////
void Curve3D::reverse()
{
  int              i, ii;
  CurveSegment3D * pCurveSegmentA;
  CurveSegment3D * pCurveSegmentB;
  int              anzHalbe = div ( segments.size(), 2 ).quot;

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

  if (  div ( segments.size(), 2 ).rem == 1 )
    segments[anzHalbe]->reverse();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Stützpunkte												                         //
///////////////////////////////////////////////////////////////////////////////
unsigned int Curve3D::getPoints ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP )
{
  vector<double> vXloc;
  vector<double> vYloc;
  vector<double> vZloc;
  double		     x, y, z;
  unsigned int   i, j, indexAnf;

  for ( i = 0; i < getCurveSegmentAnz(); i++ )
  {
    vXloc.clear();
    vYloc.clear();
    CurveSegment3D * pSeg = getCurveSegment( i );
    pSeg->getPoints ( vXloc, vYloc, vZloc );
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
//  Liefert die Anzahl alle Stützpunkte		                  								 //
///////////////////////////////////////////////////////////////////////////////
unsigned int Curve3D::getPointAnz()
{
  unsigned int i, anz;

  anz = 0;
  for ( i = 0; i < getCurveSegmentAnz(); i++ )
  {
    CurveSegment3D * pSeg = getCurveSegment( i );
    if ( i == 0 ) anz = anz + pSeg->getPointAnz();
    else		  anz = anz + pSeg->getPointAnz()-1;
  }

  return anz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Anfangs- und Endpunkt der Curve                                  //
///////////////////////////////////////////////////////////////////////////////
GeoPoint3D  Curve3D::getAnfangsPunkt ()
{
  if ( segments.size() > 0 ) return segments[0]->getAnfangsPunkt();
  else					             return GeoPoint3D(0,0,0);
}

GeoPoint3D  Curve3D::getEndPunkt()
{
  if ( segments.size() > 0 ) return segments[0]->getEndPunkt();
  else					             return GeoPoint3D(0,0,0);
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Bounding-Box								                          		 //
///////////////////////////////////////////////////////////////////////////////
bool Curve3D::getBoundingBox ( double * sw, double * no )
{
  CurveSegment3D * pSeg;
  unsigned int     i;
  bool		         success;
  double           swLoc[3], noLoc[3];

  if ( bbExist )
  {
    for ( i = 0; i < 3; i++ )
    {
      sw[i] = m_sw[i];
      no[i] = m_no[i];
    }
    return true;
  }

  for ( i = 0; i < 3; i++ )
  {
    sw[i] =  1.0E20;
    no[i] = -1.0E20;
  }

  if ( segments.size() == 0 ) return false;

  for ( i = 0; i < segments.size(); i++ )
  { 
    pSeg = segments[i];
    success = pSeg->getBoundingBox ( swLoc, noLoc );
    if ( !success ) return false;

    if ( swLoc[0] < sw[0] ) sw[0] = swLoc[0];
    if ( swLoc[1] < sw[1] ) sw[1] = swLoc[1];
    if ( swLoc[2] < sw[2] ) sw[2] = swLoc[2];
    if ( noLoc[0] > no[0] ) no[0] = noLoc[0];
    if ( noLoc[1] > no[1] ) no[1] = noLoc[1];
    if ( noLoc[2] > no[2] ) no[2] = noLoc[2];
  }

  for ( i = 0; i < 3; i++ )
  {
    m_sw[i] = sw[i];
    m_no[i] = no[i];
  }
  bbExist = true;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Länge der Kurve									                      		 //
///////////////////////////////////////////////////////////////////////////////
double Curve3D::getLength()
{
  CurveSegment3D * pSeg;
  unsigned int     i;
  double		       length = 0.0;

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
bool Curve3D::getCenter ( double &xP, double &yP, double &zP )
{
  double sw[3], no[3];
  bool   success;

  success = getBoundingBox ( sw, no );
  if ( success ) return false;

  xP = 0.5*(sw[0] + no[0] );
  yP = 0.5*(sw[1] + no[1] );
  zP = 0.5*(sw[2] + no[2] );

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt aufeinander folgende, doppelte Stützpunkte                      //
///////////////////////////////////////////////////////////////////////////////
void Curve3D::removeDoublePoints ( double minDist )
{
  unsigned int i;

  for ( i = 0; i < segments.size(); i++ )
    segments[i]->removeDoublePoints( minDist );
}

///////////////////////////////////////////////////////////////////////////////
//  CurveSegment3D                                                           //
//  Implementierung der Klasse gml:CurveSegment							              	 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor												                         //
///////////////////////////////////////////////////////////////////////////////
CurveSegment3D::CurveSegment3D(): _Curve3D ()
{
}

CurveSegment3D::CurveSegment3D( CurveSegment3D * pCurveSegmentOld ): _Curve3D ( pCurveSegmentOld )
{
  unsigned int i;

  for ( i = 0; i < pCurveSegmentOld->points.size(); i++ )
  {
    GeoPoint3D point = pCurveSegmentOld->points[i];
    points.push_back ( point );
  }
}

CurveSegment3D::~CurveSegment3D()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen von Punkten													                         //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment3D::addPoints ( std::vector<double> xCoordsP, std::vector<double> yCoordsP, 
                                 std::vector<double> zCoordsP )
{
  for ( unsigned int i = 0; i < xCoordsP.size(); i++ )
  {
    GeoPoint3D * p = new GeoPoint3D ( xCoordsP[i], yCoordsP[i], zCoordsP[i] );
    points.push_back ( p );
  }
}

void CurveSegment3D::addPoints ( std::vector<GeoPoint3D> &vPoints )
{
  for ( unsigned int i = 0; i < vPoints.size(); i++ )
  {
    points.push_back ( vPoints[i] );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt hintereinander auftretende, doppelte Stützpunkte                //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment3D::removeDoublePoints ( double minDist )
{
  unsigned int   i, j, anz;
  GeoPoint3D     p1, p2;
  double         dist;

  anz = points.size();
  for ( i = 0; i < anz-1; i++ )
  {
    p1 = points[i];
    p2 = points[i+1];
    dist = p1.dist ( p2 );

    if ( dist < minDist )
    {
      for ( j = i + 1; j < anz; j++  )
      {
        points[j-1] = points[j];
      }
      points.pop_back();
      anz = anz-1;
      i = i - 1;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern von Punkten														                           //
///////////////////////////////////////////////////////////////////////////////
unsigned int CurveSegment3D::getPoints ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, 
                                         std::vector<double> &zCoordsP )
{
  xCoordsP.clear();
  yCoordsP.clear();
  zCoordsP.clear();

  for ( unsigned int i = 0; i < points.size(); i++ )
  {
    GeoPoint3D p = points[i];
    xCoordsP.push_back ( p.getX() );
    yCoordsP.push_back ( p.getY() );
    zCoordsP.push_back ( p.getZ() );
  }
  return points.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefern der Anzahl der Punkte		                      									 //
///////////////////////////////////////////////////////////////////////////////
unsigned int CurveSegment3D::getPointAnz()
{
  return points.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Anfangs- bzw. Endpunkt des Segments		            					 //
///////////////////////////////////////////////////////////////////////////////
GeoPoint3D CurveSegment3D::getAnfangsPunkt ()
{
  if ( points.size() > 0 ) return points[0];
  else					           return GeoPoint3D(0,0,0);
}

GeoPoint3D  CurveSegment3D::getEndPunkt()
{
  if ( points.size() > 0 ) return points[points.size()-1];
  else					           return GeoPoint3D (0,0,0);
}

///////////////////////////////////////////////////////////////////////////////
//  Kehrt die Laufrichtung des Segments um		                							 //
///////////////////////////////////////////////////////////////////////////////
void CurveSegment3D::reverse()
{
  int         i, ii;
  GeoPoint3D  p;
  int         anzHalbe = div ( points.size(), 2 ).quot;

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
void CurveSegment3D::join ( CurveSegment3D * pSegP )
{
  unsigned int i;
  for ( i = 1; i < pSegP->points.size(); i++ )
  {
    GeoPoint3D p =  pSegP->points[i];
    points.push_back ( p );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die BoundingBox								                        					 //
///////////////////////////////////////////////////////////////////////////////
bool CurveSegment3D::getBoundingBox ( double * sw, double * no )
{
  vector<double> vX, vY, vZ;
  unsigned int   i, anz;

  if ( bbExist )
  {
    for ( i = 0; i < 3; i++ )
    {
      sw[i] = m_sw[i];
      no[i] = m_no[i];
    }
    return true;
  }

  for ( i = 0; i < 3; i++ )
  {
    sw[i] =  1.0E20;
    no[i] = -1.0E20;
  }

  anz = getPoints ( vX, vY, vZ );
  if ( anz == 0 ) return false;

  for ( i = 0; i < anz; i++ )
  {
    if ( vX[i]< sw[0] ) sw[0] = vX[i];
    if ( vY[i]< sw[1] ) sw[1] = vY[i];
    if ( vZ[i]< sw[2] ) sw[2] = vZ[i];
    if ( vX[i]> no[0] ) no[0] = vX[i];
    if ( vY[i]> no[1] ) no[1] = vY[i];
    if ( vZ[i]> no[2] ) no[2] = vZ[i];
  }

  for ( i = 0; i < 3; i++ )
  {
    m_sw[i] = sw[i];
    m_no[i] = no[i];
  }
  bbExist = true;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Schwerpunkt der Kurve					                    					 //
///////////////////////////////////////////////////////////////////////////////
bool CurveSegment3D::getCenter ( double &xP, double &yP, double &zP )
{
  double sw[3], no[3];
  bool   success;

  success = getBoundingBox ( sw, no );
  if ( success ) return false;

  xP = 0.5*(sw[0] + no[0] );
  yP = 0.5*(sw[1] + no[1] );
  zP = 0.5*(sw[2] + no[2] );

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  LineSegment3D                                                            //
//  Implementierung der Klasse gml:LineSegment						               		 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor											                        	 //
///////////////////////////////////////////////////////////////////////////////
LineSegment3D::LineSegment3D() : CurveSegment3D()
{
}

LineSegment3D::LineSegment3D( LineSegment3D * pLineSegmentOld ):CurveSegment3D ( pLineSegmentOld )
{
}

LineSegment3D::~LineSegment3D()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Berechnet die Länge des Segmentes					                    					 //
///////////////////////////////////////////////////////////////////////////////
double LineSegment3D::getLength()
{
  unsigned int   i;
  double         length = 0.0;
  GeoPoint3D     pA;
  GeoPoint3D     pB;

  if ( points.size() <= 1 ) return 0.0;

  pA = points[0];
  for ( i = 1; i < points.size(); i++ )
  {
    pB = points[i];
    length = length + sqrt ( (pB.getX() - pA.getX())*(pB.getX() - pA.getX()) +
                             (pB.getY() - pA.getY())*(pB.getY() - pA.getY()) +
                             (pB.getZ() - pA.getZ())*(pB.getZ() - pA.getZ()) );
    pA = pB;
  }
  return length;
}

