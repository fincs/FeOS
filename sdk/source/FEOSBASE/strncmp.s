.section .imp.FEOSBASE, "ax", %progbits
.global __imp_strncmp
.hidden __imp_strncmp
.global strncmp
.hidden strncmp
strncmp:
	ldr r12, [pc]
	bx r12
__imp_strncmp:
	.word 0