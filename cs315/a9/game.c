// standard includes
#include <mygba.h>

// header includes
#include "game.h"
#include "fade.h"
#include "Controller.h"

// resource includes
#include "title.pal"
#include "title.raw"
#include "story1.pal"
#include "story1.raw"
#include "story2.pal"
#include "story2.raw"

// resource externs
extern const unsigned char title_bitmap[38400];
extern const unsigned short title_palette[256];

extern const unsigned char story1_bitmap[38400];
extern const unsigned short story1_palette[256];

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
    
    // move to upper left corner
    g_camera_x = g_camera_y = 0;
    
    // move on to update proc
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
        trans = (controller.start) ? TRUE : FALSE;

        return 0;
    }
}

//**************//
// Game Init    //
//**************//
int GSGame()
{
    // load background (tile mode)
    ham_SetBgMode(0);
    //ham_LoadBGPal((void *) story1_palette, 256);
    //ham_LoadBitmap((void *) story1_bitmap);

    // display background
    //ham_FlipBGBuffer();

    return 1;
}

//**************//
// Game Update  //
//**************//
int GSGameProc()
{
    return 0;
}

//**************//
// Death Init   //
//**************//
int GSDeath()
{
    return 1;
}

//**************//
// Death Update //
//**************//
int GSDeathProc()
{
    return 0;
}
