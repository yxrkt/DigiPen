;---------------------------------------
;			Story Update
;---------------------------------------
story_proc
	; Check user input and etc.
	
	; Check if scrolling bool is true
	xor		a
	ld		hl,bscroll
	cp		(hl)
	jr		nz,scroll_sp
	
	; If button is hit
	call	btn_hit
	cp		0
	jr		z,done_sp

	ld		(bscroll),a					; Set scrolling bool to true


scroll_sp

	; Call scroll down subroutine
	ld		a,2
	call	scroll_down

	; Check if scrolling has stopped
	ld		a,111
	ld		hl,scy
	cp		(hl)
	jr		nz,done_sp

	; Check if A is hit
	call	btn_hit
	cp		0
	jr		z,done_sp

	; Check if on 2nd page of story
	xor		a
	ld		hl,bpart2
	cp		(hl)
	jr		nz,call_game

	; Call second part of story
	call	story_cont
	xor		a
	ld		(bscroll),a					; Set scrolling bool to false
	jr		done_sp

call_game

	; Call game init
	ld		a,(mgame)
	ld		(mode),a
	call	game_init
	jr		done_sp
	
done_sp
	ret

;---------------------------------------
;
;			Story Init
;
;---------------------------------------

story_init

	xor		a						; Bank 0
	ld		(vbk),a

	ld		hl,story_tile_data		; Load Story tile set
	ld		de,$9000
	ld		bc,$240					; Load tiles
	call	data_mov

	ld		a,1						; Switch to Bank 1
	ld		(vbk),a

	ld		hl,story_attr_data		; Load Story Attributes
	ld		de,$9800
	ld		bc,$0400
	call	data_mov

	xor		a						; Switch to Bank 0
	ld		(vbk),a

	ld		hl,story_scr_data		; Load Story Screen Data
	ld		de,$9800
	ld		bc,$0400
	call	data_mov

	ld		hl,story_pal_data		; Load Story Palette
	call	bg_palette_set
	
	ld		a,$33
	ld		(scx),a
	ld		a,$50
	ld		(scy),a

	ret

;---------------------------------------
;				Page 2
;---------------------------------------

story_cont

	xor		a						; Bank 0
	ld		(vbk),a

	ld		hl,story2_scr_data		; Load 2nd Story Screen Data
	ld		de,$9800
	ld		bc,$0400
	call	data_mov
	
	xor		a
	ld		(scy),a
	
	ld		a,1
	ld		(bpart2),a

	ret
