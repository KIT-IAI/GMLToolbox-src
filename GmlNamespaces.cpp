#include "StdAfx.h"

#include <time.h>
#include <vector>
#include <map>
#include <string>

#include "GmlNamespaces.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

std::string GmlNamespaces::xmlSchemaStandardKuerzel     = "xs";

// Target Namespaces
std::string GmlNamespaces:: gml_3_0_targetNamespace      = "http://www.opengis.net/gml";
std::string GmlNamespaces:: gml_3_2_targetNamespace      = "http://www.opengis.net/gml/3.2";
std::string GmlNamespaces:: xmlSchema_targetNamespace    = "http://www.w3.org/2001/XMLSchema";
std::string GmlNamespaces:: xmlSchemaInstance_targetNamespace ="http://www.w3.org/2001/XMLSchema-instance";
std::string GmlNamespaces::xAL_2_0_targetNamespace        =  "urn:oasis:names:tc:ciq:xsdschema:xAL:2.0";

std::string GmlNamespaces::xplanGml_2_0_targetNamespace         = "http://www.xplanung.de/xplangml";
std::string GmlNamespaces::xplanGml_3_0_targetNamespace         = "http://www.xplanung.de/xplangml/3/0";
std::string GmlNamespaces::xplanGml_4_0_targetNamespace         = "http://www.xplanung.de/xplangml/4/0";
std::string GmlNamespaces::xplanGml_4_1_targetNamespace         = "http://www.xplanung.de/xplangml/4/1";
std::string GmlNamespaces::xplanGml_5_0_targetNamespace         = "http://www.xplanung.de/xplangml/5/0";
std::string GmlNamespaces::xplanGml_5_1_targetNamespace         = "http://www.xplanung.de/xplangml/5/1";
std::string GmlNamespaces::xplanGml_5_2_targetNamespace         = "http://www.xplanung.de/xplangml/5/2";
std::string GmlNamespaces::xplanGml_5_3_targetNamespace         = "http://www.xplanung.de/xplangml/5/3";

std::string GmlNamespaces::cityGml_0_4_0_targetNamespace        = "http://www.citygml.org/citygml/1/0/0";
std::string GmlNamespaces::cityGml_1_0_targetNamespace          = "http://www.opengis.net/citygml/1.0"; 
std::string GmlNamespaces::cityGml_2_0_targetNamespace          = "http://www.opengis.net/citygml/2.0"; 
std::string GmlNamespaces::cityGml_3_0_targetNamespace          = "http://www.opengis.net/citygml/3.0"; 

std::string GmlNamespaces::cityGml_2_0_buildingNamespace        = "http://www.opengis.net/citygml/building/2.0"; 
std::string GmlNamespaces::cityGml_2_0_bridgeNamespace          = "http://www.opengis.net/citygml/bridge/2.0"; 
std::string GmlNamespaces::cityGml_2_0_tunnelNamespace          = "http://www.opengis.net/citygml/tunnel/2.0"; 
std::string GmlNamespaces::cityGml_2_0_cityFurnitureNamespace   = "http://www.opengis.net/citygml/cityfurniture/2.0"; 
std::string GmlNamespaces::cityGml_2_0_genericsNamespace        = "http://www.opengis.net/citygml/generics/2.0"; 
std::string GmlNamespaces::cityGml_2_0_landUseNamespace         = "http://www.opengis.net/citygml/landuse/2.0"; 
std::string GmlNamespaces::cityGml_2_0_reliefNamespace          = "http://www.opengis.net/citygml/relief/2.0"; 
std::string GmlNamespaces::cityGml_2_0_transportationNamespace  = "http://www.opengis.net/citygml/transportation/2.0"; 
std::string GmlNamespaces::cityGml_2_0_vegetationNamespace      = "http://www.opengis.net/citygml/vegetation/2.0"; 
std::string GmlNamespaces::cityGml_2_0_waterBodyNamespace       = "http://www.opengis.net/citygml/waterBody/2.0"; 

std::string GmlNamespaces::cityGml_2_0_georepNamespace          = "http://www.citygml.org/ade/georep/2.0";
std::string GmlNamespaces::cityGml_2_0_georepNamespaceKuerzel   = "rep";

std::string GmlNamespaces::INSPIRE_PLU_4_0_targetNamespace      = "http://inspire.ec.europa.eu/schemas/plu/4.0";
std::string GmlNamespaces::INSPIRE_targetNamespace              = "http://inspire.ec.europa.eu/schemas/base/3.3";

std::string GmlNamespaces::nas_5_1_targetNamespace              = "http://www.adv-online.de/namespaces/adv/gid/5.1";
std::string GmlNamespaces::nas_5_1_1_targetNamespace              = "http://www.adv-online.de/namespaces/adv/gid/5.1.1";
std::string GmlNamespaces::nas_6_0_targetNamespace              = "http://www.adv-online.de/namespaces/adv/gid/6.0";

std::string GmlNamespaces::boreholeML_3_0_targetNamespace       = "http://www.infogeo.de/boreholeml/3.0";
std::string GmlNamespaces::boreholeHeaderML_3_0_targetNamespace = "http://www.infogeo.de/boreholeml/3.0/header";

///////////////////////////////////////////////////////////////////////////////
//  GmlNamespaces                                                            //
//  Namespaces von ein oder mehreren eingelesenen XML-Schemadateien          //
///////////////////////////////////////////////////////////////////////////////
GmlNamespaces::GmlNamespaces()
{
}

