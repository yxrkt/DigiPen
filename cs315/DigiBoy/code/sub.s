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
	
	cp		mend
	call	z,endgame_proc
	
	cp		mlose
	call	z,lose_proc

	ret

;---------------------------------------
;	Decrement lives
;---------------------------------------

dec_lives
	ld		a,(nlives)
	cp		0
	jr		nz,_dl_dec_lives

_dl_goto_lose
	ld		a,(mlose)
	ld		(mode),a
	call	clear_objs
	call	lose_init
	jr		_dec_lives_done

_dl_dec_lives
	ld		a,(nlives)
	dec		a
	ld		(nlives),a
	
_dec_lives_done
	ret

;---------------------------------------
;	Reset
;	params: a -- mode to reset to
;---------------------------------------

reset_to

	ld		b,a
	ld		(mode),a
	call	clear_objs
	ld		a,(lcdc)
	res		5,a
	ld		(lcdc),a

_check_menu_res
	ld		a,(mmenu)				; if b is menu
	cp		b
	jr		nz,_check_story_res
	
	; reset story variables
	xor		a
	ld		(bscroll),a
	ld		(bpart2),a
	
	call	menu_init
	jr		_done_resto
	
_check_story_res
	ld		a,(mstory)				; if b is story
	cp		b
	jr		nz,_check_game_res
	
	; reset story variables
	xor		a
	ld		(bscroll),a
	ld		(bpart2),a
	
	call	story_init
	jr		_done_resto

_check_game_res
	ld		a,(mgame)				; if b is game
	cp		b
	jr		nz,_done_resto
	
	call	game_init

_done_resto
	ret

;---------------------------------------
;	Clear objects
;---------------------------------------

clear_objs

	push	bc

	xor		a	
	ld		bc,$60
	ld		hl,$fe00
	call	memset
	
	pop		bc
	
	ret

;---------------------------------------
;	Move player vertically
;---------------------------------------

movev
	ld		a,(height)
	cp		0
	jr		nz,_vmov
	
	ld		a,(trg1)
	bit		BitA,a
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
	add		a,$08
	ld		($fe10),a
	ld		($fe14),a
	add		a,$08
	ld		($fe18),a
	ld		($fe1c),a
	
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
	add		a,$08
	ld		($fe10),a
	ld		($fe14),a
	add		a,$08
	ld		($fe18),a
	ld		($fe1c),a
	
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

	ld		c,10
	call	animate_player

	xor		a
	cp		b
	jr		z,_go_left
	
_go_right
	ld		b,1
	call	flip_player
	
	; check if player is possibly not in middle of screen
	ld		a,(scx)
	cp		0
	ld		c,d
	jr		z,_loop_movr
	
	ld		a,d
	
_scroll_right
	call	scroll_right
	
	; walk without scrolling if scroll counter is non-zero
_loop_movr
	ld		a,c
	cp		0
	jp		z,_done_moveh
	
	ld		a,($fe01)					; stop at right boundary
	cp		152
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
	ld		($fe11),a
	ld		($fe19),a
	add		a,$08
	ld		($fe05),a
	ld		($fe0d),a
	ld		($fe15),a
	ld		($fe1d),a
	
	dec		c
	jr		_loop_movr
	
_go_left
	ld		b,0
	call	flip_player
	
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
	cp		8
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
	ld		($fe11),a
	ld		($fe19),a
	add		a,$08
	ld		($fe05),a
	ld		($fe0d),a
	ld		($fe15),a
	ld		($fe1d),a
	
	dec		c
	jr		_loop_movl
	
_done_moveh
	ret


;---------------------------------------
;	Walking animation for the player
;	params: c -- time per frame
;---------------------------------------
animate_player
	ld		a,(atimer)
	inc		a
	ld		(atimer),a
	cp		c
	jr		nz,_done_animplayer
	
	xor		a
	ld		(atimer),a
	
	ld		a,(brface)
	cp		0
	jr		z,_load_lfreg
	
_load_rfreg
	ld		hl,$fe1e
	jr		_cont_animp
	
_load_lfreg
	ld		hl,$fe1a

_cont_animp
	ld		a,(hl)
	cp		7
	jr		z,_2nd_pframe

_1st_pframe	
	ld		a,7
	jr		_change_frame

