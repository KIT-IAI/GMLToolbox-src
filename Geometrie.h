#pragma once

#include "Includes.h"

class _Geometrie;
class _Surface;
class _Ring;
class _Curve;
class Ring;
class Curve;
class CurveSegment;
class LineSegment;
class GeoPoint;
class PolygonGeo;
class Surface;
class ErrorReport;
class Feature;
class MultiCurve;
class MultiSurface;
class Matrix4;
class Features;
class Direction;
class Plane;
class SrsManager;
class CheckingParams;
class BRep;
class Error;
class Line;
class LinearRing;
class Triangle;

enum FIRST_POINT {  MOVE, LINE, OMIT  };

///////////////////////////////////////////////////////////////////////////////
//  Geometrie																                                 //
//  Abstrakte Basisklasse für alle Geometrie-Klassen                         //
///////////////////////////////////////////////////////////////////////////////
class _Geometrie
{
public:
	_Geometrie ( int dimP );
	_Geometrie ( _Geometrie * pGeoOld );
  _Geometrie ( const _Geometrie &geoOld );

protected:
	std::string           gmlId;      // Eindeutige id des Geometrieobjektes
	std::string           srsName;    // Bezeichnung des Koordinaten-Referenzsystems
  double                m_sw[3];    // Untere, linke Ecke der Bounding Box
  double                m_no[3];    // Obere, rechte Ecke der Bounding Box
  bool                  bbExist;    // Gibt an, ob eine Bounding Box bereits berechnet wurde
	int                   dim;        // Dimension (2 oder 3)
  GEOMETRY_CHECK_RESULT checkResult; // Ergebnis einer Überprüfung des Geometrieobjektes

public:
  virtual GEOMETRY_TYPE getGeometryType() = NULL;
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType() = NULL;

  GEOMETRY_CHECK_RESULT getCheckResult()  {  return checkResult;  }
  virtual void setCheckResult ( GEOMETRY_CHECK_RESULT r )  {  checkResult = r;  }

	virtual bool getBoundingBox ( double * sw, double * no ) = 0;
  virtual bool getCenter2D ( double &xP, double &yP ) = 0;
  virtual bool getCenter3D ( double &xP, double &yP, double &zP ) = 0;
  virtual size_t  getPolygonAnz() = 0;
  virtual size_t getCurveSegmentAnz() = 0;
  virtual size_t getPointAnz() = 0;

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone ) = NULL;
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations ) = NULL;

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries ) = NULL;

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport ) = 0;
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false ) = 0;
  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  static _Geometrie * copy ( _Geometrie * pGeo );

  static System::String ^ toString ( GEOMETRY_TYPE geoType );

  void setDim( int dimP )  {  dim = dimP;  }
  virtual int getDim()  {  return dim; }

	void setGmlId ( std::string id )  {  gmlId = id;  }
	virtual std::string getGmlId ()  {  return gmlId;  }
  virtual System::String ^ getGML_ID();

	void setSrsName ( std::string name )  {  srsName = name;  }
	virtual std::string  getSrsName ()  {  return srsName;  }
  virtual bool existReferenceSystem( SrsManager * pSrsManager ) = 0;

  static CUT_RESULT schneidet ( double p1x, double p1y, double p2x, double p2y,
                          double q1x, double q1y, double q2x, double q2y, double minDist, double minAngleDist );
  static CUT_RESULT cutPoints ( double p1x, double p1y, double p2x, double p2y,
    double q1x, double q1y, double q2x, double q2y, double minDist, double minAngleDist, std::vector<GeoPoint> &vCutPoints );

  static double lengthRatio ( double pa[], double pe[], double q[] );

  static void generateNPolygon ( GeoPoint p, double r,GeoPoint normal, int n, std::vector<GeoPoint> &points ); 

  static bool lineAreaCut ( GeoPoint pLine, GeoPoint nLine, GeoPoint pArea, GeoPoint nArea, GeoPoint & p );

  static std::string convertTypeToString ( GEOMETRY_TYPE type );
  static std::string convertTypeToString ( POLYGON_TOPOLOGY type  );
};

///////////////////////////////////////////////////////////////////////////////
//  _GeometryReference                                                       //
//  Abstrakte Basisklasse für alle Geometry-Reference Klassen                //
///////////////////////////////////////////////////////////////////////////////
class _GeometryReference
{
public:
  _GeometryReference (  Features * pFeaturesP );
  _GeometryReference ( _GeometryReference * pGeoReferenceOld );
  virtual ~_GeometryReference();

protected:
  std::string refGeoId;    // id des Referierten Objektes
  Features  * pFeatures;   // GML-Features Modul

public:
  void setRefGeoId ( std::string id )  {  refGeoId = id;  }
  std::string  getRefGeoId ()  {  return refGeoId;  }

  _Geometrie * getRefGeoObjekt();
};

///////////////////////////////////////////////////////////////////////////////
//  GeometryReference                                                        //
//  Referenz auf eine beliebiges GML-Geometrieobjekt                         //
///////////////////////////////////////////////////////////////////////////////
class GeometryReference : public _Geometrie, public _GeometryReference
{
public:
  GeometryReference( Features * pFeaturesP );
  GeometryReference ( GeometryReference * pGeoReferenceOld );
  virtual ~GeometryReference();

protected:
  std::string refGeoType;   // Typ des referierten Geometrieobjektes

public:
  virtual GEOMETRY_TYPE getGeometryType()  {  return GEOMETRY_REFERENCE;  }
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType();

  void setRefGeoType ( std::string type )  {  refGeoType = type;  }
  std::string getRefGeoType()  {  return refGeoType;  }

  virtual int getDim();

  virtual std::string  getSrsName ();
  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  );
};

///////////////////////////////////////////////////////////////////////////////
//  GeoPoint                                                                 //
//  Implementierung der Klasse gml:Point									                   //
///////////////////////////////////////////////////////////////////////////////
class GeoPoint: public _Geometrie
{
public:
  GeoPoint ();
  GeoPoint ( int dimP );
  GeoPoint ( double xP, double yP );
  GeoPoint ( double xP, double yP, double zP );
  GeoPoint ( GeoPoint * pPointOld );
  GeoPoint ( const GeoPoint & pointOld );
  virtual ~ GeoPoint();

  public:
    double x;   // x-Koordinate
    double y;   // y-Koordinate
    double z;   // z-Koordinate

  public:
    virtual GEOMETRY_TYPE getGeometryType() { return GEO_POINT;  }
    virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_POINT;  }

    void set ( double xP, double yP);
    void set ( double xP, double yP, double zP );

    void setX ( double xP )  {  x = xP;  }
    void setY ( double yP )  {  y = yP;  }
    void setZ ( double zP )  {  z = zP;  }

    double getX() const  {  return x;  }
    double getY() const  {  return y;  }
    double getZ() const  {  return z;  }

    GeoPoint operator+ ( GeoPoint p );
    GeoPoint operator-( GeoPoint p );
    GeoPoint operator= ( GeoPoint p );
    GeoPoint operator* ( double d );
    GeoPoint operator-();
    double operator[] ( int index );

    bool operator>= ( const GeoPoint p ) const;
    bool operator<= ( const GeoPoint p ) const;
    bool operator== ( const GeoPoint p ) const;

    GeoPoint operator+= ( const GeoPoint p );
    GeoPoint operator-= ( const GeoPoint p );

    virtual void transform ( Matrix4 & pMatrix );
    virtual void transformInvers ( Matrix4 & pMatrix );

    virtual double dist (  GeoPoint p );
    virtual double mult ( GeoPoint p );
    virtual void normalize();

    virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
    virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
            std::vector<Error*> &vErrorReport, bool checkAnyway = false );

    virtual bool getBoundingBox ( double * sw, double * no );
    virtual bool getCenter2D ( double &xP, double &yP );
    virtual bool getCenter3D ( double &xP, double &yP, double &zP );
    virtual size_t  getPolygonAnz();
    virtual size_t getCurveSegmentAnz();
    virtual size_t getPointAnz();

    virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
    virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

    virtual bool existReferenceSystem( SrsManager * pSrsManager );

    virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  );

    virtual std::string toStr ();
};

///////////////////////////////////////////////////////////////////////////////
//  DirectPosition                                                           //
//  Implementierung der Klasse gml:DirectPosition                            //
///////////////////////////////////////////////////////////////////////////////
class DirectPosition: public _Geometrie
{
  public:
    DirectPosition();
    virtual ~DirectPosition();

