.arch armv5te
.text
.align 2

.global FeOS_DebugPrint
.hidden FeOS_DebugPrint
.thumb_func
FeOS_DebugPrint:
	swi 0x10
	bx lr

.global FeOS_WaitForVBlank
.hidden FeOS_WaitForVBlank
.thumb_func
FeOS_WaitForVBlank:
	swi 0x11
	bx lr
