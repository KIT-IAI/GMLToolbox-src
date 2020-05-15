#pragma once

#include "Includes.h"
#include "Geometrie.h"

class TopolHalfEdge;
class KdTree;
class EdgeMap;
class KdTreeNode;
class KdTreeCursor;
class Face;
class FaceRing;
class Edge;
class EdgeMap;
class EdgeMapItem;
class Error;

///////////////////////////////////////////////////////////////////////////////
//  class TopolPoint                                                         //
//  Stützpunkt eines Ringes mit expliziter Speicherung der Half-Edges, die   //
//  diesen Stützpunkt beinhalten                                             //
//  Die Klasse ist derzeit auf den Spezialfall von 2 überlappenden oder      //
//  berührenden Ringen reduziert                                             //
///////////////////////////////////////////////////////////////////////////////
class TopolPoint
{
  public:
    TopolPoint ( double x, double y );
    TopolPoint ( GeoPoint p );
    virtual ~ TopolPoint();

    GeoPoint getPoint()  {   return m_point;}
    void setPoint ( GeoPoint p )  {  m_point = p;  }

    void setLocation ( POINT_LOCATION loc )  {  m_location = loc;  }
    int getLocation()  {  return m_location;  }

  protected:
    GeoPoint m_point;    //  Zugehöriger geometrischer Punkt
    int      m_location; //  Lage des Punktes bzgl. des zugehörigen Ringes
};

///////////////////////////////////////////////////////////////////////////////
//  class TopolHalfEdge                                                      //
//  Gerichtete Kante eines TopolRing                                         //
///////////////////////////////////////////////////////////////////////////////
class TopolHalfEdge
{
  public:
    TopolHalfEdge ( TopolPoint * pStartPoint );
    TopolHalfEdge ( TopolPoint * pStartPoint, TopolHalfEdge * pNextEdge );
    TopolHalfEdge ( TopolHalfEdge * pEdge );
    virtual ~ TopolHalfEdge();

    TopolPoint * getStartPoint()  {  return m_pStartPoint;  }
    TopolPoint * getEndPoint();

    void            setNextEdge ( TopolHalfEdge * pEdge )  {  m_pNextEdge = pEdge;  }
    TopolHalfEdge * getNextEdge()  {  return m_pNextEdge;  }

    void setCrossEdge ( TopolHalfEdge * pEdge  )  {  m_pCrossEdge = pEdge;  }
    TopolHalfEdge * getCrossEdge()  {  return m_pCrossEdge;  }
    bool getIsCrossEdge();

    void setIsDoubleEdge( bool b )  {  m_isDoubleEdge = b;  }
    bool getIsDoubleEdge()  {  return m_isDoubleEdge;  }

    void setIsProcessed ( bool b )  {  m_isProcessed = b;  }
    bool getIsProcessed()  {  return m_isProcessed;  }

    int insertCutPoint ( GeoPoint cutPoint, std::vector<TopolHalfEdge*> &vNewEdges, double minDist );
    int insertCutPoint ( GeoPoint cutPoint1, GeoPoint cutPoint2 , std::vector<TopolHalfEdge*> &vNewEdges, double minDist );

    void cross ( TopolHalfEdge * pEdge, double minDist );

    bool merge ( TopolHalfEdge * pEdge, double minDist, double minAngleDist );

    double getAngle( TopolHalfEdge * pEdge );


  protected:
    TopolPoint     * m_pStartPoint;  //  Startpunkt der gerichteten Kante
    TopolHalfEdge  * m_pNextEdge;    //  Folgende Kante. Der Startpunkt von m_pNextEdge ist der Endpunkt der Kante
    TopolHalfEdge  * m_pCrossEdge;   //  Verzweigungs-Kante
    bool             m_isDoubleEdge; //  Gibt an, ob eine andere Kante mit gleichen Stützpunkten, aber umgekehrter Richtung gibt
    bool             m_isProcessed;  //  Gibt an, ob die Kante in einem Algorithmus bereits durchlaufen wurde
};

