#include "stdafx.h"

#include <vector>
#include <map>
#include <set>
#include <deque>
#include "math.h"
#include "Constant.h"
#include "Geometrie.h"
#include "Topologie.h"
#include "Checking.h"
#include "Convert.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  class TopolPoint                                                         //
//  Stützpunkt eines Ringes mit expliziter Speicherung der Half-Edges, die   //
//  diesen Stützpunkt beinhalten                                             //
//  Die Klasse ist derzeit auf den Spezialfall von 2 überlappenden oder      //
//  berührenden Ringen reduziert                                             //
///////////////////////////////////////////////////////////////////////////////
TopolPoint::TopolPoint ( double x, double y )
{
  m_point.set( x, y );
  m_location = UNKNOWN;
}

TopolPoint::TopolPoint ( GeoPoint p )
{
  m_point.set( p.getX(), p.getY() );
  m_location = UNKNOWN;
}

TopolPoint::~TopolPoint()
{

}

///////////////////////////////////////////////////////////////////////////////
//  class TopolHalfEdge                                                      //
//  Gerichtete Kante eines TopolRing                                         //
///////////////////////////////////////////////////////////////////////////////
TopolHalfEdge::TopolHalfEdge ( TopolPoint * pStartPoint )
{
  m_pStartPoint  = pStartPoint;
  m_pNextEdge    = NULL;
  m_pCrossEdge   = NULL;
  m_isDoubleEdge = false;
  m_isProcessed  = false;
}

TopolHalfEdge::TopolHalfEdge ( TopolPoint * pStartPoint, TopolHalfEdge * pNextEdge )
{
  m_pStartPoint  = pStartPoint;
  m_pNextEdge    = pNextEdge;
  m_pCrossEdge   = NULL;
  m_isDoubleEdge = false;
  m_isProcessed  = false;
}

TopolHalfEdge::TopolHalfEdge ( TopolHalfEdge * pEdge )
{
  m_pStartPoint  = new TopolPoint ( pEdge->getStartPoint()->getPoint() );
  m_pNextEdge    = NULL;
  m_pCrossEdge   = NULL;
  m_isDoubleEdge = false;
  m_isProcessed  = false;
}


