#include "RobotLocation.h"
#include "../RobotState.h"
#include "math.h"
#include "../Utils/Util.h"

RobotLocation *RobotLocation::m_instance = NULL;

RobotLocation::RobotLocation() :
	Loggable("RobotLocation"),
	m_notifier((TimerEventHandler) RobotLocation::Periodic, this)
{
	x = y = x_last = y_last = theta_zero = d = theta = d_last = theta_last = 0.0;
	
	m_encoders = DriveEncoders::Get();
	
	sem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	
	m_notifier.StartPeriodic(1.0 / 200.0);
}

void RobotLocation::Initialize()
{
	if (m_instance == NULL)
		m_instance = new RobotLocation();
}

RobotLocation* RobotLocation::Instance()
{
	if (m_instance == NULL)
		m_instance = new RobotLocation();
	return m_instance;
}

void RobotLocation::Finalize()
{
	DELETE(m_instance);
}

void RobotLocation::Update()
{
	Synchronized s(sem);
	
	d = m_encoders->GetRobotDist();
	theta = m_encoders->GetTurnAngle() - theta_zero;
	
	double delta_d = d - d_last;
	double delta_theta = theta - theta_last;
	double translation = delta_d * 360 * sin(delta_theta / 2 * acos(-1) / 180.0) / (acos(-1) * delta_theta);
	if (translation != translation) // Catch NaN when delta_theta is zero
		translation = delta_d;
	x = x_last - translation * sin((theta_last + delta_theta / 2) * acos(-1) / 180.0);
	y = y_last + translation * cos((theta_last + delta_theta / 2) * acos(-1) / 180.0);
	
	x_last = x;
	y_last = y;
	d_last = d;
	theta_last = theta;
}

double RobotLocation::GetX()
{
	Synchronized s(sem);
	return x;
}

double RobotLocation::GetY()
{
	Synchronized s(sem);
	return y;
}

double RobotLocation::GetTheta()
{
	Synchronized s(sem);
	return theta - theta_zero;
}

void RobotLocation::Zero()
{
	Synchronized s(sem);
	x = y = x_last = y_last = 0.0;
	theta_zero = m_encoders->GetTurnAngle();
}

void RobotLocation::Log()
{
	Synchronized s(sem);
	LogToFile(&x, "X");
	LogToFile(&y, "Y");
	LogToFile(&theta, "Theta");
}

void RobotLocation::Send()
{
	SendToNetwork(Util::ToString(x) + " " + Util::ToString(y) + " " + Util::ToString(theta), "Data", "Location");
}

void RobotLocation::Periodic(void *param)
{
	((RobotLocation*)param)->Update();
}
