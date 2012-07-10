.arch armv5te
.section .itcm, "ax", %progbits
.arm
.align 2

.macro writehook name
	.word _\name\()hook
.endm

__BIOS_SWI:
.word 0xFFFF0008

.global __SWIHandler
.type __SWIHandler STT_FUNC
__SWIHandler:
	@ Redirect to the BIOS if the caller is not user mode
	mrs r12, spsr
	tst r12, #0xF
	ldrne pc, __BIOS_SWI
	
	@ Save the caller information
	push {r12, lr} @ spsr and return address
	ldrb r12, [lr, #-2] @ function number
	
	@ Switch to system mode & enable interrupts
	mrs r14, cpsr
	eor r14, r14, #0x8C
	msr cpsr, r14
	
	@ Call the function
	push {lr}
	adr r14, __SVCTable
	ldr r12, [r14, r12, lsl #2]
	blx r12
	pop {lr}
	
	@ Switch to supervisor mode
	mrs r12, cpsr
	bic r12, r12, #0xC
	msr cpsr, r12
	
	@ Restore the caller information then return
	pop {r12, lr}
	msr spsr, r12
	
	movs pc, lr

.align 2
.global __SVCTable
__SVCTable:
	@ Public functions (0x0Z)
	.word 0
	.word LoadModule_ARM7
	.word FreeModule_ARM7
	.word keyboardUpdate
	.word __FeOS_IRQPoll
	.word FeOS_DataCacheFlush
	.word FeOS_DataCacheFlushAll
	.word FeOS_InstrCacheInvalidate
	.word FeOS_InstrCacheInvalidateAll
	.word irqEnable
	.word irqDisable
	.word InitConMode
	.word InitFreeMode
	.word 0 @_SetDatamsgHandler
	.word _SetValue32Handler
	.word _SetAddressHandler

	@ Kernel functions (0x1Z)
	.word FeOS_DebugPrint
	.word FeOS_SetSuspendMode
	.word __sassert
	writehook conwrite
	writehook conerr
	.word __FeOS_SuspendIRQ_t
	.word __FeOS_RestoreIRQ_t
	.word __FeOS_DrainWriteBuffer
	.word __FeOS_WaitForMemAddr
	.word _TimerWrite
	.word _TimerTick
	.space 4*5

	@ FAT hooks (0x2Z ~ 0x3Z)

	writehook fatopen
	writehook fatclose
	writehook fatwrite
	writehook fatread
	writehook fatseek
	writehook fatfstat
	writehook fatstat
	writehook fatlink
	writehook fatunlink
	writehook fatchdir
	writehook fatrename
	writehook fatmkdir
	writehook fatdiropen
	writehook fatdirreset
	writehook fatdirnext
	writehook fatdirclose
	writehook fatstatvfs
	writehook fatftruncate
	writehook fatfsync
	.space 4*13

	@ FIFO functions (0x4Z)
	.word fifoSendAddress
	.word fifoSendValue32
	.word fifoSendDatamsg
	.word fifoCheckAddress
	.word fifoCheckValue32
	.word fifoCheckDatamsg
	.word fifoGetAddress
	.word fifoGetValue32
	.word fifoGetDatamsg
	.word fifoCheckDatamsgLength
	.space 4*6
	
	@ Math functions (0x5Z)
	.word FeOS_div3232
	.word FeOS_mod3232
	.word FeOS_div6432
	.word FeOS_mod6432
	.word FeOS_div6464
	.word FeOS_mod6464
	.word FeOS_sqrt32
	.word FeOS_sqrt64
	.space 4*8

	.space 4*160


.align 2
.global __ResetHandler
.type __ResetHandler STT_FUNC
__ResetHandler:
	@ Huh??
	@ Let the exception handler run
	mrs r0, cpsr
	bic r0, r0, #0xC0
	msr cpsr, r0
	.word 0xF7F000F0

.align 2
__FeOS_IRQPoll:
	@ savedIME = REG_IME, REG_IME = 1
	mov r2, #1
	mov r12, #0x4000000
	ldrb r3, [r12, #0x208]
	strb r2, [r12, #0x208]
	
	@ Wait for IRQ
	mov r2, #0
	mcr 15, 0, r2, c7, c0, 4
	
	@ REG_IME = savedIME
	strb r3, [r12, #0x208]
	
	@ return
	bx lr

.align 2
__FeOS_SuspendIRQ_t:
	mrs r1, cpsr
	and r0, r1, #0x80
	orr r1, #0x80
	msr cpsr, r1
	bx lr

.align 2
__FeOS_RestoreIRQ_t:
	mrs r1, cpsr
	bic r0, #0x80
	orr r1, r0
	msr cpsr, r1
	bx lr

.align 2
__FeOS_DrainWriteBuffer:
	mcr p15, 0, r0, c7, c10, 4
	bx lr

.align 2
__FeOS_WaitForMemAddr:
	bic r3, r0, #0x1F
.Lwait_addr:
	mcr p15, 0, r3, c7, c14, 1 // invalidate line
	ldrb r2, [r0]
	cmp r2, r1
	bne .Lwait_addr
	bx lr

.text
.align 2
.global PrepareUserMode
.type PrepareUserMode STT_FUNC
PrepareUserMode:
	@ Copy ITCM MPU section to GBA ROM section
	mrc p15, 0, r0, c6, c4, 0
	mcr p15, 0, r0, c6, c3, 0

	@ Setup ITCM block section
	ldr r0, =( (0b01011 << 1) | 0x01000000 | 1) @ PAGE_4K
	mcr p15, 0, r0, c6, c4, 0

	@ Set new access settings
	ldr r0, AccessSettings
	mcr p15, 0, r0, c5, c0, 3 @ code
	mcr p15, 0, r0, c5, c0, 2 @ data
	bx  lr

AccessSettings:
	.word 0x33313151
	@.word 0x33311151
	@.word 0x31113151
	@.word 0x32113551
	@.word 0x32313551

AccessSettings2:
	.word 0x33313153
	@.word 0x33311153

.align 2
.global UnblockIORegion
.type UnblockIORegion STT_FUNC
UnblockIORegion:
	ldr r0, AccessSettings2
	mcr p15, 0, r0, c5, c0, 2 @ data
	bx  lr

.align 2
.global BlockIORegion
.type BlockIORegion STT_FUNC
BlockIORegion:
	ldr r0, AccessSettings
	mcr p15, 0, r0, c5, c0, 2 @ data
	bx  lr

.align 2
.global FeOS_IRQPoll
.type FeOS_IRQPoll STT_FUNC
FeOS_IRQPoll:
	mrs r0, cpsr
	tst r0, #0xF
	beq .Lpoll_from_user_mode
	ldr pc, =__FeOS_IRQPoll

.Lpoll_from_user_mode:
	swi 0x040000
	bx lr

.align 2
.global DoTheUserMode
.type DoTheUserMode STT_FUNC
DoTheUserMode:
	@ Switch to user mode
	mrs r0, cpsr
	bic r0, r0, #0xF
	msr cpsr, r0
	bx lr

@ word_t __ARMSWP(word_t value, word_t* addr)
.align 2
.global __ARMSWP
.type __ARMSWP STT_FUNC
__ARMSWP:
	swp r0, r0, [r1]
	bx lr

.align 2
.global __getIRQStack
.type __getIRQStack STT_FUNC
__getIRQStack:
	@ Switch to IRQ mode
	mrs r1, cpsr
	bic r1, r1, #0xD
	msr cpsr, r1

	@ Get the stack pointer
	mov r0, sp

	@ Switch back to System mode
	orr r1, r1, #0xD
	msr cpsr, r1

	@ Return
	bx lr

.align 2
.global __getSWIStack
.type __getSWIStack STT_FUNC
__getSWIStack:
	@ Switch to SWI mode
	mrs r1, cpsr
	bic r1, r1, #0xC
	msr cpsr, r1

	@ Get the stack pointer
	mov r0, sp

	@ Switch back to System mode
	orr r1, r1, #0xC
	msr cpsr, r1

	@ Return
	bx lr

.align 2
.global __setSWIStack
.type __setSWIStack STT_FUNC
__setSWIStack:
	@ Switch to SWI mode
	mrs r1, cpsr
	bic r1, r1, #0xC
	msr cpsr, r1

	@ Set the stack pointer
	mov sp, r0

	@ Switch back to System mode
	orr r1, r1, #0xC
	msr cpsr, r1

	@ Return
	bx lr

.align 2
.global __getMode
.type __getMode STT_FUNC
__getMode:
	mrs r0, cpsr
	and r0, r0, #0x1F
	bx  lr

.macro swiimp name num
.align 2
.global FeOS_swi_\name
.type FeOS_swi_\name STT_FUNC
.thumb_func
FeOS_swi_\name\():
	swi \num
	bx lr
.endm

swiimp LoadModule_ARM7 0x01
swiimp FreeModule_ARM7 0x02
swiimp keyboardUpdate 0x03
swiimp DebugPrint 0x10
swiimp SetSuspendMode 0x11
swiimp assertfail 0x12
swiimp DataCacheFlush 0x05
swiimp DataCacheFlushAll 0x06
swiimp InstrCacheInvalidate 0x07
swiimp InstrCacheInvalidateAll 0x08
swiimp DrainWriteBuffer 0x17
swiimp IrqEnable 0x09
swiimp IrqDisable 0x0A
swiimp ConsoleMode 0x0B
swiimp DirectMode 0x0C
@swiimp SetDatamsgHandler 0x0D
swiimp SetValue32Handler 0x0E
swiimp SetAddressHandler 0x0F

swiimp SuspendIRQ_t 0x15
swiimp RestoreIRQ_t 0x16
swiimp WaitForMemAddr 0x18
swiimp TimerWrite 0x19
swiimp TimerTick 0x1A

swiimp FifoSendAddress 0x40
swiimp FifoSendValue32 0x41
swiimp FifoSendDatamsg 0x42
swiimp FifoCheckAddress 0x43
swiimp FifoCheckValue32 0x44
swiimp FifoCheckDatamsg 0x45
swiimp FifoGetAddress 0x46
swiimp FifoGetValue32 0x47
swiimp FifoGetDatamsg 0x48
swiimp FifoCheckDatamsgLength 0x49

swiimp div3232 0x50
swiimp mod3232 0x51
swiimp div6432 0x52
swiimp mod6432 0x53
swiimp div6464 0x54
swiimp mod6464 0x55
swiimp sqrt32  0x56
swiimp sqrt64  0x57
