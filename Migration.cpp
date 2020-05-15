#include "stdafx.h"

#include <vector>
#include <map>
#include <string>
#include <set>
#include <time.h>

#include "Convert.h"
#include "constant.h"
#include "SrsManagerRef.h"
#include "Feature.h"
#include "Geometrie.h"
#include "GmlSchema.h"
#include "TimeX.h"
#include "UnitOfMeasure.h"
#include "Migration.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  class MigrationV4                                                        //
// Transformiert XPlanGML 3.0 in Version 4.0 oder 4.1												 //
///////////////////////////////////////////////////////////////////////////////
MigrationV4::MigrationV4 ( Features * pPlanObjekteP, UOMList * pUOMList,  MigrationDateiProtokoll * pMigDateiProtP, 
                string codeSpaceCodeListsV4P, string versionXPlanGMLP )
{
  m_pPlanObjekte         = pPlanObjekteP;
  m_pUOMList             = pUOMList;
  m_pMigDateiProt        = pMigDateiProtP;
  m_codeSpaceCodeListsV4 = codeSpaceCodeListsV4P;
  m_versionXPlanGML      = versionXPlanGMLP;
  m_transObjAnz          = 0;
}

MigrationV4::~ MigrationV4()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Transformiert alle Objekte in pPlanObjekte von XPlanGML Version 3.0      //
//  nach XPlanGML Version 4.0 oder 4.1                                       //
///////////////////////////////////////////////////////////////////////////////
void MigrationV4::transformation()
{
	size_t        i, j, planAnz, planBereichAnz, planObjektAnz;
  BLeitPlan   * pPlan;
  PlanBereich * pPlanBereich;
  Feature     * pPlanObjekt;
  GmlSchema   * pGmlSchema = m_pPlanObjekte->getGmlSchema();

  vector<BLeitPlan*>   vPlaene;
  vector<PlanBereich*> vPlanBereiche;
  vector<Feature*>     vPlanObjekte;

  korrekturRelationen();

  planAnz = m_pPlanObjekte->getPlaene( vPlaene );

  for ( i = 0; i < planAnz; i++ )
  {
    pPlan = vPlaene[i];
    transformationPlan ( dynamic_cast<BLeitPlan*>(pPlan) );

    vPlanBereiche.clear();
    planBereichAnz = pPlan->getPlanBereiche ( vPlanBereiche );
    for ( j = 0; j < planBereichAnz; j++ )
    {
      pPlanBereich = vPlanBereiche[j];
      transformationPlanBereich ( dynamic_cast<PlanBereich*>(pPlanBereich) );

      //  Korrektur der GML-Struktur, um eine flache Hierarchie zu erzwingen
      if ( pPlanBereich->getParentId() == pPlan->getGmlId() )
      {
        pPlan->removeChild( pPlanBereich->getGmlId() );
        pPlanBereich->resetParent();
      }
    }

  }

  planObjektAnz = m_pPlanObjekte->getAllFeatures( vPlanObjekte );
  for ( i = 0; i < planObjektAnz; i++ )
  {
    pPlanObjekt = vPlanObjekte[i];

    Feature * pParent = pPlanObjekt->getParent();
    if ( pParent != NULL && pParent->getObjektTyp() == PLAN_BEREICH )
    {
      pParent->removeChild( pPlanObjekt->getGmlId() );
      pPlanObjekt->resetParent();      
    }

    switch ( pPlanObjekt->getObjektTyp() )
    {
    case BLEIT_PLAN:
    case PLAN_BEREICH:
      continue;

    case ANY_OBJEKT:
    case XP_GEOMETRIE_OBJEKT:
    case XP_PUNKT_OBJEKT:
    case XP_LINIEN_OBJEKT:
    case XP_FLAECHEN_OBJEKT:
      transformationObjekt ( pPlanObjekt );
      break;

    case XP_PPO:
    case XP_LPO:
    case XP_FPO:
    case XP_PTO:
    case XP_LTO:
      transformationPraesentationsobjekt ( pPlanObjekt );
      break;

    case RASTER_BASIS:
    case RASTER_AENDERUNG:
      transformationRasterdarstellung ( pPlanObjekt );
    }
  }

  // Elimination von Features XP_Hoehenangabe, die keinem Objekt zugeordnet sind
  vPlanObjekte.clear();
  planObjektAnz = m_pPlanObjekte->getFeatures( vPlanObjekte, "xplan:XP_Hoehenangabe" );
  for ( i = 0; i < planObjektAnz; i++ )
  {
    pPlanObjekt = vPlanObjekte[i];
    if ( pPlanObjekt->getParent() == NULL )
    {
      m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "",FEHLER, 
        "", "",  "Features XP_Hoehenangabe ohne Zuordnung zu Fachobjekten können nicht migriert werden",
        "Planinhalt nicht übernommen, Ausgangsplan fehlerhaft" );
      m_pPlanObjekte->removeFeature( pPlanObjekt->getGmlId() );
    }
  }

  // Elimination von Features XP_ExterneReferenz, die keinem Objekt zugeordnet sind
  vPlanObjekte.clear();
  planObjektAnz = m_pPlanObjekte->getFeatures( vPlanObjekte, "xplan:XP_ExterneReferenz" );
  for ( i = 0; i < planObjektAnz; i++ )
  {
    pPlanObjekt = vPlanObjekte[i];
    if ( pPlanObjekt->getParent() == NULL )
    {
      m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "",FEHLER, 
        "", "",  "Features XP_ExterneReferenz ohne Zuordnung zu Fachobjekten können nicht migriert werden",
        "Planinhalt nicht übernommen, Ausgangsplan fehlerhaft" );
      m_pPlanObjekte->removeFeature( pPlanObjekt->getGmlId() );
    }
  }

  // Elimination von Features XP_ExterneReferenzPlan, die keinem Objekt zugeordnet sind
  vPlanObjekte.clear();
  planObjektAnz = m_pPlanObjekte->getFeatures( vPlanObjekte, "xplan:XP_ExterneReferenzPlan" );
  for ( i = 0; i < planObjektAnz; i++ )
  {
    pPlanObjekt = vPlanObjekte[i];
    if ( pPlanObjekt->getParent() == NULL )
    {
      m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "",FEHLER, 
        "", "",  "Features XP_ExterneReferenzPlan ohne Zuordnung zu Fachobjekten können nicht migriert werden",
        "Planinhalt nicht übernommen, Ausgangsplan fehlerhaft" );
      m_pPlanObjekte->removeFeature( pPlanObjekt->getGmlId() );
    }
  }

  // Elimination von generischenAttributen, die keinem Objekt zugeordnet sind
  vPlanObjekte.clear();
  m_pPlanObjekte->getFeatures( vPlanObjekte, "xplan:XP_DoubleAttribut" );
  m_pPlanObjekte->getFeatures( vPlanObjekte, "xplan:XP_IntegerAttribut" );
  m_pPlanObjekte->getFeatures( vPlanObjekte, "xplan:XP_URLAttribut" );
  m_pPlanObjekte->getFeatures( vPlanObjekte, "xplan:XP_StringAttribut" );
  m_pPlanObjekte->getFeatures( vPlanObjekte, "xplan:XP_DatumAttribut" );

  for ( i = 0; i < (int)vPlanObjekte.size(); i++ )
  {
    pPlanObjekt = vPlanObjekte[i];
    if ( pPlanObjekt->getParent() == NULL )
    {
      m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "",FEHLER, 
        "", "",  "Generische Attribute ohne Zuordnung zu Fachobjekten können nicht migriert werden",
        "Planinhalt nicht übernommen, Ausgangsplan fehlerhaft" );
      m_pPlanObjekte->removeFeature( pPlanObjekt->getGmlId() );
    }
  }

  m_pMigDateiProt->setAnzahlObjekteTransformiert( m_transObjAnz );
}

