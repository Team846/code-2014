#include "Automation.h"
#include "../../ComponentData/CollectorRollersData.h"
#include "../../ComponentData/CollectorArmData.h"
#include "../../Sensors/DriveEncoders.h"

#include "../../Config/Configurable.h"

class Dribble: public Automation, public Configurable
{
public:
	Dribble();
	
	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

	void Configure();
private:
	float m_dribbleSpeed;
	CollectorArmData* m_armData;
	CollectorRollersData* m_rollerData;
	DriveEncoders* m_encoders;
	
};
