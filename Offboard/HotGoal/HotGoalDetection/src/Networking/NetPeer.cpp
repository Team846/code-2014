#include "NetPeer.h"
#include "NetConnection.h"

using namespace std;
using namespace Rhesus::Messenger;

const double NetPeer::kResendPacketTime = 1.0; // resend if 100 ms passes with no ACK; max of 500ms roundtrip time

NetPeer::NetPeer(char * ip, int port, NetConnectionType::Enum connType)
{
	this->m_ip = ip;
	this->m_port = port;
	this->m_connType = connType;
	
//	for(int i = 0; i < 16; i++)
//	{
//		m_reliableOrdered[i] = new MessageAwaitingACK[MAX_MESSAGE_TRACK];
//		m_reliableSequenced[i] = new MessageAwaitingACK[MAX_MESSAGE_TRACK];
//		m_reliableOrdered[i] = new MessageAwaitingACK[MAX_MESSAGE_TRACK];
//	}
	
	this->m_lastUnreliableSequenced = new int[MAX_MESSAGE_TRACK];
	this->m_lastReliableSequenced 	= new int[MAX_MESSAGE_TRACK];
	
	this->m_currentReliableUnorderedCounter = 0;
	this->m_currentReliableSequencedCounter = 0;
	this->m_currentReliableOrderedCounter = 0;
	this->m_currentUnreliableSequencedCounter = 0;
	
	InternalPlatformQueueSynchronizationCreate();
	
	InternalPlatformConnectionListSynchronizationCreate();
	
	InternalPlatformReliableUnorderedQueueSynchronizationCreate();
	InternalPlatformReliableSequencedQueueSynchronizationCreate();
	InternalPlatformReliableInOrderQueueSynchronizationCreate();
}

NetPeer::~NetPeer()
{
	Close();
}

#ifdef __VXWORKS__
INT32 NetPeer:: InternalPlatformUpdateTaskWrapper(UINT32 instance)
{
	// vxworks specific code

	//printf("Update task started. \n");
	
	NetPeer* conn = (NetPeer*)instance;
	
	while(conn->m_isRunning)
	{
		conn->Update();
		NET_SLEEP(10);
	}
	
	return 0;
}

INT32 NetPeer::InternalPlatformMessageVerificationTaskWrapper(UINT32 instance)
{
	// vxworks specific code
	
	//printf("Message check task started. \n");

	NetPeer* conn = (NetPeer*)instance;
	
	while(conn->m_isRunning)
	{
		conn->CheckMessages();
		NET_SLEEP(10);
	}
	
	return 0;
}
#elif defined(USE_BOOST)
INT32 NetPeer:: InternalPlatformUpdateTaskWrapper(NetPeer* instance)
{
	// vxworks specific code

	printf("Update task started. \n");

	NetPeer* conn = (NetPeer*)instance;

	while(conn->m_isRunning)
	{
		conn->Update();
		NET_SLEEP(10);
	}

	return 0;
}

INT32 NetPeer::InternalPlatformMessageVerificationTaskWrapper(NetPeer* instance)
{
	// vxworks specific code

	printf("Message check task started. \n");

	NetPeer* conn = (NetPeer*)instance;

	while(conn->m_isRunning)
	{
		conn->CheckMessages();
		NET_SLEEP(10);
	}

	return 0;
}
#endif

void NetPeer::InternalPlatformQueueSynchronizationCreate()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_msgQueueMutex = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
#elif defined(USE_BOOST)
	m_msgQueueMutex = new boost::signals2::mutex();
#endif
}

void NetPeer::InternalPlatformQueueSynchronizationEnter()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semTake(m_msgQueueMutex, WAIT_FOREVER);
#elif defined(USE_BOOST)
	m_msgQueueMutex->lock();
#endif
}

