#include "../Includes.h"

#if !USE_WPILIB && BUILD_ENV==BUILD_ENV_LINUX

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

SerialConnection::SerialConnection(std::string path)
	: mPath(path)
{
	
}

SerialConnection::~SerialConnection()
{
}

void SerialConnection::Open()
{
	pFD = open(mPath.c_str(), O_RDWR | O_NOCTTY);

	termios ts;
	memset(&ts, 0, sizeof(ts));

	if(tcgetattr(pFD, &ts) != 0)
	{
		// RHESUS NO THROW
		return;
	}


}

#endif