#include "CS460Proj.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, INT )
{
  CS460PROJ->Initialize( hInstance );

  int result = CS460PROJ->Run();

  return result;
}
