#include "StdAfx.h"
#include "CoordinateTransformations.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// These are the Geodesy Fundation Classes (Sam Blackburn)
CEarthCoordinate::CEarthCoordinate(void)
{
  m_X_CoordinateInMeters = 0.0;
  m_Y_CoordinateInMeters = 0.0;
  m_Z_CoordinateInMeters = 0.0;
}

CEarthCoordinate::CEarthCoordinate(const CEarthCoordinate& source)
{
  Copy(source);
}

CEarthCoordinate::~CEarthCoordinate(void)
{
  m_X_CoordinateInMeters = 0.0;
  m_Y_CoordinateInMeters = 0.0;
  m_Z_CoordinateInMeters = 0.0;
}

void CEarthCoordinate::Copy(const CEarthCoordinate& source)
{
  m_X_CoordinateInMeters = source.m_X_CoordinateInMeters;
  m_Y_CoordinateInMeters = source.m_Y_CoordinateInMeters;
  m_Z_CoordinateInMeters = source.m_Z_CoordinateInMeters;
}

void CEarthCoordinate::Get(double& x_coordinate,double& y_coordinate,double& z_coordinate) const
{
  x_coordinate = m_X_CoordinateInMeters;
  y_coordinate = m_Y_CoordinateInMeters;
  z_coordinate = m_Z_CoordinateInMeters;
}

double CEarthCoordinate::GetXCoordinateInMeters(void) const
{
  return( m_X_CoordinateInMeters );
}

double CEarthCoordinate::GetYCoordinateInMeters(void) const
{
  return( m_Y_CoordinateInMeters );
}

double CEarthCoordinate::GetZCoordinateInMeters(void) const
{
  return( m_Z_CoordinateInMeters );
}

void CEarthCoordinate::Set(double x_coordinate,double y_coordinate,double z_coordinate)
{
  m_X_CoordinateInMeters = x_coordinate;
  m_Y_CoordinateInMeters = y_coordinate;
  m_Z_CoordinateInMeters = z_coordinate;
}

void CEarthCoordinate::SetXCoordinateInMeters(double x_coordinate)
{
  m_X_CoordinateInMeters = x_coordinate;
}

void CEarthCoordinate::SetYCoordinateInMeters(double y_coordinate)
{
  m_Y_CoordinateInMeters = y_coordinate;
}

void CEarthCoordinate::SetZCoordinateInMeters(double z_coordinate)
{
  m_Z_CoordinateInMeters = z_coordinate;
}

CEarthCoordinate& CEarthCoordinate::operator=(const CEarthCoordinate& source)
{
  Copy(source);
  return(*this);
}

CPolarCoordinate::CPolarCoordinate(void)
{
  m_UpDownAngleInDegrees        = 0.0;
  m_LeftRightAngleInDegrees     = 0.0;
  m_DistanceFromSurfaceInMeters = 0.0;
}

CPolarCoordinate::CPolarCoordinate(const CPolarCoordinate& source)
{
  Copy(source);
}

CPolarCoordinate::~CPolarCoordinate(void)
{
  m_UpDownAngleInDegrees        = 0.0;
  m_LeftRightAngleInDegrees     = 0.0;
  m_DistanceFromSurfaceInMeters = 0.0;
}

void CPolarCoordinate::Copy(const CPolarCoordinate& source)
{
  m_UpDownAngleInDegrees        = source.m_UpDownAngleInDegrees;
  m_LeftRightAngleInDegrees     = source.m_LeftRightAngleInDegrees;
  m_DistanceFromSurfaceInMeters = source.m_DistanceFromSurfaceInMeters;
}

void CPolarCoordinate::Get(double& up_down_angle,double& left_right_angle,double& length) const
{
  up_down_angle    = m_UpDownAngleInDegrees;
  left_right_angle = m_LeftRightAngleInDegrees;
  length           = m_DistanceFromSurfaceInMeters;
}

double CPolarCoordinate::GetUpDownAngleInDegrees(void) const
{
  return(m_UpDownAngleInDegrees);
}

double CPolarCoordinate::GetLeftRightAngleInDegrees(void) const
{
  return(m_LeftRightAngleInDegrees);
}

double CPolarCoordinate::GetDistanceFromSurfaceInMeters(void) const
{
  return(m_DistanceFromSurfaceInMeters);
}

void CPolarCoordinate::Set(double up_down_angle,double left_right_angle,double length)
{
  m_UpDownAngleInDegrees        = up_down_angle;
  m_LeftRightAngleInDegrees     = left_right_angle;
  m_DistanceFromSurfaceInMeters = length;
}

void CPolarCoordinate::SetUpDownAngleInDegrees(double up_down_angle)
{
  m_UpDownAngleInDegrees = up_down_angle;
}

void CPolarCoordinate::SetLeftRightAngleInDegrees(double left_right_angle)
{
  m_LeftRightAngleInDegrees = left_right_angle;
}

void CPolarCoordinate::SetDistanceFromSurfaceInMeters(double distance_from_surface)
{
  m_DistanceFromSurfaceInMeters = distance_from_surface;
}

CPolarCoordinate& CPolarCoordinate::operator=(const CPolarCoordinate& source)
{
  Copy(source);
  return(*this);
}

CEarth::CEarth(int ellipsoid_identifier)
{
  m_Initialize();
  SetEllipsoid(ellipsoid_identifier);
}

CEarth::~CEarth()
{
  m_Initialize();
}

