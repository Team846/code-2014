#ifndef RHESUS_NET_CONNECTION_H_
#define RHESUS_NET_CONNECTION_H_

#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "NetPeer.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Net
{
	class NetConnection
	{
	private:
		int m_socket;
		
		sockaddr_in m_remoteEndpoint;
		NetPeer* m_netPeer;
		
	public:
		sockaddr_in* RemoteEndpoint();
		
		NetConnection(sockaddr_in iep, NetPeer* peer);
		
		void Send(NetBuffer* buff, NetChannel::Enum method, int channel);
	};
}
}
}

#endif
