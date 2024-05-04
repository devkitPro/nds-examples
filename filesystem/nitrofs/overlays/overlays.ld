/*--------------------------------------------------------------------------------
	** Example linker script for ARM9 overlays **
--------------------------------------------------------------------------------*/

MEMORY {
	/* Placeholder LMA for overlays */
	ovlmem : ORIGIN = 0xb0000000, LENGTH = 0x10000000
}

PHDRS {
	/* Program header for the overlay table */
	ovltable  PT_LOAD FLAGS(0x200007);

	/* Program headers for each overlay (in order) */
	ovl_test0 PT_LOAD FLAGS(0x200007);
	ovl_test1 PT_LOAD FLAGS(0x200007);
	ovl_test2 PT_LOAD FLAGS(0x200007);
}

/* Overlays in main RAM */
SECTIONS {

	PROVIDE_HIDDEN( __ovlarea_main_start = . );
	PROVIDE_HIDDEN( __ovlarea_main_end = . );

	/* ======== Overlay 'test0' ======== */

	. = __ovlarea_main_start;

	.ovl.test0 ALIGN(32) : {
		:test0.o (.text .text.* .gnu.linkonce.t.* .glue_7t .glue_7 .vfp11_veneer .v4_bx)
		:test0.o (.rodata .rodata1 .rodata.* .gnu.linkonce.r.*)
		:test0.o (.data .data1 .data.* .gnu.linkonce.d.*)
		. = ALIGN(4);

		PROVIDE_HIDDEN( __ovl_ctor_start_test0 = . );
		KEEP( :test0.o (SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*) .init_array .ctors) )
		PROVIDE_HIDDEN( __ovl_ctor_end_test0 = . );
		KEEP( :test0.o (SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*) .fini_array .dtors) )
		PROVIDE_HIDDEN( __ovl_dtor_end_test0 = . );
	} AT>ovlmem :ovl_test0 = 0xff

	.ovl.test0.bss . (NOLOAD) : {
		:test0.o (.bss .bss.* .dynbss .gnu.linkonce.b.* COMMON)
		. = ALIGN(4);
	} AT>ovlmem :ovl_test0

	PROVIDE_HIDDEN( __ovl_test0_end = . );
	__ovlarea_main_end = MAX(__ovlarea_main_end, .);

	/* ======== Overlay 'test2' ======== */

	. = __ovlarea_main_start;

	.ovl.test2 ALIGN(32) : {
		:test2.o (.text .text.* .gnu.linkonce.t.* .glue_7t .glue_7 .vfp11_veneer .v4_bx)
		:test2.o (.rodata .rodata1 .rodata.* .gnu.linkonce.r.*)
		:test2.o (.data .data1 .data.* .gnu.linkonce.d.*)
		. = ALIGN(4);

		PROVIDE_HIDDEN( __ovl_ctor_start_test2 = . );
		KEEP( :test2.o (SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*) .init_array .ctors) )
		PROVIDE_HIDDEN( __ovl_ctor_end_test2 = . );
		KEEP( :test2.o (SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*) .fini_array .dtors) )
		PROVIDE_HIDDEN( __ovl_dtor_end_test2 = . );
	} AT>ovlmem :ovl_test2 = 0xff

	.ovl.test2.bss . (NOLOAD) : {
		:test2.o (.bss .bss.* .dynbss .gnu.linkonce.b.* COMMON)
		. = ALIGN(4);
	} AT>ovlmem :ovl_test2

	PROVIDE_HIDDEN( __ovl_test2_end = . );
	__ovlarea_main_end = MAX(__ovlarea_main_end, .);

	. = __ovl_test2_end;

	. = __ovlarea_main_end;

}
INSERT AFTER .main.bss;

/* Overlays in ITCM */
SECTIONS {

	PROVIDE_HIDDEN( __ovlarea_itcm_start = . );
	PROVIDE_HIDDEN( __ovlarea_itcm_end = . );

	/* ======== Overlay 'test1' ======== */

	. = __ovlarea_itcm_start;

	.ovl.test1 ALIGN(32) : {
		:test1.o (.text .text.* .gnu.linkonce.t.* .glue_7t .glue_7 .vfp11_veneer .v4_bx)
		:test1.o (.rodata .rodata1 .rodata.* .gnu.linkonce.r.*)
		:test1.o (.data .data1 .data.* .gnu.linkonce.d.*)
		. = ALIGN(4);

		/* No static ctors/dtors used */
	} AT>ovlmem :ovl_test1 = 0xff

	.ovl.test1.bss . (NOLOAD) : {
		:test1.o (.bss .bss.* .dynbss .gnu.linkonce.b.* COMMON)
		. = ALIGN(4);
	} AT>ovlmem :ovl_test1

	PROVIDE_HIDDEN( __ovl_test1_end = . );
	__ovlarea_itcm_end = MAX(__ovlarea_itcm_end, .);

	. = __ovlarea_itcm_end;

}
INSERT AFTER .itcm;

SECTIONS {

	.ovltable 0 : {
		/* Static ctors/dtors for overlay 'test0' */
		LONG(__ovl_ctor_start_test0)
		LONG(__ovl_ctor_end_test0)
		LONG(__ovl_dtor_end_test0)

		/* Static ctors/dtors for overlay 'test1' (not used) */
		LONG(0)
		LONG(0)
		LONG(0)

		/* Static ctors/dtors for overlay 'test2' */
		LONG(__ovl_ctor_start_test2)
		LONG(__ovl_ctor_end_test2)
		LONG(__ovl_dtor_end_test2)
	} AT>ovlmem :ovltable

}