GmlNamespaces::GmlNamespaces(  int schemaType )
{
  namespaces["xlink"] = "http://www.w3.org/1999/xlink";
  namespaces["xsi"]   = "http://www.w3.org/2001/XMLSchema-instance";
  namespaces["xs"]    = "http://www.w3.org/2001/XMLSchema";

  namespaceKuerzel["http://www.w3.org/1999/xlink"]              = "xlink";
  namespaceKuerzel["http://www.w3.org/2001/XMLSchema-instance"] = "xsi";
  namespaceKuerzel["http://www.w3.org/2001/XMLSchema"]          = "xs";

  switch ( schemaType )
  {
  case CITYGML_0_4_0:
    namespaces["gml"]     = gml_3_0_targetNamespace;
    namespaces["xAL"]     = xAL_2_0_targetNamespace;
    namespaces["citygml"] = cityGml_0_4_0_targetNamespace;
    namespaces["noise"]   = "http://www.citygml.org/ade/noise_de";

    namespaceKuerzel[gml_3_0_targetNamespace]                    = "gml";
    namespaceKuerzel["urn:oasis:names:tc:ciq:xsdschema:xAL:2.0"] = "xAL";
    namespaceKuerzel[cityGml_0_4_0_targetNamespace]              = "citygml";
    namespaceKuerzel["http://www.citygml.org/ade/noise_de"]      = "noise";
    break;

  case CITYGML_1_0:
    namespaces["gml"]     = gml_3_0_targetNamespace;
    namespaces["xAL"]     = xAL_2_0_targetNamespace;
    namespaces["core"]    = cityGml_1_0_targetNamespace;
    namespaces["bldg"]    = "http://www.opengis.net/citygml/building/1.0";
    namespaces["trpt"]    = "http://www.opengis.net/citygml/transportation/1.0";
    namespaces["veget"]   = "http://www.opengis.net/citygml/vegetation/1.0";
    namespaces["water"]   = "http://www.opengis.net/citygml/waterbody/1.0";
    namespaces["furnit"]  = "http://www.opengis.net/citygml/cityfurniture/1.0";
    namespaces["relief"]  = "http://www.opengis.net/citygml/relief/1.0";
    namespaces["landuse"] = "http://www.opengis.net/citygml/landuse/1.0";
    namespaces["group"]   = "http://www.opengis.net/citygml/cityobjectgroup/1.0";
    namespaces["genobj"]  = "http://www.opengis.net/citygml/generics/1.0";
    namespaces["app"]     = "http://www.opengis.net/citygml/appearance/1.0";
    namespaces["texsurf"] = "http://www.opengis.net/citygml/texturedsurface/1.0";
    namespaces["noise"]   = "http://www.citygml.org/ade/noise_de";
    namespaces["adetun"]  = "http://www.opengis.net/citygml/ade/tun/1.0";
    namespaces["adebrid"] = "http://www.opengis.net/citygml/ade/bridge/1.0";
    namespaces["cafm"]    = "http://www.gis.bv.tum.de/citygml/cafm";
    namespaces["dba"]    = "http://www.opengis.net/citygml/ade/digitalerBauantrag/0.5";

    namespaceKuerzel[gml_3_0_targetNamespace]                                = "gml";
    namespaceKuerzel["urn:oasis:names:tc:ciq:xsdschema:xAL:2.0"]             = "xAL";
    namespaceKuerzel[cityGml_1_0_targetNamespace]                            = "core";
    namespaceKuerzel["http://www.opengis.net/citygml/building/1.0"]          = "bldg";
    namespaceKuerzel["http://www.opengis.net/citygml/transportation/1.0"]    = "trpt";
    namespaceKuerzel["http://www.opengis.net/citygml/vegetation/1.0"]        = "veget";
    namespaceKuerzel["http://www.opengis.net/citygml/waterbody/1.0"]         = "water";
    namespaceKuerzel["http://www.opengis.net/citygml/cityfurniture/1.0"]     = "furnit";
    namespaceKuerzel["http://www.opengis.net/citygml/relief/1.0"]            = "relief";
    namespaceKuerzel["http://www.opengis.net/citygml/landuse/1.0"]           = "landuse";
    namespaceKuerzel["http://www.opengis.net/citygml/cityobjectgroup/1.0"]   = "group";
    namespaceKuerzel["http://www.opengis.net/citygml/generics/1.0"]          = "genobj";
    namespaceKuerzel["http://www.opengis.net/citygml/appearance/1.0"]        = "app";
    namespaceKuerzel["http://www.opengis.net/citygml/texturedsurface/1.0"]   = "texsurf";
    namespaceKuerzel["http://www.citygml.org/ade/noise_de"]                  = "noise";
    namespaceKuerzel["http://www.opengis.net/citygml/ade/tun/1.0"]           = "adetun";
    namespaceKuerzel["http://www.opengis.net/citygml/ade/bridge/1.0"]        = "adebrid";
    namespaceKuerzel["http://www.gis.bv.tum.de/citygml/cafm"]                = "cafm";
    namespaceKuerzel["http://www.opengis.net/citygml/ade/digitalerBauantrag/0.5"] = "dba";
    break;

  case CITYGML_2_0:
    namespaces["gml"]     = gml_3_0_targetNamespace;
    namespaces["xAL"]     = xAL_2_0_targetNamespace;
    namespaces["core"]    = cityGml_2_0_targetNamespace;
    namespaces["bldg"]    = "http://www.opengis.net/citygml/building/2.0";
    namespaces["trpt"]    = "http://www.opengis.net/citygml/transportation/2.0";
    namespaces["veget"]   = "http://www.opengis.net/citygml/vegetation/2.0";
    namespaces["water"]   = "http://www.opengis.net/citygml/waterbody/2.0";
    namespaces["furnit"]  = "http://www.opengis.net/citygml/cityfurniture/2.0";
    namespaces["relief"]  = "http://www.opengis.net/citygml/relief/2.0";
    namespaces["landuse"] = "http://www.opengis.net/citygml/landuse/2.0";
    namespaces["group"]   = "http://www.opengis.net/citygml/cityobjectgroup/2.0";
    namespaces["genobj"]  = "http://www.opengis.net/citygml/generics/2.0";
    namespaces["app"]     = "http://www.opengis.net/citygml/appearance/2.0";
    namespaces["texsurf"] = "http://www.opengis.net/citygml/texturedsurface/2.0";
    namespaces["noise"]   = "http://www.citygml.org/ade/noise_de/2.0";
    namespaces["tun"]     = "http://www.citygml.org/tunnel/2.0";
    namespaces["brid"]    = "http://www.citygml.org/bridge/2.0";

    namespaceKuerzel[gml_3_0_targetNamespace]                                = "gml";
    namespaceKuerzel["urn:oasis:names:tc:ciq:xsdschema:xAL:2.0"]             = "xAL";
    namespaceKuerzel[cityGml_2_0_targetNamespace]                            = "core";
    namespaceKuerzel["http://www.opengis.net/citygml/building/2.0"]          = "bldg";
    namespaceKuerzel["http://www.opengis.net/citygml/transportation/2.0"]    = "trpt";
    namespaceKuerzel["http://www.opengis.net/citygml/vegetation/2.0"]        = "veget";
    namespaceKuerzel["http://www.opengis.net/citygml/waterbody/2.0"]         = "water";
    namespaceKuerzel["http://www.opengis.net/citygml/cityfurniture/2.0"]     = "furnit";
    namespaceKuerzel["http://www.opengis.net/citygml/relief/2.0"]            = "relief";
    namespaceKuerzel["http://www.opengis.net/citygml/landuse/2.0"]           = "landuse";
    namespaceKuerzel["http://www.opengis.net/citygml/cityobjectgroup/2.0"]   = "group";
    namespaceKuerzel["http://www.opengis.net/citygml/generics/2.0"]          = "genobj";
    namespaceKuerzel["http://www.opengis.net/citygml/appearance/2.0"]        = "app";
    namespaceKuerzel["http://www.opengis.net/citygml/texturedsurface/2.0"]   = "texsurf";
    namespaceKuerzel["http://www.citygml.org/ade/noise_de/2.0"]              = "noise";
    namespaceKuerzel["http://www.opengis.net/citygml/tunnel/2.0"]            = "tun";
    namespaceKuerzel["http://www.opengis.net/citygml/bridge/2.0"]            = "brid";
    break;


  case CITYGML_3_0:
    namespaces["gml"]     = gml_3_2_targetNamespace;
    namespaces["xAL"]     = xAL_2_0_targetNamespace;
    namespaces["core"]    = cityGml_3_0_targetNamespace;
    namespaces["bldg"]    = "http://www.opengis.net/citygml/building/3.0";
    namespaces["trpt"]    = "http://www.opengis.net/citygml/transportation/3.0";
    namespaces["veget"]   = "http://www.opengis.net/citygml/vegetation/3.0";
    namespaces["water"]   = "http://www.opengis.net/citygml/waterbody/3.0";
    namespaces["furnit"]  = "http://www.opengis.net/citygml/cityfurniture/3.0";
    namespaces["relief"]  = "http://www.opengis.net/citygml/relief/3.0";
    namespaces["landuse"] = "http://www.opengis.net/citygml/landuse/3.0";
    namespaces["group"]   = "http://www.opengis.net/citygml/cityobjectgroup/3.0";
    namespaces["genobj"]  = "http://www.opengis.net/citygml/generics/3.0";
    namespaces["app"]     = "http://www.opengis.net/citygml/appearance/3.0";  
    namespaces["tun"]     = "http://www.citygml.org/tunnel/3.0";
    namespaces["brid"]    = "http://www.citygml.org/bridge/3.0";
    namespaces["pcl"]     = "http://www.citygml.org/pointcloud/3.0";
    namespaces["dyn"]     = "http://www.citygml.org/dynamizer/3.0";  
    namespaces["vers"]    = "http://www.citygml.org/versioning/3.0";

    namespaceKuerzel[gml_3_2_targetNamespace]                                = "gml";
    namespaceKuerzel["urn:oasis:names:tc:ciq:xsdschema:xAL:2.0"]             = "xAL";
    namespaceKuerzel[cityGml_3_0_targetNamespace]                            = "core";
    namespaceKuerzel["http://www.opengis.net/citygml/building/3.0"]          = "bldg";
    namespaceKuerzel["http://www.opengis.net/citygml/transportation/3.0"]    = "trpt";
    namespaceKuerzel["http://www.opengis.net/citygml/vegetation/3.0"]        = "veget";
    namespaceKuerzel["http://www.opengis.net/citygml/waterbody/3.0"]         = "water";
    namespaceKuerzel["http://www.opengis.net/citygml/cityfurniture/3.0"]     = "furnit";
    namespaceKuerzel["http://www.opengis.net/citygml/relief/3.0"]            = "relief";
    namespaceKuerzel["http://www.opengis.net/citygml/landuse/3.0"]           = "landuse";
    namespaceKuerzel["http://www.opengis.net/citygml/cityobjectgroup/3.0"]   = "group";
    namespaceKuerzel["http://www.opengis.net/citygml/generics/3.0"]          = "genobj";
    namespaceKuerzel["http://www.opengis.net/citygml/appearance/3.0"]        = "app";       
    namespaceKuerzel["http://www.opengis.net/citygml/tunnel/3.0"]            = "tun";
    namespaceKuerzel["http://www.opengis.net/citygml/bridge/3.0"]            = "brid";
    namespaceKuerzel["http://www.citygml.org/pointcloud/3.0"]                = "pcl";
    namespaceKuerzel["http://www.citygml.org/dynamizer/3.0"]                 = "dyn";
    namespaceKuerzel["http://www.citygml.org/versioning/3.0"]                = "vers";
    break;

  case BOREHOLEML_3_0:
    namespaceKuerzel[gml_3_2_targetNamespace]                                   = "gml";
    namespaceKuerzel[boreholeML_3_0_targetNamespace]                            = "bml";
    namespaceKuerzel[boreholeHeaderML_3_0_targetNamespace]                      = "bmlh";
    namespaceKuerzel["http://www.isotc211.org/2005/gco"]                        = "gco";
    namespaceKuerzel["http://www.isotc211.org/2005/gmd"]                        = "gmd";
    namespaceKuerzel["urn:x-inspire:specification:gmlas:GeographicalNames:3.0"] = "gn";

    namespaces["gml"]     = gml_3_2_targetNamespace;
    namespaces["bml"]     = boreholeML_3_0_targetNamespace;
    namespaces["bmlh"]    = boreholeHeaderML_3_0_targetNamespace;
    namespaces["gco"]     = "http://www.isotc211.org/2005/gco";
    namespaces["gmd"]     = "http://www.isotc211.org/2005/gmd";
    namespaces["gn"]      = "urn:x-inspire:specification:gmlas:GeographicalNames:3.0";
    break;

  case NAS_5_1:
    namespaces["gml"]     = gml_3_0_targetNamespace;
    namespaces["wfs"]   = "http://www.opengis.net/wfs";
    namespaces["adv"]   = nas_5_1_targetNamespace;

    namespaceKuerzel[gml_3_0_targetNamespace]        = "gml";
    namespaceKuerzel["http://www.opengis.net/wfs"]   = "wfs";
    namespaceKuerzel[nas_5_1_targetNamespace]        = "adv";
    break;

  case NAS_5_1_1:
    namespaces["gml"]   = gml_3_0_targetNamespace;
    namespaces["wfs"]   = "http://www.opengis.net/wfs";
    namespaces["adv"]   = nas_5_1_1_targetNamespace;

    namespaceKuerzel[gml_3_0_targetNamespace]        = "gml";
    namespaceKuerzel["http://www.opengis.net/wfs"]   = "wfs";
    namespaceKuerzel[nas_5_1_1_targetNamespace]      = "adv";
    break;

  case NAS_6_0:
    namespaces["gml"]     = gml_3_2_targetNamespace;
    namespaces["wfs"]   = "http://www.opengis.net/wfs";
    namespaces["adv"]   = nas_6_0_targetNamespace;

    namespaceKuerzel[gml_3_2_targetNamespace]        = "gml";
    namespaceKuerzel["http://www.opengis.net/wfs"]   = "wfs";
    namespaceKuerzel[nas_6_0_targetNamespace]        = "adv";
    break;

  case  XPLANGML_2_0:
    namespaces["adv"]   = "http://www.adv-online.de/nas";
    namespaces["xplan"] = xplanGml_2_0_targetNamespace;
    namespaces["gml"]   = gml_3_0_targetNamespace;

    namespaceKuerzel[gml_3_0_targetNamespace]        = "gml";
    namespaceKuerzel["http://www.adv-online.de/nas"] = "adv";
    namespaceKuerzel[xplanGml_2_0_targetNamespace]   = "xplan";
    break;

  case  XPLANGML_3_0:
    namespaces["adv"]   = "http://www.adv-online.de/nas";
    namespaces["xplan"] = xplanGml_3_0_targetNamespace;
    namespaces["gml"]   = gml_3_0_targetNamespace;

    namespaceKuerzel[gml_3_0_targetNamespace]        = "gml";
    namespaceKuerzel["http://www.adv-online.de/nas"] = "adv";
    namespaceKuerzel[xplanGml_3_0_targetNamespace]   = "xplan";
    break;

  case  XPLANGML_4_0:
    namespaces["adv"]   = "http://www.adv-online.de/nas";
    namespaces["xplan"] = xplanGml_4_0_targetNamespace;
    namespaces["gml"]   = gml_3_2_targetNamespace;

    namespaceKuerzel[gml_3_2_targetNamespace]        = "gml";
    namespaceKuerzel["http://www.adv-online.de/nas"] = "adv";
    namespaceKuerzel[xplanGml_4_0_targetNamespace]   = "xplan";
    break;

  case  XPLANGML_4_1:
    namespaces["adv"]   = "http://www.adv-online.de/nas";
    namespaces["xplan"] = xplanGml_4_1_targetNamespace;
    namespaces["gml"]   = gml_3_2_targetNamespace;

    namespaceKuerzel[gml_3_2_targetNamespace]        = "gml";
    namespaceKuerzel["http://www.adv-online.de/nas"] = "adv";
    namespaceKuerzel[xplanGml_4_1_targetNamespace]   = "xplan";
    break;

  case  XPLANGML_5_0:
    namespaces["adv"]   = "http://www.adv-online.de/nas";
    namespaces["xplan"] = xplanGml_5_0_targetNamespace;
    namespaces["gml"]   = gml_3_2_targetNamespace;

    namespaceKuerzel[gml_3_2_targetNamespace]        = "gml";
    namespaceKuerzel["http://www.adv-online.de/nas"] = "adv";
    namespaceKuerzel[xplanGml_5_0_targetNamespace]   = "xplan";
    break;

  case  XPLANGML_5_1:
    namespaces["adv"]   = "http://www.adv-online.de/nas";
    namespaces["xplan"] = xplanGml_5_1_targetNamespace;
    namespaces["gml"]   = gml_3_2_targetNamespace;

    namespaceKuerzel[gml_3_2_targetNamespace]        = "gml";
    namespaceKuerzel["http://www.adv-online.de/nas"] = "adv";
    namespaceKuerzel[xplanGml_5_1_targetNamespace]   = "xplan";
    break;

  case  XPLANGML_5_2:
    namespaces["adv"]   = "http://www.adv-online.de/nas";
    namespaces["xplan"] = xplanGml_5_2_targetNamespace;
    namespaces["gml"]   = gml_3_2_targetNamespace;

    namespaceKuerzel[gml_3_2_targetNamespace]        = "gml";
    namespaceKuerzel["http://www.adv-online.de/nas"] = "adv";
    namespaceKuerzel[xplanGml_5_2_targetNamespace]   = "xplan";
    break;

  case  XPLANGML_5_3:
    namespaces["adv"]   = "http://www.adv-online.de/nas";
    namespaces["xplan"] = xplanGml_5_3_targetNamespace;
    namespaces["gml"]   = gml_3_2_targetNamespace;

    namespaceKuerzel[gml_3_2_targetNamespace]        = "gml";
    namespaceKuerzel["http://www.adv-online.de/nas"] = "adv";
    namespaceKuerzel[xplanGml_5_3_targetNamespace]   = "xplan";
    break;

  case INSPIRE_LU_PLANNED:
    namespaces["plu"]    = INSPIRE_PLU_4_0_targetNamespace;
    namespaces["lunom"]  = "http://http://inspire.ec.europa.eu/schemas/lunom/3.0";
    namespaces["base"]   = "http://inspire.ec.europa.eu/schemas/base/3.3";
    namespaces["base2"]  = "http://inspire.ec.europa.eu/schemas/base2/1.0";
    namespaces["gml"]    = gml_3_2_targetNamespace;

    namespaceKuerzel[gml_3_2_targetNamespace]                         = "gml";
    namespaceKuerzel[INSPIRE_PLU_4_0_targetNamespace]                 = "plu";
    namespaceKuerzel["http://inspire.ec.europa.eu/schemas/base/3.3"]  = "base";
    namespaceKuerzel["http://inspire.ec.europa.eu/schemas/base2/1.0"] = "base2";
    namespaceKuerzel["http://inspire.ec.europa.eu/schemas/lunom/3.0"] = "lunom";
    break;

  case INSPIRE:
    namespaces["gml"]       = gml_3_2_targetNamespace;
    namespaces["gco"]       = "http://www.isotc211.org/2005/gco";
    namespaces["gmd"]       = "http://www.isotc211.org/2005/gmd";
    namespaces["base"]      = "http://inspire.ec.europa.eu/schemas/base/3.3";    
    namespaces["gmlexr"]    = "http://www.opengis.net/gml/3.3/exr";    

    namespaceKuerzel[gml_3_2_targetNamespace]                              = "gml";
    namespaceKuerzel["http://www.isotc211.org/2005/gco"]                   = "gco";
    namespaceKuerzel["http://www.isotc211.org/2005/gmd"]                   = "gmd";
    namespaceKuerzel["http://inspire.ec.europa.eu/schemas/base/3.3"]       = "base";
    namespaceKuerzel["http://www.opengis.net/gml/3.3/exr"]                 = "gmlexr";
    break;

  case GML_SCHEMA_DEFAULT:
    break;

  }
}

