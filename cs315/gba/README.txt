Alex Serio
Assignment 10
CS 315A

Features implemented

1. Object rotation and scaling (fireball)

   First, a rotation set is created and enabled for 
   the fireball:

     ham_SetObjRotSetSelect(g_fireBall.sprite, ROTATION_SET);
     ham_SetObjRotEnable(g_fireBall.sprite, 1);

   Then, for every frame, the angle is increased and the
   scaling for the x and y is either increased or decreased:

     static bool dec = FALSE;
     static unsigned angle = 0, scale = 0x100;
    
     if (scale <= 0x100)
        dec = FALSE;
     else if (scale >= 0x200)
        dec = TRUE;
        
     angle = ((angle + 4) % 360);
     scale += dec ? -0x08 : 0x08;

     ham_SetObjXY(g_fireBall.sprite, g_fireBall.x, g_fireBall.y);
     ham_RotObjDefineSet(ROTATION_SET, angle, scale, scale);



2. Alpha blend (objects on background)

   First, the special effects mode is set for alphablending:

     ham_SetFxMode(FX_LAYER_SELECT(0, 0, 0, 0, 1, 0), 
                   FX_LAYER_SELECT(1, 0, 0, 0, 0, 0), 
                   FX_MODE_ALPHABLEND);

   This will blend the objects into the first (and only) 
   background. Now the intensity is set:

     ham_SetFxAlphaLevel(15, 8);

   This gives the objects about 75% opacity.