void CEarth::AddLineOfSightDistanceAndDirectionToCoordinate(const CPolarCoordinate& point_1,
                                                            double distance,
                                                            double direction,
                                                            CPolarCoordinate& point_2,
                                                            double height_above_surface_of_point_2)
{
  // The method used here is to convert the straight (line-of-sight) distance to
  // a surface distance and then find out the position using the surface distance.
  // This is a translation of the MMDIST routine found in the FORWRD3D program at
  // ftp://ftp.ngs.noaa.gov/pub/pcsoft/for_inv.3d/source/forwrd3d.for

  double c                                           = 0.0;
  double cosine_of_latitude_squared                  = 0.0;
  double cosine_of_direction_squared                 = 0.0;
  double cosine_of_point_1_latitude                  = 0.0;
  double difference_in_height                        = 0.0;
  double direction_in_radians                        = 0.0;
  double distance_adjusted_for_differences_in_height = 0.0;
  double height_above_surface_of_point_1             = 0.0;
  double n                                           = 0.0;
  double point_1_latitude_in_radians                 = 0.0;
  double polar_eccentricity_squared                  = 0.0;
  double polar_flattening                            = 0.0;
  double r                                           = 0.0;
  double sine_of_point_1_latitude                    = 0.0;
  double surface_distance                            = 0.0;
  double term_1                                      = 0.0;
  double term_2                                      = 0.0;
  double term_3                                      = 0.0;
  double two_r                                       = 0.0;
  double x                                           = 0.0;

  // Many thanks to Peter Dana (pdana@mail.utexas.edu) for educating me
  // on the finer points of Geodesy, one of which was how to compute
  // "second eccentricity squared"

  polar_eccentricity_squared = ((m_EquatorialRadiusInMeters * m_EquatorialRadiusInMeters) - (m_PolarRadiusInMeters * m_PolarRadiusInMeters )) / ( m_PolarRadiusInMeters * m_PolarRadiusInMeters);

  //printf( "polar_eccentricity_squared is %.23lf\n", polar_eccentricity_squared );

  point_1_latitude_in_radians = CMath::ConvertDegreesToRadians(point_1.GetUpDownAngleInDegrees());
  direction_in_radians        = CMath::ConvertDegreesToRadians(direction);

  cosine_of_point_1_latitude = CMath::Cosine(point_1_latitude_in_radians);
  cosine_of_latitude_squared = cosine_of_point_1_latitude * cosine_of_point_1_latitude;

  cosine_of_direction_squared = CMath::Cosine(direction_in_radians) * CMath::Cosine(direction_in_radians);

  c = (m_EquatorialRadiusInMeters * m_EquatorialRadiusInMeters) / m_PolarRadiusInMeters;

  n = c / CMath::SquareRoot(1.0 + (polar_eccentricity_squared * cosine_of_latitude_squared));

  r = n / (1.0 + (polar_eccentricity_squared * cosine_of_latitude_squared * cosine_of_direction_squared));

  height_above_surface_of_point_1 = point_1.GetDistanceFromSurfaceInMeters();

  difference_in_height = height_above_surface_of_point_2 - height_above_surface_of_point_1;

  term_1 = (distance * distance) - (difference_in_height * difference_in_height);
  term_2 = 1.0 + (height_above_surface_of_point_1 / r);
  term_3 = 1.0 + (height_above_surface_of_point_2 / r);
  distance_adjusted_for_differences_in_height = CMath::SquareRoot(term_1 / (term_2 * term_3));

  // printf( "distance_adjusted_for_differences_in_height is %.11lf\n", distance_adjusted_for_differences_in_height );

  two_r = 2.0 * r;

  surface_distance = two_r * CMath::ArcSine(distance_adjusted_for_differences_in_height / two_r);

  // printf( "surface_distance is %.11lf\n", surface_distance );

  AddSurfaceDistanceAndDirectionToCoordinate(point_1,surface_distance,direction,point_2);
}

void CEarth::AddSurfaceDistanceAndDirectionToCoordinate(const CEarthCoordinate& point_1,
                                                        double distance, 
                                                        double direction, 
                                                        CPolarCoordinate& point_2)
{
  CPolarCoordinate polar_point_1;

  Convert(point_1, polar_point_1);

  AddSurfaceDistanceAndDirectionToCoordinate(polar_point_1,distance,direction,point_2);
}

void CEarth::AddSurfaceDistanceAndDirectionToCoordinate(const CEarthCoordinate& point_1, 
                                                        double distance, 
                                                        double direction, 
                                                        CEarthCoordinate& point_2)
{
  CPolarCoordinate polar_point_1;
  CPolarCoordinate polar_point_2;

  Convert(point_1,polar_point_1);

  AddSurfaceDistanceAndDirectionToCoordinate(polar_point_1,distance,direction,polar_point_2);

  Convert(polar_point_2,point_2);
}

void CEarth::AddSurfaceDistanceAndDirectionToCoordinate(const CPolarCoordinate& point_1, 
                                                        double distance, 
                                                        double direction, 
                                                        CEarthCoordinate& point_2)
{
  CPolarCoordinate polar_coordinate;

  AddSurfaceDistanceAndDirectionToCoordinate(point_1,distance,direction,polar_coordinate);

  Convert(polar_coordinate,point_2);
}

