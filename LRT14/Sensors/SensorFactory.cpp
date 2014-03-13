#include "SensorFactory.h"

#include <Rhesus.Toolkit.h>
#include <Rhesus.Toolkit.Utilities.h>

using namespace Rhesus::Toolkit;
using namespace Rhesus::Toolkit::Utilities;

map<uint32_t, AnalogChannel*> SensorFactory::m_analog;
map<uint32_t, DigitalInput*> SensorFactory::m_digital;
map<uint32_t, Counter*> SensorFactory::m_counters;
map<pair<uint32_t, uint32_t>, LRTEncoder*> SensorFactory::m_encoders;
HotGoal* SensorFactory::m_hotGoal;
SensorFactory *SensorFactory::m_instance = NULL;

void SensorFactory::Initialize()
{
	m_instance = new SensorFactory();
}

void SensorFactory::Finalize()
{
	for (map<uint32_t, AnalogChannel*>::iterator it = m_analog.begin(); it != m_analog.end(); it++)
	{
	   map<uint32_t, AnalogChannel*>::iterator erase = it;
	   ++it;
	   ContainerCleanup::DeleteMapSecond(*erase);
	   m_analog.erase(erase);
	}
	for (map<uint32_t, DigitalInput*>::iterator it = m_digital.begin(); it != m_digital.end(); it++)
	{
	   map<uint32_t, DigitalInput*>::iterator erase = it;
	   ++it;
	   ContainerCleanup::DeleteMapSecond(*erase);
	   m_digital.erase(erase);
	}
	for (map<uint32_t, Counter*>::iterator it = m_counters.begin(); it != m_counters.end(); it++)
	{
	   map<uint32_t, Counter*>::iterator erase = it;
	   ++it;
	   ContainerCleanup::DeleteMapSecond(*erase);
	   m_counters.erase(erase);
	}
	for (map<pair<uint32_t, uint32_t>, LRTEncoder*>::iterator it = m_encoders.begin(); it != m_encoders.end(); it++)
	{
	   map<pair<uint32_t, uint32_t>, LRTEncoder*>::iterator erase = it;
	   ++it;
	   ContainerCleanup::DeleteMapSecond(*erase);
	   m_encoders.erase(erase);
	}
	
	R_DELETE(m_hotGoal);
	
	R_DELETE(m_instance);
}

AnalogChannel* SensorFactory::GetAnalogChannel(uint32_t port)
{
	if (m_analog.find(port) == m_analog.end())
	{
		m_analog[port] = new AnalogChannel(port);
	}
	return m_analog[port];
}

DigitalInput* SensorFactory::GetDigitalInput(uint32_t port)
{
	if (m_digital.find(port) == m_digital.end())
	{
		m_digital[port] = new DigitalInput(port);
	}
	return m_digital[port];
}

Counter* SensorFactory::GetCounter(uint32_t port)
{
	if (m_counters.find(port) == m_counters.end())
	{
		m_counters[port] = new Counter(port);
		m_counters[port]->Start();
	}
	return m_counters[port];
}

LRTEncoder* SensorFactory::GetLRTEncoder(std::string name, uint32_t portA, uint32_t portB)
{
	if (m_encoders.find(pair<uint32_t, uint32_t>(portA, portB)) == m_encoders.end())
	{
		m_encoders[pair<uint32_t, uint32_t>(portA, portB)] = new LRTEncoder(name, portA, portB);
		m_encoders[pair<uint32_t, uint32_t>(portA, portB)]->Start();
	}
	return m_encoders[pair<uint32_t, uint32_t>(portA, portB)];
}

GearTooth* SensorFactory::GetGearTooth(uint32_t port)
{
	if (m_counters.find(port) == m_counters.end())
	{
		m_counters[port] = new GearTooth(port);
		m_counters[port]->Start();
	}
	return (GearTooth*)m_counters[port];
}

HotGoal* SensorFactory::GetHotGoal()
{
	if(m_hotGoal == NULL) m_hotGoal = new HotGoal();
	
	return m_hotGoal;
}

void SensorFactory::Send()
{
	for (map<uint32_t, AnalogChannel*>::iterator it = m_analog.begin(); it != m_analog.end(); it++)
	{
		SendToNetwork(it->second->GetAverageValue(), "Analog" + lexical_cast(it->first), "SensorData");
	}
	for (map<uint32_t, DigitalInput*>::iterator it = m_digital.begin(); it != m_digital.end(); it++)
	{
		SendToNetwork(it->second->Get(), "Digital" + lexical_cast(it->first), "SensorData");
	}
	for (map<uint32_t, Counter*>::iterator it = m_counters.begin(); it != m_counters.end(); it++)
	{
		SendToNetwork(it->second->GetPeriod(), "Counter" + lexical_cast(it->first), "SensorData");
	}
	for (map<pair<uint32_t, uint32_t>, LRTEncoder*>::iterator it = m_encoders.begin(); it != m_encoders.end(); it++)
	{
		SendToNetwork(it->second->GetRate(), "EncoderRate" + lexical_cast(it->first.first) + "," + lexical_cast(it->first.second), "SensorData");
		SendToNetwork(it->second->Get(), "EncoderDistance" + lexical_cast(it->first.first) + "," + lexical_cast(it->first.second), "SensorData");
	}
}
