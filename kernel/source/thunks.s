.arch armv5te
.text
.arm
.align 2

.macro makehook name, swinum, isfat=1
.global _\name\()hook, _\name\()hook_knl, _\name\()hook_addr
.type _\name\()hook STT_FUNC
.type _\name\()hook_knl STT_FUNC

_\name\()hook:
	mrs r12, cpsr
	tst r12, #0xF
	bne _\name\()hook_knl
	swi \swinum\()0000
	bx lr

_\name\()hook_knl:
	ldr r12, _\name\()hook_addr
	.if \isfat
	b _FAT_jumpAddr
	.else
	bx r12
	.endif

_\name\()hook_addr:
	.word 0
.endm

@ FAT hooks

makehook fatopen, 0x10
makehook fatclose, 0x11
makehook fatwrite, 0x12
makehook fatread, 0x13
makehook fatseek, 0x14
makehook fatfstat, 0x15
makehook fatstat, 0x16
makehook fatlink, 0x17
makehook fatunlink, 0x18
makehook fatchdir, 0x19
makehook fatrename, 0x1A
makehook fatmkdir, 0x1B
makehook fatdiropen, 0x1C
makehook fatdirreset, 0x1D
makehook fatdirnext, 0x1E
makehook fatdirclose, 0x1F
makehook fatstatvfs, 0x20
makehook fatftruncate, 0x21
makehook fatfsync, 0x22

.align 2
_FAT_jumpAddr:
	push {lr}
	mov lr, #1
	str lr, __inFAT
	blx r12
	mov lr, #0
	str lr, __inFAT
	pop {pc}

.global __inFAT
__inFAT:
	.word 0
