
/* Datei SHPFileReader.cpp
Diese Datei enthält die Implementierung der Klasse SHPFileReader mit
folgenden Methoden:

 - SHPFileReader :: SHPFileReader (string);					// Konstruktor
 - SHPFileReader :: ~SHPFileReader ();						// Destruktor
 - void SHPFileReader :: open_files (void);
 - void SHPFileReader :: read_from_files (void);
 - void SHPFileReader :: readNextRecord (void);
 - void SHPFileReader :: setLayer_and_FileBaseName (string);

 - void SHPFileReader :: SHPOpen (void);
 - void SHPInfo :: SHPGetInfo (int*, int*);
 - double* SHPInfo :: SHPReadVertices (int, int*, int*, int**);
 - void SHPInfo :: SHPReadBounds (int, double*);
 - void SHPFileReader :: SHPClose (void);

 - void SHPFileReader :: DBFOpen (void);
 - DBFFieldType DBFInfo :: DBFGetFieldInfo (int, string&, int&, int&);
 - int DBFInfo :: DBFReadIntegerAttribute (int, int);
 - double DBFInfo :: DBFReadDoubleAttribute (int, int);
 - const char* DBFInfo :: DBFReadStringAttribute (int, int);
 - void SHPFileReader :: DBFClose (void);
 - template<class T> void SwapWord (int, T*);

 - SHPInfo :: SHPInfo ();
 - SHPInfo :: ~SHPInfo ();
 - DBFInfo :: DBFInfo ();
 - DBFInfo :: ~DBFInfo ();
 - uchar* DBFInfo :: DBFReadAttribute (DBFHandle, int, int);

*/

#include "StdAfx.h"
#include <vector>
#include <string>

#include "XPlanGML.h"
#include "SHPFileReader.h"
#include "shptypeenum.h"
#include "Geometrie.h"

#include <math.h>
#include <limits.h>
using namespace std;

bool bBigEndian;

//template<class T> void SwapWord (int length, T* pFirstWord);

/************************************************************************/
/*                             SfRealloc()                              */
/*                                                                      */
/*      A realloc cover function that will access a NULL pointer as     */
/*      a valid input.                                                  */
/************************************************************************/

template<class T> void SwapWord (int length, T* pFirstWord)
{
  int		i;
  uchar	temp;
  uchar	*wordP = (uchar *) pFirstWord;

  for (i=0; i < length / 2; ++i)
  {
    temp = wordP[i];
    wordP[i] = wordP[length - i - 1];
    wordP[length - i - 1] = temp;
  }
}

///////////////////////////////////////////////////////////////////////////////
//  SHPInfo                                                                  //
//  Metadaten einer *.shp Datei																						   //
///////////////////////////////////////////////////////////////////////////////
SHPInfo :: SHPInfo ()
  : eShapeType	(SHPT_NULL), nFileSize	(0), nRecords (0), nMaxRecSize (0),
  dMainXmin (0.0), dMainXmax (0.0),  dMainYmin (0.0), dMainYmax (0.0),
  dMainZmin (0.0), dMainZmax (0.0), dMainMmin (0.0), dMainMmax (0.0)
{
}

SHPInfo ::~SHPInfo ()
{

}							

