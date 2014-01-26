#ifndef RHESUS_STOPWATCH_H_
#define RHESUS_STOPWATCH_H_

#include <Timer.h>

#include "../TimeSpan.h"

using namespace Rhesus::Toolkit;

namespace Rhesus
{
namespace Toolkit
{
namespace Diagnostics
{
	class Stopwatch
	{
	public:
		Stopwatch();
		
		void Start();
		void Stop();
		void Restart();
		void Reset();
		
		bool Running();
		
		double TotalElapsedMinutes();
		double TotalElapsedSeconds();
		double TotalElapsedMilliseconds();
		
		TimeSpan TotalElapsedTime();
	private:
		double m_begin;
		double m_end;
		
		double m_totalTime;
		
		bool m_stopped;
	};
}
}
}

#endif
