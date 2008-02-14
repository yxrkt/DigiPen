AS = isas32 -nologo -w3 -g -isdmg
LK = islk32 -nologo -w bank-over

BINS =	project.isx
OBJS =	Codebank.o DataBank.o
CODES =		code\main.s\
			code\vblank.s\
			code\work.s\
			code\equ.s\
			code\vars.s\
			code\sub.s\
			code\nintendo.s\
			code\cgb_sub.s\
			code\menu.s\
			code\game.s\
			code\story.s\
			code\objects.s\
			code\physics.s\
			code\ai.s\
			code\endgame.s\
			code\lose.s\
			code\hblank.s


DATAS = 
all:	$(BINS)

CodeBank.o:	bank\CodeBank.s $(CODES)
	$(AS) bank\CodeBank.s

DataBank.o:	bank\DataBank.s $(DATAS)
	$(AS) bank\DataBank.s

$(BINS):	$(OBJS)
	$(LK) -o	project 	CodeBank.o	DataBank.o