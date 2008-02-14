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

enum HIPPY_FACE {FACE_DOWN = 0, FACE_UP = 2, FACE_SIDE = 4};

struct Sprite
{
    char dir;
    char baseFrame;
    char frameDif;
    u8 sprite;
    int x, y;
    int w, h;
};

#endif  // __GAME_H_
