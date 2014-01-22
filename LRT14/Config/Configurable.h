#ifndef CONFIGURABLE_H_
#define CONFIGURABLE_H_

#include <string>
#include "ConfigRuntime.h"

/*!
 * @brief Interface for reading from configuration file through ConfigManager.
 * @author Raphael Chang
 */

class Configurable
{
public:
	Configurable(std::string configSection);
	virtual ~Configurable();
	
	/*!
	 * @brief Overload this function to apply new configuration when configuration is loaded
	 */
	virtual void Configure() = 0;
	
	template<typename T> inline T GetConfig(std::string key, T defaultValue)
	{
		return m_config->Get<T>(m_configSection, key, defaultValue);
	}
	
private:
	ConfigRuntime * const m_config;
	std::string m_configSection;
};

#endif
