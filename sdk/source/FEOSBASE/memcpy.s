.section .imp.FEOSBASE, "ax", %progbits
.global __imp_memcpy
.hidden __imp_memcpy
.global memcpy
.hidden memcpy
memcpy:
	ldr r12, [pc]
	bx r12
__imp_memcpy:
	.word 0