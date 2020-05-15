#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>
#include "Writable/X3D/X3DColor.h"


namespace X3D
{

	
	///////////////////////////////////////////////////////////////////////////////
	//  ColorWithTransparency                                                    //
	//  R, G, B und Transparenzwert                                              //
	///////////////////////////////////////////////////////////////////////////////
	class ColorWithTransparency : public Color
	{
	  public:
		ColorWithTransparency();
		ColorWithTransparency ( double r, double g, double b, double transparency );
		ColorWithTransparency ( double r, double g, double b );
		ColorWithTransparency ( double rgb, double transparency );
		ColorWithTransparency ( double rgb );
		virtual ~ ColorWithTransparency();

	  protected:
		//X3D::Color	m_color;
		double		m_trans;

	  public:/*
		void setColor ( const X3D::Color& color )  {  m_color = color;  }
		double getColor ()	const  {  return m_color;  }
		*/
		void setTransparency ( double transparency );
		double getTransparency ()  {  return m_trans;  }

		//System::String ^ get();
	};


	///////////////////////////////////////////////////////////////////////////////
	//  X3DAdvanced                                                              //
	//  R, G, B und Transparenzwert                                              //
	///////////////////////////////////////////////////////////////////////////////
	class X3DAdvanced : public ColorWithTransparency
	{
	  public:
		X3DAdvanced();
		X3DAdvanced ( double r, double g, double b, double alpha, double width );
		//X3DAdvanced ( double r, double g, double b, double alpha );
		X3DAdvanced ( double r, double g, double b, double width );
		X3DAdvanced ( double r, double g, double b );
		virtual ~ X3DAdvanced();

	  protected:
		double m_width;

	  public:	
		void setWidth ( double width )  {  m_width = width;  }
		double getWidth ()  {  return m_width;  }
	};


	
	///////////////////////////////////////////////////////////////////////////////
	//  X3DKlassenVisualParams                                                   //
	//  X3D-Darstellungsparameter für eine GML-Klasse                            //
	///////////////////////////////////////////////////////////////////////////////
	class X3DKlassenVisualParams
	{
	  public:
		X3DKlassenVisualParams();
		virtual ~ X3DKlassenVisualParams();

	  protected:
		std::string                                klassenName;
		ColorWithTransparency								 * pKlassenColor;
		X3DAdvanced								 * pKlassenAdvanced;
		//std::map<std::string,KmlPolyVisualParams*> mPolyVisualParams;
		//std::map<std::string,KmlLineVisualParams*> mLineVisualParams;
		//KmlPolyVisualParams                      * pPolyVisualParamsDefault;
		//KmlLineVisualParams                      * pLineVisualParamsDefault;

	  public: 
		void        setKlassenName ( std::string name )  {  klassenName = name;  }
		std::string getKlassenName ()                    {  return klassenName;  }

		void		setKlassenColor ( ColorWithTransparency * pColor );
		ColorWithTransparency*	getKlassenColor () { return pKlassenColor; };

		void			setKlassenAdvanced ( X3DAdvanced * pAdvanced );
		X3DAdvanced *	getKlassenAdvanced () { return pKlassenAdvanced; };

		//void                  addPolyVisualParams ( KmlPolyVisualParams * pParams, std::string selectAttributName );
		//KmlPolyVisualParams * getPolyVisualParams( std::string selectAttributName );

		//void                  addLineVisualParams ( KmlLineVisualParams * pParams, std::string selectAttributName  );
		//KmlLineVisualParams * getLineVisualParams( std::string selectAttributName  ); 
	};




	///////////////////////////////////////////////////////////////////////////////
	//  X3DVisualParams                                                          //
	//  Zusammenfassung aller X3D-Darstellungsparameter				             //
	///////////////////////////////////////////////////////////////////////////////
	class X3DVisualParams
	{
	  public:
		X3DVisualParams();
		virtual ~ X3DVisualParams();

	  protected:
		ColorWithTransparency					    * m_pBgSceneColor;
		ColorWithTransparency					    * m_pSelectElementColor;
		X3DAdvanced								    * m_pSelectInfoParam;
		X3DAdvanced								    * m_pLineParam;
		X3DAdvanced								    * m_pPointParam;
		std::map<std::string,X3DKlassenVisualParams*> m_mKlassenVisualParams;
		//GML_SCHEMA_TYPE                               m_gmlType;

	  public:
		/*void setGmlTyp ( GML_SCHEMA_TYPE typ )  {  m_gmlType = typ;  }
		GML_SCHEMA_TYPE getGmlTyp()  { return m_gmlType; }*/

		//setter-functions
		void setBgSceneColor ( ColorWithTransparency * pColor );
		void setSelectElementColor ( ColorWithTransparency * pColor );
		void setSelectInfoParam ( X3DAdvanced * pAdvanced );
		void setLineParam ( X3DAdvanced * pAdvanced );
		void setPointParam ( X3DAdvanced * pAdvanced );

		//getter-functions
		ColorWithTransparency * getBgSceneColor() { return m_pBgSceneColor; };
		ColorWithTransparency * getSelectElementColor () { return m_pSelectElementColor; };
		X3DAdvanced * getSelectInfoParam () { return m_pSelectInfoParam; };
		X3DAdvanced * getLineParam () { return m_pLineParam; };
		X3DAdvanced * getPointParam () { return m_pPointParam; };


		void addKlassenVisualParams ( X3DKlassenVisualParams * pKlassenVisualParams );

		X3DKlassenVisualParams*		getKlassenVisualParams ( std::string klassenName );
		//KmlPolyVisualParams    * getPolygonKlassenVisualParams ( std::string klassenName, std::string selectAttributName );
		//KmlLineVisualParams    * getLineKlassenVisualParams ( std::string klassenName, std::string selectAttributName );
	};






	///////////////////////////////////////////////////////////////////////////////
	//  X3DVisualParamReader                                                     //
	// Einlesen einer Datei mit X3D-Darstellungsparametern                       //
	///////////////////////////////////////////////////////////////////////////////
	class X3DVisualParamReader
	{
	  public:
		X3DVisualParamReader( std::string&  gmlTypeP );
		virtual ~X3DVisualParamReader();

	  protected:
		//gcroot<System::Xml::XmlTextReader^>		m_reader;

		//gcroot<System::String^>					m_pActElement;
		std::string								m_gmlType;
		X3DVisualParams*						m_pX3DParams;

	  public:
		X3DVisualParams * read( std::string filename );

	  protected:
		X3DKlassenVisualParams * readKlassenVisualParams();
		//X3DPolyVisualParams    * readPolyVisualParams( std::string selectAttributName );
		//X3DLineVisualParams    * readLineVisualParams( std::string selectAttributName );
		ColorWithTransparency               * readX3DColor();
		X3DAdvanced            * readX3DAdvanced();
		//X3DColor               * readColorTableEntry ( std::string &attributWert );
	};
}
