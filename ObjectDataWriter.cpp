#include "StdAfx.h"
#include <time.h>
#include <string>
#include <map>
#include <vector>
#include <set>

#include "SrsManagerRef.h"
#include "Feature.h"
#include "GmlSchema.h"
#include "Dictionary.h"
#include "Convert.h"
#include "Checking.h"
#include "UnitOfMeasure.h"
#include "Visualisierung_2D.h"

#include "ObjectDataWriter.h"


///////////////////////////////////////////////////////////////////////////////
//  ObjectDataWriter		                            												 //
//  Export von Attribut-Daten im HTML-Format																 //
///////////////////////////////////////////////////////////////////////////////
ObjectDataWriter::ObjectDataWriter ( String ^ objFolderP, Visualisierung_2D * pVisualisierung )
{
	m_folderName = objFolderP;
	
  m_pVisualisierung  = pVisualisierung;
  m_pPlanObjekte     = m_pVisualisierung->getFeatures();
	m_pGmlObjekte		   = m_pPlanObjekte->getGmlSchema();
	m_pCodeLists		   = m_pGmlObjekte->getCodeListsDictionary();
}

ObjectDataWriter::~ObjectDataWriter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben der Attributdaten eines GML-Objektes			            				 //
///////////////////////////////////////////////////////////////////////////////
void ObjectDataWriter::write ( Feature * pPlanObjektP )
{
	string   gmlId;
	string   klassenName;
	string   attributName;
  string   attributWert;
  string   nameOhneKuerzel;
	string   enumerationName;
	string   enumSchluessel;
	string   enumText;
	string   stdString;
	string   textInhalt;
	String ^ outputFile;
	String ^ STR;
	String ^ STR1;
	size_t   attributAnz, i, index;
	size_t   j;
	bool         success;
  ATTRIBUT_TYP attributTyp;

	GmlKlasse          * pGmlKlasse;
	GmlAttribut        * pGmlAttribut;
	GmlDictionaryEntry * pDictionaryEntry;
	FeatureRelation    * pRelation;
	Feature            * pRelationZiel;

	vector<GmlAttribut*> gmlAttribute;
	vector<FeatureRelation*> relationenGes;
  vector<size_t>        classifierAnz;
  vector<string>        classifierNamen;
  vector<string>        classifierValues;
  vector<string>        classifierNamenLoc;
  vector<string>        classifierValuesLoc;
  vector<UOM*>          vUOMs;

  vector<string> attributNamen;
  vector<string> attributWerte;
  vector<ATTRIBUT_TYP> attributTypen;

  GmlElement * pGmlElement;

	klassenName  = pPlanObjektP->getKlassenName();
  pGmlElement  = m_pGmlObjekte->getGmlElement( klassenName );
  if ( pGmlElement != NULL )
	  pGmlKlasse   = m_pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );


	gmlId = pPlanObjektP->getGmlId();
	outputFile = String::Concat ( m_folderName, QuConvert::ToString ( gmlId ), ".htm" );

  index = 0;

	m_pWriter = gcnew StreamWriter ( outputFile );
	
	m_pWriter->WriteLine ( "<html>" );

	m_pWriter->WriteLine ( "<style type=\"text/css\">" );
	m_pWriter->WriteLine ( "" );
	m_pWriter->WriteLine ( "<!--" );
	m_pWriter->WriteLine ( "h1 {font-family: Arial, Helvetica, sans-serif; text-align: center;}" );
	m_pWriter->WriteLine ( "h2 {font-family: Arial, Helvetica, sans-serif; text-align: center;}" );
	m_pWriter->WriteLine ( "td {font-family: Arial, Helvetica, sans-serif;text-indent: 4px;}" );
	m_pWriter->WriteLine ( "-->" );
	m_pWriter->WriteLine ( "</style>" );

	m_pWriter->WriteLine ( "<title>Objekt-Attribute</title><body>" );

	m_pWriter->WriteLine ( "<body bgcolor=\"#FFFFAA\">" );

  nameOhneKuerzel = removeDefaultNamespaceAttribut( klassenName );
	STR = String::Concat ( "<h1>", QuConvert::ToString ( nameOhneKuerzel ), "</h1>" );
	m_pWriter->WriteLine ( STR );

	m_pWriter->WriteLine ( "<table width=\"100%\" border=\"1\" cellspacing=\"0\" bordercolor=\"#000000\">" );
	m_pWriter->WriteLine ( "<tr>" );
	m_pWriter->WriteLine ( "<td width=\"33%\"><font size=\"+1\"><strong>Attribut-Name</strong></font></td>" );
	m_pWriter->WriteLine ( "<td width=\"66%\"><font size=\"+1\"><strong>Attribut-Wert</strong></font></td>" );
	m_pWriter->WriteLine ( "</tr>" );

	attributName = "gml:id";

	m_pWriter->WriteLine ( "<tr>" );
	STR = String::Concat ( "<td width=\"33%\"><strong>", QuConvert::ToString ( attributName ), "</strong></td>" );
	m_pWriter->WriteLine ( STR );

	STR1 = QuConvert::ToString ( pPlanObjektP->getGmlId() );
	STR1 = String::Concat ( "<td width=\"33%\">", STR1, "</td>" );
	m_pWriter->WriteLine ( STR1 );
	m_pWriter->WriteLine ( "</tr>" );

  attributAnz = pPlanObjektP->getAttributWerte( attributTypen, attributNamen, attributWerte, classifierAnz, classifierNamen, classifierValues, vUOMs );
  pPlanObjektP->getFeatureRelationen( relationenGes );

	for ( i = 0; i < attributAnz; i++ )
	{
    classifierNamenLoc.clear();
    classifierValuesLoc.clear();

    attributTyp     = attributTypen[i];
    attributName    = attributNamen[i];
    attributWert    = attributWerte[i];
    if ( classifierAnz[i] > 0 )
    {
      for ( j = 0; j < classifierAnz[i]; j++ )
      {
        classifierNamenLoc.push_back ( classifierNamen[index+j] );
        classifierValuesLoc.push_back( classifierValues[index+j] );
      }
      index = index + classifierAnz[i];
    }
    if ( vUOMs[i] != NULL )
    {
      classifierNamenLoc.push_back ( "uom" );
      classifierValuesLoc.push_back( vUOMs[i]->getIdentifier() );
//      index++;
    }

    pGmlAttribut = pGmlKlasse->getGmlAttribut( attributName );
    if ( pGmlAttribut == NULL ) continue;
    nameOhneKuerzel = removeDefaultNamespaceAttribut( attributName );

		switch ( pGmlAttribut->getGmlAttributTyp() )
		{
		case _STRING:
			if ( m_pGmlObjekte->getGmlTypAsString()== "2.0"  || m_pGmlObjekte->getGmlTypAsString()== "2.1" )
			{
				if ( attributName == "xplan:textSchluessel" )
					setTextTextlicheFestsetzungen ( attributWert );
				else
				if ( attributName == "xplan:textSchluesselBegruendung" )
					setTextTextlicheBegruendungen ( attributWert );
			}
			break;

		case _DATE:
    case _DATE_TIME:
			break;

		case _DOUBLE:
		case _ANGLE:
    case _LENGTH:
    case _AREA:
    case _VOLUME:
		break;

		case _INTEGER:
			break;

		case _ANY_URI:
			break;

		case _ENUMERATION:
    case _EXTERNAL_CODE_LIST:
			enumerationName = pGmlAttribut->getGmlEnumeration();
      if ( m_pCodeLists != NULL )
			  pDictionaryEntry = m_pCodeLists->getDictionaryEntry ( enumerationName );
      else
        pDictionaryEntry = NULL;

			if ( pDictionaryEntry != NULL )
			{
        enumSchluessel = attributWert;
        success = pDictionaryEntry->getSchluesselText ( enumSchluessel, enumText );
        if ( success ) attributWert = enumText;
      }
			break;

    case _EXTERNAL_CODE_LIST_GML3_3:
      for ( j = 0; j < (int)classifierNamenLoc.size(); j++ )
      {
        if ( classifierNamenLoc[j] == "xlink:title" )
        {
          attributWert = classifierValuesLoc[j];
          break;
        }  
      }
      break;
			
		case  _REFERENZ:
		case _DATA_TYPE:
			break;

		case _BOOLEAN:
 			if ( attributWert == "true" ) attributWert = "ja";
      else				                  attributWert =  "nein";

			break;
    
    default:
      break;
		}
				
	  switch ( pGmlAttribut->getGmlAttributTyp() )
    {
		case _STRING:
		case _DATE:
    case _DATE_TIME:
		case _ENUMERATION:
		case _EXTERNAL_CODE_LIST:
		case _BOOLEAN:
      if ( attributWert != "" )
      {
			  STR1 = QuConvert::ToHtml ( attributWert );
        if ( classifierAnz[i] > 0  )
          STR1 = String::Concat( STR1, " " );
      }
      else
        STR1 = "";

      if ( classifierAnz[i] > 0 )
      {
        STR1 =  String::Concat( STR1, "<font color=\"red\">" );
        for ( j = 0; j < classifierAnz[i]; j++ )
        {
          STR1 = String::Concat( STR1, QuConvert::ToString ( classifierNamenLoc[j] ), " = ", QuConvert::ToString (classifierValuesLoc[j] ) );
          if ( j > 0 )
            STR1 = String::Concat( STR, "; " );
        }
        STR1 =  String::Concat( STR1, "</font" );
      }
      if ( STR1->CompareTo( "" ) == 0 )
        STR1 = " ";
      STR1 = String::Concat ( "<td width=\"33%\">", STR1, "</td>" );
			break;

		case _ANY_URI:
			STR1 = QuConvert::ToHtml ( attributWert );
			STR1 = String::Concat ( "<td width=\"33%\"><a href=\"", STR1,"\">", STR1, "</a></td>" );
			break;

		case _DOUBLE:
		case _ANGLE:
    case _LENGTH:
    case _AREA:
    case _VOLUME:

      if ( attributWert != "" )
      {
        STR1 = QuConvert::ToHtml ( attributWert );
        if ( classifierAnz[i] > 0  )
          STR1 = String::Concat( STR1, "; " );
      }
      else
        STR1 = " ";

      if ( classifierAnz[i] > 0 )
      {
        STR1 =  String::Concat( STR1, "<font color=\"red\">" );
        for ( j = 0; j < classifierAnz[i]; j++ )
        {
          STR1 = String::Concat( STR1, QuConvert::ToString ( classifierNamenLoc[j] ), " = ", QuConvert::ToString (classifierValuesLoc[j] ) );
          if ( j > 0 )
            STR1 = String::Concat( STR, "; " );
        }
        STR1 =  String::Concat( STR1, "</font>" );
      }
      if ( STR1->CompareTo( "" ) == 0 )
        STR1 = " ";
      STR1 = String::Concat ( "<td width=\"33%\">", STR1, "</td>" );
			break;

		case _INTEGER:
		  STR1 = String::Concat ( "<td width=\"33%\">", QuConvert::ToString ( attributWert ), "</td>" );
			break;

		case _REFERENZ:
		case _DATA_TYPE:
			break;

		 default:
				continue;
    }
			
    if ( STR1 != nullptr )
    {
		  m_pWriter->WriteLine ( "<tr>" );
		  STR = String::Concat ( "<td width=\"33%\"><strong>", QuConvert::ToString ( nameOhneKuerzel ), "</strong></td>" );
		  m_pWriter->WriteLine ( STR );
		  m_pWriter->WriteLine ( STR1 );
		  m_pWriter->WriteLine ( "</tr>" );
     }
  }

  for ( i = 0; i < relationenGes.size(); i++ )
  {
    pRelation = relationenGes[i];
    nameOhneKuerzel = removeDefaultNamespaceAttribut( pRelation->rollenName );

    if ( pRelation->rollenName == "xplan:gehoertZuPlan"              ||
      pRelation->rollenName == "xplan:gehoertZuBereich"              ||
      pRelation->rollenName == "xplan:gehoertZuBP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuFP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuLP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuRP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuSO_Bereich"           ||
      pRelation->rollenName == "xplan:fachobjekt"                    ||
      pRelation->rollenName == "xplan:nachrichtlich"                 ||
      pRelation->rollenName == "xplan:wirdDargestelltDurch"          ||
      pRelation->rollenName == "xplan:dientZurDarstellungVon"        ||
      pRelation->rollenName == "xplan:gehoertNachrichtlichZuBereich" ||
      pRelation->rollenName == "xplan:inhaltBPlan"                   ||
      pRelation->rollenName == "xplan:inhaltFPlan"                   ||
      pRelation->rollenName == "xplan:inhaltRPlan"                   ||
      pRelation->rollenName == "xplan:inhaltLPlan"                   ||
      pRelation->rollenName == "xplan:inhaltSoPlan"                  ||
      pRelation->rollenName == "xplan:praesentationsobjekt"          ||
      pRelation->rollenName == "plu:restriction"                     ||
      pRelation->rollenName == "plu:plan"
      ) continue;

    if( pRelation->rollenName == "texte" ||
      pRelation->rollenName == "refTextInhalt")
    {
      pRelationZiel = m_pPlanObjekte->getFeature ( pRelation->featureId );
      if ( pRelationZiel != NULL )
      {
        pRelationZiel->getStringAttributWert ( "text", textInhalt );
        STR1 = QuConvert::ToHtml ( textInhalt );
        STR1 =String::Concat ( "<td width=\"33%\">", STR1, "</td>" );
      }
      else
        STR1 =nullptr;
    }
    else
      STR1 =nullptr;

    if ( STR1 != nullptr )
    {
      m_pWriter->WriteLine ( "<tr>" );
      STR = String::Concat ( "<td width=\"33%\"><strong>", QuConvert::ToString ( nameOhneKuerzel ), "</strong></td>" );
      m_pWriter->WriteLine ( STR );
      m_pWriter->WriteLine ( STR1 );
      m_pWriter->WriteLine ( "</tr>" );
    }
  }

	m_pWriter->WriteLine ( "</table>" );

	for ( i = 0; i < relationenGes.size(); i++ )
	{
	 pRelation = relationenGes[i];
    if ( pRelation->rollenName == "xplan:gehoertZuPlan"              ||
      pRelation->rollenName == "xplan:gehoertZuBereich"              ||
      pRelation->rollenName == "xplan:gehoertZuBP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuFP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuLP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuRP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuSO_Bereich"           ||
      pRelation->rollenName == "xplan:wirdDargestelltDurch"          ||
      pRelation->rollenName == "xplan:dientZurDarstellungVon"        ||
      pRelation->rollenName == "xplan:fachobjekt"                    ||
      pRelation->rollenName == "xplan:nachrichtlich"                 ||
      pRelation->rollenName == "xplan:gehoertNachrichtlichZuBereich" ||
      pRelation->rollenName == "xplan:inhaltBPlan"                   ||
      pRelation->rollenName == "xplan:inhaltFPlan"                   ||
      pRelation->rollenName == "xplan:inhaltRPlan"                   ||
      pRelation->rollenName == "xplan:inhaltLPlan"                   ||
      pRelation->rollenName == "xplan:inhaltSoPlan"                  ||
      pRelation->rollenName == "plu:restriction"                     ||
      pRelation->rollenName == "xplan:praesentationsobjekt"          ||
      pRelation->rollenName == "plu:plan"     
      ) continue;
		
      pRelationZiel = m_pPlanObjekte->getFeature ( pRelation->featureId );
		  if ( pRelationZiel != NULL )
		  {
        nameOhneKuerzel = removeDefaultNamespaceAttribut( pRelation->rollenName );
			  STR = String::Concat ( "<h1>", QuConvert::ToString ( nameOhneKuerzel ), "</h1>" );
			  m_pWriter->WriteLine ( STR );
			  writeRelation ( pRelationZiel );
		  }
	  }

	  m_pWriter->WriteLine ( "</body>" );
	  m_pWriter->WriteLine ( "</html>" );

	  m_pWriter->Flush();
	  m_pWriter->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben der Attribute eines referierten Objektes				          		 //
