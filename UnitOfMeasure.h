#pragma once

class UOM;

///////////////////////////////////////////////////////////////////////////////
//  UOMList                                                                  //
//  Liste/Map von Unit of Measure Beschreibungen                             //
///////////////////////////////////////////////////////////////////////////////
class UOMList
{
public:
  UOMList();
  virtual ~UOMList();

private:
  std::string                version;             // Version der Lists
  std::string                description;         //  Beschreibung
  std::vector<UOM*>          vUOM;                //  Liste aller UOM-Objekte
  std::map<std::string,UOM*> mUOMFromIdentifier;  //  Zuordnung UOM-Identifier --> UOM-Objekt
  std::map<std::string,UOM*> mUOMFromName;        //  Zuordnung UOM-Name --> UOM-Objekt

public:
  UOM * getUOMFromIdentifier ( std::string identifier );
  UOM * getUOMFromName ( std::string name );

  UOM * getUomAngle();  
  UOM * getUomLength();
  UOM * getUomArea(); 
  UOM * getUomVolume(); 

  bool addUOM ( UOM * pUOM );

  void setDescription ( std::string str )  {   description = str;  }
  void setVersion ( std::string str )      {  version = str;       }
};

///////////////////////////////////////////////////////////////////////////////
//  UOM                                                                      //
//  Unit of Measure (uom) Beschreibung                                      //
///////////////////////////////////////////////////////////////////////////////
class UOM
{
public:
  UOM();
  virtual ~UOM();

private:
  std::string              name;              // Eindeutiger Name der uom
  std::string              description;       // Beschreibung
  std::string              identifier;        // Eindeutiger Identifikator, entspricht dem uom-Attribut des gml:MeasureType
  std::string              quantity;          // Physikalische Größe, z.B. LENGTH, ENERGY
  std::string              sign;              // Maßeinheit als lesbarer Text
  double                   factor;            // Faktor für die Umrechnung verschiedenen Maßeinheiten für dieselbe physikal. Größe
  double                   offset;            // Summand für die Umrechnung verschiedenen Maßeinheiten für dieselbe physikal. Größe
  std::vector<std::string> alternativeIdentifiers; // Weitere eindeutige Identifikatoren

public:
  void setName ( std::string n )  {  name = n;  }
  std::string getName()  {  return name;  }

  void setDescription ( std::string d )  {  description = d;  }
  std::string getDescription()  {  return description;  }

  void setIdentifier ( std::string id )  {  identifier = id;  }
  std::string getIdentifier()  {  return identifier;  }

  void setQuantity ( std::string id )  {  quantity = id;  }
  std::string getQuantity()  {  return quantity;  }

  void setSign ( std::string s )   {  sign = s;  }
  std::string getSign()  {  return sign; }

  void setFactor ( double d )  {  factor = d;  }
  double getFactor ()  {  return factor;  }

  void setOffset ( double o )  {  offset = o;  }
  double getOffset ()  {  return offset;  }

  void addAlternativeIdentifier ( std::string id )  {  alternativeIdentifiers.push_back ( id );  }

	size_t getAlternativeIdentifierAnz() {  return alternativeIdentifiers.size();  }
  std::string getAlternativeIdentifier ( int index )  {  return alternativeIdentifiers[index];  }

  bool changeUOM ( double valueOld, double &valueNew, UOM * pUomNew );
};

