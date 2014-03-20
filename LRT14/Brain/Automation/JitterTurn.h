#ifndef JITTER_TURN_H_
#define JITTER_TURN_H_

#include "Automation.h"

#include "Turn.h"

class JitterTurn : public Turn
{
public:
	JitterTurn(double base, double low, double high);
	JitterTurn(double low, double high);
	
	bool Start();
private:
	double m_low;
	double m_high;
};

#endif
