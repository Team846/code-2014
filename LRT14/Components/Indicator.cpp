#include "Indicator.h"
#include "../Config/DriverStationConfig.h"

Indicator::Indicator():
	Component("Indicator",DriverStationConfig::DigitalIns::NO_DS_DI)
{
	DigitalModule* module = DigitalModule::GetInstance(2);
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
	char shooterPattern = (char)m_indicatorData->GetShooterPattern();
	char distancePattern = (char)m_indicatorData->GetDistancePattern();
	char combinedPattern = (char)(((unsigned int)(distancePattern) << 4) | (unsigned int)shooterPattern);
	m_i2c->Write(84,combinedPattern);
}

void Indicator::UpdateDisabled()
{
	char shooterPattern = (char)IndicatorData::SHOOTER_OFF;
	char distancePattern = (char)IndicatorData::DISTANCE_OFF;
	char combinedPattern = (char)(((unsigned int)(distancePattern) << 4) | (unsigned int)shooterPattern);
	m_i2c->Write(84,combinedPattern);
}

