// =============================================================================
#ifndef WINDOWBASE_H
#define WINDOWBASE_H
// =============================================================================

#include <windows.h>

// =============================================================================
// ! Base window class: Contains window handles.
// =============================================================================
class WindowBase
{
  public:
      // Constructor/Destructor
    WindowBase();
    ~WindowBase();

      // Window initializers
    void CreateMainWindow( const char *caption = NULL, int width = CW_USEDEFAULT, 
                           int height = CW_USEDEFAULT, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT );

  protected:
      // Static window procedure used to initialize WNDCLASSEX structure
    static LRESULT CALLBACK sWndProcMain( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    static WindowBase *pThis; // Static 'this' pointer

      // Dummy window procedure
    virtual LRESULT WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

    HINSTANCE hInstance_;   // Handle to current instance
    HWND hWndMain_;         // Handle to main window

    char szDefaultCaption[MAX_PATH], szClassName[MAX_PATH];  // Window captions and class names
};

#endif