///////////////////////////////////////////////////////////////////////////////
//  Generiert die Relationen Planbereich --> Planobjekt, wenn nur die        //
//  umgekehrte Relation belegt ist                                           //
///////////////////////////////////////////////////////////////////////////////
void MigrationV4::korrekturRelationen()
{
  vector<Feature*>         vPlanObjekte;
  vector<Feature*>         vPlanBereiche;
  vector<FeatureRelation*> vRelationen;
  set<string>              sRefPlanObjekte;
  map<string,set<string>>  mBereichRelationen;
	size_t                   i, j, anz, anzRel;
  Feature                * pFeature;
  string                   bereichId;
  string                   featureId;
  PlanBereich            * pBereich;
  FeatureRelation        * pRelation;

  anz = m_pPlanObjekte->getAllFeatures( vPlanObjekte );

  for ( i = 0; i < anz; i++ )
  {
    pFeature = vPlanObjekte[i];
    if ( pFeature->getObjektTyp() == PLAN_BEREICH )
      vPlanBereiche.push_back( pFeature );
  }

  for ( i = 0; i < vPlanBereiche.size(); i++ )
  {
    sRefPlanObjekte.clear();
    pFeature = vPlanBereiche[i];
    vRelationen.clear();
    anzRel = pFeature->getFeatureRelationen( "xplan:fachobjekt", vRelationen );
    for ( j = 0; j < vRelationen.size(); j++ )
    {
      featureId = vRelationen[j]->featureId;
      sRefPlanObjekte.insert( featureId );
    }
    mBereichRelationen[pFeature->getGmlId()] = sRefPlanObjekte;
  }

  for ( i = 0; i < anz; i++ )
  {
    pFeature = vPlanObjekte[i];
    featureId =  pFeature->getGmlId();

    if ( pFeature->getObjektTyp() == BLEIT_PLAN   || 
         pFeature->getObjektTyp() == PLAN_BEREICH ||
         pFeature->getObjektTyp() == XP_PPO       ||
         pFeature->getObjektTyp() == XP_LPO       ||
         pFeature->getObjektTyp() == XP_FPO       ||
         pFeature->getObjektTyp() == XP_PTO       ||
         pFeature->getObjektTyp() == XP_LTO       ||
         pFeature->getObjektTyp() == RASTER_BASIS ||
         pFeature->getObjektTyp() == RASTER_AENDERUNG
       )
         continue;

    vRelationen.clear();
    anzRel = pFeature->getFeatureRelationen( "xplan:gehoertZuBereich", vRelationen );
    if ( anzRel > 0 )
    {
      bereichId = vRelationen[0]->featureId;
      pBereich = dynamic_cast<PlanBereich*>(m_pPlanObjekte->getFeature( bereichId ));
      if ( pBereich == NULL ) continue;

      sRefPlanObjekte = mBereichRelationen[bereichId];
      if ( sRefPlanObjekte.count( featureId ) == 0 )
      {
        pRelation = new FeatureRelation;
        pRelation->rollenName = "xplan:fachobjekt";
        pRelation->featureId = featureId;
        pRelation->relToFeature = true;
        pBereich->addFeatureRelation ( pRelation );

        pBereich->addObjekt( pFeature );
        pFeature->setBereichId( bereichId );
        pFeature->propagateBereichId();
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Transformation eines Planobjektes                                        //
///////////////////////////////////////////////////////////////////////////////
void MigrationV4::transformationPlan ( BLeitPlan * pPlan )
{
  string attributName;
  string attributNameNeu;
  string stringAttributWert;
  string gmlId;
  string gmlIdPlan;
  string stadt;
  string ortsteil;
  string datum;
  string klassenName;
  string codeListPath;
  bool   success;
	size_t i, attributWertAnz, bereichAnz;

  Feature      * pPlanObjekt;
  FeatureRelation * pRelation;
  PlanBereich     * pPlanBereich;

  vector<string>           stringAttributWerte;
  vector<PlanBereich*>     vPlanBereiche;
  vector<FeatureRelation*> vObjRelationen;

  gmlIdPlan = pPlan->getGmlId();

  pPlan->deleteStringAttribut( "xplan:xPlanGMLVersion" );
  pPlan->setStringAttribut( "xplan:xPlanGMLVersion", "4.0" ) ;

  attributName = "xplan:aendert";
  attributWertAnz = pPlan->getStringAttributWerte( attributName, stringAttributWerte );
  if ( attributWertAnz >  0 )
  {
    pPlan->deleteStringAttribut( attributName );
    for ( i = 0; i < stringAttributWerte.size(); i++ )
    {
      stringAttributWert = stringAttributWerte[i];

      gmlId = Constant::createGmlId();
      pPlanObjekt = new Feature ( ANY_OBJEKT, gmlId, m_pPlanObjekte, "xplan:XP_VerbundenerPlan" );
      attributNameNeu = "xplan:planName";
      pPlanObjekt->setStringAttribut ( attributNameNeu, stringAttributWert );
      pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "1000" );
      m_pPlanObjekte->addFeature( pPlanObjekt );

      pPlan->addChild ( pPlanObjekt->getGmlId() );
      pPlanObjekt->setParentId( pPlan->getGmlId() );

      pRelation = new FeatureRelation;
      pRelation->rollenName = attributName;
      pRelation->featureId = gmlId;
      pRelation->relToFeature = false;
      pPlan->addFeatureRelation ( pRelation );
    }
  }

  stringAttributWerte.clear();
  attributName = "xplan:wurdeGeaendertVon";
  attributWertAnz = pPlan->getStringAttributWerte( attributName, stringAttributWerte );
  if ( attributWertAnz >  0 )
  {
    pPlan->deleteStringAttribut( attributName );
    for ( i = 0; i < stringAttributWerte.size(); i++ )
    {
      stringAttributWert = stringAttributWerte[i];

      gmlId = Constant::createGmlId();
      pPlanObjekt = new Feature ( ANY_OBJEKT, gmlId, m_pPlanObjekte, "xplan:XP_VerbundenerPlan" );
      attributNameNeu = "xplan:planName";
      pPlanObjekt->setStringAttribut ( attributNameNeu, stringAttributWert );
      pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "1000" );
      m_pPlanObjekte->addFeature( pPlanObjekt );

      pPlan->addChild ( pPlanObjekt->getGmlId() );
      pPlanObjekt->setParentId( pPlan->getGmlId() );

      pRelation = new FeatureRelation;
      pRelation->rollenName = attributName;
      pRelation->featureId = gmlId;
      pRelation->relToFeature = false;
      pPlan->addFeatureRelation ( pRelation );
    }
  }

  stringAttributWerte.clear();
  attributName = "xplan:gkz";
  attributWertAnz = pPlan->getStringAttributWerte( attributName, stringAttributWerte );

  if ( attributWertAnz >  0 )
  {
    pPlan->deleteStringAttribut( attributName );
    for ( i = 0; i < stringAttributWerte.size(); i++ )
    {
      stringAttributWert = stringAttributWerte[i];

      gmlId = Constant::createGmlId();
      pPlanObjekt = new Feature ( ANY_OBJEKT, gmlId, m_pPlanObjekte, "xplan:XP_Gemeinde" );
      pPlanObjekt->setStringAttribut ( "xplan:ags", stringAttributWert );
      if ( i == 0 )
      {
        success = pPlan->getStringAttributWert( "xplan:stadt", stadt );
        if ( success )
        {
          pPlan->deleteStringAttribut( "xplan:stadt" );
          pPlanObjekt->setStringAttribut( "xplan:gemeindeName", stadt );
        }

        success = pPlan->getStringAttributWert( "xplan:ortsteil", ortsteil );
        if ( success )
        {
          pPlan->deleteStringAttribut( "xplan:ortsteil" );
          pPlanObjekt->setStringAttribut( "xplan:ortsteilName", ortsteil );
        }
      }
      m_pPlanObjekte->addFeature( pPlanObjekt );

      pPlan->addChild ( pPlanObjekt->getGmlId() );
      pPlanObjekt->setParentId( pPlan->getGmlId() );

      pRelation = new FeatureRelation;
      pRelation->rollenName = "xplan:gemeinde";
      pRelation->featureId = gmlId;
      pRelation->relToFeature = false;
      pPlan->addFeatureRelation ( pRelation );
    }
  }

  //  Datumsangaben
  DateAttr * pTime = pPlan->getDateAttributWert( "xplan:auslegungsDatum" );
  if ( pTime != NULL  )
  {
    DateAttr * pDateAttr = new DateAttr;
    pDateAttr->value = new TimePosition ( (TimePosition*)(pTime->value) );
    pPlan->setDateAttribut( "xplan:auslegungsStartDatum", pDateAttr );
    pPlan->deleteDateAttribut( "xplan:auslegungsDatum" );
    m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "auslegungsDatum",INFORMATION, 
      "", "", 
      "Attribut wird in XPlanGML 4.0 nicht unterstützt",
      "Attribut \"auslegungsDatum\" wird auf \"auslegungsStartDatum\" abgebildet" );
  }

  pTime = pPlan->getDateAttributWert( "xplan:traegerbeteiligungsDatum" );
  if (  pTime != NULL )
  {
    DateAttr * pDateAttr = new DateAttr;
    pDateAttr->value = new TimePosition ( (TimePosition*)(pTime->value) );
    pPlan->setDateAttribut( "xplan:traegerbeteiligungsStartDatum", pDateAttr );
    pPlan->deleteDateAttribut( "xplan:traegerbeteiligungsDatum" );
    m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "traegerbeteiligungsDatum",INFORMATION, 
      "", "", 
      "Attribut wird in XPlanGML 4.0 nicht unterstützt",
      "Attribut \"traegerbeteiligungsDatum\" wird auf \"traegerbeteiligungsStartDatum\" abgebildet" );
  }

  //  umweltberichtURL beim FPlan
  success = pPlan->getURLAttributWert( "xplan:umweltberichtURL", stringAttributWert );

  if ( success )
  {
    pPlan->deleteURLAttribut( "xplan:umweltberichtURL" );

    gmlId = Constant::createGmlId();
    pPlanObjekt = new Feature ( ANY_OBJEKT, gmlId, m_pPlanObjekte, "xplan:XP_ExterneReferenz" );
    pPlanObjekt->setURLAttribut ( "xplan:referenzURL", stringAttributWert );
    m_pPlanObjekte->addFeature( pPlanObjekt );

    pPlan->addChild ( pPlanObjekt->getGmlId() );
    pPlanObjekt->setParentId( pPlan->getGmlId() );

    pRelation = new FeatureRelation;
    pRelation->rollenName = "xplan:refUmweltbericht";
    pRelation->featureId = gmlId;
    pRelation->relToFeature = false;
    pPlan->addFeatureRelation ( pRelation );
  }

  // Überprüfung / korrektur der Bereiche
  klassenName = pPlan->getKlassenName();
  bereichAnz = pPlan->getPlanBereiche( vPlanBereiche );
  for ( i = 0; i < bereichAnz; i++ )
  {
    pPlanBereich = vPlanBereiche[i];

    if ( klassenName == "xplan:BP_Plan ")
      pPlanBereich->setKlassenName ( "xplan:BP_Bereich" );
    else
    if ( klassenName == "xplan:FP_Plan ")
      pPlanBereich->setKlassenName ( "xplan:FP_Bereich" );
    else
    if ( klassenName == "xplan:LP_Plan ")
      pPlanBereich->setKlassenName ( "xplan:LP_Bereich" );
    else
    if ( klassenName == "xplan:RP_Plan ")
      pPlanBereich->setKlassenName ( "xplan:RP_Bereich" );

  }

  if ( m_codeSpaceCodeListsV4 != "" )
  {
    success = pPlan->getStringAttributWert( "xplan:sonstPlanArt", stringAttributWert );
    if ( success )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_BP_SonstPlanArt.xml";

      pPlan->deleteStringAttribut( "xplan:sonstPlanArt" );
      pPlan->setStringAttribut( "xplan:sonstPlanArt", stringAttributWert, "codeSpace", codeListPath );
    }

    success = pPlan->getStringAttributWert( "xplan:status", stringAttributWert );
    if ( success )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_BP_Status.xml";

      pPlan->deleteStringAttribut( "xplan:status" );
      pPlan->setStringAttribut( "xplan:status", stringAttributWert, "codeSpace", codeListPath );
    }
  }

  pPlan->getFeatureRelationen ( "xplan:refPlangrundlage", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:refRechtsplan", vObjRelationen );
  for ( i = 0; i < vObjRelationen.size(); i++ )
  {
    pRelation = vObjRelationen[i];
    pPlanObjekt = m_pPlanObjekte->getFeature( pRelation->featureId );
    if ( pPlanObjekt != NULL )
    {
      pPlanObjekt->setKlassenName( "xplan:XP_ExterneReferenz" );
      pPlanObjekt->setStringAttribut( "xplan:art", "PlanMitGeoreferenz" );

      pPlan->addChild ( pPlanObjekt->getGmlId() );
      pPlanObjekt->setParentId( pPlan->getGmlId() );
    }
  }

  vObjRelationen.clear();
  pPlan->getFeatureRelationen ( "xplan:refLegende", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:refBegruendung", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:refExternalCodeList", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:refBeschreibung", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:informell", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:rechtsverbindlich", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:verfahrensMerkmale", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:hatGenerAttribut", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:refKoordinatenListe", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:refGrundstuecksverzeichnis", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:refPflanzliste", vObjRelationen );
  pPlan->getFeatureRelationen ( "xplan:refUmweltbericht", vObjRelationen );

  for ( i = 0; i < vObjRelationen.size(); i++ )
  {
    pRelation = vObjRelationen[i];
    pPlanObjekt = m_pPlanObjekte->getFeature( pRelation->featureId );
    if ( pPlanObjekt != NULL )
    {
      pPlan->addChild ( pPlanObjekt->getGmlId() );
      pPlanObjekt->setParentId( pPlan->getGmlId() );
    }
    else
    {
      int xxx = 0;
    }
  }

  //  Attribut rechtsstand von BP_Plan
  if ( klassenName == "xplan:BP_Plan" )
  {
    success = pPlan->getStringAttributWert( "xplan:planArt", stringAttributWert );
    if ( success && stringAttributWert == "2000" )
    {
      pPlan->deleteStringAttribut( "xplan:planArt" );
      pPlan->setStringAttribut( "xplan:planArt", "3000" );
      pPlan->setStringAttribut( "xplan:verfahren", "2000" );

      m_pMigDateiProt->createMigrationFehler ( pPlan->getGmlId(), pPlan->getKlassenName(), "planArt",INFORMATION, 
        "", "", 
        "Planart \"BPlan nach Parag 13\" wird nicht mehr unterstützt",
        "Attribut \"planArt\" wird auf \"Vorhabensbezogener BPlan (3000)\" gesetzt" );
    }
  }

  if ( klassenName == "xplan:FP_Plan" )
  {
    success = pPlan->getStringAttributWert( "xplan:planArt", stringAttributWert );
    if ( success && stringAttributWert == "5000" )
    {
      pPlan->deleteStringAttribut( "xplan:planArt" );
      pPlan->setStringAttribut( "xplan:planArt", "1000" );
      pPlan->setStringAttribut( "xplan:verfahren", "2000" );

      m_pMigDateiProt->createMigrationFehler ( pPlan->getGmlId(), pPlan->getKlassenName(), "planArt", INFORMATION, 
        "", "", 
        "Planart \"FPlan nach Parag 13\" wird nicht mehr unterstützt",
        "Attribut \"planArt\" wird auf \"FPlan (1000)\" gesetzt" );
    }
  }
  m_transObjAnz++;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformation eines Planbereichs                                        //
///////////////////////////////////////////////////////////////////////////////
void MigrationV4::transformationPlanBereich ( PlanBereich * pPlanBereich )
{
  string stringAttributWert;
  string planBereichId;
	size_t i, j, anz, anzA;
  bool   success, nachrichtlich, gefunden;
  Feature                * pRefPlanObjekt;
  Feature                * pRefPlanBereich;
  FeatureRelation        * pRelation;
  FeatureRelation        * pRelationA;
  vector<FeatureRelation*> vObjRelationen;
  vector<FeatureRelation*> vObjRelationenA;

  planBereichId = pPlanBereich->getGmlId();

  success = pPlanBereich->getStringAttributWert( "xplan:versionBauNVO", stringAttributWert );
  if ( success )
  {
    pPlanBereich->deleteStringAttribut( "xplan:versionBauNVO" );
    if ( pPlanBereich->getKlassenName() == "xplan:BP_Bereich" || 
         pPlanBereich->getKlassenName() == "xplan:FP_Bereich" )
      pPlanBereich->setStringAttribut( "xplan:versionBauNVOText", stringAttributWert );
    else
    {
      m_pMigDateiProt->createMigrationFehler ( pPlanBereich->getGmlId(), pPlanBereich->getKlassenName(),  "xplan:versionBauNVO",FEHLER, pPlanBereich->getKlassenName(), "", 
        "Attribut kann nicht übernommen werden", "" );
    }
  }

  success = pPlanBereich->getStringAttributWert( "xplan:sonstigeBedeutung", stringAttributWert );
  if ( success )
  {
    pPlanBereich->deleteStringAttribut ( "xplan:sonstigeBedeutung" );
    pPlanBereich->setStringAttribut ( "xplan:detaillierteBedeutung", stringAttributWert );
  }

  //  Relationen Bereich - Fachobjekte
  anz = pPlanBereich->getFeatureRelationen( "xplan:fachobjekt", vObjRelationen );
  for ( i = 0; i < anz; i++ )
  {
    pRelation =  vObjRelationen[i];
    pRefPlanObjekt = m_pPlanObjekte->getFeature ( pRelation->featureId );
    if ( pRefPlanObjekt != NULL )
    {
      nachrichtlich = istNachrichtlich ( pPlanBereich, pRefPlanObjekt );
      if ( nachrichtlich )
        pRelation->rollenName = "xplan:nachrichtlich";
      else
      {
        if ( pPlanBereich->getKlassenName() == "xplan:BP_Bereich" )
          pRelation->rollenName = "xplan:inhaltBPlan";
        else
        if ( pPlanBereich->getKlassenName() == "xplan:FP_Bereich" )
          pRelation->rollenName = "xplan:inhaltFPlan";
        else
        if ( pPlanBereich->getKlassenName() == "xplan:RP_Bereich" )
          pRelation->rollenName = "xplan:inhaltRPlan";
        else
        if ( pPlanBereich->getKlassenName() == "xplan:LP_Bereich" )
          pRelation->rollenName = "xplan:inhaltLPlan";
      }
      pPlanBereich->removeFeatureRelation( pRefPlanObjekt );
      pPlanBereich->addFeatureRelation( pRelation );

      vObjRelationenA.clear();
      anzA = pRefPlanObjekt->getFeatureRelationen( "xplan:gehoertZuBereich", vObjRelationenA );
      gefunden = false;
      for ( j = 0; j < anzA; j++ )
      {
        pRelationA = vObjRelationenA[j];
        pRefPlanBereich = m_pPlanObjekte->getFeature ( pRelationA->featureId );
        if ( pRefPlanBereich == pPlanBereich )
        {
          gefunden = true;
          break;
        }
      }

      if ( !gefunden )
      {
        pRelation = new FeatureRelation;
        pRelation->featureId = pPlanBereich->getGmlId();
        pRelation->rollenName    = "xplan:gehoertZuBereich";
        pRelation->relToFeature  = true;
        pRefPlanObjekt->addFeatureRelation( pRelation );
        m_pMigDateiProt->createMigrationFehler ( pRefPlanObjekt->getGmlId(), pRefPlanObjekt->getKlassenName(),  "",INFORMATION, "", "", "",
                    "Referenz Fachobjekt auf Bereich wurde ergänzt" );
      }
    }
  }
  m_transObjAnz++;
}

///////////////////////////////////////////////////////////////////////////////
//  Bestimmt ob ein Planinhalt als Inhalt oder nachrichtlich referiert wird  //
///////////////////////////////////////////////////////////////////////////////
bool MigrationV4::istNachrichtlich ( PlanBereich * pPlanBereich, Feature * pPlanObjekt )
{
  string planBereichKlasse;
  string planBereichPrefix;
  string planObjektKlasse;
  string planObjektPrefix;
  string rechtlicheCharakterisierung;
  bool   success = false;

  planBereichKlasse = pPlanBereich->getKlassenName();
  planBereichPrefix = planBereichKlasse.substr( 0, 8 );
  planObjektKlasse = pPlanObjekt->getKlassenName();
  planObjektPrefix = planObjektKlasse.substr( 0, 8 );

  if ( planBereichPrefix != planObjektPrefix ) return true;

  if ( planObjektPrefix == "xplan:BP" )
    success = pPlanObjekt->getStringAttributWert( "xplan:rechtlicheCharakterisierung", rechtlicheCharakterisierung );
  else
  if ( planObjektPrefix == "xplan:FP" ||  planObjektPrefix == "xplan:RP" )
    success = pPlanObjekt->getStringAttributWert( "rechtscharakter", rechtlicheCharakterisierung );
  else
  if ( planObjektPrefix == "xplan:LP" )
    success = pPlanObjekt->getStringAttributWert( "xplan:status", rechtlicheCharakterisierung );

  if ( success )
  {
    if ( ( planObjektPrefix == "xplan:BP" || planObjektPrefix == "xplan:FP") && rechtlicheCharakterisierung == "2000"  )  return true;
    if ( planObjektPrefix == "xplan:LP" && rechtlicheCharakterisierung == "3000" ) return true;
    if ( planObjektPrefix == "xplan:RP" &&  ( rechtlicheCharakterisierung == "3000"  || 
                                              rechtlicheCharakterisierung == "4000"  ||
                                              rechtlicheCharakterisierung == "5000"  ) ) return true;
  }

  if ( planObjektKlasse == "xplan:BP_Grabungsschutzgebiet"                ||
       planObjektKlasse == "xplan:BP_Bauschutzbereich"                    ||
       planObjektKlasse == "xplan:BP_Fachgesetz"                          ||
       planObjektKlasse == "xplan:BP_Laermschutzbereich"                  ||
       planObjektKlasse == "xplan:BP_BahnVerkehr"                         ||
       planObjektKlasse == "xplan:BP_LuftverkehrFlaeche"                  ||
       planObjektKlasse == "xplan:BP_WasserrechtlicheFestsetzungsFlaeche" ||
       planObjektKlasse == "xplan:BP_VorbHochwSchutzFlaeche" ) return true;

  if ( planObjektKlasse == "xplan:FP_Grabungsschutzgebiet"  ||
       planObjektKlasse == "xplan:FP_DenkmalschutzEnsemble" ||
       planObjektKlasse == "xplan:FP_Schutzgebiet"          ||
       planObjektKlasse == "xplan:FP_Fachgesetz"            ||
       planObjektKlasse == "xplan:FP_Bauschutzbereich"      ||
       planObjektKlasse == "xplan:FP_Laermschutzbereich"    ||
       planObjektKlasse == "xplan:FP_Bahnverkehr"           ||
       planObjektKlasse == "xplan:FP_Luftverkehr"           ||
       planObjektKlasse == "xplan:FP_Wasserrecht"           ||
       planObjektKlasse == "xplan:FP_VorbHochwSchutz"  ) return true;

  return false;    
}

///////////////////////////////////////////////////////////////////////////////
//  Transformation eines DataTypes oder  Features                            //
///////////////////////////////////////////////////////////////////////////////
void MigrationV4::transformationObjekt ( Feature * pPlanObjekt )
{
  string       klassenName;
  string       klassenNameKuerzel;
  string       stringAttributWert;
  double       doubleAttributWert;
  string       codeListPath;
  string       codeListName;
  bool         success;
	size_t       i, anz;

  vector<FeatureRelation*> vRelationen;
  PlanBereich            * pPlanBereichRel;
  Feature                * pPlanObjektRef;
  FeatureRelation        * pRel;
  FeatureRelation        * pRelBereich;

  klassenName = pPlanObjekt->getKlassenName();
  klassenNameKuerzel = klassenName.substr( 0, 8 );

  GmlSchema  * pGmlSchema  = m_pPlanObjekte->getGmlSchema();
  GmlKlasse  * pGmlKlasse = pGmlSchema->getGmlKlasseFromElementName( klassenName );
  GmlAttribut * pGmlAttribut;

  pPlanObjekt->getFeatureRelationen ( "xplan:hoehenangabe", vRelationen );
  pPlanObjekt->getFeatureRelationen ( "xplan:refGebaeudeQuerschnitt", vRelationen );
  pPlanObjekt->getFeatureRelationen ( "xplan:refMassnahmenText", vRelationen );
  pPlanObjekt->getFeatureRelationen ( "xplan:refLandschaftsplan", vRelationen );
  pPlanObjekt->getFeatureRelationen ( "xplan:informell", vRelationen );
  pPlanObjekt->getFeatureRelationen ( "xplan:rechtsverbindlich", vRelationen );
  pPlanObjekt->getFeatureRelationen ( "xplan:hatGenerAttribut", vRelationen );
  pPlanObjekt->getFeatureRelationen ( "xplan:refText", vRelationen );
  pPlanObjekt->getFeatureRelationen ( "xplan:refScan", vRelationen );
  pPlanObjekt->getFeatureRelationen ( "xplan:refLegende", vRelationen );

  for ( i = 0; i < vRelationen.size(); i++ )
  {
    pRel = vRelationen[i];
    pPlanObjektRef = m_pPlanObjekte->getFeature( pRel->featureId );
    if ( pPlanObjektRef != NULL )
    {
      pPlanObjekt->addChild ( pPlanObjektRef->getGmlId() );
      pPlanObjektRef->setParentId( pPlanObjekt->getGmlId() );
    }
  }

  vRelationen.clear();


  success = pPlanObjekt->getStringAttributWert( "xplan:gesetzlicheGrundlage", stringAttributWert );
  if ( success )
  {
    codeListPath =  m_codeSpaceCodeListsV4 + "xplan_XP_GesetzlicheGrundlage.xml";

    pPlanObjekt->deleteStringAttribut( "xplan:gesetzlicheGrundlage" );
    pPlanObjekt->setStringAttribut( "xplan:gesetzlicheGrundlage", stringAttributWert, "codeSpace", codeListPath );
  }

  if ( klassenName == "xplan:XP_Hoehenangabe" )
  {
    success = pPlanObjekt->getDoubleAttributWert( "xplan:hMin", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteDoubleAttribut( "xplan:hMin" );
      pPlanObjekt->setDoubleAttribut( "xplan:hMin", doubleAttributWert,m_pUOMList->getUomLength() );
    }

    success = pPlanObjekt->getDoubleAttributWert( "xplan:hMax", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteDoubleAttribut( "xplan:hMax" );
      pPlanObjekt->setDoubleAttribut( "xplan:hMax", doubleAttributWert, m_pUOMList->getUomLength() );
    }

    success = pPlanObjekt->getDoubleAttributWert( "xplan:hZwingend", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteDoubleAttribut( "xplan:hZwingend" );
      pPlanObjekt->setDoubleAttribut( "xplan:hZwingend", doubleAttributWert, m_pUOMList->getUomLength() );
    }

    success = pPlanObjekt->getDoubleAttributWert( "xplan:h", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteDoubleAttribut( "xplan:h" );
      pPlanObjekt->setDoubleAttribut( "xplan:h", doubleAttributWert, m_pUOMList->getUomLength() );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:abweichenderHoeherbezug", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut ( "xplan:abweichenderHoeherbezug" );
      pPlanObjekt->setStringAttribut (  "xplan:abweichenderHoehenbezug", stringAttributWert );
      m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(),  "xplan:abweichenderHoeherbezug", INFORMATION, "", "",
        "Attribut umbenannt in XPlanGML 4.x", "Attributname wurde korrigiert" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:hoehenbezug", stringAttributWert );
    if ( success && stringAttributWert == "3000" )
    {
      if ( m_versionXPlanGML == "4.0" )
      {
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(),  "xplan:hoehenbezug", WARNUNG, "", "",
          "Attributwert nicht mehr unterstützt in XPlanGML 4.0", "Attributwert muss ignoriert werden" );
      }
      if ( m_versionXPlanGML == "4.1" )
      {
        pPlanObjekt->deleteStringAttribut ( "xplan:hoehenbezug" );
      }
    }
  }

  if ( klassenName == "xplan:XP_Grenze" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_Grenze" );

    success = pPlanObjekt->getStringAttributWert( "xplan:besondererTyp",stringAttributWert );
    if ( success )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_SonstGrenzeTypen.xml";
     
      pPlanObjekt->deleteStringAttribut( "xplan:besondererTyp" );
      if ( m_codeSpaceCodeListsV4 != "" )
        pPlanObjekt->setStringAttribut( "xplan:sonstTyp", stringAttributWert, "codeSpace", codeListPath );
      else
        pPlanObjekt->setStringAttribut( "xplan:sonstTyp", stringAttributWert );
    }
  }

  if ( m_codeSpaceCodeListsV4 != "" )
  {
    if ( pPlanObjekt->getStringAttributWert( "xplan:referenzMimeType", stringAttributWert ) )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_XP_MimeTypes.xml";
      pPlanObjekt->deleteStringAttribut( "xplan:referenzMimeType" );
      pPlanObjekt->setStringAttribut( "xplan:referenzMimeType", stringAttributWert, "codeSpace", codeListPath );
    }
    else
    if ( pPlanObjekt->getStringAttributWert( "xplan:georefMimeType", stringAttributWert ) )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_XP_MimeTypes.xml";
      pPlanObjekt->deleteStringAttribut( "xplan:georefMimeType" );
      pPlanObjekt->setStringAttribut( "xplan:georefMimeType", stringAttributWert, "codeSpace", codeListPath );
    }
    else
    if ( pPlanObjekt->getStringAttributWert( "xplan:detaillierteArtDerBaulNutzung", stringAttributWert ) )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_BP_DetailArtDerBaulNutzung.xml";
      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteArtDerBaulNutzung" );
      pPlanObjekt->setStringAttribut( "xplan:detaillierteArtDerBaulNutzung", stringAttributWert, "codeSpace", codeListPath );
    }
    else
    if ( pPlanObjekt->getStringAttributWert( "xplan:detaillierteDachform", stringAttributWert ) )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_BP_DetailDachform.xml";
      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteDachform" );
      pPlanObjekt->setStringAttribut( "xplan:detaillierteDachform", stringAttributWert, "codeSpace", codeListPath );
    }
    else
    if ( pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert ) )
    {
      pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:detaillierteZweckbestimmung" );
      codeListName = pGmlAttribut->getGmlEnumeration();
			size_t index = codeListName.find( ":" );
      codeListName = codeListName.replace( index, 1, "_" );
      codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:detaillierteZweckbestimmung", stringAttributWert, "codeSpace", codeListPath );
    }
    else
    if ( pPlanObjekt->getStringAttributWert( "xplan:weitereDetailZweckbestimmung1", stringAttributWert ) )
    {
      pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:weitereDetailZweckbestimmung1" );
      codeListName = pGmlAttribut->getGmlEnumeration();
			size_t index = codeListName.find( ":" );
      codeListName = codeListName.replace( index, 1, "_" );
      codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

      pPlanObjekt->deleteStringAttribut( "xplan:weitereDetailZweckbestimmung1" );
      pPlanObjekt->setStringAttribut( "xplan:weitereDetailZweckbestimmung1", stringAttributWert, "codeSpace", codeListPath );
    }
    else
    if ( pPlanObjekt->getStringAttributWert( "xplan:weitereDetailZweckbestimmung2", stringAttributWert ) )
    {
      pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:weitereDetailZweckbestimmung2" );
      codeListName = pGmlAttribut->getGmlEnumeration();
			size_t index = codeListName.find( ":" );
      codeListName = codeListName.replace( index, 1, "_" );
      codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

      pPlanObjekt->deleteStringAttribut( "xplan:weitereDetailZweckbestimmung2" );
      pPlanObjekt->setStringAttribut( "xplan:weitereDetailZweckbestimmung2", stringAttributWert, "codeSpace", codeListPath );
    }
    else
    if ( pPlanObjekt->getStringAttributWert( "xplan:weitereDetailZweckbestimmung3", stringAttributWert ) )
    {
      pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:weitereDetailZweckbestimmung3" );
      codeListName = pGmlAttribut->getGmlEnumeration();
			size_t index = codeListName.find( ":" );
      codeListName = codeListName.replace( index, 1, "_" );
      codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

      pPlanObjekt->deleteStringAttribut( "xplan:weitereDetailZweckbestimmung3" );
      pPlanObjekt->setStringAttribut( "xplan:weitereDetailZweckbestimmung3", stringAttributWert, "codeSpace", codeListPath );
    }
    else
    if ( pPlanObjekt->getStringAttributWert( "xplan:weitereDetailZweckbestimmung4", stringAttributWert ) )
    {
      pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:weitereDetailZweckbestimmung4" );
      codeListName = pGmlAttribut->getGmlEnumeration();
			size_t index = codeListName.find( ":" );
      codeListName = codeListName.replace( index, 1, "_" );
      codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

      pPlanObjekt->deleteStringAttribut( "xplan:weitereDetailZweckbestimmung4" );
      pPlanObjekt->setStringAttribut( "xplan:weitereDetailZweckbestimmung4", stringAttributWert, "codeSpace", codeListPath );
    }
    else
    if ( pPlanObjekt->getStringAttributWert( "xplan:weitereDetailZweckbestimmung5", stringAttributWert ) )
    {
      pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:weitereDetailZweckbestimmung5" );
      codeListName = pGmlAttribut->getGmlEnumeration();
			size_t index = codeListName.find( ":" );
      codeListName = codeListName.replace( index, 1, "_" );
      codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

      pPlanObjekt->deleteStringAttribut( "xplan:weitereDetailZweckbestimmung5" );
      pPlanObjekt->setStringAttribut( "xplan:weitereDetailZweckbestimmung5", stringAttributWert, "codeSpace", codeListPath );
    }
    else
    if ( pPlanObjekt->getStringAttributWert( "xplan:spezifischePraegung", stringAttributWert ) )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_FP_SpezifischePraegungTypen.xml";

      pPlanObjekt->deleteStringAttribut( "xplan:spezifischePraegung" );
      pPlanObjekt->setStringAttribut( "xplan:spezifischePraegung", stringAttributWert, "codeSpace", codeListPath );
    }

    if ( klassenName == "xplan:BP_GenerischesObjekt" || 
         klassenName == "xplan:FP_GenerischesObjekt" || 
         klassenName == "xplan:RP_GenerischesObjekt" || 
         klassenName == "xplan:LP_GenerischesObjekt"  )
    {
      if ( pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert ) )
      {
        pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:zweckbestimmung" );
        codeListName = pGmlAttribut->getGmlEnumeration();
				size_t index = codeListName.find( ":" );
        codeListName = codeListName.replace( index, 1, "_" );
        codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

        pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );
        pPlanObjekt->setStringAttribut( "xplan:zweckbestimmung", stringAttributWert, "codeSpace", codeListPath );
      }
      else
      if ( pPlanObjekt->getStringAttributWert( "xplan:weitereZweckbestimmung1", stringAttributWert ) )
      {
        pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:weitereZweckbestimmung1" );
        codeListName = pGmlAttribut->getGmlEnumeration();
				size_t index = codeListName.find( ":" );
        codeListName = codeListName.replace( index, 1, "_" );
        codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

        pPlanObjekt->deleteStringAttribut( "xplan:weitereZweckbestimmung1" );
        pPlanObjekt->setStringAttribut( "xplan:weitereZweckbestimmung1", stringAttributWert, "codeSpace", codeListPath );
      }
      else
      if ( pPlanObjekt->getStringAttributWert( "xplan:weitereZweckbestimmung2", stringAttributWert ) )
      {
        pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:weitereZweckbestimmung2" );
        codeListName = pGmlAttribut->getGmlEnumeration();
				size_t index = codeListName.find( ":" );
        codeListName = codeListName.replace( index, 1, "_" );
        codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

        pPlanObjekt->deleteStringAttribut( "xplan:weitereZweckbestimmung2" );
        pPlanObjekt->setStringAttribut( "xplan:weitereZweckbestimmung2", stringAttributWert, "codeSpace", codeListPath );
      }
      else
      if ( pPlanObjekt->getStringAttributWert( "xplan:weitereZweckbestimmung3", stringAttributWert ) )
      {
        pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:weitereZweckbestimmung3" );
        codeListName = pGmlAttribut->getGmlEnumeration();
				size_t index = codeListName.find( ":" );
        codeListName = codeListName.replace( index, 1, "_" );
        codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

        pPlanObjekt->deleteStringAttribut( "xplan:weitereZweckbestimmung3" );
        pPlanObjekt->setStringAttribut( "xplan:weitereZweckbestimmung3", stringAttributWert, "codeSpace", codeListPath );
      }
    }

    if ( klassenNameKuerzel == "xplan:LP" )
    {
      if ( pPlanObjekt->getStringAttributWert( "xplan:detaillierteFunktion", stringAttributWert ) )
      {
        pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:detaillierteFunktion" );
        codeListName = pGmlAttribut->getGmlEnumeration();
				size_t index = codeListName.find( ":" );
        codeListName = codeListName.replace( index, 1, "_" );
        codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

        pPlanObjekt->deleteStringAttribut( "xplan:detaillierteFunktion" );
        pPlanObjekt->setStringAttribut( "xplan:detaillierteFunktion", stringAttributWert, "codeSpace", codeListPath );
      }
      else
      if ( pPlanObjekt->getStringAttributWert( "xplan:weitereDetailFunktion1", stringAttributWert ) )
      {
        pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:weitereDetailFunktion1" );
        codeListName = pGmlAttribut->getGmlEnumeration();
				size_t index = codeListName.find( ":" );
        codeListName = codeListName.replace( index, 1, "_" );
        codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

        pPlanObjekt->deleteStringAttribut( "xplan:weitereDetailFunktion1" );
        pPlanObjekt->setStringAttribut( "xplan:weitereDetailFunktion1", stringAttributWert, "codeSpace", codeListPath );
      }
      else
      if ( pPlanObjekt->getStringAttributWert( "xplan:weitereDetailFunktion2", stringAttributWert ) )
      {
        pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:weitereDetailFunktion2" );
        codeListName = pGmlAttribut->getGmlEnumeration();
				size_t index = codeListName.find( ":" );
        codeListName = codeListName.replace( index, 1, "_" );
        codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

        pPlanObjekt->deleteStringAttribut( "xplan:weitereDetailFunktion2" );
        pPlanObjekt->setStringAttribut( "xplan:weitereDetailFunktion2", stringAttributWert, "codeSpace", codeListPath );
      }
      else
      if ( pPlanObjekt->getStringAttributWert( "xplan:detailTyp", stringAttributWert ) )
      {
        pGmlAttribut = pGmlKlasse->getGmlAttribut( "xplan:detailTyp" );
        codeListName = pGmlAttribut->getGmlEnumeration();
				size_t index = codeListName.find( ":" );
        codeListName = codeListName.replace( index, 1, "_" );
        codeListPath =  m_codeSpaceCodeListsV4 + codeListName + ".xml";

        pPlanObjekt->deleteStringAttribut( "xplan:detailTyp" );
        pPlanObjekt->setStringAttribut( "xplan:detailTyp", stringAttributWert, "codeSpace", codeListPath );
      }
    }
  }

  if ( pPlanObjekt->getSymbolPositionenAnz() > 0 )
  {
    pPlanObjekt->deleteSymbolPositionen();
    m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(),  "",WARNUNG, "", "",
      "Attribut symbolPosition wird in XPlanGML 4.0 nicht mehr unterstützt",
      "symbolPosition wurde entfernt" );
  }

  anz = pPlanObjekt->getFeatureRelationen( "xplan:gehoertZuBereich", vRelationen );
  for ( i = 0; i < anz; i++ )
  {
    pRel = vRelationen[i];
    pPlanBereichRel = dynamic_cast<PlanBereich*> (m_pPlanObjekte->getFeature( pRel->featureId ));
    if ( pPlanBereichRel != NULL )
    {
      pRelBereich = pPlanBereichRel->getFeatureRelation( pPlanObjekt );
      if ( pRelBereich->rollenName == "xplan:nachrichtlich")
        pRel->rollenName = "xplan:gehoertNachrichtlichZuBereich";
      else
      {
        if ( pPlanBereichRel->getKlassenName() == "xplan:BP_Bereich" )
          pRel->rollenName = "xplan:gehoertZuBP_Bereich";
        else
        if ( pPlanBereichRel->getKlassenName() == "xplan:FP_Bereich" )
          pRel->rollenName = "xplan:gehoertZuFP_Bereich";
        else
        if ( pPlanBereichRel->getKlassenName() == "xplan:LP_Bereich" )
          pRel->rollenName = "xplan:gehoertZuLP_Bereich";
        else
        if ( pPlanBereichRel->getKlassenName() == "xplan:RP_Bereich" )
          pRel->rollenName = "xplan:gehoertZuRP_Bereich";
      }
      pPlanObjekt->removeFeatureRelation( pPlanBereichRel );
      pPlanObjekt->addFeatureRelation( pRel );
    }
  }

  if ( klassenNameKuerzel == "xplan:BP" || klassenNameKuerzel == "xplan:FP" )
  {
    success = pPlanObjekt->getStringAttributWert( "xplan:rechtlicheCharakterisierung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:rechtlicheCharakterisierung" );
      if ( stringAttributWert != "2000" )
        pPlanObjekt->setStringAttribut ( "xplan:rechtscharakter", stringAttributWert );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:rechtscharakter", stringAttributWert );
    if ( success && stringAttributWert == "2000" )
      pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );

    if ( klassenNameKuerzel == "xplan:BP" )
      transformationBPlanObjekt ( pPlanObjekt );
    else
    if ( klassenNameKuerzel == "xplan:FP" )
      transformationFPlanObjekt ( pPlanObjekt );
    else
      m_transObjAnz++;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Transformation eines BPlan-Objektes                                      //
