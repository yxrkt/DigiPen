//------------------------------------------------------------------------------
#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_
//------------------------------------------------------------------------------

/*******************/
/* ControllerState */
/*******************/

struct ControllerState
{
    char up, down, left, right;
    char b, a;
    char r, l;
    char select, start;
};

/**************/
/* Prototypes */
/**************/

void UpdateController();
struct ControllerState GetControllerState();

#endif  // __CONTROLLER_H_
