#include <iostream>
#include <windows.h>
#include <process.h>
#include <conio.h>

int nThreads;
HANDLE hConsole;
HANDLE hScreenMutex;

void GetInputQuit( void *pMyID );
void GetInputStart( void *pMyID );

int main()
{
    // console out resource
  hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

    // mutex
  hScreenMutex = CreateMutex( NULL, FALSE, NULL );

    // set thread count
  nThreads = 0;

  _beginthread( GetInputStart, 0, &nThreads );
  nThreads++;
  _beginthread( GetInputQuit, 0, &nThreads );

  while ( 1 ) ;

  return 0;
}

void GetInputStart( void *pMyID )
{
  int nKeyInfo;

  while ( 1 )
  {
    if ( kbhit() )
    {
      WaitForSingleObject( hConsole, INFINITE );
      nKeyInfo = _getch();
      std::cout << ( char ) tolower( nKeyInfo );
      ReleaseMutex( hConsole );
    }
  }
}

void GetInputQuit( void *pMyID )
{
  int nKeyInfo;

  do {
    if ( kbhit() )
    {
      WaitForSingleObject( hConsole, INFINITE );
      nKeyInfo = _getch();
      ReleaseMutex( hConsole );
    }
  } while ( tolower( nKeyInfo ) != 'q' );
}
