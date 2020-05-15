#include "StdAfx.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Windows::Forms;
using namespace System::Text;
using namespace System::Collections;

#using <mscorlib.dll>

#include <vector>
#include <map>
#include <string>
#include <set>
#include <iostream>
#include <cmath>
#include <algorithm> 

#include "GmlToolbox.h"
#include "Feature.h"
#include "Convert.h"
#include "GmlSchema.h"
#include "Dictionary.h"
#include "GmlNamespaces.h"
#include "Geometrie.h"
#include "Topologie.h"
#include "CityGML2X3D.h"
#include "X3D-Module/X3D-Viewer.h"
#include "X3D-Module/EnumDefinition.h"

#include "Triangulation\TessTriangulator.hpp"

#include "includes.h"

using namespace std;

namespace X3D
{

	///////////////////////////////////////////////////////////////////////////////
	//  CityGML2X3D                                                              //
	///////////////////////////////////////////////////////////////////////////////

	CityGML2X3D::CityGML2X3D(Features* features, CITYGML_LOD lodStufe, X3DViewer * viewer)
	{
		m_features		= features;
		m_gmlObjects	= features->getGmlSchema();
		m_lod			    = lodStufe;
		m_x3dViewer		= viewer;
	}

	bool CityGML2X3D::DefineGMLExtend()
	{
		double		  sw[3], no[3];
		bool		    success;
		std::string	srsName;
		int			    srsDimension;

		success = m_features->getBoundingBox( sw, no, srsName, srsDimension );

		if ( !success )
		{
			return false;
		}

		Point3D* minCoord = new Point3D(-sw[0], sw[2], sw[1]);
		m_x3dViewer->setMinCoord(minCoord);

		m_x3dViewer->setViewpointAttributes(Point3D(sw[0], sw[1], sw[2]), Point3D(no[0], no[1], no[2]));
		return true;
	}

	void CityGML2X3D::setSelectionAppearance()
	{
		m_x3dViewer->setPredefinedAppearance("Select");
		m_x3dViewer->setPredefinedAppearance("InfoSelect");
	}

