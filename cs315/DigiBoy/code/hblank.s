;****************************************************************
;*		h-blank interrupt Service Routine (ISR)					*
;****************************************************************

h_blank
	push	af
	
	ld		a,167
	ld		(wx),a

	ld		a,1
	ld		(hblank_f),a

	pop		af

	reti

