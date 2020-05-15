#include "StdAfx.h"

#include <time.h>
#include <string>
#include <map>
#include <sstream>
#include <set>

#include "Convert.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "Geometrie.h"
#include "GmlSchema.h"
#include "GmlNamespaces.h"
#include "TimeX.h"
#include "UnitOfMeasure.h"
#include "TimeSeries.h"
#include "Constant.h"
#include "GMLWriter.h"

///////////////////////////////////////////////////////////////////////////////
//  GMLWriter                                                                //
//  Export von GML-Dateien              				                  					 //
///////////////////////////////////////////////////////////////////////////////
GMLWriter::GMLWriter ( Features * pFeatureP )
{
	pFeatures        = pFeatureP;
  srsDimension     = pFeatures->getSrsDimension();
  anzObjWrite      = 0;
  anzGeoWritten    = 0;
  outputFormat     = 0;
  pGmlSchema       = pFeatures->getGmlSchema();
  gmlVersion       = pGmlSchema->getGmlVersion();
  pSchemaTypeNames = pGmlSchema->getSchemaTypeNames();

  precision    = 7;

  gmlIdTag  = "gml:id";

  xlinkFeatures = new _set;
  writtenFeatures = new _set;

  writeXLinks = true;
}

GMLWriter::~GMLWriter()
{
  delete xlinkFeatures;
  delete writtenFeatures;
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer gml:MultiSurface		                        						 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeMultiSurface ( MultiSurface * pMultiSurface )
{
	_Surface * pSurfaceMember;
	size_t     i, anz;
	string     stdString;
  String   ^ gmlId;

	anz = pMultiSurface->getSurfaceMemberAnz();
	if ( anz == 0 ) return;

  if ( pMultiSurface->getGeometryType() == COMPOSITE_SURFACE )
	  writer->WriteStartElement ( "gml:CompositeSurface" );
  else
  if ( pMultiSurface->getGeometryType() == MULTI_SURFACE )
    writer->WriteStartElement ( "gml:MultiSurface" );
  else
  if ( pMultiSurface->getGeometryType() == SHELL )
    writer->WriteStartElement ( "gml:Shell" );
  else
    return;

  if ( pMultiSurface->getGeometryType() != SHELL )
  {
	  stdString = pMultiSurface->getSrsName();
	  if ( stdString != "" )
		  writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

    srsDimension = pMultiSurface->getDim();

	  gmlId = pMultiSurface->getGML_ID();
    if ( gmlId == nullptr && gmlVersion == GML_3_2 )
        gmlId = createGeometryGmlId();

	  if ( gmlId != nullptr )
		  writer->WriteAttributeString ("gml:id", gmlId );
  }

	for ( i = 0; i < anz; i++ )
	{
		writer->WriteStartElement ( "gml:surfaceMember" );

		pSurfaceMember = pMultiSurface->getSurface ( i );
    write_Surface( pSurfaceMember );
		
		writer->WriteEndElement();
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer gml:MultiCurve			                        						 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeMultiCurve ( String ^ gmlTag, MultiCurve * pMultiCurve, bool isRing )
{
	_Curve  * pCurveMember;
	size_t    i, anz;
	string    stdString;
  String  ^ gmlId;

	anz = pMultiCurve->getCurveMemberAnz();
	if ( anz == 0 ) return;

	writer->WriteStartElement ( gmlTag );

  if ( !isRing || gmlVersion != GML_3_2 )
  {
	  stdString = pMultiCurve->getSrsName();
	  if ( stdString != "" )
		  writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

	  gmlId = pMultiCurve->getGML_ID();
    if ( gmlId == nullptr && gmlVersion == GML_3_2 )
        gmlId = createGeometryGmlId();

	  if ( gmlId != nullptr )
      writer->WriteAttributeString ("gml:id", gmlId );
  }

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
    case LINE_STRING_SEGMENT:
			writeLineSegment ( (LineSegment*)pCurveMember);
			break;

		case ARC:
			writeArcSegment ( (ArcSegment*)pCurveMember );
			break;

		case CIRCLE:
			writeArcSegment ( (ArcSegment*)pCurveMember );
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
	size_t      i, anz;
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
//  Schreiben einer Flächengeometrie (Klasse _Surface)         							 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::write_Surface ( _Surface * pSurface )
{
  string stdString;
  String ^ STR;

	switch ( pSurface->getGeometryType() )
	{
  case MULTI_SURFACE:
  case COMPOSITE_SURFACE:
  case SHELL:
    writeMultiSurface( (MultiSurface*)pSurface );
    break;

	case POLYGON:
		writePolygon ( "gml:Polygon", (PolygonGeo*)pSurface, false );
		break;

	case SURFACE:
		writeSurface ( (Surface*)pSurface );
		break;

  case TRIANGULATED_SURFACE:
    writeTriangulatedSurface ( (TriangulatedSurface*)pSurface );
    break;


  case ORIENTABLE_SURFACE:
    writeOrientableSurface( (OrientableSurface*)pSurface );
    break;

  case SURFACE_REFERENCE:
    if ( writeXLinks )
    {
      stdString = "#" + dynamic_cast<SurfaceReference*>(pSurface)->getRefGeoId();
      STR = QuConvert::ToString( stdString );
      writer->WriteAttributeString( "xlink:href", STR );
    }
    else
    {
      _Geometrie * pGeo =  dynamic_cast<SurfaceReference*>(pSurface)->getRefGeoObjekt();
      writeGeometrie( pGeo );
    }
    break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer gml:Surface		                          								 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeSurface ( Surface * pSurface )
{
	PolygonGeo     * pPoly;
	size_t           i, anz;
	string           stdString;
  String         ^ gmlId;
	vector<PolygonGeo*> vPatches;

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
		writePolygon ( "gml:PolygonPatch", pPoly, true );
	}

	writer->WriteEndElement();
	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer gml:TriangulatedSurface                  								 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeTriangulatedSurface ( TriangulatedSurface * pSurface )
{
  PolygonGeo     * pPoly;
	size_t           i, anz;
  string           stdString;
  String         ^ gmlId;
  vector<PolygonGeo*> vPatches;

  anz = pSurface->getPolygone( vPatches );
  if ( anz == 0 ) return;

  writer->WriteStartElement ( "gml:TriangulatedSurface" );

  stdString = pSurface->getSrsName();
  if ( stdString != "" )
    writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

  gmlId = pSurface->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
    gmlId = createGeometryGmlId();

  if ( gmlId != nullptr )
    writer->WriteAttributeString ("gml:id", gmlId );

  writer->WriteStartElement ( "gml:trianglePatches" );

    for ( i = 0; i < anz; i++ )
    {
      pPoly = vPatches[i];
      writePolygon ( "gml:Triangle", pPoly, true );
    }

    writer->WriteEndElement();
    writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines gml:Polygon							                            		 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writePolygon ( String ^ gmlTag, PolygonGeo * pPolyP, bool isPatch )
{
	unsigned int  i;
	_Ring       * pRing = pPolyP->getAussenkontur();
	string        stdString;
  String      ^ gmlId;

	if ( pRing == NULL ) return;

	writer->WriteStartElement ( gmlTag );

  if ( !isPatch )
  {
	  stdString = pPolyP->getSrsName();
	  if ( stdString != "" )
		  writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

	  gmlId = pPolyP->getGML_ID();
    if ( gmlId == nullptr && gmlVersion == GML_3_2 )
        gmlId = createGeometryGmlId();

	  if ( gmlId != nullptr )
		  writer->WriteAttributeString ("gml:id", gmlId );
  }

	writer->WriteStartElement ( "gml:exterior" );

	switch ( pRing->getRingType() )
	{
	case RING:
		writeMultiCurve ( "gml:Ring", ((Ring*)pRing)->getRingGeometrie(), true );
		break;

	case LINEAR_RING:
		writeLineSegment ( ((LinearRing*)pRing)->getCurve() );
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
				writeMultiCurve ( "gml:Ring", ((Ring*)pRing)->getRingGeometrie(), true );
				break;

			case LINEAR_RING:
				writeLineSegment ( ((LinearRing*)pRing)->getCurve() );
				break;
			}
			writer->WriteEndElement();
		}
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer Liniengeometrie (Klasse _Curve)                      		 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::write_Curve ( _Curve * pCurve )
{
  string stdString;
  String ^ STR;

	switch ( pCurve->getCurveType() )
	{
	case CURVE:
		writeCurve ( (Curve*)pCurve );
		break;

	case LINE_STRING:
	case LINE_STRING_SEGMENT:
		writeLineSegment ((LineSegment*)pCurve );
		break;

	case ARC:
		writeArcSegment ( (ArcSegment*)pCurve );
		break;

	case CIRCLE:
		writeArcSegment ( (ArcSegment*)pCurve );
		break;

  case CURVE_REFERENCE:
    if ( writeXLinks )
    {
      stdString = "#" + dynamic_cast<CurveReference*>(pCurve)->getRefGeoId();
      STR = QuConvert::ToString( stdString );
      writer->WriteAttributeString( "xlink:href", STR );
    }
    else
    {
      _Geometrie * pGeo =  dynamic_cast<CurveReference*>(pCurve)->getRefGeoObjekt();
      writeGeometrie( pGeo );
    }
    break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer gml:Curve			  			                            			 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeCurve ( Curve * pCurve )
{
	size_t    i, anz;
	string    stdString;
  String  ^ gmlId;

	anz = pCurve->getCurveSegmentAnz();
	if ( anz == 0 )  return;

  if ( anz == 1 &&  pCurve->getCurveSegment( 0 ) ->getCurveType() == LINE_STRING )
  {
    writeLineSegment ( (LineSegment*)pCurve->getCurveSegment( 0 ) );
    return;
  }

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
    case LINE_STRING_SEGMENT:
      writeLineSegment (  (LineSegment*)pCurveSegment );
			break;

		case ARC:
    case ARC_STRING:
    case CIRCLE:
      writeArcSegment ( (ArcSegment*)pCurveSegment );
			break;
  
		}
  }

	writer->WriteEndElement();
	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines Liniensegmentes (Klasse LineSegment)       							 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeLineSegment ( LineSegment * pLineSegment )
{
	vector<double>   vX;
	vector<double>   vY;
  vector<double>   vZ;
	size_t           i, anz;
	double		       x, y, z;
	string           stdString;
	string           interpolation;
	String         ^ STR;
	String         ^ STR_A;
  String         ^ gmlId;
  int              srsDimensionLoc;

  switch ( pLineSegment->getLineSegmentType() )
  {
  case LineSegment::_LINEAR_RING:
    writer->WriteStartElement ( "gml:LinearRing" );
    if ( gmlVersion != GML_3_2 )
    {
      gmlId = pLineSegment->getGML_ID();
      if ( gmlId != nullptr )
        writer->WriteAttributeString ("gml:id", gmlId );
    }
    break;

  case LineSegment::_LINE_STRING:
    writer->WriteStartElement ( "gml:LineString" );
    gmlId = pLineSegment->getGML_ID();
    if ( gmlId == nullptr && gmlVersion == GML_3_2 )
      gmlId = createGeometryGmlId();
    writer->WriteAttributeString ("gml:id", gmlId );    
    break;

  case LineSegment::_LINE_STRING_SEGMENT:
    writer->WriteStartElement ( "gml:LineStringSegment" );
    if ( gmlVersion != GML_3_2 )
    {
      gmlId = pLineSegment->getGML_ID();
      if ( gmlId != nullptr )
        writer->WriteAttributeString ("gml:id", gmlId );
    }
		interpolation = pLineSegment->getInterpolation();
		if ( interpolation  != "" )
		{
			STR = QuConvert::ToString ( interpolation );
			writer->WriteAttributeString ("interpolation", STR );
		}
    break;
  }
   srsDimensionLoc = pLineSegment->getDim();
  if ( srsDimensionLoc == -1 )
    srsDimensionLoc = srsDimension;
	

  if ( srsDimensionLoc == 2 )
	  anz = pLineSegment->getPoints2D ( vX, vY );
  else
    anz = pLineSegment->getPoints3D ( vX, vY, vZ );

	if ( gmlSchemaType == XPLANGML_2_0 || gmlSchemaType == XPLANGML_3_0  )
	{
		for ( i = 0; i < anz; i++ )
		{
			x = vX[i];
			y = vY[i];
      if ( srsDimensionLoc == 3 )
        z = vZ[i];

			if ( srsDimensionLoc == 3  )
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
      writer->WriteAttributeString ( "srsDimension", XmlConvert::ToString( srsDimensionLoc ) );
		writer->WriteAttributeString ( "count", XmlConvert::ToString ( anz ) );
		for ( i = 0; i < anz; i++ )
		{
			x = vX[i];
			y = vY[i];
      if ( srsDimensionLoc == 3 )
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
//  Schreiben eines Kreisbogensegmentes (Klasse ArcSegment)            			 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeArcSegment ( ArcSegment * pArcSegment )
{
	vector<double>   vX;
	vector<double>   vY;
	size_t	         i, anz;
	double		       x, y, z;
	string           stdString;
	String         ^ STR;
	String         ^ STR_A;
  String         ^ gmlId;
  int              srsDimensionLoc;

  switch ( pArcSegment->getArcSegmentType() )
  {
  case ArcSegment::_ARC_STRING:
    writer->WriteStartElement ( "gml:ArcString" );

    STR = XmlConvert::ToString( pArcSegment->getNumArc() );
    writer->WriteAttributeString( "numArc", STR );
    break;

  case ArcSegment::_ARC:
    writer->WriteStartElement ( "gml:Arc" );

    writer->WriteAttributeString( "numArc", "1" );
    break;

  case ArcSegment::_CIRCLE:
    writer->WriteStartElement ( "gml:Circle" );

    writer->WriteAttributeString( "numArc", "1" );
    break;
  }

  if ( pArcSegment->getInterpolation() != "" )
    writer->WriteAttributeString( "interpolation", QuConvert::ToString( pArcSegment->getInterpolation() ) );

  if ( gmlVersion != GML_3_2 )
  {
    gmlId = pArcSegment->getGML_ID();
    if ( gmlId != nullptr )
      writer->WriteAttributeString ("gml:id", gmlId );

    stdString = pArcSegment->getSrsName();
    if ( stdString != "" )
      writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );
  }

  srsDimensionLoc = pArcSegment->getDim();
  if ( srsDimensionLoc == -1 )
    srsDimensionLoc = srsDimension;

	z = 0.0;

	anz = pArcSegment->getPoints2D ( vX, vY );

	if ( gmlSchemaType <=  XPLANGML_3_0 )
	{
		for ( i = 0; i < anz; i++ )
		{
			x = vX[i];
			y = vY[i];

			if ( srsDimensionLoc == 3 )
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
      writer->WriteAttributeString ( "srsDimension", XmlConvert::ToString( srsDimensionLoc ) );
		writer->WriteAttributeString ( "count", XmlConvert::ToString ( anz ) );
		for ( i = 0; i < anz; i++ )
		{
			x = vX[i];
			y = vY[i];

			if ( srsDimensionLoc == 3  )
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
//  Schreiben eines Punktes	(Klasse GeoPoint)	                        			 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writePoint ( GeoPoint * pPoint )
{
	String ^ STR;
	string   stdString;
  String ^ gmlId;
  int      srsDimensionLoc;

  writer->WriteStartElement ( "gml:Point" );

	stdString = pPoint->getSrsName();
	if ( stdString != "" )
		writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

  srsDimensionLoc = pPoint->getDim();
  if ( srsDimensionLoc == -1 )
    srsDimensionLoc = srsDimension;

	gmlId = pPoint->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
      gmlId = createGeometryGmlId();

	if ( gmlId != nullptr )
		writer->WriteAttributeString ("gml:id", gmlId );
	
	if ( srsDimensionLoc == 3 )
		STR = System::String::Concat ( 
                     QuConvert::ToString ( pPoint->getX(), precision ), " ", 
									   QuConvert::ToString ( pPoint->getY(), precision ), " ", 
									   QuConvert::ToString ( pPoint->getZ(), precision ) );
	else
		STR = System::String::Concat ( 
                     QuConvert::ToString ( pPoint->getX(), precision ), " ", 
									   QuConvert::ToString ( pPoint->getY(), precision )   );

	writer->WriteElementString ( "gml:pos", STR );
	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer DirectPosition						                        			 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeDirectPosition ( DirectPosition * pDirectPosition )
{
  string         stdString;
  vector<double> vDirectPosition;
  String       ^ STR;
  unsigned int   i;

  stdString = pDirectPosition->getSrsName();
  if ( stdString != "" )
    writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

  pDirectPosition->get ( vDirectPosition );

  STR = "";
  for ( i = 0; i < vDirectPosition.size(); i++ )
  {
    STR = String::Concat( STR, QuConvert::ToString( vDirectPosition[i] ) );
    if ( i < vDirectPosition.size() - 1 )
      STR = String::Concat( STR, "  " );
  }
  writer->WriteString( STR );
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Ausgabe von core:externalReference                                       //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeCityGMLExternalReference ( Feature * pExternalReference )
{
  string           namespaceKuerzel;
  string           str;
  string           attributName, attributWert;
  vector<Feature*> vRelFeatures;

  if ( pFeatures->getGmlTyp() == CITYGML_0_4_0 )
    namespaceKuerzel = "citygml:";
  else
    namespaceKuerzel = "core:";

  str = namespaceKuerzel + "externalReference";
  writer->WriteStartElement( QuConvert::ToString( str ) );

  attributName = namespaceKuerzel + "informationSystem";
  bool success = pExternalReference->getURLAttributWert( attributName, attributWert );
  if ( success )
    writer->WriteElementString( QuConvert::ToString( attributName ), QuConvert::ToString( attributWert ) );

  attributName = namespaceKuerzel + "externalObject";
	size_t anz = pExternalReference->getRelatedFeatures( attributName, vRelFeatures );
  if ( anz > 0 )
  {
    Feature * pExternalObject = vRelFeatures[0];
    writer->WriteStartElement( QuConvert::ToString( attributName ) );

    attributName = namespaceKuerzel + "name";
    success = pExternalObject->getStringAttributWert( attributName, attributWert );
    if ( success )
      writer->WriteElementString( QuConvert::ToString( attributName ), QuConvert::ToString( attributWert ) );

    attributName = namespaceKuerzel + "uri";
    success = pExternalObject->getURLAttributWert( attributName, attributWert );
    if ( success )
      writer->WriteElementString( QuConvert::ToString( attributName ), QuConvert::ToString( attributWert ) );

    writer->WriteEndElement();
  }

  writer->WriteEndElement(); 
}


///////////////////////////////////////////////////////////////////////////////
//  Schreiben von gml:Envelope								                        			 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeBoundedBy( double * sw, double * no, string srsName )
{
	System::String ^ STR;
	System::String ^ STR_X;
	System::String ^ STR_Y;
	System::String ^ STR_Z;

	gmlSchemaType = pGmlSchema->getGmlTyp();

	writer->WriteStartElement("gml:boundedBy");

	writer->WriteStartElement("gml:Envelope");

	if ( srsName != "" )
		writer->WriteAttributeString ("srsName", QuConvert::ToString ( srsName ) );

	if ( gmlSchemaType ==  XPLANGML_2_0 ||
       gmlSchemaType ==  XPLANGML_3_0 ||
       gmlSchemaType ==  NAS_5_1  )
		writer->WriteStartElement("gml:pos");
	else
		writer->WriteStartElement("gml:lowerCorner");

	STR_X = QuConvert::ToString ( sw[0], precision );
	STR_Y = QuConvert::ToString ( sw[1], precision );

	if ( srsDimension == 3 )
	{
		STR_Z = QuConvert::ToString ( sw[2], precision );
		STR = System::String::Concat ( STR_X, " ", STR_Y, " ", STR_Z );
	}
	else
		STR = System::String::Concat ( STR_X, " ", STR_Y );

	writer->WriteString ( STR );
	writer->WriteEndElement();
	
  if ( gmlSchemaType ==  XPLANGML_2_0 ||
       gmlSchemaType ==  XPLANGML_3_0 ||
       gmlSchemaType ==  NAS_5_1 )
		writer->WriteStartElement("gml:pos");
	else
		writer->WriteStartElement("gml:upperCorner");

	STR_X = QuConvert::ToString ( no[0], precision );
	STR_Y = QuConvert::ToString ( no[1], precision );
	if ( srsDimension == 3 )
	{
		STR_Z = QuConvert::ToString ( no[2], precision );
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
//  Nur XPlanGML:                                                            //
//  Schreiben der Symbol-Position					                      						 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeSymbolPosition ( Feature * pBPlanObjP, string gmlAttributName )
{
	vector<GeoPoint*> symbolPositionen;
	size_t				  i, anz;   
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
//  Schreiben eines Geometrie-Attributs eines Features                			 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeFeatureGeometrie ( Feature * pFeature, string gmlAttributName )
{
  String      ^ GML_TAG;
  _Geometrie * pGeo;
  string       srsName;
  int          srsDimension;

  GML_TAG = QuConvert::ToString( gmlAttributName );

  if ( gmlAttributName == "gml:boundedBy" )
  {
    double sw[3], no[3];

    bool success = pFeature->getBoundingBox( sw, no, srsName, srsDimension, false );
    if ( success )
      writeBoundedBy( sw, no, srsName );

    return;
  }

  for ( int i = 0; i < pFeature->getGeometryPropertyAnz( gmlAttributName ); i++ )
  {
    pGeo = pFeature->getGeometry( gmlAttributName, i );
    if ( pGeo != NULL )
    {
      writer->WriteStartElement ( GML_TAG );
      writeGeometrie( pGeo );
      writer->WriteEndElement();
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben einer GML-Geometrie (Klasse _Geometrie)                        //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeGeometrie ( _Geometrie * pGeometrie )
{
  string   stdString;
  String ^ STR;

  switch ( pGeometrie->getGeometryType() )
  {
  case GEO_POINT:
    writePoint ( dynamic_cast<GeoPoint*>(pGeometrie) );
    break;

  case MULTI_POINT:
    writeMultiPoint ( dynamic_cast<MultiPoint*>(pGeometrie) );
    break;

	case LINE_STRING:
	case LINE_STRING_SEGMENT:
    writeLineSegment ( dynamic_cast<LineSegment*>(pGeometrie) );
    break;

  case CURVE:
    writeCurve ( dynamic_cast<Curve*>(pGeometrie) );
    break;

  case MULTI_CURVE:
    writeMultiCurve ( "gml:MultiCurve", dynamic_cast<MultiCurve*>(pGeometrie), false );
    break;

  case POLYGON:
    writePolygon ( "gml:Polygon", dynamic_cast<PolygonGeo*>(pGeometrie), false );
    break;

  case SURFACE:
    writeSurface ( dynamic_cast<Surface*>(pGeometrie) );
    break;

  case TRIANGULATED_SURFACE:
    writeTriangulatedSurface ( dynamic_cast<TriangulatedSurface*>(pGeometrie) );
    break;

  case MULTI_SURFACE:
  case COMPOSITE_SURFACE:
  case SHELL:
    writeMultiSurface ( dynamic_cast<MultiSurface*>(pGeometrie) );
    break;

  case SOLID:
    writeSolid ( dynamic_cast<Solid*>(pGeometrie) );
    break;

  case MULTI_SOLID:
    writeMultiSolid ( dynamic_cast<MultiSolid*>(pGeometrie) );
    break;

  case MULTI_GEOMETRY:
    writeMultiGeometry ( dynamic_cast<MultiGeometry*>(pGeometrie) );
    break;

  case IMPLICIT_GEOMETRY:
    writeImplicitGeometry( dynamic_cast<ImplicitGeometry*>(pGeometrie) );
    break;

  case ORIENTABLE_SURFACE:
    writeOrientableSurface( dynamic_cast<OrientableSurface*>(pGeometrie) );
    break;

  case GEOMETRY_REFERENCE:
    if ( writeXLinks )
    {
      stdString = "#" + dynamic_cast<GeometryReference*>(pGeometrie)->getRefGeoId();
      STR = QuConvert::ToString( stdString );
      writer->WriteAttributeString( "xlink:href", STR );
    }
    else
    {
      _Geometrie * pGeo =  dynamic_cast<GeometryReference*>(pGeometrie)->getRefGeoObjekt();
      writeGeometrie( pGeo );
    }
    break;

  case SURFACE_REFERENCE:
    if ( writeXLinks )
    {
      stdString = "#" + dynamic_cast<SurfaceReference*>(pGeometrie)->getRefGeoId();
      STR = QuConvert::ToString( stdString );
      writer->WriteAttributeString( "xlink:href", STR );
    }
    else
    {
      _Geometrie * pGeo =  dynamic_cast<SurfaceReference*>(pGeometrie)->getRefGeoObjekt();
      writeGeometrie( pGeo );
    }
    break;

  case CURVE_REFERENCE:
    if ( writeXLinks )
    {
      stdString = "#" + dynamic_cast<CurveReference*>(pGeometrie)->getRefGeoId();
      STR = QuConvert::ToString( stdString );
      writer->WriteAttributeString( "xlink:href", STR );
    }
    else
    {
      _Geometrie * pGeo =  dynamic_cast<CurveReference*>(pGeometrie)->getRefGeoObjekt();
      writeGeometrie( pGeo );
    }
    break;

  case SOLID_REFERENCE:
    if ( writeXLinks )
    {
      stdString = "#" + dynamic_cast<SolidReference*>(pGeometrie)->getRefGeoId();
      STR = QuConvert::ToString( stdString );
      writer->WriteAttributeString( "xlink:href", STR );
    }
    else
    {
      _Geometrie * pGeo =  dynamic_cast<SolidReference*>(pGeometrie)->getRefGeoObjekt();
      writeGeometrie( pGeo );
    }
    break;

  case IMPLICIT_GEOMETRY_REFERENCE:
    if ( writeXLinks )
    {
     stdString = "#" + dynamic_cast<ImplicitGeometryReference*>(pGeometrie)->getRefGeoId();
      STR = QuConvert::ToString( stdString );
      writer->WriteAttributeString( "xlink:href", STR );
    }
    else
    {
      _Geometrie * pGeo =  dynamic_cast<SolidReference*>(pGeometrie)->getRefGeoObjekt();
      writeGeometrie( pGeo );
    }
    break;

  case DIRECT_POSITION:
    writeDirectPosition ( dynamic_cast<DirectPosition*>(pGeometrie) );
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
  _Surface  * pSurface;
  string      stdString;
  String    ^ gmlId;
  int         i;
  String    ^ REF_GEO_ID;

  pSurface = pSolid->getAussenkontur();
  if ( pSurface == NULL ) return;

  writer->WriteStartElement ( "gml:Solid" );

  stdString = pSolid->getSrsName();
  if ( stdString != "" )
    writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

  gmlId = pSolid->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
    gmlId = createGeometryGmlId();

  if ( gmlId != nullptr )
    writer->WriteAttributeString ("gml:id", gmlId );

  writer->WriteStartElement( "gml:exterior" );
  if ( pSurface->getGeometryType() == SURFACE_REFERENCE )
  {
    if ( writeXLinks )
    {
      stdString = "#" + dynamic_cast<SurfaceReference*>(pSurface)->getRefGeoId();
      REF_GEO_ID = QuConvert::ToString( stdString );
      writer->WriteAttributeString( "xlink:href", REF_GEO_ID );
    }
    else
    {
      _Geometrie * pGeo =  dynamic_cast<SurfaceReference*>(pSurface)->getRefGeoObjekt();
      writeGeometrie( pGeo );
    }
  }
  else
    write_Surface( pSurface );
  writer->WriteEndElement();

  for ( i = 0; i < pSolid->getInnenkonturAnz(); i++ )
  {
    pSurface = pSolid->getInnenkontur( i );
    writer->WriteStartElement( "gml:interior" );
    if ( pSurface->getGeometryType() == SURFACE_REFERENCE )
    {
      if ( writeXLinks )
      {
        stdString = "#" + dynamic_cast<SurfaceReference*>(pSurface)->getRefGeoId();
        REF_GEO_ID = QuConvert::ToString( stdString );
        writer->WriteAttributeString( "xlink:href", REF_GEO_ID );
      }
      else
      {
        _Geometrie * pGeo =  dynamic_cast<SurfaceReference*>(pSurface)->getRefGeoObjekt();
        writeGeometrie( pGeo );
      }
    }
    else
     write_Surface( pSurface );
    writer->WriteEndElement();
  }
  writer->WriteEndElement();
}


///////////////////////////////////////////////////////////////////////////////
//  Schreiben von gml:MultiSolid                                             //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeMultiSolid ( MultiSolid * pMultiSolid )
{
  _Solid   * pSolidMember;
	size_t     i, anz;
  string     stdString;
  String   ^ gmlId;
  String   ^ REF_GEO_ID;

  anz = pMultiSolid->getSolidMemberAnz();
  if ( anz == 0 ) return;

  if ( pMultiSolid->getIsCompositeSolid() )
    writer->WriteStartElement ( "gml:CompositeSurface" );
  else
    writer->WriteStartElement ( "gml:MultiSolid" );

  stdString = pMultiSolid->getSrsName();
  if ( stdString != "" )
    writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

  gmlId = pMultiSolid->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
    gmlId = createGeometryGmlId();

  if ( gmlId != nullptr )
    writer->WriteAttributeString ("gml:id", gmlId );

  for ( i = 0; i < anz; i++ )
  {
    writer->WriteStartElement ( "gml:solidMember" );

    pSolidMember = pMultiSolid->getSolid ( i );
    switch ( pSolidMember->getGeometryType() )
    {
    case SOLID:
      writeSolid( dynamic_cast<Solid*>(pSolidMember) );
      break;

    case  MULTI_SOLID:
      writeMultiSolid( dynamic_cast<MultiSolid*>(pSolidMember) );
      break;

    case SOLID_REFERENCE:
      if ( writeXLinks )
      {
        stdString = "#" + dynamic_cast<SolidReference*>(pSolidMember)->getRefGeoId();
        REF_GEO_ID = QuConvert::ToString( stdString );
        writer->WriteAttributeString( "xlink:href", REF_GEO_ID );
      }
      else
      {
        _Geometrie * pGeo =  dynamic_cast<SolidReference*>(pSolidMember)->getRefGeoObjekt();
        writeGeometrie( pGeo );
      }
      break;
    }

    writer->WriteEndElement();
  }
  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben von gml:MultiGeometry                                          //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeMultiGeometry ( MultiGeometry * pMultiGeometry )
{
  _Geometrie * pGeometryMember;
	size_t       i, anz;
  string       stdString;
  String     ^ gmlId;

  anz = pMultiGeometry->getGeometryMemberAnz();
  if ( anz == 0 ) return;

  writer->WriteStartElement ( "gml:MultiGeometry" );

  stdString = pMultiGeometry->getSrsName();
  if ( stdString != "" )
    writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

  gmlId = pMultiGeometry->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
    gmlId = createGeometryGmlId();

  if ( gmlId != nullptr )
    writer->WriteAttributeString ("gml:id", gmlId );

  for ( i = 0; i < anz; i++ )
  {
    writer->WriteStartElement ( "gml:geometryMember" );
    pGeometryMember = pMultiGeometry->getGeometry( i );
    writeGeometrie( pGeometryMember );
    writer->WriteEndElement();
  }
  
  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Schreiben von core:ImplicitGeometry                                      //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeImplicitGeometry ( ImplicitGeometry * pImplicitGemetry )
{
  string       stdString;
  String     ^ gmlId;
  String     ^ STR;
  Matrix4    * pMatrix;
  int          i, j;
  double       transformationsMatrix[12];
  _Geometrie * pGeometrie;
  GeoPoint   * pPoint;

  writer->WriteStartElement ( "core:ImplicitGeometry" );

  stdString = pImplicitGemetry->getSrsName();
  if ( stdString != "" )
    writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

  gmlId = pImplicitGemetry->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
    gmlId = createGeometryGmlId();

  if ( gmlId != nullptr )
    writer->WriteAttributeString ("gml:id", gmlId );

  if ( pImplicitGemetry->getMimeType() != "" )
  {
    STR = QuConvert::ToString( pImplicitGemetry->getMimeType() );
    writer->WriteStartElement( "core:mimeType" );
    writer->WriteString( STR );
    writer->WriteEndElement();
  }

  pMatrix = pImplicitGemetry->getTransformationMatrix();
  if ( pMatrix != NULL )
  {
    writer->WriteStartElement( "core:transformationMatrix" );
    pMatrix->getMatrix( transformationsMatrix );
    for ( j = 0; j < 3; j++ )
    {
      STR = "";
      for ( i = 0; i < 4; i++ )
        STR = String::Concat( STR, XmlConvert::ToString( transformationsMatrix[4*j+i] ), " " );
      writer->WriteString( STR );
    }

    STR = "";
    for ( i = 0; i < 4; i++ )
    {
      if ( i < 3 ) STR = String::Concat( STR, "0.0 " );
      else         STR = String::Concat( STR, "1.0 " );
    }
    writer->WriteString( STR );

    writer->WriteEndElement();
  }

  if ( pImplicitGemetry->getLibraryObject() != "" )
  {
    STR = QuConvert::ToString( pImplicitGemetry->getLibraryObject() );
    writer->WriteStartElement( "core:libraryObject" );
    writer->WriteString( STR );
    writer->WriteEndElement();
  }

  pGeometrie = pImplicitGemetry->getRelativeGMLGeometry();
  if ( pGeometrie != NULL )
  {
    writer->WriteStartElement ( "core:relativeGMLGeometry" );
    writeGeometrie( pGeometrie );
    writer->WriteEndElement();
  }

  pPoint = pImplicitGemetry->getReferencePoint();
  if ( pPoint != NULL )
  {
    writer->WriteStartElement( "core:referencePoint" );
    writePoint( pPoint );
    writer->WriteEndElement();
  }
  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben von gml:OrientableSurface                                      //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeOrientableSurface ( OrientableSurface * pOrientableSurface )
{
  string       stdString;
  String     ^ gmlId;
  bool         orientation;
  _Surface   * pBaseSurface;

  writer->WriteStartElement( "gml:OrientableSurface" );

  stdString = pOrientableSurface->getSrsName();
  if ( stdString != "" )
    writer->WriteAttributeString ("srsName", QuConvert::ToString ( stdString ) );

  gmlId = pOrientableSurface->getGML_ID();
  if ( gmlId == nullptr && gmlVersion == GML_3_2 )
    gmlId = createGeometryGmlId();

  if ( gmlId != nullptr )
    writer->WriteAttributeString ("gml:id", gmlId );

  orientation = pOrientableSurface->getOrientation();
  if ( orientation )
    writer->WriteAttributeString( "orientation", "+" );
  else
    writer->WriteAttributeString( "orientation", "-" );

  pBaseSurface = pOrientableSurface->getBaseSurface();
  if ( pBaseSurface != NULL )
  {
    writer->WriteStartElement( "gml:baseSurface" );
    write_Surface( pBaseSurface );
    writer->WriteEndElement();
  }
  writer->WriteEndElement();
}


///////////////////////////////////////////////////////////////////////////////
//  Erzeugung einer GML-Datei mit allen in pFeatures referierten Features		 //
//  outputFormat 0: Standard Feature-Collection                              //
//  outputFormat 1: Rausschreiben als gml:FeatureCollection                  //
//  outputFormat 2: Rausschreiben als wfs:FeatureCollection                  //
///////////////////////////////////////////////////////////////////////////////
int GMLWriter::write ( std::string fileNameP, int outputFormatP )
{
	String          ^ fileName;
	String          ^ STR;
  String          ^ NAMESPACE;
	System::String  ^ date;
	System::String  ^ comment;
	string			      srsName;
  int               srsDimension;
  string            kuerzel;
  string            namesp;
  string            gmlId;
	size_t	          j, anz;
  bool              success;
	double			      sw[3], no[3];
	char              buffer[16];
  GmlNamespaces   * pNamespaces;
  vector<string>    vKuerzel;
  vector<string>    vNamespaces;

  multimap<string,Feature*> mRootFeatures;
  set<string>               sKlassenNamen;
  Feature                 * pFeature;

  multimap<string,Feature*>::iterator iterFeatures;

  GENERIC_ATTRIBUT_NAME  = QuConvert::ToString ( pSchemaTypeNames->genericNameAttribute );
  GENERIC_ATTRIBUT_VALUE = QuConvert::ToString ( pSchemaTypeNames->genericValueAttribute );

  pNamespaces = pGmlSchema->getGmlNamespacesStandard();

	_strdate_s ( buffer, 16 );
	date = QuConvert::ToString ( string ( buffer ) );

	fileName = QuConvert::ToString ( fileNameP );
	anzObjWrite = 0;
  outputFormat = outputFormatP;

	System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();
	writer								            = gcnew XmlTextWriter( fileName, utf8);

	writer->Formatting = Formatting::Indented;
	writer->WriteStartDocument( true );

	comment = "Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: ";
	comment = String::Concat ( comment, date );

	writer->WriteComment( comment );

  if ( outputFormat == 0 )
  {
    STR = QuConvert::ToString( pSchemaTypeNames->rootName );
    writer->WriteStartElement( STR );
  }
  else
  if ( outputFormat == 1 )
    writer->WriteStartElement( "gml:FeatureCollection" );
  else
  if ( outputFormat == 2 )
    writer->WriteStartElement( "wfs:FeatureCollection" );

  anz = pNamespaces->getNamespaces ( vKuerzel, vNamespaces );
  success = false;
  for ( j = 0; j < anz; j++ )
  {
    kuerzel = vKuerzel[j];
    if ( kuerzel == "wfs" ) success = true;
    STR = String::Concat ( "xmlns:", QuConvert::ToString ( kuerzel ) );
    namesp = vNamespaces[j];
    NAMESPACE = QuConvert::ToString ( namesp );
    writer->WriteAttributeString ( STR, NAMESPACE );
  }
  if (!success )
  {
    STR = String::Concat ( "xmlns:", QuConvert::ToString ( "wfs" ) );
    NAMESPACE = QuConvert::ToString ( "http://www.opengis.net/wfs/2.0" );
    writer->WriteAttributeString ( STR, NAMESPACE );
  }

  if ( pGmlSchema->getGmlTyp() == INSPIRE_LU_PLANNED )
  {
    STR       = "xsi:schemaLocation";
    NAMESPACE = "http://inspire.ec.europa.eu/schemas/plu/4.0 http://inspire.ec.europa.eu/schemas/plu/4.0/PlannedLandUse.xsd http://www.opengis.net/wfs/2.0 http://schemas.opengis.net/wfs/2.0/wfs.xsd";
    writer->WriteAttributeString ( STR, NAMESPACE );
  }

  gmlId = pFeatures->getGmlId();
  if ( gmlId == "" )
    gmlId = Constant::createGmlId();
	writer->WriteAttributeString ( "gml:id", QuConvert::ToString ( gmlId ) );

	pFeatures->getBoundingBox ( sw, no, srsName, srsDimension ); 
	writeBoundedBy ( sw, no,srsName );

  pFeatures->getChildren ( mRootFeatures, sKlassenNamen );
  for ( iterFeatures = mRootFeatures.begin(); iterFeatures != mRootFeatures.end(); iterFeatures++ )
  {
    pFeature = iterFeatures->second;
    writeFeatureMember ( pFeature );
  }

	writer->WriteEndElement();

	
	writer->Flush();
	writer->Close();

	return anzObjWrite;
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugung einer GML-Datei mit den übergebenen Features              		 //
//  outputFormat 0: Standard Feature-Collection                              //
//  outputFormat 1: Rausschreiben als gml:FeatureCollection                  //
//  outputFormat 2: Rausschreiben als wfs:FeatureCollection                  //
///////////////////////////////////////////////////////////////////////////////
int GMLWriter::writeSingleFeatures ( std::string fileNameP, int outputFormatP, vector<Feature*> & vFeatures )
{
  String          ^ fileName;
  String          ^ STR;
  String          ^ NAMESPACE;
  System::String  ^ date;
  System::String  ^ comment;
  string			      srsName;
  int               srsDimension;
  string            kuerzel;
  string            namesp;
  string            gmlId;
	size_t            j, anz;
  bool              success;
  double			      sw[3], no[3];
  char              buffer[16];
  GmlNamespaces   * pNamespaces;
  vector<string>    vKuerzel;
  vector<string>    vNamespaces;

  GENERIC_ATTRIBUT_NAME  = QuConvert::ToString ( pSchemaTypeNames->genericNameAttribute );
  GENERIC_ATTRIBUT_VALUE = QuConvert::ToString ( pSchemaTypeNames->genericValueAttribute );

  pNamespaces = pGmlSchema->getGmlNamespacesStandard();
  outputFormat = outputFormatP;

  _strdate_s ( buffer, 16 );
  date = QuConvert::ToString ( string ( buffer ) );

  fileName = QuConvert::ToString ( fileNameP );
  anzObjWrite = 0;

  System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();
  writer								            = gcnew XmlTextWriter( 
    fileName, utf8);

  writer->Formatting = Formatting::Indented;
  writer->WriteStartDocument( true );

  comment = "Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: ";
  comment = String::Concat ( comment, date );

  writer->WriteComment( comment );

  if ( outputFormat == 0 )
  {
    STR = QuConvert::ToString( pSchemaTypeNames->rootName );
    writer->WriteStartElement( STR );
  }
  else
    if ( outputFormat == 1 )
      writer->WriteStartElement( "gml:FeatureCollection" );
    else
      if ( outputFormat == 2 )
        writer->WriteStartElement( "wfs:FeatureCollection" );

  anz = pNamespaces->getNamespaces ( vKuerzel, vNamespaces );
  success = false;
  for ( j = 0; j < anz; j++ )
  {
    kuerzel = vKuerzel[j];
    if ( kuerzel == "wfs" ) success = true;
    STR = String::Concat ( "xmlns:", QuConvert::ToString ( kuerzel ) );
    namesp = vNamespaces[j];
    NAMESPACE = QuConvert::ToString ( namesp );
    writer->WriteAttributeString ( STR, NAMESPACE );
  }
  if (!success )
  {
    STR = String::Concat ( "xmlns:", QuConvert::ToString ( "wfs" ) );
    NAMESPACE = QuConvert::ToString ( "http://www.opengis.net/wfs" );
    writer->WriteAttributeString ( STR, NAMESPACE );
  }

  gmlId = pFeatures->getGmlId();
  if ( gmlId == "" )
    gmlId = Constant::createGmlId();
  writer->WriteAttributeString ( "gml:id", QuConvert::ToString ( gmlId ) );

  pFeatures->getBoundingBox ( sw, no, srsName, srsDimension ); 
  writeBoundedBy ( sw, no,srsName );

  for ( unsigned int i = 0; i < vFeatures.size(); i++ )
  {
    writeFeatureMember ( vFeatures[i] );
  }

  if (writeXLinks && xlinkFeatures->count() > 0 )
  {
    set<string>::iterator iter;
    vector<string> vXlinkFeatures;


    while ( xlinkFeatures->count() > 0 )
    {
      vXlinkFeatures.clear();

      for ( iter = xlinkFeatures->getIds().begin(); iter != xlinkFeatures->getIds().end(); iter++ )
        vXlinkFeatures.push_back( *iter );

      for ( unsigned int i = 0; i < vXlinkFeatures.size(); i++ )
      {
        Feature * pFeature = pFeatures->getFeature( vXlinkFeatures[i] );
        writeFeatureMember ( pFeature );
      }
    }
  }

  writer->WriteEndElement();


  writer->Flush();
  writer->Close();

  return anzObjWrite;
}


///////////////////////////////////////////////////////////////////////////////
//  Schreibt ein Feature Member                                              //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeFeatureMember ( Feature * pFeature )
{
  string featureMemberName;

  if ( pFeature->getFeatureMemberName() != "" && outputFormat != 0 )
    featureMemberName = pFeature->getFeatureMemberName();
  else
    featureMemberName = pSchemaTypeNames->featureMemberName;

  writer->WriteStartElement( QuConvert::ToString ( featureMemberName) );
  writeFeature ( pFeature, NULL );
  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt ein Feature	  									                            	 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeFeature ( Feature * pFeature, GmlKlasse * pGmlReferenceTypeP )
{
	string					       name;
	string				      	 klassenName;
	string					       gmlId;
  string                 externalCodeListsPfad;
  string                 generStringWert;
	size_t				         i, j;
  int                    generIntWert;
  unsigned int           k;
	vector<GmlAttribut*>	 gmlAttribute;
  GmlKlasse            * pFeatureKlasse;
  GmlKlasse            * pGmlReferenceTypeLoc;
	GmlAttribut			     * pGmlAttribut;
	size_t				         gmlAttributAnz;
	size_t		          	 attributWertAnz;
	ATTRIBUT_TYP		    	 gmlAttributTyp;
	string				      	 gmlAttributName;
  string                 gmlAttributTypName;
	StringAttr           * stringAttributWert;
	IntAttr						   * intAttributWert;
	DoubleAttr					 * doubleAttributWert = nullptr;
	BoolAttr					   * boolAttributWert;
	bool				        	 multiple;
  bool                   nillable;
	FeatureRelation		   * pRelation;
	Feature			         * pRelationZiel;
	vector<StringAttr*>	   stringAttributWerte;
	vector<IntAttr*>	     intAttributWerte;
	vector<DoubleAttr*>    doubleAttributWerte;
  vector<DateAttr*>      dateAttributWerte;
	vector<FeatureRelation*> relationen;
	vector<Feature*>         planObjekte;
  vector<string>         classifierNamen;
  vector<string>         classifierValues;
  StringAttr           * pSimpleContent;
  vector<UOM*>           vUOMs;
  
  map<string,string>::iterator iterClassifier;

	String				   ^ GML_ATTRIBUT_NAME;
	String				   ^ ATTRIBUT_NAME;
	String				   ^ ATTRIBUT_WERT;
	String			     ^ GML_ID;
  String           ^ STR;

  pSimpleContent = pFeature->getSimpleContent();
  if ( pSimpleContent != NULL )
  {
    name = pFeature->getKlassenName ();
    writer->WriteStartElement ( QuConvert::ToString ( name ) );

    for ( iterClassifier = pSimpleContent->classifier.begin(); iterClassifier != pSimpleContent->classifier.end(); iterClassifier++ )
    {
      ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
      ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
      if ( ATTRIBUT_NAME->CompareTo( "gml:id") != 0 )
        writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
    }

    if ( pSimpleContent->value != "" )
      writer->WriteString ( QuConvert::ToString ( pSimpleContent->value ) );

    writer->WriteEndElement();
    return;
  }

	klassenName = pFeature->getKlassenName ();
  GmlElement * pGmlElement = pGmlSchema->getGmlElement( klassenName );
  if ( pGmlElement != NULL )
    pFeatureKlasse = pGmlSchema->getGmlKlasse ( pGmlElement->getGmlTypeName() );
  else
    pFeatureKlasse = pGmlReferenceTypeP;

	if ( pFeatureKlasse == NULL )
		return;

  anzObjWrite++;

	name = pFeature->getKlassenName ();
	writer->WriteStartElement ( QuConvert::ToString ( name ) );

  gmlId = pFeature->getGmlId();
  xlinkFeatures->removeId( gmlId );
  writtenFeatures->addId( gmlId );

  if ( gmlId != "" && pFeatureKlasse->getIsGmlFeature() )
    writer->WriteAttributeString ( gmlIdTag, QuConvert::ToString ( gmlId ) );


	gmlAttributAnz = pFeatureKlasse->getGmlAttribute ( gmlAttribute );
	for ( i = 0; i < gmlAttributAnz; i++ )
	{
		stringAttributWerte.clear();
		intAttributWerte.clear();
		doubleAttributWerte.clear();
    dateAttributWerte.clear();
		relationen.clear();

		pGmlAttribut    = gmlAttribute[i];
		gmlAttributTyp  = pGmlAttribut->getGmlAttributTyp();
		gmlAttributName = pGmlAttribut->getGmlAttributName();
    gmlAttributTypName = pGmlAttribut->getGmlAttributTypName();
    pGmlReferenceTypeLoc  = pGmlSchema->getGmlKlasse ( gmlAttributTypName );

		multiple		    = pGmlAttribut->getMultiple();
    nillable        = pGmlAttribut->getNillable();
		
		GML_ATTRIBUT_NAME = QuConvert::ToString ( gmlAttributName );

		switch ( gmlAttributTyp )
		{
    case _ADDRESS:
      attributWertAnz = pFeature->getFeatureRelationen ( "Address", relationen );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        pRelation = relationen[j];
        pRelationZiel = pFeatures->getFeature ( pRelation->featureId );
        if ( pRelationZiel == NULL ) break;

        writeAddress ( pRelationZiel );
      }
      break;

    case _MEASURE_LISTE:
    case _QUANTITY_EXTENT:
      attributWertAnz = pFeature->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte  );
      if ( attributWertAnz == 0  )
        break;

      if ( attributWertAnz == 1 &&  doubleAttributWerte[0]->isNil )
      {
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );
        writer->WriteAttributeString ( "xsi:nil", "true" );
        if ( doubleAttributWert->getNilReason() != "" )
          writer->WriteAttributeString( "nilReason", QuConvert::ToString( doubleAttributWert->getNilReason() ) );
        writer->WriteEndElement();
        break;
      }

      STR = "";
      for ( j = 0; j < attributWertAnz; j++ )
      {
        double d = doubleAttributWerte[j]->value;
        String ^ STR_LOC = XmlConvert::ToString ( d );
        STR = String::Concat( STR, STR_LOC, " " );
      }

      writer->WriteStartElement ( GML_ATTRIBUT_NAME );
      if ( doubleAttributWerte[0]->uom != NULL )
      {
        ATTRIBUT_NAME = QuConvert::ToString ( "uom" );
        ATTRIBUT_WERT = QuConvert::ToString ( doubleAttributWerte[0]->uom->getIdentifier() );
        writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
      }

      for ( iterClassifier = doubleAttributWerte[0]->classifier.begin(); iterClassifier != doubleAttributWerte[0]->classifier.end(); iterClassifier++ )
      {
        ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
        ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
        if ( iterClassifier->first != "uom" || doubleAttributWerte[0]->uom == NULL )
          writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
      }
      writer->WriteString ( STR );

      writer->WriteEndElement();      
      break;

		case _ENUMERATION:
    case _EXTERNAL_CODE_LIST:
		case _STRING:
			attributWertAnz = pFeature->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];

				writer->WriteStartElement ( GML_ATTRIBUT_NAME );

        if ( stringAttributWert->isNil )
        {
          writer->WriteAttributeString ( "xsi:nil", "true" );
          if ( stringAttributWert->getNilReason() != "" )
            writer->WriteAttributeString( "nilReason", QuConvert::ToString( stringAttributWert->getNilReason() ) );
        }
        else
        {
          for ( iterClassifier = stringAttributWert->classifier.begin(); iterClassifier != stringAttributWert->classifier.end(); iterClassifier++ )
          {
            ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
            ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
            writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
          }
          writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
        }
				writer->WriteEndElement();
			}
			break;

    case _EXTERNAL_CODE_LIST_GML3_3:
      attributWertAnz = pFeature->getStringAttributWerte ( gmlAttributName, stringAttributWerte );

      for ( j = 0; j < attributWertAnz; j++ )
      {
        stringAttributWert = stringAttributWerte[j];

        writer->WriteStartElement ( GML_ATTRIBUT_NAME );

        if ( stringAttributWert->isNil )
        {
          writer->WriteAttributeString ( "xsi:nil", "true" );
          if ( stringAttributWert->getNilReason() != "" )
            writer->WriteAttributeString( "nilReason", QuConvert::ToString( stringAttributWert->getNilReason() ) );
        }
        else
        {

          for ( iterClassifier = stringAttributWert->classifier.begin(); iterClassifier != stringAttributWert->classifier.end(); iterClassifier++ )
          {
            ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
            ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
            writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
          }
        }       
        writer->WriteEndElement();
      }
      break;
    

		case _ANY_URI:
			attributWertAnz = pFeature->getURLAttributWerte ( gmlAttributName, stringAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

			for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];

        writer->WriteStartElement ( GML_ATTRIBUT_NAME );

        if ( stringAttributWert->isNil )
        {
          writer->WriteAttributeString ( "xsi:nil", "true" );
          if ( stringAttributWert->getNilReason() != "" )
            writer->WriteAttributeString( "nilReason", QuConvert::ToString( stringAttributWert->getNilReason() ) );
        }
        else
        {
          for ( iterClassifier = stringAttributWert->classifier.begin(); iterClassifier != stringAttributWert->classifier.end(); iterClassifier++ )
          {
            ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
            ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
            writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
          }
          writer->WriteString ( QuConvert::ToString ( stringAttributWert->value ) );
        }
        writer->WriteEndElement();
			}
			break;

		case _DATE:
    case _DATE_TIME:
    case TIME_POSITION:
			attributWertAnz = pFeature->getDateAttributWerte ( gmlAttributName, dateAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      for ( j = 0; j < attributWertAnz; j++ )
			{
        DateAttr * pDateAttr = dateAttributWerte[j];
        if ( pDateAttr->isNil )
        {
          writer->WriteStartElement ( GML_ATTRIBUT_NAME );
          writer->WriteAttributeString ( "xsi:nil", "true" );
          if ( pDateAttr->getNilReason() != "" )
            writer->WriteAttributeString( "nilReason", QuConvert::ToString( pDateAttr->getNilReason() ) );
          writer->WriteEndElement();
        }
        else
        {
          TimePosition * pTimePosition = (TimePosition*)(dateAttributWerte[j]->value);
          if ( pTimePosition != NULL )
          {
            writer->WriteStartElement ( GML_ATTRIBUT_NAME );
            writeTimePosition( pTimePosition );
            writer->WriteEndElement();
          }
        }
			}
			break;

    case TIME_PERIOD:
      attributWertAnz = pFeature->getDateAttributWerte ( gmlAttributName, dateAttributWerte );
      if ( !multiple && attributWertAnz > 1 )
        attributWertAnz = 1;

      for ( j = 0; j < attributWertAnz; j++ )
      {
        DateAttr * pDateAttr = dateAttributWerte[j];

        if ( pDateAttr->isNil )
        {
          writer->WriteStartElement ( GML_ATTRIBUT_NAME );
          writer->WriteAttributeString ( "xsi:nil", "true" );
          if ( pDateAttr->getNilReason() != "" )
            writer->WriteAttributeString( "nilReason", QuConvert::ToString( pDateAttr->getNilReason() ) );
          writer->WriteEndElement();
        }
        else
        {
          TimePeriod * pTimePeriod = (TimePeriod*)(pDateAttr->value);

          writer->WriteStartElement ( GML_ATTRIBUT_NAME );
          writeTimePeriod( pTimePeriod );
          writer->WriteEndElement();
        }
      }
      break;

      case ABSTRACT_TIME_SERIES:
      {
        vector<AbstractTimeSeries*> vTimeSeries;

        attributWertAnz = pFeature->getAllTimeSeries( gmlAttributName, vTimeSeries );
        for ( j = 0; j < attributWertAnz; j++ )
        {
          AbstractTimeSeries * pTimeSeries = vTimeSeries[j];

          writer->WriteStartElement ( GML_ATTRIBUT_NAME );

          switch ( pTimeSeries->getTimeSeriesType() )
          {
          case AbstractTimeSeries::Regular:
            writeRegularTimeSeries ( (RegularTimeSeries*)pTimeSeries );
            break;

          case AbstractTimeSeries::RegularFile:
            writeRegularTimeSeriesFile ( (RegularTimeSeriesFile*)pTimeSeries );
            break;

          case AbstractTimeSeries::Irregular:
            writeIrregularTimeSeries ( (IrregularTimeSeries*)pTimeSeries );
            break;

          case AbstractTimeSeries::IrregularFile:
            writeIrregularTimeSeriesFile ( (IrregularTimeSeriesFile*)pTimeSeries );
            break;

          case AbstractTimeSeries::Monthly:
            writeMonthlyTimeSeries( (MonthlyTimeSeries*)pTimeSeries );
          }
          writer->WriteEndElement();
        }
        break;
      }

    case REGULAR_TIME_SERIES:
      {
        vector<AbstractTimeSeries*> vTimeSeries;

        attributWertAnz = pFeature->getAllTimeSeries( gmlAttributName, vTimeSeries );
        for ( j = 0; j < attributWertAnz; j++ )
        {
          RegularTimeSeries * pTimeSeries = (RegularTimeSeries*)vTimeSeries[j];
          writer->WriteStartElement ( GML_ATTRIBUT_NAME );
          writeRegularTimeSeries ( pTimeSeries );
          writer->WriteEndElement();
        }
        break;
      }

    case REGULAR_TIME_SERIES_FILE:
      {
        vector<AbstractTimeSeries*> vTimeSeries;

        attributWertAnz = pFeature->getAllTimeSeries( gmlAttributName, vTimeSeries );
        for ( j = 0; j < attributWertAnz; j++ )
        {
          RegularTimeSeriesFile * pTimeSeries = (RegularTimeSeriesFile*)vTimeSeries[j];
          writer->WriteStartElement ( GML_ATTRIBUT_NAME );
          writeRegularTimeSeriesFile ( pTimeSeries );
          writer->WriteEndElement();
        }
        break;
      }

    case IRREGULAR_TIME_SERIES:
      {
        vector<AbstractTimeSeries*> vTimeSeries;

        attributWertAnz = pFeature->getAllTimeSeries( gmlAttributName, vTimeSeries );
        for ( j = 0; j < attributWertAnz; j++ )
        {
          IrregularTimeSeries * pTimeSeries = (IrregularTimeSeries*)vTimeSeries[j];
          writer->WriteStartElement ( GML_ATTRIBUT_NAME );
          writeIrregularTimeSeries ( pTimeSeries );
          writer->WriteEndElement();
        }
        break;
      }

    case IRREGULAR_TIME_SERIES_FILE:
      {
        vector<AbstractTimeSeries*> vTimeSeries;

        attributWertAnz = pFeature->getAllTimeSeries( gmlAttributName, vTimeSeries );
        for ( j = 0; j < attributWertAnz; j++ )
        {
          IrregularTimeSeriesFile * pTimeSeries = (IrregularTimeSeriesFile*)vTimeSeries[j];
          writer->WriteStartElement ( GML_ATTRIBUT_NAME );
          writeIrregularTimeSeriesFile ( pTimeSeries );
          writer->WriteEndElement();
        }
        break;
      }

		case _ANGLE:
		case _LENGTH:
		case _AREA:
		case _VOLUME:
    case _MEASURE:
			attributWertAnz = pFeature->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      for ( j = 0; j < attributWertAnz; j++ )
			{
        doubleAttributWert = doubleAttributWerte[j];

        if ( doubleAttributWert->isNil )
        {
          writer->WriteStartElement ( GML_ATTRIBUT_NAME );
          writer->WriteAttributeString ( "xsi:nil", "true" );
          if ( doubleAttributWert->getNilReason() != "" )
            writer->WriteAttributeString( "nilReason", QuConvert::ToString( doubleAttributWert->getNilReason() ) );
          writer->WriteEndElement();
        }
        else
        {
          writer->WriteStartElement ( GML_ATTRIBUT_NAME );
          if ( doubleAttributWert->uom != NULL )
          {
            ATTRIBUT_NAME = QuConvert::ToString ( "uom" );
            ATTRIBUT_WERT = QuConvert::ToString ( doubleAttributWert->uom->getIdentifier() );
            writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
          }

          for ( iterClassifier = doubleAttributWert->classifier.begin(); iterClassifier != doubleAttributWert->classifier.end(); iterClassifier++ )
          {
            ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
            ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
            if ( iterClassifier->first != "uom" || doubleAttributWert->uom == NULL )
              writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
          }
          writer->WriteString ( XmlConvert::ToString ( doubleAttributWert->value ) );

          writer->WriteEndElement();
        }
			}
			break;

    case _GENER_STRING:
      attributWertAnz = pFeature->getFeatureRelationen (  "_GenericStringAttribute", relationen );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        pRelation = relationen[j];
        pRelationZiel = pFeatures->getFeature ( pRelation->featureId );
        if ( pRelationZiel == NULL ) break;		

        writer->WriteStartElement ( QuConvert::ToString ( pSchemaTypeNames->genericStringAttribute ) );
        pRelationZiel->getStringAttributWert ( pSchemaTypeNames->genericNameAttribute, generStringWert );
        STR = QuConvert::ToString ( generStringWert );
        writer->WriteAttributeString ( GENERIC_ATTRIBUT_NAME, STR );

        pRelationZiel->getStringAttributWert ( pSchemaTypeNames->genericValueAttribute, generStringWert );
        STR = QuConvert::ToString ( generStringWert );
        writer->WriteElementString ( GENERIC_ATTRIBUT_VALUE, STR );
        writer->WriteEndElement();
      }
      break;

    case _GENER_DATE:
      attributWertAnz = pFeature->getFeatureRelationen (  "_GenericDateAttribute", relationen );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        pRelation = relationen[j];
        pRelationZiel = pFeatures->getFeature ( pRelation->featureId );
        if ( pRelationZiel == NULL ) break;		

        writer->WriteStartElement ( QuConvert::ToString ( pSchemaTypeNames->genericDateAttribute ) );
        pRelationZiel->getStringAttributWert ( pSchemaTypeNames->genericNameAttribute, generStringWert );
        STR = QuConvert::ToString ( generStringWert );
        writer->WriteAttributeString ( GENERIC_ATTRIBUT_NAME, STR );

        DateAttr * pDateAttr = pRelationZiel->getDateAttributWert( pSchemaTypeNames->genericValueAttribute );
        if ( pDateAttr !=NULL )
        {
          TimePosition * pTimePosition = (TimePosition*)(pDateAttr->value);
          writer->WriteStartElement( GENERIC_ATTRIBUT_VALUE );
          writeTimePosition( pTimePosition );
          writer->WriteEndElement();
        }
        writer->WriteEndElement();
        
      }
      break;

    case _GENER_URL:
      attributWertAnz = pFeature->getFeatureRelationen (  "_GenericURLAttribute", relationen );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        pRelation = relationen[j];
        pRelationZiel = pFeatures->getFeature ( pRelation->featureId );
        if ( pRelationZiel == NULL ) break;		

        writer->WriteStartElement ( QuConvert::ToString ( pSchemaTypeNames->genericUrlAttribute ) );
        pRelationZiel->getStringAttributWert ( pSchemaTypeNames->genericNameAttribute, generStringWert );
        STR = QuConvert::ToString ( generStringWert );
        writer->WriteAttributeString ( GENERIC_ATTRIBUT_NAME, STR );
        pRelationZiel->getStringAttributWert ( pSchemaTypeNames->genericValueAttribute, generStringWert );
        STR = QuConvert::ToString ( generStringWert );
        writer->WriteElementString ( GENERIC_ATTRIBUT_VALUE, STR );
        writer->WriteEndElement();
      }
      break;

    case _GENER_INTEGER:
      attributWertAnz = pFeature->getFeatureRelationen (  "_GenericIntegerAttribute", relationen );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        pRelation = relationen[j];
        pRelationZiel = pFeatures->getFeature ( pRelation->featureId );
        if ( pRelationZiel == NULL ) break;		

        writer->WriteStartElement ( QuConvert::ToString ( pSchemaTypeNames->genericIntAttribute ) );
        pRelationZiel->getStringAttributWert ( pSchemaTypeNames->genericNameAttribute, generStringWert );
        STR = QuConvert::ToString ( generStringWert );
        writer->WriteAttributeString ( GENERIC_ATTRIBUT_NAME, STR );
        pRelationZiel->getIntegerAttributWert ( pSchemaTypeNames->genericValueAttribute, generIntWert );
        STR = XmlConvert::ToString ( generIntWert );
        writer->WriteElementString ( GENERIC_ATTRIBUT_VALUE, STR );
        writer->WriteEndElement();
      }
      break;

    case _GENER_DOUBLE:
      attributWertAnz = pFeature->getFeatureRelationen (  "_GenericDoubleAttribute", relationen );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        pRelation = relationen[j];
        pRelationZiel = pFeatures->getFeature ( pRelation->featureId );
        if ( pRelationZiel == NULL ) break;		

        writer->WriteStartElement ( QuConvert::ToString ( pSchemaTypeNames->genericDoubleAttribute ) );
        pRelationZiel->getStringAttributWert ( pSchemaTypeNames->genericNameAttribute, generStringWert );
        STR = QuConvert::ToString ( generStringWert );
        writer->WriteAttributeString ( GENERIC_ATTRIBUT_NAME, STR );
        DoubleAttr * pDoubleAttr = pRelationZiel->getDoubleAttributWert ( pSchemaTypeNames->genericValueAttribute );
        if ( pDoubleAttr != NULL )
        {
          STR = XmlConvert::ToString ( pDoubleAttr->value );
          writer->WriteElementString ( GENERIC_ATTRIBUT_VALUE, STR );
          writer->WriteEndElement();
        }
      }
      break;

    case _GENER_MEASURE:
      attributWertAnz = pFeature->getFeatureRelationen (  "_GenericMeasureAttribute", relationen );
      for ( j = 0; j < attributWertAnz; j++ )
      {
        DoubleAttr * pDoubleAttr;

        classifierNamen.clear();
        classifierValues.clear();

        pRelation = relationen[j];
        pRelationZiel = pFeatures->getFeature ( pRelation->featureId );
        if ( pRelationZiel == NULL ) break;		

        writer->WriteStartElement ( QuConvert::ToString ( pSchemaTypeNames->genericMeasureAttribute ) );
        pRelationZiel->getStringAttributWert ( pSchemaTypeNames->genericNameAttribute, generStringWert );
        STR = QuConvert::ToString ( generStringWert );
        writer->WriteAttributeString ( GENERIC_ATTRIBUT_NAME, STR );
        pDoubleAttr = pRelationZiel->getDoubleAttributWert ( pSchemaTypeNames->genericValueAttribute );
        STR = XmlConvert::ToString ( pDoubleAttr->value );
        writer->WriteStartElement( GENERIC_ATTRIBUT_VALUE );
        ATTRIBUT_NAME = "uom";
        if ( pDoubleAttr->uom != NULL )
          ATTRIBUT_WERT = QuConvert::ToString ( pDoubleAttr->uom->getIdentifier() );
        else
          ATTRIBUT_WERT = "";
        writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );

        writer->WriteString ( STR );
        writer->WriteEndElement();
        writer->WriteEndElement();
      }
      break;

		case _DOUBLE:
			attributWertAnz = pFeature->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      for ( j = 0; j < attributWertAnz; j++ )
			{
				doubleAttributWert = doubleAttributWerte[j];

        writer->WriteStartElement ( GML_ATTRIBUT_NAME );

        if (doubleAttributWert->isNil )
        {
          writer->WriteAttributeString ( "xsi:nil", "true" );
          if ( doubleAttributWert->getNilReason() != "" )
            writer->WriteAttributeString( "nilReason", QuConvert::ToString( doubleAttributWert->getNilReason() ) );
        }
        else
        {
          for ( iterClassifier = doubleAttributWert->classifier.begin(); iterClassifier != doubleAttributWert->classifier.end(); iterClassifier++ )
          {
            ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
            ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
            writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
          }
          writer->WriteString ( XmlConvert::ToString ( doubleAttributWert->value ) );
        }
        writer->WriteEndElement();
			}
			break;

		case _INTEGER:
			attributWertAnz = pFeature->getIntegerAttributWerte ( gmlAttributName, intAttributWerte );
			if ( !multiple && attributWertAnz > 1 )
				attributWertAnz = 1;

      for ( j = 0; j < attributWertAnz; j++ )
			{
				intAttributWert = intAttributWerte[j];
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );

        if ( intAttributWert->isNil )
        {
          writer->WriteAttributeString ( "xsi:nil", "true" );
          if ( intAttributWert->getNilReason() != "" )
            writer->WriteAttributeString( "nilReason", QuConvert::ToString( intAttributWert->getNilReason() ) );
        }
        else
        {
          for ( iterClassifier = intAttributWert->classifier.begin(); iterClassifier != intAttributWert->classifier.end(); iterClassifier++ )
          {
            ATTRIBUT_NAME = QuConvert::ToString ( iterClassifier->first );
            ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
            writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
          }
          writer->WriteString ( XmlConvert::ToString ( intAttributWert->value ) );
        }
        writer->WriteEndElement();
			}
			break;

		case _BOOLEAN:
			boolAttributWert = pFeature->getBoolAttributWert ( gmlAttributName );
			if ( boolAttributWert != NULL )
			{
        writer->WriteStartElement ( GML_ATTRIBUT_NAME );

        if ( boolAttributWert->isNil )
        {
          writer->WriteAttributeString ( "xsi:nil", "true" );
          if ( boolAttributWert->getNilReason() != "" )
            writer->WriteAttributeString( "nilReason", QuConvert::ToString( boolAttributWert->getNilReason() ) );
        }
        else
        {
          for ( iterClassifier = boolAttributWert->classifier.begin(); iterClassifier != boolAttributWert->classifier.end(); iterClassifier++ )
          {
            ATTRIBUT_NAME = QuConvert::ToString (iterClassifier->first );
            ATTRIBUT_WERT = QuConvert::ToString ( iterClassifier->second );
            writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
          }
          writer->WriteString ( XmlConvert::ToString ( boolAttributWert->value ) );
        }

        writer->WriteEndElement();
			}    
			break;

    case _REFERENZ:
      attributWertAnz = pFeature->getFeatureRelationen ( gmlAttributName, relationen );

      for ( j = 0; j < attributWertAnz; j++ )
      {
        pRelation = relationen[j];

        if ( pRelation->nil )
        {
          writer->WriteStartElement ( GML_ATTRIBUT_NAME );
          writer->WriteAttributeString ( "xsi:nil", "true" );
          if (pRelation->nilReason != "" )
            writer->WriteAttributeString ( "nilReason", QuConvert::ToString( pRelation->nilReason ) );
          writer->WriteEndElement();
        }
        else
        {
          pRelationZiel = pFeatures->getFeature ( pRelation->featureId );
          if ( pRelationZiel == NULL )
          {
            writer->WriteStartElement( GML_ATTRIBUT_NAME );
            GML_ID = QuConvert::ToString( "#" + pRelation->featureId );
            writer->WriteAttributeString ( "xlink:href", GML_ID );
            if ( !xlinkFeatures->existId( gmlId )  && !writtenFeatures->existId( gmlId ) )
              xlinkFeatures->addId( pRelation->featureId );
            writer->WriteEndElement();
          }
          else
          {
            writer->WriteStartElement( GML_ATTRIBUT_NAME );

            for ( k = 0; k < pRelation->classifierName.size(); k++ )
            {
              ATTRIBUT_NAME = QuConvert::ToString ( pRelation->classifierName[k] );
              ATTRIBUT_WERT = QuConvert::ToString ( pRelation->classifierValue[k] );
              writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
            }
            if ( pRelationZiel->getParent() != pFeature )
            {
              gmlId = pRelationZiel->getGmlId();
              GML_ID = String::Concat ( "#", QuConvert::ToString ( gmlId ) );
              writer->WriteAttributeString ( "xlink:href", GML_ID );
              if ( !xlinkFeatures->existId( gmlId ) && !writtenFeatures->existId( gmlId ) )
                xlinkFeatures->addId( gmlId );
            }
            else
              writeFeature ( pRelationZiel, NULL );
            writer->WriteEndElement();
          }
        }
      }
      break;

		case _DATA_TYPE:
			attributWertAnz = pFeature->getFeatureRelationen ( gmlAttributName, relationen );

      for ( j = 0; j < attributWertAnz; j++ )
			{
				pRelation = relationen[j];
        if ( pRelation->nil )
        {
          writer->WriteStartElement ( GML_ATTRIBUT_NAME );
          writer->WriteAttributeString ( "xsi:nil", "true" );
          if ( pRelation->nilReason != "" )
            writer->WriteAttributeString ( "nilReason", QuConvert::ToString( pRelation->nilReason ) );
          writer->WriteEndElement();
        }
        else
        {
				  pRelationZiel = pFeatures->getFeature ( pRelation->featureId );

          if (  ( pFeatures->getGmlTyp() == CITYGML_0_4_0 ||  pFeatures->getGmlTyp() == CITYGML_1_0 || pFeatures->getGmlTyp() == CITYGML_2_0  ) &&
                ( gmlAttributName == "citygml:externalReference" || gmlAttributName == "core:externalReference" ) )
                writeCityGMLExternalReference ( pRelationZiel );
          else
          {
  		      writer->WriteStartElement( GML_ATTRIBUT_NAME);

            for ( k = 0; k < pRelation->classifierName.size(); k++ )
            {
              ATTRIBUT_NAME = QuConvert::ToString ( pRelation->classifierName[k] );
              ATTRIBUT_WERT = QuConvert::ToString ( pRelation->classifierValue[k] );
              writer->WriteAttributeString ( ATTRIBUT_NAME, ATTRIBUT_WERT );
            }

            if ( pRelationZiel == NULL || pRelationZiel->getParent() != pFeature )
            {
              gmlId = pRelation->featureId;
              GML_ID = String::Concat ( "#", QuConvert::ToString ( gmlId ) );
              writer->WriteAttributeString ( "xlink:href", GML_ID );
              if ( !xlinkFeatures->existId( gmlId ) && !writtenFeatures->existId( gmlId )) 
                xlinkFeatures->addId( gmlId );
            }
            else
              writeFeature ( pRelationZiel, NULL );

            writer->WriteEndElement();
          }

       }
			}
			break;
			
		case _GEOMETRIE:
    case _DIRECT_POSITION:
				writeFeatureGeometrie ( pFeature, gmlAttributName );
			break;

		case _POS_XY:
			writeSymbolPosition ( pFeature, gmlAttributName );
      break;

		case _SONST:
			break;
		}
	}

	writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML:                                                             //
//  Schreiben der CityGML Adresse                                            //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeAddress ( Feature * pFeatureP )
{
  string  stdString;
  string  street;
  string  number;
  bool    success, successA, successB;

  if ( pFeatures->getGmlTyp() == CITYGML_0_4_0 )
  {
    writer->WriteStartElement ( "citygml:address" );
    writer->WriteStartElement ( "citygml:Address" );
    writer->WriteStartElement ( "citygml:xalAddress" );
  }
  else
  if ( pFeatures->getGmlTyp() == CITYGML_1_0 )
  {
    writer->WriteStartElement ( "bldg:address" );
    writer->WriteStartElement ( "core:Address" );
    writer->WriteStartElement ( "core:xalAddress" );
  }
  else
  if ( pFeatures->getGmlTyp() == CITYGML_2_0 || 
       pFeatures->getGmlTyp() == CITYGML_3_0  )
  {
    writer->WriteStartElement ( "bldg:address" );
    writer->WriteStartElement ( "core:Address" );
    writer->WriteStartElement ( "core:xalAddress" );
  }
  else
    return;

  writer->WriteStartElement ( "xAL:AddressDetails" );

  writer->WriteStartElement ( "xAL:Country" );

  success = pFeatureP->getStringAttributWert ( "xAL:CountryName", stdString );
  if ( success )
    writer->WriteElementString ( "xAL:CountryName", QuConvert::ToString ( stdString ) );

  writer->WriteStartElement ( "xAL:Locality" );
  writer->WriteAttributeString ( "Type", "Town" );

  success = pFeatureP->getStringAttributWert ( "xAL:LocalityName", stdString );
  if ( success )
    writer->WriteElementString (  "xAL:LocalityName", QuConvert::ToString ( stdString ) );

  success = pFeatureP->getStringAttributWert ( "xAL:PostBoxNumber", stdString );
  if ( success )
  {
    writer->WriteStartElement ( "xAL:PostBox" );
    writer->WriteElementString (  "xAL:PostBoxNumber", QuConvert::ToString ( stdString ) );
    writer->WriteEndElement();
  }

  successA = pFeatureP->getStringAttributWert ( "xAL:ThoroughfareNumber", number );
  successB = pFeatureP->getStringAttributWert ( "xAL:ThoroughfareName", street );

  if ( successA || successB )
  {
    writer->WriteStartElement ( "xAL:Thoroughfare" );
    writer->WriteAttributeString ( "Type", "Street" );

    if ( successA )
      writer->WriteElementString ( "xAL:ThoroughfareNumber", QuConvert::ToString ( number ) );

    if ( successB )
      writer->WriteElementString ( "xAL:ThoroughfareName", QuConvert::ToString ( street ) );

    writer->WriteEndElement();
  }

  success = pFeatureP->getStringAttributWert ( "xAL:PostalCodeNumber", number );
  if ( success )
  {
    writer->WriteStartElement ( "xAL:PostalCode" );
    writer->WriteElementString (  "xAL:PostalCodeNumber", QuConvert::ToString ( number ) );
    writer->WriteEndElement();
  }

  writer->WriteEndElement();
  writer->WriteEndElement();
  writer->WriteEndElement();
  writer->WriteEndElement();
  writer->WriteEndElement();

  writer->WriteEndElement();
}

//////////////////////////////////////////////////////////////////////////////
//  Schreiben eines Zeitpunktes (Klasse TimePosition)                       //
//////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeTimePosition ( TimePosition * pTimePosition )
{
  string stdString= pTimePosition->toXMLString();

  if ( pTimePosition->getQualifier() != TimePosition::UNDEFINED )
  {
    string qualifierString = pTimePosition->getQualifierAsString();
    writer->WriteAttributeString ( "indeterminatePosition",  QuConvert::ToString ( qualifierString ) );
  }

  if ( pTimePosition->getEra() != "" )
    writer->WriteAttributeString ( "calendarEraName",  QuConvert::ToString ( pTimePosition->getEra() ) );

  writer->WriteString ( QuConvert::ToString ( stdString ) );
}


///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines Zeitraums (Klasse TimePeriod)                            //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeTimePeriod ( TimePeriod * pTimePeriod )
{
  writer->WriteStartElement( "gml:TimePeriod" );

  writer->WriteStartElement( "gml:beginPosition" );
  writeTimePosition ( &pTimePeriod->getStartPosition() );
  writer->WriteEndElement();

  writer->WriteStartElement( "gml:endPosition" );
  writeTimePosition ( &pTimePeriod->getEndPosition() );
  writer->WriteEndElement();

  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML Energy ADE                                                   //
//  Schreiben einer Zeitreihe mit äquidistanten Zeitpunkten                  //
//  (Klasse RegularTimeSeries)                                               //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeRegularTimeSeries ( RegularTimeSeries * pRegularTimeSeries )
{
  writer->WriteStartElement( "energy:RegularTimeSeries" );

  writer->WriteStartElement( "energy:variableProperties" );
  writer->WriteStartElement( "energy:TimeValuesProperties" );

  writer->WriteElementString( "energy:acquisitionMethod", QuConvert::ToString( pRegularTimeSeries->getAquisitionMethod() ) );
  writer->WriteElementString( "energy:interpolationType", QuConvert::ToString( pRegularTimeSeries->getInterpolationTypeAsString() ) );
  if ( pRegularTimeSeries->getQualityDescription() != "" )
    writer->WriteElementString( "energy:qualityDescription", QuConvert::ToString( pRegularTimeSeries->getQualityDescription() ) );
  if ( pRegularTimeSeries->getSource() != "" )
    writer->WriteElementString( "energy:source", QuConvert::ToString( pRegularTimeSeries->getSource() ) );
  if ( pRegularTimeSeries->getThematicDescription() != "" )
    writer->WriteElementString( "energy:thematicDescription", QuConvert::ToString( pRegularTimeSeries->getThematicDescription() ) );
	if ( pRegularTimeSeries->getProperty() != "" )
		writer->WriteElementString( "energy:property", QuConvert::ToString( pRegularTimeSeries->getProperty() ) );
	if ( pRegularTimeSeries->getPropertyQualifier() != "" )
		writer->WriteElementString( "energy:propertyQualifier", QuConvert::ToString( pRegularTimeSeries->getPropertyQualifier() ) );

  writer->WriteEndElement();
  writer->WriteEndElement();

  writer->WriteStartElement( "energy:temporalExtent" );
  writeTimePeriod(  pRegularTimeSeries->getTemporalExtent() );
  writer->WriteEndElement();

  writer->WriteStartElement( "energy:timeInterval" );
  writer->WriteAttributeString( "unit", QuConvert::ToString( pRegularTimeSeries->getTimeIntervalLength()->getTimeUnitString() ) );
  writer->WriteString( XmlConvert::ToString ( pRegularTimeSeries->getTimeIntervalLength()->getValue() ) );
  writer->WriteEndElement();

  writer->WriteStartElement( "energy:values" );
  if ( pRegularTimeSeries->getUOM() != NULL )
    writer->WriteAttributeString( "uom", QuConvert::ToString( pRegularTimeSeries->getUOM()->getIdentifier() ) );
  else
    writer->WriteAttributeString( "uom", "unknown" );


  String ^ STR = "";
  for ( unsigned int i = 0; i < pRegularTimeSeries->getValueCount(); i++ )
  {
    STR = String::Concat ( STR, XmlConvert::ToString( pRegularTimeSeries->getValue( i ) ), " " );
    if ( i > 0 && i % 20 == 0 )
      STR = String::Concat ( STR, "\n" );
  }

  writer->WriteString( STR );
  writer->WriteEndElement();

  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML Energy ADE                                                   //
//  Schreiben einer Zeitreihe mit äquidistanten Zeitpunkten, wobei die Werte //
//  auf einer CSV-Datei gespeichert sind (Klasse RegularTimeSeriesFile)      //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeRegularTimeSeriesFile ( RegularTimeSeriesFile * pRegularTimeSeriesFile )
{
  string uomIdentifer;

  writer->WriteStartElement( "energy:RegularTimeSeriesFile" );

  writer->WriteStartElement( "energy:variableProperties" );
  writer->WriteStartElement( "energy:TimeValuesProperties" );

  writer->WriteElementString( "energy:acquisitionMethod", QuConvert::ToString( pRegularTimeSeriesFile->getAquisitionMethod() ) );
  writer->WriteElementString( "energy:interpolationType", QuConvert::ToString(  pRegularTimeSeriesFile->getInterpolationTypeAsString() ) );
  if ( pRegularTimeSeriesFile->getQualityDescription() != "" )
    writer->WriteElementString( "energy:qualityDescription", QuConvert::ToString( pRegularTimeSeriesFile->getQualityDescription() ) );
  if ( pRegularTimeSeriesFile->getSource() != "" )
    writer->WriteElementString( "energy:source", QuConvert::ToString( pRegularTimeSeriesFile->getSource() ) );
  if ( pRegularTimeSeriesFile->getThematicDescription() != "" )
    writer->WriteElementString( "energy:thematicDescription", QuConvert::ToString( pRegularTimeSeriesFile->getThematicDescription() ) );
	if ( pRegularTimeSeriesFile->getProperty() != "" )
		writer->WriteElementString( "energy:property", QuConvert::ToString( pRegularTimeSeriesFile->getProperty() ) );
	if ( pRegularTimeSeriesFile->getPropertyQualifier() != "" )
		writer->WriteElementString( "energy:propertyQualifier", QuConvert::ToString( pRegularTimeSeriesFile->getPropertyQualifier() ) );

  writer->WriteEndElement();
  writer->WriteEndElement();

  writer->WriteStartElement( "energy:uom");
  if ( pRegularTimeSeriesFile->getUOM() != NULL )
    uomIdentifer = pRegularTimeSeriesFile->getUOM()->getIdentifier();
  else
    uomIdentifer = "";
  writer->WriteAttributeString( "uom", QuConvert::ToString( uomIdentifer ) );
  writer->WriteEndElement();

  writer->WriteElementString( "energy:file", QuConvert::ToString( pRegularTimeSeriesFile->getFile() ) );


  writer->WriteStartElement( "energy:temporalExtent" );
  writeTimePeriod(  pRegularTimeSeriesFile->getTemporalExtent() );
  writer->WriteEndElement();

  writer->WriteStartElement( "energy:timeInterval" );
  writer->WriteAttributeString( "unit", QuConvert::ToString( pRegularTimeSeriesFile->getTimeIntervalLength()->getTimeUnitString() ) );
  writer->WriteString( XmlConvert::ToString ( pRegularTimeSeriesFile->getTimeIntervalLength()->getValue() ) );
  writer->WriteEndElement();

  writer->WriteElementString ( "energy:numberOfHeaderLines", XmlConvert::ToString( pRegularTimeSeriesFile->getNumberOfHeaderLines() ) );
  writer->WriteElementString ( "energy:fieldSeparator", QuConvert::ToString( pRegularTimeSeriesFile->getFieldSeparator() ) );
  writer->WriteElementString ( "energy:recordSeparator", QuConvert::ToString( pRegularTimeSeriesFile->getRecordSeparator() ) );
  writer->WriteElementString ( "energy:decimalSymbol", QuConvert::ToString( pRegularTimeSeriesFile->getDecimalSymbol() ) );
  writer->WriteElementString ( "energy:valueColumnNumber", XmlConvert::ToString( pRegularTimeSeriesFile->getValueColumnNumber() ) );

  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML Energy ADE                                                   //
//  Schreiben einer Zeitreihe mit beliebigen    Zeitpunkten                  //
//  (Klasse IrregularTimeSeries)                                             //
//  Noch nicht implementiert                                                 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeIrregularTimeSeries ( IrregularTimeSeries * pIrregularTimeSeries )
{

}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML Energy ADE                                                   //
//  Schreiben einer Zeitreihe mit beliebigen Zeitpunkten, wobei die Werte    //
//  auf einer CSV-Datei gespeichert sind (Klasse IrregularTimeSeriesFile)    //
//  Noch nicht implementiert                                                 //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeIrregularTimeSeriesFile ( IrregularTimeSeriesFile * pIrregularTimeSeriesFile )
{

}

///////////////////////////////////////////////////////////////////////////////
//  Nur CityGML Energy ADE                                                   //
//  Schreiben einer Zeitreihe mit 12 Monatswerten                            //
///////////////////////////////////////////////////////////////////////////////
void GMLWriter::writeMonthlyTimeSeries ( MonthlyTimeSeries * pMonthlyTimeSeries )
{
  string         uomIdentifer;
  vector<double> vValues;

  writer->WriteStartElement( "energy:MonthlyTimeSeries" );

  writer->WriteStartElement( "energy:variableProperties" );
  writer->WriteStartElement( "energy:TimeValuesProperties" );

  writer->WriteElementString( "energy:acquisitionMethod", QuConvert::ToString( pMonthlyTimeSeries->getAquisitionMethod() ) );
  writer->WriteElementString( "energy:interpolationType", QuConvert::ToString(  pMonthlyTimeSeries->getInterpolationTypeAsString() ) );
  if ( pMonthlyTimeSeries->getQualityDescription() != "" )
    writer->WriteElementString( "energy:qualityDescription", QuConvert::ToString( pMonthlyTimeSeries->getQualityDescription() ) );
  if ( pMonthlyTimeSeries->getSource() != "" )
    writer->WriteElementString( "energy:source", QuConvert::ToString( pMonthlyTimeSeries->getSource() ) );
  if ( pMonthlyTimeSeries->getThematicDescription() != "" )
    writer->WriteElementString( "energy:thematicDescription", QuConvert::ToString( pMonthlyTimeSeries->getThematicDescription() ) );
	if ( pMonthlyTimeSeries->getProperty() != "" )
		writer->WriteElementString( "energy:property", QuConvert::ToString( pMonthlyTimeSeries->getProperty() ) );
	if ( pMonthlyTimeSeries->getPropertyQualifier() != "" )
		writer->WriteElementString( "energy:propertyQualifier", QuConvert::ToString( pMonthlyTimeSeries->getPropertyQualifier() ) );

  writer->WriteEndElement();
  writer->WriteEndElement();

  writer->WriteStartElement( "energy:uom");
  if ( pMonthlyTimeSeries->getUOM() != NULL )
    uomIdentifer = pMonthlyTimeSeries->getUOM()->getIdentifier();
  else
    uomIdentifer = "";
  writer->WriteAttributeString( "uom", QuConvert::ToString( uomIdentifer ) );
  writer->WriteEndElement();

  writer->WriteElementString( "energy:year", XmlConvert::ToString(pMonthlyTimeSeries->getYear() ) );

  writer->WriteStartElement( "energy:values" );

  vValues = pMonthlyTimeSeries->getValues();
  String ^ STR = "";
  for ( unsigned int i = 0; i < vValues.size(); i++ )
    STR = String::Concat ( STR, XmlConvert::ToString( vValues[i] ), " " );

  writer->WriteString( STR );
  writer->WriteEndElement();

  writer->WriteEndElement();
}


///////////////////////////////////////////////////////////////////////////////
//  _set                                                                     //
//  Datenstruktur zur Verwaltung eines Set von strings                       //
///////////////////////////////////////////////////////////////////////////////
void _set::addId ( string id )
{
	ids.insert ( id );
}

void _set::removeId( string id )
{
  ids.erase( id );
}

bool _set::existId ( string id )
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













