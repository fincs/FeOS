////
// FeOS C++ Standard Library
// BidiIter.h
//     Bidirectional iterator support
//

#pragma once
#include <iter/ForwardIter.h>

namespace std
{
	namespace iter
	{
		template <class T>
		class IBidiIter : public IForwardIter<T> // bidirectional iterator
		{
		public:
			typedef IBidiIter<T> _Self;
			virtual _Self& operator --() = 0;
			virtual _Self& operator --(int) = 0;
		};

		template <class T>
		class CReverseBidiIter : public CReverseFwIter<T>
		{
		public:
			inline CReverseBidiIter& operator --() { this->mPtr ++; return *this; }
			inline CReverseBidiIter operator --(int) { CReverseBidiIter prev(*this); this->mPtr ++; return prev; }
		};
	}
}