///////////////////////////////////////////////////////////////////////////////
//  TopolRing                                                                //
//  Repräsentation eines _Ring mit Topologie-Informationen                   //
///////////////////////////////////////////////////////////////////////////////
class TopolRing
{
  public:
    enum CUT_FOLLOW {  SELF, CROSS, END  };
  public:
    TopolRing();
    TopolRing ( TopolRing * pRing );
    TopolRing ( std::vector<TopolHalfEdge*> vEdges );
    virtual ~ TopolRing();

    TopolHalfEdge * getStartEdge() {  return m_pStartEdge;  }
    _Ring         * getOriginalRing()  {  return m_pOriginalRing; }

    bool create ( _Ring * pRing );
    LinearRing * createLinearRing();
    void createOriginalRing();

    void merge ( TopolRing * pRing );

		size_t cut ( TopolRing * pCutRing, std::vector<TopolRing*> &vResultRings );
		size_t difference (  TopolRing * pDiffRing, std::vector<TopolRing*> &vResultRings );
		size_t _union (  TopolRing * pUnionRing, std::vector<TopolRing*> &vResultRings );

    void setMinDist ( double d )  {  m_minDist = d; }
    double getMinDist()  {  return m_minDist;  }

    void setMinAngleDist ( double d )  {  m_minAngleDist = d; }
    double getMinAngleDist()  {  return m_minAngleDist;  }

    int createLinearRings( std::vector<LinearRing*> &vLinearRings );
		size_t getPoints ( std::vector<GeoPoint> &vPoints );
    std::string getPointsAsString ();

		size_t debug ( std::vector<std::string> &vTopolPoints );

		size_t getPointAnz();

    double getArea();

    void getPointClassifications ( unsigned int & anzInnerPoints, unsigned int & anzBoundaryPoints, unsigned int & anzOuterPoints );

		size_t getDoubleEdges ( std::vector<TopolHalfEdge*> &vDoubleEdges );
		size_t getCrossPoints ( std::vector<TopolPoint*> &vCrossPoints );

  protected:
    TopolRing * followCutEdge ( TopolHalfEdge * pEdge );
    TopolRing * followDiffEdge ( TopolHalfEdge * pEdge );
    TopolRing * followUnionEdge ( TopolHalfEdge * pEdge );
    CUT_FOLLOW checkCutFollow ( TopolHalfEdge * pEdge );
    CUT_FOLLOW checkDiffFollow ( TopolHalfEdge * pEdge, TopolHalfEdge * pPrevEdge );

  protected:
    TopolHalfEdge * m_pStartEdge;            //  Anfangs-Kante des topol, Ringes
    _Ring         * m_pOriginalRing;         //  Zugehöriger geometrischer Ring
    bool            m_OriginalRingGenerated; //  Gibt an, ob m_pOriginalRing erzeugt wurde
    double          m_minDist;               //  Minimaler Abstand zweier unterschiedlicher Punkte
    double          m_minAngleDist;          //  Minimale Winkeldifferenz der Orientierunger zweier nicht-paralleler Ebenen
};

///////////////////////////////////////////////////////////////////////////////
//  TopolRingSet                                                             //
//  Menge von TopolRing Objekten                                             //
///////////////////////////////////////////////////////////////////////////////
class TopolRingSet
{
public:
  TopolRingSet ();
  virtual ~TopolRingSet();

protected:
  std::set<TopolRing*> sTopolRings;   //  set der TopolRing* Objekte

public:
  void addRing ( TopolRing * pRing )  {  sTopolRings.insert( pRing );  }

	size_t _union ( std::vector<TopolRing*> &vUnionRings );
};


///////////////////////////////////////////////////////////////////////////////
//  BRep                                                                     //
//  Nicht-redundante Repräsentation einer offenen oder geschlossenen Fläche  //
///////////////////////////////////////////////////////////////////////////////
class BRep
{
  public:
    BRep(  _Surface * pSurface, double minDist, Features * pFeaturesP );
    virtual ~ BRep();

