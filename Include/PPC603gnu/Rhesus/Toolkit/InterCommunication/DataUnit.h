#ifndef RHESUS_COMPONENTDATA_H_
#define RHESUS_COMPONENTDATA_H_

#include <string>
#include <hash_map>

#include "../Utilities/Generic.hpp"
#include "../Tasks/SyncObject.h"

using namespace std;
using namespace Rhesus::Toolkit::Utilities;
using namespace Rhesus::Toolkit::Tasks;

typedef hash_map<string, Generic> DataMap;

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
		
		/*!
		 * Sets a specified value at a specified key
		 * @param key the key to associate the value to
		 * @param value the value to set
		 */
		template <typename T>
		void Set(string key, T value);
		
		/*!
		 * @return the value associated with the specified key
		 * @param key the key of the desired value
		 */
		template <typename T>
		T Get(string key);
		
		/*!
		 * Purges the DataUnit of all data
		 */
		void Clear();
		
	private:
		DataMap m_dataMap;
		
		SyncObject m_syncObject;
	};

}
}
}

#endif
