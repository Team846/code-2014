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

	cfsetospeed (&ts, kBaudRateCRio9074);
	cfsetispeed (&ts, kBaudRateCRio9074);

	ts.c_cflag = (ts.c_cflag & ~CSIZE) | CS8;
    ts.c_iflag &= ~IGNBRK;
    ts.c_lflag = 0;
    ts.c_oflag = 0;
    ts.c_cc[VMIN]  = 0;
    ts.c_cc[VTIME] = 10;
    ts.c_iflag &= ~(IXON | IXOFF | IXANY);
    ts.c_cflag |= (CLOCAL | CREAD);
    ts.c_cflag &= ~(PARENB | PARODD);
    ts.c_cflag |= parity;
    ts.c_cflag &= ~CSTOPB;
    ts.c_cflag &= ~CRTSCTS;

	if(tcsetattr(pFD, TCSANOW, &ts) != 0)
	{
		// RHESUS NO THROW
		return;
	}
}

INT32 SerialConnection::SendRaw(MessageBuffer& buff)
{
	const UINT8* buff = buff.GetBuffer();
	int len = buff.GetBufferLength();

	if(len < 0 || len > 255)
	{
		// RHESUS NO THROW
		return -1;
	}

	UINT8 cLen = (UINT8)len;

	write(pFD, &cLen, 1);
	write(pFD, buff, len);
	fsync(pFD);
}

#endif