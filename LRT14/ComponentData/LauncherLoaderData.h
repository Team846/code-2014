#ifndef LAUNCHER_LOADER_DATA_H_
#define LAUNCHER_LOADER_DATA_H_

#include "ComponentData.h"

/*!
 * @brief Contains commands and states for the launcher loader component.
 */
class LauncherLoaderData : public ComponentData
{
public:
	LauncherLoaderData();
	static LauncherLoaderData* Get();

	void SetFire(bool shouldFire);
	bool GetFire();
	void SetPurge(bool purge);
	bool GetPurge();
	void SetLoad(bool load);
	bool GetLoad();
	void SetHairTrigger(bool trigger);
	bool GetHairTrigger();

	bool IsLoadingComplete();
	float GetSensorValue();
	bool IsBallDetected();
	
	void ResetCommands();
	void Log();
	
private:
	void SetLoadingComplete(bool complete);
	void SetSensorValue(float value);
	void SetBallDetected(bool detected);
	
	bool m_fire;
	bool m_purge;
	bool m_load;
	bool m_trigger;
	bool m_complete;
	float m_value;
	bool m_ballDetected;
	
	friend class LauncherLoader;
};

#endif
