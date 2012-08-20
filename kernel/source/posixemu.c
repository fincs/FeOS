#include "feos.h"
#include "fxe.h"
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

static inline int _fileno_helper(FILE* f) { return fileno(f); }

BEGIN_TABLE(FEOSPOSIXEMU)
	// Directory functions
	ADD_FUNC(opendir),
	ADD_FUNC(closedir),
	ADD_FUNC(readdir),
	ADD_FUNC(rewinddir),
	ADD_FUNC(seekdir),
	ADD_FUNC(telldir),
	ADD_FUNC(mkdir),
	ADD_FUNC(fstat),
	ADD_FUNC_ALIAS(_fileno_helper, fileno),

	// CWD functions
	ADD_FUNC(chdir),
	ADD_FUNC(getcwd),

	// Misc functions
	ADD_FUNC(stat),
	ADD_FUNC(ftruncate),

	// POSIX I/O
	ADD_FUNC(open),
	ADD_FUNC(close),
	ADD_FUNC(read),
	ADD_FUNC(write),
	ADD_FUNC(lseek),
	ADD_FUNC(fdopen)
END_TABLE(FEOSPOSIXEMU)

MAKE_FAKEMODULE(FEOSPOSIXEMU)
