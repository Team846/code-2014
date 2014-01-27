#ifndef DASHBOARD_H_
#define DASHBOARD_H_

#include <Rhesus/Messenger/NetBuffer.h>
#include <Rhesus/Messenger/NetChannel.h>

/*!
 * @brief Wrapper around FRCDashboard class that provides specific functionality.
 */
class Dashboard2
{
private:
	enum DMessageType
	{
		HEADER_LOCATOR = 0x00,
		HEADER_DRIVETRAIN_TICKS = 0x01
	};
	
public:
	static void Close();
	
	static void Tick();
	
	static void Flush();
	
	static void EnqueueRawMessage(Rhesus::Messenger::NetBuffer& buff, 
			Rhesus::Messenger::NetChannel::Enum method, int channel);
	
	static void EnqueueLocatorMessage(double x, double y, double theta);
	static void EnqueueDrivetrainTicksMessage(int ticks);
};

#endif
