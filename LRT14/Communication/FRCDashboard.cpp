#include "FRCDashboard.h"

using namespace Rhesus::Toolkit::Tasks;
using namespace Rhesus::Messenger;

FRCDashboard* FRCDashboard::m_instance = NULL;

void FRCDashboard::Create()
{
	if(m_instance == NULL)
	{
		m_instance = new FRCDashboard();
	}
}

FRCDashboard* FRCDashboard::instance()
{
	Create(); // create if instance does not exist.
	
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

NetBuffer* FRCDashboard::ReadMessage()
{
	return instance()->readMessage();
}

NetBuffer* FRCDashboard::readMessage()
{
	if(m_incomingMessages.empty()) return NULL;
	
	NetBuffer* nb = m_incomingMessages.front();
	m_incomingMessages.pop();
	
	return nb;
}

void FRCDashboard::tick()
{
	NetBuffer* nb;
	
	while((nb = m_server->ReadMessage()) != NULL)
	{
		std::printf("message!\n");
		m_incomingMessages.push(nb);
	}
}

void FRCDashboard::flush()
{
	{
		lock_on l(m_queueMutex);
		
		while(!m_netBufferQueue.empty())
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