  protected:
    std::vector<double> vPos;   // Koordinatenwerte

  public:
    virtual GEOMETRY_TYPE getGeometryType() { return DIRECT_POSITION;  }
    virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_POINT;  }

    void set ( std::vector<double> vPosP );
		size_t  get ( std::vector<double> &vPosP );
    
    virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
    virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
            std::vector<Error*> &vErrorReport, bool checkAnyway = false );

    virtual bool getBoundingBox ( double * sw, double * no )  {  return false;  }
    virtual bool getCenter2D ( double &xP, double &yP )  {  return false;  }
    virtual bool getCenter3D ( double &xP, double &yP, double &zP )  {  return false;  }
    virtual size_t  getPolygonAnz() {  return 0;  }
    virtual size_t getCurveSegmentAnz()  {  return 0;  }
    virtual size_t getPointAnz()  {  return 0;  }

    virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone )  {  return 0;  }
    virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )  {  return 0;  }
    
    virtual bool existReferenceSystem( SrsManager * pSrsManager );

    virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  );

};

///////////////////////////////////////////////////////////////////////////////
//  IntersectionGeometry                                                     //
//  Datenstruktur für die Repräsentation von Überschneidungen von Flächen    //
///////////////////////////////////////////////////////////////////////////////
class IntersectionGeometry
{
public:
  IntersectionGeometry();
  virtual ~IntersectionGeometry();

public:
  std::vector<GeoPoint>         vCutPoints;       // Liste aller Schnittpunkte der Flächenkonturen
  std::vector<POLYGON_TOPOLOGY> vCutPointTypes;   // Topologische Klassifizierung der Schnittpunke
  std::vector<LineSegment>      vCutSegments;     // Liste aller Berührungslinien der Flächenkonturen
  std::vector<POLYGON_TOPOLOGY> vCutSegmentTypes; // Tolopogische Klassifizierung der Linien
  std::vector<LinearRing*>      vCutRings;        // Liste aller Überschneidungsflächen

public:
  void transform ( Matrix4 & mat );
  void transformInvers ( Matrix4 & mat );
  void copy ( IntersectionGeometry & intersection );
  void clear();
};

///////////////////////////////////////////////////////////////////////////////
//  CutResult                                                                //
//  Datenstruktur für Verschneidung zweier Liniensegmente                    //
///////////////////////////////////////////////////////////////////////////////
struct CutResult
{
public:
  CUT_RESULT cutResult;
	size_t      index1;
	size_t      index2;
};

///////////////////////////////////////////////////////////////////////////////
//  _Surface				                                												 //
//  Abstrakte Oberklasse für gml:Surface (aufgebaut aus Polygon-Patches)     //
//  und gml:Polygon							                            								 //
///////////////////////////////////////////////////////////////////////////////
class _Surface: public _Geometrie
{
public:
	_Surface( int dim );
	_Surface ( _Surface * pSurfaceOld );
	virtual ~ _Surface();

protected:
  BRep * pBRep;  // Repräsentation der Fläche als BRep

public:
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_SURFACE;  }

  virtual double getFlaecheMitLoechern () = 0;
  virtual double getFlaeche() = 0;

  virtual _Surface * removePolygon ( std::string gmlId ) = NULL;

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport ) = NULL;
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false ) = NULL;
  virtual bool topologyCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
        std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool overlapCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams,
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual POLYGON_TOPOLOGY intersect ( _Surface *pSurface, double minDist, double minAngleDist, bool ignoreHoles, IntersectionGeometry & intersectGeo );

 bool checkUeberlappung2D( double minDist, double minAngleDist );
 bool checkUeberlappung2D( double minDist, double minAngleDist, double &ueberlappungsFlaeche );

  bool checkUeberlappung2D( _Surface * pSurface, double minDist, double minAngleDist );
  bool checkUeberlappung2D( _Surface * pSurface, double minDist, double minAngleDist, double &ueberlappungsFlaeche );

  bool getUeberlappungsflaeche2D ( double minDist, double minAngleDist, MultiSurface * pUeberlappungsFlaeche );
  bool getUeberlappungsflaeche2D ( _Surface * pSurface, double minDist, double minAngleDist, MultiSurface * pUeberlappungsFlaeche );

  virtual SURFACE_NORMAL_TYPE getSurfaceNormal ( double angleTolerance, double areaTolerance ) = NULL;
  virtual Direction * getSurfaceNormal ( double angleTolerance );

  static std::string convertOrientationToString ( SURFACE_NORMAL_TYPE type );

  void generateTopology( double minDist, Features * pFeaturesP );
  BRep * getTopology();

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  );
};

///////////////////////////////////////////////////////////////////////////////
//  SurfaceReference                                                         //
//  Referenz auf eine GML_Surface                                            //
///////////////////////////////////////////////////////////////////////////////
class SurfaceReference : public _Surface, public _GeometryReference
{
public:
  SurfaceReference( Features * pFearuresP );
  SurfaceReference ( SurfaceReference * pSurfaceReferenvcOld );
  virtual ~SurfaceReference();

protected:

public:
  virtual GEOMETRY_TYPE getGeometryType()  {  return SURFACE_REFERENCE;  }
  
  virtual int getDim();
  virtual std::string  getSrsName ();

  virtual _Surface * removePolygon ( std::string gmlId );

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual double getFlaecheMitLoechern ();
  virtual double getFlaeche();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );

  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual SURFACE_NORMAL_TYPE getSurfaceNormal ( double angleTolerance, double areaTolerance );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  );
};

///////////////////////////////////////////////////////////////////////////////
//  _Curve																	                                 //
//  Abstrakte Oberklasse für gml:Curve (aufgebaut aus curve-Segments)		     //
//  und gml:LineString														                           //
///////////////////////////////////////////////////////////////////////////////
class _Curve: public _Geometrie
{
public:
	_Curve( int dim );
	_Curve ( _Curve * pCurveOld  );
  _Curve ( const _Curve & curveOld  );
	virtual ~ _Curve();

public:
	virtual GEOMETRY_TYPE getCurveType() = 0;
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_CURVE;  }

	virtual void reverse() = 0;
	virtual std::string getPointsSVG ( double offsetX, double offsetY ) = 0;
	virtual std::string getPointsReverseSVG ( double offsetX, double offsetY ) = 0;
  virtual std::string getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode ) = 0;
  virtual std::string getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode ) = 0;

	virtual size_t getPoints2D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP ) = 0;
  virtual size_t getPoints3D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP ) = 0;
  virtual size_t getPoints2DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP ) = 0;
  virtual size_t getPoints3DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP ) = 0;
  virtual size_t getPoints ( std::vector<GeoPoint> &vPoints ) = 0;

  virtual bool checkDoublePoints ( double minDist, bool remove = false ) = 0;

  virtual GeoPoint  getAnfangsPunkt () = 0;
  virtual GeoPoint  getEndPunkt() = 0;

	virtual double getLength () = 0;

	virtual size_t scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
							std::vector<double> &angleP, double scanDist, bool reverse ) = 0;

  virtual size_t  getPolygonAnz()  {  return 0;  }

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone )  {  return 0; }
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )  {  return 0;  }

  virtual LineSegment * extractCurvePart ( double lengthAnf, double lengthEnd );
  virtual size_t  extractCurvePart ( double lengthAnf, double lengthEnd, std::vector<GeoPoint> &vPoints );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  ) = NULL;

	static _Curve * copy ( _Curve * pCurve );

  static void deleteCollinearPoints ( std::vector<GeoPoint> & vPoints );

};

///////////////////////////////////////////////////////////////////////////////
//  CurveReference                                                           //
//  Referenz auf eine _Curve                                                 //
///////////////////////////////////////////////////////////////////////////////
class CurveReference : public _Curve, public _GeometryReference
{
public:
  CurveReference( Features * pFeatures );
  CurveReference ( CurveReference * pCurveReferenceOld );
  virtual ~CurveReference();

protected:

public:
  virtual GEOMETRY_TYPE getGeometryType()  {  return CURVE_REFERENCE;  }
  virtual GEOMETRY_TYPE getCurveType();

