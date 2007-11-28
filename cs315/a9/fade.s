	.file	"fade.c"
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

	.bss
g_fadeCount:
	.space	1
	.text
	.align	2
	.global	FadeBGInit
	.type	FadeBGInit, %function
FadeBGInit:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L2
	mov	r2, #0
	@ lr needed for prologue
	strb	r2, [r3, #0]	@  g_fadeCount
	bx	lr
.L3:
	.align	2
.L2:
	.word	g_fadeCount
	.size	FadeBGInit, .-FadeBGInit
	.align	2
	.global	FadeBG
	.type	FadeBG, %function
FadeBG:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {r4, fp, ip, lr, pc}
	ldr	r4, .L7
	ldrb	r3, [r4, #0]	@ zero_extendqisi2	@  g_fadeCount
	ands	r0, r3, #1
	sub	fp, ip, #4
	mov	r1, #1
	beq	.L6
.L5:
	add	r3, r3, #1
	strb	r3, [r4, #0]	@  g_fadeCount
	ldrb	r0, [r4, #0]	@ zero_extendqisi2	@  g_fadeCount
	cmp	r0, #60
	movne	r0, #0
	moveq	r0, #1
	ldmea	fp, {r4, fp, sp, lr}
	bx	lr
.L6:
	ldr	r3, .L7+4
	mov	lr, pc
	bx	r3
	ldrb	r3, [r4, #0]	@ zero_extendqisi2	@  g_fadeCount
	b	.L5
.L8:
	.align	2
.L7:
	.word	g_fadeCount
	.word	ham_FadePal
	.size	FadeBG, .-FadeBG
	.ident	"GCC: (GNU) 3.3.2"
