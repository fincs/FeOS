#include "feos.h"
#include "feosfifo.h"
#include <stdio.h>

void FeOS_IRQPoll();

//-----------------------------------------------------------------------------
// User-mode FIFO handler support, to be moved to a separate file
//-----------------------------------------------------------------------------

enum
{
	QUEUE_DATAMSG, QUEUE_VALUE32, QUEUE_ADDRESS
};

typedef struct tagFifoMsgQueueEntry
{
	struct tagFifoMsgQueueEntry* next;
	u16 type, channel;
	//void* userdata;
	union
	{
		int datamsgSize;
		word_t value32;
		void* address;
	};
} FifoMsgQueueEntry;

// Ersatz memory allocator

#define FIFOMSGSTACK_SIZE (4*1024)

static byte_t fifoMsgStack[FIFOMSGSTACK_SIZE];
static int fifoMsgStackPos = FIFOMSGSTACK_SIZE;

static FifoMsgQueueEntry* FeOS_AllocFifoMsg()
{
	int newpos = fifoMsgStackPos - sizeof(FifoMsgQueueEntry);
	if (newpos < 0)
		return NULL;
	return (FifoMsgQueueEntry*)(fifoMsgStack + (fifoMsgStackPos = newpos));
}

// User-mode FIFO message queue data structure. This must be done in
// assembly because a STRD instruction is needed to set both fields
// at the same time.

asm("\
.text\n\
	.align 3\n\
	.global __clearFifoMsgQueue\n\
__clearFifoMsgQueue:\n\
	mov r0, #0\n\
	mov r1, #0\n\
	strd r0, [pc]\n\
	bx lr\n\
	.global __fifoMsgQueue\n\
__fifoMsgQueue:\n\
	.word 0\n\
	.global __fifoMsgQueueTail\n\
__fifoMsgQueueTail:\n\
	.word 0");

#define fifoMsgQueue __fifoMsgQueue
#define fifoMsgQueueTail __fifoMsgQueueTail

extern FifoMsgQueueEntry* __fifoMsgQueue;
extern FifoMsgQueueEntry* __fifoMsgQueueTail;

void __clearFifoMsgQueue();

static void FeOS_AddFifoMsgToQueue(FifoMsgQueueEntry* msg)
{
	int cs = enterCriticalSection();

	msg->next = NULL;

	if (fifoMsgQueueTail)
		fifoMsgQueueTail->next = msg;
	else
		fifoMsgQueue = msg;

	fifoMsgQueueTail = msg;

	leaveCriticalSection(cs);
}

void FeOS_RunFifoQueue()
{
	FifoMsgQueueEntry* ent = fifoMsgQueue;

	while(ent)
	{
		// Do stuff with ent
		FifoMsgQueueEntry* next = ent->next;
		ent = next;
	}

	__clearFifoMsgQueue();
	fifoMsgStackPos = FIFOMSGSTACK_SIZE;
}

static void* userdata_array[FIFO_PROG_CH_NUM];

static void StubFifoDatamsgHandler(int datamsgSize, void* userdata)
{
	FifoMsgQueueEntry* msg = FeOS_AllocFifoMsg();
	msg->type = QUEUE_DATAMSG;
	msg->channel = (u16) (word_t) userdata;
	msg->datamsgSize = datamsgSize;
	FeOS_AddFifoMsgToQueue(msg);
}

static void StubFifoValue32Handler(word_t value32, void* userdata)
{
	FifoMsgQueueEntry* msg = FeOS_AllocFifoMsg();
	msg->type = QUEUE_VALUE32;
	msg->channel = (u16) (word_t) userdata;
	msg->value32 = value32;
	FeOS_AddFifoMsgToQueue(msg);
}

static void StubFifoAddressHandler(void* address, void* userdata)
{
	FifoMsgQueueEntry* msg = FeOS_AllocFifoMsg();
	msg->type = QUEUE_ADDRESS;
	msg->channel = (u16) (word_t) userdata;
	msg->address = address;
	FeOS_AddFifoMsgToQueue(msg);
}

void _SetDatamsgHandler(int channel, bool set)
{
	if (set)
		fifoSetDatamsgHandler(channel, StubFifoDatamsgHandler, (void*) channel);
	else
		fifoSetDatamsgHandler(channel, NULL, NULL);
}

void FeOS_FifoSetDatamsgHandler(int channel, VoidFn handler, void* userdata)
{
	userdata_array[channel - FIFO_PROG_CH] = userdata;
	//FeOS_swi_SetDatamsgHandler(channel, !!handler);
	//Here the handler would be written
	FeOS_RunFifoQueue();
}

//-----------------------------------------------------------------------------
// User mode IRQ support
//     Based on libnds code
//-----------------------------------------------------------------------------

#define MAX_IRQS 16
#define GET_IRQ ((VoidFn)0xFFFFFFFF)

static struct IntTable irqTable[MAX_IRQS];

VoidFn FeOS_SetInterrupt(word_t mask, VoidFn fn)
{
	if (!mask) return NULL;

	int i;
	for (i = 0; i < MAX_IRQS; i ++)
		if (!irqTable[i].mask || irqTable[i].mask == mask) break;

	if (i == MAX_IRQS) return NULL;

	VoidFn oldFn = irqTable[i].handler;

	if (fn != GET_IRQ)
	{
		// For IRQ-safety reasons, the registering MUST be done in this order
		irqTable[i].handler = fn;
		irqTable[i].mask = mask;
	}

	return oldFn;
}

static void FeOS_ProcessIRQ(word_t flags)
{
	// Check for FIFO recv IRQ before everything else
	if (flags & IRQ_FIFO_EMPTY)
	{
		FeOS_RunFifoQueue();

		// We don't want user code responding to this IRQ
		flags &= ~IRQ_FIFO_EMPTY;
	}

	int i; word_t mask; VoidFn fn;
	for (i = 0; i < MAX_IRQS && (mask = irqTable[i].mask); i ++)
		if ((mask & flags) && (fn = irqTable[i].handler))
		{
			fn();
			break;
		}
}

word_t FeOS_CheckPendingIRQs()
{
	word_t totalflags = 0;
	for(;;) // IRQ/FIFO might fire while the handlers are executing
	{
		word_t flags = __ARMSWP(0, &INTR_WAIT_FLAGS);
		if (flags)
			FeOS_ProcessIRQ(flags);
		else
			break;
		totalflags |= flags;
	}
	return totalflags;
};

void FeOS_WaitForIRQ(word_t mask)
{
	FeOS_CheckPendingIRQs();
	for(;;)
	{
		FeOS_IRQPoll();
		if (FeOS_CheckPendingIRQs() & mask)
			break;
	}
}
