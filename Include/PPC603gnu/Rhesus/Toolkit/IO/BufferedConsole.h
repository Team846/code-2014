#ifndef RHESUS_BUFFEREDCONSOLE_H_
#define RHESUS_BUFFEREDCONSOLE_H_

#include "../Defines.h"

#include <queue>
#include <string>

namespace Rhesus
{
namespace Toolkit
{
namespace IO
{
	/*!
	 * @brief Provides a way to perform console output asynchronously
	 * @author Varun Parthasarathy
	 */
	class BufferedConsole
	{
	public:
		static void Printfln(std::string msg, ...);
		static void Printf(std::string msg, ...);
		
	private:
		static void InternalPrintWrapper(std::string* params);
	};
}
}
}

#endif
