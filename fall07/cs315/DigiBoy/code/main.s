;----------------------------------------------------------------
;*																*
;*		Game boy (CGB) program									*
;----------------------------------------------------------------
;----------------------------------------------------------------
;*		interrupt jump routine									*
;----------------------------------------------------------------
;
	org		$00
	jp		idjmp		; rst	0
;
;--------------------------- v-blank interrupt ----------------------
;
	org		$40
	jp		v_blank
;
;------------------------- lcdc status interrupt --------------------
;
	org		$48
	jp		h_blank
;
;--------------------------- timer interrupt ------------------------
;
	org		$50
	reti
;
;--------------------------- sio interrupt --------------------------
;
	org		$58
	reti
;
;------------------------ key input interrupt -----------------------
;
	org		$60
	reti

;
;****************************************************************
;		program start
;****************************************************************
;
	org		$150
;
;Power on
;hard_reset 

	ld		sp,stackpointer ;$ffff

	call	lcdc_off		; turn off LCD

	di

;control register clear
	xor		a
	ld		(if),a
	ld		(ie),a
	ld		(scy),a
	ld		(scx),a 
	ld		(stat),a

;cgb register clear
	xor		a
	ld		(vbk),a 		;vrambank=0
	ld		(svbk),a		;workrambank=0	
	
;;Clear up vram		
	call	cgb_vram_all_clear	

;;Clear up OAM
	ld		b,$a0			
	ld		a,00
	ld		hl,objram
_loop 
	ld		(hli),a
	dec		b
	jr		nz,_loop

;;Clear registers
	xor		a
	ld		(if),a

	ld		a,$01
	ld		(ie),a

;; Menu initialization
;; load tile, palette, index & attribute data
	call	menu_init

	ld		a,%10000011			; Set up and turn on LCD display
	ld		(lcdc),a
		
	ei							; Enable interrupts
	
	ld		a,(ie)				; Enable lcdc interrupts
	set		1,a
	ld		(ie),a	
	
	ld		a,(mmenu)
	ld		(mode),a

;***** main loop *****
mainloop
	call	update_proc

	call	_wait_vb
	
	jr		mainloop

_wait_vb
	ld		a,(vblank_f)
	and		a
	jr		z,_wait_vb

	xor		a
	ld		(vblank_f),a

	ret

