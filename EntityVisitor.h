#pragma once

#include <cassert>
#include <queue>
#include <set>
#include <map>
#include <list>
#include <deque>

#include "Geometrie.h"
#include "Feature.h"
#include <boost/signal.hpp>

class EntityVisitor
{
public:
  typedef boost::signal<void(size_t, size_t)> ProgressSignal;

//  ProgressSignal m_onProgress;

    EntityVisitor();
    virtual ~EntityVisitor();

//    void setIgnoreSpaces(bool ignoreSpaces) { m_ignoreSpaces = ignoreSpaces; }
//    void setIgnoreRoomGeometry(bool ignoreRoomGeometry) { m_ignoreRoomGeometry = ignoreRoomGeometry; }

//    void start( const Feature * pFeature );
    bool visit( Feature * pFeature );

    virtual void onBegin() = 0;
    virtual void onEnd() = 0;
    virtual void onBeginEntity( const Feature & entity ) = 0;
    virtual void onEndEntity() = 0;
    virtual void onBeginGeometry() = 0;
    virtual void onEndGeometry() = 0;
    virtual void onFace( Polygon& face, TexCoordPolyloop* texCoords = 0, AppearanceData* appearance = 0 ) = 0;
  
private:
    bool visit( _Geometrie * geometry  );
    bool visit( Polygon * face );

    Features * pFeatures;
//    std::deque<Feature*>   m_entities;
//    std::queue<_Geometrie*> m_geometries;

//    std::list<_Geometrie*> m_cloned;

//    bool m_ignoreSpaces;
//    bool m_ignoreRoomGeometry;
};