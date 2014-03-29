#ifndef RHESUS_CONFIG_H_
#define RHESUS_CONFIG_H_

#include "../Defines.h"

#include <map>
#include <hash_map>
#include <string>

#ifdef RHESUS_NO_THROW
#include "../error_val.h"
#endif

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
		
		/*!
		 * Sets the section that is accessed if none is specified
		 * @param sectionId the name of the default section
		 */
		void SetDefaultSection(std::string sectionId);
		
		/*!
		 * @return The value at the specified key within the specified section
		 * @param section the section in which to search for the given key
		 * @param the key associated with the requested value
		 */
		template<typename T> T Get(std::string section, std::string key);
		
		/*!
		 * @return The value at the specified key within the default section
		 * @param the key associated with the requested value
		 */
		template<typename T> T Get(std::string key);

		/*!
		 * sets the specified value at the specified location
		 * @param section the section in which the desired key is located
		 * @param key the key for the value to change
		 * @value the desired value to be set
		 */
		void Set(std::string section, std::string key, std::string value);
		
		/*!
		 * sets the value to a specific key within the default section
		 * @param key the kay at which the desired value is located
		 * @param value the value that is to be set
		 */
		void Set(std::string key, std::string value);
		
		/*!
		 * Adds a configuration section
		 * @param section the name of the section to be added
		 */
		void AddSection(std::string section);
		
		/*!
		 * static ConfigLoader function: used to load data from a file into the config object
		 * @param filepath the path to the config file to be loaded
		 * @return a new Config object populated with data from the specified file
		 */
		static Config Load(std::string filepath);
		
	private:
		
		typedef std::hash_map<std::string, std::string> strMap;
		typedef std::hash_map<std::string, strMap> configMap;
		
		static std::string kDefaultSection;
		
		bool SectionExists(std::string section);
		bool KeyExists(std::string section, std::string key);
		
		//hash_map<string, hash_map<string, string> > m_configMap;
		configMap m_configMap;
		std::string m_defaultSection;
		
		//ConfigLoader function
		static std::string RemoveComments(std::string s);
	};

}
}
}

#endif
