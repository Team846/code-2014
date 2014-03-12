#include "Automation.h"
#include "../../ComponentData/CollectorRollersData.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../Sensors/SensorFactory.h"
#include "../../Sensors/DriveEncoders.h"
#include "GearTooth.h"

#include "../../Config/Configurable.h"
#include "../../Config/ConfigPortMappings.h"

class Dribble : public Automation, public Configurable
{
public:
	Dribble();
	
	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

	void Configure();
private:
	float m_rollerMaxSurfaceSpeed;
	float m_rollerMaxSpeed;
	float m_initialSpeed;
	float m_ballSpeedDropThreshold;
	float m_extraScale;
	float m_offset;
	GearTooth* m_gearTooth;
	CollectorArmData* m_armData;
	CollectorRollersData* m_rollerData;
	DriveEncoders* m_encoders;
};