void CEarth::AddSurfaceDistanceAndDirectionToCoordinate(const CPolarCoordinate& point_1, 
                                                        double distance, 
                                                        double direction, 
                                                        CPolarCoordinate& point_2)
{
  // This is a translation of the Fortran routine DIRCT1 found in the
  // FORWRD3D program at:
  // ftp://ftp.ngs.noaa.gov/pub/pcsoft/for_inv.3d/source/forwrd3d.for

  double c                                          = 0.0;
  double c2a                                        = 0.0;
  double cosine_of_direction                        = 0.0;
  double cosine_of_y                                = 0.0;
  double cu                                         = 0.0;
  double cz                                         = 0.0;
  double d                                          = 0.0;
  double e                                          = 0.0;
  double direction_in_radians                       = 0.0;
  double eps                                        = 0.0;
  double heading_from_point_2_to_point_1_in_radians = 0.0;
  double point_1_latitude_in_radians                = 0.0;
  double point_1_longitude_in_radians               = 0.0;
  double point_2_latitude_in_radians                = 0.0;
  double point_2_longitude_in_radians               = 0.0;
  double r                                          = 0.0;
  double sa                                         = 0.0;
  double sine_of_direction                          = 0.0;
  double sine_of_y                                  = 0.0;
  double su                                         = 0.0;
  double tangent_u                                  = 0.0;
  double term_1                                     = 0.0;
  double term_2                                     = 0.0;
  double term_3                                     = 0.0;
  double x                                          = 0.0;
  double y                                          = 0.0;

  direction_in_radians = CMath::ConvertDegreesToRadians(direction);

  eps = 0.000000000000005;

  r = 1.0 - m_Flattening;

  point_1_latitude_in_radians  = CMath::ConvertDegreesToRadians(point_1.GetUpDownAngleInDegrees());
  point_1_longitude_in_radians = CMath::ConvertDegreesToRadians(point_1.GetLeftRightAngleInDegrees());

  tangent_u = (r * CMath::Sine(point_1_latitude_in_radians)) / CMath::Cosine(point_1_latitude_in_radians);

  sine_of_direction = CMath::Sine(direction_in_radians);

  cosine_of_direction = CMath::Cosine(direction_in_radians);

  heading_from_point_2_to_point_1_in_radians = 0.0;

  if (cosine_of_direction != 0.0)
  {
    heading_from_point_2_to_point_1_in_radians = CMath::ArcTangentOfYOverX(tangent_u, cosine_of_direction) * 2.0;
  }

  cu = 1.0 / CMath::SquareRoot((tangent_u * tangent_u) + 1.0);
  su = tangent_u * cu;
  sa = cu * sine_of_direction;
  c2a = ((-sa) * sa) + 1.0;
  x = CMath::SquareRoot((((1.0 / r / r) - 1.0) * c2a) + 1.0) + 1.0;
  x = (x - 2.0) / x;
  c = 1.0 - x;
  c = (((x * x) / 4.0) + 1.0) / c;
  d = ((0.375 * (x * x)) -1.0) * x;

  tangent_u = distance / r / m_EquatorialRadiusInMeters / c;

  y = tangent_u;

  bool exit_loop = false;

  while(exit_loop != true)
  {
    sine_of_y = CMath::Sine(y);
    cosine_of_y = CMath::Cosine(y);
    cz = CMath::Cosine(heading_from_point_2_to_point_1_in_radians + y);
    e = (cz * cz * 2.0) - 1.0;
    c = y;
    x = e * cosine_of_y;
    y = (e + e) - 1.0;

    term_1 = (sine_of_y * sine_of_y * 4.0) - 3.0;
    term_2 = ((term_1 * y * cz * d) / 6.0) + x;
    term_3 = ((term_2 * d) / 4.0) - cz;
    y = (term_3 * sine_of_y * d) + tangent_u;

    if (CMath::AbsoluteValue(y - c) > eps)
    {
      exit_loop = false;
    }
    else
    {
      exit_loop = true;
    }
  }

  heading_from_point_2_to_point_1_in_radians = (cu * cosine_of_y * cosine_of_direction) - (su * sine_of_y);

  c = r * CMath::SquareRoot((sa * sa) + (heading_from_point_2_to_point_1_in_radians * heading_from_point_2_to_point_1_in_radians));
  d = (su * cosine_of_y) + (cu * sine_of_y * cosine_of_direction);

  point_2_latitude_in_radians = CMath::ArcTangentOfYOverX(d,c);

  c = (cu * cosine_of_y) - (su * sine_of_y * cosine_of_direction);
  x = CMath::ArcTangentOfYOverX(sine_of_y * sine_of_direction,c);
  c = (((((-3.0 * c2a) + 4.0) * m_Flattening) + 4.0) * c2a * m_Flattening) / 16.0;
  d = ((((e * cosine_of_y * c) + cz) * sine_of_y * c) + y) * sa;

  point_2_longitude_in_radians = (point_1_longitude_in_radians + x) - ((1.0 - c) * d * m_Flattening);

  heading_from_point_2_to_point_1_in_radians = CMath::ArcTangentOfYOverX(sa,heading_from_point_2_to_point_1_in_radians) + CMath::Pi();

  point_2.SetUpDownAngleInDegrees(CMath::ConvertRadiansToDegrees(point_2_latitude_in_radians));
  point_2.SetLeftRightAngleInDegrees(CMath::ConvertRadiansToDegrees(point_2_longitude_in_radians));
}

void CEarth::Convert(const CEarthCoordinate& cartesian_coordinate,
                     CPolarCoordinate& polar_coordinate) const
{
  // convert from cartesian to polar

  double equatorial_radius_times_eccentricity_squared = 0.0;

  equatorial_radius_times_eccentricity_squared = m_EquatorialRadiusInMeters * m_EccentricitySquared;

  double p = 0.0;

  p = CMath::SquareRoot((cartesian_coordinate.GetXCoordinateInMeters() * cartesian_coordinate.GetXCoordinateInMeters()) + 
    (cartesian_coordinate.GetYCoordinateInMeters() * cartesian_coordinate.GetYCoordinateInMeters()));

  double temp_latitude = 0.0;
  double z_coordinate  = cartesian_coordinate.GetZCoordinateInMeters(); // for convienance
  double one_minus_eccentricity_squared = 1.0 - m_EccentricitySquared;

  temp_latitude = z_coordinate / p / one_minus_eccentricity_squared;

  double old_value  = 0.0;
  double temp_value = 0.0;
  double part_a     = 0.0;
  double part_b     = 0.0;
  double part_c     = 0.0;

  unsigned long loop_index              = 0;
  unsigned long maximum_number_of_tries = 1024;

  bool convergence_was_acheived = false;

  while(convergence_was_acheived != true && loop_index < maximum_number_of_tries)
  {
    old_value = temp_latitude;

    part_a = one_minus_eccentricity_squared * temp_latitude * temp_latitude;
    part_b = equatorial_radius_times_eccentricity_squared / CMath::SquareRoot(1.0 + part_a);
    part_c = p - part_b;

    temp_latitude = z_coordinate / part_c;

    loop_index++;

    if (CMath::AbsoluteValue(temp_latitude - old_value) > 0.000000000000000000001)
    {
      // Oh well, try again...
    }
    else
    {
      // YIPEE!! We've reached convergence!
      convergence_was_acheived = true;
    }
  }

  if (convergence_was_acheived == true)
  {
    double latitude_angle_in_radians = 0.0;

    // Save the UpDown angle in degrees

    latitude_angle_in_radians = CMath::ArcTangent(temp_latitude);

    polar_coordinate.SetUpDownAngleInDegrees(CMath::ConvertRadiansToDegrees(latitude_angle_in_radians)); // Latitude

    double sine_of_latitude_in_radians   = 0.0;
    double cosine_of_latitude_in_radians = 0.0;

    sine_of_latitude_in_radians   = CMath::Sine(latitude_angle_in_radians);
    cosine_of_latitude_in_radians = CMath::Cosine(latitude_angle_in_radians);

    double longitude_in_radians = 0.0;

    longitude_in_radians = CMath::ArcTangentOfYOverX(cartesian_coordinate.GetYCoordinateInMeters(),cartesian_coordinate.GetXCoordinateInMeters());

    polar_coordinate.SetLeftRightAngleInDegrees(CMath::ConvertRadiansToDegrees(longitude_in_radians)); // Longitude

    double w = 0.0;

    w = CMath::SquareRoot(1.0 - (m_EccentricitySquared * sine_of_latitude_in_radians * sine_of_latitude_in_radians));

    double distance_from_center_to_surface_of_the_ellipsoid = 0.0;

    distance_from_center_to_surface_of_the_ellipsoid = m_EquatorialRadiusInMeters / w;

    double distance_from_surface = 0.0;

    if (CMath::AbsoluteValue(latitude_angle_in_radians) < 0.7854)
    {
      //printf( "fabs( %14.10lf ) < 0.7854\n", latitude_angle_in_radians );
      distance_from_surface = (p / cosine_of_latitude_in_radians) - distance_from_center_to_surface_of_the_ellipsoid;
    }
    else
    {
      //printf( "fabs( %14.10lf ) >= 0.7854\n", latitude_angle_in_radians );
      distance_from_surface = (z_coordinate / sine_of_latitude_in_radians) 
        - distance_from_center_to_surface_of_the_ellipsoid 
        + (m_EccentricitySquared * distance_from_center_to_surface_of_the_ellipsoid);
    }

    // printf( "CEarth::Convert() : First method produced a length of %14.10lf\n", distance_from_surface );

    polar_coordinate.SetDistanceFromSurfaceInMeters(distance_from_surface);
  }
  else
  {
    // Oh well, we gave it a shot..
    polar_coordinate.Set( 0.0, 0.0, 0.0 );
  }
}

