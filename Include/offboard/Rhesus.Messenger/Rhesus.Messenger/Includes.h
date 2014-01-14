#ifndef RHESUS_MSG_INCLUDES_H_
#define RHESUS_MSG_INCLUDES_H_

#define BUILD_ENV_WINDOWS	1
#define BUILD_ENV_LINUX		2
#define BUILD_ENV_VXWORKS	3

#define BUILD_ENV BUILD_ENV_WINDOWS

#if USE_WPILIB
#include <WPILib.h>
#define TIME_IN_SECONDS() Timer::GetFPGATimestamp();
#else
#define TIME_IN_SECONDS() clock() / (double) CLOCKS_PER_SEC;
#endif

#include "typedef.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>

#endif
