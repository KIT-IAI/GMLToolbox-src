#if !defined(COORDINATETRANSFORMATIONS_H__INCLUDED_)
#define COORDINATETRANSFORMATIONS_H__INCLUDED_

#include <math.h>


// Written by Walter Piechulla in April 2000

// Sam Blackburn's prologue about Geodesy Foundation Classes:
// http://ourworld.compuserve.com/homepages/Sam_Blackburn/homepage.htm

// Definitions
// Ellipsoid
//  A flattened sphere. Take a basketball (a sphere), let some air out of it then
//  stand on it. You now have an ellipsoid. The basketball is flat on the bottom
//  and the top but still round in the middle.
// Equator
//  0 degrees Latitude
// Flattening
//  This is a ratio between polar radius and equatorial radius. Other names for
//  flattening are elliptocyte or oblateness.
// Latitude
//  The lines that run around the Earth like belts. They measure up/down angles.
// Longituide
//  The lines that slice the Earth like an orange from top to bottom. They
//  measure left/right angles.
// Meridian
//   One of the imaginary circles on the earth's surface passing through the north
//   and south poles 
// Pi
//  The most famous constant. It is a ratio.
//  It is the Circumference of a circle divided by
//  the diameter of the circle. It is 3.14 (or roughly 22 divivded by 7).
// Prime Meridian 
//  0 degrees Longitude
// Radian
//  The unit of measurement of a circle. It is a ratio.
//  It's kind of hard to explain what
//  a radian is so let me give you an example. Take the basketball and cut
//  a length of string equal to the radius (i.e. half the width) of the
//  ball. Now, lay this string on top of the ball. See how it forms an arc?
//  Now, draw a line from each end of the string to the center of the ball.
//  The angle of these two lines at the center of the ball is roughly
//  57.2958 degrees. This is true no matter what the size of the ball is.
//  Why? Because as the size of the ball increases, so does the radius.
//  So, a radian can be considered as a relationship between the radius of
//  a sphere (ball) and the circumference of the sphere. Now, there's something
//  interesting about the number of degrees in a radian (52.2958). It is
//  equal to 180 divided by Pi. Yup! Go Figure! OK. Now we're getting somewhere.
//  To find the length of the arc when all you have is the number of radians
//  in the angle at the center of the sphere, you multiply the radius times
//  the number of radians. It's that simple (on a perfect sphere anyway).

// Geodetic Datum - description of the shape of the earth

// Reference Ellipsoid - A flattened sphere. Let some air out of
// a basketball, then stand on it. The ball is now an ellipsoid.
// Your feet are standing on the North Pole and the floor is the
// South Pole.

// Ellipsoids are described by their polar and equatorial radii.
// Polar radius is also known as semi-minor axis.
// Equatorial radius is also know as semi-major axis.
// All other items of interest about the ellipsoid are derived
// from these two data.
// Flattening is ( Equatorial radius - Polar radius ) / Equatorial radius
// There's another thing called First Eccentricity Squared, this is computed
// by ( 2 * Flattening ) - ( Flattening squared ).

// Coordinates - a means of expressing a point in space
//
// Cartesian coordinates are X, Y, and Z coordinates. These are always
// expressed in distances from 0, 0, 0 (i,e, the center of the earth).
//
// Polar coordinates are theta (often seen as a letter O with a line
// through its middle running left-to-right), phi (often seen as a
// letter O with a line through its middle running top-to-bottom), and
// r (a distance).
// These are two angles and a distance. The angles are measured from
// a common origin (the center of the earth). Theta is the plane that
// runs through the equator, phi runs through the poles. R is the 
// distance along the line running along the phi angle. For simplicity
// sake, we will refer to theta as Equatorial angle, phi as the
// polar angle, and r as Polar Distance.
//
// Converting coordinates
//
// You can convert from polar to cartesian cordinates using the following
// formula:
// X = Polar distance * cosine of Polar angle * cosine of Equatorial angle
// Y = Polar distance * cosine of Polar angle * sine of Equatorial angle
// Z = Polar distance * sine of Polar angle

