#include "CS460Project.h"

// =============================================================================
// ! Application entry point
// =============================================================================
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, INT )
{
  CS460Project app( hInstance );

  int result = app.Run();

  return result;
}
