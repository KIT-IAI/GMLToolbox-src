#pragma once

#include <gps/gps.hpp>

class GeoPoint;
class SrsManager;


///////////////////////////////////////////////////////////////////////////////
//  SrsProjection                                                            //
//  Durchführung von Koordinatensystem-Transformationen mit Hilfe der        //
//  Eye4Software                                                             //
///////////////////////////////////////////////////////////////////////////////
public ref class SrsProjection
{
  public:
    SrsProjection( SrsManager * pSrsManagerP );
    ~SrsProjection();

  protected:
    gps::Projection     * pProjection;
    gps::GridParameters * pSrcGrid;
    gps::GridParameters * pDestGrid;
    bool                            initialized;
    SrsManager                    * pSrsManager;

  public:
    //static void CheckCoordinateTransformModule();
    bool isInitialized()  {  return initialized;  }

    bool convertSrs2LatLon ( std::string srsNameShort, GeoPoint * pnt, GeoPoint * pntLatLon );

    bool convertLatLon2Srs ( std::string srsNameShort, GeoPoint * pntLatLon, GeoPoint * pnt );
    bool convertLatLon2Srs ( int srsCode, GeoPoint * pntLatLon, GeoPoint * pnt );

    bool convertSrs2Srs ( std::string srsNameShortInput, std::string srsNameShortOutput, GeoPoint * p );
};
