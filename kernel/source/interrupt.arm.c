#include "feos.h"
#include "feosfifo.h"
#include <stdio.h>

#ifndef ARM7
void DSIRQPoll();
#else
static void DSIRQPoll() { swiSetHaltCR(0x80); }
#endif

#ifdef ARM7
asm("\
.align 2\n\
.global __ARMSWP\n\
.type __ARMSWP STT_FUNC\n\
__ARMSWP:\n\
	swp r0, r0, [r1]\n\
	bx lr");

word_t __ARMSWP(word_t value, volatile word_t* addr);
#endif

//-----------------------------------------------------------------------------
// User-mode FIFO handler support, to be moved to a separate file
//-----------------------------------------------------------------------------

#define _MAKELIBNDSVER(A,B,C) ((A)*10000 + (B)*100 + (C))
#define _LIBNDS_VER _MAKELIBNDSVER(_LIBNDS_MAJOR_,_LIBNDS_MINOR_,_LIBNDS_PATCH_)

#if _LIBNDS_VER <= _MAKELIBNDSVER(1, 5, 7)

// fifoCheckDatamsgLength is amusingly missing from libnds versions up to 1.5.7.
// This code segment is to be removed when a new version is released.
#define FIFO_BUFFER_GETEXTRA(index) ((fifo_buffer[(index)*2]>>16)&0xFFF)
extern vu32 fifo_buffer[256*2];
typedef struct fifo_queue
{
	vu16 head;
	vu16 tail;
} fifo_queue;
extern fifo_queue fifo_data_queue[16];

int __attribute__((weak)) fifoCheckDatamsgLength(int channel)
{
       if(channel<0 || channel>=16) return -1;
       if(!fifoCheckDatamsg(channel)) return -1;

       int block = fifo_data_queue[channel].head;
       return FIFO_BUFFER_GETEXTRA(block);
}

#endif

static void* userdata_array[FIFO_PROG_CH_NUM];
static FifoDatamsgHandlerFunc datamsghnd_array[FIFO_PROG_CH_NUM];
static FifoValue32HandlerFunc value32hnd_array[FIFO_PROG_CH_NUM];
static FifoAddressHandlerFunc addresshnd_array[FIFO_PROG_CH_NUM];

enum
{
	QUEUE_VALUE32, QUEUE_ADDRESS
};

typedef struct tagFifoMsgQueueEntry
{
	struct tagFifoMsgQueueEntry* next;
	u16 type, channel;
	union
	{
		word_t value32;
		void* address;
	};
} FifoMsgQueueEntry;

// Ersatz memory allocator

#ifdef ARM9
#define FIFOMSGSTACK_SIZE (4*1024)
#else
#define FIFOMSGSTACK_SIZE 1024
#endif

static byte_t fifoMsgStack[FIFOMSGSTACK_SIZE];
static int fifoMsgStackPos = FIFOMSGSTACK_SIZE;

static FifoMsgQueueEntry* DSAllocFifoMsg()
{
	FifoMsgQueueEntry* ret = NULL;
	int cs = enterCriticalSection();
	int newpos = fifoMsgStackPos - sizeof(FifoMsgQueueEntry);
	if (newpos >= 0)
		ret = (FifoMsgQueueEntry*)(fifoMsgStack + (fifoMsgStackPos = newpos));
	leaveCriticalSection(cs);
	return ret;
}

// User-mode FIFO message queue data structure. This must be done in
// assembly because a STRD instruction is needed to set both fields
// at the same time.

