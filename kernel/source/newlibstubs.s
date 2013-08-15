@ Stubs for bloaty newlib functions
.text

.align 2
.global __part_load_locale
.type __part_load_locale, %function
__part_load_locale:
	.word 0xE7F000F0 @ undefined instruction

.align 2
.global _jp2uc
.thumb_func
_jp2uc: @ int _jp2uc(int x) { return x; }
	bx lr

.align 2
.global __locale_charset
.thumb_func
__locale_charset:
	adr r0, .Lstr_utf8
	bx lr

.align 2
.global __locale_mb_cur_max
.thumb_func
__locale_mb_cur_max:
	mov r0, #6
	bx lr

.Lstr_utf8:
	.asciz "UTF-8"
