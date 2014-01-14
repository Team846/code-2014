#include "../Includes.h"

#if USE_WPILIB

SerialConnection::SerialConnection()
	: mSerialPort(kBaudRate9074),
	mUpdateTask((FUNCPTR)UpdateWorkerTask)
{
	
}

SerialConnection::~SerialConnection()
{

}

void SerialConnection::Open()
{
	mUpdateTask.Start((UINT32)this);
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

	mSerialPort.Write(&cLen, 1);
	mSerialPort.Write(buff, len);
	mSerialPort.Flush();
}

INT32 SerialConnection::UpdateWorkerTask(SerialConnection* conn)
{
	while(true)
	{
		MessageBuffer buff;
		int length = -1;
		int bRx = 0;

		while((bRx = mSerialPort.GetBytesReceived()) > 0)
		{
			UINT8 b;
			mSerialPort.Read(&b, 1);

			// assert we have space
			if(bRx < (INT32)b + 1) // + 1 for the length byte
			{
				// throw exception or error
				break;
			}

			UINT8 byteBuff[b];
			mSerialPort.Read(byteBuff, b);

			MessageBuffer buff(byteBuff, b);

			mMessageQ.push(buff);
		}

		taskDelay(sysClkRateGet() / 100); // sleep for 10 ms
	}
}


#endif