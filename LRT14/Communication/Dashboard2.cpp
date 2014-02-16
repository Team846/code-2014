#include "Dashboard2.h"
#include "NetChannelDefinition.h"

#include "FRCDashboard.h"

using namespace Rhesus::Messenger;

void Dashboard2::Close()
{
	FRCDashboard::Close();
}

void Dashboard2::Tick()
{
	FRCDashboard::Tick();
}

void Dashboard2::Flush()
{
	FRCDashboard::Flush();
}

void Dashboard2::EnqueueRawMessage(NetBuffer& buff, NetChannel::Enum method, int channel)
{
	FRCDashboard::EnqueueMessage(buff, method, channel);
}

void Dashboard2::EnqueueLocatorMessage(double time, double x, double y, double theta)
{
	NetBuffer buff;
	buff.Write((UINT8)HEADER_LOCATOR);
	buff.Write((float)time);
	buff.Write((float)x);
	buff.Write((float)y);
	buff.Write((float)theta);
	
	EnqueueRawMessage(buff, NetChannel::NET_UNRELIABLE_SEQUENCED, NetChannelDefinition::UNRELIABLE_SEQUENCED::LOCATOR);
}

void Dashboard2::EnqueueDrivetrainTicksMessage(int ticks)
{
	return;
	
	NetBuffer buff;
	buff.Write((UINT8)HEADER_DRIVETRAIN_TICKS);
	buff.Write(ticks);
	
	EnqueueRawMessage(buff, NetChannel::NET_UNRELIABLE_SEQUENCED, NetChannelDefinition::UNRELIABLE_SEQUENCED::DRIVETRAIN_TICKS);
}
