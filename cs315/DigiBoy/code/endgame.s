;---------------------------------------
;			End Game Update
;---------------------------------------

endgame_proc

	call	cont
	ld		a,(cnt1)
	
	bit		BitStart,a
	jr		z,_done_endgame
	
	ld		a,(mstory)
	call	reset_to

_done_endgame
	ret


;---------------------------------------
;
;			Game Init
;
;---------------------------------------

endgame_init

	call	clear_objs				; clear objects

	xor		a						; Bank 0
	ld		(vbk),a

	ld		hl,story_tile_data		; Load Story tile set
	ld		de,$9000
	ld		bc,$130					; Load tiles
	call	data_mov

	ld		a,1						; Switch to Bank 1
	ld		(vbk),a

	ld		hl,endgame_attr_data	; Load End-Game Attributes
	ld		de,$9800
	ld		bc,$0400
	call	data_mov

	xor		a						; Switch to Bank 0
	ld		(vbk),a

	ld		hl,endgame_scr_data		; Load End-Game Screen Data
	ld		de,$9800
	ld		bc,$0400
	call	data_mov

	ld		hl,story_pal_data		; Load Story Palette
	call	bg_palette_set

	xor		a						; Reset screen pos
	ld		(scx),a
	ld		(scy),a
	
	ret
