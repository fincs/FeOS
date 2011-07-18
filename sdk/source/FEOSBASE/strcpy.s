.section .imp.FEOSBASE, "ax", %progbits
.global __imp_strcpy
.hidden __imp_strcpy
.global strcpy
.hidden strcpy
strcpy:
	ldr r12, [pc]
	bx r12
__imp_strcpy:
	.word 0