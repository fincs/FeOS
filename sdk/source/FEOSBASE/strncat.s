.section .imp.FEOSBASE, "ax", %progbits
.global __imp_strncat
.hidden __imp_strncat
.global strncat
.hidden strncat
strncat:
	ldr r12, [pc]
	bx r12
__imp_strncat:
	.word 0