// Applying this to the real world
//
// Cartesian coordinates ar commonly refered to an ECEF X,Y,Z coordinates.
// This means Earth Centered, Earth Fixed. The 0,0,0 coordinate is the
// center of the earth. The X axis runs towards the Prime Meridian.
// The Y axis runs towards the equator and the Z axis runs toward
// the poles. Positive Z numbers go towards the North pole while
// negative numbers go towards the South Pole. Positive 

// Computing Distance
//
// If you have two cartesian coordinates, you can compute a line
// of sight (as the bullet flies, aiming a laser, pointing in a straight line)
// by this formula (some guy named Pythagoras figured this out):
// SQRT( ( X1 - X2 ) ^ 2 + ( Y1 - Y2 ) ^ 2 + ( Z1 - Z2 ) ^ 2 )
//
// or in pseudo code:
//
// cartesian_coordinate first_location;
// cartesian_coordinate second_location;
//
// double temp_x;
// double temp_y;
// double temp_z;
//
// temp_x = first_location.X - second_location.X;
// temp_y = first_location.Y - second_location.Y;
// temp_z = first_location.Z - second_location.Z;
//
// temp_x = temp_x * temp_x; // square them
// temp_y = temp_y * temp_y;
// temp_z = temp_z * temp_z;
//
// double temp_double;
//
// temp_double = temp_x + temp_y + temp_z;
//
// double distance;
//
// distance = sqrt( temp_double );
//
// While this will give you distance, it will not give you direction.

// End of Sam Blackburn's prologue


// Remember: North is positive, south is negative
inline double NMEAParserGetDegreesFromNMEALat(const char nOrS,const char lat[10])
{
  char buf[3];
  double result;

  buf[0] = lat[0];
  buf[1] = lat[1];
  buf[2] = '\0';

  result = atof(buf); 

  if (nOrS == 'S') // NMEA uses capital letters
  {
    result = -result;
  }

  return result; 
}

inline double NMEAParserGetMinutesFromNMEALat(const char lat[10])
{
  char buf[8];

  buf[0] = lat[2];
  buf[1] = lat[3];  
  buf[2] = lat[4];
  buf[3] = lat[5];
  buf[4] = lat[6];
  buf[5] = lat[7];
  buf[6] = lat[8];
  buf[7] = '\0';

  return(atof(buf)); 
}

// Remember: West is negative, East is positive
inline double NMEAParserGetDegreesFromNMEALon(const char eOrW,const char lon[11])
{
  char buf[4];
  double result;

  buf[0] = lon[0];
  buf[1] = lon[1];
  buf[2] = lon[2];
  buf[3] = '\0';

  result = atof(buf);

  if (eOrW == 'W')  // NMEA uses capital letters
  {
    result = -result;
  }

  return result; 
}

static inline double NMEAParserGetMinutesFromNMEALon(const char lon[11])
{
  char buf[8];

  buf[0] = lon[3];
  buf[1] = lon[4];
  buf[2] = lon[5];
  buf[3] = lon[6];
  buf[4] = lon[7];
  buf[5] = lon[8];
  buf[6] = lon[9];
  buf[7] = '\0';

  return(atof(buf));
}

// WGS84 to Gauss-Krueger converter (recoding of Pascal sources from Ottmar Labonde)

class WGS84toDHDN
{
protected:
  void HelmertTransformation(double,double,double,double&,double&,double&);
  void BesselBLnachGaussKrueger(double,double,double&,double&);
  void BLRauenberg (double,double,double,double&,double&,double&);
  double neuF(double,double,double,double);
  double round(double);

private:
  double awgs;  // WGS84 Semi-Major Axis = Equatorial Radius in meters
  double bwgs;  // WGS84 Semi-Minor Axis = Polar Radius in meters
  double abes;  // Bessel Semi-Major Axis = Equatorial Radius in meters
  double bbes;  // Bessel Semi-Minor Axis = Polar Radius in meters
  double cbes;  // Bessel ???
  double dx;    // Translation Parameter 1
  double dy;    // Translation Parameter 2
  double dz;    // Translation Parameter 3
  double rotx;  // Rotation Parameter 1
  double roty;  // Rotation Parameter 2
  double rotz;  // Rotation Parameter 3
  double sc;    // Scaling Factor
  double Pi;    // The famous number
  double l1;    // Helper
  double b1;    // Helper
  double l2;    // Helper
  double b2;    // Helper
  double h1;    // Helper
  double h2;    // Helper
  double R;     // Helper
  double H;     // Helper
  double a;     // Helper
  double b;     // Helper
  double eq;    // Helper
  double N;     // Helper
  double Xq;    // Helper
  double Yq;    // Helper
  double Zq;    // Helper
  double X;     // Helper
  double Y;     // Helper
  double Z;     // Helper

public:
  WGS84toDHDN();
  ~WGS84toDHDN();
  void Convert(double,double,double,double&,double&);
};

