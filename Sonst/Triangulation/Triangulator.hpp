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
//#include "../Geometrie.h"

#include "..\X3D-Module\X3D-Module\Writable\X3D\Point3D.h"
#include "..\Topologie.h"

class Polygon;
class Triangle;
//class GeoPoint;
class _Ring;

class Triangulator
{
  public:
    
    typedef std::vector<X3D::Point3D*> Points;
    typedef std::vector<_Ring*> Polyloops;

	
    //Triangulator(Polygon* poly) { m_polygon = poly; }
    virtual ~Triangulator() 
	{
		//delete m_pPolygon;
		
		/*for (auto iter = m_vPoints.begin(); iter != m_vPoints.end(); ++iter)
		{
			delete (*iter);
		}*/

		delete m_kdTree;

		/*for (auto iter = m_mKdTreeNodes.begin(); iter != m_mKdTreeNodes.end(); ++iter)
		{
			delete iter->first;
		}*/
	}

    /// Sets the contour, a contour must have been transformed into a 2d plane
    /// already.
    //void setContour(const _Ring& contour) { m_contour = contour; }
    /// Adds a hole, a hole must have been transformed into a 2d plane already
    //void addHole(const _Ring& hole) { m_holes.push_back(hole); }
    /// Sets holes, a hole must have been transformed into a 2d plane already
    //void setHoles(const Polyloops& holes) { m_holes = holes; }

    /// Returns the triangles created by calling the triangulate() method
    //std::list<Triangle*>		getTriangles() const { return m_triangles; }
//	PolygonGeo *					getPolygon()	const { return m_pPolygon; }
	std::vector<int>			getCoordIndex()		const { return m_vCoordIndex; }
	std::vector<X3D::Point3D*>	getPoints()			const { return m_vPoints; }
	KdTree*						getKdTree()			const { return m_kdTree; }

//	void setPolygon ( PolygonGeo * poly)				{ m_pPolygon = poly; }

	void addIndex(int index)					{ m_vCoordIndex.push_back(index); }
	void addPoint3D(X3D::Point3D* point)		{ m_vPoints.push_back(point); }
	void addKdTreeNode(KdTreeNode* kdTreeNode)	{ m_mKdTreeNodes[kdTreeNode] = m_mKdTreeNodes.size()-1; }

	int findKdTreeNode(KdTreeNode* kdTreeNode)	{ return m_mKdTreeNodes.find(kdTreeNode)->second; }

    /// Triangulates a given face
    virtual void triangulate( PolygonGeo * pPoly, bool ori ) = 0;

  protected:
	//Input
//	PolygonGeo *					m_pPolygon;

	//Output
	std::vector<int>			     m_vCoordIndex;
	std::vector<X3D::Point3D*>	m_vPoints;

	KdTree *						        m_kdTree;
	std::map<KdTreeNode*, int>	m_mKdTreeNodes;
	//std::list<Triangle*>	m_triangles;
};

//inline bool mustTriangulate(const _Ring& contour, bool hasHoles)
//{
//  return hasHoles || contour.isConcave(IfcDB::Matrix4());
//}

