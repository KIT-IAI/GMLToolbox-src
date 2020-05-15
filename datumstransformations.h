#ifndef __DATUMSTRANSFORMATIONS_H__
#define __DATUMSTRANSFORMATIONS_H__

#include <math.h>
#include <map>

enum _ReferenceSystemType
{
  Bessell_1841,
  Krassowski_1940,
  WGS_84,
  GRS_80,
  PARAMETER_BESSEL_UTM_HAMBURG,
  PARAMETER_BESSEL_SOLDNER_BERLIN,
  UNKNOWN_REFSYS
};

enum _DatumType
{
  Gauss_Krueger,
  Geographical,
  UTM,
  Soldner,
  UNKNOWN_COORDSYS
};

struct GeoPosition
{
  double Longitude;
  double Latitude;
  double h;

  GeoPosition()
  {
    Longitude = 0.00;
    Latitude = 0.00;
    h = 0.00;
  };

  void GetLongInDegSecMin(double &deg, double &min, double &sec)
  {
    GetDegMinSecFromDec(Longitude, deg, min, sec);
  }

  void GetLatInDegSecMin(double &deg, double &min, double &sec)
  {
    GetDegMinSecFromDec(Latitude, deg, min, sec);
  }

  void SetLongFromDegMinSec(double deg, double min, double sec)
  {
    Longitude = GetDecFromDegMinSec(deg, min, sec);
  }

  void SetLatFromDegMinSec(double deg, double min, double sec)
  {
    Latitude = GetDecFromDegMinSec(deg, min, sec);
  }

private:
  void GetDegMinSecFromDec(double dec, double &deg, double &min, double &sec)
  {
    double fractional_part = 0.0;

    double integer_part = 0;

    fractional_part = modf(dec,&integer_part);

    deg = integer_part;

    if (dec < 0.0)
    {
      fractional_part *= (-1.0);
    }

    min = fractional_part * 60.0;

    fractional_part = ::modf(min,&integer_part);

    min = integer_part;

    sec = fractional_part * 60.0;
  }

  double GetDecFromDegMinSec(double deg, double min, double sec)
  {
    double decimal_degrees = 0.0;

    decimal_degrees = deg;

    if (decimal_degrees < 0.0)
    {
      decimal_degrees *= (-1.0);
    }

    decimal_degrees += (double) (min / 60.0);
    decimal_degrees += (double) (sec / 3600.0);

    if ( deg < 0.0 )
    {
      decimal_degrees *= (-1.0);
    }

    return(decimal_degrees);
  }
};

struct DecartPoint
{
  double X;
  double Y;
  double Z;

  DecartPoint()
  {
    X = 0;
    Y = 0;
    Z = 0;
  };
};

struct ProjectionParams
{
  double k;
  double BAxis;
  double LAxis;

  ProjectionParams()
  {
    k = 0;
    BAxis = 0;
    LAxis = 0;
  }
};

struct _HelmertTransformParam
{
  double dX;
  double dY;
  double dZ;
  double rotX;
  double rotY;
  double rotZ;
  double sc;

  void Inverse();
};

// Base class for all reference systems

class ReferenceSystem
{
  public:
    double eqRad;
    double aAxis;
    double bAxis;
    _ReferenceSystemType RSType;

    GeoPosition getGeo();

    void setGeo(GeoPosition p);
    void setGeo(DecartPoint p);
    void setGeo(ReferenceSystem* rs);

    ReferenceSystem();
    ReferenceSystem(_ReferenceSystemType _refSystem);

    ~ReferenceSystem();

  protected:
    bool getHelmertTransformParams(std::pair<_ReferenceSystemType, _ReferenceSystemType> tSystems, _HelmertTransformParam &params);
    void HelmertTransform(DecartPoint sGeo, DecartPoint &tGeo, _HelmertTransformParam params);
    DecartPoint getDecFromGeo();

    double Pi;
    GeoPosition geo;
};


//Base class for all projections

class Projection
{
  public:
    GeoPosition getGeo();
    void setGeo(GeoPosition p);
    ReferenceSystem* getReferenceSystem() { return refSystem; }

    void ConvertProj(Projection* target);

    virtual DecartPoint GeoToDec(GeoPosition p, ProjectionParams params){return DecartPoint();};
    virtual GeoPosition DecToGeo(DecartPoint p, ProjectionParams params){return GeoPosition();};

    Projection();
    Projection(_ReferenceSystemType refSysType);
    ~Projection();

  protected:
    ReferenceSystem* refSystem;
};


// Base class for all coordinate system
class Datum
{
  public:
    _DatumType DatumType;

    void Convert(Datum* target);

    Datum();
    ~Datum();

  protected:
    Projection* projection;
    double Pi;

    virtual void CreateProj(){};
    virtual void PackProj(){};
    virtual void UnpackProj(){};
};

#endif //__DATUMSTRANSFORMATIONS_H__