// Geodesy Foundation Classes (Sam Blackburn)

class CMath
{
  // This class encapsulates all of the math functions. It is here to allow you
  // to replace the C Runtime functions with your home-grown (and maybe better
  // implementation) routines

public:

  static inline double AbsoluteValue( const double& value );
  static inline double ArcCosine( const double& value );
  static inline double ArcSine( const double& value );
  static inline double ArcTangent( const double& value );
  static inline double ArcTangentOfYOverX( const double& y, const double& x );
  static inline double Ceiling( const double& value );
  static inline double ConvertDegreesToRadians( const double& degrees );
  static inline double ConvertRadiansToDegrees( const double& radians );
  static inline void   ConvertDecimalDegreesToDegreesMinutesSeconds( double decimal_degrees, double& degrees, double& minutes, double& seconds );
  // West is negative, East is positive, North is positive, south is negative
  static inline double ConvertDegreesMinutesSecondsCoordinateToDecimalDegrees( double degrees, double minutes, double seconds );
  static inline double Cosine( const double& value );
  static inline double HyperbolicCosine( const double& value );
  static inline double Pi( void );
  static inline double Sine( const double& value );
  static inline double SquareRoot( const double& value );
};

// This is Blackburn's inline functions implementation (original "CMath.inl") 

inline double CMath::AbsoluteValue(const double& value)
{
  return(::fabs(value));
}

inline double CMath::ArcCosine(const double& value)
{
  return(::acos(value));
}

inline double CMath::ArcSine(const double& value)
{
  return(::asin(value));
}

inline double CMath::ArcTangent(const double& value)
{
  return(::atan(value));
}

inline double CMath::ArcTangentOfYOverX( const double& y, const double& x)
{
  return(::atan2(y,x));
}

inline double CMath::Ceiling(const double& value)
{
  return(::ceil(value));
}

inline void CMath::ConvertDecimalDegreesToDegreesMinutesSeconds(double decimal_degrees,
                                                                double& degrees,
                                                                double& minutes,
                                                                double& seconds)
{
  double fractional_part = 0.0;

  double integer_part = 0;

  fractional_part = ::modf(decimal_degrees,&integer_part);

  degrees = integer_part;

  if (decimal_degrees < 0.0)
  {
    fractional_part *= (-1.0);
  }

  minutes = fractional_part * 60.0;

  fractional_part = ::modf(minutes,&integer_part);

  minutes = integer_part;

  seconds = fractional_part * 60.0;
}

inline double CMath::ConvertDegreesMinutesSecondsCoordinateToDecimalDegrees(double degrees,
                                                                            double minutes,
                                                                            double seconds)
{
  double decimal_degrees = 0.0;

  decimal_degrees = degrees;

  if (decimal_degrees < 0.0)
  {
    decimal_degrees *= (-1.0);
  }

  decimal_degrees += (double) (minutes / 60.0);
  decimal_degrees += (double) (seconds / 3600.0);

  if ( degrees < 0.0 )
  {
    decimal_degrees *= (-1.0);
  }

  return(decimal_degrees);
}

inline double CMath::ConvertDegreesToRadians(const double& degrees)
{
  double radians = 0.0;
  double pi_divided_by_180 = CMath::Pi() / 180.0;

  radians = degrees * pi_divided_by_180;

  return( radians );
}

