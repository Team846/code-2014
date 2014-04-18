#ifndef BALL_PUSHER_H_
#define BALL_PUSHER_H_

#include "Component.h"
#include "../Config/Configurable.h"
#include "../ComponentData/BallPusherData.h"
#include "../Actuators/Pneumatics.h"
#include "../Sensors/SensorFactory.h"

/*!
 * @brief Provides control over the ball pushing mechanism.
 */
class BallPusher : public Component
{
public:
	BallPusher();
	~BallPusher();
	
	void OnEnabled();
	void OnDisabled();
	
	void UpdateEnabled();
	void UpdateDisabled();
	
private:
	BallPusherData* m_ballPusherData;
	Pneumatics* m_pneumatics;
};

#endif
