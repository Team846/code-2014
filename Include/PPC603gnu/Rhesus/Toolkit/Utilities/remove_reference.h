#ifndef RHESUS_REMOVE_REFERENCE_H_
#define RHESUS_REMOVE_REFERENCE_H_

namespace Rhesus
{
namespace Toolkit
{
namespace Utilities
{
	template< class T > struct remove_reference      {typedef T type;};
	template< class T > struct remove_reference<T&>  {typedef T type;};
	
#ifdef RHESUS_HAS_CPP11
	template< class T > struct remove_reference<T&&> {typedef T type;};
#endif
}
}
}

#endif