void NetPeer::Update()
{
	int received = 0;
	char rcv_buffer[MAX_RECEIVE_BUFFER_SIZE];

	sockaddr_in from;
	int fromSize = sizeof(from);
	
	while((received = recvfrom(m_socket, rcv_buffer, MAX_RECEIVE_BUFFER_SIZE, 0, (sockaddr*)&from, &fromSize)) > 0)
	{
		NetBuffer* buff = new NetBuffer((UINT8*)rcv_buffer, received);
		
		InternalMessageType c = (InternalMessageType)buff->ReadByte();
		
		switch(c)
		{
		case LIBRARY_DATA:
		{
			// what message?
			LibraryMessageType::Enum msgType = (LibraryMessageType::Enum)buff->ReadByte();
			
			switch(msgType)
			{
			case LibraryMessageType::MESSAGE_ACK:
				{
					// first byte is the send type
					NetChannel::Enum chann = (NetChannel::Enum)buff->ReadByte();
				
					// second byte is the channel
					int channel = buff->ReadByte();
				
					// 2, 3, 4, 5 (4 bytes) form the packet id
					int id = buff->ReadInt32();
				
					switch(chann)
					{
					case NetChannel::NET_RELIABLE:
						InternalPlatformReliableUnorderedQueueSynchronizationEnter();
						m_reliableUnordered[channel].erase(id);
						InternalPlatformReliableUnorderedQueueSynchronizationLeave();
						break;
					case NetChannel::NET_RELIABLE_IN_ORDER:
						InternalPlatformReliableInOrderQueueSynchronizationEnter();
						m_reliableOrdered[channel].erase(id);
						InternalPlatformReliableInOrderQueueSynchronizationLeave();
						break;
					case NetChannel::NET_RELIABLE_SEQUENCED:
						InternalPlatformReliableSequencedQueueSynchronizationEnter();
						m_reliableSequenced[channel].erase(id);
						InternalPlatformReliableSequencedQueueSynchronizationLeave();
						break;
					default:
						break;
					}
				}
				break;
				case LibraryMessageType::CONNECTION_REQUEST:
				{
					if(m_connType == NetConnectionType::CLIENT)
					{
						NetUtil::DebugPrint("Somebody tried to connect to us..\n");
						break;
					}
					
					NetUtil::DebugPrint("Connection request receieved.\n");
					
					NetConnection* nc = new NetConnection(from, this);
					
					InternalPlatformConnectionListSynchronizationEnter();
					if(std::find(m_netConnections.begin(), m_netConnections.end(), nc) == m_netConnections.end())
						m_netConnections.push_back(nc);
					InternalPlatformConnectionListSynchronizationLeave();
					
					NetBuffer confirm;
					
					confirm.Write((UINT8)LIBRARY_DATA);
					confirm.Write((UINT8)LibraryMessageType::CONNECTION_CONFIRM);
					
					SendRaw(&confirm, nc);
				}
					break;
				case LibraryMessageType::CONNECTION_CONFIRM:
					_connected = true;
					break;
			}
		}
			break;
		case USER_DATA:
		{
			// read in the library header data first
			
			// first byte is the send type
			NetChannel::Enum chann = (NetChannel::Enum)buff->ReadByte();
			// second byte is the channel
			int channel = buff->ReadByte();
			// 2, 3, 4, 5 (4 bytes) form the packet id
			int id = buff->ReadInt32();
			
			// we only need to ACK back reliable packets
			switch(chann)
			{
			case NetChannel::NET_RELIABLE_SEQUENCED:
			case NetChannel::NET_RELIABLE_IN_ORDER:
			case NetChannel::NET_RELIABLE:
			{
				// reliable needs an ACK
				NetBuffer ack;
				
				ack.Write((UINT8)LIBRARY_DATA);
				ack.Write((UINT8)LibraryMessageType::MESSAGE_ACK);
				ack.Write((UINT8)chann);
				ack.Write((UINT8)channel);
				ack.Write(id);
				
				// TODO error handling in the future
				sendto(m_socket, (char*)ack.GetBuffer(), ack.GetBufferLength(), 0, (sockaddr*)&from, sizeof(from));
			}
				break;
			default:
				break;
			}
			
			bool receive = true;
			int lastPacket;
			
			// handle sequencing
			switch(chann)
			{
			case NetChannel::NET_UNRELIABLE_SEQUENCED:
				lastPacket = m_lastUnreliableSequenced[channel];
				
				if(id <= lastPacket) // TODO: rollover will break this
					receive = false;
				else
					m_lastUnreliableSequenced[channel] = id;
				break;
			case NetChannel::NET_RELIABLE_SEQUENCED:
				lastPacket = m_lastReliableSequenced[channel];
								
				if(id <= lastPacket) // TODO: rollover will break this
					receive = false;
				else
					m_lastReliableSequenced[channel] = id;
				break;
			case NetChannel::NET_RELIABLE_IN_ORDER:
				receive = false; // will do this later
				break;
			default:
				break;
			}
			
			if(receive)
			{
				// synchronize on the semaphore so that we make sure we're safely accessing the internal message queue
				InternalPlatformQueueSynchronizationEnter();
				m_receivedMessages.push(buff);
				InternalPlatformQueueSynchronizationLeave(); // release the lock on the queue
			}
			break;
		}
		default:
			// wtf?
			break;
		}
	}
}

