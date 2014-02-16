#include "RobotLocation.h"
#include "../RobotState.h"
#include "math.h"
#include "../Utils/Util.h"

#include "../Communication/Dashboard2.h"

#include <Rhesus.Toolkit.IO.h>

using namespace Rhesus::Toolkit::IO;
using namespace Rhesus::Toolkit::Tasks;

RobotLocation *RobotLocation::m_instance = NULL;

RobotLocation::RobotLocation() :
	Loggable("RobotLocation"),
	m_notifier((TimerEventHandler) RobotLocation::Periodic, this)
{
	x = y = x_last = y_last = theta_zero = d = theta = d_last = theta_last = 0.0;
	
	m_encoders = DriveEncoders::Get();
	
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
	R_DELETE(m_instance);
}

void RobotLocation::Update()
{
	lock_on l(m_syncRoot);
	{
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
	} // sem
}

double RobotLocation::GetX()
{
	lock_on l(m_syncRoot);
	{
		return x;
	}
}

double RobotLocation::GetY()
{
	lock_on l(m_syncRoot);
	{
		return y;
	}
}

double RobotLocation::GetTheta()
{
	lock_on l(m_syncRoot);
	{
		return theta - theta_zero;
	}
}

void RobotLocation::Zero()
{
	lock_on l(m_syncRoot);
	{
		x = y = x_last = y_last = 0.0;
		theta_zero = m_encoders->GetTurnAngle();
	}
}

void RobotLocation::Log()
{
	lock_on l(m_syncRoot);
	{
		LogToFile(&x, "X");
		LogToFile(&y, "Y");
		LogToFile(&theta, "Theta");
	}
}

void RobotLocation::Send()
{
	double totTime = RobotState::Instance().TotalTime();
	double xTx = x;
	double yTx = y;
	double thetaTx = theta;
	
	Dashboard2::EnqueueLocatorMessage(totTime, xTx, yTx, thetaTx);
	
	SendToNetwork(Util::ToString(x) + " " + Util::ToString(y) + " " + Util::ToString(theta), "Data", "Location");
}

void RobotLocation::Periodic(void *param)
{
	((RobotLocation*)param)->Update();
}
