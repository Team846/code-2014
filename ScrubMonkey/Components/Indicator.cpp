#include "Indicator.h"
#include "../Config/DriverStationConfig.h"

Indicator::Indicator():
	Component("Indicator",DriverStationConfig::DigitalIns::NO_DS_DI)
{
	DigitalModule* module = DigitalModule::GetInstance(4);
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
	uint8_t distancePattern = (uint8_t)IndicatorData::CLOSE;
	uint8_t combinedPattern = (uint8_t)(((unsigned int)(distancePattern) << 4) | (unsigned int)shooterPattern);
	m_i2c->Transaction(&combinedPattern, 1, NULL,0);
}

void Indicator::UpdateDisabled()
{
	uint8_t shooterPattern = (uint8_t)IndicatorData::SHOOTER_OFF;
	uint8_t distancePattern = (uint8_t)IndicatorData::DISTANCE_OFF;
	uint8_t combinedPattern = (uint8_t)(((unsigned int)(distancePattern) << 4) | (unsigned int)shooterPattern);
	m_i2c->Transaction(&combinedPattern, 1, NULL,0);
}

