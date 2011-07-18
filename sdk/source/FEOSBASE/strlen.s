.section .imp.FEOSBASE, "ax", %progbits
.global __imp_strlen
.hidden __imp_strlen
.global strlen
.hidden strlen
strlen:
	ldr r12, [pc]
	bx r12
__imp_strlen:
	.word 0