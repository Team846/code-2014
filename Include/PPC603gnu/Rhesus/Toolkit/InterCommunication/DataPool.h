#ifndef RHESUS_COMPONENT_DATA_MANAGER_H_
#define RHESUS_COMPONENT_DATA_MANAGER_H_

#include "DataUnit.h"
#include "../Tasks/lock_on.h"

#include <hash_map>
#include <string>

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
		static void AddUnit(std::string name);
		
		/*!
		 * @return a reference to the specified DataUnit
		 * @param unit the name of the DataUnit to retrieve 
		 */
		static DataUnit* Get(std::string unit);
		
		/*!
		 * sets a specific value within a specified DataUnit
		 * @param unit the DataUnit to access
		 * @param key the name of the value to be set
		 * @param value the value to set
		 */
		template<typename T>
		static void Set(std::string unit, std::string key, T value);
		
		/*!
		 * Purges the DataPool of all DataUnits
		 */
		static void Clear();
		
	private:
		
		static std::hash_map<std::string, DataUnit> s_componentMap;
		
		static Rhesus::Toolkit::Tasks::Mutex s_syncObject;
		
		static bool KeyExists(std::string c);
		
	};
}
}
}

#endif
