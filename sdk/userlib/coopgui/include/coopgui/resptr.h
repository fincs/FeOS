#pragma once

namespace FeOS
{
namespace UI
{

/** @addtogroup coopgui Cooperative GUI
 *  @{
 */

//! \brief Generic resource smart pointer datatype.
template <typename T>
class ResPtr
{
	T* ptr;

	inline void Clear()
	{
		if (ptr)
		{
			ptr->Dispose();
			ptr = nullptr;
		}
	}

	inline void Set(T* newPtr)
	{
		Clear();
		ptr = newPtr;
	}

public:

	inline ResPtr() : ptr(nullptr) { }
	inline ResPtr(T* newPtr) : ptr(newPtr) { }
	inline ResPtr(ResPtr<T>& oPtr) : ptr(oPtr.ptr) { oPtr.ptr = nullptr; }
	inline ~ResPtr() { Clear(); }

	inline ResPtr<T>& operator = (T* x)
	{
		Set(x);
		return *this;
	}

	inline ResPtr<T>& operator = (ResPtr<T>& x)
	{
		Set(x.ptr);
		x.ptr = nullptr;
		return *this;
	}

	inline T* operator -> () { return ptr; }
	inline operator T* () { return ptr; }
};

/** @} */

}
}
