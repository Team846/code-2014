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
//	if(!TaskPool::IsRunning())
//			TaskPool::Start();
	
	std::printf("Started...\n");
	
	char buffer[msg.length() + 1];
	msg.copy(buffer, msg.length(), 0);
	buffer[msg.length()] = '\0';
	
	std::printf("1: %s\n", buffer);
	
	va_list args;
	va_start(args, msg);
	
	std::printf("2: %s\n", msg.c_str());

	vsprintf(buffer, msg.c_str(), args);
	va_end(args);
	
	string n(buffer);
	
	std::printf("3: %s\n", n.c_str());
	
	TaskPool::EnqueueTask((FUNCPTR)InternalPrintWrapper, (UINT32)&n);
}

void BufferedConsole::InternalPrintWrapper(std::string* params)
{
	std::printf("F: %s", params->c_str());
	DELETE(params);
}


