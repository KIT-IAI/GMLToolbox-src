#include "stdafx.h"

#include <vector>
#include <map>
#include <set>
#include <string>

#include "XPlanGML.h"
#include "ShpFileObjekte.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "GmlSchema.h"
#include "Convert.h"
#include "Logfile.h"
#include "ShpFileNamenKonv.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  ShapeFileNamenKonvertierung																							 //
//  Konvertierung von Datei- und Attributnamen der Shape-Files in XPlanGML   //
//  Standards																                                 //
///////////////////////////////////////////////////////////////////////////////
ShapeFileNamenKonvertierung::ShapeFileNamenKonvertierung(  XPlanGML * pXPlanGML )
{
	m_pXPlanGML = pXPlanGML;
	plaeneAusShapeErzeugen = false;
}

ShapeFileNamenKonvertierung::~ShapeFileNamenKonvertierung()
{
	std::map<std::string,KlassenName*>::iterator iterKlassen;
	std::map<std::string,Enumeration*>::iterator iterEnums;

	for ( iterKlassen = klassenNamenMap.begin(); iterKlassen != klassenNamenMap.end(); iterKlassen++ )
		delete iterKlassen->second;

	for ( iterEnums = enumerationMap.begin(); iterEnums != enumerationMap.end(); iterEnums++ )
		delete iterEnums->second;

}
///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen der Abbildungsvorschriften für einen Shapefile      					 //
///////////////////////////////////////////////////////////////////////////////
KlassenName *  ShapeFileNamenKonvertierung::addKlassenName 
		( string & shpDateiNameP, string & klassenNameP, int bereichNummerP, string rechtlicheBedeutungStr = "" )
{
  int rechtlicheBedeutung;

  if ( rechtlicheBedeutungStr == "PLANINHALT" ) 
    rechtlicheBedeutung = PLANINHALT;
  else
  if ( rechtlicheBedeutungStr == "NACHR_UEBERNAHME" ) 
    rechtlicheBedeutung = NACHR_UEBERNAHME;
  else
    rechtlicheBedeutung = SONST_RECHTL_BEDEUTUNG;

  return addKlassenName ( shpDateiNameP, klassenNameP, bereichNummerP, rechtlicheBedeutung );	
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen der Abbildungsvorschriften für einen Shapefile      					 //
///////////////////////////////////////////////////////////////////////////////
KlassenName *  ShapeFileNamenKonvertierung::addKlassenName 
		( string & shpDateiNameP, string & klassenNameP, int bereichNummerP, int rechtlicheBedeutung )
{
  std::string text;

	KlassenName * pKlassenName = new KlassenName ( this, shpDateiNameP, klassenNameP, bereichNummerP, rechtlicheBedeutung );

	text = shpDateiNameP;
	text+= " --> ";
	text += klassenNameP;
	m_pXPlanGML->getLogfile()->writeLogFile ( text );

	klassenNamenMap.insert (  multimap<string,KlassenName*>::value_type ( shpDateiNameP, pKlassenName ) );
	return pKlassenName;
}

///////////////////////////////////////////////////////////////////////////////
//  Generierung eines neuen Objektex Enumeration														 //
///////////////////////////////////////////////////////////////////////////////
Enumeration * ShapeFileNamenKonvertierung::addEnumeration ( std::string & enumerationIdP )
{
	Enumeration * pEnumeration = new Enumeration ( enumerationIdP );
	enumerationMap[enumerationIdP] = pEnumeration;
	return pEnumeration;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Objektes Enumeration																		 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileNamenKonvertierung::addEnumeration ( Enumeration * pEnumerationP )
{
	string enumerationId = pEnumerationP->getEnumerationId();
	enumerationMap[enumerationId] = pEnumerationP;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert KlassenName Objekt zu Shape-File-Namen		            					 //
///////////////////////////////////////////////////////////////////////////////
KlassenName * ShapeFileNamenKonvertierung::getKlassenNameObj ( string &shapeFileNameP )
{
	map<string,KlassenName*>::iterator iter;

	iter = klassenNamenMap.find ( shapeFileNameP );
	if ( iter != klassenNamenMap.end() ) return iter->second;
	else return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle KlassenName Objekte	                         							 //
///////////////////////////////////////////////////////////////////////////////
size_t ShapeFileNamenKonvertierung::getKlassenNamen ( vector<KlassenName*> & klassenNamenP )
{
	map<string,KlassenName*>::iterator iter;

	for ( iter = klassenNamenMap.begin(); iter != klassenNamenMap.end(); iter++ )
	{
		KlassenName * pKlassenName = iter->second;
		klassenNamenP.push_back ( pKlassenName );
	}

	return klassenNamenMap.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den XPlanGML Klassen-Namen, der einem Shapefile zugeordnet ist   //
///////////////////////////////////////////////////////////////////////////////
std::string ShapeFileNamenKonvertierung::getKlassenName ( std::string & shpDateiName )
{
	map<std::string,KlassenName*>::iterator iter;
	
	iter = klassenNamenMap.find ( shpDateiName );
	if ( iter != klassenNamenMap.end() )
	{
		KlassenName * pKlassenName = iter->second;
		return pKlassenName->getKlassenName();
	}
	else								 return "Error";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Attribut-Objekte, die einem Shapefile zugeordnet sind			 //
///////////////////////////////////////////////////////////////////////////////
size_t ShapeFileNamenKonvertierung::getAttributObjekte (  string & shpDateiNameP, 
				string & shpAttributNameP, vector<AttributName*> & vAttrObjekteP )
{
	KlassenName * pKlassenName;
	std::map<std::string,KlassenName*>::iterator iter;

	pKlassenName = getKlassenNameObj ( shpDateiNameP );
	if ( pKlassenName == NULL )
		pKlassenName = getKlassenNameObj ( string ( "Basisklasse" ) );

	if ( pKlassenName != NULL )
		return  pKlassenName->getAttributObjekte ( shpAttributNameP, vAttrObjekteP );
	else return 0;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Enumeration Objekt                         									 //
///////////////////////////////////////////////////////////////////////////////
Enumeration  * ShapeFileNamenKonvertierung::getEnumeration  ( std::string & enumerationIdP )
{
	std::map<std::string,Enumeration*>::iterator iterEnums;

	iterEnums = enumerationMap.find ( enumerationIdP );

	if ( iterEnums != enumerationMap.end() ) return iterEnums->second;
	else									 return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Enumeration Objekte		                      							 //
///////////////////////////////////////////////////////////////////////////////
size_t ShapeFileNamenKonvertierung::getEnumerations ( vector<Enumeration*> & enumerationsP )
{
	map<string,Enumeration*>::iterator iter;

	for ( iter = enumerationMap.begin(); iter != enumerationMap.end(); iter++ )
	{
		Enumeration * pEnum = iter->second;
		enumerationsP.push_back ( pEnum );
	}

	return enumerationMap.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Überträgt die Abbildungs-Informationen in die Klasse ShapeFileObjekte		 //
///////////////////////////////////////////////////////////////////////////////
int ShapeFileNamenKonvertierung::adaptShapeFileObjekte()
{
	map<string,KlassenName*>::iterator iterKlassen;
	map<string,Enumeration*>::iterator iterEnum;

	string shapeFileName;
	string error;

	KlassenName     * pKlassenName;
	ShapeFileObjekt * pShapeFileObjekt;

	errorAnz = 0;

	GmlSchema       * pGmlObjekte       = m_pXPlanGML->getFeatures()->getGmlSchema();
	ShapeFileObjekte * pShapeFileObjekte = m_pXPlanGML->getShapeFileObjekte();
	if ( pShapeFileObjekte == NULL )
	{
		error = "Keine Shape-Dateien ausgewählt";
		errorAnz++;
		return errorAnz;
	}

	pShapeFileObjekte->resetGmlZuordnung();
	pShapeFileObjekte->setPlaeneAusShapeFileErzeugen ( plaeneAusShapeErzeugen );

	for ( iterKlassen = klassenNamenMap.begin(); iterKlassen != klassenNamenMap.end(); iterKlassen++ )
	{
		pKlassenName = iterKlassen->second;

		shapeFileName = pKlassenName->getShapeFileName();
		pShapeFileObjekt = pShapeFileObjekte->getShapeFileObjekt ( shapeFileName );
		if ( pShapeFileObjekt == NULL )
		{
			error = "In Konvertierung aufgeführte Shape-Datei wurde nicht ausgewählt: ";
			error += shapeFileName;      
			m_pXPlanGML->getLogfile()->writeLogFile ( error );
			error = "\n";
			errorAnz++;
			m_pXPlanGML->getLogfile()->writeLogFile ( error );
		}
		else
		{
			errorAnz += pKlassenName->adaptShapeFileObjekt ( pShapeFileObjekt, pGmlObjekte );
		}
	}

	return errorAnz;
}

///////////////////////////////////////////////////////////////////////////////
//  KlassenName														                                	 //
//  Modelliert die Abbildung eines Shapefiles auf eine XPlanGML-Klasse       //
///////////////////////////////////////////////////////////////////////////////
KlassenName::KlassenName (  ShapeFileNamenKonvertierung * pParantP, 
						    string & nameAktuellP, string & nameStandardP, int bereichNummerP, int rechtlicheBedeutungP )
{
	pParant			        = pParantP;
	nameAktuell         = nameAktuellP; 
	nameStandard        = nameStandardP;
	bereichNummer       = bereichNummerP;
  rechtlicheBedeutung = rechtlicheBedeutungP;  
}

KlassenName::~KlassenName ()
{
	std::multimap<std::string,AttributName*>::iterator iter;

	for ( iter = attributMap.begin(); iter != attributMap.end(); iter++ )
		delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen der Abbildungsvorschriften für ein Shapefile-Attribut				 //
///////////////////////////////////////////////////////////////////////////////
void KlassenName::addAttributName ( string shpAttributNameP,
				string attributNameP, int indexP, ATTRIBUT_TYP attributTypP, 
				string generAttributNameP,
				string enumerationIdP, string defaultP, 
				string subAttributNameP, ATTRIBUT_TYP subAttributTypP, Feature * pComplexAttribut  )
{
	AttributName * pAttrName = new AttributName;
  Feature   * pComplexAttributCopy;

	pAttrName->nameAktuell			          = shpAttributNameP;
	pAttrName->nameStandard			          = attributNameP;
	pAttrName->setIndex (indexP );
	pAttrName->typ					              = attributTypP;
	pAttrName->generAttributName	        = generAttributNameP;
	pAttrName->enumerationId		          = enumerationIdP;
	pAttrName->defaultWert			          = defaultP;
	pAttrName->subAttributName            = subAttributNameP;
	pAttrName->subAttributTyp		          = subAttributTypP;

  if ( pComplexAttribut != NULL ) 
    pComplexAttributCopy = new Feature ( pComplexAttribut, pComplexAttribut->getGmlId() );
  else
    pComplexAttributCopy = NULL;

  pAttrName->defaultWertKomplexAttribut = pComplexAttributCopy;

	attributMap.insert ( multimap<string,AttributName*>::value_type ( shpAttributNameP, pAttrName ) );
	standardMap.insert ( multimap<string,AttributName*>::value_type ( attributNameP, pAttrName ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Attribut-Objekte, die zu einem Shapefile zugeordnet sind		 //
///////////////////////////////////////////////////////////////////////////////
size_t KlassenName::getAttributObjekte ( string & shpAttributNameP, vector<AttributName*> & vAttrObjekteP )
{
	multimap<string,AttributName*>::iterator iter;

	for ( iter = attributMap.lower_bound ( shpAttributNameP ); iter != attributMap.upper_bound ( shpAttributNameP ); ++iter )
		vAttrObjekteP.push_back ( iter->second );

	return vAttrObjekteP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Attribut-Objekte, die zu einem XPlanGML-Attribut gehören 	 //
///////////////////////////////////////////////////////////////////////////////
size_t KlassenName::getAttributObjekteFromStandardNamen ( std::string & standardNameP,
								std::vector<AttributName*> & vAttrObjekteP )
{
	multimap<string,AttributName*>::iterator iter;

	for ( iter = standardMap.lower_bound ( standardNameP ); iter != standardMap.upper_bound ( standardNameP ); ++iter )
		vAttrObjekteP.push_back ( iter->second );

	return vAttrObjekteP.size();
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die Information, ob zu einem XPlanGML-Attribut mindestens ein		 //
//  Attribut-Objekt gehört																									 //
///////////////////////////////////////////////////////////////////////////////
bool KlassenName::existAttributObjektToStandardName (  std::string & standardNameP )
{
	size_t                anz;
	vector<AttributName*> vAttrObjekte;

	anz = getAttributObjekteFromStandardNamen ( standardNameP, vAttrObjekte );
	if ( anz> 0 ) 
		return true;
	else 
		return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Attribut-Objekte					                      						 //
///////////////////////////////////////////////////////////////////////////////
size_t KlassenName::getBPlanAttributObjekte ( std::vector<AttributName*> &vAttributeP )
{
	std::multimap<std::string,AttributName*>::iterator iter;

	for ( iter = attributMap.begin(); iter != attributMap.end(); iter++ )
		vAttributeP.push_back ( iter->second );

	return attributMap.size();

}

///////////////////////////////////////////////////////////////////////////////
//  Überträgt die Informationen in die Klasse ShapeFileObjekt 				       //
///////////////////////////////////////////////////////////////////////////////
int KlassenName::adaptShapeFileObjekt( ShapeFileObjekt * pShapeFileObjektP, GmlSchema * pGmlObjekteP )
{
	multimap<string,AttributName*>::iterator iter;
	vector<ShapeFileAttributWert*>           shapeFileAttributWerte;
	vector<GmlAttribut*>                     gmlAttribute;
	size_t			          i, anz;
	string				        attributWert;
	string				        defaultWert;
  string                externalCodeListPfad;
	string			          gmlWert;
	string                error;
	GmlEnumerationItem  * pEnumItem;
	GmlAttribut			    * pGmlAttribut;
	int					          errorAnz = 0;

	GmlKlasse * pGmlKlasse = pGmlObjekteP->getGmlKlasseFromElementName ( nameStandard );
	if ( pGmlKlasse == NULL ) 
	{
		error = "Fehler in Konversions-Datei, Shape-Datei: ";
		error += nameAktuell;
		pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

		error = "Zugeordnete GML-Klasse existiert nicht: ";
		error += nameStandard;
		pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

		error = "\n";
		pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

		errorAnz++;
		return errorAnz;
	}

	pShapeFileObjektP->setBereichNummer ( bereichNummer );
	pShapeFileObjektP->setRechtlichebedeutung( rechtlicheBedeutung );
  pShapeFileObjektP->setGmlKlasse ( pGmlKlasse );

	for ( iter = attributMap.begin(); iter != attributMap.end(); iter++ )
	{
		AttributName * pAttributName = iter->second;		
		GmlAttribut * pGmlAttribut = pGmlKlasse->getGmlAttribut ( pAttributName->nameStandard );

    if ( pAttributName->nameStandard == "xplan:__planId" )
		{
			pShapeFileObjektP->setNameAttributPlanId ( pAttributName->nameAktuell );
		}
		else
		if ( pGmlAttribut == NULL )
		{
			error = "Fehler in Konversions-Datei, Shape-Datei: ";
			error += nameAktuell;
			pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

			error = "GML-Attribut existiert nicht: ";
			error += pAttributName->nameStandard;
			pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

			error = "\n";
			pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );
			errorAnz++;
		}
		else
		{
			ShapeFileAttribut * pShapeFileAttribut = 
				pShapeFileObjektP->getShapeFileAttribut ( pAttributName->nameAktuell );
			if ( pShapeFileAttribut != NULL )
			{
				pShapeFileAttribut->addGmlAttribut ( pGmlAttribut );

				if ( pAttributName->typ == _REFERENZ ||  pAttributName->typ == _DATA_TYPE )
				{
          if ( pAttributName->generAttributName != "" )
            pShapeFileAttribut->setGenerAttributName ( pAttributName->generAttributName );

					if ( pAttributName->subAttributName != "" )
					{
						pShapeFileAttribut->setRefAttributName ( pAttributName->subAttributName );
						pShapeFileAttribut->setRefAttributTyp (  pAttributName->subAttributTyp );

						if ( pAttributName->subAttributTyp == _ENUMERATION ||
							pAttributName->subAttributTyp == _EXTERNAL_CODE_LIST )
						{	
							GmlEnumeration * pGmlEnum;
              pGmlEnum = pGmlObjekteP->getGmlEnumeration ( pAttributName->enumerationId );
			
							if ( pGmlEnum == NULL ) 
							{
                if ( pGmlObjekteP->getGmlTyp() == XPLANGML_2_0 ||  pGmlObjekteP->getGmlTyp() == XPLANGML_3_0 )
                {
								  error = "Fehler in Konversions-Datei, Shape-Datei: ";
								  error += nameAktuell;
								  pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

								  error = "Ungültige GML-Enumeration: ";
								  error += pAttributName->enumerationId ;
								  pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

								  error = "\n";
								  pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );
								  errorAnz++;
                }
                else
                {
                  pGmlEnum = new GmlEnumeration ( pAttributName->enumerationId, true );
                  externalCodeListPfad =  pParant->getEnumeration ( pAttributName->enumerationId )->getDictionary();
                  pGmlEnum->setCodeListDictionary(externalCodeListPfad  );
                  pGmlObjekteP->addGmlEnumeration( pGmlEnum );
                }
							}

							Enumeration * pEnum = pParant->getEnumeration ( pAttributName->enumerationId );
							if ( pEnum == NULL ) 
							{
								error = "Fehler in Konversions-Datei, Shape-Datei: ";
								error += nameAktuell;
								pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

								error = "Verwendete Enumeration-ID nicht in Konversions-Datei spezifiziert: ";
								error += pAttributName->enumerationId ;
								pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

								error = "\n";
								pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );
								errorAnz++;
							}

							if ( pEnum != NULL &&  pGmlEnum != NULL )
							{
								shapeFileAttributWerte.clear();
								anz = pShapeFileAttribut->getShapeFileAttributWerte (  shapeFileAttributWerte );
								for ( i = 0; i < anz; i++ )
								{
									ShapeFileAttributWert * pWert = shapeFileAttributWerte[i];
									attributWert = pWert->getAttributWert();
									gmlWert      = pEnum->getEnumerationItem ( attributWert );
									pEnumItem    = pGmlEnum->getEnumerationItem ( gmlWert );
									if ( pEnumItem != NULL ) pWert->setEnumerationItem ( pGmlEnum, pEnumItem );
								}
							}
						}

					}
				}

				if ( pAttributName->typ == _ENUMERATION ||
					 pAttributName->typ == _EXTERNAL_CODE_LIST )
				{	
					GmlEnumeration * pGmlEnum;
          pGmlEnum = pGmlObjekteP->getGmlEnumeration ( pAttributName->enumerationId );

					if ( pGmlEnum == NULL ) 
					{
            if ( pGmlObjekteP->getGmlTyp() == XPLANGML_2_0 ||  pGmlObjekteP->getGmlTyp() == XPLANGML_3_0 )
            {
  						error = "Fehler in Konversions-Datei, Shape-Datei: ";
	  					error += nameAktuell;
		  				pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

						  error = "Ungültige GML-Enumeration: ";
						  error += pAttributName->enumerationId ;
						  pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

						  error = "\n";
						  pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );
						  errorAnz++;
            }
            else
            {
              pGmlEnum = new GmlEnumeration ( pAttributName->enumerationId, true );
              Enumeration * pEnumeration = pParant->getEnumeration ( pAttributName->enumerationId );
              if ( pEnumeration != NULL )
              {
                externalCodeListPfad =  pEnumeration->getDictionary();
                pGmlEnum->setCodeListDictionary(externalCodeListPfad  );
              }
              pGmlObjekteP->addGmlEnumeration( pGmlEnum );
            }
					}

					Enumeration * pEnum = pParant->getEnumeration ( pAttributName->enumerationId );
					if ( pEnum == NULL ) 
					{
						error = "Fehler in Konversions-Datei, Shape-Datei: ";
						error += nameAktuell;
						pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

						error = "Verwendete Enumeration-ID nicht in Konversions-Datei spezifiziert: ";
						error += pAttributName->enumerationId ;
						pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );

						error = "\n";
						pParant->getXPlanGML()->getLogfile()->writeLogFile ( error );
						errorAnz++;
					}

					if ( pEnum != NULL &&  pGmlEnum != NULL )
					{
            pGmlEnum->setCodeListDictionary( pEnum->getDictionary() );
						shapeFileAttributWerte.clear();
						anz = pShapeFileAttribut->getShapeFileAttributWerte (  shapeFileAttributWerte );
						for ( i = 0; i < anz; i++ )
						{
							ShapeFileAttributWert * pWert = shapeFileAttributWerte[i];
							attributWert = pWert->getAttributWert();
							gmlWert      = pEnum->getEnumerationItem ( attributWert );
              if ( gmlWert == "Error" ) gmlWert = "";
							pEnumItem    = pGmlEnum->getEnumerationItem ( gmlWert );
              if ( pEnumItem == NULL )
              {
                pEnumItem = new GmlEnumerationItem( gmlWert );
                pGmlEnum->addGmlEnumerationItem ( pEnumItem );
              }
							pWert->setEnumerationItem ( pGmlEnum, pEnumItem );
						}
					}
				}
			}

			if ( pAttributName->defaultWert != "" )
			{
				StandardAttribut * pStandardAttribut = new StandardAttribut ( pGmlAttribut );
				pStandardAttribut->setDefaultWert ( pAttributName->defaultWert );
				pShapeFileObjektP->addStandardAttribut ( pStandardAttribut );
			}

      if ( pAttributName->defaultWertKomplexAttribut != NULL )
      {
        StandardAttribut * pStandardAttribut = new StandardAttribut ( pGmlAttribut );
        pStandardAttribut->setComplexDefaultWert( pAttributName->defaultWertKomplexAttribut );
        pShapeFileObjektP->addStandardAttribut ( pStandardAttribut );
      }
		}
	}

	anz = pGmlKlasse->getGmlAttribute ( gmlAttribute );
	for ( i = 0; i < anz; i++ )
	{
		pGmlAttribut = gmlAttribute[i];
		defaultWert = pGmlAttribut->getDefaultWert();
		if ( defaultWert  != "" )
		{
			StandardAttribut * pStandardAttribut = pShapeFileObjektP->getStandardAttribut( pGmlAttribut );
			if ( pStandardAttribut == NULL )
			{
				pStandardAttribut = new StandardAttribut ( pGmlAttribut );
				pStandardAttribut->setDefaultWert ( pGmlAttribut->getDefaultWert() );
				pShapeFileObjektP->addStandardAttribut ( pStandardAttribut );
			}
		}
	}

	return errorAnz;
}

///////////////////////////////////////////////////////////////////////////////
//  AttributName																														 //
//  Modelliert die Abbildung von Shapefile-Attributen auf XPlanGML-Attribute //
//  (nameAktuell != "", defaultWert == "") bzw. die Belegung von XPlanGML-   //
//  Attributen mit Standardwerten (nameAktuell = "", default != "")					 //
///////////////////////////////////////////////////////////////////////////////
AttributName::AttributName()
{
  typ                        = _SONST;
  subAttributTyp             = _SONST;
  defaultWertKomplexAttribut = NULL;
	index                      = 0;
}

AttributName:: ~AttributName()
{
  if ( defaultWertKomplexAttribut != NULL ) delete defaultWertKomplexAttribut;
}


///////////////////////////////////////////////////////////////////////////////
//  Enumeration																															 //
//  Unterstützt die Transformation von Shapefile Attributwerten auf		       //
//  XPlanGML Enumerations- oder Codelist-Einträge								             //
///////////////////////////////////////////////////////////////////////////////
Enumeration::Enumeration ( std::string & enumerationIdP )
{
	enumerationId = enumerationIdP;
}

Enumeration::~ Enumeration()
{

}

/////////////////////////////////////////////////////////////////////////////////
//  Hinzufügen einer Zuordnung Shapefile-Attributwert --> XPlanGML-Attributwert//
/////////////////////////////////////////////////////////////////////////////////
void Enumeration::addEnumerationItem ( std::string & nameAktuellP, std::string & nameStandardP )
{
	enumerationItems[nameAktuellP] = nameStandardP;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den XPlanGML Attributwert, der einem Shapefile-Attributwert    	 //
//  zugeordnet ist																													 //
///////////////////////////////////////////////////////////////////////////////
std::string Enumeration::getEnumerationItem ( std::string & shpEnumItemNameP )
{
	std::map<std::string,std::string>::iterator iter;

	iter = enumerationItems.find ( shpEnumItemNameP );
	if ( iter != enumerationItems.end() ) return iter->second;
	else								  return "Error";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Shapefile-Attributwerte und die zugeordneten XPlanGML   		 //
//  Attributwerte																														 //
///////////////////////////////////////////////////////////////////////////////
size_t Enumeration::getEnumerationItems ( vector<string> & namenAktuellP, vector<string> & namenStandardP )
{
	map<string,string>::iterator iter;

	for ( iter = enumerationItems.begin(); iter != enumerationItems.end(); iter++ )
	{
		namenAktuellP.push_back ( iter->first );
		namenStandardP.push_back ( iter->second );
	}
	return enumerationItems.size();
}

///////////////////////////////////////////////////////////////////////////////
// PlanAttribute																														 //
//  Während der Transformation Shapefile --> XPlanGML festgelegte					   //
//  Attributwerte des gesamten Plans																				 //
///////////////////////////////////////////////////////////////////////////////
PlanAttribute::PlanAttribute ()
{

}

PlanAttribute::PlanAttribute ( PlanAttribute * pPlanAttributeOld )
{
	string       attributName;
	string       attributWert;
	unsigned int i;
	double       d;
	bool         b;
  Feature * pAttr;

	std::multimap<std::string,std::string>::iterator      iterStringAttribute;
	std::multimap<std::string,int>::iterator		          iterIntegerAttribute;
	std::multimap<std::string,double>::iterator	          iterDoubleAttribute;
	std::map<std::string,bool>::iterator	              	iterBoolAttribute;
	std::multimap<std::string,std::string>::iterator      iterDateAttribute;
	std::multimap<std::string,std::string>::iterator      iterUrlAttribute;
  std::multimap<std::string,Feature*>::iterator iterKomplexAttribute;
  

	for ( iterStringAttribute = pPlanAttributeOld->stringAttributePlan.begin(); 
		  iterStringAttribute != pPlanAttributeOld->stringAttributePlan.end(); 
		  iterStringAttribute++ )
	{
		attributName = iterStringAttribute->first;
		attributWert = iterStringAttribute->second;
		stringAttributePlan.insert
			( multimap<string,string>::value_type ( attributName, attributWert ) );
	}

	for ( iterIntegerAttribute = pPlanAttributeOld->integerAttributePlan.begin(); 
		  iterIntegerAttribute != pPlanAttributeOld->integerAttributePlan.end(); 
		  iterIntegerAttribute++ )
	{
		attributName = iterIntegerAttribute->first;
		i			       = iterIntegerAttribute->second;
		integerAttributePlan.insert
			( multimap<string,int>::value_type ( attributName, i ) );
	}

	for ( iterDoubleAttribute = pPlanAttributeOld->doubleAttributePlan.begin(); 
		  iterDoubleAttribute != pPlanAttributeOld->doubleAttributePlan.end(); 
		  iterDoubleAttribute++ )
	{
		attributName = iterDoubleAttribute->first;
		d			       = iterDoubleAttribute->second;
		doubleAttributePlan.insert
			( multimap<string,double>::value_type ( attributName, d ) );
	}

	for ( iterBoolAttribute = pPlanAttributeOld->boolAttributePlan.begin(); 
		  iterBoolAttribute != pPlanAttributeOld->boolAttributePlan.end(); 
		  iterBoolAttribute++ )
	{
		attributName = iterBoolAttribute->first;
		b			 = iterBoolAttribute->second;
		boolAttributePlan.insert
			( map<string,bool>::value_type ( attributName, b ) );
	}

	for ( iterDateAttribute = pPlanAttributeOld->dateAttributePlan.begin(); 
		  iterDateAttribute != pPlanAttributeOld->dateAttributePlan.end(); 
		  iterDateAttribute++ )
	{
		attributName = iterDateAttribute->first;
		attributWert = iterDateAttribute->second;
		dateAttributePlan.insert
			( multimap<string,string>::value_type ( attributName, attributWert ) );
	}

	for ( iterUrlAttribute = pPlanAttributeOld->urlAttributePlan.begin(); 
		    iterUrlAttribute != pPlanAttributeOld->urlAttributePlan.end(); 
		    iterUrlAttribute++ )
	{
		attributName = iterUrlAttribute->first;
		attributWert = iterUrlAttribute->second;
		urlAttributePlan.insert
			( multimap<string,string>::value_type ( attributName, attributWert ) );
	}

	for ( iterKomplexAttribute = pPlanAttributeOld->komplexAttributePlan.begin(); 
		    iterKomplexAttribute != pPlanAttributeOld->komplexAttributePlan.end(); 
		    iterKomplexAttribute++ )
	{
		attributName = iterKomplexAttribute->first;
		pAttr        = iterKomplexAttribute->second;
		komplexAttributePlan.insert
			( multimap<string,Feature*>::value_type ( attributName, new Feature ( pAttr, "" ) ) );
	}

	for ( i = 0; i < pPlanAttributeOld->textAbschnitte.size(); i++ )
	{
		TextAbschnitt * pTextAbschnitt = pPlanAttributeOld->textAbschnitte[i];
		TextAbschnitt * pTextAbschnittNeu = new TextAbschnitt ( pTextAbschnitt );
		textAbschnitte.push_back ( pTextAbschnittNeu );
	}

	for ( i = 0; i < pPlanAttributeOld->bereiche.size(); i++ )
	{
		Bereich * pBereich = pPlanAttributeOld->bereiche[i];
		Bereich * pBereichNeu = new Bereich ( pBereich );
		bereiche.push_back ( pBereichNeu );
	}
}


PlanAttribute::~PlanAttribute()
{
	unsigned int                           i;
  multimap<string,Feature*>::iterator iterKomplexAttribute;

	for ( i = 0; i < bereiche.size(); i++ ) delete bereiche[i];
	for ( i = 0; i < textAbschnitte.size(); i++ ) delete textAbschnitte[i];
	for ( iterKomplexAttribute = komplexAttributePlan.begin(); 
		    iterKomplexAttribute != komplexAttributePlan.end(); 
		    iterKomplexAttribute++ )
        delete iterKomplexAttribute->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein  String-Attribut des Gesamtplans															 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::addStringAttribut ( std::string attributNameP, std::string attributWertP )
{
	stringAttributePlan.insert ( multimap<string,string>::value_type ( attributNameP, attributWertP ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein  Integer-Attribut des Gesamtplans															 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::addIntegerAttribut ( std::string attributNameP, int attributWertP )
{
  integerAttributePlan.insert ( multimap<string,int>::value_type ( attributNameP, attributWertP ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein  Double-Attribut des Gesamtplans															 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::addDoubleAttribut ( std::string attributNameP, double attributWertP )
{
	doubleAttributePlan.insert ( multimap<string,double>::value_type ( attributNameP, attributWertP ) );
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein  Bool-Attribut des Gesamtplans															   //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::setBoolAttribut ( std::string attributNameP, bool attributWertP )
{
	boolAttributePlan[attributNameP] = attributWertP;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein  Datum-Attribut des Gesamtplans															   //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::addDateAttribut ( std::string attributNameP, std::string attributWertP )
{
	dateAttributePlan.insert ( multimap<string,string>::value_type ( attributNameP, attributWertP ) );

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein  URL-Attribut des Gesamtplans															 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::addURLAttribut ( std::string attributNameP, std::string attributWertP )
{
	urlAttributePlan.insert ( multimap<string,string>::value_type ( attributNameP, attributWertP ) );

}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein  Komplexattribut des Gesamtplans															 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::addKomplexAttribut ( std::string attributNameP, Feature * pAttr )
{
  Feature * pAttrCopy = new Feature ( pAttr, pAttr->getGmlId() );
	komplexAttributePlan.insert ( multimap<string,Feature*>::value_type ( attributNameP, pAttrCopy ) );
}


///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle String-Attribute mit dem übergebenen Namen           			 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeStringAttribut ( std::string attributNameP )
{
	stringAttributePlan.erase ( attributNameP );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt ein String-Attribut, das einen bestimmten Namen und Wert hat		 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeStringAttribut ( std::string attributNameP, std::string attributWertP )
{
  multimap<string,string>::iterator iter;

  for ( iter = stringAttributePlan.lower_bound ( attributNameP );
        iter != stringAttributePlan.upper_bound ( attributNameP );
        iter++ )
  {
    if ( iter->second == attributWertP )
    {
      stringAttributePlan.erase ( iter );
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Integer-Attribute mit einem bestimmten Namen        			 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeIntegerAttribut ( std::string attributNameP )
{
	integerAttributePlan.erase ( attributNameP );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Integer-Attribute mit einem bestimmten Namen und Wert			 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeIntegerAttribut ( std::string attributNameP, int attributWertP )
{
  multimap<string,int>::iterator iter;

  for ( iter = integerAttributePlan.lower_bound ( attributNameP );
        iter != integerAttributePlan.upper_bound ( attributNameP );
        iter++ )
  {
    if ( iter->second == attributWertP )
    {
      integerAttributePlan.erase ( iter );
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Double-Attribute mit einem bestimmten Namen        			   //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeDoubleAttribut ( std::string attributNameP )
{
	doubleAttributePlan.erase ( attributNameP );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Double-Attribute mit einem bestimmten Namen und Wert		   //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeDoubleAttribut ( std::string attributNameP, double attributWertP )
{
  multimap<string,double>::iterator iter;

  for ( iter = doubleAttributePlan.lower_bound ( attributNameP );
        iter != doubleAttributePlan.upper_bound ( attributNameP );
        iter++ )
  {
    if ( iter->second == attributWertP )
    {
      doubleAttributePlan.erase ( iter );
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Boolean-Attribute mit einem bestimmten Namen        		   //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeBoolAttribut ( std::string attributNameP )
{
	boolAttributePlan.erase ( attributNameP );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Datum-Attribute mit einem bestimmten Namen        			   //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeDateAttribut ( std::string attributNameP )
{
	dateAttributePlan.erase ( attributNameP );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Datum-Attribute mit einem bestimmten Namen und Wert			   //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeDateAttribut ( string attributNameP, string attributWertP )
{
  multimap<string,string>::iterator iter;

  for ( iter = dateAttributePlan.lower_bound ( attributNameP );
        iter != dateAttributePlan.upper_bound ( attributNameP );
        iter++ )
  {
    if ( iter->second == attributWertP )
    {
      dateAttributePlan.erase ( iter );
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle URL-Attribute mit einem bestimmten Namen        			     //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeURLAttribut ( std::string attributNameP )
{
	urlAttributePlan.erase ( attributNameP );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle URL-Attribute mit einem bestimmten Namen und Wert			     //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeURLAttribut ( string attributNameP, string attributWertP )
{
  multimap<string,string>::iterator iter;

  for ( iter = urlAttributePlan.lower_bound ( attributNameP );
        iter != urlAttributePlan.upper_bound ( attributNameP );
        iter++ )
  {
    if ( iter->second == attributWertP )
    {
      urlAttributePlan.erase ( iter );
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt alle Komplex-Attribute mit einem bestimmten Namen     			     //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::removeKomplexAttribut ( std::string attributNameP )
{
  multimap<std::string,Feature*>::iterator iter;

  for ( iter = komplexAttributePlan.lower_bound( attributNameP );
        iter != komplexAttributePlan.upper_bound ( attributNameP);
        iter++)
        delete iter->second;

	komplexAttributePlan.erase ( attributNameP );
}
///////////////////////////////////////////////////////////////////////////////
//  Zuweisungs-Operator										                          				 //
///////////////////////////////////////////////////////////////////////////////
PlanAttribute PlanAttribute::operator= ( PlanAttribute & planAttributeP )
{
	string        attributName;
	string        attributWert;
	unsigned int  i;
	double        d;
	bool          b;
  Feature * pAttr;

	std::multimap<std::string,std::string>::iterator iterStringAttribute;
	std::multimap<std::string,int>::iterator		     iterIntegerAttribute;
	std::multimap<std::string,double>::iterator	     iterDoubleAttribute;
	std::map<std::string,bool>::iterator	           iterBoolAttribute;
	std::multimap<std::string,std::string>::iterator iterDateAttribute;
	std::multimap<std::string,std::string>::iterator iterUrlAttribute;
  std::multimap<string,Feature*>::iterator      iterKomplexAttribute;

	clear();

	for ( iterStringAttribute = planAttributeP.stringAttributePlan.begin(); 
		    iterStringAttribute != planAttributeP.stringAttributePlan.end(); 
		    iterStringAttribute++ )
	{
		attributName = iterStringAttribute->first;
		attributWert = iterStringAttribute->second;
		stringAttributePlan.insert
			( multimap<string,string>::value_type ( attributName, attributWert ) );
	}

	for ( iterIntegerAttribute = planAttributeP.integerAttributePlan.begin(); 
		    iterIntegerAttribute != planAttributeP.integerAttributePlan.end(); 
		    iterIntegerAttribute++ )
	{
		attributName = iterIntegerAttribute->first;
		i			       = iterIntegerAttribute->second;
		integerAttributePlan.insert
			( multimap<string,int>::value_type ( attributName, i ) );
	}

	for ( iterDoubleAttribute = planAttributeP.doubleAttributePlan.begin(); 
		    iterDoubleAttribute != planAttributeP.doubleAttributePlan.end(); 
		    iterDoubleAttribute++ )
	{
		attributName = iterDoubleAttribute->first;
		d		      	 = iterDoubleAttribute->second;
		doubleAttributePlan.insert
			( multimap<string,double>::value_type ( attributName, d ) );
	}

	for ( iterBoolAttribute = planAttributeP.boolAttributePlan.begin(); 
		    iterBoolAttribute != planAttributeP.boolAttributePlan.end(); 
		    iterBoolAttribute++ )
	{
		attributName = iterBoolAttribute->first;
		b			 = iterBoolAttribute->second;
		boolAttributePlan.insert
			( multimap<string,bool>::value_type ( attributName, b ) );
	}

	for ( iterDateAttribute = planAttributeP.dateAttributePlan.begin(); 
		    iterDateAttribute != planAttributeP.dateAttributePlan.end(); 
		    iterDateAttribute++ )
	{
		attributName = iterDateAttribute->first;
		attributWert = iterDateAttribute->second;
		dateAttributePlan.insert
			( multimap<string,string>::value_type ( attributName, attributWert ) );
	}

	for ( iterUrlAttribute = planAttributeP.urlAttributePlan.begin(); 
		    iterUrlAttribute != planAttributeP.urlAttributePlan.end(); 
		    iterUrlAttribute++ )
	{
		attributName = iterUrlAttribute->first;
		attributWert = iterUrlAttribute->second;
		urlAttributePlan.insert
			( multimap<string,string>::value_type ( attributName, attributWert ) );
	}

	for ( iterKomplexAttribute = planAttributeP.komplexAttributePlan.begin(); 
		    iterKomplexAttribute != planAttributeP.komplexAttributePlan.end(); 
		    iterKomplexAttribute++ )
	{
		attributName = iterKomplexAttribute->first;
		pAttr        = iterKomplexAttribute->second;
		komplexAttributePlan.insert
			( multimap<string,Feature*>::value_type ( attributName, new Feature ( pAttr, "" ) ) );
	}

	for ( i = 0; i < planAttributeP.textAbschnitte.size(); i++ )
	{
		TextAbschnitt * pTextAbschnitt = planAttributeP.textAbschnitte[i];
		TextAbschnitt * pTextAbschnittNeu = new TextAbschnitt ( pTextAbschnitt );
		textAbschnitte.push_back ( pTextAbschnittNeu );
	}

	for ( i = 0; i < planAttributeP.bereiche.size(); i++ )
	{
		Bereich * pBereich = planAttributeP.bereiche[i];
		Bereich * pBereichNeu = new Bereich ( pBereich );
		bereiche.push_back ( pBereichNeu );
	}

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten Wert eines string-Attributs               						 //
///////////////////////////////////////////////////////////////////////////////
bool PlanAttribute::getStringAttributWert ( string attributName, string & attributWert )
{
	multimap<string,string>::iterator iter;

	iter = stringAttributePlan.find ( attributName );
	if (  iter != stringAttributePlan.end()  )
	{
		attributWert = iter->second;
		return true;
	}
	else return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert slle string-Attribut Werte						               						 //
///////////////////////////////////////////////////////////////////////////////
size_t PlanAttribute::getStringAttributWerte ( vector<string> & vAttributName, vector<string> & vStringP )
{
	multimap<string,string>::iterator iter;
	string						                str;

	for ( iter = stringAttributePlan.begin(); iter !=  stringAttributePlan.end(); iter ++ )
	{
		str = iter->first;
		vAttributName.push_back ( str );

		str = iter->second;
		vStringP.push_back ( str );
	}

	return vAttributName.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten Wert eines Integer-Attributs				                   //
///////////////////////////////////////////////////////////////////////////////
bool PlanAttribute::getIntegerAttributWert ( std::string attributName, int & attributWert )
{
	multimap<string,int>::iterator iter;

	iter = integerAttributePlan.find ( attributName );
	if ( iter != integerAttributePlan.end() )
	{
		attributWert = iter->second;
		return true;
	}
	else return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Integer-Attribut Werte										                   //
///////////////////////////////////////////////////////////////////////////////
size_t PlanAttribute::getIntegerAttributWerte ( vector<string> & vAttributName, vector<int> & vIntP )
{
	multimap<string,int>::iterator iter;
	string			 		  str;
	int						  wert;

	for ( iter = integerAttributePlan.begin(); iter != integerAttributePlan.end(); iter ++ )
	{
		str = iter->first;
		vAttributName.push_back ( str );

		wert = iter->second;
		vIntP.push_back ( wert );
	}

	return vAttributName.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten Wert eines double-Attributs		                    	 //
///////////////////////////////////////////////////////////////////////////////
bool PlanAttribute::getDoubleAttributWert ( std::string attributName, double & attributWert )
{
	multimap<string,double>::iterator iter;

	iter = doubleAttributePlan.find ( attributName );
	if ( iter != doubleAttributePlan.end() )
	{
		attributWert = iter->second;
		return true;
	}
	else return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle double-Attribut Werte								                    	 //
///////////////////////////////////////////////////////////////////////////////
size_t PlanAttribute::getDoubleAttributWerte ( vector<string> & vAttributName, vector<double> & vDoubleP )
{
	multimap<string,double>::iterator iter;
	string			 			 str;
	double						 wert;

	for ( iter = doubleAttributePlan.begin(); iter != doubleAttributePlan.end(); iter ++ )
	{
		str = iter->first;
		vAttributName.push_back ( str );

		wert = iter->second;
		vDoubleP.push_back ( wert );
	}

	return vAttributName.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Wert eines bool-Attributs                    								 //
///////////////////////////////////////////////////////////////////////////////
bool PlanAttribute::getBoolAttributWert ( std::string attributName, bool & attributWert )
{
	std::map<std::string,bool>::iterator iter;

	iter = boolAttributePlan.find ( attributName );
	if ( iter != boolAttributePlan.end() )
	{
		attributWert = iter->second;
		return true;
	}
	else return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle bool-Attribute und ihre Werte               								 //
///////////////////////////////////////////////////////////////////////////////
size_t PlanAttribute::getBoolAttributWerte ( vector<string> & vAttributName, vector<bool> & vBoolP )
{
	map<string,bool>::iterator iter;
	string		 			   str;
	bool					   wert;

	for ( iter = boolAttributePlan.begin(); iter != boolAttributePlan.end(); iter ++ )
	{
		str = iter->first;
		vAttributName.push_back ( str );

		wert = iter->second;
		vBoolP.push_back ( wert );
	}

	return vAttributName.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten Wert eines Datum-Attributs                  					 //
///////////////////////////////////////////////////////////////////////////////
bool PlanAttribute::getDateAttributWert ( string attributName, string & attributWert )
{
	multimap<string,string>::iterator iter;

	iter = dateAttributePlan.find ( attributName );
	if (  iter != dateAttributePlan.end()  )
	{
		attributWert = iter->second;
		return true;
	}
	else return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Datum-Attribut Werte						                  					 //
///////////////////////////////////////////////////////////////////////////////
size_t PlanAttribute::getDateAttributWerte ( vector<string> & vAttributName, vector<string> & vStringP )
{
	multimap<string,string>::iterator iter;
	string						 str;

	for ( iter = dateAttributePlan.begin(); iter !=  dateAttributePlan.end(); iter ++ )
	{
		str = iter->first;
		vAttributName.push_back ( str );

		str = iter->second;
		vStringP.push_back ( str );
	}

	return vAttributName.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten Wert eines URL-Attributs			                    		 //
///////////////////////////////////////////////////////////////////////////////
bool PlanAttribute::getURLAttributWert ( string attributName, string & attributWert )
{
	multimap<string,string>::iterator iter;

	iter = urlAttributePlan.find ( attributName );
	if (  iter != urlAttributePlan.end()  )
	{
		attributWert = iter->second;
		return true;
	}
	else return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle URL-Attribut Werte									                    		 //
///////////////////////////////////////////////////////////////////////////////
size_t PlanAttribute::getURLAttributWerte ( vector<string> & vAttributName, vector<string> & vStringP )
{
	multimap<string,string>::iterator iter;
	string						 str;

	for ( iter = urlAttributePlan.begin(); iter !=  urlAttributePlan.end(); iter ++ )
	{
		str = iter->first;
		vAttributName.push_back ( str );

		str = iter->second;
		vStringP.push_back ( str );
	}

	return vAttributName.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den ersten Wert eines Komplex-Attributs	                    		 //
///////////////////////////////////////////////////////////////////////////////
Feature * PlanAttribute::getKomplexAttributWert ( string attributName )
{
	multimap<string,Feature*>::iterator iter;

	iter = komplexAttributePlan.find ( attributName );
	if (  iter != komplexAttributePlan.end()  )
		return iter->second;
	else 
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Komplex-Attribut Werte							                    		 //
///////////////////////////////////////////////////////////////////////////////
size_t PlanAttribute::getKomplexAttributWerte ( string & attributName, 
                                             vector<Feature*> &vKomplexAttribute )
{
  multimap<string,Feature*>::iterator iter;
  Feature                           * pGenerAttribut;

  if ( attributName == "xplan:hatGenerStringAttribut"  || 
       attributName == "xplan:hatGenerIntegerAttribut" || 
       attributName == "xplan:hatGenerDoubleAttribut"  ||
       attributName == "xplan:hatGenerDateAttribut"    ||
       attributName == "xplan:hatGenerURLAttribut"   )
  {
    attributName = string ( "xplan:hatGenerAttribut" );

    for ( iter = komplexAttributePlan.lower_bound( "xplan:hatGenerAttribut" );
          iter != komplexAttributePlan.upper_bound("xplan:hatGenerAttribut"); iter++ )
    {
      pGenerAttribut = iter->second;
      if ( ( pGenerAttribut->getKlassenName() == "xplan:XP_StringAttribut" ) ||
           ( pGenerAttribut->getKlassenName() == "xplan:XP_IntegerAttribut"  ) ||
           ( pGenerAttribut->getKlassenName() == "xplan:XP_DoubleAttribut" ) ||
           ( pGenerAttribut->getKlassenName() == "xplan:XP_DateAttribut"  ) ||
           ( pGenerAttribut->getKlassenName() == "xplan:XP_URLAttribut"  ) )
      {
        vKomplexAttribute.push_back (iter->second);
      }
    }
  }
  else 
    for ( iter = komplexAttributePlan.lower_bound(attributName);
          iter != komplexAttributePlan.upper_bound(attributName); iter++ )
          vKomplexAttribute.push_back (iter->second);

  return vKomplexAttribute.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Komplex-Attribut Namen und Werte			                   		 //
///////////////////////////////////////////////////////////////////////////////
size_t PlanAttribute::getKomplexAttributWerte ( vector<string> & vAttributName, vector<Feature*> & vAttrP )
{
	multimap<string,Feature*>::iterator iter;
  string                                 str;
	Feature *      				             attr;

	for ( iter = komplexAttributePlan.begin(); iter !=  komplexAttributePlan.end(); iter ++ )
	{
		str = iter->first;
		vAttributName.push_back ( str );

		attr = iter->second;
		vAttrP.push_back ( attr );
	}

	return vAttributName.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt Text-Abschnitt hinzu							                        					 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::addTextAbschnitt ( string schluesselP, string wertP, string wertURLP, string planTypP  )
{
	TextAbschnitt * pTextAbschnitt = new TextAbschnitt;

	pTextAbschnitt->schluessel = schluesselP;
	pTextAbschnitt->text       = wertP;
	pTextAbschnitt->textURL    = wertURLP;
	pTextAbschnitt->planTyp    = planTypP;
	textAbschnitte.push_back ( pTextAbschnitt );
}

///////////////////////////////////////////////////////////////////////////////
//  Ändert einen Text-Abschnitt 					                        					 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::setTextAbschnitt ( unsigned int indexP, string schluesselP, string wertP, string wertURLP, string planTypP )
{
	if ( indexP < 0 || indexP >= textAbschnitte.size() ) return;

	TextAbschnitt * pTextAbschnitt = new TextAbschnitt;

	pTextAbschnitt->schluessel = schluesselP;
	pTextAbschnitt->text       = wertP;
	pTextAbschnitt->textURL    = wertURLP;
	pTextAbschnitt->planTyp    = planTypP;

	delete textAbschnitte[indexP];
	textAbschnitte[indexP] = pTextAbschnitt;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Text-Abschnitte											                  			 //
///////////////////////////////////////////////////////////////////////////////
size_t PlanAttribute::getTextAbschnitte ( vector<TextAbschnitt*> &textAbschnitteP )
{
	unsigned int i;

	for ( i = 0; i < textAbschnitte.size(); i++ )
	{
		textAbschnitteP.push_back ( textAbschnitte[i] );
	}
	return textAbschnitte.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen Text-Abschnitt			 						                  				 //
///////////////////////////////////////////////////////////////////////////////
bool PlanAttribute::getTextAbschnitt ( unsigned int indexP, string &schluesselP, string &wertP, 
					string &wertURLP, string &planTypP )
{
	if ( indexP < 0 || indexP >= textAbschnitte.size() ) return false;

	schluesselP = textAbschnitte[indexP]->schluessel;
	wertP		    = textAbschnitte[indexP]->text;
	wertURLP    = textAbschnitte[indexP]->textURL;
	planTypP    = textAbschnitte[indexP]->planTyp;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft ob ein Schlüssel schon vergeben wurde							             //
///////////////////////////////////////////////////////////////////////////////
bool PlanAttribute::schluesselTextAbschnittExist ( string &schluesselP )
{
	unsigned int i;

	for ( i = 0; i < textAbschnitte.size(); i++ )
	{
		if ( textAbschnitte[i]->schluessel == schluesselP ) return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht einen Text-Abschnitt													                     //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::deleteTextAbschnitt ( unsigned int indexP )
{
	unsigned int i;

	if ( indexP < 0 || indexP >= textAbschnitte.size() ) return;

	delete textAbschnitte[indexP];
	for ( i = indexP + 1; i < textAbschnitte.size(); i++ )
	{
		textAbschnitte[i-1] = textAbschnitte[i];
	}

	textAbschnitte.pop_back();
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht alle Attribute									                          				 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::clear()
{
	unsigned int i;

	stringAttributePlan.clear();
	integerAttributePlan.clear();
	doubleAttributePlan.clear();
	boolAttributePlan.clear();
	dateAttributePlan.clear();
	urlAttributePlan.clear();

	for ( i = 0; i < textAbschnitte.size(); i++ ) delete textAbschnitte[i];
	textAbschnitte.clear();

	for ( i = 0; i < bereiche.size(); i++ ) delete bereiche[i];
	bereiche.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Bereiche-Definition hinzu			                  							 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::addBereich ( string nameP, string bedeutungP, string bedeutungEnum, 
								 string geometrieDatei, int massstab )
{
	int nummer;
	
	Bereich * pBereich = new Bereich;
	nummer = (int)bereiche.size();

	pBereich->nummer		 = nummer;
	pBereich->name			 = nameP;
	pBereich->bedeutung      = bedeutungP;
	pBereich->bedeutungEnum  = bedeutungEnum;
	pBereich->geometrieDatei = geometrieDatei;
	pBereich->massstab		 = massstab;

	bereiche.push_back ( pBereich );
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Bereich-Definitions Objekt hinzu                  							 //
///////////////////////////////////////////////////////////////////////////////
void PlanAttribute::addBereich ( Bereich * pBereich )
{
	bereiche.push_back ( pBereich );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Bereiche		  		                            							 //
///////////////////////////////////////////////////////////////////////////////
size_t PlanAttribute::getBereiche ( std::vector<Bereich*> &bereicheP )
{
	unsigned int i;

	for ( i = 0; i < bereiche.size(); i++ )
		bereicheP.push_back ( bereiche[i] );

	return bereiche.size();
}

//////////////////////////////////////////////////////////////////////////////
//  Liefert einen Bereich									                          				 //
///////////////////////////////////////////////////////////////////////////////
Bereich * PlanAttribute::getBereich ( unsigned int index )
{
	if ( index < 0 || index >= bereiche.size() )
		return NULL;
	else
		return bereiche[index];
}


///////////////////////////////////////////////////////////////////////////////
//  TextAbschnitt																                             //
//  Modelliert einen Objekt XP_TextAbschnitt																 //
///////////////////////////////////////////////////////////////////////////////
TextAbschnitt::TextAbschnitt()
{
	schluessel = "";
	text	   = "";
	textURL    = "";
}

TextAbschnitt::TextAbschnitt ( TextAbschnitt * pTextAbschnittOldP )
{
	schluessel = pTextAbschnittOldP->schluessel;
	text	   = pTextAbschnittOldP->text;
	textURL    = pTextAbschnittOldP->textURL;
}
TextAbschnitt::~ TextAbschnitt()
{

}

///////////////////////////////////////////////////////////////////////////////
//  ExterneReferenz												                            		   //
//  Modelliert ein XPlanGML Objekt XP_ExterneReferenz											   //
///////////////////////////////////////////////////////////////////////////////
ExterneReferenz::ExterneReferenz()
{
	relationName	   	    = "";
	informationssystemURL	= "";
	referenzName		      = "";
	referenzURL			      = "";
	referenzMimeType		  = "";
	beschreibung			    = "";
	georefURL				      = "";
	georefMimeType			  = "";
  datum                 = "";
}

ExterneReferenz::ExterneReferenz ( ExterneReferenz * pExterneReferenzOldP )
{
	relationName		      = pExterneReferenzOldP->relationName;
	informationssystemURL = pExterneReferenzOldP->informationssystemURL;
	referenzName		      = pExterneReferenzOldP->referenzName;
	referenzURL			      = pExterneReferenzOldP->referenzURL;
	referenzMimeType	    = pExterneReferenzOldP->referenzMimeType;
	beschreibung		      = pExterneReferenzOldP->beschreibung;
	georefURL			        = pExterneReferenzOldP->georefURL;
	georefMimeType		    = pExterneReferenzOldP->georefMimeType;
  datum                 = pExterneReferenzOldP->datum;
}

ExterneReferenz::~ ExterneReferenz()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Bereich										                                							 //
//  Modelliert ein XPlanGML Objekt BP_Plan, FP_Plan, LP_Plan oder LP_Plan		 //
///////////////////////////////////////////////////////////////////////////////
Bereich::Bereich()
{
	nummer		         = 0;
	name		           = "";
	bedeutung	      	 = "";
	geometrieDatei     = "";
	bedeutungEnum      = "";
	massstab           = -1;
	versionBauNVO      = "";
  versionBauNVOText  = "";

	pRasterplanBasis = NULL;
}

Bereich::Bereich ( Bereich * pBereichOld )
{
	nummer			        = pBereichOld->nummer;
	name			          = pBereichOld->name;
	bedeutung		        = pBereichOld->bedeutung;
	geometrieDatei	    = pBereichOld->geometrieDatei;
	bedeutungEnum       = pBereichOld->bedeutungEnum;
	massstab        		= pBereichOld->massstab;
	versionBauNVO       = pBereichOld->versionBauNVO;
  versionBauNVOText   = pBereichOld->versionBauNVOText;
  versionBauGBDatum   = pBereichOld->versionBauGBDatum;
  versionBauGBText    = pBereichOld->versionBauGBText;
  versionBROGDatum    = pBereichOld->versionBROGDatum;
  versionBROGText     = pBereichOld->versionBROGText;
  versionLPLGDatum    = pBereichOld->versionLPLGDatum;
  versionLPLGText     = pBereichOld->versionLPLGText;

	if ( pBereichOld->pRasterplanBasis != NULL )
		pRasterplanBasis = new RasterplanBasis ( pBereichOld->pRasterplanBasis );
	else
		pRasterplanBasis = NULL;

	for ( unsigned int i = 0; i < pBereichOld->vRasterplanAenderungen.size(); i++ )
	{
		RasterplanAenderung * pRasterplanAenderungOld = pBereichOld->vRasterplanAenderungen[i]; 
		RasterplanAenderung * pRasterplanAenderung = new RasterplanAenderung ( pRasterplanAenderungOld );
		vRasterplanAenderungen.push_back ( pRasterplanAenderung );
	}
}

Bereich::~Bereich()
{
	if ( pRasterplanBasis != NULL ) 
		delete pRasterplanBasis;

	for ( unsigned  int i = 0; i < vRasterplanAenderungen.size(); i++ )
		delete vRasterplanAenderungen[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt das Attribut RasterplanBasis																			 //
///////////////////////////////////////////////////////////////////////////////
void Bereich::setRasterplanBasis ( RasterplanBasis * pRasterplanBasisP )
{
	if ( pRasterplanBasis != NULL && pRasterplanBasis != pRasterplanBasisP )
		delete pRasterplanBasis;

	pRasterplanBasis = pRasterplanBasisP;
}


///////////////////////////////////////////////////////////////////////////////
//  Addiert ein Attribut RasterplanAenderung																 //
///////////////////////////////////////////////////////////////////////////////
void Bereich::addRasterplanAenderung ( RasterplanAenderung * pRasterplanAenderungP )
{
	vRasterplanAenderungen.push_back ( pRasterplanAenderungP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Attribut RasterplanAenderung																 //
///////////////////////////////////////////////////////////////////////////////
RasterplanAenderung * Bereich::getRasterplanAenderung ( unsigned int index )
{
	if ( index < 0 || index >= vRasterplanAenderungen.size() )
		return NULL;
	else
		return vRasterplanAenderungen[index];
}

///////////////////////////////////////////////////////////////////////////////
//  RasterplanBasis													                            		 //
//  Modelliert ein Objekt XP_RastenplanBasis (Version <= 4.1) oder           //
//  XP_Rasterdarstellung (XPlanGML 5.0)																			 //
///////////////////////////////////////////////////////////////////////////////
RasterplanBasis::RasterplanBasis()
{
	pExterneReferenzText = NULL;
}

RasterplanBasis::RasterplanBasis ( RasterplanBasis * pRasterplanBasisOld )
{
	
}

RasterplanBasis::~RasterplanBasis()
{
	unsigned int i;

	for ( i = 0; i < vExterneReferenzenLegende.size(); i++ )
		delete vExterneReferenzenLegende[i];

	for ( i = 0; i < vExterneReferenzenPlan.size(); i++ )
		delete vExterneReferenzenPlan[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine ExterneReferenz hinzu																					 //
///////////////////////////////////////////////////////////////////////////////
void RasterplanBasis::addExterneReferenz ( ExterneReferenz * pExterneReferenz )
{
  if ( pExterneReferenz->relationName == "xplan:refText" )
	{
		if ( pExterneReferenzText != NULL && pExterneReferenzText != pExterneReferenz ) 
			delete pExterneReferenzText;
		pExterneReferenzText = pExterneReferenz;
	}
	else
    if ( pExterneReferenz->relationName == "xplan:refLegende" )
		vExterneReferenzenLegende.push_back ( pExterneReferenz );
	else
    if ( pExterneReferenz->relationName == "xplan:refScan" )
		vExterneReferenzenPlan.push_back ( pExterneReferenz );
	
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle ExterneReferenz Objekte																		 //
///////////////////////////////////////////////////////////////////////////////
size_t RasterplanBasis::getExterneReferenzen ( vector<ExterneReferenz*> & vExterneReferenzenP )
{
	unsigned int i;

	if ( pExterneReferenzText != NULL ) 
		vExterneReferenzenP.push_back ( pExterneReferenzText );

	for ( i = 0; i < vExterneReferenzenLegende.size(); i++ )
		vExterneReferenzenP.push_back ( vExterneReferenzenLegende[i] );

	for ( i = 0; i < vExterneReferenzenPlan.size(); i++ )
		vExterneReferenzenP.push_back ( vExterneReferenzenPlan[i] );

	return vExterneReferenzenP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht das Attribut pExterneReferenzText																 //
///////////////////////////////////////////////////////////////////////////////
void RasterplanBasis::deleteExterneReferenzText()
{
	if ( pExterneReferenzText != NULL ) delete pExterneReferenzText;
	pExterneReferenzText = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Attribut pExterneReferenzLegende														 //
///////////////////////////////////////////////////////////////////////////////
ExterneReferenz *  RasterplanBasis::getExterneReferenzLegende ( unsigned int index )
{
	if ( index < 0 || index >= vExterneReferenzenLegende.size() )
		return NULL;
	else
		return vExterneReferenzenLegende[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht ein Attribut pExterneReferenzLegende														   //
///////////////////////////////////////////////////////////////////////////////
void RasterplanBasis::deleteExterneReferenzLegende ( unsigned int index )
{
	unsigned int i;

	if ( index >=  0 && index < vExterneReferenzenLegende.size() )
	{
		ExterneReferenz * pExterneReferenz = vExterneReferenzenLegende[index];
		for ( i = index + 1; i < vExterneReferenzenLegende.size(); i++ )
			vExterneReferenzenLegende[i-1] = vExterneReferenzenLegende[i];

		vExterneReferenzenLegende.pop_back();
		delete pExterneReferenz;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Attribut pExterneReferenzPlan																 //
///////////////////////////////////////////////////////////////////////////////
ExterneReferenz *  RasterplanBasis::getExterneReferenzPlan ( unsigned int index )
{
	if ( index < 0 || index >= vExterneReferenzenPlan.size() )
		return NULL;
	else
		return vExterneReferenzenPlan[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht ein Attribut pExterneReferenzPlan																 //
///////////////////////////////////////////////////////////////////////////////
void RasterplanBasis::deleteExterneReferenzPlan ( unsigned int index )
{
	unsigned int i;

	if ( index >=  0 && index < vExterneReferenzenPlan.size() )
	{
		ExterneReferenz * pExterneReferenz = vExterneReferenzenPlan[index];
		for ( i = index + 1; i < vExterneReferenzenLegende.size(); i++ )
			vExterneReferenzenPlan[i-1] = vExterneReferenzenPlan[i];

		vExterneReferenzenPlan.pop_back();
		delete pExterneReferenz;
	}
}


///////////////////////////////////////////////////////////////////////////////
//  RasterplanAenderung		                            											 //
//  Dummy-Implementierung										                          			 //
///////////////////////////////////////////////////////////////////////////////
RasterplanAenderung::RasterplanAenderung()
{

}

RasterplanAenderung::RasterplanAenderung( RasterplanAenderung * pRasterplanAenderungOld )
{

}

RasterplanAenderung::~ RasterplanAenderung()
{

}