void CEarth::Convert(const CPolarCoordinate& polar_coordinate,
                     CEarthCoordinate& cartesian_coordinate) const
{
  // convert from polar to cartesian

  double up_down_radians    = 0.0; // latitude
  double left_right_radians = 0.0; // longitude angle

  up_down_radians    = CMath::ConvertDegreesToRadians(polar_coordinate.GetUpDownAngleInDegrees()); 
  left_right_radians = CMath::ConvertDegreesToRadians(polar_coordinate.GetLeftRightAngleInDegrees());

  double sine_of_up_down_radians      = 0.0;
  double cosine_of_left_right_radians = 0.0; // cosine_of_longitude
  double cosine_of_up_down_radians    = 0.0; // cosine_of_latitude

  sine_of_up_down_radians      = CMath::Sine(up_down_radians);
  cosine_of_left_right_radians = CMath::Cosine(left_right_radians);
  cosine_of_up_down_radians    = CMath::Cosine(up_down_radians);

  // Now we need to calculate the distance from the center of the ellipsoid to the surface of the ellipsoid
  double w = 0.0;

  w = CMath::SquareRoot(1.0 - (m_EccentricitySquared * sine_of_up_down_radians * sine_of_up_down_radians));

  double distance_from_center_to_surface_of_the_ellipsoid = 0.0;

  distance_from_center_to_surface_of_the_ellipsoid = m_EquatorialRadiusInMeters / w;

  // printf( "en = %.25lf\n", distance_from_center_to_surface_of_the_ellipsoid );

  double coordinate = 0.0;

  coordinate = (distance_from_center_to_surface_of_the_ellipsoid + polar_coordinate.GetDistanceFromSurfaceInMeters()) 
    * cosine_of_up_down_radians * cosine_of_left_right_radians;

  cartesian_coordinate.SetXCoordinateInMeters(coordinate);

  coordinate = (distance_from_center_to_surface_of_the_ellipsoid + polar_coordinate.GetDistanceFromSurfaceInMeters()) 
    * cosine_of_up_down_radians * CMath::Sine(left_right_radians);

  cartesian_coordinate.SetYCoordinateInMeters(coordinate);

  coordinate = (distance_from_center_to_surface_of_the_ellipsoid * (1.0 - m_EccentricitySquared) 
    + polar_coordinate.GetDistanceFromSurfaceInMeters()) * sine_of_up_down_radians;

  cartesian_coordinate.SetZCoordinateInMeters(coordinate);
}

double CEarth::GetDistanceToHorizon(const CEarthCoordinate& point_1) const
{
  CPolarCoordinate polar_coordinate;

  Convert(point_1,polar_coordinate);

  return(GetDistanceToHorizon(polar_coordinate));
}

double CEarth::GetDistanceToHorizon(const CPolarCoordinate& point_1) const
{
  double distance_to_horizon = 0.0;

  // d = ::sqrt( feet ) *  1.144 for nmi
  // optical horizon is 1.317 * sqrt( h );
  // d= ::sqrt( 17 * height_in_meters ); d is in meters

  distance_to_horizon = CMath::SquareRoot(17.0 * point_1.GetDistanceFromSurfaceInMeters());

  return(distance_to_horizon);
}

double CEarth::GetEquatorialRadiusInMeters(void) const
{
  return(m_EquatorialRadiusInMeters);
}

double CEarth::GetPolarRadiusInMeters(void) const
{
  return(m_PolarRadiusInMeters);
}

double CEarth::GetLineOfSightDistanceFromCourse(const CEarthCoordinate& current_location, 
                                                const CEarthCoordinate& point_a, 
                                                const CEarthCoordinate& point_b) const
{
  // This function tells you how far off course you are from a straight line between
  // point_a and point_b.

  /*
  ** References:
  ** I got the formula from:
  ** Engineering Mathematics Handbook
  ** Jan J. Tuma, Ph.D.
  ** McGraw-Hill Book Company
  ** 1970
  ** Library of Congress Catalog Number 78-101174
  ** page 19, (a) Oblique triangle
  **
  ** Teach Yourself Trigonometry
  ** P. Abbott, B.A.
  ** English Universities Press Ltd.
  ** 102 Newgate Street
  ** London, E.C.I
  ** Originally published 1940
  ** I used the 1964 printing.
  ** Page 22, Figure 12 calls this "the altitude from the vertex A"
  */ 

  double distance_from_current_location_to_point_a = 0.0;
  double distance_from_current_location_to_point_b = 0.0;
  double distance_from_point_a_to_point_b          = 0.0;

  distance_from_current_location_to_point_a = GetLineOfSightDistance(current_location, point_a);
  distance_from_current_location_to_point_b = GetLineOfSightDistance(current_location, point_b);
  distance_from_point_a_to_point_b          = GetLineOfSightDistance(point_a,          point_b);

  double p = 0.0;

  p  = distance_from_current_location_to_point_a;
  p += distance_from_current_location_to_point_b;
  p += distance_from_point_a_to_point_b;
  p /= 2.0;

  double temp_double = 0.0;

  temp_double  = p;
  temp_double *= (double) (p - distance_from_current_location_to_point_a);
  temp_double *= (double) (p - distance_from_current_location_to_point_b);
  temp_double *= (double) (p - distance_from_point_a_to_point_b);

  double area = 0.0;

  area = CMath::SquareRoot(temp_double);

  double distance_from_course = 0.0;

  // The altitude from the vertex A is two times the area of the triangle divided by the baseline

  distance_from_course = (2.0 * area) / distance_from_point_a_to_point_b;

  return(distance_from_course);
}

