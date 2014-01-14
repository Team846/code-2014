#include "MessageBuffer.h"

using namespace Rhesus::Messenger;

#include <cstdio>
#include <iostream>
#include <bitset>

int main()
{
	MessageBuffer buff;

	buff.Write(500);
	buff.Write(true);
	buff.Write(std::string("test"));


	const UINT8* theBuff = buff.GetBuffer();
	INT32 len = buff.GetBufferLength();

	//for(int i = 0; i < len; i++)
	//{
	//	std::bitset<8> bs(theBuff[i]);
	//	std::cout << bs << " ";
	//}

	//printf("\n");

	MessageBuffer buff2(buff.GetBuffer(), buff.GetBufferLength());

	std::printf("%d\n", buff2.ReadInt32());
	std::printf("%s\n", (buff2.ReadBool() ? "True" : "False"));
	std::printf("%s\n", buff2.ReadStdString().c_str());

	system("PAUSE");
}