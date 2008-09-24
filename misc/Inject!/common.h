#pragma once

#define ASSERT( expr, msg )\
  if ( !expr ){\
  MessageBox( NULL, msg, "Damn", MB_ICONERROR );\
  exit( -1 );}

enum MESSAGE_TYPE { DATA_PTR, FUNC_PTR };

struct StageData
{
  int width, height;
};