double CEarth::GetLineOfSightDistance(const CEarthCoordinate& point_1, 
                                      const CEarthCoordinate& point_2) const
{
  // This function implements the Pythagoras method of computing the distance
  // between two points.
  // This is a line-of-sight algorithm. It does not take into acccount the 
  // curvature of the Earth. It is not a distance on the surface algorithm.
  // If you had a laser and connected the two points, this algorithm tells
  // you how long the laser beam is. 

  double distance     = 0.0;
  double x_coordinate = 0.0;
  double y_coordinate = 0.0;
  double z_coordinate = 0.0;

  x_coordinate = point_1.GetXCoordinateInMeters() - point_2.GetXCoordinateInMeters();
  y_coordinate = point_1.GetYCoordinateInMeters() - point_2.GetYCoordinateInMeters();
  z_coordinate = point_1.GetZCoordinateInMeters() - point_2.GetZCoordinateInMeters();

  // Square the coordinates
  x_coordinate *= x_coordinate;
  y_coordinate *= y_coordinate;
  z_coordinate *= z_coordinate;

  distance = CMath::SquareRoot(x_coordinate + y_coordinate + z_coordinate);

  return(distance);
}

double CEarth::GetLineOfSightDistance(const CEarthCoordinate& point_1,
                                      const CPolarCoordinate& point_2) const
{
  CEarthCoordinate earth_center_earth_fixed_point_2;

  Convert(point_2,earth_center_earth_fixed_point_2);

  return(GetLineOfSightDistance(point_1,earth_center_earth_fixed_point_2));
}

double CEarth::GetLineOfSightDistance(const CPolarCoordinate& point_1,
                                      const CEarthCoordinate& point_2) const
{
  CEarthCoordinate earth_center_earth_fixed_point_1;

  Convert(point_1,earth_center_earth_fixed_point_1);

  return(GetLineOfSightDistance(earth_center_earth_fixed_point_1,point_2));
}

double CEarth::GetLineOfSightDistance(const CPolarCoordinate& point_1,const CPolarCoordinate& point_2) const
{
  CEarthCoordinate earth_center_earth_fixed_point_1;
  CEarthCoordinate earth_center_earth_fixed_point_2;

  Convert(point_1,earth_center_earth_fixed_point_1);
  Convert(point_2,earth_center_earth_fixed_point_2);

  return(GetLineOfSightDistance(earth_center_earth_fixed_point_1,earth_center_earth_fixed_point_2));
}

double CEarth::GetSurfaceDistance(const CEarthCoordinate& point_1, 
                                  const CEarthCoordinate& point_2, 
                                  double * heading_from_point_1_to_point_2_p, 
                                  double * heading_from_point_2_to_point_1_p) const
{
  CPolarCoordinate polar_point_1;
  CPolarCoordinate polar_point_2;

  Convert(point_1,polar_point_1);
  Convert(point_2,polar_point_2);

  return(GetSurfaceDistance(polar_point_1,
    polar_point_2,
    heading_from_point_1_to_point_2_p, 
    heading_from_point_2_to_point_1_p));
}

double CEarth::GetSurfaceDistance(const CEarthCoordinate& point_1, 
                                  const CPolarCoordinate& point_2, 
                                  double * heading_from_point_1_to_point_2_p, 
                                  double * heading_from_point_2_to_point_1_p) const
{
  CPolarCoordinate polar_point_1;

  Convert(point_1,polar_point_1);

  return(GetSurfaceDistance(polar_point_1,
    point_2,
    heading_from_point_1_to_point_2_p, 
    heading_from_point_2_to_point_1_p));
}

double CEarth::GetSurfaceDistance(const CPolarCoordinate& point_1, 
                                  const CEarthCoordinate& point_2, 
                                  double * heading_from_point_1_to_point_2_p, 
                                  double * heading_from_point_2_to_point_1_p) const
{
  CPolarCoordinate polar_point_2;

  Convert(point_2,polar_point_2);

  return(GetSurfaceDistance(point_1, 
    polar_point_2, 
    heading_from_point_1_to_point_2_p, 
    heading_from_point_2_to_point_1_p));
}

