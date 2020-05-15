#pragma once
using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Text;
using namespace System::Collections;

#include "includes.h"

class Feature;
class GmlToolbox;
class GmlSchema;
class Visualisierung_2D;

///////////////////////////////////////////////////////////////////////////////
//  ObjectDataWriter                                                         //
//  Export von Attribut-Daten im HTML-Format																 //
///////////////////////////////////////////////////////////////////////////////
public ref class ObjectDataWriter
{
public:
	ObjectDataWriter ( String ^ objFolderP, Visualisierung_2D * pVisualisierung );
	virtual ~ ObjectDataWriter();

protected:
  Visualisierung_2D * m_pVisualisierung;   // Parameter für 2D-Visualisierung mit SVG
	String		        ^ m_folderName;				 // Ordner-Pfad für die erzeugten HTML-Dateien
	StreamWriter      ^ m_pWriter;					 // .NET Text-Writer

  Features      * m_pPlanObjekte;					 // Aggregation der Featutes
	GmlSchema     * m_pGmlObjekte;					 // GML-Schema
	GmlDictionary * m_pCodeLists;					   // Dictionary der Enumerationen

public:
	void write ( Feature * pPlanObjektP );
	void writeRelation ( Feature * pPlanObjektP  );

protected:
	void setTexteTextlicheFestsetzungen ( std::vector<std::string> &stringAttributWerte );
  void setTextTextlicheFestsetzungen ( std::string &stringAttributWert );

	void setTexteTextlicheBegruendungen ( std::vector<std::string> &stringAttributWerte );
  void setTextTextlicheBegruendungen ( std::string &stringAttributWert );
  std::string removeDefaultNamespaceAttribut ( std::string name );
};