void NetPeer::CheckMessages()
{
	for(int channel = 0; channel < 16; channel++)
	{
		// reliable
		double now;
		MessageAwaitingACK maack;

		InternalPlatformReliableUnorderedQueueSynchronizationEnter();
		for(map<int, MessageAwaitingACK>::iterator it = m_reliableUnordered[channel].begin(); it != m_reliableUnordered[channel].end(); it++)
		{
			maack = it->second;

			now = TIME_IN_SECONDS();
			
			if(maack.initialized && !maack.acknowledged && (now - maack.sentTime) > kResendPacketTime)
			{
				it->second.acknowledged = true;
				
				Send(maack.buff, maack.recipient, NetChannel::NET_RELIABLE, channel, it->first);
			}
		}
		InternalPlatformReliableUnorderedQueueSynchronizationLeave();

		InternalPlatformReliableSequencedQueueSynchronizationEnter();
		for(map<int, MessageAwaitingACK>::iterator it = m_reliableSequenced[channel].begin(); it != m_reliableSequenced[channel].end(); it++)
		{
			maack = it->second;

			now = TIME_IN_SECONDS();

			if(maack.initialized && !maack.acknowledged && (now - maack.sentTime) > kResendPacketTime)
			{
				it->second.acknowledged = true;
				
				Send(maack.buff, maack.recipient, NetChannel::NET_RELIABLE_SEQUENCED, channel, it->first);
			}
		}
		InternalPlatformReliableSequencedQueueSynchronizationLeave();

		InternalPlatformReliableInOrderQueueSynchronizationEnter();
		for(map<int, MessageAwaitingACK>::iterator it = m_reliableOrdered[channel].begin(); it != m_reliableOrdered[channel].end(); it++)
		{
			maack = it->second;

			now = TIME_IN_SECONDS();
			
			if(maack.initialized && !maack.acknowledged && (now - maack.sentTime) > kResendPacketTime)
			{
				it->second.acknowledged = true;
				
				Send(maack.buff, maack.recipient, NetChannel::NET_RELIABLE_IN_ORDER, channel, it->first);
			}
		}
		InternalPlatformReliableInOrderQueueSynchronizationLeave();
	}
}

void NetPeer::InternalPlatformConnectionListSynchronizationCreate()
{
#ifdef __VXWORKS__
	m_connectionListMutex = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
#elif defined(USE_BOOST)
	m_connectionListMutex = new boost::signals2::mutex();
#endif
}

void NetPeer::InternalPlatformConnectionListSynchronizationEnter()
{
#ifdef __VXWORKS__
	semTake(m_connectionListMutex, WAIT_FOREVER);
#elif defined(USE_BOOST)
	m_connectionListMutex->lock();
#endif
}

void NetPeer::InternalPlatformConnectionListSynchronizationLeave()
{
#ifdef __VXWORKS__
	semGive(m_connectionListMutex);
#elif defined(USE_BOOST)
	m_connectionListMutex->unlock();
#endif
}

void NetPeer::InternalPlatformQueueSynchronizationLeave()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semGive(m_msgQueueMutex);
#elif defined(USE_BOOST)
	m_msgQueueMutex->unlock();
#endif
}

void NetPeer::InternalPlatformReliableUnorderedQueueSynchronizationCreate()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_reliableUnorderedQueueMutex = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
#elif defined(USE_BOOST)
	m_reliableUnorderedQueueMutex = new boost::signals2::mutex();
#endif
}

void NetPeer::InternalPlatformReliableUnorderedQueueSynchronizationEnter()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semTake(m_reliableUnorderedQueueMutex, WAIT_FOREVER);
#elif defined(USE_BOOST)
	m_reliableUnorderedQueueMutex->lock();
#endif
}

void NetPeer::InternalPlatformReliableUnorderedQueueSynchronizationLeave()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semGive(m_reliableUnorderedQueueMutex);
#elif defined(USE_BOOST)
	m_reliableUnorderedQueueMutex->unlock();
#endif
}

