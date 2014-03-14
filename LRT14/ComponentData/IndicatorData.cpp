#include "IndicatorData.h"

IndicatorData::IndicatorData():
	ComponentData("IndicatorData")
{
	m_distancePattern = IndicatorData::DISTANCE_OFF;
	m_shooterPattern = IndicatorData::SHOOTER_OFF;
}

IndicatorData* IndicatorData::Get()
{
	return (IndicatorData*) ComponentData::GetComponentData("IndicatorData");
}

void IndicatorData::SetDistancePattern(Distance pattern)
{
	m_distancePattern = pattern;
}

void IndicatorData::SetShooterPattern(Shooter pattern)
{
	m_shooterPattern = pattern;
}

IndicatorData::Distance IndicatorData::GetDistancePattern()
{
	return m_distancePattern;
}

IndicatorData::Shooter IndicatorData::GetShooterPattern()
{
	return m_shooterPattern;
}
