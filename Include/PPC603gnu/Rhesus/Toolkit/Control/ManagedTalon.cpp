#include "ManagedTalon.h"

using namespace Rhesus::Toolkit::Control;

ManagedTalon::ManagedTalon(std::string name, UINT32 port, UINT32 jumperChannel)
	: SpeedController(name),
	  m_talon(port),
	  m_jumper(jumperChannel > 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	
}

ManagedTalon::ManagedTalon(std::string name, UINT8 moduleNumber, UINT32 port, UINT32 jumperChannel)
	: SpeedController(name),
	  m_talon(moduleNumber, port),
	  m_jumper(jumperChannel > 0 ? new DigitalOutput(jumperChannel) : NULL)
{
	
}

ManagedTalon::ManagedTalon(std::string name, UINT32 port, UINT8 jumperModuleNumber, UINT32 jumperChannel)
	: SpeedController(name),
	  m_talon(port),
	  m_jumper(jumperChannel > 0 ? new DigitalOutput(jumperModuleNumber, jumperChannel) : NULL)
{
	
}

ManagedTalon::ManagedTalon(std::string name, UINT8 moduleNumber, UINT32 port, UINT8 jumperModuleNumber, UINT32 jumperChannel)
	: SpeedController(name),
	  m_talon(moduleNumber, port),
	  m_jumper(jumperChannel > 0 ? new DigitalOutput(jumperModuleNumber, jumperChannel) : NULL)
{
	
}

ManagedTalon::~ManagedTalon()
{
	DELETE(m_jumper);
}

void ManagedTalon::Update()
{
	m_talon.Set(m_signal);
}
