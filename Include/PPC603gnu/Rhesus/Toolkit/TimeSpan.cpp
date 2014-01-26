#include "TimeSpan.h"

using namespace Rhesus::Toolkit;

TimeSpan::TimeSpan(INT64 ticks)
{
#ifdef __VXWORKS__
	m_totalMilliseconds = ticks * (1 / sysClkRateGet()) * 1000;
#endif
}

TimeSpan::TimeSpan(double seconds)
{
	construct(0, seconds, 0);
}

TimeSpan::TimeSpan(double minutes, double seconds)
{
	construct(minutes, seconds, 0);
}

TimeSpan::TimeSpan(double minutes, double seconds, double milliseconds)
{
	construct(minutes, seconds, milliseconds);
}

void TimeSpan::construct(double minutes, double seconds, double milliseconds)
{
	m_totalMilliseconds = minutes * 60 * 1000 + seconds * 1000 + milliseconds;
}

TimeSpan TimeSpan::operator =(const TimeSpan t)
{
	m_totalMilliseconds = t.m_totalMilliseconds;
	
	return *this;
}

TimeSpan TimeSpan::operator +(const TimeSpan t) const
{
	double total = m_totalMilliseconds + t.m_totalMilliseconds;
	
	TimeSpan res(0.0, 0.0, total);
	
	return res;
}

TimeSpan TimeSpan::operator -(const TimeSpan t) const
{
	double total = m_totalMilliseconds - t.m_totalMilliseconds;
	
	TimeSpan res(0.0, 0.0, total);
	
	return res;
}

TimeSpan& TimeSpan::operator+=(const TimeSpan t)
{
	m_totalMilliseconds += t.m_totalMilliseconds;
	
	return *this;
}

TimeSpan& TimeSpan::operator-=(const TimeSpan t)
{
	m_totalMilliseconds -= t.m_totalMilliseconds;
	
	return *this;
}

bool TimeSpan::operator <(TimeSpan t)
{
	return m_totalMilliseconds < t.m_totalMilliseconds;
}

bool TimeSpan::operator <=(TimeSpan t)
{
	return m_totalMilliseconds <= t.m_totalMilliseconds;
}

bool TimeSpan::operator >(TimeSpan t)
{
	return m_totalMilliseconds > t.m_totalMilliseconds;
}

bool TimeSpan::operator >=(TimeSpan t)
{
	return m_totalMilliseconds >= t.m_totalMilliseconds;
}

bool TimeSpan::operator ==(TimeSpan t)
{
	return m_totalMilliseconds == t.m_totalMilliseconds;
}

bool TimeSpan::operator !=(TimeSpan t)
{
	return m_totalMilliseconds != t.m_totalMilliseconds;
}

INT64 TimeSpan::TotalTicks()
{
	return (INT64)( ((double)m_totalMilliseconds / 1000.0) * sysClkRateGet() );
}

double TimeSpan::TotalMilliseconds()
{
	return m_totalMilliseconds;
}

double TimeSpan::TotalSeconds()
{
	return m_totalMilliseconds / 1000.0;
}

double TimeSpan::TotalMinutes()
{
	return m_totalMilliseconds / 1000.0 / 60.0;
}

double TimeSpan::Milliseconds()
{
	// TODO: write documentation for fractional milliseconds
	return fmod(m_totalMilliseconds, 1000.0);
}

INT32 TimeSpan::Seconds()
{
	return (INT32)(fmod(m_totalMilliseconds, 1000.0 * 60.0) / 1000);
}

INT32 TimeSpan::Minutes()
{
	// TODO: write documentation for rollover
	return (INT32)(fmod(m_totalMilliseconds, 1000.0 * 60.0 * 60.0) / 1000);
}
