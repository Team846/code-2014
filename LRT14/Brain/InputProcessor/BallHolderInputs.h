#ifndef BALL_HOLDER_INPUTS_H_
#define BALL_HOLDER_INPUTS_H_

#include "InputProcessor.h"
#include "../../ComponentData/BallHolderData.h"

/*!
 * @brief Processes joystick information for controlling the ball holder.
 */
class BallHolderInputs : public InputProcessor
{
public:
	BallHolderInputs();
	
	void Update();
	
private:
	DebouncedJoystick* m_operator_stick;
	BallHolderData* m_ballHolder;
};

#endif
