#pragma once

#include <windows.h>

#ifdef CreateWindow
#undef CreateWindow
#endif

class FrameWindow
{
  public:
    FrameWindow( void );
    ~FrameWindow( void );

    bool RegisterWindowClass( WNDCLASSEX &wcex );

    bool CreateWindow( LPCSTR lpWindowName = NULL, DWORD dwStyles = WS_OVERLAPPEDWINDOW,
                       int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, 
                       int w = CW_USEDEFAULT, int h = CW_USEDEFAULT, 
                       HWND hWndParent = NULL, HMENU hMenu = NULL );

  protected:
    static LRESULT CALLBACK sWndProcMain( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    static FrameWindow *pThis;

    virtual LRESULT WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

    HINSTANCE   m_hInstance;
    HWND        m_hWndMain;
    char        m_szCaption[MAX_PATH];

  private:
    WNDCLASSEX  m_wcex;
    char        m_szClassName[MAX_PATH];
};
