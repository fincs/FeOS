@ libgcc code defines - extracted from lib1funcs.S

/* Copyright 1995, 1996, 1998, 1999, 2000, 2003, 2004, 2005, 2007, 2008,
   2009, 2010 Free Software Foundation, Inc.

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

/* An executable stack is *not* required for these functions.  */
#if defined(__ELF__) && defined(__linux__)
.section .note.GNU-stack,"",%progbits
.previous
#endif  /* __ELF__ and __linux__ */

#ifdef __ARM_EABI__
/* Some attributes that are common to all routines in this file.  */
	/* Tag_ABI_align_needed: This code does not require 8-byte
	   alignment from the caller.  */
	/* .eabi_attribute 24, 0  -- default setting.  */
	/* Tag_ABI_align_preserved: This code preserves 8-byte
	   alignment in any callee.  */
	.eabi_attribute 25, 1
#endif /* __ARM_EABI__ */
/* ------------------------------------------------------------------------ */

/* We need to know what prefix to add to function names.  */

#ifndef __USER_LABEL_PREFIX__
#error  __USER_LABEL_PREFIX__ not defined
#endif

/* ANSI concatenation macros.  */

#define CONCAT1(a, b) CONCAT2(a, b)
#define CONCAT2(a, b) a ## b

/* Use the right prefix for global labels.  */

#define SYM(x) CONCAT1 (__USER_LABEL_PREFIX__, x)

#ifdef __ELF__
//#ifdef __thumb__
//#define __PLT__  /* Not supported in Thumb assembler (for now).  */
//#elif defined __vxworks && !defined __PIC__
#define __PLT__ /* Not supported by the kernel loader.  */
//#else
//#define __PLT__ (PLT)
//#endif
#define TYPE(x) .type SYM(x),function
#define SIZE(x) .size SYM(x), . - SYM(x)
#define LSYM(x) .x
#else
#define __PLT__
#define TYPE(x)
#define SIZE(x)
#define LSYM(x) x
#endif

/* Function end macros.  Variants for interworking.  */

#if defined(__ARM_ARCH_2__)
# define __ARM_ARCH__ 2
#endif

#if defined(__ARM_ARCH_3__)
# define __ARM_ARCH__ 3
#endif

#if defined(__ARM_ARCH_3M__) || defined(__ARM_ARCH_4__) \
	|| defined(__ARM_ARCH_4T__)
/* We use __ARM_ARCH__ set to 4 here, but in reality it's any processor with
   long multiply instructions.  That includes v3M.  */
# define __ARM_ARCH__ 4
#endif
	
#if defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5T__) \
	|| defined(__ARM_ARCH_5E__) || defined(__ARM_ARCH_5TE__) \
	|| defined(__ARM_ARCH_5TEJ__)
# define __ARM_ARCH__ 5
#endif

#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) \
	|| defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) \
	|| defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__) \
	|| defined(__ARM_ARCH_6M__)
# define __ARM_ARCH__ 6
#endif

#if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) \
	|| defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) \
	|| defined(__ARM_ARCH_7EM__)
# define __ARM_ARCH__ 7
#endif

#ifndef __ARM_ARCH__
#error Unable to determine architecture.
#endif

/* There are times when we might prefer Thumb1 code even if ARM code is
   permitted, for example, the code might be smaller, or there might be
   interworking problems with switching to ARM state if interworking is
   disabled.  */
#if (defined(__thumb__)			\
     && !defined(__thumb2__)		\
     && (!defined(__THUMB_INTERWORK__)	\
	 || defined (__OPTIMIZE_SIZE__)	\
	 || defined(__ARM_ARCH_6M__)))
# define __prefer_thumb__
#endif

/* How to return from a function call depends on the architecture variant.  */

#if (__ARM_ARCH__ > 4) || defined(__ARM_ARCH_4T__)

# define RET		bx	lr
# define RETc(x)	bx##x	lr

