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
	explicit Actuator(std::string name);
	
	virtual std::string GetName();
	virtual void Output() = 0;
	
	static void OutputAll();
	
	static std::vector<Actuator*> actuator_vector;
	
private:
	std::string m_name;
};

#endif
