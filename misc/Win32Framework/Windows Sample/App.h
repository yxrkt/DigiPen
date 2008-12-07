#pragma once

#include "FrameWindow.h"

class App : FrameWindow
{
  public:
    App( HINSTANCE hInstance );
    ~App( void );

    int Run( void );

  private:
    void Initialize( void );
    void Update( void );
    LRESULT WndProc( UINT msg, WPARAM wParam, LPARAM lParam );
};