  protected:
    std::vector<Face*> vFaces;   //  Vektor der topologischen Ringe, die die Flächen-Polygone repräsentieren
    KdTree           * pKdTree;  //  Geometrischer Index der Polygon-Stützpunkte
    EdgeMap          * edgeMap;  //  Geometrischer Index der Polygon-Kanten
    int                dim;      //  Raumdimension der Fläche
    Features         * pFeatures;  //  Features-Modul

    MultiCurve       * pSingleEdges;    // Kanten, die nur einmal durchlaufen werden
    MultiCurve       * pDoubleEdges;    // Kanten die genau zweimal mit unterschiedlicher Richtung durchlaufen werden
    MultiCurve       * pMultipleEdges;  // Kanten, die unterschiedlich häufig hin und zurück durchlaufen werden 
    MultiCurve       * pBackForthEdges; //  Kanten, mehr als einmal gleich häufig durchlaufen werden

    MultiSurface     * pSingleEdgePolygons;     // Zugehörige Polygone von pSingleEdges
    MultiSurface     * pDoubleEdgePolygons;     // Zugehörige Polygone von pDoubleEdges
    MultiSurface     * pMultipleEdgePolygons;   // Zugehörige Polygone von pMultipleEdges
    MultiSurface     * pBackForthEdgePolygons;  // Zugehörige Polygone von pBackForthEdges

  public:
    KdTree  * getKdTree()  {  return pKdTree;  }
    EdgeMap * getEdgeMap()  {  return edgeMap;  }

    bool isVolume ( Feature * pObj, std::string featureProperty, std::string geometryId, CheckingParams * pCheckParams, std::vector<Error*> &vErrors );
    bool isVolume ();
    bool checkTopology ( Feature * pObj, std::string featureProperty, std::string geometryId, CheckingParams * pCheckParams, std::vector<Error*> &vErrors );
    double getMinimalPointDistance();
    
    void    createIncorrectEdgesForSolid();
    void    createIncorrectEdgesForSurface();
    void    createPolygonsAdjacentToIncorrectEdges( bool singleEdgeError );

    MultiCurve * getSingleEdges()  {  return pSingleEdges;  }
    MultiCurve * getDoubleEdges()  {  return pDoubleEdges;  }
    MultiCurve * getMultipleEdges()  {  return pMultipleEdges;  }

    MultiSurface * getSingleEdgePolygons()  {  return pSingleEdgePolygons;  }
    MultiSurface * getDoubleEdgePolygons()  {  return pDoubleEdgePolygons;  }
    MultiSurface * getMultipleEdgePolygons()  {  return pMultipleEdgePolygons;  }

    virtual void offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries );

  protected:
    MultiCurve * createMultiCurve( std::vector<Edge*> &vEdges );
    MultiSurface * createMultiSurface ( std::set<PolygonGeo*> & sPolygons );
    void createMultiCurveWithoutAggregation( std::vector<Edge*> &vEdges );
    int findNextUnusedEdge ( std::vector<bool> & vUnusedEdges );
    bool followEdge ( Edge * pStartEdge, std::map<Edge*,int> &mEdgeIndexMap, std::vector<bool> &vEdgesUsed, 
                     int & anzEdgesUsed, std::deque<GeoPoint> &vPoints, bool direction );

};

///////////////////////////////////////////////////////////////////////////////
//  Face                                                                     //
//  Nicht-redundante Speicherung eines Polygons                              //
///////////////////////////////////////////////////////////////////////////////
class Face
{
  public:
    Face ();
    virtual ~ Face();

  protected:
    FaceRing             * pAussenkontur;   //  Topologische Darstellung der Außenkontur
    std::vector<FaceRing*> vLoecher;        //  Topologische Darstellung der Innenkonturen
    PolygonGeo           * pPolygon;        //  Ausgangs-Polygon

  public:
    void setFace ( PolygonGeo * pPoly, bool ori, BRep * pBRep );

