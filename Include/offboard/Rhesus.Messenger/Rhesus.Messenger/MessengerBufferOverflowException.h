#ifndef RHESUS_MSG_BUFFEROVERFLOWEXCEPTION_H_
#define RHESUS_MSG_BUFFEROVERFLOWEXCEPTION_H_

#include <exception>

namespace Rhesus
{
namespace Messenger
{
	class MessengerBufferOverflowException : public std::exception
	{
	public:
		MessengerBufferOverflowException(const char* message) : m_message(message) { }
				
		const char* what() const throw() { return m_message; }
				
	private:
		const char* m_message;
	};
}
}

#endif