void NetPeer::InternalPlatformReliableSequencedQueueSynchronizationCreate()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_reliableSequencedQueueMutex = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
#elif defined(USE_BOOST)
	m_reliableSequencedQueueMutex = new boost::signals2::mutex();
#endif
}

void NetPeer::InternalPlatformReliableSequencedQueueSynchronizationEnter()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semTake(m_reliableSequencedQueueMutex, WAIT_FOREVER);
#elif defined(USE_BOOST)
	m_reliableSequencedQueueMutex->lock();
#endif
}

void NetPeer::InternalPlatformReliableSequencedQueueSynchronizationLeave()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semGive(m_reliableSequencedQueueMutex);
#elif defined(USE_BOOST)
	m_reliableSequencedQueueMutex->unlock();
#endif
}

void NetPeer::InternalPlatformReliableInOrderQueueSynchronizationCreate()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_reliableInOrderQueueMutex = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
#elif defined(USE_BOOST)
	m_reliableInOrderQueueMutex = new boost::signals2::mutex();
#endif
}

void NetPeer::InternalPlatformReliableInOrderQueueSynchronizationEnter()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semTake(m_reliableInOrderQueueMutex, WAIT_FOREVER);
#elif defined(USE_BOOST)
	m_reliableInOrderQueueMutex->lock();
#endif
}

void NetPeer::InternalPlatformReliableInOrderQueueSynchronizationLeave()
{
#ifdef __VXWORKS__
	// vxworks specific code
	semGive(m_reliableInOrderQueueMutex);
#elif defined(USE_BOOST)
	m_reliableInOrderQueueMutex->unlock();
#endif
}
void NetPeer::InternalPlatformCreateUpdateTasks()
{
	static int counter = 0;
	
	stringstream s;
	
	s << "NetConnection" << counter;
	
#ifdef __VXWORKS__
	// vxworks specific code
	m_internalUpdateTask = new Task(s.str().c_str(), (FUNCPTR)InternalPlatformUpdateTaskWrapper);
#endif
	
	s << "_MessageCheck";
	
#ifdef __VXWORKS__
	// vxworks specific code
	m_internalMessageVerificationTask = new Task(s.str().c_str(), (FUNCPTR)InternalPlatformMessageVerificationTaskWrapper);
#endif
}

void NetPeer::InternalPlatformRunUpdateTasks()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_internalUpdateTask->Start((UINT32)this);
	m_internalMessageVerificationTask->Start((UINT32)this);
#elif defined(USE_BOOST)
	m_internalUpdateTask = new boost::thread(InternalPlatformUpdateTaskWrapper, this);
	m_internalMessageVerificationTask = new boost::thread(InternalPlatformMessageVerificationTaskWrapper, this);
#endif
}

void NetPeer::InternalPlatformDestroyUpdateTasks()
{
#ifdef __VXWORKS__
	// vxworks specific code
	m_internalUpdateTask->Stop();
	m_internalMessageVerificationTask->Stop();
	
	delete m_internalUpdateTask;
	m_internalUpdateTask = NULL;
	
	delete m_internalMessageVerificationTask;
	m_internalMessageVerificationTask = NULL;
	
#elif defined(USE_BOOST)
	m_internalUpdateTask->interrupt();
	m_internalMessageVerificationTask->interrupt();

	delete m_internalUpdateTask;
	m_internalUpdateTask = NULL;
	delete m_internalMessageVerificationTask;
	m_internalMessageVerificationTask = NULL;
#endif
}

int NetPeer::Open(int options, ...)
{
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	if(m_socket < 0)
	{
		Close();
		return m_socket;
	}
	
	va_list vl;
	
	va_start(vl, options);
	
	int i;
	int value;
	
	for(i = 0; i < options; i++)
	{
		value = va_arg(vl, int); // do not be alarmed if the IDE states that this is a syntax error.
		
		// before you freak out about the scope of the pointer, the pointer is deref'd once by the system and never called again.
		char enabled = 1;
		setsockopt(m_socket, SOL_SOCKET, value, &enabled, sizeof(enabled));
	}
	
	va_end(vl);
	
	m_socketEndpoint.sin_family = AF_INET;
	
	switch(this->m_connType)
	{
		case NetConnectionType::CLIENT:
			m_socketEndpoint.sin_addr.s_addr = inet_addr(this->m_ip);
			m_socketEndpoint.sin_port = htons(this->m_port);
			
			break;
		case NetConnectionType::SERVER:
			m_socketEndpoint.sin_addr.s_addr = INADDR_ANY;
			m_socketEndpoint.sin_port = htons(this->m_port);
			break;
	}
	
	int retcode = bind(m_socket, (struct sockaddr*) &m_socketEndpoint, sizeof(m_socketEndpoint));
	
	if(retcode < 0)
	{
		Close();
		return retcode;
	}
	
	m_isRunning = true;
	
	InternalPlatformCreateUpdateTasks();
	InternalPlatformRunUpdateTasks();

	return 0;
}

