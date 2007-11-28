// standard includes
#include <mygba.h>

// header includes
#include "game.h"
#include "fade.h"
#include "Controller.h"
#include "VerticalBlank.h"

// resource includes
#include "title.pal"
#include "title.raw"
#include "story1.pal"
#include "story1.raw"
#include "world.pal"
#include "world.raw"
#include "world.map"
#include "hippy.pal"
#include "hippy.raw"
#include "shroom.pal"
#include "shroom.raw"
#include "win.pal"
#include "win.raw"
#include "death.pal"
#include "death.raw"
#include "lives.pal"
#include "lives.raw"
#include "key.pal"
#include "key.raw"
#include "fire.pal"
#include "fire.raw"

// constants
const int WALK_SPEED    = 1;
const int HIPPY_FRAMES  = 2;
const int HIPPY_TPF     = 14;
const int SCREEN_WIDTH  = 240;
const int SCREEN_HEIGHT = 160;
const int MAP_WIDTH     = 512;
const int MAP_HEIGHT    = 512;

// vars
int g_cam_x      = 0;
int g_cam_y      = 0;
unsigned g_count = 0;
int g_lives      = 5;
bool g_initObjs  = TRUE;
bool g_firing    = FALSE;

struct Sprite g_hippy;
struct Sprite g_shroom;
struct Sprite g_key;
struct Sprite g_lifeCount;
struct Sprite g_fireBall;

// forward decls
void Move();
void HScroll(int shift);
void VScroll(int shift);
bool CheckCollision(const struct Sprite *lhs, const struct Sprite *rhs);
int CollisionResult();
void RunAI();

//************//
// Title Init //
//************//
int GSTitle()
{
    // load background
    ham_SetBgMode(4);
    ham_LoadBGPal((void *)title_palette, 256);
    ham_LoadBitmap((void *) title_bitmap);
    
    // display background
    ham_FlipBGBuffer();
    FadeBGInit();
    
    // set obj creation bool to true
    //g_initObjs = TRUE;
    
    // set no. of lives
    g_lives = 5;

    return 1;
}

//**************//
// Title Update //
//**************//
int GSTitleProc()
{
    static int trans = FALSE;
    
    if (trans)
    {
        int done = FadeBG();
        trans = (done) ? FALSE : TRUE;
        return done;
    }
    
    else
    {
        // get controller state
        struct ControllerState controller = GetControllerState();
        
        // begin fading transition if start is pressed
        trans = (controller.start) ? TRUE : FALSE;
        
        return 0;
    }
}

//************//
// Story Init //
//************//
int GSStory()
{
    // load background
    ham_SetBgMode(4);
    ham_LoadBGPal((void *) story1_palette, 256);
    ham_LoadBitmap((void *) story1_bitmap);
    
    // display background
    ham_FlipBGBuffer();
    FadeBGInit();
    
    return 1;
}

//**************//
// Story Update //
//**************//
int GSStoryProc()
{
    static int trans = FALSE;

    if (trans)
    {
        int done = FadeBG();
        trans = (done) ? FALSE : TRUE;
        return done;
    }

    else
    {
        // get controller state
        struct ControllerState controller = GetControllerState();

        // begin fading transition if start is pressed
        trans = (controller.a) ? TRUE : FALSE;

        return 0;
    }
}

