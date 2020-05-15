#include "StdAfx.h"
#include <math.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

#include "constant.h"
#include "GeoUtils.h"
#include "Geometrie.h"

using namespace std;

const double Geo_Utils::inverseFlattening_WGS84 = 1/298.257223563;
const double Geo_Utils::semiMajorAxis_WGS84     = 6378137.0;
const double Geo_Utils::semiMinorAxis_WGS84     = 6356752.31425;

double Geo_Utils::calcDistanceOnSphere(const GeoPoint& posA, const GeoPoint& posB)
{
  // lamda = longitude = x
  // phi   = latitude  = y

  double lambdaA = PI / 180 * posA.x;
  double phiA    = PI / 180 * posA.y;
  double lambdaB = PI / 180 * posB.x;
  double phiB    = PI / 180 * posB.y;
  double zeta    = acos(sin(phiA) * sin(phiB) + cos(phiA) * cos(phiB) * cos(lambdaB - lambdaA));
  double L = zeta * semiMajorAxis_WGS84;

  return L;
}

double Geo_Utils::calcDistanceOnWGS84Ellipsoid(const GeoPoint& posA, const GeoPoint& posB)
{
  // lamda = longitude = x
  // phi   = latitude  = y

  double F = PI / 180 * (posA.y + posB.y) / 2;
  double G = PI / 180 * (posA.y - posB.y) / 2;
  double l = PI / 180 * (posA.x - posB.x) / 2;

  double S = sin(G) * sin(G) * cos(l) * cos(l) +
             cos(F) * cos(F) * sin(l) * sin(l);
  double C = cos(G) * cos(G) * cos(l) * cos(l) +
             sin(F) * sin(F) * sin(l) * sin(l);

  double w = atan(sqrt(S / C));
  double D = 2 * w * semiMajorAxis_WGS84;

  double R = sqrt(S * C) / w;
  double H1 = (3 * R - 1) / (2 * C);
  double H2 = (3 * R + 1) / (2 * S);

  double s = D * (1 + inverseFlattening_WGS84 * H1 * sin(F) * sin(F) * cos(G) * cos(G)
                    - inverseFlattening_WGS84 * H2 * cos(F) * cos(F) * sin(G) * sin(G));

  return s;
}

void Geo_Utils::convertDecimal2DMS(double dec, int &deg, int &min, double &sec)
{
  double fractional_part, integer_part;

  fractional_part = ::modf(dec, &integer_part);

  deg = (int)integer_part;

  if (dec < 0.0)
  {
    fractional_part *= (-1.0);
  }

  fractional_part *= 60.0;

  fractional_part = ::modf(fractional_part, &integer_part);

  min = (int)integer_part;

  sec = fractional_part * 60.0;
}

double Geo_Utils::convertDMS2Decimal(int deg, int min, double sec)
{
  double decimal_degrees = 0.0;

  decimal_degrees = (double)deg;

  if (decimal_degrees < 0.0)
  {
    decimal_degrees *= (-1.0);
  }

  decimal_degrees += ((double)min / 60.0);
  decimal_degrees += (sec / 3600.0);

  if ( deg < 0.0 )
  {
    decimal_degrees *= (-1.0);
  }

  return decimal_degrees;
}

double Geo_Utils::convertDMS2Decimal(int deg, int min, int sec, int microSec)
{
  double decimal_degrees, decimal_sec;

  decimal_degrees = deg;

  if (decimal_degrees < 0.0)
  {
    decimal_degrees *= (-1.0);
  }

  decimal_degrees += ((double)min / 60.0);

  TCHAR buffer[20];
  _stprintf_s(buffer, _T("%d.%d"), sec, microSec);
  decimal_sec = _tstof(buffer);
  decimal_degrees += (decimal_sec / 3600.0);

  if ( deg < 0.0 )
  {
    decimal_degrees *= (-1.0);
  }

  return decimal_degrees;
}

std::string Geo_Utils::convertDMS2DMSString(int deg, int min, int sec, int microSec, bool isLongitude)
{
  std::string dmsString;
  char heading;

  if (deg < 0)
  {
    deg *= -1;
    heading = isLongitude ? 'W' : 'S';
  }
  else
  {
    heading = isLongitude ? 'E' : 'N';
  }

  TCHAR szMicSec[10];
  _stprintf_s(szMicSec, _T("%d"), microSec*1000);
  _tcsncpy_s(szMicSec, szMicSec, 4);

  TCHAR buffer[64];

  if(microSec == 0)
  {
    _stprintf_s(buffer, _T("%c %d° %d' %d\""), heading, deg, min, sec);
  }
  else
  {
    _stprintf_s(buffer, _T("%c %d° %d' %d.%s\""), heading, deg, min, sec, szMicSec);
  }

  return buffer;
}

double Geo_Utils::convertDMS2Decimal(std::string dmsString)
{
  int pos1, pos2, pos3;
  int deg, min;
  double sec;

  pos1 = dmsString.find(_T("°"));
  pos2 = dmsString.find(_T("'"));
  pos3 = dmsString.find(_T("\""));

  if(dmsString.find_first_of(_T("NESW")) == 0)
  {
    deg = _ttol(dmsString.substr(1, pos1).c_str());
  }
  else
  {
    deg = _ttol(dmsString.substr(pos1).c_str());
  }

  min = _ttol(dmsString.substr(pos1 + 1, pos2 - pos1).c_str());
  sec = _tstof(dmsString.substr(pos2 + 1, pos3 - pos2).c_str());

  return (deg + ((double)min / 60) + sec / 3600);
}

std::string Geo_Utils::convertDecimal2DMSString(double decimal, bool isLongitude)
{
  int deg, min;
  double sec, fractional_part, integer_part;
  char heading;

  fractional_part = ::modf(decimal, &integer_part);

  deg = (int)integer_part;

  if (decimal < 0.0)
  {
    fractional_part *= (-1.0);
  }

  fractional_part *= 60.0;

  fractional_part = ::modf(fractional_part, &integer_part);

  min = (int)integer_part;

  sec = fractional_part * 60.0;

  if (deg < 0)
  {
    deg *= -1;
    heading = isLongitude ? 'W' : 'S';
  }
  else
  {
    heading = isLongitude ? 'E' : 'N';
  }

  TCHAR buffer[64];
  _stprintf_s(buffer, _T("%c %d° %d' %.4f\""), heading, deg, min, sec);

  return buffer;
}

//
// Converts a local coordinate to an geographic coordinate based on the geographic reference point
// Has to be in meter!
//
void Geo_Utils::convertMetric2Geographic(GeoPoint& pnt, const GeoPoint& geographicRefPoint)
{
  double halbmesserRed = cos(geographicRefPoint.y * deg2rad) * Geo_Utils::semiMajorAxis_WGS84;
  double deltaX = atan(1 / halbmesserRed) * rad2deg;
  double deltaY = atan(1 / Geo_Utils::semiMajorAxis_WGS84) * rad2deg;

  pnt.x = geographicRefPoint.x + pnt.x * deltaX;
  pnt.y = geographicRefPoint.y  + pnt.y * deltaY;
}