///////////////////////////////////////////////////////////////////////////////
void MigrationV4::transformationBPlanObjekt ( Feature * pPlanObjekt )
{
  string klassenName;
  string bedingungText;
  string bedingungDatum;
  bool   success;
	size_t relAnz;
  string stringAttributWert;
  double doubleAttributWert;
  string gmlId;
  string gmlIdParent;
  string codeListPath;
  Feature                * pPlanObjNew;
  FeatureRelation        * pRel;
  vector<FeatureRelation*> vRelationen;

  gmlIdParent = pPlanObjekt->getGmlId();

  transformationMassDerBaulichenNutzung ( pPlanObjekt );

  success = pPlanObjekt->getStringAttributWert( "xplan:bedingungStart", bedingungText );
  DateAttr * pTime = pPlanObjekt->getDateAttributWert( "xplan:startDatum" );
  DateAttr * pTimeCopy = NULL;

  if ( success || pTime != NULL )
  {
    if ( success )
      pPlanObjekt->deleteStringAttribut ( "xplan:bedingungStart" );
    if ( pTime != NULL )
    {
      pTimeCopy = new DateAttr;
      pTimeCopy->value =  new TimePosition ( (TimePosition*)(pTime->value) );
      pPlanObjekt->deleteDateAttribut( "xplan:startDatum" );
    }

    gmlId = Constant::createGmlId();
    pPlanObjNew = new Feature ( ANY_OBJEKT, gmlId, m_pPlanObjekte, "xplan:BP_WirksamkeitBedingung" );
    if ( success )
      pPlanObjNew->setStringAttribut( "xplan:bedingung", bedingungText );
    if ( pTimeCopy != NULL )
      pPlanObjNew->setDateAttribut ( "xplan:datumAbsolut", pTimeCopy );
    m_pPlanObjekte->addFeature( pPlanObjNew );

    pPlanObjekt->addChild ( pPlanObjNew->getGmlId() );
    pPlanObjNew->setParentId( pPlanObjekt->getGmlId() );

    pRel = new FeatureRelation;
    pRel->featureId = pPlanObjNew->getGmlId();
    pRel->rollenName = "xplan:startBedingung";
    pRel->relToFeature = false;

    pPlanObjekt->addFeatureRelation( pRel );
  }

  success = pPlanObjekt->getStringAttributWert( "xplan:bedingungEnde", bedingungText );
  pTime = pPlanObjekt->getDateAttributWert( "xplan:endDatum" );
  pTimeCopy = NULL;

  if ( success || pTime != NULL )
  {
    if ( success )
      pPlanObjekt->deleteStringAttribut ( "xplan:bedingungEnde" );
    if (  pTime != NULL )
    {
      pTimeCopy = new DateAttr;
      pTimeCopy->value =  new TimePosition ( (TimePosition*)(pTime->value) );
      pPlanObjekt->deleteDateAttribut( "xplan:endDatum" );
    }

    gmlId = Constant::createGmlId();
    pPlanObjNew = new Feature ( ANY_OBJEKT, gmlId, m_pPlanObjekte, "xplan:BP_WirksamkeitBedingung" );
    if ( success )
      pPlanObjNew->setStringAttribut( "xplan:bedingung", bedingungText );
    if ( pTimeCopy != NULL )
      pPlanObjNew->setDateAttribut ( "xplan:datumAbsolut", pTimeCopy );
    m_pPlanObjekte->addFeature( pPlanObjNew );

    pPlanObjekt->addChild ( pPlanObjNew->getGmlId() );
    pPlanObjNew->setParentId( pPlanObjekt->getGmlId() );

    pRel = new FeatureRelation;
    pRel->featureId = pPlanObjNew->getGmlId();
    pRel->rollenName = "xplan:endeBedingung";
    pRel->relToFeature = false;

    pPlanObjekt->addFeatureRelation( pRel );
  }

  klassenName = pPlanObjekt->getKlassenName();
  
  if ( klassenName == "xplan:BP_GewaesserFlaeche" )
  {
    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success && stringAttributWert == "1300" )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:zweckbestimmung", "1100" );
    }
  }
  else
  if ( klassenName == "xplan:BP_Schutzgebiet" )
  {
    relAnz = pPlanObjekt->getFeatureRelationenAnz( "xplan:gehoertNachrichtlichZuBereich" );
    if ( relAnz > 0 )
      pPlanObjekt->setKlassenName( "xplan:SO_SchutzgebietNaturschutzrecht" );

    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );

      if ( stringAttributWert == "1550" )
      {
        stringAttributWert = "1500";
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "BP_Schutzgebiet", "zweckbestimmung",WARNUNG,
          "SO_SchutzgebietNaturschutzrecht", "artDerFestlegung", 
          "Zweckbestimmung \"FlaechenNaturdenkmal\" existiert nicht mehr",
          "Umsetzung auf Zweckbestimmung \"Naturdenkmal\"" );
      }

      if ( stringAttributWert == "1800" ) stringAttributWert = "18000";
      if ( stringAttributWert == "1900" ) stringAttributWert = "18001";

      if ( relAnz > 0 )
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", stringAttributWert );
      else
        pPlanObjekt->setStringAttribut( "xplan:zweckbestimmung", stringAttributWert );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
    if ( success )
    {

      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
      if ( m_codeSpaceCodeListsV4 != "" )
      {
        if ( relAnz > 0 )
        {
          codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizSchutzgebietNaturschutzrecht.xml";
          pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
        }
        else
        {
          codeListPath =  m_codeSpaceCodeListsV4 + "xplan_BP_DetailZweckbestNaturschutzgebiet.xml";
          pPlanObjekt->setStringAttribut( "xplan:detaillierteZweckbestimmung", stringAttributWert, "codeSpace", codeListPath );
        }
      }
      else
      {
        if ( relAnz > 0 )
          pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert );
        else
          pPlanObjekt->setStringAttribut( "xplan:detaillierteZweckbestimmung", stringAttributWert );
      }
    }
  }
  else
  if ( klassenName == "xplan:BP_Baugebiet" || klassenName == "xplan:BP_BaugebietsTeilFlaeche" )
  {
    success = pPlanObjekt->getStringAttributWert( "xplan:besondereArtDerBaulichenNutzung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:besondereArtDerBaulichenNutzung" );
      pPlanObjekt->setStringAttribut( "xplan:besondereArtDerBaulNutzung", stringAttributWert );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:versionBauNVO", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:versionBauNVO" );
      m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), klassenName, "versionBauNVO",WARNUNG,
        "", "", 
        "Attribut existiert nicht mehr",
        "" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:abweichendeBauweise", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:abweichendeBauweise" );
      if ( m_codeSpaceCodeListsV4 != "" )
      {
        codeListPath =  m_codeSpaceCodeListsV4 + "xplan_BP_AbweichendeBauweise.xml";
        pPlanObjekt->setStringAttribut( "xplan:abweichendeBauweise", stringAttributWert, "codeSpace", codeListPath );
      }
      else
        pPlanObjekt->setStringAttribut( "xplan:abweichendeBauweise", stringAttributWert );
    }

  }
  else
  if ( klassenName == "xplan:BP_AbstandsFlaeche" )
  {
    success = pPlanObjekt->getDoubleAttributWert( "xplan:tiefe", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteDoubleAttribut( "xplan:tiefe" );
      pPlanObjekt->setDoubleAttribut( "xplan:tiefe", doubleAttributWert, m_pUOMList->getUomLength() );
    }
  }
  else
  if ( klassenName == "xplan:BP_BauGrenze" || klassenName == "xplan:BP_BauLinie" )
  {
    success = pPlanObjekt->getDoubleAttributWert( "xplan:bautiefe", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteDoubleAttribut( "xplan:bautiefe" );
      pPlanObjekt->setDoubleAttribut( "xplan:bautiefe", doubleAttributWert, m_pUOMList->getUomLength() );
    }
  }
  else
  if ( klassenName == "xplan:BP_SpezielleBauweise" )
  {
    success = pPlanObjekt->getDoubleAttributWert( "xplan:BMin", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteDoubleAttribut( "xplan:BMin" );
      pPlanObjekt->setDoubleAttribut( "xplan:Bmin", doubleAttributWert, m_pUOMList->getUomLength() );
    }

    success = pPlanObjekt->getDoubleAttributWert( "xplan:BMax", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteDoubleAttribut( "xplan:BMax" );
      pPlanObjekt->setDoubleAttribut( "xplan:Bmax", doubleAttributWert, m_pUOMList->getUomLength() );
    }

    success = pPlanObjekt->getDoubleAttributWert( "xplan:TMin", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteDoubleAttribut( "xplan:TMin" );
      pPlanObjekt->setDoubleAttribut( "xplan:Tmin", doubleAttributWert, m_pUOMList->getUomLength() );
    }

    success = pPlanObjekt->getDoubleAttributWert( "xplan:TMax", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteDoubleAttribut( "xplan:TMax" );
      pPlanObjekt->setDoubleAttribut( "xplan:Tmax", doubleAttributWert, m_pUOMList->getUomLength() );
    }

  }
  else
  if ( klassenName == "xplan:BP_DenkmalschutzEinzelanlagePunkt" )
  {
    pPlanObjekt->setKlassenName( "xplan:BP_DenkmalschutzEinzelanlage" );
    relAnz = pPlanObjekt->getFeatureRelationenAnz( "xplan:gehoertNachrichtlichZuBereich" );
    if ( relAnz > 0 )
    {
      pPlanObjekt->setKlassenName( "xplan:SO_Denkmalschutzrecht" );
      pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1100" );
    }
  }
  else
  if ( klassenName == "xplan:BP_DenkmalschutzEnsembleFlaeche" )
  {
    relAnz = pPlanObjekt->getFeatureRelationenAnz( "xplan:gehoertNachrichtlichZuBereich" );
    if (  relAnz > 0 )
    {
      pPlanObjekt->setKlassenName( "xplan:SO_Denkmalschutzrecht" );
      pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
    }
  }
  else
  if ( klassenName == "xplan:BP_Grabungsschutzgebiet" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_Denkmalschutzrecht" );
    pPlanObjekt->setStringAttribut ( "xplan:artDerFestlegung", "1200" );
  }
  else
  if ( klassenName == "xplan:BP_SchutzPflegeEntwicklungsMassnahme" ||
       klassenName == "xplan:BP_SchutzPflegeEntwicklungsFlaeche" ||
       klassenName == "xplan:BP_AusgleichsMassnahme" ||
       klassenName == "xplan:BP_AusgleichsFlaeche" )
  {
    string massnahme;
    string massnahmeText;
    string massnahmeKuerzel;
    bool   createRelation;

    createRelation = false;

    success = pPlanObjekt->getStringAttributWert( "xplan:massnahme", massnahme );
    if ( success )
    {
      createRelation = true;
      pPlanObjekt->deleteStringAttribut( "xplan:massnahme" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:massnahmeText", massnahmeText );
    if ( success )
    {
      createRelation = true;
      pPlanObjekt->deleteStringAttribut( "xplan:massnahmeText" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:massnahmeKuerzel", massnahmeKuerzel );
    if ( success )
    {
      createRelation = true;
      pPlanObjekt->deleteStringAttribut( "xplan:massnahmeKuerzel" );
    }

    if ( createRelation )
    {
      gmlId = Constant::createGmlId();
      pPlanObjNew = new Feature ( ANY_OBJEKT, gmlId, m_pPlanObjekte, "xplan:XP_SPEMassnahmenDaten" );
      if ( massnahme != "" )
        pPlanObjNew->setStringAttribut ( "xplan:klassifizMassnahme", massnahme );
      if ( massnahmeText != "" )
        pPlanObjNew->setStringAttribut ( "xplan:massnahmeText", massnahmeText );
      if ( massnahmeKuerzel != "" )
        pPlanObjNew->setStringAttribut ( "xplan:massnahmeKuerzel", massnahmeKuerzel );
      m_pPlanObjekte->addFeature( pPlanObjNew );

      pPlanObjekt->addChild ( pPlanObjNew->getGmlId() );
      pPlanObjNew->setParentId( pPlanObjekt->getGmlId() );

      pRel = new FeatureRelation();
      pRel->featureId = pPlanObjNew->getGmlId();
      pRel->rollenName    = "xplan:massnahme";
      pRel->relToFeature  = false;

      pPlanObjekt->addFeatureRelation( pRel );
    }
  }
  else
  if ( klassenName == "xplan:BP_AnpflanzungBindungErhaltung" )
  {
    success = pPlanObjekt->getDoubleAttributWert( "xplan:kronendurchmesser", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteDoubleAttribut( "xplan:kronendurchmesser" );
      pPlanObjekt->setDoubleAttribut( "xplan:kronendurchmesser", doubleAttributWert, m_pUOMList->getUomLength() );
    }

  }
  else
  if ( klassenName == "xplan:BP_HoehenPunkt" )
  {
    success = pPlanObjekt->getDoubleAttributWert( "xplan:hoehe", doubleAttributWert );
    if ( success )
    {
      pPlanObjekt->setKlassenName( "xplan:BP_HoehenMass" );
      pPlanObjekt->deleteDoubleAttribut( "xplan:hoehe" );
      
      gmlId = Constant::createGmlId();
      pPlanObjNew = new Feature ( ANY_OBJEKT, gmlId, m_pPlanObjekte, "xplan:XP_Hoehenangabe" );
      pPlanObjNew->setStringAttribut( "xplan:hoehenbezug", "1000" );
      pPlanObjNew->setDoubleAttribut ( "xplan:hZwingend", doubleAttributWert, m_pUOMList->getUomLength() );
      
      m_pPlanObjekte->addFeature( pPlanObjNew );

      pPlanObjekt->addChild ( pPlanObjNew->getGmlId() );
      pPlanObjNew->setParentId( pPlanObjekt->getGmlId() );

      pRel = new FeatureRelation();
      pRel->featureId = pPlanObjNew->getGmlId();
      pRel->rollenName    = "xplan:hoehenangabe";
      pRel->relToFeature  = false;

      pPlanObjekt->addFeatureRelation( pRel );
    }
  }
  else
  if ( klassenName == "xplan:BP_Bauschutzbereich" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_SonstigesRecht" );
    pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
  }
  else
  if ( klassenName == "xplan:BP_Fachgesetz" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_SonstigesRecht" );
    pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "9999" );
    success = pPlanObjekt->getStringAttributWert( "xplan:bestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:bestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:name", stringAttributWert );
    }
  }
  else
  if ( klassenName == "xplan:BP_TechnikBestimmteFlaeche" )
  {
    pPlanObjekt->setKlassenName( "xplan:BP_ErneuerbareEnergieFlaeche" );
  }
  else
  if ( klassenName == "xplan:BP_Laermschutzbereich" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_SchutzgebietSonstigesRecht" );
    pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000");
    success = pPlanObjekt->getStringAttributWert( "xplan:schutzzone", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->setStringAttribut ( "xplan:zone", stringAttributWert );
      pPlanObjekt->deleteStringAttribut( "xplan:schutzzone" );
    }
  }
  else
  if ( klassenName == "xplan:BP_BahnVerkehr" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_Schienenverkehrsrecht" );

    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", stringAttributWert );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:besondereZweckbestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:besondereZweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:besondereArtDerFestlegung", stringAttributWert );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
    if ( success )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizNachSchienenverkehrsrecht.xml";

      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
    }

  }
  else
  if ( klassenName == "xplan:BP_LuftverkehrFlaeche" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_Luftverkehrsrecht" );

    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", stringAttributWert );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
    if ( success )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizNachLuftverkehrsrecht.xml";

      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert, "codeSpace",codeListPath );
    }

  }
  else
  if ( klassenName == "xplan:BP_StrassenVerkehrsFlaeche" )
  {
    relAnz = pPlanObjekt->getFeatureRelationenAnz( "xplan:gehoertNachrichtlichZuBereich" );
    if (  relAnz > 0 )
    {
      pPlanObjekt->setKlassenName( "xplan:SO_Strassenverkehrsrecht" );
      pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "9999" );

      success =  pPlanObjekt->getStringAttributWert( "xplan:nutzungsform", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:nutzungsform" );
        if ( stringAttributWert == "1000" )
        {
          m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "BP_StrassenVerkehrsFlaeche", "nutzungsform",FEHLER, 
            "SO_Strassenverkehrsrecht", "", 
            "Private Strassenverkehrsflächen können nicht nachrichtlich übernommen werden",
            "Umsetzung auf \"Sonstige öffentliche Strasse\"" );
        }
      }
    }
  }
  else
  if ( klassenName == "xplan:BP_WaldFlaeche" )
  {
    relAnz = pPlanObjekt->getFeatureRelationenAnz( "xplan:gehoertNachrichtlichZuBereich" );
    if (  relAnz > 0 )
    {
      pPlanObjekt->setKlassenName( "xplan:SO_Forstrecht" );
      pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "9999" );

      success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "BP_WaldFlaeche", "zweckbestimmung",FEHLER, 
          "SO_Forstrecht", "", 
          "Klassifikation von Waldflächen kann nicht nachrichtlich übernommen werden",
          "Attribut \"zweckbestimmung\" wird nicht übernommen" );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:weitereZweckbestimmung1", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:weitereZweckbestimmung1" );
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "BP_WaldFlaeche", "zweckbestimmung",FEHLER, 
          "SO_Forstrecht", "", 
          "Klassifikation von Waldflächen kann nicht nachrichtlich übernommen werden",
          "Attribut \"weitereZweckbestimmung1\" wird nicht übernommen" );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:weitereZweckbestimmung2", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:weitereZweckbestimmung2" );
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "BP_WaldFlaeche", "zweckbestimmung",FEHLER, 
          "SO_Forstrecht", "", 
          "Klassifikation von Waldflächen kann nicht nachrichtlich übernommen werden",
          "Attribut \"weitereZweckbestimmung2\" wird nicht übernommen" );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
      if ( success )
      {
        codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizNachForstrecht.xml";

        pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
        pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:weitereDetailZweckbestimmung1", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:weitereDetailZweckbestimmung1" );
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "BP_WaldFlaeche", "zweckbestimmung",FEHLER, 
          "SO_Forstrecht", "", 
          "Mehrere detaillierte Klassifikationen von Waldflächen können nicht nachrichtlich übernommen werden",
          "Atribut \"weitereDetailZweckbestimmung1\" wird nicht übernommen" );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:weitereDetailZweckbestimmung2", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:weitereDetailZweckbestimmung2" );
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "BP_WaldFlaeche", "zweckbestimmung",FEHLER, 
          "SO_Forstrecht", "", 
          "Mehrere detaillierte Klassifikationen von Waldflächen können nicht nachrichtlich übernommen werden",
          "Atribut \"weitereDetailZweckbestimmung2\" wird nicht übernommen" );
      }
    }
  }
  else
  if ( klassenName == "xplan:BP_WasserrechtlicheFestsetzungsFlaeche" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_SchutzgebietWasserrecht" );

    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );

      if ( stringAttributWert == "1000")
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
      else
      if ( stringAttributWert == "1100")
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "10001" );
      else
      if ( stringAttributWert == "1200")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
        pPlanObjekt->setStringAttribut( "xplan:zone", "1000" );
      }
      else
      if ( stringAttributWert == "1300")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
        pPlanObjekt->setStringAttribut( "zone", "1100" );
      }
      else
      if ( stringAttributWert == "1400")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
        pPlanObjekt->setStringAttribut( "zone", "1200" );
      }
      else
      if ( stringAttributWert == "1500")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
        pPlanObjekt->setStringAttribut( "zone", "1300" );
      }
      else
      if ( stringAttributWert == "1600")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
        pPlanObjekt->setStringAttribut( "zone", "1400" );
      }
      if ( stringAttributWert == "9999")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "9999" );
      }
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
    if ( success )
    {     
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizSchutzgebietWasserrecht.xml";
     
      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
    }
  }
  else
  if ( klassenName == "xplan:BP_VorbHochwSchutzFlaeche" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_Wasserrecht" );

    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );

      if ( stringAttributWert == "1000")
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "20000" );
      else
      if ( stringAttributWert == "1100")
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "20001" );
      else
      if ( stringAttributWert == "1200")
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "20002" );
      else
      if ( stringAttributWert == "9999")
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "9999" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
    if ( success )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizNachWasserrecht.xml";

      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:detaillierteArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
    }
  }
  else
  if ( klassenName == "xplan:BP_LandwirtschaftsFlaeche" )
  {
    pPlanObjekt->setKlassenName( "xplan:BP_Landwirtschaft" );
    m_pPlanObjekte->addFeature ( pPlanObjNew );
  }
  else
  if ( klassenName == "xplan:BP_LandwirtschaftsLinie" )
  {
    pPlanObjekt->setKlassenName( "xplan:BP_Landwirtschaft" );
   m_pPlanObjekte->addFeature ( pPlanObjNew );
  }
  else
  if ( klassenName == "xplan:BP_GebaeudeFlaeche" )
  {
    bool bestehend;

    success = pPlanObjekt->getBoolAttributWert( "xplan:bestehend", bestehend );
    if ( success )
    {
      pPlanObjekt->deleteBoolAttribut( "bestehend" );
      success = pPlanObjekt->getStringAttributWert( "xplan:rechtsstand", stringAttributWert );
      if ( bestehend  )
      {
        if ( success )
        {
          if ( stringAttributWert != "2000" )
          {
            pPlanObjekt->deleteStringAttribut( "xplan:rechtsstand" );
            pPlanObjekt->setStringAttribut( "xplan:rechtsstand", "2000" );
          }
        }
        else
          pPlanObjekt->setStringAttribut( "xplan:rechtsstand", "2000" );
      }
      else
      {
        if ( success )
        {
          if ( stringAttributWert != "1000" )
          {
            pPlanObjekt->deleteStringAttribut( "xplan:rechtsstand" );
            pPlanObjekt->setStringAttribut( "xplan:rechtsstand", "1000" );
          }
        }
        else
          pPlanObjekt->setStringAttribut( "xplan:rechtsstand", "1000" );
      }
    }
  }

  m_transObjAnz++;

}
///////////////////////////////////////////////////////////////////////////////
//  Transformation der Parameter für das Mass der baulichen Nutzung          //
///////////////////////////////////////////////////////////////////////////////
void MigrationV4::transformationMassDerBaulichenNutzung ( Feature * pPlanObjekt )
{
  bool   success;
  int    integerAttributWert;
  double doubleAttributWert;

  success = pPlanObjekt->getIntegerAttributWert( "xplan:Fmin", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:Fmin" );
    pPlanObjekt->setDoubleAttribut( "xplan:Fmin", integerAttributWert, m_pUOMList->getUomArea() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:Fmax", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:Fmax" );
    pPlanObjekt->setDoubleAttribut( "xplan:Fmax", integerAttributWert, m_pUOMList->getUomArea() );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:Bmin", doubleAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteDoubleAttribut( "xplan:Bmin" );
    pPlanObjekt->setDoubleAttribut( "xplan:Bmin", integerAttributWert, m_pUOMList->getUomLength() );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:Bmax", doubleAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteDoubleAttribut( "xplan:Bmax" );
    pPlanObjekt->setDoubleAttribut( "xplan:Bmax", integerAttributWert, m_pUOMList->getUomLength() );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:TMin", doubleAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteDoubleAttribut( "xplan:Tmin" );
    pPlanObjekt->setDoubleAttribut( "xplan:Tmin", integerAttributWert, m_pUOMList->getUomLength() );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:Tmax", doubleAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteDoubleAttribut( "xplan:Tmax" );
    pPlanObjekt->setDoubleAttribut( "xplan:Tmax", integerAttributWert, m_pUOMList->getUomLength() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:GFmin", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:GFmin" );
    pPlanObjekt->setDoubleAttribut( "xplan:GFmin", integerAttributWert, m_pUOMList->getUomArea() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:GFmax", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:GFmax" );
    pPlanObjekt->setDoubleAttribut( "xplan:GFmax", integerAttributWert, m_pUOMList->getUomArea() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:GF", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:GF" );
    pPlanObjekt->setDoubleAttribut( "xplan:GF", integerAttributWert, m_pUOMList->getUomArea() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:GF_Ausn", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:GF_Ausn" );
    pPlanObjekt->setDoubleAttribut( "xplan:GF_Ausn", integerAttributWert, m_pUOMList->getUomArea() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:BMmin", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:BMmin" );
    pPlanObjekt->setDoubleAttribut( "xplan:BMmin", integerAttributWert, m_pUOMList->getUomVolume() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:BMmax", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:BMmax" );
    pPlanObjekt->setDoubleAttribut( "xplan:BMmax", integerAttributWert, m_pUOMList->getUomVolume() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:BM", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:BM" );
    pPlanObjekt->setDoubleAttribut( "xplan:BM", integerAttributWert, m_pUOMList->getUomVolume() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:BM_Ausn", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:BM_Ausn" );
    pPlanObjekt->setDoubleAttribut( "xplan:BM_Ausn", integerAttributWert, m_pUOMList->getUomVolume() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:GRmin", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:GRmin" );
    pPlanObjekt->setDoubleAttribut( "xplan:GRmin", integerAttributWert, m_pUOMList->getUomArea() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:GRmax", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:GRmax" );
    pPlanObjekt->setDoubleAttribut( "xplan:GRmax", integerAttributWert, m_pUOMList->getUomArea() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:GR", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:GR" );
    pPlanObjekt->setDoubleAttribut( "xplan:GR", integerAttributWert, m_pUOMList->getUomArea() );
  }

  success = pPlanObjekt->getIntegerAttributWert( "xplan:GR_Ausn", integerAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteIntegerAttribut( "xplan:GR_Ausn" );
    pPlanObjekt->setDoubleAttribut( "xplan:GR_Ausn", integerAttributWert, m_pUOMList->getUomArea() );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:DNmin", doubleAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteDoubleAttribut( "xplan:DNmin" );
    pPlanObjekt->setDoubleAttribut( "xplan:DNmin", doubleAttributWert, m_pUOMList->getUomAngle() );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:DNmax", doubleAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteDoubleAttribut( "xplan:DNmax" );
    pPlanObjekt->setDoubleAttribut( "xplan:DNmax", doubleAttributWert, m_pUOMList->getUomAngle() );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:DN", doubleAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteDoubleAttribut( "xplan:DN" );
    pPlanObjekt->setDoubleAttribut( "xplan:DN", doubleAttributWert, m_pUOMList->getUomAngle() );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:DNZwingend", doubleAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteDoubleAttribut( "xplan:DNZwingend" );
    pPlanObjekt->setDoubleAttribut( "xplan:DNZwingend", doubleAttributWert, m_pUOMList->getUomAngle() );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:FR", doubleAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteDoubleAttribut( "xplan:FR" );
    pPlanObjekt->setDoubleAttribut( "xplan:FR", doubleAttributWert, m_pUOMList->getUomAngle() );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Transformation eines FPlan-Obektes                                       //
///////////////////////////////////////////////////////////////////////////////
void MigrationV4::transformationFPlanObjekt ( Feature * pPlanObjekt )
{
  string klassenName;
  bool   success;
	size_t relAnz;
  string stringAttributWert;
  string gmlId;
  string parentId;
  string codeListPath;

  Feature                * pPlanObjNew;
  FeatureRelation        * pRel;
  vector<FeatureRelation*> vRelationen;

  klassenName = pPlanObjekt->getKlassenName();

  parentId = pPlanObjekt->getGmlId();

  if ( klassenName == "xplan:FP_Gewaesser" )
  {
    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success && stringAttributWert == "1300" )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:zweckbestimmung", "1100" );

      m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "FP_Gewaesser", "zweckbestimmung",WARNUNG, 
        "", "", 
        "Zweckbestimmung \"Bundeswasserstrasse\" wird nicht mehr unterstützt",
        "Umsetzung auf Zweckbestimmung \"Wasserflaeche\"" );
    }
  }
  else
  if ( klassenName == "xplan:FP_BebauungsFlaeche"  )
  {
    success = pPlanObjekt->getStringAttributWert( "xplan:versionBauNVO", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:versionBauNVO" );
      m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), klassenName, "versionBauNVO",WARNUNG,
        "", "", 
        "Attribut existiert nicht mehr",
        "" );
    }
  }
  else
  if ( klassenName == "xplan:FP_Grabungsschutzgebiet" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_Denkmalschutzrecht" );
    pPlanObjekt->setStringAttribut ( "xplan:artDerFestlegung", "1200" );
  }
  else
  if ( klassenName == "xplan:FP_DenkmalschutzEnsemble" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_Denkmalschutzrecht" );
    pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );

    success = pPlanObjekt->getStringAttributWert ( "xplan:denkmal", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut ( "xplan:denkmal" );
      pPlanObjekt->setStringAttribut( "xplan:name", stringAttributWert );
    }
  }
  else
  if ( klassenName == "xplan:FP_Erhaltungssatzung" )
  {
    if ( pPlanObjekt->getGeometrieTyp() != MULTI_SURFACE )
    {
      m_pPlanObjekte->removeFeature( pPlanObjekt->getGmlId() );

      m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "FP_Erhaltungssatzung", "", FEHLER, 
        "SO_Gebiet", "", 
        "Objekt kann nur mit flächenhaftem Raumbezug übernommen werden",
        "Objekt wird nicht migriert" );
      m_transObjAnz--;
    }
    else
    {
      pPlanObjekt->setKlassenName( "xplan:SO_Gebiet" );
      pPlanObjekt->setBoolAttribut( "xplan:flaechenschluss", false );

      success = pPlanObjekt->getStringAttributWert( "xplan:grund", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:grund" );

        if ( stringAttributWert == "1000" )
          pPlanObjekt->setStringAttribut( "xplan:gebietsArt", "2000" );
        else
        if ( stringAttributWert == "2000" )
          pPlanObjekt->setStringAttribut( "xplan:gebietsArt", "2100" );
        else
        if ( stringAttributWert == "3000" )
          pPlanObjekt->setStringAttribut( "xplan:gebietsArt", "2200" );
      }
    }
  }
  else
  if ( klassenName == "xplan:FP_SchutzPflegeEntwicklung" ||
       klassenName == "xplan:FP_AusgleichsFlaeche" )
  {
    string massnahme;
    string massnahmeText;
    string massnahmeKuerzel;
    bool   createRelation;

    createRelation = false;

    success = pPlanObjekt->getStringAttributWert( "xplan:massnahme", massnahme );
    if ( success )
    {
      createRelation = true;
      pPlanObjekt->deleteStringAttribut( "xplan:massnahme" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:massnahmeText", massnahmeText );
    if ( success )
    {
      createRelation = true;
      pPlanObjekt->deleteStringAttribut( "xplan:massnahmeText" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:massnahmeKuerzel", massnahmeKuerzel );
    if ( success )
    {
      createRelation = true;
      pPlanObjekt->deleteStringAttribut( "xplan:massnahmeKuerzel" );
    }

    if ( createRelation )
    {
      gmlId = Constant::createGmlId();
      pPlanObjNew = new Feature ( ANY_OBJEKT, gmlId, m_pPlanObjekte, "xplan:XP_SPEMassnahmenDaten" );
      if ( massnahme != "" )
        pPlanObjNew->setStringAttribut ( "xplan:klassifizMassnahme", massnahme );
      if ( massnahmeText != "" )
        pPlanObjNew->setStringAttribut ( "xplan:massnahmeText", massnahmeText );
      if ( massnahmeKuerzel != "" )
        pPlanObjNew->setStringAttribut ( "xplan:massnahmeKuerzel", massnahmeKuerzel );
      m_pPlanObjekte->addFeature( pPlanObjNew );

      pPlanObjekt->addChild ( pPlanObjNew->getGmlId() );
      pPlanObjNew->setParentId( pPlanObjekt->getGmlId() );

      pRel = new FeatureRelation();
      pRel->featureId = pPlanObjNew->getGmlId();
      pRel->rollenName    = "xplan:massnahme";
      pRel->relToFeature  = false;

      pPlanObjekt->addFeatureRelation( pRel );
    }
  }
  else
  if ( klassenName == "xplan:FP_Schutzgebiet" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_SchutzgebietNaturschutzrecht" );

    success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", stringAttributWert );
    if ( success )
    {
      if ( stringAttributWert == "1000" || stringAttributWert == "2000" )
        pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );

      if ( stringAttributWert == "1550" ) stringAttributWert = "1500";
      if ( stringAttributWert == "1800" ) stringAttributWert = "18000";
      if ( stringAttributWert == "1900" ) stringAttributWert = "18001";

      pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", stringAttributWert );

      m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "FP_Schutzgebiet", "zweckbestimmung",WARNUNG, 
        "SO_SchutzgebietNaturschutzrecht", "artDerFestlegung", 
        "Zweckbestimmung \"FlaechenNaturdenkmal\" existiert nicht mehr",
        "Umsetzung auf Zweckbestimmung \"Naturdenkmal\"" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
    if ( success )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizSchutzgebietNaturschutzrecht.xml";

      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
      if ( m_codeSpaceCodeListsV4 != "" )
          pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
      else
          pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert );
    }
  }
  else
  if ( klassenName == "xplan:FP_PriviligiertesVorhaben" )
  {
    pPlanObjekt->setKlassenName( "xplan:FP_PrivilegiertesVorhaben" );
  }
  else
  if ( klassenName == "xplan:FP_Fachgesetz" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_SonstigesRecht" );

    success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", stringAttributWert );
    if ( success )
    {
      if ( stringAttributWert == "1000" || stringAttributWert == "2000" )
        pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
    }

    pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "9999" );
    success = pPlanObjekt->getStringAttributWert( "xplan:bestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:bestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:name", stringAttributWert );
    }
  }
  else
  if ( klassenName == "xplan:FP_Bauschutzbereich" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_SonstigesRecht" );

    success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", stringAttributWert );
    if ( success )
    {
      if ( stringAttributWert == "1000" || stringAttributWert == "2000" )
        pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
    }

    pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
  }
  else
  if ( klassenName == "xplan:FP_Laermschutzbereich" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_SchutzgebietSonstigesRecht" );

    success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", stringAttributWert );
    if ( success )
    {
      if ( stringAttributWert == "1000" || stringAttributWert == "2000" )
        pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
    }

    pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
  }
  else
  if ( klassenName == "xplan:FP_Bahnverkehr" )
    {
      pPlanObjekt->setKlassenName( "xplan:SO_Schienenverkehrsrecht" );

      success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", stringAttributWert );
      if ( success )
      {
        if ( stringAttributWert == "1000" || stringAttributWert == "2000" )
        pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", stringAttributWert );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:besondereZweckbestimmung", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:besondereZweckbestimmung" );
        pPlanObjekt->setStringAttribut( "xplan:besondereArtDerFestlegung", stringAttributWert );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
      if ( success )
      {
        codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizNachSchienenverkehrsrecht.xml";

        pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
        pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
      }

    }
  else
  if ( klassenName == "xplan:FP_Luftverkehr" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_Luftverkehrsrecht" );

    success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", stringAttributWert );
    if ( success )
    {
      if ( stringAttributWert == "1000" || stringAttributWert == "2000" )
        pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", stringAttributWert );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
    if ( success )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizNachLuftverkehrsrecht.xml";
      
      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
    }

  }
  else
  if ( klassenName == "xplan:FP_Strassenverkehr" )
  {
    relAnz = pPlanObjekt->getFeatureRelationenAnz( "xplan:gehoertNachrichtlichZuBereich" );
    if (  relAnz > 0 )
    {
      pPlanObjekt->setKlassenName( "xplan:SO_Strassenverkehrsrecht" );

      success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", stringAttributWert );
      if ( success )
      {
        if ( stringAttributWert == "1000" || stringAttributWert == "2000" )
          pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
      }

      success = pPlanObjekt->getStringAttributWert ( "xplan:zweckbestimmung", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );
        if ( stringAttributWert == "1000" )
          pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
        else
        {
          pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "9999" );
          m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "FP_Strassenverkehr", "zweckbestimmung",WARNUNG,
            "SO_Strassenverkehrsrecht", "artDerFestlegung",
            "Die spezifizierte Zweckbestimmung kann nachrichtlich nicht übernommen werden",
            "Umsetzung auf Zweckbestimmung \"Sonstige öffentliche Strasse\"" );
        }
      }
      else
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "9999" );

      success = pPlanObjekt->getStringAttributWert( "xplan:besondereZweckbestimmung", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:besondereZweckbestimmung" );
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "FP_Strassenverkehr", "besondereZweckbestimmung", FEHLER,
          "SO_Strassenverkehrsrecht", "artDerFestlegung",
          "Das Attribut \"besondereZweckbestimmung\" kann nachrichtlich nicht übernommen werden",
          "Attribut wird nicht übernommen" );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
      if ( success )
      {
        codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizNachStrassenverkehrsrecht.xml";

        pPlanObjekt->deleteStringAttribut ( "xplan:detaillierteZweckbestimmung" );
        pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
      }

      success =  pPlanObjekt->getStringAttributWert( "xplan:nutzungsform", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:nutzungsform" );
        if ( stringAttributWert == "1000" )
        {
          m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "FP_Strassenverkehr", "nutzungsform",FEHLER, 
            "SO_Strassenverkehrsrecht", "", 
            "Private Strassenverkehrsflächen können nicht nachrichtlich übernommen werden",
            "Umsetzung auf \"Sonstige öffentliche Strasse\"" );
        }
      }
    }
  }
  else
  if ( klassenName == "xplan:FP_WaldFlaeche" )
  {
    relAnz = pPlanObjekt->getFeatureRelationenAnz( "xplan:gehoertNachrichtlichZuBereich" );
    if (  relAnz > 0 )
    {
      pPlanObjekt->setKlassenName( "xplan:SO_Forstrecht" );

      success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", stringAttributWert );
      if ( success )
      {
        if ( stringAttributWert == "1000" || stringAttributWert == "2000" )
          pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
      }

      pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "9999" );

      success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "FP_WaldFlaeche", "zweckbestimmung",FEHLER, 
          "SO_Forstrecht", "", 
          "Klassifikation von Waldflächen kann nicht nachrichtlich übernommen werden",
          "Atribut \"zweckbestimmung\" wird nicht übernommen" );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:weitereZweckbestimmung1", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:weitereZweckbestimmung1" );
        //  Fehlermeldung
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:weitereZweckbestimmung2", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:weitereZweckbestimmung2" );
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "FP_WaldFlaeche", "zweckbestimmung",FEHLER, 
          "SO_Forstrecht", "", 
          "Klassifikation von Waldflächen kann nicht nachrichtlich übernommen werden",
          "Atribut \"weitereZweckbestimmung1\" wird nicht übernommen" );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
      if ( success )
      {
        codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizNachForstrecht.xml";

        pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
        pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:weitereDetailZweckbestimmung1", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:weitereDetailZweckbestimmung1" );
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "FP_WaldFlaeche", "zweckbestimmung",FEHLER, 
          "SO_Forstrecht", "", 
          "Klassifikation von Waldflächen kann nicht nachrichtlich übernommen werden",
          "Atribut \"weitereDetailZweckbestimmung1\" wird nicht übernommen" );
      }

      success = pPlanObjekt->getStringAttributWert( "xplan:weitereDetailZweckbestimmung2", stringAttributWert );
      if ( success )
      {
        pPlanObjekt->deleteStringAttribut( "xplan:weitereDetailZweckbestimmung2" );
        m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "BP_WaldFlaeche", "zweckbestimmung",FEHLER, 
          "SO_Forstrecht", "", 
          "Klassifikation von Waldflächen kann nicht nachrichtlich übernommen werden",
          "Atribut \"weitereDetailZweckbestimmung2\" wird nicht übernommen" );
      }

    }
  }
  else
  if ( klassenName == "xplan:FP_Wasserrecht" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_SchutzgebietWasserrecht" );

    success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", stringAttributWert );
    if ( success )
    {
      if ( stringAttributWert == "1000" || stringAttributWert == "2000" )
        pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );

      if ( stringAttributWert == "1000")
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
      else
      if ( stringAttributWert == "1100")
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "10001" );
      else
      if ( stringAttributWert == "1200")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
        pPlanObjekt->setStringAttribut( "xplan:zone", "1000" );
      }
      else
      if ( stringAttributWert == "1300")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
        pPlanObjekt->setStringAttribut( "xplan:zone", "1100" );
      }
      else
      if ( stringAttributWert == "1400")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
        pPlanObjekt->setStringAttribut( "xplan:zone", "1200" );
      }
      else
      if ( stringAttributWert == "1500")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
        pPlanObjekt->setStringAttribut( "xplan:zone", "1300" );
      }
      else
      if ( stringAttributWert == "1600")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );
        pPlanObjekt->setStringAttribut( "xplan:zone", "1400" );
      }
      if ( stringAttributWert == "9999")
      {
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "9999" );
      }
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
    if ( success )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizSchutzgebietWasserrecht.xml";

      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
    }
  }
  else
  if ( klassenName == "xplan:FP_VorbHochwSchutz" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_Wasserrecht" );

    success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", stringAttributWert );
    if ( success )
    {
      if ( stringAttributWert == "1000" || stringAttributWert == "2000" )
        pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );

      if ( stringAttributWert == "1000")
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "20000" );
      else
      if ( stringAttributWert == "1100")
      pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "20001" );
      else
      if ( stringAttributWert == "1200")
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "20002" );
      else
      if ( stringAttributWert == "9999")
        pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "9999" );
    }

    success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
    if ( success )
    {
      codeListPath =  m_codeSpaceCodeListsV4 + "xplan_SO_DetailKlassifizNachWasserrecht.xml";

      pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
      pPlanObjekt->setStringAttribut( "xplan:detaillierteArtDerFestlegung", stringAttributWert, "codeSpace", codeListPath );
    }
  }
  m_transObjAnz++;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformation eines Präsentationsobjektes                               //
