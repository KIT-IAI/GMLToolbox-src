//////////////////////////////////////////
// Author:     Steffen Hempel
// Created:    2012/06/13
//
// Changes:		Paul Kopf
// Changed:		2015/07/17
//
// Triangulator base class
////////////////////////////////////////
#pragma once
#include <list>
#include "../Geometrie.h"
#include "../Logfile.h"

#include "..\Topologie.h"

class Polygon;
class Triangle;
class GeoPoint;
class _Ring;

class Triangulator
{
  public:
    
    typedef std::vector<GeoPoint*> Points;
    typedef std::vector<_Ring*> Polyloops;

	
    virtual ~Triangulator() 
	{	
	}


	std::vector<int>			  getCoordIndex()		const { return m_vCoordIndex; }
	std::vector<GeoPoint*>	getPoints()			const { return m_vPoints; }

	void addIndex(int index)					  { m_vCoordIndex.push_back(index); }
	void addPoint3D(GeoPoint* point)		{ m_vPoints.push_back(point); }

    /// Triangulates a given face
    virtual bool triangulate( PolygonGeo * pPoly, bool ori ) = 0;

  protected:

    //Output
	std::vector<int>			  m_vCoordIndex;
	std::vector<GeoPoint*>	m_vPoints;

};


