.section .imp.FEOSBASE, "ax", %progbits
.global __imp_atoi
.hidden __imp_atoi
.global atoi
.hidden atoi
atoi:
	ldr r12, [pc]
	bx r12
__imp_atoi:
	.word 0