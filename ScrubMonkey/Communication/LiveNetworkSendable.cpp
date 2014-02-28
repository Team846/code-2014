#include "LiveNetworkSendable.h"

using namespace std;

LiveNetworkSendable::LiveNetworkSendable()
{
	m_sender = LiveNetworkSender::Instance();
	LiveNetworkSender::RegisterLiveNetworkSendable(this);
}

LiveNetworkSendable::~LiveNetworkSendable()
{
	
}
