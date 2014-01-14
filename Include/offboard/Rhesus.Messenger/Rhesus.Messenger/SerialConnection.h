#ifndef RHESUS_MSG_SERIALCONNECTION_H_
#define RHESUS_MSG_SERIALCONNECTION_H_

#include <queue>

#include "IConnection.h"

#include "MessageBuffer.h"

#if !USE_WPILIB
#if BUILD_ENV==BUILD_ENV_LINUX
#include <string>
#endif
#else
#include <sysLib.h>
#include <SerialPort.h>
#include <Task.h>
#endif

namespace Rhesus
{
namespace Messenger
{
	class SerialConnection : public IConnection
	{
	public:
#if !USE_WPILIB && BUILD_ENV==BUILD_ENV_LINUX
		SerialConnection(std::string path);
#else
		SerialConnection();
#endif
		~SerialConnection();

		void Open();

		INT32 SendRaw(MessageBuffer& buff) = 0;

		Protocol::Enum Protocol() const
		{
			return Protocol::SERIAL;
		}

	private:
		const int kBaudRateCRio9074 = 115200;

		std::queue<MessageBuffer> mMessageQ;

#if USE_WPILIB
		SerialPort mSerialPort;
		Task mUpdateTask;

		static INT32 UpdateWorkerTask(SerialConnection* conn);
#else
#if BUILD_ENV==BUILD_ENV_LINUX
		int pFD;
		std::string mPath;
#endif
#endif
	};
}
}

#endif