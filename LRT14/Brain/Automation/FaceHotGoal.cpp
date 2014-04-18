#include "FaceHotGoal.h"

FaceHotGoal::FaceHotGoal(std::string startSide, double turnDegrees)
{
	m_turnDegrees = turnDegrees;
	m_startSide = startSide;
	m_hotGoal = CheesyVisionServer::GetInstance();
}

bool FaceHotGoal::Start()
{
	if(m_startSide == "center")
	{
		double turn = m_turnDegrees * m_hotGoal->GetRightStatus() ? 1 : -1;
		setAngle(turn);
	}
	else
	{
		bool isActive = m_startSide == "right" ? m_hotGoal->GetRightStatus() : m_hotGoal->GetLeftStatus();
		
		if(isActive)
			setAngle(0.0);
		else
			setAngle(m_startSide == "right" ? -m_turnDegrees: m_turnDegrees );	
	}
	return Turn::Start();
}
