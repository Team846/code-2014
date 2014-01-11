#ifndef RHESUS_IROBOTACTION_H_
#define RHESUS_IROBOTACTION_H_

namespace Rhesus
{
namespace Toolkit
{
namespace Action
{

	/*!
	 * @brief Base interface for an object that represents a single robot action
	 * @author Varun Parthasarathy
	 */
	class IRobotAction
	{
	public:
		/*!
		 * Virtual function that is called when the action is executed
		 */
		virtual void Run() = 0;
		
	};
	
}
}
}

#endif
