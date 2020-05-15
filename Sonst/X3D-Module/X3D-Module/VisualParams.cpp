#include <vector>
#include <map>
#include <string>
#include <set>
#include <iostream>
#include <cmath>

#include "VisualParams.h"
#include "X3D-Viewer.h"
#include "EnumDefinition.h"

//#include "includes.h"

using namespace std;

namespace X3D
{
	
	///////////////////////////////////////////////////////////////////////////////
	//  ColorWithTransparency                                                                 //
	//  R, G, B und Transparenzwert                                              //
	///////////////////////////////////////////////////////////////////////////////
	ColorWithTransparency::ColorWithTransparency()
	{
		//m_color = X3D::Color();
		setR(0.);
		setG(0.);
		setB(0.);
		m_trans = 0.;
	}

	ColorWithTransparency::ColorWithTransparency ( double r, double g, double b, double transparency )
	{
		//m_color	= X3D::Color(r, g, b);
		setR(r);
		setG(g);
		setB(b);

		if(transparency >= 0. && transparency <= 1.)
			m_trans = transparency;
		else if (transparency < 0.)
			m_trans = 0.;
		else if (transparency > 1.)
			m_trans = 1.;
	}

	ColorWithTransparency::ColorWithTransparency ( double r, double g, double b )
	{
		//m_color	= X3D::Color(r, g, b);
		setR(r);
		setG(g);
		setB(b);

		m_trans = 0.;
	}

	ColorWithTransparency::ColorWithTransparency ( double rgb, double transparency )
	{
		setR(rgb);
		setG(rgb);
		setB(rgb);

		if(transparency >= 0. && transparency <= 1.)
			m_trans = transparency;
		else if (transparency < 0.)
			m_trans = 0.;
		else if (transparency > 1.)
			m_trans = 1.;
	}

	ColorWithTransparency::ColorWithTransparency ( double rgb )
	{
		setR(rgb);
		setG(rgb);
		setB(rgb);

		m_trans = 0.;
	}

	ColorWithTransparency::~ColorWithTransparency()
	{
	}

	void ColorWithTransparency::setTransparency( double transparency )
	{
		if(transparency >= 0. && transparency <= 1.)
			m_trans = transparency;
		else if (transparency < 0.)
			m_trans = 0.;
		else if (transparency > 1.)
			m_trans = 1.;
	}


	///////////////////////////////////////////////////////////////////////////////
	//  Generierung der Hex-Darstellung                                          //
	///////////////////////////////////////////////////////////////////////////////
	//String ^  ColorWithTransparency::get()
	//{
	//  String ^ STR;
	//
	//  STR = String::Concat ( Int16 ( m_trans ).ToString( "X2" ),
	//                         Int16 ( m_b ).ToString( "X2" ),
	//                         Int16 ( m_g ).ToString( "X2" ),
	//                         Int16 ( m_r ).ToString( "X2" )
	//                          );
	//
	//  return STR;
	//}








	///////////////////////////////////////////////////////////////////////////////
	//  X3DAdvanced                                                              //
	//  R, G, B, Transparenzwert und Linien- / Punktdicke                        //
	///////////////////////////////////////////////////////////////////////////////
	X3DAdvanced::X3DAdvanced()
	{
		setR(0.);
		setG(0.);
		setB(0.);
		setTransparency(0.);
		m_width = 1.0;
	}

	X3DAdvanced::X3DAdvanced ( double r, double g, double b, double transparency, double width )
	{
		setR(r);
		setG(g);
		setB(b);
		setTransparency(transparency);
		m_width = width;
	}

	//X3DAdvanced::X3DAdvanced ( double r, double g, double b, double transparency )
	//{
	//	setR(r);
	//	setG(g);
	//	setB(b);
	//	setTransparency(transparency);
	//	m_width = 1.0;
	//}

	X3DAdvanced::X3DAdvanced ( double r, double g, double b, double width )
	{
		setR(r);
		setG(g);
		setB(b);
		setTransparency(0.);
		m_width = width;
	}

