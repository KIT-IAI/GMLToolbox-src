#pragma once

#include "Writable.h"
#include "..\tinyxml2.h"

#include <string>
#include <map>

namespace X3D
{
  class IDable : public Writable
  {
    public:

      virtual ~IDable() {  };

      virtual void setID(const std::string& id);
      virtual void setName(const std::string& nam)	{ m_name = nam; }
      virtual void setClass(const std::string& cl)	{ m_class = cl; }

      /*virtual void addSceneGraphMap()	{ m_mSceneGraph[this->getID()] = this; }
      virtual void addX3dMap()			{ m_mX3d[this->getID()] = this; }*/

      virtual std::string				getID()				const { return m_id; }
      virtual std::string				getName()			const { return m_name; }
      virtual std::string				getClass()			const { return m_class; }
      /*virtual std::map<std::string, IDable*>	getSceneGraphMap()	const { return m_mSceneGraph; }
      virtual std::map<std::string, IDable*>	getX3dMap()			const { return m_mX3d; }*/

      //virtual IDable* getElementById(const std::string ident);

      virtual void write(tinyxml2::XMLPrinter& printer);

    protected:
      std::string m_id;
      std::string m_class;
      std::string m_name;

      /*std::map<std::string, IDable*>	m_mSceneGraph;
      std::map<std::string, IDable*>	m_mX3d;*/
  };
}