TopolHalfEdge::~TopolHalfEdge()
{
  delete m_pStartPoint;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Endpunkt der Kante                                           //
///////////////////////////////////////////////////////////////////////////////
TopolPoint * TopolHalfEdge::getEndPoint()
{
  if ( m_pNextEdge != NULL )
    return m_pNextEdge->getStartPoint();
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Zwischenpunkt in die TopolHalfEdge ein, sofern der            //
//  Zwischenpunkt nicht mit dem Anfangs- oder Endpunkt der Kante             //
//  übereinstimmt. Returnwert ist die Anzahl der neuen Kanten                //
///////////////////////////////////////////////////////////////////////////////
int  TopolHalfEdge::insertCutPoint ( GeoPoint cutPoint, vector<TopolHalfEdge*> &vNewEdges, double minDist )
{
  double          dist1, dist2;
  GeoPoint        point1, point2;

  if ( m_pNextEdge == NULL ) return 0;
  
  point1 =  m_pStartPoint->getPoint();
  point2 = m_pNextEdge->getStartPoint()->getPoint();

  dist1 = point1.dist( cutPoint );
  dist2 = point2.dist( cutPoint );

  if ( dist1 < minDist || dist2 < minDist )
    return 0; 

  TopolPoint    * pNewTopolPoint = new TopolPoint ( cutPoint );
  pNewTopolPoint->setLocation ( ON_VERTEX );
  TopolHalfEdge * pNewTopolHalfEdge = new TopolHalfEdge ( pNewTopolPoint, m_pNextEdge );
  m_pNextEdge = pNewTopolHalfEdge;
  vNewEdges.push_back ( pNewTopolHalfEdge );

  return 1;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt zwei Zwischenpunkte in die TopolHalfEdge ein                        //
///////////////////////////////////////////////////////////////////////////////
int TopolHalfEdge::insertCutPoint ( GeoPoint cutPoint1, GeoPoint cutPoint2 , vector<TopolHalfEdge*> &vNewEdges, double minDist )
{
  GeoPoint startPoint;
  GeoPoint endPoint;
  double   dist1, dist2;

  if ( m_pNextEdge == NULL ) return 0;

  startPoint = m_pStartPoint->getPoint();
  endPoint   = m_pNextEdge->getStartPoint()->getPoint();

  if ( cutPoint1.dist( cutPoint2 ) < minDist )
    return insertCutPoint( cutPoint1, vNewEdges, minDist );

  if (  startPoint.dist ( cutPoint1 ) < minDist || 
        endPoint.dist( cutPoint1 ) < minDist )
  {
    return insertCutPoint( cutPoint2, vNewEdges, minDist );
  }

  if (  startPoint.dist( cutPoint2 ) < minDist || 
        endPoint.dist ( cutPoint2 ) < minDist )
  {
    return insertCutPoint( cutPoint1, vNewEdges, minDist );
  }

  dist1 = startPoint.dist ( cutPoint1 );
  dist2 = startPoint.dist ( cutPoint2 );

  if ( dist1 < dist2 )
  {
    insertCutPoint( cutPoint1, vNewEdges, minDist );
    vNewEdges[vNewEdges.size()-1]->insertCutPoint( cutPoint2, vNewEdges, minDist );
  }
  else
  {
    insertCutPoint( cutPoint2, vNewEdges, minDist );
    vNewEdges[vNewEdges.size()-1]->insertCutPoint( cutPoint1, vNewEdges, minDist );
  }

  return 2;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, ob der Startpunkt der beiden Edges identisch ist und setzt in  //
//  diesem Fall die Attribute m_pCrossEdge                                   //
//  Wenn die beiden Edhges identisch, aber gegenläufig sind wird das Attribut//
//  isDoubleEdge jeweils auf true gesetzt                                    //
///////////////////////////////////////////////////////////////////////////////
void TopolHalfEdge::cross ( TopolHalfEdge * pEdge, double minDist )
{
  GeoPoint startPoint, endPoint, startPointEdge, endPointEdge;

  startPoint     = m_pStartPoint->getPoint();
  endPoint       = m_pNextEdge->getStartPoint()->getPoint();
  startPointEdge = pEdge->getStartPoint()->getPoint();
  endPointEdge   = pEdge->getNextEdge()->getStartPoint()->getPoint();

  if ( startPoint.dist ( startPointEdge ) < minDist )
  {
    m_pCrossEdge = pEdge;
    pEdge->setCrossEdge( this );
    m_pStartPoint->setLocation( ON_VERTEX );
    pEdge->getStartPoint()->setLocation( ON_VERTEX );
  }
  else
  if ( startPoint.dist ( endPointEdge ) < minDist )  
  {
    m_pStartPoint->setLocation( ON_VERTEX );
    pEdge->getNextEdge()->setCrossEdge( this );
    m_pCrossEdge = pEdge->getNextEdge();
    if ( endPoint.dist ( startPointEdge ) < minDist )
    {
      pEdge->getStartPoint()->setLocation( ON_VERTEX );
      m_isDoubleEdge = true;
      pEdge->setIsDoubleEdge( true );
    }
  }
  else
  if ( endPoint.dist ( startPointEdge ) < minDist )
  {
    m_pNextEdge->setCrossEdge( pEdge );
    pEdge->setCrossEdge( m_pNextEdge );
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Schnittpunkte beider Edges, fügt die gemeinsamen Punkte     //
//  in beide Edges ein und setzt geg. die Attribute m_pCrossEdge und         //
//  isDoubleEdge in beiden Edges                                             //
///////////////////////////////////////////////////////////////////////////////
bool TopolHalfEdge::merge ( TopolHalfEdge * pEdge, double minDist, double minAngleDist )
{
  GeoPoint         startPoint, endPoint, startPointEdge, endPointEdge;
  unsigned int     i, j;
  vector<GeoPoint> vCutPoints;
  vector<TopolHalfEdge*> vNewEdges;
  vector<TopolHalfEdge*> vNewMergeEdges;
  CUT_RESULT             cutResult;

  if ( m_pNextEdge == NULL || pEdge->getNextEdge() == NULL ) return false;

  startPoint     = m_pStartPoint->getPoint();
  endPoint       = m_pNextEdge->getStartPoint()->getPoint();
  startPointEdge = pEdge->getStartPoint()->getPoint();
  endPointEdge   = pEdge->getNextEdge()->getStartPoint()->getPoint();

  cutResult = Geometrie::cutPoints ( startPoint.getX(), startPoint.getY(), endPoint.getX(), endPoint.getY(),
                         startPointEdge.getX(), startPointEdge.getY(), endPointEdge.getX(), endPointEdge.getY(),
                         minDist, minAngleDist, vCutPoints );

  if ( cutResult == NO_CUT ) return true;


  if ( cutResult == POINT_ON_EDGE || cutResult == POINT_ON_POINT || cutResult == TRUE_CUT )
  {
    insertCutPoint( vCutPoints[0], vNewEdges, minDist );
    pEdge->insertCutPoint( vCutPoints[0], vNewMergeEdges, minDist );
  }
  else
  {
    insertCutPoint( vCutPoints[0], vCutPoints[1], vNewEdges, minDist );
    pEdge->insertCutPoint( vCutPoints[0], vCutPoints[1], vNewMergeEdges, minDist );
  }

  vNewEdges.push_back( this );
  vNewMergeEdges.push_back ( pEdge );

  for ( i = 0; i < vNewEdges.size(); i++ )
    for ( j = 0; j < vNewMergeEdges.size(); j++ )
      vNewEdges[i]->cross ( vNewMergeEdges[j], minDist );

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Winkel der Edge gegen die x-Achse                            //
///////////////////////////////////////////////////////////////////////////////
double TopolHalfEdge::getAngle( TopolHalfEdge * pEdge )
{
  GeoPoint pa = m_pStartPoint->getPoint();
  GeoPoint pe = getEndPoint()->getPoint();
  GeoPoint qa = pEdge->getStartPoint()->getPoint();
  GeoPoint qe = pEdge->getEndPoint()->getPoint();
  GeoPoint p = pe - pa;
  GeoPoint q = qe - qa;
  double   h;

  h = p.getX()*q.getY() - q.getX()*p.getY(); 
  return h;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert, ob die Edge als CrossEdge gekennzeichnet ist                    //
///////////////////////////////////////////////////////////////////////////////
bool TopolHalfEdge::getIsCrossEdge()
{
  if ( m_pCrossEdge != NULL )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  TopolRing                                                                //
//  Repräsentation eines _Ring mit Topologie-Informationen                   //
///////////////////////////////////////////////////////////////////////////////
TopolRing::TopolRing()
{
  m_pStartEdge             = NULL;
  m_pOriginalRing          = NULL;
  m_OriginalRingGenerated  = false;
  m_minDist       = MINIMAL_POINT_DISTANCE;
  m_minAngleDist  = MINIMAL_ANGLE_DISTANCE;
}

TopolRing::TopolRing ( TopolRing * pRing )
{
  TopolHalfEdge * pNextEdge;
  TopolHalfEdge * pEdge;

  m_minDist               = pRing->m_minDist;
  m_minAngleDist          = pRing->m_minAngleDist;
  m_pOriginalRing         = NULL;
  m_OriginalRingGenerated = false;

  m_pStartEdge = new TopolHalfEdge ( pRing->getStartEdge() );
  pEdge        = m_pStartEdge;

  pNextEdge = pRing->getStartEdge()->getNextEdge();
  while ( pNextEdge != pRing->getStartEdge() )
  {
    TopolHalfEdge * pNewEdge = new TopolHalfEdge ( pNextEdge );
    pEdge->setNextEdge( pNewEdge );   
    pNextEdge = pNextEdge->getNextEdge();
    pEdge = pNewEdge;
  }

  pEdge->setNextEdge ( m_pStartEdge );
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert den TopolRing aus einer Menge von TopolHalfEdge Objekte        //
///////////////////////////////////////////////////////////////////////////////
TopolRing::TopolRing ( std::vector<TopolHalfEdge*> vEdges )
{
  TopolHalfEdge * pEdge;
  unsigned int    i;

  m_minDist               = MINIMAL_POINT_DISTANCE;
  m_minAngleDist          = MINIMAL_ANGLE_DISTANCE;
  m_pOriginalRing         = NULL;
  m_OriginalRingGenerated = false;

  m_pStartEdge = new TopolHalfEdge ( vEdges[0] );
  pEdge        = m_pStartEdge;

  for ( i = 1; i < vEdges.size(); i++ )
  {
    TopolHalfEdge * pNewEdge = new TopolHalfEdge ( vEdges[i] );
    pEdge->setNextEdge( pNewEdge );
    pEdge = pNewEdge;
  }
  pEdge->setNextEdge( m_pStartEdge );
}

TopolRing::~TopolRing()
{
  TopolHalfEdge * pNextEdge;

  pNextEdge = m_pStartEdge->getNextEdge();
  while ( pNextEdge != m_pStartEdge )
  {
    TopolHalfEdge * pEdge = pNextEdge->getNextEdge();
    delete pNextEdge;
    pNextEdge = pEdge;
  }
  delete m_pStartEdge;

  if ( m_OriginalRingGenerated )
    delete m_pOriginalRing;
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt einen TopolRing aus einem _Ring                                  //
///////////////////////////////////////////////////////////////////////////////
bool TopolRing::create ( _Ring * pRing )
{
  vector<double>  vX, vY;
	size_t          i, j, anz;
  TopolPoint    * pStartPoint;
  TopolHalfEdge * pEdge = nullptr;
  TopolHalfEdge * pPrevEdge = nullptr;

  m_pOriginalRing = pRing;

  anz = pRing->getPoints2D( vX, vY );
  for ( i = 0; i < vX.size()-1; i++ )
  {
    if ( sqrt ( (vX[i]-vX[i+1])*(vX[i]-vX[i+1]) + (vY[i]-vY[i+1])*(vY[i]-vY[i+1]) ) < m_minDist )
    {
      for ( j = i+1; j < vX.size(); j++ )
      {
        vX[j-1] = vX[j];
        vY[j-1] = vY[j];
      }
      vX.pop_back();
      vY.pop_back();
    }
  }
  anz = vX.size();

  for ( i = 0; i < anz-1; i++ )
  {
    pStartPoint = new TopolPoint ( vX[i], vY[i] );
    pEdge = new TopolHalfEdge ( pStartPoint );
    if ( i > 0 )
      pPrevEdge->setNextEdge( pEdge );
    else
      m_pStartEdge = pEdge;
    pPrevEdge = pEdge;
  }
  pEdge->setNextEdge( m_pStartEdge );
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt einen LinearRing aus dem TopolRing                               //
///////////////////////////////////////////////////////////////////////////////
LinearRing * TopolRing::createLinearRing()
{
  vector<GeoPoint> vPoints;
  GeoPoint         point;
  LinearRing     * pLinearRing;
	size_t           i, anz;

  pLinearRing = new LinearRing ( 3 );

  anz = getPoints ( vPoints );
  for ( i = 0; i < anz; i++ )
    vPoints[i].setDim( 3 );

  pLinearRing->addPoints( vPoints );
  return pLinearRing;
}

void TopolRing::createOriginalRing()
{
  m_pOriginalRing = createLinearRing();
  m_OriginalRingGenerated = true;
}

///////////////////////////////////////////////////////////////////////////////
//  Verschneidet 2 ToploRing Objekte untereinander, so dass an allen         //
//  Berührungs- oder Überlappungsstellen gemeinsame Stützpunkte              //
//  eingeführt werden                                                        //
//  Die Lage der Stützpunkte zum bzgl. des zu mergenden Polygons wird        //
//  bestimmt                                                                 //
///////////////////////////////////////////////////////////////////////////////
void TopolRing::merge ( TopolRing * pMergeRing )
{
  TopolHalfEdge * pEdge;
  TopolHalfEdge * pMergeEdge;
  _Ring         * pOriginalMergeRing;
  GeoPoint        point;
  POINT_LOCATION  loc = UNKNOWN;
  vector<string>  vPoints;
  vector<string>  vMergePoints;

  pEdge = m_pStartEdge;
  pMergeEdge = pMergeRing->getStartEdge();

  debug( vPoints );
  pMergeRing->debug( vMergePoints );

  if ( m_pOriginalRing == NULL )
    createOriginalRing();  

  if ( pMergeRing->getOriginalRing() == NULL )
    pMergeRing->createOriginalRing();
  pOriginalMergeRing = pMergeRing->getOriginalRing();
  
  while ( true )
  {
    loc = pOriginalMergeRing->checkInside2D( pEdge->getStartPoint()->getPoint(), m_minDist, true );
    pEdge->getStartPoint()->setLocation( loc );

    while ( true )
    {
      loc = m_pOriginalRing->checkInside2D( pMergeEdge->getStartPoint()->getPoint(), m_minDist, true );
      pMergeEdge->getStartPoint()->setLocation( loc );
      pEdge->merge( pMergeEdge, m_minDist, m_minAngleDist );

      pMergeEdge = pMergeEdge->getNextEdge();
      if ( pMergeEdge == pMergeRing->getStartEdge() ) break;
    }
    pEdge = pEdge->getNextEdge();
    if ( pEdge == m_pStartEdge ) break;
  }
  vPoints.clear();
  vMergePoints.clear();

  debug( vPoints );
  pMergeRing->debug( vMergePoints );
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die gemeinsame Fläche (Mengenmäßiger Durchchnitt) der beiden    //
//  Ringe                                                                    //
//  Es wird vorausgesetzt, dass die Methode merge für beide Ringe ausge-     //
//  führt wurde                                                              //
///////////////////////////////////////////////////////////////////////////////
size_t TopolRing::cut ( TopolRing * pCutRing, std::vector<TopolRing*> &vResultRings )
{
  TopolHalfEdge * pEdge;
  TopolRing     * pResultRing;
  CUT_FOLLOW      cutFollow = SELF;
  int             loc = 0;

  pEdge = m_pStartEdge;

  while ( true )
  {
    if ( !pEdge->getIsProcessed() && !pEdge->getIsDoubleEdge() )
    {
      loc = pEdge->getStartPoint()->getLocation();
      if ( loc == IN_POLYGON )
      {
        pResultRing = followCutEdge ( pEdge );
        if ( pResultRing != NULL )
          vResultRings.push_back( pResultRing );
      }
      else
      if ( ( loc == ON_VERTEX || loc == ON_BOUNDARY ) && 
           ( pEdge->getCrossEdge() == NULL || pEdge->getCrossEdge()->getIsProcessed() == false ) )
      {
        cutFollow = checkCutFollow ( pEdge );
        if ( cutFollow == SELF )
        {
          pResultRing = followCutEdge ( pEdge );
          if ( pResultRing != NULL )
            vResultRings.push_back( pResultRing );
        }
        else
        if ( cutFollow == CROSS )
        {
          pResultRing = followCutEdge ( pEdge->getCrossEdge() );
          if ( pResultRing != NULL )
            vResultRings.push_back( pResultRing );
        }
      }
    }
    pEdge = pEdge->getNextEdge();
    if ( pEdge == m_pStartEdge )
      break; 
  }

  if ( vResultRings.size() == 0 )
  {
    loc = pCutRing->getStartEdge()->getStartPoint()->getLocation();
    if ( loc == IN_POLYGON )
    {
      vResultRings.push_back( new TopolRing ( pCutRing ) );
    }
  }
  return vResultRings.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Differenz der beiden  Ringe                                 //
//  Es wird vorausgesetzt, dass die Methode merge für beide Ringe ausge-     //
//  führt wurde                                                              //
///////////////////////////////////////////////////////////////////////////////
size_t TopolRing::difference (  TopolRing * pDiffRing, std::vector<TopolRing*> &vResultRings )
{
  TopolHalfEdge * pEdge;
  TopolRing     * pResultRing;
  int             loc = 0;

  pEdge = m_pStartEdge;

  while ( true )
  {
    if ( !pEdge->getIsProcessed() && !pEdge->getIsDoubleEdge() )
    {
      loc = pEdge->getStartPoint()->getLocation();
      if ( loc == OUTSIDE_POLYGON )
      {
        pResultRing = followDiffEdge ( pEdge );
        if ( pResultRing != NULL )
          vResultRings.push_back( pResultRing );
      } 
    }
    pEdge = pEdge->getNextEdge();
    if ( pEdge == m_pStartEdge )
      break; 
  }

  return vResultRings.size();
}

////////////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Vereinigungsfläche der beiden Ringe                              //
//  Es wird vorausgesetzt, dass die Methode merge für beide Ringe ausge-          //
//  führt wurde                                                                   //
//  Der return-Wert ist NULL, wenn beide Ringe sich an max. einem Punkt berühren  //
////////////////////////////////////////////////////////////////////////////////////
size_t TopolRing::_union (  TopolRing * pRing, std::vector<TopolRing*> &vResultRings )
{
  unsigned int    anzIPSelf, anzBPSelf, anzOPSelf;
  unsigned int    anzIPOther, anzBPOther, anzOPOther;
  TopolHalfEdge * pEdge;
  TopolHalfEdge * pStartEdge = nullptr;
  TopolRing     * pUnionRing;
//  vector<GeoPoint> vPoints;
//  vector<GeoPoint> vOtherPoints;
//  vector<GeoPoint> vResultPoints;

  getPointClassifications( anzIPSelf, anzBPSelf, anzOPSelf );
  pRing->getPointClassifications( anzIPOther, anzBPOther, anzOPOther );

//  show( vPoints );
//  pRing->show ( vOtherPoints );

  if ( anzIPSelf + anzBPSelf <= 1 && anzIPOther + anzBPOther <= 1  )
    return 0;

  if ( anzOPSelf == 0 && anzIPSelf > 0 && anzIPOther == 0 )
  {
    pUnionRing = new TopolRing ( pRing );
    vResultRings.push_back( pUnionRing );
    return 1;
  }

  if ( anzIPSelf == 0 && anzOPOther == 0 && anzIPOther > 0 )
  {
    pUnionRing = new TopolRing ( this );
    vResultRings.push_back( pUnionRing );
    return 1;
  }

  bool weiter = true;
  while ( weiter )
  {
    pEdge = m_pStartEdge;
    bool startEdgeFound = false;
    while ( !startEdgeFound )
    {
      if ( !pEdge->getIsProcessed() && pEdge->getIsCrossEdge()  )
      {
        pStartEdge = pEdge;
        startEdgeFound = true;
      }
      else
      {
        pEdge = pEdge->getNextEdge();
        if ( pEdge == m_pStartEdge )
          break;
      }
    }
    if ( !startEdgeFound )
      break;

    pStartEdge->setIsProcessed( true );

    if ( pStartEdge->getNextEdge()->getStartPoint()->getLocation() == OUTSIDE_POLYGON )
    {
      pUnionRing = followUnionEdge ( pStartEdge );
//      pUnionRing->show ( vResultPoints );
      vResultRings.push_back( pUnionRing );
    }
    else
    if ( pStartEdge->getCrossEdge()->getNextEdge()->getStartPoint()->getLocation() == OUTSIDE_POLYGON )
    {
      pUnionRing = followUnionEdge ( pStartEdge->getCrossEdge() );
//      pUnionRing->show ( vResultPoints );
      vResultRings.push_back( pUnionRing );     
    }    
  }
//  vResultRings[0]->show ( vResultPoints );

  if ( vResultRings.size() > 1 )
  {

    for ( unsigned int i = 0; i < vResultRings.size(); i++ )
    {
//      vResultRings[i]->show ( vResultPoints );
      double area = vResultRings[i]->getArea();
      if ( area > 0 )
      {
        if ( i > 0 )
        {
          TopolRing * pTopolRing = vResultRings[0];
          vResultRings[0] = vResultRings[i];
          vResultRings[i] = pTopolRing;
        }
        break;
      } 
    }
  }

  return vResultRings.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Hilfsroutine, Kantenverfolgung bei der Vereinigung von Ringen            //
///////////////////////////////////////////////////////////////////////////////
TopolRing * TopolRing::followUnionEdge ( TopolHalfEdge * pEdge )
{
  TopolHalfEdge        * pStartEdge = pEdge;
  TopolPoint           * pStartPoint = pStartEdge->getStartPoint();
  vector<TopolHalfEdge*> vEdges;
  
  pEdge->setIsProcessed( true );
  vEdges.push_back( pEdge );

  while ( true )
  {
    pEdge = pEdge->getNextEdge();
    if ( pEdge->getIsProcessed() == true )
      break;

    if ( pEdge->getStartPoint()->getPoint() == pStartPoint->getPoint() )
      break;

    if ( pEdge->getIsCrossEdge() )
    {
      if ( !pEdge->getIsProcessed() &&
            pEdge->getNextEdge()->getStartPoint()->getLocation() != IN_POLYGON &&             
            pEdge->getCrossEdge()->getNextEdge()->getStartPoint()->getLocation() != OUTSIDE_POLYGON )
      {
        vEdges.push_back( pEdge );
        pEdge->setIsProcessed( true );
      }
      else    
      {
        pEdge->setIsProcessed( true );
        pEdge = pEdge->getCrossEdge();
        vEdges.push_back( pEdge );
        pEdge->setIsProcessed( true );
      }      
    }
    else
    {
      vEdges.push_back( pEdge );
      pEdge->setIsProcessed( true );
    }  
  }

  TopolRing * pRing = new TopolRing ( vEdges );
  return pRing;
}

///////////////////////////////////////////////////////////////////////////////
//  Extrahiert alle als "doubleEdge" gekennzeichnete Kanten                  // 
///////////////////////////////////////////////////////////////////////////////
size_t TopolRing::getDoubleEdges ( std::vector<TopolHalfEdge*> &vDoubleEdges )
{
  TopolHalfEdge * pNextEdge;

  if ( m_pStartEdge->getIsDoubleEdge() )
    vDoubleEdges.push_back( m_pStartEdge );
  pNextEdge = m_pStartEdge->getNextEdge();

  while ( pNextEdge != m_pStartEdge )
  {
    if ( pNextEdge->getIsDoubleEdge() )
      vDoubleEdges.push_back( pNextEdge );
    pNextEdge = pNextEdge->getNextEdge();
  }

  return vDoubleEdges.size();
}

/////////////////////////////////////////////////////////////////////////////////
//  Extrahiert alle Startpunkte von Kanten, die als "CrossEdge" gekennzeichnet //
//  sind und keine "DoubleEdges" sind.                                         //
/////////////////////////////////////////////////////////////////////////////////
size_t TopolRing::getCrossPoints ( std::vector<TopolPoint*> &vCrossPoints )
{
  TopolHalfEdge * pNextEdge;

  if ( m_pStartEdge->getIsCrossEdge() && !m_pStartEdge->getIsDoubleEdge() && !m_pStartEdge->getCrossEdge()->getIsDoubleEdge() )
    vCrossPoints.push_back( m_pStartEdge->getStartPoint() );
  pNextEdge = m_pStartEdge->getNextEdge();

  while ( pNextEdge != m_pStartEdge )
  {
    if ( pNextEdge->getIsCrossEdge() && !pNextEdge->getIsDoubleEdge() && !pNextEdge->getCrossEdge()->getIsDoubleEdge() )
      vCrossPoints.push_back(  pNextEdge->getStartPoint() );
    pNextEdge = pNextEdge->getNextEdge();
  }

  return vCrossPoints.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmung aller Kanten des Flächendurchschnitts                         //
//  Folgt der Kante pEdge im Innern bzw. auf dem Rand der Ringe, bis die     //
//  Ausgangskante wieder erreicht ist                                        //
///////////////////////////////////////////////////////////////////////////////
TopolRing * TopolRing::followCutEdge ( TopolHalfEdge * pEdge )
{
  vector<TopolHalfEdge*> vEdges;
  CUT_FOLLOW             cutFollow = SELF;
  TopolHalfEdge        * pStartEdge;

  pStartEdge = pEdge;

  while ( true )
  {
    cutFollow = checkCutFollow ( pEdge );

    if ( cutFollow == SELF )
    {
      vEdges.push_back( pEdge );
      pEdge->setIsProcessed( true );
      pEdge = pEdge->getNextEdge();
    }
    else
    {
      if ( cutFollow == CROSS )
      {
        pEdge = pEdge->getCrossEdge();
        vEdges.push_back( pEdge );
        pEdge->setIsProcessed( true );
        pEdge = pEdge->getNextEdge();
      }
      else
        break;
    }
   
    if ( pEdge == pStartEdge || (  pEdge->getCrossEdge() != NULL && pEdge->getCrossEdge() == pStartEdge ) ) break;
  }

  if ( vEdges.size() == 0 )
    return NULL;

  if ( pEdge == pStartEdge || (  pEdge->getCrossEdge() != NULL && pEdge->getCrossEdge() == pStartEdge ) )
    return new TopolRing ( vEdges );
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmung aller Kanten der Flächendifferenz                             //
//  Folgt der Kante pEdge außerhalb bzw. auf dem Rand der Ringe, bis die     //
//  Ausgangskante wieder erreicht ist                                        //
///////////////////////////////////////////////////////////////////////////////
TopolRing * TopolRing::followDiffEdge ( TopolHalfEdge * pEdge )
{
  vector<TopolHalfEdge*> vEdges;
  int                    cutFollow = SELF;
  TopolHalfEdge        * pStartEdge;
  TopolHalfEdge        * pPrevEdge;

  pStartEdge = pEdge;
  pPrevEdge  = NULL;

  while ( true )
  {
    cutFollow = checkDiffFollow ( pEdge, pPrevEdge );

    if ( cutFollow == SELF )
    {
      vEdges.push_back( pEdge );
      pEdge->setIsProcessed( true );
      pPrevEdge = pEdge;
      pEdge = pEdge->getNextEdge();
    }
    else
    {
      if ( cutFollow == CROSS )
      {
        pEdge = pEdge->getCrossEdge();
        vEdges.push_back( pEdge );
        pEdge->setIsProcessed( true );
        pPrevEdge = pEdge;
        pEdge = pEdge->getNextEdge();
      }
      else
        break;
    }

    if ( pEdge == pStartEdge || (  pEdge->getCrossEdge() != NULL && pEdge->getCrossEdge() == pStartEdge ) ) break;
  }

  if ( vEdges.size() == 0 )
    return NULL;

  if ( pEdge == pStartEdge || (  pEdge->getCrossEdge() != NULL && pEdge->getCrossEdge() == pStartEdge ) )
    return new TopolRing ( vEdges );
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmung aller Kanten des Flächendurchschnitts                         //
//  Bestimmt bei Verzweigungspunkten, welchem Zweig gefolgt wird             //
///////////////////////////////////////////////////////////////////////////////
TopolRing::CUT_FOLLOW TopolRing::checkCutFollow ( TopolHalfEdge * pEdge )
{
  TopolHalfEdge * pCrossEdge               = pEdge->getCrossEdge();
  TopolPoint    * pEndPoint                 = pEdge->getEndPoint();
  TopolPoint    * pEndPointCrossEdge;
  int             endPointLocation          = pEndPoint->getLocation();
  int             endPointCrossEdgeLocation = 0;
  bool            useEdge                   = !pEdge->getIsDoubleEdge()&& !pEdge->getIsProcessed();
  bool            useCrossEdge = false;

  if ( pCrossEdge != NULL )
  {
    pEndPointCrossEdge        = pCrossEdge->getEndPoint();
    endPointCrossEdgeLocation = pEndPointCrossEdge->getLocation();
    useCrossEdge              = !pCrossEdge->getIsDoubleEdge()&& !pCrossEdge->getIsProcessed();
  }
  else
  {
    pEndPointCrossEdge        = NULL;
    endPointCrossEdgeLocation = UNKNOWN;
    useCrossEdge              = false;
  }

  if ( !useEdge && !useCrossEdge ) return END;

  if ( endPointLocation == IN_POLYGON ||
       endPointLocation == ON_VERTEX  ||
       endPointLocation == ON_BOUNDARY )
  {
    if ( !useCrossEdge )
      return SELF;
    else
    if ( endPointCrossEdgeLocation == OUTSIDE_POLYGON )
    {
      if ( useEdge )
        return SELF;
      else
        return END;
    }
    else
    if ( endPointCrossEdgeLocation == IN_POLYGON ||
         endPointCrossEdgeLocation == ON_VERTEX || 
         endPointCrossEdgeLocation == ON_BOUNDARY  )
    {
      if ( pEdge->getAngle( pCrossEdge )< 0 )
      {
        if ( useEdge ) return SELF;
        else           return CROSS;
      }
      else
      {
        if ( useCrossEdge ) return CROSS;
        else                return SELF;
      }
    }
  }
  else
  {
    if ( endPointCrossEdgeLocation == IN_POLYGON ||
         endPointCrossEdgeLocation == ON_VERTEX || 
         endPointCrossEdgeLocation == ON_BOUNDARY  )
    {
      if ( useCrossEdge )
        return CROSS;
      else
        return END;
    }
    else
    {
      return END;
    }
  }
  return END; 
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmung aller Kanten der Flächendifferenz                             //
//  Bestimmt bei Verzweigungspunkten, welchem Zweig gefolgt wird             //
///////////////////////////////////////////////////////////////////////////////
TopolRing::CUT_FOLLOW TopolRing::checkDiffFollow ( TopolHalfEdge * pEdge, TopolHalfEdge * pPrevEdge )
{
  TopolHalfEdge * pCrossEdge               = pEdge->getCrossEdge();
  TopolPoint    * pEndPoint                 = pEdge->getEndPoint();
  TopolPoint    * pEndPointCrossEdge;
  int             endPointLocation          = pEndPoint->getLocation();
  int             endPointCrossEdgeLocation = 0;
  bool            useEdge                   = !pEdge->getIsDoubleEdge()&& !pEdge->getIsProcessed();
  bool            useCrossEdge = false;
  double          h1[2], h2[2], h3[2], h4[2];

  if ( pCrossEdge != NULL )
  {
    pEndPointCrossEdge        = pCrossEdge->getEndPoint();
    endPointCrossEdgeLocation = pEndPointCrossEdge->getLocation();
    useCrossEdge              = !pCrossEdge->getIsDoubleEdge()&& !pCrossEdge->getIsProcessed();
  }
  else
  {
    pEndPointCrossEdge        = NULL;
    endPointCrossEdgeLocation = UNKNOWN;
    useCrossEdge              = false;
  }

  if ( !useEdge && !useCrossEdge ) return END;

  if ( !useEdge ) return CROSS;
  else
  if ( !useCrossEdge ) return SELF;
  else
  {
    if ( pPrevEdge != NULL )
    {
      h1[0] = pPrevEdge->getStartPoint()->getPoint().getX();
      h1[1] = pPrevEdge->getStartPoint()->getPoint().getY();
      h2[0] = pPrevEdge->getEndPoint()->getPoint().getX();
      h2[1] = pPrevEdge->getEndPoint()->getPoint().getY();
      h3[0] = pEdge->getEndPoint()->getPoint().getX();
      h3[1] = pEdge->getEndPoint()->getPoint().getY();
      h4[0] = pCrossEdge->getEndPoint()->getPoint().getX();
      h4[1] = pCrossEdge->getEndPoint()->getPoint().getY();


      if ( _Ring::calcWinkel2D ( h1, h2, h3 ) > _Ring::calcWinkel2D( h1, h2, h4 ) )
        return SELF;
      else
        return CROSS;
    }
    else
      return END;
  }

  return END; 
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert einen oder mehrere LinearRing aus dem TopolRing                //
///////////////////////////////////////////////////////////////////////////////
int TopolRing::createLinearRings( vector<LinearRing*> &vLinearRings )
{
  vector<GeoPoint> vPoints;
  vector<GeoPoint> vPointsInt;
  GeoPoint         point;
  LinearRing     * pLinearRing;
	size_t           i, j, k, anz;
  int              anzRing = 0;

  anz = getPoints ( vPoints );
  for ( i = 0; i < anz-1; i++ )
  {
    for ( j = i + 1; j < anz-1; j++ )
    {
      if ( vPoints[i].dist( vPoints[j] ) < m_minDist )
      {
        vPointsInt.clear();
        for ( k = i; k <= j; k++ )
          vPointsInt.push_back( vPoints[k] );
        if ( j > i+2 )
        {
          pLinearRing = new LinearRing ( 2 );
          pLinearRing->addPoints( vPointsInt );
          vLinearRings.push_back( pLinearRing );
          anzRing++;
        }
        for ( k = j+1; k < anz; k++ )
          vPoints[k-j+i] = vPoints[k];
        for ( k =  anz - (j-i); k < anz; k++ )
          vPoints.pop_back();
        anz = anz - (j-i);
      }
    }
  }

  pLinearRing = new LinearRing ( 2 );
  pLinearRing->addPoints( vPoints );
  vLinearRings.push_back( pLinearRing );
  return anzRing + 1; 
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Punkteliste des TopolRing                                    //
///////////////////////////////////////////////////////////////////////////////
size_t TopolRing::getPoints ( vector<GeoPoint> &vPoints )
{
  TopolHalfEdge * pEdge;
  GeoPoint        point;

  pEdge = m_pStartEdge;

  while ( true )
  {
    point = pEdge->getStartPoint()->getPoint();
    vPoints.push_back( point );
    pEdge = pEdge->getNextEdge();
    if ( pEdge == m_pStartEdge ) break;
  }

   vPoints.push_back ( pEdge->getStartPoint()->getPoint() );
   return vPoints.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Punkteliste des TopolRing als Text                           //
///////////////////////////////////////////////////////////////////////////////
std::string TopolRing::getPointsAsString ()
{
  vector<GeoPoint> vPoints;
  String         ^ STR;
  string           str, strX, strY;

	size_t anz = getPoints( vPoints );
  str = "";
  for ( size_t i = 0;  i < vPoints.size(); i++ )
  {
    GeoPoint p = vPoints[i];

    STR = Convert::ToString( p.getX() );
    QuConvert::systemStr2stdStr( strX, STR );
    STR = Convert::ToString( p.getY() );
    QuConvert::systemStr2stdStr( strY, STR );

    str = str + "(" + strX + ", " + strY + ")" + " ";
  }
  return str;
}

///////////////////////////////////////////////////////////////////////////////
//  Hilfsfunktion beim Debuggen                                              //
///////////////////////////////////////////////////////////////////////////////
size_t TopolRing::debug ( std::vector<std::string> & vTopolPoints )
{
  vector<GeoPoint> vPoints;
  String         ^ STR;
  string           str, strX, strY;
  TopolHalfEdge * pEdge;
  TopolPoint    * pTPoint;
  GeoPoint        p;
  string          crossPoint;
  string          location;

  pEdge = m_pStartEdge;

  while ( true )
  {
    if ( pEdge->getCrossEdge() != NULL )
      crossPoint = "Cross;";
    else
      crossPoint = "NoCross;";

    pTPoint = pEdge->getStartPoint();
    switch ( pTPoint->getLocation() )
    {
    case IN_POLYGON:  
      location = "inside";
      break;
    case OUTSIDE_POLYGON:
      location = "outside";
      break;
    case ON_BOUNDARY:
      location = "boundary";
      break;
    case ON_VERTEX:
      location = "vertex";
      break;
    }

    p = pEdge->getStartPoint()->getPoint();
    STR = Convert::ToString( p.getX() );
    QuConvert::systemStr2stdStr( strX, STR );
    STR = Convert::ToString( p.getY() );
    QuConvert::systemStr2stdStr( strY, STR );

    str = "(" + strX + ", " + strY + ")" + " ";
    str = str + crossPoint + location;
    vTopolPoints.push_back( str );
   
    pEdge = pEdge->getNextEdge();
    if ( pEdge == m_pStartEdge ) break;
  }
  return vTopolPoints.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte des TopolRing                              //
///////////////////////////////////////////////////////////////////////////////
size_t TopolRing::getPointAnz()
{
  TopolHalfEdge * pEdge;
  unsigned int    anz = 0;

  pEdge = m_pStartEdge;

  while ( true )
  {
    anz++;
    pEdge = pEdge->getNextEdge();
    if ( pEdge == m_pStartEdge ) break;
  }
  return anz+1;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Fläche des Ringes                                            //
///////////////////////////////////////////////////////////////////////////////
double TopolRing::getArea()
{
  vector<double>  vX, vY, vZ;
  double          xA, xE, yA, yE, flaeche;
  TopolHalfEdge * pEdge;

  pEdge = m_pStartEdge;

  while ( true )
  {
    vX.push_back( pEdge->getStartPoint()->getPoint().getX() );
    vY.push_back( pEdge->getStartPoint()->getPoint().getY() );

    pEdge = pEdge->getNextEdge();
    if ( pEdge == m_pStartEdge ) break;
  }

  flaeche = 0.0;
  for ( unsigned int i = 0; i < vX.size()-1; i++ )
  {
    xA = vX[i];
    xE = vX[i+1];
    yA = vY[i];
    yE = vY[i+1];

    flaeche = flaeche + 0.5*(xA - xE)*(yA + yE);
  }
  return flaeche;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Punkte, die als "innere Punkte", "Randpunkte" oder//
//  "äußere Punkte" klassifiziert sind                                       //
///////////////////////////////////////////////////////////////////////////////
void TopolRing::getPointClassifications ( unsigned int & anzInnerPoints, unsigned int & anzBoundaryPoints, unsigned int & anzOuterPoints )
{
  TopolHalfEdge * pEdge;
  int             location = 0;

  anzInnerPoints    = 0;
  anzBoundaryPoints = 0;
  anzOuterPoints    = 0;

  pEdge = m_pStartEdge;
  while ( true )
  {
    location = pEdge->getStartPoint()->getLocation();
    switch ( location )
    {
    case IN_POLYGON:
      anzInnerPoints++;
      break;

    case ON_BOUNDARY:
    case ON_VERTEX:
      anzBoundaryPoints++;
      break;

    case OUTSIDE_POLYGON:
      anzOuterPoints++;
      break;
    }
    pEdge = pEdge->getNextEdge();
    if ( pEdge == m_pStartEdge ) break;
  } 
}

///////////////////////////////////////////////////////////////////////////////
//  TopolRingSet                                                             //
//  Menge von TopolRing Objekten                                             //
///////////////////////////////////////////////////////////////////////////////
TopolRingSet::TopolRingSet ()
{

}

TopolRingSet::~TopolRingSet()
{
  set<TopolRing*>::iterator iter;

  for ( iter = sTopolRings.begin(); iter != sTopolRings.end(); iter++ )
    delete *iter;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die Vereinigungsmenge aller Ringe, wobei sich ergebende innere //
//  Ringe ignoriert werden                                                   //
///////////////////////////////////////////////////////////////////////////////
size_t TopolRingSet::_union ( vector<TopolRing*> &vUnionRings )
{
  set<TopolRing*>           sRingsToJoin;
  vector<TopolRing*>        vJointRings;
	size_t                    i, anz, anzResultRings;
  TopolRing               * pJointRing;
  vector<TopolRing*>        vResultRings;
  set<TopolRing*>::iterator iter;
//  vector<GeoPoint>          vPoints;
//  vector<GeoPoint>          vOtherPoints;
//  vector<GeoPoint>          vResultPoints;

  anz = sTopolRings.size();

  if ( anz == 0 )
    return 0;

  iter = sTopolRings.begin();
  if ( anz == 1 )
  {
    vUnionRings.push_back( new TopolRing ( *sTopolRings.begin() ) );
    return 1;
  }

  for ( iter = sTopolRings.begin(); iter != sTopolRings.end(); iter++ )
    sRingsToJoin.insert( new TopolRing ( *iter ) );

  bool weiter = true;
  while ( weiter )
  {
    pJointRing = *sRingsToJoin.begin();
    sRingsToJoin.erase( pJointRing );

    bool weiterA = true;
    while ( weiterA )
    {
      bool found = false;
//      pJointRing->show( vPoints );
      for ( iter = sRingsToJoin.begin(); iter != sRingsToJoin.end(); ++iter )
      {
        TopolRing * pRingToJoin = *iter;

        vResultRings.clear();
        pJointRing->merge( pRingToJoin );
//        pRingToJoin->show ( vOtherPoints );
        anzResultRings = pJointRing->_union( pRingToJoin, vResultRings );
        if ( anzResultRings > 0 )
        {
          found = true;
          sRingsToJoin.erase( *iter );
//          pRingToJoin->show ( vOtherPoints );
//          vResultRings[0]->show( vResultPoints );
          delete pJointRing;
          delete pRingToJoin;
          pJointRing = vResultRings[0];
          for ( i = 1; i < vResultRings.size(); i++ )
            delete vResultRings[i];
          break;
        }
      }

      if ( !found )
      {
        weiterA = false;
        vUnionRings.push_back( pJointRing );
        if ( sRingsToJoin.size() == 0 )
          break;

        if (  sRingsToJoin.size() == 1 )
        {
          vUnionRings.push_back( new TopolRing ( *sRingsToJoin.begin() ) );
          sRingsToJoin.erase( *sRingsToJoin.begin() );
          break;
        }
      }
      else
      {
        if ( sRingsToJoin.size() == 0 )
        {
          vUnionRings.push_back( pJointRing );
          break;
        }
      }
    }
    if ( sRingsToJoin.size() == 0 )
      break;
  }
   
  return vUnionRings.size();
}

///////////////////////////////////////////////////////////////////////////////
// 	 class KdTree					    								                               //
//   kd-Tree zur Indizierung zwei- und dreidimensionaler Punkte		      		 //
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//  Konstruktor																 //
///////////////////////////////////////////////////////////////////////////////

KdTree::KdTree( int idim )
{
  root	               = new KdTreeNode ( idim );
  nodeAnz	             = 0;
  minimalPointDistance = MINIMAL_POINT_DISTANCE;

  switch ( idim )
  {
  case 2:	  
    dimensionNum = 2;
    dimension    = ZWEI;
    break;

  case 3:
    dimensionNum = 3;
    dimension    = DREI;
    break;

  default:
    dimensionNum = 2;
    dimension    = ZWEI;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Destruktor																 //
///////////////////////////////////////////////////////////////////////////////
KdTree::~KdTree()
{
  if ( root != NULL )
    root->deleteNodesRecursive();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen neuen Punkt point in den Baum ein und generiert einen neuen   //
//  Knoten bzw. extrahiert den Knoten  aus dem Baum,  wenn schon ein Knoten	 //
//  mit denselben Koordinaten existiert.									                   //
//  Ist checkExistance == FALSE wird in jedem Fall ein neuer Knoten angelegt //
//  newPoint wird TRUE gesetzt, wenn ein neuer MGeoPoint erzeugt wurde       //
///////////////////////////////////////////////////////////////////////////////
KdTreeNode * KdTree::generateNode	 ( double point[], bool checkExistence, bool & newNode ) 
{
  vector<KdTreeNode*> nodeList;
  KdTreeNode	      * node = NULL;
  KdTreeNode	      * father = nullptr;
  DIRECTION		        direction;

  if ( root == NULL )
    root = new KdTreeNode ( dimensionNum );

  if ( nodeAnz == 0 )
  {	
    root->setNode ( point, X, 0, dimensionNum );
    nodeAnz++;
    newNode = true;
    return root;
  }

  if ( checkExistence )
  {
    nodeList = queryNeighbourPoints ( point, minimalPointDistance );
    if ( nodeList.size() > 0 )
      node = nodeList[0];
    else
      node = NULL;
  }

  if ( node != NULL )
  {	
    newNode = false;
    return node;
  }

  newNode = true;
  node = root;

  while ( node != NULL )
  {
    father = node;
    direction = kdCompare ( node, point );		
    node = node->getSon ( direction );
  }

  nodeAnz++;
  return father->setSon ( direction, point, nextDisc ( father ), dimensionNum );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den zug. kd-Tree Knoten zu point, bzw. den Nullpointer    			 //
///////////////////////////////////////////////////////////////////////////////
KdTreeNode * KdTree::retrieveNode ( double point[] )
{
  KdTreeNode	      * node;
  vector<KdTreeNode*> nodeList;

  if ( nodeAnz == 0 )	return NULL;

  nodeList = queryNeighbourPoints ( point, minimalPointDistance );
  if ( nodeList.size() > 0 )
    node = nodeList[0];
  else
    node = NULL;

  return node;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft ob es im Indexbaum einen Punkt mit gleichen Koordinaten wie  	 //
//  point gibt.																                               //
///////////////////////////////////////////////////////////////////////////////

bool  KdTree::nodeExist ( double point[] )
{
  vector<KdTreeNode*> nodeList;

  if ( nodeAnz == 0 )	return false;

  nodeList = queryNeighbourPoints ( point, minimalPointDistance );
  if ( nodeList.size() > 0 )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Startet eine Iteration durch den kd-Tree und liefert den ersten Knoten 	 //
///////////////////////////////////////////////////////////////////////////////
KdTreeNode * KdTree::getFirstNode( KdTreeCursor * cur )
{
  if ( nodeAnz == 0 )
  {
    cur->node = NULL;
    return NULL;
  }

  cur->node = root;
  return root;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert während einer Iteration durch den kd-tree den nächsten Knoten  	 //
///////////////////////////////////////////////////////////////////////////////
KdTreeNode *  KdTree::getNextNode( KdTreeCursor * cur )
{
  if ( cur->node == NULL )
    return NULL;

  if ( cur->node->lSon != NULL )
  {
    cur->node = cur->node->lSon;
    return cur->node;
  }

  if ( cur->node->rSon != NULL )
  {
    cur->node = cur->node->rSon;
    return cur->node;
  }

  // Backtracking bis ein noch nicht besuchter Knoten gefunden ist
  while ( cur->node->father != NULL )
  {
    DIRECTION iterDirection = cur->node->direction;
    cur->node = cur->node->father;
    if ( iterDirection == LEFT )
      if ( cur->node->rSon != NULL )
      {
        cur->node = cur->node->rSon;
        return cur->node;
      }
  }

  cur->node = NULL;
  return cur->node;
}


///////////////////////////////////////////////////////////////////////////////
//  Entfernt den zu point gehörigen Knoten aus dem kd-tree. Return-Wert ist  //
//  TRUE, wenn  der Punkt gefunden und entfernt wurde, sonst FALSE			 //
///////////////////////////////////////////////////////////////////////////////
bool KdTree::removeNode	(	double point[]	)
{
  KdTreeNode * nodeToDelete;

  nodeToDelete = retrieveNode ( point );
  if ( nodeToDelete == NULL )
    return false;

  deleteNode ( nodeToDelete );
  nodeAnz--;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt den  Knoten node aus dem kd-tree. Return-Wert ist  //
//  TRUE, wenn  der Punkt gefunden und entfernt wurde, sonst FALSE			 //
///////////////////////////////////////////////////////////////////////////////
void KdTree::removeNode (	KdTreeNode * nodeToDelete	)
{
  deleteNode ( nodeToDelete );
  nodeAnz--;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Gleichheit des im Knoten node gespeichert Punktes mit point//
///////////////////////////////////////////////////////////////////////////////
bool KdTree::equality	( KdTreeNode * node, double point[]	)
{
  double dist = 0.0;

  for ( int i = 0; i < dimensionNum; i++ )
    dist = dist +  fabs ( node->point[i] - point[i] );
  if ( dist <= minimalPointDistance )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft ob der Baum leer ist										  	 //
///////////////////////////////////////////////////////////////////////////////
bool KdTree::isEmpty()
{
  if ( nodeAnz == 0 )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Sucht alle Knoten, die in einer Entfernung dist von point liegen         //
///////////////////////////////////////////////////////////////////////////////
vector<KdTreeNode*> KdTree::queryNeighbourPoints  ( double * point, double dist, KdTree::DISC ignoreDim )
{
  double              pointDist;
  int                 i;
  vector<KdTreeNode*> nodeList;

  if ( nodeAnz == 0 )
    return nodeList;

  KdTreeNode * node = root;

  pointDist = 0;
  for ( i = 0; i < dimensionNum; i++ )
    if ( i != ignoreDim )
      pointDist = pointDist + ( node->point[i] - point[i] )*( node->point[i] - point[i] );

  pointDist = sqrt ( pointDist );

  if ( pointDist <= dist )
    nodeList.push_back ( node );

  if ( node->lSon != NULL )
  {
    if ( node->disc == KdTree::X   &&
      ( node->point[0] >= point[0] - dist || ignoreDim == X  ) )
      queryNeighbourPointsRecursive ( point, dist, node->lSon, ignoreDim,nodeList );

    if ( node->disc == KdTree::Y   &&
      ( node->point[1] >= point[1] - dist || ignoreDim == Y ) )
      queryNeighbourPointsRecursive ( point, dist, node->lSon, ignoreDim, nodeList );

    if ( node->disc == KdTree::Z   &&
      ( node->point[2] >= point[2] - dist  || ignoreDim == Z ) )
      queryNeighbourPointsRecursive ( point, dist, node->lSon, ignoreDim, nodeList );
  }

  if ( node->rSon != NULL )
  {
    if ( node->disc == KdTree::X   &&
      ( node->point[0] <= point[0] + dist || ignoreDim == X ) )
      queryNeighbourPointsRecursive  ( point, dist, node->rSon, ignoreDim, nodeList );

    if ( node->disc == KdTree::Y   &&
      ( node->point[1] <= point[1] + dist || ignoreDim == Y ) )
      queryNeighbourPointsRecursive  ( point, dist, node->rSon, ignoreDim, nodeList );

    if ( node->disc == KdTree::Z   &&
      ( node->point[2] <= point[2] + dist   || ignoreDim == Z ) )
      queryNeighbourPointsRecursive  ( point, dist, node->rSon, ignoreDim, nodeList );
  }

  return nodeList;
}

///////////////////////////////////////////////////////////////////////////////
//  Sucht alle Knoten, die in einer Entfernung dist von point liegen         //
//  Rekursion                                                                //
///////////////////////////////////////////////////////////////////////////////
void KdTree::queryNeighbourPointsRecursive  ( double * point, double dist, KdTreeNode * node, DISC ignoreDim, vector<KdTreeNode*> & nodeList )
{
  double pointDist;
  int    i;

  pointDist = 0;
  for ( i = 0; i < dimensionNum; i++ )
    if ( i != ignoreDim )
      pointDist = pointDist + ( node->point[i] - point[i] )*( node->point[i] - point[i] );
  pointDist = sqrt ( pointDist );

  if ( pointDist <= dist )
    nodeList.push_back ( node );

  if ( node->lSon != NULL )
  {
    if ( node->disc == KdTree::X   &&
      ( node->point[0] >= point[0] - dist || ignoreDim == X ) )
      queryNeighbourPointsRecursive ( point, dist, node->lSon, ignoreDim, nodeList );

    if ( node->disc == KdTree::Y   &&
      ( node->point[1] >= point[1] - dist || ignoreDim == Y )  )
      queryNeighbourPointsRecursive ( point, dist, node->lSon, ignoreDim, nodeList );

    if ( node->disc == KdTree::Z   &&
      ( node->point[2] >= point[2] - dist || ignoreDim == Z ) )
      queryNeighbourPointsRecursive ( point, dist, node->lSon, ignoreDim, nodeList );
  }

  if ( node->rSon != NULL )
  {
    if ( node->disc == KdTree::X   &&
      ( node->point[0] <= point[0] + dist || ignoreDim == X )  )
      queryNeighbourPointsRecursive ( point, dist, node->rSon, ignoreDim, nodeList );

    if ( node->disc == KdTree::Y   &&
      ( node->point[1] <= point[1] + dist  || ignoreDim == Y ) )
      queryNeighbourPointsRecursive ( point, dist, node->rSon, ignoreDim, nodeList );

    if ( node->disc == KdTree::Z   &&
      ( node->point[2] <= point[2] + dist  || ignoreDim == Z ) )
      queryNeighbourPointsRecursive ( point, dist, node->rSon, ignoreDim, nodeList );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Offset auf alle Knoten                                                   //
///////////////////////////////////////////////////////////////////////////////
void KdTree::offset ( GeoPoint & point )
{
  KdTreeCursor * cur = new KdTreeCursor;

  KdTreeNode * pNode = getFirstNode ( cur );
  while ( pNode != NULL )
  {
    pNode->offset( point, dimensionNum );
    pNode = getNextNode ( cur );
  }

  delete cur;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt, in welchen Unterbaum des kd-Baumes der neue Punkt eingetragen  //
//  werden muß					                              											 //
///////////////////////////////////////////////////////////////////////////////
KdTree::DIRECTION KdTree::kdCompare ( KdTreeNode * node, double point[]	)
{
  switch ( node->disc )
  {
  case X:
    if ( point[0] < node->point[0] )
      return LEFT;
    else
      return RIGHT;

  case Y:
    if ( point[1] < node->point[1] )
      return LEFT;
    else
      return RIGHT;

  case Z:
    if ( point[2] < node->point[2] )
      return LEFT;
    else
      return RIGHT;

  default:
    return RIGHT;		// Dummy
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt die Koordinatenrichtung, die als nächstes als Discreminator     //
//  benutzt wird													                              		 //
///////////////////////////////////////////////////////////////////////////////
KdTree::DISC KdTree::nextDisc	(	KdTreeNode * node	)
{
  if ( dimension == ZWEI )
    switch ( node->disc )
  {
    case X:
      return Y;

    case Y:
      return X;

    default: return X;		// Dummy
  }

  if ( dimension == DREI )
    switch ( node->disc )
  {
    case X:
      return Y;

    case Y:
      return Z;

    case Z:
      return X;

    default: return X;		// Dummy
  }

  return X;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt im von rootNode definierten Baum denjenigen Knoten, der bzgl.   //
//  der Koordinatenrichtung disc den kleinsten Wert hat						           //
///////////////////////////////////////////////////////////////////////////////
KdTreeNode * KdTree::findMinimum	(	KdTreeNode * rootNode, DISC disc,	double & minimum 	)
{
  KdTreeNode * node;
  double       minimum1, minimum2;
  KdTreeNode * node1, * node2;

  if ( rootNode->lSon == NULL && rootNode->rSon == NULL )
  {
    minimum = getDiscValue ( rootNode, disc );
    return rootNode;
  }

  if ( rootNode->disc == disc )
  {
    if ( rootNode->lSon == NULL )
    {
      minimum = getDiscValue ( rootNode, disc );
      return rootNode;
    }
    else
    {
      node = findMinimum ( rootNode->lSon, disc, minimum );
      return node;
    }
  }
  else
    if ( rootNode->lSon == NULL || rootNode->rSon == NULL )
    {
      minimum1 = getDiscValue ( rootNode, disc );
      node1    = rootNode;

      if ( rootNode->lSon == NULL )
        node2 = findMinimum ( rootNode->rSon, disc, minimum2 );
      else
        node2 = findMinimum ( rootNode->lSon, disc, minimum2 );

      if ( minimum1 < minimum2 )
      {
        minimum = minimum1;
        return node1;
      }
      else
      {
        minimum = minimum2;
        return node2;
      }
    }
    else
    {
      KdTreeNode * node3 = rootNode;
      double 		   minimum3 = getDiscValue ( rootNode, disc );

      node1 = findMinimum ( rootNode->lSon, disc, minimum1 );
      node2 = findMinimum ( rootNode->rSon, disc, minimum2 );
      if ( minimum2 <= minimum1 )
      {
        minimum1 = minimum2;
        node1 = node2;
      }

      if ( minimum3 < minimum1 )
      {
        minimum = minimum3;
        return node3;
      }
      else
      {
        minimum = minimum1;
        return node1;
      }
    }

    return rootNode;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den disk-Koordinatenwert des referierten Punktes				 //
///////////////////////////////////////////////////////////////////////////////
double KdTree::getDiscValue	(	KdTreeNode * node, DISC disc)
{
  switch ( disc )
  {
  case X:		
    return node->point[0];
    break;

  case Y:		
    return node->point[1];
    break;

  case Z:		
    return node->point[2];
    break;

  default:	
    return node->point[0];
  }
}
///////////////////////////////////////////////////////////////////////////////
//  Löscht einen Knoten des kd-Trees und baut den verbleibenden Unterbaum	   //
//  entsprechend um														                            	 //
///////////////////////////////////////////////////////////////////////////////
void KdTree::deleteNode ( KdTreeNode * nodeToDelete )
{
  KdTreeNode * resultSubtree = deleteNodeRecursive ( nodeToDelete );
  KdTreeNode * father = nodeToDelete->father;

  if ( father == NULL )
  {
    root = resultSubtree;
  }
  else
  {
    if ( nodeToDelete == father->lSon )
    {
      if ( resultSubtree != NULL )
        resultSubtree->direction = LEFT;
      father->lSon = resultSubtree;
    }
    else
    {
      if ( resultSubtree != NULL )
        resultSubtree->direction = RIGHT;
      father->rSon = resultSubtree;
    }

    if ( resultSubtree != NULL )
      resultSubtree->father = father;
  }

  delete nodeToDelete;
}

///////////////////////////////////////////////////////////////////////////////
//  Ersetzt rekursiv einen zu löschenden Knoten durch einen geeigneten		   //
//  untergeordneten Knoten													                         //
///////////////////////////////////////////////////////////////////////////////
KdTreeNode * KdTree::deleteNodeRecursive ( KdTreeNode * nodeToDelete )
{
  DISC  disc;
  double minimum;

  if ( nodeToDelete->lSon == NULL && nodeToDelete->rSon == NULL )
    return NULL;

  disc = nodeToDelete->disc;

  if ( nodeToDelete->rSon == NULL )
  {
    nodeToDelete->rSon = nodeToDelete->lSon;
    nodeToDelete->rSon->direction = RIGHT;
    nodeToDelete->lSon = NULL;
  }

  KdTreeNode * replacingNode = findMinimum ( nodeToDelete->rSon, disc, minimum );
  KdTreeNode * father = replacingNode->father;

  if ( replacingNode == father->lSon )
    father->lSon = deleteNodeRecursive ( replacingNode );
  else
    father->rSon = deleteNodeRecursive ( replacingNode );

  replacingNode->father    = nodeToDelete->father;
  replacingNode->lSon      = nodeToDelete->lSon;
  if ( replacingNode->lSon != NULL )
    replacingNode->lSon->father = replacingNode;

  replacingNode->rSon      = nodeToDelete->rSon;
  if ( replacingNode->rSon != NULL )
    replacingNode->rSon->father = replacingNode;

  replacingNode->disc		 = nodeToDelete->disc;
  replacingNode->direction = nodeToDelete->direction;

  return replacingNode;
}

///////////////////////////////////////////////////////////////////////////////
// 	 class KdTreeNode			                          	    							     //
//   Knoten in einem KdTree     										                         //
///////////////////////////////////////////////////////////////////////////////

KdTreeNode::KdTreeNode ( int dimension )
{
  point     = new double[dimension];
  disc      = KdTree::X;
  father    = NULL;
  rSon      = NULL;
  lSon      = NULL;
  direction = KdTree::LEFT;
}


///////////////////////////////////////////////////////////////////////////////
//  Konstruktor																 //
///////////////////////////////////////////////////////////////////////////////
KdTreeNode::KdTreeNode ( double pointP[],  KdTree::DISC discP, KdTreeNode * fatherP,  int dimension )
{
  point = new double[dimension];
  for ( int i = 0; i < dimension; i++ )
    point[i] = pointP[i];

  disc      = discP;
  father    = fatherP;
  rSon      = NULL;
  lSon      = NULL;
  direction = KdTree::LEFT;
}

///////////////////////////////////////////////////////////////////////////////
//  Destruktor																 //
///////////////////////////////////////////////////////////////////////////////
KdTreeNode::~KdTreeNode()
{
  delete[] point;
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht alle untergeordneten Knoten rekursiv					              			 //
///////////////////////////////////////////////////////////////////////////////
void KdTreeNode::deleteNodesRecursive()
{
  if ( lSon != NULL )
  {
    lSon->deleteNodesRecursive();
    delete lSon;
  }

  if ( rSon != NULL )
  {
    rSon->deleteNodesRecursive();
    delete rSon;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Sohn-Pointer										                        		 //
///////////////////////////////////////////////////////////////////////////////
KdTreeNode * KdTreeNode::getSon  ( KdTree::DIRECTION directionP )
{
  if ( directionP == KdTree::LEFT )
    return lSon;
  else
    return rSon;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert und setzt einen Sohn-Pointer												           //
///////////////////////////////////////////////////////////////////////////////
KdTreeNode * KdTreeNode::setSon ( KdTree::DIRECTION directionP, double pointP[],  KdTree::DISC diskP, int dimension )
{
  if ( directionP == KdTree::LEFT )
  {
    lSon = new KdTreeNode ( pointP, diskP, this, dimension );
    lSon->direction = directionP;
    return lSon;
  }
  else
  {
    rSon = new KdTreeNode ( pointP, diskP, this, dimension );
    rSon->direction = directionP;
    return rSon;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt Knotenattribute	                          	 											 //
///////////////////////////////////////////////////////////////////////////////
void KdTreeNode::setNode ( double pointP[], KdTree::DISC discP, KdTreeNode * fatherP, int dimension )
{
  for ( int i = 0; i < dimension; i++ )
    point[i] = pointP[i];

  if ( fatherP == NULL )
    direction = KdTree::LEFT;
  else
  {
    if ( fatherP->lSon == this )
      direction = KdTree::LEFT;
    else
      direction = KdTree::RIGHT;
  }

  father = fatherP;
  disc   = discP;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob es bereits eine Edge zwischen den beiden Knoten gibt, und  //
//  liefert diese Edge ggf.                                                  //
///////////////////////////////////////////////////////////////////////////////
Edge * KdTreeNode::existEdge ( KdTreeNode * pToNode, bool & orientation )
{
  unsigned int i, j;

  for (  i = 0; i < vEdges.size(); i++ )
  {
    Edge * pEdge = vEdges[i];
    for ( j = 0; j < pToNode->getEdgeAnz(); j++ )
    {
      Edge * pToEdge = pToNode->getEdge( j );

      if ( pEdge == pToEdge )
      {
        if ( pEdge->pStartVertex == this )
          orientation = true;
        else
          orientation = false;

        return pEdge;
      }
    }
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Edge                                                        //
///////////////////////////////////////////////////////////////////////////////
Edge * KdTreeNode::getEdge ( unsigned int index )
{
  if ( index < 0 || index >= vEdges.size() )
    return NULL;
  else
    return vEdges[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den geomerischen Punkt                                           //
///////////////////////////////////////////////////////////////////////////////
GeoPoint KdTreeNode::getPoint( int dim )
{
  GeoPoint geoPoint = GeoPoint ( dim );

  if ( dim == 2 )
    geoPoint.set( point[0], point[1] );
  else
    geoPoint.set ( point[0], point[1], point[2] );

  return geoPoint;
}

///////////////////////////////////////////////////////////////////////////////
//  Offset                                                                   //
///////////////////////////////////////////////////////////////////////////////
void KdTreeNode::offset ( GeoPoint & offsetPoint, int dim )
{
  point[0] = point[0] + offsetPoint.getX();
  point[1] = point[1] + offsetPoint.getY();
  if ( dim == 3 )
    point[2] = point[2] + offsetPoint.getZ();
}

///////////////////////////////////////////////////////////////////////////////
//  class BRep                                                               //
//  Nicht-redundante Repräsentation eines offenen oder geschlossenen Fläche  //
///////////////////////////////////////////////////////////////////////////////
BRep::BRep ( _Surface *pSurface, double minDist, Features * pFeaturesP )
{
  vector<PolygonGeo*> vPolygone;
  vector<bool>        vOri;
	size_t              i, anz;

  pKdTree = new KdTree ( pSurface->getDim() );
  pKdTree->setMinimalPointDistance( minDist );
  edgeMap = new EdgeMap();

  anz = pSurface->getPolygone( vPolygone, vOri );
  for ( i = 0; i < anz; i++ )
  {
    PolygonGeo * pPolygon = vPolygone[i];
    bool      ori      = vOri[i];
    Face    * pFace = new Face;
    vFaces.push_back( pFace );
    pFace->setFace( pPolygon, ori, this );
  }

  pFeatures = pFeaturesP;

  pSingleEdges    = NULL;
  pDoubleEdges    = NULL;
  pMultipleEdges  = NULL;
  pBackForthEdges = NULL;

  pSingleEdgePolygons     = NULL;
  pDoubleEdgePolygons     = NULL;
  pMultipleEdgePolygons   = NULL;
  pBackForthEdgePolygons  = NULL;

  dim =  pSurface->getDim();
}

BRep::~BRep()
{
  for ( unsigned int i = 0; i < vFaces.size(); i++ )
    delete vFaces[i];

  delete pKdTree;
  delete edgeMap;

  if ( pSingleEdges != NULL )
    delete pSingleEdges;
  if ( pDoubleEdges != NULL )
    delete pDoubleEdges;
  if ( pMultipleEdges != NULL )
    delete pMultipleEdges;
  if ( pBackForthEdges != NULL )
    delete pBackForthEdges;

  if ( pSingleEdgePolygons != NULL )
    delete pSingleEdgePolygons;
  if ( pDoubleEdgePolygons != NULL )
    delete pDoubleEdgePolygons;
  if ( pMultipleEdgePolygons != NULL )
    delete pMultipleEdgePolygons;
  if ( pBackForthEdgePolygons != NULL )
    delete pBackForthEdgePolygons;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den minimalen Punktabstand                                       // 
///////////////////////////////////////////////////////////////////////////////
double BRep::getMinimalPointDistance()
{  
  return pKdTree->getMinimalPointDistance(); 
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob der BRep ein korrektes Volumen ist                         //
///////////////////////////////////////////////////////////////////////////////
bool BRep::isVolume ()
{
	size_t          anzFaces, anzEdges, anzNodes;
  vector<Edge*>   vEdges;
  vector<bool>    vOrientations;
  std::string     errorString;
  GeometryError * pErrorMissingCounterpart = NULL;
  GeometryError * pErrorDoubleEdge         = NULL;
  GeometryError * pErrorMultipleEdge       = NULL;

  anzFaces = vFaces.size();
  anzEdges =  edgeMap->getEdges( vEdges );
  anzNodes = pKdTree->getNodeCount();

  createIncorrectEdgesForSolid();
  createPolygonsAdjacentToIncorrectEdges( true );

  if ( pSingleEdges == NULL &&  pDoubleEdges == NULL && pMultipleEdges == NULL && pBackForthEdges == NULL )
    return true;
  else
    return false;  
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft, ob pObj in der Property featureProperty und der id geometryId //
//  ein korrektes Volumen referiert                                          //
///////////////////////////////////////////////////////////////////////////////
bool BRep::isVolume ( Feature * pObj, string featureProperty, string geometryId, CheckingParams * pCheckParams, 
        std::vector<Error*> &vErrors )
{
	size_t          anzFaces, anzEdges, anzNodes;
  bool            success =  true;
  vector<Edge*>   vEdges;
  vector<bool>    vOrientations;
  std::string     errorString;
  GeometryError * pErrorMissingCounterpart = NULL;
  GeometryError * pErrorDoubleEdge         = NULL;
  GeometryError * pErrorMultipleEdge       = NULL;
  GeometryError * pErrorBackForthEdge      = NULL;
  
  anzFaces = vFaces.size();
  anzEdges =  edgeMap->getEdges( vEdges );
  anzNodes = pKdTree->getNodeCount();

  success = true;
  
  createIncorrectEdgesForSolid();
  createPolygonsAdjacentToIncorrectEdges( true );

  if ( pSingleEdges != NULL )
  {
    pErrorMissingCounterpart = new GeometryError ( pObj, Error::ERROR, GeometryError::SOLID_EDGE_CHECK_FAILED_MISSING_COUNTERPART );
    pErrorMissingCounterpart->setPropertyName( featureProperty );
    pErrorMissingCounterpart->setFeatureGeometryId( geometryId );
    pErrorMissingCounterpart->addErrorGeometry( new MultiCurve ( pSingleEdges ), "Polygon-Kanten, die nur einmal durchlaufen werden" );
    pErrorMissingCounterpart->addErrorGeometry( new MultiSurface ( pSingleEdgePolygons ), "Polygone, die an einfach durchlaufende Kanten angrenzen" );

    vErrors.push_back( pErrorMissingCounterpart );
    success = false;
  }

  if ( pDoubleEdges != NULL )
  {
    pErrorDoubleEdge = new GeometryError ( pObj, Error::ERROR, GeometryError::SOLID_EDGE_CHECK_FAILED_DOUBLE_EDGE );
    pErrorDoubleEdge->setPropertyName( featureProperty );
    pErrorDoubleEdge->setFeatureGeometryId( geometryId );
    pErrorDoubleEdge->addErrorGeometry( new MultiCurve ( pDoubleEdges ), "Polygon-Kanten, die doppelt mit gleicher Orientierung durchlaufen werden" );
    pErrorDoubleEdge->addErrorGeometry( new MultiSurface ( pDoubleEdgePolygons ), "Polygone, die an doppelt durchlaufende Kanten angrenzen" );

    vErrors.push_back( pErrorDoubleEdge );
    success = false;
  }

  if ( pMultipleEdges != NULL )
  {
    pErrorMultipleEdge = new GeometryError ( pObj, Error::ERROR, GeometryError::SOLID_EDGE_CHECK_FAILED_MULTIPLE_EDGE );
    pErrorMultipleEdge->setPropertyName( featureProperty );
    pErrorMultipleEdge->setFeatureGeometryId( geometryId );
    pErrorMultipleEdge->addErrorGeometry( new MultiCurve ( pMultipleEdges ), "Polygon-Kanten, die mehrfach und unterschiedlich häufig hin und zurück durchlaufen werden" );
    pErrorMultipleEdge->addErrorGeometry( new MultiSurface ( pMultipleEdgePolygons ), "Polygone, die an mehrfach und unterschiedlich häufig hin und zurück durchlaufende Kanten angrenzen" );

    vErrors.push_back( pErrorMultipleEdge );
    success = false;
  }

  if ( pBackForthEdges != NULL )
  {
    pErrorBackForthEdge = new GeometryError ( pObj, Error::WARNING, GeometryError::SOLID_EDGE_CHECK_INFO_BACK_FORTH_EDGE );
    pErrorBackForthEdge->setPropertyName( featureProperty );
    pErrorBackForthEdge->setFeatureGeometryId( geometryId );
    pErrorBackForthEdge->addErrorGeometry( new MultiCurve ( pBackForthEdges ), "Polygon-Kanten, die mehrfach hin und zurück durchlaufen werden" );
    pErrorBackForthEdge->addErrorGeometry( new MultiSurface ( pBackForthEdgePolygons ), "Polygone, die an mehrfach hin und zurück durchlaufende Kanten angrenzen" );

    vErrors.push_back( pErrorBackForthEdge );
    success = false;
  }

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Macht den Knoten-/Kantentest und bestimmt, welche Kanten mehrfach in     //
//  einer Richtung durchlaufen wurden, und ob Stützpunkte nicht übereinander-//
//  liegen.                                                                  //
///////////////////////////////////////////////////////////////////////////////
bool BRep::checkTopology ( Feature * pObj, string featureProperty, string geometryId, CheckingParams * pCheckParams,  std::vector<Error*> &vErrors )
{
  bool            success =  true;
  vector<Edge*>   vEdges;
  vector<bool>    vOrientations;
  std::string     errorString;
  GeometryError * pErrorDoubleEdge    = NULL;
  GeometryError * pErrorMultipleEdge  = NULL;
  GeometryError * pErrorBackForthEdge = NULL;

  createIncorrectEdgesForSurface();
  createPolygonsAdjacentToIncorrectEdges( false );


  if ( pDoubleEdges != NULL )
  {
    pErrorDoubleEdge = new GeometryError ( pObj, Error::ERROR, GeometryError::SOLID_EDGE_CHECK_FAILED_DOUBLE_EDGE );
    pErrorDoubleEdge->setPropertyName( featureProperty );
    pErrorDoubleEdge->setFeatureGeometryId( geometryId );
    pErrorDoubleEdge->addErrorGeometry( pDoubleEdges, "Polygon-Kanten, die doppelt mit gleicher Orientierung durchlaufen werden" );
    pErrorDoubleEdge->addErrorGeometry( pDoubleEdgePolygons, "Polygone, die an doppelt durchlaufende Kanten angrenzen" );

    vErrors.push_back( pErrorDoubleEdge );
    success = false;
  }

  if ( pMultipleEdges != NULL )
  {
    pErrorMultipleEdge = new GeometryError ( pObj, Error::ERROR, GeometryError::SOLID_EDGE_CHECK_INFO_BACK_FORTH_EDGE );
    pErrorMultipleEdge->setPropertyName( featureProperty );
    pErrorMultipleEdge->setFeatureGeometryId( geometryId );
    pErrorMultipleEdge->addErrorGeometry( pMultipleEdges, "Polygon-Kanten, die mehrfach und unterschiedlich häufig hin und zurück durchlaufen werden" );
    pErrorMultipleEdge->addErrorGeometry( pMultipleEdgePolygons, "Polygone, die an mehrfach und unterschiedlich häufig hin und zurück durchlaufende Kanten angrenzen" );

    vErrors.push_back( pErrorMultipleEdge );
    success = false;
  }

  if ( pBackForthEdges != NULL )
  {
    pErrorBackForthEdge = new GeometryError ( pObj, Error::WARNING, GeometryError::SOLID_EDGE_CHECK_FAILED_DOUBLE_EDGE );
    pErrorBackForthEdge->setPropertyName( featureProperty );
    pErrorBackForthEdge->setFeatureGeometryId( geometryId );
    pErrorBackForthEdge->addErrorGeometry( pBackForthEdges, "Polygon-Kanten, die mehrfach hin und zurück durchlaufen werden" );
    pErrorBackForthEdge->addErrorGeometry( pBackForthEdgePolygons, "Polygone, die an mehrfach hin und zurück durchlaufende Kanten angrenzen" );

    vErrors.push_back( pErrorBackForthEdge );
    success = false;
  }

  return success;
}


///////////////////////////////////////////////////////////////////////////////
//  Extrahiert alle Kanten, die                                              //
//          - nur einmal durchlaufen werden,                                 //
//          - zweimal in derselben Richtung durchlaufen werden               //
//          - mehrfach durchlaufen werden                                    //
//  und fasst sie jeweils in einer MultiCurve zusammen                       //
///////////////////////////////////////////////////////////////////////////////
void BRep::createIncorrectEdgesForSolid()
{
	size_t        anzEdges, i;
  vector<Edge*> vEdges;
  vector<Edge*> vSingleEdges;
  vector<Edge*> vDoubleEdges;
  vector<Edge*> vMultipleEdges;
  vector<Edge*> vBackForthEdges;
  vector<bool>  vOrientations;

  anzEdges =  edgeMap->getEdges( vEdges );

  for ( i = 0; i < anzEdges; i++ )
  {
    vOrientations.clear();

    Edge * pEdge = vEdges[i];
		size_t anzEdgeOccur = edgeMap->getEdgeOrientations( pEdge, vOrientations );

    if ( anzEdgeOccur == 1 )
      vSingleEdges.push_back( pEdge );
    else
    if ( anzEdgeOccur == 2 )
    {
        if ( vOrientations[0] == vOrientations[1] )
          vDoubleEdges.push_back( pEdge );
    }
    else
    {
      if ( anzEdgeOccur % 2 == 1 )
        vMultipleEdges.push_back( pEdge );
      else
      {
        int anzBack  = 0;
        int anzForth = 0;
        for ( int j = 0; j < anzEdgeOccur; ++j )
        {
          if ( vOrientations[j] == true )
            anzBack++;
          else
            anzForth++;
        }
        if ( anzBack == anzForth )
          vBackForthEdges.push_back( pEdge );
        else
          vMultipleEdges.push_back( pEdge );
      }
    }   
  }

  if ( pSingleEdges != NULL ) delete pSingleEdges;
  if ( vSingleEdges.size() > 0 )
    pSingleEdges = createMultiCurve( vSingleEdges );
  else
    pSingleEdges = NULL;
  
  if ( pDoubleEdges != NULL ) delete pDoubleEdges;
  if ( vDoubleEdges.size() > 0 )
    pDoubleEdges = createMultiCurve( vDoubleEdges );
  else
    pDoubleEdges = NULL;

  if ( pMultipleEdges != NULL ) delete pMultipleEdges;
  if ( vMultipleEdges.size() > 0 )
    pMultipleEdges = createMultiCurve( vMultipleEdges );
  else
    pMultipleEdges = NULL;


  if ( pBackForthEdges != NULL ) delete pBackForthEdges;
  if ( vBackForthEdges.size() > 0 )
    pBackForthEdges = createMultiCurve( vBackForthEdges );
  else
    pBackForthEdges = NULL;

}

///////////////////////////////////////////////////////////////////////////////
//  Extrahiert alle Kanten, die                                              //
//          - zweimal in derselben Richtung durchlaufen werden               //
//          - mehrfach durchlaufen werden                                    //
//  und fasst sie jeweils in einer MultiCurve zusammen                       //
///////////////////////////////////////////////////////////////////////////////
void BRep::createIncorrectEdgesForSurface()
{
	size_t        anzEdges, i;
  vector<Edge*> vEdges;
  vector<Edge*> vSingleEdges;
  vector<Edge*> vDoubleEdges;
  vector<Edge*> vMultipleEdges;
  vector<Edge*> vBackForthEdges;
  vector<bool>  vOrientations;

  anzEdges =  edgeMap->getEdges( vEdges );

  for ( i = 0; i < anzEdges; i++ )
  {
    vOrientations.clear();

    Edge * pEdge = vEdges[i];
		size_t anzEdgeOccur = edgeMap->getEdgeOrientations( pEdge, vOrientations );

    if ( anzEdgeOccur == 2 )
    {
      if ( vOrientations[0] == vOrientations[1] )
        vDoubleEdges.push_back( pEdge );
    }
    else
    {
      if ( anzEdgeOccur % 2 == 1 )
        vMultipleEdges.push_back( pEdge );
      else
      {
        int anzBack  = 0;
        int anzForth = 0;
        for ( int j = 0; j < anzEdgeOccur; ++j )
        {
          if ( vOrientations[j] == true )
            anzBack++;
          else
            anzForth++;
        }
        if ( anzBack == anzForth )
          vBackForthEdges.push_back( pEdge );
        else
          vMultipleEdges.push_back( pEdge );
      }
    }   
  }

  if ( pSingleEdges != NULL ) delete pSingleEdges;
  pSingleEdges = NULL;

  if ( pDoubleEdges != NULL ) delete pDoubleEdges;
  if ( vDoubleEdges.size() > 0 )
    pDoubleEdges = createMultiCurve( vDoubleEdges );
  else
    pDoubleEdges = NULL;

  if ( pMultipleEdges != NULL ) delete pMultipleEdges;
  if ( vMultipleEdges.size() > 0 )
    pMultipleEdges = createMultiCurve( vMultipleEdges );
  else
    pMultipleEdges = NULL;

  if ( pBackForthEdges != NULL ) delete pBackForthEdges;
  if ( vBackForthEdges.size() > 0 )
    pBackForthEdges = createMultiCurve( vBackForthEdges );
  else
    pBackForthEdges = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert ein MultiCurve Objekt aus einer Menge von Edge Objekten        //
//  Zusammenhängende Kurven werden zusammengefügt                            //
///////////////////////////////////////////////////////////////////////////////
MultiCurve * BRep::createMultiCurve ( std::vector<Edge*> &vEdges )
{
  map<Edge*,int>    mEdgeIndexMap;
  vector<bool>      vEdgesUsed;
  deque<GeoPoint>   dPoints;
	size_t            anzEdges;
	int               anzEdgesUsed;
  unsigned int      i;
  Edge            * pEdge;

  for ( i = 0; i < vEdges.size(); i++ )
  {
    pEdge = vEdges[i];
    vEdgesUsed.push_back( false );
    mEdgeIndexMap[pEdge] = i;
  }

  anzEdgesUsed = 0;
  anzEdges     = vEdges.size();

  MultiCurve * pMultiCurve = new MultiCurve ( dim );

  while ( anzEdgesUsed < anzEdges )
  {
    int startIndex = findNextUnusedEdge ( vEdgesUsed );
    if ( startIndex == -1 ) break;

    dPoints.clear();

    Edge * pStartEdge = vEdges[startIndex];
    dPoints.push_back( pStartEdge->pStartVertex->getPoint( dim ) );
    dPoints.push_back( pStartEdge->pEndVertex->getPoint( dim ) );

    vEdgesUsed[startIndex] = true;
    anzEdgesUsed++;

    bool isClosed = followEdge ( pStartEdge, mEdgeIndexMap, vEdgesUsed, anzEdgesUsed, dPoints, true );
    if ( !isClosed )
       followEdge ( pStartEdge, mEdgeIndexMap, vEdgesUsed, anzEdgesUsed, dPoints, false );

    LineSegment * pLineSegment = new LineSegment ( 3, LineSegment::_LINE_STRING );
    pLineSegment->addPoints ( dPoints );
    pMultiCurve->addCurve( pLineSegment );
  }

  return pMultiCurve;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert ein MultiCurve Objekt aus einer Menge von Edge Objekten        //
//  Zusammenhängende Kurven werden nicht zusammengefügt                      //
///////////////////////////////////////////////////////////////////////////////
void BRep::createMultiCurveWithoutAggregation ( std::vector<Edge*> &vEdges )
{
  unsigned int      i;
  Edge            * pEdge;
  vector<GeoPoint>  vPoints;

  pSingleEdges = new MultiCurve ( dim );

  for ( i = 0; i < vEdges.size(); i++ )
  {
    pEdge = vEdges[i];
    vPoints.clear();

    LineSegment * pLineSegment = new LineSegment ( 3, LineSegment::_LINE_STRING );
    vPoints.push_back ( pEdge->pStartVertex->getPoint( 3 ) );
    vPoints.push_back ( pEdge->pEndVertex->getPoint( 3 ) );
    pLineSegment->addPoints ( vPoints );

    pSingleEdges->addCurve( pLineSegment );
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Hilfsroutine: Sucht die nächste noch nicht prozessierte Kante            //
///////////////////////////////////////////////////////////////////////////////
int BRep::findNextUnusedEdge ( std::vector<bool> & vEdgesUsed )
{
  bool found = false;

  for ( unsigned int i = 0; i < vEdgesUsed.size(); i++ )
    if ( vEdgesUsed[i] == false )
      return i;

  return -1;
}

///////////////////////////////////////////////////////////////////////////////
//  Hilfsroutine Kantenverfolgung                                            //
///////////////////////////////////////////////////////////////////////////////
bool BRep::followEdge ( Edge * pStartEdge, std::map<Edge*,int> &mEdgeIndexMap, std::vector<bool> &vEdgesUsed, 
                               int & anzEdgesUsed, std::deque<GeoPoint> &dPoints, bool direction )
{
  KdTreeNode * pStartNode;
  KdTreeNode * pActNode;
  KdTreeNode * pNextNode;
  Edge       * pActEdge;
  Edge       * pNextEdge;
  Edge       * pEdge;
  unsigned int i;

  map<Edge*,int>::iterator pos;

  if ( direction == true )
  {
    pStartNode = pStartEdge->pStartVertex;
    pActNode = pStartEdge->pEndVertex;
  }
  else
  {
    pStartNode = pStartEdge->pStartVertex;
    pActNode = pStartEdge->pStartVertex;
  }

  pActEdge = pStartEdge;
  pNextEdge = NULL;
  pNextNode = NULL;

  while ( true )
  {
    pNextNode  = NULL;
    for ( i = 0; i < pActNode->getEdgeAnz(); i++ )
    {
      pEdge = pActNode->getEdge ( i );
      if ( pEdge != pActEdge )
      {
        pos = mEdgeIndexMap.find( pEdge );
        if ( pos != mEdgeIndexMap.end() )
        {
          if ( vEdgesUsed[pos->second] == false || pEdge == pStartEdge )            
          {
            pNextEdge = pEdge;
            if (  pEdge != pStartEdge )
            {
             vEdgesUsed[pos->second] = true;
              anzEdgesUsed++;
           
              if ( pActNode == pNextEdge->pStartVertex )
                pNextNode = pNextEdge->pEndVertex;
              else
                pNextNode = pNextEdge->pStartVertex;
              if ( direction == true )
                dPoints.push_back( pNextNode->getPoint( dim ) );
              else
                dPoints.push_front( pNextNode->getPoint( dim ) );
            }
            break;
          }
        }
      }
    }
    if ( pNextNode != NULL )
    {
      pActNode = pNextNode;
      pActEdge = pNextEdge;
      if ( pActEdge == pStartEdge )
      {
        break;
      }
    }
    else
      break;
  }

  if ( pActEdge == pStartEdge )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Extrahiert alle Polygone, die eine nicht korrekte Kante referieren       //
///////////////////////////////////////////////////////////////////////////////
void BRep::createPolygonsAdjacentToIncorrectEdges( bool singeEdgeError )
{
	size_t        anzEdges, i;
  vector<Edge*> vEdges;
  vector<bool>  vOrientations;
  set<PolygonGeo*> sSingleEdgePolygons;
  set<PolygonGeo*> sDoubleEdgePolygons;
  set<PolygonGeo*> sMultipleEdgePolygons;
  set<PolygonGeo*> sBackForthEdgePolygons;

  anzEdges = edgeMap->getEdges( vEdges );

  for ( i = 0; i < vEdges.size(); i++ )
  {
    vOrientations.clear();

    Edge * pEdge = vEdges[i];
		size_t anzEdgeOccur = edgeMap->getEdgeOrientations( pEdge, vOrientations );

    if ( anzEdgeOccur == 1  )
    {
      if ( singeEdgeError )
        pEdge->getPolygons( sSingleEdgePolygons );
    }
    else
    if ( anzEdgeOccur == 2 )
    {
      if ( vOrientations[0] == vOrientations[1] )
        pEdge->getPolygons( sDoubleEdgePolygons );
    }
    else
    {
      if ( anzEdgeOccur % 2 == 1 )
        pEdge->getPolygons( sMultipleEdgePolygons );
      else
      {
        int anzBack  = 0;
        int anzForth = 0;
        for ( int j = 0; j < anzEdgeOccur; ++j )
        {
          if ( vOrientations[j] == true )
            anzBack++;
          else
            anzForth++;
        }
        if ( anzBack == anzForth )
          pEdge->getPolygons( sBackForthEdgePolygons );
        else
          pEdge->getPolygons( sMultipleEdgePolygons );
      }
    }   
  }

  if ( pSingleEdgePolygons != NULL )
  {
    delete pSingleEdgePolygons;
    pSingleEdgePolygons = NULL;
  }
  if ( sSingleEdgePolygons.size() > 0 )
    pSingleEdgePolygons = createMultiSurface ( sSingleEdgePolygons );

  if ( pDoubleEdgePolygons != NULL )
  {
    delete pDoubleEdgePolygons;
    pDoubleEdgePolygons = NULL;
  }
  if ( sDoubleEdgePolygons.size() > 0 )
    pDoubleEdgePolygons = createMultiSurface ( sDoubleEdgePolygons );

  if ( pMultipleEdgePolygons != NULL )
  {
    delete pMultipleEdgePolygons;
    pMultipleEdgePolygons = NULL;
  }
  if ( sMultipleEdgePolygons.size() > 0 )
    pMultipleEdgePolygons = createMultiSurface ( sMultipleEdgePolygons );

  if ( pBackForthEdgePolygons != NULL )
  {
    delete pBackForthEdgePolygons;
    pBackForthEdgePolygons = NULL;
  }
  if ( sBackForthEdgePolygons.size() > 0 )
    pBackForthEdgePolygons = createMultiSurface ( sBackForthEdgePolygons );
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert ein MultiSurface Objekt aus einer Menge von Polygonen          //
///////////////////////////////////////////////////////////////////////////////
MultiSurface * BRep::createMultiSurface ( std::set<PolygonGeo*> & sPolygons )
{
  set<PolygonGeo*>::iterator iter;

  MultiSurface * pMultiSurface = new MultiSurface ( dim );

  for ( iter = sPolygons.begin(); iter != sPolygons.end(); iter++ )
  {
    PolygonGeo * pPoly = *iter;
    SurfaceReference * pSurfaceReference = new SurfaceReference ( pFeatures );
    pSurfaceReference->setRefGeoId( pPoly->getGmlId() );
    pMultiSurface->addSurface( pSurfaceReference );
  }

  return pMultiSurface;
}

///////////////////////////////////////////////////////////////////////////////
//  Verschiebt den BRep um einen Offset                                      //
///////////////////////////////////////////////////////////////////////////////
void BRep::offset ( GeoPoint & point, std::set<_Geometrie*> &sGeometries )
{
  if ( pKdTree != NULL )
    pKdTree->offset( point );

  if ( pSingleEdges != NULL && sGeometries.count ( pSingleEdges ) == 0 )
    pSingleEdges->offset( point, sGeometries );

  if ( pDoubleEdges != NULL && sGeometries.count ( pDoubleEdges ) == 0 )
    pDoubleEdges->offset( point, sGeometries );

  if ( pMultipleEdges != NULL && sGeometries.count ( pMultipleEdges ) == 0 )
    pMultipleEdges->offset( point, sGeometries );

  if ( pSingleEdgePolygons != NULL && sGeometries.count ( pSingleEdgePolygons ) == 0 )
    pSingleEdgePolygons->offset( point, sGeometries );

  if ( pDoubleEdgePolygons != NULL && sGeometries.count ( pDoubleEdgePolygons ) == 0 )
    pDoubleEdgePolygons->offset( point, sGeometries );

  if ( pMultipleEdgePolygons != NULL && sGeometries.count ( pMultipleEdgePolygons ) == 0 )
    pMultipleEdgePolygons->offset( point, sGeometries );
}

///////////////////////////////////////////////////////////////////////////////
//  Nicht-redundante Speicherung eines Polygons                              //
///////////////////////////////////////////////////////////////////////////////
Face::Face ()
{
  pAussenkontur = NULL;
  pPolygon      = NULL;
}

Face::~Face()
{
  if ( pAussenkontur != NULL )
    delete pAussenkontur;

  for ( unsigned int i = 0; i < vLoecher.size(); i++ )
    delete vLoecher[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert das Face zu einem Polygon                                      //
///////////////////////////////////////////////////////////////////////////////
void Face::setFace ( PolygonGeo * pPolygonP, bool ori, BRep * pBRep )
{
  pPolygon = pPolygonP;

  if ( pPolygon->getAussenkontur() != NULL )
  {
    pAussenkontur = new FaceRing ( pPolygon );
    pAussenkontur->setRing( pPolygon->getAussenkontur(), ori, this, pBRep );
  }

  for ( unsigned int i = 0; i < pPolygon->getLochAnz(); i++ )
  {
    FaceRing * pLoch = new FaceRing ( pPolygon );
    pLoch->setRing( pPolygon->getLoch(i), ori, this, pBRep );
    vLoecher.push_back( pLoch );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Nicht-redundante Speicherung eines Ringes                                //
///////////////////////////////////////////////////////////////////////////////
FaceRing::FaceRing (  PolygonGeo * pPolygonP )
{
  pPolygon = pPolygonP;
}

FaceRing:: ~FaceRing()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Generiert bzw. findet die Edges eines Ringes                             //
///////////////////////////////////////////////////////////////////////////////
void FaceRing::setRing ( _Ring *pRing, bool ori, Face * pFace, BRep * pBRep )
{
  vector<double>    vX;
  vector<double>    vY;
  vector<double>    vZ;
	size_t            i, anz;
  double            point[3];
  KdTreeNode      * pNodeAnf;
  KdTreeNode      * pNodeEnd;
  Edge            * pEdge;
  bool              newNodeAnf;
  bool              newNodeEnd;
  bool              orientation;


  KdTree  * pPointTree = pBRep->getKdTree();
  EdgeMap * pEdgeMap = pBRep->getEdgeMap();

  if ( ori )
    anz = pRing->getPoints3D( vX, vY, vZ );
  else
    anz = pRing->getPoints3DReverse( vX, vY, vZ );


  point[0] = vX[0];
  point[1] = vY[0];
  point[2] = vZ[0];
  pNodeAnf = pPointTree->generateNode ( point, true, newNodeAnf );

  for ( i = 1; i < anz; i++ )
  {
    point[0] = vX[i];
    point[1] = vY[i];
    point[2] = vZ[i];

    pNodeEnd = pPointTree->generateNode ( point, true, newNodeEnd );
    pEdge = NULL;
    if ( newNodeAnf == false && newNodeEnd == false )
    {
      pEdge = pNodeAnf->existEdge ( pNodeEnd, orientation );
      if ( pEdge != NULL )
      {
        pEdge->addPolygon( pFace->getPolygon() );
        vEdges.push_back( pEdge );
        vOrientations.push_back( orientation );
        pEdgeMap->addEdge( pEdge, orientation, pFace );
      }
    }

    if (  pEdge == NULL )
    {
      pEdge = new Edge ( pNodeAnf, pNodeEnd );
      pEdge->addPolygon( pFace->getPolygon() );

      vEdges.push_back( pEdge );
      vOrientations.push_back( true );
      pEdgeMap->createEdge( pEdge, true, pFace );

      pNodeAnf->addEdge ( pEdge );
      pNodeEnd->addEdge( pEdge );
    }

    pNodeAnf    = pNodeEnd;
    newNodeAnf = newNodeEnd;
    newNodeAnf = false;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  class Edge                                                               //
//  Von zwei Stützpunkten begrenztes Linienstück                             //
///////////////////////////////////////////////////////////////////////////////
Edge::Edge ( KdTreeNode * pStartVertexP, KdTreeNode * pEndVertexP )
{
  pStartVertex = pStartVertexP;
  pEndVertex   = pEndVertexP;
}

Edge:: ~Edge()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Polygon hinzu                                                   //
///////////////////////////////////////////////////////////////////////////////
void Edge::addPolygon ( PolygonGeo * pPolygon )
{
  sPolygons.insert( pPolygon );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Polygone                                                    //
///////////////////////////////////////////////////////////////////////////////
size_t Edge::getPolygons ( std::set<PolygonGeo*> & sPolygonsP )
{
  set<PolygonGeo*>::iterator iter;

  for ( iter = sPolygons.begin(); iter != sPolygons.end(); iter++ )
  {
    sPolygonsP.insert( *iter );
  }
  return sPolygonsP.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Speichert, wie häufig Edge-Objekte in einer bestimmten Orientierung      //
//  verwendet werden, und welche Faces damit verbúnden sind                  //
///////////////////////////////////////////////////////////////////////////////
EdgeMap::EdgeMap()
{

}

EdgeMap:: ~EdgeMap()
{
  std::map<Edge*,std::vector<EdgeMapItem>*>::iterator iter;

  for ( iter = mEdgeMap.begin(); iter != mEdgeMap.end(); iter++ )
  {
    delete iter->first;
    delete iter->second;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Referent der Edge zur Liste hinzu                              //
///////////////////////////////////////////////////////////////////////////////
void EdgeMap::addEdge ( Edge * pEdge, bool ori, Face * pFace )
{
  vector<EdgeMapItem> * pItems;

  pItems = mEdgeMap[pEdge];
  pItems->push_back(  EdgeMapItem ( ori, pFace ) );
}

void EdgeMap::createEdge ( Edge * pEdge, bool ori, Face * pFace )
{
  vector<EdgeMapItem> * pItems = new vector<EdgeMapItem>;
  pItems->push_back( EdgeMapItem ( ori, pFace )  );
  mEdgeMap[pEdge] = pItems;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Edges                                                       //
///////////////////////////////////////////////////////////////////////////////
size_t EdgeMap::getEdges ( std::vector<Edge*> &vEdges )
{
  std::map<Edge*,std::vector<EdgeMapItem>*>::iterator iter;

  for ( iter =  mEdgeMap.begin(); iter != mEdgeMap.end(); iter++ )
  {
    Edge * pEdge = iter->first;
    vEdges.push_back( pEdge );
  }
  return vEdges.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Orientierungen, unter denen die Edge vorkommt               //
///////////////////////////////////////////////////////////////////////////////
size_t EdgeMap::getEdgeOrientations ( Edge * pEdge, vector<bool> &vOrientations )
{
  map<Edge*,vector<EdgeMapItem>*>::iterator iter;
   
  iter = mEdgeMap.find( pEdge );
  if ( iter == mEdgeMap.end() )
    return 0;

  for ( unsigned int i  = 0; i < iter->second->size(); i++ )
    vOrientations.push_back( (*(iter->second))[i].orientation );

  return vOrientations.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Faces, in denen die Edge vorkommt                           //
///////////////////////////////////////////////////////////////////////////////
size_t EdgeMap::getAdjacentFaces ( Edge * pEdge, vector<Face*> &vFaces )
{
  map<Edge*,vector<EdgeMapItem>*>::iterator iter;

  iter = mEdgeMap.find( pEdge );
  if ( iter == mEdgeMap.end() )
    return 0;

  for ( unsigned int i  = 0; i < iter->second->size(); i++ )
    vFaces.push_back( (*(iter->second))[i].pFace );

  return vFaces.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Datentyp der Werte einer EdgeMap                                         //
///////////////////////////////////////////////////////////////////////////////
EdgeMapItem::EdgeMapItem ( bool oriP, Face * pFaceP )
{
  pFace       = pFaceP;
  orientation = oriP;
}

EdgeMapItem::~EdgeMapItem()
{

}



















