#pragma once
#include "includes.h"
#include "Geometrie.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Text;
using namespace System::Collections;

class Feature;
class PolygonGeo;
class CurveSegment;
class LineSegment;
class ArcSegment;
class _Surface;
class Surface;
class Ring;
class Curve;
class _Curve;
class _set;
class MultiSolid;
class MultiSurface;
class MultiCurve;
class MultiPoint;
class _Geometrie;
class DirectPosition;
class Solid;
class MultiGeometry;
class ImplicitGeometry;
class OrientableSurface;
class TriangulatedSurface;
class Feature;
class Features;
class GmlSchema;
class GmlKlasse;
class SchemaTypeNames;
class TimePosition;
class TimePeriod;
class RegularTimeSeries;
class RegularTimeSeriesFile;
class IrregularTimeSeries;
class IrregularTimeSeriesFile;
class RegularTimeSeriesSet;
class MonthlyTimeSeries;

///////////////////////////////////////////////////////////////////////////////
//  GMLWriter                                                                //
//  Export von GML-Dateien              				                  					 //
///////////////////////////////////////////////////////////////////////////////
public ref class GMLWriter
{
public:
	GMLWriter ( Features * pFeatureP );
	virtual ~ GMLWriter();

public:
  virtual int write ( std::string fileNameP, int outputFormatP );
  virtual int writeSingleFeatures ( std::string fileNameP, int outputFormatP, std::vector<Feature*> & vFeatures );

  void setWriteXLinks ( bool b )  {  writeXLinks = b;  }
  bool getWriteXLinks()  {  return writeXLinks;  }
	
protected:
  System::String  ^ gmlIdTag;         // XML-Tag der gml:id
  _set            * xlinkFeatures;    // Menge von Features, die über xlink mit einem zu schreibenden Feature verbunden sind
  _set            * writtenFeatures;  // Menge der bereits exportierten Features
	Features        * pFeatures;        // Aggregation der Features
  GmlSchema       * pGmlSchema;       // Zugehöriges GML-Schema
  SchemaTypeNames * pSchemaTypeNames; // XML-Tag Namen des Schemas 
	int			          anzObjWrite;      // Anzahl der geschriebenen Features
  int               anzGeoWritten;    // Anzahl der geschriebenen Geometrieobjekte
	int               precision;        // Anzahl der Nachkommastellen beim schreiben von Koordinaten
  int               outputFormat;     // Ausgage als XPlanAuszug, oder FeatureCollection
	GML_SCHEMA_TYPE   gmlSchemaType;    // Typ des GML-Applikationsschemas
  GML_VERSION       gmlVersion;       // GML-Version (3.1.1 oder 3.2.1)
  int               srsDimension;     // Raumdimension der zu exportierenden Geometrie
  String          ^ GENERIC_ATTRIBUT_VALUE; // Tag-Name des Wertes eines Generischen Attributs
  String          ^ GENERIC_ATTRIBUT_NAME;  // Tag-Name eines Generischen Attributs
  bool              writeXLinks;      // Steuert, ob xlinks exportiert werden

	System::Xml::XmlTextWriter ^ writer; //  .NET XML Writer

protected:
  void writeFeatureMember ( Feature * pFeature );
  void writeFeature ( Feature * Feature, GmlKlasse * pGmlReferenceTypeP );

  void writeCityGMLExternalReference ( Feature * pExternalReference );

	void writeBoundedBy( double *sw, double * no, std::string srsName );
	void writeFeatureGeometrie ( Feature * pBPlanObjP, std::string gmlAttributNameP );

	void writeMultiSurface  ( MultiSurface * pMultiSurface );
	void writeMultiCurve    ( String ^ gmlTag, MultiCurve * pMultiCurve, bool isRing );
	void writeMultiPoint    ( MultiPoint * pMultiPoint );
  void writeMultiSolid    ( MultiSolid * pMultiSolid );
  void writeMultiGeometry ( MultiGeometry * pMultiGeometry );

  void writeGeometrie ( _Geometrie * pGeometrie );
	void write_Surface ( _Surface * pSurface );
	void write_Curve ( _Curve * pCurve );

	void writeSurface ( Surface * pSurface );
  void writeTriangulatedSurface ( TriangulatedSurface * pSurface );
	void writePolygon ( String ^ gmlTag, PolygonGeo * pPolyP, bool isPatch );
  void writeSolid ( Solid * pSolid );
  void writeImplicitGeometry ( ImplicitGeometry * pImplicitGemetry );
  void writeOrientableSurface ( OrientableSurface * pOrientableSurface );

	void writeCurve ( Curve * pCurve  );
	void writeLineSegment ( LineSegment * pLineSegment );
	void writeArcSegment ( ArcSegment * pArcSegment );

	void writePoint ( GeoPoint * pPoint );
  void writeDirectPosition ( DirectPosition * pDirectPosition );
	void writeSymbolPosition ( Feature * pBPlanObjP, std::string gmlAttributNameP );

  void writeTimePosition ( TimePosition * pTimePosition );
  void writeTimePeriod ( TimePeriod * pTimePeriod );

  void writeRegularTimeSeries ( RegularTimeSeries * pRegularTimeSeries );
  void writeRegularTimeSeriesFile ( RegularTimeSeriesFile * pRegularTimeSeriesFile );
  void writeIrregularTimeSeries ( IrregularTimeSeries * pIrregularTimeSeries );
  void writeMonthlyTimeSeries ( MonthlyTimeSeries * pMonthlyTimeSeries );
  void writeIrregularTimeSeriesFile ( IrregularTimeSeriesFile * pIrregularTimeSeriesFile );

  void writeAddress ( Feature * pFeatureP );

 String ^ createGeometryGmlId();
};

///////////////////////////////////////////////////////////////////////////////
//  _set                                                                     //
//  Datenstruktur zur Verwaltung eines Set von strings                       //
///////////////////////////////////////////////////////////////////////////////
class _set
{
	public:
		_set() {   }
		virtual ~_set() {  }
	private:
		std::set<std::string> ids;   // set von strings

	public:		
		void addId ( std::string id );
    void removeId ( std::string id );
    std::set<std::string> & getIds() {  return ids;  }
		bool existId ( std::string id );
		size_t count()  {  return ids.size();  }
		void clear();
};