//**************//
// Game Init    //
//**************//
int GSGame()
{
    map_fragment_info_ptr bg_world;
    
    // setup background
    if ( g_initObjs )
    {
        ham_SetBgMode(1);
        ham_LoadBGPal((void *)world_palette, 256);
        ham_bg[0].ti = ham_InitTileSet((void *)world_tiles, SIZEOF_16BIT(world_tiles), 1, 1);
        ham_bg[0].mi = ham_InitMapEmptySet(3, 0);
        bg_world = ham_InitMapFragment((void *)world_map, 64, 64, 0, 0, 64, 64, 0);
        ham_InsertMapFragment(bg_world, 0, 0, 0);
        ham_InitBg(0, 1, 0, 0);
    }

    // set screen pos
    g_cam_x = 0;
    g_cam_y = 0;

    // setup hippy
    g_hippy.dir       = FACE_DOWN;
    g_hippy.baseFrame = 0;
    g_hippy.frameDif  = 0;
    g_hippy.w         = 32;
    g_hippy.h         = 64;
    g_hippy.x         = SCREEN_WIDTH / 4;
    g_hippy.y         = SCREEN_HEIGHT / 2 - g_hippy.h / 2;
    if ( g_initObjs )
    {
        ham_LoadObjPal((void *)hippy_palette, 256);
        g_hippy.sprite = ham_CreateObj((void *)&hippy_bitmap[0], OBJ_SIZE_32X64, OBJ_MODE_NORMAL, 1, 0, 0, 0, 0, 0, 0, g_hippy.x, g_hippy.y);
    }
    
    // setup poison shroom
    g_shroom.dir       = FACE_DOWN;
    g_shroom.baseFrame = 0;
    g_shroom.frameDif  = 0;
    g_shroom.x         = 230;
    g_shroom.y         = 75;
    g_shroom.w         = 32;
    g_shroom.h         = 32;
    if ( g_initObjs )
    {
        //ham_LoadObjPal((void *)shroom_palette, 256);
        g_shroom.sprite = ham_CreateObj((void *)&shroom_bitmap[0], OBJ_SIZE_32X32, OBJ_MODE_NORMAL, 1, 0, 0, 0, 0, 0, 0, g_shroom.x, g_shroom.y);
    }
    
    // setup key
    g_key.dir       = FACE_DOWN;
    g_key.baseFrame = 0;
    g_key.frameDif  = 0;
    g_key.x         = 262;
    g_key.y         = 75;
    g_key.w         = 16;
    g_key.h         = 16;
    if ( g_initObjs )
    {
        //ham_LoadObjPal((void *)hippy_palette, 256);
        g_key.sprite = ham_CreateObj((void *)&key_bitmap[0], OBJ_SIZE_16X16, OBJ_MODE_NORMAL, 1, 0, 0, 0, 0, 0, 0, g_key.x, g_key.y);
    }
    
    // setup lives
    g_lifeCount.dir       = FACE_DOWN;
    g_lifeCount.baseFrame = g_lives;
    g_lifeCount.frameDif  = 0;
    g_lifeCount.x         = 0;
    g_lifeCount.y         = 0;
    g_lifeCount.w         = 16;
    g_lifeCount.h         = 16;
    if ( g_initObjs )
    {
        //ham_LoadObjPal((void *)hippy_palette, 256);
        g_lifeCount.sprite = ham_CreateObj((void *)&lives_bitmap[0], OBJ_SIZE_16X16, OBJ_MODE_NORMAL, 1, 0, 0, 0, 0, 0, 0, g_lifeCount.x, g_lifeCount.y);
        ham_UpdateObjGfx(g_lifeCount.sprite, (void *)&lives_bitmap[g_lifeCount.w * g_lifeCount.h * g_lives]);
    }
    
    // setup fireball
    g_fireBall.dir       = FACE_DOWN;
    g_fireBall.baseFrame = 0;
    g_fireBall.frameDif  = 0;
    g_fireBall.x         = 241;
    g_fireBall.y         = 0;
    g_fireBall.w         = 16;
    g_fireBall.h         = 16;
    if ( g_initObjs )
    {
        g_fireBall.sprite = ham_CreateObj((void *)&fire_bitmap[0], OBJ_SIZE_16X16, OBJ_MODE_NORMAL, 1, 0, 0, 0, 0, 0, 0, g_fireBall.x, g_fireBall.y);
    }
    g_firing = FALSE;

    g_initObjs = FALSE;
    return 1;
}

//**************//
// Game Update  //
//**************//
int GSGameProc()
{
    Move();
    
    RunAI();

    return CollisionResult();
}

//**************//
// Death Init   //
//**************//
int GSDeath()
{
    // load background
    ham_SetBgMode(4);
    ham_LoadBGPal((void *)death_palette, 256);
    ham_LoadBitmap((void *)death_bitmap);

    // display background
    ham_FlipBGBuffer();
    FadeBGInit();
    
    return 1;
}

//**************//
// Death Update //
//**************//
int GSDeathProc()
{
    static int trans = FALSE;

    if (trans)
    {
        int done = FadeBG();
        trans = (done) ? FALSE : TRUE;
        return (done * 10);
    }

    else
    {
        // get controller state
        struct ControllerState controller = GetControllerState();

        // begin fading transition if start is pressed
        trans = (controller.start) ? TRUE : FALSE;

        return 0;
    }
}

//**************//
// Win Init     //
//**************//
int GSWin()
{
    // load background
    ham_SetBgMode(4);
    ham_LoadBGPal((void *)win_palette, 256);
    ham_LoadBitmap((void *)win_bitmap);

    // display background
    ham_FlipBGBuffer();
    FadeBGInit();
    
    return 1;
}

//**************//
// Win Update   //
//**************//
int GSWinProc()
{
    static int trans = FALSE;

    if (trans)
    {
        int done = FadeBG();
        trans = (done) ? FALSE : TRUE;
        return done;
    }

    else
    {
        // get controller state
        struct ControllerState controller = GetControllerState();

        // begin fading transition if start is pressed
        trans = (controller.start) ? TRUE : FALSE;

        return 0;
    }
}


