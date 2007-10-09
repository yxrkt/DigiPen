;---------------------------------------
;			Menu Main
;---------------------------------------
menu_proc
	; Check user input and etc.
	
	call	cont
	ld		a,(cnt1)
	
	bit		BitStart,a
	jr		z,done_mp
	ld		a,(mstory)
	ld		(mode),a
	call	story_init
	
	xor		a
	ld		(scx),a
	ld		(scy),a

done_mp
	ret


;---------------------------------------
;
;				Menu Init
;
;---------------------------------------
menu_init

	xor		a						; Bank 0
	ld		(vbk),a

	ld		hl,menu_tile_data		; Load Menu tile set
	ld		de,$9000
	ld		bc,$170					; Load tiles
	call	data_mov

	ld		a,1						; Switch to Bank 1
	ld		(vbk),a

	ld		hl,menu_attr_data		; Load Menu Attributes
	ld		de,$9800
	ld		bc,$0400
	call	data_mov

	xor		a						; Switch to Bank 0
	ld		(vbk),a

	ld		hl,menu_scr_data		; Load Menu Screen Data
	ld		de,$9800
	ld		bc,$0400
	call	data_mov

	ld		hl,menu_pal_data		; Load Menu Palette
	call	bg_palette_set
	
	ld		a,$33
	ld		(scx),a
	ld		a,$50
	ld		(scy),a

	ret
