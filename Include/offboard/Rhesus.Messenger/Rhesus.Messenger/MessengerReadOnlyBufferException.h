#ifndef RHESUS_MSG_READONLYBUFFEREXCEPTION_H_
#define RHESUS_MSG_READONLYBUFFEREXCEPTION_H_

#include <exception>

namespace Rhesus
{
namespace Messenger
{
		class MessengerReadOnlyBufferException : public std::exception
		{
		public:
			MessengerReadOnlyBufferException(const char* message) : m_message(message) { }
				
			const char* what() const throw() { return m_message; }
				
		private:
			const char* m_message;
		};
}
}

#endif
