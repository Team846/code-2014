#ifndef BALL_HOLDER_DATA_H_
#define BALL_HOLDER_DATA_H_

#include "ComponentData.h"

/*!
 * @brief Contains commands and states for the ball holder component.
 */
class BallHolderData : public ComponentData
{
public:
	BallHolderData();
	static BallHolderData* Get();
	
	bool GetHold();
	void SetHold(bool hold);
	
	void ResetCommands();
	void Log();
	
private:
	bool m_hold;
	
	friend class BallHolder;
};

#endif

