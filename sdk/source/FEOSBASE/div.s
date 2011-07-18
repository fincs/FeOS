.section .imp.FEOSBASE, "ax", %progbits
.global __imp_div
.hidden __imp_div
.global div
.hidden div
div:
	ldr r12, [pc]
	bx r12
__imp_div:
	.word 0