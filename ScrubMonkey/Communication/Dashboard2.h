#ifndef DASHBOARD_H_
#define DASHBOARD_H_

#include <hash_map>

#include <Rhesus/Messenger/NetBuffer.h>
#include <Rhesus/Messenger/NetChannel.h>
#include <Rhesus.Toolkit.Utilities.h>

namespace DashboardTelemetryType
{
	enum Enum
	{
        INT8 = 0x00,
        INT16 = 0x01,
        INT32 = 0x02,
        INT64 = 0x03,
        UINT8 = 0x04,
        UINT16 = 0x05,
        UINT32 = 0x06,
        UINT64 = 0x07,
        FLOAT = 0x08,
        DOUBLE = 0x09,
        STRING = 0x0A,
	};
}

namespace DashboardMessageType
{
	enum Enum
	{
		LOCATOR = 0x0,
		TELEMETRY = 0x01,
		COLLECTOR_SPEED = 0x02,
		COLLECTOR = 0x03,
		EVENT_NOTIFICATION = 0x04,
		TELEMETRY_INIT_REQ = 0x05,
	};
}

namespace DashboardTelemHeader
{
	enum Enum
	{
		TELEM_INIT = 0x00,
		TELEM_UPDATE = 0x01
	};
}

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
	
	struct TelemetryInfo
	{
		std::string label;
		DashboardTelemetryType::Enum datatype;
	};
	
	static std::hash_map<INT16, Rhesus::Toolkit::Utilities::Generic> m_telemetryEntries;
	static std::hash_map<INT16, TelemetryInfo> m_telemetryDesc;
	
public:
	static void Close();
	
	static void Tick();
	
	static void Flush();
	
	static void EnqueueRawMessage(Rhesus::Messenger::NetBuffer& buff, 
			Rhesus::Messenger::NetChannel::Enum method, int channel);
	
	static void EnqueueLocatorMessage(double time, double x, double y, double theta);
	static void EnqueueDrivetrainTicksMessage(int ticks);
	
	static void InitializeTelemetry();
	
	static void AddTelemetryData(std::string label, INT16 id, DashboardTelemetryType::Enum dataType);
	static bool SetTelemetryData(INT16 id, Rhesus::Toolkit::Utilities::Generic val);
	
	static void SetOrAddTelemetryData(std::string label, INT16 id, DashboardTelemetryType::Enum dataType, Rhesus::Toolkit::Utilities::Generic val);
};

#endif
