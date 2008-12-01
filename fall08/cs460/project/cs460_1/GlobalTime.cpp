#include "GlobalTime.h"


DWORD GlobalTime::lastTick          = 0;
DWORD GlobalTime::curTick           = 0;
DWORD GlobalTime::lastStep          = 0;
DWORD GlobalTime::lastTimeGetTime   = 0;
bool  GlobalTime::paused            = false;

