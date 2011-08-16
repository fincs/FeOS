////
// FeOS C++ Standard Library
// ForwardIter.h
//     Forward iterator support
//

#pragma once

namespace std
{
	namespace iter
	{
		template <class T>
		class IForwardIter
		{
		public:
			typedef IForwardIter<T> _Self;
			typedef T _Type;
			virtual bool operator ==(const _Self&) = 0;
			virtual bool operator !=(const _Self&) = 0;
			virtual _Type& operator *() = 0;
			virtual _Type* operator ->() = 0;
			virtual _Self& operator ++() = 0;
			virtual _Self& operator ++(int) = 0;
		};

		template <class T>
		class CReverseFwIter
		{
		protected:
			T* mPtr;
		public:
			inline CReverseFwIter() : mPtr(NULL) {}
			inline CReverseFwIter(T* ptr) : mPtr(ptr) {}

			inline bool operator ==(const CReverseFwIter& it) const { return mPtr == it.mPtr; }
			inline bool operator !=(const CReverseFwIter& it) const { return mPtr != it.mPtr; }
			inline T& operator *() const { return *mPtr; }
			inline T* operator ->() const { return mPtr; }
			inline CReverseFwIter& operator ++() { mPtr --; return *this; }
			inline CReverseFwIter operator ++(int) { CReverseFwIter prev(*this); mPtr --; return prev; }
		};
	}
}
