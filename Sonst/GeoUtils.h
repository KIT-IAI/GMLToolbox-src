/***************************************************************************
* 
* Part of generic Geometry-Library
*
* @author Andreas Geiger, www.iai.fzk.de
* @date 2008-03-25
****************************************************************************/

#ifndef GEO_UTILS_h
#define GEO_UTILS_h

class GeoPoint;

class Geo_Utils
{
  public:
    static const double inverseFlattening_WGS84;
    static const double semiMajorAxis_WGS84;
    static const double semiMinorAxis_WGS84;

  public:
    static double calcDistanceOnSphere(const GeoPoint& posA, const GeoPoint& posB);
    static double calcDistanceOnWGS84Ellipsoid(const GeoPoint& posA, const GeoPoint& posB);
    static void   convertDecimal2DMS(double dec, int &deg, int &min, double &sec);
    static double convertDMS2Decimal(int deg, int min, double sec);
    static double convertDMS2Decimal(int deg, int min, int sec, int microSec);
    static std::string convertDMS2DMSString(int deg, int min, int sec, int microSec, bool isLongitude);
    static double convertDMS2Decimal(std::string dmsString);
    static std::string convertDecimal2DMSString(double decimal, bool isLongitude);
    static void convertMetric2Geographic(GeoPoint& pnt, const GeoPoint& geographicRefPoint);
};

#endif // ifndef GEO_UTILS_h
