//#ifndef RHESUS_BUFFEREDCONSOLE_H_
//#define RHESUS_BUFFEREDCONSOLE_H_
//
//#include "../Tasks/SyncObject.h"
//#include "../Tasks/lock_on.hpp"
//#include "../Tasks/RhesusTask.h"
//#include "../Tasks/BinarySemaphore.h"
//
//using namespace Rhesus;
//using namespace Rhesus::Framework;
//using namespace Rhesus::Framework::Tasks;
//
//#include <queue>
//#include <string>
//
//using namespace std;
//
//namespace Rhesus
//{
//namespace Framework
//{
//namespace IO
//{
//
///*!
// * @brief Provides a way to print to perform console output asynchronously
// * @author Varun Parthasarathy
// */
//	class BufferedConsole
//	{
//		
//		
//		
//	public:
//		
//		static void Start();
//		
//		static void Printfln(const char* p, ...);
//		static void Printf(const char* p, ...);
//		
//	private:
//		
//		static RhesusTask s_printThread;
//		static void PrintThread();
//
//		static SyncObject s_so;
//
//		static queue<string> s_printQueue;
//
//		static BinarySemaphore s_binsem;
//		
//	};
//}
//}
//}
//
//#endif
