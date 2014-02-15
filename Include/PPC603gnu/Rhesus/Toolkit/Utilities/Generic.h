#ifndef RHESUS_GENERIC_H_
#define RHESUS_GENERIC_H_

#include "../Defines.h"
#include "remove_reference.h"
#include <typeinfo>

namespace Rhesus
{
namespace Toolkit
{
namespace Utilities
{

	/*!
	 * @brief Generic storage of any type.  Based on boost::any.
	 * @author Tony Peng
	 * @author Varun Parthasarathy
	 */
	class Generic
	{	
		
	private:
		struct placeholder
		{
			virtual ~placeholder() {}
			
			virtual const std::type_info& type() const = 0;
		};
		
		template<typename T>
		struct holder : public placeholder
		{
			T val;
			
			holder(T value)
				: val(value)
			{
			}
			
			const std::type_info& type() const
			{
				return typeid(T);
			}
		};
		
		placeholder* m_storedValue;

	public:
		
		Generic()
			: m_storedValue(NULL)
		{
		}
		
		Generic(const Generic& other)
			: m_storedValue(other.m_storedValue ? other.m_storedValue : NULL)
		{
			
		}
		
		template<typename T>
		Generic(const T& value)
			: m_storedValue(new holder<T>(value))
		{
		}
		
		~Generic()
		{
			R_DELETE(m_storedValue);
		}
		
		/*!
		 * @return whether or not the object holds a value
		 */
		bool isEmpty()
		{
			return m_storedValue == NULL;
		}
		
		template<typename T>
		Generic& operator=(const T& rhs)
		{
			m_storedValue = Generic(rhs).m_storedValue;
			
			return *this;
		}
		
		Generic& operator=(Generic rhs)
		{
			m_storedValue = rhs.m_storedValue;
			
			return *this;
		}
		
		placeholder* Held()
		{
			return m_storedValue;
		}
		
		const std::type_info& type() const
		{
			return (m_storedValue) ? m_storedValue->type() : typeid(void);
		}
	};

	template<typename T>
	T* generic_cast(Generic* var)
	{
		return (var && typeid(T) == var->type()) ? &(static_cast<Generic::holder<T>*>(var->Held())->val) :
			0;
	}
	
	template<typename T>
	inline const T* generic_cast(const Generic* var)
	{
		return generic_cast<T>(const_cast<Generic*>(var));
	}
	
	template<typename T>
	T generic_cast(Generic& var)
	{ 
		typedef typename remove_reference<T>::type nonref;
		
		nonref* res = generic_cast<nonref>(&var);
		
		if(!res) throw std::exception(); // change this to a more descriptive exception later
		
		return *res;
	}
	
	template<typename T>
	inline T generic_cast(const Generic& var)
	{
		typedef typename remove_reference<T>::type nonref;
		
		return generic_cast<const nonref&>(const_cast<Generic&>(var));
	}
	
	template<typename T>
	inline T* unsafe_generic_cast(Generic* var)
	{
		return &(static_cast<Generic::holder<T>*>(var->m_storedValue)->val);
	}
	
	template<typename T>
	inline const T* unsafe_generic_cast(const Generic* var)
	{
		return unsafe_generic_cast<T>(const_cast<Generic*>(var));
	}
}
}
}

#endif
