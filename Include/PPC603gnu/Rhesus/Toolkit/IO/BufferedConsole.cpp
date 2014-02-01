#include "BufferedConsole.h"
#include "../Defines.h"

#include "../Tasks/TaskPool.h"

#include <sstream>

using namespace std;
using namespace Rhesus::Toolkit::IO;
using namespace Rhesus::Toolkit::Tasks;

void BufferedConsole::Printfln(std::string msg, ...)
{
//	std::printf("PRINT STARTED...\n");
	
	if(!TaskPool::IsRunning())
		TaskPool::Start();
	
	std::stringstream ss;
	ss << msg << "\n";
	msg = ss.str();
	
	char buffer[msg.length()];
	msg.copy(buffer, msg.length(), 0);

	va_list args;
	va_start(args, msg);

	vsprintf(buffer, msg.c_str(), args);
	va_end(args);
	
	string n(buffer);
	
//	std::printf("ENQUED...");
	
	TaskPool::EnqueueTask((FUNCPTR)InternalPrintWrapper, (UINT32)&n);
}

void BufferedConsole::Printf(std::string msg, ...)
{
	if(!TaskPool::IsRunning())
			TaskPool::Start();
	
	char buffer[msg.length()];
	msg.copy(buffer, msg.length(), 0);

	va_list args;
	va_start(args, msg);

	vsprintf(buffer, msg.c_str(), args);
	va_end(args);
	
	string n(buffer);
	
	TaskPool::EnqueueTask((FUNCPTR)InternalPrintWrapper, (UINT32)&n);
}