///////////////////////////////////////////////////////////////////////////////
void MigrationV4::transformationPraesentationsobjekt ( Feature * pPlanObjekt )
{
  string klassenName;
  bool   success;
  string stringAttributWert;

  klassenName = pPlanObjekt->getKlassenName();

  if ( klassenName == "xplan:XP_PPO" )
  {
    success = pPlanObjekt->getURLAttributWert( "xplan:externesSymbolURL", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->deleteURLAttribut( "xplan:externesSymbolURL" );
      m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "XP_PPO", "externesSymbolURL",FEHLER, 
        "", "", 
        "Attribut wird in XPlanGML 4.0 nicht mehr unterstützt",
        "Attribut wird nicht übernommen" );
    }
  }
  m_transObjAnz++;
}

///////////////////////////////////////////////////////////////////////////////
//  Transformation eines Rasterobjektes                                      //
///////////////////////////////////////////////////////////////////////////////
void MigrationV4::transformationRasterdarstellung ( Feature * pPlanObjekt )
{
  string                   klassenName;
  bool                     success;
  string                   stringAttributWert;
  string                   gmlId;
  string                   parentId;
  Feature                * pPlanObjNew;
  Feature                * pPlanObjektRef;
  FeatureRelation        * pRel;
  vector<FeatureRelation*> vRelationen;
  unsigned int             i;

  klassenName = pPlanObjekt->getKlassenName();

  parentId = pPlanObjekt->getGmlId();

  pPlanObjekt->getFeatureRelationen ( "xplan:refText", vRelationen );
  pPlanObjekt->getFeatureRelationen ( "xplan:refLegende", vRelationen );

  for ( i = 0; i < vRelationen.size(); i++ )
  {
    pRel = vRelationen[i];
    pPlanObjektRef = m_pPlanObjekte->getFeature( pRel->featureId );
    if ( pPlanObjekt != NULL )
    {
      pPlanObjekt->addChild ( pPlanObjektRef->getGmlId() );
      pPlanObjektRef->setParentId( pPlanObjekt->getGmlId() );
    }
  }

  vRelationen.clear();

  pPlanObjekt->getFeatureRelationen ( "xplan:refScan", vRelationen );
  for ( i = 0; i < vRelationen.size(); i++ )
  {
    pRel = vRelationen[i];
    pPlanObjektRef = m_pPlanObjekte->getFeature( pRel->featureId );
    if ( pPlanObjektRef != NULL )
    {
      pPlanObjektRef->setKlassenName( "xplan:XP_ExterneReferenz" );
      pPlanObjektRef->setStringAttribut( "xplan:art", "PlanMitGeoreferenz" );

      pPlanObjekt->addChild ( pPlanObjektRef->getGmlId() );
      pPlanObjektRef->setParentId( pPlanObjekt->getGmlId() );
    }
  }

  success = pPlanObjekt->getURLAttributWert( "xplan:beschreibungURL", stringAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteURLAttribut( "xplan:beschreibungURL" );

    gmlId = Constant::createGmlId();
    pPlanObjNew = new Feature ( ANY_OBJEKT, gmlId, m_pPlanObjekte, "xplan:XP_ExterneReferenz" );
    pPlanObjNew->setURLAttribut ( "xplan:referenzURL", stringAttributWert );
    m_pPlanObjekte->addFeature( pPlanObjNew );

    pPlanObjekt->addChild ( pPlanObjNew->getGmlId() );
    pPlanObjNew->setParentId( pPlanObjekt->getGmlId() );

    pRel = new FeatureRelation;
    pRel->rollenName = "xplan:refBeschreibung";
    pRel->featureId = gmlId;
    pRel->relToFeature = false;
    pPlanObjekt->addFeatureRelation ( pRel );
  }

  success = pPlanObjekt->getURLAttributWert( "xplan:begruendungURL", stringAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteURLAttribut( "xplan:begruendungURL" );

    gmlId = Constant::createGmlId();
    pPlanObjNew = new Feature ( ANY_OBJEKT, gmlId, m_pPlanObjekte, "xplan:XP_ExterneReferenz" );
    pPlanObjNew->setURLAttribut ( "xplan:referenzURL", stringAttributWert );
    m_pPlanObjekte->addFeature( pPlanObjNew );

    pPlanObjekt->addChild ( pPlanObjNew->getGmlId() );
    pPlanObjNew->setParentId( pPlanObjekt->getGmlId() );

    pRel = new FeatureRelation;
    pRel->rollenName = "xplan:refBegruendung";
    pRel->featureId = gmlId;
    pRel->relToFeature = false;
    pPlanObjekt->addFeatureRelation ( pRel );
  }

  DateAttr * pTime = pPlanObjekt->getDateAttributWert( "xplan:auslegungsDatum" );
  if ( pTime != NULL )
  {
    DateAttr * pTimeCopy = new DateAttr;
    pTimeCopy->value =  new TimePosition ( (TimePosition*)(pTime->value) );
    pPlanObjekt->setDateAttribut( "xplan:auslegungsStartDatum",pTimeCopy );
    pPlanObjekt->deleteDateAttribut( "xplan:auslegungsDatum" );
    m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "XP_PPO", "externesSymbolURL",INFORMATION, 
        "", "", 
      "Attribut wird in XPlanGML 4.0 nicht unterstützt",
      "Attribut \"auslegungsDatum\" wird auf \"auslegungsStartDatum\" abgebildet" );
  }

  pTime = pPlanObjekt->getDateAttributWert( "xplan:traegerbeteiligungsDatum" );
  if ( success )
  {
    DateAttr * pTimeCopy = new DateAttr;
    pTimeCopy->value =  new TimePosition ( (TimePosition*)(pTime->value) );
    pPlanObjekt->setDateAttribut( "xplan:traegerbeteiligungsStartDatum",pTimeCopy );
    pPlanObjekt->deleteDateAttribut( "xplan:traegerbeteiligungsDatum" );
    m_pMigDateiProt->createMigrationFehler ( pPlanObjekt->getGmlId(), "XP_PPO", "externesSymbolURL",INFORMATION, 
        "", "", 
      "Attribut wird in XPlanGML 4.0 nicht unterstützt",
      "Attribut \"traegerbeteiligungsDatum\" wird auf \"traegerbeteiligungsStartDatum\" abgebildet" );
  }
  
  m_transObjAnz++;
}

