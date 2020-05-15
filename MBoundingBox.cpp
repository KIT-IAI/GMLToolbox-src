#include "StdAfx.h"
#include ".\mboundingbox.h"
#using <mscorlib.dll>
#include "constant.h"
#include <math.h>
#include <limits.h>
#include <float.h>

///////////////////////////////////////////////////////////////////////////////
//  class MBoundingBox										                            			 //
//  Klasse für Bounding Boxes - 	Kleinster umschließender Quader einer bel. //
//  Geometrie										                  	                         //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor									                              							 //
///////////////////////////////////////////////////////////////////////////////
MBoundingBox::MBoundingBox()
{
	BoundingBox          = NULL;
  minimalPointDistance = MINIMAL_POINT_DISTANCE;
}

///////////////////////////////////////////////////////////////////////////////
//  Destruktor								                              								 //
///////////////////////////////////////////////////////////////////////////////
MBoundingBox::~MBoundingBox()
{
	bbDelete();
}

///////////////////////////////////////////////////////////////////////////////
//  Attribute BoundingBox setzen			                      								 //
///////////////////////////////////////////////////////////////////////////////
MBoundingBox&  MBoundingBox::operator= ( MBoundingBox & box	)
{
	double sw[] = { 0.0, 0.0, 0.0};
	double no[] = {0.0, 0.0, 0.0 };
	bool   success;

	success = box.bbGet ( sw, no );
	if ( success )
		bbSet ( sw, no );

  minimalPointDistance = box.minimalPointDistance;

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
//  Gleichheitsoperator											                          			 //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::operator== (	MBoundingBox & box )
{
	double sw[3], no[3];
	bool   success;

	success = box.bbGet ( sw, no );
	if ( !success || !bbExist() ) return false;

	if ( fabs ( BoundingBox[0] - sw[0] ) >= minimalPointDistance )
		return false;

	if ( fabs ( BoundingBox[1] - sw[1] ) >= minimalPointDistance )
		return false;

	if ( fabs ( BoundingBox[2] - sw[2] ) >= minimalPointDistance )
		return false;

	if ( fabs ( BoundingBox[3] - no[0] ) >= minimalPointDistance )
		return false;

	if ( fabs ( BoundingBox[4] - no[1] ) >= minimalPointDistance )
		return false;

	if ( fabs ( BoundingBox[5] - no[2] ) >= minimalPointDistance  )
		 return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Gleichheitsoperator	bzgl. XY-Ebene			                    						 //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::equalXY ( MBoundingBox * box	)
{
	double sw[3], no[3];
	bool   success;

	success = box->bbGet ( sw, no );
	if ( !success || !bbExist() ) return false;

	if ( fabs ( BoundingBox[0] - sw[0] ) >= minimalPointDistance )
		return false;

	if ( fabs ( BoundingBox[1] - sw[1] ) >= minimalPointDistance )
		return false;

	if ( fabs ( BoundingBox[3] - no[0] ) >= minimalPointDistance )
		return false;

	if ( fabs ( BoundingBox[4] - no[1] ) >= minimalPointDistance )
		return false;

	return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Setzt die Bounding Box Parameter		                    								 //
///////////////////////////////////////////////////////////////////////////////
void MBoundingBox::bbSet ( double sw[], double no[] 	)
{
	if ( !bbExist() ) newBB();

	for ( int i = 0; i < 3; i++ )
	{
		BoundingBox[i] = sw[i];
		BoundingBox[i+3] = no[i];
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Bounding Box Parameter				                  						 //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::bbGet ( double sw[], double no[] )
{
	if ( !bbExist() ) return false;

	for ( int i = 0; i < 3; i++ )
	{
		sw[i]  = BoundingBox[i];
		no[i] = BoundingBox[i+3];
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  BoundingBox Parameter vorhanden?				                    						 //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::bbExist()
{
	return ( BoundingBox != NULL );
}

///////////////////////////////////////////////////////////////////////////////
//  Macht die BoundingBox ungültig					                    						 //
///////////////////////////////////////////////////////////////////////////////
void MBoundingBox::bbDelete()
{
	if ( BoundingBox != NULL )
	{
		delete [] BoundingBox;
		BoundingBox = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Addiert BoundingBox mit den angegebenen Parametern	          					 //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::addBB ( double sw[], double no[] )
{
	bool   changed = false;

	if ( !bbExist() ) newBB();

	if ( sw[0] < BoundingBox[0] - minimalPointDistance )
	{
		BoundingBox[0] = sw[0];
		changed = true;
	}

	if ( sw[1] < BoundingBox[1] - minimalPointDistance )
	{
		BoundingBox[1] = sw[1];
		changed = true;
	}

	if ( sw[2] < BoundingBox[2] - minimalPointDistance )
	{
		BoundingBox[2] = sw[2];
		changed = true;
	}

	if ( no[0] > BoundingBox[3] + minimalPointDistance )
	{
		BoundingBox[3] = no[0];
		changed = true;
	}

	if ( no[1] > BoundingBox[4] + minimalPointDistance )
	{
		BoundingBox[4] = no[1];
		changed = true;
	}

	if ( no[2] > BoundingBox[5] + minimalPointDistance )
	{
		BoundingBox[5] = no[2];
		changed = true;
	}

	return changed;
}

///////////////////////////////////////////////////////////////////////////////
//  Addiert BoundingBox								                          						 //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::addBB ( MBoundingBox * bb )
{
	double sw[3], no[3];
	bool   exist;

	exist = bb->bbGet ( sw, no );
	if ( !exist ) return false;

	return addBB ( sw, no );
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt die BoundingBox	um offset			                  						 //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::addOffset ( double offset[] )
{
	if ( bbExist() )
	{
		for ( int i = 0; i < 3; i++ )
    {
			BoundingBox[i] = BoundingBox[i] + offset[i];
      BoundingBox[i+3] = BoundingBox[i+3] + offset[i];
    }
	}

	return bbExist();
}


///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob bb innerhalb der BoundingBox oder teilweise auf dem Rand   //
//  liegt																                                  	 //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::inside ( MBoundingBox * bb	)
{
	double sw[3], no[3];
	bool   exist;

	exist = bb->bbGet ( sw, no );
	if ( !exist || !bbExist() ) return false;

	if ( sw[0] < BoundingBox[0] - minimalPointDistance )
		return false;

	if ( sw[1] < BoundingBox[1] - minimalPointDistance )
		return false;

	if ( sw[2] < BoundingBox[2] - minimalPointDistance )
		return false;

	if ( no[0] > BoundingBox[3] + minimalPointDistance )
		return false;

	if ( no[1] > BoundingBox[4] + minimalPointDistance )
		return false;

	if ( no[2] > BoundingBox[5] + minimalPointDistance )
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob der xy-Anteil von bb innerhalb der BoundingBox	      		 //
//  oder teilweise auf dem Rand  liegt				                  						 //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::insideXY ( MBoundingBox * bb )
{
	double sw[3], no[3];
	bool   exist;

	exist = bb->bbGet ( sw, no );
	if ( !exist ) return false;

	if ( sw[0] < BoundingBox[0] - minimalPointDistance )
		return false;

	if ( sw[1] < BoundingBox[1] - minimalPointDistance )
		return false;

	if ( no[0] > BoundingBox[3] + minimalPointDistance )
		return false;

	if ( no[1] > BoundingBox[4] + minimalPointDistance )
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob bb voll innerhalb der BoundingBox liegt	          				 //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::fullInside	( MBoundingBox * bb )
{
	double sw[3], no[3];
	bool   exist;

	exist = bb->bbGet ( sw, no );
	if ( !exist ) return false;

	if ( sw[0] < BoundingBox[0] + minimalPointDistance )
		return false;

	if ( sw[1] < BoundingBox[1] + minimalPointDistance )
		return false;

	if ( sw[2] < BoundingBox[2] + minimalPointDistance )
		return false;

	if ( no[0] > BoundingBox[3] - minimalPointDistance )
		return false;

	if ( no[1] > BoundingBox[4] - minimalPointDistance )
		return false;

	if ( no[2] > BoundingBox[5] - minimalPointDistance )
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob der xy-Anteil von bb voll innerhalb der BoundingBox liegt	 //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::fullInsideXY ( MBoundingBox * bb )
{
	double sw[3], no[3];
	bool   exist;

	exist = bb->bbGet ( sw, no );
	if ( !exist ) return false;

	if ( sw[0] < BoundingBox[0] + minimalPointDistance )
		return false;

	if ( sw[1] < BoundingBox[1] + minimalPointDistance )
		return false;

	if ( no[0] > BoundingBox[3] - minimalPointDistance )
		return false;

	if ( no[1] > BoundingBox[4] - minimalPointDistance )
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Legt das Datenfeld BoundingBox neu an							                  		 //
///////////////////////////////////////////////////////////////////////////////
void MBoundingBox::newBB()
{
	bbDelete();
	BoundingBox = new double[6];

	for ( int i = 0; i < 3; i++ )
	{
		BoundingBox[i] = FLT_MAX;
		BoundingBox[i+3] = FLT_MAX;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt ob 2 BoundingBox Objekte disjunkt sind                          //
///////////////////////////////////////////////////////////////////////////////
bool MBoundingBox::disjunct ( MBoundingBox * bb )
{
  double sw[3], no[3];
  bool   exist;

  exist = bb->bbGet ( sw, no );
  if ( !exist ) return false;

  if ( no[0] < BoundingBox[0] - minimalPointDistance ||
       no[1] < BoundingBox[1] - minimalPointDistance ||
       no[2] < BoundingBox[2] - minimalPointDistance ||
       sw[0] > BoundingBox[3] + minimalPointDistance ||
       sw[1] > BoundingBox[4] + minimalPointDistance ||
       sw[2] > BoundingBox[5] + minimalPointDistance )
       return true;
  else
    return false;
}

bool MBoundingBox::disjunct ( double swA[], double noA[], double swB[], double noB[], double minPointDist )
{

 if ( noA[0] < swB[0] - minPointDist ||
      noA[1] < swB[1] - minPointDist ||
      noA[2] < swB[2] - minPointDist ||
      swA[0] > noB[0] + minPointDist ||
      swA[1] > noB[1] + minPointDist ||
      swA[2] > noB[2] + minPointDist )
    return true;
  else
    return false;
}