inline double CMath::ConvertRadiansToDegrees(const double& radians)
{
  double degrees = 0.0;

  double conversion_factor = 180.0 / CMath::Pi();

  degrees = radians * conversion_factor;

  return( degrees );
}

inline double CMath::Cosine(const double& value)
{
  return(::cos(value));
}

inline double CMath::HyperbolicCosine(const double& value)
{
  return(::cosh(value));
}

inline double CMath::Pi(void)
{
  return( 3.1415926535897932384626433832795028841971693993751058209749445923078164 );
}

inline double CMath::Sine(const double& value)
{
  return(::sin(value));
}

inline double CMath::SquareRoot(const double& value)
{
  return(::sqrt(value));
}

// End of the inline functions

class CEarthCoordinate
{
  // This is a Cartesian coordinate (Earth Centered, Earth Fixed)

protected:

  double m_X_CoordinateInMeters; // Positive points to intersection of the Prime Meridian and the equator
  double m_Y_CoordinateInMeters; // Positive points to the intersection of 90 degrees east of Prime Meridian and the equator
  double m_Z_CoordinateInMeters; // Positive points to the North Pole, Negative towards the South Pole

public:

  CEarthCoordinate();
  CEarthCoordinate(const CEarthCoordinate& source);
  ~CEarthCoordinate();

  void   Copy(const CEarthCoordinate& source);
  void   Get(double& x_coordinate,double& y_coordinate,double& z_coordinate) const;
  double GetXCoordinateInMeters(void) const;
  double GetYCoordinateInMeters(void) const;
  double GetZCoordinateInMeters(void) const;
  void   Set(double x_coordinate,double y_coordinate,double z_coordinate);
  void   SetXCoordinateInMeters(double x_coordinate);
  void   SetYCoordinateInMeters(double y_coordinate);
  void   SetZCoordinateInMeters(double z_coordinate);

  CEarthCoordinate& operator = (const CEarthCoordinate& source);
};

class CPolarCoordinate
{
protected:

  double m_UpDownAngleInDegrees; // Polar Angle, Phi
  double m_LeftRightAngleInDegrees; // Equatorial Angle, Theta
  double m_DistanceFromSurfaceInMeters;

public:

  CPolarCoordinate();
  CPolarCoordinate(const CPolarCoordinate& source);
  ~CPolarCoordinate();

  void   Copy(const CPolarCoordinate& source);
  void   Get(double& up_down_angle,double& left_right_angle,double& distance_from_surface) const;
  double GetUpDownAngleInDegrees(void) const;
  double GetLeftRightAngleInDegrees(void) const;
  double GetDistanceFromSurfaceInMeters(void) const;
  void   Set(double up_down_angle,double left_right_angle,double distance_from_surface);
  void   SetUpDownAngleInDegrees(double up_down_angle);
  void   SetLeftRightAngleInDegrees(double left_right_angle);
  void   SetDistanceFromSurfaceInMeters(double distance_from_surface);

  CPolarCoordinate& operator = (const CPolarCoordinate& source);
};

class CEarth
{
protected:

  // These are the magic numbers. They are the "real" data. They are facts,
  // measurements. Everything else about an ellipse is derived (or computed from)
  // these two data items.

  double m_PolarRadiusInMeters;
  double m_EquatorialRadiusInMeters;

  // Here be the things that can be derived from our hard data.
  // We compute these values using the two pieces of data that we
  // know about the ellipse.

  double m_Flattening;
  double m_EccentricitySquared;

  // Here's stuff specific to the C++ class

  int m_EllipsoidID;

  void m_ComputeFlattening( void );
  void m_ComputeEccentricitySquared( void );
  void m_Initialize( void );

public:

