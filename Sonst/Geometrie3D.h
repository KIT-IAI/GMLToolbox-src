#pragma once

#include "Includes.h"
#using <mscorlib.dll>

class _Surface3D;
class _Ring3D;
class _Curve3D;
class Ring3D;
class Curve3D;
class CurveSegment3D;
class LineSegment3D;
class GeoPoint3D;
class Polygon3D;

///////////////////////////////////////////////////////////////////////////////
//  Geometrie3D															                                 //
//  Basisklasse für alle 3D-Geometrien				  				                     //
///////////////////////////////////////////////////////////////////////////////
class Geometrie3D
{
public:
  Geometrie3D ();
  Geometrie3D ( Geometrie3D * pGeoOld );
  Geometrie3D ( const Geometrie3D &geoOld );

protected:
  std::string gmlId;
  std::string srsName;
  double      m_sw[3];
  double      m_no[3];
  bool        bbExist;

public:
  virtual GEOMETRY_TYPE getGeometryType() = NULL;
  virtual bool getBoundingBox ( double * sw, double * no ) = 0;

  void setGmlId ( std::string id )  {  gmlId = id;  }
  std::string getGmlId ()  {  return gmlId;  }
  System::String ^ getGML_ID();

  void setSrsName ( std::string name )  {  srsName = name;  }
  std::string  getSrsName ()  {  return srsName;  }
};

///////////////////////////////////////////////////////////////////////////////
//  GeoPoint3D                                                               //
//  Implementierung der Klasse gml:Point mit 3D-Koordinaten                  //
///////////////////////////////////////////////////////////////////////////////
class GeoPoint3D: public Geometrie3D
{
public:
  GeoPoint3D ( );
  GeoPoint3D ( double xP, double yP, double zP );
  GeoPoint3D ( GeoPoint3D * pPointOld );
  GeoPoint3D ( const GeoPoint3D & pointOld );
  virtual ~ GeoPoint3D();

public:
  double x;
  double y;
  double z;

public:
  virtual GEOMETRY_TYPE getGeometryType() { return GEO_POINT_3D;  }

  void set ( double xP, double yP, double zP );

  double getX()  {  return x;  }
  double getY()  {  return y;  }
  double getZ()  {  return z;  }

  GeoPoint3D operator+ ( GeoPoint3D p );
  GeoPoint3D operator-( GeoPoint3D p );
  GeoPoint3D operator= ( GeoPoint3D p );

  double dist (  GeoPoint3D p );

  virtual bool getBoundingBox ( double * sw, double * no );
};

///////////////////////////////////////////////////////////////////////////////
//  _Surface3D				                              												 //
//  Abstrakte Oberklasse für gml:Surface (aufgebaut aus Polygon-Patches)     //
//  und gml:Polygon mit 3D-Koordinaten                       								 //
///////////////////////////////////////////////////////////////////////////////
class _Surface3D: public Geometrie3D
{
public:
  _Surface3D();
  _Surface3D ( _Surface3D * pSurfaceOld );
  virtual ~ _Surface3D();

public:
  virtual GEOMETRY_TYPE getSurfaceType() = 0;

  virtual unsigned int getLochAnz() = 0;

  virtual bool getBoundingBox ( double * sw, double * no ) = 0;
  virtual bool getCenter ( double &xP, double &yP, double &zP ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
//  _Curve3D																                                 //
//  Abstrakte Oberklasse für gml:Curve (aufgebaut aus curve-Segments)		     //
//  und gml:LineString mit 3D-Koordinaten 				                           //
///////////////////////////////////////////////////////////////////////////////
class _Curve3D: public Geometrie3D
{
public:
  _Curve3D();
  _Curve3D ( _Curve3D * pCurveOld  );
  virtual ~ _Curve3D();

public:
  virtual GEOMETRY_TYPE getCurveType() = 0;

  virtual void reverse() = 0;
  virtual unsigned int getPoints ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, 
                                   std::vector<double> &zCoordsP ) = 0;
  virtual unsigned int getPointAnz() = 0;

  virtual void removeDoublePoints ( double minDist ) = 0;

  virtual GeoPoint3D  getAnfangsPunkt () = 0;
  virtual GeoPoint3D  getEndPunkt() = 0;

  virtual bool getBoundingBox ( double * sw, double * no ) = 0;
  virtual bool getCenter ( double &xP, double &yP, double &zP ) = 0;
  virtual double getLength () = 0;
};

///////////////////////////////////////////////////////////////////////////////
//  _Ring3D																                                   //
//  Implementierung der abstrakten Oberklasse für gml:Ring und				       //
//  gml:LinearRing mit 3D-Geometrie 						                             //
///////////////////////////////////////////////////////////////////////////////
class _Ring3D: public Geometrie3D
{
public:
  _Ring3D();
  _Ring3D ( _Ring3D * pRingOld );
  virtual ~ _Ring3D();
public:
  virtual GEOMETRY_TYPE getRingType() = 0;