///////////////////////////////////////////////////////////////////////////////
void ObjectDataWriter::writeRelation ( Feature * pPlanObjektP )
{
	string   gmlId;
	string   klassenName;
	string   attributName;
  string   attributWert;
  string   nameOhneKuerzel;
	string   enumerationName;
	string   enumSchluessel;
	string   enumText;
	string   stdString;
	String ^ STR;
	String ^ STR1;
	size_t   attributAnz, i, index;
  int          j;
	bool         success;
  ATTRIBUT_TYP attributTyp;

	GmlKlasse          * pGmlKlasse;
  GmlElement         * pGmlElement;
	GmlAttribut        * pGmlAttribut;
	GmlDictionaryEntry * pDictionaryEntry;
	FeatureRelation    * pRelation;
	Feature         * pRelationZiel;

	vector<FeatureRelation*> relationenGes;
  vector<size_t>           classifierAnz;
	vector<string>           classifierNamen;
	vector<string>           classifierValues;
  vector<string>           classifierNamenLoc;
  vector<string>           classifierValuesLoc;
  vector<UOM*>             vUOMs;

  vector<string> attributNamen;
  vector<string> attributWerte;
  vector<ATTRIBUT_TYP> attributTypen;

	klassenName  = pPlanObjektP->getKlassenName();
  pGmlElement  = m_pGmlObjekte->getGmlElement( klassenName );
  if ( pGmlElement != NULL )
	  pGmlKlasse   = m_pGmlObjekte->getGmlKlasse ( pGmlElement->getGmlTypeName() );

	gmlId = pPlanObjektP->getGmlId();

	m_pWriter->WriteLine ( "<table width=\"100%\" border=\"1\" cellspacing=\"0\" bordercolor=\"#000000\">" );
	m_pWriter->WriteLine ( "<tr>" );
	m_pWriter->WriteLine ( "<td width=\"33%\"><font size=\"+1\"><strong>Attribut-Name</strong></font></td>" );
	m_pWriter->WriteLine ( "<td width=\"66%\"><font size=\"+1\">Attribut-Wert</strong></font></td>" );
	m_pWriter->WriteLine ( "</tr>" );

  attributAnz = pPlanObjektP->getAttributWerte( attributTypen, attributNamen, attributWerte, classifierAnz, classifierNamen, classifierValues, vUOMs );
  pPlanObjektP->getFeatureRelationen ( relationenGes );

  index = 0;
	for ( i = 0; i < attributAnz; i++ )
	{
    attributTyp     = attributTypen[i];
    attributName    = attributNamen[i];
    attributWert    = attributWerte[i];
    if ( classifierAnz[i] > 0 )
    {
      for ( j = 0; j < classifierAnz[i]; j++ )
      {
        classifierNamenLoc.push_back ( classifierNamen[index+j] );
        classifierValuesLoc.push_back( classifierValues[index+j] );
      }
      index = index + classifierAnz[i];
    }

    if ( vUOMs[i] != NULL )
    {
      classifierNamenLoc.push_back ( "uom" );
      classifierValuesLoc.push_back( vUOMs[i]->getIdentifier() );
    }

    pGmlAttribut = pGmlKlasse->getGmlAttribut( attributName );
    if ( pGmlAttribut == NULL ) continue;

    nameOhneKuerzel = removeDefaultNamespaceAttribut( attributName );

		switch ( pGmlAttribut->getGmlAttributTyp() )
		{
		case _STRING:
			break;

		case _DATE:
    case _DATE_TIME:
			break;

		case _DOUBLE:
		case _ANGLE:
		case _LENGTH:
		case _AREA:
		case _VOLUME:
			break;

		case _INTEGER:
			break;

		case _ANY_URI:
			break;

		case _ENUMERATION:
    case _EXTERNAL_CODE_LIST:
      enumerationName = pGmlAttribut->getGmlEnumeration();
      if ( m_pCodeLists != NULL )
        pDictionaryEntry = m_pCodeLists->getDictionaryEntry ( enumerationName );
      else
        pDictionaryEntry = NULL;

      if ( pDictionaryEntry != NULL )
      {
        enumSchluessel = attributWert;
        success = pDictionaryEntry->getSchluesselText ( enumSchluessel, enumText );
        if ( success ) attributWert = enumText;
      }

			break;		
		
		case  _REFERENZ:
			break;

		case _BOOLEAN:
      if ( attributWert == "true" ) attributWert = "ja";
      else				                  attributWert =  "nein";

			break;

    default:
			break;
		}
				
		switch ( pGmlAttribut->getGmlAttributTyp() )
		{
		case _STRING:
		case _EXTERNAL_CODE_LIST:
      if ( attributWert != "" )
      {
        STR1 = QuConvert::ToHtml ( attributWert );
        if ( classifierAnz[i] > 0  )
          STR1 = String::Concat( STR1, " " );
      }
      else
        STR1 = "";

      if ( classifierAnz[i] > 0 )
      {
        STR1 =  String::Concat( STR1, "<font color=\"red\">" );
        for ( j = 0; j < classifierAnz[i]; j++ )
        {
          STR1 = String::Concat( STR1, QuConvert::ToString ( classifierNamenLoc[j] ), " = ", QuConvert::ToString (classifierValuesLoc[j] ) );
          if ( j > 0 )
            STR1 = String::Concat( STR, "; " );
        }
        STR1 =  String::Concat( STR1, "</font>" );

      }
      if ( STR1->CompareTo( "" ) == 0 )
        STR1 = " ";
      STR1 = String::Concat ( "<td width=\"33%\">", STR1, "</td>" );
      break;

		case _DATE:
		case _ENUMERATION:
		case _BOOLEAN:
    case _DATE_TIME:
      if ( attributWert != "" )
      {
        STR1 = QuConvert::ToHtml ( attributWert );
        if ( classifierAnz[i] > 0  )
          STR1 = String::Concat( STR1, "; " );
      }
      else
        STR1 = "";

      if ( classifierAnz[i] > 0 )
      {
        STR1 =  String::Concat( STR1, "<font color=\"red\">" );
        for ( j = 0; j < classifierAnz[i]; j++ )
        {
          STR1 = String::Concat( STR1, QuConvert::ToString ( classifierNamenLoc[j] ), " = ", QuConvert::ToString (classifierValuesLoc[j] ) );
          if ( j > 0 )
            STR1 = String::Concat( STR, "; " );
        }
        STR1 =  String::Concat( STR1, "</font" );
      }
      if ( STR1->CompareTo( "" ) == 0 )
        STR1 = " ";
      STR1 = String::Concat ( "<td width=\"33%\">", STR1, "</td>" );			break;

		case _ANY_URI:
			STR1 = QuConvert::ToHtml ( attributWert );
			STR1 = String::Concat ( "<td width=\"33%\"><a href=\"", STR1,"\">",STR1, "</a></td>" );
			break;

		case _DOUBLE:
		case _ANGLE:
		case _LENGTH:
		case _VOLUME:
		case _AREA:
      if ( attributWert != "" )
      {
        STR1 = QuConvert::ToHtml ( attributWert );
        if ( classifierAnz[i] > 0  )
          STR1 = String::Concat( STR1, "; " );
      }
      else
        STR1 = "";

      if ( classifierAnz[i] > 0 )
      {
        STR1 =  String::Concat( STR1, "<font color=\"red\">" );
        for ( j = 0; j < classifierAnz[i]; j++ )
        {
          STR1 = String::Concat( STR1, QuConvert::ToString ( classifierNamenLoc[j] ), " = ", QuConvert::ToString (classifierValuesLoc[j] ) );
          if ( j > 0 )
            STR1 = String::Concat( STR, "; " );
        }
        STR1 =  String::Concat( STR1, "</font" );
      }
      if ( STR1->CompareTo( "" ) == 0 )
        STR1 = " ";
      STR1 = String::Concat ( "<td width=\"33%\">", STR1, "</td>" );
			break;

		case _INTEGER:
			STR1 = String::Concat ( "<td width=\"33%\">", QuConvert::ToString ( attributWert ), "</td>" );
			break;

		case _REFERENZ:
    case _DATA_TYPE:
  
			break;

		}
			
		m_pWriter->WriteLine ( "<tr>" );
		STR = String::Concat ( "<td width=\"33%\"><strong>", QuConvert::ToString ( nameOhneKuerzel ), "</strong></td>" );
		m_pWriter->WriteLine ( STR );
		m_pWriter->WriteLine ( STR1 );
		m_pWriter->WriteLine ( "</tr>" );

  }

  for ( i = 0; i < relationenGes.size(); i++ )
  {
    pRelation = relationenGes[i];
    attributName = pRelation->rollenName;
    nameOhneKuerzel = removeDefaultNamespaceAttribut( attributName );

    if ( pRelation->rollenName == "xplan:gehoertZuPlan"              ||
      pRelation->rollenName == "xplan:gehoertZuBereich"              ||
      pRelation->rollenName == "xplan:gehoertZuBP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuFP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuLP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuRP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuSO_Bereich"           ||
      pRelation->rollenName == "xplan:fachobjekt"                    ||
      pRelation->rollenName == "xplan:nachrichtlich"                 ||
      pRelation->rollenName == "xplan:gehoertNachrichtlichZuBereich" ||
      pRelation->rollenName == "xplan:inhaltBPlan"                   ||
      pRelation->rollenName == "xplan:inhaltFPlan"                   ||
      pRelation->rollenName == "xplan:inhaltRPlan"                   ||
      pRelation->rollenName == "xplan:inhaltLPlan"                   ||
      pRelation->rollenName == "xplan:inhaltSoPlan"                  ||
      pRelation->rollenName == "xplan:praesentationsobjekt"          ||
      pRelation->rollenName == "xplan:dientZurDarstellungVon"        ||
      pRelation->rollenName == "xplan:wirdDargestelltDurch"          ||
      pRelation->rollenName == "plu:restriction"                     ||
      pRelation->rollenName == "plu:plan"
      ) continue;

    STR1 =String::Concat ( "<td width=\"33%\">", "&nbsp;", "</td>" );

    m_pWriter->WriteLine ( "<tr>" );
    STR = String::Concat ( "<td width=\"33%\"><strong>", QuConvert::ToString ( nameOhneKuerzel ), "</strong></td>" );
    m_pWriter->WriteLine ( STR );
    m_pWriter->WriteLine ( STR1 );
    m_pWriter->WriteLine ( "</tr>" );
  }

	m_pWriter->WriteLine ( "</table>" );

	for ( i = 0; i < relationenGes.size(); i++ )
	{
		pRelation = relationenGes[i];

    if ( pRelation->rollenName == "xplan:gehoertZuPlan"              ||
      pRelation->rollenName == "xplan:gehoertZuBereich"              ||
      pRelation->rollenName == "xplan:gehoertZuBP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuFP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuLP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuRP_Bereich"           ||
      pRelation->rollenName == "xplan:gehoertZuSO_Bereich"           ||
      pRelation->rollenName == "xplan:fachobjekt"                    ||
      pRelation->rollenName == "xplan:nachrichtlich"                 ||
      pRelation->rollenName == "xplan:gehoertNachrichtlichZuBereich" ||
      pRelation->rollenName == "xplan:inhaltBPlan"                   ||
      pRelation->rollenName == "xplan:inhaltFPlan"                   ||
      pRelation->rollenName == "xplan:inhaltRPlan"                   ||
      pRelation->rollenName == "xplan:inhaltLPlan"                   ||
      pRelation->rollenName == "xplan:inhaltSoPlan"                  ||
      pRelation->rollenName == "xplan:praesentationsobjekt"          ||
      pRelation->rollenName == "xplan:dientZurDarstellungVon"        ||
      pRelation->rollenName == "xplan:wirdDargestelltDurch"          ||
      pRelation->rollenName == "plu:restriction"                     ||
      pRelation->rollenName == "plu:plan"
      ) continue;

		pRelationZiel = m_pPlanObjekte->getFeature ( pRelation->featureId );
		if ( pRelationZiel != NULL )
		{
			STR = String::Concat ( "<h1>", QuConvert::ToString ( pRelation->rollenName ), "</h1>" );
			m_pWriter->WriteLine ( STR );
			writeRelation ( pRelationZiel );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Ersetzt die Schlüssel von Textlichen Festsetzungen durch den zug. Texte  //
//  bzw. die URLs														                              	 //
///////////////////////////////////////////////////////////////////////////////
void ObjectDataWriter::setTexteTextlicheFestsetzungen ( std::vector<std::string> &stringAttributWerte )
{
	unsigned int i;
	bool         success;
	Feature * pTextlicheFestsetzung;
	string       schluessel;
	string       text;

	BLeitPlan * pPlan = m_pVisualisierung->getPlanAktuell();

	for ( i = 0; i < stringAttributWerte.size(); i++ )
	{
		schluessel = stringAttributWerte[i];
		pTextlicheFestsetzung = pPlan->getTextlicheFestsetzung ( schluessel );
		if ( pTextlicheFestsetzung == NULL ) continue;

		success = pTextlicheFestsetzung->getStringAttributWert ( "text", text );
		if ( ! success ) 
		success = pTextlicheFestsetzung->getURLAttributWert ( "textURL", text );
		if ( !success ) continue;
		
		stringAttributWerte[i] = text;
	}

}

///////////////////////////////////////////////////////////////////////////////
//  Ersetzt den Schlüssel einer Textlichen Festsetzung durch den zug. Text   //
//  bzw. die URL														                              	 //
///////////////////////////////////////////////////////////////////////////////
void ObjectDataWriter::setTextTextlicheFestsetzungen ( std::string &stringAttributWert )
{
  bool         success;
  Feature * pTextlicheFestsetzung;
  string       schluessel;
  string       text;

  BLeitPlan * pPlan = m_pVisualisierung->getPlanAktuell();

  pTextlicheFestsetzung = pPlan->getTextlicheFestsetzung ( stringAttributWert );
  if ( pTextlicheFestsetzung == NULL ) return;

  success = pTextlicheFestsetzung->getStringAttributWert ( "text", text );
  if ( ! success ) 
    success = pTextlicheFestsetzung->getURLAttributWert ( "textURL", text );
  if ( !success ) return;

  stringAttributWert = text;
}

///////////////////////////////////////////////////////////////////////////////
//  Ersetzt den Schlüssel der Textlichen Begründungen durch die zug. Texte   //
//  bzw. die URLs			                                											 //
///////////////////////////////////////////////////////////////////////////////
void ObjectDataWriter::setTexteTextlicheBegruendungen ( std::vector<std::string> &stringAttributWerte )
{
	unsigned int i;
	bool         success;
	Feature * pTextlicheBegruendung;
  string       schluessel;
	string       text;

	BLeitPlan * pPlan = m_pVisualisierung->getPlanAktuell();

	for ( i = 0; i < stringAttributWerte.size(); i++ )
	{
		schluessel = stringAttributWerte[i];
		pTextlicheBegruendung = pPlan->getTextlicheBegruendung ( schluessel );
		if ( pTextlicheBegruendung == NULL ) continue;

		success = pTextlicheBegruendung->getStringAttributWert ( "text", text );
		if ( ! success ) 
		success = pTextlicheBegruendung->getURLAttributWert ( "textURL", text );
		if ( !success ) continue;
		
		stringAttributWerte[i] = text;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Ersetzt den Schlüssel einer Textlichen Begründung durch den zug. Text    //
//  bzw. die URL			                                											 //
///////////////////////////////////////////////////////////////////////////////
void ObjectDataWriter::setTextTextlicheBegruendungen ( std::string &stringAttributWert )
{
  bool         success;
  Feature * pTextlicheBegruendung;
  string       text;

  BLeitPlan * pPlan = m_pVisualisierung->getPlanAktuell();

  pTextlicheBegruendung = pPlan->getTextlicheBegruendung ( stringAttributWert );
  if ( pTextlicheBegruendung == NULL ) return;

  success = pTextlicheBegruendung->getStringAttributWert ( "text", text );
  if ( ! success ) 
    success = pTextlicheBegruendung->getURLAttributWert ( "textURL", text );
  if ( !success ) return;

  stringAttributWert = text;
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt vor GML Klassen- bzw. Attributname das Default Namespace-Kürzel //
///////////////////////////////////////////////////////////////////////////////
string ObjectDataWriter::removeDefaultNamespaceAttribut ( string gmlName )
{
	size_t index;
  string kuerzel;

  index = gmlName.find ( ":" );
  if ( index != string::npos )
  {
    kuerzel = gmlName.substr( 0, index );
    if ( kuerzel == "xplan" )
      gmlName = gmlName.substr ( index+1, gmlName.length() );
  }

  return gmlName;
}









