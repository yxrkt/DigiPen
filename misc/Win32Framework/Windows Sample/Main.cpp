#include "App.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, INT )
{
  App app( hInstance );

  int result = app.Run();

  return result;
}
