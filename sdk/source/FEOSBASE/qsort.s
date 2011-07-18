.section .imp.FEOSBASE, "ax", %progbits
.global __imp_qsort
.hidden __imp_qsort
.global qsort
.hidden qsort
qsort:
	ldr r12, [pc]
	bx r12
__imp_qsort:
	.word 0