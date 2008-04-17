#include <windows.h>
#include <string>

int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR site, int )
{
  while ( true )
  {
    if ( GetAsyncKeyState( VK_RETURN ) & 0x8000 )
      system( ( std::string( "explorer \"" ) + site + "\"" ).c_str() );
    Sleep( 10 );
  }

  return 0;
}
