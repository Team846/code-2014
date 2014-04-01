#include "Indicator.h"
#include <Rhesus.Toolkit.IO.h>
#include "../Config/DriverStationConfig.h"

using namespace Rhesus::Toolkit::IO;

Indicator::Indicator():
	Component("Indicator",DriverStationConfig::DigitalIns::NO_DS_DI)
{
	DigitalModule* module = DigitalModule::GetInstance(1);
	m_i2c = module->GetI2C(168);
	m_i2c->SetCompatibilityMode(true);
	m_indicatorData = IndicatorData::Get();
}

void Indicator::OnEnabled()
{
}

void Indicator::OnDisabled()
{
}

void Indicator::UpdateEnabled()
{
	uint8_t shooterPattern = (uint8_t)IndicatorData::FIRING;
	
	BufferedConsole::Printfln("Shooter pattern: %d", (int)shooterPattern);
	
	uint8_t distancePattern = (uint8_t)IndicatorData::FAR;
	uint8_t combinedPattern = (uint8_t)(((unsigned int)(distancePattern) << 4) | (unsigned int)shooterPattern);
	m_i2c->Transaction(&combinedPattern, 1, NULL,0);
}

void Indicator::UpdateDisabled()
{
//	BufferedConsole::Printfln("Indicators disabled.");
	
	uint8_t shooterPattern = (uint8_t)IndicatorData::CLOSE;
	uint8_t distancePattern = (uint8_t)IndicatorData::UNLOADED;
	uint8_t combinedPattern = (uint8_t)(((unsigned int)(distancePattern) << 4) | (unsigned int)shooterPattern);
//	BufferedConsole::Printfln("Sent pattern: %x", combinedPattern);
	
	m_i2c->Transaction(&combinedPattern, 1, NULL, 0);
}

