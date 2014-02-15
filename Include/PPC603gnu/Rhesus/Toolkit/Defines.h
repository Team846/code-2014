#ifndef RHESUS_DEFINES_H_
#define RHESUS_DEFINES_H_

#define R_DELETE(p) { delete p; \
	p = NULL; }

#define R_DELETE_ARR(p) { delete[] p; \
	p = NULL; }

#define OVERRIDE

#define DEPRECATED __attribute__ ((deprecated))

#define R_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#endif
