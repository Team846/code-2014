#ifndef RHESUS_CONFIG_H_
#define RHESUS_CONFIG_H_

#include "../Defines.h"

#include <map>
#include <hash_map>
#include <string>

#ifdef RHESUS_NO_THROW
#include "../error_val.hpp"
#endif

using namespace std;

typedef hash_map<string, string> strMap;
typedef hash_map<string, strMap> configMap;

namespace Rhesus
{
namespace Toolkit
{
namespace Configuration
{

	/*!
	 * @brief Provides an object to store the configurations
	 * @author Varun Parthasarathy
	 */
	class Config 
	{
	public:
		Config();
		
		void SetDefaultSection(string sectionId);
		
		template<typename T> T Get(string section, string key);
		template<typename T> T Get(string key);

		void Set(string section, string key, string value);
		void Set(string key, string value);
		void AddSection(string section);
		
		//ConfigLoader function
		static Config Load(string filepath);
		
	private:
		
		static std::string kDefaultSection;
		
		bool SectionExists(string section);
		bool KeyExists(string section, string key);
		
		//hash_map<string, hash_map<string, string> > m_configMap;
		configMap m_configMap;
		string m_defaultSection;
		
		//ConfigLoader function
		static string RemoveComments(string s);
	};

}
}
}

#endif