_2nd_pframe
	ld		a,8

_change_frame
	ld		(hl),a

_done_animplayer
	ret

;---------------------------------------
;	Flip player
;	params: b -- 1 if going right
;---------------------------------------
flip_player

	xor		a
	cp		b
	jr		z,_flip_left

_flip_right
	ld		a,(brface)			; check if already facing right
	cp		1
	jp		z,_done_pflip
	
	ld		a,1					; set to facing right
	ld		(brface),a
	
	ld		a,($fe03)			; tile 0
	res		5,a
	ld		($fe03),a
	ld		a,($fe07)			; tile 1
	res		5,a
	ld		($fe07),a
	ld		a,($fe0b)			; tile 2
	res		5,a
	ld		($fe0b),a
	ld		a,($fe0f)			; tile 3
	res		5,a
	ld		($fe0f),a
	ld		a,($fe13)			; tile 4
	res		5,a
	ld		($fe13),a
	ld		a,($fe17)			; tile 5
	res		5,a
	ld		($fe17),a
	ld		a,($fe1b)			; tile 6
	res		5,a
	ld		($fe1b),a
	ld		a,($fe1f)			; tile 7
	res		5,a
	ld		($fe1f),a
	
	; swap tiles
	ld		a,0					; 0 & 1
	ld		($fe02),a
	ld		a,1
	ld		($fe06),a
	
	ld		a,2					; 2 & 3
	ld		($fe0a),a
	ld		a,3
	ld		($fe0e),a


	ld		a,4					; 4 & 5
	ld		($fe12),a
	ld		a,5
	ld		($fe16),a

	ld		a,6					; 6 & 7
	ld		($fe1a),a
	ld		a,7
	ld		($fe1e),a

	
	jr		_done_pflip

_flip_left
	ld		a,(brface)			; check if already facing left
	cp		0
	jr		z,_done_pflip
	
	xor		a					; set to facing left
	ld		(brface),a
	
	ld		a,($fe03)			; tile 0
	set		5,a
	ld		($fe03),a
	ld		a,($fe07)			; tile 1
	set		5,a
	ld		($fe07),a
	ld		a,($fe0b)			; tile 2
	set		5,a
	ld		($fe0b),a
	ld		a,($fe0f)			; tile 3
	set		5,a
	ld		($fe0f),a
	ld		a,($fe13)			; tile 4
	set		5,a
	ld		($fe13),a
	ld		a,($fe17)			; tile 5
	set		5,a
	ld		($fe17),a
	ld		a,($fe1b)			; tile 6
	set		5,a
	ld		($fe1b),a
	ld		a,($fe1f)			; tile 7
	set		5,a
	ld		($fe1f),a

	; swap tiles
	ld		a,1					; 0 & 1
	ld		($fe02),a
	ld		a,0
	ld		($fe06),a
	
	ld		a,3					; 2 & 3
	ld		($fe0a),a
	ld		a,2
	ld		($fe0e),a

	ld		a,5					; 4 & 5
	ld		($fe12),a
	ld		a,4
	ld		($fe16),a

	ld		a,7					; 6 & 7
	ld		($fe1a),a
	ld		a,6
	ld		($fe1e),a

	
_done_pflip
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

	dec		a
	ld		(scx),a
	
	ld		hl,objects
	inc		hl
	ld		b,(hl)
	ld		hl,$fe21		; starting tile's xpos
	ld		de,$04

__loop_sl					; for each obj tile
	xor		a
	cp		b
	jr		z,__loop_sl_done
	
	ld		a,(hl)
	inc		a
	ld		(hl),a
	add		hl,de
	
	dec		b
	jr		__loop_sl
__loop_sl_done
	
	dec		c
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

	inc		a
	ld		(scx),a

	ld		hl,objects
	inc		hl
	ld		b,(hl)
	ld		hl,$fe21		; starting tile's xpos
	ld		de,$04

__loop_sr					; for each obj tile
	xor		a
	cp		b
	jr		z,__loop_sr_done
	
	ld		a,(hl)
	dec		a
	ld		(hl),a
	add		hl,de
	
	dec		b
	jr		__loop_sr
__loop_sr_done

	dec		c
	jr		_loop_sr

done_scrrt	
	ret
