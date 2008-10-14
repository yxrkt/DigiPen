#include "WindowBase.h"

// =============================================================================
// ! Constructor: Initializes window class names and captions.
// =============================================================================
WindowBase::WindowBase() : hInstance_( NULL ), hWndMain_( NULL )
{
  char szCaptionBuf[] = "Default Caption";
  char szClassNameBuf[] = "CS460WINDOW";
  strcpy_s( szDefaultCaption, sizeof( szCaptionBuf ), szCaptionBuf );
  strcpy_s( szClassName, sizeof( szClassNameBuf ), szClassNameBuf );
}

// =============================================================================
// ! Destructor.
// =============================================================================
WindowBase::~WindowBase() {}

// =============================================================================
// ! Create main window.
// =============================================================================
void WindowBase::CreateMainWindow( const char *caption, int width, int height, int x, int y )
{
  WNDCLASSEX wcex;
  ZeroMemory( &wcex, sizeof( WNDCLASSEX ) );

  wcex.cbSize         = sizeof( WNDCLASSEX );
  wcex.lpfnWndProc    = sWndProcMain;
  wcex.hbrBackground  = (HBRUSH)GetStockObject( LTGRAY_BRUSH );
  wcex.hCursor        = LoadCursor( NULL, IDC_ARROW );
  wcex.hInstance      = hInstance_;
  wcex.lpszClassName  = szClassName;
  wcex.style          = CS_HREDRAW | CS_VREDRAW;

  char *pCaption = caption ? (char *)caption : szDefaultCaption;

  if ( !RegisterClassEx( &wcex ) )
  {
    MessageBox( NULL, "Failed to register class for main window.", szDefaultCaption, MB_ICONERROR );
    return;
  }

  hWndMain_ = CreateWindowEx( NULL, szClassName, pCaption, WS_OVERLAPPEDWINDOW,
                              x, y, width, height, NULL, NULL, wcex.hInstance, this );

  ShowWindow( hWndMain_, SW_SHOW );
}

// =============================================================================
// ! Dummy window procedure.
// =============================================================================
LRESULT WindowBase::WndProc( UINT, WPARAM, LPARAM )
{
  return 0L;
}

// =============================================================================
// ! Static 'this' pointer. Useful for CreateDialog().
// =============================================================================
WindowBase *WindowBase::pThis;

// =============================================================================
// ! Static window procedure.
// =============================================================================
LRESULT CALLBACK WindowBase::sWndProcMain( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  if ( msg == WM_CREATE )
  {
      // Store the static 'this' pointer
    CREATESTRUCT *lpCreateStruct = (CREATESTRUCT *)lParam;
    pThis = (WindowBase *)lpCreateStruct->lpCreateParams;
    pThis->hWndMain_ = hWnd;
    SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG)(LONG_PTR)pThis );
  }

  else
  {
    pThis = (WindowBase *)(LONG_PTR)GetWindowLongPtr( hWnd, GWLP_USERDATA );
    if ( !pThis )
      return DefWindowProc( hWnd, msg, wParam, lParam );
  }

    // Have dummy window procedure call procedure defined in derived class
  return pThis->WndProc( msg, wParam, lParam );
}
