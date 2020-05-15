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
class Polygon;
class CurveSegment;
class Ring;
class Curve;

#include "BPlanGMLWriter.h"

///////////////////////////////////////////////////////////////////////////////
//  FPlanGMLWriter															 //
//  Erzeugen einer FPlanGML-Datei											 //
///////////////////////////////////////////////////////////////////////////////
__gc class FPlanGMLWriter: public GMLWriter
{
public:
	FPlanGMLWriter ( BLeitplanViewer * pBLeitplanViewerP, bool gml3P );
	virtual ~ FPlanGMLWriter();

public:
	virtual int write ( std::string fileNameP );


protected:
	System::String * gmlSchema;
	System::String * gmlIdTag;

private:
	int writeFPlan ( BLeitPlan * pFPlanP, std::vector<PlanObjekt*> &planObjekteP );
	void writeFPlanObjekt ( PlanObjekt * pFPlanObjP );
};
