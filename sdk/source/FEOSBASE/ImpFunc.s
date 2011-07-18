.section .imp.FEOSBASE, "ax", %progbits
.global __imp_ImpFunc
.hidden __imp_ImpFunc
.global ImpFunc
.hidden ImpFunc
ImpFunc:
	ldr r12, [pc]
	bx r12
__imp_ImpFunc:
	.word 0