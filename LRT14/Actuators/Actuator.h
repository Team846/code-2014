#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include <vector>
#include "../Communication/LiveNetworkSendable.h"

/*!
 * @brief Base class for all actuators.
 */
class Actuator : public LiveNetworkSendable
{
public:
	explicit Actuator(const char* name);
	
	virtual const char* GetName();
	virtual void Output() = 0;
	
	static void OutputAll();
	
	static std::vector<Actuator*> actuator_vector;
	
private:
	const char* m_name;
};

#endif
