/*************/
/* Libraries */
/*************/

#include <mygba.h>
#include "Controller.h"

/***********/
/* Globals */
/***********/

static char g_vertBlankState = FALSE;
static int g_vertBlankCount = 0;

/*****************/
/* VerticalBlank */
/*****************/

void VerticalBlank()
{
    // update sprites
    ham_CopyObjToOAM();

    // update controller
    UpdateController();

    // update count
    g_vertBlankCount = (g_vertBlankCount == 60) ? 0 : g_vertBlankCount + 1;

    // signal vertical blank proc done
    g_vertBlankState = TRUE;
}

/*************************/
/* GetVerticalBlankState */
/*************************/

char GetVerticalBlankState()
{
    return g_vertBlankState;
}

/*************************/
/* SetVerticalBlankState */
/*************************/

void SetVerticalBlankState(char state)
{
    g_vertBlankState = state;
}

/*************************/
/* GetVerticalBlankCount */
/*************************/

int GetVerticalBlankCount()
{
    return g_vertBlankCount;
}
