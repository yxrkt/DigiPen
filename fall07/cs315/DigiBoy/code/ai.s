;---------------------------------------
;	AI Subroutines
;---------------------------------------

;---------------------------------------
;	Move the beast left to right
;---------------------------------------
update_ai

	ld		hl,$fe21
	ld		de,$04

	ld		a,(baiface)
	cp		0
	jr		z,_ai_movl	

_ai_movr
	ld		a,(scx)			; check if at right bound
	ld		b,a
	ld		a,(beastrb)
	sub		b
	ld		b,a
	ld		a,(hl)
	cp		b
	jr		z,_ai_switch_dir

	ld		a,4
	ld		c,a
_loop_shiftr
	xor		a
	cp		c
	jr		z,_loop_shiftr_end
	
	ld		a,(hl)
	inc		a
	ld		(hl),a
	add		hl,de
	
	dec		c
	jr		_loop_shiftr
_loop_shiftr_end
	
	jr		_upaimov_done
	
_ai_movl
	ld		a,(scx)			; check if at left bound
	ld		b,a
	ld		a,(beastlb)
	sub		b
	ld		b,a
	ld		a,(hl)
	cp		b
	jr		z,_ai_switch_dir0
	
	ld		a,4
	ld		c,a
_loop_shiftl
	xor		a
	cp		c
	jr		z,_loop_shiftl_end
	
	ld		a,(hl)
	dec		a
	ld		(hl),a
	add		hl,de
	
	dec		c
	jr		_loop_shiftl
_loop_shiftl_end

	jr		_upaimov_done

_ai_switch_dir0
	call	spit_fire
	
_ai_switch_dir
	call	flip_beast

_upaimov_done
	call	update_fire
	ret

;---------------------------------------
;	Update fire
;---------------------------------------
update_fire

	ld		a,(bfire)
	cp		0
	jr		z,_upfire_done
	
	ld		hl,$fe35
	ld		a,(hl)
	dec		a
	ld		(hl),a
	
	ld		b,a
	ld		a,(scx)
	add		a,b
	cp		8
	jr		nz,_upfire_done
	
	xor		a
	ld		(bfire),a
	
	ld		a,19
	ld		($fe36),a
	
_upfire_done
	ret

;---------------------------------------
;	Spawn a fireball
;---------------------------------------
spit_fire

	ld		hl,$fe34

	; Fireball Object
	ld		a,($fe28)				; y pos
	ld		(hli),a
	
	ld		a,($fe29)				; x pos
	ld		(hli),a
	
	ld		a,18					; index of tile
	ld		(hli),a
	
	ld		a,%00000100				; index of palette
	ld		(hl),a
	
	ld		a,(baiface)
	cp		0
	jr		z,_spit_fire_done
	
	set		5,(hl)

_spit_fire_done
	ld		a,1
	ld		(bfire),a
	ret

;---------------------------------------
;	Flip the beast horizontally
;---------------------------------------
flip_beast

	ld		hl,$fe23
	ld		de,$04

	ld		a,(baiface)
	cp		0
	jr		z,_flip_ai_right

_flip_ai_left
	ld		a,9
	ld		($fe22),a
	res		5,(hl)
	add		hl,de
	
	ld		a,10
	ld		($fe26),a
	res		5,(hl)
	add		hl,de
	
	ld		a,11
	ld		($fe2a),a
	res		5,(hl)
	add		hl,de
	
	ld		a,12
	ld		($fe2e),a
	res		5,(hl)
	add		hl,de
	
	jr		_flip_beast_done

_flip_ai_right
	ld		a,10
	ld		($fe22),a
	set		5,(hl)
	add		hl,de
	
	ld		a,9
	ld		($fe26),a
	set		5,(hl)
	add		hl,de
	
	ld		a,12
	ld		($fe2a),a
	set		5,(hl)
	add		hl,de
	
	ld		a,11
	ld		($fe2e),a
	set		5,(hl)
	add		hl,de
	
_flip_beast_done
	ld		a,(baiface)
	xor		1
	ld		(baiface),a

	ret