  virtual _Ring3D * copyRing () = 0;

  virtual unsigned int getPoints ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP ) = 0;
  virtual unsigned int getPointAnz() = 0;

  virtual void removeDoublePoints ( double minDist ) = 0;
  virtual bool getBoundingBox ( double * sw, double * no ) = 0;
  virtual void reverse() = 0;

  virtual bool getCenter ( double &cxP, double &cyP, double &czP );
};

///////////////////////////////////////////////////////////////////////////////
//  MultiSurface3D			                              											 //
//  Klasse für eine gml:MultiSurface oder gml:CompositeSurface	      			 //
///////////////////////////////////////////////////////////////////////////////
class MultiSurface3D: public _Surface3D
{
public:
  MultiSurface3D();
  MultiSurface3D ( MultiSurface3D * pMultiSurfaceOld );
  virtual ~MultiSurface3D();

protected:
  std::vector<_Surface3D*> position;
  bool                     isCompositeSurface;

public:
  virtual GEOMETRY_TYPE getGeometryType() { return MULTI_SURFACE_3D;  }
  virtual GEOMETRY_TYPE getSurfaceType();

  void addSurface ( _Surface3D * pPolyP )  {  position.push_back ( pPolyP );  }
  _Surface3D * getSurface ( unsigned int indexP );
  unsigned int getSurfaceMemberAnz()  {  return position.size();  }
  unsigned int getPolygone ( std::vector<Polygon3D*> &vPolygone );
  unsigned int getLochAnz();

  virtual bool getBoundingBox ( double * sw, double * no );
  bool getCenter ( double &xP, double &yP, double &zP );

  void setIsCompositeSurface ( bool b )  {  isCompositeSurface = b;  }
  bool getIsCompositeSurface ()  {  return isCompositeSurface;  }
};

///////////////////////////////////////////////////////////////////////////////
//  MultiCurve3D			                              												 //
//  Klasse für eine Multi-Curve											                      	 //
///////////////////////////////////////////////////////////////////////////////
class MultiCurve3D: public Geometrie3D
{
public:
  MultiCurve3D();
  MultiCurve3D ( MultiCurve3D * pMultiCurveOld );
  virtual ~MultiCurve3D();

protected:
  std::vector<_Curve3D*> position;
  bool				           isCompositeCurve;

public:
  virtual GEOMETRY_TYPE getGeometryType() { return MULTI_CURVE_3D;  }

  void addCurve (  _Curve3D * pCurve );
  _Curve3D * getCurve ( unsigned int index );
  unsigned int getCurveMemberAnz()  {  return position.size();  }
  unsigned int getCurves ( std::vector<_Curve3D*> &vCurves );

  virtual bool getBoundingBox ( double * sw, double * no );
  bool getCenter ( double &xP, double &yP, double &zP );
  double getLength();

  void reverse();

  void setIsCompositeCurve ( bool b )  {  isCompositeCurve = b;  }
  bool getIsCompositeCurve ()  {  return isCompositeCurve;  }

protected:
  void joinSegments ( std::vector<CurveSegment3D*> & vCurveSegments, double minDist );
  void deleteSegment ( std::vector<CurveSegment3D*> & vCurveSegments, unsigned int indexP );
};

