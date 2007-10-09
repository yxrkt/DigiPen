;---------------------------------------
;	Load objects
;	params: none
;---------------------------------------
load_obj_data

	ld		hl,objs_tile_data		; Load Object tile set
	ld		de,$8000
	ld		bc,$40					; Load objects
	call	data_mov

	ld		hl,objs_pal_data		; Load Object Palette
	call	obj_palette_set

	ret

load_objs

	; Player Object
	
	; tile 0
	ld		a,$82					; y pos
	ld		($fe00),a
	
	ld		a,8						; x pos
	ld		($fe01),a
	
	xor		a						; index of tile
	ld		($fe02),a
	
	ld		a,%00000000				; index of palette
	ld		($fe03),a
	
	; tile 1
	ld		a,$82					; y pos
	ld		($fe04),a
	
	ld		a,$10					; x pos
	ld		($fe05),a
	
	ld		a,1						; index of tile
	ld		($fe06),a
	
	ld		a,%00000000				; index of palette
	ld		($fe07),a
	
	; tile 2
	ld		a,$8a					; y pos
	ld		($fe08),a
	
	ld		a,8						; x pos
	ld		($fe09),a
	
	ld		a,2						; index of tile
	ld		($fe0a),a
	
	ld		a,%00000000				; index of palette
	ld		($fe0b),a
	
	; tile 3
	ld		a,$8a					; y pos
	ld		($fe0c),a
	
	ld		a,$10					; x pos
	ld		($fe0d),a
	
	ld		a,3						; index of tile
	ld		($fe0e),a
	
	ld		a,%00000000				; index of palette
	ld		($fe0f),a	

	ret
