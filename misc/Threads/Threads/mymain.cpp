#include <iostream>
#include <windows.h>
#include <process.h>
#include <conio.h>

int nThreads = 1;

void GetInputQuit( void *pMyID );
void GetInputStart( void *pMyID );

int main()
{
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
    nKeyInfo = _getch();
    std::cout << ( char ) tolower( nKeyInfo );
  }
}

void GetInputQuit( void *pMyID )
{
  int nKeyInfo;

  do {
    nKeyInfo = _getch();
  } while ( tolower( nKeyInfo ) != 'q' );
}