int NetPeer::Close()
{
	m_isRunning = false;
	
//#pragma warning this function is potentially bad because we might be killing the thread while it's waiting for a message. -tp
	InternalPlatformDestroyUpdateTasks();
	
	return close(m_socket);
}

int NetPeer::Send(NetBuffer* buff, NetConnection* to, NetChannel::Enum method, int channel, int id)
{
	//if(buff->m_sent)
	//{
	//	return SEND_FAILED_BUFFER_ALREADY_SENT;
	//}

	if(buff->GetBuffer() == NULL)
	{
		return SEND_FAILED_BUFFER_INVALID;
	}

	bool addToInternalQueue = id == -1;
	
	MessageAwaitingACK maack;
	
	maack.initialized = true;
	maack.buff = buff;
	maack.sentTime = TIME_IN_SECONDS();
	maack.acknowledged = false;
	maack.recipient = to;
	
	NetBuffer* localBuff = new NetBuffer();
	localBuff->Write((UINT8)USER_DATA);
	localBuff->Write((UINT8)method);
	localBuff->Write((UINT8)channel);
	
	switch(method)
	{
		// TODO: specific handlers for different methods
	case NetChannel::NET_RELIABLE:
		id = id == -1 ? ++m_currentReliableUnorderedCounter : id;
		
		localBuff->Write(id);
		
		if(addToInternalQueue)
		{
			InternalPlatformReliableUnorderedQueueSynchronizationEnter();
			m_reliableUnordered[channel][id] = maack;
			InternalPlatformReliableUnorderedQueueSynchronizationLeave();
		}
		break;
	case NetChannel::NET_RELIABLE_IN_ORDER:
		id = id == -1 ? ++m_currentReliableOrderedCounter : id;
		
		localBuff->Write(id);
		
		if(addToInternalQueue)
		{
			InternalPlatformReliableInOrderQueueSynchronizationEnter();
			m_reliableOrdered[channel][id] = maack;
			InternalPlatformReliableInOrderQueueSynchronizationLeave();
		}
		break;
	case NetChannel::NET_RELIABLE_SEQUENCED:
		id = id == -1 ? ++m_currentReliableSequencedCounter : id;
		
		localBuff->Write(id);
		
		if(addToInternalQueue)
		{
			InternalPlatformReliableSequencedQueueSynchronizationEnter();
			m_reliableSequenced[channel][id] = maack;
			InternalPlatformReliableSequencedQueueSynchronizationLeave();
		}
		break;
	case NetChannel::NET_UNRELIABLE:
		localBuff->Write(0);
		 break;
	case NetChannel::NET_UNRELIABLE_SEQUENCED:
		localBuff->Write(++m_currentUnreliableSequencedCounter);
		break;
	default:

		break;
	}

	localBuff->WriteRaw(buff->GetBuffer(), buff->GetBufferLength());

	buff->m_sent = true;
	
	int iResult = sendto(m_socket, (char*)localBuff->GetBuffer(), localBuff->GetBufferLength(), 0, (sockaddr*)to->RemoteEndpoint(), sizeof(*(to->RemoteEndpoint())));
	
	return iResult;
}

void NetPeer::SendRaw(NetBuffer* nb, NetConnection* nc)
{
	nb->m_sent = true;
	
	sendto(m_socket, (char*)nb->GetBuffer(), nb->GetBufferLength(), 0, (sockaddr*)nc->RemoteEndpoint(), sizeof(*nc->RemoteEndpoint()));
}

NetBuffer* NetPeer::ReadMessage()
{
	NetBuffer* buff;
	
	InternalPlatformQueueSynchronizationEnter();
	
	if(m_receivedMessages.size() > 0)
	{
		buff = m_receivedMessages.front();
		m_receivedMessages.pop();
	}
	else
	{
		buff = NULL;
	}
	
	InternalPlatformQueueSynchronizationLeave();
	
	return buff;
}
