#ifndef RHESUS_NETBUFFEROVERFLOWEXCEPTION_H_
#define RHESUS_NETBUFFEROVERFLOWEXCEPTION_H_

#include <exception>

using namespace std;

namespace Rhesus
{
	namespace Toolkit
	{
		namespace Net
		{
			class NetBufferOverflowException : public exception
			{
			public:
				NetBufferOverflowException(const char* message) : m_message(message) { }
				
				const char* what() const throw() { return m_message; }
				
			private:
				const char* m_message;
			};
		}
	}
}

#endif
