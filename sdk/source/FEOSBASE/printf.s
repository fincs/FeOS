.section .imp.FEOSBASE, "ax", %progbits
.global __imp_printf
.hidden __imp_printf
.global printf
.hidden printf
printf:
	ldr r12, [pc]
	bx r12
__imp_printf:
	.word 0