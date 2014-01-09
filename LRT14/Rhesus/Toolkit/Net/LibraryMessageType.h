#ifndef RHESUS_LIBRARY_MESSAGETYPE_H_
#define RHESUS_LIBRARY_MESSAGETYPE_H_

namespace Rhesus
{
namespace Toolkit
{
namespace Net
{
	/*!
	 * @brief Contains an enumeration defining networked message headers used by the underlying networking library.
	 * @author Tony Peng
	 */
	namespace LibraryMessageType
	{
		enum Enum
		{
			MESSAGE_ACK = 0x00,
			CONNECTION_REQUEST = 0x10,
			CONNECTION_CONFIRM = 0x11,
		};
	};
}
}
}

#endif
