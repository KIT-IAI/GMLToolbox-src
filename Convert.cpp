#include "StdAfx.h"
#include ".\convert.h"

#include <string.h>
#include <vcclr.h>
using namespace System;

using namespace System::Runtime::InteropServices;

///////////////////////////////////////////////////////////////////////////////
//  QuConvert                                                                //
//  Verschiedene Konvertierungs-Routinen für std::string                     //
///////////////////////////////////////////////////////////////////////////////

QuConvert::QuConvert(void)
{
}

QuConvert::~QuConvert(void)
{
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertierung std::string --> String ^                                   //
///////////////////////////////////////////////////////////////////////////////
String ^ QuConvert::ToString(std::string str)
{
	System::String ^ systemStr = gcnew System::String(str.c_str());
	return(systemStr);
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertierung std::string --> double                                     //
///////////////////////////////////////////////////////////////////////////////
double QuConvert::ToDouble( std::string str)
{
  System::String ^ systemStr = gcnew System::String(str.c_str());
  double           d;

  try
  {
    d = Convert::ToDouble( systemStr );
  }
 
  catch ( System::Exception ^ )
  {
    d = 0.0;
  }

  return d;
}


///////////////////////////////////////////////////////////////////////////////
//  Konvertierung double --> String ^ mit Dezimalpunkt                       //
///////////////////////////////////////////////////////////////////////////////
String ^ QuConvert::ToString( double d )
{
	return XmlConvert::ToString ( d );
}

String ^ QuConvert::ToString( double d, int precision )
{
	double   h = 5.0;
	int	     index, indexExp, length;
	String ^ STR;
  String ^ ERGEBNIS;

	if ( precision <= 0 ) return XmlConvert::ToString ( d );
	
	STR = XmlConvert::ToString ( d );
  index = STR->IndexOf ( "." );
  indexExp = STR->IndexOf ( "E" );
	if ( index == -1 || indexExp >= 0 ) 
		return STR;

  length  = STR->Length;
	if ( index + precision + 1 > length ) return STR;

  ERGEBNIS = STR->Substring ( 0, index +  precision + 1 );
 
  if ( indexExp >= 0 )
    ERGEBNIS = String::Concat( ERGEBNIS, STR->Substring ( indexExp, length-indexExp ) );

  return ERGEBNIS;
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertierung String ^  --> std::string                                  //
///////////////////////////////////////////////////////////////////////////////
void QuConvert::systemStr2stdStr(std::string& toStr, System::String ^ fromStr)
{
  if (!fromStr)
  {
    toStr.clear();
    return;
  }
	IntPtr intPtr = Marshal::StringToHGlobalAnsi(fromStr);
    char* pStr;
	

	void* p = intPtr.ToPointer();
	pStr = (char*)p;
	toStr = pStr;

	Marshal::FreeCoTaskMem(intPtr);

}

//////////////////////////////////////////////////////////////////////////////////
//  Konvertierung std::string --> String ^ mit html-Codierung von Sonderzeichen //
//////////////////////////////////////////////////////////////////////////////////
String ^ QuConvert::ToHtml ( std::string str )
{
	size_t pos;

	pos = str.find ( "ä" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&auml;" );
		pos = str.find ( "ä" );
	}


	pos = str.find ( "Ä" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&Auml;" );
		pos = str.find ( "Ä" );
	}

	pos = str.find ( "ü" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&uuml;" );
		pos = str.find ( "ü" );
	}


	pos = str.find ( "Ü" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&Uuml;" );
		pos = str.find ( "Ü" );
	}


	pos = str.find ( "ö" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&ouml;" );
		pos = str.find ( "ö" );
	}


	pos = str.find ( "Ö" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&Ouml;" );
		pos = str.find ( "Ö" );
	}


	pos = str.find ( "ß" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&szlig;" );
		pos = str.find ( "ß" );
	}


	pos = str.find ( "„" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&#8222;" );
		pos = str.find ( "„" );
	}
	

	pos = str.find ( "“" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&#8220;" );
		pos = str.find ( "“" );
	}

	pos = str.find ( "–" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&#8211;" );
		pos = str.find ( "–" );
	}

	pos = str.find ( "\"" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&quot;" );
		pos = str.find ( "\"" );
	}

	pos = str.find ( "§" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&sect;" );
		pos = str.find ( "§" );
	}

	pos = str.find ( "²" );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "&sup2;" );
		pos = str.find ( "²" );
	}

	return ToString ( str );
}

///////////////////////////////////////////////////////////////////////////////
//  Konvertierung std::string --> String ^ mit Umwandlung von Sonderzeichen  //
//  in ASCII Characters                                                      //
///////////////////////////////////////////////////////////////////////////////
std::string QuConvert::ToAscii ( std::string str )
{
	size_t pos;

  pos = str.find ( "ä" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "ae" );
    pos = str.find ( "ä" );
  }


  pos = str.find ( "Ä" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "Ae" );
    pos = str.find ( "Ä" );
  }

  pos = str.find ( "ü" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "ue" );
    pos = str.find ( "ü" );
  }


  pos = str.find ( "Ü" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "Ue" );
    pos = str.find ( "Ü" );
  }


  pos = str.find ( "ö" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "oe" );
    pos = str.find ( "ö" );
  }


  pos = str.find ( "Ö" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "Oe" );
    pos = str.find ( "Ö" );
  }


  pos = str.find ( "ß" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "ss" );
    pos = str.find ( "ß" );
  }


  pos = str.find ( "„" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "" );
    pos = str.find ( "„" );
  }


  pos = str.find ( "“" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "" );
    pos = str.find ( "“" );
  }

  pos = str.find ( "–" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "" );
    pos = str.find ( "–" );
  }

	pos = str.find ( " " );
	while ( pos != string::npos )
	{
		str.replace ( pos, 1, "_" );
		pos = str.find ( " " );
	}

  pos = str.find ( "\"" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "" );
    pos = str.find ( "\"" );
  }

  pos = str.find ( "§" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "" );
    pos = str.find ( "§" );
  }

  pos = str.find ( "²" );
  while ( pos != string::npos )
  {
    str.replace ( pos, 1, "" );
    pos = str.find ( "²" );
  }

  return str;
}