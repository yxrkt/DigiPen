	.file	"Controller.c"
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
	.global	UpdateController
	.type	UpdateController, %function
UpdateController:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r0, #67108864
	add	r0, r0, #304
	ldrh	r3, [r0, #0]
	mvn	r3, r3
	ldr	ip, .L2
	tst	r3, #64
	moveq	r2, #0
	movne	r2, #1
	strb	r2, [ip, #0]	@  g_controllerState.up
	ldrh	r3, [r0, #0]
	mvn	r3, r3
	tst	r3, #128
	moveq	r2, #0
	movne	r2, #1
	strb	r2, [ip, #1]	@  g_controllerState.down
	ldrh	r3, [r0, #0]
	mvn	r3, r3
	tst	r3, #32
	moveq	r2, #0
	movne	r2, #1
	strb	r2, [ip, #2]	@  g_controllerState.left
	ldrh	r3, [r0, #0]
	mvn	r3, r3
	tst	r3, #16
	moveq	r2, #0
	movne	r2, #1
	strb	r2, [ip, #3]	@  g_controllerState.right
	ldrh	r3, [r0, #0]
	mvn	r3, r3
	tst	r3, #2
	moveq	r2, #0
	movne	r2, #1
	strb	r2, [ip, #4]	@  g_controllerState.b
	ldrh	r3, [r0, #0]
	mvn	r3, r3
	and	r3, r3, #1
	strb	r3, [ip, #5]	@  g_controllerState.a
	ldrh	r2, [r0, #0]
	mvn	r2, r2
	tst	r2, #256
	moveq	r1, #0
	movne	r1, #1
	strb	r1, [ip, #6]	@  g_controllerState.r
	ldrh	r3, [r0, #0]
	mvn	r3, r3
	and	r3, r3, #1
	strb	r3, [ip, #7]	@  g_controllerState.l
	ldrh	r2, [r0, #0]
	mvn	r2, r2
	tst	r2, #4
	moveq	r1, #0
	movne	r1, #1
	strb	r1, [ip, #8]	@  g_controllerState.select
	ldrh	r3, [r0, #0]
	mvn	r3, r3
	tst	r3, #8
	moveq	r2, #0
	movne	r2, #1
	@ lr needed for prologue
	strb	r2, [ip, #9]	@  g_controllerState.start
	bx	lr
.L3:
	.align	2
.L2:
	.word	g_controllerState
	.size	UpdateController, .-UpdateController
	.align	2
	.global	GetControllerState
	.type	GetControllerState, %function
GetControllerState:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L5
	mov	ip, r0
	ldmia	r3, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	mov	r0, ip
	@ lr needed for prologue
	bx	lr
.L6:
	.align	2
.L5:
	.word	g_controllerState
	.size	GetControllerState, .-GetControllerState
	.bss
	.align	2
g_controllerState:
	.space	12
	.ident	"GCC: (GNU) 3.3.2"
