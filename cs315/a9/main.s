	.file	"main.c"
@ GNU C version 3.3.2 (arm-thumb-elf)
@	compiled by GNU C version 3.3.1 (cygming special).
@ GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
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
	ldr	r5, .L28
	ldr	r6, .L28+4
	ldr	r9, .L28+8
	ldr	sl, .L28+12
	ldr	r8, .L28+16
	ldr	r7, .L28+20
	sub	fp, ip, #4
	mov	r4, #0	@  gameState
.L25:
	mov	lr, pc
	bx	r5
	tst	r0, #255
	beq	.L25
	mov	r0, #0
	mov	lr, pc
	bx	r6
	cmp	r4, #8	@  gameState
	ldrls	pc, [pc, r4, asl #2]	@  gameState
	b	.L11
	.p2align 2
.L22:
	.word	.L12
	.word	.L13
	.word	.L14
	.word	.L15
	.word	.L16
	.word	.L17
	.word	.L18
	.word	.L19
	.word	.L20
.L12:
	add	r4, r4, #1	@  gameState,  gameState
.L11:
	cmp	r4, #8	@  gameState
	movgt	r4, #1	@  gameState
	b	.L25
.L13:
	ldr	r3, .L28+24
.L27:
	mov	lr, pc
	bx	r3
.L26:
	add	r4, r4, r0	@  gameState,  gameState
	b	.L11
.L14:
	ldr	r3, .L28+28
	b	.L27
.L15:
	ldr	r3, .L28+32
	b	.L27
.L16:
	mov	lr, pc
	bx	r9
	b	.L26
.L17:
	ldr	r3, .L28+36
	b	.L27
.L18:
	mov	lr, pc
	bx	sl
	b	.L26
.L19:
	mov	lr, pc
	bx	r8
	b	.L26
.L20:
	mov	lr, pc
	bx	r7
	b	.L26
.L29:
	.align	2
.L28:
	.word	GetVerticalBlankState
	.word	SetVerticalBlankState
	.word	GSStoryProc
	.word	GSGameProc
	.word	GSDeath
	.word	GSDeathProc
	.word	GSTitle
	.word	GSTitleProc
	.word	GSStory
	.word	GSGame
	.size	GameProc, .-GameProc
	.ident	"GCC: (GNU) 3.3.2"
