#ifndef RHESUS_IROBOTACTION_H_
#define RHESUS_IROBOTACTION_H_

namespace Rhesus
{
namespace Toolkit
{
namespace Action
{

	/*!
	 * @brief Base class for an object that represents a single robot action
	 * @author Varun Parthasarathy
	 */
	class IRobotAction
	{
	public:
		virtual void Run() = 0;
		
	};
	
}
}
}

#endif
