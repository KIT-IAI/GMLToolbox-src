#include "stdafx.h"

#include <vector>
#include <map>
#include <string>
#include <set>

#include "GmlSchema.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "XPlanGML.h"

#include "ShpFileObjekte.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  class ShapeFileObjekte												                        	 //
//  Modelliert die Struktur einer Anzahl von Shape-Files		            		 //
///////////////////////////////////////////////////////////////////////////////
ShapeFileObjekte::ShapeFileObjekte ( XPlanGML * pXPlanGML )
{
  m_pXPlanGML = pXPlanGML;
	plaeneAusShapeErzeugen = false;
}

 ShapeFileObjekte::~ShapeFileObjekte()
 {
	 map<string,ShapeFileObjekt*>::iterator iter;

	 for ( iter = shapeFileObjekte.begin(); iter != shapeFileObjekte.end(); iter++ )
		 delete iter->second;

 }

///////////////////////////////////////////////////////////////////////////////
//  Fügt Shapefile-Objekt hinzu							                      					 //
 ///////////////////////////////////////////////////////////////////////////////
bool ShapeFileObjekte::addShapeFileObjekt ( ShapeFileObjekt * pShapeFileObjekt )
{
	string shapeFileName;

	shapeFileName = pShapeFileObjekt->getShapeFileName ();

	if ( shapeFileObjekte.find ( shapeFileName )  != shapeFileObjekte.end() ) return false;

	shapeFileObjekte[shapeFileName] = pShapeFileObjekt;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Shapefile-Objekte							                    				 //
///////////////////////////////////////////////////////////////////////////////
size_t ShapeFileObjekte::getShapeFileObjekte ( std::vector<ShapeFileObjekt*> & shapeFileObjekteP )
{
	map<string,ShapeFileObjekt*>::iterator iter;

	for ( iter = shapeFileObjekte.begin(); iter != shapeFileObjekte.end(); iter++ )
	{
		ShapeFileObjekt * pShapeFileObjekt = iter->second;
		shapeFileObjekteP.push_back ( pShapeFileObjekt );
	}

	return shapeFileObjekte.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein einzelnes Shapefile-Objekt			                  					 //
///////////////////////////////////////////////////////////////////////////////
ShapeFileObjekt * ShapeFileObjekte::getShapeFileObjekt ( std::string shapeFileNameP )
{
	map<string,ShapeFileObjekt*>::iterator iter;

	iter = shapeFileObjekte.find ( shapeFileNameP );
	if ( iter != shapeFileObjekte.end() ) return iter->second;
	else								  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Sucht Shapefile-Attribute, die auf GML-Enumerationen abgebildet werden	 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileObjekte::initEnumConversion( GmlSchema * pGmlObjekteP )
{
	size_t  	          i, j, k, anzShapeFiles, anzShapeFileAttribute, anzGmlAttribute;
	ShapeFileObjekt   * pShapeFileObjekt;
	ShapeFileAttribut * pShapeFileAttribut;
	GmlAttribut		    * pGmlAttribut;
	GmlEnumeration    * pGmlEnumeration;
	string			      	gmlEnumerationId;
  string              externalCodeListPath;

	vector<ShapeFileObjekt*>   shapeFileObjekte;
	vector<ShapeFileAttribut*> shapeFileAttributeLoc;
	vector<GmlAttribut*>       gmlAttribute;
	vector<StandardAttribut*>  standardAttribute;

	gmlEnumerations.clear();
	shapeFiles.clear();
	shapeFileAttribute.clear();

	anzShapeFiles = getShapeFileObjekte ( shapeFileObjekte );
	for ( i = 0; i < anzShapeFiles; i++ )
	{
		shapeFileAttributeLoc.clear();
		pShapeFileObjekt = shapeFileObjekte[i];
		anzShapeFileAttribute = pShapeFileObjekt->getShapeFileAttribute ( shapeFileAttributeLoc );
		for ( j = 0; j < anzShapeFileAttribute; j++ )
		{
			gmlAttribute.clear();
			pShapeFileAttribut = shapeFileAttributeLoc[j];
			anzGmlAttribute = pShapeFileAttribut->getGmlAttribute ( gmlAttribute );
			for ( k = 0; k < anzGmlAttribute; k++ )
			{
				pGmlAttribut = gmlAttribute[k];
				if ( pGmlAttribut->getGmlAttributTyp() == _ENUMERATION ||
					 pGmlAttribut->getGmlAttributTyp() == _EXTERNAL_CODE_LIST  )
				{
					gmlEnumerationId = pGmlAttribut->getGmlEnumeration();
					if (  pGmlAttribut->getGmlAttributTyp() == _ENUMERATION )
					pGmlEnumeration = pGmlObjekteP->getGmlEnumeration ( gmlEnumerationId );
					else
					if (  pGmlAttribut->getGmlAttributTyp() == _EXTERNAL_CODE_LIST )
          {
            pGmlEnumeration = pGmlObjekteP->getGmlEnumeration ( gmlEnumerationId );
            if ( pGmlEnumeration == NULL )
            {
              pGmlEnumeration = new GmlEnumeration ( gmlEnumerationId, true );
              externalCodeListPath = pGmlObjekteP->createExternalCodeListsPath( gmlEnumerationId );
              pGmlEnumeration->setCodeListDictionary(externalCodeListPath  );
              pGmlObjekteP->addGmlEnumeration( pGmlEnumeration );
            }
          }

				  gmlEnumerations.push_back ( pGmlEnumeration );
					shapeFiles.push_back ( pShapeFileObjekt );
					shapeFileAttribute.push_back ( pShapeFileAttribut );
				}
				if ( ( ( pGmlAttribut->getGmlAttributTyp() == _REFERENZ || pGmlAttribut->getGmlAttributTyp() == _DATA_TYPE ) && 
					    ( pShapeFileAttribut->getRefAttributTyp() == _ENUMERATION || pShapeFileAttribut->getRefAttributTyp() == _EXTERNAL_CODE_LIST ) )   )
				{
					string refElement = pGmlAttribut->getReferenziertesElement();
					GmlKlasse * pRefGmlKlasse = pGmlObjekteP->getGmlKlasse ( refElement );
					GmlAttribut * pRefAttribut = pRefGmlKlasse->getGmlAttribut ( pShapeFileAttribut->getRefAttributName() );

					gmlEnumerationId = pRefAttribut->getGmlEnumeration();
          pGmlEnumeration = pGmlObjekteP->getGmlEnumeration ( gmlEnumerationId );
          if ( pGmlEnumeration != NULL )
					  gmlEnumerations.push_back ( pGmlEnumeration );
					shapeFiles.push_back ( pShapeFileObjekt );
					shapeFileAttribute.push_back ( pShapeFileAttribut );				
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Shapefile-Attribute, die auf GML-Enumerationen abgebildet   //
//  werden			                                														 //
///////////////////////////////////////////////////////////////////////////////
size_t ShapeFileObjekte::getUsedEnums ( vector<GmlEnumeration*> &gmlEnumerationsP, 
									  vector<ShapeFileObjekt*> & shapeFilesP,
									  vector<ShapeFileAttribut*> &shapeFileAttributeP )
{
	unsigned int i;

	for ( i = 0; i < gmlEnumerations.size(); i++ )
		gmlEnumerationsP.push_back ( gmlEnumerations[i] );

	for ( i = 0; i < shapeFiles.size(); i++ )
		shapeFilesP.push_back ( shapeFiles[i] );

	for ( i = 0; i < shapeFileAttribute.size(); i++ )
		shapeFileAttributeP.push_back ( shapeFileAttribute[i] );

	return gmlEnumerationsP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt alle Zuordnungen zu GML-Klassen und Attributen zurück	      			 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileObjekte::resetGmlZuordnung()
{
	map<string,ShapeFileObjekt*>::iterator iter;
	
	for ( iter = shapeFileObjekte.begin(); iter != shapeFileObjekte.end(); iter++ )
	{
		ShapeFileObjekt * pObj = iter->second;
		pObj->resetGmlZuordnung();
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugt eine Konvertierung								                      				 //
///////////////////////////////////////////////////////////////////////////////
ShapeFileNamenKonvertierung * ShapeFileObjekte::generateConversion()
{
	map<string,ShapeFileObjekt*>::iterator iter;

	ShapeFileNamenKonvertierung * pKonv = new ShapeFileNamenKonvertierung ( m_pXPlanGML );

	for ( iter = shapeFileObjekte.begin(); iter != shapeFileObjekte.end(); iter++ )
	{
		ShapeFileObjekt * pShapeFileObjekt = iter->second;
		pShapeFileObjekt->generateConversion ( pKonv );
	}
	pKonv->setPlaeneErzeugen ( plaeneAusShapeErzeugen );

	return pKonv;
}

///////////////////////////////////////////////////////////////////////////////
//  class ShapeFileObjekt											                          		 //
//  Modelliert die Struktur eines einzelnen Shapefiles			          			 //
///////////////////////////////////////////////////////////////////////////////
ShapeFileObjekt::ShapeFileObjekt ( std::string shapeFileNameP )
{
	shapeFileName = shapeFileNameP;
	geometrieTyp  = SHPT_NULL;
  rechtlicheBedeutung = PLANINHALT;

	pGmlKlasse    = NULL;
	bereichNummer = 0;

}
ShapeFileObjekt::~ ShapeFileObjekt ()
{
	 map<string,ShapeFileAttribut*>::iterator iter;
   unsigned int i;

	 for ( iter = shapeFileAttribute.begin(); iter != shapeFileAttribute.end(); iter++ )
		 delete iter->second;

   for ( i = 0; i < standardAttribute.size(); i++ )
     delete standardAttribute[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Attribut hinzu							                        						 //
///////////////////////////////////////////////////////////////////////////////
bool ShapeFileObjekt::addShapeFileAttribut ( ShapeFileAttribut * pShapeFileAttributP )
{
	string shapeFileAttributName;

	shapeFileAttributName = pShapeFileAttributP->getShapeFileAttributName();
	if ( shapeFileAttribute.find ( shapeFileAttributName ) != shapeFileAttribute.end () ) return false;

	shapeFileAttribute[shapeFileAttributName] = pShapeFileAttributP;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert Shapefile-Attribute						                      						 //
///////////////////////////////////////////////////////////////////////////////
size_t ShapeFileObjekt::getShapeFileAttribute ( vector<ShapeFileAttribut*> & shapeFileAttributeP )
{
	 map<string,ShapeFileAttribut*>::iterator iter;

	for ( iter = shapeFileAttribute.begin(); iter != shapeFileAttribute.end(); iter++ )
	{
		ShapeFileAttribut * pAttribut = iter->second;
		shapeFileAttributeP.push_back ( pAttribut );
	}

	return shapeFileAttribute.size();
}

ShapeFileAttribut * ShapeFileObjekt::getShapeFileAttribut ( string shapeAttributNameP )
{
	map<string,ShapeFileAttribut*>::iterator iter;

	iter = shapeFileAttribute.find ( shapeAttributNameP );
	if ( iter != shapeFileAttribute.end() ) return iter->second;
	else									return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Standard-Attribut hinzu							                    			 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileObjekt::addStandardAttribut ( StandardAttribut * pStandardAttributP )
{
  StandardAttribut * pStandardAttributCopy = new StandardAttribut ( pStandardAttributP );
	standardAttribute.push_back ( pStandardAttributCopy );
}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt das zu pGmlAttributP gehörige Standard-Attribut		        		 //
///////////////////////////////////////////////////////////////////////////////
bool ShapeFileObjekt::removeStandardAttribut ( GmlAttribut * pGmlAttributP )
{
	unsigned  int i, j;
	
	for ( i = 0; i < standardAttribute.size(); i++ )
	{
		StandardAttribut * pStandardAttribut = standardAttribute[i];
		if ( pStandardAttribut->getGmlAttribut() == pGmlAttributP )
		{
			for ( j = i + 1; j < standardAttribute.size(); j++ )
				standardAttribute[j-1] = standardAttribute[j];
			delete pStandardAttribut;
			standardAttribute.pop_back();
      i = i - 1;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Standard-Attribute                        									 //
///////////////////////////////////////////////////////////////////////////////
size_t ShapeFileObjekt::getStandardAttribute ( std::vector<StandardAttribut*> & standardAttributeP )
{
	unsigned int i;

	for ( i = 0; i < standardAttribute.size(); i++ )
		standardAttributeP.push_back ( standardAttribute[i] );

	return standardAttribute.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Standar-Attribut an Listenposition index   									 //
///////////////////////////////////////////////////////////////////////////////
StandardAttribut * ShapeFileObjekt::getStandardAttribut ( unsigned int index )
{
	if ( index < 0 || index >= standardAttribute.size() ) return NULL;
	else												  return standardAttribute[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das erste einem XPlanGML-Attribut zugeordnete Standard-Attribut  //
///////////////////////////////////////////////////////////////////////////////
StandardAttribut * ShapeFileObjekt::getStandardAttribut ( GmlAttribut * pGmlAttributP )
{
	unsigned int i;

	for ( i = 0; i < standardAttribute.size(); i++ )
	{
		StandardAttribut * pStandardAttribut = standardAttribute[i];
		if ( pStandardAttribut->getGmlAttribut()->getGmlAttributName() ==
			  pGmlAttributP->getGmlAttributName() ) return pStandardAttribut;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle einem XPlanGML-Attribut zugeordnete Standard-Attribute      //
///////////////////////////////////////////////////////////////////////////////
size_t ShapeFileObjekt::getStandardAttribute ( GmlAttribut * pGmlAttributP, std::vector<StandardAttribut*> & standardAttributeP )
{
  unsigned int i;

  for ( i = 0; i < standardAttribute.size(); i++ )
  {
    StandardAttribut * pStandardAttribut = standardAttribute[i];
    if ( pStandardAttribut->getGmlAttribut()->getGmlAttributName() == pGmlAttributP->getGmlAttributName() )
      standardAttributeP.push_back( pStandardAttribut );
  }
  return standardAttributeP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht alle Zuordnungen von GML-Klassen und Attributen	        				 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileObjekt::resetGmlZuordnung()
{
	map<string,ShapeFileAttribut*>::iterator iter;
	unsigned int	        									 i;

	pGmlKlasse = NULL;

	for ( iter = shapeFileAttribute.begin(); iter != shapeFileAttribute.end(); iter++ )
	{
		ShapeFileAttribut * pAttribut = iter->second;
		pAttribut->resetGmlZuordnung();
	}

	for ( i = 0; i < standardAttribute.size(); i++ )
		delete standardAttribute[i];

	attributNamePlanId = "";

	standardAttribute.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert Konversion							                          						 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileObjekt::generateConversion( ShapeFileNamenKonvertierung * pKonvP )
{
	string             gmlKlassenName;
	string             shapeFileAttributName;
	string             generAttributName;
	string             refAttributName;
	int                refAttributIndex;
	string             generAttributBedeutung;
	string             gmlAttributName;
	string             gmlEnumerationId;
	string             shapeAttributWert;
	string             gmlAttributWert;
	string			       defaultWert;
	size_t             anzGmlAttribute, i, j, anzAttributWerte;
	unsigned int       index = 0;
	ATTRIBUT_TYP       gmlAttributTyp;
	ATTRIBUT_TYP       refAttributTyp;
	StandardAttribut * pStandardAttribut;
	GmlAttribut      * pGmlAttribut;
  Feature       * pComplexDefaultWert;

	vector<GmlAttribut*>                     gmlAttribute;
	vector<ShapeFileAttributWert*>           shapeFileAttributWerte;
	map<string,ShapeFileAttribut*>::iterator iter;

	if ( pGmlKlasse == NULL ) return;

	GmlSchema * pGmlObjekte = pKonvP->getXPlanGML()->getFeatures()->getGmlSchema();

  gmlKlassenName = pGmlKlasse->getGmlElementName();
	KlassenName * pKonvKlasse = pKonvP->addKlassenName ( shapeFileName, gmlKlassenName,
                                                       bereichNummer, rechtlicheBedeutung );

	for ( iter = shapeFileAttribute.begin(); iter != shapeFileAttribute.end(); iter++ )
	{
		ShapeFileAttribut * pShapeFileAttribut = iter->second;

		shapeFileAttributName = pShapeFileAttribut->getShapeFileAttributName();
		gmlAttribute.clear();
		anzGmlAttribute = pShapeFileAttribut->getGmlAttribute ( gmlAttribute );		

		for ( i = 0; i < anzGmlAttribute; i++ )
		{
			pGmlAttribut    = gmlAttribute[i];
			gmlAttributName = pGmlAttribut->getGmlAttributName();
			gmlAttributTyp  = pGmlAttribut->getGmlAttributTyp();
			generAttributName = "";
			generAttributBedeutung = "";

			if ( gmlAttributTyp == _ENUMERATION || 
				 gmlAttributTyp == _EXTERNAL_CODE_LIST )
			{
				GmlEnumeration * pGmlEnumeration;

				gmlEnumerationId = pGmlAttribut->getGmlEnumeration();	
				Enumeration  * pKonvEnumeration = pKonvP->getEnumeration ( gmlEnumerationId );
        pGmlEnumeration = pGmlObjekte->getGmlEnumeration ( gmlEnumerationId );	
		
				shapeFileAttributWerte.clear();
				anzAttributWerte = pShapeFileAttribut->getShapeFileAttributWerte ( shapeFileAttributWerte );
				for ( j = 0; j < anzAttributWerte; j++ )
				{
					ShapeFileAttributWert * pAttributWert = shapeFileAttributWerte[j];
					shapeAttributWert = pAttributWert->getAttributWert();
					GmlEnumerationItem * pEnumerationItem = pAttributWert->getEnumerationItem( pGmlEnumeration );
					if ( pEnumerationItem != NULL )
					{
						gmlAttributWert = pEnumerationItem->getGmlEnumerationItemName();
						if ( pKonvEnumeration == NULL )
						{
							pKonvEnumeration = new Enumeration ( gmlEnumerationId );
							pKonvP->addEnumeration ( pKonvEnumeration );
              pKonvEnumeration->setDictionary( pGmlEnumeration->getCodeListDictionary() );
						}
						pKonvEnumeration->addEnumerationItem ( shapeAttributWert, gmlAttributWert );
					}
				}						
			}
			else gmlEnumerationId = "";

			refAttributTyp = _SONST;
			refAttributName = "";
			refAttributIndex = 0;
			if ( gmlAttributTyp == _REFERENZ || gmlAttributTyp == _DATA_TYPE )
			{
				refAttributName = pShapeFileAttribut->getRefAttributName();
				refAttributTyp  = pShapeFileAttribut->getRefAttributTyp();
				refAttributIndex = pShapeFileAttribut->getRefAttributIndex();
        generAttributName = pShapeFileAttribut->getGenerAttributName();

				if ( refAttributTyp == _ENUMERATION || 
					 refAttributTyp == _EXTERNAL_CODE_LIST )
				{
					GmlEnumeration * pGmlEnumeration;

					string refElementName = pGmlAttribut->getReferenziertesElement();
					GmlKlasse * pRefGmlKlasse = pGmlObjekte->getGmlKlasse ( refElementName );
					GmlAttribut * pRefGmlAttribut = pRefGmlKlasse->getGmlAttribut ( refAttributName );
					gmlEnumerationId = pRefGmlAttribut->getGmlEnumeration();	
					Enumeration  * pKonvEnumeration = pKonvP->getEnumeration ( gmlEnumerationId );
          pGmlEnumeration = pGmlObjekte->getGmlEnumeration ( gmlEnumerationId );
		
          pKonvEnumeration->setDictionary( pGmlEnumeration->getCodeListDictionary() );
				
					shapeFileAttributWerte.clear();
					anzAttributWerte = pShapeFileAttribut->getShapeFileAttributWerte ( shapeFileAttributWerte );
					for ( j = 0; j < anzAttributWerte; j++ )
					{
						ShapeFileAttributWert * pAttributWert = shapeFileAttributWerte[j];
						shapeAttributWert = pAttributWert->getAttributWert();
						GmlEnumerationItem * pEnumerationItem = pAttributWert->getEnumerationItem( pGmlEnumeration );
						if ( pEnumerationItem != NULL )
						{
							gmlAttributWert = pEnumerationItem->getGmlEnumerationItemName();
							if ( pKonvEnumeration == NULL )
							{
								pKonvEnumeration = new Enumeration ( gmlEnumerationId );
								pKonvP->addEnumeration ( pKonvEnumeration );
							}
							pKonvEnumeration->addEnumerationItem ( shapeAttributWert, gmlAttributWert );
						}
					}					
				}
			}

			pStandardAttribut = getStandardAttribut ( pGmlAttribut );
			if ( pStandardAttribut != NULL && (gmlAttributTyp == _REFERENZ || gmlAttributTyp == _DATA_TYPE) )
      {
        defaultWert = pStandardAttribut->getDefaultWert();
        pComplexDefaultWert = pStandardAttribut->getComplexDefaultWert();
      }
			else
      {
        defaultWert = "";
        pComplexDefaultWert = NULL;
      }

			pKonvKlasse->addAttributName ( shapeFileAttributName, gmlAttributName, refAttributIndex, gmlAttributTyp,
										   generAttributName, gmlEnumerationId, defaultWert, refAttributName, refAttributTyp, pComplexDefaultWert );
		}
	}

	for ( i = 0; i < standardAttribute.size(); i++ )
	{
		pStandardAttribut = standardAttribute[i];
		pGmlAttribut      = pStandardAttribut->getGmlAttribut();
		gmlAttributName   = pGmlAttribut->getGmlAttributName();
    pComplexDefaultWert   = pStandardAttribut->getComplexDefaultWert();

		bool exist = pKonvKlasse->existAttributObjektToStandardName ( gmlAttributName );
		if ( !exist )
		{
			gmlAttributTyp        = pGmlAttribut->getGmlAttributTyp();
			gmlEnumerationId      = pGmlAttribut->getGmlEnumeration();
			defaultWert           = pStandardAttribut->getDefaultWert();
			shapeFileAttributName = "";

			pKonvKlasse->addAttributName ( shapeFileAttributName, gmlAttributName, pStandardAttribut->getIndex(), gmlAttributTyp,
										   "",  gmlEnumerationId, defaultWert, "", _SONST, pComplexDefaultWert );
		}
	}

	if ( attributNamePlanId != "" )
	{
		gmlEnumerationId = "";
		defaultWert     = "";
    pKonvKlasse->addAttributName ( attributNamePlanId, string("xplan:__planId"), 0, _STRING,
										   "",  gmlEnumerationId, defaultWert, "", _SONST, NULL );
	}
}

///////////////////////////////////////////////////////////////////////////////
//  class ShapeFileAttribut		                        											 //
//  Modelliert ein Attribut eines Shapefiles					                			 //
///////////////////////////////////////////////////////////////////////////////
ShapeFileAttribut::ShapeFileAttribut ( std::string shapeFileAttributNameP )
{
	shapeFileAttributName = shapeFileAttributNameP;
	datentyp			   = _SONST;
	refAttributTyp   = _SONST;
	refAttributIndex = 0;
}

ShapeFileAttribut::~ ShapeFileAttribut ()
{
	map<string,ShapeFileAttributWert*>::iterator iter;

	for ( iter = shapeFileAttributWerte.begin(); iter != shapeFileAttributWerte.end(); iter ++ )
		delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein GML-Attribut hinzu                      											   //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileAttribut::addGmlAttribut ( GmlAttribut * pGmlAttributP )
{
	gmlAttribute.push_back ( pGmlAttributP );
}


///////////////////////////////////////////////////////////////////////////////
//  Ordnet ein GML-Attribut zu	                      											 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileAttribut::setGmlAttribut ( unsigned int indexP, GmlAttribut * pGmlAttributP )
{
	if ( indexP < gmlAttribute.size() ) gmlAttribute[indexP] = pGmlAttributP;
	else
	if ( indexP == gmlAttribute.size() )gmlAttribute.push_back ( pGmlAttributP );

}

///////////////////////////////////////////////////////////////////////////////
//  Entfernt ein GML-Attribut									                        			 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileAttribut::removeGmlAttribut ( unsigned int indexP )
{
	unsigned int i;

	if ( indexP < 0 || indexP >= gmlAttribute.size() ) return;

	for ( i = indexP; i < gmlAttribute.size()-1; i++ )
		gmlAttribute[i] = gmlAttribute[i+1];

	gmlAttribute.pop_back();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle zugeordneten GML-Attribute		                							 //
///////////////////////////////////////////////////////////////////////////////
size_t ShapeFileAttribut::getGmlAttribute ( std::vector<GmlAttribut*> & gmlAttributeP )
{
	unsigned int i;

	for ( i = 0; i < gmlAttribute.size(); i++ )
	{
		GmlAttribut * pGmlAttribut = gmlAttribute[i];
		gmlAttributeP.push_back ( pGmlAttribut );
	}

	return gmlAttribute.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Attribut-Wert hinzu						                    					 //
///////////////////////////////////////////////////////////////////////////////
bool ShapeFileAttribut::addShapeFileAttributWert (  ShapeFileAttributWert * pShapeFileAttributWertP )
{
	string      wert;

	wert = pShapeFileAttributWertP->getAttributWert();
	if ( shapeFileAttributWerte.find ( wert ) != shapeFileAttributWerte.end() ) return false;

	shapeFileAttributWerte[wert] = pShapeFileAttributWertP;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert true, wenn ein Attribut-Wert schon zugeordnet wurde	      			 //
///////////////////////////////////////////////////////////////////////////////
bool ShapeFileAttribut::existShapeFileAttributWert (  std::string shapeFileAttributWert )
{
	if ( shapeFileAttributWerte.find ( shapeFileAttributWert ) != shapeFileAttributWerte.end() ) return true;
	else return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen Attribut-Wert								                      				 //
///////////////////////////////////////////////////////////////////////////////
ShapeFileAttributWert * ShapeFileAttribut::getShapeFileAttributWert ( std::string shapeFileAttributWert )
{
	map<string,ShapeFileAttributWert*>::iterator iter;

	iter = shapeFileAttributWerte.find ( shapeFileAttributWert );
	if ( iter != shapeFileAttributWerte.end() ) return iter->second;
	else									    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Attribut-Werte							                      					 //
///////////////////////////////////////////////////////////////////////////////
size_t ShapeFileAttribut::getShapeFileAttributWerte ( vector<ShapeFileAttributWert*> & shapeFileAttributWerteP )
{
	map<string,ShapeFileAttributWert*>::iterator iter;

	for ( iter = shapeFileAttributWerte.begin(); iter != shapeFileAttributWerte.end(); ++iter )
	{
		ShapeFileAttributWert * pShapeFileAttributWert = iter->second;
		shapeFileAttributWerteP.push_back ( pShapeFileAttributWert );
	}

	return shapeFileAttributWerteP.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht alle Zuordnungen von GML-Klassen und Attributen	        				 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileAttribut::resetGmlZuordnung()
{
	map<string,ShapeFileAttributWert*>::iterator iter;

	gmlAttribute.clear();

	for ( iter = shapeFileAttributWerte.begin(); iter != shapeFileAttributWerte.end(); iter++ )
	{
		ShapeFileAttributWert * pAttributWert = iter->second;
		pAttributWert->resetGmlZuordnung();
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Löscht alle Zuordnungen der Enumerationen             	        				 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileAttribut::resetAttributWerte()
{
  map<string,ShapeFileAttributWert*>::iterator iter;

  for ( iter = shapeFileAttributWerte.begin(); iter != shapeFileAttributWerte.end(); iter++ )
  {
    ShapeFileAttributWert * pAttributWert = iter->second;
    pAttributWert->resetGmlZuordnung();
  }
}

///////////////////////////////////////////////////////////////////////////////
//  class StandardAttribut					                        								 //
//  Vorgabe eines Attribut-Standardwertes für alle aus dem Shapefile         //
//  konvertieren Objekte				                            								 //
///////////////////////////////////////////////////////////////////////////////
StandardAttribut::StandardAttribut ( GmlAttribut * pGmlAttributP )
{
	pGmlAttribut        = pGmlAttributP;
  pComplexDefaultWert = NULL;
	index               = 0;
}

StandardAttribut::StandardAttribut ( StandardAttribut * StandardAttributOld )
{
	pGmlAttribut = StandardAttributOld->pGmlAttribut;
	defaultWert  = StandardAttributOld->defaultWert;
	index        = StandardAttributOld->index;

	if ( StandardAttributOld->pComplexDefaultWert != NULL )
		pComplexDefaultWert = new Feature ( StandardAttributOld->pComplexDefaultWert, StandardAttributOld->pComplexDefaultWert->getGmlId() );
	else
		pComplexDefaultWert = NULL;
}

StandardAttribut::~StandardAttribut()
{
  if ( pComplexDefaultWert != NULL ) delete pComplexDefaultWert;

}

///////////////////////////////////////////////////////////////////////////////
// Setzt das zugeordnete Komplex-Attribut																		 //
///////////////////////////////////////////////////////////////////////////////
void StandardAttribut::setComplexDefaultWert (Feature * pComplexDefaultWertP )
{ 
  if ( pComplexDefaultWertP == pComplexDefaultWert )return;

  if ( pComplexDefaultWert != NULL ) delete pComplexDefaultWert;

  if ( pComplexDefaultWertP != NULL  )
    pComplexDefaultWert = new Feature ( pComplexDefaultWertP, pComplexDefaultWertP->getGmlId() );
  else
    pComplexDefaultWert = NULL;  
}

///////////////////////////////////////////////////////////////////////////////
//  class ShapeFileAttributWert					                      							 //
//  Modelliert einen Wert, den ein Shapefile-Attribut annimmt		        		 //
///////////////////////////////////////////////////////////////////////////////
ShapeFileAttributWert::ShapeFileAttributWert ( std::string wertP )
{
	attributWert	 = wertP;
}

ShapeFileAttributWert::~ShapeFileAttributWert()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Ordnet eine XPlanGML Enumerations-Item und die zugehörige Enumeration zu //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileAttributWert::setEnumerationItem ( GmlEnumeration * pEnumerationP, GmlEnumerationItem * pEnumerationItemP )
{
	unsigned int i;

	for ( i = 0; i < vEnumerationen.size(); i++ )
	{
		if ( vEnumerationen[i] == pEnumerationP )
		{
			vEnumerationsItems[i] = pEnumerationItemP;
			return;
		}
	}

	vEnumerationen.push_back ( pEnumerationP );
	vEnumerationsItems.push_back ( pEnumerationItemP );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das in der übergebenen Enumeration zugeordnete Item							 //
///////////////////////////////////////////////////////////////////////////////
GmlEnumerationItem * ShapeFileAttributWert::getEnumerationItem ( GmlEnumeration * pEnumerationP )
{
	unsigned int i;

	for ( i = 0; i < vEnumerationen.size(); i++ )
	{
		if ( vEnumerationen[i] == pEnumerationP )return	vEnumerationsItems[i];
	}

	return NULL;

}

///////////////////////////////////////////////////////////////////////////////
//  Macht die Zuordnung des Enumerations-Items aus der übergebenen					 //
//  Enumeration rückgängig																									 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileAttributWert::resetGmlZuordnung( GmlEnumeration * pEnumerationP )
{
	size_t i;
	size_t index = vEnumerationen.size();

	for ( i = 0; i < vEnumerationen.size(); i++ )
	{
		if ( vEnumerationen[i] == pEnumerationP )
		{
			index = i; 
			break;
		}
	}

	if ( index <  vEnumerationen.size() )
	{
		for ( i = index + 1; i < vEnumerationen.size(); i++ )
		{
			vEnumerationen[i-1] = vEnumerationen[i];
			vEnumerationsItems[i-1] = vEnumerationsItems[i];
		}
		vEnumerationen.pop_back();
		vEnumerationsItems.pop_back();
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Macht alle Zuordnungen rückgängig																				 //
///////////////////////////////////////////////////////////////////////////////
void ShapeFileAttributWert::resetGmlZuordnung()
{
	vEnumerationen.clear();
	vEnumerationsItems.clear();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Dictionary der ersten zugeordneten Enumeration						   //
///////////////////////////////////////////////////////////////////////////////
std::string ShapeFileAttributWert::getCodeListDictionary()
{
  if ( vEnumerationen.size() == 0 ) return "";

  return vEnumerationen[0]->getCodeListDictionary();
}









