#ifndef RHESUS_EXECUTION_STATE_H_
#define RHESUS_EXECUTION_STATE_H_

namespace Rhesus 
{
namespace Toolkit 
{
namespace Scripting 
{
namespace ExecutionState
{
	enum Enum
	{
		FINISHED = 0x00,
		PAUSED = 0x01,
		EXEC_ERROR = 0x02
	};
}

}
}
}

#endif
