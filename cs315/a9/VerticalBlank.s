	.file	"VerticalBlank.c"
@ GNU C version 3.3.2 (arm-thumb-elf)
@	compiled by GNU C version 3.3.1 (cygming special).
@ GGC heuristics: --param ggc-min-expand=99 --param ggc-min-heapsize=130827
@ options passed:  -fpreprocessed -mthumb-interwork -mlong-calls
@ -auxbase-strip -O2 -Wall -fverbose-asm
@ options enabled:  -fdefer-pop -fomit-frame-pointer
@ -foptimize-sibling-calls -fcse-follow-jumps -fcse-skip-blocks
@ -fexpensive-optimizations -fthread-jumps -fstrength-reduce -fpeephole
@ -fforce-mem -ffunction-cse -fkeep-static-consts -fcaller-saves
@ -freg-struct-return -fgcse -fgcse-lm -fgcse-sm -floop-optimize
@ -fcrossjumping -fif-conversion -fif-conversion2 -frerun-cse-after-loop
@ -frerun-loop-opt -fdelete-null-pointer-checks -fschedule-insns
@ -fschedule-insns2 -fsched-interblock -fsched-spec -fbranch-count-reg
@ -freorder-blocks -freorder-functions -fcprop-registers -fcommon
@ -fverbose-asm -fgnu-linker -fregmove -foptimize-register-move
@ -fargument-alias -fstrict-aliasing -fmerge-constants
@ -fzero-initialized-in-bss -fident -fpeephole2 -fguess-branch-probability
@ -fmath-errno -ftrapping-math -mapcs -mapcs-frame -mapcs-32 -msoft-float
@ -mthumb-interwork -mlong-calls

	.bss
g_vertBlankState:
	.space	1
	.align	2
g_vertBlankCount:
	.space	4
	.text
	.align	2
	.global	VerticalBlank
	.type	VerticalBlank, %function
VerticalBlank:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	ldr	r3, .L4
	sub	fp, ip, #4
	mov	lr, pc
	bx	r3
	ldr	r2, .L4+4
	mov	lr, pc
	bx	r2
	ldr	r2, .L4+8
	ldr	r3, [r2, #0]	@  g_vertBlankCount
	cmp	r3, #60
	add	r3, r3, #1
	moveq	r3, #0
	str	r3, [r2, #0]	@  g_vertBlankCount
	ldr	r3, .L4+12
	mov	r2, #1
	strb	r2, [r3, #0]	@  g_vertBlankState
	ldmea	fp, {fp, sp, lr}
	bx	lr
.L5:
	.align	2
.L4:
	.word	ham_CopyObjToOAM
	.word	UpdateController
	.word	g_vertBlankCount
	.word	g_vertBlankState
	.size	VerticalBlank, .-VerticalBlank
	.align	2
	.global	GetVerticalBlankState
	.type	GetVerticalBlankState, %function
GetVerticalBlankState:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L7
	ldrb	r0, [r3, #0]	@ zero_extendqisi2	@  g_vertBlankState
	@ lr needed for prologue
	bx	lr
.L8:
	.align	2
.L7:
	.word	g_vertBlankState
	.size	GetVerticalBlankState, .-GetVerticalBlankState
	.align	2
	.global	SetVerticalBlankState
	.type	SetVerticalBlankState, %function
SetVerticalBlankState:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L10
	@ lr needed for prologue
	strb	r0, [r3, #0]	@  g_vertBlankState
	bx	lr
.L11:
	.align	2
.L10:
	.word	g_vertBlankState
	.size	SetVerticalBlankState, .-SetVerticalBlankState
	.align	2
	.global	GetVerticalBlankCount
	.type	GetVerticalBlankCount, %function
GetVerticalBlankCount:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L13
	ldr	r0, [r3, #0]	@  g_vertBlankCount
	@ lr needed for prologue
	bx	lr
.L14:
	.align	2
.L13:
	.word	g_vertBlankCount
	.size	GetVerticalBlankCount, .-GetVerticalBlankCount
	.ident	"GCC: (GNU) 3.3.2"
