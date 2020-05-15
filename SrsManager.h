#pragma once

#include <gps/gps.hpp>

class     GeoPoint;
class CoordSystem;

///////////////////////////////////////////////////////////////////////////////
//  SrsManager                                                               //
//  Verwaltung von Koordinaten-Referenzsystemen                              //
///////////////////////////////////////////////////////////////////////////////
class SrsManager
{
  public:
    SrsManager();
    virtual ~SrsManager();

  protected:
    std::map<std::string,CoordSystem*>  m_CoordSystems;
    std::map<std::string,CoordSystem*> m_CoordSystemsUsed;

  public:
    void readCoordSystemList ( std::string fileName );
    bool addCoordSystem ( CoordSystem * coordSystem );

    CoordSystem * getCoordSystem ( std::string srsName );
    bool existCoordSystem ( std::string srsName );

    int           getCoordSystemInternalCode ( std::string srsName );

		size_t getCoordSystems ( std::vector<CoordSystem*> &vCoordSystems );

    int getSrsDimension( std::string srsNameShort );

    static std::string makeLowerString ( std::string const & s );
    static std::string makeUpperString ( std::string const & s );
};


///////////////////////////////////////////////////////////////////////////////
//  KoordSystem                                                              //
//  Beschreibung eines Koordinaten-Referenzsystems                           //
///////////////////////////////////////////////////////////////////////////////
class CoordSystem
{
  public:
    CoordSystem();
    virtual ~CoordSystem();

  protected:
    std::string  srsNameShort;
    int          internalCode;
    std::string  srsNameFull;
    int          srsDimension;
    std::string  description;
    std::string  coordSystemType;
    std::string  country;

  public:
    void setSrsNameShort ( std::string s ) {  srsNameShort = s;  }
    std::string getSrsNameShort() {  return srsNameShort;  }

    void setInternalCode ( int i )  {  internalCode = i;  }
    int getInternalCode()  {  return internalCode;  }

    void setSrsNameFull ( std::string s )  {  srsNameFull = s;  }
    std::string getSrsNameFull ()  {  return srsNameFull; }

    void setDescription ( std::string s )  {  description = s;  }
    std::string getDescription ()  {  return description;  }

    void setSrsDimension ( int i )  {  srsDimension = i;  }
    int getSrsDimension()  {  return srsDimension;  }

    void setCoordSystemType ( std::string s  )  { coordSystemType = s;  }
    std::string getCoordSystemtype()  {  return coordSystemType;  }

    void setCountry ( std::string s )  {  country = s;  }
    std::string getCountry()  {  return country;  }

};

