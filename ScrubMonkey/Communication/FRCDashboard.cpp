#include "FRCDashboard.h"

using namespace Rhesus::Toolkit::Tasks;
using namespace Rhesus::Messenger;

FRCDashboard* FRCDashboard::m_instance = NULL;

FRCDashboard* FRCDashboard::instance()
{
	if(m_instance == NULL)
	{
		m_instance = new FRCDashboard();
	}
	
	return m_instance;
}

void FRCDashboard::Close()
{
	if(m_instance == NULL)
	{
		return;
	}
	
	R_DELETE(m_instance);
}

FRCDashboard::FRCDashboard()
{
	m_server = new NetServer(1140);
	m_server->Open();
}

FRCDashboard::~FRCDashboard()
{
	m_server->Close();
	R_DELETE(m_server);
}

void FRCDashboard::Tick()
{
	instance()->tick();
}

void FRCDashboard::Flush()
{
	instance()->flush();
}

void FRCDashboard::EnqueueMessage(NetBuffer& buff, NetChannel::Enum method, int channel)
{
	instance()->enqueueMessage(buff, method, channel);
}

void FRCDashboard::tick()
{
	// TODO perhaps later, we may want to send some heartbeats or packets of the sort
	
	// Flush the buffer
	Flush();
}

void FRCDashboard::flush()
{
	{
		lock_on l(m_queueMutex);
		
		for(unsigned int i = 0; i < m_netBufferQueue.size(); i++)
		{
			DMessage nb = m_netBufferQueue.front();
			m_netBufferQueue.pop();
			
			m_server->SendToAll(nb.nb, nb.method, nb.channel);
			
			R_DELETE(nb.nb);
		}
	} // m_queueSem
}

void FRCDashboard::enqueueMessage(NetBuffer& buff, NetChannel::Enum method, int channel)
{
	DMessage msg;
	msg.nb = new NetBuffer(buff.GetBuffer(), buff.GetBufferLength());
	msg.method = method;
	msg.channel = channel;
			
	{
		lock_on l(m_queueMutex);
	
		m_netBufferQueue.push(msg);
	}
}
