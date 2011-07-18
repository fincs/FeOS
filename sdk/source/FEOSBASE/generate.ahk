;
; File encoding:  UTF-8
; Platform:  Windows XP/Vista/7
; Author:    A.N.Other <myemail@nowhere.com>
;
; Script description:
;	Template script
;

#NoEnv
SendMode Input
SetWorkingDir, %A_ScriptDir%

;list = ImpFunc, FeOS_FindSymbol, strcmp, strcpy, strcat, strlen, sprintf, memcpy, memset, memcmp, memmove
list =
(LTrim Join,
ImpFunc
FeOS_FindSymbol
atoi
rand
srand
malloc
calloc
free
relloc
bsearch
qsort
abs
div
strlen
strcpy
strcat
strcmp
strlen
strncpy
strncat
strncmp
memcpy
memmove
memset
memcmp
printf
sprintf
)

Loop, Parse, list, `,, %A_Space%%A_Tab%
{
	FileDelete, %A_LoopField%.s
	text =
(
.section .imp.FEOSBASE, "ax", `%progbits
.global __imp_%A_LoopField%
.hidden __imp_%A_LoopField%
.global %A_LoopField%
.hidden %A_LoopField%
%A_LoopField%:
	ldr r12, [pc]
	bx r12
__imp_%A_LoopField%:
	.word 0
)
	FileAppend, % text, %A_LoopField%.s
}
