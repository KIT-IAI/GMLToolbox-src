///////////////////////////////////////////////////////////////////////////////
//  ImplicitGeometry                                                         //
//  Klasse für core:ImplicitGeometry                                         //
///////////////////////////////////////////////////////////////////////////////
class ImplicitGeometry: public _Geometrie
{
public:
  ImplicitGeometry ( int dim );
  ImplicitGeometry ( ImplicitGeometry * pImplicitGeometryOld );
  virtual ~ ImplicitGeometry();

protected:
  std::string  mimeType;
  std::string  libraryObject;
  Matrix4    * pTransformationMatrix;
  _Geometrie * pRelativeGMLGeometry;
  GeoPoint   * pReferencePoint;

public:
  virtual GEOMETRY_TYPE getGeometryType() { return IMPLICIT_GEOMETRY;  }
  virtual ABSTRACT_GEOMETRY_TYPE getAbstractGeometryType();

  virtual bool getBoundingBox ( double * sw, double * no );
  virtual bool getCenter2D ( double &xP, double &yP );
  virtual bool getCenter3D ( double &xP, double &yP, double &zP );
  virtual int getPolygonAnz();
  virtual int getCurveSegmentAnz();
  virtual int getPointAnz();

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

//  virtual bool check ( Feature * pObj, double minDist, std::vector<Error*> &vErrorReport );
  virtual bool basicCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, std::vector<Error*> &vErrorReport );
  virtual bool advancedCheck ( Feature * pObj, std::string featureProperty, CheckingParams * pCheckParams, 
          std::vector<Error*> &vErrorReport, bool checkAnyway = false );

  virtual bool existReferenceSystem( SrsManager * pSrsManager );
  
  virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );
};
