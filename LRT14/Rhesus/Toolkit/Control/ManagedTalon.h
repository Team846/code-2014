#ifndef RHESUS_MANAGEDTALON_H_
#define RHESUS_MANAGEDTALON_H_

#include <string>

#include <Talon.h>
#include <DigitalOutput.h>

#include "SpeedController.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Control
{

class ManagedTalon : SpeedController
{
public:
	ManagedTalon(std::string name, UINT32 port, UINT32 jumperChannel);
	ManagedTalon(std::string name, UINT8 moduleNumber, UINT32 port, UINT32 jumperChannel);
	ManagedTalon(std::string name, UINT32 port, UINT8 jumperModuleNumber, UINT32 jumperChannel);
	ManagedTalon(std::string name, UINT8 moduleNumber, UINT32 port, UINT8 jumperModuleNumber, UINT32 jumperChannel);
	~ManagedTalon();
	
	/** Properties **/
	Talon& WrappedTalon()
	{
		return m_talon;
	}
	
	void SetDutyCycle(float f) OVERRIDE
	{
		m_signal = f;
	}
	
	void Update() OVERRIDE;
	
private:
	Talon m_talon;
	DigitalOutput* m_jumper;
	
	float m_signal;
};

}
}
}

#endif