double CEarth::GetSurfaceDistance(const CPolarCoordinate& point_1, 
                                  const CPolarCoordinate& point_2, 
                                  double * heading_from_point_1_to_point_2_p, 
                                  double * heading_from_point_2_to_point_1_p) const
{
  // This is a translation of the Fortran routine INVER1 found in the
  // INVERS3D program at:
  // ftp://ftp.ngs.noaa.gov/pub/pcsoft/for_inv.3d/source/invers3d.for

  // The ton of variables used...

  double c           = 0.0;
  double c_value_1   = 0.0;
  double c_value_2   = 0.0;
  double c2a         = 0.0;
  double cosine_of_x = 0.0;
  double cy          = 0.0;
  double cz          = 0.0;
  double d           = 0.0;
  double e           = 0.0;
  double r_value     = 0.0;
  double s           = 0.0;
  double s_value_1   = 0.0;
  double sa          = 0.0;
  double sine_of_x   = 0.0;
  double sy          = 0.0;
  double tangent_1   = 0.0;
  double tangent_2   = 0.0;
  double x           = 0.0;
  double y           = 0.0;

  int loop_count = 0;

  double heading_from_point_1_to_point_2 = 0.0;
  double heading_from_point_2_to_point_1 = 0.0;

  // UpDown    == Latitude
  // LeftRight == Longitude

  double point_1_latitude_in_radians  = CMath::ConvertDegreesToRadians(point_1.GetUpDownAngleInDegrees());
  double point_1_longitude_in_radians = CMath::ConvertDegreesToRadians(point_1.GetLeftRightAngleInDegrees());
  double point_2_latitude_in_radians  = CMath::ConvertDegreesToRadians(point_2.GetUpDownAngleInDegrees());
  double point_2_longitude_in_radians = CMath::ConvertDegreesToRadians(point_2.GetLeftRightAngleInDegrees());

  r_value = 1.0 - m_Flattening;
  tangent_1 = (r_value * CMath::Sine(point_1_latitude_in_radians)) / CMath::Cosine(point_1_latitude_in_radians);
  tangent_2 = (r_value * CMath::Sine(point_2_latitude_in_radians)) / CMath::Cosine(point_2_latitude_in_radians);
  c_value_1 = 1.0 / CMath::SquareRoot((tangent_1 * tangent_1) + 1.0);
  s_value_1 = c_value_1 * tangent_1;
  c_value_2 = 1.0 / CMath::SquareRoot((tangent_2 * tangent_2) + 1.0);
  s = c_value_1 * c_value_2;

  heading_from_point_2_to_point_1 = s * tangent_2; // backward_azimuth
  heading_from_point_1_to_point_2 = heading_from_point_2_to_point_1 * tangent_1;

  x = point_2_longitude_in_radians - point_1_longitude_in_radians;

  bool exit_loop = false;

  while(exit_loop != true)
  {
    sine_of_x = CMath::Sine(x);
    cosine_of_x = CMath::Cosine(x);
    tangent_1 = c_value_2 * sine_of_x;
    tangent_2 = heading_from_point_2_to_point_1 - (s_value_1 * c_value_2 * cosine_of_x);
    sy = CMath::SquareRoot((tangent_1 * tangent_1) + (tangent_2 * tangent_2));
    cy = (s * cosine_of_x) + heading_from_point_1_to_point_2;
    y = CMath::ArcTangentOfYOverX(sy,cy);

    // Thanks to John Werner (werner@tij.wb.xerox.com) for
    // finding a bug where sy could be zero. Here's his fix:

    if ((s * sine_of_x ) == 0.0 && (sy == 0.0))
    {
      sa = 1.0;
    }
    else
    {
      sa = (s * sine_of_x) / sy;
    }

    c2a = ((-sa) * sa) + 1.0;
    cz = heading_from_point_1_to_point_2 + heading_from_point_1_to_point_2;

    if (c2a > 0.0)
    {
      cz = ((-cz) / c2a) + cy;
    }

    e = (cz * cz * 2.0) - 1.0;
    c = (((((-3.0 * c2a) + 4.0) * m_Flattening) + 4.0) * c2a * m_Flattening) / 16.0;
    d = x;
    x = ((((e * cy * c) + cz) * sy * c) + y) * sa;
    x = ((1.0 - c) * x * m_Flattening) + point_2_longitude_in_radians - point_1_longitude_in_radians;

    if (CMath::AbsoluteValue(d - x) > 0.00000000000000000000005)
    {
      exit_loop = false;
    }
    else
    {
      exit_loop = true;
    }
  }

  heading_from_point_1_to_point_2 = CMath::ArcTangentOfYOverX(tangent_1,tangent_2);

  double temp_degrees         = 0.0;
  double temp_minutes         = 0.0;
  double temp_seconds         = 0.0;
  double temp_decimal_degrees = 0.0;

  temp_decimal_degrees = CMath::ConvertRadiansToDegrees(heading_from_point_1_to_point_2);

  if (temp_decimal_degrees < 0.0)
  {
    temp_decimal_degrees += 360.0;
  }

  if (heading_from_point_1_to_point_2_p != NULL)
  {
    // The user passed us a pointer, don't trust it.
    // If you are using Visual C++ on Windows NT, the following
    // try/catch block will ensure you won't blow up when random
    // pointers are passed to you. If you are on a legacy operating
    // system like Unix, you are screwed.

    try
    {
      *heading_from_point_1_to_point_2_p = temp_decimal_degrees;
    }
    catch( ... )
    {
      // Do Nothing
    }
  }

  heading_from_point_2_to_point_1 = CMath::ArcTangentOfYOverX(c_value_1 * sine_of_x,
    ((heading_from_point_2_to_point_1 * cosine_of_x) - (s_value_1 * c_value_2))) + CMath::Pi();

  temp_decimal_degrees = CMath::ConvertRadiansToDegrees(heading_from_point_2_to_point_1);

  if (temp_decimal_degrees < 0)
  {
    temp_decimal_degrees += 360.0;
  }

  if (heading_from_point_2_to_point_1_p != NULL)
  {
    // The user passed us a pointer, don't trust it.
    // If you are using Visual C++ on Windows NT, the following
    // try/catch block will ensure you won't blow up when random
    // pointers are passed to you. If you are on a legacy operating
    // system like Unix, you are screwed.

    try
    {
      *heading_from_point_2_to_point_1_p = temp_decimal_degrees;
    }
    catch( ... )
    {
      // Do Nothing
    }
  }

  x = CMath::SquareRoot((((1.0 / r_value / r_value) - 1) * c2a) + 1.0) + 1.0;
  x = (x - 2.0) / x;
  c = 1.0 - x;
  c = (((x * x) / 4.0) + 1.0) / c;
  d = ((0.375 * (x * x)) - 1.0) * x;

  // 1998-09-01
  // Thanks go to Gerard Murphy (bjmillar@dera.gov.uk) for finding a typo here.

  x = e * cy;

  s = (1.0 - e) - e;

  double term_1 = 0.0;
  double term_2 = 0.0;
  double term_3 = 0.0;
  double term_4 = 0.0;
  double term_5 = 0.0;

  term_1 = (sy * sy * 4.0) - 3.0;
  term_2 = ((s * cz * d) / 6.0) - x;
  term_3 = term_1 * term_2;
  term_4 = ((term_3 * d) / 4.0) + cz;
  term_5 = (term_4 * sy * d) + y;

  s = term_5 * c * m_EquatorialRadiusInMeters * r_value;

  return( s );
}

void CEarth::m_ComputeEccentricitySquared(void)
{
  if (m_Flattening == 0.0)
  {
    m_EccentricitySquared = 0.0;
    return;
  }

  m_EccentricitySquared = (2.0 * m_Flattening) - (m_Flattening * m_Flattening);
  //printf( "Eccentricity Squared = %.23lf\n", m_EccentricitySquared );
}

