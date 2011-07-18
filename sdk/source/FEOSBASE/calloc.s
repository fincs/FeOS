.section .imp.FEOSBASE, "ax", %progbits
.global __imp_calloc
.hidden __imp_calloc
.global calloc
.hidden calloc
calloc:
	ldr r12, [pc]
	bx r12
__imp_calloc:
	.word 0