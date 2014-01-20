#ifndef SHOOTER_LOADER_DATA_H_
#define SHOOTER_LOADER_DATA_H_

#include "ComponentData.h"

class ShooterLoaderData : public ComponentData
{
public:
	ShooterLoaderData();

	void ResetCommands();
	void Log();
};

#endif
