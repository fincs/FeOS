.section .imp.FEOSBASE, "ax", %progbits
.global __imp_strncpy
.hidden __imp_strncpy
.global strncpy
.hidden strncpy
strncpy:
	ldr r12, [pc]
	bx r12
__imp_strncpy:
	.word 0