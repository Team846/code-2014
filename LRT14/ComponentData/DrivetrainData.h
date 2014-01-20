#ifndef DRIVETRAIN_DATA_H_
#define DRIVETRAIN_DATA_H_

#include "ComponentData.h"

class DrivetrainData : public ComponentData
{
public:
	enum ControlMode
	{
		POSITION_CONTROL, VELOCITY_CONTROL, OPEN_LOOP
	};

	enum Axis
	{
		FORWARD = 0, TURN = 1
	};

	enum Side
	{
		LEFT = 0, RIGHT = 1
	};
	
	DrivetrainData();
	static DrivetrainData* Get();
	void ResetCommands();
	void Log();
	
	void SetControlMode(Axis axis, ControlMode mode);
	void SetVelocitySetpoint(Axis axis, float velocity);
	void SetOpenLoopOutput(Axis axis, double setpoint);
	void SetRelativePositionSetpoint(Axis axis, double setpoint);
	void SetPositionControlMaxSpeed(Axis axis, double speed);
	ControlMode GetControlMode(Axis axis);
	double GetOpenLoopOutput(Axis axis);
	double GetVelocitySetpoint(Axis axis);
	double GetPositionSetpoint(Axis axis);
	double GetPositionControlMaxSpeed(Axis axis);
	void OverrideForwardCurrentLimit(float limit);
	void OverrideReverseCurrentLimit(float limit);
	float GetForwardCurrentLimit();
	float GetReverseCurrentLimit();
	bool ShouldOverrideForwardCurrentLimit();
	bool ShouldOverrideReverseCurrentLimit();
	
private:
	ControlMode m_controlModes[2];
	double m_desiredOpenLoopOutputs[2];
	double m_desiredRates[2];
	double m_positionSetpoints[2];
	double m_maxSpeeds[2];
	float m_zeroHeading;
	bool m_overrideCurrentLimitForward;
	bool m_overrideCurrentLimitReverse;
	float m_currentLimitForward;
	float m_currentLimitReverse;
	
	bool m_resetPositionSetpoint[2];
};

#endif