  virtual int getDim();
  virtual std::string  getSrsName ();

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual void reverse();
  virtual std::string getPointsSVG ( double offsetX, double offsetY );
  virtual std::string getPointsReverseSVG ( double offsetX, double offsetY );
  virtual std::string getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode );
  virtual std::string getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode );

  virtual size_t getPoints2D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP );
  virtual size_t getPoints3D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
  virtual size_t getPoints2DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP );
  virtual size_t getPoints3DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
  virtual size_t getPoints ( std::vector<GeoPoint> &vPoints );

  virtual bool checkDoublePoints ( double minDist, bool remove = false );

  virtual GeoPoint  getAnfangsPunkt ();
  virtual GeoPoint  getEndPunkt();

  virtual double getLength ();

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );

  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  virtual size_t scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
    std::vector<double> &angleP, double scanDist, bool reverse );

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  );
};

///////////////////////////////////////////////////////////////////////////////
//  _Ring																	                                   //
//  Implementierung der abstrakten Oberklasse für gml:Ring und				       //
//  gml:LinearRing															                             //
///////////////////////////////////////////////////////////////////////////////
class _Ring: public _Geometrie
{
public:
  enum POINT_CLASSIFICATION {  ISOLATED, START_INNER_SEGMENT, END_INNER_SEGMENT, START_CONTOUR_SEGMENT,
                               CONTOUR_SEGMENT,  END_CONTOUR_SEGMENT  };
public:
	_Ring( int dim );
	_Ring ( _Ring * pRingOld );
	virtual ~ _Ring();

protected:
  Plane * e;   // Fläche, in der der Ring liegt

public:
	virtual GEOMETRY_TYPE getRingType() = 0;
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_RING;  }

  virtual _Ring * copyRing () = 0;

	virtual size_t getPoints2D ( std::vector<double> &vXP, std::vector<double> &vYP ) = 0;
  virtual size_t getPoints2DReverse ( std::vector<double> &vXP, std::vector<double> &vYP ) = 0;
  virtual size_t getPoints3D ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP ) = 0;
  virtual size_t getPoints3DReverse ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP ) = 0;

  virtual size_t getPoints ( std::vector<GeoPoint> &vPoints ) = 0;
  virtual size_t getPointsReverse ( std::vector<GeoPoint> &vPoints ) = 0;
  virtual GeoPoint getPoint ( size_t index ) = 0;

  virtual bool checkDoublePoints ( double minDist, bool remove = false ) = 0;

	virtual size_t scanRing2D ( std::vector<double> &xP, std::vector<double> &yP, 
							std::vector<double> &angleP, double scanDist, bool reverse ) = 0;

	virtual void reverse() = 0;
	virtual std::string getPointsSVG ( double offsetX, double offsetY ) =  0;
	virtual std::string getPointsReverseSVG ( double offsetX, double offsetY ) = 0;

	virtual UMLAUFSINN getUmlaufsinn2D();
  double getWinkelsumme ( std::vector<double> &vX, std::vector<double> &vY );
  
	AREA_LOCATION checkTopology2D ( _Ring * pRing, double minDist, double minAngtleDist );
  POLYGON_TOPOLOGY checkTopology2DGlobal ( _Ring * pRing, double minDist );

	virtual POINT_LOCATION checkInside2D ( double * p, double minDist, unsigned int &cutIndex, bool noCheck = false );
  virtual POINT_LOCATION checkInside2D ( GeoPoint p, double minDist, bool noCheck = false );
  virtual bool  cut2D ( _Ring * pRing, double minDist, IntersectionGeometry &intersection  );
  virtual bool  touch2D ( _Ring * pRing, double minDist, IntersectionGeometry &intersection  );
  virtual bool  difference2D ( _Ring * pRing, double minDist, std::vector<LinearRing*> &vDifferenceRings  );

  POLYGON_TOPOLOGY intersect2D ( _Ring *pRing, double minDist, IntersectionGeometry & intersection );
  POLYGON_TOPOLOGY intersect3D ( _Ring *pRing, Plane * e1, Plane * e2, double minDist, double minAngleDist, IntersectionGeometry & intersection );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
            std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  bool checkPlanarity ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport,
            double &maximalError, double & avarageEror, bool checkAnyway = false );


  bool calcPlane();
  Plane * getPlane();
  virtual double getFlaeche();
	virtual double getUmfang() = 0;

  static double calcWinkel2D ( double *h1, double * h2, double * h3 );

  virtual void transform ( Matrix4 & matrix ) = 0;
  virtual void transformInvers ( Matrix4 & matrix ) = 0;
  virtual _Ring* transformToHorizontal () = 0;

  virtual size_t  getPolygonAnz()  {  return 0;  }

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone )  {  return 0;  }
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )  {  return 0;  }

  LineSegment * transformToCurve();

	size_t getCutPoints ( Line & line,std::vector<GeoPoint> &vSortedPoints,std:: vector<double> &vLineParams,  std::vector<int> &vPointTypes, double minDist );
  static std::string convertPointTypeToString( POINT_CLASSIFICATION type );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  ) = NULL;

protected:
	double getFlaeche( std::vector<double> vX, std::vector<double> vY );
  static void eliminateDoublePoints (  std::vector<double> &vX, std::vector<double> &vY, double minDist );
  double getFlaeche2D();
	size_t checkSelfOverlap ( std::vector<double> &vX, std::vector<double> &vY, double minimalPointDistance, double minAngleDist, std::vector<CutResult> &vCutResults );
};

///////////////////////////////////////////////////////////////////////////////
//  MultiGeometry                                                            //
//  Klasse für gml:MultiGeometry                                             //
///////////////////////////////////////////////////////////////////////////////
class MultiGeometry: public _Geometrie
{
public:
  MultiGeometry ( int dim );
  MultiGeometry ( MultiGeometry * pMultiGeometryOld );
  virtual ~MultiGeometry();

protected:
  std::vector<_Geometrie*> position;   // Liste der Geometrie-Member

public:
  virtual GEOMETRY_TYPE getGeometryType() {  return MULTI_GEOMETRY;  }
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_MULTI_GEOMETRY;  }

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  void addGeometry ( _Geometrie * pGeo )  {  position.push_back( pGeo );  };
  _Geometrie * getGeometry ( size_t index );
	size_t getGeometryMemberAnz()  {  return position.size();  }

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  );
};

///////////////////////////////////////////////////////////////////////////////
//  GeometricComplex                                                         //
//  Klasse für gml:GeometricComplex                                          //
///////////////////////////////////////////////////////////////////////////////
class GeometricComplex: public _Geometrie
{
public:
  GeometricComplex ( int dim );
  GeometricComplex ( GeometricComplex * pGeometricComplexOld );
  virtual ~GeometricComplex();

protected:
  std::vector<_Geometrie*> position;   // Liste der Geometrie-Member

public:
  virtual GEOMETRY_TYPE getGeometryType() {  return GEOMETRIC_COMPLEX;  }
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_GEOMETRIC_COMPLEX;  }

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  void addGeometry ( _Geometrie * pGeo )  {  position.push_back( pGeo );  };
  _Geometrie * getGeometry ( size_t index );
	size_t getGeometryMemberAnz()  {  return position.size();  }

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  );
};

///////////////////////////////////////////////////////////////////////////////
//  MultiSurface				                              											 //
//  Klasse für eine gml:MultiSurface                                  			 //
///////////////////////////////////////////////////////////////////////////////
class MultiSurface: public _Surface
{
public:
	MultiSurface( int dimP );
	MultiSurface ( MultiSurface * pMultiSurfaceOld, Features * pFeatures = NULL, bool generateMemberLinks = false );
	virtual ~MultiSurface();

protected:
	std::vector<_Surface*> position;  // Liste der Surface-Member

public:
  virtual GEOMETRY_TYPE getGeometryType() { return MULTI_SURFACE;  }
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_MULTI_GEOMETRY;  }

	void addShapePolygons2D ( std::vector<double> xCoordP, std::vector<double> yCoordP, 
				                    std::vector<int> ringOffsetsP, std::vector<int> ringTypenP, Features * pFeatures );
	void addSurface ( _Surface * pPolyP )  {  position.push_back ( pPolyP );  }
	_Surface * getSurface ( size_t indexP );
  void clearSurfaces()  {  position.clear();  }

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual size_t copyWithReferences ( Features * pFeatures, std::vector<SurfaceReference*> &vRefs );

	size_t getSurfaceMemberAnz()  {  return position.size();  }

	virtual bool getBoundingBox ( double * sw, double * no );
	void removeMultiPolygon();
	bool getCenter2D ( double &xP, double &yP );
  bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual bool hasLoch();
  virtual _Surface * removePolygon( std::string gmlId );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool topologyCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  virtual double getFlaecheMitLoechern ();
  virtual double getFlaeche();

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual SURFACE_NORMAL_TYPE getSurfaceNormal ( double angleTolerance, double areaTolerance );

  virtual MultiCurve * createBoundaryCurves( double minDist, Features * pFeaturesP );

	size_t clusterPolygonsByOrientations ( std::map<Direction*,std::vector<PolygonGeo*>> &mClusters, double maximalAngleDifference );
  static size_t mergeCluster( std::vector<PolygonGeo*> vCluster, Direction clusterOrientation, 
                           std::vector<PolygonGeo*> &vMergedCluster, double maxPointDistance, double maxOrientationDistance );

  int mergeSurfaceMember ( double maximalAngleDifference, std::vector<PolygonGeo*> &vMergedPolygons,
                          double minimalPointDistance, double minimalAngleDistance );
    
  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  );
};

