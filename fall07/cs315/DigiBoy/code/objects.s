;---------------------------------------
;	Load objects
;	params: none
;---------------------------------------
load_obj_data

	ld		hl,objs_tile_data		; Load Object tile set
	ld		de,$8000
	ld		bc,$140					; Load object tiles
	call	data_mov

	ld		hl,objs_pal_data		; Load Object Palette
	call	obj_palette_set

	ret

load_objs

	; Player Object
	
	; tile 0
	ld		a,$70					; y pos
	ld		($fe00),a
	
	ld		a,$08					; x pos
	ld		($fe01),a
	
	xor		a						; index of tile
	ld		($fe02),a
	
	ld		a,%00000000				; index of palette
	ld		($fe03),a
	
	; tile 1
	ld		a,$70					; y pos
	ld		($fe04),a
	
	ld		a,$10					; x pos
	ld		($fe05),a
	
	ld		a,1						; index of tile
	ld		($fe06),a
	
	ld		a,%00000000				; index of palette
	ld		($fe07),a
	
	; tile 2
	ld		a,$78					; y pos
	ld		($fe08),a
	
	ld		a,8						; x pos
	ld		($fe09),a
	
	ld		a,2						; index of tile
	ld		($fe0a),a
	
	ld		a,%00000001				; index of palette
	ld		($fe0b),a
	
	; tile 3
	ld		a,$78					; y pos
	ld		($fe0c),a
	
	ld		a,$10					; x pos
	ld		($fe0d),a
	
	ld		a,3						; index of tile
	ld		($fe0e),a
	
	ld		a,%00000001				; index of palette
	ld		($fe0f),a
	
	; tile 4
	ld		a,$80					; y pos
	ld		($fe10),a
	
	ld		a,8						; x pos
	ld		($fe11),a
	
	ld		a,4						; index of tile
	ld		($fe12),a
	
	ld		a,%00000001				; index of palette
	ld		($fe13),a
	
	; tile 5
	ld		a,$80					; y pos
	ld		($fe14),a
	
	ld		a,$10					; x pos
	ld		($fe15),a
	
	ld		a,5						; index of tile
	ld		($fe16),a
	
	ld		a,%00000001				; index of palette
	ld		($fe17),a
	
	; tile 6
	ld		a,$88					; y pos
	ld		($fe18),a
	
	ld		a,8						; x pos
	ld		($fe19),a
	
	ld		a,6						; index of tile
	ld		($fe1a),a
	
	ld		a,%00000001				; index of palette
	ld		($fe1b),a
	
	; tile 7
	ld		a,$88					; y pos
	ld		($fe1c),a
	
	ld		a,$10					; x pos
	ld		($fe1d),a
	
	ld		a,7						; index of tile
	ld		($fe1e),a
	
	ld		a,%00000001				; index of palette
	ld		($fe1f),a
	
	; Beast Object
	
	; tile 9
	ld		a,$80					; y pos
	ld		($fe20),a
	
	ld		a,$70					; x pos
	ld		($fe21),a
	
	ld		a,9						; index of tile
	ld		($fe22),a
	
	ld		a,%00000010				; index of palette
	ld		($fe23),a
	
	; tile 10
	ld		a,$80					; y pos
	ld		($fe24),a
	
	ld		a,$78					; x pos
	ld		($fe25),a
	
	ld		a,10					; index of tile
	ld		($fe26),a
	
	ld		a,%00000010				; index of palette
	ld		($fe27),a	
	
	; tile 11
	ld		a,$88					; y pos
	ld		($fe28),a
	
	ld		a,$70					; x pos
	ld		($fe29),a
	
	ld		a,11					; index of tile
	ld		($fe2a),a
	
	ld		a,%00000010				; index of palette
	ld		($fe2b),a
	
	; tile 12
	ld		a,$88					; y pos
	ld		($fe2c),a
	
	ld		a,$78					; x pos
	ld		($fe2d),a
	
	ld		a,12					; index of tile
	ld		($fe2e),a
	
	ld		a,%00000010				; index of palette
	ld		($fe2f),a
	
	; Key Object
	
	; tile 17
	ld		a,$88					; y pos
	ld		($fe30),a
	
	ld		a,$c8					; x pos
	ld		($fe31),a
	
	ld		a,17					; index of tile
	ld		($fe32),a
	
	ld		a,%00000011				; index of palette
	ld		($fe33),a
	
	; Fireball Object takes $fe34 - $fe37
	
	ret
