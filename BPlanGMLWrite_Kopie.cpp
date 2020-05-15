#include "StdAfx.h"
#include <time.h>
#include <string>
#include <map>
#include <sstream>
#include <set>

#include "Convert.h"
#include "BPlan.h"
#include "BPlanObjekt.h"
#include "Geometrie.h"
#include "GmlSchema.h"

#include ".\BPlanGMLWriter.h"
#using <mscorlib.dll>

///////////////////////////////////////////////////////////////////////////////
//  class GMLWriter							                            								 //
//  Basisklasse für spezielle GML-Writer			                  						 //
///////////////////////////////////////////////////////////////////////////////
GMLWriter::GMLWriter (Features * pPlanZuSchreibenP, int anzObjWriteMaxP )
{
	pPlanZuSchreiben = pPlanZuSchreibenP;

	precision    = 3;
  srsDimension = 2;

	anzObjWrite    = 0;
	anzObjWriteMax = anzObjWriteMaxP;
  gmlVersion     = pPlanZuSchreiben->getGmlObjekte()->getGmlVersion();
}

GMLWriter::~GMLWriter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer gml:MultiSurface		                        						 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeMultiSurface ( MultiSurface * pMultiSurface )
{
	_Surface * pSurfaceMember;
	int        i, anz;
	string     stdString;
  String   ^ gmlId;

	anz = pMultiSurface->getSurfaceMemberAnz();
	if ( anz == 0 ) return;

	writer->WriteStartElement ( "gml:MultiSurface" );

	stdString = pMultiSurface->getSrsName();
	if ( stdString != "" )
		writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

	gmlId = pMultiSurface->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
      gmlId = createGeometryGmlId();

	if ( gmlId != nullptr )
		writer->WriteAttributeString ("gml:id", gmlId );

	for ( i = 0; i < anz; i++ )
	{
		writer->WriteStartElement ( "gml:surfaceMember" );

		pSurfaceMember = pMultiSurface->getSurface ( i );
		switch ( pSurfaceMember->getGeometryType() )
		{
		case  SURFACE:
			writeSurface ( (Surface*)pSurfaceMember );
			break;

		case POLYGON:
			writePolygon ( "gml:Polygon", (Polygon*)pSurfaceMember );
			break;
		}
		writer->WriteEndElement();
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer gml:MultiCurve			                        						 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeMultiCurve ( String ^ gmlTag, MultiCurve * pMultiCurve )
{
	_Curve  * pCurveMember;
	int       i, anz;
	string    stdString;
  String  ^ gmlId;

	anz = pMultiCurve->getCurveMemberAnz();
	if ( anz == 0 ) return;

	writer->WriteStartElement ( gmlTag );

	stdString = pMultiCurve->getSrsName();
	if ( stdString != "" )
		writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

	gmlId = pMultiCurve->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
      gmlId = createGeometryGmlId();

	if ( gmlId != nullptr )
		writer->WriteAttributeString ("gml:id", gmlId );

	for ( i = 0; i < anz; i++ )
	{
		writer->WriteStartElement ( "gml:curveMember" );

		pCurveMember = pMultiCurve->getCurve ( i );
		switch ( pCurveMember->getCurveType() )
		{
		case  CURVE:
			writeCurve ( (Curve*)pCurveMember );
			break;

		case LINE_STRING:
			writeLineSegment ( "gml:LineString", (LineSegment*)pCurveMember, true );
			break;

		case ARC:
			writeArcSegment ( "gml:Arc", (ArcSegment*)pCurveMember, true );
			break;

		case CIRCLE:
			writeArcSegment ( "gml:Circle", (ArcSegment*)pCurveMember, true );
			break;
		}
		writer->WriteEndElement();
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines gml:MultiPoint	                    										 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeMultiPoint ( MultiPoint * pMultiPoint )
{
	GeoPoint  * pPointMember;
	int         i, anz;
	string      stdString;
  String    ^ gmlId;

	anz = pMultiPoint->getPointAnz();
	if ( anz == 0 ) return;

  writer->WriteStartElement ( "gml:MultiPoint" );

	stdString = pMultiPoint->getSrsName();
	if ( stdString != "" )
		writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

	gmlId = pMultiPoint->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
      gmlId = createGeometryGmlId();

	if ( gmlId != nullptr )
		writer->WriteAttributeString ("gml:id", gmlId );

	for ( i = 0; i < anz; i++ )
	{
		writer->WriteStartElement ( "gml:pointMember" );

		pPointMember = pMultiPoint->getPoint ( i );
		writePoint ( pPointMember );
		
		writer->WriteEndElement();
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Ausgabe _Surface							                            							 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::write_Surface ( _Surface * pSurface )
{
	switch ( pSurface->getGeometryType() )
	{
  case MULTI_SURFACE:
    writeMultiSurface( (MultiSurface*)pSurface );
    break;

	case POLYGON:
		writePolygon ( "gml:Polygon", (Polygon*)pSurface );
		break;

	case SURFACE:
		writeSurface ( (Surface*)pSurface );
		break;
	}
}


///////////////////////////////////////////////////////////////////////////////
//  Ausgabe gml:Surface						                          								 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeSurface ( Surface * pSurface )
{
	Polygon        * pPoly;
	int              i, anz;
	string           stdString;
  String         ^ gmlId;
	vector<Polygon*> vPatches;

	anz = pSurface->getPolygone( vPatches );
	if ( anz == 0 ) return;

  writer->WriteStartElement ( "gml:Surface" );

	stdString = pSurface->getSrsName();
	if ( stdString != "" )
		writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

	gmlId = pSurface->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
      gmlId = createGeometryGmlId();

	if ( gmlId != nullptr )
		writer->WriteAttributeString ("gml:id", gmlId );

	writer->WriteStartElement ( "gml:patches" );

	for ( i = 0; i < anz; i++ )
	{
		pPoly = vPatches[i];
		writePolygon ( "gml:PolygonPatch", pPoly );
	}

	writer->WriteEndElement();
	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Ausgabe gml:Polygon											                            		 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writePolygon ( String ^ gmlTag, Polygon * pPolyP )
{
	unsigned int  i;
	_Ring       * pRing = pPolyP->getAussenkontur();
	string        stdString;
  String      ^ gmlId;

	if ( pRing == NULL ) return;

	writer->WriteStartElement ( gmlTag );

	stdString = pPolyP->getSrsName();
	if ( stdString != "" )
		writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

	gmlId = pPolyP->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
      gmlId = createGeometryGmlId();

	if ( gmlId != nullptr )
		writer->WriteAttributeString ("gml:id", gmlId );

	writer->WriteStartElement ( "gml:exterior" );

	switch ( pRing->getRingType() )
	{
	case RING:
		writeMultiCurve ( "gml:Ring", ((Ring*)pRing)->getRingGeometrie() );
		break;

	case LINEAR_RING:
		writeLineSegment ( "gml:LinearRing", ((LinearRing*)pRing)->getCurve(), false );
		break;
	}

	writer->WriteEndElement();

	for ( i = 0; i < pPolyP->getLochAnz(); i++  )
	{
		pRing = pPolyP->getLoch ( i );
		if ( pRing->getPointAnz() >= 4 ) 
		{
			writer->WriteStartElement ( "gml:interior" );

			switch ( pRing->getRingType() )
			{
			case RING:
				writeMultiCurve ( "gml:Ring", ((Ring*)pRing)->getRingGeometrie() );
				break;

			case LINEAR_RING:
				writeLineSegment ( "gml:LinearRing", ((LinearRing*)pRing)->getCurve(), false );
				break;
			}
			writer->WriteEndElement();
		}
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Ausgabe _Curve													                            		 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::write_Curve ( _Curve * pCurve )
{
	switch ( pCurve->getCurveType() )
	{
	case CURVE:
		writeCurve ( (Curve*)pCurve );
		break;

	case LINE_STRING:
		writeLineSegment ( "gml:LineString", (LineSegment*)pCurve, true );
		break;

	case ARC:
		writeArcSegment ( "gml:ArcString", (ArcSegment*)pCurve, true );
		break;

	case CIRCLE:
		writeArcSegment ( "gml:Circle", (ArcSegment*)pCurve, true );
		break;
	}
}


///////////////////////////////////////////////////////////////////////////////
//  Ausgabe gml:Curve											                            			 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeCurve ( Curve * pCurve )
{
	int       i, anz;
	string    stdString;
  String  ^ gmlId;

	anz = pCurve->getCurveSegmentAnz();
	if ( anz == 0 )  return;

	writer->WriteStartElement ( "gml:Curve" );

  gmlId = pCurve->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
    gmlId = createGeometryGmlId();

  if ( gmlId != nullptr )
    writer->WriteAttributeString ("gml:id", gmlId );

	writer->WriteStartElement ( "gml:segments" );
	for ( i = 0; i < anz; i++ )
	{
		CurveSegment * pCurveSegment = pCurve->getCurveSegment( i );
		switch ( pCurveSegment->getCurveType() )
		{
		case LINE_STRING:
			writeLineSegment ( "LineStringSegment", (LineSegment*)pCurveSegment, false );
			break;

		case ARC:
			writeArcSegment ( "Arc", (ArcSegment*)pCurveSegment, false );
			break;

		case CIRCLE:
			writeArcSegment ( "Circle", (ArcSegment*)pCurveSegment, false );
			break;
		}
	}

	writer->WriteEndElement();
	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Ausgabe GML-LineString						                        							 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeLineSegment ( String ^ gmlTag, LineSegment * pLineSegment, bool writeGmlId  )
{
	vector<double>   vX;
	vector<double>   vY;
  vector<double>   vZ;
	int			         i, anz;
	double		       x, y, z;
	string           stdString;
	String         ^ STR;
	String         ^ STR_A;
  String         ^ gmlId;

	writer->WriteStartElement ( gmlTag );

	stdString = pLineSegment->getSrsName();
	if ( stdString != "" )
		writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

  if ( writeGmlId )
  {
    gmlId = pLineSegment->getGML_ID();
    if ( gmlId == nullptr && gmlVersion == GML_3_2 )
      gmlId = createGeometryGmlId();

    if ( gmlId != nullptr )
      writer->WriteAttributeString ("gml:id", gmlId );
  }
	

  if ( srsDimension == 2 )
	  anz = pLineSegment->getPoints2D ( vX, vY );
  else
    anz = pLineSegment->getPoints3D ( vX, vY, vZ );

	if ( xPlanGmlVersion == XPLANGML_V_2 || xPlanGmlVersion == XPLANGML_V_3  )
	{
		for ( i = 0; i < anz; i++ )
		{
			x = vX[i];
			y = vY[i];
      if ( srsDimension == 3 )
        z = vZ[i];

			if ( srsDimension == 3  )
				STR = System::String::Concat (  QuConvert::ToString ( x, precision ), " ",
												QuConvert::ToString ( y, precision ), " ", 
												QuConvert::ToString ( z, precision ) );
			else
				STR = System::String::Concat (  QuConvert::ToString ( x, precision ), " ",
											QuConvert::ToString ( y, precision ) );

			
		writer->WriteElementString ( "gml:pos", STR );
		}
	}
	else
	{
		writer->WriteStartElement( "gml:posList" );
      writer->WriteAttributeString ( "srsDimension", XmlConvert::ToString( srsDimension ) );
		writer->WriteAttributeString ( "count", XmlConvert::ToString ( anz ) );
		for ( i = 0; i < anz; i++ )
		{
			x = vX[i];
			y = vY[i];
      if ( srsDimension == 3 )
      {
        z = vZ[i];
				STR_A = System::String::Concat ( QuConvert::ToString ( x, precision ), " ",
			                									 QuConvert::ToString ( y, precision ), " ", 
											                	 QuConvert::ToString ( z, precision ), " " );
      }
			else
				STR_A = System::String::Concat ( QuConvert::ToString ( x, precision ), " ",
										  		               QuConvert::ToString ( y, precision ), " " );
			if ( i == 0 )
				STR = STR_A;
			else
				STR = String::Concat ( STR, STR_A );
		}
		writer->WriteString ( STR );
		writer->WriteEndElement();
	}

	writer->WriteEndElement();
}


///////////////////////////////////////////////////////////////////////////////
//  Ausgabe GML-LineString										                        			 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeArcSegment ( String ^ gmlTag, ArcSegment * pArcSegment, bool writeGmlId )
{
	vector<double>   vX;
	vector<double>   vY;
	int			         i, anz;
	double		       x, y, z;
	string           stdString;
	String         ^ STR;
	String         ^ STR_A;
  String         ^ gmlId;

	writer->WriteStartElement ( gmlTag );

  if ( writeGmlId )
  {
    gmlId = pArcSegment->getGML_ID();
    if ( gmlId == nullptr && gmlVersion == GML_3_2 )
      gmlId = createGeometryGmlId();

    if ( gmlId != nullptr )
      writer->WriteAttributeString ("gml:id", gmlId );
  }

	stdString = pArcSegment->getSrsName();
	if ( stdString != "" )
		writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

	z = 0.0;

	anz = pArcSegment->getPoints2D ( vX, vY );

	if ( xPlanGmlVersion <=  XPLANGML_V_3 )
	{
		for ( i = 0; i < anz; i++ )
		{
			x = vX[i];
			y = vY[i];

			if ( srsDimension == 3 )
				STR = System::String::Concat (  QuConvert::ToString ( x, precision ), " ",
												QuConvert::ToString ( y, precision ), " ", 
												QuConvert::ToString ( z, precision ) );
			else
				STR = System::String::Concat (  QuConvert::ToString ( x, precision ), " ",
												QuConvert::ToString ( y, precision ) );
		writer->WriteElementString ( "gml:pos", STR );
		}
	}
	else
	{
		writer->WriteStartElement( "gml:posList" );
      writer->WriteAttributeString ( "srsDimension", XmlConvert::ToString( srsDimension ) );
		writer->WriteAttributeString ( "count", XmlConvert::ToString ( anz ) );
		for ( i = 0; i < anz; i++ )
		{
			x = vX[i];
			y = vY[i];

			if ( srsDimension == 3  )
				STR_A = System::String::Concat (  QuConvert::ToString ( x, precision ), " ",
												  QuConvert::ToString ( y, precision ), " ", 
												  QuConvert::ToString ( z, precision ), " " );
			else
				STR_A = System::String::Concat (  QuConvert::ToString ( x, precision ), " ",
										  		  QuConvert::ToString ( y, precision ), " " );
			if ( i == 0 )
				STR = STR_A;
			else
				STR = String::Concat ( STR, STR_A );
		}
		writer->WriteString ( STR );
		writer->WriteEndElement();
	}

	writer->WriteEndElement();
}


///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines Punktes										                        			 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writePoint ( GeoPoint * pPoint )
{
	String ^ STR;
	string   stdString;
  String ^ gmlId;

  writer->WriteStartElement ( "gml:Point" );

	stdString = pPoint->getSrsName();
	if ( stdString != "" )
		writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

	gmlId = pPoint->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
      gmlId = createGeometryGmlId();

	if ( gmlId != nullptr )
		writer->WriteAttributeString ("gml:id", gmlId );
	
	if ( srsDimension == 3 )
		STR = System::String::Concat ( QuConvert::ToString ( pPoint->getX(), precision ), " ", 
									   QuConvert::ToString ( pPoint->getY(), precision ), " ", 
									   QuConvert::ToString ( 0.0, precision )	   );
	else
		STR = System::String::Concat ( QuConvert::ToString ( pPoint->getX(), precision ), " ", 
									   QuConvert::ToString ( pPoint->getY(), precision )   );
	writer->WriteElementString ( "gml:pos", STR );

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Ausgabe der BoundingBox										                        			 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeBoundedBy( double swX, double swY, double noX, double noY, string srsName )
{
	System::String ^ STR;
	System::String ^ STR_X;
	System::String ^ STR_Y;
	System::String ^ STR_Z;

	xPlanGmlVersion = pPlanZuSchreiben->getGmlObjekte()->getGmlTyp();

	writer->WriteStartElement("gml:boundedBy");

	writer->WriteStartElement("gml:Envelope");

	if ( srsName != "" )
		writer->WriteAttributeString ("srsName", QuConvert::ToString ( srsName ) );

	if ( xPlanGmlVersion <=  XPLANGML_V_3 )
		writer->WriteStartElement("gml:pos");
	else
		writer->WriteStartElement("gml:lowerCorner");

	STR_X = QuConvert::ToString ( swX, precision );
	STR_Y = QuConvert::ToString ( swY, precision );
	if ( srsDimension == 3 )
	{
		STR_Z = QuConvert::ToString ( 0.0, precision );
		STR = System::String::Concat ( STR_X, " ", STR_Y, " ", STR_Z );
	}
	else
		STR = System::String::Concat ( STR_X, " ", STR_Y );

	writer->WriteString ( STR );
	writer->WriteEndElement();
	
	if ( xPlanGmlVersion <=  XPLANGML_V_3 )
		writer->WriteStartElement("gml:pos");
	else
		writer->WriteStartElement("gml:upperCorner");

	STR_X = QuConvert::ToString ( noX, precision );
	STR_Y = QuConvert::ToString ( noY, precision );
	if ( srsDimension == 3 )
	{
		STR_Z = QuConvert::ToString ( 0.0, precision );
		STR = System::String::Concat ( STR_X, " ", STR_Y, " ", STR_Z );
	}
	else
		STR = System::String::Concat ( STR_X, " ", STR_Y );

	writer->WriteString ( STR );
	writer->WriteEndElement();

	writer->WriteEndElement();

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben der Symbol-Position					                      						 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeSymbolPosition ( Feature * pBPlanObjP, string gmlAttributName )
{
	vector<GeoPoint*> symbolPositionen;
	int				  i, anz;   
	GeoPoint		* pPoint;
	String			^ GML_TAG;

  GML_TAG = QuConvert::ToString( gmlAttributName );

  if ( gmlAttributName == "xplan:symbolPosition"  )
	{
		symbolPositionen.clear();
		anz = pBPlanObjP->getSymbolPositionen ( symbolPositionen );
		for ( i = 0; i < anz; i++ )
		{
			pPoint = symbolPositionen[i];
			writer->WriteStartElement ( GML_TAG );
			writePoint ( pPoint );
			writer->WriteEndElement();	
		}
		return;
	}
}


///////////////////////////////////////////////////////////////////////////////
//  Schreiben zur zugeordneten Geometrie						                  			 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeFeatureGeometrie ( Feature * pFeature, string gmlAttributName )
{
//	MultiSurface  * pFlaechenGeometrie;
//	MultiCurve		* pLinienGeometrie;
//	MultiPoint		* pPunktGeometrie;
  String      ^ GML_TAG;
  _Geometrie * pGeo;

  GML_TAG = QuConvert::ToString( gmlAttributName );

  if ( gmlAttributName == "gml:boundedBy" )
  {
    double sw[2], no[2];

    string srsName = pFeature->getSrsName();
    bool success = pFeature->getBoundingBox( sw, no );
    if ( success )
      writeBoundedBy( sw[0], sw[1], no[0], no[1], srsName );

    return;
  }

  pGeo = pFeature->getGeometry( gmlAttributName );
  if ( pGeo != NULL )
  {
    writer->WriteStartElement ( GML_TAG );
    writeGeometrie( pGeo );
    writer->WriteEndElement();
  }

  /*
	switch ( pBPlanObjP->getObjektTyp() )
	{
	case BLEIT_PLAN:
    if ( gmlAttributName == "xplan:raeumlicherGeltungsbereich" )
		{
			pFlaechenGeometrie = pBPlanObjP->getFlaechenGeometrie();
			if ( pFlaechenGeometrie != NULL )
			{
				writer->WriteStartElement ( GML_TAG );

				if ( pFlaechenGeometrie->getSurfaceMemberAnz() == 1 )
					write_Surface ( pFlaechenGeometrie->getSurface( 0 ) );
				else
					writeMultiSurface ( pFlaechenGeometrie );

				writer->WriteEndElement();
			}
		}
		break;

	case PLAN_BEREICH:
    if ( gmlAttributName == "xplan:geltungsbereich" )
		{
			pFlaechenGeometrie = pBPlanObjP->getFlaechenGeometrie();
			if ( pFlaechenGeometrie != NULL )
			{
				writer->WriteStartElement ( GML_TAG );

				if ( pFlaechenGeometrie->getSurfaceMemberAnz() == 1 )
					write_Surface ( pFlaechenGeometrie->getSurface( 0 ) );
				else
					writeMultiSurface ( pFlaechenGeometrie );

				writer->WriteEndElement();
			}
		}
		break;

	case XP_FLAECHEN_OBJEKT: 
	case XP_FPO: 
    if ( gmlAttributName == "xplan:position" )
		{
			pFlaechenGeometrie = pBPlanObjP->getFlaechenGeometrie();
			if ( pFlaechenGeometrie != NULL )
			{
				writer->WriteStartElement ( GML_TAG );

				if ( pFlaechenGeometrie->getSurfaceMemberAnz() == 1 )
					write_Surface (  pFlaechenGeometrie->getSurface( 0 ) );
				else
					writeMultiSurface ( pFlaechenGeometrie );

				writer->WriteEndElement();
			}
		}
		break;

	case XP_LINIEN_OBJEKT: 
	case XP_LPO: 
	case XP_LTO: 
    if ( gmlAttributName == "xplan:position" )
		{
			pLinienGeometrie = pBPlanObjP->getLinienGeometrie();
			if ( pLinienGeometrie != NULL )
			{
				writer->WriteStartElement ( GML_TAG );

				if ( pLinienGeometrie->getCurveMemberAnz() == 1 )
					write_Curve ( pLinienGeometrie->getCurve( 0 ) );
				else
					writeMultiCurve ( "gml:MultiCurve", pLinienGeometrie );
				
				writer->WriteEndElement();
			}
		}
		break;

	case XP_PUNKT_OBJEKT:
	case XP_PTO:
	case XP_PPO:
    if ( gmlAttributName == "xplan:position"  )
		{
			pPunktGeometrie = pBPlanObjP->getPunktGeometrie();
			if ( pPunktGeometrie != NULL )
			{
				writer->WriteStartElement ( GML_TAG );

				if ( pPunktGeometrie->getPointAnz() == 1 )
					writePoint ( pPunktGeometrie->getPoint( 0 ) );
				else
					writeMultiPoint ( pPunktGeometrie );
			
				writer->WriteEndElement();
			}
		}
		break;

	case XP_GEOMETRIE_OBJEKT:
    if ( gmlAttributName == "xplan:position" ||  gmlAttributName == "plu:geometry" )
		{
			switch ( pBPlanObjP->getGeometrieTyp() )
			{
			case MULTI_SURFACE:
				pFlaechenGeometrie = pBPlanObjP->getFlaechenGeometrie();
				if ( pFlaechenGeometrie != NULL )
				{
					writer->WriteStartElement ( GML_TAG );

					if ( pFlaechenGeometrie->getSurfaceMemberAnz() == 1 )
						write_Surface ( pFlaechenGeometrie->getSurface( 0 ) );
					else
						writeMultiSurface ( pFlaechenGeometrie );

					writer->WriteEndElement();
				}
				break;

			case MULTI_CURVE:
				pLinienGeometrie = pBPlanObjP->getLinienGeometrie();
				if ( pLinienGeometrie != NULL )
				{
					writer->WriteStartElement ( GML_TAG );

					if ( pLinienGeometrie->getCurveMemberAnz() == 1 )
						write_Curve ( pLinienGeometrie->getCurve( 0 ) );
					else
						writeMultiCurve ( "gml:MultiCurve", pLinienGeometrie );
					
					writer->WriteEndElement();
				}
			break;

			case MULTI_POINT:
				pPunktGeometrie = pBPlanObjP->getPunktGeometrie();
				if ( pPunktGeometrie != NULL )
				{
					writer->WriteStartElement ( GML_TAG );

					if ( pPunktGeometrie->getPointAnz() == 1 )
						writePoint ( pPunktGeometrie->getPoint( 0 ) );
					else
						writeMultiPoint ( pPunktGeometrie );
			
					writer->WriteEndElement();
					break;
				}

			}
		}
	}
*/
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer GML-Geometrie                                            //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeGeometrie ( _Geometrie * pGeometrie )
{
  switch ( pGeometrie->getGeometryType() )
  {
  case GEO_POINT:
    writePoint ( dynamic_cast<GeoPoint*>(pGeometrie) );
    break;

  case MULTI_POINT:
    writeMultiPoint ( dynamic_cast<MultiPoint*>(pGeometrie) );
    break;

  case LINE_STRING:
    writeLineSegment ( "gml:LineString", dynamic_cast<LineSegment*>(pGeometrie), true );
    break;

  case CURVE:
    writeCurve ( dynamic_cast<Curve*>(pGeometrie) );
    break;

  case MULTI_CURVE:
    writeMultiCurve ( "gml:MultiCurve", dynamic_cast<MultiCurve*>(pGeometrie) );
    break;

  case POLYGON:
    writePolygon ( "gml:Polygon", dynamic_cast<Polygon*>(pGeometrie) );
    break;

  case SURFACE:
    writeSurface ( dynamic_cast<Surface*>(pGeometrie) );
    break;

  case MULTI_SURFACE:
    writeMultiSurface ( dynamic_cast<MultiSurface*>(pGeometrie) );
    break;

  case SOLID:
    writeSolid ( dynamic_cast<Solid*>(pGeometrie) );
    break;

  case MULTI_SOLID:
    writeMultiSolid ( dynamic_cast<MultiSolid*>(pGeometrie) );
    break;

  case ALLG_GEOMETRIE:
    writeMultiGeometry ( dynamic_cast<MultiGeometry*>(pGeometrie) );
    break;

  case IMPLICIT_GEOMETRY:
    writeImplicitGeometry( dynamic_cast<ImplicitGeometry*>(pGeometrie) );
    break;
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine gml:id für Geometrieobjekte                               //
///////////////////////////////////////////////////////////////////////////////
String ^ GMLWriter::createGeometryGmlId()
{
  String ^ STR;

  anzGeoWritten++;
  STR = String::Concat ( "GEO_", Convert::ToString ( anzGeoWritten ) );

  return STR; 
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben von gml:Solid                                                  //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeSolid ( Solid * pSolid )
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben von gml:MultiSolid                                             //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeMultiSolid ( MultiSolid * pSolid )
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben von gml:Geometry                                               //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeMultiGeometry ( MultiGeometry * pGeometry )
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben von core:ImplicitGeometry                                      //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeImplicitGeometry ( ImplicitGeometry * pImplicitGemetry )
{

}


///////////////////////////////////////////////////////////////////////////////
//  XPlanGMLWriter										                              				 //
//  Erzeugen einer XPlanGML-Datei							                      				 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor									                        			 //
///////////////////////////////////////////////////////////////////////////////
XPlanGMLWriter::XPlanGMLWriter ( Features * pPlanZuSchreibenP, int anzObjWriteMaxP )
:GMLWriter ( pPlanZuSchreibenP, anzObjWriteMaxP )
{
	gmlIdTag  = "gml:id";

	objIdWritten = new _set;
}
XPlanGMLWriter::~ XPlanGMLWriter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Aufruf writer													                              		 //
//  outputFormat 0: Rausschreiben als xplan:PlanAuszug bzw. plu:SpatialPlan  //
//  outputFormat 1: Rausschreiben als gml:FeatureCollection                  //
//  outputFormat 2: Rausschreiben als wfs:FeatureCollection                  //
///////////////////////////////////////////////////////////////////////////////
int XPlanGMLWriter::write ( std::string fileNameP, int outputFormat )
{
	System::String ^ fileName;
	System::String ^ STR;
	System::String ^ date;
	System::String ^ comment;
	string			     srsName;
  string           kuerzel;
  string           gmlId;
	int			         i, j, planAnz, relAnz;
	double			     sw[2], no[2];
	char             buffer[16];

	vector<BLeitPlan*>       vBPlaene;
	vector<Feature*>      vBPlanRelationen;
	vector<Feature*>      vBPlanObjekte;

  if ( pPlanZuSchreiben->getSpatialPlan() != NULL )
    return writeSpatialPlan ( fileNameP, outputFormat );

	_strdate_s ( buffer, 16 );
	date = QuConvert::ToString ( string ( buffer ) );

	fileName = QuConvert::ToString ( fileNameP );
	anzObjWrite = 0;

	System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();
	writer								            = gcnew XmlTextWriter( fileName, utf8);

	writer->Formatting = Formatting::Indented;
	writer->WriteStartDocument( true );

	comment = "Erzeugt mit KIT (www.kit.edu) XPlanGML-Toolbox, Erstellungsdatum: ";
	comment = String::Concat ( comment, date );

	writer->WriteComment( comment );

  writer->WriteStartElement("XPlanAuszug");

	if ( pPlanZuSchreiben->getGmlObjekte()->getGmlTyp() ==  XPLANGML_V_2 )
	{
		writer->WriteAttributeString("xmlns",	    "http://www.xplanung.de/xplangml");
		writer->WriteAttributeString("xmlns:xplan",	"http://www.xplanung.de/xplangml");
	  writer->WriteAttributeString("xmlns:gml",   "http://www.opengis.net/gml");
    writer->WriteAttributeString("xmlns:wfs",	"http://www.opengis.net/wfs");
    writer->WriteAttributeString("xmlns:xlink", "http://www.w3.org/1999/xlink" );
	}
	else
	if ( pPlanZuSchreiben->getGmlObjekte()->getGmlTyp() ==  XPLANGML_V_3 )
	{
		writer->WriteAttributeString("xmlns",	    "http://www.xplanung.de/xplangml/3/0");
		writer->WriteAttributeString("xmlns:xplan",	"http://www.xplanung.de/xplangml/3/0");
    writer->WriteAttributeString("xmlns:gml",   "http://www.opengis.net/gml");
    writer->WriteAttributeString("xmlns:wfs",	"http://www.opengis.net/wfs");
    writer->WriteAttributeString("xmlns:xlink", "http://www.w3.org/1999/xlink" );
	}
	else
	if ( pPlanZuSchreiben->getGmlObjekte()->getGmlTyp() ==  XPLANGML_V_4 )
	{
		writer->WriteAttributeString("xmlns",	    "http://www.xplanung.de/xplangml/4/0");
		writer->WriteAttributeString("xmlns:xplan",	"http://www.xplanung.de/xplangml/4/0");
	  writer->WriteAttributeString("xmlns:gml",   "http://www.opengis.net/gml/3.2");
    writer->WriteAttributeString("xmlns:wfs",	"http://www.adv-online.de/namespaces/adv/gid/wfs");
    writer->WriteAttributeString("xmlns:xlink", "http://www.w3.org/1999/xlink" );
  }
/*    if ( pPlanZuSchreiben->getBLeitplanViewer()->getXPlanADESchemaPfad() != "" )
    {
      ADESchemaPfad      = QuConvert::ToString ( pPlanZuSchreiben->getBLeitplanViewer()->getXPlanADESchemaPfad() );
      ADESchemaNamespace = QuConvert::ToString( pPlanZuSchreiben->getBLeitplanViewer()->getXplanADENamespace() );
      kuerzel = pPlanZuSchreiben->getBLeitplanViewer()->getGmlNamespaces()->getKuerzel (  pPlanZuSchreiben->getBLeitplanViewer()->getXplanADENamespace() );

      STR = String::Concat( "xmlns:",  QuConvert::ToString( kuerzel ) );
      writer->WriteAttributeString( STR, ADESchemaNamespace );
    }
*/
	

  /*
  if ( pPlanZuSchreiben->getBLeitplanViewer()->getXPlanADESchemaPfad() != "" )
  {
    schemaLocation = String::Concat ( ADESchemaNamespace, " file:/", ADESchemaPfad );
  }
  else
  {
	  xplanSchemaPfad = pPlanZuSchreiben->getBLeitplanViewer()->getXPlanOperationenSchemaPfad();
	  STR = QuConvert::ToString ( xplanSchemaPfad );
	  STR = String::Concat ( "file:/", STR );
	  if ( pPlanZuSchreiben->getBLeitplanViewer()->getGmlObjekte()->getGmlTyp() ==  XPLANGML_V_2 )
		  schemaLocation = System::String::Concat ( "http://www.xplanung.de/xplangml ", STR );
	  else
	  if ( pPlanZuSchreiben->getBLeitplanViewer()->getGmlObjekte()->getGmlTyp() ==  XPLANGML_V_3 )
		  schemaLocation = System::String::Concat ( "http://www.xplanung.de/xplangml/3/0 ", STR );
	  else
	  if ( pPlanZuSchreiben->getBLeitplanViewer()->getGmlObjekte()->getGmlTyp() ==  XPLANGML_V_4 )
		  schemaLocation = System::String::Concat ( "http://www.xplanung.de/xplangml/4/0 ", STR );
  }
	writer->WriteAttributeString ( "xsi:schemaLocation", schemaLocation );
*/

  gmlId = pPlanZuSchreiben->getGmlId();
  if ( gmlId == "" )
    gmlId = pPlanZuSchreiben->getBLeitplanViewer()->createGmlId();
	writer->WriteAttributeString ( "gml:id", QuConvert::ToString ( gmlId ) );

	pPlanZuSchreiben->getBoundingBox ( sw, no ); 
	srsName =  pPlanZuSchreiben->getSrsName();

	writeBoundedBy ( sw[0], sw[1], no[0], no[1], srsName );

	planAnz = pPlanZuSchreiben->getPlaene ( vBPlaene );
	for ( i = 0;  i < planAnz; i++ )
	{
		BLeitPlan * pFPlan = vBPlaene[i];
		vBPlanRelationen.clear();
		relAnz = writeBPlan ( pFPlan, vBPlanRelationen );

		for ( j = 0; j < relAnz; j++ )
		{
			Feature * pBPlanRelObj = vBPlanRelationen[j];

			if ( objIdWritten->existId ( pBPlanRelObj->getGmlId() ) == false )
				writeBPlanRelation ( pBPlanRelObj );
		}
	}

	writer->WriteEndElement();

	if ( anzObjWrite >= anzObjWriteMax )
	{
		STR = String::Concat ( "In dieser Version können nur", Convert::ToString ( anzObjWriteMax ), " GML-Objekte geschrieben werden" );
		MessageBox::Show ( STR );
	}

	writer->Flush();
	writer->Close();

	return anzObjWrite;
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben INSPIRE Planned Land Use - Spatial Plan                    //
///////////////////////////////////////////////////////////////////////////////
int XPlanGMLWriter::writeSpatialPlan ( std::string fileNameP, int outputFormat )
{
  String                    ^ fileName;
  String                    ^ STR;
  String                    ^ NAMESPACE;
  String                    ^ date;
  String                    ^ comment;
  string			                srsName;
  string                      stdString;
  string                      kuerzel;
  string                      gmlId;
  StringAttr                * stringAttributWert;
  string                      externalCodeListsPfad;
  IntAttr			              * intAttributWert;
  DoubleAttr				        * doubleAttributWert;
  BoolAttr        	        * boolAttributWert;
  FeatureRelation           * pRelation;
  Feature                * pRelationZiel;
  ATTRIBUT_TYP	    	        gmlAttributTyp;
  string				              gmlAttributName;
  int			                    i, j, anz, gmlAttributAnz;
  double                      sw[2], no[2];
  unsigned int                k;
  int					                attributWertAnz;
  bool                        success;
  bool                        isNilable;
  char                        buffer[16];
  vector<string>              vKuerzel;
  vector<string>              vNamespaces;
  vector<StringAttr*>  	      stringAttributWerte;
  vector<IntAttr*>            intAttributWerte;
  vector<DoubleAttr*>         doubleAttributWerte;
  vector<FeatureRelation*>    relationen;
  vector<Feature*>         planObjekte;
  vector<GmlAttribut*>        gmlAttribute;
  GmlElement                * pGmlElement;
  GmlAttribut		            * pGmlAttribut;
  GmlKlasse                 * pBPlanKlasse;
  GmlObjekte                * pGmlObjekte = pPlanZuSchreiben->getGmlObjekte();
  GmlNamespaces             * pGmlNamespaces = pPlanZuSchreiben->getGmlNamespaces();
  GeometryValuePairCoverage * pSpatialPlan = pPlanZuSchreiben->getSpatialPlan();

  String			              ^ GML_ATTRIBUT_NAME;
  String                    ^ ATTRIBUT_NAME;
  String                    ^ ATTRIBUT_WERT;

  _strdate_s ( buffer, 16 );
  date = QuConvert::ToString ( string ( buffer ) );

  fileName = QuConvert::ToString ( fileNameP );
  anzObjWrite = 0;

  System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();
  writer								            = gcnew XmlTextWriter( fileName, utf8);

  writer->Formatting = Formatting::Indented;
  writer->WriteStartDocument( true );

  comment = "Erzeugt mit KIT (www.kit.edu) XPlanGML-Toolbox, Erstellungsdatum: ";
  comment = String::Concat ( comment, date );

  writer->WriteComment( comment );

  switch ( outputFormat )
  {
  case 0:
    writer->WriteStartElement("plu:SpatialPlan");
    break;

  case 1:
    writer->WriteStartElement("gml:FeatureCollection" );
    break;

  case 2:
    writer->WriteStartElement("wfs:FeatureCollection" );
    break;

  }

  pGmlNamespaces->addNamespace( "xlink", "http://www.w3.org/1999/xlink" );
  pGmlNamespaces->addNamespace( "xsi", "http://www.w3.org/2001/XMLSchema-instance" );
  anz = pGmlNamespaces->getNamespaces ( vKuerzel, vNamespaces );
  success = false;
  for ( j = 0; j < anz; j++ )
  {
    stdString = vKuerzel[j];
    STR = String::Concat ( "xmlns:", QuConvert::ToString ( stdString ) );
    stdString = vNamespaces[j];
    NAMESPACE = QuConvert::ToString ( stdString );
    writer->WriteAttributeString ( STR, NAMESPACE );
  }

  gmlId = pSpatialPlan->getGmlId();
  if ( outputFormat != 0 )
    gmlId = gmlId + "_C";
  writer->WriteAttributeString ( "gml:id", QuConvert::ToString ( gmlId ) );

  pGmlElement = pGmlObjekte->getGmlElement( pSpatialPlan->getKlassenName () );
  if ( pGmlElement == NULL ) return 0;

  pBPlanKlasse = pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );
  if ( pBPlanKlasse == NULL )	return 0;

  pPlanZuSchreiben->getBoundingBox ( sw, no ); 
  srsName =  pPlanZuSchreiben->getSrsName();
  writeBoundedBy ( sw[0], sw[1], no[0], no[1], srsName );

  if ( outputFormat != 0 )
  {
    writer->WriteStartElement("gml:featureMember" );
    writer->WriteStartElement( "plu:SpatialPlan" );
    writer->WriteAttributeString ( "gml:id", QuConvert::ToString ( pSpatialPlan->getGmlId() ) );
  }

  gmlAttributAnz = pBPlanKlasse->getGmlAttribute ( gmlAttribute );
  for ( i = 0; i < gmlAttributAnz; i++ )
  {
    stringAttributWerte.clear();
    intAttributWerte.clear();
    doubleAttributWerte.clear();
    relationen.clear();

    pGmlAttribut   = gmlAttribute[i];
    gmlAttributTyp = pGmlAttribut->getGmlAttributTyp();
    gmlAttributName = pGmlAttribut->getGmlAttributName();
    isNilable       = pGmlAttribut->getNillable();

    GML_ATTRIBUT_NAME = QuConvert::ToString ( gmlAttributName );

    switch ( gmlAttributTyp )
    {
    case _ENUMERATION:
    case _STRING:
      attributWertAnz = pSpatialPlan->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
      if ( attributWertAnz == 0 && isNilable )
      {
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
      }
      break;

    case _EXTERNAL_CODE_LIST:
      attributWertAnz = pSpatialPlan->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
      if ( attributWertAnz == 0 && isNilable  )
      {
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString (  QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
      }
      break;
    case _ANY_URI:
      attributWertAnz = pSpatialPlan->getURLAttributWerte ( gmlAttributName, stringAttributWerte );
      if ( attributWertAnz == 0 && isNilable )
      {
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString (  QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
      }
      break;

    case _DATE:
    case _DATE_TIME:
      attributWertAnz = pSpatialPlan->getDateAttributWerte ( gmlAttributName, stringAttributWerte );
      if ( attributWertAnz == 0 && isNilable )
      {
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString (  QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
      }
      break;

    case _DOUBLE:
      attributWertAnz = pSpatialPlan->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
      if ( attributWertAnz == 0 && isNilable )
      {
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else
      for ( j = 0; j < attributWertAnz; j++ )
      {
        doubleAttributWert = doubleAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < doubleAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( doubleAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( doubleAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString (  XmlConvert::ToString ( doubleAttributWert->value ) );
        writer->WriteEndElement();
      }
      break;

    case _INTEGER:
      attributWertAnz = pSpatialPlan->getIntegerAttributWerte ( gmlAttributName, intAttributWerte );
      if ( attributWertAnz == 0 && isNilable )
      {
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else
      for ( j = 0; j < attributWertAnz; j++ )
      {
        intAttributWert = intAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < intAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( intAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( intAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString (  XmlConvert::ToString ( intAttributWert->value ) );
        writer->WriteEndElement();
      }
      break;

    case _BOOLEAN:
      boolAttributWert = pSpatialPlan->getBoolAttributWert ( gmlAttributName  );
      if ( boolAttributWert != NULL )
      {
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < boolAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( boolAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( boolAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString (  XmlConvert::ToString ( boolAttributWert->value ) );
        writer->WriteEndElement();
      }
      else
      if ( attributWertAnz == 0 && isNilable )
      {
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }

      break;

    case _DATA_TYPE:
      attributWertAnz = pSpatialPlan->getFeatureRelationen ( gmlAttributName, relationen );
      if ( attributWertAnz == 0 && isNilable )
      {
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else
      for ( j = 0; j < attributWertAnz; j++ )
      {
        pRelation = relationen[j];
        if ( !pRelation->nil )
        {
          pRelationZiel = pPlanZuSchreiben->getFeature ( pRelation->featureId );
          if ( pRelationZiel == NULL ) break;		
        }

        writer->WriteStartElement( GML_ATTRIBUT_NAME );	
        for ( k = 0; k < pRelation->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( pRelation->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( pRelation->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        if ( !pRelation->nil )
          writeBPlanObjekt ( pRelationZiel );		
        writer->WriteEndElement();
      }

      break;

    case _REFERENZ:
      attributWertAnz = pSpatialPlan->getFeatureRelationen ( gmlAttributName, relationen );
      if ( attributWertAnz == 0 && isNilable )
      {
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else
      for ( j = 0; j < attributWertAnz; j++ )
      {
        pRelation = relationen[j];
        pRelationZiel = pPlanZuSchreiben->getFeature ( pRelation->featureId );
        if ( pRelationZiel == NULL ) break;		

        writer->WriteStartElement( GML_ATTRIBUT_NAME );		
        for ( k = 0; k < pRelation->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( pRelation->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( pRelation->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writeBPlanObjekt ( pRelationZiel );
        writer->WriteEndElement();
      }

      break;

    case  _DOMAIN_SET:
      writer->WriteStartElement( "gml:domainSet" );
      writer->WriteEndElement();
//      writeDomainSet ( pSpatialPlan );
      break;

    case _RANGE_SET:
      writeRangeSet ( pSpatialPlan );
      break;

    case _RANGE_TYPE:
      writer->WriteStartElement( "gmlcov:rangeType" );
      writer->WriteEndElement();
      break;

    case _CVGVP_DOMAIN_EXTENT:
      writer->WriteStartElement( "cvgvp:domainExtent" );
      writer->WriteEndElement();
      break;

    case _CVGVP_ELEMENT:
      writeCoverageGeometryValuePairs ( pSpatialPlan );
      break;

    case _GEOMETRIE:
      if ( gmlAttributName == "lucm:extent" )
        writeSpatialPlanExtent ( pSpatialPlan );
      break;

    case _SONST:
      break;
    }
  }

  if ( outputFormat != 0 )
  {
    writer->WriteEndElement();
    writer->WriteEndElement();
  }

  writer->Flush();
  writer->Close();

  return anzObjWrite;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt den DomainSet des SpatialPlan                                   //
///////////////////////////////////////////////////////////////////////////////
void XPlanGMLWriter::writeMultiSurfaceDomain( GeometryValuePairCoverage * pSpatialPlan )
{
  /*
  unsigned int   i;
  string         stdString;
  MultiSurface * pMultiSurface;
  
  writer->WriteStartElement( "gml:multiSurfaceDomain" );

  writer->WriteStartElement ( "gml:MultiSurface");

  stdString = pSpatialPlan->getGmlId();
  stdString = stdString + "_Domain";
  writer->WriteAttributeString( "gml:id", QuConvert::ToString( stdString ) );

  stdString = pSpatialPlan->getSrsName();
  if ( stdString != "" )
    writer->WriteAttributeString( "srsName", QuConvert::ToString( stdString ) );

  for ( i = 0; i < pSpatialPlan->getSurfaceValuePairAnz(); i++ )
  {
    SurfaceValuePair * pPair = pSpatialPlan->getSurfaceValuePair( i );

    writer->WriteStartElement( "gml:surfaceMember" );

    switch ( pPair->pSurface->getGeometryType() )
    {
    case MULTI_SURFACE:
      pMultiSurface = dynamic_cast<MultiSurface*>(pPair->pSurface);
      if ( pMultiSurface->getIsCompositeSurface() )
        writeMultiSurface( "gml:CompositeSurface", pMultiSurface  );
      else
        writeMultiSurface( "gml:MultiSurface", pMultiSurface  );
      break;

    case SURFACE:
      writeSurface( "gml:Surface", dynamic_cast<Surface*>(pPair->pSurface) );
      break;

    case POLYGON:
      writePolygon( "gml:Polygon", dynamic_cast<Polygon*>(pPair->pSurface) );
      break;
    }
    writer->WriteEndElement();
  }

  writer->WriteEndElement();
  writer->WriteEndElement();
  */
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt den RangeSet des SpatialPlan                                    //
///////////////////////////////////////////////////////////////////////////////
void XPlanGMLWriter::writeRangeSet( GeometryValuePairCoverage * pSpatialPlan )
{
  writer->WriteStartElement( "gml:rangeSet" );

  writer->WriteStartElement ( "gml:File" );

  writer->WriteStartElement( "gml:rangeParameters" );
  writer->WriteEndElement();

  writer->WriteStartElement( "gml:fileReference" );
  writer->WriteEndElement();

  writer->WriteStartElement( "gml:fileStructure" );
  writer->WriteEndElement();

  writer->WriteEndElement();

  writer->WriteEndElement();

  /*
  unsigned int i;

  writer->WriteStartElement( "gml:rangeSet" );

  for ( i = 0; i < pSpatialPlan->getSurfaceValuePairAnz(); i++ )
  {
    SurfaceValuePair * pPair = pSpatialPlan->getSurfaceValuePair( i );
    writeBPlanObjekt( pPair->pValue );
  }

  writer->WriteEndElement();
  */
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt den Extent des Spatial Plan                                     //
///////////////////////////////////////////////////////////////////////////////
void XPlanGMLWriter::writeSpatialPlanExtent( GeometryValuePairCoverage * pSpatialPlan )
{
  _Geometrie * pGeometrie = pSpatialPlan->getGeometry( "lucm:extent" );
  if ( pGeometrie == NULL ) return;

  writer->WriteStartElement( "lucm:extent" );
  writer->WriteStartElement( "gmd:EX_BoundingPolygon" );
  writer->WriteStartElement ( "gmd:polygon" );

  writeGeometrie( pGeometrie );

  writer->WriteEndElement();
  writer->WriteEndElement();
  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben der Geometry-Value Pairs der Coverage                      //
///////////////////////////////////////////////////////////////////////////////
void XPlanGMLWriter::writeCoverageGeometryValuePairs( GeometryValuePairCoverage * pSpatialPlan )
{
  unsigned int        i;
  GeometryValuePair * pPair;

  for ( i = 0; i < pSpatialPlan->getGeometryValuePairAnz(); i++ )
  {
    pPair = pSpatialPlan->getGeometryValuePair( i );

    writer->WriteStartElement ( "cvgvp:element" );
    writer->WriteStartElement( "cvgvp:GeometryValuePair" );
    
    writer->WriteStartElement( "cvgvp:geometry" );

    switch ( pPair->pGeometry->getGeometryType() )
    {
    case SURFACE:
      write_Surface( dynamic_cast<_Surface*>(pPair->pGeometry) );
      break;

    case POLYGON:
      writePolygon ( "gml:Polygon", dynamic_cast<Polygon*>(pPair->pGeometry) );
      break;

    case MULTI_SURFACE:
      writeMultiSurface( dynamic_cast<MultiSurface*>(pPair->pGeometry) );
      break;

    }

    writer->WriteEndElement();

    writer->WriteStartElement( "cvgvp:value" );
    writeBPlanObjekt ( pPair->pValue );
    writer->WriteEndElement();

    writer->WriteEndElement();
    writer->WriteEndElement();
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben BPlan			                          											 //
///////////////////////////////////////////////////////////////////////////////
int XPlanGMLWriter::writeBPlan ( BLeitPlan * pBPlanP, vector<Feature*> &planObjekteP )
{
	vector<GmlAttribut*> gmlAttribute;
	string				       gmlId;
	string               name;
	GmlAttribut		     * pGmlAttribut;
	int					         gmlAttributAnz;
	int					         attributWertAnz;
	int				           i, j;
  unsigned int         k;
	ATTRIBUT_TYP	    	 gmlAttributTyp;
	string				       gmlAttributName;
	StringAttr	       * stringAttributWert;
  string               externalCodeListsPfad;
	IntAttr					   * intAttributWert;
	DoubleAttr				 * doubleAttributWert;
	BoolAttr			     * boolAttributWert;
	FeatureRelation *    pRelation;
	Feature      *    pRelationZiel;
	vector<StringAttr*>	 stringAttributWerte;
	vector<IntAttr*>     intAttributWerte;
	vector<DoubleAttr*>  doubleAttributWerte;
	vector<FeatureRelation*> relationen;
	vector<Feature*>      planObjekte;

	String			   ^ GML_ATTRIBUT_NAME;
	String			   ^ GML_ID;
  String         ^ ATTRIBUT_NAME;
  String         ^ ATTRIBUT_WERT;

	GmlObjekte * pGmlObjekte = pPlanZuSchreiben->getGmlObjekte();
  GmlElement * pGmlElement = pGmlObjekte->getGmlElement( pBPlanP->getKlassenName () );
  if ( pGmlElement == NULL ) return 0;
	GmlKlasse * pBPlanKlasse = pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );
	if ( pBPlanKlasse == NULL )	return 0;

	writer->WriteStartElement("gml:featureMember");  // <gml:featureMember>
	anzObjWrite++;

	name = pBPlanP->getKlassenName ();
	gmlId = pBPlanP->getGmlId();
	GML_ID = QuConvert::ToString ( gmlId );

	writer->WriteStartElement( QuConvert::ToString ( name ) );		// <bplan:BPlan gml:id="XXX">
	writer->WriteAttributeString ( gmlIdTag, GML_ID );

	gmlAttributAnz = pBPlanKlasse->getGmlAttribute ( gmlAttribute );
	for ( i = 0; i < gmlAttributAnz; i++ )
	{
		stringAttributWerte.clear();
		intAttributWerte.clear();
		doubleAttributWerte.clear();
		relationen.clear();

		pGmlAttribut   = gmlAttribute[i];
		gmlAttributTyp = pGmlAttribut->getGmlAttributTyp();
		gmlAttributName = pGmlAttribut->getGmlAttributName();
		
		GML_ATTRIBUT_NAME = QuConvert::ToString ( gmlAttributName );

		switch ( gmlAttributTyp )
		{
		case _ENUMERATION:
		case _STRING:
			attributWertAnz = pBPlanP->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;
    
    case _EXTERNAL_CODE_LIST:
      attributWertAnz = pBPlanP->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString (  QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
      }
      break;
		case _ANY_URI:
			attributWertAnz = pBPlanP->getURLAttributWerte ( gmlAttributName, stringAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString (  QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _DATE:
			attributWertAnz = pBPlanP->getDateAttributWerte ( gmlAttributName, stringAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString (  QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _DOUBLE:
			attributWertAnz = pBPlanP->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				doubleAttributWert = doubleAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < doubleAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( doubleAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( doubleAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
				writer->WriteString ( XmlConvert::ToString ( doubleAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _INTEGER:
			attributWertAnz = pBPlanP->getIntegerAttributWerte ( gmlAttributName, intAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				intAttributWert = intAttributWerte[j];
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < intAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( intAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( intAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( XmlConvert::ToString ( intAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _BOOLEAN:
			boolAttributWert = pBPlanP->getBoolAttributWert ( gmlAttributName );
			if ( boolAttributWert != NULL )
			{
        writer->WriteStartElement( GML_ATTRIBUT_NAME );
        for ( k = 0; k < boolAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( boolAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( boolAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( XmlConvert::ToString ( boolAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _DATA_TYPE:
			attributWertAnz = pBPlanP->getFeatureRelationen ( gmlAttributName, relationen );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				pRelation = relationen[j];
				pRelationZiel = pPlanZuSchreiben->getFeature ( pRelation->featureId );
				if ( pRelationZiel == NULL ) break;		

				writer->WriteStartElement( GML_ATTRIBUT_NAME );	
        for ( k = 0; k < pRelation->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( pRelation->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( pRelation->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
				writeBPlanObjekt ( pRelationZiel );		
				writer->WriteEndElement();
			}

			break;

		case _REFERENZ:
			attributWertAnz = pBPlanP->getFeatureRelationen ( gmlAttributName, relationen );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				pRelation = relationen[j];
				pRelationZiel = pPlanZuSchreiben->getFeature ( pRelation->featureId );
				if ( pRelationZiel == NULL ) break;		

				writer->WriteStartElement( GML_ATTRIBUT_NAME );		
        for ( k = 0; k < pRelation->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( pRelation->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( pRelation->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
				GML_ID = String::Concat ( "#", QuConvert::ToString (  pRelation->featureId ) );
				writer->WriteAttributeString ( "xlink:href", GML_ID );
				planObjekteP.push_back ( pRelationZiel );

				writer->WriteEndElement();
			}

			break;
			
		case _GEOMETRIE:
				writeFeatureGeometrie ( pBPlanP, gmlAttributName );
        break;

		case _POS_XY:
			writeSymbolPosition ( pBPlanP, gmlAttributName );
      break;

		case _SONST:
			break;
		}
	}

	writer->WriteEndElement();
	writer->WriteEndElement();



	return planObjekteP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreigt die Relations-Ziele der Klasse BPlan		                  			 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGMLWriter::writeBPlanRelation ( Feature * pBPlanRelObjP )
{
	vector<GmlAttribut*> gmlAttribute;
	vector<Feature*>  planObjekte;
	string				       gmlId;
	string               name;
  string               externalCodeListsPfad;
	GmlAttribut		     * pGmlAttribut;
	int					         gmlAttributAnz;
	int					         attributWertAnz;
	int				           i, j, anz;
  unsigned int         k;
	ATTRIBUT_TYP	    	 gmlAttributTyp;
	string				       gmlAttributName;
	StringAttr				 * stringAttributWert;
	IntAttr					   * intAttributWert;
	DoubleAttr			   * doubleAttributWert;
	BoolAttr			     * boolAttributWert;
	bool			        	 multiple;
	FeatureRelation *    pRelation;
	Feature      *    pRelationZiel;
	vector<StringAttr*>  stringAttributWerte;
	vector<IntAttr*>     intAttributWerte;
	vector<DoubleAttr*>  doubleAttributWerte;
	vector<FeatureRelation*> relationen;

	String			   ^ GML_ATTRIBUT_NAME;
	String			   ^ ATTRIBUT_NAME;
	String			   ^ ATTRIBUT_WERT;
	String			   ^ GML_ID;

	if ( anzObjWrite >= anzObjWriteMax )
		return;

	writer->WriteStartElement("gml:featureMember");  // <gml:featureMember>
	anzObjWrite++;

	GmlObjekte * pGmlObjekte = pPlanZuSchreiben->getGmlObjekte();
  GmlElement * pGmlElement = pGmlObjekte->getGmlElement( pBPlanRelObjP->getKlassenName () );
  if ( pGmlElement == NULL ) return;
	GmlKlasse * pBPlanKlasse = pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );
	if ( pBPlanKlasse == NULL )	return;

	name = pBPlanRelObjP->getKlassenName ();
	gmlId = pBPlanRelObjP->getGmlId();

	writer->WriteStartElement ( QuConvert::ToString ( name ) );
	writer->WriteAttributeString ( gmlIdTag, QuConvert::ToString ( gmlId ) );

	objIdWritten->addId ( pBPlanRelObjP->getGmlId() );

	gmlAttributAnz = pBPlanKlasse->getGmlAttribute ( gmlAttribute );
	for ( i = 0; i < gmlAttributAnz; i++ )
	{
		stringAttributWerte.clear();
		intAttributWerte.clear();
		doubleAttributWerte.clear();
		relationen.clear();

		pGmlAttribut   = gmlAttribute[i];
		gmlAttributTyp = pGmlAttribut->getGmlAttributTyp();
		gmlAttributName = pGmlAttribut->getGmlAttributName();
		multiple		= pGmlAttribut->getMultiple();
		
		GML_ATTRIBUT_NAME = QuConvert::ToString ( gmlAttributName );

		switch ( gmlAttributTyp )
		{
		case _ENUMERATION:
		case _STRING:
			attributWertAnz = pBPlanRelObjP->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;
			for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];
				writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
				{
					ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
					ATTRIBUT_WERT = QuConvert::ToString (stringAttributWert->classifierValue[k] );
					writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
				}
				writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
				writer->WriteEndElement();
			}
			break;

    case _EXTERNAL_CODE_LIST:
      attributWertAnz = pBPlanRelObjP->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString (stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
      }
      break;


		case _ANGLE:
		case _LENGTH:
		case _AREA:
		case _VOLUME:
			attributWertAnz = pBPlanRelObjP->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;
			for ( j = 0; j < attributWertAnz; j++ )
			{
				writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        doubleAttributWert = doubleAttributWerte[j];
        for ( k = 0; k < doubleAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( doubleAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString (doubleAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( XmlConvert::ToString ( doubleAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;


		case _ANY_URI:
			attributWertAnz = pBPlanRelObjP->getURLAttributWerte ( gmlAttributName, stringAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;
			for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString (stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _DATE:
    case _DATE_TIME:
			attributWertAnz = pBPlanRelObjP->getDateAttributWerte ( gmlAttributName, stringAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;
			for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString (stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _DOUBLE:
			attributWertAnz = pBPlanRelObjP->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;
			for ( j = 0; j < attributWertAnz; j++ )
			{
				doubleAttributWert = doubleAttributWerte[j];
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < doubleAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( doubleAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString (doubleAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( XmlConvert::ToString ( doubleAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _INTEGER:
			attributWertAnz = pBPlanRelObjP->getIntegerAttributWerte ( gmlAttributName, intAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;
			for ( j = 0; j < attributWertAnz; j++ )
			{
				intAttributWert = intAttributWerte[j];
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < intAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( intAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString (intAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( XmlConvert::ToString ( intAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _BOOLEAN:
			boolAttributWert = pBPlanRelObjP->getBoolAttributWert ( gmlAttributName );
			if ( boolAttributWert != NULL )
			{
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < boolAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( boolAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString (boolAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( XmlConvert::ToString ( boolAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _DATA_TYPE:
			attributWertAnz = pBPlanRelObjP->getFeatureRelationen ( gmlAttributName, relationen );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;
			for ( j = 0; j < attributWertAnz; j++ )
			{
				pRelation = relationen[j];
				pRelationZiel = pPlanZuSchreiben->getFeature ( pRelation->featureId );
				if ( pRelationZiel == NULL ) break;		

				writer->WriteStartElement( GML_ATTRIBUT_NAME );	
        for ( k = 0; k < pRelation->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( pRelation->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString (pRelation->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
				writeBPlanObjekt ( pRelationZiel );		
				writer->WriteEndElement();
			}

			break;

		case _REFERENZ:
			attributWertAnz = pBPlanRelObjP->getFeatureRelationen ( gmlAttributName, relationen );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;
			for ( j = 0; j < attributWertAnz; j++ )
			{
				pRelation = relationen[j];
				pRelationZiel = pPlanZuSchreiben->getFeature ( pRelation->featureId );
				if ( pRelationZiel == NULL )
					break;		

				writer->WriteStartElement( GML_ATTRIBUT_NAME );		
        for ( k = 0; k < pRelation->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( pRelation->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( pRelation->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
				GML_ID = String::Concat ( "#", QuConvert::ToString (  pRelation->featureId ) );
				writer->WriteAttributeString ( "xlink:href", GML_ID );
        if ( pRelation->rollenName != "xplan:gehoertZuPlan" &&
          pRelation->rollenName != "xplan:gehoertZuBereich" &&
          pRelation->rollenName != "xplan:gehoertNachrichtlichZuBereich" &&
          pRelation->rollenName != "xplan:gehoertZuBP_Bereich" &&
          pRelation->rollenName != "xplan:gehoertZuFP_Bereich" &&
          pRelation->rollenName != "xplan:gehoertZuLP_Bereich" &&
          pRelation->rollenName != "xplan:gehoertZuRP_Bereich" &&
          pRelation->rollenName != "xplan:gehoertZuSO_Bereich" &&
          pRelation->rollenName != "xplan:dientZurDarstellungVon" &&
					objIdWritten->existId ( pRelation->featureId ) == false )
					planObjekte.push_back ( pRelationZiel );

				writer->WriteEndElement();
			}

			break;
			
		case _GEOMETRIE:
			writeFeatureGeometrie ( pBPlanRelObjP, gmlAttributName );
			break;


		case _POS_XY:
			writeSymbolPosition ( pBPlanRelObjP, gmlAttributName );
      break;

		case _SONST:
			break;
		}
	}

	writer->WriteEndElement();
	writer->WriteEndElement();

	anz = planObjekte.size();
	for ( i = 0; i < anz; i++ )
	{
		Feature * pPlanObjekt = planObjekte[i];
		writeBPlanRelation ( pPlanObjekt );
	}

}


///////////////////////////////////////////////////////////////////////////////
//  Schreibt Objekt				  									                            	 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGMLWriter::writeBPlanObjekt ( Feature * pBPlanObjP )
{
	string					       name;
	string				      	 klassenName;
	string					       gmlId;
  string                 externalCodeListsPfad;
	int						         i, j;
  unsigned int           k;
	vector<GmlAttribut*>	 gmlAttribute;
	GmlAttribut			     * pGmlAttribut;
	int						         gmlAttributAnz;
	int					        	 attributWertAnz;
	ATTRIBUT_TYP		    	 gmlAttributTyp;
	string				      	 gmlAttributName;
	StringAttr           * stringAttributWert;
	IntAttr						   * intAttributWert;
	DoubleAttr					 * doubleAttributWert;
	BoolAttr					   * boolAttributWert;
	bool				        	 multiple;
  bool                   nillable;
	FeatureRelation		   * pRelation;
	Feature			     * pRelationZiel;
	vector<StringAttr*>	   stringAttributWerte;
	vector<IntAttr*>	     intAttributWerte;
	vector<DoubleAttr*>    doubleAttributWerte;
	vector<FeatureRelation*> relationen;
	vector<Feature*>      planObjekte;

	String				   ^ GML_ATTRIBUT_NAME;
	String				   ^ ATTRIBUT_NAME;
	String				   ^ ATTRIBUT_WERT;
	String			     ^ GML_ID;

	if ( anzObjWrite >= anzObjWriteMax ) 
		return;


	GmlObjekte * pGmlObjekte = pPlanZuSchreiben->getGmlObjekte();
	klassenName = pBPlanObjP->getKlassenName ();
  GmlElement * pGmlElement = pGmlObjekte->getGmlElement( klassenName );
  if ( pGmlElement == NULL )
    return;
	GmlKlasse * pBPlanKlasse = pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );
	if ( pBPlanKlasse == NULL )
		return;

  anzObjWrite++;

	name = pBPlanObjP->getKlassenName ();
	writer->WriteStartElement ( QuConvert::ToString ( name ) );

  gmlId = pBPlanObjP->getGmlId();

  if ( gmlId != "" && pBPlanKlasse->getIsGmlFeature() )
    writer->WriteAttributeString ( gmlIdTag, QuConvert::ToString ( gmlId ) );

	gmlAttributAnz = pBPlanKlasse->getGmlAttribute ( gmlAttribute );
	for ( i = 0; i < gmlAttributAnz; i++ )
	{
		stringAttributWerte.clear();
		intAttributWerte.clear();
		doubleAttributWerte.clear();
		relationen.clear();

		pGmlAttribut    = gmlAttribute[i];
		gmlAttributTyp  = pGmlAttribut->getGmlAttributTyp();
		gmlAttributName = pGmlAttribut->getGmlAttributName();
		multiple		    = pGmlAttribut->getMultiple();
    nillable        = pGmlAttribut->getNillable();
		
		GML_ATTRIBUT_NAME = QuConvert::ToString ( gmlAttributName );

		switch ( gmlAttributTyp )
		{
		case _ENUMERATION:
		case _STRING:
			attributWertAnz = pBPlanObjP->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;
      if ( attributWertAnz == 0 && nillable )
      {
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else
			for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];
				writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
				writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
				writer->WriteEndElement();
			}
			break;

    case _EXTERNAL_CODE_LIST:
      attributWertAnz = pBPlanObjP->getStringAttributWerte ( gmlAttributName, stringAttributWerte );

      if ( attributWertAnz == 0 && nillable )
      {
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else      
      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
      }
      break;
		case _ANY_URI:
			attributWertAnz = pBPlanObjP->getURLAttributWerte ( gmlAttributName, stringAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      if ( attributWertAnz == 0 && nillable )
      {
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else      
			for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
				writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _DATE:
    case _DATE_TIME:
			attributWertAnz = pBPlanObjP->getDateAttributWerte ( gmlAttributName, stringAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      if ( attributWertAnz == 0 && nillable )
      {
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else      
      for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( stringAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( stringAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _ANGLE:
		case _LENGTH:
		case _AREA:
		case _VOLUME:
			attributWertAnz = pBPlanObjP->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      if ( attributWertAnz == 0 && nillable )
      {
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else      
      for ( j = 0; j < attributWertAnz; j++ )
			{
        doubleAttributWert = doubleAttributWerte[j];
				writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( doubleAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( doubleAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
				writer->WriteString ( XmlConvert::ToString ( doubleAttributWert->value ) );
				writer->WriteEndElement();
			}
			break;


		case _DOUBLE:
			attributWertAnz = pBPlanObjP->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      if ( attributWertAnz == 0 && nillable )
      {
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else      
      for ( j = 0; j < attributWertAnz; j++ )
			{
				doubleAttributWert = doubleAttributWerte[j];
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < stringAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( doubleAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( doubleAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( XmlConvert::ToString ( doubleAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _INTEGER:
			attributWertAnz = pBPlanObjP->getIntegerAttributWerte ( gmlAttributName, intAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      if ( attributWertAnz == 0 && nillable )
      {
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else      
      for ( j = 0; j < attributWertAnz; j++ )
			{
				intAttributWert = intAttributWerte[j];
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < intAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( intAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( intAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( XmlConvert::ToString ( intAttributWert->value ) );
        writer->WriteEndElement();
			}
			break;

		case _BOOLEAN:
			boolAttributWert = pBPlanObjP->getBoolAttributWert ( gmlAttributName );
			if ( boolAttributWert != NULL )
			{
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        for ( k = 0; k < boolAttributWert->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( boolAttributWert->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( boolAttributWert->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        writer->WriteString ( XmlConvert::ToString ( boolAttributWert->value ) );
        writer->WriteEndElement();
			}
      else
      if ( attributWertAnz == 0 && nillable )
      {
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }   

			break;

		case _DATA_TYPE:
			attributWertAnz = pBPlanObjP->getFeatureRelationen ( gmlAttributName, relationen );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      if ( attributWertAnz == 0 && nillable )
      {
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else      
      for ( j = 0; j < attributWertAnz; j++ )
			{
				pRelation = relationen[j];
        if ( !pRelation->nil )
        {
				  pRelationZiel = pPlanZuSchreiben->getFeature ( pRelation->featureId );
				  if ( pRelationZiel == NULL ) break;		
        }

				writer->WriteStartElement( GML_ATTRIBUT_NAME );	
        for ( k = 0; k < pRelation->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( pRelation->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( pRelation->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
        if ( !pRelation->nil )
				  writeBPlanObjekt ( pRelationZiel );		
				writer->WriteEndElement();
			}

			break;

		case _REFERENZ:
			attributWertAnz = pBPlanObjP->getFeatureRelationen ( gmlAttributName, relationen );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      if ( attributWertAnz == 0 && nillable )
      {
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        writer->WriteEndElement();
      }
      else      
      for ( j = 0; j < attributWertAnz; j++ )
			{
				pRelation = relationen[j];
				pRelationZiel = pPlanZuSchreiben->getFeature ( pRelation->featureId );
				if ( pRelationZiel == NULL ) break;		

				writer->WriteStartElement( GML_ATTRIBUT_NAME );	
        for ( k = 0; k < pRelation->classifierName.size(); k++ )
        {
          ATTRIBUT_NAME = QuConvert::ToString ( pRelation->classifierName[k] );
          ATTRIBUT_WERT = QuConvert::ToString ( pRelation->classifierValue[k] );
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
        }
//        if ( gmlVersion == XPLANGML_V_2 || gmlVersion == XPLANGML_V_3 || gmlVersion == XPLANGML_V_4 || 
//             gmlVersion == NAS_5_1 || gmlVersion == NAS_6_0 )
//        {
		    GML_ID = String::Concat ( "#", QuConvert::ToString (  pRelation->featureId ) );
		    writer->WriteAttributeString ( "xlink:href", GML_ID );
//        }
//        else
//        {
//          writeBPlanObjekt ( pRelationZiel );		
//        }
				
				writer->WriteEndElement();
			}

			break;
			
		case _GEOMETRIE:
				writeFeatureGeometrie ( pBPlanObjP, gmlAttributName );
			break;


		case _POS_XY:
			writeSymbolPosition ( pBPlanObjP, gmlAttributName );
      break;

		case _SONST:
			break;
		}
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  _set																	 //
///////////////////////////////////////////////////////////////////////////////
void _set::addId ( std::string id )
{
	ids.insert ( id );
}
bool _set::existId ( std::string id )
{
	if ( ids.find ( id ) != ids.end() )
		return true;
	else
		return false;
}
void _set::clear()
{
	ids.clear();
}













