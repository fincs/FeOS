.section .imp.FEOSBASE, "ax", %progbits
.global __imp_FeOS_FindSymbol
.hidden __imp_FeOS_FindSymbol
.global FeOS_FindSymbol
.hidden FeOS_FindSymbol
FeOS_FindSymbol:
	ldr r12, [pc]
	bx r12
__imp_FeOS_FindSymbol:
	.word 0