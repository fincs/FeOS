.section .imp.FEOSBASE, "ax", %progbits
.global __imp_malloc
.hidden __imp_malloc
.global malloc
.hidden malloc
malloc:
	ldr r12, [pc]
	bx r12
__imp_malloc:
	.word 0