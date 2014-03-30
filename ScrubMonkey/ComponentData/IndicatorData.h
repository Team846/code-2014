#include "ComponentData.h"

class IndicatorData: public ComponentData
{
public:
	enum Distance
	{
		DISTANCE_OFF = 0,
		CLOSE = 1 << 0,
		MIDDLE = 1 << 1,
		FAR = 1 << 2,
	};     
	
	enum Shooter
	{
		SHOOTER_OFF = 0,
		UNLOADED = 1 << 0,
		LOADED = 1 << 1,
		FIRING = 1 << 2
	};
	
	IndicatorData();
	
	static IndicatorData* Get();
	
	void SetShooterPattern(Shooter pattern);
	void SetDistancePattern(Distance pattern);
	
	Shooter GetShooterPattern(void);
	Distance GetDistancePattern(void);
	
private:	
	Distance m_distancePattern;
	Shooter m_shooterPattern;
	
	friend class Indicator;
	
};
