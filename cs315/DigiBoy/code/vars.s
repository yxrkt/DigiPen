;---------------------------------------
;	Local Varibles
;---------------------------------------
		org		$c000
		
mode	ds		1		; game mode
bscroll	ds		1		; scrolling down ?
bpart2	ds		1		; part 2 of story ?
height	ds		1		; height from jump
bjump	ds		1		; jumping ?
brface	ds		1		; right facing ?
atimer	ds		1		; animation timer
baiface	ds		1		; beast facing right ?
bfire	ds		1		; fireball out ?
nlives	ds		1		; remaining lives
terrain	ds		41		; max 10 terrains
objects	ds		52		; max 10 objects
