#pragma once

// 3D hardware supplementary definitions

#define VRAM_CR     (*(vu32*)0x04000240)
#define VRAM_A_CR   (*(vu8*)0x04000240)
#define VRAM_B_CR   (*(vu8*)0x04000241)
#define VRAM_C_CR   (*(vu8*)0x04000242)
#define VRAM_D_CR   (*(vu8*)0x04000243)
#define VRAM_EFG_CR	(*(vu32*)0x04000244)
#define VRAM_E_CR   (*(vu8*)0x04000244)
#define VRAM_F_CR   (*(vu8*)0x04000245)
#define VRAM_G_CR   (*(vu8*)0x04000246)
#define WRAM_CR     (*(vu8*)0x04000247)
#define VRAM_H_CR   (*(vu8*)0x04000248)
#define VRAM_I_CR   (*(vu8*)0x04000249)

#define GFX_CONTROL           (*(vu16*) 0x04000060)
#define GFX_FIFO              (*(vu32*) 0x04000400)
#define GFX_STATUS            (*(vu32*) 0x04000600)
#define GFX_COLOR             (*(vu32*) 0x04000480)
#define GFX_VERTEX10          (*(vu32*) 0x04000490)
#define GFX_VERTEX_XY         (*(vu32*) 0x04000494)
#define GFX_VERTEX_XZ         (*(vu32*) 0x04000498)
#define GFX_VERTEX_YZ         (*(vu32*) 0x0400049C)
#define GFX_VERTEX_DIFF       (*(vu32*) 0x040004A0)
#define GFX_VERTEX16          (*(vu32*) 0x0400048C)
#define GFX_TEX_COORD         (*(vu32*) 0x04000488)
#define GFX_TEX_FORMAT        (*(vu32*) 0x040004A8)
#define GFX_PAL_FORMAT        (*(vu32*) 0x040004AC)
#define GFX_CLEAR_COLOR       (*(vu32*) 0x04000350)
#define GFX_CLEAR_DEPTH       (*(vu16*) 0x04000354)
#define GFX_LIGHT_VECTOR      (*(vu32*) 0x040004C8)
#define GFX_LIGHT_COLOR       (*(vu32*) 0x040004CC)
#define GFX_NORMAL            (*(vu32*) 0x04000484)
#define GFX_DIFFUSE_AMBIENT   (*(vu32*) 0x040004C0)
#define GFX_SPECULAR_EMISSION (*(vu32*) 0x040004C4)
#define GFX_SHININESS         (*(vu32*) 0x040004D0)
#define GFX_POLY_FORMAT       (*(vu32*) 0x040004A4)
#define GFX_ALPHA_TEST        (*(vu16*) 0x04000340)
#define GFX_BEGIN             (*(vu32*) 0x04000500)
#define GFX_END               (*(vu32*) 0x04000504)
#define GFX_FLUSH             (*(vu32*) 0x04000540)
#define GFX_VIEWPORT          (*(vu32*) 0x04000580)
#define GFX_TOON_TABLE        ((vu16*)  0x04000380)
#define GFX_EDGE_TABLE        ((vu16*)  0x04000330)
#define GFX_FOG_COLOR         (*(vu32*) 0x04000358)
#define GFX_FOG_OFFSET        (*(vu32*) 0x0400035C)
#define GFX_FOG_TABLE         ((vu8*)   0x04000360)
#define GFX_BOX_TEST          (*(vs32*) 0x040005C0)
#define GFX_POS_TEST          (*(vu32*) 0x040005C4)
#define GFX_POS_RESULT        ((vs32*)  0x04000620)
#define GFX_VEC_TEST          (*(vu32*) 0x040005C8)
#define GFX_VEC_RESULT        ((vs16*)  0x04000630)
#define GFX_BUSY              (GFX_STATUS & BIT(27))
#define GFX_VERTEX_RAM_USAGE  (*(vu16*)  0x04000606)
#define GFX_POLYGON_RAM_USAGE (*(vu16*)  0x04000604)
#define GFX_CUTOFF_DEPTH      (*(vu16*)  0x04000610)

#define MATRIX_CONTROL        (*(vu32*) 0x04000440)
#define MATRIX_PUSH           (*(vu32*) 0x04000444)
#define MATRIX_POP            (*(vu32*) 0x04000448)
#define MATRIX_SCALE          (*(vs32*) 0x0400046C)
#define MATRIX_TRANSLATE      (*(vs32*) 0x04000470)
#define MATRIX_RESTORE        (*(vu32*) 0x04000450)
#define MATRIX_STORE          (*(vu32*) 0x0400044C)
#define MATRIX_IDENTITY       (*(vu32*) 0x04000454)
#define MATRIX_LOAD4x4        (*(vs32*) 0x04000458)
#define MATRIX_LOAD4x3        (*(vs32*) 0x0400045C)
#define MATRIX_MULT4x4        (*(vs32*) 0x04000460)
#define MATRIX_MULT4x3        (*(vs32*) 0x04000464)
#define MATRIX_MULT3x3        (*(vs32*) 0x04000468)

