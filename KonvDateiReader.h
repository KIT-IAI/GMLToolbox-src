#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Windows::Forms;
using namespace System::Text;
using namespace System::Collections;

class ShapeFileNamenKonvertierung;
class PlanAttribute;
class KlassenName;
class Enumeration;
class XPlanGML;

///////////////////////////////////////////////////////////////////////////////
//  __gc class KonvDateiReader												                       //
//  Einlesen einer XML-Datei mit Konvertierungsregeln Shapefile -> XPlanGML  //
///////////////////////////////////////////////////////////////////////////////
public ref class KonvDateiReader
{
public:
	KonvDateiReader( XPlanGML * pXPlanGML );
	virtual ~ KonvDateiReader();

protected:
	XmlTextReader       ^ validReader;		// .NET XML-Reader

	System::String ^ schemaLocation;			// Pfadname des zug. XPlanGML-Schemas
	System::String ^ pActElement;					// Aktuell gelesenes XML-Tag
	System::String ^ pActAttrName;				// Aktuell gelesenes XML-Attribut

	XPlanGML          			    * m_pXPlanGML;					// XPlanGML-Modul
	ShapeFileNamenKonvertierung * m_pKonv;							// Konvertierungsregel-Objekt
	PlanAttribute               * m_pPlanAttribute;			// Attribute des gesamten Plans

public:
	ShapeFileNamenKonvertierung * read( std::string fileName );

private:
	void readTextAbschnitt();
	void readBereich();
	RasterplanBasis     * readRasterplanBasis ();
	RasterplanAenderung * readRasterplanAenderung();
  ExterneReferenz     * readExterneReferenz();
	void readClassTransform();
	void readEnumerationTransform();
  std::string addDefaultNamespaceKuerzel ( std::string name );

};

///////////////////////////////////////////////////////////////////////////////
//  __gc class KonvDateiWriter																							 //
//  Erzeugung einer XML-Repräsentation der Konvertierungsregeln							 //
///////////////////////////////////////////////////////////////////////////////
public ref class KonvDateiWriter
{
public:
	KonvDateiWriter( XPlanGML * pXPlanGML );
	virtual ~ KonvDateiWriter();

public:
	void write ( std::string fileNameP );

private:
	void writeKlassenName( KlassenName * pKlassenNameP );
	void writeEnumeration( Enumeration * pEnumerationP );
	void writeRasterplanBasis( RasterplanBasis * pRasterplanBasis );
	void writeExterneReferenz ( ExterneReferenz * pExterneReferenz );


protected:
		XPlanGML			              * m_pXPlanGML;		// XPlanGML-Modul
		XmlTextWriter               ^ writer;					// .NET XML-Writer
		ShapeFileNamenKonvertierung * m_pKonv;				// Konvertierungsregel-Objekt
};





