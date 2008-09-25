#include <windows.h>
#include <sstream>

#include "common.h"


StageData *g_pStageData;

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

int APIENTRY WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
  WNDCLASSEX wcex;
  memset( &wcex, 0, sizeof( wcex ) );
  wcex.cbSize         = sizeof( WNDCLASSEX );
  wcex.lpfnWndProc    = &WndProc;
  wcex.hbrBackground  = (HBRUSH)GetStockObject( WHITE_BRUSH );
  wcex.hCursor        = LoadCursor( NULL, IDC_ARROW );
  wcex.hInstance      = hInst;
  wcex.style          = CS_HREDRAW | CS_VREDRAW;
  wcex.lpszClassName  = "patient";
  ASSERT( RegisterClassEx( &wcex ), "creating patient window class failed =(" );

  HWND hWnd = CreateWindowEx( 0, "patient", "patient", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
                              CW_USEDEFAULT, 256, 256, NULL, NULL, hInst, NULL );

  ShowWindow( hWnd, SW_SHOW );

  MSG msg;
  memset( &msg, 0, sizeof( msg  ) );
  while ( msg.message != WM_QUIT )
  {
    if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
    Sleep( 1 );

    if ( g_pStageData )
    {
      if ( g_pStageData->width == 666 )
        MessageBox( NULL, "waesome", "waesome", MB_OK );
    }
  }

  return (int)msg.wParam;
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
      //MessageBox( NULL, stageInfo.str().c_str(), "Success!", MB_ICONINFORMATION );
    }
    break;

    case WM_APP + 1:
    {
      ( (StageData *)lParam )->width    = 100;
      ( (StageData *)lParam )->height   = 100;
      ( (StageData *)lParam )->changed  = true;
    }
    break;

    case WM_CLOSE:
      PostQuitMessage( 0 );
    break;

    default:
      return DefWindowProc( hWnd, msg, wParam, lParam );
  }

  return 0L;
}
