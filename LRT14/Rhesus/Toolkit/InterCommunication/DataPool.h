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
		/*!
		 * Adds a DataUnit (a new section of data) to the DataPool
		 * @param name the name of the added DataUnit (used to access it)
		 */
		static void AddUnit(string name);
		
		/*!
		 * @return a reference to the specified DataUnit
		 * @param unit the name of the DataUnit to retrieve 
		 */
		static DataUnit& Get(string unit);
		
		/*!
		 * sets a specific value within a specified DataUnit
		 * @param unit the DataUnit to access
		 * @param key the name of the value to be set
		 * @param value the value to set
		 */
		template<typename T>
		static void Set(string unit, string key, T value);
		
		/*!
		 * Purges the DataPool of all DataUnits
		 */
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
