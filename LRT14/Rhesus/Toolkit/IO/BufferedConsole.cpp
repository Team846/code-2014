//#include "BufferedConsole.h"
//#include "../Defines.h"
//
//using namespace std;
//using namespace Rhesus::Framework::IO;
//
//BinarySemaphore BufferedConsole::s_binsem(0);
//SyncObject BufferedConsole::s_so;
//queue<string> BufferedConsole::s_printQueue;
//RhesusTask BufferedConsole::s_printThread = RhesusTask::CreateNew("BufferedConsole", (FUNCPTR) PrintThread);;
//
//void BufferedConsole::Start() 
//{
//	s_printThread.Start();
//}
//
//void BufferedConsole::PrintThread()
//{
//	
//	while(true)
//	{
//
//		s_binsem.Take(WAIT_FOREVER);
//		
//		{
//			lock_on l(BufferedConsole::s_so);
//
//			while(!s_printQueue.empty())
//			{
//
//				string msg = s_printQueue.front();
//				s_printQueue.pop();
//				printf(msg.c_str());
//
//			}
//		
//		}
//
//	}
//	
//}
//
//void BufferedConsole::Printf(const char* p, ...)
//{
//	
//	char buffer[256];
//
//	va_list args;
//	va_start(args, p);
//
//	vsprintf(buffer, p, args);
//	va_end(args);
//	
//	string n = string(buffer);
//	
//	{
//		
//		lock_on l(s_so);
//		
//		s_printQueue.push(n);
//		
//	}
//	
//	s_binsem.Give();
//	
//}
//
//void BufferedConsole::Printfln(const char* p, ...)
//{
//	
//	char buffer[256];
//
//	va_list args;
//	va_start(args, p);
//
//	vsprintf(buffer, p, args);
//	va_end(args);
//
//	string n = string(buffer) + "\n";
//
//	{
//
//		lock_on l(s_so);
//
//		s_printQueue.push(n);
//
//	}
//
//	s_binsem.Give();
//	
//}
//

