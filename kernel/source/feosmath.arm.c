#include "feos.h"
#include "fxe.h"

extern bool conMode;

static inline void divmod3232(int a, int b)
{
	REG_DIVCNT = DIV_32_32;
	while(REG_DIVCNT & DIV_BUSY);

	REG_DIV_NUMER_L = a;
	REG_DIV_DENOM_L = b;
	while(REG_DIVCNT & DIV_BUSY);
}

static inline void divmod6432(long long a, int b)
{
	REG_DIVCNT = DIV_64_32;
	while(REG_DIVCNT & DIV_BUSY);

	REG_DIV_NUMER   = a;
	REG_DIV_DENOM_L = b;
	while(REG_DIVCNT & DIV_BUSY);
}

static inline void divmod6464(long long a, long long b)
{
	REG_DIVCNT = DIV_64_64;
	while(REG_DIVCNT & DIV_BUSY);

	REG_DIV_NUMER = a;
	REG_DIV_DENOM = b;
	while(REG_DIVCNT & DIV_BUSY);
}

int FeOS_swi_div3232(int, int);
int FeOS_div3232(int a, int b)
{
	if (conMode && isUserMode()) return FeOS_swi_div3232(a, b);
	divmod3232(a, b);
	return REG_DIV_RESULT_L;
}

int FeOS_swi_mod3232(int, int);
int FeOS_mod3232(int a, int b)
{
	if (conMode && isUserMode()) return FeOS_swi_mod3232(a, b);
	divmod3232(a, b);
	return REG_DIVREM_RESULT_L;
}

int FeOS_swi_div6432(long long, int);
int FeOS_div6432(long long a, int b)
{
	if (conMode && isUserMode()) return FeOS_swi_div6432(a, b);
	divmod6432(a, b);
	return REG_DIV_RESULT_L;
}

int FeOS_swi_mod6432(long long, int);
int FeOS_mod6432(long long a, int b)
{
	if (conMode && isUserMode()) return FeOS_swi_mod6432(a, b);
	divmod6432(a, b);
	return REG_DIVREM_RESULT_L;
}

long long FeOS_swi_div6464(long long, long long);
long long FeOS_div6464(long long a, long long b)
{
	if (conMode && isUserMode()) return FeOS_swi_div6464(a, b);
	divmod6464(a, b);
	return REG_DIV_RESULT;
}

long long FeOS_swi_mod6464(long long, long long);
long long FeOS_mod6464(long long a, long long b)
{
	if (conMode && isUserMode()) return FeOS_swi_mod6464(a, b);
	divmod6464(a, b);
	return REG_DIVREM_RESULT;
}

unsigned int FeOS_swi_sqrt32(unsigned int);
unsigned int FeOS_sqrt32(unsigned int a)
{
	if (conMode && isUserMode()) return FeOS_swi_sqrt32(a);

	REG_SQRTCNT = SQRT_32;
	while(REG_SQRTCNT & SQRT_BUSY);

	REG_SQRT_PARAM_L = a;
	while(REG_SQRTCNT & SQRT_BUSY);

	return REG_SQRT_RESULT;
}

unsigned int FeOS_swi_sqrt64(long long);
unsigned int FeOS_sqrt64(long long a)
{
	if (conMode && isUserMode()) return FeOS_swi_sqrt64(a);

	REG_SQRTCNT = SQRT_64;
	while(REG_SQRTCNT & SQRT_BUSY);

	REG_SQRT_PARAM = a;
	while(REG_SQRTCNT & SQRT_BUSY);

	return REG_SQRT_RESULT;
}

BEGIN_TABLE(FEOSMATH)
	ADD_FUNC(sinLerp),
	ADD_FUNC(cosLerp),
	ADD_FUNC(tanLerp),
	ADD_FUNC(asinLerp),
	ADD_FUNC(acosLerp),
	ADD_FUNC(FeOS_div3232),
	ADD_FUNC(FeOS_mod3232),
	ADD_FUNC(FeOS_div6432),
	ADD_FUNC(FeOS_mod6432),
	ADD_FUNC(FeOS_div6464),
	ADD_FUNC(FeOS_mod6464),
	ADD_FUNC(FeOS_sqrt32),
	ADD_FUNC(FeOS_sqrt64)
END_TABLE(FEOSMATH)

MAKE_FAKEMODULE(FEOSMATH)
