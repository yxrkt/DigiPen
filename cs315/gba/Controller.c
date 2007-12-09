/*************/
/* Libraries */
/*************/

#include <mygba.h>
#include "Controller.h"

/***********/
/* Globals */
/***********/

static struct ControllerState g_controllerState;

/********************/
/* ControllerUpdate */
/********************/

void UpdateController()
{
    // update D-Pad
    g_controllerState.up = (F_CTRLINPUT_UP_PRESSED) ? TRUE : FALSE;
    g_controllerState.down = (F_CTRLINPUT_DOWN_PRESSED) ? TRUE : FALSE;
    g_controllerState.left = (F_CTRLINPUT_LEFT_PRESSED) ? TRUE : FALSE;
    g_controllerState.right = (F_CTRLINPUT_RIGHT_PRESSED) ? TRUE : FALSE;

    // update action buttons
    g_controllerState.b = (F_CTRLINPUT_B_PRESSED) ? TRUE : FALSE;
    g_controllerState.a = (F_CTRLINPUT_A_PRESSED) ? TRUE : FALSE;

    // update shoulder buttons
    g_controllerState.r = (F_CTRLINPUT_R_PRESSED) ? TRUE : FALSE;
    g_controllerState.l = (F_CTRLINPUT_A_PRESSED) ? TRUE : FALSE;

    // update menu buttons
    g_controllerState.select = (F_CTRLINPUT_SELECT_PRESSED) ? TRUE : FALSE;
    g_controllerState.start = (F_CTRLINPUT_START_PRESSED) ? TRUE : FALSE;
}

/**********************/
/* GetControllerState */
/**********************/

struct ControllerState GetControllerState()
{
    return g_controllerState;
}
