
#pragma once

#include "includes.h"

///////////////////////////////////////////////////////////////////////////////
//  GmlNamespaces                                                            //
//  Namespaces von ein oder mehreren eingelesenen XML-Schemadateien          //
///////////////////////////////////////////////////////////////////////////////
class GmlNamespaces
{
public:
  GmlNamespaces();
  GmlNamespaces (int schemaType );
  virtual ~ GmlNamespaces();

private:
  std::string                            defaultNamespace;  // Default-Namespace einer XML-Datei
  std::map<std::string,std::string>      namespaces;        // map; Namespace--> Namespace-Kürzel 
  std::map<std::string,std::string>      namespaceKuerzel;  // map: Namespace-Kürzel --> Namespace
  std::multimap<std::string,std::string> schemaDateien;     // multiMap: Target-Namespace XML-Schema Datei --> Dateipfad

public:
   // Target Namespaces der fest implementierten GML-Applikationsschemata
 static std::string                xmlSchemaStandardKuerzel; 
  static std::string                gml_3_0_targetNamespace;
  static std::string                gml_3_2_targetNamespace;
  static std::string                xmlSchema_targetNamespace;
  static std::string                xmlSchemaInstance_targetNamespace;

  static std::string                cityGml_0_4_0_targetNamespace;
  static std::string                cityGml_1_0_targetNamespace;
  static std::string                cityGml_2_0_targetNamespace;
  static std::string                cityGml_3_0_targetNamespace;

  static std::string                cityGml_2_0_buildingNamespace;
  static std::string                cityGml_2_0_bridgeNamespace;
  static std::string                cityGml_2_0_tunnelNamespace;
  static std::string                cityGml_2_0_cityFurnitureNamespace;
  static std::string                cityGml_2_0_genericsNamespace;
  static std::string                cityGml_2_0_landUseNamespace;
  static std::string                cityGml_2_0_reliefNamespace;
  static std::string                cityGml_2_0_transportationNamespace;
  static std::string                cityGml_2_0_vegetationNamespace;
  static std::string                cityGml_2_0_waterBodyNamespace;

  static std::string                cityGml_2_0_georepNamespace;
  static std::string                cityGml_2_0_georepNamespaceKuerzel;

  static std::string                xplanGml_2_0_targetNamespace;
  static std::string                xplanGml_3_0_targetNamespace;
  static std::string                xplanGml_4_0_targetNamespace;
  static std::string                xplanGml_4_1_targetNamespace;
  static std::string                xplanGml_5_0_targetNamespace;
  static std::string                xplanGml_5_1_targetNamespace;
  static std::string                xplanGml_5_2_targetNamespace;

  static std::string                xAL_2_0_targetNamespace;
  static std::string                nas_5_1_targetNamespace;
  static std::string                nas_5_1_1_targetNamespace;
  static std::string                nas_6_0_targetNamespace;

  static std::string                INSPIRE_PLU_4_0_targetNamespace;
  static std::string                INSPIRE_targetNamespace;

  static std::string                boreholeML_3_0_targetNamespace;
  static std::string                boreholeHeaderML_3_0_targetNamespace;
  static std::string                gnde_targetNamespace;
  static std::string                geosciml_2_0_targetNamespace;
  static std::string                cgiUtilities_1_0_targetNamespace;
 
  void setDefaultNamespace ( std::string namespaceP )  {  defaultNamespace = namespaceP; } 
  std::string getDefaultNamespace ()  {  return defaultNamespace;  }

  void addNamespace ( std::string kuerzelP, std::string namespaceP );
  void addSchemadatei ( std::string namespaceP, std::string schemaDatei );

	size_t getNamespaces ( std::vector<std::string> & vKuerzel, std::vector<std::string> & vNamespaces );
	size_t getSchemaDateien ( std::vector<std::string> & vNamespaces, std::vector<std::string> & SchemaDateien );

  std::string getKuerzel ( std::string namespaceName );
  std::string getNamespace ( std::string kuerzelName );
  static GML_SCHEMA_TYPE getSchemaType( std::string standardKuerzel );

  static std::string getCityGML20CodelistModuleFolder ( std::string namespaceKuerzel );
  static std::string getCityGML20CodelistName ( std::string klassenNameQualified, std::string namespaceKuerzel );


  GML_SCHEMA_TYPE getSchemaType();

  bool isINSPIRENamespace();
};

///////////////////////////////////////////////////////////////////////////////
//  SchemaTypeNames                                                          //
//  Wichtige XML-Tags der eingelesenen XML-Schemadateien                     //
///////////////////////////////////////////////////////////////////////////////
class SchemaTypeNames
{
public:
  SchemaTypeNames ( GmlNamespaces * pNamespaceP );
  virtual ~ SchemaTypeNames();

public:

  GmlNamespaces * pNamespace;   // Namespaces der Schemadateien

  std::string rootName;         // Root-Element eines zugehörigen Instanzdokuments
  std::string featureMemberName; // Zugehöriger GML FeatureMember Name

