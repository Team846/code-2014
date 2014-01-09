#include "TestApparatus.h"

#include "RUnitException.h"

using namespace RUnit;

const unsigned char TestApparatus::OPT_QUIET = 1; // 0001
const unsigned char TestApparatus::OPT_BASIC = 3; // 0011
const unsigned char TestApparatus::OPT_VERBOSE = 7; // 0111

TestApparatus::TestApparatus()
{
	m_options = 0;
	m_failedCount = 0;
}

void TestApparatus::EnableOption(unsigned char option)
{
	m_options |= option;
}

void TestApparatus::DisableOption(unsigned char option)
{
	m_options &= ~option;
}

bool TestApparatus::OptionEnabled(unsigned char option)
{
	return (m_options & option) == option;
}

void TestApparatus::RegisterTest(RU_FUNCPTR func, std::string testName)
{
	Test test;
	test.name = testName;
	test.target = func;
	
	m_tests.push_back(test);
}

void TestApparatus::Run()
{
	for(std::vector<Test>::iterator it = m_tests.begin(); it != m_tests.end(); ++it)
	{
		it->result = TestResult(TestResultType::NO_RESULT, TestStatus::IN_PROGRESS, "");
		
		bool success = true;
		
		try
		{
			it->target();
		}
		catch(RUnitException r)
		{
			++m_failedCount;
			
			it->result = TestResult(TestResultType::FAIL, TestStatus::DONE, r.errorMsg(), r.should(), r.actual());
			
			success = false;
		}
		
		if(success)
		{
			it->result = TestResult(TestResultType::SUCCESS, TestStatus::DONE, "Succeeded.");
		}
	}
}

void TestApparatus::Reset()
{
	m_options = 0;
	m_failedCount = 0;
	m_tests.clear();
}