asm("\
.text\n\
	.align 3\n\
	.global __clearFifoMsgQueue\n\
	.type __clearFifoMsgQueue STT_FUNC\n\
__clearFifoMsgQueue:\n\
	mov r0, #0\n\
	mov r1, #0\n"
#ifdef ARM9
"	strd r0, [pc]\n"
#else
"	adr r2, __fifoMsgQueue\n"
"	stm r2, {r0, r1}\n"
#endif
"	bx lr\n\
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

static void DSAddFifoMsgToQueue(FifoMsgQueueEntry* msg)
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

void DSRunFifoQueue()
{
	FifoMsgQueueEntry* ent;

	for(ent = fifoMsgQueue; ent; ent = ent->next)
	{
		int chn = ent->channel - FIFO_PROG_CH;
		void* userdata = userdata_array[chn];
		switch(ent->type)
		{
			case QUEUE_VALUE32:
				if (value32hnd_array[chn])
					value32hnd_array[chn](ent->value32, userdata);
				break;
			case QUEUE_ADDRESS:
				if (addresshnd_array[chn])
					addresshnd_array[chn](ent->address, userdata);
				break;
		}
	}

	__clearFifoMsgQueue();
	fifoMsgStackPos = FIFOMSGSTACK_SIZE;
}

void DSHandleDatamsgs()
{
	int i, length;
	for (i = 0; i < FIFO_PROG_CH_NUM; i ++)
		if (datamsghnd_array[i])
			while ((length = fifoCheckDatamsgLength(FIFO_PROG_CH + i)) != -1)
				datamsghnd_array[i](length, userdata_array[i]);
}

static void StubFifoValue32Handler(word_t value32, void* userdata)
{
	FifoMsgQueueEntry* msg = DSAllocFifoMsg();
	DSAddFifoMsgToQueue(msg);
	msg->type = QUEUE_VALUE32;
	msg->channel = (u16) (word_t) userdata;
	msg->value32 = value32;
}

static void StubFifoAddressHandler(void* address, void* userdata)
{
	FifoMsgQueueEntry* msg = DSAllocFifoMsg();
	DSAddFifoMsgToQueue(msg);
	msg->type = QUEUE_ADDRESS;
	msg->channel = (u16) (word_t) userdata;
	msg->address = address;
}

void DSSetValue32Handler(int channel, bool set)
{
	if (set)
		fifoSetValue32Handler(channel, StubFifoValue32Handler, (void*) channel);
	else
		fifoSetValue32Handler(channel, NULL, NULL);
}

void DSSetAddressHandler(int channel, bool set)
{
	if (set)
		fifoSetAddressHandler(channel, StubFifoAddressHandler, (void*) channel);
	else
		fifoSetAddressHandler(channel, NULL, NULL);
}

#ifdef ARM7
#define DSFifoSetDatamsgHandler coopFifoSetDatamsgHandler
#define DSFifoSetValue32Handler coopFifoSetValue32Handler
#define DSFifoSetAddressHandler coopFifoSetAddressHandler
#define DSIrqSet coopIrqSet
#define DSProcessIRQs coopIrqCheck
#define DSWaitForIRQRaw coopIrqWait
#define DSWaitForNextIRQRaw coopIrqNext
#endif

void DSFifoSetDatamsgHandler(int channel, FifoDatamsgHandlerFunc handler, void* userdata)
{
	int ch = channel - FIFO_PROG_CH;
	userdata_array[ch] = userdata;
	datamsghnd_array[ch] = handler;
	if (handler)
	{
		int l = 0;
		while ((l = fifoCheckDatamsgLength(channel)) != -1)
			handler(l, userdata);
	}
	DSRunFifoQueue();
}

void DSFifoSetValue32Handler(int channel, FifoValue32HandlerFunc handler, void* userdata)
{
	int ch = channel - FIFO_PROG_CH;
	userdata_array[ch] = userdata;
	value32hnd_array[ch] = handler;
	DSSetValue32Handler(channel, !!handler);
	DSRunFifoQueue();
}

void DSFifoSetAddressHandler(int channel, FifoAddressHandlerFunc handler, void* userdata)
{
	int ch = channel - FIFO_PROG_CH;
	userdata_array[ch] = userdata;
	addresshnd_array[ch] = handler;
	DSSetAddressHandler(channel, !!handler);
	DSRunFifoQueue();
}

//-----------------------------------------------------------------------------
// User mode IRQ support
//     Based on libnds code
//-----------------------------------------------------------------------------

#define MAX_IRQS 16
#define GET_IRQ ((VoidFn)0xFFFFFFFF)

static struct IntTable irqTable[MAX_IRQS];

VoidFn DSIrqSet(word_t mask, VoidFn fn)
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

static void DSProcessIRQ(word_t flags)
{
	// Check for FIFO recv IRQ before everything else
	if (flags & IRQ_FIFO_NOT_EMPTY)
	{
		DSRunFifoQueue();
		DSHandleDatamsgs();

		// We don't want user code responding to this IRQ
		flags &= ~IRQ_FIFO_NOT_EMPTY;
	}

#ifdef ARM9
	if (flags & IRQ_VBLANK)
	{
		extern volatile bool inHeadphoneSleep;
		extern bool bKeyUpd, bBgUpd, bOAMUpd, conMode;
		void DSBgUpdate();

		if (!conMode) do
		{
			if (bKeyUpd) scanKeys();

			if (inHeadphoneSleep)
				break;

			if (bBgUpd)
			{
				bBgUpd = false;
				DSBgUpdate();
				bBgUpd = true;
			}

			if (bOAMUpd)
			{
				oamUpdate(&oamMain);
				oamUpdate(&oamSub);
			}
		} while(0);
	}
#endif

	int i; word_t mask; VoidFn fn;
	for (i = 0; i < MAX_IRQS && (mask = irqTable[i].mask); i ++)
		if ((mask & flags) && (fn = irqTable[i].handler))
		{
			fn();
			break;
		}
}

word_t DSProcessIRQs()
{
	word_t totalflags = 0;
	for(;;) // IRQ/FIFO might fire while the handlers are executing
	{
		word_t flags = __ARMSWP(0, &INTR_WAIT_FLAGS);
		if (flags)
			DSProcessIRQ(flags);
		else
			break;
		totalflags |= flags;
	}
	return totalflags;
};

void DSWaitForIRQRaw(word_t mask)
{
	DSProcessIRQs();
	for(;;)
	{
		DSIRQPoll();
		if (DSProcessIRQs() & mask)
			break;
	}
}

word_t DSWaitForNextIRQRaw()
{
	word_t flags = 0;
	for(;;)
	{
		flags = DSProcessIRQs();
		if (flags == 0)
			DSIRQPoll(); // wait for *any* IRQs to happen
		else
			break;
	}
	return flags;
}

#ifdef ARM9
word_t DSFifoGetRetValue32(int ch)
{
	static volatile byte_t counters[FIFO_PROG_CH_NUM];
	volatile byte_t* pCounter = counters + ch - FIFO_PROG_CH;
#define WAIT_COUNTER (*pCounter)

	byte_t queuePos = WAIT_COUNTER++;
	byte_t prevCnt = queuePos;

	// Wait for previous calls to finish
	while (queuePos)
	{
		byte_t curCnt = WAIT_COUNTER;
		if (curCnt < prevCnt)
			// Decrement queue position
			queuePos -= prevCnt - curCnt;
		prevCnt = curCnt;
		DSWaitForIRQ(~0);
	}

	// Wait for the return value
	while (!fifoCheckValue32(ch))
		DSWaitForIRQ(~0);

	// Return
	WAIT_COUNTER --;
	return fifoGetValue32(ch);

#undef WAIT_COUNTER
}

void __real_swiIntrWait(word_t, word_t);
void __wrap_swiIntrWait(word_t mode, word_t mask)
{
	if (isUserMode())
		mode ? DSWaitForIRQRaw(mask) : DSWaitForNextIRQ(mask);
	else
		__real_swiIntrWait(mode, mask);
}

void __real_swiWaitForVBlank();
void __wrap_swiWaitForVBlank()
{
	if (isUserMode())
		DSWaitForVBlankRaw();
	else
		__real_swiWaitForVBlank();
}
#endif

#ifdef ARM7
void coop_swiIntrWaitCompat(word_t how, word_t what)
{
	how ? coopIrqWait(what) : coopIrqNext(what);
}
#endif
