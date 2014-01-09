#ifndef RUNIT_DEFINES_H_
#define RUNIT_DEFINES_H_

#include <vxWorks.h>

#define DELETE(p) delete p; \
	p = NULL;

#define DELETE_ARR(p) delete[] p \
	p = NULL;

typedef INT32 (*RU_FUNCPTR)(...);

#endif
