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
	 * @brief Provides an object to store individual component data
	 * @author Varun Parthasarathy
	 * @author Tony Peng
	 */
	class DataUnit
	{
	public:
		DataUnit();
		
		template <typename T>
		void Set(string key, T value);
		
		template <typename T>
		T Get(string key);
		
		void Clear();
		
	private:
		DataMap m_dataMap;
		
		SyncObject m_syncObject;
	};

}
}
}

#endif