  enum _EllipsoidType
  {
    Unknown,
    Perfect_Sphere,
    Airy,
    Austrailian_National,
    Bessell_1841,
    Bessel_1841_Nambia,
    Clarke_1866,
    Clarke_1880,
    Everest,
    Fischer_1960_Mercury,
    Fischer_1968,
    GRS_1967,
    GRS_1980,
    Helmert_1906,
    Hough,
    International,
    Krassovsky,
    Modified_Airy,
    Modified_Everest,
    Modified_Fischer_1960,
    South_American_1969,
    Topex_Poseidon_Pathfinder_ITRF,
    WGS_60,
    WGS_66,
    WGS_72,
    WGS_84,
    Custom
  }
  ELLIPSOIDS;

  enum _DatumTypes
  {
    NAD_27 = Clarke_1866,
    Tokyo  = Bessell_1841,
  }
  DATUMS;

  CEarth(int ellipsoid = WGS_84); // Default constructor sets WGS 84 model
  ~CEarth();

  virtual void   AddLineOfSightDistanceAndDirectionToCoordinate(const CPolarCoordinate& point_1,double distance,double direction,CPolarCoordinate& point_2,double height_above_surface_of_point_2 = 0.0);
  virtual void   AddSurfaceDistanceAndDirectionToCoordinate(const CEarthCoordinate& point_1,double distance,double direction,CEarthCoordinate& point_2);
  virtual void   AddSurfaceDistanceAndDirectionToCoordinate(const CEarthCoordinate& point_1,double distance,double direction,CPolarCoordinate& point_2);
  virtual void   AddSurfaceDistanceAndDirectionToCoordinate(const CPolarCoordinate& point_1,double distance,double direction,CEarthCoordinate& point_2);
  virtual void   AddSurfaceDistanceAndDirectionToCoordinate(const CPolarCoordinate& point_1,double distance,double direction,CPolarCoordinate& point_2);
  virtual void   Convert(const CEarthCoordinate& cartesian_coordinate,CPolarCoordinate& polar_coordinate) const;
  virtual void   Convert(const CPolarCoordinate& polar_coordinate,CEarthCoordinate& cartesian_coordinate) const;
  virtual double GetDistanceToHorizon(const CEarthCoordinate& point_1) const;
  virtual double GetDistanceToHorizon(const CPolarCoordinate& point_1) const;
  virtual double GetEquatorialRadiusInMeters(void) const;
  virtual double GetPolarRadiusInMeters(void) const;
  virtual double GetLineOfSightDistanceFromCourse(const CEarthCoordinate& current_location,const CEarthCoordinate& point_a,const CEarthCoordinate& point_b) const;
  virtual double GetLineOfSightDistance(const CEarthCoordinate& point_1,const CEarthCoordinate& point_2) const;
  virtual double GetLineOfSightDistance(const CPolarCoordinate& point_1,const CEarthCoordinate& point_2) const;
  virtual double GetLineOfSightDistance(const CEarthCoordinate& point_1,const CPolarCoordinate& point_2) const;
  virtual double GetLineOfSightDistance(const CPolarCoordinate& point_1,const CPolarCoordinate& point_2) const;
  virtual double GetSurfaceDistance(const CEarthCoordinate& point_1,const CEarthCoordinate& point_2,double * heading_from_point_1_to_point_2 = 0,double * heading_from_point_2_to_point_1 = 0) const;
  virtual double GetSurfaceDistance(const CEarthCoordinate& point_1,const CPolarCoordinate& point_2,double * heading_from_point_1_to_point_2 = 0,double * heading_from_point_2_to_point_1 = 0) const;
  virtual double GetSurfaceDistance(const CPolarCoordinate& point_1,const CEarthCoordinate& point_2,double * heading_from_point_1_to_point_2 = 0,double * heading_from_point_2_to_point_1 = 0) const;
  virtual double GetSurfaceDistance(const CPolarCoordinate& point_1,const CPolarCoordinate& point_2,double * heading_from_point_1_to_point_2 = 0,double * heading_from_point_2_to_point_1 = 0) const;
  virtual void   SetEllipsoid(int ellipsoid);
  virtual void   SetEllipsoidByRadii(double equatorial_radius,double polar_radius);
  virtual void   SetEllipsoidByEquatorialRadiusAndFlattening(double equatorial_radius,double flattening);
};

#endif // COORDINATETRANSFORMATIONS_H__INCLUDED_
