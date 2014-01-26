#ifndef RHESUS_NETARGUMENTOUTOFRANGEEXCEPTION_H_
#define RHESUS_NETARGUMENTOUTOFRANGEEXCEPTION_H_

#include <exception>

namespace Rhesus
{
namespace Messenger
{
	class NetArgumentOutOfRangeException : public std::invalid_argument
	{
	public:
		NetArgumentOutOfRangeException() : std::invalid_argument("(Rhesus::Framework::ArgumentOutOfRange) Argument is out of range.") { }
		NetArgumentOutOfRangeException(const char* message) : std::invalid_argument("(Rhesus::Framework::ArgumentOutOfRange) Argument is out of range.") { }
	};
}
}

#endif
