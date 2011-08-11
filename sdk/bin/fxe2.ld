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
		KEEP( *(.fini_array) )
		KEEP( *(SORT(.fini_array.*)) )
		__feos_fini_size = . ;
		LONG((__feos_fini_size - __feos_fini_array) / 4);

		/* (Experimental) Exception Frame */
		KEEP( *(.eh_frame) )

		/* (Experimental) GCC excpt metadata */
		*(.gcc_except_table)
	} : loadable

	__text_end =  . ;

	.rodata ALIGN(4) :
	{
		*(.rodata)
		*(.roda)
		*(.rodata.*)
		*(.pai)
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

	/DISCARD/ : { *(.group .comment .note) }
}
