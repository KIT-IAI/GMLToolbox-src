#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <utility>
#include <functional>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <iterator>
#include <stack>
#include "Geometrie.h"
#include "DatumsTransfTest.h"

TransversMerkator::TransversMerkator(): Projection()
{
}

TransversMerkator::TransversMerkator(_ReferenceSystemType refSysType): Projection(refSysType)
{
}

TransversMerkator::~TransversMerkator()
{
}


GeoPosition TransversMerkator::DecToGeo(DecartPoint p, ProjectionParams params)
{
  GeoPosition resPoint;

  double l0 = params.LAxis;
  double b0 = params.BAxis;
  double k0 = params.k;

  double X = (p.X - 500000);
  double Y = p.Y;

  double a = refSystem->aAxis;
  double b = refSystem->bAxis;
  double eq = refSystem->eqRad;

  double M0 = a*((1 - eq/4)*b0 - (3*eq/8 + 3*eq*eq/32)*sin(2*b0));

  double mu = (M0 + Y / k0) / (a*(1 - eq/4 - 3*eq*eq/64 -5*eq*eq*eq/256));
  double e1 = (1-sqrt(1 - eq)) / (1 + sqrt(1 - eq));

  double fi1 = mu + (3*e1/2 - 27*e1*e1*e1/32)*sin(2*mu) +
              (21*e1*e1/16 - 55*e1*e1*e1*e1/32)*sin(4*mu) + 
              (151*e1*e1*e1/96)*sin(6*mu) +
              (1097*e1*e1*e1*e1/512)*sin(8*mu);

  double es = eq/(1 - eq);
  double N1 = a / sqrt(1 - eq*sin(fi1)*sin(fi1));
  double R1 = a*(1 - eq)/sqrt((1 - eq*sin(fi1)*sin(fi1)) * (1 - eq*sin(fi1)*sin(fi1)) * (1 - eq*sin(fi1)*sin(fi1)));
  double T1 = tan(fi1) * tan(fi1);
  double C1 = es*cos(fi1)*cos(fi1);
  double D = X / (N1*k0);

  resPoint.Latitude = fi1 - (N1*tan(fi1)/R1)*(D*D/2 - (5 + 3*T1 + 10*C1 - 4*C1*C1 - 9*es)*D*D*D*D/24 +
                      (61 + 90*T1 + 298*C1 + 45*T1*T1 - 252*es - 3*C1*C1)*D*D*D*D*D*D/720);

  resPoint.Longitude = l0 + (D - (1 + 2*T1 + C1)*D*D*D/6 + (5 - 2*C1 + 28*T1 - 3*C1*C1 +
                       8*es + 24*T1*T1)*D*D*D*D*D/120)/cos(fi1);

  return resPoint;
}

DecartPoint TransversMerkator::GeoToDec(GeoPosition p, ProjectionParams params)
{
  DecartPoint resDec;
  double B = p.Latitude;
  double L = p.Longitude;

  double k0 = params.k;
  double B0 = params.BAxis;
  double L0 = params.LAxis;

  double a = refSystem->aAxis;
  double b = refSystem->bAxis;
  double eq = refSystem->eqRad;

  double es = eq/(1 - eq);
  double N = a / sqrt(1 - eq*sin(B)*sin(B));
  double T = tan(B) * tan(B);
  double C = es*cos(B)*cos(B);
  double A = (L - L0)*cos(B);

  double M = a*((1 - eq/4 - 3*eq*eq/64 - 5*eq*eq*eq/256)*B -
            (3*eq/8 + 3*eq*eq/32 + 45*eq*eq*eq/1024)*sin(2*B) +
            (15*eq*eq/256 + 45*eq*eq*eq/1024)*sin(4*B) -
            (35*eq*eq*eq/3072)*sin(6*B));

  double M0 = a*((1 - eq/4 - 3*eq*eq/64 - 5*eq*eq*eq/256)*B0 -
             (3*eq/8 + 3*eq*eq/32 + 45*eq*eq*eq/1024)*sin(2*B0) +
             (15*eq*eq/256 + 45*eq*eq*eq/1024)*sin(4*B0) -
             (35*eq*eq*eq/3072)*sin(6*B0));

  resDec.X = k0*N*(A + (1 - T + C)*((A*A*A)/6) + (5 - 18*T + T*T + 72*C - 58*es)*((A*A*A*A*A)/120));
  resDec.Y = k0*(M - M0 + N*tan(B)*((A*A)/2 + (5 - T + 9*C + 4*C*C)*((A*A*A*A)/24) + (61 - 58*T + T*T + 600*C - 330*es)*((A*A*A*A*A*A)/720)));

  resDec.X += 500000;

  return resDec;
}


/*************************************************************************
** UTM
*************************************************************************/
UTMDatum::UTMDatum() : Datum()
{
  DatumType = UTM;
  R = 0.0;
  H = 0.0;
  h = 0.0;
  Zone = 0;
}

UTMDatum::UTMDatum(double _R, double _H, double _h, int _Zone)
{
  DatumType = UTM;
  R = _R;
  H = _H;
  h = _h;
  Zone = _Zone;
}

