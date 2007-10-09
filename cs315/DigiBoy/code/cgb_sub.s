;===============================================================
;	Game Boy Subroutine
;
;	Nintendo Software Technology Corporation
;===============================================================
;***************************************************************
;
;              Name: cont
;
;       Description: Key Input
;
;        Parameters: 
;		
;			 Output:	cnt1 --	holding key
;						trg1 -- trigger key
;
;    Registers Used:	a,b,c
;
;	+-------------------------------+
;	|								|
;	|		7						|
;	|	6		5	3	4	2	1	|
;	|		8						|
;	|								|
;	+-------------------------------+
;***************************************************************
;
cont
;
	ld		a,$20			; load 00100000 into a (read up down left right keys)
	ld		(p1),a			; set the bit to read the directional pad
	ld		a,(p1)			; load content of p1 into a (load which direction is pressed)
	ld		a,(p1)			; and again to make sure 
	cpl						; take ones complement of what keys are pressed (bits 0 - 3)
	and		$0f				; a & 00001111 where a contains bits 0 - 3 
	swap	a				; swap the bits into the high nibble of a 
	ld		b,a				; load a into b 
	ld		a,$10			; load 00010000 into a (read a b select start)
	ld		(p1),a			; set the bit to read the buttons (read select start a b)
	ld		a,(p1)			; load into a the bits for the buttons 
	ld		a,(p1)			; and again
	ld		a,(p1)			; and again
	ld		a,(p1)			; and again
	ld		a,(p1)			; and again
	ld		a,(p1)			; and again ... now we are sure 
	cpl						; take the ones complement of a 
	and		$0f				; again we and it with 00001111
	or		b				; a | b (populate b with buttons ie dPad = high buttons = low)
	ld		c,a				; load the controller's status into c
	ld		a,(cnt1)		; load contents of cnt1 into a 
	xor		c				; controller's state xor with contents of cnt1
	and		c				; and the result with the controller's state
	ld		(trg1),a		; load contents of a into contents of trg1 (one time press)
	ld		a,c				; load our controller's state into a		 
	ld		(cnt1),a		; load controller's state into the contents of cnt1 (constant press)
	ld		a,$30			; load in 00110000 into a 
	ld		(p1),a			; load a into contents of p1 (resets control register)

	ret			


;***************************************************************
;
;              Name: idjmp
;
;       Description: program jump routine
;
;        Parameters: a -- id number
;
;    Registers Used: a,de,hl,stack
;
;***************************************************************
idjmp
;
	ld		l,a
	ld		h,0
	add		hl,hl
	ld		e,l
	ld		d,h
;
	pop		hl
	add		hl,de
	ld		a,(hli)	
	ld		h,(hl)
	ld		l,a
	jp		(hl)
;
;---------------------------------------------------------------
;	name:		lcdc_off
;	function:	turn off lcd controller
;	entry:		None
;	return:		None
;	register:	a,ieb
;---------------------------------------------------------------
;
lcdc_off
;
	ld		a,(lcdc)
	bit		7,a
	ret		z			; lcdc off

	ld		a,(ie)
	ld		(ieb),a		; save interupt flag
	res		0,a			; disable v-blank interupte
	ld		(ie),a
_loop
	ld		a,(ly)		; wait for v-blank
	cp		145
	jr		nz,_loop

	ld		a,(lcdc)	; turn off lcd controller
	and		%01111111
	ld		(lcdc),a

	xor		a
	ld		(if),a
	ld		a,(ieb)
	ld		(ie),a		; restore interupt flag

	ret


;***************************************************************
;
;              Name: data_mov
;
;       Description: move data
;
;        Parameters: de -- destination address
;					 bc -- data size
;					 hl -- source
;
;    Registers Used: a,bc,hl,de
;
;***************************************************************
data_mov
;
	ld		a,(hli)
	ld		(de),a
	inc		de
	dec		bc
	ld		a,b
	or		c
	jr		nz,data_mov

	ret


;***************************************************************
;
;              Name: obj_cls
;
;       Description: obj ram buffer clear
;
;        Parameters:
;
;    Registers Used: a
;
;***************************************************************
;
obj_cls
;
	ld		b,$a0		; clear OBJ RAM
	xor		a
	ld		hl,objram

_cls00
	ld		(hli),a
	dec		b
	jr		nz,_cls00

	ret



;***************************************************************
;
;              Name: memset
;
;       Description: set memeory as input value
;
;        Parameters: a -- the value need to be set
;					 bc -- data size
;					 hl-- memory address
;
;    Registers Used: a,b,c,hl,e
;
;***************************************************************
;
memset
;
	ld	(hli),a
	dec	bc
	ld	e,a
	ld	a,c
	or	b
	ld	a,e
	jr	nz,memset

	ret

;---------------------------------------------------------------
;	name:		cgb_vram_clear
;	function:	clear v-ram ($8000 - $9FFF)
;	entry:		None
;	return:		None
;	register:	a,hl,bc
;---------------------------------------------------------------
;
cgb_vram_all_clear 
;
	ld		a,$01
	ld		(vbk),a		;vrambank = 1
	xor		a
	ld		hl,$8000
	ld		bc,$2000
	call	memset

	xor		a
	ld		(vbk),a		;vrambank = 0
	ld		hl,$8000
	ld		bc,$2000
	call	memset

	ret

;---------------------------------------------------------------
;	name:		cgb_screen_clear
;	function:	clear screen v-ram ($9800 - $9BFF)
;	entry:		a -- space tile number
;	return:		None
;	register:	a,hl,bc
;---------------------------------------------------------------
;
cgb_screen_clear
;
	push	af

	ld		a,$01
	ld		(vbk),a		;screen attribute v-ram
	ld		hl,$9800
	ld		bc,$400
	xor		a			;clear the v-ram in $00
	call	memset

	xor		a
	ld		(vbk),a		;screen data v-ram
	ld		hl,$9800
	ld		bc,$400
	pop		af			;clear the v-ram in space tile data
	call	memset

	ret

;---------------------------------------------------------------
;	name:		bg_palette_set
;	function:	set all the background palette
;	entry:		hl -- address of background palette data
;	return:		None
;	register:	a,hl,bc
;---------------------------------------------------------------
;
bg_palette_set
;
	ld		a,%10000000
	ld		(bcps),a	;bit7:(1:autoincrement),543:palletno,21:palletdatano,0:(h:1,l:0)
	ld		b,$40		;2(lh)*4(palette data number)*8(palette number)=64=$40
	ld		c,<bcpd
_loop 
	ld		a,(hli)
	ld		(c),a
	dec		b
	jr		nz,_loop

	ret

;---------------------------------------------------------------
;	name:		obj_palette_set
;	function:	set all the object palette
;	entry:		hl -- address of object palette data
;	return:		None
;	register:	a,hl,bc
;---------------------------------------------------------------
;
obj_palette_set
	ld		a,%10000000
	ld		(ocps),a
	ld		b,$40
	ld		c,<ocpd
_loop 
	ld		a,(hli)
	ld		(c),a
	dec		b
	jr		nz,_loop

	ret