    PolygonGeo * getPolygon()  {  return pPolygon;  }
};

///////////////////////////////////////////////////////////////////////////////
//  FaceRing                                                                 //
//  Nicht-redundante Speicherung eines Ringes                                //
///////////////////////////////////////////////////////////////////////////////
class FaceRing
{
  public:
    FaceRing ( PolygonGeo * pPolygon );
    virtual ~ FaceRing();

  protected:
    std::vector<Edge*> vEdges;          //  Topologische Darstellung der Liniensegmente 
    std::vector<bool>  vOrientations;   //  Zugehörige Laufrichtungen der Liniensegmente
    PolygonGeo * pPolygon;              //  Ausgangs-Polygon

  public:
    void setRing ( _Ring *pRing, bool ori, Face * pFace, BRep * pBRep );
};

///////////////////////////////////////////////////////////////////////////////
//  Edge                                                                     //
//  Von zwei Stützpunkten begrenztes Liniensegment                           //
///////////////////////////////////////////////////////////////////////////////
class Edge
{
  public:
    Edge ( KdTreeNode * pStartVertexP, KdTreeNode * pEndVertexP );
    virtual ~Edge();

  public:  
    KdTreeNode       * pStartVertex;   //  Start-Knoten
    KdTreeNode       * pEndVertex;     //  End-Knoten
    std::set<PolygonGeo*> sPolygons;   // Set von Polygonen, die die Edge berühren

  public:
    void addPolygon ( PolygonGeo * pPolygon );
		size_t getPolygons ( std::set<PolygonGeo*> & sPolygons );
};


///////////////////////////////////////////////////////////////////////////////
// 	 class KdTree     						    						                           //
//   kd-Tree zur Indizierung mehrdimensionaler Objekte vom Typ GeoPoint      //
///////////////////////////////////////////////////////////////////////////////

class  KdTree
{
  public:
    KdTree( int dimension = 3 );
    virtual ~KdTree();

  public:
    enum DISC  { X   = 0,  Y   = 1, Z   = 2, NO  = -1  };
    enum DIRECTION { LEFT, RIGHT };
    enum DIMENSION { ZWEI, DREI };

  protected:
    KdTreeNode	* root;                 //  Wurzel des Baumes
    DIMENSION	    dimension;            //  Raumdimension der Geometrie als Enumeration
    int				    dimensionNum;         //  Raumdimension der Geometrie als Zahl
    int				    nodeAnz;              //  Anzahl der Knoten im Raum
    double        minimalPointDistance; //  Minimaler Abstand zweier unterschiedlicher Punkte

  public:
    virtual KdTreeNode * generateNode  ( double point[],  bool checkExistence,  bool & newNode    );

    virtual void removeNode ( KdTreeNode * nodeToDelete  );
    virtual bool removeNode ( double point[] );

    virtual KdTreeNode * retrieveNode ( double point[] );

    virtual void deleteNode	( KdTreeNode * nodeToDelete );

    virtual bool nodeExist ( double point[] );

    KdTreeNode * getFirstNode ( KdTreeCursor * cur );
    KdTreeNode * getNextNode  ( KdTreeCursor * cur );

    int	getNodeCount()  {  return nodeAnz;  }
    bool isEmpty();

    int	getDimension()  {  return dimensionNum;  }

    void setMinimalPointDistance ( double minimalPointDistanceP ) {  minimalPointDistance = minimalPointDistanceP;  }
    double getMinimalPointDistance ()  {  return minimalPointDistance;  }

    std::vector<KdTreeNode*> queryNeighbourPoints  ( double * point, double dist, DISC ignoreDim= NO  );
    void queryNeighbourPointsRecursive ( double * point, double dist,  KdTreeNode * node, DISC ignoreDim, std::vector<KdTreeNode*> & nodeList );
  
    void offset ( GeoPoint & point );
  
protected:
    KdTreeNode * findMinimum  ( KdTreeNode * rootNode,  DISC disc, double & minimum  );