///////////////////////////////////////////////////////////////////////////////
//  CompositeSurface				                              									 //
//  Klasse für eine gml:CompositeSurface	                                   //
///////////////////////////////////////////////////////////////////////////////
class CompositeSurface: public MultiSurface
{
public:
  CompositeSurface( int dimP );
  CompositeSurface ( CompositeSurface * pCompositeSurfaceOld );
  virtual ~CompositeSurface();

public:
  virtual GEOMETRY_TYPE getGeometryType() { return COMPOSITE_SURFACE;  }
};

///////////////////////////////////////////////////////////////////////////////
//  Shell				                                          									 //
//  Klasse für eine gml:Shell           	                                   //
///////////////////////////////////////////////////////////////////////////////
class Shell: public CompositeSurface
{
public:
  Shell( int dimP );
  Shell ( Shell * pCompositeSurfaceOld );
  virtual ~Shell();

public:
  virtual GEOMETRY_TYPE getGeometryType() { return SHELL;  }
};


///////////////////////////////////////////////////////////////////////////////
//  MultiCurve				                              												 //
//  Klasse für eine Multi-Curve											                      	 //
///////////////////////////////////////////////////////////////////////////////
class MultiCurve: public _Geometrie
{
public:
	MultiCurve( int dimP );
	MultiCurve ( MultiCurve * pMultiCurveOld );
	virtual ~MultiCurve();

protected:
	std::vector<_Curve*> position;  //  Liste der Curve-Member
	bool				 isCompositeCurve;  // Gibt an, ob es sich um eine CompositeCurve handelt

public:
  virtual GEOMETRY_TYPE getGeometryType() { return MULTI_CURVE;  }
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_MULTI_GEOMETRY;  }

	void addShapeLines2D ( std::vector<double> xCoordP, std::vector<double> yCoordP, 
				                 std::vector<int> partOffsetsP, Features * pFeatures );

	void addCurve (  _Curve * pCurve );
	_Curve * getCurve ( size_t index );
	size_t getCurveMemberAnz()  {  return position.size();  }
	size_t getCurves ( std::vector<_Curve*> &vCurves );

	virtual bool getBoundingBox ( double * sw, double * no );
	bool getCenter2D ( double &xP, double &yP );
  bool getCenter3D ( double &xP, double &yP , double &zP );
  virtual size_t  getPolygonAnz()  {  return 0;  }
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();
  virtual void clearCurveMember()  {  position.clear();  }

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone )  {  return 0;  }
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )  {  return 0;  }

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

	double getLength();

	void reverse();

	void setIsCompositeCurve ( bool b )  {  isCompositeCurve = b;  }
	bool getIsCompositeCurve ()  {  return isCompositeCurve;  }

	std::string getPointsSVG ( double offsetX, double offsetY );
	std::string getPointsReverseSVG ( double offsetX, double offsetY );

	void removeMultiLine();

  virtual void transform ( Matrix4 & matrix );
  virtual void transformInvers ( Matrix4 & matrix );

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  );

protected:
	void joinSegments ( std::vector<CurveSegment*> & vCurveSegments, double minDist );
	void deleteSegment ( std::vector<CurveSegment*> & vCurveSegments, unsigned int indexP );
};

///////////////////////////////////////////////////////////////////////////////
//  MultiPoint																                               //
//  Klasse für Multi-Points													                         //
///////////////////////////////////////////////////////////////////////////////
class MultiPoint: public _Geometrie
{
public:
	MultiPoint( int dimP );
	MultiPoint ( MultiPoint * pMultiPointOld );
	virtual ~ MultiPoint();

protected:
	std::vector<GeoPoint*>  position; // Liste der einzelnen Punkt-Member

public:
  virtual GEOMETRY_TYPE getGeometryType() { return MULTI_POINT;  }
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_MULTI_GEOMETRY;  }

	void addShapePoints2D ( std::vector<double> xCoordP, std::vector<double> yCoordP, Features * pFeatures );

	void addPoint (  GeoPoint * pPoint );
	GeoPoint * getPoint ( size_t index );
	size_t getPoints2D ( std::vector<double> &xCoordP, std::vector<double> &yCoordP );
	size_t getPoints3D ( std::vector<double> &xCoordP, std::vector<double> &yCoordP, std::vector<double> &zCoordP );
	size_t getPoints ( std::vector<GeoPoint*> &vPoints );
  void clearPoints()  {  position.clear();  }
  
  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

	virtual bool getBoundingBox ( double * sw, double * no );
	bool getCenter2D ( double &xP, double &yP );
  bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz()  {  return 0;  }
  virtual size_t getCurveSegmentAnz()  {  return 0;  }
  virtual size_t getPointAnz()  {  return position.size();  }

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone )  {  return 0;  }
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations )  {  return 0;  }

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries  );
};

///////////////////////////////////////////////////////////////////////////////
//  OrientableSurface                                                        //
//  Klasse für gml:OrientableSurface                                         //
///////////////////////////////////////////////////////////////////////////////
class OrientableSurface: public _Surface
{
public:
  OrientableSurface ( int dimP );
  OrientableSurface ( OrientableSurface * pSurfaceOld );
  virtual ~ OrientableSurface();

protected:
  bool       orientation;   // Orientierung der Fläche
  _Surface * pBaseSurface;  // Basisfläche

public:
  virtual GEOMETRY_TYPE getGeometryType() { return ORIENTABLE_SURFACE;  }

  void setOrientation ( bool orientationP )  {  orientation = orientationP;  }
  bool getOrientation()                      {  return orientation;  }

  void setBaseSurface ( _Surface * pSurface  )  {  pBaseSurface = pSurface;  }
  _Surface * getBaseSurface ()  {  return pBaseSurface;  }

  virtual _Surface * removePolygon( std::string gmlId );

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &cxP, double &cyP );
  virtual bool getCenter3D ( double &cxP, double &cyP, double &czP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual double getFlaecheMitLoechern ();
  virtual double getFlaeche();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual SURFACE_NORMAL_TYPE getSurfaceNormal ( double angleTolerance, double areaTolerance );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};

///////////////////////////////////////////////////////////////////////////////
//  Surface				                                													 //
//  Implementierung der Klasse gml:Surface			                						 //
///////////////////////////////////////////////////////////////////////////////
class Surface: public _Surface
{
public:
	Surface( int dimP );
	Surface ( Surface * pSurfaceOld );
	virtual ~Surface();

protected:
	std::vector<PolygonGeo*> patches;  // Liste der Polygone, aus denen sich die gml:Surface zusammensetzt

public:
  virtual GEOMETRY_TYPE getGeometryType() { return SURFACE;  }

	virtual bool getBoundingBox ( double * sw, double * no );
	virtual bool getCenter2D ( double &cxP, double &cyP );
  virtual bool getCenter3D ( double &cxP, double &cyP, double &czP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

	void addPolygon ( PolygonGeo * pPoly ) {  patches.push_back ( pPoly );  }
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );
  PolygonGeo * getPolygon ( unsigned int index );

  virtual double getFlaecheMitLoechern ();
  virtual double getFlaeche();

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );

  bool transformToPolygon2D ( double minDist, double minAngleDist );

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual SURFACE_NORMAL_TYPE getSurfaceNormal ( double angleTolerance, double areaTolerance );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );

  virtual _Surface * removePolygon( std::string gmlId );
};

