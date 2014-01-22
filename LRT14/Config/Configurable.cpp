#include "Configurable.h"

Configurable::Configurable(string configSection)
:m_config(ConfigRuntime::Instance())
{
	m_configSection = configSection;
	ConfigRuntime::Register(this);
}

Configurable::~Configurable()
{
	
}
