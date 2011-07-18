.section .imp.FEOSBASE, "ax", %progbits
.global __imp_strcmp
.hidden __imp_strcmp
.global strcmp
.hidden strcmp
strcmp:
	ldr r12, [pc]
	bx r12
__imp_strcmp:
	.word 0