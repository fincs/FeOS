.arch armv5te

.section .init
.global __entrypoint
.hidden __entrypoint

.align 2

@ This word-sized space is reserved by the OS.
.word 0

@ r0 - word_t event
@ r1 - word_t prm1
@ r2 - word_t prm2
@ r3 - word_t prm3
__entrypoint:
	push {lr}
	
	@ Jump to the handler
	bl  __FeOSMain
	pop {pc}