	X3DAdvanced::X3DAdvanced ( double r, double g, double b )
	{
		setR(r);
		setG(g);
		setB(b);
		setTransparency(0.);
		m_width = 1.0;
	}

	X3DAdvanced::~X3DAdvanced()
	{
	}


	
	///////////////////////////////////////////////////////////////////////////////
	//  X3DKlassenVisualParams                                                   //
	//  X3D-Darstellungsparameter für eine GML-Klasse                            //
	///////////////////////////////////////////////////////////////////////////////

	X3DKlassenVisualParams::X3DKlassenVisualParams()
	{
	  pKlassenColor = NULL;

	  klassenName        = "";
	}

	X3DKlassenVisualParams::~X3DKlassenVisualParams()
	{
	  if ( pKlassenColor != NULL ) delete pKlassenColor;
	  //if ( pLineVisualParamsDefault != NULL ) delete pLineVisualParamsDefault;
	}



	void X3DKlassenVisualParams::setKlassenColor ( ColorWithTransparency * pColor )
	{
	  if ( pKlassenColor != NULL ) delete pKlassenColor;
	  pKlassenColor = pColor;
	}

	void X3DKlassenVisualParams::setKlassenAdvanced ( X3DAdvanced * pAdvanced )
	{
	  if ( pKlassenAdvanced != NULL ) delete pKlassenAdvanced;
	  pKlassenAdvanced = pAdvanced;
	}




	///////////////////////////////////////////////////////////////////////////////
	//  X3DVisualParams                                                          //
	//  Zusammenfassung aller X3D-Darstellungsparameter				             //
	///////////////////////////////////////////////////////////////////////////////
	X3DVisualParams::X3DVisualParams()
	{
		m_pBgSceneColor			= NULL;
		m_pSelectElementColor	= NULL;
		m_pSelectInfoParam		= NULL;
		m_pLineParam			= NULL;
		m_pPointParam			= NULL;
	}

	X3DVisualParams::~X3DVisualParams()
	{
	  //map<string,X3DKlassenVisualParams*>::iterator iter;

	  if( m_pBgSceneColor != NULL ) delete m_pBgSceneColor;
	  if( m_pSelectElementColor != NULL ) delete m_pSelectElementColor;
	  if( m_pSelectInfoParam != NULL ) delete m_pSelectInfoParam;
	  if( m_pLineParam != NULL ) delete m_pLineParam;
	  if( m_pPointParam != NULL ) delete m_pPointParam;

	  for ( auto iter = m_mKlassenVisualParams.begin(); iter != m_mKlassenVisualParams.end(); iter++ )
	  {
		delete iter->second;
	  }
	}


	//setter-functions
	void X3DVisualParams::setBgSceneColor ( ColorWithTransparency * pColor )
	{
	  if ( m_pBgSceneColor != NULL ) delete m_pBgSceneColor;
	  m_pBgSceneColor = pColor;
	}

	void X3DVisualParams::setSelectElementColor ( ColorWithTransparency * pColor )
	{
	  if ( m_pSelectElementColor != NULL ) delete m_pSelectElementColor;
	  m_pSelectElementColor = pColor;
	}

	void X3DVisualParams::setSelectInfoParam ( X3DAdvanced * pAdvanced )
	{
	  if ( m_pSelectInfoParam != NULL ) delete m_pSelectInfoParam;
	  m_pSelectInfoParam = pAdvanced;
	}

	void X3DVisualParams::setLineParam ( X3DAdvanced * pAdvanced )
	{
	  if ( m_pLineParam != NULL ) delete m_pLineParam;
	  m_pLineParam = pAdvanced;
	}

	void X3DVisualParams::setPointParam ( X3DAdvanced * pAdvanced )
	{
	  if ( m_pPointParam != NULL ) delete m_pPointParam;
	  m_pPointParam = pAdvanced;
	}


