#ifndef RUNIT_EXCEPTION_H_
#define RUNIT_EXCEPTION_H_

#include <string>

namespace RUnit
{

class RUnitException
{
public:
	RUnitException(std::string should, std::string actual, std::string errorMsg)
		: m_shouldBe(should), m_actually(actual), m_errorMsg(errorMsg)
	{
		
	}
	
	std::string should()
	{
		return m_shouldBe;
	}
	
	std::string actual()
	{
		return m_actually;
	}
	
	std::string errorMsg()
	{
		return m_errorMsg;
	}
	
private:
	std::string m_shouldBe;
	std::string m_actually;
	std::string m_errorMsg;
};

}
#endif