///////////////////////////////////////////////////////////////////////////////
//  MultiPoint3D															                               //
//  Klasse für Multi-Points													                         //
///////////////////////////////////////////////////////////////////////////////
class MultiPoint3D: public Geometrie3D
{
public:
  MultiPoint3D();
  MultiPoint3D ( MultiPoint3D * pMultiPointOld );
  virtual ~ MultiPoint3D();

protected:
  std::vector<GeoPoint3D*>  position;

public:
  virtual GEOMETRY_TYPE getGeometryType() { return MULTI_POINT;  }

  void addPoint (  GeoPoint3D * pPoint );
  GeoPoint3D * getPoint ( unsigned int index );
  unsigned int getPointAnz()  {  return position.size();  }
  unsigned int getPoints ( std::vector<double> &xCoordP, std::vector<double> &yCoordP, std::vector<double> &zCoordP );

  virtual bool getBoundingBox ( double * sw, double * no );
  bool getCenter ( double &xP, double &yP, double &zP );
};

///////////////////////////////////////////////////////////////////////////////
//  Surface3D			                                													 //
//  Implementierung der Klasse gml:Surface			                						 //
///////////////////////////////////////////////////////////////////////////////
class Surface3D: public _Surface3D
{
public:
  Surface3D();
  Surface3D ( Surface3D * pSurfaceOld );
  virtual ~Surface3D();

protected:
  std::vector<Polygon3D*> patches;

public:
  virtual GEOMETRY_TYPE getGeometryType() { return SURFACE_3D;  }
  virtual GEOMETRY_TYPE getSurfaceType() {  return SURFACE_3D;  }

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter ( double &cxP, double &cyP, double &czP );

  void addPolygon ( Polygon3D * pPoly ) {  patches.push_back ( pPoly ); }
  int getPatches ( std::vector<Polygon3D*> &vPatches );
  int getPatchesAnz()  {  return patches.size();  }

  virtual unsigned int getLochAnz();
};


///////////////////////////////////////////////////////////////////////////////
//  Implementierung der Klasse gml:Polygon			                						 //
///////////////////////////////////////////////////////////////////////////////
class Polygon3D: public _Surface3D
{
public:
  Polygon3D();
  Polygon3D ( Polygon3D * pPolyOld );
  virtual ~Polygon3D();

protected:
  _Ring3D			 * aussenkontur;
  std::vector<_Ring3D*> loecher;

public:
  virtual GEOMETRY_TYPE getGeometryType() { return POLYGON_3D;  }
  virtual GEOMETRY_TYPE getSurfaceType() {  return POLYGON_3D;  }

  void setAussenkontur ( _Ring3D * aussenkonturP );
  _Ring3D * getAussenkontur()  {  return aussenkontur;  }

  void addLoch ( _Ring3D * lochP );
  _Ring3D * getLoch ( int indexP ) {  return loecher[indexP];  }
  virtual unsigned int getLochAnz()  {  return loecher.size();  }

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter ( double &cxP, double &cyP, double &czP );
};

///////////////////////////////////////////////////////////////////////////////
//  Ring3D                                                                   //
//  Implementierung der Klasse gml:Ring			                  							 //
///////////////////////////////////////////////////////////////////////////////
class Ring3D : public _Ring3D
{
public:
  Ring3D();
  Ring3D( Ring3D * pRingOld );
  virtual ~Ring3D();

protected:
  MultiCurve3D * pGeometrie;

public:
  virtual GEOMETRY_TYPE getGeometryType() { return RING_3D;  }
  virtual GEOMETRY_TYPE getRingType() {  return RING_3D;  }

  virtual _Ring3D * copyRing ();

  void setRingGeometrie ( MultiCurve3D * pMultiCurve );
  MultiCurve3D * getRingGeometrie()   {  return pGeometrie;  }

  void addRingMember ( _Curve3D * pCurveMember );
  _Curve3D * getRingMember ( int indexP );
  unsigned int getRingMemberAnz();

  virtual unsigned int getPoints ( std::vector<double> &vXP, std::vector<double> &vYP, 
                                   std::vector<double> &vZP );
  virtual unsigned int getPointAnz();

  virtual void removeDoublePoints ( double minDist );

  virtual bool getBoundingBox ( double * sw, double * no );

  virtual void reverse();

  virtual double getUmfang();
};