    bool equality  ( KdTreeNode * node, double point[] );

    DIRECTION kdCompare ( KdTreeNode * node, double point[] );

    DISC nextDisc  ( KdTreeNode * node );
    double getDiscValue ( KdTreeNode * node, DISC disc );

    KdTreeNode * deleteNodeRecursive ( KdTreeNode * nodeToDelete  );
};


///////////////////////////////////////////////////////////////////////////////
// 	 class KdTreeNode					    						                               //
//   Knoten in einem  KdTree										                             //
///////////////////////////////////////////////////////////////////////////////
class   KdTreeNode
{
  public:
    KdTreeNode( int dim );

    KdTreeNode ( double	point[], KdTree::DISC disc, KdTreeNode * father, int dim  );
    virtual ~KdTreeNode();

  public:
    KdTree::DISC 	disc;     //  Hilfsvariable zum AUfbau des Baumes
    KdTreeNode  * father;   //  Vater-Knoten
    KdTreeNode  * lSon;     //  Erster (linker) Kind-Knoten
    KdTreeNode  * rSon;     //  Zweiter (rechter) Kind-Knoten

    double		      * point;     // Koordinaten des Punktes
    KdTree::DIRECTION	direction; // Hilfsvariable zum Aufbau des Baumes

    std::vector<Edge*> vEdges;

  public:
    virtual KdTreeNode * getSon ( KdTree::DIRECTION direction );
    virtual KdTreeNode * setSon ( KdTree::DIRECTION direction, double point[], KdTree::DISC  disk, int dimension );

    virtual void setNode  ( double point[], KdTree::DISC disc, KdTreeNode * father, int dimension );
    void deleteNodesRecursive();

    Edge * existEdge ( KdTreeNode * toNode, bool & orientation );

    virtual void addEdge ( Edge * pEdge )  {  vEdges.push_back( pEdge );  }
    
		size_t getEdgeAnz()  {  return vEdges.size();  }
    Edge * getEdge ( unsigned int index );

    GeoPoint getPoint( int dim );

    void offset ( GeoPoint & point, int dim );
};

///////////////////////////////////////////////////////////////////////////////
//  KdTreeCursor                                                             //
// 	Cursorklasse zur Iteration durch KdTrees      						               //
///////////////////////////////////////////////////////////////////////////////
class  KdTreeCursor
{
  public:
    KdTreeCursor ()  { node = NULL; }
    ~KdTreeCursor() {}

  public:
    KdTreeNode * node;  //  Aktueller Knoten
};

///////////////////////////////////////////////////////////////////////////////
//  EdgeMap                                                                  //
//  Speichert, wie häufig Edge-Objekte in einer bestimmten Orientierung      //
//  verwendet werden, und welche Faces damit verbúnden sind                  //
///////////////////////////////////////////////////////////////////////////////
class EdgeMap
{
  public:
    EdgeMap();
    virtual ~EdgeMap();

  protected:  
    std::map<Edge*,std::vector<EdgeMapItem>*> mEdgeMap;

  public:
    void addEdge ( Edge * pEdge, bool ori, Face * pFace );
    void createEdge ( Edge * pEdge, bool ori, Face * pFace );

		size_t size()  {  return mEdgeMap.size();  }

		size_t getEdges ( std::vector<Edge*> &vEdges );

		size_t getEdgeOrientations ( Edge * pEdge, std::vector<bool> &vOrientations );
		size_t getAdjacentFaces ( Edge * pEdge, std::vector<Face*> &vFaces );
};

///////////////////////////////////////////////////////////////////////////////
//  EdgeMapItem                                                              //
//  Datentyp der Werte einer EdgeMap                                         //
///////////////////////////////////////////////////////////////////////////////
class EdgeMapItem
{
  public: 
    EdgeMapItem ( bool oriP, Face * pFaceP );
    virtual ~ EdgeMapItem();


  public:
    Face * pFace;
    bool   orientation;
};






