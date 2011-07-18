.section .imp.FEOSBASE, "ax", %progbits
.global __imp_memmove
.hidden __imp_memmove
.global memmove
.hidden memmove
memmove:
	ldr r12, [pc]
	bx r12
__imp_memmove:
	.word 0