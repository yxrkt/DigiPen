;---------------------------------------
;			Game Update
;---------------------------------------

game_proc

	call	cont
	ld		a,(cnt1)
	
	bit		BitLeft,a					; Check d-pad left
	jr		z,check_right
	xor		a
	ld		b,a
	jr		movepl_
	
check_right								; Check d-pad right
	bit		BitRight,a
	jr		z,done_gp
	ld		a,1
	ld		b,a

movepl_
	ld		a,1							; Walk left or right
	ld		d,a
	call	moveh
	
done_gp
	call	movev						; Jumping and falling

	call	update_ai
	call	update_physics

	ret


;---------------------------------------
;
;			Game Init
;
;---------------------------------------

game_init

	xor		a						; Bank 0
	ld		(vbk),a

	ld		hl,game_tile_data		; Load Game tile set
	ld		de,$9000
	ld		bc,$130					; Load tiles
	call	data_mov

	ld		a,1						; Switch to Bank 1
	ld		(vbk),a

	ld		hl,game_attr_data		; Load Game Attributes
	ld		de,$9800
	ld		bc,$0400
	call	data_mov

	xor		a						; Switch to Bank 0
	ld		(vbk),a

	ld		hl,game_scr_data		; Load Game Screen Data
	ld		de,$9800
	ld		bc,$0400
	call	data_mov

	ld		hl,game_pal_data		; Load Game Palette
	call	bg_palette_set

	xor		a
	ld		(scx),a
	ld		(scy),a
	
	ld		(height),a				; initialize variables
	ld		(bjump),a
	ld		(atimer),a
	ld		(baiface),a
	ld		(bfire),a
	
	ld		a,1
	ld		(brface),a
	
	call	load_obj_data			; load object tileset and palettes
	call	load_objs				; load objects
	call	load_phys_objs			; load physics properties for objs

	ret
