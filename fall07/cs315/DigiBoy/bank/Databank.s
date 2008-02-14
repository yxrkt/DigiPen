; constants
bank00	group 00
		puball
	
	org		$1000
	
mmenu	db		0
mstory	db		1
mgame	db		2
mend	db		3
mlose	db		4
maxh	db		$28
beastlb	db		$70
beastrb	db		$b0

; resources
bank01  group 01
		puball

	org		$4000

menu_tile_data
	lib			data\menu.bg

menu_pal_data
	lib			data\menu.bpl

menu_screen_data
	lib			data\menu.scn


story_tile_data
	lib			data\story.bg

story_pal_data
	lib			data\story.bpl

story_screen_data
	lib			data\story.scn
	
story2_screen_data
	lib			data\story2.scn


game_tile_data
	lib			data\game.bg

game_pal_data
	lib			data\game.bpl

game_screen_data
	lib			data\game.scn


endgame_screen_data
	lib			data\endgame.scn


lose_screen_data
	lib			data\lose.scn


object_data
	lib			data\objects.gbo