void CEarth::m_ComputeFlattening(void)
{
  if (m_EquatorialRadiusInMeters == 0.0 || m_PolarRadiusInMeters == 0.0)
  {
    return;
  }

  m_Flattening = CMath::AbsoluteValue(m_EquatorialRadiusInMeters - m_PolarRadiusInMeters) / m_EquatorialRadiusInMeters;
  //printf( "Flattening = %.23lf\n", m_Flattening );
}

void CEarth::m_Initialize(void)
{
  m_EllipsoidID              = 0;
  m_PolarRadiusInMeters      = 0.0;
  m_EquatorialRadiusInMeters = 0.0;
  m_Flattening               = 0.0;
  m_EccentricitySquared      = 0.0;
}

void CEarth::SetEllipsoid(int ellipsoid_identifier)
{
  m_EllipsoidID = ellipsoid_identifier;

  switch(ellipsoid_identifier)
  {
  case Perfect_Sphere:
    m_EquatorialRadiusInMeters = 6378137.0;
    m_PolarRadiusInMeters      = 6378137.0;
    break;

  case Airy:
    m_EquatorialRadiusInMeters = 6377563.396;
    m_PolarRadiusInMeters      = 6356256.909237;
    break;

  case Austrailian_National:
    m_EquatorialRadiusInMeters = 6378160.0;
    m_PolarRadiusInMeters      = 6356774.719195;
    break;

  case Bessell_1841:
    m_EquatorialRadiusInMeters = 6377397.155;
    m_PolarRadiusInMeters      = 6356078.962818;
    break;

  case Bessel_1841_Nambia:
    m_EquatorialRadiusInMeters = 6377483.865;
    m_PolarRadiusInMeters      = 6356165.382966;
    break;

  case Clarke_1866:
    m_EquatorialRadiusInMeters = 6378206.4;
    m_PolarRadiusInMeters      = 6356583.799999;
    break;

  case Clarke_1880:
    m_EquatorialRadiusInMeters = 6378249.145;
    m_PolarRadiusInMeters      = 6356514.86955;
    break;

  case Everest:
    m_EquatorialRadiusInMeters = 6377276.345;
    m_PolarRadiusInMeters      = 6356075.41314;
    break;

  case Fischer_1960_Mercury:
    m_EquatorialRadiusInMeters = 6378166.0;
    m_PolarRadiusInMeters      = 6356784.283607;
    break;

  case Fischer_1968:
    m_EquatorialRadiusInMeters = 6378150.0;
    m_PolarRadiusInMeters      = 6356768.337244;
    break;

  case GRS_1967:
    m_EquatorialRadiusInMeters = 6378160.0;
    m_PolarRadiusInMeters      = 6356774.516091;
    break;

  case GRS_1980:
    m_EquatorialRadiusInMeters = 6378137.0;
    m_PolarRadiusInMeters      = 6356752.31414;
    break;

  case Helmert_1906:
    m_EquatorialRadiusInMeters = 6378200.0;
    m_PolarRadiusInMeters      = 6356818.169628;
    break;

  case Hough:
    m_EquatorialRadiusInMeters = 6378270.0;
    m_PolarRadiusInMeters      = 6356794.343434;
    break;

  case International:
    m_EquatorialRadiusInMeters = 6378388.0;
    m_PolarRadiusInMeters      = 6356911.946128;
    break;

  case Krassovsky:
    m_EquatorialRadiusInMeters = 6378245.0;
    m_PolarRadiusInMeters      = 6356863.018773;
    break;

  case Modified_Airy:
    m_EquatorialRadiusInMeters = 6377340.189;
    m_PolarRadiusInMeters      = 6356034.447939;
    break;

  case Modified_Everest:
    m_EquatorialRadiusInMeters = 6377304.063;
    m_PolarRadiusInMeters      = 6356103.038993;
    break;

  case Modified_Fischer_1960:
    m_EquatorialRadiusInMeters = 6378155.0;
    m_PolarRadiusInMeters      = 6356773.320483;
    break;

  case South_American_1969:
    m_EquatorialRadiusInMeters = 6378160.0;
    m_PolarRadiusInMeters      = 6356774.719195;
    break;

  case Topex_Poseidon_Pathfinder_ITRF: // Source is http://neptune.gsfc.nasa.gov/~krachlin/corr/refframe.html
    m_EquatorialRadiusInMeters = 6378136.3;
    m_PolarRadiusInMeters      = 6356751.6005629376;
    break;

  case WGS_60:
    m_EquatorialRadiusInMeters = 6378165.0;
    m_PolarRadiusInMeters      = 6356783.286959;
    break;

  case WGS_66:
    m_EquatorialRadiusInMeters = 6378145.0;
    m_PolarRadiusInMeters      = 6356759.769489;
    break;

  case WGS_72:
    m_EquatorialRadiusInMeters = 6378135.0;
    m_PolarRadiusInMeters      = 6356750.520016;
    break;

  case WGS_84:
    // Computed polar radius from the flattening value specified at
    // http://acro.harvard.edu/SSA/BGA/wg84figs.html
    // because it had the most digits after the decimal point.

    m_EquatorialRadiusInMeters = 6378137.0;
    m_PolarRadiusInMeters      = 6356752.3142451793;
    break;

  case Unknown:
  default:

    m_EllipsoidID = Unknown;

    m_Initialize();
    return;
  }

  m_ComputeFlattening();
  m_ComputeEccentricitySquared();
}

void CEarth::SetEllipsoidByRadii(double equatorial_radius,double polar_radius)
{
  m_EquatorialRadiusInMeters = equatorial_radius;
  m_PolarRadiusInMeters      = polar_radius;
  m_EllipsoidID              = Custom;

  m_ComputeFlattening();
  m_ComputeEccentricitySquared();
}

void CEarth::SetEllipsoidByEquatorialRadiusAndFlattening(double equatorial_radius,double flattening)
{
  m_EquatorialRadiusInMeters = equatorial_radius;
  m_Flattening               = flattening;
  m_EllipsoidID              = Custom;

  // We must compute the polar radius

  double temp_double = m_Flattening * m_EquatorialRadiusInMeters;

  m_PolarRadiusInMeters = m_EquatorialRadiusInMeters - temp_double;

  m_ComputeEccentricitySquared();
}

