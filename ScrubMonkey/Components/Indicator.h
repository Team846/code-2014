#include "Component.h"
#include "../ComponentData/IndicatorData.h"

#include "I2C.h"

class Indicator: public Component
{
public:
	Indicator();
	
	void OnEnabled();
	void OnDisabled();
	
	void UpdateEnabled();
	void UpdateDisabled();
private:
	I2C* m_i2c;
	IndicatorData* m_indicatorData;
	

};
