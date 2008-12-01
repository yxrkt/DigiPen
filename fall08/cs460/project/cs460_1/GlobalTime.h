#pragma once

#include <windows.h>


class GlobalTime
{
  private:
    static DWORD  lastTick;
    static DWORD  curTick;
    static DWORD  lastStep;
    static DWORD  lastTimeGetTime;
    static bool   paused;

    GlobalTime( void );
    GlobalTime( const GlobalTime &rhs );
    GlobalTime &operator =( const GlobalTime &rhs );

  public:
    static void Update( void )
    {
      if ( !paused )
      {
        lastStep = timeGetTime() - lastTimeGetTime;
        lastTick = curTick;
        curTick  += lastStep;
      }
      else
      {
        lastStep = 0;
      }
      lastTimeGetTime = timeGetTime();
    }

    static void Pause( void )
    {
      paused = true;
    }

    static void Unpause( void )
    {
      paused = false;
    }

    static DWORD GetCurTick( void )
    {
      return curTick;
    }

    static DWORD GetLastTick( void )
    {
      return lastTick;
    }

    static DWORD GetStep( void )
    {
      return lastStep;
    }
};
