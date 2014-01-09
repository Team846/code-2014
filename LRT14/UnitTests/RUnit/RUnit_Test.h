#ifndef RUNIT_TEST_H_
#define RUNIT_TEST_H_

#include <string>

#include "TestResult.h"

namespace RUnit
{

struct Test
{
	RU_FUNCPTR target;
	std::string name;
	TestResult result;
};

}

#endif
