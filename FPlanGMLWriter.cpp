#include "StdAfx.h"

#include <string>
#include <map>
#include <sstream>

#include "Convert.h"
#include "BPlan.h"
#include "BPlanObjekt.h"
#include "Geometrie.h"
#include "GmlSchema.h"

#include ".\FPlanGMLWriter.h"

///////////////////////////////////////////////////////////////////////////////
//  FPlanGMLWriter															 //
//  Erzeugen einer FPlanGML-Datei											 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor												 //
///////////////////////////////////////////////////////////////////////////////
FPlanGMLWriter::FPlanGMLWriter ( BLeitplanViewer * pBLeitplanViewerP, bool gml3P )
:GMLWriter ( pBLeitplanViewerP, gml3P )
{
	xmlns		  = "fplan:";

	if ( gml3 )	
	{
		gmlSchema = S"FPlan-Operationen.xsd";
		gmlIdTag  = "gml:id";
	}
	else
	{
		gmlSchema = S"FPlan-Operationen_2.xsd";
		gmlIdTag  = "fid";
	}
}
FPlanGMLWriter::~ FPlanGMLWriter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Aufruf writer															 //
///////////////////////////////////////////////////////////////////////////////
int FPlanGMLWriter::write ( std::string fileNameP )
{
	System::String* fileName;
	System::String * schemaLocation;
	System::String * schemaFolder;
	int			     i, j, planAnz, objektAnz;
	OBJEKT_TYP       objTyp;
	double			 swX, swY, noX, noY;
	
	vector<BLeitPlan*>       vFPlaene;
	vector<PlanObjekt*>      vFPlanObjekte;

	fileName = QuConvert::ToString ( fileNameP );
	anzObjWrite = 0;

	System::Text::UTF8Encoding * utf8	= new UTF8Encoding();
	writer								= new XmlTextWriter( fileName, utf8);

	writer->Formatting = Formatting::Indented;
	writer->WriteStartDocument( true );
	writer->WriteComment("Generated by: FZK-Shapefile-GML Converter");

    writer->WriteStartElement("FPlanAuszug ");

	writer->WriteAttributeString("xmlns",	    "http://www.xplanung.de/fplangml");
	writer->WriteAttributeString("xmlns:gml",   "http://www.opengis.net/gml");
	writer->WriteAttributeString("xmlns:xlink",	"http://www.w3.org/1999/xlink");
	writer->WriteAttributeString("xmlns:xsi",	"http://www.w3.org/2001/XMLSchema-instance");
	writer->WriteAttributeString("xmlns:fplan",	"http://www.xplanung.de/fplangml");
	writer->WriteAttributeString("xmlns:wfs",	"http://www.opengis.net/wfs");

	schemaFolder = QuConvert::ToString ( pBLeitplanViewer->getSchemaFolder() );
	schemaLocation = System::String::Concat ( S"http://www.xplanung.de/fplangml  ", schemaFolder, gmlSchema );
	writer->WriteAttributeString ( "xsi:schemaLocation", schemaLocation );

	pBLeitplanViewer->getBoundingBox ( swX, swY, noX, noY ); 
	writeBoundedBy ( swX, swY, noX, noY );

	planAnz = pBLeitplanViewer->getPlaene ( vFPlaene );
	for ( i = 0;  i < planAnz; i++ )
	{
		BLeitPlan * pFPlan = vFPlaene[i];
		vFPlanObjekte.clear();
		objektAnz = writeFPlan ( pFPlan, vFPlanObjekte );

		for ( j = 0; j < objektAnz; j++ )
		{
			PlanObjekt * pFPlanObj = vFPlanObjekte[j];
			writeFPlanObjekt ( pFPlanObj );
		}
	}

	writer->WriteEndElement();

	writer->Flush();
	writer->Close();

	return anzObjWrite;
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben FPlan														 //
///////////////////////////////////////////////////////////////////////////////
int FPlanGMLWriter::writeFPlan ( BLeitPlan * pFPlanP, std::vector<PlanObjekt*> &planObjekteP )
{
	vector<GmlAttribut*> gmlAttribute;
	string				 gmlId;
	string               name;
	GmlAttribut		   * pGmlAttribut;
	int					 gmlAttributAnz;
	int					 attributWertAnz;
	int				     i, j;
	bool				 success;
	ATTRIBUT_TYP		 gmlAttributTyp;
	string				 gmlAttributName;
	string				 stringAttributWert;
	int					 intAttributWert;
	double				 doubleAttributWert;
	bool				 boolAttributWert;
	PlanObjRelation *    pRelation;
	PlanObjekt      *    pRelationZiel;
	vector<string>		 stringAttributWerte;
	vector<int>			 intAttributWerte;
	vector<double>		 doubleAttributWerte;
	vector<PlanObjRelation*> relationen;

	String			   * GML_ATTRIBUT_NAME;
	String			   * STR;
	String			   * GML_ID;

	GmlObjekte * pGmlObjekte = pBLeitplanViewer->getGmlObjekte();
	GmlKlasse * pFPlanKlasse = pGmlObjekte->getGmlKlasse ( "FPlan" );
	if ( pFPlanKlasse == NULL ) return 0;

	writer->WriteStartElement("gml:featureMember");  // <gml:featureMember>
	anzObjWrite++;

	QuConvert::systemStr2stdStr ( name, xmlns );
	name += pFPlanP->getKlassenName ();
	writer->WriteStartElement( QuConvert::ToString ( name ) );		

	gmlId  = pFPlanP->getGmlId ();
	GML_ID = QuConvert::ToString ( gmlId );
	writer->WriteAttributeString ( gmlIdTag, GML_ID );

	gmlAttributAnz = pFPlanKlasse->getGmlAttribute ( gmlAttribute );
	for ( i = 0; i < gmlAttributAnz; i++ )
	{
		stringAttributWerte.clear();
		intAttributWerte.clear();
		doubleAttributWerte.clear();
		relationen.clear();

		pGmlAttribut   = gmlAttribute[i];
		gmlAttributTyp = pGmlAttribut->getGmlAttributTyp();
		gmlAttributName = pGmlAttribut->getGmlAttributName();
		
		STR = QuConvert::ToString ( gmlAttributName );
		GML_ATTRIBUT_NAME = String::Concat ( xmlns, STR );

		switch ( gmlAttributTyp )
		{
		case _ENUMERATION:
		case _ANY_URI:
		case _STRING:
		case _DATE:
			attributWertAnz = pFPlanP->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];
				writer->WriteElementString ( GML_ATTRIBUT_NAME, QuConvert::ToString ( stringAttributWert ) );
			}
			break;

		case _DOUBLE:
			attributWertAnz = pFPlanP->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				doubleAttributWert = doubleAttributWerte[j];
				writer->WriteElementString ( GML_ATTRIBUT_NAME, XmlConvert::ToString ( doubleAttributWert ) );
			}
			break;

		case _INTEGER:
			attributWertAnz = pFPlanP->getIntegerAttributWerte ( gmlAttributName, intAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				intAttributWert = intAttributWerte[j];
				writer->WriteElementString ( GML_ATTRIBUT_NAME, XmlConvert::ToString ( intAttributWert ) );
			}
			break;

		case _BOOLEAN:
			success = pFPlanP->getBoolAttributWert ( gmlAttributName, boolAttributWert );
			if ( success )
			{
				writer->WriteElementString ( GML_ATTRIBUT_NAME, XmlConvert::ToString ( boolAttributWert ) );
			}
			break;

		case _REFERENZ:
			attributWertAnz = pFPlanP->getObjRelationen ( gmlAttributName, relationen );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				pRelation = relationen[j];
				pRelationZiel = pBLeitplanViewer->getPlanObjekt ( pRelation->bPlanObjektId );
				if ( pRelationZiel == NULL ) break;		

				writer->WriteStartElement( GML_ATTRIBUT_NAME );		
				if ( gml3										||
					pRelation->rollenName == "gehoertZuFPlan" 
					)
				{
					GML_ID = String::Concat ( "#", QuConvert::ToString (  pRelation->bPlanObjektId ) );
					writer->WriteAttributeString ( "xlink:href", GML_ID );
					planObjekteP.push_back ( pRelationZiel );
				}
				else writeFPlanObjekt ( pRelationZiel );
				writer->WriteEndElement();
			}

			break;
			
		case _GEOMETRIE:
		case _SONST:
			break;
		}
	}

	writer->WriteEndElement();
	writer->WriteEndElement();

	return planObjekteP.size();
}

