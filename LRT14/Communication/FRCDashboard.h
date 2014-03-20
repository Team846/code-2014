#ifndef FRCDASH_DASHBOARD_H_
#define FRCDASH_DASHBOARD_H_

#include <WPILib.h>
#include <semLib.h>

#include <queue>
#include <sstream>

#include <Rhesus/Toolkit/Tasks/Rhesus.Toolkit.Tasks.h>

#include <Rhesus/Messenger/NetServer.h>
#include <Rhesus/Messenger/NetBuffer.h>
#include <Rhesus/Messenger/NetChannel.h>

class FRCDashboard
{
private:
	typedef struct
	{
		Rhesus::Messenger::NetBuffer* nb;
		Rhesus::Messenger::NetChannel::Enum method;
		int channel;
	} DMessage;
	
public:
	static void Close();
	
	/*!
	 * Updates the dashboard and flushes the message queue.
	 */
	static void Tick();
	
	/*!
	 * @brief Forces a flush of the message queue.
	 */
	static void Flush();

	static void EnqueueMessage(Rhesus::Messenger::NetBuffer& buff, 
			Rhesus::Messenger::NetChannel::Enum method, int channel);

	static Rhesus::Messenger::NetBuffer* ReadMessage();
	
private:
	
	static FRCDashboard* instance();
	
	static FRCDashboard* m_instance;
	
	FRCDashboard();
	~FRCDashboard();
	
	void tick();
	
	void flush();

	void enqueueMessage(Rhesus::Messenger::NetBuffer& buff, 
			Rhesus::Messenger::NetChannel::Enum method, int channel);
	
	Rhesus::Messenger::NetBuffer* readMessage();
	
	Rhesus::Toolkit::Tasks::Mutex m_queueMutex;
	std::queue<DMessage> m_netBufferQueue;
	std::queue<Rhesus::Messenger::NetBuffer*> m_incomingMessages;
	
	Rhesus::Messenger::NetServer* m_server;
};

#endif