///////////////////////////////////////////////////////////////////////////////
//  TriangulatedSurface                                   									 //
//  Klasse für eine gml:TriangulatedSurface                                  //
///////////////////////////////////////////////////////////////////////////////
class TriangulatedSurface: public Surface
{
public:
  TriangulatedSurface( int dimP );
  TriangulatedSurface ( TriangulatedSurface * pTriangulatedSurfaceOld );
  virtual ~TriangulatedSurface();

public:
  virtual GEOMETRY_TYPE getGeometryType() { return TRIANGULATED_SURFACE; }
};

///////////////////////////////////////////////////////////////////////////////
//  PolygonGeo                                                               //
//  Implementierung der Klasse gml:Polygon			                						 //
///////////////////////////////////////////////////////////////////////////////
class PolygonGeo: public _Surface
{
public:
	PolygonGeo( int dimP );
	PolygonGeo ( PolygonGeo * pPolyOld );
	virtual ~PolygonGeo();

protected:
	_Ring			           * aussenkontur;  // Außenkontur des Polygons
	std::vector<_Ring*>    loecher;       // Liste der Innenkonturen des Polygons
  std::vector<Triangle*> vTriangles;    // Triangulation des Polygons

public:
  virtual GEOMETRY_TYPE getGeometryType() { return POLYGON;  }

  void addShapePolygons2D ( std::vector<double> xCoordP, std::vector<double> yCoordP, 
    std::vector<int> ringOffsetsP, std::vector<int> ringTypenP, Features * pFeatures );

	void setAussenkontur ( _Ring * aussenkonturP );
	_Ring * getAussenkontur() const {  return aussenkontur;  }

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

	void addLoch ( _Ring * lochP );
	_Ring * getLoch ( size_t indexP ) const {  return loecher[indexP];  }
	virtual size_t getLochAnz() const  {  return loecher.size();  }

  _Ring * removeLoch ( std::string gmlId );

	bool isHole2D ( _Ring * pRingP, double minDist, double minAngleDist );
	int checkTopology2D ( PolygonGeo * pPolygon, double minDist, double minAngleDist );
  int checkTopology2D ( _Ring * pRing, double minDist, double minAngleDist );

  virtual POLYGON_TOPOLOGY intersect ( PolygonGeo *pPolygon, double minDist, double minAngleDist, bool ignoreHoles, IntersectionGeometry & intersectGeo );

  POLYGON_TOPOLOGY intersect2D ( PolygonGeo *pPolygon, Plane * e, double minDist, bool ignoreHoles, IntersectionGeometry & intersectGeo );
  POLYGON_TOPOLOGY intersect3D ( PolygonGeo *pPolygon, Plane * e1, Plane * e2, double minDist, double minAngleDist, bool ignoreHoles, IntersectionGeometry & intersectGeo );

  bool getUeberlappungsflaeche2D (  PolygonGeo * pPolygon, double minDist, double minANgleDist, MultiSurface * pUeberlappungsflaeche );
  bool getDifferenzflaeche2D ( _Ring * pRing, double minDist, double minAngleDist, MultiSurface * pUeberlappungsflaeche );
  bool getDifferenzflaecheUeberlappend2D ( std::vector<_Ring*> vRinge, double minDist, double minAngleDist,  MultiSurface * pUeberlappungsflaeche );

	virtual bool getBoundingBox ( double * sw, double * no );
	virtual bool getCenter2D ( double &cxP, double &cyP );
  virtual bool getCenter3D ( double &cxP, double &cyP, double &czP);
  virtual size_t  getPolygonAnz() {   return 1;  }
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual double getFlaeche();
  virtual double getFlaecheMitLoechern ();

  virtual Direction * getNormal();
  virtual Plane * getPlane();

  virtual void reverse();

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );

  virtual void transform ( Matrix4 &  matrix );
  virtual void transformInvers ( Matrix4 &  matrix );
  virtual PolygonGeo * transformToHorizontal();
  virtual PolygonGeo * transformToHorizontal( Plane * pPlane );

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual SURFACE_NORMAL_TYPE getSurfaceNormal ( double angleTolerance, double areaTolerance );

  virtual bool changeContourNarrowingIntoVoid ( size_t index1, size_t index2, CheckingParams * pParams );

  virtual void triangulate();
	size_t getTriangles ( std::vector<Triangle*> & vTrianglesP );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );

  virtual _Surface * removePolygon( std::string gmlId );
};

///////////////////////////////////////////////////////////////////////////////
//  Triangle                                                                 //
//  Implementierung der Klasse gml:Triangle                                  //
///////////////////////////////////////////////////////////////////////////////
class Triangle: public PolygonGeo
{
public:
  Triangle( int dimP );
  Triangle ( Triangle * pPolyOld );
  virtual ~Triangle();

public:
  virtual GEOMETRY_TYPE getGeometryType() { return TRIANGLE;  }
  void set ( GeoPoint p0, GeoPoint p1, GeoPoint p2 );
  void get ( GeoPoint &p0, GeoPoint &p1, GeoPoint &p2 );

};

///////////////////////////////////////////////////////////////////////////////
//  Ring                                                                     //
//  Implementierung der Klasse gml:Ring			                  							 //
///////////////////////////////////////////////////////////////////////////////
class Ring : public _Ring
{
public:
	Ring( int dimP );
	Ring( Ring * pRingOld );
	virtual ~Ring();

protected:
	MultiCurve * pGeometrie;  // Liniensegmente des Rings

public:
  virtual GEOMETRY_TYPE getGeometryType() { return RING;  }
	virtual GEOMETRY_TYPE getRingType() {  return RING;  }

  virtual _Ring * copyRing ();

	void setRingGeometrie ( MultiCurve * pMultiCurve );
	MultiCurve * getRingGeometrie()   {  return pGeometrie;  }
  void setRingGeometrie ( _Curve * pCurveMember );

	void addRingMember ( _Curve * pCurveMember );
	_Curve * getRingMember ( size_t indexP );
	size_t getRingMemberAnz();

	virtual size_t getPoints2D ( std::vector<double> &vXP, std::vector<double> &vYP );
  virtual size_t getPoints2DReverse ( std::vector<double> &vXP, std::vector<double> &vYP );
  virtual size_t getPoints3D ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP );
  virtual size_t getPoints3DReverse ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP );

  virtual size_t getPoints ( std::vector<GeoPoint> &vPoints );
  virtual size_t getPointsReverse ( std::vector<GeoPoint> &vPoints );
  virtual GeoPoint getPoint ( size_t index );

  virtual bool checkDoublePoints ( double minDist, bool remove = false );

	virtual size_t scanRing2D ( std::vector<double> &xP, std::vector<double> &yP, 
							std::vector<double> &angleP, double scanDist, bool reverse );

	virtual bool getBoundingBox ( double * sw, double * no );
  bool getCenter2D ( double &xP, double &yP );
  bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

	virtual void reverse();
	virtual std::string getPointsSVG ( double offsetX, double offsetY );
	virtual std::string getPointsReverseSVG ( double offsetX, double offsetY );

	virtual double getUmfang();
//  virtual double getFlaeche();

  virtual void transform ( Matrix4 & matrix );
  virtual void transformInvers ( Matrix4 & matrix );
  virtual _Ring  * transformToHorizontal ();

  virtual bool existReferenceSystem( SrsManager * pSrsManager );
  
  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};

///////////////////////////////////////////////////////////////////////////////
//  LinearRing															                              	 //
//  Implementierung der Klasse gml:LinearRing							                	 //
///////////////////////////////////////////////////////////////////////////////
class LinearRing : public _Ring
{
public:
	LinearRing( int dimP );
	LinearRing( LinearRing * pRingOld );
	virtual ~LinearRing();

protected:
	LineSegment * points;   //  AUßenkontur des LinearRing Objektes

public:
  virtual GEOMETRY_TYPE getGeometryType() { return LINEAR_RING;  }
  virtual GEOMETRY_TYPE getRingType() {  return LINEAR_RING;  }

  virtual _Ring * copyRing ();

