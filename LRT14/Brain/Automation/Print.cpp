#include "Print.h"

Print::Print(std::string msg):
	Automation("Print")
{
	m_msg = msg;
}

bool Print::Start()
{
	return true;
}

bool Print::Run()
{
	BufferedConsole::Println(m_msg);
	return true;
}

bool Print::Abort()
{
	return true;
}

void Print::AllocateResources()
{
	
}
