#include "StdAfx.h"
//#using <mscorlib.dll>

#include <time.h>
#include <vector>
#include <map>
#include <string>
#include <set>

#include "Convert.h"
#include "Constant.h"
#include "GmlSchema.h"
#include "Dictionary.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "Geometrie.h"
#include "TimeX.h"
#include "UnitOfMeasure.h"

#include "KonversionINSPIRE_PLU.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  class TransformationINSPIRE_PLU_Basis                                    //
//  Basisklasse für XPlanung-->INSPIRE PLU Transformtionen                   //
///////////////////////////////////////////////////////////////////////////////
TransformationINSPIRE_PLU_Basis::TransformationINSPIRE_PLU_Basis (Features * pObjekteXPlanGML, Features * pObjekteINSPIREP,
              UOMList * pUOMList, TransformationINSPIRE_PLU_Parameter * pParameterP)
{
  m_pObjekteXPlanGML   = pObjekteXPlanGML;
  m_pGmlObjekteXPlan   = m_pObjekteXPlanGML->getGmlSchema();
  m_pObjekteINSPIRE    = pObjekteINSPIREP;
  m_pGmlObjekteINSPIRE = m_pObjekteINSPIRE->getGmlSchema();
  m_pUOMList           = pUOMList;
  m_pParameter         = pParameterP;

  m_pPlanINSPIRE       = NULL;
  m_pProtokoll         = NULL;
}