  //  Standard GML Element und Typ-Namen
  std::string gmlAbstractObject;  
  std::string gmlAbstractGML;
  std::string gmlAbstractFeature;
  std::string gmlAbstractFeatureCollection;
  std::string gmlName;
  std::string gmlIdentifier;
  std::string gmlDescription;
  std::string gmlBoundedBy;
  std::string gmlAngleType;
  std::string gmlLengthType;
  std::string gmlAreaType;
  std::string gmlVolumeType;
  std::string gmlScaleType;
  std::string gmlQuantityExtentType;
  std::string gmlSignType;
  std::string gmlAssociationType;
  std::string gmlReferenceType;
  std::string gmlCodeType;
  std::string gmlFeaturePropertyType;
  std::string gmlGeometryPropertyType;
  std::string gmlAbstractGeometricPrimitiveType;
  std::string gmlGeometricPrimitivePropertyType;
  std::string gmlGeometricComplexPropertyType;
  std::string gmlMultiCurvePropertyType;
  std::string gmlCurvePropertyType;
  std::string gmlMultiPointPropertyType;
  std::string gmlPointPropertyType;
  std::string gmlMultiSurfacePropertyType;
  std::string gmlSurfacePropertyType;
  std::string gmlPolygonPropertyType;
  std::string gmlTINPropertyType;
  std::string gmlSolidPropertyType;
  std::string gmlMultiSolidPropertyType;
  std::string gmlMeasureOrNullListType;
  std::string gmlMeasureListType;
  std::string gmlMeasureType;
  std::string gmlSpeedType;
  std::string gmlIntegerListType;
  std::string gmlCodeWithAuthorityType;
  std::string gmlDirectPositionType;
  std::string gmlCalDate;
  std::string gmlTimePositionType;
  std::string gmlTimePeriodType;
  std::string gmlTimeIntervalType;

  std::string gmlAbstractDiscreteCoverageType;

  //  Standard XML Typnamen
  std::string xmlStringType;
  std::string xmlBooleanType;
  std::string xmlDoubleType;
  std::string xmlFloatType;
  std::string xmlDecimalType;
  std::string xmlAngleType;
  std::string xmlIntegerType;
  std::string xmlIntType;
  std::string xmlLongType;
  std::string xmlShortType;
  std::string xmlByteType;
  std::string xmlUnsignedIntType;
  std::string xmlUnsignedLongType;
  std::string xmlUnsignedShortType;
  std::string xmlUnsignedByteType;
  std::string xmlNonNegativeIntegerType;
  std::string xmlNonPositiveIntegerType;
  std::string xmlNegativeIntegerType;
  std::string xmlPositiveIntegerType;
  std::string xmlAnyUriType;
  std::string xmlDateType;
  std::string xmlTimeType;
  std::string xmlDurationType;
  std::string xmlDateTimeType;
  std::string xmlGYearType;
  std::string xmlGMonthType;
  std::string xmlGDayType;
  std::string xmlGYearMonthType;
  std::string xmlGMonthDayType;

  // Elementnamen Externe Referenz
  std::string externalReferenceTag; 
  std::string informationSystemTag;
  std::string externalObjectTag;
  std::string externalObjectNameTag;
  std::string externalObjectURITag;

  // Elementnamen Generische Attribute
  std::string genericAttribute;
  std::string genericStringAttribute;
  std::string genericDoubleAttribute;
  std::string genericIntAttribute;
  std::string genericDateAttribute;
  std::string genericUrlAttribute;
  std::string genericMeasureAttribute;
  std::string genericAttributeSet;
  std::string genericNameAttribute;
  std::string genericValueAttribute;

  // Elementnamen XAl
  std::string xalStreetName;
  std::string xalCity;
  std::string xalZipCode;
  std::string xalHouseNumber;
  std::string xalCountryName;
  std::string xalLocalityName;
  std::string xalThoroughfareNumber;
  std::string xalThoroughfareName;
  std::string xalPostalCodeNumber;

  // Spezielle Geometrietypen XPlanGML
  std::string xpVariableGeometrieType;
  std::string xpPunktGeometrieType;
  std::string xpLinienGeometrieType;
  std::string xpFlaechenGeometrieType;

  //  Spezielle Geometrietypen ALKIS
  std::string advTAPointComponentType;
  std::string advTACurveComponentType;
  std::string advTASurfaceComponentType;
  std::string advTAMultiSurfaceComponentType;
  std::string advAGObjektType;
  std::string advAGPunktobjektType;
  std::string advAGLinienobjektType;
  std::string advAGFlaechenobjektType;

  // Spezielle Typen Coverages
  std::string spatialPlan;
  std::string domainSetProperty;
  std::string rangeSetProperty;
  std::string rangeTypeProperty;
  std::string cvgvpDomainExtentProperty;
  std::string cvgvpElementProperty;
  std::string zoningElementType;

  // Spezielle Typen CityGML
  std::string addressPropertyType;
  std::string tinPropertyType;
  std::string implicitRepresentationPropertyType;
  std::string appearanceMember;
  std::string lodType;

  // Spezielle Typen Zeitreihen
  std::string abstractTimeSeriesType;
  std::string regularTimeSeriesType;
  std::string regularTimeSeriesSetType;
  std::string irregularTimeSeriesType;
  std::string regularTimeSeriesFileType;
  std::string irregularTimeSeriesFileType;

public:
  std::string getGmlKuerzel();
  std::string getGmlCoverageKuerzel();
  std::string getXmlSchemaKuerzel();
};
