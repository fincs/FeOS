.section .imp.FEOSBASE, "ax", %progbits
.global __imp_memcmp
.hidden __imp_memcmp
.global memcmp
.hidden memcmp
memcmp:
	ldr r12, [pc]
	bx r12
__imp_memcmp:
	.word 0