#ifndef RUNIT_ASSERT_H_
#define RUNIT_ASSERT_H_

#include "RUnitException.h"

#include <string>

inline void RU_ASSERT(bool b, std::string failMessage)
{
	if(!b) throw RUnit::RUnitException("true", "false", failMessage);
}

inline void RU_ASSERT(bool b)
{
	RU_ASSERT(b, "RU_ASSERT() failed.");
}

inline void RU_ASSERT_FALSE(bool b, std::string failMessage)
{
	if(b) throw RUnit::RUnitException("false", "true", failMessage);
}

inline void RU_ASSERT_FALSE(bool b)
{
	RU_ASSERT_FALSE(b, "RU_ASSERT_FALSE() failed.");
}

inline void RU_ASSERT_EQU(void* ptr1, void* ptr2, std::string failMessage)
{
	if(ptr1 != ptr2) throw RUnit::RUnitException("true", "false", failMessage);
}

inline void RU_ASSERT_EQU(void* ptr1, void* ptr2)
{
	RU_ASSERT_EQU(ptr1, ptr2, "RU_ASSERT_EQU() failed.");
}

inline void RU_ASSERT_EQU(int a, int b, std::string failMessage)
{
	if(a != b) throw RUnit::RUnitException("true", "false", failMessage);
}

inline void RU_ASSERT_EQU(int a, int b)
{
	RU_ASSERT_EQU(a, b, "RU_ASSERT_EQU() failed.");
}

#endif
