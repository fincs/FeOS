////
// FeOS C++ Standard Library
// CharFwIter.h
//     `char` forward iterator
//

#pragma once
#include <iter/IterBase.h>

namespace std
{
	namespace iter
	{
		// Forward iterators
		typedef IForwardIter<char> ICharFwIter;
		typedef IForwardIter<const char> IConstCharFwIter;
		typedef char* CCharFwIter;
		typedef const char* CConstCharFwIter;
		typedef CReverseFwIter<char> CReverseCharFwIter;
		typedef CReverseFwIter<const char> CReverseConstCharFwIter;

		// Bidirectional iterators
		typedef IBidiIter<char> ICharBidiIter;
		typedef IBidiIter<const char> IConstCharBidiIter;
		typedef char* CCharBidiIter;
		typedef const char* CConstCharBidiIter;
		typedef CReverseBidiIter<char> CReverseCharBidiIter;
		typedef CReverseBidiIter<const char> CReverseConstCharBidiIter;
	}
}
