.section .imp.FEOSBASE, "ax", %progbits
.global __imp_abs
.hidden __imp_abs
.global abs
.hidden abs
abs:
	ldr r12, [pc]
	bx r12
__imp_abs:
	.word 0