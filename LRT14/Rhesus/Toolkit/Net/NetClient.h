#ifndef RHESUS_NET_CLIENT_H_
#define RHESUS_NET_CLIENT_H_

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
	class NetClient : public NetPeer
	{
	public:
		NetClient();
		
		void Connect(sockaddr_in ep);
	private:
		void CheckMessages();
		
		NetConnection* _server;
		
		double _connectionRequestTime;
		bool _connectionRequested;
	};
}
}
}

#endif
