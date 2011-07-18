.section .imp.FEOSBASE, "ax", %progbits
.global __imp_memset
.hidden __imp_memset
.global memset
.hidden memset
memset:
	ldr r12, [pc]
	bx r12
__imp_memset:
	.word 0