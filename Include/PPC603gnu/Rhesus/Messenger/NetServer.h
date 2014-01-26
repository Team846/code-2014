#ifndef RHESUS_NET_SERVER_H_
#define RHESUS_NET_SERVER_H_

#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "NetPeer.h"
#include "NetChannel.h"

namespace Rhesus
{
namespace Messenger
{
	class NetServer : public NetPeer
	{
	public:
		NetServer(int port);
		
		void SendToAll(NetBuffer* buff, NetChannel::Enum method, int channel);
		
		vector<NetConnection*> Connections(); 
	};
}
}

#endif
