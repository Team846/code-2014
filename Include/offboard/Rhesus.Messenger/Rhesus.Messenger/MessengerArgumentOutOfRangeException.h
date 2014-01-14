#ifndef RHESUS_MSG_ARGUMENTOUTOFRANGEEXCEPTION_H_
#define RHESUS_MSG_ARGUMENTOUTOFRANGEEXCEPTION_H_

#include <exception>

namespace Rhesus
{
namespace Messenger
{
	class MessengerArgumentOutOfRangeException : public std::exception
	{
	public:
		MessengerArgumentOutOfRangeException(const char* message) : m_message(message) { }
				
		const char* what() const throw() { return m_message; }
				
	private:
		const char* m_message;
	};
}
}

#endif
