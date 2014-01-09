#ifndef RUNIT_TEST_APPARATUS_H_
#define RUNIT_TEST_APPARATUS_H_

#include "Defines.h"

#include <string>
#include <vector>

#include "RUnit_Assert.h"

#include "RUnit_Test.h"

namespace RUnit
{

class TestApparatus
{
public:
	static const unsigned char OPT_QUIET;
	static const unsigned char OPT_BASIC;
	static const unsigned char OPT_VERBOSE;
	
	TestApparatus();
	
	void EnableOption(unsigned char option);
	void DisableOption(unsigned char option);
	bool OptionEnabled(unsigned char option);
	
	void RegisterTest(RU_FUNCPTR func, std::string testName);
	
	void operator()(RU_FUNCPTR func, std::string testName) // shorthand
	{
		RegisterTest(func, testName);
	}
	
	void Run();
	
	void Reset();
	
private:
	unsigned char m_options;
	
	std::vector<Test> m_tests;
	int m_failedCount;
};

}
#endif
