bank00	group	00

	puball

	lib		code\equ.s
	lib		code\nintendo.s
	lib		code\work.s
	lib		code\vars.s		; variables

	lib		code\main.s		; org $150 bank 0
	lib		code\menu.s
	lib		code\vblank.s
	lib		code\cgb_sub.s
	lib		code\sub.s
	lib		code\story.s
	lib		code\game.s
	lib		code\objects.s
	lib		code\physics.s
	lib		code\ai.s
	lib		code\endgame.s
	lib		code\lose.s
	
	extern	mmenu,mstory,mgame,mend,mlose,maxh,beastlb,beastrb

	extern	menu_tile_data,menu_screen_data,menu_pal_data
	extern	menu_scr_data,menu_attr_data

	extern	story_tile_data,story_screen_data,story_pal_data
	extern	story_scr_data,story2_scr_data,story_attr_data	

	extern	game_tile_data,game_screen_data,game_pal_data
	extern	game_scr_data,game_attr_data
	
	extern	endgame_screen_data,endgame_scr_data,endgame_attr_data
	
	extern	lose_screen_data,lose_scr_data,lose_attr_data
	
	extern	objs_tile_data,objs_pal_data

