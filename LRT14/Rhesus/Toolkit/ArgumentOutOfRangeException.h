#ifndef RHESUS_ARGUMENTOUTOFRANGEEXCEPTION_H_
#define RHESUS_ARGUMENTOUTOFRANGEEXCEPTION_H_

using namespace std;

#include <exception>

#include "exception_message.hpp"

namespace Rhesus
{
	namespace Toolkit
	{
		class ArgumentOutOfRangeException : public std::invalid_argument, public exception_message
		{
		public:
			ArgumentOutOfRangeException() : std::invalid_argument("(Rhesus::Framework::ArgumentOutOfRange) Argument is out of range."), exception_message("") { }
			ArgumentOutOfRangeException(const char* message) : std::invalid_argument("(Rhesus::Framework::ArgumentOutOfRange) Argument is out of range."), exception_message(message) { }
		};
	}
}

#endif
