.arch armv5te
.text
.arm
.align 2

.macro makehook name, swinum, isfat=1
.global _\name\()hook, _\name\()hook_knl, _\name\()hook_addr

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

makehook conwrite, 0x13, 0
makehook conerr, 0x14, 0

@ FAT hooks

makehook fatopen, 0x20
makehook fatclose, 0x21
makehook fatwrite, 0x22
makehook fatread, 0x23
makehook fatseek, 0x24
makehook fatfstat, 0x25
makehook fatstat, 0x26
makehook fatlink, 0x27
makehook fatunlink, 0x28
makehook fatchdir, 0x29
makehook fatrename, 0x2A
makehook fatmkdir, 0x2B
makehook fatdiropen, 0x2C
makehook fatdirreset, 0x2D
makehook fatdirnext, 0x2E
makehook fatdirclose, 0x2F
makehook fatstatvfs, 0x30
makehook fatftruncate, 0x31
makehook fatfsync, 0x32

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