///////////////////////////////////////////////////////////////////////////////
//  class MigrationProtokoll                                                 //
//  Protokoll der Migration von XPlanGML Dateien		    										 // 
///////////////////////////////////////////////////////////////////////////////
MigrationProtokoll::MigrationProtokoll ()
{

}

MigrationProtokoll:: ~MigrationProtokoll()
{
  for ( unsigned int i = 0; i < vDateiProtokolle.size(); i++  )
    delete vDateiProtokolle[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt ein Datei-Protokoll hinzu																					 //
///////////////////////////////////////////////////////////////////////////////
void MigrationProtokoll::addDateiProtokoll ( MigrationDateiProtokoll * pMigDateiProt )
{
  vDateiProtokolle.push_back( pMigDateiProt );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert ein Datei-Protokoll																							 //
///////////////////////////////////////////////////////////////////////////////
MigrationDateiProtokoll * MigrationProtokoll::getDateiProtokoll ( unsigned int index )
{
  if ( index < 0 || index >= vDateiProtokolle.size() )
    return NULL;
  else
    return vDateiProtokolle[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl aller Migrationsfehler																 //
///////////////////////////////////////////////////////////////////////////////
size_t MigrationProtokoll::getMigrationFehlerAnz()
{
	size_t i;
	size_t fehlerAnz = 0;

  for ( i = 0; i < vDateiProtokolle.size(); i++ )
    fehlerAnz = fehlerAnz + vDateiProtokolle[i]->getMigrationFehlerAnz();

  return fehlerAnz;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Migrationsfehler mit einer best. Klassifikation   //
///////////////////////////////////////////////////////////////////////////////
size_t MigrationProtokoll::getMigrationFehlerAnz( FEHLER_KLASSIFIKATION fehlerKlassifikation )
{
	size_t i;
	size_t fehlerAnz = 0;

  for ( i = 0; i < vDateiProtokolle.size(); i++ )
    fehlerAnz = fehlerAnz + vDateiProtokolle[i]->getMigrationFehlerAnz( fehlerKlassifikation );

  return fehlerAnz;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der bei der Migration behandelten XPlanGML Objekte    //
///////////////////////////////////////////////////////////////////////////////
size_t MigrationProtokoll::getAnzahlObjekte()
{
	size_t anzahlObjekte = 0;
	size_t i;

  for ( i = 0; i < vDateiProtokolle.size(); i++ )
    anzahlObjekte = anzahlObjekte + vDateiProtokolle[i]->getAnzahlObjekte();

  return anzahlObjekte;
}

///////////////////////////////////////////////////////////////////////////////
//  class MigrationDateiProtokoll                                            //
//  Protokoll der Migration einer einzelnen XPlanGML Datei									 //
///////////////////////////////////////////////////////////////////////////////
MigrationDateiProtokoll::MigrationDateiProtokoll ()
{
  anzahlObjekte               = 0;
  anzahlObjekteTransformiert  = 0;
}

MigrationDateiProtokoll:: ~MigrationDateiProtokoll()
{
  unsigned int i;

  for ( i = 0; i < vMigrationFehler.size(); i++ )
    delete vMigrationFehler[i];
}

///////////////////////////////////////////////////////////////////////////////
//  Fügt einen Migrationsfehler zum Protokoll hinzu                          //
///////////////////////////////////////////////////////////////////////////////
void MigrationDateiProtokoll::addMigrationFehler ( MigrationFehler * pFehler )
{
  vMigrationFehler.push_back( pFehler );
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert einen Migrationsfehler									                         //
///////////////////////////////////////////////////////////////////////////////
MigrationFehler * MigrationDateiProtokoll::getMigrationFehler ( unsigned int index )
{
  if ( index < 0 || index >= vMigrationFehler.size() )
    return NULL;

  return vMigrationFehler[index];
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl aller Migrationsfehler				                         //
///////////////////////////////////////////////////////////////////////////////
size_t MigrationDateiProtokoll::getMigrationFehlerAnz()
{
  return vMigrationFehler.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Anzahl der Migrationsfehler mit einer best. Klassifikation   //
///////////////////////////////////////////////////////////////////////////////
size_t MigrationDateiProtokoll::getMigrationFehlerAnz ( FEHLER_KLASSIFIKATION fehlerKlassifikation )
{
	size_t anz = 0;

  for ( size_t i = 0; i < vMigrationFehler.size(); i++ )
  {
    MigrationFehler * pFehler = vMigrationFehler[i];
    if ( pFehler->fehlerKlassifikation == fehlerKlassifikation )
      anz++;
  }
  return anz;
}


///////////////////////////////////////////////////////////////////////////////
//  Generiert ein MigrationFehler Objekt                                     //
///////////////////////////////////////////////////////////////////////////////
MigrationFehler * MigrationDateiProtokoll::createMigrationFehler ( string gmlId, string klassenNameQuelle, string attributNameQuelle,
                        FEHLER_KLASSIFIKATION klassifikation, string klassenNameZiel, string attributNameZiel,
                        string fehlerText, string aktionText )
{
  MigrationFehler * pFehler = new MigrationFehler;

  pFehler->objectId             = gmlId;
  pFehler->klassenNameQuelle    = klassenNameQuelle;
  pFehler->attributNameQuelle   = attributNameQuelle;
  pFehler->klassenNameQuelle    = klassenNameZiel;
  pFehler->attributNameZiel     = attributNameZiel;
  pFehler->fehlerKlassifikation = klassifikation;
  pFehler->fehlerBeschreibung   = fehlerText;
  pFehler->aktionBeschreibung   = aktionText;

  addMigrationFehler ( pFehler );
  return pFehler;
}

///////////////////////////////////////////////////////////////////////////////
//  class MigrationFehler                                                    //
//  Beschreibung von Fehlern bei der Migration															 //
///////////////////////////////////////////////////////////////////////////////
MigrationFehler::MigrationFehler()
{
  fehlerKlassifikation = INFORMATION;
}
MigrationFehler:: ~MigrationFehler()
{

}

///////////////////////////////////////////////////////////////////////////////
//  class MigrationProtokollWriter                                           //
//  Export der Migrationsfehler in einem XML-Format													 //
///////////////////////////////////////////////////////////////////////////////
MigrationProtokollWriter::MigrationProtokollWriter( MigrationProtokoll * pProtP )
{
  pProt     = pProtP;
  anzFehler = 0;
}

MigrationProtokollWriter::~MigrationProtokollWriter()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugung des Migrations-Protokolls im XML-Format												 //
///////////////////////////////////////////////////////////////////////////////
int MigrationProtokollWriter::write ( std::string fileNameP, string schemaQuelle, string schemaZiel )
{
  String                    ^ fileName;
  String                    ^ date;
  String                    ^ comment;
  string                      stdString;
  int                         i;
  char                        buffer[16];
  MigrationDateiProtokoll * pDateiProt;

  _strdate_s ( buffer, 16 );
  date = QuConvert::ToString ( string ( buffer ) );
  anzFehler = 0;

  fileName = QuConvert::ToString ( fileNameP );

  System::Text::UTF8Encoding ^ utf8	= gcnew UTF8Encoding();
  writer								            = gcnew XmlTextWriter( fileName, utf8);

  writer->Formatting = Formatting::Indented;
  writer->WriteStartDocument( true );

  comment = "Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: ";
  comment = String::Concat ( comment, date );

  writer->WriteComment( comment );
  writer->WriteStartElement("MigrationProtokoll");
  writer->WriteAttributeString( "schemaQuelle", QuConvert::ToString( schemaQuelle ) );
  writer->WriteAttributeString( "schemaZiel", QuConvert::ToString( schemaZiel ) );    

  stdString = pProt->getDatum();
  if ( stdString != "" )
    writer->WriteAttributeString( "migrationsDatum", QuConvert::ToString( stdString ) );

  for ( i = 0; i < pProt->getDateiProtokollAnz(); i++ )
  {
    pDateiProt = pProt->getDateiProtokoll( i );
    writeDateiProtokoll( pDateiProt );
  }

  writer->WriteEndElement();
  writer->Flush();
  writer->Close();

  return anzFehler;
}

///////////////////////////////////////////////////////////////////////////////
//  Erzeugung des Migrations-Protokolls für eine XPlanGML-Datei							 //
///////////////////////////////////////////////////////////////////////////////
void MigrationProtokollWriter::writeDateiProtokoll ( MigrationDateiProtokoll * pDateiProt )
{
  string              stdString;
  int                 i, anz;
  MigrationFehler * pFehler;

  writer->WriteStartElement( "MigrationDateiProtokoll" );

  stdString = pDateiProt->getDateiName();
  if ( stdString != "" )
    writer->WriteAttributeString( "dateiName", QuConvert::ToString( stdString ) );

  anz = pDateiProt->getAnzahlObjekte();
  writer->WriteAttributeString( "anzObjGelesen", XmlConvert::ToString( anz ) );

  anz = pDateiProt->getAnzahlObjekteTransformiert();
  writer->WriteAttributeString( "anzObjTransformiert", XmlConvert::ToString( anz ) );

  for ( i = 0; i < pDateiProt->getMigrationFehlerAnz(); i++ )
  {
    pFehler = pDateiProt->getMigrationFehler( i );
    writeFehler( pFehler );
  }

  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  Schreiben eines Migratins-Fehlers																			   //
///////////////////////////////////////////////////////////////////////////////
void MigrationProtokollWriter::writeFehler ( MigrationFehler * pFehler )
{
  string   stdString;

  writer->WriteStartElement( "MigrationFehler" );

  if ( pFehler->objectId != "" )
    writer->WriteAttributeString( "objectId", QuConvert::ToString( pFehler->objectId ) );

  if ( pFehler->klassenNameQuelle != "" )
    writer->WriteAttributeString( "klassenNameQuelle", QuConvert::ToString( pFehler->klassenNameQuelle ) );

  if ( pFehler->attributNameQuelle != "" )
    writer->WriteAttributeString( "attributNameQuelle", QuConvert::ToString( pFehler->attributNameQuelle ) );

  if ( pFehler->klassenNameZiel != "" )
    writer->WriteAttributeString( "klassenNameZiel", QuConvert::ToString( pFehler->klassenNameZiel ) );

  if ( pFehler->attributNameZiel != "" )
    writer->WriteAttributeString( "attributNameZiel", QuConvert::ToString( pFehler->attributNameZiel ) );

  switch ( pFehler->fehlerKlassifikation )
  {
  case INFORMATION:
    writer->WriteAttributeString( "fehlerKlassifikation", QuConvert::ToString( "INFORMATION" ) );
    break;

  case WARNUNG:
    writer->WriteAttributeString( "fehlerKlassifikation", QuConvert::ToString( "WARNUNG" ) );
    break;

  case FEHLER:
    writer->WriteAttributeString( "fehlerKlassifikation", QuConvert::ToString( "FEHLER" ) );
    break;

  case SCHWERER_FEHLER:
    writer->WriteAttributeString( "fehlerKlassifikation", QuConvert::ToString( "SCHWERER_FEHLER" ) );
    break;
  }

  if ( pFehler->fehlerBeschreibung != "" )
    writer->WriteAttributeString( "fehlerBeschreibung", QuConvert::ToString( pFehler->fehlerBeschreibung ) );

  if ( pFehler->aktionBeschreibung != "" )
    writer->WriteAttributeString( "aktionBeschreibung", QuConvert::ToString( pFehler->aktionBeschreibung ) );

  anzFehler++;

  writer->WriteEndElement();
}

///////////////////////////////////////////////////////////////////////////////
//  class MigrationV5                                                        //
// Transformiert XPlanGML 4.1 in Version 5.0, 5.0 oder 5.2, sowie von        //
// Version 5.0 nach Version 5.1 oder 5.2                                     //
///////////////////////////////////////////////////////////////////////////////
MigrationV5::MigrationV5 ( Features * pPlanObjekteP, UOMList * pUOMList, MigrationDateiProtokoll * pMigDateiProtP,
                 std::string versionXPlanGMLZiel )
{
  m_pPlanObjekte         = pPlanObjekteP;
  m_pUOMList             = pUOMList;
  m_pMigDateiProt        = pMigDateiProtP;
  m_transObjAnz          = 0;
  m_versionXPlanGMLZiel  = versionXPlanGMLZiel;
}

MigrationV5::~ MigrationV5()
{

}

///////////////////////////////////////////////////////////////////////////////
//  Migration eines gesamten Plans                                           //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::transformation ()
{
	size_t        i, j, planAnz, planBereichAnz, planObjektAnz;
  BLeitPlan   * pPlan;
  PlanBereich * pPlanBereich;
  Feature     * pPlanObjekt;
  GmlSchema   * pGmlSchema = m_pPlanObjekte->getGmlSchema();

  vector<BLeitPlan*>   vPlaene;
  vector<PlanBereich*> vPlanBereiche;
  vector<Feature*>     vPlanObjekte;

  if ( m_pPlanObjekte->getGmlSchema()->getGmlTyp() == XPLANGML_5_0 )
  {
    transformation_V5_0();
    return;
  }

  planAnz = m_pPlanObjekte->getPlaene( vPlaene );

  for ( i = 0; i < planAnz; i++ )
  {
    pPlan = vPlaene[i];
    transformationPlan ( dynamic_cast<BLeitPlan*>(pPlan) );

    vPlanBereiche.clear();
    planBereichAnz = pPlan->getPlanBereiche ( vPlanBereiche );
    for ( j = 0; j < planBereichAnz; j++ )
    {
      pPlanBereich = vPlanBereiche[j];
      transformationPlanBereich ( dynamic_cast<PlanBereich*>(pPlanBereich) );
    }
  }

  planObjektAnz = m_pPlanObjekte->getAllFeatures( vPlanObjekte );
  for ( i = 0; i < planObjektAnz; i++ )
  {
    pPlanObjekt = vPlanObjekte[i];

    switch ( pPlanObjekt->getObjektTyp() )
    {
    case BLEIT_PLAN:
    case PLAN_BEREICH:
    case RASTER_BASIS:
     continue;

    case ANY_OBJEKT:
    case XP_GEOMETRIE_OBJEKT:
    case XP_PUNKT_OBJEKT:
    case XP_LINIEN_OBJEKT:
    case XP_FLAECHEN_OBJEKT:
      transformationObjekt ( pPlanObjekt );
      break;
    }
  } 

  m_pMigDateiProt->setAnzahlObjekteTransformiert( m_transObjAnz );
}

///////////////////////////////////////////////////////////////////////////////
//  Migration von XPlanGML 5.0 --> 5.1 oder 5.2                              //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::transformation_V5_0 ()
{
	size_t               i, j, planAnz, planBereichAnz, rasterAnz;
  BLeitPlan          * pPlan;
  PlanBereich        * pPlanBereich;
  vector<BLeitPlan*>   vPlaene;
  vector<PlanBereich*> vPlanBereiche;
  vector<Feature*>     vRasterdarstellungen;
  string               stringAttributWert, stringAttributWertA;
  DateAttr           * pDateAttr;
  DateAttr           * pDateAttrNew;
  bool                 success, successA;

  planAnz = m_pPlanObjekte->getPlaene( vPlaene );

  for ( i = 0; i < planAnz; i++ )
  {
    pPlan = vPlaene[i];

    vPlanBereiche.clear();
    planBereichAnz = pPlan->getPlanBereiche ( vPlanBereiche );
    for ( j= 0; j < planBereichAnz; ++j )
    {
      pPlanBereich = vPlanBereiche[j];

      success = pPlanBereich->getStringAttributWert( "xplan:versionBauGBText", stringAttributWert );
      if ( success )
      {
        successA = pPlan->getStringAttributWert( "xplan:versionBauGBText", stringAttributWertA );
        if ( !successA )
          pPlan->setStringAttribut ( "xplan:versionBauGBText", stringAttributWert );
        pPlanBereich->deleteStringAttribut( "xplan:versionBauGBText" );
      }

      pDateAttr = pPlanBereich->getDateAttributWert( "xplan:versionBauGBDatum" );
      if ( pDateAttr != NULL )
      {
        pDateAttrNew = pPlan->getDateAttributWert( "xplan:versionBauGBDatum" );
        if ( pDateAttrNew == NULL )
        {
          pDateAttrNew = new DateAttr ( pDateAttr );
          pPlan->setDateAttribut ( "xplan:versionBauGBDatum", pDateAttrNew );
        }
        pPlanBereich->deleteDateAttribut( "xplan:versionBauGBDatum" );
      }

      success = pPlanBereich->getStringAttributWert( "xplan:versionBauNVOText", stringAttributWert );
      if ( success )
      {
        successA = pPlan->getStringAttributWert( "xplan:versionBauNVOText", stringAttributWertA );
        if ( !successA )
          pPlan->setStringAttribut ( "xplan:versionBauNVOText", stringAttributWert );
        pPlanBereich->deleteStringAttribut( "xplan:versionBauNVOText" );
      }

      pDateAttr = pPlanBereich->getDateAttributWert( "xplan:versionBauNVODatum" );
      if ( pDateAttr != NULL )
      {
        pDateAttrNew = pPlan->getDateAttributWert( "xplan:versionBauNVODatum" );
        if ( pDateAttrNew == NULL )
        {
          pDateAttrNew = new DateAttr ( pDateAttr );
          pPlan->setDateAttribut ( "xplan:versionBauNVODatum", pDateAttrNew );
        }
        pPlanBereich->deleteDateAttribut( "xplan:versionBauNVODatum" );
      }

      vRasterdarstellungen.clear();
      rasterAnz = pPlanBereich->getRelatedFeatures( "xplan:rasterBasis", vRasterdarstellungen );
      if ( rasterAnz > 0 )
        transformRasterBasis( pPlan, pPlanBereich, vRasterdarstellungen[0] );

    }   
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Migration der Planklasse                                                 //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::transformationPlan ( BLeitPlan * pPlan )
{
	size_t            i, anz;
  bool              success;
  string            attributName;
  string            stringAttributWert;
  Feature         * pPlanObjekt;
  StringAttr      * pStringAttr;   

  vector<FeatureRelation*> vObjRelationen;

  //  CR-006
  pPlan->deleteStringAttribut ( "xplan:xplanGMLVersion" );

  //  CR-016
  anz = pPlan->getGeometryPropertyAnz( "xplan:raeumlicherGeltungsbereich" );
  if ( anz == 0 )
  {
    m_pMigDateiProt->createMigrationFehler( pPlan->getGmlId(), pPlan->getKlassenName(), "xplan:raeumlicherGeltungsbereich", SCHWERER_FEHLER,
                   pPlan->getKlassenName(), "xplan:raeumlicherGeltungsbereich", "Der Geltungsbereich des Plans ist Pflichtattribut", "Keine Korrektur möglich" );
  }

  if ( pPlan->getKlassenName() == "xplan:SO_Plan ")
  {
    success = pPlan->getStringAttributWert( "xplan:planTyp", stringAttributWert );
    if ( success )
    {
      pPlan->deleteStringAttribut( "xplan:planTyp" );
      pPlan->setStringAttribut( "xplan:planArt", stringAttributWert );
    }
  }

  anz = pPlan->getStringAttributWertAnz( "xplan:planArt" );
  if ( anz == 0 )
  {
    m_pMigDateiProt->createMigrationFehler( pPlan->getGmlId(), pPlan->getKlassenName(), "xplan:planArt", FEHLER,
      pPlan->getKlassenName(), "xplan:planArt", "Der Plantyp ist Pflichtattribut", "Die Plan-Art wirk auf 9999 (Sonstiger Plan) gesetzt" );
    pPlan->setStringAttribut( "xplan:planArt", "9999" );
  }

  anz = pPlan->getStringAttributWertAnz ( "xplan:name" );
  if (anz == 0)
  {
    m_pMigDateiProt->createMigrationFehler( pPlan->getGmlId(), pPlan->getKlassenName(), "xplan:name", FEHLER,
      pPlan->getKlassenName(), "xplan:planArt", "Der Plan-Name ist Pflichtattribut", "Der Plan-Name wirk auf \"Unbekannt\" gesetzt" );
    pPlan->setStringAttribut( "xplan:name", "Unbekannt" );
  }

  // CR-035
  anz = pPlan->getFeatureRelationen( vObjRelationen );
  for ( i = 0; i < anz; i++ )
  {
    FeatureRelation * pRel = vObjRelationen[i];
    if ( pRel->rollenName == "xplan:rechtsverbindlich" || 
         pRel->rollenName == "xplan:informell" || 
         pRel->rollenName == "xplan:refBeschreibung" || 
         pRel->rollenName == "xplan:refBegruendung" || 
         pRel->rollenName == "xplan:refLegende" || 
         pRel->rollenName == "xplan:refRechtsplan" || 
         pRel->rollenName == "xplan:refPlangrundlage" || 
         pRel->rollenName == "xplan:refKoordinatenliste" || 
         pRel->rollenName == "xplan:refGrundstuecksverzeichnis" || 
         pRel->rollenName == "xplan:refPflanzliste" || 
         pRel->rollenName == "xplan:refUmweltbericht" || 
         pRel->rollenName == "xplan:refSatzung" || 
         pRel->rollenName == "xplan:refGruenordnungsplan" || 
         pRel->rollenName == "xplan:refErlaeuterung" )
    {
      pPlanObjekt = pPlan->getRelatedFeature( pRel );
      pPlan->removeFeatureRelation( pPlanObjekt );

      if ( pPlanObjekt != NULL )
      {
        // CR-059
        pStringAttr = pPlanObjekt->getStringAttributWert( "xplan:art" );
        if ( pStringAttr != NULL )
          pStringAttr->classifier.erase( "codeSpace" );

        pPlanObjekt->setKlassenName( "xplan:XP_SpezExterneReferenz" );
        if ( pRel->rollenName == "xplan:rechtsverbindlich" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "9998" );
        else
        if ( pRel->rollenName == "xplan:informell" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "9999" );
        else
        if ( pRel->rollenName == "xplan:refBeschreibung" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "1000" );
        else
        if ( pRel->rollenName == "xplan:refBegruendung" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "1010" );
        else
        if ( pRel->rollenName == "xplan:refLegende" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "1020" );
        else
        if ( pRel->rollenName == "xplan:refRechtsplan" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "1030" );
        else
        if ( pRel->rollenName == "xplan:refPlangrundlage" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "1040" );
        else
        if (  pRel->rollenName == "xplan:refKoordinatenliste" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "2000" );
        else
        if ( pRel->rollenName == "xplan:refGrundstuecksverzeichnis" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "2100" );
        else
        if (  pRel->rollenName == "xplan:refPflanzliste" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "2200" );
        else
        if (  pRel->rollenName == "xplan:refUmweltbericht" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "1050" );
        else
        if (  pRel->rollenName == "xplan:refSatzung" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "1060" );
        else
        if ( pRel->rollenName == "xplan:refGruenordnungsplan" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "2300" );
        else
        if ( pRel->rollenName == "xplan:refErlaeuterung" )
          pPlanObjekt->setStringAttribut ( "xplan:typ", "1080" );
      }

      pRel->rollenName = "xplan:externeReferenz";
      pPlan->addFeatureRelation( pRel );
    }
    else
    if ( pRel->rollenName == "xplan:refExternalCodeList" )
    {
      pPlanObjekt = pPlan->getRelatedFeature( pRel );
      if ( pPlanObjekt != NULL )
        m_pPlanObjekte->removeFeature( pPlanObjekt->getGmlId() );
      pPlan->deleteFeatureRelationen ( pRel->rollenName );

      m_pMigDateiProt->createMigrationFehler( pPlan->getGmlId(), pPlan->getKlassenName(), "xplan:refExternalCodeList", FEHLER,
        "", "", "Die externe Referenz auf eine Codeliste existiert nicht mehr", "Die Referenz wird gelöscht" );    
    }
    else
    // CR-058
    if ( pRel->rollenName == "xplan:texte" )
    {
      pPlanObjekt = pPlan->getRelatedFeature( pRel );
      if ( pPlanObjekt != NULL && pPlanObjekt->getKlassenName() == "xplan:XP_TextAbschnitt" )
      {
        if ( pPlan->getKlassenName() == "xplan:BP_Plan" )
        {
          pPlanObjekt->setKlassenName( "xplan:BP_TextAbschnitt" );
          pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "1000" );
        }
        else
        if ( pPlan->getKlassenName() == "xplan:FP_Plan" )
        {
          pPlanObjekt->setKlassenName( "xplan:FP_TextAbschnitt" );
          pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "1000" );
        }
        else
        if ( pPlan->getKlassenName() == "xplan:SO_Plan" )
        {
          pPlanObjekt->setKlassenName( "xplan:SO_TextAbschnitt" );
          pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "9999" );
        }
      }

    }
  }

  // CR-008
  if ( m_versionXPlanGMLZiel == "5.1" ||   m_versionXPlanGMLZiel == "5.2" )
  {
		size_t               planBereichAnz;
    vector<PlanBereich*> vPlanBereiche;
    PlanBereich        * pPlanBereich;
    DateAttr           * pDateAttr;

    planBereichAnz = pPlan->getPlanBereiche ( vPlanBereiche );
    if ( planBereichAnz == 0 )
      return;

    pPlanBereich = vPlanBereiche[0];

    success = pPlanBereich->getStringAttributWert ( "xplan:versionBauNVO", stringAttributWert );
    if ( success )
    {
      pPlanBereich->deleteStringAttribut ( "xplan:versionBauNVO" );
      if ( stringAttributWert == "1000" )
      {
        pDateAttr = new DateAttr;

        pDateAttr->value = new TimePosition ( 1962, 8, 1 );
        pPlanObjekt->setDateAttribut( "xplan:versionBauNVODatum", pDateAttr );
      }
      else
      if ( stringAttributWert == "2000" )
      {
        pDateAttr = new DateAttr;
        pDateAttr->value = new TimePosition ( 1968, 11, 26 );
        pPlan->setDateAttribut( "xplan:versionBauNVODatum", pDateAttr );
      }
      else
      if ( stringAttributWert == "3000" )
      {
        pDateAttr = new DateAttr;
        pDateAttr->value = new TimePosition ( 1977, 9, 15 );
        pPlanObjekt->setDateAttribut( "xplan:versionBauNVODatum", pDateAttr );
      }
      else
      if ( stringAttributWert == "4000" )
      {
        pDateAttr = new DateAttr;
        pDateAttr->value = new TimePosition ( 1990, 1, 23 );
        pPlanObjekt->setDateAttribut( "xplan:versionBauNVODatum", pDateAttr );
      }
      else
      if ( stringAttributWert == "9999" )
      {
        success = pPlanBereich->getStringAttributWert( "xplan:versionBauNVOText", stringAttributWert );
        if ( success )
        {
          pPlanBereich->deleteStringAttribut( "xplan:versionBauNVOText" );
          pPlanObjekt->setStringAttribut( "xplan:versionSonstRechtsgrundlageText", stringAttributWert );
        }
        else
          m_pMigDateiProt->createMigrationFehler( pPlanBereich->getGmlId(), pPlanBereich->getKlassenName(), "xplan:versionBauNVO", FEHLER,
          pPlanBereich->getKlassenName(), "xplan:versionBauNVODatum", 
          "Attributwert 'Andere gesetzliche Bestimmung' kann nicht übernommen werden", "Das Attribut wird ignoriert" );
      }

      success = pPlanBereich->getStringAttributWert( "xplan:versionBauNVOText", stringAttributWert );
      if ( success )
      {
        pPlanBereich->deleteStringAttribut( "xplan:versionBauNVOText" );
        pPlan->setStringAttribut( "xplan:versionBauNVOText", stringAttributWert );
      }

      pDateAttr = pPlanBereich->getDateAttributWert( "xplan:versionBauGB" );
      if ( pDateAttr != NULL )
      {
        DateAttr * pDateAttrCopy = new DateAttr ( pDateAttr );
        pPlanBereich->deleteDateAttribut( "xplan:versionBauGB" );
        pPlan->setDateAttribut ( "xplan:versionBauGBDatum", pDateAttrCopy );
      }

      success = pPlanBereich->getStringAttributWert( "xplan:versionBauGBText", stringAttributWert );
      if ( success )
      {
        pPlanBereich->deleteStringAttribut( "xplan:versionBauGBText" );
        pPlan->setStringAttribut( "xplan:versionBauGBText", stringAttributWert );
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Migration der Bereichsklasse                                             //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::transformationPlanBereich ( PlanBereich * pPlanBereich )
{
	size_t     i, anz;
  bool       success;
  string     klassenName;
  string     modulName;
  string     attributName;
  string     stringAttributWert;
  string     rechtscharakter;
  Feature  * pPlanObjekt;
  DateAttr * pDateAttr;

  vector<FeatureRelation*> vObjRelationen;

  // CR-004
  success = pPlanBereich->getStringAttributWert ( "xplan:erstellungsMasstab", stringAttributWert );
  if ( success )
  {
    pPlanBereich->deleteStringAttribut ( "erstellungsMasstab" );
    pPlanBereich->setStringAttribut( "erstellungsMassstab", stringAttributWert );
  }

  //  CR-008
  if ( m_versionXPlanGMLZiel == "5.0" )
  {
    success = pPlanBereich->getStringAttributWert ( "xplan:versionBauNVO", stringAttributWert );
    if ( success )
    {
      pPlanBereich->deleteStringAttribut ( "xplan:versionBauNVO" );
      if ( stringAttributWert == "1000" )
      {
        pDateAttr = new DateAttr;

        pDateAttr->value = new TimePosition ( 1962, 8, 1 );
        pPlanBereich->setDateAttribut( "xplan:versionBauNVODatum", pDateAttr );
      }
      else
      if ( stringAttributWert == "2000" )
      {
        pDateAttr = new DateAttr;
        pDateAttr->value = new TimePosition ( 1968, 11, 26 );
        pPlanBereich->setDateAttribut( "xplan:versionBauNVODatum", pDateAttr );
      }
      else
      if ( stringAttributWert == "3000" )
      {
        pDateAttr = new DateAttr;
        pDateAttr->value = new TimePosition ( 1977, 9, 15 );
        pPlanBereich->setDateAttribut( "xplan:versionBauNVODatum", pDateAttr );
      }
      else
      if ( stringAttributWert == "4000" )
      {
        pDateAttr = new DateAttr;
        pDateAttr->value = new TimePosition ( 1990, 1, 23 );
        pPlanBereich->setDateAttribut( "xplan:versionBauNVODatum", pDateAttr );
      }
      else
      if ( stringAttributWert == "9999" )
      {
        success = pPlanBereich->getStringAttributWert( "xplan:versionBauNVOText", stringAttributWert );
        if ( success )
        {
          pPlanBereich->deleteStringAttribut( "xplan:versionBauNVOText" );
          pPlanBereich->setStringAttribut( "xplan:versionSonstRechtsgrundlageText", stringAttributWert );
        }
        else
          m_pMigDateiProt->createMigrationFehler( pPlanBereich->getGmlId(), pPlanBereich->getKlassenName(), "xplan:versionBauNVO", FEHLER,
          pPlanBereich->getKlassenName(), "xplan:versionBauNVODatum", 
          "Attributwert 'Andere gesetzliche Bestimmung' kann nicht übernommen werden", "Das Attribut wird ignoriert" );

        pPlanBereich->setStringAttribut ("xplan:versionBauNVOText", "");

        DateAttr * pDateAttr = new DateAttr;
        TimePosition * pDate = new TimePosition (9999, 1, 1);
        pDateAttr->value = pDate;

        pPlanBereich->setDateAttribut ("xplan:versionBauNVODatum", pDateAttr);
      }
    }

    pDateAttr = pPlanBereich->getDateAttributWert( "xplan:versionBauGB" );
    if ( pDateAttr != NULL )
    {
      DateAttr * pDateAttrCopy = new DateAttr ( pDateAttr );
      pPlanBereich->deleteDateAttribut( "xplan:versionBauGB" );
      pPlanBereich->setDateAttribut ( "xplan:versionBauGBDatum", pDateAttrCopy );
    }
  }


  // CR-019
  success = pPlanBereich->getStringAttributWert( "xplan:bedeutung", stringAttributWert );
  if ( success )
  {
    if ( stringAttributWert != "1600" && stringAttributWert != "1800" && stringAttributWert != "9999")
    {
      pPlanBereich->deleteStringAttribut( "xplan:bedeutung" );
      m_pMigDateiProt->createMigrationFehler( pPlanBereich->getGmlId(), pPlanBereich->getKlassenName(), "xplan:bedeutung", FEHLER,
        pPlanBereich->getKlassenName(), "xplan:bedeutung", 
        "Spezifizierter Attributwert "+ stringAttributWert + " kann nicht übernommen werden", "Das Attribut wird ignoriert" );
    }
  }

  // CR-007
  anz = pPlanBereich->getFeatureRelationen( "xplan:nachrichtlich", vObjRelationen );
  for ( i = 0; i < anz; i++ )
  {
    FeatureRelation * pRel = vObjRelationen[i];
    pPlanObjekt = pPlanBereich->getRelatedFeature ( pRel );
    if ( pPlanObjekt != NULL )
    {
      success = pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", rechtscharakter );
      if ( success )        
        pPlanObjekt->deleteStringAttribut ( "xplan:rechtscharakter");
      else
        rechtscharakter = "";

      klassenName = pPlanObjekt->getKlassenName();
      modulName = klassenName.substr( 0, 8 );
      if ( modulName == "xplan:BP" )
        pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "2000" );
      else
      if ( modulName == "xplan:FP" )
        pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "2000" );
      else
      if ( modulName == "xplan:SO" )
      {
        success = pPlanObjekt->getStringAttributWert( "xplan:sonstRechtscharakter", stringAttributWert );
        if ( success )
        {
          pPlanObjekt->deleteStringAttribut ( "xplan:sonstRechtscharakter" );

          if ( stringAttributWert == "FestsetzungBPlan" )
            pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "1000" );
          else
          if ( stringAttributWert == "DarstellungFPlan" )
            pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "1500" );
        }
        else
        if ( rechtscharakter != "" )
          pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", rechtscharakter );
        else
          pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "2000" );
      }

      pPlanBereich->removeFeatureRelation( pPlanObjekt );
      pRel->rollenName = "xplan:planinhalt";
      pPlanBereich->addFeatureRelation( pRel );
    }
  }

  vObjRelationen.clear();
  anz = pPlanBereich->getFeatureRelationen( "xplan:inhaltBPlan", vObjRelationen );
  for ( i = 0; i < anz; i++ )
  {
    FeatureRelation * pRel = vObjRelationen[i];

    pPlanBereich->removeFeatureRelation( pRel );
    pRel->rollenName = "xplan:planinhalt";
    pPlanBereich->addFeatureRelation( pRel );
  }

  vObjRelationen.clear();
  anz = pPlanBereich->getFeatureRelationen( "xplan:inhaltFPlan", vObjRelationen );
  for ( i = 0; i < anz; i++ )
  {
    FeatureRelation * pRel = vObjRelationen[i];

    pPlanBereich->removeFeatureRelation( pRel );
    pRel->rollenName = "xplan:planinhalt";
    pPlanBereich->addFeatureRelation( pRel );
  }

  vObjRelationen.clear();
  anz = pPlanBereich->getFeatureRelationen( "xplan:inhaltSOPlan", vObjRelationen );
  for ( i = 0; i < anz; i++ )
  {
    FeatureRelation * pRel = vObjRelationen[i];

    pPlanBereich->removeFeatureRelation( pRel );
    pRel->rollenName = "xplan:planinhalt";
    pPlanBereich->addFeatureRelation( pRel );
  }

  //  Umbenennung XP_RasterplanBsis --> XP_Rasterdarstellung  
  vObjRelationen.clear();
  anz = pPlanBereich->getFeatureRelationen( "xplan:rasterBasis", vObjRelationen );
  if ( anz > 0 )
  {
    FeatureRelation * pRel = vObjRelationen[0];
    Feature * pRasterplanBasis = m_pPlanObjekte->getFeature( pRel->featureId );

    if (  m_versionXPlanGMLZiel == "5.0" )
    {
      if ( pRasterplanBasis->getKlassenName() == "xplan:XP_RasterplanBasis" )
        pRasterplanBasis->setKlassenName( "xplan:XP_Rasterdarstellung" );
    }
    else
    if (  m_versionXPlanGMLZiel == "5.1" || m_versionXPlanGMLZiel == "5.2" )
    {
      transformRasterBasis ( pPlanBereich->getBPlan(), pPlanBereich, pRasterplanBasis );
    }
  }

  // CR-055
  vObjRelationen.clear();
  anz = pPlanBereich->getFeatureRelationen( "xplan:rasterAenderung", vObjRelationen );
  if ( anz > 0 )
  {
    m_pMigDateiProt->createMigrationFehler( pPlanBereich->getGmlId(), pPlanBereich->getKlassenName(), "xplan:rasterAenderung", SCHWERER_FEHLER,
      pPlanBereich->getKlassenName(), "", 
      "Rasterplan_Aenderung wird nicht mehr unterstützt", "Das Objekt wird nicht übernommen" );

    for ( i = 0; i < anz; i++ )
    {
      FeatureRelation * pRel = vObjRelationen[i];
      m_pPlanObjekte->removeFeature( pRel->featureId );   
      pPlanBereich->removeFeatureRelation( pRel );
    }
  }

  for ( i = 0; i < anz; i++ )
  {
    FeatureRelation * pRel = vObjRelationen[i];

    pPlanObjekt = pPlanBereich->getRelatedFeature ( pRel );
    if ( pPlanObjekt != NULL )
    {
      vector<FeatureRelation*> vRel;

			size_t anzLoc = pPlanObjekt->getFeatureRelationen( vRel );
      for ( size_t j = 0; j < anzLoc; j++ )
      {
        FeatureRelation * pRelLoc = vRel[j];
        Feature * pFeatureLoc = pPlanObjekt->getRelatedFeature ( pRelLoc );
        if ( pFeatureLoc != NULL ) 
          m_pPlanObjekte->removeFeature( pFeatureLoc->getGmlId() );
      }

      pPlanBereich->deleteFeatureRelation( pPlanObjekt );
      m_pPlanObjekte->removeFeature( pPlanObjekt->getGmlId() );
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Migration eines Planobjektes                                             //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::transformationObjekt ( Feature * pPlanObjekt )
{
  string klassenName;
  string modulName;

  klassenName = pPlanObjekt->getKlassenName();
  modulName = klassenName.substr ( 0, 8 );

  if ( modulName == "xplan:BP" )
    transformationBPlanObjekt( pPlanObjekt );
  else
  if ( modulName == "xplan:FP" )
    transformationFPlanObjekt( pPlanObjekt );
  else
  if ( modulName == "xplan:SO" )
    transformationSOPlanObjekt( pPlanObjekt );
}

///////////////////////////////////////////////////////////////////////////////
//  Migration eines BPlan-Objektes                                           //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::transformationBPlanObjekt ( Feature * pPlanObjekt )
{
  string klassenName = pPlanObjekt->getKlassenName();
  string stringAttributWert;
  bool   success;

  // CR-016
  success = pPlanObjekt->getStringAttributWert( "xplan:rechtscharakter", stringAttributWert );
  if ( !success || stringAttributWert == "" )
  {
    m_pMigDateiProt->createMigrationFehler( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "xplan:rechtscharakter", WARNUNG,
      pPlanObjekt->getKlassenName(),  "xplan:rechtscharakter",  "Das Attribut 'rechtscharakter' ist ein Pflichtattribut", "Der Rechtscharakter wird auf 9998 (Unbekannt) gesetzt" );
    pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
    pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "9998" );
  }

  // CR-007
  correctBereichRelation ( pPlanObjekt, "xplan:gehoertZuBP_Bereich" );

  //  CR-058
  correctTextAbschnitt ( pPlanObjekt, "xplan:BP_TextAbschnitt" );

  // CR-009, CR-031
  if ( klassenName == "xplan:BP_GemeinschaftsanlagenFlaeche" ||
       klassenName == "xplan:BP_NebenanlagenFlaeche"         ||
       klassenName == "xplan:BP_GemeinbedarfsFlaeche"        ||
       klassenName == "xplan:BP_SpielSportanlagenFlaeche"    ||
       klassenName == "xplan:BP_WaldFlaeche"                 ||
       klassenName == "xplan:BP_GruenFlaeche"                ||
       klassenName == "xplan:BP_Landwirtschaft"              ||
       klassenName == "xplan:BP_GenerischesObjekt"           ||
       klassenName == "xplan:BP_VerEntsorgung"               )
       correctZweckbestimmungen ( pPlanObjekt );

  //  CR-009
  if ( klassenName == "xplan:BP_SchutzPflegeEntwicklungsMassnahme" ||
       klassenName == "xplan:BP_AusgleichsMassnahme"               ||
       klassenName == "xplan:BP_SchutzPflegeEntwicklungsFlaeche"   ||
       klassenName == "xplan:BP_AusgleichFlaeche"                  )
       correctMassnahmen ( pPlanObjekt );

  // CR-053
  success = pPlanObjekt->getStringAttributWert( "xplan:textSchluessel", stringAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteStringAttribut( "xplan:textSchluessel" );
    createTextObjekt ( pPlanObjekt, "xplan:BP_TextAbschnitt", stringAttributWert );
  }

  // CR-053
  success = pPlanObjekt->getStringAttributWert( "xplan:textSchluesselBegruendung", stringAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteStringAttribut( "xplan:textSchluesselBegruendung" );
    createBegruendungObjekt ( pPlanObjekt, stringAttributWert );
  } 

  // CR-057
  if ( klassenName == "xplan:FP_VerEntsorgung" )
    correctVerEntsorgungCodes ( pPlanObjekt );

  // CR-069
  if ( klassenName == "xplan:BaugebietsTeilFlaeche"           ||
       klassenName == "xplan:UeberbaubareGrundstuecksFlaeche" ||
       klassenName == "xplan:GemeinbedarfsFlaeche"            ||
       klassenName == "xplan:SpielSportanlagenFlaeche"        ||
       klassenName == "xplan:GruenFlaeche"                    ||
       klassenName == "xplan:VErEntsorgung"       )
       checkMasszahlen ( pPlanObjekt );

  // CR-081
  if ( klassenName =="xplan:BP_DenkmalschutzEnsembleFlaeche" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_Denkmalschutzrecht" );
    pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1000" );

    success = pPlanObjekt->getStringAttributWert( "xplan:denkmal", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->setStringAttribut( "xplan:name", stringAttributWert );
      pPlanObjekt->deleteStringAttribut( "xplan:denkmal" );
    }
  }

  if ( klassenName =="xplan:BP_DenkmalschutzEinzelanlage" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_Denkmalschutzrecht" );
    pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", "1100" );

    success = pPlanObjekt->getStringAttributWert( "xplan:denkmal", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->setStringAttribut( "xplan:name", stringAttributWert );
      pPlanObjekt->deleteStringAttribut( "xplan:denkmal" );
    }
  }

  if ( klassenName =="xplan:BP_Schutzgebiet" )
  {
    pPlanObjekt->setKlassenName( "xplan:SO_SchutzgebietNaturschutzrecht" );

    success = pPlanObjekt->getStringAttributWert( "xplan:zweckbestimmung", stringAttributWert );
    if ( success )
    {
      pPlanObjekt->setStringAttribut( "xplan:artDerFestlegung", stringAttributWert );
      pPlanObjekt->deleteStringAttribut( "xplan:zweckbestimmung" );
    }
  }

  success = pPlanObjekt->getStringAttributWert( "xplan:detaillierteZweckbestimmung", stringAttributWert );
  if ( success )
  {
    pPlanObjekt->setStringAttribut( "xplan:detailArtDerFestlegung", stringAttributWert );
    pPlanObjekt->deleteStringAttribut( "xplan:detaillierteZweckbestimmung" );
  }

  // CR-038
  if ( klassenName == "xplan:BP_LuftreinhalteFlaeche" )
  {
    pPlanObjekt->setKlassenName( "xplan:BP_TechnischeMassnahmenFlaeche" );
    pPlanObjekt->setStringAttribut( "xplan:zweckbestimmung", "1000" );
  }

  if ( klassenName == "xplan:BP_ErneuerbareEnergieFlaeche" )
  {
    pPlanObjekt->setKlassenName( "xplan:BP_TechnischeMassnahmenFlaeche" );
    pPlanObjekt->setStringAttribut( "xplan:zweckbestimmung", "2000" );

    success = pPlanObjekt->getStringAttributWert( "xplan:technischeMaßnahme", stringAttributWert );
    if (success )
    {
      pPlanObjekt->deleteStringAttribut( "xplan:technischeMaßnahme" );
      pPlanObjekt->setStringAttribut( "xplan:technischeMassnahme", stringAttributWert );
    }
  }  

}

///////////////////////////////////////////////////////////////////////////////
//  Migration eines FPlan-Objektes                                           //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::transformationFPlanObjekt ( Feature * pPlanObjekt )
{
  string klassenName = pPlanObjekt->getKlassenName();
  string stringAttributWert;
  bool   success;

  // CR-016
  success = pPlanObjekt->getStringAttributWert( "xplan:rechtscharakter", stringAttributWert );
  if ( !success || stringAttributWert == "" )
  {
    m_pMigDateiProt->createMigrationFehler( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "xplan:rechtscharakter", WARNUNG,
      pPlanObjekt->getKlassenName(),  "xplan:rechtscharakter",  "Das Attribut 'rechtscharakter' ist ein Pflichtattribut", "Der Rechtscharakter wird auf 9998 (Unbekannt) gesetzt" );
    pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
    pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "9998" );
  }

  //  CR-058
  correctTextAbschnitt ( pPlanObjekt, "xplan:FP_TextAbschnitt" );


  // CR-007
  correctBereichRelation ( pPlanObjekt, "xplan:gehoertZuFP_Bereich"  );

  // CR-009, CR-031
  if ( klassenName == "xplan:FP_Gemeinbedarf"           ||
       klassenName == "xplan:FP_SpielSportanlage"       ||
       klassenName == "xplan:FP_WaldFlaeche"            ||
       klassenName == "xplan:FP_Gruen"                  ||
       klassenName == "xplan:FP_LandwirtschaftsFlaeche" ||
       klassenName == "xplan:FP_GenerischesObjekt"      ||
       klassenName == "xplan:FP_PrivilegiertesVorhaben" ||
       klassenName == "xplan:FP_VerEntsorgung"          ||
       klassenName == "xplan:FP_Kennzeichnung"      )
       correctZweckbestimmungen ( pPlanObjekt );

  //  CR-009
  if ( klassenName == "xplan:FP_SchutzPflegeEntwicklung" ||
       klassenName == "xplan:FP_AusgleichsFlaeche" )
       correctMassnahmen ( pPlanObjekt );

  // CR-017
  if ( klassenName == "xplan:Baugebiet" )
  {
    m_pMigDateiProt->createMigrationFehler( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "", SCHWERER_FEHLER,
      "",  "",  "Die Klasse 'Baugebiet' wird nicht mehr unterstützt", "Das Objekt wird nicht übernommen" );

    m_pPlanObjekte->removeFeature( pPlanObjekt->getGmlId() );
  }

  // CR-046
  if ( klassenName == "xplan:FP_AufschuettungsFlaeche" )
    pPlanObjekt->setKlassenName( "FP_Aufschuettung" );

  if ( klassenName == "xplan:FP_AbgrabungsFlaeche" )
    pPlanObjekt->setKlassenName( "FP_Abgrabung" );

  if ( klassenName == "xplan:FP_BodenschaetzeFlaeche" )
    pPlanObjekt->setKlassenName( "FP_Bodenschaetze" );

  // CR-057
  if ( klassenName == "xplan:FP_VerEntsorgung" )
    correctVerEntsorgungCodes ( pPlanObjekt );

  // CR-053
  success = pPlanObjekt->getStringAttributWert( "xplan:textSchluessel", stringAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteStringAttribut( "xplan:textSchluessel" );
    createTextObjekt ( pPlanObjekt, "xplan:FP_TextAbschnitt", stringAttributWert );
  }

  // CR-053
  success = pPlanObjekt->getStringAttributWert( "xplan:textSchluesselBegruendung", stringAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteStringAttribut( "xplan:textSchluesselBegruendung" );
    createBegruendungObjekt ( pPlanObjekt, stringAttributWert );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Migration eines SOPlan-Objektes                                          //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::transformationSOPlanObjekt ( Feature * pPlanObjekt )
{
  string klassenName = pPlanObjekt->getKlassenName();
  string stringAttributWert;
  bool   success;

  // CR-016
  success = pPlanObjekt->getStringAttributWert( "xplan:rechtscharakter", stringAttributWert );
  if ( !success || stringAttributWert == "" )
  {
    m_pMigDateiProt->createMigrationFehler( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "xplan:rechtscharakter", FEHLER,
      pPlanObjekt->getKlassenName(),  "xplan:rechtscharakter",  "Das Attribut 'rechtscharakter' ist ein Pflichtattribut", "Der Rechtscharakter wird auf 9998 (Unbekannt) gesetzt" );
    pPlanObjekt->deleteStringAttribut( "xplan:rechtscharakter" );
    pPlanObjekt->setStringAttribut( "xplan:rechtscharakter", "9998" );
  }

  //  CR-058
  correctTextAbschnitt ( pPlanObjekt, "xplan:SO_TextAbschnitt" );

  // CR-007
  correctBereichRelation ( pPlanObjekt, "xplan:gehoertZuSO_Bereich"  );

  //  CR-009
  if ( klassenName == "xplan:SO_Schienenverkehrsrecht" )
    correctArtDerFestlegung ( pPlanObjekt );

  // CR-053
  success = pPlanObjekt->getStringAttributWert( "xplan:textSchluessel", stringAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteStringAttribut( "xplan:textSchluessel" );
    createTextObjekt ( pPlanObjekt, "xplan:SO_TextAbschnitt", stringAttributWert );
  }

  // CR-053
  success = pPlanObjekt->getStringAttributWert( "xplan:textSchluesselBegruendung", stringAttributWert );
  if ( success )
  {
    pPlanObjekt->deleteStringAttribut( "xplan:textSchluesselBegruendung" );
    createBegruendungObjekt ( pPlanObjekt, stringAttributWert );
  }

  //  CR-073
  if ( klassenName == "xplan:SO_SchutzgebietSonstRecht" )
  {
    success = pPlanObjekt->getStringAttributWert( "xplan:zone", stringAttributWert );
    if ( success )
    {
      m_pMigDateiProt->createMigrationFehler( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "xplan:zone", FEHLER,
        pPlanObjekt->getKlassenName(),  "",  "Das Attribut 'zone' wird nicht mehr unterstützt", "Attribut wird ignoriert" );
      pPlanObjekt->deleteStringAttribut( "xplan:zone" );
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Korrektur der Relationen von Planobjekt zu Bereich                       //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::correctBereichRelation ( Feature * pPlanObjekt, string rollenName  )
{
  vector<FeatureRelation*> vRel;
	size_t                   i, anz;

  anz = pPlanObjekt->getFeatureRelationen( rollenName, vRel );
  for ( i = 0; i < anz; i++ )
  {
    FeatureRelation * pRel = vRel[i];
    pPlanObjekt->removeFeatureRelation( pRel );
    pRel->rollenName = "xplan:gehoertZuBereich";
    pPlanObjekt->addFeatureRelation( pRel );
  }

  vRel.clear();
  anz = pPlanObjekt->getFeatureRelationen( "xplan:gehoertNachrichtlichZuBereich", vRel );
  for ( i = 0; i < anz; i++ )
  {
    FeatureRelation * pRel = vRel[i];
    pPlanObjekt->removeFeatureRelation( pRel );
    pRel->rollenName = "xplan:gehoertZuBereich";
    pPlanObjekt->addFeatureRelation( pRel );
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Vereinigung von 'zweckbestimmung' und 'besondereZweckbestimmung'         //
//  Elimination von 'weitereZweckbestimmungX', 'weitereBesondZweckbestimmungX'//
//  und 'weitereDetailZwechbestimmungX'                                      //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::correctZweckbestimmungen ( Feature * pPlanObjekt  )
{
  bool weiter;

  vector<string> zweckbestimmungen;
  vector<string> detaillierteZweckbestimmungen;
  vector<string> codespaces;

  weiter = correctZweckbestimmung ( pPlanObjekt, zweckbestimmungen, detaillierteZweckbestimmungen, codespaces, "" );
  if ( weiter )
    weiter = correctZweckbestimmung ( pPlanObjekt, zweckbestimmungen, detaillierteZweckbestimmungen, codespaces, "1" );
  if ( weiter )
    weiter = correctZweckbestimmung ( pPlanObjekt, zweckbestimmungen, detaillierteZweckbestimmungen, codespaces, "2" );
  if ( weiter )
    weiter = correctZweckbestimmung ( pPlanObjekt, zweckbestimmungen, detaillierteZweckbestimmungen, codespaces, "3" );
  if ( weiter )
    weiter = correctZweckbestimmung ( pPlanObjekt, zweckbestimmungen, detaillierteZweckbestimmungen, codespaces, "4" );
  if ( weiter )
    weiter = correctZweckbestimmung ( pPlanObjekt, zweckbestimmungen, detaillierteZweckbestimmungen, codespaces, "5" );

  for ( unsigned int i = 0; i < zweckbestimmungen.size(); i++ )
  {
    if ( i < detaillierteZweckbestimmungen.size() && detaillierteZweckbestimmungen[i] != "" )
    {
       pPlanObjekt->setStringAttribut( "xplan:zweckbestimmung", zweckbestimmungen[i] );
       if ( codespaces[i] != "" )
         pPlanObjekt->setStringAttribut( "xplan:detaillierteZweckbestimmung", detaillierteZweckbestimmungen[i], "codeSpace", codespaces[i] );
       else
         pPlanObjekt->setStringAttribut( "xplan:detaillierteZweckbestimmung", detaillierteZweckbestimmungen[i] );
    }
  }

  for ( unsigned int i = 0; i < zweckbestimmungen.size(); i++ )
  {
    if ( i >= detaillierteZweckbestimmungen.size() || detaillierteZweckbestimmungen[i] == "" )
      pPlanObjekt->setStringAttribut( "xplan:zweckbestimmung", zweckbestimmungen[i] );
  }
}


///////////////////////////////////////////////////////////////////////////////
//  Vereinigung von 'zweckbestimmung' und 'besondereZweckbestimmung'         //
//  Elimination von 'weitereZweckbestimmungX', 'weitereBesondZweckbestimmungX'//
//  und 'weitereDetailZwechbestimmungX'                                      //
///////////////////////////////////////////////////////////////////////////////
bool MigrationV5::correctZweckbestimmung ( Feature * pPlanObjekt, vector<string> &vZweckbestimmungen,  
                  vector<string> &vDetaillierteZweckbestimmungen, vector<string> &vCodespaces, std::string suffix )
{
  string attributNameZweckbestimmung;
  string attributNameBesondereZweckbestimmung;
  string attributNameDetaillierteZweckbestimmung;

  vector<string>      vZweckbestimmungenLoc;
  vector<string>      vBesondZweckbestimmungenLoc;
  vector<string>      vDetailZweckbestimmungenLoc;
  vector<string>      vCodespacesLoc;
  vector<StringAttr*> vDetailZweckbestimmungenObjLoc;

	size_t              anzZweckbest, anzBesondZweckbest, anzDetailZweckbest, i;

  if ( suffix== "" )
  {
    attributNameZweckbestimmung             = "xplan:zweckbestimmung";
    attributNameBesondereZweckbestimmung    = "xplan:besondereZweckbestimmung";
    attributNameDetaillierteZweckbestimmung = "xplan:detaillierteZweckbestimmung";
  }
  else
  {
    attributNameZweckbestimmung             = "xplan:weitereZweckbestimmung"       + suffix;
    attributNameBesondereZweckbestimmung    = "xplan:weitereBesondZweckbestimmung" + suffix;
    attributNameDetaillierteZweckbestimmung = "xplan:weitereDetailZweckbestimmung" + suffix;
  }

  anzZweckbest       = pPlanObjekt->getStringAttributWerte( attributNameZweckbestimmung, vZweckbestimmungenLoc );
  if ( anzZweckbest > 0 )
    pPlanObjekt->deleteStringAttribut( attributNameZweckbestimmung );

  anzBesondZweckbest = pPlanObjekt->getStringAttributWerte( attributNameBesondereZweckbestimmung, vBesondZweckbestimmungenLoc );
  if ( anzBesondZweckbest > 0 )
    pPlanObjekt->deleteStringAttribut( attributNameBesondereZweckbestimmung );

  anzDetailZweckbest = pPlanObjekt->getStringAttributWerte( attributNameDetaillierteZweckbestimmung, vDetailZweckbestimmungenObjLoc );
  if ( anzDetailZweckbest > 0 )
  {
    for ( i = 0; i < anzDetailZweckbest; i++ )
    {
      vDetailZweckbestimmungenLoc.push_back( vDetailZweckbestimmungenObjLoc[i]->value );
      if ( vDetailZweckbestimmungenObjLoc[i]->classifier.find( "codeSpace" ) !=  vDetailZweckbestimmungenObjLoc[i]->classifier.end() )
        vCodespacesLoc.push_back( vDetailZweckbestimmungenObjLoc[i]->classifier["codeSpace"] );
      else
        vCodespacesLoc.push_back( "" );
    }

    pPlanObjekt->deleteStringAttribut( attributNameDetaillierteZweckbestimmung );
  }

  if ( anzZweckbest > 0 )
  {    
    if ( anzBesondZweckbest > 0  )
    {
      for ( i = 0; i < anzBesondZweckbest; i++ )
        vZweckbestimmungen.push_back( vBesondZweckbestimmungenLoc[i] );
      if ( anzDetailZweckbest > 0 )
      {
        for ( i = 0; i < anzDetailZweckbest; i++ )
        {
          vDetaillierteZweckbestimmungen.push_back( vDetailZweckbestimmungenLoc[i] );
          vCodespaces.push_back( vCodespacesLoc[i] );
        }
      }
      else
      {
        for ( i = 0; i < anzBesondZweckbest; i++ )
        {
          vDetaillierteZweckbestimmungen.push_back( "" );
          vCodespaces.push_back( "" );
        }
      }
    }
    else
    {
      for ( i = 0; i < anzZweckbest; i++ )
        vZweckbestimmungen.push_back( vZweckbestimmungenLoc[i] );

      if ( anzDetailZweckbest > 0 )
      {
        for ( i = 0; i < anzDetailZweckbest; i++ )
        {
          vDetaillierteZweckbestimmungen.push_back( vDetailZweckbestimmungenLoc[i] );
          vCodespaces.push_back( vCodespacesLoc[i] );
        }
      }
      else
      {
        for ( i = 0; i < anzZweckbest; i++ )
        {
          vDetaillierteZweckbestimmungen.push_back( "" );
          vCodespaces.push_back( "" );
        }
      }
    }
  }
  else
  {
    if ( anzBesondZweckbest > 0  )
    {
      for ( i = 0; i < anzBesondZweckbest; i++ )
        vZweckbestimmungen.push_back( vBesondZweckbestimmungenLoc[i] );
      if ( anzDetailZweckbest > 0 )
      {
        for ( i = 0; i < anzDetailZweckbest; i++ )
        {
          vDetaillierteZweckbestimmungen.push_back( vDetailZweckbestimmungenLoc[i] );
          vCodespaces.push_back( vCodespacesLoc[i] );
        }
      }
      else
      {
        for ( i = 0; i < anzBesondZweckbest; i++ )
        {
          vDetaillierteZweckbestimmungen.push_back( "" );
          vCodespaces.push_back( "" );
        }
      }
    }
  }

  if ( anzZweckbest > 0 || anzBesondZweckbest > 0 )
    return true;
  else
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//  Vereinigung von 'artDerFestlegung' und 'besondereArtDerFestlegung'       //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::correctArtDerFestlegung ( Feature * pPlanObjekt )
{
  string attributNameArtDerFestlegung;
  string attributNameBesondereArtDerFestlegung;
  string attributNameDetaillierteArtDerFestlegung;

  vector<string>      vArtDerFestlegung;
  vector<string>      vBesondArtDerFestlegung;  
  vector<string>      vDetailArtDerFestlegung;
  vector<string>      vCodespace;  
  vector<StringAttr*> vDetailArtDerFestlegungObj;

	size_t anzArtDerFestlegung, anzBesondArtDerFestlegung, anzDetailArtDerFestlegung, i;

  attributNameArtDerFestlegung             = "xplan:artDerFestlegung";
  attributNameBesondereArtDerFestlegung    = "xplan:besondereArtDerFestlegung";
  attributNameDetaillierteArtDerFestlegung = "xplan:detailArtDerFestlegung";
  
  anzArtDerFestlegung = pPlanObjekt->getStringAttributWerte( attributNameArtDerFestlegung, vArtDerFestlegung );
  if ( anzArtDerFestlegung > 0 )
    pPlanObjekt->deleteStringAttribut( attributNameArtDerFestlegung );

  anzBesondArtDerFestlegung = pPlanObjekt->getStringAttributWerte( attributNameBesondereArtDerFestlegung, vBesondArtDerFestlegung );
  if ( anzBesondArtDerFestlegung > 0 )
    pPlanObjekt->deleteStringAttribut( attributNameBesondereArtDerFestlegung );

  anzDetailArtDerFestlegung = pPlanObjekt->getStringAttributWerte( attributNameDetaillierteArtDerFestlegung, vDetailArtDerFestlegungObj );
  if ( anzDetailArtDerFestlegung > 0 )
  {
    for ( i = 0; i < anzDetailArtDerFestlegung; i++ )
    {
      vDetailArtDerFestlegung.push_back( vDetailArtDerFestlegungObj[i]->value );
      if ( vDetailArtDerFestlegungObj[i]->classifier.find( "codeSpace" ) !=  vDetailArtDerFestlegungObj[i]->classifier.end() )
        vCodespace.push_back( vDetailArtDerFestlegungObj[i]->classifier["codeSpace"] );
      else
        vCodespace.push_back( "" );
    }

    pPlanObjekt->deleteStringAttribut( attributNameDetaillierteArtDerFestlegung );
  }

  if ( anzArtDerFestlegung > 0 )
  {    
    if ( anzBesondArtDerFestlegung > 0  )
    {
      for ( i = 0; i < anzBesondArtDerFestlegung; i++ )
        pPlanObjekt->setStringAttribut( attributNameArtDerFestlegung, vBesondArtDerFestlegung[i] );     
        
      if ( anzDetailArtDerFestlegung > 0 )
      {
        for ( i = 0; i < anzDetailArtDerFestlegung; i++ )
        {
          if ( vCodespace[i] != "" )
            pPlanObjekt->setStringAttribut( attributNameDetaillierteArtDerFestlegung, vDetailArtDerFestlegung[i], "codeSpace", vCodespace[i] );
          else
            pPlanObjekt->setStringAttribut( attributNameDetaillierteArtDerFestlegung, vDetailArtDerFestlegung[i] );
        }
      }      
    }
    else
    {
      for ( i = 0; i < anzArtDerFestlegung; i++ )
      pPlanObjekt->setStringAttribut( attributNameArtDerFestlegung, vArtDerFestlegung[i] );     
      if ( anzDetailArtDerFestlegung > 0 )
      {
        for ( i = 0; i < anzDetailArtDerFestlegung; i++ )
        {
          if ( vCodespace[i] != "" )
            pPlanObjekt->setStringAttribut( attributNameDetaillierteArtDerFestlegung,  vDetailArtDerFestlegung[i], "codeSpace", vCodespace[i] );
          else
            pPlanObjekt->setStringAttribut( attributNameDetaillierteArtDerFestlegung, vDetailArtDerFestlegung[i] );
        }
      }     
    }
  }
  else
  {
    if ( anzBesondArtDerFestlegung > 0  )
    {
      for ( i = 0; i < anzBesondArtDerFestlegung; i++ )
        pPlanObjekt->setStringAttribut( attributNameArtDerFestlegung, vBesondArtDerFestlegung[i] );  

      if ( anzDetailArtDerFestlegung> 0 )
      {
        for ( i = 0; i < anzDetailArtDerFestlegung; i++ )
        {
          if ( vCodespace[i] != "" )
            pPlanObjekt->setStringAttribut( attributNameDetaillierteArtDerFestlegung,  vDetailArtDerFestlegung[i], "codeSpace", vCodespace[i] );
          else
            pPlanObjekt->setStringAttribut( attributNameDetaillierteArtDerFestlegung, vDetailArtDerFestlegung[i] );
        }
      }
    }      
  }  
}

///////////////////////////////////////////////////////////////////////////////
//  Elimination der Attribute 'weitereMassnahme1' und 'weitereMassnahme2'    //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::correctMassnahmen ( Feature * pPlanObjekt  )
{
  vector<FeatureRelation*> vRel;
	size_t                   i, anz;

  anz = pPlanObjekt->getFeatureRelationen( "xplan:weitereMassnahme1", vRel );
  for ( i = 0; i < anz; i++ )
  {
    FeatureRelation * pRel = vRel[i];
    pPlanObjekt->removeFeatureRelation( pRel );
    pRel->rollenName = "xplan:massnahme";
    pPlanObjekt->addFeatureRelation( pRel );
  }

  vRel.clear();

  anz = pPlanObjekt->getFeatureRelationen( "xplan:weitereMassnahme2", vRel );
  for ( i = 0; i < anz; i++ )
  {
    FeatureRelation * pRel = vRel[i];
    pPlanObjekt->removeFeatureRelation( pRel );
    pRel->rollenName = "xplan:massnahme";
    pPlanObjekt->addFeatureRelation( pRel );
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Umwandlung Attribut 'textSchluesselBegruendung' in Relation auf          //
//  XP_BegruendungAbschnitt                                                  //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::createBegruendungObjekt ( Feature * pPlanObjekt, std::string schluessel )
{
  Feature * pTextObjekt = m_pPlanObjekte->createFeature( "xplan:XP_BegruendungAbschnitt", NULL, "gml:featureMember" );
  pTextObjekt->setStringAttribut( "xplan:schluessel", schluessel );

  FeatureRelation * pRel = new FeatureRelation;
  pRel->rollenName = "xplan:refBegruendungInhalt";
  pRel->featureId  = pTextObjekt->getGmlId();
  pPlanObjekt->addFeatureRelation( pRel );
}

///////////////////////////////////////////////////////////////////////////////
//  Umwandlung Attribut 'textSchluessel' in Relation auf                     //
//  Text-Objekt                                                              //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::createTextObjekt ( Feature * pPlanObjekt, std::string textKlassenName, std::string schluessel )
{
  string rechtscharakter;
  bool   success;

  Feature * pTextObjekt = m_pPlanObjekte->createFeature( textKlassenName, NULL, "gml:featureMember" );
  pTextObjekt->setStringAttribut( "xplan:schluessel", schluessel );

  success = pPlanObjekt->getStringAttributWert( "xplan:rechtscharakter", rechtscharakter );
  if ( success )
    pTextObjekt->setStringAttribut( "xplan:rechtscharakter", rechtscharakter );

  FeatureRelation * pRel = new FeatureRelation;
  pRel->rollenName = "xplan:refTextInhalt";
  pRel->featureId  = pTextObjekt->getGmlId();
  pPlanObjekt->addFeatureRelation( pRel );
}

///////////////////////////////////////////////////////////////////////////////
//  Korrektur geänderter Codes für Zweckbestimmung Ver- und Entsorgung       //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::correctVerEntsorgungCodes ( Feature * pPlanObjekt )
{
  vector<StringAttr*> vStringAttr;
	size_t              i, anz;

  anz = pPlanObjekt->getStringAttributWerte( "xplan:zweckbestimmung", vStringAttr );
  for ( i = 0; i < anz; i++ )
  {
    StringAttr * pAttr = vStringAttr[i];

    if ( pAttr->value == "10010" )
      pAttr->value = "100010";
    else
    if ( pAttr->value == "28000" )
      pAttr->value = "10003";
    else
    if ( pAttr->value == "28001" )
      pAttr->value = "10002";
    else
    if ( pAttr->value == "28002" )
      pAttr->value = "10007";
    else
    if ( pAttr->value == "28003" )
      pAttr->value = "10004";
    else
    if ( pAttr->value == "28004" )
      pAttr->value = "2800";
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Überprüft die Verwendung von BMmin, BMmax. BMZmin, BMZmax                //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::checkMasszahlen ( Feature * pPlanObjekt )
{
  bool   success;
  double wert;

  success = pPlanObjekt->getDoubleAttributWert( "xplan:BMmin", wert );
  if ( success )
  {
    m_pMigDateiProt->createMigrationFehler( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "xplan:BMmin", FEHLER,
      pPlanObjekt->getKlassenName(),  "",  "Das Attribut 'BMmin' wird nicht mehr unterstützt", "Attribut wird ignoriert" );
    pPlanObjekt->deleteDoubleAttribut( "xplan:BMmin" );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:BMmax", wert );
  if ( success )
  {
    m_pMigDateiProt->createMigrationFehler( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "xplan:BMmax", FEHLER,
      pPlanObjekt->getKlassenName(),  "",  "Das Attribut 'BMmax' wird nicht mehr unterstützt", "Attribut wird ignoriert" );
    pPlanObjekt->deleteDoubleAttribut( "xplan:BMmax" );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:BMZmin", wert );
  if ( success )
  {
    m_pMigDateiProt->createMigrationFehler( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "xplan:BMZmin", FEHLER,
      pPlanObjekt->getKlassenName(),  "",  "Das Attribut 'BMZmin' wird nicht mehr unterstützt", "Attribut wird ignoriert" );
    pPlanObjekt->deleteDoubleAttribut( "xplan:BMZmin" );
  }

  success = pPlanObjekt->getDoubleAttributWert( "xplan:BMZmax", wert );
  if ( success )
  {
    m_pMigDateiProt->createMigrationFehler( pPlanObjekt->getGmlId(), pPlanObjekt->getKlassenName(), "xplan:BMZmax", FEHLER,
      pPlanObjekt->getKlassenName(),  "",  "Das Attribut 'BMZmax' wird nicht mehr unterstützt", "Attribut wird ignoriert" );
    pPlanObjekt->deleteDoubleAttribut( "xplan:BMZmax" );
  }

}

///////////////////////////////////////////////////////////////////////////////
//  Umbenennung XP_RasterplanBsis --> XP_Rasterdarstellung                   //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::transformRasterBasis (  Feature * pPlan, Feature * pPlanBereich, Feature * pRasterplanBasis )
{
  vector<Feature*> vExterneReferenzen;
  Feature        * pExterneReferenz;
  Feature        * pExterneReferenzNew;
  Feature        * pTextObjekt;
	size_t           i, anz;
  string           textObjektName;
  string           textObjektRechtscharakter;

  anz = pRasterplanBasis->getRelatedFeatures( "xplan:refScan", vExterneReferenzen );
  for ( i = 0; i < anz; ++i )
  {
    pExterneReferenz = vExterneReferenzen[i];
    pExterneReferenzNew = new Feature ( pExterneReferenz, pExterneReferenz->getGmlId() );
    m_pPlanObjekte->removeFeature( pExterneReferenz->getGmlId() );
    m_pPlanObjekte->addFeature( pExterneReferenzNew );

    pPlanBereich->addRelatedFeature( pExterneReferenzNew, "xplan:refScan", false );
    pExterneReferenzNew->setParentId(pPlanBereich->getGmlId() );
  }

  vExterneReferenzen.clear();
  anz = pRasterplanBasis->getRelatedFeatures( "xplan:refText", vExterneReferenzen );

  if ( pPlan->getKlassenName() == "xplan:BP_Plan" )
  {
    textObjektName = "xplan:BP_TextAbschnitt";
    textObjektRechtscharakter = "1000";
  }
  else
  if ( pPlan->getKlassenName() == "xplan:FP_Plan" )
  {
    textObjektName = "xplan:FP_TextAbschnitt";
    textObjektRechtscharakter = "1000";
  }
  else
  if ( pPlan->getKlassenName() == "xplan:SO_Plan" )
  {
    textObjektName = "xplan:SO_TextAbschnitt";
    textObjektRechtscharakter = "2000";
  }

  for ( i = 0; i < anz; ++i )
  {
    pExterneReferenz = vExterneReferenzen[i];
    pExterneReferenzNew = new Feature ( pExterneReferenz, pExterneReferenz->getGmlId() );
    m_pPlanObjekte->removeFeature( pExterneReferenz->getGmlId() );
    m_pPlanObjekte->addFeature( pExterneReferenzNew );

    pTextObjekt = m_pPlanObjekte->createFeature( textObjektName, NULL, "gml:featureMember", Constant::createGmlId() );
    pTextObjekt->addRelatedFeature( pExterneReferenzNew, "xplan:refText", false );
    pTextObjekt->setStringAttribut( "xplan:rechtscharakter", textObjektRechtscharakter );

    pExterneReferenzNew->setParentId( pTextObjekt->getGmlId() );
  }

  vExterneReferenzen.clear();
  anz = pRasterplanBasis->getRelatedFeatures( "xplan:refLegende", vExterneReferenzen );
  for ( i = 0; i < anz; ++i )
  {
    pExterneReferenz = vExterneReferenzen[i];
    pExterneReferenzNew = new Feature ( pExterneReferenz, pExterneReferenz->getGmlId() );
    m_pPlanObjekte->removeFeature( pExterneReferenz->getGmlId() );
    m_pPlanObjekte->addFeature( pExterneReferenzNew );

    pExterneReferenzNew->setKlassenName( "xplan:XP_SpezExterneReferenz" );
    pExterneReferenzNew->setStringAttribut( "xplan:typ", "1020" );

    pPlan->addRelatedFeature( pExterneReferenzNew, "xplan:externeReferenz", false );
    pExterneReferenzNew->setParentId( pPlan->getGmlId() );
  }

  pPlanBereich->removeFeatureRelation( pRasterplanBasis );
  pPlanBereich->removeChild( pRasterplanBasis->getGmlId() );
  m_pPlanObjekte->removeFeature( pRasterplanBasis->getGmlId() );
}

///////////////////////////////////////////////////////////////////////////////
//  Umwandlung der virtuellen Klasse XP_TextAbschnitt in eine reale Klasse   //
///////////////////////////////////////////////////////////////////////////////
void MigrationV5::correctTextAbschnitt (  Feature * pPlanObjekt, std::string textAbschnittKlasse )
{
  vector<Feature*> vFeatures;
	size_t           i, anz;
  string           rechtscharakter;

  pPlanObjekt->getStringAttributWert ( "xplan:rechtscharakter", rechtscharakter );

  anz = pPlanObjekt->getRelatedFeatures( "refTextInhalt", vFeatures );

  for ( i = 0; i < anz; i++ )
  {
    Feature * pTextAbschnitt = vFeatures[i];

    if ( pTextAbschnitt->getKlassenName() != textAbschnittKlasse )
      pTextAbschnitt->setKlassenName( textAbschnittKlasse );
    pTextAbschnitt->setStringAttribut( "xplan:rechtscharakter", rechtscharakter );
  }

}












