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
.type __SWIHandler, %function
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
	.word 0
	.word 0
	.word 0
	.word _DSIRQPoll
	.word DC_FlushRange
	.word DC_FlushAll
	.word IC_InvalidateRange
	.word IC_InvalidateAll
	.word 0
	.word 0
	.word 0
	.word 0
	.word __SetExcptHandler
	.word 0
	.word 0

	@ Kernel functions (0x1Z)
	.word 0
	.word 0
#ifdef DEBUG
	.word __sassert
#else
	.word 0
#endif
	writehook conwrite
	writehook conerr
	.word 0
	.word 0
	.word _DC_DrainWriteBuffer
	.word _DSWaitForMemAddr
	.word 0
	.word 0
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

	.space 4*192


.align 2
.global __ResetHandler
.type __ResetHandler, %function
__ResetHandler:
	@ Huh??
	@ Let the exception handler run
	mrs r0, cpsr
	bic r0, r0, #0xC0
	msr cpsr, r0
	.word 0xE7F000F0

.align 2
_DSIRQPoll:
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
_DC_DrainWriteBuffer:
	mcr p15, 0, r0, c7, c10, 4
	bx lr

.align 2
_DSWaitForMemAddr:
	bic r3, r0, #0x1F
.Lwait_addr:
	mcr p15, 0, r3, c7, c14, 1 // invalidate line
	ldrb r2, [r0]
	cmp r2, r1
	bne .Lwait_addr
	bx lr

.text
.align 2
.global KeInitUserMode
.type KeInitUserMode, %function
KeInitUserMode:
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
	.word 0x33313153

.align 2
.global DSIRQPoll
.type DSIRQPoll, %function
DSIRQPoll:
	mrs r0, cpsr
	tst r0, #0xF
	beq .Lpoll_from_user_mode
	ldr pc, =_DSIRQPoll

.Lpoll_from_user_mode:
	swi 0x040000
	bx lr

.align 2
.global KeEnterUserMode
.type KeEnterUserMode, %function
KeEnterUserMode:
	@ Switch to user mode
	mrs r0, cpsr
	bic r0, r0, #0xF
	msr cpsr, r0
	bx lr

@ word_t __ARMSWP(word_t value, word_t* addr)
.align 2
.global __ARMSWP
.type __ARMSWP, %function
__ARMSWP:
	swp r0, r0, [r1]
	bx lr

.align 2
.global __getIRQStack
.type __getIRQStack, %function
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
.type __getSWIStack, %function
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
.type __setSWIStack, %function
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
.type __getMode, %function
__getMode:
	mrs r0, cpsr
	and r0, r0, #0x1F
	bx  lr

.align 2
.global __isEmulator
.type __isEmulator, %function
__isEmulator: @ Only to be called once!
	ldr r0, .Lmov_r0_1
	mov r1, pc
	str r0, [r1]
	mov r0, #0
	bx lr

.Lmov_r0_1:
	mov r0, #1

.align 2
.global __enterThread
.type __enterThread, %function
__enterThread: @ r0 - param, r1 - entrypoint, r2 - stack pointer
	mov sp, r2
	ldr lr, =ThrExit
	bx r1

.macro swiimp name num
.align 2
.global \name
.type \name, %function
.thumb_func
\name\():
	swi \num
	bx lr
.endm

#ifdef DEBUG
swiimp __assert2 0x12
#endif
swiimp KeDataCacheFlush 0x05
swiimp KeDataCacheFlushAll 0x06
swiimp KeInstrCacheInvalidate 0x07
swiimp KeInstrCacheInvalidateAll 0x08
swiimp DC_DrainWriteBuffer 0x17
swiimp KeSetExcptHandler 0x0D
swiimp KeWaitForMemAddr 0x18
