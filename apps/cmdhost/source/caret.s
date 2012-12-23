
@{{BLOCK(caret)

@=======================================================================
@
@	caret, 8x8@4, 
@	+ palette 16 entries, not compressed
@	+ 1 tiles not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2012-08-18, 18:07:36
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.10
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global caretTiles		@ 32 unsigned chars
	.hidden caretTiles
caretTiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x11111111

	.section .rodata
	.align	2
	.global caretPal		@ 32 unsigned chars
	.hidden caretPal
caretPal:
	.hword 0x0000,0x7FFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(caret)