	void setLineSegment ( LineSegment * pLineSegment );
	void addPoints2D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP );
  void addPoints3D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
  void addPoints ( std::vector<GeoPoint> &vPoints );
	
	virtual size_t getPoints2D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP );
	virtual size_t getPoints2DReverse ( std::vector<double> &vXP, std::vector<double> &vYP );
	virtual size_t getPoints3D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
	virtual size_t getPoints3DReverse ( std::vector<double> &vXP, std::vector<double> &vYP, std::vector<double> &vZP ) ;

	virtual size_t getPoints ( std::vector<GeoPoint> & vPoints );
  virtual size_t getPointsReverse ( std::vector<GeoPoint> & vPoints );

  virtual GeoPoint getPoint ( size_t index );

  virtual bool checkDoublePoints ( double minDist, bool remove = false );

	LineSegment * getCurve()  {  return points;  }

	virtual size_t scanRing2D ( std::vector<double> &xP, std::vector<double> &yP, 
							std::vector<double> &angleP, double scanDist, bool reverse );

	virtual bool getBoundingBox ( double * sw, double * no );
  bool getCenter2D ( double &xP, double &yP );
  bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t getCurveSegmentAnz()  {  return 1;  }
  virtual size_t getPointAnz();

	virtual void reverse();
	virtual std::string getPointsSVG ( double offsetX, double offsetY );
	virtual std::string getPointsReverseSVG ( double offsetX, double offsetY );
  virtual std::string getPointsAsString ( double offsetX, double offsetY );

	virtual double getUmfang();

  virtual void transform ( Matrix4 & matrix );
  virtual void transformInvers ( Matrix4 & matrix );
  virtual _Ring * transformToHorizontal ();

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual void setZKoordinate ( double zP );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};

///////////////////////////////////////////////////////////////////////////////
//  Curve                                                                    //
//  Implementierung der Klasse gml:Curve					                  				 //
///////////////////////////////////////////////////////////////////////////////
class Curve: public _Curve
{
public:
	Curve( int dimP );
	Curve ( Curve * pCurveOld );
	virtual ~Curve();

protected:
	std::vector<CurveSegment*> segments;  // Liste der Curve-Segmente

public:
  virtual GEOMETRY_TYPE getGeometryType() { return CURVE;  }
  virtual GEOMETRY_TYPE getCurveType() {  return CURVE;  }

	void addCurveSegment ( CurveSegment * segmentP )  {  segments.push_back ( segmentP );  }
	CurveSegment * getCurveSegment( size_t indexP )  {  return segments[indexP];  }
	virtual size_t getPoints2D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP );
  virtual size_t getPoints3D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
  virtual size_t getPoints2DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP );
  virtual size_t getPoints3DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
  virtual size_t getPoints ( std::vector<GeoPoint> &vPoints );

  virtual bool checkDoublePoints ( double minDist, bool remoove = false );

  virtual GeoPoint  getAnfangsPunkt ();
  virtual GeoPoint  getEndPunkt();

	virtual void reverse();
	virtual std::string getPointsSVG ( double offsetX, double offsetY );
	virtual std::string getPointsReverseSVG ( double offsetX, double offsetY );
  virtual std::string getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode );
  virtual std::string getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode );

	virtual bool getBoundingBox ( double * sw, double * no );
	virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t getCurveSegmentAnz()  {  return segments.size();  }
  virtual size_t getPointAnz();

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );

	virtual double getLength();

	virtual size_t scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
			 				std::vector<double> &angleP, double scanDist, bool reverse );

  virtual void transform ( Matrix4 & matrix );
  virtual void transformInvers ( Matrix4 & matrix );

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};

///////////////////////////////////////////////////////////////////////////////
//  OrientableCurve																												   //
//  Implementierung der Klasse gml:OrientableCurve												   //
///////////////////////////////////////////////////////////////////////////////
class OrientableCurve: public _Curve
{
public:
	OrientableCurve( int dimP );
	OrientableCurve ( OrientableCurve * pCurveOld );
	virtual ~OrientableCurve();

protected:
  _Curve    * pBaseCurve;  // Basis-Kurve
	std::string baseCurveId; // gml:id der Basiskurve
	bool        orientation; // Orientierung der OrientableCurve

public:
	virtual GEOMETRY_TYPE getGeometryType() { return ORIENTABLE_CURVE;  }
	virtual GEOMETRY_TYPE getCurveType() {  return ORIENTABLE_CURVE;  }

	void setBaseCurve ( _Curve * baseCurveP )  {  pBaseCurve = baseCurveP;  }
	_Curve * getBaseCurve()  {  return pBaseCurve;  }

	void setOrientation ( bool b )   {  orientation = b;  }
	bool getOrientation()  {  return orientation;  }

	void setBaseCurveId ( std::string id ) {  baseCurveId = id;  }
	std::string getBaseCurveId()   {  return baseCurveId;  }

	virtual size_t getPoints2D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP );
	virtual size_t getPoints3D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
	virtual size_t getPoints2DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP );
	virtual size_t getPoints3DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
	virtual size_t getPoints ( std::vector<GeoPoint> &vPoints );

	virtual bool checkDoublePoints ( double minDist, bool remove = false );

	virtual GeoPoint  getAnfangsPunkt ();
	virtual GeoPoint  getEndPunkt();

	virtual void reverse();
	virtual std::string getPointsSVG ( double offsetX, double offsetY );
	virtual std::string getPointsReverseSVG ( double offsetX, double offsetY );
	virtual std::string getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode );
	virtual std::string getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode );

	virtual bool getBoundingBox ( double * sw, double * no );
	virtual bool getCenter2D ( double &xP, double &yP );
	virtual bool getCenter3D ( double &xP, double &yP, double &zP );
	virtual size_t getCurveSegmentAnz();
	virtual size_t getPointAnz();

	virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
	virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
		std::vector<Error*> &vErrorReport, bool checkAnyway = false );

	virtual double getLength();

	virtual size_t scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
		std::vector<double> &angleP, double scanDist, bool reverse );

	virtual bool existReferenceSystem( SrsManager * pSrsManager );

	virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};


///////////////////////////////////////////////////////////////////////////////
//  CurveSegment                                                             //
//  Implementierung der Klasse gml:CurveSegment							              	 //
///////////////////////////////////////////////////////////////////////////////
class CurveSegment: public _Curve
{
public:
	CurveSegment( int dimP );
	CurveSegment( CurveSegment * pCurveSegmentOld );
  CurveSegment( const CurveSegment & curveSegmentOld );
	virtual ~ CurveSegment();

protected:
	std::vector<GeoPoint> points;        // Liste der Segment-Stützpunkte
	std::string           interpolation; // Interpolation der Stützpunkze

public:
	virtual GEOMETRY_TYPE getCurveType() = 0;

	virtual void addPoints2D ( std::vector<double> xCoordsP, std::vector<double> yCoordsP );
  virtual void addPoints3D ( std::vector<double> xCoordsP, std::vector<double> yCoordsP, std::vector<double> zCoordsP );
  virtual void addPoints ( std::vector<GeoPoint> &vPoints );
  virtual void addPoints ( std::deque<GeoPoint> &vPoints );

  virtual void addPoint2D ( double x, double y );
  virtual void addPoint3D ( double x, double y, double z );
  virtual void addPoint ( GeoPoint &point );

	virtual size_t getPoints2D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP );
  virtual size_t getPoints3D ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
  virtual size_t getPoints2DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP );
  virtual size_t getPoints3DReverse ( std::vector<double> &xCoordsP, std::vector<double> &yCoordsP, std::vector<double> &zCoordsP );
  virtual size_t getPoints ( std::vector<GeoPoint> &vPoints );
  virtual size_t getPoints ( size_t index1, size_t index2, std::vector<GeoPoint> &vPoints, bool extract = false  );
  GeoPoint getPoint (  size_t index );

  void setPoint ( unsigned int index, GeoPoint & point );

  virtual bool checkDoublePoints ( double minDist, bool remove = false );

	virtual std::string getPointsSVG ( double offsetX, double offsetY ) = NULL;
	virtual std::string getPointsReverseSVG ( double offsetX, double offsetY ) = NULL;
  virtual std::string getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode ) = NULL;
  virtual std::string getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode ) = NULL;

	virtual GeoPoint  getAnfangsPunkt ();
	virtual GeoPoint  getEndPunkt();

	virtual void reverse();
	void join ( CurveSegment * pSegP );

	virtual double getAnfangsSteigung2D()=NULL;
	virtual double getEndSteigung2D()=NULL;

	virtual bool getBoundingBox ( double * sw, double * no );
	virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  size_t getCurveSegmentAnz()  {  return 1;  }
  virtual size_t getPointAnz()  {  return points.size();  }

	virtual double getLength() = NULL;
	virtual size_t scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
							std::vector<double> &angleP, double scanDist, bool reverse ) = NULL;

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual void setZKoordinate ( double zP );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );

	virtual void transform ( Matrix4 & matrix ) = NULL;
	virtual void transformInvers ( Matrix4 & matrix ) = NULL;

	void setInterpolation ( std::string interpol )  {  interpolation = interpol;  }
	std::string getInterpolation()  {  return interpolation;  }

};