	//getter-functions
	//void X3DVisualParams::getBgSceneColorSet ( X3DColor * pColor )
	//{
	//  if ( m_pBgSceneColor != NULL )	return true;
	//  else							return false;
	//}
	//
	//void X3DVisualParams::getSelectElementColorSet ( X3DColor * pColor )
	//{
	//  if ( m_pSelectElementColor != NULL )	return true;
	//  else									return false;
	//}
	//
	//void X3DVisualParams::getSelectInfoParamSet ( X3DAdvanced * pAdvanced )
	//{
	//  if ( m_pSelectInfoParam != NULL )	return true;
	//  else								return false;
	//}
	//
	//void X3DVisualParams::getLineParamSet ( X3DAdvanced * pAdvanced )
	//{
	//  if ( m_pLineParam != NULL ) 	return true;
	//  else							return false;
	//}
	//
	//void X3DVisualParams::getPointParamSet ( X3DAdvanced * pAdvanced )
	//{
	//  if ( m_pPointParam != NULL )	return true;
	//  else							return false;
	//}


	///////////////////////////////////////////////////////////////////////////////
	//  Objekt zur map hinzufügen                                                //
	///////////////////////////////////////////////////////////////////////////////
	void X3DVisualParams::addKlassenVisualParams (  X3DKlassenVisualParams * pKlassenVisualParams )
	{
	  map<string,X3DKlassenVisualParams*>::iterator iter;
	  string                                        klassenName = pKlassenVisualParams->getKlassenName();

	  iter = m_mKlassenVisualParams.find( klassenName );
	  if ( iter != m_mKlassenVisualParams.end() ) 
		delete iter->second;

	  m_mKlassenVisualParams[klassenName] = pKlassenVisualParams;
	}



	///////////////////////////////////////////////////////////////////////////////
	//  Zugriff auf die Objekte                                                  //
	///////////////////////////////////////////////////////////////////////////////
	X3DKlassenVisualParams * X3DVisualParams::getKlassenVisualParams ( std::string klassenName )
	{
	  map<string,X3DKlassenVisualParams*>::iterator iter;

	  iter = m_mKlassenVisualParams.find( klassenName );

	  if ( iter != m_mKlassenVisualParams.end() )
		return iter->second;
	  else
		return NULL;
	}






	///////////////////////////////////////////////////////////////////////////////
	//  X3DVisualParamReader                                                     //
	// Einlesen einer Datei mit X3D-Darstellungsparametern                       //
	///////////////////////////////////////////////////////////////////////////////
	X3DVisualParamReader::X3DVisualParamReader( std::string&  gmlTypeP )
	{
	  //m_gmlType    = gmlTypeP;
	  m_pX3DParams = NULL;
	}

	X3DVisualParamReader::~X3DVisualParamReader()
	{

	}