///////////////////////////////////////////////////////////////////////////////
//  LinearRing3D															                              	 //
//  Implementierung der Klasse gml:LinearRing							                	 //
///////////////////////////////////////////////////////////////////////////////
class LinearRing3D : public _Ring3D
{
public:
  LinearRing3D();
  LinearRing3D( LinearRing3D * pRingOld );
  virtual ~LinearRing3D();

protected:
  LineSegment3D * points;

public:
  virtual GEOMETRY_TYPE getGeometryType() { return RING_3D;  }
  virtual GEOMETRY_TYPE getRingType() {  return LINEAR_RING_3D;  }

  virtual _Ring3D * copyRing ();

  void setLineSegment ( LineSegment3D * pLineSegment );
  void addPoints ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
  void addPoints ( std::vector<GeoPoint3D> &vPoints );
  unsigned int getPoints ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
  unsigned int getPointAnz();

  virtual void removeDoublePoints ( double minDist );

  LineSegment3D * getCurve()  {  return points;  }

  virtual bool getBoundingBox ( double * sw, double * no );

  virtual void reverse();

  virtual double getUmfang();
};

///////////////////////////////////////////////////////////////////////////////
//  Curve3D                                                                  //
//  Implementierung der Klasse gml:Curve					                  				 //
///////////////////////////////////////////////////////////////////////////////
class Curve3D: public _Curve3D
{
public:
  Curve3D();
  Curve3D ( Curve3D * pCurveOld );
  virtual ~Curve3D();

protected:
  std::vector<CurveSegment3D*> segments;

public:
  virtual GEOMETRY_TYPE getGeometryType() { return CURVE_3D;  }
  virtual GEOMETRY_TYPE getCurveType() {  return CURVE_3D;  }

  void addCurveSegment ( CurveSegment3D * segmentP )  {  segments.push_back ( segmentP );  }
  CurveSegment3D * getCurveSegment( int indexP )  {  return segments[indexP];  }
  unsigned int getCurveSegmentAnz()  {  return segments.size();    }
  virtual unsigned int getPoints ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, 
                                   std::vector<double> &zCoordsP );
  virtual unsigned int getPointAnz();

  virtual void removeDoublePoints ( double minDist );

  virtual GeoPoint3D  getAnfangsPunkt ();
  virtual GeoPoint3D  getEndPunkt();

  virtual void reverse();

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter ( double &xP, double &yP, double &zP );
  virtual double getLength();
};

///////////////////////////////////////////////////////////////////////////////
//  CurveSegment3D                                                           //
//  Implementierung der Klasse gml:CurveSegment							              	 //
///////////////////////////////////////////////////////////////////////////////
class CurveSegment3D: public _Curve3D
{
public:
  CurveSegment3D();
  CurveSegment3D( CurveSegment3D * pCurveSegmentOld );
  virtual ~ CurveSegment3D();

protected:
  std::vector<GeoPoint3D> points;

public:
  virtual GEOMETRY_TYPE getCurveType() = 0;

  virtual void addPoints ( std::vector<double> xCoordsP, std::vector<double> yCoordsP, std::vector<double> zCoordsP );
  virtual void addPoints ( std::vector<GeoPoint3D> &vPoints );
  virtual unsigned int getPoints ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
  virtual unsigned int getPointAnz();

  virtual void removeDoublePoints ( double minDist );

  virtual GeoPoint3D getAnfangsPunkt ();
  virtual GeoPoint3D getEndPunkt();

  virtual void reverse();
  void join ( CurveSegment3D * pSegP );

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter ( double &xP, double &yP, double &zP );

  virtual double getLength() = NULL;
};

///////////////////////////////////////////////////////////////////////////////
//  LineSegment3D                                                            //
//  Implementierung der Klasse gml:LineSegment							              	 //
///////////////////////////////////////////////////////////////////////////////
class LineSegment3D: public CurveSegment3D
{
public:
  LineSegment3D();
  LineSegment3D( LineSegment3D * pLineSegmentOld );
  virtual ~ LineSegment3D();

public:
  virtual GEOMETRY_TYPE getGeometryType() { return LINE_STRING_3D;  }
  virtual GEOMETRY_TYPE getCurveType() {  return LINE_STRING;  }

  virtual double getLength();
};


