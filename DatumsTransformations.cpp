#include "stdafx.h"
#include "DatumsTransformations.h"
#include <iostream>

void _HelmertTransformParam::Inverse()
{
  dX = (-1)*dX;
  dY = (-1)*dY;
  dZ = (-1)*dZ;

  rotX = (-1)*rotX;
  rotY = (-1)*rotY;
  rotZ = (-1)*rotZ;
}

ReferenceSystem::ReferenceSystem()
{
  aAxis = 0.00;
  bAxis = 0.00;
  eqRad = 0.00;
  RSType = UNKNOWN_REFSYS;
}

ReferenceSystem::~ReferenceSystem()
{
}

ReferenceSystem::ReferenceSystem(_ReferenceSystemType _refSystem)
{
  switch(_refSystem)
  {
    case Bessell_1841:
    case PARAMETER_BESSEL_SOLDNER_BERLIN:
    case PARAMETER_BESSEL_UTM_HAMBURG:
      aAxis = 6377397.155;
      bAxis = 6356078.96282;
      break;

    case Krassowski_1940:
      aAxis = 6378245.0;
      bAxis = 6378245.0;
      break;

    case WGS_84:
      aAxis = 6378137.0;
      bAxis = 6356752.31425;
      break;

    case GRS_80:
      aAxis = 6378137.0;
      bAxis = 6356752.31414;
      break;
  }

  eqRad = (aAxis*aAxis - bAxis*bAxis)/(aAxis*aAxis);
  RSType = _refSystem;

  Pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164;
}


bool ReferenceSystem::getHelmertTransformParams(std::pair<_ReferenceSystemType, _ReferenceSystemType> tSystems, _HelmertTransformParam &params)
{
  if ((tSystems.first == WGS_84) && (tSystems.second == Bessell_1841))
  {
    params.dX   = -585.7;
    params.dY   = -87.0;
    params.dZ   = -409.2;
    params.rotX = 2.540423689E-6;
    params.rotY = 7.514612057E-7;
    params.rotZ = -1.368144208E-5;
    params.sc   = 1/0.99999122;
  }
  else if ((tSystems.first == WGS_84) && (tSystems.second == PARAMETER_BESSEL_UTM_HAMBURG))
  {
    //params.dX   = 0.0;
    //params.dY   = 0.0;
    //params.dZ   = 0.0;
    //params.rotX = 0.0;
    //params.rotY = 0.0;
    //params.rotZ = 0.0;
    //params.sc   = 0.0;
  }
  else if ((tSystems.first == WGS_84) && (tSystems.second == PARAMETER_BESSEL_SOLDNER_BERLIN))
  {
    params.dX   = 675.239155;
    params.dY   = 25.303490;
    params.dZ   = 422.544682;
    params.rotX = -0.717994;
    params.rotY = -1.766241;
    params.rotZ = -0.719541;
    params.sc   = -0.245916;
  }
  else if ((tSystems.first == WGS_84) && (tSystems.second == Krassowski_1940))
  {
    //params.dX   = -24.9;
    //params.dY   = 126.4;
    //params.dZ   = 93.2;
    //params.rotX = 0.0;
    //params.rotY = 0.0;
    //params.rotZ = 0.0;
    //params.sc   = 0.0;
  }
  else
    return false;

  return true;
}

void ReferenceSystem::HelmertTransform(DecartPoint sDec, DecartPoint &tDec, _HelmertTransformParam params)
{
  tDec.X = params.dX + (params.sc * (sDec.X + params.rotZ * sDec.Y - params.rotY * sDec.Z));
  tDec.Y = params.dY + (params.sc * (-params.rotZ * sDec.X + sDec.Y + params.rotX * sDec.Z));
  tDec.Z = params.dZ + (params.sc * (params.rotY * sDec.X - params.rotX * sDec.Y + sDec.Z));
}

GeoPosition ReferenceSystem::getGeo()
{
  return geo;
}

DecartPoint ReferenceSystem::getDecFromGeo()
{
  DecartPoint p;

  double N = aAxis / sqrt(1 - eqRad*sin(geo.Latitude)*sin(geo.Latitude));

  p.X = (N + geo.h)*cos(geo.Latitude)*cos(geo.Longitude);
  p.Y = (N + geo.h)*cos(geo.Latitude)*sin(geo.Longitude);
  p.Z = (N*(1 - eqRad) + geo.h)*sin(geo.Latitude);

  return p;
}

void ReferenceSystem::setGeo(GeoPosition p)
{
  geo = p;
}

void ReferenceSystem::setGeo(DecartPoint p)
{
  double s = sqrt(p.X * p.X + p.Y * p.Y);
  double N;
  double f = (45 * Pi) / 180;
  double f1 = 0;
  double f2 = 0;

  do
  {
    N = aAxis / sqrt(1 - eqRad * sin(f) * sin(f));
    f1 = atan(p.Z / (s * (1 - (eqRad * N) / (s / cos(f)))));
    f2 = f;
    f = f1;
  }
  while(!(fabs(f2 - f1) < 10E-10));

  geo.Latitude = f;
  geo.Longitude = atan(p.Y / p.X);
  geo.h = s / cos(f1) - N;
}

void ReferenceSystem::setGeo(ReferenceSystem* rs)
{
  if (rs->RSType == RSType)
    geo = rs->geo;
  else
  {
    _HelmertTransformParam params;
    DecartPoint dec;

    if (getHelmertTransformParams(std::pair<_ReferenceSystemType, _ReferenceSystemType> (rs->RSType, RSType), params))
    {
      HelmertTransform(rs->getDecFromGeo(), dec, params);
      setGeo(dec);
    }
    else if (getHelmertTransformParams(std::pair<_ReferenceSystemType, _ReferenceSystemType> (RSType, rs->RSType), params))
    {
      params.Inverse();
      HelmertTransform(rs->getDecFromGeo(), dec, params);
      setGeo(dec);
    }
    else
      geo = rs->geo;
  }
}


Projection::Projection()
{
  refSystem = new ReferenceSystem(WGS_84);
}

Projection::Projection(_ReferenceSystemType refSysType)
{
  refSystem = new ReferenceSystem(refSysType);
}

Projection::~Projection()
{
  delete(refSystem);
}


void Projection::ConvertProj(Projection* target)
{
  target->refSystem->setGeo(refSystem);
}


GeoPosition Projection::getGeo()
{
  return refSystem->getGeo();
}

void Projection::setGeo(GeoPosition p)
{
  return refSystem->setGeo(p);
}



Datum::Datum()
: projection(NULL)
{
  DatumType = UNKNOWN_COORDSYS;
  Pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164;
}

Datum::~Datum()
{
  if (projection)
    delete(projection);
}

void Datum::Convert(Datum* target)
{
  CreateProj();
  target->CreateProj();
  PackProj();
  projection->ConvertProj(target->projection);
  target->UnpackProj();
}