UTMDatum::~UTMDatum()
{
}

void UTMDatum::CreateProj()
{
  projection = dynamic_cast<Projection*>(new TransversMerkator());
}

void UTMDatum::PackProj()
{
  ProjectionParams proj_params;
  DecartPoint p;

  p.X = R;
  p.Y = H;
  p.Z = 0;

  proj_params.LAxis = (Zone * 6 - 3) - 180;

  /*if (proj_params.LAxis < 0)
    proj_params.LAxis = 360 - proj_params.LAxis;*/

  proj_params.LAxis = (proj_params.LAxis * Pi) / 180;
  proj_params.BAxis = 0;
  proj_params.k = 0.9996;

  projection->setGeo(projection->DecToGeo(p, proj_params));
}

void UTMDatum::UnpackProj()
{
  ProjectionParams proj_params;
  int              z;

  GeoPosition g = projection->getGeo();

  proj_params.k = 0.9996;

  if ( Zone == 0 )
    z = int(ceill(((g.Longitude * 180) / Pi) / 6) + 30);
  else
    z = Zone;

  proj_params.LAxis = (((z * 6 - 3) - 180) * Pi) / 180;

  DecartPoint dec = projection->GeoToDec(g, proj_params);
  R = dec.X;
  H = dec.Y;
  Zone = z;
}

/*************************************************************************
** Gauß-Krüger
*************************************************************************/
GKDatum::GKDatum() : Datum()
{
  DatumType = Gauss_Krueger;
  R = 0.0;
  H = 0.0;
  h = 0.0;
  Zone = 0;
}

GKDatum::GKDatum(double _R, double _H, double _h, int _Zone)
{
  DatumType = Gauss_Krueger;
  R = _R;
  H = _H;
  h = _h;
  Zone = _Zone;
}

GKDatum::~GKDatum()
{
}

void GKDatum::CreateProj()
{
  projection = dynamic_cast<Projection*>(new TransversMerkator(Bessell_1841));
}

void GKDatum::PackProj()
{
  ProjectionParams proj_params;
  DecartPoint p;

  p.X = R;
  p.Y = H;
  p.Z = 0;

  proj_params.LAxis = (Zone * 3);

  if (proj_params.LAxis < 0)
    proj_params.LAxis = 360 - proj_params.LAxis;

  proj_params.LAxis = (proj_params.LAxis * Pi) / 180;
  proj_params.BAxis = 0;
  proj_params.k = 1;

  projection->setGeo(projection->DecToGeo(p, proj_params));
}

void GKDatum::UnpackProj()
{
  ProjectionParams proj_params;
  int              z;

  GeoPosition g = projection->getGeo();

  g.Latitude = (g.Latitude * 180) / Pi;
  g.Longitude = (g.Longitude * 180) / Pi;

  proj_params.k = 1;

  if ( Zone == 0 )
  {
    z = int(ceill(g.Longitude / 3));

    if (g.Longitude < (3 * z - 1.5))
      z -= 1;
  }
  else
    z = Zone;

  proj_params.LAxis = (z * 3);

  if (proj_params.LAxis < 0)
    proj_params.LAxis = 360 - proj_params.LAxis;

  proj_params.LAxis = (proj_params.LAxis * Pi) / 180;

  g.Latitude = (g.Latitude * Pi) / 180;
  g.Longitude = (g.Longitude * Pi) / 180;

  DecartPoint dec = projection->GeoToDec(g, proj_params);
  R = dec.X;
  H = dec.Y;
  Zone = z;
}


/*************************************************************************
** WGS
*************************************************************************/
WGSDatum::WGSDatum() : Datum()
{
  DatumType   = Geographical;
  p.Longitude = 0.0;
  p.Latitude  = 0.0;
}

WGSDatum::WGSDatum(double B, double L, double h)
{
  DatumType   = Geographical;
  p.Longitude = L;
  p.Latitude  = B;
}

WGSDatum::~WGSDatum()
{
}

void WGSDatum::CreateProj()
{
  projection = new Projection();
}

void WGSDatum::PackProj()
{
  p.Latitude  = (p.Latitude * Pi) / 180;
  p.Longitude = (p.Longitude * Pi) / 180;
  //p.h = h;
  projection->setGeo(p);
}

void WGSDatum::UnpackProj()
{
  p.Longitude = (projection->getGeo().Longitude * 180) / Pi;
  p.Latitude  = (projection->getGeo().Latitude * 180) / Pi;
  p.h = projection->getGeo().h;
}


/*************************************************************************
** Soldner
*************************************************************************/
SoldnerDatum::SoldnerDatum() : Datum()
{
  DatumType   = Soldner;
  R = 0.0;
  H = 0.0;
  h = 0.0;
}

SoldnerDatum::SoldnerDatum(double _R, double _H, double _h)
{
  DatumType   = Soldner;
  R = _R;
  H = _H;
  h = _h;
}

SoldnerDatum::~SoldnerDatum()
{
}

