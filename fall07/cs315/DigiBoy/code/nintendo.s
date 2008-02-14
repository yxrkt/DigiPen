;----------------------------------------------------------------
;*																*
;*		 Game Pak Registration Data    							*
;*		    as of June 25, 1998		  							*
;* 		        												*
;----------------------------------------------------------------

	org		$100

;------------------------- STARTING VECTOR ----------------------
	nop					; $00
	jp		$150		; $C3,$50,$01
						; USER PROGRAM START ADDRESS


;------------------ NINTENDO LOGO CHARACTER DATA ----------------

;104H~
	defb	$CE,$ED,$66,$66
	defb	$CC,$0D,$00,$0B
	defb	$03,$73,$00,$83
	defb	$00,$0C,$00,$0D
	defb	$00,$08,$11,$1F
	defb	$88,$89,$00,$0E
	defb	$DC,$CC,$6E,$E6
	defb	$DD,$DD,$D9,$99
	defb	$BB,$BB,$67,$63
	defb	$6E,$0E,$EC,$CC
	defb	$DD,$DC,$99,$9F
	defb	$BB,$B9,$33,$3E

;       Title and Game registration area

;--------------  GAME TITLE (15 BYTES TOTAL) -------------------
; Note:  If letters are surrounded by ', then the will become 
;	 ASCII Characters
;134H~
;	defb	'Flash RAM Clear'	; 15 BYTES
	defb	'BIONIC-COMMANDO'	; 15 BYTES

;	defb	$00,$00,$00,$00		; ‚l‚`‚w‚P‚UƒoƒCƒg
;	defb	$00,$00,$00,$00		;Maximun 16 Bytes
;	defb	$00,$00,$00,$00
;	defb	$00,$00,$00

;---------------------=  CGB CODE ------------------------------
; The game code used to be 16 Bytes, but this has changed for 
; CGB.  Prior games contained 00H or ASCII code.  The CGB code 
; is 80H.
;143H
	defb	$80			; 80: CGB, 00(or other ASCII): Not CGB
;	defb	$00			; 80: CGB, 00(or other ASCII): Not CGB

;----------------- Maker Code (ASCII CODE) ---------------------
;144H~
	defb	$00,$00		; 0,0: null(These are ASCII CODE,
						; and '0','0' doesn't actually exsist)

;----------------  SUPER GAMEBOY FUNCTION ----------------------
;146H
	DEFB	$00			; 3: Uses SGB functions
						; 0: GAMEBOY(No SUPER GAMEBOY)

;---------------------- GAME PAK TYPE --------------------------
;147H (Caution:  You must use MBC5 with 2x speed mode - 06/01/98)
	defb	$1A	; 0:ONLY ROM.1:ROM+MBC1,2:ROM+MBC1+SRAM
;	defb	$00	; 0:ONLY ROM.1:ROM+MBC1,2:ROM+MBC1+SRAM
			; 3:ROM+MBC1+SRAM(Battery),5:ROM+MBC2
			; 6:ROM+MBC2(Battery),8:ROM+SRAM,9:ROM+SRAM(Battery)
			; B:ROM+MMM01,C:ROM+MMM01+SRAM,D:ROM+MMM01+SRAM(Battery)
			; F:ROM+MBC3(Timer,Battery),10:ROM+MBC3(Timer)+SRAM(Battery)
			; 11:ROM+MBC3,12:ROM+MBC3+SRAM,13:ROM+MBC3+SRAM(Battery)
			; 15:ROM+MBC4,16:ROM+MBC4+SRAM,17:ROM+MBC4+SRAM(Battery)
			; 19:ROM+MBC5,1A:ROM+MBC5+SRAM,1B:ROM+MBC5+SRAM(Battery)
			; 1C:ROM+MBC5(Rumble),1D:ROM+MBC5(Rumble)+SRAM,1E:ROM+MBC5(Rumble)+SRAM(Battery)
			; Don't care about parts structure listed below
			; FC: Pocket Camera, FD: Bandai TAMA5
			; FE: Hudson HuC-3, FF: Hudson HuC-1

;------------------------ ROM SIZE -----------------------------
;148H
	defb	$00			; 0: 256Kb, 1: 512Kb, 2: 1Mb
						; 3: 2Mb, 4: 4Mb, 5: 8Mb, 6: 16Mb
						; 52: 9Mb, 53: 10Mb, 54: 12Mb
;------------------------ RAM SIZE -----------------------------
;149H
	defb	$04			; 0: None or MBC2, 2: 64Kb, 3: 256Kb
;	defb	$00			; 0: None or MBC2, 2: 64Kb, 3: 256Kb
						; 4: 1Mb (planned in future)
;-------------------- DESTINATION CODE -------------------------
;14AH
	defb	$01			; 0: Japan, 1: Other Countries
;	defb	$00			; 0: Japan, 1: Other Countries

;---------------------- FIXED VALUE ----------------------------
;14BH
	defb	$33			; Fixed value
;	defb	$00			; 0: Japan, 1: Other Countries

;--------------------- VERSION NUMBER --------------------------
;14CH
	defb	$00

;-------------------- SUPPLUMENT CHECK -------------------------
;14DH
	defb	$E6			; Sum of registers (134H-14CH) + 25 then
						; take the complement of this number.
						; E7H is used for games in development

;----------------------- CHECK SUM -----------------------------
;14EH~
	defb	$00,$00		; ROM summation for entire program
						; These Bytes must be 00 when summing,
						; then enter final Check Sum.

	end
