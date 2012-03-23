.arch armv5te
.text
.global __newThread
.hidden __newThread
.type __newThread STT_FUNC

.align 2
__newThread: @ r0 - param, r1 - entrypoint, r2 - stack pointer
	mov sp, r2
	ldr lr, =FeOS_ExitThread
	bx r1
