#pragma once
#include "feos.h"

typedef struct
{
	byte_t ids[IDMGR_MAXIDS];
	int free;
} idmgr_t;

static inline void IdMgrInit(idmgr_t* mgr)
{
	int i;
	for (i = 0; i < IDMGR_MAXIDS; i ++)
		mgr->ids[i] = i;
	mgr->free = 0;
}

static inline int IdMgrAlloc(idmgr_t* mgr)
{
	if (mgr->free == IDMGR_MAXIDS)
		return -1;

	return mgr->ids[mgr->free++];
}

static inline void IdMgrFree(idmgr_t* mgr, int id)
{
	mgr->ids[--mgr->free] = id;
}