	///////////////////////////////////////////////////////////////////////////////
	//  Einlesen des XML-Files der X3D-Visualisierungsparameter                  //
	///////////////////////////////////////////////////////////////////////////////
	X3DVisualParams * X3DVisualParamReader::read( std::string filename )
	{
	/*
	  System::String			^ fName;
	  string					stdString;
	  bool						first = true;
	  X3DKlassenVisualParams *	pX3DKlassenVisualParams;
	  ColorWithTransparency               * pColor;
	  X3DAdvanced            * pAdvanced;

	  m_pX3DParams = new X3DVisualParams;
	  fName  = QuConvert::ToString ( filename );
	  m_reader = gcnew XmlTextReader(fName);

	  while( m_reader->Read() )
	  {
		if ( m_reader->NodeType == XmlNodeType::Element )
		{
		  m_pActElement = m_reader->LocalName;
		  if ( first )
		  {
			first = false;
			if ( m_pActElement->CompareTo("X3DVisualisierung") != 0 )
			{
	//          MessageBox::Show( "Ungültige Datei mit X3D-Darstellungsparametern");
			  delete m_pX3DParams;
			  m_pX3DParams = NULL;
			  break;
			}      
		  }

		  if ( m_pActElement->CompareTo("X3DKlassenVisualParams") == 0 )
		  {
			pX3DKlassenVisualParams = readKlassenVisualParams();
			if ( pX3DKlassenVisualParams != NULL )
			  m_pX3DParams->addKlassenVisualParams( pX3DKlassenVisualParams );
		  }
		  else
		  if ( m_pActElement->CompareTo("BgSceneColor") == 0 )
		  {
			pColor = readX3DColor();
			if ( pColor != NULL ) m_pX3DParams->setBgSceneColor( pColor );
		  }
		  else
		  if ( m_pActElement->CompareTo("SelectElementColor") == 0 )
		  {
			pColor = readX3DColor();
			if ( pColor != NULL ) m_pX3DParams->setSelectElementColor( pColor );
		  }
		  else
		  if ( m_pActElement->CompareTo("SelectInfoParam") == 0 )
		  {
			pAdvanced = readX3DAdvanced();
			if ( pAdvanced != NULL ) m_pX3DParams->setSelectInfoParam( pAdvanced );
		  }
		  else
		  if ( m_pActElement->CompareTo("LineParam") == 0 )
		  {
			pAdvanced = readX3DAdvanced();
			if ( pAdvanced != NULL ) m_pX3DParams->setLineParam( pAdvanced );
		  }
		  else
		  if ( m_pActElement->CompareTo("PointParam") == 0 )
		  {
			pAdvanced = readX3DAdvanced();
			if ( pAdvanced != NULL ) m_pX3DParams->setPointParam( pAdvanced );
		  }
		}
		else
		if ( m_reader->NodeType == XmlNodeType::EndElement )
		{
		  m_pActElement = m_reader->LocalName;
		  if ( m_pActElement->CompareTo("X3DVisualisierung") == 0 ) break;
		}
	  }
	  */
	  return m_pX3DParams;
	}

	///////////////////////////////////////////////////////////////////////////////
	//  Einlesen der Visualisierungsparameter für eine Klasse                    //
	///////////////////////////////////////////////////////////////////////////////
	X3DKlassenVisualParams * X3DVisualParamReader::readKlassenVisualParams()
	{
	  X3DKlassenVisualParams * pKlassenVisualParams = new X3DKlassenVisualParams;
	  //X3DPolyVisualParams    * pPolyVisualParams;
	  //X3DLineVisualParams    * pLineVisualParams;
/*
	  String                 ^ STR;
	  string                   stdString;
	  //string                   selectAttributName;
	  ColorWithTransparency               * pColor = NULL;
	  X3DAdvanced            * pAdvanced = NULL;

	  while( m_reader->Read() )
	  {
		if ( m_reader->NodeType == XmlNodeType::Element )
		{
		  m_pActElement = m_reader->LocalName;

		  if ( m_pActElement->CompareTo("KlassenName") == 0 )
		  {
			STR =  m_reader->ReadString();
			QuConvert::systemStr2stdStr( stdString, STR );
			pKlassenVisualParams->setKlassenName( stdString );
		  }
		  else
		  if ( m_pActElement->CompareTo("KlassenColor") == 0 )
		  {
			if ( pAdvanced == NULL || pColor == NULL )
			{
				pColor = readX3DColor();
				if ( pColor != NULL )
				{
					pKlassenVisualParams->setKlassenColor( pColor );
					delete pColor;
					pColor = NULL;
				}
			}
			else
			{
				MessageBox::Show( "ERROR: X3DKlassenVisualParams darf nur 1 KlassenColor oder 1 KlassenParam beinhalten");
				delete pAdvanced;
				delete pColor;
				pAdvanced = NULL;
				pColor = NULL;
				break;
			}
		  }
		  else
		  if ( m_pActElement->CompareTo("KlassenParam") == 0 )
		  {
			if ( pAdvanced == NULL || pColor == NULL )
			{
				pColor = readX3DAdvanced();
				if ( pAdvanced != NULL )
				{
					pKlassenVisualParams->setKlassenAdvanced( pAdvanced );
					delete pAdvanced;
					pAdvanced = NULL;
				}
			}
			else
			{
				MessageBox::Show( "ERROR: X3DKlassenVisualParams darf nur 1 KlassenColor oder 1 KlassenParam beinhalten");
				delete pAdvanced;
				delete pColor;
				pAdvanced = NULL;
				pColor = NULL;
				break;
			}
		  }
		}
		else
		if ( m_reader->NodeType == XmlNodeType::EndElement )
		{
		  m_pActElement = m_reader->LocalName;
		  if ( m_pActElement->CompareTo("X3DKlassenVisualParams") == 0 ) break;
		}
	  }
	  */
	  return pKlassenVisualParams;
	}


