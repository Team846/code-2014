#ifndef RHESUS_CONTAINERCLEANUP_H_
#define RHESUS_CONTAINERCLEANUP_H_

#include <queue>
#include "../Defines.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Utilities
{
	class ContainerCleanup
	{
	public:
		/*!
		 * @brief Frees the resources pointed to in a vector<pointer>
		 */
		template<class A>
		static bool DeleteVector(A x)
		{
			if(x == NULL)
				return true;
			
			DELETE(x);
			
			return true;
		}
		
		/*!
		 * @brief Swaps a queue for an empty queue.
		 */
		template<class T>
		static void ClearQueue(std::queue<T>& q)
		{
			std::queue<T> empty;
			std::swap(q, empty);
		}
	};
}
}
}

#endif
