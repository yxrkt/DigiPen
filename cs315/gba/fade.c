/*************/
/* Libraries */
/*************/

#include <mygba.h>

/***********/
/* Globals */
/***********/

static char g_fadeCount = 0;

/**************/
/* FadeBGInit */
/**************/

void FadeBGInit()
{
    g_fadeCount = 0;
}

/**********/
/* FadeBG */
/**********/

char FadeBG()
{
    if (g_fadeCount % 2 == 0)
    {
        ham_FadePal(0, 1);
    }

    ++g_fadeCount;

    return (g_fadeCount == 60) ? 1 : 0;
}
