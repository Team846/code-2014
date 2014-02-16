#include "OffboardCommunication.h"

using namespace std;
using namespace Rhesus::Toolkit::Tasks;

OffboardCommunication *OffboardCommunication::m_instance = NULL;

OffboardCommunication* OffboardCommunication::Instance()
{
	if (m_instance == NULL)
		m_instance = new OffboardCommunication();
	return m_instance;
}

void OffboardCommunication::Initialize()
{
	if (m_instance == NULL)
		m_instance = new OffboardCommunication();
	m_instance->Start();
}

void OffboardCommunication::Finalize()
{
	R_DELETE(m_instance);
}

OffboardCommunication::OffboardCommunication() :
	AsyncProcess("OffboardCommunication")
{
	m_serial = new SerialPort(115200);
	buffers[AUTO_AIM].reserve(4);
	buffers[GAME_PIECE_TRACKING].reserve(16);
	buffers[LIDAR].reserve(4324); // Three sets of 360 ints and one float
	buffer.reserve(4324);
	currentStream = UNSET;
	currentState = WAIT_HEADER;
}

OffboardCommunication::~OffboardCommunication()
{
	delete m_serial;
	m_serial = NULL;
}

void OffboardCommunication::Tick()
{
	while (m_serial->GetBytesReceived() > 0)
	{
		char byte;
		m_serial->Read(&byte, 1);
		switch (currentState)
		{
		case WAIT_HEADER:
			if (byte == AUTO_AIM_FLAG || byte == GAME_PIECE_TRACKING_FLAG || byte == LIDAR_FLAG)
			{
				currentStream = FlagToStream((Flag)byte);
				currentState = IN_MSG;
				buffer.clear();
			}
			break;
		case IN_MSG:
			if (byte == ESCAPE)
			{
				currentState = AFTER_ESC;
				break;
			}
			if (byte == AUTO_AIM_FLAG || byte == GAME_PIECE_TRACKING_FLAG || byte == LIDAR_FLAG)
			{
				if (FlagToStream((Flag)byte) == currentStream)
				{
					{
						lock_on l(m_syncRoot[currentStream]);
						{
							buffers[currentStream].clear();
							buffers[currentStream].insert(buffers[currentStream].begin(), buffer.begin(), buffer.end());
						}
					}
					currentStream = UNSET;
					currentState = WAIT_HEADER;
					break;
				}
			}
		case AFTER_ESC:
			buffer.push_back(byte);
			currentState = IN_MSG;
			break;
		}
	}
	taskDelay(sysClkRateGet() / 1000);
}

void OffboardCommunication::Read(OffboardCommunication::Stream stream, vector<char> &buffer)
{
	lock_on l(m_syncRoot[stream]);
	{
		buffer.clear();
		buffer.insert(buffer.begin(), buffers[stream].begin(), buffers[stream].end());
	}
}
