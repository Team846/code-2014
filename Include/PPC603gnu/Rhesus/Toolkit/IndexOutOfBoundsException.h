#ifndef RHESUS_INDEXOUTOFBOUNDSEXCEPTION_H_
#define RHESUS_INDEXOUTOFBOUNDSEXCEPTION_H_

#include <exception>

#include "exception_message.hpp"

namespace Rhesus
{
	namespace Toolkit
	{
		class IndexOutOfBoundsException : public std::exception, public exception_message
		{
		public:
			IndexOutOfBoundsException() : exception_message("") {}
			IndexOutOfBoundsException(const char* message) : exception_message(message) {}
			
			const char* what() const throw() { return "(Rhesus::Framework::IndexOutOfBounds) Index is out of bounds."; }
			
		};
	}
}

#endif
