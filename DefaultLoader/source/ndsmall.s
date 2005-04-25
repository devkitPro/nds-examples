@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ NDSmall
@   by Rafael Vuijk (aka DarkFader)
@
@ Small NDS loader for PassMe/WifiMe/FlashMe
@
@ Usage:
@   cat ndsmall.bin game.nds > game.ds.gba 
@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.equ	save_REGS,				0x027FF000
	.equ	NewARM9Loop_dest,		0x027FF100
	.equ	RAM_HEADER,				0x027FFE00
	.equ	NDSROM_HEADER,			0x08000200

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global	_start
_start:
	b		start0
	.ds.b	12
	.byte	0x0D,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	.ascii	"Small NDS loader"
	.ascii	" for GBA flash- "
	.ascii	"cards.          "
	.byte	0x0D,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	.ascii	"cat ndsmall.bin "
	.ascii	"game.nds >      "
	.ascii	"game.ds.gba     "
	.byte	0x0D,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1A
	.ds.b	160 - .
	.ascii	"NDSmall 1.0 "	@ title (short)
	.ascii	"PASS"	@ gamecode
	.ascii	"DF"	@ maker code
	.byte	0x96	@ fixed
	.ds.b	13
start0:

	@ save registers
	ldr		r14, =save_REGS
	stmia	r14, {r0-r11,r13}

	@ clear RAM
	ldr		r0, =0x02000000
	ldmia	r0, {r0-r7}
	ldr		r8, =0x02004000
	ldr		r9, =0x023FC000
1:
	stmia	r8!, {r0-r7}
	cmp		r8, r9
	bne		1b

	@ commonly used addresses
	ldr		r11, =NDSROM_HEADER
	ldr		r12, =RAM_HEADER

	@ copy new ARM9 loop
	ldr		r0, NewARM9Loop
	ldr		r4, =NewARM9Loop_dest
	str		r0, [r4, #0]				@ place ldr instruction
	str		r4, [r4, #4]				@ address of ldr instruction
	str		r4, [r12, #0x24]			@ go to new loop

	@ copy header
	mov		r0, r11
	mov		r1, r12
	mov		r2, #0x170
	bl		Copy

	@ header CRC mirrors
	add		r4, r12, #0x100
	ldrh	r0, [r4, #0x5E]
	ldr		r2, =0x027FF808
	strh	r0, [r2]
	ldr		r2, =0x027FFC08
	strh	r0, [r2]

	@ secure area CRC mirrors
	add		r4, r12, #0x6C
	ldrh	r0, [r4]
	ldr		r2, =0x027FF80A
	strh	r0, [r2]
	ldr		r2, =0x027FFC0A
	strh	r0, [r2]

	@ copy ARM9 binary
	ldr		r0, [r12, #0x20]				@ ROM offset
	add		r0, r0, r11
	ldr		r1, [r12, #0x28]				@ RAM address
	ldr		r2, [r12, #0x2C]				@ code size
	bl		CopyAlign

	@ copy ARM7 binary
	ldr		r0, [r12, #0x30]				@ ROM offset
	add		r0, r0, r11
	ldr		r1, [r12, #0x38]				@ RAM address
	ldr		r2, [r12, #0x3C]				@ code size
	bl		Copy

	@ get ARM9/7 entry
	ldr		r0, [r12, #0x24]
	ldr		r12, [r12, #0x34]

	@ start ARM9
	ldr		r4, =NewARM9Loop_dest
	str		r0, [r4, #0x4]

	@ restore registers
	ldr		r14, =save_REGS
	ldmia	r14, {r0-r11,r13}

	mov		pc, r12

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

NewARM9Loop:
	ldr		pc, . + 4		@- 0x160 + 0x24			@ ARM9 entry

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ copy [r0+] to [r1+]; length r2

CopyAlign:
	mov		r3, #0x1FC
	add		r2, r2, r3
	bic		r2, r2, r3
Copy:
	ldr		r3, [r0], #4
	str		r3, [r1], #4
	subs	r2, r2, #4
	bgt		Copy
	mov		pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.pool

	.ds.b	0x200 - .

	.end

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
