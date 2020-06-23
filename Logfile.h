#pragma once
#include <string>
#include <fstream>

///////////////////////////////////////////////////////////////////////////////
//  Textausgaben auf einen zentralen Logfile                                 //
///////////////////////////////////////////////////////////////////////////////
class Logfile
{
public:
  Logfile  ();
  virtual ~ Logfile();

protected:
  std::ofstream logFile;   //  Logfile

public:
  void writeLogFile ( std::string textP );
};


///////////////////////////////////////////////////////////////////////////////
//  Textausgaben auf einen lokale Logfile                                    //
///////////////////////////////////////////////////////////////////////////////
class LogfileLokal
{
public:
  LogfileLokal  ( std::string pathName );
  virtual ~LogfileLokal();

protected:
  std::ofstream logFile;   //  Logfile

public:
  void writeLogFile ( std::string textP );
};




    
    
    