/* Special precautions for interworking on armv4t.  */
# if (__ARM_ARCH__ == 4)

/* Always use bx, not ldr pc.  */
#  if (defined(__thumb__) || defined(__THUMB_INTERWORK__))
#    define __INTERWORKING__
#   endif /* __THUMB__ || __THUMB_INTERWORK__ */

/* Include thumb stub before arm mode code.  */
#  if defined(__thumb__) && !defined(__THUMB_INTERWORK__)
#   define __INTERWORKING_STUBS__
#  endif /* __thumb__ && !__THUMB_INTERWORK__ */

#endif /* __ARM_ARCH == 4 */

#else

# define RET		mov	pc, lr
# define RETc(x)	mov##x	pc, lr

#endif

.macro	cfi_pop		advance, reg, cfa_offset
#ifdef __ELF__
	.pushsection	.debug_frame
	.byte	0x4		/* DW_CFA_advance_loc4 */
	.4byte	\advance
	.byte	(0xc0 | \reg)	/* DW_CFA_restore */
	.byte	0xe		/* DW_CFA_def_cfa_offset */
	.uleb128 \cfa_offset
	.popsection
#endif
.endm
.macro	cfi_push	advance, reg, offset, cfa_offset
#ifdef __ELF__
	.pushsection	.debug_frame
	.byte	0x4		/* DW_CFA_advance_loc4 */
	.4byte	\advance
	.byte	(0x80 | \reg)	/* DW_CFA_offset */
	.uleb128 (\offset / -4)
	.byte	0xe		/* DW_CFA_def_cfa_offset */
	.uleb128 \cfa_offset
	.popsection
#endif
.endm
.macro cfi_start	start_label, end_label
#ifdef __ELF__
	.pushsection	.debug_frame
LSYM(Lstart_frame):
	.4byte	LSYM(Lend_cie) - LSYM(Lstart_cie) @ Length of CIE
LSYM(Lstart_cie):
        .4byte	0xffffffff	@ CIE Identifier Tag
        .byte	0x1	@ CIE Version
        .ascii	"\0"	@ CIE Augmentation
        .uleb128 0x1	@ CIE Code Alignment Factor
        .sleb128 -4	@ CIE Data Alignment Factor
        .byte	0xe	@ CIE RA Column
        .byte	0xc	@ DW_CFA_def_cfa
        .uleb128 0xd
        .uleb128 0x0

	.align 2
LSYM(Lend_cie):
	.4byte	LSYM(Lend_fde)-LSYM(Lstart_fde)	@ FDE Length
LSYM(Lstart_fde):
	.4byte	LSYM(Lstart_frame)	@ FDE CIE offset
	.4byte	\start_label	@ FDE initial location
	.4byte	\end_label-\start_label	@ FDE address range
	.popsection
#endif
.endm
.macro cfi_end	end_label
#ifdef __ELF__
	.pushsection	.debug_frame
	.align	2
LSYM(Lend_fde):
	.popsection
\end_label:
#endif
.endm

/* Don't pass dirn, it's there just to get token pasting right.  */

.macro	RETLDM	regs=, cond=, unwind=, dirn=ia
#if defined (__INTERWORKING__)
	.ifc "\regs",""
	ldr\cond	lr, [sp], #8
	.else
# if defined(__thumb2__)
	pop\cond	{\regs, lr}
# else
	ldm\cond\dirn	sp!, {\regs, lr}
# endif
	.endif
	.ifnc "\unwind", ""
	/* Mark LR as restored.  */
97:	cfi_pop 97b - \unwind, 0xe, 0x0
	.endif
	bx\cond	lr
#else
	/* Caller is responsible for providing IT instruction.  */
	.ifc "\regs",""
	ldr\cond	pc, [sp], #8
	.else
# if defined(__thumb2__)
	pop\cond	{\regs, pc}
# else
	ldm\cond\dirn	sp!, {\regs, pc}
