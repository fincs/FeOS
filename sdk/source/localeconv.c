#include <locale.h>
#include <limits.h>

static struct lconv _dummy_loc =
{
	(char*)".",(char*)"",(char*)"",(char*)"",(char*)"",(char*)"",(char*)"",(char*)"",(char*)"",(char*)"",
	CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX,CHAR_MAX
};

struct lconv* localeconv()
{
	return &_dummy_loc;
}
