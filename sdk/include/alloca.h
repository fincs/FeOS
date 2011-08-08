//
// FeOS C Standard Library
// alloca.h
//     Stack allocation function
//

#pragma once

#define alloca(size) __builtin_alloca(size)
#define _alloca alloca
