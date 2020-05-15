#include "StdAfx.h"

#include "EntityVisitor.h"

const wchar_t DEFAULT_REPRESENTATION_IDENTIFIER[] = L"Body";

typedef std::vector<_Geometrie*> Geometries;

using namespace std;

EntityVisitor::EntityVisitor()
{
  pFeatures = NULL;
}

EntityVisitor::~EntityVisitor()
{
//  for (std::list<_Geometrie*>::const_iterator it = m_cloned.begin(); it != m_cloned.end(); ++it)
//  {
//    delete *it;
//  }
}

/*
void EntityVisitor::visit(IfcDB::ifcOid oid)
{
  IfcEntityList children;
  m_db->getChildren(oid, children);

  for (IfcEntityList::const_iterator it = children.begin(); it != children.end(); ++it)
  {
    IfcDB::IfcEntity* entity = *it;

    onBeginEntity(*entity);

    m_onProgress(m_currIndex, m_db->getSizeOfEntities());
    ++m_currIndex;

    visit(*entity);

    visit(entity->getOid());

    onEndEntity();
  }
}
*/

/*
void EntityVisitor::start( Feature * pFeature )
{

  visit( pFeature );

  onEnd();
}
*/
bool EntityVisitor::visit( Feature * pFeature )
{
  vector<string>      vGeometryProperties;
  vector<_Geometrie*> vGeometries;
  _Geometrie        * pGeometry;
  int                 i, anz;
  bool                geometryExist = false;
  bool                geometryExistLoc;

  pFeatures = pFeature->getFeatures();

  onBegin();

  anz = pFeature->getGeometryProperties ( vGeometryProperties, vGeometries );

  for ( i = 0; i < anz; i++ )
  {
    pGeometry = vGeometries[i];

    onBeginGeometry();

    geometryExistLoc = visit( pGeometry );
    if ( geometryExistLoc == true )
      geometryExist = true;

/*    m_geometries.push( pGeometry );


    while ( !m_geometries.empty() )
    {
      _Geometrie * geometry = m_geometries.front();
      m_geometries.pop();

      geometryExistLoc = visit( *geometry );
      if ( geometryExistLoc == true )
        geometryExist = true;
    }
*/
    onEndGeometry();
  }

  return geometryExist;
}

bool EntityVisitor::visit ( _Geometrie * pGeometry )
{
  unsigned int i, anz;
  bool     geometryExist = false;
  bool     geometryExistLoc;


  GEOMETRY_TYPE type = pGeometry->getGeometryType();

  switch (type)
  {
  case MULTI_GEOMETRY:
    {
      vector<_Geometrie*> vMembers;

      MultiGeometry * pMultiGeometry = dynamic_cast<MultiGeometry*>(pGeometry);
      for ( i = 0; i < pMultiGeometry->getGeometryMemberAnz(); i++ )
      {
        _Geometrie * pGeo = pMultiGeometry->getGeometry( i );
        geometryExistLoc = visit ( pGeo );
        if ( geometryExistLoc == true )
          geometryExist = true;
      }

      break;
    }


  case GEOMETRIC_COMPLEX:
    {
      vector<_Geometrie*> vMembers;

      GeometricComplex * pMultiGeometry = dynamic_cast<GeometricComplex*>(pGeometry);
      for ( i = 0; i < pMultiGeometry->getGeometryMemberAnz(); i++ )
      {
        _Geometrie * pGeo = pMultiGeometry->getGeometry( i );
        geometryExistLoc = visit ( pGeo );
        if ( geometryExistLoc == true )
          geometryExist = true;
      }

      break;
    }

  case MULTI_SURFACE:
  case COMPOSITE_SURFACE:
  case SHELL:
  case ORIENTABLE_SURFACE:
  case SURFACE:
  case TRIANGULATED_SURFACE:
    {
      vector <Polygon*> vPolygone;
      vector<bool>      vOrientations;
      
      _Surface * pAbstractSurface = dynamic_cast<_Surface*>(pGeometry);
      anz =  pAbstractSurface->getPolygone( vPolygone, vOrientations );

      for ( i = 0; i < anz; i++ )
      {
        geometryExistLoc = visit( vPolygone[i] );
        if ( geometryExistLoc == true )
          geometryExist = true;
      }

      break;
    }

  case SOLID:
  case MULTI_SOLID:
    {
      vector <Polygon*> vPolygone;
      vector<bool>      vOrientations;

      _Solid * pAbstractSolid = dynamic_cast<_Solid*>(pGeometry);
      anz = pAbstractSolid->getAussenkonturPolygone( vPolygone, vOrientations );

      for ( i = 0; i < anz; i++ )
      {
        geometryExistLoc = visit( vPolygone[i] );
        if ( geometryExistLoc == true )
          geometryExist = true;
      }

      break;
    }

  case POLYGON:
  case TRIANGLE:
    {
      Polygon * pPolygon = dynamic_cast<Polygon*>(pGeometry);

      geometryExist = visit( pPolygon );
      break;
    }
 
  default:
    {
      break;
    }
  }
  return geometryExist;
}


bool EntityVisitor::visit( Polygon * pPolygon )
{
  if ( pPolygon->getAussenkontur() != NULL )
  {
    vector<string>            thema;
    vector<AppearanceData*>   appearanceData;
    vector<TexCoordPolyloop*> texCoords;


    pFeatures->getGeoAppearanceTexture(pPolygon->getAussenkontur()->getGmlId(), thema, appearanceData, texCoords );

    if (thema.size() > 0)
    {
      // TODO: Themes
      TexCoordPolyloop * texCoord = texCoords[0];
      AppearanceData   * appearance = appearanceData[0];

      onFace( *pPolygon, texCoord, appearance);
    }
    else
    {
      onFace( *pPolygon );
    }
    return true;
  }
  else return false;
}

