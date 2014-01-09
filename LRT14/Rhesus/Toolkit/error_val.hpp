#ifndef RHESUS_NOTHROW_H_
#define RHESUS_NOTHROW_H_

namespace Rhesus
{
namespace Toolkit
{
	template<class T>
	class error_val
	{
	public:
		T operator()()
		{
			return T();
		}
	};
}
}

#endif
