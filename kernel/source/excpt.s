.arch armv5te
.section .itcm, "ax", %progbits
.arm
.align 2

__UNDHandler:
	@ Redirect to the BIOS if the caller is not user mode
	push {r12}
	mrs r12, spsr
	tst r12, #0xF
	ldreq pc, =.Hnd_Undefined
	pop {r12}
	ldr pc, =0xFFFF0004

__PABTHandler:
	@ Redirect to the BIOS if the caller is not user mode
	push {r12}
	mrs r12, spsr
	tst r12, #0xF
	ldreq pc, =.Hnd_PrefetchAbort
	pop {r12}
	ldr pc, =0xFFFF000C

__DABTHandler:
	@ Redirect to the BIOS if the caller is not user mode
	push {r12}
	mrs r12, spsr
	tst r12, #0xF
	ldreq pc, =.Hnd_DataAbort
	pop {r12}
	ldr pc, =0xFFFF0010

.global __SetExcptHandler
.type __SetExcptHandler, %function

__SetExcptHandler:
	ldr r1, =__ExcptHandler
	ldr r3, =SystemVectors
	str r0, [r1]
	cmp r0, #0
	beq .LdisableExcpt
	adr r1, __UNDHandler
	str r1, [r3, #0x4]
	adr r1, __PABTHandler
	str r1, [r3, #0xC]
	adr r1, __DABTHandler
	str r1, [r3, #0x10]
	bx lr

.LdisableExcpt:
	ldr r1, =0xFFFF0004
	str r1, [r3, #0x4]
	add r1, #0xC-0x4
	str r1, [r3, #0xC]
	add r1, #0x10-0xC
	str r1, [r3, #0x10]
	bx lr

.pool

@@------------

.text
.arm
.align 2

.Ldbgregs:
	.space 17*4

.Hnd_Undefined:
	@ Save registers
	tst r12, #(1<<5) @ test for T bit
	str r12, .Ldbgregs + 16*4 @ CPSR = SPSR_und
	adr r12, .Ldbgregs
	stm r12, {r0-r11} @ r0 through r11
	subeq lr, #4 @ Adjust saved PC (ARM)
	subne lr, #2 @ Adjust saved PC (THUMB)
	mov r6, #0 @ Set type of exception to UNDEFINED
	b .Hnd_Common

.Hnd_PrefetchAbort:
	@ Save registers
	tst r12, #(1<<5) @ test for T bit
	str r12, .Ldbgregs + 16*4 @ CPSR = SPSR_und
	adr r12, .Ldbgregs
	stm r12, {r0-r11} @ r0 through r11
	sub lr, #4 @ Adjust saved PC
	mov r6, #1 @ Set type of exception to PREFETCH ABORT
	b .Hnd_Common

.Hnd_DataAbort:
	@ Save registers
	tst r12, #(1<<5) @ test for T bit
	str r12, .Ldbgregs + 16*4 @ CPSR = SPSR_und
	adr r12, .Ldbgregs
	stm r12, {r0-r11} @ r0 through r11
	sub lr, #8 @ Adjust saved PC
	mov r6, #2 @ Set type of exception to DATA ABORT
	@b .Hnd_Common

.Hnd_Common:
	pop {r0}
	mov r5, r12
	str r0, [r5, #12*4] @ r12
	orrne lr, #1 @ if T bit was set reflect it in the saved PC
	str lr, [r5, #15*4] @ store PC

	@ Switch to system mode (!)
	mrs r0, cpsr
	orr r0, #0xF
	msr cpsr, r0
	
	str sp, [r5, #13*4] @ store user SP
	str lr, [r5, #14*4] @ store user LR

	@ Get the excpt handler fp
	ldr r4, =__ExcptHandler
	ldr r4, [r4]

	@ Disable exception handling
	mov r0, #0
	bl __SEH_Far

	@ Configure a temp stack
	ldr sp, =.Lexcptstackend

	@ Switch to user mode, re-enable IRQ and call the excpt handler
	mrs r3, cpsr
	eor r3, #0x8F
	msr cpsr, r3
	mov r0, r5
	mov r1, r6
	blx r4

	@ Re-enable exception handling if result is zero
	cmp r0, #0
	bne .LnoRestore
	mov r0, r4
	bl KeSetExcptHandler

.LnoRestore:
	@ Resume execution
	ldr r12, [r5, #16*4] @ load saved CPSR
	msr cpsr_f, r12 @ Restore control flags
	ldm r5, {r0-r15} @ Resume execution

__SEH_Far:
	ldr pc, [pc, #-4]
	.word __SetExcptHandler

__ExcptHandler:
	.word 0

.pool

.bss
.align 2
.Lexcptstack:
	.space 4096
.Lexcptstackend:
