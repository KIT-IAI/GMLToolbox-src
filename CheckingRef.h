#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Windows::Forms;
using namespace System::Text;
using namespace System::Collections;

#include "includes.h"
#include "Geometrie.h"

class ProfileRule;
class ProfileFilter;
class AttributeFilter;
class AttributeCardinalityFilter;
class RelationFilter;
class ErrorReport;
class SemanticError;
class CodeListError;
class AttributeValueList;
class NamesValuesList;
class GroupFilter;
class GeometryFilter;
class BLeitPlan;
class PlanBereich;
class Feature;
class GmlAttribut;
class GmlSchema;
class MultiSurface;
class SemanticRules;
class GeometryError;
class GmlNamespaces;
class GmlModelStatistics;
class CheckingMethods;
class ReadError;
class ValidationError;
class UOMList;
class Logfile;

///////////////////////////////////////////////////////////////////////////////
//  SchemaValidierung                                                        //
//  Hilfsklasse zur Schema-Validierung                                       //
///////////////////////////////////////////////////////////////////////////////
ref class SchemaValidierung
{
  public:
    SchemaValidierung ( GmlSchema * pSchemaP, GmlNamespaces * pNamespacesStandard, ErrorReport * pErrorReport, std::string dateiName );
    virtual ~SchemaValidierung();

  private:
    GmlNamespaces * m_pNamespaces;  // Namespaces der Schema-Dateien
    GmlSchema     * m_pSchema;      // XML-Schema
    ErrorReport   * m_pErrorReport; // Festgestellte Validierungsfehler
    String        ^ m_pDateiName;   // Pfadname der zu validierenden Datei 

  public: 
    void Validate();
    void ValidateProfile( std::string profilName );
    
    String ^ getTargetNamespace ( String ^ schemaDatei );

private:
  void ValidationCallback(System::Object ^ sender, ValidationEventArgs ^ args );

};

///////////////////////////////////////////////////////////////////////////////
//  ProfileReader                                                            //
//  Einlesen einer Datei mit Konformitätsbedingungen                         //
///////////////////////////////////////////////////////////////////////////////
public ref class ProfileReader
{
	public:
		ProfileReader( SemanticRules * pProfileP );
		virtual ~ ProfileReader();

	protected:
		SemanticRules * pProfile;
		XmlTextReader   ^ validReader;
		System::String  ^ pActElement;

	public:
		bool read ( std::string filename );

	protected:
		bool readRule();
		bool readProfileFilter ( ProfileRule * pRule, std::string ruleType, String ^ combination  );
    std::string addDefaultNamespaceKuerzel ( std::string name );
};

///////////////////////////////////////////////////////////////////////////////
//  ErrorReportWriter                                                        //
//  Schreiben der festgestellten Konformitätsfehler	              					 //
///////////////////////////////////////////////////////////////////////////////
public ref class ErrorReportWriter
{
	public:
		ErrorReportWriter (  ErrorReport * pErrorReportP );
		virtual ~ ErrorReportWriter();

	protected:
		ErrorReport   * m_pErrorReport;   //  Sammlung der festgestellten Fehler
		XmlTextWriter ^ writer;           // .NET XML-Writer

	public:
		void write ( std::string fileName );

	protected:
		void writeSemanticError ( SemanticError * pError );
    void writeGeometryError (  GeometryError * pError );

		void writeProfileRule ( ProfileRule * pProfileRule );
		void writeProfileFilter ( String ^ pFilterType, ProfileFilter * pFilter );
		void writeAttributeFilter ( AttributeFilter * pAttributeFilter );
		void writeRelationFilter ( RelationFilter * pRelationFilter );
//		void writeCodeListError ( CodeListError * pCodeListError );
		void writeNamesValuesList ( String ^ pListType, NamesValuesList * pList );
};
