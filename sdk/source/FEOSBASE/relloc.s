.section .imp.FEOSBASE, "ax", %progbits
.global __imp_relloc
.hidden __imp_relloc
.global relloc
.hidden relloc
relloc:
	ldr r12, [pc]
	bx r12
__imp_relloc:
	.word 0