GmlNamespaces::~GmlNamespaces()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Namespace hinzufügen                                                     //
///////////////////////////////////////////////////////////////////////////////
void GmlNamespaces::addNamespace ( string kuerzelP, string namespaceP )
{
    namespaces[kuerzelP]         = namespaceP;
    namespaceKuerzel[namespaceP] = kuerzelP;
}

///////////////////////////////////////////////////////////////////////////////
//  Schemadatei hinzufügen                                                   //
///////////////////////////////////////////////////////////////////////////////
void GmlNamespaces::addSchemadatei ( std::string namespaceP, std::string schemaDatei )
{
  schemaDateien.insert ( multimap<string,string>::value_type ( namespaceP, schemaDatei ) );
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Namespaces                                                  //
///////////////////////////////////////////////////////////////////////////////
size_t GmlNamespaces::getNamespaces ( vector<string> & vKuerzel, vector<string> & vNamespaces )
{
  map<string,string>::iterator iter;

  for ( iter = namespaces.begin(); iter != namespaces.end(); iter++ )
  {
    vKuerzel.push_back ( iter->first );
    vNamespaces.push_back ( iter->second );
  }

  return vKuerzel.size();
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert alle Schema-Dateien                                              //
///////////////////////////////////////////////////////////////////////////////
size_t GmlNamespaces::getSchemaDateien ( std::vector<std::string> & vNamespaces, std::vector<std::string> & vSchemaDateien )
{
  multimap<string,string>::iterator iter;

  for ( iter = schemaDateien.begin(); iter != schemaDateien.end(); iter++ )
  {
    vNamespaces.push_back ( iter->first );
    vSchemaDateien.push_back ( iter->second );
  }

  return schemaDateien.size();
}

///////////////////////////////////////////////////////////////////////////////
// Liefert das Kürzel eines Namespaces                                      //
///////////////////////////////////////////////////////////////////////////////
std::string GmlNamespaces::getKuerzel( string namespaceName )
{
  map<string,string>::iterator iter;

  iter = namespaceKuerzel.find( namespaceName );
  if ( iter != namespaceKuerzel.end() )
    return iter->second;
  else
    return ""; 
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Namespace zu einem Namespace-Kürzel                          //
///////////////////////////////////////////////////////////////////////////////
std::string GmlNamespaces::getNamespace ( std::string kuerzelName )
{
  map<string,string>::iterator iter;

  iter = namespaces.find( kuerzelName );
  if ( iter != namespaces.end() )
    return iter->second;
  else
    return ""; 
}

///////////////////////////////////////////////////////////////////////////////
//  Interpretiert die Namespaces zur Bestimmung des GML Schematyps           //
///////////////////////////////////////////////////////////////////////////////
GML_SCHEMA_TYPE GmlNamespaces::getSchemaType()
{
  map<string,string>::iterator iter;

  if ( namespaceKuerzel.find ( cityGml_0_4_0_targetNamespace ) != namespaceKuerzel.end() ||
    cityGml_0_4_0_targetNamespace == defaultNamespace )
    return CITYGML_0_4_0;

  if ( namespaceKuerzel.find ( cityGml_1_0_targetNamespace ) != namespaceKuerzel.end() ||
    cityGml_1_0_targetNamespace == defaultNamespace )
    return CITYGML_1_0;

  if ( namespaceKuerzel.find ( cityGml_2_0_targetNamespace ) != namespaceKuerzel.end()        ||
       namespaceKuerzel.find ( cityGml_2_0_buildingNamespace ) != namespaceKuerzel.end()      ||
       namespaceKuerzel.find ( cityGml_2_0_bridgeNamespace ) != namespaceKuerzel.end()        ||
       namespaceKuerzel.find ( cityGml_2_0_tunnelNamespace ) != namespaceKuerzel.end()        ||
       namespaceKuerzel.find ( cityGml_2_0_cityFurnitureNamespace ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( cityGml_2_0_genericsNamespace ) != namespaceKuerzel.end()      ||
       namespaceKuerzel.find ( cityGml_2_0_landUseNamespace ) != namespaceKuerzel.end()       ||
       namespaceKuerzel.find ( cityGml_2_0_reliefNamespace ) != namespaceKuerzel.end()        ||
       namespaceKuerzel.find ( cityGml_2_0_transportationNamespace ) != namespaceKuerzel.end()||
       namespaceKuerzel.find ( cityGml_2_0_vegetationNamespace ) != namespaceKuerzel.end()    ||
       namespaceKuerzel.find ( cityGml_2_0_waterBodyNamespace ) != namespaceKuerzel.end()     ||
       cityGml_2_0_targetNamespace == defaultNamespace                                        ||
       cityGml_2_0_buildingNamespace == defaultNamespace                                      ||
       cityGml_2_0_bridgeNamespace == defaultNamespace                                        ||
       cityGml_2_0_tunnelNamespace == defaultNamespace                                        ||
       cityGml_2_0_cityFurnitureNamespace == defaultNamespace                                 ||
       cityGml_2_0_genericsNamespace == defaultNamespace                                      ||
       cityGml_2_0_landUseNamespace == defaultNamespace                                       ||
       cityGml_2_0_reliefNamespace == defaultNamespace                                        ||
       cityGml_2_0_transportationNamespace == defaultNamespace                                ||
       cityGml_2_0_vegetationNamespace == defaultNamespace                                    ||
       cityGml_2_0_waterBodyNamespace == defaultNamespace          
    )
    return CITYGML_2_0;

  if ( namespaceKuerzel.find ( cityGml_3_0_targetNamespace ) != namespaceKuerzel.end() ||
    cityGml_3_0_targetNamespace == defaultNamespace )
    return CITYGML_3_0;

  if ( namespaceKuerzel.find ( xplanGml_2_0_targetNamespace ) != namespaceKuerzel.end() ||
      xplanGml_2_0_targetNamespace == defaultNamespace )
      return  XPLANGML_2_0;

  if ( namespaceKuerzel.find ( xplanGml_3_0_targetNamespace ) != namespaceKuerzel.end() ||
      xplanGml_3_0_targetNamespace == defaultNamespace )
      return  XPLANGML_3_0;

  if ( namespaceKuerzel.find ( xplanGml_4_0_targetNamespace ) != namespaceKuerzel.end() ||
    xplanGml_4_0_targetNamespace == defaultNamespace )
    return  XPLANGML_4_0;

  if ( namespaceKuerzel.find ( xplanGml_4_1_targetNamespace ) != namespaceKuerzel.end() ||
    xplanGml_4_1_targetNamespace == defaultNamespace )
    return  XPLANGML_4_1;

  if ( namespaceKuerzel.find ( xplanGml_5_0_targetNamespace ) != namespaceKuerzel.end() ||
    xplanGml_5_0_targetNamespace == defaultNamespace )
    return  XPLANGML_5_0;

  if ( namespaceKuerzel.find ( xplanGml_5_1_targetNamespace ) != namespaceKuerzel.end() ||
    xplanGml_5_1_targetNamespace == defaultNamespace )
    return  XPLANGML_5_1;

  if ( namespaceKuerzel.find ( xplanGml_5_2_targetNamespace ) != namespaceKuerzel.end() ||
    xplanGml_5_2_targetNamespace == defaultNamespace )
    return  XPLANGML_5_2;

  if ( namespaceKuerzel.find ( xplanGml_5_3_targetNamespace ) != namespaceKuerzel.end() ||
    xplanGml_5_3_targetNamespace == defaultNamespace )
    return  XPLANGML_5_3;

  if ( namespaceKuerzel.find ( nas_5_1_targetNamespace ) != namespaceKuerzel.end() ||
    nas_5_1_targetNamespace == defaultNamespace )
    return NAS_5_1;

  if ( namespaceKuerzel.find ( nas_5_1_1_targetNamespace ) != namespaceKuerzel.end() ||
       nas_5_1_1_targetNamespace == defaultNamespace )
    return NAS_5_1_1;

  if ( namespaceKuerzel.find ( nas_6_0_targetNamespace ) != namespaceKuerzel.end() ||
    nas_6_0_targetNamespace == defaultNamespace )
    return NAS_6_0;

  if ( namespaceKuerzel.find ( INSPIRE_PLU_4_0_targetNamespace ) != namespaceKuerzel.end() ||
    INSPIRE_PLU_4_0_targetNamespace == defaultNamespace )
    return  INSPIRE_LU_PLANNED;

  if ( isINSPIRENamespace() )
    return  INSPIRE;

  if ( namespaceKuerzel.find ( boreholeML_3_0_targetNamespace ) != namespaceKuerzel.end()       ||
       namespaceKuerzel.find ( boreholeHeaderML_3_0_targetNamespace ) != namespaceKuerzel.end() ||
       boreholeML_3_0_targetNamespace == defaultNamespace                                       ||
       boreholeHeaderML_3_0_targetNamespace == defaultNamespace )
    return  BOREHOLEML_3_0;

  return GML_SCHEMA_UNKNOWN;
}

//////////////////////////////////////////////////////////////////////////////
//  Liefert true, wenn in der map namespaceKuerzel ein INSPIRE namespace    //
//  vorhanden ist                                                           //
//////////////////////////////////////////////////////////////////////////////
bool GmlNamespaces::isINSPIRENamespace()
{
  if ( namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/base/3.3" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/act-core/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/ad/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/au/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/af/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/tn-a/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/am/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/base2/2.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/br/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/bu-base/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/bu-core2d/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/bu-core3d/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/tn-c/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/cp/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/tn/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/cvbase/1.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/us-net-el/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/el-bas/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/el-cov/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/el-tin/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/el-vec/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/er-b/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/er-c/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/er-v/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/us-emf/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/ef/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/elu/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/gn/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/ge-core/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/ge_gp/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/us-govserv/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/gelu/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/hb/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/hh/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/hy/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/ge_hg/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/hy-n/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/hy-p/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/lcn/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/lcr/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/lcv/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/lunom/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/mr-core/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/nz-core/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/net/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/omop/3.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/omor/3.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/us-net-ogc/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/oi/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/plu/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/pd/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/ompr/3.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/pf/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/ps/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/tn-ra/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/tn-ro/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/selu/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/sr/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/us-net-sw/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/so/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/omso/3.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/sd/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/su-core/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/su-grid/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/su-vector/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/us-net-tc/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/us-net-th/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/us-net-common/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/us-net-wa/4.0" ) != namespaceKuerzel.end() ||
       namespaceKuerzel.find ( "http://inspire.ec.europa.eu/schemas/tn-w/4.0" ) != namespaceKuerzel.end() )
       return true;

  if (  "http://inspire.ec.europa.eu/schemas/base/3.3" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/act-core/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/ad/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/au/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/af/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/tn-a/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/am/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/base2/2.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/br/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/bu-base/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/bu-core2d/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/bu-core3d/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/tn-c/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/cp/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/tn/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/cvbase/1.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/us-net-el/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/el-bas/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/el-cov/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/el-tin/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/el-vec/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/er-b/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/er-c/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/er-v/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/us-emf/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/ef/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/elu/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/gn/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/ge-core/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/ge_gp/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/us-govserv/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/gelu/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/hb/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/hh/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/hy/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/ge_hg/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/hy-n/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/hy-p/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/lcn/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/lcr/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/lcv/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/lunom/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/mr-core/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/nz-core/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/net/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/omop/3.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/omor/3.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/us-net-ogc/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/oi/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/plu/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/pd/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/ompr/3.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/pf/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/ps/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/tn-ra/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/tn-ro/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/selu/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/sr/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/us-net-sw/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/so/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/omso/3.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/sd/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/su-core/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/su-grid/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/su-vector/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/us-net-tc/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/us-net-th/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/us-net-common/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/us-net-wa/4.0" == defaultNamespace ||
     "http://inspire.ec.europa.eu/schemas/tn-w/4.0" == defaultNamespace )
    return true;

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//  Liefert den Folder-Namen, unter dem die Codelisten eines CityGML 2.0     //
//  Moduls zu finden sind                                                    //
///////////////////////////////////////////////////////////////////////////////
string GmlNamespaces::getCityGML20CodelistModuleFolder ( string namespaceKuerzel )
{
  string folderName;

  if ( namespaceKuerzel == "core" )
    folderName = "/core/2.0/";
  else
  if ( namespaceKuerzel == "bldg" )
    folderName = "/building/2.0/";
  else
  if ( namespaceKuerzel == "trpt" )
    folderName = "/transportation/2.0/";
  else
  if ( namespaceKuerzel == "veget" )
    folderName = "/vegetation/2.0/";
  else
  if ( namespaceKuerzel == "water" )
    folderName = "/waterbody/2.0/";
  else
  if ( namespaceKuerzel == "furnit" )
    folderName = "/cityfurniture/2.0/";
  else
  if ( namespaceKuerzel == "landuse" )
    folderName = "/landuse/2.0/";
  else
  if ( namespaceKuerzel == "group" )
    folderName = "/cityobjectgroup/2.0/";
  else
  if ( namespaceKuerzel == "app" )
    folderName = "/appearance/2.0/";
  else
    folderName = "/";

  return folderName;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert den Codelist-Namen eines CityGML 2.0 Codelist Attributs          //
///////////////////////////////////////////////////////////////////////////////
string GmlNamespaces::getCityGML20CodelistName ( string klassenNameQualified, string attributNameQualified )
{
  string codelistName;

  if ( attributNameQualified == "app:mimeType" )
    return  "_Texture_mimeType";

  if ( klassenNameQualified == "bldg:BuildingType"       ||
       klassenNameQualified == "bldg:BuildingPartType"   ||
       klassenNameQualified == "bldg:AbstractBuildingType")
  {
    if ( attributNameQualified == "bldg:class" )
      codelistName = "_AbstractBuilding_class";
    else
    if ( attributNameQualified == "bldg:function" )
      codelistName = "_AbstractBuilding_function";
    else
    if ( attributNameQualified == "bldg:usage" )
      codelistName = "_AbstractBuilding_usage";
    else
    if ( attributNameQualified == "bldg:roofType" )
      codelistName = "_AbstractBuilding_roofType";
    
    return codelistName;
  }

  if ( klassenNameQualified == "bldg:BuildingInstallationType")
  {
    if ( attributNameQualified == "bldg:class" )
      codelistName = "BuildingInstallation_class";
    else
    if ( attributNameQualified == "bldg:function" )
      codelistName = "BuildingInstallation_function";
    else
    if ( attributNameQualified == "bldg:usage" )
      codelistName = "BuildingInstallation_usage";

    return codelistName;
  }

  if ( klassenNameQualified == "bldg:IntBuildingInstallationType")
  {
    if ( attributNameQualified == "bldg:class" )
      codelistName = "IntBuildingInstallation_class";
    else
    if ( attributNameQualified == "bldg:function" )
      codelistName = "IntBuildingInstallation_function";
    else
    if ( attributNameQualified == "bldg:usage" )
      codelistName = "IntBuildingInstallation_usage";

    return codelistName;
  }

  if ( klassenNameQualified == "bldg:RoomType")
  {
    if ( attributNameQualified == "bldg:class" )
      codelistName = "Room_class";
    else
    if ( attributNameQualified == "bldg:function" )
      codelistName = "Room_function";
    else
    if ( attributNameQualified == "bldg:usage" )
      codelistName = "Room_usage";

    return codelistName;
  }

  if ( klassenNameQualified == "bldg:BuildingFurnitureType")
  {
    if ( attributNameQualified == "bldg:class" )
      codelistName = "BuildingFurniture_class";
    else
    if ( attributNameQualified == "bldg:function" )
      codelistName = "BuildingFurniture_function";
    else
    if ( attributNameQualified == "bldg:usage" )
      codelistName = "BuildingFurniture_usage";

    return codelistName;
  }

  if ( klassenNameQualified == "brid:AbstractBridgeType" || 
       klassenNameQualified == "brid:BridgeType"         ||
       klassenNameQualified == "brid:BridgePartType")
  {
    if ( attributNameQualified == "brid:class" )
      codelistName = "_AbstractBridge_class";
    else
    if ( attributNameQualified == "brid:function" )
      codelistName = "_AbstractBridge_function";
    else
    if ( attributNameQualified == "brid:usage" )
      codelistName = "_AbstractBridge_usage";

    return codelistName;
  }

  if ( klassenNameQualified == "furnit:CityFurnitureType")
  {
    if ( attributNameQualified == "furnit:class" )
      codelistName = "CityFurniture_class";
    else
    if ( attributNameQualified == "furnit:function" )
      codelistName = "cityFurniture_function";
    else
    if ( attributNameQualified == "furnit:usage" )
      codelistName = "CityFurniture_usage";

    return codelistName;
  }
  
  if ( klassenNameQualified == "group:CityObjectGroupType")
  {
    if ( attributNameQualified == "group:class" )
      codelistName = "CityObjectGroup_class";
    else
    if ( attributNameQualified == "group:function" )
      codelistName = "CityObjectGroup_function";
    else
    if ( attributNameQualified == "group:usage" )
      codelistName = "CityObjectGroup_usage";

    return codelistName;
  }
  
  if ( klassenNameQualified == "core:ImplicitGeometryType")
  {
    if ( attributNameQualified == "core:mimeType" )
      return "ImplicitGeometry_mimeType";
  }
  
  if ( klassenNameQualified == "landuse:LandUseType")
  {
    if ( attributNameQualified == "landuse:class" )
      codelistName = "LandUse_class";
    else
      if ( attributNameQualified == "landuse:function" )
        codelistName = "LandUse_function";
      else
        if ( attributNameQualified == "landuse:usage" )
          codelistName = "LandUse_usage";

    return codelistName;
  }
    
  if ( klassenNameQualified == "trpt:TransportationComplexType")
  {
    if ( attributNameQualified == "trpt:class" )
      codelistName = "TransportationComplex_class";
    else
    if ( attributNameQualified == "trpt:function" )
      codelistName = "TransportationComplex_function";
    else
    if ( attributNameQualified == "trpt:usage" )
      codelistName = "TransportationComplex_usage";

    return codelistName;
  }
    
  if ( klassenNameQualified == "trpt:TrafficAreaType")
  {
    if ( attributNameQualified == "trpt:class" )
      codelistName = "TrafficArea_class";
    else
    if ( attributNameQualified == "trpt:function" )
      codelistName = "TrafficArea_function";
    else
    if ( attributNameQualified == "trpt:usage" )
      codelistName = "TrafficArea_usage";

    return codelistName;
  }
    
  if ( klassenNameQualified == "trpt:AuxiliaryTrafficAreaType")
  {
    if ( attributNameQualified == "trpt:class" )
      codelistName = "AuxiliaryTrafficArea_class";
    else
    if ( attributNameQualified == "trpt:function" )
      codelistName = "AuxiliaryTrafficArea_function";
    else
    if ( attributNameQualified == "trpt:usage" )
      codelistName = "AuxiliaryTrafficArea_usage";

    return codelistName;
  }
    
  if ( klassenNameQualified == "tun:AbstractTunnelType" ||
       klassenNameQualified == "tun:TunnelType"         || 
       klassenNameQualified == "tun:TunnelPartType"         )
  {
    if ( attributNameQualified == "tun:class" )
      codelistName = "_AbstractTunnel_class";
    else
    if ( attributNameQualified == "tun:function" )
      codelistName = "_AbstractTunnel_function";
    else
    if ( attributNameQualified == "tun:usage" )
      codelistName = "_AbstractTunnel_usage";

    return codelistName;
  }

  if ( klassenNameQualified == "veget:PlantCoverType")
  {
    if ( attributNameQualified == "veget:class" )
      codelistName = "PlantCover_class";
    else
    if ( attributNameQualified == "veget:function" )
      codelistName = "PlantCover_function";
    else
    if ( attributNameQualified == "veget:usage" )
      codelistName = "PlantCover_usage";

    return codelistName;
  }

  if ( klassenNameQualified == "veget:SolitaryVegetationObjectType")
  {
    if ( attributNameQualified == "veget:class" )
      codelistName = "SolitaryVegetationObject_class";
    else
    if ( attributNameQualified == "veget:function" )
      codelistName = "SolitaryVegetationObject_function";
    else
    if ( attributNameQualified == "veget:usage" )
      codelistName = "SolitaryVegetationObject_usage";
    else
    if ( attributNameQualified == "veget:species" )
      codelistName = "SolitaryVegetationObject_species";

    return codelistName;
  }

  if ( klassenNameQualified == "water:WaterBodyType")
  {
    if ( attributNameQualified == "water:class" )
      codelistName = "WaterBody_class";
    else
    if ( attributNameQualified == "water:function" )
      codelistName = "WaterBody_function";
    else
    if ( attributNameQualified == "water:usage" )
      codelistName = "WaterBody_usage";

    return codelistName;
  }

  if ( klassenNameQualified == "water:WaterSurfaceType")
  {
    if ( attributNameQualified == "water:waterLevel" )
      return "WaterSurface_waterLevel";
  }

  return "Unknown";
}


///////////////////////////////////////////////////////////////////////////////
//  SchemaTypeNames                                                          //
//  Wichtige XML-Tags der eingelesenen XML-Schemadateien                     //
///////////////////////////////////////////////////////////////////////////////
SchemaTypeNames::SchemaTypeNames ( GmlNamespaces * pNamespaceP )
{
  string gmlKuerzel;
  string gmlCoverageKuerzel;
  string xmlSchemaKuerzel;
  string gcoKuerzel;
  string genericAttributeKuerzel;
  string reliefKuerzel;
  string addressKuerzel;
  string addressPropKuerzel;
  string appearanceKuerzel;
  string baseKuerzel;
  string cguKuerzel;
  string gsmlKuerzel;

  pNamespace     = pNamespaceP;
  int schemaType = pNamespace->getSchemaType();

  gmlKuerzel         = getGmlKuerzel();
  gmlCoverageKuerzel = getGmlCoverageKuerzel();
  xmlSchemaKuerzel   = getXmlSchemaKuerzel();
//  gcoKuerzel         = getGcoKuerzel();

  switch ( schemaType )
  {
  case CITYGML_0_4_0:
    rootName                = "citygml:CityModel";
    featureMemberName       = "citygml:cityObjectMember";
    gmlKuerzel              = pNamespace->getKuerzel ( GmlNamespaces::gml_3_0_targetNamespace );
    baseKuerzel             = pNamespace->getKuerzel( GmlNamespaces::cityGml_0_4_0_targetNamespace );
    reliefKuerzel           = pNamespace->getKuerzel( GmlNamespaces::cityGml_0_4_0_targetNamespace );
    addressKuerzel          = pNamespace->getKuerzel ( GmlNamespaces::xAL_2_0_targetNamespace );
    addressPropKuerzel      = pNamespace->getKuerzel ( GmlNamespaces::cityGml_0_4_0_targetNamespace );
    genericAttributeKuerzel = pNamespace->getKuerzel( GmlNamespaces::cityGml_0_4_0_targetNamespace );
    appearanceKuerzel       = pNamespace->getKuerzel( GmlNamespaces::cityGml_0_4_0_targetNamespace );
    genericStringAttribute  = "stringAttribute";
    genericDoubleAttribute  = "doubleAttribute";
    genericIntAttribute     = "intAttribute";
    genericDateAttribute    = "dateAttribute";
    genericUrlAttribute     = "uriAttribute";
    genericNameAttribute    = "name";
    genericValueAttribute   = "value";
    externalReferenceTag    = "citygml:externalReference";
    informationSystemTag    = "citygml:informationSystem";
    externalObjectTag       = "citygml:externalObject";
    externalObjectNameTag   = "citygml:name";
    externalObjectURITag    = "citygml:uri";
    break;

  case CITYGML_1_0:
    rootName                = "core:CityModel";
    featureMemberName       = "core:cityObjectMember";
    gmlKuerzel               = pNamespace->getKuerzel ( GmlNamespaces::gml_3_0_targetNamespace );
    baseKuerzel              = pNamespace->getKuerzel ( GmlNamespaces::cityGml_1_0_targetNamespace );
    reliefKuerzel            = pNamespace->getKuerzel ( "http://www.opengis.net/citygml/relief/1.0" );
    addressKuerzel           = pNamespace->getKuerzel ( GmlNamespaces::xAL_2_0_targetNamespace );
    addressPropKuerzel       = pNamespace->getKuerzel ( GmlNamespaces::cityGml_1_0_targetNamespace );
    genericAttributeKuerzel  = pNamespace->getKuerzel ( "http://www.opengis.net/citygml/generics/1.0" );
    appearanceKuerzel        = pNamespace->getKuerzel ( "http://www.opengis.net/citygml/appearance/1.0" );
    genericStringAttribute  = "stringAttribute";
    genericDoubleAttribute  = "doubleAttribute";
    genericIntAttribute     = "intAttribute";
    genericDateAttribute    = "dateAttribute";
    genericUrlAttribute     = "uriAttribute";
    genericNameAttribute    = "name";
    genericValueAttribute   = "value";
    externalReferenceTag    = "core:externalReference";
    informationSystemTag    = "core:informationSystem";
    externalObjectTag       = "core:externalObject";
    externalObjectNameTag   = "core:name";
    externalObjectURITag    = "core:uri";
    break;

  case CITYGML_2_0:
    rootName                = "core:CityModel";
    featureMemberName       = "core:cityObjectMember";
    gmlKuerzel              = pNamespace->getKuerzel ( GmlNamespaces::gml_3_0_targetNamespace );
    baseKuerzel             = pNamespace->getKuerzel ( GmlNamespaces::cityGml_2_0_targetNamespace );
    reliefKuerzel           = pNamespace->getKuerzel ( "http://www.opengis.net/citygml/relief/2.0" );
    addressKuerzel          = pNamespace->getKuerzel ( GmlNamespaces::xAL_2_0_targetNamespace );
    addressPropKuerzel      = pNamespace->getKuerzel ( GmlNamespaces::cityGml_2_0_targetNamespace );
    genericAttributeKuerzel = pNamespace->getKuerzel ( "http://www.opengis.net/citygml/generics/2.0" );
    appearanceKuerzel       = pNamespace->getKuerzel ( "http://www.opengis.net/citygml/appearance/2.0" );
    genericStringAttribute  = "stringAttribute";
    genericDoubleAttribute  = "doubleAttribute";
    genericIntAttribute     = "intAttribute";
    genericDateAttribute    = "dateAttribute";
    genericUrlAttribute     = "uriAttribute";
    genericMeasureAttribute = "measureAttribute";
    genericAttributeSet     = "genericAttributeSet";
    genericNameAttribute    = "name";
    genericValueAttribute   = "value";
    externalReferenceTag    = "core:externalReference";
    informationSystemTag    = "core:informationSystem";
    externalObjectTag       = "core:externalObject";
    externalObjectNameTag   = "core:name";
    externalObjectURITag    = "core:uri";
    break;


  case CITYGML_3_0:
    rootName                = "core:CityModel";
    featureMemberName       = "core:cityObjectMember";
    gmlKuerzel              = pNamespace->getKuerzel ( GmlNamespaces::gml_3_2_targetNamespace );
    baseKuerzel             = pNamespace->getKuerzel ( GmlNamespaces::cityGml_3_0_targetNamespace );
    reliefKuerzel           = pNamespace->getKuerzel ( "http://www.opengis.net/citygml/relief/3.0" );
    addressKuerzel          = pNamespace->getKuerzel ( GmlNamespaces::xAL_2_0_targetNamespace );
    addressPropKuerzel      = pNamespace->getKuerzel ( GmlNamespaces::cityGml_3_0_targetNamespace );
    genericAttributeKuerzel = pNamespace->getKuerzel ( "http://www.opengis.net/citygml/generics/3.0" );
    appearanceKuerzel       = pNamespace->getKuerzel ( "http://www.opengis.net/citygml/appearance/3.0" );
    genericStringAttribute  = "stringAttribute";
    genericDoubleAttribute  = "doubleAttribute";
    genericIntAttribute     = "intAttribute";
    genericDateAttribute    = "dateAttribute";
    genericUrlAttribute     = "uriAttribute";
    genericMeasureAttribute = "measureAttribute";
    genericAttributeSet     = "genericAttributeSet";
    genericNameAttribute    = "name";
    genericValueAttribute   = "value";
    externalReferenceTag    = "core:externalReference";
    informationSystemTag    = "core:informationSystem";
    externalObjectTag       = "core:externalObject";
    externalObjectNameTag   = "core:name";
    externalObjectURITag    = "core:uri";
    break;

  case NAS_5_1:
  case NAS_5_1_1:
    rootName                = "AX_Bestandsdatenauszug";
    featureMemberName       = "gml:featureMember";
    gmlKuerzel              = pNamespace->getKuerzel ( GmlNamespaces::gml_3_0_targetNamespace );
    break;

  case NAS_6_0:
    rootName                = "AX_Bestandsdatenauszug";
    featureMemberName       = "gml:featureMember";
    gmlKuerzel              = pNamespace->getKuerzel ( GmlNamespaces::gml_3_2_targetNamespace );
    break;

  case  XPLANGML_2_0:
    rootName                = "xplan:XPlanAuszug";
    featureMemberName       = "gml:featureMember";
    genericAttributeKuerzel = pNamespace->getKuerzel ( GmlNamespaces::xplanGml_2_0_targetNamespace );
    genericStringAttribute  = "XP_StringAttribut";
    genericDoubleAttribute  = "XP_DoubleAttribut";
    genericIntAttribute     = "XP_IntegerAttribut";
    genericDateAttribute    = "XP_DatumAttribut";
    genericUrlAttribute     = "XP_URLAttribut";
    genericNameAttribute    = "name";
    genericValueAttribute   = "wert";
    break;

  case  XPLANGML_3_0:
    rootName                = "xplan:XPlanAuszug";
    featureMemberName       = "gml:featureMember";
    genericAttributeKuerzel = pNamespace->getKuerzel ( GmlNamespaces::xplanGml_3_0_targetNamespace );
    genericStringAttribute  = "XP_StringAttribut";
    genericDoubleAttribute  = "XP_DoubleAttribut";
    genericIntAttribute     = "XP_IntegerAttribut";
    genericDateAttribute    = "XP_DatumAttribut";
    genericUrlAttribute     = "XP_URLAttribut";
    genericNameAttribute    = "name";
    genericValueAttribute   = "wert";
    break;

  case  XPLANGML_4_0:
    rootName                = "xplan:XPlanAuszug";
    featureMemberName       = "gml:featureMember";
    genericAttributeKuerzel = pNamespace->getKuerzel ( GmlNamespaces::xplanGml_4_0_targetNamespace );
    genericStringAttribute  = "XP_StringAttribut";
    genericDoubleAttribute  = "XP_DoubleAttribut";
    genericIntAttribute     = "XP_IntegerAttribut";
    genericDateAttribute    = "XP_DatumAttribut";
    genericUrlAttribute     = "XP_URLAttribut";
    genericNameAttribute    = "name";
    genericValueAttribute   = "wert";
    break;

  case  XPLANGML_4_1:
    rootName                = "xplan:XPlanAuszug";
    featureMemberName       = "gml:featureMember";
    genericAttributeKuerzel = pNamespace->getKuerzel ( GmlNamespaces::xplanGml_4_1_targetNamespace );
    genericStringAttribute  = "XP_StringAttribut";
    genericDoubleAttribute  = "XP_DoubleAttribut";
    genericIntAttribute     = "XP_IntegerAttribut";
    genericDateAttribute    = "XP_DatumAttribut";
    genericUrlAttribute     = "XP_URLAttribut";
    genericNameAttribute    = "name";
    genericValueAttribute   = "wert";
    break;

  case  XPLANGML_5_0:
    rootName                = "xplan:XPlanAuszug";
    featureMemberName       = "gml:featureMember";
    genericAttributeKuerzel = pNamespace->getKuerzel ( GmlNamespaces::xplanGml_5_0_targetNamespace );
    genericStringAttribute  = "XP_StringAttribut";
    genericDoubleAttribute  = "XP_DoubleAttribut";
    genericIntAttribute     = "XP_IntegerAttribut";
    genericDateAttribute    = "XP_DatumAttribut";
    genericUrlAttribute     = "XP_URLAttribut";
    genericNameAttribute    = "name";
    genericValueAttribute   = "wert";
    break;

  case  XPLANGML_5_1:
    rootName                = "xplan:XPlanAuszug";
    featureMemberName       = "gml:featureMember";
    genericAttributeKuerzel = pNamespace->getKuerzel ( GmlNamespaces::xplanGml_5_1_targetNamespace );
    genericStringAttribute  = "XP_StringAttribut";
    genericDoubleAttribute  = "XP_DoubleAttribut";
    genericIntAttribute     = "XP_IntegerAttribut";
    genericDateAttribute    = "XP_DatumAttribut";
    genericUrlAttribute     = "XP_URLAttribut";
    genericNameAttribute    = "name";
    genericValueAttribute   = "wert";
    break;

  case  XPLANGML_5_2:
    rootName                = "xplan:XPlanAuszug";
    featureMemberName       = "gml:featureMember";
    genericAttributeKuerzel = pNamespace->getKuerzel ( GmlNamespaces::xplanGml_5_2_targetNamespace );
    genericStringAttribute  = "XP_StringAttribut";
    genericDoubleAttribute  = "XP_DoubleAttribut";
    genericIntAttribute     = "XP_IntegerAttribut";
    genericDateAttribute    = "XP_DatumAttribut";
    genericUrlAttribute     = "XP_URLAttribut";
    genericNameAttribute    = "name";
    genericValueAttribute   = "wert";
    break;

  case  XPLANGML_5_3:
    rootName                = "xplan:XPlanAuszug";
    featureMemberName       = "gml:featureMember";
    genericAttributeKuerzel = pNamespace->getKuerzel ( GmlNamespaces::xplanGml_5_3_targetNamespace );
    genericStringAttribute  = "XP_StringAttribut";
    genericDoubleAttribute  = "XP_DoubleAttribut";
    genericIntAttribute     = "XP_IntegerAttribut";
    genericDateAttribute    = "XP_DatumAttribut";
    genericUrlAttribute     = "XP_URLAttribut";
    genericNameAttribute    = "name";
    genericValueAttribute   = "wert";
    break;

  case INSPIRE_LU_PLANNED:
    featureMemberName       = "gml:featureMember";
    spatialPlan                = pNamespace->getKuerzel( GmlNamespaces::INSPIRE_PLU_4_0_targetNamespace ) + ":SpatialPlanType";
    zoningElementType          = pNamespace->getKuerzel( GmlNamespaces::INSPIRE_PLU_4_0_targetNamespace ) + ":ZoningElementType" ;
    break;

  case INSPIRE:
    rootName          = "gml:FeatureCollection";
    featureMemberName = "gml:featureMember";
    break;

  case BOREHOLEML_3_0:
    rootName          = "gml:FeatureCollection";
    featureMemberName = "gml:featureMember";
    break;

  case GML_SCHEMA_DEFAULT:
    break;

  }

  gmlAbstractObject            = gmlKuerzel + ":AbstractObject";
  gmlAbstractGML               = gmlKuerzel + ":AbstractGMLType";
  gmlAbstractFeature           = gmlKuerzel + ":AbstractFeatureType";
  gmlAbstractFeatureCollection = gmlKuerzel + ":AbstractFeatureCollectionType";
  gmlName                      = gmlKuerzel + ":name";
  gmlDescription               = gmlKuerzel + ":description";
  gmlIdentifier                = gmlKuerzel + ":identifier";
  gmlBoundedBy                 = gmlKuerzel + ":boundedBy";
  gmlAngleType                 = gmlKuerzel + ":AngleType";
  gmlLengthType                = gmlKuerzel + ":LengthType";
  gmlAreaType                  = gmlKuerzel + ":AreaType";
  gmlVolumeType                = gmlKuerzel + ":VolumeType";
  gmlQuantityExtentType        = gmlKuerzel + ":QuantityExtentType";
  gmlAssociationType           = gmlKuerzel + ":AssociationType";
  gmlReferenceType             = gmlKuerzel + ":ReferenceType";
  gmlCodeType                  = gmlKuerzel + ":CodeType";
  gmlFeaturePropertyType       = gmlKuerzel + ":FeaturePropertyType";
  gmlAbstractGeometricPrimitiveType = gmlKuerzel + ":AbstractGeometricPrimitiveType";
  gmlGeometryPropertyType           = gmlKuerzel + ":GeometryPropertyType";
  gmlGeometricPrimitivePropertyType = gmlKuerzel + ":GeometricPrimitivePropertyType";
  gmlGeometricComplexPropertyType = gmlKuerzel + ":GeometricComplexPropertyType";
  gmlMultiCurvePropertyType    = gmlKuerzel + ":MultiCurvePropertyType";
  gmlCurvePropertyType         = gmlKuerzel + ":CurvePropertyType";
  gmlMultiPointPropertyType    = gmlKuerzel + ":MultiPointPropertyType";
  gmlPointPropertyType         = gmlKuerzel + ":PointPropertyType";
  gmlMultiSurfacePropertyType  = gmlKuerzel + ":MultiSurfacePropertyType";
  gmlSurfacePropertyType       = gmlKuerzel + ":SurfacePropertyType";
  gmlSolidPropertyType         = gmlKuerzel + ":SolidPropertyType";
  gmlMultiSolidPropertyType    = gmlKuerzel + ":MultiSolidPropertyType";
  gmlGeometryPropertyType      = gmlKuerzel + ":GeometryPropertyType";
  gmlMeasureOrNullListType     = gmlKuerzel + ":MeasureOrNullListType";
  gmlMeasureListType           = gmlKuerzel + ":MeasureListType";
  gmlMeasureType               = gmlKuerzel + ":MeasureType";
  gmlSpeedType                 = gmlKuerzel + ":SpeedType";
  gmlIntegerListType           = gmlKuerzel + ":integerList";
  gmlCodeWithAuthorityType     = gmlKuerzel + ":CodeWithAuthorityType";
  gmlDirectPositionType        = gmlKuerzel + ":DirectPositionType";
  gmlCalDate                   = gmlKuerzel + ":CalDate";
  gmlTimePositionType          = gmlKuerzel + ":TimePositionType";
  gmlTimePeriodType            = gmlKuerzel + ":TimePeriodPropertyType";
  gmlTimeIntervalType          = gmlKuerzel + ":TimeIntervalLengthType";
  gmlScaleType                 = gmlKuerzel + ":ScaleType";

  gmlAbstractDiscreteCoverageType  = gmlCoverageKuerzel + ":AbstractDiscreteCoverageType";

  if ( xmlSchemaKuerzel != "" )
    xmlSchemaKuerzel = xmlSchemaKuerzel + ":";

  xmlStringType                = xmlSchemaKuerzel + "string";
  xmlBooleanType               = xmlSchemaKuerzel + "boolean";
  xmlDoubleType                = xmlSchemaKuerzel + "double";
  xmlFloatType                 = xmlSchemaKuerzel + "float";
  xmlDecimalType               = xmlSchemaKuerzel + "decimal";
  xmlAngleType                 = xmlSchemaKuerzel + "angle";
  xmlIntegerType               = xmlSchemaKuerzel + "integer";
  xmlIntType                   = xmlSchemaKuerzel + "int";
  xmlLongType                  = xmlSchemaKuerzel + "long";
  xmlShortType                 = xmlSchemaKuerzel + "short";
  xmlByteType                  = xmlSchemaKuerzel + "byte";
  xmlUnsignedIntType           = xmlSchemaKuerzel + "usignedInt";
  xmlUnsignedLongType          = xmlSchemaKuerzel + "unsignedLong";
  xmlUnsignedShortType         = xmlSchemaKuerzel + "unsignedShort";
  xmlUnsignedByteType          = xmlSchemaKuerzel + "unsignedByte";
  xmlNonNegativeIntegerType    = xmlSchemaKuerzel + "nonNegativeInteger";
  xmlNonPositiveIntegerType    = xmlSchemaKuerzel + "nonPositiveInteger";
  xmlNegativeIntegerType       = xmlSchemaKuerzel + "negativeInteger";
  xmlPositiveIntegerType       = xmlSchemaKuerzel + "positiveInteger";
  xmlAnyUriType                = xmlSchemaKuerzel + "anyURI";
  xmlDateType                  = xmlSchemaKuerzel + "date";
  xmlTimeType                  = xmlSchemaKuerzel + "time";
  xmlDateTimeType              = xmlSchemaKuerzel + "dateTime";
  xmlDurationType              = xmlSchemaKuerzel + "duration";
  xmlGYearType                 = xmlSchemaKuerzel + "gYear";
  xmlGMonthType                = xmlSchemaKuerzel + "gMonth";
  xmlGDayType                  = xmlSchemaKuerzel + "gDay";
  xmlGYearMonthType            = xmlSchemaKuerzel + "gYearMonth";
  xmlGMonthDayType             = xmlSchemaKuerzel + "gMonthDay";  

  xalStreetName          = addressKuerzel + ":streetName";
  xalCity                = addressKuerzel + ":city";
  xalZipCode             = addressKuerzel + ":zipCode";
  xalHouseNumber         = addressKuerzel + ":houseNumber";
  xalCountryName         = addressKuerzel + ":CountryName";
  xalLocalityName        = addressKuerzel + ":LocalityName";
  xalThoroughfareNumber  = addressKuerzel + ":ThoroughfareNumber";
  xalThoroughfareName    = addressKuerzel + ":ThoroughfareName";
  xalPostalCodeNumber    = addressKuerzel + ":PostalCodeNumber";

  if ( genericAttributeKuerzel != "" )
  {
    if ( schemaType == CITYGML_0_4_0 )
      genericAttribute = pNamespace->getKuerzel ( GmlNamespaces::cityGml_0_4_0_targetNamespace ) + 
      ":_GenericApplicationPropertyOfCityObject";
    else
    if ( schemaType == CITYGML_1_0   )
      genericAttribute        = pNamespace->getKuerzel ( GmlNamespaces::cityGml_1_0_targetNamespace ) + 
      ":_GenericApplicationPropertyOfCityObject";
    else
    if ( schemaType == CITYGML_2_0 )
      genericAttribute        = pNamespace->getKuerzel ( GmlNamespaces::cityGml_2_0_targetNamespace ) + 
      ":_GenericApplicationPropertyOfCityObject";
    else
    if ( schemaType == CITYGML_3_0 )
      genericAttribute        = pNamespace->getKuerzel ( GmlNamespaces::cityGml_3_0_targetNamespace ) + 
      ":_GenericApplicationPropertyOfCityObject";
    else
      genericAttribute        = genericAttributeKuerzel + ":_genericAttribute";

    genericStringAttribute  = genericAttributeKuerzel + ":" + genericStringAttribute;
    genericDoubleAttribute  = genericAttributeKuerzel + ":" + genericDoubleAttribute;
    genericIntAttribute     = genericAttributeKuerzel + ":" + genericIntAttribute;
    genericDateAttribute    = genericAttributeKuerzel + ":" + genericDateAttribute;
    genericUrlAttribute     = genericAttributeKuerzel + ":" + genericUrlAttribute;
    genericValueAttribute   = genericAttributeKuerzel + ":" + genericValueAttribute;
    genericMeasureAttribute = genericAttributeKuerzel + ":" + genericMeasureAttribute;
    genericAttributeSet = genericAttributeKuerzel + ":" + genericAttributeSet;

    if ( schemaType == XPLANGML_2_0 ||
         schemaType == XPLANGML_3_0 ||
         schemaType == XPLANGML_4_0 ||
         schemaType == XPLANGML_4_1 ||
         schemaType == XPLANGML_5_0 ||
         schemaType == XPLANGML_5_1 ||
         schemaType == XPLANGML_5_2 ||
         schemaType == XPLANGML_5_3 )
    {
      genericNameAttribute    = genericAttributeKuerzel + ":" + genericNameAttribute;
      genericValueAttribute   = genericAttributeKuerzel + ":" + genericValueAttribute;
    }
  }
  else
  {
    genericAttribute        = "_genericAttribute";
  }

  xpVariableGeometrieType = "xplan:XP_VariableGeometrieType";
  xpPunktGeometrieType    = "xplan:XP_PunktgeometrieType";
  xpLinienGeometrieType   = "xplan:XP_LiniengeometrieType";
  xpFlaechenGeometrieType = "xplan:XP_FlaechengeometrieType";

  advTAPointComponentType         = "adv:TA_PointComponentType";
  advTACurveComponentType         = "adv:TA_CurveComponentType";
  advTASurfaceComponentType       = "adv:TA_SurfaceComponentType";
  advTAMultiSurfaceComponentType  = "adv:TA_MultiSurfaceComponentType";
  advAGObjektType                 = "adv:AG_ObjektType";
  advAGPunktobjektType            = "adv:AG_PunktobjektType";
  advAGLinienobjektType           = "adv:AG_LinienobjektType";
  advAGFlaechenobjektType         = "adv:AG_FlaechenobjektType";

  abstractTimeSeriesType      = "energy:AbstractTimeSeriesPropertyType";
  regularTimeSeriesType       = "energy:RegularTimeSeriesType";
  regularTimeSeriesSetType    = "energy:RegularTimeSeriesSetType";
  irregularTimeSeriesType     = "energy:IrregularTimeSeriesType";
  regularTimeSeriesFileType   = "energy:RegularTimeSeriesFileType";
  irregularTimeSeriesFileType = "energy:IrregularTimeSeriesFileType";

  if ( addressPropKuerzel != "" )
    addressPropertyType     = addressPropKuerzel + ":AddressPropertyType" ;
  else
    addressPropertyType = "AddressPropertyType" ;

  if ( reliefKuerzel != "" )
    tinPropertyType = reliefKuerzel + ":tinPropertyType";
  else
    tinPropertyType = "tinPropertyType";

  if ( appearanceKuerzel != "" )
    appearanceMember = appearanceKuerzel + ":appearanceMember";
  else
    appearanceMember = "appearanceMember";

  if ( baseKuerzel != "" )
  {
    implicitRepresentationPropertyType = baseKuerzel + ":ImplicitRepresentationPropertyType";
    lodType                            = baseKuerzel + ":integerBetween0and4";
  }
  else
  {
    implicitRepresentationPropertyType = "ImplicitRepresentationPropertyType";
    lodType                            = "integerBetween0and4";
  }
}

SchemaTypeNames::~ SchemaTypeNames()
{

}

///////////////////////////////////////////////////////////////////////////////
// Liefert das Kürzel des GML-Namespaces                                     //
///////////////////////////////////////////////////////////////////////////////
std::string SchemaTypeNames::getGmlKuerzel()
{
  return "gml";
}

///////////////////////////////////////////////////////////////////////////////
// Liefert das Kürzel des GML-Coverage Namespaces                            //
///////////////////////////////////////////////////////////////////////////////
std::string SchemaTypeNames::getGmlCoverageKuerzel()
{
  return "gmlcov";
}

///////////////////////////////////////////////////////////////////////////////
// Liefert das Kürzel des XML-Schema Namespaces                              //
///////////////////////////////////////////////////////////////////////////////
std::string SchemaTypeNames::getXmlSchemaKuerzel()
{
  return  "xs";
}


