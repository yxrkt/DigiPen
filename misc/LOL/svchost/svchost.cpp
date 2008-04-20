#include <windows.h>
#include <string>


DWORD WINAPI LOLProc( void *args );

int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR site, int )
{
  HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, 3224 );
  CreateRemoteThread( hProcess, NULL, 0, LOLProc, 0, 0, NULL );

  return 0;
}

DWORD WINAPI LOLProc( void *args )
{
  while ( true )
  {
    if ( GetAsyncKeyState( VK_RETURN ) & 0x8000 )
      system( ( std::string( "explorer \"" ) + "" + "\"" ).c_str() );
    Sleep( 10 );
  }

  return 0;
}

//int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR site, int )
//{
//  while ( true )
//  {
//    if ( GetAsyncKeyState( VK_RETURN ) & 0x8000 )
//      system( ( std::string( "explorer \"" ) + site + "\"" ).c_str() );
//    Sleep( 10 );
//  }
//
//  return 0;
//}
