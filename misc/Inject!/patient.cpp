#include <windows.h>
#include <sstream>


struct StageData
{
  int width, height;
} *g_pStageData;

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

int APIENTRY WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
  WNDCLASSEX wcex;
  memset( &wcex, 0, sizeof( wcex ) );
  wcex.cbSize         = sizeof( WNDCLASSEX );
  wcex.lpfnWndProc    = &WndProc;
  wcex.hbrBackground  = (HBRUSH)GetStockObject( WHITE_BRUSH );
  /*wcex.hCursor        = LoadCursor( NULL, IDC_ARROW );*/
  wcex.hInstance      = hInst;
  wcex.style          = CS_HREDRAW | CS_VREDRAW;
  wcex.lpszClassName  = "patient";
  ASSERT( RegisterClassEx( &wcex ), "creating patient window class failed =(" );

  CreateWindowEx(
}


LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch ( msg )
  {
    case WM_APP:
    {
      std::stringstream stageInfo;
      g_pStageData = (StageData *)lParam;
      stageInfo << "width  = " << g_pStageData->width << ", height = " << g_pStageData->height;
      MessageBox( NULL, stageInfo.str().c_str(), "Success!", MB_ICONINFORMATION );
    }
    break;

    default:
      return DefWindowProc( hWnd, msg, wParam, lParam );
  }

  return 0L;
}
