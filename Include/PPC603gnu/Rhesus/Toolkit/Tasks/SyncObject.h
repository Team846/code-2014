#ifndef RHESUS_SYNCOBJECT_H_
#define RHESUS_SYNCOBJECT_H_

#include "../Defines.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Tasks
{
	
	class SyncObject
	{
	public:
		static const int TIMEOUT_WAIT_FOREVER;
		static const int TIMEOUT_NO_WAIT;
		
		static const int STATE_FULL;
		static const int STATE_EMPTY;
		
	private:
		virtual void _() = 0; // dummy to make this an interface
	};
	
}
}
}

#endif
