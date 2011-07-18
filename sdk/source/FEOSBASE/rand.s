.section .imp.FEOSBASE, "ax", %progbits
.global __imp_rand
.hidden __imp_rand
.global rand
.hidden rand
rand:
	ldr r12, [pc]
	bx r12
__imp_rand:
	.word 0