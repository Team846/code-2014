#include "Profiler.h"

#include <sstream>

#include "../Tasks/lock_on.hpp"
#include "../Utilities/ContainerCleanup.hpp"
#include "../Defines.h"

using namespace Rhesus::Toolkit::Diagnostics;
using namespace Rhesus::Toolkit::Utilities;

SyncObject Profiler::s_syncObj;

std::hash_map<string, Stopwatch*> Profiler::s_profilers;
std::hash_map<string, pair<double, int> > Profiler::s_averageTimes;
std::hash_map<string, double> Profiler::s_lastTimes;
std::hash_map<string, double> Profiler::s_maxTimes;

void Profiler::Clear()
{
	{
		lock_on l(s_syncObj);
		
		for(std::hash_map<string, Stopwatch*>::iterator it = s_profilers.begin(); it != s_profilers.end(); ++it)
		{
			DELETE(it->second);
		}
			
		s_profilers.clear();
		s_averageTimes.clear();
		s_lastTimes.clear();
		s_maxTimes.clear();
	}
}

void Profiler::StartActivity(string taskName)
{
	bool exists;
	
	string name = taskName;
	int counter = 1;
	
	while(exists = (s_profilers.find(name) != s_profilers.end()))
	{
		ostringstream ss;
		
		ss << taskName << " (" << (counter++) << ")";
		
		name = ss.str();
	}
	
	Stopwatch* sw = new Stopwatch();
	
	{
		lock_on l(s_syncObj);
		
		s_profilers.insert(std::make_pair(name, sw));
	}
}

double Profiler::End(string taskName)
{	
	Stopwatch* sw;
	double elapsedMilliseconds = -1;
	
	{
		lock_on l(s_syncObj);
		
		if(s_profilers.find(taskName) == s_profilers.end())
			return -1;
		
		sw = s_profilers[taskName];
		s_profilers.erase(taskName);
		
		sw->Stop();
		
		elapsedMilliseconds = sw->TotalElapsedMilliseconds();
		
		DELETE(sw);
		
		if(s_averageTimes.find(taskName) == s_averageTimes.end())
		{
			s_averageTimes.insert(std::make_pair(taskName, std::make_pair(elapsedMilliseconds, 1)));
		}
		else
		{
			std::pair<double, int> avgPair = s_averageTimes[taskName];
			
			double newAvg = (avgPair.first + elapsedMilliseconds) / (avgPair.second + 1);
			
			s_averageTimes[taskName] = std::make_pair(newAvg, avgPair.second + 1);
		}
		
		if(s_maxTimes.find(taskName) == s_maxTimes.end())
			s_maxTimes.insert(std::make_pair(taskName, elapsedMilliseconds));
		else
		{
			if(elapsedMilliseconds > s_maxTimes[taskName])
				s_maxTimes[taskName] = elapsedMilliseconds;
		}
		
		if(s_lastTimes.find(taskName) == s_lastTimes.end())
			s_lastTimes.insert(std::make_pair(taskName, elapsedMilliseconds));
		else
			s_lastTimes[taskName] = elapsedMilliseconds;
	}
	
	return elapsedMilliseconds;
}

double Profiler::GetAverageTime(string taskName)
{
	{
		lock_on l(s_syncObj);
		
		if(s_averageTimes.find(taskName) == s_averageTimes.end())
			return -1;
		
		return s_averageTimes[taskName].first;
	}
}

double Profiler::GetLastTime(string taskName)
{
	{
		lock_on l(s_syncObj);
		
		if(s_lastTimes.find(taskName) == s_lastTimes.end())
			return -1;
		
		return s_lastTimes[taskName];
	}
}

double Profiler::GetMaxTime(string taskName)
{
	{
		lock_on l(s_syncObj);
		
		if(s_maxTimes.find(taskName) == s_maxTimes.end())
			return -1;
		
		return s_maxTimes[taskName];
	}
}

std::hash_map<string, pair<double, int> > Profiler::CloneAverageTimes()
{
	std::hash_map<string, pair<double, int> > mapCopy;
	
	{
		lock_on l(s_syncObj);
		
		mapCopy.insert(s_averageTimes.begin(), s_averageTimes.end());
	}
	
	return mapCopy;
}

std::hash_map<string, double> Profiler::CloneLastTimes()
{
	std::hash_map<string, double> mapCopy;
	
	{
		lock_on l(s_syncObj);
		
		mapCopy.insert(s_lastTimes.begin(), s_lastTimes.end());
	}
	
	return mapCopy;
}

std::hash_map<string, double> Profiler::CloneMaxTimes()
{
	std::hash_map<string, double> mapCopy;
	
	{
		lock_on l(s_syncObj);
		
		mapCopy.insert(s_maxTimes.begin(), s_maxTimes.end());
	}
	
	return mapCopy;
}
