#ifndef RHESUS_TIMESPAN_H_
#define RHESUS_TIMESPAN_H_

#include <WPILib.h>
#include <sysLib.h>
#include <cmath>

namespace Rhesus
{
namespace Toolkit
{
	class TimeSpan
	{
	public:
		TimeSpan(INT64 ticks);
		TimeSpan(double seconds);
		TimeSpan(double minutes, double seconds);
		TimeSpan(double minutes, double seconds, double milliseconds);
		
		TimeSpan operator =(const TimeSpan t);
		
		TimeSpan operator +(const TimeSpan t) const;
		TimeSpan operator -(const TimeSpan t) const;
		
		TimeSpan& operator+=(const TimeSpan t);
		TimeSpan& operator-=(const TimeSpan t);
		
		bool operator <(TimeSpan t);
		bool operator <=(TimeSpan t);
		
		bool operator >(TimeSpan t);
		bool operator >=(TimeSpan t);
		
		bool operator ==(TimeSpan t);
		bool operator !=(TimeSpan t);
		
		INT64 TotalTicks();
		double TotalMilliseconds();
		double TotalSeconds();
		double TotalMinutes();
		
		double Milliseconds();
		INT32 Seconds();
		INT32 Minutes();
		
	private:
		void construct(double minutes, double seconds, double milliseconds);
		
		double m_totalMilliseconds;
	};
}
}

#endif
