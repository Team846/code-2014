#ifndef RHESUS_COMPONENT_DATA_MANAGER_H_
#define RHESUS_COMPONENT_DATA_MANAGER_H_

#include "DataUnit.h"
#include "../Tasks/lock_on.hpp"

#include <hash_map>
#include <string>

using namespace std;
using namespace Rhesus::Toolkit::Tasks;

namespace Rhesus
{
namespace Toolkit
{
namespace InterCommunication
{

	/*!
	 * @brief Provides access to multiple DataUnit objects
	 * @author Varun Parthasarathy
	 * @author Tony Peng
	 */
	class DataPool
	{
	public:
		static void AddUnit(string name);
		
		static DataUnit& Get(string unit);
		
		template<typename T>
		static void Set(string unit, string key, T value);
		
		static void Clear();
		
	private:
		
		static hash_map<string, DataUnit> s_componentMap;
		
		static SyncObject s_syncObject;
		
		static bool KeyExists(string c);
		
	};
}
}
}

#endif