void SHPInfo :: SHPGetInfo (int& nEntities, SHPTypeEnum& eActShapeType)
{
  nEntities = nRecords;

  eActShapeType = eShapeType;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die (x,y) Bounding Box                                           //
///////////////////////////////////////////////////////////////////////////////
void SHPInfo :: SHPReadBounds (int hEntity, SHPTypeEnum& iRST, double &xMin, double &xMax, double &yMin, double &yMax /*MBoundingBox& rBB*/ )
{
//	double sw[3], len[3];
//	int    i;
	/* -------------------------------------------------------------------- */
	/*      Validate the record/entity number.                              */
	/* -------------------------------------------------------------------- */
	if (hEntity < -1 || hEntity >= nRecords)
	{
    xMin = 0.0;
    xMax = 0.0;
    yMin = 0.0;
    yMax = 0.0;
		return;
	}

	/* -------------------------------------------------------------------- */
	/*	If the entity is -1 we fetch the bounds for the whole file.					*/
	/* -------------------------------------------------------------------- */
	if (hEntity == -1)
	{

    xMin = dMainXmin;
    xMax = dMainXmax;
    yMin = dMainYmin;
    yMax = dMainYmax;

		return;
	}

	/* -------------------------------------------------------------------- */
	/*      Check the shape type of the record, to detect Null Shapes.      */
	/* -------------------------------------------------------------------- */
	int    iRecSHPType;

	fpSHP.seekg (vnRecOffsets[hEntity] + 8, ios::beg);
	fpSHP.read ((char*) (&iRecSHPType), 4);

	if (bBigEndian)
		SwapWord (4, &iRecSHPType);
	iRST = static_cast<SHPTypeEnum> (iRecSHPType);

	if (iRST == 0)
	{
    xMin = 0.0;
    xMax = 0.0;
    yMin = 0.0;
    yMax = 0.0;
	}

	/* -------------------------------------------------------------------- */
	/*      Extract bounds for any record but a point record.               */
	/* -------------------------------------------------------------------- */
	else if (iRST != SHPT_POINT &&
					 iRST != SHPT_POINT_Z &&
					 iRST != SHPT_POINT_M
					)
	{
		double padBounds[4];

		fpSHP.seekg (vnRecOffsets[hEntity] + 12, ios::beg);
		fpSHP.read ((char*) padBounds, 32);

		if (bBigEndian)
		{
			SwapWord (8, padBounds);
			SwapWord (8, padBounds + 1);
			SwapWord (8, padBounds + 2);
			SwapWord (8, padBounds + 3);
		}

    xMin = padBounds[0];
    yMin = padBounds[1];
    xMax = padBounds[2];
    yMax = padBounds[3];
	}

	/* -------------------------------------------------------------------- */
	/*      For points we fetch the point, and duplicate it as the          */
	/*      minimum and maximum bound.                                      */
	/* -------------------------------------------------------------------- */
	else
	{
		double padCoords[2];

		fpSHP.seekg (vnRecOffsets[hEntity] + 12, ios::beg);
		fpSHP.read ((char*) padCoords, 16);

		if (bBigEndian)
		{
			SwapWord (8, padCoords);
			SwapWord (8, padCoords + 1);
		}

    xMin = xMax = padCoords[0];
    yMin = yMax = padCoords[1];
	}
}

///////////////////////////////////////////////////////////////////////////////
//  DBFInfo																																	 //
//  Metadaten einer *.dbf Datei																							 //
///////////////////////////////////////////////////////////////////////////////
DBFInfo :: DBFInfo ()
  : nRecords	(0), nFields (0), nRecordLength(0), nHeaderLength(0),
  pszHeader (NULL), pszCurrentRecord	(NULL), pszStringField (NULL),
  nCurrentRecord	(-1), nStringFieldLen (0)
{
}


DBFInfo :: ~DBFInfo ()
{
  if (pszHeader != NULL)
  {
    delete [] pszHeader;
    pszHeader = NULL;
  }
  if (pszCurrentRecord != NULL)
  {
    delete [] pszCurrentRecord;
    pszCurrentRecord = NULL;
  }
  if (pszStringField != NULL)
  {
    delete [] pszStringField;
    pszStringField = NULL;
  }
}

static void * SfRealloc( void * pMem, int nNewSize )

{
  if( pMem == NULL )
    return( (void *) malloc(nNewSize) );
  else
    return( (void *) realloc(pMem,nNewSize) );
}

///////////////////////////////////////////////////////////////////////////////
//  Konstruktor                                                              //
///////////////////////////////////////////////////////////////////////////////
SHPFileReader :: SHPFileReader ( XPlanGML* p, const string& SHPName)
	: m_pXPlanGML ( p ), hSHP (NULL), hDBF (NULL),
		iRange (1), nLayers (0),
		nTotalRecords (0), nTotalVertices (0), nActRecords (0), nActVertices (0),
		nTotalPtRecords (0), nTotalMPtRecords (0), nTotalLZRecords (0),
		nTotalAreaRecords (0), nTotalNullRecords (0),
		uActOAT (0), nActPtRecords (0), nActMPtRecords (0), nActLZRecords (0),
		nActAreaRecords (0), nActNullRecords (0),
		nRecords (0), eType (SHPT_NULL), nVertices (0), nParts (0),
		nAttributes (0), nStrAttrs (0), nIntAttrs (0), nDblAttrs (0), nDateAttrs (0), pabyRec (NULL),
    dXmin (0.0), dXmax (0.0), dYmin (0.0), dYmax (0.0), dZmin (0.0), dZmax (0.0), dMmin (0.0), dMmax (0.0)
{
	int i = 1;

	/* -------------------------------------------------------------------- */
	/*	Establish the byte order on this machine.												    */
	/* -------------------------------------------------------------------- */
	if (*((uchar *) &i) == 1)
		bBigEndian = false;
	else
		bBigEndian = true;

	setLayer_and_FileBaseName (SHPName);
}

///////////////////////////////////////////////////////////////////////////////
//  Destruktor                                                               //
///////////////////////////////////////////////////////////////////////////////
SHPFileReader :: ~SHPFileReader ()
{
	if (hSHP) SHPClose ();
	if (hDBF) DBFClose ();

	if (pabyRec != NULL)
	{
		delete [] pabyRec;
		pabyRec = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  setRangeFlag                                                             //
///////////////////////////////////////////////////////////////////////////////
void SHPFileReader :: setRangeFlag (int iFlag)
{
	iRange = iFlag;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlsen eines Shapefiles                                                 //
///////////////////////////////////////////////////////////////////////////////
bool SHPFileReader :: read_from_files (void)
{
	int iRecord = 0;
	bool success;

	if (nLayers > 1)
	{
		nActRecords = 0;
		nActVertices = 0;
		nActPtRecords = 0;
		nActMPtRecords = 0;
		nActLZRecords = 0;
		nActAreaRecords = 0;
		nActNullRecords = 0;
	}

	success = SHPinit ();
	if ( !success ) return false;

	success = DBFinit ();
	if ( !success ) return false;

	while (iRecord < nRecords && success )
	{
		success = readNextRecord (iRecord);
		++iRecord;
	}
	return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Initialisierung                                                          //
///////////////////////////////////////////////////////////////////////////////
bool SHPFileReader :: SHPinit ()
{
	SHPTypeEnum eRecordShapeType;

	bool success = SHPOpen ();
	if ( !success ) return false;

	hSHP->SHPGetInfo (nRecords, eType);
	hSHP->SHPReadBounds (-1, eRecordShapeType, dXmin, dXmax, dYmin, dYmax );
	return true;	
}

///////////////////////////////////////////////////////////////////////////////
// Generierung eines neuen Shapefiles (shp und shx-Datei)                    //
///////////////////////////////////////////////////////////////////////////////
/*
bool SHPFileReaderWriter::SHPCreate( SHPTypeEnum	eShapeType )
{
  string		    FullFileName; 
  std::fstream	fpSHP;
  std::fstream	fpSHX;
  uchar     	abyHeader[100];
  int32	i32;
  double	dValue;


  FullFileName = FileBaseName + ".shp";
  fpSHP.open (FullFileName.c_str (), ios::out | ios::binary);
  if ( !fpSHP )
    return false;

  FullFileName = FileBaseName + ".shx";
  fpSHX.open (FullFileName.c_str (), ios::out | ios::binary);
  if ( !fpSHP )
    return false;

  // -------------------------------------------------------------------- //
  //      Prepare header block for .shp file.                             //
  // -------------------------------------------------------------------- //
  for( unsigned i = 0; i < 100; i++ )
    abyHeader[i] = 0;

  abyHeader[2] = 0x27;				// magic cookie 
  abyHeader[3] = 0x0a;

  i32 = 50;						// file size 
  ByteCopy( &i32, abyHeader+24, 4 );
  if( !bBigEndian ) SwapWord( 4, abyHeader+24 );

  i32 = 1000;						// version 
  ByteCopy( &i32, abyHeader+28, 4 );
  if( bBigEndian ) SwapWord( 4, abyHeader+28 );

  i32 = eShapeType;					// shape type 
  ByteCopy( &i32, abyHeader+32, 4 );
  if( bBigEndian ) SwapWord( 4, abyHeader+32 );

  dValue = 0.0;					// set bounds 
  ByteCopy( &dValue, abyHeader+36, 8 );
  ByteCopy( &dValue, abyHeader+44, 8 );
  ByteCopy( &dValue, abyHeader+52, 8 );
  ByteCopy( &dValue, abyHeader+60, 8 );

  // -------------------------------------------------------------------- //
  //      Write .shp file header.                                         //
  // -------------------------------------------------------------------- //
  fpSHP.write ( (char*)abyHeader, 100 );

//  fwrite( abyHeader, 100, 1, fpSHP );

  // -------------------------------------------------------------------- //
  //      Prepare, and write .shx file header.                            //
  // -------------------------------------------------------------------- //
  i32 = 50;						// file size 
  ByteCopy( &i32, abyHeader+24, 4 );
  if( !bBigEndian ) SwapWord( 4, abyHeader+24 );
  fpSHX.write ( (char*)abyHeader, 100 );
//  fwrite( abyHeader, 100, 1, fpSHX );

  // -------------------------------------------------------------------- //
  //      Close the files, and then open them as regular existing files.  //
  // -------------------------------------------------------------------- //
  fpSHP.flush();
  fpSHP.close();

  fpSHX.flush();
  fpSHX.close();

  return SHPinit();
}
*/
///////////////////////////////////////////////////////////////////////////////
//  Generierung eines neuen Shapefiles (dbf-Datei)                           //
///////////////////////////////////////////////////////////////////////////////
/*
bool SHPFileReaderWriter::DBFCreate(void)
{
  string		    FullFileName; 
  std::fstream	fpDBF;

  hDBF = new DBFInfo ();

  FullFileName = FileBaseName + ".dbf";
  hDBF->fpDBF.open (FullFileName.c_str (), ios::out | ios::binary);
  if ( !hDBF->fpDBF )
    return false;

  hDBF->nRecordLength = 1;
  hDBF->nHeaderLength = 33;

  return true;
}
*/
///////////////////////////////////////////////////////////////////////////////
//  Hinzufügen eines Sachdatenfeldes                                         //
///////////////////////////////////////////////////////////////////////////////
/*
bool SHPFileReaderWriter::DBFAddField( std::string fieldName, DBFFieldType fieldType )
{
   int    nWidth;
   int    nDecimals;
   int    i;
   char	* pszFInfo;

  // -------------------------------------------------------------------- //
  //      Do some checking to ensure we can add records to this file.     //
  // -------------------------------------------------------------------- //
  if( hDBF->nRecords > 0 )
    return false;

  switch ( fieldType )
  {
  case FTString:
    nWidth = 255;
    nDecimals = 0;
    break;

  case FTInteger:
    nWidth = 12;
    nDecimals = 0;
    break;

  case FTDate:
    nWidth = 15;
    nDecimals = 0;
    break;

  case FTDouble:
    nWidth = 30;
    nDecimals = 15;
    break;
  }

  // -------------------------------------------------------------------- //
  //      SfRealloc all the arrays larger to hold the additional field    //
  //      information.                                                    //
  // -------------------------------------------------------------------- //
  hDBF->nFields++;

  hDBF->vnFieldOffsets.resize( hDBF->nFields, 0 );

  hDBF->vnFieldSizes.resize( hDBF->nFields, 0 );

  hDBF->vnFieldDecimals.resize( hDBF->nFields, 0 );
  
  hDBF->vchFieldTypes.resize( hDBF->nFields, 0 );

	// -------------------------------------------------------------------- //
  //      Assign the new field information fields.                        //
  // -------------------------------------------------------------------- //
  hDBF->vnFieldOffsets[hDBF->nFields-1] = hDBF->nRecordLength;
  hDBF->nRecordLength += nWidth;
  hDBF->vnFieldSizes[hDBF->nFields-1] = nWidth;
  hDBF->vnFieldDecimals[hDBF->nFields-1] = nDecimals;

  if( eType == FTString )
    hDBF->vchFieldTypes[hDBF->nFields-1] = 'C';
  else
  if( eType == FTDate )
    hDBF->vchFieldTypes[hDBF->nFields-1] = 'D';
  else
    hDBF->vchFieldTypes[hDBF->nFields-1] = 'N';

  // -------------------------------------------------------------------- //
  //      Extend the required header information.                         //
  // -------------------------------------------------------------------- //
  hDBF->nHeaderLength += 32;

  hDBF->pszHeader = (char *) SfRealloc(hDBF->pszHeader,hDBF->nFields*32);

  pszFInfo = hDBF->pszHeader + 32 * (hDBF->nFields-1);

  for( i = 0; i < 32; i++ )
    pszFInfo[i] = '\0';

  if( fieldName.length() < 10 )
    strncpy( pszFInfo, fieldName.c_str(), fieldName.length() );
  else
    strncpy( pszFInfo,  fieldName.c_str(), 10 );

  pszFInfo[11] = hDBF->vchFieldTypes[hDBF->nFields-1];

  if( eType == FTString )
  {
    pszFInfo[16] = nWidth % 256;
    pszFInfo[17] = nWidth / 256;
  }
  else
  {
    pszFInfo[16] = nWidth;
    pszFInfo[17] = nDecimals;
  }

  // -------------------------------------------------------------------- //
  //      Make the current record buffer appropriately larger.            //
  // -------------------------------------------------------------------- //
  hDBF->pszCurrentRecord = (char *) SfRealloc(hDBF->pszCurrentRecord, hDBF->nRecordLength);

  return true;
}
*/
///////////////////////////////////////////////////////////////////////////////
//  Scheiben dea Headers der dbf-Datei                                       //
///////////////////////////////////////////////////////////////////////////////
/*
void SHPFileReaderWriter::DBFWriteHeader()

{
  unsigned char	abyHeader[32];
  int		i;

  // -------------------------------------------------------------------- //
  //	Initialize the file header information.				//
  // -------------------------------------------------------------------- //
  for( i = 0; i < 32; i++ )
    abyHeader[i] = 0;

  abyHeader[0] = 0x03;		

  // date updated on close, record count preset at zero 

  abyHeader[8] = hDBF->nHeaderLength % 256;
  abyHeader[9] = hDBF->nHeaderLength / 256;

  abyHeader[10] = hDBF->nRecordLength % 256;
  abyHeader[11] = hDBF->nRecordLength / 256;

  // -------------------------------------------------------------------- //
  //      Write the initial 32 byte file header, and all the field        //
  //      descriptions.                                     		          //
  // -------------------------------------------------------------------- //

  hDBF->fpDBF.write ( (char*)abyHeader, 32 );

//  fwrite( psDBF->pszHeader, XBASE_FLDHDR_SZ, psDBF->nFields, psDBF->fp );

  for ( i = 0; i < hDBF->nFields; i++ )
    hDBF->fpDBF.write ( hDBF->pszHeader, 32 );

  // -------------------------------------------------------------------- //
  //      Write out the newline character if there is room for it.        //
  // -------------------------------------------------------------------- //
  if( hDBF->nHeaderLength > 32*hDBF->nFields + 32 )
  {
    char	cNewline;

    cNewline = 0x0d;
    hDBF->fpDBF.write (&cNewline, 1 );
//    fwrite( &cNewline, 1, 1, hDBF->fp );
  }

  hDBF->fpDBF.flush();
}
*/
///////////////////////////////////////////////////////////////////////////////
//  Initialisierung des Einlesens der dbf-Datei                              //
///////////////////////////////////////////////////////////////////////////////
bool SHPFileReader :: DBFinit (void)
{
	int			 iF;
	int			 iFWidth, iFDec;
	string	     HelpString;
	bool         success;
	DBFFieldType eFieldType;

	success = DBFOpen ();
	if ( !success ) return false;

	if (hDBF->DBFGetRecordCount () != nRecords)
	{
		DBFClose ();
		SHPClose ();
//		MessageBox::Show("IDMSG_SHPFileReader_1");
		//theApp.getLanguage()->displayMessage ( string ( "IDMSG_SHPFileReader_1" ) );
		return false;
	}

	nAttributes = hDBF->DBFGetFieldCount ();

	nStrAttrs = 0;
	nIntAttrs = 0;
	nDblAttrs = 0;

	veAttrTypes.clear ();
	vnAttrWidths.clear ();
	vnAttrDecimals.clear ();

	vsStrAttrNames.clear ();
	vsIntAttrNames.clear ();
	vsDblAttrNames.clear ();
	vsDateAttrNames.clear ();

	vsStrAttrValues.clear ();
	vnIntAttrValues.clear ();
	vdDblAttrValues.clear ();
	vsDateAttrValues.clear ();

  vsIntAttrNames.push_back( "FID" );

	for (iF = 0; iF < nAttributes; ++iF)
	{
		iFWidth = iFDec = 0;

		eFieldType = hDBF->DBFGetFieldInfo (iF, HelpString, iFWidth, iFDec);

		adaptString (HelpString);

		veAttrTypes.push_back (eFieldType);
		vnAttrWidths.push_back (iFWidth);
		vnAttrDecimals.push_back (iFDec);

		if (eFieldType == FTString)
		{
			++nStrAttrs;
			vsStrAttrNames.push_back (HelpString);
		}

		if (eFieldType == FTInteger)
		{
			++nIntAttrs;
			vsIntAttrNames.push_back (HelpString);
		}

		if (eFieldType == FTDouble)
		{
			++nDblAttrs;
			vsDblAttrNames.push_back (HelpString);
		}

		if (eFieldType == FTDate)
		{
			++nDateAttrs;
			vsDateAttrNames.push_back (HelpString);
		}
	}

	success = m_pXPlanGML->setShapeFileStructure ( eType, LayerName, vsStrAttrNames, 
													vsIntAttrNames, vsDblAttrNames,vsDateAttrNames  );
	return success;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen eines Features eines Shapefiles                                 //
///////////////////////////////////////////////////////////////////////////////
bool SHPFileReader :: readNextRecord (int iRecord)
{
	int iF;
	SHPTypeEnum eRecordShapeType;
	string HelpString;
  double xMin, xMax, yMin, yMax;

	if (hSHP && hDBF)
	{
		hSHP->SHPReadBounds (iRecord, eRecordShapeType, xMin, xMax, yMin, yMax );

		if (eRecordShapeType == SHPT_NULL)
		{
			++nTotalNullRecords;
			++nActNullRecords;
			++iRecord;
			return true;
		}

		SHPReadVertices (iRecord);

		++nTotalRecords;
		++nActRecords;
		nTotalVertices += nVertices;
		nActVertices   += nVertices;

		vsStrAttrValues.clear ();
		vnIntAttrValues.clear ();
		vdDblAttrValues.clear ();
		vsDateAttrValues.clear ();

    vnIntAttrValues.push_back( iRecord );

		for (iF = 0; iF < nAttributes; ++iF)
		{
			DBFReadAttribute (iRecord, iF);
		}

		switch (eRecordShapeType)
		{
			case SHPT_POINT:
			case SHPT_POINT_Z:
			case SHPT_POINT_M:
				++nTotalPtRecords;
				++nActPtRecords;
			case SHPT_MULTIPOINT:
			case SHPT_MULTIPOINT_Z:
			case SHPT_MULTIPOINT_M:
				if (eRecordShapeType == SHPT_MULTIPOINT ||
						eRecordShapeType == SHPT_MULTIPOINT_Z ||
						eRecordShapeType == SHPT_MULTIPOINT_M
					 )
				{
					++nTotalMPtRecords;
					++nActMPtRecords;
				}

				m_pXPlanGML->addShpPoint 
					( eRecordShapeType, LayerName, vCPVertexData,
					  vsStrAttrValues, vnIntAttrValues, vdDblAttrValues, vsDateAttrValues );
				break;

			case SHPT_ARC:
			case SHPT_ARC_Z:
			case SHPT_ARC_M:
				++nTotalLZRecords;
				++nActLZRecords;

				m_pXPlanGML->addShpLine 
					( eRecordShapeType, LayerName,
					vCPVertexData, vnPartOffsets,
					vsStrAttrValues, vnIntAttrValues, vdDblAttrValues, vsDateAttrValues );
				break;

			case SHPT_POLYGON:
			case SHPT_POLYGON_Z:
			case SHPT_POLYGON_M:
				++nTotalAreaRecords;
				++nActAreaRecords;

				analyzePolygonParts();

				m_pXPlanGML->addShpPolygon 
					( eRecordShapeType, LayerName ,
					 vCPVertexData, vnPartOffsets, vePartTypes,
					 vsStrAttrValues, vnIntAttrValues, vdDblAttrValues, vsDateAttrValues );

				break;

			case SHPT_MULTIPATCH:
				return false;

			default:
				break;
		}

		return true;
	}
	else
	{
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Sortieren der eingelesenen Rings nach Außenkonturen und zugehörigen      //
//  Innenkonturen                                                            //
///////////////////////////////////////////////////////////////////////////////
void SHPFileReader::analyzePolygonParts ( void )
{
	unsigned int         i, k, indexFirst, indexLast, cutIndex;
  int                  j;
	unsigned int         outerFaceIndex = 1;
	unsigned int         holeAnz      = 0;
	double               h[2];
	CoordPair		         coord;
	vector<double>       vX, vY;
	bool                 umlaufsinn;
	vector<LinearRing*>  aussenKonturen;
	LinearRing		     * pRing;
	LineSegment        * pSeg;
	double				       minDist = MINIMAL_POINT_DISTANCE;


	vePartTypes.clear();

	if ( nParts == 1 )
	{
		vePartTypes.push_back ( outerFaceIndex );
		return;
	}

	for ( i = 0; i < nParts; i++ )
	{
		indexFirst = vnPartOffsets[i];
		if ( i < nParts - 1 ) indexLast = vnPartOffsets[i+1];
		else				  indexLast = nVertices;
		vX.clear();
		vY.clear();

		for ( k = indexFirst; k < indexLast; k++ )
		{
			coord = vCPVertexData[k];
			vX.push_back ( coord.x );
			vY.push_back ( coord.y );
		}
		pSeg = new LineSegment ( 2, LineSegment::_LINEAR_RING );
		pSeg->addPoints2D ( vX, vY );
		pRing = new LinearRing ( 2 );
		pRing->setLineSegment ( pSeg );

		umlaufsinn = pRing->getUmlaufsinn2D ();
		if ( umlaufsinn ) //  Äußere Kontur
		{
			aussenKonturen.push_back ( pRing );
			vePartTypes.push_back ( outerFaceIndex );
			outerFaceIndex++;
		}
		else
		{
			vePartTypes.push_back ( 0 );
			holeAnz++;
			delete pRing;
		}
	}

	if ( holeAnz == 0 )
	{
		for ( i = 0; i < aussenKonturen.size(); i++ )	delete aussenKonturen[i];
		return;
	}

	for ( i = 0; i < nParts; i++ )
	{
		if ( vePartTypes[i] == 0 )
		{
			coord = vCPVertexData[vnPartOffsets[i]];
			h[0] = coord.x;
			h[1] = coord.y;

			for ( k = 0; k < aussenKonturen.size(); k++ )
			{
				pRing = aussenKonturen[k];
				POINT_LOCATION loc = pRing->checkInside2D ( h, minDist, cutIndex );
				if ( loc != OUTSIDE_POLYGON )
				{
          j = (int)k;
					vePartTypes[i] = -(j+1);
					break;
				}
			}
		}
	}

	for ( i = 0; i < aussenKonturen.size(); i++ )	delete aussenKonturen[i];
}

///////////////////////////////////////////////////////////////////////////////
// Analyse des übergebenen Basisnamens des Shapefiles                        //
///////////////////////////////////////////////////////////////////////////////
void SHPFileReader :: setLayer_and_FileBaseName (const string& FileName)
{
	int i, j, k;

	if (hSHP != NULL)
		SHPClose ();
	if (hDBF != NULL)
		DBFClose ();

	/* -------------------------------------------------------------------- */
	/*	Compute the base (layer) name.  If there is any extension			*/
	/*	on the passed in filename we will strip it off.						*/
	/* -------------------------------------------------------------------- */
	FileBaseName = FileName;

	i = (int)FileBaseName.find_last_of ('.');
	j = (int)FileBaseName.find_last_of ('/');
	k = (int)FileBaseName.find_last_of ('\\');

	// Determine the base name including the path to the files:
	if ((i > 0) && (i > j) && (i > k)) FileBaseName.erase (i);

	i = (int)FileBaseName.length ();

	// The layer name is the same, but without path information
	if ((k > 0) && (k > j)) LayerName = FileBaseName.substr (++k, i - k);
	else if (j > 0)					LayerName = FileBaseName.substr (++j, i - j);
	else										LayerName = FileBaseName;

	++nLayers;
}

///////////////////////////////////////////////////////////////////////////////
//  Öffnen eines vorhandenen Shapefiles                                      //
///////////////////////////////////////////////////////////////////////////////
bool SHPFileReader :: SHPOpen ()
{
	string		FullFileName; 
	//CString     msg;
	std::string msg;
	SHPHandle	psSHP;
    
	uchar			*pabyBuf;
	int				i;
    
	/* -------------------------------------------------------------------- */
	/*	Initialize the info structure.																			*/
	/* -------------------------------------------------------------------- */
	psSHP = new SHPInfo ();

	/* -------------------------------------------------------------------- */
	/*	Open the .shp and .shx files.  Note that files pulled from					*/
	/*	a PC to Unix with upper case filenames won't work!									*/
	/* -------------------------------------------------------------------- */
	FullFileName = FileBaseName;
	FullFileName += ".shp";

	  psSHP->fpSHP.open (FullFileName.c_str (), ios::in | ios::binary);

	if (!psSHP->fpSHP)
	{
		delete psSHP;
		hSHP = NULL;
		return false;
	}

	FullFileName = FileBaseName;
	FullFileName += ".shx";

  psSHP->fpSHX.open (FullFileName.c_str (), ios::in | ios::binary);

	if (!psSHP->fpSHX)
	{
		psSHP->fpSHP.close ();
		delete psSHP;
		hSHP = NULL;
		return false;
	}

	/* -------------------------------------------------------------------- */
	/*  Read the file size from the SHP file.																*/
	/* -------------------------------------------------------------------- */

	pabyBuf = new uchar[100];

	psSHP->fpSHP.read ((char*) pabyBuf, 100);

	psSHP->nFileSize = (pabyBuf[24] * 256 * 256 * 256 +
											pabyBuf[25] * 256 * 256 +
											pabyBuf[26] * 256 +
											pabyBuf[27]) * 2;

	/* -------------------------------------------------------------------- */
	/*  Read SHX file Header info                                           */
	/* -------------------------------------------------------------------- */
	psSHP->fpSHX.read ((char*) pabyBuf, 100);

	if (pabyBuf[0] != 0 ||
			pabyBuf[1] != 0 ||
			pabyBuf[2] != 0x27 ||
			(pabyBuf[3] != 0x0a && pabyBuf[3] != 0x0d))
	{
		psSHP->fpSHP.close ();
		psSHP->fpSHX.close ();
		delete psSHP;
		hSHP = NULL;
		delete [] pabyBuf;

		//msg = theApp.getLanguage()->getMessageCharString ( string ( "IDSTR_SHPFileReader_4" ) );
//		msg = "IDSTR_SHPFileReader_4";
//		msg += FullFileName.c_str();
//		MessageBox::Show ( msg.c_str() );
		return false;
	}

	double x1, y1, x2, y2;
//	double sw[3], len[3];
//	sw[2]  = 0.0;
//	len[2] = 0.0;

	psSHP->nRecords = pabyBuf[27] +
										pabyBuf[26] * 256 +
										pabyBuf[25] * 256 * 256 +
										pabyBuf[24] * 256 * 256 * 256;
	psSHP->nRecords = (psSHP->nRecords * 2 - 100) / 8;

	if (bBigEndian) SwapWord (4, pabyBuf + 32);
	memcpy (&i, pabyBuf + 32, 4);
	psSHP->eShapeType = static_cast<SHPTypeEnum> (i);

	if (bBigEndian) SwapWord (8, pabyBuf + 36);
	memcpy (&x1, pabyBuf + 36, 8);

	if (bBigEndian) SwapWord (8, pabyBuf + 44);
	memcpy (&y1, pabyBuf + 44, 8);

	if (bBigEndian) SwapWord (8, pabyBuf + 52);
	memcpy (&x2, pabyBuf + 52, 8);

	if (bBigEndian) SwapWord (8, pabyBuf + 60);
	memcpy (&y2, pabyBuf + 60, 8);

//	sw[0] = x1;
//	sw[1] = y1;
//	len[0] = x2 - x1;
//	len[1] = y2 - y1;
//	psSHP->Bounds.bbSet ( sw, len );
  psSHP->dMainXmin = x1;
  psSHP->dMainXmax = x2;
  psSHP->dMainYmin = y1;
  psSHP->dMainYmax = y2;

	if (bBigEndian) SwapWord (8, pabyBuf + 68);
	memcpy (&(psSHP->dMainZmin), pabyBuf + 68, 8);

	if (bBigEndian) SwapWord (8, pabyBuf + 76);
	memcpy (&(psSHP->dMainZmax), pabyBuf + 76, 8);

	if (bBigEndian) SwapWord (8, pabyBuf + 84);
	memcpy (&(psSHP->dMainMmin), pabyBuf + 84, 8);

	if (bBigEndian) SwapWord (8, pabyBuf + 92);
	memcpy (&(psSHP->dMainMmax), pabyBuf + 92, 8);

	delete [] pabyBuf;

	/* -------------------------------------------------------------------- */
	/*	Read the .shx file to get the offsets to each record in 						*/
	/*	the .shp file.																											*/
	/* -------------------------------------------------------------------- */
	
	(psSHP->vnRecOffsets).reserve (psSHP->nRecords);
	(psSHP->vnRecSizes).reserve (psSHP->nRecords);

	pabyBuf = new uchar[8];
	int32		nOffset, nLength;

	for (i = 0; i < psSHP->nRecords; ++i)
	{
		psSHP->fpSHX.read ((char*) pabyBuf, 8);

		memcpy (&nOffset, pabyBuf, 4);
		memcpy (&nLength, pabyBuf + 4, 4);

		if (!bBigEndian)
		{
			SwapWord (4, &nOffset);
			SwapWord (4, &nLength);
		}

		(psSHP->vnRecOffsets).push_back (nOffset * 2);

		nLength *= 2;
		(psSHP->vnRecSizes).push_back (nLength);

		psSHP->nMaxRecSize = (nLength > psSHP->nMaxRecSize) ? nLength : psSHP->nMaxRecSize;
	}

	psSHP->nMaxRecSize += 8;

	delete [] pabyBuf;

	hSHP = psSHP;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Einlesen der Geometrie                                                   //
///////////////////////////////////////////////////////////////////////////////
void SHPFileReader :: SHPReadVertices (int hEntity)
{
	unsigned int i, iOffset, iRecordLength;
	double       x, y, z, m;
	SHPTypeEnum eSHPType = hSHP->eShapeType;

	/* -------------------------------------------------------------------- */
	/*      Ensure our record buffer is large enough.                       */
	/* -------------------------------------------------------------------- */

	if (pabyRec == NULL)
		pabyRec = new uchar [hSHP->nMaxRecSize];

	nVertices = nParts = 0;
	vnPartOffsets.clear ();
	vCPVertexData.clear ();
	vePartTypes.clear ();

	/* -------------------------------------------------------------------- */
	/*      Validate the record/entity number.                              */
	/* -------------------------------------------------------------------- */

	if (hEntity < 0 || hEntity >= nRecords)
		return;

	/* -------------------------------------------------------------------- */
	/*      Read the record.                                                */
	/* -------------------------------------------------------------------- */

	iRecordLength = hSHP->vnRecSizes[hEntity] + 8;

	hSHP->fpSHP.seekg (hSHP->vnRecOffsets[hEntity], ios::beg);
	hSHP->fpSHP.read ((char*) pabyRec, iRecordLength);

	/* -------------------------------------------------------------------- */
	/*  Extract vertices for a Polygon, Arc or Multi-Patch.									*/
	/* -------------------------------------------------------------------- */

	if (eSHPType == SHPT_POLYGON ||
			eSHPType == SHPT_ARC ||
			eSHPType == SHPT_POLYGON_Z ||
			eSHPType == SHPT_ARC_Z ||
			eSHPType == SHPT_POLYGON_M ||
			eSHPType == SHPT_ARC_M ||
			eSHPType == SHPT_MULTIPATCH
		 )
	{
		unsigned int nPoints, nPartCount;

		memcpy (&nPartCount, pabyRec + 36 + 8, 4);
		memcpy (&nPoints, pabyRec + 40 + 8, 4);

		if (bBigEndian)
		{
			SwapWord (4, &nPartCount);
			SwapWord (4, &nPoints);
		}

		nParts = nPartCount;
		nVertices = nPoints;

		/* -------------------------------------------------------------------- */
		/*      Copy out the part array from the record.                        */
		/* -------------------------------------------------------------------- */

		for (i = 0; i < nParts; ++i)
		{
			memcpy (&nPartCount, pabyRec + 44 + 8 + 4 * i, 4);
			if (bBigEndian)
				SwapWord (4, &nPartCount);
			vnPartOffsets.push_back (nPartCount);
		}

		iOffset = 8 + 44 + 4 * nParts;

		if (eSHPType == SHPT_MULTIPATCH)
		{
			for (i = 0; i < nParts; ++i)
			{
				memcpy (&nPartCount, pabyRec + iOffset + 4 * i, 4);
				if (bBigEndian)
					SwapWord (4, &nPartCount);
				vePartTypes.push_back ( nPartCount );
			}

			iOffset += 4 * nParts;
		}

		/* -------------------------------------------------------------------- */
		/*      Copy out the vertices from the record.                          */
		/* -------------------------------------------------------------------- */

		for (i = 0; i < nPoints; ++i)
		{
			memcpy (&x, pabyRec + iOffset + i * 16, 8);
			memcpy (&y, pabyRec + iOffset + i * 16 + 8, 8 );

			if (bBigEndian)
			{
				SwapWord (8, &x);
				SwapWord (8, &y);
			}

			vCPVertexData.push_back (CoordPair (x, y));
		}

		if (eSHPType > SHPT_POLYGON)
		{
			iOffset += 16 * nPoints;

			if (eSHPType == SHPT_POLYGON_Z ||
					eSHPType == SHPT_ARC_Z ||
					eSHPType == SHPT_MULTIPATCH
				 )
			{
				memcpy (&x, pabyRec + iOffset, 8);
				memcpy (&y, pabyRec + iOffset + 8, 8);

				if (bBigEndian)
				{
					SwapWord (8, &x);
					SwapWord (8, &y);
				}

				dZmin = x;
				dZmax = y;

				iOffset += 16;

				for (i = 0; i < nPoints; ++i)
				{
					memcpy (&x, pabyRec + iOffset + i * 8, 8);

					if (bBigEndian)
						SwapWord (8, &x);
					vCPVertexData[i].z = x;
				}
				
				iOffset += 8 * nPoints;
			}

			if (iOffset < iRecordLength)
			{
				memcpy (&x, pabyRec + iOffset, 8);
				memcpy (&y, pabyRec + iOffset + 8, 8);

				if (bBigEndian)
				{
					SwapWord (8, &x);
					SwapWord (8, &y);
				}

				dMmin = x;
				dMmax = y;

				iOffset += 16;

				for (i = 0; i < nPoints; ++i)
				{
					memcpy (&x, pabyRec + iOffset + i * 8, 8);

					if (bBigEndian)
						SwapWord (8, &x);

					vCPVertexData[i].m = x;
				}
			}
		}
	}

	/* -------------------------------------------------------------------- */
	/*  Extract vertices for a MultiPoint.																	*/
	/* -------------------------------------------------------------------- */
	else if (eSHPType == SHPT_MULTIPOINT ||
					 eSHPType == SHPT_MULTIPOINT_Z ||
					 eSHPType == SHPT_MULTIPOINT_M
					)
	{
		unsigned int nPoints;

		memcpy (&nPoints, pabyRec + 44, 4);
		if (bBigEndian) SwapWord (4, &nPoints);

		nVertices = nPoints;

		for (i = 0; i < nPoints; ++i)
		{
			memcpy (&x, pabyRec + 48 + 16 * i, 8);
			memcpy (&y, pabyRec + 48 + 16 * i + 8, 8);

			if (bBigEndian)
			{
				SwapWord (8, &x);
				SwapWord (8, &y);
			}

			vCPVertexData.push_back (CoordPair (x, y));
		}

		if (eSHPType > SHPT_MULTIPOINT)
		{
			iOffset = 48 + 16 * nPoints;

			if (eSHPType == SHPT_MULTIPOINT_Z)
			{
				memcpy (&x, pabyRec + iOffset, 8);
				memcpy (&y, pabyRec + iOffset + 8, 8);

				if (bBigEndian)
				{
					SwapWord (8, &x);
					SwapWord (8, &y);
				}

				dZmin = x;
				dZmax = y;

				iOffset += 16;

				for (i = 0; i < nPoints; ++i)
				{
					memcpy (&x, pabyRec + iOffset + i * 8, 8);

					if (bBigEndian)
						SwapWord (8, &x);

					vCPVertexData[i].z = x;
				}

				iOffset += 8 * nPoints;
			}

			if (iOffset < iRecordLength)
			{
				memcpy (&x, pabyRec + iOffset, 8);
				memcpy (&y, pabyRec + iOffset + 8, 8);

				if (bBigEndian)
				{
					SwapWord (8, &x);
					SwapWord (8, &y);
				}

				dMmin = x;
				dMmax = y;

				iOffset += 16;

				for (i = 0; i < nPoints; ++i)
				{
					memcpy (&x, pabyRec + iOffset + i * 8, 8);

					if (bBigEndian)
						SwapWord (8, &x);

					vCPVertexData[i].m = x;
				}
			}
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Extract vertices for a point.                                   */
	/* -------------------------------------------------------------------- */
	else if (eSHPType == SHPT_POINT ||
					 eSHPType == SHPT_POINT_Z ||
					 eSHPType == SHPT_POINT_M
					)
	{
		nVertices = 1;

		memcpy (&x, pabyRec + 12, 8);
		memcpy (&y, pabyRec + 20, 8);

		if (bBigEndian)
		{
			SwapWord (8, &x);
			SwapWord (8, &y);
		}


		z = 0.0;
		m = 0.0;
		if (eSHPType > SHPT_POINT)
		{
			iOffset = 28;

			if (eSHPType == SHPT_POINT_Z)
			{
				memcpy (&z, pabyRec + iOffset, 8);

				if (bBigEndian)
					SwapWord (8, &z);

//				vdElevations.push_back (x);

				iOffset += 8;
			}

			memcpy (&m, pabyRec + iOffset, 8);

			if (bBigEndian)
				SwapWord (8, &m);

		}

		vCPVertexData.push_back (CoordPair (x, y, z, m));
	}

	return;
}

///////////////////////////////////////////////////////////////////////////////
//  Schließt die shp- und shx-Dateien                                        //
///////////////////////////////////////////////////////////////////////////////
void SHPFileReader :: SHPClose (void)
{
	/* -------------------------------------------------------------------- */
	/*      Free all resources, and close files.                            */
	/* -------------------------------------------------------------------- */
	if (hSHP != NULL)
	{
		hSHP->fpSHX.close ();
		hSHP->fpSHP.close ();
	}

	delete hSHP;
	hSHP = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Öffnet eine dbf-Datei                                                    //
///////////////////////////////////////////////////////////////////////////////
bool SHPFileReader :: DBFOpen (void)
{
	string		FullFileName;
	//CString		msg;
	std::string msg;
	DBFHandle	psDBF;
	uchar*		pabyBuf;
	int				nFields, nRecords, nHeadLen, nRecLen, iField, nFieldLen;

	psDBF = new DBFInfo ();

	FullFileName = FileBaseName;
	FullFileName += ".dbf";

	psDBF->fpDBF.open (FullFileName.c_str (), ios::in | ios::binary);

	if (!psDBF->fpDBF)
	{
		SHPClose ();
		delete psDBF;
		hDBF = NULL;

		//msg = theApp.getLanguage()->getMessageCharString ( string ( "IDSTR_SHPFileReader_5" ) );
//		msg = "IDSTR_SHPFileReader_5";
//		msg += FullFileName.c_str();
//		MessageBox::Show ( msg.c_str() );
		return false;

	}

	/* -------------------------------------------------------------------- */
	/*  Read Table Header info                                              */
	/* -------------------------------------------------------------------- */
	pabyBuf = new uchar[32];

	psDBF->fpDBF.read ((char*) pabyBuf, 32);

	psDBF->nRecords = nRecords = pabyBuf[4] +
															 pabyBuf[5] * 256 +
															 pabyBuf[6] * 256 * 256 +
															 pabyBuf[7] * 256 * 256 * 256;

	psDBF->nHeaderLength = nHeadLen = pabyBuf[8] + pabyBuf[9] * 256;
	psDBF->nRecordLength = nRecLen = pabyBuf[10] + pabyBuf[11] * 256;
    
	psDBF->nFields = nFields = (nHeadLen - 32) / 32;

	psDBF->pszCurrentRecord = new char[nRecLen];
	psDBF->pszHeader = new char[nHeadLen];

	delete [] pabyBuf;

	char* pachBuf = psDBF->pszHeader;

	/* -------------------------------------------------------------------- */
	/*  Read in Field Definitions                                           */
	/* -------------------------------------------------------------------- */
    
	psDBF->fpDBF.seekg (32, ios::beg);
	psDBF->fpDBF.read (pachBuf, nHeadLen);

	psDBF->vnFieldOffsets.reserve (nFields);
	psDBF->vnFieldSizes.reserve (nFields);
	psDBF->vnFieldDecimals.reserve (nFields);
	psDBF->vchFieldTypes.reserve (nFields);

	for (iField = 0; iField < nFields; ++iField)
	{
		uchar *pabyFInfo;

		pabyFInfo = (uchar*) (pachBuf + iField * 32);

		if ( pabyFInfo[11] == 'N' || pabyFInfo[11] == 'F' || pabyFInfo[11] == 'D' )
		{
			nFieldLen = pabyFInfo[16];
			psDBF->vnFieldDecimals.push_back (pabyFInfo[17]);
		}
		else
		{
			nFieldLen = pabyFInfo[16] + pabyFInfo[17] * 256;
			psDBF->vnFieldDecimals.push_back (0);
		}

		psDBF->vnFieldSizes.push_back (nFieldLen);
		psDBF->nStringFieldLen = (nFieldLen > psDBF->nStringFieldLen) ?
															nFieldLen : psDBF->nStringFieldLen;

		psDBF->vchFieldTypes.push_back ((char) pabyFInfo[11]);

		if (iField == 0)
			psDBF->vnFieldOffsets.push_back (1);
		else
			psDBF->vnFieldOffsets.push_back (psDBF->vnFieldOffsets[iField - 1] +
																			 psDBF->vnFieldSizes[iField - 1]);
	}

	++psDBF->nStringFieldLen;
	psDBF->pszStringField = new char[psDBF->nStringFieldLen];

	hDBF = psDBF;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//  Liefert die Field-Struktur der dbf-Datei                                 //
///////////////////////////////////////////////////////////////////////////////
DBFFieldType DBFInfo :: DBFGetFieldInfo (int iField, string& pszFieldName,
																				 int& nWidth, int& nDecimals)
{
	if (iField < 0 || iField >= nFields)
		return (FTInvalid);

	if (nWidth >= 0)
		nWidth = vnFieldSizes[iField];

	if (nDecimals >= 0)
		nDecimals = vnFieldDecimals[iField];

	if (&pszFieldName != NULL)
	{
		int	i;

		pszFieldName.assign ((const char*) (pszHeader + iField * 32), 11);
		for (i = 10; i > 0 && (pszFieldName[i] == ' ' || pszFieldName[i] == '\0' ); --i)
			pszFieldName.erase (i, 1);
	}

	if ( vchFieldTypes[iField] == 'N' ||
			vchFieldTypes[iField] == 'D' ||
			vchFieldTypes[iField] == 'F' )
	{
		if (  vchFieldTypes[iField] == 'D' )
			return (FTDate);
		else
		if (vnFieldDecimals[iField] > 0)
			return (FTDouble);
		else
			return (FTInteger);
	}
	else
	{
		return (FTString);
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Schließt die dbf-Datei                                                   //
///////////////////////////////////////////////////////////////////////////////
void SHPFileReader :: DBFClose (void)
{
	/* -------------------------------------------------------------------- */
	/*      Close, and free resources.                                      */
	/* -------------------------------------------------------------------- */
	if (hDBF != NULL)
		hDBF->fpDBF.close ();

	delete hDBF;
	hDBF = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  Liest ein Field aus der dbf-Datei                                        //
///////////////////////////////////////////////////////////////////////////////
void SHPFileReader :: DBFReadAttribute (int hEntity, int iField)
{
	int    nRecordOffset, ipos;
	uchar* pabyRec;
	double dDoubleField;

	/* -------------------------------------------------------------------- */
	/*	Have we read the record?																						*/
	/* -------------------------------------------------------------------- */
	if (hEntity < 0 || hEntity >= nRecords)
		return;

	if (hDBF->nCurrentRecord != hEntity)
	{
		nRecordOffset = hDBF->nRecordLength * hEntity + hDBF->nHeaderLength;

		(hDBF->fpDBF).seekg (nRecordOffset, ios::beg);
		(hDBF->fpDBF).read (hDBF->pszCurrentRecord, hDBF->nRecordLength);

		hDBF->nCurrentRecord = hEntity;
	}

	pabyRec = (uchar *) (hDBF->pszCurrentRecord);

	/* -------------------------------------------------------------------- */
	/*	Extract the requested field.																				*/
	/* -------------------------------------------------------------------- */
  strncpy (hDBF->pszStringField,
    (const char*) (pabyRec + hDBF->vnFieldOffsets[iField]),
    hDBF->vnFieldSizes[iField]);
	hDBF->pszStringField[hDBF->vnFieldSizes[iField]] = '\0';

	/* -------------------------------------------------------------------- */
	/*      Decode the field.                                               */
	/* -------------------------------------------------------------------- */
	if (hDBF->vchFieldTypes[iField] == 'N' ||
			hDBF->vchFieldTypes[iField] == 'D' ||
		    hDBF->vchFieldTypes[iField] == 'F'	)
	{
		if ( hDBF->vchFieldTypes[iField] == 'D' )
		{
			string HelpString (hDBF->pszStringField);
			adaptString (HelpString);
			vsDateAttrValues.push_back (HelpString);
		}
		else
		if (hDBF->vnFieldDecimals[iField] == 0)
		{
			vnIntAttrValues.push_back (atoi (hDBF->pszStringField));
		}
		else
		{
			sscanf_s (hDBF->pszStringField, "%lf", &dDoubleField /*, hDBF->nStringFieldLen*/);
			vdDblAttrValues.push_back (dDoubleField);
		}
	}
	else
	{
		ipos = hDBF->vnFieldSizes[iField] - 1;
		while ((ipos >= 0) && (hDBF->pszStringField[ipos] == ' '))
		{
			hDBF->pszStringField[ipos] = '\0 ';
			--ipos;
		}

		string HelpString (hDBF->pszStringField);
		adaptString (HelpString);
		vsStrAttrValues.push_back (HelpString);
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Hilfsroutine für Umwandlung von Sonderzeichen                            //
///////////////////////////////////////////////////////////////////////////////
void SHPFileReader :: adaptString (string& sTarget)
{
	if (sTarget.empty ()) return;

	const char cQ[8] = {(char) 142, (char) 153, (char) 154,
											(char) 132, (char) 148, (char) 129, (char) 225, '\"'};


	string::size_type pos = 0;

	while (((pos = sTarget.find_last_of (' ')) != string::npos) &&
				 (pos == sTarget.length () - 1))
		sTarget.resize (pos);

	pos = 0;

	while ((pos = sTarget.find_first_of (cQ, pos, 8)) != string::npos)
	{
		switch (sTarget[pos])
		{
			case ((char) 142):
				sTarget = sTarget.replace (pos, 1, 1, 'Ä');
				break;
			case ((char) 153):
				sTarget = sTarget.replace (pos, 1, 1, 'Ö');
				break;
			case ((char) 154):
				sTarget = sTarget.replace (pos, 1, 1, 'Ü');
				break;
			case ((char) 132):
				sTarget = sTarget.replace (pos, 1, 1, 'ä');
				break;
			case ((char) 148):
				sTarget = sTarget.replace (pos, 1, 1, 'ö');
				break;
			case ((char) 129):
				sTarget = sTarget.replace (pos, 1, 1, 'ü');
				break;
			case ((char) 225):
				sTarget = sTarget.replace (pos, 1, 1, 'ß');
				break;
			case ('\"'):
				sTarget = sTarget.replace (pos, 1, 1, '\'');
				break;
			default:
				break;
		}
	}

	return;
}

///////////////////////////////////////////////////////////////////////////////
//  Anlegen der Struktur eine neuen Shapefiles                               //
///////////////////////////////////////////////////////////////////////////////
/*
bool SHPFileReaderWriter::createShapefile ( SHPTypeEnum	eShapeType, vector<string> &vFieldNames, vector<DBFFieldType> vFieldTypes )
{
  bool success;

  success = SHPCreate ( eShapeType );
  if ( !success )
    return false;

  success = DBFCreate();
  if ( !success )
    return false;

  for ( unsigned int i = 0; i < vFieldNames.size(); i++ )
    DBFAddField ( vFieldNames[i], vFieldTypes[i] );

  DBFWriteHeader();

  return true;

}
*/
///////////////////////////////////////////////////////////////////////////////
//  Schreibt Point-Feature                                                   //
///////////////////////////////////////////////////////////////////////////////
/*
void SHPFileReaderWriter::writeShapefilePoint (  int nShapeId, GeoPoint * pPoint )
{
  int            nSHPType, nParts, nVertices;
  vector<int>    vPanPartStart;
  vector<int>    vPanPartType;
  vector<double> vPadfX;
  vector<double> vPadfY;
  vector<double> vPadfZ;
  vector<double> vPadfM;

  nSHPType  = SHPT_POINT;
  nParts    = 1;
  nVertices = 1;

  vPadfX.push_back ( pPoint->getX() );
  vPadfY.push_back ( pPoint->getY() );
  vPadfZ.push_back ( pPoint->getZ() );

  writeShapefileGeometry( nSHPType, -1, nParts, nVertices, vPanPartStart, vPanPartType, vPadfX, vPadfY, vPadfZ, vPadfM );
}
*/

///////////////////////////////////////////////////////////////////////////////
//  Schreiben des Geometrie-Teil eines neuen Features                        //
///////////////////////////////////////////////////////////////////////////////
/*
void SHPFileReaderWriter::writeShapefileGeometry( int nSHPType, int nShapeId, int nParts, int nVertices, 
                    vector<int> &vPanPartStart, vector<int> &vPanPartType,
                    vector<double> &vPadfX, vector<double> &vPadfY, vector<double> &vPadfZ, vector<double> &vPadfM )
{
  int	    nRecordOffset, i, nRecordSize;
  bool    hasM, hasZ;
  double  xMin, yMin, zMin, mMin, xMax, yMax, zMax, mMax;
  uchar	* pabyRec;
  int * panPartStart;
  int * panPartType;
  double * padfX;
  double * padfY;
  double * padfZ;
  double * padfM;

  if ( nVertices == 0 )
    return;

  //  Felder anlegen und Daten überttragen
  panPartStart = (int *)  malloc(sizeof(int)*nParts);
  panPartType  = (int *)  malloc(sizeof(int)*nParts);


  //* -------------------------------------------------------------------- 
  //*	Establish whether this shape type has M, and Z values.		
  //* -------------------------------------------------------------------- 
  if( nSHPType == SHPT_ARC_M
    || nSHPType == SHPT_POINT_M
    || nSHPType == SHPT_POLYGON_M
    || nSHPType == SHPT_MULTIPOINT_M )
  {
    hasM = true;
    hasZ = false;
  }
  else if( nSHPType == SHPT_ARC_Z
    || nSHPType == SHPT_POINT_Z
    || nSHPType == SHPT_POLYGON_Z
    || nSHPType == SHPT_MULTIPOINT_Z
    || nSHPType == SHPT_MULTIPATCH )
  {
    hasM = true;
    hasZ = true;
  }
  else
  {
    hasM = false;
    hasZ = false;
  }

  padfX = (double *) calloc(sizeof(double), nVertices );
  padfY = (double *) calloc(sizeof(double), nVertices );
  for( i = 0; i < nVertices; i++ )
  {
    padfX[i] = vPadfX[i];
    padfY[i] = vPadfY[i];
  }

  if ( hasZ )
  {
    padfZ = (double *) calloc(sizeof(double), nVertices );
    for( i = 0; i < nVertices; i++ )
      padfZ[i] = vPadfZ[i];
  }
  else padfZ = NULL;

  if ( hasM )
  {
    padfM = (double *) calloc(sizeof(double), nVertices );
    for( i = 0; i < nVertices; i++ )
      padfM[i] = vPadfM[i];
  }
  else padfM = NULL;

  if ( vPanPartType.size() == 0 )
    for ( i = 0; i < nParts; i++ )
      panPartType[i] = SHPRing;

  xMin = xMax = padfX[0];
  yMin = yMax = padfY[0];
  if ( hasZ )
    zMin = zMax = padfZ[0];
  else
    zMin = zMax = 0.0;

  if ( hasM )
    mMin = mMax = padfM[0];
  else
    mMin = mMax = 0.0;


  for( i = 1; i < nVertices; i++ )
  {
    if ( padfX[i] < xMin ) xMin = padfX[i];
    if ( padfX[i] > xMax ) xMax = padfX[i];
    if ( padfY[i] < yMin ) yMin = padfY[i];
    if ( padfY[i] > yMax ) yMax = padfY[i];

    if ( hasZ )
    {
      if ( padfZ[i] < zMin ) zMin = padfZ[i];
      if ( padfZ[i] > zMax ) zMax = padfZ[i];
    }
    if ( hasM )
    {
      if ( padfM[i] < mMin ) mMin = padfM[i];
      if ( padfM[i] > mMax ) mMax = padfM[i];
    }
  }


//* -------------------------------------------------------------------- 
//*      Add the new entity to the in memory index.                      
//* -------------------------------------------------------------------- 

	hSHP->nMaxRecSize =(int) ( hSHP->nMaxRecSize * 1.3 + 100);

  hSHP->vnRecOffsets.resize( hSHP->nMaxRecSize );
//	hSHP->panRecOffset = (int *) 
//            SfRealloc(psSHP->panRecOffset,sizeof(int) * psSHP->nMaxRecords );

  hSHP->vnRecSizes.resize( hSHP->nMaxRecSize );
//	psSHP->panRecSize = (int *) 
//            SfRealloc(psSHP->panRecSize,sizeof(int) * psSHP->nMaxRecords );
  
//* -------------------------------------------------------------------- 
//*      Initialize record.                                              
//* -------------------------------------------------------------------- 
  pabyRec = (uchar *) malloc( (nVertices * 4 * sizeof(double))  + (nParts * 8) + 128);
    
//* -------------------------------------------------------------------- 
//*  Extract vertices for a Polygon or Arc.				
//* -------------------------------------------------------------------- 
  if( nSHPType == SHPT_POLYGON
        || nSHPType == SHPT_POLYGON_Z
        || nSHPType == SHPT_POLYGON_M
        || nSHPType == SHPT_ARC 
        || nSHPType == SHPT_ARC_Z
        || nSHPType == SHPT_ARC_M
        || nSHPType == SHPT_MULTIPATCH )
  {
	  int32		_nPoints, _nParts;

  	_nPoints = nVertices;
	  _nParts = nParts;

	  SHPSetBounds( pabyRec + 12, xMin, xMax, yMin, yMax );

	  if( bBigEndian ) SwapWord( 4, &_nPoints );
	  if( bBigEndian ) SwapWord( 4, &_nParts );

	  ByteCopy( &_nPoints, pabyRec + 40 + 8, 4 );
	  ByteCopy( &_nParts, pabyRec + 36 + 8, 4 );

    nRecordSize = 52;
        
        // * Write part start positions.
         
	  ByteCopy( panPartStart, pabyRec + 44 + 8, 4 * nParts );
	  for( i = 0; i < nParts; i++ )
	  {
	    if( bBigEndian ) SwapWord( 4, pabyRec + 44 + 8 + 4*i );
      nRecordSize += 4;
	  }

        
         // Write multipatch part types if needed.
         
    if( nSHPType == SHPT_MULTIPATCH )
    {
        memcpy( pabyRec + nRecordSize, panPartType, 4*nParts );
        for( i = 0; i < nParts; i++ )
        {
            if( bBigEndian ) SwapWord( 4, pabyRec + nRecordSize );
            nRecordSize += 4;
        }
    }

        
        // Write the (x,y) vertex values.
         
	  for( i = 0; i < nVertices; i++ )
	  {
	    ByteCopy( padfX + i, pabyRec + nRecordSize, 8 );
	    ByteCopy( padfY + i, pabyRec + nRecordSize + 8, 8 );

	    if( bBigEndian )
        SwapWord( 8, pabyRec + nRecordSize );
            
	    if( bBigEndian )
        SwapWord( 8, pabyRec + nRecordSize + 8 );

      nRecordSize += 2 * 8;
    }

        
         // Write the Z coordinates (if any).
         
    if( nSHPType == SHPT_POLYGON_Z ||
        nSHPType == SHPT_ARC_Z ||
        nSHPType == SHPT_MULTIPATCH )
    {
      ByteCopy( &(zMin), pabyRec + nRecordSize, 8 );
      if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
      nRecordSize += 8;
            
      ByteCopy( &(zMax), pabyRec + nRecordSize, 8 );
      if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
      nRecordSize += 8;

      for( i = 0; i < nVertices; i++ )
      {
        ByteCopy( padfZ + i, pabyRec + nRecordSize, 8 );
        if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
        nRecordSize += 8;
      }
    }

        
         // Write the M values, if any.
         
    if( nSHPType == SHPT_POLYGON_M  ||
        nSHPType == SHPT_ARC_M      ||    
        nSHPType == SHPT_POLYGON_Z  ||
        nSHPType == SHPT_ARC_Z )
    {
      ByteCopy( &(mMin), pabyRec + nRecordSize, 8 );
      if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
      nRecordSize += 8;
            
      ByteCopy( &(mMax), pabyRec + nRecordSize, 8 );
      if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
      nRecordSize += 8;

      for( i = 0; i <nVertices; i++ )
      {
        ByteCopy( padfM + i, pabyRec + nRecordSize, 8 );
        if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
        nRecordSize += 8;
      }
    }
  }

//* -------------------------------------------------------------------- 
//*  Extract vertices for a MultiPoint.					
//* -------------------------------------------------------------------- 
  else if( nSHPType == SHPT_MULTIPOINT     ||
           nSHPType == SHPT_MULTIPOINT_Z   ||
           nSHPType == SHPT_MULTIPOINT_M )
  {
	  int32		_nPoints;
	  int    		i;

	  _nPoints = nVertices;

    SHPSetBounds( pabyRec + 12, xMin, xMax, yMin, yMax );

	  if( bBigEndian ) SwapWord( 4, &_nPoints );
	  ByteCopy( &_nPoints, pabyRec + 44, 4 );
	
	  for( i = 0; i < nVertices; i++ )
	  {
	    ByteCopy( padfX + i, pabyRec + 48 + i*16, 8 );
	    ByteCopy( padfY + i, pabyRec + 48 + i*16 + 8, 8 );

	    if( bBigEndian ) SwapWord( 8, pabyRec + 48 + i*16 );
	    if( bBigEndian ) SwapWord( 8, pabyRec + 48 + i*16 + 8 );
	  }

	  nRecordSize = 48 + 16 * nVertices;

    if( nSHPType == SHPT_MULTIPOINT_Z )
    {
      ByteCopy( &(zMin), pabyRec + nRecordSize, 8 );
      if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
      nRecordSize += 8;

      ByteCopy( &(zMax), pabyRec + nRecordSize, 8 );
      if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
      nRecordSize += 8;
            
      for( i = 0; i < nVertices; i++ )
      {
          ByteCopy( padfZ + i, pabyRec + nRecordSize, 8 );
          if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
          nRecordSize += 8;
      }
    }

    if( nSHPType == SHPT_MULTIPOINT_Z ||
        nSHPType == SHPT_MULTIPOINT_M )
    {
      ByteCopy( &(mMin), pabyRec + nRecordSize, 8 );
      if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
      nRecordSize += 8;

      ByteCopy( &(mMax), pabyRec + nRecordSize, 8 );
      if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
      nRecordSize += 8;
            
      for( i = 0; i < nVertices; i++ )
      {
        ByteCopy( padfM + i, pabyRec + nRecordSize, 8 );
        if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
        nRecordSize += 8;
      }
    }
  }

//* -------------------------------------------------------------------- 
//*      Write point.							
//* -------------------------------------------------------------------- 
  else if( nSHPType == SHPT_POINT  ||
           nSHPType == SHPT_POINT_Z  ||
           nSHPType == SHPT_POINT_M )
  {
	  ByteCopy( padfX, pabyRec + 12, 8 );
	  ByteCopy( padfY, pabyRec + 20, 8 );

	  if( bBigEndian ) SwapWord( 8, pabyRec + 12 );
	  if( bBigEndian ) SwapWord( 8, pabyRec + 20 );

    nRecordSize = 28;
        
    if( nSHPType == SHPT_POINT_Z )
    {
      ByteCopy( padfZ, pabyRec + nRecordSize, 8 );
      if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
      nRecordSize += 8;
    }
        
    if( nSHPType == SHPT_POINT_Z  ||
        nSHPType == SHPT_POINT_M )
    {
      ByteCopy( padfM, pabyRec + nRecordSize, 8 );
      if( bBigEndian ) SwapWord( 8, pabyRec + nRecordSize );
      nRecordSize += 8;
    }
  }

//* -------------------------------------------------------------------- 
//*      Not much to do for null geometries.                             
//* -------------------------------------------------------------------- 
  else if( nSHPType == SHPT_NULL )
  {
    nRecordSize = 12;
  }


// -------------------------------------------------------------------- //
//      Establish where we are going to put this record. If we are      //
//      rewriting and existing record, and it will fit, then put it     //
//      back where the original came from.  Otherwise write at the end. //
// -------------------------------------------------------------------- //
  if( nShapeId == -1 )
    nShapeId = hSHP->nRecords++;

  hSHP->vnRecOffsets[nShapeId] = nRecordOffset = hSHP->nFileSize;
  hSHP->vnRecSizes[nShapeId] = nRecordSize-8;
  hSHP->nFileSize += nRecordSize;
    
    
// -------------------------------------------------------------------- //
//      Set the shape type, record number, and record size.             //
// -------------------------------------------------------------------- //
  int i32;

  i32 = nShapeId+1;					
  if( !bBigEndian ) SwapWord( 4, &i32 );
  ByteCopy( &i32, pabyRec, 4 );

  i32 = (nRecordSize-8)/2;				// record size
  if( !bBigEndian ) SwapWord( 4, &i32 );
  ByteCopy( &i32, pabyRec + 4, 4 );

  i32 = nSHPType;				// shape type 
  if( bBigEndian ) SwapWord( 4, &i32 );
  ByteCopy( &i32, pabyRec + 8, 4 );

// -------------------------------------------------------------------- //
//      Write out record.                                               //
// -------------------------------------------------------------------- //

  hSHP->fpSHP.write( (char*)pabyRec, nRecordSize );
    


// -------------------------------------------------------------------- //
//	Expand file wide bounds based on this shape.			//
// -------------------------------------------------------------------- //


  if ( xMin < hSHP->dMainXmin )  hSHP->dMainXmin = xMin;
  if ( xMax > hSHP->dMainXmax )  hSHP->dMainXmax = xMax;
  if ( yMin < hSHP->dMainYmin )  hSHP->dMainYmin = yMin;
  if ( yMax > hSHP->dMainYmax )  hSHP->dMainYmax = yMax;

  if ( hasZ )
  {
    if ( zMin < hSHP->dMainZmin )  hSHP->dMainZmin = zMin;
    if ( zMax > hSHP->dMainZmax )  hSHP->dMainZmax = zMax;
  }

  if ( hasM )
  {
    if ( mMin < hSHP->dMainMmin )  hSHP->dMainMmin = mMin;
    if ( mMax > hSHP->dMainMmax )  hSHP->dMainMmax = mMax;
  }

  if( padfX != NULL )  free( padfX );
  if( padfY != NULL )  free( padfY );
  if( padfZ != NULL )  free( padfZ );
  if( padfM != NULL )  free( padfM );

  if( panPartStart != NULL )  free( panPartStart );
  if( panPartType != NULL )  free( panPartType );


  
//    return( nShapeId  );
}
*/
/*
void SHPFileReaderWriter::SHPSetBounds( uchar * pabyRec,double xMin, double xMax, double yMin, double yMax )
{
  ByteCopy( &(xMin), pabyRec +  0, 8 );
  ByteCopy( &(yMin), pabyRec +  8, 8 );
  ByteCopy( &(xMax), pabyRec + 16, 8 );
  ByteCopy( &(yMax), pabyRec + 24, 8 );

  if( bBigEndian )
  {
    SwapWord( 8, pabyRec + 0 );
    SwapWord( 8, pabyRec + 8 );
    SwapWord( 8, pabyRec + 16 );
    SwapWord( 8, pabyRec + 24 );
  }

  //  Fehlt: Freigeben
}
*/


/************************************************************************/
/*                         DBFWriteAttribute()                          */
/*									                                                    */
/*	Write an attribute record to the file.				                      */
/************************************************************************/
/*
bool SHPFileReaderWriter:: DBFWriteAttribute( int hEntity, int iField, void * pValue )
{
  int	       	    i;
	size_t          j;
  unsigned char	* pabyRec;
  char	          szSField[400], szFormat[20];
  bool            nRetResult = true;

  // -------------------------------------------------------------------- //
  //	Is this a valid record?						                                  //
  // -------------------------------------------------------------------- //
  if( hEntity < 0 || hEntity > hDBF->nRecords )
    return false;

  // -------------------------------------------------------------------- //
  //      Is this a brand new record?                                     //
  // -------------------------------------------------------------------- //

  hDBF->nRecords++;
  for( i = 0; i < hDBF->nRecordLength; i++ )
    hDBF->pszCurrentRecord[i] = ' ';

  hDBF->nCurrentRecord = hEntity;   
  pabyRec = (unsigned char *) hDBF->pszCurrentRecord;


  // -------------------------------------------------------------------- //
  //      Assign all the record fields.                                   //
  // -------------------------------------------------------------------- //
  switch( hDBF->vchFieldTypes[iField] )
  {
  case 'D':
  case 'N':
  case 'F':
    if( hDBF->vnFieldDecimals[iField] == 0 )
    {
      int		nWidth = hDBF->vnFieldSizes[iField];

      if( sizeof(szSField)-2 < nWidth )
        nWidth = sizeof(szSField)-2;

      sprintf( szFormat, "%%%dd", nWidth );
      sprintf( szSField, szFormat, (int) *((double *) pValue) );
      if( (int)strlen(szSField) > hDBF->vnFieldSizes[iField] )
      {
        szSField[hDBF->vnFieldSizes[iField]] = '\0';
        nRetResult = false;
      }

      strncpy((char *) (pabyRec+hDBF->vnFieldOffsets[iField]), szSField, strlen(szSField) );
    }
    else
    {
      int		nWidth = hDBF->vnFieldSizes[iField];

      if( sizeof(szSField)-2 < nWidth )
        nWidth = sizeof(szSField)-2;

      sprintf( szFormat, "%%%d.%df",  nWidth, hDBF->vnFieldDecimals[iField] );
      sprintf(szSField, szFormat, *((double *) pValue) );
      if( (int) strlen(szSField) > hDBF->vnFieldSizes[iField] )
      {
        szSField[hDBF->vnFieldSizes[iField]] = '\0';
        nRetResult = false;
      }
      strncpy((char *) (pabyRec + hDBF->vnFieldOffsets[iField]), szSField, strlen(szSField) );
    }
    break;

  case 'L':
    if ( hDBF->vnFieldSizes[iField] >= 1  && (*(char*)pValue == 'F' || *(char*)pValue == 'T'))
      *(pabyRec+hDBF->vnFieldOffsets[iField]) = *(char*)pValue;
    break;

  default:
    if( (int) strlen((char *) pValue) > hDBF->vnFieldSizes[iField] )
    {
      j = hDBF->vnFieldSizes[iField];
      nRetResult = false;
    }
    else
    {
      memset( pabyRec+hDBF->vnFieldOffsets[iField], ' ', hDBF->vnFieldSizes[iField] );
      j = strlen((char *) pValue);
    }

    strncpy((char *) (pabyRec + hDBF->vnFieldOffsets[iField]), (char *) pValue, j );
    break;
  }

  
  hDBF->fpDBF.write( hDBF->pszCurrentRecord, hDBF->nRecordLength );

  return( nRetResult );
}
*/
///////////////////////////////////////////////////////////////////////////////
//  Schreiben aller Sachdaten eines neuen Features                           //
///////////////////////////////////////////////////////////////////////////////
/*
bool SHPFileReaderWriter::writeShapefileAttributes ( int nShapeId, std::vector<void*> vValues )
{
  for ( unsigned int i = 0; i < vValues.size(); i++ )
    DBFWriteAttribute ( nShapeId, i, vValues[i] );

  return true;
}
*/
///////////////////////////////////////////////////////////////////////////////
//  Schließen des Shapefiles                                                 //
///////////////////////////////////////////////////////////////////////////////
void SHPFileReader::closeShapefile()
{
  DBFClose();
  SHPClose();
}