	void CityGML2X3D::iterFeatures(std::multimap<std::string,Feature*>& features, const std::string& parentID, std::list<std::string>& featureTypes)
	{
		std::string				      klassenName;
    int                     featureAnz;
		std::list<std::string>	featureClasses;
    bool                    success;
    std::string	            featureName;
    int                     i;
    ABSTRACT_GEOMETRY_TYPE  geomType;
		
		//pGmlObjekte        = features.getGmlSchema();


		for ( auto iter = features.begin(); iter != features.end(); ++iter )
		{
			klassenName		        = iter->first;
			Feature *	feature			= iter->second;
			std::string gmlID			= feature->getGmlId();

			std::string parID			= feature->getParentId();
			std::string klassenID		= parID + klassenName;

			//bool					existFeatureClass;
			if(klassenName == "gen:stringAttribute" || klassenName == "gen:doubleAttribute")
			{
				std::string attributeValue;
				feature->getAttributWert(_STRING, "name", attributeValue);
				if(parID == "")
				{
					parID = "CityModel";
				}
				Group* pFeatureGroup = m_x3dViewer->getFeature(parID);
				if (klassenName == "gen:stringAttribute")
				{

					pFeatureGroup->addMetadataString(attributeValue, "");
				}
				else if (klassenName == "gen:doubleAttribute")
				{
					//feature->getAttributeWert(_DOUBLE, "name", attributeValue);
					pFeatureGroup->addMetadataDouble(attributeValue, 0.);
				}			
			}
			else
			{
				std::multimap<std::string, Feature*> children;
				std::set<std::string>                klassenNamen;
        std::vector<_Geometrie*>	           geom;
				bool                                 hasChildren;
        Group                              * featureGroup;

				int childCount = feature->getChildren( children, klassenNamen );
				if (childCount == 0)
					hasChildren = false;
				else
					hasChildren = true;

				if( std::find(featureClasses.begin(), featureClasses.end(), klassenID + "s") == featureClasses.end())
				{
          i  = 1;
					featureClasses.push_back(klassenID + "s");
          featureAnz = features.count( klassenName);
					m_x3dViewer->setFeature(klassenID + "s", parentID, klassenName, klassenName + "s", true, featureAnz );
				}

				if(std::find(featureTypes.begin(), featureTypes.end(), klassenName + "s") == featureTypes.end())
				{
					featureTypes.push_back(klassenName + "s");

					Input* inpu = new Input(klassenName + "_Layer", klassenName);
					inpu->setName("Layer");
					inpu->setOnchange("visibility(this)");

					m_x3dViewer->setXHTMLFeatureWithoutChild(klassenName, klassenName, inpu, m_x3dViewer->getFeatureType(), false);
				}

        success = feature->getStringAttributWert( "gml:name", featureName );
				if( ! success )
				{
					std::ostringstream str;
					str << klassenName << i;
					featureName = str.str();
					++i;
				}

				//m_x3dViewer.setFeature(gmlID, klassenID + "s", klassenName, featureName, hasChildren, childCount, x3dGeomType);
				m_x3dViewer->setFeature( gmlID, klassenID + "s", klassenName, featureName, true, 0 );

		    featureGroup = m_x3dViewer->getFeature(gmlID);
		    featureData(feature, featureGroup);

				int anz = feature->getGeometry( geom, m_lod );

        for ( unsigned int j = 0; j < anz; j++ )
        {
          _Geometrie * pGeometry = geom[j];
          geomType = pGeometry->getAbstractGeometryType();
          if ( geomType == ABSTRACT_MULTI_GEOMETRY )
          {
            GEOMETRY_TYPE geomTypeExact = pGeometry->getGeometryType();

            if (  geomTypeExact == MULTI_SURFACE || geomTypeExact == COMPOSITE_SURFACE || geomTypeExact == SHELL )
              geomType = ABSTRACT_SURFACE;
            else
            if (  geomTypeExact == MULTI_SOLID || geomTypeExact == COMPOSITE_SOLID )
              geomType = ABSTRACT_SOLID;
          }

					if (geomType == ABSTRACT_SURFACE || geomType == ABSTRACT_SOLID )
					{
					try
					{
            TessTriangulator          tessTria;
						std::vector<PolygonGeo*>	polyVec;
						std::vector<bool>		      oriVec;
            std::vector<Point3D*>     vPoint3DPoly;
            std::vector<int>          vIndexPoly;
            std::vector<Point3D*>     vPoint3DLine;
            std::vector<int>          vIndexLine;
						unsigned int			        polyAnz;
						std::string	              geomID = gmlID;

						if (geomType == ABSTRACT_SURFACE)
						{
							if(hasChildren)
              {
								geomID = gmlID + "_Surface";
								m_x3dViewer->setFeature( geomID, gmlID, "Surface", featureName + " Surface", false, 0 );
              }

              _Surface* multi = dynamic_cast<_Surface *>(pGeometry);
							polyAnz	= multi->getPolygone(polyVec, oriVec);
						}
						else if ( geomType == ABSTRACT_SOLID )
						{
							if(hasChildren)
              {
								geomID = gmlID + "_Solid";
								m_x3dViewer->setFeature(geomID, gmlID, "SOLID", featureName + " Solid", false, 0 );
              }

              _Solid* solid = dynamic_cast<_Solid*>(pGeometry);
							polyAnz	= solid->getAussenkonturPolygone(polyVec, oriVec);
						}	

                 
            tessTria.triangulatePolygons ( polyVec, vIndexPoly, vPoint3DPoly, oriVec );
            getLineGeometry ( polyVec, vIndexLine, vPoint3DLine );
	
            m_x3dViewer->setFaceGeometry(geomID, vPoint3DPoly, vIndexPoly, klassenName);
            m_x3dViewer->setLineGeometry( geomID + "_Line", vPoint3DLine, vIndexLine, klassenName+ "_Line" );
//  					m_x3dViewer->setPointGeometry(geomID + "_Point", gmlID + "_Points", klassenName + "_Point");
				  }
					catch (std::string e)
					{
					}

					}
        }			

			
				if(childCount != 0)
				{
					iterFeatures(children, gmlID, featureTypes);
				}
			}
	
		}
	}


