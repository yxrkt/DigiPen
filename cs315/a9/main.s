	.file	"main.c"
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

	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	ldr	r3, .L2
	sub	fp, ip, #4
	mov	lr, pc
	bx	r3
	ldr	r1, .L2+4
	mov	r0, #0
	ldr	r3, .L2+8
	mov	lr, pc
	bx	r3
	ldr	r2, .L2+12
	mov	lr, pc
	bx	r2
	mov	r0, #0
	ldmea	fp, {fp, sp, lr}
	bx	lr
.L3:
	.align	2
.L2:
	.word	ham_Init
	.word	VerticalBlank
	.word	ham_StartIntHandler
	.word	GameProc
	.size	main, .-main
	.align	2
	.global	GameProc
	.type	GameProc, %function
GameProc:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}
	ldr	r5, .L30
	ldr	r6, .L30+4
	ldr	r7, .L30+8
	ldr	r8, .L30+12
	ldr	sl, .L30+16
	ldr	r9, .L30+20
	sub	fp, ip, #4
	mov	r4, #0	@  gameState
.L27:
	mov	lr, pc
	bx	r5
	tst	r0, #255
	beq	.L27
	mov	r0, #0
	mov	lr, pc
	bx	r6
	cmp	r4, #10	@  gameState
	ldrls	pc, [pc, r4, asl #2]	@  gameState
	b	.L11
	.p2align 2
.L24:
	.word	.L12
	.word	.L13
	.word	.L14
	.word	.L15
	.word	.L16
	.word	.L17
	.word	.L18
	.word	.L19
	.word	.L20
	.word	.L21
	.word	.L22
.L12:
	add	r4, r4, #1	@  gameState,  gameState
.L11:
	cmp	r4, #10	@  gameState
	movgt	r4, #1	@  gameState
	b	.L27
.L13:
	ldr	r3, .L30+24
.L28:
	mov	lr, pc
	bx	r3
.L29:
	add	r4, r4, r0	@  gameState,  gameState
	b	.L11
.L14:
	ldr	r3, .L30+28
	b	.L28
.L15:
	ldr	r3, .L30+32
	b	.L28
.L16:
	mov	lr, pc
	bx	r7
	b	.L29
.L17:
	mov	lr, pc
	bx	r8
	b	.L29
.L18:
	mov	lr, pc
	bx	sl
	b	.L29
.L19:
	ldr	r3, .L30+36
	b	.L28
.L20:
	mov	lr, pc
	bx	r9
	b	.L29
.L21:
	ldr	r3, .L30+40
	b	.L28
.L22:
	ldr	r3, .L30+44
	b	.L28
.L31:
	.align	2
.L30:
	.word	GetVerticalBlankState
	.word	SetVerticalBlankState
	.word	GSStoryProc
	.word	GSGame
	.word	GSGameProc
	.word	GSDeathProc
	.word	GSTitle
	.word	GSTitleProc
	.word	GSStory
	.word	GSDeath
	.word	GSWin
	.word	GSWinProc
	.size	GameProc, .-GameProc
	.ident	"GCC: (GNU) 3.3.2"
