#ifndef RHESUS_PROFILER_H_
#define RHESUS_PROFILER_H_

#include <hash_map>
#include <string>

#include "Stopwatch.h"

#include "../Tasks/Mutex.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Diagnostics
{
	/*!
	 * @brief Provides a method to profile activities
	 * @author Tony Peng
	 */
	class Profiler
	{
	public:
		static void Clear();
		
		static void StartActivity(string taskName);
		static double End(string taskName);
		
		static double GetAverageTime(string taskName);
		static double GetLastTime(string taskName);
		static double GetMaxTime(string taskName);
		
		static std::hash_map<string, pair<double, int> > CloneAverageTimes();
		static std::hash_map<string, double> CloneLastTimes();
		static std::hash_map<string, double> CloneMaxTimes();
		
	private:
		static std::hash_map<string, Stopwatch*> s_profilers;
		static std::hash_map<string, pair<double, int> > s_averageTimes;
		static std::hash_map<string, double> s_lastTimes;
		static std::hash_map<string, double> s_maxTimes;
		
		static Rhesus::Toolkit::Tasks::Mutex s_syncObj;
	};
}
}
}

#endif