	///////////////////////////////////////////////////////////////////////////////
	//  Schreibt die Sachdaten eines Features                                    //
	///////////////////////////////////////////////////////////////////////////////
	std::string CityGML2X3D::featureData ( Feature * pFeature, Group* parentGroup)
	{
		std::string		gmlId;
		std::string		klassenName;
		std::string		attributName;
		std::string		stringAttributWert;
		std::string		enumerationName;
		std::string		enumSchluessel;
		std::string		enumText;
		std::string		stdString;
		std::string		textInhalt;
		std::string		genericAttributName;
		std::string		genericAttributWert;
		std::string		STR;
		std::string		STR1;
		unsigned int	attributAnz, i, j, k, index, attributWertAnz;
		bool			success;
		ATTRIBUT_TYP	attributTyp;
		StringAttr *	pSimpleContent;
		DoubleAttr *	doubleAttributWert;
		IntAttr    *	intAttributWert;
		BoolAttr   *	boolAttributWert;

		GmlKlasse  *	pGmlKlasse;
		GmlAttribut        * pGmlAttribut;
		GmlDictionaryEntry * pDictionaryEntry;

		vector<GmlAttribut*>     gmlAttribute;
		vector<int>              classifierAnz;
		vector<string>           classifierNamen;
		vector<string>           classifierValues;
		vector<string>           classifierNamenLoc;
		vector<string>           classifierValuesLoc;
		vector<string>           genericAttributNamen;
		vector<StringAttr*>      stringAttributWerte;
		vector<IntAttr*>         intAttributWerte;
		vector<DoubleAttr*>      doubleAttributWerte;
		FeatureRelation        * pRelation;
		Feature                * pRelationZiel;
		vector<FeatureRelation*> relationen;
		vector<Feature*>         adressen;

		GmlDictionary*			pCodeLists;

		vector<string> attributNamen;
		vector<string> attributWerte;
		vector<ATTRIBUT_TYP> attributTypen;

		GmlElement * pGmlElement;
		StringWriter ^ stringWriter = gcnew StringWriter;

		klassenName = pFeature->getKlassenName ();

		pGmlElement = m_gmlObjects->getGmlElement( klassenName );


		pCodeLists         = m_gmlObjects->getCodeListsDictionary();

		if ( pGmlElement == NULL )
		{
			if ( klassenName == "core:externalReference" )
				pGmlKlasse = m_gmlObjects->getGmlKlasse ( "core:ExternalReferenceType" );
			else
				if ( klassenName == "citygml:externalReference" )
					pGmlKlasse = m_gmlObjects->getGmlKlasse ( "citygml:ExternalReferenceType" );
				else
					if ( klassenName == "core:externalObject" )
						pGmlKlasse = m_gmlObjects->getGmlKlasse ( "core:ExternalObjectReferenceType" );
					else
						if ( klassenName == "citygml:externalObject" )
							pGmlKlasse = m_gmlObjects->getGmlKlasse ( "citygml:ExternalObjectReferenceType" );
						else
							if ( klassenName == "core:xalAddress" )
							{
								//      writeXalAddress ( pPlanObjekt );
								//      return;
							}
		}
		else
			pGmlKlasse = m_gmlObjects->getGmlKlasse ( pGmlElement->getGmlTypeName() );

		if ( pGmlKlasse == NULL )
			return "";

		gmlId = pFeature->getGmlId();
		index = 0;

	// 	stringWriter->WriteLine ( "<html>" );
	// 
	// 	stringWriter->WriteLine ( "<style type=\"text/css\">\n" );
	// 	stringWriter->WriteLine ( "h1 {font-family: Arial, Helvetica, sans-serif; text-align: center;}" );
	// 	stringWriter->WriteLine ( "h2 {font-family: Arial, Helvetica, sans-serif; text-align: center;}" );
	// 	stringWriter->WriteLine ( "td {font-family: Arial, Helvetica, sans-serif;text-indent: 4px;}" );
	// 	stringWriter->WriteLine ( "</style>" );

		//  nameOhneKuerzel = removeDefaultNamespaceAttribut( klassenName );
	// 	STR = String::Concat ( "<h1>", QuConvert::ToString ( klassenName ), "</h1>" );
	// 	stringWriter->WriteLine ( STR );
	// 
	// 	stringWriter->WriteLine ( "<table width=\"100%\" border=\"1\" cellspacing=\"0\" bordercolor=\"#000000\">" );
	// 	stringWriter->WriteLine ( "<tr>" );
	// 	stringWriter->WriteLine ( "<td> <strong>Attribut-Name</strong></td>" );
	// 	stringWriter->WriteLine ( "<td> <strong>Attribut-Wert</strong></td>" );
	// 	stringWriter->WriteLine ( "</tr>" );
	// 
	// 	attributName = "gml:id";
	// 
	// 	stringWriter->WriteLine ( "<tr>" );
	// 	STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
	// 	stringWriter->WriteLine ( STR );
	// 
	// 	STR1 = QuConvert::ToString ( pFeature->getGmlId() );
	// 	STR1 = String::Concat ( "<td>", STR1, "</td>" );
	// 	stringWriter->WriteLine ( STR1 );
	// 	stringWriter->WriteLine ( "</tr>" );

		//  attributAnz = pFeature->getAttributWerte( attributTypen, attributNamen, attributWerte, classifierAnz, classifierNamen, classifierValues );

		attributAnz = pGmlKlasse->getGmlAttribute ( gmlAttribute );
		for ( i = 0; i < attributAnz; i++ )
		{
			stringAttributWerte.clear();
			intAttributWerte.clear();
			doubleAttributWerte.clear();
			relationen.clear();

			pGmlAttribut    = gmlAttribute[i];
			attributTyp  = pGmlAttribut->getGmlAttributTyp();
			attributName = pGmlAttribut->getGmlAttributName();

			switch ( pGmlAttribut->getGmlAttributTyp() )
			{
			case _STRING:
				attributWertAnz = pFeature->getStringAttributWerte ( attributName, stringAttributWerte );
				for ( j = 0; j < attributWertAnz; j++ )
				{
					stringAttributWert = stringAttributWerte[j]->value;

					parentGroup->addMetadataString(attributName, stringAttributWert);

	// 				STR1 = QuConvert::ToHtml ( stringAttributWert );
	// 				STR1 = String::Concat ( "<td>", STR1, "</td>" );
	// 
	// 				stringWriter->WriteLine ( "<tr>" );
	// 				STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
	// 				stringWriter->WriteLine ( STR );
	// 				stringWriter->WriteLine ( STR1 );
	// 				stringWriter->WriteLine ( "</tr>" );
				}
				break;

			case _DATE:
			case _DATE_TIME:
				attributWertAnz = pFeature->getDateAttributWerte ( attributName, stringAttributWerte );
				for ( j = 0; j < attributWertAnz; j++ )
				{
					stringAttributWert = stringAttributWerte[j]->value;

					parentGroup->addMetadataString(attributName, stringAttributWert);

	// 				STR1 = QuConvert::ToHtml ( stringAttributWert );
	// 				STR1 = String::Concat ( "<td>", STR1, "</td>" );
	// 
	// 				stringWriter->WriteLine ( "<tr>" );
	// 				STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
	// 				stringWriter->WriteLine ( STR );
	// 				stringWriter->WriteLine ( STR1 );
	// 				stringWriter->WriteLine ( "</tr>" );
				}
				break;

			case _DOUBLE:
			case _ANGLE:
			case _LENGTH:
			case _AREA:
			case _VOLUME:
			case _MEASURE:
				attributWertAnz = pFeature->getDoubleAttributWerte ( attributName, doubleAttributWerte );
				for ( j = 0; j < attributWertAnz; j++ )
				{
					doubleAttributWert = doubleAttributWerte[j];

					/////Debug ERROR bei Kaiserstraße/////////
					parentGroup->addMetadataDouble(attributName, doubleAttributWert->value);

	// 				STR1 = Convert::ToString( doubleAttributWert->value );
	// 				STR1 = String::Concat ( "<td>", STR1, "</td>" );
	// 
	// 				stringWriter->WriteLine ( "<tr>" );
	// 				STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
	// 				stringWriter->WriteLine ( STR );
	// 				stringWriter->WriteLine ( STR1 );
	// 				stringWriter->WriteLine ( "</tr>" );
				}
				break;

			case _INTEGER:
				attributWertAnz = pFeature->getIntegerAttributWerte ( attributName, intAttributWerte );
				for ( j = 0; j < attributWertAnz; j++ )
				{
					intAttributWert = intAttributWerte[j];

					parentGroup->addMetadataInt(attributName, intAttributWert->value);

	// 				STR1 = Convert::ToString( intAttributWert->value );
	// 				STR1 = String::Concat ( "<td>", STR1, "</td>" );
	// 
	// 				stringWriter->WriteLine ( "<tr>" );
	// 				STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
	// 				stringWriter->WriteLine ( STR );
	// 				stringWriter->WriteLine ( STR1 );
	// 				stringWriter->WriteLine ( "</tr>" );
				}
				break;

			case _ANY_URI:
				attributWertAnz = pFeature->getURLAttributWerte ( attributName, stringAttributWerte );
				for ( j = 0; j < attributWertAnz; j++ )
				{
					stringAttributWert = stringAttributWerte[j]->value;

					parentGroup->addMetadataString(attributName, stringAttributWert);

	// 				STR1 = QuConvert::ToHtml ( stringAttributWert );
	// 				STR1 = String::Concat ( "<td>", STR1, "</td>" );
	// 
	// 				stringWriter->WriteLine ( "<tr>" );
	// 				STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
	// 				stringWriter->WriteLine ( STR );
	// 				stringWriter->WriteLine ( STR1 );
	// 				stringWriter->WriteLine ( "</tr>" );
				}
				break;

			case _EXTERNAL_CODE_LIST_GML3_3:
				attributWertAnz = pFeature->getStringAttributWerte ( attributName, stringAttributWerte );
				for ( j = 0; j < attributWertAnz; j++ )
				{
					stringAttributWert = stringAttributWerte[j]->value;


					enumText = "";
					for ( k = 0; k < stringAttributWerte[j]->classifierName.size(); k++ )
					{
						if ( stringAttributWerte[j]->classifierName[k] == "xlink:title" )
						{
							enumText =  stringAttributWerte[j]->classifierValue[k];
							break;
						}
					}

					if ( enumText != "" )  stringAttributWert = enumText;

					parentGroup->addMetadataString(attributName, stringAttributWert);

	// 				STR1 = QuConvert::ToHtml ( stringAttributWert );
	// 				STR1 = String::Concat ( "<td>", STR1, "</td>" );
	// 
	// 				stringWriter->WriteLine ( "<tr>" );
	// 				STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
	// 				stringWriter->WriteLine ( STR );
	// 				stringWriter->WriteLine ( STR1 );
	// 				stringWriter->WriteLine ( "</tr>" );
				}
				break;

			case _ENUMERATION:
			case _EXTERNAL_CODE_LIST:
				attributWertAnz = pFeature->getStringAttributWerte ( attributName, stringAttributWerte );
				enumerationName = pGmlAttribut->getGmlEnumeration();
				pDictionaryEntry = pCodeLists->getDictionaryEntry ( enumerationName );
				for ( j = 0; j < attributWertAnz; j++ )
				{
					stringAttributWert = stringAttributWerte[j]->value;


					if ( pDictionaryEntry != NULL )
					{
						enumSchluessel = stringAttributWert;
						success = pDictionaryEntry->getSchluesselText ( enumSchluessel, enumText );
						if ( success ) stringAttributWert = enumText;

						parentGroup->addMetadataString(attributName, stringAttributWert);
					}



	// 				STR1 = QuConvert::ToHtml ( stringAttributWert );
	// 				STR1 = String::Concat ( "<td>", STR1, "</td>" );
	// 
	// 				stringWriter->WriteLine ( "<tr>" );
	// 				STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
	// 				stringWriter->WriteLine ( STR );
	// 				stringWriter->WriteLine ( STR1 );
	// 				stringWriter->WriteLine ( "</tr>" );
				}
				break;

			case  _REFERENZ:
				break;

			case _DATA_TYPE:
				attributWertAnz = pFeature->getFeatureRelationen (attributName, relationen );
				for ( j = 0; j < attributWertAnz; j++ )
				{
					pRelation = relationen[j];
					pRelationZiel = m_features->getFeature ( pRelation->featureId );

					if ( pRelationZiel != NULL )
					{
						pSimpleContent = pRelationZiel->getSimpleContent();
						if ( pSimpleContent != NULL )
						{
	// 						STR1 = QuConvert::ToString ( pSimpleContent->value );
	// 
	// 						stringWriter->WriteLine ( "<tr>" );
	// 						STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
	// 						stringWriter->WriteLine ( STR );
	// 						STR1 = String::Concat ( "<td>", STR1, "</td>" );
	// 						stringWriter->WriteLine ( STR1 );
	// 						stringWriter->WriteLine ( "</tr>" );
						}
						else
						{
	// 						stringWriter->WriteLine ( "<tr>" );
	// 						STR = String::Concat ( "<td><strong>", QuConvert::ToString ( attributName ), "</strong></td>" );
	// 						stringWriter->WriteLine ( STR );
	// 						STR = String::Concat ( "<td>", "  ", "</td>" );
	// 						stringWriter->WriteLine ( STR );
	// 						stringWriter->WriteLine ( "</tr>" );
	// 						writeDataType ( 1, stringWriter, pRelationZiel );
						}

					}
				}
				break;

			case _BOOLEAN:
				boolAttributWert = pFeature->getBoolAttributWert ( attributName );
				if ( boolAttributWert != NULL )
				{
					if ( boolAttributWert->value == true )
						stringAttributWert = "ja";
					else
						stringAttributWert = "nein";

					parentGroup->addMetadataBool(attributName, boolAttributWert->value);

	// 				STR1 = QuConvert::ToHtml ( stringAttributWert );
	// 				STR1 = String::Concat ( "<td>", STR1, "</td>" );
	// 
	// 				stringWriter->WriteLine ( "<tr>" );
	// 				STR = String::Concat ( "<td>", QuConvert::ToString ( attributName ), "</td>" );
	// 				stringWriter->WriteLine ( STR );
	// 				stringWriter->WriteLine ( STR1 );
	// 				stringWriter->WriteLine ( "</tr>" );
				}
				break;

			default:
				break;
			}
		}

		return std::string();
	}

