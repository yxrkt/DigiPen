;****************************************************************
;*		v-blank interrupt Service Routine (ISR)					*
;****************************************************************

v_blank
	push	af

	ld		a,1
	ld		(vblank_f),a

	pop		af

	reti

