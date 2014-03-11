#include "PressurePlateData.h"

PressurePlateData::PressurePlateData() :
	ComponentData("PressurePlateData")
{
	ResetCommands();
	m_applyPressure = false;
}

PressurePlateData* PressurePlateData::Get()
{
	return (PressurePlateData*) ComponentData::GetComponentData("PressurePlateData");
}

void PressurePlateData::ResetCommands()
{
}

void PressurePlateData::Log()
{
	LogToFile(&m_applyPressure, "Apply");
}

bool PressurePlateData::GetPressure()
{
	return m_applyPressure;
}

void PressurePlateData::SetPressure(bool apply)
{
	m_applyPressure = apply;
}