# endif
	.endif
#endif
.endm

/* The Unified assembly syntax allows the same code to be assembled for both
   ARM and Thumb-2.  However this is only supported by recent gas, so define
   a set of macros to allow ARM code on older assemblers.  */
#if defined(__thumb2__)
.macro do_it cond, suffix=""
	it\suffix	\cond
.endm
.macro shift1 op, arg0, arg1, arg2
	\op	\arg0, \arg1, \arg2
.endm
#define do_push	push
#define do_pop	pop
#define COND(op1, op2, cond) op1 ## op2 ## cond
/* Perform an arithmetic operation with a variable shift operand.  This
   requires two instructions and a scratch register on Thumb-2.  */
.macro shiftop name, dest, src1, src2, shiftop, shiftreg, tmp
	\shiftop \tmp, \src2, \shiftreg
	\name \dest, \src1, \tmp
.endm
#else
.macro do_it cond, suffix=""
.endm
.macro shift1 op, arg0, arg1, arg2
	mov	\arg0, \arg1, \op \arg2
.endm
#define do_push	stmfd sp!,
#define do_pop	ldmfd sp!,
#define COND(op1, op2, cond) op1 ## cond ## op2
.macro shiftop name, dest, src1, src2, shiftop, shiftreg, tmp
	\name \dest, \src1, \src2, \shiftop \shiftreg
.endm
#endif

#ifdef __ARM_EABI__
.macro ARM_LDIV0 name signed
	cmp	r0, #0
	.ifc	\signed, unsigned
	movne	r0, #0xffffffff
	.else
	movgt	r0, #0x7fffffff
	movlt	r0, #0x80000000
	.endif
	b	SYM (__aeabi_idiv0) __PLT__
