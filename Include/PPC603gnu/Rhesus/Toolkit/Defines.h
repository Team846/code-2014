#ifndef RHESUS_DEFINES_H_
#define RHESUS_DEFINES_H_

#define DELETE(p) delete p; \
	p = NULL;

#define DELETE_ARR(p) delete[] p; \
	p = NULL;

#define OVERRIDE

#define DEPRECATED __attribute__ ((deprecated))

#endif
