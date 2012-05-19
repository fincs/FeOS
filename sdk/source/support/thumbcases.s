@ BLATANTLY STOLEN FROM LIBGCC

.arch armv5te
.text

.macro BEGIN x
.global __gnu_thumb1_case_\x
.hidden __gnu_thumb1_case_\x
.align 2
.thumb_func
__gnu_thumb1_case_\x:
.endm

BEGIN sqi
	push  {r1}
	mov   r1, lr
	lsr   r1, r1, #1
	lsl   r1, r1, #1
	ldrsb r1, [r1, r0]
	lsl   r1, r1, #1
	add   lr, lr, r1
	pop   {r1}
	bx    lr

BEGIN uqi
	push {r1}
	mov  r1, lr
	lsr  r1, r1, #1
	lsl  r1, r1, #1
	ldrb r1, [r1, r0]
	lsl  r1, r1, #1
	add  lr, lr, r1
	pop  {r1}
	bx   lr

BEGIN shi
	push  {r0, r1}
	mov   r1, lr
	lsr   r1, r1, #1
	lsl   r0, r0, #1
	lsl   r1, r1, #1
	ldrsh r1, [r1, r0]
	lsl   r1, r1, #1
	add   lr, lr, r1
	pop   {r0, r1}
	bx    lr

BEGIN uhi
	push {r0, r1}
	mov  r1, lr
	lsr  r1, r1, #1
	lsl  r0, r0, #1
	lsl  r1, r1, #1
	ldrh r1, [r1, r0]
	lsl  r1, r1, #1
	add  lr, lr, r1
	pop  {r0, r1}
	bx   lr

BEGIN si
	push {r0, r1}
	mov  r1, lr
	add  r1, r1, #2	@ Align to word.
	lsr  r1, r1, #2
	lsl  r0, r0, #2
	lsl  r1, r1, #2
	ldr  r0, [r1, r0]
	add  r0, r0, r1
	mov  lr, r0
	pop  {r0, r1}
	bx   lr
