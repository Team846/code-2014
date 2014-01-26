#include "Config.h"

#include "../Utilities/StringUtil.hpp"

#include <sstream>
#include <fstream>
#include <vector>

using namespace Rhesus;
using namespace Rhesus::Toolkit;
using namespace Rhesus::Toolkit::Configuration;
using namespace Rhesus::Toolkit::Utilities;

std::string Config::kDefaultSection = "default";

Config::Config()
{
	m_defaultSection = kDefaultSection;
} 

void Config::SetDefaultSection(string sectionId)
{
	m_defaultSection = sectionId;
}

void Config::AddSection(string section)
{
	m_configMap.insert(make_pair(section, strMap()));
}

void Config::Set(string section, string key, string value)
{
	if(SectionExists(section)) {
		m_configMap[section].insert(make_pair(key, value));
	} else {
#ifndef RHESUS_NO_THROW
		throw exception();
#endif
	}
	
}

void Config::Set(string key, string value)
{
	if(!SectionExists(m_defaultSection)) {
		AddSection(m_defaultSection);
	}
	
	m_configMap[m_defaultSection].insert(make_pair(key, value));
}

bool Config::KeyExists(string section, string key)
{	
	if(SectionExists(section)) {
		return m_configMap[section].find(key) != m_configMap[section].end();
	}
	
	return false;	
}

bool Config::SectionExists(string section)
{	
	return m_configMap.find(section) != m_configMap.end();	
}

template<typename T> T Config::Get(string section, string key)
{
	if (KeyExists(section, key))
	{
		stringstream sstream(m_configMap[section][key]);
		T value;
		sstream >> value;
		return value;
		
	} else {
#ifndef RHESUS_NO_THROW
		throw exception();
#endif
	}
}

template<typename T> T Config::Get(string key)
{
	if (KeyExists(m_defaultSection, key))
	{
		stringstream sstream(m_configMap[m_defaultSection][key]);
		T value;
		sstream >> value;
		return value;

	} else {
#ifndef RHESUS_NO_THROW
		throw exception();
#endif
	}
}

//ConfigLoader functions

string Config::RemoveComments(string s) 
{
	string ret;
	
	for(UINT32 i = 0; i < s.length(); i++) {
		
		if(s.at(i) == '#' || s.at(i) == ';') {
			break;
		} else if(i != (s.length() - 1)) {
			if(s.at(i) == '/' && s.at(i + 1) == '/')
				break;
		}
		
		ret.push_back(s.at(i));
		
	}
	
	return ret;
}

Config Config::Load(string filepath)
{
	ifstream cf(filepath.c_str());

	if (!cf.is_open())
	{
#ifndef RHESUS_NO_THROW
		throw exception();
#else
		error_val<Config> err;
		
		return err();
#endif
	}

	Config ret = Config();
	
	string currentSection = kDefaultSection;
	
	while (!cf.eof())
	{
		string line;
		getline(cf, line);
		
		if(line.at(0) == '[') {
			currentSection = line.substr(1, line.length() - 2);
			ret.AddSection(currentSection);
		} else if(line.length() == 0 || line.at(0) == '#' || line.at(0) == ';' || 
				(line.at(0) == '/' && line.at(1) == '/')) {
			continue;
		} else {
			
			line = StringUtil::Trim(line);
			line = RemoveComments(line);
			std::vector<string> t_list = StringUtil::Split(line, '=');
			
			if(currentSection == "def") {
				ret.Set(t_list.front(), t_list.back());
			} else {
				ret.Set(currentSection, t_list.front(), t_list.back());
			}
			
		}
	}
	
	cf.close();

	return ret;
}
