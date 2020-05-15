/* Datei SHPFileReader.h
Diese Datei enthält folgende Klassendeklaration(en):

  - class SHPFileReader

  Quelle: Shapefile C Library V1.1 von Frank Warmerdam (PCI, 1995)
			Die public domain software wird hier von C in C++ umgesetzt
			(nur Leseroutinen; Erstellen und Modifizieren der Shapefiles
			ist nicht vorgesehen).
*/

#pragma once
//#pragma warning(disable: 4786)
//#pragma warning(disable: 4800)

class BPlan;

#include <string>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <map>
#include <vector>
#include "CoordPair.h"

#include "SHPTypeEnum.h"

typedef unsigned char uchar;
class XPlanGML;
class GeoPoint;

#if UINT_MAX == 65535
typedef long	    int32;
#else
typedef int	      int32;
#endif

#define ByteCopy( a, b, c )	memcpy( b, a, c )
#ifndef MAX
#  define MIN(a,b)      ((a<b) ? a : b)
#  define MAX(a,b)      ((a>b) ? a : b)
#endif

enum DBFFieldType
{
	FTString,
	FTInteger,
	FTDouble,
	FTDate,
	FTInvalid
};

class SHPInfo;
typedef SHPInfo* SHPHandle;

///////////////////////////////////////////////////////////////////////////////
//  SHPInfo																																	 //
//  Metadaten einer *.shp Datei																						   //
///////////////////////////////////////////////////////////////////////////////
class SHPInfo
{
private:
	std::fstream	fpSHP;
	std::fstream	fpSHX;

	SHPTypeEnum	eShapeType;	// SHPT_*
	int		nFileSize;				// SHP file

	int		nRecords;
	int		nMaxRecSize;

	std::vector<int>	vnRecOffsets;
	std::vector<int>	vnRecSizes;

//	MBoundingBox Bounds;
  double dMainXmin;
  double dMainXmax;
  double dMainYmin;
  double dMainYmax;
	double dMainZmin;
	double dMainZmax;
	double dMainMmin;
	double dMainMmax;

	SHPInfo ();									// Default Constructor
	SHPInfo (const SHPInfo&);		// Copy Constructor (nicht zugelassen)
	void operator= (const SHPInfo&);	// Zuweisung (nicht zugelassen)
	virtual ~SHPInfo ();					// Destructor

	void		SHPGetInfo (int&, SHPTypeEnum&);
	void		SHPReadBounds (int, SHPTypeEnum&, double &xMin, double &xMax, double &yMin, double &yMax /*MBoundingBox&*/ );

	friend class SHPFileReader;
};

class DBFInfo;
typedef DBFInfo* DBFHandle;

///////////////////////////////////////////////////////////////////////////////
//  DBFInfo																																	 //
//  Metadaten einer *.dbf Datei																							 //
///////////////////////////////////////////////////////////////////////////////
class DBFInfo
{
public:
  DBFInfo ();				                	// Default Constructor - Shapefiles lesen
  ~DBFInfo ();

	std::fstream	fpDBF;

protected:
	int		nRecords;

	int		nRecordLength;
	int		nHeaderLength;
	int		nFields;
	std::vector<int>	vnFieldOffsets;
	std::vector<int>	vnFieldSizes;
	std::vector<int>	vnFieldDecimals;
	std::vector<char>	vchFieldTypes;

	char*	pszHeader;
	char*	pszCurrentRecord;
	char*	pszStringField;   

	int		nStringFieldLen;
	int		nCurrentRecord;

	double dDoubleField;

	int		DBFGetFieldCount (void) { return (nFields); }
	int		DBFGetRecordCount (void) { return (nRecords); }
	DBFFieldType DBFGetFieldInfo (int, std::string&, int&, int&);

	friend class SHPFileReader;
};

