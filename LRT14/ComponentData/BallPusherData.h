#ifndef BALL_PUSHER_DATA_H_
#define BALL_PUSHER_DATA_H_

#include "ComponentData.h"

/*!
 * @brief Contains commands and states for the ball pusher component.
 */
class BallPusherData : public ComponentData
{
public:
	BallPusherData();
	static BallPusherData* Get();
	
	bool GetPush();
	void SetPush(bool push);
	
	void ResetCommands();
	void Log();
	
private:
	bool m_push;
	
	friend class BallPusher;
};

#endif

