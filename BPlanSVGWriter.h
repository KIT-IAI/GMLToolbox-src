#pragma once
using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Text;
using namespace System::Collections;
#using <mscorlib.dll>

class BLeitPlan;
class PlanObjekt;

///////////////////////////////////////////////////////////////////////////////
//  BPlanSVGWriter															 //
//  Visualisierung eines BPlan über SVG										 //
///////////////////////////////////////////////////////////////////////////////
__gc class BPlanSVGWriter
{
public:
	BPlanSVGWriter ( BLeitplanViewer * pBLeitplanViewerP );
	virtual ~ BPlanSVGWriter();

public:
	int write ( std::string fileNameP, BLeitPlan * pBPlanP, PlanBereich * pBPlanEbeneP );


private:
	BLeitplanViewer * pBLeitplanViewer;

	System::String * svgStylesheetDatei;
	System::String * svgDefinesDatei;

	double swX;
	double swY;
	double noX;
	double noY;

	System::String * gmlIdTag;
	System::Xml::XmlTextWriter * writer;

private:
	void writePlanObjekt ( PlanObjekt * pBPlanObjektP );
	void writePlanFlaechenObjekt ( PlanFlaechenObjekt * pBPlanObjektP );
	void writePlanLinienObjekt ( PlanLinienObjekt * pBPlanObjektP );
	void writePlanPunktObjekt ( PlanPunktObjekt * pBPlanObjektP );
	void writePlanPTO ( PlanPunktObjekt * pBPlanObjektP );

	void writeFonts();

	void writePolygon ( std::string idP, Polygon * pPolygonP, 
			String * class1, String * style1, bool clip1,
			String * class2, String * style2, bool clip2,
			String * class3, String * style3, bool clip3,
			String * class4, String * style4, bool clip4 );

	void writePolygon ( std::string idP, Polygon * pPolygonP, 
			String * class1, String * style1, bool clip1 );

	void writePolygon ( std::string idP, Polygon * pPolygonP, 
			String * class1, String * style1, bool clip1,
			String * textClass, String * annotChar, double charSize );

	void writePolygonWithTextPath ( std::string idP, Polygon * pPolygonP, 
			String * class1, String * style1, bool clip1,
			String * textStyle, String * annotChar, double charSize );

	void writePolygon ( std::string idP, Polygon * pPolygonP, 
			String * class1, String * style1, bool clip1,
			String * class2, String * style2, bool clip2 );

	void writePolygon ( std::string idP, Polygon * pPolygonP, 
			String * class1, String * style1, bool clip1,
			String * class2, String * style2, bool clip2,
			String * class3, String * style3, bool clip3 );

	void drawText ( String * textStringP, double fontSizeP, double circleSizeP,
					double xPosP, double yPosP );

	void drawSymbol ( String * symbolIdP, double xPosP, double yPosP, double angle );


	void writeCurve ( std::string idP, Curve * pCurveP, String * class1 );
	void writeCurve ( std::string idP, Curve * pCurveP, String * class1, String * class2 );

	void writeRaeumlicherGeltungsbereich (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeKennzeichnungsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeImmissionsschutzFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeLuftreinhalteFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeFreiFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeWegerechteBelasteteFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeNebenanlagenFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeGemeinschaftsanlagenFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeFlaecheAllgemein (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeStrassenkoerper (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeAusgleichsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );

	void writeBaugebietsFlaechenTeil (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeFlaecheMitBesonderemNutzungszweck (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeUeberbaubareGrundstuecksflaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );

	void writeFoerderungsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeTechnikBestimmteFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writePersGruppenBestimmteFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );

	void writeAbgrabungsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeBodenschaetzeFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeAufschuettungsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );

	void writeGemeinbedarfsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeSpielSportanlagenFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );

	void writeWaldFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeLandwirtschaftsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeGruenFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );

	void writeAnpflanzungBindungErhaltungsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeSchutzgebietFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeSchutzPflegeEntwicklungsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );

	void writeErhaltungsbereich (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeDenkmalschutzBereich (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeDenkmalschutzEnsemble (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );

	void writeVerEntsorgungsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );

	void writeLuftverkehrFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeBahnFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeVerkehrsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeVerkehrsFlaecheBesondererZweckbestimmung (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );

	void writeWasserFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeWasserwirtschaftsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeWasserrechtsFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );
	void writeVorbHochwSchutzFlaeche (  PlanFlaechenObjekt * pBPlanObjektP, Polygon * pPolyP );

	void writeBaulinie ( PlanLinienObjekt * pObjP, Curve * pCurveP ); 
	void writeBaugrenze ( PlanLinienObjekt * pObjP, Curve * pCurveP ); 
	void writeFirstRichtung ( PlanLinienObjekt * pObjP, Curve * pCurveP ); 
	void writeAnpflanzungBindungErhaltungsLinie ( PlanLinienObjekt * pObjP, Curve * pCurveP ); 
	void writeStrassenkoerperLinie ( PlanLinienObjekt * pObjP, Curve * pCurveP ); 
	void writeLinieAllgemein ( PlanLinienObjekt * pObjP, Curve * pCurveP ); 
	void writeVerEntsorgungsleitung ( PlanLinienObjekt * pObjP, Curve * pCurveP ); 
	void writeStrassenbegrenzungslinie ( PlanLinienObjekt * pObjP, Curve * pCurveP ); 
	void writeBahnLinie ( PlanLinienObjekt * pObjP, Curve * pCurveP ); 
	void writeEinfahrtsbereich ( PlanLinienObjekt * pObjP, Curve * pCurveP ); 
	void writeBereichOhneEinAusfahrt ( PlanLinienObjekt * pObjP, Curve * pCurveP ); 

	void writeAnpflanzungBindungErhaltungsMassnahme ( PlanPunktObjekt * pBPlanObjektP, GeoPoint * pPoint );
	void writeAusgleichsMassnahme (  PlanPunktObjekt * pBPlanObjektP, GeoPoint * pPoint );
	void writeHoehenObjekt (  PlanPunktObjekt * pBPlanObjektP, GeoPoint * pPoint );
	void writePunktAllgemein (  PlanPunktObjekt * pBPlanObjektP, GeoPoint * pPoint );
	void writeDenkmalschutzEinzelanlage (  PlanPunktObjekt * pBPlanObjektP, GeoPoint * pPoint );
	void writeEinfahrt (  PlanPunktObjekt * pBPlanObjektP, GeoPoint * pPoint );


};
