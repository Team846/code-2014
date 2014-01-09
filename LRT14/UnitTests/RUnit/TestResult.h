#ifndef RUNIT_TEST_RESULT_H_
#define RUNIT_TEST_RESULT_H_

#include <string>

#include "TestResultType.h"
#include "TestStatus.h"

namespace RUnit
{

class TestResult
{
public:
	TestResult()
	{
		m_result = TestResultType::NO_RESULT;
		m_status = TestStatus::PENDING;
		m_message = "";
		m_failShouldBe = "";
		m_failActual = "";
	}
	
	TestResult(TestResultType::Enum res, TestStatus::Enum status, std::string msg)
	{
		m_result = res;
		m_status = status;
		m_message = msg;
		m_failShouldBe = "";
		m_failActual = "";
	}
	
	TestResult(TestResultType::Enum res, TestStatus::Enum status, std::string msg, std::string failShouldBe, std::string failActual)
	{
		m_result = res;
		m_status = status;
		m_message = msg;
		m_failShouldBe = failShouldBe;
		m_failActual = failActual;
	}
	
	TestResultType::Enum Result()
	{
		return m_result;
	}
	
	TestStatus::Enum Status()
	{
		return m_status;
	}
	
	std::string Message()
	{
		return m_message;
	}

	std::string ShouldBe()
	{
		return m_failShouldBe;
	}
	
	std::string Actual()
	{
		return m_failActual;
	}
private:
	TestResultType::Enum m_result;
	TestStatus::Enum m_status;
	
	std::string m_failShouldBe;
	std::string m_failActual;
	
	std::string m_message;
};

}
#endif
