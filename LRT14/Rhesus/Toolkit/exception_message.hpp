#ifndef RHESUS_EXCEPTION_MESSAGE_H_
#define RHESUS_EXCEPTION_MESSAGE_H_

#include<string>

namespace Rhesus
{
namespace Toolkit
{

	/*!
	 * @brief Exception message object
	 * @author Varun Parthasarathy
	 */
	class exception_message
	{
	public:
		
		exception_message(std::string msg)
		{
			m_message = msg;
		}
		
		const char* message()
		{
			return m_message.c_str();
		}
		
	private:
		
		std::string m_message;
		
	};
}
}

#endif
