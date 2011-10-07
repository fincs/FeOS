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
	@ Public functions
	.word __FeOS_IRQPoll
	.word LoadModule_ARM7
	.word FreeModule_ARM7
	.word keyboardUpdate
	.word 0
	.word FeOS_DataCacheFlush
	.word FeOS_DataCacheFlushAll
	.word FeOS_InstrCacheInvalidate
	.word FeOS_InstrCacheInvalidateAll
	.word irqEnable
	.word irqDisable
	.space 4*5

	@ Kernel functions
	.word FeOS_DebugPrint
	.word swiWaitForVBlank
	.word FeOS_IsValidName
	writehook conwrite
	writehook conerr
	.space 4*11

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

	@ FIFO functions
	.word fifoSendAddress
	.word fifoSendValue32
	.word fifoSendDatamsg
	.word fifoCheckAddress
	.word fifoCheckValue32
	.word fifoCheckDatamsg
	.word fifoGetAddress
	.word fifoGetValue32
	.word fifoGetDatamsg
	.space 4*7

	.space 4*176

.align 2
.global __ResetHandler
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

.text
.align 2
.global PrepareUserMode
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
.global FeOS_IRQPoll
FeOS_IRQPoll:
	mrs r0, cpsr
	tst r0, #0xF
	beq .Lpoll_from_user_mode
	ldr pc, =__FeOS_IRQPoll

.Lpoll_from_user_mode:
	swi 0x000000
	bx lr

.align 2
.global DoTheUserMode
DoTheUserMode:
	@ Switch to user mode
	mrs r0, cpsr
	bic r0, r0, #0xF
	msr cpsr, r0
	bx lr

@ word_t __ARMSWP(word_t value, word_t* addr)
.align 2
.global __ARMSWP
__ARMSWP:
	swp r0, r0, [r1]
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
swiimp keyboardUpdate 0x03
swiimp DebugPrint 0x10
swiimp DataCacheFlush 0x05
swiimp DataCacheFlushAll 0x06
swiimp InstrCacheInvalidate 0x07
swiimp InstrCacheInvalidateAll 0x08
swiimp IrqEnable 0x09
swiimp IrqDisable 0x0A

swiimp FifoSendAddress 0x40
swiimp FifoSendValue32 0x41
swiimp FifoSendDatamsg 0x42
swiimp FifoCheckAddress 0x43
swiimp FifoCheckValue32 0x44
swiimp FifoCheckDatamsg 0x45
swiimp FifoGetAddress 0x46
swiimp FifoGetValue32 0x47
swiimp FifoGetDatamsg 0x48
