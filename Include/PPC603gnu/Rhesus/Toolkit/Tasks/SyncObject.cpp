#include "SyncObject.h"

using namespace Rhesus::Toolkit::Tasks;

#ifdef __VXWORKS__
	const int SyncObject::TIMEOUT_WAIT_FOREVER = -1;
	const int SyncObject::TIMEOUT_NO_WAIT = 0;
	
	const int SyncObject::STATE_FULL = 1;
	const int SyncObject::STATE_EMPTY = 0;
#endif
