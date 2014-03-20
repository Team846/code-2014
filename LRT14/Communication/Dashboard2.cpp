#include "Dashboard2.h"
#include "NetChannelDefinition.h"

#include "FRCDashboard.h"

#include "../RobotState.h"

using namespace Rhesus::Messenger;
using namespace Rhesus::Toolkit::Utilities;

void Dashboard2::Close()
{
	FRCDashboard::Close();
}

void Dashboard2::Tick()
{
	FRCDashboard::Tick();
	
	NetBuffer* nb;
	
	while((nb = FRCDashboard::ReadMessage()) != NULL)
	{
		DashboardMessageType::Enum header = (DashboardMessageType::Enum)nb->ReadByte();
		
		switch(header)
		{
			case DashboardMessageType::TELEMETRY_INIT_REQ: // request telem init
			{
				// send init packet
				NetBuffer reply; // TODO: possible optimization by pre-setting max length
				
				reply.Write((UINT8)DashboardMessageType::TELEMETRY);
				reply.Write((UINT8)DashboardTelemHeader::TELEM_INIT);
				reply.Write((INT16)m_telemetryDesc.size());
				
				for(std::hash_map<INT16, TelemetryInfo>::iterator it = m_telemetryDesc.begin(); it != m_telemetryDesc.end(); ++it)
				{
					TelemetryInfo info = it->second;
					INT16 tId = it->first;
					
					reply.Write(info.label);
					reply.Write((INT16)tId);
					reply.Write((UINT8)info.datatype);
					reply.Write(false);
				}
				
				EnqueueRawMessage(reply, NetChannel::NET_UNRELIABLE_SEQUENCED, NetChannelDefinition::UNRELIABLE_SEQUENCED::TELEM_INIT);
			}
			break;
			default:
			break;
		}
		
		R_DELETE(nb);
	}
	
	// update telemetry
	NetBuffer telemUpdate; // TODO: possible optimization by pre-setting max length
	
	telemUpdate.Write((UINT8)DashboardMessageType::TELEMETRY);
	telemUpdate.Write((UINT8)DashboardTelemHeader::TELEM_UPDATE);
	telemUpdate.Write((float)RobotState::Instance().TotalTime());
	telemUpdate.Write((INT16)m_telemetryDesc.size());
	
	for(std::hash_map<INT16, Generic>::iterator it = m_telemetryEntries.begin(); it != m_telemetryEntries.end(); ++it)
	{
		Generic value = it->second;
		INT16 tId = it->first;
		
		if(m_telemetryDesc.find(tId) == m_telemetryDesc.end()) continue;
		
		TelemetryInfo info = m_telemetryDesc[tId];
		
		// the tId is repeated so we can safely discard this telemetry if the datatype handler isn't found.
		
		switch(info.datatype)
		{
		case DashboardTelemetryType::DOUBLE:
		{
			telemUpdate.Write((INT16)tId);
			
			double d = generic_cast<double>(value);
			
			telemUpdate.Write(d);
		}
			break;
		case DashboardTelemetryType::FLOAT:
		{
			telemUpdate.Write((INT16)tId);
			
			float f = generic_cast<float>(value);
			
			telemUpdate.Write(f);
		}
			break;
		case DashboardTelemetryType::INT16:
		{
			telemUpdate.Write((INT16)tId);
			
			INT16 s = generic_cast<INT16>(value);
			
			telemUpdate.Write(s);
		}
			break;
		case DashboardTelemetryType::INT32:
		{
			telemUpdate.Write((INT16)tId);
			
			INT32 i = generic_cast<INT32>(value);
			
			telemUpdate.Write(i);
		}
			break;
		case DashboardTelemetryType::INT64:
		{
			telemUpdate.Write((INT16)tId);
			
			INT64 l = generic_cast<INT64>(value);
			
			telemUpdate.Write(l);
		}
			break;
		case DashboardTelemetryType::INT8:
		{
			telemUpdate.Write((INT16)tId);
			
			INT8 sb = generic_cast<INT8>(value);
			
			telemUpdate.Write(sb);
		}
			break;
		case DashboardTelemetryType::STRING:
		{
			telemUpdate.Write((INT16)tId);
			
			std::string str = generic_cast<std::string>(value);
			
			//telemUpdate.Write(str);
		}
			break;
		case DashboardTelemetryType::UINT16:
		{
			telemUpdate.Write((INT16)tId);
			
			UINT16 us = generic_cast<UINT16>(value);
			
			telemUpdate.Write(us);
		}
			break;
		case DashboardTelemetryType::UINT32:
		{
			telemUpdate.Write((INT16)tId);
			
			UINT32 ui = generic_cast<UINT32>(value);
			
			telemUpdate.Write(ui);
		}
			break;
		case DashboardTelemetryType::UINT64:
		{
			telemUpdate.Write((INT16)tId);
			
			UINT64 ul = generic_cast<UINT64>(value);
			
			telemUpdate.Write(ul);
		}
			break;
		case DashboardTelemetryType::UINT8:
		{
			telemUpdate.Write((INT16)tId);
			
			UINT8 b = generic_cast<UINT8>(value);
			
			telemUpdate.Write(b);
		}
			break;
		default:
		{
			continue;
		}
			break;
		}
	}
	
	EnqueueRawMessage(telemUpdate, NetChannel::NET_UNRELIABLE_SEQUENCED, NetChannelDefinition::UNRELIABLE_SEQUENCED::TELEM_UPDATE);
	
	// flush the send buffer
	Flush();
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
	NetBuffer buff;
	buff.Write((UINT8)HEADER_DRIVETRAIN_TICKS);
	buff.Write(ticks);
	
	EnqueueRawMessage(buff, NetChannel::NET_UNRELIABLE_SEQUENCED, NetChannelDefinition::UNRELIABLE_SEQUENCED::DRIVETRAIN_TICKS);
}

void Dashboard2::AddTelemetryData(std::string label, INT16 id, DashboardTelemetryType::Enum dataType)
{
	m_telemetryEntries.insert(std::make_pair(id, "???"));
	
	TelemetryInfo ti;
	ti.datatype = dataType;
	ti.label = label;
	
	m_telemetryEntries.insert(std::make_pair(id, label));
}

bool Dashboard2::SetTelemetryData(INT16 id, Rhesus::Toolkit::Utilities::Generic val)
{
	if(m_telemetryEntries.find(id) == m_telemetryEntries.end()) return false;
	
	m_telemetryEntries[id] = val;

	return true;
}