///////////////////////////////////////////////////////////////////////////////
//  SHPFileReader																														 //
//  ESRI Shapefile Reader																									   //
///////////////////////////////////////////////////////////////////////////////
class SHPFileReader
{
private:
	XPlanGML    * m_pXPlanGML;
	std::string		LayerName, FileBaseName;

	SHPHandle				hSHP;
	DBFHandle				hDBF;

	int						iRange;

	int						nLayers;
	int						nTotalRecords;
	int						nTotalVertices;
	int						nActRecords;
	int						nActVertices;

	int						nTotalPtRecords;
	int						nTotalMPtRecords;
	int						nTotalLZRecords;
	int						nTotalAreaRecords;
	int						nTotalNullRecords;

	unsigned int  uActOAT;
	int						nActPtRecords;
	int						nActMPtRecords;
	int						nActLZRecords;
	int						nActAreaRecords;
	int						nActNullRecords;

	int						nRecords;
	SHPTypeEnum   eType;
	int						nVertices;
	unsigned int	nParts;

	int						nAttributes;
	int						nStrAttrs;
	int						nIntAttrs;
	int						nDblAttrs;
	int						nDateAttrs;

	std::vector<int>					vnPartOffsets;

	uchar*										pabyRec;
	std::vector<CoordPair>		vCPVertexData;

	std::vector<DBFFieldType>	veAttrTypes;
	std::vector<int>			    vnAttrWidths;
	std::vector<int>			    vnAttrDecimals;

	std::vector<std::string>	vsStrAttrNames;
	std::vector<std::string>	vsIntAttrNames;
	std::vector<std::string>	vsDblAttrNames;
	std::vector<std::string>	vsDateAttrNames;

	std::vector<std::string>	vsStrAttrValues;
	std::vector<int>		    	vnIntAttrValues;
	std::vector<double>			  vdDblAttrValues;
	std::vector<std::string>	vsDateAttrValues;

  double dXmin, dXmax, dYmin, dYmax;
	double dMmin;
	double dMmax;
	std::vector<double>	vdMeasures;

	double dZmin;
	double dZmax;
	std::vector<double>				vdElevations;

	std::vector<int>	vePartTypes;

public:
	explicit SHPFileReader ( XPlanGML * pXPLanGML, const std::string&);	// Konstruktor
	virtual ~SHPFileReader ();										// Destruktor

//  bool createShapefile ( SHPTypeEnum	eShapeType, std::vector<std::string> &vFieldNames, std::vector<DBFFieldType> vFieldTypes );

//  void writeShapefilePoint (  int nShapeId, GeoPoint * pPoint );


//  void writeShapefileGeometry( int nSHPType, int nShapeId, int nParts, int nVertices, 
//          std::vector<int> &vPanPartStart, std::vector<int> &vPanPartType,
//          std::vector<double> &vPadfX, std::vector<double> &vPadfY,  std::vector<double> &vPadfZ, std::vector<double> &vPadfM );
//  bool writeShapefileAttributes ( int nShapeId, std::vector<void*> vValues );
	
  bool read_from_files (void);
	bool readNextRecord (int);
	void setLayer_and_FileBaseName (const std::string&);
	void setRangeFlag (int);
  void closeShapefile();

protected:
	bool SHPOpen ();
	bool SHPinit ();
	void SHPClose (void);
//  bool SHPCreate( SHPTypeEnum	eShapeType );
//  void	SHPSetBounds( uchar * pabyRec, double xMin, double xMax, double yMin, double yMax );
	void SHPReadVertices (int);


//  bool DBFCreate(void);
	bool DBFOpen (void);
	bool DBFinit (void);
//  void DBFWriteHeader(void);
//  bool DBFAddField( std::string fieldName, DBFFieldType fieldType );
//  bool DBFWriteAttribute( int hEntity, int iField, void * pValue );
	void DBFReadAttribute (int, int);


	void DBFClose (void);

	void analyzePolygonParts ( void );

	void adaptString (std::string& sTarget);
};

