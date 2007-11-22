//------------------------------------------------------------------------------
#ifndef __GAME_H_
#define __GAME_H_
//------------------------------------------------------------------------------

/***********/
/* Globals */
/***********/
//static unsigned frame;
int g_camera_x = 0, g_camera_y = 0;

/**********/
/* Sprite */
/**********/

struct Sprite
{
    char isLeft;
    char frame;
    u8 sprite;
    int x, y;
};

#endif  // __GAME_H_
