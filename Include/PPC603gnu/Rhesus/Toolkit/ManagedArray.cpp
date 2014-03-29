#include "ManagedArray.h"
#include "IndexOutOfBoundsException.h"
#include "ArgumentOutOfRangeException.h"
#ifdef RHESUS_NO_THROW
#include "error_val.h"
#endif

using namespace Rhesus::Toolkit;

template <class T>
ManagedArray<T>::ManagedArray(const INT32& length)
{
	m_internalArray = NULL;
	
	allocate(length);
}

template <class T>
ManagedArray<T>::~ManagedArray()
{
	deallocate();
}

template <class T>
const T& ManagedArray<T>::operator[](const INT32 i) const
{
	return Get(i);
}

template <class T>
T& ManagedArray<T>::operator[](const INT32 i)
{
	return Get(i);
}

template <class T>
T& ManagedArray<T>::Get(const INT32& index)
{
	if(index < 0 || index >= m_length)
	{
#ifdef RHESUS_NO_THROW
		error_val<T> err;
		
		return err();
#else
		throw IndexOutOfBoundsException("Index was outside the bounds of the array!");
		return;
#endif
	}
	
	return m_internalArray[index];
}

template <class T>
void ManagedArray<T>::Set(const INT32& index, const T& t)
{
	if(index < 0 || index >= m_length)
	{
#ifndef RHESUS_NO_THROW
		throw IndexOutOfBoundsException("Index was outside the bounds of the array!");
#endif
		return;
	}
	
	m_internalArray[index] = t;
}

template <class T>
int ManagedArray<T>::Length()
{
	return m_length;
}

template <class T>
void ManagedArray<T>::allocate(const INT32& length)
{
	if(length < 0)
	{
		m_length = 0;
		
#ifndef RHESUS_NO_THROW
		throw ArgumentOutOfRangeException("Invalid length specified.");
#else
		return;
#endif
	}
		
	m_length = length; // remember how long the array is so we can perform checks
	
	m_internalArray = new T[length];
}

template <class T>
void ManagedArray<T>::deallocate()
{
	// did we already clean up?
	if(m_internalArray == NULL)
		return;
	
	for(int i = 0; i < m_length; i++)
		delete m_internalArray[i];
	
	delete[] m_internalArray;
	m_internalArray = NULL;
	
	m_length = 0;
}
