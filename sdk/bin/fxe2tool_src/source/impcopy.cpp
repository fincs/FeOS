#include <stdio.h>
#include <map>

#include "types.h"
#include "elf.h"
#include "fxe2.h"

using std::map;

static map<word_t, word_t> impcopytbl;

extern "C" void AddImpCopy(word_t from, word_t to)
{
	impcopytbl[to] = from;
}

extern "C" int HasImpCopy()
{
	return !impcopytbl.empty();
}

extern "C" void WriteImpCopy(FILE* f)
{
	// Write header
	{
		fxe2_impcopy_head_t head = { FX2_IMPCOPY_NORMAL, (word_t)impcopytbl.size() };
		fwrite(&head, sizeof(fxe2_impcopy_head_t), 1, f);
	}

	// Write entries
	map<word_t, word_t>::iterator it;
	for (it = impcopytbl.begin() ; it != impcopytbl.end(); it ++)
	{
		fxe2_impcopy_t st = { (*it).second, (*it).first };
		fwrite(&st, sizeof(fxe2_impcopy_t), 1, f);
	}
}
