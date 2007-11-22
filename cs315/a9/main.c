/*************/
/* Libraries */
/*************/

#include <mygba.h>
#include "VerticalBlank.h"

/***************/
/* Game States */
/***************/

#define GS_INIT             0
#define GS_TITLE            1
#define GS_TITLE_PROC       2
#define GS_STORY            3
#define GS_STORY_PROC       4
#define GS_GAME             5
#define GS_GAME_PROC        6
#define GS_DEATH            7
#define GS_DEATH_PROC       8

/**************/
/* Prototypes */
/**************/

void GameProc();

int GSTitle();
int GSTitleProc();

int GSStory();
int GSStoryProc();

int GSGame();
int GSGameProc();

int GSDeath();
int GSDeathProc();

/********/
/* main */
/********/

int main(void)
{
    ham_Init();

    ham_StartIntHandler(INT_TYPE_VBL, (void *) &VerticalBlank);
    GameProc();

    return 0;
}

/************/
/* GameProc */
/************/

void GameProc()
{
    int gameState = 0;

    // inifinite game loop
    while (TRUE)
    {
        // wait for a vertical blank
        while (!GetVerticalBlankState());

        // mark vertical blank use
        SetVerticalBlankState(FALSE);

        // perform correct game proc.
        switch (gameState)
        {
            case GS_INIT:
            ++gameState;
            break;

            case GS_TITLE:
            gameState += GSTitle();
            break;

            case GS_TITLE_PROC:
            gameState += GSTitleProc();
            break;

            case GS_STORY:
            gameState += GSStory();
            break;

            case GS_STORY_PROC:
            gameState += GSStoryProc();
            break;

            case GS_GAME:
            gameState += GSGame();
            break;

            case GS_GAME_PROC:
            gameState += GSGameProc();
            break;

            case GS_DEATH:
            gameState += GSDeath();
            break;

            case GS_DEATH_PROC:
            gameState += GSDeathProc();

            default:
            break;
        }

        // reset game state if needed
        if (gameState > GS_DEATH_PROC)
        {
            gameState = GS_TITLE;
        }
    }
}

/* END OF FILE */
