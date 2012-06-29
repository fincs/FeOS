.arch armv5te

.arm
.align 2
.global setjmp, longjmp
.hidden setjmp, longjmp
.type setjmp STT_FUNC
.type longjmp STT_FUNC

setjmp: @ r0 - buffer
	@ Write the registers to the jump buffer
	stmia r0!, {r4-r11, sp, lr}
	
	@ Return 0 to indicate no exception
	mov r0, #0
	bx  lr

longjmp: @ r0 - buffer, r1 - excpt code
	@ Disallow 0 as setjmp return value
	cmp r1, #0
	moveq r1, #1
	
	@ Read the registers from the jump buffer
	ldmia r0!, {r4-r11, sp, lr}
	
	@ Return to the caller of setjmp.
	mov r0, r1
	bx  lr