///////////////////////////////////////////////////////////////////////////////
//  LimeSegment                                                              //
//  Implementierung der Klasse gml:LineSegment							              	 //
///////////////////////////////////////////////////////////////////////////////
class LineSegment: public CurveSegment
{
public:
  enum LINE_SEGMENT_TYPE {  _LINEAR_RING, _LINE_STRING, _LINE_STRING_SEGMENT };

public:
	LineSegment( int dimP, LINE_SEGMENT_TYPE lineSegmentTypeP );
	LineSegment( LineSegment * pLineSegmentOld );
  LineSegment( const LineSegment & lineSegmentOld );
	virtual ~ LineSegment();

protected:
 LINE_SEGMENT_TYPE lineSegmentType;   // Typ des Liniensegments

public:
  virtual GEOMETRY_TYPE getGeometryType()  {  return LINE_STRING_SEGMENT;  }
  virtual GEOMETRY_TYPE getCurveType()     {  return LINE_STRING_SEGMENT;  }
  LINE_SEGMENT_TYPE     getLineSegmentType ()  {  return lineSegmentType; }

	virtual std::string getPointsSVG ( double offsetX, double offsetY );
	virtual std::string getPointsReverseSVG ( double offsetX, double offsetY );
  virtual std::string getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode );
  virtual std::string getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode );

	virtual double getLength();

	virtual double getAnfangsSteigung2D();
	virtual double getEndSteigung2D();

	virtual size_t scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
							std::vector<double> &angleP, double scanDist, bool reverse );

  virtual void transform ( Matrix4 & matrix );
  virtual void transformInvers ( Matrix4 & matrix );

   _Geometrie * extrude ( double r, int anz, bool generateVolume );

   virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
   virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
};

///////////////////////////////////////////////////////////////////////////////
//  ArcSegemnt                                                               //
//  Implementierung der Klasse gml:Arc bzw. gml:Circle		          				 //
///////////////////////////////////////////////////////////////////////////////
class ArcSegment: public CurveSegment
{
public:
  enum ARC_SEGMENT_TYPE {  _ARC_STRING, _ARC, _CIRCLE  };

public:
	ArcSegment(ARC_SEGMENT_TYPE type );
	ArcSegment( ArcSegment * pArcSegmentOld );
	virtual ~ ArcSegment();

protected:
  ARC_SEGMENT_TYPE arcSegmentType;  // Typ des Kreisbogen-Segments

	GeoPoint  * arcCenter;
	double      radius;
	double      startAngle;
	double      endAngle;
	double      gesAngle;
	int         ellipsenbogen;
	int         richtung;
  int         numArc;

public:
  virtual GEOMETRY_TYPE getGeometryType();
	virtual GEOMETRY_TYPE getCurveType();
  ARC_SEGMENT_TYPE getArcSegmentType() {  return arcSegmentType;  }

	virtual void addPoints2D ( std::vector<double> xCoordsP,  std::vector<double> yCoordsP );

	bool calcArcParameters();

	GeoPoint  *  getArcCenter ()  {  return arcCenter;  }
	double getRadius ()  {  return radius;  }

	virtual std::string getPointsSVG ( double offsetX, double offsetY );
	virtual std::string getPointsReverseSVG ( double offsetX, double offsetY );
  virtual std::string getSegmentPointsSVG ( double offsetX, double offsetY, FIRST_POINT mode );
  virtual std::string getSegmentPointsReverseSVG ( double offsetX, double offsetY, FIRST_POINT mode );

	virtual double getLength();

	virtual double getAnfangsSteigung2D();
	virtual double getEndSteigung2D();

  virtual void transform ( Matrix4 & matrix );
  virtual void transformInvers ( Matrix4 & matrix );

	virtual size_t scanCurve2D ( std::vector<double> &xP, std::vector<double> &yP, 
							std::vector<double> &angleP, double scanDist, bool reverse );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );

  void setNumArc ( int anz )  {  numArc = anz;  }
  int getNumArc()  {  return numArc;  }
};

///////////////////////////////////////////////////////////////////////////////
//  _Solid                                                                   //
//  Abstrakte Oberklasse von Solid und CompositeSolid                        //
///////////////////////////////////////////////////////////////////////////////
class _Solid: public _Geometrie
{
public:
  _Solid();
  _Solid ( _Solid * pSolidOld );
  virtual ~ _Solid();

public:
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_SOLID;  }

  virtual size_t getAussenkonturPolygone( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations ) = NULL;
  virtual size_t getInnenkonturPolygone( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations ) = NULL;

  virtual bool getBoundingBox ( double * sw, double * no ) = NULL;
  virtual bool getCenter2D ( double &xP, double &yP ) = NULL;
  virtual bool getCenter3D ( double &xP, double &yP, double &zP ) = NULL;

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport ) = NULL;
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false ) = NULL;
  virtual bool topologyCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false ) = NULL;
  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport ) = NULL;


  virtual double getVolume() = NULL;
  virtual double getOberflaeche() = NULL;
};

///////////////////////////////////////////////////////////////////////////////
//  SolidReference                                                           //
//  Referenz auf einen _Solid                                                //
///////////////////////////////////////////////////////////////////////////////
class SolidReference : public _Solid, public _GeometryReference
{
public:
  SolidReference( Features * pFeaturesP );
  SolidReference ( SolidReference * pSolidReferenceOld );
  virtual ~SolidReference();

protected:

public:
  virtual GEOMETRY_TYPE getGeometryType()  {  return SOLID_REFERENCE;  }

  virtual size_t getAussenkonturPolygone( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );
  virtual size_t getInnenkonturPolygone( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual int getDim();
  virtual std::string  getSrsName ();

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool topologyCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual double getVolume();
  virtual double getOberflaeche();

  virtual bool existReferenceSystem( SrsManager * pSrsManager );
  
  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};

///////////////////////////////////////////////////////////////////////////////
//  Solid				                                												     //
//  Klasse für gml:Solid                                                     //
///////////////////////////////////////////////////////////////////////////////
class Solid: public _Solid
{
public:
  Solid();
  Solid ( Solid * pSolidOld );
  virtual ~ Solid();

protected:
  _Surface             * pAussenkontur;  // Außenfläche des Volumenkörpers
  std::vector<_Surface*> vInnenkonturen; // Innenflächen des Volumenkörpers

public:
  virtual GEOMETRY_TYPE getGeometryType() { return SOLID;  }

  void setAussenkontur ( _Surface * pAussenkonturP );
  _Surface * getAussenkontur()  {  return pAussenkontur;  }

  void addInnenkontur ( _Surface * pInnenkonturP )  {  vInnenkonturen.push_back( pInnenkonturP ); }
  _Surface * getInnenkontur ( unsigned int index );
	size_t     getInnenkonturAnz()  {  return vInnenkonturen.size();  }

  virtual size_t getAussenkonturPolygone( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );
  virtual size_t getInnenkonturPolygone( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool topologyCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  virtual double getVolume();
  static double getVolume ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual double getOberflaeche();

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  void generateTopology( double minDist, Features * pFeaturesP );
  BRep * getTopology();

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};

///////////////////////////////////////////////////////////////////////////////
//  MultiSolid	                                												     //
//  Klasse für gml:MultiSolid und gml:CompositeSolid                         //
///////////////////////////////////////////////////////////////////////////////
class MultiSolid: public _Solid
{
public:
  MultiSolid();
  MultiSolid ( MultiSolid * pSolidOld );
  virtual ~ MultiSolid();

protected:
  std::vector<_Solid*> vSolids;          // Liste des einzelnen Solids
  bool                 isCompositeSolid; // Legt fest, ob es sich um einen gml:CompositeSolid handelt

public:
  virtual GEOMETRY_TYPE getGeometryType() { return MULTI_SOLID;  }
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_MULTI_GEOMETRY;  }

  void addSolid ( _Solid * pSolid )  {  vSolids.push_back( pSolid );  }
  _Solid * getSolid ( size_t index );
	size_t getSolidMemberAnz()  {  return vSolids.size();  }
	size_t getSolids ( std::vector<_Solid*> &vSolids );
  void clearSolids()  {  vSolids.clear();  }

  void setIsCompositeSolid ( bool b )  {  isCompositeSolid = b;  }
  bool getIsCompositeSolid()  {  return isCompositeSolid;  }

  virtual size_t getAussenkonturPolygone( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );
  virtual size_t getInnenkonturPolygone( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );
  virtual bool topologyCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );

  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  virtual double getVolume();
  virtual double getOberflaeche();

  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};

///////////////////////////////////////////////////////////////////////////////
//  Geometrie     	                             												     //
//  Klasse für gml:AbstractGeometry                                          //
///////////////////////////////////////////////////////////////////////////////
class Geometrie: public _Geometrie
{
public:
  Geometrie( int dim );
  Geometrie ( Geometrie * pGeometrieOld );
  virtual ~ Geometrie();

protected:
  _Geometrie * pGeometrie;  // Allgemeines Geometrieobjekt

public:
  virtual GEOMETRY_TYPE getGeometryType() { return ALLG_GEOMETRIE;  }
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType();


  void setGeometrie ( _Geometrie * pGeometrie );
  _Geometrie * getGeometrie ()  {  return pGeometrie;  }

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );


  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual bool existReferenceSystem( SrsManager * pSrsManager );
  
  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};

///////////////////////////////////////////////////////////////////////////////
//  ImplicitGeometry                                                         //
//  Klasse für CityGML core:ImplicitGeometry                                 //
///////////////////////////////////////////////////////////////////////////////
class ImplicitGeometry: public _Geometrie
{
public:
  ImplicitGeometry ( int dim );
  ImplicitGeometry ( ImplicitGeometry * pImplicitGeometryOld );
  virtual ~ ImplicitGeometry();

protected:
  std::string  mimeType;              // Mime-type der externen Prototyp-Geometrie
  std::string  libraryObject;         // Pfad der externen Prototyp-Geometrie
  Matrix4    * pTransformationMatrix; // Transformationsmatrix
  _Geometrie * pRelativeGMLGeometry;  // Im CityGML-File gespeicherte Prototyp-Geometrie
  GeoPoint   * pReferencePoint;       // Referenzpunkt für die Verschiebung der Prototyp-Geometrie

public:
  virtual GEOMETRY_TYPE getGeometryType() { return IMPLICIT_GEOMETRY;  }
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType()  {  return ABSTRACT_IMPLICIT_GEOMETRY;  }

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  void setMimeType ( std::string mimeTypeP )  {  mimeType = mimeTypeP;  }
  std::string getMimeType()  {  return mimeType;  }

  void setLibraryObject ( std::string libraryObjectP )  {  libraryObject = libraryObjectP;  }
  std::string getLibraryObject ()  {  return libraryObject;  }

  void setTransformationMatrix ( Matrix4 * pMatrixP );
  Matrix4 * getTransformationMatrix()  {  return pTransformationMatrix;  }

  void setRelativeGMLGeometry ( _Geometrie * pGeo );
  _Geometrie * getRelativeGMLGeometry()  {  return pRelativeGMLGeometry;  }

  void setReferencePoint ( GeoPoint * pPoint );
  GeoPoint * getReferencePoint()  {  return pReferencePoint;  }

  void getExplicitGeometry ( std::vector<_Curve*> & vCurves, std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );

  virtual bool existReferenceSystem( SrsManager * pSrsManager );
  
  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};

///////////////////////////////////////////////////////////////////////////////
//  ImplicitGeometryReference                                                //
//  Referenz auf eine implizite Geometrie                                    //
///////////////////////////////////////////////////////////////////////////////
class ImplicitGeometryReference : public _Geometrie, public _GeometryReference
{
public:
  ImplicitGeometryReference( Features * pFeaturesP );
  ImplicitGeometryReference ( ImplicitGeometryReference * pGeoReferenceOld );
  virtual ~ImplicitGeometryReference();

protected:

public:
  virtual GEOMETRY_TYPE getGeometryType()  {  return IMPLICIT_GEOMETRY_REFERENCE;  }
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType();

  virtual int getDim();

  virtual std::string  getSrsName ();
  virtual bool existReferenceSystem( SrsManager * pSrsManager );

  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );

  virtual bool checkReferences ( Feature * pObj, std::string propertyName, std::vector<Error*> &vErrorReport );

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual size_t  getPolygonAnz();
  virtual size_t getCurveSegmentAnz();
  virtual size_t getPointAnz();

  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone );
  virtual size_t getPolygone ( std::vector<PolygonGeo*> &vPolygone, std::vector<bool> &vOrientations );

  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};

///////////////////////////////////////////////////////////////////////////////
//  Matrix4                                                                  //
//  Klasse für eine homogene Matrix                                          //
///////////////////////////////////////////////////////////////////////////////
class Matrix4
{
public:
  Matrix4();
  Matrix4 ( double xx, double xy, double xz, double xt,
            double yx, double yy, double yz, double yt,
            double zx, double zy, double zz, double zt );
  Matrix4 ( Matrix4 * pMatrixOld );
  
  virtual ~ Matrix4();

protected:
  double m[3][4];  // Matrix

public:
  void getMatrix  ( double matrix[] )const;

  GeoPoint transform ( GeoPoint & p );
  void transform ( double p[], double pTrans[] );

  GeoPoint transformInvers ( GeoPoint & p );
  void transformInvers ( double p[], double pTrans[] );


  Matrix4 operator* ( Matrix4 & mat );

  Matrix4 invers();

  void createRotationXY ( GeoPoint &p, double angle);
};

///////////////////////////////////////////////////////////////////////////////
//  Direction                                                                //
//  Klasse für Richtungsvektoren                                             //
///////////////////////////////////////////////////////////////////////////////
class Direction
{
  public:
    Direction ();
    Direction ( const double xP, const double yP, const double zP );
    Direction ( const GeoPoint &from, const GeoPoint & to );
    Direction ( const Direction  & dir );

  protected:
    double x, y, z;  // x, y und z-Komponente des Richtungsvektors

  public:
    void get ( double &x, double &y, double &z );
    double getX() {  return x;  }
    double getY() {  return y;  }
    double getZ() {  return z;  }

    double operator[] ( int index );

    double getMaxComponent( int &index );

    Direction & operator= ( const Direction& rhs );

    Direction operator+ ( const Direction &rhs );
    Direction operator- (  const Direction &rhs );
    double    operator* ( const Direction &rhs );
    Direction scale ( double c );

    GeoPoint toPoint();

    void normalize(); 
    double getLength();
    void reverse();
    double getDifference ( Direction& rhs );
    void getAzimutAndInclination ( double & azimut, double & elevation );

    Direction crossProduct ( Direction & dir );
};

///////////////////////////////////////////////////////////////////////////////
//  Plane                                                                    //
//  Klasse für Ebenen                                                        //
///////////////////////////////////////////////////////////////////////////////
class Plane
{
  public:
    Plane ();
    Plane ( const GeoPoint &p, const Direction &n );
    Plane ( const Plane &pl );
    Plane ( Plane *plOld );
    virtual ~Plane();

  protected:
    GeoPoint  point;  // Bezugspunkt der Ebene
    Direction normal; // Normalenvektor der Ebene

  public:
    Direction & getNormal ()  {  return normal;  }
    GeoPoint & getCenter()  {  return point;  }

    void reverse();

    double getDistance ( GeoPoint p );
    double getDistance ();
    void   getDifference ( Plane * pPlane, double &angleDifference, double &pointDifference );

    Matrix4  getTransformationToHorizontal();

    bool cut ( Plane & plane, Line & cutLine, double minimalAngleDistance );
    bool isHorizontal();
    bool isXZPlane();
    bool isYZPlane();
};

///////////////////////////////////////////////////////////////////////////////
//  Line                                                                     //
//  Klasse für Linien                                                        //
///////////////////////////////////////////////////////////////////////////////
class Line
{
  public:
    Line();
    Line ( const GeoPoint & p, const Direction & d );
    Line ( const Line & line );
    virtual ~Line();

  protected:
    GeoPoint  point;  // Bezugspunkt der Linie
    Direction dir;    // Richtungsvektor der Linie

  public:
    void set ( GeoPoint p, Direction d );
    GeoPoint getOrigin()  {  return point; }
    Direction getDirection()  {  return dir;  }

    double getDistance ( GeoPoint p );

    double getPositionOnLine ( GeoPoint p );
    GeoPoint getPointFromParam ( double c );
    void   sortPointsOnLine ( std::vector<GeoPoint> &vPoints, std::map<double,GeoPoint> &mSortedPoints );

};