.endm
#else
.macro ARM_LDIV0 name signed
	str	lr, [sp, #-8]!
98:	cfi_push 98b - __\name, 0xe, -0x8, 0x8
	bl	SYM (__div0) __PLT__
	mov	r0, #0			@ About as wrong as it could be.
	RETLDM	unwind=98b
.endm
#endif


#ifdef __ARM_EABI__
.macro THUMB_LDIV0 name signed
#if defined(__ARM_ARCH_6M__)
	.ifc \signed, unsigned
	cmp	r0, #0
	beq	1f
	mov	r0, #0
	mvn	r0, r0		@ 0xffffffff
1:
	.else
	cmp	r0, #0
	beq	2f
	blt	3f
	mov	r0, #0
	mvn	r0, r0
	lsr	r0, r0, #1	@ 0x7fffffff
	b	2f
3:	mov	r0, #0x80
	lsl	r0, r0, #24	@ 0x80000000
2:
	.endif
	push	{r0, r1, r2}
	ldr	r0, 4f
	adr	r1, 4f
	add	r0, r1
	str	r0, [sp, #8]
	@ We know we are not on armv4t, so pop pc is safe.
	pop	{r0, r1, pc}
	.align	2
4:
	.word	__aeabi_idiv0 - 4b
#elif defined(__thumb2__)
	.syntax unified
	.ifc \signed, unsigned
	cbz	r0, 1f
	mov	r0, #0xffffffff
1:
	.else
	cmp	r0, #0
	do_it	gt
	movgt	r0, #0x7fffffff
	do_it	lt
	movlt	r0, #0x80000000
	.endif
	b.w	SYM(__aeabi_idiv0) __PLT__
#else
	.align	2
	bx	pc
	nop
	.arm
	cmp	r0, #0
	.ifc	\signed, unsigned
	movne	r0, #0xffffffff
	.else
	movgt	r0, #0x7fffffff
	movlt	r0, #0x80000000
	.endif
	b	SYM(__aeabi_idiv0) __PLT__
	.thumb
#endif
.endm
#else
.macro THUMB_LDIV0 name signed
	push	{ r1, lr }
98:	cfi_push 98b - __\name, 0xe, -0x4, 0x8
	bl	SYM (__div0)
	mov	r0, #0			@ About as wrong as it could be.
#if defined (__INTERWORKING__)
	pop	{ r1, r2 }
	bx	r2
#else
	pop	{ r1, pc }
#endif
.endm
#endif

.macro FUNC_END name
	SIZE (__\name)
.endm

.macro DIV_FUNC_END name signed
	cfi_start	__\name, LSYM(Lend_div0)
LSYM(Ldiv0):
#ifdef __thumb__
	THUMB_LDIV0 \name \signed
#else
	ARM_LDIV0 \name \signed
#endif
	cfi_end	LSYM(Lend_div0)
	FUNC_END \name
.endm

.macro THUMB_FUNC_START name
	.globl	SYM (\name)
	TYPE	(\name)
	.thumb_func
SYM (\name):
.endm

/* Function start macros.  Variants for ARM and Thumb.  */

#ifdef __thumb__
#define THUMB_FUNC .thumb_func
#define THUMB_CODE .force_thumb
# if defined(__thumb2__)
#define THUMB_SYNTAX .syntax divided
# else
#define THUMB_SYNTAX
# endif
#else
#define THUMB_FUNC
#define THUMB_CODE
#define THUMB_SYNTAX
#endif

.macro FUNC_START name
	.text
	.globl SYM (__\name)
	.hidden SYM (__\name)
	TYPE (__\name)
	.align 0
	THUMB_CODE
	THUMB_FUNC
	THUMB_SYNTAX
SYM (__\name):
.endm

/* Special function that will always be coded in ARM assembly, even if
   in Thumb-only compilation.  */

#if defined(__thumb2__)

/* For Thumb-2 we build everything in thumb mode.  */
.macro ARM_FUNC_START name
       FUNC_START \name
       .syntax unified
.endm
#define EQUIV .thumb_set
.macro  ARM_CALL name
	bl	__\name
.endm

#elif defined(__INTERWORKING_STUBS__)

.macro	ARM_FUNC_START name
	FUNC_START \name
	bx	pc
	nop
	.arm
/* A hook to tell gdb that we've switched to ARM mode.  Also used to call
   directly from other local arm routines.  */
_L__\name:		
.endm
#define EQUIV .thumb_set
/* Branch directly to a function declared with ARM_FUNC_START.
   Must be called in arm mode.  */
.macro  ARM_CALL name
	bl	_L__\name
.endm

#else /* !(__INTERWORKING_STUBS__ || __thumb2__) */

#ifdef __ARM_ARCH_6M__
#define EQUIV .thumb_set
#else
.macro	ARM_FUNC_START name
	.text
	.globl SYM (__\name)
	.hidden SYM (__\name)
	TYPE (__\name)
	.align 0
	.arm
SYM (__\name):
.endm
#define EQUIV .set
.macro  ARM_CALL name
	bl	__\name
.endm
#endif

#endif

.macro	FUNC_ALIAS new old
	.globl	SYM (__\new)
	.hidden	SYM (__\new)
#if defined (__thumb__)
	.thumb_set	SYM (__\new), SYM (__\old)
#else
	.set	SYM (__\new), SYM (__\old)
#endif
.endm

#ifndef __ARM_ARCH_6M__
.macro	ARM_FUNC_ALIAS new old
	.globl	SYM (__\new)
	.hidden	SYM (__\new)
	EQUIV	SYM (__\new), SYM (__\old)
#if defined(__INTERWORKING_STUBS__)
	.set	SYM (_L__\new), SYM (_L__\old)
#endif
.endm
#endif

#ifdef __ARMEB__
#define xxh r0
#define xxl r1
#define yyh r2
#define yyl r3
#else
#define xxh r1
#define xxl r0
#define yyh r3
#define yyl r2
#endif	

#ifdef __ARM_EABI__
.macro	WEAK name
	.weak SYM (__\name)
.endm
#endif
