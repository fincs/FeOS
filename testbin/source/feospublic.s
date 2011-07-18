.arch armv5te
.text
.align 2

.global FeOS_Malloc
.hidden FeOS_Malloc
.thumb_func
FeOS_Malloc:
	swi 0x03
	bx lr

.global FeOS_Free
.hidden FeOS_Free
.thumb_func
FeOS_Free:
	swi 0x04
	bx lr
