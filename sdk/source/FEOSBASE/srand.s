.section .imp.FEOSBASE, "ax", %progbits
.global __imp_srand
.hidden __imp_srand
.global srand
.hidden srand
srand:
	ldr r12, [pc]
	bx r12
__imp_srand:
	.word 0