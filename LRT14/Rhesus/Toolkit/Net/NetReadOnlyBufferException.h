#ifndef RHESUS_NETREADONLYBUFFEREXCEPTION_H_
#define RHESUS_NETREADONLYBUFFEREXCEPTION_H_

#include <exception>

using namespace std;

namespace Rhesus
{
	namespace Toolkit
	{
		namespace Net
		{
			class NetReadOnlyBufferException : public exception
			{
			public:
				NetReadOnlyBufferException(const char* message) : m_message(message) { }
				
				const char* what() const throw() { return m_message; }
				
			private:
				const char* m_message;
			};
		}
	}
}

#endif
