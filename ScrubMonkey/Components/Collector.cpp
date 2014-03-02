#include "Collector.h"

#include "../Config/DriverStationConfig.h"

Collector::Collector()
	: Component("Collector", DriverStationConfig::DigitalIns::COLLECTOR),
	  m_collector(6, "collector")
{
	m_collectorData = CollectorData::Get();
}
	
Collector::~Collector()
{
	
}

void Collector::UpdateEnabled()
{
	int dir = (int)m_collectorData->GetDirection();
	
	if(m_collectorData->IsEnabled())
	{
		m_collector.SetDutyCycle(dir * 1.0f);
	}
	else
	{
		m_collector.SetDutyCycle(0.0f);
	}
}

void Collector::UpdateDisabled()
{
	m_collector.SetDutyCycle(0.0);
}

void Collector::OnEnabled()
{
	
}

void Collector::OnDisabled()
{
	m_collector.SetDutyCycle(0.0);
}
