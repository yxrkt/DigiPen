#pragma once

#include "FrameWindow.h"
#include "Cloth.h"
#include "Sphere.h"

#define CS460PROJ CS460Proj::Instance()

class CS460Proj : FrameWindow
{
  public:
    static CS460Proj *Instance( void );

    void Initialize( HINSTANCE hInstance );
    int Run( void );
    HWND GetWindow( void );

  private:
    CS460Proj( void );
    CS460Proj( const CS460Proj &rhs );
    void operator =( const CS460Proj &rhs );
    ~CS460Proj( void );

    void Update( void );
    LRESULT WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

  private:
    Cloth  *pCloth;
    Sphere *pSphere;
};
