OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(__entrypoint)

PHDRS
{
	loadable PT_LOAD;
	metadata PT_LOAD; /* will not be loaded */
}

SECTIONS
{
	/* ===========================
	   ==== Physical sections ====
	   =========================== */
	. = 0;
	__start__ = . ;
	__hinstance = . ;
	__load_start = . ;

	__text_start = . ;

	.text ALIGN(4) :
	{
		/* .init */
		KEEP( *(.init) )
		. = ALIGN(4);

		/* .text */
		*(.text)
		*(.text.*)
		/* *(.imp.*) */ /* Let ld do it for us and preserve section names */
		*(.glue_7)
		*(.glue_7t)
		*(.stub)
		*(.gnu.warning)
		*(.gnu.linkonce.t*)
		. = ALIGN(4);

		/* Constructor table */
		__feos_init_array =  . ;
		KEEP( *(SORT(.preinit_array.*)) )
		KEEP( *(.preinit_array) )
		KEEP( *(SORT(.init_array.*)) )
		KEEP( *(.init_array) )
		KEEP( *(SORT(.ctors.*)) )
		KEEP( *(.ctors) )
		__feos_init_size = . ;
		LONG((__feos_init_size - __feos_init_array) / 4);

		/* Destructor table */
		__feos_fini_array =  . ;
		KEEP( *(SORT(.dtors.*)) )
		KEEP( *(.dtors) )
		KEEP( *(SORT(.fini_array.*)) )
		KEEP( *(.fini_array) )
		__feos_fini_size = . ;
		LONG((__feos_fini_size - __feos_fini_array) / 4);
	} : loadable

	__text_end =  . ;

	.rodata ALIGN(4) :
	{
		*(.rodata)
		*(.roda)
		*(.rodata.*)
		*all.rodata*(*)
		*(.gnu.linkonce.r*)
		SORT(CONSTRUCTORS)
		. = ALIGN(4);
	} : loadable

	.data ALIGN(4) :
	{
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d*)
		CONSTRUCTORS
		. = ALIGN(4);
	} : loadable

	.ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } : loadable
 	__exidx_start = .;
	ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } : loadable
 	__exidx_end = .;

	__load_end = . ;

	__bss_start = . ;

	.bss ALIGN(4) (NOLOAD) :
	{
		*(.dynbss)
		*(.bss)
		*(.bss.*)
		*(.gnu.linkonce.b*)
		*(COMMON)
		. = ALIGN(4);
	}

	__bss_end = . ;

	__end__ = ABSOLUTE(.) ;

	/* ==================
	   ==== Metadata ====
	   ================== */

	.meta ALIGN(4) :
	{
		*(.meta)
	}

	/* Discard sections that difficult fxe2tool post-processing */
	/DISCARD/ : { *(.group .comment .note) }

	/* Stabs debugging sections. */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }

	/* DWARF debug sections.
	   Symbols in the DWARF debugging sections are relative to the beginning
	   of the section so we begin them at 0. */

	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }

	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }

	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }

	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
}
