#include "JitterTurn.h"

JitterTurn::JitterTurn(double base, double low, double high)
	: Turn(base)
{
	m_low = low;
	m_high = high;
}

JitterTurn::JitterTurn(double low, double high)
	: Turn()
{
	m_low = low;
	m_high = high;
}

bool JitterTurn::Start()
{
	double base = getAngle();
	double randVal = ((double)rand() / RAND_MAX);
	double angle = base + m_low + (randVal * (m_high - m_low));
	
	setAngle(angle);
	
	return Turn::Start();
}