TransformationINSPIRE_PLU_Basis::~TransformationINSPIRE_PLU_Basis()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Liest die Dictionaries der ExternalCodeLists ein                         //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU_Basis::generateDictionaries ( Features * pPlanObjekteINSPIRE, TransformationINSPIRE_PLU_Parameter * pParameter )
{
  GmlDictionaryEntry  * pDictionaryEntry;
  GmlDictionaryReader ^ pDictionaryReader;
  GmlSchema          * pGmlObjekte = pPlanObjekteINSPIRE->getGmlSchema();

  pDictionaryReader = gcnew GmlDictionaryReader ( m_pObjekteXPlanGML->getGmlTyp() );

  pDictionaryEntry = pDictionaryReader->readINSPIRECodeList( pParameter->registryHILUCSValue );
  if ( pDictionaryEntry != NULL )
    pGmlObjekte->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );

  pDictionaryEntry = pDictionaryReader->readINSPIRECodeList( pParameter->registryLandUseClassificationValue );
  if ( pDictionaryEntry != NULL )
    pGmlObjekte->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );

  pDictionaryEntry = pDictionaryReader->readINSPIRECodeList( pParameter->registrySupplementaryRegulationValue );
  if ( pDictionaryEntry != NULL )
    pGmlObjekte->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );

  pDictionaryEntry = pDictionaryReader->readINSPIRECodeList( pParameter->registryLocalSupplementaryRegulationValue );
  if ( pDictionaryEntry != NULL )
    pGmlObjekte->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );

  pDictionaryEntry = pDictionaryReader->readINSPIRECodeList( pParameter->registryLevelOfSpatialPlanValue );
  if ( pDictionaryEntry != NULL )
    pGmlObjekte->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );

  pDictionaryEntry = pDictionaryReader->readINSPIRECodeList( pParameter->registryProcessStepGeneralValue );
  if ( pDictionaryEntry != NULL )
    pGmlObjekte->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );

  pDictionaryEntry = pDictionaryReader->readINSPIRECodeList( pParameter->registryRegulationNatureValue );
  if ( pDictionaryEntry != NULL )
    pGmlObjekte->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );

  pDictionaryEntry = pDictionaryReader->readINSPIRECodeList( pParameter->registryPlanTypeNameValue );
  if ( pDictionaryEntry != NULL )
    pGmlObjekte->getCodeListsDictionary()->addDictionaryEntry( pDictionaryEntry );

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert für jedes erzeugte INSPIRE-Objekte eine inspireId              //
///////////////////////////////////////////////////////////////////////////////
void TransformationINSPIRE_PLU_Basis::generateINSPIREId ( string gmlIdXPlan, Feature * pObjektINSPIRE )
{
  string            rollenName;
  string            gmlId;
  Feature         * pIdentifier;
  FeatureRelation * pRel;

  rollenName = "plu:inspireId";

  gmlId = Constant::createGmlId();
  pIdentifier = new Feature ( ANY_OBJEKT, gmlId, m_pObjekteINSPIRE, "base:Identifier" );
  pIdentifier->setStringAttribut( "base:localId", gmlIdXPlan );
  pIdentifier->setStringAttribut( "base:namespace", m_pParameter->identifierNamespace );
  m_pObjekteINSPIRE->addFeature( pIdentifier );

  pRel = new FeatureRelation;
  pRel->rollenName = rollenName;
  pRel->featureId = pIdentifier->getGmlId();
  pRel->relToFeature = false;
  pObjektINSPIRE->addFeatureRelation ( pRel );

  pObjektINSPIRE->addChild ( pIdentifier->getGmlId() );
  pIdentifier->setParentId( pObjektINSPIRE->getGmlId() );
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert Mime-Type Bezeichnungen                                    //
///////////////////////////////////////////////////////////////////////////////
string TransformationINSPIRE_PLU_Basis::convertMimeTypeXPlanToINSPIRE ( string mimeTypeXPlan )
{
  if ( mimeTypeXPlan == "image/bmp" ) return "bmp";
  else
  if ( mimeTypeXPlan == "application/msword" ) return "doc";
  else
  if ( mimeTypeXPlan == "image/gif" ) return "gif";
  else
  if ( mimeTypeXPlan == "image/jpg" ) return "jpg";
  else
  if ( mimeTypeXPlan == "application/vnd.oasis.opendocument.formula" ) return "odf";
  else
  if ( mimeTypeXPlan == "application/vnd.oasis.opendocument.spreadsheet" ) return "ods";
  else
  if ( mimeTypeXPlan == "image/png" ) return "png";
  else
  if ( mimeTypeXPlan == "application/pdf" ) return "pdf";
  else
  if ( mimeTypeXPlan == "image/tiff" ) return "tiff";
  else
  if ( mimeTypeXPlan == "application/pdf" ) return "pdf";
  else
  if ( mimeTypeXPlan == "application/xml" ) return "xml";
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen von void-Attributen falls notwendig                           //
///////////////////////////////////////////////////////////////////////////////
void TransformationINSPIRE_PLU_Basis::addVoidAttributes ( Feature * pFeature,
									map<string,TransformationINSPIRE_PLU_Parameter::VOID_ATTRIBUT_TYP> &mVoidAttribute )
{
  map<string,TransformationINSPIRE_PLU_Parameter::VOID_ATTRIBUT_TYP>::iterator iter;

  for ( iter = mVoidAttribute.begin(); iter != mVoidAttribute.end(); iter++ )
  {
    string voidAttribut                                         = iter->first;
    TransformationINSPIRE_PLU_Parameter::VOID_ATTRIBUT_TYP  typ = iter->second;

    switch ( typ )
    {
    case TransformationINSPIRE_PLU_Parameter::STRING:
      if ( pFeature->getStringAttributWertAnz( voidAttribut ) == 0 )
      {
        StringAttr * pStringAttr = new StringAttr;
        pStringAttr->isNil = true;
        pStringAttr->setNilReason( "unknown" );
        pFeature->setStringAttribut ( voidAttribut, pStringAttr );
      }
      break;

    case TransformationINSPIRE_PLU_Parameter::INTEGER:
      if ( pFeature->getIntegerAttributWertAnz( voidAttribut ) == 0 )
      {
        IntAttr * pAttr = new IntAttr;
        pAttr->isNil = true;
        pAttr->setNilReason( "unknown" );
        pFeature->setIntegerAttribut ( voidAttribut, pAttr );
      }
      break;

    case TransformationINSPIRE_PLU_Parameter::DOUBLE:
      if ( pFeature->getDoubleAttributWertAnz( voidAttribut ) == 0 )
      {
        DoubleAttr * pAttr = new DoubleAttr;
        pAttr->isNil = true;
        pAttr->setNilReason( "unknown" );
        pFeature->setDoubleAttribut ( voidAttribut, pAttr );
      }
      break;

    case TransformationINSPIRE_PLU_Parameter::BOOLEAN:
      if ( pFeature->getBoolAttributWert( voidAttribut ) == NULL )
      {
        BoolAttr * pAttr = new BoolAttr;
        pAttr->isNil = true;
        pAttr->setNilReason( "unknown" );
        pFeature->setBoolAttribut ( voidAttribut, pAttr );
      }
      break;

    case TransformationINSPIRE_PLU_Parameter::DATE:
      if ( pFeature->getDateAttributWertAnz( voidAttribut ) == 0 )
      {
        DateAttr * pAttr = new DateAttr;
        pAttr->isNil = true;
        pAttr->setNilReason( "unknown" );
        pFeature->setDateAttribut ( voidAttribut, pAttr );
      }
      break;

    case TransformationINSPIRE_PLU_Parameter::URL:
      if ( pFeature->getURLAttributWertAnz( voidAttribut ) == 0 )
      {
        StringAttr * pAttr = new StringAttr;
        pAttr->isNil = true;
        pAttr->setNilReason( "unknown" );
        pFeature->setURLAttribut ( voidAttribut, pAttr );
      }
      break;

    case TransformationINSPIRE_PLU_Parameter::RELATION:
      if ( pFeature->getFeatureRelationenAnz( voidAttribut ) == 0 )
      {
        FeatureRelation * pAttr = new FeatureRelation;
        pAttr->rollenName = voidAttribut;
        pAttr->nil = true;
        pAttr->nilReason = "unknown";
        pFeature->addFeatureRelation ( pAttr );
      }
      break;
    }

  }
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert ein Objekt gmd:CI_Date                                         //
///////////////////////////////////////////////////////////////////////////////
Feature * TransformationINSPIRE_PLU_Basis::createDocumentDate ( std::string date, std::string dateType )
{
  string            gmlId;
  StringAttr      * pSimpleContent;
  FeatureRelation * pRel;

  gmlId = Constant::createGmlId();
  Feature * pDocumentDate =  new Feature ( INSPIRE_PLU_OBJEKT, gmlId, m_pObjekteINSPIRE, "gmd:CI_Date" );
  m_pObjekteINSPIRE->addFeature( pDocumentDate );

  gmlId = Constant::createGmlId();
  Feature * pDate = new Feature ( INSPIRE_PLU_OBJEKT, gmlId, m_pObjekteINSPIRE, "gco:Date");
  m_pObjekteINSPIRE->addFeature( pDate );
  pSimpleContent = new StringAttr;
  pSimpleContent->value = date;
  pDate->setSimpleContent( pSimpleContent );

  pRel = new FeatureRelation;
  pRel->rollenName = "gmd:date";
  pRel->featureId = pDate->getGmlId();
  pRel->relToFeature = false;
  pDocumentDate->addFeatureRelation ( pRel );

  pDocumentDate->addChild ( pDate->getGmlId() );
  pDate->setParentId( pDocumentDate->getGmlId() );

  gmlId = Constant::createGmlId();
  Feature * pDateType = new Feature ( INSPIRE_PLU_OBJEKT, gmlId, m_pObjekteINSPIRE, "gmd:CI_DateTypeCode");
  m_pObjekteINSPIRE->addFeature( pDateType );
  pSimpleContent = new StringAttr;
  pSimpleContent->classifier["codeList"] = "CI_DateTypeCode";
  pSimpleContent->classifier["codeListValue"] = dateType;
  pDateType->setSimpleContent( pSimpleContent );

  pRel = new FeatureRelation;
  pRel->rollenName = "gmd:dateType";
  pRel->featureId = pDateType->getGmlId();
  pRel->relToFeature = false;
  pDocumentDate->addFeatureRelation ( pRel );

  pDocumentDate->addChild ( pDateType->getGmlId() );
  pDateType->setParentId( pDocumentDate->getGmlId() );

  return pDocumentDate;
}

///////////////////////////////////////////////////////////////////////////////
//  Extrahiert einen XPlanGML Attribut-Wert                                  //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU_Basis::getAttributwertXPlan ( Feature * pObjektXPlan, std::string attributNameXPlan, std::string & attributWertXPlan )
{
  GmlElement  * pGmlElement;
  GmlKlasse   * pGmlKlasse;
  GmlAttribut * pGmlAttribut;
  ATTRIBUT_TYP  attributTyp;
  double        h;
  int           ih;
  bool          b, success;
  String      ^ STR;

  attributWertXPlan = "";

  pGmlElement = m_pGmlObjekteXPlan->getGmlElement( pObjektXPlan->getKlassenName() );
  if ( pGmlElement == NULL ) return false;

  pGmlKlasse = m_pGmlObjekteXPlan->getGmlKlasse ( pGmlElement->getGmlTypeName() );
  if ( pGmlKlasse == NULL ) return false;

  pGmlAttribut = pGmlKlasse->getGmlAttribut( attributNameXPlan );
  if ( pGmlAttribut == NULL ) return false;

  attributTyp = pGmlAttribut->getGmlAttributTyp();

  switch ( attributTyp )
  {
  case _ENUMERATION:
  case _EXTERNAL_CODE_LIST:
  case _STRING:
    return pObjektXPlan->getStringAttributWert( attributNameXPlan, attributWertXPlan);

  case _DATE:
  case _DATE_TIME:
    {
      TimePosition * pTime = (TimePosition*)pObjektXPlan->getDateAttributWert ( attributNameXPlan );
      if ( pTime != NULL )
      {
        attributWertXPlan = pTime->toXMLString();
        return true;
      }
      else
        return false;
    }

  case _ANY_URI:
    return pObjektXPlan->getURLAttributWert( attributNameXPlan, attributWertXPlan);

  case _DOUBLE:
  case _ANGLE:
  case _LENGTH:
  case _AREA:
  case _VOLUME:
    success = pObjektXPlan->getDoubleAttributWert( attributNameXPlan, h );
    if ( success )
    {
      STR = XmlConvert::ToString( h );
      QuConvert::systemStr2stdStr( attributWertXPlan, STR );
    }
    return success;

  case _INTEGER:
    success = pObjektXPlan->getIntegerAttributWert( attributNameXPlan, ih );
    if ( success )
    {
      STR = XmlConvert::ToString( ih );
      QuConvert::systemStr2stdStr( attributWertXPlan, STR );
    }
    return success;

  case _BOOLEAN:
    success = pObjektXPlan->getBoolAttributWert( attributNameXPlan, b );
    if ( success )
    {
      STR = XmlConvert::ToString( b );
      QuConvert::systemStr2stdStr( attributWertXPlan, STR );
    }
    return success;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Registry einer INSPIRE-CodeList                              //
///////////////////////////////////////////////////////////////////////////////
string TransformationINSPIRE_PLU_Basis::getRegistry ( string attributNameINSPIRE )
{
  if ( attributNameINSPIRE == "plu:hilucsLandUse" )
    return m_pParameter->registryHILUCSValue;
  else
  if ( attributNameINSPIRE == "plu:specificLandUse" )
    return m_pParameter->registryLandUseClassificationValue;
  else
  if ( attributNameINSPIRE == "plu:supplementaryRegulation" )
    return m_pParameter->registrySupplementaryRegulationValue;
  else
  if ( attributNameINSPIRE == "plu:localSupplelementarylRegulation" )
    return m_pParameter->registryLocalSupplementaryRegulationValue;
  else
  if ( attributNameINSPIRE == "plu:levelOfSpatialPlan" )
    return m_pParameter->registryLevelOfSpatialPlanValue;
  else
  if ( attributNameINSPIRE == "plu:planTypeName" )
    return m_pParameter->registryPlanTypeNameValue;
  else
  if ( attributNameINSPIRE == "plu:regulationNature" )
    return m_pParameter->registryRegulationNatureValue;
  else
  if ( attributNameINSPIRE == "plu:processStepGeneral" )
    return m_pParameter->registryProcessStepGeneralValue;
  else
  if ( attributNameINSPIRE == "base2:level" )
    return m_pParameter->registryLegislationLevelValue;

  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Typ eines INSPIRE PLU Attributs                              //
///////////////////////////////////////////////////////////////////////////////
ATTRIBUT_TYP TransformationINSPIRE_PLU_Basis::getAttributTypINSPIRE ( string klassenNameINSPIRE, string attributNameINSPIRE, 
									string & referenzKlassenName )
{
  GmlElement  * pGmlElement;
  GmlKlasse   * pGmlKlasse;
  GmlAttribut * pGmlAttribut;
  ATTRIBUT_TYP  attributTyp;

  pGmlElement = m_pGmlObjekteINSPIRE->getGmlElement( klassenNameINSPIRE );
  if ( pGmlElement == NULL ) return _SONST;

  pGmlKlasse = m_pGmlObjekteINSPIRE->getGmlKlasse ( pGmlElement->getGmlTypeName() );
  if ( pGmlKlasse == NULL ) return _SONST;

  pGmlAttribut = pGmlKlasse->getGmlAttribut( attributNameINSPIRE );
  if ( pGmlAttribut == NULL ) return _SONST;

  attributTyp = pGmlAttribut->getGmlAttributTyp();

  if ( attributTyp == _DATA_TYPE || attributTyp == _REFERENZ )
    referenzKlassenName = pGmlAttribut->getReferenziertesElement();
  else
    referenzKlassenName = "";

  return attributTyp;
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt ein INSPIRE Attribut auf Basis der Transformations-Parameter und   //
//  des XPlanGML-Attributwertes                                              //
///////////////////////////////////////////////////////////////////////////////
void TransformationINSPIRE_PLU_Basis::setAttributINSPIRE ( Feature * pObjektINSPIRE, INSPIRE_AttributZuordnung * pZuordnung, 
										string attributWertXPlan, string klassenNameInspire, string uom )
{
  string            klassenNameINSPIRE;
  string            attributNameINSPIRE;
  string            attributWertINSPIRE;
  string            referenzKlassenName;
  string            stdString;
  string            codeSpace;
  ATTRIBUT_TYP      attributTypINSPIRE;
  ATTRIBUT_TYP      attributTypZusatzINSPIRE;
  vector<string>    vAttributWerteInspire;
  FeatureRelation * pRelation;
  Feature         * pObjekt;
	size_t            i, anz;

  vector<INSPIRE_AttributZuweisung*> vAttributZusatzINSPIRE;
  INSPIRE_AttributZuweisung        * pAttributZuweisung;

  attributNameINSPIRE = pZuordnung->getAttributNameINSPIRE();
  klassenNameINSPIRE  = pObjektINSPIRE->getKlassenName();
  attributTypINSPIRE = getAttributTypINSPIRE( klassenNameINSPIRE, attributNameINSPIRE, referenzKlassenName );
  if ( klassenNameInspire != "" ) 
    referenzKlassenName = klassenNameInspire;

  pZuordnung->getAttributZusatzINSPIRE( vAttributZusatzINSPIRE );

  if ( attributTypINSPIRE == _DATE_TIME )
  {
    attributWertXPlan = attributWertXPlan + "T00:00:00";
  }

  anz =  pZuordnung->getAttributwerteINSPIRE(  attributWertXPlan, vAttributWerteInspire );
  if ( attributTypINSPIRE != _DATA_TYPE )
  {
    for ( i = 0; i < anz; i++ )
      setAttributINSPIRE( pObjektINSPIRE, attributNameINSPIRE, vAttributWerteInspire[i], attributTypINSPIRE, uom );

    for ( i = 0; i < vAttributZusatzINSPIRE.size(); i++ )
    {
      pAttributZuweisung = vAttributZusatzINSPIRE[i];
      attributTypZusatzINSPIRE = getAttributTypINSPIRE( klassenNameINSPIRE, pAttributZuweisung->attributName, referenzKlassenName );
      setAttributINSPIRE( pObjektINSPIRE, pAttributZuweisung->attributName, pAttributZuweisung->attributWert, 
        attributTypZusatzINSPIRE, uom );
    }
  }
  else
  {
    pObjekt = new Feature ( INSPIRE_PLU_OBJEKT, Constant::createGmlId(), m_pObjekteINSPIRE, referenzKlassenName );
    m_pObjekteINSPIRE->addFeature( pObjekt );

    pRelation                = new FeatureRelation;
    pRelation->rollenName    = attributNameINSPIRE;
    pRelation->featureId = pObjekt->getGmlId();
    pRelation->relToFeature  = false;
    pObjektINSPIRE->addFeatureRelation( pRelation );

    pObjektINSPIRE->addChild( pObjekt->getGmlId() );
    pObjekt->setParentId ( pObjektINSPIRE->getGmlId() );

    attributTypZusatzINSPIRE = getAttributTypINSPIRE( referenzKlassenName, pZuordnung->getReferenzAttributNameINSPIRE(), stdString );
    for ( i = 0; i < anz; i++ )
    {
      if ( attributTypZusatzINSPIRE == _DATE_TIME )
      {
        vAttributWerteInspire[i] = vAttributWerteInspire[i] + "T00:00:00";
      }
      setAttributINSPIRE( pObjekt, pZuordnung->getReferenzAttributNameINSPIRE(), vAttributWerteInspire[i], attributTypZusatzINSPIRE, uom );
    }

    //    setAttributINSPIRE( pObjekt, pZuordnung->getReferenzAttributNameINSPIRE(), attributWertINSPIRE, 
    //                attributTypZusatzINSPIRE, prioritaet );

    for ( i = 0; i < vAttributZusatzINSPIRE.size(); i++ )
    {
      pAttributZuweisung = vAttributZusatzINSPIRE[i];
      attributTypZusatzINSPIRE = getAttributTypINSPIRE( referenzKlassenName, pAttributZuweisung->attributName, stdString );
      setAttributINSPIRE( pObjekt, pAttributZuweisung->attributName, pAttributZuweisung->attributWert, 
        attributTypZusatzINSPIRE, uom );
    }   
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Setzt einen INSPIRE Attributwert                                         //
///////////////////////////////////////////////////////////////////////////////
void TransformationINSPIRE_PLU_Basis::setAttributINSPIRE ( Feature * pObjektINSPIRE, string attributNameINSPIRE, 
						string attributWertINSPIRE, ATTRIBUT_TYP attributTypINSPIRE, string uom )
{
  string   registry;
  string   stdString;
  double   h;
  int      ih;
  bool     b;
  UOM    * pUom;

  switch ( attributTypINSPIRE )
  {
  case _ENUMERATION:
    if ( attributWertINSPIRE == "" ) break;

    pObjektINSPIRE->setStringAttribut( attributNameINSPIRE, attributWertINSPIRE );
    break;

  case _EXTERNAL_CODE_LIST_GML3_3:
    if ( attributWertINSPIRE == "" ) break;

    registry = getRegistry ( attributNameINSPIRE );        
    stdString = registry + attributWertINSPIRE;
    pObjektINSPIRE->setStringAttribut( attributNameINSPIRE, "", "xlink:href", stdString );
    break;

  case _STRING:
    pObjektINSPIRE->setStringAttribut( attributNameINSPIRE, attributWertINSPIRE );
    break;

  case _DATE:
  case _DATE_TIME:
    {
      if ( attributWertINSPIRE == "" ) break;

      TimePosition * pTime = new TimePosition;
      pTime->fromXMLString( attributWertINSPIRE );
      DateAttr * pDateAttr = new DateAttr;
      pDateAttr->value = pTime;
      pObjektINSPIRE->setDateAttribut( attributNameINSPIRE, pDateAttr );
      break;
    }

  case _ANY_URI:
    if ( attributWertINSPIRE == "" ) break;

    pObjektINSPIRE->setURLAttribut( attributNameINSPIRE, attributWertINSPIRE );
    break;

  case _DOUBLE:
    if ( attributWertINSPIRE == "" ) break;

    h = XmlConvert::ToDouble( QuConvert::ToString( attributWertINSPIRE ) );
    pObjektINSPIRE->setDoubleAttribut( attributNameINSPIRE, h );
    break;

  case _INTEGER:
    if ( attributWertINSPIRE == "" ) break;

    ih = XmlConvert::ToInt32( QuConvert::ToString( attributWertINSPIRE ) );
    pObjektINSPIRE->setIntegerAttribut( attributNameINSPIRE, ih );
    break;

  case _BOOLEAN:
    if ( attributWertINSPIRE == "" ) break;

    b = XmlConvert::ToBoolean( QuConvert::ToString( attributWertINSPIRE ) );
    pObjektINSPIRE->setBoolAttribut( attributNameINSPIRE, b );
    break;

  case _ANGLE:
    if ( attributWertINSPIRE == "" ) break;

    h = XmlConvert::ToDouble( QuConvert::ToString( attributWertINSPIRE ) );
    if ( uom != "" )
    {
      pUom = m_pUOMList->getUOMFromIdentifier( uom );
      pObjektINSPIRE->setDoubleAttribut( attributNameINSPIRE, h, pUom );
    }
    else
      pObjektINSPIRE->setDoubleAttribut( attributNameINSPIRE, h, m_pUOMList->getUomAngle() );
    break;

  case _LENGTH:
    if ( attributWertINSPIRE == "" ) break;

    h = XmlConvert::ToDouble( QuConvert::ToString( attributWertINSPIRE ) );
    if ( uom != "" )
    {
      pUom = m_pUOMList->getUOMFromIdentifier( uom );
      pObjektINSPIRE->setDoubleAttribut( attributNameINSPIRE, h, pUom );
    }
    else
      pObjektINSPIRE->setDoubleAttribut( attributNameINSPIRE, h,m_pUOMList->getUomLength() );
    break;

  case _AREA:
    if ( attributWertINSPIRE == "" ) break;

    h = XmlConvert::ToDouble( QuConvert::ToString( attributWertINSPIRE ) );
    if ( uom != "" )
    {
      pUom = m_pUOMList->getUOMFromIdentifier( uom );
      pObjektINSPIRE->setDoubleAttribut( attributNameINSPIRE, h, pUom );
    }
    else
      pObjektINSPIRE->setDoubleAttribut( attributNameINSPIRE, h, m_pUOMList->getUomArea() );
    break;

  case _VOLUME:
    if ( attributWertINSPIRE == "" ) break;

    h = XmlConvert::ToDouble( QuConvert::ToString( attributWertINSPIRE ) );
    if (  uom != "" )
    {
      pUom = m_pUOMList->getUOMFromIdentifier( uom );
      pObjektINSPIRE->setDoubleAttribut( attributNameINSPIRE, h, pUom );
    }
    else
      pObjektINSPIRE->setDoubleAttribut( attributNameINSPIRE, h, m_pUOMList->getUomArea() );
    break;

  case _MEASURE:
    if ( attributWertINSPIRE == "" ) break;

    h = XmlConvert::ToDouble( QuConvert::ToString( attributWertINSPIRE ) );
    pUom = m_pUOMList->getUOMFromIdentifier( uom );
    pObjektINSPIRE->setDoubleAttribut( attributNameINSPIRE, h, pUom );
    break;
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Generiert ein INSPIRE Objekt LegislationCitation												 //
///////////////////////////////////////////////////////////////////////////////
void TransformationINSPIRE_PLU_Basis::generateINSPIRELegislationCitation ( string gesetzLevel, string gesetzDatum,
										string documentName )
{
  string            gmlId;
  vector<string>    nilattributNamen;
  vector<string>    nilAttributWerte;
  Feature         * pDocument;
  Feature         * pLegislationCitation;
  FeatureRelation * pRel;

  nilattributNamen.push_back( "xsi:nil" );
  nilattributNamen.push_back( "nilReason" );

  nilAttributWerte.push_back( "true" );
  nilAttributWerte.push_back( "unknown" );


  gmlId = Constant::createGmlId();
  pDocument  = new Feature ( INSPIRE_PLU_OBJEKT, gmlId, m_pObjekteINSPIRE, "plu:OfficialDocumentation" );
  m_pObjekteINSPIRE->addFeature( pDocument );
  generateINSPIREId( gmlId, pDocument );

  pRel = new FeatureRelation;
  pRel->rollenName = "plu:officialDocument";
  pRel->featureId = pDocument->getGmlId();
  pRel->relToFeature = false;
  m_pPlanINSPIRE->addFeatureRelation ( pRel );

  gmlId = Constant::createGmlId();
  pLegislationCitation = new Feature ( INSPIRE_PLU_OBJEKT, gmlId, m_pObjekteINSPIRE, "base2:LegislationCitation" );
  m_pObjekteINSPIRE->addFeature( pLegislationCitation );
  generateINSPIREId( gmlId, pLegislationCitation );

  pLegislationCitation->setStringAttribut( "base2:name", documentName );
  pLegislationCitation->setURLAttribut( "base2:link", "", nilattributNamen, nilAttributWerte );
  pLegislationCitation->setStringAttribut( "base2:level", "", "xlink:href", gesetzLevel );
  if (  gesetzDatum != "" )
  {
    Feature * pCitationDate = createDocumentDate ( gesetzDatum, "publication");

    pRel = new FeatureRelation;
    pRel->rollenName = "base2:date";
    pRel->featureId = pCitationDate->getGmlId();
    pRel->relToFeature = false;
    pLegislationCitation->addFeatureRelation ( pRel );      

    pLegislationCitation->addChild( pCitationDate->getGmlId() );
    pCitationDate->setParentId( pLegislationCitation->getGmlId() );
  }

  pRel = new FeatureRelation;
  pRel->rollenName = "plu:legislationCitation";
  pRel->featureId = pLegislationCitation->getGmlId();
  pRel->relToFeature = false;
  pDocument->addFeatureRelation ( pRel );

  pDocument->addChild ( pLegislationCitation->getGmlId() );
  pLegislationCitation->setParentId( pDocument->getGmlId() );
}

///////////////////////////////////////////////////////////////////////////////
//  class TransformationINSPIRE_PLU                                          //
//  Transformiert einen XPlanGML 4.0 oder 4.1 Datensatz nach INSPIRE PLU     //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor                                                 //
///////////////////////////////////////////////////////////////////////////////
TransformationINSPIRE_PLU::TransformationINSPIRE_PLU ( Features * pObjekteXPlanGML, Features * pObjekteINSPIREP, 
            UOMList * pUOMList, TransformationINSPIRE_PLU_Parameter * pParameterP): 
TransformationINSPIRE_PLU_Basis ( pObjekteXPlanGML, pObjekteINSPIREP, pUOMList, pParameterP )
{  
}

TransformationINSPIRE_PLU::~TransformationINSPIRE_PLU()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Führt die Transformation durch                                           //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU::transformationToPLU ( BLeitPlan * pPlanP, vector<PlanBereich*> vPlanBereiche,
							TransformationINSPIREProtokoll * pProtokollP )
{
  vector<Feature*> vFeatures;
  Feature        * pPlanObjekt;
	size_t           i, j, anz;
  bool             success, b;
  string           gmlId;
  string           rechtscharakter;
  string           klassenName;
  string           klassenNamePrefix;

  TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt;

  m_pPlanINSPIRE    = NULL;
  m_pProtokoll      = pProtokollP;

  success = transformationPlan ( pPlanP );
  if ( !success ) return false;

  if ( vPlanBereiche.size() == 0 )
    return true;

  for ( i = 0; i < vPlanBereiche.size(); i++ )
  {
    PlanBereich * pPlanBereich = vPlanBereiche[i]; 

    vFeatures.clear();
    anz = pPlanBereich->getRelatedFeatures( "xplan:nachrichtlich", vFeatures );
    for ( j = 0; j < anz; j++ )
    {    
      pPlanObjekt = vFeatures[j];
      if ( pPlanObjekt != NULL ) 
      {
        success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", rechtscharakter );
        if ( !success )
        {
          klassenName = pPlanObjekt->getKlassenName();
          klassenNamePrefix = klassenName.substr( 0, 8 );

          if ( klassenNamePrefix == "xplan:BP" ||
               klassenNamePrefix == "xplan:SO" )
               pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "1000" );
          else
          if ( klassenNamePrefix == "xplan:FP" )
            pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "2000" );
          else
          if ( klassenNamePrefix == "xplan:RP" ||
               klassenNamePrefix == "xplan:LP" )
               pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "3000" );
        }       
      }
    }
  }

  vFeatures.clear();
  for ( i = 0; i < vPlanBereiche.size(); i++ )
  {
    PlanBereich * pPlanBereich = vPlanBereiche[i]; 
    pPlanBereich->getBPlanObjekte( vFeatures, true );
  }

  for ( i = 0; i < vFeatures.size(); i++ )
  {
    pPlanObjekt = vFeatures[i];
    switch ( pPlanObjekt->getObjektTyp() )
    {
    case XP_PUNKT_OBJEKT:
    case XP_LINIEN_OBJEKT:
      success = transformationUeberlagerungsobjekt ( pPlanObjekt );
      break;

    case XP_FLAECHEN_OBJEKT:
      success = pPlanObjekt->getBoolAttributWert( "xplan:flaechenschluss", b );
      if ( success && b )
        success = transformationFlaechenschlussobjekt ( pPlanObjekt );
      else
        success = transformationUeberlagerungsobjekt ( pPlanObjekt );
      break;

    case XP_GEOMETRIE_OBJEKT:
      if ( pPlanObjekt->getGeometrieTyp() == MULTI_POINT ||  pPlanObjekt->getGeometrieTyp() == MULTI_CURVE )
        success = transformationUeberlagerungsobjekt ( pPlanObjekt );
      else
      {
        success = pPlanObjekt->getBoolAttributWert( "xplan:flaechenschluss", b );
        if ( success && b )
          success = transformationFlaechenschlussobjekt ( pPlanObjekt );
        else
          success = transformationUeberlagerungsobjekt ( pPlanObjekt );
      }
      break;

    case RASTER_BASIS:
      transformRasterplanBasis ( pPlanObjekt );
      break;

    default:
      if ( pPlanObjekt->getKlassenName() != "xplan:XP_TextAbschnitt" &&
           pPlanObjekt->getKlassenName() != "xplan:BP_TextAbschnitt" && 
           pPlanObjekt->getKlassenName() != "xplan:FP_TextAbschnitt"  )
      {
        pProtokollObjekt = new TransformationINSPIREProtokollXPlanObjekt;
        m_pProtokoll->vInstanzProtokolle.push_back( pProtokollObjekt );

        pProtokollObjekt->gmlId                    = pPlanObjekt->getGmlId();
        pProtokollObjekt->xplanKlassenName         = pPlanObjekt->getKlassenName();
        pProtokollObjekt->transformationsGrad      = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
        pProtokollObjekt->transformationsHindernis = TransformationINSPIREProtokoll::KLASSE_NICHT_ABBILDBAR;
        pProtokollObjekt->objektTyp                = TransformationINSPIREProtokoll::SONST_OBJEKT ;
      }
    }

  }

  vFeatures.clear();
  anz = m_pObjekteXPlanGML->getAllFeatures ( vFeatures );
  for ( i = 0; i < anz; i++ )
  {
    Feature * pFeature = vFeatures[i];
    if ( pFeature->getKlassenName() == "xplan:XP_TextAbschnitt" ||
         pFeature->getKlassenName() == "xplan:BP_TextAbschnitt" ||
         pFeature->getKlassenName() == "xplan:FP_TextAbschnitt" )
    {
      transformTextlicheFestsetzung( pFeature );
    }
  }

  pProtokollP->generateZusammenfassung();

  vFeatures.clear();
  anz = m_pObjekteINSPIRE->getAllFeatures( vFeatures );
  for ( i = 0; i < anz; i++ )
  {
    Feature * pINSPIREObjekt = vFeatures[i];
    if ( pINSPIREObjekt->getKlassenName() == "plu:ZoningElement" )
      addVoidAttributes ( pINSPIREObjekt, m_pParameter->mVoidAttributeZoningElement );
    else
    if ( pINSPIREObjekt->getKlassenName() == "plu:SupplementaryRegulation" )
      addVoidAttributes ( pINSPIREObjekt, m_pParameter->mVoidAttributeSupplementaryRegulation );
    else
    if ( pINSPIREObjekt->getKlassenName() == "plu:SpatialPlan" )
      addVoidAttributes ( pINSPIREObjekt, m_pParameter->mVoidAttributeSpatialPlan );
    else
    if ( pINSPIREObjekt->getKlassenName() == "base2:DocumentCitation"||
        pINSPIREObjekt->getKlassenName() == "base2:LegislationCitation" )
        addVoidAttributes ( pINSPIREObjekt, m_pParameter->mVoidAttributeCitation );
  }

  return success;
}


///////////////////////////////////////////////////////////////////////////////
//  Transformiert ein Plan-Objekt in ein Objekt plu:SpatialPlan              //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU::transformationPlan ( BLeitPlan * pPlanP )
{
  bool                 success;
  MultiSurface       * pExtent;
	size_t               anzBereiche;
  vector<PlanBereich*> vPlanBereiche;

  TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt = new TransformationINSPIREProtokollXPlanObjekt;
  m_pProtokoll->vInstanzProtokolle.push_back( pProtokollObjekt );

  pProtokollObjekt->gmlId            = pPlanP->getGmlId();
  pProtokollObjekt->xplanKlassenName = pPlanP->getKlassenName();
  pProtokollObjekt->objektTyp        = TransformationINSPIREProtokoll::PLANOBJEKT;

//  pPlanINSPIRE = new GeometryValuePairCoverage ( pObjekteINSPIRE, pPlanP->getGmlId(), getKlassenNameINSPIRE( SPATIAL_PLAN ) );
  m_pPlanINSPIRE = new Feature ( INSPIRE_PLU_OBJEKT, pPlanP->getGmlId(), m_pObjekteINSPIRE, Constant::getKlassenNameINSPIRE( SPATIAL_PLAN ) );

  _Surface * pGeltungsbereich = dynamic_cast<_Surface*>(pPlanP->getGeometry( "xplan:raeumlicherGeltungsbereich", 0 ));
  if ( pGeltungsbereich == NULL )
  {
    pProtokollObjekt->transformationsGrad      = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
    pProtokollObjekt->transformationsHindernis = TransformationINSPIREProtokoll::XPLANOBJEKT_NICHT_KORREKT;
    return false;
  }

  switch ( pGeltungsbereich->getGeometryType() )
  {
  case MULTI_SURFACE:
      pExtent = new MultiSurface ( dynamic_cast<MultiSurface*>(pGeltungsbereich ));
      break;

  case POLYGON:
    {
      pExtent = new MultiSurface ( 2 );
      PolygonGeo * pPoly = new PolygonGeo ( dynamic_cast<PolygonGeo*>(pGeltungsbereich ));
      pExtent->addSurface( pPoly );
      break;
    }

  case SURFACE:
    {
      pExtent = new MultiSurface ( 2 );
      Surface * pSurface = new Surface ( dynamic_cast<Surface*>(pGeltungsbereich ));
      pExtent->addSurface( pSurface );
      break;
    }
  }

  m_pPlanINSPIRE->addGeometryProperty( "plu:extent", pExtent );
  success = transformationAttributeUndRelationen ( pPlanP, m_pPlanINSPIRE, SPATIAL_PLAN, pProtokollObjekt );

  if ( success )
    m_pObjekteINSPIRE->addFeature( m_pPlanINSPIRE );
  else
  {
    delete m_pPlanINSPIRE;
    m_pPlanINSPIRE = NULL;
  }

  if ( !success )
    return false;

  anzBereiche = pPlanP->getPlanBereiche( vPlanBereiche );
  if ( anzBereiche >= 1 )
  {
    pProtokollObjekt->transformationsGrad      = TransformationINSPIREProtokoll::TRANSFORMIERT;
    PlanBereich * pPlanBereich = vPlanBereiche[0];
    transformBereichAttribute ( pPlanP, pPlanBereich, m_pPlanINSPIRE, pProtokollObjekt );
  }
 
  return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Transformiert die Angaben zu gesetzlichen Bestimmungen (z.B. BauNVO, BauGB//
// aus den Bereich-Objekten                                                   //
////////////////////////////////////////////////////////////////////////////////
void TransformationINSPIRE_PLU::transformBereichAttribute ( BLeitPlan * pPlanP, PlanBereich * pPlanBereichP, 
                 Feature * pPlanObjektINSPIRE, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt )
{
  string    gesetz1Text = "";
  string    gesetz1Datum = "";
  string    gesetz1Level = "";
  string    gesetz2Text = "";
  string    gesetz2Datum = "";
  string    gesetz2Level = "";
  string    registry = getRegistry ( "base2:level" );
  string    temp;
  string    gmlId;
  vector<string> nilattributNamen;
  vector<string> nilAttributWerte;
  bool      success;
  Feature * pDocument;

  nilattributNamen.push_back( "xsi:nil" );
  nilattributNamen.push_back( "nilReason" );

  nilAttributWerte.push_back( "true" );
  nilAttributWerte.push_back( "unknown" );

  Feature         * pLegislationCitation;
  FeatureRelation * pRel;

  switch ( pPlanP->getPlanTyp() )
  {
  case BPLAN:
  case FPLAN:
    success = pPlanBereichP->getStringAttributWert( "xplan:versionBauNVO", temp );
    if ( success )
    {
      gesetz1Level  = registry + "#" + "Unknown";

      if ( temp == "1000" )
      {
        gesetz1Text = "Baunutzungsverordnung 1962";
        gesetz1Datum = "1962-08-01";
        gesetz1Level  = registry + "#" + "national";
      }
      else
      if ( temp == "2000" )
      {
        gesetz1Text = "Baunutzungsverordnung 1968";
        gesetz1Datum = "1969-01-01";
        gesetz1Level  = registry + "#" + "national";
      }
      else
      if ( temp == "3000" )
      {
        gesetz1Text = "Baunutzungsverordnung 1977";
        gesetz1Datum = "1977-10-01";
        gesetz1Level  = registry + "#" + "national";
      }
      else
      if ( temp == "4000" )
      {
        gesetz1Text = "Baunutzungsverordnung 1990";
        gesetz1Datum = "1990-01-27";
        gesetz1Level  = registry + "#" + "national";
      }    
    }

    success = pPlanBereichP->getStringAttributWert( "xplan:versionBauGBText", temp );
    if ( success )
    {
      gesetz2Level  = registry + "#" + "national";

      gesetz2Text = temp;
      TimePosition * pTime = (TimePosition*)pPlanBereichP->getDateAttributWert( "xplan:versionBauGB" );
      if ( pTime != NULL )
        gesetz2Datum = pTime->toXMLString();
    }

    break;

  case RPLAN:
    success = pPlanBereichP->getStringAttributWert( "xplan:versionBROGText", temp );
    if ( success )
    {
      gesetz1Level  = registry + "#" + "national";

      gesetz1Text = temp;
      TimePosition * pTime = (TimePosition*)pPlanBereichP->getDateAttributWert( "xplan:versionBROG" );
      if ( pTime != NULL )
        gesetz1Datum = pTime->toXMLString();
    }

    success = pPlanBereichP->getStringAttributWert( "xplan:versionLPLGText", temp );
    if ( success )
    {
      gesetz2Level  = registry + "#" + "sub-national";

      gesetz2Text = temp;
      TimePosition * pTime = (TimePosition*)pPlanBereichP->getDateAttributWert( "xplan:versionLPLG" );
      if ( pTime != NULL )
        gesetz2Datum = pTime->toXMLString();
    }

    break;
  }

  if ( gesetz1Text != "" )
  {
    gmlId = Constant::createGmlId();
    pDocument  = new Feature ( INSPIRE_PLU_OBJEKT,gmlId, m_pObjekteINSPIRE, "plu:OfficialDocumentation" );
    m_pObjekteINSPIRE->addFeature( pDocument );
    generateINSPIREId( gmlId, pDocument );

    pRel = new FeatureRelation;
    pRel->rollenName = "plu:officialDocument";
    pRel->featureId = pDocument->getGmlId();
    pRel->relToFeature = false;
    m_pPlanINSPIRE->addFeatureRelation ( pRel );

    gmlId = Constant::createGmlId();
    pLegislationCitation = new Feature ( INSPIRE_PLU_OBJEKT, gmlId, m_pObjekteINSPIRE, "base2:LegislationCitation" );
    m_pObjekteINSPIRE->addFeature( pLegislationCitation );
    generateINSPIREId( gmlId, pLegislationCitation );

    pLegislationCitation->setStringAttribut( "base2:name", gesetz1Text );
    pLegislationCitation->setURLAttribut( "base2:link", "", nilattributNamen, nilAttributWerte );
    pLegislationCitation->setStringAttribut( "base2:level", "", "xlink:href", gesetz1Level );
    if (  gesetz1Datum != "" )
    {
      Feature * pCitationDate = createDocumentDate ( gesetz1Datum, "publication");

      pRel = new FeatureRelation;
      pRel->rollenName = "base2:date";
      pRel->featureId = pCitationDate->getGmlId();
      pRel->relToFeature = false;
      pLegislationCitation->addFeatureRelation ( pRel );      

      pLegislationCitation->addChild( pCitationDate->getGmlId() );
      pCitationDate->setParentId( pLegislationCitation->getGmlId() );
    }
   
    pRel = new FeatureRelation;
    pRel->rollenName = "plu:legislationCitation";
    pRel->featureId = pLegislationCitation->getGmlId();
    pRel->relToFeature = false;
    pDocument->addFeatureRelation ( pRel );

    pDocument->addChild ( pLegislationCitation->getGmlId() );
    pLegislationCitation->setParentId( pDocument->getGmlId() );
  }

  if ( gesetz2Text != "" )
  {
    gmlId = Constant::createGmlId();
    pDocument  = new Feature ( INSPIRE_PLU_OBJEKT,gmlId, m_pObjekteINSPIRE, "plu:OfficialDocumentation" );
    m_pObjekteINSPIRE->addFeature( pDocument );
    generateINSPIREId( gmlId, pDocument );

    pRel = new FeatureRelation;
    pRel->rollenName = "plu:officialDocument";
    pRel->featureId = pDocument->getGmlId();
    pRel->relToFeature = false;
    m_pPlanINSPIRE->addFeatureRelation ( pRel );

    gmlId = Constant::createGmlId();
    pLegislationCitation = new Feature ( INSPIRE_PLU_OBJEKT, gmlId, m_pObjekteINSPIRE, "base2:LegislationCitation" );
    m_pObjekteINSPIRE->addFeature( pLegislationCitation );
    generateINSPIREId( gmlId, pLegislationCitation );

    pLegislationCitation->setStringAttribut( "base2:name", gesetz2Text );
    pLegislationCitation->setURLAttribut( "base2:link", "", nilattributNamen, nilAttributWerte );
    pLegislationCitation->setStringAttribut( "base2:level", "", "xlink:href", gesetz2Level );
    if (  gesetz1Datum != "" )
    {
      Feature * pCitationDate = createDocumentDate ( gesetz1Datum, "publication");

      pRel = new FeatureRelation;
      pRel->rollenName = "base2:date";
      pRel->featureId = pCitationDate->getGmlId();
      pRel->relToFeature = false;
      pLegislationCitation->addFeatureRelation ( pRel );    

      pLegislationCitation->addChild( pCitationDate->getGmlId() );
      pCitationDate->setParentId( pLegislationCitation->getGmlId() );
    }

    pRel = new FeatureRelation;
    pRel->rollenName = "plu:legislationCitation";
    pRel->featureId = pLegislationCitation->getGmlId();
    pRel->relToFeature = false;
    pDocument->addFeatureRelation ( pRel );

    pDocument->addChild ( pLegislationCitation->getGmlId() );
    pLegislationCitation->setParentId( pDocument->getGmlId() );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert die Attribute und Relationen eines XPlanGML-Objektes       //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU::transformationAttributeUndRelationen ( Feature * pPlanObjektXPlan, 
              Feature * pINSPIREObjekt, INSPIRE_OBJEKT_TYP typ, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt )
{
  vector<ATTRIBUT_TYP>                     vAttributTypen;
  vector<string>                           vAttributNamen;
  vector<string>                           vAttributWerte;
  vector<ATTRIBUT_TYP>                     vINSPIREAttributTypen;
  vector<string>                           vINSPIREAttributNamen;
  vector<FeatureRelation*>                 vRelationen;
  vector<INSPIRE_AttributZuweisung*>       vStandardAttributZuweisung;
  vector<XPlanGML_AttributTransformation*> vAttributTransformation;
  vector<INSPIRE_AttributZuordnung*>       vAttributZuordnung;
  XPlanGML_ClassTransformation           * pClassTransformation;
  XPlanGML_AttributTransformation        * pAttributTransformation;
  INSPIRE_AttributZuordnung              * pAttributZuordnung;
	size_t                                   i, j, anz, anzINSPIRE;
  bool                                     success;
  string                                   attributName, attributWert, attributNameINSPIRE, attributWertINSPIRE;
  string                                   gmlTypName;
  string                                   klassenName;
  string                                   referenzKlassenName;
  string                                   klassenNameInspire;
  string                                   uom;
  FeatureRelation                        * pRelation;
  FeatureRelation                        * pRelationINSPIRE;
  Feature                                * pRefObjekt;
  Feature                                * pRefObjektINSPIRE;
  ATTRIBUT_TYP                             attributTypINSPIRE;
  TransformationINSPIREProtokollXPlanAttribut * pAttributProtikoll;
  vector<UOM*>                            vUOMs;

  klassenName = pPlanObjektXPlan->getKlassenName();
  pClassTransformation = m_pParameter->getXPlanGML_ClassTransformation ( klassenName, typ );
  if ( pClassTransformation == NULL )
  {
    pProtokollObjekt->transformationsGrad      = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
    pProtokollObjekt->transformationsHindernis = TransformationINSPIREProtokoll::KLASSE_NICHT_ABBILDBAR;
    return false;
  }
  if ( pClassTransformation->getWirdErwartet() == false )
  {
    pProtokollObjekt->transformationsGrad      = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
    pProtokollObjekt->transformationsHindernis = TransformationINSPIREProtokoll::KLASSE_NICHT_ERWARTET;
    return false;
  }

  if ( pClassTransformation->getKannAbgebildetWerden() == false )
  {
    pProtokollObjekt->transformationsGrad      = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
    pProtokollObjekt->transformationsHindernis = TransformationINSPIREProtokoll::KLASSE_NICHT_ABBILDBAR;
    return false;
  }

  generateINSPIREId ( pPlanObjektXPlan->getGmlId(), pINSPIREObjekt );

  anz = pClassTransformation->getStandardAttributZuweisung(  vStandardAttributZuweisung );
  for ( i = 0; i < anz; i++ )
  {
    attributNameINSPIRE = vStandardAttributZuweisung[i]->attributName;
    attributWertINSPIRE = vStandardAttributZuweisung[i]->attributWert;
    attributTypINSPIRE = getAttributTypINSPIRE( pINSPIREObjekt->getKlassenName(), attributNameINSPIRE, referenzKlassenName );
    setAttributINSPIRE ( pINSPIREObjekt, attributNameINSPIRE, attributWertINSPIRE, attributTypINSPIRE, "" );
  }

  anz = pClassTransformation->getXPlanGML_AttributTransformationNotwendig ( vAttributTransformation );
  for ( i = 0; i < anz; i++ )
  {
    pAttributTransformation = vAttributTransformation[i];
    attributName            = pAttributTransformation->getAttributNameXPlanGML();
    klassenNameInspire      = pAttributTransformation->getKlassenNameINSPIRE();
    uom                     = pAttributTransformation->getUom();

    success = getAttributwertXPlan ( pPlanObjektXPlan, attributName, attributWert );

    vAttributZuordnung.clear();
    anzINSPIRE = pAttributTransformation->getINSPIRE_AttributZuordnung( vAttributZuordnung );
    for ( j = 0; j < anzINSPIRE; j++ )
    { 
      pAttributZuordnung = vAttributZuordnung[j];
      setAttributINSPIRE ( pINSPIREObjekt, pAttributZuordnung, attributWert, klassenNameInspire, uom );
    }   
  }

  anz = pPlanObjektXPlan->getAttributWerte( vAttributTypen, vAttributNamen, vAttributWerte, vUOMs );
  for ( i = 0; i < anz; i++ )
  {
    attributName = vAttributNamen[i];
    attributWert = vAttributWerte[i];

    pAttributTransformation = pClassTransformation->getXPlanGML_AttributTransformation( attributName );
    if ( pAttributTransformation == NULL  )
    {
      if ( attributName != "xplan:flaechenschluss"                                                      && 
         ( attributName != "xplan:ebene" || ( attributName == "xplan:ebene" && attributWert != "0" ) )  &&
         ( attributName != "xplan:rechtsstand" || ( attributName == "xplan:rechtsstand" && attributWert != "1000" ) ) )
      {
        pAttributProtikoll = new TransformationINSPIREProtokollXPlanAttribut;
        pProtokollObjekt->vAttributProtokolle.push_back( pAttributProtikoll );

        pAttributProtikoll->attributName             = attributName;
        pAttributProtikoll->attributWert             = attributWert;
        pAttributProtikoll->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUT_NICHT_ABBILDBAR;
      }
      continue;
    }

    if ( pAttributTransformation->getIstNotwendig() ) continue;

    klassenNameInspire  = pAttributTransformation->getKlassenNameINSPIRE();
    uom                 = pAttributTransformation->getUom();
    vAttributZuordnung.clear();
    anzINSPIRE = pAttributTransformation->getINSPIRE_AttributZuordnung( vAttributZuordnung );
    for ( j = 0; j < anzINSPIRE; j++ )
    { 
      pAttributZuordnung = vAttributZuordnung[j];
      setAttributINSPIRE ( pINSPIREObjekt, pAttributZuordnung, attributWert, klassenNameInspire, uom );
    }   
  }

  anz = pPlanObjektXPlan->getFeatureRelationen( vRelationen );
  for ( i = 0; i < anz; i++ )
  {
    pRelation = vRelationen[i];
    pRefObjekt = m_pObjekteXPlanGML->getFeature( pRelation->featureId );
    if ( pRefObjekt == NULL )
      continue;
    GmlElement * pGmlElement = m_pGmlObjekteXPlan->getGmlElement( pRefObjekt->getKlassenName() );
    gmlTypName = pGmlElement->getGmlTypeName();

    if ( gmlTypName == "xplan:BP_BereichType"                || 
         gmlTypName == "xplan:FP_BereichType"                || 
         gmlTypName == "xplan:RP_Bereich"                    || 
         gmlTypName == "xplan:LP_Bereich"                    || 
         gmlTypName == "xplan:SO_Bereich"                    ||
         gmlTypName == "xplan:gehoertNachrichtlichZuBereich" ||
         gmlTypName == "xplan:gehoertZuBP_Bereich"           ||
         gmlTypName == "xplan:gehoertZuFP_Bereich"           ||
         gmlTypName == "xplan:gehoertZuRP_Bereich"           ||
         gmlTypName == "xplan:gehoertZuLP_Bereich"           ||
         gmlTypName == "xplan:gehoertZuSO_Bereich"    )
      continue;

    pRefObjektINSPIRE = NULL;
    success           = false;
    if ( gmlTypName == "xplan:XP_TextAbschnittType" || 
         gmlTypName == "xplan:FP_TextAbschnittType" ||
         gmlTypName == "xplan:BP_TextAbschnittType" )
    {
      pRelationINSPIRE = new FeatureRelation;
      pRelationINSPIRE->rollenName = "plu:officialDocument";
      pRelationINSPIRE->featureId = pRefObjekt->getGmlId();
      pRelationINSPIRE->relToFeature = false;
      pINSPIREObjekt->addFeatureRelation ( pRelationINSPIRE );
    }
    else
    if ( gmlTypName == "xplan:XP_ExterneReferenzType" )
    {
        pRefObjektINSPIRE =  transformExterneReferenz ( pRelation->rollenName, pRefObjekt, pINSPIREObjekt, pProtokollObjekt );
    }
    else
    if ( gmlTypName == "xplan:BP_WirksamkeitBedingungType" )
    {
      if ( pRelation->rollenName == "xplan:startBedingung")
      {
        DateAttr * pTime = pRefObjekt->getDateAttributWert( "xplan:datumAbsolut" );
        if ( pTime != NULL )
        {
          DateAttr * pDateAttr = new DateAttr;
          pDateAttr->value = new TimePosition ( (TimePosition*)pTime );

          pINSPIREObjekt->setDateAttribut( "plu:validFrom", pDateAttr );
        }
      }
      else
      if ( pRelation->rollenName == "xplan:endeBedingung")
      {
        DateAttr * pTime = pRefObjekt->getDateAttributWert( "xplan:datumAbsolut" );
        if ( pTime->value != NULL  ) 
        {
          DateAttr * pDateAttr = new DateAttr;
          pDateAttr->value =  new TimePosition ( (TimePosition*)(pTime->value) );
          pINSPIREObjekt->setDateAttribut( "plu:validTo", pDateAttr );
        }
      }
    }

    if ( pRefObjektINSPIRE != NULL )
    {
      m_pObjekteINSPIRE->addFeature( pRefObjektINSPIRE );
      pRelationINSPIRE = new FeatureRelation;

      if (  gmlTypName == "xplan:XP_TextAbschnittType" )
        pRelationINSPIRE->rollenName = "plu:officialDocument";
      else
      if ( gmlTypName == "xplan:XP_ExterneReferenzType" )
      {
        if ( pRelation->rollenName == "xplan:refPlangrundlage" )
          pRelationINSPIRE->rollenName = "plu:backgroundMap";
        else
          pRelationINSPIRE->rollenName = "plu:officialDocument";
      }

      pRelationINSPIRE->featureId = pRefObjektINSPIRE->getGmlId();
      pRelationINSPIRE->relToFeature = true;
      pINSPIREObjekt->addFeatureRelation ( pRelationINSPIRE );

    }
    else
    if ( !success )
    {
      pAttributProtikoll = new TransformationINSPIREProtokollXPlanAttribut;
      pProtokollObjekt->vAttributProtokolle.push_back( pAttributProtikoll );

      pAttributProtikoll->attributName             = pRelation->rollenName;
      pAttributProtikoll->attributWert             = gmlTypName;
      pAttributProtikoll->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUT_NICHT_ABBILDBAR;
    }
  }

  pProtokollObjekt->transformationsGrad = TransformationINSPIREProtokoll::TRANSFORMIERT;

  generateAndCorrectSpecificAttributes ( pPlanObjektXPlan, pINSPIREObjekt );
  
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert ein Objekt RasterplanBasis                                 //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU::transformRasterplanBasis ( Feature * pRasterplan )
{
  Feature                * pDocument;
  Feature                * pRelFeature;
  FeatureRelation        * pRel;
  vector<FeatureRelation*> vObjRelationen;
  unsigned int             i;

  TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt = new TransformationINSPIREProtokollXPlanObjekt;

  pRasterplan->getFeatureRelationen( "xplan:refScan", vObjRelationen );
  for ( i = 0; i < vObjRelationen.size(); i++ )
  {
    pRel = vObjRelationen[i];
    pRelFeature =  m_pObjekteXPlanGML->getFeature( pRel->featureId );

    pDocument = transformExterneReferenz( "xplan:refScan", pRelFeature, m_pPlanINSPIRE, pProtokollObjekt );
    if ( pDocument != NULL )
    {
      m_pObjekteINSPIRE->addFeature( pDocument );

      pRel = new FeatureRelation;
      pRel->rollenName = "plu:officialDocument";
      pRel->featureId = pDocument->getGmlId();
      pRel->relToFeature = true;
      m_pPlanINSPIRE->addFeatureRelation ( pRel );
    }
  }

  vObjRelationen.clear();

  pRasterplan->getFeatureRelationen( "xplan:refText", vObjRelationen );
  for ( i = 0; i < vObjRelationen.size(); i++ )
  {
    pRel = vObjRelationen[i];
    pRelFeature =  m_pObjekteXPlanGML->getFeature( pRel->featureId );

    pDocument = transformExterneReferenz( "xplan:refText", pRelFeature, m_pPlanINSPIRE, pProtokollObjekt );
    if ( pDocument != NULL )
    {
      m_pObjekteINSPIRE->addFeature( pDocument );

      pRel = new FeatureRelation;
      pRel->rollenName = "plu:officialDocument";
      pRel->featureId = pDocument->getGmlId();
      pRel->relToFeature = true;
      m_pPlanINSPIRE->addFeatureRelation ( pRel );
    }
  }

  vObjRelationen.clear();

  pRasterplan->getFeatureRelationen( "xplan:refLegende", vObjRelationen );
  for ( i = 0; i < vObjRelationen.size(); i++ )
  {
    pRel = vObjRelationen[i];
    pRelFeature =  m_pObjekteXPlanGML->getFeature( pRel->featureId );

    pDocument = transformExterneReferenz( "xplan:refLegende", pRelFeature, m_pPlanINSPIRE, pProtokollObjekt );
    if ( pDocument != NULL )
    {
      m_pObjekteINSPIRE->addFeature( pDocument );

      pRel = new FeatureRelation;
      pRel->rollenName = "plu:officialDocument";
      pRel->featureId = pDocument->getGmlId();
      pRel->relToFeature = true;
      m_pPlanINSPIRE->addFeatureRelation ( pRel );
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert bzw. korrigiert INSPIRE-Attribute                              //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU::generateAndCorrectSpecificAttributes ( Feature * pPlanObjektXPlan, Feature * pPlanObjektINSPIRE )
{
  //  Setzen des Attributs beginLifespanVersion
  DateTime  DATE_TIME = DateTime::Now;
  String  ^ dateTime;
  string    s_dateTime;
  string    planArt;
  bool      success;

  dateTime = XmlConvert::ToString ( DATE_TIME, "yyyy-MM-ddTHH:mm:ss" );
  QuConvert::systemStr2stdStr( s_dateTime, dateTime );

  TimePosition * pTime = new TimePosition;
  pTime->fromXMLString ( s_dateTime );
  DateAttr * pDateAttr = new DateAttr;
  pDateAttr->value = pTime;
  pPlanObjektINSPIRE->setDateAttribut( "plu:beginLifespanVersion", pDateAttr );

  //  Korrektur des Attributes regulationNature: Bei Vorhabens- und Erschließungsplänen: bindíngForAuthorities
  PlanBereich * pPlanBereich =  pPlanObjektXPlan->getPlanBereich();
  if ( pPlanBereich != NULL )
  {
    BLeitPlan * pPlan = pPlanBereich->getBPlan();
    if ( pPlan->getPlanTyp() == BPLAN )
    {
      success = pPlan->getStringAttributWert ( "xplan:planArt", planArt );
      if ( success && planArt == "3000" ) 
      {
        pPlanObjektINSPIRE->deleteStringAttribut( "plu:regulationNature" );
        pPlanObjektINSPIRE->setStringAttribut( "plu:regulationNature", "", "xlink:href", "bindingForDevelopers" );
      }
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert ein XPlanGML Flächenschlussobjekt                          //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU::transformationFlaechenschlussobjekt ( Feature * pPlanObjekt )
{
  Feature         * pZoningElement;
  _Geometrie      * pSurface;
  _Surface        * pSurfaceCopy;
  MultiSurface    * pMultiSurface = NULL;
  bool              success;
  FeatureRelation * pRel;

  TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt = new TransformationINSPIREProtokollXPlanObjekt;
  m_pProtokoll->vInstanzProtokolle.push_back( pProtokollObjekt );

  pProtokollObjekt->gmlId            = pPlanObjekt->getGmlId();
  pProtokollObjekt->xplanKlassenName = pPlanObjekt->getKlassenName();
  pProtokollObjekt->objektTyp        = TransformationINSPIREProtokoll::FLAECHENSCHLUSSOBJEKT;

  pZoningElement = new Feature ( INSPIRE_PLU_OBJEKT, pPlanObjekt->getGmlId(), m_pObjekteINSPIRE,
                  Constant::getKlassenNameINSPIRE( ZONING_ELEMENT ) );

  pSurface     = pPlanObjekt->getGeometry( "xplan:position", 0 );
  switch ( pSurface->getGeometryType() )
  {
  case SURFACE:
    {
      pMultiSurface = new MultiSurface( 2 );
      pSurfaceCopy = new Surface ( (Surface*)pSurface );
      pMultiSurface->addSurface ( pSurfaceCopy );
      break;
    }

  case POLYGON:
    {
      pMultiSurface = new MultiSurface( 2 );
      pSurfaceCopy = new PolygonGeo ( (PolygonGeo*)pSurface );
      pMultiSurface->addSurface ( pSurfaceCopy );
      break;
    }

  case MULTI_SURFACE:
    {
      pMultiSurface = new MultiSurface ( (MultiSurface*)pSurface );
      break;
    }

  default:
    return false;

  }
  pZoningElement->addGeometryProperty( "plu:geometry", pMultiSurface );

  success = transformationAttributeUndRelationen( pPlanObjekt, pZoningElement, ZONING_ELEMENT, pProtokollObjekt );
  if ( success )
  {
    m_pObjekteINSPIRE->addFeature( pZoningElement );

    pRel = new FeatureRelation;
    pRel->rollenName = "plu:member";
    pRel->featureId = pZoningElement->getGmlId();
    pRel->relToFeature  = true;
    m_pPlanINSPIRE->addFeatureRelation( pRel );

    pRel = new FeatureRelation;
    pRel->rollenName = "plu:plan";
    pRel->featureId = m_pPlanINSPIRE->getGmlId();
    pRel->relToFeature  = true;
    pZoningElement->addFeatureRelation( pRel );
  }

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert ein XPlanGML Überlagerungsobjekt                           //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU::transformationUeberlagerungsobjekt ( Feature * pPlanObjekt )
{
  Feature          * pSupplementaryRestriction;
  _Geometrie       * pGeometrie;
  _Geometrie       * pGeometrieCopy;
  FeatureRelation  * pRelation;
  bool               success;

  TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt = new TransformationINSPIREProtokollXPlanObjekt;
  m_pProtokoll->vInstanzProtokolle.push_back( pProtokollObjekt );

  pProtokollObjekt->gmlId            = pPlanObjekt->getGmlId();
  pProtokollObjekt->xplanKlassenName = pPlanObjekt->getKlassenName();
  pProtokollObjekt->objektTyp        = TransformationINSPIREProtokoll::UEBERLAGERUNGSOBJEKT;

  pSupplementaryRestriction = new Feature ( INSPIRE_PLU_OBJEKT, pPlanObjekt->getGmlId(), m_pObjekteINSPIRE, 
              Constant::getKlassenNameINSPIRE( SUPPLEMENTARY_RESTRICTION ) );
  pGeometrie = pPlanObjekt->getGeometry( "xplan:position", 0 );

  pGeometrieCopy = _Geometrie::copy ( pGeometrie );
  pSupplementaryRestriction->addGeometryProperty( "plu:geometry", pGeometrieCopy );

  success = transformationAttributeUndRelationen( pPlanObjekt, pSupplementaryRestriction, SUPPLEMENTARY_RESTRICTION, pProtokollObjekt );

  if ( success )
  {
    m_pObjekteINSPIRE->addFeature( pSupplementaryRestriction );

    pRelation = new FeatureRelation;
    pRelation->featureId = pSupplementaryRestriction->getGmlId();
    pRelation->rollenName    = "plu:restriction";
    pRelation->relToFeature  = true;
    m_pPlanINSPIRE->addFeatureRelation( pRelation );

    pRelation = new FeatureRelation;
    pRelation->rollenName = "plu:plan";
    pRelation->featureId = m_pPlanINSPIRE->getGmlId();
    pRelation->relToFeature = true;
    pSupplementaryRestriction->addFeatureRelation ( pRelation );
  }
  else
    delete pSupplementaryRestriction;

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert eine XPlanGML Textliche Festsetzung                        //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU::transformTextlicheFestsetzung ( Feature * pTextAbschnitt )
{
  Feature      * pTextualRegulation;
  string            attributWert;
  bool              success;

  TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt = new TransformationINSPIREProtokollXPlanObjekt;
  m_pProtokoll->vInstanzProtokolle.push_back( pProtokollObjekt );

  pProtokollObjekt->gmlId            = pTextAbschnitt->getGmlId();
  pProtokollObjekt->xplanKlassenName = pTextAbschnitt->getKlassenName();
  pProtokollObjekt->objektTyp        = TransformationINSPIREProtokoll::SONST_OBJEKT;

  success = pTextAbschnitt->getStringAttributWert ( "xplan:text", attributWert );
  if ( !success )
  {
    pProtokollObjekt->transformationsGrad = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
    return false;
  }
  pProtokollObjekt->transformationsGrad = TransformationINSPIREProtokoll::TRANSFORMIERT;

  pTextualRegulation = new Feature ( INSPIRE_PLU_OBJEKT, pTextAbschnitt->getGmlId(), m_pObjekteINSPIRE, "plu:OfficialDocumentation" );
  pTextualRegulation->setStringAttribut( "plu:regulationText", attributWert );

  generateINSPIREId ( pTextAbschnitt->getGmlId(), pTextualRegulation );

  m_pObjekteINSPIRE->addFeature( pTextualRegulation );

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert eine XPlanGML Externe Referenz                             //
///////////////////////////////////////////////////////////////////////////////
Feature * TransformationINSPIRE_PLU::transformExterneReferenz ( string rollenName, Feature * pExterneReferenz, Feature * pParent,
                         TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt )
{
  Feature         * pDocument;
  Feature         * pPlanDocument;
  FeatureRelation * pRel;
  string            referenzName;
  string            referenzURL;
  string            georefURL;
  bool              success;
  string            date;
  string            gmlId;
  string            inspireDocumentName;

  success = pExterneReferenz->getURLAttributWert( "xplan:referenzURL", referenzURL );
  if ( !success ) referenzURL = "";

  success = pExterneReferenz->getURLAttributWert( "xplan:georefURL", georefURL );
  if ( !success ) georefURL = "";

  if ( referenzURL == "" && georefURL == "" )
  {
    TransformationINSPIREProtokollXPlanAttribut * pProtokollObjektAttribut = new TransformationINSPIREProtokollXPlanAttribut;
    pProtokollObjektAttribut->attributName = rollenName;
    pProtokollObjektAttribut->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUTWERT_NICHT_KORREKT;
    pProtokollObjekt->vAttributProtokolle.push_back( pProtokollObjektAttribut );

    return NULL;
  }

  if ( rollenName == "xplan:refPlangrundlage" )
  {
    TimePosition * pTime = (TimePosition*)pExterneReferenz->getDateAttributWert( "xplan:datum" );
    if ( pTime == NULL )
    {
      TransformationINSPIREProtokollXPlanAttribut * pProtokollObjektAttribut = new TransformationINSPIREProtokollXPlanAttribut;
      pProtokollObjektAttribut->attributName = "xplan:refPlangrundlage\\xplan:datum";
      pProtokollObjektAttribut->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUTWERT_NICHT_VORHANDEN;
      pProtokollObjekt->vAttributProtokolle.push_back( pProtokollObjektAttribut );

      return NULL; 
    }

    pDocument = new Feature ( INSPIRE_PLU_OBJEKT, pExterneReferenz->getGmlId(), m_pObjekteINSPIRE, "plu:BackgroundMapValue" );

    pParent->addChild ( pDocument->getGmlId() );
    pDocument->setParentId( pParent->getGmlId() );

    success = pExterneReferenz->getStringAttributWert( "xplan:referenzName", referenzName );
    if ( success )
      pDocument->setStringAttribut ( "plu:backgroundMapReference", referenzName );
  
    date = pTime->toXMLString();
    date = date + "T00:00:00";
    TimePosition * pDateTime = new TimePosition;
    pDateTime->fromXMLString( date );
    DateAttr * pDateAttr = new DateAttr;
    pDateAttr->value = pDateTime;
    pDocument->setDateAttribut ( "plu:backgroundMapDate", pDateAttr );
    
    if ( referenzURL != "" )
      pDocument->setURLAttribut ( "plu:backgroudMapURI", referenzURL );  

    return pDocument;
  }

  inspireDocumentName = m_pParameter->getInspireDocumentName( rollenName );
  if ( inspireDocumentName != "" )
  {
    pDocument  = new Feature ( INSPIRE_PLU_OBJEKT, pExterneReferenz->getGmlId(), m_pObjekteINSPIRE, "plu:OfficialDocumentation" );
    generateINSPIREId( pExterneReferenz->getGmlId(), pDocument );

    gmlId = Constant::createGmlId();
    pPlanDocument = new Feature ( INSPIRE_PLU_OBJEKT, gmlId, m_pObjekteINSPIRE, "base2:DocumentCitation" );
    m_pObjekteINSPIRE->addFeature( pPlanDocument );
    generateINSPIREId( gmlId, pPlanDocument );

    pPlanDocument->setStringAttribut( "base2:name", inspireDocumentName );
    if ( referenzURL != "" )
      pPlanDocument->setURLAttribut( "base2:link", referenzURL );
    if ( georefURL != "" )
      pPlanDocument->setURLAttribut( "base2:link", georefURL );

    pRel = new FeatureRelation;
    pRel->rollenName = "plu:planDocument";
    pRel->featureId = pPlanDocument->getGmlId();
    pRel->relToFeature = false;
    pDocument->addFeatureRelation ( pRel );

    pDocument->addChild ( pPlanDocument->getGmlId() );
    pPlanDocument->setParentId( pDocument->getGmlId() );
  }
  else
  {
    TransformationINSPIREProtokollXPlanAttribut * pProtokollObjektAttribut = new TransformationINSPIREProtokollXPlanAttribut;
    pProtokollObjektAttribut->attributName = rollenName;
    pProtokollObjektAttribut->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUTWERT_NICHT_KORREKT;
    pProtokollObjekt->vAttributProtokolle.push_back( pProtokollObjektAttribut );

    pDocument = NULL;
  }

  return pDocument;
}

///////////////////////////////////////////////////////////////////////////////
//  class TransformationINSPIRE_PLU_V5                                       //
//  Transformiert einen XPlanGML 5.0 Datensatz nach INSPIRE PLU              //
//  Aktuell wird nur das XPlanung-Profil Raster-Umring unterstützt           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor / Destruktor                                                 //
///////////////////////////////////////////////////////////////////////////////
TransformationINSPIRE_PLU_V5::TransformationINSPIRE_PLU_V5 ( Features * pObjekteXPlanGML, Features * pObjekteINSPIREP,
              UOMList * pUOMList, TransformationINSPIRE_PLU_Parameter * pParameterP) :
                  TransformationINSPIRE_PLU_Basis ( pObjekteXPlanGML, pObjekteINSPIREP, pUOMList, pParameterP )
{  
}

TransformationINSPIRE_PLU_V5::~TransformationINSPIRE_PLU_V5()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Führt die Transformation durch																					 //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU_V5::transformationToPLU ( BLeitPlan * pPlanP, vector<PlanBereich*> vPlanBereiche, 
										TransformationINSPIREProtokoll * pProtokollP )
{
  vector<Feature*>         vPlanObjekte;
  vector<FeatureRelation*> vPlanObjRelationen;
	size_t                   i, anz;
  bool                     success;
  string                   gmlId;
  string                   rechtscharakter;
  string                   klassenName;
  string                   klassenNamePrefix;

  m_pPlanINSPIRE    = NULL;
  m_pProtokoll      = pProtokollP;

  success = transformationPlan ( pPlanP );
  if ( !success ) return false;

  anz = m_pObjekteXPlanGML->getAllFeatures ( vPlanObjekte );
  for ( i = 0; i < anz; i++ )
  {
    Feature * pFeature = vPlanObjekte[i];
    if ( pFeature->getKlassenName() == "xplan:XP_TextAbschnitt" ||
      pFeature->getKlassenName() == "xplan:BP_TextAbschnitt" ||
      pFeature->getKlassenName() == "xplan:FP_TextAbschnitt" )
    {
      transformTextlicheFestsetzung( pFeature );
    }   
  }

  if ( vPlanBereiche.size() == 0 )
    return true; 

  pProtokollP->generateZusammenfassung();

  vPlanObjekte.clear();
  anz = m_pObjekteINSPIRE->getAllFeatures( vPlanObjekte );
  for ( i = 0; i < anz; i++ )
  {
    Feature * pINSPIREObjekt = vPlanObjekte[i];
    if ( pINSPIREObjekt->getKlassenName() == "plu:SpatialPlan" )
          addVoidAttributes ( pINSPIREObjekt, m_pParameter->mVoidAttributeSpatialPlan );
    else
    if ( pINSPIREObjekt->getKlassenName() == "base2:DocumentCitation"||
         pINSPIREObjekt->getKlassenName() == "base2:LegislationCitation" )
        addVoidAttributes ( pINSPIREObjekt, m_pParameter->mVoidAttributeCitation );
  }

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert eine Planklasse in das Objekt plu:SpatialPlan              //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU_V5::transformationPlan ( BLeitPlan * pPlanP )
{
  bool                 success;
  MultiSurface       * pExtent;
	size_t               i, anzBereiche, anzRaster;
  vector<PlanBereich*> vPlanBereiche;
  vector<Feature*>     vRasterdarstellung;

  TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt = new TransformationINSPIREProtokollXPlanObjekt;
  m_pProtokoll->vInstanzProtokolle.push_back( pProtokollObjekt );

  pProtokollObjekt->gmlId            = pPlanP->getGmlId();
  pProtokollObjekt->xplanKlassenName = pPlanP->getKlassenName();
  pProtokollObjekt->objektTyp        = TransformationINSPIREProtokoll::PLANOBJEKT;

  m_pPlanINSPIRE = new Feature ( INSPIRE_PLU_OBJEKT, pPlanP->getGmlId(), m_pObjekteINSPIRE, 
                Constant::getKlassenNameINSPIRE( SPATIAL_PLAN ) );

  _Surface * pGeltungsbereich = dynamic_cast<_Surface*>(pPlanP->getGeometry( "xplan:raeumlicherGeltungsbereich", 0 ));
  if ( pGeltungsbereich == NULL )
  {
    pProtokollObjekt->transformationsGrad      = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
    pProtokollObjekt->transformationsHindernis = TransformationINSPIREProtokoll::XPLANOBJEKT_NICHT_KORREKT;
    return false;
  }

  switch ( pGeltungsbereich->getGeometryType() )
  {
  case MULTI_SURFACE:
    pExtent = new MultiSurface ( dynamic_cast<MultiSurface*>(pGeltungsbereich ));
    break;

  case POLYGON:
    {
      pExtent = new MultiSurface ( 2 );
      PolygonGeo * pPoly = new PolygonGeo ( dynamic_cast<PolygonGeo*>(pGeltungsbereich ));
      pExtent->addSurface( pPoly );
      break;
    }

  case SURFACE:
    {
      pExtent = new MultiSurface ( 2 );
      Surface * pSurface = new Surface ( dynamic_cast<Surface*>(pGeltungsbereich ));
      pExtent->addSurface( pSurface );
      break;
    }
  }

  m_pPlanINSPIRE->addGeometryProperty( "plu:extent", pExtent );
  success = transformationAttributeUndRelationen ( pPlanP, m_pPlanINSPIRE, SPATIAL_PLAN, pProtokollObjekt );

  if ( success )
    m_pObjekteINSPIRE->addFeature( m_pPlanINSPIRE );
  else
  {
    delete m_pPlanINSPIRE;
    m_pPlanINSPIRE = NULL;
  }

  if ( !success )
    return false;

  anzBereiche = pPlanP->getPlanBereiche( vPlanBereiche );
  for ( i = 0; i < anzBereiche; i++ )
  {
    PlanBereich * pPlanBereich = vPlanBereiche[i];

    pProtokollObjekt = new TransformationINSPIREProtokollXPlanObjekt;
    m_pProtokoll->vInstanzProtokolle.push_back( pProtokollObjekt );

    pProtokollObjekt->gmlId            = pPlanBereich->getGmlId();
    pProtokollObjekt->xplanKlassenName = pPlanBereich->getKlassenName();
    pProtokollObjekt->objektTyp        = TransformationINSPIREProtokoll::SONST_OBJEKT;


    if ( i == 0 )
    {
      transformBereichAttribute ( pPlanP, pPlanBereich, m_pPlanINSPIRE, pProtokollObjekt );
      pProtokollObjekt->transformationsGrad = TransformationINSPIREProtokoll::TRANSFORMIERT;
    }

    anzRaster = pPlanBereich->getRelatedFeatures( "xplan:rasterBasis", vRasterdarstellung );
    if ( anzRaster > 0 )
      transformRasterdarstellung ( vRasterdarstellung[0] );
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Transformiert die Angaben zu gesetzlichen Bestimmungen (z.B. BauNVO, BauGB//
// aus den Bereich-Objekten                                                   //
////////////////////////////////////////////////////////////////////////////////
void TransformationINSPIRE_PLU_V5::transformBereichAttribute ( BLeitPlan * pPlanP, PlanBereich * pPlanBereichP, 
                           Feature * pPlanObjektINSPIRE, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt )
{
  string     gesetzDatum = "";
  string     gesetzLevel = ""; 
  string     registry = getRegistry ( "base2:level" );
  bool       success;
  DateAttr * pDateAttr;
  string     documentName;

  switch ( pPlanP->getPlanTyp() )
  {
  case BPLAN:
  case FPLAN:
    pDateAttr = pPlanBereichP->getDateAttributWert( "xplan:versionBauNVODatum" );
    success   = pPlanBereichP->getStringAttributWert( "xplan:versionBauNVOText", documentName );

    if ( success || pDateAttr != NULL )
    {
      gesetzLevel  = registry + "#" + "national";
      if ( !success )
        documentName = "Baunutzungsverordnung";
      if ( pDateAttr != NULL )
      {
        TimePosition * pTime = (TimePosition*)(pDateAttr->value);
        gesetzDatum = pTime->toXMLString();
      }
      else
        gesetzDatum = "";

      generateINSPIRELegislationCitation ( gesetzLevel, gesetzDatum, documentName );    
    }

    pDateAttr = pPlanBereichP->getDateAttributWert( "xplan:versionBauGBDatum" );
    success   = pPlanBereichP->getStringAttributWert( "xplan:versionBauGBText", documentName );

    if ( success || pDateAttr != NULL )
    {
      gesetzLevel  = registry + "#" + "national";
      if ( !success )
        documentName = "Baugesetzbuch";
      if ( pDateAttr != NULL )
      {
        TimePosition * pTime = (TimePosition*)(pDateAttr->value);
        gesetzDatum = pTime->toXMLString();
      }
      else
        gesetzDatum = "";

      generateINSPIRELegislationCitation ( gesetzLevel, gesetzDatum, documentName );    
    }
  
    break;
  }  
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert die Attribute und Relationen                               //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU_V5::transformationAttributeUndRelationen ( Feature * pPlanObjektXPlan, 
  Feature * pINSPIREObjekt, INSPIRE_OBJEKT_TYP typ, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt )
{
  vector<ATTRIBUT_TYP>                     vAttributTypen;
  vector<string>                           vAttributNamen;
  vector<string>                           vAttributWerte;
  vector<ATTRIBUT_TYP>                     vINSPIREAttributTypen;
  vector<string>                           vINSPIREAttributNamen;
  vector<FeatureRelation*>                 vRelationen;
  vector<INSPIRE_AttributZuweisung*>       vStandardAttributZuweisung;
  vector<XPlanGML_AttributTransformation*> vAttributTransformation;
  vector<INSPIRE_AttributZuordnung*>       vAttributZuordnung;
  XPlanGML_ClassTransformation           * pClassTransformation;
  XPlanGML_AttributTransformation        * pAttributTransformation;
  INSPIRE_AttributZuordnung              * pAttributZuordnung;
	size_t                                   i, j, anz, anzINSPIRE;
  bool                                     success;
  string                                   attributName, attributWert, attributNameINSPIRE, attributWertINSPIRE;
  string                                   gmlTypName;
  string                                   klassenName;
  string                                   referenzKlassenName;
  string                                   klassenNameInspire;
  string                                   uom;
  FeatureRelation                        * pRelation;
  FeatureRelation                        * pRelationINSPIRE;
  Feature                                * pRefObjekt;
  Feature                                * pRefObjektINSPIRE;
  ATTRIBUT_TYP                             attributTypINSPIRE;
  TransformationINSPIREProtokollXPlanAttribut * pAttributProtikoll;
  vector<UOM*>                            vUOMs;

  klassenName = pPlanObjektXPlan->getKlassenName();
  pClassTransformation = m_pParameter->getXPlanGML_ClassTransformation ( klassenName, typ );
  if ( pClassTransformation == NULL )
  {
    pProtokollObjekt->transformationsGrad      = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
    pProtokollObjekt->transformationsHindernis = TransformationINSPIREProtokoll::KLASSE_NICHT_ABBILDBAR;
    return false;
  }
  if ( pClassTransformation->getWirdErwartet() == false )
  {
    pProtokollObjekt->transformationsGrad      = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
    pProtokollObjekt->transformationsHindernis = TransformationINSPIREProtokoll::KLASSE_NICHT_ERWARTET;
    return false;
  }

  if ( pClassTransformation->getKannAbgebildetWerden() == false )
  {
    pProtokollObjekt->transformationsGrad      = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
    pProtokollObjekt->transformationsHindernis = TransformationINSPIREProtokoll::KLASSE_NICHT_ABBILDBAR;
    return false;
  }

  generateINSPIREId ( pPlanObjektXPlan->getGmlId(), pINSPIREObjekt );

  anz = pClassTransformation->getStandardAttributZuweisung(  vStandardAttributZuweisung );
  for ( i = 0; i < anz; i++ )
  {
    attributNameINSPIRE = vStandardAttributZuweisung[i]->attributName;
    attributWertINSPIRE = vStandardAttributZuweisung[i]->attributWert;
    attributTypINSPIRE = getAttributTypINSPIRE( pINSPIREObjekt->getKlassenName(), attributNameINSPIRE, referenzKlassenName );
    setAttributINSPIRE ( pINSPIREObjekt, attributNameINSPIRE, attributWertINSPIRE, attributTypINSPIRE, "" );
  }

  anz = pClassTransformation->getXPlanGML_AttributTransformationNotwendig ( vAttributTransformation );
  for ( i = 0; i < anz; i++ )
  {
    pAttributTransformation = vAttributTransformation[i];
    attributName            = pAttributTransformation->getAttributNameXPlanGML();
    klassenNameInspire      = pAttributTransformation->getKlassenNameINSPIRE();
    uom                     = pAttributTransformation->getUom();

    success = getAttributwertXPlan ( pPlanObjektXPlan, attributName, attributWert );

    vAttributZuordnung.clear();
    anzINSPIRE = pAttributTransformation->getINSPIRE_AttributZuordnung( vAttributZuordnung );
    for ( j = 0; j < anzINSPIRE; j++ )
    { 
      pAttributZuordnung = vAttributZuordnung[j];
      setAttributINSPIRE ( pINSPIREObjekt, pAttributZuordnung, attributWert, klassenNameInspire, uom );
    }   
  }

  anz = pPlanObjektXPlan->getAttributWerte( vAttributTypen, vAttributNamen, vAttributWerte, vUOMs );
  for ( i = 0; i < anz; i++ )
  {
    attributName = vAttributNamen[i];
    attributWert = vAttributWerte[i];

    pAttributTransformation = pClassTransformation->getXPlanGML_AttributTransformation( attributName );
    if ( pAttributTransformation == NULL  )
    {      
      pAttributProtikoll = new TransformationINSPIREProtokollXPlanAttribut;
      pProtokollObjekt->vAttributProtokolle.push_back( pAttributProtikoll );

      pAttributProtikoll->attributName             = attributName;
      pAttributProtikoll->attributWert             = attributWert;
      pAttributProtikoll->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUT_NICHT_ABBILDBAR;
      continue;
    }

    if ( pAttributTransformation->getIstNotwendig() ) continue;

    klassenNameInspire  = pAttributTransformation->getKlassenNameINSPIRE();
    uom                 = pAttributTransformation->getUom();
    vAttributZuordnung.clear();
    anzINSPIRE = pAttributTransformation->getINSPIRE_AttributZuordnung( vAttributZuordnung );
    for ( j = 0; j < anzINSPIRE; j++ )
    { 
      pAttributZuordnung = vAttributZuordnung[j];
      setAttributINSPIRE ( pINSPIREObjekt, pAttributZuordnung, attributWert, klassenNameInspire, uom );
    }   
  }

  anz = pPlanObjektXPlan->getFeatureRelationen( vRelationen );
  for ( i = 0; i < anz; i++ )
  {
    pRelation = vRelationen[i];
    pRefObjekt = m_pObjekteXPlanGML->getFeature( pRelation->featureId );
    if ( pRefObjekt == NULL )
      continue;

    GmlElement * pGmlElement = m_pGmlObjekteXPlan->getGmlElement( pRefObjekt->getKlassenName() );
    gmlTypName = pGmlElement->getGmlTypeName();

    if ( gmlTypName == "xplan:BP_BereichType"                || 
         gmlTypName == "xplan:FP_BereichType"                || 
         gmlTypName == "xplan:RP_BereichType"                || 
         gmlTypName == "xplan:LP_BereichType"                || 
         gmlTypName == "xplan:SO_BereichType"                ||
         gmlTypName == "xplan:BP_PlanType"                   || 
         gmlTypName == "xplan:FP_PlanType"                   || 
         gmlTypName == "xplan:RP_PlanType"                   || 
         gmlTypName == "xplan:LP_PlanType"                   || 
         gmlTypName == "xplan:SO_PlanType"                    )
      continue;

    pRefObjektINSPIRE = NULL;
    success           = false;
    if ( gmlTypName == "xplan:XP_TextAbschnittType" || 
         gmlTypName == "xplan:FP_TextAbschnittType" ||
         gmlTypName == "xplan:BP_TextAbschnittType" )
    {
      pRelationINSPIRE = new FeatureRelation;
      pRelationINSPIRE->rollenName = "plu:officialDocument";
      pRelationINSPIRE->featureId = pRefObjekt->getGmlId();
      pRelationINSPIRE->relToFeature = false;
      pINSPIREObjekt->addFeatureRelation ( pRelationINSPIRE );
    }
    else
    if ( gmlTypName == "xplan:XP_SpezExterneReferenzType")
    {
      pRefObjektINSPIRE =  transformSpezExterneReferenz ( pRelation->rollenName, pRefObjekt, pINSPIREObjekt, pProtokollObjekt );
    }
    else
    if ( gmlTypName == "xplan:XP_ExterneReferenzType" )
    {
      pRefObjektINSPIRE =  transformExterneReferenz ( pRelation->rollenName, pRefObjekt, pINSPIREObjekt, pProtokollObjekt );
    }
    else
    if ( gmlTypName == "xplan:BP_WirksamkeitBedingungType" )
    {
      if ( pRelation->rollenName == "xplan:startBedingung")
      {
        DateAttr * pTime = pRefObjekt->getDateAttributWert( "xplan:datumAbsolut" );
        if ( pTime != NULL )
        {
          DateAttr * pDateAttr = new DateAttr;
          pDateAttr->value = new TimePosition ( (TimePosition*)pTime );

          pINSPIREObjekt->setDateAttribut( "plu:validFrom", pDateAttr );
        }
      }
      else
      if ( pRelation->rollenName == "xplan:endeBedingung")
      {
        DateAttr * pTime = pRefObjekt->getDateAttributWert( "xplan:datumAbsolut" );
        if ( pTime->value != NULL  ) 
        {
          DateAttr * pDateAttr = new DateAttr;
          pDateAttr->value =  new TimePosition ( (TimePosition*)(pTime->value) );
          pINSPIREObjekt->setDateAttribut( "plu:validTo", pDateAttr );
        }
      }
    }
    else
    if ( ( gmlTypName == "xplan:XP_StringAttributType" || gmlTypName == "xplan:XP_IntegerAttributType" || gmlTypName == "xplan:XP_DoubleAttributType" ) &&
         ( typ == ZONING_ELEMENT || typ == SUPPLEMENTARY_RESTRICTION ) )
    {
      
    }
    else
    if ( gmlTypName == "xplan:XP_DatumAttributType" && typ == SPATIAL_PLAN )
    {
      transformGenerDatumAttribut ( pRefObjekt, pINSPIREObjekt, pProtokollObjekt );
      continue;
    }
    else
    if ( gmlTypName == "xplan:XP_URLAttributType" )
    {
      pRefObjektINSPIRE = transformGenerURLAttribut ( pRefObjekt, pProtokollObjekt );
     
    }

    if ( pRefObjektINSPIRE != NULL )
    {
      m_pObjekteINSPIRE->addFeature( pRefObjektINSPIRE );
      pRelationINSPIRE = new FeatureRelation;

      if (  pRefObjektINSPIRE->getKlassenName() == "plu:OfficialDocumentation" )
        pRelationINSPIRE->rollenName = "plu:officialDocument";
      else
      if ( pRefObjektINSPIRE->getKlassenName() == "plu:BackgroundMapValue" )     
        pRelationINSPIRE->rollenName = "plu:backgroundMap";
      else
      if ( pRefObjektINSPIRE->getKlassenName() == "plu:OrdinanceValue" )     
        pRelationINSPIRE->rollenName = "plu:ordinance";

      
      pRelationINSPIRE->featureId = pRefObjektINSPIRE->getGmlId();
      pRelationINSPIRE->relToFeature = true;
      pINSPIREObjekt->addFeatureRelation ( pRelationINSPIRE );
    }
    else
    if ( !success )
    {
      pAttributProtikoll = new TransformationINSPIREProtokollXPlanAttribut;
      pProtokollObjekt->vAttributProtokolle.push_back( pAttributProtikoll );

      pAttributProtikoll->attributName             = pRelation->rollenName;
      pAttributProtikoll->attributWert             = gmlTypName;
      pAttributProtikoll->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUT_NICHT_ABBILDBAR;
    }
  }

  pProtokollObjekt->transformationsGrad = TransformationINSPIREProtokoll::TRANSFORMIERT;

  generateAndCorrectSpecificAttributes ( pPlanObjektXPlan, pINSPIREObjekt );

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert bzw. korrigiert INSPIRE-Attribute                              //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU_V5::generateAndCorrectSpecificAttributes ( Feature * pPlanObjektXPlan, Feature * pPlanObjektINSPIRE )
{
  //  Setzen des Attribute beginLifespanVersion
  DateTime  DATE_TIME = DateTime::Now;
  String  ^ dateTime;
  string    s_dateTime;
  string    planArt;
  bool      success;

  dateTime = XmlConvert::ToString ( DATE_TIME, "yyyy-MM-ddTHH:mm:ss" );
  QuConvert::systemStr2stdStr( s_dateTime, dateTime );

  TimePosition * pTime = new TimePosition;
  pTime->fromXMLString ( s_dateTime );
  DateAttr * pDateAttr = new DateAttr;
  pDateAttr->value = pTime;
  pPlanObjektINSPIRE->setDateAttribut( "plu:beginLifespanVersion", pDateAttr );

  //  Korrektur des Attributes regulationNature: Bei Vorhabens- und Erschließungsplänen: bindíngForAuthorities
  PlanBereich * pPlanBereich =  pPlanObjektXPlan->getPlanBereich();
  if ( pPlanBereich != NULL )
  {
    BLeitPlan * pPlan = pPlanBereich->getBPlan();
    if ( pPlan->getPlanTyp() == BPLAN )
    {
      success = pPlan->getStringAttributWert ( "xplan:planArt", planArt );
      if ( success && planArt == "3000" ) 
      {
        pPlanObjektINSPIRE->deleteStringAttribut( "plu:regulationNature" );
        pPlanObjektINSPIRE->setStringAttribut( "plu:regulationNature", "", "xlink:href", "bindingForDevelopers" );
      }
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert ein Objekt Rasterdatstellung                               //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU_V5::transformRasterdarstellung ( Feature * pRasterplan )
{
  Feature                * pDocument;
  Feature                * pRelFeature;
  FeatureRelation        * pRel;
  vector<FeatureRelation*> vObjRelationen;
  unsigned int             i;

  TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt = new TransformationINSPIREProtokollXPlanObjekt;
  m_pProtokoll->vInstanzProtokolle.push_back( pProtokollObjekt );

  pProtokollObjekt->gmlId            = pRasterplan->getGmlId();
  pProtokollObjekt->xplanKlassenName = pRasterplan->getKlassenName();
  pProtokollObjekt->objektTyp        = TransformationINSPIREProtokoll::SONST_OBJEKT;

  pRasterplan->getFeatureRelationen( "xplan:refScan", vObjRelationen );
  for ( i = 0; i < vObjRelationen.size(); i++ )
  {
    pRel = vObjRelationen[i];
    pRelFeature =  m_pObjekteXPlanGML->getFeature( pRel->featureId );

    pDocument = transformExterneReferenz( "xplan:refScan", pRelFeature, m_pPlanINSPIRE, pProtokollObjekt );
    if ( pDocument != NULL )
    {
      pProtokollObjekt->transformationsGrad = TransformationINSPIREProtokoll::TRANSFORMIERT;
      m_pObjekteINSPIRE->addFeature( pDocument );

      pRel = new FeatureRelation;
      pRel->rollenName = "plu:officialDocument";
      pRel->featureId = pDocument->getGmlId();
      pRel->relToFeature = true;
      m_pPlanINSPIRE->addFeatureRelation ( pRel );
    }
  }
  
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert eine XPlanGML Textliche Festsetzung                        //
///////////////////////////////////////////////////////////////////////////////
bool TransformationINSPIRE_PLU_V5::transformTextlicheFestsetzung ( Feature * pTextAbschnitt )
{
  Feature        * pTextualRegulation;
  vector<Feature*> vTexte;
  string           attributWert;
  bool             success;
	size_t           anz;

  TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt = new TransformationINSPIREProtokollXPlanObjekt;
  m_pProtokoll->vInstanzProtokolle.push_back( pProtokollObjekt );

  pProtokollObjekt->gmlId            = pTextAbschnitt->getGmlId();
  pProtokollObjekt->xplanKlassenName = pTextAbschnitt->getKlassenName();
  pProtokollObjekt->objektTyp        = TransformationINSPIREProtokoll::SONST_OBJEKT;

  success = pTextAbschnitt->getStringAttributWert ( "xplan:text", attributWert );
  if ( !success )
  {
    anz = pTextAbschnitt->getRelatedFeatures( "xplan:refText", vTexte );
    if ( anz == 0 )
    {
      pProtokollObjekt->transformationsGrad = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
      return false;
    }

    Feature * pDocument = transformExterneReferenz( "xplan:refText", vTexte[0], pTextAbschnitt, pProtokollObjekt );
    if ( pDocument != NULL )
    {
      m_pObjekteINSPIRE->addFeature( pDocument );
      pProtokollObjekt->transformationsGrad = TransformationINSPIREProtokoll::TRANSFORMIERT;
      return true;
   }
  }
  else
  {
    pTextualRegulation = new Feature ( INSPIRE_PLU_OBJEKT, pTextAbschnitt->getGmlId(), m_pObjekteINSPIRE, "plu:OfficialDocumentation" );
    pTextualRegulation->setStringAttribut( "plu:regulationText", attributWert );
    generateINSPIREId ( pTextAbschnitt->getGmlId(), pTextualRegulation );
    m_pObjekteINSPIRE->addFeature( pTextualRegulation );
    pProtokollObjekt->transformationsGrad = TransformationINSPIREProtokoll::TRANSFORMIERT;
    return true;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert eine XPlanGML Externe Referenz                             //
///////////////////////////////////////////////////////////////////////////////
Feature * TransformationINSPIRE_PLU_V5::transformExterneReferenz ( string rollenName, Feature * pExterneReferenz, Feature * pParent,
                TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt )
{
  Feature         * pDocument;
  Feature         * pPlanDocument;
  FeatureRelation * pRel;
  string            referenzName;
  string            referenzURL;
  string            referenzTyp;
  string            georefURL;
  bool              success;
  string            date;
  string            gmlId;
  string            inspireDocumentShortName;
  TimePosition    * pTime;

  success = pExterneReferenz->getURLAttributWert( "xplan:referenzURL", referenzURL );
  if ( !success ) referenzURL = "";

  success = pExterneReferenz->getURLAttributWert( "xplan:georefURL", georefURL );
  if ( !success ) georefURL = "";

  DateAttr * pDateAttr = pExterneReferenz->getDateAttributWert( "xplan:datum" );
  if ( pDateAttr != NULL )
    pTime = (TimePosition*)(pDateAttr->value);
  else
    pTime = NULL;

  inspireDocumentShortName = m_pParameter->getInspireDocumentName( rollenName );
  if ( inspireDocumentShortName != "" )
  {
    pDocument  = new Feature ( INSPIRE_PLU_OBJEKT, pExterneReferenz->getGmlId(), m_pObjekteINSPIRE, "plu:OfficialDocumentation" );
    //    pObjekteINSPIRE->addFeature( pDocument );
    generateINSPIREId( pExterneReferenz->getGmlId(), pDocument );

    gmlId = Constant::createGmlId();
    pPlanDocument = new Feature ( INSPIRE_PLU_OBJEKT, gmlId, m_pObjekteINSPIRE, "base2:DocumentCitation" );
    m_pObjekteINSPIRE->addFeature( pPlanDocument );
    generateINSPIREId( gmlId, pPlanDocument );

    success = pExterneReferenz->getStringAttributWert( "xplan:referenzName", referenzName );
    if ( success )
      pPlanDocument->setStringAttribut( "base2:name", referenzName );
    else
      pPlanDocument->setStringAttribut( "base2:name", inspireDocumentShortName );

    pPlanDocument->setStringAttribut( "base2:shortName", inspireDocumentShortName );
    if ( referenzURL != "" )
      pPlanDocument->setURLAttribut( "base2:link", referenzURL );
    if ( georefURL != "" )
      pPlanDocument->setURLAttribut( "base2:link", georefURL );

    if ( pTime != NULL )
    {
      date = pTime->toXMLString();
      date = date + "T00:00:00";
      TimePosition * pDateTime = new TimePosition;
      pDateTime->fromXMLString( date );
      DateAttr * pDateAttr = new DateAttr;
      pDateAttr->value = pDateTime;
      pPlanDocument->setDateAttribut ( "plu:backgroundMapDate", pDateAttr );
    }

    pRel = new FeatureRelation;
    pRel->rollenName = "plu:planDocument";
    pRel->featureId = pPlanDocument->getGmlId();
    pRel->relToFeature = false;
    pDocument->addFeatureRelation ( pRel );

    pDocument->addChild ( pPlanDocument->getGmlId() );
    pPlanDocument->setParentId( pDocument->getGmlId() );
  }
  else
  {
    TransformationINSPIREProtokollXPlanAttribut * pProtokollObjektAttribut = new TransformationINSPIREProtokollXPlanAttribut;
    pProtokollObjektAttribut->attributName = rollenName;
    pProtokollObjektAttribut->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUTWERT_NICHT_KORREKT;
    pProtokollObjekt->vAttributProtokolle.push_back( pProtokollObjektAttribut );

    pDocument = NULL;
  }

  return pDocument;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert eine XPlanGML Objekt XP_SpezExterneReferenz                //
///////////////////////////////////////////////////////////////////////////////
Feature * TransformationINSPIRE_PLU_V5::transformSpezExterneReferenz ( std::string rollenName, Feature * pExterneReferenz,
					Feature * pParent, TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt )
{
  Feature         * pDocument;
  Feature         * pPlanDocument;
  FeatureRelation * pRel;
  string            referenzName;
  string            referenzURL;
  string            referenzTyp;
  string            georefURL;
  bool              success;
  string            date;
  string            gmlId;
  string            inspireDocumentShortName;
  TimePosition    * pTime;

  success = pExterneReferenz->getStringAttributWert( "xplan:typ", referenzTyp );
  if ( !success )
    return NULL;

  success = pExterneReferenz->getURLAttributWert( "xplan:referenzURL", referenzURL );
  if ( !success ) referenzURL = "";

  success = pExterneReferenz->getURLAttributWert( "xplan:georefURL", georefURL );
  if ( !success ) georefURL = "";

  DateAttr * pDateAttr = pExterneReferenz->getDateAttributWert( "xplan:datum" );
  if ( pDateAttr != NULL )
    pTime = (TimePosition*)(pDateAttr->value);
  else
    pTime = NULL;

  if ( referenzTyp == "1040" )  // Referenz auf Plangrundlage
  {
    if ( pTime == NULL )
    {
      TransformationINSPIREProtokollXPlanAttribut * pProtokollObjektAttribut = new TransformationINSPIREProtokollXPlanAttribut;
      pProtokollObjektAttribut->attributName = "xplan:datum";
      pProtokollObjektAttribut->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUTWERT_NICHT_VORHANDEN;
      pProtokollObjekt->vAttributProtokolle.push_back( pProtokollObjektAttribut );

      return NULL; 
    }

    pDocument = new Feature ( INSPIRE_PLU_OBJEKT, pExterneReferenz->getGmlId(), m_pObjekteINSPIRE, "plu:BackgroundMapValue" );

    pParent->addChild ( pDocument->getGmlId() );
    pDocument->setParentId( pParent->getGmlId() );

    success = pExterneReferenz->getStringAttributWert( "xplan:referenzName", referenzName );
    if ( success )
      pDocument->setStringAttribut ( "plu:backgroundMapReference", referenzName );
    else
      pDocument->setStringAttribut ( "plu:backgroundMapReference", "Plangrundlage" );

    date = pTime->toXMLString();
    date = date + "T00:00:00";
    TimePosition * pDateTime = new TimePosition;
    pDateTime->fromXMLString( date );
    DateAttr * pDateAttr = new DateAttr;
    pDateAttr->value = pDateTime;
    pDocument->setDateAttribut ( "plu:backgroundMapDate", pDateAttr );

    if ( referenzURL != "" )
      pDocument->setURLAttribut ( "plu:backgroudMapURI", referenzURL );  

    return pDocument;
  }

  inspireDocumentShortName = m_pParameter->getInspireDocumentName( referenzTyp );
  if ( inspireDocumentShortName != "" )
  {
    pDocument  = new Feature ( INSPIRE_PLU_OBJEKT, pExterneReferenz->getGmlId(), m_pObjekteINSPIRE, "plu:OfficialDocumentation" );
    generateINSPIREId( pExterneReferenz->getGmlId(), pDocument );

    gmlId = Constant::createGmlId();
    pPlanDocument = new Feature ( INSPIRE_PLU_OBJEKT, gmlId, m_pObjekteINSPIRE, "base2:DocumentCitation" );
    m_pObjekteINSPIRE->addFeature( pPlanDocument );
    generateINSPIREId( gmlId, pPlanDocument );

    success = pExterneReferenz->getStringAttributWert( "xplan:referenzName", referenzName );
    if ( success )
      pPlanDocument->setStringAttribut( "base2:name", referenzName );
    else
      pPlanDocument->setStringAttribut( "base2:name", inspireDocumentShortName );

    pPlanDocument->setStringAttribut( "base2:shortName", inspireDocumentShortName );
    if ( referenzURL != "" )
      pPlanDocument->setURLAttribut( "base2:link", referenzURL );
    if ( georefURL != "" )
      pPlanDocument->setURLAttribut( "base2:link", georefURL );

    if ( pTime != NULL )
    {
      date = pTime->toXMLString();
      date = date + "T00:00:00";
      TimePosition * pDateTime = new TimePosition;
      pDateTime->fromXMLString( date );
      DateAttr * pDateAttr = new DateAttr;
      pDateAttr->value = pDateTime;
      pPlanDocument->setDateAttribut ( "plu:backgroundMapDate", pDateAttr );
    }

    pRel = new FeatureRelation;
    pRel->rollenName = "plu:planDocument";
    pRel->featureId = pPlanDocument->getGmlId();
    pRel->relToFeature = false;
    pDocument->addFeatureRelation ( pRel );

    pDocument->addChild ( pPlanDocument->getGmlId() );
    pPlanDocument->setParentId( pDocument->getGmlId() );
  }
  else
  {
    TransformationINSPIREProtokollXPlanAttribut * pProtokollObjektAttribut = new TransformationINSPIREProtokollXPlanAttribut;
    pProtokollObjektAttribut->attributName = rollenName;
    pProtokollObjektAttribut->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUTWERT_NICHT_KORREKT;
    pProtokollObjekt->vAttributProtokolle.push_back( pProtokollObjektAttribut );

    pDocument = NULL;
  }

  return pDocument;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert ein generisches Datum-Attribut						                 //
///////////////////////////////////////////////////////////////////////////////
Feature * TransformationINSPIRE_PLU_V5::transformGenerDatumAttribut ( Feature * pDatumAttribut, Feature * pPlanObjektINSPIRE, 
                TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt )
{
  bool       success;
  string     attributName;
  DateAttr * pDateAttr;

  success = pDatumAttribut->getStringAttributWert( "xplan:name", attributName );
  if ( !success )
  {
    TransformationINSPIREProtokollXPlanAttribut * pProtokollObjektAttribut = new TransformationINSPIREProtokollXPlanAttribut;
    pProtokollObjektAttribut->attributName = "xplan:name";
    pProtokollObjektAttribut->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUTWERT_NICHT_VORHANDEN;
    pProtokollObjekt->vAttributProtokolle.push_back( pProtokollObjektAttribut );

    return NULL;
  }

  pDateAttr = pDatumAttribut->getDateAttributWert( "xplan:wert" );
  if ( pDateAttr == NULL )
  {
    TransformationINSPIREProtokollXPlanAttribut * pProtokollObjektAttribut = new TransformationINSPIREProtokollXPlanAttribut;
    pProtokollObjektAttribut->attributName = "xplan:wert";
    pProtokollObjektAttribut->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUTWERT_NICHT_VORHANDEN;
    pProtokollObjekt->vAttributProtokolle.push_back( pProtokollObjektAttribut );

    return NULL;
  }

  Feature * pFeature  = new Feature ( INSPIRE_PLU_OBJEKT, pDatumAttribut->getGmlId(), m_pObjekteINSPIRE, "plu:OrdinanceValue" );
  m_pObjekteINSPIRE->addFeature( pFeature );
 
  pFeature->setStringAttribut( "plu:ordinanceReference", attributName );
  
  DateAttr * pDateAttrCopy = new DateAttr;
  TimePosition * pTimePosition = new TimePosition ( (TimePosition*)(pDateAttr->value) );
  pTimePosition->setDefaultTime();
  pDateAttrCopy->value = pTimePosition;
  pFeature->setDateAttribut( "plu:ordinanceDate", pDateAttrCopy );

  FeatureRelation * pRel = new FeatureRelation;
  pRel->rollenName = "plu:ordinance";
  pRel->featureId = pFeature->getGmlId();
  pRel->relToFeature = false;
  pPlanObjektINSPIRE->addFeatureRelation ( pRel );

  pPlanObjektINSPIRE->addChild ( pFeature->getGmlId() );
  pFeature->setParentId( pPlanObjektINSPIRE->getGmlId() );

  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert ein generisches URL-Attribut  						                 //
///////////////////////////////////////////////////////////////////////////////
Feature * TransformationINSPIRE_PLU_V5::transformGenerURLAttribut ( Feature * pURLAttribut,  TransformationINSPIREProtokollXPlanObjekt * pProtokollObjekt )
{
  bool    success;
  string  attributName;
  string  url;

  success = pURLAttribut->getStringAttributWert( "xplan:name", attributName );
  if ( !success )
  {
    TransformationINSPIREProtokollXPlanAttribut * pProtokollObjektAttribut = new TransformationINSPIREProtokollXPlanAttribut;
    pProtokollObjektAttribut->attributName = "xplan:name";
    pProtokollObjektAttribut->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUTWERT_NICHT_VORHANDEN;
    pProtokollObjekt->vAttributProtokolle.push_back( pProtokollObjektAttribut );

    return NULL;
  }

  success = pURLAttribut->getURLAttributWert( "xplan:wert", url );
  if (!success )
  {
    TransformationINSPIREProtokollXPlanAttribut * pProtokollObjektAttribut = new TransformationINSPIREProtokollXPlanAttribut;
    pProtokollObjektAttribut->attributName = "xplan:wert";
    pProtokollObjektAttribut->transformationsHindernis = TransformationINSPIREProtokoll::ATTRIBUTWERT_NICHT_VORHANDEN;
    pProtokollObjekt->vAttributProtokolle.push_back( pProtokollObjektAttribut );

    return NULL;
  }

  Feature * pDocument  = new Feature ( INSPIRE_PLU_OBJEKT, pURLAttribut->getGmlId(), m_pObjekteINSPIRE, "plu:OfficialDocumentation" );
  generateINSPIREId( pURLAttribut->getGmlId(), pDocument );

  string gmlId = Constant::createGmlId();
  Feature * pPlanDocument = new Feature ( INSPIRE_PLU_OBJEKT, gmlId, m_pObjekteINSPIRE, "base2:DocumentCitation" );
  m_pObjekteINSPIRE->addFeature( pPlanDocument );
  generateINSPIREId( gmlId, pPlanDocument );

  pPlanDocument->setStringAttribut( "base2:name", attributName );
  pPlanDocument->setStringAttribut( "base2:shortName", "URLAttribut" );
  pPlanDocument->setURLAttribut( "base2:link", url );

  FeatureRelation * pRel = new FeatureRelation;
  pRel->rollenName = "plu:planDocument";
  pRel->featureId = pPlanDocument->getGmlId();
  pRel->relToFeature = false;
  pDocument->addFeatureRelation ( pRel );

  pDocument->addChild ( pPlanDocument->getGmlId() );
  pPlanDocument->setParentId( pDocument->getGmlId() );

  return pDocument;
}

///////////////////////////////////////////////////////////////////////////////
//  XPlanGML_INSPIRE_Transformation                                          //
//  Parameter zur Steuerung der Transformation XPlanGML -> INSPIRE PLU       //
///////////////////////////////////////////////////////////////////////////////
TransformationINSPIRE_PLU_Parameter::TransformationINSPIRE_PLU_Parameter ()
{
  mVoidAttributeSpatialPlan["plu:alternativeTitle"]     = STRING;
  mVoidAttributeSpatialPlan["plu:processStepGeneral"]   = STRING;
  mVoidAttributeSpatialPlan["plu:backgroundMap"]        = RELATION;
  mVoidAttributeSpatialPlan["plu:ordinance"]            = RELATION;
  mVoidAttributeSpatialPlan["plu:beginLifespanVersion"] = DATE;
  mVoidAttributeSpatialPlan["plu:endLifespanVersion"]   = DATE;
  mVoidAttributeSpatialPlan["plu:officialDocument"]     = RELATION;

  mVoidAttributeZoningElement["plu:hilucsPresence"]       = RELATION;
  mVoidAttributeZoningElement["plu:specificLandUse"]      = STRING;    
  mVoidAttributeZoningElement["plu:specificPresence"]     = RELATION;
  mVoidAttributeZoningElement["plu:processStepGeneral"]   = STRING;
  mVoidAttributeZoningElement["plu:backgroundMap"]        = RELATION;
  mVoidAttributeZoningElement["plu:beginLifespanVersion"] = DATE;
  mVoidAttributeZoningElement["plu:endLifespanVersion"]   = DATE;
  mVoidAttributeZoningElement["plu:officialDocument"]     = RELATION;

  mVoidAttributeSupplementaryRegulation["plu:specificSupplementaryRegulation"] = STRING;
  mVoidAttributeSupplementaryRegulation["plu:processStepGeneral"]              = STRING;
  mVoidAttributeSupplementaryRegulation["plu:backgroundMap"]                   = RELATION;
  mVoidAttributeSupplementaryRegulation["plu:inheritedFromOtherPlans"]         = BOOLEAN;
  mVoidAttributeSupplementaryRegulation["plu:specificRegulationNature"]        = STRING;
  mVoidAttributeSupplementaryRegulation["plu:beginLefespanVersion"]            = DATE;
  mVoidAttributeSupplementaryRegulation["plu:endLefespanVersion"]              = DATE;
  mVoidAttributeSupplementaryRegulation["plu:officialDocument"]                = RELATION;

  mVoidAttributeCitation["base2:date"] = RELATION;
}

TransformationINSPIRE_PLU_Parameter::~TransformationINSPIRE_PLU_Parameter()
{
  map<string,XPlanGML_ClassTransformation*>::iterator iter;

  for ( iter = mPlanklassen.begin(); iter != mPlanklassen.end(); iter++ )
    delete iter->second;

  for ( iter = mFlaechenschluss.begin(); iter != mFlaechenschluss.end(); iter++ )
    delete iter->second;


  for ( iter = mUeberlagerung.begin(); iter != mUeberlagerung.end(); iter++ )
    delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
// Fügt ein XPlanGML-Transformationsobjekt hinzu														 //
///////////////////////////////////////////////////////////////////////////////
void TransformationINSPIRE_PLU_Parameter::addXPlanGML_ClassTransformation ( XPlanGML_ClassTransformation * pTransformation, INSPIRE_OBJEKT_TYP typ )
{
  string klassenNameXPlan = pTransformation->getKlassenNameXPlanGML();

  switch ( typ )
  {
  case SPATIAL_PLAN:
    mPlanklassen[klassenNameXPlan]= pTransformation;
    break;

  case ZONING_ELEMENT:
    mFlaechenschluss[klassenNameXPlan]= pTransformation;
    break;

  case SUPPLEMENTARY_RESTRICTION:
    mUeberlagerung[klassenNameXPlan]= pTransformation;
    break;
  }
}

///////////////////////////////////////////////////////////////////////////////
// Liefert ein XPlanGML-Transformationsobjekt																 //
///////////////////////////////////////////////////////////////////////////////
XPlanGML_ClassTransformation * TransformationINSPIRE_PLU_Parameter::getXPlanGML_ClassTransformation ( string klassenNameXPlanGML, INSPIRE_OBJEKT_TYP typ )
{
  map<string,XPlanGML_ClassTransformation*>::iterator iter;

  switch ( typ )
  {
  case SPATIAL_PLAN:
    iter = mPlanklassen.find( klassenNameXPlanGML );
    if ( iter != mPlanklassen.end() ) return iter->second;
    break;

  case ZONING_ELEMENT:
    iter = mFlaechenschluss.find( klassenNameXPlanGML );
    if ( iter != mFlaechenschluss.end() ) return iter->second;
    break;

  case SUPPLEMENTARY_RESTRICTION:
    iter = mUeberlagerung.find( klassenNameXPlanGML );
    if ( iter != mUeberlagerung.end() ) return iter->second;
    break;
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Liefert alle XPlanGML-Transformationsobjekte															 //
///////////////////////////////////////////////////////////////////////////////
size_t TransformationINSPIRE_PLU_Parameter::getXPlanGML_ClassTransformation ( vector<XPlanGML_ClassTransformation*> &vClassTransform, INSPIRE_OBJEKT_TYP typ )
{
  map<string,XPlanGML_ClassTransformation*>::iterator iter;

  switch ( typ )
  {
  case ZONING_ELEMENT:
    for (  iter = mFlaechenschluss.begin(); iter != mFlaechenschluss.end(); iter++ )
      vClassTransform.push_back( iter->second );
    return vClassTransform.size();

  case SUPPLEMENTARY_RESTRICTION:
    for (  iter = mUeberlagerung.begin(); iter != mUeberlagerung.end(); iter++ )
      vClassTransform.push_back( iter->second );
    return vClassTransform.size();

  case SPATIAL_PLAN:
    for (  iter = mPlanklassen.begin(); iter != mPlanklassen.end(); iter++ )
      vClassTransform.push_back( iter->second );
    return vClassTransform.size();
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Fügt eine INSPIRE DocumentCitation Transformation hinzu									 // 
///////////////////////////////////////////////////////////////////////////////
void TransformationINSPIRE_PLU_Parameter::addDocumentTypeTranslation ( string xplanProperty, string inspireDocumentName )
{
  mDocumentTypes[xplanProperty] = inspireDocumentName;
}

///////////////////////////////////////////////////////////////////////////////
// Liefert einen INSPIRE Document Name																			 // 
///////////////////////////////////////////////////////////////////////////////
std::string TransformationINSPIRE_PLU_Parameter::getInspireDocumentName ( std::string xplanProperty )
{
  if ( mDocumentTypes.find ( xplanProperty ) != mDocumentTypes.end() )
    return mDocumentTypes[xplanProperty];
  else
    return "";
}

///////////////////////////////////////////////////////////////////////////////
//  XPlanGML_ClassTransformation                                             //
//  Parameter zur Transformation einer XPlanGML 4.0 Klasse nach INSPIRE PLU  //
///////////////////////////////////////////////////////////////////////////////
XPlanGML_ClassTransformation::XPlanGML_ClassTransformation()
{
  klassenNameXPlanGML  = "";
  kannAbgebildetWerden = true;
  wirdErwartet         = true;;
}

XPlanGML_ClassTransformation::~XPlanGML_ClassTransformation()
{
  map<string,XPlanGML_AttributTransformation*>::iterator iter;
  unsigned int                                           i;

  for ( i = 0; i < standardAttributZuweisung.size(); i++ )
    delete standardAttributZuweisung[i];

  for ( iter = mAttributTransformation.begin(); iter != mAttributTransformation.end(); iter++ )
    delete iter->second;
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Standard-Attributzuweisung																	   //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML_ClassTransformation::addStandardAttributZuweisung ( INSPIRE_AttributZuweisung *pZuweisung )
{
  standardAttributZuweisung.push_back( pZuweisung );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Standard Attribut-Zuweisungen  													   //
///////////////////////////////////////////////////////////////////////////////
size_t XPlanGML_ClassTransformation::getStandardAttributZuweisung ( std::vector<INSPIRE_AttributZuweisung*> &vZuweisung )
{
	size_t i;

  for ( i = 0; i < standardAttributZuweisung.size(); i++ )
    vZuweisung.push_back( standardAttributZuweisung[i] );

  return standardAttributZuweisung.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine Attribut-Abbildung hinzu																		   //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML_ClassTransformation::addXPlanGML_AttributTransformation ( XPlanGML_AttributTransformation * pTransform )
{
  string attributNameXPlan = pTransform->getAttributNameXPlanGML();

  mAttributTransformation[attributNameXPlan] = pTransform;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Attribut-Abbildungen																			   //
///////////////////////////////////////////////////////////////////////////////
size_t XPlanGML_ClassTransformation::getXPlanGML_AttributTransformation ( std::vector<XPlanGML_AttributTransformation*> &vAttributTransform )
{
  map<string,XPlanGML_AttributTransformation*>::iterator iter;

  for ( iter = mAttributTransformation.begin(); iter != mAttributTransformation.end(); iter++ )
    vAttributTransform.push_back( iter->second );

  return mAttributTransformation.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle notwendigen Attribut-Abbildungen													   //
///////////////////////////////////////////////////////////////////////////////
size_t XPlanGML_ClassTransformation::getXPlanGML_AttributTransformationNotwendig ( std::vector<XPlanGML_AttributTransformation*> &vAttributTransform )
{
  bool                                                   notwendig;
  map<string,XPlanGML_AttributTransformation*>::iterator iter;

  for ( iter = mAttributTransformation.begin(); iter != mAttributTransformation.end(); iter++ )
  {
    notwendig = iter->second->getIstNotwendig();
    if ( notwendig )
      vAttributTransform.push_back( iter->second );
  }

  return vAttributTransform.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine Attribut-Abbildung																				   //
///////////////////////////////////////////////////////////////////////////////
XPlanGML_AttributTransformation * XPlanGML_ClassTransformation::getXPlanGML_AttributTransformation ( string attributNameXPlanGML )
{
  map<string,XPlanGML_AttributTransformation*>::iterator iter;

  iter = mAttributTransformation.find( attributNameXPlanGML );
  if ( iter != mAttributTransformation.end() ) 
    return iter->second;
  else
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  XPlanGML_AttributTransformation                                          //
//  Abbildung eines XPlanGML-Attributs auf ein oder mehrere INSPIRE PLU      //
//  Attribute                                                                //
///////////////////////////////////////////////////////////////////////////////
XPlanGML_AttributTransformation::XPlanGML_AttributTransformation()
{
  attributNameXPlanGML = "";
  klassenNameINSPIRE   = "";
  uom                  = "";
  kannAbgebildetWerden = true;
  istNotwendig         = false;
}

XPlanGML_AttributTransformation::XPlanGML_AttributTransformation( XPlanGML_AttributTransformation * pTrans )
{
  unsigned int                i;
  INSPIRE_AttributZuordnung * pZuordnung;

  attributNameXPlanGML = pTrans->attributNameXPlanGML;
  klassenNameINSPIRE   = pTrans->klassenNameINSPIRE;
  uom                  = pTrans->uom;
  kannAbgebildetWerden = pTrans->kannAbgebildetWerden;
  istNotwendig         = pTrans->istNotwendig;

  for ( i = 0; i < pTrans->vAttributZuordnung.size(); i++ )
  {
    pZuordnung =  pTrans->vAttributZuordnung[i];
    vAttributZuordnung.push_back( new INSPIRE_AttributZuordnung ( pZuordnung ) );
  }

}


XPlanGML_AttributTransformation::~XPlanGML_AttributTransformation()
{
  unsigned int i;

  for ( i = 0; i < vAttributZuordnung.size(); i++ )
    delete vAttributZuordnung[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine INSPIRE Attribut-Zuordnung hinzu															 //
///////////////////////////////////////////////////////////////////////////////
void XPlanGML_AttributTransformation::addINSPIRE_AttributZuordnung ( INSPIRE_AttributZuordnung * pZoordnung )
{
  vAttributZuordnung.push_back(  pZoordnung );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert eine INSPIRE Attribut-Zuordnung																	 //
///////////////////////////////////////////////////////////////////////////////
size_t XPlanGML_AttributTransformation::getINSPIRE_AttributZuordnung ( std::vector<INSPIRE_AttributZuordnung*> &vZuordnung )
{
  unsigned int i;

  for ( i = 0; i < vAttributZuordnung.size(); i++ )
    vZuordnung.push_back( vAttributZuordnung[i] );

  return vAttributZuordnung.size();
}

///////////////////////////////////////////////////////////////////////////////
//  INSPIRE_AttributZuordnung                                                //
//  Parameter zur Abbildung von XPlanGML Attributwerten aus INSPIRE PLU      //
//  Attributwerte                                                            //
///////////////////////////////////////////////////////////////////////////////
INSPIRE_AttributZuordnung::INSPIRE_AttributZuordnung()
{
  attributNameINSPIRE         = "";
  referenzAttributNameINSPIRE = "";
}

INSPIRE_AttributZuordnung::INSPIRE_AttributZuordnung( INSPIRE_AttributZuordnung * pZuordnung )
{ 
  unsigned int                      i;
  multimap<string,string>::iterator iter;
  INSPIRE_AttributZuweisung       * pZuweisung;

  attributNameINSPIRE         = pZuordnung->attributNameINSPIRE;
  referenzAttributNameINSPIRE = pZuordnung->referenzAttributNameINSPIRE;
  for ( i = 0; i < pZuordnung->vStandardAttributWerteINSPIRE.size(); i++ )
    vStandardAttributWerteINSPIRE.push_back( pZuordnung->vStandardAttributWerteINSPIRE[i] );

  for ( i = 0; i <  pZuordnung->vAttributZusatzINSPIRE.size(); i++ )
  {
    pZuweisung = pZuordnung->vAttributZusatzINSPIRE[i];
    vAttributZusatzINSPIRE.push_back( new INSPIRE_AttributZuweisung ( pZuweisung ) );
  }

  for ( iter = pZuordnung->mAttributwertZuordnung.begin(); iter != pZuordnung->mAttributwertZuordnung.end(); iter++ )
  {
    mAttributwertZuordnung.insert (  multimap<string,string>::value_type ( iter->first, iter->second ) );
  }

}


INSPIRE_AttributZuordnung::~INSPIRE_AttributZuordnung()
{
  unsigned int i;

  for ( i = 0; i < vAttributZusatzINSPIRE.size(); i++ )
    delete vAttributZusatzINSPIRE[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt eine INSPIRE Attribut-Zuweisung als Objekt hinzu										 //
///////////////////////////////////////////////////////////////////////////////
void INSPIRE_AttributZuordnung::addAttributZusatzINSPIRE ( INSPIRE_AttributZuweisung * pZuweisung )
{
  vAttributZusatzINSPIRE.push_back( pZuweisung );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle INSPIRE Attribut-Zuweisungen  															 //
///////////////////////////////////////////////////////////////////////////////
size_t INSPIRE_AttributZuordnung::getAttributZusatzINSPIRE ( vector<INSPIRE_AttributZuweisung*> & vZuweisung )
{
  unsigned int i;

  for ( i = 0; i < vAttributZusatzINSPIRE.size(); i++ )
    vZuweisung.push_back( vAttributZusatzINSPIRE[i] );

  return vAttributZusatzINSPIRE.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Paar XPlanGML-Attributwert / INSPIRE Attributwert hinzu				 //
///////////////////////////////////////////////////////////////////////////////
void INSPIRE_AttributZuordnung::addAttributwertZuordnung ( string attributwertXPlan, string attributwertINSPIRE )
{
  mAttributwertZuordnung.insert ( multimap<string,string>::value_type ( attributwertXPlan, attributwertINSPIRE ));
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert zu einem XPlanGML-Attributwert alle zugeordneten INSPIRE				//
//  Attributwerte																														 //
///////////////////////////////////////////////////////////////////////////////
size_t INSPIRE_AttributZuordnung::getAttributwerteINSPIRE ( string attributwertXPlan, std::vector<std::string> &vAttributWerteInspire )
{
  multimap<string,string>::iterator iter;
  unsigned int                      i;

  for ( iter = mAttributwertZuordnung.lower_bound( attributwertXPlan ); iter != mAttributwertZuordnung.upper_bound( attributwertXPlan ); iter++ )
    vAttributWerteInspire.push_back( iter->second );

  if ( vAttributWerteInspire.size() == 0 && vStandardAttributWerteINSPIRE.size() > 0 )
  {
    for ( i = 0; i < vStandardAttributWerteINSPIRE.size(); i++ )
      vAttributWerteInspire.push_back( vStandardAttributWerteINSPIRE[i] );
  }

  if ( vAttributWerteInspire.size() == 0 )
    vAttributWerteInspire.push_back( attributwertXPlan );

  return vAttributWerteInspire.size();
}

///////////////////////////////////////////////////////////////////////////////
//  INSPIRE_AttributZuweisung																								 //
//  Name und Wert eines Zusatzes zu einem INSPIRE Attribut                   //
///////////////////////////////////////////////////////////////////////////////
INSPIRE_AttributZuweisung::INSPIRE_AttributZuweisung()
{

}

INSPIRE_AttributZuweisung::INSPIRE_AttributZuweisung( INSPIRE_AttributZuweisung * pZuweisung )
{
  attributName = pZuweisung->attributName;
  attributWert = pZuweisung->attributWert;
}


INSPIRE_AttributZuweisung::~INSPIRE_AttributZuweisung()
{

}


///////////////////////////////////////////////////////////////////////////////
//  XPlanGML_INSPIRE_Transformation_Reader                                   //
//  Einlesen der Transformationsparameter von einem XML-File                 //
///////////////////////////////////////////////////////////////////////////////
XPlanGML_INSPIRE_Transformation_Reader::XPlanGML_INSPIRE_Transformation_Reader( TransformationINSPIRE_PLU_Parameter * pTransformP )
{
  pTransform = pTransformP;
}
XPlanGML_INSPIRE_Transformation_Reader::~XPlanGML_INSPIRE_Transformation_Reader( )
{
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Abbildungsvorschriften																			 //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::read( string fileName )
{
  String ^ fName;
  bool     first = true;
  bool     success = true;

  fName  = QuConvert::ToString( fileName );
  reader = gcnew XmlTextReader( fName );

  while( reader->Read() )
  {
    pActElement = reader->LocalName;

    if ( reader->NodeType == XmlNodeType::Element )
    {
      if ( first )
      {
        if ( pActElement->CompareTo ( "XPlanGMLTransformation" ) != 0 )
        {
          success = false;
          break;
        }

        first = false;
      }

      if ( pActElement->CompareTo( "GlobaleParameter" ) == 0 )
        success = readGlobaleParameter();
      if ( pActElement->CompareTo ( "FlaechenschlussobjektTransformation" ) == 0 )
        success = readFlaechenschlussobjektTransformation();
      else
      if ( pActElement->CompareTo  ( "UeberlagerungsobjektTransformation" ) == 0 )
        success = readUeberlagerungsobjektTransformation();
      else
      if ( pActElement->CompareTo ( "PlanklassenTransformation" ) == 0 )
        success = readPlanklassenTransformation();
      else
        if ( pActElement->CompareTo ( "OberklassenTransformationFlaechenschluss" ) == 0 )
          success = readOberklassenTransformationFlaechenschluss();
      else
        if ( pActElement->CompareTo ( "OberklassenTransformationUeberlagerung" ) == 0 )
          success = readOberklassenTransformationUeberlagerung();
      else
        if ( pActElement->CompareTo ( "OberklassenTransformationPlan" ) == 0 )
          success = readOberklassenTransformationPlan();
      else
      if ( pActElement->CompareTo ( "MassBaulicheNutzungTransformation" ) == 0 )
        success = readMassBaulicheNutzungTransformation();
      else
      if ( pActElement->CompareTo ( "VerfahrensangabenTransformation" ) == 0 )
        success = readVerfahrensangabenTransformation();
      else
      if ( pActElement->CompareTo ( "ExterneReferenzTransformation" ) == 0 )
        success = readExterneReferenzTransformation();

      if ( !success ) 
        break;     
    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      if (pActElement->CompareTo( "XPlanGMLTransformation" ) == 0 ) break;
    }
  }

  reader->Close();

  return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen globaler Transformationsparameter                               //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readGlobaleParameter()
{
  String ^ STR;

  STR = reader->GetAttribute( "identifierNamespace" );
  if ( STR == nullptr ) return false;
  else
    QuConvert::systemStr2stdStr( pTransform->identifierNamespace, STR );

  STR = reader->GetAttribute( "registryHILUCSValue" );
  if ( STR == nullptr ) return false;
  else
    QuConvert::systemStr2stdStr( pTransform->registryHILUCSValue, STR );

  STR = reader->GetAttribute( "registryLandUseClassificationValue" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( pTransform->registryLandUseClassificationValue, STR );

  STR = reader->GetAttribute( "registrySupplementaryRegulationValue" );
  if ( STR == nullptr ) return false;
  else
    QuConvert::systemStr2stdStr( pTransform->registrySupplementaryRegulationValue, STR );

  STR = reader->GetAttribute( "registryLocalSupplementaryRegulationValue" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( pTransform->registryLocalSupplementaryRegulationValue, STR );

  STR = reader->GetAttribute( "registryLevelOfSpatialPlanValue" );
  if ( STR == nullptr ) return false;
  else
    QuConvert::systemStr2stdStr( pTransform->registryLevelOfSpatialPlanValue, STR );

  STR = reader->GetAttribute( "registryProcessStepGeneralValue" );
  if ( STR == nullptr ) return false;
  else
    QuConvert::systemStr2stdStr( pTransform->registryProcessStepGeneralValue, STR );

  STR = reader->GetAttribute( "registryRegulationNatureValue" );
  if ( STR == nullptr ) return false;
  else
    QuConvert::systemStr2stdStr( pTransform->registryRegulationNatureValue, STR );

  STR = reader->GetAttribute( "registryPlanTypeNameValue" );
  if ( STR == nullptr ) return false;
  else
    QuConvert::systemStr2stdStr( pTransform->registryPlanTypeNameValue, STR );

  STR = reader->GetAttribute( "registryLegislationLevelValue" );
  if ( STR == nullptr ) return false;
  else
    QuConvert::systemStr2stdStr( pTransform->registryLegislationLevelValue, STR );

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter Flächenschlussobjekte                  //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readFlaechenschlussobjektTransformation()
{
  String                       ^ STR;
  string                         stdString;
  string                         dominantLandUse;
  string                         specificLandUse;
  XPlanGML_ClassTransformation * pClassTransformation;
  INSPIRE_AttributZuweisung    * pStandardZuweisung;
  bool                           success;

  STR = reader->GetAttribute( "XPlanKlassenName" );
  if ( STR == nullptr ) return false;

  QuConvert::systemStr2stdStr( stdString, STR );
  pClassTransformation = new XPlanGML_ClassTransformation;
  pClassTransformation->setKlassenNameXPlanGML( stdString );
  pTransform->addXPlanGML_ClassTransformation( pClassTransformation, ZONING_ELEMENT );

  STR = reader->GetAttribute( "kannAbgebildetWerden" );
  if ( STR != nullptr )
  {
    if ( STR->CompareTo( "false") == 0 )
      pClassTransformation->setKannABgebildetWerden( false );
    else
      pClassTransformation->setKannABgebildetWerden( true );
  }

  STR = reader->GetAttribute( "wirdErwartet" );
  if ( STR != nullptr )
  {
    if ( STR->CompareTo( "false") == 0 )
      pClassTransformation->setWirdErwartet( false );
    else
      pClassTransformation->setWirdErwartet( true );
  }

  if ( reader->IsEmptyElement ) return true;

  while( reader->Read() )
  {
    pActElement = reader->LocalName;

    if ( reader->NodeType == XmlNodeType::Element )
    {
      if ( pActElement->CompareTo( "StandardKlassifikationDominantLandUse" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr( dominantLandUse, STR );

        pStandardZuweisung = new INSPIRE_AttributZuweisung;
        pClassTransformation->addStandardAttributZuweisung( pStandardZuweisung );
        pStandardZuweisung->attributName = "plu:hilucsLandUse";
        pStandardZuweisung->attributWert = dominantLandUse;
      }
      else
        if ( pActElement->CompareTo( "StandardKlassifikationSpecificLandUse" ) == 0 )
        {
          STR = reader->ReadString();
          QuConvert::systemStr2stdStr( specificLandUse, STR );

          pStandardZuweisung = new INSPIRE_AttributZuweisung;
          pClassTransformation->addStandardAttributZuweisung( pStandardZuweisung );
          pStandardZuweisung->attributName = "plu:specificLandUse";
          pStandardZuweisung->attributWert = specificLandUse; 
        }

      if ( pActElement->CompareTo( "AttributTransformation" ) == 0 )
        success = readFlaechenschlussobjektAttributTransformation( pClassTransformation );

    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      if ( pActElement->CompareTo( "FlaechenschlussobjektTransformation" ) == 0 )
        break;
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter Attribut eines Flächenschluss-         //
//  objektes                                                                 //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readFlaechenschlussobjektAttributTransformation ( XPlanGML_ClassTransformation * pClassTransformation )
{
  String  ^ STR;
  string    stdString;
  string    xplanAttributName;
  string    xPlanAttributwert;
  string    dominantLandUse;
  string    specificLandUse;
  bool      istNotwendig = false;

  XPlanGML_AttributTransformation * pTransDominantSpecificLandUse;
  INSPIRE_AttributZuordnung       * pZuordnungDominantLandUse = NULL;
  INSPIRE_AttributZuordnung       * pZuordnungSpecificLandUse = NULL;

  STR = reader->GetAttribute( "XPlanAttributName" );
  QuConvert::systemStr2stdStr( xplanAttributName, STR );

  STR = reader->GetAttribute( "istNotwendig" );
  if ( STR != nullptr )
    istNotwendig = XmlConvert::ToBoolean ( STR );

  pTransDominantSpecificLandUse = new XPlanGML_AttributTransformation;
  pTransDominantSpecificLandUse->setAttributNameXPlanGML( xplanAttributName );
  pTransDominantSpecificLandUse->setIstNotwendig( istNotwendig );
  pClassTransformation->addXPlanGML_AttributTransformation( pTransDominantSpecificLandUse );

  pZuordnungDominantLandUse = new INSPIRE_AttributZuordnung;
  pZuordnungDominantLandUse->setAttributNameINSPIRE( "plu:hilucsLandUse" );
  pTransDominantSpecificLandUse->addINSPIRE_AttributZuordnung( pZuordnungDominantLandUse );

  if ( reader->IsEmptyElement ) return true;

  while( reader->Read() )
  {
    pActElement = reader->LocalName;

    if ( reader->NodeType == XmlNodeType::Element )
    {
      if ( pActElement->CompareTo( "INSPIREDominantLandUseDefault" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr( dominantLandUse, STR );
        pZuordnungDominantLandUse->addStandardAttributWertINSPIRE( dominantLandUse );
      }
      else
      if ( pActElement->CompareTo( "INSPIRESpecificLandUseDefault" ) == 0 )
      {
        if ( pZuordnungSpecificLandUse == NULL )
        {
          pZuordnungSpecificLandUse = new INSPIRE_AttributZuordnung;
          pZuordnungSpecificLandUse->setAttributNameINSPIRE( "plu:specificLandUse" );
          pTransDominantSpecificLandUse->addINSPIRE_AttributZuordnung( pZuordnungSpecificLandUse );
        }

        STR = reader->ReadString();
        QuConvert::systemStr2stdStr( specificLandUse, STR );
        pZuordnungSpecificLandUse->addStandardAttributWertINSPIRE( specificLandUse );
      }

      if ( pActElement->CompareTo( "AttributwertTransformation" ) == 0 )
      {
        dominantLandUse      = "";
        specificLandUse      = "";

        STR = reader->GetAttribute( "XPlanAttributWert_Schluessel" );
        QuConvert::systemStr2stdStr( xPlanAttributwert, STR );

        while( reader->Read() )
        {
          pActElement = reader->LocalName;

          if ( reader->NodeType == XmlNodeType::Element )
          {
            if ( pActElement->CompareTo("INSPIREDominantLandUse") == 0 )
            {
              STR = reader->ReadString();
              QuConvert::systemStr2stdStr( dominantLandUse, STR );
              pZuordnungDominantLandUse->addAttributwertZuordnung( xPlanAttributwert, dominantLandUse );
            }
            else
            if ( pActElement->CompareTo("INSPIRESpecificLandUse") == 0 )
            {
              if ( pZuordnungSpecificLandUse == NULL )
              {
                pZuordnungSpecificLandUse = new INSPIRE_AttributZuordnung;
                pZuordnungSpecificLandUse->setAttributNameINSPIRE( "plu:specificLandUse" );
                pTransDominantSpecificLandUse->addINSPIRE_AttributZuordnung( pZuordnungSpecificLandUse );
              }

              STR = reader->ReadString();
              QuConvert::systemStr2stdStr( specificLandUse, STR );
              pZuordnungSpecificLandUse->addAttributwertZuordnung( xPlanAttributwert, specificLandUse );
            }
          }
          else
          if ( reader->NodeType == XmlNodeType::EndElement )
          {
            if (  pActElement->CompareTo( "AttributwertTransformation" ) == 0 ) 
              break;
          }
        }
      }
    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      if (  pActElement->CompareTo( "AttributTransformation" ) == 0 ) 
        break;
    }
  }

  return true;  
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter Überlagerungsobjekte                   //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readUeberlagerungsobjektTransformation()
{
  String                       ^ STR;
  string                         stdString;
  string                         supplementaryRegulation;
  string                         specificSupplementaryRegulation;
  XPlanGML_ClassTransformation * pClassTransformation;
  INSPIRE_AttributZuweisung    * pStandardZuweisung;
  bool                           success;

  STR = reader->GetAttribute( "XPlanKlassenName" );
  if ( STR == nullptr ) return false;

  QuConvert::systemStr2stdStr( stdString, STR );
  pClassTransformation = new XPlanGML_ClassTransformation;
  pClassTransformation->setKlassenNameXPlanGML( stdString );
  pTransform->addXPlanGML_ClassTransformation( pClassTransformation, SUPPLEMENTARY_RESTRICTION );

  STR = reader->GetAttribute( "kannAbgebildetWerden" );
  if ( STR != nullptr )
  {
    if ( STR->CompareTo( "false") == 0 )
      pClassTransformation->setKannABgebildetWerden( false );
    else
      pClassTransformation->setKannABgebildetWerden( true );
  }

  STR = reader->GetAttribute( "wirdErwartet" );
  if ( STR != nullptr )
  {
    if ( STR->CompareTo( "false") == 0 )
      pClassTransformation->setWirdErwartet( false );
    else
      pClassTransformation->setWirdErwartet( true );
  }

  if ( reader->IsEmptyElement ) return true;

  while( reader->Read() )
  {
    pActElement = reader->LocalName;

    if ( reader->NodeType == XmlNodeType::Element )
    {
      if ( pActElement->CompareTo( "StandardKlassifikationSupplementaryRegulation" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr( supplementaryRegulation, STR );

        pStandardZuweisung = new INSPIRE_AttributZuweisung;
        pClassTransformation->addStandardAttributZuweisung( pStandardZuweisung );
        pStandardZuweisung->attributName = "plu:supplementaryRegulation";
        pStandardZuweisung->attributWert = supplementaryRegulation;
      }
      else
      if ( pActElement->CompareTo( "StandardKlassifikationLocalSupplementaryRegulation" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr( specificSupplementaryRegulation, STR );

        pStandardZuweisung = new INSPIRE_AttributZuweisung;
        pClassTransformation->addStandardAttributZuweisung( pStandardZuweisung );
        pStandardZuweisung->attributName = "plu:specificSupplementaryRegulation";
        pStandardZuweisung->attributWert = specificSupplementaryRegulation;
      }

      if ( pActElement->CompareTo( "AttributTransformation" ) == 0 )
        success = readUeberlagerungsobjektAttributTransformation( pClassTransformation );
    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      if ( pActElement->CompareTo( "UeberlagerungsobjektTransformation" ) == 0 )
        break;
    }
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter Attribut eines Überlagerungsobjektes   //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readUeberlagerungsobjektAttributTransformation ( XPlanGML_ClassTransformation * pClassTransformation )
{
  String  ^ STR;
  string    stdString;
  string    xplanAttributName;
  string    xPlanAttributwert;
  string    supplementaryRegulation;
  string    specificSupplementaryRegulation;
  bool      istNotwendig = false;

  XPlanGML_AttributTransformation * pTransSupplementaryRegulation;
  INSPIRE_AttributZuordnung       * pZuordnungSupplementaryRegulation = NULL;
  INSPIRE_AttributZuordnung       * pZuordnungSpecificSupplementaryRegulation = NULL;

  STR = reader->GetAttribute( "XPlanAttributName" );
  QuConvert::systemStr2stdStr( xplanAttributName, STR );

  STR = reader->GetAttribute( "istNotwendig" );
  if ( STR != nullptr )
    istNotwendig = XmlConvert::ToBoolean ( STR );

  pTransSupplementaryRegulation = new XPlanGML_AttributTransformation;
  pTransSupplementaryRegulation->setAttributNameXPlanGML( xplanAttributName );
  pTransSupplementaryRegulation->setIstNotwendig( istNotwendig );
  pClassTransformation->addXPlanGML_AttributTransformation( pTransSupplementaryRegulation );

  pZuordnungSupplementaryRegulation = new INSPIRE_AttributZuordnung;
  pZuordnungSupplementaryRegulation->setAttributNameINSPIRE( "plu:supplementaryRegulation" );
  pTransSupplementaryRegulation->addINSPIRE_AttributZuordnung( pZuordnungSupplementaryRegulation );


  if ( reader->IsEmptyElement ) return true;

  while( reader->Read() )
  {
    pActElement = reader->LocalName;

    if ( reader->NodeType == XmlNodeType::Element )
    {
      if ( pActElement->CompareTo( "SupplementaryRegulationDefault" ) == 0 )
      {
        STR = reader->ReadString();
        QuConvert::systemStr2stdStr( supplementaryRegulation, STR );
        pZuordnungSupplementaryRegulation->addStandardAttributWertINSPIRE( supplementaryRegulation );
      }
      else
      if ( pActElement->CompareTo( "LocalSupplementaryRegulationDefault" ) == 0 )
      {
        if ( pZuordnungSpecificSupplementaryRegulation == NULL )
        {
          pZuordnungSpecificSupplementaryRegulation = new INSPIRE_AttributZuordnung;
          pZuordnungSpecificSupplementaryRegulation->setAttributNameINSPIRE( "plu:specificSupplementaryRegulation" );
          pTransSupplementaryRegulation->addINSPIRE_AttributZuordnung( pZuordnungSpecificSupplementaryRegulation );
        }

        STR = reader->ReadString();
        QuConvert::systemStr2stdStr( specificSupplementaryRegulation, STR );
        pZuordnungSpecificSupplementaryRegulation->addStandardAttributWertINSPIRE( specificSupplementaryRegulation );
      }

      if ( pActElement->CompareTo( "AttributwertTransformation" ) == 0 )
      {
        supplementaryRegulation         = "";
        specificSupplementaryRegulation = "";

        STR = reader->GetAttribute( "XPlanAttributWert_Schluessel" );
        QuConvert::systemStr2stdStr( xPlanAttributwert, STR );

        while( reader->Read() )
        {
          pActElement = reader->LocalName;

          if ( reader->NodeType == XmlNodeType::Element )
          {
            if ( pActElement->CompareTo("SupplementaryRegulation") == 0 )
            {
              STR = reader->ReadString();
              QuConvert::systemStr2stdStr( supplementaryRegulation, STR );
              pZuordnungSupplementaryRegulation->addAttributwertZuordnung( xPlanAttributwert, supplementaryRegulation );
            }
            else
            if ( pActElement->CompareTo("LocalSupplementaryRegulation") == 0 )
            {
              if ( pZuordnungSpecificSupplementaryRegulation == NULL )
              {
                pZuordnungSpecificSupplementaryRegulation = new INSPIRE_AttributZuordnung;
                pZuordnungSpecificSupplementaryRegulation->setAttributNameINSPIRE( "plu:specificSupplementaryRegulation" );
                pTransSupplementaryRegulation->addINSPIRE_AttributZuordnung( pZuordnungSpecificSupplementaryRegulation );
              }

              STR = reader->ReadString();
              QuConvert::systemStr2stdStr( specificSupplementaryRegulation, STR );
              pZuordnungSpecificSupplementaryRegulation->addAttributwertZuordnung( xPlanAttributwert, specificSupplementaryRegulation );
            }
          }
          else
          if ( reader->NodeType == XmlNodeType::EndElement )
          {
            if (  pActElement->CompareTo( "AttributwertTransformation" ) == 0 ) 
              break;
          }
        }
      }
    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      if (  pActElement->CompareTo( "AttributTransformation" ) == 0 ) 
        break;
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter für Klassen                            //
///////////////////////////////////////////////////////////////////////////////
XPlanGML_ClassTransformation * XPlanGML_INSPIRE_Transformation_Reader::readKlassenTransformation( String ^ END_TAG )
{
  String                       ^ STR;
  string                         stdString;
  string                         attributNameINSPIRE;
  string                         attributWertINSPIRE;
  XPlanGML_ClassTransformation * pClassTransformation;
  INSPIRE_AttributZuweisung    * pStandardZuweisung;
  bool                           success;

  STR = reader->GetAttribute( "XPlanKlassenname" );
  if ( STR == nullptr ) return NULL;

  QuConvert::systemStr2stdStr( stdString, STR );
  pClassTransformation = new XPlanGML_ClassTransformation;
  pClassTransformation->setKlassenNameXPlanGML( stdString );

  if ( reader->IsEmptyElement ) return pClassTransformation;

  while( reader->Read() )
  {
    pActElement = reader->LocalName;

    if ( reader->NodeType == XmlNodeType::Element )
    {
      if ( pActElement->CompareTo( "StandardAttributbelegung" ) == 0 )
      {
        STR = reader->GetAttribute( "attributName" );
        QuConvert::systemStr2stdStr( attributNameINSPIRE, STR );

        STR = reader->GetAttribute( "attributWert" );
        QuConvert::systemStr2stdStr( attributWertINSPIRE, STR );

        pStandardZuweisung = new INSPIRE_AttributZuweisung;
        pClassTransformation->addStandardAttributZuweisung( pStandardZuweisung );
        pStandardZuweisung->attributName = attributNameINSPIRE;
        pStandardZuweisung->attributWert = attributWertINSPIRE;
      }
      else
        if ( pActElement->CompareTo( "AttributTransformation" ) == 0 )
          success = readKlassenAttributTransformation( pClassTransformation );

    }
    else
      if ( reader->NodeType == XmlNodeType::EndElement )
      {
        if ( pActElement->CompareTo( END_TAG ) == 0 )
          break;
      }
  }
  return pClassTransformation;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter Attribut eines Planklasse              //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readKlassenAttributTransformation ( XPlanGML_ClassTransformation * pClassTransformation )
{
  String  ^ STR;
  string    xplanAttributName;
  string    xPlanAttributwert;
  bool      istNotwendig = false;

  XPlanGML_AttributTransformation * pAttributTransform;

  STR = reader->GetAttribute( "XPlanAttributName" );
  QuConvert::systemStr2stdStr( xplanAttributName, STR );


  STR = reader->GetAttribute( "istNotwendig" );
  if ( STR != nullptr )
    istNotwendig = XmlConvert::ToBoolean ( STR );

  pAttributTransform = new XPlanGML_AttributTransformation;
  pAttributTransform->setAttributNameXPlanGML( xplanAttributName );
  pAttributTransform->setIstNotwendig( istNotwendig );
  pClassTransformation->addXPlanGML_AttributTransformation( pAttributTransform );


  if ( reader->IsEmptyElement ) return true;

  while( reader->Read() )
  {
    pActElement = reader->LocalName;

    if ( reader->NodeType == XmlNodeType::Element )
    {
      if ( pActElement->CompareTo( "AttributZuordnung" ) == 0 )
        readKlassenAttributZuordnung ( pAttributTransform );     
    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      if (  pActElement->CompareTo( "AttributTransformation" ) == 0 ) 
        break;
    }
  }

  return true;  
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen von Attributwert-Zuordnungen für XPlanGML Enumerationen         //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readKlassenAttributZuordnung ( XPlanGML_AttributTransformation * pAttributTransformation )
{
  String  ^ STR;
  string    INSPIREAttributName;
  string    INSPIREReferenzAttributName;
  string    INSPIREDefaultAttributWert;
  string    xPlanAttributWert;
  string    INSPIREAttributWert;

  INSPIRE_AttributZuordnung  * pAttributZuordnung;
  INSPIRE_AttributZuweisung  * pZuweisung;

  STR = reader->GetAttribute( "INSPIREAttributName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( INSPIREAttributName, STR );

  STR = reader->GetAttribute( "INSPIREReferenzAttributName" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( INSPIREReferenzAttributName, STR );

  STR = reader->GetAttribute( "INSPIREDefaultAttributWert" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( INSPIREDefaultAttributWert, STR );

  pAttributZuordnung = new INSPIRE_AttributZuordnung;
  pAttributZuordnung->setAttributNameINSPIRE( INSPIREAttributName );
  if ( INSPIREReferenzAttributName != "" )
    pAttributZuordnung->setReferenzAttributNameINSPIRE( INSPIREReferenzAttributName );
  if ( INSPIREDefaultAttributWert != "" )
    pAttributZuordnung->addStandardAttributWertINSPIRE( INSPIREDefaultAttributWert );
  pAttributTransformation->addINSPIRE_AttributZuordnung( pAttributZuordnung );

  if ( reader->IsEmptyElement ) return true;

  while( reader->Read() )
  {
    pActElement = reader->LocalName;

    if ( reader->NodeType == XmlNodeType::Element )
    {
      if ( pActElement->CompareTo( "AttributwertZuordnung" ) == 0 )
      {
        STR = reader->GetAttribute( "XPlanAttributWert_Schluessel" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( xPlanAttributWert, STR );

        STR = reader->GetAttribute( "INSPIREAttributWert" );
        if ( STR != nullptr )
          QuConvert::systemStr2stdStr( INSPIREAttributWert, STR );

        pAttributZuordnung->addAttributwertZuordnung( xPlanAttributWert, INSPIREAttributWert );        
      }
      else
        if ( pActElement->CompareTo( "INSPIREAttributZusatz" ) == 0 )
        {
          STR = reader->GetAttribute( "attributName" );
          if ( STR != nullptr )
            QuConvert::systemStr2stdStr( INSPIREAttributName, STR );

          STR = reader->GetAttribute( "attributWert" );
          if ( STR != nullptr )
            QuConvert::systemStr2stdStr( INSPIREAttributWert, STR );

          pZuweisung = new INSPIRE_AttributZuweisung;
          pZuweisung->attributName = INSPIREAttributName;
          pZuweisung->attributWert = INSPIREAttributWert;

          pAttributZuordnung->addAttributZusatzINSPIRE( pZuweisung );
        }
    }
    else
    if ( reader->NodeType == XmlNodeType::EndElement )
    {
      if (  pActElement->CompareTo( "AttributZuordnung" ) == 0 ) 
        break;
    }
  }

  return true;  
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter für Planklassen                        //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readPlanklassenTransformation()
{
  XPlanGML_ClassTransformation * pKlassenTransformation;

  pKlassenTransformation = readKlassenTransformation( "PlanklassenTransformation" );
  if ( pKlassenTransformation != NULL )
  {
    pTransform->addXPlanGML_ClassTransformation( pKlassenTransformation, SPATIAL_PLAN );
    return true;
  }
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter für Oberklassen (flaechenschluss=true) //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readOberklassenTransformationFlaechenschluss()
{
  string                                   oberklassenName, klassenname, klassenPrefix;
  string                                   filter;
	size_t                                   i, anz;
  unsigned int                             j;
  XPlanGML_ClassTransformation           * pClassTransform;
  XPlanGML_AttributTransformation        * pAttributTransform;
  XPlanGML_AttributTransformation        * pAttributTransformCopy;
  INSPIRE_AttributZuweisung              * pAttributZuweisung;
  INSPIRE_AttributZuweisung              * pAttributZuweisungCopy;
  vector<XPlanGML_ClassTransformation*>    vClassTransform ;
  vector<INSPIRE_AttributZuweisung*>       vZuweisung;
  vector<XPlanGML_AttributTransformation*> vAttributTransform;

  XPlanGML_ClassTransformation * pKlassenTransformation;

  pKlassenTransformation = readKlassenTransformation( "OberklassenTransformationFlaechenschluss" );
  oberklassenName = pKlassenTransformation->getKlassenNameXPlanGML();
  filter = oberklassenName.substr( 0, 8 );

  pKlassenTransformation->getStandardAttributZuweisung( vZuweisung );
  pKlassenTransformation->getXPlanGML_AttributTransformation( vAttributTransform );

  anz = pTransform->getXPlanGML_ClassTransformation( vClassTransform, ZONING_ELEMENT );
  for ( i = 0; i < anz; i++ )
  {
    pClassTransform = vClassTransform[i];
    klassenname     = pClassTransform->getKlassenNameXPlanGML();
    klassenPrefix   = klassenname.substr( 0, 8 );
    if ( filter == klassenPrefix )
    {
      for ( j = 0; j < vZuweisung.size(); j++ )
      {
        pAttributZuweisung = vZuweisung[j];
        pAttributZuweisungCopy = new INSPIRE_AttributZuweisung ( pAttributZuweisung );
        pClassTransform->addStandardAttributZuweisung( pAttributZuweisungCopy );
      }

      for ( j = 0; j < vAttributTransform.size(); j++ )
      {
        pAttributTransform = vAttributTransform[j];
        pAttributTransformCopy = new XPlanGML_AttributTransformation ( pAttributTransform );
        pClassTransform->addXPlanGML_AttributTransformation( pAttributTransformCopy );
      }
    }
  }

  delete pKlassenTransformation;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter für Oberklassen (flaechenschluss=false)//
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readOberklassenTransformationUeberlagerung()
{
  string                                   oberklassenName, klassenname, klassenPrefix;
  string                                   filter;
	size_t                                   i, anz;
  unsigned int                             j;
  XPlanGML_ClassTransformation           * pClassTransform;
  XPlanGML_AttributTransformation        * pAttributTransform;
  XPlanGML_AttributTransformation        * pAttributTransformCopy;
  INSPIRE_AttributZuweisung              * pAttributZuweisung;
  INSPIRE_AttributZuweisung              * pAttributZuweisungCopy;
  vector<XPlanGML_ClassTransformation*>    vClassTransform ;
  vector<INSPIRE_AttributZuweisung*>       vZuweisung;
  vector<XPlanGML_AttributTransformation*> vAttributTransform;

  XPlanGML_ClassTransformation * pKlassenTransformation;

  pKlassenTransformation = readKlassenTransformation( "OberklassenTransformationUeberlagerung" );
  oberklassenName = pKlassenTransformation->getKlassenNameXPlanGML();
  filter = oberklassenName.substr( 0, 8 );

  pKlassenTransformation->getStandardAttributZuweisung( vZuweisung );
  pKlassenTransformation->getXPlanGML_AttributTransformation( vAttributTransform );

  anz = pTransform->getXPlanGML_ClassTransformation( vClassTransform, SUPPLEMENTARY_RESTRICTION );
  for ( i = 0; i < anz; i++ )
  {
    pClassTransform = vClassTransform[i];
    klassenname     = pClassTransform->getKlassenNameXPlanGML();
    klassenPrefix   = klassenname.substr( 0, 8 );
    if ( filter == klassenPrefix )
    {
      for ( j = 0; j < vZuweisung.size(); j++ )
      {
        pAttributZuweisung = vZuweisung[j];
        pAttributZuweisungCopy = new INSPIRE_AttributZuweisung ( pAttributZuweisung );
        pClassTransform->addStandardAttributZuweisung( pAttributZuweisungCopy );
      }

      for ( j = 0; j < vAttributTransform.size(); j++ )
      {
        pAttributTransform = vAttributTransform[j];
        pAttributTransformCopy = new XPlanGML_AttributTransformation ( pAttributTransform );
        pClassTransform->addXPlanGML_AttributTransformation( pAttributTransformCopy );
      }
    }
  }

  delete pKlassenTransformation;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter für XP_Plan                            //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readOberklassenTransformationPlan()
{
	size_t                                   i, anz;
  unsigned int                             j;
  XPlanGML_ClassTransformation           * pClassTransform;
  XPlanGML_AttributTransformation        * pAttributTransform;
  XPlanGML_AttributTransformation        * pAttributTransformCopy;
  INSPIRE_AttributZuweisung              * pAttributZuweisung;
  INSPIRE_AttributZuweisung              * pAttributZuweisungCopy;
  vector<XPlanGML_ClassTransformation*>    vClassTransform ;
  vector<INSPIRE_AttributZuweisung*>       vZuweisung;
  vector<XPlanGML_AttributTransformation*> vAttributTransform;

  XPlanGML_ClassTransformation * pKlassenTransformation;

  pKlassenTransformation = readKlassenTransformation( "OberklassenTransformationPlan" );

  pKlassenTransformation->getStandardAttributZuweisung( vZuweisung );
  pKlassenTransformation->getXPlanGML_AttributTransformation( vAttributTransform );

  anz = pTransform->getXPlanGML_ClassTransformation( vClassTransform, SPATIAL_PLAN );
  for ( i = 0; i < anz; i++ )
  {
    pClassTransform = vClassTransform[i];
    
    for ( j = 0; j < vZuweisung.size(); j++ )
    {
      pAttributZuweisung = vZuweisung[j];
      pAttributZuweisungCopy = new INSPIRE_AttributZuweisung ( pAttributZuweisung );
      pClassTransform->addStandardAttributZuweisung( pAttributZuweisungCopy );
    }

    for ( j = 0; j < vAttributTransform.size(); j++ )
    {
      pAttributTransform = vAttributTransform[j];
      pAttributTransformCopy = new XPlanGML_AttributTransformation ( pAttributTransform );
      pClassTransform->addXPlanGML_AttributTransformation( pAttributTransformCopy );
    }
  }

  delete pKlassenTransformation;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter für das Maß der baul. Nutzung          //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readMassBaulicheNutzungTransformation()
{
  unsigned int                          i;
  String                              ^ STR;
  string                                xplanAttributName;
  string                                klassenName;
  string                                inspireDimensioningIndicationReference;
  string                                indicationType;
  string                                uom;
  INSPIRE_AttributZuordnung           * pAttributZuordnung;
  INSPIRE_AttributZuweisung           * pZuweisung;
  XPlanGML_ClassTransformation        * pClassTransform;
  XPlanGML_AttributTransformation     * pAttributTransform;
  XPlanGML_AttributTransformation     * pAttributTransformCopy;
  vector<XPlanGML_ClassTransformation*> vClassTransform ;

  STR = reader->GetAttribute( "XPlanAttributName" );
  QuConvert::systemStr2stdStr( xplanAttributName, STR );

  STR = reader->GetAttribute( "INSPIREDimensioningIndicationReference" );
  QuConvert::systemStr2stdStr( inspireDimensioningIndicationReference, STR );

  STR = reader->GetAttribute( "AttributTyp" );
  QuConvert::systemStr2stdStr( indicationType, STR );

  STR = reader->GetAttribute( "uom" );
  if ( STR != nullptr )
    QuConvert::systemStr2stdStr( uom, STR );
  else
    uom = "";

  pAttributTransform = new XPlanGML_AttributTransformation;
  pAttributTransform->setAttributNameXPlanGML( xplanAttributName );
  if ( uom != "" )
    pAttributTransform->setUom( uom );

  if ( indicationType == "General" )
    pAttributTransform->setKlassenNameINSPIRE( "plu:DimensioningIndicationValue" );
  else
  if ( indicationType == "Integer" )
    pAttributTransform->setKlassenNameINSPIRE( "plu:DimensioningIndicationIntegerValue" );
  else
  if ( indicationType == "Real" )
    pAttributTransform->setKlassenNameINSPIRE( "plu:DimensioningIndicationRealValue" );
  else
  if ( indicationType == "String" )
    pAttributTransform->setKlassenNameINSPIRE( "plu:DimensioningIndicationCharacterValue" );
  else
  if ( indicationType == "Measure" )
    pAttributTransform->setKlassenNameINSPIRE( "plu:DimensioningIndicationMeasureValue" );

  pAttributZuordnung = new INSPIRE_AttributZuordnung;
  pAttributZuordnung->setAttributNameINSPIRE( "plu:dimensioningIndication" );
  pAttributZuordnung->setReferenzAttributNameINSPIRE( "plu:value" );
  pAttributTransform->addINSPIRE_AttributZuordnung( pAttributZuordnung );

  pZuweisung = new INSPIRE_AttributZuweisung;
  pZuweisung->attributName = "plu:indicationReference";
  pZuweisung->attributWert = inspireDimensioningIndicationReference;
  pAttributZuordnung->addAttributZusatzINSPIRE( pZuweisung );

  //  Zuweisung des Attributes zu den Klassen BP_BaugebietsTeilFlaeche, BP_BesondererNutzungszweckFlaeche, BP_GemeinbedarfsFlaeche
  //  BP_SpielSportanlagenFlaeche, BP_GruenFlaeche, BP_VerEntsorgung

  pTransform->getXPlanGML_ClassTransformation( vClassTransform, ZONING_ELEMENT );
  pTransform->getXPlanGML_ClassTransformation( vClassTransform, SUPPLEMENTARY_RESTRICTION );
  for ( i = 0; i < vClassTransform.size(); i++ )
  {
    pClassTransform = vClassTransform[i];
    klassenName     = pClassTransform->getKlassenNameXPlanGML();
    if ( klassenName == "xplan:BP_BaugebietsTeilFlaeche"           ||
         klassenName == "xplan:BP_BesondererNutzungszweckFlaeche"  ||
         klassenName == "xplan:BP_UeberbaubareGrundstuecksFlaeche" ||
         klassenName == "xplan:BP_GemeinbedarfsFlaeche"            ||
         klassenName == "xplan:BP_SpielSportanlagenFlaeche"        ||
         klassenName == "xplan:BP_GruenFlaeche"                    ||
         klassenName == "xplan:BP_VerEntsorgung"     )
    {
        pAttributTransformCopy = new XPlanGML_AttributTransformation ( pAttributTransform );
        pClassTransform->addXPlanGML_AttributTransformation( pAttributTransformCopy );
    }
  }

  delete pAttributTransform;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter für OrdinanceValue                     //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readVerfahrensangabenTransformation()
{
  unsigned int                          i;
  String                              ^ STR;
  string                                xplanAttributName;
  string                                klassenName;
  string                                ordinanceReference;
  INSPIRE_AttributZuordnung           * pAttributZuordnung;
  INSPIRE_AttributZuweisung           * pZuweisung;
  XPlanGML_ClassTransformation        * pClassTransform;
  XPlanGML_AttributTransformation     * pAttributTransform;
  XPlanGML_AttributTransformation     * pAttributTransformCopy;
  vector<XPlanGML_ClassTransformation*> vClassTransform ;

  STR = reader->GetAttribute( "XPlanAttributName" );
  QuConvert::systemStr2stdStr( xplanAttributName, STR );

  STR = reader->GetAttribute( "INSPIREOrdinanceReference" );
  QuConvert::systemStr2stdStr( ordinanceReference, STR );

  pAttributTransform = new XPlanGML_AttributTransformation;
  pAttributTransform->setAttributNameXPlanGML( xplanAttributName );
  pAttributTransform->setKlassenNameINSPIRE( "plu:OrdinanceValue" );

  pAttributZuordnung = new INSPIRE_AttributZuordnung;
  pAttributZuordnung->setAttributNameINSPIRE( "plu:ordinance" );
  pAttributZuordnung->setReferenzAttributNameINSPIRE( "plu:ordinanceDate" );
  pAttributTransform->addINSPIRE_AttributZuordnung( pAttributZuordnung );

  pZuweisung = new INSPIRE_AttributZuweisung;
  pZuweisung->attributName = "plu:ordinanceReference";
  pZuweisung->attributWert = ordinanceReference;
  pAttributZuordnung->addAttributZusatzINSPIRE( pZuweisung );

  //  Zuweisung des Attributes zu den Klassen BP_Plan, FP_Plan, ...

  pTransform->getXPlanGML_ClassTransformation( vClassTransform, SPATIAL_PLAN );
  for ( i = 0; i < vClassTransform.size(); i++ )
  {
    pClassTransform = vClassTransform[i];
    pAttributTransformCopy = new XPlanGML_AttributTransformation ( pAttributTransform );
    pClassTransform->addXPlanGML_AttributTransformation( pAttributTransformCopy );
  }

  delete pAttributTransform;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen Transformationsparameter für ExterneReferenz                    //
///////////////////////////////////////////////////////////////////////////////
bool XPlanGML_INSPIRE_Transformation_Reader::readExterneReferenzTransformation()
{
  String ^ STR;
  string   xplanProperty, inspireDocumentName;

  STR = reader->GetAttribute( "XPlanAttributName" );
  QuConvert::systemStr2stdStr( xplanProperty, STR );

  STR = reader->GetAttribute( "INSPIREPlanDocumentName" );
  QuConvert::systemStr2stdStr( inspireDocumentName, STR );

  pTransform->addDocumentTypeTranslation( xplanProperty, inspireDocumentName );

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//  TransformationINSPIREProtokoll																				   //
//  Protokoll der Transformation XPlanGML 4.0 -> INSPIRE PLU                 //
///////////////////////////////////////////////////////////////////////////////
TransformationINSPIREProtokoll::TransformationINSPIREProtokoll( GML_SCHEMA_TYPE versionXPlanGML )
{
  anzFeaturesTransformiert          = 0;
  anzFeaturesNichtTransformiert     = 0;
}

TransformationINSPIREProtokoll::~TransformationINSPIREProtokoll()
{
  unsigned int i;

  for ( i = 0; i < vInstanzProtokolle.size(); i++ )
    delete vInstanzProtokolle[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert eine Zusammenfassung des Protokolls													   //
///////////////////////////////////////////////////////////////////////////////
void TransformationINSPIREProtokoll::generateZusammenfassung()
{
  unsigned int                                i;
  TransformationINSPIREProtokollXPlanObjekt * pInstanzProtokoll;
  string                                      klassenName;

  for ( i = 0; i < vInstanzProtokolle.size(); i++ )
  {
    pInstanzProtokoll = vInstanzProtokolle[i];
    klassenName       = pInstanzProtokoll->xplanKlassenName;
    switch ( pInstanzProtokoll->transformationsGrad )
    {
    case TRANSFORMIERT:
      anzFeaturesTransformiert++;
      switch ( pInstanzProtokoll->objektTyp )
      {
      case FLAECHENSCHLUSSOBJEKT:
        if ( mFlaechenschlussKlassenVollstaendigTransformiert.find( klassenName ) == mFlaechenschlussKlassenVollstaendigTransformiert.end() )
          mFlaechenschlussKlassenVollstaendigTransformiert[klassenName] = 1;
        else
          mFlaechenschlussKlassenVollstaendigTransformiert[klassenName]++;
        break;

      case UEBERLAGERUNGSOBJEKT:
        if ( mUeberlagerungsKlassenVollstaendigTransformiert.find( klassenName ) == mUeberlagerungsKlassenVollstaendigTransformiert.end() )
          mUeberlagerungsKlassenVollstaendigTransformiert[klassenName] = 1;
        else
          mUeberlagerungsKlassenVollstaendigTransformiert[klassenName]++;
        break;

      case PLANOBJEKT:
      case SONST_OBJEKT:
        if ( mSonstigeKlassenVollstaendigTransformiert.find( klassenName ) == mSonstigeKlassenVollstaendigTransformiert.end() )
          mSonstigeKlassenVollstaendigTransformiert[klassenName] = 1;
        else
          mSonstigeKlassenVollstaendigTransformiert[klassenName]++;
        break;
      }
      break;
  

    case NICHT_TRANSFORMIERT:
      anzFeaturesNichtTransformiert++;

      switch ( pInstanzProtokoll->objektTyp )
      {
      case FLAECHENSCHLUSSOBJEKT:
        if ( pInstanzProtokoll->transformationsHindernis == KLASSE_NICHT_ABBILDBAR )
        {
          if ( mFlaechenschlussKlassenNichtTransformiert.find( klassenName ) == mFlaechenschlussKlassenNichtTransformiert.end() )
            mFlaechenschlussKlassenNichtTransformiert[klassenName] = 1;
          else
            mFlaechenschlussKlassenNichtTransformiert[klassenName]++;
        }
        else
        {
          if ( mFlaechenschlussKlassenNichtErwartet.find( klassenName ) == mFlaechenschlussKlassenNichtErwartet.end() )
            mFlaechenschlussKlassenNichtErwartet[klassenName] = 1;
          else
            mFlaechenschlussKlassenNichtErwartet[klassenName]++;
        }
        break;

      case UEBERLAGERUNGSOBJEKT:
        if ( pInstanzProtokoll->transformationsHindernis == KLASSE_NICHT_ABBILDBAR )
        {
          if ( mUeberlagerungsKlassenNichtTransformiert.find( klassenName ) == mUeberlagerungsKlassenNichtTransformiert.end() )
            mUeberlagerungsKlassenNichtTransformiert[klassenName] = 1;
          else
            mUeberlagerungsKlassenNichtTransformiert[klassenName]++;
        }
        else
        {
          if ( mUeberlagerungsKlassenNichtErwartet.find( klassenName ) == mUeberlagerungsKlassenNichtErwartet.end() )
            mUeberlagerungsKlassenNichtErwartet[klassenName] = 1;
          else
            mUeberlagerungsKlassenNichtErwartet[klassenName]++;
        }
        break;

      case PLANOBJEKT:
      case SONST_OBJEKT:
        if ( mSonstigeKlassenNichtTransformiert.find( klassenName ) == mSonstigeKlassenNichtTransformiert.end() )
          mSonstigeKlassenNichtTransformiert[klassenName] = 1;
        else
          mSonstigeKlassenNichtTransformiert[klassenName]++;

      }
      break;

    }

  }

}
///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben des Protokolls                                             //
///////////////////////////////////////////////////////////////////////////////
void TransformationINSPIREProtokoll::writeProtokoll ( std::string protokollDatei )
{
  String        ^ fileName;
  String        ^ date;
  String        ^ comment;
  string          klassenName;
  int             anzahl;
  unsigned int    i;
  char            buffer[16];
  XmlTextWriter ^ writer;

  map<string,int>::iterator iter;

  _strdate_s ( buffer, 16 );
  date = QuConvert::ToString ( string ( buffer ) );

  fileName = QuConvert::ToString ( protokollDatei );

  System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();
  writer								            = gcnew XmlTextWriter( fileName, utf8);

  writer->Formatting = Formatting::Indented;
  writer->WriteStartDocument( true );

  comment = "Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: ";
  comment = String::Concat ( comment, date );

  writer->WriteComment( comment );

  writer->WriteStartElement("TransformationsProtokollINSPIRE_PLU");

  writer->WriteStartElement( "Zusammenfassung" );

  writer->WriteAttributeString( "anzObjekteTransformiert", Convert::ToString ( anzFeaturesTransformiert ) );
  writer->WriteAttributeString( "anzObjekteNichtTransformiert", Convert::ToString (anzFeaturesNichtTransformiert ) );

  if ( mFlaechenschlussKlassenVollstaendigTransformiert.size() > 0 )
  {
    writer->WriteStartElement( "FlaechenschlussKlassenVollstaendigTransformiert" );

    for ( iter = mFlaechenschlussKlassenVollstaendigTransformiert.begin(); iter != mFlaechenschlussKlassenVollstaendigTransformiert.end(); iter++ )
    {
      klassenName = iter->first;
      anzahl      = iter->second;

      writer->WriteStartElement( "InstanzAnzahl" );
      writer->WriteAttributeString( "klassenName", QuConvert::ToHtml( klassenName ) );
      writer->WriteAttributeString( "anzahl", XmlConvert::ToString( anzahl ) );
      writer->WriteEndElement();
    }
  }

  
  if ( mFlaechenschlussKlassenNichtErwartet.size() > 0 )
  {
    writer->WriteStartElement( "FlaechenschlussKlassenNichtErwartet" );

    for ( iter = mFlaechenschlussKlassenNichtErwartet.begin(); iter != mFlaechenschlussKlassenNichtErwartet.end(); iter++ )
    {
      klassenName = iter->first;
      anzahl      = iter->second;

      writer->WriteStartElement( "InstanzAnzahl" );
      writer->WriteAttributeString( "klassenName", QuConvert::ToHtml( klassenName ) );
      writer->WriteAttributeString( "anzahl", XmlConvert::ToString( anzahl ) );
      writer->WriteEndElement();
    }
  }

  if ( mFlaechenschlussKlassenNichtTransformiert.size() > 0 )
  {
    writer->WriteStartElement( "FlaechenschlussKlassenNichtTransformiert" );

    for ( iter = mFlaechenschlussKlassenNichtTransformiert.begin(); iter != mFlaechenschlussKlassenNichtTransformiert.end(); iter++ )
    {
      klassenName = iter->first;
      anzahl      = iter->second;

      writer->WriteStartElement( "InstanzAnzahl" );
      writer->WriteAttributeString( "klassenName", QuConvert::ToHtml( klassenName ) );
      writer->WriteAttributeString( "anzahl", XmlConvert::ToString( anzahl ) );
      writer->WriteEndElement();
    }

    writer->WriteEndElement();
  }

  if ( mUeberlagerungsKlassenVollstaendigTransformiert.size() > 0 )
  {
    writer->WriteStartElement( "UeberlagerungsKlassenVollstaendigTransformiert" );

    for ( iter = mUeberlagerungsKlassenVollstaendigTransformiert.begin(); iter != mUeberlagerungsKlassenVollstaendigTransformiert.end(); iter++ )
    {
      klassenName = iter->first;
      anzahl      = iter->second;

      writer->WriteStartElement( "InstanzAnzahl" );
      writer->WriteAttributeString( "klassenName", QuConvert::ToHtml( klassenName ) );
      writer->WriteAttributeString( "anzahl", XmlConvert::ToString( anzahl ) );
      writer->WriteEndElement();
    }

    writer->WriteEndElement();
  }

  if ( mUeberlagerungsKlassenNichtErwartet.size() > 0 )
  {
    writer->WriteStartElement( "UeberlagerungsKlassenNichtErwartet" );

    for ( iter = mUeberlagerungsKlassenNichtErwartet.begin(); iter != mUeberlagerungsKlassenNichtErwartet.end(); iter++ )
    {
      klassenName = iter->first;
      anzahl      = iter->second;

      writer->WriteStartElement( "InstanzAnzahl" );
      writer->WriteAttributeString( "klassenName", QuConvert::ToHtml( klassenName ) );
      writer->WriteAttributeString( "anzahl", XmlConvert::ToString( anzahl ) );
      writer->WriteEndElement();
    }

    writer->WriteEndElement();
  }

  if ( mUeberlagerungsKlassenNichtTransformiert.size() > 0 )
  {
    writer->WriteStartElement( "UeberlagerungsKlassenNichtTransformiert" );

    for ( iter = mUeberlagerungsKlassenNichtTransformiert.begin(); iter != mUeberlagerungsKlassenNichtTransformiert.end(); iter++ )
    {
      klassenName = iter->first;
      anzahl      = iter->second;

      writer->WriteStartElement( "InstanzAnzahl" );
      writer->WriteAttributeString( "klassenName", QuConvert::ToHtml( klassenName ) );
      writer->WriteAttributeString( "anzahl", XmlConvert::ToString( anzahl ) );
      writer->WriteEndElement();
    }

    writer->WriteEndElement();
  }

  if ( mSonstigeKlassenVollstaendigTransformiert.size() > 0 )
  {
    writer->WriteStartElement( "SonstigeKlassenVollstaendigTransformiert" );

    for ( iter = mSonstigeKlassenVollstaendigTransformiert.begin(); iter != mSonstigeKlassenVollstaendigTransformiert.end(); iter++ )
    {
      klassenName = iter->first;
      anzahl      = iter->second;

      writer->WriteStartElement( "InstanzAnzahl" );
      writer->WriteAttributeString( "klassenName", QuConvert::ToHtml( klassenName ) );
      writer->WriteAttributeString( "anzahl", XmlConvert::ToString( anzahl ) );
      writer->WriteEndElement();
    }

    writer->WriteEndElement();
  }

  if ( mSonstigeKlassenNichtTransformiert.size() > 0 )
  {
    writer->WriteStartElement( "SonstigeKlassenNichtTransformiert" );

    for ( iter = mSonstigeKlassenNichtTransformiert.begin(); iter != mSonstigeKlassenNichtTransformiert.end(); iter++ )
    {
      klassenName = iter->first;
      anzahl      = iter->second;

      writer->WriteStartElement( "InstanzAnzahl" );
      writer->WriteAttributeString( "klassenName", QuConvert::ToHtml( klassenName ) );
      writer->WriteAttributeString( "anzahl", XmlConvert::ToString( anzahl ) );
      writer->WriteEndElement();
    }

    writer->WriteEndElement();
  }

  writer->WriteEndElement();

  for ( i = 0; i < vInstanzProtokolle.size(); i++ )
    vInstanzProtokolle[i]->writeInstanzProtokoll( writer );

  writer->WriteEndElement();

  writer->Flush();
  writer->Close();
}

///////////////////////////////////////////////////////////////////////////////
//  TransformationINSPIREProtokollXPlanObjekt                                //
//  Protokoll der Transformation einer XPlanGML 4.0 Klasseninstanz           //
///////////////////////////////////////////////////////////////////////////////
TransformationINSPIREProtokollXPlanObjekt::TransformationINSPIREProtokollXPlanObjekt()
{
  objektTyp                = TransformationINSPIREProtokoll::FLAECHENSCHLUSSOBJEKT;
  transformationsGrad      = TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT;
  transformationsHindernis = TransformationINSPIREProtokoll::KLASSE_NICHT_ABBILDBAR;
}

TransformationINSPIREProtokollXPlanObjekt::~TransformationINSPIREProtokollXPlanObjekt()
{
  unsigned int i;

  for ( i = 0; i < vAttributProtokolle.size(); i++ )
    delete vAttributProtokolle[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Rausschreiben des Objekt-Protokolls																			 //
///////////////////////////////////////////////////////////////////////////////
void TransformationINSPIREProtokollXPlanObjekt::writeInstanzProtokoll ( XmlTextWriter ^ writer )
{
  String     ^ STR;
  unsigned int i;

  TransformationINSPIREProtokollXPlanAttribut * pAttributProtokoll;

  writer->WriteStartElement( "InstanzProtokoll" );

  STR = QuConvert::ToString( gmlId );
  writer->WriteAttributeString( "gmlId", STR );

  STR = QuConvert::ToString( xplanKlassenName );
  writer->WriteAttributeString( "klassenNameXPlan", STR );

  STR = QuConvert::ToString(  getObjektTyp() );
  writer->WriteAttributeString( "objektTyp", STR );

  STR = QuConvert::ToString(  getTransformationdGrad() );
  writer->WriteAttributeString( "transformationsGrad", STR );

  if ( transformationsGrad == TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT )
  {
    STR = QuConvert::ToString(  getTransformationsHindernis() );
    writer->WriteAttributeString( "transformationsHindernis", STR );
  }

  for ( i = 0; i < vAttributProtokolle.size(); i++ )
  {
    pAttributProtokoll = vAttributProtokolle[i];
    pAttributProtokoll->writeProtokoll( writer );
  }

  writer->WriteEndElement();  
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Typ des zugehörigen XPlanGML-Objektes												 //
///////////////////////////////////////////////////////////////////////////////
std::string TransformationINSPIREProtokollXPlanObjekt::getObjektTyp()
{
  switch ( objektTyp )
  {
  case TransformationINSPIREProtokoll::FLAECHENSCHLUSSOBJEKT:
    return "Flächenschlussobjekt";

  case TransformationINSPIREProtokoll::UEBERLAGERUNGSOBJEKT:
    return "Überlagerungsobjekt";

  case TransformationINSPIREProtokoll::PLANOBJEKT:
    return "Planobjekt";

  case TransformationINSPIREProtokoll::SONST_OBJEKT:
    return "Sonstiges Objekt";  
  }

  return "";}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Erfolg der Transformation																	   //
///////////////////////////////////////////////////////////////////////////////
std::string TransformationINSPIREProtokollXPlanObjekt::getTransformationdGrad()
{
  switch ( transformationsGrad )
  {
  case TransformationINSPIREProtokoll::TRANSFORMIERT:
    return "Transformiert";

  case TransformationINSPIREProtokoll::NICHT_TRANSFORMIERT:
    return "Nicht transformiert";
  }

  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Transformations-Hindernis																	   //
///////////////////////////////////////////////////////////////////////////////
std::string TransformationINSPIREProtokollXPlanObjekt::getTransformationsHindernis()
{
  switch ( transformationsHindernis )
  {
  case TransformationINSPIREProtokoll::XPLANOBJEKT_NICHT_KORREKT:
    return "XPlan-Objekt nicht korrekt";
    
  case TransformationINSPIREProtokoll::KLASSE_NICHT_ABBILDBAR:
    return "Klasse nicht abbildbar";
    
  case TransformationINSPIREProtokoll::KLASSE_NICHT_ERWARTET:
    return "Klasse nicht erwartet";
  }

  return "";
}

///////////////////////////////////////////////////////////////////////////////
//  TransformationINSPIREProtokollXPlanAttribut                              //
//  Protokoll der Nicht-Transformation eines XPlanGML 4.0 Attributs          //
///////////////////////////////////////////////////////////////////////////////
TransformationINSPIREProtokollXPlanAttribut::TransformationINSPIREProtokollXPlanAttribut()
{

}

TransformationINSPIREProtokollXPlanAttribut::~TransformationINSPIREProtokollXPlanAttribut()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Schreibt das Protokoll																									 //
///////////////////////////////////////////////////////////////////////////////
void TransformationINSPIREProtokollXPlanAttribut::writeProtokoll ( XmlTextWriter ^ writer )
{
  String ^ STR;

  writer->WriteStartElement( "AttributProtokoll" );

  STR = QuConvert::ToString( attributName );
  writer->WriteAttributeString ( "attributName", STR );

  if ( attributWert != "" )
  {
    STR = QuConvert::ToString( attributWert );
    writer->WriteAttributeString ( "attributWert", STR );
  }

  STR = QuConvert::ToString( getTransformationsHindernis() );
  writer->WriteAttributeString ( "transformationsHindernis", STR );

  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert das Transformations-Hindernis																	   //
///////////////////////////////////////////////////////////////////////////////
std::string TransformationINSPIREProtokollXPlanAttribut::getTransformationsHindernis()
{
  switch ( transformationsHindernis )
  {
  case TransformationINSPIREProtokoll::ATTRIBUT_NICHT_ABBILDBAR:
    return "Attribut nicht abbildbar";
    
  case TransformationINSPIREProtokoll::ATTRIBUTWERT_NICHT_ABBILDBAR:
    return "Attribut-Wert nicht abbildbar";
  }

  return "";
}















