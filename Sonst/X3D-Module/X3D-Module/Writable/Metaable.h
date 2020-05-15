#pragma once

#include "DEFable.h"

#include "..\tinyxml2.h"
#include <map>
#include <string>

namespace X3D
{
	class Metaable : public DEFable
	{
	public:
		virtual ~Metaable() {};
		virtual void addMetadataString(const std::string& key, const std::string& val)	{ m_metadataString[key] = val; }
		virtual void addMetadataDouble(const std::string& key, const double& val)		{ m_metadataDouble.insert(std::pair<std::string, double>(key, val)); }
		virtual void addMetadataFloat(const std::string& key, const float& val)			{ m_metadataFloat.insert(std::pair<std::string, float>(key, val)); }
		virtual void addMetadataInt(const std::string& key, const int& val)				{ m_metadataInt.insert(std::pair<std::string, int>(key, val)); }
		virtual void addMetadataBool(const std::string& key, const bool& val)			{ m_metadataInt.insert(std::pair<std::string, bool>(key, val)); }

		virtual std::map<std::string, std::string>	getMetadataString()	const { return m_metadataString; }
		virtual std::map<std::string, double>		getMetadataDouble()	const { return m_metadataDouble; }
		virtual std::map<std::string, float>		getMetadataFloat()	const { return m_metadataFloat; }
		virtual std::map<std::string, int>			getMetadataInt()	const { return m_metadataInt; }
		virtual std::map<std::string, bool>			getMetadataBool()	const { return m_metadataBool; }

		virtual void write(tinyxml2::XMLPrinter& printer);

	private:
		std::map<std::string, std::string>		m_metadataString;
		std::map<std::string, double>			m_metadataDouble;
		std::map<std::string, float>			m_metadataFloat;
		std::map<std::string, int>				m_metadataInt;
		std::map<std::string, bool>				m_metadataBool;
	};
}