	///////////////////////////////////////////////////////////////////////////////
	//  Einlesen einer Farbdefinition                                            //
	///////////////////////////////////////////////////////////////////////////////
	ColorWithTransparency * X3DVisualParamReader::readX3DColor()
	{
	  ColorWithTransparency * pX3DColor = new ColorWithTransparency;
/*
	  String ^ STR;
	  short    h;


	  STR = m_reader->GetAttribute( "r" );
	  if ( STR != nullptr )
	  {
		//h = XmlConvert::ToInt16( STR );
		h = XmlConvert::ToDouble( STR );
		pX3DColor->setR(h);
	  }

	  STR = m_reader->GetAttribute( "g" );
	  if ( STR != nullptr )
	  {
		//h = XmlConvert::ToInt16( STR );
		h = XmlConvert::ToDouble( STR );
		pX3DColor->setG(h);
	  }

	  STR = m_reader->GetAttribute( "b" );
	  if ( STR != nullptr )
	  {
		//h = XmlConvert::ToInt16( STR );
		h = XmlConvert::ToDouble( STR );
		pX3DColor->setB(h);
	  }

	  STR = m_reader->GetAttribute( "transparency" );
	  if ( STR != nullptr )
	  {
		//h = XmlConvert::ToInt16( STR );
		h = XmlConvert::ToDouble( STR );
		pX3DColor->setTransparency(h);
	  }
	*/
	  return pX3DColor;
	}


	///////////////////////////////////////////////////////////////////////////////
	//  Einlesen einer Advanced-definition                                            //
	///////////////////////////////////////////////////////////////////////////////
	X3DAdvanced * X3DVisualParamReader::readX3DAdvanced()
	{
	  X3DAdvanced * pX3DAdvanced = new X3DAdvanced;
	  /*
	  String ^ STR;
	  short    h;


	  STR = m_reader->GetAttribute( "r" );
	  if ( STR != nullptr )
	  {
		//h = XmlConvert::ToInt16( STR );
		h = XmlConvert::ToDouble( STR );
		pX3DAdvanced->setR(h);
	  }

	  STR = m_reader->GetAttribute( "g" );
	  if ( STR != nullptr )
	  {
		//h = XmlConvert::ToInt16( STR );
		h = XmlConvert::ToDouble( STR );
		pX3DAdvanced->setG(h);
	  }

	  STR = m_reader->GetAttribute( "b" );
	  if ( STR != nullptr )
	  {
		  //h = XmlConvert::ToInt16( STR );
		h = XmlConvert::ToDouble( STR );
		pX3DAdvanced->setB(h);
	  }

	  STR = m_reader->GetAttribute( "width" );
	  if ( STR != nullptr )
	  {
		  //h = XmlConvert::ToInt16( STR );
		h = XmlConvert::ToDouble( STR );
		pX3DAdvanced->setTransparency(h);
	  }

	  STR = m_reader->GetAttribute( "width" );
	  if ( STR != nullptr )
	  {
		  //h = XmlConvert::ToInt16( STR );
		h = XmlConvert::ToDouble( STR );
		pX3DAdvanced->setWidth(h);
	  }
*/
	  return pX3DAdvanced;
	}
}
