#include "DataUnit.h"

#include "../Tasks/lock_on.hpp"

using namespace Rhesus::Toolkit::InterCommunication;

DataUnit::DataUnit()
{
}

template<typename T>
void DataUnit::Set(string key, T value)
{
	{
		lock_on l(m_syncObject);
		
		m_dataMap.insert(key, Generic(value));
	}
	
}

template<typename T>
T DataUnit::Get(string key)
{
	{
		lock_on l(m_syncObject);
		
		if(m_dataMap.find(key) != m_dataMap.end())
			return m_dataMap[key];
		else
		{
#ifndef RHESUS_NO_THROW
			throw exception();
#endif
		}
	
	}
}

void DataUnit::Clear()
{
	{
		lock_on l(m_syncObject);
		
		m_dataMap.clear();
	}
}