void FPlanGMLWriter::writeFPlanObjekt ( PlanObjekt * pFPlanObjP )
{
	string					 name;
	string					 gmlId;
	int						 i, j,anz;
	double					 swX, swY, noX, noY;
	vector<GmlAttribut*>	 gmlAttribute;
	GmlAttribut			   * pGmlAttribut;
	int						 gmlAttributAnz;
	int						 attributWertAnz;
	bool					 success;
	ATTRIBUT_TYP			 gmlAttributTyp;
	string					 gmlAttributName;
	string					 stringAttributWert;
	int						 intAttributWert;
	double					 doubleAttributWert;
	bool					 boolAttributWert;
	PlanObjRelation		   * pRelation;
	PlanObjekt			   * pRelationZiel;
	vector<string>			 stringAttributWerte;
	vector<int>				 intAttributWerte;
	vector<double>		     doubleAttributWerte;
	vector<PlanObjRelation*> relationen;

	String				   * GML_ATTRIBUT_NAME;
	String			       * STR;
	String			       * GML_ID;

	if ( gml3 ) writer->WriteStartElement("gml:featureMember");  // <gml:featureMember>
	anzObjWrite++;

	GmlObjekte * pGmlObjekte = pBLeitplanViewer->getGmlObjekte();
	GmlKlasse * pFPlanKlasse = pGmlObjekte->getGmlKlasse ( pFPlanObjP->getKlassenName () );
	if ( pFPlanKlasse == NULL )
		return;

	QuConvert::systemStr2stdStr ( name, xmlns );
	name += pFPlanObjP->getKlassenName ();
	gmlId = pFPlanObjP->getGmlId();

	writer->WriteStartElement ( QuConvert::ToString ( name ) );
	writer->WriteAttributeString ( gmlIdTag, QuConvert::ToString ( gmlId ) );

	gmlAttributAnz = pFPlanKlasse->getGmlAttribute ( gmlAttribute );
	for ( i = 0; i < gmlAttributAnz; i++ )
	{
		stringAttributWerte.clear();
		intAttributWerte.clear();
		doubleAttributWerte.clear();
		relationen.clear();

		pGmlAttribut   = gmlAttribute[i];
		gmlAttributTyp = pGmlAttribut->getGmlAttributTyp();
		gmlAttributName = pGmlAttribut->getGmlAttributName();
		
		STR = QuConvert::ToString ( gmlAttributName );
		GML_ATTRIBUT_NAME = String::Concat ( xmlns, STR );

		switch ( gmlAttributTyp )
		{
		case _ENUMERATION:
		case _ANY_URI:
		case _STRING:
		case _DATE:
			attributWertAnz = pFPlanObjP->getStringAttributWerte ( gmlAttributName, stringAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				stringAttributWert = stringAttributWerte[j];
				writer->WriteElementString ( GML_ATTRIBUT_NAME, QuConvert::ToString ( stringAttributWert ) );
			}
			break;

		case _ANGLE:
			attributWertAnz = pFPlanObjP->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				writer->WriteStartElement ( GML_ATTRIBUT_NAME );
				writer->WriteAttributeString ( "uom", "rad" );
				doubleAttributWert = doubleAttributWerte[j];
				writer->WriteString ( XmlConvert::ToString ( doubleAttributWert ) );
				writer->WriteEndElement();
			}
			break;


		case _DOUBLE:
			attributWertAnz = pFPlanObjP->getDoubleAttributWerte ( gmlAttributName, doubleAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				doubleAttributWert = doubleAttributWerte[j];
				writer->WriteElementString ( GML_ATTRIBUT_NAME, XmlConvert::ToString ( doubleAttributWert ) );
			}
			break;

		case _INTEGER:
			attributWertAnz = pFPlanObjP->getIntegerAttributWerte ( gmlAttributName, intAttributWerte );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				intAttributWert = intAttributWerte[j];
				writer->WriteElementString ( GML_ATTRIBUT_NAME, XmlConvert::ToString ( intAttributWert ) );
			}
			break;

		case _BOOLEAN:
			success = pFPlanObjP->getBoolAttributWert ( gmlAttributName, boolAttributWert );
			if ( success )
			{
				writer->WriteElementString ( GML_ATTRIBUT_NAME, XmlConvert::ToString ( boolAttributWert ) );
			}
			break;

		case _REFERENZ:
			attributWertAnz = pFPlanObjP->getObjRelationen ( gmlAttributName, relationen );
			for ( j = 0; j < attributWertAnz; j++ )
			{
				pRelation = relationen[j];
				pRelationZiel = pBLeitplanViewer->getPlanObjekt ( pRelation->bPlanObjektId );
				if ( pRelationZiel == NULL ) break;		

				writer->WriteStartElement( GML_ATTRIBUT_NAME );	
				if ( gml3 )
				{
					GML_ID = String::Concat ( "#", QuConvert::ToString (  pRelation->bPlanObjektId ) );
					writer->WriteAttributeString ( "xlink:href", GML_ID );
				}
				else
				{
					//  Bestimmte Relationen einbetten
					//writeFPlanObjekt ( pRelationZiel );
				}
				writer->WriteEndElement();
			}

			break;
			
		case _GEOMETRIE:
			if ( gmlAttributName == "boundedBy" )
			{
				pFPlanObjP->getBoundingBox ( swX, swY, noX, noY );
				writeBoundedBy ( swX, swY, noX, noY );
			}

			if ( gmlAttributName == "position" )
				writeGeometrie ( pFPlanObjP );

			break;

		case _SONST:
			break;
		}
	}

	writer->WriteEndElement();
	if ( gml3 ) writer->WriteEndElement();

}







