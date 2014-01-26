#include "DataPool.h"

#include <hash_map>
#include <string>

#include "../Tasks/lock_on.h"

using namespace std;
using namespace Rhesus::Toolkit::Tasks;
using namespace Rhesus::Toolkit::InterCommunication;

hash_map<string, DataUnit> DataPool::s_componentMap;
SyncObject DataPool::s_syncObject;

void DataPool::AddUnit(string name)
{
	{
		lock_on l(s_syncObject);
		
		s_componentMap.insert(make_pair(name, DataUnit()));
	}
	
}

DataUnit& DataPool::Get(string unit)
{
	{
		lock_on l(s_syncObject);
		
		if(KeyExists(unit))
			return s_componentMap[unit];
		else
		{
#ifndef RHESUS_NO_THROW
			throw exception();
#endif
		}
	}
}

template<typename T>
void DataPool::Set(string unit, string key, T value)
{
	{
		lock_on l(s_syncObject);
		
		if(KeyExists(unit))
			s_componentMap[unit].Set<T>(key, value);
		else
		{
#ifndef RHESUS_NO_THROW
			throw exception();
#endif
		}
	}
	
}

void DataPool::Clear()
{
	{
		lock_on l(s_syncObject);
		
		s_componentMap.clear();
	}
}

bool DataPool::KeyExists(string c)
{
	// deadlock avoided because the internal mutex is re-entrant.
	{
		lock_on l(s_syncObject);
		
		return s_componentMap.find(c) != s_componentMap.end();
	}
}
