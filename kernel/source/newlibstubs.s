@ Stubs for bloaty newlib functions
.text

.align 2
.global _jp2uc
.thumb_func
_jp2uc: @ int _jp2uc(int x) { return x; }
	bx lr

.align 2
.global __part_load_locale
.thumb_func
__part_load_locale:
.global setlocale
.thumb_func
setlocale:
.global _setlocale_r
.thumb_func
_setlocale_r:
.global localeconv
.thumb_func
localeconv:
.global _localeconv_r
.thumb_func
_localeconv_r:
	svc 0x00 @ svcSoftReset, will visibly wreck havoc

.align 2
.global __locale_charset
.thumb_func
__locale_charset:
	ldr r0, =.Lstr_utf8
	bx lr

.align 2
.global __locale_msgcharset
.thumb_func
__locale_msgcharset:
	ldr r0, =.Lstr_ascii
	bx lr

.align 2
.global __locale_mb_cur_max
.thumb_func
__locale_mb_cur_max:
	mov r0, #6
	bx lr

.align 2
.global __locale_cjk_lang
.thumb_func
__locale_cjk_lang:
	mov r0, #0
	bx lr

.align 2
.Lstr_utf8:
	.asciz "UTF-8"

.align 2
.Lstr_ascii:
	.asciz "ASCII"
