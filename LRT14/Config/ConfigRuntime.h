#ifndef CONFIG_RUNTINE_H_
#define CONFIG_RUNTINE_H_

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <WPILib.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>

class Configurable;

/*!
 * @brief Provides an interface for accessing robot configuration.
 * @author Raphael Chang, Tony Peng
 */

struct Config
{
	string value;
	list<string>::iterator position;
};
typedef map<string, map<string, Config> > config;

class ConfigRuntime
{
public:
	static ConfigRuntime* Instance();
	static void Finalize();
	
	~ConfigRuntime();
	
	void Load();
	void Save();
	/*!
	 * @brief Returns value of key at section from loaded file. A default value is required for correct type to be returned.
	 */
	template<typename T> T Get(std::string section, std::string key, T defaultValue);
	/*!
	 * @brief Sets value of key at section in memory. Does not save to config file until Save() is called.
	 */
	template<typename T> void Set(std::string section, std::string key, T value);
	/*!
	 * @brief Registers a configurable into configurable list
	 */
	static void Register(Configurable* configurable);
	/*!
	 * @brief Calls Configure() on all registered configurables
	 */
	static void ConfigureAll();
	/*!
	 * @brief Checks if file has been changed
	 */
	void CheckForFileUpdates();
	
private:
	ConfigRuntime();
	static ConfigRuntime* m_instance;
	static std::vector<Configurable*> configurables;
	
	/*!
	 * @brief Loads the config file into memory
	 */
	void LoadConfig(std::string path);
	/*!
	 * @brief Saves config data from memory into config file
	 */
	void SaveConfig(std::string path);
	std::list<string> *fileData;
	config* configData;
	std::map<string, std::list<std::string>::iterator> *sectionMap;
	std::time_t lastReadTime;
	bool KeyExists(std::string section, std::string key);
	
	const static std::string ConfigRuntime::CONFIG_FILE_PATH;
	const static std::string ConfigRuntime::COMMENT_DELIMITERS;
	
	DISALLOW_COPY_AND_ASSIGN(ConfigRuntime);
};

#endif
