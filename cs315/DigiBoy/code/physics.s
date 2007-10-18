;---------------------------------------
;
;	Physics Engine
;
;---------------------------------------

update_physics
	call	check_vs_objects
	ld		a,$ff
	cp		b
	jr		z,_done_upphys
	
	ld		a,1					; if hitting key
	cp		b
	jr		nz,_phys_not_key
	
	ld		a,(mend)
	ld		(mode),a
	call	endgame_init
	jr		_done_upphys
	
_phys_not_key
	call	dec_lives
	ld		a,(nlives)
	cp		0
	jr		z,_done_upphys

	ld		a,(mgame)
	call	reset_to

_done_upphys
	ret

;---------------------------------------
;	Manages jumping and falling for player
;	params: d -- speed of jump
;---------------------------------------
update_vpos
	ld		a,(height)		; if standing
	cp		0
	jr		nz,_cont_upvpos

	call	cont
	ld		a,(trg1)
	bit		BitA,a
	jr		z,_done_upvpos
	
	ld		a,1				; set jump to true
	ld		(bjump),a
	
_cont_upvpos
	call	check_vs_terrain
	jr		z,_land_upvpos
	ld		a,(bjump)

_land_upvpos
	xor		a
	ld		(height),a

_done_upvpos
	ret

;---------------------------------------
;	Checks terrain collision
;	output: e -- L[1] R[2] U[4] D[8]
;	registers used: a,c,hl
;---------------------------------------
check_vs_terrain
	ld		hl,terrain
	ld		a,(hli)
	ld		c,a
	
_loop_chkter
	xor		a
	cp		c
	jr		z,_done_chkter

_loop_chkter_end	
	dec		c
	jr		_loop_chkter

_done_chkter
	ret

;---------------------------------------
;	Checks object collision
;	output: b -- index of obj hit
;	registers used: a,b,c,d,e,hl
;---------------------------------------
check_vs_objects

	call	load_phys_objs			; get object info

	ld		hl,objects
	ld		a,(hli)
	ld		c,a
	ld		b,a
	
	inc		hl						; skip total tiles
	
_loop_chkobjs
	xor		a
	cp		c
	jr		z,_no_hit

	push	hl
	
_check_left

	ld		a,(hli)
	ld		d,a
	ld		a,($fe01)
	add		a,$10
	cp		d						; if obj left < p right
	jr		c,_loop_chkobjs_end

_check_right
	sub		$10
	ld		e,a
	ld		a,(hli)
	add		a,d
	cp		e						; if obj right > p left
	jr		c,_loop_chkobjs_end
	
_check_top
	ld		a,(hli)
	ld		d,a
	ld		a,($fe00)
	add		a,$20
	cp		d						; if obj top < p bottom
	jr		c,_loop_chkobjs_end
	
_check_bottom	
	sub		$20
	ld		e,a
	ld		a,(hli)
	add		a,d
	cp		e						; if obj bottom > p top
	jr		c,_loop_chkobjs_end
	
	ld		a,b						; set object index
	sub		c
	ld		b,a
	pop		hl
	jr		_done_chkobjs

_loop_chkobjs_end
	pop		hl
	ld		de,$04
	add		hl,de
	dec		c
	jr		_loop_chkobjs

_no_hit
	ld		a,$ff
	ld		b,a

_done_chkobjs
	ret

;---------------------------------------
;	Load physics objects
;	registers used: a,hl
;---------------------------------------
load_phys_objs

	;; Load entity objects
	ld		hl,objects			; set hl to point to objects
	ld		a,3					; number of entity objects
	ld		(hli),a
	ld		a,6					; total no. of tiles
	ld		(hli),a
	
	; Load beast object (4 tiles)
	ld		a,($fe21)			; xpos
	ld		(hli),a
	ld		a,$10				; width
	ld		(hli),a
	ld		a,($fe20)			; ypos
	ld		(hli),a
	ld		a,$10				; height
	ld		(hli),a
	
	; Load key object (1 tile)
	ld		a,($fe31)			; xpos
	ld		(hli),a
	ld		a,$08				; width
	ld		(hli),a
	ld		a,($fe30)			; ypos
	ld		(hli),a
	ld		a,$08				; height
	ld		(hli),a
	
	; Load fireball object (1 tile)
	ld		a,($fe35)			; xpos
	ld		(hli),a
	ld		a,$08				; width
	ld		(hli),a
	ld		a,($fe34)			; ypos
	ld		(hli),a
	ld		a,$08				; height
	ld		(hli),a

	ret
	
;---------------------------------------
;	Update positions of objects
;	registers used: a,b,c,de,hl
;---------------------------------------
update_objs_pos

	ld		a,(hli)		; no. of objects
	ld		c,a

_loop_upobjpos			; FOR EACH OBJECT
	xor		a
	cp		c
	jr		z,_update_objs_pos_end
	
	ld		a,(hli)		; no. of tiles
	ld		b,a
	ld		a,(hli)		; xpos
	ld		d,a
	inc		hl
	ld		a,(hli)		; ypos
	ld		e,a
	inc		hl
	
	push	hl
	ld		hl,$fe20
	
__loop_upobjpos			; FOR EACH TILE
	xor		a
	cp		b
	jr		z,__loop_upobjpos_end
	
	ld		a,(hl)
	
	dec		b
	jr		__loop_upobjpos
__loop_upobjpos_end

	dec		c
	jr		_loop_upobjpos

_update_objs_pos_end	
	ret