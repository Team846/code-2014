#ifndef FIRE_H_
#define FIRE_H_

#include "Automation.h"
#include "../../ComponentData/LauncherLoaderData.h"
#include "../../ComponentData/LauncherAngleData.h"

class Fire : public Automation
{
public:
	Fire();

	bool Start();
	bool Run();
	bool Abort();
	void AllocateResources();

private:
	LauncherLoaderData* m_loaderData;
	LauncherAngleData* m_angleData;
};

#endif
