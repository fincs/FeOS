////
// FeOS C++ Standard Library
// IterBase.h
//     Internal basis for template-less iterator support
//

#pragma once
#include <iter/ForwardIter.h>
#include <iter/BidiIter.h>

namespace std
{
	namespace iter
	{
		// A generic iterator implementation for arrays (CGeneric*Iter<T>) is
		// not needed because a pointer to said array of T is enough (!).

		// A generic *reverse* iterator implementation, OTOH, is required though,
		// because ++ actually decrements the pointer on such an iterator.
	}
}