#define MATRIX_READ_CLIP      ((vs32*) (0x04000640))
#define MATRIX_READ_VECTOR    ((vs32*) (0x04000680))
#define POINT_RESULT          ((vs32*) (0x04000620))
#define VECTOR_RESULT         ((vu16*) (0x04000630))


#define DMA0_SRC       (*(vu32*)0x040000B0)
#define DMA0_DEST      (*(vu32*)0x040000B4)
#define DMA0_CR        (*(vu32*)0x040000B8)
#define DMA1_SRC       (*(vu32*)0x040000BC)
#define DMA1_DEST      (*(vu32*)0x040000C0)
#define DMA1_CR        (*(vu32*)0x040000C4)
#define DMA2_SRC       (*(vu32*)0x040000C8)
#define DMA2_DEST      (*(vu32*)0x040000CC)
#define DMA2_CR        (*(vu32*)0x040000D0)
#define DMA3_SRC       (*(vu32*)0x040000D4)
#define DMA3_DEST      (*(vu32*)0x040000D8)
#define DMA3_CR        (*(vu32*)0x040000DC)

#define DMA_SRC(n)     (*(vu32*)(0x040000B0+(n*12)))
#define DMA_DEST(n)    (*(vu32*)(0x040000B4+(n*12)))
#define DMA_CR(n)      (*(vu32*)(0x040000B8+(n*12)))
#define DMA_FILL(n)    (*(vu32*)(0x040000E0+(n*4)))

#define DMA_ENABLE      BIT(31)
#define DMA_BUSY        BIT(31)
#define DMA_IRQ_REQ     BIT(30)

#define DMA_START_NOW   0
#define DMA_START_CARD  (5<<27)

#define DMA_START_HBL   BIT(28)
#define DMA_START_VBL   BIT(27)
#define DMA_START_FIFO  (7<<27)
#define DMA_DISP_FIFO   (4<<27)

#define DMA_16_BIT      0
#define DMA_32_BIT      BIT(26)

#define DMA_REPEAT      BIT(25)

#define DMA_SRC_INC     (0)
#define DMA_SRC_DEC     BIT(23)
#define DMA_SRC_FIX     BIT(24)

#define DMA_DST_INC     (0)
#define DMA_DST_DEC     BIT(21)
#define DMA_DST_FIX     BIT(22)
#define DMA_DST_RESET   (3<<21)

#define DMA_COPY_WORDS     (DMA_ENABLE | DMA_32_BIT | DMA_START_NOW)
#define DMA_COPY_HALFWORDS (DMA_ENABLE | DMA_16_BIT | DMA_START_NOW)
#define DMA_FIFO           (DMA_ENABLE | DMA_32_BIT  | DMA_DST_FIX | DMA_START_FIFO)

// Supplementary types & misc

typedef byte_t uint8;
typedef char_t int8;
typedef hword_t uint16;
typedef short_t int16;
typedef word_t uint32;
typedef long_t int32;
typedef unsigned long long uint64;
typedef long long int64;

#define COPY_MODE_COPY  0
#define COPY_MODE_FILL  BIT(24)
#define COPY_MODE_HWORD 0
#define COPY_MODE_WORD  BIT(26)

static inline void swiCopy(const void* source, void* dest, word_t flags)
{
	word_t i;
	word_t size = flags & ~0xFFE00000;

	if (flags & COPY_MODE_FILL)
	{
		if (flags & COPY_MODE_WORD)
		{
			word_t* src = (word_t*) source;
			word_t* dst = (word_t*) dest;

			for(i = 0; i < size; i ++)
				*dst++ = *src;
		}else
		{
			hword_t* src = (hword_t*) source;
			hword_t* dst = (hword_t*) dest;

			for(i = 0; i < size; i ++)
				*dst++ = *src;
		}
	} else
	{
		if (flags & COPY_MODE_WORD)
		{
			word_t* src = (word_t*) source;
			word_t* dst = (word_t*) dest;

			for(i = 0; i < size; i ++)
				*dst++ = *src++;
		}else
		{
			hword_t* src = (hword_t*) source;
			hword_t* dst = (hword_t*) dest;
			
			for(i = 0; i < size; i ++)
				*dst++ = *src++;
		}
	}
}
