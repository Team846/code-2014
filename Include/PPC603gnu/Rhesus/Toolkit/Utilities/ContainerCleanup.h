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
			
			R_DELETE(x);
			
			return true;
		}
		
		/*!
		 * @brief Frees the resources pointed to in a map<pointer, object>
		 */
		template<class A, class B>
		static bool DeleteMapFirst(std::pair<A*, B> x)
		{
			if(x.first == NULL)
				return true;
			
			DELETE(x.first);
			
			return true;
		}
		
		/*!
		 * @brief Frees the resources pointed to in a map<object, pointer>
		 */
		template<class A, class B>
		static bool DeleteMapSecond(std::pair<A, B*> x)
		{
			if(x.second == NULL)
				return true;
			
			R_DELETE(x.second);
			
			return true;
		}
		
		/*!
		 * @brief Frees the resources pointed to in a map<pointer, pointer>
		 */
		template<class A, class B>
		static bool DeleteMapBoth(std::pair<A*, B*> x)
		{
			if(x.first != NULL)
			{
				DELETE(x.first);
			}
			
			if(x.second != NULL)
			{
				DELETE(x.second);
			}
			
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
