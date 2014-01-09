#ifndef RUNNING_SUM_H_
#define RUNNING_SUM_H_

namespace Rhesus
{
namespace Toolkit
{
namespace Control
{
	/*!
	 * @brief Maintains a running sum
	 */
	class RunningSum
	{
	public:
		/*!
		 * @brief Constructs a m_is_running sum
		 * @param decayConstant rate of decay
		 */
		RunningSum(double decayConstant) :
			decayConstant(decayConstant), runningSum(0)
		{
	
		}
	
		/*!
		 * @brief Adds a value into the sum
		 * @param x
		 * @return new value
		 */
		double UpdateSum(double x)
		{
			runningSum *= decayConstant;
			runningSum += x;
			return runningSum * (1 - decayConstant);
		}
	
		/*!
		 * @brief clears the m_is_running sum
		 */
		void Clear()
		{
			runningSum = 0;
		}
	
		/*!
		 * @brief sets the decay constant
		 * @param decayConstant
		 */
		void setDecayConstant(double decayConstant)
		{
			this->decayConstant = decayConstant;
		}
	
	private:
		double decayConstant;
		double runningSum;
	};

}
}
}

#endif
