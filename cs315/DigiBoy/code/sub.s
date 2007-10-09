;---------------------------------------
;
;			Update Game
;
;---------------------------------------

update_proc
	ld		a,(mode)
	
	cp		mmenu
	call	z,menu_proc
	
	cp		mstory
	call	z,story_proc
	
	cp		mgame
	call	z,game_proc

	ret

;---------------------------------------
;	Move player vertically
;---------------------------------------

movev
	ld		a,(height)
	cp		0
	jr		nz,_vmov
	
	call	btn_hit
	cp		0
	jr		z,_vmov
	
	ld		a,1
	ld		(bjump),a

_vmov
	xor		a							; check if falling
	ld		hl,bjump
	cp		(hl)
	jr		z,_vfall
	
	ld		a,(maxh)					; start falling if at max height
	ld		hl,height
	cp		(hl)
	jr		z,_vfall
	
	ld		a,($fe00)					; move player up one pixel
	dec		a
	ld		($fe00),a
	ld		($fe04),a
	add		a,$08
	ld		($fe08),a
	ld		($fe0c),a
	
	ld		a,(height)					; increment height counter
	inc		a
	ld		(height),a
	
	jr		_done_movev

_vfall
	ld		a,(height)					; stop falling if landed
	cp		0
	jr		z,_done_movev
	
	xor		a							; set falling to true
	ld		(bjump),a
	
	ld		a,($fe00)					; move player down one pixel
	inc		a
	ld		($fe00),a
	ld		($fe04),a
	add		a,$08
	ld		($fe08),a
	ld		($fe0c),a
	
	ld		a,(height)					; increment height counter
	dec		a
	ld		(height),a

_done_movev
	ret

;---------------------------------------
;	Move player horizontally
;	params: b -- zero if moving left
;			d -- speed to move
;---------------------------------------

moveh

	xor		a
	cp		b
	jr		z,_go_left
	
	; check if player is possibly not in middle of screen
	ld		a,(scx)
	cp		0
	ld		c,d
	jr		z,_loop_movr
	
	; scroll right
	ld		a,d
_scroll_right
	call	scroll_right
	
	; walk without scrolling if scroll counter is non-zero
_loop_movr
	ld		a,c
	cp		0
	jr		z,_done_moveh
	
	ld		a,($fe01)					; stop at right boundary
	cp		154
	jr		z,_done_moveh
	
	ld		a,(scx)						; approaching right boundary
	cp		95
	jr		z,_movr_cont
	
	ld		a,($fe01)					; reached center of screen
	cp		79
	ld		a,c
	jr		z,_scroll_right

_movr_cont	
	ld		a,($fe01)					; move player 1 pixel right
	inc		a
	ld		($fe01),a
	ld		($fe09),a
	add		a,$08
	ld		($fe05),a
	ld		($fe0d),a
	
	dec		c
	jr		_loop_movr
	
_go_left
	; check if player is possibly not in middle of screen
	ld		a,(scx)
	cp		95
	ld		c,d
	jr		z,_loop_movl
	
	; scroll left
	ld		a,d
_scroll_left
	call	scroll_left
	
	; walk without scrolling if scroll counter is non-zero
_loop_movl
	ld		a,c
	cp		0
	jr		z,_done_moveh
	
	ld		a,($fe01)					; stop at left boundary
	cp		6
	jr		z,_done_moveh
	
	ld		a,(scx)						; approaching left boundary
	cp		0
	jr		z,_movl_cont
	
	ld		a,($fe01)					; reached center of screen
	cp		79
	ld		a,c
	jr		z,_scroll_left

_movl_cont	
	ld		a,($fe01)					; move player 1 pixel left
	dec		a
	ld		($fe01),a
	ld		($fe09),a
	add		a,$08
	ld		($fe05),a
	ld		($fe0d),a
	
	dec		c
	jr		_loop_movl
	
_done_moveh
	ret

;---------------------------------------
;	Scroll Down
;	params:	a -- pixels per frame
;---------------------------------------

scroll_down

	ld		c,a

_loop_sd
	xor		a
	cp		c
	jr		z,done_scrdwn
	dec		c	
	ld		a,(scy)
	cp		111
	jr		z,done_scrdwn
	inc		a
	ld		(scy),a
	jr		_loop_sd

done_scrdwn	
	ret

;---------------------------------------
;	Scroll Left
;	params:	a -- pixels per frame
;---------------------------------------

scroll_left

	ld		c,a

_loop_sl
	xor		a
	cp		c
	jr		z,done_scrlft
	ld		a,(scx)
	cp		0
	jr		z,done_scrlft
	dec		c
	dec		a
	ld		(scx),a
	jr		_loop_sl

done_scrlft	
	ret

;---------------------------------------
;	Scroll Right
;	params:	a -- pixels per frame
;---------------------------------------

scroll_right

	ld		c,a

_loop_sr
	xor		a
	cp		c
	jr		z,done_scrrt
	ld		a,(scx)
	cp		95
	jr		z,done_scrrt
	dec		c	
	inc		a
	ld		(scx),a
	jr		_loop_sr

done_scrrt	
	ret

;---------------------------------------
;	Button Down
;	params: none
;	note: z is 1 if not hit
;---------------------------------------

btn_hit
	; Get input state
	call	cont
	ld		a,(cnt1)
	bit		BitA,a
	jr		z,reset_hold

	; Check if A is held
	xor		a
	ld		hl,bheld
	cp		(hl)
	jr		nz,done_bhit

	; Set a to non-zero and bheld to true
	ld		a,1
	ld		(bheld),a
	jr		done_bhit
	
reset_hold
	xor		a
	ld		(bheld),a

done_bhit
	cp		0
	ret
