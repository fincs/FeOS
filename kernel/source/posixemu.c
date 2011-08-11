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

extern void* _inst_FEOSPOSIXEMU;

fxe_runtime_header _header_FEOSPOSIXEMU =
{
	&_inst_FEOSPOSIXEMU, // hThis
	"FEOSPOSIXEMU", // name
	1, // refcount
	-1, // file
	NULL, // entrypoint
	MAKE_EXPORTSTRUCT(FEOSPOSIXEMU), // exp
	{ 0, NULL }, // imp
	{ NULL, 0 }, // exidx
	NULL, // next
	NULL // prev
};

void* _inst_FEOSPOSIXEMU = &_header_FEOSPOSIXEMU;

