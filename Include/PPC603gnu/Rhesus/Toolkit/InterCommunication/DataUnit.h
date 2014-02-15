#ifndef RHESUS_COMPONENTDATA_H_
#define RHESUS_COMPONENTDATA_H_

#include "../Defines.h"

#include <string>
#include <hash_map>

#include "../Utilities/Generic.h"
#include "../Tasks/Mutex.h"

namespace Rhesus 
{
namespace Toolkit
{
namespace InterCommunication
{
	
	/*!
	 * @brief Provides an object to store individual component data.  Usually accessed through DataPool and not directly
	 * @author Varun Parthasarathy
	 * @author Tony Peng
	 */
	class DataUnit
	{
	public:
		DataUnit();
		~DataUnit();
		
		/*!
		 * Sets a specified value at a specified key
		 * @param key the key to associate the value to
		 * @param value the value to set
		 */
		template <typename T>
		void Set(std::string key, T value);
		
		/*!
		 * @return the value associated with the specified key
		 * @param key the key of the desired value
		 */
		template <typename T>
		T Get(std::string key);
		
		/*!
		 * Purges the DataUnit of all data
		 */
		void Clear();
		
	private:
		typedef std::hash_map<std::string, Rhesus::Toolkit::Utilities::Generic> DataMap;

		DataMap m_dataMap;
		
		Rhesus::Toolkit::Tasks::Mutex* m_syncObject;
	};

}
}
}

#endif
