#include "FrameWindow.h"

FrameWindow *FrameWindow::pThis;

FrameWindow::FrameWindow()
: m_hInstance( NULL )
, m_hWndMain( NULL )
{
  ZeroMemory( m_szCaption, MAX_PATH );
  strcpy_s( m_szClassName, MAX_PATH, "FrameWindow" );
  ZeroMemory( &m_wcex, sizeof( m_wcex ) );
}

FrameWindow::~FrameWindow( void )
{
}

bool FrameWindow::RegisterWindowClass( WNDCLASSEX &wcex )
{
  if ( !RegisterClassEx( &wcex ) )
  {
    MessageBox( NULL, "Registering window class failed.", wcex.lpszClassName, MB_ICONERROR );
    return false;
  }

  strcpy_s( m_szClassName, MAX_PATH, wcex.lpszClassName );
  m_wcex = wcex;

  return true;
}

bool FrameWindow::CreateWindow( LPCSTR lpWindowName, DWORD dwStyles, int x, int y, 
                                int w, int h, HWND hWndParent, HMENU hMenu )
{
  WNDCLASSEX wcex = { 0 };

  if ( m_wcex.cbSize )
  {
    memcpy( &wcex, &m_wcex, sizeof( wcex ) );
  }
  else
  {
    strcpy_s( m_szClassName, MAX_PATH, "FrameWindow" ); 

    wcex.cbSize        = sizeof( wcex );
    wcex.lpfnWndProc   = sWndProcMain;
    wcex.hbrBackground = (HBRUSH)GetStockObject( LTGRAY_BRUSH );
    wcex.hInstance     = m_hInstance;
    wcex.lpszClassName = m_szClassName;
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.hCursor       = LoadCursor( NULL, IDC_ARROW );

    if ( !RegisterClassEx( &wcex ) )
    {
      MessageBox( NULL, "Failed to register window class.", m_szClassName, MB_ICONERROR );
      return false;
    }
  }

  strcpy_s( m_szCaption, MAX_PATH, lpWindowName );

  m_hWndMain = CreateWindowEx( NULL, m_szClassName, m_szCaption, dwStyles, x, y, w, h, 
                               hWndParent, hMenu, wcex.hInstance, this );

  ShowWindow( m_hWndMain, SW_SHOW );

  return true;
}

LRESULT FrameWindow::WndProc( UINT, WPARAM, LPARAM )
{
  return 0L;
}

LRESULT CALLBACK FrameWindow::sWndProcMain( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  if ( msg == WM_CREATE )
  {
    CREATESTRUCT *lpCreateStruct = (CREATESTRUCT *)lParam;
    pThis = (FrameWindow *)lpCreateStruct->lpCreateParams;
    pThis->m_hWndMain = hWnd;
    SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG)(LONG_PTR)pThis );
  }
  else
  {
    pThis = (FrameWindow *)(LONG_PTR)GetWindowLongPtr( hWnd, GWLP_USERDATA );
    if ( !pThis )
      return DefWindowProc( hWnd, msg, wParam, lParam );
  }

  return pThis->WndProc( msg, wParam, lParam );
}
