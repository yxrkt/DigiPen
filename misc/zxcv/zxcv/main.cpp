#include <iostream>
#include <windows.h>
#include <process.h>
#include <conio.h>

void threadProc1( void *pMyID )
{
  do {
  } while ( !( GetAsyncKeyState( 'Q' ) & 0x8000 ) );

  std::cout << "thread1 terminated" << std::endl;

  exit( 0 );
}

int main()
{
  _beginthread( threadProc1, 0, NULL );

  int key;
  do {
    key = _getch();
  } while ( (char) key != 'x' );

  return 0;
};