//******************//
// Helper Functions //
//******************//
void Move()
{
    struct ControllerState cont = GetControllerState();
    bool moving = FALSE;
    
    // move player
    if ( cont.left  )
    {
        if ( g_cam_x > 0 && ((g_hippy.x + g_hippy.w / 2) == (SCREEN_WIDTH / 2)) )
            HScroll(-WALK_SPEED);
        else if ( g_hippy.x > 0 )
            g_hippy.x -= WALK_SPEED;

        g_hippy.baseFrame = FACE_SIDE;
        ham_SetObjHFlip(g_hippy.sprite, FALSE);
        moving = TRUE;
    }
    if ( cont.right  )
    {
        if ( g_cam_x < (MAP_WIDTH - SCREEN_WIDTH - 1) && ((g_hippy.x + g_hippy.w / 2) == (SCREEN_WIDTH / 2)) )
            HScroll(WALK_SPEED);
        else if ( g_hippy.x < (SCREEN_WIDTH - g_hippy.w) )
            g_hippy.x += WALK_SPEED;

        if ( !moving )
        {
            g_hippy.baseFrame = FACE_SIDE;
            ham_SetObjHFlip(g_hippy.sprite, TRUE);
            moving = TRUE;
        }
    }
    if ( cont.up  )
    {
        if ( g_cam_y > 0 && ((g_hippy.y + g_hippy.h / 2) == (SCREEN_HEIGHT / 2)) )
            VScroll(-WALK_SPEED);
        else if ( g_hippy.y > 0 )
            g_hippy.y -= WALK_SPEED;

        if ( !moving )
        {
            g_hippy.baseFrame = FACE_UP;
            moving = TRUE;
        }
    }
    if ( cont.down  )
    {
        if ( g_cam_y < (MAP_HEIGHT - SCREEN_HEIGHT - 1) && ((g_hippy.y + g_hippy.h / 2) == (SCREEN_HEIGHT / 2)) )
            VScroll(WALK_SPEED);
        else if ( g_hippy.y < (SCREEN_HEIGHT - g_hippy.h) )
            g_hippy.y += WALK_SPEED;

        if ( !moving )
        {
            g_hippy.baseFrame = FACE_DOWN;
            moving = TRUE;
        }
    }

    // move player
    ham_SetBgXY(0, g_cam_x, g_cam_y);
    ham_SetObjXY(g_hippy.sprite, g_hippy.x, g_hippy.y);
    
    // move shroom
    ham_SetObjXY(g_shroom.sprite, g_shroom.x, g_shroom.y);
    
    // move key
    ham_SetObjXY(g_key.sprite, g_key.x, g_key.y);
    
    // set hippy frame
    int animIndex = g_hippy.w * g_hippy.h * (g_hippy.baseFrame + g_hippy.frameDif);
    ham_UpdateObjGfx(g_hippy.sprite, (void *)&hippy_bitmap[animIndex]);
    
    g_count++;
    if ( moving && ((g_count % HIPPY_TPF) == 0) )
        g_hippy.frameDif =(g_hippy.frameDif + 1) % HIPPY_FRAMES;
}

void HScroll(int shift)
{
    g_cam_x += shift;
    g_shroom.x -= shift;
    g_key.x -= shift;
    
    if (g_firing)
        g_fireBall.x -= shift;
}

void VScroll(int shift)
{
    g_cam_y += shift;
    g_shroom.y -= shift;
    g_key.y -= shift;
    
    if (g_firing)
        g_fireBall.y -= shift;
}

bool CheckCollision(const struct Sprite *lhs, const struct Sprite *rhs)
{
    if ( lhs->x + lhs->w < rhs->x )
        return FALSE;
    if ( lhs->x > rhs->x + rhs->w )
        return FALSE;
    if ( lhs->y + lhs->h < rhs->y )
        return FALSE;
    if ( lhs->y > rhs->y + rhs->h )
        return FALSE;

    return TRUE;
}

int CollisionResult()
{
    // collision with bad shroom
    if ( CheckCollision(&g_hippy, &g_shroom) || CheckCollision(&g_hippy, &g_fireBall) )
    {
        g_lives--;
        if (g_lives < 0)
        {
            //ham_DeleteObj(g_hippy.sprite);
            //ham_DeleteObj(g_shroom.sprite);
            return 1;
        }
        else
        {
            // set life counter frame
            ham_UpdateObjGfx(g_lifeCount.sprite, (void *)&lives_bitmap[g_lifeCount.w * g_lifeCount.h * g_lives]);
            return -1;
        }
    }
    
    // collision with key shroom
    if ( CheckCollision(&g_hippy, &g_key) )
    {
        ham_DeleteObj(g_hippy.sprite);
        ham_DeleteObj(g_shroom.sprite);
        ham_DeleteObj(g_key.sprite);
        return 3;
    }

    return 0;
}

void RunAI()
{
    // fireball
    if ( (g_count % 100) && !g_firing )
    {
        g_fireBall.x = g_shroom.x - g_fireBall.w;
        g_fireBall.y = g_shroom.y;
        g_firing = TRUE;
    }
    
    if ( g_firing )
    {
        g_fireBall.x -= WALK_SPEED;
        if ( g_fireBall.x < 0 )
        {
            g_fireBall.x = 241;
            g_firing = FALSE;
        }
    }
    
    // update positions
    ham_SetObjXY(g_fireBall.sprite, g_fireBall.x, g_fireBall.y);
}


