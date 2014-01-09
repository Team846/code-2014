#ifndef RHESUS_INDEXOUTOFBOUNDSEXCEPTION_H_
#define RHESUS_INDEXOUTOFBOUNDSEXCEPTION_H_

#include <exception>

#include "exception_message.hpp"

using namespace std;

namespace Rhesus
{
	namespace Toolkit
	{
		class IndexOutOfBoundsException : public exception, public exception_message
		{
		public:
			IndexOutOfBoundsException() : exception_message("") {}
			IndexOutOfBoundsException(const char* message) : exception_message(message) {}
			
			const char* what() const throw() { return "(Rhesus::Framework::IndexOutOfBounds) Index is out of bounds."; }
			
		};
	}
}

#endif
