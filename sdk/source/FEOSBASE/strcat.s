.section .imp.FEOSBASE, "ax", %progbits
.global __imp_strcat
.hidden __imp_strcat
.global strcat
.hidden strcat
strcat:
	ldr r12, [pc]
	bx r12
__imp_strcat:
	.word 0