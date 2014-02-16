#ifndef SENSOR_FACTORY_H_
#define SENSOR_FACTORY_H_

#include <map>
#include <DigitalInput.h>
#include <AnalogChannel.h>
#include <Counter.h>
#include <GearTooth.h>
#include "DriveEncoders.h"
#include "LRTEncoder.h"
#include "../Communication/LiveNetworkSendable.h"

/*!
 * @brief Contains factory methods for accessing sensors.
 */
class SensorFactory : public LiveNetworkSendable
{
public:
	static void Initialize();
	static void Finalize();
	
	static AnalogChannel* GetAnalogChannel(uint32_t port);
	static DigitalInput* GetDigitalInput(uint32_t port);
	static LRTEncoder* GetLRTEncoder(std::string name, uint32_t portA, uint32_t portB);
	static Counter* GetCounter(uint32_t port);
	static GearTooth* GetGearTooth(uint32_t port);
	
	void Send();
	
private:
	static map<uint32_t, AnalogChannel*> m_analog;
	static map<uint32_t, DigitalInput*> m_digital;
	static map<uint32_t, Counter*> m_counters;
	static map<pair<uint32_t, uint32_t>, LRTEncoder*> m_encoders;
	
	static SensorFactory *m_instance;
};

#endif
