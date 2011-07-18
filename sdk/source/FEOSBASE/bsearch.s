.section .imp.FEOSBASE, "ax", %progbits
.global __imp_bsearch
.hidden __imp_bsearch
.global bsearch
.hidden bsearch
bsearch:
	ldr r12, [pc]
	bx r12
__imp_bsearch:
	.word 0