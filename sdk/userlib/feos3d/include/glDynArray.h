/*---------------------------------------------------------------------------------

	A simple vector like dynamic data structure (feos3d version)

  Copyright (C) 2005
			Jason Rogers (dovoto)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any
  damages arising from the use of this software.

  Permission is granted to anyone to use this software for any
  purpose, including commercial applications, and to alter it and
  redistribute it freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you
     must not claim that you wrote the original software. If you use
     this software in a product, an acknowledgment in the product
     documentation would be appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and
     must not be misrepresented as being the original software.
  3. This notice may not be removed or altered from any source
     distribution.


---------------------------------------------------------------------------------*/
/*! \file glDynArray.h
	\brief A dynamically resizing array for general use.
*/

#ifndef __glDynArray_H__
#define __glDynArray_H__

#include <stdlib.h>
#include <string.h>

//#include <nds/ndstypes.h>
#include <feos.h>


//! A resizable array
typedef struct glDynArray
{
	void** data; 			//!< pointer to array of void pointers
	unsigned int cur_size; 	//!< currently allocated size of the array
}glDynArray;



/*! \brief Initializes an array with the supplied initial size
	\param v the array to initialize
	\param initialSize the initial size to allocate
	\return a pointer to the data, or NULL on error.
*/
void* glDynArrayInit(glDynArray* v, unsigned int initialSize);


/*! \brief Frees memory allocated by the dynamic array
	\param v The array to delete
*/
void glDynArrayDelete(glDynArray* v);


/*! \brief Gets the entry at the supplied index
	\param v The array to get from.
	\param index The index of the data to get.
	\return The data or NULL if v is NULL or the index is out of range.
*/
void* glDynArrayGet(glDynArray* v, unsigned int index);


/*! \brief Sets the entry to the supplied value
	\param v The array to set
	\param index The index of the data to set (array will be resized to fit the index).
	\param item The data to set.
	\return false if v is NULL or there isn't enough memory, true otherwise
*/
bool glDynArraySet(glDynArray *v, unsigned int index, void* item);


#endif
