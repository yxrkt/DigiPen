;---------------------------------------
;			Lose Screen
;---------------------------------------

lose_proc

	call	cont
	ld		a,(cnt1)
	
	bit		BitStart,a
	jr		z,_done_lose
	
	ld		a,(mstory)
	call	reset_to

_done_lose
	ret


;---------------------------------------
;
;			Lose Screen Init
;
;---------------------------------------

lose_init

	call	clear_objs				; clear objects

	xor		a						; Bank 0
	ld		(vbk),a

	ld		hl,menu_tile_data		; Load Menu tile set
	ld		de,$9000
	ld		bc,$180					; Load tiles
	call	data_mov

	ld		a,1						; Switch to Bank 1
	ld		(vbk),a

	ld		hl,lose_attr_data		; Load Lose Attributes
	ld		de,$9800
	ld		bc,$0400
	call	data_mov

	xor		a						; Switch to Bank 0
	ld		(vbk),a

	ld		hl,lose_scr_data		; Load End-Game Screen Data
	ld		de,$9800
	ld		bc,$0400
	call	data_mov

	ld		hl,menu_pal_data		; Load Story Palette
	call	bg_palette_set

	xor		a						; Reset screen pos
	ld		(scx),a
	ld		(scy),a
	
	ret
