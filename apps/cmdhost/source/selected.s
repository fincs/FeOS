
@{{BLOCK(selected)

@=======================================================================
@
@	selected, 8x8@4, 
@	+ palette 16 entries, not compressed
@	+ 1 tiles not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2012-12-23, 21:25:07
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.10
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global selectedTiles		@ 32 unsigned chars
	.hidden selectedTiles
selectedTiles:
	.word 0x00111100,0x01111110,0x11111111,0x11111111,0x11111111,0x11111111,0x01111110,0x00111100

	.section .rodata
	.align	2
	.global selectedPal		@ 32 unsigned chars
	.hidden selectedPal
selectedPal:
	.hword 0x7FFE,0x6000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(selected)
