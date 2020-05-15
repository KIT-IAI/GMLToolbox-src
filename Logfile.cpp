#include "StdAfx.h"
#include "constant.h"
#include "Logfile.h"

///////////////////////////////////////////////////////////////////////////////
//  Textausgaben auf einen zentralen Logfile                                 //
///////////////////////////////////////////////////////////////////////////////
Logfile::Logfile  ()
{
  std::string logfilePath = Constant::getLogfilePath() ;
  logFile.open ( logfilePath.c_str() );
}

Logfile::~Logfile()
{
  logFile.close();
}

void Logfile::writeLogFile ( std::string textP )
{
  logFile << textP << std::endl;

}

///////////////////////////////////////////////////////////////////////////////
//  Textausgaben auf einen lokalen Logfile                                   //
///////////////////////////////////////////////////////////////////////////////
LogfileLokal::LogfileLokal  ( std::string pathName )
{
  logFile.open ( pathName.c_str() );
}

LogfileLokal::~LogfileLokal()
{
  logFile.flush();
  logFile.close();
}

void LogfileLokal::writeLogFile ( std::string textP )
{
  logFile << textP << std::endl;

}


