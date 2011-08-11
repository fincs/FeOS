#include "feos.h"
#include "fxe.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

BEGIN_TABLE(FEOSPOSIXEMU)
	// Directory functions
	ADD_FUNC(opendir),
	ADD_FUNC(closedir),
	ADD_FUNC(readdir),
	ADD_FUNC(rewinddir),
	ADD_FUNC(seekdir),
	ADD_FUNC(telldir),
	ADD_FUNC(mkdir),

	// CWD functions
	ADD_FUNC(chdir),
	ADD_FUNC(getcwd),

	// Misc functions
	ADD_FUNC(stat)
END_TABLE(FEOSPOSIXEMU)

MAKE_FAKEMODULE(FEOSPOSIXEMU)
