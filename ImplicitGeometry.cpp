///////////////////////////////////////////////////////////////////////////////
//  ImplicitGeometry                                                         //
//  Klasse für core:ImplicitGeometry                                         //
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

ABSTRACT_GEOMETRY_TYPE ImplicitGeometry::getAbstractGeometryType()
{
  return ABSTRACT_IMPLICIT_GEOMETRY;
}


///////////////////////////////////////////////////////////////////////////////
//  Berechnet die BoundingBox                                                //
//  Noch zu tun                                                              //
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
//  Berechnet den Schwerpunkt                                                //
//  Noch zu tun                                                              //
///////////////////////////////////////////////////////////////////////////////
bool ImplicitGeometry::getCenter2D ( double &xP, double &yP )
{
  return false;
}

bool ImplicitGeometry::getCenter3D ( double &xP, double &yP, double &zP )
{
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Polygone                                          //
///////////////////////////////////////////////////////////////////////////////
int ImplicitGeometry::getPolygonAnz()
{
  if (  pRelativeGMLGeometry != NULL )
    return pRelativeGMLGeometry->getPolygonAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der CurveSegments                                     //
///////////////////////////////////////////////////////////////////////////////
int ImplicitGeometry::getCurveSegmentAnz()
{
  if (  pRelativeGMLGeometry != NULL )
    return pRelativeGMLGeometry->getCurveSegmentAnz();
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte                                            //
///////////////////////////////////////////////////////////////////////////////
int ImplicitGeometry::getPointAnz()
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
//  Überprüft die Geometrie                                                  //
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
//  Offset                                                                   //
///////////////////////////////////////////////////////////////////////////////
void ImplicitGeometry::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  if ( pRelativeGMLGeometry != NULL && sGeometries.count( pRelativeGMLGeometry ) == 0 )
    pRelativeGMLGeometry->offset( point, sGeometries );

  sGeometries.insert( this );
}
