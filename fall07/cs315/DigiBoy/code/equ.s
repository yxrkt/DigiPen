;***************************************************************
;	register address
;***************************************************************
P1			EQU	$FF00		; Keyboard
SC			EQU	$FF02		; Serial Control
DIV			EQU	$FF04		; Divider Register
TIMA		EQU	$FF05		; Timer Register
TMA			EQU	$FF06		; Modulo Register
TAC			EQU	$FF07		; Timer Control
IF			EQU	$FF0F		; Interrupt Request Flag
IE			EQU	$FFFF		; Interrupt Enbale Flag
LCDC		EQU	$FF40		; LCD Control
STAT		EQU	$FF41		; LCD Status
SCY			EQU	$FF42		; Scroll X
SCX			EQU	$FF43		; Scroll Y
LY			EQU	$FF44		; LCDC Y Coordinate
LYC			EQU	$FF45		; LY Compare Register
DMA			EQU	$FF46		; DMA Transfer
BGP			EQU	$FF47		; BG Palatte
OBP0		EQU	$FF48		; OBJ Palatte 0
OBP1		EQU	$FF49		; OBJ Palatte 1
WY			EQU	$FF4A		; Window X Coordinate
WX			EQU	$FF4B		; Window Y Coordinate
KEY1		EQU	$FF4D		; CPU Speed Switch
VBK			EQU	$FF4F		; VRAM Bank Specify
HDMA1		EQU	$FF51		; New (H-BLANK, Pan-Use) DMA Specify Transfer Begins (Upper Bits)
HDMA2		EQU	$FF52		; New (H-BLANK, Pan-Use) DMA Specify Transfer Begins (Lower Bits)
HDMA3		EQU	$FF53		; New (H-BLANK, Pan-Use) DMA Specify Transfer Ends (Upper Bits)
HDMA4		EQU	$FF54		; New (H-BLANK, Pan-Use) DMA Specify Transfer Ends (Lower Bits)
HDMA5		EQU	$FF55		; New (H-BLANK, Pan-Use) DMA Transfer Data And Transfer Flager
RP			EQU	$FF56		; Wireless IR Communication Port
BCPS		EQU	$FF68		; BG Color Writing Specification
BCPD		EQU	$FF69		; BG Color Writing Data
OCPS		EQU	$FF6A		; OBJ Color Writing Specification
OCPD		EQU	$FF6B		; OBJ Color Writing Data
SVBK		EQU	$FF70		; WRAM Bank Specification
CGBFLAG		EQU	$FFFE

;***************************************************************
;	dma
;***************************************************************
DMASUB		EQU	$FF80		; DMA transfer sub-routine
OBJRAM		EQU	$C000		; OAM

; MBC5 Bank Switch Register (Write Only)
; Depends on the different Type of board, comfirm each memory control according the manuel

;***************************************************************
;	memory
;***************************************************************
RAMG		EQU	$0000		;Expanded Memory Setting Register (0000--1FFF)
							;$00: can't access RAM(Data Protect), $0A: can
ROMB0		EQU	$2000		;ROM Bank Lower Bits Register
ROMB1		EQU	$3000		;ROM Bank Upper Bits Register
RAMB		EQU	$4000		;RAM Bank Register

;********************************
;*	Stack   					*
;********************************

STACKPOINTER	EQU	$FFFF

;********************************
;*	Controller 					*
;********************************
; Note:  With these definitions, you don't need to which bit is high
;        depending on the button pressed.

BitA				EQU	$00
BitB				EQU	$01
BitSelect			EQU	$02
BitStart			EQU	$03
BitRight			EQU	$04
BitLeft				EQU	$05
BitUp				EQU	$06
BitDown				EQU	$07


	END
