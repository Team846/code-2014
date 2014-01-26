#ifndef RHESUS_NEUTRALACTION_H_
#define RHESUS_NEUTRALACTION_H_

namespace Rhesus
{
namespace Toolkit
{
namespace Control
{
	/*!
	 * Describes what action to take when a SpeedController is sent a zero signal (neutral state).
	 */
	namespace NeutralAction
	{
		/*!
		 * @brief Enumeration for NeutralAction.
		 */
		enum Enum
		{
			/*!
			 * @brief Uses the jumper setting.
			 */
			Jumper,
			/*!
			 * @brief Uses the coast setting.
			 * @remarks This overrides the jumper setting.
			 */
			Coast,
			/*!
			 * @brief Uses the brake setting.
			 * @remarks This overrides the brake setting.
			 */
			Brake
		};
	}
}
}
}

#endif
