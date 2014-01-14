#ifndef RHESUS_MSG_ICONNECTION_H_
#define RHESUS_MSG_ICONNECTION_H_

#include "Includes.h"
#include "Protocol.h"
#include "MessageBuffer.h"

namespace Rhesus
{
namespace Messenger
{
	class IConnection
	{
	public:
		virtual void Open();

		virtual INT32 SendRaw(MessageBuffer& buff) = 0;

		virtual Protocol::Enum Protocol() const = 0;
		
	};
}
}

#endif