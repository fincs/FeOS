.arch armv5te
.section .itcm, "ax", %progbits
.arm
.align 2

.macro writehook name
	.word _\name\()hook
.endm

.global __SWIHandler, __ResetHandler, __SVCTable, DoTheUserMode, PrepareUserMode, FeOS_WaitForVBlank

__BIOS_SWI:
.word 0xFFFF0008

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
__SVCTable:
	@ Public functions
	.word 0
	.word LoadModule_ARM7
	.word FreeModule_ARM7
	.word 0 @ malloc
	.word 0 @ free
	.word FeOS_DataCacheFlush
	.word FeOS_DataCacheFlushAll
	.word FeOS_InstrCacheInvalidate
	.word FeOS_InstrCacheInvalidateAll
	.space 4*7

	@ Kernel functions
	.word FeOS_DebugPrint
	.word swiWaitForVBlank
	.word FeOS_IsValidName
	writehook conwrite
	writehook conread
	writehook conerr
	.space 4*10

	@ FAT hooks

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
__ResetHandler:
	@ Huh??
	@ Let the exception handler run
	mrs r0, cpsr
	bic r0, r0, #0xC0
	msr cpsr, r0
	.word 0xF7F000F0

.text
.align 2
PrepareUserMode:
	@ Set new access settings
	ldr r0, AccessSettings
	mcr p15, 0, r0, c5, c0, 3 @ code
	mcr p15, 0, r0, c5, c0, 2 @ data
	bx  lr

AccessSettings:
	.word 0x33311151
	@.word 0x31113151
	@.word 0x32113551
	@.word 0x32313551

.align 2
DoTheUserMode:
	@ Switch to user mode
	mrs r0, cpsr
	bic r0, r0, #0xF
	msr cpsr, r0
	bx lr

.align 2
.global __getIRQStack
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
.thumb_func
FeOS_WaitForVBlank:
	swi 0x11
	bx lr

.macro swiimp name num
.align 2
.global FeOS_swi_\name
.thumb_func
FeOS_swi_\name\():
	swi \num
	bx lr
.endm

swiimp LoadModule_ARM7 0x01
swiimp FreeModule_ARM7 0x02
swiimp DebugPrint 0x10
swiimp DataCacheFlush 0x05
swiimp DataCacheFlushAll 0x06
swiimp InstrCacheInvalidate 0x07
swiimp InstrCacheInvalidateAll 0x08
