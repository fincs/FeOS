#include <sndlock.h>
#include <time.h>

static const char* g_lockName;
static sndlock_t g_lock;
static word_t g_ranSeed;

// http://en.wikipedia.org/wiki/Lehmer_RNG#Sample_C99_code
static word_t quickRand16()
{
	if (g_ranSeed == 0)
		g_ranSeed = (word_t)time(NULL) ^ (word_t)&g_ranSeed;

	g_ranSeed = (word_t)(((u64)g_ranSeed * 279470273UL) % 4294967291UL);
	return g_ranSeed & 0xFFFF;
}

static word_t quickRand()
{
	return quickRand16() | (quickRand16() << 16);
}

FEOS_EXPORT bool SndLock_Acquire(const char* name, sndlock_t* pLock)
{
	if (g_lockName || !name || !pLock) return false;
	g_lockName = name;
	*pLock = (g_lock = quickRand());
	return true;
}

FEOS_EXPORT bool SndLock_Release(sndlock_t lock)
{
	if (!g_lockName || g_lock != lock) return false;
	g_lockName = NULL;
	return true;
}

FEOS_EXPORT const char* SndLock_GetName()
{
	return g_lockName;
}
