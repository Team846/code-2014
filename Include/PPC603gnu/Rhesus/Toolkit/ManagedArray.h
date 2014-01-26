#ifndef RHESUS_SAFEARRAY_H_
#define RHESUS_SAFEARRAY_H_

#include <cstddef>

#ifdef __VXWORKS__
#include <vxWorks.h>
#endif

#include "Defines.h"

namespace Rhesus
{
namespace Toolkit
{
	template <class T>
	class ManagedArray
	{
	public:
		ManagedArray(const INT32& length);
		~ManagedArray();
		
		const T& operator[](const INT32 i) const; // getter
		T& operator[](const INT32 i); // setter
		
		T& Get(const INT32& index);
		void Set(const INT32& index, const T& t);
		
		INT32 Length();
	private:
		void allocate(const INT32& length);
		void deallocate();
		
		T* m_internalArray;
		
		INT32 m_length;
	};
}
}

#endif
