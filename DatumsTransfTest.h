#ifndef __DATUMSTRANSFTEST_H__
#define __DATUMSTRANSFTEST_H__

#include "DatumsTransformations.h"
#include "Geometrie.h"


class TransversMerkator : public Projection
{
  public:
    TransversMerkator();
    TransversMerkator(_ReferenceSystemType refSysType);
    ~TransversMerkator();

  protected:
    virtual GeoPosition DecToGeo(DecartPoint p, ProjectionParams params);
    virtual DecartPoint GeoToDec(GeoPosition p, ProjectionParams params);
};

class UTMDatum : public Datum
{
  public:
    double R;
    double H;
    double h;
    int Zone;

    UTMDatum();
    UTMDatum(double _R, double _H, double _h, int _Zone);
    ~UTMDatum();

  protected:
    virtual void CreateProj();
    virtual void PackProj();
    virtual void UnpackProj();
};

class GKDatum : public Datum
{
  public:
    double R;
    double H;
    double h;
    int    Zone;

    GKDatum();
    GKDatum(double _R, double _H, double _h, int _Zone);
    ~GKDatum();

  protected:
    virtual void CreateProj();
    virtual void PackProj();
    virtual void UnpackProj();
};

class WGSDatum : public Datum
{
  public:
    GeoPosition p;

    WGSDatum();
    WGSDatum(double B, double L, double h);
    ~WGSDatum();

  protected:
    virtual void CreateProj();
    virtual void PackProj();
    virtual void UnpackProj();
};

class SoldnerDatum : public Datum
{
  public:
    double R;
    double H;
    double h;

    SoldnerDatum();
    SoldnerDatum(double _R, double _H, double _h);
    ~SoldnerDatum();

  protected:
    virtual void CreateProj();
    virtual void PackProj();
    virtual void UnpackProj();
};

#endif //__DATUMSTRANSFTEST_H__
