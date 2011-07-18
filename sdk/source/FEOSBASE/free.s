.section .imp.FEOSBASE, "ax", %progbits
.global __imp_free
.hidden __imp_free
.global free
.hidden free
free:
	ldr r12, [pc]
	bx r12
__imp_free:
	.word 0