	void CityGML2X3D::writeX3DViewer (const std::string fileName)
	{
		m_x3dViewer->writeX3DOM(fileName);
	}

  void CityGML2X3D::writeX3DFile (const std::string fileName)
  {
    m_x3dViewer->writeX3D(fileName);
  }


  void CityGML2X3D::getLineGeometry ( const std::vector<PolygonGeo*> &polyVec, std::vector<int> &coordIndexVec, std::vector<Point3D*> &pointVec )
  {
    unsigned int        index = 0;
    unsigned int        i, j, k, pointAnz, lochAnz;
    std::vector<double> vX, vY, vZ;

    for ( i = 0; i < polyVec.size(); i++ )
    {
      PolygonGeo * pPoly = polyVec[i];
      pointAnz = pPoly->getAussenkontur()->getPoints3D ( vX, vY, vZ );

      for ( j = 0; j < pointAnz; j++  )
      {
        Point3D * pPoint = new Point3D( -vX[j], vZ[j], vY[j] );

        pointVec.push_back( pPoint );
        coordIndexVec.push_back ( index );
        index++;
      }

      lochAnz = pPoly->getLochAnz();

      if ( lochAnz > 0 )
      {  
        for ( k = 0; k < lochAnz; k++ )
        {
          coordIndexVec.push_back( -1 );
          vX.clear();
          vY.clear();
          vZ.clear();

          pointAnz = pPoly->getLoch( k )->getPoints3D ( vX, vY, vZ );
          for ( j = 0; j < pointAnz; j++  )
          {
            Point3D * pPoint = new Point3D ( -vX[j], vZ[j], vY[j] );

            pointVec.push_back( pPoint );
            coordIndexVec.push_back ( index );
            index++;
          }
        }
      }
      else
      {
        vX.clear();
        vY.clear();
        vZ.clear();
      }

      coordIndexVec.push_back( -1 );
    }
  }
}