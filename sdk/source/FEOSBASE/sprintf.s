.section .imp.FEOSBASE, "ax", %progbits
.global __imp_sprintf
.hidden __imp_sprintf
.global sprintf
.hidden sprintf
sprintf:
	ldr r12, [pc]
	bx r12
__imp_sprintf:
	.word 0