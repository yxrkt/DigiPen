;********************************************************************
;*		      <<<<<    ram equ	 >>>>>								*
;********************************************************************
;
	org	$ff8a
;
cnt1		ds	1	; button status
trg1		ds	1	; triger input (controller)
vblank_f	ds	1	; v-blank end flag
hblank_f	ds	1	; h-blank end flag
ieb			ds	1	; interrupt flag

	end
