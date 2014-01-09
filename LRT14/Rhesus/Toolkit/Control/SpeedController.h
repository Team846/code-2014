#ifndef RHESUS_SPEEDCONTROLLER_H_
#define RHESUS_SPEEDCONTROLLER_H_

#include <string>

#include "../Defines.h"

#include "NeutralAction.h"

namespace Rhesus
{
namespace Toolkit
{
namespace Control
{

	/*!
	 * @brief Communicates with a speed controller.  This is an abstract class.
	 */
	class SpeedController
	{
	public:
		explicit SpeedController(std::string name)
		{
			m_name = name;
			m_isEnabled = false;
			m_neutralSetting = NeutralAction::Coast;
		}
		
		virtual ~SpeedController()
		{
			
		}
		
		/* Properties/Accessors */
		std::string Name()
		{
			return m_name;
		}
		
		bool IsEnabled()
		{
			return m_isEnabled;
		}
		
		NeutralAction::Enum NeutralSetting()
		{
			return m_neutralSetting;
		}
		
		void NeutralSetting(NeutralAction::Enum setting)
		{
			m_neutralSetting = setting;
		}
		
		/* Methods */
		void Enable()
		{
			m_isEnabled = true;
		}
		
		void Disable()
		{
			m_isEnabled = false;
		}
		
		virtual void SetDutyCycle(float f) = 0;
		
		virtual void Update() = 0;
		
	private:
		std::string m_name;
		
		bool m_isEnabled;
		
		NeutralAction::Enum m_neutralSetting;
	};

}
}
}

#endif