void SoldnerDatum::CreateProj()
{
  projection = new Projection(PARAMETER_BESSEL_SOLDNER_BERLIN);
}

void SoldnerDatum::PackProj()
{
  ProjectionParams proj_params;
  DecartPoint p;

  p.X = R;
  p.Y = H;
  p.Z = 0;

  proj_params.LAxis = (33 * 6 - 3) - 180;
  proj_params.LAxis = (proj_params.LAxis * Pi) / 180;
  proj_params.BAxis = 0;
  proj_params.k = 0.9996;

  projection->setGeo(projection->DecToGeo(p, proj_params));
/*
  double a, b, n, y, G, N, B, L, L0, B0, B0rad, Bf, Bf0, t, eta2, e2;

  // Referenzpunkt Müggelberg
  //L0 = 13.6254292633; ??
  //B0 = 52.4172549880; ??
  L0 = 13.6272036666667;
  B0 = 52.4186482777778;
  B0rad = B0 * Pi / 180;

  a = projection->getReferenceSystem()->aAxis;
  b = projection->getReferenceSystem()->bAxis;
  e2 = projection->getReferenceSystem()->eqRad;
  y = R;

  n = (a - b) / (a + b);
  t = tan(B0rad);
  eta2 = e2/(1-e2)*cos(B0rad)*cos(B0rad);
  N = a / (sqrt(1 - (e2*sin(B0rad)*sin(B0rad))));

  // Meridianbogenlänge
  G = (a/(1 + n)) * ((1 + (n * n) / 4) * B0 - 3 / 2 * (n - (n * n * n) / 8) * sin(2 * B0rad) + 15 / 16 * n * n * sin(4 * B0rad) - 35 / 48 * n * n * n * sin(6 * B0rad));

  // Fußpunktsbreite
  Bf0 = G * (1 + n) / (a * (1 + n * n / 4 + n * n * n * n / 64));
  Bf = Bf0 + 3/2 * (n - 9/16 * n * n * n) * sin(2 * B0rad) + 21/16 * n * n * sin(4 * B0rad) + 151/96 * n * n * n * sin(6 * B0rad);

  B = Bf - (1 + (e2 / (1 - e2)) * t * cos(B0rad) * cos(B0rad) * (((y * y)/(2 * N * N)) - (1 / 24) * (1 + 3 * t * t + eta2 - 9 * t * t * eta2) * ((y * y * y * y) / (N * N * N * N))));
  L = L0 + (1 / cos(B0rad))*(y / N) - (1 / (3 * cos(B0rad)) * t * t * ((y * y * y) / (N * N)) + (1 / (15 * cos(B0rad))) * t * t * (1 + 3 * t * t) * ((y * y * y * y * y) / (N * N * N * N * N)));

  UnpackProj();
*/
}

void SoldnerDatum::UnpackProj()
{
  ProjectionParams proj_params;
  GeoPosition g = projection->getGeo();

  DecartPoint dec = projection->GeoToDec(g, proj_params);
  R = dec.X;
  H = dec.Y;
/*
  double L, L0, B, Brad, B0, B0rad;
  double a, b, n, y0, G, G0, x0, m0, N, l, t, eta2, e2;

  B = 0.0;
  L = 0.0;
  x0 = 0.0;
  y0 = 0.0;

  // Referenzpunkt Müggelberg
  L0 = 13.6272036666667;
  B0 = 52.4186482777778;
  B0rad = B0 * Pi / 180;
  Brad = B * Pi / 180;

  a = projection->getReferenceSystem()->aAxis;
  b = projection->getReferenceSystem()->bAxis;
  e2 = projection->getReferenceSystem()->eqRad;

  m0 = 1.0;
  l = L - L0;
  n = (a - b) / (a + b);
  t = tan(Brad);
  eta2 = e2/(1-e2)*cos(Brad)*cos(Brad);
  N = a / (sqrt(1 - (e2*sin(Brad)*sin(Brad))));

  // Meridianbogenlänge
  G = (a/(1 + n)) * ((1 + (n * n) / 4) * B - 3 / 2 * (n - (n * n * n) / 8) * sin(2 * Brad) + 15 / 16 * n * n * sin(4 * Brad) - 35 / 48 * n * n * n * sin(6 * Brad));
  G0 = (a/(1 + n)) * ((1 + (n * n) / 4) * B0 - 3 / 2 * (n - (n * n * n) / 8) * sin(2 * B0rad) + 15 / 16 * n * n * sin(4 * B0rad) - 35 / 48 * n * n * n * sin(6 * B0rad));

  H = G - G0 + x0 + m0 * ((N / 2) * sin(Brad) * cos(Brad) + (N / 24) * (5 - t * t - 5 * eta2) * l * l * l * l * sin(Brad) * cos(Brad) * cos(Brad) * cos(Brad));
  R = y0 + m0 * (N * l * cos(Brad) - (N / 6) * l * l * l * sin(Brad) * sin(Brad) * cos(Brad) - (N / 120) * (8 - t * t) * l * l * l * l * l * sin(B) * cos(Brad) * cos(Brad) * cos(Brad));
*/
}