// WGS84 to Gauss-Krueger converter (recoding of Pascal sources from Ottmar Labonde)

void WGS84toDHDN::HelmertTransformation(double x,double y,double z,double& xo,double& yo,double& zo)
{
  xo=dx+(sc*(1*x+rotz*y-roty*z));
  yo=dy+(sc*(-rotz*x+1*y+rotx*z));
  zo=dz+(sc*(roty*x-rotx*y+1*z));
}

void WGS84toDHDN::BesselBLnachGaussKrueger(double b,double ll,double& Re,double& Ho)
{
  double l;
  double l0;
  double bg;
  double lng;
  double Ng;
  double k;
  double t;
  double eq;
  double Vq;
  double v;
  double nk;
  double X;
  double gg;
  double SS;
  double Y;
  double kk;
  double Pii;
  double RVV;

  bg=180*b/Pi;
  lng=180*ll/Pi;
  l0=3*round((180*ll/Pi)/3);
  l0=Pi*l0/180;
  l=ll-l0;
  k=cos(b);
  t=sin(b)/k;
  eq=(abes*abes-bbes*bbes)/(bbes*bbes);
  Vq=1+eq*k*k;
  v=sqrt(Vq);
  Ng=abes*abes/(bbes*v);
  nk=(abes-bbes)/(abes+bbes);
  X=((Ng*t*k*k*l*l)/2)+((Ng*t*(9*Vq-t*t-4)*k*k*k*k*l*l*l*l)/24);

  gg=b+(((-3*nk/2)+(9*nk*nk*nk/16))*sin(2*b)+15*nk*nk*sin(4*b)/16-35*nk*nk*nk*sin(6*b)/48);
  SS=gg*180*cbes/Pi;
  Ho=(SS+X);
  Y=Ng*k*l+Ng*(Vq-t*t)*k*k*k*l*l*l/6+Ng*(5-18*t*t+t*t*t*t)*k*k*k*k*k*l*l*l*l*l/120;
  kk=500000;
  Pii=Pi;
  RVV=round((180*ll/Pii)/3);
  Re=RVV*1000000+kk+Y;
}

void WGS84toDHDN::BLRauenberg (double x,double y,double z,double& b,double& l,double& h)
{
  double f;
  double f1;
  double f2;
  double ft;
  double p;

  f=Pi*50/180;
  p=Z/sqrt(x*x+y*y);

  do
  {
    f1=neuF(f,x,y,p);
    f2=f;
    f=f1;
    ft=180*f1/Pi;
  }
  while(!(fabs(f2-f1)<10E-10));

  b=f;
  l=atan(y/x);
  h=sqrt(x*x+y*y)/cos(f1)-a/sqrt(1-eq*sin(f1)*sin(f1));
}

double WGS84toDHDN::neuF(double f,double x,double y,double p)
{
  double zw;
  double nnq;

  zw=a/sqrt(1-eq*sin(f)*sin(f));
  nnq=1-eq*zw/(sqrt(x*x+y*y)/cos(f));
  return(atan(p/nnq));
}

double WGS84toDHDN::round(double src)
{
  double theInteger;
  double theFraction;
  double criterion = 0.5;

  theFraction = modf(src,&theInteger);

  if (!(theFraction < criterion))
  {
    theInteger += 1; 
  } 

  return theInteger;
}

WGS84toDHDN::WGS84toDHDN()
{
  awgs = 6378137.0;        // WGS84 Semi-Major Axis = Equatorial Radius in meters
  bwgs = 6356752.314;      // WGS84 Semi-Minor Axis = Polar Radius in meters
  abes = 6377397.155;      // Bessel Semi-Major Axis = Equatorial Radius in meters
  bbes = 6356078.962;      // Bessel Semi-Minor Axis = Polar Radius in meters
  cbes = 111120.6196;      // Bessel ???
  dx   = -585.7;           // Translation Parameter 1
  dy   = -87.0;            // Translation Parameter 2
  dz   = -409.2;           // Translation Parameter 3
  rotx = 2.540423689E-6;   // Rotation Parameter 1
  roty = 7.514612057E-7;   // Rotation Parameter 2
  rotz = -1.368144208E-5;  // Rotation Parameter 3
  sc   = 1/0.99999122;     // Scaling Factor
  Pi   = 3.1415926535897932384626433832795028841971693993751058209749445923078164;
  l1 = 0.0;
  b1 = 0.0;
  l2 = 0.0;
  b2 = 0.0;
  h1 = 0.0;
  h2 = 0.0;
  R = 0.0;
  H = 0.0;
  a = 0.0;
  b = 0.0;
  eq = 0.0;
  N = 0.0;
  Xq = 0.0;
  Yq = 0.0;
  Zq = 0.0;
  X = 0.0;
  Y = 0.0;
  Z = 0.0;
}

WGS84toDHDN::~WGS84toDHDN()
{
  l1 = 0.0;
  b1 = 0.0;
  l2 = 0.0;
  b2 = 0.0;
  h1 = 0.0;
  h2 = 0.0;
  R = 0.0;
  H = 0.0;
  a = 0.0;
  b = 0.0;
  eq = 0.0;
  N = 0.0;
  Xq = 0.0;
  Yq = 0.0;
  Zq = 0.0;
  X = 0.0;
  Y = 0.0;
  Z = 0.0;
}

// Precision is +- 1 meter for Germany
void WGS84toDHDN::Convert(double latIn,double lonIn,double heightIn,double& ROut,double& HOut)
{
  b1 = latIn;
  l1 = lonIn;
  h1 = heightIn;

  l1=Pi*l1/180;
  b1=Pi*b1/180;
  a=awgs; 
  b=bwgs;
  eq=(a*a-b*b)/(a*a);
  N=a/sqrt(1-eq*sin(b1)*sin(b1));
  Xq=(N+h1)*cos(b1)*cos(l1);
  Yq=(N+h1)*cos(b1)*sin(l1);
  Zq=((1-eq)*N+h1)*sin(b1);
  HelmertTransformation(Xq,Yq,Zq,X,Y,Z);
  a=abes;
  b=bbes;
  eq=(a*a-b*b)/(a*a);
  BLRauenberg(X,Y,Z,b2,l2,h2);
  BesselBLnachGaussKrueger(b2,l2,R,H);

  ROut = R;
  HOut = H;
}


