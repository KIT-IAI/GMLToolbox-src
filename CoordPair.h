/* Datei CoordPair.h

  Diese Datei enthält die Definition der Hilfsklasse CoordPair:

  - struct CoordPair

	sowie einiger Operatoren für diese Klasse

*/

#ifndef COORDPAIR_H
#define COORDPAIR_H

#pragma warning(disable: 4786)
#pragma warning(disable: 4800)

#include <cmath>
#include "constant.h"

struct CoordPair
{
	double x;  // x-Koordinate
	double y;  // y-Koordinate
	double z;  // z-Koordinate
	double m;  // m-Koordinate

	CoordPair (void) : x (0.0), y (0.0), z (0.0), m (0.0) {}
	CoordPair (double X, double Y) : x (X), y (Y), z (0.0), m (0.0) {}
	CoordPair (double X, double Y, double Z, double M) : x (X), y (Y), z (Z), m (M) {}
	CoordPair (const CoordPair& oldCP) : x (oldCP.x), y (oldCP.y),  z (oldCP.z),  m (oldCP.m) {}
	virtual ~CoordPair () {}

	CoordPair operator= (const CoordPair& rhs)
	{
		x = rhs.x; 
		y = rhs.y; 
		z = rhs.z; 
		m = rhs.m; 
		
		return *this;
	}

	CoordPair operator+= (const CoordPair& rhs)
	{
		x += rhs.x; 
		y += rhs.y; 
		z += rhs.z; 
		
		return *this;
	}

	CoordPair operator-= (const CoordPair& rhs)
	{
		x -= rhs.x; 
		y -= rhs.y; 
		z -= rhs.z; 
		
		return *this;
	}

	CoordPair operator*= (const double& fac)
	{
		x *= fac; 
		y *= fac; 
		z *= fac; 
		
		return *this;
	}

	CoordPair normalized (void) const
	{ 
		double betr = sqrt (x * x + y * y + z * z);
		return CoordPair (x / betr, y / betr, z / betr, m);
	}

	CoordPair scaledBy (const CoordPair& Scales)
	{
		return CoordPair (Scales.x * x, Scales.y * y, Scales.z * z, m );
	}

	double length (void) const
	{
		return sqrt (x * x + y * y + z * z);
	}
};

inline
CoordPair operator+ (const CoordPair& CP1, const CoordPair& CP2)
{ return CoordPair (CP1.x + CP2.x, CP1.y + CP2.y, CP1.z + CP2.z, CP1.m + CP2.m); }

inline
CoordPair operator- (const CoordPair& CP1, const CoordPair& CP2)
{ return CoordPair (CP1.x - CP2.x, CP1.y - CP2.y,  CP1.z - CP2.z,  CP1.m - CP2.m); }

inline
CoordPair operator* (const double& fac, const CoordPair& CP)
{ return CoordPair (fac * CP.x, fac * CP.y, fac * CP.z, fac * CP.m); }

inline
CoordPair operator* (const CoordPair& CP, const double& fac)
{ return CoordPair (fac * CP.x, fac * CP.y, fac * CP.z, fac * CP.m); }

inline
double operator* (const CoordPair& CP1, const CoordPair& CP2)
{ return (CP1.x * CP2.x + CP1.y * CP2.y, CP1.z * CP2.z); }

inline
bool operator== (const CoordPair& CP1, const CoordPair& CP2)
{ return ((fabs (CP2.x - CP1.x) < MINIMAL_POINT_DISTANCE) &&
		  (fabs (CP2.y - CP1.y) < MINIMAL_POINT_DISTANCE) &&
		  (fabs (CP2.z - CP1.z) < MINIMAL_POINT_DISTANCE) &&
		  (fabs (CP2.m - CP1.m) < MINIMAL_POINT_DISTANCE)			);
}

inline
bool operator!= (const CoordPair& CP1, const CoordPair& CP2)
{ return (!(CP1 == CP2)); }


struct PLVertex : CoordPair
{
	double bulge;

	PLVertex (void) : CoordPair (), bulge (0.0) {}
	PLVertex (double X, double Y, double B = 0.0) : CoordPair (X, Y), bulge (B) {}
	PLVertex (const PLVertex& oldPLV) : CoordPair (oldPLV), bulge (oldPLV.bulge) {}
	explicit PLVertex (const CoordPair& oldCP, double B = 0.0)
		: CoordPair (oldCP), bulge (B) {}
};

#endif
